/*
 * Levers.c — levers and brick (floor) switches, and the things they do: doors,
 * trap doors, platforms, conveyor belts, zapper beams, gold/silver floors,
 * torches and skeletons.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources & Headers/Levers.c").
 */
#include "MMGame.h"

    // how PerformActionOnSprite affects its target
enum { kActionToggle = 0, kActionStart = 3, kActionStop = 4, kActionOpen = 5 };

    // brick switch actions
enum {
    kSwitchOpenTarget = 0, kSwitchStartTarget = 1, kSwitchStopTarget = 2,
    kSwitchConveyorsRight = 3, kSwitchConveyorsLeft = 4,
    kSwitchStartPlatforms = 5, kSwitchStopPlatforms = 6,
    kSwitchStartConveyors = 7, kSwitchStopConveyors = 8,
    kSwitchStartZappers = 9, kSwitchStopZappers = 10,
    kSwitchSilverFloors = 11, kSwitchGoldFloors = 12,
    kSwitchOpenTrapDoors = 13, kSwitchOpenGrayDoors = 14, kSwitchCloseGrayDoors = 15,
    kSwitchLightTorches = 16, kSwitchExtinguishTorches = 17,
    kSwitchTorchFlameForward = 18, kSwitchTorchFlameBackward = 19,
    kSwitchWakeSkeletons = 20
};

static Boolean IsGrayDoor(SpritePtr spriteP)
{
    return spriteP->userData > 0x40 && spriteP->userData < 0x45;
}

/// The hero pressed the action key: pull a lever if one is right there. Levers
/// that no longer work only get a tug. Returns true if a lever was pulled.
Boolean PullLeverIfPossible(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);
    SpritePtr leverP;
    Boolean leverIsPulled;

    for (leverP = SWGetNextSprite(gLeverSpriteLayerP, NULL); leverP != NULL; leverP = leverP->nextSpriteP) {
        if (GetDistBetweenSpritesHorizMid(spriteP, leverP) < 30 &&
            GetDistBetweenSpritesVertMid(spriteP, leverP) < 16 &&
            mAbs(spriteP->destFrameRect.bottom - leverP->destFrameRect.bottom) < 7)
            break;
    }
    if (leverP == NULL)
        return false;

    SW_ASSERT((leverP->userData >= kFirstLeverSprite && leverP->userData <= kLastLeverSprite) ||
              (leverP->userData >= kFirstBrickSwitchSprite && leverP->userData <= kLastBrickSwitchSprite));
    leverIsPulled = leverP->curFrameIndex != 0;
    heroP->leverStartHorizLoc = (short)SWGetSpriteHorizLoc(spriteP);
    SWMoveSprite(spriteP, leverP->destFrameRect.left, spriteP->destFrameRect.top);

        // stand on the side to pull it toward
    if (leverIsPulled) {
        SWOffsetSprite(spriteP, -2.0f, 0);
        SWSetSpriteFrameAdvance(leverP, -1);
        heroP->facingRight = false;
        if (spriteP->userData == kHeroSprite) {
            SWSetSpriteFrameRange(spriteP, 34, 37);
            SWSetSpriteFlipX(spriteP, true);
            SWSetCurrentFrameIndex(spriteP, 34);
        }
        else {
            SWSetSpriteFrameRange(spriteP, 10, 13);
            SWSetCurrentFrameIndex(spriteP, 10);
        }
    }
    else {
        SWOffsetSprite(spriteP, 2.0f, 0);
        SWSetSpriteFrameAdvance(leverP, 1);
        heroP->facingRight = true;
        if (spriteP->userData == kHeroSprite) {
            SWSetSpriteFrameRange(spriteP, 34, 37);
            SWSetSpriteFlipX(spriteP, false);
            SWSetCurrentFrameIndex(spriteP, 34);
        }
        else {
            SWSetSpriteFrameRange(spriteP, 14, 17);
            SWSetCurrentFrameIndex(spriteP, 14);
        }
    }
    SWSetSpriteMoveProc(spriteP, NULL);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameTime(spriteP, 0);
    SWResetSpriteFrameTimer(gSpriteWorldP, spriteP);

    if (!LEVER(leverP)->isActive) {
        MMPlaySound(0x18, 9, 1);
        heroP->leverTimer = 20;
        SWSetSpriteFrameProc(spriteP, TryToPullLeverFrameProc);
        SWSetCurrentFrameIndex(spriteP, 35);
        SWSetSpriteFlipX(spriteP, leverIsPulled);
        SWOffsetSprite(spriteP, leverIsPulled ? -10.0f : 10.0f, 0);
    }
    else {
        heroP->leverTimer = 12;
        SWSetSpriteFrameProc(spriteP, PullLeverFrameProc);
    }
    heroP->leverP = leverP;
    gKeys[kKeyAction] = false;
    return true;
}

