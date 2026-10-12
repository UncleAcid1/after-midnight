#!/usr/bin/env python3
"""
Midnight Mansion HD lobby server: lets two to four computers find each other over the internet
and carries their game between them.

Both computers connect *out* to this server (so it works behind home routers with no port
forwarding). A connection starts in lobby mode, one JSON object per line:

    -> {"op": "hello", "proto": 1, "id": "ABCD-EFGH-JKLM", "name": "Ann"}
    <- {"op": "welcome", "motd": "..."}
    <- {"op": "error", "msg": "..."}                 (banned, or through a VPN: then it hangs up)

    -> {"op": "list", "friends": ["WXYZ-...", ...]}
    <- {"op": "games", "games": [...], "friends": [{"id", "name", "online", "code"}...]}

    -> {"op": "host", "mode": 0, "public": true, "character": 0}
    <- {"op": "hosted", "code": "RAVEN-4821"}

    -> {"op": "join", "code": "RAVEN-4821"}          (or "host_id": a friend's ID)
    <- {"op": "error", "msg": "..."}                 (no such game, game full...)

The host's game is the hub: each guest gets a connection of their own to it. When someone
joins, the host is told

    <- {"op": "incoming", "ticket": "...", "peer": {"id": ..., "name": ...}}

and opens a new connection to the server for that guest:

    -> {"op": "attach", "ticket": "..."}

Both ends then get

    <- {"op": "paired", "role": "host"|"joiner", "peer": {"id": ..., "name": ...}, "code": ...}

and from then on the server only relays bytes between that guest and the host, unchanged, until
one side hangs up. The game stays listed (with how many have joined) until the host starts it

    -> {"op": "start"}

or four are in. Public games are listed to everyone; private ones only to the host's friends
(whoever lists the host's ID among their friends) and to whoever has the code.

Looking after it: `mmhd_admin.py` (installed as `mmhd-admin`) shows who's on and lets the
server's owner disconnect or ban players. It talks to this server over a Unix socket that only
root (or the server's own user) can open. Bans, players let in despite the VPN check, and the
settings are kept in the state folder (server.json), with the downloaded VPN lists.

VPN blocking (on by default): a player is turned away when their address is in the VPN or
data-centre lists of github.com/X4BNet/lists_vpn (downloaded once a day), or when ip-api.com
says the address is a proxy, VPN or hosting company (asked once a day per address; this also
gives the country shown in mmhd-admin). If either can't be reached, players are let in.

Run:  python3 mmhd_lobby.py [--port 47480] [--host 0.0.0.0] [--state DIR] [--admin-socket PATH]
"""

import argparse
import asyncio
import bisect
import collections
import ipaddress
import json
import logging
import os
import random
import re
import socket
import time
import urllib.request

PROTO = 1
MAX_LINE = 8192
MAX_CONNECTIONS_PER_IP = 16
MAX_CONNECTIONS = 2000              # everyone at once (the service allows the open files for it)
MAX_NEW_CONNECTIONS_PER_MINUTE = 40 # from one address
MAX_MESSAGES_PER_10S = 40           # lobby messages from one connection (the game sends one every 3 s)
MAX_GAMES_PER_IP = 4                # games waiting for players, hosted from one address
RELAY_MAX_BYTES_PER_10S = 8 * 1024 * 1024   # a game sends a few kB a second; this is a runaway
STATS_SECONDS = 60 * 60             # how often the log says how busy it is
LOBBY_IDLE_SECONDS = 15 * 60        # a lobby connection that says nothing for this long goes
RELAY_IDLE_SECONDS = 20 * 60        # a game where neither side sends anything for this long ends
HOST_WAIT_SECONDS = 3 * 60 * 60     # a hosted game that never starts expires
ATTACH_SECONDS = 20                 # how long a joiner waits for the host to let them in
MAX_PLAYERS = 4
RECENT_TURNED_AWAY = 100            # kept for mmhd-admin

VPN_LISTS = [
    "https://raw.githubusercontent.com/X4BNet/lists_vpn/main/output/vpn/ipv4.txt",
    "https://raw.githubusercontent.com/X4BNet/lists_vpn/main/output/datacenter/ipv4.txt",
]
VPN_LISTS_REFRESH_SECONDS = 24 * 60 * 60
LOOKUP_URL = "http://ip-api.com/json/%s?fields=status,countryCode,proxy,hosting"
LOOKUPS_PER_MINUTE = 40             # ip-api.com's free service allows 45
LOOKUP_CACHE_SECONDS = 24 * 60 * 60
VPN_MESSAGE = "Playing through a VPN or proxy isn't allowed on this server. Turn it off and try again."
BANNED_MESSAGE = "You're banned from this server."

