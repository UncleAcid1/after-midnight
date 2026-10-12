// Ghidra decompilation of functions from original source file:
//   Sources & Headers/ZapperStuff.c

// ---- _ZapperNodeMoveProc @ 0x6df19 (475 bytes)

void ZapperNodeMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  SpritePtr srcSpriteP;
  int iVar3;
  int iVar4;
  int iVar5;
  uchar uVar6;
  SWBoolean SStack_20;
  short sStack_e;
  
  srcSpriteP = *(SpritePtr *)&param_1->f_10c;
  if ((srcSpriteP != (SpritePtr)0x0) &&
     (*(short *)&param_1->f_100 = *(short *)&param_1->f_100 + -1, sVar1._0_1_ = param_1->f_100,
     sVar1._1_1_ = param_1->f_101, sVar1 < 1)) {
    sStack_e = 0;
    do {
      param_1->f_104 = param_1->f_104 == '\0';
      if (param_1->f_104 == '\0') {
        uVar6 = param_1->f_0ff;
        param_1->f_100 = param_1->f_0fe;
        param_1->f_101 = uVar6;
        _SWSetSpriteFrameAdvance(param_1,0);
        _SWSetSpriteFrameAdvance(srcSpriteP,0);
      }
      else {
        param_1->f_102 = '\0';
        param_1->f_103 = '\0';
        uVar6 = param_1->f_0fd;
        param_1->f_100 = param_1->f_0fc;
        param_1->f_101 = uVar6;
        if (param_1->f_105 == '\0') {
          _SWSetSpriteFrameAdvance(param_1,1);
          _SWSetSpriteFrameAdvance(srcSpriteP,1);
        }
      }
      if ((param_1->f_104 == '\0') || (param_1->f_105 != '\0')) {
        SStack_20 = '\0';
      }
      else {
        SStack_20 = '\x01';
      }
      iVar3._0_1_ = param_1->f_110;
      iVar3._1_1_ = param_1->f_111;
      iVar3._2_1_ = param_1->f_112;
      iVar3._3_1_ = param_1->f_113;
      if (iVar3 != 0) {
        _SWSetSpriteVisible(*(SpritePtr *)&param_1->f_110,SStack_20);
      }
      iVar4._0_1_ = param_1->f_114;
      iVar4._1_1_ = param_1->f_115;
      iVar4._2_1_ = param_1->f_116;
      iVar4._3_1_ = param_1->f_117;
      if (iVar4 != 0) {
        _SWSetSpriteVisible(*(SpritePtr *)&param_1->f_114,SStack_20);
      }
      iVar5._0_1_ = param_1->f_118;
      iVar5._1_1_ = param_1->f_119;
      iVar5._2_1_ = param_1->f_11a;
      iVar5._3_1_ = param_1->f_11b;
      if (iVar5 != 0) {
        _SWSetSpriteVisible(*(SpritePtr *)&param_1->f_118,SStack_20);
      }
      sStack_e = sStack_e + 1;
      sVar2._0_1_ = param_1->f_100;
      sVar2._1_1_ = param_1->f_101;
    } while ((sVar2 == 0) && (sStack_e < 2));
  }
  return;
}


// ---- _ZapperNodeFrameProc @ 0x6e0f4 (209 bytes)

void ZapperNodeFrameProc(int param_1)

{
  ushort uVar1;
  
  *(short *)(param_1 + 0x102) = *(short *)(param_1 + 0x102) + 1;
  if (7 < *(short *)(param_1 + 0x102)) {
    *(undefined2 *)(param_1 + 0x102) = 0;
  }
  uVar1 = _GetRandom(0x3fff,0x9fff);
  if (*(int *)(param_1 + 0x110) != 0) {
    _SWSetSpriteTranslucency(*(SpritePtr *)(param_1 + 0x110),(uint)uVar1);
  }
  if (*(int *)(param_1 + 0x114) != 0) {
    _SWSetSpriteTranslucency(*(SpritePtr *)(param_1 + 0x114),(uint)uVar1);
  }
  if (*(int *)(param_1 + 0x118) != 0) {
    _SWSetSpriteTranslucency(*(SpritePtr *)(param_1 + 0x118),(uint)uVar1);
  }
  return;
}


