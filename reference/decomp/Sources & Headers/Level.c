// Ghidra decompilation of functions from original source file:
//   Sources & Headers/Level.c

// ---- _LoadRoom @ 0x7257c (1159 bytes)

void _LoadRoom(short param_1,undefined1 param_2)

{
  undefined1 uVar1;
  undefined1 uVar2;
  char cVar3;
  SWError SVar4;
  TileMapStructPtr local_11c;
  char local_116 [255];
  char local_17 [3];
  char local_14;
  short local_12;
  int local_10;
  
  local_10 = 0;
  SVar4 = _SWStickyError();
  if (SVar4 != 0) {
    SVar4 = _SWStickyError();
    _MMFatalErrorCallback
              (SVar4,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
               PTR____FUNCTION___27422_001c4f48,0x15);
  }
  _SDL_GL_MakeCurrent(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158),
                      *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x15c));
  _WriteCStringToFile("LoadRoom() entered for room ID: ");
  _WriteNumberToFile((int)param_1);
  _WriteCarriageReturn(1);
  _strcpy(local_17,*(char **)PTR__TMAPResType_001c4f30);
  for (local_12 = 0; local_12 < 7; local_12 = local_12 + 1) {
    _sprintf(local_116,"%s\'%s\'/%05d",PTR__gCurOpenMansionFilePath_001c4124,local_17,(int)param_1);
    local_10 = _SWLoadTileMap(local_116,&local_11c);
    if (local_10 != 0) {
      _WriteCStringToFile("SWLoadTileMap FAILED for layer ");
      _WriteNumberToFile((int)local_12);
      _WriteCarriageReturn(1);
    }
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
                 PTR____FUNCTION___27422_001c4f48,0x2b);
    }
    _SWInstallTileMap(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_11c,local_12);
    local_14 = local_14 + '\x01';
  }
  _MMLoadExtraBackFrameForRoom(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)param_1);
  local_10 = _LoadSpritesInRoom((int)param_1,param_2);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
               PTR____FUNCTION___27422_001c4f48,0x35);
  }
  SVar4 = _SWStickyError();
  if (SVar4 != 0) {
    SVar4 = _SWStickyError();
    _MMFatalErrorCallback
              (SVar4,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
               PTR____FUNCTION___27422_001c4f48,0x37);
  }
  _SWCustomSortSpriteLayer
            (*(SpriteLayerPtr *)PTR__gTrapDoorSpriteLayerP_001c40e0,
             (SpriteCompareProcPtr_t)PTR_CompareSpriteLeft_001c4d8c);
  _CreateZapperBeamsForRoom();
  SVar4 = _SWStickyError();
  if (SVar4 != 0) {
    SVar4 = _SWStickyError();
    _MMFatalErrorCallback
              (SVar4,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
               PTR____FUNCTION___27422_001c4f48,0x41);
  }
  if (*(int *)PTR__gCurrentGameMode_001c42d0 != 6) {
    _ResetConveyorBeltDir();
  }
  _SetLeversToCorrectState(*(undefined4 *)PTR__gLeverSpriteLayerP_001c41d4);
  _SetBrickSwitchesToCorrectState(*(undefined4 *)PTR__gBrickSwitchSpriteLayerP_001c4188);
  _OpenDoorsThatWereOpenedByPlayer();
  _OpenDoorsThatAreOpenToStart();
  _CreateGrayStatueOnSavePedestals();
  SVar4 = _SWStickyError();
  if (SVar4 != 0) {
    SVar4 = _SWStickyError();
    _MMFatalErrorCallback
              (SVar4,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
               PTR____FUNCTION___27422_001c4f48,0x52);
  }
  uVar1 = *PTR__gRoomWasDark_001c4814;
  uVar2 = _IsRoomDark((int)param_1);
  _MakeRoomDarkOrLight(uVar2,uVar1);
  *(undefined2 *)(PTR__gRoomMoveBoundsRect_001c4d5c + 2) = 0;
  *(undefined2 *)PTR__gRoomMoveBoundsRect_001c4d5c = 0;
  *(short *)(PTR__gRoomMoveBoundsRect_001c4d5c + 6) =
       *(short *)(**(int **)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + 2) << 4;
  *(short *)(PTR__gRoomMoveBoundsRect_001c4d5c + 4) =
       *(short *)**(undefined4 **)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) << 4;
  _SWSetScrollingWorldMoveBounds
            (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
             (SWRect *)PTR__gRoomMoveBoundsRect_001c4d5c);
  cVar3 = _IsRoomDark((int)param_1);
  if (cVar3 == '\0') {
    _ConvertTilesForCurRoom();
  }
  _FixCandleStandsInRoom();
  _AddSpriteShadows();
  if (*(int *)PTR__gCurrentGameMode_001c42d0 == 6) {
    _ProcessOneFrame(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    _ProcessOneFrame(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  }
  SVar4 = _SWStickyError();
  if (SVar4 != 0) {
    SVar4 = _SWStickyError();
    _MMFatalErrorCallback
              (SVar4,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
               PTR____FUNCTION___27422_001c4f48,0x8b);
  }
  return;
}


// ---- _DrawTileShadowsCallback @ 0x72a03 (1397 bytes)

void DrawTileShadowsCallback(SpriteWorldPtr param_1,undefined4 param_2,char param_3)

{
  char cVar1;
  int aiStack_54 [10];
  TileMapStructPtr pTStack_2c;
  short sStack_28;
  short sStack_26;
  short sStack_24;
  short sStack_22;
  short sStack_20;
  short sStack_1e;
  int iStack_1c;
  TileMapStruct *pTStack_18;
  TileMapStruct *pTStack_14;
  TileMapStruct *pTStack_10;
  
  if ((((param_3 != '\0') && (PTR__gLEVLStruct_001c4f1c[0x16] == '\0')) &&
      (*PTR__gDisableDynamicShadows_001c4f64 == '\0')) &&
     ((*(short *)PTR__gOldBackgroundPictResID_001c47b0 == 1 ||
      (PTR__gLEVLStruct_001c4f1c[0x15] != '\0')))) {
    for (sStack_28 = 1; sStack_28 < 4; sStack_28 = sStack_28 + 1) {
      pTStack_18 = param_1->tileLayerArray[sStack_28];
      for (sStack_26 = 0; sStack_26 < pTStack_18->numRows; sStack_26 = sStack_26 + 1) {
        for (sStack_24 = 0; sStack_24 < pTStack_18->numCols; sStack_24 = sStack_24 + 1) {
          sStack_22 = _ConvertTileID((int)param_1->tileLayerArray[4]->tileMap[sStack_26][sStack_24])
          ;
          if ((0 < sStack_22) && (sStack_22 < 0x1a3)) {
            pTStack_18->tileMap[sStack_26][sStack_24] = -1;
          }
        }
      }
    }
    for (sStack_1e = 0; sStack_1e < 10; sStack_1e = sStack_1e + 1) {
      aiStack_54[sStack_1e] = 0;
    }
    for (sStack_1e = 1; sStack_1e < 7; sStack_1e = sStack_1e + 1) {
      iStack_1c = _SWCloneTileMap(param_1->tileLayerArray[sStack_1e],&pTStack_2c);
      if (iStack_1c == 0) {
        aiStack_54[sStack_1e] = (int)param_1->tileLayerArray[sStack_1e];
        _SWInstallTileMap(param_1,pTStack_2c,sStack_1e);
      }
    }
    if (aiStack_54[4] != 0) {
      pTStack_14 = param_1->tileLayerArray[4];
      for (sStack_26 = 0; sStack_26 < pTStack_14->numRows; sStack_26 = sStack_26 + 1) {
        for (sStack_24 = 0; sStack_24 < pTStack_14->numCols; sStack_24 = sStack_24 + 1) {
          sStack_22 = pTStack_14->tileMap[sStack_26][sStack_24];
          sStack_20 = param_1->tileLayerArray[6]->tileMap[sStack_26][sStack_24];
          cVar1 = _IsWallTile((int)sStack_22);
          if (cVar1 != '\0') {
            cVar1 = _IsWallTile((int)sStack_20);
            if (cVar1 != '\0') {
              pTStack_14->tileMap[sStack_26][sStack_24] = -1;
            }
          }
        }
      }
    }
    for (sStack_1e = 1; sStack_1e < 7; sStack_1e = sStack_1e + 1) {
      if (aiStack_54[sStack_1e] != 0) {
        pTStack_10 = param_1->tileLayerArray[sStack_1e];
        for (sStack_26 = 0; sStack_26 < pTStack_10->numRows; sStack_26 = sStack_26 + 1) {
          for (sStack_24 = 0; sStack_24 < pTStack_10->numCols; sStack_24 = sStack_24 + 1) {
            sStack_22 = _ConvertTileID((int)pTStack_10->tileMap[sStack_26][sStack_24]);
            if ((0x2e8 < sStack_22) &&
               ((sStack_22 < 0x53f || ((0x59c < sStack_22 && (sStack_22 < 0x5a0)))))) {
              pTStack_10->tileMap[sStack_26][sStack_24] = -1;
            }
            if (*PTR__gSilverFloorIsActive_001c4958 == '\0') {
              if ((0x1ae < sStack_22) && (sStack_22 < 0x1bd)) {
                pTStack_10->tileMap[sStack_26][sStack_24] = -1;
              }
            }
            else if ((0x1bc < sStack_22) && (sStack_22 < 0x1cb)) {
              pTStack_10->tileMap[sStack_26][sStack_24] = -1;
            }
          }
        }
      }
    }
    _SWSetStdTileColor(param_1,0,0,0,0x5fff,1);
    param_1->tileOffsetV = -0x10;
    param_1->tileOffsetH = -0xc;
    for (sStack_28 = 1; sStack_28 < 7; sStack_28 = sStack_28 + 1) {
      _SWDrawTileLayerInRect(param_1,sStack_28,&param_1->visScrollRect,'\0','\x01');
    }
    for (sStack_1e = 0; sStack_1e < 7; sStack_1e = sStack_1e + 1) {
      if (aiStack_54[sStack_1e] != 0) {
        _SWDisposeTileMap(param_1->tileLayerArray + sStack_1e);
        param_1->tileLayerArray[sStack_1e] = (TileMapStruct *)aiStack_54[sStack_1e];
      }
    }
    param_1->tileOffsetV = 0;
    param_1->tileOffsetH = 0;
    _SWSetStdTileColor(param_1,0xffff,0xffff,0xffff,0xffff,0);
  }
  return;
}


// ---- _FixCandleStandsInRoom @ 0x72f78 (189 bytes)

void _FixCandleStandsInRoom(void)

{
  SpritePtr pSVar1;
  SpritePtr local_14;
  
  pSVar1 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gCandleHolderSpriteLayerP_001c417c,
                            (SpritePtr)0x0);
  while (local_14 = pSVar1, local_14 != (SpritePtr)0x0) {
    pSVar1 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gCandleHolderSpriteLayerP_001c417c,local_14);
    if (local_14->userData == 0xa8) {
      _SWRemoveSprite(local_14);
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gFloorCandleStandSpriteLayerP_001c4808,local_14);
      _SWCollideSpriteWithLayer
                (local_14,*(undefined4 *)PTR__gCandleSpriteLayerP_001c421c,PTR_LAB_001c4f50);
    }
  }
  return;
}


// ---- _SmallCandleCollideProc @ 0x73035 (221 bytes)

void SmallCandleCollideProc(SpritePtr param_1,SpritePtr param_2)

