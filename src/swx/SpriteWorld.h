/*
 * SpriteWorld X fork (Midnight Mansion HD) — main header.
 * See SWCommonHeaders.h for license and provenance.
 *
 * Structures keep the field order of the fork. They are not binary compatible with it
 * (pointer sizes differ on 64-bit), which nothing depends on.
 */
#ifndef __SPRITEWORLD__
#define __SPRITEWORLD__

#include "SWCommonHeaders.h"
#include "BlitKernel.h"

#ifdef __cplusplus
extern "C" {
#endif

#define kNumTileLayers      10
#define kStdTranslucency    0xFFFF

typedef void (*FrameProcPtr)(SpritePtr srcSpriteP, FramePtr curFrameP, long *frameIndex);
typedef void (*DrawProcPtr)(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc);
typedef void (*MoveProcPtr)(SpritePtr srcSpriteP);
typedef void (*CollideProcPtr)(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect);
typedef void (*CallBackPtr)(SpriteWorldPtr spriteWorldP);
typedef void (*TileChangeProcPtr)(SpriteWorldPtr spriteWorldP);
typedef void (*TileLayerCallBackPtr)(SpriteWorldPtr spriteWorldP, short tileLayer, SWBoolean layerWasDrawn);
typedef short (*SpriteCompareProcPtr)(SpritePtr a, SpritePtr b);
typedef SWBoolean (*SpriteInMaskProcPtr)(SpritePtr srcSpriteP, int h, int v);
typedef SWBoolean (*SpriteCollisionProcPtr)(SpritePtr a, SpritePtr b);
typedef void (*CleanUpCallBackPtr)(void);

typedef enum { kNoLoopingMode = 0, kPatrollingMode = 1 } AdvanceType;   // frameAdvanceMode (0 loops)
typedef enum { kDontRemoveSprite = 0, kRemoveSprite = 1, kRemoveAndDisposeSprite = 2 } RemovalType;
typedef enum { kNoMask = 0, kAlphaChannelMask = 1 } MaskType;

/// Tile search directions for SWCheckSpriteWithTiles.
typedef enum {
    kSW_CheckAll = 0,
    kSW_CheckTop = 1,
    kSW_CheckRight = 2,
    kSW_CheckBottom = 3,
    kSW_CheckLeft = 4
} SWTileSearchType;

/// A sprite field that holds either a number or a pointer.
typedef union SWSpriteMisc {
    long        n;
    void       *ptr;
    SpritePtr   sprite;
} SWSpriteMisc;

struct FrameRec {
    SDL_Surface *frameSurfaceP;     ///< surface the frame draws from (may be shared)
    SDL_Surface *originalSurfaceP;  ///< surface before conversion
    int          isVideoSurface;
    int          sharesSurface;     ///< frameRect selects a part of a shared surface
    SWBoolean    isFrameLocked;
    int          maskType;
    SWRect       frameRect;
    int          hotSpotH;
    int          hotSpotV;
    SWBoolean    usesCollisionInset;
    SWRect       collisionInset;
    unsigned short useCount;
    BKGLSurface *glSurfaceP;
    long         userData;
};

struct SpriteRec {
    SpriteLayerPtr  parentSpriteLayerP;
    SpritePtr       nextSpriteP;
    SpritePtr       prevSpriteP;
    SpritePtr       nextActiveSpriteP;
    SpritePtr       nextIdleSpriteP;
    long            spriteRecSize;      ///< size of the record including game extensions

        // mounting: a "rider" sprite that follows this one
    SpritePtr       mountedSpriteP;     ///< sprite riding on this one
    SpritePtr       mountParentP;       ///< sprite this one rides on
    float           mountOffsetH;
    float           mountOffsetV;
    SWBoolean       unknown28;
    SWBoolean       mountCopiesFrame;   ///< rider takes the parent's frame index
    SWBoolean       mountIsDrawn;       ///< draw the rider right after the parent
    SWBoolean       mountUsesGlobalOffset;
    SWBoolean       inheritTranslucency;
    SWBoolean       inheritColor;
    SWBoolean       inheritScale;
    SWBoolean       inheritFlip;
    SWBoolean       inheritRotation;
    SWBoolean       inheritVisible;

        // drawing
    SWBoolean       isVisible;
    SWBoolean       needsToBeDrawn;
    SWBoolean       needsToBeErased;
    SWBoolean       doFastMovingCheck;
    SWRect          destFrameRect;
    SWRect          oldFrameRect;
    SWRect          deltaFrameRect;
    float           unscaledWidth;
    float           unscaledHeight;
    DrawProcPtr     frameDrawProc;
    SWBoolean       destRectIsVisible;
    SWBoolean       oldRectIsVisible;
    SWRect          clippedSourceRect;
    SWRect          destOffscreenRect;
    SWBoolean       drawTilesOverSprite;
    SWRect          drawTilesInsetRect;     ///< stored as left/top/right/bottom insets

        // frames
    FramePtr       *frameArray;
    FramePtr        curFrameP;
    long            numFrames;
    long            maxFrames;
    long            frameTimeInterval;      ///< ms between frame changes; < 0 never changes
    unsigned long   timeOfLastFrameChange;
    long            frameAdvance;
    long            curFrameIndex;
    long            firstFrameIndex;
    long            lastFrameIndex;
    FrameProcPtr    frameChangeProc;

        // movement
    long            moveTimeInterval;
    unsigned long   timeOfLastMove;
    double          pseudoMoveTimeInterval;
    double          moveTimeAccum;
    SWBoolean       moveProcHasFired;
    SWRect          moveBoundsRect;
    MoveProcPtr     spriteMoveProc;
    MoveProcPtr     realSpriteMoveProc;

    CollideProcPtr  spriteCollideProc;
    int             frameAdvanceMode;
    int             spriteRemoval;
    SDL_Surface    *sharedSurface;
    DrawDataPtr     drawData;
    SWBoolean       scalingMode;            ///< keep the sprite centered when scaling
    long            userData;
        // spare fields the game uses in different ways per sprite type
    SWSpriteMisc    misc1;              ///< 0xec
    SWSpriteMisc    misc2;              ///< 0xf0
    float           miscFloat;          ///< 0xf4
};

struct SpriteLayerRec {
    SpriteWorldPtr  parentSpriteWorldP;
    SpriteLayerPtr  nextSpriteLayerP;
    SpriteLayerPtr  prevSpriteLayerP;
    SpritePtr       headSpriteP;
    SpritePtr       tailSpriteP;
    int             isPaused;
    int             layerIsNonScrolling;
    SWBoolean       layerIsVisible;
    short           tileLayerBelow;     ///< tile layer drawn before this layer's sprites, or -1
    short           tileLayerAbove;     ///< tile layer drawn after this layer's sprites, or -1
    short           sanityValue;
    long            userData;
        /// called before the layer's sprites are drawn (added by this version)
    void            (*preDrawCallBack)(SpriteWorldPtr spriteWorldP, SpriteLayerPtr layerP);
};

#define kSpriteLayerSanityValue 0x4ca7

struct TileMapStruct {
    short           numRows;
    short           numCols;
    short         **tileMap;
    float           zoom;
    SWBoolean       isVisible;
    SWBoolean       drawEveryFrame;
    short         **changedTileMap;
    SWBoolean       tilesChanged;
    unsigned char **tileChangedFlags;
};

typedef struct TileColorRec {
    SWBoolean       hasColor;
    unsigned short  red, green, blue, alpha;
} TileColorRec;

typedef struct UpdateRectStruct {
    SWRect updateRect;
    struct UpdateRectStruct *nextRectStructP;
} UpdateRectStruct, *UpdateRectStructPtr;

struct SpriteWorldRec {
    SpriteLayerPtr  headSpriteLayerP;
    SpriteLayerPtr  tailSpriteLayerP;
    SpriteLayerPtr  deadSpriteLayerP;   ///< where SWRemoveSpriteFromAnimation parks sprites
    UpdateRectStructPtr headUpdateRectP;
    SWBoolean       useUpdateRectOptimizations;
    FramePtr        backFrameP;
    FramePtr        screenFrameP;
    SpritePtr       backgroundTileSpriteP;  ///< cached background of static tiles (unused here)

        // window and scaling
    short           viewportX, viewportY, viewportWidth, viewportHeight;
    short           scaledWidth, scaledHeight;  ///< world size in window pixels
    short           letterboxH, letterboxV;     ///< bars at the sides / top and bottom
    short           fullViewportX, fullViewportY, fullViewportWidth, fullViewportHeight;
    short           designWidth, designHeight;  ///< -1 when no design resolution set
    float           designScaleH, designScaleV;
    float           windowScaleH, windowScaleV;
    SWBoolean       autoScaling;
    SWBoolean       stretchToFit;
    CallBackPtr     postEraseCallBack;
    CallBackPtr     postDrawCallBack;
    TileLayerCallBackPtr *tileLayerCallbacks;   ///< kNumTileLayers entries

    SWRect          screenRect;
    SWRect          backRect;
    SWRect          worldRect;          ///< current world rectangle in the window
    SWRect          workAreaRect;
    SWRect          backgroundTileRect;
    SWRect          visScrollRect;
    SWRect          oldVisScrollRect;
    short           tileOffsetH;        ///< tiles are drawn shifted by -tileOffset
    short           tileOffsetV;
    short           horizScrollDelta;
    short           vertScrollDelta;
    SWRect          scrollRectMoveBounds;
    float           spriteOffsetH;      ///< sprites are drawn shifted by -spriteOffset
    float           spriteOffsetV;
    float           zoomH, zoomV;

        // tiling
    TileMapStructPtr *tileLayerArray;   ///< kNumTileLayers entries
    SWBoolean       tilingIsInitialized;
    SWBoolean       tilingIsOn;
    short           lastActiveTileLayer;
    FramePtr       *tileFrameArray;
    short          *curTileImage;
    short          *tileSourceIndex;
    short           maxNumTiles;
    short           tileWidth;
    short           tileHeight;
    short           tileImageWidth;     ///< pixel size of tile images (may differ from grid)
    short           tileImageHeight;
    TileChangeProcPtr tileChangeProc;
    TileColorRec   *tileColorArray;
    unsigned short  stdTileRed, stdTileGreen, stdTileBlue, stdTileAlpha;
    SWBoolean       useStdTileColor;

    unsigned char   pixelDepth;
    short           fpsTimeInterval;    ///< ms per frame (1000/maxFPS), 0 = unlimited
    double          pseudoFPS;
    unsigned long   runningTimeCount;
    unsigned long   timeOfLastFrame;
    unsigned long   minimumFrameRate;   ///< cap on ms counted for one frame
    unsigned long   millisecsSinceLastFrame;
    double          pseudoFrameAccum;
    SWBoolean       pseudoFrameHasFired;
    SWBoolean       frameHasOccurred;
    SWBoolean       thereAreNonScrollingLayers;
    SWBoolean       makeTranslucencyAdjustments;
    SWBoolean       doFastMovingSpritesChecks;
    SWBoolean       isAdditionalWorld;  ///< shares another world's window; doesn't clear it
    SDL_Color       backgroundColor;
    int             useOpenGL;
    float           globalMountOffsetH;
    float           globalMountOffsetV;
    SDL_Window     *mainWindow;
    SDL_GLContext   glContext;
    long            userData;
    SWBoolean       hasSlot;            ///< draws into slotRect of the window only (split screen)
    SWRect          slotRect;           ///< in drawable pixels, origin at the top left
    int             slotTargetHeight;   ///< height of what the slot is in (0: the window)
};

/// Set while every SWProcessSpriteWorld must start a frame (split screen paces frames itself).
extern SWBoolean gSWForceFrames;
/// Set while a game runs in step on several computers (relay co-op): sprites' frame and move
/// timers then run on gSWGameClock (milliseconds of game time, moved on by the game once a
/// game frame) instead of the real clock, so every computer animates every sprite alike.
extern SWBoolean gSWUseGameClock;
extern unsigned long gSWGameClock;
extern SWBoolean gSWSkipDrawing;
extern float gSWPixelAspect;
/// A new main SpriteWorld drawing into mainWorldP's window, with its size and settings.
SWError SWCreateSpriteWorldSharingWindow(SpriteWorldPtr *spriteWorldPP, SpriteWorldPtr mainWorldP);

/// Called instead of swapping the window when set (split screen presents once per frame).
typedef void (*SWPresentHookPtr)(SpriteWorldPtr spriteWorldP);
extern SWPresentHookPtr gSWPresentHook;
/// Shows what was drawn: swaps the window, or calls gSWPresentHook.
void    SWPresentWindow(SpriteWorldPtr spriteWorldP);
/// Confines the world to part of the window (a NULL rect gives it the whole window back).
void    SWSetSpriteWorldSlot(SpriteWorldPtr spriteWorldP, const SWRect *slotP);
/// The drawable size the world fits itself into: its slot, or the whole window.
void    SWGetWorldDrawableSize(SpriteWorldPtr spriteWorldP, int *w, int *h);
/// Where the world's slot starts, in OpenGL window coordinates (origin at the bottom left).
void    SWGetSlotOrigin(SpriteWorldPtr spriteWorldP, int *x, int *y);

// ---- SpriteWorld.c ---------------------------------------------------------

SWError SWEnterSpriteWorld(SWBoolean shouldInitSDL);
void    SWExitSpriteWorld(void);
SWError SWCreateSpriteWorld(SpriteWorldPtr *spriteWorldPP, const char *windowTitle,
                            short designWidth, short designHeight, int windowWidth, int windowHeight,
                            int depth, SWBoolean makeFullScreen, SWBoolean wantOpenGL);
SWError SWCreateSpriteWorldFromSDLWindow(SpriteWorldPtr *spriteWorldPP, SDL_Window *window,
                            short designWidth, short designHeight, int depth, SWBoolean useOpenGL,
                            SWRect *worldRectP, FramePtr backFrameP, int maxDepth);
SWError SWCreateAdditionalSpriteWorld(SpriteWorldPtr *spriteWorldPP, SpriteWorldPtr mainWorldP, SWRect *backRectP);
SWError SWCreateSpriteWorldFromFrames(SpriteWorldPtr *spriteWorldPP, FramePtr screenFrameP,
                            FramePtr backFrameP, SWRect *backRectP, SWRect *worldRectP);
void    SWDisposeSpriteWorld(SpriteWorldPtr *spriteWorldPP);
void    SWResetGLSetup(SpriteWorldPtr spriteWorldP);
void    SWSetSpriteWorldZoom(SpriteWorldPtr spriteWorldP, float zoomH, float zoomV);
void    SWCalculateViewport(SpriteWorldPtr spriteWorldP, int windowWidth, int windowHeight);
void    SWSetSpriteWorldAutoScaling(SpriteWorldPtr spriteWorldP, SWBoolean autoScale);
void    SWAutoScaleRect(SpriteWorldPtr spriteWorldP, SWRect *rectP);
void    SWSetDesignResolution(SpriteWorldPtr spriteWorldP, short width, short height);
void    SWGetWindowScalingFactor(SpriteWorldPtr spriteWorldP, float *scaleH, float *scaleV);
void    SWChangeWindowSize(SpriteWorldPtr spriteWorldP, short width, short height);
void    SWSetSpriteWorldStretchToFit(SpriteWorldPtr spriteWorldP, SWBoolean stretch);
void    SWUpdateWindowSize(SpriteWorldPtr spriteWorldP);
SWError SWChangeWorldRect(SpriteWorldPtr spriteWorldP, SWRect *worldRectP, SWBoolean changeBackFrame);
void    SWRestoreWorldRect(SpriteWorldPtr spriteWorldP);
void    SWEraseBackground(SpriteWorldPtr spriteWorldP);
void    SWAddSpriteLayer(SpriteWorldPtr spriteWorldP, SpriteLayerPtr newSpriteLayerP);
void    SWRemoveSpriteLayer(SpriteWorldPtr spriteWorldP, SpriteLayerPtr oldSpriteLayerP);
void    SWSwapSpriteLayer(SpriteWorldPtr spriteWorldP, SpriteLayerPtr srcSpriteLayerP, SpriteLayerPtr dstSpriteLayerP);
SpriteLayerPtr SWGetNextSpriteLayer(SpriteWorldPtr spriteWorldP, SpriteLayerPtr curSpriteLayerP);
void    SWLockSpriteWorld(SpriteWorldPtr spriteWorldP);
void    SWUnlockSpriteWorld(SpriteWorldPtr spriteWorldP);
void    SWSetScrollingWorldMoveBounds(SpriteWorldPtr spriteWorldP, SWRect *scrollRectMoveBounds);
void    SWResizeVisScrollRect(SpriteWorldPtr spriteWorldP, short horizSize, short vertSize);
void    SWMoveVisScrollRect(SpriteWorldPtr spriteWorldP, short horizPos, short vertPos);
void    SWSetPostEraseCallBack(SpriteWorldPtr spriteWorldP, CallBackPtr callBack);
void    SWSetPostDrawCallBack(SpriteWorldPtr spriteWorldP, CallBackPtr callBack);
void    SWSetSpriteWorldMaxFPS(SpriteWorldPtr spriteWorldP, short framesPerSec);
void    SWSetSpriteWorldPseudoFPS(SpriteWorldPtr spriteWorldP, short framesPerSec);
void    SWSetMinimumFrameRate(SpriteWorldPtr spriteWorldP, long minimumFrameRate);
void    SWSetBackgroundColor(SpriteWorldPtr spriteWorldP, SDL_Color color);
void    SWUpdateSpriteWorld(SpriteWorldPtr spriteWorldP, int updateWindow);
void    SWProcessSpriteWorld(SpriteWorldPtr spriteWorldP);
void    SWProcessSpriteWorldForOneLayer(SpriteWorldPtr spriteWorldP, SpriteLayerPtr spriteLayerP);
void    SWProcessSpriteLayer(SpriteWorldPtr spriteWorldP, SpriteLayerPtr curSpriteLayerP);
void    SWProcessSprite(SpriteWorldPtr spriteWorldP, SpritePtr srcSpriteP);
void    SWProcessNonScrollingLayers(SpriteWorldPtr spriteWorldP);
void    SWAnimateSpriteWorld(SpriteWorldPtr spriteWorldP);
void    SWFindSpritesToBeRemoved(SpriteWorldPtr spriteWorldP);
SWError SWFlagRectAsChanged(SpriteWorldPtr spriteWorldP, SWRect *theChangedRect);
unsigned long SWGetMilliseconds(SpriteWorldPtr spriteWorldP);
void    SWResetElapsedMilliseconds(SpriteWorldPtr spriteWorldP);
unsigned long SWGetElapsedMilliseconds(SpriteWorldPtr spriteWorldP);
void    SWResetMovementTimer(SpriteWorldPtr spriteWorldP);
SWBoolean SWHasPseudoFrameFired(SpriteWorldPtr spriteWorldP);
float   SWGetCurrentFPS(SpriteWorldPtr spriteWorldP);
void    SWSetCleanUpSpriteWorld(SpriteWorldPtr spriteWorldP);
void    SWSetCleanUpFunction(CleanUpCallBackPtr callBack);
unsigned long SWGetSpriteWorldVersion(void);
unsigned char SWGetSpriteWorldPixelDepth(SpriteWorldPtr spriteWorldP);
void    SWPaintRectToSidesOfWorldRect(SpriteWorldPtr spriteWorldP);
void    SWDrawRectangle(SpriteWorldPtr spriteWorldP, short left, short top, short right, short bottom);
void    SWClearStickyError(void);
SWError SWStickyError(void);
void    SWSetStickyIfError(SWError errNum);
void    SWSetAssertionFailureCallback(AssertFailProcPtr callBack);
void    SWAssertFail(const char *filename, const char *functionname, int lineNum);

// ---- SpriteLayer.c ---------------------------------------------------------

SWError SWCreateSpriteLayer(SpriteLayerPtr *spriteLayerPP);
void    SWDisposeSpriteLayer(SpriteLayerPtr *spriteLayerPP);
SWError SWAddSprite(SpriteLayerPtr spriteLayerP, SpritePtr newSpriteP);
SWError SWRemoveSprite(SpritePtr oldSpriteP);
void    SWRemoveAllSpritesFromLayer(SpriteLayerPtr srcSpriteLayerP);
void    SWDisposeAllSpritesInLayer(SpriteLayerPtr spriteLayerP);
int     SWCountNumSpritesInLayer(SpriteLayerPtr spriteLayerP);
SWError SWSwapSprite(SpritePtr srcSpriteP, SpritePtr dstSpriteP);
SWError SWInsertSpriteAfterSprite(SpritePtr newSpriteP, SpritePtr dstSpriteP);
SWError SWInsertSpriteBeforeSprite(SpritePtr newSpriteP, SpritePtr dstSpriteP);
SpritePtr SWGetNextSprite(SpriteLayerPtr spriteLayerP, SpritePtr curSpriteP);
SpritePtr SWGetPreviousSprite(SpriteLayerPtr spriteLayerP, SpritePtr curSpriteP);
void    SWLockSpriteLayer(SpriteLayerPtr spriteLayerP);
void    SWUnlockSpriteLayer(SpriteLayerPtr spriteLayerP);
void    SWCollideSpriteLayer(SpriteWorldPtr spriteWorldP, SpriteLayerPtr srcSpriteLayerP, SpriteLayerPtr dstSpriteLayerP);
SWBoolean SWSpriteCollidesWithLayer(SpritePtr srcSpriteP, SpriteLayerPtr spriteLayerP, SpriteCollisionProcPtr collideFunc);
void    SWSortSpriteLayer(SpriteLayerPtr theLayer);
void    SWCustomSortSpriteLayer(SpriteLayerPtr theLayer, SpriteCompareProcPtr compareFunc);
void    SWPauseSpriteLayer(SpriteLayerPtr spriteLayerP);
void    SWUnpauseSpriteLayer(SpriteLayerPtr spriteLayerP);
void    SWSetSpriteLayerVisible(SpriteLayerPtr spriteLayerP, SWBoolean visible);
SWBoolean SWGetSpriteLayerVisible(SpriteLayerPtr spriteLayerP);
void    SWSetSpriteLayersVisible(SpriteLayerPtr firstLayerP, SpriteLayerPtr lastLayerP, SWBoolean visible);
void    SWSetAllSpriteLayersVisible(SpriteWorldPtr spriteWorldP, SWBoolean visible);
void    SWSetLayerAsNonScrolling(SpriteLayerPtr spriteLayerP, int layerIsNonScrolling);
SpritePtr SWFindSpriteByPoint(SpriteLayerPtr spriteLayerP, SpritePtr startSpriteP, int h, int v);
SpritePtr SWTopSpriteFromPoint(SpriteLayerPtr spriteLayerP, SpritePtr startSpriteP, int h, int v, SpriteInMaskProcPtr testFunc);
void    SWMoveSpriteLayerToTail(SpriteWorldPtr spriteWorldP, SpriteLayerPtr moveSpriteLayerP);

// ---- Sprite.c --------------------------------------------------------------

void    SWRemoveSpriteFromAnimation(SpritePtr spriteP, SWBoolean disposeOfSprite);
void    SWDisposeSprite(SpritePtr *deadSpritePP);
void    SWBeginDeferredSpriteDisposal(void);
void    SWEndDeferredSpriteDisposal(void);
void    SWLockSprite(SpritePtr srcSpriteP);
void    SWUnlockSprite(SpritePtr srcSpriteP);
SWError SWAddFrame(SpritePtr srcSpriteP, FramePtr newFrameP);
SWError SWInsertFrame(SpritePtr srcSpriteP, FramePtr newFrameP, long frameIndex);
void    SWRemoveFrame(SpritePtr srcSpriteP, FramePtr oldFrameP);
FramePtr SWGetNextFrame(SpritePtr srcSpriteP, FramePtr curFrameP);
void    SWSetSpriteDrawProc(SpritePtr srcSpriteP, DrawProcPtr drawProc);
void    SWStdSpriteDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc);
void    SWSolidColorSpriteDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc);
void    SWTempSpriteMoveProc(SpritePtr srcSpriteP);
SWError SWSetCurrentFrame(SpritePtr srcSpriteP, FramePtr curFrameP);
SWError SWSetCurrentFrameIndex(SpritePtr srcSpriteP, long frameIndex);
void    SWSetSpriteCollideProc(SpritePtr srcSpriteP, CollideProcPtr collideProc);
void    SWSetSpriteFrameAdvance(SpritePtr srcSpriteP, long frameAdvance);
void    SWSetSpriteFrameAdvanceMode(SpritePtr srcSpriteP, int advanceMode);
void    SWSetSpriteFrameRange(SpritePtr srcSpriteP, long firstFrameIndex, long lastFrameIndex);
void    SWSetSpriteFrameTime(SpritePtr srcSpriteP, long timeInterval);
void    SWSetSpriteFrameProc(SpritePtr srcSpriteP, FrameProcPtr frameProc);
void    SWSetSpriteLocation(SpritePtr srcSpriteP, SWMovementType horizLoc, SWMovementType vertLoc);
void    SWSetSpriteMoveBounds(SpritePtr srcSpriteP, SWRect *moveBoundsRect);
void    SWSetSpriteMoveDelta(SpritePtr srcSpriteP, SWMovementType horizDelta, SWMovementType vertDelta);
void    SWSetSpriteMoveTime(SpritePtr srcSpriteP, long timeInterval);
long    SWGetSpriteMoveTime(SpritePtr srcSpriteP);
void    SWSetSpriteMoveProc(SpritePtr srcSpriteP, MoveProcPtr moveProc);
void    SWSetSpritePseudoMoveTime(SpritePtr srcSpriteP, long timeInterval);
void    SWSetSpriteVisible(SpritePtr srcSpriteP, SWBoolean isVisible);
void    SWSetSpriteTranslucency(SpritePtr srcSpriteP, unsigned long translucencyLevel);
void    SWSetSpriteTranslucencyLevel(SpritePtr srcSpriteP, unsigned long translucencyLevel);
void    SWSetSpriteColor(SpritePtr srcSpriteP, unsigned short red, unsigned short green, unsigned short blue);
void    SWSetSpriteBlendMode(SpritePtr srcSpriteP, unsigned int srcMode, unsigned int dstMode);
void    SWSetSpriteTemporaryClip(SpritePtr srcSpriteP, short left, short top, short right, short bottom);
void    SWSetSpriteClip(SpritePtr srcSpriteP, short left, short top, short right, short bottom);
void    SWSetSpriteFlipX(SpritePtr srcSpriteP, SWBoolean flip);
void    SWSetSpriteFlipY(SpritePtr srcSpriteP, SWBoolean flip);
void    SWSetSpriteRotation(SpritePtr srcSpriteP, float rotation);
void    SWSetSpriteScaledSize(SpritePtr srcSpriteP, float width, float height);
void    SWSetSpriteScaledWidth(SpritePtr srcSpriteP, float width);
void    SWSetSpriteScaledHeight(SpritePtr srcSpriteP, float height);
void    SWSetSpriteScaledWidthFactor(SpritePtr srcSpriteP, float factor);
void    SWSetSpriteScaledHeightFactor(SpritePtr srcSpriteP, float factor);
void    SWSetSpriteScaledFactor(SpritePtr srcSpriteP, float widthFactor, float heightFactor);
void    SWSetSpriteScalingMode(SpritePtr srcSpriteP, SWBoolean centered);
void    SWSetSpriteFastMovingCheck(SpritePtr srcSpriteP, SWBoolean check);
void    SWInvalidateSprite(SpritePtr srcSpriteP);
void    SWMountSprite(SpritePtr baseSpriteP, SpritePtr riderSpriteP, float offsetH, float offsetV,
                      SWBoolean copyFrame, SWBoolean drawRider, SWBoolean useGlobalOffset);
