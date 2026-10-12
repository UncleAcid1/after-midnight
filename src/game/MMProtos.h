/*
 * Midnight Mansion HD — declarations of the game's support files.
 * Included at the end of MMGame.h. One section per original source file.
 */
#ifndef MMPROTOS_H
#define MMPROTOS_H

#include "MMFont.h"

// ---------------------------------------------------------------------------
// MyUtils.c

extern Boolean gCursorIsVisible;

void    MyHideCursor(void);
void    MyInitCursor(void);
Boolean IsCursorVisible(void);
void    SetPascalString(StringPtr dst, ConstStringPtr src);
void    GetPascalStringFromCString(StringPtr dst, const char *src);
void    GetHFSUniFromCString(unsigned char *dst, const char *src);
void    GetCStringFromHFSUniStr(char *dst, const unsigned char *src);
void    GetPascalStringFromHFSUniStr(StringPtr dst, const unsigned char *src);
Boolean IsCharWhiteSpace(char c);
void    RemoveTrailingSpaces(StringPtr s);
void    RemoveLeadingSpaces(StringPtr s);
void    RemoveLeadingInvisibleString(StringPtr s);
void    StripPrefixFromString(StringPtr s, short prefixLen);
void    StripPrefixFromCString(char *s, short prefixLen);
void    StripStringFromStartOfString(ConstStringPtr prefix, ConstStringPtr src, StringPtr dst);
void    StripStringFromEndOfCString(const char *suffix, char *src, char *dst);
void    StripStringFromEndOfString(ConstStringPtr suffix, ConstStringPtr src, StringPtr dst);
void    RemoveNumberFromEndOfString(StringPtr s);
Boolean CompareCStrings(const char *a, const char *b);
Boolean CompareCStringsForLength(const char *a, const char *b, short len);
char   *strstrIgnoreCase(const char *haystack, const char *needle);
void    StripNDirectoriesFromEndOfPath(char *path, short n);
void    StripEndingFromPath(char *path);
char   *GetFilenameFromPath(char *path);
void    MyWriteDataToFile(SDL_RWops *file, long size, void *data);
int     MyReadDataFromFile(SDL_RWops *file, long size, void *data);
void    ClearFileStickyError(void);
int     GetFileStickyError(void);
void    SetFileStickyIfError(int err);
Boolean WasASpecificKeyPressed(void);
Boolean IsRectInRect(const SWRect *a, const SWRect *b);
Boolean IsRectFullyInRect(const SWRect *inner, const SWRect *outer);
void    MoveRect(SWRect *r, short h, short v);
void    OffsetRectForFrame(SWRect *r, FramePtr frameP);
void    SwapVariables(short *a, short *b);
int     sign(short n);
int     mAbs(int n);
Boolean IsNumberEven(long n);
Boolean IsNumberOdd(long n);
double  GetAngleFromXandY(double x, double y);
double  GetXFromAngleAndVelocity(double angle, double velocity);
double  GetYFromAngleAndVelocity(double angle, double velocity);
double  MyCos(double angle);
double  MySin(double angle);
double  MyATan(double n);
void    AddBooleanToByte(Boolean value, unsigned char *byte, unsigned char bit);
Boolean ReadBooleanFromByte(unsigned char byte, unsigned char bit);
int     GetCustomRandom(int min, int max);
float   GetCustomRandomFloat(float min, float max);
void    StartTicksTimer(void);
void    WaitTicks(void);
void    WaitTicksOrMouseClick(void);
void    MyDelay(void);
void    MyWaitForKeyDelay(void);
void    StartMicrosecTimer(void);
void    WaitMicroseconds(Uint32 ms);
Uint32  GetElapsedSeconds(void);
Uint32  GetElapsedMillisecs(void);
void    WaitMicrosecondsOrKeyPress(Uint32 ms);
void    WaitMicrosecondsOrMouseClick(Uint32 ms);

// ---------------------------------------------------------------------------
// MySWUtils.c

typedef SWBoolean (*SpriteTestProcPtr)(SpritePtr srcSpriteP, SpritePtr dstSpriteP);

