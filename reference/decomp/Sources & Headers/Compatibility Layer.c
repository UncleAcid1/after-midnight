// Ghidra decompilation of functions from original source file:
//   Sources & Headers/Compatibility Layer.c

// ---- _LoadMidnightMansionTestSprites @ 0x9500e (70 bytes)

void LoadMidnightMansionTestSprites(void)

{
  _InitSpriteArray();
  _DisplaySplashScreen();
  _ChangeSplashScreenLoadingText(1);
  _LoadPixelMaskPICTSprites();
  _LoadNonPixelMaskSprites();
  _ChangeSplashScreenLoadingText(2);
  _DoBeatTheMansionScreen(0);
  return;
}


// ---- _MyDrawText @ 0x95054 (96 bytes)

void _MyDrawText(undefined4 param_1,undefined4 param_2,short param_3,undefined2 param_4,
                undefined4 param_5)

{
  undefined2 local_14;
  short local_12;
  undefined2 local_10;
  short local_e;
  
  local_12 = param_3;
  local_14 = param_4;
  local_e = param_3 + 200;
  local_10 = local_14;
  _MyDrawCTextJustified(param_1,param_2,&local_14,param_5);
  return;
}


// ---- _MyDrawPascalTextJustified @ 0x950b4 (73 bytes)

void _MyDrawPascalTextJustified
               (char *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char local_264 [608];
  
  _SWCopyPascalStringToCString(param_1,local_264);
  _MyDrawCTextJustified(local_264,param_2,param_3,param_4);
  return;
}


// ---- _MyDrawCTextJustified @ 0x950fd (440 bytes)

SpritePtr _MyDrawCTextJustified(undefined4 param_1,char *param_2,int param_3,undefined2 *param_4)

{
  char local_11c [256];
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_1a;
  SpritePtr local_18;
  short local_14;
  short local_12;
  int local_10;
  
  local_18 = (SpritePtr)0x0;
  local_10 = 0;
  _SWCopyPascalStringToCString(param_2,local_11c);
  local_12 = *(short *)(param_2 + 0x102);
  local_1c = (undefined1)((ushort)*param_4 >> 8);
  local_1b = (undefined1)((ushort)param_4[1] >> 8);
  local_1a = (undefined1)((ushort)param_4[2] >> 8);
  do {
    local_14 = _GetTextSpriteRenderWidth(local_11c,(int)local_12,param_1);
    if ((int)*(short *)(param_3 + 6) - (int)*(short *)(param_3 + 2) < (int)local_14) {
      local_12 = local_12 + -1;
    }
  } while ((int)*(short *)(param_3 + 6) - (int)*(short *)(param_3 + 2) < (int)local_14);
  local_10 = _SWCreateTextSprite(&local_18,param_1,local_11c,(int)local_12,
                                 (int)*(short *)(param_2 + 0x100),&local_1c,param_2[0x105]);
  if (local_10 == 0) {
    _SetTextSpriteRect(local_18,param_3,(int)*(short *)(param_2 + 0x106),0);
    _SWSetSpriteLocation
              (local_18,(float)(int)*(short *)(param_3 + 2),
               (float)((int)*(short *)(param_3 + 4) - (int)local_12));
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gTextSpriteLayerP_001c4804,local_18);
  }
  return local_18;
}


// ---- _MyDrawTextBox @ 0x952b5 (1388 bytes)

SpritePtr _MyDrawTextBox(SpriteWorldPtr param_1,char *param_2,char *param_3,undefined4 *param_4,
                        undefined2 *param_5,SpriteLayerPtr param_6,char param_7)

