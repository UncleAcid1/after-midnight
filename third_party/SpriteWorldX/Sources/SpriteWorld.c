//---------------------------------------------------------------------------------------
//  SpriteWorld.c
//
//  Portions are copyright: © 1991-94 Tony Myles, All rights reserved worldwide.
//
//  Description:    implementation of the sprite world architecture
//---------------------------------------------------------------------------------------


#ifndef __SWCOMMON__
#include <SWCommonHeaders.h>
#endif

#ifndef __SPRITEWORLD__
#include <SpriteWorld.h>
#endif

#ifndef __SPRITELAYER__
#include <SpriteLayer.h>
#endif

#ifndef __SPRITE__
#include <Sprite.h>
#endif

#ifndef __SPRITEFRAME__
#include <SpriteFrame.h>
#endif


#ifndef __SCROLLING__
#include <Scrolling.h>
#endif

#ifndef __TILING__
#include <Tiling.h>
#endif

#ifndef __BLITKERNEL__
#include <BlitKernel.h>
#endif

//---------------------------------------------------------------------------------------
//  globals
//---------------------------------------------------------------------------------------

SWBoolean           gSWInitializedSDL = false;
SWBoolean           gSWUseOpenGL = false;

DrawDataPtr         gSWCurrentElementDrawData = NULL;
SpriteWorldPtr      gSWCurrentSpriteWorld = NULL;

SpriteWorldPtr      gSWCleanUpSpriteWorldP = NULL;
CleanUpCallBackPtr  gSWCleanUpCallBackP = NULL;

SWError         gSWStickyError = kNoError; //changed from SWError and move out of spriteworldutils

AssertFailProcPtr   gSWAssertFailProc = &SWAssertFail; //moved from spriteworldutils - ben


//---------------------------------------------------------------------------------------
//  SWEnterSpriteWorld
//---------------------------------------------------------------------------------------

SWError SWEnterSpriteWorld(SWBoolean shouldInitSDL)
{
    SWError err = kNoError;
    
    if (shouldInitSDL)
    {
        /*initialize SDL Library*/
        if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0)
        {
            fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
            err = kFailedToInitSDL;
        }
    }
    
    gSWInitializedSDL = shouldInitSDL;
    
    SWSetStickyIfError(err);
    return err;
}


//---------------------------------------------------------------------------------------
//  SWExitSpriteWorld
//---------------------------------------------------------------------------------------

void SWExitSpriteWorld(void)
{
    if( gSWInitializedSDL )
        SDL_Quit();
}


//---------------------------------------------------------------------------------------
//  SWCreateSpriteWorld - uses new SDL 1.3 API, so that our window can be centered by default, repositioned, and resized
//---------------------------------------------------------------------------------------

SWError SWCreateSpriteWorld(
	SpriteWorldPtr  *spriteWorldPP,
	char			*windowTitle,
	int             w,
	int             h,
	int             depth,
	SWBoolean       makeFullScreen,
	SWBoolean       wantOpenGL )
{
    SWError			err = kNoError;
    char			*error;
    SDL_bool        opengl;
	int				i;
	SDL_WindowID	mainWindow;
	Uint32			windowFlags;
    
    *spriteWorldPP = NULL;
    
#ifdef HAVE_OPENGL
    opengl = wantOpenGL;
#else
    opengl = SDL_FALSE;
#endif
	
#if SDL_VERSION_ATLEAST(1,3,0)
	windowFlags = SDL_WINDOW_SHOWN;
	
	if (makeFullScreen)
		windowFlags |= SDL_WINDOW_FULLSCREEN;
	else
		windowFlags |= SDL_WINDOW_RESIZABLE;	// NEW
	
	if (opengl)
	{
		windowFlags |= SDL_WINDOW_OPENGL;  
		BKLoadGLfunctions();
		gSWUseOpenGL = true;
	}
	
//	windowFlags |= SDL_WINDOW_RESIZABLE;
	
		// Create our window, centered
    mainWindow = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, windowFlags);
    if (!mainWindow)
        return -1;
#endif
	
#if SDL_VERSION_ATLEAST(1,3,0)
	SDL_SelectRenderer(mainWindow);
	SDL_SetRenderDrawColor(0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);	// black background if nothing else drawn
	SDL_RenderClear();
#endif
	
	
    error = SDL_GetError();
    if( strlen(error) ) // this sometimes returns ""
        fprintf(stderr, "SDL_VideoMode: %s\n", error);
	
    
    if( err == kNoError )
    {
			// 3rd parameter is worldRect. Totally ignored here!
		err = SWCreateSpriteWorldFromSDLWindow( spriteWorldPP, mainWindow, depth, w, h, opengl, NULL, NULL, 0 );	
    }
    
    SWSetStickyIfError(err);
    return err;
}


//---------------------------------------------------------------------------------------
//  SWCreateSpriteWorldUsingOldSDL - uses the 1.2 API, which is still "compatible" with SDL 1.3,
//  but the new features in 1.3, such as moving/resizing the window, are not available if you do it this way.
//---------------------------------------------------------------------------------------

SWError SWCreateSpriteWorldUsingOldSDL(
    SpriteWorldPtr  *spriteWorldPP,
    int             w,
    int             h,
    int             depth,
    SWBoolean       makeFullScreen,
    SWBoolean       wantOpenGL )
{
    SWError err = kNoError;
    char *error;
    SDL_Surface *videoSurface;
    Uint32 flags;
    
    SDL_bool        opengl, hardware, pageflip;
    
    *spriteWorldPP = NULL;
    
#ifdef HAVE_OPENGL
    opengl = wantOpenGL;
#else
    opengl = SDL_FALSE;
#endif
    
    pageflip = SDL_TRUE;    // even if it doesn't have it, it can usually fake it?
    hardware = SDL_TRUE;    // we usually require hardware video memory surface...
    
    while (1)
    {
        flags = makeFullScreen ? SDL_FULLSCREEN : 0;

        if (opengl)
        {
            flags |= SDL_OPENGL;

                // load the library handle
            (void) BKLoadGLfunctions();

                // require double buffering
            SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
        }
        else
        {
            flags |= hardware ? SDL_HWSURFACE : SDL_SWSURFACE;
            flags |= pageflip ? SDL_DOUBLEBUF : 0;
        }
        
        if (SDL_VideoModeOK(w, h, depth, flags) > 0 )
        {
            videoSurface = SDL_SetVideoMode( w, h, depth, flags );
            if (videoSurface != NULL)
                break;
        }
        else
        {
            error = SDL_GetError();
            //fprintf(stderr, "SDL_VideoModeOK 0x%0x: %s", flags, error);
            SDL_ClearError();
            
            // note: this is a ugly workaround for bogus returned "errors"
            if (!*error || strcmp(error,"SDL not configured with thread support") == 0)
            {
                videoSurface = SDL_SetVideoMode( w, h, depth, flags );
                if (videoSurface != NULL)
                    break;
                else
                {
                    error = SDL_GetError();
                    //fprintf(stderr, "SDL_VideoMode 0x%0x: %s\n", flags, error);
                    SDL_ClearError();
                }
            }
        }
            
        if (opengl)
        {
            // try again, but without using OpenGL this time
            opengl = SDL_FALSE;
            continue;
        }
        else if (pageflip)
        {
            // try again, but without page flipping this time
            pageflip = SDL_FALSE;
            continue;
        }
        else if (hardware)
        {
            // try again, but without video memory this time
            hardware = SDL_FALSE;
            continue;
        }
        else
        {
            // no valid mode found
            videoSurface = NULL;
            break;
        }
    }
    
    error = SDL_GetError();
    if( strlen(error) ) // this sometimes returns ""
        fprintf(stderr, "SDL_VideoMode: %s\n", error);

    if( ! videoSurface )
        err = kSDLSetVideoMode;
    
    if( err == kNoError )
    {
        err = SWCreateSpriteWorldFromVideoSurface(
            spriteWorldPP,
            videoSurface,
            NULL, NULL, 0 );	// 3rd parameter is worldRect. Totally ignored here!
    }
    
    SWSetStickyIfError(err);
    return err;
}


//---------------------------------------------------------------------------------------
//  SWCreateSpriteWorldFromSDLWindow
//---------------------------------------------------------------------------------------

SWError SWCreateSpriteWorldFromSDLWindow(
	SpriteWorldPtr      *spriteWorldPP,
//	SDL_Surface         *videoSurfaceP,
	SDL_WindowID        mainWindow,
	short				depth,
	int					w,
	int					h,
	SWBoolean			useOpenGL,
	SWRect              *worldRectP,
	SWRect              *backRectP,
	int                 maxDepth )
{
    SWError         err = kNoError;
    SpriteWorldPtr  tempSpriteWorldP;
    FramePtr        screenFrameP;
    FramePtr        backFrameP;
    FramePtr        workFrameP;
    SWRect          windRect;
    SWRect          tempRect;
	SDL_GLContext	openglContext;
    
    *spriteWorldPP = tempSpriteWorldP = NULL;
    screenFrameP = backFrameP = workFrameP = NULL;
	
    if (worldRectP == NULL )
    {
        SW_SET_RECT( windRect, 0, 0, w, h );
    }
    else
    {
        windRect = *worldRectP;
    }
    
    if ( maxDepth != 0 && depth > maxDepth )
    {
        depth = maxDepth;
    }
	
	
		// If a custom offscreenRect has been requested, use it instead.
    if (backRectP == 0)
        tempRect = windRect;
    else
        tempRect = *backRectP;
	
		// offset Rect to (0,0) for back and work frames
    SW_OFFSET_RECT( tempRect, -tempRect.left, -tempRect.top);
	
	
	
	if (useOpenGL)
	{
			// WARNING: The GL_SetAttribute functions are supposed to be called before the SDL *WINDOW* is created!
		
#if SDL_VERSION_ATLEAST(1,3,0)
		/* Request an opengl 3.2 context.
		 * SDL doesn't have the ability to choose which profile at this time of writing,
		 * but it should default to the core profile */
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		
		/* Turn on double buffering with a 24bit Z buffer.
		 * You may need to change this to 16 or 32 for your system */
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	// Apparently this MUST be true, even on MacOS X, for it to work at all
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		
		// NOTE: Unlike SDL 1.2, where SDL_SetVideoMode() creates an OpenGL context for us,
		// we must do it ourselves with SDL 1.3, and attach it to the SDL window we created.
		// Otherwise, various SpriteWorld functions will crash when calling GL functions
		// that require a context to already have been created and made active.
		
		openglContext = SDL_GL_CreateContext(mainWindow);
#endif
		
	}
    
		// WARNING: If OpenGL is being used, we must create the context above BEFORE calling the functions below, otherwise they'll crash!
    if (err == kNoError)
    {
			// create screen frame
   //     err = SWCreateFrameFromSurface( &screenFrameP, videoSurfaceP, true );
		
			// Create dummy screen frame for now
		err = SWCreateBlankFrame( &screenFrameP, tempRect.right, tempRect.bottom, depth, false ); // CRASH if OpenGL context not set up by now
    }
    
    if (err == kNoError)
    {
			// create back drop frame
        err = SWCreateBlankFrame( &backFrameP, tempRect.right, tempRect.bottom, depth, false );
    }
	
#ifdef HAVE_OPENGL
    if (err == kNoError && !(useOpenGL))
    {
			// create work frame
        err = SWCreateBlankFrame( &workFrameP, tempRect.right, tempRect.bottom, depth, false );
    }
#endif // OpenGL
	
    if (err == kNoError)
    {
			// create sprite world
        err = SWCreateSpriteWorldFromFrames( spriteWorldPP, screenFrameP, backFrameP, workFrameP );
    }
	
	if (err == kNoError)
	{
		(*spriteWorldPP)->mainWindow = mainWindow;
		(*spriteWorldPP)->glContext = openglContext;
	}
	
	
	
	if (err == kNoError)
    {
#ifdef HAVE_OPENGL
        if ( useOpenGL )
        {
        	BKFlatGLSetup( w, h );
			
            (*spriteWorldPP)->useOpenGL = SDL_TRUE;
        }
        else
#endif // OpenGL
        {
            (*spriteWorldPP)->useOpenGL = SDL_FALSE;
        }
		
        printf("OpenGL: %d\n", (*spriteWorldPP)->useOpenGL);
    }
	
	
    if (err != kNoError )
    {
		// an error occurred so dispose of anything we managed to create
		
        if (screenFrameP != NULL)
        {
            SWDisposeFrame(&screenFrameP);
        }
		
        if (backFrameP != NULL)
        {
            SWDisposeFrame(&backFrameP);
        }
		
        if (workFrameP != NULL)
        {
            SWDisposeFrame(&workFrameP);
        }
    }
	
    SWSetStickyIfError( err );
    return err;
}


//---------------------------------------------------------------------------------------
//  SWCreateSpriteWorldFromVideoSurface
//---------------------------------------------------------------------------------------

