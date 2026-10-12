// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTCharmap.cpp

// ---- __ZN9FTCharmapC2EP6FTFace @ 0x385c2 (190 bytes)

/* FTCharmap::FTCharmap(FTFace*) */

void __thiscall FTCharmap::FTCharmap(FTCharmap *this,FTFace *param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  
  *(undefined **)this = PTR_vtable_001c4a18 + 8;
  puVar1 = (undefined4 *)FTFace::Face();
  *(undefined4 *)(this + 8) = *puVar1;
  FTCharToGlyphIndexMap::FTCharToGlyphIndexMap((FTCharToGlyphIndexMap *)(this + 0xc));
  *(undefined4 *)(this + 0x14) = 0;
  if (*(int *)(*(int *)(this + 8) + 0x5c) == 0) {
    uVar2 = _FT_Set_Charmap(*(undefined4 *)(this + 8),**(undefined4 **)(*(int *)(this + 8) + 0x28));
    *(undefined4 *)(this + 0x14) = uVar2;
  }
  *(undefined4 *)(this + 4) = *(undefined4 *)(*(int *)(*(int *)(this + 8) + 0x5c) + 4);
  return;
}


// ---- __ZN9FTCharmapC1EP6FTFace @ 0x38680 (190 bytes)

/* FTCharmap::FTCharmap(FTFace*) */

void __thiscall FTCharmap::FTCharmap(FTCharmap *this,FTFace *param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  
  *(undefined **)this = PTR_vtable_001c4a18 + 8;
  puVar1 = (undefined4 *)FTFace::Face();
  *(undefined4 *)(this + 8) = *puVar1;
  FTCharToGlyphIndexMap::FTCharToGlyphIndexMap((FTCharToGlyphIndexMap *)(this + 0xc));
  *(undefined4 *)(this + 0x14) = 0;
  if (*(int *)(*(int *)(this + 8) + 0x5c) == 0) {
    uVar2 = _FT_Set_Charmap(*(undefined4 *)(this + 8),**(undefined4 **)(*(int *)(this + 8) + 0x28));
    *(undefined4 *)(this + 0x14) = uVar2;
  }
  *(undefined4 *)(this + 4) = *(undefined4 *)(*(int *)(*(int *)(this + 8) + 0x5c) + 4);
  return;
}


// ---- __ZN9FTCharmapD2Ev @ 0x3873e (90 bytes)

/* WARNING: Removing unreachable block (ram,0x00038786) */
/* FTCharmap::~FTCharmap() */

void __thiscall FTCharmap::~FTCharmap(FTCharmap *this)

{
  *(undefined **)this = PTR_vtable_001c4a18 + 8;
  FTCharToGlyphIndexMap::clear();
  FTCharToGlyphIndexMap::~FTCharToGlyphIndexMap((FTCharToGlyphIndexMap *)(this + 0xc));
  return;
}


// ---- __ZN9FTCharmapD1Ev @ 0x38798 (90 bytes)

/* WARNING: Removing unreachable block (ram,0x000387e0) */
/* FTCharmap::~FTCharmap() */

void __thiscall FTCharmap::~FTCharmap(FTCharmap *this)

{
  *(undefined **)this = PTR_vtable_001c4a18 + 8;
  FTCharToGlyphIndexMap::clear();
  FTCharToGlyphIndexMap::~FTCharToGlyphIndexMap((FTCharToGlyphIndexMap *)(this + 0xc));
  return;
}


// ---- __ZN9FTCharmapD0Ev @ 0x387f2 (90 bytes)

/* FTCharmap::~FTCharmap() */

void __thiscall FTCharmap::~FTCharmap(FTCharmap *this)

{
  *(undefined **)this = PTR_vtable_001c4a18 + 8;
  FTCharToGlyphIndexMap::clear();
  FTCharToGlyphIndexMap::~FTCharToGlyphIndexMap((FTCharToGlyphIndexMap *)(this + 0xc));
  operator_delete(this);
  return;
}