{
  char local_fd0;
  undefined4 local_fc4;
  undefined4 local_fc0;
  undefined1 auStack_fbc [100];
  char *apcStack_f58 [100];
  char local_dc8 [400];
  char local_c38 [400];
  undefined4 local_aa8;
  undefined4 local_aa4;
  char local_aa0 [256];
  char local_9a0 [2400];
  undefined1 local_40;
  undefined1 local_3f;
  undefined1 local_3e;
  SpritePtr local_3c;
  SpritePtr local_38;
  SpriteLayerPtr local_34 [3];
  SWError local_28;
  char local_22;
  char local_21;
  short local_20;
  short local_1e;
  char *local_1c;
  char *local_18;
  short local_12;
  short local_10;
  short local_e;
  
  local_fd0 = param_7;
  local_38 = (SpritePtr)0x0;
  local_28 = 0;
  if (param_7 == '\0') {
    param_3[0x100] = '\0';
    param_3[0x101] = '\0';
  }
  else {
    if (param_1 == (SpriteWorldPtr)0x0) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Compatibility Layer.c"
                 ,PTR____FUNCTION___27641_001c524c,0xd4);
    }
    local_28 = _SWCreateSpriteLayer(local_34);
    if (local_28 == 0) {
      _SWAddSpriteLayer(param_1,local_34[0]);
    }
    else {
      local_fd0 = '\0';
    }
  }
  _SWCopyCString(param_2,local_9a0);
  _SWCopyPascalStringToCString(param_3,local_aa0);
  local_40 = (undefined1)((ushort)*param_5 >> 8);
  local_3f = (undefined1)((ushort)param_5[1] >> 8);
  local_3e = (undefined1)((ushort)param_5[2] >> 8);
  local_aa4 = param_4[1];
  local_aa8 = *param_4;
  _SWOffsetRect(&local_aa8,0,(int)*(short *)(param_3 + 0x102));
  local_1e = local_aa4._2_2_ - local_aa8._2_2_;
  local_12 = 0;
  local_10 = 0;
  local_18 = local_9a0;
  do {
    local_18 = _strchr(local_18,0xd);
    if (local_18 != (char *)0x0) {
      local_18 = local_18 + 1;
      if (*local_18 == '\r') {
        auStack_fbc[local_10] = 1;
        local_10 = local_10 + 1;
        local_18 = local_18 + 1;
      }
      else {
        auStack_fbc[local_10] = 0;
        local_10 = local_10 + 1;
      }
    }
  } while (local_18 != (char *)0x0);
  local_10 = 0;
  local_18 = _strtok(local_9a0,"\r");
  do {
    apcStack_f58[local_10] = local_18;
    local_10 = local_10 + 1;
    local_18 = _strtok((char *)0x0,"\r");
  } while (local_18 != (char *)0x0);
  local_12 = 0;
  local_1c = _strtok(apcStack_f58[0]," ");
  local_22 = '\0';
  while (local_22 == '\0') {
    local_e = *(short *)(param_3 + 0x102) + 2;
    local_c38[0] = '\0';
    _strcpy(local_c38,local_1c);
    local_21 = '\0';
    while (local_21 == '\0') {
      local_1c = _strtok((char *)0x0," ");
      while (local_1c == (char *)0x0) {
        local_12 = local_12 + 1;
        if (local_10 <= local_12) {
          local_22 = '\x01';
          goto LAB_0009563d;
        }
        local_1c = _strtok(apcStack_f58[local_12]," ");
        if (auStack_fbc[local_12 + -1] != '\0') {
          local_e = *(short *)(param_3 + 0x102) + local_e + 2;
        }
        local_21 = '\x01';
        if (local_1c != (char *)0x0) goto LAB_0009563d;
      }
      _sprintf(local_dc8,"%s %s",local_c38,local_1c);
      local_20 = _GetTextSpriteRenderWidth(local_aa0,(int)*(short *)(param_3 + 0x102),local_dc8);
      if (local_20 < local_1e) {
        _strcpy(local_c38,local_dc8);
      }
      else {
        local_21 = '\x01';
      }
    }
LAB_0009563d:
    if (local_c38[0] == '\0') break;
    local_28 = _SWCreateTextSprite(&local_3c,local_c38,local_aa0,(int)*(short *)(param_3 + 0x102),
                                   (int)*(short *)(param_3 + 0x100),&local_40,param_3[0x105]);
    if (local_28 == 0) {
      _SetTextSpriteRect(local_3c,&local_aa8,(int)*(short *)(param_3 + 0x106),1);
      _SWSetSpriteLocation(local_3c,(float)(int)local_aa8._2_2_,(float)(int)(short)local_aa8);
      _SWOffsetRect(&local_aa8,0,(int)local_e);
      if (local_fd0 == '\0') {
        _SWAddSprite(param_6,local_3c);
      }
      else {
        _SWAddSprite(local_34[0],local_3c);
      }
    }
  }
  if (local_fd0 != '\0') {
    _MMRenderSceneForWorkAreaSprite(param_1);
    local_fc0 = param_4[1];
    local_fc4 = *param_4;
    _SWAutoScaleRect(param_1,&local_fc4);
    local_28 = _SWCreateSpriteFromWorkArea(param_1,&local_38,0,&local_fc4,0);
    if (local_28 == 0) {
      _SWDisposeAllSpritesInLayer(local_34[0]);
      _SWAddSprite(param_6,local_38);
    }
    _SWRemoveSpriteLayer(param_1,local_34[0]);
    _SWDisposeSpriteLayer(local_34);
  }
  return local_38;
}


// ---- _MMRenderSceneForWorkAreaSprite @ 0x95821 (298 bytes)

void _MMRenderSceneForWorkAreaSprite(SpriteWorldPtr param_1)

{
  float zoomH;
  float zoomV;
  
  zoomH = param_1->zoomH;
  zoomV = param_1->zoomV;
  _SWSetSpriteWorldZoom(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,1.0,1.0);
  _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gCustomCursorSpriteLayerP_001c49f4,'\0');
  _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gFadeToBlackSpriteLayerP_001c43e4,'\0');
  _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gTopSpriteLayerP_001c41b4,'\0');
  _SWUpdateSpriteWorld(param_1,0);
  _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gFadeToBlackSpriteLayerP_001c43e4,'\x01');
  _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gCustomCursorSpriteLayerP_001c49f4,'\x01');
  _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gTopSpriteLayerP_001c41b4,'\x01');
  _SWSetSpriteWorldZoom(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,zoomH,zoomV);
  return;
}


// ---- _LoadAndInstallBackgroundSprite @ 0x9594b (498 bytes)

int _LoadAndInstallBackgroundSprite
              (undefined4 *param_1,undefined4 param_2,undefined4 param_3,short param_4,
              undefined1 param_5)

{
  SpritePtr local_1c;
  int local_18;
  undefined4 local_14;
  short local_e;
  
  local_18 = 0;
  local_14 = _AddFilenameToPath(param_2,param_3);
  if (*PTR__gBackgroundSpriteCacheWasInited_001c522c == '\0') {
    for (local_e = 0; local_e < 10; local_e = local_e + 1) {
      *(undefined4 *)(PTR__gBackgroundSpriteCache_001c5218 + local_e * 4) = 0xffffffff;
    }
    *PTR__gBackgroundSpriteCacheWasInited_001c522c = 1;
  }
  if (((param_4 == -1) || (9 < param_4)) ||
     (*(int *)(PTR__gBackgroundSpriteCache_001c5218 + param_4 * 4) == -1)) {
    local_18 = _SWCreateSpriteFromFile(PTR__gCurBackgroundSprite_001c5224,0,local_14,1,param_5);
    if (((local_18 == 0) && (param_4 != -1)) && (param_4 < 10)) {
      *(undefined4 *)(PTR__gBackgroundSpriteCache_001c5218 + param_4 * 4) =
           *(undefined4 *)PTR__gCurBackgroundSprite_001c5224;
    }
  }
  else {
    *(undefined4 *)PTR__gCurBackgroundSprite_001c5224 =
         *(undefined4 *)(PTR__gBackgroundSpriteCache_001c5218 + param_4 * 4);
  }
  if (local_18 == 0) {
    _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gBackgroundScreenSpriteLayerP_001c4780);
    if (param_4 == -1) {
      local_1c = *(SpritePtr *)PTR__gCurBackgroundSprite_001c5224;
    }
    else {
      _SWCloneSprite(*(undefined4 *)PTR__gCurBackgroundSprite_001c5224,&local_1c,0,0);
    }
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBackgroundScreenSpriteLayerP_001c4780,local_1c);
    _SWSetSpriteLocation(local_1c,0.0,0.0);
    if (param_4 != -1) {
      _SWSetSpriteScaledSize(local_1c,0,0);
    }
    if (param_1 != (undefined4 *)0x0) {
      *param_1 = local_1c;
    }
  }
  return local_18;
}


