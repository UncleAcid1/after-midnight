/*
 * BlitKernel SDL surface helpers.
 * Based on BlitKernel (C) 2005 Anders F Bjoerklund, zlib license.
 * ALTERED SOURCE VERSION: reconstructed from the Midnight Mansion HD build, ported to SDL2.
 * Images are decoded with stb_image instead of SDL_image.
 */

#include "BlitKernel.h"
#include "SWCommonHeaders.h"

#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_ONLY_TGA
#define STBI_ONLY_BMP
#define STBI_ONLY_GIF
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_bool BKSurfaceHasAlpha(SDL_Surface *surface)
{
    return (surface != NULL && surface->format->Amask != 0) ? SDL_TRUE : SDL_FALSE;
}

static SDL_bool BKSurfaceHasColorKey(SDL_Surface *surface)
{
    return SDL_HasColorKey(surface);
}

/*  Fully transparent pixels keep whatever colour the image editor left in them (often
    white). Linear texture filtering blends that colour into the edges of every sprite,
    giving a thin light fringe over dark backdrops. Give each transparent pixel the
    average colour of its visible neighbours instead, spreading outwards a few pixels. */
static void BKBleedTransparentEdges(Uint8 *pixels, int w, int h, int pitch)
{
    Uint8 *known;
    int pass, x, y, dx, dy, changed;

    known = (Uint8 *)malloc((size_t)w * h);
    if (known == NULL)
        return;
    for (y = 0; y < h; y++)
        for (x = 0; x < w; x++)
            known[y * w + x] = pixels[y * pitch + x * 4 + 3] != 0;
    for (pass = 1; pass <= 4; pass++) {
        changed = 0;
        for (y = 0; y < h; y++) {
            for (x = 0; x < w; x++) {
                int r = 0, g = 0, b = 0, n = 0;
                Uint8 *p;
                if (known[y * w + x] != 0)
                    continue;
                for (dy = -1; dy <= 1; dy++) {
                    for (dx = -1; dx <= 1; dx++) {
                        int nx = x + dx, ny = y + dy;
                        if (nx < 0 || ny < 0 || nx >= w || ny >= h)
                            continue;
                        if (known[ny * w + nx] == 0 || known[ny * w + nx] > pass)
                            continue;
                        p = pixels + ny * pitch + nx * 4;
                        r += p[0]; g += p[1]; b += p[2]; n++;
                    }
                }
                if (n == 0)
                    continue;
                p = pixels + y * pitch + x * 4;
                p[0] = (Uint8)(r / n); p[1] = (Uint8)(g / n); p[2] = (Uint8)(b / n);
                known[y * w + x] = (Uint8)(pass + 1);
                changed = 1;
            }
        }
        if (!changed)
            break;
    }
    free(known);
}

/*  Loads an image file. Images with an alpha channel load as 32-bit RGBA surfaces,
    the rest as 24-bit RGB, so that "has alpha" checks behave as in the original. */
SDL_Surface *BKLoadSurface(const char *filename)
{
    int w, h, channels;
    unsigned char *pixels;
    SDL_Surface *surface;
    SDL_RWops *rw;
    Sint64 size;
    unsigned char *data;

    rw = SDL_RWFromFile(filename, "rb");
    if (rw == NULL)
        return NULL;
    size = SDL_RWsize(rw);
    data = (size > 0) ? malloc((size_t)size) : NULL;
    if (data == NULL || SDL_RWread(rw, data, 1, (size_t)size) != (size_t)size) {
        free(data);
        SDL_RWclose(rw);
        fprintf(stderr, "BKLoadSurface: cannot read %s\n", filename);
        return NULL;
    }
    SDL_RWclose(rw);

    pixels = stbi_load_from_memory(data, (int)size, &w, &h, &channels, 0);
    free(data);
    if (pixels == NULL) {
        fprintf(stderr, "IMG_Load %s: %s\n", filename, stbi_failure_reason());
        SDL_SetError("%s", stbi_failure_reason());
        return NULL;
    }

    if (channels == 4 || channels == 2) {
        surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_ABGR8888);
        if (surface != NULL) {
            int x, y;
            for (y = 0; y < h; y++) {
                Uint8 *dst = (Uint8 *)surface->pixels + y * surface->pitch;
                const Uint8 *src = pixels + (size_t)y * w * channels;
                for (x = 0; x < w; x++) {
                    if (channels == 4) {
                        dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3];
                    } else {
                        dst[0] = dst[1] = dst[2] = src[0]; dst[3] = src[1];
                    }
                    dst += 4;
                    src += channels;
                }
            }
            BKBleedTransparentEdges((Uint8 *)surface->pixels, w, h, surface->pitch);
        }
    } else {
        surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 24, SDL_PIXELFORMAT_RGB24);
        if (surface != NULL) {
            int x, y;
            for (y = 0; y < h; y++) {
                Uint8 *dst = (Uint8 *)surface->pixels + y * surface->pitch;
                const Uint8 *src = pixels + (size_t)y * w * channels;
                for (x = 0; x < w; x++) {
                    if (channels == 3) {
                        dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
                    } else {
                        dst[0] = dst[1] = dst[2] = src[0];
                    }
                    dst += 3;
                    src += channels;
                }
            }
        }
    }
    stbi_image_free(pixels);
    return surface;
}

