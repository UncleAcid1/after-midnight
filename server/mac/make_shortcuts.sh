#!/bin/bash
# Makes double-click shortcuts for looking after the Midnight Mansion HD server from a Mac, and
# for making the game's Mac disk image:
#
#   Midnight Mansion Server.command          the server's control screen (mmhd-admin)
#   Update Midnight Mansion Server.command   copies the newest server files to the server and
#                                            reinstalls (bans and settings are kept), then
#                                            refreshes the shortcuts
#   Make After Midnight DMG.command          builds the newest game into a DMG (make_dmg.sh)
#
# Run it once from the game's folder:
#
#   bash server/mac/make_shortcuts.sh [--key <ssh key file>] [--server ubuntu@<address>] [--into <folder>]
#
# The shortcuts go in the folder that holds the game's folder when that's a folder on the Desktop
# (a "master folder"), otherwise on the Desktop; --into puts them elsewhere. They can be moved
# afterwards: the update refreshes them wherever they are. They find the SSH key by themselves
# (on the Desktop, in a folder on it, in Downloads or Documents) if it moves. The settings are
# kept in ~/.mmhd-server.conf.
# (Written for the bash that comes with macOS, 3.2.)

set -e
ARG_KEY=""
ARG_SERVER=""
INTO=""
while [ $# -gt 0 ]; do
    case "$1" in
        --key) ARG_KEY="$2"; shift 2;;
        --server) ARG_SERVER="$2"; shift 2;;
        --into) INTO="$2"; shift 2;;
        *) echo "Unknown option: $1" >&2; exit 1;;
    esac
done
HERE="$(cd "$(dirname "$0")/../.." && pwd)"
if [ ! -f "$HERE/server/mmhd_lobby.py" ]; then
    echo "Run this from the game's folder: bash server/mac/make_shortcuts.sh" >&2
    exit 1
fi

# ---------------------------------------------------------------------------------------------
# What the shortcuts share (written into each, and used here): the settings, finding the key
# and the game's folder

read -r -d '' SHARED <<'EOF' || true
CONF="$HOME/.mmhd-server.conf"
KEY=""; SERVER="ubuntu@129.158.224.129"; REPO=""; SHORTCUTS=""
[ -f "$CONF" ] && . "$CONF"

save_settings() {
    {
        echo "# Midnight Mansion HD server: used by the shortcuts (server/mac/make_shortcuts.sh)"
        echo "KEY=\"$KEY\""
        echo "SERVER=\"$SERVER\""
        echo "REPO=\"$REPO\""
        echo "SHORTCUTS=\"$SHORTCUTS\""
    } > "$CONF"
}

# The server's SSH key: where it was, or else the newest ssh-key-*.key in the usual places.
find_key() {
    [ -n "$KEY" ] && [ -f "$KEY" ] && return 0
    local f best=""
    for f in "$HOME"/Desktop/ssh-key-*.key "$HOME"/Desktop/*/ssh-key-*.key "$HOME"/Desktop/*/*/ssh-key-*.key \
             "$HOME"/Downloads/ssh-key-*.key "$HOME"/Downloads/*/ssh-key-*.key "$HOME"/Documents/ssh-key-*.key \
             "$HOME"/Documents/*/ssh-key-*.key "$HOME"/.ssh/ssh-key-*.key; do
        [ -f "$f" ] || continue
        if [ -z "$best" ] || [ "$f" -nt "$best" ]; then best="$f"; fi
    done
    [ -n "$best" ] || return 1
    KEY="$best"
    save_settings
    return 0
}

# The newest copy of the game's folder: the one set up last time, or a newer download of it.
find_game_folder() {
    local f best=""
    for f in "$REPO/server/mmhd_lobby.py" "$HOME"/Desktop/*/server/mmhd_lobby.py "$HOME"/Desktop/*/*/server/mmhd_lobby.py \
             "$HOME"/Downloads/*/server/mmhd_lobby.py "$HOME"/Downloads/*/*/server/mmhd_lobby.py \
             "$HOME"/Documents/*/server/mmhd_lobby.py "$HOME"/Documents/*/*/server/mmhd_lobby.py; do
        [ -f "$f" ] || continue
        if [ -z "$best" ] || [ "$f" -nt "$best" ]; then best="$f"; fi
    done
    [ -n "$best" ] || return 1
    GAME_FOLDER="$(cd "$(dirname "$best")/.." && pwd)"
    return 0
}

no_key() {
    echo "Couldn't find the server's SSH key (ssh-key-....key) on the Desktop, in a folder on it,"
    echo "in Downloads or in Documents. Put it in one of those, or say where it is (in Terminal, from"
    echo "the game's folder):  bash server/mac/make_shortcuts.sh --key \"<the key file>\""
}
EOF

# ---------------------------------------------------------------------------------------------
# The settings: saved ones, with what's given now winning

eval "$SHARED"
[ -n "$ARG_KEY" ] && KEY="$ARG_KEY"
[ -n "$ARG_SERVER" ] && SERVER="$ARG_SERVER"
REPO="$HERE"
if [ -n "$ARG_KEY" ] && [ ! -f "$KEY" ]; then
    echo "There's no key file at: $KEY" >&2
    exit 1
fi
if ! find_key; then
    no_key >&2
    exit 1
fi
    # where the shortcuts go
if [ -n "$INTO" ]; then
    SHORTCUTS="$(cd "$INTO" && pwd)"
elif [ -z "$SHORTCUTS" ] || [ ! -d "$SHORTCUTS" ]; then
    parent="$(dirname "$HERE")"
    if [ "$(dirname "$parent")" = "$HOME/Desktop" ]; then
        SHORTCUTS="$parent"                 # the game's folder is in a folder on the Desktop
    else
        SHORTCUTS="$HOME/Desktop"
    fi
