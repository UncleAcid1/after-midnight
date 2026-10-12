/*
 * SpriteWorld X — reconstruction of the private fork used by Midnight Mansion HD.
 *
 * Based on SpriteWorld X (C) 2003-2006 Michael Cabral, Ben Henke, Justin Van Eaton,
 * David Beck, Anders F Bjorklund, and SpriteWorld (C) 1995-2005 Karl Bunker, Vern Jensen,
 * Anders Bjorklund, Ken Pajala, (C) 1991-1994 Tony Myles. zlib license (see
 * third_party/SpriteWorldX/LICENSE).
 *
 * ALTERED SOURCE VERSION: rewritten from the fork's compiled code for SDL2 + OpenGL.
 * Only the OpenGL rendering path of the fork is implemented.
 */

#ifndef __SWCOMMON__
#define __SWCOMMON__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <SDL.h>
#include "BKGL.h"

typedef float SWMovementType;

typedef struct SpriteWorldRec   SpriteWorldRec, *SpriteWorldPtr;
typedef struct TileMapStruct    TileMapStruct, *TileMapStructPtr;
typedef struct SpriteLayerRec   SpriteLayerRec, *SpriteLayerPtr;
typedef struct SpriteRec        SpriteRec, *SpritePtr;
typedef struct FrameRec         FrameRec, *FramePtr;
#ifndef BKGLSURFACE_TYPEDEF
#define BKGLSURFACE_TYPEDEF
typedef struct BKGLSurface BKGLSurface;
#endif

typedef unsigned char SWBoolean;
typedef char SWStr255[256];

#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

typedef struct SWPoint {
    short v;
    short h;
} SWPoint, *SWPointPtr;

typedef struct SWRect {
    short top;
    short left;
    short bottom;
    short right;
} SWRect, *SWRectPtr;

typedef struct SWRGBColor {
    unsigned short red;
    unsigned short green;
    unsigned short blue;
} SWRGBColor, *SWRGBColorPtr;

/// Per-sprite drawing state. Field order follows the fork (0x58 bytes on i386).
typedef struct DrawDataRec {
    SpritePtr       parentSpriteP;
    SWMovementType  horizLoc;           ///< horizontal sprite position (hotspot)
    SWMovementType  vertLoc;            ///< vertical sprite position (hotspot)
    SWMovementType  horizMoveDelta;
    SWMovementType  vertMoveDelta;
    unsigned long   translucencyLevel;  ///< 0..0xFFFF, used as GL alpha
    float           rotation;           ///< degrees
    float           rotationOffsetHoriz;
    float           rotationOffsetVert;
    float           scaledWidth;        ///< <= 0 means unscaled
    float           scaledHeight;
    float           scaledWidthFactor;
    float           scaledHeightFactor;
    short           frameHotSpotH;
    short           frameHotSpotV;
    SWBoolean       flippedHorizontal;
    SWBoolean       flippedVertical;
    SWBoolean       useClip;
    SWBoolean       temporaryClip;      ///< clip is cleared after the next draw
    short           clipLeft, clipTop, clipRight, clipBottom;   ///< insets in pixels
    unsigned int    srcBlendMode;
    unsigned int    dstBlendMode;
    SWBoolean       useBlendMode;
    SWBoolean       autoScaling;        ///< scale with the world's design resolution
    SWBoolean       nonScrolling;       ///< ignore the world's sprite offset
    unsigned short  colorRed;
    unsigned short  colorGreen;
    unsigned short  colorBlue;
} DrawDataRec, *DrawDataPtr;

enum SWError {
    kNoError                    = 0,
    kSystemTooOldErr            = 100,
    kMaxFramesErr               = 101,
    kInvalidFramesIndexErr      = 102,
    kNotCWindowErr              = 103,
    kNilParameterErr            = 104,
    kWrongDepthErr              = 105,
    kWrongMaskErr               = 106,
    kOutOfRangeErr              = 107,
    kTilingNotInitialized       = 108,
    kTilingAlreadyInitialized   = 109,
    kNullTileMapErr             = 110,
    kTileMapNotLockedErr        = 111,
    kAlreadyCalledErr           = 112,
    kSpriteNotCompiledErr       = 113,
    kBadParameterErr            = 114,
    kSpriteAlreadyInLayer       = 115,
    kNilFrameErr                = 116,
    kNotLockedErr               = 117,
    kMemoryAllocationError      = 200,
    kSpriteCreationError        = 201,
    kFrameCreationError         = 202,
    kFailedToInitSDL            = 300,
    kSDLSetVideoMode            = 301,
    kSDLCreateSurface           = 302,
    kSDLCreateSurfaceFromFile   = 303,
    kSDLBlitError               = 304,
    KSDLBlitVideoMemoryError    = 305,
    kSDLSurfaceConversion       = 306,
    kSDLSetAlpha                = 307,
    kUnknownError               = 999
};
typedef int SWError;

typedef void (*AssertFailProcPtr)(const char *filename, const char *functionname, int lineNum);
extern AssertFailProcPtr gSWAssertFailProc;

#ifndef SW_ASSERT_ON
#define SW_ASSERT_ON 1
#endif

#if SW_ASSERT_ON
#define SW_ASSERT(condition) \
    do { if (!(condition)) gSWAssertFailProc(__FILE__, __func__, __LINE__); } while (0)
#else
#define SW_ASSERT(condition) do {} while (0)
#endif

#define SW_UNUSED(var)  (void) var
#define SW_ABS(x)       ((x) < 0 ? -(x) : (x))
#define SW_MIN(a, b)    ((a) < (b) ? (a) : (b))
#define SW_MAX(a, b)    ((a) > (b) ? (a) : (b))

#define SW_RECT_WIDTH(r)    ((r).right - (r).left)
#define SW_RECT_HEIGHT(r)   ((r).bottom - (r).top)

#define SW_SET_RECT(theRect, myleft, mytop, myright, mybottom) \
    do { (theRect).top = (mytop); (theRect).left = (myleft); \
         (theRect).bottom = (mybottom); (theRect).right = (myright); } while (0)

#define SW_OFFSET_RECT(theRect, deltaH, deltaV) \
    do { (theRect).top += (deltaV); (theRect).left += (deltaH); \
         (theRect).bottom += (deltaV); (theRect).right += (deltaH); } while (0)

#define SW_INSET_RECT(theRect, insetX, insetY) \
    do { (theRect).top += (insetY); (theRect).left += (insetX); \
         (theRect).bottom -= (insetY); (theRect).right -= (insetX); } while (0)

#define SW_RECT_IS_IN_RECT(rectA, rectB) \
    ( ((rectA).top < (rectB).bottom) && ((rectB).top < (rectA).bottom) && \
      ((rectA).left < (rectB).right) && ((rectB).left < (rectA).right) )

#define SW_CLIP_RECT(dst, clip) \
    do { if ((dst).top < (clip).top) (dst).top = (clip).top; \
         if ((dst).bottom > (clip).bottom) (dst).bottom = (clip).bottom; \
         if ((dst).left < (clip).left) (dst).left = (clip).left; \
         if ((dst).right > (clip).right) (dst).right = (clip).right; } while (0)

/// The SpriteWorld currently being processed or drawn (NULL between frames).
extern SpriteWorldPtr gSWCurrentSpriteWorld;
extern SWBoolean      gSWUseOpenGL;

#endif /* __SWCOMMON__ */
