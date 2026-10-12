/*
 * MoveProcSupport.c — helpers for the sprites' move procs: ladders, jumping and
 * landing, doors, save pedestals, and the wall/floor collision checks.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources & Headers/MoveProcSupport.c").
 */
#include "MMGame.h"

#define kNumLadderTiles     0x5a2

    // tile ID ranges
#define kFirstWallTile          1
#define kLastWallTile           0x1a2
#define kFirstSilverFloorTile   0x1af
#define kLastSilverFloorTile    0x1b2
#define kFirstGoldFloorTile     0x1bd
#define kLastGoldFloorTile      0x1c0
#define kFirstVineTopTile       0x1fb
#define kLastVineTopTile        0x1fc
#define kFirstLadderTile        0x20b
#define kLastLadderTile         0x240
#define kFirstLeftConveyorTile  0x241
#define kLastLeftConveyorTile   0x243
#define kFirstRightConveyorTile 0x249
#define kLastRightConveyorTile  0x24b
#define kFirstDisappearingFloorTile 0x2c1
#define kLastDisappearingFloorTile  0x2c3
#define kFirstLightSaberFloorTile   0x2e9
#define kLastLightSaberFloorTile    0x2eb

short gLadderArray[kNumLadderTiles];

static short FloorDiv16(short n)
{
    return (short)((n < 0 ? n + 15 : n) >> 4);
}

static TileMapStructPtr TileLayer(short layer)
{
    return gSpriteWorldP->tileLayerArray[layer];
}

// ---------------------------------------------------------------------------

void RemoveSpriteIfOutsideRoom(SpritePtr spriteP)
{
    RemoveSpriteIfOutsideRoomByMargin(spriteP, 200);
}

/// Removes the sprite once it is more than margin pixels outside the room, unless
/// the hero is standing on it.
void RemoveSpriteIfOutsideRoomByMargin(SpritePtr spriteP, short margin)
{
    SWRect boundsRect = gRoomMoveBoundsRect;
    SpritePtr heroP;

    SWInsetRect(&boundsRect, -margin, -margin);
    if (!SWIsSpriteInRect(spriteP, &boundsRect)) {
        heroP = GetHeroSprite();
        if (heroP == NULL || HERO(heroP)->platformP != spriteP)
            SWRemoveSpriteFromAnimation(spriteP, true);
    }
}

/// Ladder tiles come in groups of 16 columns: 2 left rails (1), 8 rungs (2),
/// 2 right rails (3), 2 empty (0), then a narrow ladder: 2 rungs (2), 2 rails (3).
void SetUpLadderArray(void)
{
    short tile, end;

    for (tile = 0; tile < kNumLadderTiles; tile++)
        gLadderArray[tile] = 0;

    tile = kFirstLadderTile;
    while (tile <= kLastLadderTile) {
        for (end = tile + 2; tile < end; tile++) gLadderArray[tile] = 1;
        for (end = tile + 8; tile < end; tile++) gLadderArray[tile] = 2;
        for (end = tile + 2; tile < end; tile++) gLadderArray[tile] = 3;
        for (end = tile + 2; tile < end; tile++) gLadderArray[tile] = 0;
        for (end = tile + 2; tile < end; tile++) gLadderArray[tile] = 2;
        for (end = tile + 2; tile < end; tile++) gLadderArray[tile] = 3;
    }
}

short GetLadderValue(short tileID)
{
    if (tileID < 0 || tileID >= kNumLadderTiles)
        return 0;
    return gLadderArray[tileID];
}

/// When the hero walks off an edge of the room, loads the neighboring room and
/// moves the hero to the opposite edge.
void ExitRoomIfAppropriate(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);
    SWRect *workArea = &gSpriteWorldP->workAreaRect;
    short hOffset = 0, vOffset = 0;
    Boolean exitRoom = false, exitVertically = false;
    short deltaH, deltaV, roomWidth, roomHeight;
    short left, top, right, bottom, width, height;
    short row, col, newLevel, newResID;

    if (heroP->isDead || heroP->platformP != NULL)
        return;

    deltaH = spriteP->destFrameRect.left - spriteP->oldFrameRect.left;
    deltaV = spriteP->destFrameRect.top - spriteP->oldFrameRect.top;
    roomWidth = workArea->right - workArea->left;
    roomHeight = workArea->bottom - workArea->top;

    left = spriteP->destFrameRect.left + heroP->wallInset.left;
    top = spriteP->destFrameRect.top + heroP->wallInset.top;
    right = spriteP->destFrameRect.right - heroP->wallInset.right;
    bottom = spriteP->destFrameRect.bottom - heroP->wallInset.bottom;
    width = right - left;
    height = bottom - top;

    GEGetRowAndColFromLevelNumber(gCurrentLevel, &row, &col, 200);

    if (left + width < workArea->left && deltaH < 0) {
        hOffset = width + roomWidth + 3;
        col--;
        exitRoom = true;
    }
    else if (workArea->right < right - width && deltaH > 0) {
        hOffset = -9 - (roomWidth + width);
        col++;
        exitRoom = true;
    }

    if (top + height < workArea->top && deltaV < 0 && abs(deltaV) < roomHeight / 2 && !heroP->isJumping) {
        vOffset = height + roomHeight;
        row--;
        exitRoom = true;
        exitVertically = true;
    }
    else if (workArea->bottom < bottom - height && deltaV > 0 && abs(deltaV) < roomHeight / 2) {
        vOffset = -(height + roomHeight);
        row++;
        exitRoom = true;
        exitVertically = true;
    }

    if (!exitRoom)
        return;

    gLastLevelExited = gCurrentLevel;
    newLevel = GEGetLevelNumberFromRowAndCol(row, col, 200);
    newResID = GEGetLevelResID(newLevel, 1, 200);
    if (!Resource1Exists("TMAP", newResID))
        return;

    WriteRoomInfoToFile();
    SmoothTransitionCaptureOldRoom();       // "Smooth room transitions" (Extras)
    if (exitVertically)
        heroP->lastGroundVertLoc += vOffset + 0x30;
    SWOffsetSprite(spriteP, hOffset, vOffset);

    if (gStartRecordingMovie) {
        gCurrentLevel = newLevel;
        gRunningFrameCount = 0;
        ResetFloorTileAnimations(false, true);
        OpenMovieFileForOutput(false, false, SWGetSpriteHorizLoc(spriteP), SWGetSpriteVertLoc(spriteP));
    }
    LoadLevel(newLevel, false);
    SetUpLevelForPlaying();
    DrawLevel();
    SmoothTransitionToNewRoom(hOffset < 0 ? 1 : hOffset > 0 ? -1 : 0, vOffset < 0 ? 1 : vOffset > 0 ? -1 : 0);
    if (gStartRecordingMovie) {
        gKeys[kKeyJump] = gKeys[kKeyJumpLeft] = gKeys[kKeyJumpRight] = false;
        WriteCurrentKeysStateToMovie();
        SaveStartingPosition(spriteP);
        gStartRecordingMovie = false;
    }
    DoGameFadeToNormal(30);
}

/// Remembers where the hero entered the room (to restart there after dying),
/// once the hero is fully in the room and on safe ground.
void SaveStartingPositionIfAppropriate(SpritePtr spriteP, Boolean onGround)
{
    HeroSpritePtr heroP = HERO(spriteP);
    SWRect roomRect;
    MoveProcPtr moveProc;
    Boolean silver, gold, onFloor = onGround;

    if (onGround) {
            // a vanishing gold/silver floor is no place to restart
        silver = gSilverFloorIsActive;
        gold = gGoldFloorIsActive;
        gSilverFloorIsActive = gGoldFloorIsActive = false;
        onFloor = CheckForWall(spriteP, kSW_CheckBottom, &heroP->wallInset, false, true, false);
        gSilverFloorIsActive = silver;
        gGoldFloorIsActive = gold;
    }

    roomRect = gSpriteWorldP->worldRect;
    SWOffsetRect(&roomRect, -roomRect.left, -roomRect.top);
    moveProc = SWGetSpriteMoveProc(spriteP);
    if (moveProc != ClimbingSpriteMoveProc && moveProc != VineClimbingMoveProc)
        roomRect.top -= 32;

    if (gLastLevelPositionWasSaved != gCurrentLevel || gLastLevelExited != gCurrentLevel) {
        if (moveProc != ClimbingSpriteMoveProc && moveProc != VineClimbingMoveProc) {
            if (!onFloor || heroP->isJumping || !heroP->wasOnGround)
                return;
        }
        if (SWIsSpriteFullyInRect(spriteP, &roomRect))
            SaveStartingPosition(spriteP);
    }
}

void SaveStartingPosition(SpritePtr spriteP)
{
    SW_ASSERT(spriteP->userData == kHeroSprite);
    SaveHeroStartingLocation((short)SWGetSpriteHorizLoc(spriteP), (short)SWGetSpriteVertLoc(spriteP));
    gStartingFacingRight = HERO(spriteP)->facingRight;
    gStartingSpriteState = GetSpriteState(spriteP);
    gStartingVineTileLayer = HERO(spriteP)->vineTileLayer;
}

