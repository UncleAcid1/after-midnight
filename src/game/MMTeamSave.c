/*
 * MMTeamSave.c — Arcade Split Screen team saves, added by this version.
 *
 * A team save is one file, "<name>.mmteam", in the group's folder (MMMultiSave.c):
 *
 *     "MMTEAMSV"  format (u32)  number of sections (u32)
 *     sections:   tag (u32)  length (u32)  CRC-32 of the data (u32)  data
 *                 'INFO'  who played, the mansion, when (lines of "key = value")
 *                 'TEAM'  the team's game: what a save file holds (WriteSaveDataToFile)
 *                 'PLYR'  one per player: their ID (14 characters), then their part (where they
 *                         are or would come back, their lives) in the same form
 *     CRC-32 of everything before (u32)
 *
 * Numbers are little-endian. A file that doesn't check out isn't used, and files are written
 * to "<name>.tmp" first and then renamed, so a save is either whole or not there.
 *
 * On several computers the host makes the save, since every message goes through it in order:
 * when a player stands on a statue, the host asks every computer for its player's part
 * (kMsgSaveCollect). Each sends what its player changed in the team's game before its part, so
 * once the host has every part its own copy of the team's game has all of it. The host writes
 * the file, sends it to every computer (kMsgSaveData), each checks it, keeps it and says so
 * (kMsgSaveAck), and the host tells everyone how it went (kMsgSaveDone): "Team game saved", or
 * whose part or copy is missing. A computer that doesn't answer in time is left out, not
 * waited for. Continuing a save, the host sends the file before the game starts.
 */

#include <sys/stat.h>
#include <time.h>
#include "MMGame.h"
#include "MMProtos.h"

#define kMagic          "MMTEAMSV"
#define kFormat         1
#define kTagInfo        0x4f464e49u     ///< "INFO"
#define kTagTeam        0x4d414554u     ///< "TEAM"
#define kTagPlayer      0x52594c50u     ///< "PLYR"
#define kMaxSaveData    (1024 * 1024)
#define kChunk          60000
#define kCollectMs      8000            ///< how long the host waits for the parts
#define kDistributeMs   10000           ///< and for the copies to be kept
#define kSerialContinue 0xffffffffu     ///< the save being continued (sent before the start)

// ---------------------------------------------------------------------------
// CRC-32 (the zip one)

static Uint32 Crc32(const Uint8 *data, int length, Uint32 crc)
{
    static Uint32 table[256];
    static Boolean made = false;
    int i, k;

    if (!made) {
        for (i = 0; i < 256; i++) {
            Uint32 c = (Uint32)i;
            for (k = 0; k < 8; k++)
                c = c & 1 ? 0xedb88320u ^ (c >> 1) : c >> 1;
            table[i] = c;
        }
        made = true;
    }
    crc = ~crc;
    for (i = 0; i < length; i++)
        crc = table[(crc ^ data[i]) & 0xff] ^ (crc >> 8);
    return ~crc;
}

// ---------------------------------------------------------------------------
// The game's save data, in memory

/// What a save file holds, now: the team's game (part false) or this player's part (where
/// they are, or where they'd come back if they're down). Returns malloc'ed data or NULL.
Uint8 *TeamSaveCapture(Boolean part, int *lengthOut)
{
    Uint8 *buffer = malloc(kMaxSaveData), *result;
    SDL_RWops *rw;
    SpritePtr heroP = NULL;
    short saveLevel = gCurrentLevel;
    float h = gStartingCol, v = gStartingRow;
    Sint64 length;
    int err;

    *lengthOut = 0;
    if (buffer == NULL)
        return NULL;
    if (gCurrentGameMode == kInGameMapMode)
        gCurrentLevel = GetMapPlayerLevel();
    else if (gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kReadingSignMode)
        heroP = GetHeroSprite();
    if (heroP != NULL && !HERO(heroP)->isDead) {
        h = SWGetSpriteHorizLoc(heroP);
        v = SWGetSpriteVertLoc(heroP);
    } else if (part && gCurrentGameMode != kInGameMapMode) {
        gCurrentLevel = gLastLevelPositionWasSaved;     // down: back where they'd come back
    }
    rw = SDL_RWFromMem(buffer, kMaxSaveData);
    ClearFileStickyError();
    WriteSaveDataToFile(rw, h, v, !part);
    err = GetFileStickyError();
    length = SDL_RWtell(rw);
    SDL_RWclose(rw);
    gCurrentLevel = saveLevel;
    if (err != 0 || length <= 0) {
        free(buffer);
        return NULL;
    }
    result = realloc(buffer, (size_t)length);
    *lengthOut = (int)length;
    return result != NULL ? result : buffer;
}

