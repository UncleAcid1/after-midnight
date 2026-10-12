/*
 * SpriteWorld X fork — tile maps and tile layers.
 * See SWCommonHeaders.h for license and provenance.
 *
 * Up to kNumTileLayers tile maps can be installed. A sprite layer can have one tile layer
 * drawn below and one above its sprites.
 *
 * Background cache: SWCreateBackgroundTextureFromTiles captures the drawn screen into a
 * sprite that is then drawn in place of clearing. The game hides the captured tile
 * layers afterwards; tiles changed with SWDrawTile are recorded in each map's
 * changedTileMap and drawn over the cached background.
 */

#include "SpriteWorld.h"

static SWBoolean gIsDrawingTilesAboveSprite = false;
long gNumTilesDrawnThisFrame = 0;

static void SWResetChangedTilesMap(TileMapStructPtr tileMapP);
static void SWMarkTilesToBeRedrawn(SpriteWorldPtr spriteWorldP, short tileLayer, SWRect *updateRectP);

SWError SWInitTiling(SpriteWorldPtr spriteWorldP, short tileHeight, short tileWidth, short maxNumTiles)
{
    SWError err = kNoError;
    short i;

    SW_ASSERT(spriteWorldP != NULL);
    if (spriteWorldP->tilingIsInitialized)
        return kTilingAlreadyInitialized;

    spriteWorldP->tilingIsOn = true;
    spriteWorldP->tilingIsInitialized = true;
    spriteWorldP->lastActiveTileLayer = 0;
    spriteWorldP->maxNumTiles = maxNumTiles;
    spriteWorldP->tileHeight = tileHeight;
    spriteWorldP->tileWidth = tileWidth;
    spriteWorldP->tileImageWidth = tileWidth;
    spriteWorldP->tileImageHeight = tileHeight;

    spriteWorldP->tileFrameArray = calloc(maxNumTiles, sizeof(FramePtr));
    spriteWorldP->tileColorArray = calloc(maxNumTiles, sizeof(TileColorRec));
    spriteWorldP->curTileImage = calloc(maxNumTiles, sizeof(short));
    spriteWorldP->tileSourceIndex = calloc(maxNumTiles, sizeof(short));
    spriteWorldP->tileLayerArray = calloc(kNumTileLayers, sizeof(TileMapStructPtr));
    if (!spriteWorldP->tileFrameArray || !spriteWorldP->tileColorArray || !spriteWorldP->curTileImage ||
        !spriteWorldP->tileSourceIndex || !spriteWorldP->tileLayerArray)
        return kMemoryAllocationError;

    for (i = 0; i < maxNumTiles; i++) {
        spriteWorldP->tileColorArray[i].hasColor = false;
        spriteWorldP->tileColorArray[i].red = 0xFFFF;
        spriteWorldP->tileColorArray[i].green = 0xFFFF;
        spriteWorldP->tileColorArray[i].blue = 0xFFFF;
        spriteWorldP->tileColorArray[i].alpha = 0xFFFF;
        spriteWorldP->curTileImage[i] = i;
        spriteWorldP->tileSourceIndex[i] = i;
    }
    return err;
}

void SWExitTiling(SpriteWorldPtr spriteWorldP)
{
    SW_ASSERT(spriteWorldP != NULL);
    if (!spriteWorldP->tilingIsInitialized)
        return;
    free(spriteWorldP->tileFrameArray);
    free(spriteWorldP->curTileImage);
    free(spriteWorldP->tileLayerArray);
    free(spriteWorldP->tileColorArray);
    free(spriteWorldP->tileSourceIndex);
    spriteWorldP->tileFrameArray = NULL;
    spriteWorldP->curTileImage = NULL;
    spriteWorldP->tileLayerArray = NULL;
    spriteWorldP->tileColorArray = NULL;
    spriteWorldP->tileSourceIndex = NULL;
    spriteWorldP->tilingIsInitialized = false;
    spriteWorldP->tilingIsOn = false;
}

/// Pixel size of tile images, when it differs from the tile grid (images get scaled).
void SWSetTileResolution(SpriteWorldPtr spriteWorldP, short tileImageWidth, short tileImageHeight)
{
    spriteWorldP->tileImageWidth = tileImageWidth;
    spriteWorldP->tileImageHeight = tileImageHeight;
}

static void SWFreeTileMapRows(TileMapStructPtr t)
{
    short row;

    for (row = 0; row < t->numRows; row++) {
        if (t->tileMap) free(t->tileMap[row]);
        if (t->changedTileMap) free(t->changedTileMap[row]);
        if (t->tileChangedFlags) free(t->tileChangedFlags[row]);
    }
    free(t->tileMap);
    free(t->changedTileMap);
    free(t->tileChangedFlags);
}

