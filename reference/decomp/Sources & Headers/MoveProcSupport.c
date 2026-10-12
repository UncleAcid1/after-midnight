// Ghidra decompilation of functions from original source file:
//   Sources & Headers/MoveProcSupport.c

// ---- _RemoveSpriteIfOutsideRoom @ 0x56afd (33 bytes)

void _RemoveSpriteIfOutsideRoom(undefined4 param_1)

{
  _RemoveSpriteIfOutsideRoomByMargin(param_1,200);
  return;
}


// ---- _RemoveSpriteIfOutsideRoomByMargin @ 0x56b1e (169 bytes)

void _RemoveSpriteIfOutsideRoomByMargin(SpritePtr param_1,short param_2)

{
  SWBoolean SVar1;
  SWRect local_1a;
  char local_11;
  int local_10;
  
  local_1a._4_4_ = *(undefined4 *)(PTR__gRoomMoveBoundsRect_001c4d5c + 4);
  local_1a._0_4_ = *(undefined4 *)PTR__gRoomMoveBoundsRect_001c4d5c;
  local_11 = '\x01';
  _SWInsetRect(&local_1a,(int)-param_2,(int)-param_2);
  SVar1 = _SWIsSpriteInRect(param_1,&local_1a);
  if (SVar1 == '\0') {
    local_10 = _GetHeroSprite();
    if ((local_10 != 0) && (*(SpritePtr *)(local_10 + 0x1c4) == param_1)) {
      local_11 = '\0';
    }
    if (local_11 != '\0') {
      _SWRemoveSpriteFromAnimation(param_1,'\x01');
    }
  }
  return;
}


// ---- _SetUpLadderArray @ 0x56bc7 (365 bytes)

void _SetUpLadderArray(void)

{
  short sVar1;
  short local_10;
  
  for (local_10 = 0; local_10 < 0x5a2; local_10 = local_10 + 1) {
    *(undefined2 *)(PTR__gLadderArray_001c4d4c + local_10 * 2) = 0;
  }
  local_10 = 0x20b;
  while (local_10 < 0x241) {
    sVar1 = local_10 + 2;
    for (; local_10 < sVar1; local_10 = local_10 + 1) {
      *(undefined2 *)(PTR__gLadderArray_001c4d4c + local_10 * 2) = 1;
    }
    sVar1 = local_10 + 8;
    for (; local_10 < sVar1; local_10 = local_10 + 1) {
      *(undefined2 *)(PTR__gLadderArray_001c4d4c + local_10 * 2) = 2;
    }
    sVar1 = local_10 + 2;
    for (; local_10 < sVar1; local_10 = local_10 + 1) {
      *(undefined2 *)(PTR__gLadderArray_001c4d4c + local_10 * 2) = 3;
    }
    sVar1 = local_10 + 2;
    for (; local_10 < sVar1; local_10 = local_10 + 1) {
      *(undefined2 *)(PTR__gLadderArray_001c4d4c + local_10 * 2) = 0;
    }
    sVar1 = local_10 + 2;
    for (; local_10 < sVar1; local_10 = local_10 + 1) {
      *(undefined2 *)(PTR__gLadderArray_001c4d4c + local_10 * 2) = 2;
    }
    sVar1 = local_10 + 2;
    for (; local_10 < sVar1; local_10 = local_10 + 1) {
      *(undefined2 *)(PTR__gLadderArray_001c4d4c + local_10 * 2) = 3;
    }
  }
  return;
}


// ---- _GetLadderValue @ 0x56d34 (74 bytes)

int _GetLadderValue(short param_1)

{
  int local_14;
  
  if ((param_1 < 0) || (0x5a1 < param_1)) {
    local_14 = 0;
  }
  else {
    local_14 = (int)*(short *)(PTR__gLadderArray_001c4d4c + param_1 * 2);
  }
  return local_14;
}


// ---- _ExitRoomIfAppropriate @ 0x56d7e (1188 bytes)

void _ExitRoomIfAppropriate(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined2 uVar8;
  undefined2 uVar9;
  char cVar10;
  short sVar11;
  short sVar12;
  uint uVar13;
  SWMovementType SVar14;
  SWMovementType SVar15;
  short local_3c;
  short sStack_3a;
  short local_38;
  short sStack_36;
  short local_34;
  short sStack_32;
  short local_30;
  short sStack_2e;
  short local_2c;
  short local_2a;
  SpritePtr local_28;
  short local_22;
  short local_20;
  short local_1e;
  short local_1c;
  short local_1a;
  short local_18;
  short local_16;
  short local_14;
  short local_12;
  short local_10;
  char local_e;
  char local_d;
  
  local_28 = param_1;
  local_12 = 0;
  local_10 = 0;
  local_e = '\0';
  local_d = '\0';
  if ((param_1->f_17a == '\0') &&
     (iVar5._0_1_ = param_1->f_1c4, iVar5._1_1_ = param_1->f_1c5, iVar5._2_1_ = param_1->f_1c6,
     iVar5._3_1_ = param_1->f_1c7, iVar5 == 0)) {
    local_1a = (param_1->destFrameRect).left - (param_1->oldFrameRect).left;
    local_18 = (param_1->destFrameRect).top - (param_1->oldFrameRect).top;
    uVar6 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x80);
    uVar7 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
    sStack_2e = (short)((uint)uVar6 >> 0x10);
    sStack_32 = (short)((uint)uVar7 >> 0x10);
    local_1e = sStack_2e - sStack_32;
    local_30 = (short)uVar6;
    local_34 = (short)uVar7;
    local_1c = local_30 - local_34;
    uVar3 = (param_1->destFrameRect).bottom;
    uVar8 = (param_1->destFrameRect).right;
    uVar4 = (param_1->destFrameRect).top;
    uVar9 = (param_1->destFrameRect).left;
    sVar1 = uVar9 + *(short *)&param_1->f_144;
    sVar2 = uVar4 + *(short *)&param_1->f_142;
    sVar11 = uVar8 - *(short *)&param_1->f_148;
    sVar12 = uVar3 - *(short *)&param_1->f_146;
    local_16 = sVar11 - sVar1;
    local_14 = sVar12 - sVar2;
    _GEGetRowAndColFromLevelNumber
              ((int)*(short *)PTR__gCurrentLevel_001c49bc,&local_2a,&local_2c,200);
    if (((int)sVar1 + (int)local_16 < (int)sStack_32) && (local_1a < 0)) {
      local_12 = local_16 + local_1e + 3;
      local_2c = local_2c + -1;
      local_e = '\x01';
    }
    else if (((int)sStack_2e < (int)sVar11 - (int)local_16) && (0 < local_1a)) {
      local_12 = -9 - (local_1e + local_16);
      local_2c = local_2c + 1;
      local_e = '\x01';
    }
    if (((((int)sVar2 + (int)local_14 < (int)local_34) && (local_18 < 0)) &&
        (uVar13 = (int)local_18 >> 0x1f,
        (int)((uVar13 ^ (int)local_18) - uVar13) < (int)(local_1c / 2))) &&
       (local_28->f_170 == '\0')) {
      local_10 = local_14 + local_1c;
      local_2a = local_2a + -1;
      local_e = '\x01';
      local_d = '\x01';
    }
    else if ((((int)local_30 < (int)sVar12 - (int)local_14) && (0 < local_18)) &&
            (uVar13 = (int)local_18 >> 0x1f,
            (int)((uVar13 ^ (int)local_18) - uVar13) < (int)(local_1c / 2))) {
      local_10 = -(local_14 + local_1c);
      local_2a = local_2a + 1;
      local_e = '\x01';
      local_d = '\x01';
    }
    if (local_e != '\0') {
      *(undefined2 *)PTR__gLastLevelExited_001c4d18 = *(undefined2 *)PTR__gCurrentLevel_001c49bc;
      local_22 = _GEGetLevelNumberFromRowAndCol((int)local_2a,(int)local_2c,200);
      local_20 = _GEGetLevelResID((int)local_22,1,200);
      cVar10 = _Resource1Exists("TMAP",(int)local_20);
      if (cVar10 != '\0') {
        _WriteRoomInfoToFile();
        if (local_d != '\0') {
          *(short *)&local_28->f_186 = *(short *)&local_28->f_186 + local_10 + 0x30;
        }
        _SWOffsetSprite(param_1,(float)(int)local_12,(float)(int)local_10);
        if (*PTR__gStartRecordingMovie_001c48d4 != '\0') {
          *(short *)PTR__gCurrentLevel_001c49bc = local_22;
          *(undefined4 *)PTR__gRunningFrameCount_001c481c = 0;
          _ResetFloorTileAnimations(0,1);
          SVar14 = _SWGetSpriteVertLoc(param_1);
          SVar15 = _SWGetSpriteHorizLoc(param_1);
          _OpenMovieFileForOutput(0,0,SVar15,SVar14);
        }
        _LoadLevel((int)local_22,0);
        _SetUpLevelForPlaying();
        _DrawLevel();
        if (*PTR__gStartRecordingMovie_001c48d4 != '\0') {
          PTR__gKeys_001c4af8[6] = 0;
          PTR__gKeys_001c4af8[5] = PTR__gKeys_001c4af8[6];
          PTR__gKeys_001c4af8[4] = PTR__gKeys_001c4af8[5];
          _WriteCurrentKeysStateToMovie();
          _SaveStartingPosition(param_1);
          *PTR__gStartRecordingMovie_001c48d4 = 0;
        }
        _DoGameFadeToNormal(0x1e);
      }
    }
  }
  return;
}


// ---- _SaveStartingPositionIfAppropriate @ 0x57222 (482 bytes)

void _SaveStartingPositionIfAppropriate(SpritePtr param_1,char param_2)

{
  SWBoolean SVar1;
  undefined *puVar2;
  char local_20;
  SWRect local_1c;
  SpritePtr local_14;
  undefined1 local_e;
  undefined1 local_d;
  
  local_20 = param_2;
  local_14 = param_1;
  if (param_2 != '\0') {
    local_e = *PTR__gSilverFloorIsActive_001c4958;
    local_d = *PTR__gGoldFloorIsActive_001c47a8;
    *PTR__gGoldFloorIsActive_001c47a8 = 0;
    *PTR__gSilverFloorIsActive_001c4958 = *PTR__gGoldFloorIsActive_001c47a8;
    local_20 = _CheckForWall(param_1,3,&param_1->f_142,0,1,0);
    *PTR__gSilverFloorIsActive_001c4958 = local_e;
    *PTR__gGoldFloorIsActive_001c47a8 = local_d;
  }
  local_1c._4_4_ = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x78);
  local_1c._0_4_ = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x74);
  _SWOffsetRect(&local_1c,(int)-local_1c.left,(int)-local_1c.top);
  puVar2 = (undefined *)_SWGetSpriteMoveProc(param_1);
  if ((puVar2 != PTR__ClimbingSpriteMoveProc_001c4d34) &&
     (puVar2 = (undefined *)_SWGetSpriteMoveProc(param_1),
     puVar2 != PTR__VineClimbingMoveProc_001c4d6c)) {
    local_1c.top = local_1c.top + -0x20;
  }
  if ((*(short *)PTR__gLastLevelPositionWasSaved_001c4c78 != *(short *)PTR__gCurrentLevel_001c49bc)
     || (*(short *)PTR__gLastLevelExited_001c4d18 != *(short *)PTR__gCurrentLevel_001c49bc)) {
    puVar2 = (undefined *)_SWGetSpriteMoveProc(param_1);
    if ((puVar2 != PTR__ClimbingSpriteMoveProc_001c4d34) &&
       (puVar2 = (undefined *)_SWGetSpriteMoveProc(param_1),
       puVar2 != PTR__VineClimbingMoveProc_001c4d6c)) {
      if (local_20 == '\0') {
        return;
      }
      if (local_14->f_170 != '\0') {
        return;
      }
      if (local_14->f_180 == '\0') {
        return;
      }
    }
    SVar1 = _SWIsSpriteFullyInRect(param_1,&local_1c);
    if (SVar1 != '\0') {
      _SaveStartingPosition(param_1);
    }
  }
  return;
}


// ---- _SaveStartingPosition @ 0x57404 (206 bytes)

void _SaveStartingPosition(SpritePtr param_1)

{
  undefined2 uVar1;
  SWMovementType SVar2;
  SWMovementType SVar3;
  
  if (param_1->userData != 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___27688_001c4d60,0x224);
  }
  SVar2 = _SWGetSpriteVertLoc(param_1);
  SVar3 = _SWGetSpriteHorizLoc(param_1);
  _SaveHeroStartingLocation((int)(short)(int)SVar3,(int)(short)(int)SVar2);
  *PTR__gStartingFacingRight_001c4c94 = param_1->f_177;
  uVar1 = _GetSpriteState(param_1);
  *(undefined2 *)PTR__gStartingSpriteState_001c4c68 = uVar1;
  *(undefined2 *)PTR__gStartingVineTileLayer_001c4cf4 = *(undefined2 *)&param_1->f_1b0;
  return;
}


// ---- _GetSpriteState @ 0x574d2 (176 bytes)

int _GetSpriteState(undefined4 param_1)

{
  undefined *puVar1;
  undefined2 local_e;
  
  puVar1 = (undefined *)_SWGetSpriteMoveProc(param_1);
  if (puVar1 == PTR__ClimbingSpriteMoveProc_001c4d34) {
    local_e = 1;
  }
  else {
    puVar1 = (undefined *)_SWGetSpriteMoveProc(param_1);
    if (puVar1 == PTR__VineClimbingMoveProc_001c4d6c) {
      local_e = 2;
    }
    else {
      puVar1 = (undefined *)_SWGetSpriteMoveProc(param_1);
      if (puVar1 == PTR__PoleSlidingMoveProc_001c4d44) {
        local_e = 3;
      }
      else {
        puVar1 = (undefined *)_SWGetSpriteMoveProc(param_1);
        if (puVar1 == PTR_LAB_001c4cbc) {
          local_e = *(short *)PTR__gStartingSpriteState_001c4c68;
        }
        else {
          local_e = 0;
        }
      }
    }
  }
  return (int)local_e;
}


