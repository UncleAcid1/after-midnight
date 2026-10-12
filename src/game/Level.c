/*
 * Level.c — loading rooms ("levels"): their tile maps, sprites and settings,
 * plus the background texture, shadows, glows and dark rooms.
 *
 * A mansion is a grid of rooms; each has TMAP/TMAQ..TMAV tile maps (layers 0-6),
 * a SPRT list of sprites, a LEVL record and a BGND record.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources & Headers/Level.c").
 */
#include "MMGame.h"

LEVLRec gLEVLStruct;
static short saveCurTileArray[0x331];

    // one sprite in a room's SPRT resource (big-endian, 0x16 bytes)

static const char *const kTileMapResTypes[7] = { "TMAP", "TMAQ", "TMAR", "TMAS", "TMAT", "TMAU", "TMAV" };

/// Loads the room's tile maps and sprites and sets everything up for it.
void LoadRoom(short roomResID, Boolean includeHero)
{
    TileMapStructPtr tileMapP;
    char path[600];
    Boolean wasDark;
    short layer;
    SWError err;

    if (SWStickyError() != kNoError)
        MM_FATAL(SWStickyError());
    SDL_GL_MakeCurrent(gSpriteWorldP->mainWindow, gSpriteWorldP->glContext);
    WriteCStringToFile("LoadRoom() entered for room ID: ");
    WriteNumberToFile(roomResID);
    WriteCarriageReturn(1);

    for (layer = 0; layer < 7; layer++) {
        snprintf(path, sizeof(path), "%s'%s'/%05d", gCurOpenMansionFilePath, kTileMapResTypes[layer], roomResID);
        err = SWLoadTileMap(path, &tileMapP);
        if (err != kNoError) {
            WriteCStringToFile("SWLoadTileMap FAILED for layer ");
            WriteNumberToFile(layer);
            WriteCarriageReturn(1);
            MM_FATAL(err);
        }
        SWInstallTileMap(gSpriteWorldP, tileMapP, layer);
    }

    MMLoadExtraBackFrameForRoom(gSpriteWorldP, roomResID);
    err = LoadSpritesInRoom(roomResID, includeHero);
    if (err != kNoError)
        MM_FATAL(err);
    if (SWStickyError() != kNoError)
        MM_FATAL(SWStickyError());

    SWCustomSortSpriteLayer(gTrapDoorSpriteLayerP, CompareSpriteLeft);
    CreateZapperBeamsForRoom();
    if (SWStickyError() != kNoError)
        MM_FATAL(SWStickyError());
    if (gCurrentGameMode != kInGameMapMode)
        ResetConveyorBeltDir();
    SetLeversToCorrectState(gLeverSpriteLayerP);
    SetBrickSwitchesToCorrectState(gBrickSwitchSpriteLayerP);
    OpenDoorsThatWereOpenedByPlayer();
    OpenDoorsThatAreOpenToStart();
    CreateGrayStatueOnSavePedestals();
    if (SWStickyError() != kNoError)
        MM_FATAL(SWStickyError());

    wasDark = gRoomWasDark;
    MakeRoomDarkOrLight(IsRoomDark(roomResID), wasDark);

    gRoomMoveBoundsRect.left = 0;
    gRoomMoveBoundsRect.top = 0;
    gRoomMoveBoundsRect.right = gSpriteWorldP->tileLayerArray[0]->numCols << 4;
    gRoomMoveBoundsRect.bottom = gSpriteWorldP->tileLayerArray[0]->numRows << 4;
    SWSetScrollingWorldMoveBounds(gSpriteWorldP, &gRoomMoveBoundsRect);

    if (!IsRoomDark(roomResID))
        ConvertTilesForCurRoom();
    FixCandleStandsInRoom();
    AddSpriteShadows();
    if (gCurrentGameMode != kInGameMapMode)
        ExtrasRoomLoaded();
    if (gCurrentGameMode == kInGameMapMode) {
        ProcessOneFrame(gSpriteWorldP);
        ProcessOneFrame(gSpriteWorldP);
    }
    if (SWStickyError() != kNoError)
        MM_FATAL(SWStickyError());
}

/// Draws the walls' shadows into the background texture: the tile layers are
/// drawn a second time, offset and in translucent black, with everything that
/// shouldn't cast a shadow removed.
void DrawTileShadowsCallback(SpriteWorldPtr spriteWorldP, short tileLayer, SWBoolean layerWasDrawn)
{
    TileMapStructPtr savedMaps[10], tileMapP, cloneP, wallMapP;
    short layer, row, col, tile;

    (void)tileLayer;
    if (!layerWasDrawn || gLEVLStruct.noShadows || gDisableDynamicShadows ||
        (gOldBackgroundPictResID != 1 && !gLEVLStruct.hasOutdoorShadows))
        return;

        // nothing behind a wall casts a shadow
    for (layer = 1; layer < 4; layer++) {
        tileMapP = spriteWorldP->tileLayerArray[layer];
        for (row = 0; row < tileMapP->numRows; row++) {
            for (col = 0; col < tileMapP->numCols; col++) {
                tile = ConvertTileID(spriteWorldP->tileLayerArray[4]->tileMap[row][col]);
                if (tile > 0 && tile < 0x1a3)
                    tileMapP->tileMap[row][col] = -1;
            }
        }
    }

        // work on copies of layers 1-6
    for (layer = 0; layer < 10; layer++)
        savedMaps[layer] = NULL;
    for (layer = 1; layer < 7; layer++) {
        if (SWCloneTileMap(spriteWorldP->tileLayerArray[layer], &cloneP) == kNoError) {
            savedMaps[layer] = spriteWorldP->tileLayerArray[layer];
            SWInstallTileMap(spriteWorldP, cloneP, layer);
        }
    }
    if (savedMaps[4] != NULL) {
            // walls covered by layer 6 walls don't cast shadows twice
        wallMapP = spriteWorldP->tileLayerArray[4];
        for (row = 0; row < wallMapP->numRows; row++) {
            for (col = 0; col < wallMapP->numCols; col++) {
                if (IsWallTile(wallMapP->tileMap[row][col]) &&
                    IsWallTile(spriteWorldP->tileLayerArray[6]->tileMap[row][col]))
                    wallMapP->tileMap[row][col] = -1;
            }
        }
    }
    for (layer = 1; layer < 7; layer++) {
        if (savedMaps[layer] == NULL)
            continue;
        tileMapP = spriteWorldP->tileLayerArray[layer];
        for (row = 0; row < tileMapP->numRows; row++) {
            for (col = 0; col < tileMapP->numCols; col++) {
                tile = ConvertTileID(tileMapP->tileMap[row][col]);
                if (tile > 0x2e8 && (tile < 0x53f || (tile > 0x59c && tile < 0x5a0)))
                    tileMapP->tileMap[row][col] = -1;
                    // inactive gold/silver floors don't cast shadows
                if (!gSilverFloorIsActive) {
                    if (tile > 0x1ae && tile < 0x1bd)
                        tileMapP->tileMap[row][col] = -1;
                }
                else if (tile > 0x1bc && tile < 0x1cb) {
                    tileMapP->tileMap[row][col] = -1;
                }
            }
        }
    }

    SWSetStdTileColor(spriteWorldP, 0, 0, 0, 0x5fff, true);
    spriteWorldP->tileOffsetV = -16;
    spriteWorldP->tileOffsetH = -12;
    for (layer = 1; layer < 7; layer++)
        SWDrawTileLayerInRect(spriteWorldP, layer, &spriteWorldP->visScrollRect, false, true);
    for (layer = 0; layer < 7; layer++) {
        if (savedMaps[layer] != NULL) {
            SWDisposeTileMap(&spriteWorldP->tileLayerArray[layer]);
            spriteWorldP->tileLayerArray[layer] = savedMaps[layer];
        }
    }
    spriteWorldP->tileOffsetV = 0;
    spriteWorldP->tileOffsetH = 0;
    SWSetStdTileColor(spriteWorldP, 0xffff, 0xffff, 0xffff, 0xffff, false);
}

