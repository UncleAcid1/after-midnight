/*
 * MMMultiplayer.c — relay co-op for two to four players on one computer, added by this version.
 *
 * Everyone plays the same mansion: doors stay open, keys stay turned and treasure stays taken.
 * When the player whose turn it is dies, the next player comes in as the life is lost (lives
 * are shared). Each player has a character with his own sprites (Jack, Henry, ...), a player
 * profile for lifetime stats, and a share of the loot that is shown when the mansion is beaten.
 *
 * Player sprites live in "Game Data Extras/Sprites/playerN/" with the original file names
 * (00128.png Jack's sheet, 00130.png the save statue, 00131.png caught in a web), so new art
 * can be dropped in. A missing file falls back to player 2's, then to Jack's.
 */

#include "MMGame.h"
#include "MMProtos.h"
#include "BKGL.h"

typedef struct PlayerSprites {
    SpritePtr hero, statue, web;    ///< masters for gSpriteArray[0], [kStatueSprite], [kWebTrappedHeroSprite]
    Boolean loaded;
} PlayerSprites;

typedef struct Multiplayer {
    int     numPlayers;             ///< 1 when not playing together
    int     active;                 ///< whose turn it is
    char    profiles[kMaxPlayers][64];
    int     character[kMaxPlayers]; ///< which character each player plays (kCharacterJack...)
    Boolean split;                  ///< playing at the same time (split screen), not taking turns
    long    loot[kMaxPlayers];      ///< score earned on each player's turns
    long    scoreAtTurnStart;
    Uint32  bannerStart;            ///< "Henry's turn!" fades out from here
    int     input[kMaxPlayers];     ///< co-op on one computer: each player's controls (kSplitInput...)
} Multiplayer;

static Multiplayer gMulti = { 1, 0, { "" }, { 0, 1, 2, 3 }, false, { 0 }, 0, 0, { 0 } };
static PlayerSprites gPlayerSprites[kNumCharacters];     ///< by character

/// Spiders holding a player in their web: which player it was, so it looks right when the
/// room is visited again by someone else.
static struct { Sint32 objectID; short player; } gWebVictims[64];
static int gNumWebVictims = 0;

/// Jack 2 and Henry 2 are placeholders (darker Jack, and Henry's sprites) until their own art exists.
static const char *kCharacterNames[kNumCharacters] = { "Jack", "Henry", "Jack 2", "Henry 2" };

int GetNumPlayers(void)
{
    return gMulti.numPlayers;
}

Boolean IsMultiplayerGame(void)
{
    return gMulti.numPlayers > 1;
}

/// Taking turns (relay co-op), as opposed to playing at the same time.
Boolean IsRelayGame(void)
{
    return gMulti.numPlayers > 1 && !gMulti.split;
}

/// Sets who plays (from the multiplayer screen). profiles may be NULL to keep them.
void SetMultiplayerPlayers(int numPlayers, char profiles[][64], const int *characters, Boolean split)
{
    int p, n = numPlayers < 1 ? 1 : numPlayers > kMaxPlayers ? kMaxPlayers : numPlayers;

    gMulti.numPlayers = n;
    gMulti.split = split && n > 1;
    gMulti.active = 0;
    gActivePlayer = 0;
    for (p = 0; p < n; p++) {
        if (profiles != NULL)
            snprintf(gMulti.profiles[p], sizeof(gMulti.profiles[p]), "%s", profiles[p]);
        if (characters != NULL && characters[p] >= 0 && characters[p] < kNumCharacters)
            gMulti.character[p] = characters[p];
        if (p > 0)
            LoadLifetimeStats((short)p, gMulti.profiles[p]);
    }
}

void SetMultiplayerInputs(const int *inputs)
{
    int p;

    for (p = 0; p < kMaxPlayers; p++)
        gMulti.input[p] = inputs != NULL ? inputs[p] : kSplitInputKeyboard;
}

int GetPlayerInput(int player)
{
    return player >= 0 && player < kMaxPlayers ? gMulti.input[player] : kSplitInputKeyboard;
}

