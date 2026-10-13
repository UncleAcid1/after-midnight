/*
 * MMController.c — game controllers (SDL GameController: Xbox, PlayStation, Switch and
 * most others), added by this version.
 *
 * In play the left stick / d-pad move, A (Cross) jumps, X (Square) is the action button,
 * Y (Triangle) or Back shows the map, Start pauses and the shoulders zoom the map.
 * In menus the d-pad / stick moves a highlight between the on-screen buttons (the mouse
 * cursor is hidden and the game sees the highlighted button as pointed at), A presses it
 * and B goes back.
 */

#include "MMGame.h"
#include "MMProtos.h"

#define kStickDeadZone      12000
#define kMenuRepeatDelay    380     // ms before a held direction repeats
#define kMenuRepeatRate     110

static SDL_GameController *gControllers[4];
static SDL_JoystickID gPretendIDs[4];       // MM_DEBUG_VIRTUAL_PADS's controllers
static int gNumPretend = 0;
static int gNumControllers = 0;
static int gMenuPresses = 0;
static Boolean gPadHeld[16];
static Uint32 gRepeatAt[4];
static int gHeldDir = 0;
static SDL_JoystickID gLastPadID = -1;

static Boolean gVirtualActive = false;
static SWPoint gVirtualPoint;
static Boolean gVirtualButton = false;
static SpritePtr gFocusP = NULL;
Boolean gControllerMenuAlt = false;
Boolean gControllerMenuAlt2 = false;
int gControllerTabStep = 0;     // L1 / R1 outside a list: -1 / +1 (the Options dialog's tabs)
static Boolean gListMode = false;

/// In a scrolling list, up/down (and the shoulders) move through the list as the arrow
/// (and page) keys do; left/right still move between the buttons.
void ControllerSetListMode(Boolean on)
{
    gListMode = on;
    if (on)
        gFocusP = NULL;         // a new list: the highlight starts on Okay (button index 0)
}

Boolean IsControllerConnected(void)
{
    return gNumControllers > 0;
}

int GetNumControllers(void)
{
    return gNumControllers;
}

void InitControllers(void)
{
    const char *virtualPads = getenv("MM_DEBUG_VIRTUAL_PADS");
    int i;

    if (!SDL_WasInit(SDL_INIT_GAMECONTROLLER))
        SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);
        // testing aid: MM_DEBUG_VIRTUAL_PADS=<n> plugs in n pretend controllers (DebugVirtualPadKey)
    for (i = 0; virtualPads != NULL && i < atoi(virtualPads) && i < 4; i++) {
        int device = SDL_JoystickAttachVirtual(SDL_JOYSTICK_TYPE_GAMECONTROLLER, SDL_CONTROLLER_AXIS_MAX,
                                               SDL_CONTROLLER_BUTTON_MAX, 0);
        if (device >= 0)
            gPretendIDs[gNumPretend++] = SDL_JoystickGetDeviceInstanceID(device);
    }
    for (i = 0; i < SDL_NumJoysticks() && gNumControllers < 4; i++) {
        if (SDL_IsGameController(i)) {
            SDL_GameController *pad = SDL_GameControllerOpen(i);
            if (pad != NULL)
                gControllers[gNumControllers++] = pad;
        }
    }
    if (gSpriteWorldP != NULL)
        SWSetPostDrawCallBack(gSpriteWorldP, ExtrasPostDraw);
}

static Boolean IsPretend(SDL_GameController *pad)
{
    SDL_JoystickID id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(pad));
    int i;

    for (i = 0; i < gNumPretend; i++)
        if (gPretendIDs[i] == id)
            return true;
    return false;
}

