/*
 * After Midnight for the PS3: marks where the game's own variables end (see GameDataBegin.c).
 */
__attribute__((used, aligned(16), section(".data.mm_game_data_end"))) char __mm_game_data_end[16] = { 1 };
__attribute__((used, aligned(16), section(".bss.mm_game_bss_end"))) char __mm_game_bss_end[16];