SWBoolean SWBestMaskCollision(SpritePtr srcSpriteP, SpritePtr dstSpriteP);
void    ProcessOneFrame(SpriteWorldPtr spriteWorldP);
void    AnimateOneFrame(SpriteWorldPtr spriteWorldP);
void    SetSpriteWorldForCapturedDisplay(void);
void    SetSpriteWorldForUncapturedDisplay(void);
void    SWChangeTileImageFast(SpriteWorldPtr spriteWorldP, short tileID, short imageID);
Boolean DoesLayerHaveTiles(SpriteWorldPtr spriteWorldP, short tileLayer, short firstTileID, short lastTileID);
void    MyUpdateTilesOnScreenFaster(SpriteWorldPtr spriteWorldP, short startLayer, short endLayer,
                                    short firstTileID, short lastTileID);
SWError SaveAllSpritesToTempLayers(SpriteWorldPtr spriteWorldP);
void    RestoreSpritesFromTempLayers(SpriteWorldPtr spriteWorldP);
void    SWSaveTileMaps(SpriteWorldPtr spriteWorldP);
void    SWRestoreTileMaps(SpriteWorldPtr spriteWorldP);
void    InvalidateSpriteLayer(SpriteLayerPtr spriteLayerP);
void    OffsetSpritesInLayer(SpriteLayerPtr spriteLayerP, SWMovementType h, SWMovementType v);
void    SortSpriteLayerBottomToTop(SpriteLayerPtr spriteLayerP);
void    SortSpriteLayerRightToLeft(SpriteLayerPtr spriteLayerP);
SWBoolean SWCheckSpriteWithSprites(SpritePtr srcSpriteP, SpritePtr *hitSpritePP, SWTileSearchType searchType,
                                   SWRect *insetRectP, SpriteLayerPtr startLayerP, SpriteLayerPtr endLayerP,
                                   SWBoolean useOldRect, SWBoolean includeInvisible, SWBoolean mustBeBelow,
                                   SWBoolean fixPosition);
void    RoundOffSpriteVertLoc(SpritePtr spriteP);
void    SWCollideSpriteWithLayer(SpritePtr srcSpriteP, SpriteLayerPtr spriteLayerP, CollideProcPtr collideProc);
SpritePtr GetSpriteCollidingWithSprite(SpritePtr srcSpriteP, SpriteLayerPtr spriteLayerP, SpriteTestProcPtr testProc);
Boolean IsSpriteCollidingWithLayer(SpritePtr srcSpriteP, SpriteLayerPtr spriteLayerP,
                                   Boolean usePixelCollision, Boolean visibleOnly);
void    MoveSpritesFromLayerToLayer(SpriteLayerPtr srcLayerP, SpriteLayerPtr dstLayerP);
SpritePtr FindSpriteByRectInMultipleLayers(SpriteLayerPtr startLayerP, SpriteLayerPtr endLayerP,
                                           SpritePtr srcSpriteP, SWRect *testRect, SWBoolean useOldRect,
                                           SWBoolean includeInvisible, SWBoolean mustBeBelow);
void    SetAllSpriteLayersVisible(SpriteWorldPtr spriteWorldP, SWBoolean visible);
void    PauseAllSpriteLayers(SpriteWorldPtr spriteWorldP);
void    UnpauseAllSpriteLayers(SpriteWorldPtr spriteWorldP);
void    RemoveAllSpritesInLayer(SpriteLayerPtr spriteLayerP);
void    SWSetSpritesInLayerVisible(SpriteLayerPtr spriteLayerP, SWBoolean visible);
SWError SWCreateSolidColorSprite(SpritePtr *newSpritePP);
SpritePtr SWTopVisibleSpriteFromPoint(SpriteLayerPtr spriteLayerP, SpritePtr startSpriteP, SWPoint testPoint,
                                      SpriteInMaskProcPtr testProc);