/// A controller plugged in. A real one goes before the pretend ones (MM_DEBUG_VIRTUAL_PADS),
/// taking the last one's place when all four are in use: the pretend ones fill idle players.
static void AddController(int deviceIndex)
{
    SDL_GameController *pad;
    int i, at;

    if (!SDL_IsGameController(deviceIndex))
        return;
    pad = SDL_GameControllerOpen(deviceIndex);
    if (pad == NULL)
        return;
    for (i = 0; i < gNumControllers; i++)
        if (gControllers[i] == pad)
            return;
    if (IsPretend(pad)) {
        if (gNumControllers < 4)
            gControllers[gNumControllers++] = pad;
        return;
    }
    for (at = 0; at < gNumControllers && !IsPretend(gControllers[at]); at++)
        ;
    if (gNumControllers >= 4) {
        if (at >= 4)
            return;                     // four real ones already
        gNumControllers--;              // the last pretend one makes room
    }
    for (i = gNumControllers; i > at; i--)
        gControllers[i] = gControllers[i - 1];
    gControllers[at] = pad;
    gNumControllers++;
}

static void RemoveController(SDL_JoystickID id)
{
    int i, j;

    for (i = 0; i < gNumControllers; i++) {
        if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gControllers[i])) == id) {
            SDL_GameControllerClose(gControllers[i]);
            for (j = i; j < gNumControllers - 1; j++)
                gControllers[j] = gControllers[j + 1];
            gNumControllers--;
            break;
        }
    }
    if (gNumControllers == 0)
        gVirtualActive = gControllerHidesCursor = false;
}

void Rumble(int kind)
{
    static const struct { Uint16 low, high; Uint32 ms; } kinds[] = {
        { 0x0000, 0x5000, 60 },     // tap: high-frequency motor only
        { 0xd000, 0x2000, 420 },    // heavy: low rumble
        { 0xffff, 0xffff, 160 },    // jolt
    };
    int i;

    if (!gExtras.rumble || kind < 0 || kind > 2 || NetIsFollower())     // only for the player who has the turn
        return;
    for (i = 0; i < gNumControllers; i++) {
        if (gLastPadID != -1 &&
            SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gControllers[i])) != gLastPadID)
            continue;
        SDL_GameControllerRumble(gControllers[i], kinds[kind].low, kinds[kind].high, kinds[kind].ms);
    }
}

// ---------------------------------------------------------------------------
// Input

static SDL_Event gPendingKeyUp;
static Uint32 gPendingKeyUpAt;

/// Split screen: into the running player's own queue (SDL's queue goes to the keyboard's player).
static void PostEvent(SDL_Event *e)
{
    if (!SplitPostEvent(e))
        SDL_PushEvent(e);
}

/// A key press, released a little later (ControllerFrame): screens that look at whether a
/// key is down once a frame (Escape on most of them) missed a press released at once.
static void PushKey(SDL_Keycode sym, SDL_Scancode scancode)
{
    SDL_Event e;

    if (gPendingKeyUpAt != 0) {         // the previous key goes up first
        PostEvent(&gPendingKeyUp);
        gPendingKeyUpAt = 0;
    }
    memset(&e, 0, sizeof(e));
    e.type = SDL_KEYDOWN;
    e.key.state = SDL_PRESSED;
    e.key.keysym.sym = sym;
    e.key.keysym.scancode = scancode;
    e.key.windowID = SDL_GetWindowID(gSpriteWorldP->mainWindow);
    PostEvent(&e);
    e.type = SDL_KEYUP;
    e.key.state = SDL_RELEASED;
    gPendingKeyUp = e;
    gPendingKeyUpAt = SDL_GetTicks() + 50;
    if (gPendingKeyUpAt == 0)
        gPendingKeyUpAt = 1;
}

/// Play (and the in-game map) read the movement keys directly; menus get highlight moves.
static Boolean IsInGameplay(void)
{
    return (gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kInGameMapMode) && !gGameIsPaused &&
           !gCustomDialogIsVisible && !gDialogBoxIsOpen && !HasMMEventCallback();
}