{
  short sVar1;
  short sVar2;
  SWMovementType SVar3;
  SWMovementType SVar4;
  
  if ((0xa3 < param_2->userData) && (param_2->userData < 0xa7)) {
    SVar3 = _SWGetSpriteHorizLoc(param_1);
    SVar4 = _SWGetSpriteHorizLoc(param_2);
    sVar1 = (short)(int)(SVar3 - SVar4);
    SVar3 = _SWGetSpriteVertLoc(param_1);
    SVar4 = _SWGetSpriteVertLoc(param_2);
    sVar2 = (short)(int)(SVar3 - SVar4);
    if ((10 < sVar2) && (((sVar2 < 0x14 && (sVar1 < 8)) && (-0x28 < sVar1)))) {
      _SWRemoveSprite(param_2);
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gFloorCandleStandSpriteLayerP_001c4808,param_2);
    }
  }
  return;
}


// ---- _RemoveAllSpriteGlowsAndShadows @ 0x73112 (254 bytes)

void _RemoveAllSpriteGlowsAndShadows(void)

{
  SpriteLayerPtr local_14;
  SpritePtr local_10;
  
  for (local_14 = (SpriteLayerPtr)**(undefined4 **)PTR__gSpriteWorldP_001c40d0;
      local_14 != (SpriteLayerPtr)0x0;
      local_14 = _SWGetNextSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_14)) {
    for (local_10 = _SWGetNextSprite(local_14,(SpritePtr)0x0); local_10 != (SpritePtr)0x0;
        local_10 = _SWGetNextSprite(local_14,local_10)) {
      if ((local_10->mountedSpriteP != (SpriteRec *)0x0) &&
         ((local_10->mountedSpriteP->userData == 0xcb ||
          ((local_10->mountedSpriteP->userData == local_10->userData &&
           (local_10->mountedSpriteP->drawData->colorRed == 0)))))) {
        _SWRemoveSprite(local_10->mountedSpriteP);
        _SWDisposeSprite(&local_10->mountedSpriteP);
        local_10->mountedSpriteP = (SpriteRec *)0x0;
      }
    }
  }
  return;
}


// ---- _AddSpriteShadows @ 0x73210 (400 bytes)

void _AddSpriteShadows(void)

{
  SWError SVar1;
  
  _AddGlowForKeysAndCoins();
  if (((PTR__gLEVLStruct_001c4f1c[0x16] == '\0') && (*PTR__gDisableDynamicShadows_001c4f64 == '\0'))
     && ((*(short *)PTR__gOldBackgroundPictResID_001c47b0 == 1 ||
         (PTR__gLEVLStruct_001c4f1c[0x15] != '\0')))) {
    _AddShadowForSpritesInLayer(*(undefined4 *)PTR__gCandleSpriteLayerP_001c421c);
    _AddShadowForSpritesInLayer(*(undefined4 *)PTR__gCandleHolderSpriteLayerP_001c417c);
    _AddShadowForSpritesInLayer(*(undefined4 *)PTR__gTrapDoorSpriteLayerP_001c40e0);
    _AddShadowForSpritesInLayer(*(undefined4 *)PTR__gOpenTrapDoorSpriteLayerP_001c47dc);
    _AddShadowForSpritesInLayer(*(undefined4 *)PTR__gDisappearingBlockSpriteLayerP_001c4930);
    _AddShadowForSpritesInLayer(*(undefined4 *)PTR__gVertMovingPlatformSpriteLayerP_001c423c);
    _AddShadowForSpritesInLayer(*(undefined4 *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c);
    _AddShadowForSpritesInLayer(*(undefined4 *)PTR__gFloorCandleStandSpriteLayerP_001c4808);
    _AddShadowForSpritesInLayerRange
              (*(undefined4 *)PTR__gSignSpriteLayerP_001c41f8,
               *(undefined4 *)PTR__gSavePedestalSpriteLayerP_001c4138);
    _AddShadowForSpritesInLayerRange
              (*(undefined4 *)PTR__gSnakeSpriteLayerP_001c4208,
               *(undefined4 *)PTR__gObjectSpriteLayerP_001c4268);
    _AddShadowForSpritesInLayerRange
              (*(undefined4 *)PTR__gMedusaSpriteLayerP_001c4094,
               *(undefined4 *)PTR__gDyingMedusaSpriteLayerP_001c49a8);
    SVar1 = _SWStickyError();
    if (SVar1 != 0) {
      SVar1 = _SWStickyError();
      _MMFatalErrorCallback
                (SVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
                 PTR____FUNCTION___27848_001c4f2c,0x1b5);
    }
  }
  return;
}


// ---- _AddShadowForSpritesInLayerRange @ 0x733a0 (86 bytes)

void _AddShadowForSpritesInLayerRange(SpriteLayerPtr param_1,SpriteLayerPtr param_2)

{
  SpriteLayerPtr local_10;
  
  local_10 = param_1;
  while( true ) {
    if (local_10 == (SpriteLayerPtr)0x0) {
      return;
    }
    _AddShadowForSpritesInLayer(local_10);
    if (local_10 == param_2) break;
    local_10 = _SWGetNextSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_10);
  }
  return;
}


// ---- _AddShadowForSpritesInLayer @ 0x733f6 (76 bytes)

void _AddShadowForSpritesInLayer(SpriteLayerPtr param_1)

{
  undefined4 local_10;
  
  for (local_10 = _SWGetNextSprite(param_1,(SpritePtr)0x0); local_10 != (SpritePtr)0x0;
      local_10 = _SWGetNextSprite(param_1,local_10)) {
    _AddShadowForSprite(local_10);
  }
  return;
}


// ---- _AddShadowForSprite @ 0x73442 (385 bytes)

void _AddShadowForSprite(SpriteRec *param_1)

{
  SpritePtr local_14;
  int local_10;
  
  local_10 = 0;
  if (((param_1->userData < 0x27) || (0x2c < param_1->userData)) &&
     ((param_1->userData < 0x20 || (0x21 < param_1->userData)))) {
    local_10 = _SWCloneSprite(param_1,&local_14,0,0);
    if (local_10 == 0) {
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gShadowSpriteLayerP_001c4978,local_14);
      _SWSetSpriteMoveProc(local_14,(MoveProcPtr_t)0x0);
      _SWSetSpriteFrameProc(local_14,(FrameProcPtr_t)0x0);
      _SWMountSprite(param_1,local_14,7.0,-5.0,'\x01','\0','\x01');
      _SWSetMountInheritsAttributes(param_1,'\0','\0','\x01','\x01','\x01','\x01');
      _SWSetSpriteColor(local_14,0,0,0);
      _SWSetSpriteTranslucency(local_14,0x5fff);
    }
  }
  return;
}


// ---- _AddGlowForKeysAndCoins @ 0x735c3 (157 bytes)

void _AddGlowForKeysAndCoins(void)

{
  SpritePtr local_10;
  
  for (local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gObjectSpriteLayerP_001c4268,
                                   (SpritePtr)0x0); local_10 != (SpritePtr)0x0;
      local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gObjectSpriteLayerP_001c4268,local_10)) {
    if (((0x26 < local_10->userData) && (local_10->userData < 0x2d)) ||
       ((0x1f < local_10->userData && (local_10->userData < 0x22)))) {
      _AddGlowForKeyOrCoin(local_10);
    }
  }
  return;
}


// ---- _AddGlowForKeyOrCoin @ 0x73660 (1191 bytes)

void _AddGlowForKeyOrCoin(SpriteRec *param_1)

{
  SpritePtr oldSpriteP;
  uint uVar1;
  
  if ((((param_1->userData != 0x2c) &&
       ((((PTR__gLEVLStruct_001c4f1c[0x18] == '\0' && (*PTR__gDisableKeyGlows_001c4f5c == '\0')) ||
         (param_1->userData < 0x27)) || (0x2c < param_1->userData)))) &&
      ((((PTR__gLEVLStruct_001c4f1c[0x17] == '\0' && (*PTR__gDisableCoinGlows_001c4f70 == '\0')) ||
        (param_1->userData < 0x20)) || (0x21 < param_1->userData)))) &&
     (oldSpriteP = (SpritePtr)_NewGlowSprite(0xcb), oldSpriteP != (SpritePtr)0x0)) {
    _SWRemoveSprite(oldSpriteP);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gKeyGlowSpriteLayerP_001c47d8,oldSpriteP);
    _SWMountSprite(param_1,oldSpriteP,7.0,-5.0,'\0','\0','\0');
    _SWSetMountInheritsAttributes(param_1,'\0','\0','\0','\0','\0','\x01');
    _SWSetSpriteBlendMode(oldSpriteP,0x302,1);
    _SWSetSpriteScaledSize(oldSpriteP,0,0);
    switch(param_1->userData) {
    case 0x20:
      _SWSetSpriteScaledSize(oldSpriteP,0,0);
      _SWSetSpriteTranslucency(oldSpriteP,0x132e);
      _SWSetSpriteColor(oldSpriteP,0xffff,0xffff,0);
      break;
    case 0x21:
      _SWSetSpriteScaledSize(oldSpriteP,0,0);
      _SWSetSpriteTranslucency(oldSpriteP,0x1519);
      _SWSetSpriteColor(oldSpriteP,0xffff,0xffff,0xffff);
      break;
    case 0x27:
      _SWSetSpriteTranslucency(oldSpriteP,0x2e08);
      _SWSetSpriteColor(oldSpriteP,0xffff,0x2fff,0x2fff);
      break;
    case 0x28:
      _SWSetSpriteTranslucency(oldSpriteP,0x450c);
      _SWSetSpriteColor(oldSpriteP,0x2fff,0x2fff,0xffff);
      break;
    case 0x29:
      _SWSetSpriteTranslucency(oldSpriteP,0x18ef);
      _SWSetSpriteColor(oldSpriteP,0,0xffff,0);
      break;
    case 0x2a:
      _SWSetSpriteTranslucency(oldSpriteP,0x33c9);
      _SWSetSpriteColor(oldSpriteP,0xffff,0,0xffff);
      break;
    case 0x2b:
      _SWSetSpriteTranslucency(oldSpriteP,0x1cc5);
      _SWSetSpriteColor(oldSpriteP,0xffff,0xffff,0);
    }
    _SWCenterSpriteOverSprite(oldSpriteP,param_1);
    _SWSetSpriteMountOffset
              (oldSpriteP,oldSpriteP->drawData->horizLoc - param_1->drawData->horizLoc,
               oldSpriteP->drawData->vertLoc - param_1->drawData->vertLoc);
    if ((0x26 < param_1->userData) && (param_1->userData < 0x2d)) {
      _SWSetSpriteFrameProc(oldSpriteP,(FrameProcPtr_t)PTR_KeyGlowFrameProc_001c4f54);
      _SWSetSpriteFrameTime(oldSpriteP,0);
      uVar1 = _SWGetSpriteTranslucencyLevel(oldSpriteP);
      oldSpriteP->misc1 = uVar1;
    }
  }
  return;
}


// ---- _ConvertTilesForCurRoom @ 0x73b07 (96 bytes)

void _ConvertTilesForCurRoom(void)

{
  undefined2 local_e;
  
  _ConvertTilesForCurRoomLayer4(4);
  _ConvertTilesForCurRoomLayer6(6);
  for (local_e = 0; local_e < 7; local_e = local_e + 1) {
    _SWResetTileMapForLayer(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_e);
  }
  return;
}


// ---- _ConvertTilesForCurRoomLayer4 @ 0x73b67 (846 bytes)

void _ConvertTilesForCurRoomLayer4(short param_1)

