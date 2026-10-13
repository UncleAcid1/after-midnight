/*
 * MMOnline.c — playing over the internet, added by this version: who you are (a unique ID
 * for each player profile, shown as a friend code), your friends and the people you've
 * played with, and the lobby server that introduces two computers and carries their game.
 *
 * Over the internet neither computer can usually reach the other (home routers don't let
 * connections in), so both connect out to the lobby server (server/mmhd_lobby.py). There a
 * host offers a game — public (listed for everyone) or private (only friends see it; anyone
 * with its code can join) — and a joiner picks it. The server then pairs the two
 * connections and relays the bytes between them, so the game's own messages (MMNet.c) run
 * over it exactly as over a local network.
 *
 * Files, next to the preferences:
 *   Player IDs.txt         each profile's ID ("ABCD-EFGH-JKLM<tab>Ann")
 *   Online.txt             the lobby server's address
 *   Friends/<ID>.txt       a profile's friends and recent players
 */

#include <errno.h>
#include <time.h>
#include "MMGame.h"
#include "MMProtos.h"

#if !defined(_WIN32) && !defined(MM_PS3)    // the PS3 joins in at roadmap step 7 (docs/PS3.md)
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#define MM_HAVE_SOCKETS 1
#endif

#define kLobbyProto         1
#define kDefaultLobby       "129.158.224.129:47480"
#define kListEveryMs        3000
#define kMaxFriendsKept     kMaxOnlineFriends

// ---------------------------------------------------------------------------
// A little JSON reader (the lobby's messages)

enum { kJNull, kJBool, kJNum, kJStr, kJArr, kJObj };

typedef struct JVal {
    int     type;
    char   *key;
    char   *str;
    double  num;
    int     child, next;
} JVal;

typedef struct JDoc {
    JVal    v[600];
    int     n;
    char    buf[16384];
    int     used;
    const char *p;
} JDoc;

static void JSkip(JDoc *d)
{
    while (*d->p == ' ' || *d->p == '\t' || *d->p == '\r' || *d->p == '\n')
        d->p++;
}

static char *JString(JDoc *d)
{
    char *out = d->buf + d->used, *start = out;
    char *limit = d->buf + sizeof(d->buf) - 8;

    if (*d->p != '"')
        return NULL;
    d->p++;
    while (*d->p && *d->p != '"' && out < limit) {
        if (*d->p == '\\' && d->p[1]) {
            d->p++;
            switch (*d->p) {
                case 'n': *out++ = '\n'; break;
                case 't': *out++ = '\t'; break;
                case 'r': *out++ = '\r'; break;
                case 'b': case 'f': break;
                case 'u': {
                    unsigned code = 0;
                    int i;
                    for (i = 1; i <= 4 && d->p[i]; i++) {
                        char c = d->p[i];
                        code = code * 16 + (unsigned)(c >= '0' && c <= '9' ? c - '0' : (c | 32) >= 'a' && (c | 32) <= 'f' ? (c | 32) - 'a' + 10 : 0);
                    }
                    d->p += 4;
                    if (code < 0x80) *out++ = (char)code;
                    else if (code < 0x800) { *out++ = (char)(0xc0 | code >> 6); *out++ = (char)(0x80 | (code & 0x3f)); }
                    else { *out++ = (char)(0xe0 | code >> 12); *out++ = (char)(0x80 | ((code >> 6) & 0x3f)); *out++ = (char)(0x80 | (code & 0x3f)); }
                    break;
                }
                default: *out++ = *d->p; break;
            }
            d->p++;
        } else {
            *out++ = *d->p++;
        }
    }
    if (*d->p == '"')
        d->p++;
    *out++ = 0;
    d->used = (int)(out - d->buf);
    return start;
}

static int JValue(JDoc *d, int depth)
{
    int i, last = -1, c;
    JVal *v;

    JSkip(d);
    if (d->n >= (int)SDL_arraysize(d->v) || depth > 8)
        return -1;
    i = d->n++;
    v = &d->v[i];
    memset(v, 0, sizeof(*v));
    v->child = v->next = -1;
    if (*d->p == '{' || *d->p == '[') {
        Boolean isObject = *d->p == '{';
        v->type = isObject ? kJObj : kJArr;
        d->p++;
        for (;;) {
            char *key = NULL;
            JSkip(d);
            if (*d->p == (isObject ? '}' : ']')) {
                d->p++;
                break;
            }
            if (*d->p == 0)
                break;
            if (isObject) {
                key = JString(d);
                JSkip(d);
                if (*d->p == ':')
                    d->p++;
            }
            c = JValue(d, depth + 1);
            if (c < 0)
                return -1;
            d->v[c].key = key;
            if (last < 0)
                d->v[i].child = c;
            else
                d->v[last].next = c;
            last = c;
            JSkip(d);
            if (*d->p == ',')
                d->p++;
        }
    } else if (*d->p == '"') {
        v->type = kJStr;
        v->str = JString(d);
    } else if (strncmp(d->p, "true", 4) == 0) {
        v->type = kJBool;
        v->num = 1;
        d->p += 4;
    } else if (strncmp(d->p, "false", 5) == 0) {
        v->type = kJBool;
        d->p += 5;
    } else if (strncmp(d->p, "null", 4) == 0) {
        d->p += 4;
    } else {
        char *end;
        v->type = kJNum;
        v->num = strtod(d->p, &end);
        if (end == d->p)
            return -1;
        d->p = end;
    }
    return i;
}

