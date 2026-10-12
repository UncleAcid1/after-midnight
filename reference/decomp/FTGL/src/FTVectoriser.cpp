// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTVectoriser.cpp

// ---- __Z9ftglErrormP6FTMesh @ 0x442f4 (32 bytes)

/* ftglError(unsigned long, FTMesh*) */

void ftglError(ulong param_1,FTMesh *param_2)

{
  FTMesh::Error((ulong)param_2);
  return;
}


// ---- __ZN12FTVectoriser15ProcessContoursEv @ 0x44314 (288 bytes)

/* FTVectoriser::ProcessContours() */

void __thiscall FTVectoriser::ProcessContours(FTVectoriser *this)

{
  short sVar1;
  int iVar2;
  int iVar3;
  void *pvVar4;
  FTContour *this_00;
  short local_1e;
  short local_1a;
  
  local_1e = 0;
  pvVar4 = operator_new__((int)*(short *)(this + 0xc) << 2);
  *(void **)(this + 4) = pvVar4;
  for (local_1a = 0; local_1a < *(short *)(this + 0xc); local_1a = local_1a + 1) {
    iVar2 = *(int *)(this + 0x18);
    iVar3 = *(int *)(this + 0x1c);
    sVar1 = *(short *)(*(int *)(this + 0x20) + local_1a * 2);
    this_00 = (FTContour *)operator_new(0x30);
    FTContour::FTContour
              (this_00,(FT_Vector_ *)(iVar2 + local_1e * 8),(char *)(iVar3 + local_1e),
               (int)(short)((sVar1 - local_1e) + 1));
    *(FTContour **)(*(int *)(this + 4) + local_1a * 4) = this_00;
    local_1e = sVar1 + 1;
  }
  return;
}


// ---- __ZN12FTVectoriserC1EP16FT_GlyphSlotRec_ @ 0x44434 (170 bytes)

/* FTVectoriser::FTVectoriser(FT_GlyphSlotRec_*) */

void __thiscall FTVectoriser::FTVectoriser(FTVectoriser *this,FT_GlyphSlotRec_ *param_1)

{
  *(undefined **)this = PTR_vtable_001c4a94 + 8;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined2 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  if (param_1 != (FT_GlyphSlotRec_ *)0x0) {
    *(undefined4 *)(this + 0x14) = *(undefined4 *)(param_1 + 0x6c);
    *(undefined4 *)(this + 0x18) = *(undefined4 *)(param_1 + 0x70);
    *(undefined4 *)(this + 0x1c) = *(undefined4 *)(param_1 + 0x74);
    *(undefined4 *)(this + 0x20) = *(undefined4 *)(param_1 + 0x78);
    *(undefined4 *)(this + 0x24) = *(undefined4 *)(param_1 + 0x7c);
    *(undefined2 *)(this + 0xc) = *(undefined2 *)(this + 0x14);
    *(undefined4 *)(this + 4) = 0;
    *(undefined4 *)(this + 0x10) = *(undefined4 *)(this + 0x24);
    ProcessContours(this);
  }
  return;
}


// ---- __ZN12FTVectoriserC2EP16FT_GlyphSlotRec_ @ 0x444de (170 bytes)

/* FTVectoriser::FTVectoriser(FT_GlyphSlotRec_*) */

void __thiscall FTVectoriser::FTVectoriser(FTVectoriser *this,FT_GlyphSlotRec_ *param_1)

{
  *(undefined **)this = PTR_vtable_001c4a94 + 8;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined2 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  if (param_1 != (FT_GlyphSlotRec_ *)0x0) {
    *(undefined4 *)(this + 0x14) = *(undefined4 *)(param_1 + 0x6c);
    *(undefined4 *)(this + 0x18) = *(undefined4 *)(param_1 + 0x70);
    *(undefined4 *)(this + 0x1c) = *(undefined4 *)(param_1 + 0x74);
    *(undefined4 *)(this + 0x20) = *(undefined4 *)(param_1 + 0x78);
    *(undefined4 *)(this + 0x24) = *(undefined4 *)(param_1 + 0x7c);
    *(undefined2 *)(this + 0xc) = *(undefined2 *)(this + 0x14);
    *(undefined4 *)(this + 4) = 0;
    *(undefined4 *)(this + 0x10) = *(undefined4 *)(this + 0x24);
    ProcessContours(this);
  }
  return;
}