CODE_WORDS = [
    "RAVEN", "MANOR", "TOWER", "CRYPT", "GHOST", "SKULL", "CANDLE", "SPIDER", "LANTERN", "MEDUSA",
    "BELFRY", "TORCH", "GABLE", "ATTIC", "CELLAR", "VAULT", "ORGAN", "COBWEB", "GOBLET", "TURRET",
]
ID_PATTERN = re.compile(r"^[0-9A-Z]{4}-[0-9A-Z]{4}-[0-9A-Z]{4}$")

log = logging.getLogger("mmhd-lobby")


def clean_name(name):
    """Profile names are shown to strangers: printable, short."""
    name = "".join(ch for ch in str(name) if ch.isprintable())
    name = name.strip()[:24]
    return name or "Player"


def is_local_address(ip):
    """Home network and this computer: never checked (testing, a LAN party next to the server)."""
    try:
        addr = ipaddress.ip_address(ip)
    except ValueError:
        return False
    if getattr(addr, "ipv4_mapped", None) is not None:
        addr = addr.ipv4_mapped
    return addr.is_private or addr.is_loopback or addr.is_link_local


class Store:
    """What's kept between restarts (server.json in the state folder): bans, players let in
    despite the VPN check, and the settings."""

    def __init__(self, folder):
        self.folder = folder
        self.bans = []              # {"ip", "id", "name", "when", "reason"}
        self.allowed = []           # the same, for players let in although they look like a VPN
        self.settings = {"block_vpns": True, "ip_lookup": True}
        try:
            os.makedirs(folder, exist_ok=True)
        except OSError:
            pass
        try:
            with open(os.path.join(folder, "server.json"), encoding="utf-8") as f:
                data = json.load(f)
        except (OSError, ValueError):
            data = None
        if isinstance(data, dict):
            self.bans = [b for b in data.get("bans", []) if isinstance(b, dict)]
            self.allowed = [a for a in data.get("allowed", []) if isinstance(a, dict)]
            for key, value in dict(data.get("settings", {})).items():
                if key in self.settings:
                    self.settings[key] = bool(value)

    def save(self):
        path = os.path.join(self.folder, "server.json")
        try:
            with open(path + ".tmp", "w", encoding="utf-8") as f:
                json.dump({"bans": self.bans, "allowed": self.allowed, "settings": self.settings}, f, indent=1)
            os.replace(path + ".tmp", path)
        except OSError as e:
            log.warning("couldn't save %s: %s", path, e)

    @staticmethod
    def find(entries, ip, ident):
        for entry in entries:
            if (ip and entry.get("ip") == ip) or (ident and entry.get("id") == ident):
                return entry
        return None


