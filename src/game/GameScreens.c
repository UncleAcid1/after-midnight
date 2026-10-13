// GameScreens.c — every screen outside the game itself: the ActionSoft logo, splash, title,
// help, difficulty, organ intro, mansion selection, game over, mansion complete and the ending.
//
// The registration build of the original showed a "Buy" tab, nag dialogs and a teaser screen
// for unregistered copies; those paths are gone and everything behaves as registered.

#include "MMGame.h"
#include "MMProtos.h"
#include "BKGL.h"

// Button indexes on the title screen
enum {
    kTitleNewGame = 0, kTitleOpenGame = 1, kTitleOpenMansion = 2, kTitleHighScores = 3, kTitleHelp = 4,
    kTitleOptions = 5, kTitleQuit = 6, kTitleWebsite = 11, kTitleOpenMovie = 12, kTitleChoosePlayer = 13,
        // added by this version
    kTitleRandomMansion = 20, kTitleTrophyRoom = 21, kTitleMultiplayer = 22
};

/// The title menu, top to bottom: button index and row in the button sheet. Open Mansion (row 2)
/// is shown as "Custom Mansions" (row 7) when this version's sheet is installed.
static const struct { short button, row, extraRow; } kTitleMenu[] = {
    { kTitleNewGame, 0, 0 }, { kTitleMultiplayer, -1, 10 }, { kTitleOpenGame, 1, 1 },
    { kTitleOpenMansion, 2, 7 }, { kTitleRandomMansion, -1, 8 }, { kTitleHighScores, 3, 3 },
    { kTitleTrophyRoom, -1, 9 }, { kTitleHelp, 4, 4 }, { kTitleOptions, 5, 5 }, { kTitleQuit, 6, 6 }
};

#define kBackButton         0x32
#define kLeftArrowButton    0xd6
#define kDoneButton         0xd7
#define kRightArrowButton   0xd8

static SpritePtr grayActionSoftSpriteP, colorActionSoftSpriteP, gLoadingTextSpriteP;
static Boolean gFirstTimeShowingTitleScreen = true;
static Boolean gFirstTimeStartingGame = true;
static Boolean gFinishedScrolling = false;

/// Help screen 2: monsters (sprite type, h, v), zero-terminated.
static const short gMonsterArray[][3] = {
    { 1, 29, 80 }, { 2, 45, 123 }, { 3, 45, 164 }, { 4, 41, 199 }, { 11, 27, 244 }, { 8, 36, 313 },
    { 9, 36, 360 }, { 0, 0, 0 }
};
/// Help screen 3: objects.
static const short gObjectSpritesArray[][3] = {
    { 39, 62, 83 }, { 34, 54, 143 }, { 30, 53, 196 }, { 32, 62, 243 }, { 36, 53, 274 }, { 35, 48, 316 },
    { 37, 44, 374 }, { 0, 0, 0 }
};
/// Mansion buttons on the select screen: mansions 0-4 on the left half, 5-10 on the right.
static const float gMapSpriteLocLeftSide[5][2] = {
    { 129.0f, 102.0f }, { 396.5f, 93.0f }, { 64.0f, 214.5f }, { 391.0f, 295.5f }, { 130.5f, 350.5f }
};
static const float gMapSpriteLocRightSide[6][2] = {
    { 66.5f, 181.5f }, { 138.0f, 315.0f }, { 280.5f, 183.5f }, { 421.5f, 311.0f }, { 116.5f, 70.0f },
    { 392.0f, 81.0f }
};

static SpritePtr gIllustrationSpriteP, gOrganPlayerSpriteP, gOrganLightningSpriteP;
static SpritePtr gNamesSpriteP, gEpisode1TextSpriteP, gEpisode2TextSpriteP;
static short gOldButtonIndex = -1;
static SpritePtr gameOverSpriteP;
static SWRGBColor rgbWhiteColor = { 0xffff, 0xffff, 0xffff };

static SpritePtr gBottomSpriteP;
static float gCurTitleScrollSpeed;
static short gCurrentEndingScreen;
static short gCurrentHelpScreenIndex;
static Boolean gDidChangeToSkeleton;
static long gFadeInButtonsDelay;
static Boolean gFadeOutTitleButtons;
static Boolean gIsFirstFlash;
static Boolean gMusicWasOn;
static long gNextLightningFlash;
static short gNumFlashes;
static SpritePtr gPoemTextSpriteArray[8];
static float gScrollMult;
static Boolean gShowingLeftSide;
static short lightningCount;
static Boolean spriteHasStartedFadingIn;

extern Boolean gTitleScreenSpritesAreValid;            // NewSprite.c
extern SpritePtr gLeftArrowButtonSpriteP, gRightArrowButtonSpriteP;  // HighScores.c

static void SetBackgroundToDesignSize(SpritePtr spriteP)
{
    SWSetSpriteScaledSize(spriteP, gSpriteWorldP->designWidth, gSpriteWorldP->designHeight);
}

static void PlayButtonSound(short newState, short oldState)
{
    if (newState == kButtonPushed)
        MMPlayStereoSound2(NULL, 0x39, 8, 1, gSoundVolume / 3, 1.0, 0);
    else if (oldState == kButtonPushed)
        MMPlayStereoSound2(NULL, 0x39, 8, 1, gSoundVolume / 3, 0.8, 0);
}

static void SetTextStyle(MMTextStyleRec *style, const char *pascalFontName, short styleBits, short size,
                         short justification)
{
    memset(style, 0, sizeof(*style));
    SetPascalString(style->fontName, (ConstStringPtr)pascalFontName);
    style->style = styleBits;
    style->size = size;
    style->unused = true;
    style->dropShadow = true;
    style->justification = justification;
}

// ---------------------------------------------------------------------------
// ActionSoft logo

void SetUpActionSoftScreen(void)
{
    SWError err;

    SDL_assert(gSpriteWorldP != NULL);
    err = CreateSpriteFromPictResource(gSpriteWorldP, &grayActionSoftSpriteP, NULL, 0x152c, 0x152c, 1, kNoMask);
    if (err != kNoError)
        MM_FATAL(err);
    err = CreateSpriteFromPictResource(gSpriteWorldP, &colorActionSoftSpriteP, NULL, 0x1536, 0x1536, 1, kNoMask);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteTranslucencyLevel(colorActionSoftSpriteP, 0);
    SWSetSpriteTranslucencyLevel(grayActionSoftSpriteP, 0);
    colorActionSoftSpriteP->userData = 100;    // frames to wait before fading in the color logo
    SWAddSprite(gTopSpriteLayerP, grayActionSoftSpriteP);
    SWAddSprite(gTopSpriteLayerP, colorActionSoftSpriteP);
    SWCenterSpriteInRect(colorActionSoftSpriteP, &gSpriteWorldP->workAreaRect);
    SWSetSpriteLocation(grayActionSoftSpriteP, colorActionSoftSpriteP->destFrameRect.left,
                        colorActionSoftSpriteP->destFrameRect.top);
    grayActionSoftSpriteP->userData = 0;
    SWSetSpriteFrameProc(grayActionSoftSpriteP, ActionSoftLogoGrayFrameProc);
    SWSetSpriteFrameTime(grayActionSoftSpriteP, 16);
    grayActionSoftSpriteP->misc1.n = 1;
    SWSetSpriteFrameProc(colorActionSoftSpriteP, ActionSoftLogoFrameProc);
    SWSetSpriteFrameTime(colorActionSoftSpriteP, 16);
    colorActionSoftSpriteP->misc1.n = 1;
    SWUpdateSpriteWorld(gSpriteWorldP, true);
}

/// misc1 is 1 while the logo is still changing; userData counts frames.
void DoActionSoftScreen(void)
{
    Boolean done = false;

    SDL_assert(colorActionSoftSpriteP != NULL);
    while (!done) {
        GiveOSTime();
        SWProcessSpriteWorld(gSpriteWorldP);
        SWAnimateSpriteWorld(gSpriteWorldP);
        if (gSpriteWorldP->frameHasOccurred)
            ProcessSDLEvents();
        if (colorActionSoftSpriteP->misc1.n == 0 && colorActionSoftSpriteP->userData > 40) {
            colorActionSoftSpriteP->misc1.n = 1;
            SWSetSpriteFrameProc(colorActionSoftSpriteP, ActionSoftLogoFadeOutFrameProc);
            SWRemoveSpriteFromAnimation(grayActionSoftSpriteP, true);
        }
        if (colorActionSoftSpriteP->misc1.n == 0 && colorActionSoftSpriteP->userData == -1)
            done = true;
        if (WasMouseClicked() || WasAKeyPressed() || WasGameQuit())
            done = true;
    }
    AnimateOneFrame(gSpriteWorldP);
}

void ActionSoftLogoFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    unsigned long level;

    (void)curFrameP;
    (void)frameIndex;
    if (spriteP->misc1.n == 0)
        spriteP->userData++;
    else if (spriteP->userData < 1) {
        level = SWGetSpriteTranslucencyLevel(spriteP);
        if (level < 0xfda7) {
            SWSetSpriteTranslucencyLevel(spriteP, level + 600);
            SWInvalidateSprite(spriteP);
        }
        else {
            spriteP->misc1.n = 0;
            SWSetSpriteTranslucencyLevel(spriteP, 0xffff);
        }
    }
    else
        spriteP->userData--;
}

void ActionSoftLogoFadeOutFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    unsigned long level = SWGetSpriteTranslucencyLevel(spriteP);

    (void)curFrameP;
    (void)frameIndex;
    if (level < 0x4b1) {
        spriteP->misc1.n = 0;
        SWSetSpriteTranslucencyLevel(spriteP, 0);
        spriteP->userData = -1;
    }
    else {
        SWSetSpriteTranslucencyLevel(spriteP, level - 0x4b0);
        SWInvalidateSprite(spriteP);
    }
}

void ActionSoftLogoGrayFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    unsigned long level = SWGetSpriteTranslucencyLevel(spriteP);

    (void)curFrameP;
    (void)frameIndex;
    if (level < 0xfbb3) {
        SWSetSpriteTranslucencyLevel(spriteP, level + 0x44c);
        SWInvalidateSprite(spriteP);
    }
    else {
        SWSetSpriteFrameProc(spriteP, NULL);
        spriteP->misc1.n = 0;
    }
}

// ---------------------------------------------------------------------------
// Splash screen shown while loading

void DisplaySplashScreen(void)
{
    SpritePtr backgroundP;
    SWError err;

    SDL_assert(gSpriteWorldP != NULL);
    err = LoadAndInstallBackgroundSprite(&backgroundP, GetScreensFolderPath(), "splash.jpg", -1, false);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledSize(backgroundP, 640, 480);
    err = CreateSpriteFromPictResource(gSpriteWorldP, &gLoadingTextSpriteP, NULL, 0x15e0, 0x15e0, 3, 4);
    if (err != kNoError)
        MM_FATAL(err);
    MySetSpriteScaledFactor(gLoadingTextSpriteP, 0.5f, 0.5f);
    SWLockSprite(gLoadingTextSpriteP);
    SWAddSprite(gTopSpriteLayerP, gLoadingTextSpriteP);
    SWCenterSpriteInRect(gLoadingTextSpriteP, &gSpriteWorldP->workAreaRect);
    SWMoveSprite(gLoadingTextSpriteP, SWGetSpriteHorizLoc(gLoadingTextSpriteP),
                 (float)(gSpriteWorldP->workAreaRect.bottom - 25));
    SWProcessSpriteWorld(gSpriteWorldP);
    SWUpdateSpriteWorld(gSpriteWorldP, true);
}

void ChangeSplashScreenLoadingText(short frame)
{
    SDL_assert(gLoadingTextSpriteP != NULL);
    SWSetCurrentFrameIndex(gLoadingTextSpriteP, frame);
    AnimateOneFrame(gSpriteWorldP);
}

// ---------------------------------------------------------------------------
// Title screen (designed at 1280x960; the night sky scrolls down to the mansion)

void SetUpTitleScreen(void)
{
    SetUpTitleScreenMM1();
    UpdateMenuBar();
}

/// Left of centre as in the original, or centred ("Centered main menu" in Extras).
static void PositionTitleButton(SpritePtr buttonP, short top)
{
    SWRect rect = buttonP->destFrameRect;

    SWCenterRect(&rect, &gSpriteWorldP->workAreaRect);
    SWOffsetRect(&rect, gExtras.arcadeMenu ? 0 : -310, top - rect.top);
    SWSetSpriteLocation(buttonP, rect.left, rect.top);
}

static Boolean IsTitleMenuButton(SpritePtr spriteP)
{
    short index;
    size_t i;

    if (spriteP->userData != kButtonSpriteSignature)
        return false;
    index = GetButtonIndex(spriteP);
    for (i = 0; i < SDL_arraysize(kTitleMenu); i++)
        if (kTitleMenu[i].button == index)
            return true;
    return false;
}