/// Candle stands (0xa8) stand in front of the small candles placed on them.
void FixCandleStandsInRoom(void)
{
    SpritePtr spriteP, nextP;

    for (spriteP = SWGetNextSprite(gCandleHolderSpriteLayerP, NULL); spriteP != NULL; spriteP = nextP) {
        nextP = SWGetNextSprite(gCandleHolderSpriteLayerP, spriteP);
        if (spriteP->userData == 0xa8) {
            SWRemoveSprite(spriteP);
            SWAddSprite(gFloorCandleStandSpriteLayerP, spriteP);
            SWCollideSpriteWithLayer(spriteP, gCandleSpriteLayerP, SmallCandleCollideProc);
        }
    }
}

void SmallCandleCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    short dh, dv;

    (void)sectRect;
    if (dstSpriteP->userData > 0xa3 && dstSpriteP->userData < 0xa7) {
        dh = (short)(SWGetSpriteHorizLoc(srcSpriteP) - SWGetSpriteHorizLoc(dstSpriteP));
        dv = (short)(SWGetSpriteVertLoc(srcSpriteP) - SWGetSpriteVertLoc(dstSpriteP));
        if (dv > 10 && dv < 20 && dh < 8 && dh > -40) {
            SWRemoveSprite(dstSpriteP);
            SWAddSprite(gFloorCandleStandSpriteLayerP, dstSpriteP);
        }
    }
}

void RemoveAllSpriteGlowsAndShadows(void)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP, riderP;

    for (layerP = gSpriteWorldP->headSpriteLayerP; layerP != NULL;
         layerP = SWGetNextSpriteLayer(gSpriteWorldP, layerP)) {
        for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP)) {
            riderP = spriteP->mountedSpriteP;
            if (riderP != NULL &&
                (riderP->userData == 0xcb ||
                 (riderP->userData == spriteP->userData && riderP->drawData->colorRed == 0))) {
                SWRemoveSprite(riderP);
                SWDisposeSprite(&spriteP->mountedSpriteP);
                spriteP->mountedSpriteP = NULL;
            }
        }
    }
}

void AddSpriteShadows(void)
{
    AddGlowForKeysAndCoins();
    if (gLEVLStruct.noShadows || gDisableDynamicShadows ||
        (gOldBackgroundPictResID != 1 && !gLEVLStruct.hasOutdoorShadows))
        return;

    AddShadowForSpritesInLayer(gCandleSpriteLayerP);
    AddShadowForSpritesInLayer(gCandleHolderSpriteLayerP);
    AddShadowForSpritesInLayer(gTrapDoorSpriteLayerP);
    AddShadowForSpritesInLayer(gOpenTrapDoorSpriteLayerP);
    AddShadowForSpritesInLayer(gDisappearingBlockSpriteLayerP);
    AddShadowForSpritesInLayer(gVertMovingPlatformSpriteLayerP);
    AddShadowForSpritesInLayer(gHorizMovingPlatformSpriteLayerP);
    AddShadowForSpritesInLayer(gFloorCandleStandSpriteLayerP);
    AddShadowForSpritesInLayerRange(gSignSpriteLayerP, gSavePedestalSpriteLayerP);
    AddShadowForSpritesInLayerRange(gSnakeSpriteLayerP, gObjectSpriteLayerP);
    AddShadowForSpritesInLayerRange(gMedusaSpriteLayerP, gDyingMedusaSpriteLayerP);
    if (SWStickyError() != kNoError)
        MM_FATAL(SWStickyError());
}

void AddShadowForSpritesInLayerRange(SpriteLayerPtr firstLayerP, SpriteLayerPtr lastLayerP)
{
    SpriteLayerPtr layerP;

    for (layerP = firstLayerP; layerP != NULL; layerP = SWGetNextSpriteLayer(gSpriteWorldP, layerP)) {
        AddShadowForSpritesInLayer(layerP);
        if (layerP == lastLayerP)
            break;
    }
}

void AddShadowForSpritesInLayer(SpriteLayerPtr layerP)
{
    SpritePtr spriteP;

    for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP))
        AddShadowForSprite(spriteP);
}

/// A translucent black copy mounted behind the sprite (keys and coins glow instead).
void AddShadowForSprite(SpritePtr spriteP)
{
    SpritePtr shadowP;

    if ((spriteP->userData >= 0x27 && spriteP->userData <= 0x2c) ||
        (spriteP->userData >= 0x20 && spriteP->userData <= 0x21))
        return;
    if (SWCloneSprite(spriteP, &shadowP, NULL, 0) != kNoError)
        return;
    SWAddSprite(gShadowSpriteLayerP, shadowP);
    SWSetSpriteMoveProc(shadowP, NULL);
    SWSetSpriteFrameProc(shadowP, NULL);
    SWMountSprite(spriteP, shadowP, 7.0f, -5.0f, true, false, true);
    SWSetMountInheritsAttributes(spriteP, false, false, true, true, true, true);
    SWSetSpriteColor(shadowP, 0, 0, 0);
    SWSetSpriteTranslucency(shadowP, 0x5fff);
}

void AddGlowForKeysAndCoins(void)
{
    SpritePtr spriteP;

    for (spriteP = SWGetNextSprite(gObjectSpriteLayerP, NULL); spriteP != NULL;
         spriteP = SWGetNextSprite(gObjectSpriteLayerP, spriteP)) {
        if ((spriteP->userData > 0x26 && spriteP->userData < 0x2d) ||
            (spriteP->userData > 0x1f && spriteP->userData < 0x22))
            AddGlowForKeyOrCoin(spriteP);
    }
}

/// A colored glow mounted on keys and coins (not the master key).
void AddGlowForKeyOrCoin(SpritePtr spriteP)
{
    long type = spriteP->userData;
    Boolean isKey = type >= 0x27 && type <= 0x2c, isCoin = type >= 0x20 && type <= 0x21;
    SpritePtr glowP;

    if (type == 0x2c)
        return;
    if (isKey && (gLEVLStruct.noKeyGlows || gDisableKeyGlows))
        return;
    if (isCoin && (gLEVLStruct.noCoinGlows || gDisableCoinGlows))
        return;
    glowP = NewGlowSprite(0xcb);
    if (glowP == NULL)
        return;

    SWRemoveSprite(glowP);
    SWAddSprite(gKeyGlowSpriteLayerP, glowP);
    SWMountSprite(spriteP, glowP, 7.0f, -5.0f, false, false, false);
    SWSetMountInheritsAttributes(spriteP, false, false, false, false, false, true);
    SWSetSpriteBlendMode(glowP, 0x302, 1);
    SWSetSpriteScaledSize(glowP, 90, 112);
    switch (type) {
        case 0x20:
            SWSetSpriteScaledSize(glowP, 50, 50);
            SWSetSpriteTranslucency(glowP, 0x132e);
            SWSetSpriteColor(glowP, 0xffff, 0xffff, 0);
            break;
        case 0x21:
            SWSetSpriteScaledSize(glowP, 50, 50);
            SWSetSpriteTranslucency(glowP, 0x1519);
            SWSetSpriteColor(glowP, 0xffff, 0xffff, 0xffff);
            break;
        case 0x27:
            SWSetSpriteTranslucency(glowP, 0x2e08);
            SWSetSpriteColor(glowP, 0xffff, 0x2fff, 0x2fff);
            break;
        case 0x28:
            SWSetSpriteTranslucency(glowP, 0x450c);
            SWSetSpriteColor(glowP, 0x2fff, 0x2fff, 0xffff);
            break;
        case 0x29:
            SWSetSpriteTranslucency(glowP, 0x18ef);
            SWSetSpriteColor(glowP, 0, 0xffff, 0);
            break;
        case 0x2a:
            SWSetSpriteTranslucency(glowP, 0x33c9);
            SWSetSpriteColor(glowP, 0xffff, 0, 0xffff);
            break;
        case 0x2b:
            SWSetSpriteTranslucency(glowP, 0x1cc5);
            SWSetSpriteColor(glowP, 0xffff, 0xffff, 0);
            break;
    }
    SWCenterSpriteOverSprite(glowP, spriteP);
    SWSetSpriteMountOffset(glowP, glowP->drawData->horizLoc - spriteP->drawData->horizLoc,
                           glowP->drawData->vertLoc - spriteP->drawData->vertLoc);
    if (isKey) {
        SWSetSpriteFrameProc(glowP, KeyGlowFrameProc);
        SWSetSpriteFrameTime(glowP, 0);
        glowP->misc1.n = (long)SWGetSpriteTranslucencyLevel(glowP);
    }
}