/// 0 walking, 1 on a ladder, 2 on a vine, 3 on a pole.
short GetSpriteState(SpritePtr spriteP)
{
    MoveProcPtr moveProc = SWGetSpriteMoveProc(spriteP);

    if (moveProc == ClimbingSpriteMoveProc)
        return 1;
    if (moveProc == VineClimbingMoveProc)
        return 2;
    if (moveProc == PoleSlidingMoveProc)
        return 3;
    if (moveProc == BlinkingSpriteMoveProc)
        return gStartingSpriteState;
    return 0;
}

void SaveHeroStartingLocation(short col, short row)
{
    gLastLevelPositionWasSaved = gCurrentLevel;
    gLastLevelExited = gCurrentLevel;
    gLastMusicIndex = gCurrentMusicIndex;
    gStartingCol = col;
    gStartingRow = row;
    gStartingFacingRight = true;
    gStartingSpriteState = 0;
}

/// Nudges the sprite down onto the floor below it. Returns true if it is on the floor.
Boolean MoveSpriteDownToFloor(SpritePtr spriteP)
{
    SWRect oldRect = spriteP->oldFrameRect;
    Boolean onFloor;

    spriteP->oldFrameRect = spriteP->destFrameRect;
    SWOffsetSprite(spriteP, 0, 1.3f);
    onFloor = CheckForWall(spriteP, kSW_CheckBottom, &HERO(spriteP)->wallInset, false, false, true);
    if (onFloor)
        spriteP->drawData->vertMoveDelta = 0;
    spriteP->oldFrameRect = oldRect;
    return onFloor;
}

void DoHeroLandedFromJumpFrame(SpritePtr spriteP, Boolean diesFromFall)
{
    HeroSpritePtr heroP = HERO(spriteP);

    SW_ASSERT(spriteP->userData == kHeroSprite);
    heroP->animTimer = 8;
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameProc(spriteP, HeroLandedFrameProc);
    SWSetSpriteFrameTime(spriteP, 0);
    heroP->isLanding = true;
    if (diesFromFall) {
        SWSetSpriteMoveProc(spriteP, DeadHeroMoveProc);
        heroP->deathType = kDeathNone;
    }
    heroP->onGround = true;
    SetHeroGroundFrame(spriteP);
}

/// Standing (frames 0-2, breathing) or walking (frames 3-11).
void SetHeroGroundFrame(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);
    Boolean startWalking;

    if (spriteP->frameChangeProc == IdleHeroFrameProc)
        return;

    if (heroP->isLanding) {
        SWSetSpriteFrameRange(spriteP, 14, 14);
        SWSetSpriteFlipX(spriteP, !heroP->facingRight);
    }
    else if (SWGetSpriteHorizMoveDelta(spriteP) != 0) {
        startWalking = spriteP->curFrameIndex < 3 || spriteP->curFrameIndex > 11;
        SWSetSpriteFrameRange(spriteP, 3, 11);
        SWSetSpriteFlipX(spriteP, !heroP->facingRight);
        if (startWalking) {
            SWSetSpriteFrameAdvanceMode(spriteP, kNoLoopingMode);
            SWSetSpriteFrameProc(spriteP, HeroMiscFrameProc);
            SWSetSpriteFrameAdvance(spriteP, 0);
            SWSetSpriteFrameTime(spriteP, 0);
            heroP->frameTimer = 3;
            heroP->frameDelay = 3;
            heroP->frameDirection = 1;
        }
    }
    else if (spriteP->curFrameIndex < 0 || spriteP->curFrameIndex > 2) {
        SWSetSpriteFrameRange(spriteP, 0, 2);
        SWSetSpriteFlipX(spriteP, !heroP->facingRight);
        SWSetSpriteFrameAdvanceMode(spriteP, kPatrollingMode);
        SWSetSpriteFrameProc(spriteP, HeroMiscFrameProc);
        SWSetSpriteFrameAdvance(spriteP, 0);
        SWSetSpriteFrameTime(spriteP, 0);
        heroP->frameTimer = 15;
        heroP->frameDelay = 15;
        heroP->frameDirection = 1;
    }
    SaveLastOnGroundPixelRow(spriteP);
}

void SaveLastOnGroundPixelRow(SpritePtr spriteP)
{
    HERO(spriteP)->lastGroundVertLoc = (short)SWGetSpriteVertLoc(spriteP);
}

void SetHeroJumpingFrame(SpritePtr spriteP)
{
    short frame = SWGetSpriteHorizMoveDelta(spriteP) == 0 ? 13 : 12;

    SWSetSpriteFrameRange(spriteP, frame, frame);
    SWSetSpriteFlipX(spriteP, !HERO(spriteP)->facingRight);
}

/// A jump pressed shortly before landing still happens once the hero is on the
/// ground. Returns true when the hero should jump now.
Boolean ProcessJumpIntention(SpritePtr spriteP, Boolean onGround)
{
    HeroSpritePtr heroP = HERO(spriteP);
    Boolean jump = false;

    if ((gKeys[kKeyJump] || gKeys[kKeyJumpRight] || gKeys[kKeyJumpLeft]) && !heroP->isDoingPushups) {
        heroP->jumpBufferFrames = 10;
        if (gKeys[kKeyJumpLeft])
            heroP->jumpBufferDirection = 0;
        else if (gKeys[kKeyJumpRight])
            heroP->jumpBufferDirection = 2;
        else
            heroP->jumpBufferDirection = 1;
        if (!onGround || !heroP->canJump)
            gKeys[kKeyJump] = gKeys[kKeyJumpRight] = gKeys[kKeyJumpLeft] = false;
    }
    if (heroP->jumpBufferFrames > 0)
        heroP->jumpBufferFrames--;

    if (heroP->jumpBufferFrames > 0 && heroP->canJump && onGround) {
        if (heroP->jumpBufferDirection == 0)
            gKeys[kKeyJumpLeft] = true;
        else if (heroP->jumpBufferDirection == 2)
            gKeys[kKeyJumpRight] = true;
        else
            gKeys[kKeyJump] = true;
        jump = true;
    }
    return jump;
}

void StartJump(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);
    float platformDelta;
    short direction;

    if (gKeys[kKeyJumpLeft])
        direction = -1;
    else if (gKeys[kKeyJumpRight])
        direction = 1;
    else
        direction = gKeys[kKeyRight] - gKeys[kKeyLeft];

    spriteP->drawData->horizMoveDelta = direction * 2.5f;
    platformDelta = heroP->platformHorizDelta;
    if (spriteP->drawData->horizMoveDelta != 0)
        platformDelta /= 1.5f;
    spriteP->drawData->horizMoveDelta *= gHorizontalBoost;
    spriteP->drawData->vertMoveDelta = -gJumpSpeed;
    heroP->isJumping = true;
    heroP->canJump = false;
    heroP->jumpFrameCount = 0;
    heroP->isLanding = false;
    heroP->wasFacingRight = heroP->facingRight;
    MMPlaySound(2, 9, 1);

    heroP->jumpHorizDelta = heroP->conveyorDelta + platformDelta;
    spriteP->drawData->horizMoveDelta += heroP->jumpHorizDelta;
    SetHeroJumpingFrame(spriteP);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameProc(spriteP, NULL);
    heroP->jumpStartBottom = spriteP->destFrameRect.bottom - heroP->wallInset.bottom - 1;
    SaveLastOnGroundPixelRow(spriteP);
    heroP->platformP = NULL;
}

/// Returns true if the hero landed back on the platform it jumped from.
Boolean EndJump(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);
    Boolean landedOnSamePlatform;

    heroP->isJumping = false;
    heroP->canJump = true;
    gKeys[kKeyJump] = gKeys[kKeyJumpLeft] = gKeys[kKeyJumpRight] = false;
    heroP->doorInset.top = heroP->normalDoorInsetTop;
    heroP->collisionInset.top = heroP->normalInsetTop;
    landedOnSamePlatform = heroP->jumpPlatformP != NULL && heroP->jumpPlatformP == heroP->platformP;
    heroP->platformP = NULL;
    heroP->jumpPlatformP = NULL;
    return landedOnSamePlatform;
}

/// Falling this many tiles or more kills the hero.
short getHeroFallingDeathDistance(SpritePtr spriteP)
{
    (void)spriteP;
    return ExtrasFallsAreSafe() ? 1000 : 7;
}

void HandleHeroLanding(SpritePtr spriteP, Boolean playLandingAnim, Boolean unused)
{
    HeroSpritePtr heroP = HERO(spriteP);
    short tilesFallen, deathDistance, sound;
    Boolean hurt;
    int fallen;

    (void)unused;
    fallen = (short)(SWGetSpriteVertLoc(spriteP) - heroP->lastGroundVertLoc) + 8;
    tilesFallen = (short)((fallen < 0 ? fallen + 15 : fallen) >> 4);
    deathDistance = getHeroFallingDeathDistance(spriteP);
    StatsNoteFall((float)(fallen - 8));
    heroP->jumpVertDelta = 0;

    if (tilesFallen >= deathDistance) {
        KillHeroForFallingTooFar(spriteP);
        gHeroLandedThisFrame = false;
        return;
    }

    if (playLandingAnim) {
        hurt = tilesFallen >= deathDistance - 2;
        DoHeroLandedFromJumpFrame(spriteP, hurt);
        if (hurt)
            gHeroLandedThisFrame = false;
        if (tilesFallen == deathDistance - 2) {
            heroP->animTimer = 15;
            heroP->jumpBufferFrames = 0;
        }
        else if (tilesFallen >= deathDistance - 1) {
            heroP->animTimer = 24;
            heroP->jumpBufferFrames = 0;
        }
    }

    if (tilesFallen <= deathDistance - 4)
        sound = 3;
    else if (tilesFallen <= deathDistance - 3)
        sound = 0x1d;
    else if (tilesFallen <= deathDistance - 2)
        sound = 0x1e;
    else
        sound = 0x1f;
    if (sound == 3 && FindSound(3) != -1)
        return;
    MMPlaySound(sound, 9, 1);
}

