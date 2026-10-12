//---------------------------------------------------------------------------------------
//	Sprite.c
//
//	Portions are copyright: © 1991-94 Tony Myles, All rights reserved worldwide.
//
//	Description:	implementation of the sprites
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

#ifndef __SPRITEFRAME__
#include <SpriteFrame.h>
#endif

#ifndef __BLITKERNEL__
#include <BlitKernel.h>
#endif


//---------------------------------------------------------------------------------------
//	SWRemoveSpriteFromAnimation
//---------------------------------------------------------------------------------------

void  SWRemoveSpriteFromAnimation(
	SpritePtr		spriteP,
	SWBoolean		disposeOfSprite)
{
	SpriteWorldPtr	spriteWorldP;
	
	SW_ASSERT(spriteP != NULL);
	
		// NEW! FIXES CRASHING!
	if (spriteP->spriteRemoval != kSWDontRemoveSprite)
		return;

		// No need to remove it if it's not in a layer, or if it's already been 
		// moved to the deadSpriteLayerP.
	if (spriteP->parentSpriteLayerP != NULL)
	{
		SpriteWorldPtr	spriteWorldP;
		
		spriteWorldP = spriteP->parentSpriteLayerP->parentSpriteWorldP;
		
		if (spriteWorldP != NULL)
		{
			SWSetSpriteFrameProc( spriteP, NULL );
			SWSetSpriteMoveProc( spriteP, NULL );
			
			SWSetSpriteVisible( spriteP, false );	// Invalidates the sprite. Causes it to be erased.
			SWRemoveSprite(spriteP);
			SWAddSprite(spriteWorldP->deadSpriteLayerP, spriteP);
			
			if ( disposeOfSprite )
			{
				spriteP->spriteRemoval = kSWRemoveAndDisposeSprite;
			}
			else
			{
				spriteP->spriteRemoval = kSWRemoveSprite;
			}
		}
		else
		{
				// If the layer this Sprite is in isn't part of a SpriteWorld,
				// SWRemoveSprite can be used.
			SWRemoveSprite(spriteP);
			
			if (disposeOfSprite)
				SWDisposeSprite(&spriteP);
		}
	}
}


//---------------------------------------------------------------------------------------
//	SWDisposeSprite
//---------------------------------------------------------------------------------------

void SWDisposeSprite(
	SpritePtr *deadSpritePP)
{
	SpritePtr	deadSpriteP = *deadSpritePP;
	short 			frame;
	SWBoolean		framesDisposed = false;

	if (deadSpriteP != NULL)
	{

		for (frame = 0; frame < deadSpriteP->numFrames; frame++)
		{
			framesDisposed = SWDisposeFrame(&deadSpriteP->frameArray[frame]);
		}

		if ( framesDisposed )
		{
			if ( deadSpriteP->sharedSurface != NULL )
			{
				SDL_FreeSurface(deadSpriteP->sharedSurface);
				deadSpriteP->sharedSurface = NULL;
			}

		}

		free(deadSpriteP->frameArray);
		deadSpriteP->frameArray = NULL;
		
		
		free(deadSpriteP);

		*deadSpritePP = NULL;	// Set the original pointer to NULL
	}
}


//---------------------------------------------------------------------------------------
//	SWLockSprite
//---------------------------------------------------------------------------------------

void SWLockSprite(
	SpritePtr srcSpriteP)
{
	register long 		curFrame;

	SW_ASSERT(srcSpriteP != NULL);

	for (curFrame = 0; curFrame < srcSpriteP->numFrames; curFrame++)
	{
		SWLockFrame(srcSpriteP->frameArray[curFrame]);
	}
}


//---------------------------------------------------------------------------------------
//	SWUnlockSprite
//---------------------------------------------------------------------------------------

void SWUnlockSprite(
	SpritePtr srcSpriteP)
{
	register long 		curFrame;

	SW_ASSERT(srcSpriteP != NULL);

	for (curFrame = 0; curFrame < srcSpriteP->numFrames; curFrame++)
	{
		SWUnlockFrame(srcSpriteP->frameArray[curFrame]);
	}

}



//---------------------------------------------------------------------------------------
//	SWAddFrame
//---------------------------------------------------------------------------------------

SWError SWAddFrame(
	SpritePtr srcSpriteP,
	FramePtr newFrameP)
{
	SWError err = kNoError;
	
	SW_ASSERT(srcSpriteP != NULL && newFrameP != NULL);

		// don’t exceed maximum number of frames
	if (srcSpriteP->numFrames < srcSpriteP->maxFrames)
	{      
		srcSpriteP->frameArray[srcSpriteP->numFrames] = newFrameP;
		
		if(srcSpriteP->numFrames == 0)
		  srcSpriteP->curFrameP = newFrameP;
		  
			// increment the number of frames
		srcSpriteP->numFrames++;
		newFrameP->useCount++;
	}
	else
	{
		err = kMaxFramesErr;
	}

	SWSetStickyIfError( err );
	return err;
}



//---------------------------------------------------------------------------------------
//	SWInsertFrame
//---------------------------------------------------------------------------------------

SWError SWInsertFrame(
	SpritePtr	srcSpriteP,
	FramePtr	newFrameP,
	long		frameIndex)
{
	register long		index;
	register FramePtr	*frameArray;
	SWError				err = kNoError;

	SW_ASSERT(srcSpriteP != NULL && newFrameP != NULL);

	frameArray = srcSpriteP->frameArray;

		// don’t exceed maximum number of frames
	if (frameIndex < srcSpriteP->maxFrames && frameIndex >= 0 &&
		srcSpriteP->numFrames < srcSpriteP->maxFrames)
	{
			// move frames above frameIndex up
		for (index = srcSpriteP->numFrames; index > frameIndex; index--)
			frameArray[index] = frameArray[index - 1];

			// fill the hole
		frameArray[frameIndex] = newFrameP;

		srcSpriteP->numFrames++;
		newFrameP->useCount++;
	}
	else
	{
		err = kMaxFramesErr;
	}

	SWSetStickyIfError( err );
	return err;
}


//---------------------------------------------------------------------------------------
//	SWRemoveFrame
//---------------------------------------------------------------------------------------

