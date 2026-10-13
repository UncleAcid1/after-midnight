// MMDialogs.c — replaces MMDialogsMac.c. The original used Carbon dialogs and Navigation
// Services file pickers; these are in-game equivalents drawn with the game's sprites:
// a scrolling list (choose player, open saved game / custom mansion / replay) and a text entry
// box (player name, save name). Keyboard: arrows, Page Up/Down, Return, Escape.

#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include "MMGame.h"
#include "MMProtos.h"
#include "BKGL.h"

#define kDialogOK       0       // "other buttons.png" frames: Okay, Cancel, Yes, No
#define kDialogCancel   1
#define kMaxDialogSprites 160
#define kListVisibleRows 7
#define kMaxListItems   512

typedef struct MMDialog {
    SpriteLayerPtr layerP;
    SpritePtr   sprites[kMaxDialogSprites];
    int         numSprites;
    SpritePtr   panelP;
    float       scale;              ///< designWidth / 640
    struct MMDialog *parentP;
    // saved world state (outermost dialog only)
    float       saveZoomH, saveZoomV, saveOffsetH, saveOffsetV;
    Boolean     fadeLayerWasVisible, wasPaused;
    TileChangeProcPtr saveTileChangeProc;
} MMDialog;

typedef struct ListItem {
    char label[256];            ///< shown in the list
    char name[256];             ///< file or player name
    char path[1024];            ///< full path, if a file
} ListItem;

static MMDialog *gCurDialogP = NULL;

// Keyboard state collected by the event callback while a dialog is up.
static char    gTypedText[64];
static Boolean gKeyReturn, gKeyEscape, gKeyUp, gKeyDown, gKeyPageUp, gKeyPageDown, gKeyBackspace, gKeyDelete;
static int     gWheel;

static Boolean DialogEventCallback(SDL_Event *event)
{
    size_t len;

    switch (event->type) {
        case SDL_TEXTINPUT:
            len = strlen(gTypedText);
            if (len + strlen(event->text.text) < sizeof(gTypedText))
                strcat(gTypedText, event->text.text);
            return true;
        case SDL_KEYDOWN:
            switch (event->key.keysym.scancode) {
                case SDL_SCANCODE_RETURN:
                case SDL_SCANCODE_KP_ENTER:  gKeyReturn = true; break;
                case SDL_SCANCODE_ESCAPE:    gKeyEscape = true; break;
                case SDL_SCANCODE_UP:        gKeyUp = true; break;
                case SDL_SCANCODE_DOWN:      gKeyDown = true; break;
                case SDL_SCANCODE_PAGEUP:    gKeyPageUp = true; break;
                case SDL_SCANCODE_PAGEDOWN:  gKeyPageDown = true; break;
                case SDL_SCANCODE_BACKSPACE: gKeyBackspace = true; break;
                case SDL_SCANCODE_DELETE:    gKeyDelete = true; break;
                default:
                    // Let Command-Q / fullscreen shortcuts through.
                    if (event->key.keysym.mod & (KMOD_GUI | KMOD_CTRL | KMOD_ALT))
                        return false;
                    break;
            }
            return true;
        case SDL_KEYUP:
            return !(event->key.keysym.mod & (KMOD_GUI | KMOD_CTRL | KMOD_ALT));
        case SDL_MOUSEWHEEL:
            gWheel += event->wheel.y;
            return true;
        default:
            return false;
    }
}

static void ClearDialogInput(void)
{
    gTypedText[0] = 0;
    gKeyReturn = gKeyEscape = gKeyUp = gKeyDown = gKeyPageUp = gKeyPageDown = gKeyBackspace = gKeyDelete = false;
    gWheel = 0;
}

static SpritePtr AddDialogSprite(MMDialog *d, SpritePtr spriteP)
{
    if (spriteP != NULL && d->numSprites < kMaxDialogSprites) {
        if (spriteP->parentSpriteLayerP == NULL)
            SWAddSprite(d->layerP, spriteP);
        d->sprites[d->numSprites++] = spriteP;
    }
    return spriteP;
}

/// Temporarily takes a dialog's sprites off screen (while a nested dialog is shown).
static void ShowDialogSprites(MMDialog *d, Boolean show)
{
    int i;
    for (i = 0; i < d->numSprites; i++) {
        if (show)
            SWAddSprite(d->layerP, d->sprites[i]);
        else
            SWRemoveSprite(d->sprites[i]);
    }
}

static Boolean BeginDialog(MMDialog *d, float panelScale)
{
    SWError err;

    memset(d, 0, sizeof(*d));
    d->layerP = gOptionsDialogPane2LayerP;
    d->scale = (float)(gSpriteWorldP->designWidth / 640);
    d->parentP = gCurDialogP;
    if (d->parentP != NULL)
        ShowDialogSprites(d->parentP, false);
    else {
        d->fadeLayerWasVisible = gFadeToBlackSpriteLayerP->layerIsVisible;
        SWSetSpriteLayerVisible(gFadeToBlackSpriteLayerP, false);
        d->saveZoomH = gSpriteWorldP->zoomH;
        d->saveZoomV = gSpriteWorldP->zoomV;
        SWSetSpriteWorldZoom(gSpriteWorldP, 1.0f, 1.0f);
        d->saveOffsetH = gSpriteWorldP->spriteOffsetH;
        d->saveOffsetV = gSpriteWorldP->spriteOffsetV;
        gSpriteWorldP->spriteOffsetH = 0;
        gSpriteWorldP->spriteOffsetV = 0;
        d->saveTileChangeProc = gSpriteWorldP->tileChangeProc;
        SWSetTileChangeProc(gSpriteWorldP, NULL);
        d->wasPaused = gGameIsPaused;
        PauseAllSpriteLayers(gSpriteWorldP);
        SWUnpauseSpriteLayer(d->layerP);
        SWUnpauseSpriteLayer(gCustomCursorSpriteLayerP);
        SWSetSpriteLayerVisible(d->layerP, true);
        MMSaveCursorState();
        MMShieldCursor();
        MMSetCursorForGame();
        SetMMEventCallback(DialogEventCallback);
    }
    gCurDialogP = d;

    err = MMCreateSpriteFromVerticalFile(gSpriteWorldP, &d->panelP, NULL, "custom dialog.png", 1, kNoMask);
    if (err != kNoError || d->panelP == NULL)
        return false;
    AddDialogSprite(d, d->panelP);
    SWSetSpriteScaledFactor(d->panelP, d->scale * panelScale, d->scale * panelScale);
    SWCenterSpriteInRect(d->panelP, &gSpriteWorldP->workAreaRect);
    ClearDialogInput();
    SDL_StartTextInput();
    return true;
}

static void EndDialog(MMDialog *d)
{
    int i;

    for (i = 0; i < d->numSprites; i++) {
        SWRemoveSprite(d->sprites[i]);
        SWDisposeSprite(&d->sprites[i]);
    }
    d->numSprites = 0;
    gCurDialogP = d->parentP;
    if (d->parentP != NULL) {
        ShowDialogSprites(d->parentP, true);
        ClearDialogInput();
        return;
    }
    SDL_StopTextInput();
    SetMMEventCallback(NULL);
    MMRestoreCursorState();
    gSpriteWorldP->spriteOffsetH = d->saveOffsetH;
    gSpriteWorldP->spriteOffsetV = d->saveOffsetV;
    gSpriteWorldP->zoomH = d->saveZoomH;
    gSpriteWorldP->zoomV = d->saveZoomV;
    SWSetSpriteLayerVisible(gFadeToBlackSpriteLayerP, d->fadeLayerWasVisible);
    SWSetTileChangeProc(gSpriteWorldP, d->saveTileChangeProc);
    SWUpdateSpriteWorld(gSpriteWorldP, true);
    UnpauseAllSpriteLayers(gSpriteWorldP);
    if (d->wasPaused)
        PauseSpriteLayersForPause();
    ResetKeys();
    WriteEventToMovieFile(0xb, true);
}

