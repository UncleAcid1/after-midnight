// Ghidra decompilation of functions from original source file:
//   Sources & Headers/Levers.c

// ---- _PullLeverIfPossible @ 0x6f551 (1157 bytes)

undefined1 _PullLeverIfPossible(SpritePtr param_1)

{
  short sVar1;
  int iVar2;
  bool bVar3;
  SWMovementType SVar4;
  SpritePtr local_1c;
  undefined1 local_15;
  
  local_15 = 0;
  for (local_1c = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gLeverSpriteLayerP_001c41d4,
                                   (SpritePtr)0x0); local_1c != (SpritePtr)0x0;
      local_1c = local_1c->nextSpriteP) {
    sVar1 = _GetDistBetweenSpritesHorizMid(param_1,local_1c);
    if (sVar1 < 0x1e) {
      sVar1 = _GetDistBetweenSpritesVertMid(param_1,local_1c);
      if (sVar1 < 0x10) {
        iVar2 = _mAbs((int)(param_1->destFrameRect).bottom - (int)(local_1c->destFrameRect).bottom);
        if (iVar2 < 7) break;
      }
    }
  }
  if (local_1c != (SpritePtr)0x0) {
    if (((local_1c->userData < 0xe) || (0x11 < local_1c->userData)) &&
       ((local_1c->userData < 0x12 || (0x19 < local_1c->userData)))) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Levers.c",
                 PTR____FUNCTION___27422_001c4ee8,0x2c);
    }
    bVar3 = local_1c->curFrameIndex != 0;
    SVar4 = _SWGetSpriteHorizLoc(param_1);
    param_1->f_1b2 = (char)(short)(int)SVar4;
    param_1->f_1b3 = (char)((ushort)(short)(int)SVar4 >> 8);
    _SWMoveSprite(param_1,(float)(int)(local_1c->destFrameRect).left,
                  (float)(int)(param_1->destFrameRect).top);
    if (bVar3) {
      _SWOffsetSprite(param_1,-2.0,0.0);
      _SWSetSpriteFrameAdvance(local_1c,-1);
      param_1->f_177 = '\0';
      if (param_1->userData == 0) {
        _SWSetSpriteFrameRange(param_1,0x22,0x25);
        _SWSetSpriteFlipX(param_1,'\x01');
        _SWSetCurrentFrameIndex(param_1,0x22);
      }
      else {
        _SWSetSpriteFrameRange(param_1,10,0xd);
        _SWSetCurrentFrameIndex(param_1,10);
      }
    }
    else {
      _SWOffsetSprite(param_1,2.0,0.0);
      _SWSetSpriteFrameAdvance(local_1c,1);
      param_1->f_177 = '\x01';
      if (param_1->userData == 0) {
        _SWSetSpriteFrameRange(param_1,0x22,0x25);
        _SWSetSpriteFlipX(param_1,'\0');
        _SWSetCurrentFrameIndex(param_1,0x22);
      }
      else {
        _SWSetSpriteFrameRange(param_1,0xe,0x11);
        _SWSetCurrentFrameIndex(param_1,0xe);
      }
    }
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)0x0);
    _SWSetSpriteFrameAdvance(param_1,0);
    _SWSetSpriteFrameTime(param_1,0);
    _SWResetSpriteFrameTimer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1);
    if (local_1c->f_104 == '\0') {
      _MMPlaySound(0x18,9,1);
      param_1->f_190 = '\x14';
      param_1->f_191 = '\0';
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_LAB_001c4f08);
      if (bVar3) {
        _SWSetCurrentFrameIndex(param_1,0x23);
        _SWSetSpriteFlipX(param_1,'\x01');
        _SWOffsetSprite(param_1,-10.0,0.0);
      }
      else {
        _SWSetCurrentFrameIndex(param_1,0x23);
        _SWSetSpriteFlipX(param_1,'\0');
        _SWOffsetSprite(param_1,10.0,0.0);
      }
    }
    else {
      param_1->f_190 = '\f';
      param_1->f_191 = '\0';
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_PullLeverFrameProc_001c4f0c);
    }
    *(SpritePtr *)&param_1->f_1c8 = local_1c;
    PTR__gKeys_001c4af8[7] = 0;
    local_15 = 1;
  }
  return local_15;
}


// ---- _PullLeverFrameProc @ 0x6f9d6 (707 bytes)

void PullLeverFrameProc(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  short sVar1;
  SpritePtr srcSpriteP;
  uchar uVar2;
  bool bVar3;
  SWMovementType SVar4;
  
  sVar1 = *(short *)&param_1->f_190;
  *(short *)&param_1->f_190 = sVar1 + -1;
  if (sVar1 < 1) {
    param_1->f_190 = '\f';
    param_1->f_191 = '\0';
    srcSpriteP = *(SpritePtr *)&param_1->f_1c8;
    if (param_1->userData == 0) {
      bVar3 = *param_3 == 0x25;
    }
    else if ((*param_3 == 0x11) || (*param_3 == 0xd)) {
      bVar3 = true;
    }
    else {
      bVar3 = false;
    }
    if (bVar3) {
      _FinishPullingLever(param_1,1);
      if (param_1->drawData->flippedHorizontal == '\0') {
        _SWOffsetSprite(param_1,-13.0,0.0);
      }
      else {
        _SWOffsetSprite(param_1,13.0,0.0);
      }
      sVar1._0_1_ = param_1->f_1b2;
      sVar1._1_1_ = param_1->f_1b3;
      SVar4 = _SWGetSpriteHorizLoc(param_1);
      if (0.0 < (float)(int)sVar1 - SVar4) {
        _CheckForWall(param_1,4,&param_1->f_142,param_1->f_170,0,1);
      }
      else {
        _CheckForWall(param_1,2,&param_1->f_142,param_1->f_170,0,1);
      }
      _PerformLeverAction(srcSpriteP,0);
      if (*PTR__gCanUseLadderJump_001c4828 == '\0') {
        uVar2 = _MoveSpriteDownToFloor(param_1);
        param_1->f_17e = uVar2;
        _SaveLastOnGroundPixelRow(param_1);
        if (param_1->f_17e == '\0') {
          PTR__gKeys_001c4af8[4] = 0;
        }
      }
    }
    else {
      _SWSetCurrentFrameIndex(srcSpriteP,srcSpriteP->curFrameIndex + srcSpriteP->frameAdvance);
      _MMPlaySound(0x1b,0xc,1);
    }
    if (param_1->drawData->flippedHorizontal != '\0') {
      _SWOffsetSprite(param_1,-2.0,0.0);
    }
    *param_3 = *param_3 + 1;
  }
  return;
}


// ---- _TryToPullLeverFrameProc @ 0x6fc99 (184 bytes)

void TryToPullLeverFrameProc(SpritePtr param_1)

{
  short sVar1;
  
  sVar1 = *(short *)&param_1->f_190;
  *(short *)&param_1->f_190 = sVar1 + -1;
  if (sVar1 < 1) {
    param_1->f_190 = '\x14';
    param_1->f_191 = '\0';
    _FinishPullingLever(param_1,0);
    if (param_1->drawData->flippedHorizontal == '\0') {
      _SWOffsetSprite(param_1,-13.0,0.0);
    }
    else {
      _SWOffsetSprite(param_1,13.0,0.0);
    }
  }
  return;
}