/// Pulls the lever a frame at a time along with the hero's arm.
void PullLeverFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    HeroSpritePtr heroP = HERO(spriteP);
    SpritePtr leverP;
    Boolean done;

    (void)curFrameP;
    if (heroP->leverTimer-- >= 1)
        return;
    heroP->leverTimer = 12;
    leverP = heroP->leverP;
    if (spriteP->userData == kHeroSprite)
        done = *frameIndex == 37;
    else
        done = *frameIndex == 17 || *frameIndex == 13;

    if (done) {
        FinishPullingLever(spriteP, true);
        SWOffsetSprite(spriteP, spriteP->drawData->flippedHorizontal ? 13.0f : -13.0f, 0);
        if (heroP->leverStartHorizLoc - SWGetSpriteHorizLoc(spriteP) > 0)
            CheckForWall(spriteP, kSW_CheckLeft, &heroP->wallInset, heroP->isJumping, false, true);
        else
            CheckForWall(spriteP, kSW_CheckRight, &heroP->wallInset, heroP->isJumping, false, true);
        PerformLeverAction(leverP, false);
        if (spriteP->userData == kHeroSprite)
            CoopPostLeverPulled(leverP);
        if (!gCanUseLadderJump) {
            heroP->onGround = MoveSpriteDownToFloor(spriteP);
            SaveLastOnGroundPixelRow(spriteP);
            if (!heroP->onGround)
                gKeys[kKeyJump] = false;
        }
    }
    else {
        SWSetCurrentFrameIndex(leverP, leverP->curFrameIndex + leverP->frameAdvance);
        MMPlaySound(0x1b, 12, 1);
    }
    if (spriteP->drawData->flippedHorizontal)
        SWOffsetSprite(spriteP, -2.0f, 0);
    (*frameIndex)++;
}

/// The lever doesn't work any more: just let go.
void TryToPullLeverFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    HeroSpritePtr heroP = HERO(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (heroP->leverTimer-- >= 1)
        return;
    heroP->leverTimer = 20;
    FinishPullingLever(spriteP, false);
    SWOffsetSprite(spriteP, spriteP->drawData->flippedHorizontal ? 13.0f : -13.0f, 0);
}

void FinishPullingLever(SpritePtr spriteP, Boolean wasPulled)
{
    SpritePtr leverP = HERO(spriteP)->leverP;

    if (spriteP->userData == kHeroSprite) {
        SWSetSpriteMoveProc(spriteP, MainSpriteMoveProc);
        SWSetSpriteFrameAdvance(spriteP, 1);
        SWSetSpriteFrameTime(spriteP, -1);
        SWSetSpriteFrameProc(spriteP, NULL);
    }
    if (wasPulled) {
        if (LEVER(leverP)->isOneShot)
            LEVER(leverP)->isActive = false;
        if (!LEVER(leverP)->dontRememberState) {
            if (leverP->curFrameIndex == 0)
                RemoveSpriteFromObjectArray(leverP);
            else
                AddSpriteToObjectArray(leverP);
        }
    }
    HERO(spriteP)->leverP = NULL;
}

/// The sprite in the room with this level-object ID that a lever can affect.
SpritePtr FindSpriteMatchingObjectID(long objectID)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP;

    for (layerP = SWGetNextSpriteLayer(gSpriteWorldP, NULL); layerP != NULL;
         layerP = SWGetNextSpriteLayer(gSpriteWorldP, layerP)) {
        for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP)) {
            if (GAME_SPRITE(spriteP)->objectID == objectID && CanLeverAffectSprite(spriteP))
                return spriteP;
        }
    }
    return NULL;
}

/// When a room is loaded, puts levers back the way they were left, and applies
/// the effects of pulled (or inverted) levers silently.
void SetLeversToCorrectState(SpriteLayerPtr layerP)
{
    SpritePtr leverP, markerP;
    short action;

    for (leverP = SWGetNextSprite(layerP, NULL); leverP != NULL; leverP = SWGetNextSprite(layerP, leverP)) {
        action = LEVER(leverP)->action;
        if (!IsObjectInArray(GAME_SPRITE(leverP)->objectID)) {
            if (LEVER(leverP)->isInverted && action != kLeverToggleGoldSilver && action != kLeverOpenTrapDoors &&
                action != kLeverOpenGrayDoors && action != kLeverToggleGrayDoors)
                PerformLeverAction(leverP, true);
        }
        else {
            SW_ASSERT(leverP->userData >= kFirstLeverSprite && leverP->userData <= kLastLeverSprite);
            SetLeverToPulledState(leverP);
        }
        if (!LEVER(leverP)->dontRememberState) {
            markerP = NewMiscSprite(0xcd);
            if (markerP != NULL) {
                SWAddSprite(gForegroundSpriteLayerP, markerP);
                SWSetSpriteLocation(markerP, (SWGetSpriteHorizLoc(leverP) + 17.0f) - 1.0f,
                                    (SWGetSpriteVertLoc(leverP) + 50.0f) - 1.0f);
                LEVER(leverP)->markerSpriteP = markerP;
            }
        }
    }
}

void SetLeverToPulledState(SpritePtr leverP)
{
    SWSetCurrentFrameIndex(leverP, 3);
    if (LEVER(leverP)->isOneShot)
        LEVER(leverP)->isActive = false;
    if (LEVER(leverP)->action != kLeverToggleGoldSilver && !LEVER(leverP)->isInverted)
        PerformLeverAction(leverP, true);
}

