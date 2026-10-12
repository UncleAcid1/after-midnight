// Ghidra decompilation of functions from original source file:
//   Sources & Headers/Stats.c

// ---- _CreateStatsArea @ 0x77ba9 (788 bytes)

void _CreateStatsArea(void)

{
  uint uVar1;
  uint uVar2;
  short sVar3;
  undefined4 local_20;
  undefined4 local_1c;
  uint local_18;
  undefined4 local_14;
  int local_10;
  
  *(undefined2 *)PTR__gStatsHeight_001c47cc = 0x30;
  uVar1 = *(uint *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x74);
  local_18 = uVar1 & 0xffff0000;
  local_14 = CONCAT22((short)((uint)*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x78) >>
                             0x10),*(undefined2 *)PTR__gStatsHeight_001c47cc);
  local_20 = local_18;
  uVar2 = local_20;
  local_1c = local_14;
  local_20._2_2_ = (short)(uVar1 >> 0x10);
  sVar3 = -local_20._2_2_;
  local_20 = uVar2;
  _SWOffsetRect(&local_20,(int)sVar3,0);
  local_10 = _SWCreateAdditionalSpriteWorld
                       (PTR__gStatsSpriteWorldP_001c48f4,*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                        &local_20);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Stats.c",
               PTR____FUNCTION___27417_001c4f7c,0x1b);
  }
  local_10 = _SWChangeWorldRect(*(undefined4 *)PTR__gStatsSpriteWorldP_001c48f4,&local_18,1);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Stats.c",
               PTR____FUNCTION___27417_001c4f7c,0x1e);
  }
  local_10 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gStatsBackgroundSpriteLayerP_001c4f80);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Stats.c",
               PTR____FUNCTION___27417_001c4f7c,0x24);
  }
  local_10 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gStatsSpriteLayerP_001c4010);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Stats.c",
               PTR____FUNCTION___27417_001c4f7c,0x26);
  }
  local_10 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gDigitsStatsSpriteLayerP_001c41c0);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Stats.c",
               PTR____FUNCTION___27417_001c4f7c,0x28);
  }
  local_10 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gStatsMapTextSpriteLayerP_001c4f90);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Stats.c",
               PTR____FUNCTION___27417_001c4f7c,0x2a);
  }
  local_10 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gStatsFadeSpriteLayerP_001c49cc);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Stats.c",
               PTR____FUNCTION___27417_001c4f7c,0x2c);
  }
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gStatsSpriteWorldP_001c48f4,
                    *(SpriteLayerPtr *)PTR__gStatsBackgroundSpriteLayerP_001c4f80);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gStatsSpriteWorldP_001c48f4,
                    *(SpriteLayerPtr *)PTR__gStatsSpriteLayerP_001c4010);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gStatsSpriteWorldP_001c48f4,
                    *(SpriteLayerPtr *)PTR__gDigitsStatsSpriteLayerP_001c41c0);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gStatsSpriteWorldP_001c48f4,
                    *(SpriteLayerPtr *)PTR__gStatsMapTextSpriteLayerP_001c4f90);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gStatsSpriteWorldP_001c48f4,
                    *(SpriteLayerPtr *)PTR__gStatsFadeSpriteLayerP_001c49cc);
  return;
}


// ---- _ResetStatsArea @ 0x77ebd (257 bytes)

void _ResetStatsArea(void)

{
  SpritePtr pSVar1;
  
  _RemoveAllStatsItems();
  *(undefined2 *)PTR__gCurNumStatsSlots_001c4f98 = 6;
  _LoadStatsBackFrame(5000);
  _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gDigitsStatsSpriteLayerP_001c41c0);
  pSVar1 = (SpritePtr)_NewStatsDigitsSprite(2,0);
  _SetStatsSpriteDrawProc(pSVar1,*(undefined4 *)PTR__gSpriteDrawProc_001c47ec);
  _SWSetSpriteLocation(pSVar1,37.0,13.0);
  pSVar1 = (SpritePtr)_NewStatsDigitsSprite(6,1);
  _SetStatsSpriteDrawProc(pSVar1,*(undefined4 *)PTR__gSpriteDrawProc_001c47ec);
  _SWSetSpriteLocation(pSVar1,500.0,13.0);
  _SWUpdateWindowSize(*(SpriteWorldRec **)PTR__gStatsSpriteWorldP_001c48f4);
  return;
}


