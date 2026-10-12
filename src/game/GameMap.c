// GameMap.c — the in-game map: a zoomable view of every explored room.
//
// Each room is rendered once into a texture (the "map cache"), then all visible rooms are
// drawn scaled into the window. Map coordinates are pixels at zoom 1, one room being
// kMapRoomWidth x kMapRoomHeight; gMapScrollRect is the visible part of the map.

#include "MMGame.h"
#include "MMProtos.h"
#include "BlitKernel.h"
#include "MMFont.h"

#define kMapRoomWidth   0x280   // 640
#define kMapRoomHeight  0x1b0   // 432
#define kMapCacheSize   200
#define kMinMapZoom     0.6f
#define kMaxMapZoom     6.0f

typedef struct LongRect { Sint32 top, left, bottom, right; } LongRect;
typedef struct FloatRect { float top, left, bottom, right; } FloatRect;

short   gNumRoomsPerCol = 5;
static long averageFrameRate = -1;
SpritePtr saveTileBackgroundP = NULL;

static short gBottomMostRoom, gBottomPadding, gLeftMostRoom, gLeftPadding;
static short gRightMostRoom, gRightPadding, gTopMostRoom, gTopPadding;
float   gCurMapZoom;
static short gCurrentScrollSpeed;
static Boolean gDoTrackTimeThisFrame;
Boolean gIsEnteringMapScreen;
static unsigned short gJacksHeadTranslucency;
static Boolean gKeysCopy[11];
static short gLastMouseLocX, gLastMouseLocY;
static FramePtr gMapFrameArray[kMapCacheSize];
static short gMapLevelNumber[kMapCacheSize];
static Boolean gMapMouseButtonIsDown;
static LongRect gMapScrollRect;
static short gSaveCurConveyorBeltDirection, gSaveOldConveyorBeltDirection, gSaveOldBackgroundPictResID;
static Boolean gSaveGoldFloorIsActive, gSaveSilverFloorIsActive;
static SpriteLayerPtr gSaveHiddenTriggerItemsSpriteLayerP;
static short gSavedLevelNumber;
static float gScrollIncrease;
static Boolean gUseOptimizedCaching;
static short lastLevelLoadedInAdvance;
static unsigned long millisecsLastFrame;
static short numRoomsCanAddThisFrame;

static void CenterLongRect(LongRect *rectP, const LongRect *centerInP);
static void OffsetLongRect(LongRect *rectP, long h, long v);
static void CenterFloatRect(FloatRect *rectP, const FloatRect *centerInP);
static void MMPaintFloatRect(const FloatRect *rectP, const SDL_Color *color, Uint8 alpha);
static void CustomMapDrawProc(FramePtr frameP, const FloatRect *rectP, Boolean isCachedRoom, float scale,
                              unsigned short translucency);
static void DrawCachedMapRoom(short levelNumber, const FloatRect *rectP);
static void DrawOtherPlayersOnMap(void);
static void BeginDesignSpaceDrawing(float left, float top);

static Boolean AnyMovementKeyDown(void)
{
    return gKeys[kKeyLeft] || gKeys[kKeyRight] || gKeys[kKeyUp] || gKeys[kKeyDown];
}

static unsigned long ElapsedMillisecs(unsigned long start)
{
    return SWGetMilliseconds(gSpriteWorldP) - start;
}

// ---------------------------------------------------------------------------
// Entering and running the map screen

void SetUpInGameMapScreen(void)
{
    unsigned long start;
    float frameTime;
    SWError err;

    InitMapCache();
    gUseOptimizedCaching = true;
    gIsEnteringMapScreen = true;
    CalculateMapBounds();
    StopLoopingSounds();
    DisposeAllMapCachedRooms();
    SWSetSpriteLayersVisible(gBackgroundScreenSpriteLayerP, gBackgroundPictureSpriteLayerP, true);
    saveTileBackgroundP = gSpriteWorldP->backgroundTileSpriteP;
    gSpriteWorldP->backgroundTileSpriteP = NULL;

    // The first time, time how long caching a room takes.
    if (averageFrameRate == -1) {
        millisecsLastFrame = SWGetMilliseconds(gSpriteWorldP);
        UpdateCurRoomPreviewInMapCache(NULL, gCurrentLevel);
        averageFrameRate = (long)(ElapsedMillisecs(millisecsLastFrame) * 1.5);
    }
    else
        UpdateCurRoomPreviewInMapCache(NULL, gCurrentLevel);

    gCurrentScrollSpeed = 0;
    gKeys[kKeyMap] = false;
    if (SaveAllSpritesToTempLayers(gSpriteWorldP) == kNoError) {
        MMFlushEvents();
        gCurrentGameMode = kInGameMapMode;
        UpdateMenuBar();
        gSaveCurConveyorBeltDirection = gCurConveyorBeltDirection;
        gSaveOldConveyorBeltDirection = gOldConveyorBeltDirection;
        gSaveSilverFloorIsActive = gSilverFloorIsActive;
        gSaveGoldFloorIsActive = gGoldFloorIsActive;
        gSaveOldBackgroundPictResID = gOldBackgroundPictResID;
        gSaveHiddenTriggerItemsSpriteLayerP = gHiddenTriggerItemsSpriteLayerP;
        err = SWCreateSpriteLayer(&gHiddenTriggerItemsSpriteLayerP);
        if (err != kNoError)
            MM_FATAL(err);
        SWSaveTileMaps(gSpriteWorldP);
        gSavedLevelNumber = gCurrentLevel;
        lastLevelLoadedInAdvance = -1;
        MMPaintRect(&gSpriteWorldP->worldRect, &gSDLBlackColor, 0xff);
        KeepMusicPlaying();
        if (gNumRoomsPerCol == 1)
            gNumRoomsPerCol = 3;
        ChangeStatsAreaForMapScreen();
        ComputeMapRectForLevel(gCurrentLevel, &gMapScrollRect);
        memcpy(gKeysCopy, gKeys, sizeof(gKeysCopy));
        ResetKeys();
        gTopPadding = gLeftPadding = gRightPadding = gBottomPadding = 0;
        frameTime = 20.0f;
        gJacksHeadTranslucency = 0;

        // Cache the rooms around Jack, then zoom out from his room.
        SetMapZoom(2.5f);
        numRoomsCanAddThisFrame = 9;
        DrawMapScreenRooms(false);
        SetMapZoom(1.0f);
        numRoomsCanAddThisFrame = 0;
        while (gCurMapZoom < 2.5f) {
            start = SWGetMilliseconds(gSpriteWorldP);
            SetMapZoom(((((2.5f - gCurMapZoom) / 2.0f + 1.8f) / 1000.0f) * frameTime + 1.0f) * gCurMapZoom);
            DrawMapScreen();
            gJacksHeadTranslucency = (unsigned short)(((gCurMapZoom - 1.0f) * 65535.0f) / 1.5f);
            if (gCurMapZoom > 2.0f)
                ProcessSDLEvents();
            if ((gCurMapZoom > 2.0f && gKeys[kKeyLeft]) || gKeys[kKeyRight] || gKeys[kKeyUp] ||
                gKeys[kKeyDown])
                break;
            frameTime = (float)ElapsedMillisecs(start);
        }
        gJacksHeadTranslucency = 0xffff;
        millisecsLastFrame = SWGetMilliseconds(gSpriteWorldP);
    }
    gIsEnteringMapScreen = false;
    gScrollIncrease = 0;
    gMapMouseButtonIsDown = false;
    MMSetCursorForSystem();
    puts("Done initial map set-up.");
}

