#!/bin/bash
# Makes the Mac disk image (DMG) for After Midnight from the newest source code:
#
#   1. gets the newest version of the code (git pull, when this folder is a git checkout)
#   2. builds the game (a Release build in build-release/)
#   3. makes dist/After-Midnight-<version>-macOS-<chip>.dmg: a window with the game's icon,
#      an arrow and the Applications folder over the original game's night sky, and the
#      game's icon on the disk itself
#
# Run it from Terminal (from anywhere):   bash platform/macos/make_dmg.sh
# or double-click the "Make After Midnight DMG" shortcut (server/mac/make_shortcuts.sh).
#
# The first run installs what it needs with Homebrew (cmake, sdl2, create-dmg). macOS asks
# once whether Terminal may control Finder: say OK, that's how the window gets laid out.
#
# The background is platform/macos/dmg/background.png (+ @2x), drawn by
# tools/make_dmg_background.py; the icon positions below must match it.

set -e
cd "$(dirname "$0")/../.."
ROOT="$(pwd)"
APP_NAME="Midnight Mansion HD"
BUILD="$ROOT/build-release"
DIST="$ROOT/dist"

say() { printf '\n\033[1;33m==> %s\033[0m\n' "$1"; }
fail() { printf '\n\033[1;31m%s\033[0m\n' "$1" >&2; exit 1; }

[ "$(uname)" = "Darwin" ] || fail "DMGs can only be made on a Mac."

# ---------------------------------------------------------------------------------------------
say "Getting the newest version of the code"
if [ -d .git ] && command -v git >/dev/null; then
    if [ -n "$(git status --porcelain --untracked-files=no)" ]; then
        echo "This folder has changes of its own, so it isn't updated; building it as it is."
    elif git pull --ff-only; then
        echo "Up to date: $(git log -1 --format='%h %s')"
    else
        echo "Couldn't get the newest version (offline, or GitHub needs you to sign in)."
        echo "Building the code as it is. (In GitHub Desktop: Fetch origin, then Pull.)"
    fi
else
    echo "This folder isn't a git checkout, so it's built as it is."
    echo "(To always build the newest version, get the code with GitHub Desktop or git clone.)"
fi

# ---------------------------------------------------------------------------------------------
say "Checking the tools"
if ! command -v brew >/dev/null; then
    fail "Homebrew is needed: install it from https://brew.sh, then run this again."
fi
for pkg in cmake sdl2 create-dmg; do
    if ! brew list --versions "$pkg" >/dev/null 2>&1; then
        echo "Installing $pkg..."
        brew install "$pkg"
    fi
done
xcode-select -p >/dev/null 2>&1 || fail "Apple's command line tools are needed: run  xcode-select --install  then this again."

# ---------------------------------------------------------------------------------------------
say "Building the game"
cmake -S "$ROOT" -B "$BUILD" -DCMAKE_BUILD_TYPE=Release >/dev/null
cmake --build "$BUILD" -j "$(sysctl -n hw.ncpu)"
APP="$BUILD/$APP_NAME.app"
[ -d "$APP" ] || fail "The build didn't make $APP_NAME.app."

# ---------------------------------------------------------------------------------------------
say "Making the disk image"
VERSION="$(sed -n 's/^## After Midnight \([0-9][0-9.]*\).*/\1/p' CHANGELOG.md 2>/dev/null | head -1)"
[ -n "$VERSION" ] || VERSION="dev"
if [ -d .git ] && [ "$VERSION" != "dev" ]; then
        # builds made after the release's own commit say which commit they're from
    if [ "$(git log -1 --format=%H -- CHANGELOG.md)" != "$(git rev-parse HEAD)" ]; then
        VERSION="$VERSION+$(git rev-parse --short HEAD)"
    fi
fi
CHIP="$(uname -m)"
[ "$CHIP" = "arm64" ] && CHIP="AppleSilicon" || CHIP="Intel"
DMG="$DIST/After-Midnight-$VERSION-macOS-$CHIP.dmg"

STAGE="$BUILD/dmg-staging"
rm -rf "$STAGE"
mkdir -p "$STAGE" "$DIST"
ditto "$APP" "$STAGE/$APP_NAME.app"
    # one background with both sizes, so Retina screens get the sharp one
tiffutil -cathidpicheck platform/macos/dmg/background.png platform/macos/dmg/background@2x.png \
    -out "$BUILD/dmg-background.tiff" >/dev/null 2>&1 || cp platform/macos/dmg/background.png "$BUILD/dmg-background.tiff"
rm -f "$DMG"

# Icon centres: the game at (175, 235), Applications at (485, 235), in a 660x420 window
# (tools/make_dmg_background.py draws the arrow between them).
if ! create-dmg \
        --volname "After Midnight" \
        --volicon "$APP/Contents/Resources/mm_icon.icns" \
        --background "$BUILD/dmg-background.tiff" \
        --window-pos 200 120 \
        --window-size 660 420 \
        --icon-size 112 \
        --text-size 13 \
        --icon "$APP_NAME.app" 175 235 \
        --hide-extension "$APP_NAME.app" \
        --app-drop-link 485 235 \
        --hdiutil-quiet \
        "$DMG" "$STAGE"; then
    echo
    echo "If it says Terminal isn't allowed to control Finder: open System Settings > Privacy &"
    echo "Security > Automation, turn on Finder under Terminal, and run this again."
    fail "The disk image wasn't made."
fi
rm -rf "$STAGE"

say "Done"
echo "$DMG ($(du -h "$DMG" | cut -f1))"
open -R "$DMG"