// ---- _LoadSpriteFromScreensFolder @ 0x95b3d (98 bytes)

undefined4 LoadSpriteFromScreensFolder(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  
  uVar1 = _GetScreensFolderPath();
  uVar1 = _AddFilenameToPath(uVar1,param_2);
  uVar1 = _SWCreateSpriteFromFile(param_1,0,uVar1,1,1);
  return uVar1;
}


// ---- _CreateSpriteFromSinglePict @ 0x95b9f (179 bytes)

undefined4
_CreateSpriteFromSinglePict
          (undefined4 param_1,undefined4 param_2,undefined4 param_3,short param_4,short param_5,
          short param_6,short param_7,int param_8)

{
  undefined4 uVar1;
  undefined4 local_30;
  
  if ((param_4 == param_5) && (param_8 != 0)) {
    local_30 = 1;
  }
  else {
    local_30 = 0;
  }
  uVar1 = _GetSpriteImagePath(0);
  uVar1 = _GetFileNameForResource(uVar1,(int)param_4);
  uVar1 = _SWCreateSpriteFromSingleFile(param_2,param_3,uVar1,(int)param_6,(int)param_7,local_30);
  return uVar1;
}


// ---- _CreateSpriteFromPictResource @ 0x95c52 (333 bytes)

undefined4
_CreateSpriteFromPictResource
          (undefined4 param_1,undefined4 param_2,undefined4 param_3,short param_4,short param_5,
          short param_6,int param_7)

{
  undefined4 uVar1;
  undefined1 local_1bc;
  char local_1a8 [400];
  undefined4 local_18;
  undefined1 local_11;
  undefined4 local_10;
  
  local_18 = 0;
  if ((param_4 == param_5) && (param_7 != 0)) {
    local_1bc = 1;
  }
  else {
    local_1bc = 0;
  }
  local_11 = local_1bc;
  if (param_6 == 1) {
    uVar1 = _GetSpriteImagePath(0);
    local_10 = _GetFileNameForResource(uVar1,(int)param_4);
    local_18 = _SWCreateSpriteFromFile(param_2,param_3,local_10,1,local_11);
  }
  else {
    uVar1 = _GetSpriteImagePath(0);
    _sprintf(local_1a8,"%s%s.png",uVar1,"%05d");
    local_18 = _SWCreateSpriteFromFileSequence
                         (param_2,param_3,local_1a8,(int)param_4,(int)param_6,local_11);
  }
  return local_18;
}


// ---- _CreateSpriteFromSinglePictXY @ 0x95d9f (292 bytes)

undefined4
CreateSpriteFromSinglePictXY
          (undefined4 param_1,undefined4 param_2,undefined4 param_3,short param_4,short param_5,
          short param_6,short param_7,short param_8,short param_9,undefined1 param_10,short param_11
          ,int param_12)

{
  undefined4 uVar1;
  undefined4 uStack_48;
  
  uVar1 = _GetSpriteImagePath(0);
  uVar1 = _GetFileNameForResource(uVar1,(int)param_4);
  if ((param_4 == param_5) && (param_12 != 0)) {
    uStack_48 = 1;
  }
  else {
    uStack_48 = 0;
  }
  uVar1 = _SWCreateSpriteFromSingleFileXY
                    (param_2,param_3,uVar1,(int)param_6,(int)param_7,(int)param_8,(int)param_9,
                     param_10,(int)param_11,uStack_48);
  return uVar1;
}


// ---- _MMCreateSpriteFromSingleFileXY @ 0x95ec3 (169 bytes)

undefined4
_MMCreateSpriteFromSingleFileXY
          (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
          undefined4 param_5,short param_6,short param_7,short param_8,int param_9)

{
  undefined4 uVar1;
  
  uVar1 = _GetScreensFolderPath();
  uVar1 = _AddFilenameToPath(uVar1,param_4);
  uVar1 = _SWCreateSpriteFromSingleFileXY
                    (param_2,param_3,uVar1,(int)param_6,(int)param_7,0,0,0,(int)param_8,param_9 != 0
                    );
  return uVar1;
}


// ---- _CreateSpriteFromVerticalPict @ 0x95f6c (172 bytes)

undefined4
CreateSpriteFromVerticalPict
          (undefined4 param_1,undefined4 param_2,undefined4 param_3,short param_4,short param_5,
          short param_6,int param_7)

{
  undefined4 uVar1;
  undefined4 uStack_2c;
  
  uVar1 = _GetSpriteImagePath(0);
  uVar1 = _GetFileNameForResource(uVar1,(int)param_4);
  if ((param_4 == param_5) && (param_7 != 0)) {
    uStack_2c = 1;
  }
  else {
    uStack_2c = 0;
  }
  uVar1 = _SWCreateSpriteFromVerticalFile(param_2,param_3,uVar1,0,(int)param_6,uStack_2c);
  return uVar1;
}


// ---- _MMCreateSpriteFromVerticalFile @ 0x96018 (118 bytes)

undefined4
_MMCreateSpriteFromVerticalFile
          (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
          undefined4 param_5,short param_6,int param_7)

{
  undefined4 uVar1;
  
  uVar1 = _GetScreensFolderPath();
  uVar1 = _AddFilenameToPath(uVar1,param_4);
  uVar1 = _SWCreateSpriteFromVerticalFile(param_2,param_3,uVar1,0,(int)param_6,param_7 != 0);
  return uVar1;
}


