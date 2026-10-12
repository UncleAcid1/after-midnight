// Ghidra decompilation of functions from original source file:
//   Sources & Headers/High Scores.c

// ---- _InitializeHighScores @ 0x7f95a (502 bytes)

void _InitializeHighScores(void)

{
  undefined1 *puVar1;
  undefined *puVar2;
  uint uVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  short local_28;
  short local_26;
  short local_24;
  short local_22;
  
  _puts("Initializing High Scores, because file can\'t be found.");
  for (local_26 = 0; puVar2 = PTR__gHighScores_001c5028, local_26 < 0xb; local_26 = local_26 + 1) {
    for (local_28 = 0; local_28 < 3; local_28 = local_28 + 1) {
      for (local_24 = 0; local_24 < 10; local_24 = local_24 + 1) {
        puVar1 = puVar2 + (local_28 * 10 + (int)local_24) * 0x40 + local_26 * 0x780 + 4;
        if (local_28 == 0) {
          local_22 = local_24 + 4;
        }
        else if (local_28 == 2) {
          local_22 = local_24 + 8;
        }
        else {
          local_22 = local_24;
        }
        for (; 9 < local_22; local_22 = local_22 + -10) {
        }
        uVar3 = **(byte **)(PTR__defaultNameArray_001c5040 + local_22 * 4) + 1;
        if (0x11 < uVar3) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/High Scores.c",
                     PTR____FUNCTION___27425_001c5024,0x5b);
        }
        puVar4 = *(undefined1 **)(PTR__defaultNameArray_001c5040 + local_22 * 4);
        puVar5 = puVar1;
        for (; uVar3 != 0; uVar3 = uVar3 - 1) {
          *puVar5 = *puVar4;
          puVar4 = puVar4 + 1;
          puVar5 = puVar5 + 1;
        }
        *(int *)(puVar1 + 0x28) = local_24 * -500 + 5000;
        *(int *)(puVar1 + 0x2c) = local_24 * -0x3c + 0x4b0;
        *(int *)(puVar1 + 0x30) = local_24 * -5 + 0x32;
        puVar1[0x34] = 0;
        puVar1[0x35] = 0;
        *(undefined2 *)(puVar1 + 0x36) = 0xffff;
        *(undefined4 *)(puVar1 + 0x38) = 0xffffffff;
        *(undefined4 *)(puVar1 + 0x3c) = 0xffffffff;
      }
    }
  }
  return;
}


// ---- _LoadHighScores @ 0x7fb50 (51 bytes)

void _LoadHighScores(void)

{
  int iVar1;
  
  iVar1 = _LoadHighScoresFromFile();
  if (iVar1 != 0) {
    _InitializeHighScores();
    if (iVar1 == -1) {
      _WriteHighScoresToFile();
    }
  }
  return;
}


// ---- _WriteHighScoresToFile @ 0x7fb83 (199 bytes)

undefined4 _WriteHighScoresToFile(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 local_10;
  
  uVar1 = _GetAppSupportPath("Midnight Mansion HD High Scores");
  iVar2 = _SDL_RWFromFile(uVar1,"wb");
  if (iVar2 == 0) {
    local_10 = 0xffffffff;
  }
  else {
    _ClearFileStickyError();
    _SwapEndiansForHighScoreData();
    *(undefined2 *)PTR__gHighScores_001c5028 = 100;
    _MyWriteDataToFile(iVar2,0x5284,PTR__gHighScores_001c5028);
    _SwapEndiansForHighScoreData();
    (**(code **)(iVar2 + 0xc))(iVar2);
    _SetFileCreatorAndTypeForPath(uVar1,0x4d414e53,0x48494748);
    local_10 = _GetFileStickyError();
  }
  return local_10;
}


// ---- _LoadHighScoresFromFile @ 0x7fc4a (167 bytes)

undefined4 _LoadHighScoresFromFile(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 local_10;
  
  local_10 = 0;
  uVar1 = _GetAppSupportPath("Midnight Mansion HD High Scores");
  iVar2 = _SDL_RWFromFile(uVar1,"rb");
  if (iVar2 == 0) {
    local_10 = 0xffffffff;
  }
  else {
    _printf("Loading high scores from path: %s\n",uVar1);
    _ClearFileStickyError();
    _MyReadDataFromFile(iVar2,0x5284,PTR__gHighScores_001c5028);
    _SwapEndiansForHighScoreData();
    (**(code **)(iVar2 + 0xc))(iVar2);
  }
  return local_10;
}


