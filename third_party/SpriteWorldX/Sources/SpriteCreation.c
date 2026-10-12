//---------------------------------------------------------------------------------------
//	SpriteCreation.c
//
//	Portions are copyright: © 1991-94 Tony Myles, All rights reserved worldwide.
//  X86 Port to SDL created by: Michael Cabral 2003
//
//	Description:	sprite creation routines
//---------------------------------------------------------------------------------------

#ifndef __SWCOMMON__
#include <SWCommonHeaders.h>
#endif

#ifndef __SPRITEWORLD__
#include <SpriteWorld.h>
#endif

#ifndef __SPRITE__
#include <Sprite.h>
#endif

#ifndef __SPRITECREATION__
#include <SpriteCreation.h>
#endif

#ifndef __BLITKERNEL__
#include <BlitKernel.h>
#endif








//---------------------------------------------------------------------------------------
//  SWCreateDrawData
// This is the structure that holds all the blitter information of Sprites
//---------------------------------------------------------------------------------------

DrawDataPtr SWCreateDrawData (void)
{
    DrawDataPtr newDrawData;

    newDrawData = (DrawDataPtr)calloc(1,sizeof(DrawDataRec));
    if (newDrawData != 0) {
        newDrawData->parentSpriteP =  0;
        newDrawData->horizLoc = 0;
        newDrawData->vertLoc = 0;
        newDrawData->horizMoveDelta = 0;
        newDrawData->vertMoveDelta = 0;

        newDrawData->flippedHorizontal = false;
        newDrawData->flippedVertical = false;

        newDrawData->frameHotSpotH = 0;
        newDrawData->frameHotSpotV = 0;
		newDrawData->rotation = 0;
		newDrawData->scaledWidth = -1;
		newDrawData->scaledHeight = -1;
		
		newDrawData->scaledWidthFactor = 1.0;	// 1.0 = normal size
		newDrawData->scaledHeightFactor = 1.0;
		
		newDrawData->srcBlendMode = GL_SRC_ALPHA;
		newDrawData->dstBlendMode = GL_ONE_MINUS_SRC_ALPHA;
		
		newDrawData->translucencyLevel = 0xFFFF;
		newDrawData->colorRed = 0xFFFF;
		newDrawData->colorGreen = 0xFFFF;
		newDrawData->colorBlue = 0xFFFF;
    }
    else
    {
        SWSetStickyIfError( kMemoryAllocationError );
    }

    return newDrawData;
}


//---------------------------------------------------------------------------------------
//	SWCreateSpriteFromSingleFile
//---------------------------------------------------------------------------------------

SWError SWCreateSpriteFromSingleFile( 
									 SpritePtr* newSpriteP,
									 void* spriteStorageP,
									 const char* fileName,
									 short frameDimension,	// The width (if laid out horizontally) or height (if laid out vertically) of each frame
									 short borderSize)		// Border between each frame, in pixels
{
	SWError		err = kNoError;
	SDL_Surface	*surface;
/*	SpritePtr		tempSpriteP;
	int 			widthStart, heightStart;
	SWRect		frameRect;
	int			frame;
	FramePtr 		newFrameP;*/
	
	int		frameWidth, frameHeight, borderWidth, borderHeight, maxFrames;
	
	surface = BKLoadSurface( fileName );
	if ( !surface ) 
		return kSDLCreateSurfaceFromFile;
	
	//	SW_SET_RECT(frameRect, 0, 0, surface->w, surface->h);
	
	if ( surface->w < surface->h )	// Taller than it is wide?
	{
		/*	verticalStrip = true;
		 frameRect.bottom = -borderSize;
		 maxWidth = surface->w;
		 maxHeight = frameDimension;*/
		
		frameWidth = surface->w;
		frameHeight = frameDimension;
		borderWidth = 0;
		borderHeight = borderSize;
		maxFrames = (surface->h + borderSize) / (frameDimension + borderSize);
	}
	else	// Wider than it is tall...
	{
		/*	verticalStrip = false;
		 frameRect.right = -borderSize;
		 maxWidth = frameDimension;
		 maxHeight =surface->h;*/
		
		frameWidth = frameDimension;
		frameHeight = surface->h;
		borderHeight = 0;
		borderWidth = borderSize;
		maxFrames = (surface->w + borderSize) / (frameDimension + borderSize);
	}
	
	
	return SWCreateSpriteFromSurfaceXY( newSpriteP, spriteStorageP, surface, 
										  frameWidth, frameHeight, borderWidth, borderHeight, false, maxFrames );
}


