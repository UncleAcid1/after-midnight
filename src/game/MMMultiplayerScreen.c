/*
 * MMMultiplayerScreen.c — the Multiplayer screen, added by this version.
 *
 * Choosing Multiplayer on the title screen lifts the view off the mansion into the night sky
 * (the reverse of the opening scroll): the mansion, hill and fog sink away while the moon and
 * clouds drift a little, and small birds now and then cross the stars from left to right.
 *
 * On the left are the kinds of game (Relay Co-op, Arcade Twin, Parallel Phantoms); on the
 * right a wood-panelled window lists the games that can be joined — a new one on this
 * computer, and the ones being hosted on the local network — and, once one is chosen, who
 * plays: each player's profile, character and controls.
 *
 * Everything here is drawn over the title screen's sprites in the world's post-draw callback,
 * in the title screen's 1280x960 design coordinates. Mouse, keyboard (arrows, Return,
 * Escape) and controllers (d-pad, A, B; A on a controller that isn't playing yet adds it as
 * a player) all work.
 */

#include <dirent.h>
#include <sys/stat.h>
#include "MMGame.h"
#include "MMProtos.h"
#include "BKGL.h"
#include "BlitKernel.h"
#include "MMFont.h"

#define kSkyLift        2000.0f     ///< how far the camera rises (design units)
#define kLiftMs         1700        ///< and how long it takes
#define kMaxScrollers   256
#define kMaxRows        24
#define kMaxFound       8

enum { kPhaseRising, kPhaseMenu, kPhaseSinking };
enum { kModeRelay, kModeTwin, kModePhantoms, kNumModes };
enum { kViewGames, kViewLobby, kViewOnline, kViewFriends };

/// What each row on the right does.
enum {
    kRowHeader, kRowLocalGame, kRowFoundGame, kRowHost, kRowJoinAddress, kRowSearching,
    kRowPlayer, kRowAddPlayer, kRowRemovePlayer, kRowChooseMansion, kRowCustomMansion, kRowBackToGames,
    kRowGoOnline, kRowOnlineGame, kRowHostPublic, kRowHostPrivate, kRowJoinCode, kRowGoFriends, kRowLobbyAddress,
    kRowAddFriend, kRowFriend, kRowRecent, kRowBackToOnline, kRowInfo, kRowContinueSave
};

typedef struct ModeInfo {
    const char *title;
    const char *tagline;
    const char *details;
    const char *badges;
    Boolean     network;
} ModeInfo;

static const ModeInfo kModes[kNumModes] = {
    { "Co-op", "Take turns, share the lives",
      "When the player whose turn it is dies, the next one comes in. Doors, keys, treasure and lives are shared.",
      "2-4 players  |  This computer or Wi-Fi", true },
    { "Arcade Split Screen", "Everyone at once, side by side",
      "Everyone plays at once, each in their own view. One mansion: what one player opens or takes, all share.",
      "2-4 players  |  Split screen", false },
    { "Ghost Race", "A race through twin mansions",
      "Everyone gets their own copy of the mansion; the others show as ghosts. Fastest out wins; each life lost "
      "adds 15 seconds.",
      "2-4 players  |  Split screen or Wi-Fi", true },
};

typedef struct Row {
    int     kind;
    int     arg;            ///< player index, found game index
    char    text[160];
    char    detail[160];
    float   top, bottom;
} Row;

typedef struct Scroller {
    SpritePtr sprite;
    float     factor;
} Scroller;

static struct {
    Boolean     active;
    int         phase;
    float       lifted, speed;
    Scroller    scrollers[kMaxScrollers];
    int         numScrollers;
    CallBackPtr savedPostDraw;
    MMFont     *titleFont, *font;
    GLuint      woodTexture;
    int         woodW, woodH;
    GLuint      modeTitles;     ///< the game types' names, lettered like the menu buttons
    Boolean     pointer;        ///< the mouse was used last (not the keys or a controller)
    int         hoverCard;      ///< the game type under the pointer, or -1

    int         mode;
    int         view;
    int         column;         ///< 0 the kinds of game, 1 the window on the right, 2 Back
    int         row;            ///< focused row on the right
    int         field;          ///< lobby: 0 profile, 1 character, 2 controls
    Row         rows[kMaxRows];
    int         numRows;
    NetFoundGame found[kMaxFound];
    int         numFound;
    OnlineGame  online[kMaxOnlineGames];
    int         numOnline;
    OnlineFriend friends[kMaxOnlineFriends];
    int         numFriends;
    OnlineFriend recent[8];
    int         numRecent;
    MultiSaveInfo saves[3];     ///< the lobby's players' saved games (Co-op)
    int         numSaves;
    char        savesKey[400];
    Uint32      savesCheckedAt;

    int         numPlayers;
    char        profiles[kMaxPlayers][64];
    int         characters[kMaxPlayers];
    int         inputs[kMaxPlayers];

    SWPoint     mouse;
    Boolean     mouseWasDown, mouseMoved;
    int         keyPresses;     ///< kPad... bits from the keyboard
    Uint32      nextBird;
    Uint32      phaseStart;
    char        message[200];   ///< a short note under the window ("Plug in a controller...")
    Uint32      messageUntil;
} gMP;

static void MultiplayerScreenPostDraw(SpriteWorldPtr w);
static void BuildRows(void);

// ---------------------------------------------------------------------------
// Who played last time ("Multiplayer.txt" next to the preferences)

static Boolean ProfileExists(const char *name)
{
    struct stat info;
    return name[0] != 0 && stat(GetPlayerProfilePath(name), &info) == 0;
}

static Boolean ProfileInUse(const char *name, int except)
{
    int i;
    for (i = 0; i < gMP.numPlayers; i++)
        if (i != except && strcmp(gMP.profiles[i], name) == 0)
            return true;
    return false;
}

/// Gives players without a profile the first one on this computer nobody else has.
static void FillMissingProfiles(void)
{
    char names[64][64], folder[1024];
    struct dirent *entry;
    DIR *dir;
    int count = 0, i, j, p;

    snprintf(folder, sizeof(folder), "%s", GetPlayerProfilePath(NULL));
    dir = opendir(folder);
    if (dir == NULL)
        return;
    while ((entry = readdir(dir)) != NULL && count < 64)
        if (entry->d_name[0] != '.')
            snprintf(names[count++], sizeof(names[0]), "%.63s", entry->d_name);
    closedir(dir);
    for (i = 0; i < count; i++)             // alphabetical
        for (j = i + 1; j < count; j++)
            if (SDL_strcasecmp(names[j], names[i]) < 0) {
                char t[64];
                memcpy(t, names[i], 64);
                memcpy(names[i], names[j], 64);
                memcpy(names[j], t, 64);
            }
    for (p = 0; p < gMP.numPlayers; p++) {
        if (ProfileExists(gMP.profiles[p]) && !ProfileInUse(gMP.profiles[p], p))
            continue;
        gMP.profiles[p][0] = 0;
        for (i = 0; i < count; i++)
            if (!ProfileInUse(names[i], p)) {
                snprintf(gMP.profiles[p], sizeof(gMP.profiles[p]), "%s", names[i]);
                break;
            }
    }
}

static void LoadLobby(void)
{
    FILE *file = fopen(GetAppSupportPath("Multiplayer.txt"), "r");
    char line[200], key[32], value[160];
    int p;

    if (file == NULL)
        return;
    while (fgets(line, sizeof(line), file) != NULL) {
        value[0] = 0;
        if (sscanf(line, " %31[a-z_0-9] = %159[^\n]", key, value) < 1)
            continue;
        if (strcmp(key, "mode") == 0)
            gMP.mode = SDL_clamp(atoi(value), 0, kNumModes - 1);
        else if (strcmp(key, "players") == 0)
            gMP.numPlayers = SDL_clamp(atoi(value), 2, kMaxPlayers);
        else if (sscanf(key, "profile_%d", &p) == 1 && p >= 1 && p <= kMaxPlayers)
            snprintf(gMP.profiles[p - 1], sizeof(gMP.profiles[0]), "%s", value);
        else if (sscanf(key, "character_%d", &p) == 1 && p >= 1 && p <= kMaxPlayers)
            gMP.characters[p - 1] = SDL_clamp(atoi(value), 0, kNumCharacters - 1);
        else if (sscanf(key, "controls_%d", &p) == 1 && p >= 1 && p <= kMaxPlayers)
            gMP.inputs[p - 1] = SDL_clamp(atoi(value), 0, kNumSplitInputs - 1);
    }
    fclose(file);
}

static void SaveLobby(void)
{
    FILE *file = fopen(GetAppSupportPath("Multiplayer.txt"), "w");
    int p;

    if (file == NULL)
        return;
    fprintf(file, "# Midnight Mansion HD: who played together last time\n");
    fprintf(file, "mode = %d\nplayers = %d\n", gMP.mode, gMP.numPlayers);
    for (p = 0; p < kMaxPlayers; p++)
        fprintf(file, "profile_%d = %s\ncharacter_%d = %d\ncontrols_%d = %d\n", p + 1, gMP.profiles[p], p + 1,
                gMP.characters[p], p + 1, gMP.inputs[p]);
    fclose(file);
}

