// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTFont.cpp

// ---- __ZN6FTFontC2EPKc @ 0x3cf94 (274 bytes)

/* FTFont::FTFont(char const*) */

void __thiscall FTFont::FTFont(FTFont *this,char *param_1)

{
  undefined4 uVar1;
  FTGlyphContainer *this_00;
  
  *(undefined **)this = PTR_vtable_001c4a34 + 8;
  FTFace::FTFace((FTFace *)(this + 4),param_1);
  FTSize::FTSize((FTSize *)(this + 0x38));
  this[0x54] = (FTFont)0x1;
  *(undefined4 *)(this + 0x5c) = 0;
  FTPoint::FTPoint((FTPoint *)(this + 0x60));
  uVar1 = FTFace::Error();
  *(undefined4 *)(this + 0x58) = uVar1;
  if (*(int *)(this + 0x58) == 0) {
    this_00 = (FTGlyphContainer *)operator_new(0x1c);
    FTGlyphContainer::FTGlyphContainer(this_00,(FTFace *)(this + 4));
    *(FTGlyphContainer **)(this + 0x5c) = this_00;
  }
  return;
}


// ---- __ZN6FTFontC1EPKc @ 0x3d0a6 (274 bytes)

/* FTFont::FTFont(char const*) */

void __thiscall FTFont::FTFont(FTFont *this,char *param_1)

{
  undefined4 uVar1;
  FTGlyphContainer *this_00;
  
  *(undefined **)this = PTR_vtable_001c4a34 + 8;
  FTFace::FTFace((FTFace *)(this + 4),param_1);
  FTSize::FTSize((FTSize *)(this + 0x38));
  this[0x54] = (FTFont)0x1;
  *(undefined4 *)(this + 0x5c) = 0;
  FTPoint::FTPoint((FTPoint *)(this + 0x60));
  uVar1 = FTFace::Error();
  *(undefined4 *)(this + 0x58) = uVar1;
  if (*(int *)(this + 0x58) == 0) {
    this_00 = (FTGlyphContainer *)operator_new(0x1c);
    FTGlyphContainer::FTGlyphContainer(this_00,(FTFace *)(this + 4));
    *(FTGlyphContainer **)(this + 0x5c) = this_00;
  }
  return;
}


// ---- __ZN6FTFontC2EPKhm @ 0x3d1b8 (274 bytes)

/* FTFont::FTFont(unsigned char const*, unsigned long) */

void __thiscall FTFont::FTFont(FTFont *this,uchar *param_1,ulong param_2)

{
  undefined4 uVar1;
  FTGlyphContainer *this_00;
  
  *(undefined **)this = PTR_vtable_001c4a34 + 8;
  FTFace::FTFace((FTFace *)(this + 4),param_1,param_2);
  FTSize::FTSize((FTSize *)(this + 0x38));
  *(undefined4 *)(this + 0x5c) = 0;
  FTPoint::FTPoint((FTPoint *)(this + 0x60));
  uVar1 = FTFace::Error();
  *(undefined4 *)(this + 0x58) = uVar1;
  if (*(int *)(this + 0x58) == 0) {
    this_00 = (FTGlyphContainer *)operator_new(0x1c);
    FTGlyphContainer::FTGlyphContainer(this_00,(FTFace *)(this + 4));
    *(FTGlyphContainer **)(this + 0x5c) = this_00;
  }
  return;
}


// ---- __ZN6FTFontC1EPKhm @ 0x3d2ca (274 bytes)

/* FTFont::FTFont(unsigned char const*, unsigned long) */

void __thiscall FTFont::FTFont(FTFont *this,uchar *param_1,ulong param_2)