/// Taking turns on this computer: each player's own controls play their turn.
Boolean IsLocalRelayGame(void)
{
    return IsRelayGame() && !NetGameActive();
}

int GetActivePlayer(void)
{
    return gMulti.active;
}

/// Each character's colour for names, trails and map heads: Jack amber, Henry emerald, Jack 2
/// orange, Henry 2 cyan.
const float *GetCharacterColor(int character)
{
    static const float colors[kNumCharacters][3] = {
        { 1.00f, 0.82f, 0.35f }, { 0.45f, 1.00f, 0.80f }, { 1.00f, 0.62f, 0.30f }, { 0.50f, 0.85f, 1.00f }
    };
    return colors[character >= 0 && character < kNumCharacters ? character : 0];
}

/// The character a player plays ("Jack", "Henry", ...).
const char *GetCharacterName(int player)
{
    return player >= 0 && player < kMaxPlayers ? kCharacterNames[gMulti.character[player]] : "";
}

const char *GetCharacterNameByIndex(int character)
{
    return character >= 0 && character < kNumCharacters ? kCharacterNames[character] : "";
}

int GetPlayerCharacter(int player)
{
    return player >= 0 && player < kMaxPlayers ? gMulti.character[player] : 0;
}

void SetPlayerCharacter(int player, int character)
{
    if (player >= 0 && player < kMaxPlayers && character >= 0 && character < kNumCharacters)
        gMulti.character[player] = character;
}

const char *GetPlayerProfileName(int player)
{
    return player >= 0 && player < kMaxPlayers ? gMulti.profiles[player] : "";
}

/// What to call a player on screen: their profile name, or their character's without one.
const char *GetPlayerDisplayName(int player)
{
    if (player < 0 || player >= kMaxPlayers)
        return "";
    return gMulti.profiles[player][0] != 0 ? gMulti.profiles[player] : kCharacterNames[gMulti.character[player]];
}

/// Back to one player (anything started from the title screen but Multiplayer).
void SetSinglePlayer(void)
{
    char name[256];

    SplitCancel();
    gMulti.numPlayers = 1;
    gMulti.split = false;
    gMulti.active = 0;
    gActivePlayer = 0;
    gMulti.character[0] = kCharacterJack;
    if (gCurrentPlayerNameExists) {
        SWCopyPascalStringToCString((ConstStringPtr)gCurrentPlayerName, name);
        snprintf(gMulti.profiles[0], sizeof(gMulti.profiles[0]), "%s", name);
    }
}

// ---------------------------------------------------------------------------
// Sprites

/// A character's sprite file: its own, then (for Henry 2) Henry's; NULL means use Jack's.
static const char *CharacterSpritePath(int character, const char *file)
{
    char rel[200];
    const char *path;
    int c;

    for (c = character; c >= 1; c = (c == kCharacterHenry2) ? kCharacterHenry : 0) {
        snprintf(rel, sizeof(rel), "Sprites/player%d/%s", c + 1, file);
        path = GetExtrasFilePath(rel);
        if (path != NULL)
            return path;
    }
    return NULL;
}

static void LoadCharacterSprites(int character)
{
    PlayerSprites *set = &gPlayerSprites[character];
    const char *path;

    if (set->loaded)
        return;
    set->loaded = true;
    set->hero = gSpriteArray[0];
    set->statue = gSpriteArray[kStatueSprite];
    set->web = gSpriteArray[kWebTrappedHeroSprite];
    if (character == kCharacterJack)
        return;
        // anything that can't be loaded stays Jack's
    path = CharacterSpritePath(character, "00128.png");
    if (path != NULL && SWCreateSpriteFromSingleFileXY(&set->hero, NULL, path, 128, 128, 0, 0, false, 84, true) == kNoError)
        MySetSpriteScaledSize(set->hero, 64, 64);
    else
        set->hero = gSpriteArray[0];
    path = CharacterSpritePath(character, "00130.png");
    if (path != NULL && SWCreateSpriteFromSingleFileXY(&set->statue, NULL, path, 128, 128, 0, 0, false, 2, true) == kNoError)
        MySetSpriteScaledFactor(set->statue, 0.5f, 0.5f);
    else
        set->statue = gSpriteArray[kStatueSprite];
    path = CharacterSpritePath(character, "00131.png");
    if (path != NULL && SWCreateSpriteFromSingleFile(&set->web, NULL, path, 112, 0, true) == kNoError)
        MySetSpriteScaledFactor(set->web, 0.5f, 0.5f);
    else
        set->web = gSpriteArray[kWebTrappedHeroSprite];
}

