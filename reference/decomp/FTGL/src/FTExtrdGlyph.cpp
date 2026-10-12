// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTExtrdGlyph.cpp

// ---- __Z41__static_initialization_and_destruction_0ii @ 0x2ff2 (98 bytes)

/* __static_initialization_and_destruction_0(int, int) */

void __regparm3 __static_initialization_and_destruction_0(int param_1,int param_2)

{
  if ((param_2 == 0xffff) && (param_1 == 1)) {
    std::ios_base::Init::Init((Init *)PTR___ioinit_001c4a28);
    ___cxa_atexit(PTR___tcf_0_001c5368,0,PTR_MACH_HEADER_001c400c);
  }
  return;
}


// ---- __GLOBAL__I__ZN12FTExtrdGlyphC2EP16FT_GlyphSlotRec_fb @ 0x3054 (29 bytes)

void _GLOBAL__I__ZN12FTExtrdGlyphC2EP16FT_GlyphSlotRec_fb(void)

{
  __static_initialization_and_destruction_0(1,0xffff);
  return;
}


// ---- __ZN12FTExtrdGlyphD2Ev @ 0x3a5c2 (142 bytes)

/* WARNING: Removing unreachable block (ram,0x0003a616) */
/* FTExtrdGlyph::~FTExtrdGlyph() */

void __thiscall FTExtrdGlyph::~FTExtrdGlyph(FTExtrdGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a2c + 8;
  _glDeleteLists(*(undefined4 *)(this + 0x3c),1);
  FTGlyph::~FTGlyph((FTGlyph *)this);
  return;
}


// ---- __ZN12FTExtrdGlyphD1Ev @ 0x3a650 (142 bytes)

/* WARNING: Removing unreachable block (ram,0x0003a6a4) */
/* FTExtrdGlyph::~FTExtrdGlyph() */

void __thiscall FTExtrdGlyph::~FTExtrdGlyph(FTExtrdGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a2c + 8;
  _glDeleteLists(*(undefined4 *)(this + 0x3c),1);
  FTGlyph::~FTGlyph((FTGlyph *)this);
  return;
}


// ---- __ZN12FTExtrdGlyphD0Ev @ 0x3a6de (142 bytes)

/* FTExtrdGlyph::~FTExtrdGlyph() */

void __thiscall FTExtrdGlyph::~FTExtrdGlyph(FTExtrdGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a2c + 8;
  _glDeleteLists(*(undefined4 *)(this + 0x3c),1);
  FTGlyph::~FTGlyph((FTGlyph *)this);
  operator_delete(this);
  return;
}


// ---- __ZN12FTExtrdGlyph6RenderERK7FTPoint @ 0x3a76c (132 bytes)

/* FTExtrdGlyph::Render(FTPoint const&) */

FTPoint * FTExtrdGlyph::Render(FTPoint *param_1)

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


// ---- __ZN12FTExtrdGlyph9GetNormalERK7FTPointS2_ @ 0x3a7f0 (298 bytes)

/* FTExtrdGlyph::GetNormal(FTPoint const&, FTPoint const&) */

FTPoint * FTExtrdGlyph::GetNormal(FTPoint *param_1,FTPoint *param_2)

{
  longdouble lVar1;
  longdouble lVar2;
  float fVar3;
  float fVar4;
  undefined4 local_10;
  
  lVar1 = (longdouble)FTPoint::X();
  lVar2 = (longdouble)FTPoint::X();
  fVar3 = (float)((double)lVar1 - (double)lVar2);
  lVar1 = (longdouble)FTPoint::Y();
  lVar2 = (longdouble)FTPoint::Y();
  fVar4 = (float)((double)lVar1 - (double)lVar2);
  local_10 = SQRT(fVar4 * fVar4 + fVar3 * fVar3);
  if (0.01 < local_10) {
    local_10 = 1.0 / local_10;
  }
  else {
    local_10 = 0.0;
  }
  FTPoint::FTPoint(param_1,(double)(-fVar4 * local_10),(double)(fVar3 * local_10),0.0);
  return param_1;
}


// ---- __ZN12FTExtrdGlyphC1EP16FT_GlyphSlotRec_fb @ 0x3a91a (3506 bytes)

