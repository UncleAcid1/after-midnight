/*
 * PS3GL.c — the OpenGL calls After Midnight uses (see PS3GL.h), drawn with the PS3's RSX.
 *
 * How it maps:
 * - glBegin/glEnd: each vertex is transformed on the PPU by projection * modelview (as the
 *   fixed-function pipeline would) and written to a vertex ring in RSX memory; glEnd draws
 *   the batch as triangles. One pass-through vertex program, and three fragment programs:
 *   color only, texture * color (GL_MODULATE), and the same with GL_REPEAT done in the shader.
 * - Textures: GL's R,G,B,A bytes become the RSX's A8R8G8B8 (linear, any size). GL_ALPHA8
 *   becomes white with that alpha, which is what GL_MODULATE makes of it.
 * - Window coordinates: GL counts rows from the bottom. The screen's buffers are stored top
 *   row first, so drawing to the screen flips; a texture's row 0 is GL's row 0, so drawing
 *   into a texture (framebuffer object) doesn't.
 * - glCopyTexSubImage2D draws the current buffer into the texture (opaque when it's the
 *   screen: a computer's window has no alpha channel either).
 * - Frames: PS3GLSwap queues the flip and waits until the RSX has finished the frame, so the
 *   vertex ring and textures can be reused safely. Simple and safe; the PPU and RSX don't
 *   overlap yet, which can come later if the speed is needed.
 * - The command buffer is our own (1 MB of the memory mapped for the RSX), not the system's:
 *   that one is handed out in 28 KB segments by libgcm's callback, which hung RPCS3. When ours
 *   is full, a jump takes the RSX back to its start, and the PPU waits until the RSX has taken
 *   it before writing there again.
 */

#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>

#include <rsx/rsx.h>
#include <sysutil/video.h>

#include "PS3GL.h"
#include "ps3gl_vp_vpo.h"
#include "ps3gl_color_fp_fpo.h"
#include "ps3gl_texture_fp_fpo.h"
#include "ps3gl_repeat_fp_fpo.h"
#include "ps3gl_opaque_fp_fpo.h"

#define kSystemBufferSize   0x10000         // the system's command buffer, used only to start
#define kIOSize             (2 * 1024 * 1024)   // main memory mapped for the RSX
#define kCommandBufferSize  (1024 * 1024)   // ours: the second half of it
#define kVertexRingSize     (4 * 1024 * 1024)
#define kLabelIndex         255
#define kMaxModelview       32
#define kMaxProjection      4
#define kMaxTextures        4096
#define kMaxFramebuffers    16
#define kTargetArenaSize    (64 * 1024 * 1024)  // memory for the textures the RSX draws into
#define kMaxArenaBlocks     4096

typedef struct { float m[16]; } Mat4;       // column-major, as OpenGL

typedef struct {
    float x, y, z, w;
    float u, v;
    float r, g, b, a;
} Vertex;

typedef struct {
    int     exists;             // name handed out by glGenTextures
    void   *mem;                // A8R8G8B8 rows in RSX memory
    u32     offset;
    int     width, height;
    u32     pitch;
    int     linearFilter;
    int     repeat;
    u32     usedInFrame;        // last frame that drew with it (or into it)
    int     target;             // its memory is from the render-target arena (see there)
    u32     size;
} Texture;

typedef struct {
    int     exists;
    GLuint  texture;
} Framebuffer;

// RSX
static gcmContextData  *gContext;
static void            *gHostAddr;
static u32              gScreenW, gScreenH;
static void            *gColorBuffer[2];
static u32              gColorOffset[2];
static u32              gColorPitch;
static void            *gDepthBuffer;
static u32              gDepthOffset, gDepthPitch, gDepthW, gDepthH;
static int              gCurBuffer;
static int              gFirstFlip = 1;
static u32              gLabelValue = 1;
static u32              gFrame = 1;
static float            gOverscan;
static float            gDisplayAspect = 16.0f / 9.0f;          // 0, or how much smaller the picture is shown
static void            *gGameBuffer;        // with overscan the game draws here, not on screen
static u32              gGameOffset;
volatile const char    *gPS3GLDoing = "starting";
volatile unsigned       gPS3GLLastSwap, gPS3GLFrames;

// programs
static rsxVertexProgram    *gVP;
static void                *gVPUCode;
static rsxProgramAttrib    *gPosAttr, *gColAttr, *gTexAttr;
typedef struct {
    rsxFragmentProgram *fp;
    u32                 offset;
    rsxProgramAttrib   *texUnit;
} FragProg;
enum { kProgColor, kProgTexture, kProgRepeat, kProgOpaque, kNumProgs };
static FragProg         gFP[kNumProgs];

// render-target arena: the textures the RSX draws into, never shared with ordinary images
typedef struct { u32 start, size; } ArenaBlock;
static u8              *gArena;
static ArenaBlock       gArenaFree[kMaxArenaBlocks];    // free blocks, by address
static int              gArenaFreeCount;
static u32              gTextureBytes, gTargetBytes;

// vertex ring
static Vertex          *gRing;
static u32              gRingOffset;
static u32              gRingCount;         // vertices used this frame
static u32              gRingCapacity;

// GL state
static GLenum           gError = GL_NO_ERROR;
static GLenum           gMatrixMode = GL_MODELVIEW;
static Mat4             gModelview[kMaxModelview], gProjection[kMaxProjection];
static int              gModelviewTop, gProjectionTop;
static Mat4             gMVP;
static int              gMVPDirty = 1;
static float            gColor[4] = { 1, 1, 1, 1 };
static float            gTexCoord[2];
static int              gTextureEnabled, gBlendEnabled, gScissorEnabled;
static GLenum           gBlendSrc = GL_ONE, gBlendDst = GL_ZERO;
static GLint            gViewport[4], gScissor[4];
static float            gClearColor[4];
static GLint            gUnpackRowLength, gUnpackAlignment = 4;
static Texture          gTextures[kMaxTextures];
static GLuint           gBoundTexture;
static Framebuffer      gFramebuffers[kMaxFramebuffers];
static GLuint           gBoundFramebuffer;

// immediate mode
static GLenum           gPrimitive;
static int              gInBegin;
static Vertex           gPending[4];        // corners of a quad / last two strip vertices
static int              gPendingCount;
static int              gStripParity;
static u32              gBatchStart;

// what the RSX was last told (so unchanged state isn't sent again)
static int              gStateDirty = 1;
static int              gCurProg = -1;
static GLuint           gCurTexture = (GLuint)-1;
static int              gTextureCacheDirty = 1;
static int              gTargetDirty = 1;

static void SetError(GLenum e)
{
    if (gError == GL_NO_ERROR)
        gError = e;
}

GLenum glGetError(void)
{
    GLenum e = gError;
    gError = GL_NO_ERROR;
    return e;
}

// ---------------------------------------------------------------------------
// Synchronization

/// Returns when the RSX has done everything queued so far.
static gcmContextData   gCommands;          // our command buffer (static: the system needs a 32-bit address)
static u32              gCommandStart;      // its I/O offset

/// Takes the RSX back to the start of the command buffer. The jump goes in first, just past
/// the last command (RPCS3 reads a little beyond PUT, and read the old words there); then
/// the RSX reads everything up to it, PUT moves to the start, and the PPU waits until the RSX
/// has taken the jump, so nothing it hasn't read is overwritten. The callback librsx calls
/// when the buffer is full.
static void WaitForGet(u32 offset, const char *what)
{
    gcmControlRegister volatile *control = gcmGetControlRegister();
    int waited = 0;

    while (control->get != offset) {
        usleep(10);
        if (++waited == 200000)
            printf("PS3GL: the RSX hasn't %s (get %x, put %x)\n", what, control->get, control->put);
    }
}

static s32 WrapCommandBuffer(gcmContextData *context, u32 count)
{
    gcmControlRegister volatile *control = gcmGetControlRegister();
    u32 end;

    (void)count;
    rsxAddressToOffset(context->current, &end);
    *context->current = 0x20000000u | gCommandStart;     // the old-style JUMP command
    __asm__ volatile("sync" ::: "memory");
    control->put = end;
    WaitForGet(end, "read its commands");
    context->current = context->begin;
    __asm__ volatile("sync" ::: "memory");
    control->put = gCommandStart;
    WaitForGet(gCommandStart, "taken the jump back");
    return 0;
}

/*  Room for the next commands, made before each group of them. librsx's own check calls the
    callback above when the buffer is full, but the code around it keeps writing where it was
    (the callback's inline assembly doesn't tell the compiler memory changed), over the jump:
    so we never let it come to that. */