/// A character's hero sprite sheet (loaded on first use), for drawing other players.
SpritePtr GetCharacterHeroMaster(int character)
{
    if (character < 0 || character >= kNumCharacters)
        character = kCharacterJack;
    LoadCharacterSprites(kCharacterJack);
    LoadCharacterSprites(character);
    return gPlayerSprites[character].hero;
}

/// Puts a player's sprites where the game makes Jack from.
static void UsePlayerSprites(int player)
{
    int c = gMulti.character[player];

    LoadCharacterSprites(kCharacterJack);   // remembers Jack's (which a mansion may have replaced)
    LoadCharacterSprites(c);
    gSpriteArray[0] = gPlayerSprites[c].hero;
    gSpriteArray[kStatueSprite] = gPlayerSprites[c].statue;
    gSpriteArray[kWebTrappedHeroSprite] = gPlayerSprites[c].web;
}

/// Before a mansion is opened while a game is being set up (continuing a saved game): the
/// mansion loader replaces (and frees) what's in Jack's places, so Jack's own sprites go back
/// there and the other characters' sets are let go (they're made again from their files).
void MultiplayerReleaseCharacterSprites(void)
{
    PlayerSprites *jack = &gPlayerSprites[kCharacterJack];
    int c;

    if (jack->loaded) {
        gSpriteArray[0] = jack->hero;
        gSpriteArray[kStatueSprite] = jack->statue;
        gSpriteArray[kWebTrappedHeroSprite] = jack->web;
    }
    for (c = 0; c < kNumCharacters; c++) {
        PlayerSprites *set = &gPlayerSprites[c];
        if (!set->loaded || c == kCharacterJack)
            continue;
        if (set->hero != NULL && set->hero != jack->hero && set->hero != gSpriteArray[0])
            SWDisposeSprite(&set->hero);
        if (set->statue != NULL && set->statue != jack->statue && set->statue != gSpriteArray[kStatueSprite])
            SWDisposeSprite(&set->statue);
        if (set->web != NULL && set->web != jack->web && set->web != gSpriteArray[kWebTrappedHeroSprite])
            SWDisposeSprite(&set->web);
        set->loaded = false;
    }
    jack->loaded = false;
}

/// The mansion of a continued game is open: the player whose turn it is gets their sprites.
void MultiplayerMansionOpened(void)
{
    if (IsMultiplayerGame())
        UsePlayerSprites(gMulti.active);
}

/// When a game ends: Jack's sprites go back, and Jack's set is forgotten (the next mansion
/// may bring its own).
void MultiplayerGameEnded(void)
{
    if (gPlayerSprites[kCharacterJack].loaded) {
        int c = gMulti.character[0];
        gMulti.character[0] = kCharacterJack;
        UsePlayerSprites(0);
        gMulti.character[0] = c;
        gPlayerSprites[kCharacterJack].loaded = false;
    }
    gNumWebVictims = 0;
}

void MultiplayerNoteWebVictim(SpritePtr spiderP)
{
    Sint32 id = GAME_SPRITE(spiderP)->objectID;
    int i;

    if (!IsRelayGame())
        return;
    for (i = 0; i < gNumWebVictims; i++)
        if (gWebVictims[i].objectID == id)
            break;
    if (i == gNumWebVictims) {
        if (gNumWebVictims >= (int)SDL_arraysize(gWebVictims))
            return;
        gNumWebVictims++;
    }
    gWebVictims[i].objectID = id;
    gWebVictims[i].player = (short)gMulti.character[gMulti.active];
}

