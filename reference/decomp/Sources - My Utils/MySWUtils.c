// Ghidra decompilation of functions from original source file:
//   Sources - My Utils/MySWUtils.c

// ---- _SWBestMaskCollision @ 0x13d29 (35 bytes)

SWBoolean _SWBestMaskCollision(SpritePtr param_1,SpritePtr param_2)

{
  SWBoolean SVar1;
  
  SVar1 = _SWPixelCollision(param_1,param_2);
  return SVar1;
}


// ---- _ProcessOneFrame @ 0x13d4c (91 bytes)

void _ProcessOneFrame(SpriteWorldPtr param_1)

{
  if (param_1->runningTimeCount - param_1->timeOfLastFrame < (uint)(int)param_1->fpsTimeInterval) {
    param_1->timeOfLastFrame = param_1->runningTimeCount - (int)param_1->fpsTimeInterval;
  }
  _SWProcessSpriteWorld(param_1);
  return;
}


// ---- _AnimateOneFrame @ 0x13da7 (102 bytes)

void _AnimateOneFrame(SpriteWorldPtr param_1)

{
  if (param_1->runningTimeCount - param_1->timeOfLastFrame < (uint)(int)param_1->fpsTimeInterval) {
    param_1->timeOfLastFrame = param_1->runningTimeCount - (int)param_1->fpsTimeInterval;
  }
  _SWProcessSpriteWorld(param_1);
  _SWAnimateSpriteWorld(param_1);
  return;
}


// ---- _SetSpriteWorldForCapturedDisplay @ 0x13e0d (14 bytes)

void SetSpriteWorldForCapturedDisplay(void)

{
  return;
}


// ---- _SetSpriteWorldForUncapturedDisplay @ 0x13e1b (14 bytes)

void SetSpriteWorldForUncapturedDisplay(void)

{
  return;
}


// ---- _SWChangeTileImageFast @ 0x13e29 (238 bytes)

void _SWChangeTileImageFast(int param_1,short param_2,short param_3)

{
  if ((param_1 == 0) || (*(char *)(param_1 + 200) == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___21335_001c43cc,0x184);
  }
  if (*(short *)(param_1 + 0xe0) <= param_2) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___21335_001c43cc,0x185);
  }
  if (*(short *)(param_1 + 0xe0) <= param_3) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___21335_001c43cc,0x186);
  }
  *(short *)(*(int *)(param_1 + 0xd8) + param_2 * 2) = param_3;
  return;
}


// ---- _DoesLayerHaveTiles @ 0x13f17 (222 bytes)

undefined4 _DoesLayerHaveTiles(int param_1,short param_2,short param_3,short param_4)

{
  short sVar1;
  short local_12;
  short local_10;
  
  local_12 = 0;
  do {
    if (**(short **)(*(int *)(param_1 + 0xc4) + param_2 * 4) <= local_12) {
      return 0;
    }
    for (local_10 = 0; local_10 < *(short *)(*(int *)(*(int *)(param_1 + 0xc4) + param_2 * 4) + 2);
        local_10 = local_10 + 1) {
      sVar1 = *(short *)(*(int *)(local_12 * 4 +
                                 *(int *)(*(int *)(*(int *)(param_1 + 0xc4) + param_2 * 4) + 4)) +
                        local_10 * 2);
      if ((param_3 <= sVar1) && (sVar1 <= param_4)) {
        return 1;
      }
    }
    local_12 = local_12 + 1;
  } while( true );
}


// ---- _MyUpdateTilesOnScreenFaster @ 0x13ff5 (502 bytes)

void _MyUpdateTilesOnScreenFaster
               (SpriteWorldPtr param_1,short param_2,short param_3,short param_4,short param_5)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  short sVar7;
  short sVar8;
  short tileID;
  short **ppsVar9;
  short local_20;
  short local_1e;
  short local_1c;
  
  local_1c = param_2;
  if ((param_1 == (SpriteWorldPtr)0x0) || (param_1->tilingIsInitialized == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___21421_001c43d0,0x1c5);
  }
  sVar1 = (param_1->visScrollRect).top;
  sVar2 = param_1->tileHeight;
  sVar3 = (param_1->visScrollRect).left;
  sVar4 = param_1->tileWidth;
  sVar5 = (param_1->visScrollRect).bottom;
  sVar6 = param_1->tileHeight;
  sVar7 = (param_1->visScrollRect).right;
  sVar8 = param_1->tileWidth;
  for (; local_1c <= param_3; local_1c = local_1c + 1) {
    if (param_1->tileLayerArray[local_1c] != (TileMapStruct *)0x0) {
      ppsVar9 = param_1->tileLayerArray[local_1c]->tileMap;
      for (local_20 = sVar1 / sVar2; local_1e = sVar3 / sVar4,
          local_20 <= (short)((sVar5 + -1) / (int)sVar6); local_20 = local_20 + 1) {
        for (; local_1e <= (short)((sVar7 + -1) / (int)sVar8); local_1e = local_1e + 1) {
          tileID = ppsVar9[local_20][local_1e];
          if ((param_4 <= tileID) && (tileID <= param_5)) {
            _SWDrawTile(param_1,local_1c,local_20,local_1e,tileID);
          }
        }
      }
    }
  }
  return;
}


// ---- _SaveAllSpritesToTempLayers @ 0x141eb (516 bytes)

int _SaveAllSpritesToTempLayers(SpriteWorldPtr param_1)