//---------------------------------------------------------------------------------------
//	SWCreateSpriteFromSingleFileXY - Create a sprite from an Image file
//---------------------------------------------------------------------------------------

SWError SWCreateSpriteFromSingleFileXY( 
									   SpritePtr* newSpriteP,
									   void* spriteStorageP,
									   const char* fileName,
									   int frameWidth,
									   int frameHeight,
									   int borderWidth,
									   int borderHeight,
									   int hasOutsideBorder,
									   long maxFrames )
{
	SDL_Surface	*	surface;
	
	surface = BKLoadSurface( fileName );
	if ( !surface ) 
		return kSDLCreateSurfaceFromFile;
	
	
	return SWCreateSpriteFromSurfaceXY( newSpriteP, spriteStorageP, surface, 
									   frameWidth, frameHeight, borderWidth, borderHeight, hasOutsideBorder, maxFrames );
		
}


//---------------------------------------------------------------------------------------
//	SWCreateSpriteFromVerticalFile - for frames laid out vertically
//---------------------------------------------------------------------------------------

SWError SWCreateSpriteFromVerticalFile(SpritePtr* newSpriteP,
									   void* spriteStorageP,
									   const char* fileName,
									   int borderHeight,
									   long numFrames )
{
	SDL_Surface	*surface;
	
	surface = BKLoadSurface( fileName );
	if ( !surface ) 
		return kSDLCreateSurfaceFromFile;
	
	return SWCreateSpriteFromSurfaceXY( newSpriteP, spriteStorageP, surface, 
									   surface->w, surface->h/numFrames, 0, borderHeight, false, numFrames );
}


//---------------------------------------------------------------------------------------
//	SWCreateSpriteFromSurfaceXY - Called by BOTH methods above
//---------------------------------------------------------------------------------------

SWError SWCreateSpriteFromSurfaceXY( 
	SpritePtr* newSpriteP,
	void* spriteStorageP,
	SDL_Surface	*surface,
	int frameWidth,
	int frameHeight,
	int borderWidth,
	int borderHeight,
	int hasOutsideBorder,
	long maxFrames )
{
	SWError			err = kNoError;
	SpritePtr		tempSpriteP;
	int 			widthStart, heightStart;
	SWRect			frameRect;
	int				frame;
	FramePtr 		newFrameP;
		
	SW_ASSERT( surface );
	
	if( maxFrames == 0 )
		maxFrames = (surface->w/frameWidth) * (surface->h/frameHeight);
	
	if ( hasOutsideBorder )
	{
		widthStart = borderWidth / 2;
		heightStart = borderHeight / 2;
	}
	else
	{
		widthStart = 0;
		heightStart = 0;
	}

	err = SWCreateSprite( newSpriteP, spriteStorageP, maxFrames);
	
	
	if( err == kNoError )
	{
		tempSpriteP = *newSpriteP;
		tempSpriteP->sharedSurface = surface;

		frameRect.left = widthStart;
		frameRect.top = heightStart;
		frameRect.right = widthStart + frameWidth;
		frameRect.bottom = heightStart + frameHeight;
		
		for (frame = 0; frame < maxFrames; frame++)
		{
			err = SWCreateFrameFromSurfaceAndRect( &newFrameP, surface, &frameRect );
			
			if (err == kNoError)
			{
				err = SWAddFrame(tempSpriteP, newFrameP);
				if ( frame < maxFrames-1 )
				{
					frameRect.left += frameWidth + borderWidth;
					frameRect.right = frameRect.left + frameWidth;
					if ( frameRect.right > surface->w )
					{
						frameRect.left = widthStart;
						frameRect.right = widthStart + frameWidth;

						frameRect.top += frameHeight + borderHeight;
						frameRect.bottom = frameRect.top + frameHeight;
						if ( frameRect.bottom > surface->h )
						{
							err = kOutOfRangeErr;
						}
					}
				}
			}
			
			if ( err != kNoError )
			{
				SWDisposeFrame(&newFrameP);
				SWDisposeSprite(&tempSpriteP);
				break;
			}
		}
	}
		
	if (err == kNoError )
	{
		SWSetSpriteFrameRange(tempSpriteP, 0, maxFrames - 1);
		err = SWSetCurrentFrameIndex(tempSpriteP, 0);
	}
	
	SWSetStickyIfError( err );
	return err;
}