// ---- __ZN12FTVectoriser10PointCountEv @ 0x44588 (92 bytes)

/* FTVectoriser::PointCount() */

int FTVectoriser::PointCount(void)

{
  int iVar1;
  uint uVar2;
  undefined4 local_14;
  undefined4 local_10;
  
  local_14 = 0;
  local_10 = 0;
  while( true ) {
    uVar2 = ::FTVectoriser::ContourCount();
    if (uVar2 <= local_10) break;
    iVar1 = FTContour::PointCount();
    local_14 = local_14 + iVar1;
    local_10 = local_10 + 1;
  }
  return local_14;
}


// ---- __ZNK12FTVectoriser7ContourEj @ 0x445e4 (62 bytes)

/* FTVectoriser::Contour(unsigned int) const */

undefined4 __thiscall FTVectoriser::Contour(FTVectoriser *this,uint param_1)

{
  uint uVar1;
  undefined4 local_10;
  
  uVar1 = ::FTVectoriser::ContourCount();
  if (param_1 < uVar1) {
    local_10 = *(undefined4 *)(*(int *)(this + 4) + param_1 * 4);
  }
  else {
    local_10 = 0;
  }
  return local_10;
}


// ---- __ZN6FTMeshD1Ev @ 0x44622 (148 bytes)

/* FTMesh::~FTMesh() */

void __thiscall FTMesh::~FTMesh(FTMesh *this)

{
  FTTesselation *this_00;
  undefined4 *puVar1;
  uint uVar2;
  uint local_10;
  
  local_10 = 0;
  while( true ) {
    uVar2 = FTVector<FTTesselation*>::size();
    if (uVar2 <= local_10) break;
    puVar1 = (undefined4 *)FTVector<FTTesselation*>::operator[]((ulong)(this + 4));
    this_00 = (FTTesselation *)*puVar1;
    if (this_00 != (FTTesselation *)0x0) {
      FTTesselation::~FTTesselation(this_00);
      operator_delete(this_00);
    }
    local_10 = local_10 + 1;
  }
  FTVector<FTTesselation*>::clear();
  FTList<FTPoint>::~FTList((FTList<FTPoint> *)(this + 0x14));
  FTVector<FTTesselation*>::~FTVector((FTVector<FTTesselation*> *)(this + 4));
  return;
}


// ---- __ZN12FTVectoriserD0Ev @ 0x446b6 (206 bytes)

/* FTVectoriser::~FTVectoriser() */

void __thiscall FTVectoriser::~FTVectoriser(FTVectoriser *this)

{
  FTContour *this_00;
  FTMesh *this_01;
  uint uVar1;
  uint uStack_10;
  
  *(undefined **)this = PTR_vtable_001c4a94 + 8;
  for (uStack_10 = 0; uVar1 = ::FTVectoriser::ContourCount(), uStack_10 < uVar1;
      uStack_10 = uStack_10 + 1) {
    this_00 = *(FTContour **)(*(int *)(this + 4) + uStack_10 * 4);
    if (this_00 != (FTContour *)0x0) {
      FTContour::~FTContour(this_00);
      operator_delete(this_00);
    }
  }
  if (*(int *)(this + 4) != 0) {
    operator_delete__(*(void **)(this + 4));
  }
  this_01 = *(FTMesh **)(this + 8);
  if (this_01 != (FTMesh *)0x0) {
    FTMesh::~FTMesh(this_01);
    operator_delete(this_01);
  }
  operator_delete(this);
  return;
}


// ---- __ZN12FTVectoriserD1Ev @ 0x44784 (206 bytes)

/* WARNING: Removing unreachable block (ram,0x00044840) */
/* FTVectoriser::~FTVectoriser() */

void __thiscall FTVectoriser::~FTVectoriser(FTVectoriser *this)

