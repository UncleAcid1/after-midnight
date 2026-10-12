# Playing over the internet

Computers on the same network find each other with UDP broadcasts and connect directly
(see EXTRAS.md, Several computers). Over the internet neither works: broadcasts don't leave the
home network, and home routers don't accept connections from outside. So every computer
connects *out* to a small lobby server, which introduces them and then carries their game
(2 to 4 players).

## The lobby server

`server/mmhd_lobby.py`: Python 3.8 or later, no other dependencies, one file.

    python3 server/mmhd_lobby.py --port 47480 --motd "Welcome!"

It needs one TCP port open to the internet (47480 by default). The smallest cloud instance is
plenty: a game sends a few dozen bytes a frame each way, so one server carries many games at
once (each has its own code and its own connections). It keeps only bans and settings on disk; a game
disappears when its host leaves or after three hours without starting.

Players point the game at it in Multiplayer → a game type → Play online... → Lobby server
(saved in `Online.txt` next to the preferences), or with `MM_LOBBY=host:port`. The built-in
default is `kDefaultLobby` in `src/game/MMOnline.c` (the project's server, `129.158.224.129:47480`);
for testing on one computer use `MM_LOBBY=127.0.0.1:47480`.

`server/install.sh` runs it as a systemd service (below): it starts when the machine boots,
restarts three seconds after it stops for any reason (it never gives up), runs as a throwaway
user that can't write anywhere, and may hold 8192 open connections.

## Setting it up on Oracle Cloud (free)

1. Sign up at cloud.oracle.com (a card is asked for to verify you; Always Free resources
   aren't charged). Pick a home region near your players: it can't be changed later.
2. **Compute → Instances → Create instance.** Image: Canonical Ubuntu (22.04 or 24.04). Shape:
   *VM.Standard.E2.1.Micro* (AMD, Always Free) or *VM.Standard.A1.Flex* with 1 OCPU and 6 GB
   (Arm, Always Free; if it says "out of capacity", try again later or use the E2 Micro). Keep
   "Assign a public IPv4 address" on, and download the SSH private key it offers.
3. **Open the port in Oracle's firewall:** on the instance's page click the subnet → its
   *Default Security List* → *Add Ingress Rules*: source CIDR `0.0.0.0/0`, IP protocol TCP,
   destination port `47480`.
4. **Install the server** (from the repository folder on your computer; the instance's public
   IP is on its page):

       scp -i <key file> server/mmhd_lobby.py server/mmhd_admin.py server/install.sh ubuntu@<public IP>:
       ssh -i <key file> ubuntu@<public IP> sudo bash install.sh

   (On Windows, `scp` and `ssh` work in PowerShell; on macOS/Linux first `chmod 600 <key file>`.)
   `install.sh` installs it as a service that starts with the server and restarts if it stops,
   and opens the port in the server's own firewall: Oracle's Ubuntu images block every port but
   SSH even when the security list allows it.
5. **Check it from your computer:** `python3 server/check_lobby.py <public IP>`.
6. **Point the game at it:** set `kDefaultLobby` in `src/game/MMOnline.c` to `"<public IP>:47480"`
   and rebuild (or type it in Play online... → Lobby server). A free name from duckdns.org
   pointing at the IP lets you move the server later without a new build.

## Looking after it: mmhd-admin

**From a Mac, with no typing:** run `bash server/mac/make_shortcuts.sh` once from the game's
folder. It makes two shortcuts, in the folder holding the game's folder if that's a folder on
the Desktop, otherwise on the Desktop (`--into <folder>` for elsewhere; they can be moved
later). It remembers the server and the folder in `~/.mmhd-server.conf`, and the shortcuts
find the SSH key by themselves if it moves (on the Desktop, in a folder on it, in Downloads
or Documents):

- **Midnight Mansion Server**: double-click to open the control screen below.
- **Update Midnight Mansion Server**: double-click after getting a new version of the game's
  folder: it copies the newest server files to the server and reinstalls (bans and settings
  are kept), then refreshes both shortcuts. It uses the newest copy of the folder on the
  Desktop, in Downloads or Documents (and `git pull`s it first if it's a git checkout).

Otherwise, on the server (in the SSH window) type `mmhd-admin`. It's a plain screen that updates every
second:

- **1 Players**: everyone connected: name, player ID, address, country, how long they've been
  on and what they're doing (in the lobby, hosting, waiting for the start, playing).
  **D** disconnects the chosen player (they can come back; their game ends for everyone in it),
  **B** bans them.
- **2 Turned away**: who the server refused lately and why (banned, or a VPN).
  **L** lets a player in anyway (for when the VPN check is wrong about someone), **B** bans.
- **3 Banned**: **U** unbans.
- **4 Let in**: players let in despite the VPN check; **R** puts them through it again.
- **V** turns VPN blocking on or off, **Q** quits. Every action asks "are you sure?" first.

Left/Right (or 1-4) switch lists, Up/Down choose. A ban blocks both the player's address and
their player ID, so a new address or a new profile alone doesn't get them back in. For
scripts: `mmhd-admin list`, `mmhd-admin ban <address or ID>`, `mmhd-admin unban ...`,
`mmhd-admin vpn on|off`. Bans and settings are kept in `/var/lib/mmhd-lobby/server.json`
across restarts.

### VPN blocking

On by default. A player is turned away, with "Playing through a VPN or proxy isn't allowed
on this server", when their address is:

- in the VPN or data-centre lists of [X4BNet/lists_vpn](https://github.com/X4BNet/lists_vpn)
  (about 55,000 address ranges, downloaded again every day), or
- one that [ip-api.com](https://ip-api.com) says belongs to a proxy, VPN or hosting company
  (asked once a day per address; it also gives the country shown in mmhd-admin; free, up to
  45 questions a minute, which the server stays under).

No VPN check is perfect: a VPN on home-broadband addresses ("residential proxies") gets
through, and once in a while a real player is flagged (some phone networks, iCloud Private
Relay, work networks). They show in **Turned away**; **L** lets them in. If the lists or
ip-api.com can't be reached, players are let in rather than locked out. Addresses on a home
network (192.168..., 10...) are never checked.

### Other protection

Anything that isn't the game (port scanners, web browsers, bots) is dropped without filling
the log. From one address: at most 16 connections at once, 40 new ones a minute, and 4 games
waiting for players; at most 2000 connections in all. A connection sending more than 40
lobby messages in 10 seconds, or a game passing far more data than a game sends, is cut off.
A connection that says nothing goes after 15 minutes (a hosted game after three hours); TCP
keepalives notice a vanished computer within a few minutes, and a game where nobody has sent
anything for 20 minutes ends. The admin socket (`/run/mmhd-lobby/admin.sock`) can only be
opened by root, so only someone logged in to the server with sudo can use mmhd-admin.

The server itself: Oracle's Ubuntu images only accept SSH logins with your key (no
passwords), and only ports 22 and 47480 are open. Keep the key file private.

## Keeping it running

- **Logs:** `journalctl -u mmhd-lobby -f` (games hosted, joined and started; once an hour a
  `status:` line with how many are connected, waiting and playing). journald rotates them.
- **State:** `systemctl status mmhd-lobby`. Restart: `sudo systemctl restart mmhd-lobby`
  (games in progress end; players just host again).
- **Updating:** the **Update Midnight Mansion Server** shortcut, or by hand: copy the new
  `mmhd_lobby.py`, `mmhd_admin.py` and `install.sh` over and run `sudo bash install.sh` again
  (bans and settings are kept).
- **Reboots:** safe at any time; the service and the firewall rule come back by themselves.
  Ubuntu installs security updates on its own (unattended-upgrades) but doesn't reboot for
  them: `sudo reboot` now and then when the login message says one is required.
- **Idle reclamation:** Oracle may reclaim (stop) an Always Free instance whose CPU, network
  and memory stay nearly idle for 7 days, which a quiet lobby is. Upgrade the account to
  **Pay As You Go** (Billing → Upgrade and Manage Payment): Always Free resources stay free,
  and idle instances aren't reclaimed. Set a budget alert (Billing → Budgets, e.g. $1) to be
  told if anything ever costs money.
- **Protocol changes:** the `hello` carries a protocol number (`PROTO` in the server, sent by
  `MMOnline.c`); an older game connecting to a newer lobby is told it's for a different version.

## How it works

A connection starts in lobby mode: one JSON object per line.

| From the game | From the server |
|---|---|
| `hello` (protocol version, the player's ID and name) | `welcome` (message of the day) |
| `list` (the player's friends' IDs), every 3 s while the list is shown | `games` (public games and friends' private ones, and each friend's status) |
| `host` (game type, public or private, character) | `hosted` (the game's code, `RAVEN-4821`) |
| `join` (a code, or a friend's ID) | to the host: `incoming` (a ticket and who it is) |
| `attach` (the host, on a new connection: the ticket) | `paired` to both sides, or `error` |
| `start` / `cancel` (the host) | `started` / `cancelled` |

The host's game is the hub, as on a local network: each guest gets a connection of its own to
it. For every joiner the host opens one more connection to the server and `attach`es it with
the ticket; from `paired` on the server only relays bytes between that guest and the host,
unchanged, until one side hangs up. The game stays listed (with how many have joined) until
the host starts it or four are in. The game's own messages (`MMNet.c`: hello, roster, start,
frames, ghost positions, chat...) then run exactly as over a local network, the host passing
each guest's messages on to the others, so relay co-op, ghost races, chat and continuing a
saved game all work online with no special cases.

Private games are listed only to the host's friends (whoever has the host's ID among their
friends); anyone else needs the code. Profile names are shown to strangers, so the server
strips anything unprintable and cuts them to 24 characters. It refuses more than 16
connections from one address and drops a connection that sends an over-long line.

## Player IDs and friends

Every player profile gets an ID the first time it's needed: 12 characters from an alphabet
without look-alikes, `ABCD-EFGH-JKLM` (`Player IDs.txt`). It's the profile's friend code, and
what multiplayer saves are filed under. Typing a code is forgiving: spaces, dashes, O for 0 and
I or L for 1 are all accepted. Friends and recent players are per profile, in
`Friends/<ID>.txt`. Adding a friend is one-sided (like following): they see your private games
once they add you back.

## Not done (ideas for later)

- Direct connections (UDP hole punching) to save the relay's round trip; the relay is simple and
  always works, and turn-based co-op doesn't mind the latency.
- Mansion fingerprints in the game list, so games whose mansion you don't have are greyed out
  (today joining one says which mansion to install, as on a local network).
- Downloading a missing custom mansion from the host.
- Reporting/hiding players, an online name separate from the profile name.
- Split screen online (two players on one computer racing two on another).
- Carrying on without a player who left: today, when someone leaves a game on several
  computers, everyone is told who left and the game ends.
