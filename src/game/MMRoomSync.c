/*
 * MMRoomSync.c — Arcade Split Screen (co-op): everyone in a room sees the same room, added by
 * this version.
 *
 * Each player runs their own copy of the room they are in (MMSplit.c), and things that move by
 * themselves — moving and falling platforms, trap doors, enemies, zappers — would drift apart
 * between the copies: a platform under one player's feet could be somewhere else in another
 * player's view. So the player who has been in a room longest keeps it: after each of their
 * frames the state of everything that moves there is noted down (a RoomSnapshot), and the
 * others in that room take it at the start of their frame, then run one frame of their own
 * from there (which comes out the same, unless their own player changed something). A player
 * who walks in takes the room as it is at once. If the keeper leaves, opens the map or is
 * saving, the next player in the room keeps it, from where it was.
 *
 * Enemies chase and notice the keeper's player (theirs is the copy that counts); a player who
 * kills one or steps on a falling block still does it for everyone (MMCoopSync.c).
 *
 * On several computers the keeper's computer sends the snapshot to the others in the room a
 * few times a second (kMsgRoomSnap); in between each runs the room on its own. Snapshots are
 * written field by field in a fixed byte order (the PS3 is big-endian), procs by number
 * (MMProcTable.h, the same list on every computer running this version).
 */

#include <stddef.h>
#include "MMGame.h"
#include "MMProtos.h"
#include "MMProcTable.h"

#define kSendEvery      3       ///< frames between the snapshots sent to other computers
#define kMaxAge         30      ///< frames a snapshot from another computer stays good

// ---------------------------------------------------------------------------
// Procs by number

#define X(name) name,
static const MoveProcPtr kMoveProcs[] = { NULL, MM_MOVE_PROCS };
static const FrameProcPtr kFrameProcs[] = { NULL, MM_FRAME_PROCS };
#undef X

enum { kUnknownProc = 0xffff };

static Uint16 MoveProcNumber(MoveProcPtr proc)
{
    Uint16 i;
    for (i = 0; i < (Uint16)SDL_arraysize(kMoveProcs); i++)
        if (kMoveProcs[i] == proc)
            return i;
    return kUnknownProc;
}

static Uint16 FrameProcNumber(FrameProcPtr proc)
{
    Uint16 i;
    for (i = 0; i < (Uint16)SDL_arraysize(kFrameProcs); i++)
        if (kFrameProcs[i] == proc)
            return i;
    return kUnknownProc;
}

// ---------------------------------------------------------------------------
// What is kept for each kind of sprite: its own fields, without the ones that point at other
// sprites (each copy has its own)

typedef struct SyncField {
    Uint16  offset;
    Uint8   size;           ///< 1, 2, 4 (a long or a float) or 8 (a double)
    Uint8   count;
} SyncField;

#define F(type, field)      { (Uint16)offsetof(type, field), (Uint8)sizeof(((type *)0)->field), 1 }
#define FA(type, field, n)  { (Uint16)offsetof(type, field), (Uint8)sizeof(((type *)0)->field[0]), n }
#define RECT(type, field)   F(type, field.top), F(type, field.left), F(type, field.bottom), F(type, field.right)
#define END                 { 0, 0, 0 }

