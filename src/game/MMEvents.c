/*
 * MMEvents.c — SDL event handling: keys, mouse, gamepad, window.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources & Headers/MMEvents.c").
 *
 * gKeys/gGamepad hold the state of the game's controls:
 *   0 up, 1 right, 2 down, 3 left, 4 jump, 5 and 6 (keypad 7/9), 7 action (return),
 *   9 map zoom in, 10 map zoom out.
 * gKeyPrefs.primaryKeys holds SDL scancodes for controls 0..8 (8 = show map).
 */
#include "MMGame.h"

int     gMouseButton = 0;
Uint32  gTimeOfLastWindowResize = 0;
int     gTestSoundIndex = 0;
static MMEventCallbackPtr gMMEventCallback = NULL;
static short gCurTileLayerToPrint = 0;
Boolean gMusicWasPlaying = false;
Boolean gResetMusic = false;
int     gMouseLocX, gMouseLocY;
static Boolean musicWasPlaying;

#define kAxisDeadZone   3000

Boolean gControllerHidesCursor = false;

void ProcessSDLEvents(void)
{
    SDL_Event event;

    if (SplitPumpPlayerEvents())        // split screen: this player's own input
        return;
    while (SDL_PollEvent(&event))
        MyHandleSDLEvent(&event);
    ControllerFrame();
    MMSyncHardwareCursor();
}

/// Left Cmd/Option/... are reported like the left-hand modifiers.
int ConvertKeyScanCode(int scancode)
{
    switch (scancode) {
        case SDL_SCANCODE_RCTRL:  return SDL_SCANCODE_LCTRL;
        case SDL_SCANCODE_RSHIFT: return SDL_SCANCODE_LSHIFT;
        case SDL_SCANCODE_RALT:   return SDL_SCANCODE_LALT;
        case SDL_SCANCODE_RGUI:   return SDL_SCANCODE_LGUI;
        default:                  return scancode;
    }
}

static Boolean IsMapScreenActive(void)
{
    return gCurrentGameMode == kInGameMapMode && !gIsEnteringMapScreen;
}

static void HandleJoyAxis(const SDL_JoyAxisEvent *e)
{
    if (e->axis == 0) {
        gKeys[kKeyRight] = false;
        gKeys[kKeyLeft] = false;
        if (e->value < -kAxisDeadZone)
            gKeys[kKeyLeft] = true;
        else if (e->value > kAxisDeadZone)
            gKeys[kKeyRight] = true;
    }
    if (e->axis == 1) {
        gKeys[kKeyDown] = false;
        gKeys[kKeyUp] = false;
        if (e->value < -kAxisDeadZone)
            gKeys[kKeyUp] = true;
        else if (e->value > kAxisDeadZone)
            gKeys[kKeyDown] = true;
    }
}

static void HandleJoyButton(const SDL_JoyButtonEvent *e)
{
    Boolean onGround;
    SpritePtr heroP;

        // buttons 0..3 are the d-pad
    if (e->button < 4) {
        if (e->button == 2)
            gGamepad[kKeyLeft] = e->state;
        else if (e->button == 3)
            gGamepad[kKeyRight] = e->state;
        else if (e->button == 0)
            gGamepad[kKeyUp] = e->state;
        else if (e->button == 1)
            gGamepad[kKeyDown] = e->state;
        gKeys[kKeyLeft] = gGamepad[kKeyLeft];
        gKeys[kKeyRight] = gGamepad[kKeyRight];
        gKeys[kKeyUp] = gGamepad[kKeyUp];

            // ignore "down" while walking on the ground, so a diagonal press doesn't crouch
        heroP = GetHeroSprite();
        onGround = heroP == NULL ? true : ((HeroSpritePtr)heroP)->onGround;
        if ((!gGamepad[kKeyLeft] && !gGamepad[kKeyRight]) || gCurrentGameMode == kInGameMapMode || !onGround)
            gKeys[kKeyDown] = gGamepad[kKeyDown];
        else
            gKeys[kKeyDown] = false;
    }

    if (e->button == 4 || e->button == 5)
        UpdateMovementKeys(gKeyPrefs.primaryKeys[4], gKeyPrefs.secondaryKeys[4], e->state, false);
    else if (e->button == 6)
        UpdateMovementKeys(gKeyPrefs.primaryKeys[7], gKeyPrefs.secondaryKeys[7], e->state, false);

    if (e->button == 7 && e->state == SDL_PRESSED) {
        if (IsMapScreenActive())
            ExitInGameMapScreen(true);
        else if (gCurrentGameMode == kPlayingGameMode)
            SetUpInGameMapScreen();
    }

    if (gCurrentGameMode == kInGameMapMode) {
        if (e->button == 11 || e->button == 13)
            gKeys[kKeyZoomIn] = e->state;
        else if (e->button == 12 || e->button == 14)
            gKeys[kKeyZoomOut] = e->state;
    }
}

