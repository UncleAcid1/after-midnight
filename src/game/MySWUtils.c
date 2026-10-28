/*
 * MySWUtils.c — SpriteWorld helpers used by the game.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources - My Utils/MySWUtils.c").
 */
#include "MMGame.h"

#define kMaxTempLayers  100
#define kMaxTileLayers  10      // the original's (fixed) number of saved tile layers

static short gNumLayersSaved = 0;
static Boolean gSpritesWereSaved = false;
static Boolean gTileMapsWereSaved = false;
static TileMapStructPtr gSaveTileLayerArray[16];
static SpriteLayerPtr gTempLayerArray[kMaxTempLayers];

/// Collision rects with the frames' collision insets applied, if either sprite uses them.
static Boolean SpritesOverlapWithInsets(SpritePtr a, SpritePtr b)
{
    FramePtr fa = a->curFrameP, fb = b->curFrameP;

    if (!(a->destFrameRect.top < b->destFrameRect.bottom && b->destFrameRect.top < a->destFrameRect.bottom &&
          a->destFrameRect.left < b->destFrameRect.right && b->destFrameRect.left < a->destFrameRect.right))
        return false;

    if (!fa->usesCollisionInset && !fb->usesCollisionInset)
        return true;

    return a->destFrameRect.top + fa->collisionInset.top < b->destFrameRect.bottom - fb->collisionInset.bottom &&
           b->destFrameRect.top + fb->collisionInset.top < a->destFrameRect.bottom - fa->collisionInset.bottom &&
           a->destFrameRect.left + fa->collisionInset.left < b->destFrameRect.right - fb->collisionInset.right &&
           b->destFrameRect.left + fb->collisionInset.left < a->destFrameRect.right - fa->collisionInset.right;
}

SWBoolean SWBestMaskCollision(SpritePtr srcSpriteP, SpritePtr dstSpriteP)
{
    return SWPixelCollision(srcSpriteP, dstSpriteP);
}

/// Makes the frame-rate timer let the next frame start at once.
static void MakeFrameDue(SpriteWorldPtr spriteWorldP)
{
        // (with the game clock in use, runningTimeCount isn't the real time the timer counts in)
    unsigned long now = gSWUseGameClock ? SWGetMilliseconds(spriteWorldP) : spriteWorldP->runningTimeCount;

    if (now - spriteWorldP->timeOfLastFrame < (unsigned long)spriteWorldP->fpsTimeInterval)
        spriteWorldP->timeOfLastFrame = now - spriteWorldP->fpsTimeInterval;
}

/// Process one frame immediately, regardless of the frame-rate timer.
void ProcessOneFrame(SpriteWorldPtr spriteWorldP)
{
    MakeFrameDue(spriteWorldP);
    SWProcessSpriteWorld(spriteWorldP);
}

void AnimateOneFrame(SpriteWorldPtr spriteWorldP)
{
    MakeFrameDue(spriteWorldP);
    SWProcessSpriteWorld(spriteWorldP);
    SWAnimateSpriteWorld(spriteWorldP);
}

void SetSpriteWorldForCapturedDisplay(void) {}
void SetSpriteWorldForUncapturedDisplay(void) {}

/// Changes a tile's image without redrawing it.
void SWChangeTileImageFast(SpriteWorldPtr spriteWorldP, short tileID, short imageID)
{
    SW_ASSERT(spriteWorldP != NULL && spriteWorldP->tilingIsInitialized);
    SW_ASSERT(tileID < spriteWorldP->maxNumTiles);
    SW_ASSERT(imageID < spriteWorldP->maxNumTiles);
    spriteWorldP->curTileImage[tileID] = imageID;
}

Boolean DoesLayerHaveTiles(SpriteWorldPtr spriteWorldP, short tileLayer, short firstTileID, short lastTileID)
{
    TileMapStructPtr tileMapP = spriteWorldP->tileLayerArray[tileLayer];
    short row, col, tileID;

    for (row = 0; row < tileMapP->numRows; row++) {
        for (col = 0; col < tileMapP->numCols; col++) {
            tileID = tileMapP->tileMap[row][col];
            if (tileID >= firstTileID && tileID <= lastTileID)
                return true;
        }
    }
    return false;
}