/* FTExtrdGlyph::FTExtrdGlyph(FT_GlyphSlotRec_*, float, bool) */

void __thiscall
FTExtrdGlyph::FTExtrdGlyph(FTExtrdGlyph *this,FT_GlyphSlotRec_ *param_1,float param_2,bool param_3)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  uint uVar6;
  undefined4 *puVar7;
  double *pdVar8;
  longdouble lVar9;
  longdouble lVar10;
  FTVectoriser *pFVar11;
  uint local_1d4;
  int local_1d0;
  undefined4 local_12c;
  undefined4 local_128;
  undefined4 local_124;
  undefined4 local_120;
  undefined4 local_11c;
  undefined4 local_118;
  char local_100;
  FTVectoriser local_f0 [40];
  undefined4 local_c8;
  undefined4 local_c4;
  undefined4 local_c0;
  undefined4 local_bc;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  FTPoint local_68 [24];
  uint local_50;
  int local_4c;
  int local_48;
  FTMesh *local_44;
  uint local_40;
  uint local_3c;
  undefined4 local_38;
  uint local_34;
  uint local_30;
  undefined4 local_2c;
  uint local_28;
  uint local_24;
  uint local_20;
  uint local_1c;
  uint local_18;
  uint local_14;
  int local_10;
  
  local_100 = param_3;
  FTGlyph::FTGlyph((FTGlyph *)this,param_1,true);
  *(undefined **)this = PTR_vtable_001c4a2c + 8;
  *(undefined4 *)(this + 0x3c) = 0;
  FTBBox::SetDepth((float)(this + 0x1c));
  if (*(int *)(param_1 + 0x48) == 0x6f75746c) {
    FTVectoriser::FTVectoriser(local_f0,param_1);
    iVar3 = FTVectoriser::ContourCount();
    if ((iVar3 == 0) || (uVar4 = FTVectoriser::PointCount(), uVar4 < 3)) {
      bVar1 = true;
    }
    else {
      bVar1 = false;
    }
    if (!bVar1) {
      if (local_100 != '\0') {
        uVar5 = _glGenLists(1);
        *(undefined4 *)(this + 0x3c) = uVar5;
        _glNewList(*(undefined4 *)(this + 0x3c),0x1300);
      }
      FTVectoriser::MakeMesh((double)ZEXT48(local_f0));
      _glNormal3d(0,0,0,0,0,0x3ff00000);
      local_4c = (uint)*(ushort *)(*(int *)(*(int *)(param_1 + 4) + 0x58) + 0xc) << 6;
      local_48 = (uint)*(ushort *)(*(int *)(*(int *)(param_1 + 4) + 0x58) + 0xe) << 6;
      local_44 = (FTMesh *)FTVectoriser::GetMesh();
      local_50 = 0;
      while (uVar4 = FTMesh::TesselationCount(), local_50 < uVar4) {
        local_3c = FTMesh::Tesselation(local_44,local_50);
        local_38 = FTTesselation::PolygonType();
        _glBegin(local_38);
        local_34 = 0;
        while (uVar4 = local_3c, uVar6 = FTTesselation::PointCount(), local_34 < uVar6) {
          puVar7 = (undefined4 *)FTTesselation::Point(local_3c);
          local_80 = *puVar7;
          local_7c = puVar7[1];
          local_78 = puVar7[2];
          local_74 = puVar7[3];
          local_70 = puVar7[4];
          local_6c = puVar7[5];
          lVar9 = (longdouble)FTPoint::Y();
          iVar3 = local_48 + -0x80000000;
          lVar10 = (longdouble)FTPoint::X();
          _glTexCoord2f((float)lVar10 / ((float)(local_4c + -0x80000000) + 2.1474836e+09),
                        (float)lVar9 / ((float)iVar3 + 2.1474836e+09));
          lVar9 = (longdouble)FTPoint::Y();
          lVar10 = (longdouble)FTPoint::X();
          _glVertex3f((float)((double)lVar10 / 64.0),(float)((double)lVar9 / 64.0),0);
          local_34 = local_34 + 1;
        }
        _glEnd(uVar4);
        local_50 = local_50 + 1;
      }
      FTVectoriser::MakeMesh((double)ZEXT48(local_f0));
      _glNormal3d(0,0,0,0,0,0xbff00000);
      local_44 = (FTMesh *)FTVectoriser::GetMesh();
      local_50 = 0;
      while (uVar4 = FTMesh::TesselationCount(), local_50 < uVar4) {
        local_30 = FTMesh::Tesselation(local_44,local_50);
        local_2c = FTTesselation::PolygonType();
        _glBegin(local_2c);
        local_28 = 0;
        while (uVar4 = local_30, uVar6 = FTTesselation::PointCount(), local_28 < uVar6) {
          puVar7 = (undefined4 *)FTTesselation::Point(local_30);
          local_98 = *puVar7;
          local_94 = puVar7[1];
          local_90 = puVar7[2];
          local_8c = puVar7[3];
          local_88 = puVar7[4];
          local_84 = puVar7[5];
          FTTesselation::Point(local_30);
          lVar9 = (longdouble)FTPoint::Y();
          iVar3 = local_48 + -0x80000000;
          FTTesselation::Point(local_30);
          lVar10 = (longdouble)FTPoint::X();
          _glTexCoord2f((float)lVar10 / ((float)(local_4c + -0x80000000) + 2.1474836e+09),
                        (float)lVar9 / ((float)iVar3 + 2.1474836e+09));
          FTTesselation::Point(local_30);
          lVar9 = (longdouble)FTPoint::Y();
          FTTesselation::Point(local_30);
          lVar10 = (longdouble)FTPoint::X();
          _glVertex3f((float)((double)lVar10 / 64.0),(float)((double)lVar9 / 64.0),-param_2);
          local_28 = local_28 + 1;
        }
        _glEnd(uVar4);
        local_50 = local_50 + 1;
      }
      local_40 = FTVectoriser::ContourFlag();
      local_24 = 0;
      while( true ) {
        pFVar11 = local_f0;
        uVar4 = FTVectoriser::ContourCount();
        if (uVar4 <= local_24) break;
        local_20 = FTVectoriser::Contour(local_f0,local_24);
        local_1c = FTContour::PointCount();
        _glBegin(8);
        for (local_18 = 0; local_18 <= local_1c; local_18 = local_18 + 1) {
          if (local_18 == local_1c) {
            local_1d4 = 0;
          }
          else {
            local_1d4 = local_18;
          }
          local_14 = local_1d4;
          if (local_1c - 1 == local_1d4) {
            local_1d0 = 0;
          }
          else {
            local_1d0 = local_1d4 + 1;
          }
          local_10 = local_1d0;
          puVar7 = (undefined4 *)FTContour::Point(local_20);
          local_b0 = *puVar7;
          local_ac = puVar7[1];
          local_a8 = puVar7[2];
          local_a4 = puVar7[3];
          local_a0 = puVar7[4];
          local_9c = puVar7[5];
          FTContour::Point(local_20);
          GetNormal((FTPoint *)&local_12c,(FTPoint *)this);
          local_c8 = local_12c;
          local_c4 = local_128;
          local_c0 = local_124;
          local_bc = local_120;
          local_b8 = local_11c;
          local_b4 = local_118;
          FTPoint::FTPoint(local_68,0.0,0.0,0.0);
          cVar2 = operator!=((FTPoint *)&local_c8,local_68);
          if (cVar2 != '\0') {
            pdVar8 = FTPoint::operator_cast_to_double_((FTPoint *)&local_c8);
            _glNormal3dv(pdVar8);
          }
          if ((local_40 >> 2 & 1) == 0) {
            lVar9 = (longdouble)FTPoint::Y();
            iVar3 = local_48 + -0x80000000;
            lVar10 = (longdouble)FTPoint::X();
            _glTexCoord2f((float)lVar10 / ((float)(local_4c + -0x80000000) + 2.1474836e+09),
                          (float)lVar9 / ((float)iVar3 + 2.1474836e+09));
            lVar9 = (longdouble)FTPoint::Y();
            lVar10 = (longdouble)FTPoint::X();
            _glVertex3f((float)((double)lVar10 / 64.0),(float)((double)lVar9 / 64.0),-param_2);
            lVar9 = (longdouble)FTPoint::Y();
            lVar10 = (longdouble)FTPoint::X();
            _glVertex3f((float)((double)lVar10 / 64.0),(float)((double)lVar9 / 64.0),0);
          }
          else {
            lVar9 = (longdouble)FTPoint::X();
            iVar3 = local_48 + -0x80000000;
            lVar10 = (longdouble)FTPoint::X();
            _glTexCoord2f((float)lVar10 / ((float)(local_4c + -0x80000000) + 2.1474836e+09),
                          (float)lVar9 / ((float)iVar3 + 2.1474836e+09));
            lVar9 = (longdouble)FTPoint::Y();
            lVar10 = (longdouble)FTPoint::X();
            _glVertex3f((float)((double)lVar10 / 64.0),(float)((double)lVar9 / 64.0),0);
            lVar9 = (longdouble)FTPoint::Y();
            lVar10 = (longdouble)FTPoint::X();
            _glVertex3f((float)((double)lVar10 / 64.0),(float)((double)lVar9 / 64.0),-param_2);
          }
        }
        _glEnd();
        local_24 = local_24 + 1;
      }
      if (local_100 != '\0') {
        _glEndList(pFVar11);
      }
    }
    FTVectoriser::~FTVectoriser(local_f0);
    return;
  }
  *(undefined4 *)(this + 0x38) = 0x14;
  return;
}


