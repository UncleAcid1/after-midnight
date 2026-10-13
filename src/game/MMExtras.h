/*
 * MMExtras.h — what this version adds to the original game: the Fun and Extras settings,
 * lifetime statistics (the Trophy Room), the second player, and the artwork/sounds that
 * come with them (the "Game Data Extras" folder, see tools/make_extras.py).
 */
#ifndef MMEXTRAS_H
#define MMEXTRAS_H

#include "MMGame.h"

// ---------------------------------------------------------------------------
// Settings

typedef struct ExtrasSettings {
    // Extras tab
    Boolean arcadeMenu;         ///< title menu centered, arcade-cabinet style
    Boolean smoothRooms;        ///< slide between rooms instead of cutting
#ifdef MM_PS3
    Boolean tvSafe;             ///< the picture a little smaller, for a TV that crops its edges
#endif
    Boolean rumble;             ///< controller rumble
    Boolean multiplayerSmoke;   ///< multiplayer: a cloud of smoke where a player dies
    // Fun tab
    Boolean medusaLate;         ///< "Medusa's Late for Work": fast Medusa heads
    Boolean piracyMedusas;      ///< "You Wouldn't Download a Mansion"
    Boolean tooScared;          ///< "I'm Too S-S-Scared": no Medusas, eyeballs, spiders, skeletons
    Boolean myTuba;             ///< "My Tuba!" instead of the laugh when starting a game
    Boolean nineLives;          ///< "Nine-Lived Malone": infinite lives in beaten mansions
    Boolean espresso;           ///< "Jack on Espresso": faster Jack in beaten mansions
    Boolean pinata;             ///< "Jack's Pinata Party": confetti when Jack dies
    Boolean moonGravity;        ///< "Jackie, Watch the Stars": slow falls
    Boolean allSpiders;         ///< "Oops! All Spiders"
    Boolean pitchBlack;         ///< "Pitch Black": only a circle of light around Jack
} ExtrasSettings;

extern ExtrasSettings gExtras;

typedef struct ExtrasOption {
    const char *key;            ///< name in the settings file
    const char *label;          ///< checkbox text
    const char *description;    ///< shown in the box under the options while hovered
    Boolean    *valueP;
} ExtrasOption;

enum { kExtrasTabFun = 0, kExtrasTabExtras = 1 };

/// The options shown on a tab; returns how many.
int  GetExtrasOptions(int tab, const ExtrasOption **optionsP);
void LoadExtrasSettings(void);
void SaveExtrasSettings(void);
void ExtrasSettingsChanged(void);

// MMExtrasUI.c
SpritePtr NewSolidBoxSprite(SpriteLayerPtr layerP, const SWRect *rectP, unsigned short red, unsigned short green,
                            unsigned short blue, unsigned long alpha);
void SetSolidBoxRect(SpritePtr boxP, const SWRect *rectP);
SpritePtr NewFramedBox(SpriteLayerPtr layerP, const SWRect *rectP, short border, unsigned long alpha,
                       SpritePtr *frameP);
short GetExtrasOptionButtonIndex(int tab, int option);
void AddExtrasItemsToOptionsDialog(SpriteLayerPtr layerP, SWRect *dialogRectP, int tab);
void UpdateExtrasOptionDescription(SpriteLayerPtr layerP, int tab);
void AddOptionsPaneTitle(SpriteLayerPtr layerP, SWRect *dialogRectP, const char *title, float scale);
void LayOutTitleScreenButtons(void);
void SmoothTransitionCaptureOldRoom(void);
void SmoothTransitionToNewRoom(int dirH, int dirV);

/// Rows in extras/Screens/titleScreen/buttons_extra.png: the original seven, then Custom
/// Mansions, Random Mansion, Trophy Room, Multiplayer.
#define kNumTitleButtonRows 11
extern Boolean gTitleScreenHasExtraButtons;

/// A file this version adds, e.g. "Sounds/my_tuba.ogg", or NULL if it isn't installed.
const char *GetExtrasFilePath(const char *relativePath);
/// The folder holding pre-installed custom mansions (may not exist).
const char *GetBundledCustomMansionsPath(void);

// ---------------------------------------------------------------------------
// Lifetime statistics (per player profile)

enum {
    kDeathByLava, kDeathByZapper, kDeathByMedusa, kDeathByFall, kDeathBySpider, kDeathBySkeleton,
    kDeathByEyeball, kDeathByCreature, kDeathByOther, kNumDeathCauses
};