// ---- __ZN9FTCharmap7CharMapE12FT_Encoding_ @ 0x3884c (128 bytes)

/* FTCharmap::CharMap(FT_Encoding_) */

bool __thiscall FTCharmap::CharMap(FTCharmap *this,int param_2)

{
  undefined4 uVar1;
  bool bVar2;
  
  if (*(int *)(this + 4) == param_2) {
    bVar2 = true;
  }
  else {
    uVar1 = _FT_Select_Charmap(*(undefined4 *)(this + 8),param_2);
    *(undefined4 *)(this + 0x14) = uVar1;
    if (*(int *)(this + 0x14) == 0) {
      *(int *)(this + 4) = param_2;
    }
    else {
      *(undefined4 *)(this + 4) = 0;
    }
    FTCharToGlyphIndexMap::clear();
    bVar2 = *(int *)(this + 0x14) == 0;
  }
  return bVar2;
}


// ---- __ZN9FTCharmap14GlyphListIndexEj @ 0x388cc (36 bytes)

/* FTCharmap::GlyphListIndex(unsigned int) */

void FTCharmap::GlyphListIndex(uint param_1)

{
  FTCharToGlyphIndexMap::find(param_1 + 0xc);
  return;
}


// ---- __ZN9FTCharmap9FontIndexEj @ 0x388f0 (36 bytes)

/* FTCharmap::FontIndex(unsigned int) */

void __thiscall FTCharmap::FontIndex(FTCharmap *this,uint param_1)

{
  _FT_Get_Char_Index(*(undefined4 *)(this + 8),param_1);
  return;
}


// ---- __ZN9FTCharmap11InsertIndexEjj @ 0x38914 (42 bytes)

/* FTCharmap::InsertIndex(unsigned int, unsigned int) */

void FTCharmap::InsertIndex(uint param_1,uint param_2)

{
  FTCharToGlyphIndexMap::insert(param_1 + 0xc,param_2);
  return;
}


// ---- __ZNK6FTFace4FaceEv @ 0x3893e (20 bytes)

/* FTFace::Face() const */

undefined4 __thiscall FTFace::Face(FTFace *this)

{
  return *(undefined4 *)(this + 4);
}


// ---- __ZN21FTCharToGlyphIndexMapC1Ev @ 0x38952 (46 bytes)

/* FTCharToGlyphIndexMap::FTCharToGlyphIndexMap() */

void __thiscall FTCharToGlyphIndexMap::FTCharToGlyphIndexMap(FTCharToGlyphIndexMap *this)

{
  *(undefined **)this = PTR_001c4a1c + 8;
  *(undefined4 *)(this + 4) = 0;
  return;
}


// ---- __ZN21FTCharToGlyphIndexMap5clearEv @ 0x38980 (136 bytes)

/* FTCharToGlyphIndexMap::clear() */

void __thiscall FTCharToGlyphIndexMap::clear(FTCharToGlyphIndexMap *this)

{
  int iStack_10;
  
  if (*(int *)(this + 4) != 0) {
    for (iStack_10 = 0; iStack_10 < 0x100; iStack_10 = iStack_10 + 1) {
      if (*(int *)(*(int *)(this + 4) + iStack_10 * 4) != 0) {
        if (*(int *)(*(int *)(this + 4) + iStack_10 * 4) != 0) {
          operator_delete__(*(void **)(*(int *)(this + 4) + iStack_10 * 4));
        }
        *(undefined4 *)(*(int *)(this + 4) + iStack_10 * 4) = 0;
      }
    }
  }
  return;
}


// ---- __ZN21FTCharToGlyphIndexMapD1Ev @ 0x38a08 (116 bytes)

/* WARNING: Removing unreachable block (ram,0x00038a6b) */
/* FTCharToGlyphIndexMap::~FTCharToGlyphIndexMap() */

