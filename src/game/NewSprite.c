/*
 * NewSprite.c — loading the game's sprite images (gSpriteArray and the screens'
 * arrays) and creating every kind of game sprite from them.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources & Headers/NewSprite.c").
 */
#include <dirent.h>
#include "MMGame.h"

SDL_Color gSDLWhiteColor = { 255, 255, 255, 0 };
SDL_Color gSDLTealColor = { 0, 255, 255, 0 };
SDL_Color gSDLPurpleColor = { 255, 0, 255, 0 };
SDL_Color gSDLBlackColor = { 0, 0, 0, 0 };

Boolean gTitleScreenSpritesAreValid = false;
static Boolean gDifficultyScreenSpritesAreValid = false;
static Boolean gOrganScreenSpritesAreValid = false;
static Boolean gMapScreenSpritesAreValid = false;
static Boolean gStatsAreaSpritesAreValid = false;
static Boolean gSpriteArrayWasInited = false;
static Boolean spriteFlagsWereInitialized = false;

    // the bitmap font's character widths, as a fraction of a cell
static const float fontSpacingArray[112] = {
    0.8f, 0.8f, 0.8f, 0.82f, 0.79f, 0.75f, 0.8f, 0.8f, 0.28f, 0.66f,
    0.81f, 0.72f, 0.91f, 0.86f, 0.8f, 0.8f, 0.8f, 0.8f, 0.75f, 0.8f,
    0.78f, 0.82f, 0.96f, 0.8f, 0.77f, 0.8f, 0.95f, 0.93f, 0.92f, 0.93f,
    0.92f, 0.88f, 0.94f, 0.92f, 0.34f, 0.76f, 0.92f, 0.92f, 0.98f, 0.95f,
    0.95f, 0.92f, 0.95f, 0.92f, 0.89f, 0.9f, 0.92f, 0.94f, 0.98f, 0.92f,
    0.94f, 0.92f, 0.88f, 0.44f, 0.88f, 0.88f, 0.88f, 0.88f, 0.88f, 0.88f,
    0.88f, 0.88f, 0.3f, 0.3f, 0.3f, 0.38f, 0.92f, 0.92f, 0.92f, 0.6f,
    0.45f, 0.45f, 0.4f, 0.8f, 0.95f, 0.6f, 0.6f, 0.2f, 0.3f, 0.3f,
    0.2f, 0.9f, 0.4f, 0.4f, 0.95f, 0.95f, 0.95f, 0.95f, 0.95f, 0.95f,
    0.95f, 0.95f, 0.95f, 0.95f, 0.95f, 0.95f, 0.95f, 0.95f, 0.95f, 0.95f,
    0.95f, 0.95f, 0.95f, 0.95f, 0.95f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f
};

    // custom mansions can replace sprite images: their 'PICT' folder is scanned once
#define kMaxValidPICTResIDs 5000
static short gLastValidPICTResIDIndex = 0;
static short gValidPICTResIDs[kMaxValidPICTResIDs];
static char scannedPath[512];
static short spriteFlags[kNumSpriteArrayEntries];

/// Allocates a game sprite record of the given size as a copy of masterP.
static SpritePtr CloneGameSprite(SpritePtr masterP, size_t recordSize)
{
    SpritePtr newSpriteP = NULL;
    void *storageP;
    SWError err;

    storageP = calloc(1, recordSize);
    if (storageP == NULL)
        MM_FATAL(kMemoryAllocationError);
    err = SWCloneSprite(masterP, &newSpriteP, storageP, (long)recordSize);
    if (err != kNoError)
        MM_FATAL(err);
    return newSpriteP;
}

/// Sets every frame's collision inset.
static void SetCollisionInsetForAllFrames(SpritePtr spriteP, SWRect *insetP)
{
    long frame;

    for (frame = 0; frame < spriteP->maxFrames; frame++)
        SWSetFrameCollisionInset(spriteP->frameArray[frame], insetP->left, insetP->top, insetP->right, insetP->bottom);
}

// ---------------------------------------------------------------------------
// Loading

void LoadSprites(void)
{
    long saveGameMode;
    short i;

    InitSpriteArray();
    if (!gMainSpritesAreLoaded)
        LoadStatsSprites();
    saveGameMode = gCurrentGameMode;
    gCurrentGameMode = kNoGameMode;
    puts("\nLoading non-pixel-mask sprites... ");
    LoadNonPixelMaskSprites();
    puts("\nLoading pixel mask sprites...");
    LoadPixelMaskPICTSprites();
    gCurrentGameMode = saveGameMode;
    WriteCStringToFile("Loading Completed. About to lock sprites...");
    WriteCarriageReturn(2);
    for (i = 0; i < kNumSpriteArrayEntries; i++)
        SWLockSprite(gSpriteArray[i]);
    gMainSpritesAreLoaded = true;
    WriteCStringToFile("LoadSprites completed successfully.\r");
}

void InitSpriteArray(void)
{
    short i;

    if (gSpriteArrayWasInited)
        return;
    for (i = 0; i < kNumSpriteArrayEntries; i++)
        gSpriteArray[i] = NULL;
    for (i = 0; i < 14; i++)
        gTitleScreenSpriteArray[i] = NULL;
    gSpriteArrayWasInited = true;
}