/// Places the menu buttons (again, after the Extras setting changes). With this version's
/// buttons there are ten, so they are a little smaller than the original seven.
void LayOutTitleScreenButtons(void)
{
    SpritePtr spriteP;
    short top = gTitleScreenHasExtraButtons ? 262 : 284, step = gTitleScreenHasExtraButtons ? 61 : 80;

    if (gTopSpriteLayerP == NULL)
        return;
    for (spriteP = SWGetNextSprite(gTopSpriteLayerP, NULL); spriteP != NULL;
         spriteP = SWGetNextSprite(gTopSpriteLayerP, spriteP)) {
        if (!IsTitleMenuButton(spriteP))
            continue;
        PositionTitleButton(spriteP, top);
        top = (short)(top + step);
    }
}

/// Adds rows of clouds that drift left and wrap around.
static void AddCloudRows(short firstRow, short lastRow, short rowSpacing, short rowBase, float speedFactor,
                         Boolean title)
{
    SpritePtr cloudP;
    short row;
    long h, index;
    float delta;

    for (row = firstRow; row <= lastRow; row++) {
        h = 0;
        index = row * 2;
        if (!title && index > 6)
            index = 0;
        while (h < gSpriteWorldP->workAreaRect.right) {
            cloudP = NewTitleScreenSprite((short)(index / 2 + 8));
            if (cloudP == NULL)
                break;
            SWRemoveSprite(cloudP);
            SWAddSprite(gExitSpriteLayerP, cloudP);
            if ((index & 1) == 0)
                SWSetSpriteFlipX(cloudP, true);
            SWSetSpriteLocation(cloudP, (float)h, (float)(row * rowSpacing + rowBase) - SWGetSpriteHeight(cloudP));
            SWSetSpriteMoveBounds(cloudP, &gSpriteWorldP->workAreaRect);
            SWSetSpriteMoveProc(cloudP, SWWrapSpriteHorizMoveProc);
            if (title)
                delta = (float)-row / 4.0f + 0.13f;
            else
                delta = -((float)row * 0.05f + 0.03f) * speedFactor;
            SWSetSpriteMoveDelta(cloudP, delta, 0.0f);
            SWSetSpriteFrameProc(cloudP, ResetSpriteFrameProc);
            cloudP->miscFloat = 0.55f;
            if (!title)
                SWSetSpriteScaledFactor(cloudP, 1.0f, 1.0f);
            h = (long)((float)(title ? GetRandom(80, 160) : GetRandom(-20, 80)) + SWGetSpriteWidth(cloudP) + (float)h);
            if (++index > 6)
                index = 0;
        }
    }
}

void SetUpTitleScreenMM1(void)
{
    SpritePtr spriteP, logoP;
    SWRect titleRect, rect;
    short i;
    long f;
    SWError err;

    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
    SWSetDesignResolution(gSpriteWorldP, 1280, 960);
    SWSetRect(&titleRect, 0, 0, 1280, 960);
    DrawTitleScreenBackground(&rect);
    GiveOSTime();
    gCurrentGameMode = kNoGameMode;
    LoadTitleScreenSprites();
    for (f = 0; f < gTitleScreenSpriteArray[0]->numFrames; f++)
        SWSetFrameCollisionInset(gTitleScreenSpriteArray[0]->frameArray[f], 30, 8, 22, 22);

    // Menu buttons
    for (i = 0; i < (short)SDL_arraysize(kTitleMenu); i++) {
        short row = gTitleScreenHasExtraButtons ? kTitleMenu[i].extraRow : kTitleMenu[i].row;
        if (row < 0)
            continue;
        spriteP = NewTitleScreenButtonSprite(0, kTitleMenu[i].button);
        if (spriteP == NULL) {
            MM_FATAL(-1);
            continue;
        }
        SWSetCurrentFrameIndex(spriteP, row * 2);
        SetUpButtonSprite(spriteP, row * 2, row * 2 + 1, row * 2, 0);
        InstallButtonProc(spriteP, MMTitleScreenButtonProc);
        SetButtonPushedOffset(spriteP, 0, 0);
        if (gTitleScreenHasExtraButtons)
            SWSetSpriteScaledSize(spriteP, 335, 61);
        else
            SWSetSpriteScaledSize(spriteP, 440, 80);
        MakeTitleScreenSpriteFadeIn(spriteP);
    }
    LayOutTitleScreenButtons();

    // Tab along the bottom: Choose Player. (The original also had a "More Games" tab linking
    // to the ActionSoft website beside it.)
    spriteP = NewTitleScreenButtonSprite(2, kTitleChoosePlayer);
    if (spriteP == NULL)
        MM_FATAL(-1);
    else {
        SWSetCurrentFrameIndex(spriteP, 6);
        SetUpButtonSprite(spriteP, 6, 7, 6, 0);
        InstallButtonProc(spriteP, MMTabButtonProc);
        SetButtonPushedOffset(spriteP, 0, 0);
        rect = spriteP->destFrameRect;
        SWOffsetRect(&rect, 0, (short)(960.0f - SWGetSpriteHeight(spriteP)));
        SWSetSpriteLocation(spriteP, rect.left, rect.top);
        MakeTitleScreenSpriteFadeIn(spriteP);
    }

    // Logo, with Jack's head beside it
    logoP = NewTitleScreenSprite(1);
    if (logoP != NULL) {
        rect = logoP->destFrameRect;
        SWCenterRect(&rect, &titleRect);
        SWOffsetRect(&rect, 0, 80 - rect.top);
        SWSetSpriteLocation(logoP, rect.left, rect.top);
        MakeTitleScreenSpriteFadeIn(logoP);
        rect.bottom = logoP->destFrameRect.bottom;
        spriteP = NewMiscSprite(0xe2);
        if (spriteP != NULL) {
            SWSetCurrentFrameIndex(spriteP, 2);
            SWAddSprite(gHeroSpriteLayerP, spriteP);
            SWSetSpriteLocation(spriteP, ((float)rect.right - SWGetSpriteWidth(spriteP)) - 13.0f,
                                (float)(rect.bottom - 40));
            MakeTitleScreenSpriteFadeIn(spriteP);
        }
    }

    // The mansion and hill at the bottom; everything with miscFloat scrolls during the intro.
    gBottomSpriteP = NewTitleScreenSprite(4);
    if (gBottomSpriteP != NULL) {
        SWRemoveSprite(gBottomSpriteP);
        SWAddSprite(gBottomSpriteLayerP, gBottomSpriteP);
        SWSetSpriteLocation(gBottomSpriteP, 0.0f, gSpriteWorldP->designHeight - SWGetSpriteHeight(gBottomSpriteP));
        gBottomSpriteP->miscFloat = 1.0f;
    }
    spriteP = NewTitleScreenSprite(5);
    if (spriteP != NULL && gBottomSpriteP != NULL) {
        SWRemoveSprite(spriteP);
        SWAddSprite(gBackgroundSpriteLayerP, spriteP);
        SWSetSpriteLocation(spriteP,
                            (SWGetSpriteHorizLoc(gBottomSpriteP) + SWGetSpriteWidth(gBottomSpriteP)) -
                                SWGetSpriteWidth(spriteP),
                            (SWGetSpriteVertLoc(gBottomSpriteP) - SWGetSpriteHeight(spriteP)) + 61.0f);
        spriteP->miscFloat = 1.0f;
    }
    for (i = 0; i < 2; i++) {
        spriteP = NewTitleScreenSprite(6);     // fog, two copies side by side
        if (spriteP != NULL) {
            SWSetSpriteLocation(spriteP, i == 0 ? 0.0f : SWGetSpriteWidth(spriteP),
                                gSpriteWorldP->designHeight - SWGetSpriteHeight(spriteP));
            SWSetSpriteMoveDelta(spriteP, -1.0f, 0.0f);
            SWSetSpriteMoveBounds(spriteP, &gSpriteWorldP->workAreaRect);
            SWSetSpriteMoveProc(spriteP, SWWrapSpriteHorizMoveProc);
            spriteP->miscFloat = 1.0f;
        }
    }
    spriteP = NewTitleScreenSprite(7);         // moon
    if (spriteP != NULL) {
        SWRemoveSprite(spriteP);
        SWAddSprite(gBottomSpriteLayerP, spriteP);
        SWSetSpriteLocation(spriteP, 179.0f, 164.0f);
        spriteP->miscFloat = 0.55f;
    }
    spriteP = NewTitleScreenSprite(0xc);
    if (spriteP != NULL) {
        SWRemoveSprite(spriteP);
        SWAddSprite(gBottomSpriteLayerP, spriteP);
        SWSetSpriteFrameTime(spriteP, 100);
        SWSetSpriteFrameAdvanceMode(spriteP, kPatrollingMode);
        SWSetSpriteLocation(spriteP, 640.0f, 759.0f);
        spriteP->miscFloat = 1.0f;
    }
    spriteP = NewTitleScreenSprite(0xd);
    if (spriteP != NULL) {
        SWRemoveSprite(spriteP);
        SWAddSprite(gBackgroundSpriteLayerP, spriteP);
        SWSetSpriteFrameTime(spriteP, 0x8e);
        SWSetSpriteFrameAdvanceMode(spriteP, kPatrollingMode);
        SWSetSpriteLocation(spriteP, 989.0f, -715.0f);
        spriteP->miscFloat = 1.0f;
    }
    AddCloudRows(1, 3, 13, 380, 1.0f, true);

    MMShieldCursor();
    MMSetCursorForGame();
    UpdateMenuBar();
    SWSetSpriteWorldMaxFPS(gSpriteWorldP, 60);
    gCurrentGameMode = kTitleScreenMode;
    gSpriteWorldIsValid = true;
    if (SWStickyError() != kNoError)
        MM_FATAL(SWStickyError());

    if (!gFirstTimeShowingTitleScreen || gOpeningMusicRefNum != NULL)
        PlayCricketSound();
    else {
        StartMusicByFadingIn("Opening Music", 12000, gTitleMusicVolume);
        gOpeningMusicRefNum = GetCurrentMusicReference();
    }
    if (gFirstTimeShowingTitleScreen) {
        gFinishedScrolling = false;
        gCurTitleScrollSpeed = 0;
        gFadeInButtonsDelay = 0;
        gScrollMult = 1.0f;
        ScrollAllSpriteLayers(gSpriteWorldP, 0, 980.0f);
        MMPlaySound(0x4b, 0, 1);
    }
    gNumFlashes = (short)GetRandom(1, 3);
    gNextLightningFlash = GetRandom(30, 120);
    gIsFirstFlash = true;
    gFadeOutTitleButtons = false;
    gFirstTimeShowingTitleScreen = false;
    gDoOpeningMusic = false;
    DoGameFadeToNormalFullAnim(30);
}

void SetUpTitleScreenMM2(void)
{
}

void PlayCricketSound(void)
{
    if (FindSound(0x4c) == -1)
        MMPlayStereoSound2(NULL, 0x4c, 6, 2, 0xff, 1.0, -1);
}

/// Moves every sprite vertically by v times its parallax factor (miscFloat).
void ScrollAllSpriteLayers(SpriteWorldPtr spriteWorldP, SWMovementType h, float v)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP;

    for (layerP = spriteWorldP->headSpriteLayerP; layerP != NULL; layerP = SWGetNextSpriteLayer(spriteWorldP, layerP))
        for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP))
            SWOffsetSprite(spriteP, h, spriteP->miscFloat * v);
}

/// Fades to red while zooming in (starting a new game for the first time).
void DoFadeToRedAndZoom(short numFrames)
{
    SpritePtr redP, blackP;
    unsigned long level;
    float translucency;

    if (SWGetNextSprite(gFadeToBlackSpriteLayerP, NULL) != NULL)
        return;

    redP = NewFadeSprite(gSpriteWorldP, numFrames);
    if (redP != NULL) {
        SWSetSpriteColor(redP, 0xffff, 0, 0);
        SWSetSpriteBlendMode(redP, GL_SRC_ALPHA, GL_ONE);
    }
    blackP = NewFadeSprite(gSpriteWorldP, numFrames);
    if (blackP != NULL)
        SWSetSpriteMoveProc(blackP, NULL);

    while (IsCurrentlyFadingToBlack() && !gQuitGame) {
        SWProcessSpriteWorld(gSpriteWorldP);
        if (IsMouseDown()) {
            // Speed up
            if (redP != NULL)
                redP->userData = 0xffff / (numFrames / 8);
            if (blackP != NULL)
                blackP->userData = 0xffff / (numFrames / 8);
        }
        if (!gSpriteWorldP->frameHasOccurred)
            KeepMusicPlaying();
        else {
            if (redP != NULL)
                redP->userData++;
            if (blackP != NULL)
                blackP->userData++;
            ProcessSDLEvents();
            SWSetSpriteWorldZoom(gSpriteWorldP, gSpriteWorldP->zoomH * 1.0009f, gSpriteWorldP->zoomV * 1.0009f);
            gSpriteWorldP->spriteOffsetH += 1.0f;
            gSpriteWorldP->spriteOffsetV += 0.5f;
            redP = SWGetNextSprite(gFadeToBlackSpriteLayerP, NULL);
            if (redP != NULL) {
                level = SWGetSpriteTranslucencyLevel(redP);
                if (level > 0x8fff && blackP != NULL) {
                    // Past halfway, fade the black sprite in too.
                    SWSetSpriteMoveProc(blackP, FadeToBlackMoveProc);
                    translucency = ((float)(level - 0x8fff) / 28672.0f) * 65535.0f;
                    if (translucency <= 0.0f)
                        translucency = 0.0f;
                    SWSetSpriteTranslucencyLevel(blackP, (unsigned long)translucency);
                }
            }
        }
        SWAnimateSpriteWorld(gSpriteWorldP);
    }
    if (redP != NULL) {
        SWRemoveSprite(redP);
        SWDisposeSprite(&redP);
    }
    SWSetSpriteWorldZoom(gSpriteWorldP, 1.0f, 1.0f);
    gSpriteWorldP->spriteOffsetH = 0;
    gSpriteWorldP->spriteOffsetV = 0;
}

void FadeInTitleTextFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    unsigned long level = SWGetSpriteTranslucencyLevel(spriteP);

    (void)curFrameP;
    (void)frameIndex;
    if (!gFadeOutTitleButtons) {
        if (gFadeInButtonsDelay > 20)
            SWSetSpriteTranslucencyLevel(spriteP, level < 0xfe0b ? level + 500 : 0xffff);
    }
    else if (level < 0x8fd) {
        SWSetSpriteFrameProc(spriteP, NULL);
        SWSetSpriteFrameTime(spriteP, -1);
        SWSetSpriteTranslucencyLevel(spriteP, 0);
    }
    else
        SWSetSpriteTranslucencyLevel(spriteP, level - 0x8fc);
}

/// Pulsing glow behind a tab (only used by the unregistered "Buy" tab).
void TabGlowPulseFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    SpritePtr tabP = spriteP->misc2.sprite;
    unsigned long level;

    (void)curFrameP;
    (void)frameIndex;
    SDL_assert(tabP->misc2.sprite == spriteP);
    if (!gFadeOutTitleButtons) {
        if (SWGetSpriteTranslucencyLevel(tabP) != 0) {
            level = SWGetSpriteTranslucencyLevel(spriteP);
            if (spriteP->misc1.n < 0) {
                level -= 800;
                if (level < 0x1fff)
                    spriteP->misc1.n = 1;
            }
            else {
                level += 800;
                if (level > 0x9ffe)
                    spriteP->misc1.n = -1;
            }
            SWSetSpriteTranslucencyLevel(spriteP, level);
        }
    }
    else {
        level = SWGetSpriteTranslucencyLevel(spriteP);
        if (level < 0x8fd) {
            SWSetSpriteFrameProc(spriteP, NULL);
            SWSetSpriteFrameTime(spriteP, -1);
            SWSetSpriteTranslucencyLevel(spriteP, 0);
        }
        else
            SWSetSpriteTranslucencyLevel(spriteP, level - 0x8fc);
    }
}

void DrawTitleScreenBackground(SWRect *rectP)
{
    DrawTitleScreenBackgroundMM1(rectP);
}

static void TitleScreenEditionAndCredit(float scale);

/// Star background plus "Welcome back" and the version number.
void DrawTitleScreenBackgroundMM1(SWRect *rectP)
{
    SpritePtr spriteP;
    MMTextStyleRec style;
    SWRGBColor gray = { 0xc000, 0xc000, 0xc000 };
    char playerName[256], message[255];
    float scale = (float)(gSpriteWorldP->designWidth / 640);
    SWError err;

    err = LoadAndInstallBackgroundSprite(&spriteP, GetTitleScreenFolderPath(NULL), "starBackground.jpg", -1, false);
    if (err != kNoError)
        MM_FATAL(err);
    SetBackgroundToDesignSize(spriteP);

    SetTextStyle(&style, "\012Action Man", 0, (short)(scale * 16.0f), 0);
        // beside the Change Player tab (a long name gets smaller rather than run into the
        // credit in the other corner)
    *rectP = gSpriteWorldP->workAreaRect;
    rectP->top = rectP->bottom - 30;
    rectP->left += 290;
    rectP->right -= (short)((rectP->right - rectP->left) * 0.42f);
    SWOffsetRect(rectP, 0, -16);
    if (gCurrentPlayerNameExists) {
        SWCopyPascalStringToCString((ConstStringPtr)gCurrentPlayerName, playerName);
        snprintf(message, sizeof(message), "Welcome back, %s", playerName);
        spriteP = MyDrawCTextJustified(message, &style, rectP, &gray);
        MakeTitleScreenSpriteFadeIn(spriteP);
        SWOffsetRect(rectP, 0, 30);
    }

        // (the original showed its version number here)
        // small, in the top right corner, a little in from the edges
    SetTextStyle(&style, "\012Action Man", 0, (short)(scale * 9.0f), 1);
    *rectP = gSpriteWorldP->workAreaRect;
    rectP->right -= (short)(scale * 10.0f);
    rectP->left = rectP->right - (short)(scale * 300.0f);
    rectP->top += (short)(scale * 8.0f);
    rectP->bottom = rectP->top + (short)(scale * 10.0f);
    spriteP = MyDrawCTextJustified("After Midnight - An UncleAcid1 Project", &style, rectP, &gray);
    MakeTitleScreenSpriteFadeIn(spriteP);

    TitleScreenEditionAndCredit(scale);
}

/// This edition's name under the logo, and whose game it is, in the corner (added by this version).
static void TitleScreenEditionAndCredit(float scale)
{
    SWRect area = gSpriteWorldP->workAreaRect, r;
    float w = (float)(area.right - area.left), h = (float)(area.bottom - area.top);
    SWRGBColor gold = { 0xf2f2, 0xdcdc, 0x8c8c }, gray = { 0xa8a8, 0xa8a8, 0xa8a8 };
    MMTextStyleRec style;
    SpritePtr spriteP;

    SetTextStyle(&style, "\015BlackChancery", 0, (short)(scale * 24.0f), 1);
    r.right = (short)(area.left + w * 0.942f);
    r.left = (short)(r.right - w * 0.5f);
    r.bottom = (short)(area.top + h * 0.298f);
    r.top = (short)(r.bottom - scale * 30.0f);
    spriteP = MyDrawCTextJustified("After Midnight", &style, &r, &gold);
    if (spriteP != NULL) {      // with the logo, under dialogs (the text layer is above them)
        SWRemoveSprite(spriteP);
        SWAddSprite(gHeroSpriteLayerP, spriteP);
    }
    MakeTitleScreenSpriteFadeIn(spriteP);

    SetTextStyle(&style, "\012Action Man", 0, (short)(scale * 10.0f), 1);
    r.right = (short)(area.right - scale * 8.0f);
    r.left = (short)(r.right - w * 0.4f);
    r.bottom = (short)(area.bottom - scale * 6.0f);
    r.top = (short)(r.bottom - scale * 16.0f);
    spriteP = MyDrawCTextJustified("Midnight Mansion created by Vern Jensen", &style, &r, &gray);
    MakeTitleScreenSpriteFadeIn(spriteP);
}

void MakeTitleScreenSpriteFadeIn(SpritePtr spriteP)
{
    if (spriteP == NULL)
        return;
    SWSetSpriteFrameTime(spriteP, 16);
    SWSetSpriteFrameProc(spriteP, FadeInTitleTextFrameProc);
    SWSetSpriteFrameAdvance(spriteP, 0);
    if (gFirstTimeShowingTitleScreen)
        SWSetSpriteTranslucencyLevel(spriteP, 0);
}

/// Shows "Resume your suspended game?" and resumes it. Returns the dialog choice (3: start anew).
static short OfferSuspendedGame(short button)
{
    char fileName[256], path[500], message[256];
    Str255 pascalName;
    short choice;
    int err;

    if (!gSuspendedGameExists || button == kTitleOpenMansion)
        return 3;

    GetCStringFromHFSUniStr(fileName, gSuspendedGameFilenameHFSUni);
    GetSuspendedGameFilePath(path, fileName);
    GetPascalStringFromHFSUniStr(pascalName, gSuspendedGameFilenameHFSUni);
    StripStringFromEndOfString((ConstStringPtr)"\012 Suspended", pascalName, pascalName);
    SWCopyPascalStringToCString(pascalName, fileName);
    snprintf(message, sizeof(message), "Resume your suspended game in %s?", fileName);
    choice = MMDoCustomDialog(gSpriteWorldP, message, 0);
    if (choice == 3)
        DeleteSuspendedGame(path);
    else if (choice == 2) {
        MMHideCursor();
        DoGameFadeToBlackFullAnim(30, true);
        ExitTitleScreen();
        err = OpenSuspendedGame(path);
        if (err == 0)
            DeleteSuspendedGame(path);
        else {
            CleanUpAfterGame();
            SetUpTitleScreen();
            if (err != -2)
                MMDoErrorMessage("Sorry, but there was an error when opening the suspended game. (Error code %d).",
                                 err);
        }
    }
    return choice;
}

static void OpenSavedGameFromTitleScreen(void)
{
    char path[1024], fileName[256];
    int err;

    err = MMDoOpenGameDialog(GetSavedGamesFilePath(NULL, false), path, fileName);
    if (err != 0) {
        DoGameFadeToNormal(1);
        return;
    }
    MMHideCursor();
    DoGameFadeToBlackFullAnim(30, true);
    ExitTitleScreen();
    err = MMRestoreFromSaveGameFile(path, fileName);
    if (err != 0) {
        CleanUpAfterGame();
        SetUpTitleScreen();
        if (err == -11223)
            MMDoCustomDialog(gSpriteWorldP,
                "This saved game was created in a mansion that you can't play until you've registered.", 3);
        else if (err != -11111 && err != -22222)
            MMDoErrorMessage("Sorry, but there was an error when trying to open the saved game (error code %d).",
                             err);
    }
}

/// Lightning lights up the clouds now and then (title and multiplayer screens).
void TitleScreenLightning(void)
{
    SpritePtr cloudP;

    if (gNextLightningFlash-- < 0) {
        for (cloudP = gExitSpriteLayerP->headSpriteP; cloudP != NULL; cloudP = SWGetNextSprite(gExitSpriteLayerP, cloudP)) {
            SWSetCurrentFrameIndex(cloudP, 1);
            SWSetSpriteFrameTime(cloudP, 0xa6);
        }
        if (FindSound(0x3a) == -1 && gIsFirstFlash && GetRandom(1, 2) == 1)
            MMPlaySound(0x3a, 0, 1);
        if (gNumFlashes-- < 1) {
            gNextLightningFlash = GetRandom(24, 240);
            gNumFlashes = (short)GetRandom(2, 4);
            gIsFirstFlash = true;
        }
        else {
            gNextLightningFlash = GetRandom(7, 10);
            gIsFirstFlash = false;
        }
    }
}

/// The multiplayer screen: fades the title menu (buttons, logo, texts) out, or back in.
void TitleScreenFadeMenu(Boolean out)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP;

    if (out) {
        gFadeOutTitleButtons = true;
        return;
    }
    gFadeOutTitleButtons = false;
    gFadeInButtonsDelay = 21;
    for (layerP = gSpriteWorldP->headSpriteLayerP; layerP != NULL; layerP = SWGetNextSpriteLayer(gSpriteWorldP, layerP))
        for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP))
            if (spriteP->miscFloat == 0.0f && SWGetSpriteTranslucencyLevel(spriteP) == 0 &&
                (layerP == gTopSpriteLayerP || layerP == gTextSpriteLayerP || layerP == gHeroSpriteLayerP ||
                 layerP == gBottomSpriteLayerP || layerP == gBackgroundSpriteLayerP)) {
                SWSetSpriteFrameTime(spriteP, 16);
                SWSetSpriteFrameProc(spriteP, FadeInTitleTextFrameProc);
                SWSetSpriteFrameAdvance(spriteP, 0);
            }
}

