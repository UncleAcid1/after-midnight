// Ghidra decompilation of functions from original source file:
//   ../SpriteWorldX/Utils/SWStats.c

// ---- _CreateStatsSpriteClone @ 0x2b1df (343 bytes)

SpritePtr _CreateStatsSpriteClone
                    (int param_1,undefined2 param_2,undefined4 param_3,undefined4 param_4)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_10 = 0;
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWStats.c",
               PTR____FUNCTION___20934_001c46e0,0x1b);
  }
  if (*(int *)(param_1 + 0x84) < 10) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWStats.c",
               PTR____FUNCTION___20934_001c46e0,0x1c);
  }
  local_14 = _calloc(1,0x10c);
  if (local_14 == (void *)0x0) {
    local_10 = 200;
  }
  if (local_10 == 0) {
    local_10 = _SWCloneSprite(param_1,&local_18,local_14,0x10c);
  }
  if (local_10 == 0) {
    local_18->frameDrawProc = (DrawProcPtr_t)PTR_StatsItemDrawProc_001c46dc;
    _SWSetCurrentFrameIndex(local_18,local_18->curFrameIndex);
    *(undefined2 *)((int)local_14 + 0xfc) = param_2;
    *(undefined4 *)((int)local_14 + 0xf8) = 0xffffffff;
    *(undefined4 *)((int)local_14 + 0x100) = param_3;
    *(undefined4 *)((int)local_14 + 0x104) = param_4;
    *(undefined **)((int)local_14 + 0x108) = PTR__SWStdSpriteDrawProc_001c45dc;
  }
  else {
    local_18 = (SpritePtr)0x0;
  }
  return local_18;
}


// ---- _SetUpStatsSprite @ 0x2b336 (150 bytes)

int SetUpStatsSprite(SpritePtr param_1,SpriteLayerPtr param_2,undefined4 param_3,short param_4,
                    short param_5,undefined4 param_6)

{
  undefined4 uStack_10;
  
  uStack_10 = 0;
  if (param_1 == (SpritePtr)0x0) {
    uStack_10 = -1;
  }
  if (uStack_10 == 0) {
    _SWSetSpriteLocation(param_1,(float)(int)param_4,(float)(int)param_5);
    _SWAddSprite(param_2,param_1);
    _SetStatsSpriteDrawProc(param_1,param_3);
    _SetStatsSpriteNumber(param_1,param_6);
  }
  return uStack_10;
}


// ---- _SetStatsSpriteDrawProc @ 0x2b3cc (32 bytes)

void _SetStatsSpriteDrawProc(int param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 0x108) = param_2;
  return;
}


// ---- _SetStatsSpriteNumber @ 0x2b3ec (53 bytes)

void _SetStatsSpriteNumber(int param_1,int param_2)

{
  if (*(int *)(param_1 + 0xf8) != param_2) {
    *(int *)(param_1 + 0xf8) = param_2;
    *(undefined1 *)(param_1 + 0x33) = 1;
  }
  return;
}


// ---- _GetStatsSpriteNumber @ 0x2b421 (23 bytes)

undefined4 GetStatsSpriteNumber(int param_1)

{
  return *(undefined4 *)(param_1 + 0xf8);
}


// ---- _StatsItemDrawProc @ 0x2b438 (698 bytes)

void StatsItemDrawProc(int param_1)

{
  int iVar1;
  char acStackY_8040 [32676];
  char cStack_70;
  short sStack_56;
  short sStack_52;
  undefined4 uStack_48;
  char acStack_40 [10];
  short sStack_36;
  short sStack_34;
  short sStack_32;
  short sStack_30;
  undefined2 uStack_2e;
  int iStack_2c;
  int iStack_28;
  int *piStack_24;
  int iStack_20;
  
  iStack_20 = *(int *)(param_1 + 0xe0);
  iStack_28 = param_1;
  piStack_24 = *(int **)(param_1 + 0x78);
  iStack_2c = *(int *)(param_1 + 0xf8);
  if (*(int *)(param_1 + 0x104) == 0) {
    uStack_2e = 0xffff;
  }
  else {
    uStack_2e = 0;
  }
  for (sStack_36 = 0; sStack_36 < *(short *)(iStack_28 + 0xfc); sStack_36 = sStack_36 + 1) {
    acStack_40[sStack_36] = (char)uStack_2e;
  }
  if (-1 < iStack_2c) {
    if (*(int *)(iStack_28 + 0x100) == 1) {
      sStack_30 = _GetNumberLength(iStack_2c);
      sStack_30 = sStack_30 + -1;
    }
    else {
      sStack_30 = *(short *)(iStack_28 + 0xfc) + -1;
    }
    do {
      cStack_70 = (char)iStack_2c + (char)(iStack_2c / 10) * -10;
      acStack_40[sStack_30] = cStack_70;
      sStack_30 = sStack_30 + -1;
      iStack_2c = iStack_2c / 10;
      if (iStack_2c < 1) break;
    } while (-1 < sStack_30);
  }
  uStack_48 = *(undefined4 *)(param_1 + 0x36);
  sStack_52 = (short)((uint)*(undefined4 *)(*piStack_24 + 0x1c) >> 0x10);
  sStack_56 = (short)((uint)*(undefined4 *)(*piStack_24 + 0x18) >> 0x10);
  sStack_32 = (short)(int)(*(float *)(iStack_20 + 0x2c) * (float)((int)sStack_52 - (int)sStack_56));
  for (sStack_30 = 0; sStack_30 < *(short *)(iStack_28 + 0xfc); sStack_30 = sStack_30 + 1) {
    sStack_34 = (short)acStack_40[sStack_30];
    if (-1 < sStack_34) {
      iVar1 = piStack_24[sStack_34];
      *(float *)(iStack_20 + 4) = (float)(int)uStack_48._2_2_;
      *(float *)(iStack_20 + 8) = (float)(int)(short)uStack_48;
      if (*(int *)PTR__gSWCurrentSpriteWorld_001c439c != 0) {
        uStack_48 = CONCAT22(uStack_48._2_2_ +
                             *(short *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x8e),
                             (short)uStack_48 +
                             *(short *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x8c));
      }
      (**(code **)(iStack_28 + 0x108))
                (param_1,iVar1,(float)(int)uStack_48._2_2_,(float)(int)(short)uStack_48);
    }
    uStack_48 = CONCAT22(uStack_48._2_2_ + sStack_32,(short)uStack_48);
  }
  return;
}


// ---- _GetNumberLength @ 0x2b6f2 (183 bytes)

undefined2 _GetNumberLength(int param_1)

{
  undefined2 local_e;
  
  if (param_1 < 0) {
    param_1 = -param_1;
  }
  if (param_1 < 10) {
    local_e = 1;
  }
  else if (param_1 < 100) {
    local_e = 2;
  }
  else if (param_1 < 1000) {
    local_e = 3;
  }
  else if (param_1 < 10000) {
    local_e = 4;
  }
  else if (param_1 < 100000) {
    local_e = 5;
  }
  else if (param_1 < 1000000) {
    local_e = 6;
  }
  else if (param_1 < 10000000) {
    local_e = 7;
  }
  else if (param_1 < 100000000) {
    local_e = 8;
  }
  else if (param_1 < 1000000000) {
    local_e = 9;
  }
  else {
    local_e = 10;
  }
  return local_e;
}