// ---- _SaveHeroStartingLocation @ 0x57582 (154 bytes)

void _SaveHeroStartingLocation(undefined2 param_1,undefined2 param_2)

{
  *(undefined2 *)PTR__gLastLevelPositionWasSaved_001c4c78 =
       *(undefined2 *)PTR__gCurrentLevel_001c49bc;
  *(undefined2 *)PTR__gLastLevelExited_001c4d18 = *(undefined2 *)PTR__gCurrentLevel_001c49bc;
  *(undefined2 *)PTR__gLastMusicIndex_001c4c80 = *(undefined2 *)PTR__gCurrentMusicIndex_001c476c;
  *(undefined2 *)PTR__gStartingCol_001c4c74 = param_1;
  *(undefined2 *)PTR__gStartingRow_001c4c7c = param_2;
  *PTR__gStartingFacingRight_001c4c94 = 1;
  *(undefined2 *)PTR__gStartingSpriteState_001c4c68 = 0;
  return;
}


// ---- _MoveSpriteDownToFloor @ 0x5761c (199 bytes)

bool _MoveSpriteDownToFloor(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  short sVar7;
  char cVar8;
  
  sVar6 = (param_1->oldFrameRect).bottom;
  sVar7 = (param_1->oldFrameRect).right;
  sVar4 = (param_1->oldFrameRect).top;
  sVar5 = (param_1->oldFrameRect).left;
  sVar2 = (param_1->destFrameRect).bottom;
  sVar3 = (param_1->destFrameRect).right;
  sVar1 = (param_1->destFrameRect).left;
  (param_1->oldFrameRect).top = (param_1->destFrameRect).top;
  (param_1->oldFrameRect).left = sVar1;
  (param_1->oldFrameRect).bottom = sVar2;
  (param_1->oldFrameRect).right = sVar3;
  _SWOffsetSprite(param_1,0.0,1.3);
  cVar8 = _CheckForWall(param_1,3,&param_1->f_142,0,0,1);
  if (cVar8 != '\0') {
    param_1->drawData->vertMoveDelta = 0.0;
  }
  (param_1->oldFrameRect).top = sVar4;
  (param_1->oldFrameRect).left = sVar5;
  (param_1->oldFrameRect).bottom = sVar6;
  (param_1->oldFrameRect).right = sVar7;
  return cVar8 != '\0';
}


// ---- _DoHeroLandedFromJumpFrame @ 0x576e3 (236 bytes)

void _DoHeroLandedFromJumpFrame(SpritePtr param_1,char param_2)

{
  if (param_1->userData != 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___27751_001c4d0c,0x28a);
  }
  param_1->f_196 = '\b';
  param_1->f_197 = '\0';
  _SWSetSpriteFrameAdvance(param_1,0);
  _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_LAB_001c4d70);
  _SWSetSpriteFrameTime(param_1,0);
  param_1->f_172 = '\x01';
  if (param_2 != '\0') {
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4c90);
    param_1->f_16c = '\0';
    param_1->f_16d = '\0';
  }
  param_1->f_17e = '\x01';
  _SetHeroGroundFrame(param_1);
  return;
}


// ---- _SetHeroGroundFrame @ 0x577cf (625 bytes)

void _SetHeroGroundFrame(SpritePtr param_1)

{
  bool bVar1;
  SWMovementType SVar2;
  
  if (param_1->frameChangeProc != (FrameProcPtr_t)PTR_IdleHeroFrameProc_001c4cfc) {
    if (param_1->f_172 == '\0') {
      SVar2 = _SWGetSpriteHorizMoveDelta(param_1);
      if ((SVar2 != 0.0) || (NAN(SVar2))) {
        bVar1 = false;
        if ((param_1->curFrameIndex < 3) || (0xb < param_1->curFrameIndex)) {
          bVar1 = true;
        }
        _SWSetSpriteFrameRange(param_1,3,0xb);
        _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
        if (bVar1) {
          _SWSetSpriteFrameAdvanceMode(param_1,(void *)0x0);
          _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_HeroMiscFrameProc_001c41d0);
          _SWSetSpriteFrameAdvance(param_1,0);
          _SWSetSpriteFrameTime(param_1,0);
          param_1->f_0fc = '\x03';
          param_1->f_0fd = '\0';
          param_1->f_100 = '\x03';
          param_1->f_101 = '\0';
          param_1->f_0fe = '\x01';
          param_1->f_0ff = '\0';
        }
      }
      else if ((param_1->curFrameIndex < 0) || (2 < param_1->curFrameIndex)) {
        _SWSetSpriteFrameRange(param_1,0,2);
        _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
        _SWSetSpriteFrameAdvanceMode(param_1,(void *)0x1);
        _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_HeroMiscFrameProc_001c41d0);
        _SWSetSpriteFrameAdvance(param_1,0);
        _SWSetSpriteFrameTime(param_1,0);
        param_1->f_0fc = '\x0f';
        param_1->f_0fd = '\0';
        param_1->f_100 = '\x0f';
        param_1->f_101 = '\0';
        param_1->f_0fe = '\x01';
        param_1->f_0ff = '\0';
      }
    }
    else {
      _SWSetSpriteFrameRange(param_1,0xe,0xe);
      _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
    }
    _SaveLastOnGroundPixelRow(param_1);
  }
  return;
}


// ---- _SaveLastOnGroundPixelRow @ 0x57a40 (51 bytes)

void _SaveLastOnGroundPixelRow(SpritePtr param_1)

{
  SWMovementType SVar1;
  
  SVar1 = _SWGetSpriteVertLoc(param_1);
  param_1->f_186 = (char)(short)(int)SVar1;
  param_1->f_187 = (char)((ushort)(short)(int)SVar1 >> 8);
  return;
}


// ---- _SetHeroJumpingFrame @ 0x57a73 (202 bytes)

void _SetHeroJumpingFrame(SpritePtr param_1)

{
  SWMovementType SVar1;
  
  SVar1 = _SWGetSpriteHorizMoveDelta(param_1);
  if (SVar1 == 0.0) {
    _SWSetSpriteFrameRange(param_1,0xd,0xd);
    _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
  }
  else {
    _SWSetSpriteFrameRange(param_1,0xc,0xc);
    _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
  }
  return;
}


// ---- _ProcessJumpIntention @ 0x57b3d (398 bytes)

undefined1 _ProcessJumpIntention(int param_1,char param_2)

{
  undefined1 local_d;
  
  local_d = 0;
  if ((((PTR__gKeys_001c4af8[4] != '\0') || (PTR__gKeys_001c4af8[6] != '\0')) ||
      (PTR__gKeys_001c4af8[5] != '\0')) && (*(char *)(param_1 + 0x171) == '\0')) {
    *(undefined2 *)(param_1 + 0x19a) = 10;
    if (PTR__gKeys_001c4af8[5] == '\0') {
      if (PTR__gKeys_001c4af8[6] == '\0') {
        *(undefined2 *)(param_1 + 0x19c) = 1;
      }
      else {
        *(undefined2 *)(param_1 + 0x19c) = 2;
      }
    }
    else {
      *(undefined2 *)(param_1 + 0x19c) = 0;
    }
    if ((param_2 == '\0') || (*(char *)(param_1 + 0x16e) == '\0')) {
      PTR__gKeys_001c4af8[4] = 0;
      PTR__gKeys_001c4af8[6] = 0;
      PTR__gKeys_001c4af8[5] = 0;
    }
  }
  if (0 < *(short *)(param_1 + 0x19a)) {
    *(short *)(param_1 + 0x19a) = *(short *)(param_1 + 0x19a) + -1;
  }
  if (((0 < *(short *)(param_1 + 0x19a)) && (*(char *)(param_1 + 0x16e) != '\0')) &&
     (param_2 != '\0')) {
    if (*(short *)(param_1 + 0x19c) == 0) {
      PTR__gKeys_001c4af8[5] = 1;
    }
    else if (*(short *)(param_1 + 0x19c) == 2) {
      PTR__gKeys_001c4af8[6] = 1;
    }
    else {
      PTR__gKeys_001c4af8[4] = 1;
    }
    local_d = 1;
  }
  return local_d;
}


// ---- _StartJump @ 0x57ccb (610 bytes)

void _StartJump(SpritePtr param_1)

{
  SWMovementType SVar1;
  float local_14;
  short local_10;
  
  if (PTR__gKeys_001c4af8[5] == '\0') {
    if (PTR__gKeys_001c4af8[6] == '\0') {
      local_10 = (ushort)(byte)PTR__gKeys_001c4af8[1] - (ushort)(byte)PTR__gKeys_001c4af8[3];
    }
    else {
      local_10 = 1;
    }
  }
  else {
    local_10 = -1;
  }
  param_1->drawData->horizMoveDelta = (float)(int)local_10 * 2.5;
  SVar1 = _SWGetSpriteHorizMoveDelta(param_1);
  param_1->drawData->horizMoveDelta =
       *(float *)PTR__gHorizontalBoost_001c47d0 * param_1->drawData->horizMoveDelta;
  param_1->drawData->vertMoveDelta = -*(float *)PTR__gJumpSpeed_001c4708;
  param_1->f_170 = '\x01';
  param_1->f_16e = '\0';
  param_1->f_174 = '\0';
  param_1->f_175 = '\0';
  param_1->f_172 = '\0';
  param_1->f_178 = param_1->f_177;
  _MMPlaySound(2,9,1);
  if (SVar1 == 0.0) {
    local_14 = *(float *)&param_1->f_150;
  }
  else {
    local_14 = *(float *)&param_1->f_150 / 1.5;
  }
  *(float *)&param_1->f_15c = *(float *)&param_1->f_158 + local_14;
  param_1->drawData->horizMoveDelta = *(float *)&param_1->f_15c + param_1->drawData->horizMoveDelta;
  _SetHeroJumpingFrame(param_1);
  _SWSetSpriteFrameAdvance(param_1,0);
  _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
  *(short *)&param_1->f_182 = ((param_1->destFrameRect).bottom - *(short *)&param_1->f_146) + -1;
  _SaveLastOnGroundPixelRow(param_1);
  param_1->f_1c4 = '\0';
  param_1->f_1c5 = '\0';
  param_1->f_1c6 = '\0';
  param_1->f_1c7 = '\0';
  return;
}


// ---- _EndJump @ 0x57f2d (210 bytes)

undefined1 _EndJump(int param_1)

{
  undefined1 local_20;
  
  *(undefined1 *)(param_1 + 0x170) = 0;
  *(undefined1 *)(param_1 + 0x16e) = 1;
  PTR__gKeys_001c4af8[4] = 0;
  PTR__gKeys_001c4af8[5] = 0;
  PTR__gKeys_001c4af8[6] = 0;
  *(undefined2 *)(param_1 + 0x10a) = *(undefined2 *)(param_1 + 0x14a);
  *(undefined2 *)(param_1 + 0x102) = *(undefined2 *)(param_1 + 0x14c);
  if ((*(int *)(param_1 + 0x164) == 0) || (*(int *)(param_1 + 0x164) != *(int *)(param_1 + 0x1c4)))
  {
    local_20 = 0;
  }
  else {
    local_20 = 1;
  }
  *(undefined4 *)(param_1 + 0x1c4) = 0;
  *(undefined4 *)(param_1 + 0x164) = 0;
  return local_20;
}


// ---- _getHeroFallingDeathDistance @ 0x57fff (30 bytes)

undefined4 _getHeroFallingDeathDistance(void)

{
  return 7;
}


// ---- _HandleHeroLanding @ 0x5801d (454 bytes)

void _HandleHeroLanding(SpritePtr param_1,char param_2)

{
  bool bVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  SWMovementType SVar6;
  short local_18;
  
  SVar6 = _SWGetSpriteVertLoc(param_1);
  sVar3._0_1_ = param_1->f_186;
  sVar3._1_1_ = param_1->f_187;
  iVar4 = (int)(short)(int)(SVar6 - (float)(int)sVar3);
  iVar5 = iVar4 + 8;
  if (iVar5 < 0) {
    iVar5 = iVar4 + 0x17;
  }
  sVar3 = (short)(iVar5 >> 4);
  sVar2 = _getHeroFallingDeathDistance(param_1);
  param_1->f_160 = '\0';
  param_1->f_161 = '\0';
  param_1->f_162 = '\0';
  param_1->f_163 = '\0';
  if (sVar3 < sVar2) {
    if (param_2 != '\0') {
      bVar1 = sVar2 + -2 <= (int)sVar3;
      _DoHeroLandedFromJumpFrame(param_1,bVar1);
      if (bVar1) {
        *PTR__gHeroLandedThisFrame_001c4c70 = 0;
      }
      if ((int)sVar3 == sVar2 + -2) {
        param_1->f_196 = '\x0f';
        param_1->f_197 = '\0';
        param_1->f_19a = '\0';
        param_1->f_19b = '\0';
      }
      else if (sVar2 + -1 <= (int)sVar3) {
        param_1->f_196 = '\x18';
        param_1->f_197 = '\0';
        param_1->f_19a = '\0';
        param_1->f_19b = '\0';
      }
    }
    if (sVar2 + -4 < (int)sVar3) {
      if (sVar2 + -3 < (int)sVar3) {
        if (sVar2 + -2 < (int)sVar3) {
          local_18 = 0x1f;
        }
        else {
          local_18 = 0x1e;
        }
      }
      else {
        local_18 = 0x1d;
      }
    }
    else {
      local_18 = 3;
    }
    if ((local_18 == 3) && (sVar3 = _FindSound(3), sVar3 != -1)) {
      return;
    }
    _MMPlaySound(local_18,9,1);
  }
  else {
    _KillHeroForFallingTooFar(param_1);
    *PTR__gHeroLandedThisFrame_001c4c70 = 0;
  }
  return;
}