void ComputeMapRectForLevel(short levelNumber, void *rectP)
{
    LongRect *r = (LongRect *)rectP;
    short row, col;

    GEGetRowAndColFromLevelNumber(levelNumber, &row, &col, 200);
    r->top = row * kMapRoomHeight;
    r->left = col * kMapRoomWidth;
    r->right = r->left + kMapRoomWidth;
    r->bottom = r->top + kMapRoomHeight;
}

void SetMapZoom(float zoom)
{
    if (zoom > kMaxMapZoom)
        zoom = kMaxMapZoom;
    else if (zoom < kMinMapZoom)
        zoom = kMinMapZoom;
    gCurMapZoom = zoom;
    SetRectZoom(&gMapScrollRect, zoom);
}

/// Resizes the rect to one room scaled by `zoom`, keeping its center.
void SetRectZoom(void *rectP, float zoom)
{
    LongRect *r = (LongRect *)rectP;
    LongRect newRect;

    newRect.top = r->top;
    newRect.left = r->left;
    newRect.bottom = (Sint32)(zoom * kMapRoomHeight + newRect.top);
    newRect.right = (Sint32)(zoom * kMapRoomWidth + newRect.left);
    CenterLongRect(&newRect, r);
    *r = newRect;
}

void InitMapCache(void)
{
    static Boolean wasInited = false;
    short i;

    if (!wasInited) {
        for (i = 0; i < kMapCacheSize; i++) {
            gMapFrameArray[i] = NULL;
            gMapLevelNumber[i] = -1;
        }
        wasInited = true;
    }
}

static void ExtendMapBounds(short levelNumber)
{
    short row, col;

    GEGetRowAndColFromLevelNumber(levelNumber, &row, &col, 200);
    if (col < gLeftMostRoom) gLeftMostRoom = col;
    if (col > gRightMostRoom) gRightMostRoom = col;
    if (row < gTopMostRoom) gTopMostRoom = row;
    if (row > gBottomMostRoom) gBottomMostRoom = row;
}

void CalculateMapBounds(void)
{
    short i, count;

    gLeftMostRoom = 200;
    gTopMostRoom = 160;
    gRightMostRoom = 0;
    gBottomMostRoom = 0;
    count = GetNumRoomsExplored();
    for (i = 0; i < count; i++)
        ExtendMapBounds(GetRoomArrayElement(i));
    count = GetNumRoomsInMapArray();
    for (i = 0; i < count; i++)
        ExtendMapBounds(GetMapArrayElement(i));
    gLeftMostRoom -= 3;
    gTopMostRoom -= 3;
    gRightMostRoom += 3;
    gBottomMostRoom += 3;
}

/// Replaces the stats bar with the mansion's name.
void ChangeStatsAreaForMapScreen(void)
{
    Str255 mansionName;
    MMTextStyleRec style;
    SWRGBColor white = { 0xffff, 0xffff, 0xffff };
    SWRect rect;
    SpritePtr textP;

    LoadStatsBackFrame(0xc80);
    HideStatsSprites();
    if (LoadMansionNameFromGAMEResource(mansionName) == 0) {
        memset(&style, 0, sizeof(style));
        SetPascalString(style.fontName, (ConstStringPtr)"\015BlackChancery");
        style.style = 1;
        style.size = 36;
        style.unused = true;
        style.dropShadow = true;
        style.justification = 2;
        SWSetRect(&rect, 0, 0, 640, 36);
        rect.bottom -= 2;
        textP = MyDrawPascalTextJustified(mansionName, &style, &rect, &white);
        if (textP != NULL) {
            SWRemoveSprite(textP);
            SWAddSprite(gStatsMapTextSpriteLayerP, textP);
        }
    }
    SWUpdateSpriteWorld(gStatsSpriteWorldP, false);
}