{
  short sVar1;
  short *psVar2;
  char cVar3;
  short sVar4;
  ushort local_2a;
  ushort local_28;
  short local_26;
  
  psVar2 = *(short **)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + param_1 * 4);
  local_2a = 0;
  do {
    if (*psVar2 <= (short)local_2a) {
      return;
    }
    for (local_28 = 0; (short)local_28 < psVar2[1]; local_28 = local_28 + 1) {
      local_26 = *(short *)(*(int *)(*(int *)(psVar2 + 2) + (short)local_2a * 4) +
                           (short)local_28 * 2);
      sVar4 = _GetEditorTileID(psVar2,(int)(short)local_2a,(int)(short)(local_28 - 1));
      cVar3 = _isNormalBrick((int)sVar4);
      if (cVar3 == '\0') {
        sVar4 = _GetEditorTileID(psVar2,(int)(short)local_2a,(int)(short)(local_28 + 1));
        cVar3 = _isNormalBrick((int)sVar4);
        if (cVar3 != '\0') goto LAB_00073c82;
        sVar4 = _GetEditorTileID(psVar2,(int)(short)(local_2a - 1),(int)(short)local_28);
        cVar3 = _isNormalBrick((int)sVar4);
        if (cVar3 != '\0') goto LAB_00073c82;
        sVar4 = _GetEditorTileID(psVar2,(int)(short)(local_2a + 1),(int)(short)local_28);
        cVar3 = _isNormalBrick((int)sVar4);
        if (cVar3 != '\0') goto LAB_00073c82;
      }
      else {
LAB_00073c82:
        if ((local_26 == 0x12f) || (local_26 == 0x13d)) {
          local_26 = 0x1e;
        }
        if ((local_26 == 0xe7) || (local_26 == 0xf5)) {
          local_26 = 1;
        }
        if ((local_26 == 0x10b) || (local_26 == 0x119)) {
          local_26 = 0xf;
        }
        if ((local_26 == 0x14b) || (local_26 == 0x159)) {
          local_26 = 0x2b;
        }
        if ((local_26 == 0x17b) || (local_26 == 0x189)) {
          local_26 = 0x39;
        }
      }
      if ((0 < local_26) && (local_26 < 0xd3)) {
        sVar4 = _GetRandom(0,0xb);
        sVar4 = sVar4 * 0xd2 + 0x5a1;
        local_26 = local_26 + sVar4;
        if (local_26 < *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe0)) {
          _MMDrawTileAndFringeCallback
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)param_1,(int)(short)local_2a,
                     (int)(short)local_28,(int)local_26);
          if (((((local_2a & 1) == 0) || ((local_28 & 1) != 0)) &&
              (((local_2a & 1) != 0 || ((local_28 & 1) == 0)))) &&
             ((short)local_28 + 1 < (int)psVar2[1])) {
            sVar1 = *(short *)(*(int *)(*(int *)(psVar2 + 2) + (short)local_2a * 4) +
                               (short)local_28 * 2 + 2);
            cVar3 = _isLeftBrick((int)local_26);
            if (cVar3 == '\0') {
LAB_00073e10:
              cVar3 = _isRightBrick((int)local_26);
              if (cVar3 != '\0') {
                cVar3 = _isLeftBrick((int)sVar1);
                if (cVar3 != '\0') goto LAB_00073e30;
              }
            }
            else {
              cVar3 = _isRightBrick((int)sVar1);
              if (cVar3 == '\0') goto LAB_00073e10;
LAB_00073e30:
              if (sVar1 < 0x5a2) {
                local_26 = sVar1 + sVar4;
              }
              _MMDrawTileAndFringeCallback
                        (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)param_1,
                         (int)(short)local_2a,(int)(short)(local_28 + 1),(int)local_26);
            }
          }
        }
      }
    }
    local_2a = local_2a + 1;
  } while( true );
}


// ---- _ConvertTilesForCurRoomLayer6 @ 0x73eb5 (1138 bytes)

void _ConvertTilesForCurRoomLayer6(short param_1)

{
  short *psVar1;
  char cVar2;
  short sVar3;
  bool bVar4;
  ushort local_2e;
  ushort local_2c;
  short local_2a;
  short local_28;
  
  psVar1 = *(short **)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + param_1 * 4);
  for (local_2e = 0; (short)local_2e < *psVar1; local_2e = local_2e + 1) {
    for (local_2c = 0; (short)local_2c < psVar1[1]; local_2c = local_2c + 1) {
      local_2a = *(short *)(*(int *)(*(int *)(psVar1 + 2) + (short)local_2e * 4) +
                           (short)local_2c * 2);
      if ((0 < local_2a) && (local_2a < 0xd3)) {
        sVar3 = _GetRandom(0,0xb);
        sVar3 = sVar3 * 0xd2 + 0x5a1;
        local_2a = local_2a + sVar3;
        if (local_2a < *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe0)) {
          _SWDrawTile(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,local_2e,local_2c,
                      local_2a);
          if ((((local_2e & 1) == 0) || ((local_2c & 1) != 0)) &&
             (((local_2e & 1) != 0 || ((local_2c & 1) == 0)))) {
            if ((short)local_2c + 1 < (int)psVar1[1]) {
              local_28 = *(short *)(*(int *)(*(int *)(psVar1 + 2) + (short)local_2e * 4) +
                                    (short)local_2c * 2 + 2);
              bVar4 = local_28 == -1;
              if (bVar4) {
                local_28 = _ConvertTileID((int)*(short *)(*(int *)(*(int *)(*(int *)(*(int *)(*(int 
                                                  *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + 0x10) + 4)
                                                  + (short)local_2e * 4) + (short)local_2c * 2 + 2))
                ;
              }
              cVar2 = _isLeftBrick((int)local_2a);
              if (((cVar2 != '\0') && (cVar2 = _isRightBrick((int)local_28), cVar2 != '\0')) ||
                 ((cVar2 = _isRightBrick((int)local_2a), cVar2 != '\0' &&
                  (cVar2 = _isLeftBrick((int)local_28), cVar2 != '\0')))) {
                if (local_28 < 0x5a2) {
                  local_2a = local_28 + sVar3;
                }
                if (bVar4) {
                  _MMDrawTileAndFringeCallback
                            (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,4,(int)(short)local_2e,
                             (int)(short)(local_2c + 1),(int)local_2a);
                }
                else {
                  _SWDrawTile(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,local_2e,
                              local_2c + 1,local_2a);
                  _UpgradeFringeForTile
                            (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)param_1,
                             (int)(short)local_2e,(int)(short)(local_2c + 1),(int)sVar3);
                }
              }
            }
          }
          else if (-1 < (short)local_2c + -1) {
            local_28 = *(short *)(*(int *)(*(int *)(psVar1 + 2) + (short)local_2e * 4) +
                                  (short)local_2c * 2 + -2);
            bVar4 = local_28 == -1;
            if (bVar4) {
              local_28 = _ConvertTileID((int)*(short *)(*(int *)(*(int *)(*(int *)(*(int *)(*(int *)
                                                  PTR__gSpriteWorldP_001c40d0 + 0xc4) + 0x10) + 4) +
                                                  (short)local_2e * 4) + (short)local_2c * 2 + -2));
            }
            cVar2 = _isLeftBrick((int)local_2a);
            if (((cVar2 != '\0') && (cVar2 = _isRightBrick((int)local_28), cVar2 != '\0')) ||
               ((cVar2 = _isRightBrick((int)local_2a), cVar2 != '\0' &&
                (cVar2 = _isLeftBrick((int)local_28), cVar2 != '\0')))) {
              if (local_28 < 0x5a2) {
                local_2a = local_28 + sVar3;
              }
              if (bVar4) {
                _SWDrawTile(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,4,local_2e,local_2c - 1,
                            local_2a);
                _UpgradeFringeForTile
                          (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,4,(int)(short)local_2e,
                           (int)(short)(local_2c - 1),(int)sVar3);
              }
            }
          }
        }
      }
    }
  }
  return;
}


// ---- _isBrickTile @ 0x74327 (97 bytes)

undefined1 isBrickTile(undefined4 param_1,short param_2,short param_3)

{
  short sVar1;
  undefined1 uStack_28;
  
  sVar1 = _GetEditorTileID(param_1,(int)param_2,(int)param_3);
  if ((sVar1 < 1) || (0xd2 < sVar1)) {
    uStack_28 = 0;
  }
  else {
    uStack_28 = 1;
  }
  return uStack_28;
}


// ---- _UpgradeFringeForTile @ 0x74388 (196 bytes)

void _UpgradeFringeForTile
               (undefined4 param_1,short param_2,short param_3,short param_4,short param_5)

{
  undefined2 local_12;
  undefined2 local_10;
  undefined2 local_e;
  
  if (param_2 == 6) {
    local_12 = 6;
    local_10 = 6;
  }
  else {
    local_12 = 1;
    local_10 = 3;
  }
  for (local_e = local_12; local_e <= local_10; local_e = local_e + 1) {
    _UpdateFringeForTileLoc
              (param_1,(int)local_e,(int)(short)(param_3 + -1),(int)param_4,(int)param_5);
    _UpdateFringeForTileLoc
              (param_1,(int)local_e,(int)param_3,(int)(short)(param_4 + 1),(int)param_5);
  }
  return;
}


// ---- _UpdateFringeForTileLoc @ 0x7444c (288 bytes)

void _UpdateFringeForTileLoc(int param_1,short param_2,short param_3,short param_4,short param_5)

{
  short sVar1;
  char cVar2;
  
  if ((((-1 < param_3) && (param_3 < **(short **)(*(int *)(param_1 + 0xc4) + param_2 * 4))) &&
      (-1 < param_4)) &&
     (((param_4 < *(short *)(*(int *)(*(int *)(param_1 + 0xc4) + param_2 * 4) + 2) &&
       (sVar1 = *(short *)(*(int *)(*(int *)(*(int *)(*(int *)(param_1 + 0xc4) + param_2 * 4) + 4) +
                                   param_3 * 4) + param_4 * 2), 0 < sVar1)) && (sVar1 < 0xd3)))) {
    cVar2 = _isTileFringe((int)sVar1);
    if (cVar2 != '\0') {
      _SWDrawTile(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_2,param_3,param_4,
                  param_5 + sVar1);
    }
  }
  return;
}


// ---- _DrawBackgroundTextureForCurRoom @ 0x7456c (985 bytes)

void _DrawBackgroundTextureForCurRoom(void)

{
  uchar visible;
  uchar visible_00;
  uchar visible_01;
  uchar visible_02;
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = _glGetError();
  if (iVar1 != 0) {
    uVar2 = _BKGetGLErrorMessage(iVar1);
    _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
             "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
             PTR____FUNCTION___28464_001c4f44,0x3a0,uVar2);
  }
  if (*(int *)PTR__gCurrentGameMode_001c42d0 == 5) {
    visible = *(uchar *)(*(int *)PTR__gCustomDialogSpriteLayerP_001c494c + 0x1c);
    visible_00 = *(uchar *)(*(int *)PTR__gOptionsDialogPane1LayerP_001c4970 + 0x1c);
    visible_01 = *(uchar *)(*(int *)PTR__gOptionsDialogPane2LayerP_001c4838 + 0x1c);
    visible_02 = *(uchar *)(*(int *)PTR__gPausedSpriteLayerP_001c47f4 + 0x1c);
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gCustomDialogSpriteLayerP_001c494c,'\0');
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gOptionsDialogPane1LayerP_001c4970,'\0');
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gOptionsDialogPane2LayerP_001c4838,'\0');
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gPausedSpriteLayerP_001c47f4,'\0');
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gShadowSpriteLayerP_001c4978,'\0');
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gHaloSpriteLayerP_001c41e0,'\0');
    _SWSetSpriteLayersVisible(*(undefined4 *)PTR__gBottomSpriteLayerP_001c4140,0,0);
    _SWSetSpriteLayersVisible
              (*(undefined4 *)PTR__gBackgroundScreenSpriteLayerP_001c4780,
               *(undefined4 *)PTR__gBackgroundPictureSpriteLayerP_001c475c,1);
    _SWSetTileLayersVisible(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0,6,1);
    _HideDisappearingFloorTiles();
    _SWSetTileLayerCallback(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0,PTR_LAB_001c4f74);
    _SWChangeTileImage(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x5a0,0);
    _SWChangeTileImage(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x5a1,0);
    _SWCreateBackgroundTextureFromTiles(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    _SWSetTileLayerCallback(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0,0);
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gShadowSpriteLayerP_001c4978,'\x01');
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gHaloSpriteLayerP_001c41e0,'\x01');
    _SWSetSpriteLayersVisible(*(undefined4 *)PTR__gBottomSpriteLayerP_001c4140,0,1);
    _SWSetSpriteLayersVisible
              (*(undefined4 *)PTR__gBackgroundScreenSpriteLayerP_001c4780,
               *(undefined4 *)PTR__gBackgroundPictureSpriteLayerP_001c475c,0);
    _SWSetTileLayersVisible(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0,5,0);
    _RestoreDisappearingFloorTiles();
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gCustomDialogSpriteLayerP_001c494c,visible);
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gOptionsDialogPane1LayerP_001c4970,visible_00)
    ;
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gOptionsDialogPane2LayerP_001c4838,visible_01)
    ;
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gPausedSpriteLayerP_001c47f4,visible_02);
    _SWChangeTileImage(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x5a0,0x5a0);
    _SWChangeTileImage(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x5a1,0x5a1);
  }
  return;
}


