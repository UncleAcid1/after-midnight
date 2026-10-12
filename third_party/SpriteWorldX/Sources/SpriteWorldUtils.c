
#include <time.h>
#include <stdlib.h>

#ifndef __SWCOMMON__
#include <SWCommonHeaders.h>
#endif

#ifndef __SWUTILS__
#include "SpriteWorldUtils.h"
#endif 

SWRGBColor		gSWTransparentColor =	// transparent pixel color
				{0xFFFF,0xFFFF,0xFFFF};	// default color: white


///--------------------------------------------------------------------------------------
//	CenterRect - centers rectA in rectB, returning result in rectA. This will work
//  properly even when rectA is _larger_ than rectB, and will result in centering rectA
//	around rectB.
///--------------------------------------------------------------------------------------

void CenterRect(SWRect* rectA, SWRect* rectB)
{
	short width = (rectA->right - rectA->left);
	short height = (rectA->bottom - rectA->top);

	rectA->left = rectB->left + (((rectB->right - rectB->left) / 2) - (width / 2));
	rectA->top = rectB->top + (((rectB->bottom - rectB->top) / 2) - (height / 2));
	rectA->right = rectA->left + width;
	rectA->bottom = rectA->top + height;
}


///--------------------------------------------------------------------------------------
// CenterRect - NEW version. Not tested as much as old. Should be identical in effect.
//	centers aRect in bRect. It's fine for rectA to be larger than rectB.
///--------------------------------------------------------------------------------------
/*
void	CenterRect( SWRect *aRectP, SWRect *bRectP)
{
	short midAHoriz, midBHoriz, midAVert, midBVert;
	
		// Compute the midpoint of each rectangle
	midAHoriz = SW_RECT_WIDTH( *aRectP ) / 2 + aRectP->left;
	midBHoriz = SW_RECT_WIDTH( *bRectP ) / 2 + bRectP->left;
	midAVert = SW_RECT_HEIGHT( *aRectP ) / 2 + aRectP->top;
	midBVert = SW_RECT_HEIGHT( *bRectP ) / 2 + bRectP->top;
	
		// Move rect A so its midpoint is over rect B's midpoint
	OffsetRect( aRectP, midBHoriz - midAHoriz, midBVert - midAVert );
}
*/


///--------------------------------------------------------------------------------------
// SetRect - initializes the rectangle
///--------------------------------------------------------------------------------------

void	SetRect( SWRect *rectP, short left, short top, short right, short bottom )
{
	rectP->left = left;
	rectP->top = top;
	rectP->right = right;
	rectP->bottom = bottom;
}


///--------------------------------------------------------------------------------------
// OffsetRect - offsets the rectangle by the given offset
///--------------------------------------------------------------------------------------

void	OffsetRect( SWRect *aRectP, short horizOffset, short vertOffset )
{
	aRectP->left += horizOffset;
	aRectP->right += horizOffset;
	aRectP->top += vertOffset;
	aRectP->bottom += vertOffset;
}


///--------------------------------------------------------------------------------------
// EqualRect - returns true if both rectangles are identical
///--------------------------------------------------------------------------------------

SWBoolean	EqualRect( SWRect *aRectP, SWRect *bRectP )
{
	return ( aRectP->top == bRectP->top && 
			 aRectP->left == bRectP->left &&
			 aRectP->right == bRectP->right &&
			 aRectP->bottom == bRectP->bottom );
}

//---------------------------------------------------------------------------------------
//  Randomize
//---------------------------------------------------------------------------------------

void Randomize(void)
{
    srand( (unsigned int)time(NULL) );
}


//---------------------------------------------------------------------------------------
//  GetRandom - generates pseudo random number. min and max are inclusive
//---------------------------------------------------------------------------------------

int GetRandom( int min, int max )
{
    int num;
    float random;
    
    random = rand()/(RAND_MAX + 1.0); //a number from 0.0 to 0.9999
    num =  (int)((1+ max-min) * random) ;
    num += min;
    
    return num;
}


//---------------------------------------------------------------------------------------
//  GetRandomFloat - - New version -- UNTESTED!!
//---------------------------------------------------------------------------------------

