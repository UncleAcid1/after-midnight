//---------------------------------------------------------------------------------------
/// @file SpriteWorldUtils.h
/// Constants, structures, and function prototypes for helper utilities
//---------------------------------------------------------------------------------------

#ifndef __SWUTILS__
#define __SWUTILS__

#ifndef __SWCOMMON__
#include <SWCommonHeaders.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define FatalError(err)         if (err) ReportError(err, __FILE__, (char *)__FUNCTION__, __LINE__);

void CenterRect(SWRect* rectA, SWRect* rectB);
void	SetRect( SWRect *rectP, short left, short top, short right, short bottom );
void OffsetRect( SWRect *aRectP, short horizOffset, short vertOffset );

SWBoolean	EqualRect( SWRect *aRectP, SWRect *bRectP );

void Randomize(void);
int GetRandom( int min, int max );
float GetRandomFloat( float min, float max );


void	GiveOSTime( void );

void ReportError( SWError err, char* fileName, char* functionName, int lineNumber);

void SWSetTransparentColor(SWRGBColor *RGB);

void  SWCopyPascalString (SWStr255 sourceString, SWStr255 targetString);
void  SWCopyCString (char * sourceCString, char * targetCString);
void  SWCopyCStringToPascalString (char * sourceCString, SWStr255 targetPString);
void  SWCopyPascalStringToCString (SWStr255 sourcePString, char * targetCString);
void  SWConcatPascalStrings (SWStr255 targetString, SWStr255 appendString);

#ifdef __cplusplus
}
#endif

#endif // __SWUTILS__

