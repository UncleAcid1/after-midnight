/*
 * PS3Platform.c — the PS3 side of After Midnight's platform layer (see PS3Platform.h).
 *
 * - Folders: the game's own folder (USRDIR, where EBOOT.BIN is) holds "Game Data" and
 *   "Game Data Extras"; saves, profiles and settings go in its "Saves" folder.
 * - The log: when a USB drive is plugged in (any port, the first found), everything the game
 *   prints goes to "After Midnight log.txt" on it (the last run's is kept as "... previous.txt").
 *   Without one it goes to the system's TTY (RPCS3 shows it in TTY.log).
 * - Settings for testing: lines NAME=value in "After Midnight settings.txt" on the USB drive
 *   (or in the game's folder) become environment variables, the same testing aids the
 *   computer versions read (MM_DEBUG_AUTOSTART, MM_DEBUG_VIRTUAL_PADS...).
 * - Controllers: each PS3 pad becomes one of SDL's virtual game controllers, so the game sees
 *   it as it sees any controller elsewhere. Cross = A, Circle = B, Square = X, Triangle = Y.
 *   They are read whenever the game asks SDL for events, on the game's thread.
 * - A thread of our own answers the system (quit from the XMB) and watches for a stuck game:
 *   no new frame for 3 seconds puts what the game was doing in the log.
 * - Quit from the XMB (PS button) sends the game an SDL_QUIT, as closing the window would.
 * - Sound: one audio port (48 kHz stereo), filled block by block from the game's mixer (the
 *   SDL audio callback) on a thread of its own.
 */

#define PS3_PLATFORM_IMPLEMENTATION
#include "PS3Platform.h"

#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include <audio/audio.h>
#include <io/pad.h>
#include <sys/event_queue.h>
#include <sys/process.h>
#include <sys/thread.h>
#include <sys/tty.h>
#include <sysutil/sysutil.h>
#include <unistd.h>
#include <malloc.h>
#include <ppu-lv2.h>

SYS_PROCESS_PARAM(1001, 0x100000)   // main thread: normal priority, 1 MB of stack

#define kMaxPads        7
#define kLogName        "After Midnight log.txt"
#define kOldLogName     "After Midnight log previous.txt"
#define kSettingsName   "After Midnight settings.txt"

static char gUSBFolder[16];         // "/dev_usb000"... or "" when no drive is in
static char gResources[512];
static char gSaves[600];
static int gVideoUp = 0;
static char gWindowStandIn[64];     // the SDL_Window the game holds: never looked inside
static sys_ppu_thread_t gPollThread;
static volatile int gPolling = 0;
static SDL_Joystick *gPadJoystick[kMaxPads];
static int gPadsReady = 0;
static int gQuitSent = 0;

