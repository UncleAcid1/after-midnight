/*
 * Midnight Mansion HD — shared declarations.
 *
 * Reconstructed from the Mac build (v1.0.1b). Each section lists the globals and
 * functions of one original source file, in the order of that file.
 */
#ifndef MMGAME_H
#define MMGAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL.h>
#include "SpriteWorld.h"

// ---------------------------------------------------------------------------
// Basic types (classic Mac names kept from the original code)

typedef unsigned char Boolean;
typedef unsigned char Str255[256];
typedef unsigned char *StringPtr;
typedef const unsigned char *ConstStringPtr;
typedef short OSErr;

#ifndef noErr
#define noErr 0
#endif

/// Fatal error report with the source position, like the original's MMFatalErrorCallback.
#define MM_FATAL(err) MMFatalErrorCallback((err), __FILE__, __func__, __LINE__)
#define MM_FATAL_IF(err) do { SWError e_ = (err); if (e_ != kNoError) MM_FATAL(e_); } while (0)

// ---------------------------------------------------------------------------
// Game modes (gCurrentGameMode)

enum {
    kTitleScreenMode        = 0,
    kDifficultyScreenMode   = 1,
    kSelectMansionMode      = 2,
    kHelpScreenMode         = 3,
    kHighScoresScreenMode   = 4,
    kPlayingGameMode        = 5,
    kInGameMapMode          = 6,
    kGameOverScreenMode     = 7,
    kAboutScreenMode        = 8,
    kBeatTheMansionMode     = 9,
    kBeatTheGameMode        = 10,
    kOrganScreenMode        = 11,
    kStoryScreenMode        = 12,
    kTeaserScreenMode       = 13,
    kReadingSignMode        = 14,
    kNoGameMode             = 15,
    kMultiplayerScreenMode  = 16,   ///< added: the multiplayer screen (MMMultiplayerScreen.c)
    kQuittingMode           = 26
};

// ---------------------------------------------------------------------------
// Structures

/// Keyboard configuration (written to the prefs file as-is).
typedef struct KeyPrefsRec {
    long primaryKeys[9];        ///< SDL keycodes: left, right, up, down, jump, ... (see MMEvents.c)
    long secondaryKeys[9];
    char gamepadNames[9][50];
    char reserved[2];
} KeyPrefsRec;

/// gKeys[] indices (also KeyPrefsRec order).
enum {
    kKeyUp = 0, kKeyRight = 1, kKeyDown = 2, kKeyLeft = 3, kKeyJump = 4,
    kKeyJumpLeft = 5, kKeyJumpRight = 6, kKeyAction = 7, kKeyMap = 8,
    kKeyZoomIn = 9, kKeyZoomOut = 10
};

/// Inventory items (ObjectsCarriedRec.counts index and stats-bar icon).
enum {
    kRedKeyItem = 0, kBlueKeyItem = 1, kGreenKeyItem = 2, kPurpleKeyItem = 3,
    kYellowKeyItem = 4, kMasterKeyItem = 5, kShieldItem = 6
};

/// Items Jack is carrying.
typedef struct ObjectsCarriedRec {
    short counts[7];
    Boolean hasLantern;
    Boolean hasBackpack;      ///< 8 inventory slots instead of 6
} ObjectsCarriedRec;

/// 'SPRT' resource entry (one per sprite placed in a room; big-endian on disk).
typedef struct SPRTRecord {
    short   type;
    short   v;
    short   h;
    short   param3, param4, param5;
    Sint32  objectID;
    short   param8, param9;         ///< for levers and switches, the target's object ID
    short   param10;
} SPRTRecord;

/// 'LEVL' resource (0x2c bytes, big-endian on disk).
typedef struct LEVLRec {
    Uint8  isDark;              // 0x00
    Uint8  isSecret;            // 0x01
    short  musicIndex;          // 0x02
    short  unknownShorts[8];    // 0x04..0x13 (swapped together with musicIndex)
    Uint8  unknown14;           // 0x14
    Uint8  hasOutdoorShadows;   // 0x15
    Uint8  noShadows;           // 0x16
    Uint8  noCoinGlows;         // 0x17
    Uint8  noKeyGlows;          // 0x18
    Uint8  padding[0x2c - 0x19];
} LEVLRec;

extern LEVLRec gLEVLStruct;

// Globals shared between the game files (the original declared these in each file).
extern SDL_Color gSDLWhiteColor, gSDLTealColor, gSDLPurpleColor, gSDLBlackColor;   // NewSprite.c
extern Boolean gFileIsOpen;                             // GameSupport.c (output log)
extern FILE   *gOutputFileP;
extern unsigned char gSuspendedGameFilenameHFSUni[512];
extern Boolean gIsEnteringMapScreen;                    // GameMap.c

