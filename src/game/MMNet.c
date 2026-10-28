/*
 * MMNet.c — playing on two to four computers, added by this version: relay co-op, and the
 * Ghost Race. Hosts on the local network announce themselves (UDP broadcast) so the
 * multiplayer screen can list them; over the internet the lobby server (MMOnline.c)
 * introduces the computers and carries their messages.
 *
 * The host is player 1 and the hub: each guest is connected to the host only, and the host
 * passes on what each guest sends that the others need (wrapped in kMsgFrom, saying whose
 * it is). Guests join while the host waits; the host starts the game when everyone's in.
 *
 * Relay co-op: every computer runs the same game: the same mansion, the same random seed and
 * the same Fun settings, so given the same input they stay in step frame by frame (the
 * replay feature relies on the same property). The computer whose player has the turn
 * "drives": at the start of every game frame it sends the state of the game keys and any
 * special events (cheats, a sign being closed, easy jumping) for that frame. The others apply
 * them at the same frame and so show exactly what the driver sees. A save pedestal's choice
 * is sent the moment it is made. When the driving player dies, the next player's turn starts
 * on every computer and the roles move on.
 *
 * Ghost Race: each computer plays its own copy of the mansion (in split screen's machinery,
 * with one player here and the others elsewhere). Every frame each sends where its player is
 * (kMsgPhantom); the others draw them as ghosts, and the times decide the race.
 *
 * Arcade Split Screen: the same, and the team's game is shared: what each player collects,
 * opens and pulls goes to the others (kMsgTeam, MMCoopSync.c), and whoever keeps a room sends
 * its snapshot to the players in it elsewhere (kMsgRoomSnap, MMRoomSync.c).
 *
 * Messages are [type:1][length:2][payload] over TCP (port 47474). While a host waits it
 * broadcasts "MMHD-LAN" beacons on UDP port 47475 once a second.
 */

#include "MMGame.h"
#include "MMProtos.h"

#if !defined(_WIN32) && !defined(MM_PS3)
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#define MM_HAVE_SOCKETS 1
#elif defined(MM_PS3)
#include "PS3Net.h"                          // the PS3's sockets, with what PSL1GHT lacks
#define MM_HAVE_SOCKETS 1
#endif

#define kNetPort        47474
#define kBeaconPort     47475
#define kNetVersion     6           ///< 6: relay co-op on the game clock (1.3); 5: Arcade Split Screen
#define kBeaconMagic    "MMHD-LAN 5"
#define kMaxFoundGames  16
#define kMaxQueuedFrames (60 * 60 * 3)  ///< frames a watching computer can be behind (3 minutes)
#define kMaxLinks       (kMaxPlayers - 1)
#define kLinkBufSize    70000
#define kMaxOutQueue    (4 * 1024 * 1024)   ///< a computer this far behind with reading has gone

enum {
    kMsgHello = 1, kMsgStart = 2, kMsgReady = 3, kMsgFrame = 4, kMsgSave = 5, kMsgBye = 6, kMsgPhantom = 7,
    kMsgNoMansion = 8, kMsgChat = 9, kMsgRoster = 10, kMsgFrom = 11, kMsgLeft = 12, kMsgTeam = 13,
    kMsgRoomSnap = 14,      // 15-19: Arcade Split Screen team saves (MMExtras.h, MMTeamSave.c)
    kMsgRefused = 20        ///< host to a guest it won't take: why (text)
};

typedef struct NetFrame {
    Uint32  frame;
    Uint16  keys;
    Uint8   numEvents;
    Uint8   events[32];
    Boolean hasCheck;       ///< the driver's game-state checksum came with it (every 60 frames)
    Uint32  check;
} NetFrame;

#define kCheckEvery     60      ///< frames between the relay co-op checksums
#define kCheckMark      'C'     ///< after a frame's events: then the checksum (older versions ignore it)

/// A connection to another computer: on the host, one per guest; on a guest, the host.
typedef struct NetLink {
    int     sock;
    int     player;             ///< the player at the other end (-1: a guest that hasn't said hello)
    Uint8   in[kLinkBufSize];
    int     inLen;
    Uint8  *out;                ///< what's waiting to go (the connection takes it as it can)
    int     outStart, outLen, outSize;
    Boolean broken;             ///< it failed, or the other end stopped reading: NetPump hangs up
} NetLink;

typedef struct NetSession {
    Boolean active;             ///< connected to other computers
    Boolean isHost;
    Boolean inGame;             ///< a game was started together
    Boolean peerLeft;           ///< someone left (relay co-op ends then)
    Boolean fellBehind;         ///< this computer fell more than kMaxQueuedFrames behind
    Boolean peerHasNoMansion;   ///< someone doesn't have the mansion: they went back to the menu
    int     noMansionPlayer;
    Boolean missingMansion;     ///< this computer doesn't have the host's mansion
    char    missingMansionName[256];
    Boolean online;             ///< through the lobby server, over the internet
    int     listenSock;
    NetLink links[kMaxLinks];
    int     numLinks;
    int     localPlayer;        ///< 0 on the host
    int     mode;               ///< kNetModeRelay or kNetModeRace
    Boolean rosterReceived;
        // everyone in the game, by player
    int     numPlayers;
    char    names[kMaxPlayers][64];
    char    ids[kMaxPlayers][16];
    Uint8   characters[kMaxPlayers];
    Boolean left[kMaxPlayers];
    Uint8   beaten[kMaxPlayers];        ///< has each player beaten this mansion (Nine Lives, Espresso)
    NetRaceState states[kMaxPlayers];   ///< race: where each player is
    Boolean stateValid[kMaxPlayers];
    Uint32  lastBeacon;
    Uint32  beaconToken;        ///< tells this host's beacons apart (one host heard on two networks)
    int     beaconSock;
    Uint32  seed;
    Uint32  frame;              ///< game frames since the start (every computer counts the same)
    NetFrame queue[kMaxQueuedFrames];
    int     queueHead, queueCount;
    int     saveChoice;         ///< -1 until the driver's save pedestal choice arrives
    char    lastSaveName[128];  ///< the name the driver saved under
    char    continuePath[1500]; ///< host: the saved game to continue; guest: where the host's copy went
    Boolean startReceived;
    Uint8   startPayload[65536];    ///< (with a saved game in it when continuing one)
    int     startLen;
    Uint32  waitStart;          ///< when the watcher started waiting for a frame (0: not waiting)
    Uint32  lastRedraw;
        // outgoing events for the next frame
    Uint8   pending[32];
    int     numPending;
        // a guest's own Fun settings, while the host's are in use
    ExtrasSettings savedExtras;
    Boolean extrasSaved;
    char    refusal[256];       ///< guest: why the host wouldn't take this computer
} NetSession;

static NetSession gNet = { false };
static Boolean gWaitCancelled;

/// Browsing for games on the local network.
static struct {
    int         sock;
    Boolean     open;
    NetFoundGame games[kMaxFoundGames];
    int         count;
} gBrowse = { -1, false };

/// A relay co-op game on several computers is being played (they run in lockstep).
Boolean NetGameActive(void)
{
    return gNet.active && gNet.inGame && gNet.mode == kNetModeRelay;
}

/// A race on several computers is being played.
Boolean NetRaceActive(void)
{
    return gNet.active && gNet.inGame && (gNet.mode == kNetModeRace || gNet.mode == kNetModeArcade);
}

/// Arcade Split Screen on several computers is being played.
Boolean NetArcadeActive(void)
{
    return gNet.active && gNet.inGame && gNet.mode == kNetModeArcade;
}

int NetMode(void)
{
    return gNet.mode;
}

/// What the host offers (before NetHostGame).
void NetSetMode(int mode)
{
    gNet.mode = mode;
}

Boolean NetIsConnected(void)
{
    return gNet.active;
}

int NetLocalPlayer(void)
{
    return gNet.localPlayer;
}

int NetNumPlayers(void)
{
    return gNet.numPlayers;
}

const char *NetPlayerName(int player)
{
    return player >= 0 && player < gNet.numPlayers ? gNet.names[player] : "";
}

const char *NetPlayerID(int player)
{
    return player >= 0 && player < gNet.numPlayers ? gNet.ids[player] : "";
}

int NetPlayerCharacter(int player)
{
    return player >= 0 && player < gNet.numPlayers ? gNet.characters[player] : 0;
}

Boolean NetPlayerLeft(int player)
{
    return player >= 0 && player < kMaxPlayers && gNet.left[player];
}

Boolean NetIsOnline(void)
{
    return gNet.active && gNet.online;
}

/// Another computer's player has the turn: this one only watches.
Boolean NetIsFollower(void)
{
    return NetGameActive() && GetActivePlayer() != gNet.localPlayer;
}

/// "Ann, Bob and Cy" (for the waiting rooms).
static void PlayerList(char *text, size_t size)
{
    size_t n = 0;
    int p;

    text[0] = 0;
    for (p = 0; p < gNet.numPlayers; p++)
        n += (size_t)snprintf(text + n, size > n ? size - n : 0, "%s%s%s", p == 0 ? "" : p == gNet.numPlayers - 1 ? " and " : ", ",
                              gNet.names[p], p == 0 ? " (host)" : "");
}

// ---------------------------------------------------------------------------
// Numbers in messages: little-endian whatever the computer (the PS3 is big-endian)

void NetPutU16(Uint8 *p, Uint16 v)
{
    p[0] = (Uint8)v; p[1] = (Uint8)(v >> 8);
}

Uint16 NetGetU16(const Uint8 *p)
{
    return (Uint16)(p[0] | p[1] << 8);
}

void NetPutU32(Uint8 *p, Uint32 v)
{
    p[0] = (Uint8)v; p[1] = (Uint8)(v >> 8); p[2] = (Uint8)(v >> 16); p[3] = (Uint8)(v >> 24);
}

Uint32 NetGetU32(const Uint8 *p)
{
    return (Uint32)p[0] | (Uint32)p[1] << 8 | (Uint32)p[2] << 16 | (Uint32)p[3] << 24;
}

void NetPutF32(Uint8 *p, float v)
{
    Uint32 bits;
    memcpy(&bits, &v, 4);
    NetPutU32(p, bits);
}

