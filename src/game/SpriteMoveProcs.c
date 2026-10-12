/*
 * SpriteMoveProcs.c — the per-frame movement of the hero, enemies, platforms
 * and other animated sprites.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources & Headers/SpriteMoveProcs.c").
 */
#include "MMGame.h"

#define kWalkSpeed      (2.5f * ExtrasWalkSpeedFactor())     // "Jack on Espresso"
#define kClimbSpeed     2.2f

    // tile ranges
#define kFirstLadderTile    0x20b
#define kLastLadderTile     0x240
#define kFirstVineTile      0x1f9
#define kLastVineTile       0x20a
#define kFirstPoleTile      0x1e8
#define kLastPoleTile       0x1f7
#define kFirstLavaTile      0x309
#define kLastLavaTile       0x330
#define kSmallSpiderWebTile 0x5a0
#define kBigSpiderWebTile   0x5a1

static const short ladderOffsetArray[6] = { 0, 14, 8, 7, 8, 0 };

static short FloorDiv16(int n)
{
    return (short)((n < 0 ? n + 15 : n) >> 4);
}

/// Bounces the sprite off the left/right edges of the room (as seen through its
/// frame's collision inset).
static void BounceOffRoomSides(SpritePtr spriteP, SWRect *boundsP)
{
    DrawDataPtr d = spriteP->drawData;

    if (spriteP->destFrameRect.left + spriteP->curFrameP->collisionInset.left < boundsP->left && d->horizMoveDelta < 0)
        d->horizMoveDelta = -d->horizMoveDelta;
    else if (boundsP->right < spriteP->destFrameRect.right - spriteP->curFrameP->collisionInset.right &&
             d->horizMoveDelta > 0)
        d->horizMoveDelta = -d->horizMoveDelta;
}

/// Gravity for walking enemies: fast at first, then slower, up to maxSpeed.
static void ApplyEnemyGravity(SpritePtr spriteP, float slowAccel, float fastAccel, float maxSpeed)
{
    DrawDataPtr d = spriteP->drawData;

    d->vertMoveDelta += d->vertMoveDelta <= 1.0f ? fastAccel : slowAccel;
    if (d->vertMoveDelta > maxSpeed)
        d->vertMoveDelta = maxSpeed;
}

/// When a falling sprite crosses into a new tile row, a conveyor belt under it
/// adds the belt's speed.
static float ConveyorPush(SpritePtr spriteP, SWRect *insetP)
{
    float push = 0;

    if (FloorDiv16(spriteP->oldFrameRect.bottom - insetP->bottom - 1) <
        FloorDiv16(spriteP->destFrameRect.bottom - insetP->bottom - 1)) {
        if (SWCheckSpriteWithTiles(gSpriteWorldP, spriteP, kSW_CheckBottom, insetP, 4, 4, 0x241, 0x243, false))
            push -= gCurConveyorBeltDirection;
        if (SWCheckSpriteWithTiles(gSpriteWorldP, spriteP, kSW_CheckBottom, insetP, 4, 4, 0x249, 0x24b, false))
            push += gCurConveyorBeltDirection;
    }
    return push;
}

/// Turns the sprite around when it walks into a wall.
static void TurnAtWalls(SpritePtr spriteP, SWRect *insetP, float push)
{
    DrawDataPtr d = spriteP->drawData;

    if (d->horizMoveDelta + push > 0) {
        if (CheckForWall(spriteP, kSW_CheckRight, insetP, false, false, true) && d->horizMoveDelta > 0)
            d->horizMoveDelta = -d->horizMoveDelta;
    }
    else if (d->horizMoveDelta + push < 0) {
        if (CheckForWall(spriteP, kSW_CheckLeft, insetP, false, false, true) && d->horizMoveDelta < 0)
            d->horizMoveDelta = -d->horizMoveDelta;
    }
}

/// Puts the hero back into walking mode after getting off a ladder.
static void StepOffLadder(SpritePtr spriteP, Boolean moveDownToFloor)
{
    HeroSpritePtr heroP = HERO(spriteP);

    if (!gCanUseLadderJump) {
        heroP->onGround = MoveSpriteDownToFloor(spriteP);
        SaveLastOnGroundPixelRow(spriteP);
        if (heroP->onGround)
            SetHeroGroundFrame(spriteP);
    }
    else {
        if (moveDownToFloor)
            MoveSpriteDownToFloor(spriteP);
        heroP->onGround = true;
        SetHeroGroundFrame(spriteP);
    }
        // don't grab the ladder again right away
    if (gKeys[kKeyUp]) {
        heroP->upKeyDelay = 13;
    }
    else if (gKeys[kKeyDown]) {
        heroP->downKeyDelay = 13;
        heroP->canDoPushups = false;
    }
}

// ---------------------------------------------------------------------------
// The hero

void MainSpriteMoveProc(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);
    DrawDataPtr d = spriteP->drawData;
    SpritePtr platformP;
    SWRect inset, ladderRect, centeredRect;
    short ladderWalkDir = 0, direction, sound, bottomNow, rise, tilesFallen;
    float platformH, platformV, jumpBase, platformVert;
    Boolean onGround, landedOnSamePlatform;

    if (!gKeys[kKeyAction] && heroP->actionKeyFrames < 5)
        heroP->actionKeyFrames++;

        // wake up from idling
    if (spriteP->frameChangeProc == IdleHeroFrameProc && heroP->idleState == 5 &&
        (gKeys[kKeyDown] || gKeys[kKeyLeft] || gKeys[kKeyRight] || gKeys[kKeyJumpLeft] ||
         gKeys[kKeyJumpRight] || gKeys[kKeyJump])) {
        DeleteZzzzSprites();
        MMPlaySound(0x47, 9, 1);
    }

    gHeroLandedThisFrame = false;
    if (heroP->isLanding && SWGetSpriteFrameTime(spriteP) < 0)
        heroP->isLanding = false;

    if (heroP->isDoingPushups && !gKeys[kKeyDown] && heroP->onGround) {
        heroP->isDoingPushups = false;
        heroP->collisionInset.top = heroP->normalInsetTop;
        SWSetSpriteFrameProc(spriteP, NULL);
        SWSetSpriteFrameTime(spriteP, -1);
        SWSetSpriteFrameAdvance(spriteP, 0);
        SetHeroGroundFrame(spriteP);
    }

        // ride the platform the hero is standing on
    heroP->platformHorizDelta = 0;
    heroP->unused154 = 0;
    if (heroP->platformP == NULL) {
        if (spriteP->parentSpriteLayerP != gHeroSpriteLayerP) {
            SWRemoveSprite(spriteP);
            SWAddSprite(gHeroSpriteLayerP, spriteP);
        }
    }
    else {
        platformP = heroP->platformP;
        if (d->vertMoveDelta >= 0 && spriteP->parentSpriteLayerP != gHeroOnPlatformSpriteLayerP) {
            SWRemoveSprite(spriteP);
            SWAddSprite(gHeroOnPlatformSpriteLayerP, spriteP);
        }
        if (!platformP->isVisible) {
            platformH = PLATFORM(platformP)->moveDeltaH;
            platformV = PLATFORM(platformP)->moveDeltaV;
        }
        else {
            platformH = platformP->destFrameRect.left - platformP->oldFrameRect.left;
            platformV = platformP->destFrameRect.top - platformP->oldFrameRect.top;
        }
        if (platformH != 0)
            heroP->platformHorizDelta = SWGetSpriteHorizMoveDelta(platformP);
        if (platformV < 0)
            platformV = 0;
        SWOffsetSprite(spriteP, platformH, platformV);
        if (platformH > 0)
            CheckForWall(spriteP, kSW_CheckRight, &heroP->wallInset, heroP->isJumping, false, true);
        else if (platformH < 0)
            CheckForWall(spriteP, kSW_CheckLeft, &heroP->wallInset, heroP->isJumping, false, true);
    }

    onGround = heroP->onGround;
    if (onGround)
        SaveLastOnGroundPixelRow(spriteP);
    SaveStartingPositionIfAppropriate(spriteP, onGround);

        // (the demo version ended the game here once the demo limit was reached)

        // pull levers and read signs
    if (gKeys[kKeyAction] && onGround && heroP->actionKeyFrames > 1 &&
        spriteP->destFrameRect.bottom == spriteP->oldFrameRect.bottom) {
        if (PullLeverIfPossible(spriteP))
            return;
        SWCollideSpriteWithLayer(spriteP, gSignSpriteLayerP, HeroSignCollideProc);
    }

    if (heroP->upKeyDelay > 0)
        heroP->upKeyDelay--;
    if (heroP->downKeyDelay > 0)
        heroP->downKeyDelay--;
    if (!gKeys[kKeyUp])
        heroP->upKeyDelay = 0;
    if (!gKeys[kKeyDown]) {
        heroP->downKeyDelay = 0;
        heroP->canDoPushups = true;
    }
    if (!gKeys[kKeyLeft] && !gKeys[kKeyRight])
        heroP->canDoPushups = true;

    if (onGround && (gKeys[kKeyUp] || gKeys[kKeyAction]))
        SWCollideSpriteWithLayer(spriteP, gExitSpriteLayerP, HeroExitCollideProc);

        // ladders
    if (onGround &&
        ((gKeys[kKeyUp] && !gKeys[kKeyDown] && heroP->upKeyDelay == 0) ||
         ((gKeys[kKeyDown] || gGamepad[kKeyDown]) && !gKeys[kKeyUp] && heroP->downKeyDelay == 0))) {
        inset = gKeys[kKeyUp] ? heroP->ladderUpInset : heroP->ladderDownInset;
        if (!GetLadderRect(gSpriteWorldP, spriteP, &inset, &ladderRect, 5, kFirstLadderTile, kLastLadderTile)) {
                // down with no ladder: push-ups
            if (gKeys[kKeyDown] && heroP->canDoPushups &&
                (spriteP->frameChangeProc == NULL || spriteP->frameChangeProc == HeroLandedFrameProc ||
                 spriteP->frameChangeProc == HeroMiscFrameProc)) {
                SWSetSpriteFrameRange(spriteP, 15, 15);
                SWSetSpriteFlipX(spriteP, !heroP->facingRight);
                SWSetSpriteFrameProc(spriteP, PushupDelayFrameProc);
                SWSetSpriteFrameTime(spriteP, 16);
                heroP->pushupDelay = GetRandom(180, 420);
                heroP->isDoingPushups = true;
                heroP->isLanding = false;
                heroP->collisionInset.top = heroP->normalInsetTop + 16;
                SWSetSpriteMoveDelta(spriteP, 0, 0);
            }
        }
        else {
            centeredRect = spriteP->destFrameRect;
            SWCenterRect(&centeredRect, &ladderRect);
            if (spriteP->destFrameRect.left + 2 < centeredRect.left) {
                if (gKeys[kKeyRight] - gKeys[kKeyLeft] >= 0)
                    ladderWalkDir = 1;
            }
            else if (spriteP->destFrameRect.left - 2 <= centeredRect.left) {
                    // lined up with the ladder: get on it
                heroP->platformP = NULL;
                heroP->jumpHorizDelta = 0;
                heroP->jumpVertDelta = 0;
                heroP->platformHorizDelta = 0;
                heroP->unused154 = 0;
                heroP->conveyorDelta = 0;
                StopChannel(4);
                if (gKeys[kKeyUp]) {
                    SWSetSpriteMoveProc(spriteP, ClimbingSpriteMoveProc);
                    SWSetSpriteFrameRange(spriteP, 18, 25);
                    SWSetSpriteFlipX(spriteP, false);
                    SWSetSpriteFrameAdvanceMode(spriteP, kNoLoopingMode);
                    SWSetSpriteFrameAdvance(spriteP, 1);
                    SWSetSpriteFrameProc(spriteP, NULL);
                    heroP->onGround = false;
                    heroP->hasClimbed = false;
                    heroP->isLanding = false;
                    ClimbingSpriteMoveProc(spriteP);
                    return;
                }
                if (!gGamepad[kKeyDown])
                    heroP->ladderDirection = gKeys[kKeyDown] - gKeys[kKeyUp];
                else
                    heroP->ladderDirection = gGamepad[kKeyDown];
                heroP->onGround = false;
                heroP->isLanding = false;
                heroP->ladderStepTimer = 4;
                SWSetSpriteMoveProc(spriteP, FinishClimbingLadderMoveProc);
                SWSetSpriteFrameAdvance(spriteP, 1);
                SWSetSpriteFrameProc(spriteP, NULL);
                FinishClimbingLadderMoveProc(spriteP);
                return;
            }
            else if (gKeys[kKeyRight] - gKeys[kKeyLeft] <= 0) {
                ladderWalkDir = -1;
            }
        }
    }

        // vines and poles
    if (!onGround || gKeys[kKeyDown]) {
        if (GetOnVineIfAppropriate(spriteP, onGround, 4, true) || GetOnVineIfAppropriate(spriteP, onGround, 5, true))
            return;
    }
    if (!onGround && GetOnPoleIfAppropriate(spriteP))
        return;

        // walking
    if (onGround && !heroP->isJumping && !heroP->isDoingPushups) {
        if (ladderWalkDir == 0) {
            direction = gKeys[kKeyRight] - gKeys[kKeyLeft];
            if (gKeys[kKeyJumpLeft])
                direction = -1;
            else if (gKeys[kKeyJumpRight])
                direction = 1;
        }
        else {
            direction = ladderWalkDir;
        }
        d->horizMoveDelta = direction * kWalkSpeed;
        if (direction != 0) {
            heroP->facingRight = direction >= 1;
            heroP->wasFacingRight = heroP->facingRight;
        }
        heroP->onGround = true;
        SetHeroGroundFrame(spriteP);

            // footsteps
        if (SWGetSpriteHorizMoveDelta(spriteP) != 0 && spriteP->curFrameIndex != heroP->lastStepFrame) {
            sound = GetRandom(4, 6);
            if (spriteP->curFrameIndex == 3) {
                heroP->lastStepFrame = 3;
                MMPlaySound(sound, 9, 1);
            }
            else if (spriteP->curFrameIndex == 7 && gSoundIsTurnedOn) {
                heroP->lastStepFrame = 7;
                MMPlayStereoSound2(NULL, sound, 9, 1, gSoundVolume, 1.0, 0);
            }
            heroP->lastStepFrame = (short)spriteP->curFrameIndex;
        }
    }

        // steering in the air
    if (gCanControlWhileJumping && (!onGround || heroP->isJumping) && !heroP->isDoingPushups) {
        jumpBase = heroP->jumpHorizDelta;
        direction = gKeys[kKeyRight] - gKeys[kKeyLeft];
        if (direction != 0 || !heroP->isJumping)
            d->horizMoveDelta = jumpBase + direction * kWalkSpeed;
        if (jumpBase < d->horizMoveDelta)
            heroP->facingRight = true;
        else if (d->horizMoveDelta < jumpBase)
            heroP->facingRight = false;
        else
            heroP->facingRight = heroP->wasFacingRight;
        heroP->wasFacingRight = heroP->facingRight;
        SetHeroJumpingFrame(spriteP);
    }

        // falling off an edge
    if (!onGround && !heroP->isJumping) {
        if (!heroP->wasOnGround) {
            if (abs(heroP->fallStartTop - spriteP->oldFrameRect.top) > 16) {
                SWSetSpriteFrameRange(spriteP, 13, 13);
                SWSetSpriteFlipX(spriteP, !heroP->facingRight);
                SWSetSpriteFrameTime(spriteP, -1);
            }
        }
        else {
            heroP->fallStartTop = spriteP->oldFrameRect.top;
        }
    }

        // jumping
    if (ProcessJumpIntention(spriteP, onGround))
        StartJump(spriteP);

    if (heroP->isJumping) {
        bottomNow = spriteP->destFrameRect.bottom - heroP->wallInset.bottom - 1;
        if (heroP->jumpStartBottom < bottomNow) {
            heroP->doorInset.top = heroP->normalDoorInsetTop;
            heroP->collisionInset.top = heroP->normalInsetTop;
        }
        else {
            if (heroP->jumpVertDelta != 0 && heroP->jumpPlatformP != NULL) {
                platformVert = SWGetSpriteVertMoveDelta(heroP->jumpPlatformP);
                if (platformVert == 0 || (heroP->jumpVertDelta < 0 && platformVert > 0) ||
                    (heroP->jumpVertDelta > 0 && platformVert < 0))
                    heroP->jumpVertDelta = 0;
            }
            heroP->jumpFrameCount++;
            d->vertMoveDelta = heroP->jumpVertDelta + (heroP->jumpFrameCount * gJumpChange - gJumpSpeed);

                // while above the take-off height, the insets shrink so the hero
                // can jump up through thin floors
            rise = heroP->jumpStartBottom - bottomNow;
            heroP->doorInset.top = heroP->normalDoorInsetTop + rise;
            heroP->collisionInset.top = heroP->normalInsetTop + rise;
            if (d->vertMoveDelta > 0) {
                heroP->doorInset.top = (short)(heroP->doorInset.top - (d->vertMoveDelta + 6.0f));
                heroP->collisionInset.top = (short)(heroP->collisionInset.top - (d->vertMoveDelta + 6.0f));
            }
            if (heroP->doorInset.top >= SWGetSpriteHeight(spriteP))
                heroP->doorInset.top = (short)(SWGetSpriteHeight(spriteP) - 1.0f);
            if (heroP->collisionInset.top >= SWGetSpriteHeight(spriteP))
                heroP->collisionInset.top = (short)(SWGetSpriteHeight(spriteP) - 1.0f);
        }
    }

    if (!onGround) {
        int fallen = (short)(SWGetSpriteVertLoc(spriteP) - heroP->lastGroundVertLoc) + 8;

        tilesFallen = FloorDiv16(fallen);
        if (tilesFallen >= getHeroFallingDeathDistance(spriteP) && !heroP->playedFallingSound) {
            MMPlaySound(0x28, 9, 1);
            heroP->playedFallingSound = true;
        }
    }
    if (!onGround && !heroP->isJumping && heroP->wasOnGround)
        d->horizMoveDelta = 0;

        // gravity
    d->vertMoveDelta += 1.3f;
    if (d->vertMoveDelta > 12.0f)
        d->vertMoveDelta = 12.0f;
    if (d->vertMoveDelta > 12.0f * ExtrasFallGravityFactor())       // moon gravity: slow falls
        d->vertMoveDelta = 12.0f * ExtrasFallGravityFactor();

    gHeroIsOnConveyorBelt = false;
    heroP->pedestalP = NULL;
    heroP->wasOnGround = onGround;
    onGround = MoveAndCollideMainSprite(spriteP);
    heroP->onGround = onGround;

    if (onGround)
        heroP->jumpHorizDelta = 0;
    if (onGround && gHeroIsOnConveyorBelt && gCurConveyorBeltDirection != 0) {
        if (FindSound(0x3f) == -1)
            MMPlayStereoSound2(NULL, 0x3f, 4, 2, (short)(gSoundVolume / 1.5), 1.0, -1);
    }
    else {
        StopChannel(4);
    }

        // landing
    if (!heroP->isJumping || d->vertMoveDelta < 0 || !onGround) {
        if (d->vertMoveDelta >= 0 && onGround && !heroP->wasOnGround)
            HandleHeroLanding(spriteP, abs(heroP->fallStartTop - spriteP->oldFrameRect.top) > 16, false);
    }
    else {
        landedOnSamePlatform = EndJump(spriteP);
        direction = gKeys[kKeyRight] - gKeys[kKeyLeft];
        d->horizMoveDelta = direction * kWalkSpeed;
        SWSetSpriteFrameAdvance(spriteP, 1);
        gHeroLandedThisFrame = true;
        HandleHeroLanding(spriteP, true, landedOnSamePlatform);
    }

    if (d->vertMoveDelta > 0 && gSpriteWorldP->workAreaRect.bottom + 100 <= spriteP->destFrameRect.top) {
        SetNextDeathCause(kDeathByFall);
        KillHero(spriteP);
    }

    if (heroP->pedestalP != NULL && !heroP->isDead)
        SaveHeroOnPedestal(spriteP, heroP->pedestalP);

        // idle animation after standing still a while
    if (!onGround || gKeys[kKeyLeft] + gKeys[kKeyRight] != 0 || gKeys[kKeyDown]) {
        heroP->idleDelay = GetRandom(160, 300);
        if (spriteP->frameChangeProc == IdleHeroFrameProc)
            SWSetSpriteFrameProc(spriteP, NULL);
    }
    else if (spriteP->frameChangeProc != IdleHeroFrameProc && heroP->idleDelay-- < 1) {
        SWSetSpriteFrameProc(spriteP, IdleHeroFrameProc);
        heroP->idleState = 0;
    }

    ExitRoomIfAppropriate(spriteP);
    SWSetGlobalMountOffset(gSpriteWorldP, (320.0f - SWGetSpriteHorizLoc(spriteP)) / 20.0f, 5.0f);
    UpdateHaloSpritePosition(spriteP);
}

