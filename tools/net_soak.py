#!/usr/bin/env python3
"""Soak test for games on several computers: two to four copies of the game on this computer,
each on its own virtual screen (Xvfb) with its own player profile, playing by themselves
(MM_DEBUG_BOT), through a connection that delivers data in bursts with now and then a stall,
as Wi-Fi and the internet do. It watches for crashes, failed checks and games stopping, and in
Co-op compares the copies' game states frame by frame (MM_DEBUG_CHECKSUM).

    python3 tools/net_soak.py --mode coop --players 2 --minutes 10
    python3 tools/net_soak.py --mode arcade --players 4 --online --minutes 10 --hog
    python3 tools/net_soak.py --mode race --players 3 --lives 3 --stall

Needs Linux with Xvfb (and ImageMagick's `import` for the screenshots at the end), a built game
(build/MidnightMansionHD) and the original data (original/...). --online starts a lobby server
of its own (server/mmhd_lobby.py) on this computer. Everything goes in --out (logs, profiles,
screenshots). Exit status 0 when nothing went wrong.
"""

import argparse
import os
import random
import re
import shutil
import signal
import socket
import subprocess
import sys
import tempfile
import threading
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MODES = {"coop": 0, "race": 1, "arcade": 2}
NAMES = ["Ann", "Bob", "Cy", "Di"]
LAN_PORT = 47474


# ---------------------------------------------------------------------------
# A bursty connection: every chunk waits for the next "release" (every 0-250 ms at random,
# with an occasional 600 ms stall)

def burst_pipe(src, dst, rnd, jitter):
    buf, lock, done = bytearray(), threading.Lock(), [False]

    def reader():
        while True:
            try:
                data = src.recv(65536)
            except OSError:
                data = b""
            if not data:
                done[0] = True
                return
            with lock:
                buf.extend(data)

    threading.Thread(target=reader, daemon=True).start()
    while not (done[0] and not buf):
        time.sleep((0.6 if rnd.random() < 0.03 else rnd.uniform(0, 0.25)) if jitter else 0.005)
        with lock:
            out = bytes(buf)
            buf.clear()
        if out:
            try:
                dst.sendall(out)
            except OSError:
                return
    try:
        dst.shutdown(socket.SHUT_WR)
    except OSError:
        pass


def start_proxy(listen_port, target_port, jitter):
    server = socket.socket()
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind(("127.0.0.1", listen_port))
    server.listen(64)

    def serve():
        while True:
            client, _ = server.accept()
            try:
                target = socket.create_connection(("127.0.0.1", target_port))
            except OSError:
                client.close()
                continue
            for a, b in ((client, target), (target, client)):
                a.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
                threading.Thread(target=burst_pipe, args=(a, b, random.Random(), jitter), daemon=True).start()

    threading.Thread(target=serve, daemon=True).start()


def free_port():
    s = socket.socket()
    s.bind(("127.0.0.1", 0))
    port = s.getsockname()[1]
    s.close()
    return port


# ---------------------------------------------------------------------------
# Comparing Co-op logs (MM_DEBUG_CHECKSUM): "checksum frame N: state" then "  layers: ..."

def read_checks(path):
    checks, frame = {}, None
    try:
        with open(path, errors="replace") as f:
            for line in f:
                m = re.match(r"checksum frame (\d+): (.*)", line)
                if m:
                    frame = int(m.group(1))
                    state = re.sub(r" rand \w+", "", m.group(2))    # (GetRandom's numbers may differ)
                    checks[frame] = [state, {}]
                elif line.startswith("  layers:") and frame in checks:
                    checks[frame][1] = dict(x.split(":", 1) for x in line.split(":", 1)[1].split())
    except OSError:
        pass
    return checks


def compare(host_log, guest_log, quiet_layers):
    a, b = read_checks(host_log), read_checks(guest_log)
    common = sorted(set(a) & set(b))
    first_state = first_layer = None
    for frame in common:
        if first_state is None and a[frame][0] != b[frame][0]:
            first_state = (frame, a[frame][0], b[frame][0])
        layers = [k for k in set(a[frame][1]) | set(b[frame][1])
                  if a[frame][1].get(k) != b[frame][1].get(k) and k not in quiet_layers]
        if first_layer is None and layers:
            first_layer = (frame, sorted(layers, key=int))
    return len(common), first_state, first_layer


# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--mode", choices=MODES, default="coop")
    parser.add_argument("--players", type=int, default=2, choices=[2, 3, 4])
    parser.add_argument("--minutes", type=float, default=10)
    parser.add_argument("--online", action="store_true", help="through a lobby server (started here)")
    parser.add_argument("--mansion", type=int, default=1, help="standard mansion (0 Hint House, 1 Falcon Manor...)")
    parser.add_argument("--room", help="start room (resource ID)")
    parser.add_argument("--lives", type=int, default=99)
    parser.add_argument("--no-jitter", action="store_true", help="a smooth connection")
    parser.add_argument("--hog", action="store_true", help="keep the processors busy half the time")
    parser.add_argument("--stall", action="store_true", help="freeze one game for 30 s, a third of the way in")
    parser.add_argument("--game", default=os.path.join(ROOT, "build", "MidnightMansionHD"))
    parser.add_argument("--resources", default=os.path.join(ROOT, "original", "Midnight Mansion HD.app",
                                                            "Contents", "Resources"))
    parser.add_argument("--out", default=None, help="folder for logs and screenshots (default: a new one)")
    parser.add_argument("--display", type=int, default=91, help="first Xvfb display number")
    args = parser.parse_args()

    out = os.path.abspath(args.out or tempfile.mkdtemp(prefix="net_soak_"))
    os.makedirs(out, exist_ok=True)
    print("Logs and screenshots:", out)
    procs, games = [], []

    def stop_all():
        for p in games + procs:
            try:
                p.send_signal(signal.SIGCONT)
                p.kill()
            except OSError:
                pass

    try:
        displays = [args.display + i for i in range(args.players)]
        for d in displays:
            procs.append(subprocess.Popen(["Xvfb", ":%d" % d, "-screen", "0", "1280x800x24"],
                                          stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL))
        time.sleep(1.5)

        env = dict(os.environ, SDL_AUDIODRIVER="dummy", MM_RESOURCES_DIR=args.resources, MM_DEBUG_BOT="1",
                   MM_DEBUG_CHEATS="1", MM_DEBUG_LIVES=str(args.lives), MM_DEBUG_AUTOSTART=str(args.mansion))
        if args.room:
            env["MM_DEBUG_START_ROOM"] = args.room
        if args.mode == "coop":
            env["MM_DEBUG_CHECKSUM"] = "1"
        mode = MODES[args.mode]

        if args.online:
            lobby_port = free_port()
            state = tempfile.mkdtemp(prefix="mmhd")         # (a short path: the admin socket is in it)
            procs.append(subprocess.Popen([sys.executable, os.path.join(ROOT, "server", "mmhd_lobby.py"),
                                           "--host", "127.0.0.1", "--port", str(lobby_port), "--state", state,
                                           "--admin-socket", os.path.join(state, "a.sock")],
                                          stdout=open(os.path.join(out, "lobby.log"), "w"), stderr=subprocess.STDOUT))
            proxy_port = free_port()
            start_proxy(proxy_port, lobby_port, not args.no_jitter)
            env["MM_LOBBY"] = "127.0.0.1:%d" % proxy_port
            time.sleep(2)
        else:
            proxy_port = free_port()
            start_proxy(proxy_port, LAN_PORT, not args.no_jitter)

        logs = []
        for i, d in enumerate(displays):
            home = os.path.join(out, "home%d" % (i + 1))
            os.makedirs(home, exist_ok=True)
            e = dict(env, DISPLAY=":%d" % d, HOME=home, MM_DEBUG_PLAYER=NAMES[i])
            if i == 0:
                e["MM_DEBUG_NET"] = ("online-host:%d" if args.online else "host:%d") % mode
                e["MM_DEBUG_NET_START"] = str(args.players)
            elif args.online:
                ids = os.path.join(out, "home1", ".local", "share", "ActionSoft", "Midnight Mansion", "Player IDs.txt")
                for _ in range(60):
                    host_id = next((l.split("\t")[0] for l in open(ids) if "\t" in l and not l.startswith("#")),
                                   None) if os.path.exists(ids) else None
                    if host_id:
                        break
                    time.sleep(0.5)
                e["MM_DEBUG_NET"] = "online-join:%s" % host_id
            else:
                e["MM_DEBUG_NET"] = "join:127.0.0.1:%d" % proxy_port
            log = os.path.join(out, "player%d.log" % (i + 1))
            logs.append(log)
            games.append(subprocess.Popen(["stdbuf", "-oL", args.game], env=e, cwd=out,
                                          stdout=open(log, "w"), stderr=subprocess.STDOUT))
            time.sleep(5 if i == 0 else 2)

        if args.hog:
            for _ in range(os.cpu_count() or 2):
                procs.append(subprocess.Popen(["timeout", str(int(args.minutes * 30)), "sh", "-c", "while :; do :; done"]))
        stall_at = time.time() + args.minutes * 20 if args.stall else 0
        end = time.time() + args.minutes * 60
        problems = []
        bad = re.compile(r"out of step|ASSERT FAILED|The game crashed|left the game")
        while time.time() < end and not problems:
            time.sleep(5)
            if stall_at and time.time() >= stall_at:
                victim = random.choice(games[1:])
                print("Freezing player %d's game for 30 s" % (games.index(victim) + 1))
                victim.send_signal(signal.SIGSTOP)
                time.sleep(30)
                victim.send_signal(signal.SIGCONT)
                stall_at = 0
            for i, g in enumerate(games):
                if g.poll() is not None:
                    problems.append("player %d's game quit (exit status %s)" % (i + 1, g.returncode))
            for i, log in enumerate(logs):
                with open(log, errors="replace") as f:
                    for line in f:
                        if bad.search(line):
                            problems.append("player %d: %s" % (i + 1, line.strip()))
                            break

        if shutil.which("import"):
            for i, d in enumerate(displays):
                subprocess.call(["import", "-display", ":%d" % d, "-window", "root", "-resize", "50%",
                                 os.path.join(out, "player%d.png" % (i + 1))], stderr=subprocess.DEVNULL)

        if args.mode == "coop":
                # the map leaves its hidden backdrop (layer 0) and the lantern's light (56) where
                # it was on the computer that opened it: only looks
            for i in range(1, args.players):
                n, state, layer = compare(logs[0], logs[i], {"0", "56"})
                print("Player 1 and %d: %d frames compared" % (i + 1, n))
                if n == 0:
                    problems.append("player 1 and %d: nothing to compare (did the game start?)" % (i + 1))
                if state:
                    problems.append("player 1 and %d differ at frame %d:\n    %s\n    %s" % ((i + 1,) + state))
                if layer:
                    print("  (sprites first differed at frame %d, layers %s)" % layer)

        if problems:
            print("PROBLEMS:")
            for p in problems:
                print("  " + p)
            return 1
        print("OK: %.0f minutes, %d players, %s%s, nothing went wrong" %
              (args.minutes, args.players, args.mode, " online" if args.online else ""))
        return 0
    finally:
        stop_all()


if __name__ == "__main__":
    sys.exit(main())
