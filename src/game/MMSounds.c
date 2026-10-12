/*
 * MMSounds.c — sound effects.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("MMSounds.c" and "MMSounds SDL_Mixer.c").
 * Sound n is "Game Data/Sounds/00<128+n>.wav" (or .ogg). Sounds are panned by the
 * position of the sprite that makes them relative to Jack.
 *
 * Channel modes for PlaySoundDetailed:
 *   1 any free channel (else the given one), 2 the given channel,
 *   3 the channel already playing this sound (restarts it).
 */
#include "MMGame.h"
#include "MMMixer.h"

#define kMaxMixChannels     16

static short gTempStereoFadeDist = 2500;
static Boolean gAudioWasInitialized = false;
static short gNumChannels = 0;
static short gNumSounds = 0;
static void LoadExtraSounds(void);
static short gNumReservedChannels = 0;
static MMixChunk **gSounds = NULL;

/// Split screen: each player's game asks for a loop's volume; the loudest wins (MMSoundsSplitFrameEnd).
static struct {
    Boolean used, stopIfSilent;
    short   volume, stereo;
} gLoopTargets[kMaxMixChannels];

// ---------------------------------------------------------------------------
// MMSounds.c

void MMSetTempStereoFadeDist(short distance)
{
    gTempStereoFadeDist = distance;
}

void MMPlaySound(short soundNum, short channel, short mode)
{
    MMPlayStereoSound2(NULL, soundNum, channel, mode, gSoundVolume, 1.0, 0);
}

void MMPlayStereoSound(SpritePtr spriteP, short soundNum, short channel, short mode)
{
    MMPlayStereoSound2(spriteP, soundNum, channel, mode, gSoundVolume, 1.0, 0);
}

/// Plays a sound panned by spriteP's position across a gTempStereoFadeDist-wide
/// area centered on Jack (spriteP NULL: centered).
int MMPlayStereoSound2(SpritePtr spriteP, short soundNum, short channel, short mode, short volume,
                       double rate, int loops)
{
    SpritePtr heroP;
    SWRect area;
    short stereoPosition = 0;

    if (!gSoundIsTurnedOn)
        return 0;
    if (spriteP != NULL) {
        heroP = GetHeroSprite();
        if (heroP != NULL) {
            SWSetRect(&area, 0, 0, gTempStereoFadeDist, 480);
            gTempStereoFadeDist = 2500;
            SWCenterRect(&area, &heroP->destFrameRect);
            stereoPosition = GetStereoPositionOfSprite(spriteP, &area);
        }
    }
    return PlaySoundDetailed(soundNum, channel, mode, volume, stereoPosition, rate, loops);
}

int MMPlaySoundDetailed(short soundNum, short channel, short mode, short volume, short stereoPosition,
                        double rate, int loops)
{
    if (!gSoundIsTurnedOn)
        return 0;
    return PlaySoundDetailed(soundNum, channel, mode, volume, stereoPosition, rate, loops);
}

short GetStereoPositionOfSprite(SpritePtr spriteP, SWRect *areaP)
{
    short center = spriteP->destFrameRect.left +
                   (short)((spriteP->destFrameRect.right - spriteP->destFrameRect.left) / 2);
    return GetStereoPositionOfColumn(center, areaP);
}

/// -256 (left edge of area) .. 256 (right edge).
short GetStereoPositionOfColumn(int column, SWRect *areaP)
{
    short width = areaP->right;
    short left = areaP->left;
    if (left != 0) {
        width -= left;
        column -= left;
    }
    if (column < 0)
        column = 0;
    else if (column > width)
        column = width;
    if (width / 2 == 0)
        return 0;
    return (short)(((column - width / 2) * 0x100) / (width / 2));
}

