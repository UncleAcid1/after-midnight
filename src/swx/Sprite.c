/*
 * SpriteWorld X fork — sprites.
 * See SWCommonHeaders.h for license and provenance.
 */

#include "SpriteWorld.h"

static void SWCopyDestToOldAndDelta(SpritePtr spriteP)
{
    spriteP->deltaFrameRect = spriteP->destFrameRect;
    spriteP->oldFrameRect = spriteP->destFrameRect;
}

/*  Takes a sprite out of the animation at the next frame. The sprite moves to the
    world's dead layer and is removed (and optionally disposed) by SWProcessSpriteWorld. */
void SWRemoveSpriteFromAnimation(SpritePtr spriteP, SWBoolean disposeOfSprite)
{
    SpriteWorldPtr spriteWorldP;

    SW_ASSERT(spriteP != NULL);
    if (spriteP->spriteRemoval != kDontRemoveSprite || spriteP->parentSpriteLayerP == NULL)
        return;
    spriteWorldP = spriteP->parentSpriteLayerP->parentSpriteWorldP;
    if (spriteWorldP == NULL) {
        SWRemoveSprite(spriteP);
        if (disposeOfSprite)
            SWDisposeSprite(&spriteP);
    } else {
        SWSetSpriteFrameProc(spriteP, NULL);
        SWSetSpriteMoveProc(spriteP, NULL);
        SWSetSpriteVisible(spriteP, false);
        SWRemoveSprite(spriteP);
        SWAddSprite(spriteWorldP->deadSpriteLayerP, spriteP);
        spriteP->spriteRemoval = disposeOfSprite ? kRemoveAndDisposeSprite : kRemoveSprite;
    }
}

/*  Move and frame procs often dispose of their own sprite (or another one) while the layer
    is being processed, and the engine then still reads that sprite. The original got away
    with reading freed memory; here sprites disposed during processing are detached at once
    and freed when processing ends. */
static int        gSWDeferDisposalDepth = 0;
static SpritePtr *gSWDeferredSprites = NULL;
static int        gSWNumDeferredSprites = 0, gSWDeferredCapacity = 0;

static void SWDisposeSpriteNow(SpritePtr *deadSpritePP);

void SWBeginDeferredSpriteDisposal(void)
{
    gSWDeferDisposalDepth++;
}

void SWEndDeferredSpriteDisposal(void)
{
    int i;

    SW_ASSERT(gSWDeferDisposalDepth > 0);
    if (--gSWDeferDisposalDepth > 0)
        return;
    for (i = 0; i < gSWNumDeferredSprites; i++)
        SWDisposeSpriteNow(&gSWDeferredSprites[i]);
    gSWNumDeferredSprites = 0;
}

static void SWDeferSpriteDisposal(SpritePtr spriteP)
{
    SpritePtr *newArray;
    int i;

    for (i = 0; i < gSWNumDeferredSprites; i++)
        if (gSWDeferredSprites[i] == spriteP)
            return;
    if (gSWNumDeferredSprites == gSWDeferredCapacity) {
        newArray = (SpritePtr *)realloc(gSWDeferredSprites, (gSWDeferredCapacity + 64) * sizeof(SpritePtr));
        if (newArray == NULL) {
            SWSetStickyIfError(kMemoryAllocationError);
            return;
        }
        gSWDeferredSprites = newArray;
        gSWDeferredCapacity += 64;
    }
    gSWDeferredSprites[gSWNumDeferredSprites++] = spriteP;
    // Detach now, so nothing processes or draws it any more.
    if (spriteP->parentSpriteLayerP != NULL)
        SWRemoveSprite(spriteP);
    if (spriteP->mountParentP != NULL) {
        spriteP->mountParentP->mountedSpriteP = NULL;
        spriteP->mountParentP = NULL;
    }
    if (spriteP->mountedSpriteP != NULL && spriteP->mountedSpriteP->parentSpriteLayerP != NULL)
        SWRemoveSprite(spriteP->mountedSpriteP);
}

void SWDisposeSprite(SpritePtr *deadSpritePP)
{
    if (*deadSpritePP == NULL)
        return;
    if (gSWDeferDisposalDepth > 0) {
        SWDeferSpriteDisposal(*deadSpritePP);
        *deadSpritePP = NULL;
        return;
    }
    SWDisposeSpriteNow(deadSpritePP);
}

static void SWDisposeSpriteNow(SpritePtr *deadSpritePP)
{
    SpritePtr spriteP = *deadSpritePP;
    SWBoolean lastFrameFreed = false;
    long i;

    if (spriteP == NULL)
        return;
    for (i = 0; i < spriteP->numFrames; i++)
        lastFrameFreed = (SWBoolean)SWDisposeFrame(&spriteP->frameArray[i]);
    if (lastFrameFreed && spriteP->sharedSurface != NULL) {
        SDL_FreeSurface(spriteP->sharedSurface);
        spriteP->sharedSurface = NULL;
    }
    if (spriteP->mountedSpriteP != NULL) {
        if (spriteP->mountedSpriteP->parentSpriteLayerP != NULL)
            SWRemoveSprite(spriteP->mountedSpriteP);
        SWDisposeSprite(&spriteP->mountedSpriteP);
    }
    if (spriteP->mountParentP != NULL)
        spriteP->mountParentP->mountedSpriteP = NULL;
    free(spriteP->frameArray);
    free(spriteP->drawData);
    free(spriteP);
    *deadSpritePP = NULL;
}

void SWLockSprite(SpritePtr srcSpriteP)
{
    long i;
    SW_ASSERT(srcSpriteP != NULL);
    for (i = 0; i < srcSpriteP->numFrames; i++)
        SWLockFrame(srcSpriteP->frameArray[i]);
}

void SWUnlockSprite(SpritePtr srcSpriteP)
{
    long i;
    SW_ASSERT(srcSpriteP != NULL);
    for (i = 0; i < srcSpriteP->numFrames; i++)
        SWUnlockFrame(srcSpriteP->frameArray[i]);
}

