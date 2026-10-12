/*
 * MMFont.c — TrueType text for the game's text sprites (replaces FTGL).
 *
 * Each face size gets its own glyph atlas (an alpha texture) holding the 224
 * printable MacRoman characters.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BKGL.h"
#include "MMFont.h"

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "stb_truetype.h"

#define kFirstChar      32
#define kNumChars       224         // 32..255
#define kMaxSizes       24

typedef struct FontSizeCache {
    int              pixelSize;
    GLuint           texture;
    int              atlasW, atlasH;
    stbtt_packedchar chars[kNumChars];
} FontSizeCache;

struct MMFont {
    unsigned char  *data;
    stbtt_fontinfo  info;
    int             faceSize;
    FontSizeCache   sizes[kMaxSizes];
    int             numSizes;
    int             nextEvict;
};

/// MacRoman 0x80..0xFF to Unicode.
static const unsigned short kMacRomanHigh[128] = {
    0x00C4, 0x00C5, 0x00C7, 0x00C9, 0x00D1, 0x00D6, 0x00DC, 0x00E1, 0x00E0, 0x00E2, 0x00E4, 0x00E3, 0x00E5, 0x00E7, 0x00E9, 0x00E8,
    0x00EA, 0x00EB, 0x00ED, 0x00EC, 0x00EE, 0x00EF, 0x00F1, 0x00F3, 0x00F2, 0x00F4, 0x00F6, 0x00F5, 0x00FA, 0x00F9, 0x00FB, 0x00FC,
    0x2020, 0x00B0, 0x00A2, 0x00A3, 0x00A7, 0x2022, 0x00B6, 0x00DF, 0x00AE, 0x00A9, 0x2122, 0x00B4, 0x00A8, 0x2260, 0x00C6, 0x00D8,
    0x221E, 0x00B1, 0x2264, 0x2265, 0x00A5, 0x00B5, 0x2202, 0x2211, 0x220F, 0x03C0, 0x222B, 0x00AA, 0x00BA, 0x03A9, 0x00E6, 0x00F8,
    0x00BF, 0x00A1, 0x00AC, 0x221A, 0x0192, 0x2248, 0x2206, 0x00AB, 0x00BB, 0x2026, 0x00A0, 0x00C0, 0x00C3, 0x00D5, 0x0152, 0x0153,
    0x2013, 0x2014, 0x201C, 0x201D, 0x2018, 0x2019, 0x00F7, 0x25CA, 0x00FF, 0x0178, 0x2044, 0x20AC, 0x2039, 0x203A, 0xFB01, 0xFB02,
    0x2021, 0x00B7, 0x201A, 0x201E, 0x2030, 0x00C2, 0x00CA, 0x00C1, 0x00CB, 0x00C8, 0x00CD, 0x00CE, 0x00CF, 0x00CC, 0x00D3, 0x00D4,
    0xF8FF, 0x00D2, 0x00DA, 0x00DB, 0x00D9, 0x0131, 0x02C6, 0x02DC, 0x00AF, 0x02D8, 0x02D9, 0x02DA, 0x00B8, 0x02DD, 0x02DB, 0x02C7
};

static int MacRomanToUnicode(unsigned char c)
{
    return c < 0x80 ? c : kMacRomanHigh[c - 0x80];
}

MMFont *MMFontLoad(const char *path)
{
    MMFont *font;
    FILE *f = fopen(path, "rb");
    long size;

    if (f == NULL)
        return NULL;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);

    font = (MMFont *)calloc(1, sizeof(MMFont));
    if (font == NULL || size <= 0) {
        free(font);
        fclose(f);
        return NULL;
    }
    font->data = (unsigned char *)malloc((size_t)size);
    if (font->data == NULL || fread(font->data, 1, (size_t)size, f) != (size_t)size ||
        !stbtt_InitFont(&font->info, font->data, stbtt_GetFontOffsetForIndex(font->data, 0))) {
        free(font->data);
        free(font);
        fclose(f);
        return NULL;
    }
    fclose(f);
    font->faceSize = 12;
    return font;
}

void MMFontDispose(MMFont *font)
{
    int i;
    if (font == NULL)
        return;
    for (i = 0; i < font->numSizes; i++) {
        if (font->sizes[i].texture != 0)
            glDeleteTextures(1, &font->sizes[i].texture);
    }
    free(font->data);
    free(font);
}

void MMFontFaceSize(MMFont *font, int pixelSize)
{
    if (font != NULL)
        font->faceSize = pixelSize > 0 ? pixelSize : 1;
}

int MMFontGetFaceSize(MMFont *font)
{
    return font != NULL ? font->faceSize : 0;
}

static FontSizeCache *GetSizeCache(MMFont *font, int pixelSize)
{
    FontSizeCache *cache;
    stbtt_pack_context pc;
    stbtt_pack_range range;
    int codepoints[kNumChars];
    unsigned char *pixels;
    int i, side;

    for (i = 0; i < font->numSizes; i++) {
        if (font->sizes[i].pixelSize == pixelSize)
            return &font->sizes[i];
    }

    if (font->numSizes < kMaxSizes) {
        cache = &font->sizes[font->numSizes++];
    } else {
        cache = &font->sizes[font->nextEvict];
        font->nextEvict = (font->nextEvict + 1) % kMaxSizes;
        if (cache->texture != 0)
            glDeleteTextures(1, &cache->texture);
    }
    memset(cache, 0, sizeof(*cache));
    cache->pixelSize = pixelSize;

    for (i = 0; i < kNumChars; i++)
        codepoints[i] = MacRomanToUnicode((unsigned char)(kFirstChar + i));

    range.font_size = STBTT_POINT_SIZE((float)pixelSize);     // em size, like FreeType's FaceSize
    range.first_unicode_codepoint_in_range = 0;
    range.array_of_unicode_codepoints = codepoints;
    range.num_chars = kNumChars;
    range.chardata_for_range = cache->chars;

        // grow the atlas until everything fits
    for (side = 128; side <= 4096; side *= 2) {
        pixels = (unsigned char *)calloc(1, (size_t)side * side);
        if (pixels == NULL)
            return NULL;
        if (stbtt_PackBegin(&pc, pixels, side, side, 0, 1, NULL)) {
            int ok;
            stbtt_PackSetOversampling(&pc, 1, 1);
            ok = stbtt_PackFontRanges(&pc, font->data, 0, &range, 1);
            stbtt_PackEnd(&pc);
            if (ok) {
                cache->atlasW = side;
                cache->atlasH = side;
                glGenTextures(1, &cache->texture);
                glBindTexture(GL_TEXTURE_2D, cache->texture);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, side, side, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
                free(pixels);
                return cache;
            }
        }
        free(pixels);
    }
    return NULL;
}

float MMFontRenderWidth(MMFont *font, const char *text)
{
    float scale, width = 0.0f;
    int advance, lsb, prev = 0, cp;
    const unsigned char *p;

    if (font == NULL || text == NULL)
        return 0.0f;
    scale = stbtt_ScaleForMappingEmToPixels(&font->info, (float)font->faceSize);
    for (p = (const unsigned char *)text; *p != '\0'; p++) {
        cp = MacRomanToUnicode(*p);
        if (prev != 0)
            width += scale * stbtt_GetCodepointKernAdvance(&font->info, prev, cp);
        stbtt_GetCodepointHMetrics(&font->info, cp, &advance, &lsb);
        width += scale * advance;
        prev = cp;
    }
    return width;
}

void MMFontRender(MMFont *font, const char *text, float x, float y, float unitScale)
{
    FontSizeCache *cache;
    const unsigned char *p;
    float scale, penX = 0.0f, penY = 0.0f;
    int prev = 0, cp;

    if (font == NULL || text == NULL || *text == '\0')
        return;
    cache = GetSizeCache(font, font->faceSize);
    if (cache == NULL)
        return;
    scale = stbtt_ScaleForMappingEmToPixels(&font->info, (float)font->faceSize);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, cache->texture);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    for (p = (const unsigned char *)text; *p != '\0'; p++) {
        stbtt_aligned_quad q;
        cp = MacRomanToUnicode(*p);
        if (*p < kFirstChar) {
            prev = 0;
            continue;
        }
        if (prev != 0)
            penX += scale * stbtt_GetCodepointKernAdvance(&font->info, prev, cp);
        stbtt_GetPackedQuad(cache->chars, cache->atlasW, cache->atlasH, *p - kFirstChar, &penX, &penY, &q, 0);
        glTexCoord2f(q.s0, q.t0); glVertex2f(x + q.x0 * unitScale, y + q.y0 * unitScale);
        glTexCoord2f(q.s1, q.t0); glVertex2f(x + q.x1 * unitScale, y + q.y0 * unitScale);
        glTexCoord2f(q.s1, q.t1); glVertex2f(x + q.x1 * unitScale, y + q.y1 * unitScale);
        glTexCoord2f(q.s0, q.t1); glVertex2f(x + q.x0 * unitScale, y + q.y1 * unitScale);
        prev = cp;
    }
    glEnd();
}
