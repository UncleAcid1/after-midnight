/*
 * SpriteWorld X fork — frames (one image of a sprite).
 * See SWCommonHeaders.h for license and provenance.
 */

#include "SpriteWorld.h"

SWBoolean gSWUseOpenGL = false;

SWError SWCreateFrame(FramePtr *newFrameP)
{
    FramePtr frameP = calloc(1, sizeof(FrameRec));

    if (frameP == NULL)
        return kMemoryAllocationError;
    *newFrameP = frameP;
    return kNoError;
}

SWError SWCreateFrameFromSurface(FramePtr *newFrameP, SDL_Surface *theSurface, int isVideoSurface)
{
    FramePtr frameP = NULL;
    SWError err = SWCreateFrame(&frameP);

    if (err == kNoError) {
        frameP->frameSurfaceP = theSurface;
        frameP->isVideoSurface = isVideoSurface;
        err = SWUpdateFrame(frameP);
    }
    if (err == kNoError)
        *newFrameP = frameP;
    else
        free(frameP);
    return err;
}

/// Makes a frame showing one rectangle of a surface shared with other frames.
SWError SWCreateFrameFromSurfaceAndRect(FramePtr *newFrameP, SDL_Surface *surface, SWRect *frameRect)
{
    FramePtr frameP = NULL;
    SWError err;

    SW_ASSERT(surface != NULL);
    SW_ASSERT(frameRect->top >= 0 && frameRect->left >= 0 &&
              frameRect->right > frameRect->left && frameRect->bottom > frameRect->top);
    *newFrameP = NULL;
    err = SWCreateFrame(&frameP);
    if (err == kNoError) {
        frameP->frameSurfaceP = surface;
        frameP->frameRect = *frameRect;
        frameP->sharesSurface = true;
        err = SWUpdateFrame(frameP);
    }
    if (err == kNoError)
        *newFrameP = frameP;
    else
        free(frameP);
    return err;
}

SWError SWCreateBlankFrame(FramePtr *newFrameP, int w, int h, unsigned char depth, SWBoolean createAlphaChannel)
{
    FramePtr frameP = NULL;
    SDL_Surface *surface = NULL;
    SWError err = SWCreateFrame(&frameP);

    if (err == kNoError) {
        surface = BKCreateSurface(w, h, depth, SDL_FALSE, SDL_FALSE, createAlphaChannel);
        if (surface == NULL)
            err = kSDLCreateSurface;
    }
    if (err == kNoError) {
        frameP->originalSurfaceP = surface;
        err = SWUpdateFrame(frameP);
    }
    if (err == kNoError)
        *newFrameP = frameP;
    else {
        if (surface != NULL)
            SDL_FreeSurface(surface);
        free(frameP);
    }
    return err;
}

SWError SWCreateBlankGLFrame(FramePtr *newFrameP, short w, short h, unsigned char depth)
{
    FramePtr frameP = NULL;
    SWError err = SWCreateFrame(&frameP);

    if (err == kNoError) {
        frameP->originalSurfaceP = NULL;
        frameP->glSurfaceP = BKCreateBlankGLSurface(w, h, depth, SDL_FALSE);
        SW_SET_RECT(frameP->frameRect, 0, 0, w, h);
        *newFrameP = frameP;
    }
    return err;
}

SWError SWCreateFrameFromFile(FramePtr *newFrameP, const char *filename, SWBoolean selfMask)
{
    FramePtr frameP = NULL;
    SDL_Surface *surface = NULL;
    SWError err = SWCreateFrame(&frameP);

    if (err == kNoError) {
        surface = BKLoadSurface(filename);
        if (surface == NULL)
            err = kSDLCreateSurfaceFromFile;
    }
    if (err == kNoError && selfMask)
        surface = SWDoSelfMaskingForSurface(surface);
    if (err == kNoError) {
        frameP->originalSurfaceP = surface;
        err = SWUpdateFrame(frameP);
    }
    if (err == kNoError)
        *newFrameP = frameP;
    else {
        if (surface != NULL)
            SDL_FreeSurface(surface);
        free(frameP);
    }
    return err;
}