/// The player caught in this spider's web: that player's look, whoever is playing now.
SpritePtr NewWebTrappedHeroForSpider(SpritePtr spiderP)
{
    Sint32 id = GAME_SPRITE(spiderP)->objectID;
    SpritePtr saveP = gSpriteArray[kWebTrappedHeroSprite], trappedP;
    int i;

    for (i = 0; i < gNumWebVictims; i++) {
        if (gWebVictims[i].objectID == id && gPlayerSprites[gWebVictims[i].player].loaded) {
            gSpriteArray[kWebTrappedHeroSprite] = gPlayerSprites[gWebVictims[i].player].web;
            break;
        }
    }
    trappedP = NewWebTrappedSprite(kWebTrappedHeroSprite);
    gSpriteArray[kWebTrappedHeroSprite] = saveP;
    return trappedP;
}

// ---------------------------------------------------------------------------
// Split screen (MMSplit.c): every player runs a copy of the game

/// A game on several computers: everyone in it, in the host's order (each player has their own
/// profile on their own computer).
void MultiplayerSetNetworkPlayers(void)
{
    int p, n = NetNumPlayers();

    gMulti.numPlayers = n < 2 ? 2 : n > kMaxPlayers ? kMaxPlayers : n;
    gMulti.split = false;
    for (p = 0; p < gMulti.numPlayers; p++) {
        snprintf(gMulti.profiles[p], sizeof(gMulti.profiles[p]), "%s", NetPlayerName(p));
        gMulti.character[p] = NetPlayerCharacter(p);
    }
}

/// A race on several computers (host and guests, once everyone's in): this computer's player
/// here, the others as ghosts.
void MultiplayerSetUpNetworkRace(void)
{
    int me = NetLocalPlayer();

    MultiplayerSetNetworkPlayers();
    gMulti.split = true;
    gMulti.active = me;
    SplitConfigureNetworkRace(me, gMulti.numPlayers);
}

/// The character sprites are loaded once for all players' games.
void MultiplayerShareState(void (*share)(void *, size_t))
{
    share(gPlayerSprites, sizeof(gPlayerSprites));
}

/// In a player's own game as it starts: this game is that player's.
void MultiplayerSplitPlayerStarting(int player)
{
    gMulti.active = player;
    gActivePlayer = (short)player;
    gMulti.scoreAtTurnStart = gScore;
    UsePlayerSprites(player);
    if (gStatsSpriteWorldP != NULL)
        SWSetPostDrawCallBack(gStatsSpriteWorldP, ExtrasPostDraw);
}

/// Back in the first player's game after split screen.
void MultiplayerSplitEnded(void)
{
    gMulti.active = 0;
    gActivePlayer = 0;
}

// ---------------------------------------------------------------------------
// Turns

/// Called when a new game starts.
void MultiplayerGameStarted(void)
{
    int i;

    NetGameStarted();       // two computers: the host tells the other which mansion, and the seed
    for (i = 0; i < kMaxPlayers; i++)
        gMulti.loot[i] = 0;
    gMulti.active = 0;
    gActivePlayer = 0;
    gMulti.scoreAtTurnStart = gScore;
    gNumWebVictims = 0;
    if (IsMultiplayerGame()) {
        UsePlayerSprites(0);
        gMulti.bannerStart = SDL_GetTicks();
    }
    if (gStatsSpriteWorldP != NULL)
        SWSetPostDrawCallBack(gStatsSpriteWorldP, ExtrasPostDraw);
}

long GetPlayerLoot(int player)
{
    return player >= 0 && player < kMaxPlayers ? gMulti.loot[player] : 0;
}

