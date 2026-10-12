// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTGLTextureFont.cpp

// ---- __ZN15FTGLTextureFontC1EPKhm @ 0x3f77c (224 bytes)

/* FTGLTextureFont::FTGLTextureFont(unsigned char const*, unsigned long) */

void __thiscall FTGLTextureFont::FTGLTextureFont(FTGLTextureFont *this,uchar *param_1,ulong param_2)

{
  undefined4 uVar1;
  
  FTFont::FTFont((FTFont *)this,param_1,param_2);
  *(undefined **)this = PTR_vtable_001c4a50 + 8;
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x7c) = 0;
  *(undefined4 *)(this + 0x80) = 0;
  FTVector<unsigned_long>::FTVector();
  *(undefined4 *)(this + 0x94) = 0;
  *(undefined4 *)(this + 0x98) = 0;
  *(undefined4 *)(this + 0x9c) = 3;
  *(undefined4 *)(this + 0xa8) = 0;
  *(undefined4 *)(this + 0xac) = 0;
  uVar1 = FTFace::GlyphCount();
  *(undefined4 *)(this + 0xa0) = uVar1;
  *(undefined4 *)(this + 0xa4) = *(undefined4 *)(this + 0xa0);
  return;
}


// ---- __ZN15FTGLTextureFontC2EPKhm @ 0x3f85c (224 bytes)

/* FTGLTextureFont::FTGLTextureFont(unsigned char const*, unsigned long) */

void __thiscall FTGLTextureFont::FTGLTextureFont(FTGLTextureFont *this,uchar *param_1,ulong param_2)

{
  undefined4 uVar1;
  
  FTFont::FTFont((FTFont *)this,param_1,param_2);
  *(undefined **)this = PTR_vtable_001c4a50 + 8;
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x7c) = 0;
  *(undefined4 *)(this + 0x80) = 0;
  FTVector<unsigned_long>::FTVector();
  *(undefined4 *)(this + 0x94) = 0;
  *(undefined4 *)(this + 0x98) = 0;
  *(undefined4 *)(this + 0x9c) = 3;
  *(undefined4 *)(this + 0xa8) = 0;
  *(undefined4 *)(this + 0xac) = 0;
  uVar1 = FTFace::GlyphCount();
  *(undefined4 *)(this + 0xa0) = uVar1;
  *(undefined4 *)(this + 0xa4) = *(undefined4 *)(this + 0xa0);
  return;
}


// ---- __ZN15FTGLTextureFontC1EPKc @ 0x3f93c (216 bytes)

/* FTGLTextureFont::FTGLTextureFont(char const*) */

void __thiscall FTGLTextureFont::FTGLTextureFont(FTGLTextureFont *this,char *param_1)

{
  undefined4 uVar1;
  
  FTFont::FTFont((FTFont *)this,param_1);
  *(undefined **)this = PTR_vtable_001c4a50 + 8;
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x7c) = 0;
  *(undefined4 *)(this + 0x80) = 0;
  FTVector<unsigned_long>::FTVector();
  *(undefined4 *)(this + 0x94) = 0;
  *(undefined4 *)(this + 0x98) = 0;
  *(undefined4 *)(this + 0x9c) = 3;
  *(undefined4 *)(this + 0xa8) = 0;
  *(undefined4 *)(this + 0xac) = 0;
  uVar1 = FTFace::GlyphCount();
  *(undefined4 *)(this + 0xa0) = uVar1;
  *(undefined4 *)(this + 0xa4) = *(undefined4 *)(this + 0xa0);
  return;
}


// ---- __ZN15FTGLTextureFontC2EPKc @ 0x3fa14 (216 bytes)

/* FTGLTextureFont::FTGLTextureFont(char const*) */

void __thiscall FTGLTextureFont::FTGLTextureFont(FTGLTextureFont *this,char *param_1)