#define kCommandRoom 2048           // words: more than any one operation here sends
static void EnsureCommandSpace(u32 words)
{
    if (gContext == &gCommands && gCommands.current + words > gCommands.end)
        WrapCommandBuffer(&gCommands, words);
}
#define ROOM() EnsureCommandSpace(kCommandRoom)

/// Moves the RSX from the system's command buffer (where rsxInit left it) to ours.
static int UseOwnCommandBuffer(void)
{
    gcmControlRegister volatile *control = gcmGetControlRegister();
    u32 *buffer = (u32 *)((char *)gHostAddr + kIOSize - kCommandBufferSize);
    int waited = 0;

    if (rsxAddressToOffset(buffer, &gCommandStart) != 0)
        return -1;
    rsxSetupContextData(&gCommands, buffer, kCommandBufferSize, (gcmContextCallback)WrapCommandBuffer);
    gCommands.end = buffer + kCommandBufferSize / 4 - 1;     // room for the jump back
        // the same hand-over as a wrap: a jump in the system's buffer, PUT to ours
    *gContext->current = 0x20000000u | gCommandStart;
    __asm__ volatile("sync" ::: "memory");
    gContext->current++;
    control->put = gCommandStart;
    while (control->get != gCommandStart) {
        usleep(10);
        if (++waited == 200000) {
            printf("PS3GL: the RSX didn't move to our command buffer\n");
            return -1;
        }
    }
    gContext = &gCommands;
    return 0;
}

// The last few operations (not every draw), for the log when the RSX seems stuck
#define kTraceLines 24
static char gTrace[kTraceLines][64];
static unsigned gTraceNext;
static unsigned gDrawsSinceTrace;

static void Trace(const char *format, ...) __attribute__((format(printf, 1, 2)));
static void Trace(const char *format, ...)
{
    va_list args;
    char *line = gTrace[gTraceNext++ % kTraceLines];
    int n = snprintf(line, 64, "f%u d%u ", gFrame, gDrawsSinceTrace);

    gDrawsSinceTrace = 0;
    va_start(args, format);
    vsnprintf(line + n, 64 - n, format, args);
    va_end(args);
}

void PS3GLPrintTrace(FILE *f)
{
    unsigned i;

    fprintf(f, "PS3GL: the last operations (frame, draws before it, what):\n");
    for (i = 0; i < kTraceLines; i++) {
        const char *line = gTrace[(gTraceNext + i) % kTraceLines];
        if (line[0] != '\0')
            fprintf(f, "  %s\n", line);
    }
}

unsigned PS3GLNow(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

static void WaitIdle(void)
{
    int waited = 0;
    volatile const char *was = gPS3GLDoing;

    ROOM();
    gPS3GLDoing = "waiting for the RSX to finish";
    rsxSetWriteBackendLabel(gContext, kLabelIndex, gLabelValue);
    rsxFlushBuffer(gContext);
    while (*(vu32 *)gcmGetLabelAddress(kLabelIndex) != gLabelValue) {
        usleep(20);
        if (++waited == 100000)         // a couple of seconds at least: say so in the log
            printf("PS3GL: the RSX hasn't finished frame %u (label %u, waiting for %u)\n", gFrame,
                   *(vu32 *)gcmGetLabelAddress(kLabelIndex), gLabelValue);
    }
    gLabelValue++;
    gPS3GLDoing = was;
}

/// Makes the RSX finish drawing before it goes on (before reading what it drew as a texture).
static void RenderBarrier(void)
{
    ROOM();
    rsxSetWriteBackendLabel(gContext, kLabelIndex, gLabelValue);
    rsxSetWaitLabel(gContext, kLabelIndex, gLabelValue);
    gLabelValue++;
    gTextureCacheDirty = 1;
}

/// Before the PPU changes a texture: if the RSX may still read it this frame, let it finish.
static void WaitForTexture(Texture *t)
{
    if (t->usedInFrame == gFrame) {
        WaitIdle();
        gRingCount = gBatchStart = 0;
        gFrame++;
    }
}

// ---------------------------------------------------------------------------
// Matrices

static void MatIdentity(Mat4 *m)
{
    memset(m, 0, sizeof(*m));
    m->m[0] = m->m[5] = m->m[10] = m->m[15] = 1.0f;
}

static void MatMul(Mat4 *out, const Mat4 *a, const Mat4 *b)
{
    Mat4 r;
    int i, j, k;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) {
            float s = 0;
            for (k = 0; k < 4; k++)
                s += a->m[k * 4 + i] * b->m[j * 4 + k];
            r.m[j * 4 + i] = s;
        }
    *out = r;
}

static Mat4 *CurrentMatrix(void)
{
    return gMatrixMode == GL_PROJECTION ? &gProjection[gProjectionTop] : &gModelview[gModelviewTop];
}

static void MultCurrent(const Mat4 *m)
{
    Mat4 *cur = CurrentMatrix();
    MatMul(cur, cur, m);
    gMVPDirty = 1;
}

void glMatrixMode(GLenum mode)
{
    if (mode != GL_MODELVIEW && mode != GL_PROJECTION) {
        SetError(GL_INVALID_ENUM);
        return;
    }
    gMatrixMode = mode;
}

void glLoadIdentity(void)
{
    MatIdentity(CurrentMatrix());
    gMVPDirty = 1;
}

void glPushMatrix(void)
{
    if (gMatrixMode == GL_PROJECTION) {
        if (gProjectionTop + 1 >= kMaxProjection) {
            SetError(GL_STACK_OVERFLOW);
            return;
        }
        gProjection[gProjectionTop + 1] = gProjection[gProjectionTop];
        gProjectionTop++;
    } else {
        if (gModelviewTop + 1 >= kMaxModelview) {
            SetError(GL_STACK_OVERFLOW);
            return;
        }
        gModelview[gModelviewTop + 1] = gModelview[gModelviewTop];
        gModelviewTop++;
    }
}

void glPopMatrix(void)
{
    int *top = gMatrixMode == GL_PROJECTION ? &gProjectionTop : &gModelviewTop;

    if (*top == 0) {
        SetError(GL_STACK_UNDERFLOW);
        return;
    }
    (*top)--;
    gMVPDirty = 1;
}

void glOrtho(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f)
{
    Mat4 m;

    MatIdentity(&m);
    m.m[0] = (float)(2.0 / (r - l));
    m.m[5] = (float)(2.0 / (t - b));
    m.m[10] = (float)(-2.0 / (f - n));
    m.m[12] = (float)(-(r + l) / (r - l));
    m.m[13] = (float)(-(t + b) / (t - b));
    m.m[14] = (float)(-(f + n) / (f - n));
    MultCurrent(&m);
}

void glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
    Mat4 m;

    MatIdentity(&m);
    m.m[12] = x;
    m.m[13] = y;
    m.m[14] = z;
    MultCurrent(&m);
}

void glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    Mat4 m;

    MatIdentity(&m);
    m.m[0] = x;
    m.m[5] = y;
    m.m[10] = z;
    MultCurrent(&m);
}

void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    Mat4 m;
    float len = sqrtf(x * x + y * y + z * z), c, s, ic;

    if (len == 0)
        return;
    x /= len;
    y /= len;
    z /= len;
    c = cosf(angle * (float)M_PI / 180.0f);
    s = sinf(angle * (float)M_PI / 180.0f);
    ic = 1.0f - c;
    MatIdentity(&m);
    m.m[0] = x * x * ic + c;
    m.m[1] = y * x * ic + z * s;
    m.m[2] = x * z * ic - y * s;
    m.m[4] = x * y * ic - z * s;
    m.m[5] = y * y * ic + c;
    m.m[6] = y * z * ic + x * s;
    m.m[8] = x * z * ic + y * s;
    m.m[9] = y * z * ic - x * s;
    m.m[10] = z * z * ic + c;
    MultCurrent(&m);
}

// ---------------------------------------------------------------------------
// Simple state

void glEnable(GLenum cap)
{
    switch (cap) {
        case GL_TEXTURE_2D:     gTextureEnabled = 1; break;
        case GL_BLEND:          gBlendEnabled = 1; gStateDirty = 1; break;
        case GL_SCISSOR_TEST:   gScissorEnabled = 1; gTargetDirty = 1; break;
        case GL_DEPTH_TEST:     break;                  // never used: no depth buffer
        default:                SetError(GL_INVALID_ENUM); break;
    }
}

