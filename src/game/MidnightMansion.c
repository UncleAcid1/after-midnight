/*
 * Midnight Mansion HD — startup, main loop, preferences, game flow, fades, cursors.
 * Reconstructed from "Midnight Mansion.c" of the Mac build (v1.0.1b).
 *
 * Changes from the original:
 *  - Registration, episode licensing, demo limits and the mansion checksum/tamper
 *    checks are removed; both episodes are always available.
 *  - The mansion editor is not part of this build.
 */

#include "MMGame.h"

// ---------------------------------------------------------------------------
// Globals

Boolean gDoOpeningMusic = true;
Boolean gShowEpisode2LinkedMessage = false;
Boolean gShowEpisode1LinkedMessage = false;
long    gLightSaberFloorTranslucency = 0xFFFF;
Boolean gIgnoreCheating = false;
static Boolean worldRectIsSetForGame = false;

/// Tint applied to alternate tile color variations (RGB per variation).
static const float tileColorVars[6][3] = {
    { 0x1.c140fep-1f, 0x1.ff5a96p-1f, 0x1.f48bcap-1f },
    { 0x1.be5d9ep-1f, 0x1.edaab4p-1f, 0x1.edd16p-1f },
    { 0x1.c3c83ap-1f, 0x1.d8972p-1f, 0x1.ee006ep-1f },
    { 0x1.e1529p-1f, 0x1.ec5ed4p-1f, 0x1.d81344p-1f },
    { 0x1.dd98e2p-1f, 0x1.bf0bf2p-1f, 0x1.e81dcp-1f },
    { 0x1.e8aaa8p-1f, 0x1.c50204p-1f, 0x1.be0696p-1f }
};

static Boolean tileFlagsWereInitialized = false;
static Boolean gamepadInitialized = false;
static Boolean initedSubsystem = false;
Boolean gDidFadeAllMonitors = false;
Boolean gCustomCursorIsVisible = false;
Boolean gCursorIsShielded = false;

short   gAppResFile, gGraphicsFile, gSpritesFile, gSoundsFile;
Boolean gArrowKeysAreEnabledForTesting, gArrowKeysEnabled;
short   gBackgroundMusicVolume, gTitleMusicVolume, gSoundVolume;
Boolean gBeatGameScreenWasDismissed, gBeatMansionScreenWasDismissed, gBeatTheMansion;
Boolean gCanControlWhileJumping, gCanPlayBothEpisodes, gCanQuit, gCanUseLadderJump;
short   gCurConveyorBeltDirection, gOldConveyorBeltDirection;
Boolean gCurMansionUsesWaterSound;
short   gCurNumStatsSlots;
char    gCurOpenMansionFileName[256];
char    gCurOpenMansionFilePath[600];
short   gCurrentFPSValue;
long    gCurrentGameMode;
short   gCurrentLevel;
short   gCurrentMansionNumber;
short   gCurrentMusicIndex, gLastMusicIndex;
char    gCurrentPlayerName[256];
Boolean gCurrentPlayerNameExists;
Boolean gCurrentRoomHasLava, gCurrentRoomHasWind;
long    gCurrentSaveIndex;
float   gCurrentWindDelta, gMaxWindDelta, gMinWindDelta, gTargetWindDelta, gWindDeltaChange;
long    gNextWindChange;
Boolean gCurrentlyInMansion;
Boolean gCursorIsInWindow;
SWRect  gCursorShieldRect;
Boolean gCustomDialogIsVisible;
char    gCustomMansionFolderPath[800];
Str255  gDefaultHighScoreName;
short   gDelayUntilUpdatingFPSValue;
Boolean gDemoMode, gDialogBoxIsOpen, gDidCheat;
SpritePtr gDifficultyScreenSpriteArray[5];
char    gDifficultySetting;
Boolean gDisableCoinGlows, gDisableDynamicShadows, gDisableKeyGlows;
short   gDisappearingFloorAnimDelay, gDisappearingFloorMode, gDisappearingFloorWait;
Boolean gDisplayFPSAndLevelResID, gDoFrameSkipping;
Boolean gEasyJumping, gEditorIsRunning, gEndGame;
Boolean gEpisode1IsUnlocked, gEpisode2IsUnlocked;
Boolean gEscapeKey;
Boolean gFamilyFriendly, gFasterInGameMap;
Boolean gFullScreenWindow, gFullScreenWindowPrefs;
Boolean gGameIsPaused, gGameOver, gGameTimerIsPaused;
Boolean gGamepad[12];           ///< gamepad state, same layout as gKeys
static SDL_Joystick *gJoystick;
Boolean gGoldFloorIsActive, gSilverFloorIsActive;
Boolean gHasShownOrganScreen, gHeroIsOnConveyorBelt, gHeroLandedThisFrame;
float   gHorizontalBoost, gJumpChange, gJumpSpeed;
Boolean gKeyIsHiddenInRoom;
KeyPrefsRec gKeyPrefs;
Boolean gKeys[16];
short   gLastLevelExited;
short   gLastLevelPositionWasSaved;     // level number of the last restart position
short   gLastOpenDialog;
char    gLastSaveFilename[400];
long    gLastSavePedestalObjectID;
short   gLavaSnd;
Boolean gLayersArePausedForSaveAnim;
short   gLightSaberCurTranslucencyOffset, gLightSaberFloorAnimDelay, gLightSaberFloorMode, gLightSaberFloorWait;
long    gLightSaberFloorCurTranslucency;
Boolean gLimitFullscreenResolution;
Boolean gMMIsActive, gMainSpritesAreLoaded;
char    gMansionFilename[255];
Boolean gMansionIsCustomMansion;
char    gMansionName[256];
SpritePtr gMapScreenSpriteArray[16];
Boolean gMenuBarIsVisible;
long    gModifiers;
Boolean gMusicIsOn, gSofterMusic, gSoundIsTurnedOn;
long    gMyOldNumFrames, gMyOldTicks, gNumFrames, gNumFramesSinceEnteringRoom;
short   gNumItemsBeingCarried, gNumLivesLeft;
long    gNumSecretsFound, gTotalSecretsInMansion, gNumWeatherSpritesOnGround;
ObjectsCarriedRec gObjects;
short   gOldBackgroundPictResID;
long    gScore, gOldScore;
void   *gOpeningMusicRefNum;
SpritePtr gOrganScreenSpriteArray[3];
Boolean gOrganScreenWasDismissed;
short   gOrigTileIDs[0xf7a];
Boolean gPheliosDistribution, gPlayTutorialMansion, gPlayingGame, gPlayingGameMovie;
short   gPrevWindowHeight, gPrevWindowWidth;
Boolean gQuitGame;
SWRGBColor gRGBBlackColor, gRGBWhiteColor, gWindowColor;
Boolean gReadingSignWasDismissed, gRecordMovies, gRemovedGlowsAndShadows, gRestartLevel;
Boolean gRoomIsDark, gRoomWasDark;
SWRect  gRoomMoveBoundsRect;
long    gRunningFrameCount;
long    gSaveCurrentGameMode;
Boolean gSaveCustomCursorIsVisible, gSaveSystemCursorIsShielded, gSaveSystemCursorIsVisible;
TileChangeProcPtr gSaveTileChangeProc;
Boolean gShowSecrets;
SpritePtr gSpriteArray[kNumSpriteArrayEntries];
DrawProcPtr gSpriteDrawProc, gTranslucentMaskDrawProc;
Boolean gSpriteWorldIsValid;
SpriteWorldPtr gSpriteWorldP;
Boolean gStartRecordingMovie;
short   gStartingCol, gStartingRow, gStartingSpriteState, gStartingVineTileLayer;
Boolean gStartingFacingRight;
short   gStatsHeight;
short gStatsItemArray[13];
SpritePtr gStatsItemSpriteArray[13];
SpritePtr gStatsSpriteArray[2];
SpriteWorldPtr gStatsSpriteWorldP;
Boolean gStatuesAreDoneFading, gStoryScreenWasDismissed, gStretchToFit;
Boolean gSuspendedGameExists;
long    gTimeSpentInMansion;
SpritePtr gTitleScreenSpriteArray[14];
Boolean gTriggerPresentInRoom;
Boolean gUseCustomCursor, gUseGamePad, gUseHaloSprite, gUseOutputFile, gUsingEditor;
Boolean gWasFullscreenWhenMinimized, gWereAnyTilesReplaced, gWindowIsMinimized;
short   gWindowedResolutionX, gWindowedResolutionY;
static Boolean layersWerePausedForSaveAnim;
SWPoint oldGlobalMouseLoc, oldMouseLoc;
static short tileFlags[0x5a2];  ///< per tile: -1 not loaded, 0 standard image, 1 replaced by the mansion

SpriteLayerPtr gBackgroundScreenSpriteLayerP, gRearBackgroundPictureSpriteLayerP,
    gBackgroundPictureSpriteLayerP, gLowerTileFringeSpriteLayerP, gBottomSpriteLayerP,
    gExitSpriteLayerP, gBackgroundSpriteLayerP, gShadowSpriteLayerP, gBackLavaSplashSpriteLayerP,
    gFrontLavaSplashSpriteLayerP, gLavaGlowSpriteLayerP, gSignSpriteLayerP, gGlowSpriteLayerP,
    gLanternGlowSpriteLayerP, gTorchSpriteLayerP, gFlameSpriteLayerP, gCandleSpriteLayerP,
    gCandleHolderSpriteLayerP, gFloorCandleStandSpriteLayerP, gLeverSpriteLayerP, gSnakeSpriteLayerP,
    gMedusaSpriteLayerP, gGhostSpriteLayerP, gEyeballSpriteLayerP, gBirdSpriteLayerP,
    gWebTrappedSpriteLayerP, gSkeletonBonesSpriteLayerP, gEnemySpriteLayerP, gLadybugSpriteLayerP,
    gSpyVsSpySpriteLayerP, gBigSpiderSpriteLayerP, gDyingMedusaSpriteLayerP,
    gVertMovingPlatformSpriteLayerP, gHorizMovingPlatformSpriteLayerP, gFallingPlatformSpriteLayerP,
    gMovingPlatformBlockerSpriteLayerP, gDisappearingBlockSpriteLayerP, gHeroOnPlatformSpriteLayerP,
    gForegroundSpriteLayerP, gSavePedestalSpriteLayerP, gStatueSpriteLayerP, gBrickSwitchSpriteLayerP,
    gTrapDoorSpriteLayerP, gOpenTrapDoorSpriteLayerP, gDoorSpriteLayerP, gOpenedDoorSpriteLayerP,
    gBloodDripSpriteLayerP, gObjectSpriteLayerP, gPuffOfSmokeSpriteLayerP,
    gBottomCircleOfLightSpriteLayerP, gHeroSpriteLayerP, gKeyGlowSpriteLayerP,
    gQuestionMarkSpriteLayerP, gTopCircleOfLightSpriteLayerP, gDeadHeroSpriteLayerP,
    gZapperGlowSpriteLayerP, gZapperNodeSpriteLayerP, gZapperBeamSpriteLayerP, gRainSnowSpriteLayerP,
    gHiddenTriggerSpriteLayerP, gMiscTriggerSpriteLayerP, gSecretIndicatorSpriteLayerP,
    gExtrasOverlaySpriteLayerP,
    gTopSpriteLayerP, gSignContentsSpriteLayerP, gTipContentsSpriteLayerP, gFPSDisplaySpriteLayerP,
    gPausedSpriteLayerP, gCustomDialogSpriteLayerP, gOptionsDialogPane1LayerP,
    gOptionsDialogPane2LayerP, gOptionsDialogPane3LayerP, gOptionsDialogPane4LayerP, gTextSpriteLayerP,
    gCustomCursorSpriteLayerP, gHaloSpriteLayerP,
    gHiddenTriggerItemsSpriteLayerP, gFadeToBlackSpriteLayerP,
    gDigitsStatsSpriteLayerP, gStatsBackgroundSpriteLayerP, gStatsFadeSpriteLayerP,
    gStatsMapTextSpriteLayerP, gStatsSpriteLayerP;

// ---------------------------------------------------------------------------
// Startup

int main(int argc, char *argv[])
{
    SW_UNUSED(argc);
    SW_UNUSED(argv);
#ifdef MM_PS3
    PS3PlatformInit(argc, argv);    // folders, the log on the USB drive
#endif
    InitVariables();
    Randomize();
    CreateAppSupportDirectories();
    CheckForLicenseAndDataFiles();
    OpenGameSupportFiles();
    InitSpriteArray();
    SWSetCleanUpFunction(MyCleanUpFunction);
    LoadPreferences();
    StartUpGameCarbon();
    ShutDown();
    return 0;
}

/// The original looked for episode license files here. Both episodes are unlocked.
void CheckForLicenseAndDataFiles(void)
{
    gEpisode1IsUnlocked = true;
    gEpisode2IsUnlocked = true;
    gCanPlayBothEpisodes = true;
}

void CreateAppSupportDirectories(void)
{
    MMCreateDirectory(GetAppSupportPath(""), "Player Profiles");
    MMCreateDirectory(GetAppSupportPath(""), "Saved Games");
    MMCreateDirectory(GetAppSupportPath(""), "Suspended Game Data");
    MMCreateDirectory(GetAppSupportPath(""), "User Movies");
    MMCreateDirectory(GetAppSupportPath(""), "Mansion Data");
}

void InitVariables(void)
{
    gEditorIsRunning = false;
    gStatsHeight = 0x30;
    gQuitGame = false;
    gFileIsOpen = false;
    gCanUseLadderJump = true;
    gUseOutputFile = false;
    gSpriteWorldIsValid = false;
    gMainSpritesAreLoaded = false;
    gCurrentGameMode = kNoGameMode;
    gDialogBoxIsOpen = false;
    gCustomDialogIsVisible = false;
    gMenuBarIsVisible = true;
    gSpriteWorldP = NULL;
    gStatsSpriteWorldP = NULL;
    gLastOpenDialog = 0x10;
    gPlayingGame = false;
    gMMIsActive = true;
    gUseHaloSprite = true;
    gArrowKeysAreEnabledForTesting = false;
    gAppResFile = -1;
    gGraphicsFile = -1;
    gSpritesFile = -1;
    gSoundsFile = -1;
    gOpeningMusicRefNum = NULL;
    gCurrentMusicIndex = 0;
    gCanQuit = true;
    gDemoMode = false;
    gFullScreenWindow = false;
    gWindowIsMinimized = false;
    gWasFullscreenWhenMinimized = false;
    gPheliosDistribution = false;
    gHasShownOrganScreen = false;
    gCurrentLevel = 1;
    gNumItemsBeingCarried = 0;
    gDisplayFPSAndLevelResID = false;
    gStartRecordingMovie = false;
    gLayersArePausedForSaveAnim = false;
    gTotalSecretsInMansion = 0;
    gCursorIsInWindow = true;
    gUseCustomCursor = true;
    gJumpSpeed = 0x1.39999ap+3f;        // 9.8
    gJumpChange = 0x1.19999ap-1f;       // 0.55
    gHorizontalBoost = 0x1.147ae2p+0f;  // 1.08
    gDifficultySetting = 1;
    gFasterInGameMap = false;
}

static void CreateLayer(SpriteLayerPtr *layerPP)
{
    MM_FATAL_IF(SWCreateSpriteLayer(layerPP));
}