{
  undefined4 uVar1;
  FTGlyphContainer *this_00;
  
  *(undefined **)this = PTR_vtable_001c4a34 + 8;
  FTFace::FTFace((FTFace *)(this + 4),param_1,param_2);
  FTSize::FTSize((FTSize *)(this + 0x38));
  *(undefined4 *)(this + 0x5c) = 0;
  FTPoint::FTPoint((FTPoint *)(this + 0x60));
  uVar1 = FTFace::Error();
  *(undefined4 *)(this + 0x58) = uVar1;
  if (*(int *)(this + 0x58) == 0) {
    this_00 = (FTGlyphContainer *)operator_new(0x1c);
    FTGlyphContainer::FTGlyphContainer(this_00,(FTFace *)(this + 4));
    *(FTGlyphContainer **)(this + 0x5c) = this_00;
  }
  return;
}


// ---- __ZN6FTFontD2Ev @ 0x3d3dc (216 bytes)

/* WARNING: Removing unreachable block (ram,0x0003d477) */
/* FTFont::~FTFont() */

void __thiscall FTFont::~FTFont(FTFont *this)

{
  FTGlyphContainer *this_00;
  
  *(undefined **)this = PTR_vtable_001c4a34 + 8;
  this_00 = *(FTGlyphContainer **)(this + 0x5c);
  if (this_00 != (FTGlyphContainer *)0x0) {
    FTGlyphContainer::~FTGlyphContainer(this_00);
    operator_delete(this_00);
  }
  FTSize::~FTSize((FTSize *)(this + 0x38));
  FTFace::~FTFace((FTFace *)(this + 4));
  return;
}


// ---- __ZN6FTFontD1Ev @ 0x3d4b4 (216 bytes)

/* WARNING: Removing unreachable block (ram,0x0003d54f) */
/* FTFont::~FTFont() */

void __thiscall FTFont::~FTFont(FTFont *this)

{
  FTGlyphContainer *this_00;
  
  *(undefined **)this = PTR_vtable_001c4a34 + 8;
  this_00 = *(FTGlyphContainer **)(this + 0x5c);
  if (this_00 != (FTGlyphContainer *)0x0) {
    FTGlyphContainer::~FTGlyphContainer(this_00);
    operator_delete(this_00);
  }
  FTSize::~FTSize((FTSize *)(this + 0x38));
  FTFace::~FTFace((FTFace *)(this + 4));
  return;
}


// ---- __ZN6FTFontD0Ev @ 0x3d58c (216 bytes)

/* FTFont::~FTFont() */

void __thiscall FTFont::~FTFont(FTFont *this)

{
  FTGlyphContainer *this_00;
  
  *(undefined **)this = PTR_vtable_001c4a34 + 8;
  this_00 = *(FTGlyphContainer **)(this + 0x5c);
  if (this_00 != (FTGlyphContainer *)0x0) {
    FTGlyphContainer::~FTGlyphContainer(this_00);
    operator_delete(this_00);
  }
  FTSize::~FTSize((FTSize *)(this + 0x38));
  FTFace::~FTFace((FTFace *)(this + 4));
  operator_delete(this);
  return;
}


// ---- __ZN6FTFont6AttachEPKc @ 0x3d664 (90 bytes)

/* FTFont::Attach(char const*) */

bool __thiscall FTFont::Attach(FTFont *this,char *param_1)

{
  char cVar1;
  undefined4 uVar2;
  
  cVar1 = FTFace::Attach((FTFace *)(this + 4),param_1);
  if (cVar1 == '\0') {
    uVar2 = FTFace::Error();
    *(undefined4 *)(this + 0x58) = uVar2;
  }
  else {
    *(undefined4 *)(this + 0x58) = 0;
  }
  return cVar1 != '\0';
}


// ---- __ZN6FTFont6AttachEPKhm @ 0x3d6be (98 bytes)

/* FTFont::Attach(unsigned char const*, unsigned long) */

bool FTFont::Attach(uchar *param_1,ulong param_2)

