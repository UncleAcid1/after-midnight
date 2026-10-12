/*
 * HighScores.c — the high score tables and their screen.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources & Headers/High Scores.c").
 * There is one table of 10 scores per mansion (11) and difficulty (3), saved as
 * "Midnight Mansion HD High Scores" in the app support folder (big-endian, like
 * the Mac version's file).
 *
 * Not carried over: submitting scores to the ActionSoft website (ASWHighScores.c,
 * a service that no longer exists) and the pirated-license check that came with it.
 */
#include "MMGame.h"

#define kHighScoresFileVersion  100
#define kHighScoresFileName     "Midnight Mansion HD High Scores"

#define kLeftArrowButton        0xd6
#define kDoneButton             0xd7
#define kRightArrowButton       0xd8

HighScoresFileRec gHighScores;
static Boolean exitToMapScreen;
short gCurrentHighScoresScreenIndex;
static short gFirstHighScoresIndex;
static short gLastHighScoresIndex;
SpritePtr gLeftArrowButtonSpriteP;   // shared with the help screen
SpritePtr gRightArrowButtonSpriteP;

static const char *defaultNameArray[12] = {
    "Headless Joe", "Tiny Tim", "Fearless Sam", "Silly Willy", "Nervous Skeleton", "Grumpy Dwarf",
    "Mighty Mike", "Hungry Ogre", "Timid George", "Sir Runsalot", "ERROR", "ERROR 2"
};

/// Fills every table with the default names and scores.
void InitializeHighScores(void)
{
    short mansion, difficulty, rank, nameIndex;
    HighScoreRec *scoreP;

    puts("Initializing High Scores, because file can't be found.");
    for (mansion = 0; mansion < kNumHighScoreMansions; mansion++) {
        for (difficulty = 0; difficulty < 3; difficulty++) {
            for (rank = 0; rank < 10; rank++) {
                scoreP = &gHighScores.scores[mansion][difficulty][rank];
                    // rotate the names so each difficulty's list looks different
                if (difficulty == 0)
                    nameIndex = rank + 4;
                else if (difficulty == 2)
                    nameIndex = rank + 8;
                else
                    nameIndex = rank;
                while (nameIndex > 9)
                    nameIndex -= 10;
                SWCopyCStringToPascalString(defaultNameArray[nameIndex], scoreP->name);
                scoreP->score = 5000 - rank * 500;
                scoreP->time = 1200 - rank * 60;
                scoreP->percent = 50 - rank * 5;
                scoreP->wonMansion = false;
                scoreP->unused35 = false;
                scoreP->unused36 = -1;
                scoreP->unused38 = -1;
                scoreP->unused3c = -1;
            }
        }
    }
}

void LoadHighScores(void)
{
    int err = LoadHighScoresFromFile();
    if (err != 0) {
        InitializeHighScores();
        if (err == -1)
            WriteHighScoresToFile();
    }
}

int WriteHighScoresToFile(void)
{
    char path[1024];
    SDL_RWops *file;

    snprintf(path, sizeof(path), "%s", GetAppSupportPath(kHighScoresFileName));
    file = SDL_RWFromFile(path, "wb");
    if (file == NULL)
        return -1;
    ClearFileStickyError();
    gHighScores.version = kHighScoresFileVersion;
    SwapEndiansForHighScoreData();
    MyWriteDataToFile(file, sizeof(HighScoresFileRec), &gHighScores);
    SwapEndiansForHighScoreData();
    SDL_RWclose(file);
    SetFileCreatorAndTypeForPath(path, 'MANS', 'HIGH');
    return GetFileStickyError();
}

int LoadHighScoresFromFile(void)
{
    char path[1024];
    SDL_RWops *file;

    snprintf(path, sizeof(path), "%s", GetAppSupportPath(kHighScoresFileName));
    file = SDL_RWFromFile(path, "rb");
    if (file == NULL)
        return -1;
    printf("Loading high scores from path: %s\n", path);
    ClearFileStickyError();
    MyReadDataFromFile(file, sizeof(HighScoresFileRec), &gHighScores);
    SwapEndiansForHighScoreData();
    SDL_RWclose(file);
    return 0;
}