/// Called every frame while the map is shown: scrolling and zooming with the keys.
void RunInGameMapScreen(void)
{
    unsigned long now, elapsed;
    float zoomStep, scaledRate;
    SWRect worldRect = gSpriteWorldP->worldRect;

    now = SWGetMilliseconds(gSpriteWorldP);
    elapsed = now - millisecsLastFrame;
    if ((long)elapsed < averageFrameRate && (AnyMovementKeyDown() || gKeys[kKeyZoomIn] || gKeys[kKeyZoomOut]))
        return;

    millisecsLastFrame = now;
    if (!AnyMovementKeyDown())
        gScrollIncrease = 0;
    else if (gScrollIncrease == 0.0f)
        gScrollIncrease = 1.0f;
    else {
        gScrollIncrease += 0.03f;
        if (gScrollIncrease > 1.8f)
            gScrollIncrease = 1.8f;
    }

    if (averageFrameRate < 1)
        gCurrentScrollSpeed = (short)((gCurMapZoom * 15.0f + 15.0f) / 2.0f);
    else {
        scaledRate = (float)averageFrameRate / 2.2f;
        gCurrentScrollSpeed = (short)((scaledRate * gCurMapZoom + scaledRate) / 2.0f);
    }

    if (gKeys[kKeyLeft])
        MoveMapScrollRect((short)(gScrollIncrease * -gCurrentScrollSpeed), 0);
    else if (gKeys[kKeyRight])
        MoveMapScrollRect((short)(gScrollIncrease * gCurrentScrollSpeed), 0);
    if (gKeys[kKeyUp])
        MoveMapScrollRect(0, (short)(gScrollIncrease * -gCurrentScrollSpeed));
    else if (gKeys[kKeyDown])
        MoveMapScrollRect(0, (short)(gScrollIncrease * gCurrentScrollSpeed));

    zoomStep = (averageFrameRate < 1) ? 0.015f : averageFrameRate * 0.001f;
    if (gKeys[kKeyZoomIn])
        SetMapZoom((1.0f - zoomStep) * gCurMapZoom);
    else if (gKeys[kKeyZoomOut])
        SetMapZoom((zoomStep + 1.0f) * gCurMapZoom);

    GetMapRoomWidth(&worldRect);
    numRoomsCanAddThisFrame = 1;
    gDoTrackTimeThisFrame = false;
    millisecsLastFrame = SWGetMilliseconds(gSpriteWorldP);
    DrawMapScreen();
    now = SWGetMilliseconds(gSpriteWorldP);

    // Frames that cached a room are timed to pace scrolling.
    if (gDoTrackTimeThisFrame) {
        if (averageFrameRate == -1)
            averageFrameRate = (long)(now - millisecsLastFrame);
        else
            averageFrameRate = (averageFrameRate * 3 + (long)(now - millisecsLastFrame)) / 4;
        if (averageFrameRate > 200)
            averageFrameRate = 200;
    }
}

void MoveMapScrollRect(short h, short v)
{
    OffsetLongRect(&gMapScrollRect, h, v);

    // Pre-cache rooms on the side we're moving towards.
    if (h < 0) {
        gLeftPadding = 3;
        gRightPadding = 0;
    }
    else if (h > 0) {
        gRightPadding = 3;
        gLeftPadding = 0;
    }
    if (v < 0) {
        gTopPadding = 3;
        gBottomPadding = 0;
    }
    else if (v > 0) {
        gBottomPadding = 3;
        gTopPadding = 0;
    }
}

void HandleMapScreenMouseButton(short button, Boolean isDown, short x, short y)
{
    if (button == SDL_BUTTON_LEFT) {
        gMapMouseButtonIsDown = isDown;
        gLastMouseLocX = x;
        gLastMouseLocY = y;
    }
}

/// Dragging with the mouse scrolls the map.
void HandleMapScreenMouseMoved(short x, short y)
{
    float scale = gSpriteWorldP->designScaleH;

    if (gMapMouseButtonIsDown && SDL_GetMouseFocus() == gSpriteWorldP->mainWindow) {
        MoveMapScrollRect((short)((((gLastMouseLocX - x) * gCurMapZoom) / scale) * 3.0f),
                          (short)((((gLastMouseLocY - y) * gCurMapZoom) / scale) * 3.0f));
        gLastMouseLocX = x;
        gLastMouseLocY = y;
    }
}

void HandleMapScreenScrollWheelEvent(float delta)
{
    SetMapZoom((delta / -40.0f + 1.0f) * gCurMapZoom);
}

// ---------------------------------------------------------------------------
// Drawing

void DrawMapScreen(void)
{
    short saveLevel = gCurrentLevel;

    if (numRoomsCanAddThisFrame > 0)
        DrawMapScreenRooms(false);
    SWUpdateSpriteWorld(gStatsSpriteWorldP, false);
    glViewport(gSpriteWorldP->viewportX, gSpriteWorldP->viewportY, gSpriteWorldP->viewportWidth,
               gSpriteWorldP->viewportHeight);
    MMPaintRect(&gSpriteWorldP->workAreaRect, &gSDLBlackColor, 0xff);
    DrawMapScreenRooms(true);
    gCurrentLevel = saveLevel;
    SWPaintRectToSidesOfWorldRect(gSpriteWorldP);
    SWPresentWindow(gSpriteWorldP);
}

