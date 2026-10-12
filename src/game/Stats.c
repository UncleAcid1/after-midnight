/*
 * Stats.c — the status bar at the top of the screen: lives, score and the items
 * Jack carries. It is a second sprite world drawn over the main one.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources & Headers/Stats.c").
 */
#include "MMGame.h"

#define kStatsBackgroundPict            5000    ///< 6 item slots
#define kStatsBackgroundWithBackpack    5010    ///< 8 item slots
#define kLanternItem                    7

void CreateStatsArea(void)
{
    SWRect worldRect, backRect;
    SWError err;

    gStatsHeight = 48;
    worldRect.top = 0;
    worldRect.left = gSpriteWorldP->worldRect.left;
    worldRect.bottom = gStatsHeight;
    worldRect.right = gSpriteWorldP->worldRect.right;
    backRect = worldRect;
    SWOffsetRect(&backRect, -worldRect.left, 0);

    err = SWCreateAdditionalSpriteWorld(&gStatsSpriteWorldP, gSpriteWorldP, &backRect);
    if (err != kNoError)
        MM_FATAL(err);
    err = SWChangeWorldRect(gStatsSpriteWorldP, &worldRect, true);
    if (err != kNoError)
        MM_FATAL(err);

    MM_FATAL_IF(SWCreateSpriteLayer(&gStatsBackgroundSpriteLayerP));
    MM_FATAL_IF(SWCreateSpriteLayer(&gStatsSpriteLayerP));
    MM_FATAL_IF(SWCreateSpriteLayer(&gDigitsStatsSpriteLayerP));
    MM_FATAL_IF(SWCreateSpriteLayer(&gStatsMapTextSpriteLayerP));
    MM_FATAL_IF(SWCreateSpriteLayer(&gStatsFadeSpriteLayerP));
    SWAddSpriteLayer(gStatsSpriteWorldP, gStatsBackgroundSpriteLayerP);
    SWAddSpriteLayer(gStatsSpriteWorldP, gStatsSpriteLayerP);
    SWAddSpriteLayer(gStatsSpriteWorldP, gDigitsStatsSpriteLayerP);
    SWAddSpriteLayer(gStatsSpriteWorldP, gStatsMapTextSpriteLayerP);
    SWAddSpriteLayer(gStatsSpriteWorldP, gStatsFadeSpriteLayerP);
}

void ResetStatsArea(void)
{
    SpritePtr spriteP;

    RemoveAllStatsItems();
    gCurNumStatsSlots = 6;
    LoadStatsBackFrame(kStatsBackgroundPict);
    SWDisposeAllSpritesInLayer(gDigitsStatsSpriteLayerP);

        // lives (2 digits) and score (6 digits)
    spriteP = NewStatsDigitsSprite(2, false);
    SetStatsSpriteDrawProc(spriteP, gSpriteDrawProc);
    SWSetSpriteLocation(spriteP, 37, 13);
    spriteP = NewStatsDigitsSprite(6, true);
    SetStatsSpriteDrawProc(spriteP, gSpriteDrawProc);
    SWSetSpriteLocation(spriteP, 500, 13);
    SWUpdateWindowSize(gStatsSpriteWorldP);
}

void RestoreStatsFromObjectsCarried(void)
{
    short item, count;

    RemoveAllStatsItems();
    if (!gObjects.hasBackpack) {
        gCurNumStatsSlots = 6;
        LoadStatsBackFrame(kStatsBackgroundPict);
    } else {
        AddBackpackToStatsArea();
    }
    if (gObjects.hasLantern)
        AddLanternToStatsArea();
    for (item = 0; item < 7; item++) {
        for (count = gObjects.counts[item]; count > 0; count--)
            AddItemToStats(item);
    }
}

void LoadStatsBackFrame(short pictID)
{
    char folder[1024];
    SWError err;
    snprintf(folder, sizeof(folder), "%s", GetSpriteImagePath(NULL));
    err = LoadAndInstallStatsBackgroundSprite(NULL, folder, pictID);
    if (err != kNoError)
        MM_FATAL(err);
}

SWError LoadAndInstallStatsBackgroundSprite(SpritePtr *spritePP, const char *folderPath, short pictID)
{
    char path[400];
    SpritePtr spriteP;
    SWError err;

    snprintf(path, sizeof(path), "%s%05d.png", folderPath, pictID);
    err = SWCreateSpriteFromFile(&spriteP, NULL, path, 1, false);
    if (err == kNoError) {
        MySetSpriteScaledSize(spriteP, 640, 48);
        RemoveAllSpritesInLayer(gStatsBackgroundSpriteLayerP);
        SWAddSprite(gStatsBackgroundSpriteLayerP, spriteP);
        SWSetSpriteLocation(spriteP, 0, 0);
        if (spritePP != NULL)
            *spritePP = spriteP;
    }
    return err;
}