static int FolderExists(const char *path)
{
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

static FILE *gLog = NULL;       // the log, for this file's threads too (stdout is per thread)

/// printf for any thread: into the log on the USB drive when there is one.
static void Log(const char *format, ...) __attribute__((format(printf, 1, 2)));
static void Log(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    vfprintf(gLog != NULL ? gLog : stdout, format, args);
    va_end(args);
    fflush(gLog != NULL ? gLog : stdout);
}

static void FindUSBDrive(void)
{
    int i;

    for (i = 0; i < 8; i++) {
        snprintf(gUSBFolder, sizeof(gUSBFolder), "/dev_usb%03d", i);
        if (FolderExists(gUSBFolder))
            return;
    }
    gUSBFolder[0] = '\0';
}

static void StartLog(void)
{
    char path[256], old[256];

    if (gUSBFolder[0] == '\0')
        return;
    snprintf(path, sizeof(path), "%s/%s", gUSBFolder, kLogName);
    snprintf(old, sizeof(old), "%s/%s", gUSBFolder, kOldLogName);
    remove(old);
    rename(path, old);
    if (freopen(path, "w", stdout) == NULL)
        return;
    setvbuf(stdout, NULL, _IOLBF, 0);
    stderr = stdout;            // one file, in the order things happened
    gLog = stdout;
}

static void TrimLine(char *s)
{
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r' || s[n - 1] == ' ' || s[n - 1] == '\t'))
        s[--n] = '\0';
}

static void ReadSettings(const char *path)
{
    FILE *f = fopen(path, "r");
    char line[512];

    if (f == NULL)
        return;
    printf("Settings from %s:\n", path);
    while (fgets(line, sizeof(line), f) != NULL) {
        char *eq, *name = line;
        TrimLine(line);
        while (*name == ' ' || *name == '\t')
            name++;
        if (*name == '\0' || *name == '#' || (eq = strchr(name, '=')) == NULL)
            continue;
        *eq = '\0';
        TrimLine(name);
        setenv(name, eq + 1, 1);
        printf("  %s=%s\n", name, eq + 1);
    }
    fclose(f);
}

/// The PS3's free main memory (of 256 MB) and the game's heap, in the log.
void PS3LogMemory(const char *when)
{
    struct { u32 total, available; } info = { 0, 0 };
    struct mallinfo heap = mallinfo();

    {
        lv2syscall1(352, (u64)&info);       // sys_memory_get_user_memory_size
    }
    printf("Memory (%s): %u KB free of %u KB; heap %u KB in use\n", when, info.available / 1024,
           info.total / 1024, (unsigned)heap.uordblks / 1024);
}

void PS3PlatformInit(int argc, char *argv[])
{
    char settings[600];
    char *slash;
    time_t now = time(NULL);

    FindUSBDrive();
    StartLog();
    printf("After Midnight for PS3, built %s %s\n", __DATE__, __TIME__);
    if (gUSBFolder[0] != '\0')
        printf("USB drive: %s\n", gUSBFolder);
    printf("Started %s", ctime(&now));
        // argv[0] is the EBOOT.BIN in the game's USRDIR
    snprintf(gResources, sizeof(gResources), "%s", argc > 0 && argv[0] != NULL ? argv[0] : "");
    slash = strrchr(gResources, '/');
    if (slash != NULL)
        *slash = '\0';
    else
        snprintf(gResources, sizeof(gResources), "/dev_hdd0/game/AMHD00001/USRDIR");
    printf("Game folder: %s\n", gResources);
    snprintf(gSaves, sizeof(gSaves), "%s/Saves", gResources);
    mkdir(gSaves, 0777);
    setenv("MM_RESOURCES_DIR", gResources, 1);
    setenv("HOME", gSaves, 1);
    snprintf(settings, sizeof(settings), "%s/%s", gResources, kSettingsName);
    ReadSettings(settings);
    if (gUSBFolder[0] != '\0') {
        snprintf(settings, sizeof(settings), "%s/%s", gUSBFolder, kSettingsName);
        ReadSettings(settings);
    }
    fflush(stdout);
}

const char *PS3ResourcesFolder(void)
{
    return gResources;
}

const char *PS3SaveFolder(void)
{
    return gSaves;
}

// ---------------------------------------------------------------------------
// System messages and controllers

static void SystemCallback(u64 status, u64 param, void *userdata)
{
    (void)param;
    (void)userdata;
    if (status == SYSUTIL_EXIT_GAME && !gQuitSent) {
        SDL_Event e;
        Log("Quit from the XMB\n");
        memset(&e, 0, sizeof(e));
        e.type = SDL_QUIT;
        SDL_PushEvent(&e);
        gQuitSent = 1;
    }
}

static Sint16 StickAxis(unsigned int v)
{
    int a = ((int)v - 128) * 256;
    if (a > 32767) a = 32767;
    if (a < -32768) a = -32768;
    return (Sint16)a;
}

static void ClosePad(int i)
{
    SDL_JoystickID id = SDL_JoystickInstanceID(gPadJoystick[i]);
    int n;

    SDL_JoystickClose(gPadJoystick[i]);
    gPadJoystick[i] = NULL;
    for (n = 0; n < SDL_NumJoysticks(); n++) {
        if (SDL_JoystickGetDeviceInstanceID(n) == id) {
            SDL_JoystickDetachVirtual(n);
            break;
        }
    }
    Log("Controller %d unplugged\n", i + 1);
}

static void OpenPad(int i)
{
    int device = SDL_JoystickAttachVirtual(SDL_JOYSTICK_TYPE_GAMECONTROLLER, SDL_CONTROLLER_AXIS_MAX,
                                           SDL_CONTROLLER_BUTTON_MAX, 0);
    if (device < 0) {
        Log("Controller %d: no virtual controller (%s)\n", i + 1, SDL_GetError());
        return;
    }
    gPadJoystick[i] = SDL_JoystickOpen(device);
    Log("Controller %d plugged in\n", i + 1);
}

static void ReadPad(int i)
{
    SDL_Joystick *j = gPadJoystick[i];
    padData d;

    if (ioPadGetData(i, &d) != 0 || d.len == 0)
        return;                 // nothing new since last time
    if (getenv("MM_PS3_DEBUG_PADS") != NULL)        // testing aid: every change on the pads
        printf("Controller %d: buttons %04x%04x sticks %u,%u %u,%u\n", i + 1,
               (unsigned)d.button[2], (unsigned)d.button[3], d.ANA_L_H, d.ANA_L_V, d.ANA_R_H, d.ANA_R_V);
    SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_A, d.BTN_CROSS);
    SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_B, d.BTN_CIRCLE);
    SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_X, d.BTN_SQUARE);
    SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_Y, d.BTN_TRIANGLE);
    SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_BACK, d.BTN_SELECT);
    SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_START, d.BTN_START);
    SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_LEFTSTICK, d.BTN_L3);
    SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_RIGHTSTICK, d.BTN_R3);
    SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_LEFTSHOULDER, d.BTN_L1);
    SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, d.BTN_R1);
    SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_DPAD_UP, d.BTN_UP);
    SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_DPAD_DOWN, d.BTN_DOWN);
    SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_DPAD_LEFT, d.BTN_LEFT);
    SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_DPAD_RIGHT, d.BTN_RIGHT);
    SDL_JoystickSetVirtualAxis(j, SDL_CONTROLLER_AXIS_LEFTX, StickAxis(d.ANA_L_H));
    SDL_JoystickSetVirtualAxis(j, SDL_CONTROLLER_AXIS_LEFTY, StickAxis(d.ANA_L_V));
    SDL_JoystickSetVirtualAxis(j, SDL_CONTROLLER_AXIS_RIGHTX, StickAxis(d.ANA_R_H));
    SDL_JoystickSetVirtualAxis(j, SDL_CONTROLLER_AXIS_RIGHTY, StickAxis(d.ANA_R_V));
    SDL_JoystickSetVirtualAxis(j, SDL_CONTROLLER_AXIS_TRIGGERLEFT, d.BTN_L2 ? 32767 : 0);
    SDL_JoystickSetVirtualAxis(j, SDL_CONTROLLER_AXIS_TRIGGERRIGHT, d.BTN_R2 ? 32767 : 0);
}