SWError SWCreateTileMap(TileMapStructPtr *tileMapStructPP, short numTileMapRows, short numTileMapCols)
{
    TileMapStructPtr t;
    short row;
    SWError err = kNoError;

    t = calloc(1, sizeof(TileMapStruct));
    if (t == NULL)
        err = kMemoryAllocationError;
    if (err == kNoError) {
        t->numRows = numTileMapRows;
        t->numCols = numTileMapCols;
        t->zoom = 1.0f;
        t->isVisible = true;
        t->drawEveryFrame = false;
        t->tilesChanged = false;
        t->tileMap = calloc(numTileMapRows, sizeof(short *));
        t->changedTileMap = calloc(numTileMapRows, sizeof(short *));
        t->tileChangedFlags = calloc(numTileMapRows, sizeof(unsigned char *));
        if (!t->tileMap || !t->changedTileMap || !t->tileChangedFlags)
            err = kMemoryAllocationError;
    }
    if (err == kNoError) {
        for (row = 0; row < numTileMapRows; row++) {
            t->tileMap[row] = calloc(numTileMapCols, sizeof(short));
            t->changedTileMap[row] = calloc(numTileMapCols, sizeof(short));
            t->tileChangedFlags[row] = calloc(numTileMapCols, 1);
            if (!t->tileMap[row] || !t->changedTileMap[row] || !t->tileChangedFlags[row]) {
                err = kMemoryAllocationError;
                break;
            }
        }
    }
    if (err == kNoError) {
        SWResetTileMap(t);
    } else if (t != NULL) {
        SWFreeTileMapRows(t);
        free(t);
        t = NULL;
    }
    *tileMapStructPP = t;
    return err;
}

SWError SWCloneTileMap(TileMapStructPtr srcTileMapP, TileMapStructPtr *newTileMapPP)
{
    SWError err;
    short row, col;

    SW_ASSERT(srcTileMapP->numRows > 0 && srcTileMapP->numCols > 0);
    err = SWCreateTileMap(newTileMapPP, srcTileMapP->numRows, srcTileMapP->numCols);
    if (err == kNoError) {
        SWResetTileMap(*newTileMapPP);
        for (row = 0; row < srcTileMapP->numRows; row++)
            for (col = 0; col < srcTileMapP->numCols; col++)
                (*newTileMapPP)->tileMap[row][col] = srcTileMapP->tileMap[row][col];
    }
    return err;
}

void SWResetTileMapForLayer(SpriteWorldPtr spriteWorldP, short tileLayer)
{
    SWResetTileMap(spriteWorldP->tileLayerArray[tileLayer]);
}

/// Clears the record of changed tiles (the map itself is unchanged).
void SWResetTileMap(TileMapStructPtr tileMapP)
{
    short row, col;

    for (row = 0; row < tileMapP->numRows; row++)
        for (col = 0; col < tileMapP->numCols; col++)
            tileMapP->changedTileMap[row][col] = -1;
}

void SWDisposeTileMap(TileMapStructPtr *tileMapStructPP)
{
    TileMapStructPtr t = *tileMapStructPP;

    if (t == NULL)
        return;
    SWFreeTileMapRows(t);
    free(t);
    *tileMapStructPP = NULL;
}

void SWInstallTileMap(SpriteWorldPtr spriteWorldP, TileMapStructPtr tileMapStructP, short tileLayer)
{
    short i;

    SW_ASSERT(spriteWorldP != NULL);
    SW_ASSERT(tileLayer < kNumTileLayers);
    spriteWorldP->tileLayerArray[tileLayer] = tileMapStructP;
    spriteWorldP->lastActiveTileLayer = 0;
    for (i = 0; i < kNumTileLayers; i++) {
        if (spriteWorldP->tileLayerArray[i] != NULL)
            spriteWorldP->lastActiveTileLayer = i;
    }
}

/// Tile map file: rows and cols (big-endian 16-bit), then rows*cols tile IDs.
SWError SWLoadTileMap(const char *fileName, TileMapStructPtr *tileMapStructPP)
{
    TileMapStructPtr t = NULL;
    SDL_RWops *rw;
    short numRows = 0, numCols = 0, row, col;
    SWError err = kNoError;

    rw = SDL_RWFromFile(fileName, "rb");
    if (rw == NULL) {
        err = -1;
    } else {
        numRows = (short)SDL_ReadBE16(rw);
        numCols = (short)SDL_ReadBE16(rw);
    }
    if (err == kNoError)
        err = SWCreateTileMap(&t, numRows, numCols);
    if (err == kNoError) {
        for (row = 0; row < numRows; row++)
            for (col = 0; col < numCols; col++)
                t->tileMap[row][col] = (short)SDL_ReadBE16(rw);
        *tileMapStructPP = t;
    }
    if (rw != NULL)
        SDL_RWclose(rw);
    return err;
}

SWError SWSaveTileMap(const char *fileName, TileMapStructPtr t)
{
    SDL_RWops *rw;
    short row, col;

    if (t == NULL)
        return kNullTileMapErr;
    rw = SDL_RWFromFile(fileName, "wb");
    if (rw == NULL)
        return -1;
    SDL_WriteBE16(rw, (Uint16)t->numRows);
    SDL_WriteBE16(rw, (Uint16)t->numCols);
    for (row = 0; row < t->numRows; row++)
        for (col = 0; col < t->numCols; col++)
            SDL_WriteBE16(rw, (Uint16)t->tileMap[row][col]);
    SDL_RWclose(rw);
    return kNoError;
}

void SWInstallTileLayerBelowSpriteLayer(short tileLayer, SpriteLayerPtr spriteLayerP)
{
    SW_ASSERT(spriteLayerP != NULL);
    spriteLayerP->tileLayerBelow = tileLayer;
}