// ---- _SwapEndiansForHighScoreData @ 0x7fcf1 (265 bytes)

void _SwapEndiansForHighScoreData(void)

{
  int iVar1;
  undefined *puVar2;
  undefined2 uVar3;
  undefined4 uVar4;
  int local_20;
  int local_1c;
  int local_18;
  
  uVar3 = _SDL_Swap16(*(undefined2 *)PTR__gHighScores_001c5028);
  *(undefined2 *)PTR__gHighScores_001c5028 = uVar3;
  for (local_20 = 0; puVar2 = PTR__gHighScores_001c5028, local_20 < 0xb; local_20 = local_20 + 1) {
    for (local_1c = 0; local_1c < 3; local_1c = local_1c + 1) {
      for (local_18 = 0; local_18 < 10; local_18 = local_18 + 1) {
        iVar1 = (local_1c * 10 + local_18) * 0x40 + local_20 * 0x780 + 4;
        uVar4 = _SDL_Swap32(*(undefined4 *)(puVar2 + iVar1 + 0x28));
        *(undefined4 *)(puVar2 + iVar1 + 0x28) = uVar4;
        uVar4 = _SDL_Swap32(*(undefined4 *)(puVar2 + iVar1 + 0x2c));
        *(undefined4 *)(puVar2 + iVar1 + 0x2c) = uVar4;
        uVar4 = _SDL_Swap32(*(undefined4 *)(puVar2 + iVar1 + 0x30));
        *(undefined4 *)(puVar2 + iVar1 + 0x30) = uVar4;
      }
    }
  }
  return;
}


// ---- _SDL_Swap32 @ 0x7fdfa (25 bytes)

uint _SDL_Swap32(uint param_1)

{
  return param_1 >> 0x18 | (param_1 & 0xff0000) >> 8 | (param_1 & 0xff00) << 8 | param_1 << 0x18;
}


// ---- _SDL_Swap16 @ 0x7fe13 (35 bytes)

undefined2 _SDL_Swap16(undefined2 param_1)

{
  return CONCAT11((char)param_1,(char)((ushort)param_1 >> 8));
}


// ---- _SetUpHighScoresScreen @ 0x7fe36 (1231 bytes)

void _SetUpHighScoresScreen(short param_1,char param_2,undefined1 param_3)