// ---- _RestoreStatsFromObjectsCarried @ 0x77fbe (406 bytes)

void _RestoreStatsFromObjectsCarried(void)

{
  short local_e;
  
  _RemoveAllStatsItems();
  if (PTR__gObjects_001c4910[0xf] == '\0') {
    *(undefined2 *)PTR__gCurNumStatsSlots_001c4f98 = 6;
    _LoadStatsBackFrame(5000);
  }
  else {
    _AddBackpackToStatsArea(0);
  }
  if (PTR__gObjects_001c4910[0xe] != '\0') {
    _AddLanternToStatsArea();
  }
  for (local_e = *(short *)PTR__gObjects_001c4910; 0 < local_e; local_e = local_e + -1) {
    _AddItemToStats(0);
  }
  for (local_e = *(short *)(PTR__gObjects_001c4910 + 2); 0 < local_e; local_e = local_e + -1) {
    _AddItemToStats(1);
  }
  for (local_e = *(short *)(PTR__gObjects_001c4910 + 4); 0 < local_e; local_e = local_e + -1) {
    _AddItemToStats(2);
  }
  for (local_e = *(short *)(PTR__gObjects_001c4910 + 6); 0 < local_e; local_e = local_e + -1) {
    _AddItemToStats(3);
  }
  for (local_e = *(short *)(PTR__gObjects_001c4910 + 8); 0 < local_e; local_e = local_e + -1) {
    _AddItemToStats(4);
  }
  for (local_e = *(short *)(PTR__gObjects_001c4910 + 10); 0 < local_e; local_e = local_e + -1) {
    _AddItemToStats(5);
  }
  for (local_e = *(short *)(PTR__gObjects_001c4910 + 0xc); 0 < local_e; local_e = local_e + -1) {
    _AddItemToStats(6);
  }
  return;
}


// ---- _LoadStatsBackFrame @ 0x78154 (120 bytes)

void _LoadStatsBackFrame(short param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = _GetSpriteImagePath(0);
  iVar2 = _LoadAndInstallStatsBackgroundSprite(0,uVar1,(int)param_1);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Stats.c",
               PTR____FUNCTION___27506_001c4f88,0x95);
  }
  return;
}


// ---- _LoadAndInstallStatsBackgroundSprite @ 0x781cc (266 bytes)

int _LoadAndInstallStatsBackgroundSprite(undefined4 *param_1,undefined4 param_2,short param_3)

{
  char local_1a4 [400];
  SpritePtr local_14;
  int local_10;
  
  local_10 = 0;
  _sprintf(local_1a4,"%s%05d.png",param_2,(int)param_3);
  local_10 = _SWCreateSpriteFromFile(&local_14,0,local_1a4,1,0);
  if (local_10 == 0) {
    _MySetSpriteScaledSize(local_14,0x280,0x30);
    _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gStatsBackgroundSpriteLayerP_001c4f80);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gStatsBackgroundSpriteLayerP_001c4f80,local_14);
    _SWSetSpriteLocation(local_14,0.0,0.0);
    if (param_1 != (undefined4 *)0x0) {
      *param_1 = local_14;
    }
  }
  return local_10;
}


// ---- _AddBackpackToStatsArea @ 0x782d6 (68 bytes)

void _AddBackpackToStatsArea(void)

{
  PTR__gObjects_001c4910[0xf] = 1;
  *(undefined2 *)PTR__gCurNumStatsSlots_001c4f98 = 8;
  _LoadStatsBackFrame(0x1392);
  return;
}


// ---- _LoadAppropriateStatsBackground @ 0x7831a (67 bytes)

