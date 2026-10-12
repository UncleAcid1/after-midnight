///--------------------------------------------------------------------------------------
// Simple.c
///--------------------------------------------------------------------------------------


#include <SWIncludes.h>         // Automatically include all SpriteWorld headers
#include <SWUtils.h>

#include <BlitKernel.h>

#include "Simple.h"

#define kMaxFPS                 0         // Keep the animation from going too fast

#define kWindowWidth            640		// UNUSED.  See "Simple.ini"
#define kWindowHeight           480		// UNUSED. See "Simple.ini"
#define kDepth					32
#define kFullscreen				false

#define kCollisions				false

#define kNumSprites				10	// Number of balls
#define kMaxSpriteMoveDelta     2

/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <SDL/SDL_opengl.h>

SpriteWorldPtr      gSpriteWorldP = NULL;
SpriteLayerPtr      gSpriteLayerP = NULL;
SpritePtr           gMasterBallSpriteP = NULL;

char*               gSpriteFilename;
SWBoolean           gCollisions = kCollisions;

void LoadFont();

///--------------------------------------------------------------------------------------
// main
///--------------------------------------------------------------------------------------


int main(int argc, char *argv[])
{
    Randomize();

    SetUpSpriteWorld();
    CreateBallSprite();
	
    AddSprites( gMasterBallSpriteP, kNumSprites );
    RunAnimation();
    CleanUp();
    
    return 0;
}


///--------------------------------------------------------------------------------------
//  SetUpSpriteWorld
///--------------------------------------------------------------------------------------

void    SetUpSpriteWorld(void)
{
    SWError         err;
	SDL_Color       bg = { 100, 100 ,150 };
//    SDL_Color       bg = { 0, 0 ,150 };
    SWPropertiesPtr prop;
    int             width;
    int             height;
    int             depth;
    char*           sprite;
	Uint32			color;
    SWBoolean       fullscreen;
    SWBoolean       opengl;
    
        // Initialize the SpriteWorld package
    err = SWEnterSpriteWorld( true );
    FatalError(err);
    
    err = SWCreateProperties(&prop);
    FatalError(err);
    err = SWLoadProperties(prop, "Simple.ini");
    if (err != kNoError) fprintf(stderr, "SWLoadProperties: %d\n", err);
    width = SWGetPropertyInteger(prop, "width", kWindowWidth);
    height = SWGetPropertyInteger(prop, "height", kWindowHeight);
    depth = SWGetPropertyInteger(prop, "depth", kDepth);
    fullscreen = SWGetPropertyBoolean(prop, "fullscreen", kFullscreen);
    opengl = SWGetPropertyBoolean(prop, "opengl", true);
    sprite = SWGetPropertyString(prop, "sprite", NULL);
    if (sprite)
    {
        gSpriteFilename = malloc(strlen(sprite)+1);
        if (!gSpriteFilename) exit(1);
        strcpy(gSpriteFilename, sprite);
    }
    gCollisions = SWGetPropertyBoolean(prop, "collisions", gCollisions);
    SWDisposeProperties(&prop);
    
        // Create the SpriteWorld
#if SDL_VERSION_ATLEAST(1,3,0)
    err = SWCreateSpriteWorld(&gSpriteWorldP, "Simple", width, height, depth, fullscreen, opengl);
#else
    err = SWCreateSpriteWorldUsingOldSDL(&gSpriteWorldP, width, height, depth, fullscreen, opengl);
#endif

    FatalError(err);

    SW_ASSERT(gSpriteWorldP != NULL);

        // Create the Sprite Layers
    err = SWCreateSpriteLayer(&gSpriteLayerP);
    FatalError(err);
    
        // Put the pieces together and lock the SpriteWorld. 
        // Note: since we are locking the SpriteWorld before adding the sprites,
        // we must make sure to lock each sprite individually when creating them.
    SWAddSpriteLayer(gSpriteWorldP, gSpriteLayerP);
    SWLockSpriteWorld(gSpriteWorldP);
	
    
    SWSetBackgroundColor(gSpriteWorldP, bg); // dark blue
	
		// THIS CURRENTLY HAS NO EFFECT.
	color = SDL_MapRGB( gSpriteWorldP->backFrameP->originalSurfaceP->format, bg.r, bg.g ,bg.b );
    SDL_FillRect(
        gSpriteWorldP->backFrameP->originalSurfaceP, //frameSurfaceP, originalSurfaceP
        NULL, color);
    SWUpdateFrame(gSpriteWorldP->backFrameP);
    
        // Limit the frame rate to a specific speed
    SWSetSpriteWorldMaxFPS(gSpriteWorldP, kMaxFPS);
	
//	SWSetPostDrawCallBack( gSpriteWorldP, MyPostDrawCallback );
}


///--------------------------------------------------------------------------------------
//  CreateBallSprite - load the master ball sprite and prepare it for cloning later
///--------------------------------------------------------------------------------------