/// With draw false, caches up to numRoomsCanAddThisFrame visible rooms; with draw true, draws
/// the visible rooms, Jack's head on his room, and shading on unexplored or secret rooms.
/// Changes gCurrentLevel.
void DrawMapScreenRooms(Boolean draw)
{
    long firstRow = gMapScrollRect.top / kMapRoomHeight;
    long firstCol = gMapScrollRect.left / kMapRoomWidth;
    long lastRow = (gMapScrollRect.bottom - 1) / kMapRoomHeight;
    long lastCol = (gMapScrollRect.right - 1) / kMapRoomWidth;
    long row, col;
    short resID;
    FloatRect roomRect, headRect;
    FramePtr headFrameP;

    for (row = firstRow; row <= lastRow; row++) {
        for (col = firstCol; col <= lastCol; col++) {
            gCurrentLevel = GEGetLevelNumberFromRowAndCol((short)row, (short)col, 200);
            resID = GEGetLevelResID(gCurrentLevel, 1, 200);
            if (!IsRoomInArray(gCurrentLevel) && !IsRoomInMapArray(gCurrentLevel))
                continue;
            if (!ResourceExists("TMAP", resID))
                continue;

            roomRect.top = (float)(row * kMapRoomHeight - gMapScrollRect.top) / gCurMapZoom;
            roomRect.left = (float)(col * kMapRoomWidth - gMapScrollRect.left) / gCurMapZoom;
            roomRect.right = (float)(col * kMapRoomWidth + kMapRoomWidth - gMapScrollRect.left) / gCurMapZoom;
            roomRect.bottom = (float)(row * kMapRoomHeight + kMapRoomHeight - gMapScrollRect.top) / gCurMapZoom;

            if (!draw) {
                if (numRoomsCanAddThisFrame > 0 && AddMapRoomToCacheIfNecessary(gCurrentLevel, false))
                    numRoomsCanAddThisFrame--;
                continue;
            }

            DrawMapRoomToScreen(gCurrentLevel, roomRect.top, roomRect.left, roomRect.bottom, roomRect.right);
            if (gCurrentLevel == gSavedLevelNumber && gNumRoomsPerCol > 1 && gSpriteArray[204] != NULL) {
                // Jack's head marks the room he is in.
                headFrameP = gSpriteArray[204]->frameArray[0];
                headRect.top = headFrameP->frameRect.top;
                headRect.left = headFrameP->frameRect.left;
                headRect.bottom = (headFrameP->frameRect.bottom - headRect.top) / (gCurMapZoom / 2.0f);
                headRect.right = (headFrameP->frameRect.right - headRect.left) / (gCurMapZoom / 2.0f);
                CenterFloatRect(&headRect, &roomRect);
                CustomMapDrawProc(headFrameP, &headRect, false, 2.0f / gCurMapZoom, gJacksHeadTranslucency);
            }
            if (gShowSecrets && IsRoomASecretRoom(resID))
                MMPaintFloatRect(&roomRect, &gSDLTealColor, 0x50);
            else if (!IsRoomInArray(gCurrentLevel))
                MMPaintFloatRect(&roomRect, &gSDLBlackColor, 100);
        }
    }
    if (draw)
        DrawOtherPlayersOnMap();
}

/// The room the player is in while the map is up.
short GetMapPlayerLevel(void)
{
    return gSavedLevelNumber;
}

