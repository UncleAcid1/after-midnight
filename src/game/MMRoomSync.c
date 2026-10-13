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
 * The snapshot is plain data (procs by number, MMProcTable.h), so another computer can use it.
 */

#include <stddef.h>
#include "MMGame.h"
#include "MMProtos.h"
#include "MMProcTable.h"

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
// What is kept for each kind of sprite: its own fields after the object ID, without the ones
// that point at other sprites (each copy has its own)

typedef struct SyncKind {
    long    first, last;    ///< sprite types
    size_t  from, to;       ///< the bytes of the record copied
} SyncKind;

#define FIELDS(type, firstField) offsetof(type, firstField), sizeof(type)

static const SyncKind kKinds[] = {
    { kFirstMovingPlatformSprite, kLastFallingPlatformSprite, FIELDS(PlatformSpriteRec, collisionInset) },
    { kFirstTrapDoorSprite, kLastTrapDoorSprite, FIELDS(PlatformSpriteRec, collisionInset) },
    { kSkullSpiderSprite, kSkullSpiderSprite, FIELDS(SkullSpiderSpriteRec, collisionInset) },
    { kScorpionSprite, kScorpionSprite, FIELDS(CrawlerSpriteRec, frameTimer) },
    { kSnakeSprite1, kSnakeSprite2, FIELDS(CrawlerSpriteRec, frameTimer) },
    { kEyeballSprite, kEyeballSprite, FIELDS(EyeballSpriteRec, collisionInset) },
    { kBlueRavenSprite, kRedRavenSprite, FIELDS(RavenSpriteRec, collisionInset) },
    { kMedusaHeadSprite1, kMedusaHeadSprite2, FIELDS(MedusaSpriteRec, frameDelay) },
    { kSkeletonFishSprite, kSkeletonFishSprite, FIELDS(FishSpriteRec, restVertLoc) },
    { kWalkingSkeletonSprite, kWalkingSkeletonSprite, FIELDS(SkeletonSpriteRec, frameDelay) },
    { kFirstZapperNodeSprite, kLastZapperNodeSprite, offsetof(ZapperNodeSpriteRec, onTime),
      offsetof(ZapperNodeSpriteRec, partnerP) },
};

static const SyncKind *KindOf(SpritePtr spriteP)
{
    size_t i;

    if ((size_t)spriteP->spriteRecSize < sizeof(GameSpriteRec) || GAME_SPRITE(spriteP)->objectID <= 0)
        return NULL;            // only what the level placed (the others' copies have the same)
    for (i = 0; i < SDL_arraysize(kKinds); i++)
        if (spriteP->userData >= kKinds[i].first && spriteP->userData <= kKinds[i].last)
            return &kKinds[i];
    return NULL;
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
    Uint8   bytes[kMaxSyncBytes];
} SyncSprite;

typedef struct RoomSnapshot {
    short       room;           ///< level number, -1 none
    Uint32      frame;          ///< when it was taken
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
} RoomSyncState;

static RoomSyncState gSync;     // shared by every player's game (RoomSyncShareState)

void RoomSyncShareState(void (*share)(void *, size_t))
{
    share(&gSync, sizeof(gSync));
}

void RoomSyncReset(void)
{
    int i;

    memset(&gSync, 0, sizeof(gSync));
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

/// Who keeps a room: of the players running it, the one there longest.
static int KeeperOf(short room)
{
    int i, keeper = -1;

    for (i = 0; i < kMaxPlayers; i++) {
        if (gSync.players[i].room != room)
            continue;
        if (keeper < 0 || gSync.players[i].since < gSync.players[keeper].since)
            keeper = i;
    }
    return keeper;
}

static void TakeSprite(SpritePtr spriteP, const SyncKind *kind, SyncSprite *out)
{
    size_t to = kind->to;

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
    if ((size_t)spriteP->spriteRecSize < to)
        to = (size_t)spriteP->spriteRecSize;
    if (to > kind->from && to - kind->from <= kMaxSyncBytes) {
        out->numBytes = (Uint8)(to - kind->from);
        memcpy(out->bytes, (const unsigned char *)spriteP + kind->from, out->numBytes);
    }
}

static void TakeSnapshot(RoomSnapshot *snap)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP;
    const SyncKind *kind;

    snap->room = gCurrentLevel;
    snap->frame = gSync.frame;
    snap->count = 0;
    for (layerP = SWGetNextSpriteLayer(gSpriteWorldP, NULL); layerP != NULL;
         layerP = SWGetNextSpriteLayer(gSpriteWorldP, layerP)) {
        if (layerP == gHeroSpriteLayerP || layerP == gHeroOnPlatformSpriteLayerP || layerP == gDeadHeroSpriteLayerP)
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
    size_t to = kind->to;

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
    if ((size_t)spriteP->spriteRecSize < to)
        to = (size_t)spriteP->spriteRecSize;
    if (to > kind->from && to - kind->from == in->numBytes)
        memcpy((unsigned char *)spriteP + kind->from, in->bytes, in->numBytes);
}

static void ApplySnapshot(const RoomSnapshot *snap)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP, heroP = GetHeroSprite();
    const SyncKind *kind;
    int i, start = 0;

    for (layerP = SWGetNextSpriteLayer(gSpriteWorldP, NULL); layerP != NULL;
         layerP = SWGetNextSpriteLayer(gSpriteWorldP, layerP)) {
        if (layerP == gHeroSpriteLayerP || layerP == gHeroOnPlatformSpriteLayerP || layerP == gDeadHeroSpriteLayerP)
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
    if (snap->room != gCurrentLevel || gSync.frame - snap->frame > 2)
        return;
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