// ---- _HandleHeroQuestionMark @ 0x581e3 (202 bytes)

void HandleHeroQuestionMark(SpritePtr param_1)

{
  SpritePtr spriteP;
  SWMovementType SVar1;
  SWMovementType SVar2;
  
  spriteP = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gQuestionMarkSpriteLayerP_001c4938,
                             (SpritePtr)0x0);
  if (param_1->f_1d4 == '\0') {
    if (spriteP != (SpritePtr)0x0) {
      _SWRemoveSpriteFromAnimation(spriteP,'\x01');
    }
  }
  else if (spriteP != (SpritePtr)0x0) {
    SVar1 = _SWGetSpriteVertLoc(param_1);
    SVar2 = _SWGetSpriteHorizLoc(param_1);
    _SWMoveSprite(spriteP,SVar2 + 21.0,SVar1 - 29.0);
  }
  return;
}


// ---- _UpdateHaloSpritePosition @ 0x582ad (708 bytes)

void _UpdateHaloSpritePosition(undefined4 param_1)

{
  char cVar1;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  longdouble extraout_ST0_01;
  longdouble extraout_ST0_02;
  SWMovementType SVar2;
  SpritePtr local_14;
  float local_10;
  
  if ((*PTR__gUseHaloSprite_001c4700 != '\0') && (cVar1 = _IsCurRoomOutdoors(), cVar1 == '\0')) {
    local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gHaloSpriteLayerP_001c41e0,(SpritePtr)0x0);
    if ((local_14 == (SpritePtr)0x0) &&
       (local_14 = (SpritePtr)_NewHaloSprite(), local_14 != (SpritePtr)0x0)) {
      _SWCenterSpriteOverSprite(local_14,param_1);
    }
    if (local_14 != (SpritePtr)0x0) {
      local_10 = local_14->drawData->scaledWidthFactor;
      if (local_14->miscFloat < local_10) {
        local_10 = local_10 + local_10 / -9.0;
        if (local_10 < local_14->miscFloat) {
          local_10 = local_14->miscFloat;
        }
        _SWSetSpriteScaledFactor(local_14,local_10,local_10);
        _SWCenterSpriteOverSprite(local_14,param_1);
      }
      else {
        _CenterSpriteOverSpriteSmoothly(local_14,param_1);
      }
    }
    if (local_14 != (SpritePtr)0x0) {
      SVar2 = _SWGetSpriteVertLoc(local_14);
      if (0.0 < SVar2) {
        SVar2 = _SWGetSpriteHorizLoc(local_14);
        _SWMoveSprite(local_14,SVar2,0.0);
      }
      SVar2 = _SWGetSpriteVertLoc(local_14);
      _SWGetSpriteHeight(local_14);
      if (SVar2 + (float)extraout_ST0 < 480.0) {
        _SWGetSpriteHeight(local_14);
        SVar2 = _SWGetSpriteHorizLoc(local_14);
        _SWMoveSprite(local_14,SVar2,480.0 - (float)extraout_ST0_00);
      }
      SVar2 = _SWGetSpriteHorizLoc(local_14);
      if (0.0 < SVar2) {
        SVar2 = _SWGetSpriteVertLoc(local_14);
        _SWMoveSprite(local_14,0.0,SVar2);
      }
      SVar2 = _SWGetSpriteHorizLoc(local_14);
      _SWGetSpriteWidth(local_14);
      if (SVar2 + (float)extraout_ST0_01 < 640.0) {
        SVar2 = _SWGetSpriteVertLoc(local_14);
        _SWGetSpriteWidth(local_14);
        _SWMoveSprite(local_14,640.0 - (float)extraout_ST0_02,SVar2);
      }
    }
  }
  return;
}


// ---- _CenterSpriteOverSpriteSmoothly @ 0x58571 (472 bytes)

void _CenterSpriteOverSpriteSmoothly(SpritePtr param_1,SpritePtr param_2)

{
  int iVar1;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  longdouble extraout_ST0_01;
  longdouble extraout_ST0_02;
  SWMovementType SVar2;
  short local_28;
  short local_26;
  undefined2 local_24;
  undefined2 local_22;
  short local_20;
  short local_1e;
  undefined2 local_1c;
  undefined2 local_1a;
  float local_18;
  SWMovementType local_14;
  float local_10;
  
  local_20 = 0;
  local_1e = 0;
  _SWGetSpriteWidth(param_1);
  local_1a = (undefined2)(int)extraout_ST0;
  _SWGetSpriteHeight(param_1);
  local_1c = (undefined2)(int)extraout_ST0_00;
  SVar2 = _SWGetSpriteVertLoc(param_2);
  local_28 = (short)(int)SVar2;
  SVar2 = _SWGetSpriteHorizLoc(param_2);
  local_26 = (short)(int)SVar2;
  iVar1 = (int)local_26;
  _SWGetSpriteWidth(param_2);
  local_22 = (undefined2)(int)((float)iVar1 + (float)extraout_ST0_01);
  iVar1 = (int)local_28;
  _SWGetSpriteHeight(param_2);
  local_24 = (undefined2)(int)((float)iVar1 + (float)extraout_ST0_02);
  _SWCenterRect(&local_20,&local_28);
  iVar1 = (int)local_1e;
  SVar2 = _SWGetSpriteHorizLoc(param_1);
  local_18 = (float)iVar1 - SVar2;
  iVar1 = (int)local_20;
  SVar2 = _SWGetSpriteVertLoc(param_1);
  local_14 = (float)iVar1 - SVar2;
  local_10 = SQRT(local_14 * local_14 + local_18 * local_18);
  if (12.0 < local_10) {
    local_18 = local_18 / 15.0;
    local_14 = local_14 / 15.0;
  }
  _SWOffsetSprite(param_1,local_18,local_14);
  return;
}


// ---- _IsCurRoomOutdoors @ 0x58749 (41 bytes)

bool _IsCurRoomOutdoors(void)

{
  return *(short *)PTR__gOldBackgroundPictResID_001c47b0 != 1;
}


// ---- _GetHeroSprite @ 0x58772 (92 bytes)

SpritePtr _GetHeroSprite(void)

{
  undefined4 local_10;
  
  local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164,(SpritePtr)0x0);
  if (local_10 == (SpritePtr)0x0) {
    local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gHeroOnPlatformSpriteLayerP_001c491c,
                                (SpritePtr)0x0);
  }
  return local_10;
}


// ---- _LayWebForSpider @ 0x587ce (422 bytes)

void _LayWebForSpider(SpritePtr param_1,short param_2,short param_3,short param_4)

{
  short sVar1;
  short sVar2;
  short sVar3;
  SWMovementType SVar4;
  undefined2 local_1e;
  
  SVar4 = _SWGetSpriteHorizLoc(param_1);
  sVar1 = (short)(int)((SVar4 + (float)(int)param_2) / 16.0 + 1.0);
  SVar4 = _SWGetSpriteVertLoc(param_1);
  sVar2 = (short)(int)((SVar4 + (float)(int)param_3) / 16.0);
  local_1e = 5;
  do {
    if (local_1e < 4) {
LAB_000588ef:
      for (local_1e = 4; local_1e < 6; local_1e = local_1e + 1) {
        sVar3 = _MyGetTile(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_1e,
                           (int)(short)(sVar2 + 1),(int)sVar1);
        if (sVar3 == param_4) {
          _MyDrawTile(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_1e,
                      (int)(short)(sVar2 + 1),(int)sVar1,0xffffffff);
        }
      }
      return;
    }
    sVar3 = _MyGetTile(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_1e,(int)sVar2,
                       (int)sVar1);
    if (sVar3 == -1) {
      _MyDrawTile(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_1e,(int)sVar2,(int)sVar1,
                  (int)param_4);
      goto LAB_000588ef;
    }
    local_1e = local_1e + -1;
  } while( true );
}


// ---- _EraseWebForSpider @ 0x58974 (301 bytes)

void _EraseWebForSpider(SpritePtr param_1,short param_2,short param_3,short param_4)

{
  short sVar1;
  short sVar2;
  short sVar3;
  SWMovementType SVar4;
  undefined2 local_e;
  
  SVar4 = _SWGetSpriteHorizLoc(param_1);
  sVar1 = (short)(int)((SVar4 + (float)(int)param_2) / 16.0 + 1.0);
  SVar4 = _SWGetSpriteVertLoc(param_1);
  sVar2 = (short)(int)((SVar4 + (float)(int)param_3) / 16.0 + 1.0);
  for (local_e = 0; local_e < 7; local_e = local_e + 1) {
    sVar3 = _MyGetTile(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_e,(int)sVar2,(int)sVar1
                      );
    if (sVar3 == param_4) {
      _MyDrawTile(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_e,(int)sVar2,(int)sVar1,
                  0xffffffff);
    }
  }
  return;
}


// ---- _LayWebUpToCeiling @ 0x58aa1 (342 bytes)

void _LayWebUpToCeiling(SpritePtr param_1,short param_2,short param_3,short param_4)

{
  short sVar1;
  short sVar2;
  SWMovementType SVar3;
  undefined2 local_22;
  undefined2 local_20;
  
  SVar3 = _SWGetSpriteHorizLoc(param_1);
  sVar1 = (short)(int)((SVar3 + (float)(int)param_2) / 16.0 + 1.0);
  SVar3 = _SWGetSpriteVertLoc(param_1);
  local_20 = (short)(int)((SVar3 + (float)(int)param_3) / 16.0);
  do {
    if (local_20 < 0) {
      return;
    }
    for (local_22 = 5; -1 < local_22; local_22 = local_22 + -1) {
      sVar2 = _MyGetTile(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_22,(int)local_20,
                         (int)sVar1);
      if (sVar2 == -1) {
        _MyDrawTile(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_22,(int)local_20,
                    (int)sVar1,(int)param_4);
        break;
      }
      if (sVar2 == param_4) break;
    }
    local_20 = local_20 + -1;
  } while( true );
}


// ---- _DeleteWebForSmallSpider @ 0x58bf7 (320 bytes)

void _DeleteWebForSmallSpider(SpritePtr param_1)

{
  bool bVar1;
  short sVar2;
  short sVar3;
  SWMovementType SVar4;
  short local_12;
  short local_10;
  
  SVar4 = _SWGetSpriteHorizLoc(param_1);
  sVar2 = (short)(int)((SVar4 + 8.0) / 16.0 + 1.0);
  SVar4 = _SWGetSpriteVertLoc(param_1);
  local_12 = (short)(int)((SVar4 + 4.0) / 16.0);
  while( true ) {
    if (local_12 < 0) {
      return;
    }
    bVar1 = false;
    for (local_10 = 6; -1 < local_10; local_10 = local_10 + -1) {
      sVar3 = _MyGetTile(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,(int)local_12,
                         (int)sVar2);
      if (sVar3 == 0x5a0) {
        _MyDrawTile(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,(int)local_12,
                    (int)sVar2,0xffffffff);
        bVar1 = true;
      }
    }
    if (!bVar1) break;
    local_12 = local_12 + -1;
  }
  return;
}


// ---- _MovePreyToSpiderLocation @ 0x58d37 (131 bytes)

void _MovePreyToSpiderLocation(SpritePtr param_1,undefined4 param_2)

{
  _SWCenterSpriteOverSprite(param_1,param_2);
  if (param_1->userData == 0xc3) {
    _SWOffsetSprite(param_1,0.0,32.0);
  }
  else {
    _SWOffsetSprite(param_1,0.0,34.0);
  }
  return;
}


// ---- _CheckBigSpiderWithSprite @ 0x58dba (158 bytes)

undefined1 _CheckBigSpiderWithSprite(SpritePtr param_1,SpritePtr param_2,short param_3)

{
  short sVar1;
  longdouble extraout_ST0;
  SWMovementType SVar2;
  SWMovementType SVar3;
  
  sVar1 = _GetDistBetweenSpritesHorizMid(param_2,param_1);
  if (sVar1 < param_3) {
    SVar2 = _SWGetSpriteVertLoc(param_2);
    SVar3 = _SWGetSpriteVertLoc(param_1);
    _SWGetSpriteHeight(param_2);
    if ((float)extraout_ST0 / 2.0 + SVar3 < SVar2) {
      return 1;
    }
  }
  return 0;
}


// ---- _MakeSpriteDripBloodOrGoo @ 0x58e58 (365 bytes)

void _MakeSpriteDripBloodOrGoo(SpritePtr param_1,short param_2,uchar param_3,short param_4)

{
  short sVar1;
  int iVar2;
  SpritePtr srcSpriteP;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SWMovementType SVar3;
  short local_28;
  
  local_28 = param_4;
  if ((*PTR__gFamilyFriendly_001c46f0 == '\0') || (param_4 != 0xbc)) {
    if ((*PTR__gFamilyFriendly_001c46f0 == '\0') && (param_4 == 0xbe)) {
      local_28 = 0xbc;
    }
    SVar3 = _SWGetSpriteHorizLoc(param_1);
    _SWGetSpriteWidth(param_1);
    iVar2 = _GetRandom(1,(int)param_2);
    if (iVar2 < 2) {
      srcSpriteP = (SpritePtr)_NewBloodOrGooDripSprite((int)local_28);
      if (srcSpriteP != (SpritePtr)0x0) {
        _SWGetSpriteWidth(param_1);
        sVar1 = (short)(int)((float)extraout_ST0_00 / 4.0);
        sVar1 = _GetRandom(-(int)sVar1,(int)sVar1);
        _SWSetSpriteLocation
                  (srcSpriteP,
                   (float)(int)(short)(sVar1 + (short)(int)((float)extraout_ST0 / 2.0 + SVar3) + -8)
                   ,(float)(int)(short)((param_1->destFrameRect).bottom + -0xe));
      }
      srcSpriteP->f_105 = param_3;
    }
  }
  return;
}


