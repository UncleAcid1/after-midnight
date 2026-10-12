/*
 * ZapperStuff.c — zapper nodes and the electric beams between them.
 *
 * Node types: 0x1a shoots right, 0x1b down, 0x1c left, 0x1d up. A right/up
 * node pairs with the left/down node facing it and owns the beam sprites.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources & Headers/ZapperStuff.c").
 */
#include "MMGame.h"

enum { kZapperRight = 0x1a, kZapperDown = 0x1b, kZapperLeft = 0x1c, kZapperUp = 0x1d };

static short TileRowOf(SpritePtr spriteP)
{
    return (short)(SWGetSpriteVertLoc(spriteP) / (float)gSpriteWorldP->tileHeight);
}

static short TileColOf(SpritePtr spriteP)
{
    return (short)(SWGetSpriteHorizLoc(spriteP) / (float)gSpriteWorldP->tileWidth);
}

static Boolean IsBeamOn(SpritePtr nodeP)
{
    return ZAPPER(nodeP)->isOn && !ZAPPER(nodeP)->isDisabled;
}

/// Switches the beam on and off on a timer.
void ZapperNodeMoveProc(SpritePtr spriteP)
{
    ZapperNodeSpritePtr zP = ZAPPER(spriteP);
    SpritePtr partnerP = zP->partnerP;
    Boolean visible;
    short loops = 0;

    if (partnerP == NULL || --zP->timer >= 1)
        return;
    do {
        zP->isOn = !zP->isOn;
        if (!zP->isOn) {
            zP->timer = zP->offTime;
            SWSetSpriteFrameAdvance(spriteP, 0);
            SWSetSpriteFrameAdvance(partnerP, 0);
        }
        else {
            zP->frameCounter = 0;
            zP->timer = zP->onTime;
            if (!zP->isDisabled) {
                SWSetSpriteFrameAdvance(spriteP, 1);
                SWSetSpriteFrameAdvance(partnerP, 1);
            }
        }
        visible = zP->isOn && !zP->isDisabled;
        if (zP->endGlowP != NULL)
            SWSetSpriteVisible(zP->endGlowP, visible);
        if (zP->beamGlowP != NULL)
            SWSetSpriteVisible(zP->beamGlowP, visible);
        if (zP->farGlowP != NULL)
            SWSetSpriteVisible(zP->farGlowP, visible);
        loops++;
    } while (zP->timer == 0 && loops < 2);
}

/// The glows flicker.
void ZapperNodeFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    ZapperNodeSpritePtr zP = ZAPPER(spriteP);
    unsigned short level;

    (void)curFrameP; (void)frameIndex;
    if (++zP->frameCounter > 7)
        zP->frameCounter = 0;
    level = (unsigned short)GetRandom(0x3fff, 0x9fff);
    if (zP->endGlowP != NULL)
        SWSetSpriteTranslucency(zP->endGlowP, level);
    if (zP->beamGlowP != NULL)
        SWSetSpriteTranslucency(zP->beamGlowP, level);
    if (zP->farGlowP != NULL)
        SWSetSpriteTranslucency(zP->farGlowP, level);
}

/// Beam tiles animate with their node (8 frames per tile position when vertical).
void ZapperBeamMoveProc(SpritePtr spriteP)
{
    ZapperBeamSpritePtr beamP = BEAM(spriteP);
    SpritePtr nodeP = beamP->nodeP;

    if (!beamP->isHorizontal)
        SWSetCurrentFrameIndex(spriteP, ZAPPER(nodeP)->frameCounter + beamP->baseFrame);
    else
        SWSetCurrentFrameIndex(spriteP, ZAPPER(nodeP)->frameCounter * 8 + beamP->baseFrame);

    if (!IsBeamOn(nodeP)) {
        if (spriteP->isVisible)
            SWSetSpriteVisible(spriteP, false);
    }
    else if (!spriteP->isVisible) {
        SWSetSpriteVisible(spriteP, true);
    }
}