{
  char cVar1;
  undefined4 uVar2;
  ulong in_stack_0000000c;
  
  cVar1 = FTFace::Attach((FTFace *)(param_1 + 4),(uchar *)param_2,in_stack_0000000c);
  if (cVar1 == '\0') {
    uVar2 = FTFace::Error();
    *(undefined4 *)(param_1 + 0x58) = uVar2;
  }
  else {
    param_1[0x58] = '\0';
    param_1[0x59] = '\0';
    param_1[0x5a] = '\0';
    param_1[0x5b] = '\0';
  }
  return cVar1 != '\0';
}


// ---- __ZN6FTFont8FaceSizeEjj @ 0x3d720 (244 bytes)

/* FTFont::FaceSize(unsigned int, unsigned int) */

undefined4 __thiscall FTFont::FaceSize(FTFont *this,uint param_1,uint param_2)

{
  FTSize *pFVar1;
  undefined4 uVar2;
  FTGlyphContainer *pFVar3;
  undefined4 local_18;
  
  pFVar1 = (FTSize *)FTFace::Size((FTFace *)(this + 4),param_1,param_2);
  FTSize::operator=((FTSize *)(this + 0x38),pFVar1);
  uVar2 = FTFace::Error();
  *(undefined4 *)(this + 0x58) = uVar2;
  if (*(int *)(this + 0x58) == 0) {
    if ((*(int *)(this + 0x5c) != 0) &&
       (pFVar3 = *(FTGlyphContainer **)(this + 0x5c), pFVar3 != (FTGlyphContainer *)0x0)) {
      FTGlyphContainer::~FTGlyphContainer(pFVar3);
      operator_delete(pFVar3);
    }
    pFVar3 = (FTGlyphContainer *)operator_new(0x1c);
    FTGlyphContainer::FTGlyphContainer(pFVar3,(FTFace *)(this + 4));
    *(FTGlyphContainer **)(this + 0x5c) = pFVar3;
    local_18 = 1;
  }
  else {
    local_18 = 0;
  }
  return local_18;
}


// ---- __ZNK6FTFont8FaceSizeEv @ 0x3d814 (28 bytes)

/* FTFont::FaceSize() const */

void FTFont::FaceSize(void)

{
  int in_stack_00000004;
  
  FTSize::CharSize((FTSize *)(in_stack_00000004 + 0x38));
  return;
}


// ---- __ZN6FTFont7CharMapE12FT_Encoding_ @ 0x3d830 (64 bytes)

/* FTFont::CharMap(FT_Encoding_) */

undefined1 __thiscall FTFont::CharMap(FTFont *this,undefined4 param_2)

{
  undefined1 uVar1;
  undefined4 uVar2;
  
  uVar1 = FTGlyphContainer::CharMap(*(FTGlyphContainer **)(this + 0x5c),param_2);
  uVar2 = FTGlyphContainer::Error();
  *(undefined4 *)(this + 0x58) = uVar2;
  return uVar1;
}


// ---- __ZN6FTFont12CharMapCountEv @ 0x3d870 (28 bytes)

/* FTFont::CharMapCount() */

void FTFont::CharMapCount(void)

{
  int in_stack_00000004;
  
  FTFace::CharMapCount((FTFace *)(in_stack_00000004 + 4));
  return;
}


// ---- __ZN6FTFont11CharMapListEv @ 0x3d88c (28 bytes)

/* FTFont::CharMapList() */

void FTFont::CharMapList(void)

{
  int in_stack_00000004;
  
  FTFace::CharMapList((FTFace *)(in_stack_00000004 + 4));
  return;
}


// ---- __ZN6FTFont14UseDisplayListEb @ 0x3d8a8 (30 bytes)

/* FTFont::UseDisplayList(bool) */

void __thiscall FTFont::UseDisplayList(FTFont *this,bool param_1)

{
  this[0x54] = (FTFont)param_1;
  return;
}


// ---- __ZNK6FTFont8AscenderEv @ 0x3d8c6 (60 bytes)

