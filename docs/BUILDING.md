# Building Midnight Mansion HD: After Midnight

The game needs the original game data (`Game Data` folder, ~400 MB), which is in
`original/Midnight Mansion HD.app/Contents/Resources/` in this repository.

## macOS (Apple Silicon or Intel)

1. Install the tools (once), with [Homebrew](https://brew.sh):

       xcode-select --install
       brew install cmake sdl2

2. Build:

       git clone https://github.com/UncleAcid1/after-midnight.git
       cd after-midnight
       cmake -B build -DCMAKE_BUILD_TYPE=Release
       cmake --build build -j

3. Run it:

       open "build/Midnight Mansion HD.app"

   You can also drag `Midnight Mansion HD.app` to Applications. The build copies the game
   data and SDL2 into the app, so it doesn't depend on the repository afterwards.

Saved games, player profiles, replays and settings go in
`~/Library/Application Support/ActionSoft/Midnight Mansion/`.

## The Mac disk image (DMG)

To share the game, make a disk image: a window with the game's icon, an arrow and the
Applications folder over the original game's night sky, ready to drag and install.

    bash platform/macos/make_dmg.sh

It gets the newest code first (`git pull`, when the folder is a git checkout), installs what it
needs with Homebrew the first time (cmake, sdl2, create-dmg), builds the game, and leaves
`dist/After-Midnight-<version>-macOS-<chip>.dmg` (the version comes from `CHANGELOG.md`; a
build after the release's commit adds the commit, `1.0+3f2a1b9`). The first time, macOS asks
whether Terminal may control Finder: allow it, that's how the window is laid out. The
**Make After Midnight DMG** shortcut (made by `server/mac/make_shortcuts.sh`) does the same
with a double-click.

The DMG holds a build for the Mac's own chip (Apple Silicon or Intel), signed ad hoc rather
than with an Apple developer certificate: the first time, open the game with right-click →
Open (or System Settings → Privacy & Security → Open Anyway). Its background is
`platform/macos/dmg/background.png` (+ `@2x`), drawn by `tools/make_dmg_background.py`.

## Linux

    sudo apt install cmake libsdl2-dev
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build -j
    MM_RESOURCES_DIR="$PWD/original/Midnight Mansion HD.app/Contents/Resources" ./build/MidnightMansionHD

(`MM_RESOURCES_DIR` is the folder that contains `Game Data`; by default the game looks next to
the executable.)

## Controls

Arrow keys (or keypad 4/6/8/2) to walk and climb, Space to jump, keypad 7/9 to jump left/right,
Return for action, M for the map, Escape to pause. Everything can be changed under
Options → Controls. Game controllers (Xbox, PlayStation, Switch and others) work in menus and
in play without any setup. What this version adds (Fun and Extras options, multiplayer on one
or several computers, Trophy Room, custom mansions) is described in [EXTRAS.md](EXTRAS.md).