/// gSpriteArray entries, by index: 0 Jack, 1-13 enemies, 14-25 levers and switches,
/// 26-29 zapper nodes, 30-44 objects, 45-68 doors, 69-96 platforms and trap doors,
/// 97-111 pedestals/statues, 112-157 backgrounds, 158-182 torches, candles and
/// treasure, 183-226 effects, buttons and the like.
void LoadPixelMaskPICTSprites(void)
{
    const char *path;
    short i, pictID;
    SWError err;

    WriteCStringToFile("LoadPixelMaskPICTSprites entered...\r");
    err = MyLoadSpriteFromSinglePictXY(gSpriteWorldP, 0, 0, 128, 128, 128, 128, 0, 0, 0, 84, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[0], 64, 64);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePictXY(gSpriteWorldP, 194, 0, 130, 130, 128, 128, 0, 0, 0, 2, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[194], 0.5f, 0.5f);
    KeepMusicPlaying();
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 195, 0, 131, 131, 112, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[195], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 196, 0, 135, 135, 80, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[196], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 1, 0, 150, 150, 64, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[1], 64, 32);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 2, 0, 155, 155, 64, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[2], 32, 32);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 3, 0, 160, 160, 64, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[3], 32, 32);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 4, 0, 165, 165, 84, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[4], 42, 42);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 5, 0, 166, 166, 84, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[5], 42, 42);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 6, 0, 170, 170, 64, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[6], 32, 32);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 7, 0, 171, 171, 64, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[7], 32, 32);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 8, 0, 174, 174, 84, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[8], 52, 52);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 9, 0, 175, 175, 84, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[9], 52, 52);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 10, 0, 180, 180, 64, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[10], 32, 32);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 11, 0, 181, 181, 128, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[11], 64, 64);
    IncreaseProgressBar(1);
    KeepMusicPlaying();
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 12, 0, 185, 185, 56, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[12], 40, 40);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePictXY(gSpriteWorldP, 13, 0, 188, 188, 96, 96, 0, 0, 0, 14, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[13], 64, 64);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 187, 0, 910, 910, 96, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[187], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 26, 0, 190, 190, 32, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[26], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 27, 0, 191, 191, 32, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[27], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 28, 0, 192, 192, 32, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[28], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 29, 0, 193, 193, 32, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[29], 0.5f, 0.5f);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePictXY(gSpriteWorldP, 185, 0, 195, 195, 32, 32, 0, 0, 0, 64, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[185], 16, 16);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePictXY(gSpriteWorldP, 186, 0, 196, 196, 32, 32, 0, 0, 0, 64, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[186], 16, 16);
    IncreaseProgressBar(1);
    KeepMusicPlaying();
    err = MyLoadSpriteFromPictResource(gSpriteWorldP, 30, 0, 800, 800, 1, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[30], 32, 32);
    err = MyLoadSpriteFromPictResource(gSpriteWorldP, 31, 0, 801, 801, 1, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[31], 32, 32);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 32, 0, 802, 802, 32, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[32], 16, 16);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 33, 0, 803, 803, 32, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[33], 16, 16);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 34, 0, 810, 810, 64, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[34], 32, 32);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 35, 0, 815, 815, 70, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[35], 40, 48);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 36, 0, 820, 820, 64, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[36], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 37, 0, 825, 825, 96, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[37], 0.5f, 0.5f);
    err = MyLoadSpriteFromPictResource(gSpriteWorldP, 38, 0, 830, 830, 1, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[38], 0.5f, 0.5f);
    IncreaseProgressBar(1);
    pictID = 1200;
    for (i = 39; i < 45; i++, pictID++) {
        err = MyLoadSpriteFromSinglePict(gSpriteWorldP, i, 0, pictID, pictID, 32, 0, 4);
        if (err != kNoError)
            MM_FATAL(err);
        MySetSpriteScaledSize(gSpriteArray[i], 16, 32);
        IncreaseProgressBar(1);
    }
    pictID = 700;
    for (i = 69; i < 80; i++, pictID++) {
        err = MyLoadSpriteFromPictResource(gSpriteWorldP, i, 0, pictID, pictID, 1, 4);
        if (err != kNoError)
            MM_FATAL(err);
        MySetSpriteScaledFactor(gSpriteArray[i], 0.5f, 0.5f);
    }
    IncreaseProgressBar(1);
    pictID = 746;
    for (i = 80; i < 92; i++, pictID++) {
        err = MyLoadSpriteFromPictResource(gSpriteWorldP, i, 0, pictID, pictID, 1, 4);
        if (err != kNoError)
            MM_FATAL(err);
        MySetSpriteScaledFactor(gSpriteArray[i], 0.5f, 0.5f);
    }
    IncreaseProgressBar(1);
    pictID = 760;
    for (i = 92; i < 95; i++, pictID++) {
        err = MyLoadSpriteFromSinglePict(gSpriteWorldP, i, 0, pictID, pictID, 48, 0, 4);
        if (err != kNoError)
            MM_FATAL(err);
        MySetSpriteScaledFactor(gSpriteArray[i], 0.5f, 0.5f);
    }
    IncreaseProgressBar(1);
    KeepMusicPlaying();
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 95, 0, 770, 770, 135, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[95], 90, 85);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 96, 0, 771, 771, 135, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[96], 90, 85);
    IncreaseProgressBar(1);
    pictID = 840;
    for (i = 97; i < 101; i++, pictID++) {
        err = MyLoadSpriteFromPictResource(gSpriteWorldP, i, 0, pictID, pictID, 1, 4);
        if (err != kNoError)
            MM_FATAL(err);
        MySetSpriteScaledFactor(gSpriteArray[i], 0.5f, 0.5f);
    }
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 101, 0, 844, 844, 128, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[101], 0.5f, 0.5f);
    pictID = 850;
    for (i = 102; i < 112; i++, pictID++) {
        err = MyLoadSpriteFromPictResource(gSpriteWorldP, i, 0, pictID, pictID, 1, 4);
        if (err != kNoError)
            MM_FATAL(err);
        MySetSpriteScaledFactor(gSpriteArray[i], 0.5f, 0.5f);
    }
    pictID = 880;
    for (i = 112; i < 116; i++, pictID++) {
        err = MyLoadSpriteFromSinglePict(gSpriteWorldP, i, 0, pictID, pictID, 128, 0, 4);
        if (err != kNoError)
            MM_FATAL(err);
        MySetSpriteScaledFactor(gSpriteArray[i], 0.5f, 0.5f);
    }
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 116, 0, 890, 890, 192, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[116], 0.5f, 0.5f);
    pictID = 1100;
    for (i = 133; i < 158; i++, pictID++) {
        err = MyLoadSpriteFromPictResource(gSpriteWorldP, i, 0, pictID, pictID, 1, 4);
        if (err != kNoError)
            MM_FATAL(err);
        MySetSpriteScaledFactor(gSpriteArray[i], 0.5f, 0.5f);
    }
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 158, 0, 940, 940, 48, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[158], 0.5f, 0.5f);
    KeepMusicPlaying();
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 159, 0, 945, 945, 64, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[159], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 160, 0, 946, 946, 64, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[160], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 193, 0, 950, 950, 64, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[193], 0.5f, 0.5f);
    IncreaseProgressBar(1);
    pictID = 960;
    for (i = 161; i < 164; i++, pictID++) {
        err = MyLoadSpriteFromSinglePict(gSpriteWorldP, i, 0, pictID, pictID, 32, 0, 4);
        if (err != kNoError)
            MM_FATAL(err);
        MySetSpriteScaledFactor(gSpriteArray[i], 0.5f, 0.5f);
    }
    pictID = 970;
    for (i = 164; i < 167; i++, pictID++) {
        err = MyLoadSpriteFromSinglePict(gSpriteWorldP, i, 0, pictID, pictID, 16, 0, 4);
        if (err != kNoError)
            MM_FATAL(err);
        MySetSpriteScaledFactor(gSpriteArray[i], 0.5f, 0.5f);
    }
    pictID = 979;
    for (i = 167; i < 177; i++, pictID++) {
        err = MyLoadSpriteFromPictResource(gSpriteWorldP, i, 0, pictID, pictID, 1, 4);
        if (err != kNoError)
            MM_FATAL(err);
        MySetSpriteScaledFactor(gSpriteArray[i], 0.5f, 0.5f);
    }
    pictID = 1000;
    for (i = 14; i < 18; i++, pictID++) {
        err = MyLoadSpriteFromSinglePict(gSpriteWorldP, i, 0, pictID, pictID, 96, 0, 4);
        if (err != kNoError)
            MM_FATAL(err);
        MySetSpriteScaledSize(gSpriteArray[i], 64, 64);
    }
    SWSetTransparentColor(&gSDLTealColor);
    pictID = 1010;
    for (i = 18; i < 26; i++, pictID++) {
        err = MyLoadSpriteFromSinglePict(gSpriteWorldP, i, 0, pictID, pictID, 80, 0, 4);
        if (err != kNoError)
            MM_FATAL(err);
        MySetSpriteScaledFactor(gSpriteArray[i], 0.5f, 0.5f);
    }
    pictID = 1300;
    for (i = 129; i < 133; i++, pictID++) {
        err = MyLoadSpriteFromPictResource(gSpriteWorldP, i, 0, pictID, pictID, 1, 4);
        if (err != kNoError)
            MM_FATAL(err);
        MySetSpriteScaledSize(gSpriteArray[i], 64, 128);
    }
    SWSetTransparentColor(&gSDLWhiteColor);
    IncreaseProgressBar(1);
    KeepMusicPlaying();
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 188, 0, 900, 900, 40, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[188], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 189, 0, 901, 901, 80, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[189], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 190, 0, 905, 905, 40, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[190], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 191, 0, 906, 906, 80, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[191], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 192, 0, 920, 920, 80, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[192], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePictXY(gSpriteWorldP, 197, 0, 1500, 1500, 128, 32, 0, 0, 0, 3, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[197], 64, 16);
    err = MyLoadSpriteFromSinglePictXY(gSpriteWorldP, 198, 0, 1501, 1501, 128, 32, 0, 0, 0, 3, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[198], 64, 16);
    pictID = 2000;
    for (i = 177; i < 183; i++, pictID++) {
        err = MyLoadSpriteFromPictResource(gSpriteWorldP, i, 0, pictID, pictID, 1, 4);
        if (err != kNoError)
            MM_FATAL(err);
        if (i == 180) {
            MySetSpriteScaledFactor(gSpriteArray[180], 0.668f, 0.668f);
        }
        else {
            MySetSpriteScaledFactor(gSpriteArray[i], 0.5f, 0.5f);
        }
    }
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 199, 0, 2010, 2010, 16, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[199], 0.5f, 0.5f);
    KeepMusicPlaying();
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 206, 0, 930, 930, 72, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[206], 48, 24);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 207, 0, 931, 931, 72, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[207], 48, 24);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 208, 0, 935, 935, 48, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[208], 32, 16);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 209, 0, 936, 936, 48, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[209], 32, 16);
    pictID = 861;
    for (i = 118; i < 127; i++, pictID++) {
        err = MyLoadSpriteFromPictResource(gSpriteWorldP, i, 0, pictID, pictID, 1, 0);
        if (err != kNoError)
            MM_FATAL(err);
        MySetSpriteScaledFactor(gSpriteArray[i], 0.5f, 0.5f);
    }
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 117, 0, 875, 875, 130, 0, 0);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[117], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 127, 0, 876, 876, 112, 0, 0);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[127], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 128, 0, 877, 877, 112, 0, 0);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[128], 0.5f, 0.5f);
    err = MyLoadSpriteFromPictResource(gSpriteWorldP, 183, 0, 2020, 2020, 1, 0);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gSpriteArray[183], 96, 111);
    err = MyLoadSpriteFromPictResource(gSpriteWorldP, 205, 0, 1005, 1005, 1, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[205], 0.5f, 0.5f);
    err = MyLoadSpriteFromPictResource(gSpriteWorldP, 184, 0, 2050, 2050, 1, 4);
    if (err != kNoError)
        MM_FATAL(err);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 210, 0, 129, 129, 32, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[210], 0.5f, 0.5f);
    err = MyLoadSpriteFromPictResource(gSpriteWorldP, 211, 0, 9100, 9100, 1, 0);
    if (err != kNoError)
        MM_FATAL(err);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 212, 0, 9210, 9210, 64, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[212], 0.5f, 0.5f);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 213, 0, 9220, 9220, 64, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[213], 0.5f, 0.5f);
    err = LoadStandardDimmableButtonSprite(&gSpriteArray[214], 7500, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[214], 0.6f, 0.6f);
    err = LoadStandardButtonSprite(&gSpriteArray[215], 7520, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[215], 0.6f, 0.6f);
    err = MyLoadSpriteFromPictResource(gSpriteWorldP, 219, 0, 140, 140, 1, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[219], 0.5f, 0.5f);
    KeepMusicPlaying();
    path = GetFontsFolderPath("BitmapFont.png");
    err = SWCreateSpriteFromSingleFileXY(&gSpriteArray[220], 0, path, 52, 52, 0, 0, 0, 116, 0);
    if (err != kNoError)
        MM_FATAL(err);
    path = GetSpriteImagePath("optionsDialog/checkbox.png");
    err = LoadMiscButtonSpriteFromFile(&gSpriteArray[221], 4, path, 0);
    if (err != kNoError)
        MM_FATAL(err);
    path = GetSpriteImagePath("optionsDialog/keyInputBox.png");
    err = LoadMiscButtonSpriteFromFile(&gSpriteArray[222], 2, path, 0);
    if (err != kNoError)
        MM_FATAL(err);
    path = GetSpriteImagePath("optionsDialog/controlsTab.png");
    err = LoadMiscButtonSpriteFromFile(&gSpriteArray[224], 2, path, 0);
    if (err != kNoError)
        MM_FATAL(err);
    path = GetSpriteImagePath("optionsDialog/optionsTab.png");
    err = LoadMiscButtonSpriteFromFile(&gSpriteArray[223], 2, path, 0);
    if (err != kNoError)
        MM_FATAL(err);
        // Fun and Extras tabs (this version); fall back to the Options tab if they are missing
    path = GetExtrasFilePath("Sprites/optionsDialog/funTab.png");
    if (path == NULL || LoadMiscButtonSpriteFromFile(&gSpriteArray[227], 2, path, 0) != kNoError)
        LoadMiscButtonSpriteFromFile(&gSpriteArray[227], 2, GetSpriteImagePath("optionsDialog/optionsTab.png"), 0);
    path = GetExtrasFilePath("Sprites/optionsDialog/extrasTab.png");
    if (path == NULL || LoadMiscButtonSpriteFromFile(&gSpriteArray[228], 2, path, 0) != kNoError)
        LoadMiscButtonSpriteFromFile(&gSpriteArray[228], 2, GetSpriteImagePath("optionsDialog/optionsTab.png"), 0);
    path = GetSpriteImagePath("episodeText.png");
    SWCreateSpriteFromVerticalFile(&gSpriteArray[226], 0, path, 0, 3, 0);
    path = GetSpriteImagePath("questionMark.png");
    err = SWCreateSpriteFromFile(&gSpriteArray[225], 0, path, 1, 0);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledFactor(gSpriteArray[225], 0.5f, 0.5f);
}

