// Ghidra decompilation of functions from original source file:
//   Sources & Headers/GameMap.c

// ---- _SetUpInGameMapScreen @ 0x877c2 (1669 bytes)

void _SetUpInGameMapScreen(void)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined4 uVar3;
  int iVar4;
  SWError SVar5;
  int iVar6;
  float local_18;
  
  _InitMapCache();
  *PTR__gUseOptimizedCaching_001c515c = 1;
  *PTR__gIsEnteringMapScreen_001c4aec = 1;
  _CalculateMapBounds();
  _StopLoopingSounds();
  _DisposeAllMapCachedRooms();
  _SWSetSpriteLayersVisible
            (*(undefined4 *)PTR__gBackgroundScreenSpriteLayerP_001c4780,
             *(undefined4 *)PTR__gBackgroundPictureSpriteLayerP_001c475c,1);
  *(undefined4 *)PTR__saveTileBackgroundP_001c4ff0 =
       *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x1c);
  *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x1c) = 0;
  if (*(int *)PTR__averageFrameRate_001c517c == -1) {
    uVar3 = _SWGetMilliseconds(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    *(undefined4 *)PTR__millisecsLastFrame_001c50f0 = uVar3;
    _UpdateCurRoomPreviewInMapCache(0,(int)*(short *)PTR__gCurrentLevel_001c49bc);
    iVar4 = _SWGetMilliseconds(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    *(int *)PTR__averageFrameRate_001c517c =
         (int)(((double)((iVar4 - *(int *)PTR__millisecsLastFrame_001c50f0) + -0x80000000) +
               2147483648.0) * 1.5);
  }
  else {
    _UpdateCurRoomPreviewInMapCache(0,(int)*(short *)PTR__gCurrentLevel_001c49bc);
  }
  *(undefined2 *)PTR__gCurrentScrollSpeed_001c5110 = 0;
  PTR__gKeys_001c4af8[8] = 0;
  iVar4 = _SaveAllSpritesToTempLayers(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  if (iVar4 == 0) {
    _MMFlushEvents();
    *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 6;
    _UpdateMenuBar();
    *(undefined2 *)PTR__gSaveCurConveyorBeltDirection_001c5150 =
         *(undefined2 *)PTR__gCurConveyorBeltDirection_001c470c;
    *(undefined2 *)PTR__gSaveOldConveyorBeltDirection_001c50fc =
         *(undefined2 *)PTR__gOldConveyorBeltDirection_001c4f18;
    *PTR__gSaveSilverFloorIsActive_001c5164 = *PTR__gSilverFloorIsActive_001c4958;
    *PTR__gSaveGoldFloorIsActive_001c5114 = *PTR__gGoldFloorIsActive_001c47a8;
    *(undefined2 *)PTR__gSaveOldBackgroundPictResID_001c5108 =
         *(undefined2 *)PTR__gOldBackgroundPictResID_001c47b0;
    *(undefined4 *)PTR__gSaveHiddenTriggerItemsSpriteLayerP_001c5104 =
         *(undefined4 *)PTR__gHiddenTriggerItemsSpriteLayerP_001c4864;
    SVar5 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gHiddenTriggerItemsSpriteLayerP_001c4864);
    if (SVar5 != 0) {
      _MMFatalErrorCallback
                (SVar5,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameMap.c",
                 PTR____FUNCTION___27456_001c5160,0xb0);
    }
    _SWSaveTileMaps(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    *(undefined2 *)PTR__gSavedLevelNumber_001c5138 = *(undefined2 *)PTR__gCurrentLevel_001c49bc;
    *(undefined2 *)PTR__lastLevelLoadedInAdvance_001c5180 = 0xffff;
    _MMPaintRect(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x74,PTR__gSDLBlackColor_001c514c,0xff);
    _KeepMusicPlaying();
    if (*(short *)PTR__gNumRoomsPerCol_001c5120 == 1) {
      *(undefined2 *)PTR__gNumRoomsPerCol_001c5120 = 3;
    }
    _ChangeStatsAreaForMapScreen();
    _ComputeMapRectForLevel((int)*(short *)PTR__gCurrentLevel_001c49bc,PTR__gMapScrollRect_001c516c)
    ;
    puVar2 = PTR__gKeysCopy_001c5174;
    puVar1 = PTR__gKeys_001c4af8;
    *(undefined4 *)PTR__gKeysCopy_001c5174 = *(undefined4 *)PTR__gKeys_001c4af8;
    *(undefined4 *)(puVar2 + 4) = *(undefined4 *)(puVar1 + 4);
    *(undefined2 *)(puVar2 + 8) = *(undefined2 *)(puVar1 + 8);
    puVar2[10] = puVar1[10];
    _ResetKeys();
    *(undefined2 *)PTR__gBottomPadding_001c5188 = 0;
    *(undefined2 *)PTR__gRightPadding_001c5140 = *(undefined2 *)PTR__gBottomPadding_001c5188;
    *(undefined2 *)PTR__gLeftPadding_001c50f4 = *(undefined2 *)PTR__gRightPadding_001c5140;
    *(undefined2 *)PTR__gTopPadding_001c50ec = *(undefined2 *)PTR__gLeftPadding_001c50f4;
    local_18 = 20.0;
    *(undefined2 *)PTR__gJacksHeadTranslucency_001c5170 = 0;
    _SetMapZoom(0x40200000);
    *(undefined2 *)PTR__numRoomsCanAddThisFrame_001c5124 = 9;
    _DrawMapScreenRooms(0);
    _SetMapZoom(0x3f800000);
    *(undefined2 *)PTR__numRoomsCanAddThisFrame_001c5124 = 0;
    while (*(float *)PTR__gCurMapZoom_001c5100 < 2.5) {
      iVar4 = _SWGetMilliseconds(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
      _SetMapZoom(((((2.5 - *(float *)PTR__gCurMapZoom_001c5100) / 2.0 + 1.8) / 1000.0) * local_18 +
                  1.0) * *(float *)PTR__gCurMapZoom_001c5100);
      _DrawMapScreen(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x74),
                     *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x78),
                     *(int *)PTR__gSpriteWorldP_001c40d0 + 0x74);
      *(short *)PTR__gJacksHeadTranslucency_001c5170 =
           (short)(int)(((*(float *)PTR__gCurMapZoom_001c5100 - 1.0) * 65535.0) / 1.5);
      if (2.0 < *(float *)PTR__gCurMapZoom_001c5100) {
        _ProcessSDLEvents();
      }
      if ((((2.0 < *(float *)PTR__gCurMapZoom_001c5100) && (PTR__gKeys_001c4af8[3] != '\0')) ||
          (PTR__gKeys_001c4af8[1] != '\0')) ||
         ((*PTR__gKeys_001c4af8 != '\0' || (PTR__gKeys_001c4af8[2] != '\0')))) break;
      iVar6 = _SWGetMilliseconds(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
      local_18 = (float)((uint)(iVar6 - iVar4) >> 0x10) * 65536.0 + (float)(iVar6 - iVar4 & 0xffff);
    }
    *(undefined2 *)PTR__gJacksHeadTranslucency_001c5170 = 0xffff;
    uVar3 = _SWGetMilliseconds(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    *(undefined4 *)PTR__millisecsLastFrame_001c50f0 = uVar3;
  }
  *PTR__gIsEnteringMapScreen_001c4aec = 0;
  *(undefined4 *)PTR__gScrollIncrease_001c5144 = 0;
  *PTR__gMapMouseButtonIsDown_001c5178 = 0;
  _MMSetCursorForSystem();
  _puts("Done initial map set-up.");
  return;
}


// ---- _ComputeMapRectForLevel @ 0x87e47 (131 bytes)

void _ComputeMapRectForLevel(short param_1,int *param_2)

{
  short local_10;
  short local_e [5];
  
  _GEGetRowAndColFromLevelNumber((int)param_1,local_e,&local_10,200);
  *param_2 = local_e[0] * 0x1b0;
  param_2[1] = local_10 * 0x280;
  param_2[3] = param_2[1] + 0x280;
  param_2[2] = *param_2 + 0x1b0;
  return;
}


// ---- _SetMapZoom @ 0x87eca (126 bytes)

void _SetMapZoom(float param_1)

{
  if (6.0 < param_1) {
    param_1 = 6.0;
  }
  else if (param_1 < 0.6) {
    param_1 = 0.6;
  }
  *(float *)PTR__gCurMapZoom_001c5100 = param_1;
  _SetRectZoom(PTR__gMapScrollRect_001c516c,param_1);
  return;
}


// ---- _SetRectZoom @ 0x87f48 (160 bytes)

void _SetRectZoom(int *param_1,float param_2)

{
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  
  local_1c = *param_1;
  local_18 = param_1[1];
  local_14 = (int)(param_2 * 432.0 + (float)local_1c);
  local_10 = (int)(param_2 * 640.0 + (float)local_18);
  _CenterLongRect(&local_1c,param_1);
  *param_1 = local_1c;
  param_1[1] = local_18;
  param_1[2] = local_14;
  param_1[3] = local_10;
  return;
}


// ---- _InitMapCache @ 0x87fe8 (105 bytes)

void _InitMapCache(void)

{
  short local_e;
  
  if (*PTR__wasInited_27603_001c50f8 == '\0') {
    for (local_e = 0; local_e < 200; local_e = local_e + 1) {
      *(undefined4 *)(PTR__gMapFrameArray_001c5154 + local_e * 4) = 0;
      *(undefined2 *)(PTR__gMapLevelNumber_001c5118 + local_e * 2) = 0xffff;
    }
    *PTR__wasInited_27603_001c50f8 = 1;
  }
  return;
}


// ---- _CalculateMapBounds @ 0x88051 (637 bytes)

void _CalculateMapBounds(void)

{
  short local_16;
  short local_14;
  short local_12;
  short local_10;
  short local_e;
  
  *(undefined2 *)PTR__gLeftMostRoom_001c511c = 200;
  *(undefined2 *)PTR__gTopMostRoom_001c5134 = 0xa0;
  *(undefined2 *)PTR__gRightMostRoom_001c5130 = 0;
  *(undefined2 *)PTR__gBottomMostRoom_001c5168 = 0;
  local_10 = _GetNumRoomsExplored();
  for (local_12 = 0; local_12 < local_10; local_12 = local_12 + 1) {
    local_e = _GetRoomArrayElement((int)local_12);
    _GEGetRowAndColFromLevelNumber((int)local_e,&local_14,&local_16,200);
    if (local_16 < *(short *)PTR__gLeftMostRoom_001c511c) {
      *(short *)PTR__gLeftMostRoom_001c511c = local_16;
    }
    if (*(short *)PTR__gRightMostRoom_001c5130 < local_16) {
      *(short *)PTR__gRightMostRoom_001c5130 = local_16;
    }
    if (local_14 < *(short *)PTR__gTopMostRoom_001c5134) {
      *(short *)PTR__gTopMostRoom_001c5134 = local_14;
    }
    if (*(short *)PTR__gBottomMostRoom_001c5168 < local_14) {
      *(short *)PTR__gBottomMostRoom_001c5168 = local_14;
    }
  }
  local_10 = _GetNumRoomsInMapArray();
  for (local_12 = 0; local_12 < local_10; local_12 = local_12 + 1) {
    local_e = _GetMapArrayElement((int)local_12);
    _GEGetRowAndColFromLevelNumber((int)local_e,&local_14,&local_16,200);
    if (local_16 < *(short *)PTR__gLeftMostRoom_001c511c) {
      *(short *)PTR__gLeftMostRoom_001c511c = local_16;
    }
    if (*(short *)PTR__gRightMostRoom_001c5130 < local_16) {
      *(short *)PTR__gRightMostRoom_001c5130 = local_16;
    }
    if (local_14 < *(short *)PTR__gTopMostRoom_001c5134) {
      *(short *)PTR__gTopMostRoom_001c5134 = local_14;
    }
    if (*(short *)PTR__gBottomMostRoom_001c5168 < local_14) {
      *(short *)PTR__gBottomMostRoom_001c5168 = local_14;
    }
  }
  *(short *)PTR__gLeftMostRoom_001c511c = *(short *)PTR__gLeftMostRoom_001c511c + -3;
  *(short *)PTR__gTopMostRoom_001c5134 = *(short *)PTR__gTopMostRoom_001c5134 + -3;
  *(short *)PTR__gRightMostRoom_001c5130 = *(short *)PTR__gRightMostRoom_001c5130 + 3;
  *(short *)PTR__gBottomMostRoom_001c5168 = *(short *)PTR__gBottomMostRoom_001c5168 + 3;
  return;
}


// ---- _ChangeStatsAreaForMapScreen @ 0x882ce (318 bytes)

void _ChangeStatsAreaForMapScreen(void)

{
  undefined1 local_22a [256];
  undefined2 local_12a;
  undefined2 local_128;
  undefined2 local_126;
  undefined1 local_124 [256];
  undefined2 local_24;
  undefined2 local_22;
  undefined1 local_20;
  undefined1 local_1f;
  undefined2 local_1e;
  undefined1 local_1c [4];
  short local_18;
  int local_14;
  SpritePtr local_10;
  
  local_12a = 0xffff;
  local_128 = 0xffff;
  local_126 = 0xffff;
  _LoadStatsBackFrame(0xc80);
  _HideStatsSprites();
  local_14 = _LoadMansionNameFromGAMEResource(local_22a);
  if (local_14 == 0) {
    _SetPascalString(local_124,"\rBlackChancery");
    local_24 = 1;
    local_22 = 0x24;
    local_20 = 1;
    local_1f = 1;
    local_1e = 2;
    _SWSetRect(local_1c,0,0,0x280,0x24);
    local_18 = local_18 + -2;
    local_10 = (SpritePtr)_MyDrawPascalTextJustified(local_22a,local_124,local_1c,&local_12a);
    if (local_10 != (SpritePtr)0x0) {
      _SWRemoveSprite(local_10);
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gStatsMapTextSpriteLayerP_001c4f90,local_10);
    }
  }
  _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gStatsSpriteWorldP_001c48f4,0);
  return;
}


// ---- _RunInGameMapScreen @ 0x8840c (1530 bytes)

void _RunInGameMapScreen(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined4 local_2a;
  undefined4 local_26;
  undefined2 local_22;
  undefined2 local_20;
  undefined2 local_1e;
  int local_1c;
  int local_18;
  undefined4 local_14;
  float local_10;
  
  local_26 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x78);
  local_2a = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x74);
  local_1c = _SWGetMilliseconds(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  local_18 = local_1c - *(int *)PTR__millisecsLastFrame_001c50f0;
  if ((*(int *)PTR__averageFrameRate_001c517c <= local_18) ||
     (((((PTR__gKeys_001c4af8[3] == '\0' && (PTR__gKeys_001c4af8[1] == '\0')) &&
        (*PTR__gKeys_001c4af8 == '\0')) &&
       ((PTR__gKeys_001c4af8[2] == '\0' && (PTR__gKeys_001c4af8[9] == '\0')))) &&
      (PTR__gKeys_001c4af8[10] == '\0')))) {
    *(int *)PTR__millisecsLastFrame_001c50f0 = local_1c;
    local_1e = *(undefined2 *)PTR__gCurrentLevel_001c49bc;
    local_14 = 0xffffffff;
    local_20 = 0;
    local_22 = 0;
    if (((PTR__gKeys_001c4af8[3] == '\0') && (PTR__gKeys_001c4af8[1] == '\0')) &&
       ((*PTR__gKeys_001c4af8 == '\0' && (PTR__gKeys_001c4af8[2] == '\0')))) {
      *(undefined4 *)PTR__gScrollIncrease_001c5144 = 0;
    }
    else if (*(float *)PTR__gScrollIncrease_001c5144 == 0.0) {
      *(undefined4 *)PTR__gScrollIncrease_001c5144 = 0x3f800000;
    }
    else {
      *(float *)PTR__gScrollIncrease_001c5144 = *(float *)PTR__gScrollIncrease_001c5144 + 0.03;
      if (1.8 < *(float *)PTR__gScrollIncrease_001c5144) {
        *(undefined4 *)PTR__gScrollIncrease_001c5144 = 0x3fe66666;
      }
    }
    if (*(int *)PTR__averageFrameRate_001c517c < 1) {
      *(short *)PTR__gCurrentScrollSpeed_001c5110 =
           (short)(int)((*(float *)PTR__gCurMapZoom_001c5100 * 15.0 + 15.0) / 2.0);
    }
    else {
      *(short *)PTR__gCurrentScrollSpeed_001c5110 =
           (short)(int)((((float)*(int *)PTR__averageFrameRate_001c517c / 2.2) *
                         *(float *)PTR__gCurMapZoom_001c5100 +
                        (float)*(int *)PTR__averageFrameRate_001c517c / 2.2) / 2.0);
    }
    if (PTR__gKeys_001c4af8[3] == '\0') {
      if (PTR__gKeys_001c4af8[1] != '\0') {
        _MoveMapScrollRect((int)(short)(int)(*(float *)PTR__gScrollIncrease_001c5144 *
                                            (float)(int)*(short *)PTR__gCurrentScrollSpeed_001c5110)
                           ,0);
      }
    }
    else {
      _MoveMapScrollRect((int)(short)(int)(*(float *)PTR__gScrollIncrease_001c5144 *
                                          (float)-(int)*(short *)PTR__gCurrentScrollSpeed_001c5110),
                         0);
    }
    if (*PTR__gKeys_001c4af8 == '\0') {
      if (PTR__gKeys_001c4af8[2] != '\0') {
        _MoveMapScrollRect(0,(int)(short)(int)(*(float *)PTR__gScrollIncrease_001c5144 *
                                              (float)(int)*(short *)
                                                  PTR__gCurrentScrollSpeed_001c5110));
      }
    }
    else {
      _MoveMapScrollRect(0,(int)(short)(int)(*(float *)PTR__gScrollIncrease_001c5144 *
                                            (float)-(int)*(short *)PTR__gCurrentScrollSpeed_001c5110
                                            ));
    }
    if (*(int *)PTR__averageFrameRate_001c517c < 1) {
      local_10 = 0.015;
    }
    else {
      local_10 = (float)*(int *)PTR__averageFrameRate_001c517c * 0.001;
    }
    if (PTR__gKeys_001c4af8[9] == '\0') {
      if (PTR__gKeys_001c4af8[10] != '\0') {
        _SetMapZoom((local_10 + 1.0) * *(float *)PTR__gCurMapZoom_001c5100);
      }
    }
    else {
      _SetMapZoom((1.0 - local_10) * *(float *)PTR__gCurMapZoom_001c5100);
    }
    _GetMapRoomWidth(&local_2a);
    *(undefined2 *)PTR__numRoomsCanAddThisFrame_001c5124 = 1;
    *PTR__gDoTrackTimeThisFrame_001c510c = 0;
    uVar1 = _SWGetMilliseconds(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    *(undefined4 *)PTR__millisecsLastFrame_001c50f0 = uVar1;
    _DrawMapScreen(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x74),
                   *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x78),
                   *(int *)PTR__gSpriteWorldP_001c40d0 + 0x74);
    iVar2 = _SWGetMilliseconds(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    if (*PTR__gDoTrackTimeThisFrame_001c510c != '\0') {
      if (*(int *)PTR__averageFrameRate_001c517c == -1) {
        *(int *)PTR__averageFrameRate_001c517c = iVar2 - *(int *)PTR__millisecsLastFrame_001c50f0;
      }
      else {
        iVar3 = *(int *)PTR__averageFrameRate_001c517c * 3 +
                (iVar2 - *(int *)PTR__millisecsLastFrame_001c50f0);
        iVar2 = iVar3 + 3;
        if (iVar2 < 0) {
          iVar2 = iVar3 + 6;
        }
        *(int *)PTR__averageFrameRate_001c517c = iVar2 >> 2;
      }
      if (200 < *(int *)PTR__averageFrameRate_001c517c) {
        *(undefined4 *)PTR__averageFrameRate_001c517c = 200;
      }
    }
  }
  return;
}