// ---- _MakeSkeletonDoUncollapse @ 0x58fc5 (257 bytes)

void _MakeSkeletonDoUncollapse(SpritePtr param_1)

{
  if (param_1->userData != 0xd) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___28360_001c4d1c,0x5aa);
  }
  if (param_1->f_10b == '\0') {
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)0x0);
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_LAB_001c4d2c);
    _SWSetSpriteFrameRange(param_1,0,7);
    _SWSetSpriteFrameAdvance(param_1,0);
    _SWSetSpriteFrameTime(param_1,0);
    param_1->f_0fc = '\x06';
    param_1->f_0fd = '\0';
    param_1->f_0fe = '\x06';
    param_1->f_0ff = '\0';
    param_1->f_100 = '\x01';
    param_1->f_101 = '\0';
    param_1->f_10b = '\x01';
  }
  return;
}


// ---- _MakeZzzzFromJacksHead @ 0x590c6 (333 bytes)

void _MakeZzzzFromJacksHead(SpritePtr param_1)

{
  SpritePtr newSpriteP;
  SWMovementType SVar1;
  SWMovementType SVar2;
  
  newSpriteP = (SpritePtr)_NewMiscSprite(0xd2);
  if (newSpriteP != (SpritePtr)0x0) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,newSpriteP);
    _SWSetSpriteDrawProc(newSpriteP,*(DrawProcPtr_t *)PTR__gTranslucentMaskDrawProc_001c411c);
    _SWSetSpriteMoveProc(newSpriteP,(MoveProcPtr_t)PTR_LAB_001c4cf8);
    _SWSetSpriteFrameTime(newSpriteP,0x10);
    _SWSetSpriteFrameAdvance(newSpriteP,0);
    _SWSetSpriteFrameProc(newSpriteP,(FrameProcPtr_t)PTR_LAB_001c4d3c);
    _SWSetSpriteMoveDelta(newSpriteP,0.15,-0.17);
    _SWSetSpriteVisible(newSpriteP,'\0');
    SVar1 = _SWGetSpriteVertLoc(param_1);
    SVar2 = _SWGetSpriteHorizLoc(param_1);
    _SWSetSpriteLocation(newSpriteP,SVar2 + 30.0,SVar1 + 5.0);
  }
  return;
}


// ---- _DeleteZzzzSprites @ 0x59213 (131 bytes)

void _DeleteZzzzSprites(void)

{
  SpritePtr pSVar1;
  SpritePtr local_14;
  
  pSVar1 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,(SpritePtr)0x0);
  while (local_14 = pSVar1, local_14 != (SpritePtr)0x0) {
    pSVar1 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,local_14);
    if (local_14->userData == 0xd2) {
      _SWRemoveSpriteFromAnimation(local_14,'\x01');
    }
  }
  return;
}


// ---- _GetMovieCompatibleRandomFloat @ 0x59296 (54 bytes)

longdouble _GetMovieCompatibleRandomFloat(undefined4 param_1,undefined4 param_2)

{
  longdouble lVar1;
  
  lVar1 = (longdouble)_GetCustomRandomFloat(param_1,param_2);
  return (longdouble)(float)lVar1;
}


// ---- _GetMovieCompatibleRandom @ 0x592cc (49 bytes)

int GetMovieCompatibleRandom(short param_1,short param_2)

{
  short sVar1;
  
  sVar1 = _GetCustomRandom((int)param_1,(int)param_2);
  return (int)sVar1;
}


// ---- _OpenDoor @ 0x592fd (893 bytes)

void _OpenDoor(int param_1)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  
  bVar2 = true;
  bVar1 = false;
  iVar3 = _SWGetSpriteMoveProc(param_1);
  if (iVar3 == 0) {
    if ((*(int *)(param_1 + 0xe8) < 0x31) || (0x34 < *(int *)(param_1 + 0xe8))) {
      if ((*(int *)(param_1 + 0xe8) < 0x2d) || (0x30 < *(int *)(param_1 + 0xe8))) {
        if ((*(int *)(param_1 + 0xe8) < 0x35) || (0x38 < *(int *)(param_1 + 0xe8))) {
          if ((*(int *)(param_1 + 0xe8) < 0x3d) || (0x40 < *(int *)(param_1 + 0xe8))) {
            if ((*(int *)(param_1 + 0xe8) < 0x39) || (0x3c < *(int *)(param_1 + 0xe8))) {
              if ((*(int *)(param_1 + 0xe8) < 0x41) || (0x44 < *(int *)(param_1 + 0xe8))) {
                (**(code **)PTR__gSWAssertFailProc_001c4170)
                          ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c"
                           ,PTR____FUNCTION___28453_001c4d08,0x6ac);
              }
              else {
                bVar1 = true;
              }
            }
            else if (0 < *(short *)(PTR__gObjects_001c4910 + 6)) {
              _WriteCStringToFile("Opened purple door.\r");
              _RemoveItemFromStats(3);
              *(short *)(PTR__gObjects_001c4910 + 6) = *(short *)(PTR__gObjects_001c4910 + 6) + -1;
              bVar2 = false;
            }
          }
          else if (0 < *(short *)(PTR__gObjects_001c4910 + 8)) {
            _WriteCStringToFile("Opened yellow door.\r");
            _RemoveItemFromStats(4);
            *(short *)(PTR__gObjects_001c4910 + 8) = *(short *)(PTR__gObjects_001c4910 + 8) + -1;
            bVar2 = false;
          }
        }
        else if (0 < *(short *)(PTR__gObjects_001c4910 + 4)) {
          _WriteCStringToFile("Opened green door.\r");
          _RemoveItemFromStats(2);
          *(short *)(PTR__gObjects_001c4910 + 4) = *(short *)(PTR__gObjects_001c4910 + 4) + -1;
          bVar2 = false;
        }
      }
      else if (0 < *(short *)PTR__gObjects_001c4910) {
        _WriteCStringToFile("Opened red door.\r");
        _RemoveItemFromStats(0);
        *(short *)PTR__gObjects_001c4910 = *(short *)PTR__gObjects_001c4910 + -1;
        bVar2 = false;
      }
    }
    else if (0 < *(short *)(PTR__gObjects_001c4910 + 2)) {
      _WriteCStringToFile("Opened blue door.\r");
      _RemoveItemFromStats(1);
      *(short *)(PTR__gObjects_001c4910 + 2) = *(short *)(PTR__gObjects_001c4910 + 2) + -1;
      bVar2 = false;
    }
    if (!bVar1) {
      if (bVar2) {
        if (0 < *(short *)(PTR__gObjects_001c4910 + 10)) {
          _RemoveItemFromStats(5);
          _WriteCStringToFile("Used master key.\r");
          *(short *)(PTR__gObjects_001c4910 + 10) = *(short *)(PTR__gObjects_001c4910 + 10) + -1;
          _MakeDoorOpen(param_1,1);
          _MMPlayStereoSound(param_1,1,8,1);
        }
      }
      else {
        _MakeDoorOpen(param_1,1);
        _MMPlayStereoSound(param_1,1,8,1);
      }
    }
  }
  return;
}


// ---- _MakeDoorOpen @ 0x5967a (156 bytes)

void _MakeDoorOpen(SpritePtr param_1,char param_2)

{
  undefined4 local_10;
  
  if (param_2 != '\0') {
    _AddSpriteToObjectArray(param_1);
  }
  _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_DoorSpriteMoveProc_001c4d54);
  _SWSetSpriteDrawProc(param_1,(DrawProcPtr_t)PTR_DoorSpriteDrawProc_001c4044);
  if (param_2 == '\0') {
    local_10 = PTR_OpenDoorButDontAddCollideProc_001c4d48;
  }
  else {
    local_10 = PTR_LAB_001c4d24;
  }
  _SWCollideSpriteWithLayer(param_1,*(undefined4 *)PTR__gDoorSpriteLayerP_001c41dc,local_10);
  return;
}


// ---- _MakeDoorClose @ 0x59716 (229 bytes)

void _MakeDoorClose(SpritePtr param_1,char param_2)

{
  SpritePtr srcSpriteP;
  
  srcSpriteP = *(SpritePtr *)&param_1->f_124;
  if (param_2 != '\0') {
    _RemoveSpriteFromObjectArray(param_1);
    if (srcSpriteP != (SpritePtr)0x0) {
      _RemoveSpriteFromObjectArray(srcSpriteP);
    }
  }
  _InitializeDoorMovementVariables(param_1);
  _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4d64);
  _SWRemoveSprite(param_1);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,param_1);
  if (srcSpriteP != (SpritePtr)0x0) {
    _InitializeDoorMovementVariables(srcSpriteP);
    _SWSetSpriteMoveProc(srcSpriteP,(MoveProcPtr_t)PTR_LAB_001c4d64);
    _SWRemoveSprite(srcSpriteP);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,srcSpriteP);
  }
  return;
}


// ---- _OpenDoorCollideProc @ 0x597fb (125 bytes)

void OpenDoorCollideProc(int param_1,SpritePtr param_2)

{
  if (*(short *)(param_1 + 0x38) == (param_2->destFrameRect).left) {
    _SWSetSpriteMoveProc(param_2,(MoveProcPtr_t)PTR_DoorSpriteMoveProc_001c4d54);
    _SWSetSpriteDrawProc(param_2,(DrawProcPtr_t)PTR_DoorSpriteDrawProc_001c4044);
    _AddSpriteToObjectArray(param_2);
    *(SpritePtr *)(param_1 + 0x124) = param_2;
    param_2->f_124 = (undefined1)param_1;
    param_2->f_125 = param_1._1_1_;
    param_2->f_126 = param_1._2_1_;
    param_2->f_127 = param_1._3_1_;
  }
  return;
}


// ---- _OpenDoorInstantlyCollideProc @ 0x59878 (88 bytes)

void OpenDoorInstantlyCollideProc(int param_1,int param_2)

{
  if ((*(short *)(param_1 + 0x38) == *(short *)(param_2 + 0x38)) &&
     (*(char *)(param_2 + 0x104) == '\0')) {
    _SetDoorToOpenedState(param_2);
    *(int *)(param_1 + 0x124) = param_2;
    *(int *)(param_2 + 0x124) = param_1;
  }
  return;
}


// ---- _OpenDoorInstantlyAndAddCollideProc @ 0x598d0 (110 bytes)

void OpenDoorInstantlyAndAddCollideProc(int param_1,int param_2)

{
  if ((*(short *)(param_1 + 0x38) == *(short *)(param_2 + 0x38)) &&
     (*(char *)(param_2 + 0x104) == '\0')) {
    _SetDoorToOpenedState(param_2);
    _AddSpriteToObjectArray(param_2);
    _AddSpriteToDoOnceArray(param_2);
    *(int *)(param_1 + 0x124) = param_2;
    *(int *)(param_2 + 0x124) = param_1;
  }
  return;
}


// ---- _CloseDoorInstantlyCollideProc @ 0x5993e (88 bytes)

void CloseDoorInstantlyCollideProc(int param_1,int param_2)

{
  if ((*(short *)(param_1 + 0x38) == *(short *)(param_2 + 0x38)) &&
     (*(char *)(param_2 + 0x104) == '\x01')) {
    _SetDoorToClosedState(param_2);
    *(int *)(param_1 + 0x124) = param_2;
    *(int *)(param_2 + 0x124) = param_1;
  }
  return;
}


// ---- _OpenDoorButDontAddCollideProc @ 0x59996 (114 bytes)

void OpenDoorButDontAddCollideProc(int param_1,SpritePtr param_2)

{
  if (*(short *)(param_1 + 0x38) == (param_2->destFrameRect).left) {
    _SWSetSpriteMoveProc(param_2,(MoveProcPtr_t)PTR_DoorSpriteMoveProc_001c4d54);
    _SWSetSpriteDrawProc(param_2,(DrawProcPtr_t)PTR_DoorSpriteDrawProc_001c4044);
    *(SpritePtr *)(param_1 + 0x124) = param_2;
    param_2->f_124 = (undefined1)param_1;
    param_2->f_125 = param_1._1_1_;
    param_2->f_126 = param_1._2_1_;
    param_2->f_127 = param_1._3_1_;
  }
  return;
}


// ---- _MakeTrapDoorOpen @ 0x59a08 (114 bytes)

void _MakeTrapDoorOpen(SpritePtr param_1)

{
  _AddSpriteToObjectArray(param_1);
  _SWRemoveSprite(param_1);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gOpenTrapDoorSpriteLayerP_001c47dc,param_1);
  _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_TrapDoorFrameProc_001c4d40);
  _SWSetSpriteFrameTime(param_1,100);
  return;
}


// ---- _OpenTrapDoorCollideProc @ 0x59a7a (44 bytes)

void OpenTrapDoorCollideProc(int param_1,int param_2)

{
  if (*(short *)(param_1 + 0x36) == *(short *)(param_2 + 0x36)) {
    _MakeTrapDoorOpen(param_2);
  }
  return;
}


// ---- _OpenTrapDoorInstantlyCollideProc @ 0x59aa6 (105 bytes)

void OpenTrapDoorInstantlyCollideProc(int param_1,SpritePtr param_2)