/// Asks a yes/no question over the current dialog. Returns true for Yes.
static Boolean AskYesNo(const char *question)
{
    Boolean answer;

    if (gCurDialogP != NULL)
        ShowDialogSprites(gCurDialogP, false);
    SetMMEventCallback(NULL);
    answer = MMDoCustomDialog(gSpriteWorldP, question, 1) == 2;
    SetMMEventCallback(DialogEventCallback);
    if (gCurDialogP != NULL) {
        ShowDialogSprites(gCurDialogP, true);
        SWUnpauseSpriteLayer(gCurDialogP->layerP);
    }
    ClearDialogInput();
    return answer;
}

static SpritePtr AddDialogText(MMDialog *d, const char *text, float scale, short h, short v, short maxWidth)
{
    SpritePtr textP = NewBitmapTextSprite(maxWidth);

    if (textP != NULL) {
        SWSetSpriteScaledFactor(textP, d->scale * scale, d->scale * scale);
        SetBMTextSpriteText(textP, text);
        AddDialogSprite(d, textP);
        SWSetSpriteLocation(textP, h, v);
    }
    return textP;
}

/// Sets a text sprite's text and centers it horizontally on the panel (text that would need
/// to wrap stays left-aligned at its current position).
static void SetDialogTextCentered(MMDialog *d, SpritePtr textP, const char *text)
{
    SWRect *panel = &d->panelP->destFrameRect;
    float width, left;

    if (textP == NULL)
        return;
    SetBMTextSpriteText(textP, text);
    width = GetWidthOfBMText(textP, text);
    left = ((float)panel->left + (float)panel->right - width) / 2.0f;
    if (width < (float)(panel->right - panel->left) - 40.0f * d->scale)
        SWSetSpriteLocation(textP, left, SWGetSpriteVertLoc(textP));
}

/// Okay (and Cancel) at the lower right of the panel, like MMDoCustomDialog.
static void AddDialogButtons(MMDialog *d, Boolean withCancel)
{
    SpritePtr panelP = d->panelP;
    short h, v;

    v = (short)((SWGetSpriteHeight(panelP) - (GetButtonHeight(0xda) + 16) * d->scale) + SWGetSpriteVertLoc(panelP));
    h = (short)((SWGetSpriteWidth(panelP) - (GetButtonWidth(0xda) + 19) * d->scale) + SWGetSpriteHorizLoc(panelP));
    AddDialogSprite(d, AddButtonToDialog(0xda, kDialogOK, d->layerP, h, v));
    if (withCancel) {
        h = (short)(h - (GetButtonWidth(0xda) + 4) * d->scale);
        AddDialogSprite(d, AddButtonToDialog(0xda, kDialogCancel, d->layerP, h, v));
    }
}

/// Runs one frame; returns the clicked button index or -1.
static short DialogFrame(MMDialog *d, SWPoint *mouseP, Boolean *mouseDownP)
{
    SWProcessSpriteWorld(gSpriteWorldP);
    SWAnimateSpriteWorld(gSpriteWorldP);
    ProcessSDLEvents();
    *mouseDownP = MMGetMouse(mouseP);
    SWConvertCoordinates(gSpriteWorldP, mouseP);
    if (!gSpriteWorldP->frameHasOccurred)
        return -1;
    return ProcessButtonSprites(d->layerP, d->layerP, *mouseP, *mouseDownP, false, NULL);
}

static Boolean PointInSprite(SWPoint point, SpritePtr spriteP)
{
    SWRect *r = &spriteP->destFrameRect;
    return spriteP->isVisible && point.h >= r->left && point.h < r->right && point.v >= r->top && point.v < r->bottom;
}

// ---------------------------------------------------------------------------
// Text entry, with an on-screen keyboard (this version) for controllers and mice. The keys are
// the Controls pane's key boxes with Action Man lettering; typing on a keyboard still works.

#define kKeyButtonBase  0x100
enum { kKeyShift = -1, kKeySpace = -2, kKeyDelete = -3 };

static const char *kKeyboardRows[4] = { "1234567890", "qwertyuiop", "asdfghjkl'", "zxcvbnm-.!" };

typedef struct OnScreenKey {
    SpritePtr buttonP, labelP;
    int code;               ///< the character, or kKeyShift / kKeySpace / kKeyDelete
} OnScreenKey;

typedef struct OnScreenKeyboard {
    OnScreenKey keys[48];
    int numKeys;
    Boolean shift;
} OnScreenKeyboard;

static void SetKeyLabel(OnScreenKey *key, Boolean shift)
{
    char label[8];

    if (key->labelP == NULL)
        return;
    if (key->code == kKeyShift)
        snprintf(label, sizeof(label), "Shift");
    else if (key->code == kKeySpace)
        snprintf(label, sizeof(label), "Space");
    else if (key->code == kKeyDelete)
        snprintf(label, sizeof(label), "Delete");
    else
        snprintf(label, sizeof(label), "%c", shift ? toupper(key->code) : key->code);
    SetTextSpriteText(key->labelP, label);
}

/// Draws a key box at any width without thickening its sides: the ends keep their shape and
/// only the middle of the image is stretched.
static void KeyCapDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    SpriteWorldPtr w = gSWCurrentSpriteWorld;
    DrawDataPtr d = srcSpriteP->drawData;
    BKGLSurface *g = curFrameP != NULL ? curFrameP->glSurfaceP : NULL;
    float width = srcSpriteP->destFrameRect.right - srcSpriteP->destFrameRect.left;
    float height = srcSpriteP->destFrameRect.bottom - srcSpriteP->destFrameRect.top;
    float cap, edge = 0.16f;
    float xs[4], us[4] = { 0.0f, 0.16f, 0.84f, 1.0f };
    int i;

    if (g == NULL || g->height == 0)
        return;
    cap = height * (float)g->width / (float)g->height * edge;
    if (cap * 2.0f > width)
        cap = width / 2.0f;
    xs[0] = 0.0f;
    xs[1] = cap;
    xs[2] = width - cap;
    xs[3] = width;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    if (w != NULL)
        glScalef(w->designScaleH, w->designScaleV, 1.0f);
    glTranslatef(horizLoc, vertLoc, 0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g->texture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4us(d->colorRed, d->colorGreen, d->colorBlue, (GLushort)(d->translucencyLevel & 0xFFFF));
    glBegin(GL_QUADS);
    for (i = 0; i < 3; i++) {
        glTexCoord2f(us[i], 0.0f);     glVertex2f(xs[i], 0.0f);
        glTexCoord2f(us[i + 1], 0.0f); glVertex2f(xs[i + 1], 0.0f);
        glTexCoord2f(us[i + 1], 1.0f); glVertex2f(xs[i + 1], height);
        glTexCoord2f(us[i], 1.0f);     glVertex2f(xs[i], height);
    }
    glEnd();
    glDisable(GL_BLEND);
    glColor4f(1, 1, 1, 1);
    glPopMatrix();
}