// ---- _FinishPullingLever @ 0x6fd51 (252 bytes)

void _FinishPullingLever(SpritePtr param_1,char param_2)

{
  int iVar1;
  
  iVar1 = *(int *)&param_1->f_1c8;
  if (param_1->userData == 0) {
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4098);
    _SWSetSpriteFrameAdvance(param_1,1);
    _SWSetSpriteFrameTime(param_1,-1);
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
  }
  if (param_2 != '\0') {
    if (*(char *)(iVar1 + 0x105) != '\0') {
      *(undefined1 *)(iVar1 + 0x104) = 0;
    }
    if (*(char *)(iVar1 + 0x106) == '\0') {
      if (*(int *)(iVar1 + 0x94) == 0) {
        _RemoveSpriteFromObjectArray(iVar1);
      }
      else {
        _AddSpriteToObjectArray(iVar1);
      }
    }
  }
  param_1->f_1c8 = '\0';
  param_1->f_1c9 = '\0';
  param_1->f_1ca = '\0';
  param_1->f_1cb = '\0';
  return;
}


// ---- _FindSpriteMatchingObjectID @ 0x6fe4d (188 bytes)

SpritePtr _FindSpriteMatchingObjectID(int param_1)

{
  int iVar1;
  char cVar2;
  SpriteLayerPtr local_14;
  SpritePtr local_10;
  
  local_14 = _SWGetNextSpriteLayer
                       (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,(SpriteLayerPtr)0x0);
  do {
    if (local_14 == (SpriteLayerPtr)0x0) {
      return (SpritePtr)0x0;
    }
    for (local_10 = _SWGetNextSprite(local_14,(SpritePtr)0x0); local_10 != (SpritePtr)0x0;
        local_10 = _SWGetNextSprite(local_14,local_10)) {
      iVar1._0_1_ = local_10->f_0f8;
      iVar1._1_1_ = local_10->f_0f9;
      iVar1._2_1_ = local_10->f_0fa;
      iVar1._3_1_ = local_10->f_0fb;
      if ((iVar1 == param_1) && (cVar2 = _CanLeverAffectSprite(local_10), cVar2 != '\0')) {
        return local_10;
      }
    }
    local_14 = _SWGetNextSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_14);
  } while( true );
}


// ---- _SetLeversToCorrectState @ 0x6ff09 (505 bytes)

void _SetLeversToCorrectState(SpriteLayerPtr param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  undefined4 uVar5;
  char cVar6;
  SpritePtr newSpriteP;
  SWMovementType SVar7;
  SWMovementType SVar8;
  SpritePtr local_18;
  
  for (local_18 = _SWGetNextSprite(param_1,(SpritePtr)0x0); local_18 != (SpritePtr)0x0;
      local_18 = _SWGetNextSprite(param_1,local_18)) {
    uVar5._0_1_ = local_18->f_0f8;
    uVar5._1_1_ = local_18->f_0f9;
    uVar5._2_1_ = local_18->f_0fa;
    uVar5._3_1_ = local_18->f_0fb;
    cVar6 = _IsObjectInArray(uVar5);
    if (cVar6 == '\0') {
      if ((((local_18->f_107 != '\0') &&
           (sVar1._0_1_ = local_18->f_0fc, sVar1._1_1_ = local_18->f_0fd, sVar1 != 7)) &&
          (sVar2._0_1_ = local_18->f_0fc, sVar2._1_1_ = local_18->f_0fd, sVar2 != 8)) &&
         ((sVar3._0_1_ = local_18->f_0fc, sVar3._1_1_ = local_18->f_0fd, sVar3 != 9 &&
          (sVar4._0_1_ = local_18->f_0fc, sVar4._1_1_ = local_18->f_0fd, sVar4 != 10)))) {
        _PerformLeverAction(local_18,1);
      }
    }
    else {
      if ((local_18->userData < 0xe) || (0x11 < local_18->userData)) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Levers.c",
                   PTR____FUNCTION___27608_001c4ef4,0x167);
      }
      _SetLeverToPulledState(local_18);
    }
    if (local_18->f_106 == '\0') {
      newSpriteP = (SpritePtr)_NewMiscSprite(0xcd);
      if (newSpriteP != (SpritePtr)0x0) {
        _SWAddSprite(*(SpriteLayerPtr *)PTR__gForegroundSpriteLayerP_001c41f0,newSpriteP);
        SVar7 = _SWGetSpriteVertLoc(local_18);
        SVar8 = _SWGetSpriteHorizLoc(local_18);
        _SWSetSpriteLocation(newSpriteP,(SVar8 + 17.0) - 1.0,(SVar7 + 50.0) - 1.0);
        *(SpritePtr *)&local_18->f_114 = newSpriteP;
      }
    }
  }
  return;
}


// ---- _SetLeverToPulledState @ 0x70102 (112 bytes)

void _SetLeverToPulledState(SpritePtr param_1)

{
  short sVar1;
  
  _SWSetCurrentFrameIndex(param_1,3);
  if (param_1->f_105 != '\0') {
    param_1->f_104 = '\0';
  }
  sVar1._0_1_ = param_1->f_0fc;
  sVar1._1_1_ = param_1->f_0fd;
  if ((sVar1 != 7) && (param_1->f_107 == '\0')) {
    _PerformLeverAction(param_1,1);
  }
  return;
}


// ---- _CheckForPressedBrickSwitches @ 0x70172 (101 bytes)

void _CheckForPressedBrickSwitches(void)

{
  undefined4 local_10;
  
  for (local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gBrickSwitchSpriteLayerP_001c4188,
                                   (SpritePtr)0x0); local_10 != (SpritePtr)0x0;
      local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gBrickSwitchSpriteLayerP_001c4188,local_10
                                 )) {
    _CheckBrickSwitchWithSprites(local_10);
  }
  return;
}


// ---- _CheckBrickSwitchWithSprites @ 0x701d7 (377 bytes)

void _CheckBrickSwitchWithSprites(SpritePtr param_1)

{
  char cVar1;
  short sVar2;
  int iVar3;
  SpritePtr local_14;
  char local_d;
  
  if (param_1->userData < 0x12) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Levers.c",
               PTR____FUNCTION___27684_001c4efc,0x1cd);
  }
  if (0x19 < param_1->userData) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Levers.c",
               PTR____FUNCTION___27684_001c4efc,0x1ce);
  }
  sVar2 = (short)param_1->curFrameIndex;
  local_d = '\0';
  local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gSpyVsSpySpriteLayerP_001c4874,(SpritePtr)0x0)
  ;
  do {
    if (local_14 == (SpritePtr)0x0) {
LAB_000702c6:
      if ((local_d == '\0') && (iVar3 = _GetHeroSprite(), iVar3 != 0)) {
        local_d = _IsSpriteOnSwitch(iVar3,param_1);
      }
      if (local_d == '\0') {
        if ((sVar2 != 0) && (param_1->f_104 != '\0')) {
          _SWSetCurrentFrameIndex(param_1,0);
        }
      }
      else {
        _SWSetCurrentFrameIndex(param_1,1);
        if (sVar2 == 0) {
          _PushBrickSwitch(param_1);
        }
      }
      return;
    }
    cVar1 = _IsSpriteOnSwitch(local_14,param_1);
    if (cVar1 != '\0') {
      local_d = '\x01';
      goto LAB_000702c6;
    }
    local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gSpyVsSpySpriteLayerP_001c4874,local_14);
  } while( true );
}


