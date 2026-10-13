// GameSupport.c — cheat codes, replay movies, the debug output log, suspended/saved games,
// player profiles and the in-game dialog boxes.
//
// Registration, receipt and anti-piracy code from the original is not included: the game always
// behaves as registered.

#include <time.h>
#include <dirent.h>
#include "MMGame.h"
#include "MMProtos.h"

#define kMovieFileVersion       0x6e
#define kMovieHeaderEndMarker   0x47db
#define kSuspendFileVersion     100
#define kProfileVersion         0x66

// Movie events: 0-7 keys, 0xb reset keys, 0xc/0xd easy jumping, 0xe+ cheat codes,
// 0x2d action sound, 0x2e sign dismissed, 0x30 end of movie.
enum {
    kMovieKeyLeft = 0, kMovieKeyRight = 1, kMovieKeyUp = 2, kMovieKeyDown = 3, kMovieKeyJump = 4,
    kMovieKeyAction = 5, kMovieKeyJumpLeft = 6, kMovieKeyJumpRight = 7,
    kMovieResetKeys = 0xb, kMovieEasyJumpingOn = 0xc, kMovieEasyJumpingOff = 0xd,
    kMovieFirstCheat = 0xe, kMovieLastCheat = 0x2c,
    kMovieActionSound = 0x2d, kMovieSignDismissed = 0x2e, kEndOfMovieEvent = 0x30
};

static short gNextFreeCheatCodeElement = 0;
static const char *gCheatCodeArray[] = {
    "gimmie red", "gimmie blue", "gimmie green", "gimmie purple", "gimmie yellow", "extra life",
    "i win", "reveal map", "gimmie money", "gimmie gold", "show secrets", "enter movie",
    "enable save", "gimmie lantern", "double fps", "no fps", "normal fps", "i lose", "suicide"
};
#define kNumCheatCodes ((short)SDL_arraysize(gCheatCodeArray))
#define kCheatCodeCacheSize 0x31
static char gCheatCodeCache[kCheatCodeCacheSize + 1];

static Boolean gMovieFileIsOpen = false;
static Boolean gProfileWasSaved = false;
static short   gCurOptionsDialogPane = 1;

static SpritePtr gControlsTabSpriteP, gOptionsTabSpriteP, gGamepadCheckboxSpriteP, gFunTabSpriteP, gExtrasTabSpriteP;
static Boolean gEscKeyIsDown, gReturnKeyIsDown;
static SDL_RWops *gMovieFileRef;
static Uint32  gNextFrame;
static Boolean gNextFrameIsInitialized;
static Str255  gSaveCurrentPlayerName;
static Boolean gSaveCurrentPlayerNameExists;
static Boolean wasFullscreen;
extern SpritePtr saveTileBackgroundP;  // GameMap.c

Boolean gFileIsOpen = false;
FILE   *gOutputFileP = NULL;
unsigned char gSuspendedGameFilenameHFSUni[512];

// ---------------------------------------------------------------------------
// Cheat codes (typed while playing)

void ClearCheatCodeCache(void)
{
    gNextFreeCheatCodeElement = 0;
    gCheatCodeCache[0] = 0;
}

void AddKeyToCheatCodeCache(char key)
{
    short i;

    gCheatCodeCache[gNextFreeCheatCodeElement++] = key;
    gCheatCodeCache[gNextFreeCheatCodeElement] = 0;
    if (gNextFreeCheatCodeElement == kCheatCodeCacheSize) {
        for (i = 0; i < kCheatCodeCacheSize; i++)
            gCheatCodeCache[i] = gCheatCodeCache[i + 1];
        gNextFreeCheatCodeElement--;
        gCheatCodeCache[gNextFreeCheatCodeElement] = 0;
    }
    CheckCheatCodes();
}

void CheckCheatCodes(void)
{
    short code, start;

    for (code = 0; code < kNumCheatCodes; code++) {
        for (start = 0; start < gNextFreeCheatCodeElement; start++) {
            if (CompareCStrings(gCheatCodeArray[code], gCheatCodeCache + start)) {
                DoCheatCode(code);
                WriteEventToMovieFile((char)(code + kMovieFirstCheat), true);
                ClearCheatCodeCache();
            }
        }
    }
}

void DoCheatCode(short code)
{
    static const char *keyNames[] = { "red", "blue", "green", "purple", "yellow" };
    char message[64];
    SpritePtr spriteP;

    switch (code) {
        case 0: case 1: case 2: case 3: case 4:
            if (AddItemToStats(code)) {
                gDidCheat = true;
                gObjects.counts[code]++;
                snprintf(message, sizeof(message), "CHEAT: Got %s key.\r", keyNames[code]);
                WriteCStringToFile(message);
                MMPlaySound(0, 8, 1);
            }
            break;
        case 5:
            gDidCheat = true;
            gNumLivesLeft++;
            WriteCStringToFile("CHEAT: Got extra life.\r");
            MMPlaySound(0x23, 9, 1);
            break;
        case 6:
            WriteCStringToFile("CHEAT: Testing Beat Mansion.\r");
            gBeatTheMansion = true;
            MMPlaySound(0x23, 9, 1);
            break;
        case 7:
            gDidCheat = true;
            WriteCStringToFile("CHEAT: Revealed in-game map, but not secrets.\r");
            MMPlayStereoSound2(NULL, 0x27, 8, 1, gSoundVolume, 2.0, 0);
            RevealGridInMap(0x80, 0x7fff);
            break;
        case 8:
        case 9:
            gDidCheat = true;
            WriteCStringToFile("CHEAT: Got 500 points.\r");
            gScore += 500;
            MMPlaySound(0x19, 8, 1);
            break;
        case 10:
            gDidCheat = true;
            WriteCStringToFile("CHEAT: Revealed secrets on map.\r");
            MMPlaySound(0x3e, 8, 1);
            RevealSecretRooms();
            gShowSecrets = true;
            break;
        case 11:
            if (IsMoviePlaying()) {
                gDidCheat = true;
                WriteCStringToFile("CHEAT: Entered movie!\r");
                CloseGameMovieFile();
                ResetKeys();
                MMPlaySound(0x48, 8, 1);
            }
            break;
        case 12:
            gDidCheat = true;
            WriteCStringToFile("CHEAT: Re-used a save pedestal!\r");
            for (spriteP = SWGetNextSprite(gSavePedestalSpriteLayerP, NULL); spriteP != NULL;
                 spriteP = SWGetNextSprite(gSavePedestalSpriteLayerP, spriteP)) {
                SWSetCurrentFrameIndex(spriteP, 0);
                RemoveSpriteFromObjectArray(spriteP);
                RemoveSpriteFromDoOnceArray(spriteP);
            }
            MMPlaySound(0x48, 8, 1);
            break;
        case 13:
            gDidCheat = true;
            WriteCStringToFile("CHEAT: Got lantern for entire map.\r");
            MMPlaySound(0x3e, 8, 1);
            GetLantern(NULL);
            break;
        case 14:
            SWSetSpriteWorldMaxFPS(gSpriteWorldP, 120);
            break;
        case 15:
            SWSetSpriteWorldMaxFPS(gSpriteWorldP, 0);
            break;
        case 16:
            SWSetSpriteWorldMaxFPS(gSpriteWorldP, 60);
            break;
        case 17:
            gGameOver = true;
            break;
        case 18:
            spriteP = GetHeroSprite();
            MMPlaySound(9, 10, 1);
            KillHero(spriteP);
            break;
        case 19:
            gArrowKeysAreEnabledForTesting = true;
            gDidCheat = true;
            break;
        case 20:
            MMPlayStereoSound2(NULL, 0x27, 8, 1, gSoundVolume, 2.0, 0);
            RevealEntireMap();
            gDidCheat = true;
            break;
        case 21:
            if (!gMansionIsCustomMansion && gCurrentMansionNumber != -2) {
                gDidCheat = false;
                ResumeGame();
                CheckForHighScore(gCurrentMansionNumber, gDifficultySetting, gScore, GetPercentSecrets(), true);
            }
            break;
    }
}

// ---------------------------------------------------------------------------
// Replay movies
//
// File: version, random seed, isNewGame, easyJumping, then either the start-of-game info or a
// full save-data block. The rest is a stream of (frame number, event byte) pairs; an event byte
// >= 0x7f means "key released" (event + 0x7f).

static void StartHeroInState(SpritePtr heroP, short state, short vineTileLayer)
{
    if (state == 1) {
        SWSetSpriteFrameRange(heroP, 0x12, 0x19);
        SWSetSpriteMoveProc(heroP, ClimbingSpriteMoveProc);
    }
    else if (state == 2) {
        GetOnVine(heroP);
        HERO(heroP)->vineTileLayer = vineTileLayer;
    }
    else if (state == 3) {
        GetOnPole(heroP, 0);
    }
}