{
  undefined4 uVar1;
  
  FTFont::FTFont((FTFont *)this,param_1);
  *(undefined **)this = PTR_vtable_001c4a50 + 8;
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x7c) = 0;
  *(undefined4 *)(this + 0x80) = 0;
  FTVector<unsigned_long>::FTVector();
  *(undefined4 *)(this + 0x94) = 0;
  *(undefined4 *)(this + 0x98) = 0;
  *(undefined4 *)(this + 0x9c) = 3;
  *(undefined4 *)(this + 0xa8) = 0;
  *(undefined4 *)(this + 0xac) = 0;
  uVar1 = FTFace::GlyphCount();
  *(undefined4 *)(this + 0xa0) = uVar1;
  *(undefined4 *)(this + 0xa4) = *(undefined4 *)(this + 0xa0);
  return;
}


// ---- __ZN15FTGLTextureFont6RenderEPKc @ 0x3faec (86 bytes)

/* FTGLTextureFont::Render(char const*) */

void __thiscall FTGLTextureFont::Render(FTGLTextureFont *this,char *param_1)

{
  _glPushAttrib(0x6000);
  _glEnable(0xbe2);
  _glBlendFunc(0x302,0x303);
  FTTextureGlyph::ResetActiveTexture();
  FTFont::Render((FTFont *)this,param_1);
  _glPopAttrib();
  return;
}


// ---- __ZN15FTGLTextureFont6RenderEPKw @ 0x3fb42 (86 bytes)

/* FTGLTextureFont::Render(wchar_t const*) */

void __thiscall FTGLTextureFont::Render(FTGLTextureFont *this,wchar_t *param_1)

{
  _glPushAttrib(0x6000);
  _glEnable(0xbe2);
  _glBlendFunc(0x302,0x303);
  FTTextureGlyph::ResetActiveTexture();
  FTFont::Render((FTFont *)this,param_1);
  _glPopAttrib();
  return;
}


// ---- __ZN15FTGLTextureFont8FaceSizeEjj @ 0x3fb98 (184 bytes)

/* FTGLTextureFont::FaceSize(unsigned int, unsigned int) */

undefined1 FTGLTextureFont::FaceSize(uint param_1,uint param_2)

{
  char cVar1;
  undefined1 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  uint in_stack_0000000c;
  
  cVar1 = FTVector<unsigned_long>::empty();
  if (cVar1 != '\x01') {
    uVar3 = FTVector<unsigned_long>::operator[](param_1 + 0x84);
    uVar4 = FTVector<unsigned_long>::size();
    _glDeleteTextures(uVar4,uVar3);
    FTVector<unsigned_long>::clear();
    uVar3 = FTFace::GlyphCount();
    *(undefined4 *)(param_1 + 0xa0) = uVar3;
    *(undefined4 *)(param_1 + 0xa4) = *(undefined4 *)(param_1 + 0xa0);
  }
  uVar2 = FTFont::FaceSize((FTFont *)param_1,param_2,in_stack_0000000c);
  return uVar2;
}


// ---- __ZN15FTGLTextureFontD0Ev @ 0x3fc50 (224 bytes)

/* FTGLTextureFont::~FTGLTextureFont() */

void __thiscall FTGLTextureFont::~FTGLTextureFont(FTGLTextureFont *this)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  *(undefined **)this = PTR_vtable_001c4a50 + 8;
  uVar1 = FTVector<unsigned_long>::operator[]((ulong)(this + 0x84));
  uVar2 = FTVector<unsigned_long>::size();
  _glDeleteTextures(uVar2,uVar1);
  FTVector<unsigned_long>::~FTVector((FTVector<unsigned_long> *)(this + 0x84));
  FTFont::~FTFont((FTFont *)this);
  operator_delete(this);
  return;
}


// ---- __ZN15FTGLTextureFontD1Ev @ 0x3fd30 (224 bytes)

/* WARNING: Removing unreachable block (ram,0x0003fdba) */
/* FTGLTextureFont::~FTGLTextureFont() */