//---------------------------------------------------------------------------------------
//	SWCreateSpriteFromSequence - Create a sprite from a sequence of Image file
//  filename is a printf format string, use only one %d
//  ie pass "sprite%03d.png", 1, 200 will get "sprite001.png" through "sprite200.png"
//  pass "sprite-%d.png, 1, 200 will get "sprite-1.png" through "sprite-200.png"
//---------------------------------------------------------------------------------------

SWError SWCreateSpriteFromFileSequence(
	SpritePtr* newSpriteP,
	void* spriteStorageP, 
    const char *filename, 
	int start, 
	long maxFrames)
{
	SWError err = kNoError;
    char tmp_file[255];
    int x;
    
    FramePtr tempFrameP;
    SpritePtr tempSpriteP;
    
	SW_UNUSED(spriteStorageP);

    tempSpriteP = *newSpriteP;
    
    /*initialize the sprite*/
	err =  SWCreateSprite(&tempSpriteP, 0, maxFrames);
	
	if (err == kNoError)
	{
		for(x = 0; x < maxFrames; x++)
		{
			sprintf(tmp_file, filename, x+start);
			
			err = SWCreateFrameFromFile(&tempFrameP, tmp_file);
			
			if (err == kNoError)
			{
				err = SWAddFrame(tempSpriteP, tempFrameP);
			}
			
			if (err != kNoError) 
			{
				SWDisposeFrame(&tempFrameP);
				SWDisposeSprite(&tempSpriteP);
				break;
			}
		}
	}
    
	if (err == kNoError)
	{
		SWSetSpriteFrameRange(tempSpriteP, 0, maxFrames - 1);
		err = SWSetCurrentFrameIndex(tempSpriteP, 0);
		*newSpriteP = tempSpriteP;
	}
	
	return err;
}

//---------------------------------------------------------------------------------------
//	SWCreateSpriteFromFile - Create a sprite from an Image file
//
//  NOTE: This is for single-frame Sprites only! I'm not sure where this function accepts
//  a "maxFrames" parameter -- the caller would have to use SWAddFrame() to add to the sprite
//  before it is locked and used.
//---------------------------------------------------------------------------------------

SWError SWCreateSpriteFromFile(
	SpritePtr* newSpriteP,
	void* spriteStorageP,
	const char * file,
	long maxFrames)
{
	SWError err = kNoError;
    FramePtr  tempFrameP = 0;
    SpritePtr tempSpriteP = 0;
    
    /*initialize the sprite*/
    err =  SWCreateSprite(&tempSpriteP, spriteStorageP, maxFrames);
	if (err == kNoError)
	{
		/*Add a single frame to the Sprite (for now) */
		err = SWCreateFrameFromFile(&tempFrameP, file);
	}
	
	if (err == kNoError)
	{
		err = SWAddFrame(tempSpriteP, tempFrameP);
	}

	if(err == kNoError )
	{
		err = SWSetCurrentFrameIndex(tempSpriteP, 0);
	}
    
	if (err == kNoError)
	{
		*newSpriteP = tempSpriteP;
	}
	
	return err;
}

//---------------------------------------------------------------------------------------
//	SWCreateSpriteFromFileMask - Create a sprite from an Image file
//---------------------------------------------------------------------------------------

/*Create a sprite from an Image file*/
SWError SWCreateSpriteFromFileMask(
	SpritePtr* newSpriteP,
	void* spriteStorageP,
	const char * file,
	const char * mask,
	SWBoolean alpha,
	long maxFrames)
{
	SWError err = kNoError;
    FramePtr  tempFrameP = 0;
    SpritePtr tempSpriteP = 0;
    
    /*initialize the sprite*/
    err =  SWCreateSprite(&tempSpriteP, spriteStorageP, maxFrames);
	if (err == kNoError)
	{
		/*Add a single frame to the Sprite (for now) */
		err = SWCreateFrameFromFileMask(&tempFrameP, file, mask, alpha);
	}
	
	if (err == kNoError)
	{
		err = SWAddFrame(tempSpriteP, tempFrameP);
	}

	if(err == kNoError )
	{
		err = SWSetCurrentFrameIndex(tempSpriteP, 0);
	}
    
	if (err == kNoError)
	{
		*newSpriteP = tempSpriteP;
	}
	
	return err;
}