/// Creates every sprite layer and adds them to the world in drawing order.
void CreateAndAddSpriteLayers(void)
{
    SpriteLayerPtr *creationOrder[] = {
        &gBackgroundScreenSpriteLayerP, &gRearBackgroundPictureSpriteLayerP, &gBackgroundPictureSpriteLayerP,
        &gLowerTileFringeSpriteLayerP, &gBottomSpriteLayerP, &gExitSpriteLayerP, &gBackgroundSpriteLayerP,
        &gShadowSpriteLayerP, &gBackLavaSplashSpriteLayerP, &gFrontLavaSplashSpriteLayerP,
        &gLavaGlowSpriteLayerP, &gSignSpriteLayerP, &gGlowSpriteLayerP, &gLanternGlowSpriteLayerP,
        &gTorchSpriteLayerP, &gFlameSpriteLayerP, &gCandleSpriteLayerP, &gCandleHolderSpriteLayerP,
        &gFloorCandleStandSpriteLayerP, &gLeverSpriteLayerP, &gSnakeSpriteLayerP, &gMedusaSpriteLayerP,
        &gGhostSpriteLayerP, &gEyeballSpriteLayerP, &gBirdSpriteLayerP, &gWebTrappedSpriteLayerP,
        &gSkeletonBonesSpriteLayerP, &gEnemySpriteLayerP, &gLadybugSpriteLayerP, &gSpyVsSpySpriteLayerP,
        &gBigSpiderSpriteLayerP, &gDyingMedusaSpriteLayerP, &gVertMovingPlatformSpriteLayerP,
        &gHorizMovingPlatformSpriteLayerP, &gFallingPlatformSpriteLayerP,
        &gMovingPlatformBlockerSpriteLayerP, &gDisappearingBlockSpriteLayerP, &gHeroOnPlatformSpriteLayerP,
        &gForegroundSpriteLayerP, &gSavePedestalSpriteLayerP, &gStatueSpriteLayerP,
        &gBrickSwitchSpriteLayerP, &gTrapDoorSpriteLayerP, &gOpenTrapDoorSpriteLayerP, &gDoorSpriteLayerP,
        &gOpenedDoorSpriteLayerP, &gBloodDripSpriteLayerP, &gObjectSpriteLayerP, &gPuffOfSmokeSpriteLayerP,
        &gBottomCircleOfLightSpriteLayerP, &gHeroSpriteLayerP, &gKeyGlowSpriteLayerP,
        &gQuestionMarkSpriteLayerP, &gTopCircleOfLightSpriteLayerP, &gDeadHeroSpriteLayerP,
        &gZapperGlowSpriteLayerP, &gZapperNodeSpriteLayerP, &gZapperBeamSpriteLayerP,
        &gRainSnowSpriteLayerP, &gHiddenTriggerSpriteLayerP, &gMiscTriggerSpriteLayerP,
        &gSecretIndicatorSpriteLayerP, &gTopSpriteLayerP, &gExtrasOverlaySpriteLayerP, &gSignContentsSpriteLayerP,
        &gTipContentsSpriteLayerP, &gFPSDisplaySpriteLayerP, &gPausedSpriteLayerP,
        &gCustomDialogSpriteLayerP, &gOptionsDialogPane1LayerP, &gOptionsDialogPane2LayerP,
        &gOptionsDialogPane3LayerP, &gOptionsDialogPane4LayerP,
        &gTextSpriteLayerP, &gCustomCursorSpriteLayerP, &gHaloSpriteLayerP,
        &gHiddenTriggerItemsSpriteLayerP, &gFadeToBlackSpriteLayerP
    };
    SpriteLayerPtr *drawingOrder[] = {
        &gBackgroundScreenSpriteLayerP, &gRearBackgroundPictureSpriteLayerP, &gBackgroundPictureSpriteLayerP,
        &gShadowSpriteLayerP, &gLowerTileFringeSpriteLayerP, &gBottomSpriteLayerP, &gExitSpriteLayerP,
        &gBackgroundSpriteLayerP, &gGlowSpriteLayerP, &gLanternGlowSpriteLayerP, &gTorchSpriteLayerP,
        &gCandleHolderSpriteLayerP, &gCandleSpriteLayerP, &gFlameSpriteLayerP, &gTrapDoorSpriteLayerP,
        &gOpenTrapDoorSpriteLayerP, &gDisappearingBlockSpriteLayerP, &gVertMovingPlatformSpriteLayerP,
        &gFallingPlatformSpriteLayerP, &gHorizMovingPlatformSpriteLayerP, &gMovingPlatformBlockerSpriteLayerP,
        &gHeroOnPlatformSpriteLayerP, &gFloorCandleStandSpriteLayerP, &gSignSpriteLayerP,
        &gLeverSpriteLayerP, &gForegroundSpriteLayerP, &gSavePedestalSpriteLayerP,
        &gBottomCircleOfLightSpriteLayerP, &gStatueSpriteLayerP, &gTopCircleOfLightSpriteLayerP,
        &gBrickSwitchSpriteLayerP, &gBackLavaSplashSpriteLayerP, &gDeadHeroSpriteLayerP,
        &gRainSnowSpriteLayerP, &gDoorSpriteLayerP, &gOpenedDoorSpriteLayerP, &gBloodDripSpriteLayerP,
        &gSnakeSpriteLayerP, &gWebTrappedSpriteLayerP, &gSkeletonBonesSpriteLayerP, &gEnemySpriteLayerP,
        &gLadybugSpriteLayerP, &gSpyVsSpySpriteLayerP, &gEyeballSpriteLayerP, &gBirdSpriteLayerP,
        &gBigSpiderSpriteLayerP, &gKeyGlowSpriteLayerP, &gObjectSpriteLayerP, &gPuffOfSmokeSpriteLayerP,
        &gZapperGlowSpriteLayerP, &gZapperBeamSpriteLayerP, &gZapperNodeSpriteLayerP,
        &gMedusaSpriteLayerP, &gDyingMedusaSpriteLayerP, &gGhostSpriteLayerP, &gHeroSpriteLayerP,
        &gHaloSpriteLayerP, &gLavaGlowSpriteLayerP, &gQuestionMarkSpriteLayerP,
        &gFrontLavaSplashSpriteLayerP, &gHiddenTriggerSpriteLayerP, &gMiscTriggerSpriteLayerP,
        &gSecretIndicatorSpriteLayerP, &gTopSpriteLayerP, &gExtrasOverlaySpriteLayerP, &gTipContentsSpriteLayerP,
        &gSignContentsSpriteLayerP, &gFPSDisplaySpriteLayerP, &gPausedSpriteLayerP,
        &gCustomDialogSpriteLayerP, &gOptionsDialogPane1LayerP, &gOptionsDialogPane2LayerP,
        &gOptionsDialogPane3LayerP, &gOptionsDialogPane4LayerP,
        &gTextSpriteLayerP, &gCustomCursorSpriteLayerP, &gFadeToBlackSpriteLayerP
    };
    size_t i;

    for (i = 0; i < sizeof(creationOrder) / sizeof(creationOrder[0]); i++)
        CreateLayer(creationOrder[i]);
    for (i = 0; i < sizeof(drawingOrder) / sizeof(drawingOrder[0]); i++)
        SWAddSpriteLayer(gSpriteWorldP, *drawingOrder[i]);
    // gHiddenTriggerItemsSpriteLayerP is created but never drawn, as in the original.
}

/// Creates the 640x480 game world in a window (or full screen, per preferences).
void CreateSpriteWorld(void)
{
    SDL_DisplayMode mode;

    MM_FATAL_IF(SWEnterSpriteWorld(true));
    gPrevWindowWidth = gWindowedResolutionX;
    gPrevWindowHeight = gWindowedResolutionY;
    gFullScreenWindow = gFullScreenWindowPrefs;
    if (gFullScreenWindow && SDL_GetCurrentDisplayMode(0, &mode) == 0) {
        gPrevWindowWidth = (short)mode.w;
        gPrevWindowHeight = (short)mode.h;
    }
    MM_FATAL_IF(SWCreateSpriteWorld(&gSpriteWorldP, "Midnight Mansion HD: After Midnight", 640, 480,
                                    gPrevWindowWidth, gPrevWindowHeight, 32, gFullScreenWindow, true));
    SWSetSpriteWorldStretchToFit(gSpriteWorldP, gStretchToFit);
    if (SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(gSpriteWorldP->mainWindow), &mode) == 0)
        printf("\nCurrent display mode width = %d, height = %d \n", mode.w, mode.h);
    gSpriteDrawProc = SWStdSpriteDrawProc;
    gTranslucentMaskDrawProc = SWStdSpriteDrawProc;
    SWSetSpriteWorldMaxFPS(gSpriteWorldP, 60);
    MM_FATAL_IF(SWStickyError());
}

/// Shrinks the world rect to leave room for the stats bar at the top.
void ChangeWorldRectForGame(SpriteWorldPtr spriteWorldP, short statsHeight)
{
    SWRect r;

    if (worldRectIsSetForGame)
        return;
    r = spriteWorldP->worldRect;
    r.top += statsHeight;
    SWChangeWorldRect(spriteWorldP, &r, false);
    worldRectIsSetForGame = true;
}

Boolean IsStatsAreaVisible(void)
{
    return worldRectIsSetForGame;
}

void MyRestoreWorldRect(SpriteWorldPtr spriteWorldP)
{
    if (worldRectIsSetForGame) {
        SWRestoreWorldRect(spriteWorldP);
        worldRectIsSetForGame = false;
    }
}

/*  Tiles are 16x16 in the world, drawn from 32x32 images. Tile layers sit between the
    sprite layers as follows. */
void SetUpTiling(void)
{
    SetUpLadderArray();
    MM_FATAL_IF(SWInitTiling(gSpriteWorldP, 16, 16, 0xf7a));
    SWSetTileResolution(gSpriteWorldP, 32, 32);
    SWInstallTileLayerAboveSpriteLayer(0, gBackgroundPictureSpriteLayerP);
    SWInstallTileLayerBelowSpriteLayer(1, gLowerTileFringeSpriteLayerP);
    SWInstallTileLayerAboveSpriteLayer(2, gLowerTileFringeSpriteLayerP);
    SWInstallTileLayerAboveSpriteLayer(3, gDisappearingBlockSpriteLayerP);
    SWInstallTileLayerAboveSpriteLayer(4, gMovingPlatformBlockerSpriteLayerP);
    SWInstallTileLayerAboveSpriteLayer(5, gHeroOnPlatformSpriteLayerP);
    SWInstallTileLayerAboveSpriteLayer(6, gHeroSpriteLayerP);
    LoadTiles();
    SetUpTileColorVariations();
}

/*  Tiles 0x5a2 and up are 12 tinted copies of tiles 1..0xd2 (each 0xd2 tiles), used to
    give rooms slightly different shades. gOrigTileIDs maps every tile to its original. */
void SetUpTileColorVariations(void)
{
    short i, variation, tileID;

    for (i = 0; i < 0x5a2; i++)
        gOrigTileIDs[i] = i;
    for (variation = 0; variation < 12; variation++) {
        short first = variation * 0xd2 + 0x5a2;
        short last = variation * 0xd2 + 0x673;
        short base = variation * 0xd2;
        int offset1 = (variation * 0xffff) / 24;
        int level = (int)((double)((variation * 0xffff) / 48) + 43690.0);
        unsigned short gray = (unsigned short)((level * 3 + offset1 + 0x7fff) / 4);
        unsigned short darker = (unsigned short)((offset1 + 0x27ffd) / 3);
        float r, g, b;

        SWCloneTiles(gSpriteWorldP, first, last, 1);
        for (i = first, tileID = 1; i <= last; i++, tileID++)
            gOrigTileIDs[i] = tileID;

        if ((variation & 1) != 0) {
            SWSetTileColorForRange(gSpriteWorldP, first, last, gray, gray, gray, 0xffff);
        } else {
            int tint = variation / 2;
            SW_ASSERT(tint <= 5);
            SWSetTileColorForRange(gSpriteWorldP, first, last,
                                   (unsigned short)(int)(tileColorVars[tint][0] * (float)gray),
                                   (unsigned short)(int)(tileColorVars[tint][1] * (float)gray),
                                   (unsigned short)(int)(tileColorVars[tint][2] * (float)gray), 0xffff);
        }
        b = GetRandomFloat(0.93f, 1.0f);
        g = GetRandomFloat(0.93f, 1.0f);
        r = GetRandomFloat(0.93f, 1.0f);
        SWSetTileColorForRange(gSpriteWorldP, base + 0x666, base + 0x673,
                               (unsigned short)(int)((float)level * r), (unsigned short)(int)((float)level * g),
                               (unsigned short)(int)((float)level * b), 0xffff);
        SWSetTileColorForRange(gSpriteWorldP, base + 0x5cc, base + 0x5e7, gray, gray, gray, 0xffff);
        SWSetTileColorForRange(gSpriteWorldP, base + 0x612, base + 0x62d, gray, gray, gray, 0xffff);
        SWSetTileColorForRange(gSpriteWorldP, base + 0x62e, base + 0x657, darker, darker, darker, 0xffff);
        SWSetTileColorForRange(gSpriteWorldP, base + 0x658, base + 0x665, 0xffff, 0xffff, 0xffff, 0xffff);
    }
    SWSetTileColorForRange(gSpriteWorldP, 0xd3, 0x18a, 0xd998, 0xd998, 0xd998, 0xffff);
    SetUpFringeArray();
    SetUpTileFamilyArray();
}

static void LoadTileRange(short firstTile, short lastTile, short firstResID, short lastResID, short unused)
{
    MM_FATAL_IF(MyLoadTilesFromPict(gSpriteWorldP, firstTile, lastTile, firstResID, lastResID, unused, 0, 0, 0));
}

/*  Loads all tile images. Tile sets come from numbered PICT images (now PNG files);
    a custom mansion can replace individual sets with its own images. */
void LoadTiles(void)
{
    short resID, tileID;
    long savedMode;

    SW_ASSERT(gSpriteWorldP != NULL);
    gWereAnyTilesReplaced = false;
    savedMode = gCurrentGameMode;
    gCurrentGameMode = kNoGameMode;
    SWSetTransparentColor(&gSDLTealColor);

    LoadTileRange(0, 0, 200, 201, 4);
    for (resID = 0xcd, tileID = 1; resID < 0xd7; resID++, tileID += 0xe) {
        LoadTileRange(tileID, tileID + 0xd, resID, resID, 4);
        IncreaseProgressBar(1);
    }
    for (resID = 0xdc, tileID = 0x8d; resID < 0xe0; resID++, tileID += 0xe) {
        LoadTileRange(tileID, tileID + 0xd, resID, resID, 4);
        IncreaseProgressBar(1);
    }
    LoadTileRange(0xc5, 0xd2, 0xe2, 0xe2, 4);
    IncreaseProgressBar(1);
    for (resID = 0x15e, tileID = 0xd3; resID < 0x161; resID++, tileID += 0x24) {
        LoadTileRange(tileID, tileID + 0x23, resID, resID, 4);
        IncreaseProgressBar(1);
    }
    LoadTileRange(0x13f, 0x15a, 0x168, 0x168, 4);
    IncreaseProgressBar(1);
    LoadTileRange(0x15b, 0x18a, 0x16d, 0x16d, 4);
    IncreaseProgressBar(1);
    LoadTileRange(0x18b, 0x1a2, 0x172, 0x172, 4);
    IncreaseProgressBar(1);
    LoadTileRange(0x1a3, 0x1ae, 0x173, 0x173, 4);
    IncreaseProgressBar(1);
    for (resID = 0xe6, tileID = 0x1af; resID < 0xea; resID++, tileID += 0xe) {
        LoadTileRange(tileID, tileID + 0xd, resID, resID, 4);
        IncreaseProgressBar(1);
    }
    LoadTileRange(0x1f9, 0x20a, 0xf5, 0xf5, 4);
    IncreaseProgressBar(1);
    LoadTileRange(0x1e7, 0x1f8, 0xf0, 0xf0, 4);
    IncreaseProgressBar(1);
    for (resID = 0xfa, tileID = 0x20b; resID < 0xfd; resID++, tileID += 0x12) {
        LoadTileRange(tileID, tileID + 0x11, resID, resID, 4);
        IncreaseProgressBar(1);
    }
    LoadTileRange(0x241, 0x2c0, 300, 300, 8);       // conveyor belts
    IncreaseProgressBar(1);
    LoadTileRange(0x2c1, 0x2e8, 400, 400, 4);       // disappearing floors
    IncreaseProgressBar(1);
    LoadTileRange(0x2e9, 0x308, 500, 500, 4);       // light-saber floors
    IncreaseProgressBar(1);
    LoadTileRange(0x309, 0x318, 0x1fe, 0x1fe, 0x40);   // lava
    IncreaseProgressBar(1);
    LoadTileRange(0x319, 800, 0x204, 0x206, 0x10);
    IncreaseProgressBar(1);
    LoadTileRange(0x321, 0x328, 0x205, 0x207, 0x10);
    IncreaseProgressBar(1);
    LoadTileRange(0x329, 0x330, 0x1ff, 0x208, 4);
    IncreaseProgressBar(1);
    LoadTileRange(0x331, 0x3b4, 0x226, 0x226, 4);
    IncreaseProgressBar(1);
    LoadTileRange(0x3b5, 0x4ae, 0x227, 0x227, 4);
    IncreaseProgressBar(1);
    LoadTileRange(0x4af, 0x51e, 0x230, 0x230, 4);
    IncreaseProgressBar(1);
    LoadTileRange(0x51f, 0x53e, 0x231, 0x231, 4);
    IncreaseProgressBar(1);
    LoadTileRange(0x53f, 0x57a, 0x22b, 0x22b, 4);
    IncreaseProgressBar(1);
    LoadTileRange(0x57b, 0x598, 0x22c, 0x22c, 4);
    IncreaseProgressBar(1);
    LoadTileRange(0x599, 0x59a, 0x235, 0x235, 4);
    LoadTileRange(0x59b, 0x5a1, 0x23a, 0x23a, 4);

    SWSetTransparentColor(&gSDLWhiteColor);
    gCurrentGameMode = savedMode;
    if (gWereAnyTilesReplaced)
        SetUpTileColorVariations();
}