// ---- _MoveMapScrollRect @ 0x88a06 (207 bytes)

void _MoveMapScrollRect(short param_1,short param_2)

{
  _OffsetLongRect(PTR__gMapScrollRect_001c516c,(int)param_1,(int)param_2);
  if (param_1 < 0) {
    *(undefined2 *)PTR__gLeftPadding_001c50f4 = 3;
    *(undefined2 *)PTR__gRightPadding_001c5140 = 0;
  }
  else if (0 < param_1) {
    *(undefined2 *)PTR__gRightPadding_001c5140 = 3;
    *(undefined2 *)PTR__gLeftPadding_001c50f4 = 0;
  }
  if (param_2 < 0) {
    *(undefined2 *)PTR__gTopPadding_001c50ec = 3;
    *(undefined2 *)PTR__gBottomPadding_001c5188 = 0;
  }
  else if (0 < param_2) {
    *(undefined2 *)PTR__gBottomPadding_001c5188 = 3;
    *(undefined2 *)PTR__gTopPadding_001c50ec = 0;
  }
  return;
}


// ---- _HandleMapScreenMouseButton @ 0x88ad5 (105 bytes)

void _HandleMapScreenMouseButton
               (short param_1,undefined1 param_2,undefined2 param_3,undefined2 param_4)