//---------------------------------------------------------------------------------------
//  SWCloneSprite
//---------------------------------------------------------------------------------------

SWError SWCloneSprite(
	SpritePtr	cloneSpriteP,
	SpritePtr*	newSpriteP,
	void*		spriteStorageP)
{
	SpritePtr 		tempSpriteP;
	short 			index;
	int				frameArraySize;
	SWError 		err = kNoError;

	SW_ASSERT(cloneSpriteP != NULL);

		// Allocate memory for the new Sprite
	tempSpriteP = (spriteStorageP == NULL) ? (SpritePtr)malloc(sizeof(SpriteRec)) : (SpritePtr)spriteStorageP;
	if (tempSpriteP == NULL)
		err = kMemoryAllocationError;

	if (err == kNoError)
	{
			// Make sure the Ptr passed to this function is large enough.
		//SW_ASSERT( GetPtrSize((Ptr)tempSpriteP) >= sizeof(SpriteRec) );
		
			// copy the clone sprite into the temp sprite
		*tempSpriteP = *cloneSpriteP;

			// clear the layer fields, in case the parent sprite is in a layer
		tempSpriteP->parentSpriteLayerP = NULL;
		tempSpriteP->nextSpriteP = NULL;
		tempSpriteP->prevSpriteP = NULL;

			// Initialize other fields
		tempSpriteP->spriteMoveProc = SWTempSpriteMoveProc;
		tempSpriteP->realSpriteMoveProc = cloneSpriteP->realSpriteMoveProc;
		tempSpriteP->drawData = (DrawDataPtr)calloc(1,sizeof(DrawDataRec));

		*(tempSpriteP->drawData) = *(cloneSpriteP->drawData);
		tempSpriteP->drawData->parentSpriteP = tempSpriteP;
	}

	if (err == kNoError)
	{
			// Create a new frameArray for this sprite
		frameArraySize = cloneSpriteP->maxFrames * sizeof(FramePtr);
		tempSpriteP->frameArray = (FramePtr *)calloc(1,frameArraySize);
		
		if (tempSpriteP->frameArray != NULL)
		{
				// copy the frame array, and increment useCount for each frame
			for (index = 0; index < cloneSpriteP->maxFrames; index++)
			{
				tempSpriteP->frameArray[index] = cloneSpriteP->frameArray[index];
				SW_ASSERT(tempSpriteP->frameArray[index] != NULL );
				tempSpriteP->frameArray[index]->useCount++;
			}

		}
		else
		{
			err = kMemoryAllocationError;
			free((void*)tempSpriteP);
		}
	}

	if (err == kNoError)
		*newSpriteP = tempSpriteP;

	SWSetStickyIfError( err );
	return err;
}


///--------------------------------------------------------------------------------------
//	SWCloneSpriteFromTile - NEEDS PORTING TO SDL
///--------------------------------------------------------------------------------------

/*
SW_FUNC OSErr SWCloneSpriteFromTile(
									SpriteWorldPtr spriteWorldP,
									SpritePtr* newSpriteP,
									void* spriteStorageP,
									short firstTileID,
									short lastTileID)
{

	OSErr 			err;
	SpritePtr 		tempSpriteP;
	FramePtr 		tileFrameP;
	short 			curTileID, curIndex, numFrames;
	
	SW_ASSERT(firstTileID <= lastTileID);
	SW_ASSERT(spriteWorldP->tileFrameArray != NULL);
	SW_ASSERT(firstTileID >= 0 && lastTileID < spriteWorldP->maxNumTiles);
	
	*newSpriteP = NULL;
	numFrames = (lastTileID - firstTileID) + 1;
	err = SWCreateSprite(&tempSpriteP, spriteStorageP, numFrames);
	
	if (err == noErr)
	{
		for (curTileID = firstTileID; curTileID <= lastTileID; curTileID++)
		{
			tileFrameP = spriteWorldP->tileFrameArray[curTileID];
			SW_ASSERT(tileFrameP != NULL);
			err = SWAddFrame(tempSpriteP, tileFrameP);
			
			if (err != noErr)
			{
				SWDisposeSprite(&tempSpriteP);
				break;
			}
		}
		
		if (err == noErr)
		{
			SWSetSpriteFrameRange(tempSpriteP, 0, numFrames - 1);
			
			// Set current index to be the same as the Tile's current image, if possible.
			curIndex = spriteWorldP->curTileImage[firstTileID];
			if (curIndex >= firstTileID && curIndex <= lastTileID)
				curIndex = spriteWorldP->curTileImage[firstTileID] - firstTileID;
			else
				curIndex = 0;
			
			SWSetCurrentFrameIndex(tempSpriteP, curIndex);
			*newSpriteP = tempSpriteP;
		}
	}
	
	SWSetStickyIfError( err );
	return err;

}
*/