{
  FTContour *this_00;
  FTMesh *this_01;
  uint uVar1;
  uint local_10;
  
  *(undefined **)this = PTR_vtable_001c4a94 + 8;
  for (local_10 = 0; uVar1 = ::FTVectoriser::ContourCount(), local_10 < uVar1;
      local_10 = local_10 + 1) {
    this_00 = *(FTContour **)(*(int *)(this + 4) + local_10 * 4);
    if (this_00 != (FTContour *)0x0) {
      FTContour::~FTContour(this_00);
      operator_delete(this_00);
    }
  }
  if (*(int *)(this + 4) != 0) {
    operator_delete__(*(void **)(this + 4));
  }
  this_01 = *(FTMesh **)(this + 8);
  if (this_01 != (FTMesh *)0x0) {
    FTMesh::~FTMesh(this_01);
    operator_delete(this_01);
  }
  return;
}


// ---- __ZN12FTVectoriserD2Ev @ 0x44852 (206 bytes)

/* WARNING: Removing unreachable block (ram,0x0004490e) */
/* FTVectoriser::~FTVectoriser() */

void __thiscall FTVectoriser::~FTVectoriser(FTVectoriser *this)

{
  FTContour *this_00;
  FTMesh *this_01;
  uint uVar1;
  uint uStack_10;
  
  *(undefined **)this = PTR_vtable_001c4a94 + 8;
  for (uStack_10 = 0; uVar1 = ::FTVectoriser::ContourCount(), uStack_10 < uVar1;
      uStack_10 = uStack_10 + 1) {
    this_00 = *(FTContour **)(*(int *)(this + 4) + uStack_10 * 4);
    if (this_00 != (FTContour *)0x0) {
      FTContour::~FTContour(this_00);
      operator_delete(this_00);
    }
  }
  if (*(int *)(this + 4) != 0) {
    operator_delete__(*(void **)(this + 4));
  }
  this_01 = *(FTMesh **)(this + 8);
  if (this_01 != (FTMesh *)0x0) {
    FTMesh::~FTMesh(this_01);
    operator_delete(this_01);
  }
  return;
}


// ---- __ZN6FTMeshD2Ev @ 0x44920 (148 bytes)

/* FTMesh::~FTMesh() */

void __thiscall FTMesh::~FTMesh(FTMesh *this)

{
  FTTesselation *this_00;
  undefined4 *puVar1;
  uint uVar2;
  uint uStack_10;
  
  uStack_10 = 0;
  while( true ) {
    uVar2 = FTVector<FTTesselation*>::size();
    if (uVar2 <= uStack_10) break;
    puVar1 = (undefined4 *)FTVector<FTTesselation*>::operator[]((ulong)(this + 4));
    this_00 = (FTTesselation *)*puVar1;
    if (this_00 != (FTTesselation *)0x0) {
      FTTesselation::~FTTesselation(this_00);
      operator_delete(this_00);
    }
    uStack_10 = uStack_10 + 1;
  }
  FTVector<FTTesselation*>::clear();
  FTList<FTPoint>::~FTList((FTList<FTPoint> *)(this + 0x14));
  FTVector<FTTesselation*>::~FTVector((FTVector<FTTesselation*> *)(this + 4));
  return;
}


// ---- __ZN6FTMesh7CombineEddd @ 0x449b4 (138 bytes)

/* FTMesh::Combine(double, double, double) */

void FTMesh::Combine(double param_1,double param_2,double param_3)

{
  FTPoint *this;
  undefined4 in_stack_0000001c;
  FTPoint local_24 [32];
  
  FTPoint::FTPoint(local_24,(double)CONCAT44(param_2._0_4_,param_1._4_4_),
                   (double)CONCAT44(param_3._0_4_,param_2._4_4_),
                   (double)CONCAT44(in_stack_0000001c,param_3._4_4_));
  FTList<FTPoint>::push_back((FTPoint *)(param_1._0_4_ + 0x14));
  this = (FTPoint *)FTList<FTPoint>::back();
  FTPoint::operator_cast_to_double_(this);
  return;
}


// ---- __Z11ftglCombinePdPPvPfS1_P6FTMesh @ 0x44a3e (84 bytes)

/* ftglCombine(double*, void**, float*, void**, FTMesh*) */

void ftglCombine(double *param_1,void **param_2,float *param_3,void **param_4,FTMesh *param_5)