void HandleTitleScreenCallback(void)
{
    SWPoint mouse;
    Boolean mouseDown, haveProfile = false;
    Str255 newName;
    SWRect rect;
    float distance;
    short button, choice;

    SWProcessSpriteWorld(gSpriteWorldP);
    SWAnimateSpriteWorld(gSpriteWorldP);
    KeepMusicPlaying();
    if (!gSpriteWorldP->frameHasOccurred)
        return;

    TitleScreenLightning();

    if (!gFinishedScrolling) {
        // The opening scroll down from the sky; eases out near the bottom.
        ScrollAllSpriteLayers(gSpriteWorldP, 0, -gCurTitleScrollSpeed);
        distance = (SWGetSpriteVertLoc(gBottomSpriteP) + SWGetSpriteHeight(gBottomSpriteP)) -
                   (float)gSpriteWorldP->workAreaRect.bottom;
        if (distance > 490.0f)
            gCurTitleScrollSpeed += gScrollMult * 0.04f;
        else
            gCurTitleScrollSpeed += gScrollMult * -0.04f;
        if (gCurTitleScrollSpeed < 0.1f)
            gCurTitleScrollSpeed = 0.1f;
        if (distance <= 0.0f) {
            ScrollAllSpriteLayers(gSpriteWorldP, 0, -distance);
            gFinishedScrolling = true;
        }
        if (IsMouseDown() && gScrollMult == 1.0f) {
            gScrollMult = 1.0f;
            gCurTitleScrollSpeed = 50.0f;
            gFadeInButtonsDelay += 5000;
        }
        return;
    }

    gFadeInButtonsDelay++;
    mouseDown = MMGetMouse(&mouse);
    SWConvertCoordinates(gSpriteWorldP, &mouse);
    button = ProcessButtonSprites(gTopSpriteLayerP, gTopSpriteLayerP, mouse, mouseDown, false, NULL);
    if (button == -1)
        return;
    AnimateOneFrame(gSpriteWorldP);

    switch (button) {
        case kTitleNewGame:
        case kTitleOpenGame:
        case kTitleOpenMansion:
        case kTitleRandomMansion:
        case kTitleMultiplayer:
            // A player profile is needed first.
            if (!gCurrentPlayerNameExists) {
                if (CountNumProfilesOnDisk() < 1) {
                    haveProfile = DoEnterNameForNewProfileDialog(newName);
                    if (haveProfile)
                        CreateNewProfile(newName);
                }
                else if (DoChoosePlayerDialog()) {
                    DrawTitleScreenBackground(&rect);
                    SWUpdateSpriteWorld(gSpriteWorldP, true);
                    haveProfile = true;
                }
            }
            else
                haveProfile = true;
            if (!haveProfile) {
                printf("No profile!");
                break;
            }
            if (button != kTitleMultiplayer)
                SetSinglePlayer();
            choice = OfferSuspendedGame(button);
            if (choice != 3)
                break;
            if (button == kTitleMultiplayer && getenv("MM_OLD_MULTIPLAYER_DIALOG") == NULL) {
                EnterMultiplayerScreen();       // the sky above the mansion (MMMultiplayerScreen.c)
                break;
            }
            if (button == kTitleMultiplayer) {
                choice = DoMultiplayerSetupDialog();
                if (choice == kMultiplayerNoMansion)
                    ShowMissingMansionMessage();
                if (choice == kMultiplayerCancelled || choice == kMultiplayerNoMansion)
                    break;
                if (choice == kMultiplayerJoined) {     // the other computer picked the mansion
                    StartCustomMansion();
                    break;
                }
                button = kTitleNewGame;
            }
            if (button == kTitleNewGame) {
                MMHideCursor();
                if (!gFirstTimeStartingGame || AreAnyMansionsBeaten())
                    DoGameFadeToBlackFullAnim(30, false);
                else {
                    ExtrasPlayNewGameSound();
                    gFirstTimeStartingGame = false;
                    gFadeOutTitleButtons = true;
                    MMHideAllCursors();
                    DoFadeToRedAndZoom(500);
                }
                ExitTitleScreen();
                SetUpDifficultyScreen();
            }
            else if (button == kTitleOpenMansion)
                HandleOpenMansionButton();
            else if (button == kTitleRandomMansion) {
                if (MMDoRandomMansionDialog() == 0)
                    StartCustomMansion();
                else
                    DoGameFadeToNormal(0);
            }
            else
                OpenSavedGameFromTitleScreen();
            break;
        case kTitleHighScores:
            MMHideCursor();
            DoGameFadeToBlackFullAnim(30, false);
            ExitTitleScreen();
            SetUpHighScoresScreen(1, false, false);
            break;
        case kTitleHelp:
            MMHideCursor();
            DoGameFadeToBlackFullAnim(30, false);
            ExitTitleScreen();
            SetUpHelpScreen();
            break;
        case kTitleOptions:
            MMDoOptionsDialog();
            break;
        case kTitleTrophyRoom:
            MMDoTrophyRoom();
            break;
        case kTitleQuit:
            MMHideCursor();
            DoGameFadeToBlackFullAnim(15, false);
            ExitTitleScreen();
            gQuitGame = true;
            break;
        case kTitleChoosePlayer:
            if (DoChoosePlayerDialog()) {
                SWDisposeAllSpritesInLayer(gTextSpriteLayerP);
                DrawTitleScreenBackground(&rect);
                SWUpdateSpriteWorld(gSpriteWorldP, true);
            }
            break;
        case kTitleOpenMovie:
            MMDoOpenMovieDialog();
            break;
    }
}

void ExitTitleScreen(void)
{
    gCurrentGameMode = kNoGameMode;
    gSpriteWorldIsValid = false;
    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
    SWSetDesignResolution(gSpriteWorldP, 640, 480);
}

// ---------------------------------------------------------------------------
// Help screen (four pages)

void SetUpHelpScreen(void)
{
    SpritePtr spriteP;
    short h = 0x1a9;

    gCurrentlyInMansion = false;
    gCurrentHelpScreenIndex = 1;
    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
    CreateAnimatedMoonBackground(0.3f);
    DrawContentsForHelpScreenIndex(gCurrentHelpScreenIndex);
    gCurrentGameMode = kNoGameMode;

    gLeftArrowButtonSpriteP = NewButtonSprite(0xd6, kLeftArrowButton);
    if (gLeftArrowButtonSpriteP != NULL) {
        SWAddSprite(gTopSpriteLayerP, gLeftArrowButtonSpriteP);
        SetUpButtonSprite(gLeftArrowButtonSpriteP, 0, 1, 2, 3);
        InstallButtonProc(gLeftArrowButtonSpriteP, MMStandardButtonProc);
        SWSetSpriteLocation(gLeftArrowButtonSpriteP, 425.0f, 448.0f);
        h = (short)(SWGetSpriteWidth(gLeftArrowButtonSpriteP) + 15.0f + 425.0f);
        SetButtonDimmedMode(gLeftArrowButtonSpriteP, true);
    }
    spriteP = NewButtonSprite(0xd7, kDoneButton);
    if (spriteP != NULL) {
        SWAddSprite(gTopSpriteLayerP, spriteP);
        SetUpButtonSprite(spriteP, 0, 1, 2, 0);
        InstallButtonProc(spriteP, MMStandardButtonProc);
        SWSetSpriteLocation(spriteP, h, 448.0f);
        h = (short)(SWGetSpriteWidth(spriteP) + 15.0f + h);
    }
    gRightArrowButtonSpriteP = NewButtonSprite(0xd6, kRightArrowButton);
    if (gRightArrowButtonSpriteP != NULL) {
        SWAddSprite(gTopSpriteLayerP, gRightArrowButtonSpriteP);
        SetUpButtonSprite(gRightArrowButtonSpriteP, 0, 1, 2, 3);
        InstallButtonProc(gRightArrowButtonSpriteP, MMStandardButtonProc);
        SWSetSpriteFlipX(gRightArrowButtonSpriteP, true);
        SWSetSpriteLocation(gRightArrowButtonSpriteP, h, 448.0f);
    }
    MMShieldCursor();
    MMSetCursorForGame();
    SWProcessSpriteWorld(gSpriteWorldP);
    SWUpdateSpriteWorld(gSpriteWorldP, true);
    gCurrentGameMode = kHelpScreenMode;
    gSpriteWorldIsValid = true;
    DoGameFadeToNormalFullAnim(30);
}

void HandleHelpScreenCallback(void)
{
    SWPoint mouse;
    Boolean mouseDown;
    short button;

    SWProcessSpriteWorld(gSpriteWorldP);
    SWAnimateSpriteWorld(gSpriteWorldP);
    if (!gSpriteWorldP->frameHasOccurred)
        return;
    mouseDown = MMGetMouse(&mouse);
    SWConvertCoordinates(gSpriteWorldP, &mouse);
    button = ProcessButtonSprites(gTopSpriteLayerP, gTopSpriteLayerP, mouse, mouseDown, false, NULL);
    if (button == -1)
        return;
    AnimateOneFrame(gSpriteWorldP);
    if (button == kLeftArrowButton) {
        gCurrentHelpScreenIndex--;
        SetButtonDimmedMode(gRightArrowButtonSpriteP, false);
        if (gCurrentHelpScreenIndex == 1)
            SetButtonDimmedMode(gLeftArrowButtonSpriteP, true);
        DrawContentsForHelpScreenIndex(gCurrentHelpScreenIndex);
    }
    else if (button == kRightArrowButton) {
        gCurrentHelpScreenIndex++;
        SetButtonDimmedMode(gLeftArrowButtonSpriteP, false);
        if (gCurrentHelpScreenIndex == 4)
            SetButtonDimmedMode(gRightArrowButtonSpriteP, true);
        DrawContentsForHelpScreenIndex(gCurrentHelpScreenIndex);
    }
    else if (button == kDoneButton) {
        DoGameFadeToBlackFullAnim(30, false);
        ExitHelpScreen();
        SetUpTitleScreen();
    }
}

void DrawContentsForHelpScreenIndex(short index)
{
    char path[1024];
    SpritePtr pageP;
    SWError err;

    snprintf(path, sizeof(path), "%shelpScreen%d.png", GetScreensFolderPath(), index);
    err = SWCreateSpriteFromFile(&pageP, NULL, path, 1, false);
    if (err == kNoError) {
        SetBackgroundToDesignSize(pageP);
        RemoveAllSpritesInLayer(gForegroundSpriteLayerP);
        SWAddSprite(gForegroundSpriteLayerP, pageP);
        SWCenterSpriteInRect(pageP, &gSpriteWorldP->workAreaRect);
    }
    RemoveAllSpritesInLayer(gEnemySpriteLayerP);
    RemoveAllSpritesInLayer(gBirdSpriteLayerP);
    RemoveAllSpritesInLayer(gBigSpiderSpriteLayerP);
    RemoveAllSpritesInLayer(gMedusaSpriteLayerP);
    RemoveAllSpritesInLayer(gEyeballSpriteLayerP);
    RemoveAllSpritesInLayer(gObjectSpriteLayerP);
    RemoveAllSpritesInLayer(gGhostSpriteLayerP);
    RemoveAllSpritesInLayer(gSpyVsSpySpriteLayerP);
    SWUpdateSpriteWorld(gSpriteWorldP, true);
    if (err == kNoError) {
        if (index == 2)
            AddMonsterSpritesToHelpScreen();
        else if (index == 3)
            AddObjectSpritesToHelpScreen();
    }
}

static SpritePtr AddHelpSprite(const short entry[3])
{
    SpritePtr spriteP = CreateMySprite(entry[0]);

    if (spriteP != NULL) {
        SWSetSpriteLocation(spriteP, entry[1], entry[2]);
        SetUpSpriteFrameRange(spriteP);
        SWSetSpriteMoveProc(spriteP, NULL);
    }
    return spriteP;
}

void AddMonsterSpritesToHelpScreen(void)
{
    SpritePtr spriteP;
    short i, type;

    for (i = 0; gMonsterArray[i][0] != 0; i++) {
        type = gMonsterArray[i][0];
        spriteP = AddHelpSprite(gMonsterArray[i]);
        if (spriteP == NULL)
            continue;
        if (type == kBlueRavenSprite || type == kScorpionSprite || type == kSkullSpiderSprite)
            SWSetSpriteFlipX(spriteP, true);
        else if (type == 8 || type == 9) {     // Medusa heads
            SWSetSpriteFrameRange(spriteP, 0, 2);
            SWSetSpriteFlipX(spriteP, true);
        }
    }
}

void AddObjectSpritesToHelpScreen(void)
{
    short i;

    for (i = 0; gObjectSpritesArray[i][0] != 0; i++)
        AddHelpSprite(gObjectSpritesArray[i]);
}

void ExitHelpScreen(void)
{
    gCurrentGameMode = kNoGameMode;
    UpdateMenuBar();
    gSpriteWorldIsValid = false;
    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
}

// ---------------------------------------------------------------------------
// Difficulty screen

void SetUpDifficultyScreen(void)
{
    SpritePtr spriteP;
    SWRect rect;
    short i, top = 0xaa;

    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
    CreateAnimatedMoonBackground(1.0f);
    gCurrentGameMode = kNoGameMode;
    GiveOSTime();
    LoadDifficultyScreenSprites();
    spriteP = NewDifficultyScreenSprite(3);
    if (spriteP != NULL)
        SWSetSpriteLocation(spriteP, 40.0f, 10.0f);
    for (i = 0; i < 3; i++) {
        spriteP = NewDifficultyScreenButtonSprite(i);
        if (spriteP == NULL) {
            MM_FATAL(-1);
        }
        else {
            SetUpButtonSprite(spriteP, 0, 1, 2, 0);
            InstallButtonProc(spriteP, DifficultyScreenButtonProc);
            AdjustCollisionRectForOffset(spriteP);
            rect = spriteP->destFrameRect;
            SWCenterRect(&rect, &gSpriteWorldP->workAreaRect);
            SWOffsetRect(&rect, -130, top - rect.top);
            SWSetSpriteLocation(spriteP, rect.left, rect.top);
        }
        top += 0x46;
    }
    gIllustrationSpriteP = NewDifficultyScreenSprite(4);
    if (gIllustrationSpriteP != NULL) {
        SWSetSpriteLocation(gIllustrationSpriteP, 296.0f, 89.0f);
        SWSetSpriteVisible(gIllustrationSpriteP, false);
    }
    AddBackButtonToScreen(gTopSpriteLayerP, 10);
    MMShieldCursor();
    MMSetCursorForGame();
    SWProcessSpriteWorld(gSpriteWorldP);
    SWUpdateSpriteWorld(gSpriteWorldP, true);
    gCurrentGameMode = kDifficultyScreenMode;
    UpdateMenuBar();
    gSpriteWorldIsValid = true;
    GiveOSTime();
    DoGameFadeToNormalFullAnim(30);
}

