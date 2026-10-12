/*
 * MyButtonSprite.c — buttons, checkboxes and key input boxes made of sprites.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources - My Utils/MyButtonSprite.c").
 *
 * A button's image has one frame per state (normal, highlighted, pushed, and
 * optionally dimmed). ProcessButtonSprites is called once per frame with the
 * mouse state and returns the index of a button that was clicked (or -1).
 */
#include "MMGame.h"

static SpriteLayerPtr prevButtonFirstSpriteLayerP = NULL;
static SpriteLayerPtr prevButtonLastSpriteLayerP = NULL;
static SpritePtr prevSelectedSpriteP = NULL;
static SpritePtr curHilightedSpriteP = NULL;
static Boolean prevButtonWasDown = false;
static Uint32 gLastProcessButtonsTime = 0;

static const SDL_Keycode gKeyCodeArray[] = {
    SDLK_RETURN, SDLK_ESCAPE, SDLK_BACKSPACE, SDLK_TAB, SDLK_SPACE, SDLK_MINUS, SDLK_EQUALS,
    SDLK_LEFTBRACKET, SDLK_RIGHTBRACKET, SDLK_BACKSLASH, SDLK_SEMICOLON, SDLK_QUOTE, SDLK_BACKQUOTE,
    SDLK_COMMA, SDLK_PERIOD, SDLK_SLASH,
    SDLK_F1, SDLK_F2, SDLK_F3, SDLK_F4, SDLK_F5, SDLK_F6, SDLK_F7, SDLK_F8, SDLK_F9, SDLK_F10, SDLK_F11, SDLK_F12,
    SDLK_PRINTSCREEN, SDLK_SCROLLLOCK, SDLK_PAUSE, SDLK_INSERT, SDLK_HOME, SDLK_PAGEUP, SDLK_DELETE, SDLK_END,
    SDLK_PAGEDOWN, SDLK_RIGHT, SDLK_LEFT, SDLK_DOWN, SDLK_UP, SDLK_NUMLOCKCLEAR,
    SDLK_KP_DIVIDE, SDLK_KP_MULTIPLY, SDLK_KP_MINUS, SDLK_KP_PLUS, SDLK_KP_ENTER,
    SDLK_KP_1, SDLK_KP_2, SDLK_KP_3, SDLK_KP_4, SDLK_KP_5, SDLK_KP_6, SDLK_KP_7, SDLK_KP_8, SDLK_KP_9, SDLK_KP_0,
    SDLK_KP_PERIOD, SDLK_KP_EQUALS,
    SDLK_LCTRL, SDLK_LSHIFT, SDLK_LALT, SDLK_LGUI, SDLK_RCTRL, SDLK_RSHIFT, SDLK_RALT, SDLK_RGUI,
    -1
};

static const char *gKeyCodeTextArray[] = {
    "Return", "Esc", "Delete", "Tab", "Space", "-", "=", "[", "]", "\\", ";", "'", "`", ",", ".", "/",
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
    "Print scrn", "Scroll lock", "Pause", "Ins", "Home", "Pageup", "del", "End", "pagedown",
    "Right arrow", "Left arrow", "Down arrow", "Up arrow", "Num lock",
    "KeyPad /", "KeyPad *", "KeyPad -", "KeyPad +", "Enter",
    "KeyPad 1", "KeyPad 2", "KeyPad 3", "KeyPad 4", "KeyPad 5", "KeyPad 6", "KeyPad 7", "KeyPad 8", "KeyPad 9",
    "KeyPad 0", "KeyPad .", "KeyPad =",
    "Control", "Shift", "Option", "Command", "Control", "Shift", "Option", "Command", "Error"
};

static ButtonSpritePtr AsButton(SpritePtr spriteP)
{
    SW_ASSERT(spriteP->userData == kButtonSpriteSignature);
    return (ButtonSpritePtr)spriteP;
}

// ---------------------------------------------------------------------------
// Loading: the image is split vertically into equal frames

static SWError LoadButtonFramesFromSurface(SpritePtr *spritePP, SDL_Surface *surface, int numFrames, Boolean selfMask)
{
    if (surface == NULL)
        return kSDLCreateSurfaceFromFile;
    return SWCreateSpriteFromSurfaceXY(spritePP, NULL, surface, surface->w, surface->h / numFrames, 0, 0, false,
                                       numFrames, selfMask);
}

static SDL_Surface *LoadSpriteImage(short pictID)
{
    char path[1024];
    snprintf(path, sizeof(path), "%s", GetFileNameForResource(GetSpriteImagePath(NULL), pictID));
    return BKLoadSurface(path);
}

SWError LoadStandardButtonSprite(SpritePtr *spritePP, short pictID, MaskType maskType)
{
    return LoadButtonFramesFromSurface(spritePP, LoadSpriteImage(pictID), 3, maskType != kNoMask);
}

