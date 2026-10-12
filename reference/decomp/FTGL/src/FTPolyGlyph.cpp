// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTPolyGlyph.cpp

// ---- __ZN11FTPolyGlyphD2Ev @ 0x42918 (142 bytes)

/* WARNING: Removing unreachable block (ram,0x0004296c) */
/* FTPolyGlyph::~FTPolyGlyph() */

void __thiscall FTPolyGlyph::~FTPolyGlyph(FTPolyGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a74 + 8;
  _glDeleteLists(*(undefined4 *)(this + 0x3c),1);
  FTGlyph::~FTGlyph((FTGlyph *)this);
  return;
}


// ---- __ZN11FTPolyGlyphD1Ev @ 0x429a6 (142 bytes)

/* WARNING: Removing unreachable block (ram,0x000429fa) */
/* FTPolyGlyph::~FTPolyGlyph() */

void __thiscall FTPolyGlyph::~FTPolyGlyph(FTPolyGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a74 + 8;
  _glDeleteLists(*(undefined4 *)(this + 0x3c),1);
  FTGlyph::~FTGlyph((FTGlyph *)this);
  return;
}


// ---- __ZN11FTPolyGlyphD0Ev @ 0x42a34 (142 bytes)

/* FTPolyGlyph::~FTPolyGlyph() */

void __thiscall FTPolyGlyph::~FTPolyGlyph(FTPolyGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a74 + 8;
  _glDeleteLists(*(undefined4 *)(this + 0x3c),1);
  FTGlyph::~FTGlyph((FTGlyph *)this);
  operator_delete(this);
  return;
}


// ---- __ZN11FTPolyGlyph6RenderERK7FTPoint @ 0x42ac2 (132 bytes)

/* FTPolyGlyph::Render(FTPoint const&) */

FTPoint * FTPolyGlyph::Render(FTPoint *param_1)

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


// ---- __ZN11FTPolyGlyphC1EP16FT_GlyphSlotRec_b @ 0x42b46 (978 bytes)

/* FTPolyGlyph::FTPolyGlyph(FT_GlyphSlotRec_*, bool) */

void __thiscall FTPolyGlyph::FTPolyGlyph(FTPolyGlyph *this,FT_GlyphSlotRec_ *param_1,bool param_2)

{
  bool bVar1;
  int iVar2;
  uint uVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  uint uVar6;
  longdouble lVar7;
  longdouble lVar8;
  FTMesh *pFVar9;
  undefined4 uVar10;
  FTVectoriser local_68 [40];
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  int local_28;
  int local_24;
  FTMesh *local_20;
  uint local_1c;
  uint local_18;
  undefined4 local_14;
  uint local_10;
  
  FTGlyph::FTGlyph((FTGlyph *)this,param_1,true);
  *(undefined **)this = PTR_vtable_001c4a74 + 8;
  *(undefined4 *)(this + 0x3c) = 0;
  if (*(int *)(param_1 + 0x48) == 0x6f75746c) {
    FTVectoriser::FTVectoriser(local_68,param_1);
    iVar2 = FTVectoriser::ContourCount();
    if ((iVar2 == 0) || (uVar3 = FTVectoriser::PointCount(), uVar3 < 3)) {
      bVar1 = true;
    }
    else {
      bVar1 = false;
    }
    if (!bVar1) {
      local_28 = (uint)*(ushort *)(*(int *)(*(int *)(param_1 + 4) + 0x58) + 0xc) << 6;
      local_24 = (uint)*(ushort *)(*(int *)(*(int *)(param_1 + 4) + 0x58) + 0xe) << 6;
      uVar4 = 0;
      uVar10 = 0x3ff00000;
      FTVectoriser::MakeMesh((double)ZEXT48(local_68));
      if (param_2) {
        uVar4 = _glGenLists(1,uVar4,uVar10);
        *(undefined4 *)(this + 0x3c) = uVar4;
        _glNewList(*(undefined4 *)(this + 0x3c),0x1300);
      }
      local_20 = (FTMesh *)FTVectoriser::GetMesh();
      local_1c = 0;
      while (pFVar9 = local_20, uVar3 = FTMesh::TesselationCount(), local_1c < uVar3) {
        local_18 = FTMesh::Tesselation(local_20,local_1c);
        local_14 = FTTesselation::PolygonType();
        _glBegin(local_14);
        local_10 = 0;
        while (uVar3 = local_18, uVar6 = FTTesselation::PointCount(), local_10 < uVar6) {
          puVar5 = (undefined4 *)FTTesselation::Point(local_18);
          local_40 = *puVar5;
          local_3c = puVar5[1];
          local_38 = puVar5[2];
          local_34 = puVar5[3];
          local_30 = puVar5[4];
          local_2c = puVar5[5];
          lVar7 = (longdouble)FTPoint::Y();
          iVar2 = local_24 + -0x80000000;
          lVar8 = (longdouble)FTPoint::X();
          _glTexCoord2f((float)lVar8 / ((float)(local_28 + -0x80000000) + 2.1474836e+09),
                        (float)lVar7 / ((float)iVar2 + 2.1474836e+09));
          lVar7 = (longdouble)FTPoint::Y();
          lVar8 = (longdouble)FTPoint::X();
          _glVertex3f((float)((double)lVar8 / 64.0),(float)((double)lVar7 / 64.0),0);
          local_10 = local_10 + 1;
        }
        _glEnd(uVar3);
        local_1c = local_1c + 1;
      }
      if (param_2) {
        _glEndList(pFVar9);
      }
    }
    FTVectoriser::~FTVectoriser(local_68);
    return;
  }
  *(undefined4 *)(this + 0x38) = 0x14;
  return;
}