SWError SWAddFrame(SpritePtr srcSpriteP, FramePtr newFrameP)
{
    SWError err = kNoError;

    SW_ASSERT(srcSpriteP != NULL && newFrameP != NULL);
    if (srcSpriteP->numFrames < srcSpriteP->maxFrames) {
        srcSpriteP->frameArray[srcSpriteP->numFrames] = newFrameP;
        if (srcSpriteP->numFrames == 0)
            srcSpriteP->curFrameP = newFrameP;
        srcSpriteP->numFrames++;
        newFrameP->useCount++;
    } else {
        err = kMaxFramesErr;
    }
    SWSetStickyIfError(err);
    return err;
}

SWError SWInsertFrame(SpritePtr srcSpriteP, FramePtr newFrameP, long frameIndex)
{
    SWError err = kNoError;
    long i;

    SW_ASSERT(srcSpriteP != NULL && newFrameP != NULL);
    if (frameIndex < srcSpriteP->maxFrames && frameIndex >= 0 && srcSpriteP->numFrames < srcSpriteP->maxFrames) {
        for (i = srcSpriteP->numFrames; i > frameIndex; i--)
            srcSpriteP->frameArray[i] = srcSpriteP->frameArray[i - 1];
        srcSpriteP->frameArray[frameIndex] = newFrameP;
        srcSpriteP->numFrames++;
        newFrameP->useCount++;
    } else {
        err = kMaxFramesErr;
    }
    SWSetStickyIfError(err);
    return err;
}

void SWRemoveFrame(SpritePtr srcSpriteP, FramePtr oldFrameP)
{
    long i;

    SW_ASSERT(srcSpriteP != NULL && oldFrameP != NULL);
    for (i = srcSpriteP->numFrames - 1; i >= 0; i--) {
        if (srcSpriteP->frameArray[i] == oldFrameP)
            break;
    }
    if (i < 0)
        return;
    srcSpriteP->numFrames--;
    oldFrameP->useCount--;
    for (; i <= srcSpriteP->numFrames - 1; i++)
        srcSpriteP->frameArray[i] = srcSpriteP->frameArray[i + 1];
}

FramePtr SWGetNextFrame(SpritePtr srcSpriteP, FramePtr curFrameP)
{
    unsigned char i;

    SW_ASSERT(srcSpriteP != NULL);
    if (curFrameP == NULL)
        return srcSpriteP->frameArray[0];
    for (i = 0; i < srcSpriteP->numFrames - 2; i++) {
        if (srcSpriteP->frameArray[i] == curFrameP)
            return srcSpriteP->frameArray[i + 1];
    }
    return NULL;
}

void SWSetSpriteDrawProc(SpritePtr srcSpriteP, DrawProcPtr drawProc)
{
    srcSpriteP->frameDrawProc = drawProc;
}

/*  Sets up the modelview matrix for a sprite at (horizLoc, vertLoc): design-resolution
    scaling, world zoom, scroll offset, flips, rotation and scaling. Shared by the draw
    procs; the caller pops the matrix. */
static void SWPushSpriteMatrix(SpritePtr srcSpriteP, DrawDataPtr d, float horizLoc, float vertLoc, SWBoolean full)
{
    SpriteWorldPtr world = gSWCurrentSpriteWorld;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    if (world != NULL) {
        if (!full || d->autoScaling)
            glScalef(world->designScaleH, world->designScaleV, 1.0f);
        if (full)
            glScalef(world->zoomH, world->zoomV, 1.0f);
    }
    if (full) {
        if (world != NULL)
            glTranslatef(-world->spriteOffsetH, -world->spriteOffsetV, 0);
        if (d->nonScrolling && world != NULL)
            glTranslatef(world->spriteOffsetH, world->spriteOffsetV, 0);
    }
    glTranslatef(horizLoc, vertLoc, 0);
    if (full) {
        if (d->flippedVertical) {
            glTranslatef(0, SWGetSpriteHeight(srcSpriteP), 0);
            glRotatef(180.0f, 1.0f, 0, 0);
        }
        if (d->flippedHorizontal) {
            glTranslatef(SWGetSpriteWidth(srcSpriteP), 0, 0);
            glRotatef(180.0f, 0, 1.0f, 0);
        }
        if (d->rotation != 0.0f) {
            glTranslatef(d->rotationOffsetHoriz, d->rotationOffsetVert, 0);
            glRotatef(d->rotation, 0, 0, 1.0f);
        }
    }
    if (d->scaledHeightFactor != 1.0f || d->scaledWidthFactor != 1.0f)
        glScalef(d->scaledWidthFactor, d->scaledHeightFactor, 1.0f);
}

void SWStdSpriteDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    DrawDataPtr d;
    SWBoolean blending = false;

    SW_ASSERT(srcSpriteP != NULL);
    if (curFrameP == NULL || curFrameP->glSurfaceP == NULL)
        return;
    d = srcSpriteP->drawData;
    glColor4us(d->colorRed, d->colorGreen, d->colorBlue, (GLushort)(d->translucencyLevel & 0xFFFF));
    SWPushSpriteMatrix(srcSpriteP, d, horizLoc, vertLoc, true);

    if (DoesSurfaceNeedBlend(curFrameP->glSurfaceP) || d->translucencyLevel != kStdTranslucency || d->useBlendMode) {
        glEnable(GL_BLEND);
        glBlendFunc(d->srcBlendMode, d->dstBlendMode);
        blending = true;
    }
    if (!d->useClip)
        BKBlitGLSurface(curFrameP->glSurfaceP);
    else
        BKBlitClippedGLSurface(curFrameP->glSurfaceP, d->clipLeft, d->clipTop, d->clipRight, d->clipBottom);
    if (blending)
        glDisable(GL_BLEND);
    if (d->useClip && d->temporaryClip)
        d->useClip = false;
    glPopMatrix();
}