// ---------------------------------------------------------------------------
// Coming and going

static void AddScroller(SpritePtr spriteP, float factor)
{
    if (gMP.numScrollers < kMaxScrollers) {
        gMP.scrollers[gMP.numScrollers].sprite = spriteP;
        gMP.scrollers[gMP.numScrollers].factor = factor;
        gMP.numScrollers++;
    }
}

/// Everything that moves as the view rises: what scrolled in the opening moves the same way,
/// except the moon and clouds, which stay in the sky.
static void CollectScrollers(void)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP;
    int numClouds = 0;

    gMP.numScrollers = 0;
    for (layerP = gSpriteWorldP->headSpriteLayerP; layerP != NULL; layerP = SWGetNextSpriteLayer(gSpriteWorldP, layerP))
        for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP)) {
            if (spriteP->miscFloat == 0.0f)
                continue;
                // the moon drifts up into the sky above the panels; the clouds settle along the
                // bottom, under them
            if (layerP == gExitSpriteLayerP) {                  // clouds
                float target = 790.0f + (float)(numClouds++ % 3) * 30.0f;
                AddScroller(spriteP, (target - SWGetSpriteVertLoc(spriteP)) / kSkyLift);
            }
            else if (spriteP->miscFloat < 0.9f)                 // the moon
                AddScroller(spriteP, -0.075f);
            else
                AddScroller(spriteP, 1.0f);
        }
}

static void Lift(float amount)
{
    int i;

    for (i = 0; i < gMP.numScrollers; i++)
        SWOffsetSprite(gMP.scrollers[i].sprite, 0, gMP.scrollers[i].factor * amount);
    gMP.lifted += amount;
}

static Boolean MPEventCallback(SDL_Event *event);

/// An image as a texture (its size into w and h when asked), or 0.
static GLuint LoadTexture(const char *path, int *w, int *h)
{
    SDL_Surface *surface = BKLoadSurface(path), *rgba;
    GLuint texture = 0;

    if (surface == NULL)
        return 0;
    rgba = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(surface);
    if (rgba == NULL)
        return 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, rgba->pitch / 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, rgba->w, rgba->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba->pixels);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    if (w != NULL)
        *w = rgba->w;
    if (h != NULL)
        *h = rgba->h;
    SDL_FreeSurface(rgba);
    return texture;
}

void EnterMultiplayerScreen(void)
{
    char name[256];
    int i;

    gMP.active = true;
    gMP.phase = kPhaseRising;
    gMP.phaseStart = SDL_GetTicks();
    gMP.lifted = 0;
    gMP.speed = 0;
    gMP.view = kViewGames;
    gMP.column = 0;
    gMP.row = 0;
    gMP.field = 0;
    gMP.message[0] = 0;
    CollectScrollers();
    TitleScreenFadeMenu(true);
    gMP.savedPostDraw = gSpriteWorldP->postDrawCallBack;
    SWSetPostDrawCallBack(gSpriteWorldP, MultiplayerScreenPostDraw);
    if (gMP.titleFont == NULL)
        gMP.titleFont = MMFontLoad(GetFontsFolderPath("BlackChancery.TTF"));
    if (gMP.font == NULL)
        gMP.font = MMFontLoad(GetFontsFolderPath("Action Man.ttf"));
    if (gMP.woodTexture == 0) {
        snprintf(name, sizeof(name), "%scustom dialog.png", GetScreensFolderPath());
        gMP.woodTexture = LoadTexture(name, &gMP.woodW, &gMP.woodH);
    }
    if (gMP.modeTitles == 0 && GetExtrasFilePath("Screens/multiplayer/mode_titles.png") != NULL)
        gMP.modeTitles = LoadTexture(GetExtrasFilePath("Screens/multiplayer/mode_titles.png"), NULL, NULL);
    gMP.pointer = !gControllerHidesCursor;
    gMP.hoverCard = -1;
        // who plays: this profile first, then whoever played last time
    if (gMP.numPlayers < 2) {
        gMP.numPlayers = 2;
        gMP.characters[0] = kCharacterJack;
        gMP.characters[1] = kCharacterHenry;
        gMP.characters[2] = kCharacterJack2;
        gMP.characters[3] = kCharacterHenry2;
        gMP.inputs[0] = kSplitInputKeyboard;
        gMP.inputs[1] = IsControllerConnected() ? kSplitInputPad1 : kSplitInputKeyboardLeft;
        gMP.inputs[2] = kSplitInputPad2;
        gMP.inputs[3] = kSplitInputPad3;
        LoadLobby();
    }
        // player 1 is whoever is signed in
    SWCopyPascalStringToCString((ConstStringPtr)gCurrentPlayerName, name);
    for (i = 1; i < gMP.numPlayers; i++)
        if (strcmp(gMP.profiles[i], name) == 0)
            snprintf(gMP.profiles[i], sizeof(gMP.profiles[i]), "%s", gMP.profiles[0]);
    snprintf(gMP.profiles[0], sizeof(gMP.profiles[0]), "%s", name);
    FillMissingProfiles();
    SetMMEventCallback(MPEventCallback);
    NetBrowseStart();
    gMP.nextBird = SDL_GetTicks() + 1200;
    gCurrentGameMode = kMultiplayerScreenMode;
    MMPlaySound(0x4b, 0, 1);        // the whoosh of the opening scroll
}

/// Back down to the mansion (the title screen).
static void LeaveToTitle(void)
{
    if (gMP.phase == kPhaseSinking)
        return;
    gMP.phase = kPhaseSinking;
    gMP.phaseStart = SDL_GetTicks();
    gMP.speed = 0;
    NetBrowseStop();
    OnlineDisconnect();
}

/// Stops drawing the screen (before something else takes the world over).
static void ShutDownScreen(void)
{
    if (gCurrentGameMode == kMultiplayerScreenMode)
        gCurrentGameMode = kTitleScreenMode;    // what's left is the title screen's world
    SWSetPostDrawCallBack(gSpriteWorldP, gMP.savedPostDraw);
    SetMMEventCallback(NULL);
    NetBrowseStop();
    OnlineDisconnect();
    gMP.active = false;
}

Boolean IsMultiplayerScreenActive(void)
{
    return gMP.active;
}

// ---------------------------------------------------------------------------
// Birds: small dark ravens crossing the sky from left to right

static void BirdMoveProc(SpritePtr spriteP)
{
    float x = SWGetSpriteHorizLoc(spriteP) + spriteP->miscFloat;
    float angle = (float)(spriteP->misc1.n++) * 0.05f;

    SWSetSpriteLocation(spriteP, x, (float)spriteP->userData + sinf(angle) * 6.0f);
    if (x > gSpriteWorldP->designWidth + 40)
        SWRemoveSpriteFromAnimation(spriteP, true);
}

static void SpawnBird(void)
{
    SpritePtr masterP = gSpriteArray[kBlueRavenSprite], birdP = NULL;
    float scale, shade, flockY;
    int flockSize, i;

    if (masterP == NULL)
        return;
    flockSize = GetRandom(1, 10) <= 7 ? 1 : GetRandom(2, 4);
        // mostly where the sky shows: above the panels, below them, now and then behind them
    switch (GetRandom(1, 6)) {
        case 1: case 2: case 3: case 4: flockY = GetRandomFloat(20.0f, 150.0f); break;
        case 5:                 flockY = GetRandomFloat(862.0f, 880.0f); break;
        default:                flockY = GetRandomFloat(160.0f, 780.0f); break;
    }
    scale = GetRandomFloat(0.35f, 0.75f);
    for (i = 0; i < flockSize; i++) {
        if (SWCloneSprite(masterP, &birdP, NULL, 0) != kNoError || birdP == NULL)
            return;
        SWAddSprite(gRearBackgroundPictureSpriteLayerP, birdP);
        SWSetSpriteScaledFactor(birdP, masterP->drawData->scaledWidthFactor * scale,
                                masterP->drawData->scaledHeightFactor * scale);
            // far away against the night: dark blue, a little see-through
        shade = 0.20f + scale * 0.25f;
        SWSetSpriteColor(birdP, (unsigned short)(0xffff * shade * 0.7f), (unsigned short)(0xffff * shade * 0.8f),
                         (unsigned short)(0xffff * shade * 1.2f > 0xffff ? 0xffff : 0xffff * shade * 1.2f));
        SWSetSpriteTranslucencyLevel(birdP, 0xd800);
        SWSetSpriteFlipX(birdP, true);
        SWSetSpriteFrameAdvanceMode(birdP, kPatrollingMode);
        SWSetSpriteFrameTime(birdP, (short)GetRandom(70, 110));
        SWSetSpriteFrameAdvance(birdP, 1);
        SWSetCurrentFrameIndex(birdP, GetRandom(0, birdP->maxFrames - 1));
        birdP->userData = (long)(flockY + (float)i * GetRandomFloat(10.0f, 22.0f));
        birdP->miscFloat = GetRandomFloat(1.3f, 2.6f) * (0.6f + scale);
        birdP->misc1.n = GetRandom(0, 360);
        SWSetSpriteLocation(birdP, -80.0f - (float)i * GetRandomFloat(22.0f, 40.0f), (float)birdP->userData);
        SWSetSpriteMoveProc(birdP, BirdMoveProc);
        SWSetSpriteMoveTime(birdP, 16);
    }
}

