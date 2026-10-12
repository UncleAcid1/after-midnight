/*
 * MMChat.c — chatting with the player on the other computer, added by this version.
 *
 * In a game on two computers (online or on the local network) T opens a line at the bottom
 * of the play area; Return sends it, Escape closes it. While typing, the keys go to the line
 * and not to the game. The last few lines show for a while over the play area.
 *
 * Every line, sent or received, goes into "Chat Log.txt" next to the preferences, with the
 * date and time, under a heading for each game.
 */

#include <stdarg.h>
#include <time.h>
#include "MMGame.h"
#include "MMProtos.h"
#include "MMFont.h"

#define kChatLines      6
#define kChatShowMs     9000
#define kChatFadeMs     1500

typedef struct ChatLine {
    char    name[64];
    char    text[kChatMaxLength + 1];
    int     player;         ///< who said it (their colour)
    Uint32  when;
} ChatLine;

static struct {
    ChatLine    lines[kChatLines];
    int         count;
    Boolean     typing;
    char        draft[kChatMaxLength + 1];
    Uint32      openedAt;
    MMFont     *font;
} gChat;

static void CloseLine(void);

// ---------------------------------------------------------------------------
// The log

static void LogLine(const char *format, ...)
{
    FILE *file = fopen(GetAppSupportPath("Chat Log.txt"), "a");
    char stamp[40];
    time_t now = time(NULL);
    va_list args;

    if (file == NULL)
        return;
    strftime(stamp, sizeof(stamp), "%Y-%m-%d %H:%M", localtime(&now));
    fprintf(file, "[%s] ", stamp);
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);
    fputc('\n', file);
    fclose(file);
}

/// Connected to the other players: a heading in the log, and a fresh chat.
void ChatNoteGameStarting(char names[][64], int numPlayers, int localPlayer, Boolean online)
{
    char who[400];
    size_t n = 0;
    int p, k = 0;

    for (p = 0; p < numPlayers; p++) {
        if (p == localPlayer)
            continue;
        n += (size_t)snprintf(who + n, sizeof(who) > n ? sizeof(who) - n : 0, "%s%s",
                              k == 0 ? "" : k == numPlayers - 2 ? " and " : ", ", names[p]);
        k++;
    }
    LogLine("--- %s with %s, %s ---", localPlayer >= 0 && localPlayer < numPlayers ? names[localPlayer] : "?",
            k ? who : "nobody", online ? "online" : "on the local network");
    gChat.count = 0;
    if (gChat.typing)
        CloseLine();
}

static void AddLine(const char *name, const char *text, int player)
{
    ChatLine *line;

    if (gChat.count == kChatLines) {
        memmove(&gChat.lines[0], &gChat.lines[1], sizeof(ChatLine) * (kChatLines - 1));
        gChat.count--;
    }
    line = &gChat.lines[gChat.count++];
    snprintf(line->name, sizeof(line->name), "%s", name);
    snprintf(line->text, sizeof(line->text), "%s", text);
    line->player = player;
    line->when = SDL_GetTicks();
    LogLine("%s: %s", name, text);
}

/// A line from the other computer.
void ChatReceived(const char *name, const char *text, int player)
{
    char clean[kChatMaxLength + 1];
    int i, n = 0;

    for (i = 0; text[i] && n < kChatMaxLength; i++)         // no control characters
        if ((unsigned char)text[i] >= 0x20)
            clean[n++] = text[i];
    clean[n] = 0;
    if (n == 0)
        return;
    AddLine(name, clean, player);
    MMPlaySound(0x0b, 8, 1);
}

// ---------------------------------------------------------------------------
// Typing

/// There's someone to talk to: a game on two computers is being played.
Boolean ChatAvailable(void)
{
    return NetIsConnected() && (NetGameActive() || NetRaceActive());
}

Boolean ChatIsTyping(void)
{
    return gChat.typing;
}

static void CloseLine(void)
{
    gChat.typing = false;
    gChat.draft[0] = 0;
    SDL_StopTextInput();
}

static void Send(void)
{
    char me[256];

    if (gChat.draft[0] != 0) {
        SWCopyPascalStringToCString((ConstStringPtr)gCurrentPlayerName, me);
        NetSendChat(gChat.draft);
        AddLine(me, gChat.draft, NetLocalPlayer());
    }
    CloseLine();
}

