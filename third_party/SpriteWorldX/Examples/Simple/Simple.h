#ifndef __SIMPLE__
#define __SIMPLE__

#include <SWIncludes.h>

void	SetUpSpriteWorld();
void	CreateBallSprite();
void	AddSprites();
void	RunAnimation();
void	CleanUp();
void	MyCleanUpFunction();
void	BallSpriteMoveProc(SpritePtr ballSpriteP);
void	BallSpriteBounceProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect* sectRect);


#endif /* __APPLICATION__ */