/// Draws a filled rectangle the size of the sprite in the sprite's color.
void SWSolidColorSpriteDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    DrawDataPtr d;
    short width, height;

    SW_ASSERT(srcSpriteP != NULL);
    d = srcSpriteP->drawData;
    glDisable(GL_TEXTURE_2D);
    SWPushSpriteMatrix(srcSpriteP, d, horizLoc, vertLoc, false);
    glColor4us(d->colorRed, d->colorGreen, d->colorBlue, (GLushort)(d->translucencyLevel & 0xFFFF));
    glEnable(GL_BLEND);
    glBlendFunc(d->srcBlendMode, d->dstBlendMode);
    SW_UNUSED(curFrameP);
    width = srcSpriteP->destFrameRect.right - srcSpriteP->destFrameRect.left;
    height = srcSpriteP->destFrameRect.bottom - srcSpriteP->destFrameRect.top;
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(width, 0);
    glVertex2i(width, height);
    glVertex2i(0, height);
    glEnd();
    glPopMatrix();
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}

/*  Installed as spriteMoveProc for new and cloned sprites: the first call switches to the
    real move proc, so a sprite's move proc does not run in the frame it was created in
    unless it is visible-and-drawn already. */
void SWTempSpriteMoveProc(SpritePtr srcSpriteP)
{
    srcSpriteP->spriteMoveProc = srcSpriteP->realSpriteMoveProc;
    if (!srcSpriteP->needsToBeDrawn && srcSpriteP->realSpriteMoveProc != NULL)
        srcSpriteP->realSpriteMoveProc(srcSpriteP);
}

SWError SWSetCurrentFrame(SpritePtr srcSpriteP, FramePtr curFrameP)
{
    short i;

    SW_ASSERT(srcSpriteP != NULL && curFrameP != NULL);
    for (i = 0; i < srcSpriteP->numFrames; i++) {
        if (srcSpriteP->frameArray[i] == curFrameP) {
            SWSetCurrentFrameIndex(srcSpriteP, i);
            return kNoError;
        }
    }
    SWSetStickyIfError(kBadParameterErr);
    return kBadParameterErr;
}

/*  Switches frames, keeping the sprite's hotspot position: the new frame is placed so
    that its hotspot lands where the old one was. Applies scaled size if set. */
SWError SWSetCurrentFrameIndex(SpritePtr srcSpriteP, long frameIndex)
{
    FramePtr newFrameP;
    short offsetH, offsetV;

    SW_ASSERT(srcSpriteP != NULL);
    SW_ASSERT(frameIndex >= 0);
    if (frameIndex >= srcSpriteP->numFrames)
        return kInvalidFramesIndexErr;

    newFrameP = srcSpriteP->frameArray[frameIndex];
    if (srcSpriteP->curFrameP == NULL)
        srcSpriteP->curFrameP = newFrameP;

    offsetH = (srcSpriteP->destFrameRect.left - newFrameP->frameRect.left) +
              ((short)srcSpriteP->curFrameP->hotSpotH - (short)newFrameP->hotSpotH);
    offsetV = (srcSpriteP->destFrameRect.top - newFrameP->frameRect.top) +
              ((short)srcSpriteP->curFrameP->hotSpotV - (short)newFrameP->hotSpotV);
    srcSpriteP->curFrameP = newFrameP;
    srcSpriteP->destFrameRect = newFrameP->frameRect;

    if (srcSpriteP->drawData->scaledWidth > 0.0f) {
        srcSpriteP->destFrameRect.right = (short)(int)(srcSpriteP->drawData->scaledWidth +
                                                       (float)srcSpriteP->destFrameRect.left);
        srcSpriteP->drawData->scaledWidthFactor = srcSpriteP->drawData->scaledWidth /
                    (float)(newFrameP->frameRect.right - newFrameP->frameRect.left);
    }
    if (srcSpriteP->drawData->scaledHeight > 0.0f) {
        srcSpriteP->destFrameRect.bottom = (short)(int)(srcSpriteP->drawData->scaledHeight +
                                                        (float)srcSpriteP->destFrameRect.top);
        srcSpriteP->drawData->scaledHeightFactor = srcSpriteP->drawData->scaledHeight /
                    (float)(newFrameP->frameRect.bottom - newFrameP->frameRect.top);
    }
    srcSpriteP->drawData->frameHotSpotH = (short)srcSpriteP->curFrameP->hotSpotH;
    srcSpriteP->drawData->frameHotSpotV = (short)srcSpriteP->curFrameP->hotSpotV;
    SW_OFFSET_RECT(srcSpriteP->destFrameRect, offsetH, offsetV);
    srcSpriteP->curFrameIndex = frameIndex;
    srcSpriteP->needsToBeDrawn = true;
    return kNoError;
}

void SWSetSpriteCollideProc(SpritePtr srcSpriteP, CollideProcPtr collideProc)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->spriteCollideProc = collideProc;
}

void SWSetSpriteFrameAdvance(SpritePtr srcSpriteP, long frameAdvance)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->frameAdvance = frameAdvance;
}

void SWSetSpriteFrameAdvanceMode(SpritePtr srcSpriteP, int advanceMode)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->frameAdvanceMode = advanceMode;
}

void SWSetSpriteFrameRange(SpritePtr srcSpriteP, long firstFrameIndex, long lastFrameIndex)
{
    SW_ASSERT(srcSpriteP != NULL);
    if (firstFrameIndex < 0)
        firstFrameIndex = 0;
    if (lastFrameIndex > srcSpriteP->maxFrames - 1)
        lastFrameIndex = srcSpriteP->maxFrames - 1;
    srcSpriteP->firstFrameIndex = firstFrameIndex;
    srcSpriteP->lastFrameIndex = lastFrameIndex;
    if (srcSpriteP->curFrameIndex < firstFrameIndex)
        SWSetCurrentFrameIndex(srcSpriteP, firstFrameIndex);
    else if (srcSpriteP->curFrameIndex > lastFrameIndex)
        SWSetCurrentFrameIndex(srcSpriteP, lastFrameIndex);
}

/// Sets ms between frame changes and restarts the frame timer.
void SWSetSpriteFrameTime(SpritePtr srcSpriteP, long timeInterval)
{
    SpriteWorldPtr world = NULL;

    SW_ASSERT(srcSpriteP != NULL);
    if (srcSpriteP->frameTimeInterval == timeInterval)
        return;
    srcSpriteP->frameTimeInterval = timeInterval;
    if (srcSpriteP->parentSpriteLayerP != NULL)
        world = srcSpriteP->parentSpriteLayerP->parentSpriteWorldP;
    if (world == NULL)
        world = gSWCurrentSpriteWorld;
    if (world != NULL)
        srcSpriteP->timeOfLastFrameChange = world->runningTimeCount;
}