static int JParse(JDoc *d, const char *text)
{
    d->n = 0;
    d->used = 0;
    d->p = text;
    return JValue(d, 0);
}

static int JGet(JDoc *d, int obj, const char *key)
{
    int c;
    if (obj < 0 || d->v[obj].type != kJObj)
        return -1;
    for (c = d->v[obj].child; c >= 0; c = d->v[c].next)
        if (d->v[c].key != NULL && strcmp(d->v[c].key, key) == 0)
            return c;
    return -1;
}

static const char *JStr(JDoc *d, int obj, const char *key)
{
    int i = JGet(d, obj, key);
    return i >= 0 && d->v[i].type == kJStr && d->v[i].str != NULL ? d->v[i].str : "";
}

static double JNum(JDoc *d, int obj, const char *key)
{
    int i = JGet(d, obj, key);
    return i >= 0 && (d->v[i].type == kJNum || d->v[i].type == kJBool) ? d->v[i].num : 0;
}

/// A string for a JSON message, quoted and escaped.
static void JQuote(char *out, size_t size, const char *text)
{
    size_t n = 0;
    if (size < 3)
        return;
    out[n++] = '"';
    for (; *text && n + 8 < size; text++) {
        unsigned char c = (unsigned char)*text;
        if (c == '"' || c == '\\') {
            out[n++] = '\\';
            out[n++] = (char)c;
        } else if (c < 0x20) {
            n += (size_t)snprintf(out + n, size - n, "\\u%04x", c);
        } else {
            out[n++] = (char)c;
        }
    }
    out[n++] = '"';
    out[n] = 0;
}

// ---------------------------------------------------------------------------
// Who you are: an ID for each profile

static const char kIDAlphabet[] = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";

static void MakeNewID(char *out)
{
    Uint8 bytes[12];
    FILE *random = fopen("/dev/urandom", "rb");
    Uint64 mix = SDL_GetPerformanceCounter() ^ ((Uint64)time(NULL) << 20) ^ (Uint64)(uintptr_t)out;
    int i, j;

    if (random == NULL || fread(bytes, 1, sizeof(bytes), random) != sizeof(bytes)) {
        for (i = 0; i < (int)sizeof(bytes); i++) {
            mix = mix * 6364136223846793005ULL + 1442695040888963407ULL;
            bytes[i] = (Uint8)(mix >> 33);
        }
    }
    if (random != NULL)
        fclose(random);
    for (i = 0, j = 0; i < 12; i++) {
        out[j++] = kIDAlphabet[bytes[i] & 31];
        if (i == 3 || i == 7)
            out[j++] = '-';
    }
    out[j] = 0;
}

/// The ID of a player profile ("ABCD-EFGH-JKLM"), made the first time it's asked for.
const char *OnlineProfileID(const char *profile)
{
    static char id[4][16];
    static int next = 0;
    char line[300], *tab, *result = id[next = (next + 1) % 4];
    FILE *file;
    size_t len;

    result[0] = 0;
    if (profile == NULL || profile[0] == 0)
        return result;
    file = fopen(GetAppSupportPath("Player IDs.txt"), "r");
    if (file != NULL) {
        while (fgets(line, sizeof(line), file) != NULL) {
            len = strlen(line);
            while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
                line[--len] = 0;
            tab = strchr(line, '\t');
            if (tab == NULL || line[0] == '#')
                continue;
            *tab = 0;
            if (strcmp(tab + 1, profile) == 0 && strlen(line) == 14) {
                snprintf(result, 16, "%s", line);
                break;
            }
        }
        fclose(file);
    }
    if (result[0] == 0) {
        MakeNewID(result);
        file = fopen(GetAppSupportPath("Player IDs.txt"), "a");
        if (file != NULL) {
            if (ftell(file) == 0)
                fputs("# Midnight Mansion HD: each player profile's ID (its friend code online)\n", file);
            fprintf(file, "%s\t%s\n", result, profile);
            fclose(file);
        }
    }
    return result;
}