/* FTFont::Ascender() const */

longdouble __thiscall FTFont::Ascender(FTFont *this)

{
  longdouble lVar1;
  
  lVar1 = (longdouble)FTSize::Ascender((FTSize *)(this + 0x38));
  return (longdouble)(float)lVar1;
}


// ---- __ZNK6FTFont9DescenderEv @ 0x3d902 (60 bytes)

/* FTFont::Descender() const */

longdouble FTFont::Descender(void)

{
  longdouble lVar1;
  int in_stack_00000004;
  
  lVar1 = (longdouble)FTSize::Descender((FTSize *)(in_stack_00000004 + 0x38));
  return (longdouble)(float)lVar1;
}


// ---- __ZNK6FTFont10LineHeightEv @ 0x3d93e (60 bytes)

/* FTFont::LineHeight() const */

longdouble __thiscall FTFont::LineHeight(FTFont *this)

{
  longdouble lVar1;
  
  lVar1 = (longdouble)FTSize::Height((FTSize *)(this + 0x38));
  return (longdouble)(float)lVar1;
}


// ---- __ZN6FTFont6RenderEPKw @ 0x3d97a (292 bytes)

/* FTFont::Render(wchar_t const*) */

void __thiscall FTFont::Render(FTFont *this,wchar_t *param_1)

{
  char cVar1;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  wchar_t *local_20;
  
  local_20 = param_1;
  FTPoint::X((double)ZEXT48(this + 0x60));
  FTPoint::Y((double)ZEXT48(this + 0x60));
  for (; *local_20 != L'\0'; local_20 = local_20 + 1) {
    cVar1 = ::FTFont::CheckGlyph((uint)this);
    if (cVar1 != '\0') {
      FTGlyphContainer::Render
                ((FTGlyphContainer *)&local_4c,*(undefined4 *)(this + 0x5c),*local_20,local_20[1],
                 *(undefined4 *)(this + 0x60),*(undefined4 *)(this + 100),
                 *(undefined4 *)(this + 0x68),*(undefined4 *)(this + 0x6c),
                 *(undefined4 *)(this + 0x70),*(undefined4 *)(this + 0x74));
      *(undefined4 *)(this + 0x60) = local_4c;
      *(undefined4 *)(this + 100) = local_48;
      *(undefined4 *)(this + 0x68) = local_44;
      *(undefined4 *)(this + 0x6c) = local_40;
      *(undefined4 *)(this + 0x70) = local_3c;
      *(undefined4 *)(this + 0x74) = local_38;
    }
  }
  return;
}


// ---- __ZN6FTFont6RenderEPKc @ 0x3da9e (298 bytes)

/* FTFont::Render(char const*) */

void __thiscall FTFont::Render(FTFont *this,char *param_1)

{
  char cVar1;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  char *local_20;
  
  local_20 = param_1;
  FTPoint::X((double)ZEXT48(this + 0x60));
  FTPoint::Y((double)ZEXT48(this + 0x60));
  for (; *local_20 != '\0'; local_20 = local_20 + 1) {
    cVar1 = ::FTFont::CheckGlyph((uint)this);
    if (cVar1 != '\0') {
      FTGlyphContainer::Render
                ((FTGlyphContainer *)&local_4c,*(undefined4 *)(this + 0x5c),*local_20,local_20[1],
                 *(undefined4 *)(this + 0x60),*(undefined4 *)(this + 100),
                 *(undefined4 *)(this + 0x68),*(undefined4 *)(this + 0x6c),
                 *(undefined4 *)(this + 0x70),*(undefined4 *)(this + 0x74));
      *(undefined4 *)(this + 0x60) = local_4c;
      *(undefined4 *)(this + 100) = local_48;
      *(undefined4 *)(this + 0x68) = local_44;
      *(undefined4 *)(this + 0x6c) = local_40;
      *(undefined4 *)(this + 0x70) = local_3c;
      *(undefined4 *)(this + 0x74) = local_38;
    }
  }
  return;
}