/// Redraws the visible tiles of the given layers whose IDs are in [firstTileID, lastTileID].
void MyUpdateTilesOnScreenFaster(SpriteWorldPtr spriteWorldP, short startLayer, short endLayer,
                                 short firstTileID, short lastTileID)
{
    short layer, row, col, tileID;
    short startRow, startCol, endRow, endCol;

    SW_ASSERT(spriteWorldP != NULL && spriteWorldP->tilingIsInitialized);

    startRow = spriteWorldP->visScrollRect.top / spriteWorldP->tileHeight;
    startCol = spriteWorldP->visScrollRect.left / spriteWorldP->tileWidth;
    endRow = (spriteWorldP->visScrollRect.bottom - 1) / spriteWorldP->tileHeight;
    endCol = (spriteWorldP->visScrollRect.right - 1) / spriteWorldP->tileWidth;

    for (layer = startLayer; layer <= endLayer; layer++) {
        short **tileMap;
        if (spriteWorldP->tileLayerArray[layer] == NULL)
            continue;
        tileMap = spriteWorldP->tileLayerArray[layer]->tileMap;
        for (row = startRow; row <= endRow; row++) {
            for (col = startCol; col <= endCol; col++) {
                tileID = tileMap[row][col];
                if (tileID >= firstTileID && tileID <= lastTileID)
                    SWDrawTile(spriteWorldP, layer, row, col, tileID);
            }
        }
    }
}

/// Moves every sprite (except the fade layer's) to temporary layers, e.g. while a menu screen shows.
SWError SaveAllSpritesToTempLayers(SpriteWorldPtr spriteWorldP)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP, nextSpriteP;
    SWError err = kNoError;
    short i;

    if (gSpritesWereSaved == true)
        return -1;

    gNumLayersSaved = 0;
    layerP = NULL;
    while ((layerP = SWGetNextSpriteLayer(spriteWorldP, layerP)) != NULL)
        gNumLayersSaved++;
    gNumLayersSaved--;     // the fade layer is not saved

    if (gNumLayersSaved >= kMaxTempLayers)
        err = -1;

    if (err == kNoError) {
        for (i = 0; i < gNumLayersSaved; i++) {
            err = SWCreateSpriteLayer(&gTempLayerArray[i]);
            if (err != kNoError)
                break;
        }
    }

    if (err == kNoError) {
        i = 0;
        layerP = NULL;
        while ((layerP = SWGetNextSpriteLayer(spriteWorldP, layerP)) != NULL) {
            if (layerP == gFadeToBlackSpriteLayerP)
                continue;
            SW_ASSERT(i < gNumLayersSaved);
            spriteP = SWGetNextSprite(layerP, NULL);
            while (spriteP != NULL) {
                nextSpriteP = SWGetNextSprite(layerP, spriteP);
                SWRemoveSprite(spriteP);
                SWAddSprite(gTempLayerArray[i], spriteP);
                spriteP = nextSpriteP;
            }
            i++;
        }
        gSpritesWereSaved = true;
    }
    return err;
}

void RestoreSpritesFromTempLayers(SpriteWorldPtr spriteWorldP)
{
    SpriteLayerPtr layerP, tempLayerP;
    SpritePtr spriteP, nextSpriteP;
    short i;

    if (!gSpritesWereSaved)
        return;

    layerP = SWGetNextSpriteLayer(spriteWorldP, NULL);
    for (i = 0; i < gNumLayersSaved; i++) {
        tempLayerP = gTempLayerArray[i];
        if (layerP == gFadeToBlackSpriteLayerP) {
            // The original skips the fade layer here and loses this temp layer's
            // sprites to the next one in line; matching that, i still advances.
            layerP = SWGetNextSpriteLayer(spriteWorldP, layerP);
            continue;
        }
        SW_ASSERT(layerP != NULL);
        if (layerP != NULL) {
            spriteP = SWGetNextSprite(tempLayerP, NULL);
            while (spriteP != NULL) {
                nextSpriteP = SWGetNextSprite(tempLayerP, spriteP);
                SWRemoveSprite(spriteP);
                SWAddSprite(layerP, spriteP);
                spriteP = nextSpriteP;
            }
        }
        layerP = SWGetNextSpriteLayer(spriteWorldP, layerP);
    }

    for (i = 0; i < gNumLayersSaved; i++)
        SWDisposeSpriteLayer(&gTempLayerArray[i]);
    gSpritesWereSaved = false;
}