static const SyncField kPlatform[] = {
    RECT(PlatformSpriteRec, collisionInset), F(PlatformSpriteRec, isMoving), F(PlatformSpriteRec, startVertLoc),
    F(PlatformSpriteRec, reappearTimer), F(PlatformSpriteRec, fallDelay), F(PlatformSpriteRec, moveDeltaH),
    F(PlatformSpriteRec, moveDeltaV), END
};
static const SyncField kSkullSpider[] = {
    RECT(SkullSpiderSpriteRec, collisionInset), F(SkullSpiderSpriteRec, stepSound), F(SkullSpiderSpriteRec, frameTimer),
    F(SkullSpiderSpriteRec, frameDirection), F(SkullSpiderSpriteRec, frameDelay), END
};
static const SyncField kCrawler[] = {
    F(CrawlerSpriteRec, frameTimer), F(CrawlerSpriteRec, frameDirection), RECT(CrawlerSpriteRec, collisionInset), END
};
static const SyncField kEyeball[] = {
    RECT(EyeballSpriteRec, collisionInset), F(EyeballSpriteRec, bounceSpeed), F(EyeballSpriteRec, normalHeight),
    F(EyeballSpriteRec, normalWidth), F(EyeballSpriteRec, landedVertLoc), F(EyeballSpriteRec, landedHorizLoc),
    FA(EyeballSpriteRec, squishHeights, kMaxEyeballSquishFrames), F(EyeballSpriteRec, numSquishFrames),
    F(EyeballSpriteRec, squishTimer), END
};
static const SyncField kRaven[] = {
    RECT(RavenSpriteRec, collisionInset), F(RavenSpriteRec, frameTimer), F(RavenSpriteRec, frameDirection),
    F(RavenSpriteRec, baseVertLoc), F(RavenSpriteRec, angle), F(RavenSpriteRec, amplitude),
    F(RavenSpriteRec, angleStep), F(RavenSpriteRec, ignoresWalls), END
};
static const SyncField kMedusa[] = {
    F(MedusaSpriteRec, frameDelay), F(MedusaSpriteRec, frameTimer), F(MedusaSpriteRec, frameDirection),
    RECT(MedusaSpriteRec, collisionInset), F(MedusaSpriteRec, soundTimer), F(MedusaSpriteRec, speed),
    F(MedusaSpriteRec, fallSpeed), F(MedusaSpriteRec, fallAccel), F(MedusaSpriteRec, ignoresWalls), END
};
static const SyncField kFish[] = {
    F(FishSpriteRec, restVertLoc), F(FishSpriteRec, firstFrame), F(FishSpriteRec, frameDelay),
    F(FishSpriteRec, frameTimer), F(FishSpriteRec, frameDirection), F(FishSpriteRec, waitTime),
    F(FishSpriteRec, waitTimer), F(FishSpriteRec, jumpSpeed), F(FishSpriteRec, gravity), F(FishSpriteRec, hasTurned),
    END
};
static const SyncField kSkeleton[] = {
    F(SkeletonSpriteRec, frameDelay), F(SkeletonSpriteRec, frameTimer), F(SkeletonSpriteRec, frameDirection),
    RECT(SkeletonSpriteRec, collisionInset), F(SkeletonSpriteRec, facingRight), F(SkeletonSpriteRec, hasUncollapsed),
    F(SkeletonSpriteRec, shakeCount), F(SkeletonSpriteRec, wakeMode), F(SkeletonSpriteRec, wakeDelay),
    F(SkeletonSpriteRec, wakeDistance), F(SkeletonSpriteRec, speed), F(SkeletonSpriteRec, wakeTimer), END
};
static const SyncField kZapper[] = {
    F(ZapperNodeSpriteRec, onTime), F(ZapperNodeSpriteRec, offTime), F(ZapperNodeSpriteRec, timer),
    F(ZapperNodeSpriteRec, frameCounter), F(ZapperNodeSpriteRec, isOn), F(ZapperNodeSpriteRec, isDisabled),
    F(ZapperNodeSpriteRec, beamLength), F(ZapperNodeSpriteRec, distance), END
};

typedef struct SyncKind {
    long             first, last;   ///< sprite types
    size_t           recordSize;
    const SyncField *fields;
} SyncKind;

static const SyncKind kKinds[] = {
    { kFirstMovingPlatformSprite, kLastFallingPlatformSprite, sizeof(PlatformSpriteRec), kPlatform },
    { kFirstTrapDoorSprite, kLastTrapDoorSprite, sizeof(PlatformSpriteRec), kPlatform },
    { kSkullSpiderSprite, kSkullSpiderSprite, sizeof(SkullSpiderSpriteRec), kSkullSpider },
    { kScorpionSprite, kScorpionSprite, sizeof(CrawlerSpriteRec), kCrawler },
    { kSnakeSprite1, kSnakeSprite2, sizeof(CrawlerSpriteRec), kCrawler },
    { kEyeballSprite, kEyeballSprite, sizeof(EyeballSpriteRec), kEyeball },
    { kBlueRavenSprite, kRedRavenSprite, sizeof(RavenSpriteRec), kRaven },
    { kMedusaHeadSprite1, kMedusaHeadSprite2, sizeof(MedusaSpriteRec), kMedusa },
    { kSkeletonFishSprite, kSkeletonFishSprite, sizeof(FishSpriteRec), kFish },
    { kWalkingSkeletonSprite, kWalkingSkeletonSprite, sizeof(SkeletonSpriteRec), kSkeleton },
    { kFirstZapperNodeSprite, kLastZapperNodeSprite, sizeof(ZapperNodeSpriteRec), kZapper },
};