/// Says in the log when no frame has been shown for a while, and what was going on.
static void Watchdog(void)
{
    static unsigned reportedFrames = 0;
    unsigned now = PS3GLNow(), last = gPS3GLLastSwap;

    if (last == 0 || now - last < 3000 || reportedFrames == gPS3GLFrames)
        return;
    reportedFrames = gPS3GLFrames;
    {       // to the system's TTY first: the log may be what the game is stuck on
        char line[200];
        u32 written;
        int n = snprintf(line, sizeof(line), "No new frame for %u ms (after frame %u): %s\n", now - last,
                         gPS3GLFrames, gPS3GLDoing);
        sysTtyWrite(0, line, (u32)n, &written);
    }
    Log("No new frame for %u ms (after frame %u): %s\n", now - last, gPS3GLFrames, gPS3GLDoing);
    PS3GLPrintTrace(gLog != NULL ? gLog : stdout);
    fflush(gLog != NULL ? gLog : stdout);
}

static Uint32 gLastAskedForInput;

/// The return addresses on this thread's stack (the PPC64 back chain), for the log; the
/// addresses go into "ppu-addr2line -e EBOOT.elf" to name the functions.
static void LogBacktrace(const char *why)
{
    char line[400];
    u64 *frame;
    u32 written;
    int i, n;

    __asm__ volatile("mr %0,1" : "=r"(frame));
    n = snprintf(line, sizeof(line), "%s; called from:", why);
    for (i = 0; i < 16 && frame != NULL && n < (int)sizeof(line) - 20; i++) {
        u64 *caller = (u64 *)(uintptr_t)frame[0];
        if (caller == NULL || caller <= frame)
            break;
        n += snprintf(line + n, sizeof(line) - n, " %llx", (unsigned long long)caller[2]);  // its saved LR
        frame = caller;
    }
    n += snprintf(line + n, sizeof(line) - n, "\n");
    sysTtyWrite(0, line, (u32)n, &written);
    Log("%s", line);
}