// ---- _GetFileNameForResource @ 0x9608e (73 bytes)

undefined * _GetFileNameForResource(undefined4 param_1,undefined4 param_2)

{
  _sprintf(PTR__gReturnedFilename_001c5254,"%s%05d.png",param_1,param_2);
  return PTR__gReturnedFilename_001c5254;
}


// ---- _AddFilenameToPath @ 0x960d7 (73 bytes)

undefined * _AddFilenameToPath(undefined4 param_1,undefined4 param_2)

{
  _sprintf(PTR__gReturnedFilename_001c5254,"%s%s",param_1,param_2);
  return PTR__gReturnedFilename_001c5254;
}


// ---- _GetSpriteImagePath @ 0x96120 (120 bytes)

undefined * _GetSpriteImagePath(int param_1)

{
  undefined4 uVar1;
  
  if (param_1 == 0) {
    uVar1 = _GetGameDataPath();
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Sprites/",uVar1);
  }
  else {
    uVar1 = _GetGameDataPath();
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Sprites/%s",uVar1,param_1);
  }
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetScreensFolderPath @ 0x96198 (68 bytes)

undefined * _GetScreensFolderPath(void)

{
  undefined4 uVar1;
  
  uVar1 = _GetGameDataPath();
  _sprintf(PTR__gImageFilePath_001c5230,"%s/Screens/",uVar1);
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetTitleScreenFolderPath @ 0x961dc (120 bytes)

undefined * _GetTitleScreenFolderPath(int param_1)

{
  undefined4 uVar1;
  
  if (param_1 == 0) {
    uVar1 = _GetGameDataPath();
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Screens/titleScreen/",uVar1);
  }
  else {
    uVar1 = _GetGameDataPath();
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Screens/titleScreen/%s",uVar1,param_1);
  }
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetMapScreenFolderPath @ 0x96254 (120 bytes)

undefined * _GetMapScreenFolderPath(int param_1)

{
  undefined4 uVar1;
  
  if (param_1 == 0) {
    uVar1 = _GetGameDataPath();
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Screens/mapScreen/",uVar1);
  }
  else {
    uVar1 = _GetGameDataPath();
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Screens/mapScreen/%s",uVar1,param_1);
  }
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetOrganScreenFolderPath @ 0x962cc (120 bytes)

undefined * _GetOrganScreenFolderPath(int param_1)

{
  undefined4 uVar1;
  
  if (param_1 == 0) {
    uVar1 = _GetGameDataPath();
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Screens/organScreen/",uVar1);
  }
  else {
    uVar1 = _GetGameDataPath();
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Screens/organScreen/%s",uVar1,param_1);
  }
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetSoundsFolderPath @ 0x96344 (68 bytes)

undefined * _GetSoundsFolderPath(void)

{
  undefined4 uVar1;
  
  uVar1 = _GetGameDataPath();
  _sprintf(PTR__gImageFilePath_001c5230,"%s/Sounds/",uVar1);
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetMusicFolderPath @ 0x96388 (68 bytes)

undefined * _GetMusicFolderPath(void)

{
  undefined4 uVar1;
  
  uVar1 = _GetGameDataPath();
  _sprintf(PTR__gImageFilePath_001c5230,"%s/Music/",uVar1);
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetFontsFolderPath @ 0x963cc (120 bytes)

undefined * _GetFontsFolderPath(int param_1)

{
  undefined4 uVar1;
  
  if (param_1 == 0) {
    uVar1 = _GetGameDataPath();
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Fonts/",uVar1);
  }
  else {
    uVar1 = _GetGameDataPath();
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Fonts/%s",uVar1,param_1);
  }
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetMansionFolderPath @ 0x96444 (130 bytes)

undefined * _GetMansionFolderPath(char *param_1)

{
  undefined4 uVar1;
  
  if ((param_1 == (char *)0x0) || (*param_1 == '\0')) {
    uVar1 = _GetGameDataPath();
    _sprintf(PTR__gMansionPath_001c5220,"%s/Mansions/",uVar1);
  }
  else {
    uVar1 = _GetGameDataPath();
    _sprintf(PTR__gMansionPath_001c5220,"%s/Mansions/%s/",uVar1,param_1);
  }
  return PTR__gMansionPath_001c5220;
}


// ---- _GetMansionDataFolderPath @ 0x964c6 (130 bytes)

undefined * _GetMansionDataFolderPath(char *param_1)

{
  undefined4 uVar1;
  
  if ((param_1 == (char *)0x0) || (*param_1 == '\0')) {
    uVar1 = _GetGameDataPath();
    _sprintf(PTR__gMansionDataPath_001c523c,"%s/Mansion Data/",uVar1);
  }
  else {
    uVar1 = _GetGameDataPath();
    _sprintf(PTR__gMansionDataPath_001c523c,"%s/Mansion Data/%s",uVar1,param_1);
  }
  return PTR__gMansionDataPath_001c523c;
}


// ---- _GetCustomMansionsFolderPath @ 0x96548 (113 bytes)

undefined * _GetCustomMansionsFolderPath(undefined4 param_1)

{
  undefined4 uVar1;
  
  uVar1 = _GetGameDirectory();
  _sprintf(PTR__gImageFilePath_001c5230,"%s/%s/%s/",uVar1,"Midnight Mansion HD Custom Mansions",
           param_1);
  _printf("Looking for custom mansions in: %s \n",PTR__gImageFilePath_001c5230);
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetPrefsFilePath @ 0x965b9 (25 bytes)

void _GetPrefsFilePath(undefined4 param_1)

{
  _GetAppSupportPath(param_1);
  return;
}


// ---- _GetRealPrefsFilePath @ 0x965d2 (133 bytes)

undefined * _GetRealPrefsFilePath(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = _GetHomeDirectory();
  if (iVar1 == 0) {
    uVar2 = _GetGameDataPath();
    _sprintf(PTR__gImageFilePath_001c5230,"%s/%s",uVar2,param_1);
  }
  else {
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Library/Preferences/%s",iVar1,param_1);
  }
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetAppSupportPath @ 0x96657 (170 bytes)

undefined * _GetAppSupportPath(char *param_1)

{
  int iVar1;
  undefined4 uVar2;
  char *local_10;
  
  iVar1 = _GetHomeDirectory();
  if (*param_1 == '\0') {
    local_10 = "";
  }
  else {
    local_10 = "/";
  }
  if (iVar1 == 0) {
    uVar2 = _GetGameDataPath();
    _sprintf(PTR__gImageFilePath_001c5230,"%s/%s",uVar2,param_1);
  }
  else {
    _sprintf(PTR__gImageFilePath_001c5230,
             "%s/Library/Application Support/ActionSoft/Midnight Mansion%s%s",iVar1,local_10,param_1
            );
  }
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetPlainAppSupportPath @ 0x96701 (96 bytes)

undefined * _GetPlainAppSupportPath(void)

{
  int iVar1;
  undefined *local_20;
  
  iVar1 = _GetHomeDirectory();
  if (iVar1 == 0) {
    local_20 = (undefined *)_GetGameDataPath();
  }
  else {
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Library/Application Support",iVar1);
    local_20 = PTR__gImageFilePath_001c5230;
  }
  return local_20;
}


// ---- _GetSavedGamesFilePath @ 0x96761 (584 bytes)

undefined * _GetSavedGamesFilePath(int param_1,char param_2)

{
  undefined4 uVar1;
  undefined *local_214;
  char local_20b [255];
  char local_10c [260];
  
  if (*PTR__gCurrentPlayerNameExists_001c4830 != '\0') {
    _SWCopyPascalStringToCString(PTR__gCurrentPlayerName_001c48e8,local_10c);
  }
  if (param_1 == 0) {
    if (*PTR__gCurrentPlayerNameExists_001c4830 == '\0') {
      uVar1 = _GetAppSupportPath("");
      _sprintf(PTR__gImageFilePath_001c5230,"%s/Saved Games/",uVar1);
    }
    else {
      uVar1 = _GetAppSupportPath("");
      _sprintf(PTR__gImageFilePath_001c5230,"%s/Saved Games/%s/",uVar1,local_10c);
    }
  }
  else if (*PTR__gCurrentPlayerNameExists_001c4830 == '\0') {
    uVar1 = _GetAppSupportPath("");
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Saved Games/%s",uVar1,param_1);
  }
  else {
    uVar1 = _GetAppSupportPath("");
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Saved Games/%s/%s",uVar1,local_10c,param_1);
  }
  if (param_2 == '\0') {
    _printf("Saved game file path returned: %s \n",PTR__gImageFilePath_001c5230);
    local_214 = PTR__gImageFilePath_001c5230;
  }
  else {
    _strcpy(local_20b,PTR__gMansionFilename_001c489c);
    _StripStringFromEndOfCString(".zip",local_20b,0);
    _StripStringFromEndOfCString(".ZIP",local_20b,0);
    _sprintf(PTR__gSavedGameFilePath_001c521c,"%s/%s",PTR__gImageFilePath_001c5230,local_20b);
    _printf("Saved game file path returned: %s \n",PTR__gSavedGameFilePath_001c521c);
    local_214 = PTR__gSavedGameFilePath_001c521c;
  }
  return local_214;
}


// ---- _GetMoviesFilePath @ 0x969a9 (344 bytes)

undefined * _GetMoviesFilePath(int param_1)

{
  undefined4 uVar1;
  char local_10c [260];
  
  if (*PTR__gCurrentPlayerNameExists_001c4830 != '\0') {
    _SWCopyPascalStringToCString(PTR__gCurrentPlayerName_001c48e8,local_10c);
  }
  if (param_1 == 0) {
    if (*PTR__gCurrentPlayerNameExists_001c4830 == '\0') {
      uVar1 = _GetAppSupportPath("");
      _sprintf(PTR__gImageFilePath_001c5230,"%s/User Movies/",uVar1);
    }
    else {
      uVar1 = _GetAppSupportPath("");
      _sprintf(PTR__gImageFilePath_001c5230,"%s/User Movies/%s/",uVar1,local_10c);
    }
  }
  else if (*PTR__gCurrentPlayerNameExists_001c4830 == '\0') {
    uVar1 = _GetAppSupportPath("");
    _sprintf(PTR__gImageFilePath_001c5230,"%s/User Movies/%s",uVar1,param_1);
  }
  else {
    uVar1 = _GetAppSupportPath("");
    _sprintf(PTR__gImageFilePath_001c5230,"%s/User Movies/%s/%s",uVar1,local_10c,param_1);
  }
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetPlainSavedGamesFilePath @ 0x96b01 (164 bytes)

undefined * _GetPlainSavedGamesFilePath(int param_1)

{
  undefined4 uVar1;
  
  if (param_1 == 0) {
    uVar1 = _GetAppSupportPath("");
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Saved Games/",uVar1);
  }
  else {
    uVar1 = _GetAppSupportPath("");
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Saved Games/%s",uVar1,param_1);
  }
  _printf("GetPlainSavedGamesFilePath returned: %s\n",PTR__gImageFilePath_001c5230);
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetPlayerProfilePath @ 0x96ba5 (138 bytes)

undefined * _GetPlayerProfilePath(int param_1)

{
  undefined4 uVar1;
  
  if (param_1 == 0) {
    uVar1 = _GetAppSupportPath("");
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Player Profiles/",uVar1);
  }
  else {
    uVar1 = _GetAppSupportPath("");
    _sprintf(PTR__gImageFilePath_001c5230,"%s/Player Profiles/%s",uVar1,param_1);
  }
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetSuspendedGameFilePath @ 0x96c2f (293 bytes)

void _GetSuspendedGameFilePath(char *param_1,int param_2)

{
  undefined4 uVar1;
  char local_10c [260];
  
  if (*PTR__gCurrentPlayerNameExists_001c4830 == '\0') {
    if (param_2 == 0) {
      uVar1 = _GetAppSupportPath("");
      _sprintf(param_1,"%s/Suspended Game Data/",uVar1);
    }
    else {
      uVar1 = _GetAppSupportPath("");
      _sprintf(param_1,"%s/Suspended Game Data/%s",uVar1,param_2);
    }
  }
  else {
    _SWCopyPascalStringToCString(PTR__gCurrentPlayerName_001c48e8,local_10c);
    if (param_2 == 0) {
      uVar1 = _GetAppSupportPath("");
      _sprintf(param_1,"%s/Suspended Game Data/%s/",uVar1,local_10c);
    }
    else {
      uVar1 = _GetAppSupportPath("");
      _sprintf(param_1,"%s/Suspended Game Data/%s/%s",uVar1,local_10c,param_2);
    }
  }
  return;
}


// ---- _GetPlainSuspendedGameFilePath @ 0x96d54 (120 bytes)

void _GetPlainSuspendedGameFilePath(char *param_1,int param_2)

{
  undefined4 uVar1;
  
  if (param_2 == 0) {
    uVar1 = _GetAppSupportPath("");
    _sprintf(param_1,"%s/Suspended Game Data/",uVar1);
  }
  else {
    uVar1 = _GetAppSupportPath("");
    _sprintf(param_1,"%s/Suspended Game Data/%s",uVar1,param_2);
  }
  return;
}


// ---- _GetUserMoviesPath @ 0x96dcc (138 bytes)

undefined * _GetUserMoviesPath(int param_1)

{
  undefined4 uVar1;
  
  if (param_1 == 0) {
    uVar1 = _GetAppSupportPath("");
    _sprintf(PTR__gImageFilePath_001c5230,"%s/User Movies/",uVar1);
  }
  else {
    uVar1 = _GetAppSupportPath("");
    _sprintf(PTR__gImageFilePath_001c5230,"%s/User Movies/%s",uVar1,param_1);
  }
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetHomeDirectory @ 0x96e56 (82 bytes)

char * _GetHomeDirectory(void)

{
  uid_t uVar1;
  int iVar2;
  char *local_14;
  
  local_14 = _getenv("HOME");
  if (local_14 == (char *)0x0) {
    uVar1 = _getuid();
    iVar2 = _getpwuid(uVar1);
    if (iVar2 != 0) {
      local_14 = *(char **)(iVar2 + 0x1c);
    }
  }
  return local_14;
}


// ---- _GetGameDataPath @ 0x96ea8 (145 bytes)

undefined * _GetGameDataPath(void)

{
  char local_418 [1024];
  char *local_18;
  char local_11;
  char *local_10;
  
  local_11 = 0;
  local_10 = "Game Data";
  local_18 = (char *)_GetResourcesDirectory();
  local_11 = '\x01';
  _strcpy(local_418,local_18);
  _sprintf(PTR__gImageFilePath_001c5230,"%s/%s",local_418,local_10);
  if (local_11 != '\0') {
    _free(local_18);
  }
  return PTR__gImageFilePath_001c5230;
}


// ---- _GetCurrentDirectory @ 0x96f39 (118 bytes)

undefined * _GetCurrentDirectory(void)

{
  if (*PTR__didSetCWD_001c5244 == '\0') {
    _FixCurrentWorkingDirectory();
    *PTR__gReturnedFilename_001c5254 = 0;
    _getcwd(PTR__gCurrentWorkingDirectory_001c5248,0x400);
    *PTR__didSetCWD_001c5244 = 1;
  }
  return PTR__gCurrentWorkingDirectory_001c5248;
}


// ---- _GetGameDirectory @ 0x96faf (94 bytes)

undefined * _GetGameDirectory(void)

{
  char local_410 [1024];
  char *local_10;
  
  local_10 = (char *)_GetCurrentDirectory();
  _strcpy(local_410,local_10);
  _strcpy(PTR__gDirPath_001c5250,local_410);
  return PTR__gDirPath_001c5250;
}


// ---- _FixCurrentWorkingDirectory @ 0x9700d (327 bytes)

void _FixCurrentWorkingDirectory(void)

{
  char cVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  char *local_18;
  
  if (*PTR__didFixCWD_001c5228 == '\0') {
    uVar2 = _CFBundleGetMainBundle();
    uVar2 = _CFBundleCopyBundleURL(uVar2);
    uVar3 = _CFURLCopyAbsoluteURL(uVar2);
    _CFRelease(uVar2);
    uVar2 = _CFURLCopyFileSystemPath(uVar3,0);
    _CFRelease(uVar3);
    uVar3 = _CFStringGetLength(uVar2);
    iVar4 = _CFStringGetMaximumSizeForEncoding(uVar3,0x8000100);
    local_18 = (char *)_malloc(iVar4 + 2);
    cVar1 = _CFStringGetCString(uVar2,local_18,iVar4 + 1,0x8000100);
    if (cVar1 == '\0') {
      _free(local_18);
      local_18 = (char *)0x0;
    }
    if (local_18 != (char *)0x0) {
      _StripNDirectoriesFromEndOfPath(local_18,1);
      _printf("Fixing current working directory to: %s\n",local_18);
      _chdir(local_18);
      _free(local_18);
    }
    _CFRelease(uVar2);
    *PTR__didFixCWD_001c5228 = 1;
  }
  return;
}


// ---- _GetResourcesDirectory @ 0x97154 (255 bytes)

char * _GetResourcesDirectory(void)

{
  char cVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int iVar6;
  char *local_30;
  char *local_18;
  
  uVar2 = _CFBundleGetMainBundle();
  uVar2 = _CFBundleCopyResourcesDirectoryURL(uVar2);
  uVar3 = _CFURLCopyAbsoluteURL(uVar2);
  uVar4 = _CFURLCopyFileSystemPath(uVar3,0);
  uVar5 = _CFStringGetLength(uVar4);
  iVar6 = _CFStringGetMaximumSizeForEncoding(uVar5,0x8000100);
  local_18 = (char *)_malloc(iVar6 + 2);
  cVar1 = _CFStringGetCString(uVar4,local_18,iVar6 + 1,0x8000100);
  if (cVar1 == '\0') {
    _free(local_18);
    local_18 = (char *)0x0;
  }
  _CFRelease(uVar2);
  _CFRelease(uVar3);
  if (local_18 == (char *)0x0) {
    local_30 = "ERROR: GetResourcesDirectory returned NULL string!";
  }
  else {
    _CFRelease(uVar4);
    local_30 = local_18;
  }
  return local_30;
}


// ---- _ScanDirectoryForResIDs @ 0x97253 (319 bytes)

undefined4 _ScanDirectoryForResIDs(undefined4 param_1,char *param_2)

{
  short sVar1;
  undefined4 local_210;
  char local_20c [500];
  int local_18;
  int local_14;
  int local_10;
  
  *(undefined2 *)PTR__gLastValidLevelResIDIndex_001c5240 = 0;
  _strcpy(PTR__gScannedResType_001c5238,param_2);
  _sprintf(local_20c,"%s/%s",param_1,param_2);
  local_18 = _opendir(local_20c);
  if (local_18 == 0) {
    local_210 = 0xffffffff;
  }
  else {
    do {
      do {
        local_14 = _readdir(local_18);
        if (local_14 == 0) goto LAB_0009736d;
      } while (*(char *)(local_14 + 6) != '\b');
      local_10 = _atoi((char *)(local_14 + 8));
      sVar1 = *(short *)PTR__gLastValidLevelResIDIndex_001c5240;
      *(short *)(PTR__gValidLevelResIDs_001c5234 + sVar1 * 2) = (short)local_10;
      *(short *)PTR__gLastValidLevelResIDIndex_001c5240 = sVar1 + 1;
    } while (*(short *)PTR__gLastValidLevelResIDIndex_001c5240 < 5000);
    _printf("ERROR! ScanDirectoryForResIDs exceeded array bounds!");
    *(undefined2 *)PTR__gLastValidLevelResIDIndex_001c5240 = 0;
LAB_0009736d:
    _closedir(local_18);
    local_210 = 0;
  }
  return local_210;
}


// ---- _ResourceExists @ 0x97392 (43 bytes)

undefined1 _ResourceExists(undefined4 param_1,short param_2)

{
  undefined1 uVar1;
  
  uVar1 = _Resource1Exists(param_1,(int)param_2);
  return uVar1;
}


// ---- _Resource1Exists @ 0x973bd (321 bytes)

undefined1 _Resource1Exists(undefined4 param_1,short param_2)

{
  int iVar1;
  char local_121 [255];
  char local_22 [10];
  int local_18;
  int local_14;
  undefined1 local_d;
  
  _sprintf(local_22,"\'%s\'",param_1);
  iVar1 = _strcmp(local_22,PTR__gScannedResType_001c5238);
  if ((iVar1 == 0) && (0 < *(short *)PTR__gLastValidLevelResIDIndex_001c5240)) {
    for (local_18 = 0; local_18 < *(short *)PTR__gLastValidLevelResIDIndex_001c5240;
        local_18 = local_18 + 1) {
      if (*(short *)(PTR__gValidLevelResIDs_001c5234 + local_18 * 2) == param_2) {
        return 1;
      }
    }
    local_d = 0;
  }
  else {
    _sprintf(local_121,"%s\'%s\'/%05d",PTR__gCurOpenMansionFilePath_001c4124,param_1,(int)param_2);
    local_14 = _SDL_RWFromZZIP(local_121,"rb");
    local_d = local_14 != 0;
    if (local_14 != 0) {
      (**(code **)(local_14 + 0xc))(local_14);
    }
  }
  return local_d;
}


// ---- _DoesFileExist @ 0x974fe (83 bytes)

bool _DoesFileExist(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = _SDL_RWFromFile(param_1,"rb");
  if (iVar1 != 0) {
    (**(code **)(iVar1 + 0xc))(iVar1);
  }
  return iVar1 != 0;
}


// ---- _CountNumSuccessiveFiles @ 0x97551 (140 bytes)

int _CountNumSuccessiveFiles(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  char local_208 [500];
  short local_14;
  char local_11;
  undefined4 local_10;
  
  local_10 = 0;
  for (local_14 = 1; -1 < local_14; local_14 = local_14 + 1) {
    _sprintf(local_208,"%s%s %d%s",param_1,param_2,(int)local_14,param_3);
    local_11 = _DoesFileExist(local_208);
    if (local_11 == '\0') break;
  }
  return local_14 + -1;
}


// ---- _MMCountResources @ 0x975dd (78 bytes)

char _MMCountResources(undefined4 param_1)

{
  char cVar1;
  char cVar2;
  undefined2 local_10;
  
  cVar1 = '\0';
  for (local_10 = 0x80; local_10 != 0x7fff; local_10 = local_10 + 1) {
    cVar2 = _Resource1Exists(param_1,(int)local_10);
    if (cVar2 != '\0') {
      cVar1 = cVar1 + '\x01';
    }
  }
  return cVar1;
}


// ---- _PrintFilesInFolder @ 0x9762b (177 bytes)

undefined4 PrintFilesInFolder(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uStack_20;
  int iStack_10;
  
  iStack_10 = 0;
  iVar1 = _opendir(param_1);
  if (iVar1 == 0) {
    uStack_20 = 0xffffffff;
  }
  else {
    while( true ) {
      iVar2 = _readdir(iVar1);
      if (iVar2 == 0) break;
      if (*(char *)(iVar2 + 6) == '\b') {
        iStack_10 = iStack_10 + 1;
        _printf("\n%s (type %i)",iVar2 + 8,(uint)*(byte *)(iVar2 + 6));
      }
    }
    _printf("\n\nreaddir() found a total of %i files\n",iStack_10);
    _closedir(iVar1);
    uStack_20 = 0;
  }
  return uStack_20;
}


// ---- _CountNumFilesInFolder @ 0x976dc (107 bytes)

int _CountNumFilesInFolder(undefined4 param_1)

{
  short sVar1;
  int iVar2;
  int iVar3;
  undefined4 local_20;
  
  sVar1 = 0;
  iVar2 = _opendir(param_1);
  if (iVar2 == 0) {
    local_20 = 0;
  }
  else {
    while( true ) {
      iVar3 = _readdir(iVar2);
      if (iVar3 == 0) break;
      if (*(char *)(iVar3 + 6) == '\b') {
        sVar1 = sVar1 + 1;
      }
    }
    _closedir(iVar2);
    local_20 = (int)sVar1;
  }
  return local_20;
}


// ---- _MMGetResource @ 0x97747 (70 bytes)

void _MMGetResource(undefined4 param_1,short param_2,undefined4 param_3)

{
  _MMGetResourceUsingPath(param_1,(int)param_2,param_3,PTR__gCurOpenMansionFilePath_001c4124);
  return;
}


// ---- _MMGetResourceUsingPath @ 0x9778d (214 bytes)

void * _MMGetResourceUsingPath(undefined4 param_1,short param_2,size_t param_3,undefined4 param_4)

{
  char local_117 [255];
  void *local_18;
  int local_14;
  undefined4 local_10;
  
  local_18 = (void *)0x0;
  _sprintf(local_117,"%s\'%s\'/%05d",param_4,param_1,(int)param_2);
  local_14 = _SDL_RWFromZZIP(local_117,"rb");
  if (local_14 != 0) {
    local_18 = _calloc(1,param_3);
    if (local_18 != (void *)0x0) {
      local_10 = (**(code **)(local_14 + 4))(local_14,local_18,1,param_3);
    }
    (**(code **)(local_14 + 0xc))(local_14);
  }
  return local_18;
}


// ---- _MMGetMansionVersion @ 0x97863 (201 bytes)

undefined4
_MMGetMansionVersion(undefined4 param_1,short param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  char local_117 [255];
  undefined4 local_18;
  int local_14;
  undefined4 local_10;
  
  local_18 = 0xffffffff;
  _sprintf(local_117,"%s\'%s\'/%05d",param_4,param_1,(int)param_2);
  local_14 = _SDL_RWFromZZIP(local_117,"rb");
  if (local_14 != 0) {
    local_10 = (**(code **)(local_14 + 4))(local_14,&local_18,1,4);
    (**(code **)(local_14 + 0xc))(local_14);
  }
  uVar1 = _SDL_Swap32(local_18);
  return uVar1;
}


// ---- _SDL_Swap32 @ 0x9792c (25 bytes)

uint _SDL_Swap32(uint param_1)

{
  return param_1 >> 0x18 | (param_1 & 0xff0000) >> 8 | (param_1 & 0xff00) << 8 | param_1 << 0x18;
}


// ---- _MMGetCStringFromTEXTResource @ 0x97945 (332 bytes)

undefined1 * _MMGetCStringFromTEXTResource(short param_1)

{
  int iVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  char local_12b [255];
  undefined1 *local_2c;
  undefined1 *local_28;
  int local_24;
  int local_20;
  
  local_2c = (undefined1 *)0x0;
  local_28 = (undefined1 *)0x0;
  _sprintf(local_12b,"%s\'%s\'/%05d.txt",PTR__gCurOpenMansionFilePath_001c4124,"TEXT",(int)param_1);
  local_24 = _SDL_RWFromZZIP(local_12b,"rb");
  if (local_24 == 0) {
    _printf("Couldn\'t load TEXT resource at path: %s \n",local_12b);
  }
  else {
    local_2c = (undefined1 *)_calloc(1,4000);
    if (local_2c != (undefined1 *)0x0) {
      local_20 = (**(code **)(local_24 + 4))(local_24,local_2c,1,4000);
    }
    (**(code **)(local_24 + 0xc))(local_24);
    if (local_20 != 0) {
      local_28 = (undefined1 *)_calloc(1,local_20 + 2);
      puVar2 = local_2c;
      puVar3 = local_28;
      for (iVar1 = local_20; iVar1 != 0; iVar1 = iVar1 + -1) {
        *puVar3 = *puVar2;
        puVar2 = puVar2 + 1;
        puVar3 = puVar3 + 1;
      }
    }
    _free(local_2c);
    local_28[local_20] = 0;
  }
  return local_28;
}


// ---- _GetSPRTResourceSize @ 0x97a91 (212 bytes)

int _GetSPRTResourceSize(short param_1)

{
  char local_117 [255];
  int local_18;
  int local_14;
  undefined4 local_10;
  
  local_18 = 0;
  _sprintf(local_117,"%s\'%s\'/%05d",PTR__gCurOpenMansionFilePath_001c4124,"SPRT",(int)param_1);
  local_14 = _SDL_RWFromZZIP(local_117,"rb");
  if (local_14 != 0) {
    local_10 = (**(code **)(local_14 + 4))(local_14,&local_18,4,1);
    local_18 = _SDL_Swap32(local_18);
    (**(code **)(local_14 + 0xc))(local_14);
  }
  return local_18 * 0x16;
}


// ---- _EnableRecordMovieMenuItem @ 0x97b65 (20 bytes)

void _EnableRecordMovieMenuItem(void)

{
  return;
}


// ---- _SetUpMenuBar @ 0x97b79 (14 bytes)

void _SetUpMenuBar(void)

{
  return;
}


// ---- _OpenGameSupportFiles @ 0x97b87 (14 bytes)

void _OpenGameSupportFiles(void)

{
  return;
}


// ---- _StartUpEditor @ 0x97b95 (14 bytes)

void _StartUpEditor(void)

{
  return;
}


// ---- _ExitToEditor @ 0x97ba3 (14 bytes)

void _ExitToEditor(void)

{
  return;
}


