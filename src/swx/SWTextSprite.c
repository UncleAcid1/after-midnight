/*
 * SWTextSprite — text drawn from a bitmap font sprite (one frame per glyph).
 * Reconstructed from the Midnight Mansion HD build (SpriteWorld X fork).
 * See SWCommonHeaders.h for license and provenance.
 *
 * Glyph frames: letters from uppercaseStart (and lowercaseStart if the font has
 * lowercase), digits from digitStart, then specialChars and UTF-16 unicodeChars.
 */

#include "SpriteWorld.h"

static short StrchrUnicode(const char *unicodeChars, short c);

/// Creates a text sprite from a font sprite with at least 26 glyph frames.
SpritePtr CreateBMTextSprite(SpritePtr fontSpriteP, short maxLineWidth)
{
    BMTextSpritePtr t;
    SpritePtr newSpriteP = NULL;
    short i;

    SW_ASSERT(fontSpriteP != NULL);
    SW_ASSERT(fontSpriteP->maxFrames >= 26);
    t = calloc(1, sizeof(BMTextSpriteRec));
    if (t == NULL || SWCloneSprite(fontSpriteP, &newSpriteP, t, sizeof(BMTextSpriteRec)) != kNoError) {
        free(t);
        return NULL;
    }
    SWSetSpriteDrawProc(newSpriteP, BMTextSpriteDrawProc);
    SWSetCurrentFrameIndex(newSpriteP, newSpriteP->curFrameIndex);
    t->text[0] = '\0';
    t->lowercaseStart = 0;
    t->uppercaseStart = 26;
    t->digitStart = 52;
    t->hasLowercase = true;
    t->specialStart = t->digitStart + 10;
    strcpy(t->specialChars, " .,!#$%*()-+/?\"';:");
    t->unicodeStart = t->digitStart + 29;
    t->unicodeChars[0] = 0;
    t->useCharSpacing = false;
    t->maxLineWidth = maxLineWidth;
    for (i = 0; i < kBMTextMaxChars; i++)
        t->charSpacing[i] = 0;
    newSpriteP->userData = kBMTextSpriteMagic;
    return newSpriteP;
}

/*  Clones a text sprite. Note: the fork copies the glyph settings in the wrong direction
    (from the blank clone into the source), clearing the source's spacing, special
    characters and text. That behavior is kept. */
SpritePtr CloneBMTextSprite(SpritePtr textSpriteP, short maxLineWidth)
{
    BMTextSpritePtr src = (BMTextSpritePtr)textSpriteP, t;
    SpritePtr newSpriteP = NULL;
    short i;

    SW_ASSERT(textSpriteP != NULL);
    SW_ASSERT(textSpriteP->userData == kBMTextSpriteMagic);
    t = calloc(1, sizeof(BMTextSpriteRec));
    if (t == NULL || SWCloneSprite(textSpriteP, &newSpriteP, t, sizeof(BMTextSpriteRec)) != kNoError) {
        free(t);
        return NULL;
    }
    SWSetSpriteDrawProc(newSpriteP, BMTextSpriteDrawProc);
    t->lowercaseStart = src->lowercaseStart;
    t->uppercaseStart = src->uppercaseStart;
    t->digitStart = src->digitStart;
    t->specialStart = src->specialStart;
    t->unicodeStart = src->unicodeStart;
    t->hasLowercase = src->hasLowercase;
    t->useCharSpacing = src->useCharSpacing;
    t->maxLineWidth = maxLineWidth;
    for (i = 0; i < kBMTextMaxChars; i++)
        src->charSpacing[i] = t->charSpacing[i];
    strcpy(src->specialChars, t->specialChars);
    strcpy(src->unicodeChars, t->unicodeChars);
    strcpy(src->text, t->text);
    newSpriteP->userData = kBMTextSpriteMagic;
    return newSpriteP;
}

