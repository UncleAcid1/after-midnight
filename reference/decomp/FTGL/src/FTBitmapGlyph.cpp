// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTBitmapGlyph.cpp

// ---- __ZN13FTBitmapGlyphC2EP16FT_GlyphSlotRec_ @ 0x37db0 (554 bytes)

/* FTBitmapGlyph::FTBitmapGlyph(FT_GlyphSlotRec_*) */

void __thiscall FTBitmapGlyph::FTBitmapGlyph(FTBitmapGlyph *this,FT_GlyphSlotRec_ *param_1)

{
  undefined4 uVar1;
  void *pvVar2;
  int iVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  uint uStack_30;
  int iStack_2c;
  undefined1 *puStack_28;
  undefined1 *puStack_24;
  uint uStack_20;
  
  FTGlyph::FTGlyph((FTGlyph *)this,param_1,true);
  *(undefined **)this = PTR_vtable_001c4a10 + 8;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  FTPoint::FTPoint((FTPoint *)(this + 0x48));
  *(undefined4 *)(this + 0x60) = 0;
  uVar1 = _FT_Render_Glyph(param_1,2);
  *(undefined4 *)(this + 0x38) = uVar1;
  if ((*(int *)(this + 0x38) == 0) && (*(int *)(param_1 + 0x48) == 0x62697473)) {
    uStack_30 = *(uint *)(param_1 + 0x4c);
    uStack_34 = *(undefined4 *)(param_1 + 0x50);
    iStack_2c = *(int *)(param_1 + 0x54);
    puVar4 = *(undefined1 **)(param_1 + 0x58);
    *(undefined4 *)(this + 0x3c) = uStack_34;
    *(uint *)(this + 0x40) = uStack_30;
    *(int *)(this + 0x44) = iStack_2c;
    if ((*(int *)(this + 0x3c) != 0) && (*(int *)(this + 0x40) != 0)) {
      pvVar2 = operator_new__(*(int *)(this + 0x40) * *(int *)(this + 0x44));
      *(void **)(this + 0x60) = pvVar2;
      puStack_28 = (undefined1 *)
                   (*(int *)(this + 0x60) + *(int *)(this + 0x44) * (*(int *)(this + 0x40) + -1));
      puStack_24 = puVar4;
      for (uStack_20 = 0; iVar3 = iStack_2c, puVar4 = puStack_24, puVar5 = puStack_28,
          uStack_20 < uStack_30; uStack_20 = uStack_20 + 1) {
        for (; iVar3 != 0; iVar3 = iVar3 + -1) {
          *puVar5 = *puVar4;
          puVar4 = puVar4 + 1;
          puVar5 = puVar5 + 1;
        }
        puStack_28 = puStack_28 + -*(int *)(this + 0x44);
        puStack_24 = puStack_24 + iStack_2c;
      }
    }
    FTPoint::FTPoint((FTPoint *)&uStack_4c,(double)*(int *)(param_1 + 100),
                     (double)(int)(uStack_30 - *(int *)(param_1 + 0x68)),0.0);
    *(undefined4 *)(this + 0x48) = uStack_4c;
    *(undefined4 *)(this + 0x4c) = uStack_48;
    *(undefined4 *)(this + 0x50) = uStack_44;
    *(undefined4 *)(this + 0x54) = uStack_40;
    *(undefined4 *)(this + 0x58) = uStack_3c;
    *(undefined4 *)(this + 0x5c) = uStack_38;
  }
  return;
}


// ---- __ZN13FTBitmapGlyphC1EP16FT_GlyphSlotRec_ @ 0x37fda (554 bytes)

/* FTBitmapGlyph::FTBitmapGlyph(FT_GlyphSlotRec_*) */

void __thiscall FTBitmapGlyph::FTBitmapGlyph(FTBitmapGlyph *this,FT_GlyphSlotRec_ *param_1)

{
  undefined4 uVar1;
  void *pvVar2;
  int iVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  uint local_30;
  int local_2c;
  undefined1 *local_28;
  undefined1 *local_24;
  uint local_20;
  
  FTGlyph::FTGlyph((FTGlyph *)this,param_1,true);
  *(undefined **)this = PTR_vtable_001c4a10 + 8;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  FTPoint::FTPoint((FTPoint *)(this + 0x48));
  *(undefined4 *)(this + 0x60) = 0;
  uVar1 = _FT_Render_Glyph(param_1,2);
  *(undefined4 *)(this + 0x38) = uVar1;
  if ((*(int *)(this + 0x38) == 0) && (*(int *)(param_1 + 0x48) == 0x62697473)) {
    local_30 = *(uint *)(param_1 + 0x4c);
    local_34 = *(undefined4 *)(param_1 + 0x50);
    local_2c = *(int *)(param_1 + 0x54);
    puVar4 = *(undefined1 **)(param_1 + 0x58);
    *(undefined4 *)(this + 0x3c) = local_34;
    *(uint *)(this + 0x40) = local_30;
    *(int *)(this + 0x44) = local_2c;
    if ((*(int *)(this + 0x3c) != 0) && (*(int *)(this + 0x40) != 0)) {
      pvVar2 = operator_new__(*(int *)(this + 0x40) * *(int *)(this + 0x44));
      *(void **)(this + 0x60) = pvVar2;
      local_28 = (undefined1 *)
                 (*(int *)(this + 0x60) + *(int *)(this + 0x44) * (*(int *)(this + 0x40) + -1));
      local_24 = puVar4;
      for (local_20 = 0; iVar3 = local_2c, puVar4 = local_24, puVar5 = local_28, local_20 < local_30
          ; local_20 = local_20 + 1) {
        for (; iVar3 != 0; iVar3 = iVar3 + -1) {
          *puVar5 = *puVar4;
          puVar4 = puVar4 + 1;
          puVar5 = puVar5 + 1;
        }
        local_28 = local_28 + -*(int *)(this + 0x44);
        local_24 = local_24 + local_2c;
      }
    }
    FTPoint::FTPoint((FTPoint *)&local_4c,(double)*(int *)(param_1 + 100),
                     (double)(int)(local_30 - *(int *)(param_1 + 0x68)),0.0);
    *(undefined4 *)(this + 0x48) = local_4c;
    *(undefined4 *)(this + 0x4c) = local_48;
    *(undefined4 *)(this + 0x50) = local_44;
    *(undefined4 *)(this + 0x54) = local_40;
    *(undefined4 *)(this + 0x58) = local_3c;
    *(undefined4 *)(this + 0x5c) = local_38;
  }
  return;
}