// ---------------------------------------------------------------------------
// Brick switches

void CheckForPressedBrickSwitches(void)
{
    SpritePtr switchP;

    for (switchP = SWGetNextSprite(gBrickSwitchSpriteLayerP, NULL); switchP != NULL;
         switchP = SWGetNextSprite(gBrickSwitchSpriteLayerP, switchP))
        CheckBrickSwitchWithSprites(switchP);
}

/// A switch is down while someone stands on it; it acts when first pressed.
void CheckBrickSwitchWithSprites(SpritePtr switchP)
{
    SpritePtr spriteP, heroP;
    short frame;
    Boolean pressed = false, byHero = false;

    SW_ASSERT(switchP->userData >= kFirstBrickSwitchSprite);
    SW_ASSERT(switchP->userData <= kLastBrickSwitchSprite);
    frame = (short)switchP->curFrameIndex;
    for (spriteP = SWGetNextSprite(gSpyVsSpySpriteLayerP, NULL); spriteP != NULL;
         spriteP = SWGetNextSprite(gSpyVsSpySpriteLayerP, spriteP)) {
        if (IsSpriteOnSwitch(spriteP, switchP)) {
            pressed = true;
            break;
        }
    }
    if (!pressed && (heroP = GetHeroSprite()) != NULL)
        pressed = byHero = IsSpriteOnSwitch(heroP, switchP);

    if (!pressed) {
        if (frame != 0 && LEVER(switchP)->isActive)
            SWSetCurrentFrameIndex(switchP, 0);
    }
    else {
        SWSetCurrentFrameIndex(switchP, 1);
        if (frame == 0) {
            PushBrickSwitch(switchP);
            if (byHero)
                CoopPostSwitchPressed(switchP);
        }
    }
}

/// True if spriteP is standing with its feet on the switch.
Boolean IsSpriteOnSwitch(SpritePtr spriteP, SpritePtr switchP)
{
    SWRect feetRect;
    SWRect *insetP = &spriteP->curFrameP->collisionInset;

    if (!HERO(spriteP)->onGround)
        return false;
    feetRect.left = spriteP->destFrameRect.left + insetP->left;
    feetRect.right = spriteP->destFrameRect.right - insetP->right;
    feetRect.top = spriteP->destFrameRect.bottom - insetP->bottom;
    feetRect.bottom = feetRect.top + 6;
    feetRect.top += 5;
    return SWIsSpriteInRect(switchP, &feetRect);
}

void PushBrickSwitch(SpritePtr switchP)
{
    if (!PerformBrickSwitchAction(switchP, false))
        MMPlaySound(0x1c, 12, 1);
    FinishPushingBrickSwitch(switchP);
}

void FinishPushingBrickSwitch(SpritePtr switchP)
{
    if (LEVER(switchP)->isOneShot || LEVER(switchP)->action == kSwitchOpenTarget) {
        LEVER(switchP)->isActive = false;
        SWSetCurrentFrameIndex(switchP, 1);
        SWSetSpriteFrameProc(switchP, NULL);
    }
    if (!LEVER(switchP)->dontRememberState && !LEVER(switchP)->isActive)
        AddSpriteToObjectArray(switchP);
}

void SetBrickSwitchesToCorrectState(SpriteLayerPtr layerP)
{
    SpritePtr switchP;
    short action;

    for (switchP = SWGetNextSprite(layerP, NULL); switchP != NULL; switchP = SWGetNextSprite(layerP, switchP)) {
        action = LEVER(switchP)->action;
        if (!IsObjectInArray(GAME_SPRITE(switchP)->objectID)) {
            if (LEVER(switchP)->isInverted && action != kSwitchSilverFloors && action != kSwitchGoldFloors &&
                action != kSwitchOpenTrapDoors && action != kSwitchOpenGrayDoors && action != kSwitchCloseGrayDoors)
                PerformBrickSwitchAction(switchP, true);
        }
        else {
            SW_ASSERT(switchP->userData >= kFirstBrickSwitchSprite && switchP->userData <= kLastBrickSwitchSprite);
            SetBrickSwitchToPulledState(switchP);
        }
    }
}

void SetBrickSwitchToPulledState(SpritePtr switchP)
{
    SWSetCurrentFrameIndex(switchP, 1);
    SWSetSpriteFrameProc(switchP, NULL);
    if (LEVER(switchP)->isOneShot)
        LEVER(switchP)->isActive = false;
    if (LEVER(switchP)->action != kSwitchSilverFloors && LEVER(switchP)->action != kSwitchGoldFloors &&
        !LEVER(switchP)->isInverted)
        PerformBrickSwitchAction(switchP, true);
}

// ---------------------------------------------------------------------------
// Actions. "silently" means the room is being set up: no sounds or animations.