void glDisable(GLenum cap)
{
    switch (cap) {
        case GL_TEXTURE_2D:     gTextureEnabled = 0; break;
        case GL_BLEND:          gBlendEnabled = 0; gStateDirty = 1; break;
        case GL_SCISSOR_TEST:   gScissorEnabled = 0; gTargetDirty = 1; break;
        case GL_DEPTH_TEST:     break;
        default:                SetError(GL_INVALID_ENUM); break;
    }
}

void glBlendFunc(GLenum sfactor, GLenum dfactor)
{
    gBlendSrc = sfactor;
    gBlendDst = dfactor;
    gStateDirty = 1;
}

void glTexEnvi(GLenum target, GLenum pname, GLint param)
{
        // GL_MODULATE is the only mode the game uses, and the one the shaders do
    if (target != GL_TEXTURE_ENV || pname != GL_TEXTURE_ENV_MODE || param != GL_MODULATE)
        SetError(GL_INVALID_ENUM);
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    gViewport[0] = x;
    gViewport[1] = y;
    gViewport[2] = width;
    gViewport[3] = height;
    gTargetDirty = 1;
}

void glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    gScissor[0] = x;
    gScissor[1] = y;
    gScissor[2] = width;
    gScissor[3] = height;
    gTargetDirty = 1;
}

void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    gClearColor[0] = red;
    gClearColor[1] = green;
    gClearColor[2] = blue;
    gClearColor[3] = alpha;
}

void glPixelStorei(GLenum pname, GLint param)
{
    if (pname == GL_UNPACK_ROW_LENGTH)
        gUnpackRowLength = param;
    else if (pname == GL_UNPACK_ALIGNMENT)
        gUnpackAlignment = param;
    else
        SetError(GL_INVALID_ENUM);
}

void glGetIntegerv(GLenum pname, GLint *params)
{
    if (pname == GL_TEXTURE_BINDING_2D)
        *params = (GLint)gBoundTexture;
    else
        SetError(GL_INVALID_ENUM);
}

// ---------------------------------------------------------------------------
// Render targets: the screen's back buffer, or a texture (framebuffer object)

typedef struct {
    u32 offset, pitch, width, height;
    int flip;                   // rows stored top first (the screen)
} Target;

static Texture *TextureFor(GLuint name)
{
    if (name == 0 || name >= kMaxTextures || !gTextures[name].exists)
        return NULL;
    return &gTextures[name];
}

static void CurrentTarget(Target *t)
{
    if (gBoundFramebuffer != 0) {
        Texture *tex = TextureFor(gFramebuffers[gBoundFramebuffer].texture);
        if (tex != NULL && tex->mem != NULL) {
            t->offset = tex->offset;
            t->pitch = tex->pitch;
            t->width = tex->width;
            t->height = tex->height;
            t->flip = 0;
            return;
        }
    }
    t->offset = gOverscan > 0 ? gGameOffset : gColorOffset[gCurBuffer];
    t->pitch = gColorPitch;
    t->width = gScreenW;
    t->height = gScreenH;
    t->flip = 1;
}

static void EnsureDepthBuffer(u32 w, u32 h)
{
    if (gDepthBuffer != NULL && w <= gDepthW && h <= gDepthH)
        return;
    if (gDepthBuffer != NULL) {
        WaitIdle();
        rsxFree(gDepthBuffer);
    }
    gDepthW = w > gDepthW ? w : gDepthW;
    gDepthH = h > gDepthH ? h : gDepthH;
    gDepthPitch = (gDepthW * 4 + 63) & ~63u;
    gDepthBuffer = rsxMemalign(64, gDepthPitch * gDepthH);
    rsxAddressToOffset(gDepthBuffer, &gDepthOffset);
}

static void SetSurface(const Target *t)
{
    gcmSurface sf;

    ROOM();
    EnsureDepthBuffer(t->width, t->height);
    memset(&sf, 0, sizeof(sf));
    sf.colorFormat = GCM_SURFACE_A8R8G8B8;
    sf.colorTarget = GCM_SURFACE_TARGET_0;
    sf.colorLocation[0] = GCM_LOCATION_RSX;
    sf.colorOffset[0] = t->offset;
    sf.colorPitch[0] = t->pitch;
    sf.colorLocation[1] = sf.colorLocation[2] = sf.colorLocation[3] = GCM_LOCATION_RSX;
    sf.colorPitch[1] = sf.colorPitch[2] = sf.colorPitch[3] = 64;
    sf.depthFormat = GCM_SURFACE_ZETA_Z24S8;
    sf.depthLocation = GCM_LOCATION_RSX;
    sf.depthOffset = gDepthOffset;
    sf.depthPitch = gDepthPitch;
    sf.type = GCM_SURFACE_TYPE_LINEAR;
    sf.antiAlias = GCM_SURFACE_CENTER_1;
    sf.width = (u16)t->width;
    sf.height = (u16)t->height;
    sf.x = 0;
    sf.y = 0;
    rsxSetSurface(gContext, &sf);
}

/// A GL window rectangle (rows from the bottom) as rows of the target, clipped to it.
/// Returns 0 when nothing of it is left.
static int TargetRect(const Target *t, int x, int y, int w, int h, u16 out[4])
{
    int x0 = x, x1 = x + w, y0, y1;

    if (t->flip) {
        y0 = (int)t->height - (y + h);
        y1 = (int)t->height - y;
    } else {
        y0 = y;
        y1 = y + h;
    }
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 > (int)t->width) x1 = (int)t->width;
    if (y1 > (int)t->height) y1 = (int)t->height;
    if (x1 <= x0 || y1 <= y0) {
        out[0] = out[1] = out[2] = out[3] = 0;
        return 0;
    }
    out[0] = (u16)x0;
    out[1] = (u16)y0;
    out[2] = (u16)(x1 - x0);
    out[3] = (u16)(y1 - y0);
    return 1;
}

static void Intersect(u16 a[4], const u16 b[4])
{
    int x0 = a[0] > b[0] ? a[0] : b[0];
    int y0 = a[1] > b[1] ? a[1] : b[1];
    int x1 = a[0] + a[2] < b[0] + b[2] ? a[0] + a[2] : b[0] + b[2];
    int y1 = a[1] + a[3] < b[1] + b[3] ? a[1] + a[3] : b[1] + b[3];

    if (x1 <= x0 || y1 <= y0) {
        a[0] = a[1] = a[2] = a[3] = 0;
        return;
    }
    a[0] = (u16)x0;
    a[1] = (u16)y0;
    a[2] = (u16)(x1 - x0);
    a[3] = (u16)(y1 - y0);
}

/// Viewport and scissor for drawing: GL clips to the viewport, and to the scissor box when on.
/// Returns 0 when nothing can be drawn.
static int ApplyTarget(void)
{
    Target t;
    u16 vp[4], sc[4];
    float scale[4], offset[4];
    int visible;

    CurrentTarget(&t);
    visible = TargetRect(&t, gViewport[0], gViewport[1], gViewport[2], gViewport[3], vp);
    sc[0] = vp[0]; sc[1] = vp[1]; sc[2] = vp[2]; sc[3] = vp[3];
    if (gScissorEnabled) {
        u16 box[4];
        if (!TargetRect(&t, gScissor[0], gScissor[1], gScissor[2], gScissor[3], box))
            visible = 0;
        Intersect(sc, box);
    }
    if (!gTargetDirty)
        return visible && sc[2] > 0 && sc[3] > 0;
    gTargetDirty = 0;
    scale[0] = gViewport[2] * 0.5f;
    offset[0] = gViewport[0] + gViewport[2] * 0.5f;
    if (t.flip) {
        scale[1] = gViewport[3] * -0.5f;
        offset[1] = (float)t.height - gViewport[1] - gViewport[3] * 0.5f;
    } else {
        scale[1] = gViewport[3] * 0.5f;
        offset[1] = gViewport[1] + gViewport[3] * 0.5f;
    }
    scale[2] = 0.5f;
    offset[2] = 0.5f;
    scale[3] = offset[3] = 0.0f;
    rsxSetViewport(gContext, 0, 0, (u16)t.width, (u16)t.height, 0.0f, 1.0f, scale, offset);
    rsxSetScissor(gContext, sc[0], sc[1], sc[2], sc[3]);
    return visible && sc[2] > 0 && sc[3] > 0;
}