SWError LoadStandardButtonSpriteFromFile(SpritePtr *spritePP, const char *path, Boolean selfMask)
{
    return LoadButtonFramesFromSurface(spritePP, BKLoadSurface(path), 3, selfMask);
}

SWError LoadStandardButtonSpriteMask(SpritePtr *spritePP, short pictID, short maskID, MaskType maskType)
{
    (void)maskID;
    return LoadButtonFramesFromSurface(spritePP, LoadSpriteImage(pictID), 3, maskType != kNoMask);
}

SWError LoadStandardDimmableButtonSprite(SpritePtr *spritePP, short pictID, MaskType maskType)
{
    return LoadButtonFramesFromSurface(spritePP, LoadSpriteImage(pictID), 4, maskType != kNoMask);
}

SWError LoadMiscButtonSprite(SpritePtr *spritePP, short numFrames, short pictID, short maskID, MaskType maskType)
{
    (void)maskID;
    return LoadButtonFramesFromSurface(spritePP, LoadSpriteImage(pictID), numFrames, maskType != kNoMask);
}

SWError LoadMiscButtonSpriteFromFile(SpritePtr *spritePP, short numFrames, const char *path, Boolean selfMask)
{
    return LoadButtonFramesFromSurface(spritePP, BKLoadSurface(path), numFrames, selfMask);
}

/// Makes a button from a loaded master sprite. Returns NULL on failure.
SpritePtr CloneButtonSprite(SpritePtr masterP, short buttonIndex)
{
    ButtonSpritePtr buttonP;
    SpritePtr spriteP = NULL;

    buttonP = (ButtonSpritePtr)calloc(1, sizeof(ButtonSpriteRec));
    if (buttonP == NULL)
        return NULL;
    if (SWCloneSprite(masterP, &spriteP, buttonP, sizeof(ButtonSpriteRec)) != kNoError) {
        free(buttonP);
        return NULL;
    }
    spriteP->userData = kButtonSpriteSignature;
    buttonP->controlType = kButtonControl;
    buttonP->buttonIndex = buttonIndex;
    buttonP->normalFrame = 0;
    buttonP->highlightedFrame = 0;
    buttonP->pushedFrame = 0;
    buttonP->uncheckedFrame = 0;
    buttonP->uncheckedHighlightedFrame = 0;
    buttonP->checkedFrame = 0;
    buttonP->checkedHighlightedFrame = 0;
    buttonP->isChecked = false;
    buttonP->textSpriteP = NULL;
    buttonP->hasColor = false;
    buttonP->hasHighlightedColor = false;
    buttonP->hasPushedColor = false;
    buttonP->clickRadius = -1;
    buttonP->keyBoxIsSelected = false;
    buttonP->isDimmed = false;
    buttonP->pushedOffsetH = 0;
    buttonP->pushedOffsetV = 0;
    buttonP->bounceBaseScale = -1.0f;
    buttonP->linkedCheckboxP = NULL;
    buttonP->linkedScancodeP = NULL;
    buttonP->ignoreCommandKey = true;
    buttonP->ignoreEscapeKey = false;
    buttonP->convertScancodes = true;
    buttonP->buttonProc = NULL;
    return spriteP;
}

void SetButtonSpriteIndex(SpritePtr spriteP, short buttonIndex)
{
    AsButton(spriteP)->buttonIndex = buttonIndex;
}

void SetUpButtonSprite(SpritePtr spriteP, short normalFrame, short highlightedFrame, short pushedFrame,
                       short dimmedFrame)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    buttonP->controlType = kButtonControl;
    buttonP->normalFrame = normalFrame;
    buttonP->highlightedFrame = highlightedFrame;
    buttonP->pushedFrame = pushedFrame;
    buttonP->dimmedFrame = dimmedFrame;
}

void SetUpCheckboxSprite(SpritePtr spriteP, short uncheckedFrame, short uncheckedHighlightedFrame,
                         short checkedFrame, short checkedHighlightedFrame, Boolean checked)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    buttonP->controlType = kCheckboxControl;
    buttonP->uncheckedFrame = uncheckedFrame;
    buttonP->uncheckedHighlightedFrame = uncheckedHighlightedFrame;
    buttonP->checkedFrame = checkedFrame;
    buttonP->checkedHighlightedFrame = checkedHighlightedFrame;
    buttonP->isChecked = checked;
    buttonP->wasChecked = checked;
    SetCorrectCheckboxFrame(spriteP, false);
}

void SetUpKeyInputBox(SpritePtr spriteP, short normalFrame, short selectedFrame)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    buttonP->controlType = kKeyInputBoxControl;
    buttonP->normalFrame = normalFrame;
    buttonP->pushedFrame = selectedFrame;
}

