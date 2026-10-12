/* SWStats — sprites that draw a number using digit frames 0-9.
 * Reconstructed from the Midnight Mansion HD build (SpriteWorld X fork). */
#ifndef __SWSTATS__
#define __SWSTATS__

#include "SWCommonHeaders.h"

#ifdef __cplusplus
extern "C" {
#endif

enum { kLeftJustify = 1, kRightJustify = 0 };

typedef struct StatsSpriteRec {
    SpriteRec   sprite;
    long        number;             ///< -1 draws nothing
    short       numDigits;
    long        justification;      ///< kLeftJustify or kRightJustify
    long        showLeadingZeros;
    void      (*itemDrawProc)(SpritePtr, FramePtr, float, float);
} StatsSpriteRec, *StatsSpritePtr;

SpritePtr CreateStatsSpriteClone(SpritePtr digitsSpriteP, short numDigits, long justification, long showLeadingZeros);
SWError   SetUpStatsSprite(SpritePtr statsSpriteP, SpriteLayerPtr layerP, void (*itemDrawProc)(SpritePtr, FramePtr, float, float),
                           short horizLoc, short vertLoc, long number);
void      SetStatsSpriteDrawProc(SpritePtr statsSpriteP, void (*itemDrawProc)(SpritePtr, FramePtr, float, float));
void      SetStatsSpriteNumber(SpritePtr statsSpriteP, long number);
long      GetStatsSpriteNumber(SpritePtr statsSpriteP);
void      StatsItemDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc);
short     GetNumberLength(long number);

#ifdef __cplusplus
}
#endif

#endif
