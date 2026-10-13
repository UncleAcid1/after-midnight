/*
 * MyUtils.c — string, rect, math and timing helpers.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources - My Utils/MyUtils.c").
 */
#include "MMGame.h"
#include <ctype.h>

// ---------------------------------------------------------------------------
// Globals

static int gFileStickyError = 0;
Boolean gCursorIsVisible;
static Uint32 gLastMilliseconds;

// ---------------------------------------------------------------------------
// Cursor

void MyHideCursor(void)
{
    SDL_ShowCursor(SDL_DISABLE);
    gCursorIsVisible = false;
}

void MyInitCursor(void)
{
    SDL_ShowCursor(SDL_ENABLE);
    gCursorIsVisible = true;
}

Boolean IsCursorVisible(void)
{
    return SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE;
}

// ---------------------------------------------------------------------------
// Pascal / C / HFS-unicode strings

void SetPascalString(StringPtr dst, ConstStringPtr src)
{
    short i;
    for (i = 0; i <= src[0]; i++)
        dst[i] = src[i];
}

void GetPascalStringFromCString(StringPtr dst, const char *src)
{
    short i;
    dst[0] = 0;
    for (i = 0; src[i] != '\0' && i < 255; i++) {
        dst[0]++;
        dst[i + 1] = (unsigned char)src[i];
    }
}

/// HFSUniStr255 layout: big-endian UInt16 length, then big-endian UTF-16 units.
void GetHFSUniFromCString(unsigned char *dst, const char *src)
{
    short len = (short)strlen(src);
    short srcI = 0, dstI = 2;
    dst[0] = 0;
    dst[1] = (unsigned char)len;
    for (; len > 0; len--) {
        dst[dstI] = 0;
        dst[dstI + 1] = (unsigned char)src[srcI++];
        dstI += 2;
    }
}

void GetCStringFromHFSUniStr(char *dst, const unsigned char *src)
{
    short srcI = 3, dstI = 0, len;
    for (len = (signed char)src[1]; len > 0; len--) {
        dst[dstI++] = (char)src[srcI];
        srcI += 2;
    }
    dst[dstI] = '\0';
}

void GetPascalStringFromHFSUniStr(StringPtr dst, const unsigned char *src)
{
    short srcI = 3, dstI = 1, len;
    dst[0] = src[1];
    for (len = (signed char)src[1]; len > 0; len--) {
        dst[dstI++] = src[srcI];
        srcI += 2;
    }
}

Boolean IsCharWhiteSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void RemoveTrailingSpaces(StringPtr s)
{
    short i;
    for (i = s[0]; i > 0; i--) {
        if (!IsCharWhiteSpace((char)s[i]))
            break;
    }
    s[0] = (unsigned char)i;
}

void RemoveLeadingSpaces(StringPtr s)
{
    short i;
    for (i = 1; i <= s[0]; i++) {
        if (!IsCharWhiteSpace((char)s[i]))
            break;
    }
    if (i > 1)
        memmove(s + 1, s + i, s[0] - i + 1);
    s[0] = (unsigned char)(s[0] - i + 1);
}

/// Removes everything up to and including the last line break.
void RemoveLeadingInvisibleString(StringPtr s)
{
    short i, start;
    for (i = s[0]; i > 0 && s[i] != '\n' && s[i] != '\r'; i--)
        ;
    start = i + 1;
    // The original computed the move length from s[start] instead of s[0]; the
    // length byte is what is meant.
    if (start > 1)
        memmove(s + 1, s + start, s[0] - start + 1);
    s[0] = (unsigned char)(s[0] - start + 1);
}

void StripPrefixFromString(StringPtr s, short prefixLen)
{
    if (s[0] - prefixLen > 0)
        memmove(s + 1, s + 1 + prefixLen, s[0] - prefixLen);
    s[0] = (unsigned char)(s[0] - prefixLen);
}

void StripPrefixFromCString(char *s, short prefixLen)
{
    int remaining = (int)strlen(s) - prefixLen;
    if (remaining != 0)
        memmove(s, s + prefixLen, remaining);
    s[remaining] = '\0';
}

/// Copies to dst whatever part of src follows its common prefix with prefix.
void StripStringFromStartOfString(ConstStringPtr prefix, ConstStringPtr src, StringPtr dst)
{
    unsigned int len = src[0];
    unsigned int i;
    int count;
    for (i = 1; i < len && prefix[i] == src[i]; i++)
        ;
    count = (int)(len - i) + 1;
    if (count != 0) {
        memmove(dst + 1, src + i, count);
        dst[0] = (unsigned char)count;
    }
}