void HandleHeroQuestionMark(SpritePtr spriteP)
{
    SpritePtr questionMarkP = SWGetNextSprite(gQuestionMarkSpriteLayerP, NULL);

    if (!HERO(spriteP)->showQuestionMark) {
        if (questionMarkP != NULL)
            SWRemoveSpriteFromAnimation(questionMarkP, true);
    }
    else if (questionMarkP != NULL) {
        SWMoveSprite(questionMarkP, SWGetSpriteHorizLoc(spriteP) + 21.0f, SWGetSpriteVertLoc(spriteP) - 29.0f);
    }
}

/// The halo is a big dark sprite with a hole that follows the hero in dark rooms.
/// It shrinks to its normal size when created, then follows the hero, never
/// letting its edges show inside the 640x480 screen.
void UpdateHaloSpritePosition(SpritePtr spriteP)
{
    SpritePtr haloP;
    float scale;

    if (!gUseHaloSprite || IsCurRoomOutdoors())
        return;

    haloP = SWGetNextSprite(gHaloSpriteLayerP, NULL);
    if (haloP == NULL) {
        haloP = NewHaloSprite();
        if (haloP != NULL)
            SWCenterSpriteOverSprite(haloP, spriteP);
    }
    if (haloP == NULL)
        return;

    scale = haloP->drawData->scaledWidthFactor;
    if (haloP->miscFloat < scale) {
        scale += scale / -9.0f;
        if (scale < haloP->miscFloat)
            scale = haloP->miscFloat;
        SWSetSpriteScaledFactor(haloP, scale, scale);
        SWCenterSpriteOverSprite(haloP, spriteP);
    }
    else {
        CenterSpriteOverSpriteSmoothly(haloP, spriteP);
    }

    if (SWGetSpriteVertLoc(haloP) > 0)
        SWMoveSprite(haloP, SWGetSpriteHorizLoc(haloP), 0);
    if (SWGetSpriteVertLoc(haloP) + SWGetSpriteHeight(haloP) < 480.0f)
        SWMoveSprite(haloP, SWGetSpriteHorizLoc(haloP), 480.0f - SWGetSpriteHeight(haloP));
    if (SWGetSpriteHorizLoc(haloP) > 0)
        SWMoveSprite(haloP, 0, SWGetSpriteVertLoc(haloP));
    if (SWGetSpriteHorizLoc(haloP) + SWGetSpriteWidth(haloP) < 640.0f)
        SWMoveSprite(haloP, 640.0f - SWGetSpriteWidth(haloP), SWGetSpriteVertLoc(haloP));
}

/// Moves spriteP toward being centered over targetP, 1/15 of the way per frame
/// when it is far away.
void CenterSpriteOverSpriteSmoothly(SpritePtr spriteP, SpritePtr targetP)
{
    SWRect rect = {0, 0, 0, 0}, targetRect;
    float dh, dv;

    rect.right = (short)SWGetSpriteWidth(spriteP);
    rect.bottom = (short)SWGetSpriteHeight(spriteP);
    targetRect.top = (short)SWGetSpriteVertLoc(targetP);
    targetRect.left = (short)SWGetSpriteHorizLoc(targetP);
    targetRect.right = (short)(targetRect.left + SWGetSpriteWidth(targetP));
    targetRect.bottom = (short)(targetRect.top + SWGetSpriteHeight(targetP));
    SWCenterRect(&rect, &targetRect);

    dh = rect.left - SWGetSpriteHorizLoc(spriteP);
    dv = rect.top - SWGetSpriteVertLoc(spriteP);
    if (sqrtf(dv * dv + dh * dh) > 12.0f) {
        dh /= 15.0f;
        dv /= 15.0f;
    }
    SWOffsetSprite(spriteP, dh, dv);
}

Boolean IsCurRoomOutdoors(void)
{
    return gOldBackgroundPictResID != 1;
}

SpritePtr GetHeroSprite(void)
{
    SpritePtr heroP = SWGetNextSprite(gHeroSpriteLayerP, NULL);

    if (heroP == NULL)
        heroP = SWGetNextSprite(gHeroOnPlatformSpriteLayerP, NULL);
    return heroP;
}

// ---------------------------------------------------------------------------
// Spider webs (drawn as tiles)

/// Puts a web tile in the first empty layer (5 down to 4) at the spider's spot,
/// and removes webTileID from the tile below.
void LayWebForSpider(SpritePtr spriteP, short offsetH, short offsetV, short webTileID)
{
    short col = (short)((SWGetSpriteHorizLoc(spriteP) + offsetH) / 16.0f + 1.0f);
    short row = (short)((SWGetSpriteVertLoc(spriteP) + offsetV) / 16.0f);
    short layer;

    for (layer = 5; layer >= 4; layer--) {
        if (MyGetTile(gSpriteWorldP, layer, row, col) == -1) {
            MyDrawTile(gSpriteWorldP, layer, row, col, webTileID);
            break;
        }
    }
    for (layer = 4; layer < 6; layer++) {
        if (MyGetTile(gSpriteWorldP, layer, row + 1, col) == webTileID)
            MyDrawTile(gSpriteWorldP, layer, row + 1, col, -1);
    }
}

void EraseWebForSpider(SpritePtr spriteP, short offsetH, short offsetV, short webTileID)
{
    short col = (short)((SWGetSpriteHorizLoc(spriteP) + offsetH) / 16.0f + 1.0f);
    short row = (short)((SWGetSpriteVertLoc(spriteP) + offsetV) / 16.0f + 1.0f);
    short layer;

    for (layer = 0; layer < 7; layer++) {
        if (MyGetTile(gSpriteWorldP, layer, row, col) == webTileID)
            MyDrawTile(gSpriteWorldP, layer, row, col, -1);
    }
}

void LayWebUpToCeiling(SpritePtr spriteP, short offsetH, short offsetV, short webTileID)
{
    short col = (short)((SWGetSpriteHorizLoc(spriteP) + offsetH) / 16.0f + 1.0f);
    short row = (short)((SWGetSpriteVertLoc(spriteP) + offsetV) / 16.0f);
    short layer, tile;

    for (; row >= 0; row--) {
        for (layer = 5; layer >= 0; layer--) {
            tile = MyGetTile(gSpriteWorldP, layer, row, col);
            if (tile == -1) {
                MyDrawTile(gSpriteWorldP, layer, row, col, webTileID);
                break;
            }
            if (tile == webTileID)
                break;
        }
    }
}

#define kSmallSpiderWebTile 0x5a0

void DeleteWebForSmallSpider(SpritePtr spriteP)
{
    short col = (short)((SWGetSpriteHorizLoc(spriteP) + 8.0f) / 16.0f + 1.0f);
    short row = (short)((SWGetSpriteVertLoc(spriteP) + 4.0f) / 16.0f);
    short layer;
    Boolean found;

    for (; row >= 0; row--) {
        found = false;
        for (layer = 6; layer >= 0; layer--) {
            if (MyGetTile(gSpriteWorldP, layer, row, col) == kSmallSpiderWebTile) {
                MyDrawTile(gSpriteWorldP, layer, row, col, -1);
                found = true;
            }
        }
        if (!found)
            break;
    }
}

void MovePreyToSpiderLocation(SpritePtr preyP, SpritePtr spiderP)
{
    SWCenterSpriteOverSprite(preyP, spiderP);
    SWOffsetSprite(preyP, 0, preyP->userData == kWebTrappedHeroSprite ? 32.0f : 34.0f);
}

/// True if spriteP is below the big spider and less than distance away horizontally.
Boolean CheckBigSpiderWithSprite(SpritePtr spiderP, SpritePtr spriteP, short distance)
{
    if (GetDistBetweenSpritesHorizMid(spriteP, spiderP) < distance &&
        SWGetSpriteVertLoc(spiderP) + SWGetSpriteHeight(spriteP) / 2.0f < SWGetSpriteVertLoc(spriteP))
        return true;
    return false;
}

/// Now and then (1 in chance) drops a blood or goo drip from the sprite.
void MakeSpriteDripBloodOrGoo(SpritePtr spriteP, short chance, Boolean leavesSlime, short dripType)
{
    SpritePtr dripP;
    short offset;
    float horizLoc;

    if (gFamilyFriendly && dripType == kBloodDripSprite)
        return;
    if (!gFamilyFriendly && dripType == kGooDripSprite)
        dripType = kBloodDripSprite;

    horizLoc = SWGetSpriteHorizLoc(spriteP) + SWGetSpriteWidth(spriteP) / 2.0f;
    if (GetRandom(1, chance) < 2) {
        dripP = NewBloodOrGooDripSprite(dripType);
        if (dripP != NULL) {
            offset = (short)(SWGetSpriteWidth(spriteP) / 4.0f);
            offset = GetRandom(-offset, offset);
            SWSetSpriteLocation(dripP, (short)(offset + (short)horizLoc - 8),
                                (short)(spriteP->destFrameRect.bottom - 14));
            DRIP(dripP)->playsSplashSound = leavesSlime;
        }
    }
}