class VpnCheck:
    """Whether an address belongs to a VPN, proxy or data centre (see the top of the file)."""

    def __init__(self, folder):
        self.folder = folder
        self.starts = []
        self.ends = []
        self.ranges = 0
        self.updated = 0            # when the lists were last downloaded (time.time())
        self.cache = {}             # ip -> (when, ip-api.com's answer)
        self.recent_lookups = collections.deque()
        self.lookup_failures = 0
        cached = []
        for i in range(len(VPN_LISTS)):
            try:
                with open(self.list_path(i), encoding="utf-8") as f:
                    cached += self.parse(f.read())
            except OSError:
                pass
        self.use(cached)

    def list_path(self, i):
        return os.path.join(self.folder, "vpn-list-%d.txt" % i)

    @staticmethod
    def parse(text):
        ranges = []
        for line in text.splitlines():
            line = line.split("#")[0].strip()
            if not line:
                continue
            try:
                net = ipaddress.ip_network(line, strict=False)
            except ValueError:
                continue
            if net.version == 4:
                ranges.append((int(net.network_address), int(net.broadcast_address)))
        return ranges

    def use(self, ranges):
        ranges.sort()
        merged = []
        for start, end in ranges:
            if merged and start <= merged[-1][1] + 1:
                merged[-1][1] = max(merged[-1][1], end)
            else:
                merged.append([start, end])
        self.starts = [m[0] for m in merged]
        self.ends = [m[1] for m in merged]
        self.ranges = len(ranges)

    def listed(self, ip):
        try:
            addr = ipaddress.ip_address(ip)
        except ValueError:
            return False
        if getattr(addr, "ipv4_mapped", None) is not None:
            addr = addr.ipv4_mapped
        if addr.version != 4:
            return False
        n = int(addr)
        i = bisect.bisect_right(self.starts, n) - 1
        return i >= 0 and n <= self.ends[i]

    def download(self):
        """(In a thread.) Fetches the lists; keeps the old copy of one that can't be fetched."""
        ranges, fetched = [], 0
        for i, url in enumerate(VPN_LISTS):
            try:
                with urllib.request.urlopen(url, timeout=60) as response:
                    text = response.read(50 * 1024 * 1024).decode("utf-8", "replace")
                part = self.parse(text)
                if len(part) < 100:
                    raise ValueError("the list is nearly empty")
                with open(self.list_path(i) + ".tmp", "w", encoding="utf-8") as f:
                    f.write(text)
                os.replace(self.list_path(i) + ".tmp", self.list_path(i))
                ranges += part
                fetched += 1
            except Exception as e:
                log.warning("couldn't download %s (%s); using the last copy", url, e)
                try:
                    with open(self.list_path(i), encoding="utf-8") as f:
                        ranges += self.parse(f.read())
                except OSError:
                    pass
        return ranges, fetched

    async def keep_fresh(self):
        loop = asyncio.get_running_loop()
        while True:
            ranges, fetched = await loop.run_in_executor(None, self.download)
            if ranges:
                self.use(ranges)
            if fetched:
                self.updated = time.time()
            log.info("VPN lists: %d address ranges (%d of %d downloaded)", self.ranges, fetched, len(VPN_LISTS))
            await asyncio.sleep(VPN_LISTS_REFRESH_SECONDS if fetched == len(VPN_LISTS) else 60 * 60)

    @staticmethod
    def ask_ip_api(ip):
        with urllib.request.urlopen(LOOKUP_URL % ip, timeout=4) as response:
            answer = json.loads(response.read(4096).decode("utf-8", "replace"))
        return answer if isinstance(answer, dict) and answer.get("status") == "success" else None

    async def lookup(self, ip):
        """ip-api.com's answer for the address ({"countryCode", "proxy", "hosting"}), or None when
        it can't be asked just now. Kept for a day."""
        now = time.time()
        hit = self.cache.get(ip)
        if hit is not None and now - hit[0] < LOOKUP_CACHE_SECONDS:
            return hit[1]
        while self.recent_lookups and now - self.recent_lookups[0] > 60:
            self.recent_lookups.popleft()
        if len(self.recent_lookups) >= LOOKUPS_PER_MINUTE:
            return None
        self.recent_lookups.append(now)
        try:
            answer = await asyncio.wait_for(asyncio.get_running_loop().run_in_executor(None, self.ask_ip_api, ip), 5)
        except Exception:
            answer = None
        if answer is None:
            self.lookup_failures += 1
            return None
        if len(self.cache) > 50000:
            self.cache = {k: v for k, v in self.cache.items() if now - v[0] < LOOKUP_CACHE_SECONDS}
        self.cache[ip] = (now, answer)
        return answer


class Client:
    def __init__(self, reader, writer):
        self.reader = reader
        self.writer = writer
        peer = writer.get_extra_info("peername")
        self.ip = peer[0] if peer else "?"
        self.id = None
        self.name = "Player"
        self.country = ""
        self.since = time.time()
        self.hosting = None         # the Game it's hosting, until it starts
        self.game = None            # the Game it plays in, once paired
        self.last_seen = time.monotonic()
        self.messages = collections.deque()
        self.closed = False
        self.role = None            # "host" or "joiner" once paired

    def send(self, obj):
        if self.closed:
            return
        try:
            self.writer.write((json.dumps(obj, separators=(",", ":")) + "\n").encode("utf-8"))
        except Exception:
            self.closed = True

    def close(self):
        if not self.closed:
            self.closed = True
            try:
                self.writer.close()
            except Exception:
                pass