void LoadNonPixelMaskSprites(void)
{
    SWError err;
    short i, pictID;

    WriteCStringToFile("LoadNonPixelMaskSprites entered...\r");
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 202, NULL, 3005, 3005, 50, 0, kNoMask);
    if (err != kNoError)
        MM_FATAL(err);
    err = MyLoadSpriteFromPictResource(gSpriteWorldP, 203, NULL, 9200, 9200, 1, kNoMask);
    if (err != kNoError)
        MM_FATAL(err);
    IncreaseProgressBar(1);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 200, NULL, 3010, 3010, 40, 0, kNoMask);
    if (err != kNoError)
        MM_FATAL(err);
    err = MyLoadSpriteFromSinglePict(gSpriteWorldP, 201, NULL, 3015, 3015, 20, 0, kNoMask);
    if (err != kNoError)
        MM_FATAL(err);
    IncreaseProgressBar(1);

        // doors (each color: 4 PICTs)
    for (i = 49, pictID = 600; i < 53; i++, pictID++) {
        err = MyLoadSpriteFromPictResource(gSpriteWorldP, i, NULL, pictID, pictID, 1, 16);
        if (err != kNoError)
            MM_FATAL(err);
    }
    IncreaseProgressBar(1);
    KeepMusicPlaying();
    for (i = 57, pictID = 610; i < 61; i++, pictID++) {
        err = MyLoadSpriteFromPictResource(gSpriteWorldP, i, NULL, pictID, pictID, 1, 16);
        if (err != kNoError)
            MM_FATAL(err);
    }
    IncreaseProgressBar(1);
    for (i = 45, pictID = 620; i < 49; i++, pictID++) {
        err = MyLoadSpriteFromPictResource(gSpriteWorldP, i, NULL, pictID, pictID, 1, 16);
        if (err != kNoError)
            MM_FATAL(err);
    }
    IncreaseProgressBar(1);
    for (i = 61, pictID = 630; i < 65; i++, pictID++) {
        err = MyLoadSpriteFromPictResource(gSpriteWorldP, i, NULL, pictID, pictID, 1, 16);
        if (err != kNoError)
            MM_FATAL(err);
    }
    IncreaseProgressBar(1);
    for (i = 53, pictID = 640; i < 57; i++, pictID++) {
        err = MyLoadSpriteFromPictResource(gSpriteWorldP, i, NULL, pictID, pictID, 1, 16);
        if (err != kNoError)
            MM_FATAL(err);
    }
    IncreaseProgressBar(1);
    for (i = 65, pictID = 650; i < 69; i++, pictID++) {
        err = MyLoadSpriteFromPictResource(gSpriteWorldP, i, NULL, pictID, pictID, 1, 16);
        if (err != kNoError)
            MM_FATAL(err);
    }
    IncreaseProgressBar(1);

    err = MyLoadSpriteFromPictResource(gSpriteWorldP, 204, NULL, 3103, 3103, 1, 16);
    if (err != kNoError)
        MM_FATAL(err);
    err = MMCreateSpriteFromVerticalFile(gSpriteWorldP, &gSpriteArray[216], NULL, "paused dialog.png", 1, kNoMask);
    if (err != kNoError)
        MM_FATAL(err);
    err = MMCreateSpriteFromSingleFileXY(gSpriteWorldP, &gSpriteArray[217], NULL, "paused dialog buttons.png",
                                         134, 64, 15, 32);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[217], 0.6f, 0.6f);
    err = MMCreateSpriteFromSingleFileXY(gSpriteWorldP, &gSpriteArray[218], NULL, "other buttons.png",
                                         134, 64, 12, 32);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gSpriteArray[218], 0.6f, 0.6f);
    IncreaseProgressBar(1);
    KeepMusicPlaying();
}

void MySetSpriteScaledFactor(SpritePtr spriteP, float widthFactor, float heightFactor)
{
    SWSetSpriteScaledFactor(spriteP, widthFactor, heightFactor);
}

void MySetSpriteScaledSize(SpritePtr spriteP, short width, short height)
{
    SWSetSpriteScaledSize(spriteP, width, height);
}

/// The file for a sprite image: the custom mansion's own if it has one, else the
/// game's standard one.
static const char *SpriteImageFileName(short pictID, Boolean isCustom, const char *customPath)
{
    return isCustom ? customPath : GetFileNameForResource(GetSpriteImagePath(NULL), pictID);
}

SWError MyLoadSpriteFromSinglePict(SpriteWorldPtr spriteWorldP, short spriteIndex, void *storageP, short pictID,
                                   short maskID, short frameWidth, short borderWidth, int maskType)
{
    char customPath[500];
    Boolean isCustom, selfMask = pictID == maskID && maskType != kNoMask;

    (void)spriteWorldP;
    if (!CheckIfSpriteShouldBeLoaded(pictID, spriteIndex, &isCustom, customPath))
        return kNoError;
    if (gSpriteArray[spriteIndex] != NULL)
        SWDisposeSprite(&gSpriteArray[spriteIndex]);
    return SWCreateSpriteFromSingleFile(&gSpriteArray[spriteIndex], storageP,
                                        SpriteImageFileName(pictID, isCustom, customPath),
                                        frameWidth, borderWidth, selfMask);
}

SWError MyLoadSpriteFromPictResource(SpriteWorldPtr spriteWorldP, short spriteIndex, void *storageP, short pictID,
                                     short maskID, short maxFrames, int maskType)
{
    char customPath[500];
    Boolean isCustom, selfMask = pictID == maskID && maskType != kNoMask;

    (void)spriteWorldP;
    SW_ASSERT(maxFrames == 1);
    if (!CheckIfSpriteShouldBeLoaded(pictID, spriteIndex, &isCustom, customPath))
        return kNoError;
    if (gSpriteArray[spriteIndex] != NULL)
        SWDisposeSprite(&gSpriteArray[spriteIndex]);
    return SWCreateSpriteFromFile(&gSpriteArray[spriteIndex], storageP,
                                  SpriteImageFileName(pictID, isCustom, customPath), maxFrames, selfMask);
}

SWError MyLoadSpriteFromSinglePictXY(SpriteWorldPtr spriteWorldP, short spriteIndex, void *storageP, short pictID,
                                     short maskID, short frameWidth, short frameHeight, short borderWidth,
                                     short borderHeight, Boolean hasOutsideBorder, short maxFrames, int maskType)
{
    char customPath[500];
    Boolean isCustom, selfMask = pictID == maskID && maskType != kNoMask;

    (void)spriteWorldP;
    if (!CheckIfSpriteShouldBeLoaded(pictID, spriteIndex, &isCustom, customPath))
        return kNoError;
    if (gSpriteArray[spriteIndex] != NULL)
        SWDisposeSprite(&gSpriteArray[spriteIndex]);
    return SWCreateSpriteFromSingleFileXY(&gSpriteArray[spriteIndex], storageP,
                                          SpriteImageFileName(pictID, isCustom, customPath), frameWidth,
                                          frameHeight, borderWidth, borderHeight, hasOutsideBorder, maxFrames,
                                          selfMask);
}

/**
 * Standard sprites are loaded once. When a mansion is opened, sprites it has its
 * own image for are reloaded from the mansion (and set back to standard when
 * another mansion without one is opened).
 */
Boolean CheckIfSpriteShouldBeLoaded(short pictID, short spriteIndex, Boolean *isCustom, char *customPath)
{
    short i;

    if (!spriteFlagsWereInitialized) {
        for (i = 0; i < kNumSpriteArrayEntries; i++)
            spriteFlags[i] = -1;
        spriteFlagsWereInitialized = true;
    }
    *isCustom = false;
    if (spriteFlags[spriteIndex] == -1) {
        spriteFlags[spriteIndex] = 0;
        return true;
    }
    if (DoesCustomPICTResourceExist(gCurOpenMansionFilePath, pictID)) {
        sprintf(customPath, "%s'%s'/%05d.png", gCurOpenMansionFilePath, "PICT", pictID);
        spriteFlags[spriteIndex] = 1;
        *isCustom = true;
        return true;
    }
    if (spriteFlags[spriteIndex] == 1) {
        spriteFlags[spriteIndex] = 0;
        return true;
    }
    return false;
}

/// Lists the numbered images in the mansion's 'PICT' folder.
int ScanDirectoryForPICTResIDs(const char *mansionPath)
{
    char folderPath[500];
    DIR *dirP;
    struct dirent *entryP;
    int resID;

    gLastValidPICTResIDIndex = 0;
    snprintf(folderPath, sizeof(folderPath), "%s%s", mansionPath, "'PICT'");
    dirP = opendir(folderPath);
    if (dirP == NULL)
        return -1;
    printf("Scanning path for custom Tile/Sprite PICT res IDs... %s \n", folderPath);
    snprintf(scannedPath, sizeof(scannedPath), "%s", mansionPath);
    while ((entryP = readdir(dirP)) != NULL) {
        if (entryP->d_type != DT_REG)
            continue;
        resID = atoi(entryP->d_name);
        if (resID > 0) {
            gValidPICTResIDs[gLastValidPICTResIDIndex++] = (short)resID;
            if (gLastValidPICTResIDIndex >= kMaxValidPICTResIDs) {
                printf("ERROR! ScanDirectoryForPICTResIDs exceeded array bounds!");
                gLastValidPICTResIDIndex = 0;
                break;
            }
        }
    }
    closedir(dirP);
    return 0;
}

Boolean DoesCustomPICTResourceExist(const char *mansionPath, short pictID)
{
    char path[500];
    short i;

    if (gLastValidPICTResIDIndex <= 0 || strcmp(scannedPath, mansionPath) != 0)
        return false;
    for (i = 0; i < gLastValidPICTResIDIndex; i++) {
        if (gValidPICTResIDs[i] == pictID) {
            snprintf(path, sizeof(path), "%s'%s'/%05d.png", gCurOpenMansionFilePath, "PICT", pictID);
            return DoesFileExist(path);
        }
    }
    return false;
}

void LoadStatsSprites(void)
{
    SWError err;

    SW_ASSERT(!gStatsAreaSpritesAreValid);
    err = CreateSpriteFromSinglePict(gSpriteWorldP, &gStatsSpriteArray[0], NULL, 5100, 5100, 40, 0, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gStatsSpriteArray[0], 0.5f, 0.5f);
    err = CreateSpriteFromSinglePict(gSpriteWorldP, &gStatsSpriteArray[1], NULL, 5200, 5201, 60, 0, 40);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gStatsSpriteArray[1], 0.5f, 0.5f);
    gStatsAreaSpritesAreValid = true;
}

void LoadMapScreenSprites(void)
{
    char fileName[255];
    SWError err;
    short i;

    SW_ASSERT(!gMapScreenSpritesAreValid);
    for (i = 0; i < 11; i++) {
        snprintf(fileName, sizeof(fileName), "map_mansion_%d.png", i);
        err = SWCreateSpriteFromFile(&gMapScreenSpriteArray[i], NULL, GetMapScreenFolderPath(fileName), 1, false);
        if (err != kNoError)
            MM_FATAL(err);
        SWSetSpriteScaledFactor(gMapScreenSpriteArray[i], 0.5f, 0.5f);
    }
    err = SWCreateSpriteFromSingleFile(&gMapScreenSpriteArray[11], NULL,
                                       GetMapScreenFolderPath("map_screen_mansion_names.png"), 48, 0, false);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledFactor(gMapScreenSpriteArray[11], 0.5f, 0.5f);
    err = CreateSpriteFromPictResource(gSpriteWorldP, &gMapScreenSpriteArray[12], NULL, 6250, 6250, 1, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gMapScreenSpriteArray[12], 0.73f, 0.73f);
    err = SWCreateSpriteFromFile(&gMapScreenSpriteArray[13], NULL, GetMapScreenFolderPath("select_a_mansion.png"),
                                 1, false);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledFactor(gMapScreenSpriteArray[13], 0.5f, 0.5f);
    err = SWCreateSpriteFromFile(&gMapScreenSpriteArray[15], NULL, GetMapScreenFolderPath("start_here.png"), 1, false);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledFactor(gMapScreenSpriteArray[15], 0.5f, 0.5f);
    err = CreateSpriteFromPictResource(gSpriteWorldP, &gMapScreenSpriteArray[14], NULL, 6260, 6260, 1, 4);
    if (err != kNoError)
        MM_FATAL(err);
    gMapScreenSpritesAreValid = true;
}