// ---------------------------------------------------------------------------
// The file

void TeamSaveFree(TeamSave *t)
{
    int i;

    free(t->team);
    for (i = 0; i < kMaxPlayers; i++)
        free(t->parts[i]);
    memset(t, 0, sizeof(*t));
}

static void PutU32(Uint8 *p, Uint32 v) { NetPutU32(p, v); }
static Uint32 GetU32(const Uint8 *p) { return NetGetU32(p); }

static int PutSection(Uint8 *out, Uint32 tag, const Uint8 *a, int aLen, const Uint8 *b, int bLen)
{
    Uint32 crc = Crc32(a, aLen, 0);

    crc = Crc32(b, bLen, crc);
    PutU32(out, tag);
    PutU32(out + 4, (Uint32)(aLen + bLen));
    PutU32(out + 8, crc);
    if (aLen > 0)
        memcpy(out + 12, a, (size_t)aLen);
    if (bLen > 0)
        memcpy(out + 12 + aLen, b, (size_t)bLen);
    return 12 + aLen + bLen;
}

static void WriteInfo(const TeamSave *t, char *text, size_t size)
{
    size_t n;
    int p;

    n = (size_t)snprintf(text, size, "name = %s\nmode = arcade\nmansion = %s\nsaved = %lu\nplayers = %d\n", t->name,
                         t->mansion, (unsigned long)t->savedAt, t->numPlayers);
    for (p = 0; p < t->numPlayers && n < size; p++)
        n += (size_t)snprintf(text + n, size - n, "player = %s\t%d\t0\t%s\n", t->ids[p], t->characters[p], t->names[p]);
}

/// The file's bytes (malloc'ed), or NULL.
Uint8 *TeamSaveEncode(const TeamSave *t, int *lengthOut)
{
    char info[2048];
    int length, p, n;
    Uint8 *out;

    WriteInfo(t, info, sizeof(info));
    length = 16 + 12 + (int)strlen(info) + 12 + t->teamLength + 4;
    for (p = 0; p < t->numPlayers; p++)
        if (t->parts[p] != NULL)
            length += 12 + 14 + t->partLengths[p];
    out = malloc((size_t)length);
    if (out == NULL)
        return NULL;
    memcpy(out, kMagic, 8);
    PutU32(out + 8, kFormat);
    n = 16;
    n += PutSection(out + n, kTagInfo, (const Uint8 *)info, (int)strlen(info), NULL, 0);
    n += PutSection(out + n, kTagTeam, t->team, t->teamLength, NULL, 0);
    for (p = 0; p < t->numPlayers; p++)
        if (t->parts[p] != NULL)
            n += PutSection(out + n, kTagPlayer, (const Uint8 *)t->ids[p], 14, t->parts[p], t->partLengths[p]);
    for (p = 0, length = 2; p < t->numPlayers; p++)      // the number of sections: info, team, parts
        length += t->parts[p] != NULL;
    PutU32(out + 12, (Uint32)length);
    PutU32(out + n, Crc32(out, n, 0));                  // then the CRC of it all
    n += 4;
    *lengthOut = n;
    return out;
}

static void ReadInfo(const char *text, int length, TeamSave *t)
{
    char line[400];
    int i = 0, k;

    while (i < length) {
        for (k = 0; i < length && text[i] != '\n' && k < (int)sizeof(line) - 1; i++)
            line[k++] = text[i];
        line[k] = 0;
        i++;
        if (strncmp(line, "name = ", 7) == 0)
            snprintf(t->name, sizeof(t->name), "%s", line + 7);
        else if (strncmp(line, "mansion = ", 10) == 0)
            snprintf(t->mansion, sizeof(t->mansion), "%s", line + 10);
        else if (strncmp(line, "saved = ", 8) == 0)
            t->savedAt = (Uint32)strtoul(line + 8, NULL, 10);
        else if (strncmp(line, "player = ", 9) == 0 && t->numPlayers < kMaxPlayers) {
            int p = t->numPlayers++;
            char *tab;
            snprintf(t->ids[p], sizeof(t->ids[p]), "%.14s", line + 9);
            tab = strchr(line + 9, '\t');
            if (tab != NULL) {
                t->characters[p] = atoi(tab + 1);
                tab = strchr(tab + 1, '\t');
                if (tab != NULL)
                    tab = strchr(tab + 1, '\t');
            }
            if (tab != NULL)
                snprintf(t->names[p], sizeof(t->names[p]), "%s", tab + 1);
        }
    }
}

