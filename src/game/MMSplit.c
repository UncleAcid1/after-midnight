/*
 * MMSplit.c — split screen for two to four players on one computer, added by this version.
 *
 * The game keeps everything in globals and has one room loaded at a time, so each player
 * gets a whole copy of the game: the program's global variables (its data and bss
 * sections) are swapped in and out around each player's turn of the frame, and each player
 * has their own SpriteWorld, sprite layers, room and status bar. What all players share —
 * the window, sounds and music, controllers, settings, and in co-op the mansion's progress —
 * is registered as shared and left alone by the swap.
 *
 * Each player's game runs as a coroutine on its own stack. Whenever it would show a frame
 * (SWPresentWindow) it yields, so the game's own loops — fades, room slides, the map,
 * signs, dying — just work: the scheduler runs every player once per frame, each drawing
 * into their own offscreen framebuffer, then puts the four-by-three viewports side by side
 * (or two by two) between carved mahogany columns and shows the window once.
 *
 * Modes:
 *   Arcade Twin (co-op): one mansion; doors, keys, treasure and score are shared, and what
 *   one player does in a room is done in the others' copies of it (MMCoopSync.c). Lives are
 *   each player's own.
 *   Parallel Phantoms (race): everyone plays their own copy of the mansion; the others show
 *   as see-through phantoms. Fastest to the exit wins; each lost life adds 15 seconds.
 */

#include "MMGame.h"
#include "MMProtos.h"
#include "BKGL.h"
#include "BlitKernel.h"
#include "MMFont.h"
#include "MMCoroutine.h"
#include <sys/stat.h>
#if defined(__APPLE__)
#include <mach-o/getsect.h>
#include <mach-o/ldsyms.h>
#endif

#define kMaxSegments        6
#define kMaxShared          160
#define kMaxRanges          (kMaxShared + kMaxSegments + 4)
#define kEventQueueSize     256
#define kCoroutineStack     (16 * 1024 * 1024)
#define kFrameMs            (1000.0 / 60.0)
#define kDeathPenaltyMs     15000

enum { kPlayerStarting, kPlayerPlaying, kPlayerFinished, kPlayerOut };
enum { kEndNone, kEndTeamWon, kEndTeamLost, kEndRaceOver, kEndQuit };

typedef struct MemRange {
    unsigned char  *addr;
    size_t          size;
    size_t          offset;         ///< where it goes in a player's image
} MemRange;

typedef struct SplitPlayer {
    int             index;
    int             input;          ///< kSplitInput...
    Boolean         remote;         ///< playing on another computer (network race)
    MMCoroutine    *co;
    unsigned char  *image;          ///< this player's globals while another player runs
    SpriteWorldPtr  world, statsWorld;
    GLuint          fbo, texture;
    int             texW, texH;
    SWRect          slot;           ///< where it shows in the window (drawable pixels)
    SWRect          view;           ///< the part of the slot the game fills (the 4:3 picture)
    SDL_Event       queue[kEventQueueSize];
    int             queueHead, queueTail;
    int             status;
    int             debugAction;    ///< testing aid: 1 finish, 2 die
    Boolean         presented;      ///< drew a frame since the last time events were pumped
    Boolean         pumped;
    Uint32          finishMs;       ///< race time, penalties included
    int             deaths;
    int             place;          ///< 1st, 2nd... (race)
    Uint32          remoteElapsedMs;///< a remote player's race time so far
    Boolean         left;           ///< a remote player who left the game
    ObjectsCarriedRec lastObjects;  ///< co-op: redo the status bar when the shared items change

        // published every frame for the other players (phantoms, map)
    short           room;
    float           heroX, heroY;
    short           heroFrame;
    Boolean         heroFlipped;
    float           heroScaleW, heroScaleH;
    Boolean         heroVisible;
} SplitPlayer;

typedef struct Split {
    int             mode;
    int             numPlayers;
    int             numLocal;       ///< players on this computer (the others are remote)
    int             firstLocal;     ///< whose game runs in the program's own globals
    Boolean         network;        ///< a race against another computer
    int             input[kMaxPlayers];
    Boolean         pending;        ///< configured; the next StartGame starts it
    Boolean         active;
    Boolean         paused;
    int             pauseChoice;
    int             end;            ///< kEnd...
    int             running;        ///< player whose coroutine runs, -1 none
    short           levelNumber;
    SplitPlayer     players[kMaxPlayers];

    MemRange        segments[kMaxSegments];
    int             numSegments;
    size_t          imageSize;
    MemRange        shared[kMaxShared];
    int             numShared;
    MemRange        ranges[kMaxRanges]; ///< the segments without the shared parts
    int             numRanges;
    unsigned char  *liveImage;      ///< player 1's globals while another player runs

    Uint32          startTicks, pausedAt;
    double          nextFrameAt;
    int             windowW, windowH;
    GLuint          brickTexture;
    int             brickW, brickH;
    MMFont         *font, *titleFont;
    Uint32          endShownAt;
    char            notice[400];    ///< said once back at the title screen
    char            continuePath[1500]; ///< co-op: the team's saved game being continued
    Uint32          savedNoticeUntil[kMaxPlayers];  ///< "Team game saved" in that player's view
} Split;

static Split *gSplit = NULL;

static void SplitPresent(SpriteWorldPtr spriteWorldP);
static void DrawOtherPlayersBeforeHero(SpriteWorldPtr w, SpriteLayerPtr layerP);

// ---------------------------------------------------------------------------
// Configuration

static Split *GetSplit(void)
{
    if (gSplit == NULL) {
        gSplit = calloc(1, sizeof(Split));
        if (gSplit != NULL)
            gSplit->running = -1;
    }
    return gSplit;
}

void SplitConfigure(int mode, int numPlayers, const int *inputs)
{
    Split *s = GetSplit();
    int i;

    if (s == NULL || s->active)
        return;
    s->mode = mode;
    s->numPlayers = numPlayers < 1 ? 1 : numPlayers > kMaxPlayers ? kMaxPlayers : numPlayers;
    for (i = 0; i < kMaxPlayers; i++)
        s->input[i] = inputs != NULL ? inputs[i] : i;
    s->pending = mode != kSplitModeNone && s->numPlayers > 1;
    s->continuePath[0] = 0;
    s->network = false;
    s->numLocal = s->numPlayers;
    s->firstLocal = 0;
}

/// A race on several computers: one player here (localPlayer), the others elsewhere.
void SplitConfigureNetworkRace(int localPlayer, int numPlayers)
{
    Split *s = GetSplit();
    int i;

    if (s == NULL || s->active)
        return;
    s->mode = kSplitModeRace;
    s->numPlayers = numPlayers < 2 ? 2 : numPlayers > kMaxPlayers ? kMaxPlayers : numPlayers;
    s->numLocal = 1;
    s->firstLocal = localPlayer >= 0 && localPlayer < s->numPlayers ? localPlayer : 0;
    s->network = true;
    for (i = 0; i < kMaxPlayers; i++)
        s->input[i] = kSplitInputKeyboard;
    s->pending = true;
}

void SplitCancel(void)
{
    if (gSplit != NULL && !gSplit->active)
        gSplit->pending = false;
}

Boolean SplitScreenPending(void)
{
    return gSplit != NULL && gSplit->pending;
}

Boolean SplitScreenActive(void)
{
    return gSplit != NULL && gSplit->active;
}

int SplitScreenMode(void)
{
    return gSplit != NULL && (gSplit->active || gSplit->pending) ? gSplit->mode : kSplitModeNone;
}

/// The player whose game is running now (0 outside split screen).
int SplitCurrentPlayer(void)
{
    return gSplit != NULL && gSplit->active && gSplit->running >= 0 ? gSplit->running : 0;
}

/// True while running inside one player's game.
static Boolean InPlayerGame(void)
{
    return gSplit != NULL && gSplit->active && gSplit->running >= 0 && MMCoroutineCurrent() != NULL;
}

static SplitPlayer *RunningPlayer(void)
{
    return InPlayerGame() ? &gSplit->players[gSplit->running] : NULL;
}

// ---------------------------------------------------------------------------
// The program's globals: one image per player

#if defined(__APPLE__)
/// The executable's writable variables: __data, __bss and __common, in __DATA and (where the
/// linker moved frequently written ones) __DATA_DIRTY.
static void FindSegments(Split *s)
{
    static const char *segments[] = { "__DATA", "__DATA_DIRTY" };
    static const char *sections[] = { "__data", "__bss", "__common" };
    unsigned long size;
    uint8_t *p;
    int i, j;

    s->numSegments = 0;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 3 && s->numSegments < kMaxSegments; j++) {
            p = getsectiondata(&_mh_execute_header, segments[i], sections[j], &size);
            if (p != NULL && size > 0) {
                s->segments[s->numSegments].addr = p;
                s->segments[s->numSegments].size = size;
                s->numSegments++;
            }
        }
}
#elif defined(MM_PS3)
/*  Everything is linked into one program on the PS3 (the C library, SDL): only the game's own
    variables are swapped, which platform/ps3/GameDataBegin.c and GameDataEnd.c bracket. */
extern char __mm_game_data_start[], __mm_game_data_end[], __mm_game_bss_start[], __mm_game_bss_end[];

static void FindSegments(Split *s)
{
    s->segments[0].addr = (unsigned char *)__mm_game_data_start;
    s->segments[0].size = (size_t)(__mm_game_data_end - __mm_game_data_start);
    s->segments[1].addr = (unsigned char *)__mm_game_bss_start;
    s->segments[1].size = (size_t)(__mm_game_bss_end - __mm_game_bss_start);
    s->numSegments = 2;
}
#else
extern char __data_start[], _end[];

static void FindSegments(Split *s)
{
    s->segments[0].addr = (unsigned char *)__data_start;
    s->segments[0].size = (size_t)(_end - __data_start);
    s->numSegments = 1;
}
#endif

/// Leaves a variable alone when players are swapped: every player sees the same one.
static void Share(void *addr, size_t size)
{
    Split *s = gSplit;

    if (s->numShared < kMaxShared && size > 0) {
        s->shared[s->numShared].addr = (unsigned char *)addr;
        s->shared[s->numShared].size = size;
        s->numShared++;
    }
}