void SWSaveTileMaps(SpriteWorldPtr spriteWorldP)
{
    short i;
    if (gTileMapsWereSaved)
        return;
    for (i = 0; i < kMaxTileLayers; i++) {
        gSaveTileLayerArray[i] = spriteWorldP->tileLayerArray[i];
        spriteWorldP->tileLayerArray[i] = NULL;
    }
    spriteWorldP->lastActiveTileLayer = 0;
    gTileMapsWereSaved = true;
}

void SWRestoreTileMaps(SpriteWorldPtr spriteWorldP)
{
    short i;
    if (!gTileMapsWereSaved)
        return;
    for (i = 0; i < kMaxTileLayers; i++) {
        SWInstallTileMap(spriteWorldP, gSaveTileLayerArray[i], i);
        gSaveTileLayerArray[i] = NULL;
    }
    gTileMapsWereSaved = false;
}

void InvalidateSpriteLayer(SpriteLayerPtr spriteLayerP)
{
    SpritePtr spriteP;
    SW_ASSERT(spriteLayerP != NULL);
    for (spriteP = spriteLayerP->headSpriteP; spriteP != NULL; spriteP = spriteP->nextSpriteP)
        SWInvalidateSprite(spriteP);
}

void OffsetSpritesInLayer(SpriteLayerPtr spriteLayerP, SWMovementType h, SWMovementType v)
{
    SpritePtr spriteP;
    SW_ASSERT(spriteLayerP != NULL);
    for (spriteP = spriteLayerP->headSpriteP; spriteP != NULL; spriteP = spriteP->nextSpriteP)
        SWOffsetSprite(spriteP, h, v);
}

/// Insertion sort: sprites lower on screen draw later (on top).
void SortSpriteLayerBottomToTop(SpriteLayerPtr spriteLayerP)
{
    SpritePtr prevP, curP, searchP;

    SortSpriteLayerRightToLeft(spriteLayerP);

    prevP = spriteLayerP->headSpriteP;
    curP = SWGetNextSprite(spriteLayerP, prevP);
    while (curP != NULL) {
        if (prevP->destFrameRect.bottom < curP->destFrameRect.bottom) {
            searchP = curP->prevSpriteP;
            while (searchP != NULL && searchP->destFrameRect.bottom < curP->destFrameRect.bottom)
                searchP = searchP->prevSpriteP;
            SWRemoveSprite(curP);
            if (searchP == NULL)
                SWInsertSpriteBeforeSprite(curP, spriteLayerP->headSpriteP);
            else
                SWInsertSpriteAfterSprite(curP, searchP);
            curP = SWGetNextSprite(spriteLayerP, prevP);
        } else {
            prevP = curP;
            curP = curP->nextSpriteP;
        }
    }
}

void SortSpriteLayerRightToLeft(SpriteLayerPtr spriteLayerP)
{
    SpritePtr prevP, curP, searchP;

    prevP = spriteLayerP->headSpriteP;
    curP = SWGetNextSprite(spriteLayerP, prevP);
    while (curP != NULL) {
        if (curP->destFrameRect.right < prevP->destFrameRect.right) {
            searchP = curP->prevSpriteP;
            while (searchP != NULL && curP->destFrameRect.right < searchP->destFrameRect.right)
                searchP = searchP->prevSpriteP;
            SWRemoveSprite(curP);
            if (searchP == NULL)
                SWInsertSpriteBeforeSprite(curP, spriteLayerP->headSpriteP);
            else
                SWInsertSpriteAfterSprite(curP, searchP);
            curP = SWGetNextSprite(spriteLayerP, prevP);
        } else {
            prevP = curP;
            curP = curP->nextSpriteP;
        }
    }
}

/**
 * Sprite-versus-sprites version of SWCheckSpriteWithTiles: looks for a sprite in
 * layers startLayerP..endLayerP inside the area the sprite moved through on the
 * given side. With fixPosition, the sprite is pushed back against what it hit.
 */