Boolean gTitleScreenHasExtraButtons = false;

void LoadTitleScreenSprites(void)
{
    char fileName[255];
    SWError err = kNoError;
    short i, cloud;

    if (gTitleScreenSpritesAreValid)
        return;
    WriteCStringToFile("LoadTitleScreenSprites entered...\r");

        // this version's sheet adds rows after the original seven (tools/make_extras.py)
    gTitleScreenHasExtraButtons = false;
    if (GetExtrasFilePath("Screens/titleScreen/buttons_extra.png") != NULL &&
        SWCreateSpriteFromSingleFileXY(&gTitleScreenSpriteArray[0], NULL,
                                       GetExtrasFilePath("Screens/titleScreen/buttons_extra.png"), 360, 60, 0, 0,
                                       false, kNumTitleButtonRows * 2, false) == kNoError)
        gTitleScreenHasExtraButtons = true;
    else
        err = SWCreateSpriteFromSingleFileXY(&gTitleScreenSpriteArray[0], NULL, GetTitleScreenFolderPath("buttons.png"),
                                             360, 60, 0, 0, false, 14, false);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledSize(gTitleScreenSpriteArray[0], 960, 560);
    err = SWCreateSpriteFromSingleFileXY(&gTitleScreenSpriteArray[2], NULL,
                                         GetTitleScreenFolderPath("tab_buttons.png"), 240, 72, 0, 0, false, 8, false);
    if (err != kNoError)
        MM_FATAL(err);
    err = SWCreateSpriteFromFile(&gTitleScreenSpriteArray[3], NULL, GetTitleScreenFolderPath("tab_button_glow.png"),
                                 1, false);
    if (err != kNoError)
        MM_FATAL(err);
    KeepMusicPlaying();

    err = SWCreateSpriteFromFile(&gTitleScreenSpriteArray[1], NULL, GetTitleScreenFolderPath("logo.png"), 1, false);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledSize(gTitleScreenSpriteArray[1], 1166, 168);
    err = SWCreateSpriteFromFile(&gTitleScreenSpriteArray[4], NULL,
                                 GetTitleScreenFolderPath("mansion_bottom_and_ground.png"), 1, false);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledSize(gTitleScreenSpriteArray[4], 1280, 590);
    err = SWCreateSpriteFromFile(&gTitleScreenSpriteArray[5], NULL, GetTitleScreenFolderPath("mansion_top.png"),
                                 1, false);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledSize(gTitleScreenSpriteArray[5], 669, 1120);
    err = SWCreateSpriteFromFile(&gTitleScreenSpriteArray[6], NULL, GetTitleScreenFolderPath("foreground_fog.png"),
                                 1, false);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledSize(gTitleScreenSpriteArray[6], 1280, 403);
    err = SWCreateSpriteFromFile(&gTitleScreenSpriteArray[7], NULL, GetTitleScreenFolderPath("full_moon.png"),
                                 1, false);
    if (err != kNoError)
        MM_FATAL(err);

    for (i = 8, cloud = 1; i < 12; i++, cloud++) {
        snprintf(fileName, sizeof(fileName), "cloud_%d.png", cloud);
        err = SWCreateSpriteFromVerticalFile(&gTitleScreenSpriteArray[i], NULL, GetTitleScreenFolderPath(fileName),
                                             0, 2, false);
        if (err != kNoError)
            MM_FATAL(err);
        SWSetSpriteScaledFactor(gTitleScreenSpriteArray[i], 1.4286f, 1.4286f);
    }
    err = SWCreateSpriteFromVerticalFile(&gTitleScreenSpriteArray[12], NULL,
                                         GetTitleScreenFolderPath("jack_animated.png"), 0, 3, false);
    if (err != kNoError)
        MM_FATAL(err);
    err = SWCreateSpriteFromVerticalFile(&gTitleScreenSpriteArray[13], NULL,
                                         GetTitleScreenFolderPath("flag_animated.png"), 0, 3, false);
    if (err != kNoError)
        MM_FATAL(err);

    WriteCStringToFile("LoadTitleScreenSprites completed with no problems.\r");
    gTitleScreenSpritesAreValid = true;
    if (SWStickyError() != kNoError)
        MM_FATAL(SWStickyError());
}

void LoadDifficultyScreenSprites(void)
{
    SWError err;
    short i;

    SW_ASSERT(!gDifficultyScreenSpritesAreValid);
    for (i = 0; i < 3; i++) {
        err = LoadStandardButtonSprite(&gDifficultyScreenSpriteArray[i], 7320 + i, 4);
        if (err != kNoError)
            MM_FATAL(err);
        SWSetSpriteScaledFactor(gDifficultyScreenSpriteArray[i], 0.625f, 0.625f);
    }
    KeepMusicPlaying();
    err = CreateSpriteFromPictResource(gSpriteWorldP, &gDifficultyScreenSpriteArray[3], NULL, 7310, 7310, 1, 4);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledFactor(gDifficultyScreenSpriteArray[3], 0.625f, 0.625f);
    err = CreateSpriteFromPictResource(gSpriteWorldP, &gDifficultyScreenSpriteArray[4], NULL, 7350, 7350, 3, 4);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledFactor(gDifficultyScreenSpriteArray[4], 0.9f, 0.9f);
    KeepMusicPlaying();
    gDifficultyScreenSpritesAreValid = true;
}

void LoadOrganScreenSprites(void)
{
    SWError err;
    short i;

    SW_ASSERT(!gOrganScreenSpritesAreValid);
    err = SWCreateSpriteFromSingleFile(&gOrganScreenSpriteArray[0], NULL, GetOrganScreenFolderPath("organ_dude.png"),
                                       360, 0, false);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledFactor(gOrganScreenSpriteArray[0], 0.5f, 0.5f);
    err = SWCreateSpriteFromVerticalFile(&gOrganScreenSpriteArray[2], NULL,
                                         GetOrganScreenFolderPath("organScreenText.png"), 0, 8, false);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledFactor(gOrganScreenSpriteArray[2], 0.5f, 0.5f);
    err = SWCreateSpriteFromVerticalFile(&gOrganScreenSpriteArray[1], NULL,
                                         GetOrganScreenFolderPath("animated_keys.png"), 0, 4, false);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledFactor(gOrganScreenSpriteArray[1], 0.5f, 0.5f);
    for (i = 0; i < 3; i++)
        SWLockSprite(gOrganScreenSpriteArray[i]);
    gOrganScreenSpritesAreValid = true;
}

void DisposeMapScreenSprites(void)
{
    short i;

    if (!gMapScreenSpritesAreValid)
        return;
    for (i = 0; i < 16; i++)
        SWDisposeSprite(&gMapScreenSpriteArray[i]);
    gMapScreenSpritesAreValid = false;
}

void DisposeTitleScreenSprites(void)
{
    short i;

    if (!gTitleScreenSpritesAreValid)
        return;
    for (i = 0; i < 14; i++)
        SWDisposeSprite(&gTitleScreenSpriteArray[i]);
    gTitleScreenSpritesAreValid = false;
}

void DisposeDifficultyScreenSprites(void)
{
    short i;

    if (!gDifficultyScreenSpritesAreValid)
        return;
    for (i = 0; i < 5; i++)
        SWDisposeSprite(&gDifficultyScreenSpriteArray[i]);
    gDifficultyScreenSpritesAreValid = false;
}

void DisposeOrganScreenSprites(void)
{
    short i;

    if (!gOrganScreenSpritesAreValid)
        return;
    for (i = 0; i < 3; i++)
        SWDisposeSprite(&gOrganScreenSpriteArray[i]);
    gOrganScreenSpritesAreValid = false;
}

void DisposeSprites(void)
{
    short i;

    for (i = 0; i < kNumSpriteArrayEntries; i++)
        SWDisposeSprite(&gSpriteArray[i]);
    if (gStatsAreaSpritesAreValid) {
        for (i = 0; i < 2; i++)
            SWDisposeSprite(&gStatsSpriteArray[i]);
        gStatsAreaSpritesAreValid = false;
    }
}

void IncreaseProgressBar(short amount)
{
    (void)amount;
}

// ---------------------------------------------------------------------------
// Screen sprites

/// Map screen: 0-10 mansion buttons, 11 mansion names, 12-15 decorations.
SpritePtr NewMapScreenSprite(short index)
{
    SpritePtr newSpriteP;
    SWError err;

    SW_ASSERT(gMapScreenSpritesAreValid);
    SW_ASSERT(index >= 0 && index <= 15);
    if (index <= 10) {
        newSpriteP = CloneButtonSprite(gMapScreenSpriteArray[index], index);
        if (newSpriteP != NULL) {
            SetUpButtonSprite(newSpriteP, 0, 0, 0, 0);
            SWAddSprite(gBottomSpriteLayerP, newSpriteP);
        }
    }
    else if (index == 11) {
        newSpriteP = CloneButtonSprite(gMapScreenSpriteArray[11], 0);
        if (newSpriteP == NULL)
            MM_FATAL(-1);
        SWAddSprite(gTopSpriteLayerP, newSpriteP);
    }
    else {
        err = SWCloneSprite(gMapScreenSpriteArray[index], &newSpriteP, NULL, 0);
        if (err != kNoError)
            MM_FATAL(err);
        if (index == 12 || index == 14 || index == 15)
            SWAddSprite(gForegroundSpriteLayerP, newSpriteP);
        else
            SWAddSprite(gTopSpriteLayerP, newSpriteP);
    }
    return newSpriteP;
}

SpritePtr NewTitleScreenButtonSprite(short index, short buttonIndex)
{
    SpritePtr newSpriteP;

    SW_ASSERT(gTitleScreenSpritesAreValid);
    newSpriteP = CloneButtonSprite(gTitleScreenSpriteArray[index], buttonIndex);
    if (newSpriteP != NULL)
        SWAddSprite(gTopSpriteLayerP, newSpriteP);
    return newSpriteP;
}

SpritePtr NewTitleScreenSprite(short index)
{
    SpritePtr newSpriteP;

    SW_ASSERT(gTitleScreenSpritesAreValid);
    SW_ASSERT(index >= 0 && index <= 13);
    newSpriteP = CloneGameSprite(gTitleScreenSpriteArray[index], sizeof(MiscSpriteRec));
    if (index == 6)
        SWAddSprite(gBackgroundSpriteLayerP, newSpriteP);
    if (index == 1)
        SWAddSprite(gHeroSpriteLayerP, newSpriteP);
    else if (newSpriteP->parentSpriteLayerP == NULL)
        SWAddSprite(gTopSpriteLayerP, newSpriteP);
    newSpriteP->userData = index;
    return newSpriteP;
}