{
  if (*(short *)(param_1 + 0x36) == (param_2->destFrameRect).top) {
    _SWSetCurrentFrameIndex(param_2,*(int *)(param_1 + 0x84) + -1);
    _SWRemoveSprite(param_2);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gOpenTrapDoorSpriteLayerP_001c47dc,param_2);
  }
  return;
}


// ---- _SetDoorToOpenedState @ 0x59b0f (340 bytes)

void _SetDoorToOpenedState(SpritePtr param_1)

{
  if (param_1->userData < 0x2d) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___28651_001c4d14,0x7d9);
  }
  if (0x44 < param_1->userData) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___28651_001c4d14,0x7da);
  }
  if (param_1->f_105 == '\0') {
    (param_1->destFrameRect).top = (param_1->destFrameRect).top + *(short *)&param_1->f_102;
    *(short *)&param_1->f_0fe = *(short *)&param_1->f_0fe + *(short *)&param_1->f_102;
  }
  else {
    (param_1->destFrameRect).bottom = (param_1->destFrameRect).bottom - *(short *)&param_1->f_102;
    *(short *)&param_1->f_0fc = *(short *)&param_1->f_0fc - *(short *)&param_1->f_102;
  }
  _SWRemoveSprite(param_1);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gOpenedDoorSpriteLayerP_001c49a4,param_1);
  _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)0x0);
  param_1->f_104 = '\x01';
  return;
}


// ---- _SetDoorToClosedState @ 0x59c63 (351 bytes)

void _SetDoorToClosedState(SpritePtr param_1)

{
  if (param_1->userData < 0x2d) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___28681_001c4d68,0x7fe);
  }
  if (0x44 < param_1->userData) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___28681_001c4d68,0x7ff);
  }
  _InitializeDoorMovementVariables(param_1);
  if (param_1->f_105 == '\0') {
    (param_1->destFrameRect).top = (param_1->destFrameRect).top - *(short *)&param_1->f_102;
    *(short *)&param_1->f_0fe = *(short *)&param_1->f_0fe - *(short *)&param_1->f_102;
  }
  else {
    (param_1->destFrameRect).bottom = (param_1->destFrameRect).bottom + *(short *)&param_1->f_102;
    *(short *)&param_1->f_0fc = *(short *)&param_1->f_0fc + *(short *)&param_1->f_102;
  }
  _SWRemoveSprite(param_1);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,param_1);
  _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)0x0);
  param_1->f_104 = '\0';
  return;
}


// ---- _SetTrapDoorToOpenedState @ 0x59dc2 (135 bytes)

void _SetTrapDoorToOpenedState(SpritePtr param_1,char param_2)

{
  if (param_2 != '\0') {
    _SWCollideSpriteWithLayer
              (param_1,*(undefined4 *)PTR__gTrapDoorSpriteLayerP_001c40e0,
               PTR_OpenTrapDoorInstantlyCollideProc_001c4d20);
  }
  _SWSetCurrentFrameIndex(param_1,param_1->maxFrames + -1);
  _SWRemoveSprite(param_1);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gOpenTrapDoorSpriteLayerP_001c47dc,param_1);
  return;
}


// ---- _SaveHeroOnPedestal @ 0x59e49 (435 bytes)

void _SaveHeroOnPedestal(SpritePtr param_1,SpritePtr param_2)

{
  char cVar1;
  short sVar2;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SWMovementType SVar3;
  SWMovementType SVar4;
  
  if ((*PTR__gUsingEditor_001c48b0 == '\0') && (param_2->f_104 != '\0')) {
    cVar1 = _IsSpriteInObjectArray(param_2);
    if (cVar1 == '\0') {
      cVar1 = _IsSpriteInDoOnceArray(param_2);
      if (cVar1 == '\0') {
        SVar3 = _SWGetSpriteHorizLoc(param_1);
        _SWGetSpriteWidth(param_1);
        SVar4 = _SWGetSpriteHorizLoc(param_2);
        _SWGetSpriteWidth(param_2);
        sVar2 = (short)(int)((float)extraout_ST0 / 2.0 + SVar3) -
                (short)(int)((float)extraout_ST0_00 / 2.0 + SVar4);
        if ((-6 < sVar2) && (sVar2 < 6)) {
          SVar3 = _SWGetSpriteHorizLoc(param_1);
          SVar4 = _SWGetSpriteVertLoc(param_1);
          *(undefined4 *)PTR__gLastSavePedestalObjectID_001c49f8 = *(undefined4 *)&param_2->f_0f8;
          _CreateColorAndGrayStatues
                    ((int)(short)(int)(SVar3 - (float)(int)sVar2),(int)(short)(int)SVar4,1);
          _SWSetSpriteVisible(param_1,'\0');
          _MMPauseGameTimer();
          _PauseSpriteLayersForSaveAnim();
          _AddSpriteToObjectArray(param_2);
          param_2->f_104 = '\0';
        }
      }
    }
  }
  return;
}


// ---- _StopLoopingSounds @ 0x59ffc (98 bytes)

void _StopLoopingSounds(void)

{
  _StopChannel(1);
  _StopChannel(0);
  _StopChannel(3);
  _StopChannel(4);
  _StopChannel(2);
  _StopChannel(5);
  _StopChannel(6);
  return;
}


// ---- _CreateColorAndGrayStatues @ 0x5a05e (571 bytes)

void _CreateColorAndGrayStatues(short param_1,short param_2,char param_3)

{
  int iVar1;
  undefined4 uVar2;
  SpritePtr srcSpriteP;
  SpritePtr srcSpriteP_00;
  SpritePtr local_14;
  ushort local_10;
  ushort local_e;
  
  local_e = (ushort)(param_3 == '\0');
  local_10 = (ushort)(param_3 != '\0');
  srcSpriteP = (SpritePtr)_NewStatueSprite();
  if (srcSpriteP != (SpritePtr)0x0) {
    _SWSetSpriteLocation(srcSpriteP,(float)(int)param_1,(float)(int)param_2);
    _SWSetCurrentFrameIndex(srcSpriteP,(int)(short)local_10);
  }
  srcSpriteP_00 = (SpritePtr)_NewStatueSprite();
  if (srcSpriteP_00 != (SpritePtr)0x0) {
    _SWSetSpriteLocation(srcSpriteP_00,(float)(int)param_1,(float)(int)param_2);
    _SWSetCurrentFrameIndex(srcSpriteP_00,(int)(short)local_e);
    _SWSetSpriteFrameAdvance(srcSpriteP_00,0);
    _SWSetSpriteDrawProc(srcSpriteP_00,*(DrawProcPtr_t *)PTR__gTranslucentMaskDrawProc_001c411c);
    _UpdateHaloSpritePosition(srcSpriteP_00);
    _SWSetSpriteFrameProc(srcSpriteP_00,(FrameProcPtr_t)PTR_LAB_001c4d28);
    _SWSetSpriteFrameTime(srcSpriteP_00,0);
  }
  *(SpritePtr *)&srcSpriteP->f_110 = srcSpriteP_00;
  *(SpritePtr *)&srcSpriteP_00->f_110 = srcSpriteP;
  local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gSavePedestalSpriteLayerP_001c4138,
                              (SpritePtr)0x0);
  while( true ) {
    if (local_14 == (SpritePtr)0x0) {
      return;
    }
    iVar1._0_1_ = local_14->f_0f8;
    iVar1._1_1_ = local_14->f_0f9;
    iVar1._2_1_ = local_14->f_0fa;
    iVar1._3_1_ = local_14->f_0fb;
    if (iVar1 == *(int *)PTR__gLastSavePedestalObjectID_001c49f8) break;
    _WriteCStringToFile(
                       "ERROR! gLastSavePedestalObjectID NOT found in current room! Most likely ObjectIDs were reset, or the pedestal was deleted and recreated, in this mansion, since this saved game was created. An assertion error will be caused this this.\r"
                       );
    _WriteCStringToFile("gLastSavePedestalObjectID: ");
    _WriteNumberToFile(*(undefined4 *)PTR__gLastSavePedestalObjectID_001c49f8);
    _WriteCarriageReturn(1);
    _WriteCStringToFile("((BackgroundStructPtr)pedestalSpriteP)->objectID: ");
    uVar2._0_1_ = local_14->f_0f8;
    uVar2._1_1_ = local_14->f_0f9;
    uVar2._2_1_ = local_14->f_0fa;
    uVar2._3_1_ = local_14->f_0fb;
    _WriteNumberToFile(uVar2);
    _WriteCarriageReturn(1);
    local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gSavePedestalSpriteLayerP_001c4138,local_14)
    ;
  }
  *(SpritePtr *)&srcSpriteP->f_114 = local_14;
  *(SpritePtr *)&srcSpriteP_00->f_114 = local_14;
  return;
}


// ---- _PauseSpriteLayersForSaveAnim @ 0x5a299 (108 bytes)

void _PauseSpriteLayersForSaveAnim(void)

{
  *PTR__gLayersArePausedForSaveAnim_001c487c = 1;
  _PauseAllSpriteLayers(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  _SWUnpauseSpriteLayer(*(SpriteLayerPtr *)PTR__gStatueSpriteLayerP_001c4240);
  _SWUnpauseSpriteLayer(*(SpriteLayerPtr *)PTR__gBottomCircleOfLightSpriteLayerP_001c4290);
  _SWUnpauseSpriteLayer(*(SpriteLayerPtr *)PTR__gTopCircleOfLightSpriteLayerP_001c4014);
  return;
}


// ---- _GetLadderRect @ 0x5a305 (1534 bytes)

char _GetLadderRect(int param_1,int param_2,short *param_3,undefined4 *param_4,short param_5,
                   short param_6,short param_7)

{
  undefined4 uVar1;
  int iVar2;
  short local_5c;
  short local_5a;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  short local_30;
  short local_2e;
  short local_2c;
  short local_2a;
  short local_28;
  short local_26;
  short local_24;
  short local_22;
  short *local_20;
  char local_1b;
  short local_1a;
  char local_17;
  
  local_1b = '\0';
  if ((param_1 == 0) || (*(char *)(param_1 + 200) == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___28837_001c4d10,0x948);
  }
  if (*(int *)(*(int *)(param_1 + 0xc4) + param_5 * 4) == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___28837_001c4d10,0x949);
  }
  if (param_2 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___28837_001c4d10,0x94a);
  }
  local_20 = *(short **)(*(int *)(param_1 + 0xc4) + param_5 * 4);
  uVar1 = *(undefined4 *)(param_2 + 0x3a);
  iVar2 = *(int *)(param_2 + 0x36);
  local_38 = iVar2;
  local_34 = uVar1;
  if (param_3 != (short *)0x0) {
    local_38._2_2_ = (short)((uint)iVar2 >> 0x10);
    local_38._0_2_ = (short)iVar2;
    local_38 = CONCAT22(local_38._2_2_ + param_3[1],(short)local_38 + *param_3);
    local_34._2_2_ = (short)((uint)uVar1 >> 0x10);
    local_34._0_2_ = (short)uVar1;
    local_34 = CONCAT22(local_34._2_2_ - param_3[3],(short)local_34 - param_3[2]);
  }
  if (local_38 < 0) {
    local_38 = CONCAT22(local_38._2_2_ - *(short *)(param_1 + 0xe2),(short)local_38);
  }
  if (local_34._2_2_ < 1) {
    local_34 = CONCAT22(local_34._2_2_ - *(short *)(param_1 + 0xe2),(short)local_34);
  }
  if ((short)local_38 < 0) {
    local_38 = CONCAT22(local_38._2_2_,(short)local_38 - *(short *)(param_1 + 0xe4));
  }
  if ((short)local_34 < 1) {
    local_34 = CONCAT22(local_34._2_2_,(short)local_34 - *(short *)(param_1 + 0xe4));
  }
  local_2c = (short)local_38 / *(short *)(param_1 + 0xe4);
  local_2a = (short)(((short)local_34 + -1) / (int)*(short *)(param_1 + 0xe4));
  local_28 = local_38._2_2_ / *(short *)(param_1 + 0xe2);
  local_26 = (short)((local_34._2_2_ + -1) / (int)*(short *)(param_1 + 0xe2));
  if (local_2a < 0) {
    local_1b = '\0';
  }
  else {
    if (*local_20 <= local_2a) {
      local_2a = *local_20 + -1;
    }
    if (local_2c < 0) {
      local_2c = 0;
    }
    else if (*local_20 <= local_2c) {
      return '\0';
    }
    if (local_26 < 0) {
      local_1b = '\0';
    }
    else {
      if (local_20[1] <= local_26) {
        local_26 = local_20[1] + -1;
      }
      if (local_28 < 0) {
        local_28 = 0;
      }
      else if (local_20[1] <= local_28) {
        return '\0';
      }
      local_24 = (short)(((int)local_2a - (int)local_2c) / 2) + local_2c;
      local_22 = (short)(((int)local_26 - (int)local_28) / 2) + local_28;
      local_1a = _GetTileID(local_20,(int)local_24,(int)local_22);
      if ((local_1a < param_6) || (param_7 < local_1a)) {
        for (local_30 = local_2c; local_30 <= local_2a; local_30 = local_30 + 1) {
          for (local_2e = local_28; local_2e <= local_26; local_2e = local_2e + 1) {
            local_1a = _GetTileID(local_20,(int)local_30,(int)local_2e);
            if ((param_6 <= local_1a) && (local_1a <= param_7)) {
              local_1b = '\x01';
              goto LAB_0005a714;
            }
          }
        }
LAB_0005a714:
        if (local_1b != '\0') {
          _GetLadderRectFromTile(param_1,param_4,&local_38,(int)local_1a,(int)local_2e);
        }
        if (local_1b != '\0') {
          local_17 = '\0';
          for (local_30 = local_2c; local_30 <= local_2a; local_30 = local_30 + 1) {
            for (local_2e = local_26; local_28 <= local_2e; local_2e = local_2e + -1) {
              local_1a = _GetTileID(local_20,(int)local_30,(int)local_2e);
              if ((param_6 <= local_1a) && (local_1a <= param_7)) {
                local_17 = '\x01';
                goto LAB_0005a7be;
              }
            }
          }
LAB_0005a7be:
          if (local_17 != '\0') {
            _GetLadderRectFromTile(param_1,&local_40,&local_38,(int)local_1a,(int)local_2e);
            local_38._2_2_ =
                 local_38._2_2_ + (short)(((int)local_34._2_2_ - (int)local_38._2_2_) / 2);
            local_5c = *(short *)((int)param_4 + 2) +
                       (short)(((int)*(short *)((int)param_4 + 6) -
                               (int)*(short *)((int)param_4 + 2)) / 2);
            local_5a = local_40._2_2_ + (short)(((int)local_3c._2_2_ - (int)local_40._2_2_) / 2);
            if ((int)local_5c - (int)local_38._2_2_ < 0) {
              local_5c = local_38._2_2_ - local_5c;
            }
            else {
              local_5c = local_5c - local_38._2_2_;
            }
            if ((int)local_5a - (int)local_38._2_2_ < 0) {
              local_5a = local_38._2_2_ - local_5a;
            }
            else {
              local_5a = local_5a - local_38._2_2_;
            }
            if (local_5a < local_5c) {
              *param_4 = local_40;
              param_4[1] = local_3c;
            }
          }
        }
      }
      else {
        _GetLadderRectFromTile(param_1,param_4,&local_38,(int)local_1a,(int)local_22);
        local_1b = '\x01';
      }
    }
  }
  return local_1b;
}