{
  void *pvVar1;
  
  pvVar1 = (void *)FTMesh::Combine((double)CONCAT44(SUB84(*param_1,0),param_5),
                                   (double)CONCAT44(SUB84(param_1[1],0),
                                                    (int)((ulonglong)*param_1 >> 0x20)),
                                   (double)CONCAT44(SUB84(param_1[2],0),
                                                    (int)((ulonglong)param_1[1] >> 0x20)));
  *param_4 = pvVar1;
  return;
}


// ---- __ZNK6FTMesh11TesselationEj @ 0x44a92 (72 bytes)

/* FTMesh::Tesselation(unsigned int) const */

undefined4 __thiscall FTMesh::Tesselation(FTMesh *this,uint param_1)

{
  uint uVar1;
  undefined4 *puVar2;
  undefined4 local_10;
  
  uVar1 = FTVector<FTTesselation*>::size();
  if (param_1 < uVar1) {
    puVar2 = (undefined4 *)FTVector<FTTesselation*>::operator[]((ulong)(this + 4));
    local_10 = *puVar2;
  }
  else {
    local_10 = 0;
  }
  return local_10;
}


// ---- __ZN6FTMesh5BeginEm @ 0x44ada (94 bytes)

/* FTMesh::Begin(unsigned long) */

void __thiscall FTMesh::Begin(FTMesh *this,ulong param_1)

{
  FTTesselation *this_00;
  
  this_00 = (FTTesselation *)operator_new(0x14);
  FTTesselation::FTTesselation(this_00,param_1);
  *(FTTesselation **)this = this_00;
  return;
}


// ---- __Z9ftglBeginmP6FTMesh @ 0x44b38 (32 bytes)

/* ftglBegin(unsigned long, FTMesh*) */

void ftglBegin(ulong param_1,FTMesh *param_2)

{
  FTMesh::Begin(param_2,param_1);
  return;
}


// ---- __ZN6FTMesh8AddPointEddd @ 0x44b58 (96 bytes)

/* FTMesh::AddPoint(double, double, double) */

void FTMesh::AddPoint(double param_1,double param_2,double param_3)

{
  FTTesselation::AddPoint((double)CONCAT44(param_1._4_4_,*param_1._0_4_),param_2,param_3);
  return;
}


// ---- __Z10ftglVertexPvP6FTMesh @ 0x44bb8 (76 bytes)

/* ftglVertex(void*, FTMesh*) */

void ftglVertex(void *param_1,FTMesh *param_2)

{
                    /* WARNING: Load size is inaccurate */
  FTMesh::AddPoint((double)CONCAT44((int)*param_1,param_2),
                   (double)CONCAT44((int)*(undefined8 *)((int)param_1 + 8),
                                    (int)((ulonglong)*param_1 >> 0x20)),
                   (double)CONCAT44((int)*(undefined8 *)((int)param_1 + 0x10),
                                    (int)((ulonglong)*(undefined8 *)((int)param_1 + 8) >> 0x20)));
  return;
}


// ---- __ZN6FTMesh3EndEv @ 0x44c04 (36 bytes)

/* FTMesh::End() */

void __thiscall FTMesh::End(FTMesh *this)

{
  FTVector<FTTesselation*>::push_back((FTTesselation **)(this + 4));
  return;
}


// ---- __Z7ftglEndP6FTMesh @ 0x44c28 (26 bytes)

/* ftglEnd(FTMesh*) */

void ftglEnd(FTMesh *param_1)

{
  FTMesh::End(param_1);
  return;
}


// ---- __ZN6FTMeshC1Ev @ 0x44c42 (150 bytes)

/* FTMesh::FTMesh() */

void __thiscall FTMesh::FTMesh(FTMesh *this)

{
  *(undefined4 *)this = 0;
  FTVector<FTTesselation*>::FTVector();
  FTList<FTPoint>::FTList();
  *(undefined4 *)(this + 0x20) = 0;
  FTVector<FTTesselation*>::reserve((ulong)(this + 4));
  return;
}


// ---- __ZN12FTVectoriser8MakeMeshEd @ 0x44cd8 (710 bytes)

/* FTVectoriser::MakeMesh(double) */

void FTVectoriser::MakeMesh(double param_1)