void glClear(GLbitfield mask)
{
    Target t;
    u16 sc[4];
    u32 color;

    if (!(mask & GL_COLOR_BUFFER_BIT))
        return;
    CurrentTarget(&t);
        // glClear ignores the viewport but keeps to the scissor box
    sc[0] = sc[1] = 0;
    sc[2] = (u16)t.width;
    sc[3] = (u16)t.height;
    if (gScissorEnabled && !TargetRect(&t, gScissor[0], gScissor[1], gScissor[2], gScissor[3], sc))
        return;
    color = ((u32)(gClearColor[3] * 255.0f + 0.5f) << 24) | ((u32)(gClearColor[0] * 255.0f + 0.5f) << 16) |
            ((u32)(gClearColor[1] * 255.0f + 0.5f) << 8) | (u32)(gClearColor[2] * 255.0f + 0.5f);
    rsxSetScissor(gContext, sc[0], sc[1], sc[2], sc[3]);
    Trace("clear %x fbo %u", (unsigned)color, gBoundFramebuffer);
    rsxSetClearColor(gContext, color);
    rsxSetColorMask(gContext, GCM_COLOR_MASK_R | GCM_COLOR_MASK_G | GCM_COLOR_MASK_B | GCM_COLOR_MASK_A);
    rsxClearSurface(gContext, GCM_CLEAR_R | GCM_CLEAR_G | GCM_CLEAR_B | GCM_CLEAR_A);
    gTargetDirty = 1;
    if (gBoundFramebuffer != 0) {
        Texture *tex = TextureFor(gFramebuffers[gBoundFramebuffer].texture);
        if (tex != NULL)
            tex->usedInFrame = gFrame;
    }
}

/// A new render-target texture starts transparent black, cleared by the RSX (not the CPU: to an
/// emulator, memory the RSX draws into belongs to the RSX).
static void ClearTarget(Texture *tex)
{
    Target t, cur;

    WaitForTexture(tex);
    t.offset = tex->offset;
    t.pitch = tex->pitch;
    t.width = (u32)tex->width;
    t.height = (u32)tex->height;
    t.flip = 0;
    RenderBarrier();
    SetSurface(&t);
    ROOM();
    rsxSetScissor(gContext, 0, 0, (u16)t.width, (u16)t.height);
    rsxSetClearColor(gContext, 0);
    rsxSetColorMask(gContext, GCM_COLOR_MASK_R | GCM_COLOR_MASK_G | GCM_COLOR_MASK_B | GCM_COLOR_MASK_A);
    rsxClearSurface(gContext, GCM_CLEAR_R | GCM_CLEAR_G | GCM_CLEAR_B | GCM_CLEAR_A);
    RenderBarrier();
    CurrentTarget(&cur);
    SetSurface(&cur);
    gStateDirty = gTargetDirty = 1;
}

// ---------------------------------------------------------------------------
// Textures

void glGenTextures(GLsizei n, GLuint *textures)
{
    GLuint name = 1;
    int i;

    for (i = 0; i < n; i++) {
        while (name < kMaxTextures && gTextures[name].exists)
            name++;
        if (name >= kMaxTextures) {
            SetError(GL_OUT_OF_MEMORY);
            textures[i] = 0;
            continue;
        }
        memset(&gTextures[name], 0, sizeof(Texture));
        gTextures[name].exists = 1;
        gTextures[name].linearFilter = 1;
        textures[i] = name;
    }
}

// Render-target memory is kept apart. A texture the RSX draws into (a copy of the screen, a
// framebuffer object) is a render target to an emulator: RPCS3 keeps its pixels on the host's
// graphics card, and when the same memory later holds an image written by the CPU, or a
// target of another size, it may go on showing the old target (the flickering and the black
// rooms). On a real PS3 it makes no difference.
static void *ArenaAlloc(u32 size)
{
    int i;

    size = (size + 127) & ~127u;
    for (i = 0; i < gArenaFreeCount; i++) {
        ArenaBlock *b = &gArenaFree[i];
        if (b->size >= size) {
            u32 start = b->start;
            b->start += size;
            b->size -= size;
            if (b->size == 0) {
                memmove(b, b + 1, (gArenaFreeCount - i - 1) * sizeof(*b));
                gArenaFreeCount--;
            }
            return gArena + start;
        }
    }
    return NULL;
}

static void ArenaFree(void *mem, u32 size)
{
    u32 start = (u32)((u8 *)mem - gArena);
    int i;

    size = (size + 127) & ~127u;
    for (i = 0; i < gArenaFreeCount && gArenaFree[i].start < start; i++)
        ;
    if (i > 0 && gArenaFree[i - 1].start + gArenaFree[i - 1].size == start) {
        gArenaFree[i - 1].size += size;         // joins the block before
        if (i < gArenaFreeCount && start + size == gArenaFree[i].start) {
            gArenaFree[i - 1].size += gArenaFree[i].size;
            memmove(&gArenaFree[i], &gArenaFree[i + 1], (gArenaFreeCount - i - 1) * sizeof(ArenaBlock));
            gArenaFreeCount--;
        }
        return;
    }
    if (i < gArenaFreeCount && start + size == gArenaFree[i].start) {
        gArenaFree[i].start = start;            // joins the block after
        gArenaFree[i].size += size;
        return;
    }
    if (gArenaFreeCount == kMaxArenaBlocks)
        return;                                 // too scattered: this block stays lost
    memmove(&gArenaFree[i + 1], &gArenaFree[i], (gArenaFreeCount - i) * sizeof(ArenaBlock));
    gArenaFree[i].start = start;
    gArenaFree[i].size = size;
    gArenaFreeCount++;
}

static int IsArenaMemory(void *mem)
{
    return gArena != NULL && (u8 *)mem >= gArena && (u8 *)mem < gArena + kTargetArenaSize;
}

static void FreeTextureMemory(Texture *t)
{
    if (t->mem != NULL) {
        WaitForTexture(t);
        if (IsArenaMemory(t->mem)) {
            ArenaFree(t->mem, t->size);
            gTargetBytes -= t->size;
        } else {
            rsxFree(t->mem);
            gTextureBytes -= t->size;
        }
        t->mem = NULL;
    }
}

static void *AllocTextureMemory(Texture *t, u32 size, int target)
{
    void *mem = NULL;

    if (target) {
        mem = ArenaAlloc(size);
        if (mem == NULL) {
            static int warned;
            if (!warned++)
                fprintf(stderr, "PS3GL: the render-target arena is full (%u KB used)\n", gTargetBytes / 1024);
        } else {
            gTargetBytes += size;
        }
    }
    if (mem == NULL) {
        mem = rsxMemalign(128, size);
        if (mem != NULL)
            gTextureBytes += size;
    }
    t->size = size;
    return mem;
}

void glDeleteTextures(GLsizei n, const GLuint *textures)
{
    int i, f;

    for (i = 0; i < n; i++) {
        Texture *t = TextureFor(textures[i]);
        if (t == NULL)
            continue;
        FreeTextureMemory(t);
        t->exists = 0;
        if (gBoundTexture == textures[i])
            gBoundTexture = 0;
        if (gCurTexture == textures[i])
            gCurTexture = (GLuint)-1;
        for (f = 1; f < kMaxFramebuffers; f++)
            if (gFramebuffers[f].texture == textures[i])
                gFramebuffers[f].texture = 0;
    }
}

void glBindTexture(GLenum target, GLuint texture)
{
    if (target != GL_TEXTURE_2D) {
        SetError(GL_INVALID_ENUM);
        return;
    }
    if (texture != 0 && TextureFor(texture) == NULL) {
            // a name glGenTextures didn't hand out: GL would make it; the game doesn't do this
        SetError(GL_INVALID_OPERATION);
        return;
    }
    gBoundTexture = texture;
}

void glTexParameteri(GLenum target, GLenum pname, GLint param)
{
    Texture *t = TextureFor(gBoundTexture);

    if (target != GL_TEXTURE_2D || t == NULL) {
        SetError(GL_INVALID_OPERATION);
        return;
    }
    switch (pname) {
        case GL_TEXTURE_MIN_FILTER:
        case GL_TEXTURE_MAG_FILTER:
            t->linearFilter = param != GL_NEAREST;
            break;
        case GL_TEXTURE_WRAP_S:
        case GL_TEXTURE_WRAP_T:
            t->repeat = param == GL_REPEAT;
            break;
        default:
            SetError(GL_INVALID_ENUM);
            return;
    }
    if (gCurTexture == gBoundTexture)
        gCurTexture = (GLuint)-1;
}