void    CreateBallSprite(void)
{
    SWError err;
    
//    err = SWCreateSpriteFromFile( &gMasterBallSpriteP, NULL, gSpriteFilename, 1 );
//    FatalError(err);
	
	/* err = SWCreateSpriteFromSingleFileXY( &gMasterBallSpriteP,
										    NULL,
										   gSpriteFilename,
										   128, 128,
										   0,
										   0,
										   0,
										   2 );*/
	
	err = SWCreateSpriteFromSingleFile( &gMasterBallSpriteP, NULL, gSpriteFilename, 128, 0 );
	
	FatalError(err);
    
        // Set up the sprite (remember that it must be locked!)
    SWLockSprite(gMasterBallSpriteP);
    SWSetSpriteMoveBounds(gMasterBallSpriteP, &gSpriteWorldP->backRect);
    SWSetSpriteMoveProc(gMasterBallSpriteP, BallSpriteMoveProc);
}


///--------------------------------------------------------------------------------------
//  AddSprites - clone the master sprite, and add the clones to the SpriteWorld
///--------------------------------------------------------------------------------------

void    AddSprites( SpritePtr srcSpriteP, int count )
{
    SpritePtr   newSpriteP;
    short       spriteNum;
    SWError     err;
    
    for (spriteNum = 0; spriteNum < count; spriteNum++)
    {
            // Clone the master sprite. The clone doesn't need to be locked,
            // since the master sprite was already locked.
        err = SWCloneSprite(srcSpriteP, &newSpriteP, NULL);
        FatalError(err);
        
        SWAddSprite(gSpriteLayerP, newSpriteP);
        
        SWSetSpriteLocation(newSpriteP,
                GetRandom(0, gSpriteWorldP->backRect.right-44), 
                GetRandom(0, gSpriteWorldP->backRect.bottom-44) );
		
		
        if (gCollisions)
			SWSetSpriteCollideProc(newSpriteP, BallSpriteBounceProc);

        do
        {
            SWSetSpriteMoveDelta(newSpriteP,
                    GetRandom(-kMaxSpriteMoveDelta, kMaxSpriteMoveDelta), 
                    GetRandom(-kMaxSpriteMoveDelta, kMaxSpriteMoveDelta));
        } while (newSpriteP->drawData->horizMoveDelta == 0 || newSpriteP->drawData->vertMoveDelta == 0);
    }
}


///--------------------------------------------------------------------------------------
//  RunAnimation
///--------------------------------------------------------------------------------------

void    RunAnimation(void)
{   
    unsigned long then, now, frames = 0;
	int done = false;
    SDL_Event event;

    SWUpdateSpriteWorld(gSpriteWorldP, true);
    
    then = SDL_GetTicks();
    
    do
    {
		while (SDL_PollEvent(&event)) 
            MyHandleSDLEvent(&event, &done);
		
		SWProcessSpriteWorld(gSpriteWorldP);
		
		if (gSpriteWorldP->frameHasOccurred)
		{
			if (gCollisions)
				SWCollideSpriteLayer(gSpriteWorldP, gSpriteLayerP, gSpriteLayerP);

			SWAnimateSpriteWorld(gSpriteWorldP);
			
			frames++;
		}

    } while (!done); //( ! SDL_GetMouseState( NULL, NULL ) );
    
    now = SDL_GetTicks();
    if ( now > then ) 
	{
        printf("%2.2f frames per second\n",
				((double)frames*1000)/(now-then));
    }

}



///--------------------------------------------------------------------------------------
//  CleanUp - This function is not really necessary, since the system will dispose
//  everything automatically when the program quits. However, if you sync the animation
//  to the VBL, you must either call SWDisposeSpriteWorld, or SWSyncSpriteWorldToVBL
//  with a value of false before your program quits, in order to remove the VBL task.
///--------------------------------------------------------------------------------------

void    CleanUp(void)
{
        // Dispose the master sprite, since it isn't included in the SpriteWorld
    SWDisposeSprite(&gMasterBallSpriteP);
    
        // Dispose the SpriteWorld, including all sprites and layers currently in it
    SWDisposeSpriteWorld(&gSpriteWorldP);
    SWExitSpriteWorld();
}


///--------------------------------------------------------------------------------------
//  BallSpriteMoveProc
///--------------------------------------------------------------------------------------

void BallSpriteMoveProc(SpritePtr ballSpriteP)
{   
    SWOffsetSprite(ballSpriteP, ballSpriteP->drawData->horizMoveDelta, ballSpriteP->drawData->vertMoveDelta);
    (void)SWBounceSprite(ballSpriteP);
	
	if (ballSpriteP->frameTimeInterval == -1)
		SWSetCurrentFrameIndex( ballSpriteP, 0 ); // Make it non-purple
}


///--------------------------------------------------------------------------------------
//  BallSpriteBounceProc -- the collision proc
///--------------------------------------------------------------------------------------

void BallSpriteBounceProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect* sectRect)
{
    if (srcSpriteP == dstSpriteP) 
		return;
	
	if (srcSpriteP->frameTimeInterval != -1 || dstSpriteP->frameTimeInterval != -1)
		return;
	
		// WARNING: SWPixelCollision is VERY slow if it needs to check beyond the rectangle overlap stage, 
		// and will SIGNIFICANTLY slow down an app with very many collision tests!
		//
		// It CAN be optimized by making a special-case method for 32-bits, Alpha channel surface format.
    if (!SWPixelCollision(srcSpriteP,dstSpriteP)) 
		return;

    SWSetCurrentFrameIndex( srcSpriteP, 1 ); // Make it purple while colliding
	SWSetCurrentFrameIndex( dstSpriteP, 1 ); // Make it purple while colliding
}


///--------------------------------------------------------------------------------------
//  MyHandleSDLEvent
///--------------------------------------------------------------------------------------

void MyHandleSDLEvent(SDL_Event * event, SWBoolean *done)
{
    SWBoolean wasHandled;
	
	switch (event->type) 
	{
#if SDL_VERSION_ATLEAST(1,3,0)
		case SDL_WINDOWEVENT:
		switch (event->window.event) 
		{
			case SDL_WINDOWEVENT_CLOSE:
				*done = true;
				break;
				
				// NOTE: Doesn't work properly (at least not on Mac)
				// Generates aa Enter and Leave event *every single movement* the mouse makes!
			case SDL_WINDOWEVENT_ENTER:
			//	printf("Mouse entered our window! windowID = %d \n", event->window.windowID);
				break;
			
			case SDL_WINDOWEVENT_LEAVE:
			//	printf("Mouuse left the window! windowID = %d \n", event->window.windowID);
				break;
				
			case SDL_WINDOWEVENT_RESIZED:
			/*
				SDL_GL_DeleteContext(gSpriteWorldP->glContext);
				gSpriteWorldP->glContext = SDL_GL_CreateContext(gSpriteWorldP->mainWindow);
			
				SDL_GL_MakeCurrent( gSpriteWorldP->mainWindow, gSpriteWorldP->glContext); // Not necessary... CreateContext does this
			*/
				
				
				
				
				
				BKFlatGLSetup( event->window.data1, event->window.data2 );
				
					// DOESN'T WORK!!
				printf("Window resized to: %d, %d \n", event->window.data1, event->window.data2 );
		
		
		
		
				
		/*		glViewport(0, 0, (GLsizei) event->window.data1, (GLsizei) event->window.data2 );
				
					// Do this AFTER glViewport
				glMatrixMode(GL_PROJECTION);	// Select the projection matrix
				glLoadIdentity();				// Reset projection matrix
				
					// Calculate The Aspect Ratio Of The Window
				gluPerspective(45.0f,(GLfloat)event->window.data1/(GLfloat)event->window.data2,0.1f,100.0f);
				
					// Multiply current matrix with orthographic matrix
					// specify left, right, bottom, top clipping planes, and near and far clipping planes
				glOrtho(0, event->window.data1, event->window.data2, 0, -1, 1);	
				
				glMatrixMode(GL_MODELVIEW);	// Load modelview matrix
				glLoadIdentity();			// reset it
		*/		
				break;
				
	/*		case SDL_WINDOWEVENT_EXPOSED:
				SDL_SelectRenderer(SDL_GetWindowFromID(event.window.windowID));
				SDL_SetRenderDrawColor(0xA0, 0xA0, 0xA0, 0xFF);
				SDL_RenderClear();
				break;
	 */
		}
		break;
		
		

/*		case SDL_MOUSEMOTION:
				
		//		MMHandleCursorMovement( event->motion.x, event->motion.y );
			
					printf("Mouse moved by %d,%d to (%d,%d) in windowID %d\n", 
                       event->motion.xrel, event->motion.yrel,
                       event->motion.x, event->motion.y, event->motion.windowID);
                break;
		case SDL_MOUSEBUTTONDOWN:
                printf("Mouse button %d pressed at (%d,%d)\n",
                       event->button.button, event->button.x, event->button.y);
                break;
*/
			

		
		case SDL_KEYUP:
			break;

		case SDL_KEYDOWN:
				
		switch (event->key.keysym.sym) 
		{
				// Add hotkeys here
			case SDLK_z:
				if (event->key.keysym.mod & KMOD_CTRL) 
				{
						/* Ctrl-Z minimize (must be a Windows thing) */
					SDL_MinimizeWindow( gSpriteWorldP->mainWindow );
				}
				break;
			case SDLK_q:		// Was Command-Q pushed?
				if (event->key.keysym.mod & KMOD_GUI)	// KMOD_GUI = Command key on Mac
				{
					*done = true;
					break;
				}
			
			default:
				break;
		}
		break;
#endif
		case SDL_QUIT:
			*done = true;
		break;
    }
}


