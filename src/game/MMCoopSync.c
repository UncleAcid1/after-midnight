/*
 * MMCoopSync.c — Arcade Split Screen (co-op): what one player does to the mansion happens in
 * everyone's game, added by this version.
 *
 * Each player runs their own copy of the game (MMSplit.c) with their own copy of the room
 * they are in. The mansion's progress (what's collected, which levers are pulled, which doors
 * are open) is shared, so a room always loads the way the team left it; but two players in
 * the same room each have the room loaded already. So when a player pulls a lever, presses a
 * switch, unlocks a door, kills an enemy or steps on a falling block, an event goes into a
 * queue every game shares, and the others standing in that room do the same thing in theirs.
 * Gold and silver floors are switched for the whole mansion, so those reach everyone.
 *
 * Applying an event is idempotent (a lever already in that position, a door already open,
 * an enemy already gone are left alone), so a room loaded just after the event is fine.
 */

#include "MMGame.h"
#include "MMProtos.h"

#define kQueueSize  128

enum { kCoopLever = 1, kCoopSwitch, kCoopDoor, kCoopKill, kCoopPlatform };
enum { kSwitchSilverFloors = 11, kSwitchGoldFloors = 12 };     // brick switch actions (Levers.c)

typedef struct CoopEvent {
    unsigned long   seq;
    int             kind;
    int             player;
    short           room;
    long            objectID;
    long            type;           ///< the sprite's userData
    float           x, y;           ///< where it was, to find it when it has no object ID
    int             param;          ///< lever: the frame it ended on
    short           action;         ///< lever / switch action
} CoopEvent;

typedef struct CoopQueue {
    unsigned long   nextSeq;
    CoopEvent       events[kQueueSize];
} CoopQueue;

static CoopQueue gCoopQueue;            // shared by every player's game (see CoopSyncShareState)
static unsigned long gCoopSeenSeq = 0;  // per player's game: events up to here are done
static Boolean gCoopApplying = false;   // per player's game: replaying someone else's event

void CoopSyncShareState(void (*share)(void *, size_t))
{
    share(&gCoopQueue, sizeof(gCoopQueue));
}

/// A new split screen co-op game: nothing has happened yet.
void CoopSyncReset(void)
{
    memset(&gCoopQueue, 0, sizeof(gCoopQueue));
    gCoopQueue.nextSeq = 1;
    gCoopSeenSeq = 0;
    gCoopApplying = false;
}

static Boolean CoopSyncOn(void)
{
    return SplitScreenActive() && SplitScreenMode() == kSplitModeCoop && !gCoopApplying;
}

static void Post(int kind, SpritePtr spriteP, int param, short action)
{
    CoopEvent *e;

    if (!CoopSyncOn() || spriteP == NULL)
        return;
    e = &gCoopQueue.events[gCoopQueue.nextSeq % kQueueSize];
    memset(e, 0, sizeof(*e));
    e->seq = gCoopQueue.nextSeq++;
    e->kind = kind;
    e->player = SplitCurrentPlayer();
    e->room = gCurrentLevel;
    e->objectID = GAME_SPRITE(spriteP)->objectID;
    e->type = spriteP->userData;
    e->x = SWGetSpriteHorizLoc(spriteP);
    e->y = SWGetSpriteVertLoc(spriteP);
    e->param = param;
    e->action = action;
    gCoopSeenSeq = e->seq;              // nothing to replay for the one who did it
}

void CoopPostLeverPulled(SpritePtr leverP)
{
    Post(kCoopLever, leverP, (int)leverP->curFrameIndex, LEVER(leverP)->action);
}

void CoopPostSwitchPressed(SpritePtr switchP)
{
    Post(kCoopSwitch, switchP, 0, LEVER(switchP)->action);
}

void CoopPostDoorUnlocked(SpritePtr doorP)
{
    Post(kCoopDoor, doorP, 0, 0);
}

void CoopPostEnemyKilled(SpritePtr enemyP)
{
    Post(kCoopKill, enemyP, 0, 0);
}

void CoopPostPlatformFalling(SpritePtr platformP)
{
    Post(kCoopPlatform, platformP, 0, 0);
}

// ---------------------------------------------------------------------------
// Doing it here too

