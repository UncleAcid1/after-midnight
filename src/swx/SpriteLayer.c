/*
 * SpriteWorld X fork — sprite layers (ordered lists of sprites).
 * See SWCommonHeaders.h for license and provenance.
 */

#include "SpriteWorld.h"

SWError SWCreateSpriteLayer(SpriteLayerPtr *spriteLayerPP)
{
    SpriteLayerPtr layerP;
    SWError err = kNoError;

    *spriteLayerPP = NULL;
    layerP = calloc(1, sizeof(SpriteLayerRec));
    if (layerP == NULL) {
        err = kMemoryAllocationError;
    } else {
        layerP->tileLayerBelow = -1;
        layerP->tileLayerAbove = -1;
        layerP->isPaused = false;
        layerP->layerIsVisible = true;
        layerP->layerIsNonScrolling = false;
        layerP->sanityValue = kSpriteLayerSanityValue;
        *spriteLayerPP = layerP;
    }
    SWSetStickyIfError(err);
    return err;
}

void SWDisposeSpriteLayer(SpriteLayerPtr *spriteLayerPP)
{
    if (*spriteLayerPP != NULL) {
        free(*spriteLayerPP);
        *spriteLayerPP = NULL;
    }
}

SWError SWAddSprite(SpriteLayerPtr spriteLayerP, SpritePtr newSpriteP)
{
    SpritePtr tailSpriteP;
    SWError err = kNoError;

    SW_ASSERT(spriteLayerP != NULL);
    SW_ASSERT(spriteLayerP->sanityValue == kSpriteLayerSanityValue);
    SW_ASSERT(newSpriteP != NULL);
    tailSpriteP = spriteLayerP->tailSpriteP;
    if (newSpriteP->parentSpriteLayerP != NULL)
        err = kSpriteAlreadyInLayer;
    if (err == kNoError) {
        if (tailSpriteP == NULL)
            spriteLayerP->headSpriteP = newSpriteP;
        else
            tailSpriteP->nextSpriteP = newSpriteP;
        newSpriteP->prevSpriteP = tailSpriteP;
        newSpriteP->nextSpriteP = NULL;
        spriteLayerP->tailSpriteP = newSpriteP;
        newSpriteP->needsToBeDrawn = true;
        newSpriteP->parentSpriteLayerP = spriteLayerP;
    }
    SWSetStickyIfError(err);
    return err;
}

SWError SWRemoveSprite(SpritePtr oldSpriteP)
{
    SpriteLayerPtr layerP;
    SWError err = kNoError;

    SW_ASSERT(oldSpriteP != NULL);
    layerP = oldSpriteP->parentSpriteLayerP;
    if (layerP == NULL)
        err = kBadParameterErr;
    if (err == kNoError) {
        if (oldSpriteP->nextSpriteP == NULL)
            layerP->tailSpriteP = oldSpriteP->prevSpriteP;
        else
            oldSpriteP->nextSpriteP->prevSpriteP = oldSpriteP->prevSpriteP;
        if (oldSpriteP->prevSpriteP == NULL)
            layerP->headSpriteP = oldSpriteP->nextSpriteP;
        else
            oldSpriteP->prevSpriteP->nextSpriteP = oldSpriteP->nextSpriteP;
        oldSpriteP->parentSpriteLayerP = NULL;
    }
    SWSetStickyIfError(err);
    return err;
}

void SWRemoveAllSpritesFromLayer(SpriteLayerPtr srcSpriteLayerP)
{
    SpritePtr spriteP;

    SW_ASSERT(srcSpriteLayerP != NULL);
    while ((spriteP = SWGetNextSprite(srcSpriteLayerP, NULL)) != NULL)
        SWRemoveSprite(spriteP);
}

void SWDisposeAllSpritesInLayer(SpriteLayerPtr spriteLayerP)
{
    SpritePtr spriteP, nextSpriteP;

    SW_ASSERT(spriteLayerP != NULL);
    spriteP = spriteLayerP->headSpriteP;
    while (spriteP != NULL) {
        nextSpriteP = spriteP->nextSpriteP;
        SWRemoveSprite(spriteP);
        SWDisposeSprite(&spriteP);
        spriteP = nextSpriteP;
    }
}