SWError SWCreateSpriteWorldFromVideoSurface(
    SpriteWorldPtr      *spriteWorldPP,
    SDL_Surface         *videoSurfaceP,
    SWRect              *worldRectP,
    SWRect              *backRectP,
    int                 maxDepth )
{
    SWError         err = kNoError;
    SpriteWorldPtr  tempSpriteWorldP;
    FramePtr        screenFrameP;
    FramePtr        backFrameP;
    FramePtr        workFrameP;
    SWRect          windRect;
    SWRect          tempRect;
    Uint8           depth;
    
    *spriteWorldPP = tempSpriteWorldP = NULL;
    screenFrameP = backFrameP = workFrameP = NULL;

	SW_ASSERT(videoSurfaceP);

    if (worldRectP == NULL )
    {
        SW_SET_RECT( windRect, 0, 0, videoSurfaceP->w, videoSurfaceP->h );
    }
    else
    {
        windRect = *worldRectP;
    }
    
    if ( maxDepth == 0 || videoSurfaceP->format->BitsPerPixel <= maxDepth )
    {
        depth = videoSurfaceP->format->BitsPerPixel;
    }
    else
    {
        depth = maxDepth;
    }

    if (videoSurfaceP->flags & SDL_OPENGL)
    {
        gSWUseOpenGL = true;
    }

        // If a custom offscreenRect has been requested, use it instead.
    if (backRectP == 0)
        tempRect = windRect;
    else
        tempRect = *backRectP;

        // offset Rect to (0,0) for back and work frames
    SW_OFFSET_RECT( tempRect, -tempRect.left, -tempRect.top);

    
    if (err == kNoError)
    {
            // create screen frame
        err = SWCreateFrameFromSurface( &screenFrameP, videoSurfaceP, true );
    }
    
    if (err == kNoError)
    {
            // create back drop frame
        err = SWCreateBlankFrame( &backFrameP, tempRect.right, tempRect.bottom, depth, false );
    }

#ifdef HAVE_OPENGL
    if (err == kNoError && !(videoSurfaceP->flags & SDL_OPENGL))
    {
            // create work frame
        err = SWCreateBlankFrame( &workFrameP, tempRect.right, tempRect.bottom, depth, false );
    }
#endif // OpenGL

    if (err == kNoError)
    {
            // create sprite world
        err = SWCreateSpriteWorldFromFrames( spriteWorldPP, screenFrameP,
                backFrameP, workFrameP );
    }

    if (err == kNoError)
    {
#ifdef HAVE_OPENGL
        if (videoSurfaceP->flags & SDL_OPENGL)
        {
        	BKFlatGLSetup( videoSurfaceP->w,  videoSurfaceP->h );

            (*spriteWorldPP)->useOpenGL = SDL_TRUE;
        }
        else
#endif // OpenGL
        {
            (*spriteWorldPP)->useOpenGL = SDL_FALSE;
        }

        printf("OpenGL: %d\n", (*spriteWorldPP)->useOpenGL);
    }

    if (err != kNoError )
    {
            // an error occurred so dispose of anything we managed to create

        if (screenFrameP != NULL)
        {
            SWDisposeFrame(&screenFrameP);
        }

        if (backFrameP != NULL)
        {
            SWDisposeFrame(&backFrameP);
        }

        if (workFrameP != NULL)
        {
            SWDisposeFrame(&workFrameP);
        }
    }

    SWSetStickyIfError( err );
    return err;
}

//---------------------------------------------------------------------------------------
//  SWCreateSpriteWorldFromSWSurface
//---------------------------------------------------------------------------------------

SWError SWCreateSpriteWorldFromSWSurface(
    SpriteWorldPtr      *spriteWorldPP,
    SDL_Surface         *surfaceP,
    SWRect              *worldRectP,
    SWRect              *backRectP,
    int                 maxDepth )
{
    SWError         err = kNoError;
    SpriteWorldPtr  tempSpriteWorldP;
    FramePtr        screenFrameP;
    FramePtr        backFrameP;
    FramePtr        workFrameP;
    SWRect          windRect;
    SWRect          tempRect;
    Uint8           depth;
    
    *spriteWorldPP = tempSpriteWorldP = NULL;
    screenFrameP = backFrameP = workFrameP = NULL;

	SW_ASSERT(surfaceP != NULL);

    if (worldRectP == NULL )
    {
        SW_SET_RECT( windRect, 0, 0, surfaceP->w, surfaceP->h );
    }
    else
    {
        windRect = *worldRectP;
    }
    
    if ( maxDepth == 0 || surfaceP->format->BitsPerPixel <= maxDepth )
    {
        depth = surfaceP->format->BitsPerPixel;
    }
    else
    {
        depth = maxDepth;
    }

        // If a custom offscreenRect has been requested, use it instead.
    if (backRectP == NULL)
        tempRect = windRect;
    else
        tempRect = *backRectP;

        // offset Rect to (0,0) for back and work frames
    SW_OFFSET_RECT( tempRect, -tempRect.left, -tempRect.top);

    
    if (err == kNoError)
    {
             // create screen frame
        err = SWCreateFrameFromSurface( &screenFrameP, surfaceP, false );
    }
    
    if (err == kNoError)
    {
             // create back drop frame
        err = SWCreateBlankFrame( &backFrameP, tempRect.right, tempRect.bottom, depth, false );
    }

    if (err == kNoError)
    {
             // create work frame
        err = SWCreateBlankFrame( &workFrameP, tempRect.right, tempRect.bottom, depth, false );
    }

    if (err == kNoError)
    {
            // create sprite world
        err = SWCreateSpriteWorldFromFrames( &tempSpriteWorldP, screenFrameP,
                backFrameP, workFrameP );
    }

    if (err == kNoError)
    {
        tempSpriteWorldP->useOpenGL = SDL_FALSE;
    
    	*spriteWorldPP = tempSpriteWorldP;
    }

    if (err != kNoError )
    {
            // an error occurred so dispose of anything we managed to create

        if (screenFrameP != NULL)
        {
            SWDisposeFrame(&screenFrameP);
        }

        if (backFrameP != NULL)
        {
            SWDisposeFrame(&backFrameP);
        }

        if (workFrameP != NULL)
        {
            SWDisposeFrame(&workFrameP);
        }
    }

    SWSetStickyIfError( err );
    return err;
}

//---------------------------------------------------------------------------------------
//  SWCreateSpriteWorldFromGLContext - for use with SDL 1.2
//---------------------------------------------------------------------------------------

#if SW_USE_OLD_SDL

SWError SWCreateSpriteWorldFromGLContext(
    SpriteWorldPtr      *spriteWorldPP,
    void*               glContext,
    SWRect              *worldRectP,
    GLCallBackPtr       glMakeCurrent,
    GLCallBackPtr       glSwapBuffers)
{
    SWError         err = kNoError;
    SpriteWorldPtr  tempSpriteWorldP;
    FramePtr        screenFrameP;
    FramePtr        backFrameP;
    SWRect          windRect;
    SWRect          tempRect;
    
    *spriteWorldPP = tempSpriteWorldP = NULL;
    screenFrameP = backFrameP = NULL;

    gSWUseOpenGL = true;

    SW_ASSERT(worldRectP != NULL);
    windRect = *worldRectP;
    tempRect = windRect;
    
        // offset Rect to (0,0) for back and work frames
    SW_OFFSET_RECT( tempRect, -tempRect.left, -tempRect.top);

  	if (glMakeCurrent) (*glMakeCurrent)(glContext);
 
    if (err == kNoError)
    {
            // create fake screen frame (for now)
        err = SWCreateBlankFrame( &screenFrameP, tempRect.right, tempRect.bottom, 0, false );
    }

    if (err == kNoError)
    {
            // create back drop frame
        err = SWCreateBlankFrame( &backFrameP, tempRect.right, tempRect.bottom, 0, false );
    }

    if (err == kNoError)
    {
          // create sprite world
        err = SWCreateSpriteWorldFromFrames( &tempSpriteWorldP, screenFrameP,
                backFrameP, (FramePtr) NULL );
    }

    if (err == kNoError)
    {
  		BKFlatGLSetup(  tempRect.right,  tempRect.bottom );

		tempSpriteWorldP->useOpenGL = SDL_TRUE;
		tempSpriteWorldP->glContext = glContext;

		tempSpriteWorldP->glMakeCurrent = glMakeCurrent;
		tempSpriteWorldP->glSwapBuffers = glSwapBuffers;
   		
   		*spriteWorldPP = tempSpriteWorldP;
    }

    if (err != kNoError )
    {
            // an error occurred so dispose of anything we managed to create

        if (screenFrameP != NULL)
        {
            SWDisposeFrame(&screenFrameP);
        }
		if (backFrameP != NULL)
        {
            SWDisposeFrame(&backFrameP);
        }
    }

    SWSetStickyIfError( err );
    return err;
}

#endif // USE_OLD_SDL

//---------------------------------------------------------------------------------------
//  SWCreateSpriteWorldFromFrames
//---------------------------------------------------------------------------------------

SWError SWCreateSpriteWorldFromFrames(
    SpriteWorldPtr      *spriteWorldP,
    FramePtr            screenFrameP,
    FramePtr            backFrameP,
    FramePtr            workFrameP )
{
    SWError err = kNoError;
    SpriteWorldPtr  tempSpriteWorldP;
        
    
    tempSpriteWorldP = (SpriteWorldPtr) calloc(1,(sizeof(SpriteWorldRec)));
    
    if (!tempSpriteWorldP)
    {
        err = kMemoryAllocationError;
    }
    
    if (err == kNoError)
    {
        // Create the deadSpriteLayerP, but don't add it to the linked layer list
        err = SWCreateSpriteLayer(&tempSpriteWorldP->deadSpriteLayerP);
        if (err != kNoError)
            free(tempSpriteWorldP);
    }
    
    if (err == kNoError)
    {
        tempSpriteWorldP->headSpriteLayerP = 0;
        tempSpriteWorldP->tailSpriteLayerP = 0;

        tempSpriteWorldP->screenFrameP = screenFrameP;
        tempSpriteWorldP->extraBackFrameP = 0;
        tempSpriteWorldP->backFrameP = backFrameP;
        tempSpriteWorldP->workFrameP = workFrameP;

        tempSpriteWorldP->postEraseCallBack = 0;
        tempSpriteWorldP->postDrawCallBack = 0;
        tempSpriteWorldP->screenRect = screenFrameP->frameRect;
        tempSpriteWorldP->backRect = backFrameP->frameRect;
        
        //set all drawprocs to standard drawproc
        tempSpriteWorldP->offscreenDrawProc = &SWStdOffscreenDrawProc;
        tempSpriteWorldP->screenDrawProc = &SWStdScreenDrawProc;    

        tempSpriteWorldP->headUpdateRectP = 0;
        tempSpriteWorldP->useUpdateRectOptimizations = true;

        SW_SET_RECT(tempSpriteWorldP->scrollRectMoveBounds, 0,0,32767,32767);
        
        tempSpriteWorldP->horizScrollRectOffset = 0;
        tempSpriteWorldP->vertScrollRectOffset = 0;
        tempSpriteWorldP->horizScrollDelta = 0;
        tempSpriteWorldP->vertScrollDelta = 0;
        tempSpriteWorldP->worldMoveProc = 0;
        tempSpriteWorldP->thereAreNonScrollingLayers = false;

            // Make sure visScrollRect is at 0,0 top-left corner
        tempSpriteWorldP->visScrollRect = screenFrameP->frameRect;
        
        SW_OFFSET_RECT(tempSpriteWorldP->visScrollRect, 
                -tempSpriteWorldP->visScrollRect.left, -tempSpriteWorldP->visScrollRect.top);
        
        tempSpriteWorldP->offscreenScrollRect = tempSpriteWorldP->visScrollRect;
        tempSpriteWorldP->oldVisScrollRect = tempSpriteWorldP->visScrollRect;

                
                tempSpriteWorldP->tileLayerArray = NULL;
        tempSpriteWorldP->tilingCache = NULL;
        tempSpriteWorldP->changedTiles = NULL;
        tempSpriteWorldP->tilingIsInitialized = false;
        tempSpriteWorldP->tilingIsOn = false;
        tempSpriteWorldP->numTilesChanged = 0;
        tempSpriteWorldP->tileChangeProc = NULL;
		tempSpriteWorldP->tileMaskDrawProc = SWStdSpriteDrawProc;
        //tempSpriteWorldP->partialMaskDrawProc = SWStdSpriteDrawProc;
        tempSpriteWorldP->customTileDrawProc = SWStdCustomTileDrawProc;
                

        tempSpriteWorldP->frameHasOccurred = false;
        tempSpriteWorldP->fpsTimeInterval = 0;
        tempSpriteWorldP->runningTimeCount = 0;

            // Time-based animation variables
        tempSpriteWorldP->pseudoFPS = -1;
        tempSpriteWorldP->minimumFrameRate = 1000000/1;     // Default: 1 frame per second minimum

        tempSpriteWorldP->millisecsSinceLastFrame = 0;
        tempSpriteWorldP->pseudoFrameAccum = 0;
        tempSpriteWorldP->pseudoFrameHasFired = false;


        tempSpriteWorldP->backgroundColor.r = 0;
        tempSpriteWorldP->backgroundColor.g = 0;
        tempSpriteWorldP->backgroundColor.b = 0;

        tempSpriteWorldP->makeTranslucencyAdjustments = false;
        tempSpriteWorldP->doFastMovingSpritesChecks = false;

        SWClearStickyError();

        *spriteWorldP = tempSpriteWorldP;
    }
    
    SWSetStickyIfError(err);    
    return err;
}