void SWRemoveFrame(
	SpritePtr srcSpriteP,
	FramePtr oldFrameP)
{
	register long curFrame;
	register FramePtr *frameArray;

	SW_ASSERT(srcSpriteP != NULL && oldFrameP != NULL);

	curFrame = srcSpriteP->numFrames;
	frameArray = srcSpriteP->frameArray;

		// find the frame to be removed
	while (curFrame--)
	{
		if (frameArray[curFrame] == oldFrameP)
		{
			srcSpriteP->numFrames--;
			oldFrameP->useCount--;

				// move the rest of the frames down
			while (curFrame <= (srcSpriteP->numFrames - 1L))
			{
				frameArray[curFrame] = frameArray[curFrame + 1L];
				curFrame++;
			}

			break;
		}
	}
}


//---------------------------------------------------------------------------------------
//	SWGetNextFrame
//---------------------------------------------------------------------------------------

FramePtr SWGetNextFrame(
	SpritePtr spriteP,
	FramePtr curFrameP)
{
	SWBoolean	frame;

	SW_ASSERT(spriteP != NULL);

	if ( curFrameP == NULL )
		return spriteP->frameArray[0];
	else
	{
		for ( frame = 0; frame < spriteP->numFrames - 2; frame++ )
		{
			if ( curFrameP == spriteP->frameArray[frame] )
			{
				return spriteP->frameArray[frame+1];
			}
		}
		return NULL;
	}
}

#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------------------
//	SWSetSpriteDrawProc
//---------------------------------------------------------------------------------------

SWError SWSetSpriteDrawProc(
	SpritePtr srcSpriteP,
	DrawProcPtr drawProc)
{
	SWError		err = kNoError;

	srcSpriteP->frameDrawProc = drawProc;

	return err;
}



//---------------------------------------------------------------------------------------
//	SWStdSpriteDrawProc - for drawing sprites
//---------------------------------------------------------------------------------------