int PlayMovieFile(const char *path)
{
    float heroHoriz = -1000.0f, heroVert = -1000.0f;
    float moveDeltaH = 0, moveDeltaV = 0;
    Sint32 version = 0, seed = 0, marker;
    long saveVersion;
    short vineTileLayer = 0, state = 0;
    Uint8 isNewGame = 0, easyJumping = 0;
    Boolean isSaveFile = false;
    char mansionName[255];
    const char *mansionPath;
    SpritePtr heroP;
    int err = 0;

    if (gMovieFileIsOpen)
        CloseGameMovieFile();

    gMovieFileRef = SDL_RWFromFile(path, "rb");
    if (gMovieFileRef == NULL) {
        MMDoErrorMessage("Sorry, but the movie file couldn't be opened.", 0);
        return -1;
    }

    gMovieFileIsOpen = true;
    SetUpNewGame();
    MMPauseGameTimer();
    MyReadDataFromFile(gMovieFileRef, 4, &version);
    MyReadDataFromFile(gMovieFileRef, 4, &seed);
    MyReadDataFromFile(gMovieFileRef, 1, &isNewGame);
    MyReadDataFromFile(gMovieFileRef, 1, &easyJumping);
    if (!isNewGame) {
        err = LoadSaveDataFromFile(gMovieFileRef, &heroHoriz, &heroVert, &isSaveFile, &saveVersion);
        if (err == 0) {
            MyReadDataFromFile(gMovieFileRef, 1, &gSilverFloorIsActive);
            MyReadDataFromFile(gMovieFileRef, 1, &gGoldFloorIsActive);
            MyReadDataFromFile(gMovieFileRef, 4, &moveDeltaH);
            MyReadDataFromFile(gMovieFileRef, 4, &moveDeltaV);
            MyReadDataFromFile(gMovieFileRef, 2, &vineTileLayer);
            MyReadDataFromFile(gMovieFileRef, 2, &state);
            MyReadDataFromFile(gMovieFileRef, 4, &marker);
            SDL_assert(marker == kMovieHeaderEndMarker);
        }
    }
    else {
        MyReadDataFromFile(gMovieFileRef, 255, gMansionFilename);
        gMansionFilename[254] = 0;
        MyReadDataFromFile(gMovieFileRef, 1, &gDifficultySetting);
        MyReadDataFromFile(gMovieFileRef, 2, &gCurrentMansionNumber);
        MyReadDataFromFile(gMovieFileRef, 4, &marker);
        SDL_assert(marker == kMovieHeaderEndMarker);
    }

    if (version < kMovieFileVersion) {
        MMDoCustomDialog(gSpriteWorldP,
            "This replay is in the old non-HD format. It can't be played by Midnight Mansion HD.", 3);
        err = -2;
    }
    if (err != 0)
        return err;

    SetCurMansionAsCustomMansion(gCurrentMansionNumber == -1);
    strcpy(mansionName, gMansionFilename);
    StripStringFromEndOfCString(".zip", mansionName, NULL);
    StripStringFromEndOfCString(".ZIP", mansionName, NULL);
    mansionPath = GetPathForAnyMansionFile(mansionName);
    strcpy(gCustomMansionFolderPath, mansionPath);
    err = OpenMansionUsingFilePath(mansionPath, mansionName);
    if (err != 0 && gMansionIsCustomMansion)
        err = OpenMansionUsingFilePath(GetMansionFolderPath(gMansionFilename), gMansionFilename);
    if (err != 0)
        return err;

    ExitTitleScreen();
    gNextFrame = 0;
    gNextFrameIsInitialized = false;
    gPlayingGameMovie = true;
    SetEasyJumpingMode(easyJumping);
    ResetKeys();

    if (!isNewGame) {
        SDL_assert(heroHoriz != -1000.0f);
        ChangeWorldRectForGame(gSpriteWorldP, gStatsHeight);
        SWSetPostEraseCallBack(gSpriteWorldP, MyPostEraseCallback);
        err = HandleRestoreStuff(heroHoriz, heroVert, isSaveFile, saveVersion);
        if (!isSaveFile) {
            heroP = GetHeroSprite();
            GetKeyEventsFromMovie();
            if (heroP != NULL) {
                SWSetSpriteMoveDelta(heroP, moveDeltaH, moveDeltaV);
                StartHeroInState(heroP, state, vineTileLayer);
                if (state == 1) {
                    SWSetSpriteFrameAdvanceMode(heroP, kNoLoopingMode);
                    SWSetSpriteFrameAdvance(heroP, 1);
                    SWSetSpriteFrameProc(heroP, NULL);
                    HERO(heroP)->hasClimbed = false;
                    HERO(heroP)->isLanding = false;
                }
                SaveStartingPosition(heroP);
                if (state == 1)
                    ClimbingSpriteMoveProc(heroP);
                else if (state == 2)
                    VineClimbingMoveProc(heroP);
                else if (state == 3)
                    PoleSlidingMoveProc(heroP);
            }
        }
        if (err != 0)
            MyRestoreWorldRect(gSpriteWorldP);
    }
    else {
        StartGameNoEditor();
    }

    printf("Random number seed loaded from movie file: %d \n", (int)seed);
    SelectStream(0);
    PutSeed(seed);
    return err;
}

/// Reads the next key event if it belongs to the current frame.
Boolean GetEventFromMovieFile(unsigned char *eventP, Boolean *isDownP)
{
    unsigned char event;
    int err = 0;
    Boolean gotEvent = false;

    if (gMovieFileIsOpen && gPlayingGameMovie) {
        if (!gNextFrameIsInitialized) {
            err = MyReadDataFromFile(gMovieFileRef, 4, &gNextFrame);
            gNextFrameIsInitialized = true;
        }
        if (gNextFrame == (Uint32)gRunningFrameCount) {
            err = MyReadDataFromFile(gMovieFileRef, 1, &event);
            if (err == 0 && event == kEndOfMovieEvent)
                return kEndOfMovieEvent;
            if (err == 0) {
                err = MyReadDataFromFile(gMovieFileRef, 4, &gNextFrame);
                if (err != 0)
                    return kEndOfMovieEvent;
            }
            if (err == 0) {
                if (event < 0x7f) {
                    *isDownP = true;
                    *eventP = event;
                }
                else {
                    *isDownP = false;
                    *eventP = (unsigned char)(event + 0x81);
                }
                gotEvent = true;
            }
        }
        else if (gNextFrame < (Uint32)gRunningFrameCount) {
            SDL_assert(!"movie event was skipped");
        }
    }
    if (err == -1)
        StopGameMovie();
    return gotEvent;
}

Boolean GetDataFromMovieFile(unsigned char *eventP, long size, void *dataP)
{
    unsigned char event;
    int err = 0;
    Boolean gotData = false;

    if (gMovieFileIsOpen && gPlayingGameMovie) {
        if (!gNextFrameIsInitialized) {
            err = MyReadDataFromFile(gMovieFileRef, 4, &gNextFrame);
            gNextFrameIsInitialized = true;
        }
        if (gNextFrame == (Uint32)gRunningFrameCount) {
            err = MyReadDataFromFile(gMovieFileRef, 1, &event);
            if (err == 0)
                err = MyReadDataFromFile(gMovieFileRef, size, dataP);
            if (err == 0)
                err = MyReadDataFromFile(gMovieFileRef, 4, &gNextFrame);
            if (err == 0) {
                *eventP = event;
                gotData = true;
            }
        }
        else if (gNextFrame < (Uint32)gRunningFrameCount) {
            StopGameMovie();
        }
    }
    if (err == -1)
        StopGameMovie();
    return gotData;
}

/// One recorded event: from a replay, or from the other computer in a two-computer game.
void ApplyMovieEvent(unsigned char event, Boolean isDown)
{
    switch (event) {
        case kMovieKeyLeft:      gKeys[kKeyLeft] = isDown;      break;
        case kMovieKeyRight:     gKeys[kKeyRight] = isDown;     break;
        case kMovieKeyDown:      gKeys[kKeyDown] = isDown;      break;
        case kMovieKeyUp:        gKeys[kKeyUp] = isDown;        break;
        case kMovieKeyJump:      gKeys[kKeyJump] = isDown;      break;
        case kMovieKeyJumpLeft:  gKeys[kKeyJumpLeft] = isDown;  break;
        case kMovieKeyJumpRight: gKeys[kKeyJumpRight] = isDown; break;
        case kMovieKeyAction:    gKeys[kKeyAction] = isDown;    break;
        case kMovieResetKeys:    ResetKeys();                   break;
        case kMovieEasyJumpingOn:  SetEasyJumpingMode(true);    break;
        case kMovieEasyJumpingOff: SetEasyJumpingMode(false);   break;
        case kMovieActionSound:
            if (!gDemoMode)
                MMPlaySound(0x4a, 8, 1);
            break;
        case kMovieSignDismissed:
            gReadingSignWasDismissed = true;
            if (!NetGameActive())       // two computers: closed after the frame, as on the other one
                ExitSignReadingIfPossible();
            break;
        case kEndOfMovieEvent:
            StopGameMovie();
            break;
        default:
            if (event >= kMovieFirstCheat && event <= kMovieLastCheat) {
                DoCheatCode(event - kMovieFirstCheat);
                if (FindSound(0x48) == -1)
                    MMPlaySound(0x48, 8, 1);
            }
            break;
    }
}

void GetKeyEventsFromMovie(void)
{
    unsigned char event;
    Boolean isDown, gotEvent;

    do {
        gotEvent = GetEventFromMovieFile(&event, &isDown);
        if (gotEvent)
            ApplyMovieEvent(event, isDown);
    } while (gotEvent);
}

/// Records a key change. `scancode` is an SDL scancode.
void AddKeyToMovie(long scancode, Boolean isDown, Boolean isRepeat)
{
    if (gArrowKeysEnabled) {
        if (scancode == SDL_SCANCODE_LEFT)
            WriteEventToMovieFile(kMovieKeyLeft, isDown);
        else if (scancode == SDL_SCANCODE_RIGHT)
            WriteEventToMovieFile(kMovieKeyRight, isDown);
        else if (scancode == SDL_SCANCODE_DOWN)
            WriteEventToMovieFile(kMovieKeyDown, isDown);
        else if (scancode == SDL_SCANCODE_UP)
            WriteEventToMovieFile(kMovieKeyUp, isDown);
        else if (scancode == SDL_SCANCODE_SPACE && !isRepeat)
            WriteEventToMovieFile(kMovieKeyJump, isDown);
    }

    if (gKeyPrefs.primaryKeys[kKeyLeft] == scancode)
        WriteEventToMovieFile(kMovieKeyLeft, isDown);
    else if (gKeyPrefs.primaryKeys[kKeyRight] == scancode)
        WriteEventToMovieFile(kMovieKeyRight, isDown);
    else if (gKeyPrefs.primaryKeys[kKeyDown] == scancode)
        WriteEventToMovieFile(kMovieKeyDown, isDown);
    else if (gKeyPrefs.primaryKeys[kKeyUp] == scancode)
        WriteEventToMovieFile(kMovieKeyUp, isDown);
    else if (gKeyPrefs.primaryKeys[kKeyJump] == scancode)
        WriteEventToMovieFile(kMovieKeyJump, isDown);
    else if (gKeyPrefs.primaryKeys[kKeyJumpLeft] == scancode)
        WriteEventToMovieFile(kMovieKeyJumpLeft, isDown);
    else if (gKeyPrefs.primaryKeys[kKeyJumpRight] == scancode)
        WriteEventToMovieFile(kMovieKeyJumpRight, isDown);
    else if (gKeyPrefs.primaryKeys[kKeyAction] == scancode)
        WriteEventToMovieFile(kMovieKeyAction, isDown);
    else if (gKeyPrefs.primaryKeys[kKeyMap] == scancode && !gGameIsPaused && isDown &&
             gCurrentGameMode == kPlayingGameMode)
        WriteEventToMovieFile(kMovieActionSound, isDown);
}