float NetGetF32(const Uint8 *p)
{
    Uint32 bits = NetGetU32(p);
    float v;
    memcpy(&v, &bits, 4);
    return v;
}

void NetPutF64(Uint8 *p, double v)
{
    Uint64 bits;
    memcpy(&bits, &v, 8);
    NetPutU32(p, (Uint32)bits);
    NetPutU32(p + 4, (Uint32)(bits >> 32));
}

double NetGetF64(const Uint8 *p)
{
    Uint64 bits = (Uint64)NetGetU32(p) | (Uint64)NetGetU32(p + 4) << 32;
    double v;
    memcpy(&v, &bits, 8);
    return v;
}

#ifdef MM_HAVE_SOCKETS

// ---------------------------------------------------------------------------
// Sockets

static void SetNonBlocking(int sock)
{
    int flags = fcntl(sock, F_GETFL, 0), one = 1;

    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
#ifdef SO_NOSIGPIPE
    setsockopt(sock, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof(one));
#endif
}

static void CloseSocket(int *sockP)
{
    if (*sockP >= 0)
        close(*sockP);
    *sockP = -1;
}

/*  Sending never waits: each message goes on the link's queue, and the queue goes out as the
    connection takes it (now, and at every NetPump). Waiting for a computer that isn't reading
    (in a menu, say) used to hold up the game for seconds, then give up halfway through a
    message, which garbled everything after it. */

/// Sends what's queued, as much as the connection takes now.
static void FlushLink(NetLink *link)
{
    int sent, flags = 0;

#ifdef MSG_NOSIGNAL
    flags = MSG_NOSIGNAL;
#endif
    while (link->outLen > 0 && link->sock >= 0 && !link->broken) {
        sent = (int)send(link->sock, link->out + link->outStart, (size_t)link->outLen, flags);
        if (sent > 0) {
            link->outStart += sent;
            link->outLen -= sent;
        } else if (sent < 0 && (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)) {
            break;                  // full for now: the rest goes later
        } else {
            link->broken = true;
        }
    }
    if (link->outLen == 0)
        link->outStart = 0;
}

static void QueueOnLink(NetLink *link, const Uint8 *data, int length)
{
    int need;
    Uint8 *bigger;

    if (link->broken || length <= 0)
        return;
    if (link->outStart > 0 && link->outStart + link->outLen + length > link->outSize) {
        memmove(link->out, link->out + link->outStart, (size_t)link->outLen);
        link->outStart = 0;
    }
    need = link->outStart + link->outLen + length;
    if (need > link->outSize) {
        if (need > kMaxOutQueue) {
            link->broken = true;    // it stopped reading long ago
            return;
        }
        bigger = realloc(link->out, (size_t)(need * 2 > kMaxOutQueue ? kMaxOutQueue : need * 2));
        if (bigger == NULL) {
            link->broken = true;
            return;
        }
        link->out = bigger;
        link->outSize = need * 2 > kMaxOutQueue ? kMaxOutQueue : need * 2;
    }
    memcpy(link->out + link->outStart + link->outLen, data, (size_t)length);
    link->outLen += length;
}

/// Hangs up, giving what's still queued (a goodbye, say) a moment to go first.
static void CloseLink(NetLink *link)
{
    Uint32 start = SDL_GetTicks();

    while (link->sock >= 0 && link->outLen > 0 && !link->broken && SDL_GetTicks() - start < 300) {
        FlushLink(link);
        if (link->outLen > 0)
            SDL_Delay(1);
    }
    CloseSocket(&link->sock);
    free(link->out);
    link->out = NULL;
    link->outStart = link->outLen = link->outSize = 0;
    link->broken = false;
}

static void Disconnect(void)
{
    int i;

    for (i = 0; i < gNet.numLinks; i++)
        CloseLink(&gNet.links[i]);
    gNet.numLinks = 0;
    CloseSocket(&gNet.listenSock);
    gNet.active = false;
}

static void PutU32(Uint8 *p, Uint32 v)
{
    p[0] = (Uint8)v; p[1] = (Uint8)(v >> 8); p[2] = (Uint8)(v >> 16); p[3] = (Uint8)(v >> 24);
}

static Uint32 GetU32(const Uint8 *p)
{
    return (Uint32)p[0] | (Uint32)p[1] << 8 | (Uint32)p[2] << 16 | (Uint32)p[3] << 24;
}

static void SendTo(NetLink *link, Uint8 type, const void *payload, Uint16 length)
{
    Uint8 header[3];

    if (link->sock < 0)
        return;
    header[0] = type;
    header[1] = (Uint8)(length & 0xff);
    header[2] = (Uint8)(length >> 8);
    QueueOnLink(link, header, 3);
    QueueOnLink(link, (const Uint8 *)payload, length);
    FlushLink(link);
}

/// To everyone: the host sends it to every guest; a guest to the host (who passes it on).
static void SendMessage(Uint8 type, const void *payload, Uint16 length)
{
    int i;

    if (!gNet.active)
        return;
    for (i = 0; i < gNet.numLinks; i++)
        if (gNet.links[i].player >= 0 || !gNet.isHost)
            SendTo(&gNet.links[i], type, payload, length);
}

/// Host: a guest's message for the others, saying whose it is.
static void ForwardToOthers(int from, Uint8 type, const Uint8 *payload, int length)
{
    static Uint8 wrapped[65536];
    int i;

    if (length + 2 > 65535)
        return;
    wrapped[0] = (Uint8)from;
    wrapped[1] = type;
    memcpy(wrapped + 2, payload, (size_t)length);
    for (i = 0; i < gNet.numLinks; i++)
        if (gNet.links[i].player >= 0 && gNet.links[i].player != from)
            SendTo(&gNet.links[i], kMsgFrom, wrapped, (Uint16)(length + 2));
}

// ---------------------------------------------------------------------------
// The players

/// Host: tells everyone who's playing (each guest learns which player they are).
static void SendRoster(void)
{
    Uint8 payload[2 + kMaxPlayers * 80];
    int i, p, n, len;

    for (i = 0; i < gNet.numLinks; i++) {
        NetLink *link = &gNet.links[i];
        if (link->player < 0)
            continue;
        n = 0;
        payload[n++] = (Uint8)gNet.numPlayers;
        payload[n++] = (Uint8)link->player;
        for (p = 0; p < gNet.numPlayers; p++) {
            len = (int)strlen(gNet.names[p]);
            payload[n++] = gNet.characters[p];
            memcpy(payload + n, gNet.ids[p], 14);
            n += 14;
            payload[n++] = (Uint8)len;
            memcpy(payload + n, gNet.names[p], (size_t)len);
            n += len;
        }
        SendTo(link, kMsgRoster, payload, (Uint16)n);
    }
}

/// This computer's player in the list (player 0 on the host).
static void SetLocalPlayerEntry(int player)
{
    char name[256];

    SWCopyPascalStringToCString((ConstStringPtr)gCurrentPlayerName, name);
    snprintf(gNet.names[player], sizeof(gNet.names[player]), "%.63s", name);
    snprintf(gNet.ids[player], sizeof(gNet.ids[player]), "%s", OnlineMyID());
    gNet.characters[player] = (Uint8)GetPlayerCharacter(0);
}

/// Host, before the game: a guest went away; the ones after them move up.
static void RemoveGuestBeforeStart(int linkIndex)
{
    int player = gNet.links[linkIndex].player, i, p;

    CloseLink(&gNet.links[linkIndex]);
    for (i = linkIndex; i < gNet.numLinks - 1; i++)
        gNet.links[i] = gNet.links[i + 1];
    gNet.numLinks--;
    gNet.links[gNet.numLinks].out = NULL;           // (moved up: it's the one before's now)
    gNet.links[gNet.numLinks].outStart = gNet.links[gNet.numLinks].outLen = gNet.links[gNet.numLinks].outSize = 0;
    gNet.links[gNet.numLinks].sock = -1;
    if (player < 0)
        return;
    for (p = player; p < gNet.numPlayers - 1; p++) {
        memcpy(gNet.names[p], gNet.names[p + 1], sizeof(gNet.names[p]));
        memcpy(gNet.ids[p], gNet.ids[p + 1], sizeof(gNet.ids[p]));
        gNet.characters[p] = gNet.characters[p + 1];
    }
    gNet.numPlayers--;
    for (i = 0; i < gNet.numLinks; i++)
        if (gNet.links[i].player > player)
            gNet.links[i].player--;
    SendRoster();
}

/// Someone left during the game (they stay in the list, marked gone).
static void PlayerLeft(int player)
{
    Uint8 p = (Uint8)player;
    int i;

    if (player < 0 || player >= kMaxPlayers || gNet.left[player])
        return;
    gNet.left[player] = true;
    gNet.peerLeft = true;
    if (gNet.isHost)
        for (i = 0; i < gNet.numLinks; i++)
            if (gNet.links[i].player >= 0 && gNet.links[i].player != player)
                SendTo(&gNet.links[i], kMsgLeft, &p, 1);
}

// ---------------------------------------------------------------------------
// Receiving