/// Converts rows of GL pixels into the texture's A8R8G8B8 rows (a word at a time: RSX memory
/// is slow to write byte by byte).
static void StorePixels(Texture *t, int x, int y, int w, int h, GLenum format, const void *pixels)
{
    const unsigned char *src = pixels;
    int rowLength = gUnpackRowLength > 0 ? gUnpackRowLength : w;
    int bpp = format == GL_ALPHA ? 1 : 4;
    size_t srcPitch = (size_t)rowLength * bpp;
    int row, col;

    if (gUnpackAlignment > 1)
        srcPitch = (srcPitch + gUnpackAlignment - 1) / gUnpackAlignment * gUnpackAlignment;
    for (row = 0; row < h; row++) {
        const unsigned char *s = src + row * srcPitch;
        u32 *d = (u32 *)((unsigned char *)t->mem + (size_t)(y + row) * t->pitch) + x;
        if (format == GL_ALPHA) {
            for (col = 0; col < w; col++)
                d[col] = ((u32)s[col] << 24) | 0x00ffffffu;
        } else if (((uintptr_t)s & 3) == 0) {
            const u32 *s32 = (const u32 *)s;        // R,G,B,A bytes: 0xRRGGBBAA on the PPU
            for (col = 0; col < w; col++)
                d[col] = (s32[col] >> 8) | (s32[col] << 24);
        } else {
            for (col = 0; col < w; col++, s += 4)
                d[col] = ((u32)s[3] << 24) | ((u32)s[0] << 16) | ((u32)s[1] << 8) | s[2];
        }
    }
}

void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height,
                  GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
    Texture *t = TextureFor(gBoundTexture);
    u32 pitch;

    (void)internalformat;
    if (target != GL_TEXTURE_2D || level != 0 || border != 0 || type != GL_UNSIGNED_BYTE ||
        (format != GL_RGBA && format != GL_ALPHA)) {
        SetError(GL_INVALID_ENUM);
        return;
    }
    if (t == NULL || width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        SetError(GL_INVALID_VALUE);
        return;
    }
    pitch = ((u32)width * 4 + 63) & ~63u;      // a multiple of 64: it can also be drawn into
    if (t->mem == NULL || t->size < pitch * (u32)height || t->target != (pixels == NULL)) {
            // a texture made without pixels is one the RSX will draw into: render-target memory
        FreeTextureMemory(t);
        t->target = pixels == NULL;
        t->mem = AllocTextureMemory(t, pitch * (u32)height, t->target);
        if (t->mem == NULL) {
            fprintf(stderr, "PS3GL: out of RSX memory for a %dx%d texture\n", width, height);
            SetError(GL_OUT_OF_MEMORY);
            return;
        }
        rsxAddressToOffset(t->mem, &t->offset);
    } else {
        WaitForTexture(t);
    }
    t->width = width;
    t->height = height;
    t->pitch = pitch;
    Trace("teximage %u %dx%d", gBoundTexture, width, height);
    if (pixels != NULL)
        StorePixels(t, 0, 0, width, height, format, pixels);
    else
        ClearTarget(t);
    gTextureCacheDirty = 1;
    if (gCurTexture == gBoundTexture)
        gCurTexture = (GLuint)-1;
}

void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width,
                     GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
    Texture *t = TextureFor(gBoundTexture);

    if (target != GL_TEXTURE_2D || level != 0 || type != GL_UNSIGNED_BYTE ||
        (format != GL_RGBA && format != GL_ALPHA)) {
        SetError(GL_INVALID_ENUM);
        return;
    }
    if (t == NULL || t->mem == NULL || xoffset < 0 || yoffset < 0 || xoffset + width > t->width ||
        yoffset + height > t->height) {
        SetError(GL_INVALID_VALUE);
        return;
    }
    WaitForTexture(t);
    StorePixels(t, xoffset, yoffset, width, height, format, pixels);
    gTextureCacheDirty = 1;
}

static void LoadTextureUnit(u8 unit, u32 offset, u32 w, u32 h, u32 pitch, int linear)
{
    gcmTexture tex;

    memset(&tex, 0, sizeof(tex));
    tex.format = GCM_TEXTURE_FORMAT_A8R8G8B8 | GCM_TEXTURE_FORMAT_LIN;
    tex.mipmap = 1;
    tex.dimension = GCM_TEXTURE_DIMS_2D;
    tex.cubemap = GCM_FALSE;
    tex.remap = (GCM_TEXTURE_REMAP_TYPE_REMAP << GCM_TEXTURE_REMAP_TYPE_B_SHIFT) |
                (GCM_TEXTURE_REMAP_TYPE_REMAP << GCM_TEXTURE_REMAP_TYPE_G_SHIFT) |
                (GCM_TEXTURE_REMAP_TYPE_REMAP << GCM_TEXTURE_REMAP_TYPE_R_SHIFT) |
                (GCM_TEXTURE_REMAP_TYPE_REMAP << GCM_TEXTURE_REMAP_TYPE_A_SHIFT) |
                (GCM_TEXTURE_REMAP_COLOR_B << GCM_TEXTURE_REMAP_COLOR_B_SHIFT) |
                (GCM_TEXTURE_REMAP_COLOR_G << GCM_TEXTURE_REMAP_COLOR_G_SHIFT) |
                (GCM_TEXTURE_REMAP_COLOR_R << GCM_TEXTURE_REMAP_COLOR_R_SHIFT) |
                (GCM_TEXTURE_REMAP_COLOR_A << GCM_TEXTURE_REMAP_COLOR_A_SHIFT);
    tex.width = (u16)w;
    tex.height = (u16)h;
    tex.depth = 1;
    tex.pitch = pitch;
    tex.location = GCM_LOCATION_RSX;
    tex.offset = offset;
    rsxLoadTexture(gContext, unit, &tex);
    rsxTextureControl(gContext, unit, GCM_TRUE, 0 << 8, 12 << 8, GCM_TEXTURE_MAX_ANISO_1);
    rsxTextureFilter(gContext, unit, 0, linear ? GCM_TEXTURE_LINEAR : GCM_TEXTURE_NEAREST,
                     linear ? GCM_TEXTURE_LINEAR : GCM_TEXTURE_NEAREST, GCM_TEXTURE_CONVOLUTION_QUINCUNX);
    rsxTextureWrapMode(gContext, unit, GCM_TEXTURE_CLAMP_TO_EDGE, GCM_TEXTURE_CLAMP_TO_EDGE,
                       GCM_TEXTURE_CLAMP_TO_EDGE, GCM_TEXTURE_UNSIGNED_REMAP_NORMAL, GCM_TEXTURE_ZFUNC_NEVER, 0);
}

// ---------------------------------------------------------------------------
// Drawing

static u32 GCMBlendFactor(GLenum f)
{
    switch (f) {
        case GL_ZERO:                   return GCM_ZERO;
        case GL_ONE:                    return GCM_ONE;
        case GL_SRC_COLOR:              return GCM_SRC_COLOR;
        case GL_ONE_MINUS_SRC_COLOR:    return GCM_ONE_MINUS_SRC_COLOR;
        case GL_SRC_ALPHA:              return GCM_SRC_ALPHA;
        case GL_ONE_MINUS_SRC_ALPHA:    return GCM_ONE_MINUS_SRC_ALPHA;
        case GL_DST_ALPHA:              return GCM_DST_ALPHA;
        case GL_ONE_MINUS_DST_ALPHA:    return GCM_ONE_MINUS_DST_ALPHA;
        case GL_DST_COLOR:              return GCM_DST_COLOR;
        case GL_ONE_MINUS_DST_COLOR:    return GCM_ONE_MINUS_DST_COLOR;
    }
    SetError(GL_INVALID_ENUM);
    return GCM_ONE;
}