/*  Loads tiles startTileID..endTileID from the image with resource ID firstResID. On the
    first call every tile set loads from the standard images; later calls (when a mansion
    opens) only reload sets that the mansion replaces or that were replaced before. */
SWError MyLoadTilesFromPict(SpriteWorldPtr spriteWorldP, short startTileID, short endTileID,
                            short firstResID, short lastResID, short unused1, short borderWidth,
                            short borderHeight, short unused2)
{
    char customPath[500];
    const char *path;
    Boolean shouldLoad = false, useStandard = true;
    SWError err = kNoError;
    int i;

    SW_UNUSED(lastResID);
    SW_UNUSED(unused1);
    SW_UNUSED(unused2);
    if (!tileFlagsWereInitialized) {
        for (i = 0; i < 0x5a2; i++)
            tileFlags[i] = -1;
        tileFlagsWereInitialized = true;
    }
    if (tileFlags[startTileID] == -1) {
        shouldLoad = true;
        tileFlags[startTileID] = 0;
    } else if (!DoesCustomPICTResourceExist(gCurOpenMansionFilePath, firstResID)) {
        if (tileFlags[startTileID] == 1) {
            shouldLoad = true;
            tileFlags[startTileID] = 0;
            useStandard = true;
            gWereAnyTilesReplaced = true;
        }
    } else {
        snprintf(customPath, sizeof(customPath), "%s'%s'/%05d.png", gCurOpenMansionFilePath, "PICT", firstResID);
        shouldLoad = true;
        tileFlags[startTileID] = 1;
        useStandard = false;
        gWereAnyTilesReplaced = true;
    }
    if (shouldLoad) {
        path = useStandard ? GetFileNameForResource(GetSpriteImagePath(0), firstResID) : customPath;
        err = SWLoadTilesFromSingleFileXY(spriteWorldP, path, borderWidth, borderHeight, false,
                                          endTileID - startTileID + 1, startTileID);
    }
    return err;
}

/*  Startup sequence: window, ActionSoft logo, splash screen while tiles and sprites load,
    then the title screen and the main loop. */
void StartUpGameCarbon(void)
{
    short wait;

    gUsingEditor = false;
    LoadHighScores();
    GiveOSTime();
    CreateSpriteWorld();
    CreateAndAddSpriteLayers();
    CreateStatsArea();
    if (gFullScreenWindow)
        MMHideCursor();
    MMFlushEvents();
    InitMusic();
    StartOpeningMusic(true);
    SetUpActionSoftScreen();
    LoadAndSetupSounds(0x10, 7, 0x4f, GetSoundsFolderPath());
#ifdef MM_PS3
    PS3LogMemory("sounds loaded");
#endif
    gOpeningMusicRefNum = GetCurrentMusicReference();
    StartMicrosecTimer();
    DoGameFadeToNormal(0);
    DoActionSoftScreen();
    WaitMicrosecondsOrMouseClick(0x9c4);
    if (WasGameQuit())
        return;
    DoGameFadeToBlack(0x1e, false);
    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
    if (WasGameQuit())
        return;
    MMFlushEvents();
    gOpeningMusicRefNum = GetCurrentMusicReference();
    DisplaySplashScreen();
    DoGameFadeToNormal(0x1e);
    if (WasGameQuit())
        return;
    SWSetCleanUpSpriteWorld(gSpriteWorldP);
    ChangeSplashScreenLoadingText(0);
    SetUpTiling();
#ifdef MM_PS3
    PS3LogMemory("tiles set up");
#endif
    if (WasGameQuit())
        return;
    ChangeSplashScreenLoadingText(1);
    LoadSprites();
    SWSetAssertionFailureCallback(MMAssertFailCallback);
    printf("Finished loading sprites\n");
    if (WasGameQuit())
        return;
    wait = gMusicIsOn ? 0x2e18 : 6000;
    ChangeSplashScreenLoadingText(2);
    WaitMicrosecondsOrMouseClick(wait);
    DoGameFadeToBlack(0x1e, false);
    if (WasGameQuit())
        return;
    MMFlushEvents();
    InitGamePad();
    InitControllers();
    printf("About to show title screen.\n");
#ifdef MM_PS3
    PS3LogMemory("title screen");
#endif
    SetUpTitleScreen();
    UpdateMenuBar();
    DebugAutoStart();
    RunMainLoop();
}

Boolean IsGamepadInitialized(void)
{
    return gamepadInitialized;
}

void InitGamePad(void)
{
    if (!gUseGamePad || gamepadInitialized)
        return;
    if (initedSubsystem)
        SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    puts("\nIniting SDL_INIT_JOYSTICK subsystem.");
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    initedSubsystem = true;
    printf("Num joysticks found: %d \n", SDL_NumJoysticks());
    if (SDL_NumJoysticks() > 0 && !SDL_IsGameController(0)) {     // game controllers: MMController.c
        gJoystick = SDL_JoystickOpen(0);
        if (gJoystick != NULL)
            gamepadInitialized = true;
    }
}

/// Dispatches to the handler of the current screen until the player quits.
void RunMainLoop(void)
{
    puts("\n\nEntering RunMainLoop\n");
    SWUpdateSpriteWorld(gSpriteWorldP, true);
    do {
        if (SplitScreenActive()) {      // split screen runs every player's game itself
            SplitRunFrame();
            continue;
        }
        GiveOSTime();
        ProcessSDLEvents();
        if (!gMMIsActive)
            continue;
        if (gGameIsPaused) {
            HandlePausedScreenCallback();
            continue;
        }
        MMUpdateTimeSpentInMansion();
        if (gCurrentGameMode == kPlayingGameMode)
            RunAnimationCore();
        else if (gCurrentGameMode == kInGameMapMode)
            RunInGameMapScreen();

        switch (gCurrentGameMode) {
            case kSelectMansionMode:     HandleSelectMansionScreenCallback(); break;
            case kReadingSignMode:       HandleReadingSignCallback(); break;
            case kDifficultyScreenMode:  HandleDifficultyScreenCallback(); break;
            case kTitleScreenMode:       HandleTitleScreenCallback(); break;
            case kGameOverScreenMode:    HandleGameOverScreenCallback(); break;
            case kHelpScreenMode:        HandleHelpScreenCallback(); break;
            case kBeatTheMansionMode:    HandleBeatTheMansionScreenCallback(); break;
            case kBeatTheGameMode:       HandleBeatTheGameCallback(); break;
            case kHighScoresScreenMode:  HandleHighScoresScreenCallback(); break;
            case kOrganScreenMode:       HandleOrganScreenCallback(); break;
            case kTeaserScreenMode:      HandleTeaserScreenCallback(); break;
            case kAboutScreenMode:       HandleAboutScreenCallback(); break;
            case kMultiplayerScreenMode: HandleMultiplayerScreenCallback(); break;
        }
    } while (!gQuitGame);
}

// ---------------------------------------------------------------------------
// Preferences

void InitializePreferences(void)
{
        // Numeric keypad by default, arrow keys as the alternative.
    gKeyPrefs.primaryKeys[3] = 0x5c;
    gKeyPrefs.primaryKeys[1] = 0x5e;
    gKeyPrefs.primaryKeys[0] = 0x60;
    gKeyPrefs.primaryKeys[2] = 0x5a;
    gKeyPrefs.primaryKeys[4] = 0x62;
    gKeyPrefs.primaryKeys[7] = 0x28;
    gKeyPrefs.primaryKeys[5] = 0x5f;
    gKeyPrefs.primaryKeys[6] = 0x61;
    gKeyPrefs.primaryKeys[8] = 0x10;
    gKeyPrefs.secondaryKeys[3] = 0x4000005c;
    gKeyPrefs.secondaryKeys[1] = 0x4000005e;
    gKeyPrefs.secondaryKeys[0] = 0x40000060;
    gKeyPrefs.secondaryKeys[2] = 0x4000005a;
    gKeyPrefs.secondaryKeys[4] = 0x20;
    gKeyPrefs.secondaryKeys[7] = 0x0d;
    gKeyPrefs.secondaryKeys[5] = 0x4000005f;
    gKeyPrefs.secondaryKeys[6] = 0x40000061;
    gKeyPrefs.secondaryKeys[8] = 0x6d;
    memset(gKeyPrefs.gamepadNames, 0, sizeof(gKeyPrefs.gamepadNames));
    gArrowKeysEnabled = true;
    gMusicIsOn = true;
    gSofterMusic = false;
    gTitleMusicVolume = 0xff;
    gBackgroundMusicVolume = 0xdc;
    gSoundIsTurnedOn = true;
    gSoundVolume = 0xff;
    gFullScreenWindowPrefs = false;
    gStretchToFit = false;
    gUseGamePad = false;
    gWindowedResolutionX = 1024;
    gWindowedResolutionY = 768;
    gLimitFullscreenResolution = false;
    gEasyJumping = false;
    gRecordMovies = true;
    gFamilyFriendly = true;
    gCurrentPlayerNameExists = false;
}

void LoadPreferences(void)
{
    int err;

    InitializePreferences();
    LoadExtrasSettings();
    if (ReadPreferencesFile() == -1) {
        InitializePreferences();
        WritePreferencesFile();
    }
    if (gCurrentPlayerNameExists) {
        err = LoadProfileFromDisk(gCurrentPlayerName);
        if (err != 0) {
            gCurrentPlayerNameExists = false;
            if (err == -2)
                ErrorMessage("Sorry, this profile is in an old format. Midnight Mansion HD uses a new profile format that is incompatible with the old.", 0);
            else
                ErrorMessage("Your player profile has been corrupted or is missing. Please create or select a different player.", 0);
        }
    }
}

#define kPrefsVersion   0x6c
#define kPrefsEndMarker 0x71f7

int WritePreferencesFile(void)
{
    SDL_RWops *rw = SDL_RWFromFile(GetPrefsFilePath("Midnight Mansion HD Prefs"), "wb");
    Sint32 version = kPrefsVersion, marker = kPrefsEndMarker;     // 4 bytes in the file
    int err;

    if (rw == NULL)
        return -1;
    ClearFileStickyError();
    MyWriteDataToFile(rw, 4, &version);
    MyWriteDataToFile(rw, 1, &gCurrentPlayerNameExists);
    MyWriteDataToFile(rw, 0x100, gCurrentPlayerName);
    MyWriteDataToFile(rw, sizeof(gKeyPrefs), &gKeyPrefs);
    MyWriteDataToFile(rw, 1, &gArrowKeysEnabled);
    MyWriteDataToFile(rw, 1, &gMusicIsOn);
    MyWriteDataToFile(rw, 1, &gSofterMusic);
    MyWriteDataToFile(rw, 2, &gTitleMusicVolume);
    MyWriteDataToFile(rw, 2, &gBackgroundMusicVolume);
    MyWriteDataToFile(rw, 1, &gSoundIsTurnedOn);
    MyWriteDataToFile(rw, 2, &gSoundVolume);
    MyWriteDataToFile(rw, 1, &gEasyJumping);
    MyWriteDataToFile(rw, 1, &gRecordMovies);
    MyWriteDataToFile(rw, 1, &gFamilyFriendly);
    MyWriteDataToFile(rw, 1, &gFullScreenWindowPrefs);
    MyWriteDataToFile(rw, 1, &gStretchToFit);
    MyWriteDataToFile(rw, 1, &gUseGamePad);
    MyWriteDataToFile(rw, 1, &gLimitFullscreenResolution);
    MyWriteDataToFile(rw, 2, &gWindowedResolutionX);
    MyWriteDataToFile(rw, 2, &gWindowedResolutionY);
    MyWriteDataToFile(rw, 4, &marker);
    err = GetFileStickyError();
    SDL_RWclose(rw);
    return err;
}

int ReadPreferencesFile(void)
{
    SDL_RWops *rw = SDL_RWFromFile(GetPrefsFilePath("Midnight Mansion HD Prefs"), "rb");
    Sint32 version = 0, marker = 0;
    Boolean unused;
    int err;

    if (rw == NULL)
        return -1;
    ClearFileStickyError();
    MyReadDataFromFile(rw, 4, &version);
    if (version < 0x6a) {
        SDL_RWclose(rw);
        return -1;
    }
    MyReadDataFromFile(rw, 1, &gCurrentPlayerNameExists);
    MyReadDataFromFile(rw, 0x100, gCurrentPlayerName);
    MyReadDataFromFile(rw, sizeof(gKeyPrefs), &gKeyPrefs);
    MyReadDataFromFile(rw, 1, &gArrowKeysEnabled);
    MyReadDataFromFile(rw, 1, &gMusicIsOn);
    MyReadDataFromFile(rw, 1, &gSofterMusic);
    MyReadDataFromFile(rw, 2, &gTitleMusicVolume);
    MyReadDataFromFile(rw, 2, &gBackgroundMusicVolume);
    MyReadDataFromFile(rw, 1, &gSoundIsTurnedOn);
    MyReadDataFromFile(rw, 2, &gSoundVolume);
    MyReadDataFromFile(rw, 1, &gEasyJumping);
    MyReadDataFromFile(rw, 1, &gRecordMovies);
    MyReadDataFromFile(rw, 1, &gFamilyFriendly);
    MyReadDataFromFile(rw, 1, &gFullScreenWindowPrefs);
    MyReadDataFromFile(rw, 1, &gStretchToFit);
    if (version < 0x6b)
        MyReadDataFromFile(rw, 1, &unused);
    if (version > 0x6a)
        MyReadDataFromFile(rw, 1, &gUseGamePad);
    if (version > 0x6b)
        MyReadDataFromFile(rw, 1, &gLimitFullscreenResolution);
    MyReadDataFromFile(rw, 2, &gWindowedResolutionX);
    MyReadDataFromFile(rw, 2, &gWindowedResolutionY);
    MyReadDataFromFile(rw, 4, &marker);
    err = GetFileStickyError();
    SDL_RWclose(rw);
    if (err == 0 && marker != kPrefsEndMarker)
        err = -1;
    return err;
}

// ---------------------------------------------------------------------------
// Mansions

/// Opens one of the mansions that come with the game (mansionNumber -2 is the tutorial).
int MyOpenStandardMansion(ConstStringPtr mansionName, short mansionNumber)
{
    char name[256], path[500], message[500];
    int err;

    gCurrentMansionNumber = mansionNumber;
    SetCurMansionAsCustomMansion(false);
    SWCopyPascalStringToCString(mansionName, name);
    snprintf(path, sizeof(path), "%s", GetMansionFolderPath(name));
    err = OpenMansionUsingFilePath(path, name);
    if (err == 0) {
        WriteCStringToFile("Opened mansion:");
        WriteCStringToFile(name);
        WriteCarriageReturn(1);
    } else {
        snprintf(message, sizeof(message),
                 "The mansion file '%s' could not be found. If this problem persists, try downloading the game again.",
                 name);
        MMDoErrorMessage(message, 0);
    }
    return err;
}