static void AddOnScreenKey(MMDialog *d, OnScreenKeyboard *kb, int code, short left, short top, short width,
                           short height)
{
    static const unsigned char yellow[3] = { 0xff, 0xff, 0x5f };
    OnScreenKey *key = &kb->keys[kb->numKeys];
    SWRect rect;

    if (kb->numKeys >= (int)SDL_arraysize(kb->keys))
        return;
    key->code = code;
    key->buttonP = CloneButtonSprite(gSpriteArray[222], (short)(kKeyButtonBase + kb->numKeys));
    if (key->buttonP == NULL)
        return;
    SetUpButtonSprite(key->buttonP, 0, 1, 1, 0);
    InstallButtonProc(key->buttonP, MMStandardButtonProc);
    SetButtonColor(key->buttonP, 0xffff, 0xffff, 0x5fff);
    SetButtonHighlightedColor(key->buttonP, 0x5fff, 0xffff, 0xffff);
    SetButtonPushedColor(key->buttonP, 0x9fff, 0x9fff, 0x1fff);
    SetButtonPushedOffset(key->buttonP, 1, 1);
    SWSetSpriteScaledSize(key->buttonP, width, height);
    SWSetSpriteDrawProc(key->buttonP, KeyCapDrawProc);
    AddDialogSprite(d, key->buttonP);
    SWSetSpriteLocation(key->buttonP, left, top);
    ChangeButtonColor(key->buttonP, kButtonNormal);

    key->labelP = NULL;
    if (SWCreateTextSprite(&key->labelP, " ", "Action Man", (short)(height * 0.52f), 0, yellow, true) == kNoError) {
        rect.left = left;
        rect.right = (short)(left + width);
        rect.top = top;
        rect.bottom = (short)(top + height * 0.7f);
        SetTextSpriteRect(key->labelP, &rect, 2, false);
        AddDialogSprite(d, key->labelP);
    }
    SetKeyLabel(key, false);
    kb->numKeys++;
}

/// Four rows of ten keys, then Shift, Space and Delete, filling the panel's width from top.
static void AddOnScreenKeyboard(MMDialog *d, OnScreenKeyboard *kb, short top)
{
    SWRect *panel = &d->panelP->destFrameRect;
    short margin = (short)(34 * d->scale), gap = (short)(5 * d->scale), row, col;
    short keyW = (short)((panel->right - panel->left - 2 * margin - 9 * gap) / 10);
    short keyH = (short)(25 * d->scale), left = (short)(panel->left + margin), v;

    memset(kb, 0, sizeof(*kb));
    for (row = 0; row < 4; row++) {
        v = (short)(top + row * (keyH + gap));
        for (col = 0; col < 10; col++)
            AddOnScreenKey(d, kb, kKeyboardRows[row][col], (short)(left + col * (keyW + gap)), v, keyW, keyH);
    }
    v = (short)(top + 4 * (keyH + gap));
    AddOnScreenKey(d, kb, kKeyShift, left, v, (short)(keyW * 2 + gap), keyH);
    AddOnScreenKey(d, kb, kKeySpace, (short)(left + 2 * (keyW + gap)), v, (short)(keyW * 5 + gap * 4), keyH);
    AddOnScreenKey(d, kb, kKeyDelete, (short)(left + 7 * (keyW + gap)), v, (short)(keyW * 3 + gap * 2), keyH);
}

/// Labels follow their key's highlight colour.
static void UpdateOnScreenKeyboard(OnScreenKeyboard *kb)
{
    int i;

    for (i = 0; i < kb->numKeys; i++) {
        OnScreenKey *key = &kb->keys[i];
        TextSpritePtr textP = (TextSpritePtr)key->labelP;
        Boolean lit;

        if (textP == NULL || key->buttonP == NULL)
            continue;
        lit = ((ButtonSpritePtr)key->buttonP)->state != kButtonNormal || (key->code == kKeyShift && kb->shift);
        textP->color[0] = lit ? 0x5f : 0xff;
        textP->color[1] = 0xff;
        textP->color[2] = lit ? 0xff : 0x5f;
    }
}

/// Lets the player type up to maxLen characters into text (a C string). Returns true for Okay.
static Boolean RunTextEntryDialog(const char *prompt, char *text, int maxLen, Boolean forFileName)
{
    MMDialog d;
    OnScreenKeyboard kb;
    SpritePtr entryP;
    SWRect panelRect;
    SWPoint mouse;
    Boolean mouseDown, done = false, ok = false, caretOn = true;
    char shown[300], working[256], typed[80];
    Uint32 nextBlink = 0;
    short button, textWidth;
    size_t i, len;
    int k;

    if (!BeginDialog(&d, 0.72f)) {
        EndDialog(&d);
        return false;
    }
    snprintf(working, sizeof(working), "%s", text);
    panelRect = d.panelP->destFrameRect;
    textWidth = (short)(panelRect.right - panelRect.left - 60 * d.scale);
    SetDialogTextCentered(&d, AddDialogText(&d, prompt, 0.3f, (short)(panelRect.left + 30 * d.scale),
                                            (short)(panelRect.top + 22 * d.scale), textWidth), prompt);
    entryP = AddDialogText(&d, "", 0.35f, (short)(panelRect.left + 30 * d.scale),
                           (short)(panelRect.top + 58 * d.scale), textWidth);
    if (entryP != NULL)
        SWSetSpriteColor(entryP, 0xffff, 0xffff, 0x8000);
    AddOnScreenKeyboard(&d, &kb, (short)(panelRect.top + 100 * d.scale));
    AddDialogButtons(&d, true);
        // capitals to start with, and after a space, as names usually go
    kb.shift = working[0] == 0;
    for (k = 0; k < kb.numKeys; k++)
        SetKeyLabel(&kb.keys[k], kb.shift);

    while (!done && !gQuitGame) {
        button = DialogFrame(&d, &mouse, &mouseDown);

            // keys clicked on screen become typed characters
        snprintf(typed, sizeof(typed), "%s", gTypedText);
        gTypedText[0] = 0;
        if (typed[0] != 0) {        // typing on a real keyboard: its own Shift decides
            kb.shift = typed[strlen(typed) - 1] == ' ';
            for (k = 0; k < kb.numKeys; k++)
                SetKeyLabel(&kb.keys[k], kb.shift);
        }
        if (button >= kKeyButtonBase && button < kKeyButtonBase + kb.numKeys) {
            k = kb.keys[button - kKeyButtonBase].code;
            if (k == kKeyShift)
                kb.shift = !kb.shift;
            else if (k == kKeyDelete)
                gKeyBackspace = true;
            else {
                len = strlen(typed);
                if (len + 1 < sizeof(typed)) {
                    typed[len] = k == kKeySpace ? ' ' : (char)(kb.shift ? toupper(k) : k);
                    typed[len + 1] = 0;
                }
                kb.shift = k == kKeySpace;
            }
            for (k = 0; k < kb.numKeys; k++)
                SetKeyLabel(&kb.keys[k], kb.shift);
        }
        if (gControllerMenuAlt) {       // X (Square) deletes
            gKeyBackspace = true;
            gControllerMenuAlt = false;
        }
        UpdateOnScreenKeyboard(&kb);

        // Typed characters (ASCII only: the bitmap font and file names)
        for (i = 0; typed[i] != 0; i++) {
            unsigned char c = (unsigned char)typed[i];
            len = strlen(working);
            if (c < 32 || c > 126 || (int)len >= maxLen)
                continue;
            if (forFileName && (c == '/' || c == ':' || c == '\\' || (len == 0 && c == '.')))
                continue;
            working[len] = (char)c;
            working[len + 1] = 0;
        }
        if (gKeyBackspace && working[0] != 0)
            working[strlen(working) - 1] = 0;
        gKeyBackspace = false;

        if (SDL_GetTicks() >= nextBlink) {
            caretOn = !caretOn;
            nextBlink = SDL_GetTicks() + 400;
        }
            // centered on the text itself, so the caret doesn't make the line jump
        snprintf(shown, sizeof(shown), "%s_", working);
        SetDialogTextCentered(&d, entryP, shown);
        if (!caretOn && entryP != NULL) {
            snprintf(shown, sizeof(shown), "%s ", working);
            SetBMTextSpriteText(entryP, shown);
        }

        if (gKeyEscape || button == kDialogCancel)
            done = true;
        else if ((gKeyReturn || button == kDialogOK) && working[0] != 0)
            done = ok = true;
        gKeyReturn = gKeyEscape = false;
    }
    if (ok)
        strcpy(text, working);
    AnimateOneFrame(gSpriteWorldP);
    EndDialog(&d);
    return ok;
}

