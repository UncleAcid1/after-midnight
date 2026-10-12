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
    install_name_tool -change "$LIB" "@executable_path/../Frameworks/$NAME" "$EXE"
done
codesign --force --deep --sign - "$APP"