/// Truncates src (copied to dst, if given) at an occurrence of suffix that ends it.
void StripStringFromEndOfCString(const char *suffix, char *src, char *dst)
{
    char *found;
    if (src == dst || dst == NULL)
        dst = src;
    else
        strcpy(dst, src);

    found = strstr(dst, suffix);
    while (found != NULL) {
        if (strlen(suffix) == strlen(found)) {
            *found = '\0';
            continue;   // found still points at the (now empty) tail; strlen differs next time
        }
        found = strstr(found + strlen(suffix), suffix);
    }
}

void StripStringFromEndOfString(ConstStringPtr suffix, ConstStringPtr src, StringPtr dst)
{
    short srcLen, sufLen, i, j;
    Boolean matches = true;
    if (src != dst)
        SWCopyPascalString(src, dst);
    srcLen = dst[0];
    sufLen = suffix[0];
    for (i = srcLen, j = sufLen; i > 0 && j > 0; i--, j--) {
        if (suffix[j] != dst[i]) {
            matches = false;
            break;
        }
    }
    if (matches)
        dst[0] = (unsigned char)(dst[0] - sufLen);
}

void RemoveNumberFromEndOfString(StringPtr s)
{
    while (s[s[0]] >= '0' && s[s[0]] <= '9')
        s[0]--;
}

/// True if a is a prefix of b (the original's semantics).
Boolean CompareCStrings(const char *a, const char *b)
{
    short i;
    for (i = 0; i != 0x7fff; i++) {
        if (a[i] == '\0')
            return true;
        if (b[i] == '\0')
            return false;
        if (a[i] != b[i])
            return false;
    }
    return false;
}

Boolean CompareCStringsForLength(const char *a, const char *b, short len)
{
    Boolean same = true;
    short i;
    for (i = 0; i < len; i++) {
        if (a[i] != b[i])
            same = false;
    }
    return same;
}

char *strstrIgnoreCase(const char *haystack, const char *needle)
{
    const char *h, *n;
    if (*needle == '\0')
        return (char *)haystack;
    for (; *haystack != '\0'; haystack++) {
        if (toupper((unsigned char)*haystack) != toupper((unsigned char)*needle))
            continue;
        for (h = haystack, n = needle; *h != '\0' && *n != '\0'; n++, h++) {
            if (toupper((unsigned char)*h) != toupper((unsigned char)*n))
                break;
        }
        if (*n == '\0')
            return (char *)haystack;
    }
    return NULL;
}

// ---------------------------------------------------------------------------
// Paths

void StripNDirectoriesFromEndOfPath(char *path, short n)
{
    short i;
    for (i = 0; i < n; i++)
        StripEndingFromPath(path);
}

void StripEndingFromPath(char *path)
{
    char *slash = strrchr(path, '/');
    if (slash != NULL)
        *slash = '\0';
}

char *GetFilenameFromPath(char *path)
{
    return strrchr(path, '/');
}

// ---------------------------------------------------------------------------
// Binary file IO (big-endian, like the original Mac files)

void MyWriteDataToFile(SDL_RWops *file, long size, void *data)
{
    int err = 0;
    size_t written;
    if (size <= 0)
        return;
    if (size == 2)
        written = SDL_WriteBE16(file, *(Uint16 *)data);
    else if (size == 4)
        written = SDL_WriteBE32(file, *(Uint32 *)data);
    else
        written = SDL_RWwrite(file, data, size, 1);
    if (written == 0)
        err = -1;
    SetFileStickyIfError(err);
}

int MyReadDataFromFile(SDL_RWops *file, long size, void *data)
{
    int err = 0;
    if (size <= 0)
        return 0;
    if (size == 2)
        *(Uint16 *)data = SDL_ReadBE16(file);
    else if (size == 4)
        *(Uint32 *)data = SDL_ReadBE32(file);
    else if (SDL_RWread(file, data, size, 1) == 0)
        err = -1;
    return err;
}

void ClearFileStickyError(void)
{
    gFileStickyError = 0;
}

int GetFileStickyError(void)
{
    return gFileStickyError;
}

void SetFileStickyIfError(int err)
{
    if (err != 0)
        gFileStickyError = err;
}

Boolean WasASpecificKeyPressed(void)
{
    return false;
}

// ---------------------------------------------------------------------------
// Rects

Boolean IsRectInRect(const SWRect *a, const SWRect *b)
{
    SW_ASSERT(a != NULL && b != NULL);
    return a->top < b->bottom && b->top < a->bottom &&
           a->left < b->right && b->left < a->right;
}