SpritePtr NewDifficultyScreenButtonSprite(short index)
{
    SpritePtr newSpriteP;

    SW_ASSERT(gDifficultyScreenSpritesAreValid);
    newSpriteP = CloneButtonSprite(gDifficultyScreenSpriteArray[index], index);
    if (newSpriteP != NULL)
        SWAddSprite(gTopSpriteLayerP, newSpriteP);
    return newSpriteP;
}

SpritePtr NewDifficultyScreenSprite(short index)
{
    SpritePtr newSpriteP;

    SW_ASSERT(gDifficultyScreenSpritesAreValid);
    SW_ASSERT(index >= 0 && index <= 4);
    newSpriteP = CloneGameSprite(gDifficultyScreenSpriteArray[index], sizeof(MiscSpriteRec));
    SWAddSprite(gTopSpriteLayerP, newSpriteP);
    newSpriteP->userData = index;
    return newSpriteP;
}

SpritePtr NewOrganScreenSprite(short index)
{
    SpritePtr newSpriteP;

    SW_ASSERT(gOrganScreenSpritesAreValid);
    SW_ASSERT(index >= 0 && index <= 2);
    newSpriteP = CloneGameSprite(gOrganScreenSpriteArray[index], sizeof(MiscSpriteRec));
    if (index == 0)
        SWAddSprite(gHeroSpriteLayerP, newSpriteP);
    else if (index == 1)
        SWAddSprite(gBottomSpriteLayerP, newSpriteP);
    else
        SWAddSprite(gTopSpriteLayerP, newSpriteP);
    newSpriteP->userData = index;
    return newSpriteP;
}

// ---------------------------------------------------------------------------
// Game sprites

SpritePtr NewMainSprite(void)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[0], sizeof(HeroSpriteRec));
    HeroSpritePtr heroP = HERO(spriteP);
    short height, halfWidth, frame;

    SWSetSpriteDrawProc(spriteP, AddShadowDrawProc);
    SWAddSprite(gHeroSpriteLayerP, spriteP);
    SWSetSpriteMoveProc(spriteP, MainSpriteMoveProc);
    SWSetSpriteCollideProc(spriteP, MainSpriteCollideProc);
    SWSetSpriteFrameAdvanceMode(spriteP, kNoLoopingMode);
    spriteP->userData = kHeroSprite;
    height = spriteP->destFrameRect.bottom - spriteP->destFrameRect.top;
    halfWidth = (short)((spriteP->destFrameRect.right - spriteP->destFrameRect.left) / 2);

    SWSetRect(&heroP->doorInset, 19, 13, 18, 0);
    SWSetRect(&heroP->collisionInset, 20, 15, 25, 0);
    SWSetRect(&heroP->unusedInset, 26, 15, 25, 10);
    SWSetRect(&heroP->wallInset, 18, height - 10, 25, 0);
    heroP->normalDoorInsetTop = heroP->doorInset.top;
    heroP->normalInsetTop = heroP->collisionInset.top;
    SWSetRect(&heroP->ladderUpInset, 12, 3, 12, 0);
    SWSetRect(&heroP->ladderDownInset, 16, height + 8, 16, -14);
    SWSetRect(&heroP->vineInset, 0, 40, 0, 0);
    SWSetRect(&heroP->vineFromGroundInset, 0, height + 8, 0, -14);
    SWSetRect(&heroP->poleInset, halfWidth - 2, 3, halfWidth - 2, 0);

        // the push-up frames (15-17) collide only with their lower half
    for (frame = 0; frame < spriteP->maxFrames; frame++) {
        if (frame < 15 || frame > 17)
            SWSetFrameCollisionInset(spriteP->frameArray[frame], heroP->collisionInset.left,
                                     heroP->collisionInset.top, heroP->collisionInset.right,
                                     heroP->collisionInset.bottom);
        else
            SWSetFrameCollisionInset(spriteP->frameArray[frame], 0, heroP->collisionInset.top * 2, 0,
                                     heroP->collisionInset.bottom);
    }

    heroP->platformHorizDelta = 0;
    heroP->conveyorDelta = 0;
    heroP->canJump = true;
    heroP->canDoPushups = true;
    heroP->isJumping = false;
    heroP->isDoingPushups = false;
    heroP->jumpFrameCount = 0;
    heroP->unused176 = false;
    heroP->facingRight = true;
    heroP->didSkeletonCollapse = false;
    heroP->didStoneCollapse = false;
    heroP->deathAnimIsDone = false;
    heroP->isDead = false;
    heroP->ladderDirection = 0;
    heroP->isLanding = false;
    heroP->onGround = true;
    heroP->leverP = NULL;
    heroP->playedFallingSound = false;
    heroP->pedestalP = NULL;
    heroP->deadBodyP = NULL;
    heroP->attachedSpriteP = NULL;
    heroP->actionKeyFrames = 1;
    heroP->jumpBufferFrames = 0;
    heroP->upKeyDelay = 0;
    heroP->downKeyDelay = 0;
    heroP->showQuestionMark = false;
    heroP->idleDelay = 300;
    heroP->blinkTimer = 200;
    heroP->objectID = -1;

    SWSetSpriteFrameRange(spriteP, 0, 2);
    SWSetSpriteFrameAdvanceMode(spriteP, kPatrollingMode);
    SWSetSpriteFrameProc(spriteP, HeroMiscFrameProc);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameTime(spriteP, 0);
    heroP->frameTimer = 15;
    heroP->frameDelay = 15;
    heroP->frameDirection = 1;
    return spriteP;
}

/// The dark halo around the hero in dark rooms (index 211 is its image).
SpritePtr NewHaloSprite(void)
{
    SpritePtr spriteP = NULL;
    SWError err;

    err = SWCloneSprite(gSpriteArray[211], &spriteP, NULL, 0);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteBlendMode(spriteP, 0x306, 0x303);
    spriteP->misc1.n = 1;
    spriteP->userData = 0xcfff;
    SWSetSpriteTranslucency(spriteP, spriteP->userData);
    SWAddSprite(gHaloSpriteLayerP, spriteP);
    spriteP->miscFloat = 5.15f;
    SWSetSpriteScaledFactor(spriteP, spriteP->miscFloat, spriteP->miscFloat);
    return spriteP;
}

SpritePtr NewSkullSpiderSprite(void)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[kSkullSpiderSprite], sizeof(SkullSpiderSpriteRec));
    SkullSpiderSpritePtr spiderP = SKULLSPIDER(spriteP);

    SWAddSprite(gEnemySpriteLayerP, spriteP);
    SWSetSpriteMoveProc(spriteP, SkullSpiderMoveProc);
    spriteP->userData = kSkullSpiderSprite;
    SWSetSpriteFrameAdvanceMode(spriteP, kPatrollingMode);
    SWSetSpriteFrameProc(spriteP, SkullSpiderFrameProc);
    SWSetRect(&spiderP->collisionInset, 15, 0, 15, 0);
    SetCollisionInsetForAllFrames(spriteP, &spiderP->collisionInset);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameTime(spriteP, 0);
    spiderP->frameDelay = 10;
    spiderP->frameTimer = 10;
    spiderP->frameDirection = 1;
    return spriteP;
}

SpritePtr NewScorpionSprite(void)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[kScorpionSprite], sizeof(CrawlerSpriteRec));
    CrawlerSpritePtr scorpionP = CRAWLER(spriteP);

    SWAddSprite(gEnemySpriteLayerP, spriteP);
    SWSetSpriteMoveProc(spriteP, ScorpionMoveProc);
    spriteP->userData = kScorpionSprite;
    SWSetSpriteFrameAdvanceMode(spriteP, kNoLoopingMode);
    SWSetRect(&scorpionP->collisionInset, 4, 0, 4, 0);
    SetCollisionInsetForAllFrames(spriteP, &scorpionP->collisionInset);
    SWSetSpriteFrameProc(spriteP, ScorpionFrameProc);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameTime(spriteP, 0);
    scorpionP->frameTimer = 7;
    scorpionP->frameDirection = 1;
    return spriteP;
}

SpritePtr NewEyeballSprite(void)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[kEyeballSprite], sizeof(EyeballSpriteRec));

    SWAddSprite(gEyeballSpriteLayerP, spriteP);
    SWSetSpriteMoveProc(spriteP, EyeballMoveProc);
    SWSetSpriteFrameProc(spriteP, EyeballFrameProc);
    SWSetSpriteFrameTime(spriteP, 83);
    SWSetSpriteFrameAdvance(spriteP, 0);
    spriteP->userData = kEyeballSprite;
    SWSetCurrentFrameIndex(spriteP, GetRandom(0, 6));
    SWSetRect(&EYEBALL(spriteP)->collisionInset, 0, 0, 0, 0);
    SetCollisionInsetForAllFrames(spriteP, &EYEBALL(spriteP)->collisionInset);
    return spriteP;
}

SpritePtr NewRavenSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(RavenSpriteRec));
    RavenSpritePtr ravenP = RAVEN(spriteP);

    SWAddSprite(gBirdSpriteLayerP, spriteP);
    spriteP->userData = type;
    SWSetSpriteMoveProc(spriteP, type == kBlueRavenSprite ? BlueRavenMoveProc : RedRavenMoveProc);
    SWSetSpriteFrameAdvanceMode(spriteP, kPatrollingMode);
    SWSetSpriteFrameProc(spriteP, BirdFrameProc);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameTime(spriteP, 0);
    ravenP->frameTimer = 6;
    ravenP->frameDirection = 1;
    SWSetRect(&ravenP->collisionInset, 4, 0, 6, 0);
    SetCollisionInsetForAllFrames(spriteP, &ravenP->collisionInset);
    ravenP->angle = 0;
    ravenP->baseVertLoc = -1.0f;
    return spriteP;
}

SpritePtr NewSnakeSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(CrawlerSpriteRec));
    CrawlerSpritePtr snakeP = CRAWLER(spriteP);

    SWAddSprite(gSnakeSpriteLayerP, spriteP);
    SWSetSpriteMoveProc(spriteP, SnakeMoveProc);
    spriteP->userData = type;
    SWSetSpriteFrameAdvanceMode(spriteP, kPatrollingMode);
    SWSetSpriteFrameProc(spriteP, SnakeFrameProc);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameTime(spriteP, 0);
    snakeP->frameTimer = 12;
    snakeP->frameDirection = 1;
    SWSetRect(&snakeP->collisionInset, 5, 0, 5, -2);
    SetCollisionInsetForAllFrames(spriteP, &snakeP->collisionInset);
    return spriteP;
}

