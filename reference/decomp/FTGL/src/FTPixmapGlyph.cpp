// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTPixmapGlyph.cpp

// ---- __ZN13FTPixmapGlyphC2EP16FT_GlyphSlotRec_ @ 0x420e4 (542 bytes)

/* FTPixmapGlyph::FTPixmapGlyph(FT_GlyphSlotRec_*) */

void __thiscall FTPixmapGlyph::FTPixmapGlyph(FTPixmapGlyph *this,FT_GlyphSlotRec_ *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  void *pvVar5;
  undefined1 *puStack_20;
  undefined1 *puStack_1c;
  int iStack_14;
  int iStack_10;
  
  FTGlyph::FTGlyph((FTGlyph *)this,param_1,true);
  *(undefined **)this = PTR_vtable_001c4a70 + 8;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  FTPoint::FTPoint((FTPoint *)(this + 0x44));
  *(undefined4 *)(this + 0x5c) = 0;
  uVar4 = _FT_Render_Glyph(param_1,0);
  *(undefined4 *)(this + 0x38) = uVar4;
  if ((*(int *)(this + 0x38) == 0) && (*(int *)(param_1 + 0x48) == 0x62697473)) {
    iVar1 = *(int *)(param_1 + 0x4c);
    iVar2 = *(int *)(param_1 + 0x50);
    puStack_20 = *(undefined1 **)(param_1 + 0x58);
    *(int *)(this + 0x3c) = iVar2;
    *(int *)(this + 0x40) = iVar1;
    if ((*(int *)(this + 0x3c) != 0) && (*(int *)(this + 0x40) != 0)) {
      pvVar5 = operator_new__(*(int *)(this + 0x40) * *(int *)(this + 0x3c) * 2);
      *(void **)(this + 0x5c) = pvVar5;
      puStack_1c = (undefined1 *)
                   (*(int *)(this + 0x5c) + *(int *)(this + 0x3c) * (*(int *)(this + 0x40) * 2 + -2)
                   );
      iVar3 = *(int *)(this + 0x3c);
      for (iStack_14 = 0; iStack_14 < iVar1; iStack_14 = iStack_14 + 1) {
        for (iStack_10 = 0; iStack_10 < iVar2; iStack_10 = iStack_10 + 1) {
          *puStack_1c = 0xff;
          puStack_1c[1] = *puStack_20;
          puStack_20 = puStack_20 + 1;
          puStack_1c = puStack_1c + 2;
        }
        puStack_1c = puStack_1c + iVar3 * -4;
      }
      *(int *)(this + 0x40) = iVar1;
    }
    FTPoint::X((double)CONCAT44(SUB84((double)*(int *)(param_1 + 100),0),this + 0x44));
    FTPoint::Y((double)CONCAT44(SUB84((double)(iVar1 - *(int *)(param_1 + 0x68)),0),this + 0x44));
  }
  return;
}


// ---- __ZN13FTPixmapGlyphC1EP16FT_GlyphSlotRec_ @ 0x42302 (542 bytes)

/* FTPixmapGlyph::FTPixmapGlyph(FT_GlyphSlotRec_*) */