static void HandleMessage(NetLink *link, int from, Uint8 type, const Uint8 *payload, int length)
{
    NetFrame *f;
    int i, n, p;

    switch (type) {
        case kMsgHello:         // guest to host: who they are
            if (!gNet.isHost || link == NULL || link->player >= 0)
                break;
            if (length < 17 || payload[0] != kNetVersion || gNet.inGame || gNet.numPlayers >= kMaxPlayers) {
                SendTo(link, kMsgBye, NULL, 0);     // another version, or full: no
                CloseLink(link);
                break;
            }
            for (i = 0; i < gNet.numPlayers; i++)
                if (strncmp(gNet.ids[i], (const char *)payload + 2, 14) == 0) {
                        // the same player (a profile copied from another computer): no
                    static const char why[] = "That player is already in this game (the same player profile, "
                                              "copied from another computer?). Choose or make another player "
                                              "profile on this computer, and join again.";
                    SendTo(link, kMsgRefused, why, (Uint16)(sizeof(why) - 1));
                    SendTo(link, kMsgBye, NULL, 0);
                    CloseLink(link);
                    break;
                }
            if (i < gNet.numPlayers)
                break;
            p = gNet.numPlayers++;
            link->player = p;
            gNet.characters[p] = payload[1] < kNumCharacters ? payload[1] : 0;
            snprintf(gNet.ids[p], sizeof(gNet.ids[p]), "%.14s", (const char *)payload + 2);
            snprintf(gNet.names[p], sizeof(gNet.names[p]), "%.*s", length - 16 > 63 ? 63 : length - 16,
                     (const char *)payload + 16);
            gNet.left[p] = false;
            SendRoster();
            break;
        case kMsgRoster:        // host to guests: who's playing, and which one you are
            if (gNet.isHost || length < 2)
                break;
            gNet.numPlayers = payload[0] > kMaxPlayers ? kMaxPlayers : payload[0];
            gNet.localPlayer = payload[1];
            for (p = 0, n = 2; p < gNet.numPlayers && n + 16 <= length; p++) {
                int len;
                gNet.characters[p] = payload[n] < kNumCharacters ? payload[n] : 0;
                snprintf(gNet.ids[p], sizeof(gNet.ids[p]), "%.14s", (const char *)payload + n + 1);
                len = payload[n + 15];
                n += 16;
                if (n + len > length)
                    len = length - n;
                snprintf(gNet.names[p], sizeof(gNet.names[p]), "%.*s", len > 63 ? 63 : len, (const char *)payload + n);
                n += len;
            }
            gNet.rosterReceived = true;
            break;
        case kMsgFrom:          // host to guests: another guest's message
            if (!gNet.isHost && length >= 2 && payload[1] != kMsgFrom)
                HandleMessage(link, payload[0], payload[1], payload + 2, length - 2);
            return;
        case kMsgLeft:
            if (!gNet.isHost && length >= 1)
                PlayerLeft(payload[0]);
            break;
        case kMsgPhantom:
            if (length >= 32 && from >= 0 && from < kMaxPlayers) {
                NetRaceState *st = &gNet.states[from];
                st->room = (short)NetGetU16(payload);
                st->x = NetGetF32(payload + 2);
                st->y = NetGetF32(payload + 6);
                st->frame = (short)NetGetU16(payload + 10);
                st->flipped = (payload[12] & 1) != 0;
                st->visible = (payload[12] & 2) != 0;
                st->status = payload[13];
                st->elapsedMs = GetU32(payload + 14);
                st->deaths = payload[18];
                st->scaleW = NetGetF32(payload + 19);
                st->scaleH = st->scaleW;
                st->place = payload[23];
                st->mapRoom = (short)NetGetU16(payload + 24);
                st->runningRoom = (short)NetGetU16(payload + 26);
                st->roomMs = GetU32(payload + 28);
                gNet.stateValid[from] = true;
            }
            break;
        case kMsgTeam:
            if (from >= 0 && from < kMaxPlayers && from != gNet.localPlayer)
                CoopSyncReceiveOps(from, payload, length);
            break;
        case kMsgRoomSnap:
            if (from >= 0 && from < kMaxPlayers && from != gNet.localPlayer)
                RoomSyncReceive(from, payload, length);
            break;
        case kMsgSaveRequest: case kMsgSaveCollect: case kMsgSaveData: case kMsgSaveAck: case kMsgSaveDone:
            if (from >= 0 && from < kMaxPlayers && from != gNet.localPlayer)
                TeamSaveNetMessage(type, from, payload, length);
            break;
        case kMsgRefused:           // the host won't take this computer: why
            if (!gNet.isHost) {
                snprintf(gNet.refusal, sizeof(gNet.refusal), "%.*s", length > 250 ? 250 : length, (const char *)payload);
                gNet.peerLeft = true;
            }
            break;
        case kMsgStart:
            if (!gNet.isHost && length <= (int)sizeof(gNet.startPayload)) {
                memcpy(gNet.startPayload, payload, (size_t)length);
                gNet.startLen = length;
                gNet.startReceived = true;
            }
            break;
        case kMsgReady:
            if (length >= 1 && from >= 0 && from < kMaxPlayers)
                gNet.beaten[from] = payload[0];
            break;
        case kMsgFrame:
            if (length < 7)
                break;
            if (gNet.queueCount >= kMaxQueuedFrames) {
                gNet.fellBehind = gNet.peerLeft = true;     // the frames can't all be kept: it ends
                break;
            }
            f = &gNet.queue[(gNet.queueHead + gNet.queueCount) % kMaxQueuedFrames];
            f->frame = GetU32(payload);
            f->keys = (Uint16)(payload[4] | payload[5] << 8);
            f->numEvents = payload[6] > 32 ? 32 : payload[6];
            if (7 + f->numEvents > length)
                f->numEvents = (Uint8)(length - 7);
            memcpy(f->events, payload + 7, f->numEvents);
            f->hasCheck = length >= 7 + f->numEvents + 5 && payload[7 + f->numEvents] == kCheckMark;
            if (f->hasCheck)
                f->check = GetU32(payload + 8 + f->numEvents);
            gNet.queueCount++;
            break;
        case kMsgSave:
            if (length >= 1) {
                gNet.saveChoice = payload[0];
                snprintf(gNet.lastSaveName, sizeof(gNet.lastSaveName), "%.*s", length - 1, (const char *)payload + 1);
                if (gNet.lastSaveName[0] == 0)
                    snprintf(gNet.lastSaveName, sizeof(gNet.lastSaveName), "Saved game");
            }
            break;
        case kMsgNoMansion:
            gNet.peerHasNoMansion = true;
            gNet.noMansionPlayer = from;
            snprintf(gNet.missingMansionName, sizeof(gNet.missingMansionName), "%.*s", length, (const char *)payload);
            PlayerLeft(from);
            break;
        case kMsgChat:
            if (length > 0) {
                char text[kChatMaxLength + 1];
                snprintf(text, sizeof(text), "%.*s", length < kChatMaxLength ? length : kChatMaxLength,
                         (const char *)payload);
                ChatReceived(NetPlayerName(from)[0] ? NetPlayerName(from) : "Other player", text, from);
            }
            break;
        case kMsgBye:
            if (gNet.isHost) {
                if (link != NULL && link->player >= 0)
                    PlayerLeft(link->player);
            } else {
                for (i = 0; i < gNet.numPlayers; i++)       // the host is gone: so is everyone
                    if (i != gNet.localPlayer)
                        PlayerLeft(i);
                gNet.peerLeft = true;
            }
            return;
    }
        // the host passes on what the others need to hear
    if (gNet.isHost && link != NULL && link->player >= 0 && from == link->player &&
        (type == kMsgFrame || type == kMsgSave || type == kMsgPhantom || type == kMsgChat || type == kMsgReady ||
         type == kMsgNoMansion || type == kMsgTeam || type == kMsgRoomSnap))
        ForwardToOthers(from, type, payload, length);
}

/// Reads one connection. Returns false once it has closed.
static Boolean PumpLink(NetLink *link)
{
    int got, length, used = 0;
    Boolean open = true;

    FlushLink(link);                // what's still queued for it
    if (link->broken)
        return false;
    while (link->sock >= 0 && link->inLen < kLinkBufSize) {
        got = (int)recv(link->sock, link->in + link->inLen, (size_t)(kLinkBufSize - link->inLen), 0);
        if (got > 0)
            link->inLen += got;
        else {
            if (got == 0 || (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR))
                open = false;
            break;
        }
    }
    while (link->inLen - used >= 3) {
        length = link->in[used + 1] | link->in[used + 2] << 8;
        if (link->inLen - used < 3 + length)
            break;
        HandleMessage(link, gNet.isHost ? link->player : 0, link->in[used], link->in + used + 3, length);
        used += 3 + length;
        if (link->sock < 0)
            return false;
    }
    if (used > 0) {
        memmove(link->in, link->in + used, (size_t)(link->inLen - used));
        link->inLen -= used;
    }
    return open && link->sock >= 0;
}

/// Reads whatever has arrived from every connection. Returns false once someone has left.
Boolean NetPump(void)
{
    int i;

    if (!gNet.active)
        return false;
    for (i = 0; i < gNet.numLinks; i++) {
        NetLink *link = &gNet.links[i];
        if (link->sock < 0)
            continue;
        if (!PumpLink(link)) {
            CloseLink(link);
            if (gNet.isHost && !gNet.inGame) {
                RemoveGuestBeforeStart(i);
                i--;
            } else if (gNet.isHost) {
                PlayerLeft(link->player);
            } else {
                int p;
                for (p = 0; p < gNet.numPlayers; p++)
                    if (p != gNet.localPlayer)
                        PlayerLeft(p);
                if (gNet.numPlayers == 0)
                    gNet.peerLeft = true;
            }
        }
    }
    return !gNet.peerLeft;
}

/// A new connection (accepted on the local network, or from the lobby server).
static NetLink *AddLink(int sock, const Uint8 *leftover, int leftoverLen)
{
    NetLink *link;

    if (gNet.numLinks >= kMaxLinks) {
        close(sock);
        return NULL;
    }
    link = &gNet.links[gNet.numLinks++];
    link->sock = sock;
    link->player = gNet.isHost ? -1 : 0;
    link->outStart = link->outLen = 0;      // (a buffer from before is used again)
    link->broken = false;
    link->inLen = leftoverLen < kLinkBufSize ? leftoverLen : kLinkBufSize;
    if (link->inLen > 0)
        memcpy(link->in, leftover, (size_t)link->inLen);
    SetNonBlocking(sock);
    gNet.active = true;
    return link;
}

/// Guest: tells the host who this is.
static void SendHello(void)
{
    Uint8 payload[80];
    char name[256];
    size_t len;

    SWCopyPascalStringToCString((ConstStringPtr)gCurrentPlayerName, name);
    len = strlen(name);
    if (len > 63)
        len = 63;
    payload[0] = kNetVersion;
    payload[1] = (Uint8)GetPlayerCharacter(0);
    memcpy(payload + 2, OnlineMyID(), 14);
    memcpy(payload + 16, name, len);
    SendMessage(kMsgHello, payload, (Uint16)(len + 16));
}

/// Chat: a line to everyone.
void NetSendChat(const char *text)
{
    size_t len = strlen(text);
    if (len > kChatMaxLength)
        len = kChatMaxLength;
    SendMessage(kMsgChat, text, (Uint16)len);
}