SpritePtr NewMedusasHeadSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(MedusaSpriteRec));
    MedusaSpritePtr medusaP = MEDUSA(spriteP);

    SWAddSprite(gMedusaSpriteLayerP, spriteP);
    SWSetSpriteMoveProc(spriteP, MedusasHeadMoveProc);
    SWSetSpriteCollideProc(spriteP, MedusaCollideProc);
    spriteP->userData = type;
    SWSetSpriteFrameAdvanceMode(spriteP, kPatrollingMode);
    SWSetSpriteFrameRange(spriteP, 0, 2);
    medusaP->soundTimer = 0;
    SWSetSpriteFrameProc(spriteP, MedusaFrameProc);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameTime(spriteP, 0);
    medusaP->frameDelay = 20;
    medusaP->frameTimer = 20;
    medusaP->frameDirection = 1;
    medusaP->ignoresWalls = true;
    return spriteP;
}

SpritePtr NewSpiderSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(SpiderSpriteRec));
    SpiderSpritePtr spiderP = SPIDER(spriteP);

    spriteP->userData = type;
    SWSetSpriteFrameProc(spriteP, SpiderFrameProc);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameTime(spriteP, 0);
    spiderP->frameDirection = 1;
    if (type == kBigSpiderSprite) {
        SWAddSprite(gBigSpiderSpriteLayerP, spriteP);
        SWSetSpriteMoveProc(spriteP, BigSpiderMoveProc);
        SWSetSpriteCollideProc(spriteP, BigSpiderCollideProc);
        spiderP->frameDelay = 10;
        spiderP->frameTimer = 10;
        spiderP->state = kSpiderWatching;
    }
    else {
        SWAddSprite(gEnemySpriteLayerP, spriteP);
        SWSetSpriteMoveProc(spriteP, SmallSpiderMoveProc);
        spiderP->frameDelay = 15;
        spiderP->frameTimer = 15;
        spiderP->state = kSpiderDropping;
        SWSetRect(&spiderP->collisionInset, 0, 0, 0, 2);
        SetCollisionInsetForAllFrames(spriteP, &spiderP->collisionInset);
    }
    SWSetSpriteFrameAdvanceMode(spriteP, kPatrollingMode);
    spiderP->topVertLoc = -1;
    spiderP->preyP = NULL;
    return spriteP;
}

SpritePtr NewSkeletonFishSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(FishSpriteRec));

    SWAddSprite(gEnemySpriteLayerP, spriteP);
    SWSetSpriteMoveProc(spriteP, SkeletonFishMoveProc);
    spriteP->userData = type;
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameTime(spriteP, 0);
    FISH(spriteP)->frameDirection = 1;
    FISH(spriteP)->firstFrame = 0;
    FISH(spriteP)->hasTurned = false;
    return spriteP;
}

SpritePtr NewWalkingSkeletonSprite(void)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[kWalkingSkeletonSprite], sizeof(SkeletonSpriteRec));

    SWAddSprite(gSkeletonBonesSpriteLayerP, spriteP);
    spriteP->userData = kWalkingSkeletonSprite;
    SWSetRect(&SKELETON(spriteP)->collisionInset, 18, 0, 22, 0);
    SKELETON(spriteP)->facingRight = false;
    SKELETON(spriteP)->hasUncollapsed = false;
    SKELETON(spriteP)->wakeTimer = 0;
    return spriteP;
}

/// Objects: 0x1e-0x21 treasure, 0x22 shield, 0x23 backpack, 0x24 lantern, 0x25 map
/// piece, 0x26 sign, 0x27-0x2c keys.
SpritePtr NewObjectSprite(short type)
{
    SpritePtr spriteP;
    ObjectSpritePtr objectP;

    SW_ASSERT(type >= kFirstObjectSprite && type <= kLastObjectSprite);
    spriteP = CloneGameSprite(gSpriteArray[type], sizeof(ObjectSpriteRec));
    objectP = OBJECT(spriteP);
    SWSetSpriteCollideProc(spriteP, ObjectSpriteCollideProc);
    SWAddSprite(gObjectSpriteLayerP, spriteP);
    spriteP->userData = type;
    SWSetCurrentFrameIndex(spriteP, GetRandom(0, spriteP->maxFrames - 1));
    SWSetRect(&objectP->collisionInset, 0, 0, 0, 0);

    if (type == 0x22 || type == 0x23 || type == 0x25) {
            // they glint now and then
        MySetSpriteFrameTime(spriteP, type == 0x25 ? 125 : 100);
        SWSetSpriteFrameProc(spriteP, ShieldSpriteFrameProc);
        SWSetSpriteFrameAdvance(spriteP, 0);
        SWSetCurrentFrameIndex(spriteP, 0);
        objectP->glowDelay = GetRandom(0, 19);
        if (type == 0x23)
            SWSetSpriteMoveProc(spriteP, FallingObjectMoveProc);
    }
    else if (type >= 0x27 && type <= 0x2c) {
        SWSetSpriteFrameAdvanceMode(spriteP, kNoLoopingMode);
        MySetSpriteFrameTime(spriteP, 71);
    }
    else if (type == 0x1e || type == 0x1f) {
        SWSetSpriteMoveProc(spriteP, FallingObjectMoveProc);
    }
    else if (type == kLanternObjectSprite) {
        SWSetSpriteFrameAdvance(spriteP, 0);
        SWSetSpriteFrameProc(spriteP, LanternFrameProc);
        MySetSpriteFrameTime(spriteP, 90);
    }
    else if (type == kSignObjectSprite) {
        SWSetSpriteCollideProc(spriteP, ObjectSpriteCollideProc);
        SWRemoveSprite(spriteP);
        SWAddSprite(gSignSpriteLayerP, spriteP);
    }
    else if (type != 0x25) {
        SWSetSpriteFrameAdvanceMode(spriteP, kNoLoopingMode);
        MySetSpriteFrameTime(spriteP, 100);
    }
    objectP->glowSpriteP = NULL;
    return spriteP;
}

SpritePtr NewZapperNodeSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(ZapperNodeSpriteRec));

    SWAddSprite(gZapperNodeSpriteLayerP, spriteP);
    spriteP->userData = type;
    if (type == 0x1a || type == 0x1d) {
        SWSetSpriteMoveProc(spriteP, ZapperNodeMoveProc);
        SWSetSpriteFrameProc(spriteP, ZapperNodeFrameProc);
    }
    SWSetSpriteFrameTime(spriteP, 100);
    return spriteP;
}

SpritePtr NewZapperBeamSprite(Boolean horizontal)
{
    short index = horizontal ? 0xb9 : 0xba;
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[index], sizeof(ZapperBeamSpriteRec));

    SWAddSprite(gZapperBeamSpriteLayerP, spriteP);
    SWSetSpriteMoveProc(spriteP, ZapperBeamMoveProc);
    SWSetSpriteCollideProc(spriteP, ZapperBeamSpriteCollideProc);
    spriteP->userData = index;
    BEAM(spriteP)->isHorizontal = horizontal;
    BEAM(spriteP)->baseFrame = 0;
    BEAM(spriteP)->objectID = -1;
    return spriteP;
}

SpritePtr NewMovingPlatformSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(PlatformSpriteRec));
    PlatformSpritePtr platformP = PLATFORM(spriteP);

    spriteP->userData = type;
    if (type == kPlatformBlockerSprite) {
        SWAddSprite(gMovingPlatformBlockerSpriteLayerP, spriteP);
        SWSetSpriteVisible(spriteP, false);
    }
    SWSetRect(&platformP->collisionInset, 1, 8, 8, 0);
    if (type == 0x4e) {
        platformP->collisionInset.top += 2;
        platformP->collisionInset.right += 1;
    }
    SetCollisionInsetForAllFrames(spriteP, &platformP->collisionInset);
    platformP->isMoving = true;
    return spriteP;
}

SpritePtr NewFallingPlatformSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(PlatformSpriteRec));

    spriteP->userData = type;
    SWAddSprite(gVertMovingPlatformSpriteLayerP, spriteP);
    SWSetRect(&PLATFORM(spriteP)->collisionInset, 0, 8, 7, 0);
    SetCollisionInsetForAllFrames(spriteP, &PLATFORM(spriteP)->collisionInset);
    PLATFORM(spriteP)->startVertLoc = -1;
    return spriteP;
}

SpritePtr NewTrapDoorSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(PlatformSpriteRec));
    SWRect *insetP = &PLATFORM(spriteP)->collisionInset;

    spriteP->userData = type;
    SWAddSprite(gTrapDoorSpriteLayerP, spriteP);
    if (type == 0x5f)
        SWSetRect(insetP, 12, 13, 8, 0);
    else
        SWSetRect(insetP, 10, 13, 10, 0);
    SWSetFrameCollisionInset(spriteP->frameArray[0], insetP->left, insetP->top, insetP->right, insetP->bottom);
    return spriteP;
}

SpritePtr NewDoorSprite(short type)
{
    SpritePtr spriteP;
    DoorSpritePtr doorP;

    SW_ASSERT(type >= kFirstDoorSprite && type <= kLastDoorSprite);
    spriteP = CloneGameSprite(gSpriteArray[type], sizeof(DoorSpriteRec));
    doorP = DOOR(spriteP);
    SWAddSprite(gDoorSpriteLayerP, spriteP);
    SWSetSpriteDrawProc(spriteP, DoorSpriteDrawProc);
    spriteP->userData = type;
    doorP->opensUpward = ((type - kFirstDoorSprite) & 1) == 0;
    doorP->bottomInset = 0;
    doorP->topInset = 0;
    doorP->unused100 = false;
    doorP->isOpen = false;
    doorP->height = (short)SWGetSpriteHeight(spriteP);
    InitializeDoorMovementVariables(spriteP);
    return spriteP;
}

void InitializeDoorMovementVariables(SpritePtr spriteP)
{
    DoorSpritePtr doorP = DOOR(spriteP);

    if (spriteP == NULL)
        return;
    doorP->moveFraction = 0;
    doorP->moveSpeed = 0;
    doorP->moveAccel = 0;
    doorP->openDistance = doorP->opensUpward ? doorP->height - 1 : doorP->height - 8;
}

SpritePtr NewLeverSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(LeverSpriteRec));

    SWAddSprite(gLeverSpriteLayerP, spriteP);
    spriteP->userData = type;
    LEVER(spriteP)->isActive = true;
    LEVER(spriteP)->markerSpriteP = NULL;
    return spriteP;
}

SpritePtr NewBrickSwitchSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(LeverSpriteRec));
    long frame;

    SWAddSprite(gBrickSwitchSpriteLayerP, spriteP);
    SWSetSpriteFrameTime(spriteP, 0);
    SWSetSpriteFrameAdvance(spriteP, 0);
    spriteP->userData = type;
    LEVER(spriteP)->isActive = true;
    for (frame = 0; frame < spriteP->maxFrames; frame++)
        SWSetFrameCollisionInset(spriteP->frameArray[frame], 0, 16, 7, 0);
    return spriteP;
}