/// Two computers: while the other player has the turn, this keyboard only watches (Escape
/// offers to leave). The player with the turn closes signs with Escape instead of pausing,
/// since pausing would close the sign on this computer only.
static Boolean HandleNetGameKey(SDL_Event *event)
{
    Boolean playing;

    if (!NetGameActive() || (event->type != SDL_KEYDOWN && event->type != SDL_KEYUP))
        return false;
    playing = gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kReadingSignMode;
    if (!playing || gGameIsPaused || gCustomDialogIsVisible)
        return false;
    if (event->key.keysym.mod & (KMOD_GUI | KMOD_CTRL))
        return false;
    if (NetIsFollower()) {
        if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE && !event->key.repeat)
            NetAskToLeave();
        return true;
    }
    if (gCurrentGameMode == kReadingSignMode && event->key.keysym.sym == SDLK_ESCAPE) {
        if (event->type == SDL_KEYDOWN && !event->key.repeat) {
            gReadingSignWasDismissed = true;
            WriteEventToMovieFile(0x2e, 0);
        }
        return true;
    }
    return false;
}

void MyHandleSDLEvent(SDL_Event *event)
{
    int scancode;

    if (HandleControllerEvent(event))
        return;
    if (gMMEventCallback != NULL && gMMEventCallback(event))
        return;
    if (ChatHandleEvent(event))
        return;
    if (HandleNetGameKey(event))
        return;

    switch (event->type) {
        case SDL_QUIT:
            if (gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kInGameMapMode) {
                if (MMDoCustomDialog(gSpriteWorldP, "Are you sure you want to quit?", 1) == 2)
                    gQuitGame = true;
            } else {
                gQuitGame = true;
            }
            break;

        case SDL_WINDOWEVENT:
            switch (event->window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_MAXIMIZED:
                case SDL_WINDOWEVENT_RESTORED:
                    puts("Window resized event.");
                    HandleWindowResizeEvent(event, event->window.data1, event->window.data2);
                    break;
                case SDL_WINDOWEVENT_ENTER:
                    gCursorIsInWindow = true;
                    MMHandleCursorMovement();
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    gCursorIsInWindow = false;
                    MMHandleCursorMovement();
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    puts("Activate event.");
                    HandleActivateEvent();
                    SDL_SetModState(KMOD_NONE);
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    puts("Deactivate event.");
                    HandleDeactivateEvent();
                    break;
                default:
                    break;
            }
            break;

        case SDL_KEYDOWN:
            scancode = ConvertKeyScanCode(event->key.keysym.scancode);
                // Cmd-key combinations are menu shortcuts, not game input
            if ((event->key.keysym.mod & KMOD_GUI) == 0) {
                if (!event->key.repeat)
                    UpdateMovementKeys(scancode, event->key.keysym.sym, true, event->key.repeat);
                if (!event->key.repeat &&
                    HandleKeyUpDownEvent(scancode, event->key.keysym.sym, true, event->key.repeat))
                    return;
            }
            if ((event->key.keysym.mod & KMOD_CTRL) != 0 && event->key.keysym.sym == SDLK_z)
                SDL_MinimizeWindow(gSpriteWorldP->mainWindow);
            break;

        case SDL_KEYUP:
            scancode = ConvertKeyScanCode(event->key.keysym.scancode);
            if (!event->key.repeat)
                UpdateMovementKeys(scancode, event->key.keysym.sym, false, event->key.repeat);
            HandleKeyUpDownEvent(scancode, event->key.keysym.sym, false, event->key.repeat);
            break;

        case SDL_MOUSEMOTION:
            if (IsMapScreenActive())
                HandleMapScreenMouseMoved((short)event->motion.x, (short)event->motion.y);
            UpdateMyMouseLoc((short)event->motion.x, (short)event->motion.y);
            break;

        case SDL_MOUSEBUTTONDOWN:
            gMouseButton = 1;
            if (IsMapScreenActive())
                HandleMapScreenMouseButton(event->button.button, true, (short)event->button.x, (short)event->button.y);
            if (gCurrentGameMode == kBeatTheMansionMode)
                gBeatMansionScreenWasDismissed = true;
            break;

        case SDL_MOUSEBUTTONUP:
            gMouseButton = 0;
            if (IsMapScreenActive())
                HandleMapScreenMouseButton(event->button.button, false, (short)event->button.x, (short)event->button.y);
            break;

        case SDL_MOUSEWHEEL:
            if (IsMapScreenActive())
                // The original passed the field at the x offset of SDL 1.3's wheel event;
                // vertical scrolling is what zooms on today's mice and trackpads.
                HandleMapScreenScrollWheelEvent((float)event->wheel.y);
            break;

        case SDL_JOYAXISMOTION:
            HandleJoyAxis(&event->jaxis);
            break;

        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            HandleJoyButton(&event->jbutton);
            break;

        default:
            break;
    }
}

