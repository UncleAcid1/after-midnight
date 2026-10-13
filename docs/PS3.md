# After Midnight on the PS3

The PS3 version is being built step by step (the roadmap is in `CLAUDE.md`). Where it stands:

| | |
|---|---|
| Builds automatically | Every push to GitHub builds an installable `.pkg` (GitHub Actions, about 2 minutes). |
| Title screen and menus | Work in RPCS3 with a controller: all the menus, the Options tabs, Trophy Room, High Scores, Help, the on-screen keyboard. Sound and music play. Quit returns to the system. |
| Playing a mansion | Works in RPCS3: the rooms draw as on the Mac, Jack walks, jumps, collects and goes from room to room. Only a few minutes were played. |
| Split screen | 2 and 4 players start and draw in RPCS3 (with the pretend controllers below); not played through. |
| LAN and online | Not yet (milestone 4): the PS3 builds without networking for now. |
| Real PS3 | Not tested yet: everything so far was tested in RPCS3. |

## 1. Get the package from GitHub

1. Open the repository on github.com and click the **Actions** tab (at the top, next to
   "Pull requests").
2. In the list on the left click **Build**. The runs are listed newest first; each line is
   one push. A green check means it built, a red cross that it didn't, a yellow dot that it's
   still building (about 2 minutes).