{
  undefined4 uVar1;
  SpritePtr newSpriteP;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  short local_e;
  
  if (param_1 < *(short *)PTR__gFirstHighScoresIndex_001c5034) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/High Scores.c",
               PTR____FUNCTION___27573_001c503c,0x10f);
  }
  if (10 < param_1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/High Scores.c",
               PTR____FUNCTION___27573_001c503c,0x110);
  }
  *PTR__gCurrentlyInMansion_001c4908 = 0;
  *PTR__exitToMapScreen_001c5038 = param_3;
  if (*PTR__gCanPlayBothEpisodes_001c4738 == '\0') {
    *(undefined2 *)PTR__gFirstHighScoresIndex_001c5034 = 0;
    *(undefined2 *)PTR__gLastHighScoresIndex_001c5030 = 4;
  }
  else {
    *(undefined2 *)PTR__gFirstHighScoresIndex_001c5034 = 0;
    *(undefined2 *)PTR__gLastHighScoresIndex_001c5030 = 10;
  }
  if (param_2 == '\0') {
    *(undefined2 *)PTR__gCurrentHighScoresScreenIndex_001c4afc =
         *(undefined2 *)PTR__gFirstHighScoresIndex_001c5034;
  }
  else {
    *(short *)PTR__gCurrentHighScoresScreenIndex_001c4afc = param_1;
  }
  _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
  _GiveOSTime();
  local_e = 0x1b2;
  uVar1 = _NewButtonSprite(0xd6,0xd6);
  *(undefined4 *)PTR__gLeftArrowButtonSpriteP_001c4b88 = uVar1;
  if (*(int *)PTR__gLeftArrowButtonSpriteP_001c4b88 != 0) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,
                 *(SpritePtr *)PTR__gLeftArrowButtonSpriteP_001c4b88);
    _SetUpButtonSprite(*(undefined4 *)PTR__gLeftArrowButtonSpriteP_001c4b88,0,1,2,3);
    _InstallButtonProc(*(undefined4 *)PTR__gLeftArrowButtonSpriteP_001c4b88,PTR_LAB_001c4034);
    _SWSetSpriteLocation(*(SpritePtr *)PTR__gLeftArrowButtonSpriteP_001c4b88,434.0,10.0);
    _SWGetSpriteWidth(*(SpritePtr *)PTR__gLeftArrowButtonSpriteP_001c4b88);
    local_e = (short)(int)((float)extraout_ST0 + 10.0 + 434.0);
    if (*(short *)PTR__gCurrentHighScoresScreenIndex_001c4afc ==
        *(short *)PTR__gFirstHighScoresIndex_001c5034) {
      _SetButtonDimmedMode(*(undefined4 *)PTR__gLeftArrowButtonSpriteP_001c4b88,1);
    }
  }
  newSpriteP = (SpritePtr)_NewButtonSprite(0xd7,0xd7);
  if (newSpriteP != (SpritePtr)0x0) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,newSpriteP);
    _SetUpButtonSprite(newSpriteP,0,1,2,0);
    _InstallButtonProc(newSpriteP,PTR_LAB_001c4034);
    _SWSetSpriteLocation(newSpriteP,(float)(int)local_e,10.0);
    _SWGetSpriteWidth(newSpriteP);
    local_e = (short)(int)((float)extraout_ST0_00 + 10.0 + (float)(int)local_e);
  }
  uVar1 = _NewButtonSprite(0xd6,0xd8);
  *(undefined4 *)PTR__gRightArrowButtonSpriteP_001c4c2c = uVar1;
  if (*(int *)PTR__gRightArrowButtonSpriteP_001c4c2c != 0) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,
                 *(SpritePtr *)PTR__gRightArrowButtonSpriteP_001c4c2c);
    _SetUpButtonSprite(*(undefined4 *)PTR__gRightArrowButtonSpriteP_001c4c2c,0,1,2,3);
    _InstallButtonProc(*(undefined4 *)PTR__gRightArrowButtonSpriteP_001c4c2c,PTR_LAB_001c4034);
    _SWSetSpriteFlipX(*(SpriteRec **)PTR__gRightArrowButtonSpriteP_001c4c2c,'\x01');
    _SWSetSpriteLocation
              (*(SpritePtr *)PTR__gRightArrowButtonSpriteP_001c4c2c,(float)(int)local_e,10.0);
    if (*(short *)PTR__gCurrentHighScoresScreenIndex_001c4afc ==
        *(short *)PTR__gLastHighScoresIndex_001c5030) {
      _SetButtonDimmedMode(*(undefined4 *)PTR__gRightArrowButtonSpriteP_001c4c2c,1);
    }
  }
  _MMShieldCursor();
  _MMSetCursorForGame();
  _DrawContentsForHighScoresScreen((int)*(short *)PTR__gCurrentHighScoresScreenIndex_001c4afc);
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 4;
  _UpdateMenuBar();
  *PTR__gSpriteWorldIsValid_001c48c8 = 1;
  _GiveOSTime();
  _DoGameFadeToNormal(0x1e);
  return;
}


// ---- _HandleHighScoresScreenCallback @ 0x80305 (596 bytes)

void _HandleHighScoresScreenCallback(void)