// ---- _IsSpriteOnSwitch @ 0x70350 (167 bytes)

undefined4 _IsSpriteOnSwitch(int param_1,SpritePtr param_2)

{
  SWBoolean SVar1;
  SWRect local_18;
  int local_10;
  
  local_10 = param_1;
  if (*(char *)(param_1 + 0x17e) != '\0') {
    local_18.left = *(short *)(param_1 + 0x38) + *(short *)(*(int *)(param_1 + 0x7c) + 0x2c);
    local_18.right = *(short *)(param_1 + 0x3c) - *(short *)(*(int *)(param_1 + 0x7c) + 0x30);
    local_18.top = *(short *)(param_1 + 0x3a) - *(short *)(*(int *)(param_1 + 0x7c) + 0x2e);
    local_18.bottom = local_18.top + 6;
    local_18.top = local_18.top + 5;
    SVar1 = _SWIsSpriteInRect(param_2,&local_18);
    if (SVar1 != '\0') {
      return 1;
    }
  }
  return 0;
}


// ---- _PushBrickSwitch @ 0x703f7 (81 bytes)

void _PushBrickSwitch(undefined4 param_1)

{
  char cVar1;
  
  cVar1 = _PerformBrickSwitchAction(param_1,0);
  if (cVar1 == '\0') {
    _MMPlaySound(0x1c,0xc,1);
  }
  _FinishPushingBrickSwitch(param_1);
  return;
}


// ---- _FinishPushingBrickSwitch @ 0x70448 (136 bytes)

void _FinishPushingBrickSwitch(SpritePtr param_1)

{
  short sVar1;
  
  if ((param_1->f_105 != '\0') ||
     (sVar1._0_1_ = param_1->f_0fc, sVar1._1_1_ = param_1->f_0fd, sVar1 == 0)) {
    param_1->f_104 = '\0';
    _SWSetCurrentFrameIndex(param_1,1);
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
  }
  if ((param_1->f_106 == '\0') && (param_1->f_104 == '\0')) {
    _AddSpriteToObjectArray(param_1);
  }
  return;
}


// ---- _SetBrickSwitchesToCorrectState @ 0x704d0 (305 bytes)

void _SetBrickSwitchesToCorrectState(SpriteLayerPtr param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  undefined4 uVar6;
  char cVar7;
  SpritePtr local_14;
  
  for (local_14 = _SWGetNextSprite(param_1,(SpritePtr)0x0); local_14 != (SpritePtr)0x0;
      local_14 = _SWGetNextSprite(param_1,local_14)) {
    uVar6._0_1_ = local_14->f_0f8;
    uVar6._1_1_ = local_14->f_0f9;
    uVar6._2_1_ = local_14->f_0fa;
    uVar6._3_1_ = local_14->f_0fb;
    cVar7 = _IsObjectInArray(uVar6);
    if (cVar7 == '\0') {
      if (((((local_14->f_107 != '\0') &&
            (sVar1._0_1_ = local_14->f_0fc, sVar1._1_1_ = local_14->f_0fd, sVar1 != 0xb)) &&
           (sVar2._0_1_ = local_14->f_0fc, sVar2._1_1_ = local_14->f_0fd, sVar2 != 0xc)) &&
          ((sVar3._0_1_ = local_14->f_0fc, sVar3._1_1_ = local_14->f_0fd, sVar3 != 0xd &&
           (sVar4._0_1_ = local_14->f_0fc, sVar4._1_1_ = local_14->f_0fd, sVar4 != 0xe)))) &&
         (sVar5._0_1_ = local_14->f_0fc, sVar5._1_1_ = local_14->f_0fd, sVar5 != 0xf)) {
        _PerformBrickSwitchAction(local_14,1);
      }
    }
    else {
      if ((local_14->userData < 0x12) || (0x19 < local_14->userData)) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Levers.c",
                   PTR____FUNCTION___27779_001c4f10,0x25e);
      }
      _SetBrickSwitchToPulledState(local_14);
    }
  }
  return;
}


// ---- _SetBrickSwitchToPulledState @ 0x70601 (147 bytes)

void _SetBrickSwitchToPulledState(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  
  _SWSetCurrentFrameIndex(param_1,1);
  _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
  if (param_1->f_105 != '\0') {
    param_1->f_104 = '\0';
  }
  sVar1._0_1_ = param_1->f_0fc;
  sVar1._1_1_ = param_1->f_0fd;
  if (((sVar1 != 0xb) && (sVar2._0_1_ = param_1->f_0fc, sVar2._1_1_ = param_1->f_0fd, sVar2 != 0xc))
     && (param_1->f_107 == '\0')) {
    _PerformBrickSwitchAction(param_1,1);
  }
  return;
}


// ---- _PerformLeverAction @ 0x70694 (787 bytes)

void _PerformLeverAction(int param_1,char param_2)

{
  SpriteLayerPtr local_1c;
  SpriteLayerPtr local_18;
  int local_14;
  int local_10;
  
  local_14 = param_1;
  if (*(short *)(param_1 + 0xfc) == 0) {
    local_10 = _FindSpriteMatchingObjectID(*(undefined4 *)(param_1 + 0x100));
    if (local_10 != 0) {
      _PerformActionOnSprite(local_10,param_1,param_2,0);
    }
  }
  else if (*(short *)(param_1 + 0xfc) == 1) {
    local_10 = _FindSpriteMatchingObjectID(*(undefined4 *)(param_1 + 0x100));
    if (local_10 != 0) {
      _DoLeverChangeOneMovingPlatform(local_10,param_2);
    }
  }
  else if (*(short *)(param_1 + 0xfc) == 2) {
    _DoLeverChangeAllMovingPlatforms(param_2);
  }
  else if (*(short *)(param_1 + 0xfc) == 3) {
    _DoLeverChangeConveyorBeltDir(param_2,0);
  }
  else if (*(short *)(param_1 + 0xfc) == 5) {
    _DoLeverStartOrStopConveyorBelts(param_2,0);
  }
  else if (*(short *)(param_1 + 0xfc) == 4) {
    _DoLeverStartStopAllMovingPlatforms(param_2,0);
  }
  else if (*(short *)(param_1 + 0xfc) == 6) {
    _DoLeverStartStopAllZapperBeams(param_2,0);
  }
  else if (*(short *)(param_1 + 0xfc) == 7) {
    _DoLeverToggleGoldSilverBlocks(param_2,0);
  }
  else if (*(short *)(param_1 + 0xfc) == 8) {
    _DoLeverOpenAllTrapDoors(param_2);
    *(undefined1 *)(local_14 + 0x104) = 0;
  }
  else if (*(short *)(param_1 + 0xfc) == 9) {
    _DoLeverOpenAllGrayDoors(param_1,param_2);
    *(undefined1 *)(local_14 + 0x104) = 0;
  }
  else if (*(short *)(param_1 + 0xfc) == 10) {
    if (param_2 == '\0') {
      _SWCreateSpriteLayer(&local_18);
      _SWCreateSpriteLayer(&local_1c);
      _MoveSpritesFromLayerToLayer(*(undefined4 *)PTR__gOpenedDoorSpriteLayerP_001c49a4,local_18);
      _DoLeverOpenAllGrayDoors(local_14,0);
      _MoveSpritesFromLayerToLayer(*(undefined4 *)PTR__gOpenedDoorSpriteLayerP_001c49a4,local_1c);
      _MoveSpritesFromLayerToLayer(local_18,*(undefined4 *)PTR__gOpenedDoorSpriteLayerP_001c49a4);
      _DoLeverCloseAllGrayDoors(local_14,0);
      _MoveSpritesFromLayerToLayer(local_1c,*(undefined4 *)PTR__gOpenedDoorSpriteLayerP_001c49a4);
      _SWCustomSortSpriteLayer
                (*(SpriteLayerPtr *)PTR__gOpenedDoorSpriteLayerP_001c49a4,
                 (SpriteCompareProcPtr_t)PTR_LAB_001c4ef0);
      _SWDisposeSpriteLayer(&local_18);
      _SWDisposeSpriteLayer(&local_1c);
    }
  }
  else if (*(short *)(param_1 + 0xfc) == 0xb) {
    _DoLeverToggleAllTorches();
  }
  return;
}