static const SyncKind *KindOf(SpritePtr spriteP)
{
    size_t i;

    if ((size_t)spriteP->spriteRecSize < sizeof(GameSpriteRec) || GAME_SPRITE(spriteP)->objectID <= 0)
        return NULL;            // only what the level placed (the others' copies have the same)
    for (i = 0; i < SDL_arraysize(kKinds); i++)
        if (spriteP->userData >= kKinds[i].first && spriteP->userData <= kKinds[i].last)
            return (size_t)spriteP->spriteRecSize >= kKinds[i].recordSize ? &kKinds[i] : NULL;
    return NULL;
}

/// The kind's fields, little-endian, into out (returns the length).
static int WriteFields(SpritePtr spriteP, const SyncField *f, Uint8 *out, int room)
{
    const unsigned char *rec = (const unsigned char *)spriteP;
    int n = 0, i;

    for (; f->size != 0; f++)
        for (i = 0; i < f->count; i++) {
            const unsigned char *at = rec + f->offset + (size_t)i * f->size;
            if (n + f->size > room)
                return n;
            switch (f->size) {
                case 1: out[n] = *at; break;
                case 2: { Uint16 v; memcpy(&v, at, 2); NetPutU16(out + n, v); break; }
                case 4: { Uint32 v; memcpy(&v, at, 4); NetPutU32(out + n, v); break; }
                default: { double v; memcpy(&v, at, 8); NetPutF64(out + n, v); break; }
            }
            n += f->size;
        }
    return n;
}

static int FieldsLength(const SyncField *f)
{
    int n = 0;
    for (; f->size != 0; f++)
        n += f->size * f->count;
    return n;
}

static void ReadFields(SpritePtr spriteP, const SyncField *f, const Uint8 *in)
{
    unsigned char *rec = (unsigned char *)spriteP;
    int n = 0, i;

    for (; f->size != 0; f++)
        for (i = 0; i < f->count; i++) {
            unsigned char *at = rec + f->offset + (size_t)i * f->size;
            switch (f->size) {
                case 1: *at = in[n]; break;
                case 2: { Uint16 v = NetGetU16(in + n); memcpy(at, &v, 2); break; }
                case 4: { Uint32 v = NetGetU32(in + n); memcpy(at, &v, 4); break; }
                default: { double v = NetGetF64(in + n); memcpy(at, &v, 8); break; }
            }
            n += f->size;
        }
}

// ---------------------------------------------------------------------------
// Snapshots

#define kMaxSyncSprites 96
#define kMaxSyncBytes   128

typedef struct SyncSprite {
    Sint32  objectID;
    Sint32  type;
    float   x, y;
    Sint32  frame;
    Sint32  frameAdvance, frameTime;
    float   scaleW, scaleH;
    Uint32  translucency;
    Uint16  moveProc, frameProc;
    Uint8   visible, flipped, numBytes, unused;
    Uint8   bytes[kMaxSyncBytes];   ///< the kind's fields (WriteFields)
} SyncSprite;

typedef struct RoomSnapshot {
    short       room;           ///< level number, -1 none
    Uint32      frame;          ///< when it was taken (or arrived)
    Uint32      serial;         ///< a new one each time
    int         count;
    SyncSprite  sprites[kMaxSyncSprites];
} RoomSnapshot;

typedef struct RoomSyncState {
    Uint32      frame;
    struct {
        short   room;           ///< the room this player's game is running, -1 none
        Uint32  since;          ///< frame they came in
    } players[kMaxPlayers];
    RoomSnapshot snapshots[kMaxPlayers];    ///< each keeper's room
    Uint32      serial;
} RoomSyncState;

static RoomSyncState gSync;     // shared by every player's game (RoomSyncShareState)
static Uint32 gLastApplied[kMaxPlayers];   // per player's game: the snapshots already taken

void RoomSyncShareState(void (*share)(void *, size_t))
{
    share(&gSync, sizeof(gSync));
}

void RoomSyncReset(void)
{
    int i;

    memset(&gSync, 0, sizeof(gSync));
    memset(gLastApplied, 0, sizeof(gLastApplied));
    for (i = 0; i < kMaxPlayers; i++) {
        gSync.players[i].room = -1;
        gSync.snapshots[i].room = -1;
    }
}

/// Once a frame, before the players' games run.
void RoomSyncFrameBegin(void)
{
    gSync.frame++;
}