void SWSetSpriteFrameProc(SpritePtr srcSpriteP, FrameProcPtr frameProc)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->frameChangeProc = frameProc;
}

/// Places the sprite's hotspot at (horizLoc, vertLoc) without counting it as movement.
void SWSetSpriteLocation(SpritePtr srcSpriteP, SWMovementType horizLoc, SWMovementType vertLoc)
{
    short left, top;

    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->drawData->horizLoc = horizLoc;
    srcSpriteP->drawData->vertLoc = vertLoc;
    left = (short)(int)horizLoc - srcSpriteP->drawData->frameHotSpotH;
    top = (short)(int)vertLoc - srcSpriteP->drawData->frameHotSpotV;
    srcSpriteP->destFrameRect.bottom = (srcSpriteP->destFrameRect.bottom - srcSpriteP->destFrameRect.top) + top;
    srcSpriteP->destFrameRect.right = (srcSpriteP->destFrameRect.right - srcSpriteP->destFrameRect.left) + left;
    srcSpriteP->destFrameRect.top = top;
    srcSpriteP->destFrameRect.left = left;
    SWCopyDestToOldAndDelta(srcSpriteP);
    srcSpriteP->needsToBeDrawn = true;
}

void SWSetSpriteMoveBounds(SpritePtr srcSpriteP, SWRect *moveBoundsRect)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->moveBoundsRect = *moveBoundsRect;
}

void SWSetSpriteMoveDelta(SpritePtr srcSpriteP, SWMovementType horizDelta, SWMovementType vertDelta)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->drawData->horizMoveDelta = horizDelta;
    srcSpriteP->drawData->vertMoveDelta = vertDelta;
}

void SWSetSpriteMoveTime(SpritePtr srcSpriteP, long timeInterval)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->moveTimeInterval = timeInterval;
}

long SWGetSpriteMoveTime(SpritePtr srcSpriteP)
{
    SW_ASSERT(srcSpriteP != NULL);
    return srcSpriteP->moveTimeInterval;
}

void SWSetSpriteMoveProc(SpritePtr srcSpriteP, MoveProcPtr moveProc)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->realSpriteMoveProc = moveProc;
    if (srcSpriteP->spriteMoveProc != SWTempSpriteMoveProc)
        srcSpriteP->spriteMoveProc = moveProc;
}

void SWSetSpritePseudoMoveTime(SpritePtr srcSpriteP, long timeInterval)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->pseudoMoveTimeInterval = (double)timeInterval;
}

void SWSetSpriteVisible(SpritePtr srcSpriteP, SWBoolean isVisible)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->isVisible = isVisible;
    srcSpriteP->needsToBeDrawn = true;
    srcSpriteP->needsToBeErased = !isVisible;
    if (srcSpriteP->mountedSpriteP != NULL && srcSpriteP->inheritVisible)
        SWSetSpriteVisible(srcSpriteP->mountedSpriteP, isVisible);
}

void SWSetSpriteTranslucency(SpritePtr srcSpriteP, unsigned long translucencyLevel)
{
    SWSetSpriteTranslucencyLevel(srcSpriteP, translucencyLevel);
}

void SWSetSpriteTranslucencyLevel(SpritePtr srcSpriteP, unsigned long translucencyLevel)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->drawData->translucencyLevel = translucencyLevel;
    if (srcSpriteP->mountedSpriteP != NULL && srcSpriteP->inheritTranslucency)
        SWSetSpriteTranslucencyLevel(srcSpriteP->mountedSpriteP, translucencyLevel);
}

void SWSetSpriteColor(SpritePtr srcSpriteP, unsigned short red, unsigned short green, unsigned short blue)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->drawData->colorRed = red;
    srcSpriteP->drawData->colorGreen = green;
    srcSpriteP->drawData->colorBlue = blue;
    if (srcSpriteP->mountedSpriteP != NULL && srcSpriteP->inheritColor)
        SWSetSpriteColor(srcSpriteP->mountedSpriteP, red, green, blue);
}

void SWSetSpriteBlendMode(SpritePtr srcSpriteP, unsigned int srcMode, unsigned int dstMode)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->drawData->useBlendMode = true;
    srcSpriteP->drawData->srcBlendMode = srcMode;
    srcSpriteP->drawData->dstBlendMode = dstMode;
}

/// Clips the sprite for the next draw only.
void SWSetSpriteTemporaryClip(SpritePtr srcSpriteP, short left, short top, short right, short bottom)
{
    srcSpriteP->drawData->temporaryClip = true;
    SWSetSpriteClip(srcSpriteP, left, top, right, bottom);
}

/// Insets (in pixels) to cut from each edge when drawing; all zero turns clipping off.
void SWSetSpriteClip(SpritePtr srcSpriteP, short left, short top, short right, short bottom)
{
    DrawDataPtr d;

    SW_ASSERT(srcSpriteP != NULL);
    d = srcSpriteP->drawData;
    if (left == 0 && top == 0 && right == 0 && bottom == 0) {
        d->useClip = false;
    } else {
        d->useClip = true;
        d->clipLeft = left;
        d->clipTop = top;
        d->clipRight = right;
        d->clipBottom = bottom;
    }
}

void SWSetSpriteFlipX(SpritePtr srcSpriteP, SWBoolean flip)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->drawData->flippedHorizontal = flip;
    if (srcSpriteP->mountedSpriteP != NULL && srcSpriteP->inheritFlip)
        SWSetSpriteFlipX(srcSpriteP->mountedSpriteP, flip);
}

void SWSetSpriteFlipY(SpritePtr srcSpriteP, SWBoolean flip)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->drawData->flippedVertical = flip;
    // The fork passes the flip on with SWSetSpriteFlipX here; kept as is.
    if (srcSpriteP->mountedSpriteP != NULL && srcSpriteP->inheritFlip)
        SWSetSpriteFlipX(srcSpriteP->mountedSpriteP, flip);
}