// ---------------------------------------------------------------------------
// Input

static Boolean MPEventCallback(SDL_Event *event)
{
    if (event->type != SDL_KEYDOWN)
        return event->type == SDL_KEYUP && !(event->key.keysym.mod & (KMOD_GUI | KMOD_CTRL | KMOD_ALT));
    if (event->key.keysym.mod & (KMOD_GUI | KMOD_CTRL | KMOD_ALT))
        return false;
    switch (event->key.keysym.sym) {
        case SDLK_UP: case SDLK_KP_8:       gMP.keyPresses |= kPadUp; break;
        case SDLK_DOWN: case SDLK_KP_2:     gMP.keyPresses |= kPadDown; break;
        case SDLK_LEFT: case SDLK_KP_4:     gMP.keyPresses |= kPadLeft; break;
        case SDLK_RIGHT: case SDLK_KP_6:    gMP.keyPresses |= kPadRight; break;
        case SDLK_RETURN: case SDLK_KP_ENTER: case SDLK_SPACE: gMP.keyPresses |= kPadAccept; break;
        case SDLK_ESCAPE: case SDLK_BACKSPACE: gMP.keyPresses |= kPadBack; break;
        case SDLK_TAB:                      gMP.keyPresses |= kPadRight; break;
        default: break;
    }
    return true;
}

static void ShowNote(const char *text)
{
    snprintf(gMP.message, sizeof(gMP.message), "%s", text);
    gMP.messageUntil = SDL_GetTicks() + 4000;
}

static Boolean ModeIsSplit(void)
{
    return gMP.mode == kModeTwin || gMP.mode == kModePhantoms;
}

static const char *InputName(int input)
{
    static char text[32];

    switch (input) {
        case kSplitInputKeyboard:     return "Arrow keys";
        case kSplitInputKeyboardLeft: return "W A S D keys";
        default:
            snprintf(text, sizeof(text), "Controller %d", input - kSplitInputPad1 + 1);
            return text;
    }
}

/// The next control scheme nobody else is using.
static int NextFreeInput(int player, int from)
{
    int input, i, tries;

    for (tries = 0, input = from; tries < kNumSplitInputs; tries++) {
        input = (input + 1) % kNumSplitInputs;
        for (i = 0; i < gMP.numPlayers && (i == player || gMP.inputs[i] != input); i++)
            ;
        if (i == gMP.numPlayers)
            return input;
    }
    return from;
}

static void ChooseProfile(int player)
{
    char prompt[120], chosen[64];
    char exclude[kMaxPlayers][64];
    int i, n = 0;

    for (i = 0; i < gMP.numPlayers; i++)
        if (i != player && gMP.profiles[i][0] != 0)
            snprintf(exclude[n++], sizeof(exclude[0]), "%s", gMP.profiles[i]);
    snprintf(prompt, sizeof(prompt), "Who is Player %d?", player + 1);
    chosen[0] = 0;
    SWSetPostDrawCallBack(gSpriteWorldP, gMP.savedPostDraw);
    if (DoChooseOtherPlayerDialog(prompt, exclude, n, chosen, sizeof(chosen)) && chosen[0] != 0)
        snprintf(gMP.profiles[player], sizeof(gMP.profiles[player]), "%s", chosen);
    SWSetPostDrawCallBack(gSpriteWorldP, MultiplayerScreenPostDraw);
    SetMMEventCallback(MPEventCallback);
}

static void AddPlayer(int input)
{
    int p = gMP.numPlayers;

    if (p >= kMaxPlayers)
        return;
    gMP.numPlayers++;
    gMP.profiles[p][0] = 0;
    gMP.inputs[p] = input >= 0 ? input : NextFreeInput(p, gMP.inputs[p > 0 ? p - 1 : 0]);
    FillMissingProfiles();
}

/// Sets everything up for the chosen game and leaves for the mansion choice.
static void StartLocalGame(Boolean customMansion)
{
    int i;

    for (i = 0; i < gMP.numPlayers; i++) {
        if (gMP.profiles[i][0] == 0) {
            gMP.column = 1;
            ShowNote("Every player needs a profile: choose one for each player.");
            return;
        }
        if (ModeIsSplit() && gMP.inputs[i] >= kSplitInputPad1 && gMP.inputs[i] - kSplitInputPad1 >= GetNumControllers()) {
            char note[160];
            snprintf(note, sizeof(note), "%s isn't plugged in: plug it in, or give player %d other controls.",
                     InputName(gMP.inputs[i]), i + 1);
            gMP.column = 1;
            ShowNote(note);
            return;
        }
    }
    SaveLobby();
    SetMultiplayerPlayers(gMP.numPlayers, gMP.profiles, gMP.characters, ModeIsSplit());
    if (ModeIsSplit())
        SplitConfigure(gMP.mode == kModeTwin ? kSplitModeCoop : kSplitModeRace, gMP.numPlayers, gMP.inputs);
    else
        SplitCancel();
    ShutDownScreen();
    MMHideCursor();
    if (customMansion) {
        HandleOpenMansionButton();
        if (gCurrentGameMode == kMultiplayerScreenMode || gCurrentGameMode == kTitleScreenMode)
            EnterMultiplayerScreenAgain();
        return;
    }
    DoGameFadeToBlackFullAnim(30, false);
    ExitTitleScreen();
    SetUpDifficultyScreen();
}

/// Online games: what to host or join (StartNetworkGame).
static struct {
    char    code[32];
    char    hostID[16];
} gJoin;

/// The saved games of exactly the players in the lobby (Co-op on this computer), looked up
/// again when the players change (and now and then).
static void FindLobbySaves(void)
{
    char ids[kMaxPlayers][16], key[400];
    size_t n = 0;
    int i;

    key[0] = 0;
    for (i = 0; i < gMP.numPlayers && gMP.mode != kModePhantoms; i++) {
        if (gMP.profiles[i][0] == 0) {
            key[0] = 0;
            break;
        }
        snprintf(ids[i], 16, "%s", OnlineProfileID(gMP.profiles[i]));
        n += (size_t)snprintf(key + n, sizeof(key) - n, "%s,", ids[i]);
    }
    n += (size_t)snprintf(key + n, sizeof(key) - n, "%d", gMP.mode);
    if (strcmp(key, gMP.savesKey) == 0 && SDL_GetTicks() - gMP.savesCheckedAt < 2000)
        return;
    snprintf(gMP.savesKey, sizeof(gMP.savesKey), "%s", key);
    gMP.savesCheckedAt = SDL_GetTicks();
    gMP.numSaves = key[0] && gMP.mode != kModePhantoms
                       ? MultiSaveList(gMP.mode == kModeTwin ? kMultiSaveArcade : kMultiSaveCoop, ids, gMP.numPlayers,
                                       gMP.saves, 3)
                       : 0;
}

/// On this computer (Co-op or Arcade Split Screen): carries on with a saved game of these
/// players.
static void ContinueLocalSave(int index)
{
    char path[1500];
    int i;

    for (i = 0; ModeIsSplit() && i < gMP.numPlayers; i++)
        if (gMP.inputs[i] >= kSplitInputPad1 && gMP.inputs[i] - kSplitInputPad1 >= GetNumControllers()) {
            gMP.column = 1;
            ShowNote("A player's controller isn't plugged in: plug it in, or give them other controls.");
            return;
        }
    snprintf(path, sizeof(path), "%s", gMP.saves[index].path);
    SaveLobby();
    SetMultiplayerPlayers(gMP.numPlayers, gMP.profiles, gMP.characters, ModeIsSplit());
    ShutDownScreen();
    if (ModeIsSplit()) {
        SplitConfigure(kSplitModeCoop, gMP.numPlayers, gMP.inputs);
        if (SplitContinueSave(path) != 0)
            EnterMultiplayerScreenAgain();
        return;
    }
    SplitCancel();
    MultiplayerContinueSave(path);
}