{
  if (param_1 == 1) {
    *PTR__gMapMouseButtonIsDown_001c5178 = param_2;
    *(undefined2 *)PTR__gLastMouseLocX_001c5158 = param_3;
    *(undefined2 *)PTR__gLastMouseLocY_001c512c = param_4;
  }
  return;
}


// ---- _HandleMapScreenMouseMoved @ 0x88b3e (299 bytes)

void _HandleMapScreenMouseMoved(short param_1,short param_2)

{
  int iVar1;
  
  if (*PTR__gMapMouseButtonIsDown_001c5178 != '\0') {
    iVar1 = _SDL_GetMouseFocus();
    if (iVar1 == *(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158)) {
      _MoveMapScrollRect((int)(short)(int)((((float)((int)*(short *)PTR__gLastMouseLocX_001c5158 -
                                                    (int)param_1) *
                                            *(float *)PTR__gCurMapZoom_001c5100) /
                                           *(float *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3c)) *
                                          3.0),
                         (int)(short)(int)((((float)((int)*(short *)PTR__gLastMouseLocY_001c512c -
                                                    (int)param_2) *
                                            *(float *)PTR__gCurMapZoom_001c5100) /
                                           *(float *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3c)) *
                                          3.0));
      *(short *)PTR__gLastMouseLocX_001c5158 = param_1;
      *(short *)PTR__gLastMouseLocY_001c512c = param_2;
    }
  }
  return;
}


// ---- _HandleMapScreenScrollWheelEvent @ 0x88c69 (102 bytes)

void _HandleMapScreenScrollWheelEvent(float param_1)

{
  _SetMapZoom((param_1 / -40.0 + 1.0) * *(float *)PTR__gCurMapZoom_001c5100);
  return;
}


// ---- _DrawMapScreen @ 0x88ccf (292 bytes)

void _DrawMapScreen(void)

{
  undefined2 uVar1;
  
  uVar1 = *(undefined2 *)PTR__gCurrentLevel_001c49bc;
  if (0 < *(short *)PTR__numRoomsCanAddThisFrame_001c5124) {
    _DrawMapScreenRooms(0);
  }
  _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gStatsSpriteWorldP_001c48f4,0);
  _glViewport((int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x20),
              (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x22),
              (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x24),
              (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x26));
  _MMPaintRect(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c,PTR__gSDLBlackColor_001c514c,0xff);
  _DrawMapScreenRooms(1);
  *(undefined2 *)PTR__gCurrentLevel_001c49bc = uVar1;
  _SWPaintRectToSidesOfWorldRect(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  _SDL_GL_SwapWindow(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158));
  return;
}