// ---- _HideDisappearingFloorTiles @ 0x74945 (313 bytes)

void _HideDisappearingFloorTiles(void)

{
  short local_e;
  
  for (local_e = 0x2e9; local_e < 0x2f1; local_e = local_e + 1) {
    *(undefined2 *)(PTR__saveCurTileArray_001c4f60 + local_e * 2) =
         *(undefined2 *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xd8) + local_e * 2);
    *(undefined2 *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xd8) + local_e * 2) = 0;
  }
  for (local_e = 0x2c1; local_e < 0x2c9; local_e = local_e + 1) {
    *(undefined2 *)(PTR__saveCurTileArray_001c4f60 + local_e * 2) =
         *(undefined2 *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xd8) + local_e * 2);
    *(undefined2 *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xd8) + local_e * 2) = 0;
  }
  for (local_e = 0x319; local_e < 0x331; local_e = local_e + 1) {
    *(undefined2 *)(PTR__saveCurTileArray_001c4f60 + local_e * 2) =
         *(undefined2 *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xd8) + local_e * 2);
    *(undefined2 *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xd8) + local_e * 2) = 0;
  }
  return;
}


// ---- _RestoreDisappearingFloorTiles @ 0x74a7e (238 bytes)

void _RestoreDisappearingFloorTiles(void)

{
  short local_e;
  
  for (local_e = 0x2e9; local_e < 0x2f1; local_e = local_e + 1) {
    _SWChangeTileImage(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_e,
                       *(short *)(PTR__saveCurTileArray_001c4f60 + local_e * 2));
  }
  for (local_e = 0x2c1; local_e < 0x2c9; local_e = local_e + 1) {
    _SWChangeTileImage(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_e,
                       *(short *)(PTR__saveCurTileArray_001c4f60 + local_e * 2));
  }
  for (local_e = 0x319; local_e < 0x331; local_e = local_e + 1) {
    _SWChangeTileImage(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_e,
                       *(short *)(PTR__saveCurTileArray_001c4f60 + local_e * 2));
  }
  return;
}


// ---- _CompareSpriteLeft @ 0x74b6c (36 bytes)

int CompareSpriteLeft(int param_1,int param_2)

{
  return (int)(short)(*(short *)(param_1 + 0x38) - *(short *)(param_2 + 0x38));
}


// ---- _LoadSpritesInRoom @ 0x74b90 (1697 bytes)

int _LoadSpritesInRoom(short param_1,char param_2)

{
  short sVar1;
  bool bVar2;
  char cVar3;
  undefined2 uVar4;
  int iVar5;
  undefined4 *puVar6;
  undefined4 uVar7;
  int iVar8;
  SWMovementType SVar9;
  SpritePtr local_2c;
  short local_1c;
  int local_18;
  undefined4 local_10;
  
  local_18 = 0;
  iVar5 = _GetSPRTResourceSize((int)param_1);
  puVar6 = (undefined4 *)_MMGetResource("SPRT",(int)param_1,iVar5 + 4);
  if (puVar6 == (undefined4 *)0x0) {
    local_18 = -1;
  }
  if (local_18 == 0) {
    iVar5 = _SDL_Swap32(*puVar6);
    for (local_1c = 0; local_1c < iVar5; local_1c = local_1c + 1) {
      uVar4 = _SDL_Swap16(*(undefined2 *)((int)puVar6 + local_1c * 0x16 + 4));
      *(undefined2 *)((int)puVar6 + local_1c * 0x16 + 4) = uVar4;
      uVar4 = _SDL_Swap16(*(undefined2 *)((int)puVar6 + local_1c * 0x16 + 6));
      *(undefined2 *)((int)puVar6 + local_1c * 0x16 + 6) = uVar4;
      uVar4 = _SDL_Swap16(*(undefined2 *)((int)puVar6 + local_1c * 0x16 + 8));
      *(undefined2 *)((int)puVar6 + local_1c * 0x16 + 8) = uVar4;
      uVar4 = _SDL_Swap16(*(undefined2 *)((int)puVar6 + local_1c * 0x16 + 10));
      *(undefined2 *)((int)puVar6 + local_1c * 0x16 + 10) = uVar4;
      uVar4 = _SDL_Swap16(*(undefined2 *)((int)puVar6 + local_1c * 0x16 + 0xc));
      *(undefined2 *)((int)puVar6 + local_1c * 0x16 + 0xc) = uVar4;
      uVar4 = _SDL_Swap16(*(undefined2 *)((int)puVar6 + local_1c * 0x16 + 0xe));
      *(undefined2 *)((int)puVar6 + local_1c * 0x16 + 0xe) = uVar4;
      uVar7 = _SDL_Swap32(*(undefined4 *)((int)puVar6 + local_1c * 0x16 + 0x10));
      *(undefined4 *)((int)puVar6 + local_1c * 0x16 + 0x10) = uVar7;
      uVar4 = _SDL_Swap16(*(undefined2 *)((int)puVar6 + local_1c * 0x16 + 0x18));
      *(undefined2 *)((int)puVar6 + local_1c * 0x16 + 0x18) = uVar4;
      sVar1 = *(short *)((int)puVar6 + local_1c * 0x16 + 4);
      if (((sVar1 < 0xe) || (0x11 < sVar1)) && ((sVar1 < 0x12 || (0x19 < sVar1)))) {
        iVar8 = local_1c * 0x16;
        uVar4 = _SDL_Swap16(*(undefined2 *)((int)puVar6 + iVar8 + 0x14));
        *(undefined2 *)((int)puVar6 + iVar8 + 0x14) = uVar4;
        uVar4 = _SDL_Swap16(*(undefined2 *)((int)puVar6 + iVar8 + 0x16));
        *(undefined2 *)((int)puVar6 + iVar8 + 0x16) = uVar4;
      }
      else {
        uVar7 = _SDL_Swap32(*(undefined4 *)((int)puVar6 + local_1c * 0x16 + 0x14));
        *(undefined4 *)((int)puVar6 + local_1c * 0x16 + 0x14) = uVar7;
      }
      bVar2 = false;
      cVar3 = _IsObjectInArray(*(undefined4 *)((int)puVar6 + local_1c * 0x16 + 0x10));
      if ((cVar3 == '\0') ||
         ((((*(short *)((int)puVar6 + local_1c * 0x16 + 4) < 1 ||
            (0xd < *(short *)((int)puVar6 + local_1c * 0x16 + 4))) &&
           ((*(short *)((int)puVar6 + local_1c * 0x16 + 4) < 0x1e ||
            (0x2c < *(short *)((int)puVar6 + local_1c * 0x16 + 4))))) &&
          (*(short *)((int)puVar6 + local_1c * 0x16 + 4) != 0xb8)))) {
LAB_00074f15:
        if ((*(short *)((int)puVar6 + local_1c * 0x16 + 4) == 0) && (param_2 == '\0')) {
          local_2c = (SpritePtr)0x0;
        }
        else {
          local_2c = (SpritePtr)
                     _CreateMySprite((int)*(short *)((int)puVar6 + local_1c * 0x16 + 4),
                                     (int)*(short *)((int)puVar6 + local_1c * 0x16 + 8),
                                     (int)*(short *)((int)puVar6 + local_1c * 0x16 + 6));
        }
        if (local_2c != (SpritePtr)0x0) {
          _SWSetSpriteLocation
                    (local_2c,(float)(int)*(short *)((int)puVar6 + local_1c * 0x16 + 8),
                     (float)(int)*(short *)((int)puVar6 + local_1c * 0x16 + 6));
          if (*(short *)((int)puVar6 + local_1c * 0x16 + 4) == 0) {
            _SaveHeroStartingLocation
                      ((int)*(short *)((int)puVar6 + local_1c * 0x16 + 8),
                       (int)*(short *)((int)puVar6 + local_1c * 0x16 + 6));
            *PTR__gStartingFacingRight_001c4c94 = local_2c->f_177;
            _UpdateHaloSpritePosition(local_2c);
            uVar4 = _GEGetLevelNumber((int)param_1,1,200);
            *(undefined2 *)PTR__gLastLevelPositionWasSaved_001c4c78 = uVar4;
            *(undefined2 *)PTR__gLastLevelExited_001c4d18 =
                 *(undefined2 *)PTR__gLastLevelPositionWasSaved_001c4c78;
            *(undefined2 *)PTR__gLastMusicIndex_001c4c80 =
                 *(undefined2 *)PTR__gCurrentMusicIndex_001c476c;
            SVar9 = _SWGetSpriteVertLoc(local_2c);
            local_2c->f_186 = (char)(short)(int)SVar9;
            local_2c->f_187 = (char)((ushort)(short)(int)SVar9 >> 8);
          }
          else if ((((*(short *)((int)puVar6 + local_1c * 0x16 + 4) == 0x9e) ||
                    (*(short *)((int)puVar6 + local_1c * 0x16 + 4) == 0x9f)) ||
                   (*(short *)((int)puVar6 + local_1c * 0x16 + 4) == 0xa0)) &&
                  (*(short *)((int)puVar6 + local_1c * 0x16 + 10) != 1)) {
            _LightTorch(local_2c);
          }
          _SetUpSpriteFields(local_2c,(int)puVar6 + local_1c * 0x16 + 4);
          _SetUpSpriteFrameRange(local_2c);
          if (bVar2) {
            if (local_2c->userData != 0xb) {
              (**(code **)PTR__gSWAssertFailProc_001c4170)
                        ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
                         PTR____FUNCTION___28618_001c4f40,0x4a8);
            }
            cVar3 = _IsSpriteInDoOnceArray(local_2c);
            if (cVar3 == '\0') {
              local_10 = _NewWebTrappedSprite(0xc4);
            }
            else {
              local_10 = _NewWebTrappedSprite(0xc3);
            }
            local_2c->f_10c = (undefined1)local_10;
            local_2c->f_10d = local_10._1_1_;
            local_2c->f_10e = local_10._2_1_;
            local_2c->f_10f = local_10._3_1_;
            local_2c->f_108 = '\x05';
            local_2c->f_109 = '\0';
            _MovePreyToSpiderLocation(local_10,local_2c);
            local_2c->f_110 = '\x0f';
            local_2c->f_111 = '\0';
            local_2c->f_112 = '\x0f';
            local_2c->f_113 = '\0';
          }
        }
      }
      else if (*(short *)((int)puVar6 + local_1c * 0x16 + 4) == 0xb) {
        bVar2 = true;
        goto LAB_00074f15;
      }
    }
    _free(puVar6);
  }
  return local_18;
}


// ---- _SDL_Swap16 @ 0x75231 (35 bytes)

undefined2 _SDL_Swap16(undefined2 param_1)

{
  return CONCAT11((char)param_1,(char)((ushort)param_1 >> 8));
}


// ---- _SDL_Swap32 @ 0x75254 (25 bytes)

uint _SDL_Swap32(uint param_1)

{
  return param_1 >> 0x18 | (param_1 & 0xff0000) >> 8 | (param_1 & 0xff00) << 8 | param_1 << 0x18;
}