void    SWConvertCoordinates(SpriteWorldPtr spriteWorldP, SWPoint *pointP);
void    MySetSpriteFrameTime(SpritePtr spriteP, long frameTime);
void    MyAdvanceSpriteFrame(SpritePtr spriteP, short *advance);
void    SWResetSpriteMovementTimer(SpriteWorldPtr spriteWorldP, SpritePtr spriteP);
void    SWSetSpriteHotSpot(SpritePtr spriteP, short hotSpotH, short hotSpotV);
SpriteWorldPtr GetSpriteWorldFromSprite(SpritePtr spriteP);
MoveProcPtr SWGetSpriteMoveProc(SpritePtr spriteP);
short   GetDistBetweenSprites(SpritePtr a, SpritePtr b);
short   GetDistBetweenSpritesHorizMid(SpritePtr a, SpritePtr b);
short   GetDistBetweenSpritesVertMid(SpritePtr a, SpritePtr b);
void    DelayMoveProcByValue(SpritePtr spriteP, long delay);
SpritePtr FindSpriteByPixel(SpriteLayerPtr spriteLayerP, SpritePtr startSpriteP, short h, short v);
SpritePtr FindSpriteByRect(SpriteLayerPtr spriteLayerP, SpritePtr startSpriteP, SWRect *testRect);
Boolean IsSpriteOldRectInRect(SpritePtr spriteP, SWRect *testRect);
void    SWCenterSpriteOverSprite(SpritePtr spriteP, SpritePtr targetP);
void    SWCenterSpriteInRect(SpritePtr spriteP, SWRect *rectP);

// ---------------------------------------------------------------------------
// TextSprite.c

#define kTextSpriteSignature    0x8dac7c
#define kTextSpriteMaxChars     500

/// A sprite that draws one line of TrueType text (userData == kTextSpriteSignature).
typedef struct TextSpriteRec {
    SpriteRec       sprite;
    char            text[kTextSpriteMaxChars];
    MMFont         *font;
    short           fontSize;
    short           style;          ///< 0 plain, 1 bold, 2 heavy (drawn 2 or 3 times, offset)
    Boolean         useRect;        ///< draw inside rect instead of at the sprite's location
    SWRect          rect;
    short           justification;  ///< 0 left, 1 right, 2 center
    short           rectWidth;
    Boolean         alignTop;       ///< baseline at rect.top (else rect.bottom)
    unsigned char   color[3];
    Boolean         dropShadow;
} TextSpriteRec, *TextSpritePtr;

MMFont *LoadFontUsingPath(const char *path);
MMFont *LoadFontSafe(const char *fontName);
Boolean FontExists(const char *fontName);
SWError SWCreateTextSprite(SpritePtr *newSpritePP, const char *text, const char *fontName, short fontSize,
                           short style, const unsigned char rgb[3], Boolean dropShadow);
void    SetTextSpriteText(SpritePtr spriteP, const char *text);
void    SetTextSpriteRect(SpritePtr spriteP, const SWRect *rectP, short justification, Boolean alignTop);
void    SWTextSpriteDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc);
int     GetTextSpriteWidth(SpritePtr spriteP);
int     GetTextSpriteRenderWidth(const char *fontName, short fontSize, const char *text);

// ---------------------------------------------------------------------------
// CompatibilityLayer.c

/// Text style used by MyDraw...Text (laid out like the original's 264-byte record).
typedef struct MMTextStyleRec {
    Str255  fontName;       ///< Pascal string, e.g. "\pAction Man"
    short   style;          ///< see TextSpriteRec.style
    short   size;
    Boolean unused;
    Boolean dropShadow;
    short   justification;  ///< 0 left, 1 right, 2 center
} MMTextStyleRec;

extern SpritePtr gCurBackgroundSprite;

void      MyDrawText(const char *text, MMTextStyleRec *style, short h, short v, SWRGBColor *color);
SpritePtr MyDrawPascalTextJustified(ConstStringPtr text, MMTextStyleRec *style, SWRect *rectP, SWRGBColor *color);
SpritePtr MyDrawCTextJustified(const char *text, MMTextStyleRec *style, SWRect *rectP, SWRGBColor *color);
SpritePtr MyDrawTextBox(SpriteWorldPtr spriteWorldP, const char *text, MMTextStyleRec *style, SWRect *rectP,
                        SWRGBColor *color, SpriteLayerPtr layerP, Boolean renderToSprite);