SDL_Surface *BKCreateSurface(int w, int h, int bpp, SDL_bool video, SDL_bool mask, SDL_bool alpha)
{
    SDL_Surface *surface;
    SW_UNUSED(video);

    if (bpp != 32 && bpp != 24)
        bpp = 32;
    if (alpha || bpp == 32)
        surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, alpha ? SDL_PIXELFORMAT_ABGR8888 : SDL_PIXELFORMAT_XBGR8888);
    else
        surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 24, SDL_PIXELFORMAT_RGB24);
    if (surface != NULL && mask)
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 255, 0));
    return surface;
}

SDL_Surface *BKCreateGLSurface(int w, int h, int bpp, SDL_bool power2, SDL_bool mask, SDL_bool alpha)
{
    SW_UNUSED(bpp);
    if (power2) {
        int pw = 1, ph = 1;
        while (pw < w) pw <<= 1;
        while (ph < h) ph <<= 1;
        w = pw;
        h = ph;
    }
    if (!mask && !alpha)
        return SDL_CreateRGBSurfaceWithFormat(0, w, h, 24, SDL_PIXELFORMAT_RGB24);
    return SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_ABGR8888);
}

int BKFlipHorizontal(SDL_Surface *surface)
{
    int x, y, bpp = surface->format->BytesPerPixel;
    Uint8 tmp[4];

    if (SDL_MUSTLOCK(surface) && SDL_LockSurface(surface) < 0)
        return -1;
    for (y = 0; y < surface->h; y++) {
        Uint8 *row = (Uint8 *)surface->pixels + y * surface->pitch;
        for (x = 0; x < surface->w / 2; x++) {
            Uint8 *a = row + x * bpp, *b = row + (surface->w - 1 - x) * bpp;
            memcpy(tmp, a, bpp);
            memcpy(a, b, bpp);
            memcpy(b, tmp, bpp);
        }
    }
    if (SDL_MUSTLOCK(surface))
        SDL_UnlockSurface(surface);
    return 0;
}

int BKFlipVertical(SDL_Surface *surface)
{
    int y;
    Uint8 *tmp = malloc(surface->pitch);

    if (tmp == NULL)
        return -1;
    if (SDL_MUSTLOCK(surface) && SDL_LockSurface(surface) < 0) {
        free(tmp);
        return -1;
    }
    for (y = 0; y < surface->h / 2; y++) {
        Uint8 *a = (Uint8 *)surface->pixels + y * surface->pitch;
        Uint8 *b = (Uint8 *)surface->pixels + (surface->h - 1 - y) * surface->pitch;
        memcpy(tmp, a, surface->pitch);
        memcpy(a, b, surface->pitch);
        memcpy(b, tmp, surface->pitch);
    }
    if (SDL_MUSTLOCK(surface))
        SDL_UnlockSurface(surface);
    free(tmp);
    return 0;
}

/*  Makes every pixel of the key color fully transparent and every other pixel opaque.
    The surface must be 32-bit with an alpha channel. */