/// Reads a team save from its bytes, checking every part. infoOnly: who and what, not the data.
Boolean TeamSaveDecode(const Uint8 *data, int length, TeamSave *t, Boolean infoOnly)
{
    int n = 16, count, s, p;

    memset(t, 0, sizeof(*t));
    if (length < 20 || memcmp(data, kMagic, 8) != 0 || GetU32(data + 8) != kFormat)
        return false;
    if (Crc32(data, length - 4, 0) != GetU32(data + length - 4))
        return false;           // damaged
    count = (int)GetU32(data + 12);
    for (s = 0; s < count; s++) {
        Uint32 tag, len, crc;
        if (n + 12 > length - 4)
            goto bad;
        tag = GetU32(data + n);
        len = GetU32(data + n + 4);
        crc = GetU32(data + n + 8);
        n += 12;
        if (len > (Uint32)(length - 4 - n) || Crc32(data + n, (int)len, 0) != crc)
            goto bad;
        if (tag == kTagInfo) {
            ReadInfo((const char *)data + n, (int)len, t);
        } else if (tag == kTagTeam && !infoOnly) {
            t->team = malloc(len > 0 ? len : 1);
            if (t->team == NULL)
                goto bad;
            memcpy(t->team, data + n, len);
            t->teamLength = (int)len;
        } else if (tag == kTagPlayer && !infoOnly && len >= 14) {
            char id[16];
            snprintf(id, sizeof(id), "%.14s", (const char *)data + n);
            for (p = 0; p < t->numPlayers && strcmp(t->ids[p], id) != 0; p++)
                ;
            if (p < t->numPlayers && t->parts[p] == NULL) {
                t->parts[p] = malloc(len - 14 > 0 ? len - 14 : 1);
                if (t->parts[p] == NULL)
                    goto bad;
                memcpy(t->parts[p], data + n + 14, len - 14);
                t->partLengths[p] = (int)len - 14;
            }
        }
        n += (int)len;
    }
    if (t->name[0] == 0 || t->numPlayers < 2 || (!infoOnly && t->team == NULL))
        goto bad;
    return true;
bad:
    TeamSaveFree(t);
    return false;
}

static Uint8 *ReadWholeFile(const char *path, int *lengthOut)
{
    FILE *file = fopen(path, "rb");
    Uint8 *data = NULL;
    long length;

    *lengthOut = 0;
    if (file == NULL)
        return NULL;
    if (fseek(file, 0, SEEK_END) == 0 && (length = ftell(file)) > 0 && length < 16 * 1024 * 1024 &&
        fseek(file, 0, SEEK_SET) == 0 && (data = malloc((size_t)length)) != NULL) {
        if (fread(data, 1, (size_t)length, file) == (size_t)length)
            *lengthOut = (int)length;
        else {
            free(data);
            data = NULL;
        }
    }
    fclose(file);
    return data;
}

Boolean TeamSaveRead(const char *path, TeamSave *t, Boolean infoOnly)
{
    int length;
    Uint8 *data = ReadWholeFile(path, &length);
    Boolean ok = data != NULL && TeamSaveDecode(data, length, t, infoOnly);

    free(data);
    return ok;
}

/// Writes bytes to path whole or not at all: to "<path>.tmp", then renamed over it.
Boolean TeamSaveWriteBytes(const char *path, const Uint8 *data, int length)
{
    char temp[1600];
    FILE *file;
    Boolean ok;

    snprintf(temp, sizeof(temp), "%s.tmp", path);
    file = fopen(temp, "wb");
    if (file == NULL)
        return false;
    ok = fwrite(data, 1, (size_t)length, file) == (size_t)length;
    ok = fflush(file) == 0 && ok;
    ok = fclose(file) == 0 && ok;
    if (!ok) {
        remove(temp);
        return false;
    }
    remove(path);           // (the PS3 doesn't rename over a file)
    if (rename(temp, path) != 0) {
        remove(temp);
        return false;
    }
    return true;
}