//---------------------------------------------------------------------------------------
//  SWDisposeSpriteWorld
//---------------------------------------------------------------------------------------

void SWDisposeSpriteWorld(
    SpriteWorldPtr  *spriteWorldPP)
{
    SpriteWorldPtr  spriteWorldP = *spriteWorldPP;
    SpriteLayerPtr  curLayerP;
    SpritePtr       curSpriteP;

    if (spriteWorldP != 0)
    {
        SWUnlockSpriteWorld(spriteWorldP);

        while ((curLayerP = SWGetNextSpriteLayer(spriteWorldP, 0)) != 0)
        {
                // dispose of each sprite in the layer
            while ((curSpriteP = SWGetNextSprite(curLayerP, 0)) != 0)
            {
                SWRemoveSprite(curSpriteP);
                SWDisposeSprite(&curSpriteP);
            }

                // dispose of each layer in the spriteWorld
            SWRemoveSpriteLayer(spriteWorldP, curLayerP);
            SWDisposeSpriteLayer(&curLayerP);
        }

        SWDisposeAllSpritesInLayer(spriteWorldP->deadSpriteLayerP);
        SWDisposeSpriteLayer(&spriteWorldP->deadSpriteLayerP);

        SWDisposeFrame(&spriteWorldP->backFrameP);
        SWDisposeFrame(&spriteWorldP->workFrameP);
        SWDisposeExtraBackFrame(spriteWorldP);

#if SDL_VERSION_ATLEAST(1,3,0)
		// SDL stuff
        SDL_GL_DeleteContext(spriteWorldP->glContext);
		SDL_DestroyWindow(spriteWorldP->mainWindow);
#endif
        
        free(spriteWorldP);
        *spriteWorldPP = 0; // Set original pointer to 0
    }
}


#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------------------
//  SWAddSpriteLayer
//---------------------------------------------------------------------------------------

void SWAddSpriteLayer(
    SpriteWorldPtr      spriteWorldP,
    SpriteLayerPtr      newSpriteLayerP)
{
    SpriteLayerPtr  tailSpriteLayerP = spriteWorldP->tailSpriteLayerP;

    SW_ASSERT(spriteWorldP != 0);

    newSpriteLayerP->parentSpriteWorldP = spriteWorldP;

    if (tailSpriteLayerP != 0)
        tailSpriteLayerP->nextSpriteLayerP = newSpriteLayerP;
    else
        spriteWorldP->headSpriteLayerP = newSpriteLayerP;

    newSpriteLayerP->prevSpriteLayerP = tailSpriteLayerP;
    newSpriteLayerP->nextSpriteLayerP = 0;

        // make the new layer the tail
    spriteWorldP->tailSpriteLayerP = newSpriteLayerP;
}


//---------------------------------------------------------------------------------------
//  SWRemoveSpriteLayer
//---------------------------------------------------------------------------------------

void SWRemoveSpriteLayer(
    SpriteWorldPtr  spriteWorldP,
    SpriteLayerPtr  oldSpriteLayerP)
{
    SW_ASSERT(spriteWorldP != 0);

    oldSpriteLayerP->parentSpriteWorldP = 0;

        // is there a next layer?
    if (oldSpriteLayerP->nextSpriteLayerP != 0)
    {
            // link the next layer to the prev layer
        oldSpriteLayerP->nextSpriteLayerP->prevSpriteLayerP = oldSpriteLayerP->prevSpriteLayerP;
    }
    else
    {
            // make the prev layer the tail
        spriteWorldP->tailSpriteLayerP = oldSpriteLayerP->prevSpriteLayerP;
    }

        // is there a prev layer?
    if (oldSpriteLayerP->prevSpriteLayerP != 0)
    {
            // link the prev layer to the next layer
        oldSpriteLayerP->prevSpriteLayerP->nextSpriteLayerP = oldSpriteLayerP->nextSpriteLayerP;
    }
    else
    {
            // make the next layer the head
        spriteWorldP->headSpriteLayerP = oldSpriteLayerP->nextSpriteLayerP;
    }
}


//---------------------------------------------------------------------------------------
//  SWSwapSpriteLayer
//---------------------------------------------------------------------------------------

void SWSwapSpriteLayer(
    SpriteWorldPtr  spriteWorldP,
    SpriteLayerPtr  srcSpriteLayerP,
    SpriteLayerPtr  dstSpriteLayerP)
{
    register SpriteLayerPtr     swapSpriteLayerP;

    SW_ASSERT(spriteWorldP != 0);
    SW_ASSERT(srcSpriteLayerP != 0 && dstSpriteLayerP != 0);

        // Do nothing if the sprite layers are the same
    if (srcSpriteLayerP == dstSpriteLayerP)
        return;

        // adjacent Layers are a special case
    if ( srcSpriteLayerP->nextSpriteLayerP == dstSpriteLayerP ||
        dstSpriteLayerP->nextSpriteLayerP == srcSpriteLayerP )
    {
        if ( srcSpriteLayerP->nextSpriteLayerP == dstSpriteLayerP )
        {
            if ( srcSpriteLayerP->prevSpriteLayerP != 0 )
                (srcSpriteLayerP->prevSpriteLayerP)->nextSpriteLayerP = dstSpriteLayerP;
            if ( dstSpriteLayerP->nextSpriteLayerP != 0 )
                (dstSpriteLayerP->nextSpriteLayerP)->prevSpriteLayerP = srcSpriteLayerP;

            dstSpriteLayerP->prevSpriteLayerP = srcSpriteLayerP->prevSpriteLayerP;
            srcSpriteLayerP->nextSpriteLayerP = dstSpriteLayerP->nextSpriteLayerP;

            dstSpriteLayerP->nextSpriteLayerP = srcSpriteLayerP;
            srcSpriteLayerP->prevSpriteLayerP = dstSpriteLayerP;
        }
        else
        {
            if ( dstSpriteLayerP->prevSpriteLayerP != 0 )
                (dstSpriteLayerP->prevSpriteLayerP)->nextSpriteLayerP = srcSpriteLayerP;
            if ( srcSpriteLayerP->nextSpriteLayerP != 0 )
                (srcSpriteLayerP->nextSpriteLayerP)->prevSpriteLayerP = dstSpriteLayerP;

            srcSpriteLayerP->prevSpriteLayerP = dstSpriteLayerP->prevSpriteLayerP;
            dstSpriteLayerP->nextSpriteLayerP = srcSpriteLayerP->nextSpriteLayerP;

            srcSpriteLayerP->nextSpriteLayerP = dstSpriteLayerP;
            dstSpriteLayerP->prevSpriteLayerP = srcSpriteLayerP;
        }
    }
    else
    {
        if (srcSpriteLayerP->prevSpriteLayerP != 0)
            srcSpriteLayerP->prevSpriteLayerP->nextSpriteLayerP = dstSpriteLayerP;
        if (dstSpriteLayerP->prevSpriteLayerP != 0)
            dstSpriteLayerP->prevSpriteLayerP->nextSpriteLayerP = srcSpriteLayerP;
        if (srcSpriteLayerP->nextSpriteLayerP != 0)
            srcSpriteLayerP->nextSpriteLayerP->prevSpriteLayerP = dstSpriteLayerP;
        if (dstSpriteLayerP->nextSpriteLayerP != 0)
            dstSpriteLayerP->nextSpriteLayerP->prevSpriteLayerP = srcSpriteLayerP;

        swapSpriteLayerP = srcSpriteLayerP->nextSpriteLayerP;
        srcSpriteLayerP->nextSpriteLayerP = dstSpriteLayerP->nextSpriteLayerP;
        dstSpriteLayerP->nextSpriteLayerP = swapSpriteLayerP;

        swapSpriteLayerP = srcSpriteLayerP->prevSpriteLayerP;
        srcSpriteLayerP->prevSpriteLayerP = dstSpriteLayerP->prevSpriteLayerP;
        dstSpriteLayerP->prevSpriteLayerP = swapSpriteLayerP;
    }

    if (srcSpriteLayerP->nextSpriteLayerP == 0)
    {
        spriteWorldP->tailSpriteLayerP = srcSpriteLayerP;
    }
    else if (srcSpriteLayerP->prevSpriteLayerP == 0)
    {
        spriteWorldP->headSpriteLayerP = srcSpriteLayerP;
    }

    if (dstSpriteLayerP->nextSpriteLayerP == 0)
    {
        spriteWorldP->tailSpriteLayerP = dstSpriteLayerP;
    }
    else if (dstSpriteLayerP->prevSpriteLayerP == 0)
    {
        spriteWorldP->headSpriteLayerP = dstSpriteLayerP;
    }
}


//---------------------------------------------------------------------------------------
//  SWGetNextSpriteLayer
//---------------------------------------------------------------------------------------

SpriteLayerPtr SWGetNextSpriteLayer(
    SpriteWorldPtr  spriteWorldP,
    SpriteLayerPtr  curSpriteLayerP)
{
    SW_ASSERT(spriteWorldP != 0);

    return (curSpriteLayerP == 0) ?
            spriteWorldP->headSpriteLayerP :
            curSpriteLayerP->nextSpriteLayerP;
}


//---------------------------------------------------------------------------------------
//  SWLockSpriteWorld
//---------------------------------------------------------------------------------------

void SWLockSpriteWorld(
    SpriteWorldPtr  spriteWorldP)
{
    SpriteLayerPtr curSpriteLayerP;

    SW_ASSERT(spriteWorldP != 0);
    
    //SWLockWindowFrame(spriteWorldP->screenFrameP);
    if (spriteWorldP->workFrameP != 0)
        SWLockFrame(spriteWorldP->workFrameP);
    if (spriteWorldP->backFrameP != 0)
        SWLockFrame(spriteWorldP->backFrameP);
    if (spriteWorldP->extraBackFrameP != 0)
        SWLockFrame(spriteWorldP->extraBackFrameP);

        
    SWLockTiles(spriteWorldP);
        
        
    curSpriteLayerP = spriteWorldP->headSpriteLayerP;

    while (curSpriteLayerP != 0)
    {
        SWLockSpriteLayer(curSpriteLayerP);

        curSpriteLayerP = curSpriteLayerP->nextSpriteLayerP;
    }
}


//---------------------------------------------------------------------------------------
//  SWUnlockSpriteWorld
//---------------------------------------------------------------------------------------

void SWUnlockSpriteWorld(
    SpriteWorldPtr  spriteWorldP)
{
    SpriteLayerPtr curSpriteLayerP;

    SW_ASSERT(spriteWorldP != 0);

    //SWUnlockWindowFrame(spriteWorldP->screenFrameP);
    if (spriteWorldP->workFrameP != 0)
        SWUnlockFrame(spriteWorldP->workFrameP);
    if (spriteWorldP->backFrameP != 0)
        SWUnlockFrame(spriteWorldP->backFrameP);
    if (spriteWorldP->extraBackFrameP != 0)
        SWUnlockFrame(spriteWorldP->extraBackFrameP);

        
    SWUnlockTiles(spriteWorldP);
        
        
    curSpriteLayerP = spriteWorldP->headSpriteLayerP;

    while (curSpriteLayerP != 0)
    {
        SWUnlockSpriteLayer(curSpriteLayerP);

        curSpriteLayerP = curSpriteLayerP->nextSpriteLayerP;
    }
}

#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------------------
//  SWSetPostEraseCallBack
//---------------------------------------------------------------------------------------

void SWSetPostEraseCallBack(
    SpriteWorldPtr  spriteWorldP,
    CallBackPtr     callBack)
{
    spriteWorldP->postEraseCallBack = callBack;
}


//---------------------------------------------------------------------------------------
//  SWSetPostDrawCallBack
//---------------------------------------------------------------------------------------

void SWSetPostDrawCallBack(
    SpriteWorldPtr  spriteWorldP,
    CallBackPtr     callBack)
{
    spriteWorldP->postDrawCallBack = callBack;
}