// ---- __ZN6FTFont7AdvanceEPKc @ 0x3dbc8 (166 bytes)

/* FTFont::Advance(char const*) */

longdouble __thiscall FTFont::Advance(FTFont *this,char *param_1)

{
  char cVar1;
  longdouble lVar2;
  byte *pbStack_14;
  float fStack_10;
  
  fStack_10 = 0.0;
  for (pbStack_14 = (byte *)param_1; *pbStack_14 != 0; pbStack_14 = pbStack_14 + 1) {
    cVar1 = ::FTFont::CheckGlyph((uint)this);
    if (cVar1 != '\0') {
      lVar2 = (longdouble)
              FTGlyphContainer::Advance
                        (*(FTGlyphContainer **)(this + 0x5c),(uint)*pbStack_14,(uint)pbStack_14[1]);
      fStack_10 = fStack_10 + (float)lVar2;
    }
  }
  return (longdouble)fStack_10;
}


// ---- __ZN6FTFont7AdvanceEPKw @ 0x3dc6e (160 bytes)

/* FTFont::Advance(wchar_t const*) */

longdouble __thiscall FTFont::Advance(FTFont *this,wchar_t *param_1)

{
  char cVar1;
  longdouble lVar2;
  wchar_t *pwStack_14;
  float fStack_10;
  
  fStack_10 = 0.0;
  for (pwStack_14 = param_1; *pwStack_14 != L'\0'; pwStack_14 = pwStack_14 + 1) {
    cVar1 = ::FTFont::CheckGlyph((uint)this);
    if (cVar1 != '\0') {
      lVar2 = (longdouble)
              FTGlyphContainer::Advance
                        (*(FTGlyphContainer **)(this + 0x5c),*pwStack_14,pwStack_14[1]);
      fStack_10 = fStack_10 + (float)lVar2;
    }
  }
  return (longdouble)fStack_10;
}


// ---- __ZN6FTFont4BBoxEPKwRfS2_S2_S2_S2_S2_ @ 0x3dd0e (634 bytes)

/* FTFont::BBox(wchar_t const*, float&, float&, float&, float&, float&, float&) */

void __thiscall
FTFont::BBox(FTFont *this,wchar_t *param_1,float *param_2,float *param_3,float *param_4,
            float *param_5,float *param_6,float *param_7)

{
  char cVar1;
  longdouble lVar2;
  FTGlyphContainer aFStack_74 [24];
  float fStack_5c;
  float fStack_58;
  float fStack_54;
  float fStack_50;
  float fStack_4c;
  float fStack_48;
  float fStack_44;
  float fStack_40;
  float fStack_3c;
  float fStack_38;
  float fStack_34;
  float fStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  wchar_t *pwStack_14;
  float fStack_10;
  
  FTBBox::FTBBox((FTBBox *)&fStack_5c);
  if ((param_1 != (wchar_t *)0x0) && (*param_1 != L'\0')) {
    pwStack_14 = param_1;
    fStack_10 = 0.0;
    cVar1 = ::FTFont::CheckGlyph((uint)this);
    if (cVar1 != '\0') {
      FTGlyphContainer::BBox((FTGlyphContainer *)&fStack_44,*(uint *)(this + 0x5c));
      fStack_5c = fStack_44;
      fStack_58 = fStack_40;
      fStack_54 = fStack_3c;
      fStack_50 = fStack_38;
      fStack_4c = fStack_34;
      fStack_48 = fStack_30;
      FTBBox::~FTBBox((FTBBox *)&fStack_44);
      lVar2 = (longdouble)
              FTGlyphContainer::Advance
                        (*(FTGlyphContainer **)(this + 0x5c),*pwStack_14,pwStack_14[1]);
      fStack_10 = (float)lVar2;
    }
    while (pwStack_14 = pwStack_14 + 1, *pwStack_14 != L'\0') {
      cVar1 = ::FTFont::CheckGlyph((uint)this);
      if (cVar1 != '\0') {
        FTGlyphContainer::BBox(aFStack_74,*(uint *)(this + 0x5c));
        FTPoint::FTPoint((FTPoint *)&uStack_2c,(double)fStack_10,0.0,0.0);
        FTBBox::Move(aFStack_74,uStack_2c,uStack_28,uStack_24,uStack_20,uStack_1c,uStack_18);
        FTBBox::operator+=((FTBBox *)&fStack_5c,(FTBBox *)aFStack_74);
        lVar2 = (longdouble)
                FTGlyphContainer::Advance
                          (*(FTGlyphContainer **)(this + 0x5c),*pwStack_14,pwStack_14[1]);
        fStack_10 = fStack_10 + (float)lVar2;
        FTBBox::~FTBBox((FTBBox *)aFStack_74);
      }
    }
  }
  *param_2 = fStack_5c;
  *param_3 = fStack_58;
  *param_4 = fStack_54;
  *param_5 = fStack_50;
  *param_6 = fStack_4c;
  *param_7 = fStack_48;
  FTBBox::~FTBBox((FTBBox *)&fStack_5c);
  return;
}