void HandleDifficultyScreenCallback(void)
{
    SWPoint mouse;
    Boolean mouseDown;
    short button;

    SWProcessSpriteWorld(gSpriteWorldP);
    SWAnimateSpriteWorld(gSpriteWorldP);
    if (!gSpriteWorldP->frameHasOccurred)
        return;
    mouseDown = MMGetMouse(&mouse);
    SWConvertCoordinates(gSpriteWorldP, &mouse);
    SWSetSpriteVisible(gIllustrationSpriteP, false);
    button = ProcessButtonSprites(gTopSpriteLayerP, gTopSpriteLayerP, mouse, mouseDown, false, NULL);
    if (gEscapeKey)
        button = kBackButton;
    if (button == -1)
        return;

    AnimateOneFrame(gSpriteWorldP);
    if (button == kBackButton) {
        DoGameFadeToBlackFullAnim(30, false);
        ExitDifficultyScreen();
        SetUpTitleScreen();
    }
    else {
        gDifficultySetting = (char)button;
        SetEasyJumpingMode(gEasyJumping);
        MMHideCursor();
        DoGameFadeToBlackFullAnim(30, !gPlayTutorialMansion);
        ExitDifficultyScreen();
        if (!AreAnyMansionsBeaten() && !gHasShownOrganScreen)
            DoOrganScreen();
        else
            SetUpSelectMansionScreen(1);
    }
}

void HandleOpenMansionButton(void)
{
    char message[300];
    const char *folderName = "Midnight Mansion HD Custom Mansions";

    if (!gCanPlayBothEpisodes) {
        MMDoCustomDialog(gSpriteWorldP,
            "This feature is available when you purchase both Episode 1 and Episode 2. \r\rThen you can play "
            "over 100 custom mansions created by fans of the game!", 3);
        return;
    }
        // (not asked when this version's bundled mansions are there to choose from)
    if (!DoesFileOrFolderExistInPath(GetGameDirectory(), folderName) && GetBundledCustomMansionsPath()[0] == '\0') {
        snprintf(message, sizeof(message),
                 "The folder: \r%s\rdoesn't exist. Do you want to create it now? \r\r(It will be placed in the same "
                 "directory as Midnight Mansion.)", folderName);
        if (MMDoCustomDialog(gSpriteWorldP, message, 2) == 2)
            MMCreateDirectory(GetGameDirectory(), folderName);
    }
    if (MMDoOpenMansionDialog() == 0)
        StartCustomMansion();
    else
        DoGameFadeToNormal(0);
}

void StartCustomMansion(void)
{
    MMHideCursor();
    DoGameFadeToBlackFullAnim(30, true);
    if (gCurrentGameMode == kDifficultyScreenMode)
        ExitDifficultyScreen();
    else if (gCurrentGameMode == kTitleScreenMode)
        ExitTitleScreen();
    StartGameNoEditor();
}

/// Easy jumping (easy difficulty only) gives a lower, more controllable jump.
void SetEasyJumpingMode(Boolean easy)
{
    if (!IsMoviePlaying())
        gEasyJumping = easy;
    if (gDifficultySetting == 0 && easy == true) {
        gJumpSpeed = 8.2f;
        gJumpChange = 0.37f;
        gHorizontalBoost = 1.1f;
    }
    else {
        gJumpSpeed = 9.8f;
        gJumpChange = 0.55f;
        gHorizontalBoost = 1.08f;
    }
}

void ExitDifficultyScreen(void)
{
    WriteProfileToDisk();
    gCurrentGameMode = kNoGameMode;
    UpdateMenuBar();
    gSpriteWorldIsValid = false;
    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
    DisposeDifficultyScreenSprites();
    gIllustrationSpriteP = NULL;
}

/// Night sky with the moon and drifting clouds (help and difficulty screens).
void CreateAnimatedMoonBackground(float cloudSpeed)
{
    SpritePtr spriteP;
    SWError err;

    if (!gTitleScreenSpritesAreValid)
        puts("Can't show animated moon background -- title screen sprites not loaded!");
    err = LoadAndInstallBackgroundSprite(&spriteP, GetTitleScreenFolderPath(NULL), "starBackground.jpg", -1, false);
    if (err != kNoError)
        MM_FATAL(err);
    SetBackgroundToDesignSize(spriteP);

    spriteP = NewTitleScreenSprite(7);
    if (spriteP != NULL) {
        SWRemoveSprite(spriteP);
        SWAddSprite(gBottomSpriteLayerP, spriteP);
        SWSetSpriteLocation(spriteP, 280.0f, 93.0f);
        spriteP->miscFloat = 0.55f;
        SWSetSpriteScaledFactor(spriteP, 1.0f, 1.0f);
    }
    spriteP = NewTitleScreenSprite(0xd);
    if (spriteP != NULL) {
        SWRemoveSprite(spriteP);
        SWAddSprite(gBackgroundSpriteLayerP, spriteP);
        SWSetSpriteFrameTime(spriteP, 0x8e);
        SWSetSpriteFrameAdvanceMode(spriteP, kPatrollingMode);
        SWSetSpriteLocation(spriteP, 487.0f, 134.0f);
        spriteP->miscFloat = 1.0f;
        SWSetSpriteScaledFactor(spriteP, 0.5f, 0.5f);
    }
    spriteP = NewTitleScreenSprite(5);
    if (spriteP != NULL) {
        SWRemoveSprite(spriteP);
        SWAddSprite(gBackgroundSpriteLayerP, spriteP);
        SWSetSpriteLocation(spriteP, 314.0f, 139.0f);
        spriteP->miscFloat = 1.0f;
        SWSetSpriteScaledFactor(spriteP, 0.5f, 0.5f);
    }
    AddCloudRows(1, 5, 30, 290, cloudSpeed, false);
}

// ---------------------------------------------------------------------------
// Organ screen: the intro poem, shown before the first mansion

void DoOrganScreen(void)
{
    SpritePtr spriteP = NULL;
    SWRect rect;
    short i, v;
    SWError err;

    for (i = 0; i < 8; i++)
        gPoemTextSpriteArray[i] = NULL;
    LoadOrganScreenSprites();
    StopAllChannels();
    DoGameFadeToBlackFullAnim(30, false);
    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
    err = LoadAndInstallBackgroundSprite(&spriteP, GetOrganScreenFolderPath(NULL), "organscreen.jpg", -1, false);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledSize(spriteP, 640, 480);

    // White flash for lightning
    err = SWCreateSprite(&gOrganLightningSpriteP, NULL, 0);
    if (err == kNoError) {
        SWSetSpriteDrawProc(gOrganLightningSpriteP, SWSolidColorSpriteDrawProc);
        gOrganLightningSpriteP->destFrameRect = gSpriteWorldP->workAreaRect;
        SWAddSprite(gBackgroundPictureSpriteLayerP, gOrganLightningSpriteP);
        SWSetSpriteColor(gOrganLightningSpriteP, 0xffff, 0xffff, 0xffff);
        SWSetSpriteVisible(gOrganLightningSpriteP, false);
        SWSetSpriteTranslucencyLevel(gOrganLightningSpriteP, 0x7fff);
        SWSetSpriteBlendMode(gOrganLightningSpriteP, GL_SRC_ALPHA, GL_ONE);
    }

    gOrganPlayerSpriteP = NewOrganScreenSprite(0);
    if (gOrganPlayerSpriteP != NULL) {
        SWSetSpriteLocation(gOrganPlayerSpriteP, 213.5f, 272.0f);
        SWSetSpriteFrameTime(gOrganPlayerSpriteP, 0x115);
        SWSetSpriteFrameAdvance(gOrganPlayerSpriteP, 0);
        SWSetSpriteFrameProc(gOrganPlayerSpriteP, RandomFrameProc);
        SWSetSpriteFrameRange(gOrganPlayerSpriteP, 0, 3);
        SWSetSpriteMoveProc(gOrganPlayerSpriteP, NULL);
    }
    spriteP = NewOrganScreenSprite(1);
    if (spriteP != NULL) {
        SWSetSpriteLocation(spriteP, 190.5f, 317.0f);
        SWSetSpriteFrameTime(spriteP, 0x115);
        SWSetSpriteFrameAdvance(spriteP, 0);
        SWSetSpriteFrameProc(spriteP, RandomFrameProc);
    }

    // The poem scrolls up from below the screen.
    v = 0x1cc;
    for (i = 0; i < 8; i++) {
        spriteP = NewOrganScreenSprite(2);
        if (spriteP == NULL)
            continue;
        rect = spriteP->destFrameRect;
        SWCenterRect(&rect, &gSpriteWorldP->workAreaRect);
        SWSetCurrentFrameIndex(spriteP, i);
        SWSetSpriteLocation(spriteP, rect.left, v);
        SWSetSpriteMoveProc(spriteP, PoemSpriteMoveProc);
        SWSetSpriteVisible(spriteP, false);
        gPoemTextSpriteArray[i] = spriteP;
        v += (i == 1 || i == 3 || i == 5) ? 0x73 : 0x32;
    }

    SWSetSpriteWorldMaxFPS(gSpriteWorldP, 0);
    SWSetSpriteWorldPseudoFPS(gSpriteWorldP, 60);
    SWSetMinimumFrameRate(gSpriteWorldP, 10);
    if (!gFullScreenWindow) {
        MMRemoveGameCursor();
        MMShieldCursor();
    }
    else
        MMHideAllCursors();
    SWUpdateSpriteWorld(gSpriteWorldP, true);
    gCurrentGameMode = kOrganScreenMode;
    gSpriteWorldIsValid = true;
    UpdateMenuBar();
    GiveOSTime();
    MMFlushEvents();
    DoGameFadeToNormalFullAnim(30);
    gOrganScreenWasDismissed = false;
    lightningCount = 0;
    gDidChangeToSkeleton = false;
    gHasShownOrganScreen = true;
    WriteProfileToDisk();
}

void HandleOrganScreenCallback(void)
{
    SWProcessSpriteWorld(gSpriteWorldP);
    SWAnimateSpriteWorld(gSpriteWorldP);

    // Three lightning flashes; on the second the organist turns into a skeleton.
    if (gSpriteWorldP->frameHasOccurred && lightningCount > 0 && lightningCount < 7 && GetElapsedMillisecs() > 100) {
        StartMicrosecTimer();
        if (lightningCount == 1 || lightningCount == 3 || lightningCount == 5) {
            SWSetSpriteVisible(gOrganLightningSpriteP, true);
            if (FindSound(0x3a) == -1)
                MMPlayStereoSound2(NULL, 0x3a, 0, 1, gSoundVolume / 2, 0.7, 0);
        }
        else if (lightningCount == 2 || lightningCount == 4 || lightningCount == 6)
            SWSetSpriteVisible(gOrganLightningSpriteP, false);
        if (lightningCount == 3) {
            if (gOrganPlayerSpriteP != NULL)
                SWSetSpriteFrameRange(gOrganPlayerSpriteP, 4, 7);
            gDidChangeToSkeleton = true;
        }
        lightningCount++;
    }
    if (gPoemTextSpriteArray[7] != NULL && lightningCount == 0 && !gDidChangeToSkeleton &&
        SWGetSpriteVertLoc(gPoemTextSpriteArray[7]) < 260.0f) {
        lightningCount = 1;
        StartMicrosecTimer();
    }

    if (IsMouseDown() || gOrganScreenWasDismissed || (GetElapsedMillisecs() > 7500 && gDidChangeToSkeleton)) {
        DoGameFadeToBlackFullAnim(55, true);
        MMFlushEvents();
        MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
        DisposeOrganScreenSprites();
        gOrganPlayerSpriteP = NULL;
        SWSetSpriteWorldMaxFPS(gSpriteWorldP, 60);
        MMFlushEvents();
        SetUpSelectMansionScreen(1);
    }
}

/// Poem lines rise and fade in near the bottom, fade out near the top.
void PoemSpriteMoveProc(SpritePtr spriteP)
{
    int v, percent;

    SWOffsetSpriteBasedOnTime(spriteP, 0.0f, -0.55f);
    v = (int)SWGetSpriteVertLoc(spriteP);
    if (v < 0x1b9 && v > 0x27) {
        if (v >= 0x169)
            percent = 100 - (v * 100 - 36000) / 80;
        else if (v < 0x78)
            percent = (v * 100 - 12000) / 80 + 100;
        else
            percent = 100;
    }
    else
        percent = 0;

    if (percent == 100) {
        if (!spriteP->isVisible)
            SWSetSpriteVisible(spriteP, true);
    }
    else if (percent == 0)
        SWSetSpriteVisible(spriteP, false);
    else {
        if (!spriteP->isVisible)
            SWSetSpriteVisible(spriteP, true);
        SWSetSpriteTranslucencyLevel(spriteP, percent * 0x28f);
    }
    if (v < -300) {
        SWSetSpriteVisible(spriteP, false);
        SWSetSpriteMoveProc(spriteP, NULL);
    }
}

