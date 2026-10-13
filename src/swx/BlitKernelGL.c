/*
 * BlitKernel OpenGL surfaces.
 * Based on BlitKernel (C) 2005 Anders F Bjoerklund, zlib license.
 * ALTERED SOURCE VERSION: reconstructed from the Midnight Mansion HD build.
 *
 * The original uploaded each surface as a grid of rectangle textures and drew it with
 * per-tile quads. Here a surface is one GL_TEXTURE_2D (non-power-of-two sizes are fine
 * on OpenGL 2.0+), drawn as one quad from (0,0) to (width,height) in the current
 * modelview space, with texture coordinates in [0,1].
 */

#include "BlitKernel.h"
#include "SWCommonHeaders.h"

static GLuint BKUploadTexture(SDL_Surface *rgba)
{
    GLuint tex = 0;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, rgba->pitch / 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, rgba->w, rgba->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba->pixels);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    BK_CHECK_GL();
    return tex;
}

/*  Copies the given rect of a surface into a new RGBA surface (byte order R,G,B,A).
    A color key becomes transparent alpha. */
static SDL_Surface *BKCopyToRGBA(SDL_Surface *surface, const SDL_Rect *rect)
{
    SDL_Rect src = rect ? *rect : (SDL_Rect){ 0, 0, surface->w, surface->h };
    SDL_Surface *converted, *out;
    SDL_bool hadKey = SDL_HasColorKey(surface);

    converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    if (converted == NULL)
        return NULL;
    out = SDL_CreateRGBSurfaceWithFormat(0, src.w, src.h, 32, SDL_PIXELFORMAT_RGBA32);
    if (out != NULL) {
        int y;
        SDL_Rect clipped = src;
        if (clipped.x + clipped.w > converted->w) clipped.w = converted->w - clipped.x;
        if (clipped.y + clipped.h > converted->h) clipped.h = converted->h - clipped.y;
        for (y = 0; y < clipped.h; y++)
            memcpy((Uint8 *)out->pixels + y * out->pitch,
                   (Uint8 *)converted->pixels + (clipped.y + y) * converted->pitch + clipped.x * 4,
                   (size_t)clipped.w * 4);
        if (hadKey) {
            Uint32 key;
            SDL_GetColorKey(surface, &key);
            // SDL_ConvertSurfaceFormat already turns the color key transparent; keep alpha as is.
            SW_UNUSED(key);
        }
    }
    SDL_FreeSurface(converted);
    return out;
}

/// Notes the visible part of the pixels (for the controller's highlight box), so the pixels
/// themselves needn't be kept in memory once they are in the texture.
static void BKMeasureOpaque(BKGLSurface *gls, SDL_Surface *rgba)
{
    int x, y, minX = rgba->w, minY = rgba->h, maxX = -1, maxY = -1;

    for (y = 0; y < rgba->h; y++) {
        const Uint8 *row = (const Uint8 *)rgba->pixels + y * rgba->pitch;
        for (x = 0; x < rgba->w; x++) {
            if (row[x * 4 + 3] > 96) {      // RGBA32: the bytes are R, G, B, A
                if (x < minX) minX = x;
                if (x > maxX) maxX = x;
                if (y < minY) minY = y;
                if (y > maxY) maxY = y;
            }
        }
    }
    if (maxX >= minX && maxY >= minY) {
        gls->opaqueX = minX;
        gls->opaqueY = minY;
        gls->opaqueW = maxX - minX + 1;
        gls->opaqueH = maxY - minY + 1;
    } else {
        gls->opaqueX = gls->opaqueY = 0;
        gls->opaqueW = rgba->w;
        gls->opaqueH = rgba->h;
    }
}

BKGLSurface *BKCreateNewGLSurface(SDL_Surface *surface, const SDL_Rect *rect)
{
    BKGLSurface *gls;
    SDL_Surface *rgba;

    SW_ASSERT(surface != NULL);
    BK_CHECK_GL();
    rgba = BKCopyToRGBA(surface, rect);
    if (rgba == NULL)
        return NULL;
    gls = calloc(1, sizeof(BKGLSurface));
    if (gls == NULL) {
        SDL_FreeSurface(rgba);
        SDL_SetError("Out of memory");
        return NULL;
    }
    gls->width = rgba->w;
    gls->height = rgba->h;
    gls->needsBlend = BKSurfaceHasAlpha(surface) || SDL_HasColorKey(surface);
    gls->texture = BKUploadTexture(rgba);
    BKMeasureOpaque(gls, rgba);
    SDL_FreeSurface(rgba);
    return gls;
}

BKGLSurface *BKCreateBlankGLSurface(int w, int h, int depth, SDL_bool alpha)
{
    SDL_Surface *s = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);
    BKGLSurface *gls;
    SW_UNUSED(depth);

    if (s == NULL)
        return NULL;
    SDL_FillRect(s, NULL, alpha ? 0 : SDL_MapRGBA(s->format, 0, 0, 0, 255));
    gls = calloc(1, sizeof(BKGLSurface));
    if (gls == NULL) {
        SDL_FreeSurface(s);
        return NULL;
    }
    gls->width = w;
    gls->height = h;
    gls->opaqueW = w;
    gls->opaqueH = h;
    gls->needsBlend = alpha;
    gls->texture = BKUploadTexture(s);
    SDL_FreeSurface(s);
    return gls;
}