void MakeSkeletonDoUncollapse(SpritePtr spriteP)
{
    SkeletonSpritePtr skeletonP = SKELETON(spriteP);

    SW_ASSERT(spriteP->userData == kWalkingSkeletonSprite);
    if (skeletonP->hasUncollapsed)
        return;

    SWSetSpriteMoveProc(spriteP, NULL);
    SWSetSpriteFrameProc(spriteP, SkeletonUncollapseFrameProc);
    SWSetSpriteFrameRange(spriteP, 0, 7);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameTime(spriteP, 0);
    skeletonP->frameTimer = 6;
    skeletonP->frameDelay = 6;
    skeletonP->frameDirection = 1;
    skeletonP->hasUncollapsed = true;
}

void MakeZzzzFromJacksHead(SpritePtr spriteP)
{
    SpritePtr zzzzP = NewMiscSprite(kZzzzSprite);

    if (zzzzP == NULL)
        return;
    SWAddSprite(gTopSpriteLayerP, zzzzP);
    SWSetSpriteDrawProc(zzzzP, gTranslucentMaskDrawProc);
    SWSetSpriteMoveProc(zzzzP, ZzzzSpriteMoveProc);
    SWSetSpriteFrameTime(zzzzP, 16);
    SWSetSpriteFrameAdvance(zzzzP, 0);
    SWSetSpriteFrameProc(zzzzP, ZzzzFadeFrameProc);
    SWSetSpriteMoveDelta(zzzzP, 0.15f, -0.17f);
    SWSetSpriteVisible(zzzzP, false);
    SWSetSpriteLocation(zzzzP, SWGetSpriteHorizLoc(spriteP) + 30.0f, SWGetSpriteVertLoc(spriteP) + 5.0f);
}

void DeleteZzzzSprites(void)
{
    SpritePtr spriteP, nextP;

    for (spriteP = SWGetNextSprite(gTopSpriteLayerP, NULL); spriteP != NULL; spriteP = nextP) {
        nextP = SWGetNextSprite(gTopSpriteLayerP, spriteP);
        if (spriteP->userData == kZzzzSprite)
            SWRemoveSpriteFromAnimation(spriteP, true);
    }
}

/// Random numbers that replay identically when a recorded game movie is played.
float GetMovieCompatibleRandomFloat(float min, float max)
{
    return GetCustomRandomFloat(min, max);
}

short GetMovieCompatibleRandom(short min, short max)
{
    return (short)GetCustomRandom(min, max);
}

// ---------------------------------------------------------------------------
// Doors

/// The hero walked into a closed door: open it if Jack has the matching key
/// (or a master key). Gray doors only open with levers.
void OpenDoor(SpritePtr doorP)
{
    long type = doorP->userData;
    short item = -1;
    Boolean needsMasterKey = true;
    static const char *const kDoorMessages[] = {
        "Opened red door.\r", "Opened blue door.\r", "Opened green door.\r",
        "Opened purple door.\r", "Opened yellow door.\r"
    };

    if (SWGetSpriteMoveProc(doorP) != NULL)
        return;

    if (type >= 0x31 && type <= 0x34)
        item = kBlueKeyItem;
    else if (type >= 0x2d && type <= 0x30)
        item = kRedKeyItem;
    else if (type >= 0x35 && type <= 0x38)
        item = kGreenKeyItem;
    else if (type >= 0x3d && type <= 0x40)
        item = kYellowKeyItem;
    else if (type >= 0x39 && type <= 0x3c)
        item = kPurpleKeyItem;
    else if (type >= 0x41 && type <= 0x44)
        return;                         // gray door
    else
        SW_ASSERT(false);

    if (item >= 0 && gObjects.counts[item] > 0) {
        WriteCStringToFile(kDoorMessages[item]);
        RemoveItemFromStats(item);
        gObjects.counts[item]--;
        needsMasterKey = false;
    }

    if (needsMasterKey) {
        if (gObjects.counts[kMasterKeyItem] <= 0)
            return;
        RemoveItemFromStats(kMasterKeyItem);
        WriteCStringToFile("Used master key.\r");
        gObjects.counts[kMasterKeyItem]--;
    }
    MakeDoorOpen(doorP, true);
    MMPlayStereoSound(doorP, 1, 8, 1);
    StatsNoteKeyTurned();
    CoopPostDoorUnlocked(doorP);
}

void MakeDoorOpen(SpritePtr doorP, Boolean rememberIt)
{
    if (rememberIt)
        AddSpriteToObjectArray(doorP);
    SWSetSpriteMoveProc(doorP, DoorSpriteMoveProc);
    SWSetSpriteDrawProc(doorP, DoorSpriteDrawProc);
    SWCollideSpriteWithLayer(doorP, gDoorSpriteLayerP,
                             rememberIt ? OpenDoorCollideProc : OpenDoorButDontAddCollideProc);
}

void MakeDoorClose(SpritePtr doorP, Boolean rememberIt)
{
    SpritePtr partnerP = DOOR(doorP)->partnerDoorP;

    if (rememberIt) {
        RemoveSpriteFromObjectArray(doorP);
        if (partnerP != NULL)
            RemoveSpriteFromObjectArray(partnerP);
    }
    InitializeDoorMovementVariables(doorP);
    SWSetSpriteMoveProc(doorP, CloseDoorSpriteMoveProc);
    SWRemoveSprite(doorP);
    SWAddSprite(gDoorSpriteLayerP, doorP);
    if (partnerP != NULL) {
        InitializeDoorMovementVariables(partnerP);
        SWSetSpriteMoveProc(partnerP, CloseDoorSpriteMoveProc);
        SWRemoveSprite(partnerP);
        SWAddSprite(gDoorSpriteLayerP, partnerP);
    }
}

/// Collide procs used to find the other half of a double door (the door with the
/// same left edge, or the same top edge for trap doors).
void OpenDoorCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    (void)sectRect;
    if (srcSpriteP->destFrameRect.left == dstSpriteP->destFrameRect.left) {
        SWSetSpriteMoveProc(dstSpriteP, DoorSpriteMoveProc);
        SWSetSpriteDrawProc(dstSpriteP, DoorSpriteDrawProc);
        AddSpriteToObjectArray(dstSpriteP);
        DOOR(srcSpriteP)->partnerDoorP = dstSpriteP;
        DOOR(dstSpriteP)->partnerDoorP = srcSpriteP;
    }
}

void OpenDoorInstantlyCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    (void)sectRect;
    if (srcSpriteP->destFrameRect.left == dstSpriteP->destFrameRect.left && !DOOR(dstSpriteP)->isOpen) {
        SetDoorToOpenedState(dstSpriteP);
        DOOR(srcSpriteP)->partnerDoorP = dstSpriteP;
        DOOR(dstSpriteP)->partnerDoorP = srcSpriteP;
    }
}

void OpenDoorInstantlyAndAddCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    (void)sectRect;
    if (srcSpriteP->destFrameRect.left == dstSpriteP->destFrameRect.left && !DOOR(dstSpriteP)->isOpen) {
        SetDoorToOpenedState(dstSpriteP);
        AddSpriteToObjectArray(dstSpriteP);
        AddSpriteToDoOnceArray(dstSpriteP);
        DOOR(srcSpriteP)->partnerDoorP = dstSpriteP;
        DOOR(dstSpriteP)->partnerDoorP = srcSpriteP;
    }
}

void CloseDoorInstantlyCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    (void)sectRect;
    if (srcSpriteP->destFrameRect.left == dstSpriteP->destFrameRect.left && DOOR(dstSpriteP)->isOpen) {
        SetDoorToClosedState(dstSpriteP);
        DOOR(srcSpriteP)->partnerDoorP = dstSpriteP;
        DOOR(dstSpriteP)->partnerDoorP = srcSpriteP;
    }
}

void OpenDoorButDontAddCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    (void)sectRect;
    if (srcSpriteP->destFrameRect.left == dstSpriteP->destFrameRect.left) {
        SWSetSpriteMoveProc(dstSpriteP, DoorSpriteMoveProc);
        SWSetSpriteDrawProc(dstSpriteP, DoorSpriteDrawProc);
        DOOR(srcSpriteP)->partnerDoorP = dstSpriteP;
        DOOR(dstSpriteP)->partnerDoorP = srcSpriteP;
    }
}

void MakeTrapDoorOpen(SpritePtr trapDoorP)
{
    AddSpriteToObjectArray(trapDoorP);
    SWRemoveSprite(trapDoorP);
    SWAddSprite(gOpenTrapDoorSpriteLayerP, trapDoorP);
    SWSetSpriteFrameProc(trapDoorP, TrapDoorFrameProc);
    SWSetSpriteFrameTime(trapDoorP, 100);
}

void OpenTrapDoorCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    (void)sectRect;
    if (srcSpriteP->destFrameRect.top == dstSpriteP->destFrameRect.top)
        MakeTrapDoorOpen(dstSpriteP);
}

void OpenTrapDoorInstantlyCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    (void)sectRect;
    if (srcSpriteP->destFrameRect.top == dstSpriteP->destFrameRect.top) {
        SWSetCurrentFrameIndex(dstSpriteP, srcSpriteP->maxFrames - 1);
        SWRemoveSprite(dstSpriteP);
        SWAddSprite(gOpenTrapDoorSpriteLayerP, dstSpriteP);
    }
}

void SetDoorToOpenedState(SpritePtr spriteP)
{
    DoorSpritePtr doorP = DOOR(spriteP);

    SW_ASSERT(spriteP->userData >= kFirstDoorSprite);
    SW_ASSERT(spriteP->userData <= kLastDoorSprite);
    if (!doorP->opensUpward) {
        spriteP->destFrameRect.top += doorP->openDistance;
        doorP->topInset += doorP->openDistance;
    }
    else {
        spriteP->destFrameRect.bottom -= doorP->openDistance;
        doorP->bottomInset -= doorP->openDistance;
    }
    SWRemoveSprite(spriteP);
    SWAddSprite(gOpenedDoorSpriteLayerP, spriteP);
    SWSetSpriteMoveProc(spriteP, NULL);
    doorP->isOpen = true;
}

void SetDoorToClosedState(SpritePtr spriteP)
{
    DoorSpritePtr doorP = DOOR(spriteP);

    SW_ASSERT(spriteP->userData >= kFirstDoorSprite);
    SW_ASSERT(spriteP->userData <= kLastDoorSprite);
    InitializeDoorMovementVariables(spriteP);
    if (!doorP->opensUpward) {
        spriteP->destFrameRect.top -= doorP->openDistance;
        doorP->topInset -= doorP->openDistance;
    }
    else {
        spriteP->destFrameRect.bottom += doorP->openDistance;
        doorP->bottomInset += doorP->openDistance;
    }
    SWRemoveSprite(spriteP);
    SWAddSprite(gDoorSpriteLayerP, spriteP);
    SWSetSpriteMoveProc(spriteP, NULL);
    doorP->isOpen = false;
}

void SetTrapDoorToOpenedState(SpritePtr spriteP, Boolean includeOtherHalf)
{
    if (includeOtherHalf)
        SWCollideSpriteWithLayer(spriteP, gTrapDoorSpriteLayerP, OpenTrapDoorInstantlyCollideProc);
    SWSetCurrentFrameIndex(spriteP, spriteP->maxFrames - 1);
    SWRemoveSprite(spriteP);
    SWAddSprite(gOpenTrapDoorSpriteLayerP, spriteP);
}

// ---------------------------------------------------------------------------
// Save pedestals

/// The hero stepped onto a save pedestal: if centered on it, turn into a statue
/// (the save animation) and remember the pedestal.
void SaveHeroOnPedestal(SpritePtr spriteP, SpritePtr pedestalP)
{
    short offset;

    if (gUsingEditor || !PEDESTAL(pedestalP)->isActive)
        return;
    if (IsSpriteInObjectArray(pedestalP) || IsSpriteInDoOnceArray(pedestalP))
        return;

    offset = (short)(SWGetSpriteWidth(spriteP) / 2.0f + SWGetSpriteHorizLoc(spriteP)) -
             (short)(SWGetSpriteWidth(pedestalP) / 2.0f + SWGetSpriteHorizLoc(pedestalP));
    if (offset > -6 && offset < 6) {
        gLastSavePedestalObjectID = GAME_SPRITE(pedestalP)->objectID;
        CreateColorAndGrayStatues((short)(SWGetSpriteHorizLoc(spriteP) - offset),
                                  (short)SWGetSpriteVertLoc(spriteP), true);
        SWSetSpriteVisible(spriteP, false);
        MMPauseGameTimer();
        PauseSpriteLayersForSaveAnim();
        AddSpriteToObjectArray(pedestalP);
        PEDESTAL(pedestalP)->isActive = false;
    }
}

void StopLoopingSounds(void)
{
    StopChannel(1);
    StopChannel(0);
    StopChannel(3);
    StopChannel(4);
    StopChannel(2);
    StopChannel(5);
    StopChannel(6);
}

/// A colored statue of Jack and a gray one on top of it. When saving, the gray
/// one fades in (Jack turns to stone); when restoring, it fades out.
void CreateColorAndGrayStatues(short h, short v, Boolean isSaving)
{
    SpritePtr colorP, grayP, pedestalP;

    colorP = NewStatueSprite();
    if (colorP != NULL) {
        SWSetSpriteLocation(colorP, h, v);
        SWSetCurrentFrameIndex(colorP, isSaving ? 1 : 0);
    }
    grayP = NewStatueSprite();
    if (grayP != NULL) {
        SWSetSpriteLocation(grayP, h, v);
        SWSetCurrentFrameIndex(grayP, isSaving ? 0 : 1);
        SWSetSpriteFrameAdvance(grayP, 0);
        SWSetSpriteDrawProc(grayP, gTranslucentMaskDrawProc);
        UpdateHaloSpritePosition(grayP);
        SWSetSpriteFrameProc(grayP, PauseThenFadeStatueFrameProc);
        SWSetSpriteFrameTime(grayP, 0);
    }
    STATUE(colorP)->otherStatueP = grayP;
    STATUE(grayP)->otherStatueP = colorP;

    for (pedestalP = SWGetNextSprite(gSavePedestalSpriteLayerP, NULL); pedestalP != NULL;
         pedestalP = SWGetNextSprite(gSavePedestalSpriteLayerP, pedestalP)) {
        if (GAME_SPRITE(pedestalP)->objectID == gLastSavePedestalObjectID) {
            STATUE(colorP)->pedestalP = pedestalP;
            STATUE(grayP)->pedestalP = pedestalP;
            return;
        }
        WriteCStringToFile("ERROR! gLastSavePedestalObjectID NOT found in current room! Most likely ObjectIDs "
                           "were reset, or the pedestal was deleted and recreated, in this mansion, since this "
                           "saved game was created. An assertion error will be caused this this.\r");
        WriteCStringToFile("gLastSavePedestalObjectID: ");
        WriteNumberToFile(gLastSavePedestalObjectID);
        WriteCarriageReturn(1);
        WriteCStringToFile("((BackgroundStructPtr)pedestalSpriteP)->objectID: ");
        WriteNumberToFile(GAME_SPRITE(pedestalP)->objectID);
        WriteCarriageReturn(1);
    }
}

void PauseSpriteLayersForSaveAnim(void)
{
    gLayersArePausedForSaveAnim = true;
    PauseAllSpriteLayers(gSpriteWorldP);
    SWUnpauseSpriteLayer(gStatueSpriteLayerP);
    SWUnpauseSpriteLayer(gBottomCircleOfLightSpriteLayerP);
    SWUnpauseSpriteLayer(gTopCircleOfLightSpriteLayerP);
}

// ---------------------------------------------------------------------------
// Ladders, vines and poles

/// Clamps a rect (in pixels) to tile rows/cols of tileMapP. Returns false if it
/// lies completely outside the map.
static Boolean RectToTileRange(SpriteWorldPtr worldP, TileMapStructPtr tileMapP, SWRect *rectP,
                               short *startRow, short *endRow, short *startCol, short *endCol)
{
    SWRect r = *rectP;

    if (r.left < 0) r.left -= worldP->tileWidth;
    if (r.right < 1) r.right -= worldP->tileWidth;
    if (r.top < 0) r.top -= worldP->tileHeight;
    if (r.bottom < 1) r.bottom -= worldP->tileHeight;

    *startRow = r.top / worldP->tileHeight;
    *endRow = (short)((r.bottom - 1) / worldP->tileHeight);
    *startCol = r.left / worldP->tileWidth;
    *endCol = (short)((r.right - 1) / worldP->tileWidth);

    if (*endRow < 0)
        return false;
    if (*endRow >= tileMapP->numRows)
        *endRow = tileMapP->numRows - 1;
    if (*startRow < 0)
        *startRow = 0;
    else if (*startRow >= tileMapP->numRows)
        return false;
    if (*endCol < 0)
        return false;
    if (*endCol >= tileMapP->numCols)
        *endCol = tileMapP->numCols - 1;
    if (*startCol < 0)
        *startCol = 0;
    else if (*startCol >= tileMapP->numCols)
        return false;
    return true;
}