// ---- _PerformBrickSwitchAction @ 0x709a7 (946 bytes)

undefined1 _PerformBrickSwitchAction(int param_1,undefined1 param_2)

{
  int iVar1;
  undefined1 local_11;
  
  local_11 = 0;
  if (*(short *)(param_1 + 0xfc) == 0) {
    iVar1 = _FindSpriteMatchingObjectID(*(undefined4 *)(param_1 + 0x100));
    if (iVar1 != 0) {
      local_11 = _PerformActionOnSprite(iVar1,param_1,param_2,5);
    }
  }
  else if (*(short *)(param_1 + 0xfc) == 1) {
    iVar1 = _FindSpriteMatchingObjectID(*(undefined4 *)(param_1 + 0x100));
    if (iVar1 != 0) {
      local_11 = _PerformActionOnSprite(iVar1,param_1,param_2,3);
    }
  }
  else if (*(short *)(param_1 + 0xfc) == 2) {
    iVar1 = _FindSpriteMatchingObjectID(*(undefined4 *)(param_1 + 0x100));
    if (iVar1 != 0) {
      local_11 = _PerformActionOnSprite(iVar1,param_1,param_2,4);
    }
  }
  else if (*(short *)(param_1 + 0xfc) == 3) {
    _DoLeverChangeConveyorBeltDir(param_2,1);
  }
  else if (*(short *)(param_1 + 0xfc) == 4) {
    _DoLeverChangeConveyorBeltDir(param_2,2);
  }
  else if (*(short *)(param_1 + 0xfc) == 7) {
    _DoLeverStartOrStopConveyorBelts(param_2,3);
  }
  else if (*(short *)(param_1 + 0xfc) == 8) {
    _DoLeverStartOrStopConveyorBelts(param_2,4);
  }
  else if (*(short *)(param_1 + 0xfc) == 5) {
    _DoLeverStartStopAllMovingPlatforms(param_2,3);
  }
  else if (*(short *)(param_1 + 0xfc) == 6) {
    _DoLeverStartStopAllMovingPlatforms(param_2,4);
  }
  else if (*(short *)(param_1 + 0xfc) == 9) {
    local_11 = _DoLeverStartStopAllZapperBeams(param_2,3);
  }
  else if (*(short *)(param_1 + 0xfc) == 10) {
    local_11 = _DoLeverStartStopAllZapperBeams(param_2,4);
  }
  else if (*(short *)(param_1 + 0xfc) == 0xb) {
    _DoLeverToggleGoldSilverBlocks(param_2,1);
  }
  else if (*(short *)(param_1 + 0xfc) == 0xc) {
    _DoLeverToggleGoldSilverBlocks(param_2,2);
  }
  else if (*(short *)(param_1 + 0xfc) == 0xd) {
    _DoLeverOpenAllTrapDoors(param_2);
    *(undefined1 *)(param_1 + 0x104) = 0;
  }
  else if (*(short *)(param_1 + 0xfc) == 0xe) {
    _DoLeverOpenAllGrayDoors(param_1,param_2);
  }
  else if (*(short *)(param_1 + 0xfc) == 0xf) {
    _DoLeverCloseAllGrayDoors(param_1,param_2);
  }
  else if (*(short *)(param_1 + 0xfc) == 0x10) {
    _DoLeverLightAllTorches();
  }
  else if (*(short *)(param_1 + 0xfc) == 0x11) {
    _DoLeverExtinguishAllTorches();
  }
  else if (*(short *)(param_1 + 0xfc) == 0x12) {
    _DoLeverMoveTorchFlameForward();
  }
  else if (*(short *)(param_1 + 0xfc) == 0x13) {
    _DoLeverMoveTorchFlameBackward();
  }
  else if (*(short *)(param_1 + 0xfc) == 0x14) {
    _DoLeverMakeAllSkeletonsComeAlive();
  }
  return local_11;
}


// ---- _PerformActionOnSprite @ 0x70d59 (596 bytes)

undefined1 _PerformActionOnSprite(int param_1,int param_2,char param_3,int param_4)

{
  char cVar1;
  short sVar2;
  undefined1 local_e;
  
  sVar2 = (short)*(undefined4 *)(param_1 + 0xe8);
  local_e = 0;
  if (((sVar2 < 0x2d) || (0x44 < sVar2)) || ((param_4 != 0 && (param_4 != 5)))) {
    if ((sVar2 == 0x5f) || ((sVar2 == 0x60 && ((param_4 == 0 || (param_4 == 5)))))) {
      if ((param_3 == '\0') || (*(char *)(param_2 + 0x107) == '\0')) {
        local_e = _DoLeverOpenTrapDoor(param_1,param_3,1);
        *(undefined1 *)(param_2 + 0x104) = 0;
      }
    }
    else if ((sVar2 < 0x1a) || (0x1d < sVar2)) {
      if ((sVar2 < 0x45) || (0x4f < sVar2)) {
        if ((sVar2 == 0xd) && (param_4 == 3)) {
          _MakeSkeletonDoUncollapse(param_1);
        }
      }
      else {
        cVar1 = *(char *)(param_1 + 0x104);
        if (param_4 == 0) {
          *(bool *)(param_1 + 0x104) = cVar1 == '\0';
        }
        else if (param_4 == 4) {
          *(undefined1 *)(param_1 + 0x104) = 0;
        }
        else if (param_4 == 3) {
          *(undefined1 *)(param_1 + 0x104) = 1;
        }
        if (param_3 == '\0') {
          if (cVar1 == '\0') {
            _MMPlaySound(0x49,0xc,3);
          }
          else {
            _MMPlayStereoSound2(0,0x49,0xc,3,(int)*(short *)PTR__gSoundVolume_001c48dc,
                                0x3feccccccccccccd,0);
          }
        }
      }
    }
    else {
      local_e = _DoLeverAffectOneZapperNode(param_1,param_3,param_4);
    }
  }
  else if ((param_3 == '\0') || (*(char *)(param_2 + 0x107) == '\0')) {
    if (param_4 == 0) {
      local_e = _DoLeverToggleDoor(param_2,param_1,param_3);
    }
    else if (param_4 == 5) {
      local_e = _DoLeverOpenDoor(param_2,param_1,param_3);
      *(undefined1 *)(param_2 + 0x104) = 0;
    }
  }
  return local_e;
}


