// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTGlyphContainer.cpp

// ---- __ZN16FTGlyphContainer7CharMapE12FT_Encoding_ @ 0x40d3e (64 bytes)

/* FTGlyphContainer::CharMap(FT_Encoding_) */

undefined1 __thiscall FTGlyphContainer::CharMap(FTGlyphContainer *this,undefined4 param_2)

{
  undefined1 uVar1;
  undefined4 uVar2;
  
  uVar1 = FTCharmap::CharMap(*(FTCharmap **)(this + 4),param_2);
  uVar2 = FTCharmap::Error();
  *(undefined4 *)(this + 0x18) = uVar2;
  return uVar1;
}


// ---- __ZNK16FTGlyphContainer9FontIndexEj @ 0x40d7e (36 bytes)

/* FTGlyphContainer::FontIndex(unsigned int) const */

void __thiscall FTGlyphContainer::FontIndex(FTGlyphContainer *this,uint param_1)

{
  FTCharmap::FontIndex(*(FTCharmap **)(this + 4),param_1);
  return;
}


// ---- __ZN16FTGlyphContainerD1Ev @ 0x40da2 (210 bytes)

/* FTGlyphContainer::~FTGlyphContainer() */

void __thiscall FTGlyphContainer::~FTGlyphContainer(FTGlyphContainer *this)

{
  int *piVar1;
  int *local_10;
  
  local_10 = (int *)FTVector<FTGlyph*>::begin();
  while( true ) {
    piVar1 = (int *)FTVector<FTGlyph*>::end();
    if (piVar1 == local_10) break;
    if (*local_10 != 0) {
      (**(code **)(*(int *)*local_10 + 4))(*local_10);
    }
    local_10 = local_10 + 1;
  }
  FTVector<FTGlyph*>::clear();
  if (*(int *)(this + 4) != 0) {
    (**(code **)(**(int **)(this + 4) + 4))(*(undefined4 *)(this + 4));
  }
  FTVector<FTGlyph*>::~FTVector((FTVector<FTGlyph*> *)(this + 8));
  return;
}


// ---- __ZN16FTGlyphContainerD2Ev @ 0x40e74 (210 bytes)

/* FTGlyphContainer::~FTGlyphContainer() */

void __thiscall FTGlyphContainer::~FTGlyphContainer(FTGlyphContainer *this)

{
  int *piVar1;
  int *piStack_10;
  
  piStack_10 = (int *)FTVector<FTGlyph*>::begin();
  while( true ) {
    piVar1 = (int *)FTVector<FTGlyph*>::end();
    if (piVar1 == piStack_10) break;
    if (*piStack_10 != 0) {
      (**(code **)(*(int *)*piStack_10 + 4))(*piStack_10);
    }
    piStack_10 = piStack_10 + 1;
  }
  FTVector<FTGlyph*>::clear();
  if (*(int *)(this + 4) != 0) {
    (**(code **)(**(int **)(this + 4) + 4))(*(undefined4 *)(this + 4));
  }
  FTVector<FTGlyph*>::~FTVector((FTVector<FTGlyph*> *)(this + 8));
  return;
}


// ---- __ZN16FTGlyphContainer7AdvanceEjj @ 0x40f46 (270 bytes)

/* FTGlyphContainer::Advance(unsigned int, unsigned int) */

longdouble __thiscall FTGlyphContainer::Advance(FTGlyphContainer *this,uint param_1,uint param_2)

{
  longdouble lVar1;
  longdouble lVar2;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  float local_10;
  
  local_18 = FTCharmap::FontIndex(*(FTCharmap **)(this + 4),param_1);
  local_14 = FTCharmap::FontIndex(*(FTCharmap **)(this + 4),param_2);
  FTFace::KernAdvance((uint)&local_5c,*(uint *)this);
  local_30 = local_5c;
  local_2c = local_58;
  local_28 = local_54;
  local_24 = local_50;
  local_20 = local_4c;
  local_1c = local_48;
  lVar1 = (longdouble)FTPoint::X();
  local_10 = (float)lVar1;
  FTCharmap::GlyphListIndex(*(uint *)(this + 4));
  FTVector<FTGlyph*>::operator[]((ulong)(this + 8));
  FTGlyph::Advance();
  lVar2 = (longdouble)FTPoint::X();
  return (longdouble)((float)lVar1 + (float)lVar2);
}


// ---- __ZN16FTGlyphContainer6RenderEjj7FTPoint @ 0x41054 (316 bytes)

/* FTGlyphContainer::Render(unsigned int, unsigned int, FTPoint) */