SWBoolean SWCheckSpriteWithSprites(SpritePtr srcSpriteP, SpritePtr *hitSpritePP, SWTileSearchType searchType,
                                   SWRect *insetRectP, SpriteLayerPtr startLayerP, SpriteLayerPtr endLayerP,
                                   SWBoolean useOldRect, SWBoolean includeInvisible, SWBoolean mustBeBelow,
                                   SWBoolean fixPosition)
{
    SpritePtr hitP;
    SWRect destRect, oldRect;
    short offset;

    SW_ASSERT(startLayerP != NULL);
    SW_ASSERT(endLayerP != NULL);
    SW_ASSERT(srcSpriteP != NULL);

    destRect = srcSpriteP->destFrameRect;
    oldRect = srcSpriteP->oldFrameRect;
    if (insetRectP != NULL) {
        destRect.top += insetRectP->top;
        destRect.left += insetRectP->left;
        destRect.bottom -= insetRectP->bottom;
        destRect.right -= insetRectP->right;
        oldRect.top += insetRectP->top;
        oldRect.left += insetRectP->left;
        oldRect.bottom -= insetRectP->bottom;
        oldRect.right -= insetRectP->right;
    }

        // only check the area the sprite moved into
    switch (searchType) {
        case kSW_CheckTop:    destRect.bottom = oldRect.top; break;
        case kSW_CheckRight:  destRect.left = oldRect.right; break;
        case kSW_CheckBottom: destRect.top = oldRect.bottom; break;
        case kSW_CheckLeft:   destRect.right = oldRect.left; break;
        default: break;
    }
    if (destRect.right < destRect.left)
        SwapVariables(&destRect.left, &destRect.right);
    if (destRect.bottom < destRect.top)
        SwapVariables(&destRect.top, &destRect.bottom);

    hitP = FindSpriteByRectInMultipleLayers(startLayerP, endLayerP, srcSpriteP, &destRect,
                                            useOldRect, includeInvisible, mustBeBelow);

    if (fixPosition && hitP != NULL) {
        SWRect *inset = &hitP->curFrameP->collisionInset;
        switch (searchType) {
            case kSW_CheckTop:
                if (oldRect.top < hitP->oldFrameRect.bottom - inset->bottom) {
                    hitP = NULL;    // was already overlapping
                } else {
                    offset = (hitP->destFrameRect.bottom - inset->bottom) - destRect.top;
                    RoundOffSpriteVertLoc(srcSpriteP);
                    SWOffsetSprite(srcSpriteP, 0, offset);
                }
                break;
            case kSW_CheckBottom:
                if (hitP->oldFrameRect.top + inset->top < oldRect.bottom) {
                    hitP = NULL;
                } else {
                    offset = destRect.bottom - (hitP->destFrameRect.top + inset->top);
                    RoundOffSpriteVertLoc(srcSpriteP);
                    SWOffsetSprite(srcSpriteP, 0, -offset);
                }
                break;
            case kSW_CheckRight:
                if (hitP->oldFrameRect.left + inset->left < oldRect.right) {
                    hitP = NULL;
                } else {
                    offset = destRect.right - (hitP->destFrameRect.left + inset->left);
                    SWOffsetSprite(srcSpriteP, -offset, 0);
                }
                break;
            case kSW_CheckLeft:
                if (oldRect.left < hitP->oldFrameRect.right - inset->right) {
                    hitP = NULL;
                } else {
                    offset = (hitP->destFrameRect.right - inset->right) - destRect.left;
                    SWOffsetSprite(srcSpriteP, offset, 0);
                }
                break;
            default:
                break;
        }
    }

    if (hitSpritePP != NULL)
        *hitSpritePP = hitP;
    return hitP != NULL;
}

void RoundOffSpriteVertLoc(SpritePtr spriteP)
{
    SWMovementType v = SWGetSpriteVertLoc(spriteP);
    SWMoveSprite(spriteP, SWGetSpriteHorizLoc(spriteP), (float)(short)v);
}

