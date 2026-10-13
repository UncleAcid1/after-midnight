# Changelog

## Unreleased

### Controllers
- **Controllers menu** (Options → Controls → Controllers..., and in the Multiplayer lobby): every
  controller plugged in, by name, lighting up while it's used so you can tell them apart; which
  one runs the menus and plays alone (controller 1 unless you choose another, or any); the
  buttons for jump, action, map, jump left and jump right; rumble.
- The other controllers no longer move the menus.
- Multiplayer lobby: **Add a player** asks for their controls (the keyboard, or a controller by
  name), then their profile; each player's controls can be changed in every kind of game. A
  spare controller pressing A no longer adds a player.

### Multiplayer
- **Co-op on one computer**: only the controls of the player whose turn it is play (and
  rumble); players can pass one controller round.
- **Arcade Split Screen**: everyone in a room sees the same room. Platforms, falling blocks,
  trap doors, enemies and zappers no longer drift apart between the players' views (a player
  stood on a platform in their view and floated in another's): the player who has been in the
  room longest keeps it for the others.
- **Arcade Split Screen on the local network and online**, one player per computer, like Co-op
  and Ghost Race: the team's progress, score and items are shared, and so are the rooms.
- Split screen: a player on a controller can close a sign (it stayed open).
- Ghost Race and Arcade Split Screen: save statues work (lives back, and you come back there),
  instead of cutting the animation short; they no longer restart the music.
- Ghost Race between a PS3 and a computer: the ghosts were drawn in the wrong places (the
  computers wrote numbers in different byte orders). Network version 5: everyone needs this
  version to play together.

## After Midnight 1.1

The PlayStation 3 version: *After Midnight* now also runs on a PS3 with custom firmware (and in
the RPCS3 emulator), and plays together with the Mac and Linux versions.

### PS3
- An installable `.pkg`, built by GitHub Actions on every push, with its own XMB icon, background
  and sound by Thom Yorke. Each mansion is packed into one file, so it installs quickly.
- All the game: the 11 mansions, the 89 bundled custom mansions, Random Mansion, Trophy Room,
  high scores, the on-screen keyboard, the Fun and Extras options. Quit returns to the XMB.
- **Your own custom mansions** from a USB flash drive: a folder named
  `Midnight Mansion HD Custom Mansions` at the top of the drive.
- **Multiplayer**: Arcade Split Screen and Ghost Race for 2–4 players (one controller each), and
  Co-op and Ghost Race on the local network and online, with Macs and PCs too.
- **Picture size** (Options → Extras): Cross makes the picture smaller step by step (3% to 20%),
  for TVs that cut off the edges; a TV set to 4:3 gets the picture across its whole width, and
  **Stretch to fit** (Options) fills the screen. L1 / R1 switch the Options tabs.
- Watching a computer play in relay co-op, the PS3 keeps up instead of falling behind.
- The menus name the PS3's buttons (Cross, Circle, Square, Triangle).
- A log on the USB flash drive (`After Midnight log.txt`) for reporting problems.
- See [docs/PS3.md](docs/PS3.md) for installing it, in RPCS3 and on a PS3.

### Everywhere
- Relay co-op computes the same game on every computer (the game's own sine, cosine and random
  numbers, the same floating-point rules), and checks once a second that the computers agree:
  if they ever don't, it stops with a message instead of playing on in two different games.
- Controllers: signs close with any button; the map's mansions can be chosen; the Multiplayer
  screen and the Extras checkboxes can be reached; B goes back everywhere; lists start on Okay;
  Y deletes in the player list.
- **Larger text** (Options → Extras): bigger small print on the title and Multiplayer screens and
  for whose turn it is, and signs whose text fills the board.
- Random Mansion: a long name is drawn smaller to stay on the sign.
- Less memory: pictures are no longer kept twice once they are on the graphics card.

## After Midnight 1.0

The first release of *Midnight Mansion HD: After Midnight*, Vern Jensen's *Midnight Mansion HD*
(ActionSoft, 2011) rebuilt to run natively on today's Macs (Apple Silicon and Intel) and on
Linux. With the new options off, it plays exactly as the original did.

### Play together (2–4 players)
- **Co-op**: take turns and share the lives, on one computer, the local network or online.
- **Arcade Split Screen**: everyone at once in their own view of one mansion. Actions count
  for everyone, and every player has their own lives.
- **Ghost Race**: each player gets their own copy of the mansion, and the others show as ghosts.
  Fastest out wins. Playable on one computer, the local network or online.
- **Online**: public and private games through a lobby server, friend codes and friends lists,
  and chat (**T**) logged to `Chat Log.txt`.
- **Saves for the group**: a multiplayer save belongs to exactly the people who played it, and is
  offered when they're together again.
- The multiplayer smoke effect can be turned on or off.

### Play your way
- Game controllers everywhere: menus, play, an on-screen keyboard and rumble.
- **Fun** options: moon gravity, *Oops! All Spiders*, *Pitch Black*, *Jack on Espresso* and more.
- **Extras**: smooth room transitions, a centred main menu.
- **Trophy Room** of lifetime stats, 89 community **custom mansions** included, and
  **Random Mansion**.

### The lobby server
- One Python file that installs as a 24/7 service with `server/install.sh`.
- `mmhd-admin` control screen: who's on, disconnect, ban, and VPN blocking.
- Desktop shortcuts for Mac that open the control screen and update the server.
- See [docs/ONLINE.md](docs/ONLINE.md).

### For Mac
- A disk image to share the game: drag to Applications, over the original game's night sky.
  `platform/macos/make_dmg.sh` (or the **Make After Midnight DMG** shortcut) builds the newest
  code into a fresh one.

### Fixes
- The original's Black Chancery headings (map, high scores) show as intended. A font-cache bug
  had drawn every font in the game's folder as the first one loaded.
- Split screen: signs no longer hide the other players or draw over them, and the map (M) works.
  State no longer goes stale when leaving and re-entering rooms.
- A custom mansion's name no longer stays stuck on screen afterwards, and M no longer
  freezes the game after one.

### Credits
*Midnight Mansion* was created by **Vern Jensen**. *After Midnight* is a project by
**Thom Yorke** ([@UncleAcid1](https://github.com/UncleAcid1)), made possible using
Claude Opus 5.5 and strictly vibe coded. Full credits are in the [README](README.md#credits).