// ---------------------------------------------------------------------------
// Brick variations: plain brick tiles (1-210) are swapped for one of 12
// randomly chosen variations, keeping two-tile bricks together.

#define kNumBrickTiles      0xd2
#define kFirstBrickVariant  0x5a1

void ConvertTilesForCurRoom(void)
{
    short layer;

    ConvertTilesForCurRoomLayer4(4);
    ConvertTilesForCurRoomLayer6(6);
    for (layer = 0; layer < 7; layer++)
        SWResetTileMapForLayer(gSpriteWorldP, layer);
}

/// Bricks whose two halves start on even/odd columns alternating by row.
static Boolean IsFirstHalfOfBrick(unsigned short row, unsigned short col)
{
    return ((row & 1) == 0 || (col & 1) != 0) && ((row & 1) != 0 || (col & 1) == 0);
}

void ConvertTilesForCurRoomLayer4(short layer)
{
    TileMapStructPtr tileMapP = gSpriteWorldP->tileLayerArray[layer];
    short row, col, tile, nextTile, variant;

    for (row = 0; row < tileMapP->numRows; row++) {
        for (col = 0; col < tileMapP->numCols; col++) {
            tile = tileMapP->tileMap[row][col];
                // edge pieces next to plain bricks become plain bricks
            if (isNormalBrick(GetEditorTileID(tileMapP, row, col - 1)) ||
                isNormalBrick(GetEditorTileID(tileMapP, row, col + 1)) ||
                isNormalBrick(GetEditorTileID(tileMapP, row - 1, col)) ||
                isNormalBrick(GetEditorTileID(tileMapP, row + 1, col))) {
                if (tile == 0x12f || tile == 0x13d)
                    tile = 0x1e;
                if (tile == 0xe7 || tile == 0xf5)
                    tile = 1;
                if (tile == 0x10b || tile == 0x119)
                    tile = 0xf;
                if (tile == 0x14b || tile == 0x159)
                    tile = 0x2b;
                if (tile == 0x17b || tile == 0x189)
                    tile = 0x39;
            }
            if (tile <= 0 || tile > kNumBrickTiles)
                continue;

            variant = GetRandom(0, 11) * kNumBrickTiles + kFirstBrickVariant;
            tile += variant;
            if (tile >= gSpriteWorldP->maxNumTiles)
                continue;
            MMDrawTileAndFringeCallback(gSpriteWorldP, layer, row, col, tile);
            if (IsFirstHalfOfBrick(row, col) && col + 1 < tileMapP->numCols) {
                nextTile = tileMapP->tileMap[row][col + 1];
                if ((isLeftBrick(tile) && isRightBrick(nextTile)) || (isRightBrick(tile) && isLeftBrick(nextTile))) {
                    if (nextTile < 0x5a2)
                        tile = nextTile + variant;
                    MMDrawTileAndFringeCallback(gSpriteWorldP, layer, row, col + 1, tile);
                }
            }
        }
    }
}

void ConvertTilesForCurRoomLayer6(short layer)
{
    TileMapStructPtr tileMapP = gSpriteWorldP->tileLayerArray[layer];
    TileMapStructPtr wallMapP = gSpriteWorldP->tileLayerArray[4];
    short row, col, tile, otherTile, variant;
    Boolean otherInLayer4;

    for (row = 0; row < tileMapP->numRows; row++) {
        for (col = 0; col < tileMapP->numCols; col++) {
            tile = tileMapP->tileMap[row][col];
            if (tile <= 0 || tile > kNumBrickTiles)
                continue;
            variant = GetRandom(0, 11) * kNumBrickTiles + kFirstBrickVariant;
            tile += variant;
            if (tile >= gSpriteWorldP->maxNumTiles)
                continue;
            SWDrawTile(gSpriteWorldP, layer, row, col, tile);

            if (IsFirstHalfOfBrick(row, col)) {
                if (col + 1 >= tileMapP->numCols)
                    continue;
                    // the other half may be in layer 4
                otherTile = tileMapP->tileMap[row][col + 1];
                otherInLayer4 = otherTile == -1;
                if (otherInLayer4)
                    otherTile = ConvertTileID(wallMapP->tileMap[row][col + 1]);
                if ((isLeftBrick(tile) && isRightBrick(otherTile)) || (isRightBrick(tile) && isLeftBrick(otherTile))) {
                    if (otherTile < 0x5a2)
                        tile = otherTile + variant;
                    if (otherInLayer4) {
                        MMDrawTileAndFringeCallback(gSpriteWorldP, 4, row, col + 1, tile);
                    }
                    else {
                        SWDrawTile(gSpriteWorldP, layer, row, col + 1, tile);
                        UpgradeFringeForTile(gSpriteWorldP, layer, row, col + 1, variant);
                    }
                }
            }
            else if (col - 1 >= 0) {
                otherTile = tileMapP->tileMap[row][col - 1];
                otherInLayer4 = otherTile == -1;
                if (otherInLayer4)
                    otherTile = ConvertTileID(wallMapP->tileMap[row][col - 1]);
                if ((isLeftBrick(tile) && isRightBrick(otherTile)) || (isRightBrick(tile) && isLeftBrick(otherTile))) {
                    if (otherTile < 0x5a2)
                        tile = otherTile + variant;
                    if (otherInLayer4) {
                        SWDrawTile(gSpriteWorldP, 4, row, col - 1, tile);
                        UpgradeFringeForTile(gSpriteWorldP, 4, row, col - 1, variant);
                    }
                }
            }
        }
    }
}

Boolean isBrickTile(TileMapStructPtr tileMapP, short row, short col)
{
    short tile = GetEditorTileID(tileMapP, row, col);

    return tile >= 1 && tile <= kNumBrickTiles;
}

/// The fringe tiles above and to the right of a converted brick get the same variation.
void UpgradeFringeForTile(SpriteWorldPtr spriteWorldP, short layer, short row, short col, short variant)
{
    short firstLayer, lastLayer, fringeLayer;

    if (layer == 6) {
        firstLayer = 6;
        lastLayer = 6;
    }
    else {
        firstLayer = 1;
        lastLayer = 3;
    }
    for (fringeLayer = firstLayer; fringeLayer <= lastLayer; fringeLayer++) {
        UpdateFringeForTileLoc(spriteWorldP, fringeLayer, row - 1, col, variant);
        UpdateFringeForTileLoc(spriteWorldP, fringeLayer, row, col + 1, variant);
    }
}

void UpdateFringeForTileLoc(SpriteWorldPtr spriteWorldP, short layer, short row, short col, short variant)
{
    TileMapStructPtr tileMapP = spriteWorldP->tileLayerArray[layer];
    short tile;

    if (row < 0 || row >= tileMapP->numRows || col < 0 || col >= tileMapP->numCols)
        return;
    tile = tileMapP->tileMap[row][col];
    if (tile > 0 && tile < 0xd3 && isTileFringe(tile))
        SWDrawTile(gSpriteWorldP, layer, row, col, variant + tile);
}

