/*
 * A test picture for PS3GL (src/ps3/PS3GL.c), to check the PS3 drawing on its own:
 *   make -f platform/ps3/Makefile gltest     -> build/ps3/gltest.self (boot it in RPCS3)
 * Left to right, top row: a red square (untextured), a texture with four colored quarters
 * (red top left, green top right, blue bottom left, white bottom right), the same at half
 * alpha over a blue band, and a glCopyTexSubImage2D copy of the first two squares.
 * Bottom row: a rotated and flipped quad, then a framebuffer-object picture (yellow with a
 * dark square) drawn as a texture. Background: dark gray with a scissored green strip.
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/process.h>
#include "PS3GL.h"

SYS_PROCESS_PARAM(1001, 0x100000)

static void Quad(float x0, float y0, float x1, float y1)
{
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(x0, y0);
    glTexCoord2f(1, 0); glVertex2f(x1, y0);
    glTexCoord2f(1, 1); glVertex2f(x1, y1);
    glTexCoord2f(0, 1); glVertex2f(x0, y1);
    glEnd();
}

int main(void)
{
    static unsigned char pixels[64 * 64 * 4];
    GLuint tex, copy, fboTex, fbo;
    int w, h, x, y, frame;

    if (PS3GLInit() != 0)
        return 1;
    PS3GLGetScreenSize(&w, &h);
    for (y = 0; y < 64; y++)
        for (x = 0; x < 64; x++) {
            unsigned char *p = pixels + (y * 64 + x) * 4;
            int right = x >= 32, bottom = y >= 32;
            p[0] = (!right && !bottom) || (right && bottom) ? 255 : 0;
            p[1] = (right && !bottom) || (right && bottom) ? 255 : 0;
            p[2] = (!right && bottom) || (right && bottom) ? 255 : 0;
            p[3] = 255;
        }
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glGenTextures(1, &copy);
    glBindTexture(GL_TEXTURE_2D, copy);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 400, 200, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glGenTextures(1, &fboTex);
    glBindTexture(GL_TEXTURE_2D, fboTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glGenFramebuffers(1, &fbo);

    for (frame = 0; frame < 100000; frame++) {
            // into the framebuffer object: yellow with a dark square near its top left
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTex, 0);
        glViewport(0, 0, 128, 128);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 128, 128, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClearColor(1, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_TEXTURE_2D);
        glColor4f(0.2f, 0.2f, 0.2f, 1);
        Quad(16, 16, 56, 56);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // the screen, in the game's way: y down, (0,0) at the top left
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, w, h, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_SCISSOR_TEST);
        glClearColor(0.15f, 0.15f, 0.15f, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_SCISSOR_TEST);
        glScissor(0, 40, w, 30);                // 40..70 pixels from the bottom
        glClearColor(0, 0.6f, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glColor4f(1, 0, 0, 1);
        Quad(50, 50, 250, 250);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex);
        glColor4f(1, 1, 1, 1);
        Quad(300, 50, 500, 250);
        glDisable(GL_TEXTURE_2D);
        glColor4f(0, 0, 1, 1);
        Quad(540, 130, 760, 170);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1, 1, 1, 0.5f);
        Quad(550, 50, 750, 250);
        glDisable(GL_BLEND);
        glColor4f(1, 1, 1, 1);
            // copy the red and the textured squares (GL rows count from the bottom)
        glBindTexture(GL_TEXTURE_2D, copy);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 50, h - 250, 400, 200);
        glBegin(GL_QUADS);                      // drawn the way BlitKernelGL draws flipped surfaces
        glTexCoord2f(0, 1); glVertex2f(800, 50);
        glTexCoord2f(1, 1); glVertex2f(1200, 50);
        glTexCoord2f(1, 0); glVertex2f(1200, 250);
        glTexCoord2f(0, 0); glVertex2f(800, 250);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, tex);
        glPushMatrix();
        glTranslatef(150, 400, 0);
        glRotatef(30, 0, 0, 1);
        glRotatef(180, 0, 1, 0);                // mirrored left-right
        Quad(-80, -80, 80, 80);
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, fboTex);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(300, 320);
        glTexCoord2f(1, 1); glVertex2f(500, 320);
        glTexCoord2f(1, 0); glVertex2f(500, 520);
        glTexCoord2f(0, 0); glVertex2f(300, 520);
        glEnd();
        if (glGetError() != GL_NO_ERROR && frame < 3)
            printf("gltest: GL error in frame %d\n", frame);
        PS3GLSwap();
        if (frame % 60 == 0)
            printf("gltest: frame %d\n", frame);
    }
    return 0;
}