void SWInstallTileLayerAboveSpriteLayer(short tileLayer, SpriteLayerPtr spriteLayerP)
{
    SW_ASSERT(spriteLayerP != NULL);
    spriteLayerP->tileLayerAbove = tileLayer;
}

void SWFillTileMapWithValue(TileMapStructPtr tileMapP, short value)
{
    short row, col;

    for (row = 0; row < tileMapP->numRows; row++) {
        for (col = 0; col < tileMapP->numCols; col++) {
            tileMapP->tileMap[row][col] = value;
            tileMapP->changedTileMap[row][col] = value;
        }
    }
    tileMapP->tilesChanged = false;
}

void SWSetTileMapZoom(TileMapStructPtr tileMapP, float zoom)
{
    tileMapP->zoom = zoom;
}

void SWSetTileLayersVisible(SpriteWorldPtr spriteWorldP, short firstLayer, short lastLayer, SWBoolean visible)
{
    short i;

    for (i = firstLayer; i <= lastLayer; i++) {
        if (spriteWorldP->tileLayerArray[i] != NULL)
            spriteWorldP->tileLayerArray[i]->isVisible = visible;
    }
}

void SWSetTileLayerToDrawEveryFrame(SpriteWorldPtr spriteWorldP, short tileLayer, SWBoolean drawEveryFrame)
{
    spriteWorldP->tileLayerArray[tileLayer]->drawEveryFrame = drawEveryFrame;
}

void SWSetTileTranslucency(SpriteWorldPtr spriteWorldP, short tileID, unsigned short translucency)
{
    spriteWorldP->tileColorArray[tileID].hasColor = true;
    spriteWorldP->tileColorArray[tileID].alpha = translucency;
    if (spriteWorldP->backgroundTileSpriteP != NULL)
        SWUpdateTileOnScreen(spriteWorldP, tileID);
}

void SWSetTileTranslucencyForRange(SpriteWorldPtr spriteWorldP, short firstTileID, short lastTileID, unsigned short translucency)
{
    short i;

    for (i = firstTileID; i <= lastTileID; i++) {
        spriteWorldP->tileColorArray[i].hasColor = true;
        spriteWorldP->tileColorArray[i].alpha = translucency;
    }
    if (spriteWorldP->backgroundTileSpriteP != NULL)
        SWUpdateTilesOnScreenFast(spriteWorldP, firstTileID, lastTileID);
}

void SWSetTileColor(SpriteWorldPtr spriteWorldP, short tileID, unsigned short red, unsigned short green, unsigned short blue)
{
    spriteWorldP->tileColorArray[tileID].hasColor = true;
    spriteWorldP->tileColorArray[tileID].red = red;
    spriteWorldP->tileColorArray[tileID].green = green;
    spriteWorldP->tileColorArray[tileID].blue = blue;
    if (spriteWorldP->backgroundTileSpriteP != NULL)
        SWUpdateTileOnScreen(spriteWorldP, tileID);
}

void SWSetTileColorForRange(SpriteWorldPtr spriteWorldP, short firstTileID, short lastTileID,
                            unsigned short red, unsigned short green, unsigned short blue, unsigned short alpha)
{
    short i;

    for (i = firstTileID; i <= lastTileID; i++) {
        TileColorRec *c = &spriteWorldP->tileColorArray[i];
        c->hasColor = true;
        c->red = red;
        c->green = green;
        c->blue = blue;
        c->alpha = alpha;
    }
    if (spriteWorldP->backgroundTileSpriteP != NULL)
        SWUpdateTilesOnScreenFast(spriteWorldP, firstTileID, lastTileID);
}

/// Called after the given tile layer has been drawn each frame.
void SWSetTileLayerCallback(SpriteWorldPtr spriteWorldP, short tileLayer, TileLayerCallBackPtr callBack)
{
    SW_ASSERT(spriteWorldP != NULL);
    spriteWorldP->tileLayerCallbacks[tileLayer] = callBack;
}

/// Color for all tiles; with useStdColor set, per-tile colors are ignored.
void SWSetStdTileColor(SpriteWorldPtr spriteWorldP, unsigned short red, unsigned short green,
                       unsigned short blue, unsigned short alpha, SWBoolean useStdColor)
{
    SW_ASSERT(spriteWorldP != NULL);
    spriteWorldP->stdTileRed = red;
    spriteWorldP->stdTileGreen = green;
    spriteWorldP->stdTileBlue = blue;
    spriteWorldP->stdTileAlpha = alpha;
    spriteWorldP->useStdTileColor = useStdColor;
}

/// Makes tiles firstTileID..lastTileID copies of the tiles starting at sourceTileID.
void SWCloneTiles(SpriteWorldPtr spriteWorldP, short firstTileID, short lastTileID, short sourceTileID)
{
    short tileID, src = sourceTileID;

    for (tileID = firstTileID; tileID <= lastTileID; tileID++, src++) {
        SW_ASSERT(tileID < spriteWorldP->maxNumTiles);
        if (spriteWorldP->tileFrameArray[tileID] != NULL)
            SWDisposeTile(spriteWorldP, tileID);
        spriteWorldP->tileFrameArray[tileID] = spriteWorldP->tileFrameArray[src];
        if (spriteWorldP->tileFrameArray[tileID] != NULL)
            spriteWorldP->tileFrameArray[tileID]->useCount++;
        spriteWorldP->tileSourceIndex[tileID] = src;
        spriteWorldP->tileColorArray[tileID].hasColor = spriteWorldP->tileColorArray[src].hasColor;
        spriteWorldP->tileColorArray[tileID].red = spriteWorldP->tileColorArray[src].red;
        spriteWorldP->tileColorArray[tileID].green = spriteWorldP->tileColorArray[src].green;
        spriteWorldP->tileColorArray[tileID].blue = spriteWorldP->tileColorArray[src].blue;
    }
}