void PerformLeverAction(SpritePtr leverP, Boolean silently)
{
    LeverSpritePtr lP = LEVER(leverP);
    SpriteLayerPtr openedLayerP, closedLayerP;
    SpritePtr targetP;

    switch (lP->action) {
        case kLeverToggleTarget:
            targetP = FindSpriteMatchingObjectID(lP->targetObjectID);
            if (targetP != NULL)
                PerformActionOnSprite(targetP, leverP, silently, kActionToggle);
            break;
        case kLeverReverseOnePlatform:
            targetP = FindSpriteMatchingObjectID(lP->targetObjectID);
            if (targetP != NULL)
                DoLeverChangeOneMovingPlatform(targetP, silently);
            break;
        case kLeverReverseAllPlatforms:
            DoLeverChangeAllMovingPlatforms(silently);
            break;
        case kLeverReverseConveyors:
            DoLeverChangeConveyorBeltDir(silently, kActionToggle);
            break;
        case kLeverStartStopConveyors:
            DoLeverStartOrStopConveyorBelts(silently, kActionToggle);
            break;
        case kLeverStartStopPlatforms:
            DoLeverStartStopAllMovingPlatforms(silently, kActionToggle);
            break;
        case kLeverStartStopZappers:
            DoLeverStartStopAllZapperBeams(silently, kActionToggle);
            break;
        case kLeverToggleGoldSilver:
            DoLeverToggleGoldSilverBlocks(silently, 0);
            break;
        case kLeverOpenTrapDoors:
            DoLeverOpenAllTrapDoors(silently);
            lP->isActive = false;
            break;
        case kLeverOpenGrayDoors:
            DoLeverOpenAllGrayDoors(leverP, silently);
            lP->isActive = false;
            break;
        case kLeverToggleGrayDoors:
            if (!silently) {
                    // open the closed gray doors and close the open ones
                SWCreateSpriteLayer(&openedLayerP);
                SWCreateSpriteLayer(&closedLayerP);
                MoveSpritesFromLayerToLayer(gOpenedDoorSpriteLayerP, openedLayerP);
                DoLeverOpenAllGrayDoors(leverP, false);
                MoveSpritesFromLayerToLayer(gOpenedDoorSpriteLayerP, closedLayerP);
                MoveSpritesFromLayerToLayer(openedLayerP, gOpenedDoorSpriteLayerP);
                DoLeverCloseAllGrayDoors(leverP, false);
                MoveSpritesFromLayerToLayer(closedLayerP, gOpenedDoorSpriteLayerP);
                SWCustomSortSpriteLayer(gOpenedDoorSpriteLayerP, CompareSpriteTop);
                SWDisposeSpriteLayer(&openedLayerP);
                SWDisposeSpriteLayer(&closedLayerP);
            }
            break;
        case kLeverToggleTorches:
            DoLeverToggleAllTorches();
            break;
    }
}

/// Returns true if the switch did something audible.
Boolean PerformBrickSwitchAction(SpritePtr switchP, Boolean silently)
{
    LeverSpritePtr sP = LEVER(switchP);
    SpritePtr targetP;
    Boolean didSomething = false;

    switch (sP->action) {
        case kSwitchOpenTarget:
        case kSwitchStartTarget:
        case kSwitchStopTarget:
            targetP = FindSpriteMatchingObjectID(sP->targetObjectID);
            if (targetP != NULL)
                didSomething = PerformActionOnSprite(targetP, switchP, silently,
                                                     sP->action == kSwitchOpenTarget ? kActionOpen :
                                                     sP->action == kSwitchStartTarget ? kActionStart : kActionStop);
            break;
        case kSwitchConveyorsRight:    DoLeverChangeConveyorBeltDir(silently, 1); break;
        case kSwitchConveyorsLeft:     DoLeverChangeConveyorBeltDir(silently, 2); break;
        case kSwitchStartConveyors:    DoLeverStartOrStopConveyorBelts(silently, kActionStart); break;
        case kSwitchStopConveyors:     DoLeverStartOrStopConveyorBelts(silently, kActionStop); break;
        case kSwitchStartPlatforms:    DoLeverStartStopAllMovingPlatforms(silently, kActionStart); break;
        case kSwitchStopPlatforms:     DoLeverStartStopAllMovingPlatforms(silently, kActionStop); break;
        case kSwitchStartZappers:      didSomething = DoLeverStartStopAllZapperBeams(silently, kActionStart); break;
        case kSwitchStopZappers:       didSomething = DoLeverStartStopAllZapperBeams(silently, kActionStop); break;
        case kSwitchSilverFloors:      DoLeverToggleGoldSilverBlocks(silently, 1); break;
        case kSwitchGoldFloors:        DoLeverToggleGoldSilverBlocks(silently, 2); break;
        case kSwitchOpenTrapDoors:
            DoLeverOpenAllTrapDoors(silently);
            sP->isActive = false;
            break;
        case kSwitchOpenGrayDoors:     DoLeverOpenAllGrayDoors(switchP, silently); break;
        case kSwitchCloseGrayDoors:    DoLeverCloseAllGrayDoors(switchP, silently); break;
        case kSwitchLightTorches:      DoLeverLightAllTorches(); break;
        case kSwitchExtinguishTorches: DoLeverExtinguishAllTorches(); break;
        case kSwitchTorchFlameForward: DoLeverMoveTorchFlameForward(); break;
        case kSwitchTorchFlameBackward: DoLeverMoveTorchFlameBackward(); break;
        case kSwitchWakeSkeletons:     DoLeverMakeAllSkeletonsComeAlive(); break;
    }
    return didSomething;
}