// ---- _CanLeverAffectSprite @ 0x70fad (145 bytes)

undefined4 _CanLeverAffectSprite(int param_1)

{
  short sVar1;
  undefined4 local_20;
  
  sVar1 = (short)*(undefined4 *)(param_1 + 0xe8);
  if ((sVar1 < 0x2d) || (0x44 < sVar1)) {
    if ((sVar1 == 0x5f) || (sVar1 == 0x60)) {
      local_20 = 1;
    }
    else if ((sVar1 < 0x1a) || (0x1d < sVar1)) {
      if ((sVar1 < 0x45) || (0x4f < sVar1)) {
        if (sVar1 == 0xd) {
          local_20 = 1;
        }
        else {
          local_20 = 0;
        }
      }
      else {
        local_20 = 1;
      }
    }
    else {
      local_20 = 1;
    }
  }
  else {
    local_20 = 1;
  }
  return local_20;
}


// ---- _DoLeverChangeAllMovingPlatforms @ 0x7103e (199 bytes)

void _DoLeverChangeAllMovingPlatforms(undefined1 param_1)

{
  undefined4 local_10;
  
  for (local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c
                                   ,(SpritePtr)0x0); local_10 != (SpritePtr)0x0;
      local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c,
                                  local_10)) {
    _DoLeverChangeOneMovingPlatform(local_10,param_1);
  }
  for (local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,
                                   (SpritePtr)0x0); local_10 != (SpritePtr)0x0;
      local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,
                                  local_10)) {
    _DoLeverChangeOneMovingPlatform(local_10,param_1);
  }
  return;
}


// ---- _DoLeverChangeOneMovingPlatform @ 0x71105 (126 bytes)

void _DoLeverChangeOneMovingPlatform(SpritePtr param_1)

{
  SWMovementType SVar1;
  SWMovementType SVar2;
  
  SVar1 = _SWGetSpriteVertMoveDelta(param_1);
  SVar2 = _SWGetSpriteHorizMoveDelta(param_1);
  _SWSetSpriteMoveDelta(param_1,-SVar2,-SVar1);
  return;
}


// ---- _DoLeverChangeConveyorBeltDir @ 0x71183 (220 bytes)

void _DoLeverChangeConveyorBeltDir(char param_1,int param_2)

{
  if (param_2 == 0) {
    *(short *)PTR__gOldConveyorBeltDirection_001c4f18 =
         -*(short *)PTR__gOldConveyorBeltDirection_001c4f18;
  }
  else if (param_2 == 1) {
    *(undefined2 *)PTR__gOldConveyorBeltDirection_001c4f18 = 1;
  }
  else if (param_2 == 2) {
    *(undefined2 *)PTR__gOldConveyorBeltDirection_001c4f18 = 0xffff;
  }
  if (((param_1 == '\0') && (*(short *)PTR__gCurConveyorBeltDirection_001c470c != 0)) &&
     (*(short *)PTR__gCurConveyorBeltDirection_001c470c !=
      *(short *)PTR__gOldConveyorBeltDirection_001c4f18)) {
    _MMPlaySound(0x46,0xc,3);
  }
  if (*(short *)PTR__gCurConveyorBeltDirection_001c470c != 0) {
    *(undefined2 *)PTR__gCurConveyorBeltDirection_001c470c =
         *(undefined2 *)PTR__gOldConveyorBeltDirection_001c4f18;
  }
  return;
}


// ---- _DoLeverStartOrStopConveyorBelts @ 0x7125f (244 bytes)

void _DoLeverStartOrStopConveyorBelts(char param_1,int param_2)

{
  if (param_2 == 0) {
    if (*(short *)PTR__gCurConveyorBeltDirection_001c470c == 0) {
      param_2 = 3;
    }
    else {
      param_2 = 4;
    }
  }
  if (param_2 == 3) {
    if ((*(short *)PTR__gCurConveyorBeltDirection_001c470c == 0) &&
       (*(undefined2 *)PTR__gCurConveyorBeltDirection_001c470c =
             *(undefined2 *)PTR__gOldConveyorBeltDirection_001c4f18, param_1 == '\0')) {
      _MMPlaySound(0x44,0xc,3);
    }
  }
  else if ((param_2 == 4) && (*(short *)PTR__gCurConveyorBeltDirection_001c470c != 0)) {
    *(undefined2 *)PTR__gOldConveyorBeltDirection_001c4f18 =
         *(undefined2 *)PTR__gCurConveyorBeltDirection_001c470c;
    *(undefined2 *)PTR__gCurConveyorBeltDirection_001c470c = 0;
    if (param_1 == '\0') {
      _MMPlaySound(0x45,0xc,3);
    }
  }
  return;
}


// ---- _DoLeverStartStopAllMovingPlatforms @ 0x71353 (97 bytes)

void _DoLeverStartStopAllMovingPlatforms(undefined1 param_1,undefined4 param_2)