/*  Loads numTiles tiles from a sheet, starting at startTileID. Tile images are
    tileImageWidth x tileImageHeight with the given borders between them. */
SWError SWLoadTilesFromSingleFileXY(SpriteWorldPtr spriteWorldP, const char *fileName, int borderWidth,
                    int borderHeight, SWBoolean hasOutsideBorder, int numTiles, short startTileID)
{
    FramePtr frameP = NULL;
    SDL_Surface *surface;
    SWRect frameRect;
    short tileID = startTileID;
    short tileW = spriteWorldP->tileImageWidth, tileH = spriteWorldP->tileImageHeight;
    int startH = 0, startV = 0, i;
    SWError err = kNoError;

    SW_ASSERT(numTiles > 0);
    surface = BKLoadSurface(fileName);
    if (surface == NULL)
        err = kSDLCreateSurfaceFromFile;
    if (err == kNoError && hasOutsideBorder) {
        startH = borderWidth / 2;
        startV = borderHeight / 2;
    }
    if (err == kNoError) {
        frameRect.left = (short)startH;
        frameRect.top = (short)startV;
        frameRect.right = frameRect.left + tileW;
        frameRect.bottom = frameRect.top + tileH;
        for (i = 0; i < numTiles; i++) {
            if (tileID >= spriteWorldP->maxNumTiles)
                err = kOutOfRangeErr;
            if (err == kNoError)
                err = SWCreateFrameFromSurfaceAndRect(&frameP, surface, &frameRect);
            if (err == kNoError) {
                if (spriteWorldP->tileFrameArray[tileID] != NULL)
                    SWDisposeTile(spriteWorldP, tileID);
                spriteWorldP->tileFrameArray[tileID] = frameP;
                frameP->useCount++;
                if (i < numTiles - 1) {
                    frameRect.left += (short)borderWidth + tileW;
                    frameRect.right = frameRect.left + tileW;
                    if (frameRect.right > surface->w) {
                        frameRect.left = (short)startH;
                        frameRect.right = frameRect.left + tileW;
                        frameRect.top += (short)borderHeight + tileH;
                        frameRect.bottom = frameRect.top + tileH;
                        if (frameRect.bottom > surface->h)
                            err = kOutOfRangeErr;
                    }
                }
            }
            if (err != kNoError) {
                if (frameP != NULL)
                    SWDisposeTile(spriteWorldP, tileID);
                break;
            }
            tileID++;
        }
    }
    // The surface stays alive: the tile frames share it (as in the original).
    SWSetStickyIfError(err);
    return err;
}

SWError SWLoadTilesFromFileSequence(SpriteWorldPtr spriteWorldP, const char *fileName, int start,
                    int numTiles, short startTileID)
{
    FramePtr frameP;
    char path[1024];
    short tileID = startTileID;
    int i;
    SWError err = kNoError;

    SW_ASSERT(spriteWorldP != NULL && fileName != NULL && numTiles > 0);
    for (i = 0; i < numTiles; i++, tileID++) {
        snprintf(path, sizeof(path), fileName, start + i);
        if (tileID >= spriteWorldP->maxNumTiles)
            err = kOutOfRangeErr;
        if (err == kNoError)
            err = SWCreateFrameFromFile(&frameP, path, false);
        if (err != kNoError) {
            SWDisposeTile(spriteWorldP, tileID);
            break;
        }
        if (spriteWorldP->tileFrameArray[tileID] != NULL)
            SWDisposeTile(spriteWorldP, tileID);
        spriteWorldP->tileFrameArray[tileID] = frameP;
        frameP->useCount++;
    }
    SWSetStickyIfError(err);
    return err;
}

void SWDisposeTile(SpriteWorldPtr spriteWorldP, short tileID)
{
    FramePtr frameP;
    short i;
    int surfaceShared = false;

    SW_ASSERT(spriteWorldP != NULL && spriteWorldP->tilingIsInitialized);
    frameP = spriteWorldP->tileFrameArray[tileID];
    if (frameP == NULL)
        return;
    for (i = spriteWorldP->maxNumTiles - 1; i >= 0; i--) {
        if (i != tileID && spriteWorldP->tileFrameArray[i] != NULL &&
            spriteWorldP->tileFrameArray[i]->frameSurfaceP == frameP->frameSurfaceP)
            surfaceShared = true;
    }
    frameP->sharesSurface = surfaceShared;
    SWDisposeFrame(&spriteWorldP->tileFrameArray[tileID]);
    spriteWorldP->tileFrameArray[tileID] = NULL;
}

void SWSetTilingOn(SpriteWorldPtr spriteWorldP, SWBoolean tilingIsOn)
{
    SW_ASSERT(spriteWorldP != NULL);
    spriteWorldP->tilingIsOn = tilingIsOn;
}