{
  SpritePtr pSVar1;
  int local_30;
  SpriteLayerPtr local_20;
  SpritePtr local_1c;
  short local_12;
  int local_10;
  
  local_10 = 0;
  if (*PTR__gSpritesWereSaved_001c43c0 == '\x01') {
    local_30 = -1;
  }
  else {
    *(undefined2 *)PTR__gNumLayersSaved_001c438c = 0;
    local_20 = (SpriteLayerPtr)0x0;
    while( true ) {
      local_20 = _SWGetNextSpriteLayer(param_1,local_20);
      if (local_20 == (SpriteLayerPtr)0x0) break;
      *(short *)PTR__gNumLayersSaved_001c438c = *(short *)PTR__gNumLayersSaved_001c438c + 1;
    }
    *(short *)PTR__gNumLayersSaved_001c438c = *(short *)PTR__gNumLayersSaved_001c438c + -1;
    if (99 < *(short *)PTR__gNumLayersSaved_001c438c) {
      local_10 = -1;
    }
    if (local_10 == 0) {
      for (local_12 = 0; local_12 < *(short *)PTR__gNumLayersSaved_001c438c; local_12 = local_12 + 1
          ) {
        local_10 = _SWCreateSpriteLayer
                             ((SpriteLayerPtr *)(PTR__gTempLayerArray_001c43a4 + local_12 * 4));
        if (local_10 != 0) break;
      }
    }
    if (local_10 == 0) {
      local_12 = 0;
      local_20 = (SpriteLayerPtr)0x0;
      while( true ) {
        local_20 = _SWGetNextSpriteLayer(param_1,local_20);
        if (local_20 == (SpriteLayerPtr)0x0) break;
        if (local_20 != *(SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4) {
          if (*(short *)PTR__gNumLayersSaved_001c438c <= local_12) {
            (**(code **)PTR__gSWAssertFailProc_001c4170)
                      ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
                       PTR____FUNCTION___21508_001c43d4,0x21e);
          }
          local_1c = _SWGetNextSprite(local_20,(SpritePtr)0x0);
          while (local_1c != (SpritePtr)0x0) {
            pSVar1 = _SWGetNextSprite(local_20,local_1c);
            _SWRemoveSprite(local_1c);
            _SWAddSprite(*(SpriteLayerPtr *)(PTR__gTempLayerArray_001c43a4 + local_12 * 4),local_1c)
            ;
            local_1c = pSVar1;
          }
          local_12 = local_12 + 1;
        }
      }
      *PTR__gSpritesWereSaved_001c43c0 = 1;
    }
    local_30 = local_10;
  }
  return local_30;
}


// ---- _RestoreSpritesFromTempLayers @ 0x143ef (399 bytes)

void _RestoreSpritesFromTempLayers(SpriteWorldPtr param_1)

{
  SpriteLayerPtr spriteLayerP;
  SpritePtr pSVar1;
  SpriteLayerPtr local_1c;
  SpritePtr local_18;
  short local_e;
  
  if (*PTR__gSpritesWereSaved_001c43c0 != '\0') {
    local_1c = _SWGetNextSpriteLayer(param_1,(SpriteLayerPtr)0x0);
    for (local_e = 0; local_e < *(short *)PTR__gNumLayersSaved_001c438c; local_e = local_e + 1) {
      spriteLayerP = *(SpriteLayerPtr *)(PTR__gTempLayerArray_001c43a4 + local_e * 4);
      if (local_1c == *(SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4) {
        local_1c = _SWGetNextSpriteLayer(param_1,local_1c);
      }
      else {
        if (local_1c == (SpriteLayerPtr)0x0) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
                     PTR____FUNCTION___21558_001c43dc,0x251);
        }
        else {
          local_18 = _SWGetNextSprite(spriteLayerP,(SpritePtr)0x0);
          while (local_18 != (SpritePtr)0x0) {
            pSVar1 = _SWGetNextSprite(spriteLayerP,local_18);
            _SWRemoveSprite(local_18);
            _SWAddSprite(local_1c,local_18);
            local_18 = pSVar1;
          }
        }
        local_1c = _SWGetNextSpriteLayer(param_1,local_1c);
      }
    }
    for (local_e = 0; local_e < *(short *)PTR__gNumLayersSaved_001c438c; local_e = local_e + 1) {
      _SWDisposeSpriteLayer((SpriteLayerPtr *)(PTR__gTempLayerArray_001c43a4 + local_e * 4));
    }
    *PTR__gSpritesWereSaved_001c43c0 = 0;
  }
  return;
}


// ---- _SWSaveTileMaps @ 0x1457e (145 bytes)

void _SWSaveTileMaps(int param_1)

{
  short local_e;
  
  if (*PTR__gTileMapsWereSaved_001c43b0 == '\0') {
    for (local_e = 0; local_e < 10; local_e = local_e + 1) {
      *(undefined4 *)(PTR__gSaveTileLayerArray_001c43a0 + local_e * 4) =
           *(undefined4 *)(*(int *)(param_1 + 0xc4) + local_e * 4);
      *(undefined4 *)(*(int *)(param_1 + 0xc4) + local_e * 4) = 0;
    }
    *(undefined2 *)(param_1 + 0xca) = 0;
    *PTR__gTileMapsWereSaved_001c43b0 = 1;
  }
  return;
}


// ---- _SWRestoreTileMaps @ 0x1460f (129 bytes)

void _SWRestoreTileMaps(SpriteWorldPtr param_1)

{
  short local_e;
  
  if (*PTR__gTileMapsWereSaved_001c43b0 != '\0') {
    for (local_e = 0; local_e < 10; local_e = local_e + 1) {
      _SWInstallTileMap(param_1,*(TileMapStructPtr *)
                                 (PTR__gSaveTileLayerArray_001c43a0 + local_e * 4),local_e);
      *(undefined4 *)(PTR__gSaveTileLayerArray_001c43a0 + local_e * 4) = 0;
    }
    *PTR__gTileMapsWereSaved_001c43b0 = 0;
  }
  return;
}


// ---- _InvalidateSpriteLayer @ 0x14690 (109 bytes)

void InvalidateSpriteLayer(int param_1)

{
  SpritePtr pSStack_10;
  
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___21633_001c43b8,0x2ab);
  }
  for (pSStack_10 = *(SpritePtr *)(param_1 + 0xc); pSStack_10 != (SpritePtr)0x0;
      pSStack_10 = pSStack_10->nextSpriteP) {
    _SWInvalidateSprite(pSStack_10);
  }
  return;
}


// ---- _OffsetSpritesInLayer @ 0x146fd (123 bytes)

void _OffsetSpritesInLayer(int param_1,SWMovementType param_2,SWMovementType param_3)

{
  SpritePtr local_10;
  
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___21646_001c4380,0x2c2);
  }
  for (local_10 = *(SpritePtr *)(param_1 + 0xc); local_10 != (SpritePtr)0x0;
      local_10 = local_10->nextSpriteP) {
    _SWOffsetSprite(local_10,param_2,param_3);
  }
  return;
}


// ---- _SortSpriteLayerBottomToTop @ 0x14778 (282 bytes)

void SortSpriteLayerBottomToTop(SpriteLayerPtr param_1)