static void ResetSession(void)
{
    int i;

    gNet.active = gNet.inGame = gNet.peerLeft = gNet.startReceived = gNet.rosterReceived = false;
    gNet.fellBehind = false;
    gNet.peerHasNoMansion = gNet.missingMansion = gNet.online = false;
    gNet.noMansionPlayer = -1;
    gNet.listenSock = -1;
    gNet.numLinks = 0;
    for (i = 0; i < kMaxLinks; i++)
        gNet.links[i].sock = -1;
    gNet.numPlayers = 0;
    memset(gNet.left, 0, sizeof(gNet.left));
    memset(gNet.beaten, 0, sizeof(gNet.beaten));
    memset(gNet.stateValid, 0, sizeof(gNet.stateValid));
    gNet.queueHead = gNet.queueCount = gNet.numPending = 0;
    gNet.saveChoice = -1;
    gNet.lastSaveName[0] = 0;
    gNet.continuePath[0] = 0;
    TeamSaveNetReset();
    TeamSaveNetForgetContinue();
}

/// Host: player 1, alone so far.
static void StartHosting(void)
{
    ResetSession();
    gNet.isHost = true;
    gNet.localPlayer = 0;
    gNet.numPlayers = 1;
    SetLocalPlayerEntry(0);
}

/// This computer's addresses on the local network, for the host to read out.
static void GetLocalAddresses(char *text, size_t size)
{
    struct ifaddrs *list, *ifa;
    char address[INET_ADDRSTRLEN];
    size_t used = 0;

    text[0] = 0;
    if (getifaddrs(&list) != 0)
        return;
    for (ifa = list; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET)
            continue;
        inet_ntop(AF_INET, &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr, address, sizeof(address));
        if (strncmp(address, "127.", 4) == 0)
            continue;
        used += (size_t)snprintf(text + used, size > used ? size - used : 0, "%s%s", used > 0 ? "  or  " : "", address);
    }
    freeifaddrs(list);
    if (text[0] == 0)
        snprintf(text, size, "127.0.0.1");
}

// ---------------------------------------------------------------------------
// Hosting: the waiting room (on the local network, or online)

static char gHostAddresses[200], gHostCode[32];
static Boolean gHostPublic;

/// While hosting on the local network: "here I am", once a second.
static void SendBeacon(void)
{
    struct sockaddr_in to;
    struct ifaddrs *list, *ifa;
    char message[220], name[256];
    int one = 1, len;

    if (SDL_GetTicks() - gNet.lastBeacon < 1000)
        return;
    gNet.lastBeacon = SDL_GetTicks();
    if (gNet.beaconSock < 0) {
        gNet.beaconSock = (int)socket(AF_INET, SOCK_DGRAM, 0);
        if (gNet.beaconSock < 0)
            return;
        setsockopt(gNet.beaconSock, SOL_SOCKET, SO_BROADCAST, &one, sizeof(one));
        SetNonBlocking(gNet.beaconSock);
    }
    SWCopyPascalStringToCString((ConstStringPtr)gCurrentPlayerName, name);
    len = snprintf(message, sizeof(message), "%s\n%d\n%d\n%.60s\n%u\n%d\n", kBeaconMagic, gNet.mode,
                   GetPlayerCharacter(0), name, (unsigned)gNet.beaconToken, gNet.numPlayers);
    memset(&to, 0, sizeof(to));
    to.sin_family = AF_INET;
    to.sin_port = htons(kBeaconPort);
    to.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    sendto(gNet.beaconSock, message, (size_t)len, 0, (struct sockaddr *)&to, sizeof(to));
        // each network's own broadcast address too (some routers drop 255.255.255.255)
    if (getifaddrs(&list) == 0) {
        for (ifa = list; ifa != NULL; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET || ifa->ifa_broadaddr == NULL ||
                !(ifa->ifa_flags & IFF_BROADCAST))
                continue;
            to.sin_addr = ((struct sockaddr_in *)ifa->ifa_broadaddr)->sin_addr;
            sendto(gNet.beaconSock, message, (size_t)len, 0, (struct sockaddr *)&to, sizeof(to));
        }
        freeifaddrs(list);
    }
        // and this computer, for a second copy of the game on it
    to.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sendto(gNet.beaconSock, message, (size_t)len, 0, (struct sockaddr *)&to, sizeof(to));
}

static void HostMessage(char *text, size_t size, void *unused)
{
    char players[400];

    (void)unused;
    PlayerList(players, sizeof(players));
    if (gHostCode[0])
        snprintf(text, size,
                 "Your game's code:  %s\r%s\r\rPlaying: %s\r\r%s", gHostCode,
                 gHostPublic ? "It's in everyone's list of online games."
                             : "Private: your friends see it in their list; others need the code.",
                 players,
                 gNet.numPlayers < 2 ? "Waiting for players (up to 4)..."
                 : gNet.numPlayers < kMaxPlayers ? "Press Okay to start, or wait for more (up to 4)." : "Press Okay to start.");
    else
        snprintf(text, size,
                 "Other computers on your network choose Multiplayer: this game shows up in their list, or they "
                 "join by typing this computer's address:  %s\r\rPlaying: %s\r\r%s", gHostAddresses, players,
                 gNet.numPlayers < 2 ? "Waiting for players (up to 4)..."
                 : gNet.numPlayers < kMaxPlayers ? "Press Okay to start, or wait for more (up to 4)." : "Press Okay to start.");
}

static Boolean HostCanStart(void *unused)
{
    (void)unused;
    return gNet.numPlayers >= 2;
}

static int LanHostPoll(void *unused)
{
    int sock;

    (void)unused;
    SendBeacon();
    while ((sock = (int)accept(gNet.listenSock, NULL, NULL)) >= 0) {
        if (gNet.numLinks >= kMaxLinks) {
            close(sock);            // full
            continue;
        }
        AddLink(sock, NULL, 0);
    }
    NetPump();
    if (getenv("MM_DEBUG_NET_START") != NULL && gNet.numPlayers >= atoi(getenv("MM_DEBUG_NET_START")))
        return 1;       // testing aid: starts once this many are in
    return 0;
}

/// After the waiting room: the game is on (true), or the host gave up (false).
static Boolean FinishGathering(Boolean started)
{
    int i;

    CloseSocket(&gNet.listenSock);
    CloseSocket(&gNet.beaconSock);
    if (!started || gNet.numPlayers < 2) {
        SendMessage(kMsgBye, NULL, 0);
        Disconnect();
        return false;
    }
    for (i = 0; i < gNet.numLinks; ) {          // anyone who never said hello
        if (gNet.links[i].player < 0)
            RemoveGuestBeforeStart(i);
        else
            i++;
    }
    SendRoster();
    ChatNoteGameStarting(gNet.names, gNet.numPlayers, gNet.localPlayer, gNet.online);
    return true;
}

/// Waits for other computers on the local network to join. Returns true when the host
/// starts with at least one other player.
Boolean NetHostGame(void)
{
    struct sockaddr_in addr;
    int one = 1;

    StartHosting();
    gNet.beaconSock = -1;
    gNet.beaconToken = (Uint32)time(NULL) ^ (Uint32)SDL_GetPerformanceCounter() ^ 0x5eed;
    gNet.listenSock = (int)socket(AF_INET, SOCK_STREAM, 0);
    if (gNet.listenSock < 0) {
        MMDoCustomDialog(gSpriteWorldP, "Sorry, the network couldn't be used.", 3);
        return false;
    }
    setsockopt(gNet.listenSock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(kNetPort);
    if (bind(gNet.listenSock, (struct sockaddr *)&addr, sizeof(addr)) != 0 || listen(gNet.listenSock, 4) != 0) {
        Disconnect();
        MMDoCustomDialog(gSpriteWorldP, "Sorry, the game couldn't wait for a connection (is another copy hosting?).", 3);
        return false;
    }
    SetNonBlocking(gNet.listenSock);
    gNet.active = true;
    GetLocalAddresses(gHostAddresses, sizeof(gHostAddresses));
    gHostCode[0] = 0;
    gNet.lastBeacon = 0;
    return FinishGathering(RunLiveWaitDialog("Host a Game", HostMessage, LanHostPoll, HostCanStart, NULL));
}

// ---------------------------------------------------------------------------
// Joining

/// Guest: until the host says who's playing.
static int GuestHelloPoll(void *unused)
{
    (void)unused;
    NetPump();
    if (gNet.rosterReceived)
        return 1;
    return gNet.peerLeft || gNet.numLinks == 0 || gNet.links[0].sock < 0 ? -1 : 0;
}

static void ConnectingMessage(char *text, size_t size, void *unused)
{
    (void)unused;
    snprintf(text, size, "Connecting...");
}

/// Guest, connected to the host: says hello and waits to be let in. Returns true if in.
static Boolean GreetHost(const char *title)
{
    SendHello();
    gNet.refusal[0] = 0;
    if (!RunLiveWaitDialog(title, ConnectingMessage, GuestHelloPoll, NULL, NULL)) {
        if (gNet.refusal[0])
            MMDoCustomDialog(gSpriteWorldP, gNet.refusal, 3);
        else if (gNet.peerLeft || gNet.links[0].sock < 0)
            MMDoCustomDialog(gSpriteWorldP, "Couldn't join: that game is full, has started, or is from another version "
                             "of the game.", 3);
        SendMessage(kMsgBye, NULL, 0);
        Disconnect();
        return false;
    }
    return true;
}

static Boolean ConnectTo(const char *host)
{
    struct addrinfo hints, *result = NULL;
    char port[16], name[64], *colon;
    fd_set writable;
    struct timeval timeout;
    int err = 0, sock;
    socklen_t errLen = sizeof(err);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    snprintf(port, sizeof(port), "%d", kNetPort);
    snprintf(name, sizeof(name), "%s", host);
    if ((colon = strchr(name, ':')) != NULL) {      // "address:port" (testing through a proxy, say)
        *colon = 0;
        snprintf(port, sizeof(port), "%d", atoi(colon + 1));
    }
    if (getaddrinfo(name, port, &hints, &result) != 0 || result == NULL)
        return false;
    sock = (int)socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sock < 0) {
        freeaddrinfo(result);
        return false;
    }
    SetNonBlocking(sock);
    if (connect(sock, result->ai_addr, result->ai_addrlen) != 0 && errno != EINPROGRESS) {
        freeaddrinfo(result);
        close(sock);
        return false;
    }
    freeaddrinfo(result);
    FD_ZERO(&writable);
    FD_SET(sock, &writable);
    timeout.tv_sec = 6;
    timeout.tv_usec = 0;
    if (select(sock + 1, NULL, &writable, NULL, &timeout) != 1 ||
        getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &errLen) != 0 || err != 0) {
        close(sock);
        return false;
    }
    AddLink(sock, NULL, 0);
    return true;
}