/// Backgrounds: 0x70-0x71 slow animations, 0x72-0x73 flinching statues, 0x74 big
/// web, 0x75/0x7f/0x80 animated paintings.
SpritePtr NewBackgroundSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(BackgroundSpriteRec));
    BackgroundSpritePtr backP = BACKGROUND(spriteP);

    if (type == 0x70 || type == 0x71) {
        SWSetSpriteFrameTime(spriteP, 1000);
    }
    else if (type == 0x72 || type == 0x73) {
        SWSetSpriteFrameProc(spriteP, RandomFlinchFrameProc);
        SWSetSpriteFrameTime(spriteP, 142);
        SWSetSpriteFrameAdvance(spriteP, 0);
    }
    else if (type == 0x74) {
        SWSetSpriteMoveProc(spriteP, BigWebMoveProc);
        SWSetSpriteFrameAdvance(spriteP, 0);
        backP->isArmed = true;
    }
    else if (type == 0x7f || type == 0x75 || type == 0x80) {
        SWSetSpriteFrameProc(spriteP, AnimatedPaintingFrameProc);
        SWSetSpriteFrameAdvance(spriteP, 0);
        SWSetSpriteFrameAdvanceMode(spriteP, kPatrollingMode);
        if (type == 0x80) {
            SWSetSpriteFrameTime(spriteP, 200);
            backP->frameTimer = GetRandom(35, 100);
            backP->minDelay = 65;
            backP->maxDelay = 200;
        }
        else {
            SWSetSpriteFrameTime(spriteP, 250);
            backP->frameTimer = GetRandom(8, 32);
            backP->minDelay = 8;
            backP->maxDelay = 40;
        }
    }
    spriteP->userData = type;
    return spriteP;
}

SpritePtr NewPedestalOrStatueSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(PedestalSpriteRec));
    long frame;

    if (type == kSavePedestalSprite) {
        SWAddSprite(gSavePedestalSpriteLayerP, spriteP);
        SWSetSpriteMoveProc(spriteP, PedestalMoveProc);
    }
    else {
        SWAddSprite(gForegroundSpriteLayerP, spriteP);
    }
    for (frame = 0; frame < spriteP->maxFrames; frame++)
        SWSetFrameCollisionInset(spriteP->frameArray[frame], 0, 5, 2, 0);
    spriteP->userData = type;
    PEDESTAL(spriteP)->isActive = true;
    return spriteP;
}

SpritePtr NewCandleHolderSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(GameSpriteRec));

    SWAddSprite(gCandleHolderSpriteLayerP, spriteP);
    spriteP->userData = type;
    return spriteP;
}

SpritePtr NewTorchSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(TorchSpriteRec));

    SWAddSprite(gTorchSpriteLayerP, spriteP);
    spriteP->userData = type;
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameProc(spriteP, FlameFrameProc);
    TORCH(spriteP)->flameSpriteP = NULL;
    TORCH(spriteP)->glowSpriteP = NULL;
    return spriteP;
}

SpritePtr NewCandleSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(CandleSpriteRec));

    SWAddSprite(gCandleSpriteLayerP, spriteP);
    spriteP->userData = type;
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameProc(spriteP, CandleFrameProc);
    MySetSpriteFrameTime(spriteP, 90);
    CANDLE(spriteP)->glowSpriteP = NULL;
    return spriteP;
}

SpritePtr NewFlameSprite(void)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[0xc1], sizeof(GameSpriteRec));

    SWAddSprite(gFlameSpriteLayerP, spriteP);
    spriteP->userData = kFlameSprite;
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameProc(spriteP, FlameFrameProc);
    MySetSpriteFrameTime(spriteP, 76);
    return spriteP;
}

SpritePtr NewPuffOfSmokeSprite(void)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[kPuffOfSmokeSprite], sizeof(PuffSpriteRec));

    SWAddSprite(gPuffOfSmokeSpriteLayerP, spriteP);
    spriteP->userData = kPuffOfSmokeSprite;
    SWSetSpriteFrameProc(spriteP, PuffOfSmokeFrameProc);
    MySetSpriteFrameTime(spriteP, 100);
    SWResetSpriteFrameTimer(gSpriteWorldP, spriteP);
    PUFF(spriteP)->isDone = false;
    return spriteP;
}

/// 0xce/0xcf big splash (back/front), 0xd0/0xd1 small splash.
SpritePtr NewLavaSplashSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(PuffSpriteRec));

    if (type == 0xce || type == 0xd0)
        SWAddSprite(gBackLavaSplashSpriteLayerP, spriteP);
    else
        SWAddSprite(gFrontLavaSplashSpriteLayerP, spriteP);
    spriteP->userData = type;
    SWSetSpriteFrameProc(spriteP, PuffOfSmokeFrameProc);
    MySetSpriteFrameTime(spriteP, 125);
    SWResetSpriteFrameTimer(gSpriteWorldP, spriteP);
    PUFF(spriteP)->isDone = false;
    return spriteP;
}

SpritePtr NewBloodOrGooDripSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(DripSpriteRec));

    SWAddSprite(gBloodDripSpriteLayerP, spriteP);
    SWSetSpriteMoveProc(spriteP, BloodDripMoveProc);
    MySetSpriteFrameTime(spriteP, 100);
    spriteP->userData = type;
    SWSetRect(&DRIP(spriteP)->collisionInset, 0, 0, 0, 0);
    return spriteP;
}

SpritePtr NewBloodOrGooSplashSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(DripSpriteRec));

    SWAddSprite(gBloodDripSpriteLayerP, spriteP);
    MySetSpriteFrameTime(spriteP, 71);
    SWSetSpriteFrameProc(spriteP, PuffOfSmokeFrameProc);
    SWResetSpriteFrameTimer(gSpriteWorldP, spriteP);
    spriteP->userData = type;
    SWSetRect(&DRIP(spriteP)->collisionInset, 0, 0, 0, 0);
    DRIP(spriteP)->unused104 = false;
    return spriteP;
}

SpritePtr NewSlimeSprite(void)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[kSlimeSprite], sizeof(DripSpriteRec));

    SWAddSprite(gBloodDripSpriteLayerP, spriteP);
    MySetSpriteFrameTime(spriteP, 125);
    SWSetSpriteFrameProc(spriteP, PuffOfSmokeFrameProc);
    SWResetSpriteFrameTimer(gSpriteWorldP, spriteP);
    spriteP->userData = kSlimeSprite;
    SWSetRect(&DRIP(spriteP)->collisionInset, 0, 0, 0, 0);
    DRIP(spriteP)->unused104 = false;
    DRIP(spriteP)->objectID = -1;
    return spriteP;
}

SpritePtr NewStatueSprite(void)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[kStatueSprite], sizeof(StatueSpriteRec));

    SWAddSprite(gStatueSpriteLayerP, spriteP);
    spriteP->userData = kStatueSprite;
    STATUE(spriteP)->circleTimer = 0;
    STATUE(spriteP)->circleFrame = 0;
    STATUE(spriteP)->otherStatueP = NULL;
    STATUE(spriteP)->pedestalP = NULL;
    return spriteP;
}

SpritePtr NewWebTrappedSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(WebTrappedSpriteRec));

    SWAddSprite(gWebTrappedSpriteLayerP, spriteP);
    spriteP->userData = type;
    SWSetSpriteFrameProc(spriteP, type == kWebTrappedHeroSprite ? RandomFlinchHeroFrameProc
                                                                : RandomFlinchEnemyFrameProc);
    SWSetSpriteFrameTime(spriteP, 100);
    SWSetSpriteFrameAdvance(spriteP, 0);
    WEBTRAPPED(spriteP)->dripChance = 2;
    WEBTRAPPED(spriteP)->dripTimer = 0;
    WEBTRAPPED(spriteP)->flinchTimer = 2;
    WEBTRAPPED(spriteP)->flinchCounter = 0;
    return spriteP;
}

SpritePtr NewCircleOfLightSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(CircleOfLightSpriteRec));

    SWSetSpriteMoveProc(spriteP, CircleOfLightMoveProc);
    SWSetSpriteDrawProc(spriteP, gTranslucentMaskDrawProc);
    spriteP->userData = type;
    if (type == kBottomCircleOfLightSprite)
        SWAddSprite(gBottomCircleOfLightSpriteLayerP, spriteP);
    else
        SWAddSprite(gTopCircleOfLightSpriteLayerP, spriteP);
    CIRCLE(spriteP)->statueP = NULL;
    CIRCLE(spriteP)->partnerP = NULL;
    return spriteP;
}

SpritePtr NewTreasureSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(GameSpriteRec));

    if (type > 0xb0 && type < 0xb5)
        SWSetSpriteMoveProc(spriteP, CreateSparklesMoveProc);
    spriteP->userData = type;
    return spriteP;
}

SpritePtr NewExitSprite(void)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[kExitSprite], sizeof(GameSpriteRec));

    SWAddSprite(gExitSpriteLayerP, spriteP);
    spriteP->userData = kExitSprite;
    return spriteP;
}

SpritePtr NewGoldSparkleSprite(void)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[kGoldSparkleSprite], sizeof(PuffSpriteRec));

    spriteP->userData = kGoldSparkleSprite;
    SWSetSpriteFrameProc(spriteP, PuffOfSmokeFrameProc);
    MySetSpriteFrameTime(spriteP, 142);
    SWResetSpriteFrameTimer(gSpriteWorldP, spriteP);
    PUFF(spriteP)->isDone = false;
    PUFF(spriteP)->objectID = -1;
    return spriteP;
}

SpritePtr NewGlowSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(GameSpriteRec));

    SWAddSprite(gGlowSpriteLayerP, spriteP);
    spriteP->userData = type;
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteBlendMode(spriteP, 0x302, 1);
    SWSetSpriteScaledWidthFactor(spriteP, 1.2f);
    SWSetSpriteScaledHeightFactor(spriteP, 1.2f);
    GAME_SPRITE(spriteP)->objectID = -1;
    return spriteP;
}

SpritePtr NewMiscSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(MiscSpriteRec));

    spriteP->userData = type;
    GAME_SPRITE(spriteP)->objectID = -1;
    if (type == kHintQuestionMarkSprite) {
        SWAddSprite(gTorchSpriteLayerP, spriteP);
        SWSetSpriteFrameProc(spriteP, HintQuestionMarkFrameProc);
        SWSetSpriteFrameTime(spriteP, 16);
    }
    return spriteP;
}