// ---- _DrawMapScreenRooms @ 0x88df3 (1390 bytes)

void _DrawMapScreenRooms(char param_1)

{
  char cVar1;
  undefined2 uVar2;
  float local_68;
  float local_64;
  float local_60;
  float local_5c;
  undefined2 local_58;
  undefined2 local_56;
  undefined2 local_54;
  undefined2 local_52;
  float local_50;
  float local_4c;
  float local_48;
  float local_44;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  short local_16;
  int local_14;
  int local_10;
  
  local_30 = *(int *)PTR__gMapScrollRect_001c516c / 0x1b0;
  local_2c = *(int *)(PTR__gMapScrollRect_001c516c + 4) / 0x280;
  local_28 = (*(int *)(PTR__gMapScrollRect_001c516c + 8) + -1) / 0x1b0;
  local_24 = (*(int *)(PTR__gMapScrollRect_001c516c + 0xc) + -1) / 0x280;
  local_20 = local_30;
  do {
    if (local_28 < local_20) {
      return;
    }
    for (local_1c = local_2c; local_1c <= local_24; local_1c = local_1c + 1) {
      uVar2 = _GEGetLevelNumberFromRowAndCol((int)(short)local_20,(int)(short)local_1c,200);
      *(undefined2 *)PTR__gCurrentLevel_001c49bc = uVar2;
      local_16 = _GEGetLevelResID((int)*(short *)PTR__gCurrentLevel_001c49bc,1,200);
      cVar1 = _IsRoomInArray((int)*(short *)PTR__gCurrentLevel_001c49bc);
      if (cVar1 == '\0') {
        cVar1 = _IsRoomInMapArray((int)*(short *)PTR__gCurrentLevel_001c49bc);
        if (cVar1 != '\0') goto LAB_00088f51;
      }
      else {
LAB_00088f51:
        cVar1 = _ResourceExists("TMAP",(int)local_16);
        if (cVar1 != '\0') {
          local_40 = local_20 * 0x1b0 - *(int *)PTR__gMapScrollRect_001c516c;
          local_38 = (local_20 * 0x1b0 + 0x1b0) - *(int *)PTR__gMapScrollRect_001c516c;
          local_3c = local_1c * 0x280 - *(int *)(PTR__gMapScrollRect_001c516c + 4);
          local_34 = (local_1c * 0x280 + 0x280) - *(int *)(PTR__gMapScrollRect_001c516c + 4);
          local_50 = (float)local_40 / *(float *)PTR__gCurMapZoom_001c5100;
          local_4c = (float)local_3c / *(float *)PTR__gCurMapZoom_001c5100;
          local_44 = (float)local_34 / *(float *)PTR__gCurMapZoom_001c5100;
          local_48 = (float)local_38 / *(float *)PTR__gCurMapZoom_001c5100;
          if (param_1 == '\0') {
            if (0 < *(short *)PTR__numRoomsCanAddThisFrame_001c5124) {
              cVar1 = _AddMapRoomToCacheIfNecessary((int)*(short *)PTR__gCurrentLevel_001c49bc,0);
              if (cVar1 != '\0') {
                *(short *)PTR__numRoomsCanAddThisFrame_001c5124 =
                     *(short *)PTR__numRoomsCanAddThisFrame_001c5124 + -1;
              }
            }
          }
          else {
            _DrawMapRoomToScreen
                      ((int)*(short *)PTR__gCurrentLevel_001c49bc,local_50,local_4c,local_48,
                       local_44);
            if ((*(short *)PTR__gCurrentLevel_001c49bc == *(short *)PTR__gSavedLevelNumber_001c5138)
               && (1 < *(short *)PTR__gNumRoomsPerCol_001c5120)) {
              local_58 = (undefined2)(int)local_50;
              local_56 = (undefined2)(int)local_4c;
              local_54 = (undefined2)(int)local_48;
              local_52 = (undefined2)(int)local_44;
              local_14 = *(int *)(PTR__gSpriteArray_001c40d4 + 0x330);
              local_10 = **(int **)(local_14 + 0x78);
              local_68 = (float)(int)*(short *)(local_10 + 0x18);
              local_64 = (float)(int)*(short *)(local_10 + 0x1a);
              local_60 = ((float)(int)*(short *)(local_10 + 0x1c) - local_68) /
                         (*(float *)PTR__gCurMapZoom_001c5100 / 2.0);
              local_5c = ((float)(int)*(short *)(local_10 + 0x1e) - local_64) /
                         (*(float *)PTR__gCurMapZoom_001c5100 / 2.0);
              _CenterFloatRect(&local_68,&local_50);
              _CustomMapDrawProc(local_10,&local_68,0,2.0 / *(float *)PTR__gCurMapZoom_001c5100,
                                 *(undefined2 *)PTR__gJacksHeadTranslucency_001c5170);
            }
            if (*PTR__gShowSecrets_001c4878 != '\0') {
              cVar1 = _IsRoomASecretRoom((int)local_16);
              if (cVar1 != '\0') {
                _MMPaintFloatRect(&local_50,PTR__gSDLTealColor_001c41a0,0x50);
                goto LAB_00089336;
              }
            }
            cVar1 = _IsRoomInArray((int)*(short *)PTR__gCurrentLevel_001c49bc);
            if (cVar1 == '\0') {
              _MMPaintFloatRect(&local_50,PTR__gSDLBlackColor_001c514c,100);
            }
          }
        }
      }
LAB_00089336:
    }
    local_20 = local_20 + 1;
  } while( true );
}


// ---- _DrawMapRoomToScreen @ 0x89361 (84 bytes)

void _DrawMapRoomToScreen
               (short param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
               undefined4 param_5)

{
  short sVar1;
  
  sVar1 = _FindRoomInMapCache((int)param_1);
  if (sVar1 != -1) {
    _DrawCachedMapRoom((int)param_1,param_2,param_3,param_4,param_5);
  }
  return;
}


// ---- _AddMapRoomToCacheIfNecessary @ 0x893b5 (478 bytes)

undefined1 _AddMapRoomToCacheIfNecessary(short param_1,char param_2)

{
  undefined2 uVar1;
  CallBackPtr_t callBack;
  short sVar2;
  undefined1 local_11;
  
  local_11 = 0;
  uVar1 = *(undefined2 *)PTR__gCurrentLevel_001c49bc;
  sVar2 = _FindRoomInMapCache((int)param_1);
  if (sVar2 == -1) {
    *PTR__gDoTrackTimeThisFrame_001c510c = 1;
    if ((param_2 == '\0') ||
       ((param_2 != '\0' && (*(short *)PTR__lastLevelLoadedInAdvance_001c5180 != param_1)))) {
      _LoadLevel((int)param_1,0);
      *(short *)PTR__lastLevelLoadedInAdvance_001c5180 = param_1;
      *PTR__gSilverFloorIsActive_001c4958 = *PTR__gSaveSilverFloorIsActive_001c5164;
      *PTR__gGoldFloorIsActive_001c47a8 = *PTR__gSaveGoldFloorIsActive_001c5114;
      _SetCorrectGoldSilverTileImages(0);
      _ResetKeySpriteFrames();
      if (param_2 != '\0') {
        return 1;
      }
    }
    callBack = *(CallBackPtr_t *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x50);
    _SWSetPostEraseCallBack(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,(CallBackPtr_t)0x0);
    _SWSetSpriteWorldAutoScaling(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,'\0');
    _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0);
    _AddCurrentRoomToMapCache
              (*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x8c),
               *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x90),(int)param_1);
    _SWSetSpriteWorldAutoScaling(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,'\x01');
    _SWSetPostEraseCallBack(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,callBack);
    local_11 = 1;
  }
  *(undefined2 *)PTR__gCurrentLevel_001c49bc = uVar1;
  return local_11;
}


// ---- _calculateNumRoomsInMapCache @ 0x89593 (80 bytes)

int calculateNumRoomsInMapCache(void)