typedef struct LifetimeStats {
    char        playerName[256];
    double      pixelsFallen;           ///< hero pixels fallen (room coordinates)
    long        keysTurned;
    long        doorsOpened;
    long        deaths[kNumDeathCauses];
    long        coinsCollected;
    long        gamesStarted;
    long        mansionsBeaten;
    long        secondsPlayed;
    unsigned char looted[3][16];        ///< base mansion n fully looted on difficulty d
    unsigned char beaten[3][16];        ///< base mansion n beaten on difficulty d (also in the profile)
    char        beatenCustom[2048];     ///< names of beaten custom mansions, separated by '\n'
} LifetimeStats;

/// One per player (relay co-op). gActivePlayer is whoever is playing now.
#define kMaxPlayers 4
extern LifetimeStats gStats[kMaxPlayers];
extern short gActivePlayer;

void LoadLifetimeStats(short player, const char *playerName);
void SaveLifetimeStats(short player);
LifetimeStats *ActiveStats(void);

const char *GetDeathCauseName(int cause);
double PixelsToMiles(double pixels);

/// Hooks called by the game.
void StatsNoteDeath(int cause);
void StatsNoteFall(float pixels);
void StatsNoteKeyTurned(void);
void StatsNoteCoin(void);
void StatsNoteGameStarted(void);
void StatsNoteMansionBeaten(void);
void StatsNoteSecondsPlayed(long seconds);
int  DeathCauseForEnemy(SpritePtr enemyP);

/// True if the current player has beaten the mansion being played (any difficulty).
Boolean HasBeatenCurrentMansion(void);
Boolean HasPlayerBeatenCustomMansion(const char *name);
Boolean HasPlayerBeatenCustomMansionStats(LifetimeStats *s, const char *name);
void SaveAllLifetimeStats(void);
/// The standard mansions (Episodes 1 and 2), not counting the Hint House.
#define kNumBaseMansions 10

// ---------------------------------------------------------------------------
// Fun tab hooks

/// Called for each sprite record as a room loads. Returns false to leave it out; may change it.
Boolean ExtrasFilterRoomSprite(SPRTRecord *recP);
/// Called once a room has finished loading.
void ExtrasRoomLoaded(void);
/// Called every game frame while playing.
void ExtrasGameFrame(void);
/// Called when the hero dies.
void ExtrasHeroDied(SpritePtr heroP);
/// Called when the hero is about to lose a life. Returns true if the life is kept.
Boolean ExtrasKeepLife(void);
float ExtrasWalkSpeedFactor(void);
float ExtrasFallGravityFactor(void);
Boolean ExtrasFallsAreSafe(void);
float ExtrasMedusaSpeedFactor(void);
/// The sound played when starting a new game from the title screen.
void ExtrasPlayNewGameSound(void);

// Extra sounds (indexes after the original sounds)
enum { kExtraSoundMyTuba = 0, kExtraSoundDice, kExtraSoundBell, kNumExtraSounds };
void MMPlayExtraSound(int which);
short GetExtraSoundIndex(int which);

// ---------------------------------------------------------------------------
// Controllers (MMController.c)

void InitControllers(void);
Boolean HandleControllerEvent(SDL_Event *event);
void ControllerFrame(void);
Boolean IsControllerConnected(void);
int  GetNumControllers(void);
/// Light tap, heavy door rumble, sharp jolt.
enum { kRumbleTap, kRumbleHeavy, kRumbleJolt };
void Rumble(int kind);
/// Virtual cursor for menus driven by a controller: true while it replaces the mouse.
Boolean ControllerOverridesMouse(SWPoint *pointP, Boolean *buttonP);
/// Keyboard-like menu input from the controller, for screens that read keys.
enum { kPadUp = 1, kPadDown = 2, kPadLeft = 4, kPadRight = 8, kPadAccept = 16, kPadBack = 32, kPadAlt = 64,
       kPadAlt2 = 128, kPadPageUp = 256, kPadPageDown = 512 };
/// Menu buttons pressed since the last call (each press reported once).
int  TakeControllerMenuPresses(void);
SpritePtr GetControllerFocusSprite(void);
void ControllerSetListMode(Boolean on);
void ExtrasPostDraw(SpriteWorldPtr w);
/// Set while the game renders something it keeps (a room's background texture).
extern Boolean gSuppressExtrasOverlay;
/// Set when X (Square) is pressed in a menu; screens with a secondary action clear it.
extern Boolean gControllerMenuAlt;
extern Boolean gControllerMenuAlt2;     // Y (Triangle) in a menu