/// Continues a saved multiplayer game (relay co-op, on this computer or two): whose turn it
/// was, everyone's character and share of the loot, matched by player ID; then the game.
/// Returns 0, or the restore's error.
int MultiplayerContinueSave(const char *path)
{
    MultiSaveInfo info;
    char ids[kMaxPlayers][16];
    int p, i, err, active = 0;

    if (!MultiSaveReadInfo(path, &info))
        return -1;
    {
            // the mansion it was saved in, before the game is announced (two computers: the
            // other one opens the same mansion)
        SDL_RWops *file = SDL_RWFromFile(path, "rb");
        float h, v;
        Boolean isSaveFile;
        long version;
        if (file == NULL)
            return -1;
        err = LoadSaveDataFromFile(file, &h, &v, &isSaveFile, &version);
        SDL_RWclose(file);
        if (err != 0)
            return err;
        SetCurMansionAsCustomMansion(gCurrentMansionNumber == -1);
    }
    MultiplayerGameStarted();       // two computers: the host sends the save over
    for (p = 0; p < gMulti.numPlayers; p++) {
        if (NetIsConnected())
            snprintf(ids[p], 16, "%s", NetPlayerID(p));
        else
            snprintf(ids[p], 16, "%s", OnlineProfileID(gMulti.profiles[p]));
        for (i = 0; i < info.numPlayers; i++)
            if (strcmp(info.ids[i], ids[p]) == 0) {
                gMulti.loot[p] = info.loot[i];
                if (info.characters[i] >= 0 && info.characters[i] < kNumCharacters)
                    gMulti.character[p] = info.characters[i];
            }
        if (strcmp(ids[p], info.turnID) == 0)
            active = p;
    }
    gMulti.active = active;
    gActivePlayer = active;
    MultiplayerReleaseCharacterSprites();     // the restore opens the mansion (MultiplayerMansionOpened)
    MMHideCursor();
    DoGameFadeToBlackFullAnim(30, true);
    if (gCurrentGameMode == kTitleScreenMode)
        ExitTitleScreen();
    err = MMRestoreFromSaveGameFile(path, info.name);
    gMulti.scoreAtTurnStart = gScore;
    gMulti.bannerStart = SDL_GetTicks();
    if (err != 0) {
        CleanUpAfterGame();
        SetUpTitleScreen();
        if (err != -11111 && err != -22222)
            MMDoErrorMessage("Sorry, but there was an error when trying to open the saved game (error code %d).", err);
    }
    return err;
}

static void CreditLoot(void)
{
    gMulti.loot[gMulti.active] += gScore - gMulti.scoreAtTurnStart;
    gMulti.scoreAtTurnStart = gScore;
}

/// Jack died: the next player's turn. Called just before the hero reappears.
void MultiplayerNextTurn(void)
{
    if (!IsRelayGame())
        return;
    CreditLoot();
    gMulti.active = (gMulti.active + 1) % gMulti.numPlayers;
    gActivePlayer = (short)gMulti.active;
    ControllerTurnChanged();        // the last player's buttons aren't held by the next one
    UsePlayerSprites(gMulti.active);
    gMulti.bannerStart = SDL_GetTicks();
    NetTurnChanged();
}

/// "Ann: 54% | Bob: 46%" for the mansion complete screen (empty when playing alone). The
/// percentages are rounded so they add up to 100.
void GetLootSplitText(char *text, size_t size)
{
    long total = 0, share[kMaxPlayers], remainder[kMaxPlayers];
    int percent[kMaxPlayers], i, best, left = 100, used = 0;

    text[0] = 0;
    if (!IsRelayGame())
        return;
    CreditLoot();
    for (i = 0; i < gMulti.numPlayers; i++) {
        share[i] = gMulti.loot[i] > 0 ? gMulti.loot[i] : 0;
        total += share[i];
    }
    for (i = 0; i < gMulti.numPlayers; i++) {
        percent[i] = total > 0 ? (int)(share[i] * 100 / total) : 100 / gMulti.numPlayers;
        remainder[i] = total > 0 ? share[i] * 100 % total : 0;
        left -= percent[i];
    }
    while (left > 0 && total > 0) {         // largest remainders get the leftover points
        for (i = 1, best = 0; i < gMulti.numPlayers; i++)
            if (remainder[i] > remainder[best])
                best = i;
        percent[best]++;
        remainder[best] = -1;
        left--;
    }
    for (i = 0; i < gMulti.numPlayers && used < (int)size; i++)
        used += snprintf(text + used, size - used, "%s%s: %d%%", i > 0 ? " | " : "", GetPlayerDisplayName(i), percent[i]);
}

// ---------------------------------------------------------------------------
// Setup (Multiplayer on the title screen)