void PS3ReadControllers(void)
{
    padInfo info;
    int i;

    gLastAskedForInput = SDL_GetTicks();
    if (!gPadsReady || ioPadGetInfo(&info) != 0)
        return;
    for (i = 0; i < kMaxPads; i++) {
        int connected = info.status[i] != 0;
        if (connected && gPadJoystick[i] == NULL)
            OpenPad(i);
        else if (!connected && gPadJoystick[i] != NULL)
            ClosePad(i);
        if (gPadJoystick[i] != NULL)
            ReadPad(i);
    }
}

/*  System messages and the watchdog, on a thread of our own (SDL2_PSL1GHT's threads, and so
    its timers, get a fixed 16 KB of stack). It calls nothing in SDL but SDL_PushEvent. */
static void PollThread(void *arg)
{
    (void)arg;
    while (gPolling) {
        sysUtilCheckCallback();
        Watchdog();
        usleep(16 * 1000);
    }
    sysThreadExit(0);
}

static void StopPolling(void)
{
    u64 result;

    if (!gPolling)
        return;
    gPolling = 0;
    sysThreadJoin(gPollThread, &result);
}

// ---------------------------------------------------------------------------
// SDL, without its video subsystem

static Uint32 WithoutVideo(Uint32 flags)
{
    if (flags & SDL_INIT_VIDEO)
        flags = (flags & ~SDL_INIT_VIDEO) | SDL_INIT_EVENTS;
    return flags & ~SDL_INIT_AUDIO;     // the sound output below needs none of SDL's
}

static void StartPolling(void)
{
    if (gPolling || !SDL_WasInit(SDL_INIT_EVENTS))
        return;
    sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0, SystemCallback, NULL);
    gPolling = 1;
    if (sysThreadCreate(&gPollThread, PollThread, NULL, 1500, 64 * 1024, THREAD_JOINABLE, "AM system") != 0) {
        gPolling = 0;
        printf("The system thread didn't start\n");
        return;
    }
    atexit(StopPolling);
}

static void StartPads(void)
{
    padInfo info;

    if (gPadsReady || !SDL_WasInit(SDL_INIT_JOYSTICK))
        return;
        // SDL's own PS3 joystick driver has already set the pads up; its devices have no
        // controller mapping, so the game leaves them alone and only reads the virtual ones
    if (ioPadGetInfo(&info) != 0)
        ioPadInit(kMaxPads);
    gPadsReady = 1;
}

void PS3_PumpEvents(void)
{
    PS3ReadControllers();
    SDL_PumpEvents();
}