// Multiplayer (MMMultiplayer.c)
int  GetNumPlayers(void);
Boolean IsMultiplayerGame(void);
Boolean IsRelayGame(void);
void SetMultiplayerPlayers(int numPlayers, char profiles[][64], const int *characters, Boolean split);
void MultiplayerSplitPlayerStarting(int player);
void MultiplayerSplitEnded(void);
int  GetActivePlayer(void);
/// Characters players can choose. Jack 2 and Henry 2 are placeholders until they get their own art.
enum { kCharacterJack, kCharacterHenry, kCharacterJack2, kCharacterHenry2, kNumCharacters };
const char *GetCharacterName(int player);
const char *GetCharacterNameByIndex(int character);
int  GetPlayerCharacter(int player);
void SetPlayerCharacter(int player, int character);
const char *GetPlayerProfileName(int player);
const char *GetPlayerDisplayName(int player);
const float *GetCharacterColor(int character);
void SetSinglePlayer(void);
enum { kMultiplayerCancelled = 0, kMultiplayerNewGame = 1, kMultiplayerJoined = 2, kMultiplayerNoMansion = 3 };
int  DoMultiplayerSetupDialog(void);
int  MultiplayerConnect(int netMode, Boolean host, const char *address);
int  MultiplayerConnectOnline(int netMode, Boolean host, Boolean isPublic, const char *code, const char *hostID);
void ShowMissingMansionMessage(void);
// The multiplayer screen (MMMultiplayerScreen.c)
void EnterMultiplayerScreen(void);
void EnterMultiplayerScreenAgain(void);
void HandleMultiplayerScreenCallback(void);
Boolean IsMultiplayerScreenActive(void);
SDL_JoystickID ControllerLastPadID(void);
void MultiplayerGameStarted(void);
void MultiplayerGameEnded(void);
void MultiplayerNextTurn(void);
void MultiplayerNoteWebVictim(SpritePtr spiderP);
SpritePtr NewWebTrappedHeroForSpider(SpritePtr spiderP);
void GetLootSplitText(char *text, size_t size);
void MultiplayerDrawStatusBar(SpriteWorldPtr w);
void MultiplayerDrawPlayArea(SpriteWorldPtr w);

// Split screen (MMSplit.c)
enum { kSplitModeNone, kSplitModeCoop, kSplitModeRace };
/// Who plays with what. Pads are counted in the order the controllers were connected.
enum { kSplitInputKeyboard, kSplitInputKeyboardLeft, kSplitInputPad1, kSplitInputPad2, kSplitInputPad3,
       kSplitInputPad4, kNumSplitInputs };
typedef struct SplitOtherPlayer {
    int     player;
    short   room;               ///< level number
    float   x, y;
    short   frame;
    Boolean flipped;
    float   scaleW, scaleH;
    Boolean visible;
} SplitOtherPlayer;
void SplitConfigure(int mode, int numPlayers, const int *inputs);
void SplitConfigureNetworkRace(int localPlayer, int numPlayers);
void SplitCancel(void);
Boolean SplitScreenPending(void);
Boolean SplitScreenActive(void);
int  SplitScreenMode(void);
int  SplitCurrentPlayer(void);
void SplitScreenBegin(short levelNumber);
void SplitRunFrame(void);
Boolean SplitPumpPlayerEvents(void);
Boolean SplitGiveOSTime(void);
Boolean SplitTakeGameEnd(void);
void SplitNoteDeath(void);
int  SplitGetOtherPlayers(SplitOtherPlayer *others, int max);
void SplitDrawOtherPlayers(SpriteWorldPtr w);
void SplitSaveTeam(void);
int  SplitContinueSave(const char *path);

// Split screen co-op: what one player does happens in everyone's game (MMCoopSync.c)
void CoopSyncShareState(void (*share)(void *, size_t));
void CoopSyncReset(void);
void CoopSyncApply(void);
void CoopPostLeverPulled(SpritePtr leverP);
void CoopPostSwitchPressed(SpritePtr switchP);
void CoopPostDoorUnlocked(SpritePtr doorP);
void CoopPostEnemyKilled(SpritePtr enemyP);
void CoopPostPlatformFalling(SpritePtr platformP);
short GetMapPlayerLevel(void);
SpritePtr GetCharacterHeroMaster(int character);
int  ControllerIndexForInstanceID(SDL_JoystickID id);
Boolean DebugVirtualPadKey(const SDL_Event *event);