void SetKeyInputBoxKeysToIgnore(SpritePtr spriteP, Boolean ignoreCommandKey, Boolean ignoreEscapeKey)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    buttonP->ignoreCommandKey = ignoreCommandKey;
    buttonP->ignoreEscapeKey = ignoreEscapeKey;
}

void SetCheckboxChecked(SpritePtr spriteP, Boolean checked)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    buttonP->isChecked = checked;
    buttonP->wasChecked = checked;
    SetCorrectCheckboxFrame(spriteP, false);
}

/// Attaches a text sprite (TrueType or bitmap font) that is drawn on the button.
void SetButtonSpriteText(SpritePtr spriteP, SpritePtr textSpriteP)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    SW_ASSERT(textSpriteP->userData == kBMTextSpriteMagic || textSpriteP->userData == kTextSpriteSignature);
    buttonP->textSpriteP = textSpriteP;
    SWSetSpriteDrawProc(spriteP, CustomButtonDrawProc);
    if (buttonP->hasColor)
        ChangeButtonColor(spriteP, buttonP->state);
}

/// The checkbox shows *dataP; UpdateDataFromControlsInLayers writes it back.
void LinkCheckboxWithData(SpritePtr spriteP, Boolean *dataP)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    buttonP->linkedCheckboxP = dataP;
    buttonP->savedCheckboxValue = *dataP;
    SetCheckboxChecked(spriteP, *dataP);
}

static void SetButtonTextSpriteText(ButtonSpritePtr buttonP, const char *text)
{
    if (buttonP->textSpriteP->userData == kBMTextSpriteMagic)
        SetBMTextSpriteText(buttonP->textSpriteP, text);
    else if (buttonP->textSpriteP->userData == kTextSpriteSignature)
        SetTextSpriteText(buttonP->textSpriteP, text);
}

/// The key box shows and edits a key preference (scancode, keycode and name).
void LinkKeyInputBoxWithData(SpritePtr spriteP, long *scancodeP, long *keycodeP, char *nameP)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    buttonP->linkedScancodeP = scancodeP;
    buttonP->savedScancode = *scancodeP;
    buttonP->keyScancode = *scancodeP;
    buttonP->keyKeycode = *keycodeP;
    snprintf(buttonP->keyName, sizeof(buttonP->keyName), "%s", nameP);
    buttonP->linkedKeycodeP = keycodeP;
    buttonP->savedKeycode = *keycodeP;
    buttonP->linkedKeyNameP = nameP;
    if (*nameP == '\0')
        SetButtonTextSpriteText(buttonP, GetTextFromSDLKeyCode(*keycodeP));
    else
        SetButtonTextSpriteText(buttonP, nameP);
}

void UpdateDataFromControlsInLayers(SpriteLayerPtr firstLayerP, SpriteLayerPtr lastLayerP)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP;
    ButtonSpritePtr buttonP;

    for (layerP = firstLayerP; layerP != NULL; layerP = layerP->nextSpriteLayerP) {
        for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP)) {
            if (spriteP->userData != kButtonSpriteSignature)
                continue;
            buttonP = (ButtonSpritePtr)spriteP;
            if (buttonP->controlType == kCheckboxControl && buttonP->linkedCheckboxP != NULL) {
                *buttonP->linkedCheckboxP = buttonP->isChecked;
            } else if (buttonP->controlType == kKeyInputBoxControl && buttonP->linkedScancodeP != NULL) {
                *buttonP->linkedScancodeP = buttonP->keyScancode;
                *buttonP->linkedKeycodeP = buttonP->keyKeycode;
                strcpy(buttonP->linkedKeyNameP, buttonP->keyName);
            }
        }
        if (layerP == lastLayerP)
            break;
    }
}

/// Note: returns true when the value is *unchanged* (the original's behavior).
Boolean HasControlChangedValues(SpritePtr spriteP)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    if (buttonP->controlType == kCheckboxControl && buttonP->linkedCheckboxP != NULL)
        return buttonP->savedCheckboxValue == buttonP->isChecked;
    if (buttonP->controlType == kKeyInputBoxControl && buttonP->linkedScancodeP != NULL)
        return buttonP->savedScancode == buttonP->keyScancode;
    return false;
}

void SetButtonColor(SpritePtr spriteP, unsigned short red, unsigned short green, unsigned short blue)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    buttonP->hasColor = true;
    buttonP->color.red = red;
    buttonP->color.green = green;
    buttonP->color.blue = blue;
    SWSetSpriteColor(spriteP, red, green, blue);
    if (buttonP->textSpriteP != NULL)
        SWSetSpriteColor(buttonP->textSpriteP, red, green, blue);
}