//---------------------------------------------------------------------------------------
//  SWSetSpriteWorldMaxFPS
//---------------------------------------------------------------------------------------

void SWSetSpriteWorldMaxFPS(
    SpriteWorldPtr spriteWorldP,
    short framesPerSec)
{
    SW_ASSERT(spriteWorldP != 0);

        // is framesPerSec a valid value?
    if (framesPerSec > 0 && framesPerSec <= 1000 )
    {
        spriteWorldP->fpsTimeInterval = 1000/framesPerSec;
    }
        // framesPerSec is an "ignore it" value.
    else
    {
        spriteWorldP->fpsTimeInterval = 0;
    }
}


//---------------------------------------------------------------------------------------
//  SWSetSpriteWorldPseudoFPS - similar to maxFPS, but used for time-based animations.
//---------------------------------------------------------------------------------------

void SWSetSpriteWorldPseudoFPS(
    SpriteWorldPtr spriteWorldP,
    short framesPerSec)
{
    SW_ASSERT(spriteWorldP != 0);

    spriteWorldP->pseudoFPS = framesPerSec;
}


//---------------------------------------------------------------------------------------
// SWSetMinimumFrameRate - for use with time-based animations.
// You can pass 0 to have no limit, but this is NOT recommended.
// The default value is 1. We advise a higher minimum, such as 10fps or 20fps.
//---------------------------------------------------------------------------------------

void SWSetMinimumFrameRate(
    SpriteWorldPtr spriteWorldP,
    long minimumFrameRate)
{
    if (minimumFrameRate > 0)
        spriteWorldP->minimumFrameRate = 1000000/minimumFrameRate;
    else
        spriteWorldP->minimumFrameRate = 0xFFFFFFFF;
}


//---------------------------------------------------------------------------------------
//  SWSetBackgroundColor
//---------------------------------------------------------------------------------------

void SWSetBackgroundColor(
    SpriteWorldPtr spriteWorldP,
    SDL_Color color)
{
    SW_ASSERT(spriteWorldP != 0);

    spriteWorldP->backgroundColor = color;
}

/*
//---------------------------------------------------------------------------------------
//  SWSetTranslucencyAdjustments - set whether we need to make allowances for translucent
// or alpha masked sprites.  i.e. the entire deltaRect of each sprite will be erased
// rather than just the oldRect
//---------------------------------------------------------------------------------------

void SWSetTranslucencyAdjustments(
    SpriteWorldPtr spriteWorldP,
    int makeAdjustments)
{
    SW_ASSERT(spriteWorldP != NULL);
    spriteWorldP->makeTranslucencyAdjustments = makeAdjustments;
}


//---------------------------------------------------------------------------------------
//  SWSetGlobalFastMovingSpritesChecks - set whether we need to check every sprite to see
// if we should deal with its oldRects and destRects separately when animating, or if
// should just use the deltaRects.  If no checking is done then deltaRects will always be
// used, except where individual sprites have overridden this setting
//---------------------------------------------------------------------------------------

void SWSetGlobalFastMovingSpritesChecks(
    SpriteWorldPtr spriteWorldP,
    int makeChecks)
{
    SW_ASSERT(spriteWorldP != NULL);
    spriteWorldP->doFastMovingSpritesChecks = makeChecks;
}

*/


//---------------------------------------------------------------------------------------
//  SWStdOffscreenDrawProc - formerly called SWStdWorldDrawProc. 
//---------------------------------------------------------------------------------------

void SWStdOffscreenDrawProc(
    FramePtr    srcFrameP,
    FramePtr    dstFrameP,
    SWRect*     srcRect,
    SWRect*     dstRect)
{
    SWRect  srcFinalRect = *srcRect;
    SWRect  dstFinalRect = *dstRect;
    SDL_Rect srcSDLRect;
    SDL_Rect dstSDLRect;

		// Note: We can just remove the "gl->" to call the functions directly, but doing it this way allows us to call
		// functions in later GL versions, even if our headers are old. (i.e. call GL 2.x functions while using GL 1.x headers)
	struct BK_GL_func *gl = BKLoadGLfunctions();
	struct BK_GL_caps *caps = BKLoadGLcapabilities();
	
	if (gSWCurrentElementDrawData != NULL)
		glColor4us(gSWCurrentElementDrawData->colorRed, gSWCurrentElementDrawData->colorGreen,
				gSWCurrentElementDrawData->colorBlue, gSWCurrentElementDrawData->translucencyLevel);
	else
		gl->glColor4ub(255,255,255, SDL_ALPHA_OPAQUE);
	
	gl->glMatrixMode(GL_MODELVIEW);
	gl->glPushMatrix();
		gl->glLoadIdentity();
		gl->glTranslatef(dstFinalRect.left, dstFinalRect.top, 0.0f);
		
		if (gSWCurrentElementDrawData != NULL)
		{
			if (gSWCurrentElementDrawData->scaledHeightFactor != 1 || gSWCurrentElementDrawData->scaledWidthFactor != 1)
				gl->glScalef(gSWCurrentElementDrawData->scaledWidthFactor, gSWCurrentElementDrawData->scaledHeightFactor, 1.0f);
				
			if ( BKSurfaceNeedsBlend(srcFrameP->glSurfaceP) )
			{
					// Default values are GL_SRC_ALPHA and GL_ONE_MINUS_SRC_ALPHA
				gl->glEnable(GL_BLEND);
				gl->glBlendFunc(gSWCurrentElementDrawData->srcBlendMode, gSWCurrentElementDrawData->dstBlendMode);
			}
		}
		
			// Pass in the *source* frame only. The "dest frame" is the current GL context.
		BKBlitGLSurface((BK_GL_Surface *) srcFrameP->glSurfaceP); 
	gl->glPopMatrix();
}



//---------------------------------------------------------------------------------------
//  SWStdOffscreenDrawProc - Former version which had some non-OpenGL support
//---------------------------------------------------------------------------------------
/*
void SWStdOffscreenDrawProc(
    FramePtr    srcFrameP,
    FramePtr    dstFrameP,
    SWRect*     srcRect,
    SWRect*     dstRect)
{
    SWError err = kNoError;
    SWRect  srcFinalRect = *srcRect;
    SWRect  dstFinalRect = *dstRect;
    SDL_Rect srcSDLRect;
    SDL_Rect dstSDLRect;

#ifdef HAVE_OPENGL
    if (gSWCurrentSpriteWorld && gSWCurrentSpriteWorld->useOpenGL)
    {
			// Note: We can just remove the "gl->" to call the functions directly, but doing it this way allows us to call
			// functions in later GL versions, even if our headers are old. (i.e. call GL 2.x functions while using GL 1.x headers)
		struct BK_GL_func *gl = BKLoadGLfunctions();
        struct BK_GL_caps *caps = BKLoadGLcapabilities();
		
		if (gSWCurrentElementDrawData != NULL)
			glColor4us(gSWCurrentElementDrawData->colorRed, gSWCurrentElementDrawData->colorGreen,
					gSWCurrentElementDrawData->colorBlue, gSWCurrentElementDrawData->translucencyLevel);
		else
			gl->glColor4ub(255,255,255, SDL_ALPHA_OPAQUE);
		
        gl->glMatrixMode(GL_MODELVIEW);
        gl->glPushMatrix();
            gl->glLoadIdentity();
            gl->glTranslatef(dstFinalRect.left, dstFinalRect.top, 0.0f);
			
			if (gSWCurrentElementDrawData != NULL)
			{
				if (gSWCurrentElementDrawData->scaledHeightFactor != 1 || gSWCurrentElementDrawData->scaledWidthFactor != 1)
					gl->glScalef(gSWCurrentElementDrawData->scaledWidthFactor, gSWCurrentElementDrawData->scaledHeightFactor, 1.0f);
					
				if ( DoesSurfaceNeedBlend(srcFrameP->glSurfaceP) )
				{
						// Default values are GL_SRC_ALPHA and GL_ONE_MINUS_SRC_ALPHA
					gl->glEnable(GL_BLEND);
					gl->glBlendFunc(gSWCurrentElementDrawData->srcBlendMode, gSWCurrentElementDrawData->dstBlendMode);
				}
			}
			
            BKBlitGLSurface((BK_GL_Surface *) srcFrameP->glSurfaceP); // Pass in the *source* frame only. The "dest frame" is the current GL context.
        gl->glPopMatrix();
		
    }
    else
#endif
    {
    
    // NOTE: We need to clip here (even if QuickDraw clips to the port's rect),
    //       cause the frameRect might be only a part of the port.

        // clip off the top
    if (dstFinalRect.top < dstFrameP->frameRect.top)
    {
        srcFinalRect.top += dstFrameP->frameRect.top - dstFinalRect.top;
        dstFinalRect.top =  dstFrameP->frameRect.top;
        if (dstFinalRect.top >= dstFinalRect.bottom) return;
    }
        // clip off the bottom
    if (dstFinalRect.bottom > dstFrameP->frameRect.bottom)
    {
        srcFinalRect.bottom -= dstFinalRect.bottom - dstFrameP->frameRect.bottom;
        dstFinalRect.bottom = dstFrameP->frameRect.bottom;
        if (dstFinalRect.bottom <= dstFinalRect.top) return;
    }
        // clip off the left
    if (dstFinalRect.left < dstFrameP->frameRect.left)
    {
        srcFinalRect.left += dstFrameP->frameRect.left - dstFinalRect.left;
        dstFinalRect.left = dstFrameP->frameRect.left;
        if (dstFinalRect.left >= dstFinalRect.right)
            return;
    }
        // clip off the right
    if (dstFinalRect.right > dstFrameP->frameRect.right)
    {
        srcFinalRect.right -= dstFinalRect.right - dstFrameP->frameRect.right;
        dstFinalRect.right = dstFrameP->frameRect.right;
        if (dstFinalRect.right <= dstFinalRect.left) return;
    }

    SW_CONVERT_SW_TO_SDL_RECT( srcFinalRect, srcSDLRect );
    SW_CONVERT_SW_TO_SDL_RECT( dstFinalRect, dstSDLRect );
        
    err = SDL_BlitSurface(
        srcFrameP->frameSurfaceP,
        &srcSDLRect,
        dstFrameP->frameSurfaceP,
        &dstSDLRect );
    
    } // OpenGL
    
    SWSetStickyIfError( err );
}
*/


//---------------------------------------------------------------------------------------
//  SWStdScreenDrawProc - UNUSED when using OpenGL
//---------------------------------------------------------------------------------------

void SWStdScreenDrawProc(
    FramePtr    srcFrameP,
    FramePtr    dstFrameP,
    SWRect*     srcRect,
    SWRect*     dstRect)
{
    SWError err = kNoError;
    SWRect  srcFinalRect = *srcRect;
    SWRect  dstFinalRect = *dstRect;
    SDL_Rect srcSDLRect;
    SDL_Rect dstSDLRect;    
    
    // NOTE: We need to clip here (even if QuickDraw clips to the port's rect),
    //       cause the frameRect might be only a part of the port.

        // clip off the top
    if (dstFinalRect.top < dstFrameP->frameRect.top)
    {
        srcFinalRect.top += dstFrameP->frameRect.top - dstFinalRect.top;
        dstFinalRect.top =  dstFrameP->frameRect.top;
        if (dstFinalRect.top >= dstFinalRect.bottom) return;
    }
        // clip off the bottom
    if (dstFinalRect.bottom > dstFrameP->frameRect.bottom)
    {
        srcFinalRect.bottom -= dstFinalRect.bottom - dstFrameP->frameRect.bottom;
        dstFinalRect.bottom = dstFrameP->frameRect.bottom;
        if (dstFinalRect.bottom <= dstFinalRect.top) return;
    }
        // clip off the left
    if (dstFinalRect.left < dstFrameP->frameRect.left)
    {
        srcFinalRect.left += dstFrameP->frameRect.left - dstFinalRect.left;
        dstFinalRect.left = dstFrameP->frameRect.left;
        if (dstFinalRect.left >= dstFinalRect.right)
            return;
    }
        // clip off the right
    if (dstFinalRect.right > dstFrameP->frameRect.right)
    {
        srcFinalRect.right -= dstFinalRect.right - dstFrameP->frameRect.right;
        dstFinalRect.right = dstFrameP->frameRect.right;
        if (dstFinalRect.right <= dstFinalRect.left) return;
    }

    SW_CONVERT_SW_TO_SDL_RECT( srcFinalRect, srcSDLRect );
    SW_CONVERT_SW_TO_SDL_RECT( dstFinalRect, dstSDLRect );
    
#if SW_USE_OLD_SDL
    err = SDL_BlitSurface(
        srcFrameP->frameSurfaceP,
        &srcSDLRect,
        dstFrameP->frameSurfaceP,
        &dstSDLRect );
    
    SWSetStickyIfError( err );

    if ( dstFrameP->isVideoSurface )
		SDL_UpdateRect( dstFrameP->frameSurfaceP, dstSDLRect.x, dstSDLRect.y, dstSDLRect.w, dstSDLRect.h );
	
#else
	if (gSWCurrentSpriteWorld != NULL)
		SDL_SelectRenderer(gSWCurrentSpriteWorld->mainWindow);
	
		// NOTE: This won't work under SDL 1.3, because it requires a *Texture* as an input, not a Surface!
//	SDL_RenderCopy(srcFrameP->frameSurfaceP, &srcSDLRect, &dstSDLRect);
#endif
}