{
  _DoLeverStartStopPlatformsInLayer
            (*(undefined4 *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c,param_2,param_1);
  _DoLeverStartStopPlatformsInLayer
            (*(undefined4 *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,param_2,param_1);
  return;
}


// ---- _DoLeverStartStopPlatformsInLayer @ 0x713b4 (431 bytes)

void _DoLeverStartStopPlatformsInLayer(SpriteLayerPtr param_1,int param_2,char param_3)

{
  uchar uVar1;
  SpritePtr local_14;
  
  for (local_14 = _SWGetNextSprite(param_1,(SpritePtr)0x0); local_14 != (SpritePtr)0x0;
      local_14 = _SWGetNextSprite(param_1,local_14)) {
    uVar1 = local_14->f_104;
    if (param_2 == 0) {
      local_14->f_104 = uVar1 == '\0';
      if (param_3 == '\0') {
        if (uVar1 == '\0') {
          _MMPlaySound(0x49,0xc,3);
        }
        else {
          _MMPlayStereoSound2(0,0x49,0xc,3,(int)*(short *)PTR__gSoundVolume_001c48dc,
                              0x3feccccccccccccd,0);
        }
      }
    }
    else if (param_2 == 3) {
      if ((param_3 == '\0') && (uVar1 == '\0')) {
        _MMPlaySound(0x49,0xc,3);
      }
      local_14->f_104 = '\x01';
    }
    else if (param_2 == 4) {
      if ((param_3 == '\0') && (uVar1 == '\x01')) {
        _MMPlayStereoSound2(0,0x49,0xc,3,(int)*(short *)PTR__gSoundVolume_001c48dc,
                            0x3feccccccccccccd,0);
      }
      local_14->f_104 = '\0';
    }
  }
  return;
}


// ---- _DoLeverToggleGoldSilverBlocks @ 0x71563 (211 bytes)

void _DoLeverToggleGoldSilverBlocks(char param_1,int param_2)

{
  if (param_1 == '\0') {
    _MMPlaySound(0x42,0xc,3);
    _DisposeAllMapCachedRooms();
  }
  if (param_2 == 0) {
    *PTR__gSilverFloorIsActive_001c4958 = *PTR__gSilverFloorIsActive_001c4958 == '\0';
    *PTR__gGoldFloorIsActive_001c47a8 = *PTR__gGoldFloorIsActive_001c47a8 == '\0';
  }
  else if (param_2 == 1) {
    *PTR__gSilverFloorIsActive_001c4958 = 1;
    *PTR__gGoldFloorIsActive_001c47a8 = 0;
  }
  else if (param_2 == 2) {
    *PTR__gSilverFloorIsActive_001c4958 = 0;
    *PTR__gGoldFloorIsActive_001c47a8 = 1;
  }
  _SetCorrectGoldSilverTileImages(1);
  if (param_1 == '\0') {
    _DrawBackgroundTextureForCurRoom();
  }
  return;
}


// ---- _SetCorrectGoldSilverTileImages @ 0x71636 (313 bytes)

void _SetCorrectGoldSilverTileImages(char param_1)

{
  undefined2 local_10;
  undefined2 local_e;
  
  if (*PTR__gSilverFloorIsActive_001c4958 == '\0') {
    local_e = 0x1c;
  }
  else {
    local_e = 0;
  }
  for (local_10 = 0x1af; local_10 < 0x1bd; local_10 = local_10 + 1) {
    if (param_1 == '\0') {
      _SWChangeTileImageFast
                (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,
                 (int)(short)(local_e + local_10));
    }
    else {
      _SWChangeTileImage(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_10,local_e + local_10)
      ;
    }
  }
  if (*PTR__gGoldFloorIsActive_001c47a8 == '\0') {
    local_e = 0x1c;
  }
  else {
    local_e = 0;
  }
  for (local_10 = 0x1bd; local_10 < 0x1cb; local_10 = local_10 + 1) {
    if (param_1 == '\0') {
      _SWChangeTileImageFast
                (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,
                 (int)(short)(local_e + local_10));
    }
    else {
      _SWChangeTileImage(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_10,local_e + local_10)
      ;
    }
  }
  return;
}


// ---- _DoLeverOpenAllGrayDoors @ 0x7176f (265 bytes)

undefined1 _DoLeverOpenAllGrayDoors(undefined4 param_1,undefined1 param_2)

{
  char cVar1;
  SpritePtr pSVar2;
  SpritePtr local_18;
  undefined1 local_d;
  
  local_d = 0;
  local_18 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,(SpritePtr)0x0);
  while (local_18 != (SpritePtr)0x0) {
    pSVar2 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,local_18);
    if (((0x40 < local_18->userData) && (local_18->userData < 0x45)) &&
       (cVar1 = _DoLeverOpenDoor(param_1,local_18,param_2), cVar1 != '\0')) {
      local_d = 1;
    }
    local_18 = pSVar2;
    if ((pSVar2 != (SpritePtr)0x0) &&
       (pSVar2->parentSpriteLayerP != *(SpriteLayerRec **)PTR__gDoorSpriteLayerP_001c41dc)) {
      local_18 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,(SpritePtr)0x0)
      ;
    }
  }
  _SWCustomSortSpriteLayer
            (*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,
             (SpriteCompareProcPtr_t)PTR_LAB_001c4ef0);
  return local_d;
}


// ---- _DoLeverToggleDoor @ 0x71878 (108 bytes)

undefined1 _DoLeverToggleDoor(undefined4 param_1,int param_2,char param_3)

{
  undefined1 local_d;
  
  local_d = 0;
  if (param_3 == '\0') {
    if (*(char *)(param_2 + 0x104) == '\0') {
      local_d = _DoLeverOpenDoor(param_1,param_2,0);
    }
    else {
      local_d = _DoLeverCloseDoor(param_1,param_2,0);
    }
  }
  return local_d;
}


// ---- _DoLeverOpenDoor @ 0x718e4 (224 bytes)

undefined1 _DoLeverOpenDoor(int param_1,int param_2,char param_3)

{
  undefined *puVar1;
  undefined1 local_d;
  
  local_d = 0;
  if ((*(char *)(param_2 + 0x104) == '\0') &&
     (puVar1 = (undefined *)_SWGetSpriteMoveProc(param_2), puVar1 != PTR_DoorSpriteMoveProc_001c4d54
     )) {
    if (param_3 == '\0') {
      _MakeDoorOpen(param_2,*(char *)(param_1 + 0x106) == '\0');
      _MMPlayStereoSound(param_2,1,8,3);
      local_d = 1;
    }
    else {
      _SWCollideSpriteWithLayer
                (param_2,*(undefined4 *)PTR__gDoorSpriteLayerP_001c41dc,
                 PTR_OpenDoorInstantlyCollideProc_001c4f14);
      _SetDoorToOpenedState(param_2);
    }
  }
  return local_d;
}


// ---- _DoLeverCloseAllGrayDoors @ 0x719c4 (265 bytes)

undefined1 _DoLeverCloseAllGrayDoors(undefined4 param_1,undefined1 param_2)

{
  char cVar1;
  SpritePtr pSVar2;
  SpritePtr local_18;
  undefined1 local_d;
  
  local_d = 0;
  local_18 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gOpenedDoorSpriteLayerP_001c49a4,
                              (SpritePtr)0x0);
  while (local_18 != (SpritePtr)0x0) {
    pSVar2 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gOpenedDoorSpriteLayerP_001c49a4,local_18);
    if (((0x40 < local_18->userData) && (local_18->userData < 0x45)) &&
       (cVar1 = _DoLeverCloseDoor(param_1,local_18,param_2), cVar1 != '\0')) {
      local_d = 1;
    }
    local_18 = pSVar2;
    if ((pSVar2 != (SpritePtr)0x0) &&
       (pSVar2->parentSpriteLayerP != *(SpriteLayerRec **)PTR__gOpenedDoorSpriteLayerP_001c49a4)) {
      local_18 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gOpenedDoorSpriteLayerP_001c49a4,
                                  (SpritePtr)0x0);
    }
  }
  _SWCustomSortSpriteLayer
            (*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,
             (SpriteCompareProcPtr_t)PTR_LAB_001c4ef0);
  return local_d;
}


// ---- _DoLeverCloseDoor @ 0x71acd (254 bytes)

undefined1 _DoLeverCloseDoor(int param_1,int param_2,char param_3)

{
  undefined *puVar1;
  undefined1 local_d;
  
  local_d = 0;
  if ((*(char *)(param_2 + 0x104) == '\x01') &&
     (puVar1 = (undefined *)_SWGetSpriteMoveProc(param_2), puVar1 != PTR_LAB_001c4d64)) {
    if (param_3 == '\0') {
      _MakeDoorClose(param_2,*(char *)(param_1 + 0x106) == '\0');
      _MMPlayStereoSound(param_2,0x29,8,3);
      local_d = 1;
    }
    else {
      _SetDoorToClosedState(param_2);
      _SWCollideSpriteWithLayer
                (param_2,*(undefined4 *)PTR__gDoorSpriteLayerP_001c41dc,
                 PTR_CloseDoorInstantlyCollideProc_001c4eec);
    }
  }
  _SWCustomSortSpriteLayer
            (*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,
             (SpriteCompareProcPtr_t)PTR_LAB_001c4ef0);
  return local_d;
}


// ---- _DoLeverOpenAllTrapDoors @ 0x71bcb (205 bytes)

