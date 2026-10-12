// GameEditorStuff.c — tile classification and automatic "fringe" (edge) tiles.
//
// The original file also held the level editor UI; only the parts used by the game are kept.
// Tile IDs are first normalised with ConvertTileID() (which maps colour variations back to the
// base tile). gGEFringeArray maps a tile to the first tile of its fringe set (0 = no fringe,
// 1 = "is itself a fringe tile"), and gGETileFamily maps a tile to the first tile of its 2x2
// pattern family (-1 = none).

#include "MMGame.h"
#include "MMProtos.h"

#define kNumEditorTiles 0x5a2

Boolean gShowedMustClickOnSpriteAlert = false;
Boolean gDrawFringeInLayer6 = true;
Boolean gDrawBackgroundSprites = true;
Boolean gForceTilePattern = true;
short   gGEEmptyTileID = 0;

short gGEFringeArray[kNumEditorTiles];
short gGETileFamily[kNumEditorTiles];
short gHighestFringeLayer, gLowestFringeLayer, gMiddleFringeLayer;

static void SetRange(short *array, short first, short end, short value)
{
    short i;
    for (i = first; i < end; i++)
        array[i] = value;
}

static short ClampTileIndex(short tileID)
{
    SDL_assert(tileID < kNumEditorTiles);
    return tileID < 0 ? 0 : tileID;
}

void SetUpFringeArray(void)
{
    // Wall tile families: 4 wall tiles followed by their 10 fringe tiles.
    static const short wallFamilies[] = {
        1, 0xf, 0x1d, 0x2b, 0x39, 0x47, 0x55, 99, 0x71, 0x7f, 0x8d, 0x9b, 0xa9, 0xb7, 0xc5,
        0x1af, 0x1bd
    };
    size_t i;

    SetRange(gGEFringeArray, 0, kNumEditorTiles, 0);
    for (i = 0; i < SDL_arraysize(wallFamilies); i++)
        SetRange(gGEFringeArray, wallFamilies[i], wallFamilies[i] + 4, wallFamilies[i] + 4);

    // Eye-candy wall tiles use the fringe of the first few wall families.
    SetRange(gGEFringeArray, 0xd3, 0xf7, 5);
    SetRange(gGEFringeArray, 0xf7, 0x11b, 0x13);
    SetRange(gGEFringeArray, 0x11b, 0x13f, 0x21);
    SetRange(gGEFringeArray, 0x13f, 0x15b, 0x2f);
    SetRange(gGEFringeArray, 0x15b, 0x18b, 0x3d);

    for (i = 0; i < SDL_arraysize(wallFamilies); i++)
        SetRange(gGEFringeArray, wallFamilies[i] + 4, wallFamilies[i] + 14, 1);

    // Lava
    SetRange(gGEFringeArray, 0x309, 0x30d, 0x319);
    SetRange(gGEFringeArray, 0x319, 0x331, 1);

    // Floors (3 floor tiles followed by 5 fringe tiles)
    SetRange(gGEFringeArray, 0x241, 0x244, 0x241);
    SetRange(gGEFringeArray, 0x249, 0x24c, 0x249);
    SetRange(gGEFringeArray, 0x244, 0x249, 1);
    SetRange(gGEFringeArray, 0x24c, 0x251, 1);
    SetRange(gGEFringeArray, 0x2c1, 0x2c4, 0x2c1);
    SetRange(gGEFringeArray, 0x2c4, 0x2c9, 1);
    SetRange(gGEFringeArray, 0x2e9, 0x2ec, 0x2e9);
    SetRange(gGEFringeArray, 0x2ec, 0x2f1, 1);
}

void SetUpTileFamilyArray(void)
{
    static const short families[] = {
        1, 0xf, 0x1d, 0x2b, 0x39, 0x47, 0x55, 99, 0x71, 0x7f, 0x8d, 0x9b, 0xa9, 0xb7, 0xc5, 0x309
    };
    size_t i;

    SetRange(gGETileFamily, 0, kNumEditorTiles, -1);
    for (i = 0; i < SDL_arraysize(families); i++)
        SetRange(gGETileFamily, families[i], families[i] + 4, families[i]);
}

Boolean isRightBrick(short tileID)
{
    short converted;
    if (GetTileFamily(tileID) + 1 != tileID && GetTileFamily(tileID) + 2 != tileID)
        return false;
    converted = ConvertTileID(tileID);
    return converted > 0 && converted < 0xd3;
}