// ---------------------------------------------------------------------------
// Midnight Mansion.c

extern Boolean gDoOpeningMusic;
extern Boolean gShowEpisode2LinkedMessage;
extern Boolean gShowEpisode1LinkedMessage;
extern long gLightSaberFloorTranslucency;
extern Boolean gIgnoreCheating;
extern Boolean gDidFadeAllMonitors;
extern Boolean gCustomCursorIsVisible;
extern Boolean gCursorIsShielded;
extern long gNumTilesDrawnThisFrame;

extern short gAppResFile, gGraphicsFile, gSpritesFile, gSoundsFile;
extern Boolean gArrowKeysAreEnabledForTesting, gArrowKeysEnabled;
extern short gBackgroundMusicVolume, gTitleMusicVolume, gSoundVolume;
extern Boolean gBeatGameScreenWasDismissed, gBeatMansionScreenWasDismissed, gBeatTheMansion;
extern Boolean gCanControlWhileJumping, gCanPlayBothEpisodes, gCanQuit, gCanUseLadderJump;
extern short gCurConveyorBeltDirection, gOldConveyorBeltDirection;
extern Boolean gCurMansionUsesWaterSound;
extern short gCurNumStatsSlots;
extern char gCurOpenMansionFileName[256];
extern char gCurOpenMansionFilePath[600];
extern short gCurrentFPSValue;
extern long gCurrentGameMode;
extern short gCurrentLevel;
extern short gCurrentMansionNumber;
extern short gCurrentMusicIndex, gLastMusicIndex;
extern char gCurrentPlayerName[256];
extern Boolean gCurrentPlayerNameExists;
extern Boolean gCurrentRoomHasLava, gCurrentRoomHasWind;
extern long gCurrentSaveIndex;
extern float gCurrentWindDelta, gMaxWindDelta, gMinWindDelta, gTargetWindDelta, gWindDeltaChange;
extern long gNextWindChange;
extern Boolean gCurrentlyInMansion;
extern Boolean gCursorIsInWindow;
extern Boolean gControllerHidesCursor;   ///< a controller is driving the UI: no mouse cursor
extern SWRect gCursorShieldRect;
extern Boolean gCustomDialogIsVisible;
extern char gCustomMansionFolderPath[800];
extern Str255 gDefaultHighScoreName;
extern short gDelayUntilUpdatingFPSValue;
extern Boolean gDemoMode, gDialogBoxIsOpen, gDidCheat;
extern SpritePtr gDifficultyScreenSpriteArray[5];
extern char gDifficultySetting;
extern Boolean gDisableCoinGlows, gDisableDynamicShadows, gDisableKeyGlows;
extern short gDisappearingFloorAnimDelay, gDisappearingFloorMode, gDisappearingFloorWait;
extern Boolean gDisplayFPSAndLevelResID, gDoFrameSkipping;
extern Boolean gEasyJumping, gEditorIsRunning, gEndGame;
extern Boolean gEpisode1IsUnlocked, gEpisode2IsUnlocked;
extern Boolean gEscapeKey;
extern Boolean gFamilyFriendly, gFasterInGameMap;
extern Boolean gFullScreenWindow, gFullScreenWindowPrefs;
extern Boolean gGameIsPaused, gGameOver, gGameTimerIsPaused;
extern Boolean gGamepad[12];
extern Boolean gGoldFloorIsActive, gSilverFloorIsActive;
extern Boolean gHasShownOrganScreen, gHeroIsOnConveyorBelt, gHeroLandedThisFrame;
extern float gHorizontalBoost, gJumpChange, gJumpSpeed;
extern Boolean gKeyIsHiddenInRoom;
extern KeyPrefsRec gKeyPrefs;
extern Boolean gKeys[16];
extern short gLastLevelExited;
extern short gLastLevelPositionWasSaved;
extern short gLastOpenDialog;
extern char gLastSaveFilename[400];
extern long gLastSavePedestalObjectID;
extern short gLavaSnd;
extern Boolean gLayersArePausedForSaveAnim;
extern short gLightSaberCurTranslucencyOffset, gLightSaberFloorAnimDelay, gLightSaberFloorMode, gLightSaberFloorWait;
extern long gLightSaberFloorCurTranslucency;
extern Boolean gLimitFullscreenResolution;
extern Boolean gMMIsActive, gMainSpritesAreLoaded;
extern char gMansionFilename[255];
extern Boolean gMansionIsCustomMansion;
extern char gMansionName[256];
extern SpritePtr gMapScreenSpriteArray[16];
extern Boolean gMenuBarIsVisible;
extern long gModifiers;
extern Boolean gMusicIsOn, gSofterMusic, gSoundIsTurnedOn;
extern long gMyOldNumFrames, gMyOldTicks, gNumFrames, gNumFramesSinceEnteringRoom;
extern short gNumItemsBeingCarried, gNumLivesLeft;
extern long gNumSecretsFound, gTotalSecretsInMansion, gNumWeatherSpritesOnGround;
extern ObjectsCarriedRec gObjects;
extern short gOldBackgroundPictResID;
extern long gScore, gOldScore;
extern void *gOpeningMusicRefNum;
extern SpritePtr gOrganScreenSpriteArray[3];
extern Boolean gOrganScreenWasDismissed;
extern short gOrigTileIDs[0xf7a];
extern Boolean gPheliosDistribution, gPlayTutorialMansion, gPlayingGame, gPlayingGameMovie;
extern short gPrevWindowHeight, gPrevWindowWidth;
extern Boolean gQuitGame;
extern SWRGBColor gRGBBlackColor, gRGBWhiteColor, gWindowColor;
extern Boolean gReadingSignWasDismissed, gRecordMovies, gRemovedGlowsAndShadows, gRestartLevel;
extern Boolean gRoomIsDark, gRoomWasDark;
extern SWRect gRoomMoveBoundsRect;
extern long gRunningFrameCount;
extern long gSaveCurrentGameMode;
extern Boolean gSaveCustomCursorIsVisible, gSaveSystemCursorIsShielded, gSaveSystemCursorIsVisible;
extern TileChangeProcPtr gSaveTileChangeProc;
extern Boolean gShowSecrets;
#define kNumSpriteArrayEntries 229   ///< 227, 228: Fun and Extras option tabs (this version)
extern SpritePtr gSpriteArray[kNumSpriteArrayEntries];
extern DrawProcPtr gSpriteDrawProc, gTranslucentMaskDrawProc;
extern Boolean gSpriteWorldIsValid;
extern SpriteWorldPtr gSpriteWorldP, gStatsSpriteWorldP;
extern Boolean gStartRecordingMovie;
extern short gStartingCol, gStartingRow, gStartingSpriteState, gStartingVineTileLayer;
extern Boolean gStartingFacingRight;
extern short gStatsHeight;
extern short gStatsItemArray[13];
extern SpritePtr gStatsItemSpriteArray[13];
extern SpritePtr gStatsSpriteArray[2];
extern Boolean gStatuesAreDoneFading, gStoryScreenWasDismissed, gStretchToFit;
extern Boolean gSuspendedGameExists;
extern long gTimeSpentInMansion;
extern SpritePtr gTitleScreenSpriteArray[14];
extern Boolean gTriggerPresentInRoom;
extern Boolean gUseCustomCursor, gUseGamePad, gUseHaloSprite, gUseOutputFile, gUsingEditor;
extern Boolean gWasFullscreenWhenMinimized, gWereAnyTilesReplaced, gWindowIsMinimized;
extern short gWindowedResolutionX, gWindowedResolutionY;
extern SWPoint oldGlobalMouseLoc, oldMouseLoc;

