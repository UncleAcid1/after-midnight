/*
 * MMCoroutine.c — minimal stackful coroutines, added by this version (see MMCoroutine.h).
 *
 * The context switch saves the callee-saved registers on the old stack, stores the stack
 * pointer and loads the other one: x86-64 (System V: Linux and macOS) and ARM64 (AAPCS64:
 * Linux and Apple Silicon). ucontext isn't used because it doesn't work on Apple Silicon.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "MMCoroutine.h"

struct MMCoroutine {
    void           *sp;         ///< saved stack pointer while not running
    void           *stack;      ///< mapping (guard page first)
    size_t          mapSize;
    MMCoroutineProc proc;
    void           *arg;
    int             finished;
};

/// What runs now, and where to go back to (one level: coroutines don't resume each other).
static MMCoroutine *gRunning = NULL;
static void *gCallerSP = NULL;

void mm_coro_switch(void **saveSP, void *loadSP);
static void CoroutineMain(void *arg);
void mm_coro_entry(void);

#if defined(__APPLE__)
#define MM_SYM(name) "_" #name
#else
#define MM_SYM(name) #name
#endif

#if defined(__x86_64__)
/*  Frame on a suspended stack (low to high): mxcsr + x87 control word, r15, r14, r13, r12,
    rbx, rbp, return address. A new coroutine "returns" into mm_coro_entry with the entry
    procedure in r13 and its argument in r12. */
__asm__(
    ".text\n"
    ".globl " MM_SYM(mm_coro_switch) "\n"
    MM_SYM(mm_coro_switch) ":\n"
    "    pushq %rbp\n"
    "    pushq %rbx\n"
    "    pushq %r12\n"
    "    pushq %r13\n"
    "    pushq %r14\n"
    "    pushq %r15\n"
    "    subq $8, %rsp\n"
    "    stmxcsr (%rsp)\n"
    "    fnstcw 4(%rsp)\n"
    "    movq %rsp, (%rdi)\n"
    "    movq %rsi, %rsp\n"
    "    ldmxcsr (%rsp)\n"
    "    fldcw 4(%rsp)\n"
    "    addq $8, %rsp\n"
    "    popq %r15\n"
    "    popq %r14\n"
    "    popq %r13\n"
    "    popq %r12\n"
    "    popq %rbx\n"
    "    popq %rbp\n"
    "    ret\n"
    ".globl " MM_SYM(mm_coro_entry) "\n"
    MM_SYM(mm_coro_entry) ":\n"
    "    movq %r12, %rdi\n"
    "    callq *%r13\n"
    "    ud2\n"
);

static void *InitialStack(MMCoroutine *co, unsigned char *top)
{
    uint64_t *sp = (uint64_t *)((uintptr_t)top & ~(uintptr_t)15);
    uint32_t controls[2] = { 0x1f80, 0x037f };

    *--sp = (uint64_t)(uintptr_t)mm_coro_entry;     // popped by ret: the stack is then 16-aligned
    *--sp = 0;                              // rbp
    *--sp = 0;                              // rbx
    *--sp = (uint64_t)(uintptr_t)co;        // r12: argument
    *--sp = (uint64_t)(uintptr_t)CoroutineMain;     // r13: procedure
    *--sp = 0;                              // r14
    *--sp = 0;                              // r15
    --sp;
    memcpy(sp, controls, sizeof(controls));
    return sp;
}

#elif defined(__aarch64__) || defined(__arm64__)
/*  Frame on a suspended stack: x19-x28, x29, x30 (return address), d8-d15 (176 bytes). A
    new coroutine "returns" into mm_coro_entry with its argument in x19 and procedure in x20. */
