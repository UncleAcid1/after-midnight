/*
 * MMMixer.c — minimal SDL_mixer replacement (see MMMixer.h).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MMMixer.h"

#define STB_VORBIS_NO_PUSHDATA_API
#include "stb_vorbis.c"
#define DR_MP3_IMPLEMENTATION
#define DR_MP3_NO_STDIO_WARNINGS
#include "dr_mp3.h"

#define kMaxChannels    32
#define kMaxVolume      128

struct MMixChunk {
    Sint16 *samples;        ///< interleaved stereo at the device rate
    Uint32  numFrames;
};

typedef struct Channel {
    MMixChunk *chunk;
    Uint32     position;    ///< in frames
    int        loops;       ///< remaining extra plays, -1 forever
    int        volume;
    Uint8      panLeft, panRight;
} Channel;

struct MMixMusic {
    stb_vorbis      *vorbis;
    SDL_AudioStream *stream;    ///< converts the file's rate/channels to the device's
    int              fileChannels;
};

static SDL_AudioDeviceID gDevice = 0;
static SDL_AudioSpec     gSpec;
static int               gOpenCount = 0;
static Channel           gChannels[kMaxChannels];
static int               gNumChannels = 8;
static int               gNumReserved = 0;

static MMixMusic        *gMusic = NULL;     ///< playing (or paused) music
static int               gMusicLoops = 0;
static int               gMusicVolume = kMaxVolume;
static int               gMusicPaused = 0;
static int               gMusicFadeMs = 0;  ///< total fade length, 0 = no fade
static int               gMusicFadeElapsed = 0;
static int               gMusicFadingIn = 0;
static int               gMusicFinished = 1;

// ---------------------------------------------------------------------------
// Music decoding

/// Fills the stream with up to one block of decoded audio. Returns 0 at the end.
static int DecodeMusicBlock(MMixMusic *music)
{
    float buffer[4096];
    int frames = stb_vorbis_get_samples_float_interleaved(music->vorbis, music->fileChannels, buffer,
                                                           (int)(sizeof(buffer) / sizeof(float)));
    if (frames <= 0)
        return 0;
    SDL_AudioStreamPut(music->stream, buffer, frames * music->fileChannels * (int)sizeof(float));
    return 1;
}

static void MixMusic(Sint32 *mix, int frames)
{
    Sint16 buffer[4096];
    int needed = frames * 2 * (int)sizeof(Sint16);
    int got = 0, i, n;

    if (gMusic == NULL || gMusicPaused || gMusicFinished)
        return;

    while (got < needed) {
        int chunk = needed - got;
        if (chunk > (int)sizeof(buffer))
            chunk = (int)sizeof(buffer);
        while (SDL_AudioStreamAvailable(gMusic->stream) < chunk) {
            if (!DecodeMusicBlock(gMusic)) {
                if (gMusicLoops != 0) {
                    if (gMusicLoops > 0)
                        gMusicLoops--;
                    stb_vorbis_seek_start(gMusic->vorbis);
                    if (!DecodeMusicBlock(gMusic))
                        break;
                } else {
                    SDL_AudioStreamFlush(gMusic->stream);
                    break;
                }
            }
        }
        n = SDL_AudioStreamGet(gMusic->stream, buffer, chunk);
        if (n <= 0) {
            gMusicFinished = 1;
            break;
        }
        for (i = 0; i < n / (int)sizeof(Sint16); i++) {
            int frame = (got / (int)sizeof(Sint16) + i) / 2;
            int volume = gMusicVolume;
            if (gMusicFadeMs > 0) {
                int t = gMusicFadeElapsed + frame * 1000 / gSpec.freq;
                if (t > gMusicFadeMs)
                    t = gMusicFadeMs;
                volume = gMusicFadingIn ? volume * t / gMusicFadeMs : volume * (gMusicFadeMs - t) / gMusicFadeMs;
            }
            mix[got / (int)sizeof(Sint16) + i] += buffer[i] * volume / kMaxVolume;
        }
        got += n;
    }

    if (gMusicFadeMs > 0) {
        gMusicFadeElapsed += frames * 1000 / gSpec.freq;
        if (gMusicFadeElapsed >= gMusicFadeMs) {
            if (!gMusicFadingIn)
                gMusicFinished = 1;     // faded out: halt
            gMusicFadeMs = 0;
        }
    }
}

// ---------------------------------------------------------------------------
// Audio callback

static void AudioCallback(void *userdata, Uint8 *stream, int len)
{
    Sint16 *out = (Sint16 *)stream;
    int frames = len / 4, i, c;
    Sint32 *mix;

    (void)userdata;
    mix = (Sint32 *)SDL_calloc((size_t)frames * 2, sizeof(Sint32));
    if (mix == NULL) {
        SDL_memset(stream, 0, (size_t)len);
        return;
    }

    for (c = 0; c < gNumChannels; c++) {
        Channel *ch = &gChannels[c];
        int left = ch->volume * ch->panLeft / 255;
        int right = ch->volume * ch->panRight / 255;
        for (i = 0; i < frames && ch->chunk != NULL; i++) {
            if (ch->position >= ch->chunk->numFrames) {
                if (ch->loops != 0) {
                    if (ch->loops > 0)
                        ch->loops--;
                    ch->position = 0;
                } else {
                    ch->chunk = NULL;
                    break;
                }
            }
            mix[i * 2] += ch->chunk->samples[ch->position * 2] * left / kMaxVolume;
            mix[i * 2 + 1] += ch->chunk->samples[ch->position * 2 + 1] * right / kMaxVolume;
            ch->position++;
        }
    }

    MixMusic(mix, frames);

    for (i = 0; i < frames * 2; i++) {
        Sint32 s = mix[i];
        out[i] = (Sint16)(s > 32767 ? 32767 : (s < -32768 ? -32768 : s));
    }
    SDL_free(mix);
}

// ---------------------------------------------------------------------------
// Setup

int MMix_OpenAudio(int frequency, Uint16 format, int channels, int chunkSize)
{
    SDL_AudioSpec want;
    int i;

    if (gOpenCount > 0) {
        gOpenCount++;
        return 0;
    }
    if (!SDL_WasInit(SDL_INIT_AUDIO) && SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
        return -1;

    SDL_zero(want);
    want.freq = frequency;
    want.format = AUDIO_S16SYS;     // the mixer works in 16-bit stereo
    want.channels = 2;
    want.samples = (Uint16)chunkSize;
    want.callback = AudioCallback;
    (void)format; (void)channels;

    gDevice = SDL_OpenAudioDevice(NULL, 0, &want, &gSpec, 0);
    if (gDevice == 0)
        return -1;
    for (i = 0; i < kMaxChannels; i++) {
        SDL_zero(gChannels[i]);
        gChannels[i].volume = kMaxVolume;
        gChannels[i].panLeft = gChannels[i].panRight = 255;
    }
    gOpenCount = 1;
    SDL_PauseAudioDevice(gDevice, 0);
    return 0;
}

void MMix_CloseAudio(void)
{
    if (gOpenCount == 0 || --gOpenCount > 0)
        return;
    SDL_CloseAudioDevice(gDevice);
    gDevice = 0;
}

int MMix_AllocateChannels(int numChannels)
{
    if (numChannels < 0)
        return gNumChannels;
    if (numChannels > kMaxChannels)
        numChannels = kMaxChannels;
    SDL_LockAudioDevice(gDevice);
    gNumChannels = numChannels;
    SDL_UnlockAudioDevice(gDevice);
    return gNumChannels;
}

int MMix_ReserveChannels(int num)
{
    gNumReserved = num < gNumChannels ? num : gNumChannels;
    return gNumReserved;
}

// ---------------------------------------------------------------------------
// Chunks

/// Converts audio to the device format (16-bit stereo at the device rate).
static MMixChunk *MakeChunk(const void *data, int len, SDL_AudioFormat format, Uint8 channels, int rate)
{
    SDL_AudioStream *stream;
    MMixChunk *chunk;
    int outLen;

    stream = SDL_NewAudioStream(format, channels, rate, AUDIO_S16SYS, 2, gSpec.freq);
    if (stream == NULL)
        return NULL;
    SDL_AudioStreamPut(stream, data, len);
    SDL_AudioStreamFlush(stream);
    outLen = SDL_AudioStreamAvailable(stream);
    chunk = (MMixChunk *)calloc(1, sizeof(MMixChunk));
    if (chunk != NULL) {
        chunk->samples = (Sint16 *)malloc(outLen > 0 ? (size_t)outLen : 4);
        if (chunk->samples == NULL) {
            free(chunk);
            chunk = NULL;
        } else {
            SDL_AudioStreamGet(stream, chunk->samples, outLen);
            chunk->numFrames = (Uint32)(outLen / 4);
        }
    }
    SDL_FreeAudioStream(stream);
    return chunk;
}

MMixChunk *MMix_LoadWAV(const char *path)
{
    const char *ext = strrchr(path, '.');
    MMixChunk *chunk = NULL;

    if (gOpenCount == 0)
        return NULL;
    if (ext != NULL && SDL_strcasecmp(ext, ".ogg") == 0) {
        int channels, rate, frames;
        short *samples;
        frames = stb_vorbis_decode_filename(path, &channels, &rate, &samples);
        if (frames > 0) {
            chunk = MakeChunk(samples, frames * channels * (int)sizeof(short), AUDIO_S16SYS, (Uint8)channels, rate);
            free(samples);
        }
    } else if (ext != NULL && SDL_strcasecmp(ext, ".mp3") == 0) {
        drmp3_config config;
        drmp3_uint64 frames = 0;
        drmp3_int16 *samples = drmp3_open_file_and_read_pcm_frames_s16(path, &config, &frames, NULL);
        if (samples != NULL && frames > 0)
            chunk = MakeChunk(samples, (int)(frames * config.channels * sizeof(short)), AUDIO_S16SYS,
                              (Uint8)config.channels, (int)config.sampleRate);
        drmp3_free(samples, NULL);
    } else {
        SDL_AudioSpec spec;
        Uint8 *buffer;
        Uint32 length;
        if (SDL_LoadWAV(path, &spec, &buffer, &length) != NULL) {
            chunk = MakeChunk(buffer, (int)length, spec.format, spec.channels, spec.freq);
            SDL_FreeWAV(buffer);
        }
    }
    return chunk;
}

void MMix_FreeChunk(MMixChunk *chunk)
{
    int i;
    if (chunk == NULL)
        return;
    SDL_LockAudioDevice(gDevice);
    for (i = 0; i < kMaxChannels; i++) {
        if (gChannels[i].chunk == chunk)
            gChannels[i].chunk = NULL;
    }
    SDL_UnlockAudioDevice(gDevice);
    free(chunk->samples);
    free(chunk);
}

int MMix_PlayChannel(int channel, MMixChunk *chunk, int loops)
{
    if (gOpenCount == 0 || chunk == NULL) {
        SDL_SetError("No audio or no sound");
        return -1;
    }
    SDL_LockAudioDevice(gDevice);
    if (channel < 0) {
        int i;
        for (i = gNumReserved; i < gNumChannels; i++) {
            if (gChannels[i].chunk == NULL)
                break;
        }
        channel = i < gNumChannels ? i : -1;
    }
    if (channel >= 0 && channel < gNumChannels) {
        gChannels[channel].chunk = chunk;
        gChannels[channel].position = 0;
        gChannels[channel].loops = loops;
    } else {
        SDL_SetError("No free channels available");
        channel = -1;
    }
    SDL_UnlockAudioDevice(gDevice);
    return channel;
}

int MMix_Playing(int channel)
{
    int i, count = 0;
    if (gOpenCount == 0)
        return 0;
    if (channel >= 0)
        return channel < gNumChannels && gChannels[channel].chunk != NULL;
    for (i = 0; i < gNumChannels; i++)
        count += gChannels[i].chunk != NULL;
    return count;
}

MMixChunk *MMix_GetChunk(int channel)
{
    if (channel < 0 || channel >= gNumChannels)
        return NULL;
    return gChannels[channel].chunk;
}

int MMix_HaltChannel(int channel)
{
    int i;
    if (gOpenCount == 0)
        return 0;
    SDL_LockAudioDevice(gDevice);
    for (i = 0; i < gNumChannels; i++) {
        if (channel == -1 || channel == i)
            gChannels[i].chunk = NULL;
    }
    SDL_UnlockAudioDevice(gDevice);
    return 0;
}

int MMix_Volume(int channel, int volume)
{
    int i, previous;
    if (channel < 0 || channel >= gNumChannels) {
        if (channel != -1)
            return 0;
        previous = gChannels[0].volume;
        if (volume >= 0) {
            for (i = 0; i < gNumChannels; i++)
                gChannels[i].volume = volume > kMaxVolume ? kMaxVolume : volume;
        }
        return previous;
    }
    previous = gChannels[channel].volume;
    if (volume >= 0)
        gChannels[channel].volume = volume > kMaxVolume ? kMaxVolume : volume;
    return previous;
}

int MMix_SetPanning(int channel, Uint8 left, Uint8 right)
{
    if (channel < 0 || channel >= gNumChannels)
        return 0;
    gChannels[channel].panLeft = left;
    gChannels[channel].panRight = right;
    return 1;
}

// ---------------------------------------------------------------------------
// Music

MMixMusic *MMix_LoadMUS(const char *path)
{
    MMixMusic *music;
    stb_vorbis_info info;
    int error = 0;

    if (gOpenCount == 0)
        return NULL;
    music = (MMixMusic *)calloc(1, sizeof(MMixMusic));
    if (music == NULL)
        return NULL;
    music->vorbis = stb_vorbis_open_filename(path, &error, NULL);
    if (music->vorbis == NULL) {
        free(music);
        return NULL;
    }
    info = stb_vorbis_get_info(music->vorbis);
    music->fileChannels = info.channels;
    music->stream = SDL_NewAudioStream(AUDIO_F32SYS, (Uint8)info.channels, (int)info.sample_rate,
                                       AUDIO_S16SYS, 2, gSpec.freq);
    if (music->stream == NULL) {
        stb_vorbis_close(music->vorbis);
        free(music);
        return NULL;
    }
    return music;
}

void MMix_FreeMusic(MMixMusic *music)
{
    if (music == NULL)
        return;
    SDL_LockAudioDevice(gDevice);
    if (gMusic == music) {
        gMusic = NULL;
        gMusicFinished = 1;
    }
    SDL_UnlockAudioDevice(gDevice);
    SDL_FreeAudioStream(music->stream);
    stb_vorbis_close(music->vorbis);
    free(music);
}

static int StartMusic(MMixMusic *music, int loops, int fadeMs)
{
    if (gOpenCount == 0 || music == NULL)
        return -1;
    SDL_LockAudioDevice(gDevice);
    stb_vorbis_seek_start(music->vorbis);
    SDL_AudioStreamClear(music->stream);
    gMusic = music;
        // SDL_mixer: -1 loops forever, 0 or 1 plays once, n plays n times
    gMusicLoops = loops < 0 ? -1 : (loops > 1 ? loops - 1 : 0);
    gMusicPaused = 0;
    gMusicFinished = 0;
    gMusicFadeMs = fadeMs;
    gMusicFadeElapsed = 0;
    gMusicFadingIn = fadeMs > 0;
    SDL_UnlockAudioDevice(gDevice);
    return 0;
}

int MMix_PlayMusic(MMixMusic *music, int loops)
{
    return StartMusic(music, loops, 0);
}

int MMix_FadeInMusic(MMixMusic *music, int loops, int ms)
{
    return StartMusic(music, loops, ms);
}

int MMix_FadeOutMusic(int ms)
{
    SDL_LockAudioDevice(gDevice);
    if (gMusic != NULL && !gMusicFinished) {
        if (ms <= 0) {
            gMusicFinished = 1;
        } else {
            gMusicFadeMs = ms;
            gMusicFadeElapsed = 0;
            gMusicFadingIn = 0;
        }
    }
    SDL_UnlockAudioDevice(gDevice);
    return 1;
}

int MMix_VolumeMusic(int volume)
{
    int previous = gMusicVolume;
    if (volume >= 0)
        gMusicVolume = volume > kMaxVolume ? kMaxVolume : volume;
    return previous;
}

int MMix_PlayingMusic(void)
{
    return gMusic != NULL && !gMusicFinished;
}

int MMix_PausedMusic(void)
{
    return gMusic != NULL && gMusicPaused;
}

void MMix_PauseMusic(void)
{
    gMusicPaused = 1;
}

void MMix_ResumeMusic(void)
{
    gMusicPaused = 0;
}

void MMix_RewindMusic(void)
{
    SDL_LockAudioDevice(gDevice);
    if (gMusic != NULL) {
        stb_vorbis_seek_start(gMusic->vorbis);
        SDL_AudioStreamClear(gMusic->stream);
    }
    SDL_UnlockAudioDevice(gDevice);
}

int MMix_HaltMusic(void)
{
    SDL_LockAudioDevice(gDevice);
    gMusicFinished = 1;
    gMusicFadeMs = 0;
    SDL_UnlockAudioDevice(gDevice);
    return 0;
}

/// Split screen: every player's game plays through the same mixer.
void MMMixerShareState(void (*share)(void *, size_t))
{
    share(&gDevice, sizeof(gDevice));
    share(&gSpec, sizeof(gSpec));
    share(&gOpenCount, sizeof(gOpenCount));
    share(gChannels, sizeof(gChannels));
    share(&gNumChannels, sizeof(gNumChannels));
    share(&gNumReserved, sizeof(gNumReserved));
    share(&gMusic, sizeof(gMusic));
    share(&gMusicLoops, sizeof(gMusicLoops));
    share(&gMusicVolume, sizeof(gMusicVolume));
    share(&gMusicPaused, sizeof(gMusicPaused));
    share(&gMusicFadeMs, sizeof(gMusicFadeMs));
    share(&gMusicFadeElapsed, sizeof(gMusicFadeElapsed));
    share(&gMusicFadingIn, sizeof(gMusicFadingIn));
    share(&gMusicFinished, sizeof(gMusicFinished));
}