int PS3_PollEvent(SDL_Event *event)
{
    PS3ReadControllers();
    return SDL_PollEvent(event);
}

int PS3_WaitEvent(SDL_Event *event)
{
    return PS3_WaitEventTimeout(event, -1);
}

int PS3_WaitEventTimeout(SDL_Event *event, int timeout)
{
    Uint32 start = SDL_GetTicks();

        // in steps, so the controllers keep being read meanwhile
    for (;;) {
        int left = timeout < 0 ? 10 : timeout - (int)(SDL_GetTicks() - start);
        PS3ReadControllers();
        if (SDL_WaitEventTimeout(event, left < 10 ? (left < 0 ? 0 : left) : 10))
            return 1;
        if (timeout >= 0 && (int)(SDL_GetTicks() - start) >= timeout)
            return 0;
    }
}

int PS3_Init(Uint32 flags)
{
    int result = SDL_Init(WithoutVideo(flags));
    StartPolling();
    StartPads();
    return result;
}

int PS3_InitSubSystem(Uint32 flags)
{
    int result = SDL_InitSubSystem(WithoutVideo(flags));
    StartPolling();
    StartPads();
    return result;
}

SDL_Window *PS3_CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags)
{
    (void)title; (void)x; (void)y; (void)w; (void)h; (void)flags;
    if (!gVideoUp) {
        if (PS3GLInit() != 0) {
            SDL_SetError("The PS3's graphics didn't start");
            return NULL;
        }
        atexit(PS3GLShutdown);
        gVideoUp = 1;
    }
    return (SDL_Window *)gWindowStandIn;
}

void PS3_DestroyWindow(SDL_Window *window)
{
    (void)window;
}

int PS3_GL_SetAttribute(SDL_GLattr attr, int value)
{
    (void)attr; (void)value;
    return 0;
}

SDL_GLContext PS3_GL_CreateContext(SDL_Window *window)
{
    return window != NULL ? (SDL_GLContext)gWindowStandIn : NULL;
}

void PS3_GL_DeleteContext(SDL_GLContext context)
{
    (void)context;
}

int PS3_GL_MakeCurrent(SDL_Window *window, SDL_GLContext context)
{
    (void)window; (void)context;
    return 0;
}

int PS3ExtrasTVSafe(void);       // MMExtras.c

void PS3_GL_SwapWindow(SDL_Window *window)
{
    static Uint32 frames = 0, reported = 0;
    Uint32 now = SDL_GetTicks();

    (void)window;
    if (gPadsReady && now - gLastAskedForInput > 3000 && now - reported > 10000) {
        reported = now;
        LogBacktrace("The game hasn't read its input for 3 seconds");
    }
    if (++frames < 4 || getenv("MM_PS3_DEBUG_FRAMES") != NULL)     // testing aid: every frame's time
        printf("Frame %u at %u ms\n", (unsigned)frames, (unsigned)now);
    if (frames % 1800 == 0)
        PS3LogMemory("every 1800 frames");
    {       // the frame rate, every 10 seconds, for reports from a real PS3
        static Uint32 since = 0, sinceFrames = 0;
        if (since == 0) {
            since = now;
            sinceFrames = frames;
        } else if (now - since >= 10000) {
            printf("Frame rate: %.1f per second\n", (frames - sinceFrames) * 1000.0 / (now - since));
            since = now;
            sinceFrames = frames;
        }
    }
    PS3GLSwap();
        // "Fit the picture on a TV" (Extras), from the next frame: 7% smaller, or
        // MM_PS3_OVERSCAN percent in the settings file
    {
        const char *percent = getenv("MM_PS3_OVERSCAN");
        float amount = percent != NULL ? (float)atof(percent) / 100.0f : 0.07f;
        PS3GLSetOverscan(PS3ExtrasTVSafe() ? amount : 0.0f);
    }
}