// ---------------------------------------------------------------------------
// Scrolling list

enum { kListCancelled = 0, kListChosen = 1, kListNew = 2, kListDelete = 3 };
static Boolean gListWithoutDelete = false;

/// Shows items and lets the player pick one. With manageItems, "New" and "Delete" are offered
/// (also the N and Delete keys). *selectedP is in/out.
static int RunListDialog(const char *title, ListItem *items, int count, int *selectedP, Boolean manageItems,
                         const char *emptyText)
{
    MMDialog d;
    SpritePtr rowSprites[kListVisibleRows], newP = NULL, deleteP = NULL, moreUpP, moreDownP;
    SWRect panelRect;
    SWPoint mouse;
    Boolean mouseDown, wasDown = false, done = false;
    Uint32 lastClickTime = 0;
    int result = kListCancelled, selected = *selectedP, top = 0, lastClickRow = -1, i, row;
    short button, h, v, rowHeight, width;

    if (!BeginDialog(&d, 0.65f)) {
        EndDialog(&d);
        return kListCancelled;
    }
    ControllerSetListMode(true);
    panelRect = d.panelP->destFrameRect;
    h = (short)(panelRect.left + 40 * d.scale);
    width = (short)(panelRect.right - panelRect.left - 80 * d.scale);
    SetDialogTextCentered(&d, AddDialogText(&d, title, 0.45f, h, (short)(panelRect.top + 30 * d.scale), width), title);
    rowHeight = (short)(26 * d.scale);
    v = (short)(panelRect.top + 75 * d.scale);
    for (i = 0; i < kListVisibleRows; i++)
        rowSprites[i] = AddDialogText(&d, "", 0.33f, (short)(h + 12 * d.scale), (short)(v + i * rowHeight), width);
    moreUpP = AddDialogText(&d, "...", 0.33f, h, (short)(v - rowHeight * 0.6f), width);
    moreDownP = AddDialogText(&d, "...", 0.33f, h, (short)(v + kListVisibleRows * rowHeight - rowHeight * 0.3f), width);
    SetDialogTextCentered(&d, moreUpP, "...");
    SetDialogTextCentered(&d, moreDownP, "...");
    if (manageItems) {
        v = (short)((SWGetSpriteHeight(d.panelP) - (GetButtonHeight(0xda) + 6) * d.scale) + SWGetSpriteVertLoc(d.panelP));
#ifdef MM_PS3
            // the buttons' names are long: one above the other, left of Cancel and Okay
        newP = AddDialogText(&d, "New Player (Square)", 0.33f, h, (short)(v - 14 * d.scale), width);
        if (!gListWithoutDelete)
            deleteP = AddDialogText(&d, "Delete (Triangle)", 0.33f, h, (short)(v + 14 * d.scale), width);
#else
        newP = AddDialogText(&d, IsControllerConnected() ? "New Player (X)" : "New Player", 0.4f, h, v, width);
        if (!gListWithoutDelete)
            deleteP = AddDialogText(&d, IsControllerConnected() ? "Delete (Y)" : "Delete", 0.4f, h, v, width);
#endif
#ifndef MM_PS3
        if (newP != NULL && deleteP != NULL)
            SWSetSpriteLocation(deleteP, h + GetWidthOfBMText(newP, GetTextSpriteText(newP)) + 30 * d.scale, v);
#endif
    }
    AddDialogButtons(&d, true);
    if (count == 0 && rowSprites[0] != NULL)
        SetDialogTextCentered(&d, rowSprites[0], emptyText);
    if (selected >= count)
        selected = count - 1;
    gControllerMenuAlt = gControllerMenuAlt2 = false;   // only presses made in this list

    while (!done && !gQuitGame) {
        button = DialogFrame(&d, &mouse, &mouseDown);

        // Keyboard and wheel
        if (count > 0) {
            if (gKeyUp && selected > 0) selected--;
            if (gKeyDown && selected < count - 1) selected++;
            if (gKeyPageUp) selected = SDL_max(0, selected - kListVisibleRows);
            if (gKeyPageDown) selected = SDL_min(count - 1, selected + kListVisibleRows);
            if (selected < 0) selected = 0;
        }
        top -= gWheel;
        gWheel = 0;
        if (gKeyUp || gKeyDown || gKeyPageUp || gKeyPageDown) {
            if (selected < top) top = selected;
            if (selected >= top + kListVisibleRows) top = selected - kListVisibleRows + 1;
        }
        top = SDL_max(0, SDL_min(top, count - kListVisibleRows));
        gKeyUp = gKeyDown = gKeyPageUp = gKeyPageDown = false;

        // Mouse clicks on rows; a double-click chooses.
        if (mouseDown && !wasDown) {
            for (i = 0; i < kListVisibleRows; i++) {
                row = top + i;
                if (row < count && rowSprites[i] != NULL &&
                    mouse.v >= rowSprites[i]->destFrameRect.top && mouse.v < rowSprites[i]->destFrameRect.top + rowHeight &&
                    mouse.h >= panelRect.left && mouse.h < panelRect.right) {
                    if (row == lastClickRow && SDL_GetTicks() - lastClickTime < 400)
                        button = kDialogOK;
                    selected = row;
                    lastClickRow = row;
                    lastClickTime = SDL_GetTicks();
                }
            }
            if (newP != NULL && PointInSprite(mouse, newP))
                result = kListNew, done = true;
            if (deleteP != NULL && PointInSprite(mouse, deleteP) && selected >= 0)
                result = kListDelete, done = true;
        }
        wasDown = mouseDown;
        if (manageItems) {
            for (i = 0; gTypedText[i] != 0; i++)
                if (gTypedText[i] == 'n' || gTypedText[i] == 'N')
                    result = kListNew, done = true;
            if ((gKeyDelete || gKeyBackspace) && selected >= 0 && !gListWithoutDelete)
                result = kListDelete, done = true;
            if (gControllerMenuAlt)
                result = kListNew, done = true;
            if (gControllerMenuAlt2 && selected >= 0 && !gListWithoutDelete)
                result = kListDelete, done = true;
        }
        gControllerMenuAlt = gControllerMenuAlt2 = false;
        gTypedText[0] = 0;
        gKeyDelete = gKeyBackspace = false;

        // Draw rows
        for (i = 0; i < kListVisibleRows; i++) {
            row = top + i;
            if (rowSprites[i] == NULL || (count == 0 && i == 0))
                continue;
            SetDialogTextCentered(&d, rowSprites[i], row < count ? items[row].label : "");
            if (row == selected)
                SWSetSpriteColor(rowSprites[i], 0xffff, 0xffff, 0x2000);
            else
                SWSetSpriteColor(rowSprites[i], 0xffff, 0xffff, 0xffff);
        }
        if (moreUpP != NULL)
            SWSetSpriteVisible(moreUpP, top > 0);
        if (moreDownP != NULL)
            SWSetSpriteVisible(moreDownP, top + kListVisibleRows < count);

        if (!done) {
            if (gKeyEscape || button == kDialogCancel)
                result = kListCancelled, done = true;
            else if ((gKeyReturn || button == kDialogOK) && selected >= 0 && count > 0)
                result = kListChosen, done = true;
        }
        gKeyReturn = gKeyEscape = false;
    }
    *selectedP = selected;
    ControllerSetListMode(false);
    AnimateOneFrame(gSpriteWorldP);
    EndDialog(&d);
    return result;
}