/// The sprite in this room the event is about: by object ID, or by type and place.
static SpritePtr FindEventSprite(const CoopEvent *e)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP;

    for (layerP = SWGetNextSpriteLayer(gSpriteWorldP, NULL); layerP != NULL;
         layerP = SWGetNextSpriteLayer(gSpriteWorldP, layerP)) {
        if (layerP == gHeroSpriteLayerP || layerP == gDeadHeroSpriteLayerP)
            continue;
        for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP)) {
            if (spriteP->userData != e->type || spriteP->spriteRemoval != kDontRemoveSprite)
                continue;
            if (e->objectID != -1 && e->objectID != 0) {
                if (GAME_SPRITE(spriteP)->objectID == e->objectID)
                    return spriteP;
            } else if (fabsf(SWGetSpriteHorizLoc(spriteP) - e->x) < 3 && fabsf(SWGetSpriteVertLoc(spriteP) - e->y) < 3) {
                return spriteP;
            }
        }
    }
    return NULL;
}

static Boolean IsGoldSilverAction(const CoopEvent *e)
{
    if (e->kind == kCoopLever)
        return e->action == kLeverToggleGoldSilver;
    if (e->kind == kCoopSwitch)
        return e->action == kSwitchSilverFloors || e->action == kSwitchGoldFloors;
    return false;
}

static void ApplyLever(const CoopEvent *e, SpritePtr leverP)
{
    if (leverP->curFrameIndex == e->param)
        return;                         // already that way (the room loaded after it)
    SWSetCurrentFrameIndex(leverP, e->param);
    if (LEVER(leverP)->isOneShot)
        LEVER(leverP)->isActive = false;
    MMPlayStereoSound(leverP, 0x1b, 12, 1);
    PerformLeverAction(leverP, false);
}

static void ApplySwitch(SpritePtr switchP)
{
    if (!LEVER(switchP)->isActive)
        return;
    SWSetCurrentFrameIndex(switchP, 1);
    PushBrickSwitch(switchP);
}

static void ApplyDoor(SpritePtr doorP)
{
    if (DOOR(doorP)->isOpen || SWGetSpriteMoveProc(doorP) != NULL)
        return;
    MakeDoorOpen(doorP, false);
    MMPlayStereoSound(doorP, 1, 8, 1);
}

static void ApplyKill(SpritePtr enemyP)
{
    if (enemyP->userData == kSmallSpiderSprite)
        DeleteWebForSmallSpider(enemyP);
    SWCenterSpriteOverSprite(NewPuffOfSmokeSprite(), enemyP);
    SWRemoveSpriteFromAnimation(enemyP, true);
}

static void ApplyPlatform(SpritePtr platformP)
{
    long type = platformP->userData;

    if (SWGetSpriteMoveProc(platformP) != NULL || type < kFirstFallingPlatformSprite ||
        type > kLastFallingPlatformSprite)
        return;
    PLATFORM(platformP)->fallDelay = 30;
    SWSetSpriteMoveProc(platformP, FallingPlatformMoveProc);
    if (type >= 0x5c && type <= 0x5e) {          // skull platforms crumble
        SWSetSpriteFrameProc(platformP, SkullFallingPlatformFrameProc);
        SWSetSpriteFrameAdvance(platformP, 1);
        SWSetSpriteFrameTime(platformP, 100);
    }
}

static void Apply(const CoopEvent *e)
{
    SpritePtr spriteP = e->room == gCurrentLevel ? FindEventSprite(e) : NULL;

    if (spriteP == NULL) {
            // gold and silver floors are the whole mansion's: switch them here too
        if (IsGoldSilverAction(e))
            DoLeverToggleGoldSilverBlocks(true,
                                          e->kind == kCoopLever ? 0 : e->action == kSwitchSilverFloors ? 1 : 2);
        return;
    }
    switch (e->kind) {
        case kCoopLever:    ApplyLever(e, spriteP); break;
        case kCoopSwitch:   ApplySwitch(spriteP); break;
        case kCoopDoor:     ApplyDoor(spriteP); break;
        case kCoopKill:     ApplyKill(spriteP); break;
        case kCoopPlatform: ApplyPlatform(spriteP); break;
        default:            break;
    }
}

/// Once a frame in each player's game: does what the others did since last time.
void CoopSyncApply(void)
{
    unsigned long seq;

    if (!CoopSyncOn())
        return;
    if (gCurrentGameMode != kPlayingGameMode)
        return;                         // the map or a sign is up: when it's closed
    if (gCoopQueue.nextSeq > kQueueSize && gCoopSeenSeq < gCoopQueue.nextSeq - kQueueSize)
        gCoopSeenSeq = gCoopQueue.nextSeq - kQueueSize;     // fell far behind: what's left
    gCoopApplying = true;
    for (seq = gCoopSeenSeq + 1; seq < gCoopQueue.nextSeq; seq++) {
        const CoopEvent *e = &gCoopQueue.events[seq % kQueueSize];
        if (e->seq == seq && e->player != SplitCurrentPlayer())
            Apply(e);
    }
    gCoopApplying = false;
    gCoopSeenSeq = gCoopQueue.nextSeq - 1;
}