{
  bool bVar1;
  SpritePtr pSStack_1c;
  SpritePtr pSStack_18;
  SpritePtr pSStack_14;
  
  _SortSpriteLayerRightToLeft(param_1);
  pSStack_18 = param_1->headSpriteP;
  pSStack_1c = _SWGetNextSprite(param_1,pSStack_18);
  while (pSStack_1c != (SpritePtr)0x0) {
    if ((pSStack_18->destFrameRect).bottom < (pSStack_1c->destFrameRect).bottom) {
      pSStack_14 = pSStack_1c->prevSpriteP;
      bVar1 = false;
      while ((pSStack_14 != (SpritePtr)0x0 && (!bVar1))) {
        if ((pSStack_14->destFrameRect).bottom < (pSStack_1c->destFrameRect).bottom) {
          pSStack_14 = pSStack_14->prevSpriteP;
        }
        else {
          bVar1 = true;
        }
      }
      if (pSStack_14 == (SpritePtr)0x0) {
        _SWRemoveSprite(pSStack_1c);
        _SWInsertSpriteBeforeSprite(pSStack_1c,param_1->headSpriteP);
        pSStack_1c = _SWGetNextSprite(param_1,pSStack_18);
      }
      else {
        _SWRemoveSprite(pSStack_1c);
        _SWInsertSpriteAfterSprite(pSStack_1c,pSStack_14);
        pSStack_1c = _SWGetNextSprite(param_1,pSStack_18);
      }
    }
    else {
      pSStack_18 = pSStack_1c;
      pSStack_1c = pSStack_1c->nextSpriteP;
    }
  }
  return;
}


// ---- _SortSpriteLayerRightToLeft @ 0x14892 (271 bytes)

void _SortSpriteLayerRightToLeft(SpriteLayerPtr param_1)

{
  bool bVar1;
  SpritePtr local_1c;
  SpritePtr local_18;
  SpritePtr local_14;
  
  local_18 = param_1->headSpriteP;
  local_1c = _SWGetNextSprite(param_1,local_18);
  while (local_1c != (SpritePtr)0x0) {
    if ((local_1c->destFrameRect).right < (local_18->destFrameRect).right) {
      local_14 = local_1c->prevSpriteP;
      bVar1 = false;
      while ((local_14 != (SpritePtr)0x0 && (!bVar1))) {
        if ((local_1c->destFrameRect).right < (local_14->destFrameRect).right) {
          local_14 = local_14->prevSpriteP;
        }
        else {
          bVar1 = true;
        }
      }
      if (local_14 == (SpritePtr)0x0) {
        _SWRemoveSprite(local_1c);
        _SWInsertSpriteBeforeSprite(local_1c,param_1->headSpriteP);
        local_1c = _SWGetNextSprite(param_1,local_18);
      }
      else {
        _SWRemoveSprite(local_1c);
        _SWInsertSpriteAfterSprite(local_1c,local_14);
        local_1c = _SWGetNextSprite(param_1,local_18);
      }
    }
    else {
      local_18 = local_1c;
      local_1c = local_1c->nextSpriteP;
    }
  }
  return;
}


// ---- _SWCheckSpriteWithSprites @ 0x149a1 (1299 bytes)

bool _SWCheckSpriteWithSprites
               (SpritePtr param_1,int *param_2,uint param_3,short *param_4,int param_5,int param_6,
               undefined1 param_7,undefined4 param_8,undefined1 param_9,char param_10)

{
  short local_2a;
  short sStack_28;
  short local_26;
  short sStack_24;
  undefined4 local_22;
  undefined4 local_1e;
  undefined4 local_1a;
  undefined4 local_16;
  short local_12;
  int local_10;
  
  local_10 = 0;
  if (param_5 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___21731_001c43b4,0x378);
  }
  if (param_6 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___21731_001c43b4,0x379);
  }
  if (param_1 == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___21731_001c43b4,0x37a);
  }
  local_16._0_2_ = (param_1->destFrameRect).bottom;
  local_16._2_2_ = (param_1->destFrameRect).right;
  local_1a._0_2_ = (param_1->destFrameRect).top;
  local_1a._2_2_ = (param_1->destFrameRect).left;
  local_1e._0_2_ = (param_1->oldFrameRect).bottom;
  local_1e._2_2_ = (param_1->oldFrameRect).right;
  local_22._0_2_ = (param_1->oldFrameRect).top;
  local_22._2_2_ = (param_1->oldFrameRect).left;
  if (param_4 != (short *)0x0) {
    local_1a = CONCAT22(local_1a._2_2_ + param_4[1],(short)local_1a + *param_4);
    local_16 = CONCAT22(local_16._2_2_ - param_4[3],(short)local_16 - param_4[2]);
    local_22 = CONCAT22(local_22._2_2_ + param_4[1],(short)local_22 + *param_4);
    local_1e = CONCAT22(local_1e._2_2_ - param_4[3],(short)local_1e - param_4[2]);
  }
  if (param_3 == 2) {
    local_1a = CONCAT22(local_1e._2_2_,(short)local_1a);
  }
  else if (param_3 < 3) {
    if (param_3 == 1) {
      local_16 = CONCAT22(local_16._2_2_,(short)local_22);
    }
  }
  else if (param_3 == 3) {
    local_1a = CONCAT22(local_1a._2_2_,(short)local_1e);
  }
  else if (param_3 == 4) {
    local_16 = CONCAT22(local_22._2_2_,(short)local_16);
  }
  if (local_16._2_2_ < local_1a._2_2_) {
    _SwapVariables((int)&local_1a + 2,(int)&local_16 + 2);
  }
  if ((short)local_16 < (short)local_1a) {
    _SwapVariables(&local_1a,&local_16);
  }
  if (local_16._2_2_ < local_1a._2_2_) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___21731_001c43b4,0x3a9);
  }
  if ((short)local_16 < (short)local_1a) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___21731_001c43b4,0x3aa);
  }
  local_10 = _FindSpriteByRectInMultipleLayers
                       (param_5,param_6,param_1,&local_1a,param_7,param_8,param_9);
  if ((param_10 != '\0') && (local_10 != 0)) {
    local_2a = (short)*(undefined4 *)(local_10 + 0x3e);
    sStack_28 = (short)((uint)*(undefined4 *)(local_10 + 0x3e) >> 0x10);
    sStack_24 = (short)((uint)*(undefined4 *)(local_10 + 0x42) >> 0x10);
    local_26 = (short)*(undefined4 *)(local_10 + 0x42);
    if (param_3 == 1) {
      if ((short)local_22 < (short)(local_26 - *(short *)(*(int *)(local_10 + 0x7c) + 0x2e))) {
        local_10 = 0;
      }
      else {
        local_12 = (*(short *)(local_10 + 0x3a) - *(short *)(*(int *)(local_10 + 0x7c) + 0x2e)) -
                   (short)local_1a;
        _RoundOffSpriteVertLoc(param_1);
        _SWOffsetSprite(param_1,0.0,(float)(int)local_12);
      }
    }
    else if (param_3 == 3) {
      if ((short)(local_2a + *(short *)(*(int *)(local_10 + 0x7c) + 0x2a)) < (short)local_1e) {
        local_10 = 0;
      }
      else {
        local_12 = (short)local_16 -
                   (*(short *)(local_10 + 0x36) + *(short *)(*(int *)(local_10 + 0x7c) + 0x2a));
        _RoundOffSpriteVertLoc(param_1);
        _SWOffsetSprite(param_1,0.0,(float)-(int)local_12);
      }
    }
    else if (param_3 == 2) {
      if ((short)(sStack_28 + *(short *)(*(int *)(local_10 + 0x7c) + 0x2c)) < local_1e._2_2_) {
        local_10 = 0;
      }
      else {
        local_12 = local_16._2_2_ -
                   (*(short *)(local_10 + 0x38) + *(short *)(*(int *)(local_10 + 0x7c) + 0x2c));
        _SWOffsetSprite(param_1,(float)-(int)local_12,0.0);
      }
    }
    else if (param_3 == 4) {
      if (local_22._2_2_ < (short)(sStack_24 - *(short *)(*(int *)(local_10 + 0x7c) + 0x30))) {
        local_10 = 0;
      }
      else {
        local_12 = (*(short *)(local_10 + 0x3c) - *(short *)(*(int *)(local_10 + 0x7c) + 0x30)) -
                   local_1a._2_2_;
        _SWOffsetSprite(param_1,(float)(int)local_12,0.0);
      }
    }
  }
  if (param_2 != (int *)0x0) {
    *param_2 = local_10;
  }
  return local_10 != 0;
}