/// The signed-in player's name and ID.
static const char *CurrentProfileName(void)
{
    static char name[256];
    SWCopyPascalStringToCString((ConstStringPtr)gCurrentPlayerName, name);
    return name;
}

const char *OnlineMyID(void)
{
    return OnlineProfileID(CurrentProfileName());
}

// ---------------------------------------------------------------------------
// Friends and recent players (per profile)

static struct {
    char        ownerID[16];
    OnlineFriend friends[kMaxFriendsKept];
    int         numFriends;
    OnlineFriend recent[kMaxFriendsKept];
    int         numRecent;
} gPeople;

static const char *PeoplePath(const char *ownerID)
{
    static char path[1200];
    char folder[1024];

    snprintf(folder, sizeof(folder), "%s", GetAppSupportPath(""));
    MMCreateDirectory(folder, "Friends");
    snprintf(path, sizeof(path), "%s/Friends/%s.txt", folder, ownerID);
    return path;
}

static void SavePeople(void)
{
    FILE *file;
    int i;

    if (gPeople.ownerID[0] == 0)
        return;
    file = fopen(PeoplePath(gPeople.ownerID), "w");
    if (file == NULL)
        return;
    fprintf(file, "# Midnight Mansion HD: %s's friends and the people they played with online\n",
            CurrentProfileName());
    for (i = 0; i < gPeople.numFriends; i++)
        fprintf(file, "friend\t%s\t%s\n", gPeople.friends[i].id, gPeople.friends[i].name);
    for (i = 0; i < gPeople.numRecent; i++)
        fprintf(file, "recent\t%s\t%s\t%lu\n", gPeople.recent[i].id, gPeople.recent[i].name,
                (unsigned long)gPeople.recent[i].lastPlayed);
    fclose(file);
}

/// Reads the signed-in profile's friends (again, if the profile changed).
static void LoadPeople(void)
{
    const char *owner = OnlineMyID();
    char line[300], *fields[4], *p;
    FILE *file;
    int n;

    if (strcmp(owner, gPeople.ownerID) == 0)
        return;
    memset(&gPeople, 0, sizeof(gPeople));
    snprintf(gPeople.ownerID, sizeof(gPeople.ownerID), "%s", owner);
    file = fopen(PeoplePath(owner), "r");
    if (file == NULL)
        return;
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        if (line[0] == '#')
            continue;
        for (n = 0, p = line; n < 4 && p != NULL; n++) {
            fields[n] = p;
            p = strchr(p, '\t');
            if (p != NULL)
                *p++ = 0;
        }
        if (n < 3 || strlen(fields[1]) != 14)
            continue;
        if (strcmp(fields[0], "friend") == 0 && gPeople.numFriends < kMaxFriendsKept) {
            OnlineFriend *f = &gPeople.friends[gPeople.numFriends++];
            snprintf(f->id, sizeof(f->id), "%s", fields[1]);
            snprintf(f->name, sizeof(f->name), "%s", fields[2]);
        } else if (strcmp(fields[0], "recent") == 0 && gPeople.numRecent < kMaxFriendsKept) {
            OnlineFriend *f = &gPeople.recent[gPeople.numRecent++];
            snprintf(f->id, sizeof(f->id), "%s", fields[1]);
            snprintf(f->name, sizeof(f->name), "%s", fields[2]);
            f->lastPlayed = n > 3 ? (Uint32)strtoul(fields[3], NULL, 10) : 0;
        }
    }
    fclose(file);
}

int OnlineGetFriends(OnlineFriend *out, int max)
{
    int i;
    LoadPeople();
    for (i = 0; i < gPeople.numFriends && i < max; i++)
        out[i] = gPeople.friends[i];
    return i;
}

/// The people played with lately who aren't friends yet (newest first).
int OnlineGetRecentPlayers(OnlineFriend *out, int max)
{
    int i, n = 0;
    LoadPeople();
    for (i = 0; i < gPeople.numRecent && n < max; i++)
        if (!OnlineIsFriend(gPeople.recent[i].id))
            out[n++] = gPeople.recent[i];
    return n;
}

Boolean OnlineIsFriend(const char *id)
{
    int i;
    LoadPeople();
    for (i = 0; i < gPeople.numFriends; i++)
        if (SDL_strcasecmp(gPeople.friends[i].id, id) == 0)
            return true;
    return false;
}

/// Normalizes what someone typed as a friend code ("abcd efgh jklm" -> "ABCD-EFGH-JKLM").
/// Returns false if it isn't one.
Boolean OnlineNormalizeID(const char *typed, char *out)
{
    int n = 0, j = 0;
    char c;

    for (; *typed; typed++) {
        c = (char)SDL_toupper((unsigned char)*typed);
        if (c == 'O') c = '0';                      // the ones people mix up
        if (c == 'I' || c == 'L') c = '1';
        if (c == ' ' || c == '-')
            continue;
        if (strchr(kIDAlphabet, c) == NULL || n >= 12)
            return false;
        out[j++] = c;
        n++;
        if (n == 4 || n == 8)
            out[j++] = '-';
    }
    out[j] = 0;
    return n == 12;
}