/// Looks for a ladder (or vine) tile in [firstTileID, lastTileID] in tileLayer
/// under the sprite's rect, reduced by insetP. On success, ladderRect is the
/// ladder's two-tile-wide column. When there are two ladders, the closer one wins.
Boolean GetLadderRect(SpriteWorldPtr worldP, SpritePtr spriteP, SWRect *insetP, SWRect *ladderRect,
                      short tileLayer, short firstTileID, short lastTileID)
{
    TileMapStructPtr tileMapP;
    SWRect spriteRect, otherRect;
    short startRow, endRow, startCol, endCol, row, col, tileID, midRow, midCol;
    short spriteMid, foundMid, otherMid;
    Boolean found = false, foundOther;

    SW_ASSERT(worldP != NULL && worldP->tilingIsInitialized);
    SW_ASSERT(worldP->tileLayerArray[tileLayer] != NULL);
    SW_ASSERT(spriteP != NULL);

    tileMapP = worldP->tileLayerArray[tileLayer];
    spriteRect = spriteP->destFrameRect;
    if (insetP != NULL) {
        spriteRect.top += insetP->top;
        spriteRect.left += insetP->left;
        spriteRect.bottom -= insetP->bottom;
        spriteRect.right -= insetP->right;
    }
    if (!RectToTileRange(worldP, tileMapP, &spriteRect, &startRow, &endRow, &startCol, &endCol))
        return false;

    midRow = (short)((endRow - startRow) / 2) + startRow;
    midCol = (short)((endCol - startCol) / 2) + startCol;
    tileID = GetTileID(tileMapP, midRow, midCol);
    if (tileID >= firstTileID && tileID <= lastTileID) {
        GetLadderRectFromTile(worldP, ladderRect, &spriteRect, tileID, midCol);
        return true;
    }

        // first ladder tile from the left...
    for (row = startRow; row <= endRow && !found; row++) {
        for (col = startCol; col <= endCol; col++) {
            tileID = GetTileID(tileMapP, row, col);
            if (tileID >= firstTileID && tileID <= lastTileID) {
                found = true;
                break;
            }
        }
    }
    if (!found)
        return false;
    GetLadderRectFromTile(worldP, ladderRect, &spriteRect, tileID, col);

        // ...and from the right; use whichever is closer to the sprite's middle
    foundOther = false;
    for (row = startRow; row <= endRow && !foundOther; row++) {
        for (col = endCol; col >= startCol; col--) {
            tileID = GetTileID(tileMapP, row, col);
            if (tileID >= firstTileID && tileID <= lastTileID) {
                foundOther = true;
                break;
            }
        }
    }
    if (foundOther) {
        GetLadderRectFromTile(worldP, &otherRect, &spriteRect, tileID, col);
        spriteMid = spriteRect.left + (short)((spriteRect.right - spriteRect.left) / 2);
        foundMid = ladderRect->left + (short)((ladderRect->right - ladderRect->left) / 2);
        otherMid = otherRect.left + (short)((otherRect.right - otherRect.left) / 2);
        foundMid = abs(foundMid - spriteMid);
        otherMid = abs(otherMid - spriteMid);
        if (otherMid < foundMid)
            *ladderRect = otherRect;
    }
    return true;
}

/// Ladder tiles are 2 wide; even tiles (counting from 0x1f9) are the left half.
void GetLadderRectFromTile(SpriteWorldPtr worldP, SWRect *ladderRect, SWRect *spriteRect, short tileID, short col)
{
    if (((tileID - 0x1f9) & 1) == 0) {
        ladderRect->left = worldP->tileWidth * col;
        ladderRect->right = worldP->tileWidth * (col + 2);
    }
    else {
        ladderRect->left = worldP->tileWidth * (col - 1);
        ladderRect->right = worldP->tileWidth * (col + 1);
    }
    ladderRect->top = spriteRect->top;
    ladderRect->bottom = spriteRect->bottom;
}

/// Like GetLadderRect for poles, which are one tile wide.
Boolean GetPoleRect(SpriteWorldPtr worldP, SpritePtr spriteP, SWRect *insetP, SWRect *poleRect,
                    short tileLayer, short firstTileID, short lastTileID)
{
    TileMapStructPtr tileMapP;
    SWRect spriteRect;
    short startRow, endRow, startCol, endCol, row, col, tileID, midCol;

    SW_ASSERT(worldP != NULL && worldP->tilingIsInitialized);
    SW_ASSERT(worldP->tileLayerArray[tileLayer] != NULL);
    SW_ASSERT(spriteP != NULL);

    tileMapP = worldP->tileLayerArray[tileLayer];
    spriteRect = spriteP->destFrameRect;
    if (insetP != NULL) {
        spriteRect.top += insetP->top;
        spriteRect.left += insetP->left;
        spriteRect.bottom -= insetP->bottom;
        spriteRect.right -= insetP->right;
    }
    if (!RectToTileRange(worldP, tileMapP, &spriteRect, &startRow, &endRow, &startCol, &endCol))
        return false;

    midCol = (short)((endCol - startCol) / 2) + startCol;
    tileID = GetTileID(tileMapP, (short)((endRow - startRow) / 2) + startRow, midCol);
    if (tileID >= firstTileID && tileID <= lastTileID) {
        col = midCol;
    }
    else {
        for (row = startRow; row <= endRow; row++) {
            for (col = startCol; col <= endCol; col++) {
                tileID = GetTileID(tileMapP, row, col);
                if (tileID >= firstTileID && tileID <= lastTileID)
                    goto found;
            }
        }
        return false;
    }
found:
    poleRect->top = spriteRect.top;
    poleRect->bottom = spriteRect.bottom;
    poleRect->left = worldP->tileWidth * col;
    poleRect->right = worldP->tileWidth * (col + 1);
    return true;
}

// ---------------------------------------------------------------------------
// Walls and floors

/// Walls, conveyor belts, active gold/silver floors, solid disappearing floors,
/// and (only from above) the tops of vines.
Boolean IsTileFloorTile(short tileID, Boolean includeDisappearingFloors)
{
    if (IsTileHorizWall(tileID))
        return true;
    if (includeDisappearingFloors && gDisappearingFloorMode != 3 &&
        tileID >= kFirstDisappearingFloorTile && tileID <= kLastDisappearingFloorTile)
        return true;
    return tileID >= kFirstVineTopTile && tileID <= kLastVineTopTile;
}

Boolean IsTileFloorNotVineTile(short tileID)
{
    if (IsTileHorizWall(tileID))
        return true;
    return gDisappearingFloorMode != 3 &&
           tileID >= kFirstDisappearingFloorTile && tileID <= kLastDisappearingFloorTile;
}

Boolean IsTileHorizWall(short tileID)
{
    short id = ConvertTileID(tileID);

    return (id >= kFirstWallTile && id <= kLastWallTile) ||
           (gSilverFloorIsActive && id >= kFirstSilverFloorTile && id <= kLastSilverFloorTile) ||
           (gGoldFloorIsActive && id >= kFirstGoldFloorTile && id <= kLastGoldFloorTile) ||
           (id >= kFirstLeftConveyorTile && id <= kLastLeftConveyorTile) ||
           (id >= kFirstRightConveyorTile && id <= kLastRightConveyorTile);
}

/// A wall tile with another wall tile directly above or below it.
Boolean IsTileThickHorizWall(TileMapStructPtr tileMapP, short row, short col)
{
    if (!IsTileHorizWall(GetTileID(tileMapP, row, col)))
        return false;
    return IsTileHorizWall(GetTileID(tileMapP, row - 1, col)) ||
           IsTileHorizWall(GetTileID(tileMapP, row + 1, col));
}

/**
 * Checks the sprite against walls, floors and solid sprites on one side.
 * insetP is the sprite's inset for this check; for the hero, its collision and
 * door insets are adjusted by the same amount it differs from wallInset.
 * isJumping is passed on to the tile check; with oneAbove, the bottom is checked
 * one pixel higher and tiles only when standing exactly on a tile edge.
 */
Boolean CheckForWall(SpritePtr spriteP, SWTileSearchType direction, SWRect *insetP, Boolean isJumping,
                     Boolean oneAbove, Boolean fixPosition)
{
    HeroSpritePtr heroP;
    SWRect tileInset, collisionInset, doorInset;
    Boolean onPlatform, onTileEdge, hitTiles = false, hitSprites;
    short bottom;

    SW_ASSERT(insetP != NULL);
    tileInset = *insetP;

    if (spriteP->userData == kHeroSprite) {
        heroP = HERO(spriteP);
        onPlatform = heroP->platformP != NULL;
        collisionInset = heroP->collisionInset;
        doorInset = heroP->doorInset;
        if (spriteP->curFrameIndex == 49) {
            tileInset.top = 40;
            collisionInset.top = 40;
            doorInset.top = 40;
        }
        if (!SWEqualRect(insetP, &heroP->wallInset)) {
            short dLeft = insetP->left - heroP->wallInset.left;
            short dTop = insetP->top - heroP->wallInset.top;
            short dRight = insetP->right - heroP->wallInset.right;
            short dBottom = insetP->bottom - heroP->wallInset.bottom;

            collisionInset.top += dTop;
            collisionInset.left += dLeft;
            collisionInset.bottom += dBottom;
            collisionInset.right += dRight;
            doorInset.top += dTop;
            doorInset.left += dLeft;
            doorInset.bottom += dBottom;
            doorInset.right += dRight;
        }
    }
    else {
        onPlatform = false;
        collisionInset = *insetP;
        doorInset = *insetP;
    }

    bottom = spriteP->destFrameRect.bottom - collisionInset.bottom;
    onTileEdge = (FloorDiv16(bottom) << 4) == bottom;
    if (oneAbove) {
        tileInset.bottom--;
        collisionInset.bottom--;
        doorInset.bottom--;
    }

    if (!onPlatform) {
        if (onTileEdge || !oneAbove)
            hitTiles = CheckForWallWithTiles(spriteP, direction, fixPosition, isJumping, &tileInset, &collisionInset);
        hitSprites = CheckForWallWithSprites(spriteP, direction, fixPosition, &collisionInset, &doorInset);
    }
    else {
        hitSprites = CheckForWallWithSprites(spriteP, direction, fixPosition, &collisionInset, &doorInset);
        if (onTileEdge || !oneAbove)
            hitTiles = CheckForWallWithTiles(spriteP, direction, fixPosition, isJumping, &tileInset, &collisionInset);
    }
    return hitTiles || hitSprites;
}