void __thiscall FTPixmapGlyph::FTPixmapGlyph(FTPixmapGlyph *this,FT_GlyphSlotRec_ *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  void *pvVar5;
  undefined1 *local_20;
  undefined1 *local_1c;
  int local_14;
  int local_10;
  
  FTGlyph::FTGlyph((FTGlyph *)this,param_1,true);
  *(undefined **)this = PTR_vtable_001c4a70 + 8;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  FTPoint::FTPoint((FTPoint *)(this + 0x44));
  *(undefined4 *)(this + 0x5c) = 0;
  uVar4 = _FT_Render_Glyph(param_1,0);
  *(undefined4 *)(this + 0x38) = uVar4;
  if ((*(int *)(this + 0x38) == 0) && (*(int *)(param_1 + 0x48) == 0x62697473)) {
    iVar1 = *(int *)(param_1 + 0x4c);
    iVar2 = *(int *)(param_1 + 0x50);
    local_20 = *(undefined1 **)(param_1 + 0x58);
    *(int *)(this + 0x3c) = iVar2;
    *(int *)(this + 0x40) = iVar1;
    if ((*(int *)(this + 0x3c) != 0) && (*(int *)(this + 0x40) != 0)) {
      pvVar5 = operator_new__(*(int *)(this + 0x40) * *(int *)(this + 0x3c) * 2);
      *(void **)(this + 0x5c) = pvVar5;
      local_1c = (undefined1 *)
                 (*(int *)(this + 0x5c) + *(int *)(this + 0x3c) * (*(int *)(this + 0x40) * 2 + -2));
      iVar3 = *(int *)(this + 0x3c);
      for (local_14 = 0; local_14 < iVar1; local_14 = local_14 + 1) {
        for (local_10 = 0; local_10 < iVar2; local_10 = local_10 + 1) {
          *local_1c = 0xff;
          local_1c[1] = *local_20;
          local_20 = local_20 + 1;
          local_1c = local_1c + 2;
        }
        local_1c = local_1c + iVar3 * -4;
      }
      *(int *)(this + 0x40) = iVar1;
    }
    FTPoint::X((double)CONCAT44(SUB84((double)*(int *)(param_1 + 100),0),this + 0x44));
    FTPoint::Y((double)CONCAT44(SUB84((double)(iVar1 - *(int *)(param_1 + 0x68)),0),this + 0x44));
  }
  return;
}


// ---- __ZN13FTPixmapGlyphD2Ev @ 0x42520 (96 bytes)

/* WARNING: Removing unreachable block (ram,0x0004256f) */
/* FTPixmapGlyph::~FTPixmapGlyph() */

void __thiscall FTPixmapGlyph::~FTPixmapGlyph(FTPixmapGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a70 + 8;
  if (*(int *)(this + 0x5c) != 0) {
    operator_delete__(*(void **)(this + 0x5c));
  }
  FTGlyph::~FTGlyph((FTGlyph *)this);
  return;
}


// ---- __ZN13FTPixmapGlyphD1Ev @ 0x42580 (96 bytes)

/* WARNING: Removing unreachable block (ram,0x000425cf) */
/* FTPixmapGlyph::~FTPixmapGlyph() */

void __thiscall FTPixmapGlyph::~FTPixmapGlyph(FTPixmapGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a70 + 8;
  if (*(int *)(this + 0x5c) != 0) {
    operator_delete__(*(void **)(this + 0x5c));
  }
  FTGlyph::~FTGlyph((FTGlyph *)this);
  return;
}


// ---- __ZN13FTPixmapGlyphD0Ev @ 0x425e0 (96 bytes)

/* FTPixmapGlyph::~FTPixmapGlyph() */

void __thiscall FTPixmapGlyph::~FTPixmapGlyph(FTPixmapGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a70 + 8;
  if (*(int *)(this + 0x5c) != 0) {
    operator_delete__(*(void **)(this + 0x5c));
  }
  FTGlyph::~FTGlyph((FTGlyph *)this);
  operator_delete(this);
  return;
}


// ---- __ZN13FTPixmapGlyph6RenderERK7FTPoint @ 0x42640 (425 bytes)

/* FTPixmapGlyph::Render(FTPoint const&) */

FTPoint * FTPixmapGlyph::Render(FTPoint *param_1)

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
  if (*(int *)(param_1 + 0x5c) != 0) {
    _glPixelStorei(0xcf2,0);
    _glPixelStorei(0xcf5,2);
    _glDrawPixels(*(undefined4 *)(param_1 + 0x3c),*(undefined4 *)(param_1 + 0x40),
                  "k/Versions/A/Carbon",0x1401,*(undefined4 *)(param_1 + 0x5c));
  }
  lVar1 = (longdouble)FTPoint::Y();
  lVar2 = (longdouble)FTPoint::X();
  _glBitmap(0,0,0,0,-(float)lVar2,(float)lVar1,0);
  return param_1 + 4;
}