{
  FTMesh *pFVar1;
  undefined4 uVar2;
  FTPoint *this;
  double *pdVar3;
  uint uVar4;
  uint uVar5;
  undefined4 in_stack_0000000c;
  undefined4 local_1c;
  undefined4 local_14;
  
  if ((*(int *)(param_1._0_4_ + 8) != 0) &&
     (pFVar1 = *(FTMesh **)(param_1._0_4_ + 8), pFVar1 != (FTMesh *)0x0)) {
    FTMesh::~FTMesh(pFVar1);
    operator_delete(pFVar1);
  }
  pFVar1 = (FTMesh *)operator_new(0x24);
  FTMesh::FTMesh(pFVar1);
  *(FTMesh **)(param_1._0_4_ + 8) = pFVar1;
  uVar2 = _gluNewTess();
  _gluTessCallback(uVar2,0x1870a,PTR_ftglBegin_001c4a80);
  _gluTessCallback(uVar2,0x1870b,PTR_ftglVertex_001c4a84);
  _gluTessCallback(uVar2,0x1870f,PTR_ftglCombine_001c4a98);
  _gluTessCallback(uVar2,0x1870c,PTR_ftglEnd_001c4a90);
  _gluTessCallback(uVar2,0x1870d,PTR_ftglError_001c4a8c);
  if ((*(uint *)(param_1._0_4_ + 0x10) >> 1 & 1) == 0) {
    _gluTessProperty(uVar2,0x1872c,0,0x40f87230);
  }
  else {
    _gluTessProperty(uVar2,0x1872c,0,0x40f87220);
  }
  _gluTessProperty(uVar2,0x1872e,0,0);
  _gluTessNormal(uVar2,0,0,0,0,CONCAT44(in_stack_0000000c,param_1._4_4_));
  _gluTessBeginPolygon(uVar2,*(undefined4 *)(param_1._0_4_ + 8));
  local_1c = 0;
  while( true ) {
    uVar5 = ::FTVectoriser::ContourCount();
    if (uVar5 <= local_1c) break;
    uVar5 = *(uint *)(*(int *)(param_1._0_4_ + 4) + local_1c * 4);
    _gluTessBeginContour(uVar2);
    local_14 = 0;
    while( true ) {
      uVar4 = FTContour::PointCount();
      if (uVar4 <= local_14) break;
      this = (FTPoint *)FTContour::Point(uVar5);
      pdVar3 = FTPoint::operator_cast_to_double_(this);
      _gluTessVertex(uVar2,pdVar3,pdVar3);
      local_14 = local_14 + 1;
    }
    _gluTessEndContour(uVar2);
    local_1c = local_1c + 1;
  }
  _gluTessEndPolygon(uVar2);
  _gluDeleteTess(uVar2);
  return;
}


// ---- __ZN6FTMeshC2Ev @ 0x44f9e (149 bytes)

/* FTMesh::FTMesh() */

void __thiscall FTMesh::FTMesh(FTMesh *this)

{
  *(undefined4 *)this = 0;
  FTVector<FTTesselation*>::FTVector();
  FTList<FTPoint>::FTList();
  *(undefined4 *)(this + 0x20) = 0;
  FTVector<FTTesselation*>::reserve((ulong)(this + 4));
  return;
}


// ---- __ZN6FTMesh5ErrorEm @ 0x45034 (24 bytes)

/* FTMesh::Error(unsigned long) */

void __thiscall FTMesh::Error(FTMesh *this,ulong param_1)

{
  *(ulong *)(this + 0x20) = param_1;
  return;
}


// ---- __ZN8FTVectorIP13FTTesselationEC1Ev @ 0x4504c (68 bytes)

/* FTVector<FTTesselation*>::FTVector() */

void __thiscall FTVector<FTTesselation*>::FTVector(FTVector<FTTesselation*> *this)

{
  *(undefined **)this = PTR_001c4a88 + 8;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 4) = *(undefined4 *)(this + 8);
  *(undefined4 *)(this + 0xc) = 0;
  return;
}


// ---- __ZN6FTListI7FTPointED1Ev @ 0x45090 (56 bytes)

/* FTList<FTPoint>::~FTList() */

