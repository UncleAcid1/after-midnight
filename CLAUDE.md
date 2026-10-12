# Notes for Claude (read first)

*Midnight Mansion HD: After Midnight*: Vern Jensen's *Midnight Mansion HD* (ActionSoft, 2011)
reconstructed in C99 + SDL2 + OpenGL, with 2–4 player multiplayer (one computer, LAN, online
through `server/`). Owner: **Thom Yorke (@UncleAcid1)**, new to GitHub; explain GitHub steps
plainly, one at a time. Read `README.md`, `docs/EXTRAS.md`, `docs/ONLINE.md` and `docs/PLAN.md`
for the details.

## Rules

- Never write the owner's real name anywhere (files, commits). Credit **Thom Yorke** / UncleAcid1.
- Credit Vern Jensen as the creator; don't speak for him (no endorsement implied).
- Commits are authored as `Claude <noreply@anthropic.com>`.
- With the new options off, the game must play exactly as the original.
- `CHANGELOG.md`'s first `## After Midnight X` heading is the version (`platform/macos/make_dmg.sh`
  reads it).
- This repository is private; the old one (`UncleAcid1/mmhddecomped`) holds the earlier history.
  It also contains the original game data, a commercial font (`tools/fonts`) and a reconstruction
  without the original's copy protection: discuss rights (contact Vern Jensen, or players supplying
  their own data) before anything goes public.

## Infrastructure

- Lobby server: Oracle Cloud instance `129.158.224.129`, port 47480 (`kDefaultLobby`), installed with
  `server/install.sh`, run as systemd `mmhd-lobby`; `mmhd-admin` on the server. The owner has Mac
  shortcuts from `server/mac/make_shortcuts.sh` (control screen, server update, Make DMG).
- Mac DMG: `platform/macos/make_dmg.sh` (create-dmg; background from `tools/make_dmg_background.py`).
- Cloud sessions can't create repositories, push tags, delete branches or create releases: the
  owner does those on github.com.

## Testing in a cloud session

Build with CMake, run under Xvfb (`MM_RESOURCES_DIR=".../original/Midnight Mansion HD.app/Contents/Resources"`),
drive with xdotool, screenshot with ImageMagick `import`. Testing aids: `MM_DEBUG_AUTOSTART=<n>`,
`MM_DEBUG_SPLIT=coop:4` / `race:2`, `MM_DEBUG_CHEATS=1` (F6 loses a life), `MM_DEBUG_START_ROOM`,
`MM_LOBBY=127.0.0.1:<port>` with a local `server/mmhd_lobby.py`. Separate `HOME`s give separate
profiles for several instances.

## Roadmap (agreed; nothing started yet). The owner's priority is the PS3; Windows comes later.

The owner has a PS3 Slim on the latest **Evilnat** CFW, multiMAN, moves files by **USB flash drive**
(FAT32), and tests in **RPCS3** on the Mac. Only one or two real controllers: 4-player tests are
done in RPCS3 (pads 3-4 mapped to the keyboard or another controller), so keep a testing aid that
fills idle player slots. The PS3 version writes a log to the flash drive
(`/dev_usb000/After Midnight log.txt`) when one is present; the owner brings it back for debugging.

1. **PS3 build pipeline**: the PSL1GHT toolchain in **GitHub Actions** (Linux runners, a ps3dev
   container), producing an installable `.pkg` the owner downloads from the run's page. Mac and
   Linux keep building as they do; the Mac DMG stays a local build (macOS runners cost 10x the
   free minutes).
2. **Drawing through SDL2's renderer** (or a thin layer of our own) instead of fixed-function
   OpenGL, so the same game draws on the PS3's RSX (e.g. Tiny3D, or SDL2's PSL1GHT port) and on
   the PCs. Mac and Linux must look exactly as before.
3. **PS3 milestone 1: the menus in RPCS3.** Concerns: big-endian PPC (the original data is
   big-endian too; audit the byte swapping), a PPC64 context switch for the split-screen
   coroutines (`MMCoroutine.c`), paths (data in the game's USRDIR, saves in its own folder), the
   flash-drive log.
4. **PS3 milestone 2: a mansion on the real PS3.** 256 MB of RAM (load per mansion, free
   afterwards), speed, TV-safe margins, controllers.
5. **PS3 milestone 3: 4-controller split screen** (tested in RPCS3).
6. **Deterministic co-op across platforms**, needed before PS3 and PCs play together:
   `-ffp-contract=off` and strict float settings everywhere, identical math functions, and a
   periodic game-state checksum in relay co-op that reports a desync instead of drifting.
   (Ghost Race sends positions, so it's tolerant.)
7. **PS3 milestone 4: LAN and online play with PCs** (the PS3's netlib; the lobby needs no changes).
8. **PS3 polish and the 1.1 release**: `.pkg` with XMB art (ICON0/PIC1), Quit to XMB, custom
   mansions from USB, no trophies.
9. **Windows** (later; the owner tests with **CrossOver** on the Mac, a real PC as a final check):
   Winsock for `MMNet.c`/`MMOnline.c`, Fibers for `MMCoroutine.c`, `%APPDATA%`, and an Actions build.