{
  short sStack_10;
  short sStack_e;
  
  sStack_e = 0;
  for (sStack_10 = 0; sStack_10 < 200; sStack_10 = sStack_10 + 1) {
    if (*(short *)(PTR__gMapLevelNumber_001c5118 + sStack_10 * 2) != -1) {
      sStack_e = sStack_e + 1;
    }
  }
  return (int)sStack_e;
}


// ---- _FindRoomInMapCache @ 0x895e3 (90 bytes)

int _FindRoomInMapCache(short param_1)

{
  short local_e;
  
  local_e = 0;
  while( true ) {
    if (199 < local_e) {
      return -1;
    }
    if (*(short *)(PTR__gMapLevelNumber_001c5118 + local_e * 2) == param_1) break;
    local_e = local_e + 1;
  }
  return (int)local_e;
}


// ---- _FindEmptyMapCacheElement @ 0x8963d (83 bytes)

int _FindEmptyMapCacheElement(void)

{
  short local_e;
  
  local_e = 0;
  while( true ) {
    if (199 < local_e) {
      return -1;
    }
    if (*(short *)(PTR__gMapLevelNumber_001c5118 + local_e * 2) == -1) break;
    local_e = local_e + 1;
  }
  return (int)local_e;
}


// ---- _DeleteCachedRoomsOutsideCurrentView @ 0x89690 (142 bytes)

void DeleteCachedRoomsOutsideCurrentView(void)

{
  char cVar1;
  short sStack_e;
  
  for (sStack_e = 0; sStack_e < 200; sStack_e = sStack_e + 1) {
    if (*(short *)(PTR__gMapLevelNumber_001c5118 + sStack_e * 2) != -1) {
      cVar1 = _IsRoomInsideCurrentView
                        ((int)*(short *)(PTR__gMapLevelNumber_001c5118 + sStack_e * 2));
      if ((cVar1 == '\0') &&
         (*(short *)(PTR__gMapLevelNumber_001c5118 + sStack_e * 2) !=
          *(short *)PTR__gSavedLevelNumber_001c5138)) {
        _DisposeMapCachedRoom((int)sStack_e);
      }
    }
  }
  return;
}


// ---- _getIndexOfMapCachedRoomOutsideCurView @ 0x8971e (149 bytes)

int _getIndexOfMapCachedRoomOutsideCurView(void)

{
  char cVar1;
  short local_e;
  
  local_e = 0;
  while( true ) {
    if (199 < local_e) {
      return -1;
    }
    if (((*(short *)(PTR__gMapLevelNumber_001c5118 + local_e * 2) !=
          *(short *)PTR__gSavedLevelNumber_001c5138) &&
        (*(short *)(PTR__gMapLevelNumber_001c5118 + local_e * 2) != -1)) &&
       (cVar1 = _IsRoomInsideCurrentView
                          ((int)*(short *)(PTR__gMapLevelNumber_001c5118 + local_e * 2)),
       cVar1 == '\0')) break;
    local_e = local_e + 1;
  }
  return (int)local_e;
}


// ---- _IsRoomInsideCurrentView @ 0x897b3 (352 bytes)

undefined4 _IsRoomInsideCurrentView(short param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  short sVar7;
  int local_18;
  int local_14;
  
  local_18 = *(int *)PTR__gMapScrollRect_001c516c / 0x1b0 - (int)*(short *)PTR__gTopPadding_001c50ec
  ;
  iVar4 = *(int *)(PTR__gMapScrollRect_001c516c + 4);
  sVar1 = *(short *)PTR__gLeftPadding_001c50f4;
  iVar5 = *(int *)(PTR__gMapScrollRect_001c516c + 8);
  sVar2 = *(short *)PTR__gBottomPadding_001c5188;
  iVar6 = *(int *)(PTR__gMapScrollRect_001c516c + 0xc);
  sVar3 = *(short *)PTR__gRightPadding_001c5140;
  do {
    local_14 = iVar4 / 0x280 - (int)sVar1;
    if ((iVar5 + -1) / 0x1b0 + (int)sVar2 < local_18) {
      return 0;
    }
    for (; local_14 <= (iVar6 + -1) / 0x280 + (int)sVar3; local_14 = local_14 + 1) {
      sVar7 = _GEGetLevelNumberFromRowAndCol((int)(short)local_18,(int)(short)local_14,200);
      if (sVar7 == param_1) {
        return 1;
      }
    }
    local_18 = local_18 + 1;
  } while( true );
}


// ---- _UpdateCurRoomPreviewInMapCache @ 0x89913 (507 bytes)

void _UpdateCurRoomPreviewInMapCache(SpritePtr param_1,short param_2)

{
  CallBackPtr_t callBack;
  
  if (param_1 == (SpritePtr)0x0) {
    param_1 = (SpritePtr)_GetHeroSprite();
  }
  if (param_1 == (SpritePtr)0x0) {
    param_1 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,(SpritePtr)0x0
                              );
  }
  _SWSetSpritesInLayerVisible(*(undefined4 *)PTR__gFPSDisplaySpriteLayerP_001c41a8,0);
  _SWSetSpritesInLayerVisible(*(undefined4 *)PTR__gHaloSpriteLayerP_001c41e0,0);
  if (param_1 != (SpritePtr)0x0) {
    _SWSetSpriteVisible(param_1,'\0');
  }
  _SWSetSpriteWorldAutoScaling(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,'\0');
  callBack = *(CallBackPtr_t *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x50);
  _SWSetPostEraseCallBack(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,(CallBackPtr_t)0x0);
  _SWSetTileLayersVisible(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0,6,1);
  _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0);
  _AddCurrentRoomToMapCache
            (*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x74),
             *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x78),(int)param_2);
  _SWSetSpriteWorldAutoScaling(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,'\x01');
  _SWSetTileLayersVisible(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0,5,0);
  _SWSetSpritesInLayerVisible(*(undefined4 *)PTR__gHaloSpriteLayerP_001c41e0,1);
  _SWSetSpritesInLayerVisible(*(undefined4 *)PTR__gFPSDisplaySpriteLayerP_001c41a8,1);
  _SWSetPostEraseCallBack(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,callBack);
  if (param_1 != (SpritePtr)0x0) {
    _SWSetSpriteVisible(param_1,'\x01');
  }
  return;
}


// ---- _AddCurrentRoomToMapCache @ 0x89b0e (416 bytes)

bool _AddCurrentRoomToMapCache(undefined4 param_1,undefined4 param_2,short param_3)

{
  bool bVar1;
  undefined4 local_18;
  short local_14;
  short local_12;
  int local_10;
  
  local_10 = 0;
  _InitMapCache();
  local_14 = _FindRoomInMapCache((int)param_3);
  if (local_14 == -1) {
    if (*PTR__gUseOptimizedCaching_001c515c == '\0') {
      local_12 = -1;
    }
    else {
      local_12 = _getIndexOfMapCachedRoomOutsideCurView();
    }
    if (local_12 == -1) {
      local_14 = _FindEmptyMapCacheElement();
      if (local_14 == -1) {
        local_10 = -1;
      }
    }
    if (local_10 == 0) {
      if (local_12 == -1) {
        if (199 < local_14) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameMap.c",
                     PTR____FUNCTION___28339_001c5148,0x4a5);
        }
        local_10 = _SWCreateFrameFromWorkArea
                             (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_18,
                              *(int *)PTR__gSpriteWorldP_001c40d0 + 0x8c);
        if (local_10 == 0) {
          *(short *)(PTR__gMapLevelNumber_001c5118 + local_14 * 2) = param_3;
          *(undefined4 *)(PTR__gMapFrameArray_001c5154 + local_14 * 4) = local_18;
        }
      }
      else {
        local_18 = *(undefined4 *)(PTR__gMapFrameArray_001c5154 + local_12 * 4);
        _SWUpdateFrameFromWorkArea
                  (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,local_18,
                   *(int *)PTR__gSpriteWorldP_001c40d0 + 0x8c);
        *(short *)(PTR__gMapLevelNumber_001c5118 + local_12 * 2) = param_3;
      }
    }
    bVar1 = local_10 == 0;
  }
  else {
    _UpdateExistingRoomInMapCache((int)local_14);
    bVar1 = true;
  }
  return bVar1;
}


