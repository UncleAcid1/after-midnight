/*
 * SpriteWorld X fork — the SpriteWorld: window, timing, processing and drawing.
 * See SWCommonHeaders.h for license and provenance.
 *
 * Coordinate system: the projection maps (0,0)-(window size) to the viewport, and
 * sprites with autoScaling are scaled from the design resolution to the window, so
 * game code works in design-resolution pixels. The viewport letterboxes the world to
 * keep its aspect ratio unless stretchToFit is set.
 */

#include "SpriteWorld.h"

SpriteWorldPtr      gSWCurrentSpriteWorld = NULL;
AssertFailProcPtr   gSWAssertFailProc = SWAssertFail;
static AssertFailProcPtr gAssertFailCallback = NULL;
static SWError      gSWStickyError = kNoError;
static SWBoolean    gSWInitializedSDL = false;
static SpriteWorldPtr gSWCleanUpSpriteWorldP = NULL;
static CleanUpCallBackPtr gSWCleanUpCallBackP = NULL;

SWPresentHookPtr    gSWPresentHook = NULL;
SWBoolean           gSWForceFrames = false;
SWBoolean           gSWSkipDrawing = false;     ///< run this frame without drawing it (catching up)
SWBoolean           gSWUseGameClock = false;
unsigned long       gSWGameClock = 0;

static void SWDrawWorldContents(SpriteWorldPtr spriteWorldP);

/// Size of the window's drawable area in pixels (differs from points on Retina displays).
static void SWGetWindowDrawableSize(SpriteWorldPtr spriteWorldP, int *w, int *h)
{
    SDL_GL_GetDrawableSize(spriteWorldP->mainWindow, w, h);
    if (*w <= 0 || *h <= 0)
        SDL_GetWindowSize(spriteWorldP->mainWindow, w, h);
}

/// The area the world fits into: its slot of the window (split screen), or all of it.
static void SWGetDrawableSize(SpriteWorldPtr spriteWorldP, int *w, int *h)
{
    if (spriteWorldP->hasSlot) {
        *w = spriteWorldP->slotRect.right - spriteWorldP->slotRect.left;
        *h = spriteWorldP->slotRect.bottom - spriteWorldP->slotRect.top;
        return;
    }
    SWGetWindowDrawableSize(spriteWorldP, w, h);
}

void SWGetWorldDrawableSize(SpriteWorldPtr spriteWorldP, int *w, int *h)
{
    SWGetDrawableSize(spriteWorldP, w, h);
}

void SWGetSlotOrigin(SpriteWorldPtr spriteWorldP, int *x, int *y)
{
    int ww, wh;

    *x = 0;
    *y = 0;
    if (!spriteWorldP->hasSlot)
        return;
    if (spriteWorldP->slotTargetHeight > 0)
        wh = spriteWorldP->slotTargetHeight;
    else
        SWGetWindowDrawableSize(spriteWorldP, &ww, &wh);
    *x = spriteWorldP->slotRect.left;
    *y = wh - spriteWorldP->slotRect.bottom;
}

/// glViewport for the world's whole slot (or window), clipping drawing to it.
static void SWViewportForSlot(SpriteWorldPtr w)
{
    int ww, wh, x, y;

    SWGetDrawableSize(w, &ww, &wh);
    SWGetSlotOrigin(w, &x, &y);
    glViewport(x, y, ww, wh);
}

void SWPresentWindow(SpriteWorldPtr spriteWorldP)
{
    if (gSWPresentHook != NULL)
        gSWPresentHook(spriteWorldP);
    else
        SDL_GL_SwapWindow(spriteWorldP->mainWindow);
}

void SWSetSpriteWorldSlot(SpriteWorldPtr spriteWorldP, const SWRect *slotP)
{
    SW_ASSERT(spriteWorldP != NULL);
    spriteWorldP->hasSlot = slotP != NULL;
    if (slotP != NULL)
        spriteWorldP->slotRect = *slotP;
    SWUpdateWindowSize(spriteWorldP);
}

/// The projection maps (0,0)-(slot or window size) to the viewport.
static void SWApplyProjection(SpriteWorldPtr w)
{
    int ww, wh;

    SWGetDrawableSize(w, &ww, &wh);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, ww, wh, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

SWError SWEnterSpriteWorld(SWBoolean shouldInitSDL)
{
    SWError err = kNoError;

    if (shouldInitSDL) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
            fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
            err = kFailedToInitSDL;
        }
    }
    gSWInitializedSDL = shouldInitSDL;
    SWSetStickyIfError(err);
    return err;
}

void SWExitSpriteWorld(void)
{
    if (gSWInitializedSDL)
        SDL_Quit();
}

/*  Creates the game window and its SpriteWorld. designWidth/Height is the resolution the
    game draws at; windowWidth/Height is the initial window size. */
SWError SWCreateSpriteWorld(SpriteWorldPtr *spriteWorldPP, const char *windowTitle,
                            short designWidth, short designHeight, int windowWidth, int windowHeight,
                            int depth, SWBoolean makeFullScreen, SWBoolean wantOpenGL)
{
    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    SDL_Window *window;
    SWError err;

    *spriteWorldPP = NULL;
    if (makeFullScreen)
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    if (wantOpenGL) {
        gSWUseOpenGL = true;
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        flags |= SDL_WINDOW_OPENGL;
    }
    window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              windowWidth, windowHeight, flags);
    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        return -1;
    }
    err = SWCreateSpriteWorldFromSDLWindow(spriteWorldPP, window, designWidth, designHeight, depth,
                                           wantOpenGL, NULL, NULL, 0);
    SWSetStickyIfError(err);
    return err;
}

