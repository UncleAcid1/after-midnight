// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTTextureGlyph.cpp

// ---- __ZN14FTTextureGlyphC2EP16FT_GlyphSlotRec_iiill @ 0x4387e (764 bytes)

/* FTTextureGlyph::FTTextureGlyph(FT_GlyphSlotRec_*, int, int, int, long, long) */

void __thiscall
FTTextureGlyph::FTTextureGlyph
          (FTTextureGlyph *this,FT_GlyphSlotRec_ *param_1,int param_2,int param_3,int param_4,
          long param_5,long param_6)

{
  undefined4 uVar1;
  undefined4 uVar2;
  FTPoint *pFStack_34;
  int iStack_30;
  
  FTGlyph::FTGlyph((FTGlyph *)this,param_1,true);
  *(undefined **)this = PTR_vtable_001c4a7c + 8;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  FTPoint::FTPoint((FTPoint *)(this + 0x44));
  pFStack_34 = (FTPoint *)(this + 0x5c);
  for (iStack_30 = 1; iStack_30 != -1; iStack_30 = iStack_30 + -1) {
    FTPoint::FTPoint(pFStack_34);
    pFStack_34 = pFStack_34 + 0x18;
  }
  *(int *)(this + 0x8c) = param_2;
  uVar2 = _FT_Render_Glyph(param_1,0);
  *(undefined4 *)(this + 0x38) = uVar2;
  if ((*(int *)(this + 0x38) == 0) && (*(int *)(param_1 + 0x48) == 0x62697473)) {
    uVar2 = *(undefined4 *)(param_1 + 0x4c);
    uVar1 = *(undefined4 *)(param_1 + 0x58);
    *(undefined4 *)(this + 0x3c) = *(undefined4 *)(param_1 + 0x50);
    *(undefined4 *)(this + 0x40) = uVar2;
    if ((*(int *)(this + 0x3c) != 0) && (*(int *)(this + 0x40) != 0)) {
      _glPushClientAttrib(1);
      _glPixelStorei(0xcf1,0);
      _glPixelStorei(0xcf2,0);
      _glPixelStorei(0xcf5,1);
      _glBindTexture(0xde1,*(undefined4 *)(this + 0x8c));
      _glTexSubImage2D(0xde1,0,param_3,param_4,*(undefined4 *)(this + 0x3c),
                       *(undefined4 *)(this + 0x40),"ework/Versions/A/Carbon",0x1401,uVar1);
      _glPopClientAttrib();
    }
    FTPoint::X((double)CONCAT44(SUB84((double)((float)param_3 / (float)param_5),0),this + 0x5c));
    FTPoint::Y((double)CONCAT44(SUB84((double)((float)param_4 / (float)param_6),0),this + 0x5c));
    FTPoint::X((double)CONCAT44(SUB84((double)((float)(*(int *)(this + 0x3c) + param_3) /
                                              (float)param_5),0),this + 0x74));
    FTPoint::Y((double)CONCAT44(SUB84((double)((float)(*(int *)(this + 0x40) + param_4) /
                                              (float)param_6),0),this + 0x74));
    FTPoint::X((double)CONCAT44(SUB84((double)*(int *)(param_1 + 100),0),this + 0x44));
    FTPoint::Y((double)CONCAT44(SUB84((double)*(int *)(param_1 + 0x68),0),this + 0x44));
  }
  return;
}


// ---- __ZN14FTTextureGlyphC1EP16FT_GlyphSlotRec_iiill @ 0x43b7a (764 bytes)

/* FTTextureGlyph::FTTextureGlyph(FT_GlyphSlotRec_*, int, int, int, long, long) */

void __thiscall
FTTextureGlyph::FTTextureGlyph
          (FTTextureGlyph *this,FT_GlyphSlotRec_ *param_1,int param_2,int param_3,int param_4,
          long param_5,long param_6)

{
  undefined4 uVar1;
  undefined4 uVar2;
  FTPoint *local_34;
  int local_30;
  
  FTGlyph::FTGlyph((FTGlyph *)this,param_1,true);
  *(undefined **)this = PTR_vtable_001c4a7c + 8;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  FTPoint::FTPoint((FTPoint *)(this + 0x44));
  local_34 = (FTPoint *)(this + 0x5c);
  for (local_30 = 1; local_30 != -1; local_30 = local_30 + -1) {
    FTPoint::FTPoint(local_34);
    local_34 = local_34 + 0x18;
  }
  *(int *)(this + 0x8c) = param_2;
  uVar2 = _FT_Render_Glyph(param_1,0);
  *(undefined4 *)(this + 0x38) = uVar2;
  if ((*(int *)(this + 0x38) == 0) && (*(int *)(param_1 + 0x48) == 0x62697473)) {
    uVar2 = *(undefined4 *)(param_1 + 0x4c);
    uVar1 = *(undefined4 *)(param_1 + 0x58);
    *(undefined4 *)(this + 0x3c) = *(undefined4 *)(param_1 + 0x50);
    *(undefined4 *)(this + 0x40) = uVar2;
    if ((*(int *)(this + 0x3c) != 0) && (*(int *)(this + 0x40) != 0)) {
      _glPushClientAttrib(1);
      _glPixelStorei(0xcf1,0);
      _glPixelStorei(0xcf2,0);
      _glPixelStorei(0xcf5,1);
      _glBindTexture(0xde1,*(undefined4 *)(this + 0x8c));
      _glTexSubImage2D(0xde1,0,param_3,param_4,*(undefined4 *)(this + 0x3c),
                       *(undefined4 *)(this + 0x40),"ework/Versions/A/Carbon",0x1401,uVar1);
      _glPopClientAttrib();
    }
    FTPoint::X((double)CONCAT44(SUB84((double)((float)param_3 / (float)param_5),0),this + 0x5c));
    FTPoint::Y((double)CONCAT44(SUB84((double)((float)param_4 / (float)param_6),0),this + 0x5c));
    FTPoint::X((double)CONCAT44(SUB84((double)((float)(*(int *)(this + 0x3c) + param_3) /
                                              (float)param_5),0),this + 0x74));
    FTPoint::Y((double)CONCAT44(SUB84((double)((float)(*(int *)(this + 0x40) + param_4) /
                                              (float)param_6),0),this + 0x74));
    FTPoint::X((double)CONCAT44(SUB84((double)*(int *)(param_1 + 100),0),this + 0x44));
    FTPoint::Y((double)CONCAT44(SUB84((double)*(int *)(param_1 + 0x68),0),this + 0x44));
  }
  return;
}


