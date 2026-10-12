// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTGLBitmapFont.cpp

// ---- __ZN14FTGLBitmapFontC2EPKc @ 0x3e708 (60 bytes)

/* FTGLBitmapFont::FTGLBitmapFont(char const*) */

void __thiscall FTGLBitmapFont::FTGLBitmapFont(FTGLBitmapFont *this,char *param_1)

{
  FTFont::FTFont((FTFont *)this,param_1);
  *(undefined **)this = PTR_vtable_001c4a38 + 8;
  return;
}


// ---- __ZN14FTGLBitmapFontC1EPKc @ 0x3e744 (60 bytes)

/* FTGLBitmapFont::FTGLBitmapFont(char const*) */

void __thiscall FTGLBitmapFont::FTGLBitmapFont(FTGLBitmapFont *this,char *param_1)

{
  FTFont::FTFont((FTFont *)this,param_1);
  *(undefined **)this = PTR_vtable_001c4a38 + 8;
  return;
}


// ---- __ZN14FTGLBitmapFontC2EPKhm @ 0x3e780 (66 bytes)

/* FTGLBitmapFont::FTGLBitmapFont(unsigned char const*, unsigned long) */

void __thiscall FTGLBitmapFont::FTGLBitmapFont(FTGLBitmapFont *this,uchar *param_1,ulong param_2)

{
  FTFont::FTFont((FTFont *)this,param_1,param_2);
  *(undefined **)this = PTR_vtable_001c4a38 + 8;
  return;
}


// ---- __ZN14FTGLBitmapFontC1EPKhm @ 0x3e7c2 (66 bytes)

/* FTGLBitmapFont::FTGLBitmapFont(unsigned char const*, unsigned long) */

void __thiscall FTGLBitmapFont::FTGLBitmapFont(FTGLBitmapFont *this,uchar *param_1,ulong param_2)

{
  FTFont::FTFont((FTFont *)this,param_1,param_2);
  *(undefined **)this = PTR_vtable_001c4a38 + 8;
  return;
}


// ---- __ZN14FTGLBitmapFontD2Ev @ 0x3e804 (72 bytes)

/* WARNING: Removing unreachable block (ram,0x0003e83b) */
/* FTGLBitmapFont::~FTGLBitmapFont() */

void __thiscall FTGLBitmapFont::~FTGLBitmapFont(FTGLBitmapFont *this)

{
  *(undefined **)this = PTR_vtable_001c4a38 + 8;
  FTFont::~FTFont((FTFont *)this);
  return;
}


// ---- __ZN14FTGLBitmapFontD1Ev @ 0x3e84c (72 bytes)

/* WARNING: Removing unreachable block (ram,0x0003e883) */
/* FTGLBitmapFont::~FTGLBitmapFont() */

void __thiscall FTGLBitmapFont::~FTGLBitmapFont(FTGLBitmapFont *this)

{
  *(undefined **)this = PTR_vtable_001c4a38 + 8;
  FTFont::~FTFont((FTFont *)this);
  return;
}


// ---- __ZN14FTGLBitmapFontD0Ev @ 0x3e894 (72 bytes)

/* FTGLBitmapFont::~FTGLBitmapFont() */

void __thiscall FTGLBitmapFont::~FTGLBitmapFont(FTGLBitmapFont *this)

{
  *(undefined **)this = PTR_vtable_001c4a38 + 8;
  FTFont::~FTFont((FTFont *)this);
  operator_delete(this);
  return;
}


// ---- __ZN14FTGLBitmapFont6RenderEPKc @ 0x3e8dc (118 bytes)

/* FTGLBitmapFont::Render(char const*) */

void __thiscall FTGLBitmapFont::Render(FTGLBitmapFont *this,char *param_1)

{
  _glPushClientAttrib(1);
  _glPushAttrib(0x2000);
  _glPixelStorei(0xcf1,0);
  _glPixelStorei(0xcf5,1);
  _glDisable(0xbe2);
  FTFont::Render((FTFont *)this,param_1);
  _glPopAttrib();
  _glPopClientAttrib();
  return;
}


// ---- __ZN14FTGLBitmapFont6RenderEPKw @ 0x3e952 (118 bytes)

/* FTGLBitmapFont::Render(wchar_t const*) */

void __thiscall FTGLBitmapFont::Render(FTGLBitmapFont *this,wchar_t *param_1)

{
  _glPushClientAttrib(1);
  _glPushAttrib(0x2000);
  _glPixelStorei(0xcf1,0);
  _glPixelStorei(0xcf5,1);
  _glDisable(0xbe2);
  FTFont::Render((FTFont *)this,param_1);
  _glPopAttrib();
  _glPopClientAttrib();
  return;
}


// ---- __ZN14FTGLBitmapFont9MakeGlyphEj @ 0x3e9c8 (167 bytes)

/* FTGLBitmapFont::MakeGlyph(unsigned int) */

FTBitmapGlyph * __thiscall FTGLBitmapFont::MakeGlyph(FTGLBitmapFont *this,uint param_1)

{
  FT_GlyphSlotRec_ *pFVar1;
  undefined4 uVar2;
  FTBitmapGlyph *pFStack_20;
  
  pFVar1 = (FT_GlyphSlotRec_ *)FTFace::Glyph((FTFace *)(this + 4),param_1,0);
  if (pFVar1 == (FT_GlyphSlotRec_ *)0x0) {
    uVar2 = FTFace::Error();
    *(undefined4 *)(this + 0x58) = uVar2;
    pFStack_20 = (FTBitmapGlyph *)0x0;
  }
  else {
    pFStack_20 = (FTBitmapGlyph *)operator_new(100);
    FTBitmapGlyph::FTBitmapGlyph(pFStack_20,pFVar1);
  }
  return pFStack_20;
}