#define SHARE(v) Share(&(v), sizeof(v))

static int CompareRanges(const void *a, const void *b)
{
    const MemRange *ra = a, *rb = b;
    return ra->addr < rb->addr ? -1 : ra->addr > rb->addr ? 1 : 0;
}

/// Splits the segments around the shared variables.
static void ComputeRanges(Split *s)
{
    size_t offset = 0;
    int seg, i;

    qsort(s->shared, (size_t)s->numShared, sizeof(MemRange), CompareRanges);
    s->numRanges = 0;
    for (seg = 0; seg < s->numSegments; seg++) {
        unsigned char *pos = s->segments[seg].addr, *end = pos + s->segments[seg].size;
        size_t segOffset = offset;

        for (i = 0; i < s->numShared && s->numRanges < kMaxRanges - 1; i++) {
            unsigned char *a = s->shared[i].addr, *b = a + s->shared[i].size;
            if (b <= pos || a >= end)
                continue;
            if (a > pos) {
                s->ranges[s->numRanges].addr = pos;
                s->ranges[s->numRanges].size = (size_t)(a - pos);
                s->ranges[s->numRanges].offset = segOffset + (size_t)(pos - s->segments[seg].addr);
                s->numRanges++;
            }
            if (b > pos)
                pos = b;
        }
        if (pos < end && s->numRanges < kMaxRanges) {
            s->ranges[s->numRanges].addr = pos;
            s->ranges[s->numRanges].size = (size_t)(end - pos);
            s->ranges[s->numRanges].offset = segOffset + (size_t)(pos - s->segments[seg].addr);
            s->numRanges++;
        }
        offset += s->segments[seg].size;
    }
    s->imageSize = offset;
}

static void SaveGlobals(unsigned char *image)
{
    int i;
    for (i = 0; i < gSplit->numRanges; i++)
        memcpy(image + gSplit->ranges[i].offset, gSplit->ranges[i].addr, gSplit->ranges[i].size);
}

static void LoadGlobals(const unsigned char *image)
{
    Split *s = gSplit;      // the pointer itself is shared, but keep it in a register anyway
    int i;
    for (i = 0; i < s->numRanges; i++)
        memcpy(s->ranges[i].addr, image + s->ranges[i].offset, s->ranges[i].size);
}

void MMMixerShareState(void (*share)(void *, size_t));
void MMSoundsShareState(void (*share)(void *, size_t));
void MyMusicShareState(void (*share)(void *, size_t));
void MMControllerShareState(void (*share)(void *, size_t));
void NetShareState(void (*share)(void *, size_t));
void MultiplayerShareState(void (*share)(void *, size_t));
void MMCoroutineGetState(void **addrP, size_t *sizeP, void **addr2P, size_t *size2P);
void MMSoundsSplitFrameEnd(void);
void GameArraysShareState(void (*share)(void *, size_t));

extern short gSoundVolume, gTitleMusicVolume, gBackgroundMusicVolume;
extern Boolean gArrowKeysEnabled;

static void RegisterSharedState(Split *s)
{
    void *a, *b;
    size_t sa, sb;

    s->numShared = 0;
    SHARE(gSplit);
    MMCoroutineGetState(&a, &sa, &b, &sb);
    Share(a, sa);
    Share(b, sb);
    SHARE(gSWPresentHook);
    SHARE(gSWForceFrames);
        // the window, sound, music and input devices
    MMMixerShareState(Share);
    MMSoundsShareState(Share);
    MyMusicShareState(Share);
    MMControllerShareState(Share);
    NetShareState(Share);
    OnlineShareState(Share);
    ChatShareState(Share);
    MultiplayerShareState(Share);
    SHARE(gQuitGame);
    SHARE(gMMIsActive);
    SHARE(gFullScreenWindow);
    SHARE(gWindowIsMinimized);
    SHARE(gWasFullscreenWhenMinimized);
    SHARE(gPrevWindowWidth);
    SHARE(gPrevWindowHeight);
        // settings
    SHARE(gExtras);
    SHARE(gKeyPrefs);
    SHARE(gArrowKeysEnabled);
    SHARE(gMusicIsOn);
    SHARE(gSofterMusic);
    SHARE(gSoundIsTurnedOn);
    SHARE(gSoundVolume);
    SHARE(gTitleMusicVolume);
    SHARE(gBackgroundMusicVolume);
        // lifetime stats are per player profile; each game updates its own player's
    SHARE(gStats);
    if (s->mode == kSplitModeCoop) {
            // one mansion: what's been collected, done, explored; and the team's score and items.
            // Lives are each player's own: whoever runs out sits out, the rest play on.
        GameArraysShareState(Share);
        CoopSyncShareState(Share);
        SHARE(gScore);
        SHARE(gOldScore);
        SHARE(gObjects);
        SHARE(gNumItemsBeingCarried);
        SHARE(gNumSecretsFound);
    }
    ComputeRanges(s);
}

// ---------------------------------------------------------------------------
// Offscreen framebuffers (OpenGL 3.0 / ARB_framebuffer_object, or the EXT version)

#ifndef GL_FRAMEBUFFER
#define GL_FRAMEBUFFER          0x8D40
#define GL_COLOR_ATTACHMENT0    0x8CE0
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#endif
#ifndef APIENTRY
#define APIENTRY
#endif

typedef void (APIENTRY *GenFramebuffersProc)(GLsizei, GLuint *);
typedef void (APIENTRY *DeleteFramebuffersProc)(GLsizei, const GLuint *);
typedef void (APIENTRY *BindFramebufferProc)(GLenum, GLuint);
typedef void (APIENTRY *FramebufferTexture2DProc)(GLenum, GLenum, GLenum, GLuint, GLint);
typedef GLenum (APIENTRY *CheckFramebufferStatusProc)(GLenum);

static GenFramebuffersProc pGenFramebuffers;
static DeleteFramebuffersProc pDeleteFramebuffers;
static BindFramebufferProc pBindFramebuffer;
static FramebufferTexture2DProc pFramebufferTexture2D;
static CheckFramebufferStatusProc pCheckFramebufferStatus;

static void *GLProc(const char *name)
{
    char ext[80];
    void *p = SDL_GL_GetProcAddress(name);

    if (p == NULL) {
        snprintf(ext, sizeof(ext), "%sEXT", name);
        p = SDL_GL_GetProcAddress(ext);
    }
    return p;
}

static Boolean LoadFramebufferProcs(void)
{
    if (pGenFramebuffers != NULL)
        return true;
    pGenFramebuffers = (GenFramebuffersProc)GLProc("glGenFramebuffers");
    pDeleteFramebuffers = (DeleteFramebuffersProc)GLProc("glDeleteFramebuffers");
    pBindFramebuffer = (BindFramebufferProc)GLProc("glBindFramebuffer");
    pFramebufferTexture2D = (FramebufferTexture2DProc)GLProc("glFramebufferTexture2D");
    pCheckFramebufferStatus = (CheckFramebufferStatusProc)GLProc("glCheckFramebufferStatus");
    return pGenFramebuffers && pDeleteFramebuffers && pBindFramebuffer && pFramebufferTexture2D &&
           pCheckFramebufferStatus;
}

static void DisposePlayerFramebuffer(SplitPlayer *p)
{
    if (p->fbo != 0)
        pDeleteFramebuffers(1, &p->fbo);
    if (p->texture != 0)
        glDeleteTextures(1, &p->texture);
    p->fbo = 0;
    p->texture = 0;
}

static Boolean MakePlayerFramebuffer(SplitPlayer *p, int w, int h)
{
    GLint oldTexture = 0;

    if (p->fbo != 0 && p->texW == w && p->texH == h)
        return true;
    DisposePlayerFramebuffer(p);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &oldTexture);
    glGenTextures(1, &p->texture);
    glBindTexture(GL_TEXTURE_2D, p->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    pGenFramebuffers(1, &p->fbo);
    pBindFramebuffer(GL_FRAMEBUFFER, p->fbo);
    pFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, p->texture, 0);
    if (pCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        pBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, (GLuint)oldTexture);
        DisposePlayerFramebuffer(p);
        return false;
    }
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    pBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, (GLuint)oldTexture);
    p->texW = w;
    p->texH = h;
    return true;
}

/// Points a player's worlds at their framebuffer (the whole of it).
static void ApplyPlayerSlot(SplitPlayer *p)
{
    SWRect r;

    SWSetRect(&r, 0, 0, (short)p->texW, (short)p->texH);
    if (p->world != NULL) {
        p->world->slotTargetHeight = p->texH;
        SWSetSpriteWorldSlot(p->world, &r);
    }
    if (p->statsWorld != NULL) {
        p->statsWorld->slotTargetHeight = p->texH;
        SWSetSpriteWorldSlot(p->statsWorld, &r);
    }
}

// ---------------------------------------------------------------------------
// Layout: 4:3 viewports, side by side for two, two by two for three or four

static void ComputeLayout(Split *s)
{
    int W, H, cols, rows, gap, cellW, cellH, w, h, i, n, x0, y0;

    SDL_GL_GetDrawableSize(gSpriteWorldP->mainWindow, &W, &H);
    if (W <= 0 || H <= 0)
        SDL_GetWindowSize(gSpriteWorldP->mainWindow, &W, &H);
    s->windowW = W;
    s->windowH = H;
    cols = s->numLocal <= 1 ? 1 : 2;
    rows = s->numLocal <= 2 ? 1 : 2;
    gap = W / 90 < 6 ? 6 : W / 90;         // as narrow as stays clear: the views get the room
    cellW = (W - gap * (cols + 1)) / cols;
    cellH = (H - gap * (rows + 1)) / rows;
    w = cellW;
    h = w * 3 / 4;
    if (h > cellH) {
        h = cellH;
        w = h * 4 / 3;
    }
        // centered as a block, so the column between them stays one width
    x0 = (W - (w * cols + gap * (cols - 1))) / 2;
    y0 = (H - (h * rows + gap * (rows - 1))) / 2;
    for (i = 0, n = 0; i < s->numPlayers; i++) {
        SplitPlayer *p = &s->players[i];
        int col = n % cols, row = n / cols;
        if (p->remote)
            continue;
        n++;
        p->slot.left = (short)(x0 + col * (w + gap));
        p->slot.top = (short)(y0 + row * (h + gap));
        p->slot.right = (short)(p->slot.left + w);
        p->slot.bottom = (short)(p->slot.top + h);
        p->view = p->slot;
        if (s->numLocal == 1) {
                // just one view here: the whole window, the game letterboxing itself as usual
            int vw = W, vh = W * 3 / 4;
            if (vh > H) {
                vh = H;
                vw = H * 4 / 3;
            }
            SWSetRect(&p->slot, 0, 0, (short)W, (short)H);
            SWSetRect(&p->view, (short)((W - vw) / 2), (short)((H - vh) / 2), (short)((W + vw) / 2), (short)((H + vh) / 2));
            w = W;
            h = H;
        }
        MakePlayerFramebuffer(p, w, h);
        ApplyPlayerSlot(p);
    }
}