// ---------------------------------------------------------------------------
// Select a mansion (two map halves: Episode 1 on the left, Episode 2 on the right)

/// side: 1 or 3 shows the left half, 2 the right.
void SetUpSelectMansionScreen(short side)
{
    SpritePtr titleP;
    SWRect rect;

    if (side == 3)
        side = 1;
    LoadMapScreenSprites();
    GiveOSTime();
    AddBackButtonToScreen(gBottomSpriteLayerP, 0);
    SetUpSelectMansionScreenMM1(side);
    titleP = NewMapScreenSprite(0xd);
    if (titleP != NULL) {
        SWSetSpriteLocation(titleP, 40.0f, 10.0f);
        rect = titleP->destFrameRect;
        SWCenterRect(&rect, &gSpriteWorldP->workAreaRect);
        SWSetSpriteLocation(titleP, rect.left, 0.0f);
    }
    MMShieldCursor();
    MMSetCursorForGame();
    SWProcessSpriteWorld(gSpriteWorldP);
    SWUpdateSpriteWorld(gSpriteWorldP, true);
    StopAllChannels();
    StartMusicByFadingIn("Map Screen Music", 500, (short)(gTitleMusicVolume * 0.9));
    gCurrentGameMode = kSelectMansionMode;
    UpdateMenuBar();
    gSpriteWorldIsValid = true;
    GiveOSTime();
    DoGameFadeToNormalFullAnim(30);
}

void SetUpSelectMansionScreenMM1(short side)
{
    SpritePtr spriteP, markerP;
    MMTextStyleRec style;
    SWRGBColor teal = { 0, 0xffff, 0xffff };
    SWRect rect;
    const char *difficultyText = "";
    short i;
    SWError err;

    gCurrentlyInMansion = false;
    err = LoadAndInstallBackgroundSprite(&spriteP, GetMapScreenFolderPath(NULL), "map_screen_back_left.jpg", -1, false);
    if (err == kNoError)
        SetBackgroundToDesignSize(spriteP);
    err = SWCreateSpriteFromFile(&spriteP, NULL, GetMapScreenFolderPath("map_screen_back_right.jpg"), 1, false);
    if (err == kNoError) {
        SWAddSprite(gBackgroundScreenSpriteLayerP, spriteP);
        SWSetSpriteLocation(spriteP, 640.0f, 0.0f);
        SetBackgroundToDesignSize(spriteP);
    }

    SetTextStyle(&style, "\015BlackChancery", 1, 28, 2);
    rect = gSpriteWorldP->workAreaRect;
    rect.left = rect.left + (rect.right - rect.left) / 2;
    rect.bottom -= 14;
    if (gDifficultySetting == 0)
        difficultyText = "Difficulty: Easy";
    else if (gDifficultySetting == 1)
        difficultyText = "Difficulty: Normal";
    else if (gDifficultySetting == 2)
        difficultyText = "Difficulty: Hard";
    MyDrawCTextJustified(difficultyText, &style, &rect, &teal);

    gEpisode1TextSpriteP = NewMiscSprite(0xe2);
    if (gEpisode1TextSpriteP != NULL) {
        SWSetCurrentFrameIndex(gEpisode1TextSpriteP, 0);
        SWAddSprite(gForegroundSpriteLayerP, gEpisode1TextSpriteP);
        SWSetSpriteScaledFactor(gEpisode1TextSpriteP, 0.6f, 0.6f);
        SWSetSpriteLocation(gEpisode1TextSpriteP, SWGetSpriteWidth(gEpisode1TextSpriteP) / -2.0f + 320.0f, 45.0f);
    }
    gEpisode2TextSpriteP = NewMiscSprite(0xe2);
    if (gEpisode2TextSpriteP != NULL) {
        SWSetCurrentFrameIndex(gEpisode2TextSpriteP, 1);
        SWAddSprite(gForegroundSpriteLayerP, gEpisode2TextSpriteP);
        SWSetSpriteScaledFactor(gEpisode2TextSpriteP, 0.6f, 0.6f);
        SWSetSpriteLocation(gEpisode2TextSpriteP, SWGetSpriteWidth(gEpisode2TextSpriteP) / -2.0f + 960.0f, 45.0f);
    }

    gNamesSpriteP = NewMapScreenSprite(0xb);
    if (gNamesSpriteP == NULL)
        MM_FATAL(-1);
    else
        SWSetSpriteVisible(gNamesSpriteP, false);

    for (i = 0; i < 11; i++) {
        spriteP = NewMapScreenSprite(i);
        if (spriteP == NULL) {
            MM_FATAL(-1);
            continue;
        }
        InstallButtonProc(spriteP, SelectMansionScreenButtonProc);
        ((ButtonSpritePtr)spriteP)->showsWhenHovered = true;
        if (i < 5)
            SWSetSpriteLocation(spriteP, gMapSpriteLocLeftSide[i][0], gMapSpriteLocLeftSide[i][1]);
        else
            SWSetSpriteLocation(spriteP, gMapSpriteLocRightSide[i - 5][0] + 640.0f, gMapSpriteLocRightSide[i - 5][1]);
        SWSetSpriteVisible(spriteP, false);
        if (IsMansionInArray(gDifficultySetting, i)) {
            markerP = NewMapScreenSprite(0xc);     // "completed" marker
            if (markerP != NULL)
                SWCenterSpriteOverSprite(markerP, spriteP);
        }
        if (IsMansionLocked(i)) {
            markerP = NewMapScreenSprite(0xe);     // padlock
            if (markerP != NULL) {
                SWCenterSpriteOverSprite(markerP, spriteP);
                SWOffsetSprite(markerP, SWGetSpriteWidth(spriteP) / 4.0f, SWGetSpriteHeight(spriteP) / 4.0f);
            }
        }
    }

    if (!AreAnyMansionsBeatenOnAnyDifficulty()) {
        spriteP = NewMapScreenSprite(0xf);         // "Start here" bouncing over Hint House
        if (spriteP != NULL) {
            SWSetSpriteLocation(spriteP, gMapSpriteLocLeftSide[0][0] + 15.0f, gMapSpriteLocLeftSide[0][1] - 56.0f);
            spriteP->miscFloat = SWGetSpriteVertLoc(spriteP);
            SWSetSpriteMoveProc(spriteP, StartHereBounceMoveProc);
        }
    }

    if (side == 2) {
        OffsetSpritesInLayer(gBackgroundScreenSpriteLayerP, -640.0f, 0);
        OffsetSpritesInLayer(gBottomSpriteLayerP, -640.0f, 0);
        OffsetSpritesInLayer(gForegroundSpriteLayerP, -640.0f, 0);
        gShowingLeftSide = false;
    }
    else
        gShowingLeftSide = true;
    AddArrowButtonToMM1MansionScreen(gShowingLeftSide);
}

void AddArrowButtonToMM1MansionScreen(Boolean showingLeftSide)
{
    SpritePtr arrowP;

    DisposeButtonSpritesMatchingIndex(gBottomSpriteLayerP, kLeftArrowButton);
    arrowP = NewButtonSprite(0xd6, kLeftArrowButton);
    if (arrowP == NULL)
        return;
    SWAddSprite(gBottomSpriteLayerP, arrowP);
    SetUpButtonSprite(arrowP, 0, 1, 2, 0);
    InstallButtonProc(arrowP, MMStandardButtonProc);
    SetButtonPushedOffset(arrowP, 1, 1);
    AdjustCollisionRectForOffset(arrowP);
    SWSetSpriteScaledFactor(arrowP, 0.8f, 0.8f);
    SWSetSpriteMoveProc(arrowP, ArrowButtonColorMoveProc);
    if (!showingLeftSide)
        SWSetSpriteLocation(arrowP, 10.0f, SWGetSpriteHeight(arrowP) / -2.0f + 240.0f);
    else {
        SWSetSpriteLocation(arrowP, 586.0f, SWGetSpriteHeight(arrowP) / -2.0f + 240.0f);
        SWSetSpriteFlipX(arrowP, true);
    }
}

void SetUpSelectMansionScreenMM2(void)
{
}

void HandleSelectMansionScreenCallback(void)
{
    SWPoint mouse;
    Boolean mouseDown;
    Str255 mansionName;
    ConstStringPtr difficultyName = (ConstStringPtr)"";
    short button;

    SDL_assert(gNamesSpriteP != NULL);
    if (gGameIsPaused)
        return;
    SWProcessSpriteWorld(gSpriteWorldP);
    SWAnimateSpriteWorld(gSpriteWorldP);
    if (!gSpriteWorldP->frameHasOccurred)
        return;

    mouseDown = MMGetMouse(&mouse);
    SWConvertCoordinates(gSpriteWorldP, &mouse);
    button = ProcessButtonSprites(gBottomSpriteLayerP, gBottomSpriteLayerP, mouse, mouseDown, false, NULL);
    if (gEscapeKey)
        button = kBackButton;

    if (button == kBackButton) {
        AnimateOneFrame(gSpriteWorldP);
        MMHideCursor();
        DoGameFadeToBlackFullAnim(30, true);
        ExitSelectMansionScreen();
        SetUpTitleScreen();
    }
    else if (button == kLeftArrowButton) {
        // Scroll to the other half of the map.
        SWSetSpriteLayerVisible(gBottomSpriteLayerP, false);
        SWSetSpriteVisible(gEpisode1TextSpriteP, false);
        SWSetSpriteVisible(gEpisode2TextSpriteP, false);
        ScrollSelectAMansionScreen(gShowingLeftSide);
        gShowingLeftSide = !gShowingLeftSide;
        AddArrowButtonToMM1MansionScreen(gShowingLeftSide);
        SWSetSpriteLayerVisible(gBottomSpriteLayerP, true);
        SWSetSpriteVisible(gShowingLeftSide ? gEpisode1TextSpriteP : gEpisode2TextSpriteP, true);
    }
    else if (button != -1) {
        if (!IsMansionLocked(button)) {
            // Mansion folders are named "<Mansion> <Difficulty>"; Hint House only has Normal.
            mansionName[0] = 0;
            if (gDifficultySetting == 0)
                difficultyName = (ConstStringPtr)"\005 Easy";
            else if (gDifficultySetting == 1)
                difficultyName = (ConstStringPtr)"\007 Normal";
            else if (gDifficultySetting == 2)
                difficultyName = (ConstStringPtr)"\005 Hard";
            if (button == 0)
                difficultyName = (ConstStringPtr)"\007 Normal";
            SWConcatPascalStrings(mansionName, GetMansionNameFromIndex(button));
            SWConcatPascalStrings(mansionName, difficultyName);
            MMHideCursor();
            DoGameFadeToBlackFullAnim(30, true);
            if (MyOpenStandardMansion(mansionName, button) == 0) {
                ExitSelectMansionScreen();
                StartGameNoEditor();
            }
            else
                DoGameFadeToNormal(0);
        }
        else if (button == 10 && !CanPlayFinalMansion())
            MMDoCustomDialog(gSpriteWorldP,
                "You can't play Spider Palace until you have completed all of the other mansions.", 3);
        else if (button < 5)
            MMDoCustomDialog(gSpriteWorldP,
                "You can't play this mansion until you buy Episode 1. Try playing Falcon Manor or Nightmare Mansion "
                "instead.", 3);
        else
            MMDoCustomDialog(gSpriteWorldP, "You can't play this mansion until you buy Episode 2.", 3);
    }
}

void ScrollSelectAMansionScreen(Boolean showingLeftSide)
{
    long scrolled = 0;
    float step = showingLeftSide ? -20.0f : 20.0f;

    while (scrolled < 640) {
        SWProcessSpriteWorld(gSpriteWorldP);
        SWAnimateSpriteWorld(gSpriteWorldP);
        if (gSpriteWorldP->frameHasOccurred) {
            ProcessSDLEvents();
            OffsetSpritesInLayer(gBackgroundScreenSpriteLayerP, step, 0);
            OffsetSpritesInLayer(gBottomSpriteLayerP, step, 0);
            OffsetSpritesInLayer(gForegroundSpriteLayerP, step, 0);
            scrolled += 20;
        }
    }
}