void SetButtonHighlightedColor(SpritePtr spriteP, unsigned short red, unsigned short green, unsigned short blue)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    buttonP->hasHighlightedColor = true;
    buttonP->highlightedColor.red = red;
    buttonP->highlightedColor.green = green;
    buttonP->highlightedColor.blue = blue;
}

void SetButtonPushedColor(SpritePtr spriteP, unsigned short red, unsigned short green, unsigned short blue)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    buttonP->hasPushedColor = true;
    buttonP->pushedColor.red = red;
    buttonP->pushedColor.green = green;
    buttonP->pushedColor.blue = blue;
}

void SetButtonDimmedMode(SpritePtr spriteP, Boolean dimmed)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    if (!dimmed) {
        if (buttonP->isDimmed == true) {
            SWSetCurrentFrameIndex(spriteP, buttonP->normalFrame);
            buttonP->isDimmed = false;
        }
    } else if (!buttonP->isDimmed) {
        SWSetCurrentFrameIndex(spriteP, buttonP->dimmedFrame);
        buttonP->isDimmed = true;
    }
}

/// Clicks only count within radius pixels of the button's center (-1: anywhere on it).
void SetButtonClickRadius(SpritePtr spriteP, short radius)
{
    AsButton(spriteP)->clickRadius = radius;
}

short GetButtonIndex(SpritePtr spriteP)
{
    return AsButton(spriteP)->buttonIndex;
}

void InstallButtonProc(SpritePtr spriteP, ButtonProcPtr buttonProc)
{
    AsButton(spriteP)->buttonProc = buttonProc;
}

/// How far the button moves while pushed.
void SetButtonPushedOffset(SpritePtr spriteP, short h, short v)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    buttonP->pushedOffsetH = h;
    buttonP->pushedOffsetV = v;
}

void AdjustCollisionRectForOffset(SpritePtr spriteP)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    SWSetFrameCollisionInset(spriteP->frameArray[buttonP->normalFrame], buttonP->pushedOffsetH,
                             buttonP->pushedOffsetV, 0, 0);
}

// ---------------------------------------------------------------------------
// Tracking

static void CallButtonProc(ButtonSpritePtr buttonP, short oldState)
{
    if (buttonP->buttonProc != NULL)
        buttonP->buttonProc(&buttonP->sprite, buttonP->buttonIndex, buttonP->state, oldState);
}

static void UndoPushedOffset(SpritePtr spriteP)
{
    ButtonSpritePtr buttonP = (ButtonSpritePtr)spriteP;
    SWOffsetSprite(spriteP, -buttonP->pushedOffsetH, -buttonP->pushedOffsetV);
}

/**
 * Updates every button in layers firstLayerP..lastLayerP for the mouse at point.
 * Returns the index of a button released over itself (a click), else -1;
 * *clickedSpritePP gets that button. With useMask, the mouse must be over a
 * visible pixel.
 */
short ProcessButtonSprites(SpriteLayerPtr firstLayerP, SpriteLayerPtr lastLayerP, SWPoint point,
                           Boolean mouseDown, Boolean useMask, SpritePtr *clickedSpritePP)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP;
    ButtonSpritePtr buttonP;
    SWBoolean isInside;
    short clickedIndex = -1;
    short centerH, centerV, dh, dv, distance;

    prevButtonFirstSpriteLayerP = firstLayerP;
    prevButtonLastSpriteLayerP = lastLayerP;
    gLastProcessButtonsTime = SDL_GetTicks();
    if (clickedSpritePP != NULL)
        *clickedSpritePP = NULL;
    curHilightedSpriteP = NULL;

    for (layerP = firstLayerP; layerP != NULL; layerP = layerP->nextSpriteLayerP) {
        if (SWGetSpriteLayerVisible(layerP)) {
            for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP)) {
                if (spriteP->userData != kButtonSpriteSignature)
                    continue;
                buttonP = (ButtonSpritePtr)spriteP;

                    // checkboxes with a label use their whole rect
                if (!useMask || (buttonP->controlType == kCheckboxControl && buttonP->textSpriteP != NULL))
                    isInside = SWIsPointInSprite(spriteP, point);
                else
                    isInside = SWIsPointInSpriteMask(spriteP, point.h, point.v, SWInPixelMask);

                if (isInside && buttonP->clickRadius != -1) {
                    centerH = (short)(int)(SWGetSpriteWidth(spriteP) / 2.0f + SWGetSpriteHorizLoc(spriteP));
                    centerV = (short)(int)(SWGetSpriteHeight(spriteP) / 2.0f + SWGetSpriteVertLoc(spriteP));
                    dh = centerH - point.h;
                    dv = centerV - point.v;
                    distance = (short)(int)sqrt((double)(dh * dh + dv * dv));
                    if (distance > buttonP->clickRadius)
                        isInside = false;
                }

                if (buttonP->controlType == kButtonControl)
                    ProcessNormalButtonSprite(layerP, spriteP, isInside, mouseDown, &clickedIndex, clickedSpritePP);
                else if (buttonP->controlType == kCheckboxControl)
                    ProcessCheckBoxSprite(layerP, spriteP, isInside, mouseDown, &clickedIndex, clickedSpritePP);
                else if (buttonP->controlType == kKeyInputBoxControl)
                    ProcessKeyInputBoxSprite(layerP, spriteP, isInside, mouseDown, &clickedIndex, clickedSpritePP);
                else
                    SW_ASSERT(false);
            }
        }
        if (layerP == lastLayerP)
            break;
    }

    if (!mouseDown)
        prevSelectedSpriteP = NULL;
    prevButtonWasDown = mouseDown;
    return clickedIndex;
}