/// Called at the start of every frame (used to animate tiles with SWChangeTileImage).
void SWSetTileChangeProc(SpriteWorldPtr spriteWorldP, TileChangeProcPtr tileChangeProc)
{
    SW_ASSERT(spriteWorldP != NULL);
    spriteWorldP->tileChangeProc = tileChangeProc;
}

/*  Puts tileID at (tileRow, tileCol) of a layer. With a cached background, the tile is
    also recorded as changed, as are the tiles above it in higher layers, so they are
    drawn over the background in the right order. */
void SWDrawTile(SpriteWorldPtr spriteWorldP, short dstTileLayer, short tileRow, short tileCol, short tileID)
{
    TileMapStructPtr t;
    short layer;

    if (spriteWorldP->tileLayerArray[dstTileLayer] == NULL)
        return;
    SW_ASSERT(spriteWorldP != NULL && spriteWorldP->tilingIsInitialized);
    SW_ASSERT(dstTileLayer >= 0 && dstTileLayer <= spriteWorldP->lastActiveTileLayer);
    if (tileID >= spriteWorldP->maxNumTiles) {
        printf("TileID out of range! tileID is %d while maxNumTiles is %d \n", tileID, spriteWorldP->maxNumTiles);
        SW_ASSERT(tileID < spriteWorldP->maxNumTiles);
    }
    t = spriteWorldP->tileLayerArray[dstTileLayer];
    SW_ASSERT(tileRow < t->numRows && tileCol < t->numCols);
    t->tileMap[tileRow][tileCol] = tileID;

    if (spriteWorldP->backgroundTileSpriteP != NULL) {
        t->changedTileMap[tileRow][tileCol] = tileID;
        t->tilesChanged = true;
        for (layer = dstTileLayer + 1; layer <= spriteWorldP->lastActiveTileLayer; layer++) {
            TileMapStructPtr upper = spriteWorldP->tileLayerArray[layer];
            if (upper != NULL && tileRow < upper->numRows && tileCol < upper->numCols &&
                upper->tileMap[tileRow][tileCol] != -1) {
                upper->changedTileMap[tileRow][tileCol] = upper->tileMap[tileRow][tileCol];
                upper->tilesChanged = true;
            }
        }
    }
}

/*  Draws the visible world (with all visible layers) and keeps it as the background for
    following frames. The game hides the static layers afterwards. */
void SWCreateBackgroundTextureFromTiles(SpriteWorldPtr spriteWorldP)
{
    SWRect area;

    BK_CHECK_GL();
    SWDisposeBackgroundTileTexture(spriteWorldP);
    SWUpdateSpriteWorld(spriteWorldP, false);
    SW_SET_RECT(area, 0, 0, spriteWorldP->designWidth, spriteWorldP->designHeight);
    if (SWCreateSpriteFromWorkArea(spriteWorldP, &spriteWorldP->backgroundTileSpriteP, NULL, &area) == kNoError) {
        SWSetSpriteScaledWidthFactor(spriteWorldP->backgroundTileSpriteP, 1.0f);
        SWSetSpriteScaledHeightFactor(spriteWorldP->backgroundTileSpriteP, 1.0f);
    }
}

void SWDisposeBackgroundTileTexture(SpriteWorldPtr spriteWorldP)
{
    if (spriteWorldP->backgroundTileSpriteP != NULL) {
        SWDisposeSprite(&spriteWorldP->backgroundTileSpriteP);
        spriteWorldP->backgroundTileSpriteP = NULL;
    }
}

void SWDrawTileLayersInRect(SpriteWorldPtr spriteWorldP, SWRect *updateRectP)
{
    short layer;

    if (spriteWorldP->backgroundTileSpriteP != NULL)
        return;
    for (layer = 0; layer <= spriteWorldP->lastActiveTileLayer; layer++)
        SWDrawTileLayerInRect(spriteWorldP, layer, updateRectP, true, false);
}

/*  Draws the tiles of one layer that fall in updateRectP. With a cached background and
    useChangedMap, only changed tiles (and tiles marked for redraw over sprites) are
    drawn. Returns whether anything was drawn. */