// ---- __ZN12FTExtrdGlyphC2EP16FT_GlyphSlotRec_fb @ 0x3b6cc (3506 bytes)

/* FTExtrdGlyph::FTExtrdGlyph(FT_GlyphSlotRec_*, float, bool) */

void __thiscall
FTExtrdGlyph::FTExtrdGlyph(FTExtrdGlyph *this,FT_GlyphSlotRec_ *param_1,float param_2,bool param_3)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  uint uVar6;
  undefined4 *puVar7;
  double *pdVar8;
  longdouble lVar9;
  longdouble lVar10;
  FTVectoriser *pFVar11;
  uint uStack_1d4;
  int iStack_1d0;
  undefined4 uStack_12c;
  undefined4 uStack_128;
  undefined4 uStack_124;
  undefined4 uStack_120;
  undefined4 uStack_11c;
  undefined4 uStack_118;
  char cStack_100;
  FTVectoriser aFStack_f0 [40];
  undefined4 uStack_c8;
  undefined4 uStack_c4;
  undefined4 uStack_c0;
  undefined4 uStack_bc;
  undefined4 uStack_b8;
  undefined4 uStack_b4;
  undefined4 uStack_b0;
  undefined4 uStack_ac;
  undefined4 uStack_a8;
  undefined4 uStack_a4;
  undefined4 uStack_a0;
  undefined4 uStack_9c;
  undefined4 uStack_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  undefined4 uStack_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  FTPoint aFStack_68 [24];
  uint uStack_50;
  int iStack_4c;
  int iStack_48;
  FTMesh *pFStack_44;
  uint uStack_40;
  uint uStack_3c;
  undefined4 uStack_38;
  uint uStack_34;
  uint uStack_30;
  undefined4 uStack_2c;
  uint uStack_28;
  uint uStack_24;
  uint uStack_20;
  uint uStack_1c;
  uint uStack_18;
  uint uStack_14;
  int iStack_10;
  
  cStack_100 = param_3;
  FTGlyph::FTGlyph((FTGlyph *)this,param_1,true);
  *(undefined **)this = PTR_vtable_001c4a2c + 8;
  *(undefined4 *)(this + 0x3c) = 0;
  FTBBox::SetDepth((float)(this + 0x1c));
  if (*(int *)(param_1 + 0x48) == 0x6f75746c) {
    FTVectoriser::FTVectoriser(aFStack_f0,param_1);
    iVar3 = FTVectoriser::ContourCount();
    if ((iVar3 == 0) || (uVar4 = FTVectoriser::PointCount(), uVar4 < 3)) {
      bVar1 = true;
    }
    else {
      bVar1 = false;
    }
    if (!bVar1) {
      if (cStack_100 != '\0') {
        uVar5 = _glGenLists(1);
        *(undefined4 *)(this + 0x3c) = uVar5;
        _glNewList(*(undefined4 *)(this + 0x3c),0x1300);
      }
      FTVectoriser::MakeMesh((double)ZEXT48(aFStack_f0));
      _glNormal3d(0,0,0,0,0,0x3ff00000);
      iStack_4c = (uint)*(ushort *)(*(int *)(*(int *)(param_1 + 4) + 0x58) + 0xc) << 6;
      iStack_48 = (uint)*(ushort *)(*(int *)(*(int *)(param_1 + 4) + 0x58) + 0xe) << 6;
      pFStack_44 = (FTMesh *)FTVectoriser::GetMesh();
      uStack_50 = 0;
      while (uVar4 = FTMesh::TesselationCount(), uStack_50 < uVar4) {
        uStack_3c = FTMesh::Tesselation(pFStack_44,uStack_50);
        uStack_38 = FTTesselation::PolygonType();
        _glBegin(uStack_38);
        uStack_34 = 0;
        while (uVar4 = uStack_3c, uVar6 = FTTesselation::PointCount(), uStack_34 < uVar6) {
          puVar7 = (undefined4 *)FTTesselation::Point(uStack_3c);
          uStack_80 = *puVar7;
          uStack_7c = puVar7[1];
          uStack_78 = puVar7[2];
          uStack_74 = puVar7[3];
          uStack_70 = puVar7[4];
          uStack_6c = puVar7[5];
          lVar9 = (longdouble)FTPoint::Y();
          iVar3 = iStack_48 + -0x80000000;
          lVar10 = (longdouble)FTPoint::X();
          _glTexCoord2f((float)lVar10 / ((float)(iStack_4c + -0x80000000) + 2.1474836e+09),
                        (float)lVar9 / ((float)iVar3 + 2.1474836e+09));
          lVar9 = (longdouble)FTPoint::Y();
          lVar10 = (longdouble)FTPoint::X();
          _glVertex3f((float)((double)lVar10 / 64.0),(float)((double)lVar9 / 64.0),0);
          uStack_34 = uStack_34 + 1;
        }
        _glEnd(uVar4);
        uStack_50 = uStack_50 + 1;
      }
      FTVectoriser::MakeMesh((double)ZEXT48(aFStack_f0));
      _glNormal3d(0,0,0,0,0,0xbff00000);
      pFStack_44 = (FTMesh *)FTVectoriser::GetMesh();
      uStack_50 = 0;
      while (uVar4 = FTMesh::TesselationCount(), uStack_50 < uVar4) {
        uStack_30 = FTMesh::Tesselation(pFStack_44,uStack_50);
        uStack_2c = FTTesselation::PolygonType();
        _glBegin(uStack_2c);
        uStack_28 = 0;
        while (uVar4 = uStack_30, uVar6 = FTTesselation::PointCount(), uStack_28 < uVar6) {
          puVar7 = (undefined4 *)FTTesselation::Point(uStack_30);
          uStack_98 = *puVar7;
          uStack_94 = puVar7[1];
          uStack_90 = puVar7[2];
          uStack_8c = puVar7[3];
          uStack_88 = puVar7[4];
          uStack_84 = puVar7[5];
          FTTesselation::Point(uStack_30);
          lVar9 = (longdouble)FTPoint::Y();
          iVar3 = iStack_48 + -0x80000000;
          FTTesselation::Point(uStack_30);
          lVar10 = (longdouble)FTPoint::X();
          _glTexCoord2f((float)lVar10 / ((float)(iStack_4c + -0x80000000) + 2.1474836e+09),
                        (float)lVar9 / ((float)iVar3 + 2.1474836e+09));
          FTTesselation::Point(uStack_30);
          lVar9 = (longdouble)FTPoint::Y();
          FTTesselation::Point(uStack_30);
          lVar10 = (longdouble)FTPoint::X();
          _glVertex3f((float)((double)lVar10 / 64.0),(float)((double)lVar9 / 64.0),-param_2);
          uStack_28 = uStack_28 + 1;
        }
        _glEnd(uVar4);
        uStack_50 = uStack_50 + 1;
      }
      uStack_40 = FTVectoriser::ContourFlag();
      uStack_24 = 0;
      while( true ) {
        pFVar11 = aFStack_f0;
        uVar4 = FTVectoriser::ContourCount();
        if (uVar4 <= uStack_24) break;
        uStack_20 = FTVectoriser::Contour(aFStack_f0,uStack_24);
        uStack_1c = FTContour::PointCount();
        _glBegin(8);
        for (uStack_18 = 0; uStack_18 <= uStack_1c; uStack_18 = uStack_18 + 1) {
          if (uStack_18 == uStack_1c) {
            uStack_1d4 = 0;
          }
          else {
            uStack_1d4 = uStack_18;
          }
          uStack_14 = uStack_1d4;
          if (uStack_1c - 1 == uStack_1d4) {
            iStack_1d0 = 0;
          }
          else {
            iStack_1d0 = uStack_1d4 + 1;
          }
          iStack_10 = iStack_1d0;
          puVar7 = (undefined4 *)FTContour::Point(uStack_20);
          uStack_b0 = *puVar7;
          uStack_ac = puVar7[1];
          uStack_a8 = puVar7[2];
          uStack_a4 = puVar7[3];
          uStack_a0 = puVar7[4];
          uStack_9c = puVar7[5];
          FTContour::Point(uStack_20);
          GetNormal((FTPoint *)&uStack_12c,(FTPoint *)this);
          uStack_c8 = uStack_12c;
          uStack_c4 = uStack_128;
          uStack_c0 = uStack_124;
          uStack_bc = uStack_120;
          uStack_b8 = uStack_11c;
          uStack_b4 = uStack_118;
          FTPoint::FTPoint(aFStack_68,0.0,0.0,0.0);
          cVar2 = operator!=((FTPoint *)&uStack_c8,aFStack_68);
          if (cVar2 != '\0') {
            pdVar8 = FTPoint::operator_cast_to_double_((FTPoint *)&uStack_c8);
            _glNormal3dv(pdVar8);
          }
          if ((uStack_40 >> 2 & 1) == 0) {
            lVar9 = (longdouble)FTPoint::Y();
            iVar3 = iStack_48 + -0x80000000;
            lVar10 = (longdouble)FTPoint::X();
            _glTexCoord2f((float)lVar10 / ((float)(iStack_4c + -0x80000000) + 2.1474836e+09),
                          (float)lVar9 / ((float)iVar3 + 2.1474836e+09));
            lVar9 = (longdouble)FTPoint::Y();
            lVar10 = (longdouble)FTPoint::X();
            _glVertex3f((float)((double)lVar10 / 64.0),(float)((double)lVar9 / 64.0),-param_2);
            lVar9 = (longdouble)FTPoint::Y();
            lVar10 = (longdouble)FTPoint::X();
            _glVertex3f((float)((double)lVar10 / 64.0),(float)((double)lVar9 / 64.0),0);
          }
          else {
            lVar9 = (longdouble)FTPoint::X();
            iVar3 = iStack_48 + -0x80000000;
            lVar10 = (longdouble)FTPoint::X();
            _glTexCoord2f((float)lVar10 / ((float)(iStack_4c + -0x80000000) + 2.1474836e+09),
                          (float)lVar9 / ((float)iVar3 + 2.1474836e+09));
            lVar9 = (longdouble)FTPoint::Y();
            lVar10 = (longdouble)FTPoint::X();
            _glVertex3f((float)((double)lVar10 / 64.0),(float)((double)lVar9 / 64.0),0);
            lVar9 = (longdouble)FTPoint::Y();
            lVar10 = (longdouble)FTPoint::X();
            _glVertex3f((float)((double)lVar10 / 64.0),(float)((double)lVar9 / 64.0),-param_2);
          }
        }
        _glEnd();
        uStack_24 = uStack_24 + 1;
      }
      if (cStack_100 != '\0') {
        _glEndList(pFVar11);
      }
    }
    FTVectoriser::~FTVectoriser(aFStack_f0);
    return;
  }
  *(undefined4 *)(this + 0x38) = 0x14;
  return;
}