void OpenMovieFileForOutput(Boolean isNewGame, Boolean isSaveFile, float heroHoriz, float heroVert)
{
    char mansionName[255], path[500];
    const char *folder;
    Sint32 version = kMovieFileVersion, seed, marker = kMovieHeaderEndMarker;
    Uint8 newGameFlag = isNewGame;
    float moveDeltaH = 0, moveDeltaV = 0;
    short vineTileLayer = 4, state = 0;
    SpritePtr heroP;

    if (IsMovieFileOpen()) {
        if (!IsMovieFileOpenForRecording())
            return;
        CloseGameMovieFile();
    }

    seed = (Sint32)time(NULL);
    printf("Random number seed selected at start of movie: %d \n", (int)seed);
    SelectStream(0);
    PutSeed(seed);

    folder = GetMoviesFilePath(NULL);
    strcpy(mansionName, gMansionFilename);
    StripStringFromEndOfCString(".zip", mansionName, NULL);
    StripStringFromEndOfCString(".ZIP", mansionName, NULL);
    snprintf(path, sizeof(path), "%s%s %d", folder, mansionName,
             CountNumSuccessiveFiles(folder, mansionName, "") + 1);
    printf("Attempting to create movie file: %s\n", path);

    gMovieFileRef = SDL_RWFromFile(path, "wb");
    if (gMovieFileRef == NULL)
        return;

    SetFileCreatorAndTypeForPath(path, 'MANS', 'MOV2');
    gMovieFileIsOpen = true;
    gPlayingGameMovie = false;
    MyWriteDataToFile(gMovieFileRef, 4, &version);
    MyWriteDataToFile(gMovieFileRef, 4, &seed);
    MyWriteDataToFile(gMovieFileRef, 1, &newGameFlag);
    MyWriteDataToFile(gMovieFileRef, 1, &gEasyJumping);
    if (!isNewGame) {
        WriteSaveDataToFile(gMovieFileRef, heroHoriz, heroVert, isSaveFile);
        MyWriteDataToFile(gMovieFileRef, 1, &gSilverFloorIsActive);
        MyWriteDataToFile(gMovieFileRef, 1, &gGoldFloorIsActive);
        heroP = GetHeroSprite();
        if (heroP != NULL) {
            moveDeltaH = SWGetSpriteHorizMoveDelta(heroP);
            moveDeltaV = SWGetSpriteVertMoveDelta(heroP);
            vineTileLayer = HERO(heroP)->vineTileLayer;
        }
        MyWriteDataToFile(gMovieFileRef, 4, &moveDeltaH);
        MyWriteDataToFile(gMovieFileRef, 4, &moveDeltaV);
        MyWriteDataToFile(gMovieFileRef, 2, &vineTileLayer);
        if (heroP != NULL)
            state = GetSpriteState(heroP);
        MyWriteDataToFile(gMovieFileRef, 2, &state);
        MyWriteDataToFile(gMovieFileRef, 4, &marker);
    }
    else {
        MyWriteDataToFile(gMovieFileRef, 255, gMansionFilename);
        MyWriteDataToFile(gMovieFileRef, 1, &gDifficultySetting);
        MyWriteDataToFile(gMovieFileRef, 2, &gCurrentMansionNumber);
        MyWriteDataToFile(gMovieFileRef, 4, &marker);
    }
    UpdateMenuBar();
}

void WriteEventToMovieFile(char event, Boolean isDown)
{
    Sint32 frame;
    char byte = event;

    NetRecordEvent(event, isDown);      // two computers: the other one does the same

    if (gMovieFileIsOpen && !gPlayingGameMovie) {
        frame = (Sint32)gRunningFrameCount;
        MyWriteDataToFile(gMovieFileRef, 4, &frame);
        if (!isDown)
            byte = (char)(event + 0x7f);
        MyWriteDataToFile(gMovieFileRef, 1, &byte);
    }
}

void WriteDataToMovieFile(char event, long size, void *dataP)
{
    Sint32 frame;

    if (gMovieFileIsOpen && !gPlayingGameMovie) {
        frame = (Sint32)gRunningFrameCount;
        MyWriteDataToFile(gMovieFileRef, 4, &frame);
        MyWriteDataToFile(gMovieFileRef, 1, &event);
        MyWriteDataToFile(gMovieFileRef, size, dataP);
    }
}

void WriteCurrentKeysStateToMovie(void)
{
    if (gKeys[kKeyLeft])      WriteEventToMovieFile(kMovieKeyLeft, true);
    if (gKeys[kKeyRight])     WriteEventToMovieFile(kMovieKeyRight, true);
    if (gKeys[kKeyDown])      WriteEventToMovieFile(kMovieKeyDown, true);
    if (gKeys[kKeyUp])        WriteEventToMovieFile(kMovieKeyUp, true);
    if (gKeys[kKeyJump])      WriteEventToMovieFile(kMovieKeyJump, true);
    if (gKeys[kKeyJumpLeft])  WriteEventToMovieFile(kMovieKeyJumpLeft, true);
    if (gKeys[kKeyJumpRight]) WriteEventToMovieFile(kMovieKeyJumpRight, true);
    if (gKeys[kKeyAction])    WriteEventToMovieFile(kMovieKeyAction, true);
}

void StopGameMovie(void)
{
    gEndGame = true;
}

Boolean IsMovieFileOpen(void)
{
    return gMovieFileIsOpen;
}

Boolean IsMoviePlaying(void)
{
    return gPlayingGameMovie;
}

Boolean IsMovieFileOpenForRecording(void)
{
    return gMovieFileIsOpen && !gPlayingGameMovie;
}

void CloseGameMovieFile(void)
{
    if (!gMovieFileIsOpen)
        return;
    if (!gPlayingGameMovie) {
        WriteEventToMovieFile(kEndOfMovieEvent, false);
        puts("Wrote kEndOfMovieEvent");
    }
    SDL_RWclose(gMovieFileRef);
    gMovieFileRef = NULL;
    gMovieFileIsOpen = false;
    gPlayingGameMovie = false;
    gNextFrameIsInitialized = false;
    puts("CloseGameMovieFile() called.");
}

// ---------------------------------------------------------------------------
// Debug output log ("Output File Log" in Application Support)

void OpenOutputFile(const char *fileName)
{
    short day, month, year;

    if (gFileIsOpen)
        CloseOutputFile();
    gOutputFileP = fopen(GetAppSupportPath(fileName), "w");
    if (gOutputFileP != NULL) {
        gFileIsOpen = true;
        MMGetCurrentDate(&day, &month, &year);
        WriteCStringToFile("Date: ");
        WriteNumberToFile(month);
        WriteCStringToFile("/");
        WriteNumberToFile(day);
        WriteCStringToFile("/");
        WriteNumberToFile(year);
        WriteCarriageReturn(1);
    }
}

void MMGetCurrentDate(short *day, short *month, short *year)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    *day = (short)t->tm_mday;
    *month = (short)(t->tm_mon + 1);
    *year = (short)(t->tm_year + 1900);
}

void WriteRoomInfoToFile(void)
{
    if (gFileIsOpen) {
        WriteNumberToFile(GEGetLevelResID(gCurrentLevel, 1, 200));
        WriteCStringToFile(" (");
        WriteNumberToFile(gNumLivesLeft);
        WriteCStringToFile(" lives left.)\r");
    }
}

void WriteCarriageReturn(short count)
{
    short i;
    if (gFileIsOpen)
        for (i = 0; i < count; i++)
            fputc('\r', gOutputFileP);
}

void WriteNumberToFile(long number)
{
    if (gFileIsOpen)
        fprintf(gOutputFileP, "%ld", number);
}

void WriteCStringToFile(const char *string)
{
    if (gFileIsOpen)
        fputs(string, gOutputFileP);
}

Boolean IsOutputFileOpen(void)
{
    return gFileIsOpen;
}

void CloseOutputFile(void)
{
    if (gFileIsOpen) {
        WriteCarriageReturn(2);
        fclose(gOutputFileP);
        gOutputFileP = NULL;
        gFileIsOpen = false;
    }
}

// ---------------------------------------------------------------------------
// Suspended games