static int CompareListItems(const void *a, const void *b)
{
    return SDL_strcasecmp(((const ListItem *)a)->label, ((const ListItem *)b)->label);
}

/// Adds the visible files (or folders) of a folder to the list. prefix is put before the label.
static int AddFolderToList(ListItem *items, int count, const char *folder, const char *prefix, Boolean wantFolders,
                           Boolean wantFiles)
{
    DIR *dir = opendir(folder);
    struct dirent *entry;
    struct stat info;
    char path[1024];
    size_t len = strlen(folder);
    const char *slash = (len > 0 && folder[len - 1] == '/') ? "" : "/";

    if (dir == NULL)
        return count;
    while ((entry = readdir(dir)) != NULL && count < kMaxListItems) {
        if (entry->d_name[0] == '.')
            continue;
        snprintf(path, sizeof(path), "%s%s%s", folder, slash, entry->d_name);
        if (stat(path, &info) != 0)
            continue;
        if ((S_ISDIR(info.st_mode) && !wantFolders) || (S_ISREG(info.st_mode) && !wantFiles))
            continue;
        snprintf(items[count].label, sizeof(items[count].label), "%s%s", prefix, entry->d_name);
        snprintf(items[count].name, sizeof(items[count].name), "%s", entry->d_name);
        snprintf(items[count].path, sizeof(items[count].path), "%s", path);
        count++;
    }
    closedir(dir);
    return count;
}

static ListItem *AllocListItems(void)
{
    ListItem *items = (ListItem *)calloc(kMaxListItems, sizeof(ListItem));
    if (items == NULL)
        MMDoErrorMessage("Out of memory.", 0);
    return items;
}

// ---------------------------------------------------------------------------
// Player profiles

/// Returns true once a player has been chosen (and loaded).
Boolean DoChoosePlayerDialog(void)
{
    ListItem *items;
    char folder[1024], name[256], question[300];
    Str255 pascalName;
    int count, selected = 0, i, action;
    Boolean chosen = false;

    items = AllocListItems();
    if (items == NULL)
        return false;

    name[0] = 0;
    if (gCurrentPlayerNameExists)
        SWCopyPascalStringToCString((ConstStringPtr)gCurrentPlayerName, name);
    for (;;) {
        snprintf(folder, sizeof(folder), "%s", GetPlayerProfilePath(NULL));
        count = AddFolderToList(items, 0, folder, "", false, true);
        qsort(items, count, sizeof(ListItem), CompareListItems);
        for (i = 0; i < count; i++)
            if (strcmp(items[i].name, name) == 0)
                selected = i;
        action = RunListDialog("Choose Player", items, count, &selected, true, "(No players yet. Click New Player.)");
        if (action == kListNew) {
            if (DoEnterNameForNewProfileDialog(pascalName)) {
                // Create it without switching to it; it gets selected in the list.
                SaveCurrentProfile();
                CreateNewProfile(pascalName);
                RestoreCurrentProfile();
                SWCopyPascalStringToCString(pascalName, name);
            }
        }
        else if (action == kListDelete) {
            if (selected >= 0 && selected < count) {
                snprintf(question, sizeof(question), "Are you sure you want to delete the player \"%s\"?",
                         items[selected].name);
                if (AskYesNo(question)) {
                    DeleteProfile(items[selected].name);
                    if (selected > 0)
                        selected--;
                }
            }
        }
        else
            break;
    }

    if (action == kListChosen && selected >= 0 && selected < count) {
        GetPascalStringFromCString(pascalName, items[selected].name);
        if (LoadProfileFromDisk(pascalName) != 0)
            MMDoCustomDialog(gSpriteWorldP, "Sorry, that player profile couldn't be opened.", 3);
        else
            chosen = true;
        WritePreferencesFile();
        gLastOpenDialog = 0x10;
    }
    free(items);
    MMSetCursorForGame();
    return chosen;
}

/// Text entry for other modules (C string). Returns true for Okay.
Boolean DoTextEntryDialog(const char *prompt, char *text, int maxLen)
{
    return RunTextEntryDialog(prompt, text, maxLen, false);
}

/// A message with a Cancel button, shown until poll returns true (then true) or the player
/// cancels (false).
Boolean RunWaitDialog(const char *title, const char *message, Boolean (*poll)(void *), void *context)
{
    MMDialog d;
    SWRect panelRect;
    SWPoint mouse;
    Boolean mouseDown, result = false;
    short button, width, h;
    SpritePtr panelP;

    if (!BeginDialog(&d, 0.6f)) {
        EndDialog(&d);
        return false;
    }
    panelRect = d.panelP->destFrameRect;
    h = (short)(panelRect.left + 30 * d.scale);
    width = (short)(panelRect.right - panelRect.left - 60 * d.scale);
    SetDialogTextCentered(&d, AddDialogText(&d, title, 0.4f, h, (short)(panelRect.top + 24 * d.scale), width), title);
    AddDialogText(&d, message, 0.27f, h, (short)(panelRect.top + 70 * d.scale), width);
        // Cancel only, at the lower right
    panelP = d.panelP;
    AddDialogSprite(&d, AddButtonToDialog(0xda, kDialogCancel, d.layerP,
        (short)((SWGetSpriteWidth(panelP) - (GetButtonWidth(0xda) + 19) * d.scale) + SWGetSpriteHorizLoc(panelP)),
        (short)((SWGetSpriteHeight(panelP) - (GetButtonHeight(0xda) + 16) * d.scale) + SWGetSpriteVertLoc(panelP))));
    for (;;) {
        button = DialogFrame(&d, &mouse, &mouseDown);
        if (poll(context)) {
            result = true;
            break;
        }
        if (gKeyEscape || button == kDialogCancel || gQuitGame)
            break;
        gKeyReturn = gKeyEscape = false;
        gTypedText[0] = 0;
    }
    AnimateOneFrame(gSpriteWorldP);
    EndDialog(&d);
    return result;
}

