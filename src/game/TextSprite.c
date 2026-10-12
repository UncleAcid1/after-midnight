/*
 * TextSprite.c — sprites that draw a line of TrueType text.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources - My Utils/TextSprite.c").
 * The original drew FTGL pixmap fonts; MMFont.c replaces FTGL.
 */
#include "MMGame.h"
#include "BKGL.h"
#include "MMFont.h"

#define kMaxFTFonts     100
#define kFontNameLen    1024     // a font file's whole path (the original kept 50 characters, so every
                                    // font in the game's folder matched the first one loaded)

static short   gNumFTFontsLoaded = 0;
static char    gFTFontNames[kMaxFTFonts][kFontNameLen];
static MMFont *gFTFonts[kMaxFTFonts];

static MMFont *LoadSystemFont(const char *fontName)
{
    char path[400];
#ifdef __APPLE__
    snprintf(path, sizeof(path), "%s%s", "/Library/Fonts/", fontName);
#else
    snprintf(path, sizeof(path), "%s%s", "/usr/share/fonts/truetype/", fontName);
#endif
    return LoadFontUsingPath(path);
}

/// True if b is a prefix of a (the original's semantics).
static short FindFTFont(const char *path)
{
    short i;
    for (i = 0; i < gNumFTFontsLoaded; i++) {
        if (strcmp(path, gFTFontNames[i]) == 0)
            return i;
    }
    return -1;
}

/// Loads (once) a font from path + .ttf/.ttc/.dfont, or path itself.
MMFont *LoadFontUsingPath(const char *path)
{
    static const char *kExtensions[] = { ".ttf", ".ttc", ".TTF", ".dfont", "" };
    char fullPath[400];
    MMFont *font = NULL;
    short index;
    size_t i;

    index = FindFTFont(path);
    if (index != -1)
        return gFTFonts[index];
    if (gNumFTFontsLoaded >= kMaxFTFonts)
        return NULL;

    for (i = 0; i < sizeof(kExtensions) / sizeof(kExtensions[0]) && font == NULL; i++) {
        snprintf(fullPath, sizeof(fullPath), "%s%s", path, kExtensions[i]);
        font = MMFontLoad(fullPath);
    }
    if (font == NULL)
        return NULL;

    snprintf(gFTFontNames[gNumFTFontsLoaded], kFontNameLen, "%s", path);
    gFTFonts[gNumFTFontsLoaded++] = font;
    return font;
}

MMFont *LoadFontSafe(const char *fontName)
{
    MMFont *font = LoadFontUsingPath(GetFontsFolderPath(fontName));
    if (font == NULL)
        font = LoadSystemFont(fontName);
    if (font == NULL)
        font = LoadSystemFont("Arial");
    if (font == NULL)
        font = LoadSystemFont("Georgia");
    if (font == NULL)
        font = LoadSystemFont("Verdana");
    if (font == NULL)
        font = LoadSystemFont("Comic Sans MS");
        // last resort, so text never silently disappears
    if (font == NULL)
        font = LoadFontUsingPath(GetFontsFolderPath("Action Man"));
    return font;
}

Boolean FontExists(const char *fontName)
{
    return LoadFontUsingPath(fontName) != NULL || LoadSystemFont(fontName) != NULL;
}

SWError SWCreateTextSprite(SpritePtr *newSpritePP, const char *text, const char *fontName, short fontSize,
                           short style, const unsigned char rgb[3], Boolean dropShadow)
{
    TextSpritePtr textP;
    SWError err;

    textP = (TextSpritePtr)calloc(1, sizeof(TextSpriteRec));
    if (textP == NULL)
        return kMemoryAllocationError;

    err = SWCreateSprite(newSpritePP, textP, 1);
    if (err != kNoError) {
        free(textP);
        return err;
    }
    textP->sprite.spriteRecSize = sizeof(TextSpriteRec);
    SetTextSpriteText(&textP->sprite, text);
    SWSetSpriteDrawProc(*newSpritePP, SWTextSpriteDrawProc);

    textP->font = LoadFontSafe(fontName);
    if (textP->font == NULL) {
        SWDisposeSprite(newSpritePP);
        return -1;
    }
    textP->fontSize = fontSize;
    textP->style = style;
    textP->color[0] = rgb[0];
    textP->color[1] = rgb[1];
    textP->color[2] = rgb[2];
    textP->dropShadow = dropShadow;
    textP->useRect = false;
    (*newSpritePP)->destFrameRect.bottom += fontSize;
    (*newSpritePP)->destFrameRect.right += (short)(strlen(text) * fontSize);
    (*newSpritePP)->userData = kTextSpriteSignature;
    return kNoError;
}