int SWCountNumSpritesInLayer(SpriteLayerPtr spriteLayerP)
{
    SpritePtr spriteP;
    int count = 0;

    SW_ASSERT(spriteLayerP != NULL);
    for (spriteP = spriteLayerP->headSpriteP; spriteP != NULL; spriteP = spriteP->nextSpriteP)
        count++;
    return count;
}

/// Exchanges the positions of two sprites in their (shared) layer.
SWError SWSwapSprite(SpritePtr srcSpriteP, SpritePtr dstSpriteP)
{
    SpriteLayerPtr layerP;
    SpritePtr swap;

    SW_ASSERT(srcSpriteP != NULL && dstSpriteP != NULL);
    if (srcSpriteP == dstSpriteP)
        return kNoError;
    layerP = srcSpriteP->parentSpriteLayerP;
    if (dstSpriteP->parentSpriteLayerP != layerP || layerP == NULL) {
        SWSetStickyIfError(kBadParameterErr);
        return kBadParameterErr;
    }

    if (srcSpriteP->nextSpriteP == dstSpriteP) {
        if (srcSpriteP->prevSpriteP != NULL)
            srcSpriteP->prevSpriteP->nextSpriteP = dstSpriteP;
        if (dstSpriteP->nextSpriteP != NULL)
            dstSpriteP->nextSpriteP->prevSpriteP = srcSpriteP;
        dstSpriteP->prevSpriteP = srcSpriteP->prevSpriteP;
        srcSpriteP->nextSpriteP = dstSpriteP->nextSpriteP;
        dstSpriteP->nextSpriteP = srcSpriteP;
        srcSpriteP->prevSpriteP = dstSpriteP;
    } else if (dstSpriteP->nextSpriteP == srcSpriteP) {
        if (dstSpriteP->prevSpriteP != NULL)
            dstSpriteP->prevSpriteP->nextSpriteP = srcSpriteP;
        if (srcSpriteP->nextSpriteP != NULL)
            srcSpriteP->nextSpriteP->prevSpriteP = dstSpriteP;
        srcSpriteP->prevSpriteP = dstSpriteP->prevSpriteP;
        dstSpriteP->nextSpriteP = srcSpriteP->nextSpriteP;
        srcSpriteP->nextSpriteP = dstSpriteP;
        dstSpriteP->prevSpriteP = srcSpriteP;
    } else {
        if (srcSpriteP->prevSpriteP != NULL)
            srcSpriteP->prevSpriteP->nextSpriteP = dstSpriteP;
        if (dstSpriteP->prevSpriteP != NULL)
            dstSpriteP->prevSpriteP->nextSpriteP = srcSpriteP;
        if (srcSpriteP->nextSpriteP != NULL)
            srcSpriteP->nextSpriteP->prevSpriteP = dstSpriteP;
        if (dstSpriteP->nextSpriteP != NULL)
            dstSpriteP->nextSpriteP->prevSpriteP = srcSpriteP;
        swap = srcSpriteP->nextSpriteP;
        srcSpriteP->nextSpriteP = dstSpriteP->nextSpriteP;
        dstSpriteP->nextSpriteP = swap;
        swap = srcSpriteP->prevSpriteP;
        srcSpriteP->prevSpriteP = dstSpriteP->prevSpriteP;
        dstSpriteP->prevSpriteP = swap;
    }

    if (srcSpriteP->nextSpriteP == NULL)
        layerP->tailSpriteP = srcSpriteP;
    else if (srcSpriteP->prevSpriteP == NULL)
        layerP->headSpriteP = srcSpriteP;
    if (dstSpriteP->nextSpriteP == NULL)
        layerP->tailSpriteP = dstSpriteP;
    else if (dstSpriteP->prevSpriteP == NULL)
        layerP->headSpriteP = dstSpriteP;
    return kNoError;
}