/// A message that can change while it's shown (message() is asked again every frame), with
/// Cancel, and Okay while canStart() says so (canStart NULL: no Okay). poll() returns 1 when
/// done (the dialog returns true), -1 to give up (false), 0 to keep waiting. Okay returns true.
Boolean RunLiveWaitDialog(const char *title, void (*message)(char *, size_t, void *), int (*poll)(void *),
                          Boolean (*canStart)(void *), void *context)
{
    MMDialog d;
    SWRect panelRect;
    SWPoint mouse;
    Boolean mouseDown, result = false, startShown = false;
    short button, width, h, v;
    SpritePtr panelP, textP, okP = NULL;
    char text[1200], shown[1200] = "";
    int state;

    if (!BeginDialog(&d, 0.6f)) {
        EndDialog(&d);
        return false;
    }
    panelRect = d.panelP->destFrameRect;
    panelP = d.panelP;
    h = (short)(panelRect.left + 30 * d.scale);
    width = (short)(panelRect.right - panelRect.left - 60 * d.scale);
    SetDialogTextCentered(&d, AddDialogText(&d, title, 0.4f, h, (short)(panelRect.top + 24 * d.scale), width), title);
    message(text, sizeof(text), context);
    textP = AddDialogText(&d, text, 0.27f, h, (short)(panelRect.top + 70 * d.scale), width);
    snprintf(shown, sizeof(shown), "%s", text);
    v = (short)((SWGetSpriteHeight(panelP) - (GetButtonHeight(0xda) + 16) * d.scale) + SWGetSpriteVertLoc(panelP));
    h = (short)((SWGetSpriteWidth(panelP) - (GetButtonWidth(0xda) + 19) * d.scale) + SWGetSpriteHorizLoc(panelP));
    if (canStart != NULL) {
        okP = AddDialogSprite(&d, AddButtonToDialog(0xda, kDialogOK, d.layerP, h, v));
        h = (short)(h - (GetButtonWidth(0xda) + 4) * d.scale);
    }
    AddDialogSprite(&d, AddButtonToDialog(0xda, kDialogCancel, d.layerP, h, v));
    for (;;) {
        Boolean can = canStart != NULL && canStart(context);
        if (okP != NULL && can != startShown) {
            SWSetSpriteVisible(okP, can);
            startShown = can;
        }
        button = DialogFrame(&d, &mouse, &mouseDown);
        state = poll(context);
        if (state != 0) {
            result = state > 0;
            break;
        }
        message(text, sizeof(text), context);
        if (strcmp(text, shown) != 0 && textP != NULL) {
            SetBMTextSpriteText(textP, text);
            snprintf(shown, sizeof(shown), "%s", text);
        }
        if (gKeyEscape || button == kDialogCancel || gQuitGame)
            break;
        if (can && (gKeyReturn || button == kDialogOK)) {
            result = true;
            break;
        }
        gKeyReturn = gKeyEscape = false;
        gTypedText[0] = 0;
    }
    AnimateOneFrame(gSpriteWorldP);
    EndDialog(&d);
    return result;
}

/// Picks one of a few fixed choices. Returns its index, or -1 if cancelled.
int DoSimpleChoiceDialog(const char *title, const char **choices, int count, int selected)
{
    ListItem *items = AllocListItems();
    int i, result;

    if (items == NULL)
        return -1;
    for (i = 0; i < count && i < kMaxListItems; i++)
        snprintf(items[i].label, sizeof(items[i].label), "%s", choices[i]);
    result = RunListDialog(title, items, count, &selected, false, "") == kListChosen ? selected : -1;
    free(items);
    return result;
}

/// Picks a player profile other than the ones in exclude (making a new one is offered), for
/// another player in a multiplayer game. The current profile stays loaded.
Boolean DoChooseOtherPlayerDialog(const char *title, char exclude[][64], int numExclude, char *nameOut,
                                  size_t size)
{
    ListItem *items;
    char folder[1024], name[256] = "";
    Str255 pascalName;
    int count, kept, selected = 0, i, j, action;
    Boolean chosen = false;

    items = AllocListItems();
    if (items == NULL)
        return false;
    for (;;) {
        snprintf(folder, sizeof(folder), "%s", GetPlayerProfilePath(NULL));
        count = AddFolderToList(items, 0, folder, "", false, true);
        for (i = 0, kept = 0; i < count; i++) {
            for (j = 0; j < numExclude && strcmp(items[i].name, exclude[j]) != 0; j++)
                ;
            if (j == numExclude)
                items[kept++] = items[i];
        }
        qsort(items, kept, sizeof(ListItem), CompareListItems);
        for (i = 0; i < kept; i++)
            if (strcmp(items[i].name, name) == 0)
                selected = i;
        gListWithoutDelete = true;      // profiles are deleted from Change Player
        action = RunListDialog(title, items, kept, &selected, true,
                               "(Every player needs a profile. Choose New Player.)");
        gListWithoutDelete = false;
        if (action == kListNew) {
            if (DoEnterNameForNewProfileDialog(pascalName)) {
                SaveCurrentProfile();
                CreateNewProfile(pascalName);
                RestoreCurrentProfile();
                WritePreferencesFile();     // player 1 stays the one who opens the game next time
                SWCopyPascalStringToCString(pascalName, name);
            }
        }
        else
            break;
    }
    if (action == kListChosen && selected >= 0 && selected < kept) {
        snprintf(nameOut, size, "%s", items[selected].name);
        chosen = true;
    }
    free(items);
    MMSetCursorForGame();
    return chosen;
}

/// Asks for a new player's name (a Pascal string). Returns true if one was entered.
Boolean DoEnterNameForNewProfileDialog(StringPtr nameOut)
{
    char name[64] = "";

    for (;;) {
        if (!RunTextEntryDialog("Enter a name for the new player:", name, 0x1f, true))
            return false;
        if (!DoesProfileAlreadyExist(name))
            break;
        MMDoCustomDialog(gSpriteWorldP, "A player with that name already exists. Please choose another name.", 3);
    }
    GetPascalStringFromCString(nameOut, name);
    return true;
}

Boolean DoAreYouSureDeleteDialog(ConstStringPtr playerName)
{
    char name[256], question[300];

    SWCopyPascalStringToCString(playerName, name);
    snprintf(question, sizeof(question), "Are you sure you want to delete the player \"%s\"?", name);
    return AskYesNo(question);
}

/// Name for the high score list (Pascal strings). Returns -1 if cancelled, else 0 (the original's
/// checkbox for posting the score online, a service that no longer exists).
short MMDoEnterNameDialog(ConstStringPtr defaultName, StringPtr nameOut, Boolean wonMansion)
{
    char name[64];

    (void)wonMansion;
    SWCopyPascalStringToCString(defaultName, name);
    if (strlen(name) > 0x11)
        name[0x11] = 0;
    if (!RunTextEntryDialog("New high score! Enter your name:", name, 0x11, false))
        return -1;
    GetPascalStringFromCString(nameOut, name);
    return 0;
}

// ---------------------------------------------------------------------------
// Saved games, custom mansions and replays

/// Lets the player pick a saved game under folder (one subfolder per mansion).
/// Returns 0 and fills pathOut/nameOut, or -1 if cancelled.
int MMDoOpenGameDialog(const char *folder, char *pathOut, char *nameOut)
{
    ListItem *items, *mansions;
    char savedGames[1024], prefix[300];
    int count = 0, numMansions, i, selected = 0, err = -1;

    snprintf(savedGames, sizeof(savedGames), "%s", folder);
    items = AllocListItems();
    mansions = AllocListItems();
    if (items == NULL || mansions == NULL) {
        free(items);
        free(mansions);
        return -1;
    }
    numMansions = AddFolderToList(mansions, 0, savedGames, "", true, false);
    for (i = 0; i < numMansions; i++) {
        snprintf(prefix, sizeof(prefix), "%s: ", mansions[i].name);
        count = AddFolderToList(items, count, mansions[i].path, prefix, false, true);
    }
    count = AddFolderToList(items, count, savedGames, "", false, true);
    qsort(items, count, sizeof(ListItem), CompareListItems);

    if (RunListDialog("Open Saved Game", items, count, &selected, false, "(No saved games.)") == kListChosen) {
        strcpy(pathOut, items[selected].path);
        strcpy(nameOut, items[selected].name);
        err = 0;
    }
    free(items);
    free(mansions);
    return err;
}