// ---------------------------------------------------------------------------
// Input: each player's keys and controller go to their own game only

static int ControllerOwner(Split *s, SDL_JoystickID id)
{
    int pad = ControllerIndexForInstanceID(id), i;

    if (pad < 0)
        return -1;
    for (i = 0; i < s->numPlayers; i++)
        if (s->input[i] == kSplitInputPad1 + pad)
            return i;
    return -1;
}

static int InputOwner(Split *s, int input)
{
    int i;
    for (i = 0; i < s->numPlayers; i++)
        if (s->input[i] == input)
            return i;
    return -1;
}

static void Enqueue(SplitPlayer *p, const SDL_Event *e)
{
    int next = (p->queueTail + 1) % kEventQueueSize;

    if (next == p->queueHead)
        return;
    p->queue[p->queueTail] = *e;
    p->queueTail = next;
}

/// The left half of the keyboard (WASD) as one player's controls: which of the game's
/// controls (gKeyPrefs.primaryKeys index) a key is, or -1.
static int LeftKeyboardControl(SDL_Scancode sc)
{
    switch (sc) {
        case SDL_SCANCODE_W:      return 0;     // up
        case SDL_SCANCODE_D:      return 1;     // right
        case SDL_SCANCODE_S:      return 2;     // down
        case SDL_SCANCODE_A:      return 3;     // left
        case SDL_SCANCODE_F:      return 4;     // jump
        case SDL_SCANCODE_LSHIFT: return 4;
        case SDL_SCANCODE_Q:      return 5;     // jump left
        case SDL_SCANCODE_E:      return 6;     // jump right
        case SDL_SCANCODE_G:      return 7;     // action
        case SDL_SCANCODE_TAB:    return 8;     // map
        default:                  return -1;
    }
}

static void TogglePauseMenu(Split *s);
static void HandlePauseInput(Split *s, int press);
static void ResizeSplit(Split *s);

static void RouteKey(Split *s, SDL_Event *e)
{
    int control = LeftKeyboardControl(e->key.keysym.scancode), owner;
    SDL_Keycode sym = e->key.keysym.sym;

        // testing aid (MM_DEBUG_CHEATS): F5/F6 player 1 finishes/dies, F7/F8 player 2, ...
    if (sym >= SDLK_F5 && sym <= SDLK_F12 && getenv("MM_DEBUG_CHEATS") != NULL) {
        int player = (sym - SDLK_F5) / 2;
        if (e->type == SDL_KEYDOWN && player < s->numPlayers)
            s->players[player].debugAction = (sym - SDLK_F5) % 2 + 1;
        return;
    }

    if (e->key.keysym.sym == SDLK_ESCAPE || e->key.keysym.sym == SDLK_p) {
        if (e->type == SDL_KEYDOWN && !e->key.repeat)
            TogglePauseMenu(s);
        return;
    }
    if (s->paused || s->end != kEndNone) {
        if (e->type == SDL_KEYDOWN && !e->key.repeat) {
            switch (e->key.keysym.sym) {
                case SDLK_UP: case SDLK_w: case SDLK_KP_8: HandlePauseInput(s, kPadUp); break;
                case SDLK_DOWN: case SDLK_s: case SDLK_KP_2: HandlePauseInput(s, kPadDown); break;
                case SDLK_RETURN: case SDLK_SPACE: case SDLK_KP_ENTER: HandlePauseInput(s, kPadAccept); break;
                default: break;
            }
        }
        return;
    }
    owner = control >= 0 ? InputOwner(s, kSplitInputKeyboardLeft) : -1;
    if (owner >= 0) {
        e->key.keysym.scancode = (SDL_Scancode)gKeyPrefs.primaryKeys[control];
        e->key.keysym.sym = SDL_GetKeyFromScancode(e->key.keysym.scancode);
        e->key.keysym.mod = 0;
    } else {
        owner = InputOwner(s, kSplitInputKeyboard);
        if (control >= 0 && InputOwner(s, kSplitInputKeyboardLeft) < 0 && owner < 0)
            owner = -1;
    }
    if (owner >= 0)
        Enqueue(&s->players[owner], e);
}

static void RouteEvents(Split *s)
{
    SDL_Event e;
    int owner;

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                s->end = kEndQuit;
                gQuitGame = true;
                break;
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || e.window.event == SDL_WINDOWEVENT_RESIZED ||
                    e.window.event == SDL_WINDOWEVENT_MAXIMIZED || e.window.event == SDL_WINDOWEVENT_RESTORED)
                    ResizeSplit(s);
                else if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST && !s->paused && s->end == kEndNone)
                    TogglePauseMenu(s);
                break;
            case SDL_TEXTINPUT:
                if (s->network && !s->paused)
                    ChatHandleEvent(&e);
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                if (DebugVirtualPadKey(&e))
                    break;
                if (s->network && !s->paused && s->end == kEndNone && ChatHandleEvent(&e))
                    break;
                if ((e.key.keysym.mod & KMOD_GUI) != 0 && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q) {
                    s->end = kEndQuit;
                    gQuitGame = true;
                    break;
                }
                if ((e.key.keysym.mod & (KMOD_GUI | KMOD_ALT)) != 0 && e.type == SDL_KEYDOWN &&
                    (e.key.keysym.sym == SDLK_f || e.key.keysym.sym == SDLK_RETURN)) {
                    ToggleFullscreen();
                    ResizeSplit(s);
                    break;
                }
                RouteKey(s, &e);
                break;
            case SDL_CONTROLLERDEVICEADDED:
            case SDL_CONTROLLERDEVICEREMOVED:
            case SDL_CONTROLLERDEVICEREMAPPED:
                HandleControllerEvent(&e);
                break;
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
                if (e.cbutton.button == SDL_CONTROLLER_BUTTON_START) {
                    if (e.type == SDL_CONTROLLERBUTTONDOWN)
                        TogglePauseMenu(s);
                    break;
                }
                if (s->paused || s->end != kEndNone) {
                    if (e.type == SDL_CONTROLLERBUTTONDOWN) {
                        if (e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) HandlePauseInput(s, kPadUp);
                        else if (e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) HandlePauseInput(s, kPadDown);
                        else if (e.cbutton.button == SDL_CONTROLLER_BUTTON_A) HandlePauseInput(s, kPadAccept);
                        else if (e.cbutton.button == SDL_CONTROLLER_BUTTON_B && s->paused) TogglePauseMenu(s);
                    }
                    break;
                }
                owner = ControllerOwner(s, e.cbutton.which);
                if (owner >= 0)
                    Enqueue(&s->players[owner], &e);
                break;
            case SDL_CONTROLLERAXISMOTION:
                owner = ControllerOwner(s, e.caxis.which);
                if (owner >= 0 && !s->paused)
                    Enqueue(&s->players[owner], &e);
                break;
            default:
                break;      // the mouse and raw joystick events aren't used in split screen
        }
    }
}

/// From ProcessSDLEvents inside a player's game: hands over that player's input. Returns
/// false outside split screen (the events are read as usual).
Boolean SplitPumpPlayerEvents(void)
{
    SplitPlayer *p = RunningPlayer();

    if (p == NULL)
        return false;
    if (p->pumped && !p->presented)
        MMCoroutineYield();         // a loop waiting for something without drawing: next frame
    p->pumped = true;
    p->presented = false;
    while (p->queueHead != p->queueTail) {
        SDL_Event e = p->queue[p->queueHead];
        p->queueHead = (p->queueHead + 1) % kEventQueueSize;
        MyHandleSDLEvent(&e);
    }
    ControllerFrame();              // a made-up key press goes up again
    return true;
}

/// An event made up inside a player's game (a controller button standing in for a key): it
/// goes to that player's own queue. False outside split screen (it goes to SDL's).
Boolean SplitPostEvent(const SDL_Event *e)
{
    SplitPlayer *p = RunningPlayer();

    if (p == NULL)
        return false;
    Enqueue(p, e);
    return true;
}

/// GiveOSTime inside a player's game: nothing to give, the scheduler sleeps between frames.
Boolean SplitGiveOSTime(void)
{
    return InPlayerGame();
}

// ---------------------------------------------------------------------------
// Each player's game

/// A second (third, fourth) copy of what the title screen sets up once: the world, its
/// layers and tiles (the tile images are shared), and the status bar.
static void CreatePlayerWorlds(SplitPlayer *p, SpriteWorldPtr firstWorldP)
{
    SpriteWorldPtr w;

    MM_FATAL_IF(SWCreateSpriteWorldSharingWindow(&w, firstWorldP));
    gSpriteWorldP = w;
    CreateAndAddSpriteLayers();
    SetUpLadderArray();
    MM_FATAL_IF(SWInitTiling(w, 16, 16, firstWorldP->maxNumTiles));
    SWSetTileResolution(w, firstWorldP->tileImageWidth, firstWorldP->tileImageHeight);
    SWInstallTileLayerAboveSpriteLayer(0, gBackgroundPictureSpriteLayerP);
    SWInstallTileLayerBelowSpriteLayer(1, gLowerTileFringeSpriteLayerP);
    SWInstallTileLayerAboveSpriteLayer(2, gLowerTileFringeSpriteLayerP);
    SWInstallTileLayerAboveSpriteLayer(3, gDisappearingBlockSpriteLayerP);
    SWInstallTileLayerAboveSpriteLayer(4, gMovingPlatformBlockerSpriteLayerP);
    SWInstallTileLayerAboveSpriteLayer(5, gHeroOnPlatformSpriteLayerP);
    SWInstallTileLayerAboveSpriteLayer(6, gHeroSpriteLayerP);
    memcpy(w->tileFrameArray, firstWorldP->tileFrameArray, sizeof(FramePtr) * (size_t)w->maxNumTiles);
    memcpy(w->tileColorArray, firstWorldP->tileColorArray, sizeof(TileColorRec) * (size_t)w->maxNumTiles);
    memcpy(w->curTileImage, firstWorldP->curTileImage, sizeof(short) * (size_t)w->maxNumTiles);
    memcpy(w->tileSourceIndex, firstWorldP->tileSourceIndex, sizeof(short) * (size_t)w->maxNumTiles);
    SWSetSpriteWorldMaxFPS(w, 60);
    gStatsSpriteWorldP = NULL;
    CreateStatsArea();
    SWSetPostDrawCallBack(w, ExtrasPostDraw);
    gHeroSpriteLayerP->preDrawCallBack = DrawOtherPlayersBeforeHero;
    p->world = w;
    p->statsWorld = gStatsSpriteWorldP;
}

