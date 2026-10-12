// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTSize.cpp

// ---- __ZN6FTSizeC2Ev @ 0x432ea (96 bytes)

/* FTSize::FTSize() */

void __thiscall FTSize::FTSize(FTSize *this)

{
  *(undefined **)this = PTR_vtable_001c4a78 + 8;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  return;
}


// ---- __ZN6FTSizeC1Ev @ 0x4334a (96 bytes)

/* FTSize::FTSize() */

void __thiscall FTSize::FTSize(FTSize *this)

{
  *(undefined **)this = PTR_vtable_001c4a78 + 8;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  return;
}


// ---- __ZN6FTSizeD2Ev @ 0x433aa (62 bytes)

/* WARNING: Removing unreachable block (ram,0x000433d6) */
/* FTSize::~FTSize() */

void __thiscall FTSize::~FTSize(FTSize *this)

{
  *(undefined **)this = PTR_vtable_001c4a78 + 8;
  return;
}


// ---- __ZN6FTSizeD1Ev @ 0x433e8 (62 bytes)

/* WARNING: Removing unreachable block (ram,0x00043414) */
/* FTSize::~FTSize() */

void __thiscall FTSize::~FTSize(FTSize *this)

{
  *(undefined **)this = PTR_vtable_001c4a78 + 8;
  return;
}


// ---- __ZN6FTSizeD0Ev @ 0x43426 (62 bytes)

/* FTSize::~FTSize() */

void __thiscall FTSize::~FTSize(FTSize *this)

{
  *(undefined **)this = PTR_vtable_001c4a78 + 8;
  operator_delete(this);
  return;
}


// ---- __ZN6FTSize8CharSizeEPP11FT_FaceRec_jjj @ 0x43464 (246 bytes)

/* FTSize::CharSize(FT_FaceRec_**, unsigned int, unsigned int, unsigned int) */

bool __thiscall
FTSize::CharSize(FTSize *this,FT_FaceRec_ **param_1,uint param_2,uint param_3,uint param_4)

{
  undefined4 uVar1;
  
  if (((*(uint *)(this + 0xc) != param_2) || (*(uint *)(this + 0x10) != param_3)) ||
     (*(uint *)(this + 0x14) != param_4)) {
    uVar1 = _FT_Set_Char_Size(*param_1,0,param_2 << 6,*(undefined4 *)(this + 0x10),
                              *(undefined4 *)(this + 0x14));
    *(undefined4 *)(this + 0x18) = uVar1;
    if (*(int *)(this + 0x18) == 0) {
      *(FT_FaceRec_ ***)(this + 4) = param_1;
      *(uint *)(this + 0xc) = param_2;
      *(uint *)(this + 0x10) = param_3;
      *(uint *)(this + 0x14) = param_4;
      *(undefined4 *)(this + 8) = *(undefined4 *)(**(int **)(this + 4) + 0x58);
    }
    else {
      *(undefined4 *)(this + 4) = 0;
      *(undefined4 *)(this + 0xc) = 0;
      *(undefined4 *)(this + 0x10) = 0;
      *(undefined4 *)(this + 0x14) = 0;
      *(undefined4 *)(this + 8) = 0;
    }
  }
  return *(int *)(this + 0x18) == 0;
}


// ---- __ZNK6FTSize8CharSizeEv @ 0x4355a (20 bytes)

/* FTSize::CharSize() const */

undefined4 __thiscall FTSize::CharSize(FTSize *this)

{
  return *(undefined4 *)(this + 0xc);
}


// ---- __ZNK6FTSize8AscenderEv @ 0x4356e (102 bytes)

/* FTSize::Ascender() const */

longdouble __thiscall FTSize::Ascender(FTSize *this)

{
  float local_10;
  
  if (*(int *)(this + 8) == 0) {
    local_10 = 0.0;
  }
  else {
    local_10 = (float)*(int *)(*(int *)(this + 8) + 0x18) / 64.0;
  }
  return (longdouble)local_10;
}


// ---- __ZNK6FTSize9DescenderEv @ 0x435d4 (102 bytes)

/* FTSize::Descender() const */

longdouble __thiscall FTSize::Descender(FTSize *this)

{
  float local_10;
  
  if (*(int *)(this + 8) == 0) {
    local_10 = 0.0;
  }
  else {
    local_10 = (float)*(int *)(*(int *)(this + 8) + 0x1c) / 64.0;
  }
  return (longdouble)local_10;
}


// ---- __ZNK6FTSize6HeightEv @ 0x4363a (268 bytes)

/* FTSize::Height() const */

longdouble __thiscall FTSize::Height(FTSize *this)

{
  float local_10;
  
  if (*(int *)(this + 8) == 0) {
    local_10 = 0.0;
  }
  else if ((*(uint *)(**(int **)(this + 4) + 8) & 1) == 0) {
    local_10 = (float)*(int *)(*(int *)(this + 8) + 0x20) / 64.0;
  }
  else {
    local_10 = (float)(*(int *)(**(int **)(this + 4) + 0x40) - *(int *)(**(int **)(this + 4) + 0x38)
                      ) *
               (((float)*(ushort *)(*(int *)(this + 8) + 0xe) + 0.0) /
               ((float)*(ushort *)(**(int **)(this + 4) + 0x44) + 0.0));
  }
  return (longdouble)local_10;
}


// ---- __ZNK6FTSize5WidthEv @ 0x43746 (268 bytes)

/* FTSize::Width() const */

longdouble __thiscall FTSize::Width(FTSize *this)

{
  float local_10;
  
  if (*(int *)(this + 8) == 0) {
    local_10 = 0.0;
  }
  else if ((*(uint *)(**(int **)(this + 4) + 8) & 1) == 0) {
    local_10 = (float)*(int *)(*(int *)(this + 8) + 0x24) / 64.0;
  }
  else {
    local_10 = (float)(*(int *)(**(int **)(this + 4) + 0x3c) - *(int *)(**(int **)(this + 4) + 0x34)
                      ) *
               (((float)*(ushort *)(*(int *)(this + 8) + 0xc) + 0.0) /
               ((float)*(ushort *)(**(int **)(this + 4) + 0x44) + 0.0));
  }
  return (longdouble)local_10;
}


// ---- __ZNK6FTSize9UnderlineEv @ 0x43852 (44 bytes)

/* FTSize::Underline() const */

longdouble FTSize::Underline(void)

{
  return (longdouble)0.0;
}