extern SpriteLayerPtr gBackgroundScreenSpriteLayerP, gRearBackgroundPictureSpriteLayerP,
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
    gExtrasOverlaySpriteLayerP,     /* added: Pitch Black darkness */
    gTopSpriteLayerP, gSignContentsSpriteLayerP, gTipContentsSpriteLayerP, gFPSDisplaySpriteLayerP,
    gPausedSpriteLayerP, gCustomDialogSpriteLayerP, gOptionsDialogPane1LayerP,
    gOptionsDialogPane2LayerP, gOptionsDialogPane3LayerP, gOptionsDialogPane4LayerP, gTextSpriteLayerP,
    gCustomCursorSpriteLayerP, gHaloSpriteLayerP,
    gHiddenTriggerItemsSpriteLayerP, gFadeToBlackSpriteLayerP,
    gDigitsStatsSpriteLayerP, gStatsBackgroundSpriteLayerP, gStatsFadeSpriteLayerP,
    gStatsMapTextSpriteLayerP, gStatsSpriteLayerP;

int     main(int argc, char *argv[]);
void    CheckForLicenseAndDataFiles(void);
void    CreateAppSupportDirectories(void);
void    InitVariables(void);
void    CreateAndAddSpriteLayers(void);
void    CreateSpriteWorld(void);
void    ChangeWorldRectForGame(SpriteWorldPtr spriteWorldP, short statsHeight);
Boolean IsStatsAreaVisible(void);
void    MyRestoreWorldRect(SpriteWorldPtr spriteWorldP);
void    SetUpTiling(void);
void    SetUpTileColorVariations(void);
void    LoadTiles(void);
SWError MyLoadTilesFromPict(SpriteWorldPtr spriteWorldP, short startTileID, short endTileID,
                            short firstResID, short lastResID, short unused1, short borderWidth,
                            short borderHeight, short unused2);