static char gLastAddress[64] = "";

/// Connects to a host, asking for its address. Returns true when let in (the host then
/// picks the mansion).
Boolean NetJoinGame(void)
{
    char address[64];

    snprintf(address, sizeof(address), "%s", gLastAddress);
    if (!DoTextEntryDialog("Address of the computer hosting the game:", address, 60))
        return false;
    return NetJoinGameAt(address);
}

/// Connects to the host at address (a name or an IP address).
Boolean NetJoinGameAt(const char *address)
{
    ResetSession();
    gNet.isHost = false;
    snprintf(gLastAddress, sizeof(gLastAddress), "%s", address);
    if (!ConnectTo(address)) {
        MMDoCustomDialog(gSpriteWorldP,
            "Couldn't connect. Check the address, and that the other computer is waiting (Multiplayer, Host).", 3);
        return false;
    }
    return GreetHost("Join a Game");
}

// ---------------------------------------------------------------------------
// Over the internet: every computer connects to the lobby server, which carries each guest's
// connection to the host (MMOnline.c)

static Boolean gOnlineRequestSent;
static char gOnlineCode[32], gOnlineHostID[16];

static Boolean OnlineHostPoll(void *unused)
{
    (void)unused;
    OnlinePoll();
    if (OnlineIsReady() && !gOnlineRequestSent) {
        OnlineHost(gNet.mode, gHostPublic, GetPlayerCharacter(0));
        gOnlineRequestSent = true;
    }
    return OnlineHostedCode() != NULL || OnlineFailed() || (gOnlineRequestSent && OnlineError()[0]);
}

/// The waiting room online: each guest the lobby announces gets a connection of their own.
static int OnlineGatherPoll(void *unused)
{
    static Uint8 leftover[4096];
    char ticket[64];
    int sock, len = 0;

    (void)unused;
    OnlinePoll();
    while (OnlineNextIncoming(ticket, sizeof(ticket))) {
        sock = OnlineAttach(ticket, leftover, &len, (int)sizeof(leftover));
        if (sock >= 0)
            AddLink(sock, leftover, len);
    }
    NetPump();
    if (getenv("MM_DEBUG_NET_START") != NULL && gNet.numPlayers >= atoi(getenv("MM_DEBUG_NET_START")))
        return 1;       // testing aid: starts once this many are in
    return OnlineFailed() ? -1 : 0;
}

static Boolean OnlineJoinPoll(void *unused)
{
    (void)unused;
    OnlinePoll();
    if (OnlineIsReady() && !gOnlineRequestSent) {
        OnlineJoin(gOnlineCode, gOnlineHostID);
        gOnlineRequestSent = true;
    }
    return OnlineIsPaired() || OnlineFailed() || (gOnlineRequestSent && OnlineError()[0]);
}

static void ShowLobbyProblem(const char *fallback)
{
    char message[400];
    snprintf(message, sizeof(message), "%s\r\r(Lobby server: %s)", OnlineError()[0] ? OnlineError() : fallback,
             OnlineLobbyAddress());
    MMDoCustomDialog(gSpriteWorldP, message, 3);
}

/// Offers a game on the lobby server and waits for players to join it.
Boolean NetHostGameOnline(Boolean isPublic)
{
    const char *code;
    Boolean started;

    StartHosting();
    gNet.online = true;
    gNet.beaconSock = -1;
    gHostPublic = isPublic;
    gOnlineRequestSent = false;
    OnlineDisconnect();
    OnlineClearError();
    OnlineConnect();
    if (!RunWaitDialog("Host Online", "Connecting to the lobby server...", OnlineHostPoll, NULL) ||
        (code = OnlineHostedCode()) == NULL) {
        if (OnlineFailed() || OnlineError()[0])
            ShowLobbyProblem("Couldn't offer the game.");
        OnlineDisconnect();
        return false;
    }
    snprintf(gHostCode, sizeof(gHostCode), "%s", code);
    gNet.active = true;
    started = RunLiveWaitDialog("Host Online", HostMessage, OnlineGatherPoll, HostCanStart, NULL);
    if (!started && OnlineFailed())
        ShowLobbyProblem("Lost the lobby server.");
    if (started)
        OnlineStartHostedGame();        // no longer listed
    OnlineDisconnect();
    gHostCode[0] = 0;
    if (!FinishGathering(started))
        return false;
    for (int p = 1; p < gNet.numPlayers; p++)
        OnlineNoteRecentPlayer(gNet.ids[p], gNet.names[p]);
    return true;
}

/// Joins a game on the lobby server: by its code, or a friend's by their ID.
Boolean NetJoinGameOnline(const char *code, const char *hostID)
{
    static Uint8 leftover[4096];
    int sock, len = 0;

    ResetSession();
    gNet.isHost = false;
    gNet.online = true;
    snprintf(gOnlineCode, sizeof(gOnlineCode), "%s", code != NULL ? code : "");
    snprintf(gOnlineHostID, sizeof(gOnlineHostID), "%s", hostID != NULL ? hostID : "");
    gOnlineRequestSent = false;
    OnlineDisconnect();
    OnlineClearError();
    OnlineConnect();
    if (!RunWaitDialog("Join Online", "Joining the game...", OnlineJoinPoll, NULL) || !OnlineIsPaired() ||
        (sock = OnlineTakePairedSocket(leftover, &len, (int)sizeof(leftover))) < 0) {
        if (OnlineFailed() || OnlineError()[0])
            ShowLobbyProblem("Couldn't join the game.");
        OnlineDisconnect();
        return false;
    }
    AddLink(sock, leftover, len);
    gNet.online = true;
    return GreetHost("Join Online");
}

// ---------------------------------------------------------------------------
// Starting a game

/// The settings every computer must share, in a fixed order.
static Boolean *SharedFunSetting(int i)
{
    Boolean *settings[] = { &gExtras.medusaLate, &gExtras.piracyMedusas, &gExtras.tooScared, &gExtras.nineLives,
                            &gExtras.espresso, &gExtras.pinata, &gExtras.moonGravity, &gExtras.allSpiders,
                            &gExtras.pitchBlack };
    return i >= 0 && i < (int)SDL_arraysize(settings) ? settings[i] : NULL;
}

/// Host: called as the game starts. Tells the others what to load.
static void SendStart(void)
{
    static Uint8 payload[65536];
    int i, n = 0;

    PutU32(payload, gNet.seed);
    n = 4;
    payload[n++] = (Uint8)gDifficultySetting;
    payload[n++] = (Uint8)(gCurrentMansionNumber & 0xff);
    payload[n++] = (Uint8)((gCurrentMansionNumber >> 8) & 0xff);
    payload[n++] = (Uint8)gMansionIsCustomMansion;
    payload[n++] = (Uint8)gEasyJumping;
    payload[n++] = gNet.beaten[0];
    for (i = 0; SharedFunSetting(i) != NULL; i++)
        payload[n++] = (Uint8)*SharedFunSetting(i);
    memcpy(payload + n, gMansionFilename, 255);
    payload[n + 254] = 0;
    n += 255;
    payload[n++] = (Uint8)gNet.mode;
    if (gNet.continuePath[0] && gNet.mode == kNetModeArcade) {
            // Arcade Split Screen: the team save went just before, whole and checked (MMTeamSave.c)
        if (TeamSaveNetSendContinue(gNet.continuePath))
            payload[n++] = 2;
        gNet.continuePath[0] = 0;
    } else if (gNet.continuePath[0]) {
            // continuing a saved game: the save and who played it go along
        char sidecar[1600];
        const char *name = strrchr(gNet.continuePath, '/');
        size_t nameLen;
        long saveLen, sideLen = 0;
        FILE *file = fopen(gNet.continuePath, "rb");
        name = name != NULL ? name + 1 : gNet.continuePath;
        nameLen = strlen(name) > 120 ? 120 : strlen(name);
        if (file != NULL) {
            payload[n++] = 1;
            payload[n++] = (Uint8)nameLen;
            memcpy(payload + n, name, nameLen);
            n += (int)nameLen;
            saveLen = (long)fread(payload + n + 4, 1, sizeof(payload) - (size_t)n - 4 - 2048, file);
            fclose(file);
            PutU32(payload + n, (Uint32)saveLen);
            n += 4 + (int)saveLen;
            snprintf(sidecar, sizeof(sidecar), "%s.txt", gNet.continuePath);
            file = fopen(sidecar, "rb");
            if (file != NULL) {
                sideLen = (long)fread(payload + n + 4, 1, 2000, file);
                fclose(file);
            }
            PutU32(payload + n, (Uint32)sideLen);
            n += 4 + (int)sideLen;
        }
        gNet.continuePath[0] = 0;
    }
    SendMessage(kMsgStart, payload, (Uint16)(n > 65535 ? 65535 : n));
}

/// Host: the next game continues this saved game (it's sent to the other computers).
void NetSetContinueSave(const char *path)
{
    snprintf(gNet.continuePath, sizeof(gNet.continuePath), "%s", path != NULL ? path : "");
}

/// Guest: the saved game the host is continuing (kept here too), or NULL for a new game.
const char *NetContinuedSavePath(void)
{
    return !gNet.isHost && gNet.continuePath[0] ? gNet.continuePath : NULL;
}

const char *NetLastSaveName(void)
{
    return gNet.lastSaveName[0] ? gNet.lastSaveName : "Saved game";
}

static void GuestWaitMessage(char *text, size_t size, void *unused)
{
    char players[400];

    (void)unused;
    PlayerList(players, sizeof(players));
    snprintf(text, size, "Waiting for %s to start and choose a mansion...\r\rPlaying: %s", gNet.names[0], players);
}

static int StartPoll(void *unused)
{
    (void)unused;
    NetPump();
    if (gNet.startReceived)
        return 1;
    return gNet.left[0] || gNet.numLinks == 0 || gNet.links[0].sock < 0 ? -1 : 0;
}