void SWStdSpriteDrawProc(
	FramePtr srcFrameP,
	FramePtr dstFrameP,
	SWRect* srcSWRect,
	SWRect* dstSWRect)
{
	SWError err = kNoError;
	SDL_Rect srcSDLRect;
	SDL_Rect dstSDLRect;
	Uint32 oldSrcAlpha = 0;

	//SW_ASSERT(srcFrameP->isFrameLocked && dstFrameP->isFrameLocked);
	
	SW_ASSERT(gSWCurrentElementDrawData != NULL);

#ifdef HAVE_OPENGL
	if (gSWCurrentSpriteWorld && gSWCurrentSpriteWorld->useOpenGL)
	{
		struct BK_GL_func *gl = BKLoadGLfunctions();
		struct BK_GL_caps *caps = BKLoadGLcapabilities();

		gl->glTexParameteri(caps->extension, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gl->glTexParameteri(caps->extension, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			// Handle translucency here
		glColor4us(gSWCurrentElementDrawData->colorRed, gSWCurrentElementDrawData->colorGreen,
					gSWCurrentElementDrawData->colorBlue, gSWCurrentElementDrawData->translucencyLevel);
		
		gl->glMatrixMode(GL_MODELVIEW);
		gl->glPushMatrix();
			gl->glLoadIdentity();
			gl->glTranslatef(dstSWRect->left,dstSWRect->top,0.0f);
		//	gl->glRotatef(0.0f,0.0f,0.0f,0.0f);
		
			if (gSWCurrentElementDrawData->scaledHeightFactor != 1 || gSWCurrentElementDrawData->scaledWidthFactor != 1)
				gl->glScalef(gSWCurrentElementDrawData->scaledWidthFactor, gSWCurrentElementDrawData->scaledHeightFactor, 1.0f);
			
			if ( BKSurfaceNeedsBlend(srcFrameP->glSurfaceP) )
			{
					// Default values are GL_SRC_ALPHA and GL_ONE_MINUS_SRC_ALPHA
				gl->glEnable(GL_BLEND);
				gl->glBlendFunc(gSWCurrentElementDrawData->srcBlendMode, gSWCurrentElementDrawData->dstBlendMode);
			}
			
			BKBlitGLSurface((BK_GL_Surface *) srcFrameP->glSurfaceP);	// Pass in the *source* frame only. The "dest frame" is the current GL context.
		gl->glPopMatrix();

		if (1)	// draw a red outline, for debugging
		{
			gl->glColor4ub(255,0,0, SDL_ALPHA_OPAQUE);
			gl->glBegin(GL_LINE_LOOP);
				gl->glVertex2i(dstSWRect->left, dstSWRect->top);
				gl->glVertex2i(dstSWRect->right, dstSWRect->top);
				gl->glVertex2i(dstSWRect->right, dstSWRect->bottom);
				gl->glVertex2i(dstSWRect->left, dstSWRect->bottom);
			gl->glEnd();
		}
	}
	else
#endif // OpenGL
	{
		
		SW_CONVERT_SW_TO_SDL_RECT( (*srcSWRect), srcSDLRect );
		SW_CONVERT_SW_TO_SDL_RECT( (*dstSWRect), dstSDLRect );
		
		if ( gSWCurrentElementDrawData != NULL && gSWCurrentElementDrawData->translucencyLevel != 0xFFFF )
		{
			oldSrcAlpha = srcFrameP->frameSurfaceP->flags & SDL_SRCALPHA;
			if( SDL_SetAlpha( srcFrameP->frameSurfaceP, SDL_SRCALPHA, gSWCurrentElementDrawData->translucencyLevel >> 8 ) == -1 )
				err = kSDLSetAlpha;
		}
			
		if ( err == kNoError )
			err = SDL_BlitSurface(
				srcFrameP->frameSurfaceP,
				&srcSDLRect,
				dstFrameP->frameSurfaceP,
				&dstSDLRect );

		if( gSWCurrentElementDrawData != NULL && gSWCurrentElementDrawData->translucencyLevel != 0xFFFF )
			SDL_SetAlpha( srcFrameP->frameSurfaceP, oldSrcAlpha, SDL_ALPHA_OPAQUE );
		
	} // OpenGL
	
	SWSetStickyIfError( err );
}


//---------------------------------------------------------------------------------------
//	SWSolidColorSpriteDrawProc
//---------------------------------------------------------------------------------------

void SWSolidColorSpriteDrawProc(
	FramePtr srcFrameP,
	FramePtr dstFrameP,
	SWRect* srcSWRect,
	SWRect* dstSWRect)
{
	SW_ASSERT(gSWCurrentElementDrawData != NULL);
	
#ifdef HAVE_OPENGL
	if (gSWCurrentSpriteWorld && gSWCurrentSpriteWorld->useOpenGL)
	{
		struct BK_GL_func *gl = BKLoadGLfunctions();
		struct BK_GL_caps *caps = BKLoadGLcapabilities();
		
			// Necessary?
		gl->glTexParameteri(caps->extension, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gl->glTexParameteri(caps->extension, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
			// Handle translucency here
		glColor4us(gSWCurrentElementDrawData->colorRed, gSWCurrentElementDrawData->colorGreen,
					gSWCurrentElementDrawData->colorBlue, gSWCurrentElementDrawData->translucencyLevel);
		
			// Enable translucency
			// Default values are GL_SRC_ALPHA and GL_ONE_MINUS_SRC_ALPHA
		gl->glEnable(GL_BLEND);
		gl->glBlendFunc(gSWCurrentElementDrawData->srcBlendMode, gSWCurrentElementDrawData->dstBlendMode);
		
		glBegin(GL_QUADS);	// Quad is a rectangle
			gl->glVertex2i(dstSWRect->left, dstSWRect->top);
			gl->glVertex2i(dstSWRect->right, dstSWRect->top);
			gl->glVertex2i(dstSWRect->right, dstSWRect->bottom);
			gl->glVertex2i(dstSWRect->left, dstSWRect->bottom);
		gl->glEnd();
		
		gl->glDisable(GL_BLEND);
	}
#endif
}


//---------------------------------------------------------------------------------------
//  SWTempSpriteMoveProc - this is installed as the Sprite's MoveProc when the Sprite is
//	first created. When this is called, it replaces itself with the real MoveProc. This
//	ensures that the Sprite is drawn in its original position before it is moved by
//	SWProcessSpriteWorld.
//---------------------------------------------------------------------------------------

void SWTempSpriteMoveProc(SpritePtr srcSpriteP)
{
		// Change the moveProc to the real one. This TempSpriteMoveProc
		// will never be called for this Sprite again.
	srcSpriteP->spriteMoveProc = srcSpriteP->realSpriteMoveProc;

		// If the Sprite has already been drawn, call its real MoveProc.
		// (We can go ahead and move it since it has been drawn in its original location.)
	if (srcSpriteP->needsToBeDrawn == false)
	{
		if (srcSpriteP->realSpriteMoveProc != NULL)
			(*srcSpriteP->realSpriteMoveProc)(srcSpriteP);
	}
}

#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------------------
//	SWSetCurrentFrame
//---------------------------------------------------------------------------------------

SWError SWSetCurrentFrame(
	SpritePtr srcSpriteP,
	FramePtr newFrameP)
{
	short		frameIndex = 0;
	SWBoolean	foundFrame = false;
	SWError		err = kNoError;

	SW_ASSERT(srcSpriteP != NULL && newFrameP != NULL);

	for (frameIndex = 0; frameIndex < srcSpriteP->numFrames; frameIndex++)
	{
		if (srcSpriteP->frameArray[frameIndex] == newFrameP)
		{
			SWSetCurrentFrameIndex(srcSpriteP, frameIndex);
			foundFrame = true;
			break;
		}
	}

	if (foundFrame == false)
		err = kBadParameterErr;

	SWSetStickyIfError( err );
	return err;
}


//---------------------------------------------------------------------------------------
//	SWSetCurrentFrameIndex
//---------------------------------------------------------------------------------------

SWError SWSetCurrentFrameIndex(
	SpritePtr srcSpriteP,
	long frameIndex)
{
	SWError err = kNoError;
	register FramePtr newFrameP;
	short horizOffset, vertOffset;

	SW_ASSERT(srcSpriteP != NULL);
    //SW_ASSERT(frameIndex >= 0);

	if (frameIndex < srcSpriteP->numFrames)
	{
		newFrameP = srcSpriteP->frameArray[frameIndex];

			// If this is a brand new sprite, it won't have a current frame.
			// (This is used when calculating the hotSpot offset from old to new frame.)
		if (srcSpriteP->curFrameP == NULL)
			srcSpriteP->curFrameP = newFrameP;

		horizOffset = (srcSpriteP->destFrameRect.left - newFrameP->frameRect.left) +
					(srcSpriteP->curFrameP->hotSpotH - newFrameP->hotSpotH);
		vertOffset = (srcSpriteP->destFrameRect.top - newFrameP->frameRect.top) +
					(srcSpriteP->curFrameP->hotSpotV - newFrameP->hotSpotV);

		srcSpriteP->curFrameP = newFrameP;
		srcSpriteP->destFrameRect = newFrameP->frameRect;

		if (srcSpriteP->drawData->scaledWidth > 0)
		{
			srcSpriteP->destFrameRect.right = srcSpriteP->destFrameRect.left +
												srcSpriteP->drawData->scaledWidth;
												
			srcSpriteP->drawData->scaledWidthFactor = (float)SW_RECT_WIDTH(srcSpriteP->destFrameRect) / SW_RECT_WIDTH(newFrameP->frameRect);
		}
		if (srcSpriteP->drawData->scaledHeight > 0)
		{
			srcSpriteP->destFrameRect.bottom = srcSpriteP->destFrameRect.top +
												srcSpriteP->drawData->scaledHeight;
												
			srcSpriteP->drawData->scaledHeightFactor = (float)SW_RECT_HEIGHT(srcSpriteP->destFrameRect) / SW_RECT_HEIGHT(newFrameP->frameRect);
		}
		
		srcSpriteP->drawData->frameHotSpotH = srcSpriteP->curFrameP->hotSpotH;
		srcSpriteP->drawData->frameHotSpotV = srcSpriteP->curFrameP->hotSpotV;

		srcSpriteP->destFrameRect.left += horizOffset;
		srcSpriteP->destFrameRect.right += horizOffset;
		srcSpriteP->destFrameRect.top += vertOffset;
		srcSpriteP->destFrameRect.bottom += vertOffset;

		srcSpriteP->curFrameIndex = frameIndex;

		srcSpriteP->needsToBeDrawn = true;
	}
	else
	{
		err = kInvalidFramesIndexErr;
	}
	
	return err;
}


//---------------------------------------------------------------------------------------
//	SWSetSpriteCollideProc
//---------------------------------------------------------------------------------------

void SWSetSpriteCollideProc(
	SpritePtr srcSpriteP,
	CollideProcPtr collideProc)
{
	SW_ASSERT(srcSpriteP != NULL);
	srcSpriteP->spriteCollideProc = collideProc;
}


//---------------------------------------------------------------------------------------
//	SWSetSpriteColor
//---------------------------------------------------------------------------------------
/*
void SWSetSpriteColor(
	SpritePtr srcSpriteP,
	SDL_Color *color)
{
	srcSpriteP->drawData->color = *color;
}*/


//---------------------------------------------------------------------------------------
//	SWSetSpriteFrameAdvance (default value for new Sprites is 1)
//---------------------------------------------------------------------------------------

void SWSetSpriteFrameAdvance(
	SpritePtr srcSpriteP,
	long frameAdvance)
{
	SW_ASSERT(srcSpriteP != NULL);
	srcSpriteP->frameAdvance = frameAdvance;
}


//---------------------------------------------------------------------------------------
//	SWSetSpriteFrameAdvanceMode
//---------------------------------------------------------------------------------------

void SWSetSpriteFrameAdvanceMode(
	SpritePtr srcSpriteP,
	AdvanceType advanceMode)
{
	SW_ASSERT(srcSpriteP != NULL);
	srcSpriteP->frameAdvanceMode = advanceMode;
}


//---------------------------------------------------------------------------------------
//	SWSetSpriteFrameRange
//---------------------------------------------------------------------------------------

void SWSetSpriteFrameRange(
	SpritePtr srcSpriteP,
	long firstFrameIndex,
	long lastFrameIndex)
{
	SW_ASSERT(srcSpriteP != NULL);

	if ( firstFrameIndex < 0 )
		firstFrameIndex = 0;

	if ( lastFrameIndex > srcSpriteP->maxFrames - 1 )
		lastFrameIndex = srcSpriteP->maxFrames - 1;

	srcSpriteP->firstFrameIndex = firstFrameIndex;
	srcSpriteP->lastFrameIndex = lastFrameIndex;

		// Make sure the sprite's curFrameIndex is within the new frame range
	if (srcSpriteP->curFrameIndex < firstFrameIndex)
		SWSetCurrentFrameIndex(srcSpriteP, firstFrameIndex);
	else if (srcSpriteP->curFrameIndex > lastFrameIndex)
		SWSetCurrentFrameIndex(srcSpriteP, lastFrameIndex);
}


//---------------------------------------------------------------------------------------
//	SWSetSpriteFrameTime - pass -1 to never change frames (default state for new Sprites),
//	0 to advance as fast as possible, or a millisecond time interval.
//---------------------------------------------------------------------------------------

void SWSetSpriteFrameTime(
	SpritePtr srcSpriteP,
	long timeInterval)
{
	SpriteWorldPtr	parentSpriteWorldP = NULL;
	SW_ASSERT(srcSpriteP != NULL);
	
	if (timeInterval != srcSpriteP->frameTimeInterval )
	{
		srcSpriteP->frameTimeInterval = timeInterval;
		
		if ( srcSpriteP->parentSpriteLayerP != NULL)
			parentSpriteWorldP = srcSpriteP->parentSpriteLayerP->parentSpriteWorldP; // Note: might be NULL
		if (parentSpriteWorldP == NULL)
			parentSpriteWorldP = gSWCurrentSpriteWorld;	// Might also be NULL
			
			// Reset the sprite's frameChange "timer".
		if (parentSpriteWorldP != NULL)
			srcSpriteP->timeOfLastFrameChange = parentSpriteWorldP->runningTimeCount;
	}
}


//---------------------------------------------------------------------------------------
//	SWSetSpriteFrameProc
//---------------------------------------------------------------------------------------

void SWSetSpriteFrameProc(
	SpritePtr srcSpriteP,
	FrameProcPtr frameProc)
{
	SW_ASSERT(srcSpriteP != NULL);
	srcSpriteP->frameChangeProc = frameProc;
}


//---------------------------------------------------------------------------------------
//	SWSetSpriteLocation
//---------------------------------------------------------------------------------------

void SWSetSpriteLocation(
	SpritePtr srcSpriteP,
	SWMovementType floatHorizLoc,
	SWMovementType floatVertLoc)
{
	short	horizLoc, vertLoc;

	SW_ASSERT(srcSpriteP != NULL);

	srcSpriteP->drawData->horizLoc = floatHorizLoc;
	srcSpriteP->drawData->vertLoc = floatVertLoc;

	horizLoc = (short) floatHorizLoc - srcSpriteP->drawData->frameHotSpotH;
	vertLoc = (short) floatVertLoc - srcSpriteP->drawData->frameHotSpotV;

	srcSpriteP->destFrameRect.bottom = vertLoc +
		(srcSpriteP->destFrameRect.bottom - srcSpriteP->destFrameRect.top);
	srcSpriteP->destFrameRect.right = horizLoc +
		(srcSpriteP->destFrameRect.right - srcSpriteP->destFrameRect.left);
	srcSpriteP->destFrameRect.top = vertLoc;
	srcSpriteP->destFrameRect.left = horizLoc;

	srcSpriteP->deltaFrameRect = srcSpriteP->destFrameRect;
	srcSpriteP->oldFrameRect = srcSpriteP->destFrameRect;

	srcSpriteP->needsToBeDrawn = true;
}


//---------------------------------------------------------------------------------------
//	SWSetSpriteMoveBounds
//---------------------------------------------------------------------------------------

void SWSetSpriteMoveBounds(
	SpritePtr srcSpriteP,
	SWRect* moveBoundsRect)
{
	SW_ASSERT(srcSpriteP != NULL);
	srcSpriteP->moveBoundsRect = *moveBoundsRect;
}


//---------------------------------------------------------------------------------------
//	SWSetSpriteMoveDelta
//---------------------------------------------------------------------------------------

void SWSetSpriteMoveDelta(
	SpritePtr srcSpriteP,
	SWMovementType horizDelta,
	SWMovementType vertDelta)
{
	SW_ASSERT(srcSpriteP != NULL);

		// set sprite’s velocity
	srcSpriteP->drawData->horizMoveDelta = horizDelta;
	srcSpriteP->drawData->vertMoveDelta = vertDelta;
}


//---------------------------------------------------------------------------------------
//	SWSetSpriteMoveTime
//---------------------------------------------------------------------------------------

void SWSetSpriteMoveTime(
	SpritePtr srcSpriteP,
	long timeInterval)
{
	SW_ASSERT(srcSpriteP != NULL);
	srcSpriteP->moveTimeInterval = timeInterval;
}


///--------------------------------------------------------------------------------------
//	SWGetSpriteMoveTime
///--------------------------------------------------------------------------------------

SW_FUNC long SWGetSpriteMoveTime(SpritePtr srcSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);
	return srcSpriteP->moveTimeInterval;
}


//---------------------------------------------------------------------------------------
//	SWSetSpriteMoveProc
//---------------------------------------------------------------------------------------

void SWSetSpriteMoveProc(
	SpritePtr srcSpriteP,
	MoveProcPtr moveProc)
{
	SW_ASSERT(srcSpriteP != NULL);

	srcSpriteP->realSpriteMoveProc = moveProc;

		// Set the current MoveProc if it isn't SWTempSpriteMoveProc.
	if (srcSpriteP->spriteMoveProc != SWTempSpriteMoveProc)
		srcSpriteP->spriteMoveProc = moveProc;
}


//---------------------------------------------------------------------------------------
//	SWSetSpritePseudoMoveTime
//---------------------------------------------------------------------------------------

void SWSetSpritePseudoMoveTime(
	SpritePtr srcSpriteP,
	long timeInterval)
{
	SW_ASSERT(srcSpriteP != NULL);

		// Note: a timeInterval is NOT the same as a FPS value!
	srcSpriteP->pseudoMoveTimeInterval = timeInterval;
//	srcSpriteP->pseudoMoveFPS = SW_ONE_MILLION / timeInterval;
}


//---------------------------------------------------------------------------------------
//	SWSetSpriteVisible
//---------------------------------------------------------------------------------------

void SWSetSpriteVisible(
	SpritePtr srcSpriteP,
	SWBoolean isVisible)
{
	SW_ASSERT(srcSpriteP != NULL);
	
	srcSpriteP->isVisible = isVisible;
	srcSpriteP->needsToBeDrawn = true;
	srcSpriteP->needsToBeErased = !isVisible;
}

//---------------------------------------------------------------------------------------
//	SWSetSpriteTranslucency - pass 0xFFFF for opaque, 0 for invisible.
//---------------------------------------------------------------------------------------

void SWSetSpriteTranslucency(
	SpritePtr srcSpriteP,
	unsigned long translucency)
{
	SW_ASSERT(srcSpriteP != NULL);	
	srcSpriteP->drawData->translucencyLevel = translucency;
}


//---------------------------------------------------------------------------------------
//	SWSetSpriteTranslucencyLevel - old function name for compatibility. 0xFFFF for fully opaque
//---------------------------------------------------------------------------------------

void SWSetSpriteTranslucencyLevel(
	SpritePtr srcSpriteP,
	unsigned long level)
{
	SW_ASSERT(srcSpriteP != NULL);	
	srcSpriteP->drawData->translucencyLevel = level;
}


//---------------------------------------------------------------------------------------
//	SWSetSpriteColor - for OpenGL real-time color blending, or shadow effect
//	Default values for new sprites are 0xFFFF for each color. (i.e. white)
//---------------------------------------------------------------------------------------

void SWSetSpriteColor(
	SpritePtr srcSpriteP,
	unsigned short red,
	unsigned short green,
	unsigned short blue)
{
	SW_ASSERT(srcSpriteP != NULL);	
	
	srcSpriteP->drawData->colorRed = red;
	srcSpriteP->drawData->colorGreen = green;
	srcSpriteP->drawData->colorBlue = blue;
}


//---------------------------------------------------------------------------------------
//	SWSetSpriteBlendMode - OpenGL blending mode, so a sprite can have a "lighting" effect or more
//	Default values are: GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
//  To make it a "lighting" blitter, use GL_ONE as the destBlendMode
//---------------------------------------------------------------------------------------

void SWSetSpriteBlendMode(
	SpritePtr srcSpriteP,
	unsigned int srcBlendMode,
	unsigned int dstBlendMode)
{
	SW_ASSERT(srcSpriteP != NULL);	
	
	srcSpriteP->drawData->srcBlendMode = srcBlendMode;
	srcSpriteP->drawData->dstBlendMode = dstBlendMode;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteScaledSize - for compatibility with the old API
///--------------------------------------------------------------------------------------

void SWSetSpriteScaledSize( SpritePtr newSpriteP, short width, short height )
{
	SWSetSpriteScaledWidth( newSpriteP, width);
	SWSetSpriteScaledHeight( newSpriteP, height);
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteScaledWidth
///--------------------------------------------------------------------------------------

SW_FUNC void SWSetSpriteScaledWidth(
	SpritePtr srcSpriteP,
	short width)
{
	SW_ASSERT(srcSpriteP != NULL);

	srcSpriteP->drawData->scaledWidth = width;

		// make sure the destRect is correct
	SWSetCurrentFrameIndex(srcSpriteP, srcSpriteP->curFrameIndex);
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteScaledHeight
///--------------------------------------------------------------------------------------

SW_FUNC void SWSetSpriteScaledHeight(
	SpritePtr srcSpriteP,
	short height)
{
	SW_ASSERT(srcSpriteP != NULL);

	srcSpriteP->drawData->scaledHeight = height;

		// make sure the destRect is correct
	SWSetCurrentFrameIndex(srcSpriteP, srcSpriteP->curFrameIndex);
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteScaledWidthFactor - NEW to SpriteWorld X. Default is 1.0. So 2.0 is double width, 0.5 is half width
///--------------------------------------------------------------------------------------

SW_FUNC void SWSetSpriteScaledWidthFactor(
	SpritePtr srcSpriteP,
	float widthFactor)
{
	SW_ASSERT(srcSpriteP != NULL);
	
	SWSetSpriteScaledWidth( srcSpriteP, SWGetSpriteWidth(srcSpriteP) * widthFactor);

//	srcSpriteP->drawData->scaledWidthFactor = widthFactor;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteScaledHeightFactor
///--------------------------------------------------------------------------------------

SW_FUNC void SWSetSpriteScaledHeightFactor(
	SpritePtr srcSpriteP,
	float heightFactor)
{
	SW_ASSERT(srcSpriteP != NULL);
	
	SWSetSpriteScaledHeight( srcSpriteP, SWGetSpriteHeight(srcSpriteP) * heightFactor);

//	srcSpriteP->drawData->scaledHeightFactor = heightFactor;
}



//---------------------------------------------------------------------------------------
//	SWSetSpriteFastMovingCheck - set whether we need to check this sprite to see
// if we should deal with its oldSWRects and destSWRects separately when animating, or if
// should just use the deltaSWRects.  If no checking is done then deltaSWRects will always be
// used.  Setting this to true overides SWSetGlobalFastMovingSpritesChecks() to false
//---------------------------------------------------------------------------------------

void SWSetSpriteFastMovingCheck(
	SpritePtr srcSpriteP,
	SWBoolean makeChecks)
{
	SW_ASSERT(srcSpriteP != NULL);
	srcSpriteP->doFastMovingCheck = makeChecks;
}


//---------------------------------------------------------------------------------------
//	SWInvalidateSprite
//---------------------------------------------------------------------------------------

void SWInvalidateSprite(
	SpritePtr srcSpriteP)
{
	srcSpriteP->needsToBeDrawn = true;
}

#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------------------
//	SWGetSpriteFrameTime - is -1 to never change frames (default state for new Sprites),
//	0 to advance as fast as possible, or a millisecond time interval.
//---------------------------------------------------------------------------------------

long SWGetSpriteFrameTime(SpritePtr srcSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);

	return srcSpriteP->frameTimeInterval;
}


//---------------------------------------------------------------------------------------
//	SWGetSpriteVertLoc
//---------------------------------------------------------------------------------------

SWMovementType SWGetSpriteVertLoc(
	SpritePtr srcSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);
	return srcSpriteP->drawData->vertLoc;
}


//---------------------------------------------------------------------------------------
//	SWGetSpriteHorizLoc
//---------------------------------------------------------------------------------------

SWMovementType SWGetSpriteHorizLoc(
	SpritePtr srcSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);
	return srcSpriteP->drawData->horizLoc;
}


///--------------------------------------------------------------------------------------
//	SWGetSpriteHorizMoveDelta
///--------------------------------------------------------------------------------------

SWMovementType SWGetSpriteHorizMoveDelta(SpritePtr srcSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);
	
	return srcSpriteP->drawData->horizMoveDelta;
}


///--------------------------------------------------------------------------------------
//	SWGetSpriteVertMoveDelta
///--------------------------------------------------------------------------------------

SWMovementType SWGetSpriteVertMoveDelta(SpritePtr srcSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);
	
	return srcSpriteP->drawData->vertMoveDelta;
}



//---------------------------------------------------------------------------------------
//	SWGetSpriteWidth
//---------------------------------------------------------------------------------------

short SWGetSpriteWidth(
	SpritePtr srcSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);

	return srcSpriteP->destFrameRect.right - srcSpriteP->destFrameRect.left;
}


//---------------------------------------------------------------------------------------
//	SWGetSpriteHeight
//---------------------------------------------------------------------------------------

short SWGetSpriteHeight(
	SpritePtr srcSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);

	return srcSpriteP->destFrameRect.bottom - srcSpriteP->destFrameRect.top;
}


///--------------------------------------------------------------------------------------
//	SWGetSpriteTranslucencyLevel
///--------------------------------------------------------------------------------------

SW_FUNC unsigned long SWGetSpriteTranslucencyLevel(SpritePtr srcSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);
	
	return srcSpriteP->drawData->translucencyLevel;
}

