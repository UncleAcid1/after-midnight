/*
 * SpriteWorld X fork — sprite creation.
 * See SWCommonHeaders.h for license and provenance.
 */

#include "SpriteWorld.h"

/// Color made transparent by self-masking (white unless the game changes it).
SDL_Color gSWTransparentColor = { 0xFF, 0xFF, 0xFF, 0xFF };

void SWSetTransparentColor(SDL_Color *color)
{
    gSWTransparentColor = *color;
}

/*  Creates a sprite. spriteStorageP may point to a larger zeroed record whose first
    member is a SpriteRec (game sprites extend it this way); pass NULL to allocate. */
SWError SWCreateSprite(SpritePtr *newSpriteP, void *spriteStorageP, long maxFrames)
{
    SpritePtr spriteP;
    SWError err = kNoError;

    *newSpriteP = NULL;
    spriteP = spriteStorageP ? (SpritePtr)spriteStorageP : calloc(1, sizeof(SpriteRec));
    if (spriteP == NULL)
        return kMemoryAllocationError;
    if (maxFrames == 0)
        maxFrames = 1;
    spriteP->frameArray = calloc(maxFrames, sizeof(FramePtr));
    if (spriteP->frameArray == NULL)
        return kMemoryAllocationError;

    spriteP->parentSpriteLayerP = NULL;
    spriteP->nextSpriteP = NULL;
    spriteP->prevSpriteP = NULL;
    spriteP->spriteRecSize = sizeof(SpriteRec);
    spriteP->isVisible = true;
    spriteP->needsToBeDrawn = true;
    spriteP->needsToBeErased = false;
    SW_SET_RECT(spriteP->destFrameRect, 0, 0, 0, 0);
    SW_SET_RECT(spriteP->oldFrameRect, 0, 0, 0, 0);
    SW_SET_RECT(spriteP->deltaFrameRect, 0, 0, 0, 0);
    spriteP->frameDrawProc = SWStdSpriteDrawProc;
    spriteP->drawTilesOverSprite = false;
    spriteP->curFrameP = NULL;
    spriteP->numFrames = 0;
    spriteP->maxFrames = maxFrames;
    spriteP->frameTimeInterval = -1;
    spriteP->timeOfLastFrameChange = 0;
    spriteP->frameAdvance = 1;
    spriteP->curFrameIndex = 0;
    spriteP->firstFrameIndex = 0;
    spriteP->lastFrameIndex = 0;
    spriteP->frameChangeProc = NULL;
    spriteP->moveTimeInterval = 0;
    spriteP->timeOfLastMove = 0;
    spriteP->pseudoMoveTimeInterval = -1.0;
    spriteP->moveTimeAccum = 0.0;
    spriteP->moveProcHasFired = false;
    SW_SET_RECT(spriteP->moveBoundsRect, 0, 0, 0, 0);
    spriteP->spriteMoveProc = SWTempSpriteMoveProc;
    spriteP->realSpriteMoveProc = NULL;
    spriteP->spriteCollideProc = NULL;
    spriteP->sharedSurface = NULL;
    spriteP->spriteRemoval = kDontRemoveSprite;
    spriteP->frameAdvanceMode = 0;
    spriteP->drawData = SWCreateDrawData();
    spriteP->doFastMovingCheck = false;
    spriteP->drawData->parentSpriteP = spriteP;
    spriteP->drawData->rotationOffsetHoriz = 0;
    spriteP->drawData->rotationOffsetVert = 0;
    spriteP->scalingMode = false;
    spriteP->mountedSpriteP = NULL;
    spriteP->mountParentP = NULL;
    spriteP->userData = 0;

    *newSpriteP = spriteP;
    return err;
}

DrawDataPtr SWCreateDrawData(void)
{
    DrawDataPtr d = calloc(1, sizeof(DrawDataRec));

    if (d == NULL) {
        SWSetStickyIfError(kMemoryAllocationError);
        return NULL;
    }
    d->rotation = 0;
    d->scaledWidth = -1.0f;
    d->scaledHeight = -1.0f;
    d->scaledWidthFactor = 1.0f;
    d->scaledHeightFactor = 1.0f;
    d->useClip = false;
    d->temporaryClip = false;
    d->srcBlendMode = GL_SRC_ALPHA;
    d->dstBlendMode = GL_ONE_MINUS_SRC_ALPHA;
    d->useBlendMode = false;
    d->autoScaling = true;
    d->nonScrolling = false;
    d->translucencyLevel = kStdTranslucency;
    d->colorRed = 0xFFFF;
    d->colorGreen = 0xFFFF;
    d->colorBlue = 0xFFFF;
    return d;
}