/// A guest: waits while the host gathers the players and picks a mansion, then opens the
/// same one. Returns true with the mansion open and ready to start.
Boolean NetClientWaitForStart(void)
{
    char mansionName[255];
    const char *mansionPath;
    const Uint8 *p = gNet.startPayload;
    int i, n, err;

    if (!RunLiveWaitDialog("Multiplayer", GuestWaitMessage, StartPoll, NULL, NULL) || !gNet.startReceived) {
        if (gNet.left[0] || gNet.peerLeft)
            MMDoCustomDialog(gSpriteWorldP, "The host left.", 3);
        NetEndSession();
        return false;
    }
    ChatNoteGameStarting(gNet.names, gNet.numPlayers, gNet.localPlayer, gNet.online);
    for (i = 1; i < gNet.numPlayers && gNet.online; i++)
        if (i != gNet.localPlayer)
            OnlineNoteRecentPlayer(gNet.ids[i], gNet.names[i]);
    if (gNet.online)
        OnlineNoteRecentPlayer(gNet.ids[0], gNet.names[0]);
        // the host's settings for this game
    gNet.seed = GetU32(p);
    gDifficultySetting = (char)p[4];
    gCurrentMansionNumber = (short)(p[5] | p[6] << 8);
    gEasyJumping = p[8] != 0;
    gNet.beaten[0] = p[9];
    gNet.savedExtras = gExtras;
    gNet.extrasSaved = true;
    for (i = 0, n = 10; SharedFunSetting(i) != NULL; i++, n++)
        *SharedFunSetting(i) = p[n] != 0;
    memcpy(gMansionFilename, p + n, 255);
    gMansionFilename[254] = 0;
    gNet.mode = gNet.startLen > n + 255 ? p[n + 255] : kNetModeRelay;
    MultiplayerSetNetworkPlayers();
    gNet.continuePath[0] = 0;
    if (gNet.startLen > n + 258 && p[n + 256] == 1) {
            // the host is continuing our saved game: keep its copy
        int at = n + 257, nameLen = p[at++], saveLen;
        char saveName[128];
        snprintf(saveName, sizeof(saveName), "%.*s", nameLen, (const char *)p + at);
        at += nameLen;
        saveLen = (int)GetU32(p + at);
        at += 4;
        if (at + saveLen + 4 <= gNet.startLen && saveName[0] != 0 && strchr(saveName, '/') == NULL) {
            int sideAt = at + saveLen + 4, sideLen = (int)GetU32(p + at + saveLen), saveMode;
            if (sideAt + sideLen > gNet.startLen)
                sideLen = gNet.startLen - sideAt;
            saveMode = kMultiSaveCoop;
            if (!MultiSaveStoreReceived(saveMode, saveName, p + at, saveLen, p + sideAt, sideLen, gNet.continuePath,
                                        sizeof(gNet.continuePath)))
                gNet.continuePath[0] = 0;
        }
    } else if (gNet.startLen > n + 256 && p[n + 256] == 2) {
            // Arcade Split Screen: the team save came just before, checked and kept (MMTeamSave.c)
        if (TeamSaveNetContinuePath() == NULL) {
            MMDoCustomDialog(gSpriteWorldP, "The host's saved game didn't arrive whole, so it can't be continued here.", 3);
            SendMessage(kMsgBye, NULL, 0);
            NetEndSession();
            return false;
        }
        snprintf(gNet.continuePath, sizeof(gNet.continuePath), "%s", TeamSaveNetContinuePath());
        TeamSaveNetForgetContinue();
    }

        // open the same mansion (as a replay does)
    SetCurMansionAsCustomMansion(p[7] != 0);
    snprintf(mansionName, sizeof(mansionName), "%s", gMansionFilename);
    StripStringFromEndOfCString(".zip", mansionName, NULL);
    StripStringFromEndOfCString(".ZIP", mansionName, NULL);
    mansionPath = GetPathForAnyMansionFile(mansionName);
    strcpy(gCustomMansionFolderPath, mansionPath);
    err = OpenMansionUsingFilePath(mansionPath, mansionName);
    if (err != 0 && gMansionIsCustomMansion && GetBundledCustomMansionsPath()[0] != '\0') {
            // the custom mansions that come with the game
        char bundled[1200];
        snprintf(bundled, sizeof(bundled), "%s/%s/", GetBundledCustomMansionsPath(), mansionName);
        strcpy(gCustomMansionFolderPath, bundled);
        err = OpenMansionUsingFilePath(bundled, mansionName);
    }
    if (err != 0 && gMansionIsCustomMansion)
        err = OpenMansionUsingFilePath(GetMansionFolderPath(gMansionFilename), gMansionFilename);
    if (err != 0) {
            // tell the others, then back to the main menu (MultiplayerConnect says why)
        SendMessage(kMsgNoMansion, mansionName, (Uint16)strlen(mansionName));
        gNet.missingMansion = true;
        snprintf(gNet.missingMansionName, sizeof(gNet.missingMansionName), "%s", mansionName);
        NetEndSession();
        return false;
    }
    gNet.beaten[gNet.localPlayer] = HasBeatenCurrentMansion();
    SendMessage(kMsgReady, &gNet.beaten[gNet.localPlayer], 1);
    if (gNet.mode == kNetModeRace || gNet.mode == kNetModeArcade)
        MultiplayerSetUpNetworkRace();
    return true;
}

/// Every computer, as the game starts (before the first room loads).
void NetGameStarted(void)
{
    int p;

    if (!gNet.active)
        return;
    if (gNet.isHost)
        gNet.beaten[0] = HasBeatenCurrentMansion();     // (before inGame: this player's own stats)
    gNet.inGame = true;
    gNet.frame = 0;
    gNet.queueHead = gNet.queueCount = 0;
    gNet.numPending = 0;
    gNet.saveChoice = -1;
    if (gNet.isHost) {
        gNet.seed = (Uint32)time(NULL) ^ (Uint32)SDL_GetPerformanceCounter();
        SendStart();
    }
        // stats: each computer keeps only its own player's (in their place in the game)
    if (gNet.localPlayer != 0)
        gStats[gNet.localPlayer] = gStats[0];
    for (p = 0; p < kMaxPlayers; p++)
        if (p != gNet.localPlayer)
            memset(&gStats[p], 0, sizeof(gStats[p]));
}

// ---------------------------------------------------------------------------
// The race: where each player is

/// Sends this computer's player (every frame).
void NetRaceSendState(const NetRaceState *st)
{
    Uint8 payload[32];

    if (!gNet.active)
        return;
    NetPutU16(payload, (Uint16)st->room);
    NetPutF32(payload + 2, st->x);
    NetPutF32(payload + 6, st->y);
    NetPutU16(payload + 10, (Uint16)st->frame);
    payload[12] = (Uint8)((st->flipped ? 1 : 0) | (st->visible ? 2 : 0));
    payload[13] = st->status;
    PutU32(payload + 14, st->elapsedMs);
    payload[18] = st->deaths;
    NetPutF32(payload + 19, st->scaleW);
    payload[23] = st->place;
    NetPutU16(payload + 24, (Uint16)st->mapRoom);
    NetPutU16(payload + 26, (Uint16)st->runningRoom);
    PutU32(payload + 28, st->roomMs);
    SendMessage(kMsgPhantom, payload, sizeof(payload));
}

/// Arcade Split Screen: what this computer's player did to the team's game.
void NetSendTeamOps(const Uint8 *ops, int length)
{
    if (NetArcadeActive() && length > 0 && length <= 65535)
        SendMessage(kMsgTeam, ops, (Uint16)length);
}

/// Arcade Split Screen team saves (MMTeamSave.c): the host to every guest, a guest to the host.
void NetSendSaveMessage(int type, const void *payload, int length)
{
    if (gNet.active && length >= 0 && length <= 65535)
        SendMessage((Uint8)type, payload, (Uint16)length);
}

/// Arcade Split Screen: the room this computer's player keeps, for the others in it.
void NetSendRoomSnapshot(const Uint8 *data, int length)
{
    if (NetArcadeActive() && length > 0 && length <= 65535)
        SendMessage(kMsgRoomSnap, data, (Uint16)length);
}

/// Reads what has arrived (once a frame, before NetRaceGetState).
void NetRacePump(void)
{
    NetPump();
}

/// A player on another computer as last heard of. Returns false until something has arrived;
/// gone is set once they (or the host, who carries them) have left.
Boolean NetRaceGetState(int player, NetRaceState *st, Boolean *goneP)
{
    *goneP = player < 0 || player >= kMaxPlayers || gNet.left[player] || !gNet.active;
    if (player < 0 || player >= kMaxPlayers || !gNet.stateValid[player])
        return false;
    *st = gNet.states[player];
    return true;
}

// ---------------------------------------------------------------------------
// Games on the local network

