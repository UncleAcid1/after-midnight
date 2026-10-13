# After Midnight: what this edition adds

*Midnight Mansion HD: After Midnight* is Midnight Mansion HD (Vern Jensen, ActionSoft, 2011)
rebuilt to run on today's computers, with some new rooms in the house. Everything here is new; with the options off the game plays as the original. Settings live in
`Midnight Mansion HD Extras.txt` next to the other preferences, lifetime stats in
`Player Stats/<player>.txt`.

## Title screen

The edition's name shows under the logo, and "Midnight Mansion created by Vern Jensen" in the
bottom corner.

| Button | What it does |
|---|---|
| Multiplayer | The multiplayer screen (below): relay co-op, split screen co-op, races; on one computer or two |
| Custom Mansions | The mansions in `Game Data Extras/Custom Mansions` plus the player's own custom mansions folder (a star marks beaten ones) |
| Random Mansion | Dice roll through the same list, lands on one (R / X rolls again) |
| Trophy Room | The current player's lifetime stats |

The "More Games" tab is gone. **Options → Extras → Centered main menu** centres the menu.
Button art: `extras/Screens/titleScreen/buttons_extra.png`, made by `tools/make_extras.py` from the
original sheet plus new rows set in Plate Gothic Bold (`tools/fonts`).

## Options

Two new tabs, **Fun** and **Extras** (tab art in `extras/Sprites/optionsDialog`). Hovering an option
(mouse or controller) fades in a short description under the list.

Fun: You Wouldn't Download a Mansion, Medusa's Late for Work?, Jackie Watch the Stars (moon
gravity), Jack's Pinata Party, Oops! All Spiders (enemies look like spiders, act as themselves),
I'm Too S-S-Scared, Nine-Lived Malone and Jack on Espresso (mansions you have beaten), Pitch
Black, My Tuba!

Extras: Centered main menu, Smooth room transitions (the rooms slide; the game pauses for the
0.3 s slide and rooms load as before), Controller rumble, Smoke in multiplayer (the cloud of
smoke where a player dies; on by default).

## Controllers

Any SDL game controller. Menus: d-pad / stick moves a box (in the highlight colour) between the
buttons, A chooses, B goes back, X is the screen's extra action (roll again, new player, delete
a letter). In lists the d-pad moves through the list. Play: stick / d-pad to move, A jump,
X / B action, Y / Back map, Start pause. Moving the mouse hands control back to it.

