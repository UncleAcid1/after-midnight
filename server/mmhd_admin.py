#!/usr/bin/env python3
"""
mmhd-admin: a simple screen for looking after the Midnight Mansion HD lobby server.

On the server, type   mmhd-admin   (it asks for sudo by itself if it needs to).

    Players       who's connected now: name, player ID, address, country, what they're doing
    Turned away   who was refused lately (banned, or through a VPN)
    Banned        everyone banned
    Let in        players allowed although they look like a VPN

Arrow keys choose, the letters on the bottom line do things, Q quits. Nothing happens without
asking "are you sure?" first.

Also from the command line (for scripts):

    mmhd-admin list                  who's connected
    mmhd-admin ban <address or ID>
    mmhd-admin unban <address or ID>
    mmhd-admin vpn on|off
"""

import curses
import json
import os
import re
import socket
import sys
import time

SOCKET = os.environ.get("MMHD_ADMIN_SOCKET", "/run/mmhd-lobby/admin.sock")
ID_PATTERN = re.compile(r"^[0-9A-Z]{4}-[0-9A-Z]{4}-[0-9A-Z]{4}$")
TABS = ["Players", "Turned away", "Banned", "Let in"]
TAB_HELP = [
    "Everyone connected right now.",
    "Players the server refused lately (banned, or using a VPN). Newest first.",
    "Banned players can't use this server (blocked by address and by player ID).",
    "Let in even though the VPN check flags them (for when it's wrong).",
]


class NotRunning(Exception):
    pass


def ask(request):
    """One request to the server; its answer."""
    try:
        s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        s.settimeout(5)
        s.connect(SOCKET)
    except PermissionError:
        raise
    except OSError:
        raise NotRunning()
    try:
        s.sendall((json.dumps(request) + "\n").encode("utf-8"))
        data = b""
        while not data.endswith(b"\n"):
            chunk = s.recv(65536)
            if not chunk:
                break
            data += chunk
    finally:
        s.close()
    return json.loads(data.decode("utf-8"))


