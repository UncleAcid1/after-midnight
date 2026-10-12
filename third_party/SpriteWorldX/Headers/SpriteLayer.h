//---------------------------------------------------------------------------------------
/// @file SpriteLayer.h
/// constants, structures, and function prototypes for sprite layers
//
//  Portions are copyright: (C) 1991-94 Tony Myles, All rights reserved worldwide.
//---------------------------------------------------------------------------------------


#ifndef __SPRITELAYER__
#define __SPRITELAYER__

#ifndef __SWCOMMON__
#include <SWCommonHeaders.h>
#endif

#ifndef __SPRITE__
#include <Sprite.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define kSWSpriteLayerSanityValue	19623

//---------------------------------------------------------------------------------------
/// sprite layer data structure
//---------------------------------------------------------------------------------------

struct SpriteLayerRec
{
    SpriteWorldPtr  parentSpriteWorldP; ///< The SpriteWorld this layer is in, otherwise NULL.

    SpriteLayerPtr  nextSpriteLayerP;   ///< next sprite layer
    SpriteLayerPtr  prevSpriteLayerP;   ///< previous sprite layer

    SpritePtr       headSpriteP;        ///< head of sprite linked list
    SpritePtr       tailSpriteP;        ///< tail of sprite linked list


    int             isPaused;           ///< does this layer get processed by SWProcessSpriteWorld?
    int             layerIsNonScrolling;///< is this a non-scrolling SpriteLayer?

	SWBoolean		layerIsVisible;
	
	short			tileLayerBelow;		// -1 or a value indicating the tileLayer to be drawn below this SpriteLayer
	short			tileLayerAbove;		// -1 or a value indicating the tileLayer to be drawn above this SpriteLayer
//	short			tileLayer;              ///< the tile layer this sprite layer is under
	short			sanityValue;
        
    long            userData;           ///< reserved for user
};

//---------------------------------------------------------------------------------------
//  sprite layer function prototypes
//---------------------------------------------------------------------------------------

SWError SWCreateSpriteLayer(
    SpriteLayerPtr *spriteLayerP);

void SWDisposeSpriteLayer(
    SpriteLayerPtr *spriteLayerPP);

SWError SWAddSprite(
    SpriteLayerPtr spriteLayerP,
    SpritePtr newSpriteP);

SWError SWRemoveSprite(
    SpritePtr oldSpriteP);

void SWRemoveAllSpritesFromLayer(
    SpriteLayerPtr srcSpriteLayerP);

void SWDisposeAllSpritesInLayer(
    SpriteLayerPtr spriteLayerP);

unsigned long SWCountNumSpritesInLayer(
    SpriteLayerPtr spriteLayerP);

SWError SWSwapSprite(
    SpritePtr srcSpriteP,
    SpritePtr dstSpriteP);

SWError SWInsertSpriteAfterSprite(
    SpritePtr newSpriteP,
    SpritePtr dstSpriteP);

SWError SWInsertSpriteBeforeSprite(
    SpritePtr newSpriteP,
    SpritePtr dstSpriteP);

SpritePtr SWGetNextSprite(
    SpriteLayerPtr spriteLayerP,
    SpritePtr curSpriteP);

SpritePtr SWGetPreviousSprite(
    SpriteLayerPtr spriteLayerP,
    SpritePtr curSpriteP);

void SWLockSpriteLayer(
    SpriteLayerPtr spriteLayerP);

void SWUnlockSpriteLayer(
    SpriteLayerPtr spriteLayerP);

void SWMoveSpriteLayer(
    SpriteLayerPtr spriteLayerP);

void SWCollideSpriteLayer(
    SpriteWorldPtr  spriteWorldP,
    SpriteLayerPtr srcSpriteLayerP,
    SpriteLayerPtr dstSpriteLayerP);

int SWSpriteCollidesWithLayer(
    SpritePtr srcSpriteP,
    SpriteLayerPtr spriteLayerP,
    SpriteCollisionProcPtr collideFunc);

void SWSortSpriteLayer(
    SpriteLayerPtr theLayer);

void SWCustomSortSpriteLayer(
    SpriteLayerPtr theLayer,
    SpriteCompareProcPtr compareFunc);

void SWPauseSpriteLayer(
    SpriteLayerPtr  spriteLayerP);

void SWUnpauseSpriteLayer(
    SpriteLayerPtr  spriteLayerP);
	
void SWSetSpriteLayerVisible(
    SpriteLayerPtr	spriteLayerP, SWBoolean isVisible);

SWBoolean SWGetSpriteLayerVisible(
    SpriteLayerPtr	spriteLayerP);

void SWSetLayerAsNonScrolling(
    SpriteLayerPtr  spriteLayerP,
    int layerIsNonScrolling);

SpritePtr SWFindSpriteByPoint(
    SpriteLayerPtr spriteLayerP,
    SpritePtr startSpriteP,
    SWPoint test);

SpritePtr SWTopSpriteFromPoint(
    SpriteLayerPtr spriteLayerP,
    SpritePtr startSpriteP,
    SWPoint test,
    SpriteInMaskProcPtr testFunc);

void SWMoveSpriteLayerToTail (
    SpriteWorldPtr  spriteWorldP,
    SpriteLayerPtr moveSpriteLayerP);


#ifdef __cplusplus
}

SW_NAMESPACE_BEGIN

#include "SpriteLayer.hpp"

SW_NAMESPACE_END

#endif /* __cplusplus */

#endif /* __SPRITELAYER__ */