void SetTextSpriteText(SpritePtr spriteP, const char *text)
{
    TextSpritePtr textP = (TextSpritePtr)spriteP;
    short i;
    for (i = 0; i < kTextSpriteMaxChars; i++) {
        textP->text[i] = text[i];
        if (text[i] == '\0')
            break;
    }
    textP->text[kTextSpriteMaxChars - 1] = '\0';
}

/// Draw the text inside rectP, justified (0 left, 1 right, 2 center); alignTop puts
/// the baseline at the rect's top instead of its bottom.
void SetTextSpriteRect(SpritePtr spriteP, const SWRect *rectP, short justification, Boolean alignTop)
{
    TextSpritePtr textP = (TextSpritePtr)spriteP;
    textP->useRect = true;
    textP->rect = *rectP;
    textP->rectWidth = rectP->right - rectP->left;
    textP->justification = justification;
    textP->alignTop = alignTop;
    spriteP->destFrameRect = *rectP;
    SWSetSpriteLocation(spriteP, rectP->left, rectP->top);
}

void SWTextSpriteDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    TextSpritePtr textP = (TextSpritePtr)srcSpriteP;
    DrawDataPtr drawData = srcSpriteP->drawData;
    SpriteWorldPtr spriteWorldP;
    float unitScale, x, textWidth;
    short top, left, baseline, bottom;
    int pixelSize;
    unsigned char alpha;

    (void)curFrameP; (void)horizLoc; (void)vertLoc;
    SW_ASSERT(srcSpriteP->userData == kTextSpriteSignature);
    if (drawData->translucencyLevel == 0)
        return;

    spriteWorldP = srcSpriteP->parentSpriteLayerP != NULL ? srcSpriteP->parentSpriteLayerP->parentSpriteWorldP
                                                          : gSWCurrentSpriteWorld;
    if (spriteWorldP == NULL)
        spriteWorldP = gSWCurrentSpriteWorld;
    SW_ASSERT(gSWCurrentSpriteWorld != NULL);

        // Glyphs are rasterized at the on-screen pixel size (the original scaled its
        // pixmap font by the window scaling factor) and drawn in design units.
    unitScale = 1.0f / gSWCurrentSpriteWorld->designScaleH;
    pixelSize = (int)((float)textP->fontSize * gSWCurrentSpriteWorld->designScaleH + 0.5f);
    if (pixelSize < 1)
        pixelSize = 1;

    glPushMatrix();
    glLoadIdentity();
    glScalef(gSWCurrentSpriteWorld->designScaleH, gSWCurrentSpriteWorld->designScaleV, 1.0f);

    MMFontFaceSize(textP->font, textP->fontSize);
    if (!textP->useRect) {
        top = srcSpriteP->destFrameRect.top;
        left = srcSpriteP->destFrameRect.left;
        bottom = srcSpriteP->destFrameRect.bottom - textP->fontSize;
    } else {
        top = textP->rect.top;
        left = textP->rect.left;
        bottom = textP->rect.bottom;
        textWidth = MMFontRenderWidth(textP->font, textP->text);
        if (textP->justification == 1)
            left = (short)(int)(((float)textP->rectWidth - textWidth) + (float)left);
        else if (textP->justification == 2)
            left = (short)(int)((((float)textP->rectWidth + textWidth) / 2.0f - textWidth) + (float)left);
    }
    baseline = textP->alignTop ? top : bottom;
    x = (float)left;

    MMFontFaceSize(textP->font, pixelSize);
    alpha = (unsigned char)((drawData->translucencyLevel >> 8) & 0xff);

    if (textP->dropShadow) {
        float shadow = (float)textP->fontSize / 9.0f;
        glColor4ub(0, 0, 0, alpha);
        MMFontRender(textP->font, textP->text, x + shadow, (float)baseline + shadow, unitScale);
    }
    glColor4ub(textP->color[0], textP->color[1], textP->color[2], alpha);
    MMFontRender(textP->font, textP->text, x, (float)baseline, unitScale);
        // fake bold: draw again slightly offset
    if (textP->style == 1 || textP->style == 2)
        MMFontRender(textP->font, textP->text, x - 0.5f, (float)baseline - 0.5f, unitScale);
    if (textP->style == 2)
        MMFontRender(textP->font, textP->text, x - 1.0f, (float)baseline - 1.0f, unitScale);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

int GetTextSpriteWidth(SpritePtr spriteP)
{
    TextSpritePtr textP = (TextSpritePtr)spriteP;
    SW_ASSERT(spriteP->userData == kTextSpriteSignature);
    if (textP->font == NULL)
        return -1;
    MMFontFaceSize(textP->font, textP->fontSize);
    return (short)(int)MMFontRenderWidth(textP->font, textP->text);
}

int GetTextSpriteRenderWidth(const char *fontName, short fontSize, const char *text)
{
    MMFont *font = LoadFontSafe(fontName);
    if (font == NULL)
        return -1;
    MMFontFaceSize(font, fontSize);
    return (short)(int)MMFontRenderWidth(font, text);
}