/// Calls collideProc for every sprite in the layer that overlaps srcSpriteP.
void SWCollideSpriteWithLayer(SpritePtr srcSpriteP, SpriteLayerPtr spriteLayerP, CollideProcPtr collideProc)
{
    SpritePtr dstP, nextP;
    SWRect sectRect;

    SW_ASSERT(spriteLayerP != NULL);
    SW_ASSERT(collideProc != NULL);

    dstP = spriteLayerP->headSpriteP;
    while (dstP != NULL) {
        nextP = dstP->nextSpriteP;
        if (srcSpriteP != dstP && SpritesOverlapWithInsets(srcSpriteP, dstP)) {
            FramePtr fs = srcSpriteP->curFrameP, fd = dstP->curFrameP;
            sectRect.left = SW_MAX(dstP->destFrameRect.left + fd->collisionInset.left,
                                   srcSpriteP->destFrameRect.left + fs->collisionInset.left);
            sectRect.top = SW_MAX(dstP->destFrameRect.top + fd->collisionInset.top,
                                  srcSpriteP->destFrameRect.top + fs->collisionInset.top);
            sectRect.right = SW_MIN(dstP->destFrameRect.right - fd->collisionInset.right,
                                    srcSpriteP->destFrameRect.right - fs->collisionInset.right);
            sectRect.bottom = SW_MIN(dstP->destFrameRect.bottom - fd->collisionInset.bottom,
                                     srcSpriteP->destFrameRect.bottom - fs->collisionInset.bottom);
            collideProc(srcSpriteP, dstP, &sectRect);
            if (srcSpriteP->spriteRemoval != kDontRemoveSprite || srcSpriteP->parentSpriteLayerP == NULL)
                return;
        }
            // the next sprite was removed by the collision: start over
        if (nextP != NULL && nextP->spriteRemoval != kDontRemoveSprite)
            dstP = spriteLayerP->headSpriteP;
        else
            dstP = nextP;
    }
}

/// Returns the first sprite in the layer that overlaps srcSpriteP and for which testProc returns true.
SpritePtr GetSpriteCollidingWithSprite(SpritePtr srcSpriteP, SpriteLayerPtr spriteLayerP,
                                       SpriteTestProcPtr testProc)
{
    SpritePtr dstP, nextP;

    SW_ASSERT(spriteLayerP != NULL);
    SW_ASSERT(testProc != NULL);

    dstP = spriteLayerP->headSpriteP;
    while (dstP != NULL) {
        nextP = dstP->nextSpriteP;
        if (srcSpriteP != dstP && SpritesOverlapWithInsets(srcSpriteP, dstP)) {
            if (testProc(srcSpriteP, dstP))
                return dstP;
            if (srcSpriteP->spriteRemoval != kDontRemoveSprite || srcSpriteP->parentSpriteLayerP == NULL)
                return dstP;
        }
        dstP = nextP;
        if (nextP != NULL && nextP->spriteRemoval != kDontRemoveSprite)
            dstP = spriteLayerP->headSpriteP;
    }
    return NULL;
}

Boolean IsSpriteCollidingWithLayer(SpritePtr srcSpriteP, SpriteLayerPtr spriteLayerP,
                                   Boolean usePixelCollision, Boolean visibleOnly)
{
    SpritePtr dstP, nextP;
    Boolean collided;

    SW_ASSERT(srcSpriteP != NULL);
    SW_ASSERT(spriteLayerP != NULL);

    dstP = spriteLayerP->headSpriteP;
    while (dstP != NULL) {
        nextP = dstP->nextSpriteP;
        if (srcSpriteP != dstP) {
            collided = SpritesOverlapWithInsets(srcSpriteP, dstP);
            if (visibleOnly && !dstP->isVisible)
                collided = false;
            if (collided && usePixelCollision)
                collided = SWBestMaskCollision(srcSpriteP, dstP);
            if (collided)
                return true;
        }
        dstP = nextP;
        if (nextP != NULL && nextP->spriteRemoval != kDontRemoveSprite)
            dstP = spriteLayerP->headSpriteP;
    }
    return false;
}

void MoveSpritesFromLayerToLayer(SpriteLayerPtr srcLayerP, SpriteLayerPtr dstLayerP)
{
    SpritePtr spriteP, nextP;
    spriteP = SWGetNextSprite(srcLayerP, NULL);
    while (spriteP != NULL) {
        nextP = SWGetNextSprite(srcLayerP, spriteP);
        SWRemoveSprite(spriteP);
        SWAddSprite(dstLayerP, spriteP);
        spriteP = nextP;
    }
}