// ---- _ZapperBeamMoveProc @ 0x6e1c5 (240 bytes)

void _ZapperBeamMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  int iVar3;
  
  iVar3 = *(int *)&param_1->f_0fc;
  if (param_1->f_102 == '\0') {
    sVar2._0_1_ = param_1->f_100;
    sVar2._1_1_ = param_1->f_101;
    _SWSetCurrentFrameIndex(param_1,(int)*(short *)(iVar3 + 0x102) + (int)sVar2);
  }
  else {
    sVar1._0_1_ = param_1->f_100;
    sVar1._1_1_ = param_1->f_101;
    _SWSetCurrentFrameIndex(param_1,*(short *)(iVar3 + 0x102) * 8 + (int)sVar1);
  }
  if ((*(char *)(iVar3 + 0x104) == '\0') || (*(char *)(iVar3 + 0x105) != '\0')) {
    if (param_1->isVisible == '\x01') {
      _SWSetSpriteVisible(param_1,'\0');
    }
  }
  else if (param_1->isVisible == '\0') {
    _SWSetSpriteVisible(param_1,'\x01');
  }
  return;
}


// ---- _ZapperGlowMoveProc @ 0x6e2b5 (96 bytes)

void _ZapperGlowMoveProc(SpritePtr param_1)

{
  SWBoolean local_20;
  
  if ((*(char *)(param_1->misc1 + 0x104) == '\0') || (*(char *)(param_1->misc1 + 0x105) != '\0')) {
    local_20 = '\0';
  }
  else {
    local_20 = '\x01';
  }
  _SWSetSpriteVisible(param_1,local_20);
  return;
}


// ---- _ZapperCanShootThroughTile @ 0x6e315 (33 bytes)

undefined4 _ZapperCanShootThroughTile(void)

{
  return 1;
}


// ---- _DeleteZapperSpritesForNode @ 0x6e336 (360 bytes)

void _DeleteZapperSpritesForNode(SpritePtr param_1)

{
  short sVar1;
  SWMovementType SVar2;
  short local_14;
  short local_12;
  
  SVar2 = _SWGetSpriteVertLoc(param_1);
  local_14 = (short)(int)(SVar2 / (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4)
                         );
  SVar2 = _SWGetSpriteHorizLoc(param_1);
  local_12 = (short)(int)(SVar2 / (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe2)
                         );
  if (param_1->userData == 0x1a) {
    sVar1 = *(short *)&param_1->f_108 + local_12;
    while (local_12 = local_12 + 1, local_12 <= sVar1) {
      _DeleteZapperBeam((int)local_14,(int)local_12,param_1);
    }
  }
  else if (param_1->userData == 0x1d) {
    sVar1 = local_14 - *(short *)&param_1->f_108;
    while (local_14 = local_14 + -1, sVar1 <= local_14) {
      _DeleteZapperBeam((int)local_14,(int)local_12,param_1);
    }
  }
  else {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/ZapperStuff.c",
               PTR____FUNCTION___27562_001c4ed0,199);
  }
  return;
}


// ---- _DeleteZapperBeam @ 0x6e49e (347 bytes)

void _DeleteZapperBeam(short param_1,short param_2,int param_3)

{
  int iVar1;
  bool bVar2;
  SpritePtr local_14;
  
  bVar2 = false;
  local_14 = (SpritePtr)0x0;
  while (!bVar2) {
    local_14 = (SpritePtr)
               _FindSpriteAtTileRowAndCol
                         (*(undefined4 *)PTR__gZapperBeamSpriteLayerP_001c41ec,local_14,(int)param_1
                          ,(int)param_2);
    if (local_14 == (SpritePtr)0x0) {
      bVar2 = true;
    }
    else {
      iVar1._0_1_ = local_14->f_0fc;
      iVar1._1_1_ = local_14->f_0fd;
      iVar1._2_1_ = local_14->f_0fe;
      iVar1._3_1_ = local_14->f_0ff;
      if (iVar1 == param_3) {
        _SWRemoveSpriteFromAnimation(local_14,'\x01');
        if (*(int *)(param_3 + 0x110) != 0) {
          _SWRemoveSpriteFromAnimation(*(SpritePtr *)(param_3 + 0x110),'\x01');
        }
        if (*(int *)(param_3 + 0x114) != 0) {
          _SWRemoveSpriteFromAnimation(*(SpritePtr *)(param_3 + 0x114),'\x01');
        }
        if (*(int *)(param_3 + 0x118) != 0) {
          _SWRemoveSpriteFromAnimation(*(SpritePtr *)(param_3 + 0x118),'\x01');
        }
        *(undefined4 *)(param_3 + 0x110) = 0;
        *(undefined4 *)(param_3 + 0x114) = 0;
        *(undefined4 *)(param_3 + 0x118) = 0;
        bVar2 = true;
      }
      else {
        local_14 = local_14->prevSpriteP;
        if (local_14 == (SpritePtr)0x0) {
          bVar2 = true;
        }
      }
    }
  }
  return;
}