class Game:
    def __init__(self, host, code, mode, public, character):
        self.host = host
        self.code = code
        self.mode = mode
        self.public = public
        self.character = character
        self.created = time.monotonic()
        self.players = 1            # the host, and the guests let in so far
        self.tickets = {}           # ticket -> (joiner, future) waiting for the host's connection
        self.started = False

    def open_places(self):
        return MAX_PLAYERS - self.players - len(self.tickets)

    def summary(self, friend):
        return {
            "code": self.code if (self.public or friend) else "",
            "host": self.host.name,
            "host_id": self.host.id,
            "mode": self.mode,
            "public": self.public,
            "friend": friend,
            "character": self.character,
            "players": self.players,
            "age": int(time.monotonic() - self.created),
        }


class Lobby:
    def __init__(self, motd="", state="."):
        self.games = {}             # code -> Game, while it can be joined
        self.tickets = {}           # ticket -> Game
        self.clients = set()        # everyone in lobby mode
        self.everyone = set()       # every connection, lobby or game
        self.per_ip = {}
        self.new_per_ip = {}        # ip -> when it connected, over the last minute
        self.connections = 0
        self.relays = 0
        self.games_started = 0
        self.started_at = time.time()
        self.turned_away = collections.deque(maxlen=RECENT_TURNED_AWAY)
        self.motd = motd
        self.store = Store(state)
        self.vpn = VpnCheck(state)

    def new_code(self):
        for _ in range(1000):
            code = "%s-%04d" % (random.choice(CODE_WORDS), random.randint(0, 9999))
            if code not in self.games:
                return code
        raise RuntimeError("out of codes")

    def online_ids(self):
        ids = {}
        for c in self.clients:
            if c.id:
                ids[c.id] = c
        return ids

    def note_turned_away(self, client, why):
        self.turned_away.append({"when": int(time.time()), "ip": client.ip, "id": client.id or "",
                                 "name": client.name, "country": client.country, "why": why})
        log.info("turned away %s (%s, %s): %s", client.name, client.id or "no ID", client.ip, why)

    def connection_allowed(self, ip):
        """Too many connections too quickly from one address: turned away quietly."""
        now = time.monotonic()
        recent = self.new_per_ip.setdefault(ip, collections.deque())
        while recent and now - recent[0] > 60:
            recent.popleft()
        recent.append(now)
        if len(self.new_per_ip) > 10000:
            self.new_per_ip = {k: v for k, v in self.new_per_ip.items() if v and now - v[-1] < 60}
        return len(recent) <= MAX_NEW_CONNECTIONS_PER_MINUTE

    async def check_player(self, client):
        """Why this player can't play here (a message for them), or None."""
        if Store.find(self.store.bans, client.ip, client.id) is not None:
            self.note_turned_away(client, "banned (player ID)" if Store.find(self.store.bans, client.ip, None) is None
                                  else "banned (address)")
            return BANNED_MESSAGE
        if is_local_address(client.ip):
            return None
        answer = await self.vpn.lookup(client.ip) if self.store.settings["ip_lookup"] else None
        if answer is not None:
            client.country = str(answer.get("countryCode", ""))[:3]
        if not self.store.settings["block_vpns"] or Store.find(self.store.allowed, client.ip, client.id) is not None:
            return None
        if self.vpn.listed(client.ip):
            self.note_turned_away(client, "VPN (on the VPN lists)")
            return VPN_MESSAGE
        if answer is not None and (answer.get("proxy") or answer.get("hosting")):
            self.note_turned_away(client, "VPN (ip-api.com: %s)" % (
                "proxy or VPN" if answer.get("proxy") else "hosting company"))
            return VPN_MESSAGE
        return None

    async def handle(self, reader, writer):
        client = Client(reader, writer)
        count = self.per_ip.get(client.ip, 0)
        if not self.connection_allowed(client.ip):
            client.close()
            return
        ban = Store.find(self.store.bans, client.ip, None)
        if ban is not None:
            client.name = ban.get("name") or "?"
            self.note_turned_away(client, "banned (address)")
            client.send({"op": "error", "msg": BANNED_MESSAGE})
            await self.hang_up(client)
            return
        if count >= MAX_CONNECTIONS_PER_IP or self.connections >= MAX_CONNECTIONS:
            client.send({"op": "error", "msg": "Too many connections from your address."
                         if count >= MAX_CONNECTIONS_PER_IP else "The lobby is full; try again soon."})
            await self.hang_up(client)
            return
        sock = writer.get_extra_info("socket")
        if sock is not None:
            try:        # notice a computer that vanished (a dropped home connection) in minutes
                sock.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1)
                sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
                if hasattr(socket, "TCP_KEEPIDLE"):
                    sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPIDLE, 120)
                    sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPINTVL, 30)
                    sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPCNT, 4)
            except OSError:
                pass
        self.per_ip[client.ip] = count + 1
        self.connections += 1
        self.clients.add(client)
        self.everyone.add(client)
        try:
            relay_with = None
            try:
                relay_with = await self.lobby_loop(client)
            except (asyncio.IncompleteReadError, ConnectionError, asyncio.TimeoutError, OSError,
                    ValueError, TypeError, AttributeError):
                pass        # hung up, went quiet, or isn't the game (a port scanner, a web browser...)
            except Exception:
                log.exception("client %s", client.ip)
            finally:
                self.clients.discard(client)
                if client.hosting is not None:
                    self.end_hosting(client.hosting)
                    client.hosting = None
            if relay_with is not None:
                await self.relay(client, relay_with)
        finally:
            await self.hang_up(client)
            self.everyone.discard(client)
            self.connections -= 1
            self.per_ip[client.ip] = self.per_ip.get(client.ip, 1) - 1
            if self.per_ip[client.ip] <= 0:
                self.per_ip.pop(client.ip, None)

    @staticmethod
    async def hang_up(client):
        """Closes the connection so that a last message (why: banned, a VPN...) reaches the game.
        Closing with the game's next request still unread would reset the connection, and the
        game would lose the message."""
        if client.closed:
            return
        try:
            await asyncio.wait_for(client.writer.drain(), 2)
            if client.writer.can_write_eof():
                client.writer.write_eof()

            async def until_closed():
                while await client.reader.read(4096):
                    pass
            await asyncio.wait_for(until_closed(), 3)
        except Exception:
            pass
        client.close()

    def end_hosting(self, game):
        """The host started, or went: no more joining (guests still waiting are told)."""
        self.games.pop(game.code, None)
        for ticket, (joiner, future) in list(game.tickets.items()):
            self.tickets.pop(ticket, None)
            if not future.done():
                future.set_exception(ConnectionError("host gone"))
        game.tickets.clear()

    async def read_line(self, client, timeout):
        line = await asyncio.wait_for(client.reader.readline(), timeout)
        if not line:
            raise ConnectionError("closed")
        if len(line) > MAX_LINE:
            raise ConnectionError("line too long")
        now = time.monotonic()
        client.last_seen = now
        client.messages.append(now)
        while client.messages and now - client.messages[0] > 10:
            client.messages.popleft()
        if len(client.messages) > MAX_MESSAGES_PER_10S:
            raise ConnectionError("too many messages")
        return json.loads(line.decode("utf-8", "replace"))

    async def lobby_loop(self, client):
        """Lobby messages until the client's connection carries a game (returns the other side)
        or it leaves."""
        while True:
            timeout = HOST_WAIT_SECONDS if client.hosting is not None else LOBBY_IDLE_SECONDS
            try:
                msg = await self.read_line(client, timeout)
            except asyncio.TimeoutError:
                if client.hosting is not None:
                    client.send({"op": "error", "msg": "The game didn't start for a long time."})
                raise
            if client.closed:
                return None         # disconnected with mmhd-admin
            if not isinstance(msg, dict):
                continue
            op = msg.get("op")
            if op == "hello":
                if int(msg.get("proto", 0)) != PROTO:
                    client.send({"op": "error", "msg": "This lobby is for a different version of the game."})
                    return None
                ident = str(msg.get("id", "")).upper()
                client.id = ident if ID_PATTERN.match(ident) else None
                client.name = clean_name(msg.get("name", "Player"))
                refusal = await self.check_player(client)
                if refusal is not None:
                    client.send({"op": "error", "msg": refusal})
                    return None
                client.send({"op": "welcome", "motd": self.motd})
            elif op == "ping":
                client.send({"op": "pong"})
            elif op == "list":
                friends = [str(f).upper() for f in msg.get("friends", [])][:200]
                self.send_list(client, set(friends))
            elif op == "host":
                if client.id is None:
                    client.send({"op": "error", "msg": "Say hello first."})
                    continue
                if client.hosting is not None:
                    self.end_hosting(client.hosting)
                    client.hosting = None
                if sum(1 for g in self.games.values() if g.host.ip == client.ip) >= MAX_GAMES_PER_IP:
                    client.send({"op": "error", "msg": "Too many games are being hosted from your address."})
                    continue
                code = self.new_code()
                game = Game(client, code, int(msg.get("mode", 0)), bool(msg.get("public", True)),
                            int(msg.get("character", 0)))
                self.games[code] = game
                client.hosting = game
                client.send({"op": "hosted", "code": code})
                log.info("%s (%s) hosts %s %s", client.name, client.ip, code,
                         "public" if game.public else "private")
            elif op in ("cancel", "start"):
                if client.hosting is not None:
                    game = client.hosting
                    game.started = op == "start"
                    self.end_hosting(game)
                    if op == "start":
                        self.games_started += 1
                        log.info("%s starts %s with %d players", client.name, game.code, game.players)
                    client.hosting = None
                client.send({"op": "cancelled" if op == "cancel" else "started"})
            elif op == "join":
                if client.id is None:
                    client.send({"op": "error", "msg": "Say hello first."})
                    continue
                game = None
                code = str(msg.get("code", "")).strip().upper()
                if code:
                    game = self.games.get(code)
                elif msg.get("host_id"):
                    host_id = str(msg.get("host_id")).upper()
                    game = next((g for g in self.games.values() if g.host.id == host_id), None)
                if game is None:
                    client.send({"op": "error", "msg": "That game isn't there any more (or the code is wrong)."})
                    continue
                if game.host is client:
                    client.send({"op": "error", "msg": "That's your own game."})
                    continue
                if game.open_places() <= 0:
                    client.send({"op": "error", "msg": "That game is full."})
                    continue
                    # the host opens a connection for this guest; wait for it
                ticket = "%016x" % random.getrandbits(64)
                future = asyncio.get_running_loop().create_future()
                game.tickets[ticket] = (client, future)
                self.tickets[ticket] = game
                game.host.send({"op": "incoming", "ticket": ticket,
                                "peer": {"id": client.id, "name": client.name}})
                try:
                    attached = await asyncio.wait_for(future, ATTACH_SECONDS)
                except (asyncio.TimeoutError, ConnectionError):
                    game.tickets.pop(ticket, None)
                    self.tickets.pop(ticket, None)
                    client.send({"op": "error", "msg": "The host didn't let you in (the game may have started)."})
                    continue
                client.role = "joiner"
                client.game = game
                return attached
            elif op == "attach":
                    # the host's connection for one guest
                ticket = str(msg.get("ticket", ""))
                game = self.tickets.pop(ticket, None)
                entry = game.tickets.pop(ticket, None) if game is not None else None
                if entry is None or entry[1].done():
                    client.send({"op": "error", "msg": "No one is waiting with that ticket."})
                    return None
                joiner, future = entry
                game.players += 1                       # (once full it's no longer listed)
                host_info = {"id": game.host.id, "name": game.host.name}
                client.send({"op": "paired", "role": "host", "peer": {"id": joiner.id, "name": joiner.name},
                             "code": game.code})
                joiner.send({"op": "paired", "role": "joiner", "peer": host_info, "code": game.code,
                             "mode": game.mode})
                log.info("%s joins %s's game %s (%d players)", joiner.name, game.host.name, game.code,
                         game.players)
                client.role = "host"
                    # this connection is the host's (mmhd-admin shows and disconnects it as theirs)
                client.id, client.name, client.country = game.host.id, game.host.name, game.host.country
                client.game = game
                future.set_result(client)
                return joiner
            else:
                client.send({"op": "error", "msg": "Unknown request."})

    def send_list(self, client, friends):
        games = []
        for game in self.games.values():
            if game.host is client or game.open_places() <= 0:
                continue
            friend = game.host.id in friends
            if game.public or friend:
                games.append(game.summary(friend))
        games.sort(key=lambda g: (not g["friend"], g["age"]))
        online = self.online_ids()
        status = []
        for fid in friends:
            c = online.get(fid)
            status.append({
                "id": fid,
                "name": c.name if c else "",
                "online": c is not None,
                "code": c.hosting.code if c is not None and c.hosting is not None and c.hosting.open_places() > 0
                else "",
            })
        client.send({"op": "games", "games": games[:50], "friends": status})

    async def relay(self, a, b):
        """Both sides of a game: bytes one way and the other until either hangs up. Each side's
        task comes here; the host's does the pumping both ways (it has stopped reading its own
        connection by then), the joiner's waits for the game to end."""
        if a.role != "host":
            while not (a.closed or b.closed):
                await asyncio.sleep(1)
            return

        last = [time.monotonic()]
        self.relays += 1

        async def pump(src, dst):
            window_start, window_bytes = time.monotonic(), 0
            try:
                while True:
                    data = await src.reader.read(65536)
                    if not data:
                        break
                    now = time.monotonic()
                    last[0] = now
                    if now - window_start > 10:
                        window_start, window_bytes = now, 0
                    window_bytes += len(data)
                    if window_bytes > RELAY_MAX_BYTES_PER_10S:
                        log.info("ended a game of %s: far more data than a game sends", src.name)
                        break
                    dst.writer.write(data)
                    await dst.writer.drain()
            except Exception:
                pass

        async def watchdog():
                # (one side may be quiet for a long time: a player watching someone else's turn;
                # it also notices mmhd-admin disconnecting either side)
            while time.monotonic() - last[0] < RELAY_IDLE_SECONDS and not (a.closed or b.closed):
                await asyncio.sleep(1)

        tasks = {asyncio.ensure_future(pump(a, b)), asyncio.ensure_future(pump(b, a)),
                 asyncio.ensure_future(watchdog())}
        await asyncio.wait(tasks, return_when=asyncio.FIRST_COMPLETED)
        for t in tasks:
            t.cancel()
        self.relays -= 1
        if a.game is not None and not a.game.started:
            a.game.players = max(1, a.game.players - 1)     # a guest left before the start: a place again
        a.close()
        b.close()

    async def report(self):
        """Now and then, how busy it is (in the log)."""
        while True:
            await asyncio.sleep(STATS_SECONDS)
            log.info("status: %d connected, %d in the lobby, %d games waiting, %d guests playing, "
                     "%d games started since the server started", self.connections, len(self.clients),
                     len(self.games), self.relays, self.games_started)

    # -----------------------------------------------------------------------------------------
    # mmhd-admin

    def players(self):
        """Everyone connected, one row per player (a host has a connection per guest too)."""
        rows = {}
        for c in self.everyone:
            if c.closed or not c.id:
                continue
            row = rows.get(c.id)
            if row is None:
                row = rows[c.id] = {"id": c.id, "name": c.name, "ip": c.ip, "country": c.country,
                                    "since": c.since, "doing": "in the lobby", "rank": 0}
            row["since"] = min(row["since"], c.since)
            row["country"] = row["country"] or c.country
            if c.hosting is not None and row["rank"] < 2:
                row["doing"], row["rank"] = "hosting %s, %d of %d in" % (
                    c.hosting.code, c.hosting.players, MAX_PLAYERS), 2
            elif c.role is not None and c.game is not None and row["rank"] < 1:
                row["doing"], row["rank"] = ("playing %s, %d players" % (c.game.code, c.game.players)
                                             if c.game.started else "joined %s" % c.game.code), 1
        out = sorted(rows.values(), key=lambda r: r["since"])
        for r in out:
            del r["rank"]
            r["since"] = int(r["since"])
        return out

    def disconnect(self, ident=None, ip=None):
        """Hangs up on every connection of that player (by ID, or by address). Returns how many."""
        n = 0
        for c in list(self.everyone):
            if (ident and c.id == ident) or (ip and c.ip == ip):
                if not c.closed:
                    n += 1
                c.close()
        return n

    def admin(self, req):
        op = req.get("op")
        if op == "status":
            return {"ok": True, "players": self.players(), "bans": self.store.bans, "allowed": self.store.allowed,
                    "turned_away": list(self.turned_away)[::-1], "settings": self.store.settings,
                    "uptime": int(time.time() - self.started_at), "connections": self.connections,
                    "games_waiting": len(self.games), "games_started": self.games_started,
                    "vpn_ranges": self.vpn.ranges, "vpn_updated": int(self.vpn.updated),
                    "lookup_failures": self.vpn.lookup_failures}
        ident = str(req.get("id") or "").upper() or None
        ip = str(req.get("ip") or "") or None
        name = clean_name(req.get("name") or ident or ip or "")
        if op == "disconnect":
            n = self.disconnect(ident, None if ident else ip)
            log.info("admin: disconnected %s (%s, %s)", name, ident, ip)
            return {"ok": True, "msg": "Disconnected %s." % name if n else "%s wasn't connected." % name}
        if op == "ban":
            if not ident and not ip:
                return {"ok": False, "msg": "Nothing to ban."}
            if Store.find(self.store.bans, ip, ident) is None:
                self.store.bans.append({"ip": ip or "", "id": ident or "", "name": name, "when": int(time.time()),
                                        "reason": str(req.get("reason", ""))[:100]})
                self.store.allowed = [a for a in self.store.allowed if Store.find([a], ip, ident) is None]
                self.store.save()
            self.disconnect(ident, ip)
            log.info("admin: banned %s (%s, %s)", name, ident, ip)
            return {"ok": True, "msg": "Banned %s." % name}
        if op == "unban":
            before = len(self.store.bans)
            self.store.bans = [b for b in self.store.bans if Store.find([b], ip, ident) is None]
            self.store.save()
            log.info("admin: unbanned %s", name)
            return {"ok": True, "msg": "Unbanned %s." % name if len(self.store.bans) < before
                    else "%s wasn't banned." % name}
        if op == "allow":
            if Store.find(self.store.allowed, ip, ident) is None:
                self.store.allowed.append({"ip": ip or "", "id": ident or "", "name": name, "when": int(time.time())})
                self.store.save()
            log.info("admin: let in %s (%s, %s) despite the VPN check", name, ident, ip)
            return {"ok": True, "msg": "%s can play now, VPN or not." % name}
        if op == "unallow":
            self.store.allowed = [a for a in self.store.allowed if Store.find([a], ip, ident) is None]
            self.store.save()
            return {"ok": True, "msg": "%s goes through the VPN check again." % name}
        if op == "set":
            for key in self.store.settings:
                if key in req:
                    self.store.settings[key] = bool(req[key])
                    log.info("admin: %s = %s", key, self.store.settings[key])
            self.store.save()
            return {"ok": True, "settings": self.store.settings}
        return {"ok": False, "msg": "Unknown request."}

    async def admin_handle(self, reader, writer):
        try:
            while True:
                line = await reader.readline()
                if not line:
                    break
                try:
                    reply = self.admin(json.loads(line.decode("utf-8", "replace")))
                except Exception as e:
                    reply = {"ok": False, "msg": "Bad request: %s" % e}
                writer.write((json.dumps(reply) + "\n").encode("utf-8"))
                await writer.drain()
        except Exception:
            pass
        finally:
            writer.close()