void _LoadAppropriateStatsBackground(void)

{
  if (PTR__gObjects_001c4910[0xf] == '\0') {
    _LoadStatsBackFrame(5000);
  }
  else {
    _LoadStatsBackFrame(0x1392);
  }
  return;
}


// ---- _AddLanternToStatsArea @ 0x7835d (83 bytes)

void _AddLanternToStatsArea(void)

{
  SpritePtr srcSpriteP;
  
  srcSpriteP = (SpritePtr)_NewStatsItemSprite(7);
  _SWSetSpriteLocation(srcSpriteP,406.0,9.0);
  return;
}


// ---- _UpdateStatsItems @ 0x783b0 (43 bytes)

void _UpdateStatsItems(void)

{
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gStatsSpriteWorldP_001c48f4);
  return;
}


// ---- _DrawStatsArea @ 0x783db (19 bytes)

void _DrawStatsArea(void)

{
  _UpdateStatsItems();
  return;
}


// ---- _StatsDigitsMoveProc @ 0x783ee (95 bytes)

void StatsDigitsMoveProc(int param_1)

{
  undefined4 uStack_10;
  
  if (*(short *)(param_1 + 0xfc) < 3) {
    uStack_10 = (int)*(short *)PTR__gNumLivesLeft_001c49e4;
  }
  else {
    uStack_10 = *(int *)PTR__gScore_001c48a4;
  }
  _SetStatsSpriteNumber(param_1,uStack_10);
  return;
}


// ---- _AddItemToStats @ 0x7844d (324 bytes)

bool _AddItemToStats(short param_1)

{
  short sVar1;
  short sVar2;
  SpritePtr srcSpriteP;
  
  if ((*(short *)PTR__gCurNumStatsSlots_001c4f98 <= *(short *)PTR__gNumItemsBeingCarried_001c48cc)
     && (0 < *(short *)(PTR__gObjects_001c4910 + 0xc))) {
    _RemoveItemFromStats(6);
    *(short *)(PTR__gObjects_001c4910 + 0xc) = *(short *)(PTR__gObjects_001c4910 + 0xc) + -1;
  }
  sVar1 = *(short *)PTR__gNumItemsBeingCarried_001c48cc;
  sVar2 = *(short *)PTR__gCurNumStatsSlots_001c4f98;
  if (sVar1 < sVar2) {
    *(short *)(PTR__gStatsItemArray_001c4f94 + *(short *)PTR__gNumItemsBeingCarried_001c48cc * 2) =
         param_1;
    srcSpriteP = (SpritePtr)_NewStatsItemSprite((int)param_1);
    *(SpritePtr *)
     (PTR__gStatsItemSpriteArray_001c4f84 + *(short *)PTR__gNumItemsBeingCarried_001c48cc * 4) =
         srcSpriteP;
    _SWSetSpriteLocation
              (srcSpriteP,
               (float)(int)(short)(*(short *)PTR__gNumItemsBeingCarried_001c48cc * 0x24 + 0x68),9.0)
    ;
    *(short *)PTR__gNumItemsBeingCarried_001c48cc =
         *(short *)PTR__gNumItemsBeingCarried_001c48cc + 1;
  }
  return sVar1 < sVar2;
}


// ---- _RemoveItemFromStats @ 0x78591 (440 bytes)

void _RemoveItemFromStats(short param_1)