void DeadHeroMoveProc(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);
    DrawDataPtr d = spriteP->drawData;
    SpritePtr platformP;
    SWRect inset = heroP->wallInset;
    Boolean onFloor;

        // ride platforms
    inset.bottom--;
    if (SWCheckSpriteWithSprites(spriteP, &platformP, kSW_CheckBottom, &inset, gVertMovingPlatformSpriteLayerP,
                                 gHorizMovingPlatformSpriteLayerP, true, false, false, false)) {
        SWOffsetSprite(spriteP, platformP->destFrameRect.left - platformP->oldFrameRect.left,
                       platformP->destFrameRect.top - platformP->oldFrameRect.top);
    }

    onFloor = CheckForWall(spriteP, kSW_CheckBottom, &heroP->wallInset, false, true, false);
    if (onFloor)
        d->horizMoveDelta = 0;

    if (heroP->isJumping &&
        spriteP->destFrameRect.bottom - heroP->wallInset.bottom - 1 <= heroP->jumpStartBottom) {
        heroP->jumpFrameCount++;
        d->vertMoveDelta = heroP->jumpFrameCount * gJumpChange - gJumpSpeed;
    }
    d->vertMoveDelta += 1.0f;
    if (d->vertMoveDelta > 9.0f)
        d->vertMoveDelta = 9.0f;

    heroP->wasOnGround = onFloor;
    onFloor = MoveAndCollideMainSprite(spriteP);
    heroP->onGround = onFloor;

    if (!heroP->isJumping || d->vertMoveDelta < 0 || !onFloor) {
        if (heroP->deathType == kDeathSkeletonCollapse && d->vertMoveDelta >= 0 && onFloor &&
            !heroP->wasOnGround && heroP->deathAnimIsDone && FindSound(8) == -1)
            MMPlayStereoSound(spriteP, 0x17, 9, 1);
    }
    else {
        heroP->isJumping = false;
        heroP->canJump = true;
    }

    if (heroP->deathType == kDeathSkeletonCollapse && !heroP->didSkeletonCollapse) {
        if (!onFloor) {
            if (heroP->collapseTimer > 0)
                heroP->collapseTimer--;
        }
        else {
            heroP->didSkeletonCollapse = true;
            SWSetSpriteFrameProc(spriteP, SkeletonCollapseFrameProc1);
            SWSetSpriteFrameAdvance(spriteP, 0);
            heroP->deathType = kDeathCollapsing;
        }
    }

    if (heroP->deathType == kDeathStoneCollapse && !heroP->didStoneCollapse &&
        (onFloor || SWCheckSpriteWithTiles(gSpriteWorldP, spriteP, kSW_CheckAll, NULL, 6, 6, 0x309, 0x30c, false))) {
        heroP->didStoneCollapse = true;
        SWSetSpriteFrameProc(spriteP, StoneCollapseFrameProc1);
        SWSetSpriteFrameAdvance(spriteP, 0);
        heroP->deathType = kDeathCollapsing;
        heroP->collapseTimer = 12;
        MMPlaySound(0x36, 9, 1);
    }

        // fell out of the room
    if (d->vertMoveDelta > 0 && gSpriteWorldP->workAreaRect.bottom <= spriteP->destFrameRect.top) {
        if (heroP->deathType == kDeathFellTooFar && heroP->attachedSpriteP != NULL)
            SWRemoveSpriteFromAnimation(heroP->attachedSpriteP, true);
        SWRemoveSpriteFromAnimation(spriteP, true);
        if (!heroP->deathAnimIsDone)
            gRestartLevel = true;
    }
}

/// The skeleton left behind follows the dead hero.
void LyingSkeletonMoveProc(SpritePtr spriteP)
{
    SpritePtr bodyP = HERO(spriteP)->deadBodyP;

    if (bodyP != NULL) {
        SW_ASSERT(HERO(bodyP)->attachedSpriteP != NULL);
        SWMoveSprite(spriteP, SWGetSpriteHorizLoc(bodyP), SWGetSpriteVertLoc(bodyP));
    }
}

void MoveWhenNotTouchingZapperBeam(SpritePtr spriteP)
{
    if (!IsSpriteCollidingWithLayer(spriteP, gZapperBeamSpriteLayerP, true, true))
        DeadHeroMoveProc(spriteP);
}

void HeroSinksIntoLavaMoveProc(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);
    DrawDataPtr d = spriteP->drawData;
    short depthLeft, frame;

    SW_ASSERT(spriteP->userData == kHeroSprite);
    depthLeft = (short)(heroP->lavaSurfaceRow - (SWGetSpriteVertLoc(spriteP) + 6.0f));
    if (depthLeft < 1) {
        KillHero(spriteP);
        return;
    }

    if (d->vertMoveDelta > 0.12f)
        d->vertMoveDelta /= heroP->lavaSinkFactor;
    heroP->lavaSinkFactor -= 0.004f;
    if (heroP->lavaSinkFactor < 1.01f)
        heroP->lavaSinkFactor = 1.01f;
    if (d->vertMoveDelta < 0.12f)
        d->vertMoveDelta = 0.12f;
    SWOffsetSprite(spriteP, 0, d->vertMoveDelta);

    if (depthLeft < 21)
        frame = 3;
    else if (depthLeft < 29)
        frame = 2;
    else if (depthLeft < 37)
        frame = 1;
    else
        frame = 0;
    SWSetCurrentFrameIndex(spriteP, frame + 51);
}

void SpriteSinksIntoLavaMoveProc(SpritePtr spriteP)
{
    short row, col, lastRow, lastCol, tile;

    lastRow = FloorDiv16(spriteP->destFrameRect.bottom);
    lastCol = FloorDiv16(spriteP->destFrameRect.right);
    for (row = FloorDiv16(spriteP->destFrameRect.top) - 1; row <= lastRow; row++) {
        for (col = FloorDiv16(spriteP->destFrameRect.left) - 1; col <= lastCol; col++) {
            tile = MyGetTile(gSpriteWorldP, 6, row, col);
            if (tile > 0x308 && tile < 0x331)
                goto foundLava;
        }
    }
foundLava:
    if (spriteP->destFrameRect.top < row * 16) {
        spriteP->drawData->vertMoveDelta += 0.004f;
        SWOffsetSprite(spriteP, 0, spriteP->drawData->vertMoveDelta);
    }
    else {
        if (spriteP->userData == kHeroSprite && HERO(spriteP)->attachedSpriteP != NULL)
            SWRemoveSpriteFromAnimation(HERO(spriteP)->attachedSpriteP, true);
        if (spriteP->userData < 0x50 || spriteP->userData > 0x5b)
            SWRemoveSpriteFromAnimation(spriteP, true);
        else
            MakeFallingPlatformReappearAfterDelay(spriteP);
    }
}