int OpenMansionUsingFilePath(const char *filePath, const char *mansionNameIn)
{
    Boolean needsMM2 = false;
    long version;
    Uint32 start;
    int err = 0;
    char mansionName[255];

        // (callers pass gMansionFilename itself, which is written below: copying a string onto
        // itself with snprintf left it empty, so a game saved after loading one had no mansion)
    snprintf(mansionName, sizeof(mansionName), "%s", mansionNameIn);

    printf("Attempting to open mansion at path: %s\n", filePath);
    printf("...with name: %s\n", mansionName);
    version = MMGetMansionVersion("VERS", 0x80, 4, filePath);
    if (version == -1)
        return -1;
    if (version < 2001) {
        printf("MANSION VERSION: %ld \n", version);
        if (version >= 0x350) {
            needsMM2 = true;
            printf("Mansion version = %ld\n", version);
            MMDoCustomDialog(gSpriteWorldP,
                             "This mansion requires Midnight Mansion 2. You can't open it with Midnight Mansion 1.", 3);
        }
    } else {
        printf("ERROR: Mansion version = %ld. Ignoring since invalid.\n", version);
    }
    if (!needsMM2) {
        snprintf(gCurOpenMansionFilePath, sizeof(gCurOpenMansionFilePath), "%s", filePath);
        snprintf(gCurOpenMansionFileName, sizeof(gCurOpenMansionFileName), "%s", mansionName);
        snprintf(gMansionFilename, sizeof(gMansionFilename), "%s", mansionName);
        ScanDirectoryForResIDs(filePath, "'TMAP'");
        err = LoadMansionNameFromGAMEResource(gMansionName);
        if (err == 0)
            LoadRestOfGAMEResource();
        start = SDL_GetTicks();
        ScanDirectoryForPICTResIDs(gCurOpenMansionFilePath);
        LoadTiles();
        LoadSprites();
        printf("Total ticks spent doing custom sprite/tile replacement/search: %u \n", SDL_GetTicks() - start);
    }
    return err;
}

int MMOpenCustomMansionUsingPathAndName(const char *filePath, const char *mansionName)
{
    int err;

    gDifficultySetting = 1;
    SetCurMansionAsCustomMansion(true);
    err = OpenMansionUsingFilePath(filePath, mansionName);
    if (err == -5)
        ErrorMessage("This mansion requires 'Midnight Mansion 2: The Haunted Hills'. You can't open it with Midnight Mansion 1.", 0);
    else if (err == -1)
        ErrorMessage("The mansion file couldn't be opened. Please make sure it is a HD format mansion.", 0);
    return err;
}

void SetCurMansionAsCustomMansion(Boolean isCustom)
{
    if (!isCustom) {
        gMansionIsCustomMansion = false;
        gCanUseLadderJump = false;
    } else {
        gMansionIsCustomMansion = true;
        gCurrentMansionNumber = -1;
        gCanUseLadderJump = true;
    }
}

void StartTutorialMansion(void)
{
    if (MyOpenStandardMansion((ConstStringPtr)"\x10Tutorial Mansion", -2) == 0) {
        StartGameNoEditor();
    } else {
        SetUpDifficultyScreen();
        StartOpeningMusic(false);
    }
}

/*  Testing aid: MM_DEBUG_BOT=1 plays by itself, to stress-test games on several computers.
    It walks one way for a while, jumps, climbs and pulls things now and then, and every half
    minute or so walks out of the room through a side that has a room behind it (moving Jack
    to that edge first), so it gets round the mansion. Its numbers are its own, so it draws
    none of the game's. In relay co-op it plays only on the computer with the turn, and its
    trips to the next room go to the others like the F6 testing key. */
static Uint32 gBotRandom;

static int BotRandom(int min, int max)
{
    if (gBotRandom == 0)
        gBotRandom = (Uint32)SDL_GetPerformanceCounter() ^ (Uint32)(uintptr_t)&gBotRandom ^ 0x9e3779b9u;
    gBotRandom ^= gBotRandom << 13;
    gBotRandom ^= gBotRandom >> 17;
    gBotRandom ^= gBotRandom << 5;
    return min + (int)(gBotRandom % (Uint32)(max - min + 1));
}

/// There's a room beside this one, left (dir < 0) or right, and Jack can be taken to it.
static Boolean BotRoomBeside(int dir)
{
    SpritePtr heroP = GetHeroSprite();
    short row, col;

    if (heroP == NULL || HERO(heroP)->isDead || HERO(heroP)->platformP != NULL)
        return false;
    GEGetRowAndColFromLevelNumber(gCurrentLevel, &row, &col, 200);
    col = (short)(col + (dir < 0 ? -1 : 1));
    return col >= 0 && Resource1Exists("TMAP", GEGetLevelResID(GEGetLevelNumberFromRowAndCol(row, col, 200), 1, 200));
}

/// Moves Jack just past the left (dir < 0) or right edge of the room; walking on takes him out.
void DebugBotMoveToEdge(int dir)
{
    SpritePtr heroP = GetHeroSprite();
    SWRect *workArea = &gSpriteWorldP->workAreaRect;

    if (!BotRoomBeside(dir))
        return;
    if (dir < 0)
        SWMoveSprite(heroP, (float)workArea->left - SWGetSpriteWidth(heroP) + 2.0f, SWGetSpriteVertLoc(heroP));
    else
        SWMoveSprite(heroP, (float)workArea->right - 2.0f, SWGetSpriteVertLoc(heroP));
}

/// A key press, as if typed (from SDL's timer thread too: SDL_PushEvent may be called there).
static void BotPressKey(SDL_Scancode scancode)
{
    SDL_Event event;

    memset(&event, 0, sizeof(event));
    event.type = SDL_KEYDOWN;
    event.key.state = SDL_PRESSED;
    event.key.keysym.scancode = scancode;
    event.key.keysym.sym = SDL_GetKeyFromScancode(scancode);
    SDL_PushEvent(&event);
    event.type = SDL_KEYUP;
    event.key.state = SDL_RELEASED;
    SDL_PushEvent(&event);
}

static Uint32 BotPressKeyLater(Uint32 interval, void *param)
{
    (void)interval;
    BotPressKey((SDL_Scancode)(intptr_t)param);
    return 0;
}

/// Once a game frame, before the keys are read (and sent, in relay co-op).
void DebugBotFrame(void)
{
    static int enabled = -1, walk = 0, walkFrames = 0, jumpFrames = 0, upDownFrames = 0, upDown = 0;
    static int actionFrames = 0, tripIn = 0, tripDir = 0, mapIn = 0, pauseIn = 0;
    SpritePtr heroP;

    if (enabled < 0)
        enabled = getenv("MM_DEBUG_BOT") != NULL;
    if (!enabled || gCurrentGameMode != kPlayingGameMode || gGameIsPaused || NetIsFollower())
        return;
        // now and then a look at the map, or a pause, for a few seconds
    if (mapIn <= 0)
        mapIn = BotRandom(60 * 40, 60 * 90);
    if (pauseIn <= 0)
        pauseIn = BotRandom(60 * 60, 60 * 120);
    if (--mapIn == 0) {
        BotPressKey((SDL_Scancode)gKeyPrefs.primaryKeys[kKeyMap]);
        SDL_AddTimer((Uint32)BotRandom(1500, 4000), BotPressKeyLater,
                     (void *)(intptr_t)gKeyPrefs.primaryKeys[kKeyMap]);
    }
    if (--pauseIn == 0 && !SplitScreenActive()) {      // (split screen's pause menu is its own)
        BotPressKey(SDL_SCANCODE_ESCAPE);
        SDL_AddTimer((Uint32)BotRandom(1000, 3000), BotPressKeyLater, (void *)(intptr_t)SDL_SCANCODE_ESCAPE);
    }
    heroP = GetHeroSprite();        // (none while Jack blinks after dying: a key brings him back)
    if (tripIn <= 0)
        tripIn = BotRandom(60 * 15, 60 * 45);
    if (--tripIn == 0 && heroP != NULL) {
        tripDir = BotRandom(0, 1) ? 1 : -1;
        if (!BotRoomBeside(tripDir))
            tripDir = -tripDir;
        if (BotRoomBeside(tripDir)) {
            if (NetGameActive())        // (every computer takes him there as this frame begins)
                NetRecordEvent((char)(tripDir < 0 ? kNetEventDebugTripLeft : kNetEventDebugTripRight), true);
            else
                DebugBotMoveToEdge(tripDir);
            walk = tripDir;
            walkFrames = 30;
        }
    }
    if (--walkFrames <= 0) {
        walk = BotRandom(0, 4) == 0 ? 0 : BotRandom(0, 1) ? 1 : -1;
        walkFrames = BotRandom(20, 200);
    }
    if (--jumpFrames < -BotRandom(20, 120))
        jumpFrames = BotRandom(5, 25);
    if (--upDownFrames <= 0) {
        upDown = BotRandom(0, 3) == 0 ? (BotRandom(0, 1) ? 1 : -1) : 0;
        upDownFrames = BotRandom(30, 150);
    }
    if (--actionFrames < -BotRandom(100, 400))
        actionFrames = 3;
    gKeys[kKeyLeft] = walk < 0;
    gKeys[kKeyRight] = walk > 0;
    gKeys[kKeyUp] = upDown < 0;
    gKeys[kKeyDown] = upDown > 0;
    gKeys[kKeyJump] = jumpFrames > 0;
    gKeys[kKeyJumpLeft] = gKeys[kKeyJumpRight] = false;
    gKeys[kKeyAction] = actionFrames > 0;
}

/// Testing aid: MM_DEBUG_AUTOSTART=<n> starts standard mansion n (Normal) from the title screen.
void DebugAutoStart(void)
{
    const char *autoStart = getenv("MM_DEBUG_AUTOSTART");
    const char *net = getenv("MM_DEBUG_NET");       // testing aid: "host:<net mode>" or "join:<address>"
    unsigned char mansionName[256] = { 0 };
    Boolean online = false;
    short n;

    if (net != NULL && strncmp(net, "online-", 7) == 0) {
        net += 7;           // "online-host:<net mode>", "online-join:<host's player ID>": through the lobby
        online = true;
    }
    if (net != NULL && strncmp(net, "join:", 5) == 0) {
        if ((online ? MultiplayerConnectOnline(kNetModeRelay, false, false, NULL, net + 5)
                    : MultiplayerConnect(kNetModeRelay, false, net + 5)) == kMultiplayerJoined) {
            if (NetContinuedSavePath() != NULL && NetMode() == kNetModeArcade)
                SplitContinueSave(NetContinuedSavePath());
            else if (NetContinuedSavePath() != NULL)
                MultiplayerContinueSave(NetContinuedSavePath());
            else
                StartCustomMansion();
        }
        return;
    }
    if (net != NULL && strncmp(net, "host:", 5) == 0 && getenv("MM_DEBUG_NET_CONTINUE") != NULL) {
            // testing aid: the host carries on with the group's newest save (Arcade Split Screen, or
            // with host:0 Co-op)
        MultiSaveInfo saves[1];
        if (MultiplayerConnect(atoi(net + 5), true, NULL) != kMultiplayerNewGame)
            return;
        if (atoi(net + 5) == kNetModeRelay && MultiSaveListForCurrentGroup(kMultiSaveCoop, saves, 1) == 1) {
            NetSetContinueSave(saves[0].path);      // (Co-op: the newest of these players' saves)
            MultiplayerContinueSave(saves[0].path);
            return;
        }
        if (MultiSaveListForCurrentGroup(kMultiSaveArcade, saves, 1) == 1) {
            NetSetContinueSave(saves[0].path);
            SplitContinueSave(saves[0].path);
            return;
        }
        net = NULL;     // nothing saved: a new game
    }
    if (getenv("MM_DEBUG_SPLIT_CONTINUE") != NULL && getenv("MM_DEBUG_PROFILES") != NULL) {
            // testing aid: split screen carries on with these players' newest Arcade Split Screen save
        static const int kPads[] = { kSplitInputPad1, kSplitInputPad2, kSplitInputPad3, kSplitInputPad4 };
        const char *names = getenv("MM_DEBUG_PROFILES");
        char profiles[kMaxPlayers][64];
        MultiSaveInfo saves[1];
        int k;

        memset(profiles, 0, sizeof(profiles));
        for (k = 0; k < kMaxPlayers && *names; k++) {
            size_t len = strcspn(names, ",");
            snprintf(profiles[k], sizeof(profiles[k]), "%.*s", (int)(len < 63 ? len : 63), names);
            names += len + (names[len] == ',');
        }
        SetMultiplayerPlayers(k, profiles, NULL, true);
        SetMultiplayerInputs(kPads);
        if (MultiSaveListForCurrentGroup(kMultiSaveArcade, saves, 1) == 1) {
            SplitConfigure(kSplitModeCoop, k, kPads);
            SplitContinueSave(saves[0].path);
        }
        return;
    }
    if (autoStart == NULL)
        return;
    if (net != NULL && strncmp(net, "host:", 5) == 0 &&
        (online ? MultiplayerConnectOnline(atoi(net + 5), true, false, NULL, NULL)
                : MultiplayerConnect(atoi(net + 5), true, NULL)) != kMultiplayerNewGame)     // (MM_DEBUG_NET_START)
        return;
    n = (short)atoi(autoStart);
    SWConcatPascalStrings(mansionName, GetMansionNameFromIndex(n));
    SWConcatPascalStrings(mansionName, (ConstStringPtr)"\007 Normal");
    ExitTitleScreen();
    if (MyOpenStandardMansion(mansionName, n) == 0)
        StartGameNoEditor();
}

void StartGameNoEditor(void)
{
    short levelResID = LoadLevelResIDFromGAMEResource();
    const char *startRoom = getenv("MM_DEBUG_START_ROOM");     // testing aid: start in this room resID
    const char *split = getenv("MM_DEBUG_SPLIT");               // testing aid: "race:2", "coop:3"...

    if (split != NULL && !SplitScreenPending()) {
        int n = strchr(split, ':') != NULL ? atoi(strchr(split, ':') + 1) : 2;
        Boolean relay = strncmp(split, "relay", 5) == 0;
        static const int kPads[] = { kSplitInputPad1, kSplitInputPad2, kSplitInputPad3, kSplitInputPad4 };
#ifdef MM_PS3
        Boolean pads = true;
#else
        Boolean pads = strstr(split, ":pads") != NULL;     // "coop:2:pads": controllers 1, 2...
#endif
        const char *names = getenv("MM_DEBUG_PROFILES");    // "Ann,Bob": the players' profiles
        char profiles[kMaxPlayers][64];
        int k;

        memset(profiles, 0, sizeof(profiles));
        for (k = 0; names != NULL && k < kMaxPlayers && *names; k++) {
            size_t len = strcspn(names, ",");
            snprintf(profiles[k], sizeof(profiles[k]), "%.*s", (int)(len < 63 ? len : 63), names);
            names += len + (names[len] == ',');
        }
        SetMultiplayerPlayers(n, getenv("MM_DEBUG_PROFILES") != NULL ? profiles : NULL, NULL, !relay);
        SetMultiplayerInputs(pads ? kPads : NULL);
        if (!relay) {
            SplitConfigure(strncmp(split, "race", 4) == 0 ? kSplitModeRace : kSplitModeCoop, n, pads ? kPads : NULL);
        }
    }

    if (startRoom != NULL && levelResID != -1)
        levelResID = (short)atoi(startRoom);
    MultiplayerGameStarted();
    StatsNoteGameStarted();
    if (levelResID == -1) {
        ErrorMessage("ERROR: The GAME resource from this mansion couldn't be opened.", 0);
        SetUpTitleScreen();
    } else {
        gCurrentLevel = GEGetLevelNumber(levelResID, 1, 200);
        ResetKeys();
        StartGame(levelResID, gCurrentLevel, true);
    }
}