Boolean CheckForWallWithTiles(SpritePtr spriteP, SWTileSearchType direction, Boolean fixPosition,
                              Boolean isJumping, SWRect *tileInsetP, SWRect *collisionInsetP)
{
    Boolean walls, silver = false, gold = false, leftConveyor, rightConveyor;
    Boolean disappearing = false, lightSaber = false, vineTop = false;
    SWRect vineInset;

    (void)isJumping;
    walls = MyCheckSpriteWithWallTiles(gSpriteWorldP, spriteP, direction, collisionInsetP, 4, 4,
                                       kFirstWallTile, kLastWallTile, fixPosition);
    if (gSilverFloorIsActive)
        silver = MyCheckSpriteWithWallTiles(gSpriteWorldP, spriteP, direction, collisionInsetP, 4, 4,
                                            kFirstSilverFloorTile, kLastSilverFloorTile, fixPosition);
    if (gGoldFloorIsActive)
        gold = MyCheckSpriteWithWallTiles(gSpriteWorldP, spriteP, direction, collisionInsetP, 4, 4,
                                          kFirstGoldFloorTile, kLastGoldFloorTile, fixPosition);
    leftConveyor = MyCheckSpriteWithWallTiles(gSpriteWorldP, spriteP, direction, collisionInsetP, 4, 4,
                                              kFirstLeftConveyorTile, kLastLeftConveyorTile, fixPosition);
    rightConveyor = MyCheckSpriteWithWallTiles(gSpriteWorldP, spriteP, direction, collisionInsetP, 4, 4,
                                               kFirstRightConveyorTile, kLastRightConveyorTile, fixPosition);
    if (spriteP->userData == kHeroSprite && (leftConveyor || rightConveyor))
        gHeroIsOnConveyorBelt = true;

    if (direction == kSW_CheckBottom) {
        vineInset = *tileInsetP;
        vineInset.left += 10;
        vineInset.right += 4;
        if (gDisappearingFloorMode != 3)
            disappearing = MyCheckSpriteWithWallTiles(gSpriteWorldP, spriteP, kSW_CheckBottom, tileInsetP, 4, 4,
                                                      kFirstDisappearingFloorTile, kLastDisappearingFloorTile,
                                                      fixPosition);
        if (gLightSaberFloorMode != 3)
            lightSaber = MyCheckSpriteWithWallTiles(gSpriteWorldP, spriteP, kSW_CheckBottom, tileInsetP, 4, 4,
                                                    kFirstLightSaberFloorTile, kLastLightSaberFloorTile,
                                                    fixPosition);
        vineTop = MyCheckSpriteWithWallTiles(gSpriteWorldP, spriteP, kSW_CheckBottom, &vineInset, 4, 5,
                                             kFirstVineTopTile, kLastVineTopTile, fixPosition);
    }
    return walls || leftConveyor || rightConveyor || disappearing || lightSaber || vineTop || silver || gold;
}

/// Doors, platforms (from above), trap doors, brick switches and save pedestals.
/// Also records which platform or pedestal the hero is standing on.
Boolean CheckForWallWithSprites(SpritePtr spriteP, SWTileSearchType direction, Boolean fixPosition,
                                SWRect *collisionInsetP, SWRect *doorInsetP)
{
    Boolean doors, vertPlatform = false, horizPlatform = false, trapDoor = false, brickSwitch = false;
    Boolean pedestal, isHero = spriteP->userData == kHeroSprite;
    SpritePtr hitP = NULL, pedestalP = NULL;
    long type;

    doors = SWCheckSpriteWithSprites(spriteP, NULL, direction, doorInsetP, gDoorSpriteLayerP,
                                     gDoorSpriteLayerP, false, true, false, fixPosition);

    if (direction == kSW_CheckBottom && spriteP->userData != kEyeballSprite &&
        spriteP->userData != kBigSpiderSprite && spriteP->userData != kSmallSpiderSprite) {
        vertPlatform = SWCheckSpriteWithSprites(spriteP, &hitP, kSW_CheckBottom, collisionInsetP,
                                                gVertMovingPlatformSpriteLayerP, gVertMovingPlatformSpriteLayerP,
                                                false, true, false, fixPosition);
        if (!vertPlatform)
            vertPlatform = SWCheckSpriteWithSprites(spriteP, &hitP, kSW_CheckBottom, collisionInsetP,
                                                    gFallingPlatformSpriteLayerP, gFallingPlatformSpriteLayerP,
                                                    false, true, false, fixPosition);
        if (vertPlatform && isHero && !HERO(spriteP)->isDead) {
            HERO(spriteP)->platformP = hitP;
            type = hitP->userData;
            if (type >= kFirstFallingPlatformSprite && type <= kLastFallingPlatformSprite &&
                SWGetSpriteMoveProc(hitP) == NULL) {
                CoopPostPlatformFalling(hitP);
                PLATFORM(hitP)->fallDelay = 30;
                SWSetSpriteMoveProc(hitP, FallingPlatformMoveProc);
                if (type >= 0x5c && type <= 0x5e) {          // skull platforms crumble
                    SWSetSpriteFrameProc(hitP, SkullFallingPlatformFrameProc);
                    SWSetSpriteFrameAdvance(hitP, 1);
                    SWSetSpriteFrameTime(hitP, 100);
                }
            }
        }

        horizPlatform = SWCheckSpriteWithSprites(spriteP, &hitP, kSW_CheckBottom, collisionInsetP,
                                                 gHorizMovingPlatformSpriteLayerP, gHorizMovingPlatformSpriteLayerP,
                                                 false, true, false, fixPosition);
        if (horizPlatform && isHero)
            HERO(spriteP)->platformP = hitP;
        if (isHero && !vertPlatform && !horizPlatform)
            HERO(spriteP)->platformP = NULL;
    }

    if (direction == kSW_CheckBottom) {
        trapDoor = SWCheckSpriteWithSprites(spriteP, NULL, kSW_CheckBottom, collisionInsetP, gTrapDoorSpriteLayerP,
                                            gTrapDoorSpriteLayerP, false, true, false, fixPosition);
        brickSwitch = SWCheckSpriteWithSprites(spriteP, NULL, kSW_CheckBottom, collisionInsetP,
                                               gBrickSwitchSpriteLayerP, gBrickSwitchSpriteLayerP,
                                               false, true, false, fixPosition);
    }
    pedestal = SWCheckSpriteWithSprites(spriteP, &pedestalP, direction, collisionInsetP, gSavePedestalSpriteLayerP,
                                        gSavePedestalSpriteLayerP, false, true, false, fixPosition);

    if (!(doors || vertPlatform || trapDoor || brickSwitch || pedestal || horizPlatform))
        return false;
    if (pedestal && direction == kSW_CheckBottom && isHero)
        HERO(spriteP)->pedestalP = pedestalP;
    return true;
}

/**
 * SWCheckSpriteWithTiles for walls: looks for tiles in [firstTileID, lastTileID]
 * in layers startLayer..endLayer that the sprite moved into on the given side.
 * A floor tile only counts if the tile on the near side of it isn't a floor too
 * (so only surfaces count), and a wall only if the next tile over isn't a floor.
 * For the hero, thin floors don't block sideways movement except at its feet,
 * and while jumping it can jump up past the end of a thin floor.
 * With fixPosition, the sprite is pushed back out of the tile.
 */
