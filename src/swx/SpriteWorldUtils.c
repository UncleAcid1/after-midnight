/*
 * SpriteWorld X fork — rectangle, string and math utilities.
 * See SWCommonHeaders.h for license and provenance.
 */

#include <time.h>
#include "SpriteWorld.h"

void SWCenterRect(SWRect *rectA, SWRect *rectB)
{
    short width = rectA->right - rectA->left;
    short height = rectA->bottom - rectA->top;

    rectA->left = rectB->left + ((short)((rectB->right - rectB->left) / 2) - width / 2);
    rectA->top = rectB->top + ((short)((rectB->bottom - rectB->top) / 2) - height / 2);
    rectA->right = rectA->left + width;
    rectA->bottom = rectA->top + height;
}

void SWSetRect(SWRect *rectP, short left, short top, short right, short bottom)
{
    rectP->left = left;
    rectP->top = top;
    rectP->right = right;
    rectP->bottom = bottom;
}

void SWOffsetRect(SWRect *rectP, short horizOffset, short vertOffset)
{
    rectP->left += horizOffset;
    rectP->right += horizOffset;
    rectP->top += vertOffset;
    rectP->bottom += vertOffset;
}

void SWInsetRect(SWRect *rectP, short horizInset, short vertInset)
{
    rectP->left += horizInset;
    rectP->right -= horizInset;
    rectP->top += vertInset;
    rectP->bottom -= vertInset;
}

SWBoolean SWEqualRect(SWRect *rectA, SWRect *rectB)
{
    return rectA->top == rectB->top && rectA->left == rectB->left &&
           rectA->right == rectB->right && rectA->bottom == rectB->bottom;
}

void Randomize(void)
{
    srand((unsigned)time(NULL));
}

/// Returns a random integer in [min, max].
int GetRandom(int min, int max)
{
    return (int)((float)(max - min + 1) * ((float)rand() / 2147483648.0f)) + min;
}

float GetRandomFloat(float min, float max)
{
    return (max - min) * ((float)rand() / 2147483648.0f) + min;
}

void SWCopyPascalString(const unsigned char *sourceString, unsigned char *targetString)
{
    short i;
    for (i = 0; i <= sourceString[0]; i++)
        targetString[i] = sourceString[i];
}

void SWCopyCString(const char *sourceCString, char *targetCString)
{
    short i;
    for (i = 0; sourceCString[i] != '\0'; i++)
        targetCString[i] = sourceCString[i];
    targetCString[i] = '\0';
}

SWBoolean SWEqualString(const unsigned char *stringA, const unsigned char *stringB)
{
    short len = SW_MIN(stringA[0], stringB[0]);
    short i;

    if (len > 255)
        len = 255;
    for (i = 1; i <= len; i++) {
        if (stringA[i] != stringB[i])
            return false;
    }
    return true;
}

void SWCopyCStringToPascalString(const char *sourceCString, unsigned char *targetPString)
{
    short i;
    for (i = 0; i < 255 && sourceCString[i] != '\0'; i++)
        targetPString[i + 1] = (unsigned char)sourceCString[i];
    targetPString[0] = (unsigned char)i;
}

void SWCopyPascalStringToCString(const unsigned char *sourcePString, char *targetCString)
{
    short len = (sourcePString[0] == 0xFF) ? 255 : sourcePString[0];
    short i;

    for (i = 0; i < len; i++)
        targetCString[i] = (char)sourcePString[i + 1];
    targetCString[i] = '\0';
}

void SWConcatPascalStrings(unsigned char *targetString, const unsigned char *appendString)
{
    short count = SW_MIN(appendString[0], 255 - targetString[0]);
    short start = targetString[0] + 1;
    short i;

    if (count > 0) {
        for (i = 0; i < count; i++)
            targetString[start + i] = appendString[i + 1];
        targetString[0] += (unsigned char)count;
    }
}

void SWStringToNum(const unsigned char *pString, long *theNum)
{
    char cString[265];
    SWCopyPascalStringToCString(pString, cString);
    *theNum = atol(cString);
}

void SWNumToString(long theNum, unsigned char *pString)
{
    char cString[259];
    snprintf(cString, sizeof(cString), "%ld", theNum);
    SWCopyCStringToPascalString(cString, pString);
}

/// Angles run clockwise from 0 at the top in SpriteWorld, hence the 360 - x.
double SWCos(double degrees)
{
    return cos((360.0 - degrees) * 0.017453292522222223);
}

double SWSin(double degrees)
{
    return sin((360.0 - degrees) * 0.017453292522222223);
}

double SWATan(double value)
{
    return atan(value) * -57.29577950560105 + 360.0;
}

void ReportError(int errNum, const char *file, const char *function, int line)
{
    fprintf(stderr, "\n\nFatal error (#%d).\n  File:%s\n  Function:%s\n  Line:%d\n", errNum, file, function, line);
    exit(1);
}