/// Converts between the file's big-endian numbers and native ones (both ways).
void SwapEndiansForHighScoreData(void)
{
    short mansion, difficulty, rank;
    HighScoreRec *scoreP;

    gHighScores.version = (Sint16)SDL_SwapBE16((Uint16)gHighScores.version);
    for (mansion = 0; mansion < kNumHighScoreMansions; mansion++) {
        for (difficulty = 0; difficulty < 3; difficulty++) {
            for (rank = 0; rank < 10; rank++) {
                scoreP = &gHighScores.scores[mansion][difficulty][rank];
                scoreP->score = (Sint32)SDL_SwapBE32((Uint32)scoreP->score);
                scoreP->time = (Sint32)SDL_SwapBE32((Uint32)scoreP->time);
                scoreP->percent = (Sint32)SDL_SwapBE32((Uint32)scoreP->percent);
            }
        }
    }
}

static SpritePtr AddArrowButton(short buttonID, short pictID, short h, Boolean dimmed, Boolean flip)
{
    SpritePtr buttonP = NewButtonSprite(pictID, buttonID);
    if (buttonP == NULL)
        return NULL;
    SWAddSprite(gTopSpriteLayerP, buttonP);
    SetUpButtonSprite(buttonP, 0, 1, 2, 3);
    InstallButtonProc(buttonP, MMStandardButtonProc);
    if (flip)
        SWSetSpriteFlipX(buttonP, true);
    SWSetSpriteLocation(buttonP, h, 10);
    if (dimmed)
        SetButtonDimmedMode(buttonP, true);
    return buttonP;
}

/// Shows the high scores of mansion screenIndex (if showIndex, else the first one).
void SetUpHighScoresScreen(short screenIndex, Boolean showIndex, Boolean returnToMapScreen)
{
    SpritePtr doneButtonP;
    short h;

    SW_ASSERT(screenIndex >= gFirstHighScoresIndex);
    SW_ASSERT(screenIndex <= 10);
    gCurrentlyInMansion = false;
    exitToMapScreen = returnToMapScreen;
    gFirstHighScoresIndex = 0;
    gLastHighScoresIndex = gCanPlayBothEpisodes ? 10 : 4;
    gCurrentHighScoresScreenIndex = showIndex ? screenIndex : gFirstHighScoresIndex;

    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
    gCurrentGameMode = kNoGameMode;
    GiveOSTime();

        // < Done >
    h = 434;
    gLeftArrowButtonSpriteP = AddArrowButton(kLeftArrowButton, 0xd6, h,
                                             gCurrentHighScoresScreenIndex == gFirstHighScoresIndex, false);
    if (gLeftArrowButtonSpriteP != NULL)
        h = (short)(int)(SWGetSpriteWidth(gLeftArrowButtonSpriteP) + 10.0f + 434.0f);

    doneButtonP = NewButtonSprite(kDoneButton, 0xd7);
    if (doneButtonP != NULL) {
        SWAddSprite(gTopSpriteLayerP, doneButtonP);
        SetUpButtonSprite(doneButtonP, 0, 1, 2, 0);
        InstallButtonProc(doneButtonP, MMStandardButtonProc);
        SWSetSpriteLocation(doneButtonP, h, 10);
        h = (short)(int)(SWGetSpriteWidth(doneButtonP) + 10.0f + (float)h);
    }

    gRightArrowButtonSpriteP = AddArrowButton(kRightArrowButton, 0xd6, h,
                                              gCurrentHighScoresScreenIndex == gLastHighScoresIndex, true);

    MMShieldCursor();
    MMSetCursorForGame();
    DrawContentsForHighScoresScreen(gCurrentHighScoresScreenIndex);
    gCurrentGameMode = kHighScoresScreenMode;
    UpdateMenuBar();
    gSpriteWorldIsValid = true;
    GiveOSTime();
    DoGameFadeToNormal(30);
}