#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------------------
//	SWMoveSprite
//---------------------------------------------------------------------------------------

void SWMoveSprite(
	SpritePtr srcSpriteP,
	SWMovementType floatHorizLoc,
	SWMovementType floatVertLoc)
{
	short	horizLoc, vertLoc;

	SW_ASSERT(srcSpriteP != NULL);

	srcSpriteP->drawData->horizLoc = floatHorizLoc;
	srcSpriteP->drawData->vertLoc = floatVertLoc;

	horizLoc = (short) floatHorizLoc - srcSpriteP->drawData->frameHotSpotH;
	vertLoc = (short) floatVertLoc - srcSpriteP->drawData->frameHotSpotV;

	if ((horizLoc != srcSpriteP->destFrameRect.left) ||
		(vertLoc != srcSpriteP->destFrameRect.top) )
	{
		srcSpriteP->destFrameRect.bottom = vertLoc +
			(srcSpriteP->destFrameRect.bottom - srcSpriteP->destFrameRect.top);
		srcSpriteP->destFrameRect.right = horizLoc +
			(srcSpriteP->destFrameRect.right - srcSpriteP->destFrameRect.left);
		srcSpriteP->destFrameRect.top = vertLoc;
		srcSpriteP->destFrameRect.left = horizLoc;

		srcSpriteP->needsToBeDrawn = true;
	}
}