static Boolean SyncOn(void)
{
    static int off = -1;

    if (off < 0)
        off = getenv("MM_DEBUG_NO_ROOM_SYNC") != NULL;     // testing aid: each copy on its own
    return !off && SplitScreenActive() && SplitScreenMode() == kSplitModeCoop;
}

/// Whether this player's game is running its room now (not on the map, a sign, saving, ...).
static Boolean RunningRoom(void)
{
    return gCurrentGameMode == kPlayingGameMode && !gLayersArePausedForSaveAnim;
}

/// Who keeps a room: of the players running it, the one there longest (within a second of
/// each other, the first player: computers hear of each other a little late, and must agree).
static int KeeperOf(short room)
{
    int i, keeper = -1;

    for (i = 0; i < kMaxPlayers; i++) {
        if (gSync.players[i].room != room)
            continue;
        if (keeper < 0 || (Sint32)(gSync.players[i].since - gSync.players[keeper].since) < -60)
            keeper = i;
    }
    return keeper;
}

static void TakeSprite(SpritePtr spriteP, const SyncKind *kind, SyncSprite *out)
{
    memset(out, 0, sizeof(*out));
    out->objectID = GAME_SPRITE(spriteP)->objectID;
    out->type = (Sint32)spriteP->userData;
    out->x = spriteP->drawData->horizLoc;
    out->y = spriteP->drawData->vertLoc;
    out->frame = (Sint32)spriteP->curFrameIndex;
    out->frameAdvance = (Sint32)spriteP->frameAdvance;
    out->frameTime = (Sint32)spriteP->frameTimeInterval;
    out->scaleW = spriteP->drawData->scaledWidthFactor;
    out->scaleH = spriteP->drawData->scaledHeightFactor;
    out->translucency = (Uint32)spriteP->drawData->translucencyLevel;
    out->moveProc = MoveProcNumber(spriteP->realSpriteMoveProc);
    out->frameProc = FrameProcNumber(spriteP->frameChangeProc);
    out->visible = spriteP->isVisible ? 1 : 0;
    out->flipped = spriteP->drawData->flippedHorizontal ? 1 : 0;
    out->numBytes = (Uint8)WriteFields(spriteP, kind->fields, out->bytes, kMaxSyncBytes);
}

static Boolean IsHeroLayer(SpriteLayerPtr layerP)
{
    return layerP == gHeroSpriteLayerP || layerP == gHeroOnPlatformSpriteLayerP || layerP == gDeadHeroSpriteLayerP;
}

static void TakeSnapshot(RoomSnapshot *snap)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP;
    const SyncKind *kind;

    snap->room = gCurrentLevel;
    snap->frame = gSync.frame;
    snap->serial = ++gSync.serial;
    snap->count = 0;
    for (layerP = SWGetNextSpriteLayer(gSpriteWorldP, NULL); layerP != NULL;
         layerP = SWGetNextSpriteLayer(gSpriteWorldP, layerP)) {
        if (IsHeroLayer(layerP))
            continue;
        for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL && snap->count < kMaxSyncSprites;
             spriteP = SWGetNextSprite(layerP, spriteP)) {
            if (spriteP->spriteRemoval != kDontRemoveSprite || (kind = KindOf(spriteP)) == NULL)
                continue;
            TakeSprite(spriteP, kind, &snap->sprites[snap->count++]);
        }
    }
}