3. Click the run you want (its title is the commit message).
4. Scroll to the bottom of the page, to **Artifacts**. Click **After-Midnight-PS3-xxxxxxx**
   (the letters are the start of the commit's ID). Your browser downloads a `.zip`.
5. Double-click the `.zip` in Downloads: inside is `After-Midnight-1.0-PS3.pkg` (about
   230 MB).

GitHub keeps the package for 3 days (it's big, and a free account has 500 MB for these). To
build one again, open **Actions → Build**, click **Run workflow** (on the right), pick the
branch and click the green **Run workflow** button.

The `-debug` artifact next to it holds the program with its debug information. You don't need
it: it's what turns a crash report from the log into the names of functions.

## 2. Play it in RPCS3 (on the Mac)

RPCS3 needs the PS3's system software once: download it from
[playstation.com](https://www.playstation.com/en-us/support/hardware/ps3/system-software/)
and install it with **File → Install Firmware**.

1. **File → Install Packages/Raps/Edats**, choose `After-Midnight-1.0-PS3.pkg`, then
   **Install**. (Installing again replaces the program and keeps your saves.)
2. **After Midnight** appears in RPCS3's game list. Double-click it. The first start takes a
   minute while RPCS3 prepares the game; later starts are quicker.
3. Controllers: **Pads** (the gamepad button in RPCS3's toolbar). Player 1 can be your
   controller, or the keyboard. For 4-player tests later, players 3 and 4 can be the keyboard
   or another controller.

In the game: **Cross** is A (choose, jump), **Circle** is B (back), **Square** the action
button, **Triangle** the map, **Start** pauses, the d-pad or left stick moves the highlight.

## 3. Play it on the PS3

Your PS3 needs nothing extra: Evilnat's CFW installs packages from the XMB.

1. On the Mac, copy `After-Midnight-1.0-PS3.pkg` to the top level of the USB flash drive
   (FAT32, not inside a folder). Eject the drive.
2. Plug the drive into one of the PS3's USB ports.
3. On the XMB go to **Game → Package Manager → Install Package Files → Standard**, choose
   **After Midnight** and confirm. (multiMAN can install packages too; either works.)
4. **After Midnight** is now in the **Game** column. Start it from there (or from multiMAN).

Leave the flash drive plugged in when you play: the game writes its log to it.

## 4. When something goes wrong: what to send

The game writes everything it does to **`After Midnight log.txt`** on the flash drive (the
run before is kept as `After Midnight log previous.txt`).

- **On the PS3**: leave the drive in, play until the problem, then quit (PS button → Quit
  Game). If the game froze and the PS button does nothing, switch the PS3 off with its power
  button (hold it until it beeps). Then put the drive in the Mac and send both log files.
- **In RPCS3**: the game's "flash drive" is a folder on the Mac. In Finder choose
  **Go → Go to Folder…** and paste `~/Library/Application Support/rpcs3/dev_usb000/`. Send the
  two log files from there, and RPCS3's own log, from **Go → Go to Folder…**
  `~/Library/Caches/rpcs3/` (the file `RPCS3.log`).
- **A photo or screenshot** of the screen helps (RPCS3: F12 saves one; on the PS3 a phone
  photo is fine), and a sentence on what you did just before.

If the game stops moving, after 3 seconds the log says what it was doing (`No new frame for
3000 ms ...`), with the last graphics operations. If it keeps drawing but stops reading the
controller, the log gets a line `The game hasn't read its input ... called from: ...` with
addresses; those, with the `-debug` artifact of the same run, say exactly where it was.

## 5. Testing aids

A text file **`After Midnight settings.txt`** at the top of the flash drive (or in the game's
folder) can switch on the same testing aids as on the computer, one per line:

    MM_DEBUG_AUTOSTART=0           # start the first mansion (Normal) straight away
    MM_DEBUG_SPLIT=coop:2          # with AUTOSTART: split screen co-op for 2 (or race:2, coop:4)
    MM_DEBUG_VIRTUAL_PADS=2        # pretend controllers for the empty player slots
    MM_PS3_DEBUG_PADS=1            # log every change on the controllers
    MM_PS3_DEBUG_FRAMES=1          # log the time of every frame

Lines starting with `#` are ignored. The log lists the settings it read at the top. Delete
the file (or empty it) to play normally.

## Where things are

| On the PS3 | |
|---|---|
| `/dev_hdd0/game/AMHD00001/USRDIR/` | the game: `EBOOT.BIN`, `Game Data`, `Game Data Extras` |
| `/dev_hdd0/game/AMHD00001/USRDIR/Saves/` | saved games, player profiles, settings, high scores |
| `/dev_usb000/After Midnight log.txt` | the log, on the flash drive (`dev_usb001`... if it's in another port) |

Deleting the game from the XMB deletes its saves too.

## How it works (for whoever works on it next)

- **Build**: `platform/ps3/Makefile`, in the ps3dev container (`hldtux/ps3dev-sdl2`: GCC 13
  for the PPU, PSL1GHT, SDL2 for PSL1GHT, cgcomp):

      docker run --rm -v "$PWD":/src -w /src hldtux/ps3dev-sdl2 make -f platform/ps3/Makefile -j4

  `make ... elf` builds only the program, `make ... gltest` a test picture for the drawing
  (`platform/ps3/gltest/gltest.c`). The package step uses PSL1GHT's `pkg.py` patched on the
  fly: as shipped it takes hours for the game's 120,000 files.
- **Drawing**: the game keeps calling the same OpenGL it calls on the Mac and Linux. On the
  PS3, `src/swx/BKGL.h` includes `src/ps3/PS3GL.h` instead of OpenGL, and `src/ps3/PS3GL.c`
  draws those calls with the RSX (librsx, four small Cg shaders in `src/ps3/shaders`). The
  Mac and Linux builds compile exactly as before.
- **Platform**: `src/ps3/PS3Platform.c` stands in for SDL's video and sound (SDL2_PSL1GHT's
  have no OpenGL, and its sound driver crashes), reads the controllers into SDL's virtual game
  controllers, writes the log, reads the settings file and handles Quit from the XMB.
- **Big-endian**: the game's pixel formats now say what they mean (`SDL_PIXELFORMAT_RGBA32`:
  the bytes R, G, B, A on any CPU; on a PC it's the same format as before), and the PS3 build
  uses `-fsigned-char` (PowerPC's `char` is unsigned; the original game's was signed).
- **Split screen**: `MMCoroutine.c` has a PPC64 context switch, and the game's variables are
  bracketed by `platform/ps3/GameDataBegin.c`/`GameDataEnd.c` so split screen swaps only
  them. 2 and 4 players run in RPCS3.
- **The RSX command buffer** is ours (1 MB), and PS3GL.c makes room before each group of
  commands: librsx's buffer-full callback can't be used (the code around it keeps writing at
  the old position, over the jump back).
- **Testing in a cloud session**: see "Testing the PS3 version" in `CLAUDE.md`.