// ---- _RoundOffSpriteVertLoc @ 0x14eb4 (83 bytes)

void _RoundOffSpriteVertLoc(SpritePtr param_1)

{
  SWMovementType SVar1;
  SWMovementType horizLoc;
  
  SVar1 = _SWGetSpriteVertLoc(param_1);
  horizLoc = _SWGetSpriteHorizLoc(param_1);
  _SWMoveSprite(param_1,horizLoc,(float)(int)(short)(int)SVar1);
  return;
}


// ---- _SWCollideSpriteWithLayer @ 0x14f07 (938 bytes)

void _SWCollideSpriteWithLayer(int *param_1,int param_2,code *param_3)

{
  int iVar1;
  char local_50;
  int local_48;
  int local_40;
  int local_38;
  int local_30;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_1c;
  undefined2 local_1a;
  int *local_18;
  int *local_14;
  char local_d;
  
  if (param_2 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___21904_001c43d8,0x420);
  }
  if (param_3 == (code *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___21904_001c43d8,0x421);
  }
  local_18 = *(int **)(param_2 + 0xc);
  do {
    if (local_18 == (int *)0x0) {
      return;
    }
    local_14 = (int *)local_18[1];
    if ((((param_1 != local_18) &&
         (*(short *)((int)param_1 + 0x36) < *(short *)((int)local_18 + 0x3a))) &&
        (*(short *)((int)local_18 + 0x36) < *(short *)((int)param_1 + 0x3a))) &&
       (((short)param_1[0xe] < (short)local_18[0xf] && ((short)local_18[0xe] < (short)param_1[0xf]))
       )) {
      if ((*(char *)(param_1[0x1f] + 0x28) == '\0') && (*(char *)(local_18[0x1f] + 0x28) == '\0')) {
        local_d = '\x01';
      }
      else {
        if (((((int)*(short *)((int)param_1 + 0x36) + (int)*(short *)(param_1[0x1f] + 0x2a) <
               (int)*(short *)((int)local_18 + 0x3a) - (int)*(short *)(local_18[0x1f] + 0x2e)) &&
             ((int)*(short *)((int)local_18 + 0x36) + (int)*(short *)(local_18[0x1f] + 0x2a) <
              (int)*(short *)((int)param_1 + 0x3a) - (int)*(short *)(param_1[0x1f] + 0x2e))) &&
            ((int)(short)param_1[0xe] + (int)*(short *)(param_1[0x1f] + 0x2c) <
             (int)(short)local_18[0xf] - (int)*(short *)(local_18[0x1f] + 0x30))) &&
           ((int)(short)local_18[0xe] + (int)*(short *)(local_18[0x1f] + 0x2c) <
            (int)(short)param_1[0xf] - (int)*(short *)(param_1[0x1f] + 0x30))) {
          local_50 = '\x01';
        }
        else {
          local_50 = '\0';
        }
        local_d = local_50;
      }
      if (local_d != '\0') {
        local_48 = (int)(short)local_18[0xe] + (int)*(short *)(local_18[0x1f] + 0x2c);
        iVar1 = (int)(short)param_1[0xe] + (int)*(short *)(param_1[0x1f] + 0x2c);
        if (local_48 < iVar1) {
          local_48 = iVar1;
        }
        local_1e = (undefined2)local_48;
        local_40 = (int)*(short *)((int)local_18 + 0x36) + (int)*(short *)(local_18[0x1f] + 0x2a);
        iVar1 = (int)*(short *)((int)param_1 + 0x36) + (int)*(short *)(param_1[0x1f] + 0x2a);
        if (local_40 < iVar1) {
          local_40 = iVar1;
        }
        local_20 = (undefined2)local_40;
        local_38 = (int)(short)local_18[0xf] - (int)*(short *)(local_18[0x1f] + 0x30);
        iVar1 = (int)(short)param_1[0xf] - (int)*(short *)(param_1[0x1f] + 0x30);
        if (iVar1 < local_38) {
          local_38 = iVar1;
        }
        local_1a = (undefined2)local_38;
        local_30 = (int)*(short *)((int)local_18 + 0x3a) - (int)*(short *)(local_18[0x1f] + 0x2e);
        iVar1 = (int)*(short *)((int)param_1 + 0x3a) - (int)*(short *)(param_1[0x1f] + 0x2e);
        if (iVar1 < local_30) {
          local_30 = iVar1;
        }
        local_1c = (undefined2)local_30;
        (*param_3)(param_1,local_18,&local_20);
        if (param_1[0x36] != 0) {
          return;
        }
        if (*param_1 == 0) {
          return;
        }
      }
    }
    if ((local_14 == (int *)0x0) || (local_14[0x36] == 0)) {
      local_18 = local_14;
    }
    else {
      local_18 = *(int **)(param_2 + 0xc);
    }
  } while( true );
}