static void PublishHero(SplitPlayer *p)
{
    SpritePtr heroP = GetHeroSprite();

    if (gCurrentGameMode == kInGameMapMode) {
        p->room = GetMapPlayerLevel();     // looking at the map: still standing where they were
        return;
    }
    if (heroP == NULL)
        heroP = SWGetNextSprite(gDeadHeroSpriteLayerP, NULL);     // blinking, back after dying
    p->room = gCurrentLevel;
    p->heroVisible = heroP != NULL && heroP->isVisible;
    if (heroP != NULL) {
        p->heroX = heroP->drawData->horizLoc;
        p->heroY = heroP->drawData->vertLoc;
        p->heroFrame = (short)heroP->curFrameIndex;
        p->heroFlipped = heroP->drawData->flippedHorizontal;
        p->heroScaleW = heroP->drawData->scaledWidthFactor;
        p->heroScaleH = heroP->drawData->scaledHeightFactor;
    }
}

/// The co-op status bar shows the team's items: redo it when another player changed them.
static void SyncSharedStatusBar(SplitPlayer *p)
{
    if (gSplit->mode != kSplitModeCoop)
        return;
    if (memcmp(&p->lastObjects, &gObjects, sizeof(gObjects)) != 0) {
        RestoreStatsFromObjectsCarried();
        p->lastObjects = gObjects;
    }
}

/// Co-op: treasure another player took vanishes here too, with a puff of smoke.
static void SyncCollectedObjects(void)
{
    SpritePtr spriteP, nextP;

    if (gSplit->mode != kSplitModeCoop || gCurrentGameMode != kPlayingGameMode)
        return;
    for (spriteP = SWGetNextSprite(gObjectSpriteLayerP, NULL); spriteP != NULL; spriteP = nextP) {
        nextP = SWGetNextSprite(gObjectSpriteLayerP, spriteP);
        if (spriteP->spriteRemoval != kDontRemoveSprite || !IsSpriteInObjectArray(spriteP))
            continue;
        if (OBJECT(spriteP)->glowSpriteP != NULL)
            SWRemoveSpriteFromAnimation(OBJECT(spriteP)->glowSpriteP, true);
        SWCenterSpriteOverSprite(NewPuffOfSmokeSprite(), spriteP);
        SWRemoveSpriteFromAnimation(spriteP, true);
    }
        // a secret someone else found is found
    for (spriteP = SWGetNextSprite(gSecretIndicatorSpriteLayerP, NULL); spriteP != NULL; spriteP = nextP) {
        nextP = SWGetNextSprite(gSecretIndicatorSpriteLayerP, spriteP);
        if (spriteP->spriteRemoval == kDontRemoveSprite && IsSpriteInObjectArray(spriteP))
            SWRemoveSpriteFromAnimation(spriteP, true);
    }
}

// ---------------------------------------------------------------------------
// Arcade Split Screen saves: the team's game at a save pedestal (MMMultiSave.c keeps it for
// the group): what everyone shares, and each player's own room, place and lives, one file
// each ("<save>.<player ID>") beside the save itself.

/// The file with one player's part of a team save.
static void PlayerSavePath(const char *base, int player, char *out, size_t size)
{
    snprintf(out, size, "%s.%s", base, OnlineProfileID(GetPlayerProfileName(player)));
}

/// Runs fn with another player's game in the program's globals (from inside a player's game).
static void WithPlayerGlobals(SplitPlayer *q, void (*fn)(void *), void *context)
{
    Split *s = gSplit;
    SplitPlayer *me = RunningPlayer();
    unsigned char *mine, *theirs;

    if (me == NULL || q == me) {
        fn(context);
        return;
    }
    theirs = q->index == s->firstLocal ? s->liveImage : q->image;
    mine = malloc(s->imageSize);
    if (mine == NULL || theirs == NULL) {
        free(mine);
        return;
    }
    SaveGlobals(mine);
    LoadGlobals(theirs);
    fn(context);
    SaveGlobals(theirs);
    LoadGlobals(mine);
    free(mine);
}

/// One player's part: where they are (or will come back, if they're down), their lives...
static void WritePlayerState(void *context)
{
    const char *path = (const char *)context;
    SpritePtr heroP = NULL;
    short saveLevel = gCurrentLevel;
    float h = gStartingCol, v = gStartingRow;
    SDL_RWops *file;

    if (gCurrentGameMode == kInGameMapMode)
        gCurrentLevel = GetMapPlayerLevel();
    else if (gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kReadingSignMode)
        heroP = GetHeroSprite();
    if (heroP != NULL && !HERO(heroP)->isDead) {
        h = SWGetSpriteHorizLoc(heroP);
        v = SWGetSpriteVertLoc(heroP);
    } else if (gCurrentGameMode != kInGameMapMode) {
        gCurrentLevel = gLastLevelPositionWasSaved;     // down: back where they'd come back
    }
    file = SDL_RWFromFile(path, "wb");
    if (file != NULL) {
        WriteSaveDataToFile(file, h, v, false);
        SDL_RWclose(file);
    }
    gCurrentLevel = saveLevel;
}

/// From a save pedestal in Arcade Split Screen: saves the team's game, named after the
/// mansion ("Falcon Manor Normal 3"), and says so in the saver's view. (In a race the statue
/// only gives the lives back and brings the player back there: "Saved".)
void SplitSaveTeam(void)
{
    Split *s = gSplit;
    SplitPlayer *me = RunningPlayer();
    char mansion[255], base[1500], name[300], path[1600];
    const char *folder;
    struct stat info;
    int i, n;

    if (me == NULL)
        return;
    if (s->mode != kSplitModeCoop || s->network) {
        s->savedNoticeUntil[me->index] = SDL_GetTicks() + 2500;     // a race: lives back, a checkpoint
        return;
    }
    folder = MultiSaveFolderForCurrentGame(kMultiSaveArcade);
    if (folder == NULL)
        return;
    snprintf(mansion, sizeof(mansion), "%s", gCurOpenMansionFileName);
    StripStringFromEndOfCString(".zip", mansion, NULL);
    StripStringFromEndOfCString(".ZIP", mansion, NULL);
    for (n = 1; n < 10000; n++) {
        snprintf(name, sizeof(name), "%s %d", mansion, n);
        snprintf(base, sizeof(base), "%s/%s", folder, name);
        if (stat(base, &info) != 0)
            break;
    }
    for (i = 0; i < s->numPlayers; i++) {
        PlayerSavePath(base, i, path, sizeof(path));
        WithPlayerGlobals(&s->players[i], WritePlayerState, path);
    }
    if (MultiSaveWrite(kMultiSaveArcade, name))
        s->savedNoticeUntil[me->index] = SDL_GetTicks() + 2500;
}

/// Continuing a team save: this player's part (in their own game). False if it's missing.
static Boolean RestorePlayerState(SplitPlayer *p)
{
    char path[1600];
    float h, v;
    long version;
    Boolean isSaveFile;
    SpritePtr heroP;
    SDL_RWops *file;
    int err;

    PlayerSavePath(gSplit->continuePath, p->index, path, sizeof(path));
    file = SDL_RWFromFile(path, "rb");
    if (file == NULL)
        return false;
    err = LoadSaveDataFromFile(file, &h, &v, &isSaveFile, &version);
    SDL_RWclose(file);
    if (err != 0)
        return false;
    RestoreStatsFromObjectsCarried();
    ResetFloorTileAnimations(false, true);
    SetCorrectGoldSilverTileImages(false);
    LoadLevel(gCurrentLevel, true);
    SetUpLevelForPlaying();
    SWDisposeAllSpritesInLayer(gHeroSpriteLayerP);
    SWDisposeAllSpritesInLayer(gHeroOnPlatformSpriteLayerP);
    heroP = NewMainSprite();
    if (heroP != NULL) {
        SWSetSpriteLocation(heroP, h, v);
        SaveStartingPosition(heroP);
        UpdateHaloSpritePosition(heroP);
    }
    SaveHeroStartingLocation((short)h, (short)v);
    return true;
}