void StartGame(short levelResID, short levelNumber, Boolean unused)
{
    SW_UNUSED(levelResID);
    SW_UNUSED(unused);
#ifdef MM_PS3
    PS3LogMemory("a game starts");
#endif
    LoadRestOfGAMEResource();
    DisposeTitleScreenSprites();
    StopAndDisposeMusic();
    StopAllChannels();
    gCurrentMusicIndex = -1;
    if (SplitScreenPending()) {         // split screen: each player's game starts from here
        SplitScreenBegin(levelNumber);
        if (SplitScreenActive())
            return;
    }
    SetUpNewGame();
    ChangeWorldRectForGame(gSpriteWorldP, gStatsHeight);
    SWSetPostEraseCallBack(gSpriteWorldP, MyPostEraseCallback);
    MM_FATAL_IF(SWStickyError());
    gCurrentGameMode = kPlayingGameMode;
    gRunningFrameCount = 0;
    if (gUseOutputFile) {
        if (!IsMoviePlaying())
            OpenOutputFile("Output File Log");
        else
            OpenOutputFile("Output File Log - Movie");
    }
    if (gRecordMovies && !IsMoviePlaying() && !NetGameActive())
        OpenMovieFileForOutput(true, false, 0, 0);
    NetSeedRandom();                    // two computers: the same random numbers on both
    EnableRecordMovieMenuItem();
    LoadLevel(levelNumber, true);
    SetUpLevelForPlaying();
    SetCorrectGoldSilverTileImages(true);
    DrawLevel();
    SetStretchToFit(gStretchToFit);
    gSpriteWorldIsValid = true;
    DoGameFadeToNormal(0x1e);
    MM_FATAL_IF(SWStickyError());
    RunAnimation();
}

void SetUpNewGame(void)
{
    gPlayingGame = true;
    gLastSaveFilename[0] = '\0';
    if (!gUsingEditor) {
        MMHideAllCursors();
    } else {
        MMRemoveGameCursor();
        MMShieldCursor();
    }
    ResetKeys();
    SWSetSpriteWorldMaxFPS(gSpriteWorldP, 60);
    gCanControlWhileJumping = true;
    gSilverFloorIsActive = true;
    gGoldFloorIsActive = false;
    ResetConveyorBeltDir();
    SWResetCurrentTileImages(gSpriteWorldP);
    ResetFloorTileAnimations(false, true);
    SetCorrectGoldSilverTileImages(false);
    Randomize();
    gNumFrames = 0;
    gMyOldTicks = 0;
    gHeroIsOnConveyorBelt = false;
    gCurrentlyInMansion = true;
    MMStartGameTimer();
    gRoomIsDark = false;
    gRoomWasDark = false;
    SetStartingNumLives();
    gScore = 0;
    gOldScore = 0;
    gGameOver = false;
    gEndGame = false;
    gDemoMode = false;
    gBeatTheMansion = false;
    gRestartLevel = false;
    gGameIsPaused = false;
    gDidCheat = false;
    gShowSecrets = false;
    gNumSecretsFound = 0;
    gCurrentWindDelta = 0;
    gCurrentSaveIndex = 0;
    gCurrentFPSValue = 0;
    gDelayUntilUpdatingFPSValue = 0;
    gNumFramesSinceEnteringRoom = 0;
    gDoFrameSkipping = false;
    gRemovedGlowsAndShadows = false;
    gLastSavePedestalObjectID = 0;
    memset(&gObjects, 0, sizeof(gObjects));
    InitObjectArray();
    InitDoOnceArray();
    InitRoomArray();
    InitMapArray();
    InitLitRoomArray();
    ResetStatsArea();
    DisposeAllMapCachedRooms();
    UnpauseAllSpriteLayers(gSpriteWorldP);
    gLayersArePausedForSaveAnim = false;
    gStatuesAreDoneFading = false;
    SDL_DisableScreenSaver();
}

void RunAnimation(void)
{
    gCurrentGameMode = kPlayingGameMode;
    UpdateMenuBar();
    ResetKeys();
    SWSetTileChangeProc(gSpriteWorldP, TileChangeProc);
    gRunningFrameCount = 0;
    gEscapeKey = false;
    MM_FATAL_IF(SWStickyError());
    UpdateMenuBar();
}

/*  One step of gameplay: move and draw everything, check collisions, update score,
    lives and the FPS monitor (which turns on frame skipping or drops glows and shadows
    on slow machines), then handle game over / end of mansion. */
void RunAnimationCore(void)
{
    static long fpsAccum = 1;
    static Boolean skippedFPSValue = false;
    static unsigned long prevMillisec = 0;
    SpriteLayerPtr heroLayerP;
    SpritePtr heroP;

    if (gGameIsPaused)
        return;
    SW_ASSERT(gCurrentGameMode == kPlayingGameMode);
    SW_ASSERT(gSpriteWorldP != NULL);
    if (NetMustWait()) {                // two computers: the next frame hasn't arrived yet
        NetRedrawWhileWaiting();
        if (gEndGame)
            DoEndGame(false);
        return;
    }
    SWProcessSpriteWorld(gSpriteWorldP);
    SWAnimateSpriteWorld(gSpriteWorldP);

    if (gSpriteWorldP->frameHasOccurred) {
        if (gSpriteWorldP->tileChangeProc != TileChangeProc)
            SWSetTileChangeProc(gSpriteWorldP, TileChangeProc);
        if (gDoFrameSkipping) {
            gNumFrames++;
            AnimateOneFrame(gSpriteWorldP);
        }
        gNumTilesDrawnThisFrame = 0;
        heroLayerP = (SWGetNextSprite(gHeroOnPlatformSpriteLayerP, NULL) == NULL) ? gHeroSpriteLayerP
                                                                                   : gHeroOnPlatformSpriteLayerP;
        SWCollideSpriteLayer(gSpriteWorldP, gMedusaSpriteLayerP, gFlameSpriteLayerP);
        SWCollideSpriteLayer(gSpriteWorldP, heroLayerP, gEnemySpriteLayerP);
        SWCollideSpriteLayer(gSpriteWorldP, heroLayerP, gLadybugSpriteLayerP);
        SWCollideSpriteLayer(gSpriteWorldP, heroLayerP, gBirdSpriteLayerP);
        SWCollideSpriteLayer(gSpriteWorldP, heroLayerP, gMedusaSpriteLayerP);
        SWCollideSpriteLayer(gSpriteWorldP, heroLayerP, gEyeballSpriteLayerP);
        SWCollideSpriteLayer(gSpriteWorldP, heroLayerP, gSnakeSpriteLayerP);
        SWCollideSpriteLayer(gSpriteWorldP, heroLayerP, gGhostSpriteLayerP);
        heroP = GetHeroSprite();
        if (heroP != NULL)
            SWCollideSpriteWithLayer(heroP, gSignSpriteLayerP, HeroTipCollideProc);
        SWCollideSpriteLayer(gSpriteWorldP, gBigSpiderSpriteLayerP, heroLayerP);
        SWCollideSpriteLayer(gSpriteWorldP, gBigSpiderSpriteLayerP, gBirdSpriteLayerP);
        SWCollideSpriteLayer(gSpriteWorldP, gBigSpiderSpriteLayerP, gEyeballSpriteLayerP);
        SWCollideSpriteLayer(gSpriteWorldP, gBigSpiderSpriteLayerP, gMedusaSpriteLayerP);
        SWCollideSpriteLayer(gSpriteWorldP, gObjectSpriteLayerP, heroLayerP);
        SWCollideSpriteLayer(gSpriteWorldP, gVertMovingPlatformSpriteLayerP, heroLayerP);
        SWCollideSpriteLayer(gSpriteWorldP, gZapperBeamSpriteLayerP, heroLayerP);
        SWCollideSpriteLayer(gSpriteWorldP, gSecretIndicatorSpriteLayerP, heroLayerP);
        CheckForPressedBrickSwitches();
        CheckHeroWithLavaTiles();
        CheckDeadHeroWithLavaTiles();
    }
    MM_FATAL_IF(SWStickyError());

    if (gSpriteWorldP->frameHasOccurred) {
            // An extra life every 1000 points.
        while ((gScore / 1000) * 1000 != (gOldScore / 1000) * 1000 && (gOldScore / 1000) * 1000 <= (gScore / 1000) * 1000) {
            MMPlaySound(0x23, 9, 1);
            gNumLivesLeft++;
            gOldScore += 1000;
        }
        UpdateStatsItems();
        gOldScore = gScore;
        gNumFrames++;
        gNumFramesSinceEnteringRoom++;

            // FPS monitor: ignore single slow frames (more than 3x the previous one).
        if (!skippedFPSValue && SWGetElapsedMilliseconds(gSpriteWorldP) > (unsigned long)(prevMillisec * 3)) {
            skippedFPSValue = true;
        } else {
            fpsAccum += (long)SWGetElapsedMilliseconds(gSpriteWorldP);
            gDelayUntilUpdatingFPSValue--;
            skippedFPSValue = false;
            prevMillisec = SWGetElapsedMilliseconds(gSpriteWorldP);
        }
        if (gDelayUntilUpdatingFPSValue < 1 && fpsAccum > 0) {
            gCurrentFPSValue = (short)(30000 / fpsAccum);
            if (gNumFramesSinceEnteringRoom >= 20 && gNumFramesSinceEnteringRoom <= 60) {
                    // (not in two-computer games: both must keep the same sprites)
                if (!gRemovedGlowsAndShadows && gCurrentFPSValue < 55 && !NetGameActive()) {
                    RemoveAllSpriteGlowsAndShadows();
                    gRemovedGlowsAndShadows = true;
                }
            } else if (gNumFramesSinceEnteringRoom > 60 && gNumFramesSinceEnteringRoom < 56 &&
                       !gDoFrameSkipping && gCurrentFPSValue < 220) {
                    // Never true (as in the original): frame skipping stays off.
                gDoFrameSkipping = true;
                SWSetSpriteWorldMaxFPS(gSpriteWorldP, 30);
            }
            fpsAccum = 0;
            gDelayUntilUpdatingFPSValue = 30;
        }

        MMUpdateChannelInfoForSpriteLayer(gSnakeSpriteLayerP, 0xd, 1, (short)(int)((double)gSoundVolume * 0.5), 0x140, 300);
        MMUpdateChannelInfoForSpriteLayer(gFlameSpriteLayerP, 0x21, 0, (short)(int)((double)gSoundVolume * 0.65), 0x10e, 300);
        MMUpdateChannelInfoForSpriteLayer(gZapperBeamSpriteLayerP, 0x22, 3, (short)(int)((double)gSoundVolume * 0.9), 300, 300);
        if (gCurrentRoomHasLava && IsChannelAvailable(2))
            MMPlayStereoSound2(0, gLavaSnd, 2, 2, (short)(int)((double)gSoundVolume * 0.85), 1.0, -1);
        ExtrasGameFrame();
        if (gRestartLevel) {
            RestartLevel();
            gRestartLevel = false;
        }
    }

    if (SplitScreenActive() && SplitTakeGameEnd())
        return;
    if (gGameOver) {
        if (!gUsingEditor) {
            Boolean wasMovie = IsMoviePlaying();
            CleanUpAfterGame();
            if (!wasMovie) {
                DoGameOverScreen();
            } else {
                DoGameFadeToBlack(0x37, true);
                SetUpTitleScreen();
            }
        }
    } else if (gEndGame) {
        DoEndGame(false);
    } else if (gBeatTheMansion) {
        Boolean wasMovie = IsMoviePlaying();
        MMPauseGameTimer();
        DoGameFadeToBlack(0x37, true);
        CleanUpAfterGame();
        if (wasMovie)
            SetUpTitleScreen();
        else if (gCurrentMansionNumber == 10)
            DoBeatTheGameSlideshow();
        else
            DoBeatTheMansionScreen(false);
    }
}

/// Draws the stats bar (a separate SpriteWorld) before the game world each frame.
void MyPostEraseCallback(SpriteWorldPtr spriteWorldP)
{
    if (IsStatsAreaVisible() && gStatsSpriteWorldP != NULL)
        SWUpdateSpriteWorld(gStatsSpriteWorldP, false);
    SDL_GL_MakeCurrent(spriteWorldP->mainWindow, spriteWorldP->glContext);
    SWResetGLSetup(spriteWorldP);
}

void MyTileLayer3Callback(SpriteWorldPtr spriteWorldP, short tileLayer, SWBoolean layerWasDrawn)
{
    SW_UNUSED(spriteWorldP);
    SW_UNUSED(tileLayer);
    SW_UNUSED(layerWasDrawn);
}

void SetMovingPlatformOverlapBottomInset(SpriteLayerPtr layerP, short inset)
{
    SpritePtr spriteP;

    for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP))
        SWSetDrawTilesOverSpriteInsetRect(spriteP, 0, 0, 0, inset);
}

void CleanUpAfterGame(void)
{
    AbandonInGameScreens();
    gLightSaberFloorTranslucency = 0xffff;
    CloseMansionFile();
    CloseGameMovieFile();
    gStartRecordingMovie = false;
    DisposeAllMapCachedRooms();
    SWSetPostEraseCallBack(gSpriteWorldP, NULL);
    MyRestoreWorldRect(gSpriteWorldP);
    SWSetTileChangeProc(gSpriteWorldP, NULL);
    DisposeCurrentRoom(true);
    UnpauseAllSpriteLayers(gSpriteWorldP);
    gLayersArePausedForSaveAnim = false;
    StopAllChannels();
    gRoomIsDark = false;
    SWSetAllSpriteLayersVisible(gSpriteWorldP, true);
    SWDisposeBackgroundTileTexture(gSpriteWorldP);
    SDL_EnableScreenSaver();
    gPlayingGame = false;
    MultiplayerGameEnded();
    SaveAllLifetimeStats();
    NetEndSession();
}

void CloseMansionFile(void)
{
    if (!gEditorIsRunning) {
        gCurOpenMansionFilePath[0] = '\0';
        gCurOpenMansionFileName[0] = '\0';
    }
}

void ShutDown(void)
{
    printf("ShutDown() called.\n");
    MyCleanUpFunction();
    if (gSpriteWorldP != NULL) {
        SWDisposeSpriteWorld(&gSpriteWorldP);
        SWExitSpriteWorld();
    }
    exit(0);
}

void MyCleanUpFunction(void)
{
    CloseOutputFile();
    CloseGameMovieFile();
    gCurrentGameMode = kQuittingMode;
    MMInitCursor();
    MMFlushEvents();
    DoGameFadeToNormal(0);
    StopAllChannels();
    StopAndDisposeMusic();
}

Boolean IsPlayingGame(void)
{
    return gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kInGameMapMode ||
           gCurrentGameMode == kReadingSignMode;
}

void SetStartingNumLives(void)
{
    gNumLivesLeft = GetStartingNumLives(gDifficultySetting);
}

/// 90 lives in the first mansion on the easiest difficulty, otherwise 6.
short GetStartingNumLives(char difficulty)
{
    const char *lives = getenv("MM_DEBUG_LIVES");      // testing aid: start with this many

    if (lives != NULL && atoi(lives) > 0)
        return (short)atoi(lives);
    if (gCurrentMansionNumber == 0 && difficulty == 0)
        return 90;
    return 6;
}

void RestoreJacksLives(void)
{
    if (gNumLivesLeft < GetStartingNumLives(gDifficultySetting))
        gNumLivesLeft = GetStartingNumLives(gDifficultySetting);
}

void MMStartGameTimer(void)
{
    gTimeSpentInMansion = 0;
    gMyOldNumFrames = 0;
}

/// Counts whole seconds of play (60 frames each).
void MMUpdateTimeSpentInMansion(void)
{
    unsigned long seconds = (unsigned long)(gNumFrames - gMyOldNumFrames) / 60;

    if (seconds != 0) {
        gMyOldNumFrames += seconds * 60;
        gTimeSpentInMansion += seconds;
    }
}