def ago(seconds):
    seconds = max(0, int(seconds))
    if seconds < 60:
        return "%d sec" % seconds
    if seconds < 3600:
        return "%d min" % (seconds // 60)
    if seconds < 2 * 86400:
        return "%d h %d min" % (seconds // 3600, seconds % 3600 // 60)
    return "%d days" % (seconds // 86400)


def short(seconds):
    """How long, in a few characters: 45s, 9m, 2h15m, 3d."""
    seconds = max(0, int(seconds))
    if seconds < 60:
        return "%ds" % seconds
    if seconds < 3600:
        return "%dm" % (seconds // 60)
    if seconds < 2 * 86400:
        return "%dh%02dm" % (seconds // 3600, seconds % 3600 // 60)
    return "%dd" % (seconds // 86400)


def target(text):
    text = text.strip()
    return {"id": text.upper()} if ID_PATTERN.match(text.upper()) else {"ip": text}


# ---------------------------------------------------------------------------------------------
# The screen

class Screen:
    def __init__(self, stdscr):
        self.scr = stdscr
        self.tab = 0
        self.selected = [0, 0, 0, 0]
        self.status = None
        self.error = ""
        self.message = ""
        self.message_until = 0
        self.last_fetch = 0
        curses.curs_set(0)
        stdscr.timeout(300)
        self.colors = curses.has_colors()
        if self.colors:
            curses.start_color()
            try:
                curses.use_default_colors()
                bg = -1
            except curses.error:
                bg = curses.COLOR_BLACK
            curses.init_pair(1, curses.COLOR_GREEN, bg)
            curses.init_pair(2, curses.COLOR_RED, bg)
            curses.init_pair(3, curses.COLOR_YELLOW, bg)
            curses.init_pair(4, curses.COLOR_CYAN, bg)

    def color(self, n):
        return curses.color_pair(n) if self.colors else 0

    def put(self, y, x, text, attr=0):
        h, w = self.scr.getmaxyx()
        if 0 <= y < h and x < w - 1:
            try:
                self.scr.addstr(y, x, text[:w - 1 - x], attr)
            except curses.error:
                pass

    def say(self, text):
        self.message = text
        self.message_until = time.time() + 6

    def fetch(self):
        try:
            self.status = ask({"op": "status"})
            self.error = ""
        except NotRunning:
            self.status = None
            self.error = ("The lobby server isn't running.\n\nStart it with:     sudo systemctl start mmhd-lobby\n"
                          "See what went wrong:  journalctl -u mmhd-lobby -n 30")
        except Exception as e:
            self.status = None
            self.error = "Can't talk to the lobby server: %s" % e
        self.last_fetch = time.time()

    def rows(self):
        s = self.status or {}
        return [s.get("players", []), s.get("turned_away", []), s.get("bans", []), s.get("allowed", [])][self.tab]

    def current(self):
        rows = self.rows()
        if not rows:
            return None
        self.selected[self.tab] = max(0, min(self.selected[self.tab], len(rows) - 1))
        return rows[self.selected[self.tab]]

    def confirm(self, question):
        h, w = self.scr.getmaxyx()
        self.scr.timeout(-1)
        try:
            while True:
                    # the question (on two lines if it's long), and what to press under it
                words, lines = question.split(), [""]
                for word in words:
                    if len(lines[-1]) + len(word) + 1 > w - 3 and lines[-1]:
                        lines.append("")
                    lines[-1] = (lines[-1] + " " + word).strip()
                lines = lines[-2:]
                for i in range(3):
                    self.put(h - 4 + i, 0, " " * (w - 1))
                for i, line in enumerate(lines):
                    self.put(h - 4 + i + (2 - len(lines)), 1, line, curses.A_BOLD | self.color(3))
                self.put(h - 2, 1, "Press Y for yes, N for no.", curses.A_BOLD)
                self.scr.refresh()
                key = self.scr.getch()
                if key in (ord("y"), ord("Y")):
                    return True
                if key in (ord("n"), ord("N"), 27, ord("q"), ord("Q")):
                    return False
        finally:
            self.scr.timeout(300)

    def act(self, request, question):
        if not self.confirm(question):
            self.say("Nothing done.")
            return
        try:
            reply = ask(request)
            self.say(reply.get("msg", "Done."))
        except Exception as e:
            self.say("That didn't work: %s" % e)
        self.fetch()

    @staticmethod
    def who(row):
        name = row.get("name") or "?"
        bits = []
        if row.get("ip"):
            bits.append("address " + row["ip"])
        if row.get("id"):
            bits.append("ID " + row["id"])
        return "%s (%s)" % (name, ", ".join(bits)) if bits else name

    def key(self, k):
        rows = self.rows()
        row = self.current()
        if k in (ord("q"), ord("Q")):
            return False
        if k in (9, curses.KEY_RIGHT):
            self.tab = (self.tab + 1) % len(TABS)
        elif k in (curses.KEY_BTAB, curses.KEY_LEFT):
            self.tab = (self.tab - 1) % len(TABS)
        elif ord("1") <= k <= ord("4"):
            self.tab = k - ord("1")
        elif k == curses.KEY_UP:
            self.selected[self.tab] = max(0, self.selected[self.tab] - 1)
        elif k == curses.KEY_DOWN:
            self.selected[self.tab] = min(max(0, len(rows) - 1), self.selected[self.tab] + 1)
        elif k in (ord("v"), ord("V")) and self.status:
            on = self.status["settings"].get("block_vpns", True)
            self.act({"op": "set", "block_vpns": not on},
                     "Turn VPN blocking %s?" % ("OFF (players using a VPN can play)" if on else
                                                "ON (players using a VPN are turned away)"))
        elif row is None:
            pass
        elif self.tab == 0 and k in (ord("d"), ord("D")):
            self.act({"op": "disconnect", "id": row["id"], "name": row["name"]},
                     "Disconnect %s? (They can come back; their game ends.)" % self.who(row))
        elif self.tab in (0, 1) and k in (ord("b"), ord("B")):
            self.act({"op": "ban", "id": row.get("id"), "ip": row.get("ip"), "name": row.get("name")},
                     "Ban %s? They won't be able to play on this server." % self.who(row))
        elif self.tab == 1 and k in (ord("l"), ord("L")):
            self.act({"op": "allow", "id": row.get("id"), "ip": row.get("ip"), "name": row.get("name")},
                     "Let %s in even though the VPN check flags them?" % self.who(row))
        elif self.tab == 2 and k in (ord("u"), ord("U")):
            self.act({"op": "unban", "id": row.get("id"), "ip": row.get("ip"), "name": row.get("name")},
                     "Unban %s?" % self.who(row))
        elif self.tab == 3 and k in (ord("r"), ord("R")):
            self.act({"op": "unallow", "id": row.get("id"), "ip": row.get("ip"), "name": row.get("name")},
                     "Put %s through the VPN check again?" % self.who(row))
        return True

    def draw(self):
        self.scr.erase()
        h, w = self.scr.getmaxyx()
        s = self.status
        self.put(0, 0, " AFTER MIDNIGHT  -  MIDNIGHT MANSION HD SERVER ".ljust(w - 1), curses.A_REVERSE | curses.A_BOLD)
        if s is None:
            lines = self.error.split("\n")
            for i, line in enumerate(lines):
                self.put(2 + i, 1, line, (self.color(2) | curses.A_BOLD) if i == 0 else 0)
            self.put(3 + len(lines), 1, "Trying again every second.  Q quits.")
            self.scr.refresh()
            return
        self.put(0, w - 30, ("running for " + ago(s["uptime"])).rjust(28), curses.A_REVERSE | curses.A_BOLD)
        self.put(2, 1, "Players online: %-4d  Games waiting for players: %-3d  Games started: %d" % (
            len(s["players"]), s["games_waiting"], s["games_started"]))
        vpn_on = s["settings"].get("block_vpns", True)
        self.put(3, 1, "VPN blocking: ")
        self.put(3, 15, "ON " if vpn_on else "OFF", (self.color(1) if vpn_on else self.color(2)) | curses.A_BOLD)
        if vpn_on:
            if s["vpn_ranges"]:
                lists = "%d VPN address ranges, lists updated %s ago" % (
                    s["vpn_ranges"], ago(time.time() - s["vpn_updated"])) if s["vpn_updated"] else \
                    "%d VPN address ranges (old copy)" % s["vpn_ranges"]
            else:
                lists = "the VPN lists haven't downloaded yet (only ip-api.com checks)"
            self.put(3, 20, "  " + lists)

            # the tabs
        x = 1
        for i, name in enumerate(TABS):
            count = len([s["players"], s["turned_away"], s["bans"], s["allowed"]][i])
            label = " %d %s (%d) " % (i + 1, name, count)
            self.put(5, x, label, curses.A_REVERSE | curses.A_BOLD if i == self.tab else curses.A_DIM)
            x += len(label) + 1
        self.put(6, 1, TAB_HELP[self.tab], self.color(4))

            # the list
        rows = self.rows()
        now = time.time()
        if self.tab == 0:
                # (fits a standard 80-column window)
            header = "%-14s %-14s %-15s %-3s %-6s %s" % ("NAME", "PLAYER ID", "ADDRESS", "", "ON", "DOING")
            lines = ["%-14s %-14s %-15s %-3s %-6s %s" % (r["name"][:14], r["id"], r["ip"][:15], r["country"][:3],
                                                       short(now - r["since"]), r["doing"]) for r in rows]
            empty = "Nobody is connected right now."
        elif self.tab == 1:
            header = "%-6s %-14s %-14s %-15s %-3s %s" % ("AGO", "NAME", "PLAYER ID", "ADDRESS", "", "WHY")
            lines = ["%-6s %-14s %-14s %-15s %-3s %s" % (short(now - r["when"]), r["name"][:14], r["id"] or "-",
                                                       r["ip"][:15], r.get("country", "")[:3], r["why"]) for r in rows]
            empty = "Nobody has been turned away since the server started."
        else:
            header = "%-18s %-15s %-16s %s" % ("NAME", "PLAYER ID", "ADDRESS", "SINCE")
            lines = ["%-18s %-15s %-16s %s" % ((r.get("name") or "")[:18], r.get("id") or "-", r.get("ip") or "-",
                                               time.strftime("%Y-%m-%d %H:%M", time.localtime(r.get("when", 0))))
                     for r in rows]
            empty = "Nobody is banned." if self.tab == 2 else "Nobody."
        self.put(8, 3, header, curses.A_BOLD)
        top, room = 9, max(1, h - 9 - 4)
        if not rows:
            self.put(top, 3, empty, curses.A_DIM)
        sel = self.selected[self.tab] = max(0, min(self.selected[self.tab], len(rows) - 1))
        first = max(0, sel - room + 1)
        for i, line in enumerate(lines[first:first + room]):
            n = first + i
            if n == sel:
                self.put(top + i, 1, ("> " + line).ljust(w - 2), curses.A_REVERSE)
            else:
                self.put(top + i, 3, line)

            # what can be done
        keys = {0: "D disconnect  B ban", 1: "L let in anyway  B ban", 2: "U unban",
                3: "R check again"}[self.tab]
        if not rows:
            keys = ""
        self.put(h - 1, 0, (" Up/Down choose  Left/Right lists  %s  V VPN on/off  Q quit" % keys)
                 .replace("    ", "  ").ljust(w - 1), curses.A_REVERSE)
        if self.message and time.time() < self.message_until:
            self.put(h - 2, 1, self.message, curses.A_BOLD | self.color(3))
        self.scr.refresh()

    def run(self):
        self.fetch()
        while True:
            if time.time() - self.last_fetch >= 1:
                self.fetch()
            self.draw()
            k = self.scr.getch()
            if k == -1:
                continue
            if k == curses.KEY_RESIZE:
                continue
            if not self.key(k):
                return


# ---------------------------------------------------------------------------------------------

def command_line(args):
    if args[0] == "list":
        s = ask({"op": "status"})
        if not s["players"]:
            print("Nobody is connected.")
        for r in s["players"]:
            print("%-18s %-15s %-16s %-3s %s" % (r["name"], r["id"], r["ip"], r["country"], r["doing"]))
    elif args[0] in ("ban", "unban") and len(args) == 2:
        request = {"op": args[0]}
        request.update(target(args[1]))
        print(ask(request).get("msg", ""))
    elif args[0] == "vpn" and len(args) == 2 and args[1] in ("on", "off"):
        ask({"op": "set", "block_vpns": args[1] == "on"})
        print("VPN blocking is %s." % args[1])
    else:
        print(__doc__.strip())
        return 1
    return 0


def main():
    args = sys.argv[1:]
    if args and args[0] in ("-h", "--help", "help"):
        print(__doc__.strip())
        return 0
    if os.geteuid() != 0 and not os.access(SOCKET, os.W_OK) and "MMHD_ADMIN_SOCKET" not in os.environ:
            # the socket is the server's: ask for sudo, then come back as root
        os.execvp("sudo", ["sudo", sys.executable, os.path.abspath(__file__)] + args)
    try:
        if args:
            return command_line(args)
        curses.wrapper(lambda stdscr: Screen(stdscr).run())
    except NotRunning:
        print("The lobby server isn't running. Start it with:  sudo systemctl start mmhd-lobby")
        return 1
    except PermissionError:
        print("Not allowed to talk to the server. Run it as:  sudo mmhd-admin")
        return 1
    except KeyboardInterrupt:
        pass
    return 0


if __name__ == "__main__":
    sys.exit(main())