/// Writes a team save into the group's folder ("<folder>/<name>.mmteam").
Boolean TeamSaveStore(const char *folder, const TeamSave *t)
{
    char path[1600];
    int length;
    Uint8 *data = TeamSaveEncode(t, &length);
    Boolean ok;

    if (data == NULL)
        return false;
    snprintf(path, sizeof(path), "%s/%s%s", folder, t->name, kTeamSaveExtension);
    ok = TeamSaveWriteBytes(path, data, length);
    free(data);
    if (ok)
        printf("Team game saved: %s\n", path);
    return ok;
}

/// A name nobody in the folder has yet: the mansion's, numbered ("Falcon Manor Normal 3").
void TeamSaveNewName(const char *folder, char *name, size_t size)
{
    char mansion[255], path[1600];
    struct stat info;
    int n;

    snprintf(mansion, sizeof(mansion), "%s", gCurOpenMansionFileName);
    StripStringFromEndOfCString(".zip", mansion, NULL);
    StripStringFromEndOfCString(".ZIP", mansion, NULL);
    for (n = 1; n < 10000; n++) {
        snprintf(name, size, "%s %d", mansion, n);
        snprintf(path, sizeof(path), "%s/%s%s", folder, name, kTeamSaveExtension);
        if (stat(path, &info) == 0)
            continue;
        snprintf(path, sizeof(path), "%s/%s", folder, name);       // an older save of that name
        if (stat(path, &info) != 0)
            break;
    }
}

/// Fills in who plays and the mansion (the save's name, the team's game and the parts are the
/// caller's).
void TeamSaveDescribeGroup(TeamSave *t, char ids[][16], int numPlayers)
{
    int p;

    snprintf(t->mansion, sizeof(t->mansion), "%s", gCurOpenMansionFileName);
    StripStringFromEndOfCString(".zip", t->mansion, NULL);
    StripStringFromEndOfCString(".ZIP", t->mansion, NULL);
    t->savedAt = (Uint32)time(NULL);
    t->numPlayers = numPlayers;
    for (p = 0; p < numPlayers; p++) {
        snprintf(t->ids[p], sizeof(t->ids[p]), "%s", ids[p]);
        snprintf(t->names[p], sizeof(t->names[p]), "%s", GetPlayerProfileName(p));
        t->characters[p] = GetPlayerCharacter(p);
    }
}

/// A player's part, by ID (NULL if it isn't in the save).
const Uint8 *TeamSavePart(const TeamSave *t, const char *id, int *lengthOut)
{
    int p;

    for (p = 0; p < t->numPlayers; p++)
        if (strcmp(t->ids[p], id) == 0 && t->parts[p] != NULL) {
            *lengthOut = t->partLengths[p];
            return t->parts[p];
        }
    *lengthOut = 0;
    return NULL;
}

// ---------------------------------------------------------------------------
// Several computers

typedef struct Incoming {
    Uint32  serial;
    Uint8  *data;
    int     total, got;
} Incoming;

static struct {
        // the host's save in progress
    Boolean active;
    int     phase;              ///< 1 gathering the parts, 2 sending the file out
    Uint32  serial;
    char    name[128];
    Uint32  deadline;
    Boolean ownPartPending;
    Incoming parts[kMaxPlayers];
    Boolean  partDone[kMaxPlayers];
    Boolean  acked[kMaxPlayers], ackOK[kMaxPlayers];
    Boolean  requestedAgain;    ///< a statue was used while saving: save again after
    Uint32   nextSerial;
        // a guest
    Boolean  capturePending;
    Uint32   captureSerial;
    Incoming file;
    char     fileName[128];
    char     continuePath[1500];
} gNetSave;

static Boolean IsHost(void)
{
    return NetLocalPlayer() == 0;
}

static void SendData(Uint32 serial, int kind, const Uint8 *data, int length)
{
    static Uint8 payload[13 + kChunk];
    int offset = 0, n;

    do {
        n = length - offset > kChunk ? kChunk : length - offset;
        PutU32(payload, serial);
        payload[4] = (Uint8)kind;
        PutU32(payload + 5, (Uint32)length);
        PutU32(payload + 9, (Uint32)offset);
        memcpy(payload + 13, data + offset, (size_t)n);
        NetSendSaveMessage(kMsgSaveData, payload, 13 + n);
        offset += n;
    } while (offset < length);
}

