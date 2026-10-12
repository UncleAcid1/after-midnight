/*
 * MMExtrasUI.c — screens and widgets added by this version: the Fun and Extras panes of the
 * Options dialog (with the description box that fades in under the hovered option) and
 * plain coloured boxes drawn behind added text.
 */

#include "MMGame.h"
#include "MMProtos.h"
#include "BKGL.h"

// ---------------------------------------------------------------------------
// Solid boxes

/// Fills the sprite's rect with its colour. (SWSolidColorSpriteDrawProc would apply the
/// sprite's scale twice: once in the matrix and again through the rect.)
static void SolidBoxDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    SpriteWorldPtr w = gSWCurrentSpriteWorld;
    DrawDataPtr d = srcSpriteP->drawData;
    float width = srcSpriteP->destFrameRect.right - srcSpriteP->destFrameRect.left;
    float height = srcSpriteP->destFrameRect.bottom - srcSpriteP->destFrameRect.top;

    (void)curFrameP;
    if (d->translucencyLevel == 0)
        return;
    glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    if (w != NULL)
        glScalef(w->designScaleH, w->designScaleV, 1.0f);
    glTranslatef(horizLoc, vertLoc, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4us(d->colorRed, d->colorGreen, d->colorBlue, (GLushort)(d->translucencyLevel & 0xFFFF));
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(width, 0);
    glVertex2f(width, height);
    glVertex2f(0, height);
    glEnd();
    glPopMatrix();
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glColor4f(1, 1, 1, 1);
}

/// A sprite that draws as a rectangle in its colour.
SpritePtr NewSolidBoxSprite(SpriteLayerPtr layerP, const SWRect *rectP, unsigned short red, unsigned short green,
                            unsigned short blue, unsigned long alpha)
{
    static SpritePtr masterP = NULL;
    SDL_Surface *surface;
    SpritePtr boxP = NULL;

    if (masterP == NULL) {
        surface = SDL_CreateRGBSurfaceWithFormat(0, 4, 4, 32, SDL_PIXELFORMAT_ABGR8888);
        if (surface == NULL)
            return NULL;
        SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 255, 255, 255, 255));
        if (SWCreateSpriteFromSurfaceXY(&masterP, NULL, surface, 4, 4, 0, 0, false, 1, false) != kNoError) {
            masterP = NULL;
            return NULL;
        }
    }
    if (SWCloneSprite(masterP, &boxP, NULL, 0) != kNoError)
        return NULL;
    SWSetSpriteDrawProc(boxP, SolidBoxDrawProc);
    SWSetSpriteColor(boxP, red, green, blue);
    SWSetSpriteTranslucencyLevel(boxP, alpha);
    if (layerP != NULL)
        SWAddSprite(layerP, boxP);
    SetSolidBoxRect(boxP, rectP);
    return boxP;
}

void SetSolidBoxRect(SpritePtr boxP, const SWRect *rectP)
{
    SWSetSpriteScaledSize(boxP, rectP->right - rectP->left, rectP->bottom - rectP->top);
    SWSetSpriteLocation(boxP, rectP->left, rectP->top);
}

/// A bordered panel: a light frame with a dark inside. Returns the inside sprite; the frame is
/// stored in *frameP (if not NULL).
SpritePtr NewFramedBox(SpriteLayerPtr layerP, const SWRect *rectP, short border, unsigned long alpha,
                       SpritePtr *frameP)
{
    SWRect inner = *rectP;
    SpritePtr outerP, innerP;

    outerP = NewSolidBoxSprite(layerP, rectP, 0xd000, 0xd000, 0x4800, alpha);
    SWInsetRect(&inner, border, border);
    innerP = NewSolidBoxSprite(layerP, &inner, 0x1400, 0x0c00, 0x0600, alpha);
    if (frameP != NULL)
        *frameP = outerP;
    return innerP;
}

// ---------------------------------------------------------------------------
// Options dialog: Fun and Extras panes

#define kExtrasFirstButtonFun       0x40
#define kExtrasFirstButtonExtras    0x60
#define kDescriptionAlpha           0xd000UL

typedef struct DescriptionBox {
    SpritePtr frameP, boxP, textP;
    int shown;              ///< option whose text is in the box, -1 none
    long alpha;             ///< 0..0x10000
} DescriptionBox;

static DescriptionBox gDescriptionBoxes[2];

short GetExtrasOptionButtonIndex(int tab, int option)
{
    return (short)((tab == kExtrasTabFun ? kExtrasFirstButtonFun : kExtrasFirstButtonExtras) + option);
}