/// Sends the state the next draw needs. Returns 0 when it can't draw (no room on screen).
static int PrepareDraw(void)
{
    Texture *t = gTextureEnabled ? TextureFor(gBoundTexture) : NULL;
    int prog;

    ROOM();
    if (gTextureEnabled && (t == NULL || t->mem == NULL))
        t = NULL;           // GL samples an incomplete texture as black; drawing nothing is close
    if (gTextureEnabled && t == NULL)
        return 0;
    if (!ApplyTarget())
        return 0;
    if (gStateDirty) {
        gStateDirty = 0;
        rsxSetColorMask(gContext, GCM_COLOR_MASK_R | GCM_COLOR_MASK_G | GCM_COLOR_MASK_B | GCM_COLOR_MASK_A);
        rsxSetDepthTestEnable(gContext, GCM_FALSE);
        rsxSetDepthWriteEnable(gContext, GCM_FALSE);
        rsxSetCullFaceEnable(gContext, GCM_FALSE);
        rsxSetShadeModel(gContext, GCM_SHADE_MODEL_SMOOTH);
        rsxSetLogicOpEnable(gContext, GCM_FALSE);
        if (gBlendEnabled) {
            u32 s = GCMBlendFactor(gBlendSrc), d = GCMBlendFactor(gBlendDst);
            rsxSetBlendFunc(gContext, s, d, s, d);
            rsxSetBlendEquation(gContext, GCM_FUNC_ADD, GCM_FUNC_ADD);
            rsxSetBlendEnable(gContext, GCM_TRUE);
        } else {
            rsxSetBlendEnable(gContext, GCM_FALSE);
        }
        rsxLoadVertexProgram(gContext, gVP, gVPUCode);
        gCurProg = -1;
    }
    prog = t == NULL ? kProgColor : (t->repeat ? kProgRepeat : kProgTexture);
    if (prog != gCurProg) {
        rsxLoadFragmentProgramLocation(gContext, gFP[prog].fp, gFP[prog].offset, GCM_LOCATION_RSX);
        gCurProg = prog;
        gCurTexture = (GLuint)-1;
    }
    if (t != NULL) {
        if (gTextureCacheDirty) {
            rsxInvalidateTextureCache(gContext, GCM_INVALIDATE_TEXTURE);
            gTextureCacheDirty = 0;
        }
        if (gCurTexture != gBoundTexture) {
            LoadTextureUnit(gFP[prog].texUnit->index, t->offset, t->width, t->height, t->pitch, t->linearFilter);
            gCurTexture = gBoundTexture;
        }
        t->usedInFrame = gFrame;
    }
    return 1;
}

static void FlushBatch(void)
{
    u32 count = gRingCount - gBatchStart;
    u32 offset;

    if (count == 0)
        return;
    if (PrepareDraw()) {
        offset = gRingOffset + gBatchStart * sizeof(Vertex);
        __asm__ volatile("sync" ::: "memory");      // the vertices reach RSX memory first
        ROOM();
        rsxBindVertexArrayAttrib(gContext, gPosAttr->index, 0, offset, sizeof(Vertex), 4,
                                 GCM_VERTEX_DATA_TYPE_F32, GCM_LOCATION_RSX);
        rsxBindVertexArrayAttrib(gContext, gTexAttr->index, 0, offset + 16, sizeof(Vertex), 2,
                                 GCM_VERTEX_DATA_TYPE_F32, GCM_LOCATION_RSX);
        rsxBindVertexArrayAttrib(gContext, gColAttr->index, 0, offset + 24, sizeof(Vertex), 4,
                                 GCM_VERTEX_DATA_TYPE_F32, GCM_LOCATION_RSX);
        rsxInvalidateVertexCache(gContext);
        rsxDrawVertexArray(gContext, GCM_TYPE_TRIANGLES, 0, count);
        gDrawsSinceTrace++;
        if (gBoundFramebuffer != 0) {
            Texture *target = TextureFor(gFramebuffers[gBoundFramebuffer].texture);
            if (target != NULL)
                target->usedInFrame = gFrame;
        }
    }
    gBatchStart = gRingCount;
}

static void EmitVertex(const Vertex *v)
{
    if (gRingCount >= gRingCapacity) {
            // the ring is full: draw what's there, let the RSX finish, start over
        FlushBatch();
        WaitIdle();
        gRingCount = gBatchStart = 0;
        gFrame++;
    }
    gRing[gRingCount++] = *v;
}

static void EmitTriangle(const Vertex *a, const Vertex *b, const Vertex *c)
{
    if (gRingCount + 3 > gRingCapacity) {
        FlushBatch();
        WaitIdle();
        gRingCount = gBatchStart = 0;
        gFrame++;
    }
    EmitVertex(a);
    EmitVertex(b);
    EmitVertex(c);
}

void glBegin(GLenum mode)
{
    if (gInBegin || (mode != GL_TRIANGLES && mode != GL_TRIANGLE_STRIP && mode != GL_QUADS)) {
        SetError(gInBegin ? GL_INVALID_OPERATION : GL_INVALID_ENUM);
        return;
    }
    if (gMVPDirty) {
        MatMul(&gMVP, &gProjection[gProjectionTop], &gModelview[gModelviewTop]);
        gMVPDirty = 0;
    }
    gPrimitive = mode;
    gInBegin = 1;
    gPendingCount = 0;
    gStripParity = 0;
    gBatchStart = gRingCount;
}

void glEnd(void)
{
    if (!gInBegin) {
        SetError(GL_INVALID_OPERATION);
        return;
    }
    gInBegin = 0;
    FlushBatch();
}

void glVertex2f(GLfloat x, GLfloat y)
{
    Vertex v;
    const float *m = gMVP.m;

    if (!gInBegin)
        return;
    v.x = m[0] * x + m[4] * y + m[12];
    v.y = m[1] * x + m[5] * y + m[13];
    v.z = m[2] * x + m[6] * y + m[14];
    v.w = m[3] * x + m[7] * y + m[15];
    v.u = gTexCoord[0];
    v.v = gTexCoord[1];
    v.r = gColor[0];
    v.g = gColor[1];
    v.b = gColor[2];
    v.a = gColor[3];
    switch (gPrimitive) {
        case GL_TRIANGLES:
            gPending[gPendingCount++] = v;
            if (gPendingCount == 3) {
                EmitTriangle(&gPending[0], &gPending[1], &gPending[2]);
                gPendingCount = 0;
            }
            break;
        case GL_QUADS:
            gPending[gPendingCount++] = v;
            if (gPendingCount == 4) {
                EmitTriangle(&gPending[0], &gPending[1], &gPending[2]);
                EmitTriangle(&gPending[0], &gPending[2], &gPending[3]);
                gPendingCount = 0;
            }
            break;
        case GL_TRIANGLE_STRIP:
            if (gPendingCount < 2) {
                gPending[gPendingCount++] = v;
                break;
            }
            if (gStripParity == 0)
                EmitTriangle(&gPending[0], &gPending[1], &v);
            else
                EmitTriangle(&gPending[1], &gPending[0], &v);
            gStripParity ^= 1;
            gPending[0] = gPending[1];
            gPending[1] = v;
            break;
    }
}

void glVertex2i(GLint x, GLint y)
{
    glVertex2f((GLfloat)x, (GLfloat)y);
}

void glTexCoord2f(GLfloat s, GLfloat t)
{
    gTexCoord[0] = s;
    gTexCoord[1] = t;
}

void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    gColor[0] = red;
    gColor[1] = green;
    gColor[2] = blue;
    gColor[3] = alpha;
}

void glColor4us(GLushort red, GLushort green, GLushort blue, GLushort alpha)
{
    glColor4f(red / 65535.0f, green / 65535.0f, blue / 65535.0f, alpha / 65535.0f);
}

void glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
    glColor4f(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
}

void glColor3ub(GLubyte red, GLubyte green, GLubyte blue)
{
    glColor4f(red / 255.0f, green / 255.0f, blue / 255.0f, 1.0f);
}

void glRecti(GLint x1, GLint y1, GLint x2, GLint y2)
{
    glBegin(GL_QUADS);
    glVertex2i(x1, y1);
    glVertex2i(x2, y1);
    glVertex2i(x2, y2);
    glVertex2i(x1, y2);
    glEnd();
}

// ---------------------------------------------------------------------------
// Copying from the current buffer, and framebuffer objects