/// Two computers: host or join (on the local network, or online through the lobby server),
/// then on as the title screen's old Multiplayer did.
static void StartNetworkGame(int rowKind, const char *address)
{
    int netMode = gMP.mode == kModePhantoms ? kNetModeRace : kNetModeRelay;
    Boolean online = rowKind == kRowHostPublic || rowKind == kRowHostPrivate || rowKind == kRowOnlineGame ||
                     rowKind == kRowJoinCode || rowKind == kRowFriend;
    Boolean host = rowKind == kRowHost || rowKind == kRowHostPublic || rowKind == kRowHostPrivate;
    int result;

    SWSetPostDrawCallBack(gSpriteWorldP, gMP.savedPostDraw);
    SetMMEventCallback(NULL);
    NetBrowseStop();
    OnlineDisconnect();
    SetPlayerCharacter(0, gMP.characters[0]);
    SetPlayerCharacter(1, gMP.characters[0]);
    if (online)
        result = MultiplayerConnectOnline(netMode, host, rowKind == kRowHostPublic, gJoin.code, gJoin.hostID);
    else
        result = MultiplayerConnect(netMode, host, address);
    if (result == kMultiplayerNoMansion) {
            // without the host's mansion: say so, and back down to the main menu
        ShowMissingMansionMessage();
        SWSetPostDrawCallBack(gSpriteWorldP, MultiplayerScreenPostDraw);
        SetMMEventCallback(MPEventCallback);
        LeaveToTitle();
        return;
    }
    if (result == kMultiplayerCancelled) {
        SWSetPostDrawCallBack(gSpriteWorldP, MultiplayerScreenPostDraw);
        SetMMEventCallback(MPEventCallback);
        NetBrowseStart();
        if (gMP.view == kViewOnline || gMP.view == kViewFriends)
            OnlineBrowse(true);
        return;
    }
    ShutDownScreen();
    MMHideCursor();
    if (result == kMultiplayerJoined) {
        if (NetContinuedSavePath() != NULL)
            MultiplayerContinueSave(NetContinuedSavePath());     // the host carries on with our saved game
        else
            StartCustomMansion();           // the host chose the mansion; it's open
        return;
    }
    {
            // the host picks: one of the game's mansions, a custom one (the other computer needs
            // that one too), or (Co-op) a game these two players saved together
        static char labels[5][300];
        const char *choices[5] = { "A mansion from the game", "A custom mansion" };
        MultiSaveInfo saves[3];
        int numSaves = netMode == kNetModeRelay ? MultiSaveListForCurrentGroup(kMultiSaveCoop, saves, 3) : 0;
        int i, choice;
        char detail[200];

        for (i = 0; i < numSaves; i++) {
            MultiSaveDescribe(&saves[i], detail, sizeof(detail));
            snprintf(labels[i], sizeof(labels[i]), "Continue \"%.60s\"  (%s)", saves[i].name, detail);
            choices[2 + i] = labels[i];
        }
        choice = DoSimpleChoiceDialog("Choose a mansion", choices, 2 + numSaves, 0);
        if (choice >= 2) {
            NetSetContinueSave(saves[choice - 2].path);
            MultiplayerContinueSave(saves[choice - 2].path);
            return;
        }
        if (choice == 1 && MMDoOpenMansionDialog() == 0) {
            StartCustomMansion();
            return;
        }
    }
    DoGameFadeToBlackFullAnim(30, false);
    ExitTitleScreen();
    SetUpDifficultyScreen();
}

/// A text entry dialog over the screen.
static Boolean EnterText(const char *prompt, char *text, int maxLen)
{
    Boolean ok;

    SWSetPostDrawCallBack(gSpriteWorldP, gMP.savedPostDraw);
    SetMMEventCallback(NULL);
    ok = DoTextEntryDialog(prompt, text, maxLen);
    SWSetPostDrawCallBack(gSpriteWorldP, MultiplayerScreenPostDraw);
    SetMMEventCallback(MPEventCallback);
    return ok && text[0] != 0;
}

static int AskChoice(const char *title, const char **choices, int count)
{
    int choice;

    SWSetPostDrawCallBack(gSpriteWorldP, gMP.savedPostDraw);
    SetMMEventCallback(NULL);
    choice = DoSimpleChoiceDialog(title, choices, count, 0);
    SWSetPostDrawCallBack(gSpriteWorldP, MultiplayerScreenPostDraw);
    SetMMEventCallback(MPEventCallback);
    return choice;
}

static void ShowView(int view)
{
    gMP.view = view;
    gMP.row = 0;
    gMP.field = 0;
    if (view == kViewOnline || view == kViewFriends)
        OnlineBrowse(true);
    else
        OnlineDisconnect();
}

/// A friend in the list: join their game, or let them go.
static void FriendChosen(const OnlineFriend *f)
{
    OnlineFriendStatus st;
    const char *choices[3];
    char title[120];
    int n = 0, joinChoice = -1, removeChoice, choice;

    if (OnlineFriendStatusFor(f->id, &st) && st.code[0]) {
        joinChoice = n;
        choices[n++] = "Join their game";
    }
    removeChoice = n;
    choices[n++] = "Remove from friends";
    choices[n++] = "Cancel";
    snprintf(title, sizeof(title), "%s  (%s)", f->name, f->id);
    choice = AskChoice(title, choices, n);
    if (choice == joinChoice && joinChoice >= 0) {
        snprintf(gJoin.code, sizeof(gJoin.code), "%s", st.code);
        gJoin.hostID[0] = 0;
        StartNetworkGame(kRowFriend, NULL);
    } else if (choice == removeChoice) {
        OnlineRemoveFriend(f->id);
    }
}

static void ActivateRow(Row *r)
{
    char text[64], norm[16];

    switch (r->kind) {
        case kRowGoOnline:
            ShowView(kViewOnline);
            break;
        case kRowOnlineGame:
            gMP.mode = gMP.online[r->arg].mode == kNetModeRace ? kModePhantoms : kModeRelay;
            snprintf(gJoin.code, sizeof(gJoin.code), "%s", gMP.online[r->arg].code);
            snprintf(gJoin.hostID, sizeof(gJoin.hostID), "%s", gMP.online[r->arg].code[0] ? "" : gMP.online[r->arg].hostID);
            StartNetworkGame(kRowOnlineGame, NULL);
            break;
        case kRowHostPublic:
        case kRowHostPrivate:
            gJoin.code[0] = gJoin.hostID[0] = 0;
            StartNetworkGame(r->kind, NULL);
            break;
        case kRowJoinCode:
            text[0] = 0;
            if (EnterText("The game's code (like RAVEN-4821):", text, 20)) {
                snprintf(gJoin.code, sizeof(gJoin.code), "%s", text);
                gJoin.hostID[0] = 0;
                StartNetworkGame(kRowJoinCode, NULL);
            }
            break;
        case kRowGoFriends:
            ShowView(kViewFriends);
            break;
        case kRowLobbyAddress: {
            char address[200];
            snprintf(address, sizeof(address), "%s", OnlineLobbyAddress());
            if (EnterText("Lobby server (address:port):", address, 60)) {
                OnlineSetLobbyAddress(address);
                OnlineDisconnect();
                OnlineBrowse(true);
            }
            break;
        }
        case kRowAddFriend:
            text[0] = 0;
            if (EnterText("Your friend's code (like ABCD-EFGH-JKLM):", text, 20)) {
                if (!OnlineNormalizeID(text, norm))
                    ShowNote("That isn't a friend code: it's 12 letters and digits.");
                else if (strcmp(norm, OnlineMyID()) == 0)
                    ShowNote("That's your own code.");
                else if (OnlineAddFriend(norm, NULL)) {
                    ShowNote("Friend added: their name shows once they're online.");
                    OnlineBrowse(true);         // their status, now
                }
            }
            break;
        case kRowFriend:
            FriendChosen(&gMP.friends[r->arg]);
            break;
        case kRowRecent:
            if (OnlineAddFriend(gMP.recent[r->arg].id, gMP.recent[r->arg].name)) {
                snprintf(text, sizeof(text), "%.40s is a friend now.", gMP.recent[r->arg].name);
                ShowNote(text);
                OnlineBrowse(true);
            }
            break;
        case kRowBackToOnline:
            ShowView(kViewOnline);
            break;
        case kRowContinueSave:
            ContinueLocalSave(r->arg);
            break;
        case kRowLocalGame:
            gMP.view = kViewLobby;
            gMP.row = 0;
            gMP.field = 0;
            break;
        case kRowFoundGame:
            gMP.mode = gMP.found[r->arg].mode == kNetModeRace ? kModePhantoms : kModeRelay;
            StartNetworkGame(kRowFoundGame, gMP.found[r->arg].address);
            break;
        case kRowHost:
            StartNetworkGame(kRowHost, NULL);
            break;
        case kRowJoinAddress:
            StartNetworkGame(kRowJoinAddress, NULL);
            break;
        case kRowPlayer:
            if (gMP.field == 0)
                ChooseProfile(r->arg);
            else if (gMP.field == 1)
                gMP.characters[r->arg] = (gMP.characters[r->arg] + 1) % kNumCharacters;
            else
                gMP.inputs[r->arg] = NextFreeInput(r->arg, gMP.inputs[r->arg]);
            break;
        case kRowAddPlayer:
            AddPlayer(-1);
            break;
        case kRowRemovePlayer:
            if (gMP.numPlayers > 2)
                gMP.numPlayers--;
            break;
        case kRowChooseMansion:
            StartLocalGame(false);
            break;
        case kRowCustomMansion:
            StartLocalGame(true);
            break;
        case kRowBackToGames:
            ShowView(kViewGames);
            break;
        default:
            break;
    }
}