// ---- _GetSpriteCollidingWithSprite @ 0x152b1 (633 bytes)

int * GetSpriteCollidingWithSprite(int *param_1,int param_2,code *param_3)

{
  int *piVar1;
  bool bVar2;
  char cVar3;
  int *piStack_18;
  
  if (param_2 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22070_001c437c,0x476);
  }
  if (param_3 == (code *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22070_001c437c,0x477);
  }
  piStack_18 = *(int **)(param_2 + 0xc);
  do {
    if (piStack_18 == (int *)0x0) {
      return (int *)0x0;
    }
    piVar1 = (int *)piStack_18[1];
    if ((((param_1 != piStack_18) &&
         (*(short *)((int)param_1 + 0x36) < *(short *)((int)piStack_18 + 0x3a))) &&
        (*(short *)((int)piStack_18 + 0x36) < *(short *)((int)param_1 + 0x3a))) &&
       (((short)param_1[0xe] < (short)piStack_18[0xf] &&
        ((short)piStack_18[0xe] < (short)param_1[0xf])))) {
      if ((*(char *)(param_1[0x1f] + 0x28) == '\0') && (*(char *)(piStack_18[0x1f] + 0x28) == '\0'))
      {
        bVar2 = true;
      }
      else if (((((int)*(short *)((int)param_1 + 0x36) + (int)*(short *)(param_1[0x1f] + 0x2a) <
                  (int)*(short *)((int)piStack_18 + 0x3a) - (int)*(short *)(piStack_18[0x1f] + 0x2e)
                 ) && ((int)*(short *)((int)piStack_18 + 0x36) +
                       (int)*(short *)(piStack_18[0x1f] + 0x2a) <
                       (int)*(short *)((int)param_1 + 0x3a) - (int)*(short *)(param_1[0x1f] + 0x2e))
                ) && ((int)(short)param_1[0xe] + (int)*(short *)(param_1[0x1f] + 0x2c) <
                      (int)(short)piStack_18[0xf] - (int)*(short *)(piStack_18[0x1f] + 0x30))) &&
              ((int)(short)piStack_18[0xe] + (int)*(short *)(piStack_18[0x1f] + 0x2c) <
               (int)(short)param_1[0xf] - (int)*(short *)(param_1[0x1f] + 0x30))) {
        bVar2 = true;
      }
      else {
        bVar2 = false;
      }
      if (bVar2) {
        cVar3 = (*param_3)(param_1,piStack_18);
        if (cVar3 != '\0') {
          return piStack_18;
        }
        if (param_1[0x36] != 0) {
          return piStack_18;
        }
        if (*param_1 == 0) {
          return piStack_18;
        }
      }
    }
    piStack_18 = piVar1;
    if ((piVar1 != (int *)0x0) && (piVar1[0x36] != 0)) {
      piStack_18 = *(int **)(param_2 + 0xc);
    }
  } while( true );
}


// ---- _IsSpriteCollidingWithLayer @ 0x1552a (661 bytes)

undefined1 _IsSpriteCollidingWithLayer(int param_1,int param_2,char param_3,char param_4)

{
  int iVar1;
  char local_28;
  int local_18;
  char local_e;
  
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22187_001c4384,0x4c8);
  }
  if (param_2 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22187_001c4384,0x4c9);
  }
  local_18 = *(int *)(param_2 + 0xc);
  do {
    if (local_18 == 0) {
      return 0;
    }
    iVar1 = *(int *)(local_18 + 4);
    if ((((param_1 != local_18) && (*(short *)(param_1 + 0x36) < *(short *)(local_18 + 0x3a))) &&
        (*(short *)(local_18 + 0x36) < *(short *)(param_1 + 0x3a))) &&
       ((*(short *)(param_1 + 0x38) < *(short *)(local_18 + 0x3c) &&
        (*(short *)(local_18 + 0x38) < *(short *)(param_1 + 0x3c))))) {
      if ((*(char *)(*(int *)(param_1 + 0x7c) + 0x28) == '\0') &&
         (*(char *)(*(int *)(local_18 + 0x7c) + 0x28) == '\0')) {
        local_e = '\x01';
      }
      else {
        if (((((int)*(short *)(param_1 + 0x36) + (int)*(short *)(*(int *)(param_1 + 0x7c) + 0x2a) <
               (int)*(short *)(local_18 + 0x3a) - (int)*(short *)(*(int *)(local_18 + 0x7c) + 0x2e))
             && ((int)*(short *)(local_18 + 0x36) +
                 (int)*(short *)(*(int *)(local_18 + 0x7c) + 0x2a) <
                 (int)*(short *)(param_1 + 0x3a) - (int)*(short *)(*(int *)(param_1 + 0x7c) + 0x2e))
             ) && ((int)*(short *)(param_1 + 0x38) +
                   (int)*(short *)(*(int *)(param_1 + 0x7c) + 0x2c) <
                   (int)*(short *)(local_18 + 0x3c) -
                   (int)*(short *)(*(int *)(local_18 + 0x7c) + 0x30))) &&
           ((int)*(short *)(local_18 + 0x38) + (int)*(short *)(*(int *)(local_18 + 0x7c) + 0x2c) <
            (int)*(short *)(param_1 + 0x3c) - (int)*(short *)(*(int *)(param_1 + 0x7c) + 0x30))) {
          local_28 = '\x01';
        }
        else {
          local_28 = '\0';
        }
        local_e = local_28;
      }
      if ((param_4 != '\0') && (*(char *)(local_18 + 0x32) == '\0')) {
        local_e = '\0';
      }
      if ((local_e != '\0') && (param_3 != '\0')) {
        local_e = _SWBestMaskCollision(param_1,local_18);
      }
      if (local_e != '\0') {
        return 1;
      }
    }
    local_18 = iVar1;
    if ((iVar1 != 0) && (*(int *)(iVar1 + 0xd8) != 0)) {
      local_18 = *(int *)(param_2 + 0xc);
    }
  } while( true );
}


// ---- _MoveSpritesFromLayerToLayer @ 0x157bf (100 bytes)

void _MoveSpritesFromLayerToLayer(SpriteLayerPtr param_1,SpriteLayerPtr param_2)