SWBoolean SWDrawTileLayerInRect(SpriteWorldPtr w, short tileLayer, SWRect *updateRectP,
                                SWBoolean useChangedMap, SWBoolean forceDraw)
{
    TileMapStructPtr t;
    short **map;
    short firstRow, lastRow, firstCol, lastCol, row, col, tileID;
    short tw, th;
    SWBoolean scaleTiles, colorChanged = false;
    float scaleH = 1.0f, scaleV = 1.0f;
    SWBoolean anyMarked;

    SW_ASSERT(w != NULL && w->tilingIsInitialized);
    t = w->tileLayerArray[tileLayer];
    if (t == NULL)
        return false;
    anyMarked = false;
    for (row = 0; row < t->numRows && !anyMarked; row++)
        for (col = 0; col < t->numCols; col++)
            if (t->tileChangedFlags[row][col]) { anyMarked = true; break; }
    if (!t->isVisible && !t->tilesChanged && !forceDraw && !anyMarked)
        return false;

    tw = w->tileWidth;
    th = w->tileHeight;
    scaleTiles = (w->tileWidth != w->tileImageWidth);
    if (scaleTiles) {
        scaleH = (float)w->tileWidth / (float)w->tileImageWidth;
        scaleV = (float)w->tileHeight / (float)w->tileImageHeight;
    }
    if (w->backgroundTileSpriteP == NULL || !useChangedMap || t->drawEveryFrame) {
        map = t->tileMap;
    } else {
        map = t->changedTileMap;
        if (!t->tilesChanged && !anyMarked)
            return false;
    }

    firstRow = updateRectP->top / th;
    lastRow = (short)((updateRectP->bottom - 1) / th);
    firstCol = updateRectP->left / tw;
    lastCol = (short)((updateRectP->right - 1) / tw);
    if (firstRow < 0) firstRow = 0;
    if (firstCol < 0) firstCol = 0;
    if (lastRow >= t->numRows) lastRow = t->numRows - 1;
    if (lastCol >= t->numCols) lastCol = t->numCols - 1;

    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glColor4us(w->stdTileRed, w->stdTileGreen, w->stdTileBlue, w->stdTileAlpha);

    for (row = firstRow; row <= lastRow; row++) {
        for (col = firstCol; col <= lastCol; col++) {
            FramePtr frameP;
            BKGLSurface *gls;

            if (!useChangedMap || !t->tileChangedFlags[row][col])
                tileID = map[row][col];
            else
                tileID = t->tileMap[row][col];
            if (tileID >= w->maxNumTiles) {
                printf("\n\nERROR: dstIndex = %d but maxNumTiles = %d \n", tileID, w->maxNumTiles);
                continue;
            }
            if (tileID < 0)
                continue;
            frameP = w->tileFrameArray[w->curTileImage[tileID]];
            if (frameP == NULL || frameP->glSurfaceP == NULL) {
                printf("tileFrameP is NULL! tileID is %d and curTileImage for it is %d \n", tileID, w->curTileImage[tileID]);
                continue;
            }
            gls = frameP->glSurfaceP;

            if (!w->useStdTileColor) {
                TileColorRec *c = &w->tileColorArray[tileID];
                if (!c->hasColor) {
                    if (colorChanged) {
                        glColor4us(w->stdTileRed, w->stdTileGreen, w->stdTileBlue, w->stdTileAlpha);
                        colorChanged = false;
                    }
                } else {
                    if (gIsDrawingTilesAboveSprite && c->alpha != 0xFFFF)
                        continue;
                    glColor4us(c->red, c->green, c->blue, c->alpha);
                    colorChanged = true;
                }
            }

            glLoadIdentity();
            glScalef(w->designScaleH, w->designScaleV, 1.0f);
            glTranslatef((float)(col * tw - w->tileOffsetH), (float)(row * th - w->tileOffsetV), 0);
            if (scaleTiles)
                glScalef(scaleH, scaleV, 1.0f);
            if (DoesSurfaceNeedBlend(gls)) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            } else {
                glDisable(GL_BLEND);
            }
            BKBlitGLSurface(gls);
            gNumTilesDrawnThisFrame++;
        }
    }
    glDisable(GL_BLEND);
    glPopMatrix();
    return true;
}

void SWResetAllChangedTilesMaps(SpriteWorldPtr spriteWorldP)
{
    short layer;

    if (!spriteWorldP->tilingIsOn || spriteWorldP->tileLayerArray == NULL)
        return;
    for (layer = 0; layer <= spriteWorldP->lastActiveTileLayer; layer++)
        SWResetChangedTilesMap(spriteWorldP->tileLayerArray[layer]);
}

static void SWResetChangedTilesMap(TileMapStructPtr t)
{
    short row;

    if (t == NULL)
        return;
    for (row = 0; row < t->numRows; row++)
        memset(t->tileChangedFlags[row], 0, t->numCols);
}

/*  With a cached background, marks the tiles of higher layers that cover updateRectP to
    be drawn again this frame, so they appear over the sprite just drawn. */
void SWDrawTilesAboveSprite(SpriteWorldPtr spriteWorldP, SWRect *updateRectP, short startLayer)
{
    short layer;

    if (spriteWorldP->backgroundTileSpriteP == NULL)
        return;
    gIsDrawingTilesAboveSprite = true;
    for (layer = startLayer; layer <= spriteWorldP->lastActiveTileLayer; layer++) {
        TileMapStructPtr t = spriteWorldP->tileLayerArray[layer];
        if (t != NULL && !t->drawEveryFrame)
            SWMarkTilesToBeRedrawn(spriteWorldP, layer, updateRectP);
    }
    gIsDrawingTilesAboveSprite = false;
}

static void SWMarkTilesToBeRedrawn(SpriteWorldPtr w, short tileLayer, SWRect *r)
{
    TileMapStructPtr t = w->tileLayerArray[tileLayer];
    short row, col, firstRow, lastRow, firstCol, lastCol;

    if (t == NULL || r->right <= r->left || r->bottom <= r->top)
        return;
    firstRow = r->top / w->tileHeight;
    lastRow = (short)((r->bottom - 1) / w->tileHeight);
    firstCol = r->left / w->tileWidth;
    lastCol = (short)((r->right - 1) / w->tileWidth);
    if (firstRow < 0) firstRow = 0;
    if (firstCol < 0) firstCol = 0;
    if (lastRow >= t->numRows) lastRow = t->numRows - 1;
    if (lastCol >= t->numCols) lastCol = t->numCols - 1;
    for (row = firstRow; row <= lastRow; row++)
        for (col = firstCol; col <= lastCol; col++)
            t->tileChangedFlags[row][col] = 1;
}