// ---- _LoadLevelResIDFromGAMEResource @ 0x7526d (132 bytes)

int _LoadLevelResIDFromGAMEResource(void)

{
  undefined2 *puVar1;
  undefined4 uVar2;
  short local_e;
  
  local_e = -1;
  puVar1 = (undefined2 *)_MMGetResource("GAME",0x80,0x530);
  if (puVar1 != (undefined2 *)0x0) {
    local_e = _SDL_Swap16(*puVar1);
    uVar2 = _SDL_Swap32(*(undefined4 *)(puVar1 + 6));
    *(undefined4 *)PTR__gTotalSecretsInMansion_001c4884 = uVar2;
    _free(puVar1);
  }
  return (int)local_e;
}


// ---- _LoadRestOfGAMEResource @ 0x752f1 (165 bytes)

void _LoadRestOfGAMEResource(void)

{
  void *pvVar1;
  undefined4 uVar2;
  
  pvVar1 = (void *)_MMGetResource("GAME",0x80,0x530);
  if (pvVar1 != (void *)0x0) {
    uVar2 = _SDL_Swap32(*(undefined4 *)((int)pvVar1 + 0xc));
    *(undefined4 *)PTR__gTotalSecretsInMansion_001c4884 = uVar2;
    *(undefined2 *)PTR__gLavaSnd_001c474c = 0x26;
    *PTR__gDisableDynamicShadows_001c4f64 = *(undefined1 *)((int)pvVar1 + 4);
    *PTR__gDisableCoinGlows_001c4f70 = *(undefined1 *)((int)pvVar1 + 5);
    *PTR__gDisableKeyGlows_001c4f5c = *(undefined1 *)((int)pvVar1 + 6);
    _free(pvVar1);
  }
  return;
}


// ---- _LoadMansionNameFromGAMEResource @ 0x75396 (115 bytes)

undefined4 _LoadMansionNameFromGAMEResource(char *param_1)

{
  void *pvVar1;
  undefined4 local_10;
  
  local_10 = 0;
  pvVar1 = (void *)_MMGetResource("GAME",0x80,0x530);
  if (pvVar1 == (void *)0x0) {
    local_10 = 0xffffffff;
  }
  else {
    _SWCopyPascalString((char *)((int)pvVar1 + 0x30),param_1);
    _free(pvVar1);
  }
  return local_10;
}


// ---- _LoadLEVLResource @ 0x75409 (853 bytes)

void _LoadLEVLResource(short param_1)

{
  undefined *puVar1;
  undefined2 uVar2;
  char local_117 [255];
  undefined4 *local_18;
  short local_12;
  short local_10;
  short local_e;
  
  local_10 = _GEGetLevelNumber((int)param_1,1,200);
  local_12 = _GEGetLevelResID((int)local_10,1,200);
  local_18 = (undefined4 *)_MMGetResource("LEVL",(int)local_12,0x2c);
  puVar1 = PTR__gLEVLStruct_001c4f1c;
  if (local_18 == (undefined4 *)0x0) {
    _sprintf(local_117,"Error: Can\'t load LEVL resource for room resID %d. ",(int)param_1);
    _MMDoErrorMessage(local_117,0);
  }
  else {
    *(undefined4 *)PTR__gLEVLStruct_001c4f1c = *local_18;
    *(undefined4 *)(puVar1 + 4) = local_18[1];
    *(undefined4 *)(puVar1 + 8) = local_18[2];
    *(undefined4 *)(puVar1 + 0xc) = local_18[3];
    *(undefined4 *)(puVar1 + 0x10) = local_18[4];
    *(undefined4 *)(puVar1 + 0x14) = local_18[5];
    *(undefined4 *)(puVar1 + 0x18) = local_18[6];
    *(undefined4 *)(puVar1 + 0x1c) = local_18[7];
    *(undefined4 *)(puVar1 + 0x20) = local_18[8];
    *(undefined4 *)(puVar1 + 0x24) = local_18[9];
    *(undefined4 *)(puVar1 + 0x28) = local_18[10];
    uVar2 = _SDL_Swap16(*(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 2));
    *(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 2) = uVar2;
    uVar2 = _SDL_Swap16(*(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 4));
    *(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 4) = uVar2;
    uVar2 = _SDL_Swap16(*(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 6));
    *(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 6) = uVar2;
    uVar2 = _SDL_Swap16(*(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 8));
    *(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 8) = uVar2;
    uVar2 = _SDL_Swap16(*(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 10));
    *(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 10) = uVar2;
    uVar2 = _SDL_Swap16(*(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 0xc));
    *(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 0xc) = uVar2;
    uVar2 = _SDL_Swap16(*(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 0xe));
    *(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 0xe) = uVar2;
    uVar2 = _SDL_Swap16(*(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 0x10));
    *(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 0x10) = uVar2;
    uVar2 = _SDL_Swap16(*(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 0x12));
    *(undefined2 *)(PTR__gLEVLStruct_001c4f1c + 0x12) = uVar2;
    if (*(int *)PTR__gCurrentGameMode_001c42d0 != 6) {
      *PTR__gRoomIsDark_001c483c = *PTR__gLEVLStruct_001c4f1c;
      local_e = *(short *)(PTR__gLEVLStruct_001c4f1c + 2);
      if ((local_e < 1) || (local_e == *(short *)PTR__gCurrentMusicIndex_001c476c)) {
        if ((local_e == -1) && (*(short *)PTR__gCurrentMusicIndex_001c476c != -1)) {
          if (0 < *(short *)PTR__gCurrentMusicIndex_001c476c) {
            _DoGameFadeToBlack(0x1e,1);
          }
          _StopAndDisposeMusic();
          *(undefined2 *)PTR__gCurrentMusicIndex_001c476c = 0xffff;
        }
      }
      else {
        if ((0 < *(short *)PTR__gCurrentMusicIndex_001c476c) ||
           (*(short *)PTR__gCurrentMusicIndex_001c476c == -1)) {
          _DoGameFadeToBlack(0x1e,1);
        }
        _PlayBackgroundMusic((int)local_e);
      }
    }
    _free(local_18);
  }
  return;
}


// ---- _IsRoomDark @ 0x7575e (170 bytes)

undefined1 _IsRoomDark(short param_1)

{
  char local_113 [255];
  undefined1 *local_14;
  undefined1 local_d;
  
  local_d = 0;
  local_14 = (undefined1 *)_MMGetResource("LEVL",(int)param_1,0x2c);
  if (local_14 == (undefined1 *)0x0) {
    _sprintf(local_113,
             "Error: Can\'t load LEVL resource for levelResID %d, used by IsRoomDark() function.",
             (int)param_1);
    _MMDoErrorMessage(local_113,0);
  }
  else {
    local_d = *local_14;
    _free(local_14);
  }
  return local_d;
}


// ---- _IsRoomASecretRoom @ 0x75808 (171 bytes)

undefined1 _IsRoomASecretRoom(short param_1)

{
  char local_113 [255];
  void *local_14;
  undefined1 local_d;
  
  local_d = 0;
  local_14 = (void *)_MMGetResource("LEVL",(int)param_1,0x2c);
  if (local_14 == (void *)0x0) {
    _sprintf(local_113,
             "Error: Can\'t load LEVL resource for levelResID %d, used by IsRoomASecretRoom() function."
             ,(int)param_1);
    _MMDoErrorMessage(local_113,0);
  }
  else {
    local_d = *(undefined1 *)((int)local_14 + 1);
    _free(local_14);
  }
  return local_d;
}


// ---- _CreateMySprite @ 0x758b3 (985 bytes)

int _CreateMySprite(short param_1)

{
  int local_10;
  
  local_10 = 0;
  if (param_1 == 0) {
    local_10 = _NewMainSprite();
  }
  else if (param_1 == 1) {
    local_10 = _NewSkullSpiderSprite();
  }
  else if (param_1 == 2) {
    local_10 = _NewScorpionSprite();
  }
  else if (param_1 == 3) {
    local_10 = _NewEyeballSprite();
  }
  else if ((param_1 == 4) || (param_1 == 5)) {
    local_10 = _NewRavenSprite((int)param_1);
  }
  else if ((param_1 == 7) || (param_1 == 6)) {
    local_10 = _NewSnakeSprite((int)param_1);
  }
  else if ((param_1 == 8) || (param_1 == 9)) {
    local_10 = _NewMedusasHeadSprite((int)param_1);
  }
  else if ((param_1 == 0xb) || (param_1 == 10)) {
    local_10 = _NewSpiderSprite((int)param_1);
  }
  else if (param_1 == 0xc) {
    local_10 = _NewSkeletonFishSprite(0xc);
  }
  else if (param_1 == 0xd) {
    local_10 = _NewWalkingSkeletonSprite();
  }
  else if ((param_1 < 0xe) || (0x11 < param_1)) {
    if ((param_1 < 0x12) || (0x19 < param_1)) {
      if ((param_1 < 0x2d) || (0x44 < param_1)) {
        if ((param_1 < 0x45) || (0x4f < param_1)) {
          if ((param_1 < 0x50) || (0x5e < param_1)) {
            if ((param_1 == 0x5f) || (param_1 == 0x60)) {
              local_10 = _NewTrapDoorSprite((int)param_1);
            }
            else if ((param_1 < 0x1e) || (0x2c < param_1)) {
              if ((param_1 < 0x1a) || (0x1d < param_1)) {
                if (((param_1 < 0x70) || (0x84 < param_1)) && ((param_1 < 0x85 || (0x9d < param_1)))
                   ) {
                  if (((param_1 < 0x61) || (0x65 < param_1)) &&
                     ((param_1 < 0x66 || (0x6f < param_1)))) {
                    if (((param_1 == 0x9e) || (param_1 == 0x9f)) || (param_1 == 0xa0)) {
                      local_10 = _NewTorchSprite((int)param_1);
                    }
                    else if ((param_1 < 0xa1) || (0xa6 < param_1)) {
                      if ((param_1 < 0xa7) || (0xb0 < param_1)) {
                        if ((param_1 < 0xb1) || (0xb6 < param_1)) {
                          if (param_1 == 0xb7) {
                            local_10 = _NewExitSprite();
                          }
                          else if (param_1 == 0xb8) {
                            local_10 = _NewSecretIndicatorSprite();
                          }
                          else {
                            _printf("spriteID = %d",(int)param_1);
                            (**(code **)PTR__gSWAssertFailProc_001c4170)
                                      ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c"
                                       ,PTR____FUNCTION___29146_001c4f6c,0x65a);
                          }
                        }
                        else {
                          local_10 = _NewTreasureSprite((int)param_1);
                        }
                      }
                      else {
                        local_10 = _NewCandleHolderSprite((int)param_1);
                      }
                    }
                    else {
                      local_10 = _NewCandleSprite((int)param_1);
                    }
                  }
                  else {
                    local_10 = _NewPedestalOrStatueSprite((int)param_1);
                  }
                }
                else {
                  local_10 = _NewBackgroundSprite((int)param_1);
                }
              }
              else {
                local_10 = _NewZapperNodeSprite((int)param_1);
              }
            }
            else {
              local_10 = _NewObjectSprite((int)param_1);
            }
          }
          else {
            local_10 = _NewFallingPlatformSprite((int)param_1);
          }
        }
        else {
          local_10 = _NewMovingPlatformSprite((int)param_1);
        }
      }
      else {
        local_10 = _NewDoorSprite((int)param_1);
      }
    }
    else {
      local_10 = _NewBrickSwitchSprite((int)param_1);
    }
  }
  else {
    local_10 = _NewLeverSprite((int)param_1);
  }
  if (local_10 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
               PTR____FUNCTION___29146_001c4f6c,0x65e);
  }
  return local_10;
}


// ---- _MMLoadExtraBackFrameForRoom @ 0x75c8c (534 bytes)

void _MMLoadExtraBackFrameForRoom(undefined4 param_1,short param_2)