/// Rotates the sprite about its center (degrees).
void SWSetSpriteRotation(SpritePtr srcSpriteP, float rotation)
{
    float halfW, halfH, s, c;

    SW_ASSERT(srcSpriteP != NULL);
    halfW = (float)((srcSpriteP->destFrameRect.right - srcSpriteP->destFrameRect.left) / 2);
    halfH = (float)((srcSpriteP->destFrameRect.bottom - srcSpriteP->destFrameRect.top) / 2);
    s = (float)SWSin(rotation);
    c = (float)SWCos(rotation);
    srcSpriteP->drawData->rotationOffsetHoriz = -s * halfH + -c * halfW + halfW;
    srcSpriteP->drawData->rotationOffsetVert = -c * halfH + halfW * s + halfH;
    srcSpriteP->drawData->rotation = rotation;
    // The fork passes the rotation on through SWSetSpriteFlipX; kept as is.
    if (srcSpriteP->mountedSpriteP != NULL && srcSpriteP->inheritRotation)
        SWSetSpriteFlipX(srcSpriteP->mountedSpriteP, (SWBoolean)(int)rotation);
}

void SWSetSpriteScaledSize(SpritePtr srcSpriteP, float width, float height)
{
    SW_ASSERT(srcSpriteP != NULL);
    SWSetSpriteScaledWidth(srcSpriteP, width);
    SWSetSpriteScaledHeight(srcSpriteP, height);
}

void SWSetSpriteScaledWidth(SpritePtr srcSpriteP, float width)
{
    float oldFactor;

    SW_ASSERT(srcSpriteP != NULL);
    oldFactor = srcSpriteP->drawData->scaledWidthFactor;
    srcSpriteP->drawData->scaledWidth = width;
    SWSetCurrentFrameIndex(srcSpriteP, srcSpriteP->curFrameIndex);
    if (srcSpriteP->scalingMode)
        SWOffsetSprite(srcSpriteP, -((srcSpriteP->drawData->scaledWidthFactor * srcSpriteP->unscaledWidth -
                                      srcSpriteP->unscaledWidth * oldFactor) / 2.0f), 0.0f);
    if (srcSpriteP->mountedSpriteP != NULL && srcSpriteP->inheritScale)
        SWSetSpriteScaledWidth(srcSpriteP->mountedSpriteP, width);
}

void SWSetSpriteScaledHeight(SpritePtr srcSpriteP, float height)
{
    float oldFactor;

    SW_ASSERT(srcSpriteP != NULL);
    oldFactor = srcSpriteP->drawData->scaledHeightFactor;
    srcSpriteP->drawData->scaledHeight = height;
    SWSetCurrentFrameIndex(srcSpriteP, srcSpriteP->curFrameIndex);
    if (srcSpriteP->scalingMode)
        SWOffsetSprite(srcSpriteP, 0.0f, -((srcSpriteP->drawData->scaledHeightFactor * srcSpriteP->unscaledHeight -
                                            srcSpriteP->unscaledHeight * oldFactor) / 2.0f));
    if (srcSpriteP->mountedSpriteP != NULL && srcSpriteP->inheritScale)
        SWSetSpriteScaledHeight(srcSpriteP->mountedSpriteP, height);
}

void SWSetSpriteScaledWidthFactor(SpritePtr srcSpriteP, float factor)
{
    SW_ASSERT(srcSpriteP != NULL);
    SWSetSpriteScaledWidth(srcSpriteP, srcSpriteP->unscaledWidth * factor);
}

void SWSetSpriteScaledHeightFactor(SpritePtr srcSpriteP, float factor)
{
    SW_ASSERT(srcSpriteP != NULL);
    SWSetSpriteScaledHeight(srcSpriteP, srcSpriteP->unscaledHeight * factor);
}

void SWSetSpriteScaledFactor(SpritePtr srcSpriteP, float widthFactor, float heightFactor)
{
    SW_ASSERT(srcSpriteP != NULL);
    SWSetSpriteScaledWidth(srcSpriteP, srcSpriteP->unscaledWidth * widthFactor);
    SWSetSpriteScaledHeight(srcSpriteP, srcSpriteP->unscaledHeight * heightFactor);
}

void SWSetSpriteScalingMode(SpritePtr srcSpriteP, SWBoolean centered)
{
    srcSpriteP->scalingMode = centered;
}

void SWSetSpriteFastMovingCheck(SpritePtr srcSpriteP, SWBoolean check)
{
    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->doFastMovingCheck = check;
}

void SWInvalidateSprite(SpritePtr srcSpriteP)
{
    srcSpriteP->needsToBeDrawn = true;
}

/*  Mounts riderSpriteP on baseSpriteP: the rider is moved with the base each frame
    (at the given offset, or the world's global mount offset), optionally takes the
    base's frame index and is drawn right after it, and by default inherits the base's
    translucency, color, scale, flip, rotation and visibility. */
void SWMountSprite(SpritePtr baseSpriteP, SpritePtr riderSpriteP, float offsetH, float offsetV,
                   SWBoolean copyFrame, SWBoolean drawRider, SWBoolean useGlobalOffset)
{
    baseSpriteP->mountedSpriteP = riderSpriteP;
    riderSpriteP->mountParentP = baseSpriteP;
    riderSpriteP->mountOffsetH = offsetH;
    riderSpriteP->mountOffsetV = offsetV;
    baseSpriteP->mountCopiesFrame = copyFrame;
    baseSpriteP->mountIsDrawn = drawRider;
    baseSpriteP->mountUsesGlobalOffset = useGlobalOffset;
    baseSpriteP->inheritTranslucency = true;
    baseSpriteP->inheritColor = true;
    baseSpriteP->inheritScale = true;
    baseSpriteP->inheritFlip = true;
    baseSpriteP->inheritRotation = true;
    baseSpriteP->inheritVisible = true;
}