/// Replaces a frame's image with a new file (keeps the frame itself).
SWError SWUpdateFrameFromFile(FramePtr frameP, const char *filename, SWBoolean selfMask)
{
    SDL_Surface *surface = BKLoadSurface(filename);

    if (surface == NULL)
        return kSDLCreateSurfaceFromFile;
    if (selfMask)
        surface = SWDoSelfMaskingForSurface(surface);
    if (frameP->originalSurfaceP != NULL && !frameP->sharesSurface)
        SDL_FreeSurface(frameP->originalSurfaceP);
    frameP->originalSurfaceP = surface;
    return SWUpdateFrame(frameP);
}

/*  Maps a rectangle in design coordinates to the framebuffer pixels it occupies
    (OpenGL window coordinates, origin at the bottom left). */
static void SWWorkAreaToFramebuffer(SpriteWorldPtr w, const SWRect *r, int *x, int *y, int *width, int *height)
{
    int drawW, drawH;
    float pxH, pxV;

    SWGetWorldDrawableSize(w, &drawW, &drawH);
    pxH = w->designScaleH * (float)w->viewportWidth / (float)drawW;
    pxV = w->designScaleV * (float)w->viewportHeight / (float)drawH;
    *x = w->viewportX + (int)lroundf(r->left * pxH);
    *y = w->viewportY + w->viewportHeight - (int)lroundf(r->bottom * pxV);
    *width = (int)lroundf((r->right - r->left) * pxH);
    *height = (int)lroundf((r->bottom - r->top) * pxV);
}

/*  Copies what is currently drawn in areaRectP (design coordinates) into a new frame of
    the same design size. The original read window pixels directly and drew the result
    unscaled and flipped; here the frame draws in design coordinates like any other. */
SWError SWCreateFrameFromWorkArea(SpriteWorldPtr spriteWorldP, FramePtr *newFrameP, SWRect *areaRectP)
{
    FramePtr frameP = NULL;
    SWError err;
    int x, y, w, h;

    BK_CHECK_GL();
    SDL_GL_MakeCurrent(spriteWorldP->mainWindow, spriteWorldP->glContext);
    SWWorkAreaToFramebuffer(spriteWorldP, areaRectP, &x, &y, &w, &h);
    err = SWCreateFrame(&frameP);
    if (err == kNoError) {
        frameP->glSurfaceP = BKCreateFramebufferGLSurface(w, h, areaRectP->right - areaRectP->left,
                                                          areaRectP->bottom - areaRectP->top);
        SW_SET_RECT(frameP->frameRect, 0, 0, areaRectP->right - areaRectP->left, areaRectP->bottom - areaRectP->top);
        BKCopyFramebufferToGLSurface(frameP->glSurfaceP, x, y, w, h);
        *newFrameP = frameP;
    }
    return err;
}

void SWUpdateFrameFromWorkArea(SpriteWorldPtr spriteWorldP, FramePtr frameP, SWRect *areaRectP)
{
    int x, y, w, h;

    SWWorkAreaToFramebuffer(spriteWorldP, areaRectP, &x, &y, &w, &h);
    BKCopyFramebufferToGLSurface(frameP->glSurfaceP, x, y, w, h);
}

/// Disposes a frame once nothing else uses it. Returns true if it was freed.
int SWDisposeFrame(FramePtr *oldFramePP)
{
    FramePtr frameP = *oldFramePP;
    int freed = false;

    if (frameP == NULL)
        return false;
    if (frameP->useCount < 2) {
        freed = true;
        if (!frameP->isVideoSurface) {
            if (frameP->frameSurfaceP != NULL) {
                if (!frameP->sharesSurface)
                    SDL_FreeSurface(frameP->frameSurfaceP);
                frameP->frameSurfaceP = NULL;
            }
            if (frameP->originalSurfaceP != NULL) {
                if (!frameP->sharesSurface)
                    SDL_FreeSurface(frameP->originalSurfaceP);
                frameP->originalSurfaceP = NULL;
            }
        }
        if (frameP->glSurfaceP != NULL) {
            BKFreeGLSurface(frameP->glSurfaceP);
            frameP->glSurfaceP = NULL;
        }
        free(frameP);
        *oldFramePP = NULL;
    } else {
        frameP->useCount--;
    }
    return freed;
}

void SWLockFrame(FramePtr srcFrameP)
{
    SW_ASSERT(srcFrameP != NULL);
    srcFrameP->isFrameLocked = true;
}