static void ApplySprite(SpritePtr spriteP, const SyncKind *kind, const SyncSprite *in, SpritePtr heroP)
{
    float dx = in->x - spriteP->drawData->horizLoc, dy = in->y - spriteP->drawData->vertLoc;

    if (dx != 0 || dy != 0) {
        SWSetSpriteLocation(spriteP, in->x, in->y);
            // the player standing on it goes with it
        if (heroP != NULL && HERO(heroP)->platformP == spriteP && fabsf(dx) < 16 && fabsf(dy) < 16)
            SWSetSpriteLocation(heroP, heroP->drawData->horizLoc + dx, heroP->drawData->vertLoc + dy);
    }
    if (spriteP->curFrameIndex != in->frame && in->frame >= 0 && in->frame < spriteP->maxFrames)
        SWSetCurrentFrameIndex(spriteP, in->frame);
    spriteP->frameAdvance = in->frameAdvance;
    if (spriteP->frameTimeInterval != in->frameTime)
        SWSetSpriteFrameTime(spriteP, in->frameTime);
    if (spriteP->drawData->scaledWidthFactor != in->scaleW || spriteP->drawData->scaledHeightFactor != in->scaleH)
        SWSetSpriteScaledFactor(spriteP, in->scaleW, in->scaleH);
    if (spriteP->drawData->translucencyLevel != in->translucency)
        SWSetSpriteTranslucencyLevel(spriteP, in->translucency);
    if (in->moveProc < SDL_arraysize(kMoveProcs) && spriteP->realSpriteMoveProc != kMoveProcs[in->moveProc] &&
        MoveProcNumber(spriteP->realSpriteMoveProc) != kUnknownProc)
        SWSetSpriteMoveProc(spriteP, kMoveProcs[in->moveProc]);
    if (in->frameProc < SDL_arraysize(kFrameProcs) && spriteP->frameChangeProc != kFrameProcs[in->frameProc] &&
        FrameProcNumber(spriteP->frameChangeProc) != kUnknownProc)
        SWSetSpriteFrameProc(spriteP, kFrameProcs[in->frameProc]);
    if ((spriteP->isVisible != 0) != (in->visible != 0))
        SWSetSpriteVisible(spriteP, in->visible != 0);
    if ((spriteP->drawData->flippedHorizontal != 0) != (in->flipped != 0))
        SWSetSpriteFlipX(spriteP, in->flipped != 0);
    if (in->numBytes == FieldsLength(kind->fields))
        ReadFields(spriteP, kind->fields, in->bytes);
}

static void ApplySnapshot(const RoomSnapshot *snap)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP, heroP = GetHeroSprite();
    const SyncKind *kind;
    int i, start = 0;

    if (snap->count == 0)
        return;
    for (layerP = SWGetNextSpriteLayer(gSpriteWorldP, NULL); layerP != NULL;
         layerP = SWGetNextSpriteLayer(gSpriteWorldP, layerP)) {
        if (IsHeroLayer(layerP))
            continue;
        for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP)) {
            if (spriteP->spriteRemoval != kDontRemoveSprite || (kind = KindOf(spriteP)) == NULL)
                continue;
                // the same sprites in the same order, as a rule: look from where the last one was
            for (i = 0; i < snap->count; i++) {
                const SyncSprite *in = &snap->sprites[(start + i) % snap->count];
                if (in->objectID == GAME_SPRITE(spriteP)->objectID && in->type == spriteP->userData) {
                    ApplySprite(spriteP, kind, in, heroP);
                    start = (start + i + 1) % snap->count;
                    break;
                }
            }
        }
    }
}

/// From the player's game, before its frame: the keeper's room, if someone else keeps it.
void RoomSyncBeforeFrame(void)
{
    int me = SplitCurrentPlayer(), keeper;
    const RoomSnapshot *snap;

    if (!SyncOn() || !RunningRoom())
        return;
    keeper = KeeperOf(gCurrentLevel);
    if (keeper < 0 || keeper == me)
        return;
    snap = &gSync.snapshots[keeper];
    if (snap->room != gCurrentLevel || gSync.frame - snap->frame > kMaxAge || snap->serial == gLastApplied[keeper])
        return;
    gLastApplied[keeper] = snap->serial;        // once: in between, this game runs the room itself
    ApplySnapshot(snap);
}

/// From the player's game, after its frame: which room it's running, and the snapshot if it
/// keeps that room.
void RoomSyncAfterFrame(void)
{
    int me = SplitCurrentPlayer();
    short room = RunningRoom() ? gCurrentLevel : -1;

    if (!SyncOn() || me < 0 || me >= kMaxPlayers)
        return;
    if (gSync.players[me].room != room) {
        gSync.players[me].room = room;
        gSync.players[me].since = gSync.frame;
    }
    if (room >= 0 && KeeperOf(room) == me)
        TakeSnapshot(&gSync.snapshots[me]);
    else
        gSync.snapshots[me].room = -1;
}

// ---------------------------------------------------------------------------
// Several computers

/// Arcade Split Screen on several computers: the room a player elsewhere is running (from
/// their kMsgPhantom), and for how long.
void RoomSyncSetRemotePlayer(int player, short room, Uint32 roomMs)
{
    Uint32 frames = (Uint32)(roomMs / (1000.0 / 60.0));

    if (player < 0 || player >= kMaxPlayers)
        return;
    gSync.players[player].room = room;
    gSync.players[player].since = gSync.frame - (frames < gSync.frame ? frames : gSync.frame);
    if (room < 0)
        gSync.snapshots[player].room = -1;
}