/// Adds a chunk; true once the whole thing has arrived.
static Boolean TakeChunk(Incoming *in, Uint32 serial, const Uint8 *p, int length)
{
    int total = (int)GetU32(p + 5), offset = (int)GetU32(p + 9), n = length - 13;

    if (total <= 0 || total > 16 * 1024 * 1024 || offset < 0 || n < 0 || offset + n > total)
        return false;
    if (in->data == NULL || in->serial != serial || in->total != total) {
        free(in->data);
        in->data = malloc((size_t)total);
        in->serial = serial;
        in->total = total;
        in->got = 0;
        if (in->data == NULL)
            return false;
    }
    if (offset != in->got)
        return false;           // (in order, over TCP: anything else is a muddle)
    memcpy(in->data + offset, p + 13, (size_t)n);
    in->got += n;
    return in->got == in->total;
}

static void FreeIncoming(Incoming *in)
{
    free(in->data);
    memset(in, 0, sizeof(*in));
}

static void ShowNotice(const char *text, Boolean ok)
{
    SplitShowTeamSaveNotice(text, ok);
}

/// Host: starts a save (a statue was used here or on another computer).
static void HostStartSave(void)
{
    const char *folder = MultiSaveFolderForCurrentGame(kMultiSaveArcade);
    Uint8 payload[200];
    int p, n;

    if (gNetSave.active) {
        gNetSave.requestedAgain = true;
        return;
    }
    if (folder == NULL) {
        ShowNotice("The team's game can't be saved (a player has no ID)", false);
        return;
    }
    memset(gNetSave.parts, 0, sizeof(gNetSave.parts));
    for (p = 0; p < kMaxPlayers; p++) {
        gNetSave.partDone[p] = false;
        gNetSave.acked[p] = gNetSave.ackOK[p] = false;
    }
    gNetSave.active = true;
    gNetSave.phase = 1;
    gNetSave.serial = ++gNetSave.nextSerial ^ ((Uint32)time(NULL) << 8);
    if (gNetSave.serial == kSerialContinue || gNetSave.serial == 0)
        gNetSave.serial = 1;
    TeamSaveNewName(folder, gNetSave.name, sizeof(gNetSave.name));
    gNetSave.deadline = SDL_GetTicks() + kCollectMs;
    gNetSave.ownPartPending = true;
    n = (int)strlen(gNetSave.name);
    PutU32(payload, gNetSave.serial);
    memcpy(payload + 4, gNetSave.name, (size_t)n);
    NetSendSaveMessage(kMsgSaveCollect, payload, 4 + n);
}

/// A player here stood on a save statue (Arcade Split Screen on several computers).
void TeamSaveNetRequest(void)
{
    if (IsHost())
        HostStartSave();
    else
        NetSendSaveMessage(kMsgSaveRequest, NULL, 0);
}

static Boolean Waiting(int p)
{
    return p != NetLocalPlayer() && !NetPlayerLeft(p);
}

/// Host: every part is in (or the time is up): the file, here and out to the others.
static void HostWriteAndSend(void)
{
    char ids[kMaxPlayers][16];
    const char *folder = MultiSaveFolderForCurrentGame(kMultiSaveArcade);
    TeamSave t;
    Uint8 *data;
    int p, length;

    memset(&t, 0, sizeof(t));
    for (p = 0; p < NetNumPlayers(); p++)
        snprintf(ids[p], sizeof(ids[p]), "%s", NetPlayerID(p));
    TeamSaveDescribeGroup(&t, ids, NetNumPlayers());
    snprintf(t.name, sizeof(t.name), "%s", gNetSave.name);
    t.team = TeamSaveCapture(false, &t.teamLength);
    for (p = 0; p < t.numPlayers; p++)
        if (gNetSave.partDone[p]) {
            t.parts[p] = gNetSave.parts[p].data;            // (given to t, freed with it)
            t.partLengths[p] = gNetSave.parts[p].total;
            gNetSave.parts[p].data = NULL;
        }
    data = t.team != NULL ? TeamSaveEncode(&t, &length) : NULL;
    gNetSave.acked[NetLocalPlayer()] = true;
    gNetSave.ackOK[NetLocalPlayer()] = false;
    if (data != NULL && folder != NULL) {
        char path[1600];
        snprintf(path, sizeof(path), "%s/%s%s", folder, t.name, kTeamSaveExtension);
        gNetSave.ackOK[NetLocalPlayer()] = TeamSaveWriteBytes(path, data, length);
        SendData(gNetSave.serial, 2, data, length);
    }
    free(data);
    TeamSaveFree(&t);
    for (p = 0; p < kMaxPlayers; p++)
        FreeIncoming(&gNetSave.parts[p]);
    gNetSave.phase = 2;
    gNetSave.deadline = SDL_GetTicks() + kDistributeMs;
}