// ---- _UpdateExistingRoomInMapCache @ 0x89cae (281 bytes)

void _UpdateExistingRoomInMapCache(short param_1)

{
  undefined4 local_14;
  int local_10;
  
  if (param_1 < 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameMap.c",
               PTR____FUNCTION___28386_001c5184,0x4cc);
  }
  if (199 < param_1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameMap.c",
               PTR____FUNCTION___28386_001c5184,0x4cd);
  }
  _SWSetSpriteWorldAutoScaling(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,'\0');
  local_10 = _SWCreateFrameFromWorkArea
                       (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_14,
                        *(int *)PTR__gSpriteWorldP_001c40d0 + 0x8c);
  _SWSetSpriteWorldAutoScaling(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,'\x01');
  if (local_10 == 0) {
    _SWDisposeFrame((FramePtr *)(PTR__gMapFrameArray_001c5154 + param_1 * 4));
    *(undefined4 *)(PTR__gMapFrameArray_001c5154 + param_1 * 4) = local_14;
  }
  return;
}


// ---- _DrawCachedMapRoom @ 0x89dc7 (304 bytes)

void _DrawCachedMapRoom(short param_1)

{
  short sVar1;
  
  sVar1 = _FindRoomInMapCache((int)param_1);
  if (199 < sVar1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameMap.c",
               PTR____FUNCTION___28413_001c513c,0x4e7);
  }
  if (*(int *)(PTR__gMapFrameArray_001c5154 + sVar1 * 4) == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameMap.c",
               PTR____FUNCTION___28413_001c513c,0x4e8);
  }
  if (*(short *)(PTR__gMapLevelNumber_001c5118 + sVar1 * 2) == -1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameMap.c",
               PTR____FUNCTION___28413_001c513c,0x4e9);
  }
  _CustomMapDrawProc(*(undefined4 *)(PTR__gMapFrameArray_001c5154 + sVar1 * 4),&stack0x00000008,1,
                     1.0 / *(float *)PTR__gCurMapZoom_001c5100,0xffff);
  return;
}


// ---- _DrawMapInOffscreenAreas @ 0x89ef7 (97 bytes)

void _DrawMapInOffscreenAreas(void)

{
  _DisposeCurrentRoom(1);
  _glClear(0x4000);
  _DrawMapScreen(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c),
                 *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x80),
                 *(int *)PTR__gSpriteWorldP_001c40d0 + 0x74);
  return;
}


// ---- _DisposeRoomInMapCache @ 0x89f58 (56 bytes)

void DisposeRoomInMapCache(short param_1)

{
  short sVar1;
  
  sVar1 = _FindRoomInMapCache((int)param_1);
  if (sVar1 != -1) {
    _DisposeMapCachedRoom((int)sVar1);
  }
  return;
}


// ---- _DisposeMapCachedRoom @ 0x89f90 (268 bytes)

void _DisposeMapCachedRoom(short param_1)

{
  if (param_1 < 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameMap.c",
               PTR____FUNCTION___28450_001c5128,0x51b);
  }
  if (*(short *)(PTR__gMapLevelNumber_001c5118 + param_1 * 2) == -1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameMap.c",
               PTR____FUNCTION___28450_001c5128,0x51c);
  }
  if (*(int *)(PTR__gMapFrameArray_001c5154 + param_1 * 4) == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameMap.c",
               PTR____FUNCTION___28450_001c5128,0x51d);
  }
  _SWDisposeFrame((FramePtr *)(PTR__gMapFrameArray_001c5154 + param_1 * 4));
  *(undefined4 *)(PTR__gMapFrameArray_001c5154 + param_1 * 4) = 0;
  *(undefined2 *)(PTR__gMapLevelNumber_001c5118 + param_1 * 2) = 0xffff;
  return;
}


// ---- _DisposeAllMapCachedRooms @ 0x8a09c (78 bytes)

void _DisposeAllMapCachedRooms(void)

{
  short local_e;
  
  for (local_e = 0; local_e < 200; local_e = local_e + 1) {
    if (*(int *)(PTR__gMapFrameArray_001c5154 + local_e * 4) != 0) {
      _DisposeMapCachedRoom((int)local_e);
    }
  }
  return;
}


// ---- _GetMapRoomWidth @ 0x8a0ea (67 bytes)

int _GetMapRoomWidth(int param_1)

{
  return (int)(short)(((int)*(short *)(param_1 + 6) - (int)*(short *)(param_1 + 2)) /
                     (int)*(short *)PTR__gNumRoomsPerCol_001c5120);
}


// ---- _GetMapRoomHeight @ 0x8a12d (66 bytes)

int GetMapRoomHeight(short *param_1)

{
  return (int)(short)(((int)param_1[2] - (int)*param_1) /
                     (int)*(short *)PTR__gNumRoomsPerCol_001c5120);
}


// ---- _RevealGridInMap @ 0x8a16f (352 bytes)

void _RevealGridInMap(short param_1,short param_2)

{
  char cVar1;
  short local_20;
  short local_1e;
  short local_1c;
  short local_1a;
  short local_18;
  short local_16;
  short local_14;
  short local_12;
  short local_10;
  short local_e;
  
  local_10 = _GEGetLevelNumber((int)param_1,1,200);
  local_e = _GEGetLevelNumber((int)param_2,1,200);
  _GEGetRowAndColFromLevelNumber((int)local_10,&local_1a,&local_1c,200);
  _GEGetRowAndColFromLevelNumber((int)local_e,&local_1e,&local_20,200);
  for (local_18 = local_1a; local_18 <= local_1e; local_18 = local_18 + 1) {
    for (local_16 = local_1c; local_16 <= local_20; local_16 = local_16 + 1) {
      local_12 = _GEGetLevelNumberFromRowAndCol((int)local_18,(int)local_16,200);
      local_14 = _GEGetLevelResID((int)local_12,1,200);
      cVar1 = _Resource1Exists("TMAP",(int)local_14);
      if (cVar1 != '\0') {
        cVar1 = _IsRoomASecretRoom((int)local_14);
        if (cVar1 == '\0') {
          _AddRoomToMapArray((int)local_12);
        }
      }
    }
  }
  return;
}


// ---- _RevealEntireMap @ 0x8a2cf (334 bytes)

void _RevealEntireMap(void)

{
  char cVar1;
  short local_20;
  short local_1e;
  short local_1c;
  short local_1a;
  short local_18;
  short local_16;
  short local_14;
  short local_12;
  short local_10;
  short local_e;
  
  local_10 = _GEGetLevelNumber(0x80,1,200);
  local_e = _GEGetLevelNumber(0x7fff,1,200);
  _GEGetRowAndColFromLevelNumber((int)local_10,&local_1a,&local_1c,200);
  _GEGetRowAndColFromLevelNumber((int)local_e,&local_1e,&local_20,200);
  for (local_18 = local_1a; local_18 <= local_1e; local_18 = local_18 + 1) {
    for (local_16 = local_1c; local_16 <= local_20; local_16 = local_16 + 1) {
      local_12 = _GEGetLevelNumberFromRowAndCol((int)local_18,(int)local_16,200);
      local_14 = _GEGetLevelResID((int)local_12,1,200);
      cVar1 = _Resource1Exists("TMAP",(int)local_14);
      if (cVar1 != '\0') {
        _AddRoomToMapArray((int)local_12);
        _AddRoomToArray((int)local_12);
      }
    }
  }
  return;
}


// ---- _ResetKeySpriteFrames @ 0x8a41d (115 bytes)

void _ResetKeySpriteFrames(void)

{
  SpritePtr local_10;
  
  local_10 = (SpritePtr)0x0;
  while( true ) {
    local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gObjectSpriteLayerP_001c4268,local_10);
    if (local_10 == (SpritePtr)0x0) break;
    if ((0x26 < local_10->userData) && (local_10->userData < 0x2d)) {
      _SWSetCurrentFrameIndex(local_10,0);
    }
  }
  return;
}