//---------------------------------------------------------------------------------------
//  SWCopyBackgroundToWorkArea
//---------------------------------------------------------------------------------------

void SWCopyBackgroundToWorkArea(
    SpriteWorldPtr spriteWorldP)
{
    //GWorldPtr     saveGWorld;
    //GDHandle      saveGDevice;

    SW_ASSERT(spriteWorldP != NULL);
    //SW_ASSERT(spriteWorldP->backFrameP->isFrameLocked);
    //SW_ASSERT(spriteWorldP->workFrameP->isFrameLocked);

    //GetGWorld( &saveGWorld, &saveGDevice );

    //SWSetPortToFrame( spriteWorldP->workFrameP );

        // Copy the background frame into the work area
    (*spriteWorldP->offscreenDrawProc)(spriteWorldP->backFrameP,
                                spriteWorldP->workFrameP,
                                &spriteWorldP->backFrameP->frameRect,
                                &spriteWorldP->workFrameP->frameRect);

    //SetGWorld( saveGWorld, saveGDevice );
}

#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------------------
//  SWUpdateSpriteWorld
//---------------------------------------------------------------------------------------

void SWUpdateSpriteWorld(
    SpriteWorldPtr spriteWorldP,
    int updateWindow)
{
    UpdateRectStructPtr         curRectStructP,
                                nextRectStructP;
    register SpriteLayerPtr     curSpriteLayerP;
    register SpritePtr          curSpriteP;
    SWRect                      tempRect;
//    short                       curTileLayer;

    SW_ASSERT(spriteWorldP != NULL);

    gSWCurrentSpriteWorld = spriteWorldP;

#ifdef HAVE_OPENGL
    if (spriteWorldP->useOpenGL)
    {
		
	#if SW_USE_OLD_SDL
		if (spriteWorldP->glMakeCurrent)	// SDL 1.2 way of doing things
				spriteWorldP->glMakeCurrent(spriteWorldP->glContext);
	#else
		SDL_GL_MakeCurrent(spriteWorldP->mainWindow, spriteWorldP->glContext);	// SDL 1.3 way. Probably identical anyway.
	#endif
		
        if (spriteWorldP->backFrameP != NULL)
        {
            (*spriteWorldP->offscreenDrawProc)(spriteWorldP->backFrameP,
                                          NULL,
                                           &spriteWorldP->backFrameP->frameRect,
                                           &spriteWorldP->backRect);
        }
        else
        {
            struct BK_GL_func *gl = BKLoadGLfunctions();
    
            gl->glClearColor(
                spriteWorldP->backgroundColor.r * (1.0f/255.0f),
                spriteWorldP->backgroundColor.g * (1.0f/255.0f),
                spriteWorldP->backgroundColor.b * (1.0f/255.0f),
                1.0f);
            gl->glClear(GL_COLOR_BUFFER_BIT);
        }
    
    }
    else
#endif
    {
        if (spriteWorldP->backFrameP != NULL)
        {
            (*spriteWorldP->offscreenDrawProc)(spriteWorldP->backFrameP,
                                           spriteWorldP->workFrameP,
                                           &spriteWorldP->backFrameP->frameRect,
                                           &spriteWorldP->workFrameP->frameRect);
		}
		else
		{
			(*spriteWorldP->offscreenDrawProc)(NULL,
											   spriteWorldP->workFrameP,
											   NULL,
											   &spriteWorldP->workFrameP->frameRect);
		} 

    } // OpenGL
    
            // Call the postEraseCallBack
    if (spriteWorldP->postEraseCallBack != NULL)
        (*spriteWorldP->postEraseCallBack)(spriteWorldP);

        // Build the current frame of the animation in the work area
    
    curSpriteLayerP = spriteWorldP->headSpriteLayerP;

        // iterate through the layers in this world
    while (curSpriteLayerP != NULL)
    {
			// Draw the tileLayer that's installed under this SpriteLayer, if there is one.
			// Note: currently we don't have the spriteLayer's visibility affect this
		if (spriteWorldP->tilingIsOn && curSpriteLayerP->tileLayerBelow != -1)
			SWDrawTileLayerInRect( spriteWorldP, curSpriteLayerP->tileLayerBelow, &spriteWorldP->visScrollRect);
		
		if (curSpriteLayerP->layerIsVisible)
		{
			curSpriteP = curSpriteLayerP->headSpriteP;
					
				// iterate through the sprites in this layer
			while (curSpriteP != NULL)
			{
					// it's possible to have frameless sprites
				//if (curSpriteP->curFrameP != NULL)
				//  SW_ASSERT(curSpriteP->curFrameP->isFrameLocked);

				if (curSpriteP->isVisible)
				{
					gSWCurrentElementDrawData = curSpriteP->drawData;

						// draw the sprite in the work area
					
					(*curSpriteP->frameDrawProc)(curSpriteP->curFrameP,
												spriteWorldP->workFrameP,
												&curSpriteP->curFrameP->frameRect,
												&curSpriteP->destFrameRect);
					
					gSWCurrentElementDrawData = NULL;

									
			/*		if (spriteWorldP->tilingIsOn && curSpriteP->tileDepth <= spriteWorldP->lastActiveTileLayer)
					{
						tempRect = curSpriteP->destFrameRect;

							// Clip tempRect to visScrollRect
						if (tempRect.left < spriteWorldP->visScrollRect.left)
							tempRect.left = spriteWorldP->visScrollRect.left;
						if (tempRect.right > spriteWorldP->visScrollRect.right)
							tempRect.right = spriteWorldP->visScrollRect.right;
						if (tempRect.top < spriteWorldP->visScrollRect.top)
							tempRect.top = spriteWorldP->visScrollRect.top;
						if (tempRect.bottom > spriteWorldP->visScrollRect.bottom)
							tempRect.bottom = spriteWorldP->visScrollRect.bottom;

						SWDrawTilesAboveSprite(spriteWorldP, &tempRect, curSpriteP->tileDepth);
					}
			*/

				}
				
					// set the delta and last rect to the current rect
				curSpriteP->deltaFrameRect = curSpriteP->destFrameRect;
				curSpriteP->oldFrameRect = curSpriteP->destFrameRect;

					// this sprite no longer needs to be drawn
				curSpriteP->needsToBeDrawn = false;
				curSpriteP->needsToBeErased = false;

				curSpriteP = curSpriteP->nextSpriteP;
			}
		}
		
			// Draw the tileLayer that's installed under this SpriteLayer, if there is one.
			// Note: currently we don't have the spriteLayer's visibility affect this
		if (spriteWorldP->tilingIsOn && curSpriteLayerP->tileLayerAbove != -1)
			SWDrawTileLayerInRect( spriteWorldP, curSpriteLayerP->tileLayerAbove, &spriteWorldP->visScrollRect);
		
		
		curSpriteLayerP = curSpriteLayerP->nextSpriteLayerP;
    }


        // Call the postDrawCallBack
    if (spriteWorldP->postDrawCallBack != NULL)
        (*spriteWorldP->postDrawCallBack)(spriteWorldP);


        // Copy the work area into the window
    if (updateWindow)
    {
        
#ifdef HAVE_OPENGL
        if (spriteWorldP->useOpenGL)
        {
	#if SW_USE_OLD_SDL
            if (spriteWorldP->glSwapBuffers)
                spriteWorldP->glSwapBuffers(spriteWorldP->glContext);	// SDL 1.2
            else
                SDL_GL_SwapBuffers();
	#else
			SDL_GL_SwapWindow(spriteWorldP->mainWindow);	// SDL 1.3
	#endif
        }
        else
#endif
        (*spriteWorldP->screenDrawProc)(spriteWorldP->workFrameP,
                                        spriteWorldP->screenFrameP,
                                        &spriteWorldP->visScrollRect,
                                        &spriteWorldP->screenFrameP->frameRect);

    }

        // dispose of flagged background rects
    nextRectStructP = spriteWorldP->headUpdateRectP;
    while ( nextRectStructP != NULL )
    {
        curRectStructP = nextRectStructP;
        nextRectStructP = curRectStructP->nextRectStructP;
        free(curRectStructP );
    }
    spriteWorldP->headUpdateRectP = NULL;

        
    spriteWorldP->numTilesChanged = 0;
        
        
        // This is so time-based animations work correctly.
    SWResetMovementTimer(spriteWorldP);

    gSWCurrentSpriteWorld = NULL;

    //SetGWorld( saveGWorld, saveGDevice );
}


//---------------------------------------------------------------------------------------
//  SWProcessSpriteWorld
//---------------------------------------------------------------------------------------

void SWProcessSpriteWorld(
    SpriteWorldPtr spriteWorldP)
{
    register SpriteLayerPtr     curSpriteLayerP;
    double                      pseudoFrameInterval;

    SW_ASSERT(spriteWorldP != NULL);

    gSWCurrentSpriteWorld = spriteWorldP;

    spriteWorldP->runningTimeCount = SWGetMilliseconds(spriteWorldP);

    if ( (spriteWorldP->runningTimeCount - spriteWorldP->timeOfLastFrame >=
                spriteWorldP->fpsTimeInterval) )
    {
       			// Convert milliseconds to microseconds
		spriteWorldP->millisecsSinceLastFrame = (spriteWorldP->runningTimeCount - spriteWorldP->timeOfLastFrame);
		
		if (spriteWorldP->millisecsSinceLastFrame > spriteWorldP->minimumFrameRate)	// minimumFrameInterval
			spriteWorldP->millisecsSinceLastFrame = spriteWorldP->minimumFrameRate;
		
		spriteWorldP->frameHasOccurred = true;
        spriteWorldP->timeOfLastFrame = spriteWorldP->runningTimeCount;
    }
    else
    {
        spriteWorldP->frameHasOccurred = false;
        return;
    }

        // Set the pseudoFrameHasFired variable, for time-based animations. //
    spriteWorldP->pseudoFrameAccum += spriteWorldP->millisecsSinceLastFrame;

    if (spriteWorldP->pseudoFPS >= 0)
        pseudoFrameInterval = spriteWorldP->pseudoFPS;	// 100000 / spriteWorldP->pseudoFPS
    else
        pseudoFrameInterval = spriteWorldP->fpsTimeInterval; // Allow animations that are not time-based to still use this feature.

    if (spriteWorldP->pseudoFrameAccum >= pseudoFrameInterval)
    {
        spriteWorldP->pseudoFrameHasFired = true;
        spriteWorldP->pseudoFrameAccum -= pseudoFrameInterval;
    }
    else
    {
        spriteWorldP->pseudoFrameHasFired = false;
    }

    if ( spriteWorldP->deadSpriteLayerP->headSpriteP != NULL )
    {
        SWFindSpritesToBeRemoved( spriteWorldP );
    }
    
    
        // Call the tileChangeProc, if there is one
    if (spriteWorldP->tileChangeProc != NULL)
    {
        (*spriteWorldP->tileChangeProc)(spriteWorldP);
    }
    
    
        // Set this to false before entering the loop below.
    spriteWorldP->thereAreNonScrollingLayers = false;

    curSpriteLayerP = spriteWorldP->headSpriteLayerP;

        // iterate through the layers in this world
    while (curSpriteLayerP != NULL)
    {
        if (curSpriteLayerP->layerIsNonScrolling)
        {
            spriteWorldP->thereAreNonScrollingLayers = true;
        }
        else
        {
            SWProcessSpriteLayer(spriteWorldP, curSpriteLayerP);
        }
        curSpriteLayerP = curSpriteLayerP->nextSpriteLayerP;
    }

    gSWCurrentSpriteWorld = NULL;
}

#define kDontCorrectForMissedFrames		false

//---------------------------------------------------------------------------------------
//  SWProcessSpriteLayer
//---------------------------------------------------------------------------------------