/// Host: everyone has answered (or the time is up): how it went, to everyone.
static void HostFinish(void)
{
    char text[300], missingParts[200] = "", missingCopies[200] = "";
    Uint8 payload[320];
    Boolean ok = gNetSave.ackOK[NetLocalPlayer()];
    int p, n;

    for (p = 0; p < NetNumPlayers(); p++) {
        const char *name = NetPlayerName(p)[0] ? NetPlayerName(p) : "a player";
        if (!gNetSave.partDone[p] && p != NetLocalPlayer()) {
            n = (int)strlen(missingParts);
            snprintf(missingParts + n, sizeof(missingParts) - n, "%s%s", n ? ", " : "", name);
        }
        if (p != NetLocalPlayer() && !(gNetSave.acked[p] && gNetSave.ackOK[p])) {
            n = (int)strlen(missingCopies);
            snprintf(missingCopies + n, sizeof(missingCopies) - n, "%s%s", n ? ", " : "", name);
        }
    }
    if (!ok)
        snprintf(text, sizeof(text), "The team's game couldn't be saved");
    else if (missingParts[0])
        snprintf(text, sizeof(text), "Team game saved, without where %s was", missingParts);
    else if (missingCopies[0])
        snprintf(text, sizeof(text), "Team game saved (not on %s's computer)", missingCopies);
    else
        snprintf(text, sizeof(text), "Team game saved");
    PutU32(payload, gNetSave.serial);
    payload[4] = ok && !missingParts[0] && !missingCopies[0];
    n = (int)strlen(text);
    memcpy(payload + 5, text, (size_t)n);
    NetSendSaveMessage(kMsgSaveDone, payload, 5 + n);
    ShowNotice(text, payload[4] != 0);
    gNetSave.active = false;
    if (gNetSave.requestedAgain) {
        gNetSave.requestedAgain = false;
        HostStartSave();
    }
}

/// Once a frame, between the games (this computer's player's globals are in): captures this
/// player's part when asked for it, and moves the host's save along.
void TeamSaveNetFrame(void)
{
    int p, length;
    Uint8 *part;
    Boolean all;

    if (!NetArcadeActive())
        return;
    if (gNetSave.capturePending) {
        gNetSave.capturePending = false;
        part = TeamSaveCapture(true, &length);
        if (part != NULL)
            SendData(gNetSave.captureSerial, 1, part, length);
        free(part);
    }
    if (!IsHost() || !gNetSave.active)
        return;
    if (gNetSave.ownPartPending) {
        Incoming *own = &gNetSave.parts[NetLocalPlayer()];
        gNetSave.ownPartPending = false;
        FreeIncoming(own);
        own->data = TeamSaveCapture(true, &own->total);
        own->got = own->total;
        gNetSave.partDone[NetLocalPlayer()] = own->data != NULL;
    }
    if (gNetSave.phase == 1) {
        for (all = true, p = 0; p < NetNumPlayers(); p++)
            if (Waiting(p) && !gNetSave.partDone[p])
                all = false;
        if (all || (Sint32)(SDL_GetTicks() - gNetSave.deadline) >= 0)
            HostWriteAndSend();
    } else if (gNetSave.phase == 2) {
        for (all = true, p = 0; p < NetNumPlayers(); p++)
            if (Waiting(p) && !gNetSave.acked[p])
                all = false;
        if (all || (Sint32)(SDL_GetTicks() - gNetSave.deadline) >= 0)
            HostFinish();
    }
}