void __thiscall FTGLTextureFont::~FTGLTextureFont(FTGLTextureFont *this)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  *(undefined **)this = PTR_vtable_001c4a50 + 8;
  uVar1 = FTVector<unsigned_long>::operator[]((ulong)(this + 0x84));
  uVar2 = FTVector<unsigned_long>::size();
  _glDeleteTextures(uVar2,uVar1);
  FTVector<unsigned_long>::~FTVector((FTVector<unsigned_long> *)(this + 0x84));
  FTFont::~FTFont((FTFont *)this);
  return;
}


// ---- __ZN15FTGLTextureFontD2Ev @ 0x3fe10 (223 bytes)

/* WARNING: Removing unreachable block (ram,0x0003fe9a) */
/* FTGLTextureFont::~FTGLTextureFont() */

void __thiscall FTGLTextureFont::~FTGLTextureFont(FTGLTextureFont *this)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  *(undefined **)this = PTR_vtable_001c4a50 + 8;
  uVar1 = FTVector<unsigned_long>::operator[]((ulong)(this + 0x84));
  uVar2 = FTVector<unsigned_long>::size();
  _glDeleteTextures(uVar2,uVar1);
  FTVector<unsigned_long>::~FTVector((FTVector<unsigned_long> *)(this + 0x84));
  FTFont::~FTFont((FTFont *)this);
  return;
}


// ---- __ZNK6FTFace10GlyphCountEv @ 0x3fef0 (20 bytes)

/* FTFace::GlyphCount() const */

undefined4 __thiscall FTFace::GlyphCount(FTFace *this)

{
  return *(undefined4 *)(this + 0x24);
}


// ---- __ZNK7FTGlyph4BBoxEv @ 0x3ff04 (20 bytes)

/* FTGlyph::BBox() const */

FTGlyph * __thiscall FTGlyph::BBox(FTGlyph *this)

{
  return this + 0x1c;
}


// ---- __ZN14FTTextureGlyph18ResetActiveTextureEv @ 0x3ff18 (34 bytes)

/* FTTextureGlyph::ResetActiveTexture() */

void FTTextureGlyph::ResetActiveTexture(void)

{
  *(undefined4 *)PTR_activeTextureID_001c4a54 = 0;
  return;
}


// ---- __Z12NextPowerOf2m @ 0x3ff3a (66 bytes)

/* NextPowerOf2(unsigned long) */

int NextPowerOf2(ulong param_1)

{
  uint uVar1;
  
  uVar1 = param_1 - 1 | param_1 - 1 >> 0x10;
  uVar1 = uVar1 | uVar1 >> 8;
  uVar1 = uVar1 | uVar1 >> 4;
  uVar1 = uVar1 | uVar1 >> 2;
  return (uVar1 | uVar1 >> 1) + 1;
}


// ---- __ZN8FTVectorImEC1Ev @ 0x3ff7c (68 bytes)

/* FTVector<unsigned long>::FTVector() */

void __thiscall FTVector<unsigned_long>::FTVector(FTVector<unsigned_long> *this)

{
  *(undefined **)this = PTR_001c4a4c + 8;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 4) = *(undefined4 *)(this + 8);
  *(undefined4 *)(this + 0xc) = 0;
  return;
}


// ---- __ZNK8FTVectorImE4sizeEv @ 0x3ffc0 (20 bytes)

/* FTVector<unsigned long>::size() const */

undefined4 __thiscall FTVector<unsigned_long>::size(FTVector<unsigned_long> *this)

{
  return *(undefined4 *)(this + 8);
}


// ---- __ZNK8FTVectorImE5emptyEv @ 0x3ffd4 (34 bytes)

/* FTVector<unsigned long>::empty() const */

bool FTVector<unsigned_long>::empty(void)

{
  int iVar1;
  
  iVar1 = FTVector<unsigned_long>::size();
  return iVar1 == 0;
}