/// The checkboxes of a tab (two columns when there are many) and the description box.
void AddExtrasItemsToOptionsDialog(SpriteLayerPtr layerP, SWRect *dialogRectP, int tab)
{
    const ExtrasOption *options;
    float scale = gSpriteWorldP->designWidth / 1280.0f;
    float checkboxScale = scale * 0.92f;
    int count = GetExtrasOptions(tab, &options), i, perColumn;
    short h, v, top = (short)(scale * 150.0f + dialogRectP->top);
    DescriptionBox *d = &gDescriptionBoxes[tab];
    SpritePtr checkboxP, textP;
    SWRect rect;

    AddOptionsPaneTitle(layerP, dialogRectP, tab == kExtrasTabFun ? "Fun" : "Extras", scale);
    perColumn = count > 6 ? (count + 1) / 2 : count;
    for (i = 0; i < count; i++) {
        h = (short)(dialogRectP->left + scale * (i < perColumn ? 60.0f : 620.0f));
        v = (short)(top + scale * 50.0f * (i % perColumn));
        checkboxP = AddCheckboxToOptionsDialog(options[i].label, GetExtrasOptionButtonIndex(tab, i),
                                               options[i].valueP, layerP, checkboxScale, h, v);
            // two computers share the host's Fun settings for the whole game
        if (checkboxP != NULL && tab == kExtrasTabFun && NetGameActive())
            SetButtonDimmedMode(checkboxP, true);
    }

        // the description box, under the options and left of the OK / Cancel buttons
    rect.left = (short)(dialogRectP->left + scale * 50.0f);
    rect.right = (short)(dialogRectP->right - scale * 50.0f);
    rect.top = (short)(top + scale * (50.0f * perColumn + 12.0f));
    rect.bottom = (short)(rect.top + scale * 82.0f);
    d->boxP = NewFramedBox(layerP, &rect, (short)(scale * 3.0f + 0.5f), 0, &d->frameP);
    textP = NewBitmapTextSprite((short)(rect.right - rect.left - scale * 36.0f));
    if (textP != NULL) {
        SWSetSpriteScaledFactor(textP, scale * 0.42f, scale * 0.42f);
        SWSetSpriteColor(textP, 0xffff, 0xffff, 0xb000);
        SetBMTextSpriteText(textP, "");
        SWAddSprite(layerP, textP);
        SWSetSpriteLocation(textP, rect.left + scale * 18.0f, rect.top + scale * 14.0f);
        SWSetSpriteTranslucencyLevel(textP, 0);
    }
    d->textP = textP;
    d->shown = -1;
    d->alpha = 0;
}

/// The option under the mouse (or the controller highlight) on a pane, or -1.
static int HoveredOption(SpriteLayerPtr layerP, int tab)
{
    SpritePtr spriteP;
    ButtonSpritePtr buttonP;
    short first = GetExtrasOptionButtonIndex(tab, 0);

    for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP)) {
        if (spriteP->userData != kButtonSpriteSignature)
            continue;
        buttonP = (ButtonSpritePtr)spriteP;
        if (buttonP->state != kButtonNormal && buttonP->buttonIndex >= first && buttonP->buttonIndex < first + 32)
            return buttonP->buttonIndex - first;
    }
    return -1;
}

/// Called every frame while a pane is visible: the box fades out, takes the new text, and
/// fades back in when the hovered option changes.
void UpdateExtrasOptionDescription(SpriteLayerPtr layerP, int tab)
{
    DescriptionBox *d = &gDescriptionBoxes[tab];
    const ExtrasOption *options;
    int count = GetExtrasOptions(tab, &options), want = HoveredOption(layerP, tab);
    const long kStep = 0x10000 / 8;
    unsigned long level;

    if (d->textP == NULL || d->boxP == NULL)
        return;
    if (want >= count)
        want = -1;
    if (want != d->shown) {
        d->alpha -= kStep;
        if (d->alpha <= 0) {
            d->alpha = 0;
            d->shown = want;
            SetBMTextSpriteText(d->textP, want >= 0 ? options[want].description : "");
        }
    }
    else if (d->shown >= 0 && d->alpha < 0x10000) {
        d->alpha += kStep;
        if (d->alpha > 0x10000)
            d->alpha = 0x10000;
    }
    level = (unsigned long)d->alpha;
    SWSetSpriteTranslucencyLevel(d->textP, level >= 0x10000 ? 0xffff : level);
    SWSetSpriteTranslucencyLevel(d->boxP, (unsigned long)(level * kDescriptionAlpha / 0x10000));
    SWSetSpriteTranslucencyLevel(d->frameP, (unsigned long)(level * kDescriptionAlpha / 0x10000));
}

// ---------------------------------------------------------------------------
// Drawn over every frame: the controller's highlight box