/// Continues a team save of Arcade Split Screen (after SplitConfigure): opens its mansion
/// and starts everyone where they were. Returns 0, or an error (the message is shown).
int SplitContinueSave(const char *path)
{
    char mansionName[255], bundled[1200];
    const char *mansionPath;
    float h, v;
    long version;
    Boolean isSaveFile;
    SDL_RWops *file = SDL_RWFromFile(path, "rb");
    int err;

    if (file == NULL || GetSplit() == NULL)
        return -1;
    err = LoadSaveDataFromFile(file, &h, &v, &isSaveFile, &version);
    SDL_RWclose(file);
    if (err != 0)
        return err;
    SetCurMansionAsCustomMansion(gCurrentMansionNumber == -1);
    snprintf(mansionName, sizeof(mansionName), "%s", gMansionFilename);
    StripStringFromEndOfCString(".zip", mansionName, NULL);
    StripStringFromEndOfCString(".ZIP", mansionName, NULL);
    mansionPath = GetPathForAnyMansionFile(mansionName);
    strcpy(gCustomMansionFolderPath, mansionPath);
    MultiplayerReleaseCharacterSprites();
    err = OpenMansionUsingFilePath(mansionPath, gMansionFilename);
    if (err != 0 && gMansionIsCustomMansion && GetBundledCustomMansionsPath()[0] != '\0') {
        snprintf(bundled, sizeof(bundled), "%s/%s/", GetBundledCustomMansionsPath(), mansionName);
        strcpy(gCustomMansionFolderPath, bundled);
        err = OpenMansionUsingFilePath(bundled, mansionName);
    }
    if (err != 0 && gMansionIsCustomMansion)
        err = OpenMansionUsingFilePath(GetMansionFolderPath(gMansionFilename), gMansionFilename);
    if (err != 0) {
        char message[600];
        snprintf(message, sizeof(message), "The mansion \"%s\" this game was saved in couldn't be found.", mansionName);
        MMDoCustomDialog(gSpriteWorldP, message, 3);
        SplitCancel();
        return err;
    }
    snprintf(gSplit->continuePath, sizeof(gSplit->continuePath), "%s", path);
    MMHideCursor();
    DoGameFadeToBlackFullAnim(30, true);
    if (gCurrentGameMode == kTitleScreenMode)
        ExitTitleScreen();
    StartGameNoEditor();
    return 0;
}

static void PlayerMain(void *arg)
{
    SplitPlayer *p = (SplitPlayer *)arg;
    Split *s = gSplit;

    if (p->index != s->firstLocal)
        CreatePlayerWorlds(p, s->players[s->firstLocal].world);
    ApplyPlayerSlot(p);
    MultiplayerSplitPlayerStarting(p->index);

        // the rest of StartGame, for this player
    SetUpNewGame();
    ChangeWorldRectForGame(gSpriteWorldP, gStatsHeight);
    SWSetPostEraseCallBack(gSpriteWorldP, MyPostEraseCallback);
    gCurrentGameMode = kPlayingGameMode;
    gRunningFrameCount = 0;
    if (!s->continuePath[0] || !RestorePlayerState(p)) {
        LoadLevel(s->levelNumber, true);
        SetUpLevelForPlaying();
    }
    SetCorrectGoldSilverTileImages(true);
    DrawLevel();
    gSpriteWorldIsValid = true;
    p->lastObjects = gObjects;
    p->status = kPlayerPlaying;
    DoGameFadeToNormal(0x1e);
    RunAnimation();

    while (p->status == kPlayerPlaying) {
        SplitPumpPlayerEvents();
        if (p->debugAction != 0 && gCurrentGameMode == kPlayingGameMode) {
            if (p->debugAction == 1)
                gBeatTheMansion = true;
            else if (GetHeroSprite() != NULL && !HERO(GetHeroSprite())->isDead)
                KillHero(GetHeroSprite());
            p->debugAction = 0;
        }
        switch (gCurrentGameMode) {
            case kPlayingGameMode:  RunAnimationCore(); break;
            case kInGameMapMode:    RunInGameMapScreen(); break;
            case kReadingSignMode:  HandleReadingSignCallback(); break;
            default:                break;
        }
        CoopSyncApply();
        SyncSharedStatusBar(p);
        SyncCollectedObjects();
        PublishHero(p);
        if (!p->presented)
            MMCoroutineYield();
    }
    for (;;)
        MMCoroutineYield();     // done: the scheduler stops running this game
}

/// From RunAnimationCore: the game ended for this player (game over, quit, or out of the
/// mansion). Returns true if split screen takes it from here.
Boolean SplitTakeGameEnd(void)
{
    SplitPlayer *p = RunningPlayer();
    Split *s = gSplit;
    int i, finished;

    if (p == NULL || (!gGameOver && !gEndGame && !gBeatTheMansion))
        return false;
    if (gEndGame) {
        gEndGame = false;
        s->end = kEndQuit;
        return true;
    }
    if (gBeatTheMansion) {
        p->status = kPlayerFinished;
        p->finishMs = SDL_GetTicks() - s->startTicks + (Uint32)p->deaths * kDeathPenaltyMs;
        for (i = 0, finished = 0; i < s->numPlayers; i++)
            if (s->players[i].status == kPlayerFinished)
                finished++;
        p->place = finished;
        if (s->mode == kSplitModeCoop)
            s->end = kEndTeamWon;
    } else {
        p->status = kPlayerOut;
        if (s->mode == kSplitModeCoop) {
                // out of lives: the others play on; the team has lost when nobody is left
            for (i = 0, finished = 0; i < s->numPlayers; i++)
                if (s->players[i].status == kPlayerPlaying || s->players[i].status == kPlayerStarting)
                    finished++;
            if (finished == 0)
                s->end = kEndTeamLost;
        }
    }
    return true;
}

/// Called when a hero dies (race: each death costs time).
void SplitNoteDeath(void)
{
    SplitPlayer *p = RunningPlayer();

    if (p != NULL)
        p->deaths++;
}

static void SplitPresent(SpriteWorldPtr spriteWorldP)
{
    SplitPlayer *p = RunningPlayer();

    if (p == NULL) {
        SDL_GL_SwapWindow(spriteWorldP->mainWindow);
        return;
    }
    p->presented = true;
    p->pumped = false;
    MMCoroutineYield();
}

static void RunPlayer(Split *s, SplitPlayer *p)
{
    pBindFramebuffer(GL_FRAMEBUFFER, p->fbo);
        // the GL state is everyone's: give this game the projection it would have in a window
        // of its framebuffer's size (Compose leaves the whole window's; the map screen and other
        // screens that draw by hand rely on it)
    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, p->texW, p->texH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, p->texW, p->texH, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (p->index != s->firstLocal) {
        SaveGlobals(s->liveImage);
        LoadGlobals(p->image);
    }
    s->running = p->index;
    MMCoroutineResume(p->co);
    s->running = -1;
    if (p->index != s->firstLocal) {
        SaveGlobals(p->image);
        LoadGlobals(s->liveImage);
    }
    pBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// ---------------------------------------------------------------------------
// Drawing the window

static GLuint LoadTexture(const char *relativePath, int *wP, int *hP)
{
    const char *path = GetExtrasFilePath(relativePath);
    SDL_Surface *surface, *rgba;
    GLuint texture = 0;

    if (path == NULL || (surface = BKLoadSurface(path)) == NULL)
        return 0;
    rgba = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surface);
    if (rgba == NULL)
        return 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, rgba->pitch / 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, rgba->w, rgba->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba->pixels);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    *wP = rgba->w;
    *hP = rgba->h;
    SDL_FreeSurface(rgba);
    return texture;
}

static void TexturedQuad(GLuint texture, float x0, float y0, float x1, float y1, float u0, float v0, float u1,
                         float v1)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(u0, v0); glVertex2f(x0, y0);
    glTexCoord2f(u1, v0); glVertex2f(x1, y0);
    glTexCoord2f(u1, v1); glVertex2f(x1, y1);
    glTexCoord2f(u0, v1); glVertex2f(x0, y1);
    glEnd();
}