static Boolean RowIsFocusable(const Row *r)
{
    return r->kind != kRowHeader && r->kind != kRowSearching && r->kind != kRowInfo;
}

static void MoveRow(int delta)
{
    int i, row = gMP.row;

    for (i = 0; i < gMP.numRows; i++) {
        row = (row + delta + gMP.numRows) % gMP.numRows;
        if (RowIsFocusable(&gMP.rows[row])) {
            gMP.row = row;
            return;
        }
    }
}

static void FixRowFocus(void)
{
    if (gMP.row >= gMP.numRows)
        gMP.row = gMP.numRows - 1;
    if (gMP.row < 0)
        gMP.row = 0;
    if (gMP.numRows > 0 && !RowIsFocusable(&gMP.rows[gMP.row]))
        MoveRow(1);
}

// ---------------------------------------------------------------------------
// Layout (design coordinates, 1280x960)

#define kCardLeft       60.0f
#define kCardRight      590.0f
#define kCardTop        200.0f
#define kCardHeight     170.0f
#define kCardGap        22.0f
#define kWindowLeft     640.0f
#define kWindowRight    1220.0f
#define kWindowTop      170.0f
#define kWindowBottom   850.0f
#define kRowHeight      48.0f
#define kBackLeft       60.0f
#define kBackTop        800.0f
#define kBackRight      250.0f
#define kBackBottom     860.0f

static void CardRect(int mode, float *top, float *bottom)
{
    *top = kCardTop + mode * (kCardHeight + kCardGap);
    *bottom = *top + kCardHeight;
}

static Row *AddRow(int kind, int arg, const char *text, const char *detail)
{
    Row *r;

    if (gMP.numRows >= kMaxRows)
        return NULL;
    r = &gMP.rows[gMP.numRows++];
    r->kind = kind;
    r->arg = arg;
    snprintf(r->text, sizeof(r->text), "%s", text);
    snprintf(r->detail, sizeof(r->detail), "%s", detail != NULL ? detail : "");
    return r;
}

static const char *kNetModeNames[] = { "Co-op", "Ghost Race" };

/// The lobby's state as a row, while it isn't ready.
static Boolean AddLobbyStatusRow(void)
{
    if (OnlineIsReady())
        return true;
    if (OnlineFailed()) {
        AddRow(kRowInfo, 0, OnlineError()[0] ? OnlineError() : "Can't reach the lobby server", OnlineLobbyAddress());
    } else {
        AddRow(kRowSearching, 0, "Connecting to the lobby", NULL);
    }
    return false;
}

static void BuildOnlineRows(void)
{
    char text[160], detail[160];
    int i, shown;

    AddRow(kRowHeader, 0, "Online games", NULL);
    if (AddLobbyStatusRow()) {
        gMP.numOnline = OnlineGetGames(gMP.online, kMaxOnlineGames);
        for (i = 0, shown = 0; i < gMP.numOnline && shown < 4; i++, shown++) {
            OnlineGame *g = &gMP.online[i];
            snprintf(text, sizeof(text), "%s%s's game", g->isFriend ? "* " : "", g->hostName);
            snprintf(detail, sizeof(detail), "%s, %d of 4 players, %s%s", kNetModeNames[g->mode == kNetModeRace],
                     g->numPlayers > 0 ? g->numPlayers : 1, g->isPublic ? "public" : "private",
                     g->isFriend ? ", a friend's" : "");
            AddRow(kRowOnlineGame, i, text, detail);
        }
        if (gMP.numOnline > shown) {
            snprintf(text, sizeof(text), "and %d more", gMP.numOnline - shown);
            AddRow(kRowInfo, 0, text, NULL);
        }
        if (gMP.numOnline == 0)
            AddRow(kRowSearching, 0, OnlineListArrived() ? "No games right now: host one" : "Looking for games", NULL);
    }
    snprintf(detail, sizeof(detail), "%s: anyone can join", kModes[gMP.mode].title);
    AddRow(kRowHostPublic, 0, "Host a public game", detail);
    AddRow(kRowHostPrivate, 0, "Host a private game", "Up to 4 players; friends see it, others need its code");
    AddRow(kRowJoinCode, 0, "Join with a code...", NULL);
    gMP.numFriends = OnlineGetFriends(gMP.friends, kMaxOnlineFriends);
    {
        int online = 0;
        OnlineFriendStatus st;
        for (i = 0; i < gMP.numFriends; i++)
            if (OnlineFriendStatusFor(gMP.friends[i].id, &st) && st.online)
                online++;
        snprintf(text, sizeof(text), gMP.numFriends == 0 ? "Friends" : "Friends (%d online)", online);
    }
    snprintf(detail, sizeof(detail), "Your friend code: %s", OnlineMyID());
    AddRow(kRowGoFriends, 0, text, detail);
    snprintf(text, sizeof(text), "Lobby server: %s", OnlineLobbyAddress());
    AddRow(kRowLobbyAddress, 0, text, "Change...");
    AddRow(kRowBackToGames, 0, "Back to the games", NULL);
}

static void BuildFriendRows(void)
{
    char text[160], detail[160];
    OnlineFriendStatus st;
    int i, shown;

    snprintf(text, sizeof(text), "Your friend code:  %s", OnlineMyID());
    AddRow(kRowInfo, 0, text, "Give it to friends so they can add you");
    AddRow(kRowAddFriend, 0, "Add a friend by their code...", NULL);
    AddRow(kRowHeader, 0, "Friends", NULL);
    gMP.numFriends = OnlineGetFriends(gMP.friends, kMaxOnlineFriends);
    for (i = 0, shown = 0; i < gMP.numFriends && shown < 5; i++, shown++) {
        OnlineFriend *f = &gMP.friends[i];
        Boolean known = OnlineFriendStatusFor(f->id, &st);
        if (known && st.name[0] && (strcmp(f->name, "Friend") == 0 || strcmp(f->name, st.name) != 0)) {
            OnlineAddFriend(f->id, st.name);        // they're online: their name
            snprintf(f->name, sizeof(f->name), "%s", st.name);
        }
        if (!OnlineIsReady())
            snprintf(detail, sizeof(detail), "%s", f->id);
        else if (known && st.online && st.code[0])
            snprintf(detail, sizeof(detail), "Online, hosting a game: join it");
        else
            snprintf(detail, sizeof(detail), "%s", known && st.online ? "Online" : "Offline");
        AddRow(kRowFriend, i, f->name, detail);
    }
    if (gMP.numFriends == 0)
        AddRow(kRowSearching, 0, "No friends added yet", NULL);
    gMP.numRecent = OnlineGetRecentPlayers(gMP.recent, 8);
    if (gMP.numRecent > 0) {
        AddRow(kRowHeader, 0, "Played with lately", NULL);
        for (i = 0; i < gMP.numRecent && i < 2; i++)
            AddRow(kRowRecent, i, gMP.recent[i].name, "Add as a friend");
    }
    AddRow(kRowBackToOnline, 0, "Back", NULL);
}

