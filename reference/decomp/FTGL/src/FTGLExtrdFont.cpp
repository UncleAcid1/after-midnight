// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTGLExtrdFont.cpp

// ---- __ZN13FTGLExtrdFontC2EPKc @ 0x3ea70 (74 bytes)

/* FTGLExtrdFont::FTGLExtrdFont(char const*) */

void __thiscall FTGLExtrdFont::FTGLExtrdFont(FTGLExtrdFont *this,char *param_1)

{
  FTFont::FTFont((FTFont *)this,param_1);
  *(undefined **)this = PTR_vtable_001c4a3c + 8;
  *(undefined4 *)(this + 0x78) = 0;
  return;
}


// ---- __ZN13FTGLExtrdFontC1EPKc @ 0x3eaba (74 bytes)

/* FTGLExtrdFont::FTGLExtrdFont(char const*) */

void __thiscall FTGLExtrdFont::FTGLExtrdFont(FTGLExtrdFont *this,char *param_1)

{
  FTFont::FTFont((FTFont *)this,param_1);
  *(undefined **)this = PTR_vtable_001c4a3c + 8;
  *(undefined4 *)(this + 0x78) = 0;
  return;
}


// ---- __ZN13FTGLExtrdFontC2EPKhm @ 0x3eb04 (80 bytes)

/* FTGLExtrdFont::FTGLExtrdFont(unsigned char const*, unsigned long) */

void __thiscall FTGLExtrdFont::FTGLExtrdFont(FTGLExtrdFont *this,uchar *param_1,ulong param_2)

{
  FTFont::FTFont((FTFont *)this,param_1,param_2);
  *(undefined **)this = PTR_vtable_001c4a3c + 8;
  *(undefined4 *)(this + 0x78) = 0;
  return;
}


// ---- __ZN13FTGLExtrdFontC1EPKhm @ 0x3eb54 (80 bytes)

/* FTGLExtrdFont::FTGLExtrdFont(unsigned char const*, unsigned long) */

void __thiscall FTGLExtrdFont::FTGLExtrdFont(FTGLExtrdFont *this,uchar *param_1,ulong param_2)

{
  FTFont::FTFont((FTFont *)this,param_1,param_2);
  *(undefined **)this = PTR_vtable_001c4a3c + 8;
  *(undefined4 *)(this + 0x78) = 0;
  return;
}


// ---- __ZN13FTGLExtrdFontD2Ev @ 0x3eba4 (72 bytes)

/* WARNING: Removing unreachable block (ram,0x0003ebdb) */
/* FTGLExtrdFont::~FTGLExtrdFont() */

void __thiscall FTGLExtrdFont::~FTGLExtrdFont(FTGLExtrdFont *this)

{
  *(undefined **)this = PTR_vtable_001c4a3c + 8;
  FTFont::~FTFont((FTFont *)this);
  return;
}


// ---- __ZN13FTGLExtrdFontD1Ev @ 0x3ebec (72 bytes)

/* WARNING: Removing unreachable block (ram,0x0003ec23) */
/* FTGLExtrdFont::~FTGLExtrdFont() */

void __thiscall FTGLExtrdFont::~FTGLExtrdFont(FTGLExtrdFont *this)

{
  *(undefined **)this = PTR_vtable_001c4a3c + 8;
  FTFont::~FTFont((FTFont *)this);
  return;
}


// ---- __ZN13FTGLExtrdFontD0Ev @ 0x3ec34 (72 bytes)

/* FTGLExtrdFont::~FTGLExtrdFont() */

void __thiscall FTGLExtrdFont::~FTGLExtrdFont(FTGLExtrdFont *this)

{
  *(undefined **)this = PTR_vtable_001c4a3c + 8;
  FTFont::~FTFont((FTFont *)this);
  operator_delete(this);
  return;
}


// ---- __ZN13FTGLExtrdFont5DepthEf @ 0x3ec7c (24 bytes)

/* FTGLExtrdFont::Depth(float) */

void __thiscall FTGLExtrdFont::Depth(FTGLExtrdFont *this,float param_1)

{
  *(float *)(this + 0x78) = param_1;
  return;
}


// ---- __ZN13FTGLExtrdFont9MakeGlyphEj @ 0x3ec94 (191 bytes)

/* FTGLExtrdFont::MakeGlyph(unsigned int) */

FTExtrdGlyph * __thiscall FTGLExtrdFont::MakeGlyph(FTGLExtrdFont *this,uint param_1)

{
  FT_GlyphSlotRec_ *pFVar1;
  undefined4 uVar2;
  FTExtrdGlyph *pFStack_20;
  
  pFVar1 = (FT_GlyphSlotRec_ *)FTFace::Glyph((FTFace *)(this + 4),param_1,2);
  if (pFVar1 == (FT_GlyphSlotRec_ *)0x0) {
    uVar2 = FTFace::Error();
    *(undefined4 *)(this + 0x58) = uVar2;
    pFStack_20 = (FTExtrdGlyph *)0x0;
  }
  else {
    pFStack_20 = (FTExtrdGlyph *)operator_new(0x40);
    FTExtrdGlyph::FTExtrdGlyph(pFStack_20,pFVar1,*(float *)(this + 0x78),(bool)this[0x54]);
  }
  return pFStack_20;
}


