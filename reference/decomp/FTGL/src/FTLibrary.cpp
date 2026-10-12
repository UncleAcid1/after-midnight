// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTLibrary.cpp

// ---- __ZN9FTLibraryD1Ev @ 0x417c0 (60 bytes)

/* FTLibrary::~FTLibrary() */

void __thiscall FTLibrary::~FTLibrary(FTLibrary *this)

{
  if (*(int *)this != 0) {
    _FT_Done_FreeType(**(undefined4 **)this);
    operator_delete(*(void **)this);
    *(undefined4 *)this = 0;
  }
  return;
}


// ---- ___tcf_0 @ 0x417fc (42 bytes)

void __tcf_0(void)

{
  FTLibrary::~FTLibrary((FTLibrary *)PTR_ftlib_001c4a64);
  return;
}


// ---- __ZN9FTLibraryD2Ev @ 0x41826 (60 bytes)

/* FTLibrary::~FTLibrary() */

void __thiscall FTLibrary::~FTLibrary(FTLibrary *this)

{
  if (*(int *)this != 0) {
    _FT_Done_FreeType(**(undefined4 **)this);
    operator_delete(*(void **)this);
    *(undefined4 *)this = 0;
  }
  return;
}


// ---- __ZN9FTLibrary10InitialiseEv @ 0x41862 (124 bytes)

/* FTLibrary::Initialise() */

undefined4 __thiscall FTLibrary::Initialise(FTLibrary *this)

{
  void *pvVar1;
  undefined4 uVar2;
  undefined4 local_10;
  
  if (*(int *)this == 0) {
    pvVar1 = operator_new(4);
    *(void **)this = pvVar1;
    uVar2 = _FT_Init_FreeType(*(undefined4 *)this);
    *(undefined4 *)(this + 4) = uVar2;
    if (*(int *)(this + 4) == 0) {
      local_10 = 1;
    }
    else {
      operator_delete(*(void **)this);
      *(undefined4 *)this = 0;
      local_10 = 0;
    }
  }
  else {
    local_10 = 1;
  }
  return local_10;
}


// ---- __ZN9FTLibraryC1Ev @ 0x418de (44 bytes)

/* FTLibrary::FTLibrary() */

void __thiscall FTLibrary::FTLibrary(FTLibrary *this)

{
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = 0;
  Initialise(this);
  return;
}


// ---- __ZN9FTLibrary8InstanceEv @ 0x4190a (204 bytes)

/* FTLibrary::Instance() */

undefined * FTLibrary::Instance(void)

{
  int iVar1;
  
  if (*PTR_ftlib_001c4a60 == '\0') {
    iVar1 = ___cxa_guard_acquire(PTR_ftlib_001c4a60);
    if (iVar1 != 0) {
      FTLibrary((FTLibrary *)PTR_ftlib_001c4a64);
      ___cxa_guard_release(PTR_ftlib_001c4a60);
      ___cxa_atexit(PTR___tcf_0_001c4a68,0,PTR_MACH_HEADER_001c400c);
    }
  }
  return PTR_ftlib_001c4a64;
}


// ---- __ZN9FTLibraryC2Ev @ 0x419d6 (44 bytes)

/* FTLibrary::FTLibrary() */

void __thiscall FTLibrary::FTLibrary(FTLibrary *this)

{
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = 0;
  Initialise(this);
  return;
}