/// Moves the hero by its move deltas (plus wind/conveyor push), stopping at walls
/// and floors and opening doors it walks into. Returns true if it is on the floor.
Boolean MoveAndCollideMainSprite(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);
    DrawDataPtr d = spriteP->drawData;
    SpritePtr doorP;
    float push;
    Boolean onFloor = false;

    SW_ASSERT(gSpriteWorldP != NULL);
    SW_ASSERT(spriteP != NULL);

    SWOffsetSprite(spriteP, 0, d->vertMoveDelta);
    push = (!heroP->isJumping && heroP->platformP == NULL) ? gCurrentWindDelta : 0;

    if (d->vertMoveDelta > 0) {
        if (FloorDiv16(spriteP->oldFrameRect.bottom - heroP->wallInset.bottom - 1) <
            FloorDiv16(spriteP->destFrameRect.bottom - heroP->wallInset.bottom - 1)) {
            if (SWCheckSpriteWithTiles(gSpriteWorldP, spriteP, kSW_CheckBottom, &heroP->collisionInset, 4, 4,
                                       0x241, 0x243, false))
                push -= gCurConveyorBeltDirection;
            if (SWCheckSpriteWithTiles(gSpriteWorldP, spriteP, kSW_CheckBottom, &heroP->collisionInset, 4, 4,
                                       0x249, 0x24b, false))
                push += gCurConveyorBeltDirection;
        }
        if (CheckForWall(spriteP, kSW_CheckBottom, &heroP->wallInset, false, false, true)) {
            d->vertMoveDelta = 0;
            onFloor = true;
            RoundOffSpriteVertLoc(spriteP);
        }
    }

    SWOffsetSprite(spriteP, SWGetSpriteHorizMoveDelta(spriteP) + push, 0);
    if (onFloor)
        heroP->conveyorDelta = push;

    if (!heroP->isDead && !heroP->isJumping &&
        SWCheckSpriteWithSprites(spriteP, &doorP, kSW_CheckAll, &heroP->doorInset, gDoorSpriteLayerP,
                                 gDoorSpriteLayerP, true, false, false, false))
        OpenDoor(doorP);

    if (SWGetSpriteHorizMoveDelta(spriteP) + push > 0)
        CheckForWall(spriteP, kSW_CheckRight, &heroP->wallInset, heroP->isJumping, false, true);
    else if (SWGetSpriteHorizMoveDelta(spriteP) + push < 0)
        CheckForWall(spriteP, kSW_CheckLeft, &heroP->wallInset, heroP->isJumping, false, true);
    return onFloor;
}

void ClimbingSpriteMoveProc(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);
    TileMapStructPtr ladderMap, floorMap;
    short tileHeight = gSpriteWorldP->tileHeight, tileWidth = gSpriteWorldP->tileWidth;
    short feet, row, col, tile, tileAbove, tileBelow, tileHead, tile4, tile4Right;
    float vertOffset;
    Boolean leaveLadder = false;

    if (gGamepad[kKeyDown])
        gKeys[kKeyDown] = gGamepad[kKeyDown];
    SaveStartingPositionIfAppropriate(spriteP, false);
    if (spriteP->parentSpriteLayerP != gHeroSpriteLayerP) {
        SWRemoveSprite(spriteP);
        SWAddSprite(gHeroSpriteLayerP, spriteP);
    }

    spriteP->drawData->horizMoveDelta = 0;
    ladderMap = gSpriteWorldP->tileLayerArray[5];
    floorMap = gSpriteWorldP->tileLayerArray[4];
    vertOffset = (gKeys[kKeyDown] - gKeys[kKeyUp]) * kClimbSpeed;

        // reversing direction plays the animation the other way
    if (vertOffset != 0 && heroP->ladderDirection != 0 && sign(heroP->ladderDirection) != sign((short)vertOffset))
        heroP->frameDirection = spriteP->frameAdvance == 1 ? -1 : 1;

    if (vertOffset == 0) {
        SWSetSpriteFrameTime(spriteP, -1);
        SWSetSpriteFrameProc(spriteP, NULL);
    }
    else if (spriteP->frameChangeProc != HeroMiscFrameProc) {
        SWSetSpriteFrameProc(spriteP, HeroMiscFrameProc);
        SWSetSpriteFrameAdvance(spriteP, 0);
        SWSetSpriteFrameTime(spriteP, 0);
        heroP->frameTimer = 3;
        heroP->frameDelay = 3;
        if (heroP->frameDirection == 0)
            heroP->frameDirection = 1;
    }

    SWOffsetSprite(spriteP, 0, vertOffset);
    if (vertOffset != 0)
        heroP->ladderDirection = (short)vertOffset;

    col = (short)((spriteP->destFrameRect.left + (short)((spriteP->destFrameRect.right - spriteP->destFrameRect.left) / 2) - 4) / tileWidth);

        // climbing up into a new tile row: at the top of the ladder?
    feet = spriteP->destFrameRect.bottom - heroP->wallInset.bottom - 21;
    if (vertOffset < 0 && heroP->hasClimbed &&
        feet / tileHeight < (short)(spriteP->oldFrameRect.bottom - heroP->wallInset.bottom - 21) / tileHeight) {
        row = feet / tileHeight;
        tile = GetTileID(ladderMap, row, col);
        tileAbove = GetTileID(ladderMap, row - 1, col);
        tileBelow = GetTileID(ladderMap, row + 1, col);
        tileHead = GetTileID(ladderMap, (short)((spriteP->destFrameRect.top + 25) / tileHeight), col);
        if (row < ladderMap->numRows && GetLadderValue(tile) == 1) {
                // climb off the top
            SWMoveSprite(spriteP, spriteP->destFrameRect.left,
                         (short)(row * 16 - ((spriteP->destFrameRect.bottom - heroP->wallInset.bottom) -
                                             spriteP->destFrameRect.top) + 37));
            heroP->ladderDirection = -1;
            heroP->ladderStepTimer = 4;
            SWSetSpriteMoveProc(spriteP, FinishClimbingLadderMoveProc);
            SWSetSpriteFrameProc(spriteP, NULL);
        }
        else if (GetLadderValue(tile) == 0 && GetLadderValue(tileBelow) == 0 &&
                 GetLadderValue(tileAbove) == 0 && GetLadderValue(tileHead) == 0) {
            leaveLadder = true;
        }
    }

    feet = spriteP->destFrameRect.bottom - heroP->wallInset.bottom;
    if (vertOffset <= 0 ||
        feet / tileHeight <= (short)(spriteP->oldFrameRect.bottom - heroP->wallInset.bottom) / tileHeight) {
        if (vertOffset > 0) {
                // reached a platform at the bottom?
            row = feet / tileHeight;
            if (GetLadderValue(GetTileID(ladderMap, row, col)) == 0 &&
                GetLadderValue(GetTileID(ladderMap, row + 1, col)) == 0 &&
                SWCheckSpriteWithSprites(spriteP, NULL, kSW_CheckBottom, &heroP->collisionInset,
                                         gVertMovingPlatformSpriteLayerP, gHorizMovingPlatformSpriteLayerP,
                                         false, true, false, true)) {
                leaveLadder = true;
                MMPlaySound(3, 9, 3);
            }
        }
    }
    else {
            // climbing down into a new tile row: reached the floor?
        col = (short)((spriteP->destFrameRect.left + (short)((spriteP->destFrameRect.right - spriteP->destFrameRect.left) / 2) - 4) / tileWidth);
        tileHead = GetTileID(ladderMap, (short)((spriteP->destFrameRect.top + 25) / tileHeight), col);
        row = feet / tileHeight;
        tile = GetTileID(ladderMap, row, col);
        tileAbove = GetTileID(ladderMap, row - 1, col);
        tile4 = GetTileID(floorMap, row, col);
        tile4Right = GetTileID(floorMap, row, col + 1);
        if ((GetLadderValue(tileAbove) == 0 || GetLadderValue(tileAbove) == 3) &&
            (IsTileFloorTile(tile4, true) || IsTileFloorTile(tile4Right, true))) {
            SWOffsetSprite(spriteP, 0, -kClimbSpeed);
            leaveLadder = true;
            MMPlaySound(3, 9, 3);
        }
        else if ((GetLadderValue(tileHead) == 0 || GetLadderValue(tileHead) == 3) &&
                 GetLadderValue(tileAbove) == 0 && GetLadderValue(tile) != 1) {
            leaveLadder = true;
            if (CheckForWall(spriteP, kSW_CheckBottom, &heroP->collisionInset, false, false, true))
                MMPlaySound(3, 9, 3);
        }
    }

    if (leaveLadder) {
        SWSetSpriteMoveProc(spriteP, MainSpriteMoveProc);
        SWSetSpriteFrameProc(spriteP, NULL);
        SWSetSpriteMoveDelta(spriteP, 0, 0);
        StepOffLadder(spriteP, true);
        heroP->ladderDirection = 0;
    }
    heroP->hasClimbed = true;
    ExitRoomIfAppropriate(spriteP);
    UpdateHaloSpritePosition(spriteP);
}

/// Climbing over the top of a ladder onto the floor (frames 26-29), or back down.
void FinishClimbingLadderMoveProc(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);
    short frame, nextFrame = 0, offset = 0, index;
    Boolean reachedTop = false, backOnLadder = false;

    SWSetSpriteFrameProc(spriteP, NULL);
    if (spriteP->parentSpriteLayerP != gHeroSpriteLayerP) {
        SWRemoveSprite(spriteP);
        SWAddSprite(gHeroSpriteLayerP, spriteP);
    }

    if (heroP->ladderStepTimer-- >= 1)
        return;

    heroP->ladderStepTimer = 4;
    frame = (short)spriteP->curFrameIndex;
    SWSetSpriteFrameAdvance(spriteP, 0);

    if (frame < 26 || frame > 29) {
        SWSetSpriteFrameRange(spriteP, 26, 29);
        SWSetSpriteFlipX(spriteP, !heroP->facingRight);
        if (heroP->ladderDirection < 1) {
            nextFrame = 26;
            offset = 0;
        }
        else {
            nextFrame = 29;
            offset = 8;
        }
    }
    else {
        if (gKeys[kKeyDown] || gKeys[kKeyUp])
            heroP->ladderDirection = gKeys[kKeyDown] ? 1 : -1;
        index = frame - 26;
        if (heroP->ladderDirection < 0)
            index = frame - 25;
        SW_ASSERT(index >= 0);
        SW_ASSERT(index <= 5);
        offset = ladderOffsetArray[index];
        if (heroP->ladderDirection < 1) {
            offset = -offset;
            nextFrame = frame + 1;
            if (frame == 29)
                reachedTop = true;
        }
        else {
            nextFrame = frame - 1;
            if (frame == 26)
                backOnLadder = true;
        }
    }

    if (!reachedTop && !backOnLadder)
        SWSetCurrentFrameIndex(spriteP, nextFrame);
    SWOffsetSprite(spriteP, 0, offset);

    if (reachedTop) {
        SWSetSpriteMoveProc(spriteP, MainSpriteMoveProc);
        SWSetSpriteFrameAdvance(spriteP, 1);
        SWSetSpriteMoveDelta(spriteP, 0, 0);
        StepOffLadder(spriteP, false);
    }
    else if (backOnLadder) {
        SWSetSpriteMoveProc(spriteP, ClimbingSpriteMoveProc);
        SWResetSpriteMovementTimer(gSpriteWorldP, spriteP);
        SWSetSpriteFrameTime(spriteP, -1);
        SWResetSpriteFrameTimer(gSpriteWorldP, spriteP);
        SWSetSpriteFrameAdvance(spriteP, 1);
        if (spriteP->curFrameIndex == 26)
            nextFrame = 20;
        SWSetSpriteFrameRange(spriteP, 18, 25);
        SWSetSpriteFlipX(spriteP, false);
        heroP->hasClimbed = false;
        SWSetCurrentFrameIndex(spriteP, nextFrame);
        SWOffsetSprite(spriteP, 0, 1.0f);
        ClimbingSpriteMoveProc(spriteP);
    }
    UpdateHaloSpritePosition(spriteP);
}

static Boolean IsVineTile(short tile)
{
    return tile >= kFirstVineTile && tile <= kLastVineTile;
}

void VineClimbingMoveProc(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);
    DrawDataPtr d = spriteP->drawData;
    TileMapStructPtr vineMap, floorMap;
    short tileHeight = gSpriteWorldP->tileHeight, tileWidth = gSpriteWorldP->tileWidth;
    short top, bottom, row, col, tileAbove, tileBelow, tileBelow4, tileTop, tileHere, tile4;
    float vertOffset;
    Boolean leaveVine = false;

    if (gGamepad[kKeyDown])
        gKeys[kKeyDown] = gGamepad[kKeyDown];
    if (spriteP->parentSpriteLayerP != gHeroSpriteLayerP) {
        SWRemoveSprite(spriteP);
        SWAddSprite(gHeroSpriteLayerP, spriteP);
    }

    vineMap = gSpriteWorldP->tileLayerArray[heroP->vineTileLayer];
    floorMap = gSpriteWorldP->tileLayerArray[4];
    SaveStartingPositionIfAppropriate(spriteP, false);

    if ((gKeys[kKeyJump] && gKeys[kKeyLeft] != gKeys[kKeyRight]) || gKeys[kKeyJumpLeft] || gKeys[kKeyJumpRight]) {
            // jump off sideways
        if (gKeys[kKeyJumpLeft])
            d->horizMoveDelta = -1.0f;
        else if (gKeys[kKeyJumpRight])
            d->horizMoveDelta = 1.0f;
        else
            d->horizMoveDelta = gKeys[kKeyRight] - gKeys[kKeyLeft];
        d->horizMoveDelta *= kWalkSpeed;
        heroP->facingRight = d->horizMoveDelta > 0;
        StartJump(spriteP);
        SWOffsetSprite(spriteP, d->horizMoveDelta, 0);
        SWSetSpriteMoveProc(spriteP, MainSpriteMoveProc);
        return;
    }

    d->horizMoveDelta = 0;
    vertOffset = (gKeys[kKeyDown] - gKeys[kKeyUp]) * kClimbSpeed;
    MySetSpriteFrameTime(spriteP, vertOffset != 0 ? 83 : -1);
    SWOffsetSprite(spriteP, SWGetSpriteHorizMoveDelta(spriteP), vertOffset);

    top = spriteP->destFrameRect.top + heroP->vineInset.top;
    bottom = spriteP->destFrameRect.bottom - heroP->vineInset.bottom;
    col = (short)((spriteP->destFrameRect.left + (short)((spriteP->destFrameRect.right - spriteP->destFrameRect.left) / 2)) / tileWidth);

    if (vertOffset < 0 && bottom / tileHeight < (short)(spriteP->oldFrameRect.bottom - heroP->vineInset.bottom) / tileHeight) {
            // climbed into a new row: past the top of the vine?
        row = bottom / tileHeight;
        tileAbove = GetTileID(vineMap, row - 1, col);
        tileBelow = GetTileID(vineMap, row + 1, col);
        tileBelow4 = GetTileID(floorMap, row + 1, col);
        tileTop = GetTileID(vineMap, top / tileHeight, col);
        FixHeroPositionOnVine(spriteP);
        if (!IsVineTile(tileAbove) && !IsVineTile(tileTop)) {
            leaveVine = true;
            if (IsTileFloorTile(tileBelow, true) || IsTileFloorTile(tileBelow4, true))
                MMPlaySound(3, 9, 3);
        }
        else if ((IsTileFloorTile(tileBelow, true) || IsTileFloorTile(tileBelow4, true)) && !IsVineTile(tileTop)) {
            leaveVine = true;
            MMPlaySound(3, 9, 3);
        }
    }

    if (vertOffset > 0) {
        tileTop = GetTileID(vineMap, top / tileHeight, col);
        row = bottom / tileHeight;
        tileAbove = GetTileID(vineMap, row - 1, col);
        tileHere = GetTileID(vineMap, row, col);
        tile4 = GetTileID(floorMap, row, col);
        if (IsTileFloorNotVineTile(tileHere) || IsTileFloorNotVineTile(tile4)) {
            SWOffsetSprite(spriteP, 0, -vertOffset - kClimbSpeed);
            MoveSpriteDownToFloor(spriteP);
            leaveVine = true;
            MMPlaySound(3, 9, 3);
        }
        if (!IsVineTile(tileTop) && !IsVineTile(tileAbove))
            leaveVine = true;
    }

    if (leaveVine) {
        SWSetSpriteMoveProc(spriteP, MainSpriteMoveProc);
        SWSetSpriteMoveDelta(spriteP, 0, 0);
        MoveSpriteDownToFloor(spriteP);
        MoveSpriteDownToFloor(spriteP);
        MoveSpriteDownToFloor(spriteP);
        heroP->onGround = true;
        SetHeroGroundFrame(spriteP);
        gKeys[kKeyUp] = false;
        gKeys[kKeyDown] = false;
    }
    ExitRoomIfAppropriate(spriteP);
    UpdateHaloSpritePosition(spriteP);
}