void HandleHighScoresScreenCallback(void)
{
    SWPoint mouse;
    Boolean mouseDown;
    short buttonID;

    SWProcessSpriteWorld(gSpriteWorldP);
    SWAnimateSpriteWorld(gSpriteWorldP);
    if (!gSpriteWorldP->frameHasOccurred)
        return;

    mouseDown = MMGetMouse(&mouse);
    SWConvertCoordinates(gSpriteWorldP, &mouse);
    buttonID = ProcessButtonSprites(gTopSpriteLayerP, gTopSpriteLayerP, mouse, mouseDown, false, false);
    if (buttonID == -1)
        return;

    AnimateOneFrame(gSpriteWorldP);
    if (buttonID == kLeftArrowButton) {
        gCurrentHighScoresScreenIndex--;
        SetButtonDimmedMode(gRightArrowButtonSpriteP, false);
        if (gCurrentHighScoresScreenIndex == gFirstHighScoresIndex)
            SetButtonDimmedMode(gLeftArrowButtonSpriteP, true);
        DrawContentsForHighScoresScreen(gCurrentHighScoresScreenIndex);
    } else if (buttonID == kRightArrowButton) {
        gCurrentHighScoresScreenIndex++;
        SetButtonDimmedMode(gLeftArrowButtonSpriteP, false);
        if (gCurrentHighScoresScreenIndex == gLastHighScoresIndex)
            SetButtonDimmedMode(gRightArrowButtonSpriteP, true);
        DrawContentsForHighScoresScreen(gCurrentHighScoresScreenIndex);
    } else if (buttonID == kDoneButton) {
        DoGameFadeToBlack(30, false);
        ExitHighScoresScreen();
        if (!exitToMapScreen)
            SetUpTitleScreen();
        else
            SetUpSelectMansionScreen(gCurrentMansionNumber < 5 ? 1 : 2);
    }
}

/// Draws the table for one mansion, then captures it all as one background sprite.
void DrawContentsForHighScoresScreen(short mansionIndex)
{
    static SWRGBColor kWhite = { 0xffff, 0xffff, 0xffff };
    static SWRGBColor kYellow = { 0xffff, 0xffff, 0 };
    static SWRGBColor kPaleRed = { 0xffff, 0x51eb, 0x51eb };
    MMTextStyleRec style;
    SpritePtr spriteP;
    SWRect rect;
    HighScoreRec *scoreP;
    char name[255], text[255];
    short difficulty, column, row, rank, v;
    SWError err;

    SWDisposeAllSpritesInLayer(gTextSpriteLayerP);
    SWDisposeAllSpritesInLayer(gBottomSpriteLayerP);
    err = LoadAndInstallBackgroundSprite(&spriteP, GetScreensFolderPath(), "highscores.jpg", -1, false);
    if (err != kNoError)
        MM_FATAL(err);
    SWSetSpriteScaledSize(spriteP, gSpriteWorldP->designWidth, gSpriteWorldP->designHeight);

        // title: the mansion's name
    memset(&style, 0, sizeof(style));
    SWCopyCStringToPascalString("BlackChancery", style.fontName);
    style.size = 38;
    style.unused = true;
    style.dropShadow = false;
    style.justification = 2;
    SWSetRect(&rect, 0, 0, 440, 34);
    MyDrawPascalTextJustified(GetMansionNameFromIndex(mansionIndex), &style, &rect, &kWhite);

    SWCopyCStringToPascalString("Arial", style.fontName);
    style.justification = 0;
    style.style = 0;
    style.size = 12;
    style.unused = true;
    style.dropShadow = false;
    SW_ASSERT(mansionIndex >= 0 && mansionIndex < kNumHighScoreMansions);

        // three difficulties, each in two columns of five
    for (difficulty = 0; difficulty < 3; difficulty++) {
        rank = 0;
        for (column = 0; column < 2; column++) {
            for (row = 1; row < 6; row++) {
                v = row * 24 + difficulty * 144 + 52;
                scoreP = &gHighScores.scores[mansionIndex][difficulty][rank];

                SWCopyPascalStringToCString(scoreP->name, name);
                snprintf(text, sizeof(text), " %d. %s", rank + 1, name);
                MyDrawText(text, &style, (short)(column * 320 + 2), v, scoreP->wonMansion ? &kWhite : &kPaleRed);

                snprintf(text, sizeof(text), "%d", (int)scoreP->score);
                MyDrawText(text, &style, (short)(column * 320 + 136), v, &kYellow);

                snprintf(text, sizeof(text), "%d:%s%d", (int)(scoreP->time / 60),
                         scoreP->time % 60 < 10 ? "0" : "", (int)(scoreP->time % 60));
                MyDrawText(text, &style, (short)(column * 320 + 200), v, &kYellow);

                snprintf(text, sizeof(text), "%d%%", (int)scoreP->percent);
                MyDrawText(text, &style, (short)(column * 320 + 256), v, &kYellow);
                rank++;
            }
        }
    }

    MMRenderSceneForWorkAreaSprite(gSpriteWorldP);
        // (the original autoscaled a 640x480 rect here, but captured backgroundTileRect)
    err = SWCreateSpriteFromWorkArea(gSpriteWorldP, &spriteP, NULL, &gSpriteWorldP->backgroundTileRect);
    if (err == kNoError) {
        SWDisposeAllSpritesInLayer(gTextSpriteLayerP);
        SWAddSprite(gBottomSpriteLayerP, spriteP);
        SWUpdateSpriteWorld(gSpriteWorldP, true);
    }
}