// ---- ___tcf_0 @ 0x3c47e (41 bytes)

void __tcf_0(void)

{
  std::ios_base::Init::~Init((Init *)PTR___ioinit_001c4a28);
  return;
}


// ---- __ZNK7FTPointcvPKdEv @ 0x3c4a8 (18 bytes)

/* FTPoint::operator double const*() const */

double * __thiscall FTPoint::operator_cast_to_double_(FTPoint *this)

{
  return (double *)this;
}


// ---- __ZN6FTBBox8SetDepthEf @ 0x3c4ba (36 bytes)

/* FTBBox::SetDepth(float) */

void __thiscall FTBBox::SetDepth(FTBBox *this,float param_1)

{
  *(float *)(this + 0x14) = *(float *)(this + 8) + param_1;
  return;
}


// ---- __ZNK9FTContour10PointCountEv @ 0x3c4de (26 bytes)

/* FTContour::PointCount() const */

void FTContour::PointCount(void)

{
  FTVector<FTPoint>::size();
  return;
}


// ---- __ZNK13FTTesselation10PointCountEv @ 0x3c4f8 (26 bytes)

/* FTTesselation::PointCount() const */

void FTTesselation::PointCount(void)

{
  FTVector<FTPoint>::size();
  return;
}


// ---- __ZNK13FTTesselation11PolygonTypeEv @ 0x3c512 (20 bytes)

