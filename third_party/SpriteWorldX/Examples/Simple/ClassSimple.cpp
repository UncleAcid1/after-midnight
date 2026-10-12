//	Class-Simple
//
//		A version of Simple that uses C++ objects.
//
//	originally by Anders F Bjoerklund (afb)
//	heavily redacted by John Love-Jensen (eljay)
//  re-adapted again for SWX by afb

#include <SWIncludes.h>  // Include all SpriteWorld headers
#include "SpriteWorldUtils.h"

#ifdef SW_NAMESPACE
using namespace SW_NAMESPACE;
#endif

class TBall
{
private:
	TSprite mSprite;

	static const short kMaxSpriteMoveDelta = 4;

public:
	void	createMaster(TSpriteWorld& inWorld, char *filename);
	void	addSprites(TSpriteWorld& inWorld, TSpriteLayer& inLayer, short inNumSprites);
	void	cleanUp();

	static SW_FUNC void MoveProc(SpritePtr inSrcSpriteP);
};


class TSimpleApp
{
private:
	TSpriteWorld mWorld;
	TSpriteLayer mLayer;
	TBall mBall;

	static const short kNumSprites = 4;
	static const short kMaxFPS = 0;				// Keep the animation from going too fast

public:
	void	initialize();
	void createWindow();
	void	runAnimation();
	void cleanUp();
};


extern "C" int main(int argc, char *argv[])
{
	Randomize();

	try
	{
		TSimpleApp theApp;

		theApp.initialize();

		theApp.runAnimation();

		theApp.cleanUp();
	}
	catch (std::runtime_error err)
	{
		fprintf(stderr, "Error %s!\n", err.what() );
		return 1;
	}
	
	return 0;
}


#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------------------
//	CreateBallSprite - load the master ball sprite and prepare it for cloning later
//---------------------------------------------------------------------------------------
void	TBall::createMaster(TSpriteWorld& inWorld, char *filename) {
	mSprite = inWorld.createSpriteFromFile(filename, 1);

		// Set up the sprite (remember that it must be locked!)
	mSprite.lock();
	mSprite.moveBounds(inWorld.backRect());
 
	mSprite.moveProc(MoveProc);
	}


//---------------------------------------------------------------------------------------
//	moveProc - move the sprite logic.
//---------------------------------------------------------------------------------------
SW_FUNC void TBall::MoveProc(SpritePtr inSrcSpriteP) {
	TSprite theSprite(inSrcSpriteP);
	theSprite.offset(theSprite.horizMoveDelta(), theSprite.vertMoveDelta());
	theSprite.bounce();
	}


//---------------------------------------------------------------------------------------
//	addSprites - clone the master sprite, and add the clones to the SpriteWorld
//---------------------------------------------------------------------------------------
void	TBall::addSprites(TSpriteWorld& inWorld, TSpriteLayer& inLayer, short inNumSprites) {
	for(short spriteNum = 0; spriteNum < inNumSprites; ++spriteNum) {
			// Clone the master sprite. The clone doesn't need to be locked,
			// since the master sprite was already locked.
		TSprite theNewSprite = mSprite.clone();

		inLayer.add(theNewSprite);

		theNewSprite.location(
			GetRandom(0, inWorld.backRect().right-44),
			GetRandom(0, inWorld.backRect().bottom-44));

		short theHorizDelta = GetRandom(1-kMaxSpriteMoveDelta, kMaxSpriteMoveDelta);
		short theVertDelta = GetRandom(1-kMaxSpriteMoveDelta, kMaxSpriteMoveDelta);
		if(theHorizDelta <= 0) --theHorizDelta;
		if(theVertDelta <= 0) --theVertDelta;
		theNewSprite.moveDelta(theHorizDelta, theVertDelta);
		}
	}


//---------------------------------------------------------------------------------------
//	cleanUp
//---------------------------------------------------------------------------------------
void	TBall::cleanUp() {
		// Dispose the master sprite, since it isn't included in the SpriteWorld.
	mSprite.dispose();
	}


//---------------------------------------------------------------------------------------
//	SetUpSpriteWorld
//---------------------------------------------------------------------------------------
void	TSimpleApp::initialize() {
	int				width = 640;
	int				height = 480;
	int				depth = 0;
	char*			spritename = "BlueBall.tga";
	bool			fullscreen = false;
	bool			opengl = true;

		// Initialize the SpriteWorld package
	TSpriteWorld::Enter(true);
	
		// Create the SpriteWorld
		// Turned into a FACTORY class function.
	mWorld = TSpriteWorld::CreateSpriteWorld(width, height, depth, fullscreen, opengl);

		// Create the Sprite Layer
	mLayer = TSpriteLayer::Create();

		// Put the pieces together and lock the SpriteWorld.
		// Note: since we are locking the SpriteWorld before adding the sprites,
		// we must make sure to lock each sprite individually when creating them.
	mWorld.add(mLayer);
	mWorld.lock();

		// The ball sprite.
	mBall.createMaster(mWorld, spritename);
	mBall.addSprites(mWorld, mLayer, kNumSprites);
	}


//---------------------------------------------------------------------------------------
//	RunAnimation
//---------------------------------------------------------------------------------------
void	TSimpleApp::runAnimation() {

	SDL_Event event;
	unsigned long then, now, frames = 0;

	mWorld.update(true);

	FatalError(SWStickyError());  // Make sure no errors got past us during setup

	then = SDL_GetTicks();

	do
	{
		mWorld.process();
		FatalError(SWStickyError());  // Make sure no errors occurred during a MoveProc, etc.

		mWorld.animate();

		if(mWorld.frameHasOccurred())
			frames++;
		
		SDL_PollEvent(&event);
        if(event.type == SDL_QUIT)
            break;

	} while( ! SDL_GetMouseState( NULL, NULL ) );

	now = SDL_GetTicks();
	if ( now > then )
		printf("%2.2f frames per second\n",
					((double)frames*1000)/(now-then));

	}


//---------------------------------------------------------------------------------------
// 	cleanUp - This function is not really necessary, since the system will dispose
//	everything automatically when the program quits. However, if you sync the animation
//	to the VBL, you must either call SWDisposeSpriteWorld, or SWSyncSpriteWorldToVBL
//	with a value of false before your program quits, in order to remove the VBL task.
//---------------------------------------------------------------------------------------
void	TSimpleApp::cleanUp() {
	mBall.cleanUp();

		// Dispose the SpriteWorld, including all sprites and layers currently in it.
	mWorld.dispose();

		// SpriteWorld has left the building.
	TSpriteWorld::Exit();

	}