// ---- __ZN11FTPolyGlyphC2EP16FT_GlyphSlotRec_b @ 0x42f18 (978 bytes)

/* FTPolyGlyph::FTPolyGlyph(FT_GlyphSlotRec_*, bool) */

void __thiscall FTPolyGlyph::FTPolyGlyph(FTPolyGlyph *this,FT_GlyphSlotRec_ *param_1,bool param_2)

{
  bool bVar1;
  int iVar2;
  uint uVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  uint uVar6;
  longdouble lVar7;
  longdouble lVar8;
  FTMesh *pFVar9;
  undefined4 uVar10;
  FTVectoriser aFStack_68 [40];
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  int iStack_28;
  int iStack_24;
  FTMesh *pFStack_20;
  uint uStack_1c;
  uint uStack_18;
  undefined4 uStack_14;
  uint uStack_10;
  
  FTGlyph::FTGlyph((FTGlyph *)this,param_1,true);
  *(undefined **)this = PTR_vtable_001c4a74 + 8;
  *(undefined4 *)(this + 0x3c) = 0;
  if (*(int *)(param_1 + 0x48) == 0x6f75746c) {
    FTVectoriser::FTVectoriser(aFStack_68,param_1);
    iVar2 = FTVectoriser::ContourCount();
    if ((iVar2 == 0) || (uVar3 = FTVectoriser::PointCount(), uVar3 < 3)) {
      bVar1 = true;
    }
    else {
      bVar1 = false;
    }
    if (!bVar1) {
      iStack_28 = (uint)*(ushort *)(*(int *)(*(int *)(param_1 + 4) + 0x58) + 0xc) << 6;
      iStack_24 = (uint)*(ushort *)(*(int *)(*(int *)(param_1 + 4) + 0x58) + 0xe) << 6;
      uVar4 = 0;
      uVar10 = 0x3ff00000;
      FTVectoriser::MakeMesh((double)ZEXT48(aFStack_68));
      if (param_2) {
        uVar4 = _glGenLists(1,uVar4,uVar10);
        *(undefined4 *)(this + 0x3c) = uVar4;
        _glNewList(*(undefined4 *)(this + 0x3c),0x1300);
      }
      pFStack_20 = (FTMesh *)FTVectoriser::GetMesh();
      uStack_1c = 0;
      while (pFVar9 = pFStack_20, uVar3 = FTMesh::TesselationCount(), uStack_1c < uVar3) {
        uStack_18 = FTMesh::Tesselation(pFStack_20,uStack_1c);
        uStack_14 = FTTesselation::PolygonType();
        _glBegin(uStack_14);
        uStack_10 = 0;
        while (uVar3 = uStack_18, uVar6 = FTTesselation::PointCount(), uStack_10 < uVar6) {
          puVar5 = (undefined4 *)FTTesselation::Point(uStack_18);
          uStack_40 = *puVar5;
          uStack_3c = puVar5[1];
          uStack_38 = puVar5[2];
          uStack_34 = puVar5[3];
          uStack_30 = puVar5[4];
          uStack_2c = puVar5[5];
          lVar7 = (longdouble)FTPoint::Y();
          iVar2 = iStack_24 + -0x80000000;
          lVar8 = (longdouble)FTPoint::X();
          _glTexCoord2f((float)lVar8 / ((float)(iStack_28 + -0x80000000) + 2.1474836e+09),
                        (float)lVar7 / ((float)iVar2 + 2.1474836e+09));
          lVar7 = (longdouble)FTPoint::Y();
          lVar8 = (longdouble)FTPoint::X();
          _glVertex3f((float)((double)lVar8 / 64.0),(float)((double)lVar7 / 64.0),0);
          uStack_10 = uStack_10 + 1;
        }
        _glEnd(uVar3);
        uStack_1c = uStack_1c + 1;
      }
      if (param_2) {
        _glEndList(pFVar9);
      }
    }
    FTVectoriser::~FTVectoriser(aFStack_68);
    return;
  }
  *(undefined4 *)(this + 0x38) = 0x14;
  return;
}