__asm__(
    ".text\n"
    ".globl " MM_SYM(mm_coro_switch) "\n"
    ".p2align 2\n"
    MM_SYM(mm_coro_switch) ":\n"
    "    sub sp, sp, #176\n"
    "    stp x19, x20, [sp, #0]\n"
    "    stp x21, x22, [sp, #16]\n"
    "    stp x23, x24, [sp, #32]\n"
    "    stp x25, x26, [sp, #48]\n"
    "    stp x27, x28, [sp, #64]\n"
    "    stp x29, x30, [sp, #80]\n"
    "    stp d8, d9, [sp, #96]\n"
    "    stp d10, d11, [sp, #112]\n"
    "    stp d12, d13, [sp, #128]\n"
    "    stp d14, d15, [sp, #144]\n"
    "    mov x2, sp\n"
    "    str x2, [x0]\n"
    "    mov sp, x1\n"
    "    ldp x19, x20, [sp, #0]\n"
    "    ldp x21, x22, [sp, #16]\n"
    "    ldp x23, x24, [sp, #32]\n"
    "    ldp x25, x26, [sp, #48]\n"
    "    ldp x27, x28, [sp, #64]\n"
    "    ldp x29, x30, [sp, #80]\n"
    "    ldp d8, d9, [sp, #96]\n"
    "    ldp d10, d11, [sp, #112]\n"
    "    ldp d12, d13, [sp, #128]\n"
    "    ldp d14, d15, [sp, #144]\n"
    "    add sp, sp, #176\n"
    "    ret\n"
    ".globl " MM_SYM(mm_coro_entry) "\n"
    ".p2align 2\n"
    MM_SYM(mm_coro_entry) ":\n"
    "    mov x0, x19\n"
    "    blr x20\n"
    "    brk #0\n"
);

static void *InitialStack(MMCoroutine *co, unsigned char *top)
{
    uint64_t *sp = (uint64_t *)(((uintptr_t)top & ~(uintptr_t)15) - 176);

    memset(sp, 0, 176);
    sp[0] = (uint64_t)(uintptr_t)co;                // x19: argument
    sp[1] = (uint64_t)(uintptr_t)CoroutineMain;     // x20: procedure
    sp[11] = (uint64_t)(uintptr_t)mm_coro_entry;    // x30
    return sp;
}

#else
#error "MMCoroutine: unsupported CPU (x86-64 and ARM64 only)"
#endif

/// Every coroutine starts here (on its own stack).
static void CoroutineMain(void *arg)
{
    MMCoroutine *co = (MMCoroutine *)arg;

    co->proc(co->arg);
    co->finished = 1;
    gRunning = NULL;
    mm_coro_switch(&co->sp, gCallerSP);     // never comes back
    abort();
}

MMCoroutine *MMCoroutineCreate(MMCoroutineProc proc, void *arg, size_t stackSize)
{
    MMCoroutine *co = calloc(1, sizeof(MMCoroutine));
    size_t page = (size_t)sysconf(_SC_PAGESIZE);
    void *mem;

    if (co == NULL)
        return NULL;
    stackSize = (stackSize + page - 1) / page * page;
    co->mapSize = stackSize + page;
    mem = mmap(NULL, co->mapSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (mem == MAP_FAILED) {
        free(co);
        return NULL;
    }
    mprotect(mem, page, PROT_NONE);         // guard page: an overflow faults instead of corrupting
    co->stack = mem;
    co->proc = proc;
    co->arg = arg;
    co->sp = InitialStack(co, (unsigned char *)mem + co->mapSize);
    return co;
}

void MMCoroutineDispose(MMCoroutine *co)
{
    if (co == NULL || co == gRunning)
        return;
    munmap(co->stack, co->mapSize);
    free(co);
}

void MMCoroutineResume(MMCoroutine *co)
{
    if (co == NULL || co->finished || gRunning != NULL)
        return;
    gRunning = co;
    mm_coro_switch(&gCallerSP, co->sp);
    gRunning = NULL;
}

void MMCoroutineYield(void)
{
    MMCoroutine *co = gRunning;

    if (co == NULL)
        return;
    gRunning = NULL;
    mm_coro_switch(&co->sp, gCallerSP);
    gRunning = co;
}

MMCoroutine *MMCoroutineCurrent(void)
{
    return gRunning;
}

int MMCoroutineIsFinished(const MMCoroutine *co)
{
    return co == NULL || co->finished;
}

/// For the split screen state swap: these must survive switching between players' globals.
void MMCoroutineGetState(void **addrP, size_t *sizeP, void **addr2P, size_t *size2P)
{
    *addrP = &gRunning;
    *sizeP = sizeof(gRunning);
    *addr2P = &gCallerSP;
    *size2P = sizeof(gCallerSP);
}