/* FTTesselation::PolygonType() const */

undefined4 __thiscall FTTesselation::PolygonType(FTTesselation *this)

{
  return *(undefined4 *)(this + 0x10);
}


// ---- __ZNK8FTVectorIP13FTTesselationE4sizeEv @ 0x3c526 (20 bytes)

/* FTVector<FTTesselation*>::size() const */

undefined4 __thiscall FTVector<FTTesselation*>::size(FTVector<FTTesselation*> *this)

{
  return *(undefined4 *)(this + 8);
}


// ---- __ZNK6FTMesh16TesselationCountEv @ 0x3c53a (28 bytes)

/* FTMesh::TesselationCount() const */

void FTMesh::TesselationCount(void)

{
  FTVector<FTTesselation*>::size();
  return;
}


// ---- __ZNK12FTVectoriser7GetMeshEv @ 0x3c556 (20 bytes)

/* FTVectoriser::GetMesh() const */

undefined4 __thiscall FTVectoriser::GetMesh(FTVectoriser *this)

{
  return *(undefined4 *)(this + 8);
}


// ---- __ZNK12FTVectoriser12ContourCountEv @ 0x3c56a (22 bytes)

/* FTVectoriser::ContourCount() const */

int __thiscall FTVectoriser::ContourCount(FTVectoriser *this)