static void BuildRows(void)
{
    static const char *modeNames[] = { "Co-op", "Ghost Race" };
    char text[160], detail[160];
    float y = kWindowTop + 100;
    int i;

    gMP.numRows = 0;
    if (gMP.view == kViewGames) {
        AddRow(kRowHeader, 0, "On this computer", NULL);
        AddRow(kRowLocalGame, 0, "New game", ModeIsSplit() ? "Split screen, 2 to 4 players" : "Take turns, 2 to 4 players");
        if (kModes[gMP.mode].network) {
            AddRow(kRowHeader, 0, "On your network", NULL);
            gMP.numFound = NetBrowseGames(gMP.found, kMaxFound);
            for (i = 0; i < gMP.numFound; i++) {
                snprintf(text, sizeof(text), "%s's game", gMP.found[i].hostName);
                snprintf(detail, sizeof(detail), "%s, %d of 4 players  |  %s", modeNames[gMP.found[i].mode == kNetModeRace],
                         gMP.found[i].numPlayers > 0 ? gMP.found[i].numPlayers : 1, gMP.found[i].address);
                AddRow(kRowFoundGame, i, text, detail);
            }
            if (gMP.numFound == 0)
                AddRow(kRowSearching, 0, NetBrowsing() ? "Looking for games" : "Can't look for games here", NULL);
            AddRow(kRowHost, 0, "Host a game", "Up to 3 other computers on this Wi-Fi can join");
            AddRow(kRowJoinAddress, 0, "Join by address...", "Type the host's address");
            AddRow(kRowHeader, 0, "Over the internet", NULL);
            AddRow(kRowGoOnline, 0, "Play online...", "Public and private games, friends");
        } else {
            AddRow(kRowHeader, 0, "Split screen only", NULL);
            AddRow(kRowSearching, 0, "Everyone plays on this computer", NULL);
        }
    } else if (gMP.view == kViewOnline) {
        BuildOnlineRows();
    } else if (gMP.view == kViewFriends) {
        BuildFriendRows();
    } else {
        AddRow(kRowHeader, 0, "Who plays", NULL);
        for (i = 0; i < gMP.numPlayers; i++)
            AddRow(kRowPlayer, i, "", NULL);
        if (gMP.numPlayers < kMaxPlayers)
            AddRow(kRowAddPlayer, 0, "Add a player", NULL);
        if (gMP.numPlayers > 2)
            AddRow(kRowRemovePlayer, 0, "Remove the last player", NULL);
        FindLobbySaves();
        for (i = 0; i < gMP.numSaves; i++) {
            MultiSaveDescribe(&gMP.saves[i], detail, sizeof(detail));
            snprintf(text, sizeof(text), "Continue \"%.50s\"", gMP.saves[i].name);
            AddRow(kRowContinueSave, i, text, detail);
        }
        AddRow(kRowChooseMansion, 0, "Choose a mansion", "Then the difficulty and the mansion, as usual");
        AddRow(kRowCustomMansion, 0, "Custom mansion...", NULL);
        AddRow(kRowBackToGames, 0, "Back to the games", NULL);
    }
    for (i = 0; i < gMP.numRows; i++) {
        Row *r = &gMP.rows[i];
        float h = r->kind == kRowHeader ? 40.0f : (r->kind == kRowPlayer ? 62.0f : (r->detail[0] ? 58.0f : kRowHeight));
        if (r->kind == kRowHeader && i > 0)
            y += 12;
        r->top = y;
        r->bottom = y + h;
        y += h;
    }
    FixRowFocus();
}

/// Lobby rows have three fields across: profile, character, controls (split screen only).
static int FieldAt(float x)
{
    if (x < kWindowLeft + 272)
        return 0;
    if (!ModeIsSplit() || x < kWindowLeft + 388)
        return 1;
    return 2;
}

// ---------------------------------------------------------------------------
// Each frame

static void HandleMenuInput(void)
{
    SWPoint mouse;
    Boolean mouseDown, clicked, moved;
    int presses = gMP.keyPresses | TakeControllerMenuPresses(), i;
    float top, bottom;

    gMP.keyPresses = 0;
    mouseDown = MMGetMouse(&mouse);
    SWConvertCoordinates(gSpriteWorldP, &mouse);
    moved = !gControllerHidesCursor && (mouse.h != gMP.mouse.h || mouse.v != gMP.mouse.v);
    clicked = mouseDown && !gMP.mouseWasDown && !gControllerHidesCursor;
    gMP.mouseWasDown = mouseDown;
    gMP.mouse = mouse;
    if (moved || clicked)
        gMP.pointer = true;
    if (presses != 0 || gControllerHidesCursor) {
        gMP.pointer = false;
        gMP.hoverCard = -1;
    }

        // a controller that isn't playing yet: A adds it as a player (in the lobby)
    if ((presses & kPadAccept) && gMP.view == kViewLobby && ModeIsSplit() && gControllerHidesCursor) {
        int pad = ControllerIndexForInstanceID(ControllerLastPadID());
        for (i = 0; pad >= 0 && i < gMP.numPlayers && gMP.inputs[i] != kSplitInputPad1 + pad; i++)
            ;
        if (pad >= 0 && i == gMP.numPlayers && gMP.numPlayers < kMaxPlayers) {
            AddPlayer(kSplitInputPad1 + pad);
            presses &= ~kPadAccept;
        }
    }

    if (moved || clicked) {
        gMP.hoverCard = -1;
        for (i = 0; i < kNumModes; i++) {
            CardRect(i, &top, &bottom);
            if (mouse.h >= kCardLeft && mouse.h < kCardRight && mouse.v >= top && mouse.v < bottom) {
                gMP.column = 0;
                gMP.hoverCard = i;
                if (clicked && gMP.mode != i) {
                    gMP.mode = i;
                    ShowView(kViewGames);
                    MMPlaySound(0x0b, 8, 1);
                }
            }
        }
        for (i = 0; i < gMP.numRows; i++) {
            Row *r = &gMP.rows[i];
            if (RowIsFocusable(r) && mouse.h >= kWindowLeft + 20 && mouse.h < kWindowRight - 20 && mouse.v >= r->top &&
                mouse.v < r->bottom) {
                gMP.column = 1;
                gMP.row = i;
                if (r->kind == kRowPlayer)
                    gMP.field = FieldAt(mouse.h);
                if (clicked) {
                    MMPlaySound(0x0b, 8, 1);
                    ActivateRow(r);
                    return;
                }
            }
        }
        if (mouse.h >= kBackLeft && mouse.h < kBackRight && mouse.v >= kBackTop && mouse.v < kBackBottom) {
            gMP.column = 2;
            if (clicked) {
                LeaveToTitle();
                return;
            }
        }
    }

    if (presses & kPadBack) {
        if (gMP.view == kViewFriends) {
            ShowView(kViewOnline);
        } else if (gMP.view == kViewLobby || gMP.view == kViewOnline) {
            ShowView(kViewGames);
        } else {
            LeaveToTitle();
        }
        return;
    }
    if (gMP.column == 0) {
        if (presses & kPadUp) {
            gMP.mode = (gMP.mode + kNumModes - 1) % kNumModes;
            ShowView(kViewGames);
        }
        if (presses & kPadDown) {
            if (gMP.mode == kNumModes - 1)
                gMP.column = 2;
            else {
                gMP.mode++;
                ShowView(kViewGames);
            }
        }
        if (presses & (kPadRight | kPadAccept)) {
            gMP.column = 1;
            gMP.row = 0;
            FixRowFocus();
        }
    } else if (gMP.column == 1) {
        Row *r = gMP.numRows > 0 ? &gMP.rows[gMP.row] : NULL;
        if (presses & kPadUp)
            MoveRow(-1);
        if (presses & kPadDown)
            MoveRow(1);
        if (presses & kPadLeft) {
            if (r != NULL && r->kind == kRowPlayer && gMP.field > 0)
                gMP.field--;
            else
                gMP.column = 0;
        }
        if ((presses & kPadRight) && r != NULL && r->kind == kRowPlayer && gMP.field < (ModeIsSplit() ? 2 : 1))
            gMP.field++;
        if ((presses & kPadAccept) && r != NULL) {
            MMPlaySound(0x0b, 8, 1);
            ActivateRow(r);
        }
    } else {
        if (presses & (kPadUp | kPadRight))
            gMP.column = presses & kPadUp ? 0 : 1;
        if (presses & kPadAccept)
            LeaveToTitle();
    }
}

/// Slow out of the start, slow into the end.
static float Ease(float t)
{
    return t <= 0 ? 0 : t >= 1 ? 1 : t * t * (3.0f - 2.0f * t);
}

void HandleMultiplayerScreenCallback(void)
{
    float t;

    SWProcessSpriteWorld(gSpriteWorldP);
    SWAnimateSpriteWorld(gSpriteWorldP);
    KeepMusicPlaying();
    if (!gSpriteWorldP->frameHasOccurred || !gMP.active)
        return;
    TitleScreenLightning();
    if (SDL_GetTicks() >= gMP.nextBird && gMP.phase != kPhaseSinking) {
        SpawnBird();
        gMP.nextBird = SDL_GetTicks() + (Uint32)GetRandom(1800, 5200);
    }

    switch (gMP.phase) {
        case kPhaseRising:
            t = (float)(SDL_GetTicks() - gMP.phaseStart) / (float)kLiftMs;
            if (t >= 1.0f) {
                Lift(kSkyLift - gMP.lifted);
                gMP.phase = kPhaseMenu;
                gMP.phaseStart = SDL_GetTicks();
            } else {
                Lift(kSkyLift * Ease(t) - gMP.lifted);
            }
            gMP.keyPresses = 0;
            TakeControllerMenuPresses();
            break;
        case kPhaseMenu:
            if (gMP.view == kViewOnline || gMP.view == kViewFriends)
                OnlinePoll();
            BuildRows();
            HandleMenuInput();
            break;
        case kPhaseSinking:
            t = (float)(SDL_GetTicks() - gMP.phaseStart) / (float)kLiftMs;
            Lift(kSkyLift * (1.0f - Ease(t < 1.0f ? t : 1.0f)) - gMP.lifted);
            if (t >= 1.0f) {
                Lift(-gMP.lifted);
                ShutDownScreen();
                TitleScreenFadeMenu(false);
                gCurrentGameMode = kTitleScreenMode;
            }
            break;
    }
}