Boolean isLeftBrick(short tileID)
{
    short converted;
    if (GetTileFamily(tileID) != tileID && GetTileFamily(tileID) + 3 != tileID)
        return false;
    converted = ConvertTileID(tileID);
    return converted > 0 && converted < 0xd3;
}

Boolean isNormalBrick(short tileID)
{
    short converted = ConvertTileID(tileID);
    return converted > 3 && converted < 0x8d;
}

Boolean IsWallTile(short tileID)
{
    short converted = ConvertTileID(tileID);
    return converted >= 1 && converted <= 0x1a2;
}

Boolean isTileFringe(short tileID)
{
    short converted = ClampTileIndex(ConvertTileID(tileID));
    return gGEFringeArray[converted] + 4 <= converted;
}

short GetTileFringe(short tileID)
{
    short converted = ConvertTileID(tileID);
    return (short)(gGEFringeArray[ClampTileIndex(converted)] + (tileID - converted));
}

short GetTileFamily(short tileID)
{
    short converted = ConvertTileID(tileID);
    return (short)(gGETileFamily[ClampTileIndex(converted)] + (tileID - converted));
}

short GetBaseTileFamily(short tileID)
{
    return gGETileFamily[ClampTileIndex(ConvertTileID(tileID))];
}

// ---------------------------------------------------------------------------
// Drawing

void MMDrawTileAndFringeCallback(SpriteWorldPtr spriteWorldP, short tileLayer, short row, short col,
                                 short tileID)
{
    TileMapStructPtr tileMapP = spriteWorldP->tileLayerArray[tileLayer];
    short drawTileID = tileID, leftTile, rightTile;

    if (gForceTilePattern && GetTileFamily(tileID) != -1)
        drawTileID = GetCorrectTileForRowAndCol(GetTileFamily(tileID), row, col);

    SDL_assert(drawTileID < spriteWorldP->maxNumTiles);
    if (drawTileID >= 0 && drawTileID < 1)
        drawTileID = (tileLayer == 0) ? gGEEmptyTileID : -1;

    SWDrawTile(spriteWorldP, tileLayer, row, col, drawTileID);

    leftTile = GetEditorTileID(tileMapP, row, col - 1);
    rightTile = GetEditorTileID(tileMapP, row, col + 1);
    if (GEIsTileFloorTile(leftTile))
        DrawFloorTile(spriteWorldP, tileLayer, row, col - 1, leftTile);
    if (GEIsTileFloorTile(tileID))
        DrawFloorTile(spriteWorldP, tileLayer, row, col, tileID);
    if (GEIsTileFloorTile(rightTile))
        DrawFloorTile(spriteWorldP, tileLayer, row, col + 1, rightTile);

    MMDrawFringe(spriteWorldP, tileLayer, row, col);
}

void MMDrawFringe(SpriteWorldPtr spriteWorldP, short tileLayer, short row, short col)
{
    TileMapStructPtr tileMapP = spriteWorldP->tileLayerArray[tileLayer];
    short layer, r, c, emptyTile, tileID, rawTileID, fringe;

    if (tileLayer == 4) {
        gLowestFringeLayer = 1;
        gMiddleFringeLayer = 2;
        gHighestFringeLayer = 3;
    }
    else if (tileLayer == 6) {
        gLowestFringeLayer = 6;
        gMiddleFringeLayer = 6;
        gHighestFringeLayer = 6;
        if (!gDrawFringeInLayer6)
            return;
    }
    else
        return;

    // Clear the fringe this tile may have drawn before.
    for (layer = gLowestFringeLayer; layer <= gHighestFringeLayer; layer++) {
        emptyTile = (layer == 0) ? gGEEmptyTileID : -1;
        DrawTileIfReplacingFringe(spriteWorldP, layer, row, col, emptyTile);
        DrawTileIfReplacingFringe(spriteWorldP, layer, row, col + 1, emptyTile);
        DrawTileIfReplacingFringe(spriteWorldP, layer, row - 1, col, emptyTile);
        DrawTileIfReplacingFringe(spriteWorldP, layer, row - 1, col + 1, emptyTile);
    }

    // Redraw the fringe of this tile and its 8 neighbours.
    for (r = row - 1; r <= row + 1; r++) {
        if (r < 0 || r >= tileMapP->numRows)
            continue;
        for (c = col - 1; c <= col + 1; c++) {
            if (c < 0 || c >= tileMapP->numCols)
                continue;
            tileID = GetEditorTileID(tileMapP, r, c);
            rawTileID = GetEditorTileIDNoConvert(tileMapP, r, c);
            fringe = GetTileFringe(tileID);
            if (fringe == 0 || fringe == 1)
                continue;
            if ((tileID >= 1 && tileID <= 0x1a2) || (tileID >= 0x1af && tileID <= 0x1e6))
                DrawFringeForWallTile(spriteWorldP, tileLayer, r, c, rawTileID);
            else if (tileID >= 0x309 && rawTileID <= 0x30c)
                DrawFringeForLavaTile(spriteWorldP, tileLayer, r, c, rawTileID);
            else if ((tileID >= 0x241 && tileID <= 0x24b) || (tileID >= 0x2c1 && tileID <= 0x2c3) ||
                     (tileID >= 0x2e9 && tileID <= 0x2eb))
                DrawFringeForFloorTile(spriteWorldP, tileLayer, r, c, rawTileID);
            else
                SDL_assert(!"unexpected fringe tile");
        }
    }
}