/// Volume falls off linearly with the distance from Jack, reaching 0 at maxDistance.
int MMPlayStereoSoundAdjustedVol(SpritePtr spriteP, short soundNum, short channel, short mode, short maxDistance,
                                 double rate)
{
    SpritePtr heroP;
    short volume = 0;
    int result = 0;

    if (!gSoundIsTurnedOn)
        return 0;
    heroP = GetHeroSprite();
    if (heroP == NULL)
        heroP = SWGetPreviousSprite(gDeadHeroSpriteLayerP, NULL);
    if (heroP != NULL && spriteP != NULL)
        volume = (short)((gSoundVolume * (maxDistance - GetDistBetweenSprites(heroP, spriteP))) / maxDistance);
    if (volume > 5)
        result = MMPlayStereoSound2(spriteP, soundNum, channel, mode, volume, rate, 0);
    return result;
}

/**
 * Keeps a looping sound for a whole layer of sprites (e.g. zapper beams) on
 * channel: its volume follows the nearest sprite and its panning their
 * distance-weighted average position.
 */
void MMUpdateChannelInfoForSpriteLayer(SpriteLayerPtr layerP, short soundNum, short channel, short maxVolume,
                                       short stereoWidth, short maxDistance)
{
    SpritePtr heroP, spriteP;
    SWRect area;
    float stereoSum = 0.0f, weightSum = 0.0f, weight, position;
    short volume = 0, distance, spriteVolume, currentVolume, newVolume;
    int remaining;

    if (!gSoundIsTurnedOn || !gAudioWasInitialized)
        return;

    if (!layerP->isPaused) {
        heroP = GetHeroSprite();
        if (heroP == NULL)
            heroP = SWGetPreviousSprite(gDeadHeroSpriteLayerP, NULL);
        if (heroP != NULL) {
            for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP)) {
                if (!spriteP->isVisible)
                    continue;
                distance = GetDistBetweenSprites(heroP, spriteP) - 15;
                if (distance < 0)
                    distance = 0;
                spriteVolume = (short)((maxVolume * (maxDistance - distance)) / maxDistance);
                SWSetRect(&area, 0, 0, stereoWidth, 480);
                SWCenterRect(&area, &heroP->destFrameRect);
                position = (float)GetStereoPositionOfSprite(spriteP, &area);
                if (volume < spriteVolume)
                    volume = spriteVolume;
                remaining = maxDistance - distance;
                if (remaining < 1)
                    remaining = 1;
                weight = (float)remaining / (float)maxDistance;
                weightSum += weight;
                stereoSum += position * weight;
            }
        }
        stereoSum = weightSum > 0.0f ? stereoSum / weightSum : 0.0f;
        if (SplitScreenActive() && channel >= 0 && channel < kMaxMixChannels) {
            if (!gLoopTargets[channel].used || volume > gLoopTargets[channel].volume) {
                gLoopTargets[channel].volume = volume;
                gLoopTargets[channel].stereo = (short)(int)stereoSum;
            }
            gLoopTargets[channel].used = true;
            gLoopTargets[channel].stopIfSilent = layerP == gZapperBeamSpriteLayerP;
            if (volume >= 1 && IsChannelAvailable(channel))
                MMPlayStereoSound2(NULL, soundNum, channel, 2, volume, 1.0, -1);
            return;
        }
        if (volume < 1) {
            if (!IsChannelAvailable(channel) && layerP == gZapperBeamSpriteLayerP)
                StopChannel(channel);
            volume = 0;
        } else if (IsChannelAvailable(channel)) {
            MMPlayStereoSound2(NULL, soundNum, channel, 2, volume, 1.0, -1);
        }
    } else {
        volume = 0;
    }

    volume = (short)(int)((float)volume * 0.7f);
    SetVolumeAndStereoPosition(channel, volume, (short)(int)stereoSum);

        // ease the channel's volume toward the target
    if (!IsChannelAvailable(channel)) {
        currentVolume = GetChannelVolume(channel);
        if (volume < currentVolume)
            newVolume = (short)SW_MAX(volume, currentVolume - 5);
        else
            newVolume = (short)SW_MIN(volume, currentVolume + 5);
        SetVolumeAndStereoPosition(channel, newVolume, (short)(int)stereoSum);
    }
}

// ---------------------------------------------------------------------------
// MMSounds SDL_Mixer.c