void PoleSlidingMoveProc(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);
    TileMapStructPtr poleMap;
    short row, col, tile;
    Boolean leavePole = false;

    SaveStartingPositionIfAppropriate(spriteP, false);
    if (spriteP->destFrameRect.top < gSpriteWorldP->workAreaRect.bottom + 100)
        SWOffsetSprite(spriteP, 0, 6.0f);
    ProcessJumpIntention(spriteP, heroP->onGround);

    if (CheckForWall(spriteP, kSW_CheckBottom, &heroP->poleInset, false, false, true)) {
        leavePole = true;
        heroP->onGround = true;
        MMPlaySound(3, 9, 1);
        SetHeroGroundFrame(spriteP);
    }
    else {
            // slid off the bottom of the pole?
        poleMap = gSpriteWorldP->tileLayerArray[4];
        col = (short)((spriteP->destFrameRect.left + (short)(SWGetSpriteWidth(spriteP) / 2.0f)) / gSpriteWorldP->tileWidth);
        row = (short)((spriteP->destFrameRect.bottom - 15) / gSpriteWorldP->tileHeight);
        tile = GetTileID(poleMap, row, col);
        if (tile < kFirstPoleTile || tile > kLastPoleTile) {
            tile = GetTileID(poleMap, row - 1, col);
            if (tile < kFirstPoleTile || tile > kLastPoleTile) {
                leavePole = true;
                heroP->onGround = false;
            }
        }
    }

    if (leavePole) {
        SWSetSpriteMoveProc(spriteP, MainSpriteMoveProc);
        SWSetSpriteMoveDelta(spriteP, 0, 0);
        gKeys[kKeyJump] = false;
        gKeys[kKeyJumpLeft] = false;
        gKeys[kKeyJumpRight] = false;
        SaveLastOnGroundPixelRow(spriteP);
    }
    ExitRoomIfAppropriate(spriteP);
    UpdateHaloSpritePosition(spriteP);
}

/// After restarting a room the hero blinks until a key is pressed.
void BlinkingSpriteMoveProc(SpritePtr spriteP)
{
    if (gKeys[kKeyLeft] || gKeys[kKeyRight] || gKeys[kKeyUp] || gKeys[kKeyDown] || gKeys[kKeyJump] ||
        gKeys[kKeyJumpLeft] || gKeys[kKeyJumpRight] || gKeys[kKeyAction]) {
        SWSetSpriteFrameProc(spriteP, NULL);
        SWSetSpriteFrameTime(spriteP, -1);
        SWSetSpriteFrameAdvance(spriteP, 1);
        SWSetSpriteVisible(spriteP, true);
        SWRemoveSprite(spriteP);
        SWAddSprite(gHeroSpriteLayerP, spriteP);
        if (gStartingSpriteState == 1) {
            SWSetSpriteMoveProc(spriteP, ClimbingSpriteMoveProc);
            SWSetSpriteFrameAdvanceMode(spriteP, kNoLoopingMode);
            HERO(spriteP)->hasClimbed = false;
            HERO(spriteP)->isLanding = false;
        }
        else if (gStartingSpriteState == 2) {
            GetOnVine(spriteP);
            HERO(spriteP)->vineTileLayer = gStartingVineTileLayer;
        }
        else if (gStartingSpriteState == 3) {
            GetOnPole(spriteP, 0);
        }
        else {
            SWSetSpriteMoveProc(spriteP, MainSpriteMoveProc);
        }
    }
    UpdateHaloSpritePosition(spriteP);
}

/// Keeps the hero's head from poking up into a wall above the vine.
void FixHeroPositionOnVine(SpritePtr spriteP)
{
    short col, y, top, row, tile, vineTile, rowsDown = 1;

    col = FloorDiv16(spriteP->destFrameRect.left + (short)(SWGetSpriteWidth(spriteP) / 2.0f));
    top = spriteP->destFrameRect.top + 16;
    for (y = top; y <= top + 40; y += 16) {
        row = y / gSpriteWorldP->tileHeight;
        tile = MyGetTile(gSpriteWorldP, 4, row, col);
        vineTile = MyGetTile(gSpriteWorldP, HERO(spriteP)->vineTileLayer, row + 1, col);
        if (IsWallTile(tile) && vineTile > 0x1f8 && vineTile < 0x20b)
            SWOffsetSprite(spriteP, 0, (short)(gSpriteWorldP->tileHeight * (row + rowsDown) - y));
        rowsDown++;
    }
}

Boolean GetOnVineIfAppropriate(SpritePtr spriteP, Boolean onGround, short tileLayer, Boolean playSound)
{
    HeroSpritePtr heroP = HERO(spriteP);
    SWRect vineRect, centeredRect;
    short offset;
    Boolean found;

    heroP->vineTileLayer = tileLayer;
    if (!onGround)
        found = GetLadderRect(gSpriteWorldP, spriteP, &heroP->vineInset, &vineRect, tileLayer, 0x1fb, 0x20a);
    else
        found = GetLadderRect(gSpriteWorldP, spriteP, &heroP->vineFromGroundInset, &vineRect, tileLayer, 0x1fb, 0x1fc);
    if (!found)
        return false;

    centeredRect = spriteP->destFrameRect;
    SWCenterRect(&centeredRect, &vineRect);
    offset = centeredRect.left - spriteP->destFrameRect.left;
    if (offset <= -17 || offset >= 17)
        return false;
    if (sign(offset) != sign((short)SWGetSpriteHorizMoveDelta(spriteP)) && SWGetSpriteHorizMoveDelta(spriteP) != 0)
        return false;

    StopMySound(0x28);
    StopChannel(4);
    if (!onGround && playSound)
        MMPlaySound(10, 9, 1);
    EndJump(spriteP);
    if (!onGround) {
        SWOffsetSprite(spriteP, 0, abs(offset));
        while (!GetLadderRect(gSpriteWorldP, spriteP, &heroP->vineInset, &vineRect, tileLayer, 0x1fb, 0x20a))
            SWOffsetSprite(spriteP, 0, -2.0f);
    }
    CheckForWall(spriteP, kSW_CheckBottom, &heroP->wallInset, heroP->isJumping, false, true);
    SWOffsetSprite(spriteP, offset, 0);
    GetOnVine(spriteP);
    FixHeroPositionOnVine(spriteP);
    return true;
}

Boolean GetOnPoleIfAppropriate(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);
    SWRect poleRect, centeredRect, checkInset;
    SWTileSearchType direction;
    short offset;
    Boolean getOn = false;

    SW_ASSERT(spriteP->userData == kHeroSprite);
    if (!GetPoleRect(gSpriteWorldP, spriteP, &heroP->vineInset, &poleRect, 4, 0x1e7, 0x1f8) &&
        !GetPoleRect(gSpriteWorldP, spriteP, &heroP->vineInset, &poleRect, 5, 0x1e7, 0x1f8))
        return false;

    centeredRect = spriteP->destFrameRect;
    SWCenterRect(&centeredRect, &poleRect);
    offset = centeredRect.left - spriteP->destFrameRect.left;

    if (!heroP->isJumping) {
        if (offset > -17 && offset < 17) {
                // only if there's room to move onto it
            SWOffsetSprite(spriteP, offset, 0);
            checkInset = heroP->collisionInset;
            if (offset < 0) {
                direction = kSW_CheckLeft;
                checkInset.left += 16;
            }
            else {
                direction = kSW_CheckRight;
                checkInset.right += 16;
            }
            if (!CheckForWall(spriteP, direction, &checkInset, heroP->isJumping, false, false)) {
                getOn = true;
                offset = 0;
            }
            else {
                SWOffsetSprite(spriteP, -offset, 0);
            }
        }
    }
    else if (offset > -13 && offset < 13 && SWGetSpriteHorizMoveDelta(spriteP) != 0 &&
             sign(offset) == sign((short)SWGetSpriteHorizMoveDelta(spriteP))) {
        getOn = true;
    }

    if (getOn) {
        GetOnPole(spriteP, offset);
        PoleSlidingMoveProc(spriteP);
    }
    return getOn;
}

void GetOnVine(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);

    SWSetSpriteMoveProc(spriteP, VineClimbingMoveProc);
    SWSetSpriteFrameProc(spriteP, NULL);
    SWSetSpriteFrameRange(spriteP, 30, 33);
    SWSetSpriteFlipX(spriteP, false);
    SWSetSpriteFrameAdvanceMode(spriteP, kNoLoopingMode);
    SWSetSpriteFrameAdvance(spriteP, 1);
    heroP->hasClimbed = false;
    heroP->isLanding = false;
    heroP->playedFallingSound = false;
    heroP->platformP = NULL;
    heroP->jumpHorizDelta = 0;
    heroP->jumpVertDelta = 0;
    heroP->platformHorizDelta = 0;
    heroP->unused154 = 0;
    heroP->conveyorDelta = 0;
    heroP->jumpBufferFrames = 0;
}

void GetOnPole(SpritePtr spriteP, short offset)
{
    SWSetSpriteFrameRange(spriteP, 31, 31);
    SWSetSpriteFlipX(spriteP, false);
    EndJump(spriteP);
    HERO(spriteP)->jumpBufferFrames = 0;
    StopMySound(0x28);
    StopChannel(4);
    SWOffsetSprite(spriteP, offset, 0);
    SWSetSpriteMoveDelta(spriteP, 0, 0);
    SWSetSpriteMoveProc(spriteP, PoleSlidingMoveProc);
    SWSetSpriteFrameProc(spriteP, NULL);
    SWSetSpriteFrameTime(spriteP, -1);
    HERO(spriteP)->playedFallingSound = false;
}

// ---------------------------------------------------------------------------
// Enemies

/// Skull spiders walk along floors and turn around at walls and ledges.
void SkullSpiderMoveProc(SpritePtr spriteP)
{
    SkullSpiderSpritePtr spiderP = SKULLSPIDER(spriteP);
    DrawDataPtr d = spriteP->drawData;
    TileMapStructPtr floorMap = gSpriteWorldP->tileLayerArray[4];
    SWRect bodyRect, trapDoorInset, switchInset;
    short row, col, tile;
    float push;
    Boolean onPlatform, onFloor = false, turn;

    onPlatform = HandleSpriteWithMovingPlatforms(spriteP, spiderP->collisionInset);
    ApplyEnemyGravity(spriteP, 0.43333334f, 1.0f, 12.0f);
    SWOffsetSprite(spriteP, 0, d->vertMoveDelta);

    push = gCurrentWindDelta;
    if (d->vertMoveDelta > 0) {
        push += ConveyorPush(spriteP, &spiderP->collisionInset);
        if (CheckForWall(spriteP, kSW_CheckBottom, &spiderP->collisionInset, false, false, true)) {
            d->vertMoveDelta = 0;
            onFloor = true;
        }
    }
    SWOffsetSprite(spriteP, SWGetSpriteHorizMoveDelta(spriteP) + push, 0);

    bodyRect.top = spriteP->destFrameRect.top + spiderP->collisionInset.top;
    bodyRect.left = spriteP->destFrameRect.left + spiderP->collisionInset.left;
    bodyRect.bottom = spriteP->destFrameRect.bottom - spiderP->collisionInset.bottom;
    bodyRect.right = spriteP->destFrameRect.right - spiderP->collisionInset.right;
    SWOffsetRect(&bodyRect, -2, 0);
    trapDoorInset = spiderP->collisionInset;
    trapDoorInset.bottom -= 16;
    switchInset = trapDoorInset;
    switchInset.left -= 8;

        // turn at walls, and at ledges with no floor, trap door or switch below
    if (SWGetSpriteHorizMoveDelta(spriteP) + push > 0) {
        col = FloorDiv16(bodyRect.right + 1);
        row = FloorDiv16(bodyRect.bottom + 1);
        tile = GetTileID(floorMap, row, col);
        turn = CheckForWall(spriteP, kSW_CheckRight, &spiderP->collisionInset, false, false, true) ||
               (!IsTileFloorTile(tile, false) &&
                !SWCheckSpriteWithSprites(spriteP, NULL, kSW_CheckRight, &trapDoorInset, gTrapDoorSpriteLayerP,
                                          gTrapDoorSpriteLayerP, false, true, true, false) &&
                !SWCheckSpriteWithSprites(spriteP, NULL, kSW_CheckAll, &switchInset, gBrickSwitchSpriteLayerP,
                                          gBrickSwitchSpriteLayerP, false, true, true, false));
        if (turn && SWGetSpriteHorizMoveDelta(spriteP) > 0 && !onPlatform)
            d->horizMoveDelta = -d->horizMoveDelta;
    }
    else if (SWGetSpriteHorizMoveDelta(spriteP) + push < 0) {
        col = FloorDiv16(bodyRect.left);
        row = FloorDiv16(bodyRect.bottom + 1);
        tile = GetTileID(floorMap, row, col);
        turn = CheckForWall(spriteP, kSW_CheckLeft, &spiderP->collisionInset, false, false, true) ||
               (!IsTileFloorTile(tile, false) &&
                !SWCheckSpriteWithSprites(spriteP, NULL, kSW_CheckLeft, &trapDoorInset, gTrapDoorSpriteLayerP,
                                          gTrapDoorSpriteLayerP, false, true, true, false) &&
                !SWCheckSpriteWithSprites(spriteP, NULL, kSW_CheckAll, &switchInset, gBrickSwitchSpriteLayerP,
                                          gBrickSwitchSpriteLayerP, false, true, true, false));
        if (turn && SWGetSpriteHorizMoveDelta(spriteP) < 0 && !onPlatform)
            d->horizMoveDelta = -d->horizMoveDelta;
    }

    if (bodyRect.left < gSpriteWorldP->workAreaRect.left && SWGetSpriteHorizMoveDelta(spriteP) < 0)
        d->horizMoveDelta = -d->horizMoveDelta;
    else if (gSpriteWorldP->workAreaRect.right < bodyRect.right && SWGetSpriteHorizMoveDelta(spriteP) > 0)
        d->horizMoveDelta = -d->horizMoveDelta;

    SWSetSpriteFlipX(spriteP, SWGetSpriteHorizMoveDelta(spriteP) > 0);
    RemoveSpriteIfOutsideRoom(spriteP);
    if (!onFloor)
        CheckSpriteWithLavaTiles(spriteP, true);
}

