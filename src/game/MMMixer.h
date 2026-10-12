/*
 * MMMixer — the small part of SDL_mixer the game uses, on plain SDL audio.
 *
 * Sound effects are decoded to memory (WAV via SDL, Ogg Vorbis via stb_vorbis);
 * music (Ogg Vorbis) is streamed. Volumes are 0..128 like SDL_mixer.
 */
#ifndef MMMIXER_H
#define MMMIXER_H

#include <SDL.h>

typedef struct MMixChunk MMixChunk;
typedef struct MMixMusic MMixMusic;

int        MMix_OpenAudio(int frequency, Uint16 format, int channels, int chunkSize);
void       MMix_CloseAudio(void);
int        MMix_AllocateChannels(int numChannels);
int        MMix_ReserveChannels(int num);

MMixChunk *MMix_LoadWAV(const char *path);     ///< .wav or .ogg
void       MMix_FreeChunk(MMixChunk *chunk);
int        MMix_PlayChannel(int channel, MMixChunk *chunk, int loops);   ///< -1: any free channel
int        MMix_Playing(int channel);
MMixChunk *MMix_GetChunk(int channel);
int        MMix_HaltChannel(int channel);       ///< -1: all
int        MMix_Volume(int channel, int volume); ///< volume -1 just queries
int        MMix_SetPanning(int channel, Uint8 left, Uint8 right);

MMixMusic *MMix_LoadMUS(const char *path);      ///< .ogg
void       MMix_FreeMusic(MMixMusic *music);
int        MMix_PlayMusic(MMixMusic *music, int loops);     ///< -1 forever, else play once
int        MMix_FadeInMusic(MMixMusic *music, int loops, int ms);
int        MMix_FadeOutMusic(int ms);
int        MMix_VolumeMusic(int volume);
int        MMix_PlayingMusic(void);
int        MMix_PausedMusic(void);
void       MMix_PauseMusic(void);
void       MMix_ResumeMusic(void);
void       MMix_RewindMusic(void);
int        MMix_HaltMusic(void);

#endif