int BKConvertColorKeyToAlphaChannel(SDL_Surface *surface, SDL_Color *colorkey)
{
    int x, y;
    Uint32 key, mask, amask;

    if (!BKSurfaceHasAlpha(surface)) {
        SDL_SetError("Surface has no alpha channel");
        return -1;
    }
    if (surface->format->BitsPerPixel != 32) {
        SDL_SetError("Unsupported surface bit depth");
        return -1;
    }
    if (SDL_MUSTLOCK(surface) && SDL_LockSurface(surface) < 0)
        return 1;

    mask = surface->format->Rmask | surface->format->Gmask | surface->format->Bmask;
    amask = surface->format->Amask;
    if (colorkey != NULL)
        key = SDL_MapRGB(surface->format, colorkey->r, colorkey->g, colorkey->b);
    else
        key = SDL_MapRGB(surface->format, 0, 255, 0);
    key &= mask;

    for (y = 0; y < surface->h; y++) {
        Uint32 *row = (Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch);
        for (x = 0; x < surface->w; x++) {
            if ((row[x] & mask) == key)
                row[x] = 0;
            else
                row[x] |= amask;
        }
    }
    if (SDL_MUSTLOCK(surface))
        SDL_UnlockSurface(surface);
    return 0;
}

SDL_bool BKTransparentPixel(SDL_Surface *surface, int x, int y)
{
    Uint32 pixel;

    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h)
        return SDL_TRUE;
    pixel = BKGetPixel(surface, x, y);
    if (BKSurfaceHasColorKey(surface))
        return pixel == SDL_MapRGB(surface->format, 0, 255, 0);
    if (BKSurfaceHasAlpha(surface))
        return (pixel & surface->format->Amask) == 0;
    return SDL_TRUE;
}

SDL_bool BKCollideBoundingBox(SDL_Surface *sa, int ax, int ay, SDL_Surface *sb, int bx, int by)
{
    if (bx + sb->w < ax) return SDL_FALSE;
    if (bx > ax + sa->w) return SDL_FALSE;
    if (by + sb->h < ay) return SDL_FALSE;
    if (by > ay + sa->h) return SDL_FALSE;
    return SDL_TRUE;
}

/*  Pixel collision between two sprites' current frames.
    aDest/bDest are the sprites' destination rects and aFrame/bFrame the source rects
    within each surface (both SWRect: top, left, bottom, right). The scale factors
    convert destination pixels to source pixels; flips mirror the lookup. Walks the
    overlap of the two destination rects and reports the first pixel opaque in both. */