static void SetMovementFromPad(void)
{
    Boolean left = gPadHeld[SDL_CONTROLLER_BUTTON_DPAD_LEFT] || (gHeldDir & kPadLeft);
    Boolean right = gPadHeld[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] || (gHeldDir & kPadRight);
    Boolean up = gPadHeld[SDL_CONTROLLER_BUTTON_DPAD_UP] || (gHeldDir & kPadUp);
    Boolean down = gPadHeld[SDL_CONTROLLER_BUTTON_DPAD_DOWN] || (gHeldDir & kPadDown);
    SpritePtr heroP;
    Boolean onGround;

    if (IsMoviePlaying() || NetIsFollower())
        return;
    gGamepad[kKeyLeft] = left;
    gGamepad[kKeyRight] = right;
    gGamepad[kKeyUp] = up;
    gGamepad[kKeyDown] = down;
    gKeys[kKeyLeft] = left;
    gKeys[kKeyRight] = right;
    gKeys[kKeyUp] = up;
        // as with the original gamepad code: a diagonal while walking doesn't crouch
    heroP = GetHeroSprite();
    onGround = heroP == NULL ? true : HERO(heroP)->onGround;
    gKeys[kKeyDown] = ((!left && !right) || gCurrentGameMode == kInGameMapMode || !onGround) ? down : false;
}

static int DirectionFromStick(Sint16 x, Sint16 y)
{
    int dir = 0;
    if (x < -kStickDeadZone) dir |= kPadLeft;
    if (x > kStickDeadZone)  dir |= kPadRight;
    if (y < -kStickDeadZone) dir |= kPadUp;
    if (y > kStickDeadZone)  dir |= kPadDown;
    return dir;
}

static void MenuPress(int bits)
{
    gMenuPresses |= bits;
    gVirtualActive = true;
    gControllerHidesCursor = true;
}

static void HandlePadButton(int button, Boolean down)
{
    static const int dpadBits[] = { kPadUp, kPadDown, kPadLeft, kPadRight };
    Boolean gameplay = IsInGameplay();

    if (button < 0 || button >= 16)
        return;
    gPadHeld[button] = down;

        // a sign: any key closes it, and so does any button (as Return; nothing reads menu
        // presses while it's open, and they would be left over for the next screen)
    if (gCurrentGameMode == kReadingSignMode && !gGameIsPaused && !gCustomDialogIsVisible) {
        if (down && (button == SDL_CONTROLLER_BUTTON_A || button == SDL_CONTROLLER_BUTTON_B ||
                     button == SDL_CONTROLLER_BUTTON_X || button == SDL_CONTROLLER_BUTTON_Y ||
                     button == SDL_CONTROLLER_BUTTON_START))
            PushKey(SDLK_RETURN, SDL_SCANCODE_RETURN);
        return;
    }

    if (button >= SDL_CONTROLLER_BUTTON_DPAD_UP && button <= SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
        if (gameplay) {
            SetMovementFromPad();
        } else if (down) {
            MenuPress(dpadBits[button - SDL_CONTROLLER_BUTTON_DPAD_UP]);
            gRepeatAt[button - SDL_CONTROLLER_BUTTON_DPAD_UP] = SDL_GetTicks() + kMenuRepeatDelay;
        }
        return;
    }

    if (gameplay && NetIsFollower()) {
            // two computers, the other player's turn: Start offers to leave
        if (down && button == SDL_CONTROLLER_BUTTON_START)
            PushKey(SDLK_ESCAPE, SDL_SCANCODE_ESCAPE);
        return;
    }
    if (gameplay) {
        switch (button) {
            case SDL_CONTROLLER_BUTTON_A:
                if (gCurrentGameMode == kInGameMapMode) {
                    if (down && !gIsEnteringMapScreen)
                        ExitInGameMapScreen(true);
                } else {
                    UpdateMovementKeys(gKeyPrefs.primaryKeys[4], gKeyPrefs.secondaryKeys[4], down, false);
                }
                break;
            case SDL_CONTROLLER_BUTTON_X:
            case SDL_CONTROLLER_BUTTON_B:
                if (gCurrentGameMode == kInGameMapMode) {
                    if (down && !gIsEnteringMapScreen)
                        ExitInGameMapScreen(true);
                } else {
                    UpdateMovementKeys(gKeyPrefs.primaryKeys[7], gKeyPrefs.secondaryKeys[7], down, false);
                }
                break;
            case SDL_CONTROLLER_BUTTON_Y:
            case SDL_CONTROLLER_BUTTON_BACK:
                if (down) {
                    if (gCurrentGameMode == kInGameMapMode && !gIsEnteringMapScreen)
                        ExitInGameMapScreen(true);
                    else if (gCurrentGameMode == kPlayingGameMode)
                        SetUpInGameMapScreen();
                }
                break;
            case SDL_CONTROLLER_BUTTON_START:
                if (down)
                    PushKey(SDLK_ESCAPE, SDL_SCANCODE_ESCAPE);
                break;
            case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                gKeys[kKeyZoomOut] = down && gCurrentGameMode == kInGameMapMode;
                break;
            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                gKeys[kKeyZoomIn] = down && gCurrentGameMode == kInGameMapMode;
                break;
            default:
                break;
        }
        return;
    }

        // menus
    if (!down) {
        if (button == SDL_CONTROLLER_BUTTON_A || button == SDL_CONTROLLER_BUTTON_START)
            gVirtualButton = false;
        return;
    }
    switch (button) {
        case SDL_CONTROLLER_BUTTON_A:
        case SDL_CONTROLLER_BUTTON_START:
            MenuPress(kPadAccept);
            break;
        case SDL_CONTROLLER_BUTTON_B:
            MenuPress(kPadBack);
            break;
        case SDL_CONTROLLER_BUTTON_X:
            MenuPress(kPadAlt);
            break;
        case SDL_CONTROLLER_BUTTON_Y:
            MenuPress(kPadAlt2);
            break;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            MenuPress(kPadPageUp);
            break;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            MenuPress(kPadPageDown);
            break;
        default:
            break;
    }
}