void AddBackpackToStatsArea(void)
{
    gObjects.hasBackpack = true;
    gCurNumStatsSlots = 8;
    LoadStatsBackFrame(kStatsBackgroundWithBackpack);
}

void LoadAppropriateStatsBackground(void)
{
    LoadStatsBackFrame(gObjects.hasBackpack ? kStatsBackgroundWithBackpack : kStatsBackgroundPict);
}

void AddLanternToStatsArea(void)
{
    SpritePtr spriteP = NewStatsItemSprite(kLanternItem);
    SWSetSpriteLocation(spriteP, 406, 9);
}

void UpdateStatsItems(void)
{
    SWProcessSpriteWorld(gStatsSpriteWorldP);
}

void DrawStatsArea(void)
{
    UpdateStatsItems();
}

/// The 2-digit counter shows the lives left, the longer one the score.
void StatsDigitsMoveProc(SpritePtr spriteP)
{
    StatsSpritePtr statsP = (StatsSpritePtr)spriteP;
    SetStatsSpriteNumber(spriteP, statsP->numDigits < 3 ? gNumLivesLeft : gScore);
}

/// Adds an item to the next free slot. When the slots are full, an item of type 6
/// makes room. Returns false if there was no room.
Boolean AddItemToStats(short item)
{
    SpritePtr spriteP;
    Boolean added;

    if (gNumItemsBeingCarried >= gCurNumStatsSlots && gObjects.counts[6] > 0) {
        RemoveItemFromStats(6);
        gObjects.counts[6]--;
    }
    added = gNumItemsBeingCarried < gCurNumStatsSlots;
    if (added) {
        gStatsItemArray[gNumItemsBeingCarried] = item;
        spriteP = NewStatsItemSprite(item);
        gStatsItemSpriteArray[gNumItemsBeingCarried] = spriteP;
        SWSetSpriteLocation(spriteP, (short)(gNumItemsBeingCarried * 36 + 104), 9);
        gNumItemsBeingCarried++;
    }
    return added;
}

/// Removes the first carried item of this type and slides the rest left.
void RemoveItemFromStats(short item)
{
    SpritePtr spriteP;
    short slot, i;

    for (slot = 0; slot < gNumItemsBeingCarried; slot++) {
        if (gStatsItemArray[slot] == item)
            break;
    }
    if (slot >= gNumItemsBeingCarried)
        return;

    spriteP = gStatsItemSpriteArray[slot];
    gStatsItemSpriteArray[slot] = NULL;
    SW_ASSERT(spriteP != NULL);
    SW_ASSERT(spriteP->userData == item);
    SWRemoveSpriteFromAnimation(spriteP, true);
    for (i = slot; i < gNumItemsBeingCarried - 1; i++) {
        gStatsItemArray[i] = gStatsItemArray[i + 1];
        gStatsItemSpriteArray[i] = gStatsItemSpriteArray[i + 1];
        SWOffsetSprite(gStatsItemSpriteArray[i], -36, 0);
    }
    gNumItemsBeingCarried--;
}

void RemoveAllStatsItems(void)
{
    SWDisposeAllSpritesInLayer(gStatsSpriteLayerP);
    gNumItemsBeingCarried = 0;
}

void HideStatsSprites(void)
{
    SWSetSpritesInLayerVisible(gStatsSpriteLayerP, false);
    SWSetSpritesInLayerVisible(gDigitsStatsSpriteLayerP, false);
}

void MakeStatsSpritesVisible(void)
{
    SWSetSpritesInLayerVisible(gStatsSpriteLayerP, true);
    SWSetSpritesInLayerVisible(gDigitsStatsSpriteLayerP, true);
}

/// Debug display: frame rate, room resource ID and a third counter.
void CreateFPSAndLevelResIDSprites(void)
{
    SpritePtr spriteP;

    if (!gDisplayFPSAndLevelResID)
        return;
    spriteP = NewSmallDigitsSprite(3, false);
    SetStatsSpriteDrawProc(spriteP, gSpriteDrawProc);
    SWSetSpriteLocation(spriteP, 580, 2);
    SmallDigitsMoveProc(spriteP);

    spriteP = NewSmallDigitsSprite(5, false);
    SetStatsSpriteDrawProc(spriteP, gSpriteDrawProc);
    SWSetSpriteLocation(spriteP, 0, 2);
    SmallDigitsMoveProc(spriteP);

    spriteP = NewSmallDigitsSprite(9, false);
    SetStatsSpriteDrawProc(spriteP, gSpriteDrawProc);
    SWSetSpriteLocation(spriteP, 460, (float)gSpriteWorldP->visScrollRect.bottom - SWGetSpriteHeight(spriteP));
    SmallDigitsMoveProc(spriteP);
}
