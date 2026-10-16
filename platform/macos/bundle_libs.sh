#!/bin/bash
# Copies the SDL2 dylib the game links against into the app bundle, points the
# executable at the bundled copy, and ad-hoc signs the bundle (required on Apple Silicon
# after install_name_tool changes the binary).
# Usage: bundle_libs.sh "<path to .app>"
set -e
APP="$1"
EXE="$APP/Contents/MacOS/$(defaults read "$APP/Contents/Info" CFBundleExecutable 2>/dev/null || ls "$APP/Contents/MacOS" | head -1)"
FW="$APP/Contents/Frameworks"
mkdir -p "$FW"
otool -L "$EXE" | awk '/libSDL2/ {print $1}' | while read -r LIB; do
    case "$LIB" in @*) continue ;; esac
    NAME=$(basename "$LIB")
    cp -fL "$LIB" "$FW/$NAME"
    chmod u+w "$FW/$NAME"
    install_name_tool -id "@executable_path/../Frameworks/$NAME" "$FW/$NAME"
    # Homebrew builds SDL2 for the Mac's own macOS (e.g. 26), which older macOS then refuses
    # to open ("too new"); mark it as running on macOS 11 and later like the game itself.
    SDK=$(vtool -show-build "$FW/$NAME" | awk '/sdk/ {print $2; exit}')
    vtool -set-build-version macos 11.0 "${SDK:-11.0}" -replace -output "$FW/$NAME" "$FW/$NAME"
    install_name_tool -change "$LIB" "@executable_path/../Frameworks/$NAME" "$EXE"
done
# Homebrew's "sdl2" is now sdl2-compat, which loads SDL3 at start-up; without SDL3 next to it
# the app aborts on a Mac that doesn't have Homebrew's sdl3 ("couldn't load SDL3").
# sdl2-compat looks for exactly "libSDL3.dylib" next to itself, so the copy is renamed.
SDL3=$(brew --prefix sdl3 2>/dev/null)/lib/libSDL3.0.dylib
if [ ! -f "$SDL3" ] && otool -L "$FW"/libSDL2*.dylib | grep -q . && [ -n "$(strings "$FW"/libSDL2*.dylib | grep -m1 libSDL3)" ]; then
    echo "ERROR: this SDL2 needs SDL3 but it was not found. Run: brew install sdl3" >&2
    exit 1
fi
if [ -f "$SDL3" ]; then
    cp -fL "$SDL3" "$FW/libSDL3.dylib"
    chmod u+w "$FW/libSDL3.dylib"
    install_name_tool -id "@loader_path/libSDL3.dylib" "$FW/libSDL3.dylib"
    SDK=$(vtool -show-build "$FW/libSDL3.dylib" | awk '/sdk/ {print $2; exit}')
    vtool -set-build-version macos 11.0 "${SDK:-11.0}" -replace -output "$FW/libSDL3.dylib" "$FW/libSDL3.dylib"
    if otool -L "$FW/libSDL3.dylib" | grep -q '/opt/homebrew\|/usr/local'; then
        echo "warning: libSDL3 links other Homebrew libraries; the app may not open on other Macs" >&2
    fi
fi
echo "Bundled libraries:"; ls "$FW"
codesign --force --deep --sign - "$APP"