/// Doors open/toggle, trap doors open, zapper nodes and moving platforms start or
/// stop, skeletons wake up.
Boolean PerformActionOnSprite(SpritePtr targetP, SpritePtr leverP, Boolean silently, short action)
{
    short type = (short)targetP->userData;
    Boolean didSomething = false, wasMoving;

    if (type >= kFirstDoorSprite && type <= kLastDoorSprite && (action == kActionToggle || action == kActionOpen)) {
        if (!silently || !LEVER(leverP)->isInverted) {
            if (action == kActionToggle) {
                didSomething = DoLeverToggleDoor(leverP, targetP, silently);
            }
            else {
                didSomething = DoLeverOpenDoor(leverP, targetP, silently);
                LEVER(leverP)->isActive = false;
            }
        }
    }
    else if (type == 0x5f || (type == 0x60 && (action == kActionToggle || action == kActionOpen))) {
        if (!silently || !LEVER(leverP)->isInverted) {
            didSomething = DoLeverOpenTrapDoor(targetP, silently, true);
            LEVER(leverP)->isActive = false;
        }
    }
    else if (type >= kFirstZapperNodeSprite && type <= kLastZapperNodeSprite) {
        didSomething = DoLeverAffectOneZapperNode(targetP, silently, action);
    }
    else if (type >= kFirstMovingPlatformSprite && type <= kLastMovingPlatformSprite) {
        wasMoving = PLATFORM(targetP)->isMoving;
        if (action == kActionToggle)
            PLATFORM(targetP)->isMoving = !wasMoving;
        else if (action == kActionStop)
            PLATFORM(targetP)->isMoving = false;
        else if (action == kActionStart)
            PLATFORM(targetP)->isMoving = true;
        if (!silently) {
            if (!wasMoving)
                MMPlaySound(0x49, 12, 3);
            else
                MMPlayStereoSound2(NULL, 0x49, 12, 3, gSoundVolume, 0.9, 0);
        }
    }
    else if (type == kWalkingSkeletonSprite && action == kActionStart) {
        MakeSkeletonDoUncollapse(targetP);
    }
    return didSomething;
}

Boolean CanLeverAffectSprite(SpritePtr spriteP)
{
    short type = (short)spriteP->userData;

    return (type >= kFirstDoorSprite && type <= kLastDoorSprite) || type == 0x5f || type == 0x60 ||
           (type >= kFirstZapperNodeSprite && type <= kLastZapperNodeSprite) ||
           (type >= kFirstMovingPlatformSprite && type <= kLastMovingPlatformSprite) ||
           type == kWalkingSkeletonSprite;
}

void DoLeverChangeAllMovingPlatforms(Boolean silently)
{
    SpritePtr spriteP;

    for (spriteP = SWGetNextSprite(gHorizMovingPlatformSpriteLayerP, NULL); spriteP != NULL;
         spriteP = SWGetNextSprite(gHorizMovingPlatformSpriteLayerP, spriteP))
        DoLeverChangeOneMovingPlatform(spriteP, silently);
    for (spriteP = SWGetNextSprite(gVertMovingPlatformSpriteLayerP, NULL); spriteP != NULL;
         spriteP = SWGetNextSprite(gVertMovingPlatformSpriteLayerP, spriteP))
        DoLeverChangeOneMovingPlatform(spriteP, silently);
}

void DoLeverChangeOneMovingPlatform(SpritePtr spriteP, Boolean silently)
{
    (void)silently;
    SWSetSpriteMoveDelta(spriteP, -SWGetSpriteHorizMoveDelta(spriteP), -SWGetSpriteVertMoveDelta(spriteP));
}

/// mode 0 reverses, 1 sets right, 2 sets left. Stopped belts just remember it.
void DoLeverChangeConveyorBeltDir(Boolean silently, short mode)
{
    if (mode == 0)
        gOldConveyorBeltDirection = -gOldConveyorBeltDirection;
    else if (mode == 1)
        gOldConveyorBeltDirection = 1;
    else if (mode == 2)
        gOldConveyorBeltDirection = -1;
    if (!silently && gCurConveyorBeltDirection != 0 && gCurConveyorBeltDirection != gOldConveyorBeltDirection)
        MMPlaySound(0x46, 12, 3);
    if (gCurConveyorBeltDirection != 0)
        gCurConveyorBeltDirection = gOldConveyorBeltDirection;
}

void DoLeverStartOrStopConveyorBelts(Boolean silently, short action)
{
    if (action == kActionToggle)
        action = gCurConveyorBeltDirection == 0 ? kActionStart : kActionStop;
    if (action == kActionStart) {
        if (gCurConveyorBeltDirection == 0) {
            gCurConveyorBeltDirection = gOldConveyorBeltDirection;
            if (!silently)
                MMPlaySound(0x44, 12, 3);
        }
    }
    else if (action == kActionStop && gCurConveyorBeltDirection != 0) {
        gOldConveyorBeltDirection = gCurConveyorBeltDirection;
        gCurConveyorBeltDirection = 0;
        if (!silently)
            MMPlaySound(0x45, 12, 3);
    }
}