/// Loads a strip of square-ish frames; the strip runs along the image's longer side.
SWError SWCreateSpriteFromSingleFile(SpritePtr *newSpriteP, void *spriteStorageP, const char *fileName,
                    short frameDimension, short borderSize, SWBoolean selfMask)
{
    SDL_Surface *surface = BKLoadSurface(fileName);
    int frameWidth, frameHeight, borderWidth, borderHeight, numFrames;

    if (surface == NULL)
        return kSDLCreateSurfaceFromFile;
    if (surface->w < surface->h) {
        frameWidth = surface->w;
        frameHeight = frameDimension;
        borderWidth = 0;
        borderHeight = borderSize;
        numFrames = (surface->h + borderSize) / (frameDimension + borderSize);
    } else {
        frameWidth = frameDimension;
        frameHeight = surface->h;
        borderHeight = 0;
        borderWidth = borderSize;
        numFrames = (surface->w + borderSize) / (frameDimension + borderSize);
    }
    return SWCreateSpriteFromSurfaceXY(newSpriteP, spriteStorageP, surface, frameWidth, frameHeight,
                                       borderWidth, borderHeight, false, numFrames, selfMask);
}

SWError SWCreateSpriteFromSingleFileXY(SpritePtr *newSpriteP, void *spriteStorageP, const char *fileName,
                    int frameWidth, int frameHeight, int borderWidth, int borderHeight,
                    SWBoolean hasOutsideBorder, int maxFrames, SWBoolean selfMask)
{
    SDL_Surface *surface = BKLoadSurface(fileName);

    if (surface == NULL)
        return kSDLCreateSurfaceFromFile;
    return SWCreateSpriteFromSurfaceXY(newSpriteP, spriteStorageP, surface, frameWidth, frameHeight,
                                       borderWidth, borderHeight, hasOutsideBorder, maxFrames, selfMask);
}

SWError SWCreateSpriteFromVerticalFile(SpritePtr *newSpriteP, void *spriteStorageP, const char *fileName,
                    int borderHeight, int numFrames, SWBoolean selfMask)
{
    SDL_Surface *surface = BKLoadSurface(fileName);

    if (surface == NULL)
        return kSDLCreateSurfaceFromFile;
    return SWCreateSpriteFromSurfaceXY(newSpriteP, spriteStorageP, surface, surface->w, surface->h / numFrames,
                                       0, borderHeight, false, numFrames, selfMask);
}

/*  Cuts a sprite sheet into frames, left to right then top to bottom. The sprite takes
    ownership of the surface. maxFrames 0 means as many as fit. */