void SWUnlockFrame(FramePtr srcFrameP)
{
    SW_ASSERT(srcFrameP != NULL);
    srcFrameP->isFrameLocked = false;
}

/*  Converts originalSurfaceP to the drawing format (32-bit, with alpha if the original
    had any) and creates the frame's texture from frameRect (or the whole surface). */
SWError SWUpdateFrame(FramePtr srcFrameP)
{
    SWError err = kNoError;
    SDL_Surface *converted = NULL;

    SW_ASSERT(srcFrameP != NULL);
    if (srcFrameP->originalSurfaceP != NULL) {
        SDL_Surface *orig = srcFrameP->originalSurfaceP;
        converted = SDL_ConvertSurfaceFormat(orig,
                        BKSurfaceHasAlpha(orig) ? SDL_PIXELFORMAT_ARGB8888 : SDL_PIXELFORMAT_XRGB8888, 0);
        if (converted == NULL)
            err = kSDLSurfaceConversion;
        if (err == kNoError) {
            if (srcFrameP->frameSurfaceP != NULL)
                SDL_FreeSurface(srcFrameP->frameSurfaceP);
            srcFrameP->frameSurfaceP = converted;
                // the converted copy is all that's used from now on: the original goes
                // (a frame sharing a sprite sheet has none of its own)
            if (!srcFrameP->sharesSurface) {
                SDL_FreeSurface(orig);
                srcFrameP->originalSurfaceP = NULL;
            }
        }
    }

    if (gSWUseOpenGL && err == kNoError && !srcFrameP->isVideoSurface) {
        SDL_Rect rect;
        if (srcFrameP->glSurfaceP != NULL)
            BKFreeGLSurface(srcFrameP->glSurfaceP);
        rect.x = srcFrameP->frameRect.left;
        rect.y = srcFrameP->frameRect.top;
        rect.w = srcFrameP->frameRect.right - srcFrameP->frameRect.left;
        rect.h = srcFrameP->frameRect.bottom - srcFrameP->frameRect.top;
        srcFrameP->glSurfaceP = BKCreateNewGLSurface(srcFrameP->frameSurfaceP,
                                                     srcFrameP->sharesSurface ? &rect : NULL);
        if (srcFrameP->glSurfaceP == NULL) {
            fprintf(stderr, "BKCreateNewGLSurface: %s\n", SDL_GetError());
            err = kSDLSurfaceConversion;
        }
    }

    if (err == kNoError) {
        if (!srcFrameP->sharesSurface)
            SW_SET_RECT(srcFrameP->frameRect, 0, 0, srcFrameP->frameSurfaceP->w, srcFrameP->frameSurfaceP->h);
        srcFrameP->maskType = BKSurfaceHasAlpha(srcFrameP->frameSurfaceP) ? kAlphaChannelMask : kNoMask;
    } else if (converted != NULL) {
        SDL_FreeSurface(converted);
    }
    return err;
}

void SWFreeGLTextureForFrame(FramePtr srcFrameP)
{
    if (srcFrameP->glSurfaceP != NULL) {
        BKFreeGLSurface(srcFrameP->glSurfaceP);
        srcFrameP->glSurfaceP = NULL;
    }
}

void SWSetFrameHotSpot(FramePtr srcFrameP, DrawDataPtr parentSpriteDrawDataP, short hotSpotH, short hotSpotV)
{
    SW_ASSERT(srcFrameP != NULL);
    srcFrameP->hotSpotH = hotSpotH;
    srcFrameP->hotSpotV = hotSpotV;
    if (parentSpriteDrawDataP != NULL) {
        parentSpriteDrawDataP->frameHotSpotH = hotSpotH;
        parentSpriteDrawDataP->frameHotSpotV = hotSpotV;
    }
}

void SWSetFrameCollisionInset(FramePtr srcFrameP, short leftInset, short topInset, short rightInset, short bottomInset)
{
    SW_ASSERT(srcFrameP != NULL);
    srcFrameP->usesCollisionInset = (leftInset || topInset || rightInset || bottomInset);
    srcFrameP->collisionInset.top = topInset;
    srcFrameP->collisionInset.left = leftInset;
    srcFrameP->collisionInset.bottom = bottomInset;
    srcFrameP->collisionInset.right = rightInset;
}