//---------------------------------------------------------------------------------------
//	SWOffsetSprite
//---------------------------------------------------------------------------------------

void SWOffsetSprite(
	SpritePtr srcSpriteP,
	SWMovementType floatHorizOffset,
	SWMovementType floatVertOffset)
{
	SW_ASSERT(srcSpriteP != NULL);

	if ((floatHorizOffset != 0) || (floatVertOffset != 0))
	{
		srcSpriteP->drawData->horizLoc += floatHorizOffset;
		srcSpriteP->drawData->vertLoc += floatVertOffset;

		SWMoveSprite(srcSpriteP, srcSpriteP->drawData->horizLoc, srcSpriteP->drawData->vertLoc);
	}
}


//---------------------------------------------------------------------------------------
//	SWOffsetSpriteBasedOnTime - Use this, along with a maxFPS of 0, to let computers
//  with a lot of horsepower make your animation much smoother than it would be otherwise.
//---------------------------------------------------------------------------------------

void SWOffsetSpriteBasedOnTime(
	SpritePtr theSpriteP,
	SWMovementType horizOffset,
	SWMovementType vertOffset)
{
	SpriteWorldPtr	spriteWorldP = NULL;
	double			vertMoveDelta, horizMoveDelta;
	double			frameRate;

	SW_ASSERT(theSpriteP != NULL);

	if (theSpriteP->parentSpriteLayerP != NULL)
		spriteWorldP = theSpriteP->parentSpriteLayerP->parentSpriteWorldP;

	if (spriteWorldP != NULL)
	{
		if (theSpriteP->pseudoMoveTimeInterval > 0)
			frameRate = 1000.0 / theSpriteP->pseudoMoveTimeInterval;
		else
			frameRate = spriteWorldP->pseudoFPS;

		horizMoveDelta = SWConvertOffsetBasedOnTime(spriteWorldP, horizOffset, frameRate);
		vertMoveDelta = SWConvertOffsetBasedOnTime(spriteWorldP, vertOffset, frameRate);

		SWOffsetSprite(theSpriteP, (float)horizMoveDelta, (float)vertMoveDelta);
	}
}