// ---- __ZN6FTFont4BBoxEPKcRfS2_S2_S2_S2_S2_ @ 0x3df88 (650 bytes)

/* FTFont::BBox(char const*, float&, float&, float&, float&, float&, float&) */

void __thiscall
FTFont::BBox(FTFont *this,char *param_1,float *param_2,float *param_3,float *param_4,float *param_5,
            float *param_6,float *param_7)

{
  char cVar1;
  longdouble lVar2;
  FTGlyphContainer local_74 [24];
  float local_5c;
  float local_58;
  float local_54;
  float local_50;
  float local_4c;
  float local_48;
  float local_44;
  float local_40;
  float local_3c;
  float local_38;
  float local_34;
  float local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  byte *local_14;
  float local_10;
  
  FTBBox::FTBBox((FTBBox *)&local_5c);
  if ((param_1 != (char *)0x0) && (*param_1 != '\0')) {
    local_14 = (byte *)param_1;
    local_10 = 0.0;
    cVar1 = ::FTFont::CheckGlyph((uint)this);
    if (cVar1 != '\0') {
      FTGlyphContainer::BBox((FTGlyphContainer *)&local_44,*(uint *)(this + 0x5c));
      local_5c = local_44;
      local_58 = local_40;
      local_54 = local_3c;
      local_50 = local_38;
      local_4c = local_34;
      local_48 = local_30;
      FTBBox::~FTBBox((FTBBox *)&local_44);
      lVar2 = (longdouble)
              FTGlyphContainer::Advance
                        (*(FTGlyphContainer **)(this + 0x5c),(uint)*local_14,(uint)local_14[1]);
      local_10 = (float)lVar2;
    }
    while (local_14 = local_14 + 1, *local_14 != 0) {
      cVar1 = ::FTFont::CheckGlyph((uint)this);
      if (cVar1 != '\0') {
        FTGlyphContainer::BBox(local_74,*(uint *)(this + 0x5c));
        FTPoint::FTPoint((FTPoint *)&local_2c,(double)local_10,0.0,0.0);
        FTBBox::Move(local_74,local_2c,local_28,local_24,local_20,local_1c,local_18);
        FTBBox::operator+=((FTBBox *)&local_5c,(FTBBox *)local_74);
        lVar2 = (longdouble)
                FTGlyphContainer::Advance
                          (*(FTGlyphContainer **)(this + 0x5c),(uint)*local_14,(uint)local_14[1]);
        local_10 = local_10 + (float)lVar2;
        FTBBox::~FTBBox((FTBBox *)local_74);
      }
    }
  }
  *param_2 = local_5c;
  *param_3 = local_58;
  *param_4 = local_54;
  *param_5 = local_50;
  *param_6 = local_4c;
  *param_7 = local_48;
  FTBBox::~FTBBox((FTBBox *)&local_5c);
  return;
}