void SWSetMountInheritsAttributes(SpritePtr spriteP, SWBoolean translucency, SWBoolean color,
                                  SWBoolean scale, SWBoolean flip, SWBoolean rotation, SWBoolean visible)
{
    spriteP->inheritTranslucency = translucency;
    spriteP->inheritColor = color;
    spriteP->inheritScale = scale;
    spriteP->inheritFlip = flip;
    spriteP->inheritRotation = rotation;
    spriteP->inheritVisible = visible;
}

void SWSetSpriteMountOffset(SpritePtr spriteP, float offsetH, float offsetV)
{
    spriteP->mountOffsetH = offsetH;
    spriteP->mountOffsetV = offsetV;
}

void SWSetGlobalMountOffset(SpriteWorldPtr spriteWorldP, float offsetH, float offsetV)
{
    spriteWorldP->globalMountOffsetH = offsetH;
    spriteWorldP->globalMountOffsetV = offsetV;
}

/// Tiles in layers above the sprite are redrawn over it, inset by these amounts.
void SWSetDrawTilesOverSpriteInsetRect(SpritePtr spriteP, short left, short top, short right, short bottom)
{
    SW_ASSERT(spriteP != NULL);
    if (left == 0 && top == 0 && right == 0 && bottom == 0) {
        spriteP->drawTilesOverSprite = false;
    } else {
        spriteP->drawTilesOverSprite = true;
        spriteP->drawTilesInsetRect.left = left;
        spriteP->drawTilesInsetRect.top = top;
        spriteP->drawTilesInsetRect.right = right;
        spriteP->drawTilesInsetRect.bottom = bottom;
    }
}

void SWSetSpriteAutoScaling(SpritePtr spriteP, SWBoolean autoScale)
{
    spriteP->drawData->autoScaling = autoScale;
}

void SWSetSpriteNonScrolling(SpritePtr spriteP, SWBoolean nonScrolling)
{
    spriteP->drawData->nonScrolling = nonScrolling;
}

long SWGetSpriteFrameTime(SpritePtr srcSpriteP)
{
    SW_ASSERT(srcSpriteP != NULL);
    return srcSpriteP->frameTimeInterval;
}

SWMovementType SWGetSpriteVertLoc(SpritePtr srcSpriteP)
{
    SW_ASSERT(srcSpriteP != NULL);
    return srcSpriteP->drawData->vertLoc;
}

SWMovementType SWGetSpriteHorizLoc(SpritePtr srcSpriteP)
{
    SW_ASSERT(srcSpriteP != NULL);
    SW_ASSERT(srcSpriteP->drawData != NULL);
    return srcSpriteP->drawData->horizLoc;
}

SWMovementType SWGetSpriteHorizMoveDelta(SpritePtr srcSpriteP)
{
    SW_ASSERT(srcSpriteP != NULL);
    return srcSpriteP->drawData->horizMoveDelta;
}

SWMovementType SWGetSpriteVertMoveDelta(SpritePtr srcSpriteP)
{
    SW_ASSERT(srcSpriteP != NULL);
    return srcSpriteP->drawData->vertMoveDelta;
}

float SWGetSpriteWidth(SpritePtr srcSpriteP)
{
    SW_ASSERT(srcSpriteP != NULL);
    if (srcSpriteP->drawData->scaledWidth > 0.0f)
        return srcSpriteP->drawData->scaledWidth;
    return (float)(srcSpriteP->destFrameRect.right - srcSpriteP->destFrameRect.left);
}

float SWGetSpriteHeight(SpritePtr srcSpriteP)
{
    SW_ASSERT(srcSpriteP != NULL);
    if (srcSpriteP->drawData->scaledHeight > 0.0f)
        return srcSpriteP->drawData->scaledHeight;
    return (float)(srcSpriteP->destFrameRect.bottom - srcSpriteP->destFrameRect.top);
}

unsigned long SWGetSpriteTranslucencyLevel(SpritePtr srcSpriteP)
{
    SW_ASSERT(srcSpriteP != NULL);
    return srcSpriteP->drawData->translucencyLevel;
}

SWBoolean SWGetSpriteVisible(SpritePtr srcSpriteP)
{
    SW_ASSERT(srcSpriteP != NULL);
    return srcSpriteP->isVisible;
}

long SWGetCurrentFrameIndex(SpritePtr srcSpriteP)
{
    SW_ASSERT(srcSpriteP != NULL);
    return srcSpriteP->curFrameIndex;
}

void SWMoveSprite(SpritePtr srcSpriteP, SWMovementType horizLoc, SWMovementType vertLoc)
{
    short left, top;

    SW_ASSERT(srcSpriteP != NULL);
    srcSpriteP->drawData->horizLoc = horizLoc;
    srcSpriteP->drawData->vertLoc = vertLoc;
    left = (short)(int)horizLoc - srcSpriteP->drawData->frameHotSpotH;
    top = (short)(int)vertLoc - srcSpriteP->drawData->frameHotSpotV;
    if (srcSpriteP->destFrameRect.left != left || srcSpriteP->destFrameRect.top != top) {
        srcSpriteP->destFrameRect.bottom = (srcSpriteP->destFrameRect.bottom - srcSpriteP->destFrameRect.top) + top;
        srcSpriteP->destFrameRect.right = (srcSpriteP->destFrameRect.right - srcSpriteP->destFrameRect.left) + left;
        srcSpriteP->destFrameRect.top = top;
        srcSpriteP->destFrameRect.left = left;
        srcSpriteP->needsToBeDrawn = true;
    }
}

void SWOffsetSprite(SpritePtr srcSpriteP, SWMovementType horizOffset, SWMovementType vertOffset)
{
    SW_ASSERT(srcSpriteP != NULL);
    if (horizOffset != 0.0f || vertOffset != 0.0f) {
        srcSpriteP->drawData->horizLoc += horizOffset;
        srcSpriteP->drawData->vertLoc += vertOffset;
        SWMoveSprite(srcSpriteP, srcSpriteP->drawData->horizLoc, srcSpriteP->drawData->vertLoc);
    }
}