{
  SpritePtr spriteP;
  short local_10;
  short local_e;
  
  local_10 = 0;
  while( true ) {
    if (*(short *)PTR__gNumItemsBeingCarried_001c48cc <= local_10) {
      return;
    }
    if (*(short *)(PTR__gStatsItemArray_001c4f94 + local_10 * 2) == param_1) break;
    local_10 = local_10 + 1;
  }
  spriteP = *(SpritePtr *)(PTR__gStatsItemSpriteArray_001c4f84 + local_10 * 4);
  *(undefined4 *)(PTR__gStatsItemSpriteArray_001c4f84 + local_10 * 4) = 0;
  if (spriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Stats.c",
               PTR____FUNCTION___27608_001c4f8c,0x14e);
  }
  if (spriteP->userData != (int)param_1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Stats.c",
               PTR____FUNCTION___27608_001c4f8c,0x14f);
  }
  _SWRemoveSpriteFromAnimation(spriteP,'\x01');
  for (local_e = local_10; (int)local_e < *(short *)PTR__gNumItemsBeingCarried_001c48cc + -1;
      local_e = local_e + 1) {
    *(undefined2 *)(PTR__gStatsItemArray_001c4f94 + local_e * 2) =
         *(undefined2 *)(PTR__gStatsItemArray_001c4f94 + (local_e + 1) * 2);
    *(undefined4 *)(PTR__gStatsItemSpriteArray_001c4f84 + local_e * 4) =
         *(undefined4 *)(PTR__gStatsItemSpriteArray_001c4f84 + (local_e + 1) * 4);
    _SWOffsetSprite(*(SpritePtr *)(PTR__gStatsItemSpriteArray_001c4f84 + local_e * 4),-36.0,0.0);
  }
  *(short *)PTR__gNumItemsBeingCarried_001c48cc = *(short *)PTR__gNumItemsBeingCarried_001c48cc + -1
  ;
  return;
}


// ---- _RemoveAllStatsItems @ 0x78749 (56 bytes)

void _RemoveAllStatsItems(void)

{
  _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gStatsSpriteLayerP_001c4010);
  *(undefined2 *)PTR__gNumItemsBeingCarried_001c48cc = 0;
  return;
}


// ---- _HideStatsSprites @ 0x78781 (77 bytes)

void _HideStatsSprites(void)

{
  _SWSetSpritesInLayerVisible(*(undefined4 *)PTR__gStatsSpriteLayerP_001c4010,0);
  _SWSetSpritesInLayerVisible(*(undefined4 *)PTR__gDigitsStatsSpriteLayerP_001c41c0,0);
  return;
}


// ---- _MakeStatsSpritesVisible @ 0x787ce (77 bytes)

void _MakeStatsSpritesVisible(void)

{
  _SWSetSpritesInLayerVisible(*(undefined4 *)PTR__gStatsSpriteLayerP_001c4010,1);
  _SWSetSpritesInLayerVisible(*(undefined4 *)PTR__gDigitsStatsSpriteLayerP_001c41c0,1);
  return;
}


// ---- _CreateFPSAndLevelResIDSprites @ 0x7881b (371 bytes)

void _CreateFPSAndLevelResIDSprites(void)

{
  short sVar1;
  SpritePtr pSVar2;
  longdouble extraout_ST0;
  
  if (*PTR__gDisplayFPSAndLevelResID_001c4960 != '\0') {
    pSVar2 = (SpritePtr)_NewSmallDigitsSprite(3,0);
    _SetStatsSpriteDrawProc(pSVar2,*(undefined4 *)PTR__gSpriteDrawProc_001c47ec);
    _SWSetSpriteLocation(pSVar2,580.0,2.0);
    _SmallDigitsMoveProc(pSVar2);
    pSVar2 = (SpritePtr)_NewSmallDigitsSprite(5,0);
    _SetStatsSpriteDrawProc(pSVar2,*(undefined4 *)PTR__gSpriteDrawProc_001c47ec);
    _SWSetSpriteLocation(pSVar2,0.0,2.0);
    _SmallDigitsMoveProc(pSVar2);
    pSVar2 = (SpritePtr)_NewSmallDigitsSprite(9,0);
    _SetStatsSpriteDrawProc(pSVar2,*(undefined4 *)PTR__gSpriteDrawProc_001c47ec);
    sVar1 = *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x90);
    _SWGetSpriteHeight(pSVar2);
    _SWSetSpriteLocation(pSVar2,460.0,(float)(int)sVar1 - (float)extraout_ST0);
    _SmallDigitsMoveProc(pSVar2);
  }
  return;
}