void DoLeverStartStopAllMovingPlatforms(Boolean silently, short action)
{
    DoLeverStartStopPlatformsInLayer(gHorizMovingPlatformSpriteLayerP, action, silently);
    DoLeverStartStopPlatformsInLayer(gVertMovingPlatformSpriteLayerP, action, silently);
}

void DoLeverStartStopPlatformsInLayer(SpriteLayerPtr layerP, short action, Boolean silently)
{
    SpritePtr spriteP;
    Boolean wasMoving;

    for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP)) {
        wasMoving = PLATFORM(spriteP)->isMoving;
        if (action == kActionToggle) {
            PLATFORM(spriteP)->isMoving = !wasMoving;
            if (!silently) {
                if (!wasMoving)
                    MMPlaySound(0x49, 12, 3);
                else
                    MMPlayStereoSound2(NULL, 0x49, 12, 3, gSoundVolume, 0.9, 0);
            }
        }
        else if (action == kActionStart) {
            if (!silently && !wasMoving)
                MMPlaySound(0x49, 12, 3);
            PLATFORM(spriteP)->isMoving = true;
        }
        else if (action == kActionStop) {
            if (!silently && wasMoving)
                MMPlayStereoSound2(NULL, 0x49, 12, 3, gSoundVolume, 0.9, 0);
            PLATFORM(spriteP)->isMoving = false;
        }
    }
}

/// mode 0 swaps which of the gold and silver floors is solid, 1 makes silver
/// solid, 2 gold.
void DoLeverToggleGoldSilverBlocks(Boolean silently, short mode)
{
    if (!silently) {
        MMPlaySound(0x42, 12, 3);
        DisposeAllMapCachedRooms();
    }
    if (mode == 0) {
        gSilverFloorIsActive = !gSilverFloorIsActive;
        gGoldFloorIsActive = !gGoldFloorIsActive;
    }
    else if (mode == 1) {
        gSilverFloorIsActive = true;
        gGoldFloorIsActive = false;
    }
    else if (mode == 2) {
        gSilverFloorIsActive = false;
        gGoldFloorIsActive = true;
    }
    SetCorrectGoldSilverTileImages(true);
    if (!silently)
        DrawBackgroundTextureForCurRoom();
}

/// Inactive gold/silver tiles show the outline images 0x1c tiles further on.
void SetCorrectGoldSilverTileImages(Boolean updateNow)
{
    short tile, offset;

    offset = gSilverFloorIsActive ? 0 : 0x1c;
    for (tile = 0x1af; tile < 0x1bd; tile++) {
        if (!updateNow)
            SWChangeTileImageFast(gSpriteWorldP, tile, offset + tile);
        else
            SWChangeTileImage(gSpriteWorldP, tile, offset + tile);
    }
    offset = gGoldFloorIsActive ? 0 : 0x1c;
    for (tile = 0x1bd; tile < 0x1cb; tile++) {
        if (!updateNow)
            SWChangeTileImageFast(gSpriteWorldP, tile, offset + tile);
        else
            SWChangeTileImage(gSpriteWorldP, tile, offset + tile);
    }
}

/// Opening a door moves it to another layer, so restart the scan when that happens.
Boolean DoLeverOpenAllGrayDoors(SpritePtr leverP, Boolean silently)
{
    SpritePtr doorP, nextP;
    Boolean didSomething = false;

    doorP = SWGetNextSprite(gDoorSpriteLayerP, NULL);
    while (doorP != NULL) {
        nextP = SWGetNextSprite(gDoorSpriteLayerP, doorP);
        if (IsGrayDoor(doorP) && DoLeverOpenDoor(leverP, doorP, silently))
            didSomething = true;
        doorP = nextP;
        if (nextP != NULL && nextP->parentSpriteLayerP != gDoorSpriteLayerP)
            doorP = SWGetNextSprite(gDoorSpriteLayerP, NULL);
    }
    SWCustomSortSpriteLayer(gDoorSpriteLayerP, CompareSpriteTop);
    return didSomething;
}

Boolean DoLeverToggleDoor(SpritePtr leverP, SpritePtr doorP, Boolean silently)
{
    if (silently)
        return false;
    if (!DOOR(doorP)->isOpen)
        return DoLeverOpenDoor(leverP, doorP, false);
    return DoLeverCloseDoor(leverP, doorP, false);
}

Boolean DoLeverOpenDoor(SpritePtr leverP, SpritePtr doorP, Boolean silently)
{
    if (DOOR(doorP)->isOpen || SWGetSpriteMoveProc(doorP) == DoorSpriteMoveProc)
        return false;
    if (!silently) {
        MakeDoorOpen(doorP, !LEVER(leverP)->dontRememberState);
        MMPlayStereoSound(doorP, 1, 8, 3);
        Rumble(kRumbleHeavy);
        return true;
    }
    SWCollideSpriteWithLayer(doorP, gDoorSpriteLayerP, OpenDoorInstantlyCollideProc);
    SetDoorToOpenedState(doorP);
    return false;
}

