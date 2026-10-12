// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTFace.cpp

// ---- __ZN6FTFaceC2EPKc @ 0x3c612 (290 bytes)

/* FTFace::FTFace(char const*) */

void __thiscall FTFace::FTFace(FTFace *this,char *param_1)

{
  void *pvVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  
  *(undefined **)this = PTR_vtable_001c4a30 + 8;
  FTSize::FTSize((FTSize *)(this + 8));
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  pvVar1 = operator_new(4);
  *(void **)(this + 4) = pvVar1;
  uVar3 = *(undefined4 *)(this + 4);
  FTLibrary::Instance();
  puVar2 = (undefined4 *)FTLibrary::GetLibrary();
  uVar3 = _FT_New_Face(*puVar2,param_1,0,uVar3);
  *(undefined4 *)(this + 0x30) = uVar3;
  if (*(int *)(this + 0x30) == 0) {
    *(undefined4 *)(this + 0x24) = *(undefined4 *)(**(int **)(this + 4) + 0x10);
    this[0x2c] = (FTFace)((byte)(*(uint *)(**(int **)(this + 4) + 8) >> 6) & 1);
  }
  else {
    operator_delete(*(void **)(this + 4));
    *(undefined4 *)(this + 4) = 0;
  }
  return;
}


// ---- __ZN6FTFaceC1EPKc @ 0x3c734 (290 bytes)

/* FTFace::FTFace(char const*) */

void __thiscall FTFace::FTFace(FTFace *this,char *param_1)

{
  void *pvVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  
  *(undefined **)this = PTR_vtable_001c4a30 + 8;
  FTSize::FTSize((FTSize *)(this + 8));
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  pvVar1 = operator_new(4);
  *(void **)(this + 4) = pvVar1;
  uVar3 = *(undefined4 *)(this + 4);
  FTLibrary::Instance();
  puVar2 = (undefined4 *)FTLibrary::GetLibrary();
  uVar3 = _FT_New_Face(*puVar2,param_1,0,uVar3);
  *(undefined4 *)(this + 0x30) = uVar3;
  if (*(int *)(this + 0x30) == 0) {
    *(undefined4 *)(this + 0x24) = *(undefined4 *)(**(int **)(this + 4) + 0x10);
    this[0x2c] = (FTFace)((byte)(*(uint *)(**(int **)(this + 4) + 8) >> 6) & 1);
  }
  else {
    operator_delete(*(void **)(this + 4));
    *(undefined4 *)(this + 4) = 0;
  }
  return;
}


// ---- __ZN6FTFaceC2EPKhm @ 0x3c856 (268 bytes)

/* FTFace::FTFace(unsigned char const*, unsigned long) */

void __thiscall FTFace::FTFace(FTFace *this,uchar *param_1,ulong param_2)

{
  void *pvVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  
  *(undefined **)this = PTR_vtable_001c4a30 + 8;
  FTSize::FTSize((FTSize *)(this + 8));
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  pvVar1 = operator_new(4);
  *(void **)(this + 4) = pvVar1;
  uVar3 = *(undefined4 *)(this + 4);
  FTLibrary::Instance();
  puVar2 = (undefined4 *)FTLibrary::GetLibrary();
  uVar3 = _FT_New_Memory_Face(*puVar2,param_1,param_2,0,uVar3);
  *(undefined4 *)(this + 0x30) = uVar3;
  if (*(int *)(this + 0x30) == 0) {
    *(undefined4 *)(this + 0x24) = *(undefined4 *)(**(int **)(this + 4) + 0x10);
  }
  else {
    operator_delete(*(void **)(this + 4));
    *(undefined4 *)(this + 4) = 0;
  }
  return;
}


// ---- __ZN6FTFaceC1EPKhm @ 0x3c962 (268 bytes)

/* FTFace::FTFace(unsigned char const*, unsigned long) */

void __thiscall FTFace::FTFace(FTFace *this,uchar *param_1,ulong param_2)