// ---- _CreateZapperSpritesForNode @ 0x6e5f9 (1128 bytes)

void _CreateZapperSpritesForNode(SpritePtr param_1,short param_2)

{
  short sVar1;
  undefined4 uVar2;
  SWError SVar3;
  SpritePtr srcSpriteP;
  SWMovementType SVar4;
  SpritePtr local_3c;
  short local_36;
  short local_34;
  short local_32;
  short local_30;
  short local_2a;
  short local_28;
  short local_22;
  char local_1d;
  
  if ((param_1->userData == 0x1a) || (param_1->userData == 0x1d)) {
    local_3c = param_1;
  }
  else {
    local_3c = *(SpritePtr *)&param_1->f_10c;
  }
  SVar4 = _SWGetSpriteVertLoc(param_1);
  local_36 = (short)(int)(SVar4 / (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4)
                         );
  SVar4 = _SWGetSpriteHorizLoc(param_1);
  local_34 = (short)(int)(SVar4 / (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe2)
                         );
  local_28 = 0;
  local_2a = 0;
  local_22 = 0;
  local_32 = local_36;
  if (param_1->userData == 0x1a) {
    local_28 = 1;
    local_34 = local_34 + 1;
    local_30 = param_2 + local_34;
    local_1d = '\x01';
  }
  else {
    local_30 = local_34;
    if (param_1->userData == 0x1b) {
      local_2a = 1;
      local_36 = local_36 + 1;
      local_32 = param_2 + local_36;
      local_1d = '\0';
    }
    else if (param_1->userData == 0x1c) {
      local_28 = -1;
      local_34 = local_34 + -1;
      local_30 = local_34 - param_2;
      local_1d = '\x01';
    }
    else if (param_1->userData == 0x1d) {
      local_2a = -1;
      local_36 = local_36 + -1;
      local_32 = local_36 - param_2;
      local_1d = '\0';
    }
  }
  uVar2 = _CreateZapperGlowSprite
                    (local_3c,1,local_1d,(int)(short)(*(short *)&param_1->f_108 + -1),(int)local_34,
                     (int)local_36,0);
  param_1->f_114 = (char)uVar2;
  param_1->f_115 = (char)((uint)uVar2 >> 8);
  param_1->f_116 = (char)((uint)uVar2 >> 0x10);
  param_1->f_117 = (char)((uint)uVar2 >> 0x18);
  uVar2 = _CreateZapperGlowSprite
                    (local_3c,2,local_1d,0,(int)(short)(local_34 - local_28),
                     (int)(short)(local_36 - local_2a),0);
  param_1->f_110 = (char)uVar2;
  param_1->f_111 = (char)((uint)uVar2 >> 8);
  param_1->f_112 = (char)((uint)uVar2 >> 0x10);
  param_1->f_113 = (char)((uint)uVar2 >> 0x18);
  uVar2 = _CreateZapperGlowSprite
                    (local_3c,0,local_1d,0,(int)(short)(local_28 + local_30),
                     (int)(short)(local_2a + local_32),0);
  param_1->f_118 = (char)uVar2;
  param_1->f_119 = (char)((uint)uVar2 >> 8);
  param_1->f_11a = (char)((uint)uVar2 >> 0x10);
  param_1->f_11b = (char)((uint)uVar2 >> 0x18);
  SVar3 = _SWStickyError();
  if (SVar3 != 0) {
    SVar3 = _SWStickyError();
    _MMFatalErrorCallback
              (SVar3,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/ZapperStuff.c",
               PTR____FUNCTION___27646_001c4ed8,0x141);
  }
  while ((local_36 != local_32 || (local_34 != local_30))) {
    srcSpriteP = (SpritePtr)_NewZapperBeamSprite(local_1d);
    if (srcSpriteP != (SpritePtr)0x0) {
      _SWSetSpriteLocation
                (srcSpriteP,
                 (float)((int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe2) * (int)local_34
                        ),(float)((int)local_36 *
                                 (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4)));
      if (local_1d == '\0') {
        sVar1 = (3 - local_22) * 8 + 0x20;
        _SWSetCurrentFrameIndex(srcSpriteP,(int)sVar1);
        *(short *)&srcSpriteP->f_100 = sVar1;
      }
      else {
        _SWSetCurrentFrameIndex(srcSpriteP,(int)local_22);
        srcSpriteP->f_100 = (undefined1)local_22;
        srcSpriteP->f_101 = local_22._1_1_;
      }
      *(SpritePtr *)&srcSpriteP->f_0fc = local_3c;
      _ZapperBeamMoveProc(srcSpriteP);
      local_36 = local_36 + local_2a;
      local_34 = local_34 + local_28;
      if (((int)local_34 + (int)local_28 == (int)local_30) && (local_1d == '\x01')) {
        local_22 = local_22 + 4;
      }
      else if (((int)local_36 + (int)local_2a == (int)local_32) && (local_1d == '\0')) {
        local_22 = local_22 + 4;
      }
      else {
        local_22 = local_22 + 1;
        if (3 < local_22) {
          local_22 = 0;
        }
      }
    }
  }
  return;
}