SWError SWCreateSpriteFromSurfaceXY(SpritePtr *newSpriteP, void *spriteStorageP, SDL_Surface *surface,
                    int frameWidth, int frameHeight, int borderWidth, int borderHeight,
                    SWBoolean hasOutsideBorder, int maxFrames, SWBoolean selfMask)
{
    SpritePtr spriteP = NULL;
    FramePtr frameP = NULL;
    SWRect frameRect;
    int startH, startV, i;
    SWError err;

    SW_ASSERT(surface != NULL);
    if (selfMask)
        surface = SWDoSelfMaskingForSurface(surface);
    if (maxFrames == 0)
        maxFrames = (surface->h / frameHeight) * (surface->w / frameWidth);
    if (hasOutsideBorder) {
        startH = borderWidth / 2;
        startV = borderHeight / 2;
    } else {
        startH = 0;
        startV = 0;
    }

    err = SWCreateSprite(newSpriteP, spriteStorageP, maxFrames);
    if (err == kNoError) {
        spriteP = *newSpriteP;
        spriteP->sharedSurface = surface;
        frameRect.left = (short)startH;
        frameRect.top = (short)startV;
        frameRect.right = frameRect.left + (short)frameWidth;
        frameRect.bottom = frameRect.top + (short)frameHeight;

        for (i = 0; i < maxFrames; i++) {
            err = SWCreateFrameFromSurfaceAndRect(&frameP, surface, &frameRect);
            if (err == kNoError) {
                err = SWAddFrame(spriteP, frameP);
                if (i < maxFrames - 1) {
                    frameRect.left += (short)frameWidth + (short)borderWidth;
                    frameRect.right = frameRect.left + (short)frameWidth;
                    if (frameRect.right > surface->w) {
                        frameRect.left = (short)startH;
                        frameRect.right = frameRect.left + (short)frameWidth;
                        frameRect.top += (short)frameHeight + (short)borderHeight;
                        frameRect.bottom = frameRect.top + (short)frameHeight;
                        if (frameRect.bottom > surface->h)
                            err = kOutOfRangeErr;
                    }
                }
            }
            if (err != kNoError) {
                SWDisposeFrame(&frameP);
                SWDisposeSprite(&spriteP);
                break;
            }
        }
    }
    if (err == kNoError) {
        SWSetSpriteFrameRange(spriteP, 0, maxFrames - 1);
        err = SWSetCurrentFrameIndex(spriteP, 0);
        spriteP->unscaledWidth = (float)frameWidth;
        spriteP->unscaledHeight = (float)frameHeight;
    }
    SWSetStickyIfError(err);
    return err;
}

/// Loads frames from numbered files; fileName is a printf pattern such as "walk%d.png".
SWError SWCreateSpriteFromFileSequence(SpritePtr *newSpriteP, void *spriteStorageP, const char *fileName,
                    int start, int numFrames, SWBoolean selfMask)
{
    SpritePtr spriteP = NULL;
    FramePtr frameP;
    char path[1024];
    int i;
    SWError err;
    SW_UNUSED(spriteStorageP);

    err = SWCreateSprite(&spriteP, NULL, numFrames);
    if (err == kNoError) {
        for (i = 0; i < numFrames; i++) {
            snprintf(path, sizeof(path), fileName, start + i);
            err = SWCreateFrameFromFile(&frameP, path, selfMask);
            if (err == kNoError)
                err = SWAddFrame(spriteP, frameP);
            if (err != kNoError) {
                SWDisposeFrame(&frameP);
                SWDisposeSprite(&spriteP);
                break;
            }
        }
    }
    if (err == kNoError) {
        SWSetSpriteFrameRange(spriteP, 0, numFrames - 1);
        err = SWSetCurrentFrameIndex(spriteP, 0);
        *newSpriteP = spriteP;
        spriteP->unscaledWidth = SWGetSpriteWidth(spriteP);
        spriteP->unscaledHeight = SWGetSpriteHeight(spriteP);
    }
    return err;
}

/// Creates a sprite with room for maxFrames and one frame loaded from fileName.
SWError SWCreateSpriteFromFile(SpritePtr *newSpriteP, void *spriteStorageP, const char *fileName,
                    long maxFrames, SWBoolean selfMask)
{
    SpritePtr spriteP = NULL;
    FramePtr frameP = NULL;
    SWError err;

    err = SWCreateSprite(&spriteP, spriteStorageP, maxFrames);
    if (err == kNoError)
        err = SWCreateFrameFromFile(&frameP, fileName, selfMask);
    if (err == kNoError)
        err = SWAddFrame(spriteP, frameP);
    if (err == kNoError)
        err = SWSetCurrentFrameIndex(spriteP, 0);
    if (err == kNoError) {
        spriteP->unscaledWidth = SWGetSpriteWidth(spriteP);
        spriteP->unscaledHeight = SWGetSpriteHeight(spriteP);
        *newSpriteP = spriteP;
    }
    return err;
}

SWError SWUpdateSpriteFromFile(SpritePtr srcSpriteP, const char *fileName, SWBoolean selfMask)
{
    return SWUpdateFrameFromFile(srcSpriteP->curFrameP, fileName, selfMask);
}

/*  Creates a sprite showing a copy of what is currently drawn in areaRectP (design
    coordinates; NULL means the whole work area), placed where the area was. */