/// Mansion buttons are invisible until hovered; the hovered one shows its name below it.
void SelectMansionScreenButtonProc(SpritePtr buttonP, short buttonIndex, short newState, short oldState)
{
    if (newState == oldState)
        return;

    if (newState == kButtonHighlighted || newState == kButtonPushed) {
        SWSetSpriteVisible(buttonP, true);
        gOldButtonIndex = buttonIndex;
        SWSetSpriteVisible(gNamesSpriteP, true);
        SWCenterSpriteOverSprite(gNamesSpriteP, buttonP);
        SWOffsetSprite(gNamesSpriteP, 0.0f, SWGetSpriteHeight(buttonP) / 2.0f);
        switch (buttonIndex) {
            case 2: SWOffsetSprite(gNamesSpriteP, 6.0f, -4.0f); break;
            case 4: SWOffsetSprite(gNamesSpriteP, 0.0f, -5.0f); break;
            case 5: SWOffsetSprite(gNamesSpriteP, 3.0f, 3.0f); break;
            case 6: SWOffsetSprite(gNamesSpriteP, 0.0f, 4.0f); break;
            case 7: SWOffsetSprite(gNamesSpriteP, -5.0f, -5.0f); break;
        }
        SWSetCurrentFrameIndex(gNamesSpriteP, buttonIndex);
        if (newState == kButtonPushed && oldState != kButtonPushed)
            SWSetSpriteColor(gNamesSpriteP, 0, 0xffff, 0);
        else
            SWSetSpriteColor(gNamesSpriteP, 0xffff, 0xffff, 0xffff);
        SWSetSpriteTranslucencyLevel(gNamesSpriteP, IsMansionLocked(buttonIndex) ? 36000 : 0xffff);
        MakeButtonBounceWhenSelected(gNamesSpriteP, newState, oldState, 0.1f, 0.15f, 2.0f, 2.0f);
    }
    else {
        SWSetSpriteVisible(buttonP, false);
        if (gOldButtonIndex == buttonIndex)
            SWSetSpriteVisible(gNamesSpriteP, false);
    }
    PlayButtonSound(newState, oldState);
}

ConstStringPtr GetMansionNameFromIndex(short index)
{
    switch (index) {
        case 0:  return (ConstStringPtr)"\012Hint House";
        case 1:  return (ConstStringPtr)"\014Falcon Manor";
        case 2:  return (ConstStringPtr)"\021Nightmare Mansion";
        case 3:  return (ConstStringPtr)"\016Knight Mansion";
        case 4:  return (ConstStringPtr)"\017Tomb of Princes";
        case 5:  return (ConstStringPtr)"\026House of Baron Garneau";
        case 6:  return (ConstStringPtr)"\013Castle Doom";
        case 7:  return (ConstStringPtr)"\020Cathedral Towers";
        case 8:  return (ConstStringPtr)"\020The Great Escape";
        case 9:  return (ConstStringPtr)"\021Chateau de Medusa";
        case 10: return (ConstStringPtr)"\015Spider Palace";
        default: return (ConstStringPtr)"\005Error";
    }
}

/// Only Spider Palace is ever locked: it needs every other mansion beaten on this difficulty.
Boolean IsMansionLocked(short index)
{
    if (index == 0 || IsMansionDemoMansion(index))
        return false;
    if (index < 5)
        return !gEpisode1IsUnlocked;
    if (index == 10)
        return !CanPlayFinalMansion();
    return !gEpisode2IsUnlocked;
}

Boolean IsMansionDemoMansion(short index)
{
    return index == 1 || index == 2;
}

Boolean CanPlayFinalMansion(void)
{
    short i;
    for (i = 1; i <= 9; i++)
        if (!IsMansionInArray(gDifficultySetting, i))
            return false;
    return true;
}

Boolean AreAnyMansionsBeaten(void)
{
    short i;
    for (i = 0; i <= 10; i++)
        if (IsMansionInArray(gDifficultySetting, i))
            return true;
    return false;
}

Boolean AreAnyMansionsBeatenOnAnyDifficulty(void)
{
    short d, i;
    for (d = 0; d <= 2; d++)
        for (i = 0; i < 11; i++)
            if (IsMansionInArray(d, i))
                return true;
    return false;
}

Boolean AreAllEpisode1MansionsBeaten(void)
{
    short i;
    for (i = 0; i <= 4; i++)
        if (!IsMansionInArray(gDifficultySetting, i))
            return false;
    return true;
}

void ExitSelectMansionScreen(void)
{
    gCurrentGameMode = kNoGameMode;
    UpdateMenuBar();
    gSpriteWorldIsValid = false;
    StopAndDisposeMusic();
    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
    DisposeMapScreenSprites();
    gNamesSpriteP = NULL;
}

// ---------------------------------------------------------------------------
// About screen

void DoAboutScreen(void)
{
    SWError err = LoadAndInstallBackgroundSprite(NULL, GetScreensFolderPath(), "about.jpg", -1, false);

    if (err != kNoError)
        MM_FATAL(err);
    SWUpdateSpriteWorld(gSpriteWorldP, true);
    gCurrentGameMode = kAboutScreenMode;
    gSpriteWorldIsValid = true;
    UpdateMenuBar();
    if (!gFullScreenWindow)
        MMSetCursorForSystem();
    MMFlushEvents();
    DoGameFadeToNormalFullAnim(30);
}

void HandleAboutScreenCallback(void)
{
    GiveOSTime();
    if (!IsMouseDown() && !WasAKeyPressed())
        return;
    MMFlushEvents();
    DoGameFadeToBlackFullAnim(30, false);
    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
    SetUpTitleScreen();
}

// ---------------------------------------------------------------------------
// The teaser ("buy the game") screen only appeared in unregistered copies.

Boolean DoTeaserScreen(Boolean quitWhenDone)
{
    (void)quitWhenDone;
    return false;
}

void HandleTeaserScreenCallback(void)
{
    gCanQuit = true;
    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
    SetUpTitleScreen();
}

// ---------------------------------------------------------------------------
// Game over

void DoGameOverScreen(void)
{
    SpritePtr backgroundP;
    Boolean musicWasOn;
    SWError err;

    if (gEditorIsRunning)
        return;

    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
    WriteRoomInfoToFile();
    WriteCStringToFile("GAME OVER\r");
    DoGameFadeToBlackFullAnim(55, true);
    err = LoadAndInstallBackgroundSprite(&backgroundP, GetScreensFolderPath(), "gameover.jpg", -1, false);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledSize(backgroundP, 640, 480);
    err = CreateSpriteFromPictResource(gSpriteWorldP, &gameOverSpriteP, NULL, 0x1fa4, 0x1fa4, 1, 4);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledFactor(gameOverSpriteP, 0.55f, 0.55f);
    gameOverSpriteP->userData = 0xed;
    SWSetSpriteTranslucencyLevel(gameOverSpriteP, 0);
    SWSetSpriteFrameTime(gameOverSpriteP, 0x21);
    SWAddSprite(gTopSpriteLayerP, gameOverSpriteP);
    SWCenterRect(&gameOverSpriteP->destFrameRect, &gSpriteWorldP->visScrollRect);
    SWOffsetRect(&gameOverSpriteP->destFrameRect, 0, -15);
    SWMoveSprite(gameOverSpriteP, gameOverSpriteP->destFrameRect.left, gameOverSpriteP->destFrameRect.top);
    SWSetSpriteLocation(gameOverSpriteP, gameOverSpriteP->destFrameRect.left, 100.0f);
    SWProcessSpriteWorld(gSpriteWorldP);
    SWUpdateSpriteWorld(gSpriteWorldP, true);
    if (gSoundIsTurnedOn) {
        musicWasOn = gMusicIsOn;
        gMusicIsOn = true;
        MyPlayMusic("Game Over Music", gTitleMusicVolume, 0, false);
        gMusicIsOn = musicWasOn;
    }
    gCurrentGameMode = kGameOverScreenMode;
    gSpriteWorldIsValid = true;
    UpdateMenuBar();
    GiveOSTime();
    DoGameFadeToNormal(60);
    MMFlushEvents();
    StartMicrosecTimer();
    spriteHasStartedFadingIn = false;
    printf("Elapsed milliseconds at start of Game Over: %u", (unsigned)GetElapsedMillisecs());
    gPlayingGame = false;
}

void HandleGameOverScreenCallback(void)
{
    SDL_assert(gameOverSpriteP != NULL);
    SWProcessSpriteWorld(gSpriteWorldP);
    SWAnimateSpriteWorld(gSpriteWorldP);
    if (!spriteHasStartedFadingIn && GetElapsedMillisecs() > 1000) {
        SWSetSpriteFrameProc(gameOverSpriteP, FadeInFrameProc);
        spriteHasStartedFadingIn = true;
    }
    if (!IsMouseDown() && !WasAKeyPressed() && GetElapsedMillisecs() < 8501)
        return;
    printf("Elapsed milliseconds at end of Game Over: %u", (unsigned)GetElapsedMillisecs());
    FinishGameOverOrBeatMansionScreen(true);
}

// ---------------------------------------------------------------------------
// Mansion complete

/// Shows the mansion's stats. With afterSlideshow (after the ending), stays in the current mode.
void DoBeatTheMansionScreen(Boolean afterSlideshow)
{
    SpritePtr spriteP;
    MMTextStyleRec style;
    SWRGBColor red = { 0xffff, 4, 4 };
    SWRect rect, captureRect;
    char text[255];
    Boolean musicWasOn;
    SWError err;

    if (gEditorIsRunning)
        return;

    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
    WriteRoomInfoToFile();
    WriteCStringToFile("BEAT MANSION!!!\r");
    if (gFileIsOpen)
        fprintf(gOutputFileP, "Total time spent in mansion: %ld minutes and %ld seconds.\r",
                gTimeSpentInMansion / 60, gTimeSpentInMansion % 60);
    if (gIgnoreCheating)
        gDidCheat = false;
    if (!gMansionIsCustomMansion && gCurrentMansionNumber != -2 && !gDidCheat) {
        SDL_assert(gCurrentMansionNumber >= 0);
        AddMansionToArray(gDifficultySetting, gCurrentMansionNumber);
    }
    if (!gDidCheat)
        StatsNoteMansionBeaten();
    if (gCurrentMansionNumber == -2)
        gPlayTutorialMansion = false;
    WriteProfileToDisk();

    err = LoadAndInstallBackgroundSprite(&spriteP, GetScreensFolderPath(), "mansioncomplete.jpg", -1, false);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledSize(spriteP, 640, 480);

    // The text is drawn with rect.bottom as the baseline.
    SetTextStyle(&style, "\022Comic Sans MS Bold", 0, 22, 0);
    SWSetRect(&rect, 395, 258, 640, 280);
    captureRect.top = rect.bottom - style.size;
    captureRect.left = rect.left - 50;
    captureRect.bottom = 480;
    captureRect.right = 640;
    MyDrawCTextJustified("Mansion:", &style, &rect, &rgbWhiteColor);
    SWOffsetRect(&rect, 0, 23);
    MyDrawCTextJustified(gMansionFilename, &style, &rect, &rgbWhiteColor);
    snprintf(text, sizeof(text), "Time: %ld:%s%ld", gTimeSpentInMansion / 60,
             gTimeSpentInMansion % 60 < 10 ? "0" : "", gTimeSpentInMansion % 60);
    SWOffsetRect(&rect, 0, 40);
    MyDrawCTextJustified(text, &style, &rect, &rgbWhiteColor);
    SWOffsetRect(&rect, 0, 40);
    snprintf(text, sizeof(text), "Total Score: %ld", gScore);
    MyDrawCTextJustified(text, &style, &rect, &rgbWhiteColor);
    SWOffsetRect(&rect, 0, 23);
    snprintf(text, sizeof(text), "Secrets found: %d%%", GetPercentSecrets());
    MyDrawCTextJustified(text, &style, &rect, &rgbWhiteColor);
    SWOffsetRect(&rect, 0, 40);
    snprintf(text, sizeof(text), "Lives left: %d", gNumLivesLeft);
    MyDrawCTextJustified(text, &style, &rect, &rgbWhiteColor);
        // relay co-op: who brought home the loot
    GetLootSplitText(text, sizeof(text));
    if (text[0] != '\0') {
        MMTextStyleRec small;
        SetTextStyle(&small, "\022Comic Sans MS Bold", 0, (short)(GetNumPlayers() > 2 ? 12 : 16), 0);
        SWOffsetRect(&rect, -40, 30);
        rect.right = 640;
        MyDrawCTextJustified(text, &small, &rect, &rgbWhiteColor);
    }
    if (gDidCheat) {
        rect.bottom = 240;
        captureRect.top = 240 - style.size;
        MyDrawCTextJustified("USED CHEATS", &style, &rect, &red);
    }

    // Bake the text into one sprite.
    MMRenderSceneForWorkAreaSprite(gSpriteWorldP);
        // (the original scaled captureRect to window pixels; captures here use design coordinates)
    err = SWCreateSpriteFromWorkArea(gSpriteWorldP, &spriteP, NULL, &captureRect);
    if (err == kNoError) {
        SWDisposeAllSpritesInLayer(gTextSpriteLayerP);
        SWAddSprite(gTextSpriteLayerP, spriteP);
    }
    if (!gFullScreenWindow)
        MMSetCursorForGame();
    SWProcessSpriteWorld(gSpriteWorldP);
    SWUpdateSpriteWorld(gSpriteWorldP, true);
    if (!afterSlideshow) {
        if (gSoundIsTurnedOn) {
            musicWasOn = gMusicIsOn;
            gMusicIsOn = true;
            MyPlayMusic("Beat The Mansion", gTitleMusicVolume, 0, false);
            gMusicIsOn = musicWasOn;
        }
        gCurrentGameMode = kBeatTheMansionMode;
        gSpriteWorldIsValid = true;
        UpdateMenuBar();
        GiveOSTime();
        DoGameFadeToNormalFullAnim(180);
        StartMicrosecTimer();
        MMFlushEvents();
        gBeatMansionScreenWasDismissed = false;
    }
    puts("Drawing beat mansion screen.");
}