/// Asks for a save name (suggesting "<Mansion> <number>") and saves. Returns true if saved.
Boolean MMDoSaveGameDialog(void)
{
    char mansionName[255], saveName[256], folder[1024], path[1300];
    Str255 suggested, number;
    Boolean saved = false;

    strcpy(mansionName, gCurOpenMansionFileName);
    StripStringFromEndOfCString(".zip", mansionName, NULL);
    StripStringFromEndOfCString(".ZIP", mansionName, NULL);
    if (gLastSaveFilename[0] == 0) {
        GetPascalStringFromCString(suggested, mansionName);
        SWConcatPascalStrings(suggested, (ConstStringPtr)"\001 ");
    }
    else {
        GetPascalStringFromCString(suggested, gLastSaveFilename);
        RemoveNumberFromEndOfString(suggested);
    }
    if (!gCurrentlyInMansion)
        SWConcatPascalStrings(suggested, (ConstStringPtr)"\011Completed");
    else {
        gCurrentSaveIndex++;
        SWNumToString(gCurrentSaveIndex, number);
        SWConcatPascalStrings(suggested, number);
    }
    SWCopyPascalStringToCString(suggested, saveName);

    if (IsRelayGame() && MultiSaveFolderForCurrentGame(kMultiSaveCoop) != NULL) {
            // multiplayer: saved for this group of players, not in anyone's own saves
        if (RunTextEntryDialog("Save the team's game as:", saveName, 60, true)) {
            snprintf(gLastSaveFilename, sizeof(gLastSaveFilename), "%s", saveName);
            saved = MultiSaveWrite(kMultiSaveCoop, saveName);
        }
        if (!saved)
            gCurrentSaveIndex--;
        return saved;
    }
    snprintf(folder, sizeof(folder), "%s", GetSavedGamesFilePath(NULL, false));
    MMCreateDirectory(folder, mansionName);
    if (RunTextEntryDialog("Save game as:", saveName, 60, true)) {
        snprintf(path, sizeof(path), "%s%s%s/%s", folder, folder[0] && folder[strlen(folder) - 1] == '/' ? "" : "/",
                 mansionName, saveName);
        snprintf(gLastSaveFilename, sizeof(gLastSaveFilename), "%s", saveName);
        printf("\nPath for saved game file: %s \n", path);
        saved = MMCreateSaveGameFile(path) == 0;
    }
    if (!saved)
        gCurrentSaveIndex--;
    return saved;
}

/// A list entry's mansion name: the file or folder name without ".zip".
static void MansionNameOfItem(const ListItem *item, char *name, size_t size)
{
    snprintf(name, size, "%s", item->name);
    StripStringFromEndOfCString(".zip", name, NULL);
    StripStringFromEndOfCString(".ZIP", name, NULL);
}

/// Every custom mansion: the ones that come with this version, then the player's own folder
/// (a mansion in both is listed once). Labels show the name; beaten ones get a star.
static int GatherCustomMansions(ListItem *items)
{
    char folder[1024], name[256], other[256];
    const char *bundled = GetBundledCustomMansionsPath();
    int count = 0, i, j, kept;

    if (bundled[0] != '\0')
        count = AddFolderToList(items, count, bundled, "", true, true);
    snprintf(folder, sizeof(folder), "%s", GetCustomMansionsFolderPath(""));
    count = AddFolderToList(items, count, folder, "", true, true);
    for (i = 0, kept = 0; i < count; i++) {
        size_t len = strlen(items[i].name);
        struct stat info;
            // mansions are folders or .zip files (a README isn't one)
        if (stat(items[i].path, &info) == 0 && S_ISREG(info.st_mode) &&
            (len < 4 || SDL_strcasecmp(items[i].name + len - 4, ".zip") != 0))
            continue;
        MansionNameOfItem(&items[i], name, sizeof(name));
        for (j = 0; j < kept; j++) {
            MansionNameOfItem(&items[j], other, sizeof(other));
            if (SDL_strcasecmp(name, other) == 0)
                break;
        }
        if (j < kept)
            continue;
        items[kept] = items[i];
        snprintf(items[kept].label, sizeof(items[kept].label), "%s%s", name,
                 HasPlayerBeatenCustomMansion(name) ? "  *" : "");
        kept++;
    }
    qsort(items, kept, sizeof(ListItem), CompareListItems);
    return kept;
}

static int OpenCustomMansionItem(const ListItem *item)
{
    char path[1100];

    snprintf(path, sizeof(path), "%s", item->path);
    StripStringFromEndOfCString(".zip", path, NULL);
    StripStringFromEndOfCString(".ZIP", path, NULL);
    strcat(path, "/");
    printf("\nPath for open mansion file: %s \n", path);
    strcpy(gCustomMansionFolderPath, path);
    return MMOpenCustomMansionUsingPathAndName(path, item->name);
}

/// Lets the player pick a custom mansion (bundled or their own) and opens it. Returns 0 if opened.
int MMDoOpenMansionDialog(void)
{
    ListItem *items;
    int count, selected = 0, err = -1;

    items = AllocListItems();
    if (items == NULL)
        return -1;
    count = GatherCustomMansions(items);
    if (RunListDialog("Custom Mansions", items, count, &selected, false,
                      "(No custom mansions yet. Put them in the Custom Mansions folder.)") == kListChosen)
        err = OpenCustomMansionItem(&items[selected]);
    free(items);
    return err;
}

/// "Random Mansion": dice roll, names flicker past and slow down, and the one it lands on
/// can be played (Okay), or rolled again (Return on "Roll again"). Returns 0 if one was opened.
int MMDoRandomMansionDialog(void)
{
    ListItem *items;
    MMDialog d;
    SpritePtr nameP, hintP;
    SWRect panelRect;
    SWPoint mouse;
    Boolean mouseDown, done = false, rolling = true, chosen = false;
    Uint32 start, nextFlip = 0, interval;
    int count, pick = 0, shown = -1, err = -1;
    short button, width, h;
    char name[256];

    items = AllocListItems();
    if (items == NULL)
        return -1;
    count = GatherCustomMansions(items);
    if (count == 0) {
        free(items);
        MMDoCustomDialog(gSpriteWorldP, "There are no custom mansions to pick from yet.", 3);
        return -1;
    }
    if (!BeginDialog(&d, 0.5f)) {
        EndDialog(&d);
        free(items);
        return -1;
    }
    panelRect = d.panelP->destFrameRect;
    h = (short)(panelRect.left + 30 * d.scale);
    width = (short)(panelRect.right - panelRect.left - 60 * d.scale);
    SetDialogTextCentered(&d, AddDialogText(&d, "Random Mansion", 0.4f, h, (short)(panelRect.top + 24 * d.scale),
                                            width), "Random Mansion");
    nameP = AddDialogText(&d, "", 0.36f, h, (short)(panelRect.top + 80 * d.scale), width);
    hintP = AddDialogText(&d, "", 0.25f, h, (short)(panelRect.top + 125 * d.scale), width);
    AddDialogButtons(&d, true);
    MMPlayExtraSound(kExtraSoundDice);
    start = SDL_GetTicks();
    pick = (int)GetRandom(0, (long)count - 1);

    while (!done && !gQuitGame) {
        button = DialogFrame(&d, &mouse, &mouseDown);
        if (rolling) {
                // flip through names, slowing down, for about as long as the dice sound
            Uint32 elapsed = SDL_GetTicks() - start;
            interval = 40 + elapsed * elapsed / 9000;
            if (SDL_GetTicks() >= nextFlip) {
                shown = count > 1 ? (shown + 1 + (int)GetRandom(0, (long)count - 2)) % count : 0;
                if (elapsed > 1500) {
                    shown = pick;
                    rolling = false;
                    SetDialogTextCentered(&d, hintP, IsControllerConnected() ? "Okay to play, X to roll again."
                                                                              : "Okay to play, R to roll again.");
                }
                MansionNameOfItem(&items[shown], name, sizeof(name));
                SetDialogTextCentered(&d, nameP, name);
                SWSetSpriteColor(nameP, 0xffff, 0xffff, rolling ? 0xffff : 0x2000);
                nextFlip = SDL_GetTicks() + interval;
            }
        }
        if (gTypedText[0] == 'r' || gTypedText[0] == 'R' || (gControllerMenuAlt && !rolling)) {
            rolling = true;
            start = SDL_GetTicks();
            pick = (int)GetRandom(0, (long)count - 1);
            SetDialogTextCentered(&d, hintP, "");
            MMPlayExtraSound(kExtraSoundDice);
        }
        gControllerMenuAlt = false;
        gTypedText[0] = 0;
        if (gKeyEscape || button == kDialogCancel)
            done = true;
        else if (!rolling && (gKeyReturn || button == kDialogOK))
            done = chosen = true;
        gKeyReturn = gKeyEscape = false;
    }
    AnimateOneFrame(gSpriteWorldP);
    EndDialog(&d);
    if (chosen)
        err = OpenCustomMansionItem(&items[pick]);
    free(items);
    return err;
}