void      MMRenderSceneForWorkAreaSprite(SpriteWorldPtr spriteWorldP);
SWError   LoadAndInstallBackgroundSprite(SpritePtr *spritePP, const char *folderPath, const char *fileName,
                                         short cacheIndex, Boolean hasAlpha);
SWError   LoadSpriteFromScreensFolder(SpritePtr *spritePP, const char *fileName);
SWError   CreateSpriteFromSinglePict(SpriteWorldPtr spriteWorldP, SpritePtr *spritePP, void *storageP,
                                     short pictResID, short maskResID, short frameWidth, short borderWidth,
                                     MaskType maskType);
SWError   CreateSpriteFromPictResource(SpriteWorldPtr spriteWorldP, SpritePtr *spritePP, void *storageP,
                                       short pictResID, short maskResID, short maxFrames, MaskType maskType);
SWError   CreateSpriteFromSinglePictXY(SpriteWorldPtr spriteWorldP, SpritePtr *spritePP, void *storageP,
                                       short pictResID, short maskResID, short frameWidth, short frameHeight,
                                       short borderWidth, short borderHeight, Boolean hasOutsideBorder,
                                       short maxFrames, MaskType maskType);
SWError   MMCreateSpriteFromSingleFileXY(SpriteWorldPtr spriteWorldP, SpritePtr *spritePP, void *storageP,
                                         const char *fileName, short frameWidth, short frameHeight,
                                         short maxFrames, MaskType maskType);
SWError   CreateSpriteFromVerticalPict(SpriteWorldPtr spriteWorldP, SpritePtr *spritePP, void *storageP,
                                       short pictResID, short maskResID, short maxFrames, MaskType maskType);
SWError   MMCreateSpriteFromVerticalFile(SpriteWorldPtr spriteWorldP, SpritePtr *spritePP, void *storageP,
                                         const char *fileName, short maxFrames, MaskType maskType);
const char *GetFileNameForResource(const char *folderPath, int resID);
const char *AddFilenameToPath(const char *folderPath, const char *fileName);
const char *GetSpriteImagePath(const char *fileName);
const char *GetScreensFolderPath(void);
const char *GetTitleScreenFolderPath(const char *fileName);
const char *GetMapScreenFolderPath(const char *fileName);
const char *GetOrganScreenFolderPath(const char *fileName);
const char *GetSoundsFolderPath(void);
const char *GetMusicFolderPath(void);
const char *GetFontsFolderPath(const char *fileName);
const char *GetMansionFolderPath(const char *mansionName);
const char *GetMansionDataFolderPath(const char *fileName);
const char *GetCustomMansionsFolderPath(const char *mansionName);
const char *GetPrefsFilePath(const char *fileName);
const char *GetRealPrefsFilePath(const char *fileName);
const char *GetAppSupportPath(const char *fileName);
const char *GetPlainAppSupportPath(void);
const char *GetSavedGamesFilePath(const char *fileName, Boolean addMansionName);
const char *GetMoviesFilePath(const char *fileName);
const char *GetPlainSavedGamesFilePath(const char *fileName);
const char *GetPlayerProfilePath(const char *fileName);
const char *GetUserMoviesPath(const char *fileName);
void        GetSuspendedGameFilePath(char *outPath, const char *fileName);
void        GetPlainSuspendedGameFilePath(char *outPath, const char *fileName);
const char *GetHomeDirectory(void);
const char *GetResourcesDirectory(void);
const char *GetGameDataPath(void);
const char *GetGameDirectory(void);
const char *GetCurrentDirectory(void);
SDL_RWops  *MMOpenResourceFile(const char *path, const char *mode);
int         ScanDirectoryForResIDs(const char *mansionPath, const char *resType);
Boolean     ResourceExists(const char *resType, short resID);
Boolean     Resource1Exists(const char *resType, short resID);
Boolean     DoesFileExist(const char *path);
short       CountNumSuccessiveFiles(const char *folderPath, const char *baseName, const char *extension);
short       MMCountResources(const char *resType);
int         PrintFilesInFolder(const char *folderPath);
short       CountNumFilesInFolder(const char *folderPath);
void       *MMGetResource(const char *resType, short resID, size_t size);
void       *MMGetResourceUsingPath(const char *resType, short resID, size_t size, const char *mansionPath);
long        MMGetMansionVersion(const char *resType, short resID, size_t size, const char *mansionPath);
char       *MMGetCStringFromTEXTResource(short resID);
long        GetSPRTResourceSize(short resID);
void        EnableRecordMovieMenuItem(void);
void        SetUpMenuBar(void);
void        OpenGameSupportFiles(void);
void        StartUpEditor(void);
void        ExitToEditor(void);

