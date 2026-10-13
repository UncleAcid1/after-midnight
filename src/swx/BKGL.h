/* OpenGL include shim. The engine uses the OpenGL 2.1 compatibility profile
 * (fixed-function pipeline), which macOS still provides on Apple Silicon.
 * The PS3 has no OpenGL: there the same calls go to src/ps3/PS3GL.c, which draws them
 * with the RSX, and PS3Platform.h stands in for SDL's window and OpenGL context. */
#ifndef __BKGL__
#define __BKGL__

#if defined(MM_PS3)
#include "PS3Platform.h"
#elif defined(__APPLE__)
#define GL_SILENCE_DEPRECATION 1
#include <OpenGL/gl.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#endif

#endif