// ---- _RevealSecretRooms @ 0x8a490 (338 bytes)

void _RevealSecretRooms(void)

{
  char cVar1;
  short local_20;
  short local_1e;
  short local_1c;
  short local_1a;
  short local_18;
  short local_16;
  short local_14;
  short local_12;
  short local_10;
  short local_e;
  
  local_10 = _GEGetLevelNumber(0x80,1,200);
  local_e = _GEGetLevelNumber(0x7fff,1,200);
  _GEGetRowAndColFromLevelNumber((int)local_10,&local_1a,&local_1c,200);
  _GEGetRowAndColFromLevelNumber((int)local_e,&local_1e,&local_20,200);
  for (local_18 = local_1a; local_18 <= local_1e; local_18 = local_18 + 1) {
    for (local_16 = local_1c; local_16 <= local_20; local_16 = local_16 + 1) {
      local_12 = _GEGetLevelNumberFromRowAndCol((int)local_18,(int)local_16,200);
      local_14 = _GEGetLevelResID((int)local_12,1,200);
      cVar1 = _Resource1Exists("TMAP",(int)local_14);
      if (cVar1 != '\0') {
        cVar1 = _IsRoomASecretRoom((int)local_14);
        if (cVar1 != '\0') {
          _AddRoomToMapArray((int)local_12);
        }
      }
    }
  }
  return;
}


// ---- _ExitInGameMapScreen @ 0x8a5e2 (789 bytes)

void _ExitInGameMapScreen(char param_1)

{
  undefined1 uVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined1 uVar4;
  short sVar5;
  SpritePtr pSVar6;
  longdouble extraout_ST0;
  float fVar7;
  
  _LoadAppropriateStatsBackground();
  _MakeStatsSpritesVisible();
  _SWRemoveAllSpritesFromLayer(*(SpriteLayerPtr *)PTR__gStatsMapTextSpriteLayerP_001c4f90);
  _DoZoomBackToGameEffect();
  if (*PTR__gFasterInGameMap_001c4820 == '\0') {
    _DisposeAllMapCachedRooms();
  }
  _DisposeCurrentRoom(1);
  _RestoreSpritesFromTempLayers(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  _SWRestoreTileMaps(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  *(undefined2 *)PTR__gCurrentLevel_001c49bc = *(undefined2 *)PTR__gSavedLevelNumber_001c5138;
  *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x1c) =
       *(undefined4 *)PTR__saveTileBackgroundP_001c4ff0;
  *(undefined4 *)PTR__saveTileBackgroundP_001c4ff0 = 0;
  _ResetKeys();
  _WriteEventToMovieFile(0xb,1);
  if (param_1 != '\0') {
    sVar5 = _GEGetLevelResID((int)*(short *)PTR__gCurrentLevel_001c49bc,1,200);
    _SWSetSpriteLayersVisible
              (*(undefined4 *)PTR__gBackgroundScreenSpriteLayerP_001c4780,
               *(undefined4 *)PTR__gBackgroundPictureSpriteLayerP_001c475c,0);
    _LoadLEVLResource((int)sVar5);
    uVar1 = *PTR__gRoomIsDark_001c483c;
    uVar4 = _IsRoomDark((int)sVar5);
    _MakeRoomDarkOrLight(uVar4,uVar1);
    *PTR__gSilverFloorIsActive_001c4958 = *PTR__gSaveSilverFloorIsActive_001c5164;
    *PTR__gGoldFloorIsActive_001c47a8 = *PTR__gSaveGoldFloorIsActive_001c5114;
    _SetCorrectGoldSilverTileImages(0);
    *(undefined2 *)PTR__gCurConveyorBeltDirection_001c470c =
         *(undefined2 *)PTR__gSaveCurConveyorBeltDirection_001c5150;
    *(undefined2 *)PTR__gOldConveyorBeltDirection_001c4f18 =
         *(undefined2 *)PTR__gSaveOldConveyorBeltDirection_001c50fc;
    *(undefined2 *)PTR__gOldBackgroundPictResID_001c47b0 =
         *(undefined2 *)PTR__gSaveOldBackgroundPictResID_001c5108;
    _SWDisposeSpriteLayer((SpriteLayerPtr *)PTR__gHiddenTriggerItemsSpriteLayerP_001c4864);
    *(undefined4 *)PTR__gHiddenTriggerItemsSpriteLayerP_001c4864 =
         *(undefined4 *)PTR__gSaveHiddenTriggerItemsSpriteLayerP_001c5104;
    *(undefined4 *)PTR__averageFrameRate_001c517c = 0xffffffff;
    *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 5;
    _UpdateMenuBar();
    if (*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x1c) != 0) {
      _SWGetSpriteWidth(*(SpritePtr *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x1c));
      fVar7 = (float)((int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x8a) -
                     (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x86));
      if (((float)extraout_ST0 != fVar7) || (NAN((float)extraout_ST0) || NAN(fVar7))) {
        _DrawBackgroundTextureForCurRoom();
      }
    }
    pSVar6 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gHaloSpriteLayerP_001c41e0,(SpritePtr)0x0);
    if (pSVar6 != (SpritePtr)0x0) {
      _SWSetSpriteScaledFactor(pSVar6,0x42480000,0x42480000);
    }
  }
  puVar3 = PTR__gKeysCopy_001c5174;
  puVar2 = PTR__gKeys_001c4af8;
  if (*PTR__gPlayingGameMovie_001c4fd8 != '\0') {
    *(undefined4 *)PTR__gKeys_001c4af8 = *(undefined4 *)PTR__gKeysCopy_001c5174;
    *(undefined4 *)(puVar2 + 4) = *(undefined4 *)(puVar3 + 4);
    *(undefined2 *)(puVar2 + 8) = *(undefined2 *)(puVar3 + 8);
    puVar2[10] = puVar3[10];
  }
  _MMShieldCursor();
  return;
}


// ---- _DoZoomBackToGameEffect @ 0x8a8f7 (964 bytes)

void _DoZoomBackToGameEffect(void)

{
  char cVar1;
  int iVar2;
  int local_40;
  int local_3c;
  int local_30;
  int local_2c;
  short local_26;
  short local_24;
  short local_22;
  float local_20;
  float local_1c;
  float local_18;
  float local_14;
  int local_10;
  
  *PTR__gUseOptimizedCaching_001c515c = 0;
  for (local_22 = -1; local_22 < 2; local_22 = local_22 + 1) {
    local_24 = *(short *)PTR__gSavedLevelNumber_001c5138 + local_22;
    local_26 = _GEGetLevelResID((int)local_24,1,200);
    cVar1 = _IsRoomInArray((int)local_24);
    if (((cVar1 != '\0') || (cVar1 = _IsRoomInMapArray((int)local_24), cVar1 != '\0')) &&
       (cVar1 = _ResourceExists("TMAP",(int)local_26), cVar1 != '\0')) {
      _AddMapRoomToCacheIfNecessary((int)local_24,0);
    }
  }
  local_20 = (*(float *)PTR__gCurMapZoom_001c5100 / 2.5) * 0.0006;
  *(undefined2 *)PTR__numRoomsCanAddThisFrame_001c5124 = 0;
  *(undefined2 *)PTR__gJacksHeadTranslucency_001c5170 = 0;
  while (1.0 < *(float *)PTR__gCurMapZoom_001c5100) {
    local_10 = _SWGetMilliseconds(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    _ComputeMapRectForLevel((int)*(short *)PTR__gSavedLevelNumber_001c5138,&local_40);
    _SetRectZoom(&local_40,*(undefined4 *)PTR__gCurMapZoom_001c5100);
    local_14 = (1.0 - local_20 * local_1c) * *(float *)PTR__gCurMapZoom_001c5100;
    if (local_14 < 1.0) {
      local_14 = 1.0;
    }
    _SetMapZoom(local_14);
    local_20 = local_20 + 2e-05;
    if (0.0025 < local_20) {
      local_20 = 0.0025;
    }
    local_18 = (*(float *)PTR__gCurMapZoom_001c5100 - 1.0) * 45.0 + 1.0;
    if (local_18 < 1.0) {
      local_18 = 1.0;
    }
    if (40.0 / *(float *)PTR__gCurMapZoom_001c5100 < local_18) {
      local_18 = 40.0 / *(float *)PTR__gCurMapZoom_001c5100;
    }
    local_30 = (int)(((float)(local_3c - *(int *)(PTR__gMapScrollRect_001c516c + 4)) /
                     *(float *)PTR__gCurMapZoom_001c5100) / local_18);
    local_2c = (int)(((float)(local_40 - *(int *)PTR__gMapScrollRect_001c516c) /
                     *(float *)PTR__gCurMapZoom_001c5100) / local_18);
    _OffsetLongRect(PTR__gMapScrollRect_001c516c,local_30,local_2c);
    _DrawMapScreen(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x74),
                   *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x78),
                   *(int *)PTR__gSpriteWorldP_001c40d0 + 0x74);
    iVar2 = _SWGetMilliseconds(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    local_1c = (float)((uint)(iVar2 - local_10) >> 0x10) * 65536.0 +
               (float)(iVar2 - local_10 & 0xffff);
  }
  return;
}


