/* OpenGL include shim. The engine uses the OpenGL 2.1 compatibility profile
 * (fixed-function pipeline), which macOS still provides on Apple Silicon. */
#ifndef __BKGL__
#define __BKGL__

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION 1
#include <OpenGL/gl.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#endif

#endif