FTGlyphContainer * __thiscall
FTGlyphContainer::Render(FTGlyphContainer *this,uint *param_1,uint param_2,uint param_4)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_14;
  undefined4 local_10;
  
  FTPoint::FTPoint((FTPoint *)this);
  FTPoint::FTPoint((FTPoint *)&local_44);
  local_14 = FTCharmap::FontIndex((FTCharmap *)param_1[1],param_2);
  uVar1 = FTCharmap::FontIndex((FTCharmap *)param_1[1],param_4);
  uVar4 = local_14;
  local_10 = uVar1;
  FTFace::KernAdvance((uint)&local_6c,*param_1);
  *(undefined4 *)this = local_6c;
  *(undefined4 *)(this + 4) = local_68;
  *(undefined4 *)(this + 8) = local_64;
  *(undefined4 *)(this + 0xc) = local_60;
  *(undefined4 *)(this + 0x10) = local_5c;
  *(undefined4 *)(this + 0x14) = local_58;
  iVar2 = FTFace::Error();
  if (iVar2 == 0) {
    FTCharmap::GlyphListIndex(param_1[1]);
    puVar3 = (undefined4 *)FTVector<FTGlyph*>::operator[]((ulong)(param_1 + 2));
    puVar3 = (undefined4 *)
             (**(code **)(*(int *)*puVar3 + 8))((int *)*puVar3,&stack0x00000014,uVar4,uVar1);
    local_44 = *puVar3;
    local_40 = puVar3[1];
    local_3c = puVar3[2];
    local_38 = puVar3[3];
    local_34 = puVar3[4];
    local_30 = puVar3[5];
  }
  FTPoint::operator+=((FTPoint *)this,(FTPoint *)&local_44);
  return this;
}


// ---- __ZN16FTGlyphContainer3AddEP7FTGlyphj @ 0x41190 (76 bytes)

/* FTGlyphContainer::Add(FTGlyph*, unsigned int) */

void __thiscall FTGlyphContainer::Add(FTGlyphContainer *this,FTGlyph *param_1,uint param_2)

{
  FTVector<FTGlyph*>::size();
  FTCharmap::InsertIndex(*(uint *)(this + 4),param_2);
  FTVector<FTGlyph*>::push_back((FTGlyph **)(this + 8));
  return;
}


// ---- __ZN16FTGlyphContainerC1EP6FTFace @ 0x411dc (182 bytes)

/* FTGlyphContainer::FTGlyphContainer(FTFace*) */

void __thiscall FTGlyphContainer::FTGlyphContainer(FTGlyphContainer *this,FTFace *param_1)

{
  FTCharmap *this_00;
  
  *(FTFace **)this = param_1;
  FTVector<FTGlyph*>::FTVector();
  *(undefined4 *)(this + 0x18) = 0;
  FTVector<FTGlyph*>::push_back((FTGlyph **)(this + 8));
  this_00 = (FTCharmap *)operator_new(0x18);
  FTCharmap::FTCharmap(this_00,*(FTFace **)this);
  *(FTCharmap **)(this + 4) = this_00;
  return;
}


// ---- __ZN16FTGlyphContainerC2EP6FTFace @ 0x41292 (182 bytes)

/* FTGlyphContainer::FTGlyphContainer(FTFace*) */

void __thiscall FTGlyphContainer::FTGlyphContainer(FTGlyphContainer *this,FTFace *param_1)

{
  FTCharmap *this_00;
  
  *(FTFace **)this = param_1;
  FTVector<FTGlyph*>::FTVector();
  *(undefined4 *)(this + 0x18) = 0;
  FTVector<FTGlyph*>::push_back((FTGlyph **)(this + 8));
  this_00 = (FTCharmap *)operator_new(0x18);
  FTCharmap::FTCharmap(this_00,*(FTFace **)this);
  *(FTCharmap **)(this + 4) = this_00;
  return;
}


// ---- __ZNK16FTGlyphContainer4BBoxEj @ 0x41348 (114 bytes)

/* FTGlyphContainer::BBox(unsigned int) const */

FTGlyphContainer * __thiscall FTGlyphContainer::BBox(FTGlyphContainer *this,uint param_1)

{
  undefined4 *puVar1;
  
  FTCharmap::GlyphListIndex(*(uint *)(param_1 + 4));
  FTVector<FTGlyph*>::operator[](param_1 + 8);
  puVar1 = (undefined4 *)FTGlyph::BBox();
  *(undefined4 *)this = *puVar1;
  *(undefined4 *)(this + 4) = puVar1[1];
  *(undefined4 *)(this + 8) = puVar1[2];
  *(undefined4 *)(this + 0xc) = puVar1[3];
  *(undefined4 *)(this + 0x10) = puVar1[4];
  *(undefined4 *)(this + 0x14) = puVar1[5];
  return this;
}


