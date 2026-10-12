/*
 * SWStats — sprites that draw a number with digit frames 0-9.
 * Reconstructed from the Midnight Mansion HD build (SpriteWorld X fork).
 * See SWCommonHeaders.h for license and provenance.
 */

#include "SpriteWorld.h"

/// Makes a stats sprite from a sprite whose first 10 frames are the digits 0-9.
SpritePtr CreateStatsSpriteClone(SpritePtr digitsSpriteP, short numDigits, long justification, long showLeadingZeros)
{
    StatsSpritePtr statsP;
    SpritePtr newSpriteP = NULL;
    SWError err = kNoError;

    SW_ASSERT(digitsSpriteP != NULL);
    SW_ASSERT(digitsSpriteP->maxFrames >= 10);
    statsP = calloc(1, sizeof(StatsSpriteRec));
    if (statsP == NULL)
        err = kMemoryAllocationError;
    if (err == kNoError)
        err = SWCloneSprite(digitsSpriteP, &newSpriteP, statsP, sizeof(StatsSpriteRec));
    if (err != kNoError)
        return NULL;
    newSpriteP->frameDrawProc = StatsItemDrawProc;
    SWSetCurrentFrameIndex(newSpriteP, newSpriteP->curFrameIndex);
    statsP->numDigits = numDigits;
    statsP->number = -1;
    statsP->justification = justification;
    statsP->showLeadingZeros = showLeadingZeros;
    statsP->itemDrawProc = SWStdSpriteDrawProc;
    return newSpriteP;
}

SWError SetUpStatsSprite(SpritePtr statsSpriteP, SpriteLayerPtr layerP, void (*itemDrawProc)(SpritePtr, FramePtr, float, float),
                         short horizLoc, short vertLoc, long number)
{
    if (statsSpriteP == NULL)
        return -1;
    SWSetSpriteLocation(statsSpriteP, (float)horizLoc, (float)vertLoc);
    SWAddSprite(layerP, statsSpriteP);
    SetStatsSpriteDrawProc(statsSpriteP, itemDrawProc);
    SetStatsSpriteNumber(statsSpriteP, number);
    return kNoError;
}

void SetStatsSpriteDrawProc(SpritePtr statsSpriteP, void (*itemDrawProc)(SpritePtr, FramePtr, float, float))
{
    ((StatsSpritePtr)statsSpriteP)->itemDrawProc = itemDrawProc;
}

void SetStatsSpriteNumber(SpritePtr statsSpriteP, long number)
{
    StatsSpritePtr statsP = (StatsSpritePtr)statsSpriteP;

    if (statsP->number != number) {
        statsP->number = number;
        statsSpriteP->needsToBeDrawn = true;
    }
}

long GetStatsSpriteNumber(SpritePtr statsSpriteP)
{
    return ((StatsSpritePtr)statsSpriteP)->number;
}

/// Draws the number one digit frame at a time, left to right.
void StatsItemDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    StatsSpritePtr statsP = (StatsSpritePtr)srcSpriteP;
    DrawDataPtr d = srcSpriteP->drawData;
    FramePtr *digits = srcSpriteP->frameArray;
    signed char digitArray[32];
    long number = statsP->number;
    short i, pos, digitWidth, h, v;
    signed char blank = statsP->showLeadingZeros ? 0 : -1;

    SW_UNUSED(curFrameP);
    SW_UNUSED(horizLoc);
    SW_UNUSED(vertLoc);
    for (i = 0; i < statsP->numDigits && i < 32; i++)
        digitArray[i] = blank;
    if (number >= 0) {
        if (statsP->justification == kLeftJustify)
            pos = GetNumberLength(number) - 1;
        else
            pos = statsP->numDigits - 1;
        do {
            if (pos < 32)
                digitArray[pos] = (signed char)(number % 10);
            pos--;
            number /= 10;
        } while (number >= 1 && pos >= 0);
    }

    h = srcSpriteP->destFrameRect.left;
    v = srcSpriteP->destFrameRect.top;
    digitWidth = (short)(int)(d->scaledWidthFactor * (float)(digits[0]->frameRect.right - digits[0]->frameRect.left));
    for (pos = 0; pos < statsP->numDigits && pos < 32; pos++) {
        short digit = digitArray[pos];
        if (digit >= 0) {
            d->horizLoc = (float)h;
            d->vertLoc = (float)v;
            if (gSWCurrentSpriteWorld != NULL) {
                h += gSWCurrentSpriteWorld->visScrollRect.left;
                v += gSWCurrentSpriteWorld->visScrollRect.top;
            }
            statsP->itemDrawProc(srcSpriteP, digits[digit], (float)h, (float)v);
        }
        h += digitWidth;
    }
}

short GetNumberLength(long number)
{
    short len = 1;

    if (number < 0)
        number = -number;
    while (number >= 10 && len < 10) {
        number /= 10;
        len++;
    }
    return len;
}