{
  void *pvVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  
  *(undefined **)this = PTR_vtable_001c4a30 + 8;
  FTSize::FTSize((FTSize *)(this + 8));
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  pvVar1 = operator_new(4);
  *(void **)(this + 4) = pvVar1;
  uVar3 = *(undefined4 *)(this + 4);
  FTLibrary::Instance();
  puVar2 = (undefined4 *)FTLibrary::GetLibrary();
  uVar3 = _FT_New_Memory_Face(*puVar2,param_1,param_2,0,uVar3);
  *(undefined4 *)(this + 0x30) = uVar3;
  if (*(int *)(this + 0x30) == 0) {
    *(undefined4 *)(this + 0x24) = *(undefined4 *)(**(int **)(this + 4) + 0x10);
  }
  else {
    operator_delete(*(void **)(this + 4));
    *(undefined4 *)(this + 4) = 0;
  }
  return;
}


// ---- __ZN6FTFaceD2Ev @ 0x3ca6e (176 bytes)

/* WARNING: Removing unreachable block (ram,0x0003cae1) */
/* FTFace::~FTFace() */

void __thiscall FTFace::~FTFace(FTFace *this)

{
  *(undefined **)this = PTR_vtable_001c4a30 + 8;
  if (*(int *)(this + 4) != 0) {
    _FT_Done_Face(**(undefined4 **)(this + 4));
    operator_delete(*(void **)(this + 4));
    *(undefined4 *)(this + 4) = 0;
  }
  FTSize::~FTSize((FTSize *)(this + 8));
  return;
}


// ---- __ZN6FTFaceD1Ev @ 0x3cb1e (176 bytes)

/* WARNING: Removing unreachable block (ram,0x0003cb91) */
/* FTFace::~FTFace() */

void __thiscall FTFace::~FTFace(FTFace *this)

{
  *(undefined **)this = PTR_vtable_001c4a30 + 8;
  if (*(int *)(this + 4) != 0) {
    _FT_Done_Face(**(undefined4 **)(this + 4));
    operator_delete(*(void **)(this + 4));
    *(undefined4 *)(this + 4) = 0;
  }
  FTSize::~FTSize((FTSize *)(this + 8));
  return;
}


// ---- __ZN6FTFaceD0Ev @ 0x3cbce (176 bytes)

/* FTFace::~FTFace() */

void __thiscall FTFace::~FTFace(FTFace *this)

{
  *(undefined **)this = PTR_vtable_001c4a30 + 8;
  if (*(int *)(this + 4) != 0) {
    _FT_Done_Face(**(undefined4 **)(this + 4));
    operator_delete(*(void **)(this + 4));
    *(undefined4 *)(this + 4) = 0;
  }
  FTSize::~FTSize((FTSize *)(this + 8));
  operator_delete(this);
  return;
}


// ---- __ZN6FTFace6AttachEPKc @ 0x3cc7e (60 bytes)

/* FTFace::Attach(char const*) */

bool __thiscall FTFace::Attach(FTFace *this,char *param_1)

{
  undefined4 uVar1;
  
  uVar1 = _FT_Attach_File(**(undefined4 **)(this + 4),param_1);
  *(undefined4 *)(this + 0x30) = uVar1;
  return *(int *)(this + 0x30) == 0;
}


// ---- __ZN6FTFace6AttachEPKhm @ 0x3ccba (78 bytes)

/* FTFace::Attach(unsigned char const*, unsigned long) */

bool __thiscall FTFace::Attach(FTFace *this,uchar *param_1,ulong param_2)

{
  undefined4 uVar1;
  undefined4 local_2c;
  uchar *local_28;
  ulong local_24;
  
  local_2c = 1;
  local_28 = param_1;
  local_24 = param_2;
  uVar1 = _FT_Attach_Stream(**(undefined4 **)(this + 4),&local_2c);
  *(undefined4 *)(this + 0x30) = uVar1;
  return *(int *)(this + 0x30) == 0;
}