//---------------------------------------------------------------------------------------
//	SWConvertOffsetBasedOnTime - Used for time-based animations.
//	framesPerSecond is the "pseudo" speed of this object.
//---------------------------------------------------------------------------------------

double SWConvertOffsetBasedOnTime(
	SpriteWorldPtr spriteWorldP,
	double theOffset,
	double framesPerSecond)
{
	double	timePassed = spriteWorldP->millisecsSinceLastFrame;

	return (theOffset * timePassed * framesPerSecond) * (1.0 / 1000.0);
}


//---------------------------------------------------------------------------------------
//	SWResetSpriteFrameTimer - ensures that the full frameTime delay passes between the
//  time that this function is called, and the Sprite's frameProc is called again.
//---------------------------------------------------------------------------------------

void SWResetSpriteFrameTimer(
	SpriteWorldPtr spriteWorldP,
	SpritePtr srcSpriteP)
{
	srcSpriteP->timeOfLastFrameChange = spriteWorldP->runningTimeCount;
}


//---------------------------------------------------------------------------------------
//	SWHasMoveProcFired
//---------------------------------------------------------------------------------------

SWBoolean SWHasMoveProcFired(
	SpritePtr srcSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);

	return srcSpriteP->moveProcHasFired;
}


//---------------------------------------------------------------------------------------
//	SWBounceSprite
//---------------------------------------------------------------------------------------