void    SWSetMountInheritsAttributes(SpritePtr spriteP, SWBoolean translucency, SWBoolean color,
                      SWBoolean scale, SWBoolean flip, SWBoolean rotation, SWBoolean visible);
void    SWSetSpriteMountOffset(SpritePtr spriteP, float offsetH, float offsetV);
void    SWSetGlobalMountOffset(SpriteWorldPtr spriteWorldP, float offsetH, float offsetV);
void    SWSetDrawTilesOverSpriteInsetRect(SpritePtr spriteP, short left, short top, short right, short bottom);
void    SWSetSpriteAutoScaling(SpritePtr spriteP, SWBoolean autoScale);
void    SWSetSpriteNonScrolling(SpritePtr spriteP, SWBoolean nonScrolling);
long    SWGetSpriteFrameTime(SpritePtr srcSpriteP);
SWMovementType SWGetSpriteVertLoc(SpritePtr srcSpriteP);
SWMovementType SWGetSpriteHorizLoc(SpritePtr srcSpriteP);
SWMovementType SWGetSpriteHorizMoveDelta(SpritePtr srcSpriteP);
SWMovementType SWGetSpriteVertMoveDelta(SpritePtr srcSpriteP);
float   SWGetSpriteWidth(SpritePtr srcSpriteP);
float   SWGetSpriteHeight(SpritePtr srcSpriteP);
unsigned long SWGetSpriteTranslucencyLevel(SpritePtr srcSpriteP);
SWBoolean SWGetSpriteVisible(SpritePtr srcSpriteP);
long    SWGetCurrentFrameIndex(SpritePtr srcSpriteP);
void    SWMoveSprite(SpritePtr srcSpriteP, SWMovementType horizLoc, SWMovementType vertLoc);
void    SWOffsetSprite(SpritePtr srcSpriteP, SWMovementType horizOffset, SWMovementType vertOffset);
void    SWOffsetSpriteBasedOnTime(SpritePtr srcSpriteP, SWMovementType horizOffset, SWMovementType vertOffset);
double  SWConvertOffsetBasedOnTime(SpriteWorldPtr spriteWorldP, double theOffset, double framesPerSecond);
void    SWResetSpriteFrameTimer(SpriteWorldPtr spriteWorldP, SpritePtr srcSpriteP);
SWBoolean SWHasMoveProcFired(SpritePtr srcSpriteP);
SWBoolean SWBounceSprite(SpritePtr srcSpriteP);
SWBoolean SWWrapSprite(SpritePtr srcSpriteP);
SWBoolean SWWrapSpriteHoriz(SpritePtr srcSpriteP);
SWBoolean SWWrapSpriteVert(SpritePtr srcSpriteP);
void    SWStdSpriteMoveProc(SpritePtr srcSpriteP);
void    SWWrapSpriteMoveProc(SpritePtr srcSpriteP);
void    SWWrapSpriteHorizMoveProc(SpritePtr srcSpriteP);
void    SWWrapSpriteVertMoveProc(SpritePtr srcSpriteP);
SWBoolean SWInPixelMask(SpritePtr srcSpriteP, int h, int v);
SWBoolean SWRadiusCollision(SpritePtr srcSpriteP, SpritePtr dstSpriteP);
SWBoolean SWPixelCollision(SpritePtr srcSpriteP, SpritePtr dstSpriteP);
SWBoolean SWIsSpriteInSprite(SpritePtr srcSpriteP, SpritePtr dstSpriteP);
SWBoolean SWIsSpriteInRect(SpritePtr srcSpriteP, SWRect *testRect);
SWBoolean SWIsSpriteFullyInRect(SpritePtr srcSpriteP, SWRect *testRect);
SWBoolean SWIsPointInSprite(SpritePtr srcSpriteP, SWPoint testPoint);
SWBoolean SWIsPointInSpriteMask(SpritePtr srcSpriteP, int h, int v, SpriteInMaskProcPtr testProc);