int LoadAndSetupSounds(short numChannels, short numReservedChannels, short numSounds, const char *folderPath)
{
    char path[300];
    short i;

    gNumChannels = numChannels;
    gNumReservedChannels = numReservedChannels;
    gNumSounds = numSounds;
    gSounds = (MMixChunk **)calloc(numSounds + kNumExtraSounds, sizeof(MMixChunk *));
    if (gSounds == NULL)
        return -1;

    if (MMix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) == 0) {
        gAudioWasInitialized = true;
    } else {
        printf("Unable to initialize audio: %s\n", SDL_GetError());
        gAudioWasInitialized = false;
    }

    puts("Allocating sound channels...");
    if (gAudioWasInitialized) {
        MMix_AllocateChannels(gNumChannels);
        MMix_ReserveChannels(gNumReservedChannels);
    }

    puts("Loading sounds...");
    if (gAudioWasInitialized) {
        for (i = 0; i < numSounds; i++) {
            int resID = i + 128;
            snprintf(path, sizeof(path), "%s00%d.wav", folderPath, resID);
            gSounds[i] = MMix_LoadWAV(path);
            if (gSounds[i] == NULL) {
                snprintf(path, sizeof(path), "%s00%d.ogg", folderPath, resID);
                gSounds[i] = MMix_LoadWAV(path);
            }
            if (gSounds[i] == NULL) {
                printf("  Couldn't load sound %d from %s\n", resID, folderPath);
                return -2;
            }
        }
        LoadExtraSounds();
    }
    return 0;
}

// ---------------------------------------------------------------------------
// Sounds this version adds (optional: a missing file just stays silent)

static void LoadExtraSounds(void)
{
    static const char *files[kNumExtraSounds] = { "Sounds/my_tuba", "Sounds/dice_roll", "Sounds/bell" };
    static const char *types[] = { ".ogg", ".wav", ".mp3" };
    char name[200];
    const char *path;
    int i, t;

    for (i = 0; i < kNumExtraSounds; i++) {
        gSounds[gNumSounds + i] = NULL;
        for (t = 0; t < 3 && gSounds[gNumSounds + i] == NULL; t++) {
            snprintf(name, sizeof(name), "%s%s", files[i], types[t]);
            path = GetExtrasFilePath(name);
            if (path != NULL)
                gSounds[gNumSounds + i] = MMix_LoadWAV(path);
            if (gSounds[gNumSounds + i] != NULL)
                printf("  Loaded %s\n", name);
        }
    }
}

short GetExtraSoundIndex(int which)
{
    if (gSounds == NULL || which < 0 || which >= kNumExtraSounds || gSounds[gNumSounds + which] == NULL)
        return -1;
    return (short)(gNumSounds + which);
}

void MMPlayExtraSound(int which)
{
    short index = GetExtraSoundIndex(which);
    int channel;

    if (index < 0 || !gSoundIsTurnedOn || !gAudioWasInitialized)
        return;
    channel = MMix_PlayChannel(-1, gSounds[index], 0);
    if (channel != -1)
        SetVolumeAndStereoPosition((short)channel, gSoundVolume / 2, 0);
}

/// volume 0..255, stereoPosition -255 (left) .. 255 (right); loops -1 repeats forever.
int PlaySoundDetailed(short soundNum, short channel, short mode, short volume, short stereoPosition,
                      double rate, int loops)
{
    short playChannel;
    int result;

    (void)rate;
    if (!gSoundIsTurnedOn || !gAudioWasInitialized || soundNum < 0 || soundNum >= gNumSounds)
        return 0;

    playChannel = channel;
    if (mode == 1) {
        playChannel = FindEmptyChannel();
        if (playChannel == -1)
            playChannel = channel;
    } else if (mode == 3) {
        playChannel = FindSound(soundNum);
    }

    result = MMix_PlayChannel(playChannel, gSounds[soundNum], loops == -1 ? -1 : 0);
    if (result == -1)
        printf("Unable to play WAV file: %s\n", SDL_GetError());
    else
        SetVolumeAndStereoPosition((short)result, volume / 2, stereoPosition);
    return 0;
}