/// Picks the variation of the alternating 2x2 pattern for this cell.
static short GetPatternOffset(short row, short col)
{
    if ((row & 1) == 0)
        return (col & 1) == 0 ? 2 : 3;
    return (col & 1) == 0 ? 3 : 2;
}

/// Draws the top, right and top-right fringe around a wall-style tile. `getFringe` reads the
/// fringe of a neighbouring cell; `topVar`/`rightVar`/`rightAltVar` are offsets into the set.
static void DrawWallStyleFringe(SpriteWorldPtr spriteWorldP, TileMapStructPtr tileMapP,
                                short (*getFringe)(TileMapStructPtr, short, short),
                                short row, short col, short fringe,
                                short topVar, short rightVar, short rightAltVar)
{
    short layer;

    // Above
    if (getFringe(tileMapP, row - 1, col) != fringe && row - 1 >= 0) {
        layer = gLowestFringeLayer;
        if (getFringe(tileMapP, row - 1, col - 1) == fringe)
            DrawTileIfReplacingFringe(spriteWorldP, layer, row - 1, col, fringe + 5);
        else if (getFringe(tileMapP, row, col - 1) == fringe || col == 0)
            DrawTileIfReplacingFringe(spriteWorldP, layer, row - 1, col, topVar + fringe);
        else
            DrawTileIfReplacingFringe(spriteWorldP, gHighestFringeLayer, row - 1, col, fringe);
    }

    // Right
    if (getFringe(tileMapP, row, col + 1) != fringe && col + 1 < tileMapP->numCols) {
        layer = gMiddleFringeLayer;
        if (getFringe(tileMapP, row + 1, col + 1) == fringe)
            DrawTileIfReplacingFringe(spriteWorldP, layer, row, col + 1, fringe + 5);
        else if (getFringe(tileMapP, row + 1, col) == fringe || row == tileMapP->numRows - 1)
            DrawTileIfReplacingFringe(spriteWorldP, layer, row, col + 1, rightVar + fringe);
        else
            DrawTileIfReplacingFringe(spriteWorldP, layer, row, col + 1, rightAltVar + fringe);
    }

    // Top-right corner
    if (getFringe(tileMapP, row, col + 1) != fringe && getFringe(tileMapP, row - 1, col) != fringe &&
        col + 1 < tileMapP->numCols && row - 1 >= 0)
        DrawTileIfReplacingFringe(spriteWorldP, gLowestFringeLayer, row - 1, col + 1, fringe + 4);
}

void DrawFringeForWallTile(SpriteWorldPtr spriteWorldP, short tileLayer, short row, short col, short tileID)
{
    TileMapStructPtr tileMapP;
    short fringe, topVar, rightVar = 7, rightAltVar = 9, index;

    if (tileID >= 0xd3 && tileID <= 0x18a) {
        DrawFringeForEyeCandyTile(spriteWorldP, tileLayer, row, col, tileID);
        return;
    }

    SDL_assert(GetTileFringe(tileID) != 0);
    tileMapP = spriteWorldP->tileLayerArray[tileLayer];
    fringe = GetTileFringe(tileID);
    topVar = GetPatternOffset(row, col);

    if (tileID >= 0x1af && tileID <= 0x1e6) {
        if (tileID <= 0x1bc)
            index = tileID - 0x1af;
        else if (tileID <= 0x1ca)
            index = tileID - 0x1bd;
        else
            index = 0;
        if (index == 1)
            rightVar = 6;
        if (index == 3 && GetFringeForWallTile(tileMapP, row + 1, col) != fringe)
            rightAltVar = 8;
        if (index == 0)
            topVar = 2;
        else if (index == 1)
            topVar = 3;
    }

    DrawWallStyleFringe(spriteWorldP, tileMapP, GetFringeForWallTile, row, col, fringe,
                        topVar, rightVar, rightAltVar);
}