/// Draws a rectangle of one target (s, t: 0..1, rows as stored) into a rectangle of another
/// (pixels, rows as stored), replacing what is there. Leaves the destination as the surface.
static void DrawTargetRect(const Target *src, const Target *dt, int prog, int linear, float s0, float t0,
                           float s1, float t1, float x0, float y0, float x1, float y1)
{
    float scale[4], offset[4];
    Vertex q[4];
    int i;

    RenderBarrier();
    SetSurface(dt);
    ROOM();
    scale[0] = dt->width * 0.5f;
    offset[0] = dt->width * 0.5f;
    scale[1] = dt->height * 0.5f;
    offset[1] = dt->height * 0.5f;
    scale[2] = offset[2] = 0.5f;
    scale[3] = offset[3] = 0.0f;
    rsxSetViewport(gContext, 0, 0, (u16)dt->width, (u16)dt->height, 0.0f, 1.0f, scale, offset);
    rsxSetScissor(gContext, 0, 0, (u16)dt->width, (u16)dt->height);
    rsxSetColorMask(gContext, GCM_COLOR_MASK_R | GCM_COLOR_MASK_G | GCM_COLOR_MASK_B | GCM_COLOR_MASK_A);
    rsxSetBlendEnable(gContext, GCM_FALSE);
    rsxSetDepthTestEnable(gContext, GCM_FALSE);
    rsxSetCullFaceEnable(gContext, GCM_FALSE);
    rsxLoadVertexProgram(gContext, gVP, gVPUCode);
    rsxLoadFragmentProgramLocation(gContext, gFP[prog].fp, gFP[prog].offset, GCM_LOCATION_RSX);
    rsxInvalidateTextureCache(gContext, GCM_INVALIDATE_TEXTURE);
    LoadTextureUnit(gFP[prog].texUnit->index, src->offset, src->width, src->height, src->pitch, linear);
    for (i = 0; i < 4; i++) {
        int right = i == 1 || i == 2, top = i >= 2;
        float px = right ? x1 : x0, py = top ? y1 : y0;
        q[i].x = px / dt->width * 2.0f - 1.0f;
        q[i].y = py / dt->height * 2.0f - 1.0f;
        q[i].z = 0.0f;
        q[i].w = 1.0f;
        q[i].u = right ? s1 : s0;
        q[i].v = top ? t1 : t0;
        q[i].r = q[i].g = q[i].b = q[i].a = 1.0f;
    }
    gBatchStart = gRingCount;
    EmitTriangle(&q[0], &q[1], &q[2]);
    EmitTriangle(&q[0], &q[2], &q[3]);
    {
        u32 voff = gRingOffset + gBatchStart * sizeof(Vertex);
        ROOM();
        __asm__ volatile("sync" ::: "memory");
        rsxBindVertexArrayAttrib(gContext, gPosAttr->index, 0, voff, sizeof(Vertex), 4, GCM_VERTEX_DATA_TYPE_F32, GCM_LOCATION_RSX);
        rsxBindVertexArrayAttrib(gContext, gTexAttr->index, 0, voff + 16, sizeof(Vertex), 2, GCM_VERTEX_DATA_TYPE_F32, GCM_LOCATION_RSX);
        rsxBindVertexArrayAttrib(gContext, gColAttr->index, 0, voff + 24, sizeof(Vertex), 4, GCM_VERTEX_DATA_TYPE_F32, GCM_LOCATION_RSX);
        rsxInvalidateVertexCache(gContext);
        rsxDrawVertexArray(gContext, GCM_TYPE_TRIANGLES, 0, 6);
    }
    gBatchStart = gRingCount;
    RenderBarrier();
}

void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y,
                         GLsizei width, GLsizei height)
{
    Texture *dst = TextureFor(gBoundTexture);
    Target src, dt;
    float s0, s1, t0, t1;

    if (target != GL_TEXTURE_2D || level != 0 || dst == NULL || dst->mem == NULL || width <= 0 || height <= 0) {
        SetError(GL_INVALID_VALUE);
        return;
    }
    CurrentTarget(&src);
    if (src.offset == dst->offset) {
        SetError(GL_INVALID_OPERATION);
        return;
    }
        // only what is on the source can be copied (GL leaves the rest undefined)
    if (x < 0) { xoffset -= x; width += x; x = 0; }
    if (y < 0) { yoffset -= y; height += y; y = 0; }
    if (x + width > (int)src.width) width = (int)src.width - x;
    if (y + height > (int)src.height) height = (int)src.height - y;
    if (xoffset + width > dst->width) width = dst->width - xoffset;
    if (yoffset + height > dst->height) height = dst->height - yoffset;
    if (width <= 0 || height <= 0)
        return;
        // draw the source rectangle into the texture, one texel per pixel
    Trace("copy %d,%d %dx%d to %u", x, y, width, height, gBoundTexture);
    dt.offset = dst->offset;
    dt.pitch = dst->pitch;
    dt.width = dst->width;
    dt.height = dst->height;
    dt.flip = 0;
    s0 = (float)x / src.width;
    s1 = (float)(x + width) / src.width;
    if (src.flip) {
        t0 = (float)(src.height - y) / src.height;              // GL row y, from the top
        t1 = (float)(src.height - (y + height)) / src.height;
    } else {
        t0 = (float)y / src.height;
        t1 = (float)(y + height) / src.height;
    }
        // the screen has no alpha to copy (as a computer's window): the copy is opaque
    DrawTargetRect(&src, &dt, src.flip ? kProgOpaque : kProgTexture, 0, s0, t0, s1, t1,
                   (float)xoffset, (float)yoffset, (float)(xoffset + width), (float)(yoffset + height));
    dst->usedInFrame = gFrame;
        // back to the current buffer, with everything sent again before the next draw
    SetSurface(&src);
    gStateDirty = gTargetDirty = 1;
    gCurProg = -1;
    gCurTexture = (GLuint)-1;
}

void glGenFramebuffers(GLsizei n, GLuint *framebuffers)
{
    GLuint name = 1;
    int i;

    for (i = 0; i < n; i++) {
        while (name < kMaxFramebuffers && gFramebuffers[name].exists)
            name++;
        if (name >= kMaxFramebuffers) {
            SetError(GL_OUT_OF_MEMORY);
            framebuffers[i] = 0;
            continue;
        }
        gFramebuffers[name].exists = 1;
        gFramebuffers[name].texture = 0;
        framebuffers[i] = name;
    }
}

void glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{
    int i;

    for (i = 0; i < n; i++) {
        GLuint f = framebuffers[i];
        if (f == 0 || f >= kMaxFramebuffers)
            continue;
        if (gBoundFramebuffer == f)
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        gFramebuffers[f].exists = 0;
    }
}

void glBindFramebuffer(GLenum target, GLuint framebuffer)
{
    Target t;

    if (target != GL_FRAMEBUFFER || framebuffer >= kMaxFramebuffers ||
        (framebuffer != 0 && !gFramebuffers[framebuffer].exists)) {
        SetError(GL_INVALID_OPERATION);
        return;
    }
    if (framebuffer == gBoundFramebuffer)
        return;
    Trace("bind fbo %u (texture %u)", framebuffer, framebuffer ? gFramebuffers[framebuffer].texture : 0);
    if (gBoundFramebuffer != 0)
        RenderBarrier();        // what was drawn into the texture is ready to be read
    gBoundFramebuffer = framebuffer;
    CurrentTarget(&t);
    SetSurface(&t);
    gTargetDirty = 1;
}

void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    Target t;

    if (target != GL_FRAMEBUFFER || attachment != GL_COLOR_ATTACHMENT0 || textarget != GL_TEXTURE_2D ||
        level != 0 || gBoundFramebuffer == 0) {
        SetError(GL_INVALID_OPERATION);
        return;
    }
    gFramebuffers[gBoundFramebuffer].texture = texture;
    CurrentTarget(&t);
    SetSurface(&t);
    gTargetDirty = 1;
}

GLenum glCheckFramebufferStatus(GLenum target)
{
    Texture *t;

    if (target != GL_FRAMEBUFFER || gBoundFramebuffer == 0)
        return GL_FRAMEBUFFER_COMPLETE;
    t = TextureFor(gFramebuffers[gBoundFramebuffer].texture);
    return (t != NULL && t->mem != NULL) ? GL_FRAMEBUFFER_COMPLETE : GL_FRAMEBUFFER_UNSUPPORTED;
}

void *PS3GLGetProcAddress(const char *name)
{
    static const struct { const char *name; void *proc; } procs[] = {
        { "glGenFramebuffers", (void *)glGenFramebuffers },
        { "glDeleteFramebuffers", (void *)glDeleteFramebuffers },
        { "glBindFramebuffer", (void *)glBindFramebuffer },
        { "glFramebufferTexture2D", (void *)glFramebufferTexture2D },
        { "glCheckFramebufferStatus", (void *)glCheckFramebufferStatus },
    };
    size_t i;

    for (i = 0; i < sizeof(procs) / sizeof(procs[0]); i++)
        if (strcmp(name, procs[i].name) == 0)
            return procs[i].proc;
    return NULL;
}

// ---------------------------------------------------------------------------
// Start up, frames