{
  SpritePtr pSVar1;
  undefined4 local_14;
  
  local_14 = _SWGetNextSprite(param_1,(SpritePtr)0x0);
  while (local_14 != (SpritePtr)0x0) {
    pSVar1 = _SWGetNextSprite(param_1,local_14);
    _SWRemoveSprite(local_14);
    _SWAddSprite(param_2,local_14);
    local_14 = pSVar1;
  }
  return;
}


// ---- _FindSpriteByRectInMultipleLayers @ 0x15823 (415 bytes)

SpritePtr _FindSpriteByRectInMultipleLayers
                    (int param_1,int param_2,SpriteRec *param_3,SWRect *param_4,char param_5,
                    int param_6,char param_7)

{
  char cVar1;
  SWBoolean SVar2;
  int local_18;
  SpriteRec *local_14;
  
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22321_001c4394,0x537);
  }
  if (param_3 == (SpriteRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22321_001c4394,0x538);
  }
  local_18 = param_1;
  do {
    if (local_18 == 0) {
      return local_14;
    }
    local_14 = *(SpriteRec **)(local_18 + 0xc);
    if (local_14 == param_3) {
      local_14 = local_14->nextSpriteP;
    }
    if (param_5 == '\0') {
      while (local_14 != (SpritePtr)0x0) {
        if (((param_6 != 0) || (local_14->isVisible != '\0')) &&
           (SVar2 = _SWIsSpriteInRect(local_14,param_4), SVar2 != '\0')) {
          if (param_7 == '\0') {
            return local_14;
          }
          if ((param_3->destFrameRect).top < (local_14->destFrameRect).top) {
            return local_14;
          }
        }
        local_14 = local_14->nextSpriteP;
        if (local_14 == param_3) {
          local_14 = local_14->nextSpriteP;
        }
      }
    }
    else {
      while (local_14 != (SpritePtr)0x0) {
        if (((param_6 != 0) || (local_14->isVisible != '\0')) &&
           (cVar1 = _IsSpriteOldRectInRect(local_14,param_4), cVar1 != '\0')) {
          if (param_7 == '\0') {
            return local_14;
          }
          if ((param_3->destFrameRect).top < (local_14->destFrameRect).top) {
            return local_14;
          }
        }
        local_14 = local_14->nextSpriteP;
        if (local_14 == param_3) {
          local_14 = local_14->nextSpriteP;
        }
      }
    }
    if (local_18 == param_2) {
      return local_14;
    }
    local_18 = *(int *)(local_18 + 4);
  } while( true );
}


// ---- _SetAllSpriteLayersVisible @ 0x159c2 (90 bytes)

void SetAllSpriteLayersVisible(SpriteWorldPtr param_1,uchar param_2)

{
  undefined4 uStack_10;
  
  for (uStack_10 = _SWGetNextSpriteLayer(param_1,(SpriteLayerPtr)0x0);
      uStack_10 != (SpriteLayerRec *)0x0; uStack_10 = _SWGetNextSpriteLayer(param_1,uStack_10)) {
    _SWSetSpriteLayerVisible(uStack_10,param_2);
  }
  return;
}


// ---- _PauseAllSpriteLayers @ 0x15a1c (76 bytes)

void _PauseAllSpriteLayers(SpriteWorldPtr param_1)

{
  undefined4 local_10;
  
  for (local_10 = _SWGetNextSpriteLayer(param_1,(SpriteLayerPtr)0x0);
      local_10 != (SpriteLayerPtr)0x0; local_10 = _SWGetNextSpriteLayer(param_1,local_10)) {
    _SWPauseSpriteLayer(local_10);
  }
  return;
}


// ---- _UnpauseAllSpriteLayers @ 0x15a68 (76 bytes)

void _UnpauseAllSpriteLayers(SpriteWorldPtr param_1)

{
  undefined4 local_10;
  
  for (local_10 = _SWGetNextSpriteLayer(param_1,(SpriteLayerPtr)0x0);
      local_10 != (SpriteLayerPtr)0x0; local_10 = _SWGetNextSpriteLayer(param_1,local_10)) {
    _SWUnpauseSpriteLayer(local_10);
  }
  return;
}


// ---- _RemoveAllSpritesInLayer @ 0x15ab4 (148 bytes)

void _RemoveAllSpritesInLayer(SpriteLayerPtr param_1)

{
  SpritePtr pSVar1;
  SpritePtr local_14;
  
  if (param_1 == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22406_001c4390,0x5ba);
  }
  local_14 = _SWGetNextSprite(param_1,(SpritePtr)0x0);
  while (local_14 != (SpritePtr)0x0) {
    pSVar1 = _SWGetNextSprite(param_1,local_14);
    _SWRemoveSpriteFromAnimation(local_14,'\x01');
    local_14 = pSVar1;
  }
  return;
}


// ---- _SWSetSpritesInLayerVisible @ 0x15b48 (75 bytes)

void _SWSetSpritesInLayerVisible(SpriteLayerPtr param_1,SWBoolean param_2)

{
  undefined4 local_10;
  
  local_10 = (SpritePtr)0x0;
  while( true ) {
    local_10 = _SWGetNextSprite(param_1,local_10);
    if (local_10 == (SpritePtr)0x0) break;
    _SWSetSpriteVisible(local_10,param_2);
  }
  return;
}


// ---- _SWCreateSolidColorSprite @ 0x15b93 (96 bytes)

SWError SWCreateSolidColorSprite(SpritePtr *param_1)

{
  SWError SVar1;
  
  SVar1 = _SWCreateSprite(param_1,(void *)0x0,0);
  if (SVar1 == 0) {
    _SWSetSpriteDrawProc(*param_1,(DrawProcPtr_t)PTR_SWSolidColorSpriteDrawProc_001c43ac);
  }
  return SVar1;
}


// ---- _SWTopVisibleSpriteFromPoint @ 0x15bf3 (180 bytes)

SpritePtr SWTopVisibleSpriteFromPoint
                    (SpriteLayerPtr param_1,SpritePtr param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  SpritePtr pSStack_10;
  
  if (param_1 == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22442_001c43e0,0x623);
  }
  pSStack_10 = _SWGetPreviousSprite(param_1,param_2);
  while( true ) {
    if (pSStack_10 == (SpritePtr)0x0) {
      return (SpritePtr)0x0;
    }
    cVar1 = _SWIsPointInSpriteMask(pSStack_10,(int)param_3._2_2_,(int)(short)param_3,param_4);
    if ((cVar1 != '\0') && (pSStack_10->isVisible != '\0')) break;
    pSStack_10 = _SWGetPreviousSprite(param_1,pSStack_10);
  }
  return pSStack_10;
}