/// The part of a frame that isn't transparent, in frame pixels (cached per frame).
static SDL_Rect OpaqueBoundsOfFrame(FramePtr frameP)
{
    static FramePtr cachedFrames[64];
    static SDL_Rect cachedBounds[64];
    static int next = 0;
    SDL_Surface *s;
    SDL_Rect r = { 0, 0, 0, 0 };
    int i, x, y, minX, minY, maxX = -1, maxY = -1;

    if (frameP == NULL || frameP->glSurfaceP == NULL || frameP->glSurfaceP->surface == NULL)
        return r;
    for (i = 0; i < 64; i++)
        if (cachedFrames[i] == frameP)
            return cachedBounds[i];
    s = frameP->glSurfaceP->surface;
    minX = s->w;
    minY = s->h;
    if (SDL_LockSurface(s) == 0) {
        for (y = 0; y < s->h; y++) {
            const Uint8 *row = (const Uint8 *)s->pixels + y * s->pitch;
            for (x = 0; x < s->w; x++) {
                if (row[x * 4 + 3] > 96) {      // ABGR8888 bytes are R, G, B, A
                    if (x < minX) minX = x;
                    if (x > maxX) maxX = x;
                    if (y < minY) minY = y;
                    if (y > maxY) maxY = y;
                }
            }
        }
        SDL_UnlockSurface(s);
    }
    if (maxX >= minX && maxY >= minY) {
        r.x = minX;
        r.y = minY;
        r.w = maxX - minX + 1;
        r.h = maxY - minY + 1;
    } else {
        r.w = s->w;
        r.h = s->h;
    }
    cachedFrames[next] = frameP;
    cachedBounds[next] = r;
    next = (next + 1) % 64;
    return r;
}

/// The rect to box: the visible part of the button's image, plus a checkbox's label.
static void FocusRect(SpritePtr spriteP, float *left, float *top, float *right, float *bottom)
{
    ButtonSpritePtr buttonP = (ButtonSpritePtr)spriteP;
    SWRect *r = &spriteP->destFrameRect;
    FramePtr frameP = spriteP->curFrameP;
    SDL_Rect opaque;
    float sx, sy;

    *left = r->left;
    *top = r->top;
    *right = r->right;
    *bottom = r->bottom;
    if (buttonP->controlType == kCheckboxControl || frameP == NULL || frameP->glSurfaceP == NULL ||
        frameP->glSurfaceP->surface == NULL)
        return;
    opaque = OpaqueBoundsOfFrame(frameP);
    sx = (float)(r->right - r->left) / (float)frameP->glSurfaceP->surface->w;
    sy = (float)(r->bottom - r->top) / (float)frameP->glSurfaceP->surface->h;
    *left = r->left + opaque.x * sx;
    *top = r->top + opaque.y * sy;
    *right = *left + opaque.w * sx;
    *bottom = *top + opaque.h * sy;
}

static void DrawControllerFocusBox(SpriteWorldPtr w)
{
    SpritePtr focusP = GetControllerFocusSprite();
    ButtonSpritePtr buttonP;
    float left, top, right, bottom, pad, thick;
    unsigned short red = 0x5fff, green = 0xffff, blue = 0xffff;     // the cyan of highlighted text
    int i;

    if (focusP == NULL)
        return;
    buttonP = (ButtonSpritePtr)focusP;
    if (buttonP->hasHighlightedColor && !(buttonP->highlightedColor.red == 0xffff &&
                                          buttonP->highlightedColor.green == 0xffff &&
                                          buttonP->highlightedColor.blue == 0xffff)) {
        red = buttonP->highlightedColor.red;
        green = buttonP->highlightedColor.green;
        blue = buttonP->highlightedColor.blue;
    }
    FocusRect(focusP, &left, &top, &right, &bottom);
    pad = 5.0f * w->designWidth / 1280.0f + 2.0f;
    thick = 2.0f * w->designWidth / 1280.0f + 1.0f;
    left -= pad;
    top -= pad;
    right += pad;
    bottom += pad;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glScalef(w->designScaleH * w->zoomH, w->designScaleV * w->zoomV, 1.0f);
    if (!focusP->drawData->nonScrolling)
        glTranslatef(-w->spriteOffsetH, -w->spriteOffsetV, 0);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // a dark line under the coloured one, so it shows on light and dark backgrounds
    for (i = 0; i < 2; i++) {
        float o = i == 0 ? thick * 0.6f : 0.0f;
        if (i == 0)
            glColor4f(0, 0, 0, 0.6f);
        else
            glColor4us(red, green, blue, 0xffff);
        glBegin(GL_QUADS);
        glVertex2f(left + o, top + o); glVertex2f(right + o, top + o);
        glVertex2f(right + o, top + thick + o); glVertex2f(left + o, top + thick + o);
        glVertex2f(left + o, bottom - thick + o); glVertex2f(right + o, bottom - thick + o);
        glVertex2f(right + o, bottom + o); glVertex2f(left + o, bottom + o);
        glVertex2f(left + o, top + o); glVertex2f(left + thick + o, top + o);
        glVertex2f(left + thick + o, bottom + o); glVertex2f(left + o, bottom + o);
        glVertex2f(right - thick + o, top + o); glVertex2f(right + o, top + o);
        glVertex2f(right + o, bottom + o); glVertex2f(right - thick + o, bottom + o);
        glEnd();
    }
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glColor4f(1, 1, 1, 1);
    glPopMatrix();
}