// ---- _GetLadderRectFromTile @ 0x5a903 (185 bytes)

void _GetLadderRectFromTile
               (int param_1,undefined4 *param_2,undefined2 *param_3,short param_4,short param_5)

{
  short sVar1;
  short sVar2;
  undefined4 local_16;
  undefined4 local_12;
  
  if ((param_4 - 0x1f9U & 1) == 0) {
    sVar1 = *(short *)(param_1 + 0xe2) * param_5;
    sVar2 = *(short *)(param_1 + 0xe2) * (param_5 + 2);
  }
  else {
    sVar1 = *(short *)(param_1 + 0xe2) * (param_5 + -1);
    sVar2 = *(short *)(param_1 + 0xe2) * (param_5 + 1);
  }
  local_12 = CONCAT22(sVar2,param_3[2]);
  local_16 = CONCAT22(sVar1,*param_3);
  *param_2 = local_16;
  param_2[1] = local_12;
  return;
}


// ---- _GetPoleRect @ 0x5a9bc (1167 bytes)

char _GetPoleRect(int param_1,int param_2,short *param_3,short *param_4,short param_5,short param_6,
                 short param_7)

{
  short *psVar1;
  undefined4 uVar2;
  int iVar3;
  short sVar4;
  short sVar5;
  undefined4 local_2c;
  undefined4 local_28;
  short local_24;
  short local_22;
  short local_20;
  short local_1e;
  short local_1c;
  short local_1a;
  char local_f;
  
  local_f = '\0';
  if ((param_1 == 0) || (*(char *)(param_1 + 200) == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___29106_001c4d58,0xa17);
  }
  if (*(int *)(*(int *)(param_1 + 0xc4) + param_5 * 4) == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___29106_001c4d58,0xa18);
  }
  if (param_2 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___29106_001c4d58,0xa19);
  }
  psVar1 = *(short **)(*(int *)(param_1 + 0xc4) + param_5 * 4);
  uVar2 = *(undefined4 *)(param_2 + 0x3a);
  iVar3 = *(int *)(param_2 + 0x36);
  local_2c = iVar3;
  local_28 = uVar2;
  if (param_3 != (short *)0x0) {
    local_2c._2_2_ = (short)((uint)iVar3 >> 0x10);
    local_2c._0_2_ = (short)iVar3;
    local_2c = CONCAT22(local_2c._2_2_ + param_3[1],(short)local_2c + *param_3);
    local_28._2_2_ = (short)((uint)uVar2 >> 0x10);
    local_28._0_2_ = (short)uVar2;
    local_28 = CONCAT22(local_28._2_2_ - param_3[3],(short)local_28 - param_3[2]);
  }
  if (local_2c < 0) {
    local_2c = CONCAT22(local_2c._2_2_ - *(short *)(param_1 + 0xe2),(short)local_2c);
  }
  if (local_28._2_2_ < 1) {
    local_28 = CONCAT22(local_28._2_2_ - *(short *)(param_1 + 0xe2),(short)local_28);
  }
  if ((short)local_2c < 0) {
    local_2c = CONCAT22(local_2c._2_2_,(short)local_2c - *(short *)(param_1 + 0xe4));
  }
  if ((short)local_28 < 1) {
    local_28 = CONCAT22(local_28._2_2_,(short)local_28 - *(short *)(param_1 + 0xe4));
  }
  local_20 = (short)local_2c / *(short *)(param_1 + 0xe4);
  local_1e = (short)(((short)local_28 + -1) / (int)*(short *)(param_1 + 0xe4));
  local_1c = local_2c._2_2_ / *(short *)(param_1 + 0xe2);
  local_1a = (short)((local_28._2_2_ + -1) / (int)*(short *)(param_1 + 0xe2));
  if (local_1e < 0) {
    local_f = '\0';
  }
  else {
    if (*psVar1 <= local_1e) {
      local_1e = *psVar1 + -1;
    }
    if (local_20 < 0) {
      local_20 = 0;
    }
    else if (*psVar1 <= local_20) {
      return '\0';
    }
    if (local_1a < 0) {
      local_f = '\0';
    }
    else {
      if (psVar1[1] <= local_1a) {
        local_1a = psVar1[1] + -1;
      }
      if (local_1c < 0) {
        local_1c = 0;
      }
      else if (psVar1[1] <= local_1c) {
        return '\0';
      }
      sVar4 = (short)(((int)local_1a - (int)local_1c) / 2) + local_1c;
      sVar5 = _GetTileID(psVar1,(int)(short)((short)(((int)local_1e - (int)local_20) / 2) + local_20
                                            ),(int)sVar4);
      if ((sVar5 < param_6) || (param_7 < sVar5)) {
        for (local_24 = local_20; local_24 <= local_1e; local_24 = local_24 + 1) {
          for (local_22 = local_1c; local_22 <= local_1a; local_22 = local_22 + 1) {
            sVar4 = _GetTileID(psVar1,(int)local_24,(int)local_22);
            if ((param_6 <= sVar4) && (sVar4 <= param_7)) {
              local_f = '\x01';
              goto LAB_0005adea;
            }
          }
        }
LAB_0005adea:
        if (local_f != '\0') {
          *param_4 = (short)local_2c;
          param_4[2] = (short)local_28;
          param_4[1] = *(short *)(param_1 + 0xe2) * local_22;
          param_4[3] = *(short *)(param_1 + 0xe2) * (local_22 + 1);
        }
      }
      else {
        *param_4 = (short)local_2c;
        param_4[2] = (short)local_28;
        param_4[1] = *(short *)(param_1 + 0xe2) * sVar4;
        param_4[3] = *(short *)(param_1 + 0xe2) * (sVar4 + 1);
        local_f = '\x01';
      }
    }
  }
  return local_f;
}


// ---- _IsTileFloorTile @ 0x5ae4b (146 bytes)

undefined4 _IsTileFloorTile(short param_1,char param_2)

{
  char cVar1;
  undefined4 local_18;
  
  cVar1 = _IsTileHorizWall((int)param_1);
  if (cVar1 == '\0') {
    if ((((param_2 == '\0') || (*(short *)PTR__gDisappearingFloorMode_001c48a8 == 3)) ||
        (param_1 < 0x2c1)) || (0x2c3 < param_1)) {
      if ((param_1 < 0x1fb) || (0x1fc < param_1)) {
        local_18 = 0;
      }
      else {
        local_18 = 1;
      }
    }
    else {
      local_18 = 1;
    }
  }
  else {
    local_18 = 1;
  }
  return local_18;
}


// ---- _IsTileFloorNotVineTile @ 0x5aedd (109 bytes)

undefined4 _IsTileFloorNotVineTile(short param_1)

{
  char cVar1;
  undefined4 local_14;
  
  cVar1 = _IsTileHorizWall((int)param_1);
  if (cVar1 == '\0') {
    if (((*(short *)PTR__gDisappearingFloorMode_001c48a8 == 3) || (param_1 < 0x2c1)) ||
       (0x2c3 < param_1)) {
      local_14 = 0;
    }
    else {
      local_14 = 1;
    }
  }
  else {
    local_14 = 1;
  }
  return local_14;
}


// ---- _IsTileHorizWall @ 0x5af4a (215 bytes)

undefined4 _IsTileHorizWall(short param_1)

{
  short sVar1;
  undefined4 local_14;
  
  sVar1 = _ConvertTileID((int)param_1);
  if ((sVar1 < 1) || (0x1a2 < sVar1)) {
    if ((*PTR__gSilverFloorIsActive_001c4958 == '\0') || ((sVar1 < 0x1af || (0x1b2 < sVar1)))) {
      if ((*PTR__gGoldFloorIsActive_001c47a8 == '\0') || ((sVar1 < 0x1bd || (0x1c0 < sVar1)))) {
        if ((sVar1 < 0x241) || (0x243 < sVar1)) {
          if ((sVar1 < 0x249) || (0x24b < sVar1)) {
            local_14 = 0;
          }
          else {
            local_14 = 1;
          }
        }
        else {
          local_14 = 1;
        }
      }
      else {
        local_14 = 1;
      }
    }
    else {
      local_14 = 1;
    }
  }
  else {
    local_14 = 1;
  }
  return local_14;
}


// ---- _IsTileThickHorizWall @ 0x5b021 (192 bytes)

char _IsTileThickHorizWall(undefined4 param_1,short param_2,short param_3)

{
  char cVar1;
  short sVar2;
  short sVar3;
  char local_d;
  
  sVar2 = _GetTileID(param_1,(int)param_2,(int)param_3);
  local_d = _IsTileHorizWall((int)sVar2);
  if (local_d != '\0') {
    sVar2 = _GetTileID(param_1,(int)(short)(param_2 + -1),(int)param_3);
    sVar3 = _GetTileID(param_1,(int)(short)(param_2 + 1),(int)param_3);
    cVar1 = _IsTileHorizWall((int)sVar2);
    if ((cVar1 == '\0') && (cVar1 = _IsTileHorizWall((int)sVar3), cVar1 == '\0')) {
      return '\0';
    }
    local_d = '\x01';
  }
  return local_d;
}


// ---- _CheckForWall @ 0x5b0e1 (843 bytes)

undefined1
_CheckForWall(int param_1,undefined4 param_2,short *param_3,undefined1 param_4,char param_5,
             undefined1 param_6)

{
  undefined4 uVar1;
  char cVar2;
  short sVar3;
  undefined1 local_4c;
  undefined4 local_36;
  undefined4 local_32;
  undefined4 local_2e;
  undefined4 local_2a;
  undefined4 local_26;
  undefined4 local_22;
  short local_1e;
  char local_1c;
  char local_1b;
  char local_1a;
  char local_19;
  int local_18;
  short local_14;
  short local_12;
  short local_10;
  short local_e;
  
  if (param_3 == (short *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___29391_001c4d30,0xaf4);
  }
  local_22 = *(undefined4 *)(param_3 + 2);
  local_36 = *(undefined4 *)param_3;
  local_26 = local_36;
  if (*(int *)(param_1 + 0xe8) == 0) {
    local_18 = param_1;
    local_1b = *(int *)(param_1 + 0x1c4) != 0;
    local_2a = *(undefined4 *)(param_1 + 0x106);
    local_2e = *(undefined4 *)(param_1 + 0x102);
    local_32 = *(undefined4 *)(param_1 + 0x10e);
    uVar1 = *(undefined4 *)(param_1 + 0x10a);
    if (*(int *)(param_1 + 0x94) == 0x31) {
      local_26 = CONCAT22((short)((uint)local_36 >> 0x10),0x28);
      local_2e._2_2_ = (short)((uint)local_2e >> 0x10);
      local_2e = CONCAT22(local_2e._2_2_,0x28);
      local_36._2_2_ = (short)((uint)*(undefined4 *)(param_1 + 0x10a) >> 0x10);
      local_36 = CONCAT22(local_36._2_2_,0x28);
      uVar1 = local_36;
    }
    local_36 = uVar1;
    cVar2 = _SWEqualRect(param_3,param_1 + 0x142);
    if (cVar2 == '\0') {
      local_14 = param_3[1] - *(short *)(local_18 + 0x144);
      local_e = *param_3 - *(short *)(local_18 + 0x142);
      local_12 = param_3[3] - *(short *)(local_18 + 0x148);
      local_10 = param_3[2] - *(short *)(local_18 + 0x146);
      local_2e = CONCAT22(local_2e._2_2_ + local_14,(short)local_2e + local_e);
      local_2a = CONCAT22(local_2a._2_2_ + local_12,(short)local_2a + local_10);
      local_36 = CONCAT22(local_36._2_2_ + local_14,(short)local_36 + local_e);
      local_32 = CONCAT22(local_32._2_2_ + local_12,(short)local_32 + local_10);
    }
  }
  else {
    local_1b = '\0';
    local_32 = local_22;
    local_2e = local_36;
    local_2a = local_22;
  }
  local_1e = *(short *)(param_1 + 0x3a) - (short)local_2a;
  sVar3 = local_1e;
  if (local_1e < 0) {
    sVar3 = local_1e + 0xf;
  }
  local_1c = (int)(sVar3 >> 4) << 4 == (int)local_1e;
  if (param_5 != '\0') {
    local_22 = CONCAT22(local_22._2_2_,(short)local_22 + -1);
    local_2a = CONCAT22(local_2a._2_2_,(short)local_2a + -1);
    local_32 = CONCAT22(local_32._2_2_,(short)local_32 + -1);
  }
  local_1a = '\0';
  local_19 = 0;
  if (local_1b == '\0') {
    if (((bool)local_1c) || (param_5 == '\0')) {
      local_1a = _CheckForWallWithTiles(param_1,param_2,param_6,param_4,&local_26,&local_2e);
    }
    local_19 = _CheckForWallWithSprites(param_1,param_2,param_6,&local_2e,&local_36);
  }
  else {
    local_19 = _CheckForWallWithSprites(param_1,param_2,param_6,&local_2e,&local_36);
    if ((local_1c != '\0') || (param_5 == '\0')) {
      local_1a = _CheckForWallWithTiles(param_1,param_2,param_6,param_4,&local_26,&local_2e);
    }
  }
  if ((local_1a == '\0') && (local_19 == '\0')) {
    local_4c = 0;
  }
  else {
    local_4c = 1;
  }
  return local_4c;
}