SWBoolean SWBounceSprite(
	SpritePtr srcSpriteP)
{
	short	horizOffset, vertOffset;

	SW_ASSERT(srcSpriteP != NULL);

		// Hit left side or right side
	if (srcSpriteP->destFrameRect.left < srcSpriteP->moveBoundsRect.left &&
		(srcSpriteP->drawData->horizMoveDelta < 0))
	{
		srcSpriteP->drawData->horizMoveDelta = -srcSpriteP->drawData->horizMoveDelta;
		horizOffset = (srcSpriteP->moveBoundsRect.left - srcSpriteP->destFrameRect.left) * 2;
	}
	else if (srcSpriteP->destFrameRect.right > srcSpriteP->moveBoundsRect.right &&
			 (srcSpriteP->drawData->horizMoveDelta > 0))
	{
		srcSpriteP->drawData->horizMoveDelta = -srcSpriteP->drawData->horizMoveDelta;
		horizOffset = (srcSpriteP->moveBoundsRect.right - srcSpriteP->destFrameRect.right) * 2;
	}
	else
		horizOffset = 0;


		// Hit top or bottom
	if (srcSpriteP->destFrameRect.top < srcSpriteP->moveBoundsRect.top &&
		(srcSpriteP->drawData->vertMoveDelta < 0))
	{
		srcSpriteP->drawData->vertMoveDelta = -srcSpriteP->drawData->vertMoveDelta;
		vertOffset = (srcSpriteP->moveBoundsRect.top - srcSpriteP->destFrameRect.top) * 2;
	}
	else if (srcSpriteP->destFrameRect.bottom > srcSpriteP->moveBoundsRect.bottom &&
		  	 (srcSpriteP->drawData->vertMoveDelta > 0))
	{
		srcSpriteP->drawData->vertMoveDelta = -srcSpriteP->drawData->vertMoveDelta;
		vertOffset = (srcSpriteP->moveBoundsRect.bottom - srcSpriteP->destFrameRect.bottom) * 2;
	}
	else
		vertOffset = 0;


		// Bounce the sprite if it had a collision with the wall
	if ((horizOffset != 0) || (vertOffset != 0))
	{
		SWOffsetSprite(srcSpriteP, horizOffset, vertOffset);
		return true;
	}
	else
	{
		return false;
	}
}


//---------------------------------------------------------------------------------------
//	SWWrapSprite
//---------------------------------------------------------------------------------------

SWBoolean SWWrapSprite(
	SpritePtr srcSpriteP)
{
	SWMovementType	horizOffset, vertOffset;

	SW_ASSERT(srcSpriteP != NULL);

		// Wrap to left or right
	if (srcSpriteP->oldFrameRect.left > srcSpriteP->moveBoundsRect.right &&
		srcSpriteP->destFrameRect.left > srcSpriteP->oldFrameRect.left)
	{
		horizOffset = - ((srcSpriteP->moveBoundsRect.right -
						srcSpriteP->moveBoundsRect.left) +
						(srcSpriteP->destFrameRect.right -
						srcSpriteP->destFrameRect.left) + srcSpriteP->drawData->horizMoveDelta);
	}
	else if (srcSpriteP->oldFrameRect.right < srcSpriteP->moveBoundsRect.left &&
			 srcSpriteP->destFrameRect.left < srcSpriteP->oldFrameRect.left)
	{
		horizOffset = (srcSpriteP->moveBoundsRect.right -
						srcSpriteP->moveBoundsRect.left) +
						(srcSpriteP->destFrameRect.right -
						srcSpriteP->destFrameRect.left) - srcSpriteP->drawData->horizMoveDelta;
	}
	else
		horizOffset = 0;


		// Wrap to top or bottom
	if (srcSpriteP->oldFrameRect.top > srcSpriteP->moveBoundsRect.bottom &&
		srcSpriteP->destFrameRect.top > srcSpriteP->oldFrameRect.top)
	{
		vertOffset = -((srcSpriteP->moveBoundsRect.bottom -
							srcSpriteP->moveBoundsRect.top) +
							(srcSpriteP->destFrameRect.bottom -
							srcSpriteP->destFrameRect.top) + srcSpriteP->drawData->vertMoveDelta);
	}
	else if (srcSpriteP->oldFrameRect.bottom < srcSpriteP->moveBoundsRect.top &&
			 srcSpriteP->destFrameRect.top < srcSpriteP->oldFrameRect.top)
	{
		vertOffset = (srcSpriteP->moveBoundsRect.bottom -
							srcSpriteP->moveBoundsRect.top) +
							(srcSpriteP->destFrameRect.bottom -
							srcSpriteP->destFrameRect.top) - srcSpriteP->drawData->vertMoveDelta;
	}
	else
		vertOffset = 0;


		// Wrap sprite, and set delta and old rects to current rect
	if ((horizOffset != 0) || (vertOffset != 0))
	{
		SWOffsetSprite(srcSpriteP, horizOffset, vertOffset);

		srcSpriteP->deltaFrameRect = srcSpriteP->destFrameRect;
		srcSpriteP->oldFrameRect = srcSpriteP->destFrameRect;

		return true;
	}
	else
	{
		return false;
	}
}