/// Debugging aid: prints one tile layer, then the next one on the following call.
void printTileMap(short tileLayer)
{
    TileMapStructPtr tileMapP = gSpriteWorldP->tileLayerArray[gCurTileLayerToPrint];
    short row, col;

    printf("Tile layer: %d \n\n", tileLayer);
    if (tileMapP != NULL) {
        for (row = 0; row < tileMapP->numRows; row++) {
            for (col = 0; col < tileMapP->numCols; col++)
                printf("%d ", tileMapP->tileMap[row][col]);
            putchar('\n');
        }
    }
    gCurTileLayerToPrint++;
    if (gCurTileLayerToPrint > 6)
        gCurTileLayerToPrint = 0;
}

/// Debugging aid: jumps to the room next to the current one.
void LoadRoomToSideOfCurRoom(short colOffset, short rowOffset)
{
    short row, col, levelNum, resID;

    GEGetRowAndColFromLevelNumber(gCurrentLevel, &row, &col, 200);
    row += rowOffset;
    col += colOffset;
    levelNum = GEGetLevelNumberFromRowAndCol(row, col, 200);
    resID = GEGetLevelResID(levelNum, 1, 200);
    if (Resource1Exists("TMAP", resID)) {
        LoadLevel(levelNum, true);
        SetUpLevelForPlaying();
        DrawLevel();
        DoGameFadeToNormal(false);
    }
}

void SetStretchToFit(SWBoolean stretchToFit)
{
    int width, height;

    SWSetSpriteWorldStretchToFit(gSpriteWorldP, stretchToFit);
    if (gStatsSpriteWorldP != NULL)
        SWSetSpriteWorldStretchToFit(gStatsSpriteWorldP, stretchToFit);
    if (gFullScreenWindow) {
        SDL_GetWindowSize(gSpriteWorldP->mainWindow, &width, &height);
        DoWindowResize(width, height, false);
        SWUpdateSpriteWorld(gSpriteWorldP, true);
    }
    gStretchToFit = stretchToFit;
}

Boolean HasMMEventCallback(void)
{
    return gMMEventCallback != NULL;
}

void SetMMEventCallback(MMEventCallbackPtr callback)
{
    gMMEventCallback = callback;
}

void HandleWindowResizeEvent(SDL_Event *event, int width, int height)
{
    if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
        width = event->window.data1;
        height = event->window.data2;
    } else {
        SDL_GetWindowSize(gSpriteWorldP->mainWindow, &width, &height);
    }
    printf("\nWindow resize event! width = %d, height = %d \n", width, height);
    DoWindowResize(width, height, true);
    if (!gFullScreenWindow) {
        gWindowedResolutionX = gPrevWindowWidth;
        gWindowedResolutionY = gPrevWindowHeight;
        WritePreferencesFile();
    }
}