static int LoadPrograms(void)
{
    static const void *fpos[kNumProgs] = { ps3gl_color_fp_fpo, ps3gl_texture_fp_fpo, ps3gl_repeat_fp_fpo,
                                           ps3gl_opaque_fp_fpo };
    void *ucode;
    u32 size;
    int i;

    gVP = (rsxVertexProgram *)ps3gl_vp_vpo;
    rsxVertexProgramGetUCode(gVP, &gVPUCode, &size);
    gPosAttr = rsxVertexProgramGetAttrib(gVP, "position");
    gColAttr = rsxVertexProgramGetAttrib(gVP, "color");
    gTexAttr = rsxVertexProgramGetAttrib(gVP, "texcoord");
    if (gPosAttr == NULL || gColAttr == NULL || gTexAttr == NULL)
        return -1;
    for (i = 0; i < kNumProgs; i++) {
        void *mem;
        gFP[i].fp = (rsxFragmentProgram *)fpos[i];
        rsxFragmentProgramGetUCode(gFP[i].fp, &ucode, &size);
        mem = rsxMemalign(64, size);
        if (mem == NULL)
            return -1;
        memcpy(mem, ucode, size);
        rsxAddressToOffset(mem, &gFP[i].offset);
        gFP[i].texUnit = i == kProgColor ? NULL : rsxFragmentProgramGetAttrib(gFP[i].fp, "texture");
        if (i != kProgColor && gFP[i].texUnit == NULL)
            return -1;
    }
    return 0;
}

static int SetUpVideo(void)
{
    videoState state;
    videoResolution res;
    videoConfiguration config;

    if (videoGetState(0, 0, &state) != 0 || videoGetResolution(state.displayMode.resolution, &res) != 0)
        return -1;
    memset(&config, 0, sizeof(config));
    config.resolution = state.displayMode.resolution;
    config.format = VIDEO_BUFFER_FORMAT_XRGB;
    config.pitch = res.width * 4;
    config.aspect = VIDEO_ASPECT_AUTO;
    if (videoConfigure(0, &config, NULL, 0) != 0)
        return -1;
    gScreenW = res.width;
    gScreenH = res.height;
        // the TV's shape (the XMB's TV settings): SD modes have pixels that aren't square
    gDisplayAspect = state.displayMode.aspect == VIDEO_ASPECT_4_3 ? 4.0f / 3.0f : 16.0f / 9.0f;
    printf("PS3GL: TV %ux%u, %s\n", gScreenW, gScreenH, state.displayMode.aspect == VIDEO_ASPECT_4_3 ? "4:3" : "16:9");
    return 0;
}

float PS3GLPixelAspect(void)
{
    return gScreenW > 0 && gScreenH > 0 ? gDisplayAspect / ((float)gScreenW / (float)gScreenH) : 1.0f;
}

int PS3GLInit(void)
{
    int i;
    Target t;

    gHostAddr = memalign(1024 * 1024, kIOSize);
    if (gHostAddr == NULL || rsxInit(&gContext, kSystemBufferSize, kIOSize, gHostAddr) != 0 || gContext == NULL) {
        fprintf(stderr, "PS3GL: rsxInit failed\n");
        return -1;
    }
    if (UseOwnCommandBuffer() != 0)
        return -1;
    if (SetUpVideo() != 0) {
        fprintf(stderr, "PS3GL: no video mode\n");
        return -1;
    }
    WaitIdle();
    gcmSetFlipMode(GCM_FLIP_VSYNC);
    gColorPitch = gScreenW * 4;
    for (i = 0; i < 2; i++) {
        gColorBuffer[i] = rsxMemalign(64, gColorPitch * gScreenH);
        if (gColorBuffer[i] == NULL)
            return -1;
        memset(gColorBuffer[i], 0, gColorPitch * gScreenH);
        rsxAddressToOffset(gColorBuffer[i], &gColorOffset[i]);
        gcmSetDisplayBuffer(i, gColorOffset[i], gColorPitch, gScreenW, gScreenH);
    }
    EnsureDepthBuffer(gScreenW, gScreenH);
    gRingCapacity = kVertexRingSize / sizeof(Vertex);
    gRing = rsxMemalign(128, gRingCapacity * sizeof(Vertex));
    if (gRing == NULL)
        return -1;
    rsxAddressToOffset(gRing, &gRingOffset);
    gArena = rsxMemalign(128, kTargetArenaSize);
    if (gArena != NULL) {
        gArenaFree[0].start = 0;
        gArenaFree[0].size = kTargetArenaSize;
        gArenaFreeCount = 1;
    }
    if (LoadPrograms() != 0) {
        fprintf(stderr, "PS3GL: the shaders didn't load\n");
        return -1;
    }
    MatIdentity(&gModelview[0]);
    MatIdentity(&gProjection[0]);
    gViewport[2] = gScissor[2] = (GLint)gScreenW;
    gViewport[3] = gScissor[3] = (GLint)gScreenH;
    gCurBuffer = 0;
    CurrentTarget(&t);
    SetSurface(&t);
    printf("PS3GL: %ux%u, %u KB of RSX memory for vertices\n", gScreenW, gScreenH, kVertexRingSize / 1024);
    return 0;
}

void PS3GLShutdown(void)
{
    if (gContext == NULL)
        return;
    ROOM();
    gcmSetWaitFlip(gContext);
    rsxFinish(gContext, 1);
}

void PS3GLGetScreenSize(int *w, int *h)
{
    *w = (int)gScreenW;
    *h = (int)gScreenH;
}

/// Overscan: the game's picture, smaller and centered on black, on the screen's buffer.
static void ShowGameBuffer(void)
{
    Target src, dt;
    float mx = gScreenW * gOverscan / 2.0f, my = gScreenH * gOverscan / 2.0f;

    FlushBatch();
    src.offset = gGameOffset;
    src.pitch = gColorPitch;
    src.width = gScreenW;
    src.height = gScreenH;
    src.flip = 1;
    dt = src;
    dt.offset = gColorOffset[gCurBuffer];
    RenderBarrier();
    SetSurface(&dt);
    ROOM();
    rsxSetScissor(gContext, 0, 0, (u16)gScreenW, (u16)gScreenH);
    rsxSetClearColor(gContext, 0xff000000);
    rsxSetColorMask(gContext, GCM_COLOR_MASK_R | GCM_COLOR_MASK_G | GCM_COLOR_MASK_B | GCM_COLOR_MASK_A);
    rsxClearSurface(gContext, GCM_CLEAR_R | GCM_CLEAR_G | GCM_CLEAR_B | GCM_CLEAR_A);
    DrawTargetRect(&src, &dt, kProgOpaque, 1, 0.0f, 0.0f, 1.0f, 1.0f, mx, my, gScreenW - mx, gScreenH - my);
}

void PS3GLSetOverscan(float fraction)
{
    if (fraction < 0.0f || gContext == NULL)
        fraction = 0.0f;
    if (fraction > 0.25f)
        fraction = 0.25f;
    if (fraction > 0 && gGameBuffer == NULL) {
        gGameBuffer = rsxMemalign(64, gColorPitch * gScreenH);
        if (gGameBuffer == NULL)
            fraction = 0.0f;
        else
            rsxAddressToOffset(gGameBuffer, &gGameOffset);
    }
    if (fraction != gOverscan) {
        Target t;
        FlushBatch();
        printf("PS3GL: the picture is shown %.0f%% smaller (overscan)\n", fraction * 100.0f);
        gOverscan = fraction;
        CurrentTarget(&t);
        SetSurface(&t);
        gStateDirty = gTargetDirty = 1;
        gCurProg = -1;
        gCurTexture = (GLuint)-1;
    }
}

void PS3GLSwap(void)
{
    Target t;

    if (gContext == NULL)
        return;
    if (gFrame < 4 || gFrame % 3600 == 0)
        printf("PS3GL: frame %u, %u vertices, textures %u KB, render targets %u KB\n", gFrame, gRingCount,
               gTextureBytes / 1024, gTargetBytes / 1024);
    gPS3GLDoing = "waiting for the last frame to show";
    if (!gFirstFlip) {
        while (gcmGetFlipStatus() != 0)
            usleep(200);
    }
    gcmResetFlipStatus();
    gFirstFlip = 0;
    if (gOverscan > 0)
        ShowGameBuffer();
    Trace("flip");
    ROOM();
    gcmSetFlip(gContext, gCurBuffer);
    rsxFlushBuffer(gContext);
    gcmSetWaitFlip(gContext);
    WaitIdle();                         // the frame is done: the vertex ring can start over
    gRingCount = gBatchStart = 0;
    gFrame++;
    gCurBuffer ^= 1;
    CurrentTarget(&t);
    SetSurface(&t);
    gStateDirty = gTargetDirty = gTextureCacheDirty = 1;
    gCurProg = -1;
    gCurTexture = (GLuint)-1;
    gPS3GLFrames++;
    gPS3GLLastSwap = PS3GLNow();
    gPS3GLDoing = "the game (between frames)";
}