/// Split screen: the other players' heads on the rooms they are in (Jack's head tinted with
/// their character's colour), with their names above.
static void DrawOtherPlayersOnMap(void)
{
    static MMFont *font = NULL;
    SplitOtherPlayer others[kMaxPlayers];
    FloatRect roomRect, headRect;
    FramePtr headFrameP;
    const float *rgb;
    const char *name;
    short row, col;
    float width, shift;
    int n, i, j, k;

    if (!SplitScreenActive() || gSpriteArray[204] == NULL || gNumRoomsPerCol <= 1)
        return;
    if (font == NULL)
        font = MMFontLoad(GetFontsFolderPath("Action Man.ttf"));
    n = SplitGetOtherPlayers(others, kMaxPlayers);
    headFrameP = gSpriteArray[204]->frameArray[0];
    for (i = 0; i < n; i++) {
        if (others[i].room <= 0)
            continue;
        GEGetRowAndColFromLevelNumber(others[i].room, &row, &col, 200);
        roomRect.top = (float)(row * kMapRoomHeight - gMapScrollRect.top) / gCurMapZoom;
        roomRect.left = (float)(col * kMapRoomWidth - gMapScrollRect.left) / gCurMapZoom;
        roomRect.right = (float)(col * kMapRoomWidth + kMapRoomWidth - gMapScrollRect.left) / gCurMapZoom;
        roomRect.bottom = (float)(row * kMapRoomHeight + kMapRoomHeight - gMapScrollRect.top) / gCurMapZoom;
        headRect.top = headFrameP->frameRect.top;
        headRect.left = headFrameP->frameRect.left;
        headRect.bottom = (headFrameP->frameRect.bottom - headRect.top) / (gCurMapZoom / 2.0f);
        headRect.right = (headFrameP->frameRect.right - headRect.left) / (gCurMapZoom / 2.0f);
        CenterFloatRect(&headRect, &roomRect);
            // players sharing a room stand side by side
        for (j = 0, k = others[i].room == gSavedLevelNumber ? 1 : 0; j < i; j++)
            if (others[j].room == others[i].room)
                k++;
        shift = (headRect.right - headRect.left) * 0.6f * (float)k;
        headRect.left += shift;
        headRect.right += shift;
        if (headRect.right < 0 || headRect.left > gSpriteWorldP->designWidth || headRect.bottom < 0 ||
            headRect.top > gSpriteWorldP->designHeight)
            continue;
        rgb = GetCharacterColor(GetPlayerCharacter(others[i].player));
        glColor4f(0.55f + 0.45f * rgb[0], 0.55f + 0.45f * rgb[1], 0.55f + 0.45f * rgb[2], 1.0f);
        glBindTexture(GL_TEXTURE_2D, headFrameP->glSurfaceP->texture);
        BeginDesignSpaceDrawing(headRect.left, headRect.top);
        glScalef(2.0f / gCurMapZoom, 2.0f / gCurMapZoom, 1.0f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        BKBlitGLSurface(headFrameP->glSurfaceP);
        glDisable(GL_BLEND);
        glPopMatrix();
        if (font != NULL) {
            name = GetPlayerDisplayName(others[i].player);
            MMFontFaceSize(font, 24);
            width = MMFontRenderWidth(font, name) * 0.5f;
            BeginDesignSpaceDrawing((headRect.left + headRect.right) / 2.0f - width / 2.0f, headRect.top - 2.0f);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(0, 0, 0, 0.85f);
            MMFontRender(font, name, 1.0f, 1.0f, 0.5f);
            glColor4f(rgb[0], rgb[1], rgb[2], 1.0f);
            MMFontRender(font, name, 0, 0, 0.5f);
            glDisable(GL_BLEND);
            glPopMatrix();
        }
    }
    glColor4f(1, 1, 1, 1);
}

void DrawMapRoomToScreen(short levelNumber, float top, float left, float bottom, float right)
{
    FloatRect rect;

    if (FindRoomInMapCache(levelNumber) != -1) {
        rect.top = top;
        rect.left = left;
        rect.bottom = bottom;
        rect.right = right;
        DrawCachedMapRoom(levelNumber, &rect);
    }
}

/// Loads the room and renders it into the cache. With loadOnly, only loads it (once).
/// Returns true if a room was cached (or loaded). Restores gCurrentLevel.
Boolean AddMapRoomToCacheIfNecessary(short levelNumber, Boolean loadOnly)
{
    short saveLevel = gCurrentLevel;
    CallBackPtr callBack;
    Boolean added = false;

    if (FindRoomInMapCache(levelNumber) == -1) {
        gDoTrackTimeThisFrame = true;
        if (!loadOnly || lastLevelLoadedInAdvance != levelNumber) {
            LoadLevel(levelNumber, false);
            lastLevelLoadedInAdvance = levelNumber;
            gSilverFloorIsActive = gSaveSilverFloorIsActive;
            gGoldFloorIsActive = gSaveGoldFloorIsActive;
            SetCorrectGoldSilverTileImages(false);
            ResetKeySpriteFrames();
            if (loadOnly)
                return true;
        }
        callBack = gSpriteWorldP->postEraseCallBack;
        SWSetPostEraseCallBack(gSpriteWorldP, NULL);
        SWSetSpriteWorldAutoScaling(gSpriteWorldP, false);
        SWUpdateSpriteWorld(gSpriteWorldP, false);
        AddCurrentRoomToMapCache(levelNumber);
        SWSetSpriteWorldAutoScaling(gSpriteWorldP, true);
        SWSetPostEraseCallBack(gSpriteWorldP, callBack);
        added = true;
    }
    gCurrentLevel = saveLevel;
    return added;
}

short calculateNumRoomsInMapCache(void)
{
    short i, count = 0;
    for (i = 0; i < kMapCacheSize; i++)
        if (gMapLevelNumber[i] != -1)
            count++;
    return count;
}

short FindRoomInMapCache(short levelNumber)
{
    short i;
    for (i = 0; i < kMapCacheSize; i++)
        if (gMapLevelNumber[i] == levelNumber)
            return i;
    return -1;
}

short FindEmptyMapCacheElement(void)
{
    return FindRoomInMapCache(-1);
}

void DeleteCachedRoomsOutsideCurrentView(void)
{
    short i;
    for (i = 0; i < kMapCacheSize; i++) {
        if (gMapLevelNumber[i] != -1 && !IsRoomInsideCurrentView(gMapLevelNumber[i]) &&
            gMapLevelNumber[i] != gSavedLevelNumber)
            DisposeMapCachedRoom(i);
    }
}

short getIndexOfMapCachedRoomOutsideCurView(void)
{
    short i;
    for (i = 0; i < kMapCacheSize; i++) {
        if (gMapLevelNumber[i] != gSavedLevelNumber && gMapLevelNumber[i] != -1 &&
            !IsRoomInsideCurrentView(gMapLevelNumber[i]))
            return i;
    }
    return -1;
}

/// True if the room is visible, counting the pre-cache padding.
Boolean IsRoomInsideCurrentView(short levelNumber)
{
    long row, col;
    long firstRow = gMapScrollRect.top / kMapRoomHeight - gTopPadding;
    long lastRow = (gMapScrollRect.bottom - 1) / kMapRoomHeight + gBottomPadding;
    long firstCol = gMapScrollRect.left / kMapRoomWidth - gLeftPadding;
    long lastCol = (gMapScrollRect.right - 1) / kMapRoomWidth + gRightPadding;

    for (row = firstRow; row <= lastRow; row++)
        for (col = firstCol; col <= lastCol; col++)
            if (GEGetLevelNumberFromRowAndCol((short)row, (short)col, 200) == levelNumber)
                return true;
    return false;
}

/// Caches the current room as it looks now (without Jack, his halo or the FPS display).
void UpdateCurRoomPreviewInMapCache(SpritePtr heroP, short levelNumber)
{
    CallBackPtr callBack;

    if (heroP == NULL)
        heroP = GetHeroSprite();
    if (heroP == NULL)
        heroP = SWGetNextSprite(gDeadHeroSpriteLayerP, NULL);
    SWSetSpritesInLayerVisible(gFPSDisplaySpriteLayerP, false);
    SWSetSpritesInLayerVisible(gHaloSpriteLayerP, false);
    if (heroP != NULL)
        SWSetSpriteVisible(heroP, false);
    SWSetSpriteWorldAutoScaling(gSpriteWorldP, false);
    callBack = gSpriteWorldP->postEraseCallBack;
    SWSetPostEraseCallBack(gSpriteWorldP, NULL);
    SWSetTileLayersVisible(gSpriteWorldP, 0, 6, true);
    SWUpdateSpriteWorld(gSpriteWorldP, false);
    AddCurrentRoomToMapCache(levelNumber);
    SWSetSpriteWorldAutoScaling(gSpriteWorldP, true);
    SWSetTileLayersVisible(gSpriteWorldP, 0, 5, false);
    SWSetSpritesInLayerVisible(gHaloSpriteLayerP, true);
    SWSetSpritesInLayerVisible(gFPSDisplaySpriteLayerP, true);
    SWSetPostEraseCallBack(gSpriteWorldP, callBack);
    if (heroP != NULL)
        SWSetSpriteVisible(heroP, true);
}

/// Copies the rendered room into the cache, reusing an off-screen entry when possible.
Boolean AddCurrentRoomToMapCache(short levelNumber)
{
    FramePtr frameP;
    short index, reuseIndex = -1;
    SWError err = kNoError;

    InitMapCache();
    index = FindRoomInMapCache(levelNumber);
    if (index != -1) {
        UpdateExistingRoomInMapCache(index);
        return true;
    }

    if (gUseOptimizedCaching)
        reuseIndex = getIndexOfMapCachedRoomOutsideCurView();
    if (reuseIndex == -1) {
        index = FindEmptyMapCacheElement();
        if (index == -1)
            return false;
        err = SWCreateFrameFromWorkArea(gSpriteWorldP, &frameP, &gSpriteWorldP->visScrollRect);
        if (err == kNoError) {
            gMapLevelNumber[index] = levelNumber;
            gMapFrameArray[index] = frameP;
        }
    }
    else {
        SWUpdateFrameFromWorkArea(gSpriteWorldP, gMapFrameArray[reuseIndex], &gSpriteWorldP->visScrollRect);
        gMapLevelNumber[reuseIndex] = levelNumber;
    }
    return err == kNoError;
}

void UpdateExistingRoomInMapCache(short index)
{
    FramePtr frameP;
    SWError err;

    SDL_assert(index >= 0 && index < kMapCacheSize);
    SWSetSpriteWorldAutoScaling(gSpriteWorldP, false);
    err = SWCreateFrameFromWorkArea(gSpriteWorldP, &frameP, &gSpriteWorldP->visScrollRect);
    SWSetSpriteWorldAutoScaling(gSpriteWorldP, true);
    if (err == kNoError) {
        SWDisposeFrame(&gMapFrameArray[index]);
        gMapFrameArray[index] = frameP;
    }
}

static void DrawCachedMapRoom(short levelNumber, const FloatRect *rectP)
{
    short index = FindRoomInMapCache(levelNumber);

    SDL_assert(index >= 0 && index < kMapCacheSize);
    SDL_assert(gMapFrameArray[index] != NULL);
    CustomMapDrawProc(gMapFrameArray[index], rectP, true, 1.0f / gCurMapZoom, 0xffff);
}

/// Redraws the map behind a dialog box shown over the map screen.
void DrawMapInOffscreenAreas(void)
{
    DisposeCurrentRoom(true);
    glClear(GL_COLOR_BUFFER_BIT);
    DrawMapScreen();
}

void DisposeRoomInMapCache(short levelNumber)
{
    short index = FindRoomInMapCache(levelNumber);
    if (index != -1)
        DisposeMapCachedRoom(index);
}

void DisposeMapCachedRoom(short index)
{
    SDL_assert(index >= 0);
    SDL_assert(gMapLevelNumber[index] != -1);
    SDL_assert(gMapFrameArray[index] != NULL);
    SWDisposeFrame(&gMapFrameArray[index]);
    gMapFrameArray[index] = NULL;
    gMapLevelNumber[index] = -1;
}

void DisposeAllMapCachedRooms(void)
{
    short i;
    for (i = 0; i < kMapCacheSize; i++)
        if (gMapFrameArray[i] != NULL)
            DisposeMapCachedRoom(i);
}

short GetMapRoomWidth(SWRect *rectP)
{
    return (short)((rectP->right - rectP->left) / gNumRoomsPerCol);
}

short GetMapRoomHeight(SWRect *rectP)
{
    return (short)((rectP->bottom - rectP->top) / gNumRoomsPerCol);
}

// ---------------------------------------------------------------------------
// Revealing rooms (map pedestals and cheats). Arguments are room resIDs.

static void RevealRooms(short firstResID, short lastResID, int which)
{
    short firstRow, firstCol, lastRow, lastCol, row, col, levelNumber, resID;

    GEGetRowAndColFromLevelNumber(GEGetLevelNumber(firstResID, 1, 200), &firstRow, &firstCol, 200);
    GEGetRowAndColFromLevelNumber(GEGetLevelNumber(lastResID, 1, 200), &lastRow, &lastCol, 200);
    for (row = firstRow; row <= lastRow; row++) {
        for (col = firstCol; col <= lastCol; col++) {
            levelNumber = GEGetLevelNumberFromRowAndCol(row, col, 200);
            resID = GEGetLevelResID(levelNumber, 1, 200);
            if (!Resource1Exists("TMAP", resID))
                continue;
            if (which == 0) {           // normal rooms
                if (!IsRoomASecretRoom(resID))
                    AddRoomToMapArray(levelNumber);
            }
            else if (which == 1) {      // secret rooms
                if (IsRoomASecretRoom(resID))
                    AddRoomToMapArray(levelNumber);
            }
            else {                      // everything, as explored
                AddRoomToMapArray(levelNumber);
                AddRoomToArray(levelNumber);
            }
        }
    }
}

void RevealGridInMap(short firstResID, short lastResID)
{
    RevealRooms(firstResID, lastResID, 0);
}

void RevealEntireMap(void)
{
    RevealRooms(0x80, 0x7fff, 2);
}

void RevealSecretRooms(void)
{
    RevealRooms(0x80, 0x7fff, 1);
}

/// Keys (object sprites 0x27-0x2c) stop their animation in the cached map pictures.
void ResetKeySpriteFrames(void)
{
    SpritePtr spriteP = NULL;

    while ((spriteP = SWGetNextSprite(gObjectSpriteLayerP, spriteP)) != NULL) {
        if (spriteP->userData > 0x26 && spriteP->userData < 0x2d)
            SWSetCurrentFrameIndex(spriteP, 0);
    }
}

// ---------------------------------------------------------------------------
// Leaving the map screen

static Boolean gSkipZoomBack = false;

/// A game ending while the map or a sign is up (split screen ends everyone's game at once):
/// leaves them without the animations, so nothing of them is left for the next game — the
/// mansion's name over the status bar, or the room's sprites still put away, which made the
/// next map key blank the room and stop the player.
void AbandonInGameScreens(void)
{
    if (gCurrentGameMode == kInGameMapMode) {
        gSkipZoomBack = true;
        ExitInGameMapScreen(false);
        gSkipZoomBack = false;
        gCurrentGameMode = kPlayingGameMode;
    } else if (gCurrentGameMode == kReadingSignMode) {
        SWDisposeAllSpritesInLayer(gSignContentsSpriteLayerP);
        UnpauseAllSpriteLayers(gSpriteWorldP);
        gCurrentGameMode = kPlayingGameMode;
    }
    if (gStatsMapTextSpriteLayerP != NULL)
        SWRemoveAllSpritesFromLayer(gStatsMapTextSpriteLayerP);
}

void ExitInGameMapScreen(Boolean returnToGame)
{
    short resID;
    Boolean wasDark;
    SpritePtr haloP;

    LoadAppropriateStatsBackground();
    MakeStatsSpritesVisible();
    SWRemoveAllSpritesFromLayer(gStatsMapTextSpriteLayerP);
    if (gSkipZoomBack)
        SetMapZoom(1.0f);
    else
        DoZoomBackToGameEffect();
    if (!gFasterInGameMap)
        DisposeAllMapCachedRooms();
    DisposeCurrentRoom(true);
    RestoreSpritesFromTempLayers(gSpriteWorldP);
    SWRestoreTileMaps(gSpriteWorldP);
    gCurrentLevel = gSavedLevelNumber;
    gSpriteWorldP->backgroundTileSpriteP = saveTileBackgroundP;
    saveTileBackgroundP = NULL;
    ResetKeys();
    WriteEventToMovieFile(0xb, true);

    if (returnToGame) {
        resID = GEGetLevelResID(gCurrentLevel, 1, 200);
        SWSetSpriteLayersVisible(gBackgroundScreenSpriteLayerP, gBackgroundPictureSpriteLayerP, false);
        LoadLEVLResource(resID);
        wasDark = gRoomIsDark;
        MakeRoomDarkOrLight(IsRoomDark(resID), wasDark);
        gSilverFloorIsActive = gSaveSilverFloorIsActive;
        gGoldFloorIsActive = gSaveGoldFloorIsActive;
        SetCorrectGoldSilverTileImages(false);
        gCurConveyorBeltDirection = gSaveCurConveyorBeltDirection;
        gOldConveyorBeltDirection = gSaveOldConveyorBeltDirection;
        gOldBackgroundPictResID = gSaveOldBackgroundPictResID;
        SWDisposeSpriteLayer(&gHiddenTriggerItemsSpriteLayerP);
        gHiddenTriggerItemsSpriteLayerP = gSaveHiddenTriggerItemsSpriteLayerP;
        averageFrameRate = -1;
        gCurrentGameMode = kPlayingGameMode;
        UpdateMenuBar();
        if (gSpriteWorldP->backgroundTileSpriteP != NULL &&
            SWGetSpriteWidth(gSpriteWorldP->backgroundTileSpriteP) !=
                (float)(gSpriteWorldP->backgroundTileRect.right - gSpriteWorldP->backgroundTileRect.left))
            DrawBackgroundTextureForCurRoom();
        haloP = SWGetNextSprite(gHaloSpriteLayerP, NULL);
        if (haloP != NULL)
            SWSetSpriteScaledFactor(haloP, 50.0f, 50.0f);
    }
    if (gPlayingGameMovie)
        memcpy(gKeys, gKeysCopy, sizeof(gKeysCopy));
    MMShieldCursor();
}

/// Zooms back in to Jack's room.
void DoZoomBackToGameEffect(void)
{
    LongRect targetRect;
    unsigned long start;
    short i, levelNumber, resID;
    float accel, frameTime = 0, zoom, speed;
    long dh, dv;

    gUseOptimizedCaching = false;
    for (i = -1; i < 2; i++) {
        levelNumber = gSavedLevelNumber + i;
        resID = GEGetLevelResID(levelNumber, 1, 200);
        if ((IsRoomInArray(levelNumber) || IsRoomInMapArray(levelNumber)) && ResourceExists("TMAP", resID))
            AddMapRoomToCacheIfNecessary(levelNumber, false);
    }

    accel = (gCurMapZoom / 2.5f) * 0.0006f;
    numRoomsCanAddThisFrame = 0;
    gJacksHeadTranslucency = 0;
    while (gCurMapZoom > 1.0f) {
        start = SWGetMilliseconds(gSpriteWorldP);
        ComputeMapRectForLevel(gSavedLevelNumber, &targetRect);
        SetRectZoom(&targetRect, gCurMapZoom);
        zoom = (1.0f - accel * frameTime) * gCurMapZoom;
        if (zoom < 1.0f)
            zoom = 1.0f;
        SetMapZoom(zoom);
        accel += 2e-05f;
        if (accel > 0.0025f)
            accel = 0.0025f;

        // Glide towards Jack's room, faster when zoomed out.
        speed = (gCurMapZoom - 1.0f) * 45.0f + 1.0f;
        if (speed < 1.0f)
            speed = 1.0f;
        if (speed > 40.0f / gCurMapZoom)
            speed = 40.0f / gCurMapZoom;
        dh = (long)(((float)(targetRect.left - gMapScrollRect.left) / gCurMapZoom) / speed);
        dv = (long)(((float)(targetRect.top - gMapScrollRect.top) / gCurMapZoom) / speed);
        OffsetLongRect(&gMapScrollRect, dh, dv);
        DrawMapScreen();
        frameTime = (float)ElapsedMillisecs(start);
    }
}

// ---------------------------------------------------------------------------
// Rect helpers

static void CenterLongRect(LongRect *rectP, const LongRect *centerInP)
{
    Sint32 width = rectP->right - rectP->left;
    Sint32 height = rectP->bottom - rectP->top;

    rectP->left = centerInP->left + ((centerInP->right - centerInP->left) / 2 - width / 2);
    rectP->top = centerInP->top + ((centerInP->bottom - centerInP->top) / 2 - height / 2);
    rectP->right = rectP->left + width;
    rectP->bottom = rectP->top + height;
}

static void OffsetLongRect(LongRect *rectP, long h, long v)
{
    rectP->left += h;
    rectP->right += h;
    rectP->top += v;
    rectP->bottom += v;
}

static void CenterFloatRect(FloatRect *rectP, const FloatRect *centerInP)
{
    float width = rectP->right - rectP->left;
    float height = rectP->bottom - rectP->top;

    rectP->left = width / -2.0f + (centerInP->right - centerInP->left) / 2.0f + centerInP->left;
    rectP->top = height / -2.0f + (centerInP->bottom - centerInP->top) / 2.0f + centerInP->top;
    rectP->right = rectP->left + width;
    rectP->bottom = rectP->top + height;
}

static void BeginDesignSpaceDrawing(float left, float top)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glScalef(gSpriteWorldP->designScaleH, gSpriteWorldP->designScaleV, 1.0f);
    glTranslatef(left, top, 0);
}