// ---- SpriteCreation.c ------------------------------------------------------

void    SWSetTransparentColor(SDL_Color *color);
SWError SWCreateSprite(SpritePtr *newSpriteP, void *spriteStorageP, long maxFrames);
DrawDataPtr SWCreateDrawData(void);
SWError SWCreateSpriteFromSingleFile(SpritePtr *newSpriteP, void *spriteStorageP, const char *fileName,
                    short frameDimension, short borderSize, SWBoolean selfMask);
SWError SWCreateSpriteFromSingleFileXY(SpritePtr *newSpriteP, void *spriteStorageP, const char *fileName,
                    int frameWidth, int frameHeight, int borderWidth, int borderHeight,
                    SWBoolean hasOutsideBorder, int maxFrames, SWBoolean selfMask);
SWError SWCreateSpriteFromVerticalFile(SpritePtr *newSpriteP, void *spriteStorageP, const char *fileName,
                    int borderHeight, int numFrames, SWBoolean selfMask);
SWError SWCreateSpriteFromSurfaceXY(SpritePtr *newSpriteP, void *spriteStorageP, SDL_Surface *surface,
                    int frameWidth, int frameHeight, int borderWidth, int borderHeight,
                    SWBoolean hasOutsideBorder, int maxFrames, SWBoolean selfMask);