Boolean IsRectFullyInRect(const SWRect *inner, const SWRect *outer)
{
    SW_ASSERT(inner != NULL && outer != NULL);
    return !(inner->top < outer->top || outer->bottom < inner->bottom ||
             inner->left < outer->left || outer->right < inner->right);
}

void MoveRect(SWRect *r, short h, short v)
{
    short oldTop, oldLeft;
    SW_ASSERT(r != NULL);
    oldTop = r->top;
    oldLeft = r->left;
    r->left = h;
    r->top = v;
    r->right = h + (r->right - oldLeft);
    r->bottom = v + (r->bottom - oldTop);
}

void OffsetRectForFrame(SWRect *r, FramePtr frameP)
{
    r->top    += frameP->frameRect.top;
    r->bottom += frameP->frameRect.top;
    r->left   += frameP->frameRect.left;
    r->right  += frameP->frameRect.left;
}

// ---------------------------------------------------------------------------
// Math

void SwapVariables(short *a, short *b)
{
    short t = *a;
    *a = *b;
    *b = t;
}

int sign(short n)
{
    return n < 0 ? -1 : 1;
}

int mAbs(int n)
{
    return n < 0 ? -n : n;
}

Boolean IsNumberEven(long n)
{
    return (n & 1) == 0;
}

Boolean IsNumberOdd(long n)
{
    return (n & 1) != 0;
}

/// Angles are in degrees, counter-clockwise, with screen y pointing down.
double GetAngleFromXandY(double x, double y)
{
    double angle = MyATan(y / x);
    if (x < 0.0)
        angle += 180.0;
    else if (y < 0.0)
        angle += 360.0;
    while (angle < 0.0)
        angle += 360.0;
    while (angle > 360.0)
        angle -= 360.0;
    return angle;
}

double GetXFromAngleAndVelocity(double angle, double velocity)
{
    return MyCos(angle) * velocity;
}

double GetYFromAngleAndVelocity(double angle, double velocity)
{
    return MySin(angle) * velocity;
}


double MyCos(double angle)
{
    return SWCosDegrees(360.0 - angle);     // the same on every system (relay co-op)
}

double MySin(double angle)
{
    return SWSinDegrees(360.0 - angle);
}

double MyATan(double n)
{
    return atan(n) * -57.29577950560105 + 360.0;
}

void AddBooleanToByte(Boolean value, unsigned char *byte, unsigned char bit)
{
    *byte |= (unsigned char)(value << (bit & 0x1f));
}

Boolean ReadBooleanFromByte(unsigned char byte, unsigned char bit)
{
    return ((signed char)byte >> (bit & 0x1f)) & 1;
}

int GetCustomRandom(int min, int max)
{
    return (int)((float)(max - min + 1) * (float)CustomRandom()) + min;
}

float GetCustomRandomFloat(float min, float max)
{
    return (max - min) * (float)CustomRandom() + min;
}

// ---------------------------------------------------------------------------
// Timing (the tick-based variants were already stubs in the SDL build)

void StartTicksTimer(void) {}
void WaitTicks(void) {}
void WaitTicksOrMouseClick(void) {}
void MyDelay(void) {}
void MyWaitForKeyDelay(void) {}

void StartMicrosecTimer(void)
{
    gLastMilliseconds = SDL_GetTicks();
}

/// Despite the name, the argument is in milliseconds.
void WaitMicroseconds(Uint32 ms)
{
    Uint32 elapsed;
    do {
        elapsed = SDL_GetTicks() - gLastMilliseconds;
        if (elapsed < ms - 20)
            SDL_Delay(1);
    } while (elapsed < ms);
}

Uint32 GetElapsedSeconds(void)
{
    return GetElapsedMillisecs() / 1000;
}

Uint32 GetElapsedMillisecs(void)
{
    return SDL_GetTicks() - gLastMilliseconds;
}

void WaitMicrosecondsOrKeyPress(Uint32 ms)
{
    Uint32 elapsed;
    for (;;) {
        elapsed = SDL_GetTicks() - gLastMilliseconds;
        if (elapsed < ms - 20) {
            if (WasAKeyPressed() || WasGameQuit())
                return;
        }
        if (elapsed >= ms)
            return;
        SDL_Delay(1);
    }
}

void WaitMicrosecondsOrMouseClick(Uint32 ms)
{
    Uint32 elapsed;
    for (;;) {
        elapsed = SDL_GetTicks() - gLastMilliseconds;
        if (elapsed < ms - 20) {
            if (WasMouseClicked() || WasAKeyPressed() || WasGameQuit())
                return;
        }
        if (elapsed >= ms)
            return;
        SDL_Delay(1);
    }
}