void SWProcessSpriteLayer(
    SpriteWorldPtr spriteWorldP,
    SpriteLayerPtr curSpriteLayerP)
{
    register SpritePtr          curSpriteP, nextSpriteP;
    register FramePtr           newFrameP;
    double                      moveTimeInterval;

    if (!curSpriteLayerP->isPaused)
    {
        curSpriteP = curSpriteLayerP->headSpriteP;

            // iterate through the sprites in this layer
        while (curSpriteP != NULL)
        {
                // it's possible to have frameless sprites
            //if (curSpriteP->curFrameP != NULL)
            //  SW_ASSERT(curSpriteP->curFrameP->isFrameLocked);

            nextSpriteP = curSpriteP->nextSpriteP;

                // Set the moveProcHasFired variable, for time-based animations. //
            curSpriteP->moveTimeAccum += spriteWorldP->millisecsSinceLastFrame;

            if (curSpriteP->pseudoMoveTimeInterval >= 0)
                moveTimeInterval = curSpriteP->pseudoMoveTimeInterval;
            else if (spriteWorldP->pseudoFPS >= 0)
                moveTimeInterval = 1000 / spriteWorldP->pseudoFPS;
            else
                moveTimeInterval = spriteWorldP->fpsTimeInterval; // Allow animations that are not time-based to still use this feature.

            if (curSpriteP->moveTimeAccum >= moveTimeInterval)
            {
                curSpriteP->moveProcHasFired = true;
                curSpriteP->moveTimeAccum -= moveTimeInterval;
            }
            else
            {
                curSpriteP->moveProcHasFired = false;
            }
			
				// if time of last frame change was too long ago (or zero), reset it
			if (spriteWorldP->runningTimeCount - curSpriteP->timeOfLastFrameChange > 10*curSpriteP->frameTimeInterval)
				curSpriteP->timeOfLastFrameChange = spriteWorldP->runningTimeCount;

                // is it time to advance the sprite's frame?
            while ( curSpriteP->frameTimeInterval >= 0 &&
				 (spriteWorldP->runningTimeCount - curSpriteP->timeOfLastFrameChange) >= curSpriteP->frameTimeInterval )
            {
                register long       currentFrameIndex;

				if (curSpriteP->frameTimeInterval == 0 || kDontCorrectForMissedFrames)
					curSpriteP->timeOfLastFrameChange = spriteWorldP->runningTimeCount;
				else
					curSpriteP->timeOfLastFrameChange += curSpriteP->frameTimeInterval;

                currentFrameIndex  = curSpriteP->curFrameIndex;
                currentFrameIndex += curSpriteP->frameAdvance;

                if (currentFrameIndex < curSpriteP->firstFrameIndex)
                {
                    if (curSpriteP->frameAdvanceMode == kSWWrapAroundMode)
                    {
                            // wrap to the last frame
                        currentFrameIndex = curSpriteP->lastFrameIndex;
                    }
                    else    // curSpriteP->frameAdvanceMode == kSWPatrollingMode
                    {
                            // change direction and set index to what it should be
                        curSpriteP->frameAdvance = -curSpriteP->frameAdvance;
                        currentFrameIndex += curSpriteP->frameAdvance;
                        currentFrameIndex += curSpriteP->frameAdvance;
                    }
                }
                else if (currentFrameIndex > curSpriteP->lastFrameIndex)
                {
                    if (curSpriteP->frameAdvanceMode == kSWWrapAroundMode)
                    {
                            // wrap to the first frame
                        currentFrameIndex = curSpriteP->firstFrameIndex;
                    }
                    else    // curSpriteP->frameAdvanceMode == kSWPatrollingMode
                    {
                            // change direction and set index to what it should be
                        curSpriteP->frameAdvance = -curSpriteP->frameAdvance;
                        currentFrameIndex += curSpriteP->frameAdvance;
                        currentFrameIndex += curSpriteP->frameAdvance;
                    }
                }

                curSpriteP->curFrameIndex = currentFrameIndex;


                    // is there a frame callback?
                if (curSpriteP->frameChangeProc != NULL)
                {
                        // get new frame
                    newFrameP = curSpriteP->frameArray[currentFrameIndex];

                        // call it
                    (*curSpriteP->frameChangeProc)(curSpriteP, newFrameP,
                            &curSpriteP->curFrameIndex);
                }

                    // make sure the new frame index is in range
                if (curSpriteP->curFrameIndex < 0)
                {
                    curSpriteP->curFrameIndex = 0;
                }
                else if (curSpriteP->curFrameIndex >= curSpriteP->maxFrames)
                {
                    curSpriteP->curFrameIndex = curSpriteP->maxFrames - 1;
                }

                    // change the frame
                newFrameP = curSpriteP->frameArray[curSpriteP->curFrameIndex];

                    // has the frame actually changed?
                if (curSpriteP->curFrameP != newFrameP)
                {
                    SWSetCurrentFrameIndex(curSpriteP, curSpriteP->curFrameIndex);
                }
				
				if (curSpriteP->frameTimeInterval == 0)
					break;
            }

                // is it time to move the sprite?
            if ( curSpriteP->moveTimeInterval >= 0 &&
                (spriteWorldP->runningTimeCount - curSpriteP->timeOfLastMove) >= curSpriteP->moveTimeInterval )
            {
                curSpriteP->timeOfLastMove = spriteWorldP->runningTimeCount;

                    // is there a movement callback?
                if (curSpriteP->spriteMoveProc != NULL)
                {
                    (*curSpriteP->spriteMoveProc)(curSpriteP);
                }
            }

            curSpriteP = nextSpriteP;
        }
    }
}


//---------------------------------------------------------------------------------------
//  SWProcessNonScrollingLayers - processes those layers in a scrolling world that need
//  to be moved after the visScrollRect is moved. Also ensures each Sprite's coordinate
//  system remains local to the window, not to the virtual world.
//---------------------------------------------------------------------------------------

void SWProcessNonScrollingLayers(
    SpriteWorldPtr spriteWorldP)
{
    register SpriteLayerPtr     curSpriteLayerP;

        // Exit if there's no work for us to do.
    if (spriteWorldP->thereAreNonScrollingLayers == false)
        return;

    curSpriteLayerP = spriteWorldP->headSpriteLayerP;

        // iterate through the layers in this world
    while (curSpriteLayerP != NULL)
    {
            // Process any non-scrolling layers
        if (curSpriteLayerP->layerIsNonScrolling)
        {
            SWProcessSpriteLayer(spriteWorldP, curSpriteLayerP);
        }

        curSpriteLayerP = curSpriteLayerP->nextSpriteLayerP;
    }

        // We've finished processing all non-scrolling layers for this frame.
    spriteWorldP->thereAreNonScrollingLayers = false;
}


//---------------------------------------------------------------------------------------
//  SWAnimateSpriteWorld
//---------------------------------------------------------------------------------------