/// Renders the room's static parts (background picture, tiles and their shadows)
/// into one background texture, so they don't have to be drawn every frame.
void DrawBackgroundTextureForCurRoom(void)
{
    SWBoolean dialogVisible, pane1Visible, pane2Visible, pane3Visible, pane4Visible, pausedVisible;
    GLenum glErr = glGetError();

    if (glErr != GL_NO_ERROR)
        fprintf(stderr, "\nGL error detected at %s, %s, %d, error: %s \n", __FILE__, __func__, __LINE__,
                BKGetGLErrorMessage(glErr));
    if (gCurrentGameMode != kPlayingGameMode)
        return;

    dialogVisible = gCustomDialogSpriteLayerP->layerIsVisible;
    pane1Visible = gOptionsDialogPane1LayerP->layerIsVisible;
    pane2Visible = gOptionsDialogPane2LayerP->layerIsVisible;
    pane3Visible = gOptionsDialogPane3LayerP->layerIsVisible;
    pane4Visible = gOptionsDialogPane4LayerP->layerIsVisible;
    pausedVisible = gPausedSpriteLayerP->layerIsVisible;
    SWSetSpriteLayerVisible(gCustomDialogSpriteLayerP, false);
    SWSetSpriteLayerVisible(gOptionsDialogPane1LayerP, false);
    SWSetSpriteLayerVisible(gOptionsDialogPane2LayerP, false);
    SWSetSpriteLayerVisible(gOptionsDialogPane3LayerP, false);
    SWSetSpriteLayerVisible(gOptionsDialogPane4LayerP, false);
    SWSetSpriteLayerVisible(gPausedSpriteLayerP, false);
    SWSetSpriteLayerVisible(gShadowSpriteLayerP, false);
    SWSetSpriteLayerVisible(gHaloSpriteLayerP, false);
    SWSetSpriteLayersVisible(gBottomSpriteLayerP, NULL, false);
    SWSetSpriteLayersVisible(gBackgroundScreenSpriteLayerP, gBackgroundPictureSpriteLayerP, true);
    SWSetTileLayersVisible(gSpriteWorldP, 0, 6, true);
    HideDisappearingFloorTiles();
    SWSetTileLayerCallback(gSpriteWorldP, 0, DrawTileShadowsCallback);
    SWChangeTileImage(gSpriteWorldP, 0x5a0, 0);         // spider webs come and go
    SWChangeTileImage(gSpriteWorldP, 0x5a1, 0);
    gSuppressExtrasOverlay = true;      // keep this version's overlay out of the cached background
    SWCreateBackgroundTextureFromTiles(gSpriteWorldP);
    gSuppressExtrasOverlay = false;
    SWSetTileLayerCallback(gSpriteWorldP, 0, NULL);
    SWSetSpriteLayerVisible(gShadowSpriteLayerP, true);
    SWSetSpriteLayerVisible(gHaloSpriteLayerP, true);
    SWSetSpriteLayersVisible(gBottomSpriteLayerP, NULL, true);
    SWSetSpriteLayersVisible(gBackgroundScreenSpriteLayerP, gBackgroundPictureSpriteLayerP, false);
    SWSetTileLayersVisible(gSpriteWorldP, 0, 5, false);
    RestoreDisappearingFloorTiles();
    SWSetSpriteLayerVisible(gCustomDialogSpriteLayerP, dialogVisible);
    SWSetSpriteLayerVisible(gOptionsDialogPane1LayerP, pane1Visible);
    SWSetSpriteLayerVisible(gOptionsDialogPane2LayerP, pane2Visible);
    SWSetSpriteLayerVisible(gOptionsDialogPane3LayerP, pane3Visible);
    SWSetSpriteLayerVisible(gOptionsDialogPane4LayerP, pane4Visible);
    SWSetSpriteLayerVisible(gPausedSpriteLayerP, pausedVisible);
    SWChangeTileImage(gSpriteWorldP, 0x5a0, 0x5a0);
    SWChangeTileImage(gSpriteWorldP, 0x5a1, 0x5a1);
}

/// Animated tiles (light saber and disappearing floors, lava) are kept out of the
/// background texture.
static void HideTileRange(short first, short last)
{
    short tile;

    for (tile = first; tile < last; tile++) {
        saveCurTileArray[tile] = gSpriteWorldP->curTileImage[tile];
        gSpriteWorldP->curTileImage[tile] = 0;
    }
}

void HideDisappearingFloorTiles(void)
{
    HideTileRange(0x2e9, 0x2f1);
    HideTileRange(0x2c1, 0x2c9);
    HideTileRange(0x319, 0x331);
}

void RestoreDisappearingFloorTiles(void)
{
    short tile;

    for (tile = 0x2e9; tile < 0x2f1; tile++)
        SWChangeTileImage(gSpriteWorldP, tile, saveCurTileArray[tile]);
    for (tile = 0x2c1; tile < 0x2c9; tile++)
        SWChangeTileImage(gSpriteWorldP, tile, saveCurTileArray[tile]);
    for (tile = 0x319; tile < 0x331; tile++)
        SWChangeTileImage(gSpriteWorldP, tile, saveCurTileArray[tile]);
}

short CompareSpriteLeft(SpritePtr a, SpritePtr b)
{
    return (short)(a->destFrameRect.left - b->destFrameRect.left);
}

// ---------------------------------------------------------------------------
// Sprites

/// Creates the room's sprites from its SPRT resource, skipping objects already
/// collected and enemies already killed. A big spider that caught something
/// comes back with its prey.
SWError LoadSpritesInRoom(short roomResID, Boolean includeHero)
{
    unsigned char *dataP, *recP;
    SPRTRecord rec;
    SpritePtr spriteP, preyP;
    long count, i;
    Boolean isGone, bigSpiderWithPrey;

    dataP = (unsigned char *)MMGetResource("SPRT", roomResID, GetSPRTResourceSize(roomResID) + 4);
    if (dataP == NULL)
        return -1;

    count = SDL_SwapBE32(*(Uint32 *)dataP);
    for (i = 0; i < count; i++) {
        recP = dataP + 4 + i * 0x16;
        rec.type = (short)SDL_SwapBE16(*(Uint16 *)(recP + 0));
        rec.v = (short)SDL_SwapBE16(*(Uint16 *)(recP + 2));
        rec.h = (short)SDL_SwapBE16(*(Uint16 *)(recP + 4));
        rec.param3 = (short)SDL_SwapBE16(*(Uint16 *)(recP + 6));
        rec.param4 = (short)SDL_SwapBE16(*(Uint16 *)(recP + 8));
        rec.param5 = (short)SDL_SwapBE16(*(Uint16 *)(recP + 10));
        rec.objectID = (Sint32)SDL_SwapBE32(*(Uint32 *)(recP + 12));
        if ((rec.type >= kFirstLeverSprite && rec.type <= kLastLeverSprite) ||
            (rec.type >= kFirstBrickSwitchSprite && rec.type <= kLastBrickSwitchSprite)) {
                // a 32-bit target object ID
            Uint32 target = SDL_SwapBE32(*(Uint32 *)(recP + 16));
            rec.param8 = (short)(target & 0xffff);
            rec.param9 = (short)(target >> 16);
        }
        else {
            rec.param8 = (short)SDL_SwapBE16(*(Uint16 *)(recP + 16));
            rec.param9 = (short)SDL_SwapBE16(*(Uint16 *)(recP + 18));
        }
        rec.param10 = (short)SDL_SwapBE16(*(Uint16 *)(recP + 20));

            // enemies, treasure and secrets in the object array are gone for good
        bigSpiderWithPrey = false;
        isGone = IsObjectInArray(rec.objectID) &&
                 ((rec.type >= 1 && rec.type <= 13) || (rec.type >= 0x1e && rec.type <= 0x2c) ||
                  rec.type == kSecretIndicatorSprite);
        if (isGone) {
            if (rec.type != kBigSpiderSprite)
                continue;
            bigSpiderWithPrey = true;
        }

        if (rec.type == kHeroSprite && !includeHero)
            continue;
        if (!ExtrasFilterRoomSprite(&rec))
            continue;
        spriteP = CreateMySprite(rec.type);
        if (spriteP == NULL)
            continue;

        SWSetSpriteLocation(spriteP, rec.h, rec.v);
        if (rec.type == kHeroSprite) {
            SaveHeroStartingLocation(rec.h, rec.v);
            gStartingFacingRight = HERO(spriteP)->facingRight;
            UpdateHaloSpritePosition(spriteP);
            gLastLevelPositionWasSaved = GEGetLevelNumber(roomResID, 1, 200);
            gLastLevelExited = gLastLevelPositionWasSaved;
            gLastMusicIndex = gCurrentMusicIndex;
            HERO(spriteP)->lastGroundVertLoc = (short)SWGetSpriteVertLoc(spriteP);
        }
        else if ((rec.type == 0x9e || rec.type == 0x9f || rec.type == 0xa0) && rec.param4 != 1) {
            LightTorch(spriteP);
        }
        SetUpSpriteFields(spriteP, &rec);
        SetUpSpriteFrameRange(spriteP);

        if (bigSpiderWithPrey) {
            SW_ASSERT(spriteP->userData == kBigSpiderSprite);
            preyP = IsSpriteInDoOnceArray(spriteP) ? NewWebTrappedHeroForSpider(spriteP)
                                                   : NewWebTrappedSprite(kWebTrappedEnemySprite);
            SPIDER(spriteP)->preyP = preyP;
            SPIDER(spriteP)->state = kSpiderFull;
            MovePreyToSpiderLocation(preyP, spriteP);
            SPIDER(spriteP)->frameDelay = 15;
            SPIDER(spriteP)->frameTimer = 15;
        }
    }
    free(dataP);
    return kNoError;
}