void SuspendGame(void)
{
    char mansionName[255], fileName[255], path[500];
    Str255 pascalName;
    SpritePtr heroP;
    SDL_RWops *file;
    Sint32 version = kSuspendFileVersion, marker = kMovieHeaderEndMarker;
    short vineTileLayer, state;

    heroP = GetHeroForSuspendGame();
    if (heroP == NULL || IsMoviePlaying())
        return;

    strcpy(mansionName, gMansionFilename);
    StripStringFromEndOfCString(".zip", mansionName, NULL);
    StripStringFromEndOfCString(".ZIP", mansionName, NULL);
    GetPascalStringFromCString(pascalName, mansionName);
    SWConcatPascalStrings(pascalName, (ConstStringPtr)"\012 Suspended");
    SWCopyPascalStringToCString(pascalName, fileName);
    GetSuspendedGameFilePath(path, fileName);

    file = SDL_RWFromFile(path, "w+");
    if (file == NULL) {
        MMDoErrorMessage("Sorry, but the suspended game couldn't be created.", 0);
        return;
    }

    MyWriteDataToFile(file, 4, &version);
    WriteSaveDataToFile(file, SWGetSpriteHorizLoc(heroP), SWGetSpriteVertLoc(heroP), false);
    vineTileLayer = HERO(heroP)->vineTileLayer;
    MyWriteDataToFile(file, 2, &vineTileLayer);
    state = GetSpriteState(heroP);
    MyWriteDataToFile(file, 2, &state);
    MyWriteDataToFile(file, 4, &marker);

    gSuspendedGameExists = true;
    GetHFSUniFromCString(gSuspendedGameFilenameHFSUni, fileName);
    SDL_RWclose(file);
    SetFileCreatorAndTypeForPath(path, 'MANS', 'SUSP');
    WriteProfileToDisk();
}

int OpenSuspendedGame(const char *path)
{
    float heroHoriz = -1000.0f, heroVert = -1000.0f;
    Sint32 version = 0, marker;
    long saveVersion = 0;
    short vineTileLayer = 0, state = 0;
    Boolean isSaveFile = false;
    SDL_RWops *file;
    SpritePtr heroP;
    int err;

    file = SDL_RWFromFile(path, "rb");
    if (file == NULL) {
        printf("Suspended game file not found: %s \n", path);
        return -1;
    }

    SetUpNewGame();
    MMPauseGameTimer();
    MyReadDataFromFile(file, 4, &version);
    SDL_assert(version == kSuspendFileVersion);
    err = LoadSaveDataFromFile(file, &heroHoriz, &heroVert, &isSaveFile, &saveVersion);
    if (err == 0) {
        MyReadDataFromFile(file, 2, &vineTileLayer);
        MyReadDataFromFile(file, 2, &state);
        MyReadDataFromFile(file, 4, &marker);
        SDL_assert(marker == kMovieHeaderEndMarker);
    }

    if (err == 0) {
        SetCurMansionAsCustomMansion(gCurrentMansionNumber == -1);
        ResetKeys();
        SDL_assert(heroHoriz != -1000.0f);
        ChangeWorldRectForGame(gSpriteWorldP, gStatsHeight);
        SWSetPostEraseCallBack(gSpriteWorldP, MyPostEraseCallback);
        err = HandleRestoreStuff(heroHoriz, heroVert, isSaveFile, saveVersion);
        if (err == 0) {
            heroP = GetHeroSprite();
            if (gMusicIsOn)
                ResumeMusic(true);
            if (heroP != NULL) {
                StartHeroInState(heroP, state, vineTileLayer);
                if (state == 1) {
                    HERO(heroP)->hasClimbed = false;
                    HERO(heroP)->isLanding = false;
                }
                SaveStartingPosition(heroP);
                // Jack blinks until the player presses a key.
                SWSetSpriteMoveProc(heroP, BlinkingSpriteMoveProc);
                SWSetSpriteFrameProc(heroP, BlinkingSpriteFrameProc);
                SWSetSpriteFrameTime(heroP, 250);
                SWSetSpriteFrameAdvance(heroP, 0);
                SWRemoveSprite(heroP);
                SWAddSprite(gDeadHeroSpriteLayerP, heroP);
            }
        }
        if (err != 0)
            MyRestoreWorldRect(gSpriteWorldP);
    }
    SDL_RWclose(file);
    return err;
}

/// Jack, or the blinking Jack waiting after a restore.
SpritePtr GetHeroForSuspendGame(void)
{
    SpritePtr spriteP = GetHeroSprite();

    if (spriteP == NULL) {
        while ((spriteP = SWGetNextSprite(gDeadHeroSpriteLayerP, spriteP)) != NULL)
            if (spriteP->spriteMoveProc == BlinkingSpriteMoveProc)
                break;
    }
    return spriteP;
}

Boolean CanSuspendGame(void)
{
    if (NetGameActive() || IsMultiplayerGame())     // a team's game is saved at pedestals, for the team
        return false;
    SpritePtr heroP;
    MoveProcPtr moveProc;

    if (IsMoviePlaying())
        return false;
    heroP = GetHeroForSuspendGame();
    if (heroP == NULL || HERO(heroP)->platformP != NULL)
        return false;
    moveProc = heroP->spriteMoveProc;
    if (!HERO(heroP)->onGround && moveProc != ClimbingSpriteMoveProc && moveProc != VineClimbingMoveProc &&
        moveProc != PoleSlidingMoveProc && moveProc != BlinkingSpriteMoveProc)
        return false;
    return moveProc != FinishClimbingLadderMoveProc;
}

void DeleteSuspendedGame(const char *path)
{
    remove(path);
    gSuspendedGameExists = false;
    WriteProfileToDisk();
}

// ---------------------------------------------------------------------------
// Player profiles

void CreateNewProfile(ConstStringPtr playerName)
{
    char cName[256];

    gPlayTutorialMansion = true;
    gSuspendedGameExists = false;
    gHasShownOrganScreen = false;
    InitCompletedMansionsArray();
    SWCopyPascalString(playerName, gDefaultHighScoreName);
    SWCopyPascalString(playerName, (StringPtr)gCurrentPlayerName);
    gCurrentPlayerNameExists = true;
    WritePreferencesFile();
    WriteProfileToDisk();
    SWCopyPascalStringToCString(playerName, cName);
    CreateSubdirectoriesForProfile(cName);
    LoadLifetimeStats(0, cName);
}

void CreateSubdirectoriesForProfile(const char *playerName)
{
    char path[500];

    GetPlainSuspendedGameFilePath(path, NULL);
    MMCreateDirectory(GetPlainSavedGamesFilePath(NULL), playerName);
    MMCreateDirectory(path, playerName);
    MMCreateDirectory(GetUserMoviesPath(NULL), playerName);
}

void WriteProfileToDisk(void)
{
    char cName[256];
    const char *path;
    SDL_RWops *file;
    Sint32 version = kProfileVersion, marker = kMovieHeaderEndMarker;

    SWCopyPascalStringToCString((ConstStringPtr)gCurrentPlayerName, cName);
    path = GetPlayerProfilePath(cName);
    file = SDL_RWFromFile(path, "w+");
    if (file == NULL)
        return;
    MyWriteDataToFile(file, 4, &version);
    MyWriteDataToFile(file, 1, &gPlayTutorialMansion);
    MyWriteDataToFile(file, 1, &gSuspendedGameExists);
    MyWriteDataToFile(file, sizeof(gSuspendedGameFilenameHFSUni), gSuspendedGameFilenameHFSUni);
    MyWriteDataToFile(file, 1, &gHasShownOrganScreen);
    MyWriteDataToFile(file, 256, gDefaultHighScoreName);
    WriteMansionArrayToFile(file);
    MyWriteDataToFile(file, 4, &marker);
    SDL_RWclose(file);
    SetFileCreatorAndTypeForPath(path, 'MANS', 'PLYR');
}

/// Returns 0, -1 (missing/unreadable) or -2 (old format).
int LoadProfileFromDisk(ConstStringPtr playerName)
{
    char cName[256];
    SDL_RWops *file;
    Sint32 version = 0, marker = 0;

    SWCopyPascalStringToCString(playerName, cName);
    file = SDL_RWFromFile(GetPlayerProfilePath(cName), "rb");
    if (file == NULL)
        return -1;

    MyReadDataFromFile(file, 4, &version);
    if (version < 0x65) {
        SDL_RWclose(file);
        gCurrentPlayerNameExists = false;
        return version == 100 ? -2 : -1;
    }
    MyReadDataFromFile(file, 1, &gPlayTutorialMansion);
    MyReadDataFromFile(file, 1, &gSuspendedGameExists);
    MyReadDataFromFile(file, sizeof(gSuspendedGameFilenameHFSUni), gSuspendedGameFilenameHFSUni);
    MyReadDataFromFile(file, 1, &gHasShownOrganScreen);
    if (version < kProfileVersion)
        SWCopyPascalString(playerName, gDefaultHighScoreName);
    else
        MyReadDataFromFile(file, 256, gDefaultHighScoreName);
    ReadMansionArrayFromFile(file);
    MyReadDataFromFile(file, 4, &marker);
    SDL_assert(marker == kMovieHeaderEndMarker);
    SDL_RWclose(file);

    gCurrentPlayerNameExists = true;
    SWCopyPascalString(playerName, (StringPtr)gCurrentPlayerName);
    CreateSubdirectoriesForProfile(cName);
    LoadLifetimeStats(0, cName);
    return 0;
}

void DeleteProfile(const char *playerName)
{
    remove(GetPlayerProfilePath(playerName));
    printf("Removing file: %s \n", GetPlayerProfilePath(playerName));
}

void SaveCurrentProfile(void)
{
    gProfileWasSaved = true;
    gSaveCurrentPlayerNameExists = gCurrentPlayerNameExists;
    SWCopyPascalString((ConstStringPtr)gCurrentPlayerName, gSaveCurrentPlayerName);
}

void RestoreCurrentProfile(void)
{
    if (gProfileWasSaved &&
        !SWEqualString((ConstStringPtr)gCurrentPlayerName, gSaveCurrentPlayerName)) {
        gCurrentPlayerNameExists = gSaveCurrentPlayerNameExists;
        SWCopyPascalString(gSaveCurrentPlayerName, (StringPtr)gCurrentPlayerName);
        gProfileWasSaved = false;
        LoadProfileFromDisk(gSaveCurrentPlayerName);
    }
}

short CountNumProfilesOnDisk(void)
{
    return CountNumFilesInFolder(GetPlayerProfilePath(NULL));
}