SWError SWCreateSpriteFromFileSequence(SpritePtr *newSpriteP, void *spriteStorageP, const char *fileName,
                    int start, int numFrames, SWBoolean selfMask);
SWError SWCreateSpriteFromFile(SpritePtr *newSpriteP, void *spriteStorageP, const char *fileName,
                    long maxFrames, SWBoolean selfMask);
SWError SWUpdateSpriteFromFile(SpritePtr srcSpriteP, const char *fileName, SWBoolean selfMask);
SWError SWCreateSpriteFromWorkArea(SpriteWorldPtr spriteWorldP, SpritePtr *newSpriteP, void *spriteStorageP, SWRect *areaRectP);
SWError SWCloneSprite(SpritePtr cloneSpriteP, SpritePtr *newSpriteP, void *spriteStorageP, long storageSize);
SDL_Surface *SWDoSelfMaskingForSurface(SDL_Surface *surface);

// ---- SpriteFrame.c ---------------------------------------------------------

SWError SWCreateFrame(FramePtr *newFrameP);
SWError SWCreateFrameFromSurface(FramePtr *newFrameP, SDL_Surface *theSurface, int isVideoSurface);
SWError SWCreateFrameFromSurfaceAndRect(FramePtr *newFrameP, SDL_Surface *surface, SWRect *frameRect);
SWError SWCreateBlankFrame(FramePtr *newFrameP, int w, int h, unsigned char depth, SWBoolean createAlphaChannel);
SWError SWCreateBlankGLFrame(FramePtr *newFrameP, short w, short h, unsigned char depth);
SWError SWCreateFrameFromFile(FramePtr *newFrameP, const char *filename, SWBoolean selfMask);
SWError SWUpdateFrameFromFile(FramePtr frameP, const char *filename, SWBoolean selfMask);
SWError SWCreateFrameFromWorkArea(SpriteWorldPtr spriteWorldP, FramePtr *newFrameP, SWRect *areaRectP);
void    SWUpdateFrameFromWorkArea(SpriteWorldPtr spriteWorldP, FramePtr frameP, SWRect *areaRectP);
int     SWDisposeFrame(FramePtr *oldFramePP);
void    SWLockFrame(FramePtr srcFrameP);
void    SWUnlockFrame(FramePtr srcFrameP);
SWError SWUpdateFrame(FramePtr srcFrameP);
void    SWFreeGLTextureForFrame(FramePtr srcFrameP);
void    SWSetFrameHotSpot(FramePtr srcFrameP, DrawDataPtr parentSpriteDrawDataP, short hotSpotH, short hotSpotV);
void    SWSetFrameCollisionInset(FramePtr srcFrameP, short leftInset, short topInset, short rightInset, short bottomInset);