Boolean OnlineAddFriend(const char *id, const char *name)
{
    char norm[16];
    int i;

    LoadPeople();
    if (!OnlineNormalizeID(id, norm) || strcmp(norm, gPeople.ownerID) == 0)
        return false;
    for (i = 0; i < gPeople.numFriends; i++)
        if (strcmp(gPeople.friends[i].id, norm) == 0) {
            if (name != NULL && name[0])
                snprintf(gPeople.friends[i].name, sizeof(gPeople.friends[i].name), "%s", name);
            SavePeople();
            return true;
        }
    if (gPeople.numFriends >= kMaxFriendsKept)
        return false;
    snprintf(gPeople.friends[gPeople.numFriends].id, 16, "%s", norm);
    snprintf(gPeople.friends[gPeople.numFriends].name, sizeof(gPeople.friends[0].name), "%s",
             name != NULL && name[0] ? name : "Friend");
    gPeople.numFriends++;
    SavePeople();
    return true;
}

void OnlineRemoveFriend(const char *id)
{
    int i;
    LoadPeople();
    for (i = 0; i < gPeople.numFriends; i++)
        if (strcmp(gPeople.friends[i].id, id) == 0) {
            gPeople.friends[i] = gPeople.friends[--gPeople.numFriends];
            SavePeople();
            return;
        }
}

/// After playing with someone online: they go to the top of the recent players.
void OnlineNoteRecentPlayer(const char *id, const char *name)
{
    char norm[16];
    int i;

    LoadPeople();
    if (!OnlineNormalizeID(id, norm) || strcmp(norm, gPeople.ownerID) == 0)
        return;
    for (i = 0; i < gPeople.numRecent && strcmp(gPeople.recent[i].id, norm) != 0; i++)
        ;
    if (i == gPeople.numRecent) {
        if (gPeople.numRecent < kMaxFriendsKept)
            gPeople.numRecent++;
        i = gPeople.numRecent - 1;
    }
    for (; i > 0; i--)
        gPeople.recent[i] = gPeople.recent[i - 1];
    snprintf(gPeople.recent[0].id, 16, "%s", norm);
    snprintf(gPeople.recent[0].name, sizeof(gPeople.recent[0].name), "%s", name);
    gPeople.recent[0].lastPlayed = (Uint32)time(NULL);
        // a friend's new name
    for (i = 0; i < gPeople.numFriends; i++)
        if (strcmp(gPeople.friends[i].id, norm) == 0 && name[0])
            snprintf(gPeople.friends[i].name, sizeof(gPeople.friends[i].name), "%s", name);
    SavePeople();
}

// ---------------------------------------------------------------------------
// The lobby server's address ("Online.txt", or MM_LOBBY)

static char gLobbyAddress[200] = "";

const char *OnlineLobbyAddress(void)
{
    const char *env = getenv("MM_LOBBY");
    FILE *file;
    char line[300], value[200];

    if (env != NULL && env[0])
        return env;
    if (gLobbyAddress[0] == 0) {
        snprintf(gLobbyAddress, sizeof(gLobbyAddress), "%s", kDefaultLobby);
        file = fopen(GetAppSupportPath("Online.txt"), "r");
        if (file != NULL) {
            while (fgets(line, sizeof(line), file) != NULL)
                if (sscanf(line, " lobby = %199s", value) == 1)
                    snprintf(gLobbyAddress, sizeof(gLobbyAddress), "%s", value);
            fclose(file);
        }
    }
    return gLobbyAddress;
}

void OnlineSetLobbyAddress(const char *address)
{
    FILE *file;

    snprintf(gLobbyAddress, sizeof(gLobbyAddress), "%s", address);
    file = fopen(GetAppSupportPath("Online.txt"), "w");
    if (file != NULL) {
        fprintf(file, "# Midnight Mansion HD: the lobby server for playing over the internet (host:port)\n");
        fprintf(file, "lobby = %s\n", gLobbyAddress);
        fclose(file);
    }
}

// ---------------------------------------------------------------------------
// The lobby connection

enum { kLobbyOff, kLobbyConnecting, kLobbyReady, kLobbyFailed, kLobbyPaired };

static struct {
    int         state;
    int         sock;
    Uint32      connectStart;
    char        in[32768];
    int         inLen;
    Uint32      lastList;
    Boolean     browsing;
    char        error[200];
    char        motd[200];
    char        hostedCode[32];
    Boolean     hosted;
    char        peerID[16], peerName[64];
    int         peerMode;
    Boolean     isHost;
    OnlineGame  games[kMaxOnlineGames];
    int         numGames;
    OnlineFriendStatus friends[kMaxOnlineFriends];
    int         numFriendStatus;
    Boolean     listArrived;
    char        incoming[kMaxPlayers][64];  ///< hosting: guests the lobby is holding for us
    int         numIncoming;
} gLobby = { kLobbyOff, -1 };