Boolean DoesProfileAlreadyExist(const char *playerName)
{
    DIR *dir = opendir(GetPlayerProfilePath(NULL));
    struct dirent *entry;
    Boolean exists = false;

    if (dir == NULL)
        return false;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && strcmp(entry->d_name, playerName) == 0) {
            exists = true;
            break;
        }
    }
    closedir(dir);
    return exists;
}

// ---------------------------------------------------------------------------
// Saved games

int MMRestoreFromSaveGameFile(const char *path, const char *fileName)
{
    float heroHoriz, heroVert;
    long saveVersion;
    Boolean isSaveFile;
    SDL_RWops *file;
    int err;

    printf("About to restore from saved game file at: %s \n", path);
    file = SDL_RWFromFile(path, "rb");
    if (file == NULL)
        return -1;

    SetUpNewGame();
    MMPauseGameTimer();
    strcpy(gLastSaveFilename, fileName);
    err = LoadSaveDataFromFile(file, &heroHoriz, &heroVert, &isSaveFile, &saveVersion);
    if (err == -2) {
        SDL_RWclose(file);
        MMDoCustomDialog(gSpriteWorldP,
            "Sorry, this saved game is in an old format that can't be used by Midnight Mansion HD.", 3);
        return -11111;
    }
    if (gCurrentMansionNumber != -2 && IsMansionLocked(gCurrentMansionNumber)) {
        SDL_RWclose(file);
        return -11223;
    }
    err = SDL_RWclose(file);
    if (err == 0 && gCurrentlyInMansion) {
        SetCurMansionAsCustomMansion(gCurrentMansionNumber == -1);
        RestoreJacksLives();
        ChangeWorldRectForGame(gSpriteWorldP, gStatsHeight);
        SWSetPostEraseCallBack(gSpriteWorldP, MyPostEraseCallback);
        err = HandleRestoreStuff(heroHoriz, heroVert, isSaveFile, saveVersion);
        printf("Restored from saved game file. err = %d \n", err);
        if (err != 0)
            MyRestoreWorldRect(gSpriteWorldP);
    }
    return err;
}

/// Opens the mansion named in the restored data and starts playing at the saved position.
/// `isSaveFile` means restoring at a save pedestal (statue animation) rather than a suspend.
int HandleRestoreStuff(float heroHoriz, float heroVert, Boolean isSaveFile, long version)
{
    char mansionName[255], message[500];
    const char *mansionPath;
    SpritePtr heroP;
    int err;

    if (version < 0x6a) {
        ErrorMessage("This saved game is in a format that is too old to open.", 0);
        return -1;
    }

    strcpy(mansionName, gMansionFilename);
    StripStringFromEndOfCString(".zip", mansionName, NULL);
    StripStringFromEndOfCString(".ZIP", mansionName, NULL);
    printf("HandleRestoreStuff entered. mansionNameNoZIPExt = %s\n", mansionName);
    mansionPath = GetPathForAnyMansionFile(mansionName);
    printf("LOADING MANSION FROM PATH: %s \n", mansionPath);
    strcpy(gCustomMansionFolderPath, mansionPath);

    err = OpenMansionUsingFilePath(mansionPath, gMansionFilename);
    if (err != 0 && gMansionIsCustomMansion)
        err = OpenMansionUsingFilePath(GetMansionFolderPath(gMansionFilename), gMansionFilename);

    if (err != 0) {
        if (!gMansionIsCustomMansion)
            snprintf(message, sizeof(message),
                     " The mansion file '%s' could not be found. Please make sure this mansion is inside the "
                     "Game Data -> Mansions folder and try again.", gMansionFilename);
        else
            snprintf(message, sizeof(message),
                     " The mansion file '%s' could not be found. Please make sure this mansion is inside the "
                     "Custom Mansions folder and try again.", gMansionFilename);
        MMDoCustomDialog(gSpriteWorldP, message, 3);
        return -22222;
    }

    MultiplayerMansionOpened();        // a continued multiplayer game: whose turn it is
    RestoreStatsFromObjectsCarried();
    PlayBackgroundMusic(gCurrentMusicIndex);
    PauseMusic();
    gCurrentGameMode = kPlayingGameMode;
    gRunningFrameCount = 0;
    if (gUseOutputFile && !gEditorIsRunning && !IsMoviePlaying())
        OpenOutputFile("Output File Log");
    if (gRecordMovies && !gEditorIsRunning && !IsMoviePlaying())
        OpenMovieFileForOutput(false, isSaveFile, heroHoriz, heroVert);
    EnableRecordMovieMenuItem();
    ResetFloorTileAnimations(false, true);
    SetCorrectGoldSilverTileImages(false);
    NetSeedRandom();                    // two computers continuing a saved game: the same random numbers
    LoadLevel(gCurrentLevel, true);
    SetUpLevelForPlaying();
    WriteCStringToFile("RESTORED FROM SAVED OR SUSPENDED GAME.\r");
    SWDisposeAllSpritesInLayer(gHeroSpriteLayerP);
    SWDisposeAllSpritesInLayer(gHeroOnPlatformSpriteLayerP);

    if (!isSaveFile) {
        heroP = NewMainSprite();
        if (heroP != NULL) {
            SWSetSpriteLocation(heroP, heroHoriz, heroVert);
            SaveStartingPosition(heroP);
            UpdateHaloSpritePosition(heroP);
        }
    }
    else {
        // Jack reappears from the save statue.
        SWDisposeAllSpritesInLayer(gStatueSpriteLayerP);
        CreateColorAndGrayStatues((short)heroHoriz, (short)heroVert, false);
        PauseSpriteLayersForSaveAnim();
        AddObjectToDoOnceArray(gLastSavePedestalObjectID);
        gLastSavePedestalObjectID = 0;
    }
    SaveHeroStartingLocation((short)heroHoriz, (short)heroVert);
    StopLoopingSounds();
    if (!gUsingEditor)
        MMHideAllCursors();
    else {
        MMRemoveGameCursor();
        MMShieldCursor();
    }
    DrawLevel();
    gSpriteWorldIsValid = true;
    DoGameFadeToNormal(30);
    gPlayingGame = true;
    if (isSaveFile)
        MyDelay();
    RunAnimation();
    return 0;
}

const char *GetPathForAnyMansionFile(const char *mansionName)
{
    if (!gMansionIsCustomMansion)
        return GetMansionFolderPath(mansionName);
    return GetCustomMansionsFolderPath(mansionName);
}

int MMCreateSaveGameFile(const char *path)
{
    SDL_RWops *file;
    SpritePtr heroP;
    float h = 0, v = 0;
    int err;

    file = SDL_RWFromFile(path, "w+");
    if (file == NULL)
        err = -1;
    else {
        heroP = GetHeroSprite();
        if (heroP != NULL) {
            h = SWGetSpriteHorizLoc(heroP);
            v = SWGetSpriteVertLoc(heroP);
        }
        ClearFileStickyError();
        WriteSaveDataToFile(file, h, v, true);
        err = GetFileStickyError();
        SDL_RWclose(file);
        if (err == 0)
            printf("Save game file successfully created: %s \n", path);
        SetFileCreatorAndTypeForPath(path, 'MANS', 'SAVE');
    }
    if (err != 0)
        ErrorMessage("Sorry, the save game file couldn't be created. Function MMCreateSaveGameFile, error code %d.",
                     err);
    return err;
}

/// Returns the MMDoCustomDialog button: 1 cancel, 2 suspend/quit, 3 don't suspend.
short DoSuspendGameDialog(void)
{
    short result;

    if (IsMoviePlaying() ||
        (gCurrentGameMode != kPlayingGameMode && gCurrentGameMode != kInGameMapMode))
        return 3;

    if (gCurrentGameMode == kInGameMapMode && !gGameIsPaused)
        DrawMapInOffscreenAreas();

    if (!CanSuspendGame()) {
        if (gCurrentGameMode == kInGameMapMode)
            result = MMDoCustomDialog(gSpriteWorldP,
                "Are you sure you want to quit without suspending? (You can't suspend the game while looking "
                "at the map screen.)", 1);
        else
            result = MMDoCustomDialog(gSpriteWorldP,
                "Are you sure you want to quit without suspending? (You can't suspend the game while Jack is "
                "jumping, dying, or riding on a moving platform.)", 1);
    }
    else {
        result = MMDoCustomDialog(gSpriteWorldP, "Suspend game so you can resume it later?", 0);
        if (result == 2)
            SuspendGame();
    }

    if (result == 1 && (gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kInGameMapMode) &&
        !gGameIsPaused)
        MMHideCursor();
    return result;
}

// ---------------------------------------------------------------------------
// Messages

void MMDoErrorMessage(const char *format, long value)
{
    char message[304];

    if (value == 0)
        snprintf(message, sizeof(message), "%s", format);
    else
        snprintf(message, sizeof(message), format, value);
    if (gSpriteWorldP == NULL)
        printf("%s\n", message);
    else
        MMDoCustomDialog(gSpriteWorldP, message, 3);
}

void MMReportAdvancedError(const char *format, long value, const char *function, long line, long unused)
{
    char message[304], full[400];

    (void)unused;
    if (value == 0)
        snprintf(message, sizeof(message), "%s", format);
    else
        snprintf(message, sizeof(message), format, value);
    snprintf(full, sizeof(full), "%s  Function: %s   Line: %ld", message, function, line);
    if (gSpriteWorldP == NULL)
        printf("%s\n", full);
    else
        MMDoCustomDialog(gSpriteWorldP, full, 3);
}

void HandleCustomDialogKeyEvent(long scancode, Boolean isDown)
{
    if (scancode == SDL_SCANCODE_RETURN)
        gReturnKeyIsDown = isDown;
    else if (scancode == SDL_SCANCODE_ESCAPE)
        gEscKeyIsDown = isDown;
}

/// Scale of the 1280-wide design for the current window, as (h, v) multiples of 640x480.
static void GetDialogScale(float *scaleH, float *scaleV)
{
    *scaleH = (float)(gSpriteWorldP->designWidth / 640);
    *scaleV = (float)(gSpriteWorldP->designWidth / 480);
}