// ---- Tiling.c --------------------------------------------------------------

SWError SWInitTiling(SpriteWorldPtr spriteWorldP, short tileHeight, short tileWidth, short maxNumTiles);
void    SWExitTiling(SpriteWorldPtr spriteWorldP);
void    SWSetTileResolution(SpriteWorldPtr spriteWorldP, short tileImageWidth, short tileImageHeight);
SWError SWCreateTileMap(TileMapStructPtr *tileMapStructPP, short numTileMapRows, short numTileMapCols);
SWError SWCloneTileMap(TileMapStructPtr srcTileMapP, TileMapStructPtr *newTileMapPP);
void    SWResetTileMapForLayer(SpriteWorldPtr spriteWorldP, short tileLayer);
void    SWResetTileMap(TileMapStructPtr tileMapP);
void    SWDisposeTileMap(TileMapStructPtr *tileMapStructPP);
void    SWInstallTileMap(SpriteWorldPtr spriteWorldP, TileMapStructPtr tileMapStructP, short tileLayer);
SWError SWLoadTileMap(const char *fileName, TileMapStructPtr *tileMapStructPP);
SWError SWSaveTileMap(const char *fileName, TileMapStructPtr tileMapStructP);
void    SWInstallTileLayerBelowSpriteLayer(short tileLayer, SpriteLayerPtr spriteLayerP);
void    SWInstallTileLayerAboveSpriteLayer(short tileLayer, SpriteLayerPtr spriteLayerP);
void    SWFillTileMapWithValue(TileMapStructPtr tileMapP, short value);
void    SWSetTileMapZoom(TileMapStructPtr tileMapP, float zoom);
void    SWSetTileLayersVisible(SpriteWorldPtr spriteWorldP, short firstLayer, short lastLayer, SWBoolean visible);
void    SWSetTileLayerToDrawEveryFrame(SpriteWorldPtr spriteWorldP, short tileLayer, SWBoolean drawEveryFrame);
void    SWSetTileTranslucency(SpriteWorldPtr spriteWorldP, short tileID, unsigned short translucency);
void    SWSetTileTranslucencyForRange(SpriteWorldPtr spriteWorldP, short firstTileID, short lastTileID, unsigned short translucency);
void    SWSetTileColor(SpriteWorldPtr spriteWorldP, short tileID, unsigned short red, unsigned short green, unsigned short blue);
void    SWSetTileColorForRange(SpriteWorldPtr spriteWorldP, short firstTileID, short lastTileID,
                    unsigned short red, unsigned short green, unsigned short blue, unsigned short alpha);