undefined1 _DoLeverOpenAllTrapDoors(undefined1 param_1)

{
  char cVar1;
  SpritePtr pSVar2;
  SpritePtr local_18;
  undefined1 local_d;
  
  local_d = 0;
  local_18 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gTrapDoorSpriteLayerP_001c40e0,(SpritePtr)0x0)
  ;
  while (local_18 != (SpritePtr)0x0) {
    pSVar2 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gTrapDoorSpriteLayerP_001c40e0,local_18);
    cVar1 = _DoLeverOpenTrapDoor(local_18,param_1,0);
    if (cVar1 != '\0') {
      local_d = 1;
    }
    local_18 = pSVar2;
    if ((pSVar2 != (SpritePtr)0x0) &&
       (pSVar2->parentSpriteLayerP != *(SpriteLayerRec **)PTR__gTrapDoorSpriteLayerP_001c40e0)) {
      local_18 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gTrapDoorSpriteLayerP_001c40e0,
                                  (SpritePtr)0x0);
    }
  }
  return local_d;
}


// ---- _DoLeverOpenTrapDoor @ 0x71c98 (195 bytes)

undefined1 _DoLeverOpenTrapDoor(int *param_1,char param_2,char param_3)

{
  short sVar1;
  undefined1 local_d;
  
  local_d = 0;
  if (param_2 == '\0') {
    if (*param_1 == *(int *)PTR__gTrapDoorSpriteLayerP_001c40e0) {
      _MakeTrapDoorOpen(param_1);
      if (param_3 != '\0') {
        _SWCollideSpriteWithLayer
                  (param_1,*(undefined4 *)PTR__gTrapDoorSpriteLayerP_001c40e0,
                   PTR_OpenTrapDoorCollideProc_001c4f04);
      }
      sVar1 = _FindSound(0x3b);
      if (sVar1 == -1) {
        _MMPlaySound(0x3b,8,1);
        local_d = 1;
      }
    }
  }
  else {
    _SetTrapDoorToOpenedState(param_1,param_3);
  }
  return local_d;
}


// ---- _DoLeverStartStopAllZapperBeams @ 0x71d5b (138 bytes)

undefined1 _DoLeverStartStopAllZapperBeams(undefined1 param_1,undefined4 param_2)

{
  char cVar1;
  undefined4 local_14;
  undefined1 local_d;
  
  local_d = 0;
  for (local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gZapperNodeSpriteLayerP_001c418c,
                                   (SpritePtr)0x0); local_14 != (SpritePtr)0x0;
      local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gZapperNodeSpriteLayerP_001c418c,local_14)
      ) {
    cVar1 = _DoLeverAffectOneZapperNode(local_14,param_1,param_2);
    if (cVar1 != '\0') {
      local_d = 1;
    }
  }
  return local_d;
}


// ---- _DoLeverAffectOneZapperNode @ 0x71de5 (472 bytes)

undefined1 _DoLeverAffectOneZapperNode(SpritePtr param_1,char param_2,int param_3)

{
  uchar uVar1;
  int iVar2;
  undefined1 local_f;
  ushort local_e;
  
  if ((param_1->userData < 0x1a) || (0x1d < param_1->userData)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Levers.c",
               PTR____FUNCTION___28550_001c4f00,0x658);
  }
  uVar1 = param_1->f_105;
  local_f = 0;
  if (param_3 == 0) {
    if (param_1->f_105 == '\0') {
      param_1->f_105 = '\x01';
    }
    else {
      param_1->f_105 = '\0';
    }
  }
  else if (param_3 == 3) {
    param_1->f_105 = '\0';
  }
  else if (param_3 == 4) {
    param_1->f_105 = '\x01';
  }
  if ((param_1->f_105 == '\x01') || (param_1->f_104 != '\0')) {
    local_e = (ushort)(param_1->f_105 != '\x01');
    _SWSetSpriteFrameAdvance(param_1,(int)(short)local_e);
    iVar2._0_1_ = param_1->f_10c;
    iVar2._1_1_ = param_1->f_10d;
    iVar2._2_1_ = param_1->f_10e;
    iVar2._3_1_ = param_1->f_10f;
    if (iVar2 == 0) {
      _SWSetSpriteColor(param_1,0xffff,0,0);
      local_f = 1;
    }
    else {
      _SWSetSpriteFrameAdvance(*(SpritePtr *)&param_1->f_10c,(int)(short)local_e);
      if ((param_2 == '\0') && (param_1->f_105 != uVar1)) {
        if (param_1->f_105 == '\x01') {
          _MMPlaySound(0x24,0xc,3);
        }
        else {
          _MMPlaySound(0x25,0xc,3);
        }
        local_f = 1;
      }
    }
  }
  return local_f;
}


// ---- _DoLeverToggleAllTorches @ 0x71fbd (127 bytes)

void _DoLeverToggleAllTorches(void)

{
  int iVar1;
  SpritePtr local_10;
  
  for (local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gTorchSpriteLayerP_001c4144,
                                   (SpritePtr)0x0); local_10 != (SpritePtr)0x0;
      local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gTorchSpriteLayerP_001c4144,local_10)) {
    iVar1._0_1_ = local_10->f_0fc;
    iVar1._1_1_ = local_10->f_0fd;
    iVar1._2_1_ = local_10->f_0fe;
    iVar1._3_1_ = local_10->f_0ff;
    if (iVar1 == 0) {
      _LightTorch(local_10);
    }
    else {
      _UnlightTorch(local_10);
    }
  }
  return;
}


// ---- _DoLeverLightAllTorches @ 0x7203c (114 bytes)

void _DoLeverLightAllTorches(void)

{
  int iVar1;
  SpritePtr local_10;
  
  for (local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gTorchSpriteLayerP_001c4144,
                                   (SpritePtr)0x0); local_10 != (SpritePtr)0x0;
      local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gTorchSpriteLayerP_001c4144,local_10)) {
    iVar1._0_1_ = local_10->f_0fc;
    iVar1._1_1_ = local_10->f_0fd;
    iVar1._2_1_ = local_10->f_0fe;
    iVar1._3_1_ = local_10->f_0ff;
    if (iVar1 == 0) {
      _LightTorch(local_10);
    }
  }
  return;
}


// ---- _DoLeverExtinguishAllTorches @ 0x720ae (114 bytes)

void _DoLeverExtinguishAllTorches(void)

{
  int iVar1;
  SpritePtr local_10;
  
  for (local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gTorchSpriteLayerP_001c4144,
                                   (SpritePtr)0x0); local_10 != (SpritePtr)0x0;
      local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gTorchSpriteLayerP_001c4144,local_10)) {
    iVar1._0_1_ = local_10->f_0fc;
    iVar1._1_1_ = local_10->f_0fd;
    iVar1._2_1_ = local_10->f_0fe;
    iVar1._3_1_ = local_10->f_0ff;
    if (iVar1 != 0) {
      _UnlightTorch(local_10);
    }
  }
  return;
}


// ---- _DoLeverMoveTorchFlameForward @ 0x72120 (188 bytes)

void _DoLeverMoveTorchFlameForward(void)