SWError SWInsertSpriteAfterSprite(SpritePtr newSpriteP, SpritePtr dstSpriteP)
{
    SpriteLayerPtr layerP;
    SWError err = kNoError;

    SW_ASSERT(newSpriteP != NULL && dstSpriteP != NULL);
    layerP = dstSpriteP->parentSpriteLayerP;
    if (newSpriteP->parentSpriteLayerP != NULL)
        err = kSpriteAlreadyInLayer;
    if (err == kNoError) {
        if (dstSpriteP->nextSpriteP != NULL)
            dstSpriteP->nextSpriteP->prevSpriteP = newSpriteP;
        newSpriteP->nextSpriteP = dstSpriteP->nextSpriteP;
        dstSpriteP->nextSpriteP = newSpriteP;
        newSpriteP->prevSpriteP = dstSpriteP;
        if (newSpriteP->nextSpriteP == NULL)
            layerP->tailSpriteP = newSpriteP;
        newSpriteP->needsToBeDrawn = true;
        newSpriteP->parentSpriteLayerP = layerP;
    }
    SWSetStickyIfError(err);
    return err;
}

SWError SWInsertSpriteBeforeSprite(SpritePtr newSpriteP, SpritePtr dstSpriteP)
{
    SpriteLayerPtr layerP;
    SWError err = kNoError;

    SW_ASSERT(newSpriteP != NULL && dstSpriteP != NULL);
    layerP = dstSpriteP->parentSpriteLayerP;
    if (newSpriteP->parentSpriteLayerP != NULL)
        err = kSpriteAlreadyInLayer;
    if (err == kNoError) {
        if (dstSpriteP->prevSpriteP != NULL)
            dstSpriteP->prevSpriteP->nextSpriteP = newSpriteP;
        newSpriteP->prevSpriteP = dstSpriteP->prevSpriteP;
        dstSpriteP->prevSpriteP = newSpriteP;
        newSpriteP->nextSpriteP = dstSpriteP;
        if (newSpriteP->prevSpriteP == NULL)
            layerP->headSpriteP = newSpriteP;
        newSpriteP->needsToBeDrawn = true;
        newSpriteP->parentSpriteLayerP = layerP;
    }
    SWSetStickyIfError(err);
    return err;
}

SpritePtr SWGetNextSprite(SpriteLayerPtr spriteLayerP, SpritePtr curSpriteP)
{
    SW_ASSERT(spriteLayerP != NULL);
    return curSpriteP ? curSpriteP->nextSpriteP : spriteLayerP->headSpriteP;
}

SpritePtr SWGetPreviousSprite(SpriteLayerPtr spriteLayerP, SpritePtr curSpriteP)
{
    SW_ASSERT(spriteLayerP != NULL);
    return curSpriteP ? curSpriteP->prevSpriteP : spriteLayerP->tailSpriteP;
}

void SWLockSpriteLayer(SpriteLayerPtr spriteLayerP)
{
    SpritePtr spriteP;

    SW_ASSERT(spriteLayerP != NULL);
    for (spriteP = spriteLayerP->headSpriteP; spriteP != NULL; spriteP = spriteP->nextSpriteP)
        SWLockSprite(spriteP);
}

void SWUnlockSpriteLayer(SpriteLayerPtr spriteLayerP)
{
    SpritePtr spriteP;

    SW_ASSERT(spriteLayerP != NULL);
    for (spriteP = spriteLayerP->headSpriteP; spriteP != NULL; spriteP = spriteP->nextSpriteP)
        SWUnlockSprite(spriteP);
}

/*  Calls each source sprite's collide proc for every destination sprite it overlaps
    (using the frames' collision insets when set). Restarts a list from its head when
    the next sprite was removed by a collide proc. */