/// The enabled, visible buttons of the screen that is currently processing its buttons (for
/// moving a controller highlight between them). Returns how many were stored.
int GetActiveButtonSprites(SpritePtr *buttons, int max)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP;
    int count = 0;

    if (prevButtonFirstSpriteLayerP == NULL || SDL_GetTicks() - gLastProcessButtonsTime > 200)
        return 0;
    for (layerP = prevButtonFirstSpriteLayerP; layerP != NULL && count < max; layerP = layerP->nextSpriteLayerP) {
        if (SWGetSpriteLayerVisible(layerP)) {
            for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL && count < max;
                 spriteP = SWGetNextSprite(layerP, spriteP)) {
                if (spriteP->userData != kButtonSpriteSignature || !spriteP->isVisible ||
                    ((ButtonSpritePtr)spriteP)->isDimmed)
                    continue;
                buttons[count++] = spriteP;
            }
        }
        if (layerP == prevButtonLastSpriteLayerP)
            break;
    }
    return count;
}

/// Mouse is down over the button: push it, if the press started on it.
static Boolean StartPushIfTracking(SpritePtr spriteP)
{
    ButtonSpritePtr buttonP = (ButtonSpritePtr)spriteP;
    if (((prevSelectedSpriteP == NULL && !prevButtonWasDown) || spriteP == prevSelectedSpriteP)) {
        prevSelectedSpriteP = spriteP;
        if (buttonP->state != kButtonPushed)
            return true;
    }
    return false;
}

void ProcessNormalButtonSprite(SpriteLayerPtr layerP, SpritePtr spriteP, Boolean isInside, Boolean mouseDown,
                               short *clickedIndexP, SpritePtr *clickedSpritePP)
{
    ButtonSpritePtr buttonP = (ButtonSpritePtr)spriteP;
    short oldState;

    if (buttonP->isDimmed)
        return;

    if (!isInside) {
        oldState = buttonP->state;
        buttonP->state = kButtonNormal;
        if (spriteP->curFrameIndex != buttonP->normalFrame)
            SWSetCurrentFrameIndex(spriteP, buttonP->normalFrame);
        if (buttonP->state != oldState)
            ChangeButtonColor(spriteP, buttonP->state);
        CallButtonProc(buttonP, oldState);
        if (oldState == kButtonPushed)
            UndoPushedOffset(spriteP);
    } else if (!mouseDown) {
        if (curHilightedSpriteP == NULL) {
            curHilightedSpriteP = spriteP;
            UnhighlightAllOtherButtonSprites(layerP, spriteP, kButtonControl);
            oldState = buttonP->state;
            buttonP->state = kButtonHighlighted;
            if (spriteP->curFrameIndex != buttonP->highlightedFrame)
                SWSetCurrentFrameIndex(spriteP, buttonP->highlightedFrame);
            if (buttonP->state != oldState)
                ChangeButtonColor(spriteP, buttonP->state);
            CallButtonProc(buttonP, oldState);
                // released while pushed: that's a click
            if (oldState == kButtonPushed) {
                UndoPushedOffset(spriteP);
                *clickedIndexP = buttonP->buttonIndex;
                if (clickedSpritePP != NULL)
                    *clickedSpritePP = spriteP;
            }
        }
    } else {
        oldState = buttonP->state;
        if (StartPushIfTracking(spriteP)) {
            oldState = buttonP->state;
            buttonP->state = kButtonPushed;
            if (spriteP->curFrameIndex != buttonP->pushedFrame)
                SWSetCurrentFrameIndex(spriteP, buttonP->pushedFrame);
            SWOffsetSprite(spriteP, buttonP->pushedOffsetH, buttonP->pushedOffsetV);
        }
        if (buttonP->state != oldState)
            ChangeButtonColor(spriteP, buttonP->state);
        CallButtonProc(buttonP, oldState);
    }
}