/// How long this computer's player has been running their room (for the others' keeper choice).
Uint32 RoomSyncLocalRoomMs(int player)
{
    if (player < 0 || player >= kMaxPlayers || gSync.players[player].room < 0)
        return 0;
    return (Uint32)((gSync.frame - gSync.players[player].since) * (1000.0 / 60.0));
}

short RoomSyncLocalRoom(int player)
{
    return player >= 0 && player < kMaxPlayers ? gSync.players[player].room : -1;
}

/// Once a frame on several computers: the room this computer's player keeps, to the others in
/// it, now and then.
void RoomSyncNetFrame(void)
{
    static Uint8 data[2 + 2 + kMaxSyncSprites * (48 + kMaxSyncBytes)];
    const RoomSnapshot *snap;
    int me = NetLocalPlayer(), i, n, others = 0;

    if (!SyncOn() || !NetArcadeActive() || me < 0 || me >= kMaxPlayers || gSync.frame % kSendEvery != 0)
        return;
    snap = &gSync.snapshots[me];
    if (snap->room < 0 || KeeperOf(snap->room) != me)
        return;
    for (i = 0; i < kMaxPlayers; i++)
        if (i != me && gSync.players[i].room == snap->room)
            others++;
    if (others == 0)
        return;
    NetPutU16(data, (Uint16)snap->room);
    NetPutU16(data + 2, (Uint16)snap->count);
    for (i = 0, n = 4; i < snap->count; i++) {
        const SyncSprite *sp = &snap->sprites[i];
        NetPutU32(data + n, (Uint32)sp->objectID);
        NetPutU16(data + n + 4, (Uint16)sp->type);
        NetPutF32(data + n + 6, sp->x);
        NetPutF32(data + n + 10, sp->y);
        NetPutU32(data + n + 14, (Uint32)sp->frame);
        NetPutU32(data + n + 18, (Uint32)sp->frameAdvance);
        NetPutU32(data + n + 22, (Uint32)sp->frameTime);
        NetPutF32(data + n + 26, sp->scaleW);
        NetPutF32(data + n + 30, sp->scaleH);
        NetPutU32(data + n + 34, sp->translucency);
        NetPutU16(data + n + 38, sp->moveProc);
        NetPutU16(data + n + 40, sp->frameProc);
        data[n + 42] = (Uint8)(sp->visible | sp->flipped << 1);
        data[n + 43] = sp->numBytes;
        memcpy(data + n + 44, sp->bytes, sp->numBytes);
        n += 44 + sp->numBytes;
    }
    NetSendRoomSnapshot(data, n);
}

/// A snapshot from another computer's keeper.
void RoomSyncReceive(int from, const Uint8 *data, int length)
{
    RoomSnapshot *snap;
    int count, i, n;

    if (from < 0 || from >= kMaxPlayers || length < 4)
        return;
    snap = &gSync.snapshots[from];
    count = NetGetU16(data + 2);
    if (count > kMaxSyncSprites)
        count = kMaxSyncSprites;
    for (i = 0, n = 4; i < count && n + 44 <= length; i++) {
        SyncSprite *sp = &snap->sprites[i];
        sp->objectID = (Sint32)NetGetU32(data + n);
        sp->type = (Sint16)NetGetU16(data + n + 4);
        sp->x = NetGetF32(data + n + 6);
        sp->y = NetGetF32(data + n + 10);
        sp->frame = (Sint32)NetGetU32(data + n + 14);
        sp->frameAdvance = (Sint32)NetGetU32(data + n + 18);
        sp->frameTime = (Sint32)NetGetU32(data + n + 22);
        sp->scaleW = NetGetF32(data + n + 26);
        sp->scaleH = NetGetF32(data + n + 30);
        sp->translucency = NetGetU32(data + n + 34);
        sp->moveProc = NetGetU16(data + n + 38);
        sp->frameProc = NetGetU16(data + n + 40);
        sp->visible = data[n + 42] & 1;
        sp->flipped = (data[n + 42] >> 1) & 1;
        sp->numBytes = data[n + 43] > kMaxSyncBytes ? kMaxSyncBytes : data[n + 43];
        if (n + 44 + sp->numBytes > length)
            break;
        memcpy(sp->bytes, data + n + 44, sp->numBytes);
        n += 44 + data[n + 43];
    }
    snap->count = i;
    snap->room = (short)NetGetU16(data);
    snap->frame = gSync.frame;
    snap->serial = ++gSync.serial;
}