// ---------------------------------------------------------------------------
// rngs.c

#include "rngs.h"

// ---------------------------------------------------------------------------
// MMEvents.c

typedef Boolean (*MMEventCallbackPtr)(SDL_Event *event);

extern int     gMouseButton;
extern Uint32  gTimeOfLastWindowResize;
extern int     gTestSoundIndex;
extern Boolean gMusicWasPlaying;
extern Boolean gResetMusic;
extern int     gMouseLocX, gMouseLocY;

void    ProcessSDLEvents(void);
int     ConvertKeyScanCode(int scancode);
void    MyHandleSDLEvent(SDL_Event *event);
void    printTileMap(short tileLayer);
void    LoadRoomToSideOfCurRoom(short colOffset, short rowOffset);
void    SetStretchToFit(SWBoolean stretchToFit);
void    SetMMEventCallback(MMEventCallbackPtr callback);
void    HandleWindowResizeEvent(SDL_Event *event, int width, int height);
void    DoWindowResize(int width, int height, Boolean fade);
Boolean HandleKeyUpDownEvent(int scancode, SDL_Keycode sym, Boolean isDown, Boolean isRepeat);
void    UpdateMovementKeys(int scancode, SDL_Keycode sym, Boolean isDown, Boolean isRepeat);
void    HandleActivateEvent(void);
void    HandleDeactivateEvent(void);
Boolean WasMouseClicked(void);
Boolean WasAKeyPressed(void);
Boolean IsKeyModifier(int scancode);
Boolean WasGameQuit(void);
void    MMFlushEvents(void);
void    UpdateMyMouseLoc(short x, short y);
void    ResetMyMouseLocAndButton(void);
Boolean GetMyMouse(SWPoint *pointP);
Boolean MMGetMouse(SWPoint *pointP);
Boolean IsMouseDown(void);
void    ResetKeys(void);
void    ToggleFullscreen(void);
void    ToggleMusic(void);
void    ToggleStats(void);
void    GiveOSTime(void);
void    KeepMusicPlaying(void);

// ---------------------------------------------------------------------------
// MyFileUtils.c (was MyFileUtilsMac.c)

int     MMCreateDirectory(const char *parentPath, const char *folderName);
Boolean DoesFileOrFolderExistInPath(const char *parentPath, const char *name);
Boolean DoesFileOrFolderExist(const char *path);
void    SetFileCreatorAndTypeForPath(const char *path, Uint32 creator, Uint32 fileType);
int     getProcessorSpeed(void);

// ---------------------------------------------------------------------------
// MyMusicUtils.c

int     InitMusic(void);
void    ExitMusic(void);
int     LoadMusic(const char *path);
void    SetMusicLooping(Boolean looping);
void    StartMusic(short volume);
void    SetMusicVolume(short volume);
short   GetMusicVolume(void);
void   *GetCurrentMusicReference(void);
Boolean IsMusicDone(void);
void    StopAndDisposeMusic(void);
void    PauseMusic(void);
void    ResumeMusic(Boolean rewind);
Boolean IsMusicPlaying(void);
void    FadeMusicNow(short steps);
void    FadeMusicNowNoAdjust(short ms);
void    FadeMusicIn(short steps);
void    StartMusicByFadingIn(const char *name, int fadeMs, short volume);
void    StartOpeningMusic(Boolean forceMusic);
void    PlayBackgroundMusic(short index);
void    MyPlayMusic(const char *name, short volume, short index, Boolean looping);
void    MyFadeMusicNow(void);

// ---------------------------------------------------------------------------
// MMSounds.c (and "MMSounds SDL_Mixer.c")