SWError SWCreateSpriteFromWorkArea(SpriteWorldPtr spriteWorldP, SpritePtr *newSpriteP, void *spriteStorageP, SWRect *areaRectP)
{
    SpritePtr spriteP = NULL;
    FramePtr frameP = NULL;
    SWRect *area = areaRectP ? areaRectP : &spriteWorldP->workAreaRect;
    SWError err;

    BK_CHECK_GL();
    err = SWCreateSprite(&spriteP, spriteStorageP, 1);
    if (err == kNoError)
        err = SWCreateFrameFromWorkArea(spriteWorldP, &frameP, area);
    if (err == kNoError)
        err = SWAddFrame(spriteP, frameP);
    if (err == kNoError)
        err = SWSetCurrentFrameIndex(spriteP, 0);
    if (err == kNoError) {
        spriteP->unscaledWidth = SWGetSpriteWidth(spriteP);
        spriteP->unscaledHeight = SWGetSpriteHeight(spriteP);
        SWSetSpriteLocation(spriteP, area->left, area->top);
        *newSpriteP = spriteP;
    }
    return err;
}

/*  Makes a copy of a sprite sharing its frames. spriteStorageP/storageSize let the
    caller supply a larger game record; otherwise the source's record size is used and
    the whole record (including game fields) is copied. */
SWError SWCloneSprite(SpritePtr cloneSpriteP, SpritePtr *newSpriteP, void *spriteStorageP, long storageSize)
{
    SpritePtr spriteP;
    SWError err = kNoError;
    long i;

    SW_ASSERT(cloneSpriteP != NULL);
    spriteP = spriteStorageP ? (SpritePtr)spriteStorageP : calloc(1, cloneSpriteP->spriteRecSize);
    if (spriteP == NULL)
        err = kMemoryAllocationError;

    if (err == kNoError) {
        memcpy(spriteP, cloneSpriteP, sizeof(SpriteRec));
        spriteP->spriteRecSize = spriteStorageP ? storageSize : cloneSpriteP->spriteRecSize;
        spriteP->parentSpriteLayerP = NULL;
        spriteP->nextSpriteP = NULL;
        spriteP->prevSpriteP = NULL;
        spriteP->spriteMoveProc = SWTempSpriteMoveProc;
        spriteP->realSpriteMoveProc = cloneSpriteP->realSpriteMoveProc;
        spriteP->drawData = calloc(1, sizeof(DrawDataRec));
        spriteP->unscaledWidth = cloneSpriteP->unscaledWidth;
        spriteP->unscaledHeight = cloneSpriteP->unscaledHeight;
        memcpy(spriteP->drawData, cloneSpriteP->drawData, sizeof(DrawDataRec));
        spriteP->drawData->parentSpriteP = spriteP;
    }
    if (err == kNoError) {
        spriteP->frameArray = calloc(cloneSpriteP->maxFrames, sizeof(FramePtr));
        if (spriteP->frameArray == NULL) {
            err = kMemoryAllocationError;
            if (spriteStorageP == NULL)
                free(spriteP);
        } else {
            for (i = 0; i < cloneSpriteP->maxFrames; i++) {
                spriteP->frameArray[i] = cloneSpriteP->frameArray[i];
                if (spriteP->frameArray[i] != NULL)
                    spriteP->frameArray[i]->useCount++;
            }
        }
    }
    if (err == kNoError)
        *newSpriteP = spriteP;
    SWSetStickyIfError(err);
    return err;
}

/// Turns pixels of the transparent color (white by default) into alpha, if the
/// surface has no alpha channel of its own.
SDL_Surface *SWDoSelfMaskingForSurface(SDL_Surface *surface)
{
    SDL_Surface *masked;
    SDL_Color key;

    if (BKSurfaceHasAlpha(surface))
        return surface;
    masked = BKCreateGLSurface(surface->w, surface->h, 32, SDL_FALSE, SDL_FALSE, SDL_TRUE);
    SDL_BlitSurface(surface, NULL, masked, NULL);
    SDL_FreeSurface(surface);
    key = gSWTransparentColor;
    BKConvertColorKeyToAlphaChannel(masked, &key);
    return masked;
}
