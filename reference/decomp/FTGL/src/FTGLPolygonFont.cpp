// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTGLPolygonFont.cpp

// ---- __ZN15FTGLPolygonFontC2EPKc @ 0x3f4f2 (60 bytes)

/* FTGLPolygonFont::FTGLPolygonFont(char const*) */

void __thiscall FTGLPolygonFont::FTGLPolygonFont(FTGLPolygonFont *this,char *param_1)

{
  FTFont::FTFont((FTFont *)this,param_1);
  *(undefined **)this = PTR_vtable_001c4a48 + 8;
  return;
}


// ---- __ZN15FTGLPolygonFontC1EPKc @ 0x3f52e (60 bytes)

/* FTGLPolygonFont::FTGLPolygonFont(char const*) */

void __thiscall FTGLPolygonFont::FTGLPolygonFont(FTGLPolygonFont *this,char *param_1)

{
  FTFont::FTFont((FTFont *)this,param_1);
  *(undefined **)this = PTR_vtable_001c4a48 + 8;
  return;
}


// ---- __ZN15FTGLPolygonFontC2EPKhm @ 0x3f56a (66 bytes)

/* FTGLPolygonFont::FTGLPolygonFont(unsigned char const*, unsigned long) */

void __thiscall FTGLPolygonFont::FTGLPolygonFont(FTGLPolygonFont *this,uchar *param_1,ulong param_2)

{
  FTFont::FTFont((FTFont *)this,param_1,param_2);
  *(undefined **)this = PTR_vtable_001c4a48 + 8;
  return;
}


// ---- __ZN15FTGLPolygonFontC1EPKhm @ 0x3f5ac (66 bytes)

/* FTGLPolygonFont::FTGLPolygonFont(unsigned char const*, unsigned long) */

void __thiscall FTGLPolygonFont::FTGLPolygonFont(FTGLPolygonFont *this,uchar *param_1,ulong param_2)

{
  FTFont::FTFont((FTFont *)this,param_1,param_2);
  *(undefined **)this = PTR_vtable_001c4a48 + 8;
  return;
}


// ---- __ZN15FTGLPolygonFontD2Ev @ 0x3f5ee (72 bytes)

/* WARNING: Removing unreachable block (ram,0x0003f625) */
/* FTGLPolygonFont::~FTGLPolygonFont() */

void __thiscall FTGLPolygonFont::~FTGLPolygonFont(FTGLPolygonFont *this)

{
  *(undefined **)this = PTR_vtable_001c4a48 + 8;
  FTFont::~FTFont((FTFont *)this);
  return;
}


// ---- __ZN15FTGLPolygonFontD1Ev @ 0x3f636 (72 bytes)

/* WARNING: Removing unreachable block (ram,0x0003f66d) */
/* FTGLPolygonFont::~FTGLPolygonFont() */

void __thiscall FTGLPolygonFont::~FTGLPolygonFont(FTGLPolygonFont *this)

{
  *(undefined **)this = PTR_vtable_001c4a48 + 8;
  FTFont::~FTFont((FTFont *)this);
  return;
}


// ---- __ZN15FTGLPolygonFontD0Ev @ 0x3f67e (72 bytes)

/* FTGLPolygonFont::~FTGLPolygonFont() */

void __thiscall FTGLPolygonFont::~FTGLPolygonFont(FTGLPolygonFont *this)

{
  *(undefined **)this = PTR_vtable_001c4a48 + 8;
  FTFont::~FTFont((FTFont *)this);
  operator_delete(this);
  return;
}


// ---- __ZN15FTGLPolygonFont9MakeGlyphEj @ 0x3f6c6 (181 bytes)

/* FTGLPolygonFont::MakeGlyph(unsigned int) */

FTPolyGlyph * __thiscall FTGLPolygonFont::MakeGlyph(FTGLPolygonFont *this,uint param_1)

{
  FT_GlyphSlotRec_ *pFVar1;
  undefined4 uVar2;
  FTPolyGlyph *pFStack_20;
  
  pFVar1 = (FT_GlyphSlotRec_ *)FTFace::Glyph((FTFace *)(this + 4),param_1,2);
  if (pFVar1 == (FT_GlyphSlotRec_ *)0x0) {
    uVar2 = FTFace::Error();
    *(undefined4 *)(this + 0x58) = uVar2;
    pFStack_20 = (FTPolyGlyph *)0x0;
  }
  else {
    pFStack_20 = (FTPolyGlyph *)operator_new(0x40);
    FTPolyGlyph::FTPolyGlyph(pFStack_20,pFVar1,(bool)this[0x54]);
  }
  return pFStack_20;
}


