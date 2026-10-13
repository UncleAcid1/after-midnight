/*
 * After Midnight for the PS3: marks where the game's own variables begin. The Makefile links
 * this file just before the game's objects (src/game, src/swx) and GameDataEnd.c just after,
 * and the linker keeps each kind of section in that order, so split screen (MMSplit.c) can
 * swap each player's copy of the game's variables without touching the C library's, SDL's or
 * the RSX code's, which on the PS3 are linked into the same program.
 */
__attribute__((used, aligned(16), section(".data.mm_game_data_start"))) char __mm_game_data_start[16] = { 1 };
__attribute__((used, aligned(16), section(".bss.mm_game_bss_start"))) char __mm_game_bss_start[16];