{
  SpritePtr pSVar1;
  undefined4 uVar2;
  int iVar3;
  char local_319 [255];
  char local_21a [255];
  char local_11b [255];
  short local_1c;
  short local_1a;
  undefined2 *local_18;
  char local_11;
  int local_10;
  
  local_10 = 0;
  local_18 = (undefined2 *)_MMGetResource("BGND",(int)param_2,0xc);
  if (local_18 == (undefined2 *)0x0) {
    _sprintf(local_319,
             "Error: Can\'t load LEVL resource for roomResID %d, used by MMLoadExtraBackFrameForRoom()."
             ,(int)param_2);
    _MMDoErrorMessage(local_319,0);
  }
  else {
    local_1c = _SDL_Swap16(*local_18);
    if (local_1c < 10000) {
      local_1c = 10000;
    }
    if ((*PTR__gUsingEditor_001c48b0 == '\0') || (*(int *)PTR__gCurrentGameMode_001c42d0 != 0x19)) {
      local_11 = _IsRoomDark((int)param_2);
    }
    else {
      local_11 = '\0';
    }
    if (local_11 != '\0') {
      if (PTR__gObjects_001c4910[0xe] == '\0') {
        local_1c = 10000;
      }
      _free(local_18);
    }
    local_1a = local_1c + -9999;
    if ((local_1a == *(short *)PTR__gOldBackgroundPictResID_001c47b0) &&
       (pSVar1 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gBackgroundScreenSpriteLayerP_001c4780,
                                  (SpritePtr)0x0), pSVar1 != (SpritePtr)0x0)) {
      return;
    }
    *(short *)PTR__gOldBackgroundPictResID_001c47b0 = local_1a;
    _sprintf(local_11b,"background%d.jpg",(int)local_1a);
    _sprintf(local_21a,"background%d.png",(int)local_1a);
    iVar3 = (int)local_1a;
    uVar2 = _GetScreensFolderPath();
    local_10 = _LoadAndInstallBackgroundSprite(0,uVar2,local_11b,iVar3,0);
    if (local_10 != 0) {
      iVar3 = (int)local_1a;
      uVar2 = _GetScreensFolderPath();
      _LoadAndInstallBackgroundSprite(0,uVar2,local_21a,iVar3,0);
    }
  }
  return;
}


// ---- _SetUpSpriteFields @ 0x75ea2 (3083 bytes)

void _SetUpSpriteFields(SpritePtr param_1,short *param_2)

{
  short sVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  uchar uVar6;
  short sVar7;
  SpritePtr srcSpriteP;
  undefined2 uVar8;
  longdouble lVar9;
  SWMovementType SVar10;
  SWMovementType horizLoc;
  short local_26;
  float local_24;
  SWMovementType local_20;
  SWMovementType local_1c;
  short local_12;
  
  sVar1 = *param_2;
  if ((sVar1 < 0) || (0xe3 < sVar1)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
               PTR____FUNCTION___29358_001c4f20,0x6b2);
  }
  uVar2 = *(undefined4 *)(param_2 + 6);
  param_1->f_0f8 = (char)uVar2;
  param_1->f_0f9 = (char)((uint)uVar2 >> 8);
  param_1->f_0fa = (char)((uint)uVar2 >> 0x10);
  param_1->f_0fb = (char)((uint)uVar2 >> 0x18);
  if (sVar1 != 0) {
    if ((sVar1 < 0xa1) || (0xa6 < sVar1)) {
      if (((sVar1 < 0xe) || (0x11 < sVar1)) && ((sVar1 < 0x12 || (0x19 < sVar1)))) {
        if ((sVar1 < 0x2d) || (0x44 < sVar1)) {
          if ((sVar1 < 0x70) || (0x84 < sVar1)) {
            if ((sVar1 < 0x85) || (0x9d < sVar1)) {
              if (((sVar1 < 0x27) || (0x2c < sVar1)) && ((sVar1 != 0x22 && (sVar1 != 0x23)))) {
                if (sVar1 == 0x24) {
                  _CreateGlowOverSprite(param_1);
                }
                else if (((sVar1 < 1) || (0xd < sVar1)) && ((sVar1 < 0x45 || (0x4f < sVar1)))) {
                  if ((sVar1 == 0x1a) || (sVar1 == 0x1d)) {
                    if (param_2[3] == -1) {
                      param_1->f_104 = '\x01';
                      param_1->f_100 = '\n';
                      param_1->f_101 = '\0';
                      param_1->f_0fc = 'x';
                      param_1->f_0fd = '\0';
                      param_1->f_0fe = 'x';
                      param_1->f_0ff = '\0';
                    }
                    else {
                      if ((param_2[3] == 0) || (*(int *)PTR__gCurrentGameMode_001c42d0 == 6)) {
                        param_1->f_104 = '\x01';
                        sVar1 = param_2[4];
                        param_1->f_100 = (char)sVar1;
                        param_1->f_101 = (char)((ushort)sVar1 >> 8);
                        _SWSetSpriteFrameAdvance(param_1,1);
                      }
                      else {
                        param_1->f_104 = '\0';
                        sVar1 = param_2[3];
                        param_1->f_100 = (char)sVar1;
                        param_1->f_101 = (char)((ushort)sVar1 >> 8);
                        _SWSetSpriteFrameAdvance(param_1,0);
                      }
                      sVar1 = param_2[4];
                      param_1->f_0fc = (char)sVar1;
                      param_1->f_0fd = (char)((ushort)sVar1 >> 8);
                      sVar1 = param_2[5];
                      param_1->f_0fe = (char)sVar1;
                      param_1->f_0ff = (char)((ushort)sVar1 >> 8);
                    }
                  }
                  else if ((sVar1 < 0xb1) || (0xb6 < sVar1)) {
                    if (sVar1 == 0x26) {
                      sVar1 = param_2[3];
                      param_1->f_10c = (char)sVar1;
                      param_1->f_10d = (char)((ushort)sVar1 >> 8);
                      sVar1 = param_2[4];
                      param_1->f_10e = (char)sVar1;
                      param_1->f_10f = (char)((ushort)sVar1 >> 8);
                      param_1->f_114 = param_2[5] != 1;
                      if (param_2[5] == 1) {
                        _SWSetSpriteVisible(param_1,'\0');
                      }
                      param_1->f_115 = param_2[10] == 1;
                      if ((param_1->f_115 != '\0') &&
                         (srcSpriteP = (SpritePtr)_NewMiscSprite(0xe1), srcSpriteP != (SpritePtr)0x0
                         )) {
                        SVar10 = _SWGetSpriteVertLoc(param_1);
                        horizLoc = _SWGetSpriteHorizLoc(param_1);
                        _SWSetSpriteLocation(srcSpriteP,horizLoc,SVar10);
                      }
                    }
                    else if (sVar1 == 0x25) {
                      sVar1 = param_2[3];
                      param_1->f_110 = (char)sVar1;
                      param_1->f_111 = (char)((ushort)sVar1 >> 8);
                      sVar1 = param_2[4];
                      param_1->f_112 = (char)sVar1;
                      param_1->f_113 = (char)((ushort)sVar1 >> 8);
                    }
                  }
                  else if (param_2[3] == 1) {
                    _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,param_1);
                  }
                  else {
                    _SWAddSprite(*(SpriteLayerPtr *)PTR__gForegroundSpriteLayerP_001c41f0,param_1);
                  }
                }
                else if (sVar1 == 0xc) {
                  SVar10 = _SWGetSpriteVertLoc(param_1);
                  param_1->f_0fc = (char)(short)(int)SVar10;
                  param_1->f_0fd = (char)((ushort)(short)(int)SVar10 >> 8);
                  sVar1 = param_2[3];
                  param_1->f_108 = (char)sVar1;
                  param_1->f_109 = (char)((ushort)sVar1 >> 8);
                  sVar1 = param_2[4];
                  param_1->f_106 = (char)sVar1;
                  param_1->f_107 = (char)((ushort)sVar1 >> 8);
                  *(float *)&param_1->f_10c = (float)(int)param_2[5] / 3.0;
                  *(float *)&param_1->f_110 = (float)(int)param_2[10] / 20.0;
                  sVar7._0_1_ = param_1->f_108;
                  sVar7._1_1_ = param_1->f_109;
                  if (sVar7 == 0) {
                    _StartSkeletonFishJump(param_1);
                  }
                }
                else if (sVar1 == 10) {
                  *(float *)&param_1->f_120 = (float)(int)param_2[3] / 100.0;
                  *(float *)&param_1->f_124 = (float)(int)param_2[4] / 20.0;
                  *(float *)&param_1->f_128 = (float)(int)param_2[5] / 20.0;
                  *(float *)&param_1->f_11c = (float)(int)param_2[10] / 6.0;
                  sVar1 = param_2[8];
                  param_1->f_12c = (char)(short)(int)(float)(int)sVar1;
                  param_1->f_12d = (char)((ushort)(short)(int)(float)(int)sVar1 >> 8);
                  *(float *)&param_1->f_130 = (float)(int)param_2[9] / 100.0;
                  if (1.0 < *(float *)&param_1->f_130) {
                    param_1->f_130 = '\0';
                    param_1->f_131 = '\0';
                    param_1->f_132 = 0x80;
                    param_1->f_133 = '?';
                  }
                  if (*(int *)PTR__gCurrentGameMode_001c42d0 == 5) {
                    uVar3._0_1_ = param_1->f_128;
                    uVar3._1_1_ = param_1->f_129;
                    uVar3._2_1_ = param_1->f_12a;
                    uVar3._3_1_ = param_1->f_12b;
                    uVar4._0_1_ = param_1->f_124;
                    uVar4._1_1_ = param_1->f_125;
                    uVar4._2_1_ = param_1->f_126;
                    uVar4._3_1_ = param_1->f_127;
                    lVar9 = (longdouble)_GetMovieCompatibleRandomFloat(uVar4,uVar3);
                    *(float *)&param_1->f_134 = (float)lVar9;
                  }
                  else {
                    uVar2._0_1_ = param_1->f_128;
                    uVar2._1_1_ = param_1->f_129;
                    uVar2._2_1_ = param_1->f_12a;
                    uVar2._3_1_ = param_1->f_12b;
                    uVar5._0_1_ = param_1->f_124;
                    uVar5._1_1_ = param_1->f_125;
                    uVar5._2_1_ = param_1->f_126;
                    uVar5._3_1_ = param_1->f_127;
                    lVar9 = (longdouble)_GetRandomFloat(uVar5,uVar2);
                    *(float *)&param_1->f_134 = (float)lVar9;
                  }
                }
                else if (sVar1 == 5) {
                  _SWSetSpriteMoveDelta(param_1,(float)(int)param_2[3],0.0);
                  *(double *)&param_1->f_114 = (double)(int)param_2[4];
                  *(double *)&param_1->f_11c = (double)(int)(param_2[5] / 5);
                  param_1->f_124 = (uchar)param_2[10];
                }
                else if ((sVar1 == 8) || (sVar1 == 9)) {
                  if (param_2[4] < 1) {
                    param_1->f_10c = 0xcd;
                    param_1->f_10d = 0xcc;
                    param_1->f_10e = 'L';
                    param_1->f_10f = '>';
                  }
                  else {
                    *(float *)&param_1->f_10c = (float)(int)param_2[4] / 20.0;
                  }
                }
                else if (sVar1 == 0xd) {
                  sVar1 = param_2[3];
                  param_1->f_110 = (char)sVar1;
                  param_1->f_111 = (char)((ushort)sVar1 >> 8);
                  sVar1 = param_2[4];
                  param_1->f_112 = (char)sVar1;
                  param_1->f_113 = (char)((ushort)sVar1 >> 8);
                  *(float *)&param_1->f_114 = (float)(int)param_2[5] / 2.0;
                  sVar1 = param_2[10];
                  param_1->f_10e = (char)sVar1;
                  param_1->f_10f = (char)((ushort)sVar1 >> 8);
                  uVar6 = param_1->f_111;
                  param_1->f_118 = param_1->f_110;
                  param_1->f_119 = uVar6;
                  sVar1._0_1_ = param_1->f_10e;
                  sVar1._1_1_ = param_1->f_10f;
                  if (0 < sVar1) {
                    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4f34);
                  }
                }
                else {
                  SVar10 = 0.0;
                  local_1c = 0.0;
                  local_26 = param_2[3];
                  local_24 = (float)(int)param_2[4];
                  if (local_26 == -1) {
                    local_26 = 2;
                  }
                  if (local_24 == -1.0) {
                    sVar7 = _GetDefaultSpriteSpeed((int)sVar1);
                    local_24 = (float)(int)sVar7;
                  }
                  if (sVar1 == 1) {
                    local_12 = (short)(int)(local_24 + 5.0);
                    if (0xb < local_12) {
                      local_12 = 0xc;
                    }
                    uVar8 = (undefined2)(0x3c / (longlong)(int)local_12);
                    param_1->f_10a = (char)uVar8;
                    param_1->f_10b = (char)((ushort)uVar8 >> 8);
                    uVar8 = (undefined2)(0x3c / (longlong)(int)local_12);
                    param_1->f_106 = (char)uVar8;
                    param_1->f_107 = (char)((ushort)uVar8 >> 8);
                    if (local_26 == 2) {
                      _SWSetSpriteFlipX(param_1,'\x01');
                    }
                    if (local_24 <= 2.0) {
                      param_1->f_104 = '*';
                      param_1->f_105 = '\0';
                    }
                    else if (local_24 == 3.0) {
                      param_1->f_104 = '+';
                      param_1->f_105 = '\0';
                    }
                    else {
                      param_1->f_104 = ',';
                      param_1->f_105 = '\0';
                    }
                  }
                  local_24 = local_24 / 2.0;
                  local_20 = local_24;
                  if (local_26 != 2) {
                    if (local_26 == 0) {
                      local_20 = -local_24;
                    }
                    else {
                      local_20 = SVar10;
                      if (local_26 == 1) {
                        local_1c = -local_24;
                      }
                      else if (local_26 == 3) {
                        local_1c = local_24;
                      }
                    }
                  }
                  _SWSetSpriteMoveDelta(param_1,local_20,local_1c);
                  if (((0x44 < sVar1) && (sVar1 < 0x50)) && (param_1->userData != 0x4f)) {
                    SVar10 = _SWGetSpriteHorizMoveDelta(param_1);
                    if ((SVar10 != 0.0) || (NAN(SVar10))) {
                      _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4f24);
                      _SWAddSprite(*(SpriteLayerPtr *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c
                                   ,param_1);
                    }
                    else {
                      _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4f78);
                      _SWAddSprite(*(SpriteLayerPtr *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,
                                   param_1);
                    }
                    param_1->f_104 = (uchar)param_2[5];
                  }
                }
              }
              else if (param_2[3] == 1) {
                _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_FallingObjectMoveProc_001c41b8);
              }
            }
            else if ((sVar1 < 0x94) || (0x9d < sVar1)) {
              if (param_2[3] == 1) {
                _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,param_1);
              }
              else {
                _SWAddSprite(*(SpriteLayerPtr *)PTR__gForegroundSpriteLayerP_001c41f0,param_1);
              }
            }
            else {
              _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,param_1);
            }
          }
          else if (((sVar1 < 0x70) || (0x73 < sVar1)) && (sVar1 != 0x74)) {
            if (param_2[3] == 1) {
              _SWAddSprite(*(SpriteLayerPtr *)PTR__gForegroundSpriteLayerP_001c41f0,param_1);
            }
            else {
              _SWAddSprite(*(SpriteLayerPtr *)PTR__gBackgroundSpriteLayerP_001c4070,param_1);
            }
          }
          else if (param_2[3] == 1) {
            _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,param_1);
          }
          else {
            _SWAddSprite(*(SpriteLayerPtr *)PTR__gForegroundSpriteLayerP_001c41f0,param_1);
          }
        }
        else {
          param_1->f_120 = param_2[3] == 1;
        }
      }
      else {
        sVar1 = param_2[3];
        param_1->f_0fc = (char)sVar1;
        param_1->f_0fd = (char)((ushort)sVar1 >> 8);
        uVar2 = *(undefined4 *)(param_2 + 8);
        param_1->f_100 = (char)uVar2;
        param_1->f_101 = (char)((uint)uVar2 >> 8);
        param_1->f_102 = (char)((uint)uVar2 >> 0x10);
        param_1->f_103 = (char)((uint)uVar2 >> 0x18);
        param_1->f_106 = (uchar)param_2[4];
        param_1->f_107 = (uchar)param_2[5];
        param_1->f_105 = (uchar)param_2[10];
      }
    }
    else {
      _CreateGlowOverSprite(param_1);
    }
  }
  return;
}