//---------------------------------------------------------------------------------------
//	SWCreateSprite
//---------------------------------------------------------------------------------------

SWError SWCreateSprite(
	SpritePtr* newSpriteP,
	void* spriteStorage,
	long maxFrames)
{
	SWError err = kNoError;
    long frameArraySize;
    SpritePtr tempSpriteP = 0;
    *newSpriteP = 0;
    
    tempSpriteP = (spriteStorage == 0) ? (SpritePtr)calloc(1,sizeof(SpriteRec)) : (SpritePtr)spriteStorage;
    
    if(tempSpriteP)
    {
		frameArraySize = maxFrames * sizeof(FramePtr);
		tempSpriteP->frameArray = (FramePtr*)calloc(1,frameArraySize);
		
		if (tempSpriteP->frameArray)
		{
                tempSpriteP->parentSpriteLayerP = 0;
                tempSpriteP->nextSpriteP = 0;
		        tempSpriteP->prevSpriteP = 0;
		        
					// initialize drawing fields
		        tempSpriteP->isVisible = true;
              	tempSpriteP->needsToBeDrawn = 1;
                tempSpriteP->needsToBeErased = 0;
 				SW_SET_RECT(tempSpriteP->destFrameRect,0,0,0,0);
				SW_SET_RECT(tempSpriteP->oldFrameRect,0,0,0,0);
				SW_SET_RECT(tempSpriteP->deltaFrameRect,0,0,0,0);
		        tempSpriteP->frameDrawProc = SWStdSpriteDrawProc;
//				tempSpriteP->tileDepth = 1;
 		            		
					// initialize frame fields
				tempSpriteP->curFrameP = NULL;
				tempSpriteP->numFrames = 0;
				tempSpriteP->maxFrames = maxFrames;
				tempSpriteP->frameTimeInterval = -1;
				tempSpriteP->timeOfLastFrameChange = 0;
				tempSpriteP->frameAdvance = 1;
				tempSpriteP->curFrameIndex = 0;
				tempSpriteP->firstFrameIndex = 0;
				tempSpriteP->lastFrameIndex = 0;
				tempSpriteP->frameChangeProc = NULL;

					// initialize movement fields
				tempSpriteP->moveTimeInterval = 0;
				tempSpriteP->timeOfLastMove = 0;
				tempSpriteP->pseudoMoveTimeInterval = -1;
				tempSpriteP->moveTimeAccum = 0;
				tempSpriteP->moveProcHasFired = false;
				SW_SET_RECT(tempSpriteP->moveBoundsRect,0,0,0,0);
				tempSpriteP->spriteMoveProc = SWTempSpriteMoveProc;
				tempSpriteP->realSpriteMoveProc = NULL;

				tempSpriteP->spriteCollideProc = NULL;
				
				tempSpriteP->sharedSurface = NULL;
				tempSpriteP->spriteRemoval = kSWDontRemoveSprite;
				tempSpriteP->frameAdvanceMode = kSWWrapAroundMode;
				
 				tempSpriteP->drawData = SWCreateDrawData();				// DRAW DATA
 				
				tempSpriteP->doFastMovingCheck = false;

				tempSpriteP->drawData->parentSpriteP = tempSpriteP;
				tempSpriteP->drawData->rotationOffsetHoriz = 0.0f;
				tempSpriteP->drawData->rotationOffsetVert = 0.0f;
       			
                tempSpriteP->userData = 0;
                
		        *newSpriteP = tempSpriteP;
		}
		else
			err = kMemoryAllocationError;
    } 
	else 
	{
		err = kMemoryAllocationError;
	} 
	
	return err;
}