void ScorpionMoveProc(SpritePtr spriteP)
{
    CrawlerSpritePtr scorpionP = CRAWLER(spriteP);
    DrawDataPtr d = spriteP->drawData;
    float push;
    Boolean onFloor = false;

    HandleSpriteWithMovingPlatforms(spriteP, scorpionP->collisionInset);
    ApplyEnemyGravity(spriteP, 0.43333334f, 1.0f, 8.0f);
    SWOffsetSprite(spriteP, 0, d->vertMoveDelta);

    push = gCurrentWindDelta;
    if (d->vertMoveDelta > 0) {
        push += ConveyorPush(spriteP, &scorpionP->collisionInset);
        if (CheckForWall(spriteP, kSW_CheckBottom, &scorpionP->collisionInset, false, false, true)) {
            d->vertMoveDelta = 0;
            onFloor = true;
        }
    }
    SWOffsetSprite(spriteP, SWGetSpriteHorizMoveDelta(spriteP) + push, 0);
    TurnAtWalls(spriteP, &scorpionP->collisionInset, push);

    if ((short)(spriteP->destFrameRect.left + scorpionP->collisionInset.left) < gSpriteWorldP->workAreaRect.left &&
        SWGetSpriteHorizMoveDelta(spriteP) < 0)
        d->horizMoveDelta = -d->horizMoveDelta;
    else if (gSpriteWorldP->workAreaRect.right < spriteP->destFrameRect.right && d->horizMoveDelta > 0)
        d->horizMoveDelta = -d->horizMoveDelta;

    SWSetSpriteFlipX(spriteP, SWGetSpriteHorizMoveDelta(spriteP) > 0);
    RemoveSpriteIfOutsideRoom(spriteP);
    if (!onFloor)
        CheckSpriteWithLavaTiles(spriteP, true);
}

void EyeballMoveProc(SpritePtr spriteP)
{
    EyeballSpritePtr eyeP = EYEBALL(spriteP);
    DrawDataPtr d = spriteP->drawData;
    SpritePtr slimeP;
    float wind = gCurrentWindDelta, horizLoc, vertLoc, landedVertLoc;

    d->vertMoveDelta += 0.21666667f;
    SWOffsetSprite(spriteP, 0, d->vertMoveDelta);

    if (d->vertMoveDelta > 0) {
        horizLoc = SWGetSpriteHorizLoc(spriteP);
        vertLoc = SWGetSpriteVertLoc(spriteP);
        if (CheckForWall(spriteP, kSW_CheckBottom, &eyeP->collisionInset, false, false, true)) {
                // splat: start squashing
            landedVertLoc = SWGetSpriteVertLoc(spriteP);
            SWMoveSprite(spriteP, horizLoc, vertLoc - d->vertMoveDelta);
            MMPlayStereoSound(spriteP, GetRandom(16, 17), 11, 1);
            eyeP->landedHorizLoc = SWGetSpriteHorizLoc(spriteP);
            eyeP->landedVertLoc = SWGetSpriteVertLoc(spriteP);
            eyeP->bounceSpeed = d->vertMoveDelta;
            eyeP->normalHeight = (short)SWGetSpriteHeight(spriteP);
            eyeP->normalWidth = (short)SWGetSpriteWidth(spriteP);
            eyeP->numSquishFrames = 0;
            d->vertMoveDelta = d->vertMoveDelta >= 8.0f ? 8.0f : 7.0f;
            eyeP->squishTimer = 1;
            SWSetSpriteMoveProc(spriteP, SquishEyeballMoveProc);
            slimeP = NewSlimeSprite();
            SWSetSpriteLocation(slimeP, horizLoc, landedVertLoc + 17.0f);
        }
    }

    SWOffsetSprite(spriteP, d->horizMoveDelta + wind, 0);
    TurnAtWalls(spriteP, &eyeP->collisionInset, wind);
    BounceOffRoomSides(spriteP, &gSpriteWorldP->workAreaRect);
    RemoveSpriteIfOutsideRoom(spriteP);
    CheckSpriteWithLavaTiles(spriteP, true);
}

/// Squashes the eyeball against the floor (scaling it), then plays the same
/// heights backward as it springs back up.
void SquishEyeballMoveProc(SpritePtr spriteP)
{
    EyeballSpritePtr eyeP = EYEBALL(spriteP);
    DrawDataPtr d = spriteP->drawData;
    short height, width, newHeight, newWidth;

    if (eyeP->squishTimer-- >= 1)
        return;
    eyeP->squishTimer = 1;

    if (eyeP->numSquishFrames > 0 || d->vertMoveDelta >= 0) {
        height = (short)SWGetSpriteHeight(spriteP);
        if (d->vertMoveDelta > 0) {
            newHeight = (short)(height - d->vertMoveDelta);
            d->vertMoveDelta /= 1.5f;
            if (eyeP->numSquishFrames < kMaxEyeballSquishFrames)
                eyeP->squishHeights[eyeP->numSquishFrames++] = newHeight;
        }
        else {
            eyeP->numSquishFrames--;
            SW_ASSERT(eyeP->numSquishFrames >= 0);
            SW_ASSERT(eyeP->squishHeights[eyeP->numSquishFrames] >= 0);
            SW_ASSERT(eyeP->squishHeights[eyeP->numSquishFrames] <= 999);
            newHeight = eyeP->squishHeights[eyeP->numSquishFrames];
        }
        if (d->vertMoveDelta > 0 && d->vertMoveDelta <= 2.0f) {
            d->vertMoveDelta = -d->vertMoveDelta;
            eyeP->numSquishFrames--;
        }
        width = (short)SWGetSpriteWidth(spriteP);
        newWidth = (short)((eyeP->normalHeight - newHeight) / 1.5 + eyeP->normalWidth);
        SWSetSpriteScaledHeight(spriteP, newHeight);
        SWSetSpriteScaledWidth(spriteP, newWidth);
        SWOffsetSprite(spriteP, (width - newWidth) / 2, height - newHeight);
    }
    else {
            // back to normal: bounce off
        d->vertMoveDelta = -eyeP->bounceSpeed;
        SWSetSpriteMoveProc(spriteP, EyeballMoveProc);
        SWSetSpriteDrawProc(spriteP, gSpriteDrawProc);
        SWSetSpriteScaledHeight(spriteP, eyeP->normalHeight);
        SWSetSpriteScaledWidth(spriteP, eyeP->normalWidth);
        SWSetSpriteMoveTime(spriteP, 0);
        SWMoveSprite(spriteP, eyeP->landedHorizLoc, eyeP->landedVertLoc);
    }
}

void BlueRavenMoveProc(SpritePtr spriteP)
{
    RavenSpritePtr ravenP = RAVEN(spriteP);
    DrawDataPtr d = spriteP->drawData;

    SWOffsetSprite(spriteP, gCurrentWindDelta + d->horizMoveDelta, 0);
    if (d->horizMoveDelta + gCurrentWindDelta > 0) {
        if (CheckForWall(spriteP, kSW_CheckRight, &ravenP->collisionInset, false, false, true))
            d->horizMoveDelta = -d->horizMoveDelta;
    }
    else if (CheckForWall(spriteP, kSW_CheckLeft, &ravenP->collisionInset, false, false, true)) {
        d->horizMoveDelta = -d->horizMoveDelta;
    }
    BounceOffRoomSides(spriteP, &gSpriteWorldP->workAreaRect);
    SWSetSpriteFlipX(spriteP, SWGetSpriteHorizMoveDelta(spriteP) > 0);
    RemoveSpriteIfOutsideRoom(spriteP);
}

void RedRavenMoveProc(SpritePtr spriteP)
{
    RavenSpritePtr ravenP = RAVEN(spriteP);
    DrawDataPtr d = spriteP->drawData;

    if (ravenP->baseVertLoc == -1.0f)
        ravenP->baseVertLoc = SWGetSpriteVertLoc(spriteP);
    ravenP->angle += ravenP->angleStep;
    if (ravenP->angle >= 360.0)
        ravenP->angle -= 360.0;
    SWMoveSprite(spriteP, gCurrentWindDelta + SWGetSpriteHorizLoc(spriteP) + SWGetSpriteHorizMoveDelta(spriteP),
                 (float)(ravenP->amplitude * MySin(ravenP->angle) + ravenP->baseVertLoc));

    if (!ravenP->ignoresWalls) {
        if (d->horizMoveDelta + gCurrentWindDelta > 0) {
            if (CheckForWall(spriteP, kSW_CheckRight, &ravenP->collisionInset, false, false, true))
                d->horizMoveDelta = -d->horizMoveDelta;
        }
        else if (CheckForWall(spriteP, kSW_CheckLeft, &ravenP->collisionInset, false, false, true)) {
            d->horizMoveDelta = -d->horizMoveDelta;
        }
    }
    BounceOffRoomSides(spriteP, &gSpriteWorldP->workAreaRect);
    SWSetSpriteFlipX(spriteP, SWGetSpriteHorizMoveDelta(spriteP) > 0);
    RemoveSpriteIfOutsideRoom(spriteP);
}

void SnakeMoveProc(SpritePtr spriteP)
{
    CrawlerSpritePtr snakeP = CRAWLER(spriteP);
    DrawDataPtr d = spriteP->drawData;
    short push = 0;

    HandleSpriteWithMovingPlatforms(spriteP, snakeP->collisionInset);
    ApplyEnemyGravity(spriteP, 0.325f, 2.0f, 8.0f);
    SWOffsetSprite(spriteP, 0, d->vertMoveDelta);

    if (d->vertMoveDelta > 0) {
        push = (short)ConveyorPush(spriteP, &snakeP->collisionInset);
        if (CheckForWall(spriteP, kSW_CheckBottom, &snakeP->collisionInset, false, false, true))
            d->vertMoveDelta = 0;
    }
    SWOffsetSprite(spriteP, push + d->horizMoveDelta, 0);
    TurnAtWalls(spriteP, &snakeP->collisionInset, push);
    RemoveSpriteIfOutsideRoom(spriteP);
    CheckSpriteWithLavaTiles(spriteP, true);
}

/// Medusa heads fly straight at the hero, dripping goo.
void MedusasHeadMoveProc(SpritePtr spriteP)
{
    MedusaSpritePtr medusaP = MEDUSA(spriteP);
    DrawDataPtr d = spriteP->drawData;
    SpritePtr heroP;
    short distance = 200, dh, dv, dripChance;
    double angle;
    float midH, midV;

    midH = SWGetSpriteHorizLoc(spriteP) + SWGetSpriteWidth(spriteP) / 2.0f;
    midV = SWGetSpriteVertLoc(spriteP) + SWGetSpriteHeight(spriteP) / 2.0f;
    heroP = GetHeroSprite();
    if (heroP != NULL) {
        dh = (short)(SWGetSpriteWidth(heroP) / 2.0f + SWGetSpriteHorizLoc(heroP)) - (short)midH;
        dv = (short)(SWGetSpriteHeight(heroP) / 2.0f + SWGetSpriteVertLoc(heroP)) - (short)midV;
        distance = (short)sqrt((double)(dh * dh + dv * dv));
        angle = GetAngleFromXandY(dh, dv);
        d->horizMoveDelta = (float)GetXFromAngleAndVelocity(angle, medusaP->speed);
        d->vertMoveDelta = (float)GetYFromAngleAndVelocity(angle, medusaP->speed);
        SWOffsetSprite(spriteP, gCurrentWindDelta + d->horizMoveDelta, d->vertMoveDelta);
        if (!medusaP->ignoresWalls) {
            if (d->horizMoveDelta + gCurrentWindDelta > 0)
                CheckForWall(spriteP, kSW_CheckRight, &medusaP->collisionInset, false, false, true);
            else
                CheckForWall(spriteP, kSW_CheckLeft, &medusaP->collisionInset, false, false, true);
            if (d->vertMoveDelta > 0)
                CheckForWall(spriteP, kSW_CheckBottom, &medusaP->collisionInset, false, false, true);
            else
                CheckForWall(spriteP, kSW_CheckTop, &medusaP->collisionInset, false, false, true);
        }
    }
    SWSetSpriteFlipX(spriteP, SWGetSpriteHorizMoveDelta(spriteP) > 0);

        // hiss now and then, more often when close
    if (medusaP->soundTimer++ > 89 && GetRandom(1, distance + 100) == 1 &&
        FindSound(11) == -1 && FindSound(12) == -1 && FindSound(9) == -1) {
        MMPlayStereoSound(spriteP, GetRandom(11, 12), 11, 1);
        medusaP->soundTimer = 0;
    }

    dripChance = distance < 26 ? 2 : distance / 13;
    MakeSpriteDripBloodOrGoo(spriteP, dripChance, true, kGooDripSprite);
    RemoveSpriteIfOutsideRoom(spriteP);
}

void MedusaOnFireMoveProc(SpritePtr spriteP)
{
    MedusaSpritePtr medusaP = MEDUSA(spriteP);
    DrawDataPtr d = spriteP->drawData;

    SWSetSpriteFrameRange(spriteP, 3, 5);
    medusaP->fallAccel += 0.001f;
    medusaP->fallSpeed += medusaP->fallAccel;
    if (d->horizMoveDelta < 0)
        d->horizMoveDelta += medusaP->fallAccel / -4.0f;
    else
        d->horizMoveDelta += medusaP->fallAccel / 4.0f;
    SWOffsetSprite(spriteP, gCurrentWindDelta + d->horizMoveDelta, medusaP->fallSpeed);
    RemoveSpriteIfOutsideRoom(spriteP);
}