// ---- _SWConvertCoordinates @ 0x15ca7 (99 bytes)

void _SWConvertCoordinates(int param_1,short *param_2)

{
  *param_2 = *param_2 + (*(short *)(param_1 + 0x8c) - *(short *)(param_1 + 0x74));
  param_2[1] = param_2[1] + (*(short *)(param_1 + 0x8e) - *(short *)(param_1 + 0x76));
  return;
}


// ---- _MySetSpriteFrameTime @ 0x15d0a (175 bytes)

void _MySetSpriteFrameTime(SpritePtr param_1,int param_2)

{
  int iVar1;
  SpriteWorldRec *local_10;
  
  local_10 = (SpriteWorldRec *)0x0;
  if (param_1 == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22481_001c43a8,0x651);
  }
  iVar1 = _SWGetSpriteFrameTime(param_1);
  if (iVar1 != param_2) {
    _SWSetSpriteFrameTime(param_1,param_2);
    if (param_1->parentSpriteLayerP != (SpriteLayerRec *)0x0) {
      local_10 = param_1->parentSpriteLayerP->parentSpriteWorldP;
    }
    if (local_10 == (SpriteWorldRec *)0x0) {
      local_10 = *(SpriteWorldRec **)PTR__gSWCurrentSpriteWorld_001c439c;
    }
    if (local_10 != (SpriteWorldRec *)0x0) {
      param_1->timeOfLastFrameChange = local_10->runningTimeCount;
    }
  }
  return;
}


// ---- _MyAdvanceSpriteFrame @ 0x15db9 (239 bytes)

void _MyAdvanceSpriteFrame(SpritePtr param_1,short *param_2)

{
  int local_10;
  
  local_10 = param_1->curFrameIndex + (int)*param_2;
  if (local_10 < param_1->firstFrameIndex) {
    if (param_1->frameAdvanceMode == 0) {
      local_10 = param_1->lastFrameIndex;
    }
    else {
      *param_2 = -*param_2;
      local_10 = local_10 + *param_2 + (int)*param_2;
    }
  }
  else if (param_1->lastFrameIndex < local_10) {
    if (param_1->frameAdvanceMode == 0) {
      local_10 = param_1->firstFrameIndex;
    }
    else {
      *param_2 = -*param_2;
      local_10 = local_10 + *param_2 + (int)*param_2;
    }
  }
  if (-1 < local_10) {
    _SWSetCurrentFrameIndex(param_1,local_10);
  }
  return;
}


// ---- _SWResetSpriteMovementTimer @ 0x15ea8 (32 bytes)

void _SWResetSpriteMovementTimer(int param_1,int param_2)

{
  *(undefined4 *)(param_2 + 0xa8) = *(undefined4 *)(param_1 + 0x128);
  return;
}


// ---- _SWSetSpriteHotSpot @ 0x15ec8 (121 bytes)

void SWSetSpriteHotSpot(int param_1,short param_2,short param_3)

{
  undefined2 uStack_e;
  
  for (uStack_e = 0; (int)uStack_e < *(int *)(param_1 + 0x84); uStack_e = uStack_e + 1) {
    _SWSetFrameHotSpot(*(FramePtr *)(*(int *)(param_1 + 0x78) + uStack_e * 4),
                       *(DrawDataPtr *)(param_1 + 0xe0),param_2,param_3);
  }
  return;
}


// ---- _GetSpriteWorldFromSprite @ 0x15f41 (103 bytes)

undefined4 _GetSpriteWorldFromSprite(int *param_1)

{
  undefined4 local_10;
  
  if (param_1 == (int *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22561_001c43c4,0x6b8);
  }
  if (*param_1 == 0) {
    local_10 = 0;
  }
  else {
    local_10 = *(undefined4 *)*param_1;
  }
  return local_10;
}


// ---- _SWGetSpriteMoveProc @ 0x15fa8 (23 bytes)

undefined4 _SWGetSpriteMoveProc(int param_1)

{
  return *(undefined4 *)(param_1 + 0xcc);
}


// ---- _GetDistBetweenSprites @ 0x15fbf (449 bytes)

int _GetDistBetweenSprites(SpritePtr param_1,SpritePtr param_2)

{
  short sVar1;
  short sVar2;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  longdouble extraout_ST0_01;
  longdouble extraout_ST0_02;
  SWMovementType SVar3;
  SWMovementType SVar4;
  SWMovementType SVar5;
  SWMovementType SVar6;
  
  if (param_1 == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22584_001c4398,0x6d9);
  }
  if (param_2 == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22584_001c4398,0x6da);
  }
  SVar3 = _SWGetSpriteHorizLoc(param_1);
  _SWGetSpriteWidth(param_1);
  SVar4 = _SWGetSpriteVertLoc(param_1);
  _SWGetSpriteHeight(param_1);
  SVar5 = _SWGetSpriteHorizLoc(param_2);
  _SWGetSpriteWidth(param_2);
  SVar6 = _SWGetSpriteVertLoc(param_2);
  _SWGetSpriteHeight(param_2);
  sVar1 = (short)(int)((float)extraout_ST0 / 2.0 + SVar3) -
          (short)(int)((float)extraout_ST0_01 / 2.0 + SVar5);
  sVar2 = (short)(int)((float)extraout_ST0_00 / 2.0 + SVar4) -
          (short)(int)((float)extraout_ST0_02 / 2.0 + SVar6);
  return (int)(short)(int)SQRT((double)((int)sVar1 * (int)sVar1 + (int)sVar2 * (int)sVar2));
}


// ---- _GetDistBetweenSpritesHorizMid @ 0x16180 (212 bytes)

int _GetDistBetweenSpritesHorizMid(SpritePtr param_1,SpritePtr param_2)

{
  short sVar1;
  short sVar2;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SWMovementType SVar3;
  undefined4 local_20;
  
  SVar3 = _SWGetSpriteHorizLoc(param_1);
  _SWGetSpriteWidth(param_1);
  sVar1 = (short)(int)((float)extraout_ST0 / 2.0 + SVar3);
  SVar3 = _SWGetSpriteHorizLoc(param_2);
  _SWGetSpriteWidth(param_2);
  sVar2 = (short)(int)((float)extraout_ST0_00 / 2.0 + SVar3);
  if ((int)sVar1 - (int)sVar2 < 0) {
    sVar2 = sVar2 - sVar1;
  }
  else {
    sVar2 = sVar1 - sVar2;
  }
  local_20 = (int)sVar2;
  return local_20;
}