void ProcessCheckBoxSprite(SpriteLayerPtr layerP, SpritePtr spriteP, Boolean isInside, Boolean mouseDown,
                           short *clickedIndexP, SpritePtr *clickedSpritePP)
{
    ButtonSpritePtr buttonP = (ButtonSpritePtr)spriteP;
    short oldState;

    if (!isInside) {
        oldState = buttonP->state;
        buttonP->state = kButtonNormal;
        SetCorrectCheckboxFrame(spriteP, false);
        buttonP->wasChecked = buttonP->isChecked;
        if (buttonP->state != oldState)
            ChangeButtonColor(spriteP, buttonP->state);
        CallButtonProc(buttonP, oldState);
        if (oldState == kButtonPushed)
            UndoPushedOffset(spriteP);
    } else if (!mouseDown) {
        if (curHilightedSpriteP == NULL) {
            curHilightedSpriteP = spriteP;
            UnhighlightAllOtherButtonSprites(layerP, spriteP, kButtonControl);
            oldState = buttonP->state;
            buttonP->state = kButtonHighlighted;
            buttonP->isChecked = buttonP->wasChecked;
            SetCorrectCheckboxFrame(spriteP, true);
            if (buttonP->state != oldState)
                ChangeButtonColor(spriteP, buttonP->state);
            CallButtonProc(buttonP, oldState);
            if (oldState == kButtonPushed) {
                UndoPushedOffset(spriteP);
                *clickedIndexP = buttonP->buttonIndex;
                if (clickedSpritePP != NULL)
                    *clickedSpritePP = spriteP;
            }
        }
    } else {
        oldState = buttonP->state;
        if (StartPushIfTracking(spriteP)) {
            oldState = buttonP->state;
            buttonP->state = kButtonPushed;
                // show the new value while pushed; it sticks when released over the box
            buttonP->isChecked = !buttonP->isChecked;
            SetCorrectCheckboxFrame(spriteP, true);
            buttonP->wasChecked = buttonP->isChecked;
            buttonP->isChecked = !buttonP->isChecked;
            SWOffsetSprite(spriteP, buttonP->pushedOffsetH, buttonP->pushedOffsetV);
        }
        if (buttonP->state != oldState)
            ChangeButtonColor(spriteP, buttonP->state);
        CallButtonProc(buttonP, oldState);
    }
}

void UnhighlightAllOtherButtonSprites(SpriteLayerPtr layerP, SpritePtr exceptSpriteP, short controlType)
{
    SpritePtr spriteP;
    for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP)) {
        if (spriteP != exceptSpriteP && spriteP->userData == kButtonSpriteSignature &&
            ((ButtonSpritePtr)spriteP)->controlType == controlType)
            DeselectButton(spriteP);
    }
}

void DeselectButton(SpritePtr spriteP)
{
    ButtonSpritePtr buttonP;
    short oldState;

    if (spriteP == NULL)
        return;
    buttonP = AsButton(spriteP);
    if (buttonP->isDimmed)
        return;
    oldState = buttonP->state;
    buttonP->state = kButtonNormal;
    if (buttonP->controlType == kKeyInputBoxControl)
        SetKeyInputBoxSelected(spriteP, false);
    if (spriteP->curFrameIndex != buttonP->normalFrame)
        SWSetCurrentFrameIndex(spriteP, buttonP->normalFrame);
    if (buttonP->state != oldState)
        ChangeButtonColor(spriteP, buttonP->state);
    CallButtonProc(buttonP, oldState);
}

/// Applies the color for the state (2 pushed, 1 highlighted, else normal) to the
/// button and its text.
void ChangeButtonColor(SpritePtr spriteP, short state)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    SWRGBColor *colorP = NULL;

    if (state == kButtonPushed) {
        if (buttonP->hasPushedColor)
            colorP = &buttonP->pushedColor;
    } else if (state == kButtonHighlighted) {
        if (buttonP->hasHighlightedColor)
            colorP = &buttonP->highlightedColor;
    } else if (buttonP->hasColor) {
        colorP = &buttonP->color;
    }
    if (colorP == NULL)
        return;
    SWSetSpriteColor(spriteP, colorP->red, colorP->green, colorP->blue);
    if (buttonP->textSpriteP != NULL)
        SWSetSpriteColor(buttonP->textSpriteP, colorP->red, colorP->green, colorP->blue);
}

void SetCorrectCheckboxFrame(SpritePtr spriteP, Boolean highlighted)
{
    ButtonSpritePtr buttonP = (ButtonSpritePtr)spriteP;
    if (!highlighted)
        SWSetCurrentFrameIndex(spriteP, buttonP->isChecked ? buttonP->checkedFrame : buttonP->uncheckedFrame);
    else
        SWSetCurrentFrameIndex(spriteP, buttonP->isChecked ? buttonP->checkedHighlightedFrame
                                                           : buttonP->uncheckedHighlightedFrame);
}