/// The GAME resource (128): the starting room and mansion-wide settings.
short LoadLevelResIDFromGAMEResource(void)
{
    unsigned char *gameP = (unsigned char *)MMGetResource("GAME", 128, 0x530);
    short resID = -1;

    if (gameP != NULL) {
        resID = (short)SDL_SwapBE16(*(Uint16 *)gameP);
        gTotalSecretsInMansion = (Sint32)SDL_SwapBE32(*(Uint32 *)(gameP + 12));
        free(gameP);
    }
    return resID;
}

void LoadRestOfGAMEResource(void)
{
    unsigned char *gameP = (unsigned char *)MMGetResource("GAME", 128, 0x530);

    if (gameP != NULL) {
        gTotalSecretsInMansion = (Sint32)SDL_SwapBE32(*(Uint32 *)(gameP + 12));
        gLavaSnd = 0x26;
        gDisableDynamicShadows = gameP[4];
        gDisableCoinGlows = gameP[5];
        gDisableKeyGlows = gameP[6];
        free(gameP);
    }
}

OSErr LoadMansionNameFromGAMEResource(StringPtr name)
{
    unsigned char *gameP = (unsigned char *)MMGetResource("GAME", 128, 0x530);

    if (gameP == NULL)
        return -1;
    SWCopyPascalString(gameP + 0x30, name);
    free(gameP);
    return noErr;
}

/// Loads the room's LEVL record and starts its music.
void LoadLEVLResource(short roomResID)
{
    unsigned char *levlP;
    char message[255];
    short levelNumber, levelResID, music, i;
    short *shortsP;

    levelNumber = GEGetLevelNumber(roomResID, 1, 200);
    levelResID = GEGetLevelResID(levelNumber, 1, 200);
    levlP = (unsigned char *)MMGetResource("LEVL", levelResID, sizeof(LEVLRec));
    if (levlP == NULL) {
        snprintf(message, sizeof(message), "Error: Can't load LEVL resource for room resID %d. ", roomResID);
        MMDoErrorMessage(message, 0);
        return;
    }
    memcpy(&gLEVLStruct, levlP, sizeof(LEVLRec));
    shortsP = &gLEVLStruct.musicIndex;
    for (i = 0; i < 9; i++)
        shortsP[i] = (short)SDL_SwapBE16((Uint16)shortsP[i]);

    if (gCurrentGameMode != kInGameMapMode) {
        gRoomIsDark = gLEVLStruct.isDark;
        music = gLEVLStruct.musicIndex;
        if (music >= 1 && music != gCurrentMusicIndex) {
            if (gCurrentMusicIndex > 0 || gCurrentMusicIndex == -1)
                DoGameFadeToBlack(30, true);
            PlayBackgroundMusic(music);
        }
        else if (music == -1 && gCurrentMusicIndex != -1) {
            if (gCurrentMusicIndex > 0)
                DoGameFadeToBlack(30, true);
            StopAndDisposeMusic();
            gCurrentMusicIndex = -1;
        }
    }
    free(levlP);
}

Boolean IsRoomDark(short roomResID)
{
    unsigned char *levlP = (unsigned char *)MMGetResource("LEVL", roomResID, sizeof(LEVLRec));
    char message[255];
    Boolean isDark = false;

    if (levlP == NULL) {
        snprintf(message, sizeof(message),
                 "Error: Can't load LEVL resource for levelResID %d, used by IsRoomDark() function.", roomResID);
        MMDoErrorMessage(message, 0);
    }
    else {
        isDark = levlP[0];
        free(levlP);
    }
    return isDark;
}

Boolean IsRoomASecretRoom(short roomResID)
{
    unsigned char *levlP = (unsigned char *)MMGetResource("LEVL", roomResID, sizeof(LEVLRec));
    char message[255];
    Boolean isSecret = false;

    if (levlP == NULL) {
        snprintf(message, sizeof(message),
                 "Error: Can't load LEVL resource for levelResID %d, used by IsRoomASecretRoom() function.",
                 roomResID);
        MMDoErrorMessage(message, 0);
    }
    else {
        isSecret = levlP[1];
        free(levlP);
    }
    return isSecret;
}

SpritePtr CreateMySprite(short type)
{
    SpritePtr spriteP = NULL;

    if (type == kHeroSprite)
        spriteP = NewMainSprite();
    else if (type == kSkullSpiderSprite)
        spriteP = NewSkullSpiderSprite();
    else if (type == kScorpionSprite)
        spriteP = NewScorpionSprite();
    else if (type == kEyeballSprite)
        spriteP = NewEyeballSprite();
    else if (type == kBlueRavenSprite || type == kRedRavenSprite)
        spriteP = NewRavenSprite(type);
    else if (type == kSnakeSprite1 || type == kSnakeSprite2)
        spriteP = NewSnakeSprite(type);
    else if (type == kMedusaHeadSprite1 || type == kMedusaHeadSprite2)
        spriteP = NewMedusasHeadSprite(type);
    else if (type == kSmallSpiderSprite || type == kBigSpiderSprite)
        spriteP = NewSpiderSprite(type);
    else if (type == kSkeletonFishSprite)
        spriteP = NewSkeletonFishSprite(kSkeletonFishSprite);
    else if (type == kWalkingSkeletonSprite)
        spriteP = NewWalkingSkeletonSprite();
    else if (type >= kFirstLeverSprite && type <= kLastLeverSprite)
        spriteP = NewLeverSprite(type);
    else if (type >= kFirstBrickSwitchSprite && type <= kLastBrickSwitchSprite)
        spriteP = NewBrickSwitchSprite(type);
    else if (type >= kFirstDoorSprite && type <= kLastDoorSprite)
        spriteP = NewDoorSprite(type);
    else if (type >= kFirstMovingPlatformSprite && type <= kLastMovingPlatformSprite)
        spriteP = NewMovingPlatformSprite(type);
    else if (type >= kFirstFallingPlatformSprite && type <= 0x5e)
        spriteP = NewFallingPlatformSprite(type);
    else if (type == 0x5f || type == 0x60)
        spriteP = NewTrapDoorSprite(type);
    else if (type >= kFirstObjectSprite && type <= kLastObjectSprite)
        spriteP = NewObjectSprite(type);
    else if (type >= kFirstZapperNodeSprite && type <= kLastZapperNodeSprite)
        spriteP = NewZapperNodeSprite(type);
    else if (type >= kFirstBackgroundSprite && type <= kLastBackgroundSprite)
        spriteP = NewBackgroundSprite(type);
    else if (type >= kFirstPedestalSprite && type <= kLastPedestalSprite)
        spriteP = NewPedestalOrStatueSprite(type);
    else if (type >= kFirstTorchSprite && type <= kLastTorchSprite)
        spriteP = NewTorchSprite(type);
    else if (type >= kFirstCandleSprite && type <= kLastCandleSprite)
        spriteP = NewCandleSprite(type);
    else if (type >= kFirstCandleHolderSprite && type <= kLastCandleHolderSprite)
        spriteP = NewCandleHolderSprite(type);
    else if (type >= kFirstTreasureSprite && type <= kLastTreasureSprite)
        spriteP = NewTreasureSprite(type);
    else if (type == kExitSprite)
        spriteP = NewExitSprite();
    else if (type == kSecretIndicatorSprite)
        spriteP = NewSecretIndicatorSprite();
    else {
        printf("spriteID = %d", type);
        SW_ASSERT(false);
    }
    SW_ASSERT(spriteP != NULL);
    return spriteP;
}

