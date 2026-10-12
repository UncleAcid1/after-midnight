// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTGlyph.cpp

// ---- __ZN7FTGlyphC2EP16FT_GlyphSlotRec_b @ 0x40866 (358 bytes)

/* FTGlyph::FTGlyph(FT_GlyphSlotRec_*, bool) */

void __thiscall FTGlyph::FTGlyph(FTGlyph *this,FT_GlyphSlotRec_ *param_1,bool param_2)

{
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  *(undefined **)this = PTR_vtable_001c4a58 + 8;
  FTPoint::FTPoint((FTPoint *)(this + 4));
  FTBBox::FTBBox((FTBBox *)(this + 0x1c));
  this[0x34] = (FTGlyph)param_2;
  *(undefined4 *)(this + 0x38) = 0;
  if (param_1 != (FT_GlyphSlotRec_ *)0x0) {
    FTBBox::FTBBox((FTBBox *)&local_3c,param_1);
    *(undefined4 *)(this + 0x1c) = local_3c;
    *(undefined4 *)(this + 0x20) = local_38;
    *(undefined4 *)(this + 0x24) = local_34;
    *(undefined4 *)(this + 0x28) = local_30;
    *(undefined4 *)(this + 0x2c) = local_2c;
    *(undefined4 *)(this + 0x30) = local_28;
    FTBBox::~FTBBox((FTBBox *)&local_3c);
    FTPoint::FTPoint((FTPoint *)&local_24,(double)((float)*(int *)(param_1 + 0x40) / 64.0),
                     (double)((float)*(int *)(param_1 + 0x44) / 64.0),0.0);
    *(undefined4 *)(this + 4) = local_24;
    *(undefined4 *)(this + 8) = local_20;
    *(undefined4 *)(this + 0xc) = local_1c;
    *(undefined4 *)(this + 0x10) = local_18;
    *(undefined4 *)(this + 0x14) = local_14;
    *(undefined4 *)(this + 0x18) = local_10;
  }
  return;
}


// ---- __ZN7FTGlyphC1EP16FT_GlyphSlotRec_b @ 0x409cc (358 bytes)

/* FTGlyph::FTGlyph(FT_GlyphSlotRec_*, bool) */

void __thiscall FTGlyph::FTGlyph(FTGlyph *this,FT_GlyphSlotRec_ *param_1,bool param_2)

{
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  
  *(undefined **)this = PTR_vtable_001c4a58 + 8;
  FTPoint::FTPoint((FTPoint *)(this + 4));
  FTBBox::FTBBox((FTBBox *)(this + 0x1c));
  this[0x34] = (FTGlyph)param_2;
  *(undefined4 *)(this + 0x38) = 0;
  if (param_1 != (FT_GlyphSlotRec_ *)0x0) {
    FTBBox::FTBBox((FTBBox *)&uStack_3c,param_1);
    *(undefined4 *)(this + 0x1c) = uStack_3c;
    *(undefined4 *)(this + 0x20) = uStack_38;
    *(undefined4 *)(this + 0x24) = uStack_34;
    *(undefined4 *)(this + 0x28) = uStack_30;
    *(undefined4 *)(this + 0x2c) = uStack_2c;
    *(undefined4 *)(this + 0x30) = uStack_28;
    FTBBox::~FTBBox((FTBBox *)&uStack_3c);
    FTPoint::FTPoint((FTPoint *)&uStack_24,(double)((float)*(int *)(param_1 + 0x40) / 64.0),
                     (double)((float)*(int *)(param_1 + 0x44) / 64.0),0.0);
    *(undefined4 *)(this + 4) = uStack_24;
    *(undefined4 *)(this + 8) = uStack_20;
    *(undefined4 *)(this + 0xc) = uStack_1c;
    *(undefined4 *)(this + 0x10) = uStack_18;
    *(undefined4 *)(this + 0x14) = uStack_14;
    *(undefined4 *)(this + 0x18) = uStack_10;
  }
  return;
}


// ---- __ZN7FTGlyphD2Ev @ 0x40b32 (76 bytes)

/* WARNING: Removing unreachable block (ram,0x00040b6c) */
/* FTGlyph::~FTGlyph() */

void __thiscall FTGlyph::~FTGlyph(FTGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a58 + 8;
  FTBBox::~FTBBox((FTBBox *)(this + 0x1c));
  return;
}


// ---- __ZN7FTGlyphD1Ev @ 0x40b7e (76 bytes)

/* WARNING: Removing unreachable block (ram,0x00040bb8) */
/* FTGlyph::~FTGlyph() */

void __thiscall FTGlyph::~FTGlyph(FTGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a58 + 8;
  FTBBox::~FTBBox((FTBBox *)(this + 0x1c));
  return;
}


// ---- __ZN7FTGlyphD0Ev @ 0x40bca (75 bytes)

/* FTGlyph::~FTGlyph() */

void __thiscall FTGlyph::~FTGlyph(FTGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a58 + 8;
  FTBBox::~FTBBox((FTBBox *)(this + 0x1c));
  operator_delete(this);
  return;
}


// ---- __ZN6FTBBoxC1EP16FT_GlyphSlotRec_ @ 0x40c16 (296 bytes)

/* FTBBox::FTBBox(FT_GlyphSlotRec_*) */

void __thiscall FTBBox::FTBBox(FTBBox *this,FT_GlyphSlotRec_ *param_1)

{
  int iStack_1c;
  int iStack_18;
  int iStack_14;
  int iStack_10;
  
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  _FT_Outline_Get_CBox(param_1 + 0x6c,&iStack_1c);
  *(float *)this = (float)iStack_1c / 64.0;
  *(float *)(this + 4) = (float)iStack_18 / 64.0;
  *(undefined4 *)(this + 8) = 0;
  *(float *)(this + 0xc) = (float)iStack_14 / 64.0;
  *(float *)(this + 0x10) = (float)iStack_10 / 64.0;
  *(undefined4 *)(this + 0x14) = 0;
  return;
}