/// Two computers: hosts (and waits for the other computer) or joins (address NULL: ask for
/// it). netMode is what the host offers; a joiner learns it from the host. Returns
/// kMultiplayerCancelled, kMultiplayerNewGame (the host picks a mansion as usual) or
/// kMultiplayerJoined (the host chose the mansion; it is open and ready to start).
static int AfterConnected(int netMode, Boolean host);

int MultiplayerConnect(int netMode, Boolean host, const char *address)
{
    if (host) {
        NetSetMode(netMode);
        if (!NetHostGame())
            return kMultiplayerCancelled;
    } else if (address != NULL ? !NetJoinGameAt(address) : !NetJoinGame()) {
        return kMultiplayerCancelled;
    }
    return AfterConnected(netMode, host);
}

/// Over the internet, through the lobby server: hosts a public or private game, or joins one
/// by its code (or a friend's by their ID).
int MultiplayerConnectOnline(int netMode, Boolean host, Boolean isPublic, const char *code, const char *hostID)
{
    if (host) {
        NetSetMode(netMode);
        if (!NetHostGameOnline(isPublic))
            return kMultiplayerCancelled;
    } else if (!NetJoinGameOnline(code, hostID)) {
        return kMultiplayerCancelled;
    }
    return AfterConnected(netMode, host);
}

static int AfterConnected(int netMode, Boolean host)
{
    char name[256];

    SWCopyPascalStringToCString((ConstStringPtr)gCurrentPlayerName, name);
    (void)name;
    MultiplayerSetNetworkPlayers();
    if (host) {
        if (netMode == kNetModeRace)
            MultiplayerSetUpNetworkRace();
        return kMultiplayerNewGame;
    }
    if (!NetClientWaitForStart()) {
        SetSinglePlayer();
        return NetMissingMansion() != NULL ? kMultiplayerNoMansion : kMultiplayerCancelled;
    }
    SetEasyJumpingMode(gEasyJumping);
    return kMultiplayerJoined;
}

/// Joining failed for want of the host's mansion: says which one, and where to install it.
void ShowMissingMansionMessage(void)
{
    char message[1400], folder[1024];
    size_t len;

    snprintf(folder, sizeof(folder), "%s", GetCustomMansionsFolderPath(""));
    while ((len = strlen(folder)) > 1 && folder[len - 1] == '/')
        folder[len - 1] = '\0';
    snprintf(message, sizeof(message),
             "You don't have the mansion \"%s\" installed, so you can't join this game. Install it (put it in "
             "your Custom Mansions folder, %s) and try again.",
             NetMissingMansion() != NULL ? NetMissingMansion() : "?", folder);
    MMDoCustomDialog(gSpriteWorldP, message, 3);
}

/// Asks how many play and who each one is. Player 1 is the current profile.
/// Returns kMultiplayerCancelled, kMultiplayerNewGame (pick a mansion as usual) or
/// kMultiplayerJoined (another computer chose the mansion; it is open and ready to start).
/// (The old dialog; the multiplayer screen replaces it unless MM_OLD_MULTIPLAYER_DIALOG is set.)
int DoMultiplayerSetupDialog(void)
{
    static const char *kChoices[] = {
        "Two players on this computer", "Three players on this computer", "Four players on this computer",
        "Two computers: host a game", "Two computers: join a game"
    };
    char chosen[kMaxPlayers][64], prompt[200], name[256];
    int count, p, choice;

    choice = DoSimpleChoiceDialog("Multiplayer", kChoices, 5, 0);
    if (choice < 0)
        return kMultiplayerCancelled;
    SWCopyPascalStringToCString((ConstStringPtr)gCurrentPlayerName, name);
    if (choice >= 3)
        return MultiplayerConnect(kNetModeRelay, choice == 3, NULL);
    count = choice + 2;
    snprintf(chosen[0], sizeof(chosen[0]), "%s", name);
    for (p = 1; p < count; p++) {
        snprintf(prompt, sizeof(prompt), "Who is Player %d (%s)?", p + 1, kCharacterNames[gMulti.character[p]]);
        if (!DoChooseOtherPlayerDialog(prompt, chosen, p, chosen[p], sizeof(chosen[p])))
            return kMultiplayerCancelled;
    }
    gMulti.numPlayers = count;
    for (p = 0; p < count; p++) {
        snprintf(gMulti.profiles[p], sizeof(gMulti.profiles[p]), "%s", chosen[p]);
        if (p > 0)
            LoadLifetimeStats((short)p, chosen[p]);
    }
    return kMultiplayerNewGame;
}