#ifdef MM_HAVE_SOCKETS

static void LobbyClose(void)
{
    if (gLobby.sock >= 0)
        close(gLobby.sock);
    gLobby.sock = -1;
    gLobby.inLen = 0;
}

static void LobbyFail(const char *why)
{
    LobbyClose();
    gLobby.state = kLobbyFailed;
    if (why != gLobby.error)
        snprintf(gLobby.error, sizeof(gLobby.error), "%s", why);
}

static void LobbySend(const char *line)
{
    size_t len = strlen(line), sent = 0;
    ssize_t n;
    int flags = 0;
    Uint32 start = SDL_GetTicks();

#ifdef MSG_NOSIGNAL
    flags = MSG_NOSIGNAL;
#endif
    if (gLobby.sock < 0)
        return;
    while (sent < len) {
        n = send(gLobby.sock, line + sent, len - sent, flags);
        if (n > 0)
            sent += (size_t)n;
        else if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) && SDL_GetTicks() - start < 3000)
            SDL_Delay(1);
        else {
            LobbyFail("Lost the connection to the lobby server.");
            return;
        }
    }
}

static void SendHelloToLobby(void)
{
    char line[400], name[160];

    JQuote(name, sizeof(name), CurrentProfileName());
    snprintf(line, sizeof(line), "{\"op\":\"hello\",\"proto\":%d,\"id\":\"%s\",\"name\":%s}\n", kLobbyProto,
             OnlineMyID(), name);
    LobbySend(line);
}

/// Starts connecting to the lobby (if not already).
void OnlineConnect(void)
{
    struct addrinfo hints, *result = NULL;
    char host[200], *colon, port[16] = "47480";
    int flags, one = 1;

    if (gLobby.state == kLobbyConnecting || gLobby.state == kLobbyReady)
        return;
    LobbyClose();
    gLobby.error[0] = 0;
    gLobby.hosted = false;
    gLobby.hostedCode[0] = 0;
    gLobby.numGames = 0;
    gLobby.numFriendStatus = 0;
    gLobby.listArrived = false;
    gLobby.numIncoming = 0;
    snprintf(host, sizeof(host), "%s", OnlineLobbyAddress());
    colon = strrchr(host, ':');
    if (colon != NULL) {
        *colon = 0;
        snprintf(port, sizeof(port), "%s", colon + 1);
    }
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host, port, &hints, &result) != 0 || result == NULL) {
        LobbyFail("Can't find the lobby server.");
        return;
    }
    gLobby.sock = (int)socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (gLobby.sock < 0) {
        freeaddrinfo(result);
        LobbyFail("Can't use the network.");
        return;
    }
    flags = fcntl(gLobby.sock, F_GETFL, 0);
    fcntl(gLobby.sock, F_SETFL, flags | O_NONBLOCK);
    setsockopt(gLobby.sock, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
#ifdef SO_NOSIGPIPE
    setsockopt(gLobby.sock, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof(one));
#endif
    if (connect(gLobby.sock, result->ai_addr, result->ai_addrlen) != 0 && errno != EINPROGRESS) {
        freeaddrinfo(result);
        LobbyFail("Can't reach the lobby server.");
        return;
    }
    freeaddrinfo(result);
    gLobby.state = kLobbyConnecting;
    gLobby.connectStart = SDL_GetTicks();
}

void OnlineDisconnect(void)
{
    if (gLobby.state == kLobbyPaired)
        return;                 // the game's connection now (NetAdoptOnlineSocket took it)
    LobbyClose();
    gLobby.state = kLobbyOff;
    gLobby.browsing = false;
    gLobby.hosted = false;
}