static void FillRect(float x0, float y0, float x1, float y1, float r, float g, float b, float a)
{
    glDisable(GL_TEXTURE_2D);
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
    glVertex2f(x0, y0);
    glVertex2f(x1, y0);
    glVertex2f(x1, y1);
    glVertex2f(x0, y1);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

static MMFont *LoadFont(const char *name)
{
    char path[1024];

    snprintf(path, sizeof(path), "%s", GetFontsFolderPath(name));
    return MMFontLoad(path);
}

/// Text centered on x (justify 0), from x (1) or ending at x (2); y is the baseline.
static void DrawText(MMFont *font, int size, const char *text, float x, float y, int justify, float r, float g,
                     float b, float a)
{
    float width;

    if (font == NULL)
        return;
    MMFontFaceSize(font, size);
    width = MMFontRenderWidth(font, text);
    if (justify == 0)
        x -= width / 2;
    else if (justify == 2)
        x -= width;
    glColor4f(0, 0, 0, a * 0.8f);
    MMFontRender(font, text, x + 2, y + 2, 1.0f);
    glColor4f(r, g, b, a);
    MMFontRender(font, text, x, y, 1.0f);
}

static void FormatRaceTime(char *text, size_t size, Uint32 ms)
{
    snprintf(text, size, "%u:%02u.%u", ms / 60000, (ms / 1000) % 60, (ms / 100) % 10);
}


static void DrawPlayerOverlay(Split *s, SplitPlayer *p, Uint32 now)
{
    const float *rgb = GetCharacterColor(GetPlayerCharacter(p->index));
    float x0 = p->view.left, y0 = p->view.top, x1 = p->view.right, y1 = p->view.bottom;
    float h = y1 - y0, unit = h / 480.0f;
    int small = (int)(15 * unit) < 10 ? 10 : (int)(15 * unit);
    char text[160], timeText[32];
    Uint32 elapsed;

        // the player's name under the status bar
    DrawText(s->font, small, GetPlayerDisplayName(p->index), x0 + 8 * unit, y0 + 66 * unit, 1, rgb[0], rgb[1],
             rgb[2], 0.9f);
    if (s->mode == kSplitModeRace) {
        elapsed = (p->status == kPlayerFinished) ? p->finishMs
                                                 : now - s->startTicks + (Uint32)p->deaths * kDeathPenaltyMs;
        FormatRaceTime(timeText, sizeof(timeText), elapsed);
        if (p->deaths > 0)
            snprintf(text, sizeof(text), "%s  (+%ds)", timeText, p->deaths * (kDeathPenaltyMs / 1000));
        else
            snprintf(text, sizeof(text), "%s", timeText);
        DrawText(s->font, small, text, x1 - 8 * unit, y0 + 66 * unit, 2, 1, 1, 1, 0.9f);
    }
    if (now < s->savedNoticeUntil[p->index])
        DrawText(s->titleFont, (int)(30 * unit), s->mode == kSplitModeCoop && !s->network ? "Team game saved" : "Saved", (x0 + x1) / 2, y0 + h * 0.3f, 0, 1, 0.85f, 0.3f,
                 (float)(s->savedNoticeUntil[p->index] - now) / 2500.0f);
    if (p->status == kPlayerFinished || p->status == kPlayerOut) {
        static const char *places[] = { "", "1st", "2nd", "3rd", "4th" };
        FillRect(x0, y0, x1, y1, 0, 0, 0, 0.55f);
        if (p->status == kPlayerFinished) {
            FormatRaceTime(timeText, sizeof(timeText), p->finishMs);
            snprintf(text, sizeof(text), "%s place!", places[p->place < 5 ? p->place : 4]);
            DrawText(s->titleFont, (int)(40 * unit), s->mode == kSplitModeRace ? text : "Escaped!", (x0 + x1) / 2,
                     y0 + h * 0.45f, 0, 1, 0.85f, 0.3f, 1);
            DrawText(s->font, (int)(22 * unit), timeText, (x0 + x1) / 2, y0 + h * 0.58f, 0, 1, 1, 1, 1);
        } else {
            DrawText(s->titleFont, (int)(36 * unit), "Out of lives", (x0 + x1) / 2, y0 + h * 0.5f, 0, 1, 0.4f,
                     0.3f, 1);
        }
    }
}

/// Network race: the other computer's player, on the wood under this computer's view.
static void DrawRemotePlayerStatus(Split *s, SplitPlayer *p, int line, Uint32 now)
{
    SplitPlayer *me = &s->players[s->firstLocal];
    const float *rgb = GetCharacterColor(GetPlayerCharacter(p->index));
    float unit = (me->view.bottom - me->view.top) / 480.0f, y = me->view.top + (66 + 20 * line) * unit;
    char text[200], timeText[32];

    (void)now;
    if (p->status == kPlayerFinished) {
        FormatRaceTime(timeText, sizeof(timeText), p->finishMs);
        snprintf(text, sizeof(text), "%s finished in %s", GetPlayerDisplayName(p->index), timeText);
    } else if (p->status == kPlayerOut) {
        snprintf(text, sizeof(text), p->left ? "%s left the game" : "%s is out of lives", GetPlayerDisplayName(p->index));
    } else {
        FormatRaceTime(timeText, sizeof(timeText), p->remoteElapsedMs);
        snprintf(text, sizeof(text), "%s  %s", GetPlayerDisplayName(p->index), timeText);
    }
    DrawText(s->font, (int)(20 * unit) < 11 ? 11 : (int)(20 * unit), text, (me->view.left + me->view.right) / 2.0f, y,
             0, rgb[0], rgb[1], rgb[2], 0.95f);
}

static const char *kPauseChoices[] = { "Resume", "Quit to title" };

/// Bricks as big as the mansion's own walls look in the views.
static float BrickSize(Split *s)
{
    SplitPlayer *p = &s->players[s->firstLocal];
    float h = (float)(p->view.bottom - p->view.top);
    return h > 0 ? h / 480.0f * 32.0f : 48.0f;
}

static void DrawCenteredPanel(Split *s, float w, float h, float *x0P, float *y0P)
{
    float x0 = (s->windowW - w) / 2, y0 = (s->windowH - h) / 2;

    FillRect(0, 0, (float)s->windowW, (float)s->windowH, 0, 0, 0, 0.5f);
    glColor4f(0.8f, 0.8f, 0.8f, 1);
    if (s->brickTexture != 0)
        TexturedQuad(s->brickTexture, x0, y0, x0 + w, y0 + h, 0, 0, w / BrickSize(s), h / BrickSize(s));
    else
        FillRect(x0, y0, x0 + w, y0 + h, 0.1f, 0.1f, 0.1f, 1);
    FillRect(x0, y0, x0 + w, y0 + 3, 0.84f, 0.67f, 0.34f, 1);
    FillRect(x0, y0 + h - 3, x0 + w, y0 + h, 0.84f, 0.67f, 0.34f, 1);
    *x0P = x0;
    *y0P = y0;
}

static void DrawMenu(Split *s, const char *title, const char **choices, int count, float extraHeight)
{
    float unit = s->windowH / 768.0f, w = 520 * unit, h = (150 + 56 * count) * unit + extraHeight, x0, y0;
    int i;

    DrawCenteredPanel(s, w, h, &x0, &y0);
    DrawText(s->titleFont, (int)(44 * unit), title, s->windowW / 2.0f, y0 + 70 * unit, 0, 1, 0.85f, 0.3f, 1);
    for (i = 0; i < count; i++) {
        Boolean on = i == s->pauseChoice;
        DrawText(s->font, (int)(28 * unit), choices[i], s->windowW / 2.0f,
                 y0 + extraHeight + (130 + 56 * i) * unit, 0, on ? 1.0f : 0.75f, on ? 1.0f : 0.7f, on ? 0.4f : 0.6f, 1);
    }
}

static void DrawResults(Split *s)
{
    static const char *choices[] = { "Back to the title screen" };
    float unit = s->windowH / 768.0f, lineH = 40 * unit, w, h, x0, y0, y;
    int order[kMaxPlayers], i, j, n = s->numPlayers;
    char text[200], timeText[32];

    if (s->mode != kSplitModeRace) {
        static const char *next[] = { "Continue" };
        s->pauseChoice = 0;
        DrawMenu(s, s->end == kEndTeamWon ? "Mansion escaped!" : "Game over", next, 1, 0);
        return;
    }
    for (i = 0; i < n; i++)
        order[i] = i;
    for (i = 0; i < n; i++)         // finishers by time, then the rest
        for (j = i + 1; j < n; j++) {
            SplitPlayer *a = &s->players[order[i]], *b = &s->players[order[j]];
            Boolean swap = (b->status == kPlayerFinished && a->status != kPlayerFinished) ||
                           (a->status == kPlayerFinished && b->status == kPlayerFinished && b->finishMs < a->finishMs);
            if (swap) {
                int t = order[i];
                order[i] = order[j];
                order[j] = t;
            }
        }
    w = 620 * unit;
    h = (230 + lineH / unit * n) * unit;
    DrawCenteredPanel(s, w, h, &x0, &y0);
    DrawText(s->titleFont, (int)(44 * unit), "Race results", s->windowW / 2.0f, y0 + 70 * unit, 0, 1, 0.85f, 0.3f, 1);
    for (i = 0, y = y0 + 125 * unit; i < n; i++, y += lineH) {
        SplitPlayer *p = &s->players[order[i]];
        const float *rgb = GetCharacterColor(GetPlayerCharacter(p->index));
        DrawText(s->font, (int)(26 * unit), GetPlayerDisplayName(p->index), x0 + 60 * unit, y, 1, rgb[0], rgb[1],
                 rgb[2], 1);
        if (p->status == kPlayerFinished) {
            FormatRaceTime(timeText, sizeof(timeText), p->finishMs);
            if (p->deaths > 0)
                snprintf(text, sizeof(text), "%s  (%d %s)", timeText, p->deaths, p->deaths == 1 ? "death" : "deaths");
            else
                snprintf(text, sizeof(text), "%s", timeText);
        } else {
            snprintf(text, sizeof(text), "did not finish");
        }
        DrawText(s->font, (int)(26 * unit), text, x0 + w - 60 * unit, y, 2, 1, 1, 1, 1);
    }
    DrawText(s->font, (int)(24 * unit), choices[0], s->windowW / 2.0f, y0 + h - 40 * unit, 0, 1, 1, 0.4f, 1);
}

static void Compose(Split *s)
{
    Uint32 now = s->paused ? s->pausedAt : SDL_GetTicks();
    float W = (float)s->windowW, H = (float)s->windowH;
    int i, n;

    pBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, s->windowW, s->windowH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, W, H, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

        // split screen: the mansion's dark brick all round and between the views (one view
        // on this computer, as in a race against another computer: just the game, full size)
    if (s->numLocal > 1) {
        glColor4f(1, 1, 1, 1);
        if (s->brickTexture != 0)
            TexturedQuad(s->brickTexture, 0, 0, W, H, 0, 0, W / BrickSize(s), H / BrickSize(s));
    }

    for (i = 0; i < s->numPlayers; i++) {
        SplitPlayer *p = &s->players[i];
        float x0 = p->slot.left, y0 = p->slot.top, x1 = p->slot.right, y1 = p->slot.bottom;
        if (p->remote)
            continue;
        if (s->numLocal > 1) {
            FillRect(x0 - 3, y0 - 3, x1 + 3, y1 + 3, 0, 0, 0, 1);
            FillRect(x0 - 1, y0 - 1, x1 + 1, y1 + 1, 0.35f, 0.33f, 0.30f, 1);
        }
        glColor4f(1, 1, 1, 1);
        glDisable(GL_BLEND);
        if (p->texture != 0)
            TexturedQuad(p->texture, x0, y0, x1, y1, 0, 1, 1, 0);   // framebuffers are bottom-up
        glEnable(GL_BLEND);
        DrawPlayerOverlay(s, p, now);
    }
    for (i = 0, n = 0; i < s->numPlayers; i++)
        if (s->players[i].remote)
            DrawRemotePlayerStatus(s, &s->players[i], n++, now);     // one line each, under the status bar
    if (s->network) {
            // chat with the other computer, at the bottom of this computer's view
        SplitPlayer *me = &s->players[s->firstLocal];
        float unit = (me->view.bottom - me->view.top) / 480.0f;
        ChatDraw(me->view.left + 12 * unit, me->view.bottom - 16 * unit, (me->view.right - me->view.left) * 0.7f,
                 15 * unit, 1.0f);
    }
    if (s->numLocal == 3) {
            // the fourth corner: the mansion's name on the wood
        SplitPlayer *a = &s->players[2], *b = &s->players[1];
        float cx = b->slot.left + (b->slot.right - b->slot.left) / 2.0f, cy = a->slot.top + (a->slot.bottom - a->slot.top) / 2.0f;
        float unit = (a->slot.bottom - a->slot.top) / 480.0f;
        DrawText(s->titleFont, (int)(40 * unit), gMansionName, cx, cy, 0, 1, 0.85f, 0.3f, 1);
        if (s->mode == kSplitModeRace)
            DrawText(s->font, (int)(22 * unit), "Ghost Race", cx, cy + 40 * unit, 0, 1, 1, 1, 0.9f);
    }

    if (s->end != kEndNone && s->end != kEndQuit && SDL_GetTicks() - s->endShownAt > 1500)
        DrawResults(s);
    else if (s->paused)
        DrawMenu(s, "Paused", kPauseChoices, 2, 0);

    glColor4f(1, 1, 1, 1);
    SDL_GL_SwapWindow(gSpriteWorldP->mainWindow);
}

// ---------------------------------------------------------------------------
// Pause and results menus

static void TogglePauseMenu(Split *s)
{
    int i;

    if (s->end != kEndNone)
        return;
    s->paused = !s->paused;
    s->pauseChoice = 0;
    if (s->paused) {
        s->pausedAt = SDL_GetTicks();
        PauseMusic();
    } else {
        Uint32 pausedFor = SDL_GetTicks() - s->pausedAt;
        s->startTicks += pausedFor;
        for (i = 0; i < s->numPlayers; i++)
            s->players[i].queueHead = s->players[i].queueTail;     // drop keys pressed meanwhile
        ResumeMusic(false);
    }
}

static void HandlePauseInput(Split *s, int press)
{
    if (s->end != kEndNone) {
        if (press == kPadAccept && SDL_GetTicks() - s->endShownAt > 1500)
            s->end = s->end == kEndRaceOver || s->end == kEndTeamWon || s->end == kEndTeamLost ? -s->end : s->end;
        return;
    }
    if (press == kPadUp)
        s->pauseChoice = (s->pauseChoice + 1) % 2;
    else if (press == kPadDown)
        s->pauseChoice = (s->pauseChoice + 1) % 2;
    else if (press == kPadAccept) {
        if (s->pauseChoice == 0)
            TogglePauseMenu(s);
        else {
            TogglePauseMenu(s);
            s->end = kEndQuit;
        }
    }
}

static void ResizeSplit(Split *s)
{
    int i;

    ComputeLayout(s);
    for (i = 0; i < s->numPlayers; i++) {
        SplitPlayer *p = &s->players[i];
        if (p->world != NULL)
            ApplyPlayerSlot(p);
    }
    SWUpdateWindowSize(gSpriteWorldP);
}

// ---------------------------------------------------------------------------
// Starting and ending

/// From StartGame once the title screen has gone: starts every player's game instead.
void SplitScreenBegin(short levelNumber)
{
    Split *s = gSplit;
    int i;

    if (s == NULL || !s->pending)
        return;
    if (!LoadFramebufferProcs()) {
        ErrorMessage("Split screen needs OpenGL framebuffer objects, which this computer doesn't have.", 0);
        s->pending = false;
        return;
    }
    s->pending = false;
    s->levelNumber = levelNumber;
    s->end = kEndNone;
    s->endShownAt = 0;
    s->notice[0] = '\0';
    s->paused = false;
    if (s->numSegments == 0)
        FindSegments(s);
    gSWPresentHook = SplitPresent;
    gSWForceFrames = false;
    RegisterSharedState(s);
    if (s->liveImage == NULL)
        s->liveImage = malloc(s->imageSize);
    if (s->brickTexture == 0)
        s->brickTexture = LoadTexture("Screens/split/brick.png", &s->brickW, &s->brickH);
    if (s->font == NULL)
        s->font = LoadFont("Action Man.ttf");
    if (s->titleFont == NULL)
        s->titleFont = LoadFont("BlackChancery.TTF");

    for (i = 0; i < s->numPlayers; i++) {
        SplitPlayer *p = &s->players[i];
        memset(p, 0, sizeof(*p));
        p->index = i;
        p->input = s->input[i];
        p->remote = s->network && i != s->firstLocal;
        p->status = p->remote ? kPlayerPlaying : kPlayerStarting;
        p->world = i == s->firstLocal ? gSpriteWorldP : NULL;
        p->statsWorld = i == s->firstLocal ? gStatsSpriteWorldP : NULL;
    }
    ComputeLayout(s);
    gHeroSpriteLayerP->preDrawCallBack = DrawOtherPlayersBeforeHero;
    CoopSyncReset();
    s->active = true;               // before the images are taken: every game sees split screen on
    for (i = 0; i < s->numPlayers; i++) {
        SplitPlayer *p = &s->players[i];
        if (p->remote || i == s->firstLocal)
            continue;
        p->image = malloc(s->imageSize);
        if (p->image == NULL)
            MM_FATAL(kMemoryAllocationError);
        SaveGlobals(p->image);
    }
    for (i = 0; i < s->numPlayers; i++) {
        if (s->players[i].remote)
            continue;
        s->players[i].co = MMCoroutineCreate(PlayerMain, &s->players[i], kCoroutineStack);
        if (s->players[i].co == NULL)
            MM_FATAL(kMemoryAllocationError);
    }
    s->startTicks = SDL_GetTicks();
    s->nextFrameAt = SDL_GetTicks();
    gCurrentGameMode = kPlayingGameMode;
    gPlayingGame = true;
}

/// Lets go of one player's game (not the first player's: that one ends as usual).
static void DisposePuppets(void);

static void ReleasePlayer(Split *s, SplitPlayer *p)
{
    if (p->remote)
        return;
    if (p->index != s->firstLocal) {
        SaveGlobals(s->liveImage);
        LoadGlobals(p->image);
        AbandonInGameScreens();
        DisposePuppets();
        DisposeCurrentRoom(true);
        DisposeAllMapCachedRooms();
        SWSetPostEraseCallBack(gSpriteWorldP, NULL);
        MMDisposeAllSpritesInSpriteWorld(gSpriteWorldP);
        SWDisposeBackgroundTileTexture(gSpriteWorldP);
        LoadGlobals(s->liveImage);
        free(p->image);
        p->image = NULL;
            // the worlds stay allocated (cheap) rather than risk freeing what they share
    }
    MMCoroutineDispose(p->co);
    p->co = NULL;
    DisposePlayerFramebuffer(p);
}

static void EndSplit(Split *s)
{
    int outcome = s->end < 0 ? -s->end : s->end, i;
    SplitPlayer *first = &s->players[s->firstLocal];

    for (i = 0; i < s->numPlayers; i++)
        ReleasePlayer(s, &s->players[i]);
    DisposePuppets();
    first->world->slotTargetHeight = 0;
    SWSetSpriteWorldSlot(first->world, NULL);
    if (first->statsWorld != NULL) {
        first->statsWorld->slotTargetHeight = 0;
        SWSetSpriteWorldSlot(first->statsWorld, NULL);
    }
    gSWPresentHook = NULL;
    gSWForceFrames = false;
    gHeroSpriteLayerP->preDrawCallBack = NULL;
    s->active = false;
    s->running = -1;
    s->continuePath[0] = 0;
    StopAllChannels();
    MultiplayerSplitEnded();

        // the first player's game ends as a normal game would
    gGameOver = false;
    gEndGame = false;
    gBeatTheMansion = false;
    if (gQuitGame)
        return;
    if (outcome == kEndTeamWon) {
        MMPauseGameTimer();
        CleanUpAfterGame();
        DoBeatTheMansionScreen(false);
    } else if (outcome == kEndTeamLost) {
        CleanUpAfterGame();
        DoGameOverScreen();             // the team's score may make the high scores
    } else {
        CleanUpAfterGame();
        StopAndDisposeMusic();
        SetUpTitleScreen();
    }
    if (s->notice[0] != '\0') {
        MMDoCustomDialog(gSpriteWorldP, s->notice, 3);
        s->notice[0] = '\0';
    }
}

/// The network race: tells the other computers where this player is, and hears where theirs are.
static void ExchangeWithOtherComputers(Split *s)
{
    SplitPlayer *me = &s->players[s->firstLocal];
    NetRaceState st;
    Boolean gone;
    Uint32 now = s->paused ? s->pausedAt : SDL_GetTicks();
    int i;

    memset(&st, 0, sizeof(st));
    st.room = me->room;
    st.mapRoom = me->room;
    st.x = me->heroX;
    st.y = me->heroY;
    st.scaleW = me->heroScaleW;
    st.scaleH = me->heroScaleH;
    st.frame = me->heroFrame;
    st.flipped = me->heroFlipped;
    st.visible = me->heroVisible;
    st.status = (Uint8)me->status;
    st.deaths = (Uint8)(me->deaths > 255 ? 255 : me->deaths);
    st.place = (Uint8)me->place;
    st.elapsedMs = me->status == kPlayerFinished ? me->finishMs
                                                 : now - s->startTicks + (Uint32)me->deaths * kDeathPenaltyMs;
    NetRaceSendState(&st);
    NetRacePump();

    for (i = 0; i < s->numPlayers; i++) {
        SplitPlayer *them = &s->players[i];
        if (!them->remote)
            continue;
        if (NetRaceGetState(i, &st, &gone)) {
            them->room = st.room;
            them->heroX = st.x;
            them->heroY = st.y;
            them->heroScaleW = st.scaleW;
            them->heroScaleH = st.scaleH;
            them->heroFrame = st.frame;
            them->heroFlipped = st.flipped;
            them->heroVisible = st.visible;
            them->deaths = st.deaths;
            them->remoteElapsedMs = st.elapsedMs;
            if (st.status == kPlayerFinished && them->status != kPlayerFinished) {
                them->status = kPlayerFinished;
                them->finishMs = st.elapsedMs;
                them->place = st.place;
            } else if (st.status == kPlayerOut) {
                them->status = kPlayerOut;
            }
        }
        if (gone && them->status != kPlayerFinished && them->status != kPlayerOut) {
            them->status = kPlayerOut;
            them->left = true;
        }
    }
    if (NetPeerMissingMansion() != NULL && s->end == kEndNone) {
            // someone doesn't have the mansion: no race, everyone back to the menu
        snprintf(s->notice, sizeof(s->notice),
                 "%s doesn't have the mansion \"%s\" installed, so they went back to the main menu.",
                 NetPlayerName(NetMissingMansionPlayer())[0] ? NetPlayerName(NetMissingMansionPlayer()) : "A player",
                 NetPeerMissingMansion());
        s->end = kEndQuit;
    }
}

/// One frame of split screen, from the main loop.
void SplitRunFrame(void)
{
    Split *s = gSplit;
    double now = (double)SDL_GetTicks();
    int i, playing;

    if (now < s->nextFrameAt) {
        SDL_Delay(1);
        return;
    }
    s->nextFrameAt += kFrameMs;
    if (s->nextFrameAt < now - 100)
        s->nextFrameAt = now + kFrameMs;
    RouteEvents(s);
    if (s->network)
        ExchangeWithOtherComputers(s);
    if (s->end == kEndQuit || s->end < 0 || gQuitGame) {
        EndSplit(s);
        return;
    }
    if (!s->paused) {
        gSWForceFrames = true;
        for (i = 0, playing = 0; i < s->numPlayers; i++) {
            SplitPlayer *p = &s->players[i];
            if (p->status == kPlayerStarting || p->status == kPlayerPlaying) {
                if (!p->remote)
                    RunPlayer(s, p);
                playing++;
            }
        }
        gSWForceFrames = false;
        MMSoundsSplitFrameEnd();
        if (s->end == kEndNone && playing == 0)
            s->end = s->mode == kSplitModeRace ? kEndRaceOver : kEndTeamLost;
        if (s->end != kEndNone && s->endShownAt == 0)
            s->endShownAt = SDL_GetTicks();
    }
    Compose(s);
}

/// The other players, for drawing them in a room and on the map.
int SplitGetOtherPlayers(SplitOtherPlayer *others, int max)
{
    Split *s = gSplit;
    int i, n = 0, me = SplitCurrentPlayer();

    if (s == NULL || !s->active)
        return 0;
    for (i = 0; i < s->numPlayers && n < max; i++) {
        SplitPlayer *p = &s->players[i];
        if (i == me || p->status == kPlayerStarting)
            continue;
        others[n].player = i;
        others[n].room = p->room;
        others[n].x = p->heroX;
        others[n].y = p->heroY;
        others[n].frame = p->heroFrame;
        others[n].flipped = p->heroFlipped;
        others[n].scaleW = p->heroScaleW;
        others[n].scaleH = p->heroScaleH;
        others[n].visible = p->heroVisible && p->status == kPlayerPlaying;
        n++;
    }
    return n;
}

// ---------------------------------------------------------------------------
// The other players in your room: phantoms in a race (see-through, with a trail of dust or
// sparkles), themselves in co-op. They can't touch anything in your game.

#define kMaxParticles   192
#define kPhantomAlpha   0x7333      ///< 45%

typedef struct Particle {
    float x, y, vx, vy, life, maxLife, r, g, b;
} Particle;

    // per player's game (these are swapped with the rest of the globals)
static SpritePtr gPuppets[kMaxPlayers];
static int gPuppetCharacter[kMaxPlayers];
static float gLastSeenX[kMaxPlayers], gLastSeenY[kMaxPlayers];
static Particle gParticles[kMaxParticles];
static int gNumParticles = 0;
static Uint32 gLastParticleTicks = 0;

static SpritePtr PuppetFor(int player)
{
    int character = GetPlayerCharacter(player);
    SpritePtr masterP;

    if (gPuppets[player] != NULL && gPuppetCharacter[player] == character)
        return gPuppets[player];
    if (gPuppets[player] != NULL)
        SWDisposeSprite(&gPuppets[player]);
    masterP = GetCharacterHeroMaster(character);
    if (masterP == NULL || SWCloneSprite(masterP, &gPuppets[player], NULL, 0) != kNoError) {
        gPuppets[player] = NULL;
        return NULL;
    }
    gPuppetCharacter[player] = character;
    SWSetSpriteDrawProc(gPuppets[player], SWStdSpriteDrawProc);
    return gPuppets[player];
}

static void DisposePuppets(void)
{
    int i;

    for (i = 0; i < kMaxPlayers; i++)
        if (gPuppets[i] != NULL)
            SWDisposeSprite(&gPuppets[i]);
    gNumParticles = 0;
}

static float Random01(void)
{
    return (float)(rand() % 1000) / 1000.0f;
}

/// Jack's dust is amber and gold; Henry's sparkles are emerald and cyan.
static void SpawnTrail(int character, float x, float y)
{
    Particle *pt;
    Boolean henry = character == kCharacterHenry || character == kCharacterHenry2;

    if (gNumParticles >= kMaxParticles)
        return;
    pt = &gParticles[gNumParticles++];
    pt->x = x + (Random01() - 0.5f) * 10;
    pt->y = y - Random01() * 6;
    pt->vx = (Random01() - 0.5f) * 12;
    pt->vy = henry ? -10 - Random01() * 18 : -4 - Random01() * 8;
    pt->maxLife = pt->life = 0.5f + Random01() * 0.5f;
    if (henry) {
        Boolean cyan = Random01() < 0.5f;
        pt->r = cyan ? 0.45f : 0.30f;
        pt->g = 1.0f;
        pt->b = cyan ? 1.0f : 0.60f;
    } else {
        Boolean gold = Random01() < 0.5f;
        pt->r = 1.0f;
        pt->g = gold ? 0.85f : 0.65f;
        pt->b = gold ? 0.35f : 0.20f;
    }
}

static void DrawParticles(SpriteWorldPtr w, float dt)
{
    int i;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glScalef(w->designScaleH * w->zoomH, w->designScaleV * w->zoomV, 1.0f);
    glTranslatef(-w->spriteOffsetH, -w->spriteOffsetV, 0);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBegin(GL_QUADS);
    for (i = 0; i < gNumParticles; ) {
        Particle *pt = &gParticles[i];
        float a, size;
        pt->life -= dt;
        if (pt->life <= 0) {
            *pt = gParticles[--gNumParticles];
            continue;
        }
        pt->x += pt->vx * dt;
        pt->y += pt->vy * dt;
        a = pt->life / pt->maxLife;
        size = 1.0f + a;
        glColor4f(pt->r, pt->g, pt->b, a * 0.8f);
        glVertex2f(pt->x - size, pt->y - size);
        glVertex2f(pt->x + size, pt->y - size);
        glVertex2f(pt->x + size, pt->y + size);
        glVertex2f(pt->x - size, pt->y + size);
        i++;
    }
    glEnd();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

/// A name over a player's head, in world coordinates.
static void DrawNameTag(SpriteWorldPtr w, const char *name, float centerX, float bottomY, const float *rgb, float alpha)
{
    Split *s = gSplit;
    float width;

    if (s->font == NULL)
        return;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glScalef(w->designScaleH * w->zoomH, w->designScaleV * w->zoomV, 1.0f);
    glTranslatef(-w->spriteOffsetH, -w->spriteOffsetV, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    MMFontFaceSize(s->font, 20);
    width = MMFontRenderWidth(s->font, name) * 0.5f;
    glColor4f(0, 0, 0, alpha * 0.8f);
    MMFontRender(s->font, name, centerX - width / 2 + 0.7f, bottomY + 0.7f, 0.5f);
    glColor4f(rgb[0], rgb[1], rgb[2], alpha);
    MMFontRender(s->font, name, centerX - width / 2, bottomY, 0.5f);
    glDisable(GL_BLEND);
    glPopMatrix();
}

/// Drawn just before the hero's layer (so under your own player, signs, tips and the
/// overlays): the other players who are in this room.
static void DrawOtherPlayersBeforeHero(SpriteWorldPtr w, SpriteLayerPtr layerP)
{
    (void)layerP;
    SplitDrawOtherPlayers(w);
}

void SplitDrawOtherPlayers(SpriteWorldPtr w)
{
    SplitOtherPlayer others[kMaxPlayers];
    SpriteWorldPtr saveWorld = gSWCurrentSpriteWorld;
    Uint32 now = SDL_GetTicks();
    float dt = gLastParticleTicks == 0 ? 0 : (float)(now - gLastParticleTicks) / 1000.0f;
    Boolean race;
    int n, i;

    if (!SplitScreenActive() || w != gSpriteWorldP || gSuppressExtrasOverlay ||
        (gCurrentGameMode != kPlayingGameMode && gCurrentGameMode != kReadingSignMode))
        return;
    race = gSplit->mode == kSplitModeRace;
    if (dt > 0.1f)
        dt = 0.1f;
    gLastParticleTicks = now;
    n = SplitGetOtherPlayers(others, kMaxPlayers);
    gSWCurrentSpriteWorld = w;
    for (i = 0; i < n; i++) {
        SplitOtherPlayer *o = &others[i];
        const float *rgb = GetCharacterColor(GetPlayerCharacter(o->player));
        SpritePtr puppetP;
        float width;

        if (o->room != gCurrentLevel || !o->visible || (puppetP = PuppetFor(o->player)) == NULL)
            continue;
        if (o->frame >= 0 && o->frame < puppetP->maxFrames)
            SWSetCurrentFrameIndex(puppetP, o->frame);
        puppetP->drawData->flippedHorizontal = o->flipped;
        if (o->scaleW > 0 && o->scaleH > 0) {
            puppetP->drawData->scaledWidthFactor = o->scaleW;
            puppetP->drawData->scaledHeightFactor = o->scaleH;
        }
        if (race) {
            SWSetSpriteTranslucencyLevel(puppetP, kPhantomAlpha);
            SWSetSpriteColor(puppetP, (unsigned short)(0xffff * (0.55f + 0.45f * rgb[0])),
                             (unsigned short)(0xffff * (0.55f + 0.45f * rgb[1])),
                             (unsigned short)(0xffff * (0.55f + 0.45f * rgb[2])));
        } else {
            SWSetSpriteTranslucencyLevel(puppetP, 0xffff);
            SWSetSpriteColor(puppetP, 0xffff, 0xffff, 0xffff);
        }
        puppetP->frameDrawProc(puppetP, puppetP->curFrameP, o->x, o->y);
        width = SWGetSpriteWidth(puppetP);
        if (race) {
            float moved = fabsf(o->x - gLastSeenX[o->player]) + fabsf(o->y - gLastSeenY[o->player]);
            if (moved > 0.5f && moved < 40)
                SpawnTrail(GetPlayerCharacter(o->player), o->x + width / 2, o->y + SWGetSpriteHeight(puppetP) - 3);
        }
        gLastSeenX[o->player] = o->x;
        gLastSeenY[o->player] = o->y;
        DrawNameTag(w, GetPlayerDisplayName(o->player), o->x + width / 2, o->y + 6, rgb, race ? 0.7f : 0.95f);
    }
    if (gNumParticles > 0)
        DrawParticles(w, dt);
    glColor4f(1, 1, 1, 1);
    gSWCurrentSpriteWorld = saveWorld;
}