// ---------------------------------------------------------------------------
// On screen: "Player 2" in the status bar, whose turn it is, and a banner when turns change

static SpritePtr MakeLabel(const char *fontName, short size, const unsigned char rgb[3])
{
    SpritePtr textP = NULL;

    if (SWCreateTextSprite(&textP, " ", fontName, size, 0, rgb, true) != kNoError)
        return NULL;
    return textP;
}

static void DrawLabel(SpritePtr textP, const char *text, short left, short top, short right, short bottom,
                      short justification, unsigned long alpha)
{
    SWRect rect;

    if (textP == NULL)
        return;
    SetTextSpriteText(textP, text);
    SWSetRect(&rect, left, top, right, bottom);
    SetTextSpriteRect(textP, &rect, justification, false);
    SWSetSpriteTranslucencyLevel(textP, alpha);
    SWTextSpriteDrawProc(textP, textP->curFrameP, 0, 0);
}

/// The status bar: "Player 2" right above the score.
void MultiplayerDrawStatusBar(SpriteWorldPtr w)
{
    static const unsigned char gold[3] = { 0xf4, 0xe0, 0x7a };
    static SpritePtr labelP = NULL;
    char text[32];

    (void)w;
    if (!IsMultiplayerGame() || (gCurrentGameMode != kPlayingGameMode && gCurrentGameMode != kReadingSignMode))
        return;
    if (labelP == NULL)
        labelP = MakeLabel("BlackChancery", 11, gold);
    snprintf(text, sizeof(text), "Player %d", gMulti.active + 1);
    DrawLabel(labelP, text, 480, 0, 632, 12, 1, 0xffff);
}

/// The play area: "It is Henry's turn" at the bottom right, and a banner after a turn changes.
void MultiplayerDrawPlayArea(SpriteWorldPtr w)
{
    static const unsigned char white[3] = { 0xe8, 0xe8, 0xe8 };
    static const unsigned char yellow[3] = { 0xff, 0xff, 0x5f };
    static SpritePtr turnP = NULL, bannerP = NULL;
    static Boolean turnIsLarge = false;
    char text[160];
    const char *who;
    Uint32 elapsed;
    unsigned long alpha;
    short width = w->visScrollRect.right - w->visScrollRect.left;
    short height = w->visScrollRect.bottom - w->visScrollRect.top;

    if (!IsRelayGame() || gCurrentGameMode != kPlayingGameMode || gBeatTheMansion || gGameOver)
        return;
    if (turnP != NULL && turnIsLarge != gExtras.largeText) {        // "Larger text" changed
        SWDisposeSprite(&turnP);
        turnP = NULL;
    }
    if (turnP == NULL) {
        turnP = MakeLabel("Action Man", gExtras.largeText ? 15 : 9, white);
        turnIsLarge = gExtras.largeText;
    }
    if (bannerP == NULL)
        bannerP = MakeLabel("Action Man", 30, yellow);
    who = GetPlayerDisplayName(gMulti.active);
    if (NetIsWaitingLong())
        snprintf(text, sizeof(text), "Waiting for %s...", who);
    else
        snprintf(text, sizeof(text), "It is %s's turn", who);
    DrawLabel(turnP, text, 0, (short)(height - (turnIsLarge ? 22 : 14)), (short)(width - 6), (short)(height - 4), 1,
              0xd000);

    elapsed = SDL_GetTicks() - gMulti.bannerStart;
    if (elapsed < 2600) {
        alpha = elapsed < 2000 ? 0xffff : (unsigned long)(0xffff * (2600 - elapsed) / 600);
        snprintf(text, sizeof(text), "%s's turn!", who);
        DrawLabel(bannerP, text, 0, (short)(height / 2 - 40), width, (short)(height / 2), 2, alpha);
    }
}