void __thiscall FTList<FTPoint>::~FTList(FTList<FTPoint> *this)

{
  undefined4 *puVar1;
  undefined4 *puStack_10;
  
  puStack_10 = *(undefined4 **)(this + 4);
  while (puStack_10 != (undefined4 *)0x0) {
    puVar1 = (undefined4 *)*puStack_10;
    operator_delete(puStack_10);
    puStack_10 = puVar1;
  }
  return;
}


// ---- __ZNK6FTListI7FTPointE4backEv @ 0x450c8 (24 bytes)

/* FTList<FTPoint>::back() const */

int __thiscall FTList<FTPoint>::back(FTList<FTPoint> *this)

{
  return *(int *)(this + 8) + 4;
}


// ---- __ZN9FTContourD1Ev @ 0x450e0 (36 bytes)

/* FTContour::~FTContour() */

void __thiscall FTContour::~FTContour(FTContour *this)

{
  FTVector<FTPoint>::clear();
  FTVector<FTPoint>::~FTVector((FTVector<FTPoint> *)this);
  return;
}


// ---- __ZN13FTTesselationD1Ev @ 0x45104 (36 bytes)

/* FTTesselation::~FTTesselation() */

void __thiscall FTTesselation::~FTTesselation(FTTesselation *this)

{
  FTVector<FTPoint>::clear();
  FTVector<FTPoint>::~FTVector((FTVector<FTPoint> *)this);
  return;
}


// ---- __ZN8FTVectorIP13FTTesselationE5clearEv @ 0x45128 (80 bytes)

/* FTVector<FTTesselation*>::clear() */

void __thiscall FTVector<FTTesselation*>::clear(FTVector<FTTesselation*> *this)

{
  if (*(int *)(this + 4) != 0) {
    if (*(int *)(this + 0xc) != 0) {
      operator_delete__(*(void **)(this + 0xc));
    }
    *(undefined4 *)(this + 8) = 0;
    *(undefined4 *)(this + 4) = *(undefined4 *)(this + 8);
    *(undefined4 *)(this + 0xc) = 0;
  }
  return;
}


// ---- __ZN8FTVectorIP13FTTesselationED1Ev @ 0x45178 (72 bytes)

/* WARNING: Removing unreachable block (ram,0x000451af) */
/* FTVector<FTTesselation*>::~FTVector() */

void __thiscall FTVector<FTTesselation*>::~FTVector(FTVector<FTTesselation*> *this)

{
  *(undefined **)this = PTR_001c4a88 + 8;
  FTVector<FTTesselation*>::clear();
  return;
}


// ---- __ZN8FTVectorIP13FTTesselationED0Ev @ 0x451c0 (72 bytes)

/* FTVector<FTTesselation*>::~FTVector() */

void __thiscall FTVector<FTTesselation*>::~FTVector(FTVector<FTTesselation*> *this)

{
  *(undefined **)this = PTR_001c4a88 + 8;
  FTVector<FTTesselation*>::clear();
  operator_delete(this);
  return;
}


// ---- __ZN6FTListI7FTPointE4NodeC1Ev @ 0x45208 (38 bytes)

/* FTList<FTPoint>::Node::Node() */

void __thiscall FTList<FTPoint>::Node::Node(Node *this)

{
  *(undefined4 *)this = 0;
  FTPoint::FTPoint((FTPoint *)(this + 4));
  return;
}


// ---- __ZN6FTListI7FTPointEC1Ev @ 0x4522e (76 bytes)

/* FTList<FTPoint>::FTList() */

void __thiscall FTList<FTPoint>::FTList(FTList<FTPoint> *this)

{
  Node *this_00;
  
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 8) = 0;
  this_00 = (Node *)operator_new(0x1c);
  FTList<FTPoint>::Node::Node(this_00);
  *(Node **)(this + 4) = this_00;
  return;
}


// ---- __ZNK8FTVectorIP13FTTesselationE8capacityEv @ 0x4527a (20 bytes)

/* FTVector<FTTesselation*>::capacity() const */

undefined4 __thiscall FTVector<FTTesselation*>::capacity(FTVector<FTTesselation*> *this)

{
  return *(undefined4 *)(this + 4);
}