void SetBMTextSpriteIndexes(SpritePtr textSpriteP, short lowercaseStart, short uppercaseStart,
                            short digitStart, SWBoolean hasLowercase)
{
    BMTextSpritePtr t = (BMTextSpritePtr)textSpriteP;

    SW_ASSERT(textSpriteP->userData == kBMTextSpriteMagic);
    t->lowercaseStart = lowercaseStart;
    t->uppercaseStart = uppercaseStart;
    t->digitStart = digitStart;
    t->hasLowercase = hasLowercase;
}

void SetBMTextSpriteSpecialChars(SpritePtr textSpriteP, short specialStart, const char *specialChars)
{
    BMTextSpritePtr t = (BMTextSpritePtr)textSpriteP;

    SW_ASSERT(textSpriteP->userData == kBMTextSpriteMagic);
    SW_ASSERT(strlen(specialChars) < 255);
    strcpy(t->specialChars, specialChars);
    t->specialStart = specialStart;
}

/// unicodeChars is a C string of two-byte character codes (the game passes 2-byte UTF-8
/// sequences); the glyph for unicodeChars[2n..2n+1] is frame unicodeStart + n.
void SetBMTextSpriteUnicodeChars(SpritePtr textSpriteP, short unicodeStart, const char *unicodeChars)
{
    BMTextSpritePtr t = (BMTextSpritePtr)textSpriteP;
    size_t n = strlen(unicodeChars);

    SW_ASSERT(textSpriteP->userData == kBMTextSpriteMagic);
    SW_ASSERT(n < sizeof(t->unicodeChars) - 1);
    if (n > sizeof(t->unicodeChars) - 2)
        n = sizeof(t->unicodeChars) - 2;
    memcpy(t->unicodeChars, unicodeChars, n);
    t->unicodeChars[n] = 0;
    t->unicodeChars[n + 1] = 0;
    t->unicodeStart = unicodeStart;
}

/// Sets the text and the sprite's width to fit it.
void SetBMTextSpriteText(SpritePtr textSpriteP, const char *text)
{
    BMTextSpritePtr t = (BMTextSpritePtr)textSpriteP;
    short left;

    SW_ASSERT(textSpriteP->userData == kBMTextSpriteMagic);
    strncpy(t->text, text, sizeof(t->text) - 1);
    t->text[sizeof(t->text) - 1] = '\0';
    left = textSpriteP->destFrameRect.left;
    textSpriteP->destFrameRect.right = (short)(int)((float)left + GetWidthOfBMText(textSpriteP, text));
}

void SetBMTextSpriteVertOffsetForChars(SpritePtr textSpriteP, const char *chars, float offset)
{
    size_t i;

    for (i = 0; i < strlen(chars); i++)
        SetBMTextSpriteVertOffsetForChar(textSpriteP, chars[i], offset);
}

/// Shifts one special character's glyph vertically (fraction of the glyph height).
void SetBMTextSpriteVertOffsetForChar(SpritePtr textSpriteP, char c, float offset)
{
    BMTextSpritePtr t = (BMTextSpritePtr)textSpriteP;
    char *p;
    short index;

    SW_ASSERT(textSpriteP->userData == kBMTextSpriteMagic);
    p = strchr(t->specialChars, c);
    if (p != NULL) {
        index = (short)(p - t->specialChars) + t->specialStart;
        SW_ASSERT(index < kBMTextMaxChars);
        t->vertOffset[index] = offset;
    }
}

/// Per-glyph advance as a fraction of glyph width, multiplied by scale.
void SetBMTextSpriteCharSpacing(SpritePtr textSpriteP, const float *spacing, short numChars, float scale)
{
    BMTextSpritePtr t = (BMTextSpritePtr)textSpriteP;
    short i;

    SW_ASSERT(textSpriteP->userData == kBMTextSpriteMagic);
    SW_ASSERT(numChars < kBMTextMaxChars);
    t->useCharSpacing = true;
    for (i = 0; i < numChars; i++)
        t->charSpacing[i] = spacing[i] * scale;
    for (; i < kBMTextMaxChars; i++)
        t->charSpacing[i] = scale;
}