/// Shows frame currentFrame for every tile with ID tileID (tile animation).
void SWChangeTileImage(SpriteWorldPtr spriteWorldP, short tileID, short currentFrame)
{
    SW_ASSERT(spriteWorldP != NULL && spriteWorldP->tilingIsInitialized);
    SW_ASSERT(tileID < spriteWorldP->maxNumTiles);
    SW_ASSERT(currentFrame < spriteWorldP->maxNumTiles);
    spriteWorldP->curTileImage[tileID] = currentFrame;
    if (spriteWorldP->backgroundTileSpriteP != NULL)
        SWUpdateTileOnScreen(spriteWorldP, tileID);
}

/// With a cached background, redraws every visible occurrence of tileID over it.
void SWUpdateTileOnScreen(SpriteWorldPtr spriteWorldP, short tileID)
{
    SWUpdateTilesOnScreenFast(spriteWorldP, tileID, tileID);
}

void SWUpdateTilesOnScreenFast(SpriteWorldPtr w, short firstTileID, short lastTileID)
{
    short layer, row, col, firstRow, lastRow, firstCol, lastCol;

    SW_ASSERT(w != NULL && w->tilingIsInitialized);
    if (w->backgroundTileSpriteP == NULL)
        return;
    firstRow = w->visScrollRect.top / w->tileHeight;
    lastRow = (short)((w->visScrollRect.bottom - 1) / w->tileHeight);
    firstCol = w->visScrollRect.left / w->tileWidth;
    lastCol = (short)((w->visScrollRect.right - 1) / w->tileWidth);
    for (layer = 0; layer <= w->lastActiveTileLayer; layer++) {
        TileMapStructPtr t = w->tileLayerArray[layer];
        if (t == NULL)
            continue;
        for (row = SW_MAX(firstRow, 0); row <= lastRow && row < t->numRows; row++) {
            for (col = SW_MAX(firstCol, 0); col <= lastCol && col < t->numCols; col++) {
                short tileID = t->tileMap[row][col];
                if (tileID >= firstTileID && tileID <= lastTileID)
                    SWDrawTile(w, layer, row, col, tileID);
            }
        }
    }
}

void SWResetCurrentTileImages(SpriteWorldPtr spriteWorldP)
{
    short i;

    SW_ASSERT(spriteWorldP != NULL && spriteWorldP->tilingIsInitialized);
    for (i = 0; i < spriteWorldP->maxNumTiles; i++)
        spriteWorldP->curTileImage[i] = i;
}

short SWReturnTileUnderPixel(SpriteWorldPtr spriteWorldP, short tileLayer, short pixelCol, short pixelRow)
{
    TileMapStructPtr t;
    short row, col;

    SW_ASSERT(spriteWorldP != NULL && spriteWorldP->tilingIsInitialized);
    SW_ASSERT(tileLayer < kNumTileLayers);
    t = spriteWorldP->tileLayerArray[tileLayer];
    if (t == NULL)
        return -1;
    row = pixelRow / spriteWorldP->tileHeight;
    col = pixelCol / spriteWorldP->tileWidth;
    if (row < 0 || row >= t->numRows || col < 0 || col >= t->numCols)
        return -1;
    return t->tileMap[row][col];
}

/*  Checks whether the sprite's rect (optionally inset) touches any tile in the ID range
    firstTileID..lastTileID in layers startTileLayer..endTileLayer.

    For a directional check (top/right/bottom/left) only the row or column the sprite
    moved into since last frame is tested. With fixPosition, the sprite is pushed back
    out to the edge of the tile it hit. */