// ---- __ZN13FTBitmapGlyphD2Ev @ 0x38204 (96 bytes)

/* WARNING: Removing unreachable block (ram,0x00038253) */
/* FTBitmapGlyph::~FTBitmapGlyph() */

void __thiscall FTBitmapGlyph::~FTBitmapGlyph(FTBitmapGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a10 + 8;
  if (*(int *)(this + 0x60) != 0) {
    operator_delete__(*(void **)(this + 0x60));
  }
  FTGlyph::~FTGlyph((FTGlyph *)this);
  return;
}


// ---- __ZN13FTBitmapGlyphD1Ev @ 0x38264 (96 bytes)

/* WARNING: Removing unreachable block (ram,0x000382b3) */
/* FTBitmapGlyph::~FTBitmapGlyph() */

void __thiscall FTBitmapGlyph::~FTBitmapGlyph(FTBitmapGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a10 + 8;
  if (*(int *)(this + 0x60) != 0) {
    operator_delete__(*(void **)(this + 0x60));
  }
  FTGlyph::~FTGlyph((FTGlyph *)this);
  return;
}


// ---- __ZN13FTBitmapGlyphD0Ev @ 0x382c4 (96 bytes)

/* FTBitmapGlyph::~FTBitmapGlyph() */

void __thiscall FTBitmapGlyph::~FTBitmapGlyph(FTBitmapGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a10 + 8;
  if (*(int *)(this + 0x60) != 0) {
    operator_delete__(*(void **)(this + 0x60));
  }
  FTGlyph::~FTGlyph((FTGlyph *)this);
  operator_delete(this);
  return;
}


// ---- __ZN13FTBitmapGlyph6RenderERK7FTPoint @ 0x38324 (448 bytes)

/* FTBitmapGlyph::Render(FTPoint const&) */

FTPoint * FTBitmapGlyph::Render(FTPoint *param_1)

{
  longdouble lVar1;
  longdouble lVar2;
  longdouble lVar3;
  longdouble lVar4;
  
  lVar1 = (longdouble)FTPoint::Y();
  lVar2 = (longdouble)FTPoint::Y();
  lVar3 = (longdouble)FTPoint::X();
  lVar4 = (longdouble)FTPoint::X();
  _glBitmap(0,0,0,0,(float)((double)lVar3 + (double)lVar4),(float)((double)lVar1 - (double)lVar2),0)
  ;
  if (*(int *)(param_1 + 0x60) != 0) {
    _glPixelStorei(0xcf2,*(int *)(param_1 + 0x44) << 3);
    _glBitmap(*(undefined4 *)(param_1 + 0x3c),*(undefined4 *)(param_1 + 0x40),0,0,0,0,
              *(undefined4 *)(param_1 + 0x60));
  }
  lVar1 = (longdouble)FTPoint::Y();
  lVar2 = (longdouble)FTPoint::X();
  _glBitmap(0,0,0,0,-(float)lVar2,(float)lVar1,0);
  return param_1 + 4;
}


// ---- __ZN7FTPointC1Ev @ 0x384e4 (74 bytes)

/* FTPoint::FTPoint() */

void __thiscall FTPoint::FTPoint(FTPoint *this)

{
  *(undefined8 *)this = 0;
  *(undefined8 *)(this + 8) = 0;
  *(undefined8 *)(this + 0x10) = 0;
  return;
}


// ---- __ZN7FTPointC1Eddd @ 0x3852e (88 bytes)

/* FTPoint::FTPoint(double, double, double) */

void __thiscall FTPoint::FTPoint(FTPoint *this,double param_1,double param_2,double param_3)

{
  *(double *)this = param_1;
  *(double *)(this + 8) = param_2;
  *(double *)(this + 0x10) = param_3;
  return;
}


// ---- __ZNK7FTPoint1XEv @ 0x38586 (30 bytes)

/* FTPoint::X() const */

longdouble __thiscall FTPoint::X(FTPoint *this)

{
  return (longdouble)*(double *)this;
}


// ---- __ZNK7FTPoint1YEv @ 0x385a4 (30 bytes)

/* FTPoint::Y() const */

longdouble __thiscall FTPoint::Y(FTPoint *this)

{
  return (longdouble)*(double *)(this + 8);
}


