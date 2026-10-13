/*
 * MMCoroutine.c — minimal stackful coroutines, added by this version (see MMCoroutine.h).
 *
 * The context switch saves the callee-saved registers on the old stack, stores the stack
 * pointer and loads the other one: x86-64 (System V: Linux and macOS), ARM64 (AAPCS64:
 * Linux and Apple Silicon) and the PS3's PPC64 (64-bit ELF v1). ucontext isn't used because
 * it doesn't work on Apple Silicon.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#if defined(MM_PS3)
#include <malloc.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif
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

#elif defined(__powerpc64__) && defined(_CALL_ELF) && _CALL_ELF == 1
/*  PPC64, 64-bit ELF v1 (the PS3 with PSL1GHT). Frame on a suspended stack (624 bytes, from
    the stack pointer up): back chain, then r14-r31 at 112, f14-f31 at 256, v20-v31 at 400,
    the return address at 592, r2 (TOC) at 600, CR at 608 and VRSAVE at 612. A new coroutine
    "returns" into mm_coro_entry with its argument in r14 and its procedure's function
    descriptor in r15. Functions are called through descriptors (.opd): code address, TOC. */
#define MM_PPC_SAVE_GPR(n, off) "    std " #n "," #off "(1)\n"
#define MM_PPC_LOAD_GPR(n, off) "    ld " #n "," #off "(1)\n"
#define MM_PPC_SAVE_FPR(n, off) "    stfd " #n "," #off "(1)\n"
#define MM_PPC_LOAD_FPR(n, off) "    lfd " #n "," #off "(1)\n"
#define MM_PPC_SAVE_VR(n, off)  "    li 0," #off "\n    stvx " #n ",1,0\n"
#define MM_PPC_LOAD_VR(n, off)  "    li 0," #off "\n    lvx " #n ",1,0\n"
#define MM_PPC_REGS(GPR, FPR, VR) \
    GPR(14, 112) GPR(15, 120) GPR(16, 128) GPR(17, 136) GPR(18, 144) GPR(19, 152) \
    GPR(20, 160) GPR(21, 168) GPR(22, 176) GPR(23, 184) GPR(24, 192) GPR(25, 200) \
    GPR(26, 208) GPR(27, 216) GPR(28, 224) GPR(29, 232) GPR(30, 240) GPR(31, 248) \
    FPR(14, 256) FPR(15, 264) FPR(16, 272) FPR(17, 280) FPR(18, 288) FPR(19, 296) \
    FPR(20, 304) FPR(21, 312) FPR(22, 320) FPR(23, 328) FPR(24, 336) FPR(25, 344) \
    FPR(26, 352) FPR(27, 360) FPR(28, 368) FPR(29, 376) FPR(30, 384) FPR(31, 392) \
    VR(20, 400) VR(21, 416) VR(22, 432) VR(23, 448) VR(24, 464) VR(25, 480) \
    VR(26, 496) VR(27, 512) VR(28, 528) VR(29, 544) VR(30, 560) VR(31, 576)
#define kPPCFrame 624
__asm__(
    "    .section \".opd\",\"aw\"\n"
    "    .align 3\n"
    "    .globl mm_coro_switch\n"
    "mm_coro_switch:\n"
    "    .quad .L.mm_coro_switch,.TOC.@tocbase,0\n"
    "    .globl mm_coro_entry\n"
    "mm_coro_entry:\n"
    "    .quad .L.mm_coro_entry,.TOC.@tocbase,0\n"
    "    .text\n"
    "    .align 2\n"
    ".L.mm_coro_switch:\n"
    "    stdu 1,-624(1)\n"
    "    mflr 0\n"
    "    std 0,592(1)\n"
    "    std 2,600(1)\n"
    "    mfcr 0\n"
    "    stw 0,608(1)\n"
    "    mfspr 0,256\n"
    "    stw 0,612(1)\n"
    MM_PPC_REGS(MM_PPC_SAVE_GPR, MM_PPC_SAVE_FPR, MM_PPC_SAVE_VR)
    "    std 1,0(3)\n"
    "    mr 1,4\n"
    MM_PPC_REGS(MM_PPC_LOAD_GPR, MM_PPC_LOAD_FPR, MM_PPC_LOAD_VR)
    "    lwz 0,612(1)\n"
    "    mtspr 256,0\n"
    "    lwz 0,608(1)\n"
    "    mtcrf 0xff,0\n"
    "    ld 2,600(1)\n"
    "    ld 0,592(1)\n"
    "    mtlr 0\n"
    "    addi 1,1,624\n"
    "    blr\n"
    ".L.mm_coro_entry:\n"
    "    stdu 1,-128(1)\n"
    "    li 0,0\n"
    "    std 0,0(1)\n"              // the end of the back chain
    "    mr 3,14\n"
    "    ld 0,0(15)\n"
    "    ld 2,8(15)\n"
    "    mtctr 0\n"
    "    bctrl\n"
    "    trap\n"
);

static void *InitialStack(MMCoroutine *co, unsigned char *top)
{
    uint64_t *sp = (uint64_t *)((((uintptr_t)top - 256) & ~(uintptr_t)15) - kPPCFrame);
    const uint64_t *entry = (const uint64_t *)(uintptr_t)mm_coro_entry;     // descriptors
    const uint64_t *proc = (const uint64_t *)(uintptr_t)CoroutineMain;

    memset(sp, 0, kPPCFrame);
    sp[112 / 8] = (uint64_t)(uintptr_t)co;                  // r14: argument
    sp[120 / 8] = (uint64_t)(uintptr_t)proc;                // r15: procedure
    sp[592 / 8] = entry[0];                                 // return address
    sp[600 / 8] = proc[1];                                  // TOC
    ((uint32_t *)sp)[612 / 4] = 0xffffffffu;                // VRSAVE: all vector registers live
    return sp;
}

#else
#error "MMCoroutine: unsupported CPU (x86-64, ARM64 and PPC64 only)"
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
#if defined(MM_PS3)
        // no mmap on the PS3, so no guard page either
    MMCoroutine *co = calloc(1, sizeof(MMCoroutine));

    if (co == NULL)
        return NULL;
    co->mapSize = (stackSize + 4095) & ~(size_t)4095;
    co->stack = memalign(128, co->mapSize);
    if (co->stack == NULL) {
        free(co);
        return NULL;
    }
    co->proc = proc;
    co->arg = arg;
    co->sp = InitialStack(co, (unsigned char *)co->stack + co->mapSize);
    return co;
#else
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
#endif
}

void MMCoroutineDispose(MMCoroutine *co)
{
    if (co == NULL || co == gRunning)
        return;
#if defined(MM_PS3)
    free(co->stack);
#else
    munmap(co->stack, co->mapSize);
#endif
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