void MMPauseGameTimer(void)
{
    MMUpdateTimeSpentInMansion();
}

void MMResumeGameTimer(void)
{
}

/// Disposes every sprite except the fade overlay.
void MMDisposeAllSpritesInSpriteWorld(SpriteWorldPtr spriteWorldP)
{
    SpriteLayerPtr layerP = NULL;

    while ((layerP = SWGetNextSpriteLayer(spriteWorldP, layerP)) != NULL) {
        if (layerP != gFadeToBlackSpriteLayerP)
            SWDisposeAllSpritesInLayer(layerP);
    }
}

// ---------------------------------------------------------------------------
// Animated tiles

/*  Called each frame: animates conveyor belts, disappearing floors, lava and the
    "light saber" floors that fade in and out. Timers run on frames (gNumFrames). */
void TileChangeProc(SpriteWorldPtr w)
{
    static short conveyorDelay = 0, lavaAnimDelay = 0, lavaFlowDelay = 0;
    short frames, i, next;
    Boolean updateLava = false;

    if ((gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kReadingSignMode) && !gGameIsPaused) {
            // (the pause screen's frames come here too, but aren't the game's: on several
            // computers they would go to the others and be played there)
        DebugBotFrame();                // testing aid (MM_DEBUG_BOT)
        NetFrameBegin();                // two computers: send or take this frame's keys
    }
    if (IsMoviePlaying())
        GetKeyEventsFromMovie();
    if (gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kReadingSignMode)
        gRunningFrameCount++;
    if (gLayersArePausedForSaveAnim)
        return;

    frames = (short)gNumFrames - (short)gMyOldTicks;
    gMyOldTicks = gNumFrames;

        // Conveyor belts: tiles 0x241..0x248 and 0x249..0x250 scroll in opposite directions.
    if (gCurConveyorBeltDirection != 0 && !gRoomIsDark)
        conveyorDelay += frames;
    if (conveyorDelay > 0 && gCurConveyorBeltDirection != 0) {
        short fwdFirst, fwdLast, backFirst, backLast;
        if (gCurConveyorBeltDirection < 1) {
            fwdFirst = 0x249; fwdLast = 0x250; backFirst = 0x241; backLast = 0x248;
        } else {
            fwdFirst = 0x241; fwdLast = 0x248; backFirst = 0x249; backLast = 0x250;
        }
        if (w->curTileImage[fwdFirst] + 8 < 0x2c0) {
            for (i = fwdFirst; i <= fwdLast; i++)
                SWChangeTileImageFast(w, i, w->curTileImage[i] + 8);
        } else {
            for (i = fwdFirst; i <= fwdLast; i++)
                SWChangeTileImageFast(w, i, (i - fwdFirst) + 0x241);
        }
        MyUpdateTilesOnScreenFaster(w, 1, 4, fwdFirst, fwdLast);
        if (w->curTileImage[backFirst] - 8 < 0x241) {
            for (i = backFirst; i <= backLast; i++)
                SWChangeTileImageFast(w, i, (i - backFirst) + 0x2b9);
        } else {
            for (i = backFirst; i <= backLast; i++)
                SWChangeTileImageFast(w, i, w->curTileImage[i] - 8);
        }
        MyUpdateTilesOnScreenFaster(w, 1, 4, backFirst, backLast);
        conveyorDelay = 0;
    }

        // Disappearing floors cycle: visible, fading out, gone, fading in (every 180 frames).
    gDisappearingFloorWait += frames;
    if (gDisappearingFloorWait > 0xb3) {
        gDisappearingFloorWait = 0;
        gDisappearingFloorMode++;
        if (gDisappearingFloorMode > 3)
            gDisappearingFloorMode = 0;
        gDisappearingFloorAnimDelay = 3;
    }
    if (gDisappearingFloorMode == 0 || gDisappearingFloorMode == 2) {
        gDisappearingFloorAnimDelay += frames;
        if (gDisappearingFloorAnimDelay > 2) {
            short step = (gDisappearingFloorMode == 2) ? 8 : -8;
            if (w->curTileImage[0x2c1] + step < 0x2c1 || w->curTileImage[0x2c1] + step > 0x2e7) {
                gDisappearingFloorMode = (gDisappearingFloorMode == 0) ? 1 : 3;
            } else {
                for (i = 0x2c1; i < 0x2c9; i++)
                    SWChangeTileImageFast(w, i, w->curTileImage[i] + step);
                MyUpdateTilesOnScreenFaster(w, 1, 4, 0x2c1, 0x2c8);
            }
            gDisappearingFloorAnimDelay = 0;
        }
    }

        // Lava surface animation.
    lavaAnimDelay += frames;
    if (lavaAnimDelay > 10) {
        for (i = 0x309; i < 0x30d; i++) {
            next = w->curTileImage[i] + 4;
            if (next > 0x318)
                next = i;
            SWChangeTileImageFast(w, i, next);
        }
        for (i = 0x319; i < 0x31b; i++) {
            next = w->curTileImage[i] + 2;
            if (next > 800)
                next = i;
            SWChangeTileImageFast(w, i, next);
        }
        for (i = 0x321; i < 0x323; i++) {
            next = w->curTileImage[i] + 2;
            if (next > 0x328)
                next = i;
            SWChangeTileImageFast(w, i, next);
        }
        for (i = 0x329; i < 0x32b; i++) {
            next = w->curTileImage[i] + 2;
            if (next > 0x330)
                next = i;
            SWChangeTileImageFast(w, i, next);
        }
        updateLava = true;
        lavaAnimDelay = 0;
    }
    lavaFlowDelay += frames;
    if (lavaFlowDelay > 3) {
        updateLava = true;
        lavaFlowDelay = 0;
    }
    if (updateLava)
        SWUpdateTilesOnScreenFast(w, 0x309, 0x330);

        // Light-saber floors switch on and off every 200 frames and pulse while on.
    if (!gRoomIsDark)
        gLightSaberFloorWait -= frames;
    if (gLightSaberFloorWait < 1) {
        gLightSaberFloorAnimDelay = 0;
        gLightSaberFloorMode = (gLightSaberFloorMode == 1) ? 3 : 1;
        if (gLightSaberFloorMode == 3) {
            HideLightSaberFloorsIfOff(w, true);
            gLightSaberFloorWait = 200;
        } else {
            SW_ASSERT(gLightSaberFloorMode == 1);
            gLightSaberCurTranslucencyOffset = 2000;
            gLightSaberFloorCurTranslucency = 0;
            SWSetTileColorForRange(w, 0x2e9, 0x2f0, 0xffff, 0xffff, 0xffff,
                                   (unsigned short)(gLightSaberFloorCurTranslucency & 0xffff));
            gLightSaberFloorWait = 200;
        }
    }
    if (gLightSaberFloorMode == 1 && !gRoomIsDark) {
        Boolean changeImage = false;
        gLightSaberFloorAnimDelay += frames;
        if (gLightSaberFloorAnimDelay > 0) {
            gLightSaberFloorAnimDelay = 0;
            gLightSaberFloorCurTranslucency += gLightSaberCurTranslucencyOffset;
            if (gLightSaberFloorCurTranslucency >= 0xd888 && gLightSaberCurTranslucencyOffset >= 1) {
                gLightSaberFloorCurTranslucency = 0xd888;
                gLightSaberCurTranslucencyOffset = -gLightSaberCurTranslucencyOffset;
                changeImage = true;
            } else if (gLightSaberFloorCurTranslucency < 0x2556 && gLightSaberCurTranslucencyOffset < 0) {
                gLightSaberFloorCurTranslucency = 0x2555;
                gLightSaberCurTranslucencyOffset = -gLightSaberCurTranslucencyOffset;
            }
            SWSetTileTranslucencyForRange(w, 0x2e9, 0x2f0, (unsigned short)(gLightSaberFloorCurTranslucency & 0xffff));
            gLightSaberFloorTranslucency = gLightSaberFloorCurTranslucency;
            if (changeImage) {
                short step = (gLightSaberFloorMode == 2) ? 8 : -8;
                if (w->curTileImage[0x2e9] + step < 0x2e9 || w->curTileImage[0x2e9] + step > 0x307) {
                    for (i = 0x2e9; i < 0x2f1; i++)
                        SWChangeTileImageFast(w, i, i + 0x18);
                } else {
                    for (i = 0x2e9; i < 0x2f1; i++)
                        SWChangeTileImageFast(w, i, w->curTileImage[i] + step);
                }
            }
            MyUpdateTilesOnScreenFaster(w, 1, 4, 0x2e9, 0x2f0);
        }
    }
}

/// Changes a tile if it differs (and is inside the map).
void MyDrawTile(SpriteWorldPtr w, short tileLayer, short row, short col, short tileID)
{
    TileMapStructPtr t;

    SW_ASSERT(tileLayer >= 0 && tileLayer <= w->lastActiveTileLayer);
    t = w->tileLayerArray[tileLayer];
    if (row >= 0 && row < t->numRows && col >= 0 && col < t->numCols && t->tileMap[row][col] != tileID)
        SWDrawTile(w, tileLayer, row, col, tileID);
}

void ResetFloorTileAnimations(Boolean hideLightSaberFloors, Boolean resetLightSaberFloors)
{
    short i;

    gDisappearingFloorWait = 0x5a;
    gDisappearingFloorAnimDelay = 0;
    gDisappearingFloorMode = 3;
    for (i = 0x2c1; i < 0x2c9; i++)
        gSpriteWorldP->curTileImage[i] = i + 0x20;
    if (resetLightSaberFloors) {
        gLightSaberFloorWait = 0xf;
        gLightSaberFloorAnimDelay = 0;
        gLightSaberFloorCurTranslucency = 0;
        gLightSaberCurTranslucencyOffset = 0;
        gLightSaberFloorMode = 3;
        for (i = 0x2e9; i < 0x2f1; i++)
            gSpriteWorldP->curTileImage[i] = i;
        HideLightSaberFloorsIfOff(gSpriteWorldP, hideLightSaberFloors);
    }
}

/// Shows switched-off light-saber floors as dim, translucent tiles.
void HideLightSaberFloorsIfOff(SpriteWorldPtr spriteWorldP, Boolean unused)
{
    SW_UNUSED(unused);
    if (gLightSaberFloorMode != 1)
        SWSetTileColorForRange(spriteWorldP, 0x2e9, 0x2f0, 0x46ff, 0x46ff, 0x46ff, 0x94ff);
}

/// Returns the (original, un-tinted) tile ID at a position, or -1 outside the map.
short MyGetTile(SpriteWorldPtr w, short tileLayer, short row, short col)
{
    TileMapStructPtr t;

    SW_ASSERT(tileLayer >= 0 && tileLayer <= w->lastActiveTileLayer);
    t = w->tileLayerArray[tileLayer];
    if (row < 0 || row >= t->numRows || col < 0 || col >= t->numCols)
        return -1;
    return ConvertTileID(t->tileMap[row][col]);
}

// ---------------------------------------------------------------------------
// Draw procs

void DrawNothingDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    SW_UNUSED(srcSpriteP);
    SW_UNUSED(curFrameP);
    SW_UNUSED(horizLoc);
    SW_UNUSED(vertLoc);
}

/// Doors open and close by distorting their image (see BKBlitCustomDoorSurface).
void DoorSpriteDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    DoorSpritePtr doorP = (DoorSpritePtr)srcSpriteP;
    DrawDataPtr d = srcSpriteP->drawData;

    SW_UNUSED(horizLoc);
    SW_UNUSED(vertLoc);
    SW_ASSERT(srcSpriteP->userData >= 0x2d);
    SW_ASSERT(srcSpriteP->userData <= 0x44);
    glColor4us(d->colorRed, d->colorGreen, d->colorBlue, (GLushort)(d->translucencyLevel & 0xffff));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    if (gSWCurrentSpriteWorld != NULL) {
        if (d->autoScaling)
            glScalef(gSWCurrentSpriteWorld->designScaleH, gSWCurrentSpriteWorld->designScaleV, 1.0f);
        glScalef(gSWCurrentSpriteWorld->zoomH, gSWCurrentSpriteWorld->zoomV, 1.0f);
    }
    glTranslatef((float)srcSpriteP->destFrameRect.left, (float)srcSpriteP->destFrameRect.top, 0);
    if (d->scaledHeightFactor != 1.0f || d->scaledWidthFactor != 1.0f)
        glScalef(d->scaledWidthFactor, d->scaledHeightFactor, 1.0f);
    if (DoesSurfaceNeedBlend(curFrameP->glSurfaceP) || d->translucencyLevel != 0xffff) {
        glEnable(GL_BLEND);
        glBlendFunc(d->srcBlendMode, d->dstBlendMode);
    }
    BKBlitCustomDoorSurface(curFrameP->glSurfaceP, doorP->bottomInset, doorP->topInset);
    glDisable(GL_BLEND);
    glPopMatrix();
}

/*  Draws a door frame distorted for its open/close animation.
    stretchDown > 0 shifts the image down by that many pixels while keeping its top edge
    (the door slides into view); shrinkUp > 0 lifts the bottom edge, slanting it by 7
    pixels at the right. Texture coordinates beyond the image clamp to its edge. */
int BKBlitCustomDoorSurface(BKGLSurface *gls, short stretchDown, short shrinkUp)
{
    float w, h, s0 = 0, s1 = 1, tTop, tBottom;

    SW_ASSERT(gls != NULL);
    w = (float)gls->width;
    h = (float)gls->height;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gls->texture);
    glBegin(GL_TRIANGLES);
    if (shrinkUp < 1) {
        tTop = -stretchDown / h;
        tBottom = 1.0f;
        glTexCoord2f(s0, tTop);    glVertex2f(0, 0);
        glTexCoord2f(s1, tTop);    glVertex2f(w, 0);
        glTexCoord2f(s0, tBottom); glVertex2f(0, h + stretchDown);
        glTexCoord2f(s1, tTop);    glVertex2f(w, 0);
        glTexCoord2f(s0, tBottom); glVertex2f(0, h + stretchDown);
        glTexCoord2f(s1, tBottom); glVertex2f(w, h + stretchDown);
    } else {
        float tCut = (h - shrinkUp) / h;
        float s9 = 9.0f / w;
        float tSlant = (h - 7.0f - shrinkUp) / h;
        glTexCoord2f(s0, 0);       glVertex2f(0, 0);
        glTexCoord2f(s1, 0);       glVertex2f(w, 0);
        glTexCoord2f(s0, tCut);    glVertex2f(0, h - shrinkUp);
        glTexCoord2f(s1, 0);       glVertex2f(w, 0);
        glTexCoord2f(s0, tCut);    glVertex2f(0, h - shrinkUp);
        glTexCoord2f(s9, tCut);    glVertex2f(9, h - shrinkUp);
        glTexCoord2f(s1, 0);       glVertex2f(w, 0);
        glTexCoord2f(s9, tCut);    glVertex2f(9, h - shrinkUp);
        glTexCoord2f(s1, tSlant);  glVertex2f(w, h - shrinkUp - 7.0f);
    }
    glEnd();
    return 0;
}

/// Hides the part of Jack below the floor he is sinking into.
void ClipMainSpriteDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    HeroSpritePtr heroP = (HeroSpritePtr)srcSpriteP;

    SW_ASSERT(srcSpriteP->userData == 0);
    SWSetSpriteTemporaryClip(srcSpriteP, 0, 0, 0,
        (short)(int)((float)(srcSpriteP->destFrameRect.bottom - heroP->lavaSurfaceRow) *
                     (srcSpriteP->unscaledHeight / srcSpriteP->drawData->scaledHeight)));
    SWStdSpriteDrawProc(srcSpriteP, curFrameP, horizLoc, vertLoc);
}