/// Starts listening for hosts' beacons.
void NetBrowseStart(void)
{
    struct sockaddr_in addr;
    int one = 1;

    if (gBrowse.open)
        return;
    gBrowse.count = 0;
    gBrowse.sock = (int)socket(AF_INET, SOCK_DGRAM, 0);
    if (gBrowse.sock < 0)
        return;
    setsockopt(gBrowse.sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
#ifdef SO_REUSEPORT
    setsockopt(gBrowse.sock, SOL_SOCKET, SO_REUSEPORT, &one, sizeof(one));
#endif
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(kBeaconPort);
    if (bind(gBrowse.sock, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        CloseSocket(&gBrowse.sock);
        return;
    }
    SetNonBlocking(gBrowse.sock);
    gBrowse.open = true;
}

void NetBrowseStop(void)
{
    CloseSocket(&gBrowse.sock);
    gBrowse.open = false;
    gBrowse.count = 0;
}

/// The games heard from in the last few seconds.
int NetBrowseGames(NetFoundGame *games, int max)
{
    struct sockaddr_in from;
    socklen_t fromLen;
    char buffer[300], address[INET_ADDRSTRLEN], *line, *save = NULL, *name;
    int got, i, n, mode, character, players;
    Uint32 token;
    Uint32 now = SDL_GetTicks();
    NetFoundGame *g;

    while (gBrowse.open) {
        fromLen = sizeof(from);
        got = (int)recvfrom(gBrowse.sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&from, &fromLen);
        if (got <= 0)
            break;
        buffer[got] = 0;
        line = strtok_r(buffer, "\n", &save);
        if (line == NULL || strcmp(line, kBeaconMagic) != 0)
            continue;
        line = strtok_r(NULL, "\n", &save);
        mode = line != NULL ? atoi(line) : 0;
        line = strtok_r(NULL, "\n", &save);
        character = line != NULL ? atoi(line) : 0;
        name = strtok_r(NULL, "\n", &save);
        line = strtok_r(NULL, "\n", &save);
        token = line != NULL ? (Uint32)strtoul(line, NULL, 10) : 0;
        line = strtok_r(NULL, "\n", &save);
        players = line != NULL ? atoi(line) : 1;
        inet_ntop(AF_INET, &from.sin_addr, address, sizeof(address));
        for (i = 0; i < gBrowse.count && gBrowse.games[i].token != token; i++)
            ;
        if (i == gBrowse.count) {
            if (gBrowse.count >= kMaxFoundGames)
                continue;
            memset(&gBrowse.games[gBrowse.count++], 0, sizeof(NetFoundGame));
        }
        g = &gBrowse.games[i];
            // the same host heard on another network: a host on this computer is best reached here
        if (g->address[0] == 0 || strncmp(address, "127.", 4) == 0)
            snprintf(g->address, sizeof(g->address), "%s", address);
        snprintf(g->hostName, sizeof(g->hostName), "%s", name != NULL ? name : "?");
        g->token = token;
        g->mode = mode;
        g->character = character;
        g->numPlayers = players;
        g->lastSeen = now;
    }
    for (i = 0; i < gBrowse.count; ) {        // gone quiet: gone
        if (now - gBrowse.games[i].lastSeen > 4000)
            gBrowse.games[i] = gBrowse.games[--gBrowse.count];
        else
            i++;
    }
    for (i = 0, n = 0; i < gBrowse.count && n < max; i++)
        games[n++] = gBrowse.games[i];
    return n;
}

Boolean NetBrowsing(void)
{
    return gBrowse.open;
}

/// Sprites' frame and move timers count game time in relay co-op (SpriteWorld's game clock):
/// a 60th of a second a frame. On the real clock each computer would animate a little
/// differently (a flame, a zapper, Jack's own frames), and pixel collisions and the random
/// numbers drawn as frames change would soon differ too.
static void SetGameClock(void)
{
    gSWGameClock = (unsigned long)((Uint64)gNet.frame * 1000 / 60);
    if (gSpriteWorldP != NULL)
        gSpriteWorldP->runningTimeCount = gSWGameClock;
}

/// Seeds the game's random numbers the same on every computer (just before the first room),
/// and starts the game clock.
void NetSeedRandom(void)
{
    if (!NetGameActive())
        return;
    SelectStream(0);
    PutSeed((long)(gNet.seed & 0x7fffffff));
    SWSeedRandom(gNet.seed);        // GetRandom's numbers too
    gSWUseGameClock = true;
    SetGameClock();
}

/// The join failed because this computer doesn't have the host's mansion: its name, else NULL.
const char *NetMissingMansion(void)
{
    return gNet.missingMansion ? gNet.missingMansionName : NULL;
}

/// Another computer went back to the menu for want of the mansion: its name, else NULL
/// (NetMissingMansionPlayer says whose).
const char *NetPeerMissingMansion(void)
{
    return gNet.peerHasNoMansion ? gNet.missingMansionName : NULL;
}

int NetMissingMansionPlayer(void)
{
    return gNet.noMansionPlayer;
}

Boolean NetPlayerHasBeatenMansion(int player)
{
    return player >= 0 && player < kMaxPlayers && gNet.beaten[player] != 0;
}

/// When the game ends: back to this computer's own settings and stats, and hang up.
void NetEndSession(void)
{
    gSWUseGameClock = false;
    if (gNet.active) {
        SendMessage(kMsgBye, NULL, 0);
        Disconnect();
    }
    if (gNet.extrasSaved) {
        gExtras = gNet.savedExtras;
        gNet.extrasSaved = false;
    }
    if (gNet.inGame) {
        if (gNet.localPlayer != 0) {
            gStats[0] = gStats[gNet.localPlayer];
            memset(&gStats[gNet.localPlayer], 0, sizeof(gStats[0]));
        }
        gNet.inGame = false;
    }
    gNet.active = false;
}

// ---------------------------------------------------------------------------
// Playing

static Uint16 KeysMask(void)
{
    Uint16 mask = 0;
    int i;

    for (i = kKeyUp; i <= kKeyAction; i++)
        if (gKeys[i])
            mask |= (Uint16)(1 << i);
    return mask;
}

/// Special events (not keys) the driver's game made; they go with the next frame.
void NetRecordEvent(char event, Boolean isDown)
{
    Uint8 e = (Uint8)event;

    if (!NetGameActive() || NetIsFollower())
        return;
        // keys travel as the key state; these are the other events a replay records
    if (e == 0xc || e == 0xd || (e >= 0xe && e <= 0x2e) || e == kNetEventDebugKill || e == kNetEventDebugTripLeft ||
        e == kNetEventDebugTripRight) {
        if (gNet.numPending < (int)sizeof(gNet.pending))
            gNet.pending[gNet.numPending++] = (Uint8)(e | (isDown ? 0 : 0x80));
    }
}

/// True while a watching computer hasn't received the next frame yet: play waits.
Boolean NetMustWait(void)
{
    if (!NetGameActive())
        return false;
    NetPump();
    if (!NetIsFollower()) {
        gNet.waitStart = 0;
        if (gNet.peerLeft) {
            NetPeerLeftDuringGame();
            return true;
        }
        return false;
    }
        // the frames already here are still played (to the end of the game, say)
    if (gNet.queueCount == 0 && gNet.peerLeft) {
        NetPeerLeftDuringGame();
        return true;
    }
        // frames have piled up (the driver's computer runs a little faster, or this one
        // stalled): play them faster, drawing only some. A PS3 can't show more than its
        // TV's 59.94 pictures a second, so only skipping pictures lets it catch up.
    SWSetSpriteWorldMaxFPS(gSpriteWorldP, gNet.queueCount > 6 ? 240 : gNet.queueCount > 3 ? 120 : 60);
    if (gNet.queueCount > 0) {
        gNet.waitStart = 0;
        return false;
    }
    if (gNet.waitStart == 0)
        gNet.waitStart = SDL_GetTicks();
    return true;
}

static Uint32 Mix(Uint32 h, Uint32 v)
{
    int i;
    for (i = 0; i < 4; i++, v >>= 8)
        h = (h ^ (v & 0xff)) * 16777619u;
    return h;
}

/// Testing aid (MM_DEBUG_CHECKSUM): each sprite layer's sprites (frame and place) in one number,
/// "layer:sprites:fingerprint", to compare the computers' logs by and see what drifted first.
static void PrintSpriteFingerprints(void)
{
    SpriteLayerPtr layerP = NULL;
    SpritePtr spriteP;
    Uint32 h, bits;
    float f;
    int layer, n;

    printf("  layers:");
    for (layer = 0; (layerP = SWGetNextSpriteLayer(gSpriteWorldP, layerP)) != NULL; layer++) {
        h = 2166136261u;
        for (spriteP = SWGetNextSprite(layerP, NULL), n = 0; spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP), n++) {
            h = Mix(h, (Uint32)spriteP->curFrameIndex);
            f = SWGetSpriteHorizLoc(spriteP);
            memcpy(&bits, &f, 4);
            h = Mix(h, bits);
            f = SWGetSpriteVertLoc(spriteP);
            memcpy(&bits, &f, 4);
            h = Mix(h, bits);
        }
        if (n > 0)
            printf(" %d:%d:%08x", layer, n, h);
    }
    printf("\n");
    fflush(stdout);
}

/// What relay co-op's computers must agree on: the hero, the score and lives, the room, and
/// the game's random numbers (CustomRandom: any difference in what happened shows up there).
/// Not GetRandom's: effects draw from it as they are drawn, and the computers draw different
/// numbers of pictures (a watcher catching up skips some).
static Uint32 GameStateChecksum(void)
{
    SpritePtr heroP = GetHeroSprite();
    Uint32 h = 2166136261u;
    long seed;
    float f;
    Uint32 bits;

    if (getenv("MM_DEBUG_CHECKSUM") != NULL) {      // testing aid: what goes into the checksum
        long s0;
        GetSeed(&s0);
        printf("checksum frame %u: score %ld lives %d items %d level %d seed %ld rand %08x hero %.3f,%.3f\n",
               (unsigned)gNet.frame, (long)gScore, gNumLivesLeft, gNumItemsBeingCarried, gCurrentLevel, s0,
               SWRandomState(), heroP ? SWGetSpriteHorizLoc(heroP) : -1.0f, heroP ? SWGetSpriteVertLoc(heroP) : -1.0f);
        PrintSpriteFingerprints();
    }
    h = Mix(h, gNet.frame);
    h = Mix(h, (Uint32)gScore);
    h = Mix(h, (Uint32)gNumLivesLeft);
    h = Mix(h, (Uint32)gNumItemsBeingCarried);
    h = Mix(h, (Uint32)gCurrentLevel);
    GetSeed(&seed);
    h = Mix(h, (Uint32)seed);
    if (heroP != NULL) {
        f = SWGetSpriteHorizLoc(heroP);
        memcpy(&bits, &f, 4);
        h = Mix(h, bits);
        f = SWGetSpriteVertLoc(heroP);
        memcpy(&bits, &f, 4);
        h = Mix(h, bits);
        h = Mix(h, (Uint32)HERO(heroP)->isDead);
    }
    return h;
}

/// Frames between the checksums (testing aid: every other frame with MM_DEBUG_CHECKSUM).
static Uint32 CheckEvery(void)
{
    static int debug = -1;

    if (debug < 0)
        debug = getenv("MM_DEBUG_CHECKSUM") != NULL;
    return debug ? 2 : kCheckEvery;
}

/// The computers' games differ: say so and end it, rather than play on in different games.
static void NetDesync(Uint32 frame, Uint32 here, Uint32 driver)
{
    char message[300];

    printf("Relay co-op out of step at frame %u: checksum %08x here, %08x on %s's computer\n", (unsigned)frame,
           (unsigned)here, (unsigned)driver, NetPlayerName(GetActivePlayer()));
    snprintf(message, sizeof(message), "The game on this computer got out of step with %s's (at frame %u), so it "
             "stops here. This is a bug: please send the log (After Midnight log.txt, next to the game's "
             "preferences).", NetPlayerName(GetActivePlayer()), (unsigned)frame);
    SendMessage(kMsgBye, NULL, 0);
    MMDoCustomDialog(gSpriteWorldP, message, 3);
    gEndGame = true;
}

/// At the start of each game frame (before the frame counter advances).
void NetFrameBegin(void)
{
    Uint8 payload[7 + 32 + 5];
    NetFrame *f;
    int i;

    if (!NetGameActive())
        return;
    if (!NetIsFollower()) {
        PutU32(payload, gNet.frame);
        payload[4] = (Uint8)(KeysMask() & 0xff);
        payload[5] = (Uint8)(KeysMask() >> 8);
        payload[6] = (Uint8)gNet.numPending;
        memcpy(payload + 7, gNet.pending, (size_t)gNet.numPending);
        if (gNet.frame % CheckEvery() == 0) {
            payload[7 + gNet.numPending] = kCheckMark;
            PutU32(payload + 8 + gNet.numPending, GameStateChecksum());
            SendMessage(kMsgFrame, payload, (Uint16)(7 + gNet.numPending + 5));
        } else {
            SendMessage(kMsgFrame, payload, (Uint16)(7 + gNet.numPending));
        }
        for (i = 0; i < gNet.numPending; i++)       // (the others take Jack there at this point too)
            if (gNet.pending[i] == kNetEventDebugTripLeft || gNet.pending[i] == kNetEventDebugTripRight)
                DebugBotMoveToEdge(gNet.pending[i] == kNetEventDebugTripLeft ? -1 : 1);
        gNet.numPending = 0;
    } else {
            // normally there: play only goes on once it has arrived (NetMustWait)
        while (gNet.queueCount == 0 && NetPump()) {
            SDL_PumpEvents();
            SDL_Delay(1);
        }
        while (gNet.queueCount > 0) {
            f = &gNet.queue[gNet.queueHead];
            if (f->frame > gNet.frame)
                break;
            gNet.queueHead = (gNet.queueHead + 1) % kMaxQueuedFrames;
            gNet.queueCount--;
            if (f->frame < gNet.frame)
                continue;           // from before a turn change: already played
            {       // behind: this frame isn't drawn (one in two, or four in five far behind)
                static int skipped = 0;
                gSWSkipDrawing = gNet.queueCount > 3 && skipped < (gNet.queueCount > 12 ? 4 : 1);
                skipped = gSWSkipDrawing ? skipped + 1 : 0;
            }
            if (f->hasCheck) {
                Uint32 here = GameStateChecksum();
                if (here != f->check) {
                    NetDesync(f->frame, here, f->check);
                    break;
                }
            }
            for (i = kKeyUp; i <= kKeyAction; i++)
                gKeys[i] = (f->keys >> i) & 1;
            for (i = 0; i < f->numEvents; i++) {
                if ((f->events[i] & 0x7f) == kNetEventDebugKill) {
                    if (GetHeroSprite() != NULL && !HERO(GetHeroSprite())->isDead)
                        KillHero(GetHeroSprite());
                } else if ((f->events[i] & 0x7f) == kNetEventDebugTripLeft ||
                           (f->events[i] & 0x7f) == kNetEventDebugTripRight) {
                    DebugBotMoveToEdge((f->events[i] & 0x7f) == kNetEventDebugTripLeft ? -1 : 1);
                } else
                    ApplyMovieEvent(f->events[i] & 0x7f, (f->events[i] & 0x80) == 0);
            }
            break;
        }
    }
    gNet.frame++;
    SetGameClock();
}

/// The watcher, waiting: a redraw now and then so "Waiting for..." shows.
void NetRedrawWhileWaiting(void)
{
    if (gNet.waitStart == 0 || SDL_GetTicks() - gNet.waitStart < 700 || SDL_GetTicks() - gNet.lastRedraw < 250) {
        SDL_Delay(1);
        return;
    }
    gNet.lastRedraw = SDL_GetTicks();
    SWUpdateSpriteWorld(gSpriteWorldP, true);
}

/// True once the watcher has been waiting a while (the player with the turn paused, say).
Boolean NetIsWaitingLong(void)
{
    return NetIsFollower() && gNet.waitStart != 0 && SDL_GetTicks() - gNet.waitStart >= 700;
}

/// The turn changed (on every computer, at the same frame).
void NetTurnChanged(void)
{
    if (!NetGameActive())
        return;
    ResetKeys();
    gNet.numPending = 0;
}

/// Save pedestal: the driver sends its choice; the watchers wait for it.
void NetSendSaveChoice(Boolean save)
{
    Uint8 payload[130];
    size_t len = strlen(gLastSaveFilename) > 120 ? 120 : strlen(gLastSaveFilename);

    payload[0] = save ? 1 : 0;
    memcpy(payload + 1, gLastSaveFilename, len);
    if (NetGameActive() && !NetIsFollower())
        SendMessage(kMsgSave, payload, (Uint16)(1 + (save ? len : 0)));
}

Boolean NetWaitForSaveChoice(void)
{
    Boolean save;

    while (gNet.saveChoice < 0 && NetPump()) {
        SDL_PumpEvents();
        SDL_Delay(5);
    }
    save = gNet.saveChoice == 1;
    gNet.saveChoice = -1;
    return save;
}

/// Someone is gone: say who, and end the game.
void NetPeerLeftDuringGame(void)
{
    static Boolean told = false;
    char message[300];
    int p, who = -1;

    if (told || !gNet.inGame)
        return;
    told = true;
    for (p = 0; p < gNet.numPlayers && who < 0; p++)
        if (gNet.left[p] && p != gNet.localPlayer)
            who = p;
    if (gNet.fellBehind)
        snprintf(message, sizeof(message), "This computer fell too far behind %s's game (it stopped for minutes), "
                 "so the game can't go on.", NetPlayerName(GetActivePlayer()));
    else if (gNet.peerHasNoMansion)
        snprintf(message, sizeof(message), "%s doesn't have the mansion \"%s\" installed, so they went back to the "
                 "main menu, and the game can't go on without them.", NetPlayerName(gNet.noMansionPlayer)[0]
                 ? NetPlayerName(gNet.noMansionPlayer) : "A player", gNet.missingMansionName);
    else if (who == 0 && !gNet.isHost)
        snprintf(message, sizeof(message), "%s (the host) left the game.", NetPlayerName(0));
    else
        snprintf(message, sizeof(message), "%s left the game.", who >= 0 ? NetPlayerName(who) : "A player");
    MMDoCustomDialog(gSpriteWorldP, message, 3);
    gEndGame = true;
    told = false;
}

/// A watcher pressed Escape: leave the game?
void NetAskToLeave(void)
{
    char message[200];

    snprintf(message, sizeof(message), "It's %s's turn. Leave the game?", NetPlayerName(GetActivePlayer()));
    if (MMDoCustomDialog(gSpriteWorldP, message, 1) == 2) {
        SendMessage(kMsgBye, NULL, 0);
        gEndGame = true;
    }
}

#else   // no sockets on this platform yet

Boolean NetPump(void) { return false; }
void NetRedrawWhileWaiting(void) {}
Boolean NetIsWaitingLong(void) { return false; }
Boolean NetHostGame(void)
{
    MMDoCustomDialog(gSpriteWorldP, "Playing on several computers isn't available on this system yet.", 3);
    return false;
}
Boolean NetJoinGame(void) { return NetHostGame(); }
Boolean NetJoinGameAt(const char *address) { (void)address; return NetHostGame(); }
Boolean NetHostGameOnline(Boolean isPublic) { (void)isPublic; return NetHostGame(); }
Boolean NetJoinGameOnline(const char *code, const char *hostID) { (void)code; (void)hostID; return NetHostGame(); }
Boolean NetClientWaitForStart(void) { return false; }
void NetGameStarted(void) {}
void NetSeedRandom(void) {}
Boolean NetPlayerHasBeatenMansion(int player) { (void)player; return false; }
void NetEndSession(void) {}
void NetRecordEvent(char event, Boolean isDown) { (void)event; (void)isDown; }
Boolean NetMustWait(void) { return false; }
void NetFrameBegin(void) {}
void NetTurnChanged(void) {}
void NetSendSaveChoice(Boolean save) { (void)save; }
Boolean NetWaitForSaveChoice(void) { return false; }
void NetPeerLeftDuringGame(void) {}
void NetAskToLeave(void) {}
void NetRaceSendState(const NetRaceState *st) { (void)st; }
void NetSendTeamOps(const Uint8 *ops, int length) { (void)ops; (void)length; }
void NetSendSaveMessage(int type, const void *payload, int length) { (void)type; (void)payload; (void)length; }
void NetSendRoomSnapshot(const Uint8 *data, int length) { (void)data; (void)length; }
void NetRacePump(void) {}
Boolean NetRaceGetState(int player, NetRaceState *st, Boolean *goneP) { (void)player; (void)st; *goneP = true; return false; }
void NetBrowseStart(void) {}
void NetBrowseStop(void) {}
int NetBrowseGames(NetFoundGame *games, int max) { (void)games; (void)max; return 0; }
Boolean NetBrowsing(void) { return false; }
const char *NetMissingMansion(void) { return NULL; }
const char *NetPeerMissingMansion(void) { return NULL; }
int NetMissingMansionPlayer(void) { return -1; }
void NetSendChat(const char *text) { (void)text; }
void NetSetContinueSave(const char *path) { (void)path; }
const char *NetContinuedSavePath(void) { return NULL; }
const char *NetLastSaveName(void) { return "Saved game"; }

#endif

/// Split screen: the network session isn't a player's.
void NetShareState(void (*share)(void *, size_t))
{
    share(&gNet, sizeof(gNet));
    share(&gBrowse, sizeof(gBrowse));
    share(&gWaitCancelled, sizeof(gWaitCancelled));
}