/// Small spiders drop on a thread, bounce a little, and climb back up.
void SmallSpiderMoveProc(SpritePtr spriteP)
{
    SpiderSpritePtr spiderP = SPIDER(spriteP);
    DrawDataPtr d = spriteP->drawData;
    SWRect noInset = {0, 0, 0, 0};
    float quarter, minSpeed, maxSpeed;

    if (spiderP->topVertLoc == -1)
        spiderP->topVertLoc = (short)SWGetSpriteVertLoc(spriteP);

    switch (spiderP->state) {
        case kSpiderDropping:
            d->vertMoveDelta += spiderP->dropAccel;
            SWOffsetSprite(spriteP, 0, d->vertMoveDelta);
            if (spiderP->dropSpeed < d->vertMoveDelta) {
                spiderP->peakDropSpeed = d->vertMoveDelta;
                spiderP->state = kSpiderSlowing;
            }
            if (CheckForWall(spriteP, kSW_CheckBottom, &spiderP->collisionInset, false, false, true) ||
                gRoomMoveBoundsRect.bottom <= spriteP->destFrameRect.bottom)
                spiderP->state = kSpiderClimbing;
                // don't drop into lava
            SWOffsetSprite(spriteP, 0, 32.0f);
            if (SWCheckSpriteWithTiles(gSpriteWorldP, spriteP, kSW_CheckBottom, &noInset, 6, 6,
                                       kFirstLavaTile, kLastLavaTile, false))
                spiderP->state = kSpiderClimbing;
            SWOffsetSprite(spriteP, 0, -32.0f);
            break;

        case kSpiderSlowing:
            d->vertMoveDelta -= spiderP->dropAccel;
            SWOffsetSprite(spriteP, 0, d->vertMoveDelta);
            if (d->vertMoveDelta < spiderP->bounce * -spiderP->peakDropSpeed) {
                    // bounce on the thread, then drop again a different distance
                spiderP->state = kSpiderDropping;
                quarter = (spiderP->maxDropSpeed - spiderP->minDropSpeed) / 4.0f;
                minSpeed = spiderP->minDropSpeed;
                maxSpeed = spiderP->maxDropSpeed;
                if (spiderP->dropSpeed < spiderP->minDropSpeed + quarter)
                    minSpeed += quarter;
                else
                    maxSpeed -= quarter;
                spiderP->dropSpeed = GetMovieCompatibleRandomFloat(minSpeed, maxSpeed);
            }
            if (CheckForWall(spriteP, kSW_CheckBottom, &spiderP->collisionInset, false, false, true))
                spiderP->state = kSpiderClimbing;
            break;

        case kSpiderClimbing:
            if (d->vertMoveDelta >= -spiderP->climbSpeed)
                d->vertMoveDelta += spiderP->dropAccel * -3.0f;
            if (d->vertMoveDelta <= -spiderP->climbSpeed)
                d->vertMoveDelta = -spiderP->climbSpeed;
            SWOffsetSprite(spriteP, 0, d->vertMoveDelta);
            if (SWGetSpriteVertLoc(spriteP) <= spiderP->topVertLoc) {
                SWOffsetSprite(spriteP, 0, SWGetSpriteVertLoc(spriteP) - spiderP->topVertLoc);
                spiderP->state = kSpiderWaiting;
                spiderP->waitTimer = spiderP->waitTime;
            }
            break;

        case kSpiderWaiting:
            if (spiderP->waitTimer-- < 1) {
                spiderP->state = kSpiderDropping;
                SWSetSpriteMoveDelta(spriteP, 0, 0);
            }
            break;
    }

    LayWebForSpider(spriteP, 8, 4, kSmallSpiderWebTile);
    RemoveSpriteIfOutsideRoom(spriteP);
}

/// Big spiders wait at the top of their thread and drop on whatever walks below:
/// the hero, ravens, Medusa heads or eyeballs.
void BigSpiderMoveProc(SpritePtr spriteP)
{
    SpiderSpritePtr spiderP = SPIDER(spriteP);
    SpritePtr heroP, preyP;
    Boolean found, foundRaven = false;

    if (spiderP->topVertLoc == -1)
        spiderP->topVertLoc = (short)SWGetSpriteVertLoc(spriteP);

    if (spiderP->state == kSpiderDropping) {
        SWOffsetSprite(spriteP, 0, spiderP->isFast ? 2.8f : 1.8f);
        if (CheckForWall(spriteP, kSW_CheckBottom, &spiderP->collisionInset, false, false, true) ||
            MyCheckSpriteWithWallTiles(gSpriteWorldP, spriteP, kSW_CheckBottom, &spiderP->collisionInset, 6, 6,
                                       kFirstLavaTile, kLastLavaTile, true) ||
            gRoomMoveBoundsRect.bottom <= spriteP->destFrameRect.bottom) {
            spiderP->state = kSpiderClimbing;
            spiderP->frameTimer = 10;
            spiderP->frameDelay = 10;
        }
    }
    else if (spiderP->state == kSpiderClimbing) {
        SWOffsetSprite(spriteP, 0, spiderP->preyP == NULL ? -0.6f : -1.4f);
        if (SWGetSpriteVertLoc(spriteP) <= spiderP->topVertLoc) {
            SWOffsetSprite(spriteP, 0, SWGetSpriteVertLoc(spriteP) - spiderP->topVertLoc);
            spiderP->state = kSpiderWaiting;
            spiderP->waitTimer = 60;
        }
    }
    else if (spiderP->state == kSpiderWaiting) {
        if (spiderP->waitTimer-- < 1) {
            spiderP->state = kSpiderWatching;
            spiderP->frameTimer = 10;
            spiderP->frameDelay = 10;
        }
        if (spiderP->preyP != NULL) {
            MMPlayStereoSound2(NULL, 0x32, 11, 1, gSoundVolume, 1.0, -1);
            SWSetSpriteMoveProc(spriteP, BigSpiderEatsPreyMoveProc);
            if (spiderP->preyP->userData == kWebTrappedHeroSprite)
                SWSetSpriteMoveProc(spiderP->preyP, CaughtHeroMoveProc);
            spiderP->eatTimer = 0;
        }
    }
    else if (spiderP->state == kSpiderWatching) {
        found = false;
        heroP = GetHeroSprite();
        if (heroP != NULL)
            found = CheckBigSpiderWithSprite(spriteP, heroP, 50);
        for (preyP = SWGetNextSprite(gBirdSpriteLayerP, NULL); preyP != NULL && !found;
             preyP = SWGetNextSprite(gBirdSpriteLayerP, preyP)) {
            SW_ASSERT(preyP->userData == kBlueRavenSprite || preyP->userData == kRedRavenSprite);
            found = CheckBigSpiderWithSprite(spriteP, preyP, 90);
            if (found)
                foundRaven = true;
        }
        for (preyP = SWGetNextSprite(gMedusaSpriteLayerP, NULL); preyP != NULL && !found;
             preyP = SWGetNextSprite(gMedusaSpriteLayerP, preyP))
            found = CheckBigSpiderWithSprite(spriteP, preyP, 20);
        for (preyP = SWGetNextSprite(gEyeballSpriteLayerP, NULL); preyP != NULL && !found;
             preyP = SWGetNextSprite(gEyeballSpriteLayerP, preyP))
            found = CheckBigSpiderWithSprite(spriteP, preyP, 75);
        if (found) {
            spiderP->isFast = foundRaven;
            spiderP->state = kSpiderDropping;
            spiderP->frameTimer = 6;
            spiderP->frameDelay = 6;
        }
    }

    LayWebForSpider(spriteP, 24, 10, kBigSpiderWebTile);
    if (spiderP->preyP != NULL)
        MovePreyToSpiderLocation(spiderP->preyP, spriteP);
    RemoveSpriteIfOutsideRoom(spriteP);
}

void SkeletonFishMoveProc(SpritePtr spriteP)
{
    FishSpritePtr fishP = FISH(spriteP);
    DrawDataPtr d = spriteP->drawData;

    if (fishP->waitTimer >= 1) {
        fishP->waitTimer--;
        if (fishP->waitTimer == 0)
            StartSkeletonFishJump(spriteP);
        return;
    }

    d->vertMoveDelta += fishP->gravity;
    SWOffsetSprite(spriteP, 0, d->vertMoveDelta);

        // near the top of the jump: turn to dive back down
    if (d->vertMoveDelta < 0 && !fishP->hasTurned && fabsf(d->vertMoveDelta) / fishP->gravity < 17.0f) {
        SWSetSpriteFrameRange(spriteP, fishP->firstFrame + 2, fishP->firstFrame + 7);
        SWSetCurrentFrameIndex(spriteP, fishP->firstFrame + 2);
        SWSetSpriteFrameProc(spriteP, FishTurnAroundFrameProc);
        SWSetSpriteFrameAdvance(spriteP, 0);
        fishP->frameDirection = 1;
        fishP->frameTimer = 4;
        fishP->frameDelay = 4;
        fishP->hasTurned = true;
    }

        // back in the lava
    if (fishP->restVertLoc <= SWGetSpriteVertLoc(spriteP)) {
        SWMoveSprite(spriteP, (short)SWGetSpriteHorizLoc(spriteP), fishP->restVertLoc);
        SWSetSpriteFrameProc(spriteP, NULL);
        SWSetSpriteFrameRange(spriteP, 0, 1);
        fishP->waitTimer = fishP->waitTime;
        fishP->hasTurned = false;
        MMSetTempStereoFadeDist(300);
        MMPlayStereoSoundAdjustedVol(spriteP, 0x41, 11, 1, 350, 0.6);
    }
}

void SkeletonWaitingToUncollapseMoveProc(SpritePtr spriteP)
{
    SkeletonSpritePtr skeletonP = SKELETON(spriteP);
    SpritePtr heroP;

    if (skeletonP->wakeMode == kSkeletonWakeAfterDelay) {
        if (skeletonP->wakeTimer-- < 1)
            MakeSkeletonDoUncollapse(spriteP);
    }
    else if (skeletonP->wakeMode == kSkeletonWakeWhenNear) {
        heroP = GetHeroSprite();
        if (heroP != NULL && GetDistBetweenSprites(spriteP, heroP) < skeletonP->wakeDistance)
            MakeSkeletonDoUncollapse(spriteP);
    }
    else if (skeletonP->wakeMode == kSkeletonWakeDelayWhenNear) {
        heroP = GetHeroSprite();
        if (heroP != NULL && GetDistBetweenSprites(spriteP, heroP) < skeletonP->wakeDistance)
            skeletonP->wakeMode = kSkeletonWakeAfterDelay;
    }
}

/// Walking skeletons follow the hero.
void WalkingSkeletonMoveProc(SpritePtr spriteP)
{
    SkeletonSpritePtr skeletonP = SKELETON(spriteP);
    DrawDataPtr d = spriteP->drawData;
    SpritePtr heroP;
    short push = 0, mid, heroMid;

    HandleSpriteWithMovingPlatforms(spriteP, skeletonP->collisionInset);
    ApplyEnemyGravity(spriteP, 0.43333334f, 1.0f, 8.0f);
    SWOffsetSprite(spriteP, 0, d->vertMoveDelta);
    if (d->vertMoveDelta > 0) {
        push = (short)ConveyorPush(spriteP, &skeletonP->collisionInset);
        if (CheckForWall(spriteP, kSW_CheckBottom, &skeletonP->collisionInset, false, false, true))
            d->vertMoveDelta = 0;
    }

    heroP = GetHeroSprite();
    if (heroP == NULL) {
        d->horizMoveDelta = 0;
    }
    else {
        mid = (short)(SWGetSpriteWidth(spriteP) / 2.0f + SWGetSpriteHorizLoc(spriteP));
        heroMid = (short)(SWGetSpriteWidth(heroP) / 2.0f + SWGetSpriteHorizLoc(heroP));
        if (heroMid + skeletonP->speed < mid) {
            d->horizMoveDelta = -skeletonP->speed;
            skeletonP->facingRight = false;
        }
        else if (mid < heroMid - skeletonP->speed) {
            d->horizMoveDelta = skeletonP->speed;
            skeletonP->facingRight = true;
        }
        else {
            d->horizMoveDelta = 0;
        }
    }

    SWOffsetSprite(spriteP, push + d->horizMoveDelta, 0);
    if (d->horizMoveDelta + push > 0)
        CheckForWall(spriteP, kSW_CheckRight, &skeletonP->collisionInset, false, false, true);
    else if (d->horizMoveDelta + push < 0)
        CheckForWall(spriteP, kSW_CheckLeft, &skeletonP->collisionInset, false, false, true);

    if (d->horizMoveDelta != 0) {
        SWSetSpriteFrameRange(spriteP, 10, 13);
        SWSetSpriteFlipX(spriteP, d->horizMoveDelta > 0);
    }
    else {
        SWSetSpriteFrameRange(spriteP, 8, 8);
        SWSetSpriteFlipX(spriteP, skeletonP->facingRight);
    }
    RemoveSpriteIfOutsideRoom(spriteP);
    CheckSpriteWithLavaTiles(spriteP, true);
}

/// Carries the sprite along with a moving platform under it. Returns true if it
/// is on one.
Boolean HandleSpriteWithMovingPlatforms(SpritePtr spriteP, SWRect collisionInset)
{
    SpritePtr platformP;

    collisionInset.bottom--;
    if (!SWCheckSpriteWithSprites(spriteP, &platformP, kSW_CheckBottom, &collisionInset,
                                  gVertMovingPlatformSpriteLayerP, gHorizMovingPlatformSpriteLayerP,
                                  true, true, false, false))
        return false;

    if (!platformP->isVisible)
        SWOffsetSprite(spriteP, PLATFORM(platformP)->moveDeltaH, PLATFORM(platformP)->moveDeltaV);
    else
        SWOffsetSprite(spriteP, platformP->destFrameRect.left - platformP->oldFrameRect.left,
                       platformP->destFrameRect.top - platformP->oldFrameRect.top);
    return true;
}