Boolean DoLeverCloseAllGrayDoors(SpritePtr leverP, Boolean silently)
{
    SpritePtr doorP, nextP;
    Boolean didSomething = false;

    doorP = SWGetNextSprite(gOpenedDoorSpriteLayerP, NULL);
    while (doorP != NULL) {
        nextP = SWGetNextSprite(gOpenedDoorSpriteLayerP, doorP);
        if (IsGrayDoor(doorP) && DoLeverCloseDoor(leverP, doorP, silently))
            didSomething = true;
        doorP = nextP;
        if (nextP != NULL && nextP->parentSpriteLayerP != gOpenedDoorSpriteLayerP)
            doorP = SWGetNextSprite(gOpenedDoorSpriteLayerP, NULL);
    }
    SWCustomSortSpriteLayer(gDoorSpriteLayerP, CompareSpriteTop);
    return didSomething;
}

Boolean DoLeverCloseDoor(SpritePtr leverP, SpritePtr doorP, Boolean silently)
{
    Boolean didSomething = false;

    if (DOOR(doorP)->isOpen && SWGetSpriteMoveProc(doorP) != CloseDoorSpriteMoveProc) {
        if (!silently) {
            MakeDoorClose(doorP, !LEVER(leverP)->dontRememberState);
            MMPlayStereoSound(doorP, 0x29, 8, 3);
            didSomething = true;
        }
        else {
            SetDoorToClosedState(doorP);
            SWCollideSpriteWithLayer(doorP, gDoorSpriteLayerP, CloseDoorInstantlyCollideProc);
        }
    }
    SWCustomSortSpriteLayer(gDoorSpriteLayerP, CompareSpriteTop);
    return didSomething;
}

Boolean DoLeverOpenAllTrapDoors(Boolean silently)
{
    SpritePtr trapDoorP, nextP;
    Boolean didSomething = false;

    trapDoorP = SWGetNextSprite(gTrapDoorSpriteLayerP, NULL);
    while (trapDoorP != NULL) {
        nextP = SWGetNextSprite(gTrapDoorSpriteLayerP, trapDoorP);
        if (DoLeverOpenTrapDoor(trapDoorP, silently, false))
            didSomething = true;
        trapDoorP = nextP;
        if (nextP != NULL && nextP->parentSpriteLayerP != gTrapDoorSpriteLayerP)
            trapDoorP = SWGetNextSprite(gTrapDoorSpriteLayerP, NULL);
    }
    return didSomething;
}

Boolean DoLeverOpenTrapDoor(SpritePtr trapDoorP, Boolean silently, Boolean includeOtherHalf)
{
    if (silently) {
        SetTrapDoorToOpenedState(trapDoorP, includeOtherHalf);
        return false;
    }
    if (trapDoorP->parentSpriteLayerP != gTrapDoorSpriteLayerP)
        return false;
    MakeTrapDoorOpen(trapDoorP);
    if (includeOtherHalf)
        SWCollideSpriteWithLayer(trapDoorP, gTrapDoorSpriteLayerP, OpenTrapDoorCollideProc);
    if (FindSound(0x3b) == -1) {
        MMPlaySound(0x3b, 8, 1);
        return true;
    }
    return false;
}

Boolean DoLeverStartStopAllZapperBeams(Boolean silently, short action)
{
    SpritePtr nodeP;
    Boolean didSomething = false;

    for (nodeP = SWGetNextSprite(gZapperNodeSpriteLayerP, NULL); nodeP != NULL;
         nodeP = SWGetNextSprite(gZapperNodeSpriteLayerP, nodeP)) {
        if (DoLeverAffectOneZapperNode(nodeP, silently, action))
            didSomething = true;
    }
    return didSomething;
}

/// kActionStart enables the node's beam, kActionStop disables it, kActionToggle
/// toggles. A node with no partner just glows red.
Boolean DoLeverAffectOneZapperNode(SpritePtr nodeP, Boolean silently, short action)
{
    ZapperNodeSpritePtr zP = ZAPPER(nodeP);
    Boolean wasDisabled = zP->isDisabled, didSomething = false;
    short advance;

    SW_ASSERT(nodeP->userData >= kFirstZapperNodeSprite && nodeP->userData <= kLastZapperNodeSprite);
    if (action == kActionToggle)
        zP->isDisabled = !zP->isDisabled;
    else if (action == kActionStart)
        zP->isDisabled = false;
    else if (action == kActionStop)
        zP->isDisabled = true;

    if (zP->isDisabled || zP->isOn) {
        advance = !zP->isDisabled;
        SWSetSpriteFrameAdvance(nodeP, advance);
        if (zP->partnerP == NULL) {
            SWSetSpriteColor(nodeP, 0xffff, 0, 0);
            didSomething = true;
        }
        else {
            SWSetSpriteFrameAdvance(zP->partnerP, advance);
            if (!silently && zP->isDisabled != wasDisabled) {
                MMPlaySound(zP->isDisabled ? 0x24 : 0x25, 12, 3);
                didSomething = true;
            }
        }
    }
    return didSomething;
}

// ---------------------------------------------------------------------------
// Torches and skeletons