// ---- _CheckForWallWithTiles @ 0x5b42c (1000 bytes)

undefined1
_CheckForWallWithTiles
          (int param_1,int param_2,undefined1 param_3,undefined4 param_4,undefined4 *param_5,
          undefined4 param_6)

{
  char cVar1;
  char local_3c;
  char local_38;
  undefined2 local_1e;
  short sStack_1c;
  undefined2 local_1a;
  short sStack_18;
  char local_15;
  char local_14;
  char local_13;
  char local_12;
  char local_11;
  char local_10;
  char local_f;
  char local_e;
  undefined1 local_d;
  
  local_e = '\0';
  local_f = '\0';
  local_10 = '\0';
  local_11 = '\0';
  local_12 = '\0';
  local_13 = 0;
  local_14 = 0;
  local_15 = 0;
  local_d = 0;
  local_15 = _MyCheckSpriteWithWallTiles
                       (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1,param_2,param_6,4,4,1,
                        0x1a2,param_3);
  if (*PTR__gSilverFloorIsActive_001c4958 != '\0') {
    local_f = _MyCheckSpriteWithWallTiles
                        (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1,param_2,param_6,4,4,
                         0x1af,0x1b2,param_3);
  }
  if (*PTR__gGoldFloorIsActive_001c47a8 != '\0') {
    local_e = _MyCheckSpriteWithWallTiles
                        (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1,param_2,param_6,4,4,
                         0x1bd,0x1c0,param_3);
  }
  local_14 = _MyCheckSpriteWithWallTiles
                       (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1,param_2,param_6,4,4,0x241
                        ,0x243,param_3);
  local_13 = _MyCheckSpriteWithWallTiles
                       (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1,param_2,param_6,4,4,0x249
                        ,0x24b,param_3);
  if ((*(int *)(param_1 + 0xe8) == 0) && ((local_14 != '\0' || (local_13 != '\0')))) {
    *PTR__gHeroIsOnConveyorBelt_001c49ec = 1;
  }
  if (param_2 != 3) goto LAB_0005b7d6;
  sStack_18 = (short)((uint)param_5[1] >> 0x10);
  _local_1a = CONCAT22(sStack_18 + 4,(short)param_5[1]);
  sStack_1c = (short)((uint)*param_5 >> 0x10);
  _local_1e = CONCAT22(sStack_1c + 10,(short)*param_5);
  if (*(short *)PTR__gDisappearingFloorMode_001c48a8 == 3) {
LAB_0005b6fb:
    local_3c = '\0';
  }
  else {
    cVar1 = _MyCheckSpriteWithWallTiles
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1,3,param_5,4,4,0x2c1,0x2c3,
                       param_3);
    if (cVar1 == '\0') goto LAB_0005b6fb;
    local_3c = '\x01';
  }
  local_12 = local_3c;
  if (*(short *)PTR__gLightSaberFloorMode_001c4984 == 3) {
LAB_0005b776:
    local_38 = '\0';
  }
  else {
    cVar1 = _MyCheckSpriteWithWallTiles
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1,3,param_5,4,4,0x2e9,0x2eb,
                       param_3);
    if (cVar1 == '\0') goto LAB_0005b776;
    local_38 = '\x01';
  }
  local_11 = local_38;
  local_10 = _MyCheckSpriteWithWallTiles
                       (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1,3,&local_1e,4,5,0x1fb,
                        0x1fc,param_3);
LAB_0005b7d6:
  if (((((local_15 != '\0') || (local_14 != '\0')) || (local_13 != '\0')) ||
      ((local_12 != '\0' || (local_11 != '\0')))) ||
     ((local_10 != '\0' || ((local_f != '\0' || (local_e != '\0')))))) {
    local_d = 1;
  }
  return local_d;
}


// ---- _CheckForWallWithSprites @ 0x5b814 (1232 bytes)

undefined1
_CheckForWallWithSprites
          (int param_1,int param_2,undefined1 param_3,undefined4 param_4,undefined4 param_5)