/**
 * Finds a sprite other than srcSpriteP in layers startLayerP..endLayerP (following
 * nextSpriteLayerP) whose rect (or old rect) overlaps testRect. With mustBeBelow,
 * only sprites whose top is below srcSpriteP's top count.
 */
SpritePtr FindSpriteByRectInMultipleLayers(SpriteLayerPtr startLayerP, SpriteLayerPtr endLayerP,
                                           SpritePtr srcSpriteP, SWRect *testRect, SWBoolean useOldRect,
                                           SWBoolean includeInvisible, SWBoolean mustBeBelow)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP = NULL;
    Boolean inRect;

    SW_ASSERT(startLayerP != NULL);
    SW_ASSERT(srcSpriteP != NULL);

    for (layerP = startLayerP; layerP != NULL; layerP = layerP->nextSpriteLayerP) {
        spriteP = layerP->headSpriteP;
        if (spriteP == srcSpriteP)
            spriteP = spriteP->nextSpriteP;
        while (spriteP != NULL) {
            if (includeInvisible || spriteP->isVisible) {
                inRect = useOldRect ? IsSpriteOldRectInRect(spriteP, testRect)
                                    : SWIsSpriteInRect(spriteP, testRect);
                if (inRect && (!mustBeBelow || srcSpriteP->destFrameRect.top < spriteP->destFrameRect.top))
                    return spriteP;
            }
            spriteP = spriteP->nextSpriteP;
            if (spriteP == srcSpriteP)
                spriteP = spriteP->nextSpriteP;
        }
        if (layerP == endLayerP)
            break;
    }
    return spriteP;     // NULL
}

void SetAllSpriteLayersVisible(SpriteWorldPtr spriteWorldP, SWBoolean visible)
{
    SpriteLayerPtr layerP;
    for (layerP = SWGetNextSpriteLayer(spriteWorldP, NULL); layerP != NULL;
         layerP = SWGetNextSpriteLayer(spriteWorldP, layerP))
        SWSetSpriteLayerVisible(layerP, visible);
}

void PauseAllSpriteLayers(SpriteWorldPtr spriteWorldP)
{
    SpriteLayerPtr layerP;
    for (layerP = SWGetNextSpriteLayer(spriteWorldP, NULL); layerP != NULL;
         layerP = SWGetNextSpriteLayer(spriteWorldP, layerP))
        SWPauseSpriteLayer(layerP);
}

void UnpauseAllSpriteLayers(SpriteWorldPtr spriteWorldP)
{
    SpriteLayerPtr layerP;
    for (layerP = SWGetNextSpriteLayer(spriteWorldP, NULL); layerP != NULL;
         layerP = SWGetNextSpriteLayer(spriteWorldP, layerP))
        SWUnpauseSpriteLayer(layerP);
}

void RemoveAllSpritesInLayer(SpriteLayerPtr spriteLayerP)
{
    SpritePtr spriteP, nextP;
    SW_ASSERT(spriteLayerP != NULL);
    spriteP = SWGetNextSprite(spriteLayerP, NULL);
    while (spriteP != NULL) {
        nextP = SWGetNextSprite(spriteLayerP, spriteP);
        SWRemoveSpriteFromAnimation(spriteP, true);
        spriteP = nextP;
    }
}

void SWSetSpritesInLayerVisible(SpriteLayerPtr spriteLayerP, SWBoolean visible)
{
    SpritePtr spriteP = NULL;
    while ((spriteP = SWGetNextSprite(spriteLayerP, spriteP)) != NULL)
        SWSetSpriteVisible(spriteP, visible);
}

SWError SWCreateSolidColorSprite(SpritePtr *newSpritePP)
{
    SWError err = SWCreateSprite(newSpritePP, NULL, 0);
    if (err == kNoError)
        SWSetSpriteDrawProc(*newSpritePP, SWSolidColorSpriteDrawProc);
    return err;
}