// ---- __ZNK16FTGlyphContainer5GlyphEj @ 0x413ba (61 bytes)

/* FTGlyphContainer::Glyph(unsigned int) const */

undefined4 FTGlyphContainer::Glyph(uint param_1)

{
  undefined4 *puVar1;
  
  FTCharmap::GlyphListIndex(*(uint *)(param_1 + 4));
  puVar1 = (undefined4 *)FTVector<FTGlyph*>::operator[](param_1 + 8);
  return *puVar1;
}


// ---- __ZN7FTPointpLERKS_ @ 0x413f8 (98 bytes)

/* FTPoint::TEMPNAMEPLACEHOLDERVALUE(FTPoint const&) */

FTPoint * __thiscall FTPoint::operator+=(FTPoint *this,FTPoint *param_1)

{
  *(double *)this = *(double *)param_1 + *(double *)this;
  *(double *)(this + 8) = *(double *)(param_1 + 8) + *(double *)(this + 8);
  *(double *)(this + 0x10) = *(double *)(param_1 + 0x10) + *(double *)(this + 0x10);
  return this;
}


// ---- __ZNK7FTGlyph7AdvanceEv @ 0x4145a (20 bytes)

/* FTGlyph::Advance() const */

FTGlyph * __thiscall FTGlyph::Advance(FTGlyph *this)

{
  return this + 4;
}


// ---- __ZNK9FTCharmap5ErrorEv @ 0x4146e (20 bytes)

/* FTCharmap::Error() const */

undefined4 __thiscall FTCharmap::Error(FTCharmap *this)

{
  return *(undefined4 *)(this + 0x14);
}


// ---- __ZN8FTVectorIP7FTGlyphEC1Ev @ 0x41482 (68 bytes)

/* FTVector<FTGlyph*>::FTVector() */

void __thiscall FTVector<FTGlyph*>::FTVector(FTVector<FTGlyph*> *this)

{
  *(undefined **)this = PTR_001c4a5c + 8;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 4) = *(undefined4 *)(this + 8);
  *(undefined4 *)(this + 0xc) = 0;
  return;
}


// ---- __ZN8FTVectorIP7FTGlyphE5beginEv @ 0x414c6 (20 bytes)

/* FTVector<FTGlyph*>::begin() */

undefined4 __thiscall FTVector<FTGlyph*>::begin(FTVector<FTGlyph*> *this)

{
  return *(undefined4 *)(this + 0xc);
}


// ---- __ZN8FTVectorIP7FTGlyphE5clearEv @ 0x414da (80 bytes)

/* FTVector<FTGlyph*>::clear() */

void __thiscall FTVector<FTGlyph*>::clear(FTVector<FTGlyph*> *this)

{
  if (*(int *)(this + 4) != 0) {
    if (*(int *)(this + 0xc) != 0) {
      operator_delete__(*(void **)(this + 0xc));
    }
    *(undefined4 *)(this + 8) = 0;
    *(undefined4 *)(this + 4) = *(undefined4 *)(this + 8);
    *(undefined4 *)(this + 0xc) = 0;
  }
  return;
}


// ---- __ZN8FTVectorIP7FTGlyphED1Ev @ 0x4152a (72 bytes)

/* WARNING: Removing unreachable block (ram,0x00041561) */
/* FTVector<FTGlyph*>::~FTVector() */

void __thiscall FTVector<FTGlyph*>::~FTVector(FTVector<FTGlyph*> *this)

{
  *(undefined **)this = PTR_001c4a5c + 8;
  FTVector<FTGlyph*>::clear();
  return;
}


// ---- __ZNK8FTVectorIP7FTGlyphE4sizeEv @ 0x41572 (20 bytes)

/* FTVector<FTGlyph*>::size() const */

undefined4 __thiscall FTVector<FTGlyph*>::size(FTVector<FTGlyph*> *this)

{
  return *(undefined4 *)(this + 8);
}


// ---- __ZN8FTVectorIP7FTGlyphE3endEv @ 0x41586 (50 bytes)

/* FTVector<FTGlyph*>::end() */

int FTVector<FTGlyph*>::end(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FTVector<FTGlyph*>::begin();
  iVar2 = FTVector<FTGlyph*>::size();
  return iVar1 + iVar2 * 4;
}