// ---- __ZN6FTFace4SizeEjj @ 0x3cd08 (88 bytes)

/* FTFace::Size(unsigned int, unsigned int) */

FTFace * __thiscall FTFace::Size(FTFace *this,uint param_1,uint param_2)

{
  undefined4 uVar1;
  
  FTSize::CharSize((FTSize *)(this + 8),*(FT_FaceRec_ ***)(this + 4),param_1,param_2,param_2);
  uVar1 = FTSize::Error();
  *(undefined4 *)(this + 0x30) = uVar1;
  return this + 8;
}


// ---- __ZN6FTFace12CharMapCountEv @ 0x3cd60 (26 bytes)

/* FTFace::CharMapCount() */

undefined4 __thiscall FTFace::CharMapCount(FTFace *this)

{
  return *(undefined4 *)(**(int **)(this + 4) + 0x24);
}


// ---- __ZN6FTFace11CharMapListEv @ 0x3cd7a (138 bytes)

/* FTFace::CharMapList() */

undefined4 __thiscall FTFace::CharMapList(FTFace *this)

{
  int iVar1;
  void *pvVar2;
  uint uVar3;
  uint local_10;
  
  if (*(int *)(this + 0x28) == 0) {
    iVar1 = CharMapCount(this);
    pvVar2 = operator_new__(iVar1 << 2);
    *(void **)(this + 0x28) = pvVar2;
    local_10 = 0;
    while( true ) {
      uVar3 = CharMapCount(this);
      if (uVar3 <= local_10) break;
      *(undefined4 *)(*(int *)(this + 0x28) + local_10 * 4) =
           *(undefined4 *)(*(int *)(*(int *)(**(int **)(this + 4) + 0x28) + local_10 * 4) + 4);
      local_10 = local_10 + 1;
    }
  }
  return *(undefined4 *)(this + 0x28);
}


// ---- __ZN6FTFace11KernAdvanceEjj @ 0x3ce04 (272 bytes)

/* FTFace::KernAdvance(unsigned int, unsigned int) */

uint FTFace::KernAdvance(uint param_1,uint param_2)

{
  undefined4 uVar1;
  int in_stack_0000000c;
  int in_stack_00000010;
  
  if (((*(char *)(param_2 + 0x2c) != '\0') && (in_stack_0000000c != 0)) && (in_stack_00000010 != 0))
  {
    uVar1 = _FT_Get_Kerning(**(undefined4 **)(param_2 + 4));
    *(undefined4 *)(param_2 + 0x30) = uVar1;
  }
  FTPoint::FTPoint((FTPoint *)param_1,0.0,0.0,0.0);
  return param_1;
}


// ---- __ZN6FTFace5GlyphEji @ 0x3cf14 (88 bytes)

/* FTFace::Glyph(unsigned int, int) */

undefined4 __thiscall FTFace::Glyph(FTFace *this,uint param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 local_10;
  
  uVar1 = _FT_Load_Glyph(**(undefined4 **)(this + 4),param_1,param_2);
  *(undefined4 *)(this + 0x30) = uVar1;
  if (*(int *)(this + 0x30) == 0) {
    local_10 = *(undefined4 *)(**(int **)(this + 4) + 0x54);
  }
  else {
    local_10 = 0;
  }
  return local_10;
}


// ---- __ZNK6FTSize5ErrorEv @ 0x3cf6c (20 bytes)

/* FTSize::Error() const */

undefined4 __thiscall FTSize::Error(FTSize *this)

{
  return *(undefined4 *)(this + 0x18);
}


// ---- __ZNK9FTLibrary10GetLibraryEv @ 0x3cf80 (19 bytes)

/* FTLibrary::GetLibrary() const */

undefined4 __thiscall FTLibrary::GetLibrary(FTLibrary *this)

{
  return *(undefined4 *)this;
}