{
  undefined4 local_16;
  undefined2 local_12;
  undefined1 local_f;
  short local_e;
  
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  if (*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) != '\0') {
    local_f = _MMGetMouse(&local_16);
    _SWConvertCoordinates(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_16);
    local_e = _ProcessButtonSprites
                        (*(undefined4 *)PTR__gTopSpriteLayerP_001c41b4,
                         *(undefined4 *)PTR__gTopSpriteLayerP_001c41b4,local_16,local_f,0,0);
    if (local_e != -1) {
      _AnimateOneFrame(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
      if (local_e == 0xd6) {
        *(short *)PTR__gCurrentHighScoresScreenIndex_001c4afc =
             *(short *)PTR__gCurrentHighScoresScreenIndex_001c4afc + -1;
        _SetButtonDimmedMode(*(undefined4 *)PTR__gRightArrowButtonSpriteP_001c4c2c,0);
        if (*(short *)PTR__gCurrentHighScoresScreenIndex_001c4afc ==
            *(short *)PTR__gFirstHighScoresIndex_001c5034) {
          _SetButtonDimmedMode(*(undefined4 *)PTR__gLeftArrowButtonSpriteP_001c4b88,1);
        }
        _DrawContentsForHighScoresScreen((int)*(short *)PTR__gCurrentHighScoresScreenIndex_001c4afc)
        ;
      }
      else if (local_e == 0xd8) {
        *(short *)PTR__gCurrentHighScoresScreenIndex_001c4afc =
             *(short *)PTR__gCurrentHighScoresScreenIndex_001c4afc + 1;
        _SetButtonDimmedMode(*(undefined4 *)PTR__gLeftArrowButtonSpriteP_001c4b88,0);
        if (*(short *)PTR__gCurrentHighScoresScreenIndex_001c4afc ==
            *(short *)PTR__gLastHighScoresIndex_001c5030) {
          _SetButtonDimmedMode(*(undefined4 *)PTR__gRightArrowButtonSpriteP_001c4c2c,1);
        }
        _DrawContentsForHighScoresScreen((int)*(short *)PTR__gCurrentHighScoresScreenIndex_001c4afc)
        ;
      }
      else if (local_e == 0xd7) {
        _DoGameFadeToBlack(0x1e,0);
        _ExitHighScoresScreen();
        if (*(short *)PTR__gCurrentMansionNumber_001c4824 < 5) {
          local_12 = 1;
        }
        else {
          local_12 = 2;
        }
        if (*PTR__exitToMapScreen_001c5038 == '\0') {
          _SetUpTitleScreen();
        }
        else {
          _SetUpSelectMansionScreen(local_12);
        }
      }
    }
  }
  return;
}


// ---- _DrawContentsForHighScoresScreen @ 0x80559 (1717 bytes)

void _DrawContentsForHighScoresScreen(short param_1)

{
  undefined4 uVar1;
  SpritePtr local_354;
  char local_350 [255];
  char local_251 [255];
  undefined2 local_152;
  undefined2 local_150;
  undefined2 local_14e;
  undefined2 local_14c;
  undefined2 local_14a;
  undefined2 local_148;
  undefined2 local_146;
  undefined2 local_144;
  undefined2 local_142;
  undefined2 local_140;
  undefined2 local_13e;
  undefined2 local_13c;
  undefined2 local_13a;
  undefined1 local_138 [256];
  undefined2 local_38;
  undefined2 local_36;
  undefined1 local_34;
  undefined1 local_33;
  undefined2 local_32;
  undefined2 *local_30;
  undefined *local_2c;
  char *local_28;
  char *local_24;
  short local_1e;
  short local_1c;
  short local_1a;
  short local_18;
  short local_16;
  undefined4 local_14;
  int local_10;
  
  local_13e = 0xffff;
  local_13c = 0xffff;
  local_13a = 0;
  local_144 = 0xffff;
  local_142 = 0x51eb;
  local_140 = 0x51eb;
  local_14a = 0xffff;
  local_148 = 0xffff;
  local_146 = 0xffff;
  _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gTextSpriteLayerP_001c4804);
  _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gBottomSpriteLayerP_001c4140);
  uVar1 = _GetScreensFolderPath();
  local_10 = _LoadAndInstallBackgroundSprite(&local_354,uVar1,"highscores.jpg",0xffffffff,0);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/High Scores.c",
               PTR____FUNCTION___27718_001c502c,0x1de);
  }
  _SWSetSpriteScaledSize
            (local_354,SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38),0),
             SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a),0));
  _SetPascalString(local_138,"\rBlackChancery");
  local_36 = 0x26;
  local_34 = 1;
  local_33 = 0;
  local_32 = 2;
  _SWSetRect(&local_152,0,0,0x1b8,0x22);
  local_14 = _GetMansionNameFromIndex((int)param_1);
  _MyDrawPascalTextJustified(local_14,local_138,&local_152,&local_14a);
  _SetPascalString(local_138,"\x05Arial");
  local_32 = 0;
  local_38 = 0;
  local_36 = 0xc;
  local_34 = 1;
  local_33 = 0;
  if ((param_1 < 0) || (0xb < param_1)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/High Scores.c",
               PTR____FUNCTION___27718_001c502c,0x1fa);
  }
  local_2c = PTR__gHighScores_001c5028 + param_1 * 0x780 + 4;
  for (local_18 = 0; local_18 < 3; local_18 = local_18 + 1) {
    local_1a = 0;
    for (local_1c = 0; local_1c < 2; local_1c = local_1c + 1) {
      for (local_1e = 1; local_1e < 6; local_1e = local_1e + 1) {
        local_16 = local_1e * 0x18 + local_18 * 0x90 + 0x34;
        local_28 = local_2c + (local_18 * 10 + (int)local_1a) * 0x40;
        if (local_28[0x34] == '\0') {
          local_30 = &local_144;
        }
        else {
          local_30 = &local_14a;
        }
        _SWCopyPascalStringToCString(local_28,local_350);
        _sprintf(local_251," %d. %s",local_1a + 1,local_350);
        _MyDrawText(local_251,local_138,(int)(short)(local_1c * 0x140 + 2),(int)local_16,local_30);
        _sprintf(local_251,"%d",*(undefined4 *)(local_28 + 0x28));
        _MyDrawText(local_251,local_138,(int)(short)(local_1c * 0x140 + 0x88),(int)local_16,
                    &local_13e);
        if (*(int *)(local_28 + 0x2c) % 0x3c < 10) {
          local_24 = "0";
        }
        else {
          local_24 = "";
        }
        _sprintf(local_251,"%d:%s%d",*(int *)(local_28 + 0x2c) / 0x3c,local_24,
                 *(int *)(local_28 + 0x2c) % 0x3c);
        _MyDrawText(local_251,local_138,(int)(short)(local_1c * 0x140 + 200),(int)local_16,
                    &local_13e);
        _sprintf(local_251,"%d%%",*(undefined4 *)(local_28 + 0x30));
        _MyDrawText(local_251,local_138,(int)(short)(local_1c * 0x140 + 0x100),(int)local_16,
                    &local_13e);
        local_1a = local_1a + 1;
      }
    }
  }
  _MMRenderSceneForWorkAreaSprite(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  local_152 = 0;
  local_150 = 0;
  local_14e = 0x1e0;
  local_14c = 0x280;
  _SWAutoScaleRect(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_152);
  local_10 = _SWCreateSpriteFromWorkArea
                       (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_354,0,
                        *(int *)PTR__gSpriteWorldP_001c40d0 + 0x84,0);
  if (local_10 == 0) {
    _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gTextSpriteLayerP_001c4804);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBottomSpriteLayerP_001c4140,local_354);
    _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
  }
  return;
}