void SWCollideSpriteLayer(SpriteWorldPtr spriteWorldP, SpriteLayerPtr srcSpriteLayerP, SpriteLayerPtr dstSpriteLayerP)
{
    SpritePtr srcSpriteP, nextSrcSpriteP, dstSpriteP, nextDstSpriteP;
    SWRect sectRect;

    SW_ASSERT(spriteWorldP != NULL);
    SW_ASSERT(srcSpriteLayerP != NULL && dstSpriteLayerP != NULL);
    if (!spriteWorldP->frameHasOccurred)
        return;

    srcSpriteP = srcSpriteLayerP->headSpriteP;
    while (srcSpriteP != NULL) {
        dstSpriteP = dstSpriteLayerP->headSpriteP;
        nextSrcSpriteP = srcSpriteP->nextSpriteP;
        while (dstSpriteP != NULL) {
            SWRect *s = &srcSpriteP->destFrameRect, *d = &dstSpriteP->destFrameRect;
            nextDstSpriteP = dstSpriteP->nextSpriteP;
            if (srcSpriteP != dstSpriteP && SW_RECT_IS_IN_RECT(*s, *d)) {
                SWRect *si = &srcSpriteP->curFrameP->collisionInset;
                SWRect *di = &dstSpriteP->curFrameP->collisionInset;
                int collided;

                if (!srcSpriteP->curFrameP->usesCollisionInset && !dstSpriteP->curFrameP->usesCollisionInset)
                    collided = true;
                else
                    collided = s->top + si->top < d->bottom - di->bottom &&
                               d->top + di->top < s->bottom - si->bottom &&
                               s->left + si->left < d->right - di->right &&
                               d->left + di->left < s->right - si->right;

                if (collided && srcSpriteP->spriteCollideProc != NULL) {
                    sectRect.left = (short)SW_MAX(d->left + di->left, s->left + si->left);
                    sectRect.top = (short)SW_MAX(d->top + di->top, s->top + si->top);
                    sectRect.right = (short)SW_MIN(d->right - di->right, s->right - si->right);
                    sectRect.bottom = (short)SW_MIN(d->bottom - di->bottom, s->bottom - si->bottom);
                    srcSpriteP->spriteCollideProc(srcSpriteP, dstSpriteP, &sectRect);
                    if (srcSpriteP->spriteRemoval != kDontRemoveSprite || srcSpriteP->parentSpriteLayerP == NULL)
                        break;
                }
            }
            if (nextDstSpriteP == NULL || nextDstSpriteP->spriteRemoval == kDontRemoveSprite)
                dstSpriteP = nextDstSpriteP;
            else
                dstSpriteP = dstSpriteLayerP->headSpriteP;
        }
        if (nextSrcSpriteP == NULL || nextSrcSpriteP->spriteRemoval == kDontRemoveSprite)
            srcSpriteP = nextSrcSpriteP;
        else
            srcSpriteP = srcSpriteLayerP->headSpriteP;
    }
}

SWBoolean SWSpriteCollidesWithLayer(SpritePtr srcSpriteP, SpriteLayerPtr spriteLayerP, SpriteCollisionProcPtr collideFunc)
{
    SpritePtr spriteP;

    SW_ASSERT(srcSpriteP != NULL);
    SW_ASSERT(spriteLayerP != NULL);
    for (spriteP = spriteLayerP->headSpriteP; spriteP != NULL; spriteP = spriteP->nextSpriteP) {
        if (srcSpriteP != spriteP && SW_RECT_IS_IN_RECT(srcSpriteP->destFrameRect, spriteP->destFrameRect) &&
            (collideFunc == NULL || collideFunc(srcSpriteP, spriteP)))
            return true;
    }
    return false;
}

static short SWCompareSpriteBottom(SpritePtr a, SpritePtr b)
{
    return (short)(a->destFrameRect.bottom - b->destFrameRect.bottom);
}

void SWSortSpriteLayer(SpriteLayerPtr theLayer)
{
    SWCustomSortSpriteLayer(theLayer, SWCompareSpriteBottom);
}

/// Insertion sort; compareFunc returns > 0 when a should come after b.
void SWCustomSortSpriteLayer(SpriteLayerPtr theLayer, SpriteCompareProcPtr compareFunc)
{
    SpritePtr prevSpriteP, curSpriteP, searchSpriteP;

    SW_ASSERT(theLayer != NULL);
    SW_ASSERT(compareFunc != NULL);
    prevSpriteP = theLayer->headSpriteP;
    curSpriteP = SWGetNextSprite(theLayer, prevSpriteP);
    while (curSpriteP != NULL) {
        if (compareFunc(prevSpriteP, curSpriteP) < 1) {
            prevSpriteP = curSpriteP;
            curSpriteP = curSpriteP->nextSpriteP;
        } else {
            searchSpriteP = curSpriteP->prevSpriteP;
            while (searchSpriteP != NULL && compareFunc(searchSpriteP, curSpriteP) >= 1)
                searchSpriteP = searchSpriteP->prevSpriteP;
            SWRemoveSprite(curSpriteP);
            if (searchSpriteP == NULL)
                SWInsertSpriteBeforeSprite(curSpriteP, theLayer->headSpriteP);
            else
                SWInsertSpriteAfterSprite(curSpriteP, searchSpriteP);
            curSpriteP = SWGetNextSprite(theLayer, prevSpriteP);
        }
    }
}