static void HandleLobbyMessage(const char *line)
{
    static JDoc doc;
    int root = JParse(&doc, line), arr, c;
    const char *op;

    if (root < 0)
        return;
    op = JStr(&doc, root, "op");
    if (strcmp(op, "welcome") == 0) {
        snprintf(gLobby.motd, sizeof(gLobby.motd), "%s", JStr(&doc, root, "motd"));
    } else if (strcmp(op, "error") == 0) {
        snprintf(gLobby.error, sizeof(gLobby.error), "%s", JStr(&doc, root, "msg"));
    } else if (strcmp(op, "hosted") == 0) {
        snprintf(gLobby.hostedCode, sizeof(gLobby.hostedCode), "%s", JStr(&doc, root, "code"));
        gLobby.hosted = true;
    } else if (strcmp(op, "games") == 0) {
        gLobby.numGames = 0;
        arr = JGet(&doc, root, "games");
        for (c = arr >= 0 ? doc.v[arr].child : -1; c >= 0 && gLobby.numGames < kMaxOnlineGames; c = doc.v[c].next) {
            OnlineGame *g = &gLobby.games[gLobby.numGames++];
            memset(g, 0, sizeof(*g));
            snprintf(g->code, sizeof(g->code), "%s", JStr(&doc, c, "code"));
            snprintf(g->hostName, sizeof(g->hostName), "%s", JStr(&doc, c, "host"));
            snprintf(g->hostID, sizeof(g->hostID), "%s", JStr(&doc, c, "host_id"));
            g->mode = (int)JNum(&doc, c, "mode");
            g->isPublic = JNum(&doc, c, "public") != 0;
            g->isFriend = JNum(&doc, c, "friend") != 0;
            g->character = (int)JNum(&doc, c, "character");
            g->numPlayers = (int)JNum(&doc, c, "players");
        }
        gLobby.numFriendStatus = 0;
        arr = JGet(&doc, root, "friends");
        for (c = arr >= 0 ? doc.v[arr].child : -1; c >= 0 && gLobby.numFriendStatus < kMaxOnlineFriends;
             c = doc.v[c].next) {
            OnlineFriendStatus *f = &gLobby.friends[gLobby.numFriendStatus++];
            snprintf(f->id, sizeof(f->id), "%s", JStr(&doc, c, "id"));
            snprintf(f->name, sizeof(f->name), "%s", JStr(&doc, c, "name"));
            snprintf(f->code, sizeof(f->code), "%s", JStr(&doc, c, "code"));
            f->online = JNum(&doc, c, "online") != 0;
        }
        gLobby.listArrived = true;
    } else if (strcmp(op, "incoming") == 0) {
            // hosting: someone wants to join; we open a connection for them (OnlineAttach)
        if (gLobby.numIncoming < kMaxPlayers)
            snprintf(gLobby.incoming[gLobby.numIncoming++], sizeof(gLobby.incoming[0]), "%s", JStr(&doc, root, "ticket"));
    } else if (strcmp(op, "paired") == 0) {
        c = JGet(&doc, root, "peer");
        snprintf(gLobby.peerID, sizeof(gLobby.peerID), "%s", JStr(&doc, c, "id"));
        snprintf(gLobby.peerName, sizeof(gLobby.peerName), "%s", JStr(&doc, c, "name"));
        gLobby.peerMode = (int)JNum(&doc, root, "mode");
        gLobby.isHost = strcmp(JStr(&doc, root, "role"), "host") == 0;
        gLobby.state = kLobbyPaired;
    }
}

static void SendList(void)
{
    char line[4096], quoted[64];
    size_t n;
    int i;

    LoadPeople();
    n = (size_t)snprintf(line, sizeof(line), "{\"op\":\"list\",\"friends\":[");
    for (i = 0; i < gPeople.numFriends && n + 40 < sizeof(line); i++) {
        JQuote(quoted, sizeof(quoted), gPeople.friends[i].id);
        n += (size_t)snprintf(line + n, sizeof(line) - n, "%s%s", i ? "," : "", quoted);
    }
    snprintf(line + n, sizeof(line) - n, "]}\n");
    LobbySend(line);
    gLobby.lastList = SDL_GetTicks();
}

/// Call often: finishes connecting, reads the lobby's messages, asks for the list of games.
void OnlinePoll(void)
{
    fd_set writable;
    struct timeval zero = { 0, 0 };
    int err = 0, got, used;
    Boolean hungUp = false;
    socklen_t errLen = sizeof(err);
    char *newline;

    if (gLobby.state == kLobbyConnecting) {
        FD_ZERO(&writable);
        FD_SET(gLobby.sock, &writable);
        if (select(gLobby.sock + 1, NULL, &writable, NULL, &zero) == 1) {
            if (getsockopt(gLobby.sock, SOL_SOCKET, SO_ERROR, &err, &errLen) != 0 || err != 0) {
                LobbyFail("Can't reach the lobby server.");
                return;
            }
            gLobby.state = kLobbyReady;
            SendHelloToLobby();
            if (gLobby.browsing)
                SendList();
        } else if (SDL_GetTicks() - gLobby.connectStart > 8000) {
            LobbyFail("The lobby server didn't answer.");
        }
        return;
    }
    if (gLobby.state != kLobbyReady)
        return;
    for (;;) {
        if (gLobby.inLen >= (int)sizeof(gLobby.in) - 1)
            gLobby.inLen = 0;           // a line far too long: drop it
        got = (int)recv(gLobby.sock, gLobby.in + gLobby.inLen, sizeof(gLobby.in) - 1 - (size_t)gLobby.inLen, 0);
        if (got > 0)
            gLobby.inLen += got;
        else if (got == 0) {
            hungUp = true;          // (after reading what it said last: why, as a rule)
            break;
        } else {
            if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
                hungUp = true;
            break;
        }
    }
    used = 0;
    while (gLobby.state == kLobbyReady) {
        gLobby.in[gLobby.inLen] = 0;
        newline = memchr(gLobby.in + used, '\n', (size_t)(gLobby.inLen - used));
        if (newline == NULL)
            break;
        *newline = 0;
        HandleLobbyMessage(gLobby.in + used);
        used = (int)(newline - gLobby.in) + 1;
    }
        // after "paired" what's left is the other computer's game data: kept for NetAdopt
    if (used > 0) {
        memmove(gLobby.in, gLobby.in + used, (size_t)(gLobby.inLen - used));
        gLobby.inLen -= used;
    }
    if (hungUp && gLobby.state == kLobbyReady) {
        LobbyFail(gLobby.error[0] ? gLobby.error : "Lost the connection to the lobby server.");
        return;
    }
    if (gLobby.state == kLobbyReady && gLobby.browsing && SDL_GetTicks() - gLobby.lastList >= kListEveryMs)
        SendList();
}