/// The room's background picture (Screens/background<n>.jpg or .png, from its BGND
/// resource). Dark rooms without the lantern get background 1, plain black.
void MMLoadExtraBackFrameForRoom(SpriteWorldPtr spriteWorldP, short roomResID)
{
    unsigned char *bgndP;
    char message[255], jpgName[255], pngName[255];
    short pictID, background;
    Boolean isDark;

    (void)spriteWorldP;
    bgndP = (unsigned char *)MMGetResource("BGND", roomResID, 12);
    if (bgndP == NULL) {
        snprintf(message, sizeof(message),
                 "Error: Can't load LEVL resource for roomResID %d, used by MMLoadExtraBackFrameForRoom().",
                 roomResID);
        MMDoErrorMessage(message, 0);
        return;
    }
    pictID = (short)SDL_SwapBE16(*(Uint16 *)bgndP);
    free(bgndP);
    if (pictID < 10000)
        pictID = 10000;
    if (gUsingEditor && gCurrentGameMode == 25)
        isDark = false;
    else
        isDark = IsRoomDark(roomResID);
    if (isDark && !gObjects.hasLantern)
        pictID = 10000;

    background = pictID - 9999;
    if (background == gOldBackgroundPictResID && SWGetNextSprite(gBackgroundScreenSpriteLayerP, NULL) != NULL)
        return;
    gOldBackgroundPictResID = background;
    snprintf(jpgName, sizeof(jpgName), "background%d.jpg", background);
    snprintf(pngName, sizeof(pngName), "background%d.png", background);
    if (LoadAndInstallBackgroundSprite(NULL, GetScreensFolderPath(), jpgName, background, false) != kNoError)
        LoadAndInstallBackgroundSprite(NULL, GetScreensFolderPath(), pngName, background, false);
}

/// Applies a SPRT record's parameters to a new sprite (their meaning depends on
/// the sprite type).
void SetUpSpriteFields(SpritePtr spriteP, SPRTRecord *recP)
{
    short type = recP->type, direction;
    float speed, h, v;
    short delay;
    SpritePtr hintP;

    SW_ASSERT(type >= 0 && type <= 0xe3);
    GAME_SPRITE(spriteP)->objectID = recP->objectID;
    if (type == kHeroSprite)
        return;

    if (type >= 0xa1 && type <= 0xa6) {
        CreateGlowOverSprite(spriteP);
    }
    else if ((type >= kFirstLeverSprite && type <= kLastLeverSprite) ||
             (type >= kFirstBrickSwitchSprite && type <= kLastBrickSwitchSprite)) {
        LEVER(spriteP)->action = recP->param3;
        LEVER(spriteP)->targetObjectID = (Sint32)(((Uint32)(Uint16)recP->param9 << 16) | (Uint16)recP->param8);
        LEVER(spriteP)->dontRememberState = (Boolean)recP->param4;
        LEVER(spriteP)->isInverted = (Boolean)recP->param5;
        LEVER(spriteP)->isOneShot = (Boolean)recP->param10;
    }
    else if (type >= kFirstDoorSprite && type <= kLastDoorSprite) {
        DOOR(spriteP)->startsOpen = recP->param3 == 1;
    }
    else if (type >= 0x70 && type <= 0x84) {
        if (type <= 0x74)
            SWAddSprite(recP->param3 == 1 ? gTopSpriteLayerP : gForegroundSpriteLayerP, spriteP);
        else
            SWAddSprite(recP->param3 == 1 ? gForegroundSpriteLayerP : gBackgroundSpriteLayerP, spriteP);
    }
    else if (type >= 0x85 && type <= 0x9d) {
        if (type >= 0x94)
            SWAddSprite(gTopSpriteLayerP, spriteP);
        else
            SWAddSprite(recP->param3 == 1 ? gTopSpriteLayerP : gForegroundSpriteLayerP, spriteP);
    }
    else if ((type >= 0x27 && type <= 0x2c) || type == 0x22 || type == 0x23) {
        if (recP->param3 == 1)
            SWSetSpriteMoveProc(spriteP, FallingObjectMoveProc);
    }
    else if (type == kLanternObjectSprite) {
        CreateGlowOverSprite(spriteP);
    }
    else if (type == 0x1a || type == 0x1d) {
        ZapperNodeSpritePtr zP = ZAPPER(spriteP);

        if (recP->param3 == -1) {
                // always on
            zP->isOn = true;
            zP->timer = 10;
            zP->onTime = 120;
            zP->offTime = 120;
        }
        else {
            if (recP->param3 == 0 || gCurrentGameMode == kInGameMapMode) {
                zP->isOn = true;
                zP->timer = recP->param4;
                SWSetSpriteFrameAdvance(spriteP, 1);
            }
            else {
                zP->isOn = false;
                zP->timer = recP->param3;
                SWSetSpriteFrameAdvance(spriteP, 0);
            }
            zP->onTime = recP->param4;
            zP->offTime = recP->param5;
        }
    }
    else if (type >= 0xb1 && type <= 0xb6) {
        SWAddSprite(recP->param3 == 1 ? gTopSpriteLayerP : gForegroundSpriteLayerP, spriteP);
    }
    else if (type == kSignObjectSprite) {
        ObjectSpritePtr signP = OBJECT(spriteP);

        signP->signTextResID = recP->param3;
        signP->signPictResID = recP->param4;
        signP->isVisible = recP->param5 != 1;
        if (recP->param5 == 1)
            SWSetSpriteVisible(spriteP, false);
        signP->isTip = recP->param10 == 1;
        if (signP->isTip && (hintP = NewMiscSprite(kHintQuestionMarkSprite)) != NULL)
            SWSetSpriteLocation(hintP, SWGetSpriteHorizLoc(spriteP), SWGetSpriteVertLoc(spriteP));
    }
    else if (type == 0x25) {
        OBJECT(spriteP)->mapGridH = recP->param3;
        OBJECT(spriteP)->mapGridV = recP->param4;
    }
    else if (type == kSkeletonFishSprite) {
        FishSpritePtr fishP = FISH(spriteP);

        fishP->restVertLoc = (short)SWGetSpriteVertLoc(spriteP);
        fishP->waitTimer = recP->param3;
        fishP->waitTime = recP->param4;
        fishP->jumpSpeed = recP->param5 / 3.0f;
        fishP->gravity = recP->param10 / 20.0f;
        if (fishP->waitTimer == 0)
            StartSkeletonFishJump(spriteP);
    }
    else if (type == kSmallSpiderSprite) {
        SpiderSpritePtr spiderP = SPIDER(spriteP);

        spiderP->dropAccel = recP->param3 / 100.0f;
        spiderP->minDropSpeed = recP->param4 / 20.0f;
        spiderP->maxDropSpeed = recP->param5 / 20.0f;
        spiderP->climbSpeed = recP->param10 / 6.0f;
        spiderP->waitTime = recP->param8;
        spiderP->bounce = recP->param9 / 100.0f;
        if (spiderP->bounce > 1.0f)
            spiderP->bounce = 1.0f;
        if (gCurrentGameMode == kPlayingGameMode)
            spiderP->dropSpeed = GetMovieCompatibleRandomFloat(spiderP->minDropSpeed, spiderP->maxDropSpeed);
        else
            spiderP->dropSpeed = GetRandomFloat(spiderP->minDropSpeed, spiderP->maxDropSpeed);
    }
    else if (type == kRedRavenSprite) {
        SWSetSpriteMoveDelta(spriteP, recP->param3, 0);
        RAVEN(spriteP)->amplitude = recP->param4;
        RAVEN(spriteP)->angleStep = recP->param5 / 5;
        RAVEN(spriteP)->ignoresWalls = (Boolean)recP->param10;
    }
    else if (type == kMedusaHeadSprite1 || type == kMedusaHeadSprite2) {
        MEDUSA(spriteP)->speed = (recP->param4 < 1 ? 0.2f : recP->param4 / 20.0f) * ExtrasMedusaSpeedFactor();
    }
    else if (type == kWalkingSkeletonSprite) {
        SkeletonSpritePtr skeletonP = SKELETON(spriteP);

        skeletonP->wakeDelay = recP->param3;
        skeletonP->wakeDistance = recP->param4;
        skeletonP->speed = recP->param5 / 2.0f;
        skeletonP->wakeMode = recP->param10;
        skeletonP->wakeTimer = skeletonP->wakeDelay;
        if (skeletonP->wakeMode > 0)
            SWSetSpriteMoveProc(spriteP, SkeletonWaitingToUncollapseMoveProc);
    }
    else if ((type >= 1 && type <= 13) || (type >= kFirstMovingPlatformSprite && type <= kLastMovingPlatformSprite)) {
            // walking/flying enemies and moving platforms: direction and speed
        direction = recP->param3;
        speed = recP->param4;
        if (direction == -1)
            direction = 2;
        if (speed == -1.0f)
            speed = GetDefaultSpriteSpeed(type);

        if (type == kSkullSpiderSprite) {
            delay = (short)(speed + 5.0f);
            if (delay > 11)
                delay = 12;
            SKULLSPIDER(spriteP)->frameDelay = 60 / delay;
            SKULLSPIDER(spriteP)->frameTimer = 60 / delay;
            if (direction == 2)
                SWSetSpriteFlipX(spriteP, true);
            if (speed <= 2.0f)
                SKULLSPIDER(spriteP)->stepSound = 42;
            else if (speed == 3.0f)
                SKULLSPIDER(spriteP)->stepSound = 43;
            else
                SKULLSPIDER(spriteP)->stepSound = 44;
        }

            // direction: 0 left, 1 up, 2 right, 3 down
        speed /= 2.0f;
        h = 0;
        v = 0;
        if (direction == 2)
            h = speed;
        else if (direction == 0)
            h = -speed;
        else if (direction == 1)
            v = -speed;
        else if (direction == 3)
            v = speed;
        SWSetSpriteMoveDelta(spriteP, h, v);

        if (type >= kFirstMovingPlatformSprite && type <= kLastMovingPlatformSprite &&
            spriteP->userData != kPlatformBlockerSprite) {
            if (SWGetSpriteHorizMoveDelta(spriteP) != 0) {
                SWSetSpriteMoveProc(spriteP, MovingPlatformHorizMoveProc);
                SWAddSprite(gHorizMovingPlatformSpriteLayerP, spriteP);
            }
            else {
                SWSetSpriteMoveProc(spriteP, MovingPlatformVertMoveProc);
                SWAddSprite(gVertMovingPlatformSpriteLayerP, spriteP);
            }
            PLATFORM(spriteP)->isMoving = (Boolean)recP->param5;
        }
    }
}