Boolean gSuppressExtrasOverlay = false;

/// Installed as the main sprite world's post-draw callback.
void ExtrasPostDraw(SpriteWorldPtr w)
{
    if (gSuppressExtrasOverlay)
        return;
    if (w == gStatsSpriteWorldP) {
        MultiplayerDrawStatusBar(w);
        return;
    }
    if (w != gSpriteWorldP)
        return;
    MultiplayerDrawPlayArea(w);
    if (NetGameActive())
        ChatDrawInWorld(w);
    DrawControllerFocusBox(w);
}

// ---------------------------------------------------------------------------
// "Smooth room transitions": the old room slides out as the new one slides in. Both rooms
// are snapshots, so the game itself only pauses for the slide; rooms load as they always do.

#define kSlideFrames 18

static SpritePtr gOldRoomSnapshotP = NULL;

static Boolean SmoothTransitionsApply(void)
{
    return gExtras.smoothRooms && !IsMoviePlaying() && gCurrentGameMode == kPlayingGameMode && !gEditorIsRunning;
}

static SWRect PlayAreaRect(void)
{
    SWRect rect = gSpriteWorldP->visScrollRect;

    SWOffsetRect(&rect, -rect.left, -rect.top);
    return rect;
}

static SpritePtr SnapshotPlayArea(void)
{
    SpritePtr snapshotP = NULL;
    SWRect rect = PlayAreaRect();

    gSuppressExtrasOverlay = true;
    MMRenderSceneForWorkAreaSprite(gSpriteWorldP);
    gSuppressExtrasOverlay = false;
    if (SWCreateSpriteFromWorkArea(gSpriteWorldP, &snapshotP, NULL, &rect) != kNoError)
        return NULL;
    return snapshotP;
}

/// Called as Jack walks out of a room, before the next one loads.
void SmoothTransitionCaptureOldRoom(void)
{
    if (gOldRoomSnapshotP != NULL)
        SWDisposeSprite(&gOldRoomSnapshotP);
    if (SmoothTransitionsApply())
        gOldRoomSnapshotP = SnapshotPlayArea();
}

/// Called once the new room is drawn. dirH / dirV: the way Jack went (-1, 0 or 1).
void SmoothTransitionToNewRoom(int dirH, int dirV)
{
    SpritePtr newP, oldP = gOldRoomSnapshotP;
    SWRect rect = PlayAreaRect();
    float width = rect.right - rect.left, height = rect.bottom - rect.top, t, ease;
    int frame;
    Uint32 next;

    gOldRoomSnapshotP = NULL;
    if (oldP == NULL)
        return;
    if (!SmoothTransitionsApply() || (dirH == 0 && dirV == 0) || (newP = SnapshotPlayArea()) == NULL) {
        SWDisposeSprite(&oldP);
        return;
    }
    SWAddSprite(gTextSpriteLayerP, oldP);
    SWAddSprite(gTextSpriteLayerP, newP);
    next = SDL_GetTicks();
    for (frame = 1; frame <= kSlideFrames && !gQuitGame; frame++) {
        t = (float)frame / kSlideFrames;
        ease = t * t * (3.0f - 2.0f * t);
        SWSetSpriteLocation(oldP, rect.left - dirH * width * ease, rect.top - dirV * height * ease);
        SWSetSpriteLocation(newP, rect.left + dirH * width * (1.0f - ease), rect.top + dirV * height * (1.0f - ease));
        SWUpdateSpriteWorld(gSpriteWorldP, true);
        ProcessSDLEvents();
        next += 1000 / 60;
        if (SDL_GetTicks() < next)
            SDL_Delay(next - SDL_GetTicks());
    }
    SWRemoveSpriteFromAnimation(oldP, true);
    SWRemoveSpriteFromAnimation(newP, true);
    SWUpdateSpriteWorld(gSpriteWorldP, true);
}