void SWAnimateSpriteWorld(SpriteWorldPtr spriteWorldP)
{
    UpdateRectStructPtr         curRectStructP,
                                nextRectStructP;
    register SpriteLayerPtr     curSpriteLayerP;
    register SpritePtr          curSpriteP;
    SpritePtr                   headActiveSpriteP = NULL;   // Tail of active sprite list
    SpritePtr                   headIdleSpriteP = NULL;     // Tail of idle sprite list
    SpritePtr                   curActiveSpriteP = NULL;
    SpritePtr                   curIdleSpriteP = NULL;
    SWRect                      screenDestRect;
    SWRect                      *changedRectP;
    short                       index;
    int                     spriteIsFastMoving;

    SW_ASSERT(spriteWorldP != NULL);
    //SW_ASSERT(spriteWorldP->workFrameP->isFrameLocked);
    //SW_ASSERT(spriteWorldP->screenFrameP->isFrameLocked);
    //if (spriteWorldP->backFrameP != NULL)
    //  SW_ASSERT(spriteWorldP->backFrameP->isFrameLocked);

    gSWCurrentSpriteWorld = spriteWorldP;

    if (!spriteWorldP->frameHasOccurred)
        return;

        // Add the deadSpriteLayer if there are any Sprites in it, so they get erased.
    if ( spriteWorldP->deadSpriteLayerP->headSpriteP != NULL )
    {
        SWAddSpriteLayer(spriteWorldP, spriteWorldP->deadSpriteLayerP);
    }


    //-----------------erase the sprites--------------------

#ifdef HAVE_OPENGL
    if (spriteWorldP->useOpenGL)
    {
		
	#if SW_USE_OLD_SDL
		if (spriteWorldP->glMakeCurrent)
			spriteWorldP->glMakeCurrent(spriteWorldP->glContext);
	#else
		SDL_GL_MakeCurrent(spriteWorldP->mainWindow, spriteWorldP->glContext) ;
	#endif
		
        if (spriteWorldP->backFrameP != NULL)
        {
				// NOTE: SpriteWorld is currently calling THIS instead of the glClearColor below, despite
				// the fact that the effect is the same. TO DO: Get rid of backFrameP if OpenGL is enabled.
			(*spriteWorldP->offscreenDrawProc)(spriteWorldP->backFrameP,
                                           NULL,
                                           &spriteWorldP->backFrameP->frameRect,
											&spriteWorldP->backRect);
        }
        else
        {
            struct BK_GL_func *gl = BKLoadGLfunctions();
			
            gl->glClearColor(
                spriteWorldP->backgroundColor.r * (1.0f/255.0f),
                spriteWorldP->backgroundColor.g * (1.0f/255.0f),
                spriteWorldP->backgroundColor.b * (1.0f/255.0f),
                1.0f);
            gl->glClear(GL_COLOR_BUFFER_BIT);
        }
		
    }
    else
#endif
    {
    
		curSpriteLayerP = spriteWorldP->headSpriteLayerP;

			// iterate through the layers in this world
		while (curSpriteLayerP != NULL)
		{
			if (curSpriteLayerP->layerIsVisible)
			{
				curSpriteP = curSpriteLayerP->headSpriteP;

					// iterate through the sprites in this layer
				while (curSpriteP != NULL)
				{
						// it's possible to have frameless sprites
					//if (curSpriteP->curFrameP != NULL)
					//  SW_ASSERT(curSpriteP->curFrameP->isFrameLocked);

					if ((curSpriteP->needsToBeDrawn && curSpriteP->isVisible) ||
						(curSpriteP->needsToBeErased && !curSpriteP->isVisible))
					{
							// Add sprite to active sprite list
						if (headActiveSpriteP == NULL)
							headActiveSpriteP = curSpriteP;

						if (curActiveSpriteP != NULL)
								curActiveSpriteP->nextActiveSpriteP = curSpriteP;

						curActiveSpriteP = curSpriteP;


							// if necessary, check if sprite is fast moving, otherwise assume it is
						spriteIsFastMoving = false;
						if (curSpriteP->doFastMovingCheck || spriteWorldP->doFastMovingSpritesChecks)
						{
							if (! ((curSpriteP->oldFrameRect.top <= curSpriteP->destFrameRect.bottom) &&
								   (curSpriteP->oldFrameRect.bottom >= curSpriteP->destFrameRect.top) &&
								   (curSpriteP->oldFrameRect.left <= curSpriteP->destFrameRect.right) &&
								   (curSpriteP->oldFrameRect.right >= curSpriteP->destFrameRect.left)))
							{
								spriteIsFastMoving = true;
							}
						}

						if (spriteIsFastMoving)
						{
								// just copy the destRect to the deltaRect
							curSpriteP->deltaFrameRect = curSpriteP->destFrameRect;

								// and add the oldRect to the list of rects to update later
							SWFlagRectAsChanged(spriteWorldP, &curSpriteP->oldFrameRect);
						}
						else
						{   // calculate the deltaRect

								// union last rect and current rect
								// this way is much faster than UnionRect
							curSpriteP->deltaFrameRect.top =
								SW_MIN(curSpriteP->oldFrameRect.top, curSpriteP->destFrameRect.top);
							curSpriteP->deltaFrameRect.left =
								SW_MIN(curSpriteP->oldFrameRect.left, curSpriteP->destFrameRect.left);
							curSpriteP->deltaFrameRect.bottom =
								SW_MAX(curSpriteP->oldFrameRect.bottom, curSpriteP->destFrameRect.bottom);
							curSpriteP->deltaFrameRect.right =
								SW_MAX(curSpriteP->oldFrameRect.right, curSpriteP->destFrameRect.right);

						} // spriteIsFastMoving

						
						if (spriteWorldP->makeTranslucencyAdjustments)
						{
							// copy the back drop piece using the deltaRect
							(*spriteWorldP->offscreenDrawProc)(
								spriteWorldP->backFrameP,
								spriteWorldP->workFrameP,
								&curSpriteP->deltaFrameRect,
								&curSpriteP->deltaFrameRect);
						}
						else
						{
							

							// copy the back drop piece using the oldRect
							(*spriteWorldP->offscreenDrawProc)(
								spriteWorldP->backFrameP,
								spriteWorldP->workFrameP,
								&curSpriteP->oldFrameRect,
								&curSpriteP->oldFrameRect);
						}

					}
					else if (curSpriteP->isVisible)
					{
							// Add sprite to idle sprite list
						if (headIdleSpriteP == NULL)
							headIdleSpriteP = curSpriteP;

						if (curIdleSpriteP != NULL)
							curIdleSpriteP->nextIdleSpriteP = curSpriteP;

						curIdleSpriteP = curSpriteP;
					}

					curSpriteP = curSpriteP->nextSpriteP;
				}
			}

			curSpriteLayerP = curSpriteLayerP->nextSpriteLayerP;
		}

		if (curActiveSpriteP != NULL)
			curActiveSpriteP->nextActiveSpriteP = NULL;

		if (curIdleSpriteP != NULL)
			curIdleSpriteP->nextIdleSpriteP = NULL;

			// update flagged background rects
		curRectStructP = spriteWorldP->headUpdateRectP;
		while ( curRectStructP != NULL )
		{
			(*spriteWorldP->offscreenDrawProc)(
							spriteWorldP->backFrameP,
							spriteWorldP->workFrameP,
							&curRectStructP->updateRect,
							&curRectStructP->updateRect);
			curRectStructP = curRectStructP->nextRectStructP;
		}

    } // OpenGL (the above is done if NOT OpenGL)


        // Call the postEraseCallBack
    if (spriteWorldP->postEraseCallBack != NULL)
        (*spriteWorldP->postEraseCallBack)(spriteWorldP);


    //-----------------draw the sprites--------------------
	
    curSpriteLayerP = spriteWorldP->headSpriteLayerP;

        // iterate through the layers in this world
    while (curSpriteLayerP != NULL)
    {
			// Draw the tileLayer that's installed under this SpriteLayer, if there is one.
			// Note: currently we don't have the spriteLayer's visibility affect this
		if (spriteWorldP->tilingIsOn && curSpriteLayerP->tileLayerBelow != -1)
		{
			SWDrawTileLayerInRect( spriteWorldP, curSpriteLayerP->tileLayerBelow, &spriteWorldP->visScrollRect);
		}
		
		if (curSpriteLayerP->layerIsVisible)
		{
			curSpriteP = curSpriteLayerP->headSpriteP;

				// iterate through the sprites in this layer
			while (curSpriteP != NULL)
			{
				if (curSpriteP->isVisible)
				{
					if (curSpriteP->needsToBeDrawn || spriteWorldP->useOpenGL)
					{
						gSWCurrentElementDrawData = curSpriteP->drawData;

							// draw the sprite in the work area
						(*curSpriteP->frameDrawProc)(
								curSpriteP->curFrameP,
								spriteWorldP->workFrameP,
								&curSpriteP->curFrameP->frameRect,
								&curSpriteP->destFrameRect);
						
						gSWCurrentElementDrawData = NULL;
											
											
				/*		if (spriteWorldP->tilingIsOn &&
							curSpriteP->tileDepth <= spriteWorldP->lastActiveTileLayer)
						{
							screenDestRect = curSpriteP->destFrameRect;

								// Clip screenDestRect to visScrollRect
							if (screenDestRect.left < spriteWorldP->visScrollRect.left)
								screenDestRect.left = spriteWorldP->visScrollRect.left;
							if (screenDestRect.right > spriteWorldP->visScrollRect.right)
								screenDestRect.right = spriteWorldP->visScrollRect.right;
							if (screenDestRect.top < spriteWorldP->visScrollRect.top)
								screenDestRect.top = spriteWorldP->visScrollRect.top;
							if (screenDestRect.bottom > spriteWorldP->visScrollRect.bottom)
								screenDestRect.bottom = spriteWorldP->visScrollRect.bottom;

							SWDrawTilesAboveSprite(spriteWorldP, &screenDestRect, curSpriteP->tileDepth);
						}
				*/
					}
				/*	else
					{
							// some sprites don't have frames
						if (curSpriteP->curFrameP != NULL)
							SWCheckIdleSpriteOverlap(spriteWorldP, curSpriteP, headActiveSpriteP);
					}
				*/
				
				
						// update deltaFrameRect and oldFrameRect values that Midnight Mansion uses directly at times
						// Since we are NOT updating these valuues when updating the screen (since it'sjust a single OpenGL call)
						// we must do it here, now!
					if (spriteWorldP->useOpenGL)
					{
							// set the delta and last rect to the current rect
						curSpriteP->deltaFrameRect = curSpriteP->destFrameRect;
						curSpriteP->oldFrameRect = curSpriteP->destFrameRect;
						
								// this sprite no longer needs to be drawn
						curSpriteP->needsToBeDrawn = false;
						curSpriteP->needsToBeErased = false;
					}
				}

				curSpriteP = curSpriteP->nextSpriteP;
			}
		}
		
			// Draw the tileLayer that's installed above this SpriteLayer, if there is one.
			// Note: currently we don't have the spriteLayer's visibility affect this
		if (spriteWorldP->tilingIsOn && curSpriteLayerP->tileLayerAbove != -1)
		{
			SWDrawTileLayerInRect( spriteWorldP, curSpriteLayerP->tileLayerAbove, &spriteWorldP->visScrollRect);
		}

        curSpriteLayerP = curSpriteLayerP->nextSpriteLayerP;
    }


        // Call the postDrawCallBack
    if (spriteWorldP->postDrawCallBack != NULL)
        (*spriteWorldP->postDrawCallBack)(spriteWorldP);

	
    //-----------------update the screen--------------------

        // the current port should be the one in which we are drawing
    //SWSetPortToFrame( spriteWorldP->screenFrameP );
	
        // wait for the VBL
    //SWWaitForVBL(spriteWorldP);

#ifdef HAVE_OPENGL
    if (spriteWorldP->useOpenGL)
    {
		
	#if SW_USE_OLD_SDL
			// SDL 1.2 way of doing it:
		if (spriteWorldP->glSwapBuffers)
                spriteWorldP->glSwapBuffers(spriteWorldP->glContext);
            else
                SDL_GL_SwapBuffers();
	#else
		SDL_GL_SwapWindow(spriteWorldP->mainWindow);
	#endif
	
    }
    else
#endif
    {		// The following is NOT done with OpenGL:
			
		curSpriteP = headActiveSpriteP;


		// Update on screen the tiles that have changed
		changedRectP = spriteWorldP->changedTiles;
		for (index = 0; index < spriteWorldP->numTilesChanged; index++, changedRectP++)
		{
			SWMergeUpdateRect(spriteWorldP, *changedRectP, NULL);

			//(*spriteWorldP->screenDrawProc)(spriteWorldP->workFrameP,
			//  spriteWorldP->screenFrameP, changedRectP, changedRectP);
		}
				   
				// update the sprites on the screen
		while (curSpriteP != NULL)
		{
			SWMergeUpdateRect(spriteWorldP, curSpriteP->deltaFrameRect, NULL);

			//(*spriteWorldP->screenDrawProc)(
			//      spriteWorldP->workFrameP,
			//      spriteWorldP->screenFrameP,
			//      &curSpriteP->deltaFrameRect,
			//      &curSpriteP->deltaFrameRect);

				// set the delta and last rect to the current rect
			curSpriteP->deltaFrameRect = curSpriteP->destFrameRect;
			curSpriteP->oldFrameRect = curSpriteP->destFrameRect;
					// this sprite no longer needs to be drawn
			curSpriteP->needsToBeDrawn = false;
			curSpriteP->needsToBeErased = false;
			curSpriteP = curSpriteP->nextActiveSpriteP;
		}


			// Has a custom worldRect been used? (If so, sprites need offsetting)
		if (spriteWorldP->screenRect.left || spriteWorldP->screenRect.top)
		{
				// update flagged background rects
			curRectStructP = spriteWorldP->headUpdateRectP;
			while ( curRectStructP != NULL )
			{
				screenDestRect = curRectStructP->updateRect;
				screenDestRect.left += spriteWorldP->screenRect.left;
				screenDestRect.right += spriteWorldP->screenRect.left;
				screenDestRect.top += spriteWorldP->screenRect.top;
				screenDestRect.bottom += spriteWorldP->screenRect.top;
				(*spriteWorldP->screenDrawProc)(
								spriteWorldP->workFrameP,
								spriteWorldP->screenFrameP,
								&curRectStructP->updateRect,
								&screenDestRect);
				curRectStructP = curRectStructP->nextRectStructP;
			}

		}
		else
		{
				// update flagged background rects
			curRectStructP = spriteWorldP->headUpdateRectP;
			while ( curRectStructP != NULL )
			{
				(*spriteWorldP->screenDrawProc)(
								spriteWorldP->workFrameP,
								spriteWorldP->screenFrameP,
								&curRectStructP->updateRect,
								&curRectStructP->updateRect);
				curRectStructP = curRectStructP->nextRectStructP;
			}
		}
    
    } // OpenGL
    
        // dispose of flagged background rects
    nextRectStructP = spriteWorldP->headUpdateRectP;
    while ( nextRectStructP != NULL )
    {
        curRectStructP = nextRectStructP;
        nextRectStructP = curRectStructP->nextRectStructP;
        free(curRectStructP );
    }
    spriteWorldP->headUpdateRectP = NULL;

        
    spriteWorldP->numTilesChanged = 0;
        
        
        // Remove the deadSpriteLayer if we added it earlier.
    if ( spriteWorldP->deadSpriteLayerP->headSpriteP != NULL )
    {
        SWRemoveSpriteLayer(spriteWorldP, spriteWorldP->deadSpriteLayerP);
    }

    gSWCurrentSpriteWorld = NULL;
}


//---------------------------------------------------------------------------------------
//  SWCheckIdleSpriteOverlap - deprecated due to OpenGL redrawing everything every frame
//---------------------------------------------------------------------------------------
#pragma mark SWCheckIdleSpriteOverlap
/*
void SWCheckIdleSpriteOverlap(
    SpriteWorldPtr  spriteWorldP,
    SpritePtr       idleSpriteP,
    SpritePtr       headActiveSpriteP)
{
    register SpritePtr      activeSpriteP = headActiveSpriteP;
    UpdateRectStructPtr     curRectStructP;
    SWRect  frameLocalRectCollection;
    SWRect  rectCollection = {0, 0, 0, 0};
    SWRect  *changedRectP;
    short   index;

        // iterate through the active sprites
    while (activeSpriteP != NULL)
    {
        SWCollectIdleSpriteIntersectingRects (idleSpriteP->oldFrameRect, activeSpriteP->deltaFrameRect, &rectCollection);
        activeSpriteP = activeSpriteP->nextActiveSpriteP;
    }
    
        // iterate through changed tiles
    if (spriteWorldP->numTilesChanged > 0) {
        changedRectP = spriteWorldP->changedTiles;
        for (index = 0; index < spriteWorldP->numTilesChanged; index++, changedRectP++)
        {
            SWCollectIdleSpriteIntersectingRects (idleSpriteP->oldFrameRect, *changedRectP, &rectCollection);
        }
    }
        
        
        // iterate through updateRects
    if (spriteWorldP->headUpdateRectP != NULL)
    {
        curRectStructP = spriteWorldP->headUpdateRectP;
        while (curRectStructP != NULL)
        {
            changedRectP = &curRectStructP->updateRect;
            SWCollectIdleSpriteIntersectingRects (idleSpriteP->oldFrameRect, curRectStructP->updateRect, &rectCollection);
            curRectStructP = curRectStructP->nextRectStructP;
        }
    }

        // if the collection now encloses anything use it to redraw the idle sprite
    if (! (rectCollection.left == 0 &&
             rectCollection.right == 0 &&
             rectCollection.top == 0 &&
             rectCollection.bottom == 0))
    {
            // determine the intersection rect local to the idle sprite's frame
        frameLocalRectCollection = idleSpriteP->curFrameP->frameRect;

        frameLocalRectCollection.left += (rectCollection.left - idleSpriteP->destFrameRect.left);
        frameLocalRectCollection.top += (rectCollection.top - idleSpriteP->destFrameRect.top);
        frameLocalRectCollection.right -= (idleSpriteP->destFrameRect.right - rectCollection.right);
        frameLocalRectCollection.bottom -= (idleSpriteP->destFrameRect.bottom - rectCollection.bottom);

            // copy a piece of the sprite image onto the workFrame

        gSWCurrentElementDrawData = idleSpriteP->drawData;

        (*idleSpriteP->frameDrawProc)(
                idleSpriteP->curFrameP,
                spriteWorldP->workFrameP,
            &frameLocalRectCollection,
            &rectCollection);

        gSWCurrentElementDrawData = NULL;
                
        if (spriteWorldP->tilingIsOn &&
            idleSpriteP->tileDepth <= spriteWorldP->lastActiveTileLayer)
        {
                // Clip dstSectRect to visScrollRect
            if (rectCollection.left < spriteWorldP->visScrollRect.left)
                rectCollection.left = spriteWorldP->visScrollRect.left;
            if (rectCollection.right > spriteWorldP->visScrollRect.right)
                rectCollection.right = spriteWorldP->visScrollRect.right;
            if (rectCollection.top < spriteWorldP->visScrollRect.top)
                rectCollection.top = spriteWorldP->visScrollRect.top;
            if (rectCollection.bottom > spriteWorldP->visScrollRect.bottom)
                rectCollection.bottom = spriteWorldP->visScrollRect.bottom;

            SWDrawTilesAboveSprite(spriteWorldP, &rectCollection, idleSpriteP->tileDepth);
        }
                
    }
}
*/