/// Ravens and scorpions moving right face right.
void SetUpSpriteFrameRange(SpritePtr spriteP)
{
    if ((spriteP->userData == kBlueRavenSprite || spriteP->userData == kRedRavenSprite ||
         spriteP->userData == kScorpionSprite) && SWGetSpriteHorizMoveDelta(spriteP) > 0)
        SWSetSpriteFlipX(spriteP, true);
}

short GetDefaultSpriteSpeed(short type)
{
    if (type >= kFirstMovingPlatformSprite && type <= kLastMovingPlatformSprite)
        return 2;
    switch (type) {
        case 1: return 3;
        case 2: return 2;
        case 3: return 3;
        case 4: return 4;
        case 5: return 2;
        case 8:
        case 9: return 4;
        case 13: return 2;
        default: return 0;
    }
}

/// Disposes of the room's sprites and tile maps (the hero too, if disposeHero).
void DisposeCurrentRoom(Boolean disposeHero)
{
    SpriteLayerPtr layerP = NULL;
    short layer;

    while ((layerP = SWGetNextSpriteLayer(gSpriteWorldP, layerP)) != NULL) {
        if (((layerP != gHeroSpriteLayerP && layerP != gHeroOnPlatformSpriteLayerP) || disposeHero) &&
            layerP != gBackgroundScreenSpriteLayerP && layerP != gFadeToBlackSpriteLayerP)
            SWDisposeAllSpritesInLayer(layerP);
    }
    SWDisposeAllSpritesInLayer(gHiddenTriggerItemsSpriteLayerP);
    for (layer = 0; layer < 7; layer++) {
        if (gSpriteWorldP->tileLayerArray[layer] != NULL)
            SWDisposeTileMap(&gSpriteWorldP->tileLayerArray[layer]);
        SWInstallTileMap(gSpriteWorldP, NULL, layer);
    }
    if (SWStickyError() != kNoError)
        MM_FATAL(SWStickyError());
}

/// Lava surface tiles (0x309-0x30c) go to layer 6, drawn in front of sprites that
/// sink into it. Returns true if the room has lava.
Boolean MoveLavaToLayer6(void)
{
    TileMapStructPtr wallMapP = gSpriteWorldP->tileLayerArray[4], frontMapP = gSpriteWorldP->tileLayerArray[6];
    short row, col, tile;
    Boolean hasLava = false;

    SW_ASSERT(wallMapP != NULL);
    SW_ASSERT(frontMapP != NULL);
    for (row = 0; row < wallMapP->numRows; row++) {
        for (col = 0; col < wallMapP->numCols; col++) {
            tile = wallMapP->tileMap[row][col];
            if (tile > 0x308 && tile < 0x30d) {
                wallMapP->tileMap[row][col] = -1;
                frontMapP->tileMap[row][col] = tile;
                hasLava = true;
            }
        }
    }
    return hasLava;
}

/// Glows over the lava in layers 1-3: one stretched glow per run of lava (0x329+),
/// and single glows for the lava edges.
void AddGlowToLava(void)
{
    TileMapStructPtr tileMapP;
    SpritePtr glowP;
    short layer, row, col, endCol, tile, kind;

    for (layer = 1; layer <= 3; layer++) {
        tileMapP = gSpriteWorldP->tileLayerArray[layer];
        SW_ASSERT(tileMapP != NULL);
        for (row = 0; row < tileMapP->numRows; row++) {
            for (col = 0; col < tileMapP->numCols; col++) {
                tile = tileMapP->tileMap[row][col];
                if (tile <= 0x318 || tile >= 0x331)
                    continue;
                if (tile <= 0x320) {
                    kind = 0;
                }
                else if (tile <= 0x328) {
                    kind = 2;
                    if (IsWallTile(gSpriteWorldP->tileLayerArray[4]->tileMap[row][col]))
                        continue;
                }
                else {
                    kind = 1;
                }

                endCol = col;
                if (kind == 1) {
                    do {
                        endCol++;
                    } while (endCol < tileMapP->numCols && tileMapP->tileMap[row][endCol] >= 0x329 &&
                             tileMapP->tileMap[row][endCol] < 0x331);
                }
                glowP = NewMiscSprite(0xd4);
                SWAddSprite(gLavaGlowSpriteLayerP, glowP);
                SWSetSpriteBlendMode(glowP, 0x302, 1);
                SWSetCurrentFrameIndex(glowP, kind);
                SWSetSpriteLocation(glowP, col << 4, row * 16 - 20);
                if (kind == 1) {
                    SWSetSpriteScaledWidth(glowP, (endCol - col) * 16);
                    col = endCol - 1;
                }
                SWSetSpriteScaledHeightFactor(glowP, 0.58f);
                if (kind == 0)
                    SWOffsetSprite(glowP, -16.0f, 0);
            }
        }
    }
}