/// Guest: a whole team save arrived: checked, then kept in the group's folder. Returns true if
/// it's good; pathOut (if given) is where it went.
static Boolean KeepReceivedFile(const Uint8 *data, int length, char *pathOut, size_t size)
{
    const char *folder = MultiSaveFolderForCurrentGame(kMultiSaveArcade);
    char path[1600];
    TeamSave t;

    if (folder == NULL || !TeamSaveDecode(data, length, &t, true))
        return false;
    if (strchr(t.name, '/') != NULL) {
        TeamSaveFree(&t);
        return false;
    }
    snprintf(path, sizeof(path), "%s/%s%s", folder, t.name, kTeamSaveExtension);
    TeamSaveFree(&t);
    if (!TeamSaveWriteBytes(path, data, length))
        return false;
    if (pathOut != NULL)
        snprintf(pathOut, size, "%s", path);
    return true;
}

/// A save message from another computer (MMNet.c).
void TeamSaveNetMessage(int type, int from, const Uint8 *p, int length)
{
    Uint32 serial = length >= 4 ? GetU32(p) : 0;
    Uint8 reply[5];

    switch (type) {
        case kMsgSaveRequest:           // host: a guest's player stood on a statue
            if (IsHost() && NetArcadeActive())
                HostStartSave();
            break;
        case kMsgSaveCollect:           // guest: send my player's part
            if (!IsHost() && length >= 4) {
                gNetSave.capturePending = true;
                gNetSave.captureSerial = serial;
            }
            break;
        case kMsgSaveData:
            if (length < 13)
                break;
            if (IsHost() && p[4] == 1 && from >= 0 && from < kMaxPlayers && gNetSave.active && gNetSave.phase == 1 &&
                serial == gNetSave.serial) {
                if (TakeChunk(&gNetSave.parts[from], serial, p, length))
                    gNetSave.partDone[from] = true;
            } else if (!IsHost() && (p[4] == 2 || p[4] == 3)) {
                if (TakeChunk(&gNetSave.file, serial, p, length)) {
                    Boolean ok;
                    if (p[4] == 3) {
                        ok = KeepReceivedFile(gNetSave.file.data, gNetSave.file.total, gNetSave.continuePath,
                                              sizeof(gNetSave.continuePath));
                        if (!ok)
                            gNetSave.continuePath[0] = 0;
                    } else {
                        ok = KeepReceivedFile(gNetSave.file.data, gNetSave.file.total, NULL, 0);
                        PutU32(reply, serial);
                        reply[4] = ok;
                        NetSendSaveMessage(kMsgSaveAck, reply, 5);
                    }
                    FreeIncoming(&gNetSave.file);
                }
            }
            break;
        case kMsgSaveAck:
            if (IsHost() && length >= 5 && from >= 0 && from < kMaxPlayers && gNetSave.active && serial == gNetSave.serial) {
                gNetSave.acked[from] = true;
                gNetSave.ackOK[from] = p[4] != 0;
            }
            break;
        case kMsgSaveDone:
            if (!IsHost() && length >= 5) {
                char text[300];
                snprintf(text, sizeof(text), "%.*s", length - 5 > 299 ? 299 : length - 5, (const char *)p + 5);
                ShowNotice(text, p[4] != 0);
            }
            break;
        default:
            break;
    }
}

/// Host: continuing a team save: the file goes to everyone before the game starts.
Boolean TeamSaveNetSendContinue(const char *path)
{
    TeamSave t;
    Uint8 *data = NULL;
    int length = 0;

    if (MultiSaveLoadTeam(path, &t)) {      // (an older save goes as one file too)
        data = TeamSaveEncode(&t, &length);
        TeamSaveFree(&t);
    }
    if (data == NULL)
        return false;
    SendData(kSerialContinue, 3, data, length);
    free(data);
    return true;
}

/// Guest: where the save the host is continuing went (NULL if it didn't arrive whole).
const char *TeamSaveNetContinuePath(void)
{
    return gNetSave.continuePath[0] ? gNetSave.continuePath : NULL;
}

/// A new game on several computers: nothing in progress.
void TeamSaveNetReset(void)
{
    int p;

    for (p = 0; p < kMaxPlayers; p++)
        FreeIncoming(&gNetSave.parts[p]);
    FreeIncoming(&gNetSave.file);
    gNetSave.active = false;
    gNetSave.capturePending = false;
    gNetSave.requestedAgain = false;
}

void TeamSaveNetForgetContinue(void)
{
    gNetSave.continuePath[0] = 0;
}