/// Glow sprites keep their node in misc1.
void ZapperGlowMoveProc(SpritePtr spriteP)
{
    SWSetSpriteVisible(spriteP, IsBeamOn(spriteP->misc1.sprite));
}

Boolean ZapperCanShootThroughTile(short row, short col)
{
    (void)row; (void)col;
    return true;
}

void DeleteZapperSpritesForNode(SpritePtr nodeP)
{
    short row = TileRowOf(nodeP), col = TileColOf(nodeP), last;

    if (nodeP->userData == kZapperRight) {
        last = ZAPPER(nodeP)->distance + col;
        while (++col <= last)
            DeleteZapperBeam(row, col, nodeP);
    }
    else if (nodeP->userData == kZapperUp) {
        last = row - ZAPPER(nodeP)->distance;
        while (--row >= last)
            DeleteZapperBeam(row, col, nodeP);
    }
    else {
        SW_ASSERT(false);
    }
}

/// Removes the beam tile at row/col belonging to nodeP, and the node's glows.
void DeleteZapperBeam(short row, short col, SpritePtr nodeP)
{
    ZapperNodeSpritePtr zP = ZAPPER(nodeP);
    SpritePtr beamP = NULL;

    for (;;) {
        beamP = FindSpriteAtTileRowAndCol(gZapperBeamSpriteLayerP, beamP, row, col);
        if (beamP == NULL)
            return;
        if (BEAM(beamP)->nodeP == nodeP) {
            SWRemoveSpriteFromAnimation(beamP, true);
            if (zP->endGlowP != NULL)
                SWRemoveSpriteFromAnimation(zP->endGlowP, true);
            if (zP->beamGlowP != NULL)
                SWRemoveSpriteFromAnimation(zP->beamGlowP, true);
            if (zP->farGlowP != NULL)
                SWRemoveSpriteFromAnimation(zP->farGlowP, true);
            zP->endGlowP = NULL;
            zP->beamGlowP = NULL;
            zP->farGlowP = NULL;
            return;
        }
        beamP = beamP->prevSpriteP;
        if (beamP == NULL)
            return;
    }
}

/// Creates the beam tiles (and three glows) for a beam of the given length.
void CreateZapperSpritesForNode(SpritePtr nodeP, short length)
{
    SpritePtr ownerP, beamP;
    short row, col, endRow, endCol, stepRow = 0, stepCol = 0, frame = 0, beamFrame;
    Boolean horizontal = false;

    if (nodeP->userData == kZapperRight || nodeP->userData == kZapperUp)
        ownerP = nodeP;
    else
        ownerP = ZAPPER(nodeP)->partnerP;

    row = TileRowOf(nodeP);
    col = TileColOf(nodeP);
    endRow = row;
    endCol = col;
    switch (nodeP->userData) {
        case kZapperRight:
            stepCol = 1;
            col++;
            endCol = length + col;
            horizontal = true;
            break;
        case kZapperDown:
            stepRow = 1;
            row++;
            endRow = length + row;
            break;
        case kZapperLeft:
            stepCol = -1;
            col--;
            endCol = col - length;
            horizontal = true;
            break;
        case kZapperUp:
            stepRow = -1;
            row--;
            endRow = row - length;
            break;
    }

    ZAPPER(nodeP)->beamGlowP = CreateZapperGlowSprite(ownerP, 1, horizontal, ZAPPER(nodeP)->distance - 1,
                                                      col, row, 0);
    ZAPPER(nodeP)->endGlowP = CreateZapperGlowSprite(ownerP, 2, horizontal, 0, col - stepCol, row - stepRow, 0);
    ZAPPER(nodeP)->farGlowP = CreateZapperGlowSprite(ownerP, 0, horizontal, 0, stepCol + endCol,
                                                     stepRow + endRow, 0);
    if (SWStickyError() != kNoError)
        MM_FATAL(SWStickyError());

        // each tile of the beam: frames cycle 0-3, the last tile uses the end frames
    while (row != endRow || col != endCol) {
        beamP = NewZapperBeamSprite(horizontal);
        if (beamP == NULL)
            break;
        SWSetSpriteLocation(beamP, gSpriteWorldP->tileWidth * col, row * gSpriteWorldP->tileHeight);
        if (!horizontal) {
            beamFrame = (3 - frame) * 8 + 0x20;
            SWSetCurrentFrameIndex(beamP, beamFrame);
            BEAM(beamP)->baseFrame = beamFrame;
        }
        else {
            SWSetCurrentFrameIndex(beamP, frame);
            BEAM(beamP)->baseFrame = frame;
        }
        BEAM(beamP)->nodeP = ownerP;
        ZapperBeamMoveProc(beamP);
        row += stepRow;
        col += stepCol;
        if ((col + stepCol == endCol && horizontal) || (row + stepRow == endRow && !horizontal)) {
            frame += 4;
        }
        else if (++frame > 3) {
            frame = 0;
        }
    }
}