char *GetTextSpriteText(SpritePtr textSpriteP)
{
    SW_ASSERT(textSpriteP->userData == kBMTextSpriteMagic);
    return ((BMTextSpritePtr)textSpriteP)->text;
}

/// Frame index of the glyph for text[i], or -1 if the font has none.
static short GlyphIndex(BMTextSpritePtr t, const char *text, short i)
{
    char c = text[i];
    char *p;
    short u;

    if (c >= 'A' && c <= 'Z')
        return (short)(c - 'A') + t->uppercaseStart;
    if (c >= 'a' && c <= 'z')
        return (short)(c - 'a') + (t->hasLowercase ? t->lowercaseStart : t->uppercaseStart);
    if (c >= '0' && c <= '9')
        return (short)(c - '0') + t->digitStart;
    p = strchr(t->specialChars, c);
    if (p != NULL && c != '\0')
        return (short)(p - t->specialChars) + t->specialStart;
    // The fork reads two bytes at the character (big-endian UTF-16 on PowerPC, mixed on Intel).
    u = (short)(((unsigned char)text[i] << 8) | (unsigned char)text[i + 1]);
    u = StrchrUnicode(t->unicodeChars, u);
    if (u == -1)
        return -1;
    return t->unicodeStart + u;
}

float GetWidthOfBMText(SpritePtr textSpriteP, const char *text)
{
    BMTextSpritePtr t = (BMTextSpritePtr)textSpriteP;
    FramePtr glyph0 = textSpriteP->frameArray[0];
    float glyphWidth, width = 0;
    short i, len, index;

    SW_ASSERT(textSpriteP->userData == kBMTextSpriteMagic);
    glyphWidth = textSpriteP->drawData->scaledWidthFactor * (float)(glyph0->frameRect.right - glyph0->frameRect.left);
    len = (short)strlen(text);
    for (i = 0; i < len; i++) {
        index = GlyphIndex(t, text, i);
        if (t->useCharSpacing && index != -1)
            width += t->charSpacing[index] * glyphWidth;
        else
            width += glyphWidth;
    }
    return width;
}

/// Draws one line of text at (horizLoc, vertLoc); returns its width.
float DrawBMLineOfText(SpritePtr textSpriteP, const char *text, float horizLoc, float vertLoc)
{
    BMTextSpritePtr t = (BMTextSpritePtr)textSpriteP;
    FramePtr glyph0 = textSpriteP->frameArray[0];
    float glyphWidth, glyphHeight, advance, width = 0;
    short i, len, index;

    SW_ASSERT(textSpriteP->userData == kBMTextSpriteMagic);
    glyphWidth = textSpriteP->drawData->scaledWidthFactor * (float)(glyph0->frameRect.right - glyph0->frameRect.left);
    glyphHeight = (float)(glyph0->frameRect.bottom - glyph0->frameRect.top);
    if (gSWCurrentSpriteWorld != NULL) {
        horizLoc += (float)gSWCurrentSpriteWorld->visScrollRect.left;
        vertLoc += (float)gSWCurrentSpriteWorld->visScrollRect.top;
    }
    len = (short)strlen(text);
    for (i = 0; i < len; i++) {
        index = GlyphIndex(t, text, i);
        if (index != -1) {
            SW_ASSERT(index < textSpriteP->maxFrames);
            SWStdSpriteDrawProc(textSpriteP, textSpriteP->frameArray[index], horizLoc,
                                t->vertOffset[index] * textSpriteP->drawData->scaledHeightFactor * glyphHeight + vertLoc);
        }
        advance = glyphWidth;
        if (t->useCharSpacing && index != -1)
            advance = t->charSpacing[index] * glyphWidth;
        horizLoc += advance;
        width += advance;
    }
    return width;
}