// Two computers (MMNet.c)
enum { kNetModeRelay = 0, kNetModeRace = 1 };
/// The race: where a player is and how they're doing (sent every frame).
typedef struct NetRaceState {
    short   room, mapRoom;
    float   x, y, scaleW, scaleH;
    short   frame;
    Boolean flipped, visible;
    Uint8   status;             ///< kSplitPlayer... (MMSplit.c): starting, playing, finished, out
    Uint8   deaths, place;
    Uint32  elapsedMs;
} NetRaceState;
/// A game found on the local network.
typedef struct NetFoundGame {
    char    hostName[64];
    char    address[64];
    int     mode;
    int     character;
    Uint32  token;
    Uint32  lastSeen;
    int     numPlayers;         ///< in the host's waiting room so far
} NetFoundGame;
Boolean NetRaceActive(void);
int  NetMode(void);
void NetSetMode(int mode);
Boolean NetJoinGameAt(const char *address);
void NetRaceSendState(const NetRaceState *st);
Boolean NetRaceGetState(int player, NetRaceState *st, Boolean *goneP);
void NetRacePump(void);
int  NetNumPlayers(void);
const char *NetPlayerName(int player);
const char *NetPlayerID(int player);
int  NetPlayerCharacter(int player);
Boolean NetPlayerLeft(int player);
int  NetMissingMansionPlayer(void);
void MultiplayerSetNetworkPlayers(void);
void NetBrowseStart(void);
void NetBrowseStop(void);
int  NetBrowseGames(NetFoundGame *games, int max);
Boolean NetBrowsing(void);
const char *NetMissingMansion(void);
const char *NetPeerMissingMansion(void);
void MultiplayerSetUpNetworkRace(void);
Boolean NetHostGameOnline(Boolean isPublic);
Boolean NetJoinGameOnline(const char *code, const char *hostID);
Boolean NetIsOnline(void);
void NetSendChat(const char *text);

// Playing over the internet: IDs, friends, the lobby server (MMOnline.c)
#define kMaxOnlineGames     24
#define kMaxOnlineFriends   64
typedef struct OnlineGame {
    char    code[32];
    char    hostName[64];
    char    hostID[16];
    int     mode;               ///< kNetModeRelay or kNetModeRace
    Boolean isPublic, isFriend;
    int     character;
    int     numPlayers;         ///< in the host's waiting room so far
} OnlineGame;
typedef struct OnlineFriend {
    char    id[16];
    char    name[64];
    Uint32  lastPlayed;
} OnlineFriend;
typedef struct OnlineFriendStatus {
    char    id[16];
    char    name[64];
    char    code[32];           ///< the game they are hosting, if any
    Boolean online;
} OnlineFriendStatus;
const char *OnlineProfileID(const char *profile);
const char *OnlineMyID(void);
int  OnlineGetFriends(OnlineFriend *out, int max);
int  OnlineGetRecentPlayers(OnlineFriend *out, int max);
Boolean OnlineIsFriend(const char *id);
Boolean OnlineNormalizeID(const char *typed, char *out);
Boolean OnlineAddFriend(const char *id, const char *name);
void OnlineRemoveFriend(const char *id);
void OnlineNoteRecentPlayer(const char *id, const char *name);
const char *OnlineLobbyAddress(void);
void OnlineSetLobbyAddress(const char *address);
void OnlineConnect(void);
void OnlineDisconnect(void);
void OnlinePoll(void);
void OnlineBrowse(Boolean on);
void OnlineHost(int mode, Boolean isPublic, int character);
void OnlineJoin(const char *code, const char *hostID);
int  OnlineTakePairedSocket(Uint8 *leftover, int *leftoverLen, int max);
Boolean OnlineNextIncoming(char *ticket, size_t size);
int  OnlineAttach(const char *ticket, Uint8 *leftover, int *leftoverLen, int max);
void OnlineStartHostedGame(void);
Boolean OnlineIsConnecting(void);
Boolean OnlineIsReady(void);
Boolean OnlineFailed(void);
Boolean OnlineIsPaired(void);
Boolean OnlineListArrived(void);
const char *OnlineError(void);
const char *OnlineMessageOfTheDay(void);
const char *OnlineHostedCode(void);
const char *OnlinePeerID(void);
const char *OnlinePeerName(void);
void OnlineClearError(void);
int  OnlineGetGames(OnlineGame *out, int max);
Boolean OnlineFriendStatusFor(const char *id, OnlineFriendStatus *out);
void OnlineShareState(void (*share)(void *, size_t));

