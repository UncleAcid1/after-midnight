/*
 * MMCoroutine.h — minimal stackful coroutines (x86-64 and ARM64; Linux and macOS), added by
 * this version for split screen: each player's game runs on its own stack, so the game's
 * own loops (fades, room slides, the map, signs) can hand control back after every frame.
 */
#ifndef MMCOROUTINE_H
#define MMCOROUTINE_H

#include <stddef.h>

typedef struct MMCoroutine MMCoroutine;
typedef void (*MMCoroutineProc)(void *arg);

/// A coroutine that will run proc(arg) on a stack of stackSize bytes when first resumed.
/// When proc returns the coroutine is finished and hands control back for good.
MMCoroutine *MMCoroutineCreate(MMCoroutineProc proc, void *arg, size_t stackSize);
void MMCoroutineDispose(MMCoroutine *co);
/// Runs the coroutine until it yields or finishes. Call from outside any coroutine.
void MMCoroutineResume(MMCoroutine *co);
/// From inside a coroutine: back to whoever resumed it.
void MMCoroutineYield(void);
/// The running coroutine, or NULL outside one.
MMCoroutine *MMCoroutineCurrent(void);
int  MMCoroutineIsFinished(const MMCoroutine *co);

#endif