void SWPauseSpriteLayer(SpriteLayerPtr spriteLayerP)
{
    SW_ASSERT(spriteLayerP != NULL);
    spriteLayerP->isPaused = true;
}

void SWUnpauseSpriteLayer(SpriteLayerPtr spriteLayerP)
{
    SW_ASSERT(spriteLayerP != NULL);
    spriteLayerP->isPaused = false;
}

void SWSetSpriteLayerVisible(SpriteLayerPtr spriteLayerP, SWBoolean visible)
{
    SW_ASSERT(spriteLayerP != NULL);
    spriteLayerP->layerIsVisible = visible;
}

SWBoolean SWGetSpriteLayerVisible(SpriteLayerPtr spriteLayerP)
{
    SW_ASSERT(spriteLayerP != NULL);
    return spriteLayerP->layerIsVisible;
}

/// Sets visibility for a run of layers, from firstLayerP through lastLayerP.
void SWSetSpriteLayersVisible(SpriteLayerPtr firstLayerP, SpriteLayerPtr lastLayerP, SWBoolean visible)
{
    SpriteLayerPtr layerP = firstLayerP;

    SW_ASSERT(firstLayerP != NULL);
    while (layerP != NULL) {
        layerP->layerIsVisible = visible;
        if (layerP == lastLayerP)
            break;
        layerP = layerP->nextSpriteLayerP;
    }
}

void SWSetAllSpriteLayersVisible(SpriteWorldPtr spriteWorldP, SWBoolean visible)
{
    SpriteLayerPtr layerP;

    for (layerP = spriteWorldP->headSpriteLayerP; layerP != NULL; layerP = layerP->nextSpriteLayerP)
        layerP->layerIsVisible = visible;
}

void SWSetLayerAsNonScrolling(SpriteLayerPtr spriteLayerP, int layerIsNonScrolling)
{
    SW_ASSERT(spriteLayerP != NULL);
    spriteLayerP->layerIsNonScrolling = layerIsNonScrolling;
}

SpritePtr SWFindSpriteByPoint(SpriteLayerPtr spriteLayerP, SpritePtr startSpriteP, int h, int v)
{
    return SWTopSpriteFromPoint(spriteLayerP, startSpriteP, h, v, NULL);
}

/// Searches from the top of the layer (tail) for a sprite containing the point.
SpritePtr SWTopSpriteFromPoint(SpriteLayerPtr spriteLayerP, SpritePtr startSpriteP, int h, int v, SpriteInMaskProcPtr testFunc)
{
    SpritePtr spriteP;

    SW_ASSERT(spriteLayerP != NULL);
    for (spriteP = SWGetPreviousSprite(spriteLayerP, startSpriteP); spriteP != NULL;
         spriteP = SWGetPreviousSprite(spriteLayerP, spriteP)) {
        if (SWIsPointInSpriteMask(spriteP, h, v, testFunc))
            return spriteP;
    }
    return NULL;
}

void SWMoveSpriteLayerToTail(SpriteWorldPtr spriteWorldP, SpriteLayerPtr moveSpriteLayerP)
{
    SpriteLayerPtr tailP;

    SW_ASSERT(spriteWorldP != NULL);
    tailP = spriteWorldP->tailSpriteLayerP;
    if (tailP == moveSpriteLayerP)
        return;
    if (moveSpriteLayerP->nextSpriteLayerP == NULL)
        spriteWorldP->tailSpriteLayerP = moveSpriteLayerP->prevSpriteLayerP;
    else
        moveSpriteLayerP->nextSpriteLayerP->prevSpriteLayerP = moveSpriteLayerP->prevSpriteLayerP;
    if (moveSpriteLayerP->prevSpriteLayerP == NULL)
        spriteWorldP->headSpriteLayerP = moveSpriteLayerP->nextSpriteLayerP;
    else
        moveSpriteLayerP->prevSpriteLayerP->nextSpriteLayerP = moveSpriteLayerP->nextSpriteLayerP;
    tailP->nextSpriteLayerP = moveSpriteLayerP;
    moveSpriteLayerP->prevSpriteLayerP = tailP;
    moveSpriteLayerP->nextSpriteLayerP = NULL;
    spriteWorldP->tailSpriteLayerP = moveSpriteLayerP;
}
