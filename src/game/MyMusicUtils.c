/*
 * MyMusicUtils.c — background music.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources - My Utils/MyMusicUtils.c").
 * Music files are "Game Data/Music/<name>[ <n>].ogg" (an .mp3 was tried first).
 */
#include "MMGame.h"
#include "MMMixer.h"

static MMixMusic *gMusicP = NULL;
static Boolean gCurrentMusicIsLooping = true;
static short gCurMusicVolume;
static Boolean gMusicWasInitialized;
static char gLoadedMusic[300];      ///< what MyPlayMusic last started (split screen: don't restart it)

int InitMusic(void)
{
    if (MMix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) == 0) {
        gMusicWasInitialized = true;
    } else {
        printf("Unable to initialize audio: %s\n", SDL_GetError());
        gMusicWasInitialized = false;
    }
    if (gMusicWasInitialized)
        MMix_AllocateChannels(2);
    return 0;
}

void ExitMusic(void)
{
    StopAndDisposeMusic();
}

int LoadMusic(const char *path)
{
    if (!gMusicWasInitialized)
        return 0;
    gMusicP = MMix_LoadMUS(path);
    return gMusicP == NULL ? -1 : 0;
}

void SetMusicLooping(Boolean looping)
{
    gCurrentMusicIsLooping = looping;
}

void StartMusic(short volume)
{
    if (!gMusicWasInitialized)
        return;
    SetMusicVolume(volume);
    MMix_PlayMusic(gMusicP, gCurrentMusicIsLooping ? -1 : 1);
}

/// volume is 0..1023 (0..~255 with "softer music").
void SetMusicVolume(short volume)
{
    if (!gMusicWasInitialized)
        return;
    if (!gSofterMusic)
        MMix_VolumeMusic(volume / 8 + 1);
    else
        MMix_VolumeMusic(volume / 18 + 1);
    gCurMusicVolume = volume;
}

short GetMusicVolume(void)
{
    return gCurMusicVolume;
}

void *GetCurrentMusicReference(void)
{
    return gMusicP;
}

Boolean IsMusicDone(void)
{
    if (!gMusicWasInitialized)
        return true;
    return !MMix_PlayingMusic();
}

void StopAndDisposeMusic(void)
{
    if (!gMusicWasInitialized)
        return;
    MMix_HaltMusic();
    MMix_FreeMusic(gMusicP);
    gMusicP = NULL;
}

void PauseMusic(void)
{
    if (gMusicWasInitialized)
        MMix_PauseMusic();
}

void ResumeMusic(Boolean rewind)
{
    if (!gMusicWasInitialized)
        return;
    if (!MMix_PlayingMusic()) {
        MMix_PlayMusic(gMusicP, gCurrentMusicIsLooping ? -1 : 1);
    } else {
        if (rewind)
            MMix_RewindMusic();
        MMix_ResumeMusic();
    }
}

Boolean IsMusicPlaying(void)
{
    return !MMix_PausedMusic() && MMix_PlayingMusic();
}

/// Fades out over steps * 200 ms.
void FadeMusicNow(short steps)
{
    MMix_FadeOutMusic(steps * 200);
}

void FadeMusicNowNoAdjust(short ms)
{
    MMix_FadeOutMusic(ms);
}

void FadeMusicIn(short steps)
{
    MMix_FadeInMusic(gMusicP, gCurrentMusicIsLooping ? -1 : 1, steps * 200);
}

void StartMusicByFadingIn(const char *name, int fadeMs, short volume)
{
    MyPlayMusic(name, volume, 0, true);
    if (gMusicIsOn) {
        MMix_FadeOutMusic(0);
        while (MMix_PlayingMusic())
            ;
        MMix_FadeInMusic(gMusicP, -1, fadeMs);
    }
}

/// Title screen music; without the opening music, just the crickets sound.
void StartOpeningMusic(Boolean forceMusic)
{
    if (GetCurrentMusicReference() == gOpeningMusicRefNum && !IsMusicDone())
        return;
    StopAndDisposeMusic();
    if (!gDoOpeningMusic && !forceMusic)
        MMPlayStereoSound2(NULL, 0x4c, 6, 2, 0xff, 1.0, -1);
    else
        MyPlayMusic("Opening Music", gTitleMusicVolume, 0, true);
    gOpeningMusicRefNum = GetCurrentMusicReference();
}

void PlayBackgroundMusic(short index)
{
    if (index > 0)
        MyPlayMusic("Background Music", gBackgroundMusicVolume, index, true);
}

/// Loads and (if music is on) starts "<name>.ogg", or "<name> <index>.ogg" for index > 0.
void MyPlayMusic(const char *name, short volume, short index, Boolean looping)
{
    char mp3Path[256], oggPath[256], message[500];
    char folder[1024];
    int err;

    if (gUsingEditor)
        return;

    snprintf(folder, sizeof(folder), "%s %d", name, index);
    if (SplitScreenActive() && strcmp(folder, gLoadedMusic) == 0 && IsMusicPlaying()) {
        gCurrentMusicIndex = index < 1 ? 0 : index;      // another player's already playing it
        return;
    }
    snprintf(gLoadedMusic, sizeof(gLoadedMusic), "%s", folder);
    StopAndDisposeMusic();
    snprintf(folder, sizeof(folder), "%s", GetMusicFolderPath());
    if (index < 1) {
        gCurrentMusicIndex = 0;
        snprintf(mp3Path, sizeof(mp3Path), "%s%s.mp3", folder, name);
        snprintf(oggPath, sizeof(oggPath), "%s%s.ogg", folder, name);
    } else {
        gCurrentMusicIndex = index;
        snprintf(mp3Path, sizeof(mp3Path), "%s%s %d.mp3", folder, name, index);
        snprintf(oggPath, sizeof(oggPath), "%s%s %d.ogg", folder, name, index);
    }

        // MMMixer reads Ogg Vorbis only, so the .mp3 attempt is skipped.
    (void)mp3Path;
    err = LoadMusic(oggPath);

    if (gMusicIsOn && err != 0) {
        if (index < 1)
            snprintf(message, sizeof(message),
                     "The music couldn't be loaded. Please make sure the file \r%s.ogg\r is inside the Game Data/Music folder.",
                     name);
        else
            snprintf(message, sizeof(message),
                     "The music couldn't be loaded. Please make sure the file \r%s %d.ogg\r is inside the Game Data/Music folder.",
                     name, index);
        ErrorMessage(message, false);
        gMusicIsOn = false;
    }

    SetMusicLooping(looping);
    if (!gMusicIsOn)
        SetMusicVolume(volume);
    else
        StartMusic(volume);
}

void MyFadeMusicNow(void)
{
    if (gMusicIsOn && IsMusicPlaying()) {
        FadeMusicNow(3);
        while (MMix_PlayingMusic())
            SDL_Delay(100);
        StopAndDisposeMusic();
    }
}

/// Split screen: one piece of music for everyone (whoever walks into a new part of the
/// mansion changes it; each game remembers its own part in gCurrentMusicIndex).
void MyMusicShareState(void (*share)(void *, size_t))
{
    share(&gMusicP, sizeof(gMusicP));
    share(&gCurrentMusicIsLooping, sizeof(gCurrentMusicIsLooping));
    share(&gCurMusicVolume, sizeof(gCurMusicVolume));
    share(&gMusicWasInitialized, sizeof(gMusicWasInitialized));
    share(&gOpeningMusicRefNum, sizeof(gOpeningMusicRefNum));
    share(gLoadedMusic, sizeof(gLoadedMusic));
}