void    MMSetTempStereoFadeDist(short distance);
void    MMPlaySound(short soundNum, short channel, short mode);
void    MMPlayStereoSound(SpritePtr spriteP, short soundNum, short channel, short mode);
int     MMPlayStereoSound2(SpritePtr spriteP, short soundNum, short channel, short mode, short volume,
                           double rate, int loops);
int     MMPlaySoundDetailed(short soundNum, short channel, short mode, short volume, short stereoPosition,
                            double rate, int loops);
short   GetStereoPositionOfSprite(SpritePtr spriteP, SWRect *areaP);
short   GetStereoPositionOfColumn(int column, SWRect *areaP);
int     MMPlayStereoSoundAdjustedVol(SpritePtr spriteP, short soundNum, short channel, short mode,
                                     short maxDistance, double rate);
void    MMUpdateChannelInfoForSpriteLayer(SpriteLayerPtr layerP, short soundNum, short channel, short maxVolume,
                                          short stereoWidth, short maxDistance);
int     LoadAndSetupSounds(short numChannels, short numReservedChannels, short numSounds, const char *folderPath);
int     PlaySoundDetailed(short soundNum, short channel, short mode, short volume, short stereoPosition,
                          double rate, int loops);
short   CountSounds(short soundNum);
short   FindSound(short soundNum);
void    StopMySound(short soundNum);
short   GetChannelVolume(short channel);
void    StopChannel(short channel);
void    StopAllChannels(void);
Boolean IsChannelAvailable(short channel);
short   FindEmptyChannel(void);
void    SetVolumeAndStereoPosition(short channel, short volume, short stereoPosition);

// ---------------------------------------------------------------------------
// Stats.c

void    CreateStatsArea(void);
void    ResetStatsArea(void);
void    RestoreStatsFromObjectsCarried(void);
void    LoadStatsBackFrame(short pictID);
SWError LoadAndInstallStatsBackgroundSprite(SpritePtr *spritePP, const char *folderPath, short pictID);
void    AddBackpackToStatsArea(void);
void    LoadAppropriateStatsBackground(void);
void    AddLanternToStatsArea(void);
void    UpdateStatsItems(void);
void    DrawStatsArea(void);
void    StatsDigitsMoveProc(SpritePtr spriteP);
Boolean AddItemToStats(short item);
void    RemoveItemFromStats(short item);
void    RemoveAllStatsItems(void);
void    HideStatsSprites(void);
void    MakeStatsSpritesVisible(void);
void    CreateFPSAndLevelResIDSprites(void);

// ---------------------------------------------------------------------------
// HighScores.c

#define kNumHighScoreMansions   11

/// One high score entry; the layout is that of the high scores file.
typedef struct HighScoreRec {
    unsigned char name[40];     ///< Pascal string
    Sint32  score;
    Sint32  time;               ///< seconds spent in the mansion
    Sint32  percent;            ///< of the mansion explored
    Boolean wonMansion;
    Boolean unused35;
    Sint16  unused36;
    Sint32  unused38;
    Sint32  unused3c;
} HighScoreRec;

typedef struct HighScoresFileRec {
    Sint16       version;
    Sint16       unused;
    HighScoreRec scores[kNumHighScoreMansions][3][10];     ///< [mansion][difficulty][rank]
} HighScoresFileRec;

extern HighScoresFileRec gHighScores;
extern short gCurrentHighScoresScreenIndex;

void    InitializeHighScores(void);
void    LoadHighScores(void);
int     WriteHighScoresToFile(void);
int     LoadHighScoresFromFile(void);
void    SwapEndiansForHighScoreData(void);
void    SetUpHighScoresScreen(short screenIndex, Boolean showIndex, Boolean returnToMapScreen);
void    HandleHighScoresScreenCallback(void);
void    DrawContentsForHighScoresScreen(short mansionIndex);
void    ExitHighScoresScreen(void);
Boolean CheckForHighScore(short mansionIndex, short difficulty, long score, short percent, Boolean wonMansion);
void    ShiftScoresLowerInList(short mansionIndex, short difficulty, short rank);

// ---------------------------------------------------------------------------
// MyButtonSprite.c

#include "SWTextSprite.h"