#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------------------
//	SWInPixelMask - check whether a point is within the sprite mask
//---------------------------------------------------------------------------------------

SWBoolean SWInPixelMask(
	SpritePtr spriteP,
	int testPointH, int testPointV)
{
 	SW_ASSERT(spriteP != NULL);
 	SW_ASSERT(spriteP->curFrameP != NULL);

	testPointH -= spriteP->destFrameRect.left;
	testPointV -= spriteP->destFrameRect.top;
	
	return !BKTransparentPixel(spriteP->curFrameP->frameSurfaceP, testPointH, testPointV);
}


#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------------------
//	SWRadiusCollision
//---------------------------------------------------------------------------------------

SWBoolean SWRadiusCollision(
	SpritePtr srcSpriteP,
	SpritePtr dstSpriteP)
{
	int		 		centerOneH,centerOneV , centerTwoH,centerTwoV ;
	register short		radiusOne, radiusTwo;
	register short		horizDistance, vertDistance;

	SW_ASSERT(srcSpriteP != NULL && dstSpriteP != NULL);

		// get radii - - assumes circular sprites!
	radiusOne = (srcSpriteP->destFrameRect.right-srcSpriteP->destFrameRect.left)/2;
	radiusTwo = (dstSpriteP->destFrameRect.right-dstSpriteP->destFrameRect.left)/2;

		// find centers
	centerOneH = srcSpriteP->destFrameRect.left + radiusOne;
	centerOneV = srcSpriteP->destFrameRect.top + radiusOne;
	centerTwoH = dstSpriteP->destFrameRect.left + radiusTwo;
	centerTwoV = dstSpriteP->destFrameRect.top + radiusTwo;

	horizDistance = centerOneH - centerTwoH;
	vertDistance = centerOneV - centerTwoV;

	return ( (horizDistance*horizDistance) + (vertDistance*vertDistance) <
		((radiusOne+radiusTwo)*(radiusOne+radiusTwo)) );
}


//---------------------------------------------------------------------------------------
//	SWPixelCollision
//---------------------------------------------------------------------------------------

SWBoolean SWPixelCollision(
	SpritePtr srcSpriteP,
	SpritePtr dstSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);
	SW_ASSERT(dstSpriteP != NULL);
	SW_ASSERT(srcSpriteP->curFrameP != NULL);
	SW_ASSERT(dstSpriteP->curFrameP != NULL);

	return BKCollidePixels(
		srcSpriteP->curFrameP->frameSurfaceP,
		srcSpriteP->destFrameRect.left, srcSpriteP->destFrameRect.top,
        dstSpriteP->curFrameP->frameSurfaceP,
        dstSpriteP->destFrameRect.left, dstSpriteP->destFrameRect.top);
}


#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------------------
//	SWIsSpriteInSprite - do the sprites' collisionSWRects overlap?
//---------------------------------------------------------------------------------------

SWBoolean SWIsSpriteInSprite(
	SpritePtr srcSpriteP,
	SpritePtr dstSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);
	SW_ASSERT(dstSpriteP != NULL);

	return ((SW_COLLISION_RECT_TOP(srcSpriteP) < SW_COLLISION_RECT_BOTTOM(dstSpriteP)) &&
			(SW_COLLISION_RECT_BOTTOM(srcSpriteP) > SW_COLLISION_RECT_TOP(dstSpriteP)) &&
			(SW_COLLISION_RECT_LEFT(srcSpriteP) < SW_COLLISION_RECT_RIGHT(dstSpriteP)) &&
			(SW_COLLISION_RECT_RIGHT(srcSpriteP) > SW_COLLISION_RECT_LEFT(dstSpriteP)) );
}


//---------------------------------------------------------------------------------------
//	SWIsSpriteInRect
//---------------------------------------------------------------------------------------

SWBoolean SWIsSpriteInRect(
	SpritePtr srcSpriteP,
	SWRect* testSWRect)
{
	SW_ASSERT(srcSpriteP != NULL);

	return ((SW_COLLISION_RECT_TOP(srcSpriteP) < testSWRect->bottom) &&
			(SW_COLLISION_RECT_BOTTOM(srcSpriteP) > testSWRect->top) &&
		 	(SW_COLLISION_RECT_LEFT(srcSpriteP) < testSWRect->right) &&
		 	(SW_COLLISION_RECT_RIGHT(srcSpriteP) > testSWRect->left));
}


//---------------------------------------------------------------------------------------
//	SWIsSpriteFullyInRect
//---------------------------------------------------------------------------------------

SWBoolean SWIsSpriteFullyInRect(
	SpritePtr srcSpriteP,
	SWRect* testSWRect)
{
	SW_ASSERT(srcSpriteP != NULL);

	return ((SW_COLLISION_RECT_TOP(srcSpriteP) >= testSWRect->top) &&
			(SW_COLLISION_RECT_BOTTOM(srcSpriteP) <= testSWRect->bottom) &&
		 	(SW_COLLISION_RECT_LEFT(srcSpriteP) >= testSWRect->left) &&
		 	(SW_COLLISION_RECT_RIGHT(srcSpriteP) <= testSWRect->right));
}


//---------------------------------------------------------------------------------------
//	SWIsPointInSprite -- checks whether a point is in the sprite's "collision rect"
//---------------------------------------------------------------------------------------

SWBoolean SWIsPointInSprite(
	SpritePtr srcSpriteP,
	SWPoint testPoint)
{
	SW_ASSERT(srcSpriteP != NULL);

	return	(testPoint.h >= SW_COLLISION_RECT_LEFT(srcSpriteP)) &&
			(testPoint.h < SW_COLLISION_RECT_RIGHT(srcSpriteP)) &&
			(testPoint.v >= SW_COLLISION_RECT_TOP(srcSpriteP)) &&
			(testPoint.v < SW_COLLISION_RECT_BOTTOM(srcSpriteP));
}


//---------------------------------------------------------------------------------------
//	SWIsPointInSpriteMask -- checks whether a point is in the sprite's actual mask
//---------------------------------------------------------------------------------------

SWBoolean SWIsPointInSpriteMask(
	SpritePtr srcSpriteP,
	SWPoint testPoint,
	SpriteInMaskProcPtr testProc)
{
	SW_ASSERT(srcSpriteP != NULL);

	if (testProc == NULL)
		return SWIsPointInSprite(srcSpriteP, testPoint);
	else
		return (*testProc)(srcSpriteP, testPoint.h, testPoint.v);
}