// ---- _CreateZapperGlowSprite @ 0x6ea61 (801 bytes)

SpritePtr _CreateZapperGlowSprite
                    (int param_1,short param_2,char param_3,short param_4,short param_5,
                    short param_6,short param_7)

{
  SpritePtr newSpriteP;
  short local_40;
  short local_3c;
  
  local_3c = param_5;
  local_40 = param_6;
  newSpriteP = (SpritePtr)_NewMiscSprite(0xd5);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gZapperGlowSpriteLayerP_001c4748,newSpriteP);
  _SWSetSpriteMoveProc(newSpriteP,(MoveProcPtr_t)0x0);
  _SWSetSpriteCollideProc(newSpriteP,(CollideProcPtr_t)0x0);
  _SWSetSpriteBlendMode(newSpriteP,0x302,1);
  _SWSetSpriteTranslucency(newSpriteP,0x8fff);
  if (param_3 == '\0') {
    if (param_2 == 0) {
      local_40 = (short)(int)((double)(int)param_6 + 1.0);
    }
    _SWSetSpriteLocation
              (newSpriteP,
               (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe2) *
               ((float)(int)param_5 - 1.5),
               (float)(int)param_7 +
               ((((float)(int)local_40 + 0.5) - (float)(int)param_4) - 0.2) *
               (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4));
  }
  else {
    if (param_2 == 0) {
      local_3c = (short)(int)((double)(int)param_5 - 0.5);
    }
    _SWSetSpriteRotation(newSpriteP,0x42b40000);
    _SWSetSpriteLocation
              (newSpriteP,
               (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe2) *
               ((((float)(int)local_3c - 1.8) + (float)(int)param_4) - 0.5),
               (float)(int)param_7 +
               ((float)(int)param_6 - 0.5) *
               (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4));
  }
  _SWSetSpriteScaledFactor(newSpriteP,0x3f000000,(float)(param_4 + 1) * 0.25);
  _SWSetCurrentFrameIndex(newSpriteP,(int)param_2);
  _SWSetSpriteVisible(newSpriteP,'\0');
  newSpriteP->misc1 = param_1;
  _SWSetSpriteMoveProc(newSpriteP,(MoveProcPtr_t)PTR_LAB_001c4ee4);
  _ZapperGlowMoveProc(newSpriteP);
  return newSpriteP;
}


// ---- _CreateZapperBeamsForRoom @ 0x6ed82 (192 bytes)

void _CreateZapperBeamsForRoom(void)