// ---------------------------------------------------------------------------
// Trophy Room: the current player's lifetime stats

static void AddStatLine(MMDialog *d, const char *label, const char *value, short h, short v, short valueH)
{
    SpritePtr textP = AddDialogText(d, label, 0.27f, h, v, 0);

    if (textP != NULL)
        SWSetSpriteColor(textP, 0xffff, 0xffff, 0x5fff);
    textP = AddDialogText(d, value, 0.27f, valueH, v, 0);
    if (textP != NULL)
        SWSetSpriteLocation(textP, valueH - GetWidthOfBMText(textP, value), v);
}

void MMDoTrophyRoom(void)
{
    static const char *kDifficulties[3] = { "Easy", "Normal", "Hard" };
    LifetimeStats *s = &gStats[0];
    MMDialog d;
    SWRect panelRect;
    SWPoint mouse;
    Boolean mouseDown, done = false;
    char title[300], value[64];
    short h1, h2, v, row, lineHeight, colWidth, button;
    long totalDeaths = 0;
    int i, m, looted;

    if (!BeginDialog(&d, 0.75f)) {
        EndDialog(&d);
        return;
    }
    panelRect = d.panelP->destFrameRect;
    lineHeight = (short)(19 * d.scale);
    colWidth = (short)((panelRect.right - panelRect.left - 100 * d.scale) / 2);
    h1 = (short)(panelRect.left + 40 * d.scale);
    h2 = (short)(h1 + colWidth + 20 * d.scale);
    if (s->playerName[0] != '\0')
        snprintf(title, sizeof(title), "Trophy Room: %s", s->playerName);
    else
        snprintf(title, sizeof(title), "Trophy Room");
    SetDialogTextCentered(&d, AddDialogText(&d, title, 0.42f, h1, (short)(panelRect.top + 26 * d.scale),
                                            (short)(panelRect.right - panelRect.left - 80 * d.scale)), title);

        // left column: the journey
    v = (short)(panelRect.top + 78 * d.scale);
    row = 0;
    snprintf(value, sizeof(value), "%.2f", PixelsToMiles(s->pixelsFallen));
    AddStatLine(&d, "Miles fallen", value, h1, (short)(v + lineHeight * row++), (short)(h1 + colWidth));
    snprintf(value, sizeof(value), "%ld", s->keysTurned);
    AddStatLine(&d, "Keys turned", value, h1, (short)(v + lineHeight * row++), (short)(h1 + colWidth));
    snprintf(value, sizeof(value), "%ld", s->coinsCollected);
    AddStatLine(&d, "Treasure collected", value, h1, (short)(v + lineHeight * row++), (short)(h1 + colWidth));
    snprintf(value, sizeof(value), "%ld", s->gamesStarted);
    AddStatLine(&d, "Games started", value, h1, (short)(v + lineHeight * row++), (short)(h1 + colWidth));
    snprintf(value, sizeof(value), "%ld", s->mansionsBeaten);
    AddStatLine(&d, "Mansions beaten", value, h1, (short)(v + lineHeight * row++), (short)(h1 + colWidth));
    snprintf(value, sizeof(value), "%ld:%02ld", s->secondsPlayed / 3600, (s->secondsPlayed / 60) % 60);
    AddStatLine(&d, "Time played", value, h1, (short)(v + lineHeight * row++), (short)(h1 + colWidth));
    row++;
    AddDialogText(&d, "Mansions cleaned out", 0.3f, h1, (short)(v + lineHeight * row++), 0);
    for (i = 0; i < 3; i++) {
        for (m = 0, looted = 0; m < 16; m++)
            looted += s->looted[i][m] != 0;
        snprintf(value, sizeof(value), "%d of %d", looted, kNumBaseMansions);
        AddStatLine(&d, kDifficulties[i], value, (short)(h1 + 16 * d.scale), (short)(v + lineHeight * row++),
                    (short)(h1 + colWidth));
    }

        // right column: how Jack died
    row = 0;
    for (i = 0; i < kNumDeathCauses; i++)
        totalDeaths += s->deaths[i];
    AddDialogText(&d, "Deaths", 0.3f, h2, (short)(v + lineHeight * row++), 0);
    for (i = 0; i < kNumDeathCauses; i++) {
        snprintf(value, sizeof(value), "%ld", s->deaths[i]);
        AddStatLine(&d, GetDeathCauseName(i), value, (short)(h2 + 16 * d.scale), (short)(v + lineHeight * row++),
                    (short)(h2 + colWidth));
    }
    snprintf(value, sizeof(value), "%ld", totalDeaths);
    AddStatLine(&d, "Total", value, (short)(h2 + 16 * d.scale), (short)(v + lineHeight * row++),
                (short)(h2 + colWidth));
    if (s->playerName[0] == '\0')
        SetDialogTextCentered(&d, AddDialogText(&d, "Choose a player to start keeping score.", 0.28f, h1,
                                                (short)(panelRect.bottom - 70 * d.scale), 0),
                              "Choose a player to start keeping score.");
    AddDialogButtons(&d, false);

    while (!done && !gQuitGame) {
        button = DialogFrame(&d, &mouse, &mouseDown);
        if (gKeyEscape || gKeyReturn || button == kDialogOK)
            done = true;
        gKeyReturn = gKeyEscape = false;
        gTypedText[0] = 0;
    }
    AnimateOneFrame(gSpriteWorldP);
    EndDialog(&d);
}

/// Lets the player pick a replay and plays it. Returns 0 if it started.
int MMDoOpenMovieDialog(void)
{
    ListItem *items;
    char folder[1024];
    int count, selected = 0, err = -1;

    items = AllocListItems();
    if (items == NULL)
        return -1;
    snprintf(folder, sizeof(folder), "%s", GetMoviesFilePath(NULL));
    count = AddFolderToList(items, 0, folder, "", false, true);
    qsort(items, count, sizeof(ListItem), CompareListItems);
    if (RunListDialog("Watch Replay", items, count, &selected, false, "(No replays recorded yet.)") == kListChosen)
        err = PlayMovieFile(items[selected].path);
    free(items);
    return err;
}