void    SWSetTileLayerCallback(SpriteWorldPtr spriteWorldP, short tileLayer, TileLayerCallBackPtr callBack);
void    SWSetStdTileColor(SpriteWorldPtr spriteWorldP, unsigned short red, unsigned short green,
                    unsigned short blue, unsigned short alpha, SWBoolean useStdColor);
void    SWCloneTiles(SpriteWorldPtr spriteWorldP, short firstTileID, short lastTileID, short sourceTileID);
SWError SWLoadTilesFromSingleFileXY(SpriteWorldPtr spriteWorldP, const char *fileName, int borderWidth,
                    int borderHeight, SWBoolean hasOutsideBorder, int numTiles, short startTileID);
SWError SWLoadTilesFromFileSequence(SpriteWorldPtr spriteWorldP, const char *fileName, int start,
                    int numTiles, short startTileID);
void    SWDisposeTile(SpriteWorldPtr spriteWorldP, short tileID);
void    SWSetTilingOn(SpriteWorldPtr spriteWorldP, SWBoolean tilingIsOn);
void    SWSetTileChangeProc(SpriteWorldPtr spriteWorldP, TileChangeProcPtr tileChangeProc);
void    SWDrawTile(SpriteWorldPtr spriteWorldP, short dstTileLayer, short tileRow, short tileCol, short tileID);
void    SWCreateBackgroundTextureFromTiles(SpriteWorldPtr spriteWorldP);
void    SWDisposeBackgroundTileTexture(SpriteWorldPtr spriteWorldP);
void    SWDrawTileLayersInRect(SpriteWorldPtr spriteWorldP, SWRect *updateRectP);
SWBoolean SWDrawTileLayerInRect(SpriteWorldPtr spriteWorldP, short tileLayer, SWRect *updateRectP,
                    SWBoolean useChangedMap, SWBoolean forceDraw);
