/*
 * PS3GL.h — the part of OpenGL that After Midnight draws with, for the PS3.
 *
 * The PS3 has no OpenGL. On Mac and Linux the game calls the real OpenGL 2.1 (fixed
 * function); on the PS3 BKGL.h includes this header instead, and PS3GL.c draws the same
 * calls with the RSX through PSL1GHT's librsx. Only what the game uses is here: immediate
 * mode (glBegin/glEnd), the two matrix stacks, 2D textures, blending, the scissor box,
 * glCopyTexSubImage2D and framebuffer objects (split screen). Anything else is left out on
 * purpose, so a new OpenGL call in the game fails to build for the PS3 instead of drawing
 * wrong.
 */
#ifndef PS3GL_H
#define PS3GL_H

#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int    GLenum;
typedef unsigned int    GLuint;
typedef int             GLint;
typedef int             GLsizei;
typedef unsigned int    GLbitfield;
typedef unsigned char   GLboolean;
typedef unsigned char   GLubyte;
typedef unsigned short  GLushort;
typedef float           GLfloat;
typedef float           GLclampf;
typedef double          GLdouble;
typedef void            GLvoid;

#define GL_FALSE                        0
#define GL_TRUE                         1
#define GL_NO_ERROR                     0
#define GL_INVALID_ENUM                 0x0500
#define GL_INVALID_VALUE                0x0501
#define GL_INVALID_OPERATION            0x0502
#define GL_STACK_OVERFLOW               0x0503
#define GL_STACK_UNDERFLOW              0x0504
#define GL_OUT_OF_MEMORY                0x0505

#define GL_TRIANGLES                    0x0004
#define GL_TRIANGLE_STRIP               0x0005
#define GL_QUADS                        0x0007

#define GL_DEPTH_TEST                   0x0B71
#define GL_BLEND                        0x0BE2
#define GL_SCISSOR_TEST                 0x0C11
#define GL_TEXTURE_2D                   0x0DE1
#define GL_UNPACK_ROW_LENGTH            0x0CF2
#define GL_UNPACK_ALIGNMENT             0x0CF5
#define GL_TEXTURE_BINDING_2D           0x8069

#define GL_MODELVIEW                    0x1700
#define GL_PROJECTION                   0x1701

#define GL_ZERO                         0
#define GL_ONE                          1
#define GL_SRC_COLOR                    0x0300
#define GL_ONE_MINUS_SRC_COLOR          0x0301
#define GL_SRC_ALPHA                    0x0302
#define GL_ONE_MINUS_SRC_ALPHA          0x0303
#define GL_DST_ALPHA                    0x0304
#define GL_ONE_MINUS_DST_ALPHA          0x0305
#define GL_DST_COLOR                    0x0306
#define GL_ONE_MINUS_DST_COLOR          0x0307

#define GL_COLOR_BUFFER_BIT             0x00004000

#define GL_UNSIGNED_BYTE                0x1401
#define GL_ALPHA                        0x1906
#define GL_RGBA                         0x1908
#define GL_ALPHA8                       0x803C
#define GL_RGBA8                        0x8058

#define GL_TEXTURE_ENV                  0x2300
#define GL_TEXTURE_ENV_MODE             0x2200
#define GL_MODULATE                     0x2100
#define GL_TEXTURE_MAG_FILTER           0x2800
#define GL_TEXTURE_MIN_FILTER           0x2801
#define GL_TEXTURE_WRAP_S               0x2802
#define GL_TEXTURE_WRAP_T               0x2803
#define GL_NEAREST                      0x2600
#define GL_LINEAR                       0x2601
#define GL_REPEAT                       0x2901
#define GL_CLAMP_TO_EDGE                0x812F

#define GL_FRAMEBUFFER                  0x8D40
#define GL_COLOR_ATTACHMENT0            0x8CE0
#define GL_FRAMEBUFFER_COMPLETE         0x8CD5
#define GL_FRAMEBUFFER_UNSUPPORTED      0x8CDD

#ifndef APIENTRY
#define APIENTRY
#endif

GLenum glGetError(void);
void glGetIntegerv(GLenum pname, GLint *params);

void glEnable(GLenum cap);
void glDisable(GLenum cap);
void glBlendFunc(GLenum sfactor, GLenum dfactor);
void glTexEnvi(GLenum target, GLenum pname, GLint param);

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void glClear(GLbitfield mask);

void glMatrixMode(GLenum mode);
void glLoadIdentity(void);
void glPushMatrix(void);
void glPopMatrix(void);
void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void glTranslatef(GLfloat x, GLfloat y, GLfloat z);
void glScalef(GLfloat x, GLfloat y, GLfloat z);
void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

void glBegin(GLenum mode);
void glEnd(void);
void glVertex2f(GLfloat x, GLfloat y);
void glVertex2i(GLint x, GLint y);
void glTexCoord2f(GLfloat s, GLfloat t);
void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void glColor4us(GLushort red, GLushort green, GLushort blue, GLushort alpha);
void glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
void glColor3ub(GLubyte red, GLubyte green, GLubyte blue);
void glRecti(GLint x1, GLint y1, GLint x2, GLint y2);

void glGenTextures(GLsizei n, GLuint *textures);
void glDeleteTextures(GLsizei n, const GLuint *textures);
void glBindTexture(GLenum target, GLuint texture);
void glTexParameteri(GLenum target, GLenum pname, GLint param);
void glPixelStorei(GLenum pname, GLint param);
void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height,
                  GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width,
                     GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y,
                         GLsizei width, GLsizei height);

void glGenFramebuffers(GLsizei n, GLuint *framebuffers);
void glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
void glBindFramebuffer(GLenum target, GLuint framebuffer);
void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLenum glCheckFramebufferStatus(GLenum target);

/// What SDL_GL_GetProcAddress gives on the other platforms: the framebuffer functions above.
void *PS3GLGetProcAddress(const char *name);

// ---------------------------------------------------------------------------
// The screen (PS3Platform.c and PS3GL.c): what SDL's window and OpenGL context do elsewhere.

/// Sets up the RSX and the TV's video mode. Returns 0 on success.
int  PS3GLInit(void);
void PS3GLShutdown(void);
/// The TV picture in pixels (1280x720, 1920x1080...).
void PS3GLGetScreenSize(int *w, int *h);
/// Shows what has been drawn and starts the next frame.
void PS3GLSwap(void);
/// Shows the picture this much smaller (0.05 is 5%), centered on black, for a TV that crops
/// its edges (overscan); 0 fills the screen.
void PS3GLSetOverscan(float fraction);
/// A screen pixel's width / height on this TV (1 in HD modes, not in 480 and 576 modes).
float PS3GLPixelAspect(void);
/// For the log when the game seems stuck: what the graphics code is doing, and when the last
/// frame was shown (SDL_GetTicks-style milliseconds, from PS3GLNow).
extern volatile const char *gPS3GLDoing;
extern volatile unsigned gPS3GLLastSwap, gPS3GLFrames;
unsigned PS3GLNow(void);
/// Writes the last few graphics operations to f.
void PS3GLPrintTrace(FILE *f);

#ifdef __cplusplus
}
#endif

#endif