SpritePtr SWTopVisibleSpriteFromPoint(SpriteLayerPtr spriteLayerP, SpritePtr startSpriteP, SWPoint testPoint,
                                      SpriteInMaskProcPtr testProc)
{
    SpritePtr spriteP;
    SW_ASSERT(spriteLayerP != NULL);
    for (spriteP = SWGetPreviousSprite(spriteLayerP, startSpriteP); spriteP != NULL;
         spriteP = SWGetPreviousSprite(spriteLayerP, spriteP)) {
        if (SWIsPointInSpriteMask(spriteP, testPoint.h, testPoint.v, testProc) && spriteP->isVisible)
            return spriteP;
    }
    return NULL;
}

/// Converts a point from window (worldRect) coordinates to scrolled world coordinates.
void SWConvertCoordinates(SpriteWorldPtr spriteWorldP, SWPoint *pointP)
{
    pointP->v += spriteWorldP->visScrollRect.top - spriteWorldP->worldRect.top;
    pointP->h += spriteWorldP->visScrollRect.left - spriteWorldP->worldRect.left;
}

void MySetSpriteFrameTime(SpritePtr spriteP, long frameTime)
{
    SpriteWorldPtr spriteWorldP = NULL;
    SW_ASSERT(spriteP != NULL);
    if (SWGetSpriteFrameTime(spriteP) == frameTime)
        return;
    SWSetSpriteFrameTime(spriteP, frameTime);
    if (spriteP->parentSpriteLayerP != NULL)
        spriteWorldP = spriteP->parentSpriteLayerP->parentSpriteWorldP;
    if (spriteWorldP == NULL)
        spriteWorldP = gSWCurrentSpriteWorld;
    if (spriteWorldP != NULL)
        spriteP->timeOfLastFrameChange = spriteWorldP->runningTimeCount;
}

/// Advances the frame by *advance, looping or (in patrol mode) reversing direction.
void MyAdvanceSpriteFrame(SpritePtr spriteP, short *advance)
{
    long index = spriteP->curFrameIndex + *advance;

    if (index < spriteP->firstFrameIndex) {
        if (spriteP->frameAdvanceMode == kNoLoopingMode) {
            index = spriteP->lastFrameIndex;
        } else {
            *advance = -*advance;
            index += *advance + *advance;
        }
    } else if (index > spriteP->lastFrameIndex) {
        if (spriteP->frameAdvanceMode == kNoLoopingMode) {
            index = spriteP->firstFrameIndex;
        } else {
            *advance = -*advance;
            index += *advance + *advance;
        }
    }
    if (index >= 0)
        SWSetCurrentFrameIndex(spriteP, index);
}

void SWResetSpriteMovementTimer(SpriteWorldPtr spriteWorldP, SpritePtr spriteP)
{
    spriteP->timeOfLastMove = spriteWorldP->runningTimeCount;
}

void SWSetSpriteHotSpot(SpritePtr spriteP, short hotSpotH, short hotSpotV)
{
    long i;
    for (i = 0; i < spriteP->maxFrames; i++)
        SWSetFrameHotSpot(spriteP->frameArray[i], spriteP->drawData, hotSpotH, hotSpotV);
}

SpriteWorldPtr GetSpriteWorldFromSprite(SpritePtr spriteP)
{
    SW_ASSERT(spriteP != NULL);
    if (spriteP->parentSpriteLayerP == NULL)
        return NULL;
    return spriteP->parentSpriteLayerP->parentSpriteWorldP;
}

MoveProcPtr SWGetSpriteMoveProc(SpritePtr spriteP)
{
    return spriteP->realSpriteMoveProc;
}

/// Distance between the sprites' centers, truncated to a short.
short GetDistBetweenSprites(SpritePtr a, SpritePtr b)
{
    short dh, dv;
    SW_ASSERT(a != NULL);
    SW_ASSERT(b != NULL);
    dh = (short)(int)(SWGetSpriteWidth(a) / 2.0f + SWGetSpriteHorizLoc(a)) -
         (short)(int)(SWGetSpriteWidth(b) / 2.0f + SWGetSpriteHorizLoc(b));
    dv = (short)(int)(SWGetSpriteHeight(a) / 2.0f + SWGetSpriteVertLoc(a)) -
         (short)(int)(SWGetSpriteHeight(b) / 2.0f + SWGetSpriteVertLoc(b));
    return (short)(int)sqrt((double)(dh * dh + dv * dv));
}