void DisposeButtonSpritesMatchingIndex(SpriteLayerPtr layerP, short buttonIndex)
{
    SpritePtr spriteP, nextP;
    SW_ASSERT(layerP != NULL);
    spriteP = SWGetNextSprite(layerP, NULL);
    while (spriteP != NULL) {
        nextP = SWGetNextSprite(layerP, spriteP);
        if (spriteP->userData == kButtonSpriteSignature && ((ButtonSpritePtr)spriteP)->buttonIndex == buttonIndex)
            SWRemoveSpriteFromAnimation(spriteP, true);
        spriteP = nextP;
    }
}

void ProcessKeyInputBoxSprite(SpriteLayerPtr layerP, SpritePtr spriteP, Boolean isInside, Boolean mouseDown,
                              short *clickedIndexP, SpritePtr *clickedSpritePP)
{
    ButtonSpritePtr buttonP = (ButtonSpritePtr)spriteP;
    short oldState;

    if (buttonP->isDimmed)
        return;

    if (!isInside) {
        if (!buttonP->keyBoxIsSelected) {
            oldState = buttonP->state;
            buttonP->state = kButtonNormal;
            if (buttonP->state != oldState)
                ChangeButtonColor(spriteP, buttonP->state);
            CallButtonProc(buttonP, oldState);
            if (oldState == kButtonPushed)
                UndoPushedOffset(spriteP);
        }
    } else if (!mouseDown) {
        if (curHilightedSpriteP == NULL) {
            curHilightedSpriteP = spriteP;
            UnhighlightAllOtherButtonSprites(layerP, spriteP, kButtonControl);
            oldState = buttonP->state;
            buttonP->state = kButtonHighlighted;
            if (buttonP->state != oldState)
                ChangeButtonColor(spriteP, buttonP->state);
            CallButtonProc(buttonP, oldState);
            if (oldState == kButtonPushed) {
                UndoPushedOffset(spriteP);
                    // clicked: this box now waits for a key
                UnhighlightAllOtherButtonSprites(layerP, spriteP, kKeyInputBoxControl);
                SetKeyInputBoxSelected(spriteP, true);
                *clickedIndexP = buttonP->buttonIndex;
                if (clickedSpritePP != NULL)
                    *clickedSpritePP = spriteP;
            }
        }
    } else {
        oldState = buttonP->state;
        if (StartPushIfTracking(spriteP)) {
            oldState = buttonP->state;
            buttonP->state = kButtonPushed;
            SWOffsetSprite(spriteP, buttonP->pushedOffsetH, buttonP->pushedOffsetV);
        }
        if (buttonP->state != oldState)
            ChangeButtonColor(spriteP, buttonP->state);
        CallButtonProc(buttonP, oldState);
    }
}

/// Gives a key-down event to the selected key input box. Returns true if it took it.
Boolean HandleEventInKeyInputBox(SDL_Event *event)
{
    SpritePtr spriteP;
    ButtonSpritePtr buttonP;
    SDL_Event pending[20];
    char name[50];
    int scancode;
    int numEvents;

    if (prevButtonFirstSpriteLayerP == NULL || event->type != SDL_KEYDOWN)
        return false;
    spriteP = GetCurrentlySelectedKeyInputBox();
    if (spriteP == NULL)
        return false;
    buttonP = (ButtonSpritePtr)spriteP;

    scancode = buttonP->convertScancodes ? ConvertKeyScanCode(event->key.keysym.scancode)
                                         : event->key.keysym.scancode;
        // caps lock can't be used; Cmd and Esc only if allowed
    if (scancode != SDL_SCANCODE_CAPSLOCK &&
        (!buttonP->ignoreCommandKey || scancode != SDL_SCANCODE_LGUI) &&
        (!buttonP->ignoreEscapeKey || scancode != SDL_SCANCODE_ESCAPE)) {
        buttonP->keyScancode = scancode;
        buttonP->keyKeycode = event->key.keysym.sym;
        snprintf(name, sizeof(name), "%s", ConvertKeyName(SDL_GetKeyName(event->key.keysym.sym)));
        SetButtonTextSpriteText(buttonP, name);
        snprintf(buttonP->keyName, sizeof(buttonP->keyName), "%s", name);
        numEvents = SDL_PeepEvents(pending, 20, SDL_PEEKEVENT, SDL_KEYDOWN, SDL_KEYUP);
        printf("numEvents left in queue: %d\n", numEvents);
        if (numEvents == 0)
            SetKeyInputBoxSelected(spriteP, false);
    }
    return true;
}