/// Sets up the projection for the window and resets GL state the engine relies on.
static void SWSetupGL(SpriteWorldPtr spriteWorldP, int windowWidth, int windowHeight)
{
    SWCalculateViewport(spriteWorldP, windowWidth, windowHeight);
    SWResetGLSetup(spriteWorldP);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(0, windowWidth, windowHeight, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}

SWError SWCreateSpriteWorldFromSDLWindow(SpriteWorldPtr *spriteWorldPP, SDL_Window *window,
                            short designWidth, short designHeight, int depth, SWBoolean useOpenGL,
                            SWRect *worldRectP, FramePtr backFrameP, int maxDepth)
{
    SWRect worldRect, backRect, zeroBasedBackRect;
    FramePtr screenFrameP = NULL, workFrameP = NULL;
    SDL_GLContext context = NULL;
    SWError err = kNoError;
    unsigned char pixelDepth = (unsigned char)depth;

    *spriteWorldPP = NULL;
    if (worldRectP == NULL)
        SW_SET_RECT(worldRect, 0, 0, designWidth, designHeight);
    else
        worldRect = *worldRectP;
    if (maxDepth != 0 && maxDepth < depth)
        pixelDepth = (unsigned char)maxDepth;
    backRect = backFrameP ? backFrameP->frameRect : worldRect;
    SW_SET_RECT(zeroBasedBackRect, 0, 0, backRect.right, backRect.bottom);

    if (useOpenGL) {
        context = SDL_GL_CreateContext(window);
        if (context == NULL) {
            fprintf(stderr, "SDL_GL_CreateContext: %s\n", SDL_GetError());
            return kSDLSetVideoMode;
        }
        SDL_GL_MakeCurrent(window, context);
    } else {
        err = SWCreateBlankFrame(&screenFrameP, backRect.right, backRect.bottom, pixelDepth, false);
        if (err == kNoError)
            err = SWCreateBlankFrame(&workFrameP, backRect.right, backRect.bottom, pixelDepth, false);
    }
    if (err == kNoError)
        err = SWCreateSpriteWorldFromFrames(spriteWorldPP, screenFrameP, workFrameP, &zeroBasedBackRect, &worldRect);
    if (err == kNoError) {
        SpriteWorldPtr world = *spriteWorldPP;
        world->mainWindow = window;
        world->glContext = context;
        world->designWidth = designWidth;
        world->designHeight = designHeight;
        world->pixelDepth = pixelDepth;
        world->tileLayerCallbacks = calloc(kNumTileLayers, sizeof(TileLayerCallBackPtr));
        if (!useOpenGL) {
            world->useOpenGL = false;
        } else {
            int w, h;
            SWGetDrawableSize(world, &w, &h);
            SWSetupGL(world, w, h);
            world->useOpenGL = true;
        }
        printf("OpenGL: %d\n", world->useOpenGL);
    } else {
        if (screenFrameP != NULL) SWDisposeFrame(&screenFrameP);
        if (workFrameP != NULL) SWDisposeFrame(&workFrameP);
    }
    SWSetStickyIfError(err);
    return err;
}

/// A second SpriteWorld drawing into the same window (it does not clear the window).
SWError SWCreateAdditionalSpriteWorld(SpriteWorldPtr *spriteWorldPP, SpriteWorldPtr mainWorldP, SWRect *backRectP)
{
    SWError err;
    int w, h;

    *spriteWorldPP = NULL;
    err = SWCreateSpriteWorldFromFrames(spriteWorldPP, mainWorldP->screenFrameP, mainWorldP->backFrameP,
                                        backRectP, &mainWorldP->worldRect);
    if (err == kNoError) {
        SpriteWorldPtr world = *spriteWorldPP;
        world->mainWindow = mainWorldP->mainWindow;
        world->glContext = mainWorldP->glContext;
        world->designWidth = mainWorldP->designWidth;
        world->designHeight = mainWorldP->designHeight;
        world->designScaleH = mainWorldP->designScaleH;
        world->designScaleV = mainWorldP->designScaleV;
        world->useOpenGL = mainWorldP->useOpenGL;
        world->isAdditionalWorld = true;
        world->tileLayerCallbacks = calloc(kNumTileLayers, sizeof(TileLayerCallBackPtr));
        SWGetDrawableSize(world, &w, &h);
        SWCalculateViewport(world, w, h);
    }
    SWSetStickyIfError(err);
    return err;
}

SWError SWCreateSpriteWorldSharingWindow(SpriteWorldPtr *spriteWorldPP, SpriteWorldPtr mainWorldP)
{
    SWError err;
    int w, h;

    *spriteWorldPP = NULL;
    err = SWCreateSpriteWorldFromFrames(spriteWorldPP, mainWorldP->screenFrameP, mainWorldP->backFrameP,
                                        &mainWorldP->backRect, &mainWorldP->screenRect);
    if (err == kNoError) {
        SpriteWorldPtr world = *spriteWorldPP;
        world->mainWindow = mainWorldP->mainWindow;
        world->glContext = mainWorldP->glContext;
        world->designWidth = mainWorldP->designWidth;
        world->designHeight = mainWorldP->designHeight;
        world->pixelDepth = mainWorldP->pixelDepth;
        world->useOpenGL = mainWorldP->useOpenGL;
        world->autoScaling = mainWorldP->autoScaling;
        world->backgroundColor = mainWorldP->backgroundColor;
        world->fpsTimeInterval = mainWorldP->fpsTimeInterval;
        world->minimumFrameRate = mainWorldP->minimumFrameRate;
        world->pseudoFPS = mainWorldP->pseudoFPS;
        world->tileLayerCallbacks = calloc(kNumTileLayers, sizeof(TileLayerCallBackPtr));
        SWGetDrawableSize(world, &w, &h);
        SWCalculateViewport(world, w, h);
    }
    SWSetStickyIfError(err);
    return err;
}

SWError SWCreateSpriteWorldFromFrames(SpriteWorldPtr *spriteWorldPP, FramePtr screenFrameP,
                            FramePtr backFrameP, SWRect *backRectP, SWRect *worldRectP)
{
    SpriteWorldPtr w;
    SWError err = kNoError;

    w = calloc(1, sizeof(SpriteWorldRec));
    if (w == NULL)
        err = kMemoryAllocationError;
    if (err == kNoError) {
        err = SWCreateSpriteLayer(&w->deadSpriteLayerP);
        if (err != kNoError)
            free(w);
    }
    if (err != kNoError) {
        SWSetStickyIfError(err);
        return err;
    }

    w->headSpriteLayerP = NULL;
    w->tailSpriteLayerP = NULL;
    w->screenFrameP = screenFrameP;
    w->backFrameP = backFrameP;
    w->backgroundTileSpriteP = NULL;
    w->postEraseCallBack = NULL;
    w->postDrawCallBack = NULL;
    w->screenRect = *worldRectP;
    w->backRect = *backRectP;
    w->worldRect = *worldRectP;
    w->workAreaRect = *backRectP;
    w->letterboxH = 0;
    w->letterboxV = 0;
    w->scaledWidth = worldRectP->right - worldRectP->left;
    w->scaledHeight = worldRectP->bottom - worldRectP->top;
    w->designWidth = -1;
    w->designHeight = -1;
    w->designScaleH = 1.0f;
    w->designScaleV = 1.0f;
    w->autoScaling = true;
    w->stretchToFit = false;
    w->spriteOffsetH = 0;
    w->spriteOffsetV = 0;
    w->zoomH = 1.0f;
    w->zoomV = 1.0f;
    w->headUpdateRectP = NULL;
    w->useUpdateRectOptimizations = true;
    SW_SET_RECT(w->scrollRectMoveBounds, 0, 0, 0x7fff, 0x7fff);
    w->tileOffsetH = 0;
    w->tileOffsetV = 0;
    w->horizScrollDelta = 0;
    w->vertScrollDelta = 0;
    w->thereAreNonScrollingLayers = false;
    w->visScrollRect = *worldRectP;
    w->visScrollRect.top = 0;
    w->visScrollRect.left = 0;
    w->visScrollRect.bottom = worldRectP->bottom - worldRectP->top;
    w->visScrollRect.right = worldRectP->right - worldRectP->left;
    w->oldVisScrollRect = w->visScrollRect;
    w->tileLayerArray = NULL;
    w->tilingIsInitialized = false;
    w->tilingIsOn = false;
    w->tileChangeProc = NULL;
    w->stdTileRed = w->stdTileGreen = w->stdTileBlue = w->stdTileAlpha = 0xFFFF;
    w->useStdTileColor = false;
    w->frameHasOccurred = false;
    w->fpsTimeInterval = 0;
    w->runningTimeCount = 0;
    w->pseudoFPS = -1.0;
    w->minimumFrameRate = 1000000;
    w->millisecsSinceLastFrame = 0;
    w->pseudoFrameAccum = 0;
    w->pseudoFrameHasFired = false;
    w->backgroundColor.r = w->backgroundColor.g = w->backgroundColor.b = 0;
    w->backgroundColor.a = 255;
    w->makeTranslucencyAdjustments = false;
    w->doFastMovingSpritesChecks = false;
    w->isAdditionalWorld = false;
    SWClearStickyError();
    *spriteWorldPP = w;
    return kNoError;
}

void SWDisposeSpriteWorld(SpriteWorldPtr *spriteWorldPP)
{
    SpriteWorldPtr w = *spriteWorldPP;
    SpriteLayerPtr layerP;
    SpritePtr spriteP;

    if (w == NULL)
        return;
    SWUnlockSpriteWorld(w);
    while ((layerP = SWGetNextSpriteLayer(w, NULL)) != NULL) {
        while ((spriteP = SWGetNextSprite(layerP, NULL)) != NULL) {
            SWRemoveSprite(spriteP);
            SWDisposeSprite(&spriteP);
        }
        SWRemoveSpriteLayer(w, layerP);
        SWDisposeSpriteLayer(&layerP);
    }
    SWDisposeAllSpritesInLayer(w->deadSpriteLayerP);
    SWDisposeSpriteLayer(&w->deadSpriteLayerP);
    SWDisposeFrame(&w->backFrameP);
    if (!w->isAdditionalWorld) {
        if (w->glContext != NULL)
            SDL_GL_DeleteContext(w->glContext);
        if (w->mainWindow != NULL)
            SDL_DestroyWindow(w->mainWindow);
    }
    free(w->tileLayerCallbacks);
    free(w);
    *spriteWorldPP = NULL;
}

void SWResetGLSetup(SpriteWorldPtr spriteWorldP)
{
    glViewport(spriteWorldP->viewportX, spriteWorldP->viewportY,
               spriteWorldP->viewportWidth, spriteWorldP->viewportHeight);
}

void SWSetSpriteWorldZoom(SpriteWorldPtr spriteWorldP, float zoomH, float zoomV)
{
    spriteWorldP->zoomH = zoomH;
    spriteWorldP->zoomV = zoomV;
}

/*  Works out how the world fits in a window of the given size: the largest area with the
    world's aspect ratio (or the whole window with stretchToFit), centered with bars. */
float gSWPixelAspect = 1.0f;     ///< a screen pixel's width / height (not 1 on a PS3's SD TV modes)

void SWCalculateViewport(SpriteWorldPtr w, int windowWidth, int windowHeight)
{
    float scaleH = 1.0f, scaleV = 1.0f, aspect;
    short scaledW, scaledH, barH, barV;

    if (w->designWidth != -1)
        scaleH = (float)windowWidth / (float)w->designWidth;
    if (w->designHeight != -1)
        scaleV = (float)windowHeight / (float)w->designHeight;
    aspect = (float)(w->screenRect.right - w->screenRect.left) / (float)(w->screenRect.bottom - w->screenRect.top);
    if (aspect < (float)windowWidth * gSWPixelAspect / (float)windowHeight) {
        scaledW = (short)(int)((float)windowHeight * aspect / gSWPixelAspect);
        scaledH = (short)windowHeight;
    } else {
        scaledW = (short)windowWidth;
        scaledH = (short)(int)((float)windowWidth * gSWPixelAspect / aspect);
    }
    if (w->stretchToFit) {
        scaledW = (short)windowWidth;
        scaledH = (short)windowHeight;
    }
    barH = (short)((windowWidth - scaledW) / 2);
    barV = (short)((windowHeight - scaledH) / 2);
    w->scaledWidth = scaledW;
    w->scaledHeight = scaledH;
    w->letterboxH = barH;
    w->letterboxV = barV;
    w->designScaleH = scaleH;
    w->designScaleV = scaleV;
    w->windowScaleH = (float)scaledW / (float)w->designWidth;
    w->windowScaleV = (float)scaledH / (float)w->designHeight;
    if (!w->autoScaling) {
        w->viewportX = 0;
        w->viewportY = 0;
        w->viewportWidth = w->designWidth;
        w->viewportHeight = w->designHeight;
    } else {
        w->viewportX = (short)(int)((float)w->worldRect.left * scaleH + (float)barH);
        w->viewportY = (short)(int)((float)barV - w->windowScaleV * (float)w->worldRect.top);
        w->viewportWidth = scaledW;
        w->viewportHeight = scaledH;
    }
    w->fullViewportX = w->worldRect.left + barH;
    w->fullViewportY = (short)(int)((float)-barV * 0.8f - (float)w->worldRect.top * scaleV);
    w->fullViewportWidth = (short)windowWidth;
    w->fullViewportHeight = (short)windowHeight;
    if (w->hasSlot) {
        int slotX, slotY;
        SWGetSlotOrigin(w, &slotX, &slotY);
        w->viewportX += (short)slotX;
        w->viewportY += (short)slotY;
        w->fullViewportX += (short)slotX;
        w->fullViewportY += (short)slotY;
    }
    w->backgroundTileRect.top = 0;
    w->backgroundTileRect.left = 0;
    w->backgroundTileRect.bottom = (short)(int)((float)w->viewportHeight * scaleV);
    w->backgroundTileRect.right = w->viewportWidth;
}

void SWSetSpriteWorldAutoScaling(SpriteWorldPtr spriteWorldP, SWBoolean autoScale)
{
    spriteWorldP->autoScaling = autoScale;
    SWUpdateWindowSize(spriteWorldP);
}

/// Converts a rect from design coordinates to window pixels.
void SWAutoScaleRect(SpriteWorldPtr w, SWRect *rectP)
{
    float scaleH = w->designScaleH, scaleV = w->designScaleV;

    if (w->designWidth != -1)
        scaleH = (float)w->scaledWidth / (float)w->designWidth;
    if (w->designHeight != -1)
        scaleV = (float)w->scaledHeight / (float)w->designHeight;
    rectP->right = (short)(int)((float)rectP->right * scaleH);
    rectP->left = (short)(int)((float)rectP->left * scaleH);
    rectP->top = (short)(int)((float)rectP->top * scaleV);
    rectP->bottom = (short)(int)((float)rectP->bottom * scaleV);
}

void SWSetDesignResolution(SpriteWorldPtr w, short width, short height)
{
    int ww, wh;

    SW_ASSERT(w != NULL);
    w->designWidth = width;
    w->designHeight = height;
    w->workAreaRect.right = w->workAreaRect.left + width;
    w->workAreaRect.bottom = w->workAreaRect.top + height;
    w->visScrollRect.right = w->visScrollRect.left + width;
    w->visScrollRect.bottom = w->visScrollRect.top + height;
    SWGetDrawableSize(w, &ww, &wh);
    SWSetupGL(w, ww, wh);
}

void SWGetWindowScalingFactor(SpriteWorldPtr spriteWorldP, float *scaleH, float *scaleV)
{
    SW_ASSERT(spriteWorldP != NULL);
    *scaleH = spriteWorldP->windowScaleH;
    *scaleV = spriteWorldP->windowScaleV;
}

void SWChangeWindowSize(SpriteWorldPtr spriteWorldP, short width, short height)
{
    SW_ASSERT(spriteWorldP != NULL);
    SDL_SetWindowSize(spriteWorldP->mainWindow, width, height);
    SWUpdateWindowSize(spriteWorldP);
}

void SWSetSpriteWorldStretchToFit(SpriteWorldPtr spriteWorldP, SWBoolean stretch)
{
    SW_ASSERT(spriteWorldP != NULL);
    spriteWorldP->stretchToFit = stretch;
    SWUpdateWindowSize(spriteWorldP);
}

/// Call after the window was resized or changed to/from full screen.
void SWUpdateWindowSize(SpriteWorldPtr spriteWorldP)
{
    int w, h;

    SWGetDrawableSize(spriteWorldP, &w, &h);
    SWCalculateViewport(spriteWorldP, w, h);
    if (!spriteWorldP->isAdditionalWorld) {
        SWResetGLSetup(spriteWorldP);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, w, h, 0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
    } else {
        SWResetGLSetup(spriteWorldP);
    }
}

/// Uses only part of the window for the world (e.g. leaving room for a status bar).
SWError SWChangeWorldRect(SpriteWorldPtr w, SWRect *worldRectP, SWBoolean changeBackFrame)
{
    short deltaW, deltaH;

    SW_ASSERT(w != NULL && worldRectP != NULL);
    deltaW = (w->worldRect.right - w->worldRect.left) - (worldRectP->right - worldRectP->left);
    deltaH = (w->worldRect.bottom - w->worldRect.top) - (worldRectP->bottom - worldRectP->top);
    w->worldRect = *worldRectP;
    SWResizeVisScrollRect(w, worldRectP->right - worldRectP->left, worldRectP->bottom - worldRectP->top);
    if (changeBackFrame) {
        if (w->backFrameP != NULL) {
            w->backFrameP->frameRect.bottom -= deltaH;
            w->backFrameP->frameRect.right -= deltaW;
        }
        w->workAreaRect.bottom -= deltaH;
        w->workAreaRect.right -= deltaW;
    }
    if (!w->isAdditionalWorld) {
        int ww, wh;
        SWGetDrawableSize(w, &ww, &wh);
        SWSetupGL(w, ww, wh);
        SWCalculateViewport(w, ww, wh);
    }
    return kNoError;
}

void SWRestoreWorldRect(SpriteWorldPtr w)
{
    int ww, wh;
    SWRect screenRect = w->screenRect;

    SW_ASSERT(w != NULL);
    w->worldRect = screenRect;
    w->workAreaRect = w->backRect;
    if (w->backFrameP != NULL)
        w->backFrameP->frameRect = w->backRect;
    SWResizeVisScrollRect(w, screenRect.right - screenRect.left, screenRect.bottom - screenRect.top);
    SWGetDrawableSize(w, &ww, &wh);
    SWSetupGL(w, ww, wh);
}

/// Clears the world's slot (or the whole window) to the background color.
static void SWClearSlot(SpriteWorldPtr w)
{
    int ww, wh, x, y;

    SWGetDrawableSize(w, &ww, &wh);
    SWGetSlotOrigin(w, &x, &y);
    glViewport(x, y, ww, wh);
    if (w->hasSlot) {
        glScissor(x, y, ww, wh);
        glEnable(GL_SCISSOR_TEST);
    }
    glClearColor(w->backgroundColor.r / 255.0f, w->backgroundColor.g / 255.0f, w->backgroundColor.b / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if (w->hasSlot)
        glDisable(GL_SCISSOR_TEST);
    SWResetGLSetup(w);
}

/// Clears the world's slot (or the whole window) to the background color.
void SWEraseBackground(SpriteWorldPtr w)
{
    int ww, wh;

    (void)ww;
    (void)wh;
    SWClearSlot(w);
}

void SWAddSpriteLayer(SpriteWorldPtr spriteWorldP, SpriteLayerPtr newSpriteLayerP)
{
    SpriteLayerPtr tailP;

    SW_ASSERT(spriteWorldP != NULL);
    tailP = spriteWorldP->tailSpriteLayerP;
    newSpriteLayerP->parentSpriteWorldP = spriteWorldP;
    if (tailP == NULL)
        spriteWorldP->headSpriteLayerP = newSpriteLayerP;
    else
        tailP->nextSpriteLayerP = newSpriteLayerP;
    newSpriteLayerP->prevSpriteLayerP = tailP;
    newSpriteLayerP->nextSpriteLayerP = NULL;
    spriteWorldP->tailSpriteLayerP = newSpriteLayerP;
}

void SWRemoveSpriteLayer(SpriteWorldPtr spriteWorldP, SpriteLayerPtr oldSpriteLayerP)
{
    SW_ASSERT(spriteWorldP != NULL);
    oldSpriteLayerP->parentSpriteWorldP = NULL;
    if (oldSpriteLayerP->nextSpriteLayerP == NULL)
        spriteWorldP->tailSpriteLayerP = oldSpriteLayerP->prevSpriteLayerP;
    else
        oldSpriteLayerP->nextSpriteLayerP->prevSpriteLayerP = oldSpriteLayerP->prevSpriteLayerP;
    if (oldSpriteLayerP->prevSpriteLayerP == NULL)
        spriteWorldP->headSpriteLayerP = oldSpriteLayerP->nextSpriteLayerP;
    else
        oldSpriteLayerP->prevSpriteLayerP->nextSpriteLayerP = oldSpriteLayerP->nextSpriteLayerP;
}

void SWSwapSpriteLayer(SpriteWorldPtr spriteWorldP, SpriteLayerPtr src, SpriteLayerPtr dst)
{
    SpriteLayerPtr swap;

    SW_ASSERT(spriteWorldP != NULL);
    SW_ASSERT(src != NULL && dst != NULL);
    if (src == dst)
        return;
    if (src->nextSpriteLayerP == dst) {
        if (src->prevSpriteLayerP != NULL) src->prevSpriteLayerP->nextSpriteLayerP = dst;
        if (dst->nextSpriteLayerP != NULL) dst->nextSpriteLayerP->prevSpriteLayerP = src;
        dst->prevSpriteLayerP = src->prevSpriteLayerP;
        src->nextSpriteLayerP = dst->nextSpriteLayerP;
        dst->nextSpriteLayerP = src;
        src->prevSpriteLayerP = dst;
    } else if (dst->nextSpriteLayerP == src) {
        if (dst->prevSpriteLayerP != NULL) dst->prevSpriteLayerP->nextSpriteLayerP = src;
        if (src->nextSpriteLayerP != NULL) src->nextSpriteLayerP->prevSpriteLayerP = dst;
        src->prevSpriteLayerP = dst->prevSpriteLayerP;
        dst->nextSpriteLayerP = src->nextSpriteLayerP;
        src->nextSpriteLayerP = dst;
        dst->prevSpriteLayerP = src;
    } else {
        if (src->prevSpriteLayerP != NULL) src->prevSpriteLayerP->nextSpriteLayerP = dst;
        if (dst->prevSpriteLayerP != NULL) dst->prevSpriteLayerP->nextSpriteLayerP = src;
        if (src->nextSpriteLayerP != NULL) src->nextSpriteLayerP->prevSpriteLayerP = dst;
        if (dst->nextSpriteLayerP != NULL) dst->nextSpriteLayerP->prevSpriteLayerP = src;
        swap = src->nextSpriteLayerP;
        src->nextSpriteLayerP = dst->nextSpriteLayerP;
        dst->nextSpriteLayerP = swap;
        swap = src->prevSpriteLayerP;
        src->prevSpriteLayerP = dst->prevSpriteLayerP;
        dst->prevSpriteLayerP = swap;
    }
    if (src->nextSpriteLayerP == NULL)
        spriteWorldP->tailSpriteLayerP = src;
    else if (src->prevSpriteLayerP == NULL)
        spriteWorldP->headSpriteLayerP = src;
    if (dst->nextSpriteLayerP == NULL)
        spriteWorldP->tailSpriteLayerP = dst;
    else if (dst->prevSpriteLayerP == NULL)
        spriteWorldP->headSpriteLayerP = dst;
}

SpriteLayerPtr SWGetNextSpriteLayer(SpriteWorldPtr spriteWorldP, SpriteLayerPtr curSpriteLayerP)
{
    SW_ASSERT(spriteWorldP != NULL);
    return curSpriteLayerP ? curSpriteLayerP->nextSpriteLayerP : spriteWorldP->headSpriteLayerP;
}

void SWLockSpriteWorld(SpriteWorldPtr spriteWorldP)
{
    SW_ASSERT(spriteWorldP != NULL);
}

void SWUnlockSpriteWorld(SpriteWorldPtr spriteWorldP)
{
    SW_ASSERT(spriteWorldP != NULL);
}

void SWSetScrollingWorldMoveBounds(SpriteWorldPtr spriteWorldP, SWRect *scrollRectMoveBounds)
{
    spriteWorldP->scrollRectMoveBounds = *scrollRectMoveBounds;
    SWMoveVisScrollRect(spriteWorldP, spriteWorldP->visScrollRect.left, spriteWorldP->visScrollRect.top);
}

void SWResizeVisScrollRect(SpriteWorldPtr w, short horizSize, short vertSize)
{
    short shift;

    if (w->scrollRectMoveBounds.bottom < w->visScrollRect.top + vertSize) {
        w->visScrollRect.bottom = w->scrollRectMoveBounds.bottom;
        w->visScrollRect.top = w->visScrollRect.bottom - vertSize;
        shift = w->scrollRectMoveBounds.top - w->visScrollRect.top;
        if (shift > 0) {
            w->visScrollRect.top += shift;
            w->visScrollRect.bottom += shift;
        }
    } else {
        w->visScrollRect.bottom = w->visScrollRect.top + vertSize;
    }
    if (w->scrollRectMoveBounds.right < w->visScrollRect.left + horizSize) {
        w->visScrollRect.right = w->scrollRectMoveBounds.right;
        w->visScrollRect.left = w->visScrollRect.right - horizSize;
        shift = w->scrollRectMoveBounds.left - w->visScrollRect.left;
        if (shift > 0) {
            w->visScrollRect.left += shift;
            w->visScrollRect.right += shift;
        }
    } else {
        w->visScrollRect.right = w->visScrollRect.left + horizSize;
    }
    w->oldVisScrollRect = w->visScrollRect;
}

void SWMoveVisScrollRect(SpriteWorldPtr w, short horizPos, short vertPos)
{
    short height = w->visScrollRect.bottom - w->visScrollRect.top;
    short width = w->visScrollRect.right - w->visScrollRect.left;

    if (vertPos < w->scrollRectMoveBounds.top) {
        w->visScrollRect.top = w->scrollRectMoveBounds.top;
        w->visScrollRect.bottom = w->visScrollRect.top + height;
    } else if (w->scrollRectMoveBounds.bottom < vertPos + height) {
        w->visScrollRect.bottom = w->scrollRectMoveBounds.bottom;
        w->visScrollRect.top = w->visScrollRect.bottom - height;
    } else {
        w->visScrollRect.top = vertPos;
        w->visScrollRect.bottom = vertPos + height;
    }
    if (horizPos < w->scrollRectMoveBounds.left) {
        w->visScrollRect.left = w->scrollRectMoveBounds.left;
        w->visScrollRect.right = w->visScrollRect.left + width;
    } else if (w->scrollRectMoveBounds.right < horizPos + width) {
        w->visScrollRect.right = w->scrollRectMoveBounds.right;
        w->visScrollRect.left = w->visScrollRect.right - width;
    } else {
        w->visScrollRect.left = horizPos;
        w->visScrollRect.right = horizPos + width;
    }
}

void SWSetPostEraseCallBack(SpriteWorldPtr spriteWorldP, CallBackPtr callBack)
{
    spriteWorldP->postEraseCallBack = callBack;
}

void SWSetPostDrawCallBack(SpriteWorldPtr spriteWorldP, CallBackPtr callBack)
{
    spriteWorldP->postDrawCallBack = callBack;
}

void SWSetSpriteWorldMaxFPS(SpriteWorldPtr spriteWorldP, short framesPerSec)
{
    SW_ASSERT(spriteWorldP != NULL);
    if (framesPerSec < 1 || framesPerSec > 1000)
        spriteWorldP->fpsTimeInterval = 0;
    else
        spriteWorldP->fpsTimeInterval = (short)(1000 / framesPerSec);
}

void SWSetSpriteWorldPseudoFPS(SpriteWorldPtr spriteWorldP, short framesPerSec)
{
    SW_ASSERT(spriteWorldP != NULL);
    spriteWorldP->pseudoFPS = (double)framesPerSec;
}

void SWSetMinimumFrameRate(SpriteWorldPtr spriteWorldP, long minimumFrameRate)
{
    if (minimumFrameRate < 1)
        spriteWorldP->minimumFrameRate = 0xFFFFFFFFUL;
    else
        spriteWorldP->minimumFrameRate = (unsigned long)(1000000 / minimumFrameRate);
}

void SWSetBackgroundColor(SpriteWorldPtr spriteWorldP, SDL_Color color)
{
    SW_ASSERT(spriteWorldP != NULL);
    spriteWorldP->backgroundColor = color;
}

/// Draws every visible sprite and tile layer without advancing animation.
void SWUpdateSpriteWorld(SpriteWorldPtr spriteWorldP, int updateWindow)
{
    SW_ASSERT(spriteWorldP != NULL);
    gSWCurrentSpriteWorld = spriteWorldP;
    BK_CHECK_GL();
    SWDrawWorldContents(spriteWorldP);
    if (updateWindow && spriteWorldP->useOpenGL)
        SWPresentWindow(spriteWorldP);
    SWResetMovementTimer(spriteWorldP);
    gSWCurrentSpriteWorld = NULL;
}

/// Starts a frame of animation once enough time has passed; updates the frame timing.
static SWBoolean SWStartFrame(SpriteWorldPtr w)
{
    double pseudoInterval;
    unsigned long now = SWGetMilliseconds(w);       // (frames start by the real clock either way)

    w->runningTimeCount = gSWUseGameClock ? gSWGameClock : now;
    if (!gSWForceFrames && now - w->timeOfLastFrame < (unsigned long)(long)w->fpsTimeInterval) {
        w->frameHasOccurred = false;
        return false;
    }
    w->millisecsSinceLastFrame = now - w->timeOfLastFrame;
    if (w->millisecsSinceLastFrame > w->minimumFrameRate)
        w->millisecsSinceLastFrame = w->minimumFrameRate;
    w->frameHasOccurred = true;
    w->timeOfLastFrame = now;
    w->pseudoFrameAccum += (double)w->millisecsSinceLastFrame;
    pseudoInterval = (w->pseudoFPS >= 0.0) ? w->pseudoFPS : (double)w->fpsTimeInterval;
    if (pseudoInterval <= w->pseudoFrameAccum) {
        w->pseudoFrameHasFired = true;
        w->pseudoFrameAccum -= pseudoInterval;
    } else {
        w->pseudoFrameHasFired = false;
    }
    if (w->deadSpriteLayerP->headSpriteP != NULL)
        SWFindSpritesToBeRemoved(w);
    if (w->tileChangeProc != NULL)
        w->tileChangeProc(w);
    if (gSWUseGameClock)
        w->runningTimeCount = gSWGameClock;     // (the game's frame, just begun, moved it on)
    w->thereAreNonScrollingLayers = false;
    return true;
}

/// Runs frame changes and move procs for all sprites, if it is time for a new frame.
void SWProcessSpriteWorld(SpriteWorldPtr spriteWorldP)
{
    SpriteLayerPtr layerP;

    SW_ASSERT(spriteWorldP != NULL);
    gSWCurrentSpriteWorld = spriteWorldP;
    if (SWStartFrame(spriteWorldP)) {
        for (layerP = spriteWorldP->headSpriteLayerP; layerP != NULL; layerP = layerP->nextSpriteLayerP) {
            if (!layerP->layerIsNonScrolling)
                SWProcessSpriteLayer(spriteWorldP, layerP);
            else
                spriteWorldP->thereAreNonScrollingLayers = true;
        }
    }
    gSWCurrentSpriteWorld = NULL;
}

void SWProcessSpriteWorldForOneLayer(SpriteWorldPtr spriteWorldP, SpriteLayerPtr spriteLayerP)
{
    SW_ASSERT(spriteWorldP != NULL);
    gSWCurrentSpriteWorld = spriteWorldP;
    if (SWStartFrame(spriteWorldP))
        SWProcessSpriteLayer(spriteWorldP, spriteLayerP);
    gSWCurrentSpriteWorld = NULL;
}

void SWProcessSpriteLayer(SpriteWorldPtr spriteWorldP, SpriteLayerPtr curSpriteLayerP)
{
    SpritePtr spriteP, nextSpriteP;

    if (curSpriteLayerP->isPaused)
        return;
    SWBeginDeferredSpriteDisposal();
    spriteP = curSpriteLayerP->headSpriteP;
    while (spriteP != NULL) {
        nextSpriteP = spriteP->nextSpriteP;
        SWProcessSprite(spriteWorldP, spriteP);
        if (spriteP->mountedSpriteP != NULL) {
            SpritePtr rider = spriteP->mountedSpriteP;
            if (!spriteP->mountUsesGlobalOffset)
                SWMoveSprite(rider, rider->mountOffsetH + spriteP->drawData->horizLoc,
                             spriteP->drawData->vertLoc + rider->mountOffsetV);
            else
                SWMoveSprite(rider, spriteWorldP->globalMountOffsetH + spriteP->drawData->horizLoc,
                             spriteP->drawData->vertLoc + spriteWorldP->globalMountOffsetV);
            SWProcessSprite(spriteWorldP, rider);
        }
        // The next sprite may have been removed or moved to another layer meanwhile.
        if (nextSpriteP != NULL && nextSpriteP->parentSpriteLayerP != curSpriteLayerP)
            nextSpriteP = (spriteP->parentSpriteLayerP == curSpriteLayerP) ? spriteP->nextSpriteP : NULL;
        spriteP = nextSpriteP;
    }
    SWEndDeferredSpriteDisposal();
}

/*  Advances a sprite's frame (possibly several times if frames are due) and calls its
    move proc when its move interval has passed. */
void SWProcessSprite(SpriteWorldPtr w, SpritePtr s)
{
    double moveInterval;
    long frameIndex;

    s->moveTimeAccum += (double)w->millisecsSinceLastFrame;
    if (s->pseudoMoveTimeInterval >= 0.0)
        moveInterval = s->pseudoMoveTimeInterval;
    else if (w->pseudoFPS >= 0.0)
        moveInterval = 1000.0 / w->pseudoFPS;
    else
        moveInterval = (double)w->fpsTimeInterval;
    if (moveInterval <= s->moveTimeAccum) {
        s->moveProcHasFired = true;
        s->moveTimeAccum -= moveInterval;
    } else {
        s->moveProcHasFired = false;
    }

        // Don't try to catch up on frames after a long pause.
    if ((unsigned long)(s->frameTimeInterval * 10) < w->runningTimeCount - s->timeOfLastFrameChange)
        s->timeOfLastFrameChange = w->runningTimeCount;

    while (s->frameTimeInterval >= 0 &&
           w->runningTimeCount - s->timeOfLastFrameChange >= (unsigned long)s->frameTimeInterval) {
        if (s->frameTimeInterval == 0)
            s->timeOfLastFrameChange = w->runningTimeCount;
        else
            s->timeOfLastFrameChange += s->frameTimeInterval;

        frameIndex = s->curFrameIndex + s->frameAdvance;
        if (frameIndex < s->firstFrameIndex) {
            if (s->frameAdvanceMode == 0) {
                frameIndex = s->lastFrameIndex;
            } else {
                s->frameAdvance = -s->frameAdvance;
                frameIndex += s->frameAdvance + s->frameAdvance;
            }
        } else if (frameIndex > s->lastFrameIndex) {
            if (s->frameAdvanceMode == 0) {
                frameIndex = s->firstFrameIndex;
            } else {
                s->frameAdvance = -s->frameAdvance;
                frameIndex += s->frameAdvance + s->frameAdvance;
            }
        }
        s->curFrameIndex = frameIndex;
        if (s->frameChangeProc != NULL)
            s->frameChangeProc(s, s->frameArray[frameIndex], &s->curFrameIndex);
        if (s->curFrameIndex < 0)
            s->curFrameIndex = 0;
        else if (s->curFrameIndex >= s->maxFrames)
            s->curFrameIndex = s->maxFrames - 1;
        if (s->curFrameP != s->frameArray[s->curFrameIndex])
            SWSetCurrentFrameIndex(s, s->curFrameIndex);
        if (s->frameTimeInterval == 0)
            break;
    }

    if (s->mountedSpriteP != NULL && s->mountCopiesFrame && s->mountedSpriteP->curFrameIndex != s->curFrameIndex)
        SWSetCurrentFrameIndex(s->mountedSpriteP, s->curFrameIndex);

    if (s->moveTimeInterval >= 0 &&
        (unsigned long)s->moveTimeInterval <= w->runningTimeCount - s->timeOfLastMove) {
        s->timeOfLastMove = w->runningTimeCount;
        if (s->spriteMoveProc != NULL)
            s->spriteMoveProc(s);
    }
}

void SWProcessNonScrollingLayers(SpriteWorldPtr spriteWorldP)
{
    SpriteLayerPtr layerP;

    if (!spriteWorldP->thereAreNonScrollingLayers)
        return;
    for (layerP = spriteWorldP->headSpriteLayerP; layerP != NULL; layerP = layerP->nextSpriteLayerP) {
        if (layerP->layerIsNonScrolling)
            SWProcessSpriteLayer(spriteWorldP, layerP);
    }
    spriteWorldP->thereAreNonScrollingLayers = false;
}

/// Draws one sprite with its frame draw proc, in the full window when it opts out of
/// auto scaling.
static void SWDrawOneSprite(SpriteWorldPtr w, SpritePtr s)
{
    SWBoolean fullViewport = false;

    if (w->autoScaling && !s->drawData->autoScaling) {
        glViewport(w->fullViewportX, w->fullViewportY, w->fullViewportWidth, w->fullViewportHeight);
        fullViewport = true;
    }
    s->frameDrawProc(s, s->curFrameP, s->drawData->horizLoc, s->drawData->vertLoc);
    if (fullViewport)
        SWResetGLSetup(w);
}

/*  Clears the window and draws all layers: for each sprite layer, the tile layer
    below it, its sprites (with riders and tiles drawn over sprites), then the tile layer
    above it. */
static void SWDrawWorldContents(SpriteWorldPtr w)
{
    SpriteLayerPtr layerP;
    SpritePtr s;
    short nextTileLayer = 0;
    SWBoolean drawn;

    glEnable(GL_TEXTURE_2D);
    SWResetAllChangedTilesMaps(w);
    if (w->useOpenGL) {
        SDL_GL_MakeCurrent(w->mainWindow, w->glContext);
        SWResetGLSetup(w);
        if (!w->isAdditionalWorld) {
            // Clear the whole window (or slot), bars included.
            if (w->hasSlot)
                SWApplyProjection(w);
            SWClearSlot(w);
        }
        if (w->backgroundTileSpriteP != NULL) {
            SpritePtr bg = w->backgroundTileSpriteP;
            gSWCurrentSpriteWorld = w;
            SWStdSpriteDrawProc(bg, bg->curFrameP, bg->drawData->horizLoc, bg->drawData->vertLoc);
        }
    }
    if (w->postEraseCallBack != NULL)
        w->postEraseCallBack(w);
    gSWCurrentSpriteWorld = w;

    for (layerP = w->headSpriteLayerP; layerP != NULL; layerP = layerP->nextSpriteLayerP) {
        if (w->tilingIsOn && layerP->tileLayerBelow != -1 && w->tileLayerArray[layerP->tileLayerBelow] != NULL) {
            drawn = SWDrawTileLayerInRect(w, layerP->tileLayerBelow, &w->visScrollRect, true, false);
            nextTileLayer = layerP->tileLayerBelow + 1;
            if (w->tileLayerCallbacks[layerP->tileLayerBelow] != NULL)
                w->tileLayerCallbacks[layerP->tileLayerBelow](w, layerP->tileLayerBelow, drawn);
        }
        if (layerP->preDrawCallBack != NULL)
            layerP->preDrawCallBack(w, layerP);
        if (layerP->layerIsVisible) {
            for (s = layerP->headSpriteP; s != NULL; s = s->nextSpriteP) {
                if (!s->isVisible)
                    continue;
                SWDrawOneSprite(w, s);
                if (s->mountedSpriteP != NULL && s->mountedSpriteP->isVisible && s->mountIsDrawn) {
                    SpritePtr rider = s->mountedSpriteP;
                    rider->frameDrawProc(rider, rider->curFrameP, rider->drawData->horizLoc, rider->drawData->vertLoc);
                    rider->deltaFrameRect = rider->destFrameRect;
                    rider->oldFrameRect = rider->destFrameRect;
                    rider->needsToBeDrawn = false;
                    rider->needsToBeErased = false;
                }
                if (w->tilingIsOn && nextTileLayer <= w->lastActiveTileLayer) {
                    SWRect r = s->destFrameRect;
                    SW_CLIP_RECT(r, w->visScrollRect);
                    if (s->drawTilesOverSprite) {
                        r.left += s->drawTilesInsetRect.left;
                        r.top += s->drawTilesInsetRect.top;
                        r.right -= s->drawTilesInsetRect.right;
                        r.bottom -= s->drawTilesInsetRect.bottom;
                    }
                    SWDrawTilesAboveSprite(w, &r, nextTileLayer);
                }
                s->deltaFrameRect = s->destFrameRect;
                s->oldFrameRect = s->destFrameRect;
                s->needsToBeDrawn = false;
                s->needsToBeErased = false;
            }
        }
        if (w->tilingIsOn && layerP->tileLayerAbove != -1 && w->tileLayerArray[layerP->tileLayerAbove] != NULL) {
            drawn = SWDrawTileLayerInRect(w, layerP->tileLayerAbove, &w->visScrollRect, true, false);
            nextTileLayer = layerP->tileLayerAbove + 1;
            if (w->tileLayerCallbacks[layerP->tileLayerAbove] != NULL)
                w->tileLayerCallbacks[layerP->tileLayerAbove](w, layerP->tileLayerAbove, drawn);
        }
    }
    if (w->postDrawCallBack != NULL)
        w->postDrawCallBack(w);
    SWPaintRectToSidesOfWorldRect(w);
}

/*  A frame that isn't drawn (gSWSkipDrawing) still leaves the sprites as drawing would:
    each drawn sprite's oldFrameRect, which the game reads (how far a sprite moved, which
    way it left the room), becomes where it is now. Otherwise a computer catching up in
    relay co-op would play on differently from the others. */
static void SWSkipWorldContents(SpriteWorldPtr w)
{
    SpriteLayerPtr layerP;
    SpritePtr s, rider;

    for (layerP = w->headSpriteLayerP; layerP != NULL; layerP = layerP->nextSpriteLayerP) {
        if (!layerP->layerIsVisible)
            continue;
        for (s = layerP->headSpriteP; s != NULL; s = s->nextSpriteP) {
            if (!s->isVisible)
                continue;
            rider = s->mountedSpriteP;
            if (rider != NULL && rider->isVisible && s->mountIsDrawn) {
                rider->deltaFrameRect = rider->destFrameRect;
                rider->oldFrameRect = rider->destFrameRect;
                rider->needsToBeDrawn = false;
                rider->needsToBeErased = false;
            }
            s->deltaFrameRect = s->destFrameRect;
            s->oldFrameRect = s->destFrameRect;
            s->needsToBeDrawn = false;
            s->needsToBeErased = false;
        }
    }
}

/// Draws the frame that SWProcessSpriteWorld produced, if one occurred, and shows it.
void SWAnimateSpriteWorld(SpriteWorldPtr spriteWorldP)
{
    SW_ASSERT(spriteWorldP != NULL);
    gSWCurrentSpriteWorld = spriteWorldP;
    if (spriteWorldP->frameHasOccurred && !gSWSkipDrawing) {
        if (spriteWorldP->deadSpriteLayerP->headSpriteP != NULL)
            SWAddSpriteLayer(spriteWorldP, spriteWorldP->deadSpriteLayerP);
        SWDrawWorldContents(spriteWorldP);
        if (spriteWorldP->useOpenGL)
            SWPresentWindow(spriteWorldP);
        if (spriteWorldP->deadSpriteLayerP->headSpriteP != NULL)
            SWRemoveSpriteLayer(spriteWorldP, spriteWorldP->deadSpriteLayerP);
    } else if (spriteWorldP->frameHasOccurred) {
        if (spriteWorldP->deadSpriteLayerP->headSpriteP != NULL)
            SWAddSpriteLayer(spriteWorldP, spriteWorldP->deadSpriteLayerP);
        SWSkipWorldContents(spriteWorldP);
        if (spriteWorldP->deadSpriteLayerP->headSpriteP != NULL)
            SWRemoveSpriteLayer(spriteWorldP, spriteWorldP->deadSpriteLayerP);
    }
    if (spriteWorldP->frameHasOccurred)
        gSWSkipDrawing = false;         // for one frame only
    gSWCurrentSpriteWorld = NULL;
}

/// Removes (and disposes, if asked) sprites parked in the dead layer.
void SWFindSpritesToBeRemoved(SpriteWorldPtr spriteWorldP)
{
    SpritePtr spriteP = spriteWorldP->deadSpriteLayerP->headSpriteP, nextSpriteP;

    while (spriteP != NULL) {
        nextSpriteP = spriteP->nextSpriteP;
        SW_ASSERT(spriteP->spriteRemoval != kDontRemoveSprite);
        if (!spriteP->needsToBeErased || spriteWorldP->useOpenGL) {
            SWRemoveSprite(spriteP);
            if (spriteP->spriteRemoval == kRemoveAndDisposeSprite)
                SWDisposeSprite(&spriteP);
            else
                spriteP->spriteRemoval = kDontRemoveSprite;
        }
        spriteP = nextSpriteP;
    }
}

SWError SWFlagRectAsChanged(SpriteWorldPtr spriteWorldP, SWRect *theChangedRect)
{
    // Only meaningful for the software renderer, which redraws changed rects.
    SW_UNUSED(spriteWorldP);
    SW_UNUSED(theChangedRect);
    return kNoError;
}

unsigned long SWGetMilliseconds(SpriteWorldPtr spriteWorldP)
{
    SW_UNUSED(spriteWorldP);
    return SDL_GetTicks();
}

void SWResetElapsedMilliseconds(SpriteWorldPtr spriteWorldP)
{
    spriteWorldP->millisecsSinceLastFrame = 0;
}

unsigned long SWGetElapsedMilliseconds(SpriteWorldPtr spriteWorldP)
{
    return spriteWorldP->millisecsSinceLastFrame;
}

void SWResetMovementTimer(SpriteWorldPtr spriteWorldP)
{
    SW_UNUSED(spriteWorldP);
}

SWBoolean SWHasPseudoFrameFired(SpriteWorldPtr spriteWorldP)
{
    SW_ASSERT(spriteWorldP != NULL);
    return spriteWorldP->pseudoFrameHasFired;
}

float SWGetCurrentFPS(SpriteWorldPtr spriteWorldP)
{
    unsigned long elapsed = SWGetMilliseconds(spriteWorldP) - spriteWorldP->timeOfLastFrame;
    return 1000.0f / (float)elapsed;
}

void SWSetCleanUpSpriteWorld(SpriteWorldPtr spriteWorldP)
{
    gSWCleanUpSpriteWorldP = spriteWorldP;
}

void SWSetCleanUpFunction(CleanUpCallBackPtr callBack)
{
    gSWCleanUpCallBackP = callBack;
}

unsigned long SWGetSpriteWorldVersion(void)
{
    return 0x806001;
}

unsigned char SWGetSpriteWorldPixelDepth(SpriteWorldPtr spriteWorldP)
{
    SW_ASSERT(spriteWorldP != NULL);
    return spriteWorldP->pixelDepth;
}

/// Fills the letterbox bars beside or above/below the world with the background color.
void SWPaintRectToSidesOfWorldRect(SpriteWorldPtr w)
{
    int ww, wh;

    if (w->isAdditionalWorld || !w->autoScaling || (w->letterboxH == 0 && w->letterboxV == 0))
        return;
    glDisable(GL_TEXTURE_2D);
    glColor3ub(w->backgroundColor.r, w->backgroundColor.g, w->backgroundColor.b);
    SWGetDrawableSize(w, &ww, &wh);
    SWViewportForSlot(w);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    if (w->letterboxH != 0) {
        short right = w->scaledWidth + w->letterboxH;
        glRecti(0, 0, w->letterboxH, wh);
        glRecti(right, 0, ww, wh);
    } else {
        short bottom = w->scaledHeight + w->letterboxV;
        glRecti(0, 0, ww, w->letterboxV);
        glRecti(0, bottom, ww, wh);
    }
    glPopMatrix();
    SWResetGLSetup(w);
    glEnable(GL_TEXTURE_2D);
}

void SWDrawRectangle(SpriteWorldPtr spriteWorldP, short left, short top, short right, short bottom)
{
    SW_ASSERT(spriteWorldP != NULL);
    glColor3ub(spriteWorldP->backgroundColor.r, spriteWorldP->backgroundColor.g, spriteWorldP->backgroundColor.b);
    glRecti(left, top, right, bottom);
}

void SWClearStickyError(void)
{
    gSWStickyError = kNoError;
}

SWError SWStickyError(void)
{
    return gSWStickyError;
}

void SWSetStickyIfError(SWError errNum)
{
    if (errNum != kNoError)
        gSWStickyError = errNum;
}

void SWSetAssertionFailureCallback(AssertFailProcPtr callBack)
{
    gAssertFailCallback = callBack;
}

void SWAssertFail(const char *filename, const char *functionname, int lineNum)
{
    fprintf(stderr, "\nASSERT FAILED: \n File: %s \nfunction:%s \nline: %d\n", filename, functionname, lineNum);
    if (gAssertFailCallback != NULL)
        gAssertFailCallback(filename, functionname, lineNum);
    exit(1);
}