// ---- __ZN8FTVectorIP7FTGlyphEixEm @ 0x415b8 (36 bytes)

/* FTVector<FTGlyph*>::operator[](unsigned long) */

int __thiscall FTVector<FTGlyph*>::operator[](FTVector<FTGlyph*> *this,ulong param_1)

{
  int iVar1;
  
  iVar1 = FTVector<FTGlyph*>::begin();
  return iVar1 + param_1 * 4;
}


// ---- __ZN8FTVectorIP7FTGlyphED0Ev @ 0x415dc (72 bytes)

/* FTVector<FTGlyph*>::~FTVector() */

void __thiscall FTVector<FTGlyph*>::~FTVector(FTVector<FTGlyph*> *this)

{
  *(undefined **)this = PTR_001c4a5c + 8;
  FTVector<FTGlyph*>::clear();
  operator_delete(this);
  return;
}


// ---- __ZNK8FTVectorIP7FTGlyphE8capacityEv @ 0x41624 (20 bytes)

/* FTVector<FTGlyph*>::capacity() const */

undefined4 __thiscall FTVector<FTGlyph*>::capacity(FTVector<FTGlyph*> *this)

{
  return *(undefined4 *)(this + 4);
}


// ---- __ZN8FTVectorIP7FTGlyphE6expandEm @ 0x41638 (216 bytes)

/* FTVector<FTGlyph*>::expand(unsigned long) */

void __thiscall FTVector<FTGlyph*>::expand(FTVector<FTGlyph*> *this,ulong param_1)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  uint uStack_30;
  uint uStack_20;
  undefined4 *puStack_18;
  undefined4 *puStack_10;
  
  iVar1 = FTVector<FTGlyph*>::capacity();
  if (iVar1 == 0) {
    uStack_30 = 0x100;
  }
  else {
    iVar1 = FTVector<FTGlyph*>::capacity();
    uStack_30 = iVar1 * 2;
  }
  uStack_20 = uStack_30;
  if (param_1 != 0) {
    for (; uStack_20 < param_1; uStack_20 = uStack_20 << 1) {
    }
  }
  puVar2 = (undefined4 *)operator_new__(uStack_20 << 2);
  puStack_18 = (undefined4 *)FTVector<FTGlyph*>::begin();
  puVar3 = (undefined4 *)FTVector<FTGlyph*>::end();
  puStack_10 = puVar2;
  for (; puStack_18 != puVar3; puStack_18 = puStack_18 + 1) {
    *puStack_10 = *puStack_18;
    puStack_10 = puStack_10 + 1;
  }
  if ((*(int *)(this + 4) != 0) && (*(int *)(this + 0xc) != 0)) {
    operator_delete__(*(void **)(this + 0xc));
  }
  *(undefined4 **)(this + 0xc) = puVar2;
  *(uint *)(this + 4) = uStack_20;
  return;
}


// ---- __ZN8FTVectorIP7FTGlyphE9push_backERKS1_ @ 0x41710 (120 bytes)

/* FTVector<FTGlyph*>::push_back(FTGlyph* const&) */

void __thiscall FTVector<FTGlyph*>::push_back(FTVector<FTGlyph*> *this,FTGlyph **param_1)

{
  FTGlyph *pFVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  
  iVar2 = FTVector<FTGlyph*>::size();
  iVar3 = FTVector<FTGlyph*>::capacity();
  if (iVar2 == iVar3) {
    FTVector<FTGlyph*>::expand((ulong)this);
  }
  pFVar1 = *param_1;
  FTVector<FTGlyph*>::size();
  puVar4 = (undefined4 *)FTVector<FTGlyph*>::operator[]((ulong)this);
  *puVar4 = pFVar1;
  *(int *)(this + 8) = *(int *)(this + 8) + 1;
  return;
}


// ---- __ZNK8FTVectorIP7FTGlyphE5beginEv @ 0x41788 (20 bytes)

/* FTVector<FTGlyph*>::begin() const */

undefined4 __thiscall FTVector<FTGlyph*>::begin(FTVector<FTGlyph*> *this)

{
  return *(undefined4 *)(this + 0xc);
}


// ---- __ZNK8FTVectorIP7FTGlyphEixEm @ 0x4179c (36 bytes)

/* FTVector<FTGlyph*>::operator[](unsigned long) const */

int __thiscall FTVector<FTGlyph*>::operator[](FTVector<FTGlyph*> *this,ulong param_1)

{
  int iVar1;
  
  iVar1 = FTVector<FTGlyph*>::begin();
  return iVar1 + param_1 * 4;
}