SpritePtr NewBatPlatformArrowSprite(short type)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[type], sizeof(MiscSpriteRec));

    spriteP->userData = type;
    GAME_SPRITE(spriteP)->objectID = -1;
    SWSetSpriteFrameAdvanceMode(spriteP, kPatrollingMode);
    SWSetSpriteFrameProc(spriteP, MiscSpriteFrameProc);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameTime(spriteP, 0);
    MISC(spriteP)->frameDelay = 15;
    MISC(spriteP)->frameTimer = 15;
    MISC(spriteP)->frameDirection = 1;
    return spriteP;
}

SpritePtr NewSecretIndicatorSprite(void)
{
    SpritePtr spriteP = CloneGameSprite(gSpriteArray[kSecretIndicatorSprite], sizeof(MiscSpriteRec));

    SWAddSprite(gSecretIndicatorSpriteLayerP, spriteP);
    SWSetSpriteCollideProc(spriteP, SecretSpriteCollideProc);
    SWSetSpriteVisible(spriteP, false);
    spriteP->userData = kSecretIndicatorSprite;
    GAME_SPRITE(spriteP)->objectID = -1;
    return spriteP;
}

/// Torches, candles and the lantern get a glow (candles 0xa1-0xa6 a small one).
void CreateGlowOverSprite(SpritePtr spriteP)
{
    short type = (short)spriteP->userData;
    SpritePtr glowP;

    if (type < 0xa1 || type > 0xa6) {
        glowP = NewGlowSprite(0xca);
        SWCenterSpriteOverSprite(glowP, spriteP);
        MySetSpriteFrameTime(glowP, 76);
        SWSetSpriteFrameProc(glowP, FlameFrameProc);
        if (type == kLanternObjectSprite) {
            OBJECT(spriteP)->glowSpriteP = glowP;
            SWRemoveSprite(glowP);
            SWAddSprite(gLanternGlowSpriteLayerP, glowP);
        }
        else {
            if (type == 0x9e || type == 0x9f || type == 0xa0)
                TORCH(spriteP)->glowSpriteP = glowP;
            SWOffsetSprite(glowP, 0, type == 0x9e ? -26.0f : -20.0f);
        }
    }
    else {
        glowP = NewGlowSprite(type >= 0xa4 ? 0xc9 : 200);
        SWCenterSpriteOverSprite(glowP, spriteP);
        SWOffsetSprite(glowP, 0, type >= 0xa4 ? -6.0f : -12.0f);
        CANDLE(spriteP)->glowSpriteP = glowP;
    }
}

SpritePtr NewSmallDigitsSprite(short numDigits, Boolean showLeadingZeros)
{
    SpritePtr spriteP = CreateStatsSpriteClone(gStatsSpriteArray[0], numDigits, 0, showLeadingZeros);

    if (spriteP == NULL)
        MM_FATAL(-1);
    SWAddSprite(gFPSDisplaySpriteLayerP, spriteP);
    SWSetSpriteMoveProc(spriteP, SmallDigitsMoveProc);
    spriteP->userData = 0;
    return spriteP;
}

SpritePtr NewStatsDigitsSprite(short numDigits, Boolean showLeadingZeros)
{
    SpritePtr spriteP = CreateStatsSpriteClone(gStatsSpriteArray[0], numDigits, 0, showLeadingZeros);

    if (spriteP == NULL)
        MM_FATAL(-1);
    SWAddSprite(gDigitsStatsSpriteLayerP, spriteP);
    SWSetSpriteMoveProc(spriteP, StatsDigitsMoveProc);
    spriteP->userData = 0;
    return spriteP;
}

SpritePtr NewStatsItemSprite(short item)
{
    SpritePtr spriteP;

    SW_ASSERT(item >= 0 && item <= 8);
    spriteP = CloneGameSprite(gStatsSpriteArray[1], sizeof(GameSpriteRec));
    SWAddSprite(gStatsSpriteLayerP, spriteP);
    SWSetCurrentFrameIndex(spriteP, item);
    spriteP->userData = item;
    return spriteP;
}

// ---------------------------------------------------------------------------
// Buttons and text

SpritePtr NewButtonSprite(short spriteIndex, short buttonIndex)
{
    SW_ASSERT(gSpriteArray[spriteIndex] != NULL);
    return CloneButtonSprite(gSpriteArray[spriteIndex], buttonIndex);
}

/// Adds a button from gSpriteArray to a dialog layer at h, v. 0xd9/0xda are the
/// paused-dialog button strips, whose frame is the button index.
SpritePtr AddButtonToDialog(short spriteIndex, short frame, SpriteLayerPtr layerP, short h, short v)
{
    SpritePtr newSpriteP;
    short buttonIndex = spriteIndex;
    Boolean isStrip = spriteIndex == 0xda || spriteIndex == 0xd9;

    if (isStrip)
        buttonIndex = frame;
    newSpriteP = NewButtonSprite(spriteIndex, buttonIndex);
    if (newSpriteP == NULL)
        return NULL;

    SWAddSprite(layerP, newSpriteP);
    SWSetCurrentFrameIndex(newSpriteP, frame);
    if (isStrip) {
        SetUpButtonSprite(newSpriteP, frame, frame + (spriteIndex == 0xda ? 4 : 5), frame, 0);
        SetButtonColor(newSpriteP, 0xffff, 0xffff, 0xffff);
        SetButtonPushedColor(newSpriteP, 0x8fff, 0x8fff, 0x8fff);
        SetButtonPushedOffset(newSpriteP, 1, 1);
    }
    else {
        SetUpButtonSprite(newSpriteP, 0, 1, 1, 1);
        SetButtonColor(newSpriteP, 0xffff, 0xffff, 0xffff);
        SetButtonHighlightedColor(newSpriteP, 0xffff, 0xffff, 0xffff);
        SetButtonPushedColor(newSpriteP, 0xafff, 0xafff, 0xafff);
    }
    InstallButtonProc(newSpriteP, MMStandardButtonProc);
    SWSetSpriteScaledFactor(newSpriteP, (float)(gSpriteWorldP->designWidth / 640) / 1.75f,
                            (float)(gSpriteWorldP->designHeight / 480) / 1.75f);
    SWSetSpriteLocation(newSpriteP, h, v);
    return newSpriteP;
}

short GetButtonHeight(short spriteIndex)
{
    return (short)SWGetSpriteHeight(gSpriteArray[spriteIndex]);
}

short GetButtonWidth(short spriteIndex)
{
    return (short)SWGetSpriteWidth(gSpriteArray[spriteIndex]);
}

/// The font has 116 glyphs but the spacing table 112 entries; the original read past its end
/// for the last four (accented) glyphs. Those now get the default spacing.
static short FontSpacingCount(SpritePtr textP)
{
    long count = textP->maxFrames;
    return (short)(count < (long)SDL_arraysize(fontSpacingArray) ? count : (long)SDL_arraysize(fontSpacingArray));
}

/// A text sprite using the bitmap font (gSpriteArray[220]): A-Z, a-z and digits
/// first, then punctuation, then accented letters.
SpritePtr NewBitmapTextSprite(short maxLineWidth)
{
    static const char kSpecialChars[] = " .,!#$%*()-+/?\"';:`=[]\\@<>^&";
    static const char kUnicodeChars[] =
        "\xc3\xa0\xc3\xa4\xc3\xa5\xc4\x83\xc3\xa2\xc4\x9b\xc3\xa9\xc3\xa8\xc3\xb6\xc3\xb2\xc3\xbc\xc3\xb9\xc3\x9f"
        "\xc2\xa7\xc2\xa3\xc3\xa6\xc3\xae\xc8\x9b\xc3\xa7\xc3\xb0\xc3\xbe\xc3\xac\xc3\xb8\xc2\xb0\xc2\xa8\xc2\xb4";
    SpritePtr textP = CreateBMTextSprite(gSpriteArray[220], maxLineWidth);

    SetBMTextSpriteSpecialChars(textP, 62, kSpecialChars);
    SetBMTextSpriteUnicodeChars(textP, (short)(62 + strlen(kSpecialChars)), kUnicodeChars);
    SetBMTextSpriteCharSpacing(textP, fontSpacingArray, FontSpacingCount(textP), 0.91f);
    SetBMTextSpriteVertOffsetForChars(textP, ".,:;", 0.1f);
    return textP;
}

SpritePtr NewCustomCheckbox(const char *text, float scale, short buttonIndex)
{
    SpritePtr checkboxP, textP;

    checkboxP = CloneButtonSprite(gSpriteArray[221], buttonIndex);
    if (checkboxP == NULL)
        return NULL;
    SetButtonPushedOffset(checkboxP, 1, 1);
    AdjustCollisionRectForOffset(checkboxP);
    InstallButtonProc(checkboxP, MMStandardButtonProc);
    SetUpCheckboxSprite(checkboxP, 0, 1, 2, 3, false);
    SetButtonColor(checkboxP, 0xffff, 0xffff, 0x5fff);
    SetButtonHighlightedColor(checkboxP, 0x5fff, 0xffff, 0xffff);
    SetButtonPushedColor(checkboxP, 0x9fff, 0x9fff, 0x1fff);
    SWSetSpriteScaledFactor(checkboxP, scale, scale);
    textP = NewBitmapTextSprite(640);
    if (textP != NULL) {
        SetBMTextSpriteText(textP, text);
        SWSetSpriteScaledHeight(textP, SWGetSpriteHeight(checkboxP) * 0.8f);
        SWSetSpriteScaledWidthFactor(textP, textP->drawData->scaledHeightFactor);
    }
    SetButtonSpriteText(checkboxP, textP);
    return checkboxP;
}

SpritePtr NewKeyInputBox(const char *text, float scale, short buttonIndex)
{
    SpritePtr boxP, textP;

    boxP = CloneButtonSprite(gSpriteArray[222], buttonIndex);
    if (boxP == NULL)
        return NULL;
    SetButtonPushedOffset(boxP, 1, 1);
    AdjustCollisionRectForOffset(boxP);
    InstallButtonProc(boxP, MMStandardButtonProc);
    SetUpKeyInputBox(boxP, 0, 1);
    SetKeyInputBoxKeysToIgnore(boxP, true, true);
    SetButtonColor(boxP, 0xffff, 0xffff, 0x5fff);
    SetButtonHighlightedColor(boxP, 0x5fff, 0xffff, 0xffff);
    SetButtonPushedColor(boxP, 0x9fff, 0x9fff, 0x1fff);
    SWSetSpriteScaledFactor(boxP, scale * 2.2f, scale);
    textP = NewBitmapTextSprite(0);
    if (textP != NULL) {
        SetBMTextSpriteText(textP, text);
        SetBMTextSpriteCharSpacing(textP, fontSpacingArray, FontSpacingCount(textP), 0.8f);
        SWSetSpriteScaledHeight(textP, SWGetSpriteHeight(boxP) * 0.52f);
        SWSetSpriteScaledWidthFactor(textP, textP->drawData->scaledHeightFactor);
    }
    SetButtonSpriteText(boxP, textP);
    return boxP;
}