// ---- _SetUpSpriteFrameRange @ 0x76aad (179 bytes)

void _SetUpSpriteFrameRange(SpritePtr param_1)

{
  SWMovementType SVar1;
  
  if ((param_1->userData == 4) || (param_1->userData == 5)) {
    SVar1 = _SWGetSpriteHorizMoveDelta(param_1);
    if (0.0 < SVar1) {
      _SWSetSpriteFlipX(param_1,'\x01');
    }
  }
  else if ((param_1->userData == 2) && (SVar1 = _SWGetSpriteHorizMoveDelta(param_1), 0.0 < SVar1)) {
    _SWSetSpriteFlipX(param_1,'\x01');
  }
  return;
}


// ---- _GetDefaultSpriteSpeed @ 0x76b60 (232 bytes)

undefined2 _GetDefaultSpriteSpeed(short param_1)

{
  undefined2 local_e;
  
  if ((param_1 < 0x45) || (0x4f < param_1)) {
    switch(param_1) {
    default:
      local_e = 0;
      break;
    case 1:
      local_e = 3;
      break;
    case 2:
      local_e = 2;
      break;
    case 3:
      local_e = 3;
      break;
    case 4:
      local_e = 4;
      break;
    case 5:
      local_e = 2;
      break;
    case 6:
      local_e = 0;
      break;
    case 7:
      local_e = 0;
      break;
    case 8:
    case 9:
      local_e = 4;
      break;
    case 0xd:
      local_e = 2;
    }
  }
  else {
    local_e = 2;
  }
  return local_e;
}


// ---- _DisposeCurrentRoom @ 0x76c48 (344 bytes)

void _DisposeCurrentRoom(char param_1)

{
  SWError SVar1;
  short local_12;
  SpriteLayerPtr local_10;
  
  local_10 = (SpriteLayerPtr)0x0;
  while( true ) {
    local_10 = _SWGetNextSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_10);
    if (local_10 == (SpriteLayerPtr)0x0) break;
    if ((((local_10 != *(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164) &&
         (local_10 != *(SpriteLayerPtr *)PTR__gHeroOnPlatformSpriteLayerP_001c491c)) ||
        (param_1 == '\x01')) &&
       ((local_10 != *(SpriteLayerPtr *)PTR__gBackgroundScreenSpriteLayerP_001c4780 &&
        (local_10 != *(SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4)))) {
      _SWDisposeAllSpritesInLayer(local_10);
    }
  }
  _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gHiddenTriggerItemsSpriteLayerP_001c4864);
  for (local_12 = 0; local_12 < 7; local_12 = local_12 + 1) {
    if (*(int *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + local_12 * 4) != 0) {
      _SWDisposeTileMap((TileMapStructPtr *)
                        (*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + local_12 * 4));
    }
    _SWInstallTileMap(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,(TileMapStructPtr)0x0,local_12)
    ;
  }
  SVar1 = _SWStickyError();
  if (SVar1 != 0) {
    SVar1 = _SWStickyError();
    _MMFatalErrorCallback
              (SVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
               PTR____FUNCTION___29726_001c4f38,0x92f);
  }
  return;
}


// ---- _MoveLavaToLayer6 @ 0x76da0 (358 bytes)

undefined1 _MoveLavaToLayer6(void)

{
  short sVar1;
  short *psVar2;
  int iVar3;
  short local_1e;
  short local_1c;
  undefined1 local_d;
  
  psVar2 = *(short **)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + 0x10);
  iVar3 = *(int *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + 0x18);
  if (psVar2 == (short *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
               PTR____FUNCTION___29772_001c4f3c,0x943);
  }
  if (iVar3 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
               PTR____FUNCTION___29772_001c4f3c,0x944);
  }
  local_d = 0;
  for (local_1e = 0; local_1e < *psVar2; local_1e = local_1e + 1) {
    for (local_1c = 0; local_1c < psVar2[1]; local_1c = local_1c + 1) {
      sVar1 = *(short *)(*(int *)(*(int *)(psVar2 + 2) + local_1e * 4) + local_1c * 2);
      if ((0x308 < sVar1) && (sVar1 < 0x30d)) {
        *(undefined2 *)(*(int *)(*(int *)(psVar2 + 2) + local_1e * 4) + local_1c * 2) = 0xffff;
        *(short *)(*(int *)(*(int *)(iVar3 + 4) + local_1e * 4) + local_1c * 2) = sVar1;
        local_d = 1;
      }
    }
  }
  return local_d;
}


// ---- _AddGlowToLava @ 0x76f06 (715 bytes)

void _AddGlowToLava(void)

{
  short sVar1;
  short sVar2;
  short *psVar3;
  char cVar4;
  SpritePtr newSpriteP;
  short local_20;
  short local_1e;
  short local_1c;
  short local_18;
  short local_16;
  
  local_16 = 1;
  do {
    if (3 < local_16) {
      return;
    }
    psVar3 = *(short **)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + local_16 * 4);
    if (psVar3 == (short *)0x0) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
                 PTR____FUNCTION___29840_001c4f28,0x96a);
    }
    for (local_20 = 0; local_20 < *psVar3; local_20 = local_20 + 1) {
      for (local_1e = 0; local_1e < psVar3[1]; local_1e = local_1e + 1) {
        sVar1 = *(short *)(*(int *)(*(int *)(psVar3 + 2) + local_20 * 4) + local_1e * 2);
        if ((0x318 < sVar1) && (sVar1 < 0x331)) {
          if ((sVar1 < 0x319) || (800 < sVar1)) {
            if ((sVar1 < 0x321) || (0x328 < sVar1)) {
              local_18 = 1;
            }
            else {
              local_18 = 2;
              cVar4 = _IsWallTile((int)*(short *)(*(int *)(*(int *)(*(int *)(*(int *)(*(int *)
                                                  PTR__gSpriteWorldP_001c40d0 + 0xc4) + 0x10) + 4) +
                                                  local_20 * 4) + local_1e * 2));
              if (cVar4 != '\0') goto LAB_0007718d;
            }
          }
          else {
            local_18 = 0;
          }
          sVar1 = local_1e;
          if (local_18 == 1) {
            do {
              local_1c = sVar1 + 1;
              if ((psVar3[1] <= local_1c) ||
                 (sVar2 = *(short *)(*(int *)(*(int *)(psVar3 + 2) + local_20 * 4) + local_1c * 2),
                 sVar2 < 0x329)) break;
              sVar1 = local_1c;
            } while (sVar2 < 0x331);
          }
          newSpriteP = (SpritePtr)_NewMiscSprite(0xd4);
          _SWAddSprite(*(SpriteLayerPtr *)PTR__gLavaGlowSpriteLayerP_001c49d4,newSpriteP);
          _SWSetSpriteBlendMode(newSpriteP,0x302,1);
          _SWSetCurrentFrameIndex(newSpriteP,(int)local_18);
          _SWSetSpriteLocation
                    (newSpriteP,(float)((int)local_1e << 4),(float)(local_20 * 0x10 + -0x14));
          if (local_18 == 1) {
            _SWSetSpriteScaledWidth
                      (newSpriteP,SUB42((float)(((int)local_1c - (int)local_1e) * 0x10),0));
            local_1e = local_1c + -1;
          }
          _SWSetSpriteScaledHeightFactor(newSpriteP,0.58);
          if (local_18 == 0) {
            _SWOffsetSprite(newSpriteP,-16.0,0.0);
          }
        }
LAB_0007718d:
      }
    }
    local_16 = local_16 + 1;
  } while( true );
}