/// Objects knocked loose (shields, rocks) fall and slide.
void FallingObjectMoveProc(SpritePtr spriteP)
{
    ObjectSpritePtr objectP = OBJECT(spriteP);
    DrawDataPtr d = spriteP->drawData;
    float push = 0;

    HandleSpriteWithMovingPlatforms(spriteP, objectP->collisionInset);
    ApplyEnemyGravity(spriteP, 0.43333334f, 1.0f, 8.0f);
    SWOffsetSprite(spriteP, 0, d->vertMoveDelta);
    if (d->vertMoveDelta > 0) {
        push = ConveyorPush(spriteP, &objectP->collisionInset);
        if (CheckForWall(spriteP, kSW_CheckBottom, &objectP->collisionInset, false, false, true)) {
            if ((spriteP->userData == 0x1e || spriteP->userData == 0x1f) && d->vertMoveDelta >= 3.9f)
                MMPlayStereoSound(spriteP, 0x1a, 12, 1);
            d->vertMoveDelta = 0;
        }
    }
    SWOffsetSprite(spriteP, d->horizMoveDelta + push, 0);
    TurnAtWalls(spriteP, &objectP->collisionInset, push);
    RemoveSpriteIfOutsideRoom(spriteP);
    CheckSpriteWithLavaTiles(spriteP, false);
}

void ZzzzSpriteMoveProc(SpritePtr spriteP)
{
    SWOffsetSprite(spriteP, spriteP->drawData->horizMoveDelta, spriteP->drawData->vertMoveDelta);
}

/// A small spider caught the hero: it climbs out of the room with him.
void SmallSpiderDragsHeroMoveProc(SpritePtr spriteP)
{
    SpiderSpritePtr spiderP = SPIDER(spriteP);
    DrawDataPtr d = spriteP->drawData;
    SpritePtr preyP = spiderP->preyP;

    SW_ASSERT(preyP != NULL);
    if (d->vertMoveDelta >= -spiderP->climbSpeed)
        d->vertMoveDelta -= spiderP->dropAccel;
    d->vertMoveDelta -= 0.017f;
    SWOffsetSprite(spriteP, 0, d->vertMoveDelta);
    if (SWGetSpriteVertLoc(preyP) + SWGetSpriteHeight(preyP) < -30.0f) {
        SWSetSpriteMoveProc(spriteP, SmallSpiderEatJackMoveProc);
        spiderP->eatCounter = 0;
    }
    SWCenterSpriteOverSprite(preyP, spriteP);
    SWOffsetSprite(preyP, 0, 40.0f);
    if (d->vertMoveDelta > 0)
        LayWebForSpider(spriteP, 8, 4, kSmallSpiderWebTile);
    EraseWebForSpider(spriteP, 8, 4, kSmallSpiderWebTile);
}

void SmallSpiderEatJackMoveProc(SpritePtr spriteP)
{
    SpiderSpritePtr spiderP = SPIDER(spriteP);
    SpritePtr preyP = spiderP->preyP;

    SW_ASSERT(preyP != NULL);
    MakeSpriteDripBloodOrGoo(preyP, spiderP->eatCounter < 18 ? 2 : (short)(spiderP->eatCounter / 9), true,
                             kBloodDripSprite);
    spiderP->eatCounter++;
    if (spiderP->eatCounter == 90)
        gRestartLevel = true;
    if (spiderP->eatCounter > 159) {
        SWRemoveSpriteFromAnimation(spriteP, true);
        SWRemoveSpriteFromAnimation(preyP, true);
    }
}

void BigSpiderEatsPreyMoveProc(SpritePtr spriteP)
{
    SpiderSpritePtr spiderP = SPIDER(spriteP);

    spiderP->eatTimer++;
    if (spiderP->eatTimer == 160 && spiderP->preyP->userData == kWebTrappedHeroSprite)
        gRestartLevel = true;
    if (spiderP->eatTimer > 239) {
        StopMySound(0x32);
        SWSetSpriteMoveProc(spriteP, NULL);
        spiderP->frameTimer = 15;
        spiderP->frameDelay = 15;
    }
}

/// Enemy shadows are mounted sprites, offset away from the hero (the light).
void ShadowSpriteMoveProc(SpritePtr spriteP)
{
    SpritePtr parentP = spriteP->mountParentP, heroP = GetHeroSprite();
    float dh, dist;

    if (parentP != NULL && heroP != NULL) {
        dh = SWGetSpriteHorizLoc(parentP) - SWGetSpriteHorizLoc(heroP);
        dist = floatAbs(dh);
        if (dist < 90.0f)
            dist = 90.0f;
        SWSetSpriteMountOffset(spriteP, dh / 20.0f,
                               (SWGetSpriteVertLoc(parentP) - SWGetSpriteVertLoc(heroP)) / (dist / 8.0f));
    }
}

float floatAbs(float n)
{
    return n > 0 ? n : -n;
}

/// Pulses the arrow button's color (misc1 is the direction, misc2 a delay counter).
void ArrowButtonColorMoveProc(SpritePtr spriteP)
{
    int level = spriteP->drawData->colorBlue;

    if (spriteP->misc2.n++ > 90 || level != 0xffff) {
        spriteP->misc2.n = 0;
        if (spriteP->misc1.n < 0) {
            level += 0x8ff;
            if (level > 0xffff) {
                level = 0xffff;
                spriteP->misc1.n = 1;
            }
        }
        else {
            level -= 0x8ff;
            if (level < 0x4000) {
                level = 0x4000;
                spriteP->misc1.n = -1;
            }
        }
        if (spriteP->curFrameIndex == 0)
            SWSetSpriteColor(spriteP, 0xffff, (unsigned short)level, (unsigned short)level);
    }
    if (spriteP->curFrameIndex > 0)
        SWSetSpriteColor(spriteP, 0xffff, 0xffff, 0xffff);
}

void StartSkeletonFishJump(SpritePtr spriteP)
{
    FishSpritePtr fishP = FISH(spriteP);

    fishP->firstFrame = GetRandom(0, 100) < 51 ? 0 : 9;
    SWSetSpriteMoveDelta(spriteP, 0, -fishP->jumpSpeed);
    SWSetSpriteFrameRange(spriteP, fishP->firstFrame, fishP->firstFrame + 1);
    SWSetCurrentFrameIndex(spriteP, 0);
    fishP->waitTimer = 0;
    SWSetSpriteFrameProc(spriteP, SkeletonFishFrameProc);
    SWSetSpriteFrameAdvance(spriteP, 0);
    fishP->frameDirection = 1;
    fishP->frameTimer = 6;
    fishP->frameDelay = 6;
    MMSetTempStereoFadeDist(300);
    MMPlayStereoSoundAdjustedVol(spriteP, 0x41, 11, 1, 350, 1.0);
}

void BloodDripMoveProc(SpritePtr spriteP)
{
    DrawDataPtr d = spriteP->drawData;
    SpritePtr splashP;
    short sound;

    SW_ASSERT(spriteP->userData == kBloodDripSprite || spriteP->userData == kGooDripSprite);
    d->vertMoveDelta += 0.1f;
    if (d->vertMoveDelta >= 4.0f)
        d->vertMoveDelta = 4.0f;
    SWOffsetSprite(spriteP, 0, d->vertMoveDelta);

    if (CheckForWall(spriteP, kSW_CheckBottom, &DRIP(spriteP)->collisionInset, false, false, true)) {
        SWRemoveSpriteFromAnimation(spriteP, true);
        splashP = NewBloodOrGooSplashSprite(spriteP->userData == kBloodDripSprite ? 0xbd : 0xbf);
        SWCenterSpriteOverSprite(splashP, spriteP);
        SWOffsetSprite(splashP, 0, -GetRandom(2, 6));
        if (DRIP(spriteP)->playsSplashSound) {
            sound = GetRandom(14, 15);
            if (CountSounds(14) + CountSounds(15) < 3)
                MMPlayStereoSound(spriteP, sound, 11, 1);
        }
        return;
    }
    RemoveSpriteIfOutsideRoom(spriteP);
}

/// The hero hanging in a big spider's web bleeds, more and more.
void CaughtHeroMoveProc(SpritePtr spriteP)
{
    WebTrappedSpritePtr trappedP = WEBTRAPPED(spriteP);

    SW_ASSERT(spriteP->userData == kWebTrappedHeroSprite);
    MakeSpriteDripBloodOrGoo(spriteP, trappedP->dripChance, false, kBloodDripSprite);
    if (trappedP->dripTimer++ > 5) {
        trappedP->dripChance++;
        trappedP->dripTimer = 0;
        if (trappedP->dripChance > 69)
            SWSetSpriteMoveProc(spriteP, NULL);
    }
}

/// When the circles of light are gone, the statue turns back into Jack.
void ColorStatueMoveProc(SpritePtr spriteP)
{
    StatueSpritePtr statueP = STATUE(spriteP);

    SW_ASSERT(spriteP->userData == kStatueSprite);
    if (!gStatuesAreDoneFading)
        return;
    if (SWGetNextSprite(gTopCircleOfLightSpriteLayerP, NULL) != NULL ||
        SWGetNextSprite(gBottomCircleOfLightSpriteLayerP, NULL) != NULL)
        return;

    SWSetSpriteMoveProc(spriteP, NULL);
    SWSetSpriteFrameProc(spriteP, PauseThenUnfreezeFrameProc);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameTime(spriteP, 0);
    statueP->pauseTimer = 30;
    SW_ASSERT(statueP->pedestalP != NULL);
    SWSetCurrentFrameIndex(statueP->pedestalP, 1);
}

/// While the statue fades, rings of light rise around it: one ring in front of
/// the statue and one behind, every 20 frames.
void CreateCirclesOfLightMoveProc(SpritePtr spriteP)
{
    StatueSpritePtr statueP = STATUE(spriteP);
    SpritePtr frontP, backP;
    short left, vertLoc, height;

    SW_ASSERT(spriteP->userData == kStatueSprite);
    if (gStatuesAreDoneFading) {
        SWSetSpriteMoveProc(spriteP, NULL);
        return;
    }
    if (statueP->circleTimer-- >= 1)
        return;

    left = spriteP->destFrameRect.left;
    vertLoc = spriteP->destFrameRect.bottom - 5;
    frontP = NewCircleOfLightSprite(0xc6);
    SWSetSpriteLocation(frontP, left, vertLoc);
    SWSetCurrentFrameIndex(frontP, statueP->circleFrame);
    SWSetSpriteTranslucencyLevel(frontP, 0);
    CIRCLE(frontP)->startVertLoc = vertLoc;
    SWSetSpriteBlendMode(frontP, 0x302, 1);
    height = (short)SWGetSpriteHeight(frontP);

    backP = NewCircleOfLightSprite(kBottomCircleOfLightSprite);
    SWSetSpriteLocation(backP, left, vertLoc - height);
    SWSetCurrentFrameIndex(backP, statueP->circleFrame);
    SWSetSpriteTranslucencyLevel(backP, 0);
    CIRCLE(backP)->startVertLoc = vertLoc - height;
    SWSetSpriteBlendMode(backP, 0x302, 1);

    CIRCLE(frontP)->partnerP = backP;
    CIRCLE(backP)->partnerP = frontP;
    CIRCLE(frontP)->statueP = spriteP;
    CIRCLE(backP)->statueP = spriteP;
    CIRCLE(backP)->riseSpeed = 0.2f;
    CIRCLE(frontP)->riseSpeed = 0.2f;

    statueP->circleFrame++;
    if (backP->maxFrames <= statueP->circleFrame)
        statueP->circleFrame = 0;
    statueP->circleTimer = 20;
}

/// Rings fade in as they rise to the statue's middle and out above it.
void CircleOfLightMoveProc(SpritePtr spriteP)
{
    CircleOfLightSpritePtr circleP = CIRCLE(spriteP);
    short height, statueHeight, y;
    unsigned long level;

    if (spriteP->userData == 0xc6) {
        height = (short)SWGetSpriteHeight(spriteP);
        y = (short)SWGetSpriteVertLoc(spriteP);
        statueHeight = (short)SWGetSpriteHeight(circleP->statueP);
        if (y < (short)SWGetSpriteVertLoc(circleP->statueP) + statueHeight / 2)
            y -= circleP->startVertLoc - statueHeight;
        else
            y = circleP->startVertLoc - y;

        if (y <= height) {
            if (y < 0) {
                SW_ASSERT(circleP->partnerP != NULL);
                SWRemoveSpriteFromAnimation(spriteP, true);
                SWRemoveSpriteFromAnimation(circleP->partnerP, true);
                return;
            }
            level = y * (0xffff / height);
            SWSetSpriteTranslucencyLevel(spriteP, level);
            SWSetSpriteTranslucencyLevel(circleP->partnerP, level);
        }
    }
    circleP->riseSpeed += 0.01f;
    SWOffsetSprite(spriteP, 0, -circleP->riseSpeed);
}

/// Treasure sparkles now and then (more valuable treasure more often).
void CreateSparklesMoveProc(SpritePtr spriteP)
{
    SpritePtr sparkleP;
    short chance, tries;

    switch (spriteP->userData) {
        case 0xb1: chance = 25; break;
        case 0xb2: chance = 15; break;
        case 0xb3: chance = 9; break;
        case 0xb4: chance = 2; break;
        default:   chance = 1; break;
    }
    if (GetRandom(1, chance) != 1)
        return;

    sparkleP = NewGoldSparkleSprite();
    SWAddSprite(spriteP->parentSpriteLayerP, sparkleP);
    for (tries = 0; tries < 1000; tries++) {
        SWSetSpriteLocation(sparkleP,
                            GetRandom(spriteP->destFrameRect.left, spriteP->destFrameRect.right - 6),
                            GetRandom(spriteP->destFrameRect.top, spriteP->destFrameRect.bottom - 6));
        if (SWBestMaskCollision(sparkleP, spriteP))
            break;
    }
}

/// Big webs shake when the hero walks into them.
void BigWebMoveProc(SpritePtr spriteP)
{
    BackgroundSpritePtr webP = BACKGROUND(spriteP);
    SWRect inset;

    SW_ASSERT(spriteP->userData == 0x74);
    SWSetRect(&inset, 10, 0, 10, 0);
    if (SWGetSpriteFrameTime(spriteP) != -1)
        return;
    if (!SWCheckSpriteWithSprites(spriteP, NULL, kSW_CheckAll, &inset, gHeroSpriteLayerP, gHeroSpriteLayerP,
                                  false, false, false, false)) {
        webP->isArmed = true;
    }
    else if (webP->isArmed) {
        SWSetSpriteFrameProc(spriteP, BigWebFrameProc);
        SWSetSpriteFrameTime(spriteP, 25);
        SWSetSpriteFrameAdvance(spriteP, 1);
        webP->isArmed = false;
    }
}