void __thiscall FTCharToGlyphIndexMap::~FTCharToGlyphIndexMap(FTCharToGlyphIndexMap *this)

{
  *(undefined **)this = PTR_001c4a1c + 8;
  if (*(int *)(this + 4) != 0) {
    ::FTCharToGlyphIndexMap::clear();
    if (*(int *)(this + 4) != 0) {
      operator_delete__(*(void **)(this + 4));
    }
    *(undefined4 *)(this + 4) = 0;
  }
  return;
}


// ---- __ZN21FTCharToGlyphIndexMapD0Ev @ 0x38a7c (116 bytes)

/* FTCharToGlyphIndexMap::~FTCharToGlyphIndexMap() */

void __thiscall FTCharToGlyphIndexMap::~FTCharToGlyphIndexMap(FTCharToGlyphIndexMap *this)

{
  *(undefined **)this = PTR_001c4a1c + 8;
  if (*(int *)(this + 4) != 0) {
    ::FTCharToGlyphIndexMap::clear();
    if (*(int *)(this + 4) != 0) {
      operator_delete__(*(void **)(this + 4));
    }
    *(undefined4 *)(this + 4) = 0;
  }
  operator_delete(this);
  return;
}


// ---- __ZN21FTCharToGlyphIndexMap4findEm @ 0x38af0 (148 bytes)

/* FTCharToGlyphIndexMap::find(unsigned long) */

int __thiscall FTCharToGlyphIndexMap::find(FTCharToGlyphIndexMap *this,ulong param_1)

{
  int *piVar1;
  div_t *pdVar2;
  int extraout_EDX;
  int in_stack_ffffffcc;
  int iStack_20;
  
  if (*(int *)(this + 4) == 0) {
    iStack_20 = 0;
  }
  else {
    pdVar2 = _div((div_t *)param_1,0x100,in_stack_ffffffcc);
    if (*(int *)(*(int *)(this + 4) + (int)pdVar2 * 4) == 0) {
      iStack_20 = 0;
    }
    else {
      piVar1 = (int *)(*(int *)(*(int *)(this + 4) + (int)pdVar2 * 4) + extraout_EDX * 4);
      if (*piVar1 == -1) {
        iStack_20 = 0;
      }
      else {
        iStack_20 = *piVar1;
      }
    }
  }
  return iStack_20;
}


// ---- __ZN21FTCharToGlyphIndexMap6insertEml @ 0x38b84 (250 bytes)

/* FTCharToGlyphIndexMap::insert(unsigned long, long) */

void __thiscall
FTCharToGlyphIndexMap::insert(FTCharToGlyphIndexMap *this,ulong param_1,long param_2)

{
  void *pvVar1;
  div_t *pdVar2;
  int extraout_EDX;
  int in_stack_ffffffdc;
  int iStack_14;
  int iStack_10;
  
  if (*(int *)(this + 4) == 0) {
    pvVar1 = operator_new__(0x400);
    *(void **)(this + 4) = pvVar1;
    for (iStack_14 = 0; iStack_14 < 0x100; iStack_14 = iStack_14 + 1) {
      *(undefined4 *)(*(int *)(this + 4) + iStack_14 * 4) = 0;
    }
  }
  pdVar2 = _div((div_t *)param_1,0x100,in_stack_ffffffdc);
  if (*(int *)(*(int *)(this + 4) + (int)pdVar2 * 4) == 0) {
    pvVar1 = operator_new__(0x400);
    *(void **)(*(int *)(this + 4) + (int)pdVar2 * 4) = pvVar1;
    for (iStack_10 = 0; iStack_10 < 0x100; iStack_10 = iStack_10 + 1) {
      *(undefined4 *)(*(int *)(*(int *)(this + 4) + (int)pdVar2 * 4) + iStack_10 * 4) = 0xffffffff;
    }
  }
  *(long *)(*(int *)(*(int *)(this + 4) + (int)pdVar2 * 4) + extraout_EDX * 4) = param_2;
  return;
}