Boolean DoesEventExist(Uint32 eventType)
{
    SDL_Event events[20];
    SDL_PumpEvents();
    return SDL_PeepEvents(events, 20, SDL_PEEKEVENT, eventType, eventType) > 0;
}

SpritePtr GetCurrentlySelectedKeyInputBox(void)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP;

    for (layerP = prevButtonFirstSpriteLayerP; layerP != NULL; layerP = layerP->nextSpriteLayerP) {
        for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP)) {
            if (spriteP->userData == kButtonSpriteSignature &&
                ((ButtonSpritePtr)spriteP)->controlType == kKeyInputBoxControl &&
                ((ButtonSpritePtr)spriteP)->keyBoxIsSelected)
                return spriteP;
        }
        if (layerP == prevButtonLastSpriteLayerP)
            break;
    }
    return NULL;
}

/// Shorter names for the modifier keys, as on a Mac keyboard.
const char *ConvertKeyName(const char *name)
{
    if (strstr(name, "Left Shift") != NULL || strstr(name, "Right Shift") != NULL)
        return "Shift";
    if (strstr(name, "Left Ctrl") != NULL || strstr(name, "Right Ctrl") != NULL)
        return "Control";
    if (strstr(name, "Left Option") != NULL || strstr(name, "Right Option") != NULL)
        return "Option";
    if (strstr(name, "Left Alt") != NULL || strstr(name, "Right Alt") != NULL)
        return "Alt";
    return name;
}

void SetKeyInputBoxSelected(SpritePtr spriteP, Boolean selected)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    SWSetCurrentFrameIndex(spriteP, selected ? buttonP->pushedFrame : buttonP->normalFrame);
    buttonP->keyBoxIsSelected = selected;
}

/// Draws the button, then its text: right of a checkbox, centered in a key box.
void CustomButtonDrawProc(SpritePtr spriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    ButtonSpritePtr buttonP = AsButton(spriteP);
    SpritePtr textP = buttonP->textSpriteP;
    short textWidth;

    SW_ASSERT(textP != NULL);
    SW_ASSERT(textP->userData == kBMTextSpriteMagic || textP->userData == kTextSpriteSignature);

    if (textP->userData == kBMTextSpriteMagic)
        textWidth = (short)(int)GetWidthOfBMText(textP, GetTextSpriteText(textP));
    else
        textWidth = (short)GetTextSpriteWidth(textP);

    if (buttonP->controlType == kCheckboxControl) {
            // the label counts as part of the checkbox for clicks
        spriteP->destFrameRect.right = spriteP->destFrameRect.left +
                                       (short)(int)((float)textWidth + SWGetSpriteWidth(spriteP));
        SWStdSpriteDrawProc(spriteP, curFrameP, horizLoc, vertLoc);
        BMTextSpriteDrawProc(textP, curFrameP, SWGetSpriteWidth(spriteP) + horizLoc + 3.0f,
                             SWGetSpriteHeight(spriteP) * 0.17f + vertLoc);
    } else if (buttonP->controlType == kKeyInputBoxControl) {
        SWStdSpriteDrawProc(spriteP, curFrameP, horizLoc, vertLoc);
        if (textP->userData == kBMTextSpriteMagic) {
            BMTextSpriteDrawProc(textP, curFrameP,
                                 (SWGetSpriteWidth(spriteP) / 2.0f + horizLoc) - (float)(textWidth / 2),
                                 SWGetSpriteHeight(spriteP) / 2.0f + vertLoc + SWGetSpriteHeight(textP) / -2.0f);
        } else {
            short fontSize = ((TextSpritePtr)textP)->fontSize;
            SWSetSpriteLocation(textP, (SWGetSpriteWidth(spriteP) / 2.0f + horizLoc) - (float)(textWidth / 2),
                                (float)(fontSize / 2) + vertLoc + SWGetSpriteHeight(spriteP) / 2.5f);
            SWTextSpriteDrawProc(textP, curFrameP, 0, 0);
        }
    } else {
        SW_ASSERT(false);
    }
}

/// A short name for a key, for the controls settings.
const char *GetTextFromSDLKeyCode(SDL_Keycode keycode)
{
    static char charCString[2];
    int i;

    charCString[0] = '\0';
    if (keycode >= 'a' && keycode <= 'z') {
        charCString[0] = (char)(keycode - 0x20);
        charCString[1] = '\0';
        return charCString;
    }
    if (keycode >= '1' && keycode <= '9') {
        charCString[0] = (char)keycode;
        charCString[1] = '\0';
        return charCString;
    }
    if (keycode == '0')
        return "0";
    for (i = 0; gKeyCodeArray[i] != -1; i++) {
        if (gKeyCodeArray[i] == keycode)
            return gKeyCodeTextArray[i];
    }
    return "Unknown";
}