void DrawFringeForEyeCandyTile(SpriteWorldPtr spriteWorldP, short tileLayer, short row, short col,
                               short tileID)
{
    TileMapStructPtr tileMapP;
    short fringe, topVar, rightVar = 7, rightAltVar = 9, index = 0;

    SDL_assert(GetTileFringe(tileID) != 0);
    tileMapP = spriteWorldP->tileLayerArray[tileLayer];
    fringe = GetTileFringe(tileID);
    topVar = GetPatternOffset(row, col);

    if (tileID >= 0xd3 && tileID <= 0xf6)
        index = tileID - 199;
    else if (tileID >= 0xf7 && tileID <= 0x11a)
        index = tileID - 0xeb;
    else if (tileID >= 0x11b && tileID <= 0x13e)
        index = tileID - 0x10f;
    else if (tileID >= 0x13f && tileID <= 0x15a)
        index = tileID - 299;
    else if (tileID >= 0x15b && tileID <= 0x18a)
        index = tileID - 0x15b;

    switch (index) {
        case 1: case 5: case 9: case 0xd: case 0x11: case 0x15: case 0x19: case 0x1e:
        case 0x21: case 0x23: case 0x27: case 0x2c: case 0x2f:
            rightVar = 6;
            break;
    }
    switch (index) {
        case 3: case 7: case 0xb: case 0xf: case 0x13: case 0x17: case 0x1b: case 0x1f:
        case 0x21: case 0x25: case 0x29: case 0x2d: case 0x2f:
            if (GetFringeForTile(tileMapP, row + 1, col) != fringe)
                rightAltVar = 8;
            break;
    }
    switch (index) {
        case 0: case 4: case 8: case 0xc: case 0x10: case 0x14: case 0x18: case 0x1c:
        case 0x22: case 0x26: case 0x2a:
            topVar = 2;
            break;
    }
    switch (index) {
        case 1: case 5: case 9: case 0xd: case 0x11: case 0x15: case 0x19: case 0x1d:
        case 0x23: case 0x27: case 0x2b:
            topVar = 3;
            break;
    }
    if (index == 0x1e || index == 0x20 || index == 0x2d || index == 0x2f)
        topVar = 2;

    DrawWallStyleFringe(spriteWorldP, tileMapP, GetFringeForTile, row, col, fringe,
                        topVar, rightVar, rightAltVar);
}

void DrawFringeForFloorTile(SpriteWorldPtr spriteWorldP, short tileLayer, short row, short col,
                            short tileID)
{
    short fringe = GetTileFringe(tileID);

    (void)tileLayer;
    SDL_assert(fringe != 0);
    if (tileID == fringe)
        DrawTileIfReplacingFringe(spriteWorldP, gHighestFringeLayer, row - 1, col, tileID + 3);
    else
        DrawTileIfReplacingFringe(spriteWorldP, gLowestFringeLayer, row - 1, col, tileID + 3);

    if (tileID - fringe == 2) {
        DrawTileIfReplacingFringe(spriteWorldP, gMiddleFringeLayer, row, col + 1, tileID + 5);
        DrawTileIfReplacingFringe(spriteWorldP, gLowestFringeLayer, row - 1, col + 1, tileID + 4);
    }
}