/// A used save pedestal works again once the hero has stepped off it.
void PedestalMoveProc(SpritePtr spriteP)
{
    SWRect inset;

    SW_ASSERT(spriteP->userData == kSavePedestalSprite);
    if (!PEDESTAL(spriteP)->isActive) {
        SWSetRect(&inset, 0, 0, 0, 0);
        if (!SWCheckSpriteWithSprites(spriteP, NULL, kSW_CheckAll, &inset, gHeroSpriteLayerP, gHeroSpriteLayerP,
                                      false, false, false, false))
            PEDESTAL(spriteP)->isActive = true;
    }
}

/// Tips pop up over their trigger (misc2) and disappear when the hero walks away.
void TipSpriteMoveProc(SpritePtr spriteP)
{
    SpritePtr heroP = GetHeroSprite(), triggerP = spriteP->misc2.sprite;
    SWRect rect;
    short dh, dv, bottom, right, height, width;

    SW_ASSERT(triggerP != NULL);
    if (heroP == NULL)
        return;

    dh = mAbs((int)(SWGetSpriteHorizLoc(heroP) - SWGetSpriteHorizLoc(triggerP)));
    dv = mAbs((int)(SWGetSpriteVertLoc(heroP) - SWGetSpriteVertLoc(triggerP)));
    if (sqrt((double)(dh * dh + dv * dv)) > 60.0) {
        SWRemoveSpriteFromAnimation(spriteP, true);
        return;
    }

    if (spriteP->misc1.n == 0) {
            // first frame: place it above the trigger, inside the room
        rect = spriteP->destFrameRect;
        SWCenterRect(&rect, &triggerP->destFrameRect);
        height = spriteP->destFrameRect.bottom - spriteP->destFrameRect.top;
        width = spriteP->destFrameRect.right - spriteP->destFrameRect.left;
        SWMoveSprite(spriteP, rect.left, (rect.top - height) + SWGetSpriteHeight(triggerP) / -2.0f);
        bottom = (short)(height + SWGetSpriteVertLoc(spriteP));
        right = (short)(width + SWGetSpriteHorizLoc(spriteP));
        if (SWGetSpriteVertLoc(spriteP) < 0)
            SWMoveSprite(spriteP, rect.left, SWGetSpriteVertLoc(triggerP) + SWGetSpriteHeight(triggerP) + 5.0f);
        if (gSpriteWorldP->workAreaRect.bottom < bottom)
            SWMoveSprite(spriteP, rect.left, (SWGetSpriteVertLoc(triggerP) - height) - 5.0f);
        if (SWGetSpriteHorizLoc(spriteP) < 0)
            SWOffsetSprite(spriteP, 15.0f - SWGetSpriteHorizLoc(spriteP), 0);
        else if (gSpriteWorldP->workAreaRect.right <= right)
            SWOffsetSprite(spriteP, (gSpriteWorldP->workAreaRect.right - right) - 20, 0);
        SWSetSpriteTranslucency(spriteP, 0xffff);
        spriteP->misc1.n = 1;
    }
}

/// The "start here" arrow on the map bobs up and down around miscFloat.
void StartHereBounceMoveProc(SpritePtr spriteP)
{
    spriteP->misc1.n += 5;
    if (spriteP->misc1.n > 360)
        spriteP->misc1.n -= 360;
    SWMoveSprite(spriteP, SWGetSpriteHorizLoc(spriteP), spriteP->miscFloat + (float)SWCos(spriteP->misc1.n));
}

// ---------------------------------------------------------------------------
// Platforms and doors

static Boolean PlatformHitSomething(SpritePtr spriteP, SWTileSearchType direction)
{
    PlatformSpritePtr platformP = PLATFORM(spriteP);
    Boolean wall, vert, horiz, blocker;

    wall = CheckForWall(spriteP, direction, &platformP->collisionInset, false, false, true);
    vert = SWCheckSpriteWithSprites(spriteP, NULL, direction, &platformP->collisionInset,
                                    gVertMovingPlatformSpriteLayerP, gVertMovingPlatformSpriteLayerP,
                                    false, true, false, true);
    horiz = SWCheckSpriteWithSprites(spriteP, NULL, direction, &platformP->collisionInset,
                                     gHorizMovingPlatformSpriteLayerP, gHorizMovingPlatformSpriteLayerP,
                                     false, true, false, true);
    blocker = SWCheckSpriteWithSprites(spriteP, NULL, direction, &platformP->collisionInset,
                                       gMovingPlatformBlockerSpriteLayerP, gMovingPlatformBlockerSpriteLayerP,
                                       false, true, false, true);
    return wall || vert || horiz || blocker;
}

void MovingPlatformHorizMoveProc(SpritePtr spriteP)
{
    PlatformSpritePtr platformP = PLATFORM(spriteP);
    DrawDataPtr d = spriteP->drawData;

    if (!platformP->isMoving)
        return;
    SWOffsetSprite(spriteP, d->horizMoveDelta, 0);
    platformP->moveDeltaH = d->horizMoveDelta;
    platformP->moveDeltaV = 0;
    if (PlatformHitSomething(spriteP, d->horizMoveDelta > 0 ? kSW_CheckRight : kSW_CheckLeft))
        d->horizMoveDelta = -d->horizMoveDelta;
    BounceOffRoomSides(spriteP, &gSpriteWorldP->scrollRectMoveBounds);
    RemoveSpriteIfOutsideRoom(spriteP);
}

void MovingPlatformVertMoveProc(SpritePtr spriteP)
{
    PlatformSpritePtr platformP = PLATFORM(spriteP);
    DrawDataPtr d = spriteP->drawData;
    SWRect *boundsP = &gSpriteWorldP->scrollRectMoveBounds;

    if (!platformP->isMoving)
        return;
    SWOffsetSprite(spriteP, 0, d->vertMoveDelta);
    platformP->moveDeltaH = 0;
    platformP->moveDeltaV = d->vertMoveDelta;
    if (PlatformHitSomething(spriteP, d->vertMoveDelta > 0 ? kSW_CheckBottom : kSW_CheckTop))
        d->vertMoveDelta = -d->vertMoveDelta;

    if (spriteP->destFrameRect.top + spriteP->curFrameP->collisionInset.top < boundsP->top && d->vertMoveDelta < 0)
        d->vertMoveDelta = -d->vertMoveDelta;
    else if (boundsP->bottom < spriteP->destFrameRect.bottom - spriteP->curFrameP->collisionInset.bottom &&
             d->vertMoveDelta > 0)
        d->vertMoveDelta = -d->vertMoveDelta;
    RemoveSpriteIfOutsideRoom(spriteP);
}

/// A falling platform waits fallDelay frames after being stepped on, then falls.
/// Ordinary ones come back after a while; skull platforms crumble away.
void FallingPlatformMoveProc(SpritePtr spriteP)
{
    PlatformSpritePtr platformP = PLATFORM(spriteP);
    SpritePtr heroP;

    if (platformP->fallDelay >= 1) {
        platformP->fallDelay--;
        return;
    }

    if (spriteP->parentSpriteLayerP != gFallingPlatformSpriteLayerP) {
        SWRemoveSprite(spriteP);
        SWAddSprite(gFallingPlatformSpriteLayerP, spriteP);
    }
    if (platformP->startVertLoc == -1) {
        platformP->startVertLoc = (short)SWGetSpriteVertLoc(spriteP);
        Rumble(kRumbleHeavy);           // the block gives way
    }
    spriteP->drawData->vertMoveDelta += 0.1f;
    SWOffsetSprite(spriteP, 0, spriteP->drawData->vertMoveDelta);

    if (spriteP->userData > 0x5b && spriteP->userData < 0x5f) {
        heroP = GetHeroSprite();
        if (heroP != NULL && HERO(heroP)->platformP != spriteP && spriteP->frameAdvance == 1) {
            SWSetSpriteFrameProc(spriteP, SkullFallingPlatformFrameProc);
            SWSetSpriteFrameAdvance(spriteP, -1);
            SWSetSpriteFrameTime(spriteP, 100);
        }
    }

    if (!SWIsSpriteInRect(spriteP, &gRoomMoveBoundsRect)) {
        if (spriteP->userData < 0x5c || spriteP->userData > 0x5e)
            MakeFallingPlatformReappearAfterDelay(spriteP);
        else
            SWRemoveSpriteFromAnimation(spriteP, true);
    }
    else {
        CheckSpriteWithLavaTiles(spriteP, true);
    }
}

void MakeFallingPlatformReappearAfterDelay(SpritePtr spriteP)
{
    SWOffsetSprite(spriteP, 0, gRoomMoveBoundsRect.bottom - SWGetSpriteVertLoc(spriteP));
    SWSetSpriteMoveProc(spriteP, FallingPlatformWaitMoveProc);
    SWSetSpriteVisible(spriteP, false);
    PLATFORM(spriteP)->reappearTimer = 135;
}

void FallingPlatformWaitMoveProc(SpritePtr spriteP)
{
    PlatformSpritePtr platformP = PLATFORM(spriteP);
    SWError err;

    spriteP->drawData->vertMoveDelta += 0.1f;
    SWOffsetSprite(spriteP, 0, spriteP->drawData->vertMoveDelta);
    if (platformP->reappearTimer-- >= 1)
        return;

        // fade back in where it started
    SWSetSpriteLocation(spriteP, (short)SWGetSpriteHorizLoc(spriteP), platformP->startVertLoc);
    SWRemoveSprite(spriteP);
    SWAddSprite(gVertMovingPlatformSpriteLayerP, spriteP);
    SWCustomSortSpriteLayer(gVertMovingPlatformSpriteLayerP, CompareSpriteLeft);
    SWSetSpriteMoveDelta(spriteP, 0, 0);
    SWSetSpriteMoveProc(spriteP, NULL);
    SWSetSpriteVisible(spriteP, true);
    SWSetSpriteFrameProc(spriteP, FadeInFrameProc);
    SWSetSpriteFrameTime(spriteP, 16);
    SWSetSpriteTranslucencyLevel(spriteP, 0);
    SWSetSpriteDrawProc(spriteP, gTranslucentMaskDrawProc);
}

/// Doors slide open with increasing speed.
void DoorSpriteMoveProc(SpritePtr spriteP)
{
    DoorSpritePtr doorP = DOOR(spriteP);
    short move;

    SW_ASSERT(spriteP->userData >= kFirstDoorSprite);
    SW_ASSERT(spriteP->userData <= kLastDoorSprite);

    doorP->moveFraction += doorP->moveSpeed;
    doorP->moveSpeed += doorP->moveAccel;
    doorP->moveAccel += 0.003;
    move = (short)doorP->moveFraction;
    doorP->moveFraction -= move;

    if (!doorP->opensUpward) {
        spriteP->destFrameRect.top += move;
        doorP->topInset += move;
    }
    else {
        spriteP->destFrameRect.bottom -= move;
        doorP->bottomInset -= move;
        SWSetFrameCollisionInset(spriteP->curFrameP, 0, 0, 0, doorP->bottomInset);
    }
    spriteP->needsToBeDrawn = true;
    doorP->openDistance -= move;

    if (doorP->openDistance < 1) {
        if (doorP->openDistance < 0) {
                // went too far: back up
            move = doorP->openDistance;
            if (!doorP->opensUpward) {
                spriteP->destFrameRect.top += move;
                doorP->topInset += move;
            }
            else {
                spriteP->destFrameRect.bottom -= move;
                doorP->bottomInset -= move;
            }
        }
        SWRemoveSprite(spriteP);
        SWAddSprite(gOpenedDoorSpriteLayerP, spriteP);
        SWSetSpriteMoveProc(spriteP, NULL);
        SWSetFrameCollisionInset(spriteP->curFrameP, 0, 0, 0, 0);
        doorP->isOpen = true;
    }
}

void CloseDoorSpriteMoveProc(SpritePtr spriteP)
{
    DoorSpritePtr doorP = DOOR(spriteP);
    short move;

    SW_ASSERT(spriteP->userData >= kFirstDoorSprite);
    SW_ASSERT(spriteP->userData <= kLastDoorSprite);

    doorP->moveFraction += doorP->moveSpeed;
    doorP->moveSpeed += doorP->moveAccel;
    doorP->moveAccel += 0.003;
    move = (short)doorP->moveFraction;
    doorP->moveFraction -= move;

    if (!doorP->opensUpward) {
        spriteP->destFrameRect.top -= move;
        doorP->topInset -= move;
    }
    else {
        spriteP->destFrameRect.bottom += move;
        doorP->bottomInset += move;
        SWSetFrameCollisionInset(spriteP->curFrameP, 0, 0, 0, -8 - doorP->openDistance);
    }
    spriteP->needsToBeDrawn = true;
    doorP->openDistance -= move;

    if (doorP->openDistance < 1) {
        if (doorP->openDistance < 0) {
            move = doorP->openDistance;
            if (!doorP->opensUpward) {
                spriteP->destFrameRect.top -= move;
                doorP->topInset -= move;
            }
            else {
                spriteP->destFrameRect.bottom += move;
                doorP->bottomInset += move;
            }
        }
        SWSetFrameCollisionInset(spriteP->curFrameP, 0, 0, 0, 0);
        SWSetSpriteMoveProc(spriteP, NULL);
        doorP->isOpen = false;
        InitializeDoorMovementVariables(spriteP);
    }
}

// ---------------------------------------------------------------------------
// Misc

/// The small digit displays: frame rate (up to 4 digits), room number (6) or
/// secrets found.
void SmallDigitsMoveProc(SpritePtr spriteP)
{
    StatsSpritePtr statsP = (StatsSpritePtr)spriteP;
    long number;

    if (statsP->numDigits < 5)
        number = gCurrentFPSValue;
    else if (statsP->numDigits < 7)
        number = GEGetLevelResID(gCurrentLevel, 1, 200);
    else
        number = gNumSecretsFound;
    SetStatsSpriteNumber(spriteP, number);
}

void CustomCursorMoveProc(SpritePtr spriteP)
{
    SWPoint mouse;

    MMGetMouse(&mouse);
    SWConvertCoordinates(gSpriteWorldP, &mouse);
    if (mouse.h == 0 && mouse.v == 0)
        SWMoveSprite(spriteP, 10000.0f, 10000.0f);
    else
        SWMoveSprite(spriteP, mouse.h - 4, mouse.v - 4);
}

void StdSpriteMoveProc(SpritePtr spriteP)
{
    SWOffsetSprite(spriteP, spriteP->drawData->horizMoveDelta, spriteP->drawData->vertMoveDelta);
}