void OpenDoorsThatWereOpenedByPlayer(void)
{
    SpritePtr doorP, nextP;

    doorP = SWGetNextSprite(gDoorSpriteLayerP, NULL);
    while (doorP != NULL) {
        nextP = SWGetNextSprite(gDoorSpriteLayerP, doorP);
        if (IsSpriteInObjectArray(doorP)) {
            SWCollideSpriteWithLayer(doorP, gDoorSpriteLayerP, OpenDoorInstantlyCollideProc);
            SetDoorToOpenedState(doorP);
        }
        doorP = nextP;
        if (nextP != NULL && nextP->parentSpriteLayerP != gDoorSpriteLayerP)
            doorP = SWGetNextSprite(gDoorSpriteLayerP, NULL);
    }
}

void OpenDoorsThatAreOpenToStart(void)
{
    SpritePtr doorP, nextP;

    doorP = SWGetNextSprite(gDoorSpriteLayerP, NULL);
    while (doorP != NULL) {
        nextP = SWGetNextSprite(gDoorSpriteLayerP, doorP);
        if (DOOR(doorP)->startsOpen && !IsSpriteInDoOnceArray(doorP)) {
            SWCollideSpriteWithLayer(doorP, gDoorSpriteLayerP, OpenDoorInstantlyAndAddCollideProc);
            SetDoorToOpenedState(doorP);
            AddSpriteToObjectArray(doorP);
            AddSpriteToDoOnceArray(doorP);
        }
        doorP = nextP;
        if (nextP != NULL && nextP->parentSpriteLayerP != gDoorSpriteLayerP)
            doorP = SWGetNextSprite(gDoorSpriteLayerP, NULL);
    }
}

/// The pedestal the game was saved on shows the stone statue; used ones are dark.
void CreateGrayStatueOnSavePedestals(void)
{
    SpritePtr pedestalP, statueP;

    for (pedestalP = SWGetNextSprite(gSavePedestalSpriteLayerP, NULL); pedestalP != NULL;
         pedestalP = SWGetNextSprite(gSavePedestalSpriteLayerP, pedestalP)) {
        if (IsSpriteInObjectArray(pedestalP) && GAME_SPRITE(pedestalP)->objectID == gLastSavePedestalObjectID) {
            statueP = NewStatueSprite();
            if (statueP != NULL) {
                SWSetSpriteLocation(statueP, SWGetSpriteHorizLoc(pedestalP) - 1.0f,
                                    SWGetSpriteVertLoc(pedestalP) - 59.0f);
                SWSetCurrentFrameIndex(statueP, 1);
            }
        }
        if (IsSpriteInObjectArray(pedestalP) || IsSpriteInDoOnceArray(pedestalP))
            SWSetCurrentFrameIndex(pedestalP, 1);
    }
}

/// In a dark room (without the lantern) only the hero, enemies, objects and lava
/// show; doors are drawn invisible but still block.
void MakeRoomDarkOrLight(Boolean makeDark, Boolean wasDark)
{
    SpriteLayerPtr layerP;
    SpritePtr spriteP;
    short tile, type;

    if (gObjects.hasLantern)
        makeDark = false;

    if (makeDark) {
        for (tile = 1; tile < 0x5a2; tile++) {
            if (tile < 0x309 || tile > 0x330)
                SWChangeTileImageFast(gSpriteWorldP, tile, 0);
        }
        for (layerP = SWGetNextSpriteLayer(gSpriteWorldP, NULL); layerP != NULL;
             layerP = SWGetNextSpriteLayer(gSpriteWorldP, layerP)) {
            if (layerP == gLanternGlowSpriteLayerP)
                continue;
            for (spriteP = SWGetNextSprite(layerP, NULL); spriteP != NULL; spriteP = SWGetNextSprite(layerP, spriteP)) {
                type = (short)spriteP->userData;
                if (type != 0 && (type < 1 || type > 13) && (type < 0x1e || type > 0x2c) &&
                    type != 0xb9 && type != 0xba && type != 0xbc && type != 0xbd) {
                    if (type >= kFirstDoorSprite && type <= kLastDoorSprite)
                        SWSetSpriteTranslucency(spriteP, 0);
                    else
                        SWSetSpriteVisible(spriteP, false);
                }
                if (type == 0x1e || type == 0x1f || type == kSignObjectSprite)
                    SWSetSpriteVisible(spriteP, false);
            }
        }
    }
    else if (wasDark) {
        SWResetVBOForAllTileLayers(gSpriteWorldP);
        SWResetCurrentTileImages(gSpriteWorldP);
        ResetFloorTileAnimations(true, true);
        HideLightSaberFloorsIfOff(gSpriteWorldP, true);
        SetCorrectGoldSilverTileImages(true);
        DrawBackgroundTextureForCurRoom();
    }
    gRoomIsDark = makeDark;
}

/// After dying: lose a life and reappear, or game over.
void RestartLevel(void)
{
    SpritePtr heroP;

    gHeroLandedThisFrame = false;
    if (gNumLivesLeft < 1) {
        gGameOver = true;
        return;
    }
    if (!ExtrasKeepLife())          // "Nine-Lived Malone"
        gNumLivesLeft--;
    MultiplayerNextTurn();          // relay co-op: the next player comes in
    heroP = NewMainSprite();
    if (heroP != NULL) {
        SWRemoveSprite(heroP);
        SWAddSprite(gDeadHeroSpriteLayerP, heroP);
        SWSetSpriteVisible(heroP, false);
        SWSetSpriteMoveProc(heroP, NULL);
        SWSetSpriteFrameProc(heroP, PauseThenReappearFrameProc);
        SWSetSpriteFrameTime(heroP, 0);
        HERO(heroP)->reappearTimer = 20;
    }
}

void LoadLevel(short levelNumber, Boolean includeHero)
{
    GLenum glErr;
    short roomResID;

    if (SWStickyError() != kNoError)
        MM_FATAL(SWStickyError());
    glErr = glGetError();
    if (glErr != GL_NO_ERROR)
        fprintf(stderr, "\nGL error detected at %s, %s, %d, error: %s \n", __FILE__, __func__, __LINE__,
                BKGetGLErrorMessage(glErr));

    gCurrentLevel = levelNumber;
    gRoomWasDark = gObjects.hasLantern ? false : gRoomIsDark;
    roomResID = GEGetLevelResID(gCurrentLevel, 1, 200);
    LoadLEVLResource(roomResID);
    DisposeCurrentRoom(includeHero);
    LoadRoom(roomResID, includeHero);
    if (gCurrentGameMode != kInGameMapMode)
        CreateFPSAndLevelResIDSprites();
    gNumFramesSinceEnteringRoom = 0;
    if (gDoFrameSkipping) {
        gDoFrameSkipping = false;
        SWSetSpriteWorldMaxFPS(gSpriteWorldP, 60);
    }
    gRemovedGlowsAndShadows = false;
    if (SWStickyError() != kNoError)
        MM_FATAL(SWStickyError());
}

void SetUpLevelForPlaying(void)
{
    gCurrentRoomHasLava = MoveLavaToLayer6();
    AddGlowToLava();
    ResetFloorTileAnimations(false, false);
    DrawBackgroundTextureForCurRoom();
    if (DoesLayerHaveTiles(gSpriteWorldP, 6, 0x59d, 0x59f))
        SWSetTileLayerToDrawEveryFrame(gSpriteWorldP, 6, true);
    if (!gCurrentRoomHasLava)
        StopChannel(2);
    else if (IsChannelAvailable(2))
        MMPlayStereoSound2(NULL, gLavaSnd, 2, 2, (short)(gSoundVolume * 0.85), 1.0, -1);
    StopMySound(0x32);
    AddRoomToArray(gCurrentLevel);
}

void DrawLevel(void)
{
    DrawStatsArea();
    SWUpdateSpriteWorld(gSpriteWorldP, false);
}