void SWOffsetSpriteBasedOnTime(SpritePtr srcSpriteP, SWMovementType horizOffset, SWMovementType vertOffset)
{
    SpriteWorldPtr world = NULL;
    double fps;

    SW_ASSERT(srcSpriteP != NULL);
    if (srcSpriteP->parentSpriteLayerP != NULL)
        world = srcSpriteP->parentSpriteLayerP->parentSpriteWorldP;
    if (world == NULL)
        return;
    if (srcSpriteP->pseudoMoveTimeInterval > 0.0)
        fps = 1000.0 / srcSpriteP->pseudoMoveTimeInterval;
    else
        fps = world->pseudoFPS;
    SWOffsetSprite(srcSpriteP, (float)SWConvertOffsetBasedOnTime(world, horizOffset, fps),
                   (float)SWConvertOffsetBasedOnTime(world, vertOffset, fps));
}

/// Scales a per-frame offset by the time that has passed since the last frame.
double SWConvertOffsetBasedOnTime(SpriteWorldPtr spriteWorldP, double theOffset, double framesPerSecond)
{
    return (theOffset * (double)spriteWorldP->millisecsSinceLastFrame * framesPerSecond) / 1000.0;
}

void SWResetSpriteFrameTimer(SpriteWorldPtr spriteWorldP, SpritePtr srcSpriteP)
{
    srcSpriteP->timeOfLastFrameChange = spriteWorldP->runningTimeCount;
}

SWBoolean SWHasMoveProcFired(SpritePtr srcSpriteP)
{
    SW_ASSERT(srcSpriteP != NULL);
    return srcSpriteP->moveProcHasFired;
}

SWBoolean SWBounceSprite(SpritePtr srcSpriteP)
{
    short offsetH, offsetV;
    DrawDataPtr d;

    SW_ASSERT(srcSpriteP != NULL);
    d = srcSpriteP->drawData;
    if (srcSpriteP->destFrameRect.left < srcSpriteP->moveBoundsRect.left && d->horizMoveDelta < 0.0f) {
        d->horizMoveDelta = -d->horizMoveDelta;
        offsetH = (srcSpriteP->moveBoundsRect.left - srcSpriteP->destFrameRect.left) * 2;
    } else if (srcSpriteP->destFrameRect.right > srcSpriteP->moveBoundsRect.right && d->horizMoveDelta > 0.0f) {
        d->horizMoveDelta = -d->horizMoveDelta;
        offsetH = (srcSpriteP->moveBoundsRect.right - srcSpriteP->destFrameRect.right) * 2;
    } else {
        offsetH = 0;
    }
    if (srcSpriteP->destFrameRect.top < srcSpriteP->moveBoundsRect.top && d->vertMoveDelta < 0.0f) {
        d->vertMoveDelta = -d->vertMoveDelta;
        offsetV = (srcSpriteP->moveBoundsRect.top - srcSpriteP->destFrameRect.top) * 2;
    } else if (srcSpriteP->destFrameRect.bottom > srcSpriteP->moveBoundsRect.bottom && d->vertMoveDelta > 0.0f) {
        d->vertMoveDelta = -d->vertMoveDelta;
        offsetV = (srcSpriteP->moveBoundsRect.bottom - srcSpriteP->destFrameRect.bottom) * 2;
    } else {
        offsetV = 0;
    }
    if (offsetH == 0 && offsetV == 0)
        return false;
    SWOffsetSprite(srcSpriteP, (float)offsetH, (float)offsetV);
    return true;
}

SWBoolean SWWrapSprite(SpritePtr srcSpriteP)
{
    return SWWrapSpriteHoriz(srcSpriteP) || SWWrapSpriteVert(srcSpriteP);
}

SWBoolean SWWrapSpriteHoriz(SpritePtr srcSpriteP)
{
    float offset;
    SWRect *dest, *old, *bounds;

    SW_ASSERT(srcSpriteP != NULL);
    dest = &srcSpriteP->destFrameRect;
    old = &srcSpriteP->oldFrameRect;
    bounds = &srcSpriteP->moveBoundsRect;
    if (bounds->right < old->left && old->left < dest->left)
        offset = (float)((dest->left - dest->right) - (bounds->right - bounds->left));
    else if (old->right < bounds->left && dest->left < old->left)
        offset = (float)((bounds->right - bounds->left) + (dest->right - dest->left));
    else
        offset = 0.0f;
    if (offset == 0.0f)
        return false;
    SWOffsetSprite(srcSpriteP, offset, 0.0f);
    SWCopyDestToOldAndDelta(srcSpriteP);
    return true;
}

SWBoolean SWWrapSpriteVert(SpritePtr srcSpriteP)
{
    float offset;
    SWRect *dest, *old, *bounds;

    SW_ASSERT(srcSpriteP != NULL);
    dest = &srcSpriteP->destFrameRect;
    old = &srcSpriteP->oldFrameRect;
    bounds = &srcSpriteP->moveBoundsRect;
    if (bounds->bottom < old->top && old->top < dest->top)
        offset = (float)((dest->top - dest->bottom) - (bounds->bottom - bounds->top));
    else if (old->bottom < bounds->top && dest->top < old->top)
        offset = (float)((bounds->bottom - bounds->top) + (dest->bottom - dest->top));
    else
        offset = 0.0f;
    if (offset == 0.0f)
        return false;
    SWOffsetSprite(srcSpriteP, 0.0f, offset);
    SWCopyDestToOldAndDelta(srcSpriteP);
    return true;
}

void SWStdSpriteMoveProc(SpritePtr srcSpriteP)
{
    SWOffsetSprite(srcSpriteP, srcSpriteP->drawData->horizMoveDelta, srcSpriteP->drawData->vertMoveDelta);
}

void SWWrapSpriteMoveProc(SpritePtr srcSpriteP)
{
    SWOffsetSprite(srcSpriteP, srcSpriteP->drawData->horizMoveDelta, srcSpriteP->drawData->vertMoveDelta);
    SWWrapSprite(srcSpriteP);
}

void SWWrapSpriteHorizMoveProc(SpritePtr srcSpriteP)
{
    SWOffsetSprite(srcSpriteP, srcSpriteP->drawData->horizMoveDelta, srcSpriteP->drawData->vertMoveDelta);
    SWWrapSpriteHoriz(srcSpriteP);
}