void    StartUpGameCarbon(void);
Boolean IsGamepadInitialized(void);
void    InitGamePad(void);
void    RunMainLoop(void);
void    InitializePreferences(void);
void    LoadPreferences(void);
int     WritePreferencesFile(void);
int     ReadPreferencesFile(void);
int     MyOpenStandardMansion(ConstStringPtr mansionName, short mansionNumber);
int     OpenMansionUsingFilePath(const char *filePath, const char *mansionName);
int     MMOpenCustomMansionUsingPathAndName(const char *filePath, const char *mansionName);
void    SetCurMansionAsCustomMansion(Boolean isCustom);
void    StartTutorialMansion(void);
void    StartGameNoEditor(void);
void    StartGame(short levelResID, short levelNumber, Boolean unused);
void    SetUpNewGame(void);
void    RunAnimation(void);
void    RunAnimationCore(void);
void    MyPostEraseCallback(SpriteWorldPtr spriteWorldP);
void    MyTileLayer3Callback(SpriteWorldPtr spriteWorldP, short tileLayer, SWBoolean layerWasDrawn);
void    SetMovingPlatformOverlapBottomInset(SpriteLayerPtr layerP, short inset);
void    CleanUpAfterGame(void);
void    CloseMansionFile(void);
void    ShutDown(void);
void    MyCleanUpFunction(void);
Boolean IsPlayingGame(void);
void    SetStartingNumLives(void);
short   GetStartingNumLives(char difficulty);
void    RestoreJacksLives(void);
void    MMStartGameTimer(void);
void    MMUpdateTimeSpentInMansion(void);
void    MMPauseGameTimer(void);
void    MMResumeGameTimer(void);
void    MMDisposeAllSpritesInSpriteWorld(SpriteWorldPtr spriteWorldP);
void    TileChangeProc(SpriteWorldPtr spriteWorldP);
void    MyDrawTile(SpriteWorldPtr spriteWorldP, short tileLayer, short row, short col, short tileID);
void    ResetFloorTileAnimations(Boolean hideLightSaberFloors, Boolean resetLightSaberFloors);
void    HideLightSaberFloorsIfOff(SpriteWorldPtr spriteWorldP, Boolean unused);
short   MyGetTile(SpriteWorldPtr spriteWorldP, short tileLayer, short row, short col);
void    DrawNothingDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc);
void    DoorSpriteDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc);
int     BKBlitCustomDoorSurface(BKGLSurface *glsurface, short stretchDown, short shrinkUp);
void    ClipMainSpriteDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc);
void    ClipSpriteWithLavaDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc);
void    AddShadowDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc);
void    EnemyShadowDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc);
void    TogglePause(void);
void    PauseGame(void);
void    ResumeGame(void);
void    PauseSpriteLayersForPause(void);
void    HandlePausedScreenCallback(void);
void    ExitPausedMode(void);
void    DoEndGame(Boolean askFirst);
void    DoQuitCommand(Boolean askFirst);
void    DoGameFadeToBlack(short numFrames, Boolean fadeMusic);
void    DoGameFadeToBlackFullAnim(short numFrames, Boolean fadeMusic);
void    DoGameFadeToNormal(short numFrames);
void    DoGameFadeToNormalFullAnim(short numFrames);
void    DoFadeToBlack(short numFrames, Boolean animateEverything);
void    DoFadeToNormal(short numFrames, Boolean animateEverything);
void    FadeToBlackMoveProc(SpritePtr srcSpriteP);
void    FadeToNormalMoveProc(SpritePtr srcSpriteP);
Boolean IsCurrentlyFadingToBlack(void);
SpritePtr NewFadeSprite(SpriteWorldPtr spriteWorldP, short numFrames);
void    MMSetCursorForGame(void);
void    MMSetCursorForSystem(void);
void    MMRemoveGameCursor(void);
void    MMSaveCursorState(void);
void    MMRestoreCursorState(void);
void    MMHideAllCursors(void);
void    MMInitCursor(void);
void    MMHideCursor(void);
void    MMShieldCursor(void);
Boolean IsCursorShielded(void);
void    MMHandleCursorMovement(void);
Boolean CurrentScreenHasCustomCursor(void);
void    MyGameBeep(void);
void    MMAssertFailCallback(const char *file, const char *function, int line);
void    MMFatalErrorCallback(long errNum, const char *file, const char *function, int line);
void    ErrorMessage(const char *format, long value);

#include "MMProtos.h"
#include "MMExtras.h"

#endif /* MMGAME_H */