void PS3_GL_GetDrawableSize(SDL_Window *window, int *w, int *h)
{
    (void)window;
    PS3GLGetScreenSize(w, h);
}

void *PS3_GL_GetProcAddress(const char *proc)
{
    return PS3GLGetProcAddress(proc);
}

void PS3_GetWindowSize(SDL_Window *window, int *w, int *h)
{
    int sw, sh;

    (void)window;
    PS3GLGetScreenSize(&sw, &sh);
    if (w != NULL) *w = sw;
    if (h != NULL) *h = sh;
}

void PS3_SetWindowSize(SDL_Window *window, int w, int h)
{
    (void)window; (void)w; (void)h;
}

int PS3_SetWindowFullscreen(SDL_Window *window, Uint32 flags)
{
    (void)window; (void)flags;
    return 0;
}

int PS3_GetWindowDisplayIndex(SDL_Window *window)
{
    (void)window;
    return 0;
}

int PS3_GetCurrentDisplayMode(int displayIndex, SDL_DisplayMode *mode)
{
    (void)displayIndex;
    memset(mode, 0, sizeof(*mode));
    mode->format = SDL_PIXELFORMAT_RGB888;
    mode->refresh_rate = 60;
    if (gVideoUp)
        PS3GLGetScreenSize(&mode->w, &mode->h);
    else {
        mode->w = 1280;
        mode->h = 720;
    }
    return 0;
}

int PS3_GetDesktopDisplayMode(int displayIndex, SDL_DisplayMode *mode)
{
    return PS3_GetCurrentDisplayMode(displayIndex, mode);
}

int PS3_GetDisplayBounds(int displayIndex, SDL_Rect *rect)
{
    SDL_DisplayMode mode;

    PS3_GetCurrentDisplayMode(displayIndex, &mode);
    rect->x = rect->y = 0;
    rect->w = mode.w;
    rect->h = mode.h;
    return 0;
}

char *PS3_GetPrefPath(const char *org, const char *app)
{
    size_t n = strlen(gSaves) + 2;
    char *path = SDL_malloc(n);

    (void)org; (void)app;
    if (path != NULL)
        snprintf(path, n, "%s/", gSaves);
    return path;
}

// ---------------------------------------------------------------------------
// Sound

#define kAudioRate      48000           // the PS3's audio ports always run at 48 kHz

static struct {
    SDL_AudioSpec       spec;
    SDL_mutex          *lock;
    sys_ppu_thread_t    thread;
    volatile int        running, paused;
    u32                 port;
    audioPortConfig     config;
    sys_event_queue_t   queue;
    sys_ipc_key_t       key;
    Sint16              mix[AUDIO_BLOCK_SAMPLES * 2];
} gAudio;

/// Its own lv2 thread (SDL's would get 16 KB of stack: the music decoder needs more).
static void AudioThread(void *unused)
{
    u32 next = 1;

    (void)unused;
    while (gAudio.running) {
        sys_event_t event;
        u32 playing = (u32)*(volatile u64 *)(u64)gAudio.config.readIndex, i;
        float *block;

        sysEventQueueReceive(gAudio.queue, &event, 20 * 1000);
            // fill every block between the one we filled last and the one playing now
        while (next != playing && gAudio.running) {
            SDL_LockMutex(gAudio.lock);
            if (gAudio.paused)
                memset(gAudio.mix, 0, sizeof(gAudio.mix));
            else
                gAudio.spec.callback(gAudio.spec.userdata, (Uint8 *)gAudio.mix, (int)sizeof(gAudio.mix));
            SDL_UnlockMutex(gAudio.lock);
            block = (float *)(u64)gAudio.config.audioDataStart + 2 * AUDIO_BLOCK_SAMPLES * next;
            for (i = 0; i < AUDIO_BLOCK_SAMPLES * 2; i++)
                block[i] = gAudio.mix[i] * (1.0f / 32768.0f);
            next = (next + 1) % gAudio.config.numBlocks;
        }
    }
    sysThreadExit(0);
}