/// Hides the part of a sprite that is below the surface of a lava tile.
void ClipSpriteWithLavaDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    short row, col, tile;
    short bottom = srcSpriteP->destFrameRect.bottom;
    short right = srcSpriteP->destFrameRect.right;
    short top = srcSpriteP->destFrameRect.top;
    short left = srcSpriteP->destFrameRect.left;

    if (bottom < 0) bottom += 15;
    if (right < 0) right += 15;
    if (top < 0) top += 15;
    if (left < 0) left += 15;
    for (row = (top >> 4) - 1; row <= bottom >> 4; row++) {
        for (col = (left >> 4) - 1; col <= right >> 4; col++) {
            tile = MyGetTile(gSpriteWorldP, 6, row, col);
            if (tile > 0x308 && tile < 0x331)
                goto found;
        }
    }
found:
    SWSetSpriteTemporaryClip(srcSpriteP, 0, 0, 0,
        (short)(int)((float)(srcSpriteP->destFrameRect.bottom - (short)(row * 16 - 3)) *
                     (srcSpriteP->unscaledHeight / srcSpriteP->drawData->scaledHeight)));
    SWStdSpriteDrawProc(srcSpriteP, curFrameP, horizLoc, vertLoc);
}

void AddShadowDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    SWStdSpriteDrawProc(srcSpriteP, curFrameP, horizLoc, vertLoc);
}

/// In rooms with background picture 1, enemies get a dark shadow offset down-right.
void EnemyShadowDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    DrawDataPtr d = srcSpriteP->drawData;

    if (gOldBackgroundPictResID == 1) {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        if (gSWCurrentSpriteWorld != NULL)
            glScalef(gSWCurrentSpriteWorld->designScaleH, gSWCurrentSpriteWorld->designScaleV, 1.0f);
        glTranslatef(horizLoc + 7.0f, vertLoc - 5.0f, 0);
        if (d->scaledHeightFactor != 1.0f || d->scaledWidthFactor != 1.0f)
            glScalef(d->scaledWidthFactor, d->scaledHeightFactor, 1.0f);
        glEnable(GL_BLEND);
        glBlendFunc(d->srcBlendMode, d->dstBlendMode);
        glColor4us(0, 0, 0, 0x5fff);
        BKBlitGLSurface(curFrameP->glSurfaceP);
        glDisable(GL_BLEND);
        glPopMatrix();
    }
    SWStdSpriteDrawProc(srcSpriteP, curFrameP, horizLoc, vertLoc);
}

// ---------------------------------------------------------------------------
// Pausing

void TogglePause(void)
{
    if (gCurrentGameMode == kReadingSignMode)
        CleanUpAfterSign();
    if (!gGameIsPaused)
        PauseGame();
    else
        ResumeGame();
}

/*  Pauses: dims the screen and, during play, shows the pause menu (resume, suspend,
    options, end game, quit) and the time spent in the mansion. */
void PauseGame(void)
{
    char text[255];
    SpritePtr dimP, pausedP, spriteP;
    short i, x, bottom, left;

    if (gEditorIsRunning || gDialogBoxIsOpen || gGameIsPaused)
        return;
    if (gCurrentGameMode == kReadingSignMode)
        CleanUpAfterSign();
    gMusicWasPlaying = IsMusicPlaying();
    gResetMusic = false;
    gSaveTileChangeProc = gSpriteWorldP->tileChangeProc;
    SWSetTileChangeProc(gSpriteWorldP, NULL);
    UpdateMenuBar();
    PauseMusic();
    MMPauseGameTimer();
    ClearCheatCodeCache();
    layersWerePausedForSaveAnim = gLayersArePausedForSaveAnim;
    PauseSpriteLayersForPause();
    StopAllChannels();
    if (!IsMoviePlaying())
        ResetKeys();
    SDL_EnableScreenSaver();
    gGameIsPaused = true;
    if (gCurrentGameMode == kInGameMapMode)
        DrawMapInOffscreenAreas();

    if (gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kInGameMapMode ||
        gCurrentGameMode == kSelectMansionMode) {
        if (SWCreateSprite(&dimP, NULL, 0) == kNoError) {
            SWSetSpriteDrawProc(dimP, SWSolidColorSpriteDrawProc);
            dimP->destFrameRect = gSpriteWorldP->workAreaRect;
            SWAddSprite(gPausedSpriteLayerP, dimP);
            SWSetSpriteColor(dimP, 0, 0, 0);
            SWSetSpriteTranslucencyLevel(dimP, 0x5555);
        }
    }
    if (gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kInGameMapMode) {
        MMPlaySound(0x38, 0, 1);
        pausedP = NewMiscSprite(0xd8);
        if (pausedP != NULL) {
            SWSetSpriteScaledFactor(pausedP, 0.9f, 0.9f);
            SWAddSprite(gPausedSpriteLayerP, pausedP);
            SWCenterSpriteInRect(pausedP, &gSpriteWorldP->workAreaRect);
            SWOffsetSprite(pausedP, 0.0f, -50.0f);
            bottom = (short)(int)(SWGetSpriteVertLoc(pausedP) + SWGetSpriteHeight(pausedP));
            left = (short)(int)SWGetSpriteHorizLoc(pausedP);
            x = 20;
            for (i = 0; i < 5; i++) {
                spriteP = NewButtonSprite(0xd9, i);
                if (spriteP == NULL)
                    continue;
                SWAddSprite(gPausedSpriteLayerP, spriteP);
                SWSetCurrentFrameIndex(spriteP, i);
                SetUpButtonSprite(spriteP, i, i + 5, i, i + 10);
                InstallButtonProc(spriteP, MMStandardButtonProc);
                SetButtonColor(spriteP, 0xffff, 0xffff, 0xffff);
                SetButtonPushedColor(spriteP, 0x8fff, 0x8fff, 0x8fff);
                SetButtonPushedOffset(spriteP, 1, 1);
                if (i == 1 && !CanSuspendGame())
                    SetButtonDimmedMode(spriteP, true);
                SWSetSpriteLocation(spriteP, (float)(left + x), SWGetSpriteHeight(spriteP) * -1.6f + (float)bottom);
                x = (short)(int)(SWGetSpriteWidth(spriteP) + 5.0f + (float)x);
            }
            spriteP = NewBitmapTextSprite(2000);
            if (spriteP != NULL) {
                snprintf(text, sizeof(text), "Elapsed time: %ld:%s%ld", gTimeSpentInMansion / 60,
                         (gTimeSpentInMansion % 60 < 10) ? "0" : "", gTimeSpentInMansion % 60);
                SetBMTextSpriteText(spriteP, text);
                SWSetSpriteScaledFactor(spriteP, 0.26f, 0.26f);
                SWAddSprite(gPausedSpriteLayerP, spriteP);
                SWCenterSpriteInRect(spriteP, &pausedP->destFrameRect);
                SWOffsetSprite(spriteP, GetWidthOfBMText(spriteP, text) / -2.0f, 40.0f);
            }
        }
    }
    MMSetCursorForGame();
    MMShieldCursor();
    DoGameFadeToNormal(0);
    SWUpdateSpriteWorld(gSpriteWorldP, true);
}

void ResumeGame(void)
{
    if (!gGameIsPaused)
        return;
    ExitPausedMode();
    if (gMusicWasPlaying)
        ResumeMusic(gResetMusic);
    MMResumeGameTimer();
    RemoveAllSpritesInLayer(gPausedSpriteLayerP);
    RemoveAllSpritesInLayer(gTextSpriteLayerP);
    if (!IsMoviePlaying())
        ResetKeys();
    if (gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kInGameMapMode)
        MMHideAllCursors();
    if (gCurrentGameMode == kPlayingGameMode) {
        DrawLevel();
    } else if (gCurrentGameMode == kInGameMapMode) {
        DrawMapInOffscreenAreas();
        SWUpdateSpriteWorld(gSpriteWorldP, true);
    } else if (gSpriteWorldIsValid) {
        SWUpdateSpriteWorld(gSpriteWorldP, true);
    }
    if (gCurrentGameMode == kInGameMapMode || gCurrentGameMode == kPlayingGameMode)
        DrawStatsArea();
}

void PauseSpriteLayersForPause(void)
{
    PauseAllSpriteLayers(gSpriteWorldP);
    SWUnpauseSpriteLayer(gPausedSpriteLayerP);
    SWUnpauseSpriteLayer(gCustomCursorSpriteLayerP);
}

void HandlePausedScreenCallback(void)
{
    SWPoint mouse;
    Boolean button;
    short index;

    KeepMusicPlaying();
    SWProcessSpriteWorld(gSpriteWorldP);
    SWAnimateSpriteWorld(gSpriteWorldP);
    if (!gSpriteWorldP->frameHasOccurred)
        return;
    button = MMGetMouse(&mouse);
    SWConvertCoordinates(gSpriteWorldP, &mouse);
    index = ProcessButtonSprites(gPausedSpriteLayerP, gPausedSpriteLayerP, mouse, button, false, false);
    if (index == -1)
        return;
    AnimateOneFrame(gSpriteWorldP);
    switch (index) {
        case 0: ResumeGame(); break;
        case 1: ExitPausedMode(); SuspendGame(); DoEndGame(false); break;
        case 2: MMDoOptionsDialog(); break;
        case 3: DoEndGame(false); break;
        case 4: DoQuitCommand(false); break;
    }
}

void ExitPausedMode(void)
{
    if (!gGameIsPaused)
        return;
    gGameIsPaused = false;
    SWSetTileChangeProc(gSpriteWorldP, gSaveTileChangeProc);
    UnpauseAllSpriteLayers(gSpriteWorldP);
    if (layersWerePausedForSaveAnim)
        PauseSpriteLayersForSaveAnim();
    if (!IsMoviePlaying()) {
        ResetKeys();
        WriteEventToMovieFile(0xb, true);
    }
    InitGamePad();
    SDL_DisableScreenSaver();
}

void DoEndGame(Boolean askFirst)
{
    short choice = askFirst ? DoSuspendGameDialog() : 3;

    if (choice == 2 || choice == 3) {
        if (gCurrentGameMode == kInGameMapMode)
            ExitInGameMapScreen(false);
        DoGameFadeToBlack(0x1e, true);
        gCurrentGameMode = kNoGameMode;
        gSpriteWorldIsValid = false;
        MMHideCursor();
        CleanUpAfterGame();
        ExitPausedMode();
        UnpauseAllSpriteLayers(gSpriteWorldP);
        SetUpTitleScreen();
    }
}

void DoQuitCommand(Boolean askFirst)
{
    short choice;

    if (gUsingEditor)
        askFirst = false;
    choice = askFirst ? DoSuspendGameDialog() : 3;
    if (choice != 2 && choice != 3)
        return;
    ExitPausedMode();
    if (gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kInGameMapMode)
        WriteCStringToFile("Quit Midnight Mansion.\r");
    if (gUsingEditor) {
        gQuitGame = true;
    } else if (gCurrentGameMode == kTeaserScreenMode) {
        if (gCanQuit)
            gQuitGame = true;
    } else {
        MMFlushEvents();
        gQuitGame = true;
        if (DoTeaserScreen(true))
            gQuitGame = false;
    }
}

// ---------------------------------------------------------------------------
// Fades

void DoGameFadeToBlack(short numFrames, Boolean fadeMusic)
{
    if (gSpriteWorldP == NULL)
        return;
    if (fadeMusic) {
        if (numFrames < 2)
            StopAndDisposeMusic();
        else
            FadeMusicNowNoAdjust((short)(numFrames << 4));
    }
    DoFadeToBlack(numFrames, false);
    if (fadeMusic)
        StopAndDisposeMusic();
}

void DoGameFadeToBlackFullAnim(short numFrames, Boolean fadeMusic)
{
    if (gSpriteWorldP == NULL)
        return;
    if (fadeMusic) {
        if (numFrames < 2)
            StopAndDisposeMusic();
        else
            FadeMusicNowNoAdjust((short)(numFrames << 4));
    }
    DoFadeToBlack(numFrames, true);
    if (fadeMusic)
        StopAndDisposeMusic();
}

void DoGameFadeToNormal(short numFrames)
{
    if (gSpriteWorldP != NULL)
        DoFadeToNormal(numFrames, false);
}

void DoGameFadeToNormalFullAnim(short numFrames)
{
    if (gSpriteWorldP != NULL)
        DoFadeToNormal(numFrames, true);
}

/*  Fades to black over numFrames frames with a black sprite on top of everything (and
    one in the stats bar). Unless animateEverything is set, only the fade and cursor
    layers keep running meanwhile. */
void DoFadeToBlack(short numFrames, Boolean animateEverything)
{
    TileChangeProcPtr savedTileChangeProc;

    SWUnpauseSpriteLayer(gFadeToBlackSpriteLayerP);
    if (SWGetNextSprite(gFadeToBlackSpriteLayerP, NULL) != NULL)
        return;
    savedTileChangeProc = gSpriteWorldP->tileChangeProc;
    SWSetTileChangeProc(gSpriteWorldP, NULL);
    NewFadeSprite(gSpriteWorldP, numFrames);
    NewFadeSprite(gStatsSpriteWorldP, numFrames);
    while (IsCurrentlyFadingToBlack() && !gQuitGame) {
        if (!animateEverything) {
            SWProcessSpriteWorldForOneLayer(gSpriteWorldP, gFadeToBlackSpriteLayerP);
            SWProcessSpriteLayer(gSpriteWorldP, gCustomCursorSpriteLayerP);
            if (IsStatsAreaVisible())
                SWProcessSpriteWorldForOneLayer(gStatsSpriteWorldP, gStatsFadeSpriteLayerP);
        } else {
            SWProcessSpriteWorld(gSpriteWorldP);
            if (IsStatsAreaVisible())
                SWProcessSpriteWorld(gStatsSpriteWorldP);
        }
        if (!gSpriteWorldP->frameHasOccurred)
            KeepMusicPlaying();
        else
            ProcessSDLEvents();
        SWAnimateSpriteWorld(gSpriteWorldP);
    }
    SWSetTileChangeProc(gSpriteWorldP, savedTileChangeProc);
}

/// A fade steps every 16 ms; in a game on several computers once a frame instead, so every
/// computer runs the same number of frames meanwhile (the game clock stands still in fades).
static long FadeMoveTime(void)
{
    return gSWUseGameClock ? 0 : 16;
}

/// Fades the black overlay out again (instantly if numFrames < 1).
void DoFadeToNormal(short numFrames, Boolean animateEverything)
{
    SpritePtr fadeP, statsFadeP;
    TileChangeProcPtr savedTileChangeProc;

    SWUnpauseSpriteLayer(gFadeToBlackSpriteLayerP);
    fadeP = SWGetNextSprite(gFadeToBlackSpriteLayerP, NULL);
    statsFadeP = SWGetNextSprite(gStatsFadeSpriteLayerP, NULL);
    if (statsFadeP == NULL && IsStatsAreaVisible()) {
        SWDisposeAllSpritesInLayer(gStatsFadeSpriteLayerP);
        statsFadeP = NewFadeSprite(gStatsSpriteWorldP, numFrames);
    }
    if (statsFadeP != NULL)
        SWSetSpriteTranslucencyLevel(statsFadeP, 0xffff);
    if (fadeP != NULL) {
        SWSetRect(&fadeP->destFrameRect, 0, 0, gSpriteWorldP->designWidth, gSpriteWorldP->designHeight);
        if (numFrames < 1) {
            SWRemoveSprite(fadeP);
            SWDisposeSprite(&fadeP);
            SWDisposeAllSpritesInLayer(gFadeToBlackSpriteLayerP);
            SWUpdateSpriteWorld(gSpriteWorldP, true);
            if (statsFadeP != NULL) {
                SWDisposeAllSpritesInLayer(gStatsFadeSpriteLayerP);
                SWUpdateSpriteWorld(gStatsSpriteWorldP, false);
            }
            return;
        }
        fadeP->userData = 0xffff / numFrames;
        SWSetSpriteMoveProc(fadeP, FadeToNormalMoveProc);
        SWSetSpriteMoveTime(fadeP, FadeMoveTime());
    }
    if (statsFadeP != NULL) {
        if (numFrames < 1) {
            SWDisposeAllSpritesInLayer(gStatsFadeSpriteLayerP);
            SWUpdateSpriteWorld(gStatsSpriteWorldP, false);
            return;
        }
        statsFadeP->userData = 0xffff / numFrames;
        SWSetSpriteMoveProc(statsFadeP, FadeToNormalMoveProc);
        SWSetSpriteMoveTime(statsFadeP, FadeMoveTime());
    }
    savedTileChangeProc = gSpriteWorldP->tileChangeProc;
    SWSetTileChangeProc(gSpriteWorldP, NULL);
    while (IsCurrentlyFadingToBlack() && !gQuitGame) {
        if (!animateEverything) {
            SWProcessSpriteWorldForOneLayer(gSpriteWorldP, gFadeToBlackSpriteLayerP);
            SWProcessSpriteLayer(gSpriteWorldP, gCustomCursorSpriteLayerP);
            if (statsFadeP != NULL)
                SWProcessSpriteWorldForOneLayer(gStatsSpriteWorldP, gStatsFadeSpriteLayerP);
        } else {
            SWProcessSpriteWorld(gSpriteWorldP);
            if (statsFadeP != NULL)
                SWProcessSpriteWorld(gStatsSpriteWorldP);
        }
        if (!gSpriteWorldP->frameHasOccurred)
            KeepMusicPlaying();
        else
            ProcessSDLEvents();
        SWAnimateSpriteWorld(gSpriteWorldP);
    }
    SWSetTileChangeProc(gSpriteWorldP, savedTileChangeProc);
    SWDisposeAllSpritesInLayer(gFadeToBlackSpriteLayerP);
    SWDisposeAllSpritesInLayer(gStatsFadeSpriteLayerP);
}