fi
save_settings

# ---------------------------------------------------------------------------------------------
# The shortcuts

CONTROL="$SHORTCUTS/Midnight Mansion Server.command"
UPDATE="$SHORTCUTS/Update Midnight Mansion Server.command"
DMG="$SHORTCUTS/Make After Midnight DMG.command"

{
    echo '#!/bin/bash'
    echo '# Opens the Midnight Mansion HD server'"'"'s control screen. (Made by server/mac/make_shortcuts.sh.)'
    echo "$SHARED"
    cat <<'EOF'

printf '\033]0;Midnight Mansion Server\007'
clear
if ! find_key; then
    no_key
else
    chmod 600 "$KEY" 2>/dev/null
    echo "Connecting to the server ($SERVER)..."
    ssh -t -i "$KEY" -o ServerAliveInterval=30 -o ConnectTimeout=15 "$SERVER" mmhd-admin
    status=$?
    if [ $status -eq 255 ]; then
        echo
        echo "Couldn't reach the server. Is this Mac online? Is the server running in the Oracle console?"
    elif [ $status -ne 0 ]; then
        echo
        echo "The control screen didn't open. Run the \"Update Midnight Mansion Server\" shortcut, then try again."
    fi
fi
echo
echo "You can close this window now."
exit 0
EOF
} > "$CONTROL"

{
    echo '#!/bin/bash'
    echo '# Copies the newest Midnight Mansion HD server files to the server and reinstalls it (bans and'
    echo '# settings are kept), then refreshes these shortcuts. (Made by server/mac/make_shortcuts.sh.)'
    echo "$SHARED"
    cat <<'EOF'

finish() {
    echo
    echo "You can close this window now."
    exit 0
}

printf '\033]0;Update Midnight Mansion Server\007'
clear
if ! find_game_folder; then
    echo "Couldn't find the game's folder (with server/mmhd_lobby.py in it) on the Desktop, in a folder"
    echo "on it, in Downloads or in Documents."
    finish
fi
echo "Game folder: $GAME_FOLDER"
if [ -d "$GAME_FOLDER/.git" ] && command -v git >/dev/null; then
    echo "Getting the newest version (git pull)..."
    git -C "$GAME_FOLDER" pull --ff-only || echo "(couldn't pull; using the files as they are)"
fi
if ! find_key; then
    no_key
    finish
fi
chmod 600 "$KEY" 2>/dev/null

echo
echo "Copying the server files to $SERVER..."
files=("$GAME_FOLDER/server/mmhd_lobby.py" "$GAME_FOLDER/server/install.sh")
[ -f "$GAME_FOLDER/server/mmhd_admin.py" ] && files+=("$GAME_FOLDER/server/mmhd_admin.py")
if ! scp -i "$KEY" -o ConnectTimeout=15 "${files[@]}" "$SERVER": ; then
    echo
    echo "Couldn't copy them. Is this Mac online? Is the server running in the Oracle console?"
    finish
fi
echo
echo "Installing (the server restarts; games being played right now end)..."
if ! ssh -t -i "$KEY" -o ConnectTimeout=15 "$SERVER" "sudo bash install.sh"; then
    echo
    echo "The install didn't finish; what it says above tells why."
    finish
fi

    # these shortcuts, from the new version, where they are now
if bash "$GAME_FOLDER/server/mac/make_shortcuts.sh" --into "$(dirname "$0")" >/dev/null 2>&1; then
    echo
    echo "Shortcuts refreshed."
fi
echo
echo "All done: the server runs the newest version."
finish
EOF
} > "$UPDATE"

{
    echo '#!/bin/bash'
    echo '# Builds the newest After Midnight and makes its Mac disk image (DMG), with'
    echo '# platform/macos/make_dmg.sh from the newest copy of the game'"'"'s folder.'
    echo '# (Made by server/mac/make_shortcuts.sh.)'
    echo "$SHARED"
    cat <<'EOF'

printf '\033]0;Make After Midnight DMG\007'
clear
    # the newest copy of the game's folder that can make a DMG
best=""
for f in "$REPO/platform/macos/make_dmg.sh" "$HOME"/Desktop/*/platform/macos/make_dmg.sh \
         "$HOME"/Desktop/*/*/platform/macos/make_dmg.sh "$HOME"/Documents/*/platform/macos/make_dmg.sh \
         "$HOME"/Documents/*/*/platform/macos/make_dmg.sh "$HOME"/Documents/GitHub/*/platform/macos/make_dmg.sh \
         "$HOME"/Downloads/*/platform/macos/make_dmg.sh; do
    [ -f "$f" ] || continue
    if [ -z "$best" ] || [ "$f" -nt "$best" ]; then best="$f"; fi
done
if [ -z "$best" ]; then
    echo "Couldn't find the game's folder (with platform/macos/make_dmg.sh in it) on the Desktop,"
    echo "in a folder on it, in Documents or Downloads."
else
    bash "$best"
fi
echo
echo "You can close this window now."
exit 0
EOF
} > "$DMG"

chmod +x "$CONTROL" "$UPDATE" "$DMG"
xattr -d com.apple.quarantine "$CONTROL" "$UPDATE" "$DMG" 2>/dev/null || true
echo "Made the shortcuts in: $SHORTCUTS"
echo "  Midnight Mansion Server          double-click: the server's control screen"
echo "  Update Midnight Mansion Server   double-click: put the newest server version on the server"
echo "  Make After Midnight DMG          double-click: build the newest game into a Mac disk image"
echo "(Server: $SERVER   key: $KEY)"
