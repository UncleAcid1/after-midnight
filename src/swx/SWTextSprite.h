/* SWTextSprite — sprites that draw text with a bitmap font (one frame per glyph).
 * Reconstructed from the Midnight Mansion HD build (SpriteWorld X fork). */
#ifndef __SWTEXTSPRITE__
#define __SWTEXTSPRITE__

#include "SWCommonHeaders.h"

#ifdef __cplusplus
extern "C" {
#endif

#define kBMTextSpriteMagic      0x7edf
#define kBMTextMaxChars         500

typedef struct BMTextSpriteRec {
    SpriteRec       sprite;
    char            text[1000];
    SWBoolean       hasLowercase;       ///< font has separate lowercase glyphs
    short           maxLineWidth;       ///< wrap width in pixels, <= 0 for a single line
    short           lowercaseStart;     ///< frame index of 'a'
    short           uppercaseStart;     ///< frame index of 'A'
    short           digitStart;         ///< frame index of '0'
    short           specialStart;       ///< frame index of specialChars[0]
    char            specialChars[256];
    short           unicodeStart;       ///< frame index of unicodeChars[0]
    char            unicodeChars[258];  ///< UTF-16 code units, 0-terminated
    float           charSpacing[kBMTextMaxChars];   ///< advance as a fraction of the glyph width
    float           vertOffset[kBMTextMaxChars];    ///< offset as a fraction of the glyph height
    SWBoolean       useCharSpacing;
} BMTextSpriteRec, *BMTextSpritePtr;

SpritePtr CreateBMTextSprite(SpritePtr fontSpriteP, short maxLineWidth);
SpritePtr CloneBMTextSprite(SpritePtr textSpriteP, short maxLineWidth);
void      SetBMTextSpriteIndexes(SpritePtr textSpriteP, short lowercaseStart, short uppercaseStart,
                                 short digitStart, SWBoolean hasLowercase);
void      SetBMTextSpriteSpecialChars(SpritePtr textSpriteP, short specialStart, const char *specialChars);
void      SetBMTextSpriteUnicodeChars(SpritePtr textSpriteP, short unicodeStart, const char *unicodeChars);
void      SetBMTextSpriteText(SpritePtr textSpriteP, const char *text);
void      SetBMTextSpriteVertOffsetForChars(SpritePtr textSpriteP, const char *chars, float offset);
void      SetBMTextSpriteVertOffsetForChar(SpritePtr textSpriteP, char c, float offset);
void      SetBMTextSpriteCharSpacing(SpritePtr textSpriteP, const float *spacing, short numChars, float scale);
char     *GetTextSpriteText(SpritePtr textSpriteP);
void      BMTextSpriteDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc);
float     GetWidthOfBMText(SpritePtr textSpriteP, const char *text);
float     DrawBMLineOfText(SpritePtr textSpriteP, const char *text, float horizLoc, float vertLoc);

#ifdef __cplusplus
}
#endif

#endif