// ---- __ZN7FTPoint1XEd @ 0x3e212 (38 bytes)

/* FTPoint::X(double) */

void FTPoint::X(double param_1)

{
  undefined4 in_stack_0000000c;
  
  *param_1._0_4_ = CONCAT44(in_stack_0000000c,param_1._4_4_);
  return;
}


// ---- __ZN7FTPoint1YEd @ 0x3e238 (40 bytes)

/* FTPoint::Y(double) */

void FTPoint::Y(double param_1)

{
  undefined4 in_stack_0000000c;
  
  *(ulonglong *)(param_1._0_4_ + 8) = CONCAT44(in_stack_0000000c,param_1._4_4_);
  return;
}


// ---- __ZNK7FTPoint1ZEv @ 0x3e260 (30 bytes)

/* FTPoint::Z() const */

longdouble __thiscall FTPoint::Z(FTPoint *this)

{
  return (longdouble)*(double *)(this + 0x10);
}


// ---- __ZNK6FTFace5ErrorEv @ 0x3e27e (20 bytes)

/* FTFace::Error() const */

undefined4 __thiscall FTFace::Error(FTFace *this)

{
  return *(undefined4 *)(this + 0x30);
}


// ---- __ZN6FTFont5DepthEf @ 0x3e292 (14 bytes)

/* FTFont::Depth(float) */

void FTFont::Depth(float param_1)

{
  return;
}


// ---- __ZN6FTBBoxC1Ev @ 0x3e2a0 (104 bytes)

/* FTBBox::FTBBox() */

void __thiscall FTBBox::FTBBox(FTBBox *this)

{
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  return;
}


// ---- __ZN6FTBBoxD1Ev @ 0x3e308 (14 bytes)

/* FTBBox::~FTBBox() */

void __thiscall FTBBox::~FTBBox(FTBBox *this)

{
  return;
}


// ---- __ZN6FTBBox4MoveE7FTPoint @ 0x3e316 (334 bytes)

/* FTBBox::Move(FTPoint) */

float * FTBBox::Move(float *param_1)

{
  float fVar1;
  longdouble lVar2;
  
  fVar1 = *param_1;
  lVar2 = (longdouble)FTPoint::X();
  *param_1 = fVar1 + (float)lVar2;
  fVar1 = param_1[1];
  lVar2 = (longdouble)FTPoint::Y();
  param_1[1] = fVar1 + (float)lVar2;
  fVar1 = param_1[2];
  lVar2 = (longdouble)FTPoint::Z();
  param_1[2] = fVar1 + (float)lVar2;
  fVar1 = param_1[3];
  lVar2 = (longdouble)FTPoint::X();
  param_1[3] = fVar1 + (float)lVar2;
  fVar1 = param_1[4];
  lVar2 = (longdouble)FTPoint::Y();
  param_1[4] = fVar1 + (float)lVar2;
  fVar1 = param_1[5];
  lVar2 = (longdouble)FTPoint::Z();
  param_1[5] = fVar1 + (float)lVar2;
  return param_1;
}


// ---- __ZN6FTBBoxpLERKS_ @ 0x3e464 (396 bytes)

/* FTBBox::TEMPNAMEPLACEHOLDERVALUE(FTBBox const&) */

FTBBox * __thiscall FTBBox::operator+=(FTBBox *this,FTBBox *param_1)