SWBoolean SWCheckSpriteWithTiles(SpriteWorldPtr w, SpritePtr srcSpriteP, SWTileSearchType searchType,
                    SWRect *insetRectP, short startTileLayer, short endTileLayer, short firstTileID,
                    short lastTileID, SWBoolean fixPosition)
{
    SWRect oldRect, destRect;
    short startRow = 0, endRow = 0, startCol = 0, endCol = 0, rowDelta, colDelta;
    short row = -1, col = -1, layer;
    short th, tw;
    SWBoolean found = false;
    TileMapStructPtr t;

    SW_ASSERT(w != NULL && w->tilingIsInitialized);
    SW_ASSERT(startTileLayer >= 0 && endTileLayer < kNumTileLayers);
    SW_ASSERT(srcSpriteP != NULL);
    th = w->tileHeight;
    tw = w->tileWidth;
    oldRect = srcSpriteP->oldFrameRect;
    destRect = srcSpriteP->destFrameRect;
    if (insetRectP != NULL) {
        oldRect.left += insetRectP->left;
        oldRect.top += insetRectP->top;
        oldRect.right -= insetRectP->right;
        oldRect.bottom -= insetRectP->bottom;
        destRect.left += insetRectP->left;
        destRect.top += insetRectP->top;
        destRect.right -= insetRectP->right;
        destRect.bottom -= insetRectP->bottom;
    }

        // Negative coordinates divide toward zero; shift them a tile so they round down.
    if (oldRect.top < 0) oldRect.top -= th;
    if (oldRect.bottom < 1) oldRect.bottom -= th;
    if (oldRect.left < 1) oldRect.left -= tw;
    if (oldRect.right < 1) oldRect.right -= tw;
    if (destRect.left < 0) destRect.left -= tw;
    if (destRect.right < 1) destRect.right -= tw;
    if (destRect.top < 0) destRect.top -= th;
    if (destRect.bottom < 1) destRect.bottom -= th;

    switch (searchType) {
        case kSW_CheckTop:
            startRow = oldRect.top / th;
            endRow = destRect.top / th;
            startCol = destRect.left / tw;
            endCol = (short)((destRect.right - 1) / tw);
            if (fixPosition)
                startRow--;
            if (startRow < endRow)
                return false;
            break;
        case kSW_CheckBottom:
            startRow = (short)((oldRect.bottom - 1) / th);
            endRow = (short)((destRect.bottom - 1) / th);
            startCol = destRect.left / tw;
            endCol = (short)((destRect.right - 1) / tw);
            if (fixPosition)
                startRow++;
            if (endRow < startRow)
                return false;
            break;
        case kSW_CheckRight:
            startCol = (short)((oldRect.right - 1) / tw);
            endCol = (short)((destRect.right - 1) / tw);
            startRow = destRect.top / th;
            endRow = (short)((destRect.bottom - 1) / th);
            if (fixPosition)
                startCol++;
            if (endCol < startCol)
                return false;
            break;
        case kSW_CheckLeft:
            startCol = oldRect.left / tw;
            endCol = destRect.left / tw;
            startRow = destRect.top / th;
            endRow = (short)((destRect.bottom - 1) / th);
            if (fixPosition)
                startCol--;
            if (startCol < endCol)
                return false;
            break;
        default:
            startRow = destRect.top / th;
            endRow = (short)((destRect.bottom - 1) / th);
            startCol = destRect.left / tw;
            endCol = (short)((destRect.right - 1) / tw);
            break;
    }

    rowDelta = (endRow < startRow) ? -1 : 1;
    colDelta = (endCol < startCol) ? -1 : 1;

        // Clamp to the first installed layer's map size.
    layer = startTileLayer;
    while (layer <= endTileLayer && w->tileLayerArray[layer] == NULL)
        layer++;
    if (layer > endTileLayer)
        return false;
    t = w->tileLayerArray[layer];
    if (rowDelta < 1) {
        if (startRow < 0) return false;
        if (startRow >= t->numRows) startRow = t->numRows - 1;
        if (endRow < 0) endRow = 0;
        else if (endRow >= t->numRows) return false;
    } else {
        if (endRow < 0) return false;
        if (endRow >= t->numRows) endRow = t->numRows - 1;
        if (startRow < 0) startRow = 0;
        else if (startRow >= t->numRows) return false;
    }
    if (colDelta < 1) {
        if (startCol < 0) return false;
        if (startCol >= t->numCols) startCol = t->numCols - 1;
        if (endCol < 0) endCol = 0;
        else if (endCol >= t->numCols) return false;
    } else {
        if (endCol < 0) return false;
        if (endCol >= t->numCols) endCol = t->numCols - 1;
        if (startCol < 0) startCol = 0;
        else if (startCol >= t->numCols) return false;
    }

    for (layer = startTileLayer; layer <= endTileLayer && !found; layer++) {
        t = w->tileLayerArray[layer];
        if (t == NULL)
            continue;
        if (searchType == kSW_CheckTop || searchType == kSW_CheckBottom || searchType == kSW_CheckAll) {
                // rows outermost
            for (row = startRow; ; row += rowDelta) {
                for (col = startCol; ; col += colDelta) {
                    short id = t->tileMap[row][col];
                    if (id >= firstTileID && id <= lastTileID) { found = true; goto done; }
                    if (col == endCol) break;
                }
                if (row == endRow) break;
            }
        } else {
                // columns outermost
            for (col = startCol; ; col += colDelta) {
                for (row = startRow; ; row += rowDelta) {
                    short id = t->tileMap[row][col];
                    if (id >= firstTileID && id <= lastTileID) { found = true; goto done; }
                    if (row == endRow) break;
                }
                if (col == endCol) break;
            }
        }
    }

done:
    if (found && fixPosition) {
        if (searchType == kSW_CheckTop)
            SWOffsetSprite(srcSpriteP, 0.0f, (float)(short)(th * (row + 1) - destRect.top));
        else if (searchType == kSW_CheckBottom)
            SWOffsetSprite(srcSpriteP, 0.0f, (float)-(short)(destRect.bottom - th * row));
        else if (searchType == kSW_CheckRight)
            SWOffsetSprite(srcSpriteP, (float)-(short)(destRect.right - tw * col), 0.0f);
        else if (searchType == kSW_CheckLeft)
            SWOffsetSprite(srcSpriteP, (float)(short)(tw * (col + 1) - destRect.left), 0.0f);
    }
    return found;
}

void SWResetVBOForAllTileLayers(SpriteWorldPtr spriteWorldP)
{
    SW_UNUSED(spriteWorldP);    // tiles are not drawn from vertex buffers in this port
}

void SWSetVBOForTiling(SpriteWorldPtr spriteWorldP, SWBoolean useVBO)
{
    SW_UNUSED(spriteWorldP);
    SW_UNUSED(useVBO);
}

void SWSetTileMapVBOMethod(TileMapStructPtr tileMapP, SWBoolean method)
{
    SW_UNUSED(tileMapP);
    SW_UNUSED(method);
}