// ---- __ZN8FTVectorIP13FTTesselationE5beginEv @ 0x4528e (20 bytes)

/* FTVector<FTTesselation*>::begin() */

undefined4 __thiscall FTVector<FTTesselation*>::begin(FTVector<FTTesselation*> *this)

{
  return *(undefined4 *)(this + 0xc);
}


// ---- __ZN8FTVectorIP13FTTesselationEixEm @ 0x452a2 (36 bytes)

/* FTVector<FTTesselation*>::operator[](unsigned long) */

int __thiscall FTVector<FTTesselation*>::operator[](FTVector<FTTesselation*> *this,ulong param_1)

{
  int iVar1;
  
  iVar1 = FTVector<FTTesselation*>::begin();
  return iVar1 + param_1 * 4;
}


// ---- __ZN6FTListI7FTPointE4NodeC1ERKS0_ @ 0x452c6 (78 bytes)

/* FTList<FTPoint>::Node::Node(FTPoint const&) */

void __thiscall FTList<FTPoint>::Node::Node(Node *this,FTPoint *param_1)

{
  *(undefined4 *)this = 0;
  FTPoint::FTPoint((FTPoint *)(this + 4));
  *(undefined4 *)(this + 4) = *(undefined4 *)param_1;
  *(undefined4 *)(this + 8) = *(undefined4 *)(param_1 + 4);
  *(undefined4 *)(this + 0xc) = *(undefined4 *)(param_1 + 8);
  *(undefined4 *)(this + 0x10) = *(undefined4 *)(param_1 + 0xc);
  *(undefined4 *)(this + 0x14) = *(undefined4 *)(param_1 + 0x10);
  *(undefined4 *)(this + 0x18) = *(undefined4 *)(param_1 + 0x14);
  return;
}


// ---- __ZN6FTListI7FTPointE9push_backERKS0_ @ 0x45314 (118 bytes)

/* FTList<FTPoint>::push_back(FTPoint const&) */

void __thiscall FTList<FTPoint>::push_back(FTList<FTPoint> *this,FTPoint *param_1)

{
  Node *this_00;
  
  this_00 = (Node *)operator_new(0x1c);
  FTList<FTPoint>::Node::Node(this_00,param_1);
  if (**(int **)(this + 4) == 0) {
    **(undefined4 **)(this + 4) = this_00;
  }
  if (*(int *)(this + 8) != 0) {
    **(undefined4 **)(this + 8) = this_00;
  }
  *(Node **)(this + 8) = this_00;
  *(int *)this = *(int *)this + 1;
  return;
}


// ---- __ZNK8FTVectorIP13FTTesselationE5beginEv @ 0x4538a (20 bytes)

/* FTVector<FTTesselation*>::begin() const */

undefined4 __thiscall FTVector<FTTesselation*>::begin(FTVector<FTTesselation*> *this)

{
  return *(undefined4 *)(this + 0xc);
}


// ---- __ZNK8FTVectorIP13FTTesselationEixEm @ 0x4539e (36 bytes)

/* FTVector<FTTesselation*>::operator[](unsigned long) const */

int __thiscall FTVector<FTTesselation*>::operator[](FTVector<FTTesselation*> *this,ulong param_1)

{
  int iVar1;
  
  iVar1 = FTVector<FTTesselation*>::begin();
  return iVar1 + param_1 * 4;
}


// ---- __ZN8FTVectorI7FTPointE7reserveEm @ 0x453c2 (54 bytes)

/* FTVector<FTPoint>::reserve(unsigned long) */

void __thiscall FTVector<FTPoint>::reserve(FTVector<FTPoint> *this,ulong param_1)

{
  uint uVar1;
  
  uVar1 = FTVector<FTPoint>::capacity();
  if (uVar1 < param_1) {
    FTVector<FTPoint>::expand((ulong)this);
  }
  return;
}


// ---- __ZN13FTTesselationC1Em @ 0x453f8 (92 bytes)

/* FTTesselation::FTTesselation(unsigned long) */

void __thiscall FTTesselation::FTTesselation(FTTesselation *this,ulong param_1)

{
  FTVector<FTPoint>::FTVector();
  *(ulong *)(this + 0x10) = param_1;
  FTVector<FTPoint>::reserve((ulong)this);
  return;
}