// ---- _ExitHighScoresScreen @ 0x80c0e (73 bytes)

void _ExitHighScoresScreen(void)

{
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
  _UpdateMenuBar();
  *PTR__gSpriteWorldIsValid_001c48c8 = 0;
  _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  return;
}


// ---- _CheckForHighScore @ 0x80c57 (809 bytes)

undefined1 _CheckForHighScore(short param_1,short param_2,int param_3,short param_4,char param_5)

{
  char cVar1;
  char local_21c [256];
  char local_11c [256];
  undefined *local_1c;
  char *local_18;
  short local_12;
  undefined1 local_10;
  char local_f;
  short local_e;
  
  local_f = '\0';
  local_10 = 0;
  local_e = 0;
  local_1c = PTR__gHighScores_001c5028 + param_1 * 0x780 + 4;
  local_12 = 0;
  do {
    if (9 < local_12) {
LAB_00080eba:
      if ((local_e != 0) && (param_5 != '\0')) {
        cVar1 = _IsMMRegistered(1);
        if (cVar1 != '\0') {
          _DeleteLicenseFileIfPirated("Midnight Mansion HD Episode 1 License");
          _DeleteLicenseFileIfPirated("Midnight Mansion HD Episode 2 License");
          _DeleteLicenseFileIfPirated("Midnight Mansion HD License");
        }
        _SWCopyPascalStringToCString(local_18,local_21c);
        _ASWLaunchHighScore(local_21c,(int)param_1,(int)param_2,param_3,(int)param_4,
                            *(undefined4 *)(local_18 + 0x2c));
      }
      _WriteHighScoresToFile();
      return local_10;
    }
    local_18 = local_1c + (param_2 * 10 + (int)local_12) * 0x40;
    if (((*(int *)(local_18 + 0x28) < param_3) ||
        ((*(int *)(local_18 + 0x28) == param_3 && (*(int *)(local_18 + 0x30) < (int)param_4)))) ||
       ((*(int *)(local_18 + 0x28) == param_3 &&
        (((int)param_4 == *(int *)(local_18 + 0x30) &&
         (*(int *)PTR__gTimeSpentInMansion_001c46fc < *(int *)(local_18 + 0x2c))))))) {
      if ((param_5 != '\0') && (*PTR__gFullScreenWindow_001c48ec != '\0')) {
        local_f = *PTR__gFullScreenWindow_001c48ec;
        _ToggleFullscreen();
        _DoBeatTheMansionScreen(1);
      }
      local_e = _MMDoEnterNameDialog(PTR__gDefaultHighScoreName_001c4fe4,local_11c,param_5);
      if (local_e != -1) {
        _SWCopyPascalString(local_11c,PTR__gDefaultHighScoreName_001c4fe4);
        _WriteProfileToDisk();
      }
      if ((local_f != '\0') && ((local_e != 1 || (param_5 == '\0')))) {
        _DoGameFadeToBlack(1,0);
        _ToggleFullscreen();
      }
      if (local_e == -1) {
        return 0;
      }
      *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
      _ShiftScoresLowerInList((int)param_1,(int)param_2,(int)local_12);
      _SWCopyPascalString(local_11c,local_18);
      *(int *)(local_18 + 0x28) = param_3;
      *(undefined4 *)(local_18 + 0x2c) = *(undefined4 *)PTR__gTimeSpentInMansion_001c46fc;
      *(int *)(local_18 + 0x30) = (int)param_4;
      local_18[0x34] = param_5;
      local_10 = 1;
      goto LAB_00080eba;
    }
    local_12 = local_12 + 1;
  } while( true );
}