static void CloseSoundAtExit(void)
{
    PS3_CloseAudioDevice(1);    // the thread mustn't mix freed sounds while the game exits
}

SDL_AudioDeviceID PS3_OpenAudioDevice(const char *device, int iscapture, const SDL_AudioSpec *desired,
                                      SDL_AudioSpec *obtained, int allowed_changes)
{
    static int atExit = 0;
    audioPortParam params;

    (void)device; (void)allowed_changes;
    if (iscapture || gAudio.running || desired->callback == NULL) {
        SDL_SetError("PS3: one sound output, played through a callback");
        return 0;
    }
    if (audioInit() != 0) {
        SDL_SetError("PS3: audioInit failed");
        return 0;
    }
    memset(&params, 0, sizeof(params));
    params.numChannels = AUDIO_PORT_2CH;
    params.numBlocks = AUDIO_BLOCK_8;
    params.attrib = 0;
    params.level = 1.0f;
    if (audioPortOpen(&params, &gAudio.port) != 0 || audioGetPortConfig(gAudio.port, &gAudio.config) != 0 ||
        audioCreateNotifyEventQueue(&gAudio.queue, &gAudio.key) != 0 ||
        audioSetNotifyEventQueue(gAudio.key) != 0) {
        SDL_SetError("PS3: the audio port didn't open");
        return 0;
    }
    sysEventQueueDrain(gAudio.queue);
    gAudio.spec = *desired;
    gAudio.spec.freq = kAudioRate;
    gAudio.spec.format = AUDIO_S16SYS;
    gAudio.spec.channels = 2;
    gAudio.spec.samples = AUDIO_BLOCK_SAMPLES;
    gAudio.spec.size = (Uint32)sizeof(gAudio.mix);
    if (obtained != NULL)
        *obtained = gAudio.spec;
    gAudio.lock = SDL_CreateMutex();
    gAudio.paused = 1;
    gAudio.running = 1;
    audioPortStart(gAudio.port);
    if (sysThreadCreate(&gAudio.thread, AudioThread, NULL, 500, 512 * 1024, THREAD_JOINABLE, "AM sound") != 0) {
        gAudio.running = 0;
        SDL_SetError("PS3: the sound thread didn't start");
        return 0;
    }
    printf("Sound: 48 kHz stereo, %u blocks of %d samples\n", (unsigned)gAudio.config.numBlocks, AUDIO_BLOCK_SAMPLES);
    if (!atExit) {
        atexit(CloseSoundAtExit);
        atExit = 1;
    }
    return 1;
}

void PS3_CloseAudioDevice(SDL_AudioDeviceID dev)
{
    u64 result;

    if (dev != 1 || !gAudio.running)
        return;
    gAudio.running = 0;
    sysThreadJoin(gAudio.thread, &result);
    audioPortStop(gAudio.port);
    audioRemoveNotifyEventQueue(gAudio.key);
    audioPortClose(gAudio.port);
    sysEventQueueDestroy(gAudio.queue, 0);
    audioQuit();
    SDL_DestroyMutex(gAudio.lock);
    gAudio.lock = NULL;
}

void PS3_PauseAudioDevice(SDL_AudioDeviceID dev, int pause_on)
{
    if (dev == 1)
        gAudio.paused = pause_on;
}

void PS3_LockAudioDevice(SDL_AudioDeviceID dev)
{
    volatile const char *was = gPS3GLDoing;

    if (dev == 1 && gAudio.lock != NULL) {
        gPS3GLDoing = "waiting for the sound mixer";
        SDL_LockMutex(gAudio.lock);
        gPS3GLDoing = was;
    }
}

void PS3_UnlockAudioDevice(SDL_AudioDeviceID dev)
{
    if (dev == 1 && gAudio.lock != NULL)
        SDL_UnlockMutex(gAudio.lock);
}