/// kind 0 is the far end, 1 the beam's length, 2 this end.
SpritePtr CreateZapperGlowSprite(SpritePtr nodeP, short kind, Boolean horizontal, short length, short col,
                                 short row, short offsetV)
{
    SpritePtr glowP = NewMiscSprite(0xd5);
    float tileW = gSpriteWorldP->tileWidth, tileH = gSpriteWorldP->tileHeight;

    SWAddSprite(gZapperGlowSpriteLayerP, glowP);
    SWSetSpriteMoveProc(glowP, NULL);
    SWSetSpriteCollideProc(glowP, NULL);
    SWSetSpriteBlendMode(glowP, 0x302, 1);
    SWSetSpriteTranslucency(glowP, 0x8fff);
    if (!horizontal) {
        if (kind == 0)
            row = (short)(row + 1.0);
        SWSetSpriteLocation(glowP, tileW * (col - 1.5f), offsetV + (((row + 0.5f) - length) - 0.2f) * tileH);
    }
    else {
        if (kind == 0)
            col = (short)(col - 0.5);
        SWSetSpriteRotation(glowP, 90.0f);
        SWSetSpriteLocation(glowP, tileW * (((col - 1.8f) + length) - 0.5f), offsetV + (row - 0.5f) * tileH);
    }
    SWSetSpriteScaledFactor(glowP, 0.5f, (length + 1) * 0.25f);
    SWSetCurrentFrameIndex(glowP, kind);
    SWSetSpriteVisible(glowP, false);
    glowP->misc1.sprite = nodeP;
    SWSetSpriteMoveProc(glowP, ZapperGlowMoveProc);
    ZapperGlowMoveProc(glowP);
    return glowP;
}

void CreateZapperBeamsForRoom(void)
{
    SpritePtr nodeP;

    for (nodeP = gZapperNodeSpriteLayerP->headSpriteP; nodeP != NULL; nodeP = nodeP->nextSpriteP) {
        if (nodeP->userData == kZapperRight || nodeP->userData == kZapperUp) {
            ScanForOppositeZapperNode(nodeP);
            CreateBeamsForNode(nodeP);
        }
        if (nodeP->spriteMoveProc != NULL)
            nodeP->spriteMoveProc(nodeP);
    }
    if (SWStickyError() != kNoError)
        MM_FATAL(SWStickyError());
}