void DoLeverToggleAllTorches(void)
{
    SpritePtr torchP;

    for (torchP = SWGetNextSprite(gTorchSpriteLayerP, NULL); torchP != NULL;
         torchP = SWGetNextSprite(gTorchSpriteLayerP, torchP)) {
        if (TORCH(torchP)->flameSpriteP == NULL)
            LightTorch(torchP);
        else
            UnlightTorch(torchP);
    }
}

void DoLeverLightAllTorches(void)
{
    SpritePtr torchP;

    for (torchP = SWGetNextSprite(gTorchSpriteLayerP, NULL); torchP != NULL;
         torchP = SWGetNextSprite(gTorchSpriteLayerP, torchP)) {
        if (TORCH(torchP)->flameSpriteP == NULL)
            LightTorch(torchP);
    }
}

void DoLeverExtinguishAllTorches(void)
{
    SpritePtr torchP;

    for (torchP = SWGetNextSprite(gTorchSpriteLayerP, NULL); torchP != NULL;
         torchP = SWGetNextSprite(gTorchSpriteLayerP, torchP)) {
        if (TORCH(torchP)->flameSpriteP != NULL)
            UnlightTorch(torchP);
    }
}

/// Each torch takes the lit state of the one before it (the first wraps around).
void DoLeverMoveTorchFlameForward(void)
{
    SpritePtr torchP, prevP;
    Boolean lastWasLit;

    torchP = SWGetPreviousSprite(gTorchSpriteLayerP, NULL);
    if (torchP == NULL)
        return;
    lastWasLit = TORCH(torchP)->flameSpriteP != NULL;
    while (torchP != NULL) {
        prevP = SWGetPreviousSprite(gTorchSpriteLayerP, torchP);
        if ((prevP == NULL ? lastWasLit : TORCH(prevP)->flameSpriteP != NULL))
            LightTorch(torchP);
        else
            UnlightTorch(torchP);
        torchP = prevP;
    }
}

void DoLeverMoveTorchFlameBackward(void)
{
    SpritePtr torchP, nextP;
    Boolean firstWasLit;

    torchP = SWGetNextSprite(gTorchSpriteLayerP, NULL);
    if (torchP == NULL)
        return;
    firstWasLit = TORCH(torchP)->flameSpriteP != NULL;
    while (torchP != NULL) {
        nextP = SWGetNextSprite(gTorchSpriteLayerP, torchP);
        if ((nextP == NULL ? firstWasLit : TORCH(nextP)->flameSpriteP != NULL))
            LightTorch(torchP);
        else
            UnlightTorch(torchP);
        torchP = nextP;
    }
}

void DoLeverMakeAllSkeletonsComeAlive(void)
{
    SpritePtr skeletonP, nextP;

    for (skeletonP = SWGetNextSprite(gSkeletonBonesSpriteLayerP, NULL); skeletonP != NULL; skeletonP = nextP) {
        nextP = SWGetNextSprite(gSkeletonBonesSpriteLayerP, skeletonP);
        MakeSkeletonDoUncollapse(skeletonP);
    }
}

void LightTorch(SpritePtr torchP)
{
    SpritePtr flameP;
    short h, v;

    if (TORCH(torchP)->flameSpriteP != NULL)
        return;
    flameP = NewFlameSprite();
    if (flameP != NULL) {
        h = (short)SWGetSpriteHorizLoc(torchP);
        v = (short)(SWGetSpriteVertLoc(torchP) - SWGetSpriteHeight(flameP));
        if (torchP->userData == 0x9f) {
            v += 5;
        }
        else if (torchP->userData == 0xa0) {
            v += 1;
        }
        else if (torchP->userData == 0x9e) {
            h -= 3;
            v += 7;
        }
        SWSetSpriteLocation(flameP, h, v);
        MySetSpriteFrameTime(torchP, 76);
        CreateGlowOverSprite(torchP);
    }
    TORCH(torchP)->flameSpriteP = flameP;
}

void UnlightTorch(SpritePtr torchP)
{
    SW_ASSERT(torchP->userData == 0x9e || torchP->userData == 0x9f || torchP->userData == 0xa0);
    if (TORCH(torchP)->flameSpriteP == NULL)
        return;
    SWRemoveSpriteFromAnimation(TORCH(torchP)->flameSpriteP, true);
    if (TORCH(torchP)->glowSpriteP != NULL)
        SWRemoveSpriteFromAnimation(TORCH(torchP)->glowSpriteP, true);
    TORCH(torchP)->glowSpriteP = NULL;
    SWSetSpriteFrameTime(torchP, -1);
    TORCH(torchP)->flameSpriteP = NULL;
}

void ResetConveyorBeltDir(void)
{
    gCurConveyorBeltDirection = 1;
    gOldConveyorBeltDirection = 1;
}

short CompareSpriteTop(SpritePtr a, SpritePtr b)
{
    return (short)(b->destFrameRect.bottom - a->destFrameRect.bottom);
}

short CompareSpriteBottom(SpritePtr a, SpritePtr b)
{
    return (short)(a->destFrameRect.bottom - b->destFrameRect.bottom);
}