void OnlineBrowse(Boolean on)
{
    gLobby.browsing = on;
    if (on) {
        if (gLobby.state == kLobbyOff || gLobby.state == kLobbyFailed)
            OnlineConnect();
        else if (gLobby.state == kLobbyReady)
            SendList();
    }
}

/// Offers a game (once connected). mode: kNetModeRelay or kNetModeRace.
void OnlineHost(int mode, Boolean isPublic, int character)
{
    char line[200];

    gLobby.hosted = false;
    gLobby.hostedCode[0] = 0;
    gLobby.error[0] = 0;
    snprintf(line, sizeof(line), "{\"op\":\"host\",\"mode\":%d,\"public\":%s,\"character\":%d}\n", mode,
             isPublic ? "true" : "false", character);
    LobbySend(line);
}

/// Joins a game by its code, or a friend's game by their ID.
void OnlineJoin(const char *code, const char *hostID)
{
    char line[200], quoted[80];

    gLobby.error[0] = 0;
    if (code != NULL && code[0]) {
        JQuote(quoted, sizeof(quoted), code);
        snprintf(line, sizeof(line), "{\"op\":\"join\",\"code\":%s}\n", quoted);
    } else {
        JQuote(quoted, sizeof(quoted), hostID != NULL ? hostID : "");
        snprintf(line, sizeof(line), "{\"op\":\"join\",\"host_id\":%s}\n", quoted);
    }
    LobbySend(line);
}

/// Hosting: the next guest waiting to be let in (their ticket), if any.
Boolean OnlineNextIncoming(char *ticket, size_t size)
{
    if (gLobby.numIncoming == 0)
        return false;
    snprintf(ticket, size, "%s", gLobby.incoming[0]);
    memmove(&gLobby.incoming[0], &gLobby.incoming[1], sizeof(gLobby.incoming[0]) * (size_t)(gLobby.numIncoming - 1));
    gLobby.numIncoming--;
    return true;
}

/// Hosting: a new connection to the lobby for one guest (their ticket). Returns the socket,
/// which then carries that guest's game, or -1. Waits a few seconds at most.
int OnlineAttach(const char *ticket, Uint8 *leftover, int *leftoverLen, int max)
{
    struct addrinfo hints, *result = NULL;
    char host[200], *colon, port[16] = "47480", line[200], quoted[100], buf[4096], *newline;
    int sock, got, len = 0, flags;
    Uint32 start;
    fd_set fds;
    struct timeval timeout;

    *leftoverLen = 0;
    snprintf(host, sizeof(host), "%s", OnlineLobbyAddress());
    colon = strrchr(host, ':');
    if (colon != NULL) {
        *colon = 0;
        snprintf(port, sizeof(port), "%s", colon + 1);
    }
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host, port, &hints, &result) != 0 || result == NULL)
        return -1;
    sock = (int)socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sock < 0) {
        freeaddrinfo(result);
        return -1;
    }
    flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    if (connect(sock, result->ai_addr, result->ai_addrlen) != 0 && errno != EINPROGRESS) {
        freeaddrinfo(result);
        close(sock);
        return -1;
    }
    freeaddrinfo(result);
    FD_ZERO(&fds);
    FD_SET(sock, &fds);
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    if (select(sock + 1, NULL, &fds, NULL, &timeout) != 1) {
        close(sock);
        return -1;
    }
    JQuote(quoted, sizeof(quoted), ticket);
    snprintf(line, sizeof(line), "{\"op\":\"attach\",\"ticket\":%s}\n", quoted);
    if (send(sock, line, strlen(line), 0) != (ssize_t)strlen(line)) {
        close(sock);
        return -1;
    }
        // until "paired": what follows is the guest's game
    for (start = SDL_GetTicks(); SDL_GetTicks() - start < 5000;) {
        got = (int)recv(sock, buf + len, sizeof(buf) - 1 - (size_t)len, 0);
        if (got > 0) {
            len += got;
            buf[len] = 0;
            if ((newline = memchr(buf, '\n', (size_t)len)) != NULL) {
                int rest = len - (int)(newline + 1 - buf);
                if (strstr(buf, "\"paired\"") == NULL || strstr(buf, "\"paired\"") > newline) {
                    close(sock);
                    return -1;
                }
                if (rest > max)
                    rest = max;
                memcpy(leftover, newline + 1, (size_t)rest);
                *leftoverLen = rest;
                return sock;
            }
            if (len >= (int)sizeof(buf) - 1)
                break;
        } else if (got == 0 || (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)) {
            break;
        } else {
            SDL_Delay(5);
        }
    }
    close(sock);
    return -1;
}