static void HandleStick(void)
{
    SDL_GameController *pad = NULL;
    int i, dir, newDirs;

    for (i = 0; i < gNumControllers; i++)
        if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gControllers[i])) == gLastPadID)
            pad = gControllers[i];
    if (pad == NULL)
        return;
    dir = DirectionFromStick(SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTX),
                             SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTY));
    if (dir == gHeldDir)
        return;
    newDirs = dir & ~gHeldDir;
    gHeldDir = dir;
    if (IsInGameplay()) {
        SetMovementFromPad();
    } else if (newDirs != 0 && gCurrentGameMode != kReadingSignMode) {
        MenuPress(newDirs);
        for (i = 0; i < 4; i++)
            gRepeatAt[i] = SDL_GetTicks() + kMenuRepeatDelay;
    }
}

/// Testing aid: with MM_DEBUG_PAD set, F5-F8 are the d-pad (up, down, left, right), F9 A,
/// F10 B, F11 X and F12 Y, so controller menus can be tried without a controller.
static Boolean HandleDebugPadKey(SDL_Event *event)
{
    static int enabled = -1;
    static const int buttons[] = {
        SDL_CONTROLLER_BUTTON_DPAD_UP, SDL_CONTROLLER_BUTTON_DPAD_DOWN, SDL_CONTROLLER_BUTTON_DPAD_LEFT,
        SDL_CONTROLLER_BUTTON_DPAD_RIGHT, SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B, SDL_CONTROLLER_BUTTON_X,
        SDL_CONTROLLER_BUTTON_Y
    };
    SDL_Scancode sc;

    if (enabled == -1)
        enabled = getenv("MM_DEBUG_PAD") != NULL;
    if (!enabled || (event->type != SDL_KEYDOWN && event->type != SDL_KEYUP))
        return false;
    sc = event->key.keysym.scancode;
    if (sc < SDL_SCANCODE_F5 || sc > SDL_SCANCODE_F12)
        return false;
    if (!event->key.repeat)
        HandlePadButton(buttons[sc - SDL_SCANCODE_F5], event->type == SDL_KEYDOWN);
    return true;
}