/// After a custom mansion was picked and cancelled: back in the sky, still the same game.
void EnterMultiplayerScreenAgain(void)
{
    gMP.active = true;
    gMP.savedPostDraw = gSpriteWorldP->postDrawCallBack;
    SWSetPostDrawCallBack(gSpriteWorldP, MultiplayerScreenPostDraw);
    SetMMEventCallback(MPEventCallback);
    NetBrowseStart();
    gCurrentGameMode = kMultiplayerScreenMode;
}

// ---------------------------------------------------------------------------
// Drawing

static float gScale = 1.0f;     ///< window pixels per design unit, for crisp text

static void BeginDesign(SpriteWorldPtr w)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glScalef(w->designScaleH, w->designScaleV, 1.0f);
    gScale = w->designScaleV > 0 ? w->designScaleV : 1.0f;
}

static void EndDesign(void)
{
    glPopMatrix();
}

static void Fill(float x0, float y0, float x1, float y1, float r, float g, float b, float a)
{
    glDisable(GL_TEXTURE_2D);
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
    glVertex2f(x0, y0);
    glVertex2f(x1, y0);
    glVertex2f(x1, y1);
    glVertex2f(x0, y1);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

static void Frame(float x0, float y0, float x1, float y1, float t, float r, float g, float b, float a)
{
    Fill(x0, y0, x1, y0 + t, r, g, b, a);
    Fill(x0, y1 - t, x1, y1, r, g, b, a);
    Fill(x0, y0 + t, x0 + t, y1 - t, r, g, b, a);
    Fill(x1 - t, y0 + t, x1, y1 - t, r, g, b, a);
}

/// The dialogs' wood panel at any size: the border keeps its width, the middle stretches.
static void WoodPanel(float x0, float y0, float x1, float y1, float brightness, float alpha)
{
    float bx = 14, by = 14, u = bx / gMP.woodW, v = by / gMP.woodH;
    float xs[4] = { x0, x0 + bx, x1 - bx, x1 }, ys[4] = { y0, y0 + by, y1 - by, y1 };
    float us[4] = { 0, u, 1 - u, 1 }, vs[4] = { 0, v, 1 - v, 1 };
    int i, j;

    if (gMP.woodTexture == 0) {
        Fill(x0, y0, x1, y1, 0.25f * brightness, 0.13f * brightness, 0.07f * brightness, alpha);
        return;
    }
    glBindTexture(GL_TEXTURE_2D, gMP.woodTexture);
    glColor4f(brightness, brightness, brightness, alpha);
    glBegin(GL_QUADS);
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++) {
            glTexCoord2f(us[j], vs[i]); glVertex2f(xs[j], ys[i]);
            glTexCoord2f(us[j + 1], vs[i]); glVertex2f(xs[j + 1], ys[i]);
            glTexCoord2f(us[j + 1], vs[i + 1]); glVertex2f(xs[j + 1], ys[i + 1]);
            glTexCoord2f(us[j], vs[i + 1]); glVertex2f(xs[j], ys[i + 1]);
        }
    glEnd();
}

/// Text with its baseline at y: justify 0 left, 1 centered, 2 right. Returns its width.
static float Text(MMFont *font, float size, const char *text, float x, float y, int justify, float r, float g, float b,
                  float a)
{
    int face = (int)(size * gScale + 0.5f);
    float unit, width;

    if (font == NULL || text == NULL || text[0] == 0)
        return 0;
    if (face < 6)
        face = 6;
    unit = size / (float)face;
    MMFontFaceSize(font, face);
    width = MMFontRenderWidth(font, text) * unit;
    if (justify == 1)
        x -= width / 2;
    else if (justify == 2)
        x -= width;
    glColor4f(0, 0, 0, a * 0.75f);
    MMFontRender(font, text, x + size * 0.06f, y + size * 0.06f, unit);
    glColor4f(r, g, b, a);
    MMFontRender(font, text, x, y, unit);
    return width;
}

/// Wraps text into lines no wider than width; returns the y after the last line.
static float WrappedText(MMFont *font, float size, const char *text, float x, float y, float width, float lineHeight,
                         float r, float g, float b, float a)
{
    char line[256], word[128];
    const char *p = text;
    int face = (int)(size * gScale + 0.5f);
    float unit = size / (float)(face < 6 ? 6 : face);

    line[0] = 0;
    MMFontFaceSize(font, face < 6 ? 6 : face);
    while (*p) {
        int n = 0;
        char trial[400];
        while (*p == ' ')
            p++;
        while (*p && *p != ' ' && n < (int)sizeof(word) - 1)
            word[n++] = *p++;
        word[n] = 0;
        if (n == 0)
            break;
        snprintf(trial, sizeof(trial), "%s%s%s", line, line[0] ? " " : "", word);
        if (line[0] && MMFontRenderWidth(font, trial) * unit > width) {
            Text(font, size, line, x, y, 0, r, g, b, a);
            y += lineHeight;
            snprintf(line, sizeof(line), "%s", word);
            MMFontFaceSize(font, face < 6 ? 6 : face);
        } else {
            snprintf(line, sizeof(line), "%s", trial);
        }
    }
    if (line[0]) {
        Text(font, size, line, x, y, 0, r, g, b, a);
        y += lineHeight;
    }
    return y;
}

static float Pulse(void)
{
    return 0.5f + 0.5f * sinf((float)SDL_GetTicks() / 260.0f);
}

/// A game type's name, lettered like the menu buttons (cyan when pointed at, as there), with
/// its top left at (x, y).
static void ModeTitle(int mode, float x, float y, Boolean lit, float alpha)
{
    const float h = 56.0f, w = h * 10.0f, u = lit ? 0.5f : 0.0f;

    if (gMP.modeTitles == 0) {
        Text(gMP.font, 30, kModes[mode].title, x + 4, y + 36, 0, 1.0f, 0.85f, 0.32f, alpha);
        return;
    }
    glBindTexture(GL_TEXTURE_2D, gMP.modeTitles);
    glColor4f(1, 1, 1, alpha);
    glBegin(GL_QUADS);
    glTexCoord2f(u, mode / (float)kNumModes);               glVertex2f(x, y);
    glTexCoord2f(u + 0.5f, mode / (float)kNumModes);        glVertex2f(x + w, y);
    glTexCoord2f(u + 0.5f, (mode + 1) / (float)kNumModes);  glVertex2f(x + w, y + h);
    glTexCoord2f(u, (mode + 1) / (float)kNumModes);         glVertex2f(x, y + h);
    glEnd();
}

static void DrawModeCards(float alpha)
{
    float top, bottom;
    int i;

    for (i = 0; i < kNumModes; i++) {
        Boolean selected = gMP.mode == i, focused = gMP.column == 0 && selected;
        Boolean hovered = gMP.pointer && gMP.hoverCard == i;
        CardRect(i, &top, &bottom);
        WoodPanel(kCardLeft, top, kCardRight, bottom, selected ? 1.0f : hovered ? 0.8f : 0.62f,
                  alpha * (selected ? 0.97f : 0.85f));
            // the frame shows where the keys or a controller are; with the mouse the brighter
            // card is enough
        if (selected && !gMP.pointer)
            Frame(kCardLeft - 3, top - 3, kCardRight + 3, bottom + 3, 3, 0.86f, 0.68f, 0.30f,
                  alpha * (focused ? 0.65f + 0.35f * Pulse() : 0.8f));
        ModeTitle(i, kCardLeft + 22, top + 8, hovered || (focused && !gMP.pointer),
                  alpha * (selected || hovered ? 1.0f : 0.75f));
        Text(gMP.font, 19, kModes[i].tagline, kCardLeft + 28, top + 78, 0, 0.95f, 0.93f, 0.85f,
             alpha * (selected ? 1.0f : 0.7f));
        WrappedText(gMP.font, 15, kModes[i].details, kCardLeft + 28, top + 104, kCardRight - kCardLeft - 56, 19,
                    0.85f, 0.80f, 0.70f, alpha * (selected ? 0.95f : 0.55f));
        Text(gMP.font, 14, kModes[i].badges, kCardRight - 22, bottom - 14, 2, 0.75f, 0.9f, 1.0f,
             alpha * (selected ? 0.9f : 0.5f));
    }
}