{
  int iVar1;
  int iVar2;
  SpritePtr pSVar3;
  SpritePtr local_18;
  
  local_18 = _SWGetPreviousSprite
                       (*(SpriteLayerPtr *)PTR__gTorchSpriteLayerP_001c4144,(SpritePtr)0x0);
  iVar1._0_1_ = local_18->f_0fc;
  iVar1._1_1_ = local_18->f_0fd;
  iVar1._2_1_ = local_18->f_0fe;
  iVar1._3_1_ = local_18->f_0ff;
  while (local_18 != (SpritePtr)0x0) {
    pSVar3 = _SWGetPreviousSprite(*(SpriteLayerPtr *)PTR__gTorchSpriteLayerP_001c4144,local_18);
    if (pSVar3 == (SpritePtr)0x0) {
      if (iVar1 == 0) {
        _UnlightTorch(local_18);
        local_18 = pSVar3;
      }
      else {
        _LightTorch(local_18);
        local_18 = pSVar3;
      }
    }
    else {
      iVar2._0_1_ = pSVar3->f_0fc;
      iVar2._1_1_ = pSVar3->f_0fd;
      iVar2._2_1_ = pSVar3->f_0fe;
      iVar2._3_1_ = pSVar3->f_0ff;
      if (iVar2 == 0) {
        _UnlightTorch(local_18);
        local_18 = pSVar3;
      }
      else {
        _LightTorch(local_18);
        local_18 = pSVar3;
      }
    }
  }
  return;
}


// ---- _DoLeverMoveTorchFlameBackward @ 0x721dc (188 bytes)

void _DoLeverMoveTorchFlameBackward(void)

{
  int iVar1;
  int iVar2;
  SpritePtr pSVar3;
  SpritePtr local_18;
  
  local_18 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gTorchSpriteLayerP_001c4144,(SpritePtr)0x0);
  iVar1._0_1_ = local_18->f_0fc;
  iVar1._1_1_ = local_18->f_0fd;
  iVar1._2_1_ = local_18->f_0fe;
  iVar1._3_1_ = local_18->f_0ff;
  while (local_18 != (SpritePtr)0x0) {
    pSVar3 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gTorchSpriteLayerP_001c4144,local_18);
    if (pSVar3 == (SpritePtr)0x0) {
      if (iVar1 == 0) {
        _UnlightTorch(local_18);
        local_18 = pSVar3;
      }
      else {
        _LightTorch(local_18);
        local_18 = pSVar3;
      }
    }
    else {
      iVar2._0_1_ = pSVar3->f_0fc;
      iVar2._1_1_ = pSVar3->f_0fd;
      iVar2._2_1_ = pSVar3->f_0fe;
      iVar2._3_1_ = pSVar3->f_0ff;
      if (iVar2 == 0) {
        _UnlightTorch(local_18);
        local_18 = pSVar3;
      }
      else {
        _LightTorch(local_18);
        local_18 = pSVar3;
      }
    }
  }
  return;
}


// ---- _DoLeverMakeAllSkeletonsComeAlive @ 0x72298 (107 bytes)

void _DoLeverMakeAllSkeletonsComeAlive(void)

{
  SpritePtr pSVar1;
  undefined4 local_14;
  
  local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gSkeletonBonesSpriteLayerP_001c40a0,
                              (SpritePtr)0x0);
  while (local_14 != (SpritePtr)0x0) {
    pSVar1 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gSkeletonBonesSpriteLayerP_001c40a0,local_14);
    _MakeSkeletonDoUncollapse(local_14);
    local_14 = pSVar1;
  }
  return;
}


// ---- _LightTorch @ 0x72303 (284 bytes)

void _LightTorch(SpritePtr param_1)

{
  int iVar1;
  SpritePtr srcSpriteP;
  longdouble extraout_ST0;
  SWMovementType SVar2;
  short local_14;
  short local_12;
  
  iVar1._0_1_ = param_1->f_0fc;
  iVar1._1_1_ = param_1->f_0fd;
  iVar1._2_1_ = param_1->f_0fe;
  iVar1._3_1_ = param_1->f_0ff;
  if (iVar1 == 0) {
    srcSpriteP = (SpritePtr)_NewFlameSprite();
    if (srcSpriteP != (SpritePtr)0x0) {
      SVar2 = _SWGetSpriteHorizLoc(param_1);
      local_12 = (short)(int)SVar2;
      SVar2 = _SWGetSpriteVertLoc(param_1);
      _SWGetSpriteHeight(srcSpriteP);
      local_14 = (short)(int)(SVar2 - (float)extraout_ST0);
      if (param_1->userData == 0x9f) {
        local_14 = local_14 + 5;
      }
      else if (param_1->userData == 0xa0) {
        local_14 = local_14 + 1;
      }
      else if (param_1->userData == 0x9e) {
        local_12 = local_12 + -3;
        local_14 = local_14 + 7;
      }
      _SWSetSpriteLocation(srcSpriteP,(float)(int)local_12,(float)(int)local_14);
      _MySetSpriteFrameTime(param_1,0x4c);
      _CreateGlowOverSprite(param_1);
    }
    *(SpritePtr *)&param_1->f_0fc = srcSpriteP;
  }
  return;
}


// ---- _UnlightTorch @ 0x7241f (228 bytes)

void _UnlightTorch(SpritePtr param_1)

{
  int iVar1;
  int iVar2;
  
  if (((param_1->userData != 0x9e) && (param_1->userData != 0x9f)) && (param_1->userData != 0xa0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Levers.c",
               PTR____FUNCTION___28740_001c4ef8,0x771);
  }
  iVar1._0_1_ = param_1->f_0fc;
  iVar1._1_1_ = param_1->f_0fd;
  iVar1._2_1_ = param_1->f_0fe;
  iVar1._3_1_ = param_1->f_0ff;
  if (iVar1 != 0) {
    _SWRemoveSpriteFromAnimation(*(SpritePtr *)&param_1->f_0fc,'\x01');
    iVar2._0_1_ = param_1->f_100;
    iVar2._1_1_ = param_1->f_101;
    iVar2._2_1_ = param_1->f_102;
    iVar2._3_1_ = param_1->f_103;
    if (iVar2 != 0) {
      _SWRemoveSpriteFromAnimation(*(SpritePtr *)&param_1->f_100,'\x01');
    }
    _SWSetSpriteFrameTime(param_1,-1);
    param_1->f_0fc = '\0';
    param_1->f_0fd = '\0';
    param_1->f_0fe = '\0';
    param_1->f_0ff = '\0';
  }
  return;
}


// ---- _ResetConveyorBeltDir @ 0x72503 (46 bytes)

void _ResetConveyorBeltDir(void)

{
  *(undefined2 *)PTR__gCurConveyorBeltDirection_001c470c = 1;
  *(undefined2 *)PTR__gOldConveyorBeltDirection_001c4f18 = 1;
  return;
}


// ---- _CompareSpriteTop @ 0x72531 (36 bytes)

int CompareSpriteTop(int param_1,int param_2)

{
  return (int)(short)(*(short *)(param_2 + 0x3a) - *(short *)(param_1 + 0x3a));
}


// ---- _CompareSpriteBottom @ 0x72555 (36 bytes)

int CompareSpriteBottom(int param_1,int param_2)

{
  return (int)(short)(*(short *)(param_1 + 0x3a) - *(short *)(param_2 + 0x3a));
}