{
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  
  if (*(float *)param_1 < *(float *)this) {
    uStack_24 = *(undefined4 *)param_1;
  }
  else {
    uStack_24 = *(undefined4 *)this;
  }
  *(undefined4 *)this = uStack_24;
  if (*(float *)(param_1 + 4) < *(float *)(this + 4)) {
    uStack_20 = *(undefined4 *)(param_1 + 4);
  }
  else {
    uStack_20 = *(undefined4 *)(this + 4);
  }
  *(undefined4 *)(this + 4) = uStack_20;
  if (*(float *)(param_1 + 8) < *(float *)(this + 8)) {
    uStack_1c = *(undefined4 *)(param_1 + 8);
  }
  else {
    uStack_1c = *(undefined4 *)(this + 8);
  }
  *(undefined4 *)(this + 8) = uStack_1c;
  if (*(float *)(this + 0xc) < *(float *)(param_1 + 0xc)) {
    uStack_18 = *(undefined4 *)(param_1 + 0xc);
  }
  else {
    uStack_18 = *(undefined4 *)(this + 0xc);
  }
  *(undefined4 *)(this + 0xc) = uStack_18;
  if (*(float *)(this + 0x10) < *(float *)(param_1 + 0x10)) {
    uStack_14 = *(undefined4 *)(param_1 + 0x10);
  }
  else {
    uStack_14 = *(undefined4 *)(this + 0x10);
  }
  *(undefined4 *)(this + 0x10) = uStack_14;
  if (*(float *)(this + 0x14) < *(float *)(param_1 + 0x14)) {
    uStack_10 = *(undefined4 *)(param_1 + 0x14);
  }
  else {
    uStack_10 = *(undefined4 *)(this + 0x14);
  }
  *(undefined4 *)(this + 0x14) = uStack_10;
  return this;
}


// ---- __ZNK16FTGlyphContainer5ErrorEv @ 0x3e5f0 (20 bytes)

/* FTGlyphContainer::Error() const */

undefined4 __thiscall FTGlyphContainer::Error(FTGlyphContainer *this)

{
  return *(undefined4 *)(this + 0x18);
}


// ---- __ZN6FTSizeaSERKS_ @ 0x3e604 (90 bytes)

/* FTSize::TEMPNAMEPLACEHOLDERVALUE(FTSize const&) */

FTSize * __thiscall FTSize::operator=(FTSize *this,FTSize *param_1)

{
  *(undefined4 *)(this + 4) = *(undefined4 *)(param_1 + 4);
  *(undefined4 *)(this + 8) = *(undefined4 *)(param_1 + 8);
  *(undefined4 *)(this + 0xc) = *(undefined4 *)(param_1 + 0xc);
  *(undefined4 *)(this + 0x10) = *(undefined4 *)(param_1 + 0x10);
  *(undefined4 *)(this + 0x14) = *(undefined4 *)(param_1 + 0x14);
  *(undefined4 *)(this + 0x18) = *(undefined4 *)(param_1 + 0x18);
  return this;
}


// ---- __ZN6FTFont10CheckGlyphEj @ 0x3e65e (169 bytes)

/* FTFont::CheckGlyph(unsigned int) */

undefined4 FTFont::CheckGlyph(uint param_1)

{
  int iVar1;
  undefined4 uVar2;
  FTGlyph *pFVar3;
  uint in_stack_00000008;
  
  iVar1 = FTGlyphContainer::Glyph(*(uint *)(param_1 + 0x5c));
  if (iVar1 == 0) {
    uVar2 = FTGlyphContainer::FontIndex(*(FTGlyphContainer **)(param_1 + 0x5c),in_stack_00000008);
    pFVar3 = (FTGlyph *)(**(code **)(*(int *)param_1 + 0x18))(param_1,uVar2);
    if (pFVar3 == (FTGlyph *)0x0) {
      if (*(int *)(param_1 + 0x58) == 0) {
        *(undefined4 *)(param_1 + 0x58) = 0x13;
      }
      return 0;
    }
    FTGlyphContainer::Add(*(FTGlyphContainer **)(param_1 + 0x5c),pFVar3,in_stack_00000008);
  }
  return 1;
}