// Chat with the other computer's player (MMChat.c)
#define kChatMaxLength 120
void ChatNoteGameStarting(char names[][64], int numPlayers, int localPlayer, Boolean online);
void ChatReceived(const char *name, const char *text, int player);
Boolean ChatAvailable(void);
Boolean ChatIsTyping(void);
Boolean ChatHandleEvent(const SDL_Event *event);
void ChatDraw(float x, float bottom, float width, float size, float pixelsPerUnit);
void ChatDrawInWorld(SpriteWorldPtr w);
const char *ChatHint(void);
void ChatShareState(void (*share)(void *, size_t));

// Multiplayer saves, kept for the group that played (MMMultiSave.c)
enum { kMultiSaveCoop = 0, kMultiSaveArcade = 1 };
typedef struct MultiSaveInfo {
    char    path[1500];
    char    name[256];
    char    mansion[256];
    Uint32  savedAt;
    int     numPlayers;
    char    ids[kMaxPlayers][16];
    char    names[kMaxPlayers][64];
    int     characters[kMaxPlayers];
    long    loot[kMaxPlayers];
    char    turnID[16];
} MultiSaveInfo;
const char *MultiSaveFolderForCurrentGame(int mode);
Boolean MultiSaveWrite(int mode, const char *saveName);
int  MultiSaveList(int mode, char ids[][16], int count, MultiSaveInfo *out, int max);
int  MultiSaveListForCurrentGroup(int mode, MultiSaveInfo *out, int max);
Boolean MultiSaveReadInfo(const char *path, MultiSaveInfo *info);
Boolean MultiSaveStoreReceived(int mode, const char *saveName, const Uint8 *data, int length, const Uint8 *sidecar,
                               int sidecarLength, char *pathOut, size_t size);
void MultiSaveDescribe(const MultiSaveInfo *info, char *text, size_t size);
long GetPlayerLoot(int player);
int  MultiplayerContinueSave(const char *path);
void MultiplayerReleaseCharacterSprites(void);
void MultiplayerMansionOpened(void);
void NetSetContinueSave(const char *path);
const char *NetContinuedSavePath(void);
const char *NetLastSaveName(void);
#define kNetEventDebugKill  0x7e     ///< the F6 testing key (MM_DEBUG_CHEATS), sent like a replay event
Boolean NetGameActive(void);
Boolean NetIsConnected(void);
Boolean NetIsFollower(void);
int  NetLocalPlayer(void);
Boolean NetPump(void);
Boolean NetHostGame(void);
Boolean NetJoinGame(void);
Boolean NetClientWaitForStart(void);
void NetGameStarted(void);
void NetSeedRandom(void);
Boolean NetPlayerHasBeatenMansion(int player);
void NetEndSession(void);
void NetRecordEvent(char event, Boolean isDown);
Boolean NetMustWait(void);
void NetRedrawWhileWaiting(void);
Boolean NetIsWaitingLong(void);
void NetFrameBegin(void);
void NetTurnChanged(void);
void NetSendSaveChoice(Boolean save);
Boolean NetWaitForSaveChoice(void);
void NetPeerLeftDuringGame(void);
void NetAskToLeave(void);

// Screens added by this version (MMDialogs.c)
Boolean DoTextEntryDialog(const char *prompt, char *text, int maxLen);
Boolean RunWaitDialog(const char *title, const char *message, Boolean (*poll)(void *), void *context);
Boolean RunLiveWaitDialog(const char *title, void (*message)(char *, size_t, void *), int (*poll)(void *),
                          Boolean (*canStart)(void *), void *context);
int  DoSimpleChoiceDialog(const char *title, const char **choices, int count, int selected);
Boolean DoChooseOtherPlayerDialog(const char *title, char exclude[][64], int numExclude, char *nameOut,
                                  size_t size);
int  MMDoRandomMansionDialog(void);
void MMDoTrophyRoom(void);

#endif