Boolean MyCheckSpriteWithWallTiles(SpriteWorldPtr worldP, SpritePtr spriteP, SWTileSearchType direction,
                                   SWRect *insetP, short startLayer, short endLayer, short firstTileID,
                                   short lastTileID, Boolean fixPosition)
{
    SWRect oldRect, destRect, doorInset;
    TileMapStructPtr tileMapP;
    short startRow, endRow, startCol, endCol, rowStep, colStep, row = 0, col = 0, layer, tileID;
    short sideStep, nearRow, maxRow, deltaBottom, scanCol, r;
    Boolean found = false, movedInDirection = true, rowsLeft, colsLeft, isFloor, counts, allThick;
    HeroSpritePtr heroP;

    SW_ASSERT(worldP != NULL && worldP->tilingIsInitialized);
    SW_ASSERT(startLayer >= 0 && endLayer <= 9);
    SW_ASSERT(spriteP != NULL);

    oldRect = spriteP->oldFrameRect;
    destRect = spriteP->destFrameRect;
    sideStep = direction == kSW_CheckRight ? -1 : direction == kSW_CheckLeft ? 1 : 0;

    if (insetP != NULL) {
        oldRect.top += insetP->top;
        oldRect.left += insetP->left;
        oldRect.bottom -= insetP->bottom;
        oldRect.right -= insetP->right;
        destRect.top += insetP->top;
        destRect.left += insetP->left;
        destRect.bottom -= insetP->bottom;
        destRect.right -= insetP->right;
    }

    switch (direction) {
        case kSW_CheckTop:
            startRow = oldRect.top / worldP->tileHeight;
            endRow = destRect.top / worldP->tileHeight;
            startCol = destRect.left / worldP->tileWidth;
            endCol = (short)((destRect.right - 1) / worldP->tileWidth);
            if (fixPosition)
                startRow--;
            if (startRow < endRow) {
                movedInDirection = false;
                startRow = endRow;
            }
            break;
        case kSW_CheckBottom:
            startRow = (short)((oldRect.bottom - 1) / worldP->tileHeight);
            endRow = (short)((destRect.bottom - 1) / worldP->tileHeight);
            startCol = destRect.left / worldP->tileWidth;
            endCol = (short)((destRect.right - 1) / worldP->tileWidth);
            if (fixPosition)
                startRow++;
            if (endRow < startRow) {
                movedInDirection = false;
                startRow = endRow;
            }
            break;
        case kSW_CheckRight:
            startCol = (short)((oldRect.right - 1) / worldP->tileWidth);
            endCol = (short)((destRect.right - 1) / worldP->tileWidth);
            startRow = destRect.top / worldP->tileHeight;
            endRow = (short)((destRect.bottom - 1) / worldP->tileHeight);
            if (fixPosition)
                startCol++;
            if (endCol < startCol) {
                movedInDirection = false;
                startCol = endCol;
            }
            break;
        case kSW_CheckLeft:
            startCol = oldRect.left / worldP->tileWidth;
            endCol = destRect.left / worldP->tileWidth;
            startRow = destRect.top / worldP->tileHeight;
            endRow = (short)((destRect.bottom - 1) / worldP->tileHeight);
            if (fixPosition)
                startCol--;
            if (startCol < endCol) {
                movedInDirection = false;
                startCol = endCol;
            }
            break;
        default:
            startRow = destRect.top / worldP->tileHeight;
            endRow = (short)((destRect.bottom - 1) / worldP->tileHeight);
            startCol = destRect.left / worldP->tileWidth;
            endCol = (short)((destRect.right - 1) / worldP->tileWidth);
            break;
    }
    rowStep = endRow < startRow ? -1 : 1;
    colStep = endCol < startCol ? -1 : 1;

    if (movedInDirection) {
        if (direction == kSW_CheckTop || direction == kSW_CheckBottom || direction == kSW_CheckAll) {
            nearRow = direction == kSW_CheckBottom ? -1 : direction == kSW_CheckTop ? 1 : 0;
            for (layer = startLayer; layer <= endLayer && !found; layer++) {
                tileMapP = worldP->tileLayerArray[layer];
                if (tileMapP == NULL)
                    continue;
                for (row = startRow, rowsLeft = true; rowsLeft && !found; row += rowStep) {
                    rowsLeft = row != endRow;
                    for (col = startCol, colsLeft = true; colsLeft; col += colStep) {
                        colsLeft = col != endCol;
                        tileID = GetTileID(tileMapP, row, col);
                        if (tileID < firstTileID || tileID > lastTileID)
                            continue;
                        isFloor = IsTileFloorTile(GetTileID(tileMapP, row + nearRow, col), false);
                        if (!isFloor && layer == 5 && firstTileID > 0x1fa && lastTileID < 0x1fd)
                            isFloor = IsTileFloorTile(GetTileID(worldP->tileLayerArray[4], row + nearRow, col), false);
                        if (!isFloor) {
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        break;
                }
            }
        }
        else {
            for (layer = startLayer; layer <= endLayer && !found; layer++) {
                tileMapP = worldP->tileLayerArray[layer];
                if (tileMapP == NULL)
                    continue;
                for (col = startCol, colsLeft = true; colsLeft && !found; col += colStep) {
                    colsLeft = col != endCol;
                    for (row = startRow, rowsLeft = true; rowsLeft; row += rowStep) {
                        rowsLeft = row != endRow;
                        deltaBottom = spriteP->destFrameRect.bottom - spriteP->oldFrameRect.bottom;
                        tileID = GetTileID(tileMapP, row, col);
                        counts = true;
                        if (spriteP->userData == kHeroSprite && !IsTileThickHorizWall(tileMapP, row, col)) {
                            maxRow = endRow < startRow ? startRow : endRow;
                            counts = maxRow == row &&
                                     (abs(deltaBottom) > 5 || deltaBottom == 0 || !HERO(spriteP)->isJumping);
                        }
                        if (tileID >= firstTileID && tileID <= lastTileID && counts &&
                            !IsTileHorizWall(GetTileID(tileMapP, row, col + sideStep))) {
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        break;
                }
            }
        }
    }

    if (spriteP->userData == kHeroSprite) {
        heroP = HERO(spriteP);
        allThick = true;
        if (direction == kSW_CheckRight || direction == kSW_CheckLeft) {
                // is the hero's bottom row (or the one above) a solid wall?
            tileMapP = worldP->tileLayerArray[startLayer];
            SW_ASSERT(tileMapP != NULL);
            SW_ASSERT(startRow <= endRow);
            for (scanCol = startCol, colsLeft = true; colsLeft; scanCol += colStep) {
                colsLeft = scanCol != endCol;
                tileID = GetTileID(tileMapP, endRow, scanCol);
                if (tileID < kFirstWallTile || tileID > kLastWallTile ||
                    !IsTileThickHorizWall(tileMapP, endRow, scanCol))
                    allThick = false;
            }
            if (!allThick) {
                allThick = true;
                for (scanCol = startCol, colsLeft = true; colsLeft; scanCol += colStep) {
                    colsLeft = scanCol != endCol;
                    tileID = GetTileID(tileMapP, endRow - 1, scanCol);
                    if (tileID < kFirstWallTile || tileID > kLastWallTile ||
                        !IsTileThickHorizWall(tileMapP, endRow, scanCol))
                        allThick = false;
                }
            }
        }

        if (endRow < startRow)
            SwapVariables(&startRow, &endRow);
        if (endCol < startCol)
            SwapVariables(&startCol, &endCol);
        startRow = endRow + 1;
        endRow = FloorDiv16(heroP->jumpStartBottom);
        if (endRow >= worldP->tileLayerArray[startLayer]->numRows)
            endRow = worldP->tileLayerArray[startLayer]->numRows - 1;
        if (endRow < startRow)
            startRow = endRow;

        if (!found && allThick && heroP->isJumping &&
            (direction == kSW_CheckRight || direction == kSW_CheckLeft)) {
                // jumping into the end of a wall: is there a gap above it to jump into?
            col = direction == kSW_CheckRight ? endCol : startCol;
            if (movedInDirection) {
                for (layer = startLayer; layer <= endLayer; layer++) {
                    tileMapP = worldP->tileLayerArray[layer];
                    if (tileMapP == NULL)
                        continue;
                    for (row = startRow; row <= endRow; row++) {
                        tileID = GetTileID(tileMapP, row, col);
                        if (!IsTileHorizWall(GetTileID(tileMapP, row, col + sideStep))) {
                            found = tileID >= kFirstWallTile && tileID <= kLastWallTile;
                            goto checkedGap;
                        }
                    }
                }
            }
checkedGap:
            if (!found) {
                doorInset = heroP->doorInset;
                tileMapP = worldP->tileLayerArray[startLayer];
                SW_ASSERT(tileMapP != NULL);
                for (r = startRow; r <= endRow; r++) {
                    tileID = GetTileID(tileMapP, r, col);
                    if (tileID <= 0 || tileID > kLastWallTile || !IsTileThickHorizWall(tileMapP, r, col))
                        break;
                    if (!IsTileHorizWall(GetTileID(tileMapP, r, col + sideStep)))
                        break;
                }
                doorInset.bottom = spriteP->destFrameRect.bottom - r * 16 - 16;
                return SWCheckSpriteWithSprites(spriteP, NULL, direction, &doorInset, gDoorSpriteLayerP,
                                                gDoorSpriteLayerP, true, false, false, fixPosition);
            }
        }
    }

    if (found && fixPosition && movedInDirection) {
        short delta;

        if (direction == kSW_CheckTop) {
            delta = worldP->tileHeight * (row + 1) - destRect.top;
            SWOffsetSprite(spriteP, 0, delta);
        }
        else if (direction == kSW_CheckBottom) {
            delta = destRect.bottom - worldP->tileHeight * row;
            SWOffsetSprite(spriteP, 0, -delta);
        }
        else if (direction == kSW_CheckRight) {
            delta = destRect.right - worldP->tileWidth * col;
            SWOffsetSprite(spriteP, -delta, 0);
        }
        else if (direction == kSW_CheckLeft) {
            delta = worldP->tileWidth * (col + 1) - destRect.left;
            SWOffsetSprite(spriteP, delta, 0);
        }
    }
    return found;
}

/// The (original-numbering) tile ID at row/col, clamped to the map.
short GetTileID(TileMapStructPtr tileMapP, short row, short col)
{
    if (row < 0) row = 0;
    if (col < 0) col = 0;
    if (row >= tileMapP->numRows) row = tileMapP->numRows - 1;
    if (col >= tileMapP->numCols) col = tileMapP->numCols - 1;
    return ConvertTileID(tileMapP->tileMap[row][col]);
}

/// Tiles can be swapped for look-alikes (color variations); this maps a tile
/// back to the ID it had in the level.
short ConvertTileID(short tileID)
{
    if (tileID >= gSpriteWorldP->maxNumTiles) {
        printf("ConvertTileID failed because tileID is %d while SpriteWorld's maxNumTiles is %d! \n",
               tileID, gSpriteWorldP->maxNumTiles);
        SW_ASSERT(tileID < gSpriteWorldP->maxNumTiles);
    }
    if (tileID >= 0)
        tileID = gOrigTileIDs[tileID];
    return tileID;
}