// ---- _DeleteLicenseFileIfPirated @ 0x80f80 (118 bytes)

void _DeleteLicenseFileIfPirated(undefined4 param_1)

{
  int iVar1;
  undefined1 local_214 [7];
  undefined1 local_20d [256];
  undefined1 local_10d [256];
  char local_d;
  
  iVar1 = _MMReadLicenseFile(local_10d,local_20d,local_214,param_1);
  if (iVar1 != 0) {
    local_d = _CheckIfRegCodeWasPirated(local_10d,local_20d);
    if (local_d != '\0') {
      _ChangeHiddenPiracyValue(1);
      _MMDeleteLicenseFile(param_1);
    }
  }
  return;
}


// ---- _ShiftScoresLowerInList @ 0x80ff6 (335 bytes)

void _ShiftScoresLowerInList(short param_1,short param_2,short param_3)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  undefined *puVar4;
  short local_e;
  
  if (param_3 < 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/High Scores.c",
               PTR____FUNCTION___27931_001c5020,0x2f1);
  }
  puVar4 = PTR__gHighScores_001c5028;
  iVar2 = param_1 * 0x780 + 4;
  for (local_e = 8; param_3 <= local_e; local_e = local_e + -1) {
    puVar1 = (undefined4 *)(puVar4 + (param_2 * 10 + (int)local_e) * 0x40 + iVar2);
    iVar3 = (param_2 * 10 + (int)local_e) * 0x40 + iVar2;
    *(undefined4 *)(puVar4 + iVar3 + 0x40) = *puVar1;
    *(undefined4 *)(puVar4 + iVar3 + 0x44) = puVar1[1];
    *(undefined4 *)(puVar4 + iVar3 + 0x48) = puVar1[2];
    *(undefined4 *)(puVar4 + iVar3 + 0x4c) = puVar1[3];
    *(undefined4 *)(puVar4 + iVar3 + 0x50) = puVar1[4];
    *(undefined4 *)(puVar4 + iVar3 + 0x54) = puVar1[5];
    *(undefined4 *)(puVar4 + iVar3 + 0x58) = puVar1[6];
    *(undefined4 *)(puVar4 + iVar3 + 0x5c) = puVar1[7];
    *(undefined4 *)(puVar4 + iVar3 + 0x60) = puVar1[8];
    *(undefined4 *)(puVar4 + iVar3 + 100) = puVar1[9];
    *(undefined4 *)(puVar4 + iVar3 + 0x68) = puVar1[10];
    *(undefined4 *)(puVar4 + iVar3 + 0x6c) = puVar1[0xb];
    *(undefined4 *)(puVar4 + iVar3 + 0x70) = puVar1[0xc];
    *(undefined4 *)(puVar4 + iVar3 + 0x74) = puVar1[0xd];
    *(undefined4 *)(puVar4 + iVar3 + 0x78) = puVar1[0xe];
    *(undefined4 *)(puVar4 + iVar3 + 0x7c) = puVar1[0xf];
  }
  return;
}


