// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTGLPixmapFont.cpp

// ---- __ZN14FTGLPixmapFontC2EPKc @ 0x3f0da (60 bytes)

/* FTGLPixmapFont::FTGLPixmapFont(char const*) */

void __thiscall FTGLPixmapFont::FTGLPixmapFont(FTGLPixmapFont *this,char *param_1)

{
  FTFont::FTFont((FTFont *)this,param_1);
  *(undefined **)this = PTR_vtable_001c4a44 + 8;
  return;
}


// ---- __ZN14FTGLPixmapFontC1EPKc @ 0x3f116 (60 bytes)

/* FTGLPixmapFont::FTGLPixmapFont(char const*) */

void __thiscall FTGLPixmapFont::FTGLPixmapFont(FTGLPixmapFont *this,char *param_1)

{
  FTFont::FTFont((FTFont *)this,param_1);
  *(undefined **)this = PTR_vtable_001c4a44 + 8;
  return;
}


// ---- __ZN14FTGLPixmapFontC2EPKhm @ 0x3f152 (66 bytes)

/* FTGLPixmapFont::FTGLPixmapFont(unsigned char const*, unsigned long) */

void __thiscall FTGLPixmapFont::FTGLPixmapFont(FTGLPixmapFont *this,uchar *param_1,ulong param_2)

{
  FTFont::FTFont((FTFont *)this,param_1,param_2);
  *(undefined **)this = PTR_vtable_001c4a44 + 8;
  return;
}


// ---- __ZN14FTGLPixmapFontC1EPKhm @ 0x3f194 (66 bytes)

/* FTGLPixmapFont::FTGLPixmapFont(unsigned char const*, unsigned long) */

void __thiscall FTGLPixmapFont::FTGLPixmapFont(FTGLPixmapFont *this,uchar *param_1,ulong param_2)

{
  FTFont::FTFont((FTFont *)this,param_1,param_2);
  *(undefined **)this = PTR_vtable_001c4a44 + 8;
  return;
}


// ---- __ZN14FTGLPixmapFontD2Ev @ 0x3f1d6 (72 bytes)

/* WARNING: Removing unreachable block (ram,0x0003f20d) */
/* FTGLPixmapFont::~FTGLPixmapFont() */

void __thiscall FTGLPixmapFont::~FTGLPixmapFont(FTGLPixmapFont *this)

{
  *(undefined **)this = PTR_vtable_001c4a44 + 8;
  FTFont::~FTFont((FTFont *)this);
  return;
}


// ---- __ZN14FTGLPixmapFontD1Ev @ 0x3f21e (72 bytes)

/* WARNING: Removing unreachable block (ram,0x0003f255) */
/* FTGLPixmapFont::~FTGLPixmapFont() */

void __thiscall FTGLPixmapFont::~FTGLPixmapFont(FTGLPixmapFont *this)

{
  *(undefined **)this = PTR_vtable_001c4a44 + 8;
  FTFont::~FTFont((FTFont *)this);
  return;
}


// ---- __ZN14FTGLPixmapFontD0Ev @ 0x3f266 (72 bytes)

/* FTGLPixmapFont::~FTGLPixmapFont() */

void __thiscall FTGLPixmapFont::~FTGLPixmapFont(FTGLPixmapFont *this)

{
  *(undefined **)this = PTR_vtable_001c4a44 + 8;
  FTFont::~FTFont((FTFont *)this);
  operator_delete(this);
  return;
}


// ---- __ZN14FTGLPixmapFont6RenderEPKc @ 0x3f2ae (206 bytes)

/* FTGLPixmapFont::Render(char const*) */

void __thiscall FTGLPixmapFont::Render(FTGLPixmapFont *this,char *param_1)

{
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  
  _glPushAttrib(0x6020);
  _glPushClientAttrib(1);
  _glEnable(0xbe2);
  _glBlendFunc(0x302,0x303);
  _glDisable(0xde1);
  _glGetFloatv(0xb04,&uStack_1c);
  _glPixelTransferf(0xd14,uStack_1c);
  _glPixelTransferf(0xd18,uStack_18);
  _glPixelTransferf(0xd1a,uStack_14);
  _glPixelTransferf(0xd1c,uStack_10);
  FTFont::Render((FTFont *)this,param_1);
  _glPopClientAttrib();
  _glPopAttrib();
  return;
}


// ---- __ZN14FTGLPixmapFont6RenderEPKw @ 0x3f37c (205 bytes)

/* FTGLPixmapFont::Render(wchar_t const*) */

void __thiscall FTGLPixmapFont::Render(FTGLPixmapFont *this,wchar_t *param_1)

{
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  
  _glPushAttrib(0x6020);
  _glPushClientAttrib(1);
  _glEnable(0xbe2);
  _glBlendFunc(0x302,0x303);
  _glDisable(0xde1);
  _glGetFloatv(0xb04,&uStack_1c);
  _glPixelTransferf(0xd14,uStack_1c);
  _glPixelTransferf(0xd18,uStack_18);
  _glPixelTransferf(0xd1a,uStack_14);
  _glPixelTransferf(0xd1c,uStack_10);
  FTFont::Render((FTFont *)this,param_1);
  _glPopClientAttrib();
  _glPopAttrib();
  return;
}


// ---- __ZN14FTGLPixmapFont9MakeGlyphEj @ 0x3f44a (167 bytes)

/* FTGLPixmapFont::MakeGlyph(unsigned int) */

FTPixmapGlyph * __thiscall FTGLPixmapFont::MakeGlyph(FTGLPixmapFont *this,uint param_1)

{
  FT_GlyphSlotRec_ *pFVar1;
  undefined4 uVar2;
  FTPixmapGlyph *pFStack_20;
  
  pFVar1 = (FT_GlyphSlotRec_ *)FTFace::Glyph((FTFace *)(this + 4),param_1,2);
  if (pFVar1 == (FT_GlyphSlotRec_ *)0x0) {
    uVar2 = FTFace::Error();
    *(undefined4 *)(this + 0x58) = uVar2;
    pFStack_20 = (FTPixmapGlyph *)0x0;
  }
  else {
    pFStack_20 = (FTPixmapGlyph *)operator_new(0x60);
    FTPixmapGlyph::FTPixmapGlyph(pFStack_20,pFVar1);
  }
  return pFStack_20;
}