async def main():
    parser = argparse.ArgumentParser(description="Midnight Mansion HD lobby server")
    parser.add_argument("--host", default="0.0.0.0")
    parser.add_argument("--port", type=int, default=47480)
    parser.add_argument("--motd", default="", help="message shown to players who connect")
    parser.add_argument("--state", default=os.environ.get("STATE_DIRECTORY", "mmhd-state"),
                        help="folder for bans, settings and the VPN lists")
    parser.add_argument("--admin-socket",
                        default=os.path.join(os.environ.get("RUNTIME_DIRECTORY", "."), "admin.sock"),
                        help="where mmhd-admin finds the server")
    args = parser.parse_args()
    logging.basicConfig(level=logging.INFO, format="%(asctime)s %(message)s")
    lobby = Lobby(args.motd, args.state)
    server = await asyncio.start_server(lobby.handle, args.host, args.port, limit=MAX_LINE * 2)
    try:
        os.unlink(args.admin_socket)
    except OSError:
        pass
    old_mask = os.umask(0o077)      # only root and the server's own user may open it
    admin = await asyncio.start_unix_server(lobby.admin_handle, args.admin_socket)
    os.umask(old_mask)
    log.info("Midnight Mansion HD lobby on %s:%d (admin: %s)", args.host, args.port, args.admin_socket)
    background = [asyncio.ensure_future(lobby.report()), asyncio.ensure_future(lobby.vpn.keep_fresh())]
    async with server, admin:
        await server.serve_forever()
    del background


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        pass