/// userData holds the translucency step per move.
void FadeToBlackMoveProc(SpritePtr srcSpriteP)
{
    unsigned long level = SWGetSpriteTranslucencyLevel(srcSpriteP);

    if (level < 0xffffUL - (unsigned long)srcSpriteP->userData) {
        SWSetSpriteTranslucencyLevel(srcSpriteP, srcSpriteP->userData + level);
        SWInvalidateSprite(srcSpriteP);
    } else {
        SWSetSpriteMoveProc(srcSpriteP, NULL);
        SWSetSpriteFrameTime(srcSpriteP, -1);
        SWSetSpriteTranslucencyLevel(srcSpriteP, 0xffff);
    }
}

void FadeToNormalMoveProc(SpritePtr srcSpriteP)
{
    unsigned long level = SWGetSpriteTranslucencyLevel(srcSpriteP);

    if ((unsigned long)srcSpriteP->userData < level) {
        SWSetSpriteTranslucencyLevel(srcSpriteP, level - srcSpriteP->userData);
        SWInvalidateSprite(srcSpriteP);
    } else {
        SWRemoveSprite(srcSpriteP);
        SWDisposeSprite(&srcSpriteP);
    }
}

Boolean IsCurrentlyFadingToBlack(void)
{
    SpritePtr fadeP = SWGetNextSprite(gFadeToBlackSpriteLayerP, NULL);
    return fadeP != NULL && fadeP->spriteMoveProc != NULL;
}

SpritePtr NewFadeSprite(SpriteWorldPtr spriteWorldP, short numFrames)
{
    SpritePtr fadeP = NULL;

    if (spriteWorldP == NULL)
        return NULL;
    if (SWCreateSprite(&fadeP, NULL, 0) != kNoError)
        return NULL;
    SWSetSpriteDrawProc(fadeP, SWSolidColorSpriteDrawProc);
    SWSetRect(&fadeP->destFrameRect, 0, 0, spriteWorldP->designWidth, spriteWorldP->designHeight);
    if (spriteWorldP == gSpriteWorldP)
        SWAddSprite(gFadeToBlackSpriteLayerP, fadeP);
    else
        SWAddSprite(gStatsFadeSpriteLayerP, fadeP);
    SWSetSpriteColor(fadeP, 0, 0, 0);
    SWSetSpriteTranslucencyLevel(fadeP, 0);
    if (numFrames < 1) {
        SWSetSpriteTranslucencyLevel(fadeP, 0xffff);
    } else {
        fadeP->userData = 0xffff / numFrames;
        SWSetSpriteMoveProc(fadeP, FadeToBlackMoveProc);
        SWSetSpriteMoveTime(fadeP, FadeMoveTime());
    }
    return fadeP;
}

// ---------------------------------------------------------------------------
// Cursor

// The game's own cursor is a sprite, so it can only move once per animation frame, which
// looks choppy next to the system cursor (especially on 120 Hz displays). When the game
// cursor would be showing, the same image is handed to the OS as a colour cursor instead
// and the sprite itself is not drawn.

static SDL_Cursor *gHWGameCursor = NULL;
static int gHWGameCursorSize = 0;
static SDL_Surface *gHWGameCursorImage = NULL;
static Boolean gHWGameCursorFailed = false;
static Boolean gHWGameCursorActive = false;

static void HWCursorNoDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    (void)srcSpriteP; (void)curFrameP; (void)horizLoc; (void)vertLoc;
}

/// Bilinear resample of an RGBA image (premultiplying so edges stay clean).
static SDL_Surface *ScaleCursorImage(SDL_Surface *src, int size)
{
    SDL_Surface *dst = SDL_CreateRGBSurfaceWithFormat(0, size, size, 32, SDL_PIXELFORMAT_RGBA32);
    int x, y, c;

    if (dst == NULL)
        return NULL;
    for (y = 0; y < size; y++) {
        for (x = 0; x < size; x++) {
            float fx = ((float)x + 0.5f) * (float)src->w / (float)size - 0.5f;
            float fy = ((float)y + 0.5f) * (float)src->h / (float)size - 0.5f;
            int x0 = (int)SDL_floorf(fx), y0 = (int)SDL_floorf(fy);
            float tx = fx - (float)x0, ty = fy - (float)y0, acc[4] = {0, 0, 0, 0};
            int i, j;
            Uint8 *out = (Uint8 *)dst->pixels + y * dst->pitch + x * 4;
            for (j = 0; j < 2; j++) {
                for (i = 0; i < 2; i++) {
                    int sx = SDL_clamp(x0 + i, 0, src->w - 1), sy = SDL_clamp(y0 + j, 0, src->h - 1);
                    const Uint8 *in = (const Uint8 *)src->pixels + sy * src->pitch + sx * 4;
                    float wgt = (i ? tx : 1.0f - tx) * (j ? ty : 1.0f - ty);
                    float a = (float)in[3] / 255.0f;
                    for (c = 0; c < 3; c++)
                        acc[c] += (float)in[c] * a * wgt;
                    acc[3] += a * wgt;
                }
            }
            for (c = 0; c < 3; c++)
                out[c] = (Uint8)(acc[3] > 0.0f ? SDL_min(255.0f, acc[c] / acc[3] + 0.5f) : 0);
            out[3] = (Uint8)(acc[3] * 255.0f + 0.5f);
        }
    }
    return dst;
}

/// Returns true if the OS cursor now shows the game cursor.
static Boolean UseHardwareGameCursor(void)
{
    float scaleH, scaleV;
    int size, winW, winH, drawW, drawH;
    SDL_Surface *scaled;

    if (gHWGameCursorFailed || gSpriteWorldP == NULL)
        return false;
    if (gHWGameCursorImage == NULL) {
        gHWGameCursorImage = BKLoadSurface(GetSpriteImagePath("00140.png"));
        if (gHWGameCursorImage == NULL || gHWGameCursorImage->format->BytesPerPixel != 4) {
            gHWGameCursorFailed = true;
            return false;
        }
    }
        // the sprite is 64x64 drawn at half size in design units, mouse point 4,4 from its corner
    SWGetWindowScalingFactor(gSpriteWorldP, &scaleH, &scaleV);
    SDL_GetWindowSize(gSpriteWorldP->mainWindow, &winW, &winH);
    SDL_GL_GetDrawableSize(gSpriteWorldP->mainWindow, &drawW, &drawH);
    if (drawW > 0 && winW > 0)
        scaleH = scaleH * (float)winW / (float)drawW;
    size = (int)(32.0f * scaleH + 0.5f);
    if (size < 8)
        size = 8;
    if (size != gHWGameCursorSize || gHWGameCursor == NULL) {
        scaled = ScaleCursorImage(gHWGameCursorImage, size);
        if (scaled == NULL) {
            gHWGameCursorFailed = true;
            return false;
        }
        if (gHWGameCursor != NULL)
            SDL_FreeCursor(gHWGameCursor);
        gHWGameCursor = SDL_CreateColorCursor(scaled, (int)(4.0f * scaleH + 0.5f), (int)(4.0f * scaleH + 0.5f));
        SDL_FreeSurface(scaled);
        if (gHWGameCursor == NULL) {
            gHWGameCursorFailed = true;
            return false;
        }
        gHWGameCursorSize = size;
    }
    SDL_SetCursor(gHWGameCursor);
    SDL_ShowCursor(SDL_ENABLE);
    return true;
}

/// Called after events are handled: shows the game cursor through the OS when the game
/// cursor sprite would be visible, otherwise restores the normal arrow. While a controller
/// drives the menus there is no cursor at all.
void MMSyncHardwareCursor(void)
{
    static Boolean hiddenForPad = false, spriteWasVisible = false;
    static int systemCursorWasShown = SDL_ENABLE;
    SpritePtr cursorP = NULL;
    Boolean wantGame;

    if (gSpriteWorldP != NULL && gCustomCursorSpriteLayerP != NULL)
        cursorP = SWGetNextSprite(gCustomCursorSpriteLayerP, NULL);
    if (gControllerHidesCursor) {
        if (!hiddenForPad) {
            hiddenForPad = true;
            systemCursorWasShown = gHWGameCursorActive ? SDL_DISABLE : SDL_ShowCursor(SDL_QUERY);
            spriteWasVisible = cursorP != NULL && cursorP->isVisible;
            gHWGameCursorActive = false;
            SDL_SetCursor(SDL_GetDefaultCursor());
        }
        if (cursorP != NULL && cursorP->isVisible)
            SWSetSpriteVisible(cursorP, false);
        SDL_ShowCursor(SDL_DISABLE);
        return;
    }
    if (hiddenForPad) {
        hiddenForPad = false;
        if (cursorP != NULL && spriteWasVisible)
            SWSetSpriteVisible(cursorP, true);
        SDL_ShowCursor(systemCursorWasShown);
    }

    wantGame = cursorP != NULL && cursorP->isVisible && gCustomCursorSpriteLayerP->layerIsVisible &&
               gCursorIsInWindow;
    if (wantGame && UseHardwareGameCursor()) {
        SWSetSpriteDrawProc(cursorP, HWCursorNoDrawProc);
        gHWGameCursorActive = true;
        return;
    }
    if (gHWGameCursorActive) {
        gHWGameCursorActive = false;
        SDL_SetCursor(SDL_GetDefaultCursor());
        if (cursorP != NULL)
            SDL_ShowCursor(SDL_DISABLE);
    }
}

void MMSetCursorForGame(void)
{
    if (!gUseCustomCursor) {
        MMInitCursor();
    } else {
        AddCustomCursorToScreen();
        gCustomCursorIsVisible = true;
        if (!gCursorIsShielded)
            MMHideCursor();
    }
}

void MMSetCursorForSystem(void)
{
    MMRemoveGameCursor();
    MMInitCursor();
}

void MMRemoveGameCursor(void)
{
    if (gSpriteWorldP != NULL) {
        RemoveAllSpritesInLayer(gCustomCursorSpriteLayerP);
        gCustomCursorIsVisible = false;
    }
}

void MMSaveCursorState(void)
{
    if (gSpriteWorldP == NULL)
        gSaveCustomCursorIsVisible = false;
    else
        gSaveCustomCursorIsVisible = SWGetNextSprite(gCustomCursorSpriteLayerP, NULL) != NULL;
    gSaveSystemCursorIsVisible = IsCursorVisible();
    gSaveSystemCursorIsShielded = gCursorIsShielded;
}

void MMRestoreCursorState(void)
{
    if (!gSaveCustomCursorIsVisible) {
        if (!gSaveSystemCursorIsVisible) {
            MMHideAllCursors();
        } else {
            MMRemoveGameCursor();
            if (!gSaveSystemCursorIsShielded)
                MMSetCursorForSystem();
            else
                MMHideCursor();
        }
    } else {
        if (gSaveSystemCursorIsShielded)
            MMShieldCursor();
        MMSetCursorForGame();
    }
    if (CurrentScreenHasCustomCursor()) {
        MMSetCursorForGame();
        MMShieldCursor();
    }
}

void MMHideAllCursors(void)
{
    if (gSpriteWorldP != NULL)
        RemoveAllSpritesInLayer(gCustomCursorSpriteLayerP);
    gCustomCursorIsVisible = false;
    MMHideCursor();
}

void MMInitCursor(void)
{
    gCursorIsShielded = false;
    MyInitCursor();
}

void MMHideCursor(void)
{
    if (!gFullScreenWindow) {
        MMShieldCursor();
    } else {
        gCursorIsShielded = false;
        MyHideCursor();
    }
}

/// "Shielded": the system cursor is hidden while over the window (the game draws its own).
void MMShieldCursor(void)
{
    gCursorIsShielded = true;
    MMHandleCursorMovement();
}

Boolean IsCursorShielded(void)
{
    return gCursorIsShielded;
}

void MMHandleCursorMovement(void)
{
    SpritePtr cursorP;

    if (!gCursorIsShielded)
        return;
    cursorP = gCustomCursorSpriteLayerP ? SWGetNextSprite(gCustomCursorSpriteLayerP, NULL) : NULL;
    if (!gCursorIsInWindow) {
        SDL_ShowCursor(SDL_ENABLE);
        if (cursorP != NULL)
            SWSetSpriteVisible(cursorP, false);
    } else {
        SDL_ShowCursor(SDL_DISABLE);
        if (cursorP != NULL)
            SWSetSpriteVisible(cursorP, true);
    }
}

Boolean CurrentScreenHasCustomCursor(void)
{
    switch (gCurrentGameMode) {
        case kPlayingGameMode:
        case kInGameMapMode:
        case kReadingSignMode:
        case kOrganScreenMode:
        case kBeatTheMansionMode:
            return false;
    }
    return true;
}

void MyGameBeep(void)
{
    MMPlaySound(0, 8, 3);
}

// ---------------------------------------------------------------------------
// Errors

static const char *MMBaseName(const char *path)
{
    const char *slash = strrchr(path, '/');
    return slash ? slash + 1 : path;
}

void MMAssertFailCallback(const char *file, const char *function, int line)
{
    char message[500];

    if (gSpriteWorldP == NULL)
        return;
    snprintf(message, sizeof(message),
             "Sorry! An assertion error has occurred. This program must now quit.\r\rFile: %s \rFunction: %s \rLine: %d\n",
             MMBaseName(file), function, line);
    if (gSpriteArray[0xdc] != NULL && gSpriteArray[0xda] != NULL) {
        MMDoCustomDialog(gSpriteWorldP, message, 3);
        exit(1);
    }
    printf("%s", message);
}

/*  Reports a fatal error. As in the original, it only quits if the dialog sprites are
    loaded; otherwise it prints the message and returns. */
void MMFatalErrorCallback(long errNum, const char *file, const char *function, int line)
{
    char message[500];

    snprintf(message, sizeof(message),
             "Sorry! A fatal error has occurred.\r\rError code: %ld \rFile: %s \rFunction: %s \rLine: %d\n",
             errNum, MMBaseName(file), function, line);
    if (gSpriteWorldP != NULL && gSpriteArray[0xdc] != NULL && gSpriteArray[0xda] != NULL) {
        MMDoCustomDialog(gSpriteWorldP, message, 3);
    }
    fprintf(stderr, "%s", message);
    exit(1);
}

void ErrorMessage(const char *format, long value)
{
    char message[500];

    if (value == 0)
        snprintf(message, sizeof(message), "%s", format);
    else
        snprintf(message, sizeof(message), format, value);
    if (gSpriteWorldP == NULL)
        printf("%s\n", message);
    else
        MMDoCustomDialog(gSpriteWorldP, message, 3);
}