Every text entry (player name, saved game, high score, host address) has an on-screen keyboard
(the Controls pane's key boxes, Action Man lettering) usable with a controller, the mouse or
the keyboard.

Rumble: a tap for keys and treasure, a heavy rumble for doors opening and blocks falling, a
jolt when Jack dies.

**Controllers** (Options → Controls → Controllers..., or the Multiplayer lobby): each controller
plugged in, by name (its line lights up while it's used); which one runs the menus and plays
alone (controller 1 unless another is chosen, or any controller); the buttons for jump, action,
map, jump left and jump right (choose one, then press the button; a button belongs to one action;
Start always pauses, and in menus A chooses and B goes back); rumble. Kept in
`Midnight Mansion HD Controllers.txt` next to the preferences. Only that controller moves the
menus; in multiplayer games each player's own controls play (see below).

## Multiplayer

**Multiplayer** on the title screen lifts the view off the mansion into the night sky (the
opening scroll in reverse): the mansion, hill and fog sink away, the moon drifts up above the panels and
the clouds settle along the bottom, and small dark ravens now and then cross the stars from left to right.
Escape / B / Back goes back down.

On the left are the kinds of game, their names lettered like the title screen's buttons
(`extras/Screens/multiplayer/mode_titles.png`, made by `tools/make_extras.py`; cyan under the
pointer, as on the title screen). The frame around the chosen kind shows only while the keys or
a controller are in use; with the mouse the chosen card is simply the brighter one. The
headings (Multiplayer, and the window's title) are in Black Chancery. On the right a
wood-panelled window lists the games:
**New game** on this computer, the games being hosted on the local network (found
automatically), **Host a game** and **Join by address**. A new game opens the lobby: who plays
(2-4 players), each with a profile, a character and (split screen) their controls; then
**Choose a mansion** (difficulty and mansion as usual) or **Custom mansion**. The lobby is
remembered in `Multiplayer.txt` next to the preferences. Mouse, keyboard and the menus'
controller all work. **Add a player** asks for their controls (the keyboard, or each controller
by name), then their profile; a player's controls field opens the same list. **Controllers...**
opens the Controllers menu.

| Game | Players | Where |
|---|---|---|
| Co-op | 2-4 take turns | one computer, or two |
| Arcade Split Screen | 2-4 at once, one shared mansion | split screen, or 2-4 computers |
| Ghost Race | 2-4 at once, a race, each in their own copy of the mansion | split screen, or 2-4 computers |

Characters: **Jack**, **Henry**, **Jack 2** and **Henry 2**; players 1-4 start as those, and
anyone can pick any of them. Jack 2 (a darker Jack) and Henry 2 (Henry) are placeholders until
they have their own art. Sprites: `extras/Sprites/playerN/` (N = 2 Henry, 3 Jack 2,
4 Henry 2) holding `00128.png` (Jack's sheet), `00130.png` (save statue) and `00131.png`
(caught in a web), the same sizes and layout as Jack's; drop new art over them. A missing file
falls back to Henry's (for Henry 2), then to Jack's.

In any multiplayer game a death bursts a cloud of smoke where the player fell (Options → Extras →
Smoke in multiplayer turns it off) and tolls a bell at 40% volume: `extras/Sounds/bell.ogg`, `.wav` or `.mp3` (sound effects can be MP3 now).

### Relay co-op

Everyone shares the mansion and the lives. When the player whose turn it is dies, the next
player comes in. On one computer each player has controls (the keyboard or a controller; one
controller can be passed round): only the controls of the player whose turn it is move Jack and
rumble. The status bar shows "Player N" above the score; the bottom right of the play
area and the banner when the turn changes use the player's profile name ("Ann's turn!").
Mansion Complete shows each player's share of the loot.

### Split screen (Arcade Split Screen and Ghost Race)

Each player gets an uncompromised 4:3 view: two side by side (on a 16:9 screen they fill it),
three or four two by two, with narrow gaps of the mansion's own dark brick between and around
them (`extras/Screens/split/brick.png`, made by `tools/make_split_art.py` from the wall tile; the
fourth corner with three players shows the mansion's name). The brick is only for split screen:
with one player on a computer (a race against another computer) the game fills the window
exactly as it does alone. Everyone hears the same music:
whoever walks into a part of the mansion with different music changes it for all; looping room
sounds (fire, zappers, snakes, lava) play as loud as the nearest player hears them.

Controls are per player, and nobody's input reaches anyone else's game:

| Controls | Keys |
|---|---|
| Arrow keys | the keys set in Options (arrows / keypad, Space, Return, M) |
| W A S D keys | W A S D, F (or left Shift) jump, Q / E jump left / right, G action, Tab map |
| Controller 1-4 | stick / d-pad, A jump, X / B action, Y / Back map (in the order they were plugged in) |

Escape, P or Start pauses everyone (Resume / Quit to title). Each player's map opens in their
own view and shows the other players' heads (tinted in their colour) with their names.
Save statues give the lives back and bring the player back there (no question asked: the
others are playing); in Arcade Split Screen they also save the team's game (see Saved games
below). Signs open over your own view; the other players stay where they are,
under the sign.

**Arcade Split Screen**: one mansion for the team. Each player runs their own copy of the room
they are in, so whatever one player does to the mansion is passed to the others standing in that
room and done in their copy too: levers, brick switches, doors unlocked with a key, enemies
killed, blocks that start to fall; gold and silver floors switch for everyone. What moves by
itself (platforms, falling blocks, trap doors, enemies, zappers) is kept the same in every copy:
the player who has been in the room longest keeps it, and the others' copies start each frame
from the keeper's (`src/game/MMRoomSync.c`), so the enemies follow the keeper's copy. Keys, treasure,
secrets and the score are shared; treasure one player takes vanishes for the others with a puff
of smoke; the status bars all show the team's items. Lives are each player's own: whoever runs
out sits out (their view says so) and the others play on. Anyone reaching the exit wins it for
the team; when everyone is out it's over.

**Ghost Race**: everyone plays their own copy of the mansion — a key turned or a switch
flipped on your screen does nothing to anyone else's. Rivals in the same room show as ghosts,
45% see-through and tinted (Jack's characters leave amber and gold dust, Henry's emerald and
cyan sparkles), with their names above them; they can't touch anything of yours. Each view
shows its race time; every life lost adds 15 seconds. Finishing shows your place; when everyone
has finished (or run out of lives) the results are shown, fastest first.

How it works: the game keeps its state in global variables and has one room loaded at a time,
so each player runs a whole copy of the game (`src/game/MMSplit.c`). The program's data and
bss sections are swapped in and out around each player's turn of the frame, each player has
their own SpriteWorld, layers, room and status bar, and what everyone shares (window, sound,
music, controllers, settings, and in co-op the mansion's progress) is left out of the swap.
Each game runs as a coroutine (`MMCoroutine.c`: a small context switch for x86-64 and ARM64),
so the game's own loops (fades, room slides, the map, signs, dying) yield after every frame;
each player draws into an offscreen framebuffer and the views are put together once a frame.

### Several computers

Two to four computers, one player each. One computer hosts (Multiplayer → a game type → **Host
a game**); while it waits it announces itself on the local network (UDP broadcast on port 47475,
with how many have joined), and on the other computers the game appears in the list (or use
**Join by address** with the address the host shows; TCP port 47474, open it on a firewall if
needed). Everyone waits in a waiting room listing who's in; the host presses **Okay** to start
once at least two are there, and picks the mansion. The host's computer is the hub: each guest
connects to it, and it passes every guest's messages on to the others. Every computer needs the
mansion: the custom mansions that come with the game (`extras/Custom Mansions`) are looked for as
well as the player's own folder. If a computer doesn't have it, that player is sent back to the
main menu with a message naming the mansion to install, and the others are told and go back
too. If someone leaves during the game, the others are told who and the game ends.

Over the internet: see **Online** below.

Relay co-op: every computer runs the same game from the same random seed and the host's Fun
settings; the one whose player has the turn sends its keys every frame and the others show the
same game. The turn goes round everyone in order. Escape on the watching computer offers to leave. Suspend is off in these games, and
the Fun tab is locked until the game ends.

Ghost Race: each computer plays its own copy of the mansion, full window, and sends where
its player is every frame; the other players show as ghosts (and on the map), with their
times over the view. Results when everyone is done.

Arcade Split Screen: as the race, one player per computer, and the team's game is shared: each
computer sends what its player collects or puts back, the events done, the rooms explored, lit
and mapped, the changes to the score, secrets and items (kMsgTeam), and the levers, switches,
doors, enemies and falling blocks; the keeper of a room sends its snapshot to the players in it
elsewhere a few times a second (kMsgRoomSnap), written field by field so a PS3 and a PC agree.
Anyone reaching the exit wins it for the team. (No team saves on several computers yet.)

### Online

**Play online...** in a game type's list (Co-op or Ghost Race) connects to the lobby server
(`server/mmhd_lobby.py`, see `docs/ONLINE.md`). Every computer connects out to the server, so
nobody has to open ports on their router; the server passes the game's messages between each
guest and the host, and from there the game plays exactly as on a local network (2 to 4
players; the list shows how many are in each game).

- **Online games**: public games, and your friends' private ones (marked `*`). Choose one to
  join it.
- **Host a public game**: listed for everyone. **Host a private game**: only your friends see it
  in their list; anyone else needs its code (`RAVEN-4821`), shown while you wait.
- **Join with a code...**
- **Friends**: your friend code, adding a friend by theirs, whether each friend is online and
  hosting (join their game from there), and the people you played with lately (add them as
  friends in one go). Friends are kept per profile in `Friends/<your ID>.txt`.
- **Lobby server**: its address (`host:port`), kept in `Online.txt`; `MM_LOBBY=host:port`
  overrides it.

Every player profile has a unique ID, made the first time it's needed (`Player IDs.txt`,
`ABCD-EFGH-JKLM`): it's the profile's friend code, and what multiplayer saves are filed under.

### Chat

In a game on several computers (local network or online) **T** opens a line at the bottom of the
play area: type, Return sends it, Escape closes it; while it's open the keys don't move your
player. The last lines show for a few seconds in both games. Everything said goes into
`Chat Log.txt` next to the preferences, with the date and time, under a heading for each game.

### Saved games

Multiplayer games are saved for the group that played them, never in a profile's own saved
games: in `Multiplayer Saves/<game>-<everyone's IDs>/`, with a `.txt` beside each save saying
who played (name, character, share of the loot) and whose turn it was. A save is only offered
when exactly the same people are together again, in the lobby of the same kind of game:
**Continue "Falcon Manor Normal 3"** appears above Choose a mansion. Add or swap a player and it's
gone (it comes back when the group does).

- **Co-op on one computer**: the pedestal asks for a name as usual ("Save the team's game as").
  Continuing gives the turn to whoever had it.
- **Co-op on several computers**: every computer keeps a copy. When the same group plays
  again, whoever hosts sees **Continue ...** in the mansion choice; the save goes to the other
  computers with the start, so it doesn't matter who hosts this time.
- **Arcade Split Screen**: a save pedestal saves the team's game there and then (no question:
  the others are still playing), "Team game saved" shows in that player's view, and they carry
  on. The save keeps what everyone shares and each player's room, place and lives.
- Ghost Race games aren't saved (they're races), and multiplayer games can't be suspended.

## Testing aids

| Variable | Effect |
|---|---|
| `MM_DEBUG_START_ROOM=<resID>` | New games start in that room |
| `MM_DEBUG_AUTOSTART=<n>` | Starts standard mansion n (0 Hint House, 1 Falcon Manor...) on Normal straight away |
| `MM_DEBUG_SPLIT=<game>:<players>` | With it: `coop:2`, `race:4`, `relay:3`... (no profiles needed); `coop:2:pads` plays on controllers 1, 2... |
| `MM_DEBUG_VIRTUAL_PADS=<n>` | n pretend controllers: pad 1 is J L I K (d-pad), U (A), O (B), H (X), Y (Y); pad 2 the number row 4 6 8 5, 7, 9, 3, 0 |
| `MM_DEBUG_LIVES=<n>` | Every game starts with n lives |
| `MM_DEBUG_NET=host:<mode>` / `join:<address>` | With AUTOSTART, hosts a game on the network (mode 0 Co-op, 1 Ghost Race, 2 Arcade Split Screen); or joins one |
| `MM_DEBUG_NET_START=<n>` | The host starts once n players are in |
| `MM_DEBUG_NO_ROOM_SYNC=1` | Arcade Split Screen: each player's copy of a room runs on its own (to compare) |
| `MM_DEBUG_CHEATS=1` | F6 loses a life (in a game on several computers, on the one with the turn, and the others follow); in split screen F5/F6 player 1 finishes/dies, F7/F8 player 2, F9/F10, F11/F12 |
| `MM_OLD_MULTIPLAYER_DIALOG=1` | Multiplayer opens the old simple dialog instead of the screen |
| `MM_DEBUG_PAD=1` | F5-F8 d-pad, F9 A, F10 B, F11 X, F12 Y (controller menus without a controller) |
| `MM_LOBBY=<host:port>` | The lobby server for online play (instead of `Online.txt`) |
| `MM_EXTRAS_DIR=<folder>` | Where to find the "Game Data Extras" files |

## Notes

- Deaths are counted by what killed Jack: lava, zappers, Medusas, long falls, spiders,
  skeletons, eyeballs, other creatures. The game has no spike traps.
- Plate Gothic (`tools/fonts`) is a commercial font, used only to render the button art.
- Fonts: the original kept only the first 50 characters of a font file's path, so every font in
  the game's folder came out as whichever loaded first (Black Chancery in the map and high
  score headings showed as Action Man). Font paths are kept whole now.
- MP3 decoding: `third_party/dr_libs/dr_mp3.h` (David Reid; public domain or MIT-0).
