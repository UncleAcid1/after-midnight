// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTOutlineGlyph.cpp

// ---- __ZN14FTOutlineGlyphD2Ev @ 0x41a02 (142 bytes)

/* WARNING: Removing unreachable block (ram,0x00041a56) */
/* FTOutlineGlyph::~FTOutlineGlyph() */

void __thiscall FTOutlineGlyph::~FTOutlineGlyph(FTOutlineGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a6c + 8;
  _glDeleteLists(*(undefined4 *)(this + 0x3c),1);
  FTGlyph::~FTGlyph((FTGlyph *)this);
  return;
}


// ---- __ZN14FTOutlineGlyphD1Ev @ 0x41a90 (142 bytes)

/* WARNING: Removing unreachable block (ram,0x00041ae4) */
/* FTOutlineGlyph::~FTOutlineGlyph() */

void __thiscall FTOutlineGlyph::~FTOutlineGlyph(FTOutlineGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a6c + 8;
  _glDeleteLists(*(undefined4 *)(this + 0x3c),1);
  FTGlyph::~FTGlyph((FTGlyph *)this);
  return;
}


// ---- __ZN14FTOutlineGlyphD0Ev @ 0x41b1e (142 bytes)

/* FTOutlineGlyph::~FTOutlineGlyph() */

void __thiscall FTOutlineGlyph::~FTOutlineGlyph(FTOutlineGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a6c + 8;
  _glDeleteLists(*(undefined4 *)(this + 0x3c),1);
  FTGlyph::~FTGlyph((FTGlyph *)this);
  operator_delete(this);
  return;
}


// ---- __ZN14FTOutlineGlyph6RenderERK7FTPoint @ 0x41bac (132 bytes)

/* FTOutlineGlyph::Render(FTPoint const&) */

FTPoint * FTOutlineGlyph::Render(FTPoint *param_1)

{
  longdouble lVar1;
  longdouble lVar2;
  
  lVar1 = (longdouble)FTPoint::Y();
  lVar2 = (longdouble)FTPoint::X();
  _glTranslatef((float)lVar2,(float)lVar1,0);
  if (*(int *)(param_1 + 0x3c) != 0) {
    _glCallList(*(undefined4 *)(param_1 + 0x3c));
  }
  return param_1 + 4;
}


// ---- __ZN14FTOutlineGlyphC1EP16FT_GlyphSlotRec_b @ 0x41c30 (602 bytes)

/* FTOutlineGlyph::FTOutlineGlyph(FT_GlyphSlotRec_*, bool) */

void __thiscall
FTOutlineGlyph::FTOutlineGlyph(FTOutlineGlyph *this,FT_GlyphSlotRec_ *param_1,bool param_2)

{
  bool bVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  longdouble lVar5;
  longdouble lVar6;
  FTVectoriser *pFVar7;
  FTVectoriser local_5c [40];
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  uint local_1c;
  uint local_18;
  FTVectoriser *local_14;
  uint local_10;
  
  FTGlyph::FTGlyph((FTGlyph *)this,param_1,true);
  *(undefined **)this = PTR_vtable_001c4a6c + 8;
  *(undefined4 *)(this + 0x3c) = 0;
  if (*(int *)(param_1 + 0x48) != 0x6f75746c) {
    *(undefined4 *)(this + 0x38) = 0x14;
    return;
  }
  FTVectoriser::FTVectoriser(local_5c,param_1);
  pFVar7 = local_5c;
  local_1c = FTVectoriser::ContourCount();
  if (local_1c != 0) {
    pFVar7 = local_5c;
    uVar2 = FTVectoriser::PointCount();
    if (2 < uVar2) {
      bVar1 = false;
      goto LAB_00041cdd;
    }
  }
  bVar1 = true;
LAB_00041cdd:
  if (!bVar1) {
    if (param_2) {
      uVar3 = _glGenLists(1);
      *(undefined4 *)(this + 0x3c) = uVar3;
      pFVar7 = *(FTVectoriser **)(this + 0x3c);
      _glNewList(pFVar7,0x1300);
    }
    for (local_18 = 0; local_18 < local_1c; local_18 = local_18 + 1) {
      local_14 = (FTVectoriser *)FTVectoriser::Contour(local_5c,local_18);
      _glBegin(2);
      local_10 = 0;
      while (pFVar7 = local_14, uVar2 = FTContour::PointCount(), local_10 < uVar2) {
        puVar4 = (undefined4 *)FTContour::Point((uint)local_14);
        local_34 = *puVar4;
        local_30 = puVar4[1];
        local_2c = puVar4[2];
        local_28 = puVar4[3];
        local_24 = puVar4[4];
        local_20 = puVar4[5];
        lVar5 = (longdouble)FTPoint::Y();
        lVar6 = (longdouble)FTPoint::X();
        _glVertex2f((float)((double)lVar6 / 64.0),(float)((double)lVar5 / 64.0));
        local_10 = local_10 + 1;
      }
      _glEnd(pFVar7);
    }
    if (param_2) {
      _glEndList(pFVar7);
    }
  }
  FTVectoriser::~FTVectoriser(local_5c);
  return;
}