/// Hosting: everyone's in; the lobby stops listing the game.
void OnlineStartHostedGame(void)
{
    if (gLobby.state == kLobbyReady)
        LobbySend("{\"op\":\"start\"}\n");
}

/// Hands the paired connection over (it now carries the game). Returns the socket, or -1.
int OnlineTakePairedSocket(Uint8 *leftover, int *leftoverLen, int max)
{
    int sock = gLobby.sock, n;

    if (gLobby.state != kLobbyPaired || sock < 0)
        return -1;
    n = gLobby.inLen < max ? gLobby.inLen : max;
    memcpy(leftover, gLobby.in, (size_t)n);
    *leftoverLen = n;
    gLobby.sock = -1;
    gLobby.inLen = 0;
    gLobby.state = kLobbyOff;
    gLobby.browsing = false;
    return sock;
}

#else

void OnlineConnect(void) { snprintf(gLobby.error, sizeof(gLobby.error), "Not available on this system yet."); gLobby.state = kLobbyFailed; }
void OnlineDisconnect(void) {}
void OnlinePoll(void) {}
void OnlineBrowse(Boolean on) { (void)on; OnlineConnect(); }
void OnlineHost(int mode, Boolean isPublic, int character) { (void)mode; (void)isPublic; (void)character; }
void OnlineJoin(const char *code, const char *hostID) { (void)code; (void)hostID; }
int OnlineTakePairedSocket(Uint8 *leftover, int *leftoverLen, int max) { (void)leftover; (void)max; *leftoverLen = 0; return -1; }
Boolean OnlineNextIncoming(char *ticket, size_t size) { (void)ticket; (void)size; return false; }
int OnlineAttach(const char *ticket, Uint8 *leftover, int *leftoverLen, int max) { (void)ticket; (void)leftover; (void)max; *leftoverLen = 0; return -1; }
void OnlineStartHostedGame(void) {}

#endif

Boolean OnlineIsConnecting(void) { return gLobby.state == kLobbyConnecting; }
Boolean OnlineIsReady(void) { return gLobby.state == kLobbyReady; }
Boolean OnlineFailed(void) { return gLobby.state == kLobbyFailed; }
Boolean OnlineIsPaired(void) { return gLobby.state == kLobbyPaired; }
Boolean OnlineListArrived(void) { return gLobby.listArrived; }
const char *OnlineError(void) { return gLobby.error; }
const char *OnlineMessageOfTheDay(void) { return gLobby.motd; }
const char *OnlineHostedCode(void) { return gLobby.hosted ? gLobby.hostedCode : NULL; }
const char *OnlinePeerID(void) { return gLobby.peerID; }
const char *OnlinePeerName(void) { return gLobby.peerName; }
void OnlineClearError(void) { gLobby.error[0] = 0; }

int OnlineGetGames(OnlineGame *out, int max)
{
    int i;
    for (i = 0; i < gLobby.numGames && i < max; i++)
        out[i] = gLobby.games[i];
    return i;
}

/// A friend's status as the lobby last said (false if not heard of).
Boolean OnlineFriendStatusFor(const char *id, OnlineFriendStatus *out)
{
    int i;
    for (i = 0; i < gLobby.numFriendStatus; i++)
        if (strcmp(gLobby.friends[i].id, id) == 0) {
            *out = gLobby.friends[i];
            return true;
        }
    return false;
}

/// Split screen: the lobby connection isn't a player's.
void OnlineShareState(void (*share)(void *, size_t))
{
    share(&gLobby, sizeof(gLobby));
    share(&gPeople, sizeof(gPeople));
    share(gLobbyAddress, sizeof(gLobbyAddress));
}