{
  return (int)*(short *)(this + 0xc);
}


// ---- __ZNK12FTVectoriser11ContourFlagEv @ 0x3c580 (20 bytes)

/* FTVectoriser::ContourFlag() const */

undefined4 __thiscall FTVectoriser::ContourFlag(FTVectoriser *this)

{
  return *(undefined4 *)(this + 0x10);
}


// ---- __ZNK8FTVectorI7FTPointE5beginEv @ 0x3c594 (20 bytes)

/* FTVector<FTPoint>::begin() const */

undefined4 __thiscall FTVector<FTPoint>::begin(FTVector<FTPoint> *this)

{
  return *(undefined4 *)(this + 0xc);
}


// ---- __ZNK8FTVectorI7FTPointEixEm @ 0x3c5a8 (42 bytes)

/* FTVector<FTPoint>::operator[](unsigned long) const */

int __thiscall FTVector<FTPoint>::operator[](FTVector<FTPoint> *this,ulong param_1)

{
  int iVar1;
  
  iVar1 = FTVector<FTPoint>::begin();
  return iVar1 + param_1 * 0x18;
}


// ---- __ZNK13FTTesselation5PointEj @ 0x3c5d2 (32 bytes)

/* FTTesselation::Point(unsigned int) const */

void FTTesselation::Point(uint param_1)

{
  FTVector<FTPoint>::operator[](param_1);
  return;
}


// ---- __ZNK9FTContour5PointEj @ 0x3c5f2 (32 bytes)

/* FTContour::Point(unsigned int) const */

void FTContour::Point(uint param_1)

{
  FTVector<FTPoint>::operator[](param_1);
  return;
}