float GetRandomFloat( float min, float max )
{
    float num, random;
    
    random = rand()/(RAND_MAX + 1.0); //a number from 0.0 to 0.9999
    num = ((1+ max-min) * random);
    num += min;
    
    return num;
}


//---------------------------------------------------------------------------------------
//  ReportError
//---------------------------------------------------------------------------------------

void ReportError( SWError err, char* fileName, char* functionName, int lineNumber)
{
    fprintf( stderr, "\n\nFatal error (#%d).\n  File:%s\n  Function:%s\n  Line:%d\n",
        (int) err, fileName, functionName, lineNumber );
    exit(1);
}


//--------------------------------------------------------------------------------------
//	SWSetTransparentColor - allows you to set the background, or unmasked color
//	of a sprite to a color other than white. Important: the sprite's background color
//	MUST match the color you use with gSWTransparentColor, or the Sprite won't load correctly.
//---------------------------------------------------------------------------------------

void SWSetTransparentColor(SWRGBColor *RGB)
{
	gSWTransparentColor = *RGB;
}


///--------------------------------------------------------------------------------------
// SWConcatPascalStrings - Concatenate two Pascal Strings.
///--------------------------------------------------------------------------------------
/*
SW_FUNC void  SWConcatPascalStrings (Str255 targetString, Str255 appendString)
{
	SInt16	appendLength;

	appendLength = SW_MIN(appendString[0],255 - targetString[0]);

	if(appendLength > 0)
	{
		BlockMoveData(appendString+1,targetString+targetString[0]+1,(SInt32) appendLength);
		targetString[0] += appendLength;
	}
}
*/

///--------------------------------------------------------------------------------------
// SWCopyPascalString - Copy one Pascal String to another.
///--------------------------------------------------------------------------------------

void  SWCopyPascalString(SWStr255 sourceString, SWStr255 targetString)
{
	short n;
	
	for (n = 0; n <= sourceString[0]; n++)	// We copy 1 additional character, due to the length byte
		targetString[n] = sourceString[n];
}


///--------------------------------------------------------------------------------------
// SWCopyCString - Copy one C String to another.
///--------------------------------------------------------------------------------------

void  SWCopyCString (char * sourceCString, char * targetCString)
{
	short n;
	
	for (n = 0; sourceCString[n] != '\0'; n++)
		targetCString[n] = sourceCString[n];
	
		// NULL-terminate
	targetCString[n] = '\0';
}


///--------------------------------------------------------------------------------------
// SWCopyCStringToPascalString - Copy a C String to a Pascal String.
///--------------------------------------------------------------------------------------

void  SWCopyCStringToPascalString (char * sourceCString, SWStr255 targetPString)
{
	short	n;
	
	for (n = 0; sourceCString[n] != '\0' & n < 255; n++)
		targetPString[n+1] = sourceCString[n];
	
	targetPString[0] = n;
}


///--------------------------------------------------------------------------------------
// SWCopyPascalStringToCString - Copy a Pascal to a C String.
// WARNING: The C string must have enough memory allocated for it in advance!
///--------------------------------------------------------------------------------------

void  SWCopyPascalStringToCString(SWStr255 sourcePString, char * targetCString)
{
	short length = SW_MIN(sourcePString[0],255);
	short n;
	
	for (n = 0; n < length; n++)
		targetCString[n] = sourcePString[n+1];
	
	targetCString[n] = NULL; //'\0';
}


///--------------------------------------------------------------------------------------
// SWConcatPascalStrings - Concatenate two Pascal Strings.
///--------------------------------------------------------------------------------------

void  SWConcatPascalStrings(SWStr255 targetString, SWStr255 appendString)
{
	short	appendLength, offset;
	short	n;

	appendLength = SW_MIN(appendString[0], 255 - targetString[0]);
	offset = targetString[0] + 1;
	
	if (appendLength > 0)
	{
		for (n = 0; n < appendLength; n++)
			targetString[n + offset] = appendString[n+1];
		
		targetString[0] += appendLength;
	}
}
