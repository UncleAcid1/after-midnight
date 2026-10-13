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

/*  GetRandom's numbers: the C library's rand() differs from system to system, and two
    computers playing relay co-op must draw the same numbers. So this is a generator of our own
    (xorshift, 31 bits as rand() gave), seeded from the clock as before, and seeded the same on
    every computer in a network game (SWSeedRandom). */
static Uint32 gRandomState = 0x2545f491;

void SWSeedRandom(unsigned long seed)
{
    gRandomState = (Uint32)seed != 0 ? (Uint32)seed : 0x2545f491;
}

unsigned int SWRandomState(void)
{
    return gRandomState;
}

static int SWRand31(void)
{
    gRandomState ^= gRandomState << 13;
    gRandomState ^= gRandomState >> 17;
    gRandomState ^= gRandomState << 5;
    return (int)(gRandomState >> 1);
}

void Randomize(void)
{
    SWSeedRandom((unsigned long)time(NULL));
}

/// Returns a random integer in [min, max].
int GetRandom(int min, int max)
{
    return (int)((float)(max - min + 1) * ((float)SWRand31() / 2147483648.0f)) + min;
}

float GetRandomFloat(float min, float max)
{
    return (max - min) * ((float)SWRand31() / 2147483648.0f) + min;
}

/*  Sine and cosine of an angle in degrees, the same on every system: only + - * on doubles
    (a system's sin() and cos() can differ in the last bit, and relay co-op computes the same
    frames on several computers). The angle comes down to [-45, 45] degrees exactly (fmod is
    exact); the polynomials are fdlibm's (__kernel_sin, __kernel_cos), good to < 1 ulp there. */
#if defined(__clang__)
#pragma STDC FP_CONTRACT OFF
#endif
#define kSWDegToRad 0.017453292522222223    /* the value the game always used */

static void SWSinCosDegrees(double degrees, double *sinP, double *cosP)
{
    double d = fmod(degrees, 360.0), r, x, z, s, c;
    int q;

    if (d < 0)
        d += 360.0;
    q = (int)floor((d + 45.0) / 90.0);
    r = d - 90.0 * q;
    x = r * kSWDegToRad;
    z = x * x;
    s = x + x * z * (-1.66666666666666324348e-01 + z * (8.33333333332248946124e-03 + z * (-1.98412698298579493134e-04 +
        z * (2.75573137070700676789e-06 + z * (-2.50507602534068634195e-08 + z * 1.58969099521155010221e-10)))));
    c = 1.0 - 0.5 * z + z * z * (4.16666666666666019037e-02 + z * (-1.38888888888741095749e-03 + z * (2.48015872894767294178e-05 +
        z * (-2.75573143513906633035e-07 + z * (2.08757232129817482790e-09 + z * -1.13596475577881948265e-11)))));
    switch (q & 3) {
        case 0:  *sinP = s;  *cosP = c;  break;
        case 1:  *sinP = c;  *cosP = -s; break;
        case 2:  *sinP = -s; *cosP = -c; break;
        default: *sinP = -c; *cosP = s;  break;
    }
}

double SWSinDegrees(double degrees)
{
    double s, c;
    SWSinCosDegrees(degrees, &s, &c);
    return s;
}

double SWCosDegrees(double degrees)
{
    double s, c;
    SWSinCosDegrees(degrees, &s, &c);
    return c;
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
    return SWCosDegrees(360.0 - degrees);
}

double SWSin(double degrees)
{
    return SWSinDegrees(360.0 - degrees);
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