static void DrawPlayerRow(const Row *r, Boolean focused, float alpha)
{
    int p = r->arg, c = gMP.characters[p];
    const float *rgb = GetCharacterColor(c);
    float y = r->top + 40, x = kWindowLeft + 40;
    char label[96];
    const char *fields[3];
    float fx[3] = { kWindowLeft + 96, kWindowLeft + 282, kWindowLeft + 398 };
    int f, numFields = ModeIsSplit() ? 3 : 2;

    snprintf(label, sizeof(label), "%d", p + 1);
    Text(gMP.font, 30, label, x, y, 1, rgb[0], rgb[1], rgb[2], alpha);
    fields[0] = gMP.profiles[p][0] ? gMP.profiles[p] : "Choose...";
    fields[1] = GetCharacterNameByIndex(c);
    fields[2] = InputName(gMP.inputs[p]);
    if (gMP.inputs[p] >= kSplitInputPad1 && gMP.inputs[p] - kSplitInputPad1 >= GetNumControllers()) {
        static char missing[4][48];
        snprintf(missing[p], sizeof(missing[p]), "Pad %d unplugged", gMP.inputs[p] - kSplitInputPad1 + 1);
        fields[2] = missing[p];
    }
    for (f = 0; f < numFields; f++) {
        Boolean on = focused && gMP.field == f;
        float x1 = f + 1 < numFields ? fx[f + 1] - 10 : kWindowRight - 36;
        if (on)
            Fill(fx[f] - 8, r->top + 10, x1, r->bottom - 8, 1, 0.85f, 0.4f, 0.16f * alpha);
        Text(gMP.font, f == 0 ? 20 : 17, fields[f], fx[f], y, 0, on ? 1.0f : (f == 1 ? rgb[0] : 0.92f),
             on ? 1.0f : (f == 1 ? rgb[1] : 0.9f), on ? 0.6f : (f == 1 ? rgb[2] : 0.82f),
             alpha * (gMP.profiles[p][0] || f > 0 ? 1.0f : 0.6f));
    }
    if (focused && gMP.field > 0)
        Text(gMP.font, 12, "Return / A: change", kWindowRight - 36, r->top + 12, 2, 0.8f, 0.8f, 0.7f, alpha * 0.7f);
}

static void DrawWindow(float alpha)
{
    char dots[8];
    int i, n;

    WoodPanel(kWindowLeft, kWindowTop, kWindowRight, kWindowBottom, 1.0f, alpha * 0.97f);
    Frame(kWindowLeft + 10, kWindowTop + 10, kWindowRight - 10, kWindowBottom - 10, 2, 0.84f, 0.67f, 0.34f,
          alpha * 0.5f);
    Text(gMP.titleFont, 44, gMP.view == kViewGames ? "Games" : gMP.view == kViewOnline ? "Online" :
         gMP.view == kViewFriends ? "Friends" : kModes[gMP.mode].title, (kWindowLeft + kWindowRight) / 2,
         kWindowTop + 62, 1, 1.0f, 0.85f, 0.32f, alpha);
    Fill(kWindowLeft + 40, kWindowTop + 80, kWindowRight - 40, kWindowTop + 82, 0.84f, 0.67f, 0.34f, alpha * 0.6f);

    for (i = 0; i < gMP.numRows; i++) {
        Row *r = &gMP.rows[i];
        Boolean focused = gMP.column == 1 && gMP.row == i;
        float textY = r->top + (r->detail[0] ? 30 : 32);

        if (focused && r->kind != kRowPlayer)
            Fill(kWindowLeft + 26, r->top + 4, kWindowRight - 26, r->bottom - 4, 1, 0.85f, 0.4f,
                 alpha * (0.12f + 0.08f * Pulse()));
        if (focused)
            Fill(kWindowLeft + 26, r->top + 4, kWindowLeft + 31, r->bottom - 4, 1, 0.85f, 0.4f, alpha);
        switch (r->kind) {
            case kRowHeader:
                Text(gMP.font, 15, r->text, kWindowLeft + 40, r->top + 30, 0, 0.75f, 0.9f, 1.0f, alpha * 0.85f);
                break;
            case kRowInfo:
                Text(gMP.font, 19, r->text, kWindowLeft + 52, textY, 0, 1.0f, 0.88f, 0.55f, alpha * 0.95f);
                if (r->detail[0])
                    Text(gMP.font, 14, r->detail, kWindowLeft + 54, textY + 19, 0, 0.78f, 0.74f, 0.66f, alpha * 0.85f);
                break;
            case kRowSearching:
                n = (int)(SDL_GetTicks() / 400 % 4);
                snprintf(dots, sizeof(dots), "%.*s", n, "...");
                Text(gMP.font, 18, r->text, kWindowLeft + 52, textY, 0, 0.8f, 0.78f, 0.7f, alpha * 0.8f);
                if ((NetBrowsing() || OnlineIsConnecting() || OnlineIsReady()) &&
                    (strncmp(r->text, "Looking", 7) == 0 || strncmp(r->text, "Connecting", 10) == 0))
                    Text(gMP.font, 18, dots, kWindowLeft + 52 + 10 + MMFontRenderWidth(gMP.font, r->text) *
                         (18.0f / (float)MMFontGetFaceSize(gMP.font)), textY, 0, 0.8f, 0.78f, 0.7f, alpha * 0.8f);
                break;
            case kRowPlayer:
                DrawPlayerRow(r, focused, alpha);
                break;
            default:
                Text(gMP.font, 22, r->text, kWindowLeft + 52, textY, 0, focused ? 1.0f : 0.95f, focused ? 0.95f : 0.92f,
                     focused ? 0.6f : 0.84f, alpha);
                if (r->detail[0])
                    Text(gMP.font, 14, r->detail, kWindowLeft + 54, textY + 19, 0, 0.78f, 0.74f, 0.66f, alpha * 0.9f);
                break;
        }
    }
    if (gMP.view == kViewLobby && ModeIsSplit())
        WrappedText(gMP.font, 13,
                    "Controllers: press A on one that isn't playing to add a player. Keyboards: arrows, Space, Return "
                    "and M; or W A S D, F to jump, G for action and Tab for the map.",
                    kWindowLeft + 40, kWindowBottom - 62, kWindowRight - kWindowLeft - 80, 17, 0.75f, 0.72f, 0.64f,
                    alpha * 0.8f);
}

/// The pointer goes over the panels (its layer was drawn before them).
static void DrawCursorOnTop(SpriteWorldPtr w)
{
    SpritePtr spriteP;

    if (!gCustomCursorSpriteLayerP->layerIsVisible)
        return;
    for (spriteP = SWGetNextSprite(gCustomCursorSpriteLayerP, NULL); spriteP != NULL;
         spriteP = SWGetNextSprite(gCustomCursorSpriteLayerP, spriteP))
        if (spriteP->isVisible)
            spriteP->frameDrawProc(spriteP, spriteP->curFrameP, spriteP->drawData->horizLoc, spriteP->drawData->vertLoc);
    (void)w;
}

static void MultiplayerScreenPostDraw(SpriteWorldPtr w)
{
    float alpha;
    Boolean backFocused = gMP.column == 2;

    if (w != gSpriteWorldP || !gMP.active)
        return;
        // the panels come in once the mansion has gone, and go before it comes back
    if (gMP.phase == kPhaseRising)
        alpha = ((float)(SDL_GetTicks() - gMP.phaseStart) / (float)kLiftMs - 0.6f) / 0.4f;
    else if (gMP.phase == kPhaseSinking)
        alpha = 1.0f - (float)(SDL_GetTicks() - gMP.phaseStart) / (float)kLiftMs / 0.35f;
    else
        alpha = 1.0f;
    alpha = alpha < 0 ? 0 : alpha > 1 ? 1 : alpha;
    alpha = alpha * alpha;
    if (alpha <= 0.01f)
        return;

    BeginDesign(w);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    Text(gMP.titleFont, 84, "Multiplayer", 640, 120, 1, 1.0f, 0.85f, 0.32f, alpha);
    DrawModeCards(alpha);
    DrawWindow(alpha);

        // Back, and what the keys do
    WoodPanel(kBackLeft, kBackTop, kBackRight, kBackBottom, backFocused ? 1.0f : 0.7f, alpha * 0.95f);
    if (backFocused && !gMP.pointer)
        Frame(kBackLeft - 3, kBackTop - 3, kBackRight + 3, kBackBottom + 3, 3, 0.86f, 0.68f, 0.30f,
              alpha * (0.65f + 0.35f * Pulse()));
    Text(gMP.font, 24, "Back", (kBackLeft + kBackRight) / 2, kBackTop + 39, 1, 1.0f, 0.95f, 0.8f, alpha);
    Text(gMP.font, 14, "Arrows / D-pad to move    Return / A to choose    Escape / B to go back", 640, 930, 1, 0.8f,
         0.82f, 0.9f, alpha * 0.75f);
    if (gMP.message[0] && SDL_GetTicks() < gMP.messageUntil)
        Text(gMP.font, 18, gMP.message, (kWindowLeft + kWindowRight) / 2, kWindowBottom + 36, 1, 1.0f, 0.75f, 0.5f,
             alpha);

    glColor4f(1, 1, 1, 1);
    glDisable(GL_BLEND);
    EndDesign();
    DrawCursorOnTop(w);
}
