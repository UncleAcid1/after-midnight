/*
 * MMFont — TrueType text drawing with stb_truetype, replacing FTGL's pixmap fonts.
 *
 * Text is MacRoman (the game's TEXT resources and string literals), drawn in the
 * current GL modelview units with the baseline at y and y pointing down.
 */
#ifndef MMFONT_H
#define MMFONT_H

typedef struct MMFont MMFont;

/// Loads a .ttf/.ttc file; NULL if it can't be read.
MMFont *MMFontLoad(const char *path);
void    MMFontDispose(MMFont *font);

/// Sets the pixel size used by MMFontRenderWidth/MMFontRender (FTFont_FaceSize).
void    MMFontFaceSize(MMFont *font, int pixelSize);
int     MMFontGetFaceSize(MMFont *font);

/// Advance width of the text at the current face size, in pixels.
float   MMFontRenderWidth(MMFont *font, const char *text);

/// Draws text at (x, y) = left end of the baseline. unitScale converts pixels to
/// modelview units (glyphs are rasterized at faceSize pixels, drawn faceSize*unitScale big).
void    MMFontRender(MMFont *font, const char *text, float x, float y, float unitScale);

#endif
