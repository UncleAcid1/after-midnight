# Midnight Mansion HD — source reconstruction

Goal: rebuild C source for *Midnight Mansion HD – Episode 1* (v1.0.1b, Mac, 2011, ActionSoft)
that builds and runs natively on modern macOS (Apple Silicon), without the registration,
demo-limit, and anti-piracy code. Later: an arcade-style variant portable to other platforms.

This is a **functional** reconstruction (behaves the same), not a byte-matching decomp.

## What we have

| Path | Contents |
|---|---|
| `original/` | Unmodified app bundle (executable + `Game Data`). |
| `reference/filemap.tsv` | Every function, mapped to its original source file (from STABS debug info). |
| `reference/globals.tsv` | Global/static variables with addresses and owning source file. |
| `reference/decomp/` | Raw Ghidra C output, one file per original source file. Read-only reference. |
| `third_party/SpriteWorldX/` | Public SpriteWorldX (zlib). The game uses a later private fork of it. |
| `tools/` | `decompile.sh` regenerates `reference/` (needs Ghidra 11.4.2 + `pip install lief`). |

The executable is a universal i386 + PPC binary. We work from the **i386** slice. Symbols are not
stripped: all ~2000 functions and ~1000 globals have their original names.

## Original code layout (i386, bytes of code)

**Game (rewrite from decomp):** `Midnight Mansion.c` 44K, `NewSprite.c` 49K,
`SpriteMoveProcs.c` 50K, `GameScreens.c` 41K, `GameSupport.c` 29K, `MoveProcSupport.c` 26K,
`Level.c` 22K, `GameMap.c` 16K, `SpriteFrameProcs.c` 15K, `MMDialogsMac.c` 15K,
`MyButtonSprite.c` 13K, `Levers.c` 12K, `SpriteCollideProcs.c` 12K, `Compatibility Layer.c` 11K,
`MySWUtils.c` 11K, `GameEditorStuff.c` 11K, `MMEvents.c` 8K, `MyFileUtilsMac.c` 8K,
`GameArrays.c` 7K, `High Scores.c` 6K, `ZapperStuff.c` 6K, `MyUtils.c` 5K, smaller files.

**Libraries (use upstream source):** SpriteWorldX (~130K; public base + ~34K of fork-only
functions to reconstruct: GL/VBO blitting, tile master surfaces, window scaling, sprite
flip/rotate/mount), FTGL, FreeType, libogg/libvorbis, zziplib, SDL 1.2, SDL_image.

**Dropped:** `MMRegCodeGen.c`, `MMRegCodeMisc.c`, `MMChecksumStuff.c`, `MMCheckForValidReceipt.m`,
`validatereceipt.m`, `md5 edited.c` (registration/receipt/checksums), and the demo-limit /
`MakeMedusaHeadsForHackers` paths in game code. Episode is treated as unlocked.

## Data formats

`Game Data/Mansions/<Mansion> <Difficulty>/'TYPE'/<resID>` — classic Mac resources split into
files (big-endian). Types: BGND, GAME, ID, LEVL, PICT, ROOM, SPRT, TEXT, TMAP, TMAQ–TMAV, VERS.
`Compatibility Layer.c` provides a GetResource-style loader over these folders.
Sprites/Screens are PNG/JPG, audio is WAV/OGG, fonts are TTF + a bitmap font PNG.

## Port approach

1. Platform layer: SDL2 + OpenGL 2.1 compatibility (works on macOS arm64, Linux, Windows).
   Keep SpriteWorldX's BlitKernel split so a GLES / SDL_Renderer backend can be added later.
2. Replace Mac-only code (Carbon dialogs, Navigation Services, QuickTime, Cocoa menu bar) with
   SDL-based equivalents or in-game screens.
3. Rewrite game files one at a time from `reference/decomp`, naming structs/fields from usage.
4. CMake build; app bundle for macOS.

## Progress

- [x] Original bundle committed
- [x] Decompilation pipeline + reference output
- [x] Reconstructed headers (sprite/game structs, globals)
- [x] SpriteWorldX fork additions (SDL2 + OpenGL 2.1)
- [x] Game source files (all game code ported; registration/receipt/checksum code dropped)
- [x] Platform layer: resources, files, text (stb_truetype), sound/music (own mixer + stb_vorbis)
- [x] CMake build (Linux tested)
- [x] First boot: splash, title, dialogs, difficulty, organ intro, mansion map, gameplay,
      in-game map, death/respawn, room transitions tested under Xvfb
- [ ] macOS arm64 app bundle
- [ ] Play-through testing of every mansion
- [x] Additions (see EXTRAS.md): Fun/Extras options, new title menu, custom/random mansions,
      Trophy Room, controller menus and on-screen keyboard, relay co-op on one or two computers

## Tools

| Tool | Use |
|---|---|
| `tools/show.py <file> [regex]` | Print decompiled functions with sprite fields named by offset. |
| `tools/dump_data.py <addr> <n> <type>` | Dump data from the i386 binary. |
| `tools/floatargs.py <asm> <func>...` | Recover float call arguments Ghidra shows as 0 (needs `objdump` output of `__text`). |
| `tools/genprotos.py` | Regenerate `src/game/MMGameProtos.h` after adding functions. |
| `tools/auditglobals.py` | Compare global variable widths with how the original accesses them. |

## Known decompilation pitfalls

- Functions Ghidra typed with `short` parameters that really take `float`
  (`SWSetSpriteScaledSize/Width/Height`) show pool-loaded float constants as `0`.
  Check such calls with `tools/floatargs.py`.
- Globals: check the access width in the decomp (`*(short *)PTR__gX`), not just the name.
