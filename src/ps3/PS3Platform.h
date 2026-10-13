/*
 * PS3Platform.h — what SDL's video side does on a computer, done for the PS3.
 *
 * SDL2's PS3 port (SDL2_PSL1GHT) gives the game its timer, threads, sound, files and events,
 * but its video driver has no OpenGL, and it would set up the RSX itself. So on the PS3
 * SDL's video subsystem is never started: the game's window and OpenGL calls come here
 * (through the macros below, which the PS3 build alone sees), and PS3GL.c draws.
 * With video off, SDL's other window functions do nothing and return an error, which suits
 * a console: there is nothing to move, resize or raise.
 * Sound is the same story: SDL2_PSL1GHT's audio driver was written for an older SDL and
 * crashes when a device opens, so the game's mixer plays through PS3Platform.c's own output.
 */
#ifndef PS3PLATFORM_H
#define PS3PLATFORM_H

#include <SDL.h>
#include "PS3GL.h"
#include "PS3Files.h"

#ifdef __cplusplus
extern "C" {
#endif

/// First thing in main: the log on the USB drive, the game's folders, debug settings.
void PS3PlatformInit(int argc, char *argv[]);
/// The folder that holds "Game Data" (the game's USRDIR).
const char *PS3ResourcesFolder(void);
/// Where saved games, profiles and settings go.
const char *PS3SaveFolder(void);
/// Reads the controllers into SDL's virtual game controllers. Called (through the macros
/// below) whenever the game asks SDL for events, so all of SDL stays on the game's thread.
void PS3ReadControllers(void);

void PS3_PumpEvents(void);
int  PS3_PollEvent(SDL_Event *event);
int  PS3_WaitEvent(SDL_Event *event);
int  PS3_WaitEventTimeout(SDL_Event *event, int timeout);
int  PS3_Init(Uint32 flags);
int  PS3_InitSubSystem(Uint32 flags);
SDL_Window *PS3_CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags);
void PS3_DestroyWindow(SDL_Window *window);
int  PS3_GL_SetAttribute(SDL_GLattr attr, int value);
SDL_GLContext PS3_GL_CreateContext(SDL_Window *window);
void PS3_GL_DeleteContext(SDL_GLContext context);
int  PS3_GL_MakeCurrent(SDL_Window *window, SDL_GLContext context);
void PS3_GL_SwapWindow(SDL_Window *window);
void PS3_GL_GetDrawableSize(SDL_Window *window, int *w, int *h);
void *PS3_GL_GetProcAddress(const char *proc);
void PS3_GetWindowSize(SDL_Window *window, int *w, int *h);
void PS3_SetWindowSize(SDL_Window *window, int w, int h);
int  PS3_SetWindowFullscreen(SDL_Window *window, Uint32 flags);
int  PS3_GetWindowDisplayIndex(SDL_Window *window);
int  PS3_GetCurrentDisplayMode(int displayIndex, SDL_DisplayMode *mode);
int  PS3_GetDesktopDisplayMode(int displayIndex, SDL_DisplayMode *mode);
int  PS3_GetDisplayBounds(int displayIndex, SDL_Rect *rect);
char *PS3_GetPrefPath(const char *org, const char *app);
SDL_AudioDeviceID PS3_OpenAudioDevice(const char *device, int iscapture, const SDL_AudioSpec *desired,
                                      SDL_AudioSpec *obtained, int allowed_changes);
void PS3_CloseAudioDevice(SDL_AudioDeviceID dev);
void PS3_PauseAudioDevice(SDL_AudioDeviceID dev, int pause_on);
void PS3_LockAudioDevice(SDL_AudioDeviceID dev);
void PS3_UnlockAudioDevice(SDL_AudioDeviceID dev);

#ifndef PS3_PLATFORM_IMPLEMENTATION
#define SDL_Init                    PS3_Init
#define SDL_PumpEvents              PS3_PumpEvents
#define SDL_PollEvent               PS3_PollEvent
#define SDL_WaitEvent               PS3_WaitEvent
#define SDL_WaitEventTimeout        PS3_WaitEventTimeout
#define SDL_InitSubSystem           PS3_InitSubSystem
#define SDL_CreateWindow            PS3_CreateWindow
#define SDL_DestroyWindow           PS3_DestroyWindow
#define SDL_GL_SetAttribute         PS3_GL_SetAttribute
#define SDL_GL_CreateContext        PS3_GL_CreateContext
#define SDL_GL_DeleteContext        PS3_GL_DeleteContext
#define SDL_GL_MakeCurrent          PS3_GL_MakeCurrent
#define SDL_GL_SwapWindow           PS3_GL_SwapWindow
#define SDL_GL_GetDrawableSize      PS3_GL_GetDrawableSize
#define SDL_GL_GetProcAddress       PS3_GL_GetProcAddress
#define SDL_GetWindowSize           PS3_GetWindowSize
#define SDL_SetWindowSize           PS3_SetWindowSize
#define SDL_SetWindowFullscreen     PS3_SetWindowFullscreen
#define SDL_GetWindowDisplayIndex   PS3_GetWindowDisplayIndex
#define SDL_GetCurrentDisplayMode   PS3_GetCurrentDisplayMode
#define SDL_GetDesktopDisplayMode   PS3_GetDesktopDisplayMode
#define SDL_GetDisplayBounds        PS3_GetDisplayBounds
#define SDL_GetPrefPath             PS3_GetPrefPath
#define SDL_OpenAudioDevice         PS3_OpenAudioDevice
#define SDL_CloseAudioDevice        PS3_CloseAudioDevice
#define SDL_PauseAudioDevice        PS3_PauseAudioDevice
#define SDL_LockAudioDevice         PS3_LockAudioDevice
#define SDL_UnlockAudioDevice       PS3_UnlockAudioDevice
#endif

#ifdef __cplusplus
}
#endif

#endif