short CountSounds(short soundNum)
{
    short channel, count = 0;
    if (!gAudioWasInitialized)
        return 0;
    for (channel = 0; channel < kMaxMixChannels; channel++) {
        if (MMix_Playing(channel) && MMix_GetChunk(channel) == gSounds[soundNum])
            count++;
    }
    return count;
}

/// The channel playing this sound, or -1.
short FindSound(short soundNum)
{
    short channel;
    if (!gAudioWasInitialized)
        return -1;
    for (channel = 0; channel < kMaxMixChannels; channel++) {
        if (MMix_Playing(channel) && MMix_GetChunk(channel) == gSounds[soundNum])
            return channel;
    }
    return -1;
}

void StopMySound(short soundNum)
{
    short channel;
    if (gAudioWasInitialized && soundNum >= 0 && soundNum < gNumSounds) {
        channel = FindSound(soundNum);
        if (channel != -1)
            MMix_HaltChannel(channel);
    }
}

short GetChannelVolume(short channel)
{
    return (short)MMix_Volume(channel, -1);
}

void StopChannel(short channel)
{
    if (gAudioWasInitialized)
        MMix_HaltChannel(channel);
}

void StopAllChannels(void)
{
    if (gAudioWasInitialized)
        MMix_HaltChannel(-1);
}

Boolean IsChannelAvailable(short channel)
{
    if (!gAudioWasInitialized)
        return false;
    return !MMix_Playing(channel);
}

/// First idle channel after the reserved ones, or -1.
short FindEmptyChannel(void)
{
    short channel;
    for (channel = gNumReservedChannels; channel < kMaxMixChannels; channel++) {
        if (!MMix_Playing(channel))
            return channel;
    }
    return -1;
}

void SetVolumeAndStereoPosition(short channel, short volume, short stereoPosition)
{
    short left, right, offset;

    if (volume < 0)
        volume = 0;
    if (stereoPosition < -255)
        stereoPosition = -255;
    else if (stereoPosition > 255)
        stereoPosition = 255;
    offset = (short)((volume * stereoPosition) / 255);
    left = volume - offset;
    right = volume + offset;
    if (right > 255)
        right = 255;
    if (left > 255)
        left = 255;
    MMix_Volume(channel, volume);
        // SDL_mixer's panning takes the low byte, as the original did
    MMix_SetPanning(channel, (Uint8)left, (Uint8)right);
}

/// Split screen, once a frame: the looping sounds play as loud as the nearest player hears them.
void MMSoundsSplitFrameEnd(void)
{
    short channel, volume, current;

    for (channel = 0; channel < kMaxMixChannels; channel++) {
        if (!gLoopTargets[channel].used)
            continue;
        gLoopTargets[channel].used = false;
        volume = (short)(int)((float)gLoopTargets[channel].volume * 0.7f);
        if (volume < 1 && gLoopTargets[channel].stopIfSilent && !IsChannelAvailable(channel)) {
            StopChannel(channel);
            continue;
        }
        if (IsChannelAvailable(channel))
            continue;
        current = GetChannelVolume(channel);
        volume = volume < current ? (short)SW_MAX(volume, current - 5) : (short)SW_MIN(volume, current + 5);
        SetVolumeAndStereoPosition(channel, volume, gLoopTargets[channel].stereo);
    }
}

/// Split screen: the sounds are loaded once for every player's game.
void MMSoundsShareState(void (*share)(void *, size_t))
{
    share(gLoopTargets, sizeof(gLoopTargets));
    share(&gTempStereoFadeDist, sizeof(gTempStereoFadeDist));
    share(&gAudioWasInitialized, sizeof(gAudioWasInitialized));
    share(&gNumChannels, sizeof(gNumChannels));
    share(&gNumSounds, sizeof(gNumSounds));
    share(&gNumReservedChannels, sizeof(gNumReservedChannels));
    share(&gSounds, sizeof(gSounds));
}