{
  SWError SVar1;
  int local_10;
  
  for (local_10 = *(int *)(*(int *)PTR__gZapperNodeSpriteLayerP_001c418c + 0xc); local_10 != 0;
      local_10 = *(int *)(local_10 + 4)) {
    if ((*(int *)(local_10 + 0xe8) == 0x1a) || (*(int *)(local_10 + 0xe8) == 0x1d)) {
      _ScanForOppositeZapperNode(local_10);
      _CreateBeamsForNode(local_10);
    }
    if (*(int *)(local_10 + 200) != 0) {
      (**(code **)(local_10 + 200))(local_10);
    }
  }
  SVar1 = _SWStickyError();
  if (SVar1 != 0) {
    SVar1 = _SWStickyError();
    _MMFatalErrorCallback
              (SVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/ZapperStuff.c",
               PTR____FUNCTION___27833_001c4ee0,0x1d6);
  }
  return;
}


// ---- _ScanForOppositeZapperNode @ 0x6ee42 (705 bytes)

void _ScanForOppositeZapperNode(SpritePtr param_1)

{
  short *psVar1;
  SpritePtr pSVar2;
  short sVar3;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SWMovementType SVar4;
  short local_18;
  short local_16;
  short local_14;
  short local_12;
  short local_10;
  
  psVar1 = *(short **)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + 0x10);
  local_12 = 0;
  local_14 = 0;
  local_10 = 0;
  if (param_1->userData == 0x1a) {
    local_12 = 1;
  }
  else if (param_1->userData == 0x1c) {
    local_12 = -1;
  }
  else if (param_1->userData == 0x1b) {
    local_14 = 1;
  }
  else if (param_1->userData == 0x1d) {
    local_14 = -1;
  }
  else {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/ZapperStuff.c",
               PTR____FUNCTION___27861_001c4ed4,499);
  }
  SVar4 = _SWGetSpriteVertLoc(param_1);
  _SWGetSpriteHeight(param_1);
  local_18 = (short)(int)((SVar4 + (float)extraout_ST0 / 2.0) /
                         (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4));
  SVar4 = _SWGetSpriteHorizLoc(param_1);
  _SWGetSpriteWidth(param_1);
  local_16 = (short)(int)((SVar4 + (float)extraout_ST0_00 / 2.0) /
                         (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe2));
  while( true ) {
    local_16 = local_16 + local_12;
    local_18 = local_18 + local_14;
    if (local_18 < 0) {
      return;
    }
    if (*psVar1 <= local_18) {
      return;
    }
    if (local_16 < 0) {
      return;
    }
    if (psVar1[1] <= local_16) break;
    pSVar2 = (SpritePtr)
             _FindSpriteAtTileRowAndCol
                       (*(undefined4 *)PTR__gZapperNodeSpriteLayerP_001c418c,0,(int)local_18,
                        (int)local_16);
    if ((pSVar2 != (SpritePtr)0x0) && (pSVar2 != param_1)) {
      sVar3 = (short)pSVar2->userData - (short)param_1->userData;
      if ((sVar3 == 2) || (sVar3 == -2)) {
        pSVar2->frameAdvance = param_1->frameAdvance;
        *(SpritePtr *)&param_1->f_10c = pSVar2;
        *(SpritePtr *)&pSVar2->f_10c = param_1;
        *(short *)&param_1->f_108 = local_10 + 1;
        *(short *)&pSVar2->f_108 = local_10 + 1;
      }
      return;
    }
    local_10 = local_10 + 1;
  }
  return;
}


// ---- _FindSpriteAtTileRowAndCol @ 0x6f103 (218 bytes)

SpritePtr _FindSpriteAtTileRowAndCol(int param_1,SpritePtr param_2,short param_3,short param_4)

{
  SWBoolean SVar1;
  SpritePtr local_28;
  SWRect local_18;
  SpritePtr local_10;
  
  local_18.left = *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe2) * param_4;
  local_18.right = local_18.left + *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe2);
  local_18.top = *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4) * param_3;
  local_18.bottom = local_18.top + *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4);
  if (param_2 == (SpritePtr)0x0) {
    local_28 = *(SpritePtr *)(param_1 + 0x10);
  }
  else {
    local_28 = param_2;
  }
  local_10 = local_28;
  while( true ) {
    if (local_10 == (SpritePtr)0x0) {
      return (SpritePtr)0x0;
    }
    SVar1 = _SWIsSpriteInRect(local_10,&local_18);
    if (SVar1 != '\0') break;
    local_10 = local_10->prevSpriteP;
  }
  return local_10;
}


