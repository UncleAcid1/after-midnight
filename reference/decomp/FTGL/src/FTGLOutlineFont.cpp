// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTGLOutlineFont.cpp

// ---- __ZN15FTGLOutlineFontC2EPKc @ 0x3ed54 (60 bytes)

/* FTGLOutlineFont::FTGLOutlineFont(char const*) */

void __thiscall FTGLOutlineFont::FTGLOutlineFont(FTGLOutlineFont *this,char *param_1)

{
  FTFont::FTFont((FTFont *)this,param_1);
  *(undefined **)this = PTR_vtable_001c4a40 + 8;
  return;
}


// ---- __ZN15FTGLOutlineFontC1EPKc @ 0x3ed90 (60 bytes)

/* FTGLOutlineFont::FTGLOutlineFont(char const*) */

void __thiscall FTGLOutlineFont::FTGLOutlineFont(FTGLOutlineFont *this,char *param_1)

{
  FTFont::FTFont((FTFont *)this,param_1);
  *(undefined **)this = PTR_vtable_001c4a40 + 8;
  return;
}


// ---- __ZN15FTGLOutlineFontC2EPKhm @ 0x3edcc (66 bytes)

/* FTGLOutlineFont::FTGLOutlineFont(unsigned char const*, unsigned long) */

void __thiscall FTGLOutlineFont::FTGLOutlineFont(FTGLOutlineFont *this,uchar *param_1,ulong param_2)

{
  FTFont::FTFont((FTFont *)this,param_1,param_2);
  *(undefined **)this = PTR_vtable_001c4a40 + 8;
  return;
}


// ---- __ZN15FTGLOutlineFontC1EPKhm @ 0x3ee0e (66 bytes)

/* FTGLOutlineFont::FTGLOutlineFont(unsigned char const*, unsigned long) */

void __thiscall FTGLOutlineFont::FTGLOutlineFont(FTGLOutlineFont *this,uchar *param_1,ulong param_2)

{
  FTFont::FTFont((FTFont *)this,param_1,param_2);
  *(undefined **)this = PTR_vtable_001c4a40 + 8;
  return;
}


// ---- __ZN15FTGLOutlineFontD2Ev @ 0x3ee50 (72 bytes)

/* WARNING: Removing unreachable block (ram,0x0003ee87) */
/* FTGLOutlineFont::~FTGLOutlineFont() */

void __thiscall FTGLOutlineFont::~FTGLOutlineFont(FTGLOutlineFont *this)

{
  *(undefined **)this = PTR_vtable_001c4a40 + 8;
  FTFont::~FTFont((FTFont *)this);
  return;
}


// ---- __ZN15FTGLOutlineFontD1Ev @ 0x3ee98 (72 bytes)

/* WARNING: Removing unreachable block (ram,0x0003eecf) */
/* FTGLOutlineFont::~FTGLOutlineFont() */

void __thiscall FTGLOutlineFont::~FTGLOutlineFont(FTGLOutlineFont *this)

{
  *(undefined **)this = PTR_vtable_001c4a40 + 8;
  FTFont::~FTFont((FTFont *)this);
  return;
}


// ---- __ZN15FTGLOutlineFontD0Ev @ 0x3eee0 (72 bytes)

/* FTGLOutlineFont::~FTGLOutlineFont() */

void __thiscall FTGLOutlineFont::~FTGLOutlineFont(FTGLOutlineFont *this)

{
  *(undefined **)this = PTR_vtable_001c4a40 + 8;
  FTFont::~FTFont((FTFont *)this);
  operator_delete(this);
  return;
}


// ---- __ZN15FTGLOutlineFont6RenderEPKc @ 0x3ef28 (126 bytes)

/* FTGLOutlineFont::Render(char const*) */

void __thiscall FTGLOutlineFont::Render(FTGLOutlineFont *this,char *param_1)

{
  _glPushAttrib(0xe004);
  _glDisable(0xde1);
  _glEnable(0xb20);
  _glHint(0xc52,0x1100);
  _glEnable(0xbe2);
  _glBlendFunc(0x302,0x303);
  FTFont::Render((FTFont *)this,param_1);
  _glPopAttrib();
  return;
}


// ---- __ZN15FTGLOutlineFont6RenderEPKw @ 0x3efa6 (125 bytes)

/* FTGLOutlineFont::Render(wchar_t const*) */

void __thiscall FTGLOutlineFont::Render(FTGLOutlineFont *this,wchar_t *param_1)

{
  _glPushAttrib(0xe004);
  _glDisable(0xde1);
  _glEnable(0xb20);
  _glHint(0xc52,0x1100);
  _glEnable(0xbe2);
  _glBlendFunc(0x302,0x303);
  FTFont::Render((FTFont *)this,param_1);
  _glPopAttrib();
  return;
}


// ---- __ZN15FTGLOutlineFont9MakeGlyphEj @ 0x3f024 (181 bytes)

/* FTGLOutlineFont::MakeGlyph(unsigned int) */

FTOutlineGlyph * __thiscall FTGLOutlineFont::MakeGlyph(FTGLOutlineFont *this,uint param_1)

{
  FT_GlyphSlotRec_ *pFVar1;
  undefined4 uVar2;
  FTOutlineGlyph *pFStack_20;
  
  pFVar1 = (FT_GlyphSlotRec_ *)FTFace::Glyph((FTFace *)(this + 4),param_1,2);
  if (pFVar1 == (FT_GlyphSlotRec_ *)0x0) {
    uVar2 = FTFace::Error();
    *(undefined4 *)(this + 0x58) = uVar2;
    pFStack_20 = (FTOutlineGlyph *)0x0;
  }
  else {
    pFStack_20 = (FTOutlineGlyph *)operator_new(0x40);
    FTOutlineGlyph::FTOutlineGlyph(pFStack_20,pFVar1,(bool)this[0x54]);
  }
  return pFStack_20;
}