/// Returns true if the event was a controller event (handled here).
Boolean HandleControllerEvent(SDL_Event *event)
{
    if (HandleDebugPadKey(event) || DebugVirtualPadKey(event))
        return true;
    switch (event->type) {
        case SDL_CONTROLLERDEVICEADDED:
            AddController(event->cdevice.which);
            return true;
        case SDL_CONTROLLERDEVICEREMOVED:
            RemoveController(event->cdevice.which);
            return true;
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            gLastPadID = event->cbutton.which;
            HandlePadButton(event->cbutton.button, event->type == SDL_CONTROLLERBUTTONDOWN);
            return true;
        case SDL_CONTROLLERAXISMOTION:
            gLastPadID = event->caxis.which;
            if (event->caxis.axis == SDL_CONTROLLER_AXIS_LEFTX || event->caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
                HandleStick();
            else if (event->caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT && IsInGameplay())
                gKeys[kKeyZoomOut] = event->caxis.value > 16000 && gCurrentGameMode == kInGameMapMode;
            else if (event->caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT && IsInGameplay())
                gKeys[kKeyZoomIn] = event->caxis.value > 16000 && gCurrentGameMode == kInGameMapMode;
            return true;
        case SDL_JOYAXISMOTION:
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
        case SDL_JOYHATMOTION:
                // the same device as a game controller: already handled above
            return SDL_GameControllerFromInstanceID(event->jbutton.which) != NULL;
        case SDL_MOUSEMOTION:
            if (event->motion.which != SDL_TOUCH_MOUSEID && (abs(event->motion.xrel) + abs(event->motion.yrel)) > 1) {
                gVirtualActive = false;
                gControllerHidesCursor = false;
            }
            return false;
        default:
            return false;
    }
}

int TakeControllerMenuPresses(void)
{
    int presses = gMenuPresses;
    gMenuPresses = 0;
    return presses;
}

/// Held directions repeat in menus.
static void RepeatHeldDirections(void)
{
    static const int bits[] = { kPadUp, kPadDown, kPadLeft, kPadRight };
    Uint32 now = SDL_GetTicks();
    int i;

    if (IsInGameplay())
        return;
    for (i = 0; i < 4; i++) {
        Boolean held = gPadHeld[SDL_CONTROLLER_BUTTON_DPAD_UP + i] || (gHeldDir & bits[i]);
        if (held && now >= gRepeatAt[i]) {
            MenuPress(bits[i]);
            gRepeatAt[i] = now + kMenuRepeatRate;
        }
    }
}

// ---------------------------------------------------------------------------
// Menus: a highlight that moves between the screen's buttons

static int gRawMenuUsers = 0;
static int gDialogDepth = 0;

/// While a dialog is open on top of a screen that reads the controller itself, the dialog's
/// buttons get the highlight.
void ControllerDialogOpened(Boolean open)
{
    gDialogDepth += open ? 1 : -1;
    if (gDialogDepth < 0)
        gDialogDepth = 0;
}

static Boolean RawMenuInput(void)
{
    return gRawMenuUsers > 0 && gDialogDepth == 0 && !gCustomDialogIsVisible;
}

/// Screens that read TakeControllerMenuPresses themselves turn the button highlight off.
void ControllerUseRawMenuInput(Boolean raw)
{
    gRawMenuUsers += raw ? 1 : -1;
    if (gRawMenuUsers < 0)
        gRawMenuUsers = 0;
}

static void ButtonCenter(SpritePtr spriteP, float *h, float *v)
{
    *h = (spriteP->destFrameRect.left + spriteP->destFrameRect.right) / 2.0f;
    *v = (spriteP->destFrameRect.top + spriteP->destFrameRect.bottom) / 2.0f;
}

/// The button nearest to the focused one in the given direction.
static SpritePtr FindButtonInDirection(SpritePtr *buttons, int count, SpritePtr fromP, int dir)
{
    float fh, fv, h, v, along, across, score, best = 1e9f;
    SpritePtr bestP = NULL;
    int i;

    ButtonCenter(fromP, &fh, &fv);
    for (i = 0; i < count; i++) {
        if (buttons[i] == fromP)
            continue;
        ButtonCenter(buttons[i], &h, &v);
        if (dir & (kPadLeft | kPadRight)) {
            along = (dir & kPadRight) ? h - fh : fh - h;
            across = fabsf(v - fv);
        } else {
            along = (dir & kPadDown) ? v - fv : fv - v;
            across = fabsf(h - fh);
        }
        if (along <= 2.0f)
            continue;
        score = along + across * 1.2f;     // (2.5 skipped the Extras tab's checkboxes for Cancel)
        if (score < best) {
            best = score;
            bestP = buttons[i];
        }
    }
    return bestP;
}

/// For screens made of buttons: the controller moves a highlight between them and "points"
/// the game at it. Returns true while the controller, not the mouse, is in charge.
Boolean ControllerOverridesMouse(SWPoint *pointP, Boolean *buttonP)
{
    SpritePtr buttons[64];
    int count, i, presses;
    float h, v, bestDist = 1e9f;

    RepeatHeldDirections();
    if (!gVirtualActive || RawMenuInput() || IsInGameplay())
        return false;
    count = GetActiveButtonSprites(buttons, 64);
    presses = TakeControllerMenuPresses();
    if (count == 0) {
            // nothing to point at: "press any key" screens get Return, B is Escape
        if (presses & kPadAccept)
            PushKey(SDLK_RETURN, SDL_SCANCODE_RETURN);
        if (presses & kPadBack)
            PushKey(SDLK_ESCAPE, SDL_SCANCODE_ESCAPE);
        gFocusP = NULL;
        return false;
    }
    for (i = 0; i < count && buttons[i] != gFocusP; i++)
        ;
    if (i == count && gListMode) {
            // a list: Cross chooses the highlighted entry (Okay), Circle cancels
        for (i = 0; i < count && ((ButtonSpritePtr)buttons[i])->buttonIndex != 0; i++)
            ;
        if (i < count)
            gFocusP = buttons[i];
    }
    for (i = 0; i < count && buttons[i] != gFocusP; i++)
        ;
    if (i == count) {
            // start on the button nearest the last pointer position
        gFocusP = buttons[0];
        for (i = 0; i < count; i++) {
            ButtonCenter(buttons[i], &h, &v);
            h -= gVirtualPoint.h;
            v -= gVirtualPoint.v;
            if (h * h + v * v < bestDist) {
                bestDist = h * h + v * v;
                gFocusP = buttons[i];
            }
        }
    }
    if (gListMode) {
        if (presses & kPadUp)
            PushKey(SDLK_UP, SDL_SCANCODE_UP);
        if (presses & kPadDown)
            PushKey(SDLK_DOWN, SDL_SCANCODE_DOWN);
        if (presses & kPadPageUp)
            PushKey(SDLK_PAGEUP, SDL_SCANCODE_PAGEUP);
        if (presses & kPadPageDown)
            PushKey(SDLK_PAGEDOWN, SDL_SCANCODE_PAGEDOWN);
        presses &= ~(kPadUp | kPadDown);
    }
    if (presses & (kPadUp | kPadDown | kPadLeft | kPadRight)) {
        SpritePtr nextP = FindButtonInDirection(buttons, count, gFocusP, presses & (kPadUp | kPadDown | kPadLeft | kPadRight));
        if (nextP != NULL)
            gFocusP = nextP;
    }
    if (presses & kPadAccept)
        gVirtualButton = true;
    if (presses & kPadBack)
        PushKey(SDLK_ESCAPE, SDL_SCANCODE_ESCAPE);
    if (presses & kPadAlt)
        gControllerMenuAlt = true;
    if (presses & kPadAlt2)
        gControllerMenuAlt2 = true;
    if (!gListMode && (presses & kPadPageUp))
        gControllerTabStep = -1;
    if (!gListMode && (presses & kPadPageDown))
        gControllerTabStep = 1;

    ButtonCenter(gFocusP, &h, &v);
        // button rects are in scrolled world coordinates; callers convert the pointer to those
    gVirtualPoint.h = (short)(h - (gSpriteWorldP->visScrollRect.left - gSpriteWorldP->worldRect.left));
    gVirtualPoint.v = (short)(v - (gSpriteWorldP->visScrollRect.top - gSpriteWorldP->worldRect.top));
    *pointP = gVirtualPoint;
    *buttonP = gVirtualButton;
        // a click is a press then a release: release on the frame after the press
    if (gVirtualButton && !gPadHeld[SDL_CONTROLLER_BUTTON_A] && !gPadHeld[SDL_CONTROLLER_BUTTON_START])
        gVirtualButton = false;
    return true;
}

/// The button the controller highlight is on, while the controller drives the menus.
SpritePtr GetControllerFocusSprite(void)
{
    SpritePtr buttons[64];
    int count, i;

    if (!gVirtualActive || !gControllerHidesCursor || gFocusP == NULL || RawMenuInput() || IsInGameplay())
        return NULL;
    count = GetActiveButtonSprites(buttons, 64);
    for (i = 0; i < count; i++)
        if (buttons[i] == gFocusP)
            return gFocusP;
    return NULL;
}

/// After each round of events (ProcessSDLEvents).
void ControllerFrame(void)
{
    if (gPendingKeyUpAt != 0 && (Sint32)(SDL_GetTicks() - gPendingKeyUpAt) >= 0) {
        PostEvent(&gPendingKeyUp);
        gPendingKeyUpAt = 0;
    }
}

/// Split screen: the controllers are opened once; each player's game sees only its own
/// controller's events.
void MMControllerShareState(void (*share)(void *, size_t))
{
    share(gControllers, sizeof(gControllers));
    share(&gNumControllers, sizeof(gNumControllers));
}

/// The controller that sent the last button press or stick move (-1: none yet).
SDL_JoystickID ControllerLastPadID(void)
{
    return gLastPadID;
}

/// Testing aid (MM_DEBUG_VIRTUAL_PADS): keys press the pretend controllers' buttons. Pad 1:
/// J/L left/right, I up, K down, U A, O B; pad 2: the same on the number row 4/6/8/5/7/9.
/// Returns true if the key was one of these.
Boolean DebugVirtualPadKey(const SDL_Event *event)
{
    static const struct { SDL_Keycode key; int pad, button; } keys[] = {
        { SDLK_j, 0, SDL_CONTROLLER_BUTTON_DPAD_LEFT }, { SDLK_l, 0, SDL_CONTROLLER_BUTTON_DPAD_RIGHT },
        { SDLK_i, 0, SDL_CONTROLLER_BUTTON_DPAD_UP }, { SDLK_k, 0, SDL_CONTROLLER_BUTTON_DPAD_DOWN },
        { SDLK_u, 0, SDL_CONTROLLER_BUTTON_A }, { SDLK_o, 0, SDL_CONTROLLER_BUTTON_B },
        { SDLK_4, 1, SDL_CONTROLLER_BUTTON_DPAD_LEFT }, { SDLK_6, 1, SDL_CONTROLLER_BUTTON_DPAD_RIGHT },
        { SDLK_8, 1, SDL_CONTROLLER_BUTTON_DPAD_UP }, { SDLK_5, 1, SDL_CONTROLLER_BUTTON_DPAD_DOWN },
        { SDLK_7, 1, SDL_CONTROLLER_BUTTON_A }, { SDLK_9, 1, SDL_CONTROLLER_BUTTON_B },
    };
    size_t i;

    if (getenv("MM_DEBUG_VIRTUAL_PADS") == NULL || (event->type != SDL_KEYDOWN && event->type != SDL_KEYUP) ||
        event->key.repeat)
        return false;
    for (i = 0; i < SDL_arraysize(keys); i++) {
        if (keys[i].key == event->key.keysym.sym && keys[i].pad < gNumControllers) {
            SDL_Joystick *joy = SDL_GameControllerGetJoystick(gControllers[keys[i].pad]);
            SDL_JoystickSetVirtualButton(joy, keys[i].button, event->type == SDL_KEYDOWN);
            return true;
        }
    }
    return false;
}

/// Which connected controller (0 = the first) an SDL joystick instance is, or -1.
int ControllerIndexForInstanceID(SDL_JoystickID id)
{
    int i;

    for (i = 0; i < gNumControllers; i++)
        if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gControllers[i])) == id)
            return i;
    return -1;
}