#define kButtonSpriteSignature  0x149f

enum { kButtonControl = 0, kCheckboxControl = 1, kKeyInputBoxControl = 2 };
enum { kButtonNormal = 0, kButtonHighlighted = 1, kButtonPushed = 2 };

typedef void (*ButtonProcPtr)(SpritePtr buttonP, short buttonIndex, short newState, short oldState);

/// A button, checkbox or key input box (userData == kButtonSpriteSignature).
typedef struct ButtonSpriteRec {
    SpriteRec   sprite;
    short       controlType;                ///< 0xf8  kButtonControl, ...
    short       state;                      ///< 0xfa  kButtonNormal, ...
    short       buttonIndex;                ///< 0xfc  returned by ProcessButtonSprites
    short       normalFrame;                ///< 0xfe
    short       highlightedFrame;           ///< 0x100
    short       pushedFrame;                ///< 0x102 (key input box: selected)
    short       dimmedFrame;                ///< 0x104
    short       clickRadius;                ///< 0x106 -1: whole sprite
    short       uncheckedFrame;             ///< 0x108
    short       uncheckedHighlightedFrame;  ///< 0x10a
    short       checkedFrame;               ///< 0x10c
    short       checkedHighlightedFrame;    ///< 0x10e
    Boolean     isChecked;                  ///< 0x110
    Boolean     wasChecked;                 ///< 0x111
    SpritePtr   textSpriteP;                ///< 0x114 label (TrueType or bitmap font)
    Boolean     hasColor;                   ///< 0x118
    SWRGBColor  color;
    Boolean     hasHighlightedColor;        ///< 0x120
    SWRGBColor  highlightedColor;
    Boolean     hasPushedColor;             ///< 0x128
    SWRGBColor  pushedColor;
    Boolean     keyBoxIsSelected;           ///< 0x130 waiting for a key
    long        keyScancode;                ///< 0x134
    long        keyKeycode;                 ///< 0x138
    char        keyName[10];                ///< 0x13c
    Boolean     ignoreCommandKey;           ///< 0x146
    Boolean     ignoreEscapeKey;            ///< 0x147
    Boolean     isDimmed;                   ///< 0x148
    short       pushedOffsetH;              ///< 0x14a
    short       pushedOffsetV;              ///< 0x14c
    float       bounceAngleH;               ///< 0x150 degrees, MMBounceButtonMoveProc
    float       bounceAngleV;               ///< 0x154
    float       bounceStepH;                ///< 0x158
    float       bounceStepV;                ///< 0x15c
    float       bounceAmplitude;            ///< 0x160
    float       bounceBaseScale;            ///< 0x164 -1 until first bounce
    float       bounceExtraScale;           ///< 0x168
    Boolean     convertScancodes;           ///< 0x16c right modifiers count as left ones
    Boolean    *linkedCheckboxP;            ///< 0x170
    Boolean     savedCheckboxValue;         ///< 0x174
    long       *linkedScancodeP;            ///< 0x178
    long        savedScancode;              ///< 0x17c
    long       *linkedKeycodeP;             ///< 0x180
    long        savedKeycode;               ///< 0x184
    char       *linkedKeyNameP;             ///< 0x188
    ButtonProcPtr buttonProc;               ///< 0x18c called on state changes
} ButtonSpriteRec, *ButtonSpritePtr;

SWError   LoadStandardButtonSprite(SpritePtr *spritePP, short pictID, MaskType maskType);
SWError   LoadStandardButtonSpriteFromFile(SpritePtr *spritePP, const char *path, Boolean selfMask);
SWError   LoadStandardButtonSpriteMask(SpritePtr *spritePP, short pictID, short maskID, MaskType maskType);
SWError   LoadStandardDimmableButtonSprite(SpritePtr *spritePP, short pictID, MaskType maskType);
SWError   LoadMiscButtonSprite(SpritePtr *spritePP, short numFrames, short pictID, short maskID, MaskType maskType);
SWError   LoadMiscButtonSpriteFromFile(SpritePtr *spritePP, short numFrames, const char *path, Boolean selfMask);
SpritePtr CloneButtonSprite(SpritePtr masterP, short buttonIndex);
void      SetButtonSpriteIndex(SpritePtr spriteP, short buttonIndex);
void      SetUpButtonSprite(SpritePtr spriteP, short normalFrame, short highlightedFrame, short pushedFrame,
                            short dimmedFrame);
