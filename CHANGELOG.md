# Changelog

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