// ---- __ZN15FTGLTextureFont20CalculateTextureSizeEv @ 0x3fff6 (384 bytes)

/* FTGLTextureFont::CalculateTextureSize() */

void __thiscall FTGLTextureFont::CalculateTextureSize(FTGLTextureFont *this)

{
  undefined4 uVar1;
  undefined4 uStack_24;
  undefined4 uStack_20;
  
  if (*(int *)(this + 0x78) == 0) {
    _glGetIntegerv(0xd33,this + 0x78);
    if (*(int *)(this + 0x78) == 0) {
      ___eprintf("%s:%u: failed assertion `%s\'\n",
                 "/Users/vernjensen/Simple - Midnight Mansion/FTGL/src/FTGLTextureFont.cpp",0x67,
                 "maximumGLTextureSize");
    }
  }
  uVar1 = NextPowerOf2(*(int *)(this + 0xa4) * *(int *)(this + 0x98) + *(int *)(this + 0x9c) * 2);
  *(undefined4 *)(this + 0x7c) = uVar1;
  if (*(int *)(this + 0x78) < *(int *)(this + 0x7c)) {
    uStack_24 = *(undefined4 *)(this + 0x78);
  }
  else {
    uStack_24 = *(undefined4 *)(this + 0x7c);
  }
  *(undefined4 *)(this + 0x7c) = uStack_24;
  uVar1 = NextPowerOf2(*(int *)(this + 0x94) *
                       (*(uint *)(this + 0xa0) /
                        ((uint)(*(int *)(this + 0x7c) + *(int *)(this + 0x9c) * -2) /
                        *(uint *)(this + 0x98)) + 1));
  *(undefined4 *)(this + 0x80) = uVar1;
  if (*(int *)(this + 0x78) < *(int *)(this + 0x80)) {
    uStack_20 = *(undefined4 *)(this + 0x78);
  }
  else {
    uStack_20 = *(undefined4 *)(this + 0x80);
  }
  *(undefined4 *)(this + 0x80) = uStack_20;
  return;
}


// ---- __ZN15FTGLTextureFont13CreateTextureEv @ 0x40176 (336 bytes)

/* FTGLTextureFont::CreateTexture() */

undefined4 __thiscall FTGLTextureFont::CreateTexture(FTGLTextureFont *this)