/// Shows a modal message box drawn with game sprites.
/// dialogType 0: Cancel / Don't Suspend / Suspend (returns 1, 3, 2)
///            1: Cancel / OK (1, 2)    2: Don't Suspend / Suspend (3, 2)    3: OK only (0)
short MMDoCustomDialog(SpriteWorldPtr spriteWorldP, const char *text, short dialogType)
{
    SpritePtr dialogP = NULL, textP, fadeP;
    TileChangeProcPtr saveTileChangeProc;
    SWRect textRect;
    SWPoint mouse;
    float saveZoomH, saveZoomV, saveOffsetH, saveOffsetV, scaleH, scaleV;
    Boolean fadeLayerWasVisible, wasPaused, mouseDown, done;
    short h, v, result = 1, button;
    SWError err;

    if (gCustomDialogIsVisible)
        return 1;

    fadeLayerWasVisible = gFadeToBlackSpriteLayerP->layerIsVisible;
    SWSetSpriteLayerVisible(gFadeToBlackSpriteLayerP, false);
    saveZoomH = spriteWorldP->zoomH;
    saveZoomV = spriteWorldP->zoomV;
    SWSetSpriteWorldZoom(spriteWorldP, 1.0f, 1.0f);
    saveOffsetH = gSpriteWorldP->spriteOffsetH;
    saveOffsetV = gSpriteWorldP->spriteOffsetV;
    gSpriteWorldP->spriteOffsetH = 0;
    gSpriteWorldP->spriteOffsetV = 0;
    if (gCurrentGameMode == kInGameMapMode) {
        fadeP = NewFadeSprite(gSpriteWorldP, 0);
        SWRemoveSprite(fadeP);
        SWAddSprite(gCustomDialogSpriteLayerP, fadeP);
        SWSetTilingOn(gSpriteWorldP, false);
    }
    GetDialogScale(&scaleH, &scaleV);
    gReturnKeyIsDown = false;
    gEscKeyIsDown = false;
    saveTileChangeProc = spriteWorldP->tileChangeProc;
    SWSetTileChangeProc(spriteWorldP, NULL);
    if (gCurrentGameMode == kPlayingGameMode)
        StopLoopingSounds();
    wasPaused = gGameIsPaused;
    PauseAllSpriteLayers(spriteWorldP);
    SWUnpauseSpriteLayer(gCustomDialogSpriteLayerP);
    SWSetSpriteLayerVisible(gCustomDialogSpriteLayerP, true);

    err = MMCreateSpriteFromVerticalFile(gSpriteWorldP, &dialogP, NULL, "custom dialog.png", 1, kNoMask);
    if (err == kNoError && dialogP != NULL) {
        gCustomDialogIsVisible = true;
        MMSaveCursorState();
        MMShieldCursor();
        MMSetCursorForGame();
        SWAddSprite(gCustomDialogSpriteLayerP, dialogP);
        SWSetSpriteScaledFactor(dialogP, scaleH / 2.0f, scaleV / 2.0f);
        SWCenterSpriteInRect(dialogP, &spriteWorldP->workAreaRect);
        SWOffsetSprite(dialogP, 0.0f, -50.0f);

        textRect = dialogP->destFrameRect;
        SWOffsetRect(&textRect, (short)(scaleH * 30.0f), (short)(scaleV * 24.0f));
        textRect.right = (short)(scaleH * -60.0f + textRect.right);
        textRect.bottom = (short)(scaleV * -85.0f + textRect.bottom);
        textP = NewBitmapTextSprite(textRect.right - textRect.left);
        if (textP != NULL) {
            SWSetSpriteScaledFactor(textP, scaleH * 0.3f, scaleV * 0.3f);
            SetBMTextSpriteText(textP, text);
            SWAddSprite(gCustomDialogSpriteLayerP, textP);
            SWSetSpriteLocation(textP, textRect.left, textRect.top);
        }

        // Buttons are laid out right to left from the dialog's lower-right corner.
        v = (short)((SWGetSpriteHeight(dialogP) - (GetButtonHeight(0xda) + 16) * scaleV) +
                    (short)SWGetSpriteVertLoc(dialogP));
        h = (short)((SWGetSpriteWidth(dialogP) - (GetButtonWidth(0xda) + 19) * scaleH) +
                    (short)SWGetSpriteHorizLoc(dialogP));
        switch (dialogType) {
            case 0:
                AddButtonToDialog(0xda, 2, gCustomDialogSpriteLayerP, h, v);
                h = (short)(h - (GetButtonWidth(0xda) + 4) * scaleH);
                AddButtonToDialog(0xda, 3, gCustomDialogSpriteLayerP, h, v);
                h = (short)(scaleH * 30.0f + SWGetSpriteHorizLoc(dialogP));
                AddButtonToDialog(0xda, 1, gCustomDialogSpriteLayerP, h, v);
                break;
            case 1:
                AddButtonToDialog(0xda, 2, gCustomDialogSpriteLayerP, h, v);
                h = (short)(scaleH * 30.0f + SWGetSpriteHorizLoc(dialogP));
                AddButtonToDialog(0xda, 1, gCustomDialogSpriteLayerP, h, v);
                break;
            case 2:
                AddButtonToDialog(0xda, 2, gCustomDialogSpriteLayerP, h, v);
                h = (short)(h - (GetButtonWidth(0xda) + 4) * scaleH);
                AddButtonToDialog(0xda, 3, gCustomDialogSpriteLayerP, h, v);
                break;
            case 3:
                AddButtonToDialog(0xda, 0, gCustomDialogSpriteLayerP, h, v);
                break;
            default:
                SDL_assert(!"bad dialog type");
                break;
        }

        SWUpdateSpriteWorld(spriteWorldP, true);
        DoGameFadeToNormal(0);
        mouseDown = WasMouseClicked();
        done = false;
        gControllerTabStep = 0;
        while (!done && !gQuitGame) {
            SWProcessSpriteWorld(spriteWorldP);
            SWAnimateSpriteWorld(spriteWorldP);
            ProcessSDLEvents();
            mouseDown = MMGetMouse(&mouse);
            SWConvertCoordinates(spriteWorldP, &mouse);
            if (!spriteWorldP->frameHasOccurred)
                continue;
            button = ProcessButtonSprites(gCustomDialogSpriteLayerP, gCustomDialogSpriteLayerP, mouse,
                                          mouseDown, false, NULL);
            if (button == -1) {
                if (gEscKeyIsDown) {
                    if (dialogType == 2)
                        button = 3;
                    else if (dialogType != 3)
                        button = 1;
                }
                else if (gReturnKeyIsDown) {
                    if (dialogType <= 2)
                        button = 2;
                    else if (dialogType == 3)
                        button = 0;
                }
            }
            if (button != -1) {
                AnimateOneFrame(spriteWorldP);
                done = true;
                result = button;
            }
        }
        gCustomDialogIsVisible = false;
        MMRestoreCursorState();
    }

    gSpriteWorldP->spriteOffsetH = saveOffsetH;
    gSpriteWorldP->spriteOffsetV = saveOffsetV;
    spriteWorldP->zoomH = saveZoomH;
    spriteWorldP->zoomV = saveZoomV;
    SWSetSpriteLayerVisible(gFadeToBlackSpriteLayerP, fadeLayerWasVisible);
    SWSetTilingOn(gSpriteWorldP, true);
    SWDisposeAllSpritesInLayer(gCustomDialogSpriteLayerP);
    SWSetTileChangeProc(spriteWorldP, saveTileChangeProc);
    SWUpdateSpriteWorld(spriteWorldP, true);
    UnpauseAllSpriteLayers(spriteWorldP);
    if (wasPaused)
        PauseSpriteLayersForPause();
    ResetKeys();
    WriteEventToMovieFile(kMovieResetKeys, true);
    return result;
}

// ---------------------------------------------------------------------------
// Options dialog (two panes: Controls and Options)

enum {
    kOptionsOK = 0, kOptionsCancel = 1, kOptionsMusic = 5, kOptionsSofterMusic = 6, kOptionsSound = 7,
    kOptionsFullscreen = 8, kOptionsStretch = 9, kOptionsGamepad = 0x17,
    kOptionsTabOptions = 0xdf, kOptionsTabControls = 0xe0, kOptionsTabFun = 0xe3, kOptionsTabExtras = 0xe4
};

/// Panes: 1 Controls, 2 Options, 3 Fun, 4 Extras (the last two added by this version).
static SpriteLayerPtr OptionsPaneLayer(short pane)
{
    switch (pane) {
        case 1: return gOptionsDialogPane1LayerP;
        case 2: return gOptionsDialogPane2LayerP;
        case 3: return gOptionsDialogPane3LayerP;
        default: return gOptionsDialogPane4LayerP;
    }
}

static SpritePtr OptionsPaneTab(short pane)
{
    switch (pane) {
        case 1: return gControlsTabSpriteP;
        case 2: return gOptionsTabSpriteP;
        case 3: return gFunTabSpriteP;
        default: return gExtrasTabSpriteP;
    }
}

static void ShowOptionsPane(short pane)
{
    short i;

    if (pane == gCurOptionsDialogPane)
        return;
    SetButtonDimmedMode(OptionsPaneTab(gCurOptionsDialogPane), false);
    gCurOptionsDialogPane = pane;
    for (i = 1; i <= 4; i++)
        SWSetSpriteLayerVisible(OptionsPaneLayer(i), i == pane);
}