void    SWResetAllChangedTilesMaps(SpriteWorldPtr spriteWorldP);
void    SWDrawTilesAboveSprite(SpriteWorldPtr spriteWorldP, SWRect *updateRectP, short startLayer);
void    SWChangeTileImage(SpriteWorldPtr spriteWorldP, short tileID, short currentFrame);
void    SWUpdateTileOnScreen(SpriteWorldPtr spriteWorldP, short tileID);
void    SWUpdateTilesOnScreenFast(SpriteWorldPtr spriteWorldP, short firstTileID, short lastTileID);
void    SWResetCurrentTileImages(SpriteWorldPtr spriteWorldP);
short   SWReturnTileUnderPixel(SpriteWorldPtr spriteWorldP, short tileLayer, short pixelCol, short pixelRow);
SWBoolean SWCheckSpriteWithTiles(SpriteWorldPtr spriteWorldP, SpritePtr srcSpriteP, SWTileSearchType searchType,
                    SWRect *insetRectP, short startTileLayer, short endTileLayer, short firstTileID,
                    short lastTileID, SWBoolean fixPosition);
void    SWResetVBOForAllTileLayers(SpriteWorldPtr spriteWorldP);
void    SWSetVBOForTiling(SpriteWorldPtr spriteWorldP, SWBoolean useVBO);
void    SWSetTileMapVBOMethod(TileMapStructPtr tileMapP, SWBoolean method);