// ---- _OpenDoorsThatWereOpenedByPlayer @ 0x771d1 (222 bytes)

void _OpenDoorsThatWereOpenedByPlayer(void)

{
  char cVar1;
  SpritePtr pSVar2;
  SpritePtr local_14;
  
  local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,(SpritePtr)0x0);
  while (local_14 != (SpritePtr)0x0) {
    pSVar2 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,local_14);
    cVar1 = _IsSpriteInObjectArray(local_14);
    if (cVar1 != '\0') {
      _SWCollideSpriteWithLayer
                (local_14,*(undefined4 *)PTR__gDoorSpriteLayerP_001c41dc,
                 PTR_OpenDoorInstantlyCollideProc_001c4f14);
      _SetDoorToOpenedState(local_14);
    }
    local_14 = pSVar2;
    if ((pSVar2 != (SpritePtr)0x0) &&
       (pSVar2->parentSpriteLayerP != *(SpriteLayerRec **)PTR__gDoorSpriteLayerP_001c41dc)) {
      local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,(SpritePtr)0x0)
      ;
    }
  }
  return;
}


// ---- _OpenDoorsThatAreOpenToStart @ 0x772af (258 bytes)

void _OpenDoorsThatAreOpenToStart(void)

{
  char cVar1;
  SpritePtr pSVar2;
  SpritePtr local_14;
  
  local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,(SpritePtr)0x0);
  while (local_14 != (SpritePtr)0x0) {
    pSVar2 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,local_14);
    if ((local_14->f_120 != '\0') && (cVar1 = _IsSpriteInDoOnceArray(local_14), cVar1 == '\0')) {
      _SWCollideSpriteWithLayer
                (local_14,*(undefined4 *)PTR__gDoorSpriteLayerP_001c41dc,
                 PTR_OpenDoorInstantlyAndAddCollideProc_001c4f68);
      _SetDoorToOpenedState(local_14);
      _AddSpriteToObjectArray(local_14);
      _AddSpriteToDoOnceArray(local_14);
    }
    local_14 = pSVar2;
    if ((pSVar2 != (SpritePtr)0x0) &&
       (pSVar2->parentSpriteLayerP != *(SpriteLayerRec **)PTR__gDoorSpriteLayerP_001c41dc)) {
      local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,(SpritePtr)0x0)
      ;
    }
  }
  return;
}


// ---- _CreateGrayStatueOnSavePedestals @ 0x773b1 (327 bytes)

void _CreateGrayStatueOnSavePedestals(void)

{
  int iVar1;
  char cVar2;
  SpritePtr srcSpriteP;
  SWMovementType SVar3;
  SWMovementType SVar4;
  SpritePtr local_14;
  
  local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gSavePedestalSpriteLayerP_001c4138,
                              (SpritePtr)0x0);
  do {
    if (local_14 == (SpritePtr)0x0) {
      return;
    }
    cVar2 = _IsSpriteInObjectArray(local_14);
    if ((cVar2 != '\0') &&
       (iVar1._0_1_ = local_14->f_0f8, iVar1._1_1_ = local_14->f_0f9, iVar1._2_1_ = local_14->f_0fa,
       iVar1._3_1_ = local_14->f_0fb, iVar1 == *(int *)PTR__gLastSavePedestalObjectID_001c49f8)) {
      srcSpriteP = (SpritePtr)_NewStatueSprite();
      if (srcSpriteP != (SpritePtr)0x0) {
        SVar3 = _SWGetSpriteVertLoc(local_14);
        SVar4 = _SWGetSpriteHorizLoc(local_14);
        _SWSetSpriteLocation(srcSpriteP,SVar4 - 1.0,SVar3 - 59.0);
        _SWSetCurrentFrameIndex(srcSpriteP,1);
      }
    }
    cVar2 = _IsSpriteInObjectArray(local_14);
    if (cVar2 == '\0') {
      cVar2 = _IsSpriteInDoOnceArray(local_14);
      if (cVar2 != '\0') goto LAB_000774b9;
    }
    else {
LAB_000774b9:
      _SWSetCurrentFrameIndex(local_14,1);
    }
    local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gSavePedestalSpriteLayerP_001c4138,local_14)
    ;
  } while( true );
}


// ---- _MakeRoomDarkOrLight @ 0x774f8 (589 bytes)

void _MakeRoomDarkOrLight(char param_1,char param_2)

{
  short sVar1;
  char local_20;
  SpriteLayerPtr local_18;
  SpritePtr local_14;
  short local_10;
  
  local_20 = param_1;
  if (PTR__gObjects_001c4910[0xe] != '\0') {
    local_20 = '\0';
  }
  if (local_20 == '\x01') {
    for (local_10 = 1; local_10 < 0x5a2; local_10 = local_10 + 1) {
      if ((local_10 < 0x309) || (0x330 < local_10)) {
        _SWChangeTileImageFast(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0);
      }
    }
    for (local_18 = _SWGetNextSpriteLayer
                              (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,(SpriteLayerPtr)0x0);
        local_18 != (SpriteLayerPtr)0x0;
        local_18 = _SWGetNextSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_18)) {
      if (local_18 != *(SpriteLayerPtr *)PTR__gLanternGlowSpriteLayerP_001c4214) {
        for (local_14 = _SWGetNextSprite(local_18,(SpritePtr)0x0); local_14 != (SpritePtr)0x0;
            local_14 = _SWGetNextSprite(local_18,local_14)) {
          sVar1 = (short)local_14->userData;
          if ((((sVar1 != 0) && ((sVar1 < 1 || (0xd < sVar1)))) &&
              ((sVar1 < 0x1e || (0x2c < sVar1)))) &&
             ((((sVar1 != 0xb9 && (sVar1 != 0xba)) && (sVar1 != 0xbc)) && (sVar1 != 0xbd)))) {
            if ((sVar1 < 0x2d) || (0x44 < sVar1)) {
              _SWSetSpriteVisible(local_14,'\0');
            }
            else {
              _SWSetSpriteTranslucency(local_14,0);
            }
          }
          if (((sVar1 == 0x1e) || (sVar1 == 0x1f)) || (sVar1 == 0x26)) {
            _SWSetSpriteVisible(local_14,'\0');
          }
        }
      }
    }
  }
  else if ((local_20 == '\0') && (param_2 == '\x01')) {
    _SWResetVBOForAllTileLayers(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    _SWResetCurrentTileImages(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
    _ResetFloorTileAnimations(1,1);
    _HideLightSaberFloorsIfOff(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,1);
    _SetCorrectGoldSilverTileImages(1);
    _DrawBackgroundTextureForCurRoom();
  }
  *PTR__gRoomIsDark_001c483c = local_20;
  return;
}


// ---- _RestartLevel @ 0x77745 (239 bytes)

void _RestartLevel(void)

{
  SpritePtr oldSpriteP;
  
  *PTR__gHeroLandedThisFrame_001c4c70 = 0;
  if (*(short *)PTR__gNumLivesLeft_001c49e4 < 1) {
    *PTR__gGameOver_001c4718 = 1;
  }
  else {
    *(short *)PTR__gNumLivesLeft_001c49e4 = *(short *)PTR__gNumLivesLeft_001c49e4 + -1;
    oldSpriteP = (SpritePtr)_NewMainSprite();
    if (oldSpriteP != (SpritePtr)0x0) {
      _SWRemoveSprite(oldSpriteP);
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,oldSpriteP);
      _SWSetSpriteVisible(oldSpriteP,'\0');
      _SWSetSpriteMoveProc(oldSpriteP,(MoveProcPtr_t)0x0);
      _SWSetSpriteFrameProc(oldSpriteP,(FrameProcPtr_t)PTR_LAB_001c4f4c);
      _SWSetSpriteFrameTime(oldSpriteP,0);
      oldSpriteP->f_194 = '\x14';
      oldSpriteP->f_195 = '\0';
    }
  }
  return;
}


// ---- _LoadLevel @ 0x77834 (485 bytes)

void _LoadLevel(undefined2 param_1,undefined1 param_2)

{
  short sVar1;
  SWError SVar2;
  int iVar3;
  undefined4 uVar4;
  
  SVar2 = _SWStickyError();
  if (SVar2 != 0) {
    SVar2 = _SWStickyError();
    _MMFatalErrorCallback
              (SVar2,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
               PTR____FUNCTION___30111_001c4f58,0xaf6);
  }
  iVar3 = _glGetError();
  if (iVar3 != 0) {
    uVar4 = _BKGetGLErrorMessage(iVar3);
    _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
             "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
             PTR____FUNCTION___30111_001c4f58,0xafb,uVar4);
  }
  *(undefined2 *)PTR__gCurrentLevel_001c49bc = param_1;
  if (PTR__gObjects_001c4910[0xe] == '\0') {
    *PTR__gRoomWasDark_001c4814 = *PTR__gRoomIsDark_001c483c;
  }
  else {
    *PTR__gRoomWasDark_001c4814 = 0;
  }
  sVar1 = _GEGetLevelResID((int)*(short *)PTR__gCurrentLevel_001c49bc,1,200);
  _LoadLEVLResource((int)sVar1);
  _DisposeCurrentRoom(param_2);
  _LoadRoom((int)sVar1,param_2);
  if (*(int *)PTR__gCurrentGameMode_001c42d0 != 6) {
    _CreateFPSAndLevelResIDSprites();
  }
  *(undefined4 *)PTR__gNumFramesSinceEnteringRoom_001c47c4 = 0;
  if (*PTR__gDoFrameSkipping_001c47e8 != '\0') {
    *PTR__gDoFrameSkipping_001c47e8 = 0;
    _SWSetSpriteWorldMaxFPS(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x3c);
  }
  *PTR__gRemovedGlowsAndShadows_001c4948 = 0;
  SVar2 = _SWStickyError();
  if (SVar2 != 0) {
    SVar2 = _SWStickyError();
    _MMFatalErrorCallback
              (SVar2,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Level.c",
               PTR____FUNCTION___30111_001c4f58,0xb1e);
  }
  return;
}


// ---- _SetUpLevelForPlaying @ 0x77a19 (344 bytes)

void _SetUpLevelForPlaying(void)

{
  undefined1 uVar1;
  char cVar2;
  
  uVar1 = _MoveLavaToLayer6();
  *PTR__gCurrentRoomHasLava_001c4800 = uVar1;
  _AddGlowToLava();
  _ResetFloorTileAnimations(0,0);
  _DrawBackgroundTextureForCurRoom();
  cVar2 = _DoesLayerHaveTiles(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,6,0x59d,0x59f);
  if (cVar2 != '\0') {
    _SWSetTileLayerToDrawEveryFrame(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,6,1);
  }
  if (*PTR__gCurrentRoomHasLava_001c4800 == '\0') {
    _StopChannel(2);
  }
  else {
    cVar2 = _IsChannelAvailable(2);
    if (cVar2 != '\0') {
      _MMPlayStereoSound2(0,(int)*(short *)PTR__gLavaSnd_001c474c,2,2,
                          (int)(short)(int)((double)(int)*(short *)PTR__gSoundVolume_001c48dc * 0.85
                                           ),0x3ff0000000000000,0xffffffff);
    }
  }
  _StopMySound(0x32);
  _AddRoomToArray((int)*(short *)PTR__gCurrentLevel_001c49bc);
  return;
}


// ---- _DrawLevel @ 0x77b71 (56 bytes)

void _DrawLevel(void)

{
  _DrawStatsArea();
  _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0);
  return;
}