void MMDoOptionsDialog(void)
{
    SpritePtr dialogP = NULL;
    TileChangeProcPtr saveTileChangeProc;
    SWPoint mouse;
    float saveZoomH, saveZoomV, saveOffsetH, saveOffsetV, scaleH, scaleV;
    Boolean fadeLayerWasVisible, mouseDown, done, musicWasOn, wasFullscreenWindow, wasStretched;
    short button = -1;
    SWError err;

    if (!gMainSpritesAreLoaded)
        return;

    fadeLayerWasVisible = gFadeToBlackSpriteLayerP->layerIsVisible;
    SWSetSpriteLayerVisible(gFadeToBlackSpriteLayerP, false);
    saveZoomH = gSpriteWorldP->zoomH;
    saveZoomV = gSpriteWorldP->zoomV;
    SWSetSpriteWorldZoom(gSpriteWorldP, 1.0f, 1.0f);
    saveOffsetH = gSpriteWorldP->spriteOffsetH;
    saveOffsetV = gSpriteWorldP->spriteOffsetV;
    gSpriteWorldP->spriteOffsetH = 0;
    gSpriteWorldP->spriteOffsetV = 0;
    GetDialogScale(&scaleH, &scaleV);
    gControlsTabSpriteP = gOptionsTabSpriteP = gFunTabSpriteP = gExtrasTabSpriteP = NULL;
    gReturnKeyIsDown = false;
    gEscKeyIsDown = false;
    musicWasOn = gMusicIsOn;
    wasFullscreenWindow = gFullScreenWindow;
    wasStretched = gStretchToFit;
#ifdef MM_PS3
    PS3ExtrasTVSizeDialogOpened();
#endif
    SetMMEventCallback(HandleEventInKeyInputBox);
    saveTileChangeProc = gSpriteWorldP->tileChangeProc;
    SWSetTileChangeProc(gSpriteWorldP, NULL);
    SWSetSpriteLayerVisible(gCustomDialogSpriteLayerP, true);

    err = MMCreateSpriteFromVerticalFile(gSpriteWorldP, &dialogP, NULL, "custom dialog.png", 1, kNoMask);
    if (err == kNoError && dialogP != NULL) {
        gCustomDialogIsVisible = true;
        MMSaveCursorState();
        MMShieldCursor();
        MMSetCursorForGame();
        SWAddSprite(gCustomDialogSpriteLayerP, dialogP);
        SWSetSpriteScaledFactor(dialogP, scaleH * 0.65f, scaleV * 0.65f);
        SWCenterSpriteInRect(dialogP, &gSpriteWorldP->workAreaRect);
        if (gCurrentGameMode != kTitleScreenMode)
            SWOffsetSprite(dialogP, 0.0f, -50.0f);
        gCurOptionsDialogPane = 1;
        AddOptionsItemsToOptionsDialog(gOptionsDialogPane2LayerP, &dialogP->destFrameRect);
        AddControlsItemsToOptionsDialog(gOptionsDialogPane1LayerP, &dialogP->destFrameRect);
        AddExtrasItemsToOptionsDialog(gOptionsDialogPane3LayerP, &dialogP->destFrameRect, kExtrasTabFun);
        AddExtrasItemsToOptionsDialog(gOptionsDialogPane4LayerP, &dialogP->destFrameRect, kExtrasTabExtras);
        SWSetSpriteLayerVisible(gOptionsDialogPane1LayerP, true);
        SWSetSpriteLayerVisible(gOptionsDialogPane2LayerP, false);
        SWSetSpriteLayerVisible(gOptionsDialogPane3LayerP, false);
        SWSetSpriteLayerVisible(gOptionsDialogPane4LayerP, false);
        AddStandardItemsToOptionsDialog(gCustomDialogSpriteLayerP, dialogP);
        SWUpdateSpriteWorld(gSpriteWorldP, true);
        DoGameFadeToNormal(0);
        mouseDown = WasMouseClicked();
        done = false;
        while (!done && !gQuitGame) {
            SWProcessSpriteWorld(gSpriteWorldP);
            SWAnimateSpriteWorld(gSpriteWorldP);
            ProcessSDLEvents();
            mouseDown = MMGetMouse(&mouse);
            SWConvertCoordinates(gSpriteWorldP, &mouse);
            if (!gSpriteWorldP->frameHasOccurred)
                continue;
            button = ProcessButtonSprites(gCustomDialogSpriteLayerP, gOptionsDialogPane4LayerP, mouse,
                                          mouseDown, false, NULL);
            if (gCurOptionsDialogPane >= 3)
                UpdateExtrasOptionDescription(OptionsPaneLayer(gCurOptionsDialogPane),
                                              gCurOptionsDialogPane == 3 ? kExtrasTabFun : kExtrasTabExtras);
            if (button == -1) {
                if (gEscKeyIsDown)
                    button = kOptionsCancel;
                else if (gReturnKeyIsDown)
                    button = kOptionsOK;
            }
            if (button == kOptionsMusic)
                ToggleMusic();
            if (button == kOptionsFullscreen)
                ToggleFullscreen();
            if (button == kOptionsStretch)
                SetStretchToFit(!gStretchToFit);
#ifdef MM_PS3
            if (PS3ExtrasTVSizeOption() >= 0 && button == GetExtrasOptionButtonIndex(kExtrasTabExtras, PS3ExtrasTVSizeOption()))
                PS3ExtrasTVSizeNext(OptionsPaneLayer(4), button);
#endif
            if (button == kOptionsSofterMusic)
                SetMusicVolume(GetMusicVolume());
            if (button == kOptionsGamepad && !IsGamepadInitialized())
                InitGamePad();
            if (gControllerTabStep != 0) {      // L1 / R1: the tab before or after
                ShowOptionsPane((gCurOptionsDialogPane - 1 + gControllerTabStep + 4) % 4 + 1);
                gControllerTabStep = 0;
            }
            if (button == kOptionsTabControls)
                ShowOptionsPane(1);
            else if (button == kOptionsTabOptions)
                ShowOptionsPane(2);
            else if (button == kOptionsTabFun)
                ShowOptionsPane(3);
            else if (button == kOptionsTabExtras)
                ShowOptionsPane(4);
            if (button == kOptionsCancel || button == kOptionsOK) {
                if (button == kOptionsOK) {
                    UpdateDataFromControlsInLayers(gCustomDialogSpriteLayerP, gOptionsDialogPane4LayerP);
                    gFullScreenWindowPrefs = gFullScreenWindow;
                    WritePreferencesFile();
                    SaveExtrasSettings();
                    ExtrasSettingsChanged();
                }
                SetMusicVolume(GetMusicVolume());
                AnimateOneFrame(gSpriteWorldP);
                done = true;
            }
        }
        gCustomDialogIsVisible = false;
        MMRestoreCursorState();
    }

    gSpriteWorldP->spriteOffsetH = saveOffsetH;
    gSpriteWorldP->spriteOffsetV = saveOffsetV;
    gSpriteWorldP->zoomH = saveZoomH;
    gSpriteWorldP->zoomV = saveZoomV;
    SWSetSpriteLayerVisible(gFadeToBlackSpriteLayerP, fadeLayerWasVisible);
    SWDisposeAllSpritesInLayer(gCustomDialogSpriteLayerP);
    SWDisposeAllSpritesInLayer(gOptionsDialogPane1LayerP);
    SWDisposeAllSpritesInLayer(gOptionsDialogPane2LayerP);
    SWDisposeAllSpritesInLayer(gOptionsDialogPane3LayerP);
    SWDisposeAllSpritesInLayer(gOptionsDialogPane4LayerP);
    SWSetSpriteLayerVisible(gOptionsDialogPane3LayerP, false);
    SWSetSpriteLayerVisible(gOptionsDialogPane4LayerP, false);
    SWSetTileChangeProc(gSpriteWorldP, saveTileChangeProc);
    SWUpdateSpriteWorld(gSpriteWorldP, true);
    if (button == kOptionsCancel) {
        // Undo settings that were applied live.
        if (musicWasOn != gMusicIsOn)
            ToggleMusic();
        if (wasFullscreenWindow != gFullScreenWindow)
            ToggleFullscreen();
        if (wasStretched != gStretchToFit)
            SetStretchToFit(wasStretched);
#ifdef MM_PS3
        PS3ExtrasTVSizeCancelled();
        LoadExtrasSettings();       // the picture size changed live; the rest wasn't saved
#endif
    }
    SetMMEventCallback(NULL);
}

/// OK/Cancel buttons and the two tabs.
void AddStandardItemsToOptionsDialog(SpriteLayerPtr layerP, SpritePtr dialogP)
{
    float scaleH, scaleV;
    short h, v, dialogWidth, pane;

    GetDialogScale(&scaleH, &scaleV);
    dialogWidth = (short)SWGetSpriteWidth(dialogP);
    v = (short)(((short)SWGetSpriteHeight(dialogP) - (GetButtonHeight(0xda) + 19) * scaleV) +
                (short)SWGetSpriteVertLoc(dialogP));
    h = (short)((dialogWidth - (GetButtonWidth(0xda) + 19) * scaleH) + (short)SWGetSpriteHorizLoc(dialogP));
    AddButtonToDialog(0xda, 0, layerP, h, v);
    AddButtonToDialog(0xda, 1, layerP, (short)(h - (GetButtonWidth(0xda) + 4) * scaleH), v);

    v = (short)(SWGetSpriteVertLoc(dialogP) - 20.0f);
    gControlsTabSpriteP = AddButtonToDialog(0xe0, 0, layerP, 0, v);
    gOptionsTabSpriteP = AddButtonToDialog(0xdf, 0, layerP, 0, v);
    gFunTabSpriteP = AddButtonToDialog(0xe3, 0, layerP, 0, v);
    gExtrasTabSpriteP = AddButtonToDialog(0xe4, 0, layerP, 0, v);
        // four tabs side by side, centred over the dialog
    h = (short)(dialogWidth / 2 + SWGetSpriteHorizLoc(dialogP) - SWGetSpriteWidth(gControlsTabSpriteP) * 2.0f);
    for (pane = 1; pane <= 4; pane++) {
        SpritePtr tabP = OptionsPaneTab(pane);
        if (tabP == NULL)
            continue;
        SWSetSpriteLocation(tabP, h, v);
        SetButtonPushedOffset(tabP, 0, 2);
        if (gCurOptionsDialogPane == pane)
            SetButtonDimmedMode(tabP, true);
        h = (short)(h + SWGetSpriteWidth(tabP));
    }
}