//---------------------------------------------------------------------------------------
//  SWCollectIntersectingRects - find the minimum rect that encloses the current
// enclosing rect and the intersection of checkRect1 and checkRect2
//---------------------------------------------------------------------------------------

void SWCollectIdleSpriteIntersectingRects(
    SWRect checkRect1,
    SWRect checkRect2,
    SWRect *enclosingRect)
{
    SWRect newEnclosingRect;
    SWRect intersectionRect;
    int enclosingRectNeedsInitialization;

        // determine if we need to initialize the collection
    if ((enclosingRect->left == 0) && (enclosingRect->right == 0) &&
        (enclosingRect->top == 0) && (enclosingRect->bottom == 0))
    {
        enclosingRectNeedsInitialization = true;
    }
    else
    {
        enclosingRectNeedsInitialization = false;
    }
        
        // do the rects intersect?
    if ((checkRect1.top < checkRect2.bottom) &&
         (checkRect1.bottom > checkRect2.top) &&
         (checkRect1.left < checkRect2.right) &&
         (checkRect1.right > checkRect2.left))
    {

            // calculate the intersection between the two rects
        intersectionRect.left =
                SW_MAX(checkRect1.left, checkRect2.left);
        intersectionRect.top =
                SW_MAX(checkRect1.top, checkRect2.top);
        intersectionRect.right =
                SW_MIN(checkRect1.right, checkRect2.right);
        intersectionRect.bottom =
                SW_MIN(checkRect1.bottom, checkRect2.bottom);

        if (!enclosingRectNeedsInitialization)
        {
                // caclulate the smallest rect which encloses all the intersection
                // rects found so far
            newEnclosingRect.left =
                    SW_MIN(enclosingRect->left, intersectionRect.left);
            newEnclosingRect.top =
                    SW_MIN(enclosingRect->top, intersectionRect.top);
            newEnclosingRect.right =
                    SW_MAX(enclosingRect->right, intersectionRect.right);
            newEnclosingRect.bottom =
                    SW_MAX(enclosingRect->bottom, intersectionRect.bottom);
        }
        else
        {
                // initialize the enclosing rect with the first intersection
            enclosingRectNeedsInitialization = false;
            newEnclosingRect = intersectionRect;
        }

        *enclosingRect = newEnclosingRect;

    }
}

//---------------------------------------------------------------------------------------
//  SWFindSpritesToBeRemoved
//---------------------------------------------------------------------------------------

void SWFindSpritesToBeRemoved(
    SpriteWorldPtr spriteWorldP )
{
    SpritePtr       curSpriteP, nextSpriteP;

    curSpriteP = spriteWorldP->deadSpriteLayerP->headSpriteP;

        // iterate through the sprites in the deadSpriteLayerP
    while (curSpriteP != NULL)
    {
        nextSpriteP = curSpriteP->nextSpriteP;

            // all the Sprites here should need removing...
        SW_ASSERT(curSpriteP->spriteRemoval != kSWDontRemoveSprite);

            // has this sprite been erased by SWAnimate?
        if (curSpriteP->needsToBeErased == false)
        {
                // if so, we can safely remove it from its layer
            SWRemoveSprite(curSpriteP);

                // do we want to dispose of this sprite too?
            if (curSpriteP->spriteRemoval == kSWRemoveAndDisposeSprite)
                SWDisposeSprite(&curSpriteP);
            else
                curSpriteP->spriteRemoval = kSWDontRemoveSprite;    // We're done
        }

        curSpriteP = nextSpriteP;
    }
}


//---------------------------------------------------------------------------------------
//  SWFlagRectAsChanged
//---------------------------------------------------------------------------------------

SWError SWFlagRectAsChanged(
    SpriteWorldPtr spriteWorldP,
    SWRect* theChangedRect)
{
    SWError                 err = kNoError;

    err = SWMergeUpdateRect(spriteWorldP, *theChangedRect, 0);

    return err;
}


/*
//---------------------------------------------------------------------------------------
//  SWFlagScrollingRectAsChanged - same as SWFlagRectAsChanged, minus alignment code

//---------------------------------------------------------------------------------------

SWError SWFlagScrollingRectAsChanged(
    SpriteWorldPtr spriteWorldP,
    SWRect* theChangedRect)
{
    return SWFlagRectAsChanged(spriteWorldP, theChangedRect);
}

*/

//---------------------------------------------------------------------------------------
//  SWMergeUpdateRect - merge the rect with others in the list, then add
//---------------------------------------------------------------------------------------

SWError SWMergeUpdateRect(
    SpriteWorldPtr spriteWorldP,
    SWRect  theChangedRect,
    int *countP)
{
    SWError                 err = kNoError;
    UpdateRectStructPtr     curRectStructP, newUpdateRectP;
    int                     stillNeedsMerge = true;
    int                     count = 0;
    
    newUpdateRectP = (UpdateRectStructPtr) calloc(1,sizeof(UpdateRectStruct));
    if ( newUpdateRectP == NULL )
    {
        err = kMemoryAllocationError;
        SWSetStickyIfError(err);
    }
    
    //only try to optimize if optimizations are on and there are no errors
    if (err == kNoError && spriteWorldP->useUpdateRectOptimizations)
    {
        //will loop until there are no collisions with the update rect
        //will just fall (not fail) through if there is no head update rect
        while (stillNeedsMerge) 
        {
            stillNeedsMerge = false;

            //go through each struct
            curRectStructP = spriteWorldP->headUpdateRectP; 
            while(curRectStructP != NULL)
            {
                //check for intersection with other rects
                if ( SW_RECT_IS_IN_RECT(theChangedRect, curRectStructP->updateRect) )
                {
                    count ++;

                    //union the rects
                    SW_UNION_RECT(theChangedRect, curRectStructP->updateRect);

                    //remove the other rect from the list
                    SWRemoveUpdateRectStruct(spriteWorldP, curRectStructP);
                    
                    //we need to restart the checking, break out of this iteration
                    stillNeedsMerge = true;
                    break;
                }
                
                //next link
                curRectStructP = curRectStructP->nextRectStructP;
            }//end while each struct
        }//end while stillneedsmerge

    }

    if (err == kNoError)
    {
        //now add this uber rect to the list

        newUpdateRectP->updateRect = theChangedRect;
        newUpdateRectP->nextRectStructP = NULL;

        if ( spriteWorldP->headUpdateRectP == NULL )
        {
            spriteWorldP->headUpdateRectP = newUpdateRectP;
        }
        else
        {
            curRectStructP = spriteWorldP->headUpdateRectP;
            while ( curRectStructP->nextRectStructP != NULL )
            {
                curRectStructP = curRectStructP->nextRectStructP;
            }
            curRectStructP->nextRectStructP = newUpdateRectP;
        }

    }//end if no error

    //see if the number of merges result is wanted
    if (countP)
    {   
        *countP = count;
    }

    return err;
}


//---------------------------------------------------------------------------------------
//  SWRemoveUpdateRectStruct - takes out a specific updaterectstruct 
//                              (used by SWMergeUpdateRect)
//---------------------------------------------------------------------------------------

void SWRemoveUpdateRectStruct(
    SpriteWorldPtr spriteWorldP,
    UpdateRectStructPtr removeRect)
{
    UpdateRectStructPtr     curRectStructP;

    if (removeRect ==  spriteWorldP->headUpdateRectP)
    {
        spriteWorldP->headUpdateRectP = removeRect->nextRectStructP;
    }
    else
    {
        curRectStructP = spriteWorldP->headUpdateRectP;
        while (curRectStructP)
        {
            if (curRectStructP->nextRectStructP == removeRect)
            {
                curRectStructP->nextRectStructP = removeRect->nextRectStructP;
                break;
            }
            curRectStructP = curRectStructP->nextRectStructP;
        }
    }

    free(removeRect);
}


//---------------------------------------------------------------------------------------
// SWGetMilliseconds - Returns a TimerContext's runningTimeCount; that is, the time
// in milliseconds (1/1000 sec) that have passed since this timer first started.
//---------------------------------------------------------------------------------------

unsigned long SWGetMilliseconds(SpriteWorldPtr spriteWorldP)
{
    SW_UNUSED(spriteWorldP);
    return SDL_GetTicks();
}


///--------------------------------------------------------------------------------------
// SWGetElapsedMilliseconds - return the time in microseconds that have passed since the previous frame
///--------------------------------------------------------------------------------------

unsigned long SWGetElapsedMilliseconds(SpriteWorldPtr spriteWorldP)
{
    return spriteWorldP->millisecsSinceLastFrame;
}



//---------------------------------------------------------------------------------------
//  SWResetMovementTimer - when doing time-based animation, call this function after pausing
//  the animation (such as when the user pauses, or when moving between levels) so that
//  sprites don't "jump" forward the next time they are animated.
//---------------------------------------------------------------------------------------

void SWResetMovementTimer(
    SpriteWorldPtr spriteWorldP)
{
    SW_UNUSED(spriteWorldP);
    //Microseconds( &spriteWorldP->lastMicrosecondsB );
}


//---------------------------------------------------------------------------------------
//  SWHasPseudoFrameFired
//---------------------------------------------------------------------------------------

short SWHasPseudoFrameFired(
    SpriteWorldPtr spriteWorldP)
{
    SW_ASSERT(spriteWorldP != NULL);

    return spriteWorldP->pseudoFrameHasFired;
}


//---------------------------------------------------------------------------------------
// SWGetCurrentFPS
//---------------------------------------------------------------------------------------

float SWGetCurrentFPS(SpriteWorldPtr spriteWorldP)
{
    unsigned long   frameTime;

    frameTime = SWGetMilliseconds(spriteWorldP) - spriteWorldP->timeOfLastFrame;

    return 1000.0f / (float) frameTime;
}


//---------------------------------------------------------------------------------------
//  SWSetCleanUpSpriteWorld - call this if you want FatalError() or SWAssertFail() to call
//  SWSyncSpriteWorldToVBL with a value of false for your SpriteWorld before it calls
//  ExitToShell. You pass a pointer to the SpriteWorld you want to be "cleaned up".
//---------------------------------------------------------------------------------------

void SWSetCleanUpSpriteWorld(
    SpriteWorldPtr spriteWorldP)
{
    gSWCleanUpSpriteWorldP = spriteWorldP;
}


//---------------------------------------------------------------------------------------
//  SWSetCleanUpFunction - installs a callback that is called whenever a fatal error or
//  assertion failure occurrs. Very useful for fading the screen back in, or doing any
//  other important tasks that must be done before quitting when a fatal error occurrs.
//  Your callback must be of this format:   void    MyCallBack(void).
//---------------------------------------------------------------------------------------

void SWSetCleanUpFunction(
    CleanUpCallBackPtr callBackP)
{
    gSWCleanUpCallBackP = callBackP;
}


//---------------------------------------------------------------------------------------
//  SWGetSpriteWorldVersion - use the following in the 5th digit from the left:
//
//  developStage        = 0x20,
//  alphaStage          = 0x40,
//  betaStage           = 0x60,
//  finalStage          = 0x80
//---------------------------------------------------------------------------------------

unsigned long SWGetSpriteWorldVersion(void)
{
    return 0x00504001;      // i.e. 0x00504001 would be "0.5 alpha 1"
}


//---------------------------------------------------------------------------------------
//  SWGetSpriteWorldPixelDepth
//---------------------------------------------------------------------------------------

short SWGetSpriteWorldPixelDepth( SpriteWorldPtr spriteWorldP )
{
    SW_ASSERT(spriteWorldP != NULL);
    return spriteWorldP->pixelDepth;
}


#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------------------
//  SWClearStickyError
//---------------------------------------------------------------------------------------

void SWClearStickyError(void)
{
    gSWStickyError = kNoError;
}


//---------------------------------------------------------------------------------------
//  SWStickyError
//---------------------------------------------------------------------------------------

SWError SWStickyError(void)
{
    return gSWStickyError;
}


//---------------------------------------------------------------------------------------
//  SWSetStickyIfError
//---------------------------------------------------------------------------------------

void SWSetStickyIfError(SWError errNum)
{
    if ( errNum != kNoError )
        gSWStickyError = errNum;
}


//---------------------------------------------------------------------------------------
// SWAssertFail - called when the expression in SW_ASSERT() is false.
//---------------------------------------------------------------------------------------

void SWAssertFail(char *filename, char *functionName, int lineNum)
{
    fprintf(stderr, "\nASSERT FAILED: \n File: %s \nfunction:%s \nline: %d\n", filename,functionName, lineNum);
    exit(1);
}