SpritePtr DrawTextUsingSprite(SpriteWorldPtr spriteWorldP, const char *text, void *unused, SWRect *rectP)
{
    SpritePtr textP = NewBitmapTextSprite(rectP->right - rectP->left);

    (void)unused;
    if (textP != NULL) {
        SWSetSpriteScaledFactor(textP, (float)(spriteWorldP->designWidth / 640) * 0.3f,
                                (float)(spriteWorldP->designWidth / 480) * 0.3f);
        SetBMTextSpriteText(textP, text);
        SWAddSprite(gCustomDialogSpriteLayerP, textP);
        SWSetSpriteLocation(textP, rectP->left, rectP->top);
    }
    return textP;
}

void HandleBeatTheMansionScreenCallback(void)
{
    SWProcessSpriteWorld(gSpriteWorldP);
    SWAnimateSpriteWorld(gSpriteWorldP);
    if (gBeatMansionScreenWasDismissed)
        FinishGameOverOrBeatMansionScreen(false);
}

short GetPercentSecrets(void)
{
    if (gTotalSecretsInMansion < 1)
        return 100;
    return (short)((gNumSecretsFound * 100) / gTotalSecretsInMansion);
}

// ---------------------------------------------------------------------------
// Ending slideshow (after Spider Palace)

static void ShowEndingPicture(const char *fileName, Boolean fitToDesign)
{
    SpritePtr pictureP;
    SWError err;

    SWDisposeAllSpritesInLayer(gBackgroundScreenSpriteLayerP);
    err = LoadAndInstallBackgroundSprite(&pictureP, GetScreensFolderPath(), fileName, -1, false);
    if (err != kNoError && fitToDesign)
        err = LoadAndInstallBackgroundSprite(&pictureP, GetScreensFolderPath(), "beatgame3.jpg", -1, false);
    if (err != kNoError)
        MM_FATAL(err);
    if (!fitToDesign)
        SetBackgroundToDesignSize(pictureP);
    else {
        SWSetSpriteScaledFactor(pictureP, (float)(gSpriteWorldP->designWidth / 1280),
                                (float)(gSpriteWorldP->designHeight / 960));
        SWCenterSpriteInRect(pictureP, &gSpriteWorldP->workAreaRect);
    }
    GiveOSTime();
    SWUpdateSpriteWorld(gSpriteWorldP, true);
}

void DoBeatTheGameSlideshow(void)
{
    SpritePtr pictureP;
    SWError err;

    if (gEditorIsRunning)
        return;
    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
    err = LoadAndInstallBackgroundSprite(&pictureP, GetScreensFolderPath(), "beatgame1.jpg", -1, false);
    if (err != kNoError)
        MM_FATAL(err);
    SetBackgroundToDesignSize(pictureP);
    if (!gFullScreenWindow) {
        MMRemoveGameCursor();
        MMShieldCursor();
    }
    SWProcessSpriteWorld(gSpriteWorldP);
    SWUpdateSpriteWorld(gSpriteWorldP, true);
    gMusicWasOn = gMusicIsOn;
    gMusicIsOn = true;
    MyPlayMusic("Beat The Game Intro", gTitleMusicVolume, 0, false);
    gCurrentGameMode = kBeatTheGameMode;
    gSpriteWorldIsValid = true;
    UpdateMenuBar();
    GiveOSTime();
    DoGameFadeToNormal(180);
    MMFlushEvents();
    gCurrentEndingScreen = 1;
    StartMicrosecTimer();
}

void HandleBeatTheGameCallback(void)
{
    SWProcessSpriteWorld(gSpriteWorldP);
    SWAnimateSpriteWorld(gSpriteWorldP);
    if (IsMusicDone())
        MyPlayMusic("Beat The Game Main", gTitleMusicVolume, 0, true);

    switch (gCurrentEndingScreen) {
        case 1:
            if (GetElapsedSeconds() > 7) {
                DoGameFadeToBlack(55, false);
                ShowEndingPicture("beatgame2.jpg", false);
                DoGameFadeToNormal(55);
                StartMicrosecTimer();
                gCurrentEndingScreen++;
            }
            break;
        case 2:
            if (GetElapsedSeconds() > 15) {
                DoGameFadeToBlack(55, false);
                ShowEndingPicture("beatgame3.png", true);
                DoGameFadeToNormal(55);
                StartMicrosecTimer();
                gCurrentEndingScreen++;
            }
            break;
        case 3:
            if (GetElapsedSeconds() > 3) {
                DoGameFadeToBlack(55, false);
                ShowEndingPicture("beatgame4.jpg", false);
                DoGameFadeToNormal(180);
                MMFlushEvents();
                gBeatGameScreenWasDismissed = false;
                gCurrentEndingScreen++;
            }
            break;
        case 4:
            if (IsMouseDown() || gBeatGameScreenWasDismissed) {
                if (!gMusicWasOn) {
                    MyFadeMusicNow();
                    gMusicIsOn = gMusicWasOn;
                }
                DoGameFadeToBlack(55, false);
                DoBeatTheMansionScreen(false);
            }
            break;
    }
}

/// After game over (gameOver true) or a beaten mansion: high scores, then the map or title.
void FinishGameOverOrBeatMansionScreen(Boolean gameOver)
{
    Boolean gotHighScore = false;

    gCurrentGameMode = kNoGameMode;
    gCurrentlyInMansion = false;
    MMFlushEvents();
    StopAndDisposeMusic();
    if (!gMansionIsCustomMansion && gCurrentMansionNumber != -2 && !gDidCheat)
        gotHighScore = CheckForHighScore(gCurrentMansionNumber, gDifficultySetting, gScore, GetPercentSecrets(),
                                         !gameOver);
    DoGameFadeToBlack(55, true);
    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
    if (gDidCheat)
        gotHighScore = false;

    if (!gotHighScore) {
        if (!gMansionIsCustomMansion && !gameOver && gCurrentMansionNumber != 10)
            SetUpSelectMansionScreen(gCurrentMansionNumber < 5 ? 1 : 2);
        else
            SetUpTitleScreen();
    }
    else
        SetUpHighScoresScreen(gCurrentMansionNumber, true, !gameOver && gCurrentMansionNumber != 10);
}

// ---------------------------------------------------------------------------
// Shared screen helpers

void AddBackButtonToScreen(SpriteLayerPtr layerP, short raise)
{
    SpritePtr backP = NewButtonSprite(0xd6, kBackButton);

    if (backP == NULL)
        return;
    SetUpButtonSprite(backP, 0, 1, 2, 0);
    InstallButtonProc(backP, MMStandardButtonProc);
    SetButtonPushedOffset(backP, 1, 1);
    AdjustCollisionRectForOffset(backP);
    SWAddSprite(layerP, backP);
    SWSetSpriteLocation(backP, 2.0f, ((480.0f - SWGetSpriteHeight(backP)) - 2.0f) - (float)raise);
}

void AddCustomCursorToScreen(void)
{
    SpritePtr cursorP;

    if (gSpriteWorldP == NULL || !gMainSpritesAreLoaded)
        return;
    cursorP = SWGetNextSprite(gCustomCursorSpriteLayerP, NULL);
    if (cursorP == NULL) {
        cursorP = NewMiscSprite(0xdb);
        if (cursorP != NULL) {
            SWAddSprite(gCustomCursorSpriteLayerP, cursorP);
            SWSetSpriteMoveProc(cursorP, CustomCursorMoveProc);
            SWSetSpriteNonScrolling(cursorP, true);
            SWSetSpriteScaledFactor(cursorP, (float)(gSpriteWorldP->designWidth / 1280),
                                    (float)(gSpriteWorldP->designHeight / 960));
            CustomCursorMoveProc(cursorP);
        }
    }
    SWUnpauseSpriteLayer(gCustomCursorSpriteLayerP);
}

void MMStandardButtonProc(SpritePtr buttonP, short buttonIndex, short newState, short oldState)
{
    if (newState == oldState)
        return;
    PlayButtonSound(newState, oldState);
    // Options dialog tabs dim once selected.
    if ((buttonIndex == 0xdf || buttonIndex == 0xe0 || buttonIndex == 0xe3 || buttonIndex == 0xe4) && oldState == kButtonPushed && newState == kButtonHighlighted)
        SetButtonDimmedMode(buttonP, true);
}

void MMTitleScreenButtonProc(SpritePtr buttonP, short buttonIndex, short newState, short oldState)
{
    (void)buttonIndex;
    if (newState == oldState)
        return;
    PlayButtonSound(newState, oldState);
    if (newState == kButtonPushed)
        SWSetSpriteColor(buttonP, 0x8fff, 0x8fff, 0x8fff);
    else
        SWSetSpriteColor(buttonP, 0xffff, 0xffff, 0xffff);
    MakeButtonBounceWhenSelected(buttonP, newState, oldState, 0.4f, 1.0f, 1.5f, 2.2f);
}

void MMTabButtonProc(SpritePtr buttonP, short buttonIndex, short newState, short oldState)
{
    if (newState == oldState)
        return;
    PlayButtonSound(newState, oldState);
    if (newState == kButtonPushed)
        SWSetSpriteColor(buttonP, 0x9fff, 0x9fff, 0x9fff);
    else
        SWSetSpriteColor(buttonP, 0xffff, 0xffff, 0xffff);
    // The "Buy" tab (10) lifted its glow sprite; not present in this build.
    if (buttonIndex == 10 && buttonP->misc2.sprite != NULL) {
        if (newState == kButtonHighlighted)
            SWOffsetSprite(buttonP->misc2.sprite, 0.0f, -16.0f);
        else if (oldState == kButtonHighlighted)
            SWOffsetSprite(buttonP->misc2.sprite, 0.0f, 16.0f);
    }
}

void DifficultyScreenButtonProc(SpritePtr buttonP, short buttonIndex, short newState, short oldState)
{
    if (newState == kButtonHighlighted || newState == kButtonPushed) {
        SDL_assert(gIllustrationSpriteP != NULL);
        SWSetSpriteVisible(gIllustrationSpriteP, true);
        SWSetCurrentFrameIndex(gIllustrationSpriteP, buttonIndex);
    }
    if (newState == oldState)
        return;
    PlayButtonSound(newState, oldState);
    MakeButtonBounceWhenSelected(buttonP, newState, oldState, 0.2f, 0.2f, 1.5f, 2.2f);
}

/// Highlighted buttons wobble in size; stepH/stepV are degrees per frame.
void MakeButtonBounceWhenSelected(SpritePtr spriteP, short newState, short oldState, float extraScale,
                                  float amplitude, float stepH, float stepV)
{
    ButtonSpritePtr buttonP = (ButtonSpritePtr)spriteP;

    if (buttonP->bounceBaseScale == -1.0f) {
        buttonP->bounceBaseScale = spriteP->drawData->scaledWidthFactor;
        if (buttonP->bounceBaseScale == -1.0f)
            buttonP->bounceBaseScale = 1.0f;
        SWSetSpriteScalingMode(spriteP, true);
    }
    buttonP->bounceExtraScale = extraScale;
    buttonP->bounceStepH = stepH;
    buttonP->bounceStepV = stepV;
    buttonP->bounceAmplitude = amplitude;
    if (newState == kButtonHighlighted) {
        SWSetSpriteMoveProc(spriteP, MMBounceButtonMoveProc);
        buttonP->bounceAngleH = 90.0f;
        buttonP->bounceAngleV = 120.0f;
    }
    else if (oldState == kButtonHighlighted) {
        SWSetSpriteMoveProc(spriteP, NULL);
        if (newState != kButtonPushed)
            SWSetSpriteScaledFactor(spriteP, buttonP->bounceBaseScale, buttonP->bounceBaseScale);
    }
}

void MMBounceButtonMoveProc(SpritePtr spriteP)
{
    ButtonSpritePtr buttonP = (ButtonSpritePtr)spriteP;
    float base = buttonP->bounceBaseScale + buttonP->bounceExtraScale;

    buttonP->bounceAngleH += buttonP->bounceStepH;
    if (buttonP->bounceAngleH >= 360.0f)
        buttonP->bounceAngleH -= 360.0f;
    buttonP->bounceAngleV += buttonP->bounceStepV;
    if (buttonP->bounceAngleV >= 360.0f)
        buttonP->bounceAngleV -= 360.0f;
    SWSetSpriteScaledFactor(spriteP,
                            (float)(buttonP->bounceAmplitude * (SWCos(buttonP->bounceAngleH) / 12.0) + base),
                            (float)(buttonP->bounceAmplitude * (SWCos(buttonP->bounceAngleV) / 6.0) + base));
}