short GetDistBetweenSpritesHorizMid(SpritePtr a, SpritePtr b)
{
    short ha = (short)(int)(SWGetSpriteWidth(a) / 2.0f + SWGetSpriteHorizLoc(a));
    short hb = (short)(int)(SWGetSpriteWidth(b) / 2.0f + SWGetSpriteHorizLoc(b));
    return (short)(ha - hb < 0 ? hb - ha : ha - hb);
}

short GetDistBetweenSpritesVertMid(SpritePtr a, SpritePtr b)
{
    short va = (short)(int)(SWGetSpriteHeight(a) / 2.0f + SWGetSpriteVertLoc(a));
    short vb = (short)(int)(SWGetSpriteHeight(b) / 2.0f + SWGetSpriteVertLoc(b));
    return (short)(va - vb < 0 ? vb - va : va - vb);
}

/// Postpones the sprite's next move by delay ms beyond its normal interval.
void DelayMoveProcByValue(SpritePtr spriteP, long delay)
{
    SpriteWorldPtr spriteWorldP = GetSpriteWorldFromSprite(spriteP);
    if (spriteWorldP != NULL)
        spriteP->timeOfLastMove = spriteWorldP->runningTimeCount + spriteP->moveTimeInterval + delay;
}

SpritePtr FindSpriteByPixel(SpriteLayerPtr spriteLayerP, SpritePtr startSpriteP, short h, short v)
{
    SpritePtr spriteP;
    SW_ASSERT(spriteLayerP != NULL);
    for (spriteP = startSpriteP != NULL ? startSpriteP : spriteLayerP->headSpriteP; spriteP != NULL;
         spriteP = spriteP->nextSpriteP) {
        if (spriteP->destFrameRect.top == v && spriteP->destFrameRect.left == h)
            break;
    }
    return spriteP;
}

/// Searches back from startSpriteP (or the tail) for a sprite in testRect.
SpritePtr FindSpriteByRect(SpriteLayerPtr spriteLayerP, SpritePtr startSpriteP, SWRect *testRect)
{
    SpritePtr spriteP;
    SW_ASSERT(spriteLayerP != NULL);
    for (spriteP = startSpriteP != NULL ? startSpriteP : spriteLayerP->tailSpriteP; spriteP != NULL;
         spriteP = spriteP->prevSpriteP) {
        if (SWIsSpriteInRect(spriteP, testRect))
            return spriteP;
    }
    return NULL;
}

Boolean IsSpriteOldRectInRect(SpritePtr spriteP, SWRect *testRect)
{
    SWRect *inset;
    SW_ASSERT(spriteP != NULL);
    SW_ASSERT(spriteP->curFrameP != NULL);
    inset = &spriteP->curFrameP->collisionInset;
    return spriteP->oldFrameRect.top + inset->top < testRect->bottom &&
           testRect->top < spriteP->oldFrameRect.bottom - inset->bottom &&
           spriteP->oldFrameRect.left + inset->left < testRect->right &&
           testRect->left < spriteP->oldFrameRect.right - inset->right;
}

void SWCenterSpriteOverSprite(SpritePtr spriteP, SpritePtr targetP)
{
    SWRect spriteRect, targetRect;

    spriteRect.top = 0;
    spriteRect.left = 0;
    spriteRect.right = (short)(int)SWGetSpriteWidth(spriteP);
    spriteRect.bottom = (short)(int)SWGetSpriteHeight(spriteP);

    targetRect.top = (short)(int)SWGetSpriteVertLoc(targetP);
    targetRect.left = (short)(int)SWGetSpriteHorizLoc(targetP);
    targetRect.right = (short)(int)((float)targetRect.left + SWGetSpriteWidth(targetP));
    targetRect.bottom = (short)(int)((float)targetRect.top + SWGetSpriteHeight(targetP));

    SWCenterRect(&spriteRect, &targetRect);
    SWMoveSprite(spriteP, spriteRect.left, spriteRect.top);
}

void SWCenterSpriteInRect(SpritePtr spriteP, SWRect *rectP)
{
    SWRect spriteRect = spriteP->destFrameRect;
    SWCenterRect(&spriteRect, rectP);
    SWMoveSprite(spriteP, spriteRect.left, spriteRect.top);
}