void ExitHighScoresScreen(void)
{
    gCurrentGameMode = kNoGameMode;
    UpdateMenuBar();
    gSpriteWorldIsValid = false;
    MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
}

/**
 * Called after a mansion ends: if score beats an entry of the mansion's table for
 * this difficulty, asks for the player's name and inserts it. Ties go to the
 * higher percentage, then the shorter time. Returns true if a score was added.
 */
Boolean CheckForHighScore(short mansionIndex, short difficulty, long score, short percent, Boolean wonMansion)
{
    HighScoreRec *scoreP;
    unsigned char newName[256];
    Boolean wasFullScreen = false;
    short rank, dialogResult;

    for (rank = 0; rank < 10; rank++) {
        scoreP = &gHighScores.scores[mansionIndex][difficulty][rank];
        if (scoreP->score < score ||
            (scoreP->score == score && scoreP->percent < percent) ||
            (scoreP->score == score && scoreP->percent == percent && gTimeSpentInMansion < scoreP->time)) {

            if (wonMansion && gFullScreenWindow) {
                wasFullScreen = gFullScreenWindow;
                ToggleFullscreen();
                DoBeatTheMansionScreen(true);
            }
            dialogResult = MMDoEnterNameDialog(gDefaultHighScoreName, newName, wonMansion);
            if (dialogResult != -1) {
                SWCopyPascalString(newName, gDefaultHighScoreName);
                WriteProfileToDisk();
            }
            if (wasFullScreen && (dialogResult != 1 || !wonMansion)) {
                DoGameFadeToBlack(true, false);
                ToggleFullscreen();
            }
            if (dialogResult == -1)
                return false;

            gCurrentGameMode = kNoGameMode;
            ShiftScoresLowerInList(mansionIndex, difficulty, rank);
            memset(scoreP->name, 0, sizeof(scoreP->name));
            if (newName[0] > sizeof(scoreP->name) - 1)
                newName[0] = sizeof(scoreP->name) - 1;
            memcpy(scoreP->name, newName, newName[0] + 1);
            scoreP->score = (Sint32)score;
            scoreP->time = (Sint32)gTimeSpentInMansion;
            scoreP->percent = percent;
            scoreP->wonMansion = wonMansion;
            WriteHighScoresToFile();
            return true;
        }
    }
    WriteHighScoresToFile();
    return false;
}

/// Moves entries rank..8 of a table down one place (the last one drops off).
void ShiftScoresLowerInList(short mansionIndex, short difficulty, short rank)
{
    short i;
    SW_ASSERT(rank >= 0);
    for (i = 8; i >= rank; i--)
        gHighScores.scores[mansionIndex][difficulty][i + 1] = gHighScores.scores[mansionIndex][difficulty][i];
}