void AddOptionsPaneTitle(SpriteLayerPtr layerP, SWRect *dialogRectP, const char *title, float scale)
{
    SpritePtr textP = NewBitmapTextSprite(2000);

    if (textP != NULL) {
        SWSetSpriteScaledFactor(textP, scale * 1.2f, scale * 1.2f);
        SetBMTextSpriteText(textP, title);
        SWAddSprite(layerP, textP);
        SWCenterSpriteInRect(textP, dialogRectP);
        SWSetSpriteLocation(textP, SWGetSpriteHorizLoc(textP), dialogRectP->top + scale * 70.0f);
    }
}

void AddOptionsItemsToOptionsDialog(SpriteLayerPtr layerP, SWRect *dialogRectP)
{
    float scale = gSpriteWorldP->designWidth / 1280.0f;
    float checkboxScale = scale * 1.05f;
    short h, v;
    SpritePtr textP;

    h = (short)(scale * 90.0f + dialogRectP->left);
    v = (short)(scale * 180.0f + dialogRectP->top);
    AddOptionsPaneTitle(layerP, dialogRectP, "Options", scale);

    AddCheckboxToOptionsDialog("Music (Command-M)", kOptionsMusic, &gMusicIsOn, layerP, checkboxScale, h, v);
    v = (short)(scale * 60.0f + v);
    AddCheckboxToOptionsDialog("Sound effects", kOptionsSound, &gSoundIsTurnedOn, layerP, checkboxScale, h, v);
    v = (short)(scale * 60.0f + v);
    AddCheckboxToOptionsDialog("Fullscreen", kOptionsFullscreen, &gFullScreenWindow, layerP, checkboxScale, h, v);
    v = (short)(scale * 60.0f + v);
    AddCheckboxToOptionsDialog("Stretch to fit", kOptionsStretch, &gStretchToFit, layerP, checkboxScale, h, v);
    textP = NewBitmapTextSprite((short)(scale * 650.0f));
    if (textP != NULL) {
        SWSetSpriteScaledFactor(textP, scale * 0.49f, scale * 0.49f);
        SetBMTextSpriteText(textP, "TIP: To resize the window, drag the lower-right corner.");
        SWAddSprite(layerP, textP);
        SWSetSpriteLocation(textP, h, (short)(scale * 60.0f + (short)(scale * 60.0f + v)));
    }

    h = (short)(scale * 450.0f + h);
    v = (short)(scale * 180.0f + dialogRectP->top);
    AddCheckboxToOptionsDialog("Softer music", kOptionsSofterMusic, &gSofterMusic, layerP, checkboxScale, h, v);
    v = (short)(scale * 60.0f + v);
    AddCheckboxToOptionsDialog("Limit Fullscreen Resolution", 10, &gLimitFullscreenResolution, layerP,
                               checkboxScale, h, v);
    v = (short)(scale * 60.0f + v);
    AddCheckboxToOptionsDialog("Create replays of each game", 11, &gRecordMovies, layerP, checkboxScale, h, v);
    AddCheckboxToOptionsDialog("Family friendly (no blood)", 12, &gFamilyFriendly, layerP, checkboxScale, h,
                               (short)(scale * 60.0f + v));
}

SpritePtr AddCheckboxToOptionsDialog(const char *text, short buttonIndex, Boolean *dataP, SpriteLayerPtr layerP,
                                     float scale, short h, short v)
{
    SpritePtr checkboxP = NewCustomCheckbox(text, scale, buttonIndex);

    if (checkboxP != NULL) {
        SWAddSprite(layerP, checkboxP);
        SWSetSpriteLocation(checkboxP, h, v);
        LinkCheckboxWithData(checkboxP, dataP);
    }
    return checkboxP;
}

void AddControlsItemsToOptionsDialog(SpriteLayerPtr layerP, SWRect *dialogRectP)
{
    static const struct { const char *label; short key; } leftColumn[] = {
        { "Left:", kKeyLeft }, { "Right:", kKeyRight }, { "Up:", kKeyUp }, { "Down:", kKeyDown },
        { "Jump:", kKeyJump }
    }, rightColumn[] = {
        { "Jump left:", kKeyJumpLeft }, { "Jump right:", kKeyJumpRight }, { "Action:", kKeyAction },
        { "Map:", kKeyMap }
    };
    float scale = gSpriteWorldP->designWidth / 1280.0f;
    short h, v, i, key;

    h = (short)(scale * 220.0f + dialogRectP->left);
    v = (short)(scale * 165.0f + dialogRectP->top);
    AddOptionsPaneTitle(layerP, dialogRectP, "Controls", scale);

    // Button indexes 0xd-0x15 follow the label order.
    for (i = 0; i < (short)SDL_arraysize(leftColumn); i++) {
        key = leftColumn[i].key;
        if (i > 0)
            v = (short)(scale * 60.0f + v);
        AddKeyInputBoxToOptionsDialog(leftColumn[i].label, 0xd + i, &gKeyPrefs.primaryKeys[key],
                                      &gKeyPrefs.secondaryKeys[key], gKeyPrefs.gamepadNames[key], layerP, h, v);
    }

    h = (short)(scale * 560.0f + h);
    v = (short)(scale * 165.0f + dialogRectP->top);
    for (i = 0; i < (short)SDL_arraysize(rightColumn); i++) {
        key = rightColumn[i].key;
        if (i > 0)
            v = (short)(scale * 60.0f + v);
        AddKeyInputBoxToOptionsDialog(rightColumn[i].label, 0x12 + i, &gKeyPrefs.primaryKeys[key],
                                      &gKeyPrefs.secondaryKeys[key], gKeyPrefs.gamepadNames[key], layerP, h, v);
    }

    h = (short)(scale * 110.0f + dialogRectP->left);
    v = (short)(scale * 90.0f + (short)(scale * 60.0f + v));
    AddCheckboxToOptionsDialog("Enable arrow keys", 0x16, &gArrowKeysEnabled, layerP, scale * 1.1f, h, v);
    gGamepadCheckboxSpriteP = AddCheckboxToOptionsDialog("Enable GamePad", kOptionsGamepad, &gUseGamePad, layerP,
                                                         scale * 1.1f, h, (short)(scale * 40.0f + v));
}

SpritePtr AddKeyInputBoxToOptionsDialog(const char *label, short buttonIndex, long *scancodeP, long *keycodeP,
                                        char *nameP, SpriteLayerPtr layerP, short h, short v)
{
    SpritePtr boxP, textP;
    float width;

    boxP = NewKeyInputBox("ignored", (gSpriteWorldP->designWidth / 1280.0f) * 0.8f, buttonIndex);
    if (boxP != NULL) {
        SWAddSprite(layerP, boxP);
        SWSetSpriteLocation(boxP, h, v);
        LinkKeyInputBoxWithData(boxP, scancodeP, keycodeP, nameP);
    }
    textP = NewBitmapTextSprite(2000);
    if (textP != NULL && boxP != NULL) {
        SWSetSpriteScaledHeight(textP, (short)(SWGetSpriteHeight(boxP) * 0.63f));
        SWSetSpriteScaledWidthFactor(textP, textP->drawData->scaledHeightFactor);
        SetBMTextSpriteText(textP, label);
        SWSetSpriteColor(textP, boxP->drawData->colorRed, boxP->drawData->colorGreen,
                         boxP->drawData->colorBlue);
        SWAddSprite(layerP, textP);
        width = GetWidthOfBMText(textP, label);
        SWSetSpriteLocation(textP, (h - width) - 20.0f, (float)(v + 3));
    }
    return boxP;
}

// ---------------------------------------------------------------------------
// Native dialogs (file pickers etc.) need a window, not fullscreen.

void PrepareGameForDialogBox(long gameMode)
{
    if (gDialogBoxIsOpen)
        return;

    wasFullscreen = gFullScreenWindow;
    if (gFullScreenWindow) {
        saveTileBackgroundP = gSpriteWorldP->backgroundTileSpriteP;
        gSpriteWorldP->backgroundTileSpriteP = NULL;
        ToggleFullscreen();
        ProcessSDLEvents();
        SWUpdateSpriteWorld(gSpriteWorldP, true);
    }
    gSaveCurrentGameMode = gCurrentGameMode;
    gDialogBoxIsOpen = true;
    gCurrentGameMode = gameMode;
    MMFlushEvents();
    MMSaveCursorState();
    MMSetCursorForSystem();
    SWUpdateSpriteWorld(gSpriteWorldP, true);

    // Wait for the mouse button and movement keys to be released.
    while (IsMouseDown() || gKeys[kKeyRight] || gKeys[kKeyLeft] || gKeys[kKeyUp] || gKeys[kKeyDown] ||
           gKeys[kKeyAction] || gKeys[kKeyJump])
        ProcessSDLEvents();
}

void RestoreGameAfterDialogBox(void)
{
    if (!gDialogBoxIsOpen)
        return;

    SDL_RaiseWindow(gSpriteWorldP->mainWindow);
    if (wasFullscreen) {
        ToggleFullscreen();
        SWUpdateWindowSize(gSpriteWorldP);
        SWUpdateWindowSize(gStatsSpriteWorldP);
        if (gSpriteWorldP->backgroundTileSpriteP != NULL)
            SWDisposeSprite(&gSpriteWorldP->backgroundTileSpriteP);
        gSpriteWorldP->backgroundTileSpriteP = saveTileBackgroundP;
        SWUpdateSpriteWorld(gSpriteWorldP, true);
    }
    if (!IsMoviePlaying())
        ResetKeys();
    gCurrentGameMode = gSaveCurrentGameMode;
    gDialogBoxIsOpen = false;
    MMRestoreCursorState();
    MMFlushEvents();
}