void      SetUpCheckboxSprite(SpritePtr spriteP, short uncheckedFrame, short uncheckedHighlightedFrame,
                              short checkedFrame, short checkedHighlightedFrame, Boolean checked);
void      SetUpKeyInputBox(SpritePtr spriteP, short normalFrame, short selectedFrame);
void      SetKeyInputBoxKeysToIgnore(SpritePtr spriteP, Boolean ignoreCommandKey, Boolean ignoreEscapeKey);
void      SetCheckboxChecked(SpritePtr spriteP, Boolean checked);
void      SetButtonSpriteText(SpritePtr spriteP, SpritePtr textSpriteP);
void      LinkCheckboxWithData(SpritePtr spriteP, Boolean *dataP);
void      LinkKeyInputBoxWithData(SpritePtr spriteP, long *scancodeP, long *keycodeP, char *nameP);
void      UpdateDataFromControlsInLayers(SpriteLayerPtr firstLayerP, SpriteLayerPtr lastLayerP);
Boolean   HasControlChangedValues(SpritePtr spriteP);
void      SetButtonColor(SpritePtr spriteP, unsigned short red, unsigned short green, unsigned short blue);
void      SetButtonHighlightedColor(SpritePtr spriteP, unsigned short red, unsigned short green, unsigned short blue);
void      SetButtonPushedColor(SpritePtr spriteP, unsigned short red, unsigned short green, unsigned short blue);
void      SetButtonDimmedMode(SpritePtr spriteP, Boolean dimmed);
void      SetButtonClickRadius(SpritePtr spriteP, short radius);
short     GetButtonIndex(SpritePtr spriteP);
void      InstallButtonProc(SpritePtr spriteP, ButtonProcPtr buttonProc);
void      SetButtonPushedOffset(SpritePtr spriteP, short h, short v);
void      AdjustCollisionRectForOffset(SpritePtr spriteP);
short     ProcessButtonSprites(SpriteLayerPtr firstLayerP, SpriteLayerPtr lastLayerP, SWPoint point,
                               Boolean mouseDown, Boolean useMask, SpritePtr *clickedSpritePP);
void      ProcessNormalButtonSprite(SpriteLayerPtr layerP, SpritePtr spriteP, Boolean isInside, Boolean mouseDown,
                                    short *clickedIndexP, SpritePtr *clickedSpritePP);
void      ProcessCheckBoxSprite(SpriteLayerPtr layerP, SpritePtr spriteP, Boolean isInside, Boolean mouseDown,
                                short *clickedIndexP, SpritePtr *clickedSpritePP);
void      UnhighlightAllOtherButtonSprites(SpriteLayerPtr layerP, SpritePtr exceptSpriteP, short controlType);
void      DeselectButton(SpritePtr spriteP);
void      ChangeButtonColor(SpritePtr spriteP, short state);
void      SetCorrectCheckboxFrame(SpritePtr spriteP, Boolean highlighted);
void      DisposeButtonSpritesMatchingIndex(SpriteLayerPtr layerP, short buttonIndex);
void      ProcessKeyInputBoxSprite(SpriteLayerPtr layerP, SpritePtr spriteP, Boolean isInside, Boolean mouseDown,
                                   short *clickedIndexP, SpritePtr *clickedSpritePP);
Boolean   HandleEventInKeyInputBox(SDL_Event *event);
Boolean   DoesEventExist(Uint32 eventType);
SpritePtr GetCurrentlySelectedKeyInputBox(void);
const char *ConvertKeyName(const char *name);
void      SetKeyInputBoxSelected(SpritePtr spriteP, Boolean selected);
void      CustomButtonDrawProc(SpritePtr spriteP, FramePtr curFrameP, float horizLoc, float vertLoc);
const char *GetTextFromSDLKeyCode(SDL_Keycode keycode);

#include "MMSprites.h"
#include "MMGameProtos.h"

#endif