/// Keeps the window at 4:3, at least 640x480 and no taller than the display,
/// then updates the worlds and redraws the background.
void DoWindowResize(int width, int height, Boolean fade)
{
    const float kAspect = 1.3333334f;
    SDL_Rect bounds;
    int deltaW, deltaH, maxHeight, requestedW, requestedH;
    Boolean recenter = false;

    deltaW = gPrevWindowWidth - width;
    deltaH = gPrevWindowHeight - height;
    SDL_GetDisplayBounds(0, &bounds);
    maxHeight = bounds.h - 126;     // room for the menu bar and title bar

        // follow whichever side the user dragged more
    if (abs(deltaH) < abs(deltaW))
        height = (int)((float)width / kAspect);
    else
        width = (int)((float)height * kAspect);

    if (height > maxHeight) {
        width = (int)((float)maxHeight * kAspect);
        height = maxHeight;
        recenter = true;
    }
    if (width < 640 || height < 480) {
        width = 640;
        height = 480;
    }

    SDL_SetWindowSize(gSpriteWorldP->mainWindow, width, height);
    requestedW = width;
    requestedH = height;
    SDL_GetWindowSize(gSpriteWorldP->mainWindow, &width, &height);
    if (height < requestedH) {
        width = (int)((float)height * kAspect);
        SDL_SetWindowSize(gSpriteWorldP->mainWindow, width, height);
    }
    (void)requestedW;
    if (recenter)
        SDL_SetWindowPosition(gSpriteWorldP->mainWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    gPrevWindowWidth = (short)width;
    gPrevWindowHeight = (short)height;
    SDL_FlushEvents(SDL_WINDOWEVENT, SDL_WINDOWEVENT);
    SDL_GL_MakeCurrent(gSpriteWorldP->mainWindow, gSpriteWorldP->glContext);
    SWUpdateWindowSize(gSpriteWorldP);
    if (IsStatsAreaVisible() && gStatsSpriteWorldP != NULL)
        SWUpdateWindowSize(gStatsSpriteWorldP);

    if (fade)
        DoGameFadeToBlack(false, false);
    if (gCurrentGameMode == kHighScoresScreenMode)
        DrawContentsForHighScoresScreen(gCurrentHighScoresScreenIndex);
    else
        DrawBackgroundTextureForCurRoom();
    if (fade)
        DoGameFadeToNormal(false);
}

/// Screen-specific key handling. Returns false while a (Mac) dialog box is open.
Boolean HandleKeyUpDownEvent(int scancode, SDL_Keycode sym, Boolean isDown, Boolean isRepeat)
{
    (void)isRepeat;

        // testing aid (MM_DEBUG_CHEATS): F6 loses a life
    if (sym == SDLK_F6 && isDown && gCurrentGameMode == kPlayingGameMode && getenv("MM_DEBUG_CHEATS") != NULL &&
        GetHeroSprite() != NULL && !HERO(GetHeroSprite())->isDead) {
        if (NetGameActive() && NetIsFollower())
            return true;                // only the player with the turn; it goes to the others too
        KillHero(GetHeroSprite());
        NetRecordEvent(kNetEventDebugKill, true);
        return true;
    }

    if (gCustomDialogIsVisible)
        HandleCustomDialogKeyEvent(scancode, isDown);

    if (isDown && gCurrentGameMode == kBeatTheMansionMode) {
        gBeatMansionScreenWasDismissed = true;
    } else if (isDown && gCurrentGameMode == kBeatTheGameMode) {
        gBeatGameScreenWasDismissed = true;
    } else if (isDown && gCurrentGameMode == kReadingSignMode) {
        gReadingSignWasDismissed = true;
        WriteEventToMovieFile(0x2e, 0);
    } else if (isDown && gCurrentGameMode == kOrganScreenMode) {
        gOrganScreenWasDismissed = true;
        return true;
    } else if (isDown && gCurrentGameMode == kStoryScreenMode) {
        gStoryScreenWasDismissed = true;
        return true;
    }

    if (gDialogBoxIsOpen)
        return false;

        // typing a cheat code while paused
    if (gGameIsPaused && isDown && ((sym > 0x60 && scancode < 0x7b) || sym == SDLK_SPACE))
        AddKeyToCheatCodeCache(sym);

        // fast-forward movie playback
    if (sym == SDLK_RIGHT && IsMoviePlaying() == true)
        SWSetSpriteWorldMaxFPS(gSpriteWorldP, isDown ? 120 : 60);
    if (sym == SDLK_UP && IsMoviePlaying() == true)
        SWSetSpriteWorldMaxFPS(gSpriteWorldP, isDown ? 0 : 60);

    if (!gGameIsPaused) {
        if (isDown) {
            if (gCurrentGameMode == kInGameMapMode) {
                if ((gKeyPrefs.primaryKeys[8] == scancode || gKeyPrefs.primaryKeys[7] == scancode ||
                     sym == SDLK_RETURN || sym == SDLK_SPACE || sym == SDLK_ESCAPE) &&
                    !gIsEnteringMapScreen) {
                    ExitInGameMapScreen(true);
                    return true;
                }
            } else if (gCurrentGameMode == kPlayingGameMode && gKeyPrefs.primaryKeys[8] == scancode &&
                       !gCustomDialogIsVisible) {
                SetUpInGameMapScreen();
            }
        }
        if (sym == SDLK_EQUALS || sym == SDLK_PLUS || sym == SDLK_KP_PLUS) {
            if (gCurrentGameMode == kInGameMapMode)
                gKeys[kKeyZoomIn] = isDown;
        } else if ((sym == SDLK_MINUS || sym == SDLK_KP_MINUS || sym == SDLK_KP_EQUALS) &&
                   gCurrentGameMode == kInGameMapMode) {
            gKeys[kKeyZoomOut] = isDown;
        }
    }

    if (sym == SDLK_ESCAPE && !gCustomDialogIsVisible) {
        gEscapeKey = isDown;
        if ((gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kReadingSignMode) && isDown) {
            if (!gEditorIsRunning)
                TogglePause();
            else
                ExitToEditor();
        }
    }
    return true;
}

/// Maps a key to the game's controls (and records it in a movie).
void UpdateMovementKeys(int scancode, SDL_Keycode sym, Boolean isDown, Boolean isRepeat)
{
    (void)sym;

    if (IsMovieFileOpenForRecording() && !gDialogBoxIsOpen &&
        (gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kReadingSignMode))
        AddKeyToMovie(scancode, isDown, isRepeat);

    if (IsMoviePlaying() && gCurrentGameMode != kInGameMapMode)
        return;

    if (gArrowKeysEnabled) {
        if (scancode == SDL_SCANCODE_LEFT)
            gKeys[kKeyLeft] = isDown;
        else if (scancode == SDL_SCANCODE_RIGHT)
            gKeys[kKeyRight] = isDown;
        else if (scancode == SDL_SCANCODE_DOWN)
            gKeys[kKeyDown] = isDown;
        else if (scancode == SDL_SCANCODE_UP)
            gKeys[kKeyUp] = isDown;
        else if (scancode == SDL_SCANCODE_SPACE && !isRepeat)
            gKeys[kKeyJump] = isDown;
    }

    if (gKeyPrefs.primaryKeys[3] == scancode)
        gKeys[kKeyLeft] = isDown;
    else if (gKeyPrefs.primaryKeys[1] == scancode)
        gKeys[kKeyRight] = isDown;
    else if (gKeyPrefs.primaryKeys[2] == scancode)
        gKeys[kKeyDown] = isDown;
    else if (gKeyPrefs.primaryKeys[0] == scancode)
        gKeys[kKeyUp] = isDown;
    else if (gKeyPrefs.primaryKeys[4] == scancode && !isRepeat)
        gKeys[kKeyJump] = isDown;
    else if (gKeyPrefs.primaryKeys[5] == scancode && !isRepeat)
        gKeys[kKeyJumpLeft] = isDown;
    else if (gKeyPrefs.primaryKeys[6] == scancode && !isRepeat)
        gKeys[kKeyJumpRight] = isDown;
    else if (gKeyPrefs.primaryKeys[7] == scancode)
        gKeys[kKeyAction] = isDown;
}

void HandleActivateEvent(void)
{
    int x, y;

    MMFlushEvents();
    if (gWasFullscreenWhenMinimized) {
        puts("Window WAS fullscreen when deactivated, so toggling fullscreen!");
        ProcessSDLEvents();
        if (!gFullScreenWindow)
            ToggleFullscreen();
        gWasFullscreenWhenMinimized = false;
    }
    SDL_GetMouseState(&x, &y);
    gCursorIsInWindow = !(x == gMouseLocX && y == gMouseLocY);

    if (musicWasPlaying && !gGameIsPaused)
        ResumeMusic(false);

    if (!gUsingEditor) {
        if (!gDialogBoxIsOpen && (CurrentScreenHasCustomCursor() || gGameIsPaused)) {
            MMSetCursorForGame();
            MMShieldCursor();
        }
    } else {
        MMRemoveGameCursor();
        MMShieldCursor();
    }
    if (gCurrentGameMode == kTitleScreenMode)
        PlayCricketSound();
    gMMIsActive = true;
}

void HandleDeactivateEvent(void)
{
    if (!gMMIsActive)
        return;
    if (!gPlayingGame) {
        musicWasPlaying = IsMusicPlaying();
        PauseMusic();
        StopAllChannels();
        printf("Music was playing: %d\n", musicWasPlaying);
    } else if (!gCustomDialogIsVisible) {
        PauseGame();
    }
    MMSetCursorForSystem();
    gMMIsActive = false;
}

/// True if a mouse-down is waiting in the event queue.
Boolean WasMouseClicked(void)
{
    SDL_Event events[20];
    int i, count;

    SDL_PumpEvents();
    count = SDL_PeepEvents(events, 20, SDL_PEEKEVENT, SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONDOWN);
    for (i = 0; i < count; i++) {
        if (events[i].type == SDL_MOUSEBUTTONDOWN)
            return true;
    }
    return false;
}

/// True if a (non-modifier) key-down is waiting in the event queue.
Boolean WasAKeyPressed(void)
{
    SDL_Event events[20];
    int i, count;

    SDL_PumpEvents();
    count = SDL_PeepEvents(events, 20, SDL_PEEKEVENT, SDL_KEYDOWN, SDL_KEYDOWN);
    for (i = 0; i < count; i++) {
        if (events[i].type == SDL_KEYDOWN && !IsKeyModifier(events[i].key.keysym.scancode)) {
            printf("A key was pushed!");
            return true;
        }
    }
    return false;
}

Boolean IsKeyModifier(int scancode)
{
    return scancode >= SDL_SCANCODE_LCTRL && scancode <= SDL_SCANCODE_RGUI;
}

/// Checks the queue for a window close, Cmd-Q or quit request.
Boolean WasGameQuit(void)
{
    SDL_Event events[20];
    int i, count;

    SDL_PumpEvents();
    count = SDL_PeepEvents(events, 20, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
    for (i = 0; i < count; i++) {
        if (events[i].type == SDL_WINDOWEVENT) {
            if (events[i].window.event == SDL_WINDOWEVENT_CLOSE)
                gQuitGame = true;
        } else if (events[i].type == SDL_KEYDOWN) {
            if (events[i].key.keysym.sym == SDLK_q && (events[i].key.keysym.mod & KMOD_GUI) != 0)
                gQuitGame = true;
        } else if (events[i].type == SDL_QUIT) {
            gQuitGame = true;
        }
    }
    return gQuitGame;
}

void MMFlushEvents(void)
{
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
}

void UpdateMyMouseLoc(short x, short y)
{
    gMouseLocX = x;
    gMouseLocY = y;
}

void ResetMyMouseLocAndButton(void)
{
    gMouseButton = 0;
    SDL_GetMouseState(&gMouseLocX, &gMouseLocY);
}

/// Window point (SDL mouse coordinates) to design coordinates. The world is drawn in a
/// viewport that may be letterboxed (full screen on a wide display) and measured in
/// drawable pixels, which differ from window points on high-DPI displays.
static void WindowPointToDesign(int x, int y, SWPoint *pointP)
{
    float scaleH, scaleV, pixelsH = 1.0f, pixelsV = 1.0f;
    int winW, winH, drawW, drawH;

    SWGetWindowScalingFactor(gSpriteWorldP, &scaleH, &scaleV);
    SDL_GetWindowSize(gSpriteWorldP->mainWindow, &winW, &winH);
    SDL_GL_GetDrawableSize(gSpriteWorldP->mainWindow, &drawW, &drawH);
    if (winW > 0 && winH > 0 && drawW > 0 && drawH > 0) {
        pixelsH = (float)drawW / (float)winW;
        pixelsV = (float)drawH / (float)winH;
    }
    pointP->h = (short)(int)(((float)x * pixelsH - (float)gSpriteWorldP->letterboxH) / scaleH);
    pointP->v = (short)(int)(((float)y * pixelsV - (float)gSpriteWorldP->letterboxV) / scaleV);
}

/// Last mouse location from events, in design coordinates. Returns the button state.
Boolean GetMyMouse(SWPoint *pointP)
{
    Boolean padButton;

    if (ControllerOverridesMouse(pointP, &padButton))
        return padButton;
    WindowPointToDesign(gMouseLocX, gMouseLocY, pointP);
    return (Boolean)gMouseButton;
}

/// Current mouse location, in design coordinates. Returns the button state.
Boolean MMGetMouse(SWPoint *pointP)
{
    int x, y;
    Uint32 buttons;
    Boolean padButton;

    if (ControllerOverridesMouse(pointP, &padButton))
        return padButton;
    buttons = SDL_GetMouseState(&x, &y);
    WindowPointToDesign(x, y, pointP);
    return (buttons & SDL_BUTTON_LMASK) != 0;
}

Boolean IsMouseDown(void)
{
    return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) != 0;
}

void ResetKeys(void)
{
    gKeys[kKeyLeft] = false;
    gKeys[kKeyRight] = false;
    gKeys[kKeyDown] = false;
    gKeys[kKeyUp] = false;
    gKeys[kKeyJump] = false;
    gKeys[kKeyJumpLeft] = false;
    gKeys[kKeyJumpRight] = false;
    gKeys[kKeyAction] = false;
    gKeys[kKeyZoomIn] = false;
    gKeys[kKeyZoomOut] = false;
    memcpy(gGamepad, gKeys, 11);
    gEscapeKey = false;
}

void ToggleFullscreen(void)
{
    SDL_DisplayMode mode;

    while (IsMouseDown())
        ProcessSDLEvents();

    gFullScreenWindow = !gFullScreenWindow;
    SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(gSpriteWorldP->mainWindow), &mode);
    if (gLimitFullscreenResolution) {
        if ((float)mode.w / (float)mode.h == 1.6f) {
            mode.w = 1024;
            mode.h = 640;
        } else {
            mode.w = 1024;
            mode.h = 768;
        }
    }
    SDL_SetWindowDisplayMode(gSpriteWorldP->mainWindow, &mode);
    SDL_SetWindowFullscreen(gSpriteWorldP->mainWindow, gFullScreenWindow ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
    if (!gFullScreenWindow) {
        SDL_SetWindowSize(gSpriteWorldP->mainWindow, gWindowedResolutionX, gWindowedResolutionY);
        DoWindowResize(gWindowedResolutionX, gWindowedResolutionY, false);
        SDL_SetWindowPosition(gSpriteWorldP->mainWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    } else {
        SWUpdateWindowSize(gSpriteWorldP);
    }
}

void ToggleMusic(void)
{
    if (!gGameIsPaused) {
        if (!IsMusicPlaying()) {
            ResumeMusic(true);
            gMusicIsOn = true;
        } else {
            PauseMusic();
            gMusicIsOn = false;
        }
    } else {
        gMusicWasPlaying = !gMusicWasPlaying;
        gResetMusic = true;
        gMusicIsOn = !gMusicIsOn;
    }
    WritePreferencesFile();
    UpdateMenuBar();
}

void ToggleStats(void)
{
    gDisplayFPSAndLevelResID = !gDisplayFPSAndLevelResID;
    if (gCurrentGameMode == kPlayingGameMode) {
        if (!gDisplayFPSAndLevelResID)
            RemoveAllSpritesInLayer(gFPSDisplaySpriteLayerP);
        else
            CreateFPSAndLevelResIDSprites();
    }
}

void GiveOSTime(void)
{
    if (SplitGiveOSTime())      // split screen: the scheduler sleeps between frames
        return;
    SDL_Delay(1);
}

void KeepMusicPlaying(void)
{
    if (SplitGiveOSTime())
        return;
    SDL_Delay(10);
}