{
  ulong uVar1;
  undefined1 *puVar2;
  undefined4 uStack_18;
  ulong uStack_14;
  undefined1 *puStack_10;
  
  ::FTGLTextureFont::CalculateTextureSize();
  uStack_14 = *(int *)(this + 0x80) * *(int *)(this + 0x7c);
  puStack_10 = (undefined1 *)operator_new__(uStack_14);
  puVar2 = puStack_10;
  for (uVar1 = uStack_14; uVar1 != 0; uVar1 = uVar1 - 1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  _glGenTextures(1,&uStack_18);
  _glBindTexture(0xde1,uStack_18);
  _glTexParameteri(0xde1,&DAT_00002802,&DAT_00002900);
  _glTexParameteri(0xde1,&DAT_00002803,&DAT_00002900);
  _glTexParameteri(0xde1,&DAT_00002800,&DAT_00002601);
  _glTexParameteri(0xde1,&DAT_00002801,&DAT_00002601);
  _glTexImage2D(0xde1,0,"ework/Versions/A/Carbon",*(undefined4 *)(this + 0x7c),
                *(undefined4 *)(this + 0x80),0,"ework/Versions/A/Carbon",0x1401,puStack_10);
  if (puStack_10 != (undefined1 *)0x0) {
    operator_delete__(puStack_10);
  }
  return uStack_18;
}


// ---- __ZN8FTVectorImE5clearEv @ 0x402c6 (80 bytes)

/* FTVector<unsigned long>::clear() */

void __thiscall FTVector<unsigned_long>::clear(FTVector<unsigned_long> *this)

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


// ---- __ZN8FTVectorImED1Ev @ 0x40316 (72 bytes)

/* WARNING: Removing unreachable block (ram,0x0004034d) */
/* FTVector<unsigned long>::~FTVector() */

void __thiscall FTVector<unsigned_long>::~FTVector(FTVector<unsigned_long> *this)

{
  *(undefined **)this = PTR_001c4a4c + 8;
  FTVector<unsigned_long>::clear();
  return;
}


// ---- __ZN8FTVectorImED0Ev @ 0x4035e (72 bytes)

/* FTVector<unsigned long>::~FTVector() */

void __thiscall FTVector<unsigned_long>::~FTVector(FTVector<unsigned_long> *this)

{
  *(undefined **)this = PTR_001c4a4c + 8;
  FTVector<unsigned_long>::clear();
  operator_delete(this);
  return;
}


// ---- __ZN8FTVectorImE5beginEv @ 0x403a6 (20 bytes)

/* FTVector<unsigned long>::begin() */

undefined4 __thiscall FTVector<unsigned_long>::begin(FTVector<unsigned_long> *this)

{
  return *(undefined4 *)(this + 0xc);
}


// ---- __ZN8FTVectorImEixEm @ 0x403ba (36 bytes)

/* FTVector<unsigned long>::operator[](unsigned long) */

int __thiscall FTVector<unsigned_long>::operator[](FTVector<unsigned_long> *this,ulong param_1)

{
  int iVar1;
  
  iVar1 = FTVector<unsigned_long>::begin();
  return iVar1 + param_1 * 4;
}


// ---- __ZNK8FTVectorImE8capacityEv @ 0x403de (20 bytes)

/* FTVector<unsigned long>::capacity() const */

undefined4 __thiscall FTVector<unsigned_long>::capacity(FTVector<unsigned_long> *this)

{
  return *(undefined4 *)(this + 4);
}


// ---- __ZN8FTVectorImE3endEv @ 0x403f2 (50 bytes)

/* FTVector<unsigned long>::end() */

int FTVector<unsigned_long>::end(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FTVector<unsigned_long>::begin();
  iVar2 = FTVector<unsigned_long>::size();
  return iVar1 + iVar2 * 4;
}


// ---- __ZN8FTVectorImE6expandEm @ 0x40424 (216 bytes)

/* FTVector<unsigned long>::expand(unsigned long) */

void __thiscall FTVector<unsigned_long>::expand(FTVector<unsigned_long> *this,ulong param_1)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  uint uStack_30;
  uint uStack_20;
  undefined4 *puStack_18;
  undefined4 *puStack_10;
  
  iVar1 = FTVector<unsigned_long>::capacity();
  if (iVar1 == 0) {
    uStack_30 = 0x100;
  }
  else {
    iVar1 = FTVector<unsigned_long>::capacity();
    uStack_30 = iVar1 * 2;
  }
  uStack_20 = uStack_30;
  if (param_1 != 0) {
    for (; uStack_20 < param_1; uStack_20 = uStack_20 << 1) {
    }
  }
  puVar2 = (undefined4 *)operator_new__(uStack_20 << 2);
  puStack_18 = (undefined4 *)FTVector<unsigned_long>::begin();
  puVar3 = (undefined4 *)FTVector<unsigned_long>::end();
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


// ---- __ZN8FTVectorImE9push_backERKm @ 0x404fc (120 bytes)

/* FTVector<unsigned long>::push_back(unsigned long const&) */

void __thiscall FTVector<unsigned_long>::push_back(FTVector<unsigned_long> *this,ulong *param_1)

{
  ulong uVar1;
  int iVar2;
  int iVar3;
  ulong *puVar4;
  
  iVar2 = FTVector<unsigned_long>::size();
  iVar3 = FTVector<unsigned_long>::capacity();
  if (iVar2 == iVar3) {
    FTVector<unsigned_long>::expand((ulong)this);
  }
  uVar1 = *param_1;
  FTVector<unsigned_long>::size();
  puVar4 = (ulong *)FTVector<unsigned_long>::operator[]((ulong)this);
  *puVar4 = uVar1;
  *(int *)(this + 8) = *(int *)(this + 8) + 1;
  return;
}


// ---- __ZN15FTGLTextureFont9MakeGlyphEj @ 0x40574 (753 bytes)

/* FTGLTextureFont::MakeGlyph(unsigned int) */

FTTextureGlyph * __thiscall FTGLTextureFont::MakeGlyph(FTGLTextureFont *this,uint param_1)

{
  float fVar1;
  int iVar2;
  char cVar3;
  FT_GlyphSlotRec_ *pFVar4;
  int *piVar5;
  int iVar6;
  float *pfVar7;
  undefined4 uVar8;
  longdouble lVar9;
  FTTextureGlyph *pFStack_30;
  
  pFVar4 = (FT_GlyphSlotRec_ *)FTFace::Glyph((FTFace *)(this + 4),param_1,2);
  if (pFVar4 == (FT_GlyphSlotRec_ *)0x0) {
    uVar8 = FTFace::Error();
    *(undefined4 *)(this + 0x58) = uVar8;
    pFStack_30 = (FTTextureGlyph *)0x0;
  }
  else {
    lVar9 = (longdouble)FTSize::Height((FTSize *)(this + 0x38));
    *(int *)(this + 0x94) = (int)lVar9;
    lVar9 = (longdouble)FTSize::Width((FTSize *)(this + 0x38));
    *(int *)(this + 0x98) = (int)lVar9;
    cVar3 = FTVector<unsigned_long>::empty();
    if (cVar3 != '\0') {
      ::FTGLTextureFont::CreateTexture();
      FTVector<unsigned_long>::push_back((ulong *)(this + 0x84));
      *(undefined4 *)(this + 0xac) = *(undefined4 *)(this + 0x9c);
      *(undefined4 *)(this + 0xa8) = *(undefined4 *)(this + 0xac);
    }
    if (*(int *)(this + 0x7c) - *(int *)(this + 0x98) < *(int *)(this + 0xa8)) {
      *(undefined4 *)(this + 0xa8) = *(undefined4 *)(this + 0x9c);
      *(int *)(this + 0xac) = *(int *)(this + 0xac) + *(int *)(this + 0x94);
      if (*(int *)(this + 0x80) - *(int *)(this + 0x94) < *(int *)(this + 0xac)) {
        ::FTGLTextureFont::CreateTexture();
        FTVector<unsigned_long>::push_back((ulong *)(this + 0x84));
        *(undefined4 *)(this + 0xac) = *(undefined4 *)(this + 0x9c);
      }
    }
    FTVector<unsigned_long>::size();
    piVar5 = (int *)FTVector<unsigned_long>::operator[]((ulong)(this + 0x84));
    iVar2 = *piVar5;
    pFStack_30 = (FTTextureGlyph *)operator_new(0x90);
    FTTextureGlyph::FTTextureGlyph
              (pFStack_30,pFVar4,iVar2,*(int *)(this + 0xa8),*(int *)(this + 0xac),
               *(long *)(this + 0x7c),*(long *)(this + 0x80));
    iVar2 = *(int *)(this + 0xa8);
    iVar6 = FTGlyph::BBox();
    fVar1 = *(float *)(iVar6 + 0xc);
    pfVar7 = (float *)FTGlyph::BBox();
    *(int *)(this + 0xa8) =
         iVar2 + (int)((float)(*(uint *)(this + 0x9c) >> 0x10) * 65536.0 +
                       (float)(*(uint *)(this + 0x9c) & 0xffff) + (fVar1 - *pfVar7));
    *(int *)(this + 0xa4) = *(int *)(this + 0xa4) + -1;
  }
  return pFStack_30;
}