void SWWrapSpriteVertMoveProc(SpritePtr srcSpriteP)
{
    SWOffsetSprite(srcSpriteP, srcSpriteP->drawData->horizMoveDelta, srcSpriteP->drawData->vertMoveDelta);
    SWWrapSpriteVert(srcSpriteP);
}

/// True if the pixel at window position (h, v) of the sprite's current frame is opaque.
SWBoolean SWInPixelMask(SpritePtr srcSpriteP, int h, int v)
{
    FramePtr f;

    SW_ASSERT(srcSpriteP != NULL);
    SW_ASSERT(srcSpriteP->curFrameP != NULL);
    f = srcSpriteP->curFrameP;
    return !BKTransparentPixel(f->frameSurfaceP, h - srcSpriteP->destFrameRect.left + f->frameRect.left,
                               v - srcSpriteP->destFrameRect.top + f->frameRect.top);
}

SWBoolean SWRadiusCollision(SpritePtr srcSpriteP, SpritePtr dstSpriteP)
{
    short srcRadius, dstRadius, dh, dv;

    SW_ASSERT(srcSpriteP != NULL && dstSpriteP != NULL);
    srcRadius = (short)((srcSpriteP->destFrameRect.right - srcSpriteP->destFrameRect.left) / 2);
    dstRadius = (short)((dstSpriteP->destFrameRect.right - dstSpriteP->destFrameRect.left) / 2);
    dh = (srcSpriteP->destFrameRect.left + srcRadius) - (dstSpriteP->destFrameRect.left + dstRadius);
    dv = (srcSpriteP->destFrameRect.top + srcRadius) - (dstSpriteP->destFrameRect.top + dstRadius);
    return dh * dh + dv * dv < (srcRadius + dstRadius) * (srcRadius + dstRadius);
}

SWBoolean SWPixelCollision(SpritePtr srcSpriteP, SpritePtr dstSpriteP)
{
    SW_ASSERT(srcSpriteP != NULL && dstSpriteP != NULL);
    SW_ASSERT(srcSpriteP->curFrameP != NULL && dstSpriteP->curFrameP != NULL);
    return BKCollidePixels(srcSpriteP->curFrameP->frameSurfaceP, (const short *)&srcSpriteP->destFrameRect,
                           dstSpriteP->curFrameP->frameSurfaceP, (const short *)&dstSpriteP->destFrameRect,
                           (const short *)&srcSpriteP->curFrameP->frameRect,
                           (const short *)&dstSpriteP->curFrameP->frameRect,
                           SWGetSpriteWidth(srcSpriteP) / srcSpriteP->unscaledWidth,
                           SWGetSpriteHeight(srcSpriteP) / srcSpriteP->unscaledHeight,
                           SWGetSpriteWidth(dstSpriteP) / dstSpriteP->unscaledWidth,
                           SWGetSpriteHeight(dstSpriteP) / dstSpriteP->unscaledHeight,
                           srcSpriteP->drawData->flippedHorizontal, dstSpriteP->drawData->flippedHorizontal,
                           srcSpriteP->drawData->flippedVertical, dstSpriteP->drawData->flippedVertical) ? true : false;
}

/// Collision rects (destFrameRect shrunk by each frame's collision inset) overlap.
SWBoolean SWIsSpriteInSprite(SpritePtr srcSpriteP, SpritePtr dstSpriteP)
{
    SWRect *a, *b, *ai, *bi;

    SW_ASSERT(srcSpriteP != NULL);
    SW_ASSERT(dstSpriteP != NULL);
    a = &srcSpriteP->destFrameRect;
    b = &dstSpriteP->destFrameRect;
    ai = &srcSpriteP->curFrameP->collisionInset;
    bi = &dstSpriteP->curFrameP->collisionInset;
    return a->top + ai->top < b->bottom - bi->bottom &&
           b->top + bi->top < a->bottom - ai->bottom &&
           a->left + ai->left < b->right - bi->right &&
           b->left + bi->left < a->right - ai->right;
}

SWBoolean SWIsSpriteInRect(SpritePtr srcSpriteP, SWRect *testRect)
{
    SWRect *a, *ai;

    SW_ASSERT(srcSpriteP != NULL);
    a = &srcSpriteP->destFrameRect;
    ai = &srcSpriteP->curFrameP->collisionInset;
    return a->top + ai->top < testRect->bottom && testRect->top < a->bottom - ai->bottom &&
           a->left + ai->left < testRect->right && testRect->left < a->right - ai->right;
}

SWBoolean SWIsSpriteFullyInRect(SpritePtr srcSpriteP, SWRect *testRect)
{
    SWRect *a, *ai;

    SW_ASSERT(srcSpriteP != NULL);
    a = &srcSpriteP->destFrameRect;
    ai = &srcSpriteP->curFrameP->collisionInset;
    if (a->top + ai->top < testRect->top || testRect->bottom < a->bottom - ai->bottom ||
        a->left + ai->left < testRect->left || testRect->right < a->right - ai->right)
        return false;
    return true;
}

SWBoolean SWIsPointInSprite(SpritePtr srcSpriteP, SWPoint testPoint)
{
    SWRect *a, *ai;

    SW_ASSERT(srcSpriteP != NULL);
    a = &srcSpriteP->destFrameRect;
    ai = &srcSpriteP->curFrameP->collisionInset;
    if (testPoint.h < a->left + ai->left || a->right - ai->right <= testPoint.h ||
        testPoint.v < a->top + ai->top || a->bottom - ai->bottom <= testPoint.v)
        return false;
    return true;
}

SWBoolean SWIsPointInSpriteMask(SpritePtr srcSpriteP, int h, int v, SpriteInMaskProcPtr testProc)
{
    SW_ASSERT(srcSpriteP != NULL);
    if (testProc == NULL) {
        SWPoint pt;
        pt.h = (short)h;
        pt.v = (short)v;
        return SWIsPointInSprite(srcSpriteP, pt);
    }
    return testProc(srcSpriteP, h, v);
}