// ---- _CreateBeamsForNode @ 0x6f1dd (884 bytes)

void _CreateBeamsForNode(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  SpritePtr srcSpriteP;
  int iVar4;
  char cVar5;
  short sVar6;
  SWMovementType SVar7;
  short local_18;
  short local_16;
  short local_10;
  short local_e;
  
  srcSpriteP = *(SpritePtr *)&param_1->f_10c;
  iVar4._0_1_ = param_1->f_10c;
  iVar4._1_1_ = param_1->f_10d;
  iVar4._2_1_ = param_1->f_10e;
  iVar4._3_1_ = param_1->f_10f;
  if (iVar4 != 0) {
    local_e = 0;
    local_10 = 0;
    SVar7 = _SWGetSpriteVertLoc(param_1);
    local_18 = (short)(int)(SVar7 / (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 +
                                                          0xe4));
    SVar7 = _SWGetSpriteHorizLoc(param_1);
    local_16 = (short)(int)(SVar7 / (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 +
                                                          0xe2));
    if (param_1->userData == 0x1a) {
      sVar6 = *(short *)&param_1->f_108 + local_16;
      while ((local_16 = local_16 + 1, local_16 <= sVar6 &&
             (cVar5 = _ZapperCanShootThroughTile((int)local_18,(int)local_16), cVar5 != '\0'))) {
        local_10 = local_10 + 1;
      }
    }
    else if (param_1->userData == 0x1d) {
      sVar6 = local_18 - *(short *)&param_1->f_108;
      while ((local_18 = local_18 + -1, sVar6 <= local_18 &&
             (cVar5 = _ZapperCanShootThroughTile((int)local_18,(int)local_16), cVar5 != '\0'))) {
        local_10 = local_10 + 1;
      }
    }
    else {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/ZapperStuff.c",
                 PTR____FUNCTION___27973_001c4edc,0x27e);
    }
    sVar6._0_1_ = param_1->f_108;
    sVar6._1_1_ = param_1->f_109;
    if (sVar6 != local_10) {
      SVar7 = _SWGetSpriteVertLoc(srcSpriteP);
      local_18 = (short)(int)(SVar7 / (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 +
                                                            0xe4));
      SVar7 = _SWGetSpriteHorizLoc(srcSpriteP);
      local_16 = (short)(int)(SVar7 / (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 +
                                                            0xe2));
      if (srcSpriteP->userData == 0x1c) {
        sVar6 = local_16 - *(short *)&srcSpriteP->f_108;
        while ((local_16 = local_16 + -1, sVar6 <= local_16 &&
               (cVar5 = _ZapperCanShootThroughTile((int)local_18,(int)local_16), cVar5 != '\0'))) {
          local_e = local_e + 1;
        }
      }
      else if (srcSpriteP->userData == 0x1b) {
        sVar6 = *(short *)&srcSpriteP->f_108 + local_18;
        while ((local_18 = local_18 + 1, local_18 <= sVar6 &&
               (cVar5 = _ZapperCanShootThroughTile((int)local_18,(int)local_16), cVar5 != '\0'))) {
          local_e = local_e + 1;
        }
      }
      else {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/ZapperStuff.c",
                   PTR____FUNCTION___27973_001c4edc,0x29d);
      }
    }
    sVar1._0_1_ = param_1->f_106;
    sVar1._1_1_ = param_1->f_107;
    if ((sVar1 != local_10) ||
       (sVar2._0_1_ = srcSpriteP->f_106, sVar2._1_1_ = srcSpriteP->f_107, sVar2 != local_e)) {
      _DeleteZapperSpritesForNode(param_1);
      sVar3._0_1_ = param_1->f_108;
      sVar3._1_1_ = param_1->f_109;
      if (sVar3 == local_10) {
        _CreateZapperSpritesForNode(param_1,(int)local_10);
      }
      param_1->f_106 = (undefined1)local_10;
      param_1->f_107 = local_10._1_1_;
      srcSpriteP->f_106 = (undefined1)local_e;
      srcSpriteP->f_107 = local_e._1_1_;
    }
  }
  return;
}