void DrawFringeForLavaTile(SpriteWorldPtr spriteWorldP, short tileLayer, short row, short col,
                           short tileID)
{
    TileMapStructPtr tileMapP;
    short fringe, topVar, cornerVar;

    SDL_assert(GetTileFringe(tileID) != 0);
    tileMapP = spriteWorldP->tileLayerArray[tileLayer];
    fringe = GetTileFringe(tileID);
    topVar = (col & 1) ? 0x11 : 0x10;
    cornerVar = (col & 1) ? 8 : 9;

    if (GetFringeForTile(tileMapP, row - 1, col) != fringe && row - 1 >= 0) {
        if (GetFringeForTile(tileMapP, row, col - 1) == fringe || col == 0)
            DrawTileIfReplacingFringe(spriteWorldP, gLowestFringeLayer, row - 1, col, topVar + fringe);
        else
            DrawTileIfReplacingFringe(spriteWorldP, gHighestFringeLayer, row - 1, col,
                                      ((col & 1) != 0) + fringe);
    }

    if (GetFringeForTile(tileMapP, row, col + 1) != fringe &&
        GetFringeForTile(tileMapP, row - 1, col) != fringe &&
        col + 1 < tileMapP->numCols && row - 1 >= 0)
        DrawTileIfReplacingFringe(spriteWorldP, gLowestFringeLayer, row - 1, col + 1, cornerVar + fringe);
}

/// Picks the left/middle/right floor piece depending on the neighbouring floor tiles.
void DrawFloorTile(SpriteWorldPtr spriteWorldP, short tileLayer, short row, short col, short tileID)
{
    TileMapStructPtr tileMapP = spriteWorldP->tileLayerArray[tileLayer];
    short fringe = GetTileFringe(tileID);
    short leftFringe = GetTileFringe(GetEditorTileID(tileMapP, row, col - 1));
    short rightFringe = GetTileFringe(GetEditorTileID(tileMapP, row, col + 1));
    short newTileID;

    if (leftFringe == fringe)
        newTileID = (rightFringe == fringe) ? fringe + 1 : fringe + 2;
    else
        newTileID = (rightFringe == fringe) ? fringe : tileID;

    if (newTileID != tileID) {
        SWDrawTile(spriteWorldP, tileLayer, row, col, newTileID);
        MMDrawFringe(spriteWorldP, tileLayer, row, col);
    }
}

Boolean GEIsTileFloorTile(short tileID)
{
    return (tileID >= 0x241 && tileID <= 0x243) || (tileID >= 0x249 && tileID <= 0x24b) ||
           (tileID >= 0x2c1 && tileID <= 0x2c3) || (tileID >= 0x2e9 && tileID <= 0x2eb);
}

short GetCorrectTileForRowAndCol(short familyTileID, short row, short col)
{
    if ((row & 1) == 0)
        return (col & 1) == 0 ? familyTileID : familyTileID + 1;
    return (col & 1) == 0 ? familyTileID + 2 : familyTileID + 3;
}

short GetEditorTileID(TileMapStructPtr tileMapP, short row, short col)
{
    if (row < 0 || row >= tileMapP->numRows || col < 0 || col >= tileMapP->numCols)
        return -1;
    return ConvertTileID(tileMapP->tileMap[row][col]);
}

short GetEditorTileIDNoConvert(TileMapStructPtr tileMapP, short row, short col)
{
    if (row < 0 || row >= tileMapP->numRows || col < 0 || col >= tileMapP->numCols)
        return -1;
    return tileMapP->tileMap[row][col];
}

short GetFringeForWallTile(TileMapStructPtr tileMapP, short row, short col)
{
    short tileID = GetEditorTileIDNoConvert(tileMapP, row, col);
    short converted = ConvertTileID(tileID);
    // Off-map cells read index -1 in the original, which held 0.
    short base = (converted < 0 || converted >= kNumEditorTiles) ? 0 : gGEFringeArray[converted];
    return (short)(base + (tileID - converted));
}

short GetFringeForTile(TileMapStructPtr tileMapP, short row, short col)
{
    short tileID = GetEditorTileID(tileMapP, row, col);
    if (tileID < 0 || tileID >= kNumEditorTiles)
        return 0;
    return gGEFringeArray[tileID];
}

/// Fringe tiles never overwrite real tiles in the upper layers.
void DrawTileIfReplacingFringe(SpriteWorldPtr spriteWorldP, short tileLayer, short row, short col,
                               short tileID)
{
    TileMapStructPtr tileMapP = spriteWorldP->tileLayerArray[tileLayer];

    if (tileLayer > 4 && GetFringeForTile(tileMapP, row, col) != 1 &&
        GetEditorTileID(tileMapP, row, col) > 0)
        return;
    MyDrawTile(spriteWorldP, tileLayer, row, col, tileID);
}
