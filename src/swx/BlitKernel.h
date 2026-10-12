/*
 * BlitKernel — helpers for SDL surfaces and OpenGL textures.
 * Based on BlitKernel (C) 2005 Anders F Bjoerklund, zlib license.
 * ALTERED SOURCE VERSION: reconstructed from the Midnight Mansion HD build for SDL2.
 */
#ifndef __BLITKERNEL__
#define __BLITKERNEL__

#include <SDL.h>
#include "BKGL.h"

#ifdef __cplusplus
extern "C" {
#endif

/// A texture made from an SDL surface (or a rectangle of one).
/// The original split large images into power-of-two tiles; modern OpenGL takes
/// any size, so this is always a single texture covering width x height pixels.
#ifndef BKGLSURFACE_TYPEDEF
#define BKGLSURFACE_TYPEDEF
typedef struct BKGLSurface BKGLSurface;
#endif
struct BKGLSurface {
    SDL_Surface *surface;       ///< RGBA copy of the pixels (kept for pixel tests / reloads)
    int          ownsSurface;
    GLuint       texture;
    int          width;         ///< size of the drawn quad in pixels
    int          height;
    int          needsBlend;    ///< source had an alpha channel or color key
    int          flipV;         ///< texture rows run bottom-up (copied from the framebuffer)
};

// ---- SDL surface utilities -------------------------------------------------

/// Loads a PNG/JPG/TGA/BMP file. Images without alpha load as 24-bit surfaces.
SDL_Surface *BKLoadSurface(const char *filename);
SDL_Surface *BKCreateSurface(int w, int h, int bpp, SDL_bool video, SDL_bool mask, SDL_bool alpha);
SDL_Surface *BKCreateGLSurface(int w, int h, int bpp, SDL_bool power2, SDL_bool mask, SDL_bool alpha);

/// True when the surface carries a per-pixel alpha channel (SDL 1.3's SDL_SRCALPHA).
SDL_bool BKSurfaceHasAlpha(SDL_Surface *surface);

int BKFlipHorizontal(SDL_Surface *surface);
int BKFlipVertical(SDL_Surface *surface);
int BKConvertColorKeyToAlphaChannel(SDL_Surface *surface, SDL_Color *colorkey);

SDL_bool BKTransparentPixel(SDL_Surface *surface, int x, int y);
SDL_bool BKCollideBoundingBox(SDL_Surface *sa, int ax, int ay, SDL_Surface *sb, int bx, int by);
/// Pixel-accurate collision between two (possibly scaled and flipped) frames.
SDL_bool BKCollidePixels(SDL_Surface *sa, const short *aDestRect, SDL_Surface *sb, const short *bDestRect,
                         const short *aFrameRect, const short *bFrameRect,
                         float aScaleH, float aScaleV, float bScaleH, float bScaleV,
                         int aFlipH, int bFlipH, int aFlipV, int bFlipV);

void   BKPutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
Uint32 BKGetPixel(SDL_Surface *surface, int x, int y);

// ---- OpenGL surfaces -------------------------------------------------------

BKGLSurface *BKCreateNewGLSurface(SDL_Surface *surface, const SDL_Rect *rect);
BKGLSurface *BKCreateBlankGLSurface(int w, int h, int depth, SDL_bool alpha);
void         BKFreeGLSurface(BKGLSurface *glsurface);
SDL_Surface *BKGetSDLSurface(BKGLSurface *glsurface);
int          DoesSurfaceNeedBlend(BKGLSurface *glsurface);
int          BKReloadGLSurface(BKGLSurface *glsurface, const SDL_Rect *rect);
/// Creates a texture of texW x texH pixels that draws as a quad of width x height.
BKGLSurface *BKCreateFramebufferGLSurface(int texW, int texH, int width, int height);
/// Copies a rectangle of the framebuffer (GL window coordinates) into the surface.
void         BKCopyFramebufferToGLSurface(BKGLSurface *glsurface, int x, int y, int w, int h);

/// Draws the surface with its top-left at the current origin.
int BKBlitGLSurface(BKGLSurface *glsurface);
/// Draws with each edge pulled in by the given number of pixels.
int BKBlitClippedGLSurface(BKGLSurface *glsurface, short left, short top, short right, short bottom);
/// Draws with each corner moved by its own offset (used for perspective door effects).
int BKBlitCustomGLSurface(BKGLSurface *glsurface, short tlH, short tlV, short trH, short trV,
                          short blH, short blV, short brH, short brV);

const char *BKGetGLErrorMessage(GLenum error);
void BKCheckGLError(const char *file, const char *function, int line);
#define BK_CHECK_GL() BKCheckGLError(__FILE__, __func__, __LINE__)

#ifdef __cplusplus
}
#endif

#endif /* __BLITKERNEL__ */