SDL_bool BKCollidePixels(SDL_Surface *sa, const short *aDest, SDL_Surface *sb, const short *bDest,
                         const short *aFrame, const short *bFrame,
                         float aScaleH, float aScaleV, float bScaleH, float bScaleV,
                         int aFlipH, int bFlipH, int aFlipV, int bFlipV)
{
    // SWRect indices
    enum { T = 0, L = 1, B = 2, R = 3 };
    int top, left, bottom, right, row, col;
    float aStepH, bStepH, aStepV, bStepV, aStartH, bStartH, aV, bV;
    SDL_bool result = SDL_FALSE;

    if (!BKCollideBoundingBox(sa, aDest[L], aDest[T], sb, bDest[L], bDest[T]))
        return SDL_FALSE;
    if (SDL_MUSTLOCK(sa) && SDL_LockSurface(sa) < 0)
        return SDL_FALSE;
    if (SDL_MUSTLOCK(sb) && SDL_LockSurface(sb) < 0) {
        if (SDL_MUSTLOCK(sa)) SDL_UnlockSurface(sa);
        return SDL_FALSE;
    }

    aStepH = 1.0f / aScaleH;
    bStepH = 1.0f / bScaleH;
    aStepV = 1.0f / aScaleV;
    bStepV = 1.0f / bScaleV;

    left = SW_MAX(bDest[L], aDest[L]);
    top = SW_MAX(bDest[T], aDest[T]);
    right = SW_MIN(bDest[R], aDest[R]);
    bottom = SW_MIN(bDest[B], aDest[B]);

    if (!aFlipH)
        aStartH = (float)(left - aDest[L]) * aStepH + aFrame[L];
    else {
        aStartH = ((float)(aDest[R] - aDest[L]) - (float)(left - aDest[L])) * aStepH + aFrame[L];
        aStepH = -aStepH;
    }
    if (!bFlipH)
        bStartH = (float)(left - bDest[L]) * bStepH + bFrame[L];
    else {
        bStartH = ((float)(bDest[R] - bDest[L]) - (float)(left - bDest[L])) * bStepH + bFrame[L];
        bStepH = -bStepH;
    }
    if (!aFlipV)
        aV = (float)(top - aDest[T]) * aStepV + aFrame[T];
    else {
        aV = ((float)(aDest[B] - aDest[T]) - (float)(top - aDest[T])) * aStepV + aFrame[T];
        aStepV = -aStepV;
    }
    if (!bFlipV)
        bV = (float)(top - bDest[T]) * bStepV + bFrame[T];
    else {
        bV = ((float)(bDest[B] - bDest[T]) - (float)(top - bDest[T])) * bStepV + bFrame[T];
        bStepV = -bStepV;
    }

    row = top;
    if (sa->format->BytesPerPixel == 4 && sb->format->BytesPerPixel == 4 &&
        BKSurfaceHasAlpha(sa) && BKSurfaceHasAlpha(sb))
    {
            // skip rows that fall outside either frame
        while ((aStepV > 0 && aV < aFrame[T]) || (bStepV > 0 && bV < bFrame[T])) {
            row++; aV += aStepV; bV += bStepV;
        }
        while ((aStepV < 0 && aV >= aFrame[B]) || (bStepV < 0 && bV >= bFrame[B])) {
            row++; aV += aStepV; bV += bStepV;
        }
        for (; row < bottom; row++) {
            float aH = aStartH, bH = bStartH;
            if (aV >= (float)sa->h || bV >= (float)sb->h)
                break;
            col = left;
            while ((aStepH > 0 && aH < aFrame[L]) || (bStepH > 0 && bH < bFrame[L])) {
                aH += aStepH; bH += bStepH; col++;
            }
            while ((aStepH < 0 && aH >= aFrame[R]) || (bStepH < 0 && bH >= bFrame[R])) {
                col++; aH += aStepH; bH += bStepH;
            }
            for (; col < right; col++) {
                int ax = (short)(int)aH, ay = (short)(int)aV, bx = (short)(int)bH, by = (short)(int)bV;
                Uint32 pa, pb;
                if (ax >= sa->w || bx >= sb->w)
                    break;
                if (ax >= 0 && ay >= 0 && bx >= 0 && by >= 0) {
                    pa = *(Uint32 *)((Uint8 *)sa->pixels + ay * sa->pitch + ax * 4);
                    pb = *(Uint32 *)((Uint8 *)sb->pixels + by * sb->pitch + bx * 4);
                    if ((pa & sa->format->Amask) != 0 && (pb & sb->format->Amask) != 0) {
                        result = SDL_TRUE;
                        goto done;
                    }
                }
                aH += aStepH;
                bH += bStepH;
            }
            aV += aStepV;
            bV += bStepV;
        }
    }
    else
    {
        for (; row < bottom; row++) {
            float aH = aStartH, bH = bStartH;
            for (col = left; col < right; col++) {
                if (!BKTransparentPixel(sa, (int)aH, (int)aV) && !BKTransparentPixel(sb, (int)bH, (int)bV)) {
                    result = SDL_TRUE;
                    goto done;
                }
                aH += aStepH;
                bH += bStepH;
            }
            aV += aStepV;
            bV += bStepV;
        }
    }

done:
    if (SDL_MUSTLOCK(sa)) SDL_UnlockSurface(sa);
    if (SDL_MUSTLOCK(sb)) SDL_UnlockSurface(sb);
    return result;
}

void BKPutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1: *p = (Uint8)pixel; break;
        case 2: *(Uint16 *)p = (Uint16)pixel; break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff; p[1] = (pixel >> 8) & 0xff; p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff; p[1] = (pixel >> 8) & 0xff; p[2] = (pixel >> 16) & 0xff;
            }
            break;
        case 4: *(Uint32 *)p = pixel; break;
    }
}

Uint32 BKGetPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1: return *p;
        case 2: return *(Uint16 *)p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            return p[0] | p[1] << 8 | p[2] << 16;
        case 4: return *(Uint32 *)p;
    }
    return 0;
}