/// Every event goes here first while playing: T opens the line, and while it's open the
/// keys are the line's. Returns true if the event was used.
Boolean ChatHandleEvent(const SDL_Event *event)
{
    size_t len;

    if (!gChat.typing) {
        if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_t && !event->key.repeat &&
            !(event->key.keysym.mod & (KMOD_GUI | KMOD_CTRL | KMOD_ALT)) && ChatAvailable() &&
            (gCurrentGameMode == kPlayingGameMode || gCurrentGameMode == kReadingSignMode) && !gGameIsPaused) {
            gChat.typing = true;
            gChat.draft[0] = 0;
            gChat.openedAt = SDL_GetTicks();
            ResetKeys();
            SDL_StartTextInput();
            return true;
        }
        return false;
    }
    if (!ChatAvailable()) {
        CloseLine();
        return false;
    }
    switch (event->type) {
        case SDL_TEXTINPUT:
                // the T that opened the line arrives as text too
            if (SDL_GetTicks() - gChat.openedAt < 60 && (event->text.text[0] == 't' || event->text.text[0] == 'T') &&
                gChat.draft[0] == 0)
                return true;
            len = strlen(gChat.draft);
            snprintf(gChat.draft + len, sizeof(gChat.draft) - len, "%s", event->text.text);
            return true;
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case SDLK_RETURN: case SDLK_KP_ENTER:
                    Send();
                    break;
                case SDLK_ESCAPE:
                    CloseLine();
                    break;
                case SDLK_BACKSPACE:
                    len = strlen(gChat.draft);
                    while (len > 0 && (gChat.draft[len - 1] & 0xc0) == 0x80)     // a whole UTF-8 character
                        len--;
                    if (len > 0)
                        len--;
                    gChat.draft[len] = 0;
                    break;
                default:
                    break;
            }
            return true;
        case SDL_KEYUP:
            return true;
        default:
            return false;
    }
}

// ---------------------------------------------------------------------------
// Drawing

static float LineAlpha(const ChatLine *line, Uint32 now)
{
    Uint32 age = now - line->when;
    if (gChat.typing || age < kChatShowMs)
        return 1.0f;
    if (age > kChatShowMs + kChatFadeMs)
        return 0.0f;
    return 1.0f - (float)(age - kChatShowMs) / (float)kChatFadeMs;
}

static void Shadowed(const char *text, float x, float y, float unit, float r, float g, float b, float a)
{
    glColor4f(0, 0, 0, a * 0.85f);
    MMFontRender(gChat.font, text, x + 1.2f * unit, y + 1.2f * unit, unit);
    glColor4f(r, g, b, a);
    MMFontRender(gChat.font, text, x, y, unit);
}

/// The chat over the play area: lines up from (x, bottom), in the current coordinates, where
/// one unit is pixelsPerUnit window pixels.
void ChatDraw(float x, float bottom, float width, float size, float pixelsPerUnit)
{
    Uint32 now = SDL_GetTicks();
    int face = (int)(size * pixelsPerUnit + 0.5f), i;
    float unit, y = bottom, a, nameWidth;
    char text[kChatMaxLength + 80];
    const float *rgb;

    if (gChat.count == 0 && !gChat.typing)
        return;
    if (gChat.font == NULL)
        gChat.font = MMFontLoad(GetFontsFolderPath("Action Man.ttf"));
    if (gChat.font == NULL)
        return;
    if (face < 8)
        face = 8;
    unit = size / (float)face;
    MMFontFaceSize(gChat.font, face);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (gChat.typing) {
        Boolean caret = (now / 450) % 2 == 0;
        glDisable(GL_TEXTURE_2D);
        glColor4f(0, 0, 0, 0.6f);
        glBegin(GL_QUADS);
        glVertex2f(x - 4, y - size * 1.15f);
        glVertex2f(x + width, y - size * 1.15f);
        glVertex2f(x + width, y + size * 0.35f);
        glVertex2f(x - 4, y + size * 0.35f);
        glEnd();
        glEnable(GL_TEXTURE_2D);
        snprintf(text, sizeof(text), "Say: %s%s", gChat.draft, caret ? "_" : "");
        Shadowed(text, x, y, unit, 1.0f, 1.0f, 0.6f, 1.0f);
        y -= size * 1.5f;
    }
    for (i = gChat.count - 1; i >= 0; i--) {
        ChatLine *line = &gChat.lines[i];
        a = LineAlpha(line, now);
        if (a <= 0)
            continue;
        rgb = GetCharacterColor(GetPlayerCharacter(line->player));
        snprintf(text, sizeof(text), "%s:", line->name);
        Shadowed(text, x, y, unit, rgb[0], rgb[1], rgb[2], a);
        nameWidth = MMFontRenderWidth(gChat.font, text) * unit;
        Shadowed(line->text, x + nameWidth + size * 0.4f, y, unit, 0.95f, 0.95f, 0.92f, a);
        y -= size * 1.25f;
    }
    glColor4f(1, 1, 1, 1);
}

/// The chat in the game's own play area (relay co-op on two computers).
void ChatDrawInWorld(SpriteWorldPtr w)
{
    float height = (float)(w->visScrollRect.bottom - w->visScrollRect.top);
    float width = (float)(w->visScrollRect.right - w->visScrollRect.left);

    if (!ChatAvailable() && gChat.count == 0)
        return;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glScalef(w->designScaleH, w->designScaleV, 1.0f);
    ChatDraw(10, height - 30, width * 0.7f, 13, w->designScaleV);
    glPopMatrix();
}

/// Split screen: the chat isn't a player's.
void ChatShareState(void (*share)(void *, size_t))
{
    share(&gChat, sizeof(gChat));
}

/// A hint, once, that T chats.
const char *ChatHint(void)
{
    return ChatAvailable() && !gChat.typing ? "T to chat" : NULL;
}