/// Fills a rect in design coordinates. alpha 0xff is opaque.
void MMPaintRect(SWRect *rectP, SDL_Color *color, Uint8 alpha)
{
    short width = rectP->right - rectP->left, height = rectP->bottom - rectP->top;

    glDisable(GL_TEXTURE_2D);
    glColor4ub(color->r, color->g, color->b, alpha);
    BeginDesignSpaceDrawing(rectP->left, rectP->top);
    if (alpha != 0xff) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(width, 0);
    glVertex2i(width, height);
    glVertex2i(0, height);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
}

static void MMPaintFloatRect(const FloatRect *rectP, const SDL_Color *color, Uint8 alpha)
{
    float width = rectP->right - rectP->left, height = rectP->bottom - rectP->top;

    glDisable(GL_TEXTURE_2D);
    glColor4ub(color->r, color->g, color->b, alpha);
    BeginDesignSpaceDrawing(rectP->left, rectP->top);
    if (alpha != 0xff) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(width, 0);
    glVertex2f(width, height);
    glVertex2f(0, height);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
}

/// Draws a frame scaled with linear filtering. Cached rooms were copied from the framebuffer;
/// the original flipped them here, BKBlitGLSurface does that from the surface's flipV flag.
static void CustomMapDrawProc(FramePtr frameP, const FloatRect *rectP, Boolean isCachedRoom, float scale,
                              unsigned short translucency)
{
    int needsBlend;

    (void)isCachedRoom;
    glColor4us(0xffff, 0xffff, 0xffff, translucency);
    glBindTexture(GL_TEXTURE_2D, frameP->glSurfaceP->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    BeginDesignSpaceDrawing(rectP->left, rectP->top);
    glScalef(scale, scale, 1.0f);
    needsBlend = DoesSurfaceNeedBlend(frameP->glSurfaceP) || translucency != 0xffff;
    if (needsBlend) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    BKBlitGLSurface(frameP->glSurfaceP);
    if (needsBlend)
        glDisable(GL_BLEND);
    glPopMatrix();
}