// ---- _GetDistBetweenSpritesVertMid @ 0x16254 (212 bytes)

int _GetDistBetweenSpritesVertMid(SpritePtr param_1,SpritePtr param_2)

{
  short sVar1;
  short sVar2;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SWMovementType SVar3;
  undefined4 local_20;
  
  SVar3 = _SWGetSpriteVertLoc(param_1);
  _SWGetSpriteHeight(param_1);
  sVar1 = (short)(int)((float)extraout_ST0 / 2.0 + SVar3);
  SVar3 = _SWGetSpriteVertLoc(param_2);
  _SWGetSpriteHeight(param_2);
  sVar2 = (short)(int)((float)extraout_ST0_00 / 2.0 + SVar3);
  if ((int)sVar1 - (int)sVar2 < 0) {
    sVar2 = sVar2 - sVar1;
  }
  else {
    sVar2 = sVar1 - sVar2;
  }
  local_20 = (int)sVar2;
  return local_20;
}


// ---- _DelayMoveProcByValue @ 0x16328 (68 bytes)

void DelayMoveProcByValue(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = _GetSpriteWorldFromSprite(param_1);
  if (iVar1 != 0) {
    *(int *)(param_1 + 0xa8) = *(int *)(iVar1 + 0x128) + *(int *)(param_1 + 0xa4) + param_2;
  }
  return;
}


// ---- _FindSpriteByPixel @ 0x1636c (161 bytes)

int FindSpriteByPixel(int param_1,int param_2,short param_3,short param_4)

{
  int iStack_28;
  int iStack_10;
  
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22698_001c43c8,0x73c);
  }
  if (param_2 == 0) {
    iStack_28 = *(int *)(param_1 + 0xc);
  }
  else {
    iStack_28 = param_2;
  }
  for (iStack_10 = iStack_28;
      (iStack_10 != 0 &&
      ((*(short *)(iStack_10 + 0x36) != param_4 || (*(short *)(iStack_10 + 0x38) != param_3))));
      iStack_10 = *(int *)(iStack_10 + 4)) {
  }
  return iStack_10;
}


// ---- _FindSpriteByRect @ 0x1640d (143 bytes)

SpritePtr FindSpriteByRect(int param_1,SpritePtr param_2,SWRect *param_3)

{
  SWBoolean SVar1;
  SpritePtr pSStack_20;
  SpritePtr pSStack_10;
  
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22721_001c4388,0x756);
  }
  if (param_2 == (SpritePtr)0x0) {
    pSStack_20 = *(SpritePtr *)(param_1 + 0x10);
  }
  else {
    pSStack_20 = param_2;
  }
  pSStack_10 = pSStack_20;
  while( true ) {
    if (pSStack_10 == (SpritePtr)0x0) {
      return (SpritePtr)0x0;
    }
    SVar1 = _SWIsSpriteInRect(pSStack_10,param_3);
    if (SVar1 != '\0') break;
    pSStack_10 = pSStack_10->prevSpriteP;
  }
  return pSStack_10;
}


// ---- _IsSpriteOldRectInRect @ 0x1649c (285 bytes)

undefined1 _IsSpriteOldRectInRect(int param_1,short *param_2)

{
  undefined1 local_10;
  
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22740_001c43bc,0x771);
  }
  if (*(int *)(param_1 + 0x7c) == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MySWUtils.c",
               PTR____FUNCTION___22740_001c43bc,0x772);
  }
  if (((((int)*(short *)(param_1 + 0x3e) + (int)*(short *)(*(int *)(param_1 + 0x7c) + 0x2a) <
         (int)param_2[2]) &&
       ((int)*param_2 <
        (int)*(short *)(param_1 + 0x42) - (int)*(short *)(*(int *)(param_1 + 0x7c) + 0x2e))) &&
      ((int)*(short *)(param_1 + 0x40) + (int)*(short *)(*(int *)(param_1 + 0x7c) + 0x2c) <
       (int)param_2[3])) &&
     ((int)param_2[1] <
      (int)*(short *)(param_1 + 0x44) - (int)*(short *)(*(int *)(param_1 + 0x7c) + 0x30))) {
    local_10 = 1;
  }
  else {
    local_10 = 0;
  }
  return local_10;
}


// ---- _SWCenterSpriteOverSprite @ 0x165b9 (269 bytes)

void _SWCenterSpriteOverSprite(SpritePtr param_1,SpritePtr param_2)

{
  int iVar1;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  longdouble extraout_ST0_01;
  longdouble extraout_ST0_02;
  SWMovementType SVar2;
  short local_1c;
  short local_1a;
  undefined2 local_18;
  undefined2 local_16;
  short local_14;
  short local_12;
  undefined2 local_10;
  undefined2 local_e;
  
  local_14 = 0;
  local_12 = 0;
  _SWGetSpriteWidth(param_1);
  local_e = (undefined2)(int)extraout_ST0;
  _SWGetSpriteHeight(param_1);
  local_10 = (undefined2)(int)extraout_ST0_00;
  SVar2 = _SWGetSpriteVertLoc(param_2);
  local_1c = (short)(int)SVar2;
  SVar2 = _SWGetSpriteHorizLoc(param_2);
  local_1a = (short)(int)SVar2;
  iVar1 = (int)local_1a;
  _SWGetSpriteWidth(param_2);
  local_16 = (undefined2)(int)((float)iVar1 + (float)extraout_ST0_01);
  iVar1 = (int)local_1c;
  _SWGetSpriteHeight(param_2);
  local_18 = (undefined2)(int)((float)iVar1 + (float)extraout_ST0_02);
  _SWCenterRect(&local_14,&local_1c);
  _SWMoveSprite(param_1,(float)(int)local_12,(float)(int)local_14);
  return;
}


// ---- _SWCenterSpriteInRect @ 0x166c6 (88 bytes)

void _SWCenterSpriteInRect(SpritePtr param_1,undefined4 param_2)

{
  undefined4 local_14;
  undefined4 local_10;
  
  local_10._0_2_ = (param_1->destFrameRect).bottom;
  local_10._2_2_ = (param_1->destFrameRect).right;
  local_14._0_2_ = (param_1->destFrameRect).top;
  local_14._2_2_ = (param_1->destFrameRect).left;
  _SWCenterRect(&local_14,param_2);
  _SWMoveSprite(param_1,(float)(int)local_14._2_2_,(float)(int)(short)local_14);
  return;
}