/*  Draws the text. With a maximum line width, words wrap to new lines; "\r" starts a new
    line and "\r\r" leaves a blank line. The sprite's rect is set to the area drawn. */
void BMTextSpriteDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    BMTextSpritePtr t = (BMTextSpritePtr)srcSpriteP;
    char buffer[1000], line[400], candidate[800];
    char *lines[100];
    unsigned char blankLineAfter[100];
    short numLines = 0, lineIndex, lineHeight;
    short top = (short)(int)vertLoc, right = (short)(int)horizLoc, bottom = top;
    float spacing;
    char *p, *word;
    SWBoolean done = false, lineDone;

    SW_UNUSED(curFrameP);
    SW_ASSERT(srcSpriteP->userData == kBMTextSpriteMagic);
    if (t->maxLineWidth < 1) {
        DrawBMLineOfText(srcSpriteP, t->text, horizLoc, vertLoc);
        return;
    }

    SWCopyCString(t->text, buffer);
    spacing = SWGetSpriteHeight(srcSpriteP) + 2.0f;

        // Note where "\r\r" (blank line) occurs.
    p = buffer;
    while ((p = strchr(p, '\r')) != NULL && numLines < 99) {
        p++;
        if (*p == '\r') {
            blankLineAfter[numLines++] = 1;
            p++;
        } else {
            blankLineAfter[numLines++] = 0;
        }
    }
    numLines = 0;
    for (p = strtok(buffer, "\r"); p != NULL && numLines < 100; p = strtok(NULL, "\r"))
        lines[numLines++] = p;
    if (numLines == 0)
        return;

    lineIndex = 0;
    word = strtok(lines[0], " ");
    while (!done) {
        lineHeight = (short)(int)spacing;
        line[0] = '\0';
        if (word != NULL)
            snprintf(line, sizeof(line), "%s", word);
        lineDone = false;
        while (!lineDone) {
            word = strtok(NULL, " ");
            while (word == NULL) {
                lineIndex++;
                if (lineIndex >= numLines) {
                    done = true;
                    goto drawLine;
                }
                word = strtok(lines[lineIndex], " ");
                if (blankLineAfter[lineIndex - 1])
                    lineHeight = (short)(int)((float)lineHeight + spacing);
                lineDone = true;
                if (word != NULL)
                    goto drawLine;
            }
            snprintf(candidate, sizeof(candidate), "%s %s", line, word);
            if ((short)(int)GetWidthOfBMText(srcSpriteP, candidate) < t->maxLineWidth) {
                size_t n = strlen(candidate);       // (a line that fits is far shorter than line[])
                if (n >= sizeof(line))
                    n = sizeof(line) - 1;
                memcpy(line, candidate, n);
                line[n] = '\0';
            } else
                lineDone = true;
        }
drawLine:
        if (line[0] == '\0')
            break;
        {
            float w = DrawBMLineOfText(srcSpriteP, line, horizLoc, vertLoc);
            vertLoc += (float)lineHeight;
            if ((float)right < horizLoc + w)
                right = (short)(int)(horizLoc + w);
            if ((float)bottom < vertLoc)
                bottom = (short)(int)vertLoc;
        }
    }
    srcSpriteP->destFrameRect.top = top;
    srcSpriteP->destFrameRect.left = (short)(int)horizLoc;
    srcSpriteP->destFrameRect.bottom = bottom;
    srcSpriteP->destFrameRect.right = right;
}

static short StrchrUnicode(const char *unicodeChars, short c)
{
    short i;
    const unsigned char *u = (const unsigned char *)unicodeChars;

    for (i = 0; i < 128; i++) {
        short code = (short)((u[i * 2] << 8) | u[i * 2 + 1]);
        if (code == 0)
            break;
        if (code == c)
            return i;
    }
    return -1;
}