// ---- __ZN14FTOutlineGlyphC2EP16FT_GlyphSlotRec_b @ 0x41e8a (602 bytes)

/* FTOutlineGlyph::FTOutlineGlyph(FT_GlyphSlotRec_*, bool) */

void __thiscall
FTOutlineGlyph::FTOutlineGlyph(FTOutlineGlyph *this,FT_GlyphSlotRec_ *param_1,bool param_2)

{
  bool bVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  longdouble lVar5;
  longdouble lVar6;
  FTVectoriser *pFVar7;
  FTVectoriser aFStack_5c [40];
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  uint uStack_1c;
  uint uStack_18;
  FTVectoriser *pFStack_14;
  uint uStack_10;
  
  FTGlyph::FTGlyph((FTGlyph *)this,param_1,true);
  *(undefined **)this = PTR_vtable_001c4a6c + 8;
  *(undefined4 *)(this + 0x3c) = 0;
  if (*(int *)(param_1 + 0x48) != 0x6f75746c) {
    *(undefined4 *)(this + 0x38) = 0x14;
    return;
  }
  FTVectoriser::FTVectoriser(aFStack_5c,param_1);
  pFVar7 = aFStack_5c;
  uStack_1c = FTVectoriser::ContourCount();
  if (uStack_1c != 0) {
    pFVar7 = aFStack_5c;
    uVar2 = FTVectoriser::PointCount();
    if (2 < uVar2) {
      bVar1 = false;
      goto LAB_00041f37;
    }
  }
  bVar1 = true;
LAB_00041f37:
  if (!bVar1) {
    if (param_2) {
      uVar3 = _glGenLists(1);
      *(undefined4 *)(this + 0x3c) = uVar3;
      pFVar7 = *(FTVectoriser **)(this + 0x3c);
      _glNewList(pFVar7,0x1300);
    }
    for (uStack_18 = 0; uStack_18 < uStack_1c; uStack_18 = uStack_18 + 1) {
      pFStack_14 = (FTVectoriser *)FTVectoriser::Contour(aFStack_5c,uStack_18);
      _glBegin(2);
      uStack_10 = 0;
      while (pFVar7 = pFStack_14, uVar2 = FTContour::PointCount(), uStack_10 < uVar2) {
        puVar4 = (undefined4 *)FTContour::Point((uint)pFStack_14);
        uStack_34 = *puVar4;
        uStack_30 = puVar4[1];
        uStack_2c = puVar4[2];
        uStack_28 = puVar4[3];
        uStack_24 = puVar4[4];
        uStack_20 = puVar4[5];
        lVar5 = (longdouble)FTPoint::Y();
        lVar6 = (longdouble)FTPoint::X();
        _glVertex2f((float)((double)lVar6 / 64.0),(float)((double)lVar5 / 64.0));
        uStack_10 = uStack_10 + 1;
      }
      _glEnd(pFVar7);
    }
    if (param_2) {
      _glEndList(pFVar7);
    }
  }
  FTVectoriser::~FTVectoriser(aFStack_5c);
  return;
}