// ---- __ZN14FTTextureGlyphD2Ev @ 0x43e76 (72 bytes)

/* WARNING: Removing unreachable block (ram,0x00043ead) */
/* FTTextureGlyph::~FTTextureGlyph() */

void __thiscall FTTextureGlyph::~FTTextureGlyph(FTTextureGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a7c + 8;
  FTGlyph::~FTGlyph((FTGlyph *)this);
  return;
}


// ---- __ZN14FTTextureGlyphD1Ev @ 0x43ebe (72 bytes)

/* WARNING: Removing unreachable block (ram,0x00043ef5) */
/* FTTextureGlyph::~FTTextureGlyph() */

void __thiscall FTTextureGlyph::~FTTextureGlyph(FTTextureGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a7c + 8;
  FTGlyph::~FTGlyph((FTGlyph *)this);
  return;
}


// ---- __ZN14FTTextureGlyphD0Ev @ 0x43f06 (72 bytes)

/* FTTextureGlyph::~FTTextureGlyph() */

void __thiscall FTTextureGlyph::~FTTextureGlyph(FTTextureGlyph *this)

{
  *(undefined **)this = PTR_vtable_001c4a7c + 8;
  FTGlyph::~FTGlyph((FTGlyph *)this);
  operator_delete(this);
  return;
}


// ---- __ZN14FTTextureGlyph6RenderERK7FTPoint @ 0x43f4e (933 bytes)

/* FTTextureGlyph::Render(FTPoint const&) */

FTPoint * FTTextureGlyph::Render(FTPoint *param_1)

{
  int iVar1;
  int iVar2;
  longdouble lVar3;
  longdouble lVar4;
  
  if (*(int *)(param_1 + 0x8c) != *(int *)PTR_activeTextureID_001c4a54) {
    _glBindTexture(0xde1,*(undefined4 *)(param_1 + 0x8c));
    *(undefined4 *)PTR_activeTextureID_001c4a54 = *(undefined4 *)(param_1 + 0x8c);
  }
  lVar3 = (longdouble)FTPoint::Y();
  lVar4 = (longdouble)FTPoint::X();
  _glTranslatef((float)lVar4,(float)lVar3,0);
  _glBegin(7);
  lVar3 = (longdouble)FTPoint::Y();
  lVar4 = (longdouble)FTPoint::X();
  _glTexCoord2f((float)lVar4,(float)lVar3);
  lVar3 = (longdouble)FTPoint::Y();
  lVar4 = (longdouble)FTPoint::X();
  _glVertex2f((float)lVar4,(float)lVar3);
  lVar3 = (longdouble)FTPoint::Y();
  lVar4 = (longdouble)FTPoint::X();
  _glTexCoord2f((float)lVar4,(float)lVar3);
  lVar3 = (longdouble)FTPoint::Y();
  iVar1 = *(int *)(param_1 + 0x40);
  lVar4 = (longdouble)FTPoint::X();
  _glVertex2f((float)lVar4,(float)lVar3 - (float)iVar1);
  lVar3 = (longdouble)FTPoint::Y();
  lVar4 = (longdouble)FTPoint::X();
  _glTexCoord2f((float)lVar4,(float)lVar3);
  lVar3 = (longdouble)FTPoint::Y();
  iVar1 = *(int *)(param_1 + 0x40);
  iVar2 = *(int *)(param_1 + 0x3c);
  lVar4 = (longdouble)FTPoint::X();
  _glVertex2f((float)iVar2 + (float)lVar4,(float)lVar3 - (float)iVar1);
  lVar3 = (longdouble)FTPoint::Y();
  lVar4 = (longdouble)FTPoint::X();
  _glTexCoord2f((float)lVar4,(float)lVar3);
  lVar3 = (longdouble)FTPoint::Y();
  iVar1 = *(int *)(param_1 + 0x3c);
  lVar4 = (longdouble)FTPoint::X();
  _glVertex2f((float)iVar1 + (float)lVar4,(float)lVar3);
  _glEnd();
  return param_1 + 4;
}