// ---- _CenterLongRect @ 0x8acbb (206 bytes)

void _CenterLongRect(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = param_1[1];
  iVar2 = *param_1;
  param_1[1] = param_2[1] + ((param_2[3] - param_2[1]) / 2 - (param_1[3] - iVar1) / 2);
  *param_1 = *param_2 + ((param_2[2] - *param_2) / 2 - (param_1[2] - iVar2) / 2);
  param_1[3] = param_1[1] + (param_1[3] - iVar1);
  param_1[2] = *param_1 + (param_1[2] - iVar2);
  return;
}


// ---- _OffsetLongRect @ 0x8ad89 (80 bytes)

void _OffsetLongRect(int *param_1,int param_2,int param_3)

{
  param_1[1] = param_1[1] + param_2;
  param_1[3] = param_1[3] + param_2;
  *param_1 = *param_1 + param_3;
  param_1[2] = param_1[2] + param_3;
  return;
}


// ---- _CenterFloatRect @ 0x8add9 (291 bytes)

void _CenterFloatRect(float *param_1,float *param_2)

{
  float fVar1;
  float fVar2;
  
  fVar1 = param_1[1];
  fVar2 = *param_1;
  param_1[1] = (param_1[3] - fVar1) / -2.0 + (param_2[3] - param_2[1]) / 2.0 + param_2[1];
  *param_1 = (param_1[2] - fVar2) / -2.0 + (param_2[2] - *param_2) / 2.0 + *param_2;
  param_1[3] = param_1[1] + (param_1[3] - fVar1);
  param_1[2] = *param_1 + (param_1[2] - fVar2);
  return;
}


// ---- _MMPaintRect @ 0x8aefc (446 bytes)

void _MMPaintRect(short *param_1,undefined1 *param_2,char param_3)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  void *pvVar5;
  
  pvVar5 = _BKLoadGLfunctions();
  _glColor4ub(*param_2,param_2[1],param_2[2],param_3);
  (**(code **)((int)pvVar5 + 0x74))(0x1700);
  (**(code **)((int)pvVar5 + 0x7c))();
  (**(code **)((int)pvVar5 + 0x78))();
  (**(code **)((int)pvVar5 + 0x88))
            (*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3c),
             *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x40),0x3f800000);
  (**(code **)((int)pvVar5 + 0x8c))((float)(int)param_1[1],(float)(int)*param_1,0);
  sVar1 = param_1[3];
  sVar2 = param_1[1];
  sVar3 = param_1[2];
  sVar4 = *param_1;
  if (param_3 != -1) {
    (**(code **)((int)pvVar5 + 0x14))(0xbe2);
  }
  _glBegin(7);
  (**(code **)((int)pvVar5 + 0x60))(0,0);
  (**(code **)((int)pvVar5 + 0x60))((int)(short)(sVar1 - sVar2),0);
  (**(code **)((int)pvVar5 + 0x60))((int)(short)(sVar1 - sVar2),(int)(short)(sVar3 - sVar4));
  (**(code **)((int)pvVar5 + 0x60))(0,(int)(short)(sVar3 - sVar4));
  (**(code **)((int)pvVar5 + 4))();
  (**(code **)((int)pvVar5 + 0x18))(0xbe2);
  (**(code **)((int)pvVar5 + 0x80))();
  return;
}


// ---- _MMPaintFloatRect @ 0x8b0ba (454 bytes)

void _MMPaintFloatRect(float *param_1,undefined1 *param_2,char param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  void *pvVar5;
  
  pvVar5 = _BKLoadGLfunctions();
  _glColor4ub(*param_2,param_2[1],param_2[2],param_3);
  (**(code **)((int)pvVar5 + 0x74))(0x1700);
  (**(code **)((int)pvVar5 + 0x7c))();
  (**(code **)((int)pvVar5 + 0x78))();
  (**(code **)((int)pvVar5 + 0x88))
            (*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3c),
             *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x40),0x3f800000);
  (**(code **)((int)pvVar5 + 0x8c))(param_1[1],*param_1,0);
  fVar1 = param_1[3];
  fVar2 = param_1[1];
  fVar3 = param_1[2];
  fVar4 = *param_1;
  if (param_3 != -1) {
    (**(code **)((int)pvVar5 + 0x14))(0xbe2);
  }
  _glBegin(7);
  (**(code **)((int)pvVar5 + 0x5c))(0,0);
  (**(code **)((int)pvVar5 + 0x5c))(fVar1 - fVar2,0);
  (**(code **)((int)pvVar5 + 0x5c))(fVar1 - fVar2,fVar3 - fVar4);
  (**(code **)((int)pvVar5 + 0x5c))(0,fVar3 - fVar4);
  (**(code **)((int)pvVar5 + 4))();
  (**(code **)((int)pvVar5 + 0x18))(0xbe2);
  (**(code **)((int)pvVar5 + 0x80))();
  return;
}


// ---- _CustomMapDrawProc @ 0x8b280 (536 bytes)

void _CustomMapDrawProc(int param_1,float *param_2,char param_3,undefined4 param_4,
                       undefined2 param_5)

{
  void *pvVar1;
  undefined4 *puVar2;
  int iVar3;
  
  pvVar1 = _BKLoadGLfunctions();
  puVar2 = (undefined4 *)_BKLoadGLcapabilities();
  _glColor4us(0xffff,0xffff,0xffff,param_5);
  (**(code **)((int)pvVar1 + 0x30))(*puVar2,&DAT_00002800,&DAT_00002601);
  (**(code **)((int)pvVar1 + 0x30))(*puVar2,&DAT_00002801,&DAT_00002601);
  (**(code **)((int)pvVar1 + 0x74))(0x1700);
  (**(code **)((int)pvVar1 + 0x7c))();
  (**(code **)((int)pvVar1 + 0x78))();
  (**(code **)((int)pvVar1 + 0x88))
            (*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3c),
             *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x40),0x3f800000);
  (**(code **)((int)pvVar1 + 0x8c))(param_2[1],*param_2,0);
  if (param_3 != '\0') {
    (**(code **)((int)pvVar1 + 0x8c))(0,param_2[2] - *param_2,0);
    (**(code **)((int)pvVar1 + 0x84))(0x43340000,0x3f800000,0,0);
  }
  (**(code **)((int)pvVar1 + 0x88))(param_4,param_4,0x3f800000);
  iVar3 = _DoesSurfaceNeedBlend(*(undefined4 *)(param_1 + 0x34));
  if (iVar3 != 0) {
    (**(code **)((int)pvVar1 + 0x14))(0xbe2);
  }
  _BKBlitGLSurface(*(void **)(param_1 + 0x34));
  if (iVar3 != 0) {
    (**(code **)((int)pvVar1 + 0x18))(0xbe2);
  }
  (**(code **)((int)pvVar1 + 0x80))();
  return;
}


