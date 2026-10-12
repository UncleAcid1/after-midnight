#!/usr/bin/env python3
"""
Checks a Midnight Mansion HD lobby server from any computer: connects, says hello and asks
for the list of games, as the game does.

    python3 check_lobby.py 123.45.67.89            (port 47480)
    python3 check_lobby.py lobby.example.com:47480
"""

import json
import socket
import sys


def main():
    if len(sys.argv) != 2:
        print(__doc__.strip())
        return 2
    host, _, port = sys.argv[1].rpartition(":") if ":" in sys.argv[1] else (sys.argv[1], "", "47480")
    try:
        sock = socket.create_connection((host, int(port)), timeout=8)
    except OSError as e:
        print("Can't connect to %s:%s (%s)." % (host, port, e))
        print("Check that the server is running, and that port %s is open in the cloud firewall "
              "(Oracle: the subnet's security list) and on the server (install.sh does that)." % port)
        return 1
    f = sock.makefile("rwb")

    def ask(obj):
        f.write((json.dumps(obj) + "\n").encode())
        f.flush()
        return json.loads(f.readline())

    reply = ask({"op": "hello", "proto": 1, "id": "CHEK-CHEK-CHEK", "name": "check"})
    if reply.get("op") != "welcome":
        print("Connected, but it didn't answer like a lobby server:", reply)
        return 1
    games = ask({"op": "list", "friends": []})
    print("The lobby server at %s:%s works. Public games waiting: %d" % (host, port, len(games.get("games", []))))
    if reply.get("motd"):
        print("Message of the day:", reply["motd"])
    return 0


if __name__ == "__main__":
    sys.exit(main())