{
  char cVar1;
  int iVar2;
  SpritePtr local_34;
  undefined4 local_30;
  char local_2c;
  char local_2b;
  char local_2a;
  char local_29;
  undefined1 local_28;
  char local_27;
  char local_26;
  undefined1 local_25;
  int local_24;
  short local_1e;
  SpritePtr local_1c;
  int local_18;
  int local_14;
  
  local_25 = 0;
  local_26 = '\0';
  local_27 = '\0';
  local_28 = 0;
  local_29 = '\0';
  local_2a = '\0';
  local_2b = '\0';
  local_2c = 0;
  local_2c = _SWCheckSpriteWithSprites
                       (param_1,0,param_2,param_5,*(undefined4 *)PTR__gDoorSpriteLayerP_001c41dc,
                        *(undefined4 *)PTR__gDoorSpriteLayerP_001c41dc,0,1,0,param_3);
  if ((((param_2 == 3) && (*(int *)(param_1 + 0xe8) != 3)) && (*(int *)(param_1 + 0xe8) != 0xb)) &&
     (*(int *)(param_1 + 0xe8) != 10)) {
    local_2b = _SWCheckSpriteWithSprites
                         (param_1,&local_34,3,param_4,
                          *(undefined4 *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,
                          *(undefined4 *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,0,1,0,param_3
                         );
    if (local_2b == '\0') {
      local_2b = _SWCheckSpriteWithSprites
                           (param_1,&local_34,3,param_4,
                            *(undefined4 *)PTR__gFallingPlatformSpriteLayerP_001c499c,
                            *(undefined4 *)PTR__gFallingPlatformSpriteLayerP_001c499c,0,1,0,param_3)
      ;
    }
    if (((local_2b != '\0') && (*(int *)(param_1 + 0xe8) == 0)) &&
       (local_24 = param_1, *(char *)(param_1 + 0x17a) == '\0')) {
      *(SpritePtr *)(param_1 + 0x1c4) = local_34;
      local_1e = (short)local_34->userData;
      if ((0x4f < local_1e) && (local_1e < 0x5f)) {
        iVar2 = _SWGetSpriteMoveProc(local_34);
        if (iVar2 == 0) {
          local_1c = local_34;
          local_34->f_10a = '\x1e';
          local_34->f_10b = '\0';
          _SWSetSpriteMoveProc(local_34,(MoveProcPtr_t)PTR_LAB_001c4d38);
          if ((0x5b < local_1e) && (local_1e < 0x5f)) {
            _SWSetSpriteFrameProc(local_34,(FrameProcPtr_t)PTR_LAB_001c4d00);
            _SWSetSpriteFrameAdvance(local_34,1);
            _SWSetSpriteFrameTime(local_34,100);
          }
        }
      }
    }
    local_27 = _SWCheckSpriteWithSprites
                         (param_1,&local_34,3,param_4,
                          *(undefined4 *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c,
                          *(undefined4 *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c,0,1,0,
                          param_3);
    if ((local_27 != '\0') && (*(int *)(param_1 + 0xe8) == 0)) {
      local_18 = param_1;
      *(SpritePtr *)(param_1 + 0x1c4) = local_34;
    }
    if (((*(int *)(param_1 + 0xe8) == 0) && (local_2b == '\0')) && (local_27 == '\0')) {
      local_14 = param_1;
      *(undefined4 *)(param_1 + 0x1c4) = 0;
    }
  }
  if (param_2 == 3) {
    local_2a = _SWCheckSpriteWithSprites
                         (param_1,0,3,param_4,*(undefined4 *)PTR__gTrapDoorSpriteLayerP_001c40e0,
                          *(undefined4 *)PTR__gTrapDoorSpriteLayerP_001c40e0,0,1,0,param_3);
    local_29 = _SWCheckSpriteWithSprites
                         (param_1,0,3,param_4,*(undefined4 *)PTR__gBrickSwitchSpriteLayerP_001c4188,
                          *(undefined4 *)PTR__gBrickSwitchSpriteLayerP_001c4188,0,1,0,param_3);
  }
  cVar1 = _SWCheckSpriteWithSprites
                    (param_1,&local_30,param_2,param_4,
                     *(undefined4 *)PTR__gSavePedestalSpriteLayerP_001c4138,
                     *(undefined4 *)PTR__gSavePedestalSpriteLayerP_001c4138,0,1,0,param_3);
  if ((((((local_2c != '\0') || (local_2b != '\0')) ||
        ((local_2a != '\0' || ((local_29 != '\0' || (cVar1 != '\0')))))) || (local_27 != '\0')) ||
      (local_26 != '\0')) &&
     (((local_25 = 1, cVar1 != '\0' && (param_2 == 3)) && (*(int *)(param_1 + 0xe8) == 0)))) {
    *(undefined4 *)(param_1 + 0x1c0) = local_30;
  }
  return local_25;
}


// ---- _MyCheckSpriteWithWallTiles @ 0x5bce4 (4213 bytes)

char _MyCheckSpriteWithWallTiles
               (int param_1,SpritePtr param_2,int param_3,short *param_4,short param_5,short param_6
               ,short param_7,short param_8,char param_9)

{
  short sVar1;
  char cVar2;
  uint uVar3;
  short local_86;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  short local_50;
  short local_4e;
  short local_4c;
  short local_4a;
  short local_48;
  short local_46;
  short local_44;
  short local_42;
  int local_40;
  char local_3b;
  char local_3a;
  char local_39;
  char local_38;
  char local_37;
  short local_36;
  short local_34;
  short local_32;
  short local_30;
  short local_2e;
  char local_2b;
  short local_2a;
  SpritePtr local_28;
  char local_21;
  short local_20;
  short local_1e;
  
  if ((param_1 == 0) || (*(char *)(param_1 + 200) == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___29721_001c4d04,0xc96);
  }
  if ((param_5 < 0) || (9 < param_6)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___29721_001c4d04,0xc97);
  }
  if (param_2 == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___29721_001c4d04,0xc98);
  }
  local_3b = '\0';
  local_3a = '\x01';
  local_54._0_2_ = (param_2->oldFrameRect).bottom;
  local_54._2_2_ = (param_2->oldFrameRect).right;
  local_58._0_2_ = (param_2->oldFrameRect).top;
  local_58._2_2_ = (param_2->oldFrameRect).left;
  local_5c._0_2_ = (param_2->destFrameRect).bottom;
  local_5c._2_2_ = (param_2->destFrameRect).right;
  local_60._0_2_ = (param_2->destFrameRect).top;
  local_60._2_2_ = (param_2->destFrameRect).left;
  if (param_3 == 2) {
    local_42 = -1;
  }
  else if (param_3 == 4) {
    local_42 = 1;
  }
  else {
    local_42 = 0;
  }
  if (param_4 != (short *)0x0) {
    local_58 = CONCAT22(local_58._2_2_ + param_4[1],(short)local_58 + *param_4);
    local_54 = CONCAT22(local_54._2_2_ - param_4[3],(short)local_54 - param_4[2]);
    local_60 = CONCAT22(local_60._2_2_ + param_4[1],(short)local_60 + *param_4);
    local_5c = CONCAT22(local_5c._2_2_ - param_4[3],(short)local_5c - param_4[2]);
  }
  if (param_3 == 1) {
    local_4a = (short)local_58 / *(short *)(param_1 + 0xe4);
    local_4c = (short)local_60 / *(short *)(param_1 + 0xe4);
    local_4e = local_60._2_2_ / *(short *)(param_1 + 0xe2);
    local_50 = (short)((local_5c._2_2_ + -1) / (int)*(short *)(param_1 + 0xe2));
    if (param_9 != '\0') {
      local_4a = local_4a + -1;
    }
    if (local_4a < local_4c) {
      local_3a = '\0';
      local_4a = local_4c;
    }
  }
  else if (param_3 == 3) {
    local_4a = (short)(((short)local_54 + -1) / (int)*(short *)(param_1 + 0xe4));
    local_4c = (short)(((short)local_5c + -1) / (int)*(short *)(param_1 + 0xe4));
    local_4e = local_60._2_2_ / *(short *)(param_1 + 0xe2);
    local_50 = (short)((local_5c._2_2_ + -1) / (int)*(short *)(param_1 + 0xe2));
    if (param_9 != '\0') {
      local_4a = local_4a + 1;
    }
    if (local_4c < local_4a) {
      local_3a = '\0';
      local_4a = local_4c;
    }
  }
  else if (param_3 == 2) {
    local_4e = (short)((local_54._2_2_ + -1) / (int)*(short *)(param_1 + 0xe2));
    local_50 = (short)((local_5c._2_2_ + -1) / (int)*(short *)(param_1 + 0xe2));
    local_4a = (short)local_60 / *(short *)(param_1 + 0xe4);
    local_4c = (short)(((short)local_5c + -1) / (int)*(short *)(param_1 + 0xe4));
    if (param_9 != '\0') {
      local_4e = local_4e + 1;
    }
    if (local_50 < local_4e) {
      local_3a = '\0';
      local_4e = local_50;
    }
  }
  else if (param_3 == 4) {
    local_4e = local_58._2_2_ / *(short *)(param_1 + 0xe2);
    local_50 = local_60._2_2_ / *(short *)(param_1 + 0xe2);
    local_4a = (short)local_60 / *(short *)(param_1 + 0xe4);
    local_4c = (short)(((short)local_5c + -1) / (int)*(short *)(param_1 + 0xe4));
    if (param_9 != '\0') {
      local_4e = local_4e + -1;
    }
    if (local_4e < local_50) {
      local_3a = '\0';
      local_4e = local_50;
    }
  }
  else {
    local_4a = (short)local_60 / *(short *)(param_1 + 0xe4);
    local_4c = (short)(((short)local_5c + -1) / (int)*(short *)(param_1 + 0xe4));
    local_4e = local_60._2_2_ / *(short *)(param_1 + 0xe2);
    local_50 = (short)((local_5c._2_2_ + -1) / (int)*(short *)(param_1 + 0xe2));
  }
  if (local_4c < local_4a) {
    local_30 = -1;
  }
  else {
    local_30 = 1;
  }
  local_32 = param_5;
  if (local_50 < local_4e) {
    local_2e = -1;
  }
  else {
    local_2e = 1;
  }
  while ((local_32 <= param_6 && (*(int *)(*(int *)(param_1 + 0xc4) + local_32 * 4) == 0))) {
    local_32 = local_32 + 1;
  }
  if (local_3a != '\0') {
    local_32 = param_5;
    if (((param_3 == 1) || (param_3 == 3)) || (param_3 == 0)) {
      for (; local_32 <= param_6; local_32 = local_32 + 1) {
        if (*(int *)(*(int *)(param_1 + 0xc4) + local_32 * 4) != 0) {
          local_40 = *(int *)(*(int *)(param_1 + 0xc4) + local_32 * 4);
          local_39 = '\x01';
          local_48 = local_4a;
          while (local_39 != '\0') {
            local_39 = local_48 != local_4c;
            local_38 = '\x01';
            local_46 = local_4e;
            while (local_38 != '\0') {
              local_38 = local_46 != local_50;
              local_34 = _GetTileID(local_40,(int)local_48,(int)local_46);
              if ((param_7 <= local_34) && (local_34 <= param_8)) {
                if (param_3 == 3) {
                  local_44 = -1;
                }
                else if (param_3 == 1) {
                  local_44 = 1;
                }
                else {
                  local_44 = 0;
                }
                local_34 = _GetTileID(local_40,(int)(short)(local_44 + local_48),(int)local_46);
                local_37 = _IsTileFloorTile((int)local_34,0);
                if (((local_37 == '\0') && (local_32 == 5)) &&
                   ((0x1fa < param_7 && (param_8 < 0x1fd)))) {
                  local_34 = _GetTileID(*(undefined4 *)(*(int *)(param_1 + 0xc4) + 0x10),
                                        (int)(short)(local_44 + local_48),(int)local_46);
                  local_37 = _IsTileFloorTile((int)local_34,0);
                }
                if (local_37 == '\0') {
                  local_3b = '\x01';
                  goto LAB_0005c660;
                }
              }
              local_46 = local_46 + local_2e;
            }
            local_48 = local_48 + local_30;
          }
        }
      }
    }
    else {
      for (; local_32 <= param_6; local_32 = local_32 + 1) {
        if (*(int *)(*(int *)(param_1 + 0xc4) + local_32 * 4) != 0) {
          local_40 = *(int *)(*(int *)(param_1 + 0xc4) + local_32 * 4);
          local_38 = '\x01';
          local_46 = local_4e;
          while (local_38 != '\0') {
            local_38 = local_46 != local_50;
            local_39 = '\x01';
            local_48 = local_4a;
            while (local_39 != '\0') {
              local_2b = '\0';
              local_2a = (param_2->destFrameRect).bottom - (param_2->oldFrameRect).bottom;
              local_39 = local_48 != local_4c;
              local_34 = _GetTileID(local_40,(int)local_48,(int)local_46);
              if (param_2->userData == 0) {
                cVar2 = _IsTileThickHorizWall(local_40,(int)local_48,(int)local_46);
                if (cVar2 == '\0') {
                  local_86 = local_4c;
                  if (local_4c < local_4a) {
                    local_86 = local_4a;
                  }
                  if ((local_86 == local_48) &&
                     (((uVar3 = (int)local_2a >> 0x1f, 5 < (int)(((int)local_2a ^ uVar3) - uVar3) ||
                       (local_2a == 0)) || (param_2->f_170 == '\0')))) {
                    local_2b = '\x01';
                  }
                }
                else {
                  local_2b = '\x01';
                }
              }
              else {
                local_2b = '\x01';
              }
              if (((param_7 <= local_34) && (local_34 <= param_8)) && (local_2b == '\x01')) {
                local_34 = _GetTileID(local_40,(int)local_48,(int)(short)(local_42 + local_46));
                local_37 = _IsTileHorizWall((int)local_34);
                if (local_37 == '\0') {
                  local_3b = '\x01';
                  goto LAB_0005c660;
                }
              }
              local_48 = local_48 + local_30;
            }
            local_46 = local_46 + local_2e;
          }
        }
      }
    }
  }
LAB_0005c660:
  if (param_2->userData == 0) {
    local_28 = param_2;
    local_21 = '\x01';
    if ((param_3 == 2) || (param_3 == 4)) {
      local_40 = *(int *)(*(int *)(param_1 + 0xc4) + param_5 * 4);
      if (local_40 == 0) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c"
                   ,PTR____FUNCTION___29721_001c4d04,0xdb0);
      }
      if (local_4c < local_4a) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c"
                   ,PTR____FUNCTION___29721_001c4d04,0xdb2);
      }
      local_38 = '\x01';
      local_20 = local_4e;
      while (local_38 != '\0') {
        local_38 = local_20 != local_50;
        local_34 = _GetTileID(local_40,(int)local_4c,(int)local_20);
        if (((local_34 < 1) || (0x1a2 < local_34)) ||
           (cVar2 = _IsTileThickHorizWall(local_40,(int)local_4c,(int)local_20), cVar2 == '\0')) {
          local_21 = '\0';
        }
        local_20 = local_20 + local_2e;
      }
      if (local_21 == '\0') {
        local_21 = '\x01';
        local_38 = '\x01';
        local_20 = local_4e;
        while (local_38 != '\0') {
          local_38 = local_20 != local_50;
          local_34 = _GetTileID(local_40,(int)(short)(local_4c + -1),(int)local_20);
          if (((local_34 < 1) || (0x1a2 < local_34)) ||
             (cVar2 = _IsTileThickHorizWall(local_40,(int)local_4c,(int)local_20), cVar2 == '\0')) {
            local_21 = '\0';
          }
          local_20 = local_20 + local_2e;
        }
      }
    }
    if (local_4c < local_4a) {
      _SwapVariables(&local_4a,&local_4c);
    }
    if (local_50 < local_4e) {
      _SwapVariables(&local_4e,&local_50);
    }
    local_4a = local_4c + 1;
    local_4c = *(short *)&local_28->f_182;
    if (local_4c < 0) {
      local_4c = local_4c + 0xf;
    }
    local_4c = local_4c >> 4;
    if (**(short **)(*(int *)(param_1 + 0xc4) + param_5 * 4) <= local_4c) {
      local_4c = **(short **)(*(int *)(param_1 + 0xc4) + param_5 * 4) + -1;
    }
    if (local_4c < local_4a) {
      local_4a = local_4c;
    }
    if (((local_3b == '\0') && (local_21 == '\x01')) &&
       ((local_28->f_170 != '\0' && ((param_3 == 2 || (param_3 == 4)))))) {
      if (param_3 == 2) {
        local_46 = local_50;
      }
      else {
        local_46 = local_4e;
      }
      sVar1 = param_5;
      if (local_3a != '\0') {
        while (local_32 = sVar1, local_32 <= param_6) {
          if (*(int *)(*(int *)(param_1 + 0xc4) + local_32 * 4) != 0) {
            local_40 = *(int *)(*(int *)(param_1 + 0xc4) + local_32 * 4);
            for (local_48 = local_4a; local_48 <= local_4c; local_48 = local_48 + 1) {
              local_34 = _GetTileID(local_40,(int)local_48,(int)local_46);
              if ((local_34 < 1) || (0x1a2 < local_34)) {
                local_34 = _GetTileID(local_40,(int)local_48,(int)(short)(local_42 + local_46));
                local_37 = _IsTileHorizWall((int)local_34);
                if (local_37 == '\0') {
                  local_3b = '\0';
                  goto LAB_0005ca45;
                }
              }
              else {
                local_34 = _GetTileID(local_40,(int)local_48,(int)(short)(local_42 + local_46));
                local_37 = _IsTileHorizWall((int)local_34);
                if (local_37 == '\0') {
                  local_3b = '\x01';
                  goto LAB_0005ca45;
                }
              }
            }
          }
          sVar1 = local_32 + 1;
        }
      }
LAB_0005ca45:
      if (local_3b == '\0') {
        local_64._0_1_ = local_28->f_10e;
        local_64._1_1_ = local_28->f_10f;
        local_64._2_1_ = local_28->f_110;
        local_64._3_1_ = local_28->f_111;
        local_68._0_1_ = local_28->f_10a;
        local_68._1_1_ = local_28->f_10b;
        local_68._2_1_ = local_28->f_10c;
        local_68._3_1_ = local_28->f_10d;
        local_40 = *(int *)(*(int *)(param_1 + 0xc4) + param_5 * 4);
        if (local_40 == 0) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c"
                     ,PTR____FUNCTION___29721_001c4d04,0xe30);
        }
        local_1e = local_4a;
        while ((((local_1e <= local_4c &&
                 (local_34 = _GetTileID(local_40,(int)local_1e,(int)local_46), 0 < local_34)) &&
                (local_34 < 0x1a3)) &&
               (cVar2 = _IsTileThickHorizWall(local_40,(int)local_1e,(int)local_46), cVar2 != '\0'))
              ) {
          local_34 = _GetTileID(local_40,(int)local_1e,(int)(short)(local_42 + local_46));
          local_37 = _IsTileHorizWall((int)local_34);
          if (local_37 == '\0') break;
          local_1e = local_1e + 1;
        }
        local_64 = CONCAT22(local_64._2_2_,
                            (param_2->destFrameRect).bottom + local_1e * -0x10 + -0x10);
        cVar2 = _SWCheckSpriteWithSprites
                          (param_2,0,param_3,&local_68,
                           *(undefined4 *)PTR__gDoorSpriteLayerP_001c41dc,
                           *(undefined4 *)PTR__gDoorSpriteLayerP_001c41dc,1,0,0,param_9);
        return cVar2;
      }
    }
  }
  if (((local_3b != '\0') && (param_9 != '\0')) && (local_3a != '\0')) {
    if (param_3 == 1) {
      local_36 = *(short *)(param_1 + 0xe4) * (local_48 + 1) - (short)local_60;
      _SWOffsetSprite(param_2,0.0,(float)(int)local_36);
    }
    else if (param_3 == 3) {
      local_36 = (short)local_5c - *(short *)(param_1 + 0xe4) * local_48;
      _SWOffsetSprite(param_2,0.0,(float)-(int)local_36);
    }
    else if (param_3 == 2) {
      local_36 = local_5c._2_2_ - *(short *)(param_1 + 0xe2) * local_46;
      _SWOffsetSprite(param_2,(float)-(int)local_36,0.0);
    }
    else if (param_3 == 4) {
      local_36 = *(short *)(param_1 + 0xe2) * (local_46 + 1) - local_60._2_2_;
      _SWOffsetSprite(param_2,(float)(int)local_36,0.0);
    }
  }
  return local_3b;
}


// ---- _GetTileID @ 0x5cd59 (142 bytes)

int _GetTileID(short *param_1,short param_2,short param_3)

{
  short sVar1;
  short local_14;
  short local_10;
  
  local_10 = param_2;
  local_14 = param_3;
  if (param_2 < 0) {
    local_10 = 0;
  }
  if (param_3 < 0) {
    local_14 = 0;
  }
  if (*param_1 <= local_10) {
    local_10 = *param_1 + -1;
  }
  if (param_1[1] <= local_14) {
    local_14 = param_1[1] + -1;
  }
  sVar1 = _ConvertTileID((int)*(short *)(*(int *)(*(int *)(param_1 + 2) + local_10 * 4) +
                                        local_14 * 2));
  return (int)sVar1;
}


// ---- _ConvertTileID @ 0x5cde7 (202 bytes)

int _ConvertTileID(short param_1)

{
  int local_14;
  
  if ((*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe0) <= param_1) &&
     (_printf("ConvertTileID failed because tileID is %d while SpriteWorld\'s maxNumTiles is %d! \n"
              ,(int)param_1,(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe0)),
     *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe0) <= param_1)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MoveProcSupport.c",
               PTR____FUNCTION___30381_001c4d50,0xe9c);
  }
  if (-1 < param_1) {
    param_1 = *(short *)(PTR__gOrigTileIDs_001c47c8 + param_1 * 2);
  }
  local_14 = (int)param_1;
  return local_14;
}