/// Looks along the node's direction for the node facing it, and pairs them.
void ScanForOppositeZapperNode(SpritePtr nodeP)
{
    TileMapStructPtr tileMapP = gSpriteWorldP->tileLayerArray[4];
    SpritePtr otherP;
    short row, col, stepRow = 0, stepCol = 0, distance = 0, typeDiff;

    switch (nodeP->userData) {
        case kZapperRight: stepCol = 1; break;
        case kZapperLeft:  stepCol = -1; break;
        case kZapperDown:  stepRow = 1; break;
        case kZapperUp:    stepRow = -1; break;
        default:           SW_ASSERT(false); break;
    }
    row = (short)((SWGetSpriteVertLoc(nodeP) + SWGetSpriteHeight(nodeP) / 2.0f) / gSpriteWorldP->tileHeight);
    col = (short)((SWGetSpriteHorizLoc(nodeP) + SWGetSpriteWidth(nodeP) / 2.0f) / gSpriteWorldP->tileWidth);

    for (;;) {
        col += stepCol;
        row += stepRow;
        if (row < 0 || row >= tileMapP->numRows || col < 0 || col >= tileMapP->numCols)
            return;
        otherP = FindSpriteAtTileRowAndCol(gZapperNodeSpriteLayerP, NULL, row, col);
        if (otherP != NULL && otherP != nodeP) {
            typeDiff = (short)otherP->userData - (short)nodeP->userData;
            if (typeDiff == 2 || typeDiff == -2) {
                otherP->frameAdvance = nodeP->frameAdvance;
                ZAPPER(nodeP)->partnerP = otherP;
                ZAPPER(otherP)->partnerP = nodeP;
                ZAPPER(nodeP)->distance = distance + 1;
                ZAPPER(otherP)->distance = distance + 1;
            }
            return;
        }
        distance++;
    }
}

/// The last sprite (searching backward from startP, or from the layer's tail)
/// inside the given tile.
SpritePtr FindSpriteAtTileRowAndCol(SpriteLayerPtr layerP, SpritePtr startP, short row, short col)
{
    SWRect tileRect;
    SpritePtr spriteP;

    tileRect.left = gSpriteWorldP->tileWidth * col;
    tileRect.right = tileRect.left + gSpriteWorldP->tileWidth;
    tileRect.top = gSpriteWorldP->tileHeight * row;
    tileRect.bottom = tileRect.top + gSpriteWorldP->tileHeight;
    for (spriteP = startP != NULL ? startP : layerP->tailSpriteP; spriteP != NULL; spriteP = spriteP->prevSpriteP) {
        if (SWIsSpriteInRect(spriteP, &tileRect))
            return spriteP;
    }
    return NULL;
}

/// (Re)creates the beam sprites if the beam's length changed.
void CreateBeamsForNode(SpritePtr nodeP)
{
    ZapperNodeSpritePtr zP = ZAPPER(nodeP);
    SpritePtr partnerP = zP->partnerP;
    short row, col, last, length = 0, partnerLength = 0;

    if (partnerP == NULL)
        return;

    row = TileRowOf(nodeP);
    col = TileColOf(nodeP);
    if (nodeP->userData == kZapperRight) {
        last = zP->distance + col;
        while (++col <= last && ZapperCanShootThroughTile(row, col))
            length++;
    }
    else if (nodeP->userData == kZapperUp) {
        last = row - zP->distance;
        while (--row >= last && ZapperCanShootThroughTile(row, col))
            length++;
    }
    else {
        SW_ASSERT(false);
    }

    if (zP->distance != length) {
        row = TileRowOf(partnerP);
        col = TileColOf(partnerP);
        if (partnerP->userData == kZapperLeft) {
            last = col - ZAPPER(partnerP)->distance;
            while (--col >= last && ZapperCanShootThroughTile(row, col))
                partnerLength++;
        }
        else if (partnerP->userData == kZapperDown) {
            last = ZAPPER(partnerP)->distance + row;
            while (++row <= last && ZapperCanShootThroughTile(row, col))
                partnerLength++;
        }
        else {
            SW_ASSERT(false);
        }
    }

    if (zP->beamLength != length || ZAPPER(partnerP)->beamLength != partnerLength) {
        DeleteZapperSpritesForNode(nodeP);
        if (zP->distance == length)
            CreateZapperSpritesForNode(nodeP, length);
        zP->beamLength = length;
        ZAPPER(partnerP)->beamLength = partnerLength;
    }
}