// ---- SpriteWorldUtils.c ----------------------------------------------------

void    SWCenterRect(SWRect *rectA, SWRect *rectB);
void    SWSetRect(SWRect *rectP, short left, short top, short right, short bottom);
void    SWOffsetRect(SWRect *rectP, short horizOffset, short vertOffset);
void    SWInsetRect(SWRect *rectP, short horizInset, short vertInset);
SWBoolean SWEqualRect(SWRect *rectA, SWRect *rectB);
void    Randomize(void);
void    SWSeedRandom(unsigned long seed);     ///< GetRandom's numbers, the same on every computer
unsigned int SWRandomState(void);
double  SWSinDegrees(double degrees);         ///< the same result on every system
double  SWCosDegrees(double degrees);
int     GetRandom(int min, int max);
float   GetRandomFloat(float min, float max);
void    SWCopyPascalString(const unsigned char *sourceString, unsigned char *targetString);
void    SWCopyCString(const char *sourceCString, char *targetCString);
SWBoolean SWEqualString(const unsigned char *stringA, const unsigned char *stringB);
void    SWCopyCStringToPascalString(const char *sourceCString, unsigned char *targetPString);
void    SWCopyPascalStringToCString(const unsigned char *sourcePString, char *targetCString);
void    SWConcatPascalStrings(unsigned char *targetString, const unsigned char *appendString);
void    SWStringToNum(const unsigned char *pString, long *theNum);
void    SWNumToString(long theNum, unsigned char *pString);
double  SWCos(double degrees);
double  SWSin(double degrees);
double  SWATan(double value);
void    ReportError(int errNum, const char *file, const char *function, int line);

#ifdef __cplusplus
}
#endif

#include "SWStats.h"
#include "SWTextSprite.h"

#endif /* __SPRITEWORLD__ */