// ---- __ZN13FTTesselation8AddPointEddd @ 0x45454 (112 bytes)

/* FTTesselation::AddPoint(double, double, double) */

void FTTesselation::AddPoint(double param_1,double param_2,double param_3)

{
  undefined4 in_stack_0000001c;
  FTPoint aFStack_24 [32];
  
  FTPoint::FTPoint(aFStack_24,(double)CONCAT44(param_2._0_4_,param_1._4_4_),
                   (double)CONCAT44(param_3._0_4_,param_2._4_4_),
                   (double)CONCAT44(in_stack_0000001c,param_3._4_4_));
  FTVector<FTPoint>::push_back(param_1._0_4_);
  return;
}


// ---- __ZN8FTVectorIP13FTTesselationE3endEv @ 0x454c4 (50 bytes)

/* FTVector<FTTesselation*>::end() */

int FTVector<FTTesselation*>::end(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FTVector<FTTesselation*>::begin();
  iVar2 = FTVector<FTTesselation*>::size();
  return iVar1 + iVar2 * 4;
}


// ---- __ZN8FTVectorIP13FTTesselationE6expandEm @ 0x454f6 (216 bytes)

/* FTVector<FTTesselation*>::expand(unsigned long) */

void __thiscall FTVector<FTTesselation*>::expand(FTVector<FTTesselation*> *this,ulong param_1)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  uint uStack_30;
  uint uStack_20;
  undefined4 *puStack_18;
  undefined4 *puStack_10;
  
  iVar1 = FTVector<FTTesselation*>::capacity();
  if (iVar1 == 0) {
    uStack_30 = 0x100;
  }
  else {
    iVar1 = FTVector<FTTesselation*>::capacity();
    uStack_30 = iVar1 * 2;
  }
  uStack_20 = uStack_30;
  if (param_1 != 0) {
    for (; uStack_20 < param_1; uStack_20 = uStack_20 << 1) {
    }
  }
  puVar2 = (undefined4 *)operator_new__(uStack_20 << 2);
  puStack_18 = (undefined4 *)FTVector<FTTesselation*>::begin();
  puVar3 = (undefined4 *)FTVector<FTTesselation*>::end();
  puStack_10 = puVar2;
  for (; puStack_18 != puVar3; puStack_18 = puStack_18 + 1) {
    *puStack_10 = *puStack_18;
    puStack_10 = puStack_10 + 1;
  }
  if ((*(int *)(this + 4) != 0) && (*(int *)(this + 0xc) != 0)) {
    operator_delete__(*(void **)(this + 0xc));
  }
  *(undefined4 **)(this + 0xc) = puVar2;
  *(uint *)(this + 4) = uStack_20;
  return;
}


// ---- __ZN8FTVectorIP13FTTesselationE9push_backERKS1_ @ 0x455ce (120 bytes)

/* FTVector<FTTesselation*>::push_back(FTTesselation* const&) */

void __thiscall
FTVector<FTTesselation*>::push_back(FTVector<FTTesselation*> *this,FTTesselation **param_1)

{
  FTTesselation *pFVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  
  iVar2 = FTVector<FTTesselation*>::size();
  iVar3 = FTVector<FTTesselation*>::capacity();
  if (iVar2 == iVar3) {
    FTVector<FTTesselation*>::expand((ulong)this);
  }
  pFVar1 = *param_1;
  FTVector<FTTesselation*>::size();
  puVar4 = (undefined4 *)FTVector<FTTesselation*>::operator[]((ulong)this);
  *puVar4 = pFVar1;
  *(int *)(this + 8) = *(int *)(this + 8) + 1;
  return;
}


// ---- __ZN8FTVectorIP13FTTesselationE7reserveEm @ 0x45646 (53 bytes)

/* FTVector<FTTesselation*>::reserve(unsigned long) */

void __thiscall FTVector<FTTesselation*>::reserve(FTVector<FTTesselation*> *this,ulong param_1)

{
  uint uVar1;
  
  uVar1 = FTVector<FTTesselation*>::capacity();
  if (uVar1 < param_1) {
    FTVector<FTTesselation*>::expand((ulong)this);
  }
  return;
}