BKGLSurface *BKCreateFramebufferGLSurface(int texW, int texH, int width, int height)
{
    BKGLSurface *gls = calloc(1, sizeof(BKGLSurface));

    if (gls == NULL)
        return NULL;
    gls->width = width;
    gls->height = height;
    gls->opaqueW = width;
    gls->opaqueH = height;
    gls->flipV = 1;
    glGenTextures(1, &gls->texture);
    glBindTexture(GL_TEXTURE_2D, gls->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texW > 0 ? texW : 1, texH > 0 ? texH : 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    BK_CHECK_GL();
    return gls;
}

void BKCopyFramebufferToGLSurface(BKGLSurface *gls, int x, int y, int w, int h)
{
    glBindTexture(GL_TEXTURE_2D, gls->texture);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, x, y, w, h);
    BK_CHECK_GL();
}

int BKReloadGLSurface(BKGLSurface *gls, const SDL_Rect *rect)
{
    SW_UNUSED(rect);
    if (gls == NULL || gls->surface == NULL)
        return -1;
    glBindTexture(GL_TEXTURE_2D, gls->texture);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, gls->surface->pitch / 4);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, gls->surface->w, gls->surface->h, GL_RGBA, GL_UNSIGNED_BYTE,
                    gls->surface->pixels);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    return 0;
}

void BKFreeGLSurface(BKGLSurface *gls)
{
    if (gls == NULL)
        return;
    if (gls->texture != 0)
        glDeleteTextures(1, &gls->texture);
    if (gls->surface != NULL && gls->ownsSurface)
        SDL_FreeSurface(gls->surface);
    free(gls);
}

SDL_Surface *BKGetSDLSurface(BKGLSurface *gls)
{
    return gls ? gls->surface : NULL;
}

int DoesSurfaceNeedBlend(BKGLSurface *gls)
{
    return gls ? gls->needsBlend : 0;
}

/*  Emits the two triangles of a quad. Corner order: top-left, top-right, bottom-left,
    bottom-right; v holds (x,y) pairs and t holds (s,t) pairs in the same order. */
static void BKDrawQuad(const float *v, const float *t)
{
    glBegin(GL_TRIANGLES);
    glTexCoord2f(t[0], t[1]); glVertex2f(v[0], v[1]);
    glTexCoord2f(t[2], t[3]); glVertex2f(v[2], v[3]);
    glTexCoord2f(t[4], t[5]); glVertex2f(v[4], v[5]);
    glTexCoord2f(t[2], t[3]); glVertex2f(v[2], v[3]);
    glTexCoord2f(t[4], t[5]); glVertex2f(v[4], v[5]);
    glTexCoord2f(t[6], t[7]); glVertex2f(v[6], v[7]);
    glEnd();
}

int BKBlitGLSurface(BKGLSurface *gls)
{
    SW_ASSERT(gls != NULL);
    return BKBlitClippedGLSurface(gls, 0, 0, 0, 0);
}

int BKBlitClippedGLSurface(BKGLSurface *gls, short left, short top, short right, short bottom)
{
    float w, h, v[8], t[8];

    SW_ASSERT(gls != NULL);
    w = (float)gls->width;
    h = (float)gls->height;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gls->texture);
    v[0] = left;      v[1] = top;
    v[2] = w - right; v[3] = top;
    v[4] = left;      v[5] = h - bottom;
    v[6] = w - right; v[7] = h - bottom;
    t[0] = left / w;          t[1] = top / h;
    t[2] = (w - right) / w;   t[3] = top / h;
    t[4] = left / w;          t[5] = (h - bottom) / h;
    t[6] = (w - right) / w;   t[7] = (h - bottom) / h;
    if (gls->flipV) {
        int i;
        for (i = 1; i < 8; i += 2)
            t[i] = 1.0f - t[i];
    }
    BKDrawQuad(v, t);
    return 0;
}

int BKBlitCustomGLSurface(BKGLSurface *gls, short tlH, short tlV, short trH, short trV,
                          short blH, short blV, short brH, short brV)
{
    float w, h, v[8];
    static const float tUp[8] = { 0, 0, 1, 0, 0, 1, 1, 1 };
    static const float tFlipped[8] = { 0, 1, 1, 1, 0, 0, 1, 0 };
    const float *t;

    SW_ASSERT(gls != NULL);
    w = (float)gls->width;
    h = (float)gls->height;
    t = gls->flipV ? tFlipped : tUp;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gls->texture);
    v[0] = tlH;     v[1] = tlV;
    v[2] = w + trH; v[3] = trV;
    v[4] = blH;     v[5] = h + blV;
    v[6] = w + brH; v[7] = h + brV;
    BKDrawQuad(v, t);
    return 0;
}

const char *BKGetGLErrorMessage(GLenum error)
{
    switch (error) {
        case GL_NO_ERROR:           return "GL_NO_ERROR";
        case GL_INVALID_ENUM:       return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:      return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:  return "GL_INVALID_OPERATION";
        case GL_STACK_OVERFLOW:     return "GL_STACK_OVERFLOW";
        case GL_STACK_UNDERFLOW:    return "GL_STACK_UNDERFLOW";
        case GL_OUT_OF_MEMORY:      return "GL_OUT_OF_MEMORY";
    }
    return "unknown GL error";
}

void BKCheckGLError(const char *file, const char *function, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        fprintf(stderr, "\nGL error detected at %s, %s, %d, error: %s \n", file, function, line,
                BKGetGLErrorMessage(err));
}
