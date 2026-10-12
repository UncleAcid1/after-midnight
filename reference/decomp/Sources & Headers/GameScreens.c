// Ghidra decompilation of functions from original source file:
//   Sources & Headers/GameScreens.c

// ---- _SetUpActionSoftScreen @ 0x48fbc (757 bytes)

void _SetUpActionSoftScreen(void)

{
  int iVar1;
  
  if (*(int *)PTR__gSpriteWorldP_001c40d0 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___27474_001c4bec,0x68);
  }
  iVar1 = _CreateSpriteFromPictResource
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,PTR__grayActionSoftSpriteP_001c4b9c,
                     0,0x152c,0x152c,1,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___27474_001c4bec,0x6d);
  }
  iVar1 = _CreateSpriteFromPictResource
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,PTR__colorActionSoftSpriteP_001c4b6c
                     ,0,0x1536,0x1536,1,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___27474_001c4bec,0x72);
  }
  _SWSetSpriteTranslucencyLevel(*(SpritePtr *)PTR__colorActionSoftSpriteP_001c4b6c,0);
  _SWSetSpriteTranslucencyLevel(*(SpritePtr *)PTR__grayActionSoftSpriteP_001c4b9c,0);
  *(undefined4 *)(*(int *)PTR__colorActionSoftSpriteP_001c4b6c + 0xe8) = 100;
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,
               *(SpritePtr *)PTR__grayActionSoftSpriteP_001c4b9c);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,
               *(SpritePtr *)PTR__colorActionSoftSpriteP_001c4b6c);
  _SWCenterSpriteInRect
            (*(undefined4 *)PTR__colorActionSoftSpriteP_001c4b6c,
             *(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
  _SWSetSpriteLocation
            (*(SpritePtr *)PTR__grayActionSoftSpriteP_001c4b9c,
             (float)(int)*(short *)(*(int *)PTR__colorActionSoftSpriteP_001c4b6c + 0x38),
             (float)(int)*(short *)(*(int *)PTR__colorActionSoftSpriteP_001c4b6c + 0x36));
  *(undefined4 *)(*(int *)PTR__grayActionSoftSpriteP_001c4b9c + 0xe8) = 0;
  _SWSetSpriteFrameProc
            (*(SpritePtr *)PTR__grayActionSoftSpriteP_001c4b9c,
             (FrameProcPtr_t)PTR_ActionSoftLogoGrayFrameProc_001c4c5c);
  _SWSetSpriteFrameTime(*(SpritePtr *)PTR__grayActionSoftSpriteP_001c4b9c,0x10);
  *(undefined4 *)(*(int *)PTR__grayActionSoftSpriteP_001c4b9c + 0xec) = 1;
  _SWSetSpriteFrameProc
            (*(SpritePtr *)PTR__colorActionSoftSpriteP_001c4b6c,
             (FrameProcPtr_t)PTR_ActionSoftLogoFrameProc_001c4bd0);
  _SWSetSpriteFrameTime(*(SpritePtr *)PTR__colorActionSoftSpriteP_001c4b6c,0x10);
  *(undefined4 *)(*(int *)PTR__colorActionSoftSpriteP_001c4b6c + 0xec) = 1;
  _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
  return;
}


// ---- _DoActionSoftScreen @ 0x492b1 (383 bytes)

void _DoActionSoftScreen(void)

{
  bool bVar1;
  char cVar2;
  
  bVar1 = false;
  if (*(int *)PTR__colorActionSoftSpriteP_001c4b6c == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___27516_001c4b50,0x96);
  }
  while (!bVar1) {
    _GiveOSTime();
    _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
    _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
    if (*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) != '\0') {
      _ProcessSDLEvents();
    }
    if ((*(int *)(*(int *)PTR__colorActionSoftSpriteP_001c4b6c + 0xec) == 0) &&
       (0x28 < *(int *)(*(int *)PTR__colorActionSoftSpriteP_001c4b6c + 0xe8))) {
      *(undefined4 *)(*(int *)PTR__colorActionSoftSpriteP_001c4b6c + 0xec) = 1;
      _SWSetSpriteFrameProc
                (*(SpritePtr *)PTR__colorActionSoftSpriteP_001c4b6c,(FrameProcPtr_t)PTR_LAB_001c4bd8
                );
      _SWRemoveSpriteFromAnimation(*(SpritePtr *)PTR__grayActionSoftSpriteP_001c4b9c,'\x01');
    }
    if ((*(int *)(*(int *)PTR__colorActionSoftSpriteP_001c4b6c + 0xec) == 0) &&
       (*(int *)(*(int *)PTR__colorActionSoftSpriteP_001c4b6c + 0xe8) == -1)) {
      bVar1 = true;
    }
    cVar2 = _WasMouseClicked();
    if (((cVar2 != '\0') || (cVar2 = _WasAKeyPressed(), cVar2 != '\0')) ||
       (cVar2 = _WasGameQuit(), cVar2 != '\0')) {
      bVar1 = true;
    }
  }
  _AnimateOneFrame(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  return;
}


// ---- _ActionSoftLogoFrameProc @ 0x49430 (193 bytes)

void ActionSoftLogoFrameProc(SpritePtr param_1)

{
  uint uVar1;
  
  if (param_1->misc1 == 0) {
    param_1->userData = param_1->userData + 1;
  }
  else if (param_1->userData < 1) {
    uVar1 = _SWGetSpriteTranslucencyLevel(param_1);
    if (uVar1 < 0xfda7) {
      uVar1 = _SWGetSpriteTranslucencyLevel(param_1);
      _SWSetSpriteTranslucencyLevel(param_1,uVar1 + 600);
      _SWInvalidateSprite(param_1);
    }
    else {
      param_1->misc1 = 0;
      _SWSetSpriteTranslucencyLevel(param_1,0xffff);
    }
  }
  else {
    param_1->userData = param_1->userData + -1;
  }
  return;
}


// ---- _ActionSoftLogoFadeOutFrameProc @ 0x494f1 (131 bytes)

void ActionSoftLogoFadeOutFrameProc(SpritePtr param_1)

{
  uint uVar1;
  
  uVar1 = _SWGetSpriteTranslucencyLevel(param_1);
  if (uVar1 < 0x4b1) {
    param_1->misc1 = 0;
    _SWSetSpriteTranslucencyLevel(param_1,0);
    param_1->userData = -1;
  }
  else {
    uVar1 = _SWGetSpriteTranslucencyLevel(param_1);
    _SWSetSpriteTranslucencyLevel(param_1,uVar1 - 0x4b0);
    _SWInvalidateSprite(param_1);
  }
  return;
}


// ---- _ActionSoftLogoGrayFrameProc @ 0x49574 (118 bytes)

void ActionSoftLogoGrayFrameProc(SpritePtr param_1)

{
  uint uVar1;
  
  uVar1 = _SWGetSpriteTranslucencyLevel(param_1);
  if (uVar1 < 0xfbb3) {
    uVar1 = _SWGetSpriteTranslucencyLevel(param_1);
    _SWSetSpriteTranslucencyLevel(param_1,uVar1 + 0x44c);
    _SWInvalidateSprite(param_1);
  }
  else {
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
    param_1->misc1 = 0;
  }
  return;
}


// ---- _DisplaySplashScreen @ 0x495ea (592 bytes)

void _DisplaySplashScreen(void)

{
  short sVar1;
  undefined4 uVar2;
  SWMovementType horizLoc;
  SpritePtr local_14;
  int local_10;
  
  local_10 = 0;
  if (*(int *)PTR__gSpriteWorldP_001c40d0 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___27607_001c4bbc,0x148);
  }
  uVar2 = _GetScreensFolderPath();
  local_10 = _LoadAndInstallBackgroundSprite(&local_14,uVar2,"splash.jpg",0xffffffff,0);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___27607_001c4bbc,0x14b);
  }
  _SWSetSpriteScaledSize(local_14,0,0);
  local_10 = _CreateSpriteFromPictResource
                       (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,PTR__gLoadingTextSpriteP_001c4be8
                        ,0,0x15e0,0x15e0,3,4);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___27607_001c4bbc,0x14f);
  }
  _MySetSpriteScaledFactor(*(undefined4 *)PTR__gLoadingTextSpriteP_001c4be8,0x3f000000,0x3f000000);
  _SWLockSprite(*(SpritePtr *)PTR__gLoadingTextSpriteP_001c4be8);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,
               *(SpritePtr *)PTR__gLoadingTextSpriteP_001c4be8);
  _SWCenterSpriteInRect
            (*(undefined4 *)PTR__gLoadingTextSpriteP_001c4be8,
             *(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
  sVar1 = *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x80);
  horizLoc = _SWGetSpriteHorizLoc(*(SpritePtr *)PTR__gLoadingTextSpriteP_001c4be8);
  _SWMoveSprite(*(SpritePtr *)PTR__gLoadingTextSpriteP_001c4be8,horizLoc,(float)(sVar1 + -0x19));
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
  return;
}


// ---- _ChangeSplashScreenLoadingText @ 0x4983a (131 bytes)

void _ChangeSplashScreenLoadingText(short param_1)

{
  if (*(int *)PTR__gLoadingTextSpriteP_001c4be8 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___27641_001c4b8c,0x166);
  }
  _SWSetCurrentFrameIndex(*(SpritePtr *)PTR__gLoadingTextSpriteP_001c4be8,(int)param_1);
  _AnimateOneFrame(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  return;
}


// ---- _SetUpTitleScreen @ 0x498bd (245 bytes)

void _SetUpTitleScreen(void)

{
  char cVar1;
  int iVar2;
  undefined1 local_10c [255];
  char local_d;
  
  _SetUpTitleScreenMM1();
  cVar1 = _IsMMRegistered(2);
  if (cVar1 == '\0') {
    iVar2 = _GetRegNameFromClipboard("Registered To:");
    if (iVar2 != 0) {
      iVar2 = _GetRegCodeFromClipboard("Registration code:");
      if (iVar2 != 0) {
        _PrepareGameForDialogBox(0x17);
        local_d = _DoEnterRegistrationCodeDialog(0x109,local_10c);
        _RestoreGameAfterDialogBox(0);
        if (local_d != '\0') {
          _RestoreGameAfterDialogBox(0);
          _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,local_10c,3);
          _ChangeHiddenPiracyValue(0);
          if (*PTR__gCanPlayBothEpisodes_001c4738 != '\0') {
            _ExitTitleScreen();
            _SetUpTitleScreen();
          }
        }
      }
    }
  }
  _UpdateMenuBar();
  return;
}


// ---- _SetUpTitleScreenMM1 @ 0x499b2 (4659 bytes)

void _SetUpTitleScreenMM1(void)

{
  short sVar1;
  char cVar2;
  undefined2 uVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  SWError SVar7;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  longdouble extraout_ST0_01;
  longdouble extraout_ST0_02;
  longdouble extraout_ST0_03;
  longdouble extraout_ST0_04;
  longdouble extraout_ST0_05;
  longdouble extraout_ST0_06;
  longdouble extraout_ST0_07;
  longdouble extraout_ST0_08;
  longdouble extraout_ST0_09;
  longdouble extraout_ST0_10;
  longdouble extraout_ST0_11;
  longdouble extraout_ST0_12;
  longdouble extraout_ST0_13;
  longdouble extraout_ST0_14;
  longdouble extraout_ST0_15;
  float fVar8;
  SWMovementType SVar9;
  SWMovementType SVar10;
  undefined2 local_44;
  undefined2 local_42;
  undefined2 local_40;
  undefined2 local_3e;
  undefined1 local_3c [8];
  undefined4 local_34;
  undefined4 local_30;
  SpritePtr local_2c;
  SpritePtr local_28;
  short local_24;
  short local_22;
  short local_20;
  short local_1e;
  int local_1c;
  short local_16;
  int local_14;
  int local_10;
  
  _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  _SWSetDesignResolution(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x500,0x3c0);
  local_44 = 0;
  local_42 = 0;
  local_40 = 0x3c0;
  local_3e = 0x500;
  _DrawTitleScreenBackground(local_3c);
  _GiveOSTime();
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
  _LoadTitleScreenSprites();
  local_1c = *(int *)PTR__gTitleScreenSpriteArray_001c4160;
  for (local_24 = 0; (int)local_24 < *(int *)(local_1c + 0x84); local_24 = local_24 + 1) {
    _SWSetFrameCollisionInset
              (*(FramePtr *)(*(int *)(local_1c + 0x78) + local_24 * 4),0x1e,8,0x16,0x16);
  }
  _IsMMRegistered(2);
  local_22 = 0x11c;
  for (local_24 = 0; local_24 < 7; local_24 = local_24 + 1) {
    local_2c = (SpritePtr)_NewTitleScreenButtonSprite(0,(int)local_24);
    if (local_2c == (SpritePtr)0x0) {
      _MMFatalErrorCallback
                (0xffffffff,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                 PTR____FUNCTION___27698_001c4b60,0x1ec);
    }
    else {
      _SWSetCurrentFrameIndex(local_2c,local_24 * 2);
      _SetUpButtonSprite(local_2c,(int)(short)(local_24 * 2),(int)(short)(local_24 * 2 + 1),
                         (int)(short)(local_24 * 2),0);
      _InstallButtonProc(local_2c,PTR_MMTitleScreenButtonProc_001c4c50);
      _SetButtonPushedOffset(local_2c,0,0);
      _SWSetSpriteScaledSize(local_2c,0,0);
      local_30._0_2_ = (local_2c->destFrameRect).bottom;
      local_30._2_2_ = (local_2c->destFrameRect).right;
      local_34._0_2_ = (local_2c->destFrameRect).top;
      local_34._2_2_ = (local_2c->destFrameRect).left;
      _SWCenterRect(&local_34,*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
      _SWOffsetRect(&local_34,0xfffffeca,(int)(short)(local_22 - (short)local_34));
      _SWSetSpriteLocation(local_2c,(float)(int)local_34._2_2_,(float)(int)(short)local_34);
      _MakeTitleScreenSpriteFadeIn(local_2c);
      iVar4 = (int)local_22;
      _SWGetSpriteHeight(local_2c);
      local_22 = (short)(int)((float)iVar4 + (float)extraout_ST0);
    }
  }
  cVar2 = _IsMMRegistered(2);
  if (cVar2 == '\0') {
    local_28 = (SpritePtr)_NewTitleScreenSprite(3);
    local_30._0_2_ = (local_28->destFrameRect).bottom;
    local_30._2_2_ = (local_28->destFrameRect).right;
    local_34._0_2_ = (local_28->destFrameRect).top;
    local_34._2_2_ = (local_28->destFrameRect).left;
    _SWGetSpriteHeight(local_28);
    _SWOffsetRect(&local_34,0,(int)(short)(int)((960.0 - (float)extraout_ST0_00) + 16.0));
    _SWSetSpriteLocation(local_28,(float)(int)local_34._2_2_,(float)(int)(short)local_34);
    _SWSetSpriteBlendMode(local_28,0x302,1);
    local_1e = _GetRandom(1,8);
    if (local_1e < 5) {
      _SWSetSpriteColor(local_28,0xffff,0xffff,0);
    }
    else if (local_1e < 7) {
      _SWSetSpriteColor(local_28,0,0xffff,0);
    }
    else if (local_1e == 7) {
      _SWSetSpriteColor(local_28,0xffff,0,0);
    }
    else {
      _SWSetSpriteColor(local_28,0xffff,0,0xffff);
    }
    _MakeTitleScreenSpriteFadeIn(local_28);
    _SWSetSpriteFrameProc(local_28,(FrameProcPtr_t)PTR_LAB_001c4c18);
    local_28->misc1 = 1;
  }
  for (local_24 = 0; local_24 < 4; local_24 = local_24 + 1) {
    if (((local_24 != 0) || (cVar2 = _IsMMRegistered(2), cVar2 == '\0')) && (local_24 != 2)) {
      local_2c = (SpritePtr)_NewTitleScreenButtonSprite(2,(int)(short)(local_24 + 10));
      if (local_2c == (SpritePtr)0x0) {
        _MMFatalErrorCallback
                  (0xffffffff,
                   "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                   PTR____FUNCTION___27698_001c4b60,0x23b);
      }
      else {
        _SWSetCurrentFrameIndex(local_2c,local_24 * 2);
        _SetUpButtonSprite(local_2c,(int)(short)(local_24 * 2),(int)(short)(local_24 * 2 + 1),
                           (int)(short)(local_24 * 2),0);
        _InstallButtonProc(local_2c,PTR_MMTabButtonProc_001c4b44);
        _SetButtonPushedOffset(local_2c,0,0);
        local_30._0_2_ = (local_2c->destFrameRect).bottom;
        local_30._2_2_ = (local_2c->destFrameRect).right;
        local_34._0_2_ = (local_2c->destFrameRect).top;
        local_34._2_2_ = (local_2c->destFrameRect).left;
        if (local_24 == 0) {
          _SWGetSpriteHeight(local_2c);
          _SWOffsetRect(&local_34,0x10,(int)(short)(int)(960.0 - (float)extraout_ST0_01));
        }
        else {
          local_16 = local_24;
          if (2 < local_24) {
            local_16 = local_24 + -1;
          }
          cVar2 = _IsMMRegistered(2);
          if (cVar2 == '\0') {
            _SWGetSpriteHeight(local_2c);
            iVar4 = (int)local_16;
            _SWGetSpriteWidth(local_2c);
            _SWOffsetRect(&local_34,(int)(short)(int)((float)iVar4 * (float)extraout_ST0_05 + 32.0),
                          (int)(short)(int)(960.0 - (float)extraout_ST0_04));
          }
          else {
            _SWGetSpriteHeight(local_2c);
            iVar4 = (int)local_16;
            _SWGetSpriteWidth(local_2c);
            _SWOffsetRect(&local_34,(int)(short)(int)((float)(iVar4 + -1) * (float)extraout_ST0_03),
                          (int)(short)(int)(960.0 - (float)extraout_ST0_02));
          }
        }
        _SWSetSpriteLocation(local_2c,(float)(int)local_34._2_2_,(float)(int)(short)local_34);
        _MakeTitleScreenSpriteFadeIn(local_2c);
        if (local_24 == 0) {
          local_2c->misc2 = (int)local_28;
          local_28->misc2 = (int)local_2c;
        }
      }
    }
  }
  local_2c = (SpritePtr)_NewTitleScreenSprite(1);
  if (local_2c != (SpritePtr)0x0) {
    local_30._0_2_ = (local_2c->destFrameRect).bottom;
    local_30._2_2_ = (local_2c->destFrameRect).right;
    local_34._0_2_ = (local_2c->destFrameRect).top;
    local_34._2_2_ = (local_2c->destFrameRect).left;
    _SWCenterRect(&local_34,&local_44);
    _SWOffsetRect(&local_34,0,(int)(short)(0x50 - (short)local_34));
    _SWSetSpriteLocation(local_2c,(float)(int)local_34._2_2_,(float)(int)(short)local_34);
    _MakeTitleScreenSpriteFadeIn(local_2c);
    local_30 = CONCAT22(local_30._2_2_,(local_2c->destFrameRect).bottom);
  }
  local_2c = (SpritePtr)_NewMiscSprite(0xe2);
  if (local_2c != (SpritePtr)0x0) {
    _SWSetCurrentFrameIndex(local_2c,2);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164,local_2c);
    iVar4 = (int)(short)local_30;
    iVar5 = (int)local_30._2_2_;
    _SWGetSpriteWidth(local_2c);
    _SWSetSpriteLocation
              (local_2c,((float)iVar5 - (float)extraout_ST0_06) - 13.0,(float)(iVar4 + -0x28));
    _MakeTitleScreenSpriteFadeIn(local_2c);
  }
  uVar6 = _NewTitleScreenSprite(4);
  *(undefined4 *)PTR__gBottomSpriteP_001c4b48 = uVar6;
  if (*(int *)PTR__gBottomSpriteP_001c4b48 != 0) {
    _SWRemoveSprite(*(SpritePtr *)PTR__gBottomSpriteP_001c4b48);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBottomSpriteLayerP_001c4140,
                 *(SpritePtr *)PTR__gBottomSpriteP_001c4b48);
    sVar1 = *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a);
    _SWGetSpriteHeight(*(SpritePtr *)PTR__gBottomSpriteP_001c4b48);
    _SWSetSpriteLocation
              (*(SpritePtr *)PTR__gBottomSpriteP_001c4b48,0.0,
               (float)(int)sVar1 - (float)extraout_ST0_07);
    *(undefined4 *)(*(int *)PTR__gBottomSpriteP_001c4b48 + 0xf4) = 0x3f800000;
  }
  local_2c = (SpritePtr)_NewTitleScreenSprite(5);
  if (local_2c != (SpritePtr)0x0) {
    _SWRemoveSprite(local_2c);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBackgroundSpriteLayerP_001c4070,local_2c);
    SVar9 = _SWGetSpriteVertLoc(*(SpritePtr *)PTR__gBottomSpriteP_001c4b48);
    _SWGetSpriteHeight(local_2c);
    SVar10 = _SWGetSpriteHorizLoc(*(SpritePtr *)PTR__gBottomSpriteP_001c4b48);
    _SWGetSpriteWidth(*(SpritePtr *)PTR__gBottomSpriteP_001c4b48);
    _SWGetSpriteWidth(local_2c);
    _SWSetSpriteLocation
              (local_2c,(SVar10 + (float)extraout_ST0_09) - (float)extraout_ST0_10,
               (SVar9 - (float)extraout_ST0_08) + 61.0);
    local_2c->miscFloat = 1.0;
  }
  local_2c = (SpritePtr)_NewTitleScreenSprite(6);
  if (local_2c != (SpritePtr)0x0) {
    sVar1 = *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a);
    _SWGetSpriteHeight(local_2c);
    _SWSetSpriteLocation(local_2c,0.0,(float)(int)sVar1 - (float)extraout_ST0_11);
    _SWSetSpriteMoveDelta(local_2c,-1.0,0.0);
    _SWSetSpriteMoveBounds(local_2c,(SWRect *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c));
    _SWSetSpriteMoveProc(local_2c,(MoveProcPtr_t)PTR_LAB_001c4b3c);
    local_2c->miscFloat = 1.0;
  }
  local_2c = (SpritePtr)_NewTitleScreenSprite(6);
  if (local_2c != (SpritePtr)0x0) {
    sVar1 = *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a);
    _SWGetSpriteHeight(local_2c);
    _SWGetSpriteWidth(local_2c);
    _SWSetSpriteLocation(local_2c,(float)extraout_ST0_13,(float)(int)sVar1 - (float)extraout_ST0_12)
    ;
    _SWSetSpriteMoveDelta(local_2c,-1.0,0.0);
    _SWSetSpriteMoveBounds(local_2c,(SWRect *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c));
    _SWSetSpriteMoveProc(local_2c,(MoveProcPtr_t)PTR_LAB_001c4b3c);
    local_2c->miscFloat = 1.0;
  }
  local_2c = (SpritePtr)_NewTitleScreenSprite(7);
  if (local_2c != (SpritePtr)0x0) {
    _SWRemoveSprite(local_2c);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBottomSpriteLayerP_001c4140,local_2c);
    _SWSetSpriteLocation(local_2c,179.0,164.0);
    local_2c->miscFloat = 0.55;
  }
  local_2c = (SpritePtr)_NewTitleScreenSprite(0xc);
  if (local_2c != (SpritePtr)0x0) {
    _SWRemoveSprite(local_2c);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBottomSpriteLayerP_001c4140,local_2c);
    _SWSetSpriteLocation(local_2c,179.0,164.0);
    _SWSetSpriteFrameTime(local_2c,100);
    _SWSetSpriteFrameAdvanceMode(local_2c,(void *)0x1);
    _SWSetSpriteLocation(local_2c,640.0,759.0);
    local_2c->miscFloat = 1.0;
  }
  local_2c = (SpritePtr)_NewTitleScreenSprite(0xd);
  if (local_2c != (SpritePtr)0x0) {
    _SWRemoveSprite(local_2c);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBackgroundSpriteLayerP_001c4070,local_2c);
    _SWSetSpriteLocation(local_2c,179.0,164.0);
    _SWSetSpriteFrameTime(local_2c,0x8e);
    _SWSetSpriteFrameAdvanceMode(local_2c,(void *)0x1);
    _SWSetSpriteLocation(local_2c,989.0,-715.0);
    local_2c->miscFloat = 1.0;
  }
  for (local_20 = 1; local_20 < 4; local_20 = local_20 + 1) {
    local_14 = 0;
    local_10 = local_20 * 2;
    while (local_14 < *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x82)) {
      local_2c = (SpritePtr)_NewTitleScreenSprite((int)(short)((short)(local_10 / 2) + 8));
      if (local_2c != (SpritePtr)0x0) {
        _SWRemoveSprite(local_2c);
        _SWAddSprite(*(SpriteLayerPtr *)PTR__gExitSpriteLayerP_001c403c,local_2c);
        if ((((byte)local_10 ^ 1) & 1) != 0) {
          _SWSetSpriteFlipX(local_2c,'\x01');
        }
        iVar4 = (int)local_20;
        _SWGetSpriteHeight(local_2c);
        _SWSetSpriteLocation
                  (local_2c,(float)local_14,(float)(iVar4 * 0xd + 0x17c) - (float)extraout_ST0_14);
        _SWSetSpriteMoveBounds(local_2c,(SWRect *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c));
        _SWSetSpriteMoveProc(local_2c,(MoveProcPtr_t)PTR_LAB_001c4b3c);
        _SWSetSpriteMoveDelta(local_2c,(float)-(int)local_20 / 4.0 + 0.13,0.0);
        _SWSetSpriteFrameProc(local_2c,(FrameProcPtr_t)PTR_ResetSpriteFrameProc_001c4c08);
        local_2c->miscFloat = 0.55;
        fVar8 = (float)local_14;
        _SWGetSpriteWidth(local_2c);
        iVar4 = _GetRandom(0x50,0xa0);
        local_14 = (int)((float)iVar4 + (float)extraout_ST0_15 + fVar8);
        local_10 = local_10 + 1;
        if (6 < local_10) {
          local_10 = 0;
        }
      }
    }
  }
  _MMShieldCursor();
  _MMSetCursorForGame();
  _UpdateMenuBar();
  _SWSetSpriteWorldMaxFPS(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x3c);
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0;
  *PTR__gSpriteWorldIsValid_001c48c8 = 1;
  SVar7 = _SWStickyError();
  if (SVar7 != 0) {
    SVar7 = _SWStickyError();
    _MMFatalErrorCallback
              (SVar7,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___27698_001c4b60,0x2db);
  }
  if ((*PTR__gFirstTimeShowingTitleScreen_001c4c48 == '\0') ||
     (*(int *)PTR__gOpeningMusicRefNum_001c471c != 0)) {
    _PlayCricketSound();
  }
  else {
    _StartMusicByFadingIn("Opening Music",12000,(int)*(short *)PTR__gTitleMusicVolume_001c48bc);
    uVar6 = _GetCurrentMusicReference();
    *(undefined4 *)PTR__gOpeningMusicRefNum_001c471c = uVar6;
  }
  if (*PTR__gFirstTimeShowingTitleScreen_001c4c48 != '\0') {
    *PTR__gFinishedScrolling_001c4b94 = 0;
    *(undefined4 *)PTR__gCurTitleScrollSpeed_001c4b98 = 0;
    *(undefined4 *)PTR__gFadeInButtonsDelay_001c4b24 = 0;
    *(undefined4 *)PTR__gScrollMult_001c4c10 = 0x3f800000;
    _ScrollAllSpriteLayers(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0,0x44750000);
  }
  if (*PTR__gFirstTimeShowingTitleScreen_001c4c48 != '\0') {
    _MMPlaySound(0x4b,0,1);
  }
  uVar3 = _GetRandom(1,3);
  *(undefined2 *)PTR__gNumFlashes_001c4b74 = uVar3;
  uVar6 = _GetRandom(0x1e,0x78);
  *(undefined4 *)PTR__gNextLightningFlash_001c4bf0 = uVar6;
  *PTR__gIsFirstFlash_001c4c38 = 1;
  *PTR__gFadeOutTitleButtons_001c4bd4 = 0;
  *PTR__gFirstTimeShowingTitleScreen_001c4c48 = 0;
  *PTR__gDoOpeningMusic_001c4ac4 = 0;
  _DoGameFadeToNormalFullAnim(0x1e);
  return;
}


// ---- _SetUpTitleScreenMM2 @ 0x4abe5 (14 bytes)

void SetUpTitleScreenMM2(void)

{
  return;
}


// ---- _PlayCricketSound @ 0x4abf3 (111 bytes)

void _PlayCricketSound(void)

{
  short sVar1;
  
  sVar1 = _FindSound(0x4c);
  if (sVar1 == -1) {
    _MMPlayStereoSound2(0,0x4c,6,2,0xff,0x3ff0000000000000,0xffffffff);
  }
  return;
}


// ---- _ScrollAllSpriteLayers @ 0x4ac62 (142 bytes)

void _ScrollAllSpriteLayers(SpriteWorldPtr param_1,SWMovementType param_2,float param_3)

{
  SpriteLayerPtr local_14;
  SpritePtr local_10;
  
  for (local_14 = param_1->headSpriteLayerP; local_14 != (SpriteLayerPtr)0x0;
      local_14 = _SWGetNextSpriteLayer(param_1,local_14)) {
    for (local_10 = _SWGetNextSprite(local_14,(SpritePtr)0x0); local_10 != (SpritePtr)0x0;
        local_10 = _SWGetNextSprite(local_14,local_10)) {
      _SWOffsetSprite(local_10,param_2,local_10->miscFloat * param_3);
    }
  }
  return;
}


// ---- _DoFadeToRedAndZoom @ 0x4acf0 (1052 bytes)

void _DoFadeToRedAndZoom(short param_1)

{
  int iVar1;
  int iVar2;
  char cVar3;
  short sVar4;
  float fVar5;
  SpritePtr local_1c;
  SpritePtr local_18;
  uint local_14;
  uint local_10;
  
  local_18 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4,
                              (SpritePtr)0x0);
  if (local_18 == (SpritePtr)0x0) {
    local_1c = (SpritePtr)_NewFadeSprite(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)param_1);
    if (local_1c != (SpritePtr)0x0) {
      _SWSetSpriteColor(local_1c,0xffff,0,0);
      _SWSetSpriteBlendMode(local_1c,0x302,1);
    }
    local_18 = (SpritePtr)_NewFadeSprite(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)param_1);
    if (local_18 != (SpritePtr)0x0) {
      _SWSetSpriteMoveProc(local_18,(MoveProcPtr_t)0x0);
    }
    while( true ) {
      cVar3 = _IsCurrentlyFadingToBlack();
      if ((cVar3 == '\0') || (*PTR__gQuitGame_001c49d0 != '\0')) break;
      _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
      cVar3 = _IsMouseDown();
      if (cVar3 != '\0') {
        sVar4 = param_1;
        if (param_1 < 0) {
          sVar4 = param_1 + 7;
        }
        local_1c->userData = (int)(0xffff / (longlong)(int)(sVar4 >> 3));
        sVar4 = param_1;
        if (param_1 < 0) {
          sVar4 = param_1 + 7;
        }
        local_18->userData = (int)(0xffff / (longlong)(int)(sVar4 >> 3));
      }
      if (*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) == '\0') {
        _KeepMusicPlaying();
      }
      else {
        local_1c->userData = local_1c->userData + 1;
        local_18->userData = local_18->userData + 1;
        _ProcessSDLEvents();
        _SWSetSpriteWorldZoom
                  (*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,
                   *(float *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xb4) * 1.0009,
                   *(float *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xb8) * 1.0009);
        *(float *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xac) =
             *(float *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xac) + 1.0;
        *(float *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xb0) =
             *(float *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xb0) + 0.5;
        local_1c = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4,
                                    (SpritePtr)0x0);
        if (local_1c != (SpritePtr)0x0) {
          local_14 = _SWGetSpriteTranslucencyLevel(local_1c);
          if (0x8fff < local_14) {
            _SWSetSpriteMoveProc(local_18,(MoveProcPtr_t)PTR_FadeToBlackMoveProc_001c47e0);
            fVar5 = (((float)(local_14 - 0x8fff >> 0x10) * 65536.0 +
                     (float)(local_14 - 0x8fff & 0xffff)) / 28672.0) * 65535.0;
            if (fVar5 <= 0.0) {
              fVar5 = 0.0;
            }
            local_10 = (int)(fVar5 - (float)(-(uint)(2.1474836e+09 <= fVar5) & 0x4f000000)) +
                       (uint)(2.1474836e+09 <= fVar5) * -0x80000000 |
                       -(uint)(4.2949673e+09 <= fVar5);
            _SWSetSpriteTranslucencyLevel(local_18,local_10);
          }
        }
      }
      _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
    }
    if (local_1c != (SpritePtr)0x0) {
      _SWRemoveSprite(local_1c);
      _SWDisposeSprite(&local_1c);
    }
    _SWSetSpriteWorldZoom(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,1.0,1.0);
    iVar1 = *(int *)PTR__gSpriteWorldP_001c40d0;
    iVar2 = *(int *)PTR__gSpriteWorldP_001c40d0;
    *(undefined4 *)(iVar2 + 0xb0) = 0;
    *(undefined4 *)(iVar1 + 0xac) = *(undefined4 *)(iVar2 + 0xb0);
  }
  return;
}


// ---- _FadeInTitleTextFrameProc @ 0x4b10c (229 bytes)

void FadeInTitleTextFrameProc(SpritePtr param_1)

{
  uint uVar1;
  
  if (*PTR__gFadeOutTitleButtons_001c4bd4 == '\0') {
    if (0x14 < *(int *)PTR__gFadeInButtonsDelay_001c4b24) {
      uVar1 = _SWGetSpriteTranslucencyLevel(param_1);
      if (uVar1 < 0xfe0b) {
        _SWSetSpriteTranslucencyLevel(param_1,uVar1 + 500);
      }
      else {
        _SWSetSpriteTranslucencyLevel(param_1,0xffff);
      }
    }
  }
  else {
    uVar1 = _SWGetSpriteTranslucencyLevel(param_1);
    if (uVar1 < 0x8fd) {
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
      _SWSetSpriteFrameTime(param_1,-1);
      _SWSetSpriteTranslucencyLevel(param_1,0);
    }
    else {
      _SWSetSpriteTranslucencyLevel(param_1,uVar1 - 0x8fc);
    }
  }
  return;
}


// ---- _TabGlowPulseFrameProc @ 0x4b1f1 (344 bytes)

void TabGlowPulseFrameProc(SpritePtr param_1)

{
  int iVar1;
  uint uVar2;
  uint uStack_10;
  
  iVar1 = param_1->misc2;
  if (*(SpritePtr *)(iVar1 + 0xf0) != param_1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___28165_001c4b18,0x44e);
  }
  if (*PTR__gFadeOutTitleButtons_001c4bd4 == '\0') {
    if (*(int *)(*(int *)(iVar1 + 0xe0) + 0x14) != 0) {
      uStack_10 = _SWGetSpriteTranslucencyLevel(param_1);
      if (param_1->misc1 < 0) {
        uStack_10 = uStack_10 - 800;
        if (uStack_10 < 0x1fff) {
          param_1->misc1 = 1;
        }
      }
      else {
        uStack_10 = uStack_10 + 800;
        if (0x9ffe < uStack_10) {
          param_1->misc1 = -1;
        }
      }
      _SWSetSpriteTranslucencyLevel(param_1,uStack_10);
    }
  }
  else {
    uVar2 = _SWGetSpriteTranslucencyLevel(param_1);
    if (uVar2 < 0x8fd) {
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
      _SWSetSpriteFrameTime(param_1,-1);
      _SWSetSpriteTranslucencyLevel(param_1,0);
    }
    else {
      _SWSetSpriteTranslucencyLevel(param_1,uVar2 - 0x8fc);
    }
  }
  return;
}


// ---- _DrawTitleScreenBackground @ 0x4b349 (25 bytes)

void _DrawTitleScreenBackground(undefined4 param_1)

{
  _DrawTitleScreenBackgroundMM1(param_1);
  return;
}


// ---- _DrawTitleScreenBackgroundMM1 @ 0x4b362 (855 bytes)

void _DrawTitleScreenBackgroundMM1(short *param_1)

{
  char cVar1;
  undefined4 uVar2;
  SpritePtr local_324;
  undefined1 local_320 [256];
  undefined2 local_220;
  undefined2 local_21e;
  undefined1 local_21c;
  undefined1 local_21b;
  undefined2 local_21a;
  undefined2 local_218;
  undefined2 local_216;
  undefined2 local_214;
  char local_212 [255];
  char local_113 [255];
  int local_14;
  float local_10;
  
  local_218 = 0xc000;
  local_216 = 0xc000;
  local_214 = 0xc000;
  local_10 = (float)(int)(*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38) / 0x280);
  uVar2 = _GetTitleScreenFolderPath(0);
  local_14 = _LoadAndInstallBackgroundSprite(&local_324,uVar2,"starBackground.jpg",0xffffffff,0);
  if (local_14 != 0) {
    _MMFatalErrorCallback
              (local_14,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___28207_001c4c14,0x491);
  }
  _SWSetSpriteScaledSize
            (local_324,SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38),0),
             SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a),0));
  _SetPascalString(local_320,"\nAction Man");
  local_220 = 0;
  local_21e = (undefined2)(int)(local_10 * 16.0);
  local_21c = 1;
  local_21b = 1;
  local_21a = 0;
  uVar2 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x80);
  *(undefined4 *)param_1 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
  *(undefined4 *)(param_1 + 2) = uVar2;
  *param_1 = param_1[2] + -0x1e;
  param_1[1] = param_1[3] + -0x203;
  _SWOffsetRect(param_1,0,0xfffffff0);
  cVar1 = _IsMMRegistered(2);
  if (cVar1 != '\0') {
    _SWOffsetRect(param_1,0xfffffefc,0);
  }
  if (*PTR__gCurrentPlayerNameExists_001c4830 != '\0') {
    _SWCopyPascalStringToCString(PTR__gCurrentPlayerName_001c48e8,local_113);
    _sprintf(local_212,"Welcome back, %s",local_113);
    local_324 = (SpritePtr)_MyDrawCTextJustified(local_212,local_320,param_1,&local_218);
    _MakeTitleScreenSpriteFadeIn(local_324);
    _SWOffsetRect(param_1,0,0x1e);
  }
  uVar2 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x80);
  *(undefined4 *)param_1 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
  *(undefined4 *)(param_1 + 2) = uVar2;
  param_1[1] = param_1[3] + -0x50;
  param_1[2] = *param_1 + 0x1e;
  _SetPascalString(local_320,"\nAction Man");
  local_220 = 0;
  local_21e = (undefined2)(int)(local_10 * 16.0);
  local_324 = (SpritePtr)_MyDrawCTextJustified("1.0.1",local_320,param_1,&local_218);
  _MakeTitleScreenSpriteFadeIn(local_324);
  return;
}


// ---- _MakeTitleScreenSpriteFadeIn @ 0x4b6b9 (120 bytes)

void _MakeTitleScreenSpriteFadeIn(SpritePtr param_1)

{
  _SWSetSpriteFrameTime(param_1,0x10);
  _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_FadeInTitleTextFrameProc_001c4b04);
  _SWSetSpriteFrameAdvance(param_1,0);
  if (*PTR__gFirstTimeShowingTitleScreen_001c4c48 != '\0') {
    _SWSetSpriteTranslucencyLevel(param_1,0);
  }
  return;
}


// ---- _DrawTitleScreenBackgroundMM2 @ 0x4b731 (788 bytes)

void DrawTitleScreenBackgroundMM2(short *param_1)

{
  undefined4 uVar1;
  SpritePtr pSStack_228;
  undefined1 auStack_222 [256];
  undefined2 uStack_122;
  undefined2 uStack_120;
  undefined1 uStack_11e;
  undefined1 uStack_11d;
  undefined2 uStack_11c;
  undefined2 uStack_11a;
  undefined2 uStack_118;
  undefined2 uStack_116;
  char acStack_113 [255];
  int iStack_14;
  float fStack_10;
  
  uStack_11a = 0xc000;
  uStack_118 = 0xc000;
  uStack_116 = 0xc000;
  fStack_10 = (float)(int)(*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38) / 0x280);
  uVar1 = _GetScreensFolderPath();
  iStack_14 = _LoadAndInstallBackgroundSprite(&pSStack_228,uVar1,"titlescreen.jpg",0xffffffff,0);
  if (iStack_14 != 0) {
    _MMFatalErrorCallback
              (iStack_14,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___28263_001c4bc4,0x4dc);
  }
  _SWSetSpriteScaledSize
            (pSStack_228,SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38),0)
             ,SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a),0));
  _SetPascalString(auStack_222,"\nAction Man");
  uStack_122 = 0;
  uStack_120 = (undefined2)(int)(fStack_10 * 14.0);
  uStack_11e = 1;
  uStack_11d = 1;
  uStack_11c = 2;
  uVar1 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x80);
  *(undefined4 *)param_1 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
  *(undefined4 *)(param_1 + 2) = uVar1;
  *param_1 = param_1[2] + -0x1e;
  param_1[1] = param_1[3] + -0x96;
  _SWOffsetRect(param_1,0,0xffffffd8);
  _MyDrawCTextJustified("Welcome back,",auStack_222,param_1,&uStack_11a);
  _SWOffsetRect(param_1,0,0x14);
  if (*PTR__gCurrentPlayerNameExists_001c4830 == '\0') {
    _SWCopyPascalStringToCString("\x06Nobody",acStack_113);
  }
  else {
    _SWCopyPascalStringToCString(PTR__gCurrentPlayerName_001c48e8,acStack_113);
  }
  _MyDrawCTextJustified(acStack_113,auStack_222,param_1,&uStack_11a);
  _SWOffsetRect(param_1,0xfffffe20,0);
  _SetPascalString(auStack_222,"\nAction Man");
  uStack_122 = 0;
  uStack_120 = (undefined2)(int)(fStack_10 * 14.0);
  _MyDrawCTextJustified("1.0.1",auStack_222,param_1,&uStack_11a);
  _SWOffsetRect(param_1,0x1e0,0);
  return;
}


// ---- _HandleTitleScreenCallback @ 0x4ba45 (2693 bytes)

void _HandleTitleScreenCallback(void)

{
  char cVar1;
  short sVar2;
  undefined2 uVar3;
  int iVar4;
  undefined4 uVar5;
  longdouble extraout_ST0;
  SWMovementType SVar6;
  char local_62a [500];
  char local_436 [256];
  char local_336 [256];
  undefined1 local_236 [8];
  undefined1 local_22e [8];
  undefined1 local_226 [500];
  undefined4 local_32;
  undefined1 local_2e;
  char local_2d;
  float local_2c;
  short local_28;
  short local_26;
  SpritePtr local_24;
  int local_20;
  char *local_1c;
  char *local_18;
  char *local_14;
  
  local_2d = '\0';
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _KeepMusicPlaying();
  if (*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) != '\0') {
    iVar4 = *(int *)PTR__gNextLightningFlash_001c4bf0;
    *(int *)PTR__gNextLightningFlash_001c4bf0 = iVar4 + -1;
    if (iVar4 < 0) {
      for (local_24 = *(SpritePtr *)(*(int *)PTR__gExitSpriteLayerP_001c403c + 0xc);
          local_24 != (SpritePtr)0x0;
          local_24 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gExitSpriteLayerP_001c403c,local_24))
      {
        _SWSetCurrentFrameIndex(local_24,1);
        _SWSetSpriteFrameTime(local_24,0xa6);
      }
      sVar2 = _FindSound(0x3a);
      if (((sVar2 == -1) && (*PTR__gIsFirstFlash_001c4c38 != '\0')) &&
         (iVar4 = _GetRandom(1,2), iVar4 == 1)) {
        _MMPlaySound(0x3a,0,1);
      }
      sVar2 = *(short *)PTR__gNumFlashes_001c4b74;
      *(short *)PTR__gNumFlashes_001c4b74 = sVar2 + -1;
      if (sVar2 < 1) {
        uVar5 = _GetRandom(0x18,0xf0);
        *(undefined4 *)PTR__gNextLightningFlash_001c4bf0 = uVar5;
        uVar3 = _GetRandom(2,4);
        *(undefined2 *)PTR__gNumFlashes_001c4b74 = uVar3;
        *PTR__gIsFirstFlash_001c4c38 = 1;
      }
      else {
        uVar5 = _GetRandom(7,10);
        *(undefined4 *)PTR__gNextLightningFlash_001c4bf0 = uVar5;
        *PTR__gIsFirstFlash_001c4c38 = 0;
      }
    }
    if (*PTR__gFinishedScrolling_001c4b94 == '\0') {
      _ScrollAllSpriteLayers
                (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0,
                 *(uint *)PTR__gCurTitleScrollSpeed_001c4b98 ^ 0x80000000);
      SVar6 = _SWGetSpriteVertLoc(*(SpritePtr *)PTR__gBottomSpriteP_001c4b48);
      _SWGetSpriteHeight(*(SpritePtr *)PTR__gBottomSpriteP_001c4b48);
      local_2c = (SVar6 + (float)extraout_ST0) -
                 (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x80);
      if (490.0 < local_2c) {
        *(float *)PTR__gCurTitleScrollSpeed_001c4b98 =
             *(float *)PTR__gScrollMult_001c4c10 * 0.04 +
             *(float *)PTR__gCurTitleScrollSpeed_001c4b98;
      }
      else {
        *(float *)PTR__gCurTitleScrollSpeed_001c4b98 =
             *(float *)PTR__gScrollMult_001c4c10 * -0.04 +
             *(float *)PTR__gCurTitleScrollSpeed_001c4b98;
      }
      if (*(float *)PTR__gCurTitleScrollSpeed_001c4b98 < 0.1) {
        *(undefined4 *)PTR__gCurTitleScrollSpeed_001c4b98 = 0x3dcccccd;
      }
      if (local_2c <= 0.0) {
        _ScrollAllSpriteLayers(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0,-local_2c);
        *PTR__gFinishedScrolling_001c4b94 = 1;
      }
      cVar1 = _IsMouseDown();
      if ((cVar1 != '\0') && (*(float *)PTR__gScrollMult_001c4c10 == 1.0)) {
        *(undefined4 *)PTR__gScrollMult_001c4c10 = 0x3f800000;
        *(undefined4 *)PTR__gCurTitleScrollSpeed_001c4b98 = 0x42480000;
        *(int *)PTR__gFadeInButtonsDelay_001c4b24 = *(int *)PTR__gFadeInButtonsDelay_001c4b24 + 5000
        ;
      }
    }
    else {
      *(int *)PTR__gFadeInButtonsDelay_001c4b24 = *(int *)PTR__gFadeInButtonsDelay_001c4b24 + 1;
      local_2e = _MMGetMouse(&local_32);
      _SWConvertCoordinates(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_32);
      local_28 = _ProcessButtonSprites
                           (*(undefined4 *)PTR__gTopSpriteLayerP_001c41b4,
                            *(undefined4 *)PTR__gTopSpriteLayerP_001c41b4,local_32,local_2e,0,0);
      if (local_28 != -1) {
        _AnimateOneFrame(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
        if (((local_28 == 0) || (local_28 == 1)) || (local_28 == 2)) {
          if (*PTR__gCurrentPlayerNameExists_001c4830 == '\0') {
            iVar4 = _CountNumProfilesOnDisk();
            if (iVar4 < 1) {
              local_2d = _DoEnterNameForNewProfileDialog(local_336);
              if (local_2d != '\0') {
                _CreateNewProfile(local_336);
              }
            }
            else {
              cVar1 = _DoChoosePlayerDialog();
              if (cVar1 != '\0') {
                _DrawTitleScreenBackground(local_22e);
                _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
                local_2d = '\x01';
              }
            }
          }
          else {
            local_2d = '\x01';
          }
          if (local_2d == '\0') {
            _printf("No profile!");
          }
          else {
            local_20 = 0;
            if ((*PTR__gSuspendedGameExists_001c4c34 == '\0') || (local_28 == 2)) {
              local_26 = 3;
            }
            else {
              _GetCStringFromHFSUniStr(local_336,PTR__gSuspendedGameFilenameHFSUni_001c4b84);
              _GetSuspendedGameFilePath(local_226,local_336);
              _GetPascalStringFromHFSUniStr(local_62a,PTR__gSuspendedGameFilenameHFSUni_001c4b84);
              _StripStringFromEndOfString("\n Suspended",local_62a,local_62a);
              _SWCopyPascalStringToCString(local_62a,local_336);
              _sprintf(local_436,"Resume your suspended game in %s?",local_336);
              local_26 = _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,local_436,0);
              if (local_26 == 3) {
                _DeleteSuspendedGame(local_226);
              }
            }
            if (local_26 == 2) {
              _MMHideCursor();
              _DoGameFadeToBlackFullAnim(0x1e,1);
              _ExitTitleScreen();
              local_20 = _OpenSuspendedGame(local_226);
              if (local_20 == 0) {
                _DeleteSuspendedGame(local_226);
              }
              else {
                _CleanUpAfterGame();
                _SetUpTitleScreen();
                if (local_20 == -0x3039) {
                  local_1c = 
                  "It appears the mansion in which this game was suspended has been modified. You can\'t open this suspended game until the mansion file has been restored to its original format."
                  ;
                  _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                                    "It appears the mansion in which this game was suspended has been modified. You can\'t open this suspended game until the mansion file has been restored to its original format."
                                    ,3);
                }
                else if (local_20 != -2) {
                  _MMDoErrorMessage("Sorry, but there was an error when opening the suspended game. (Error code %d)."
                                    ,local_20);
                }
              }
            }
            else if (local_26 == 3) {
              if (local_28 == 0) {
                _MMHideCursor();
                if ((*PTR__gFirstTimeStartingGame_001c4b14 == '\0') ||
                   (cVar1 = _AreAnyMansionsBeaten(), cVar1 != '\0')) {
                  _DoGameFadeToBlackFullAnim(0x1e,0);
                }
                else {
                  _MMPlaySound(0x4d,0,1);
                  *PTR__gFirstTimeStartingGame_001c4b14 = 0;
                  *PTR__gFadeOutTitleButtons_001c4bd4 = 1;
                  _MMHideAllCursors();
                  _DoFadeToRedAndZoom(500);
                }
                _ExitTitleScreen();
                _SetUpDifficultyScreen();
              }
              else if (local_28 == 2) {
                _HandleOpenMansionButton();
              }
              else if (local_28 == 1) {
                uVar5 = _GetSavedGamesFilePath(0,0);
                local_20 = _MMDoOpenGameDialog(uVar5,local_62a,local_336);
                if (local_20 == 0) {
                  _MMHideCursor();
                  _DoGameFadeToBlackFullAnim(0x1e,1);
                  _ExitTitleScreen();
                  local_20 = _MMRestoreFromSaveGameFile(local_62a,local_336);
                  if (local_20 != 0) {
                    _CleanUpAfterGame();
                    _SetUpTitleScreen();
                    if (local_20 == -0x3039) {
                      local_18 = 
                      "It appears the mansion this game was saved in has been modified. You can\'t open this saved game until the mansion file has been restored to its original format."
                      ;
                      _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                                        "It appears the mansion this game was saved in has been modified. You can\'t open this saved game until the mansion file has been restored to its original format."
                                        ,3);
                    }
                    else if (local_20 == -0x2bd7) {
                      local_14 = 
                      "This saved game was created in a mansion that you can\'t play until you\'ve registered."
                      ;
                      _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                                        "This saved game was created in a mansion that you can\'t play until you\'ve registered."
                                        ,3);
                    }
                    else if ((local_20 != -0x2b67) && (local_20 != -0x56ce)) {
                      _MMDoErrorMessage("Sorry, but there was an error when trying to open the saved game (error code %d)."
                                        ,local_20);
                    }
                  }
                }
                else {
                  _DoGameFadeToNormal(1);
                }
              }
            }
          }
        }
        else if (local_28 == 3) {
          _MMHideCursor();
          _DoGameFadeToBlackFullAnim(0x1e,0);
          _ExitTitleScreen();
          _SetUpHighScoresScreen(1,0,0);
        }
        else if (local_28 == 4) {
          _MMHideCursor();
          _DoGameFadeToBlackFullAnim(0x1e,0);
          _ExitTitleScreen();
          _SetUpHelpScreen();
        }
        else if (local_28 == 5) {
          _MMDoOptionsDialog();
        }
        else if (local_28 == 10) {
          _DoRegisterTitleScreenDialog();
        }
        else if (local_28 == 0xb) {
          _ASWLaunchURL("http://www.actionsoft.com");
        }
        else if (local_28 == 6) {
          _MMHideCursor();
          _DoGameFadeToBlackFullAnim(0xf,0);
          _ExitTitleScreen();
          *PTR__gQuitGame_001c49d0 = 1;
          cVar1 = _DoTeaserScreen(1);
          if (cVar1 != '\0') {
            *PTR__gQuitGame_001c49d0 = 0;
          }
        }
        else if (local_28 == 0xd) {
          cVar1 = _DoChoosePlayerDialog();
          if (cVar1 != '\0') {
            _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gTextSpriteLayerP_001c4804);
            _DrawTitleScreenBackground(local_236);
            _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
          }
        }
        else if (local_28 == 0xc) {
          _MMDoOpenMovieDialog();
        }
      }
    }
  }
  return;
}


// ---- _ExitTitleScreen @ 0x4c4ca (102 bytes)

void _ExitTitleScreen(void)

{
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
  *PTR__gSpriteWorldIsValid_001c48c8 = 0;
  _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  _SWSetDesignResolution(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x280,0x1e0);
  return;
}


// ---- _SetUpHelpScreen @ 0x4c530 (956 bytes)

void _SetUpHelpScreen(void)

{
  undefined4 uVar1;
  SpritePtr newSpriteP;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  undefined2 local_e;
  
  *PTR__gCurrentlyInMansion_001c4908 = 0;
  *(undefined2 *)PTR__gCurrentHelpScreenIndex_001c4b10 = 1;
  _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  _CreateAnimatedMoonBackground(0x3e99999a);
  _DrawContentsForHelpScreenIndex((int)*(short *)PTR__gCurrentHelpScreenIndex_001c4b10);
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
  local_e = 0x1a9;
  uVar1 = _NewButtonSprite(0xd6,0xd6);
  *(undefined4 *)PTR__gLeftArrowButtonSpriteP_001c4b88 = uVar1;
  if (*(int *)PTR__gLeftArrowButtonSpriteP_001c4b88 != 0) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,
                 *(SpritePtr *)PTR__gLeftArrowButtonSpriteP_001c4b88);
    _SetUpButtonSprite(*(undefined4 *)PTR__gLeftArrowButtonSpriteP_001c4b88,0,1,2,3);
    _InstallButtonProc(*(undefined4 *)PTR__gLeftArrowButtonSpriteP_001c4b88,PTR_LAB_001c4034);
    _SWSetSpriteLocation(*(SpritePtr *)PTR__gLeftArrowButtonSpriteP_001c4b88,425.0,448.0);
    _SWGetSpriteWidth(*(SpritePtr *)PTR__gLeftArrowButtonSpriteP_001c4b88);
    local_e = (short)(int)((float)extraout_ST0 + 15.0 + 425.0);
    _SetButtonDimmedMode(*(undefined4 *)PTR__gLeftArrowButtonSpriteP_001c4b88,1);
  }
  newSpriteP = (SpritePtr)_NewButtonSprite(0xd7,0xd7);
  if (newSpriteP != (SpritePtr)0x0) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,newSpriteP);
    _SetUpButtonSprite(newSpriteP,0,1,2,0);
    _InstallButtonProc(newSpriteP,PTR_LAB_001c4034);
    _SWSetSpriteLocation(newSpriteP,(float)(int)local_e,448.0);
    _SWGetSpriteWidth(newSpriteP);
    local_e = (short)(int)((float)extraout_ST0_00 + 15.0 + (float)(int)local_e);
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
              (*(SpritePtr *)PTR__gRightArrowButtonSpriteP_001c4c2c,(float)(int)local_e,448.0);
  }
  _MMShieldCursor();
  _MMSetCursorForGame();
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 3;
  *PTR__gSpriteWorldIsValid_001c48c8 = 1;
  _DoGameFadeToNormalFullAnim(0x1e);
  return;
}


// ---- _HandleHelpScreenCallback @ 0x4c8ec (512 bytes)

void _HandleHelpScreenCallback(void)

{
  undefined4 local_14;
  undefined1 local_f;
  short local_e;
  
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  if (*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) != '\0') {
    local_f = _MMGetMouse(&local_14);
    _SWConvertCoordinates(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_14);
    local_e = _ProcessButtonSprites
                        (*(undefined4 *)PTR__gTopSpriteLayerP_001c41b4,
                         *(undefined4 *)PTR__gTopSpriteLayerP_001c41b4,local_14,local_f,0,0);
    if (local_e != -1) {
      _AnimateOneFrame(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
      if (local_e == 0xd6) {
        *(short *)PTR__gCurrentHelpScreenIndex_001c4b10 =
             *(short *)PTR__gCurrentHelpScreenIndex_001c4b10 + -1;
        _SetButtonDimmedMode(*(undefined4 *)PTR__gRightArrowButtonSpriteP_001c4c2c,0);
        if (*(short *)PTR__gCurrentHelpScreenIndex_001c4b10 == 1) {
          _SetButtonDimmedMode(*(undefined4 *)PTR__gLeftArrowButtonSpriteP_001c4b88,1);
        }
        _DrawContentsForHelpScreenIndex((int)*(short *)PTR__gCurrentHelpScreenIndex_001c4b10);
      }
      else if (local_e == 0xd8) {
        *(short *)PTR__gCurrentHelpScreenIndex_001c4b10 =
             *(short *)PTR__gCurrentHelpScreenIndex_001c4b10 + 1;
        _SetButtonDimmedMode(*(undefined4 *)PTR__gLeftArrowButtonSpriteP_001c4b88,0);
        if (*(short *)PTR__gCurrentHelpScreenIndex_001c4b10 == 4) {
          _SetButtonDimmedMode(*(undefined4 *)PTR__gRightArrowButtonSpriteP_001c4c2c,1);
        }
        _DrawContentsForHelpScreenIndex((int)*(short *)PTR__gCurrentHelpScreenIndex_001c4b10);
      }
      else if (local_e == 0xd7) {
        _DoGameFadeToBlackFullAnim(0x1e,0);
        _ExitHelpScreen();
        _SetUpTitleScreen();
      }
    }
  }
  return;
}


// ---- _DrawContentsForHelpScreenIndex @ 0x4caec (489 bytes)

void _DrawContentsForHelpScreenIndex(short param_1)

{
  undefined4 uVar1;
  char local_117 [255];
  SpritePtr local_18;
  undefined1 local_11;
  int local_10;
  
  local_11 = 0;
  uVar1 = _GetScreensFolderPath();
  _sprintf(local_117,"%shelpScreen%d.png",uVar1,(int)param_1);
  local_10 = _SWCreateSpriteFromFile(&local_18,0,local_117,1,local_11);
  if (local_10 == 0) {
    _SWSetSpriteScaledSize
              (local_18,SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38),0),
               SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a),0));
    _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gForegroundSpriteLayerP_001c41f0);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gForegroundSpriteLayerP_001c41f0,local_18);
    _SWCenterSpriteInRect(local_18,*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
  }
  _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gEnemySpriteLayerP_001c40b0);
  _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gBirdSpriteLayerP_001c41c4);
  _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gBigSpiderSpriteLayerP_001c42a4);
  _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gMedusaSpriteLayerP_001c4094);
  _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gEyeballSpriteLayerP_001c41c8);
  _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gObjectSpriteLayerP_001c4268);
  _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gGhostSpriteLayerP_001c496c);
  _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gSpyVsSpySpriteLayerP_001c4874);
  _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
  if (local_10 == 0) {
    if (param_1 == 2) {
      _AddMonsterSpritesToHelpScreen();
    }
    else if (param_1 == 3) {
      _AddObjectSpritesToHelpScreen();
    }
  }
  return;
}


// ---- _AddMonsterSpritesToHelpScreen @ 0x4ccd5 (414 bytes)

void _AddMonsterSpritesToHelpScreen(void)

{
  SpritePtr srcSpriteP;
  short local_10;
  short local_e;
  
  local_e = 1;
  local_10 = 0;
  while (local_e != 0) {
    local_e = *(short *)(PTR__gMonsterArray_001c4c24 + local_10 * 6);
    if (local_e != 0) {
      srcSpriteP = (SpritePtr)
                   _CreateMySprite((int)local_e,
                                   (int)*(short *)(PTR__gMonsterArray_001c4c24 + local_10 * 6 + 2),
                                   (int)*(short *)(PTR__gMonsterArray_001c4c24 + local_10 * 6 + 4));
      if (srcSpriteP != (SpritePtr)0x0) {
        _SWSetSpriteLocation
                  (srcSpriteP,(float)(int)*(short *)(PTR__gMonsterArray_001c4c24 + local_10 * 6 + 2)
                   ,(float)(int)*(short *)(PTR__gMonsterArray_001c4c24 + local_10 * 6 + 4));
        _SetUpSpriteFrameRange(srcSpriteP);
        _SWSetSpriteMoveProc(srcSpriteP,(MoveProcPtr_t)0x0);
        if (((local_e == 4) || (local_e == 2)) || (local_e == 1)) {
          _SWSetSpriteFlipX(srcSpriteP,'\x01');
        }
        else if ((local_e == 8) || (local_e == 9)) {
          _SWSetSpriteFrameRange(srcSpriteP,0,2);
          _SWSetSpriteFlipX(srcSpriteP,'\x01');
        }
      }
    }
    local_10 = local_10 + 1;
  }
  return;
}


// ---- _AddObjectSpritesToHelpScreen @ 0x4ce73 (308 bytes)

void _AddObjectSpritesToHelpScreen(void)

{
  SpritePtr srcSpriteP;
  short local_10;
  short local_e;
  
  local_e = 1;
  local_10 = 0;
  while (local_e != 0) {
    local_e = *(short *)(PTR__gObjectSpritesArray_001c4b20 + local_10 * 6);
    if (local_e != 0) {
      srcSpriteP = (SpritePtr)
                   _CreateMySprite((int)local_e,
                                   (int)*(short *)(PTR__gObjectSpritesArray_001c4b20 +
                                                  local_10 * 6 + 2),
                                   (int)*(short *)(PTR__gObjectSpritesArray_001c4b20 +
                                                  local_10 * 6 + 4));
      if (srcSpriteP != (SpritePtr)0x0) {
        _SWSetSpriteLocation
                  (srcSpriteP,
                   (float)(int)*(short *)(PTR__gObjectSpritesArray_001c4b20 + local_10 * 6 + 2),
                   (float)(int)*(short *)(PTR__gObjectSpritesArray_001c4b20 + local_10 * 6 + 4));
        _SetUpSpriteFrameRange(srcSpriteP);
        _SWSetSpriteMoveProc(srcSpriteP,(MoveProcPtr_t)0x0);
      }
    }
    local_10 = local_10 + 1;
  }
  return;
}


// ---- _ExitHelpScreen @ 0x4cfa7 (73 bytes)

void _ExitHelpScreen(void)

{
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
  _UpdateMenuBar();
  *PTR__gSpriteWorldIsValid_001c48c8 = 0;
  _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  return;
}


// ---- _SetUpDifficultyScreen @ 0x4cff0 (678 bytes)

void _SetUpDifficultyScreen(void)

{
  undefined4 uVar1;
  undefined4 local_1c;
  undefined4 local_18;
  SpritePtr local_14;
  short local_10;
  short local_e;
  
  _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  _CreateAnimatedMoonBackground(0x3f800000);
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
  _GiveOSTime();
  _LoadDifficultyScreenSprites();
  local_14 = (SpritePtr)_NewDifficultyScreenSprite(3);
  if (local_14 != (SpritePtr)0x0) {
    _SWSetSpriteLocation(local_14,40.0,10.0);
  }
  local_e = 0xaa;
  for (local_10 = 0; local_10 < 3; local_10 = local_10 + 1) {
    local_14 = (SpritePtr)_NewDifficultyScreenButtonSprite((int)local_10);
    if (local_14 == (SpritePtr)0x0) {
      _MMFatalErrorCallback
                (0xffffffff,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                 PTR____FUNCTION___28746_001c4c20,0x86b);
    }
    else {
      _SetUpButtonSprite(local_14,0,1,2,0);
      _InstallButtonProc(local_14,PTR_DifficultyScreenButtonProc_001c4bb4);
      _AdjustCollisionRectForOffset(local_14);
      local_18._0_2_ = (local_14->destFrameRect).bottom;
      local_18._2_2_ = (local_14->destFrameRect).right;
      local_1c._0_2_ = (local_14->destFrameRect).top;
      local_1c._2_2_ = (local_14->destFrameRect).left;
      _SWCenterRect(&local_1c,*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
      _SWOffsetRect(&local_1c,0xffffff7e,(int)(short)(local_e - (short)local_1c));
      _SWSetSpriteLocation(local_14,(float)(int)local_1c._2_2_,(float)(int)(short)local_1c);
    }
    local_e = local_e + 0x46;
  }
  uVar1 = _NewDifficultyScreenSprite(4);
  *(undefined4 *)PTR__gIllustrationSpriteP_001c4be4 = uVar1;
  if (*(int *)PTR__gIllustrationSpriteP_001c4be4 != 0) {
    _SWSetSpriteLocation(*(SpritePtr *)PTR__gIllustrationSpriteP_001c4be4,296.0,89.0);
    _SWSetSpriteVisible(*(SpritePtr *)PTR__gIllustrationSpriteP_001c4be4,'\0');
  }
  _AddBackButtonToScreen(*(undefined4 *)PTR__gTopSpriteLayerP_001c41b4,10);
  _MMShieldCursor();
  _MMSetCursorForGame();
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 1;
  _UpdateMenuBar();
  *PTR__gSpriteWorldIsValid_001c48c8 = 1;
  _GiveOSTime();
  _DoGameFadeToNormalFullAnim(0x1e);
  return;
}


// ---- _HandleDifficultyScreenCallback @ 0x4d296 (435 bytes)

void _HandleDifficultyScreenCallback(void)

{
  char cVar1;
  undefined4 local_14;
  undefined1 local_f;
  short local_e;
  
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  if (*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) != '\0') {
    local_f = _MMGetMouse(&local_14);
    _SWConvertCoordinates(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_14);
    _SWSetSpriteVisible(*(SpritePtr *)PTR__gIllustrationSpriteP_001c4be4,'\0');
    local_e = _ProcessButtonSprites
                        (*(undefined4 *)PTR__gTopSpriteLayerP_001c41b4,
                         *(undefined4 *)PTR__gTopSpriteLayerP_001c41b4,local_14,local_f,0,0);
    if (*PTR__gEscapeKey_001c4734 != '\0') {
      local_e = 0x32;
    }
    if (local_e != -1) {
      _AnimateOneFrame(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
      if (local_e == 0x32) {
        _DoGameFadeToBlackFullAnim(0x1e,0);
        _ExitDifficultyScreen();
        _SetUpTitleScreen();
      }
      else {
        *PTR__gDifficultySetting_001c4870 = (char)local_e;
        _SetEasyJumpingMode(*PTR__gEasyJumping_001c4994);
        _MMHideCursor();
        _DoGameFadeToBlackFullAnim(0x1e,*PTR__gPlayTutorialMansion_001c4bdc == '\0');
        _ExitDifficultyScreen();
        cVar1 = _AreAnyMansionsBeaten();
        if ((cVar1 == '\0') && (*PTR__gHasShownOrganScreen_001c4714 == '\0')) {
          _DoOrganScreen();
        }
        else {
          _SetUpSelectMansionScreen(1);
        }
      }
    }
  }
  return;
}


// ---- _HandleOpenMansionButton @ 0x4d449 (257 bytes)

void _HandleOpenMansionButton(void)

{
  undefined4 uVar1;
  int iVar2;
  char local_111 [255];
  short local_12;
  int local_10;
  
  if (*PTR__gCanPlayBothEpisodes_001c4738 == '\0') {
    _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                      "This feature is available when you purchase both Episode 1 and Episode 2. \r\rThen you can play over 100 custom mansions created by fans of the game!"
                      ,3);
  }
  else {
    uVar1 = _GetGameDirectory();
    iVar2 = _DoesFileOrFolderExistInPath(uVar1,"Midnight Mansion HD Custom Mansions");
    if (iVar2 == 0) {
      _sprintf(local_111,
               "The folder: \r%s\rdoesn\'t exist. Do you want to create it now? \r\r(It will be placed in the same directory as Midnight Mansion.)"
               ,"Midnight Mansion HD Custom Mansions");
      local_12 = _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,local_111,2);
      if (local_12 == 2) {
        uVar1 = _GetGameDirectory();
        _CreateDirectory(uVar1,"Midnight Mansion HD Custom Mansions");
      }
    }
    local_10 = _MMDoOpenMansionDialog();
    if (local_10 == 0) {
      _StartCustomMansion();
    }
    else {
      _DoGameFadeToNormal(0);
    }
  }
  return;
}


// ---- _StartCustomMansion @ 0x4d54a (96 bytes)

void _StartCustomMansion(void)

{
  _MMHideCursor();
  _DoGameFadeToBlackFullAnim(0x1e,1);
  if (*(int *)PTR__gCurrentGameMode_001c42d0 == 1) {
    _ExitDifficultyScreen();
  }
  else if (*(int *)PTR__gCurrentGameMode_001c42d0 == 0) {
    _ExitTitleScreen();
  }
  _StartGameNoEditor();
  return;
}


// ---- _SetEasyJumpingMode @ 0x4d5aa (185 bytes)

void _SetEasyJumpingMode(char param_1)

{
  char cVar1;
  
  cVar1 = _IsMoviePlaying();
  if (cVar1 == '\0') {
    *PTR__gEasyJumping_001c4994 = param_1;
  }
  if ((*PTR__gDifficultySetting_001c4870 == '\0') && (param_1 == '\x01')) {
    *(undefined4 *)PTR__gJumpSpeed_001c4708 = 0x41033333;
    *(undefined4 *)PTR__gJumpChange_001c4790 = 0x3ebd70a4;
    *(undefined4 *)PTR__gHorizontalBoost_001c47d0 = 0x3f8ccccd;
  }
  else {
    *(undefined4 *)PTR__gJumpSpeed_001c4708 = 0x411ccccd;
    *(undefined4 *)PTR__gJumpChange_001c4790 = 0x3f0ccccd;
    *(undefined4 *)PTR__gHorizontalBoost_001c47d0 = 0x3f8a3d71;
  }
  return;
}


// ---- _ExitDifficultyScreen @ 0x4d663 (97 bytes)

void _ExitDifficultyScreen(void)

{
  _WriteProfileToDisk();
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
  _UpdateMenuBar();
  *PTR__gSpriteWorldIsValid_001c48c8 = 0;
  _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  _DisposeDifficultyScreenSprites();
  *(undefined4 *)PTR__gIllustrationSpriteP_001c4be4 = 0;
  return;
}


// ---- _CreateAnimatedMoonBackground @ 0x4d6c4 (1318 bytes)

void _CreateAnimatedMoonBackground(float param_1)

{
  undefined4 uVar1;
  int iVar2;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  float fVar3;
  SpritePtr local_24;
  short local_1e;
  float local_1c;
  int local_18;
  int local_14;
  int local_10;
  
  if (*PTR__gTitleScreenSpritesAreValid_001c4074 == '\0') {
    _puts("Can\'t show animated moon background -- title screen sprites not loaded!");
  }
  uVar1 = _GetTitleScreenFolderPath(0);
  local_18 = _LoadAndInstallBackgroundSprite(&local_24,uVar1,"starBackground.jpg",0xffffffff,0);
  if (local_18 != 0) {
    _MMFatalErrorCallback
              (local_18,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___28879_001c4c30,0x998);
  }
  _SWSetSpriteScaledSize
            (local_24,SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38),0),
             SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a),0));
  local_24 = (SpritePtr)_NewTitleScreenSprite(7);
  if (local_24 != (SpritePtr)0x0) {
    _SWRemoveSprite(local_24);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBottomSpriteLayerP_001c4140,local_24);
    _SWSetSpriteLocation(local_24,280.0,93.0);
    local_24->miscFloat = 0.55;
    _SWSetSpriteScaledFactor(local_24,0x3f800000,0x3f800000);
  }
  local_24 = (SpritePtr)_NewTitleScreenSprite(0xd);
  if (local_24 != (SpritePtr)0x0) {
    _SWRemoveSprite(local_24);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBackgroundSpriteLayerP_001c4070,local_24);
    _SWSetSpriteLocation(local_24,179.0,164.0);
    _SWSetSpriteFrameTime(local_24,0x8e);
    _SWSetSpriteFrameAdvanceMode(local_24,(void *)0x1);
    _SWSetSpriteLocation(local_24,487.0,134.0);
    local_24->miscFloat = 1.0;
    _SWSetSpriteScaledFactor(local_24,0x3f000000,0x3f000000);
  }
  local_24 = (SpritePtr)_NewTitleScreenSprite(5);
  if (local_24 != (SpritePtr)0x0) {
    _SWRemoveSprite(local_24);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBackgroundSpriteLayerP_001c4070,local_24);
    _SWSetSpriteLocation(local_24,314.0,139.0);
    local_24->miscFloat = 1.0;
    _SWSetSpriteScaledFactor(local_24,0x3f000000,0x3f000000);
  }
  for (local_1e = 1; local_1e < 6; local_1e = local_1e + 1) {
    local_14 = 0;
    local_10 = local_1e * 2;
    if (6 < local_10) {
      local_10 = 0;
    }
    while (local_14 < *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x82)) {
      local_24 = (SpritePtr)_NewTitleScreenSprite((int)(short)((short)(local_10 / 2) + 8));
      if (local_24 != (SpritePtr)0x0) {
        _SWRemoveSprite(local_24);
        _SWAddSprite(*(SpriteLayerPtr *)PTR__gExitSpriteLayerP_001c403c,local_24);
        if ((((byte)local_10 ^ 1) & 1) != 0) {
          _SWSetSpriteFlipX(local_24,'\x01');
        }
        iVar2 = (int)local_1e;
        _SWGetSpriteHeight(local_24);
        _SWSetSpriteLocation
                  (local_24,(float)local_14,(float)(iVar2 * 0x1e + 0x122) - (float)extraout_ST0);
        _SWSetSpriteMoveBounds(local_24,(SWRect *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c));
        _SWSetSpriteMoveProc(local_24,(MoveProcPtr_t)PTR_LAB_001c4b3c);
        local_1c = (float)(int)local_1e * 0.05 + 0.03;
        _SWSetSpriteMoveDelta(local_24,-local_1c * param_1,0.0);
        _SWSetSpriteFrameProc(local_24,(FrameProcPtr_t)PTR_ResetSpriteFrameProc_001c4c08);
        local_24->miscFloat = 0.55;
        _SWSetSpriteScaledFactor(local_24,0x3f800000,0x3f800000);
        fVar3 = (float)local_14;
        _SWGetSpriteWidth(local_24);
        iVar2 = _GetRandom(0xffffffec,0x50);
        local_14 = (int)((float)iVar2 + (float)extraout_ST0_00 + fVar3);
        local_10 = local_10 + 1;
        if (6 < local_10) {
          local_10 = 0;
        }
      }
    }
  }
  return;
}


// ---- _DoOrganScreen @ 0x4dbea (1370 bytes)

void _DoOrganScreen(void)

{
  int iVar1;
  short sVar2;
  undefined4 uVar3;
  undefined4 local_20;
  undefined4 local_1c;
  SpritePtr local_18;
  short local_14;
  short local_12;
  int local_10;
  
  local_18 = (SpritePtr)0x0;
  for (local_14 = 0; local_14 < 8; local_14 = local_14 + 1) {
    *(undefined4 *)(PTR__gPoemTextSpriteArray_001c4ba0 + local_14 * 4) = 0;
  }
  _LoadOrganScreenSprites();
  _StopAllChannels();
  _DoGameFadeToBlackFullAnim(0x1e,0);
  _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  uVar3 = _GetOrganScreenFolderPath(0);
  local_10 = _LoadAndInstallBackgroundSprite(&local_18,uVar3,"organscreen.jpg",0xffffffff,0);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___29009_001c4bc8,0xa0b);
  }
  _SWSetSpriteScaledSize(local_18,0,0);
  local_10 = _SWCreateSprite((SpritePtr *)PTR__gOrganLightningSpriteP_001c4bf4,(void *)0x0,0);
  if (local_10 == 0) {
    _SWSetSpriteDrawProc
              (*(SpritePtr *)PTR__gOrganLightningSpriteP_001c4bf4,
               (DrawProcPtr_t)PTR_SWSolidColorSpriteDrawProc_001c43ac);
    iVar1 = *(int *)PTR__gOrganLightningSpriteP_001c4bf4;
    uVar3 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x80);
    *(undefined4 *)(iVar1 + 0x36) = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
    *(undefined4 *)(iVar1 + 0x3a) = uVar3;
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBackgroundPictureSpriteLayerP_001c475c,
                 *(SpritePtr *)PTR__gOrganLightningSpriteP_001c4bf4);
    _SWSetSpriteColor(*(SpritePtr *)PTR__gOrganLightningSpriteP_001c4bf4,0xffff,0xffff,0xffff);
    _SWSetSpriteVisible(*(SpritePtr *)PTR__gOrganLightningSpriteP_001c4bf4,'\0');
    _SWSetSpriteTranslucencyLevel(*(SpritePtr *)PTR__gOrganLightningSpriteP_001c4bf4,0x7fff);
    _SWSetSpriteBlendMode(*(SpritePtr *)PTR__gOrganLightningSpriteP_001c4bf4,0x302,1);
  }
  uVar3 = _NewOrganScreenSprite(0);
  *(undefined4 *)PTR__gOrganPlayerSpriteP_001c4b5c = uVar3;
  if (*(int *)PTR__gOrganPlayerSpriteP_001c4b5c != 0) {
    _SWSetSpriteLocation(*(SpritePtr *)PTR__gOrganPlayerSpriteP_001c4b5c,213.5,272.0);
    _SWSetSpriteFrameTime(*(SpritePtr *)PTR__gOrganPlayerSpriteP_001c4b5c,0x115);
    _SWSetSpriteFrameAdvance(*(SpritePtr *)PTR__gOrganPlayerSpriteP_001c4b5c,0);
    _SWSetSpriteFrameProc
              (*(SpritePtr *)PTR__gOrganPlayerSpriteP_001c4b5c,
               (FrameProcPtr_t)PTR_RandomFrameProc_001c4b28);
    _SWSetSpriteFrameRange(*(SpritePtr *)PTR__gOrganPlayerSpriteP_001c4b5c,0,3);
    _SWSetSpriteMoveProc(*(SpritePtr *)PTR__gOrganPlayerSpriteP_001c4b5c,(MoveProcPtr_t)0x0);
  }
  local_18 = (SpritePtr)_NewOrganScreenSprite(1);
  if (local_18 != (SpritePtr)0x0) {
    _SWSetSpriteLocation(local_18,190.5,317.0);
    _SWSetSpriteFrameTime(local_18,0x115);
    _SWSetSpriteFrameAdvance(local_18,0);
    _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_RandomFrameProc_001c4b28);
  }
  local_12 = 0x1cc;
  for (local_14 = 0; local_14 < 8; local_14 = local_14 + 1) {
    local_18 = (SpritePtr)_NewOrganScreenSprite(2);
    sVar2 = local_12;
    if (local_18 != (SpritePtr)0x0) {
      local_1c._0_2_ = (local_18->destFrameRect).bottom;
      local_1c._2_2_ = (local_18->destFrameRect).right;
      local_20._0_2_ = (local_18->destFrameRect).top;
      local_20._2_2_ = (local_18->destFrameRect).left;
      _SWCenterRect(&local_20,*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
      _SWSetCurrentFrameIndex(local_18,(int)local_14);
      _SWSetSpriteLocation(local_18,(float)(int)local_20._2_2_,(float)(int)local_12);
      _SWSetSpriteMoveProc(local_18,(MoveProcPtr_t)PTR_LAB_001c4bac);
      _SWSetSpriteVisible(local_18,'\0');
      *(SpritePtr *)(PTR__gPoemTextSpriteArray_001c4ba0 + local_14 * 4) = local_18;
      if (((local_14 == 1) || (local_14 == 3)) || (sVar2 = local_12 + 0x32, local_14 == 5)) {
        sVar2 = local_12 + 0x73;
      }
    }
    local_12 = sVar2;
  }
  _SWSetSpriteWorldMaxFPS(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0);
  _SWSetSpriteWorldPseudoFPS(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x3c);
  _SWSetMinimumFrameRate(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,10);
  if (*PTR__gFullScreenWindow_001c48ec == '\0') {
    _MMRemoveGameCursor();
    _MMShieldCursor();
  }
  else {
    _MMHideAllCursors();
  }
  _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xb;
  *PTR__gSpriteWorldIsValid_001c48c8 = 1;
  _UpdateMenuBar();
  _GiveOSTime();
  _MMFlushEvents();
  _DoGameFadeToNormalFullAnim(0x1e);
  *PTR__gOrganScreenWasDismissed_001c4ad0 = 0;
  *(undefined2 *)PTR__lightningCount_001c4bc0 = 0;
  *PTR__gDidChangeToSkeleton_001c4b64 = 0;
  *PTR__gHasShownOrganScreen_001c4714 = 1;
  _WriteProfileToDisk();
  return;
}


// ---- _HandleOrganScreenCallback @ 0x4e144 (775 bytes)

void _HandleOrganScreenCallback(void)

{
  char cVar1;
  short sVar2;
  uint uVar3;
  SWMovementType SVar4;
  
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  if ((((*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) != '\0') &&
       (0 < *(short *)PTR__lightningCount_001c4bc0)) && (*(short *)PTR__lightningCount_001c4bc0 < 7)
      ) && (uVar3 = _GetElapsedMillisecs(), 100 < uVar3)) {
    _StartMicrosecTimer();
    if (((*(short *)PTR__lightningCount_001c4bc0 == 1) ||
        (*(short *)PTR__lightningCount_001c4bc0 == 3)) ||
       (*(short *)PTR__lightningCount_001c4bc0 == 5)) {
      _SWSetSpriteVisible(*(SpritePtr *)PTR__gOrganLightningSpriteP_001c4bf4,'\x01');
      sVar2 = _FindSound(0x3a);
      if (sVar2 == -1) {
        _MMPlayStereoSound2(0,0x3a,0,1,(int)(*(short *)PTR__gSoundVolume_001c48dc / 2),
                            0x3fe6666666666666,0);
      }
    }
    else if (((*(short *)PTR__lightningCount_001c4bc0 == 2) ||
             (*(short *)PTR__lightningCount_001c4bc0 == 4)) ||
            (*(short *)PTR__lightningCount_001c4bc0 == 6)) {
      _SWSetSpriteVisible(*(SpritePtr *)PTR__gOrganLightningSpriteP_001c4bf4,'\0');
    }
    if (*(short *)PTR__lightningCount_001c4bc0 == 3) {
      if (*(int *)PTR__gOrganPlayerSpriteP_001c4b5c != 0) {
        _SWSetSpriteFrameRange(*(SpritePtr *)PTR__gOrganPlayerSpriteP_001c4b5c,4,7);
      }
      *PTR__gDidChangeToSkeleton_001c4b64 = 1;
    }
    *(short *)PTR__lightningCount_001c4bc0 = *(short *)PTR__lightningCount_001c4bc0 + 1;
  }
  if ((((*(SpritePtr *)(PTR__gPoemTextSpriteArray_001c4ba0 + 0x1c) != (SpritePtr)0x0) &&
       (*(short *)PTR__lightningCount_001c4bc0 == 0)) &&
      (*PTR__gDidChangeToSkeleton_001c4b64 == '\0')) &&
     (SVar4 = _SWGetSpriteVertLoc(*(SpritePtr *)(PTR__gPoemTextSpriteArray_001c4ba0 + 0x1c)),
     SVar4 < 260.0)) {
    *(undefined2 *)PTR__lightningCount_001c4bc0 = 1;
    _StartMicrosecTimer();
  }
  cVar1 = _IsMouseDown();
  if (((cVar1 != '\0') || (*PTR__gOrganScreenWasDismissed_001c4ad0 != '\0')) ||
     ((uVar3 = _GetElapsedMillisecs(), 0x1d4c < uVar3 &&
      (*PTR__gDidChangeToSkeleton_001c4b64 != '\0')))) {
    _DoGameFadeToBlackFullAnim(0x37,1);
    _MMFlushEvents();
    _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    _DisposeOrganScreenSprites();
    *(undefined4 *)PTR__gOrganPlayerSpriteP_001c4b5c = 0;
    _SWSetSpriteWorldMaxFPS(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x3c);
    _MMFlushEvents();
    _SetUpSelectMansionScreen(1);
  }
  return;
}


// ---- _PoemSpriteMoveProc @ 0x4e44b (442 bytes)

void PoemSpriteMoveProc(SpritePtr param_1)

{
  int iVar1;
  SWMovementType SVar2;
  int iStack_18;
  
  _SWOffsetSpriteBasedOnTime(param_1,0.0,-0.55);
  SVar2 = _SWGetSpriteVertLoc(param_1);
  iVar1 = (int)SVar2;
  if ((iVar1 < 0x1b9) && (0x27 < iVar1)) {
    if (iVar1 < 0x169) {
      if (iVar1 < 0x78) {
        iStack_18 = (iVar1 * 100 + -12000) / 0x50 + 100;
      }
      else {
        iStack_18 = 100;
      }
    }
    else {
      iStack_18 = 100 - (iVar1 * 100 + -36000) / 0x50;
    }
  }
  else {
    iStack_18 = 0;
  }
  if (iStack_18 == 100) {
    if (param_1->isVisible == '\0') {
      _SWSetSpriteVisible(param_1,'\x01');
    }
  }
  else if (iStack_18 == 0) {
    _SWSetSpriteVisible(param_1,'\0');
  }
  else {
    if (param_1->isVisible == '\0') {
      _SWSetSpriteVisible(param_1,'\x01');
    }
    _SWSetSpriteTranslucencyLevel(param_1,iStack_18 * 0x28f);
  }
  if (iVar1 < -300) {
    _SWSetSpriteVisible(param_1,'\0');
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)0x0);
  }
  return;
}


// ---- _SetUpSelectMansionScreen @ 0x4e605 (401 bytes)

void _SetUpSelectMansionScreen(short param_1)

{
  short local_20;
  undefined4 local_18;
  undefined4 local_14;
  SpritePtr local_10;
  
  local_20 = param_1;
  if (param_1 == 3) {
    local_20 = 1;
  }
  _LoadMapScreenSprites();
  _GiveOSTime();
  _AddBackButtonToScreen(*(undefined4 *)PTR__gBottomSpriteLayerP_001c4140,0);
  _SetUpSelectMansionScreenMM1((int)local_20);
  local_10 = (SpritePtr)_NewMapScreenSprite(0xd);
  if (local_10 != (SpritePtr)0x0) {
    _SWSetSpriteLocation(local_10,40.0,10.0);
    local_14._0_2_ = (local_10->destFrameRect).bottom;
    local_14._2_2_ = (local_10->destFrameRect).right;
    local_18._0_2_ = (local_10->destFrameRect).top;
    local_18._2_2_ = (local_10->destFrameRect).left;
    _SWCenterRect(&local_18,*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
    _SWSetSpriteLocation(local_10,(float)(int)local_18._2_2_,0.0);
  }
  _MMShieldCursor();
  _MMSetCursorForGame();
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
  _StopAllChannels();
  _StartMusicByFadingIn
            ("Map Screen Music",500,
             (int)(short)(int)((double)(int)*(short *)PTR__gTitleMusicVolume_001c48bc * 0.9));
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 2;
  _UpdateMenuBar();
  *PTR__gSpriteWorldIsValid_001c48c8 = 1;
  _GiveOSTime();
  _DoGameFadeToNormalFullAnim(0x1e);
  return;
}


// ---- _SetUpSelectMansionScreenMM1 @ 0x4e796 (1984 bytes)

void _SetUpSelectMansionScreenMM1(short param_1)

{
  undefined4 uVar1;
  SpritePtr pSVar2;
  char cVar3;
  undefined4 uVar4;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  longdouble extraout_ST0_01;
  longdouble extraout_ST0_02;
  SWMovementType SVar5;
  undefined1 local_13a [256];
  undefined2 local_3a;
  undefined2 local_38;
  undefined1 local_36;
  undefined1 local_35;
  undefined2 local_34;
  undefined2 local_32;
  short sStack_30;
  short local_2e;
  short sStack_2c;
  undefined2 local_2a;
  undefined2 local_28;
  undefined2 local_26;
  SpritePtr local_24;
  short local_1e;
  int local_1c;
  char *local_18;
  int local_14;
  SpritePtr local_10;
  
  *PTR__gCurrentlyInMansion_001c4908 = 0;
  uVar4 = _GetMapScreenFolderPath(0);
  local_1c = _LoadAndInstallBackgroundSprite
                       (&local_24,uVar4,"map_screen_back_left.jpg",0xffffffff,0);
  if (local_1c == 0) {
    _SWSetSpriteScaledSize
              (local_24,SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38),0),
               SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a),0));
  }
  uVar4 = _GetMapScreenFolderPath("map_screen_back_right.jpg");
  local_1c = _SWCreateSpriteFromFile(&local_24,0,uVar4,1,0);
  if (local_1c == 0) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBackgroundScreenSpriteLayerP_001c4780,local_24);
    _SWSetSpriteLocation(local_24,640.0,0.0);
    _SWSetSpriteScaledSize
              (local_24,SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38),0),
               SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a),0));
  }
  local_2a = 0;
  local_28 = 0xffff;
  local_26 = 0xffff;
  _SetPascalString(local_13a,"\rBlackChancery");
  local_3a = 1;
  local_38 = 0x1c;
  local_36 = 1;
  local_35 = 1;
  local_34 = 2;
  uVar4 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x80);
  uVar1 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
  sStack_30 = (short)((uint)uVar1 >> 0x10);
  sStack_2c = (short)((uint)uVar4 >> 0x10);
  _local_32 = CONCAT22(sStack_30 + (short)(((int)sStack_2c - (int)sStack_30) / 2),(short)uVar1);
  local_2e = (short)uVar4;
  _local_2e = CONCAT22(sStack_2c,local_2e + -0xe);
  if (*PTR__gDifficultySetting_001c4870 == '\0') {
    local_18 = "Difficulty: Easy";
  }
  else if (*PTR__gDifficultySetting_001c4870 == '\x01') {
    local_18 = "Difficulty: Normal";
  }
  else if (*PTR__gDifficultySetting_001c4870 == '\x02') {
    local_18 = "Difficulty: Hard";
  }
  _MyDrawCTextJustified(local_18,local_13a,&local_32,&local_2a);
  local_24 = (SpritePtr)_NewMiscSprite(0xe2);
  if (local_24 != (SpritePtr)0x0) {
    _SWSetCurrentFrameIndex(local_24,0);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gForegroundSpriteLayerP_001c41f0,local_24);
    _SWSetSpriteScaledFactor(local_24,0x3f19999a,0x3f19999a);
    _SWGetSpriteWidth(local_24);
    _SWSetSpriteLocation(local_24,(float)extraout_ST0 / -2.0 + 320.0,45.0);
    *(SpritePtr *)PTR__gEpisode1TextSpriteP_001c4bb0 = local_24;
  }
  local_24 = (SpritePtr)_NewMiscSprite(0xe2);
  if (local_24 != (SpritePtr)0x0) {
    _SWSetCurrentFrameIndex(local_24,1);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gForegroundSpriteLayerP_001c41f0,local_24);
    _SWSetSpriteScaledFactor(local_24,0x3f19999a,0x3f19999a);
    _SWGetSpriteWidth(local_24);
    _SWSetSpriteLocation(local_24,(float)extraout_ST0_00 / -2.0 + 960.0,45.0);
    *(SpritePtr *)PTR__gEpisode2TextSpriteP_001c4c28 = local_24;
  }
  uVar4 = _NewMapScreenSprite(0xb);
  *(undefined4 *)PTR__gNamesSpriteP_001c4b58 = uVar4;
  if (*(int *)PTR__gNamesSpriteP_001c4b58 == 0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___29233_001c4c4c,0xb9b);
  }
  else {
    _SWSetSpriteVisible(*(SpritePtr *)PTR__gNamesSpriteP_001c4b58,'\0');
  }
  for (local_1e = 0; local_1e < 0xb; local_1e = local_1e + 1) {
    local_24 = (SpritePtr)_NewMapScreenSprite((int)local_1e);
    if (local_24 == (SpritePtr)0x0) {
      _MMFatalErrorCallback
                (0xffffffff,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                 PTR____FUNCTION___29233_001c4c4c,0xbc9);
    }
    else {
      _InstallButtonProc(local_24,PTR_LAB_001c4b0c);
      if (local_1e < 5) {
        _SWSetSpriteLocation
                  (local_24,*(SWMovementType *)(PTR__gMapSpriteLocleftSide_001c4b78 + local_1e * 8),
                   *(SWMovementType *)(PTR__gMapSpriteLocleftSide_001c4b78 + local_1e * 8 + 4));
      }
      else {
        _SWSetSpriteLocation
                  (local_24,*(float *)(PTR__gMapSpriteLocRightSide_001c4c0c + (local_1e + -5) * 8) +
                            640.0,
                   *(SWMovementType *)
                    (PTR__gMapSpriteLocRightSide_001c4c0c + (local_1e + -5) * 8 + 4));
      }
      _SWSetSpriteVisible(local_24,'\0');
      cVar3 = _IsMansionInArray((int)(char)*PTR__gDifficultySetting_001c4870,(int)local_1e);
      if (cVar3 != '\0') {
        local_14 = _NewMapScreenSprite(0xc);
        if (local_14 != 0) {
          _SWCenterSpriteOverSprite(local_14,local_24);
        }
      }
      cVar3 = _IsMansionLocked((int)local_1e);
      if (cVar3 != '\0') {
        local_10 = (SpritePtr)_NewMapScreenSprite(0xe);
        if (local_10 != (SpritePtr)0x0) {
          _SWCenterSpriteOverSprite(local_10,local_24);
          _SWGetSpriteHeight(local_24);
          _SWGetSpriteWidth(local_24);
          _SWOffsetSprite(local_10,(float)extraout_ST0_02 / 4.0,(float)extraout_ST0_01 / 4.0);
        }
      }
    }
  }
  cVar3 = _AreAnyMansionsBeatenOnAnyDifficulty();
  if (cVar3 == '\0') {
    local_24 = (SpritePtr)_NewMapScreenSprite(0xf);
    _SWSetSpriteLocation
              (local_24,*(float *)PTR__gMapSpriteLocleftSide_001c4b78 + 15.0,
               *(float *)(PTR__gMapSpriteLocleftSide_001c4b78 + 4) - 56.0);
    pSVar2 = local_24;
    SVar5 = _SWGetSpriteVertLoc(local_24);
    pSVar2->miscFloat = SVar5;
    _SWSetSpriteMoveProc(local_24,(MoveProcPtr_t)PTR_StartHereBounceMoveProc_001c4b68);
  }
  if (param_1 == 2) {
    _OffsetSpritesInLayer(*(undefined4 *)PTR__gBackgroundScreenSpriteLayerP_001c4780,0xc4200000,0);
    _OffsetSpritesInLayer(*(undefined4 *)PTR__gBottomSpriteLayerP_001c4140,0xc4200000,0);
    _OffsetSpritesInLayer(*(undefined4 *)PTR__gForegroundSpriteLayerP_001c41f0,0xc4200000,0);
    *PTR__gShowingLeftSide_001c4c00 = 0;
  }
  else {
    *PTR__gShowingLeftSide_001c4c00 = 1;
  }
  _AddArrowButtonToMM1MansionScreen(*PTR__gShowingLeftSide_001c4c00);
  return;
}


// ---- _AddArrowButtonToMM1MansionScreen @ 0x4ef56 (456 bytes)

void _AddArrowButtonToMM1MansionScreen(char param_1)

{
  SpritePtr newSpriteP;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  
  _DisposeButtonSpritesMatchingIndex(*(undefined4 *)PTR__gBottomSpriteLayerP_001c4140,0xd6);
  newSpriteP = (SpritePtr)_NewButtonSprite(0xd6,0xd6);
  if (newSpriteP != (SpritePtr)0x0) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBottomSpriteLayerP_001c4140,newSpriteP);
    _SetUpButtonSprite(newSpriteP,0,1,2,0);
    _InstallButtonProc(newSpriteP,PTR_LAB_001c4034);
    _SetButtonPushedOffset(newSpriteP,1,1);
    _AdjustCollisionRectForOffset(newSpriteP);
    _SWSetSpriteScaledFactor(newSpriteP,0x3f4ccccd,0x3f4ccccd);
    _SWSetSpriteMoveProc(newSpriteP,(MoveProcPtr_t)PTR_LAB_001c4c58);
    if (param_1 == '\0') {
      _SWGetSpriteHeight(newSpriteP);
      _SWSetSpriteLocation(newSpriteP,10.0,(float)extraout_ST0_00 / -2.0 + 240.0);
    }
    else {
      _SWGetSpriteHeight(newSpriteP);
      _SWSetSpriteLocation(newSpriteP,586.0,(float)extraout_ST0 / -2.0 + 240.0);
      _SWSetSpriteFlipX(newSpriteP,'\x01');
    }
  }
  return;
}


// ---- _SetUpSelectMansionScreenMM2 @ 0x4f11e (1217 bytes)

void SetUpSelectMansionScreenMM2(void)

{
  char cVar1;
  undefined4 uVar2;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  longdouble extraout_ST0_01;
  undefined1 auStack_13a [256];
  undefined2 uStack_3a;
  undefined2 uStack_38;
  undefined1 uStack_36;
  undefined1 uStack_35;
  undefined2 uStack_34;
  undefined4 uStack_32;
  undefined4 uStack_2e;
  undefined2 uStack_2a;
  undefined2 uStack_28;
  undefined2 uStack_26;
  SpritePtr pSStack_24;
  short sStack_1e;
  int iStack_1c;
  char *pcStack_18;
  SpritePtr pSStack_14;
  SpritePtr pSStack_10;
  
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
  _GiveOSTime();
  *PTR__gCurrentlyInMansion_001c4908 = 0;
  uVar2 = _GetScreensFolderPath();
  iStack_1c = _LoadAndInstallBackgroundSprite(&pSStack_24,uVar2,"mapScreen.jpg",0xffffffff,0);
  if (iStack_1c == 0) {
    _SWSetSpriteScaledSize
              (pSStack_24,
               SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38),0),
               SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a),0));
  }
  uStack_2a = 0xffff;
  uStack_28 = 0xd7ce;
  uStack_26 = 0;
  _SetPascalString(auStack_13a,"\rBlackChancery");
  uStack_3a = 1;
  uStack_38 = 0x1c;
  uStack_36 = 1;
  uStack_35 = 1;
  uStack_34 = 2;
  uStack_2e = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x80);
  uStack_32 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
  _SWOffsetRect(&uStack_32,0x96,0xfffffff4);
  uStack_2e = CONCAT22(uStack_2e._2_2_,(short)uStack_2e + -2);
  if (*PTR__gDifficultySetting_001c4870 == '\0') {
    pcStack_18 = "Difficulty: Easy";
  }
  else if (*PTR__gDifficultySetting_001c4870 == '\x01') {
    pcStack_18 = "Difficulty: Normal";
  }
  else if (*PTR__gDifficultySetting_001c4870 == '\x02') {
    pcStack_18 = "Difficulty: Hard";
  }
  _MyDrawCTextJustified(pcStack_18,auStack_13a,&uStack_32,&uStack_2a);
  uVar2 = _NewMapScreenSprite(0xb);
  *(undefined4 *)PTR__gNamesSpriteP_001c4b58 = uVar2;
  if (*(int *)PTR__gNamesSpriteP_001c4b58 == 0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___29420_001c4ba8,0xc49);
  }
  else {
    _SWSetSpriteVisible(*(SpritePtr *)PTR__gNamesSpriteP_001c4b58,'\0');
  }
  for (sStack_1e = 0; sStack_1e < 0xb; sStack_1e = sStack_1e + 1) {
    pSStack_24 = (SpritePtr)_NewMapScreenSprite((int)sStack_1e);
    if (pSStack_24 == (SpritePtr)0x0) {
      _MMFatalErrorCallback
                (0xffffffff,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                 PTR____FUNCTION___29420_001c4ba8,0xc7d);
    }
    else {
      _InstallButtonProc(pSStack_24,PTR_LAB_001c4b0c);
      _SWSetSpriteLocation
                (pSStack_24,(float)(int)*(short *)(PTR__gMapSpriteLocMM2_001c4b90 + sStack_1e * 4),
                 (float)(int)*(short *)(PTR__gMapSpriteLocMM2_001c4b90 + sStack_1e * 4 + 2));
      _SWSetSpriteVisible(pSStack_24,'\0');
      cVar1 = _IsMansionInArray((int)(char)*PTR__gDifficultySetting_001c4870,(int)sStack_1e);
      if (cVar1 != '\0') {
        pSStack_14 = (SpritePtr)_NewMapScreenSprite(0xc);
        if (pSStack_14 != (SpritePtr)0x0) {
          _SWCenterSpriteOverSprite(pSStack_14,pSStack_24);
          if (sStack_1e == 4) {
            _SWOffsetSprite(pSStack_14,-3.0,-6.0);
          }
          else if (sStack_1e == 5) {
            _SWOffsetSprite(pSStack_14,0.0,-5.0);
          }
          else {
            _SWOffsetSprite(pSStack_14,-1.0,-3.0);
          }
          _SWRemoveSprite(pSStack_14);
          _SWAddSprite(*(SpriteLayerPtr *)PTR__gObjectSpriteLayerP_001c4268,pSStack_14);
        }
      }
      cVar1 = _IsMansionLocked((int)sStack_1e);
      if (cVar1 != '\0') {
        pSStack_10 = (SpritePtr)_NewMapScreenSprite(0xe);
        if (pSStack_10 != (SpritePtr)0x0) {
          _SWCenterSpriteOverSprite(pSStack_10,pSStack_24);
          _SWGetSpriteHeight(pSStack_24);
          _SWGetSpriteWidth(pSStack_24);
          _SWOffsetSprite(pSStack_10,(float)extraout_ST0_00 / 4.0,(float)extraout_ST0 / 4.0);
        }
      }
      _SWGetSpriteHeight(pSStack_24);
      _SetButtonClickRadius(pSStack_24,(int)(short)(int)((float)extraout_ST0_01 * 0.43));
    }
  }
  return;
}


// ---- _HandleSelectMansionScreenCallback @ 0x4f5df (1083 bytes)

void _HandleSelectMansionScreenCallback(void)

{
  char cVar1;
  undefined2 local_120;
  undefined1 local_11e [254];
  undefined4 local_20;
  undefined1 local_1b;
  short local_1a;
  char *local_18;
  char *local_14;
  int local_10;
  
  if (*(int *)PTR__gNamesSpriteP_001c4b58 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___29515_001c4bfc,0xc8a);
  }
  if (*PTR__gGameIsPaused_001c484c == '\0') {
    _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
    _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
    if (*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) != '\0') {
      local_1b = _MMGetMouse(&local_20);
      _SWConvertCoordinates(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_20);
      local_1a = _ProcessButtonSprites
                           (*(undefined4 *)PTR__gBottomSpriteLayerP_001c4140,
                            *(undefined4 *)PTR__gBottomSpriteLayerP_001c4140,local_20,local_1b,0,0);
      if (*PTR__gEscapeKey_001c4734 != '\0') {
        local_1a = 0x32;
      }
      if (local_1a == 0x32) {
        _AnimateOneFrame(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
        _MMHideCursor();
        _DoGameFadeToBlackFullAnim(0x1e,1);
        _ExitSelectMansionScreen();
        _SetUpTitleScreen();
      }
      else if (local_1a == 0xd6) {
        _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gBottomSpriteLayerP_001c4140,'\0');
        _SWSetSpriteVisible(*(SpritePtr *)PTR__gEpisode1TextSpriteP_001c4bb0,'\0');
        _SWSetSpriteVisible(*(SpritePtr *)PTR__gEpisode2TextSpriteP_001c4c28,'\0');
        _ScrollSelectAMansionScreen(*PTR__gShowingLeftSide_001c4c00);
        *PTR__gShowingLeftSide_001c4c00 = *PTR__gShowingLeftSide_001c4c00 == '\0';
        _AddArrowButtonToMM1MansionScreen(*PTR__gShowingLeftSide_001c4c00);
        _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gBottomSpriteLayerP_001c4140,'\x01');
        if (*PTR__gShowingLeftSide_001c4c00 == '\0') {
          _SWSetSpriteVisible(*(SpritePtr *)PTR__gEpisode2TextSpriteP_001c4c28,'\x01');
        }
        else {
          _SWSetSpriteVisible(*(SpritePtr *)PTR__gEpisode1TextSpriteP_001c4bb0,'\x01');
        }
      }
      else if (local_1a != -1) {
        cVar1 = _IsMansionLocked((int)local_1a);
        if (cVar1 == '\0') {
          local_120 = 0;
          _memset(local_11e,0,0xfe);
          local_10 = 0;
          local_18 = (char *)_GetMansionNameFromIndex((int)local_1a);
          if (*PTR__gDifficultySetting_001c4870 == '\0') {
            local_14 = "\x05 Easy";
          }
          else if (*PTR__gDifficultySetting_001c4870 == '\x01') {
            local_14 = "\a Normal";
          }
          else if (*PTR__gDifficultySetting_001c4870 == '\x02') {
            local_14 = "\x05 Hard";
          }
          if (local_1a == 0) {
            local_14 = "\a Normal";
          }
          _SWConcatPascalStrings((char *)&local_120,local_18);
          _SWConcatPascalStrings((char *)&local_120,local_14);
          _MMHideCursor();
          _DoGameFadeToBlackFullAnim(0x1e,1);
          local_10 = _MyOpenStandardMansion(&local_120,(int)local_1a);
          if (local_10 == 0) {
            _ExitSelectMansionScreen();
            _StartGameNoEditor();
          }
          else {
            _DoGameFadeToNormal(0);
          }
        }
        else {
          if ((local_1a == 10) && (cVar1 = _CanPlayFinalMansion(), cVar1 == '\0')) {
            _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                              "You can\'t play Spider Palace until you have completed all of the other mansions."
                              ,3);
            return;
          }
          if (local_1a < 5) {
            _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                              "You can\'t play this mansion until you buy Episode 1. Try playing Falcon Manor or Nightmare Mansion instead."
                              ,3);
          }
          else {
            _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                              "You can\'t play this mansion until you buy Episode 2.",3);
          }
        }
      }
    }
  }
  return;
}


// ---- _ScrollSelectAMansionScreen @ 0x4fa1a (287 bytes)

void _ScrollSelectAMansionScreen(char param_1)

{
  bool bVar1;
  undefined4 local_18;
  undefined4 local_14;
  
  local_18 = 0;
  bVar1 = false;
  while (!bVar1) {
    _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
    _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
    if (*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) != '\0') {
      _ProcessSDLEvents();
      if (param_1 == '\0') {
        local_14 = 0x14;
      }
      else {
        local_14 = -0x14;
      }
      _OffsetSpritesInLayer
                (*(undefined4 *)PTR__gBackgroundScreenSpriteLayerP_001c4780,(float)local_14,0);
      _OffsetSpritesInLayer(*(undefined4 *)PTR__gBottomSpriteLayerP_001c4140,(float)local_14,0);
      _OffsetSpritesInLayer(*(undefined4 *)PTR__gForegroundSpriteLayerP_001c41f0,(float)local_14,0);
      local_18 = local_18 + 0x14;
      if (0x27f < local_18) {
        bVar1 = true;
      }
    }
  }
  return;
}


// ---- _SelectMansionScreenButtonProc @ 0x4fb39 (1114 bytes)

void SelectMansionScreenButtonProc(SpritePtr param_1,short param_2,short param_3,short param_4)

{
  char cVar1;
  longdouble extraout_ST0;
  
  if (param_3 != param_4) {
    if ((param_3 == 1) || (param_3 == 2)) {
      _SWSetSpriteVisible(param_1,'\x01');
      *(short *)PTR__gOldButtonIndex_001c4c44 = param_2;
      _SWSetSpriteVisible(*(SpritePtr *)PTR__gNamesSpriteP_001c4b58,'\x01');
      _SWCenterSpriteOverSprite(*(undefined4 *)PTR__gNamesSpriteP_001c4b58,param_1);
      _SWGetSpriteHeight(param_1);
      _SWOffsetSprite(*(SpritePtr *)PTR__gNamesSpriteP_001c4b58,0.0,(float)extraout_ST0 / 2.0);
      switch(param_2) {
      case 2:
        _SWOffsetSprite(*(SpritePtr *)PTR__gNamesSpriteP_001c4b58,6.0,-4.0);
        break;
      case 4:
        _SWOffsetSprite(*(SpritePtr *)PTR__gNamesSpriteP_001c4b58,0.0,-5.0);
        break;
      case 5:
        _SWOffsetSprite(*(SpritePtr *)PTR__gNamesSpriteP_001c4b58,3.0,3.0);
        break;
      case 6:
        _SWOffsetSprite(*(SpritePtr *)PTR__gNamesSpriteP_001c4b58,0.0,4.0);
        break;
      case 7:
        _SWOffsetSprite(*(SpritePtr *)PTR__gNamesSpriteP_001c4b58,-5.0,-5.0);
      }
      _SWSetCurrentFrameIndex(*(SpritePtr *)PTR__gNamesSpriteP_001c4b58,(int)param_2);
      if ((param_3 == 2) && (param_4 != 2)) {
        _SWSetSpriteColor(*(SpritePtr *)PTR__gNamesSpriteP_001c4b58,0,0xffff,0);
      }
      else {
        _SWSetSpriteColor(*(SpritePtr *)PTR__gNamesSpriteP_001c4b58,0xffff,0xffff,0xffff);
      }
      cVar1 = _IsMansionLocked((int)param_2);
      if (cVar1 == '\0') {
        _SWSetSpriteTranslucencyLevel(*(SpritePtr *)PTR__gNamesSpriteP_001c4b58,0xffff);
      }
      else {
        _SWSetSpriteTranslucencyLevel(*(SpritePtr *)PTR__gNamesSpriteP_001c4b58,36000);
      }
      _MakeButtonBounceWhenSelected
                (*(undefined4 *)PTR__gNamesSpriteP_001c4b58,(int)param_3,(int)param_4,0x3dcccccd,
                 0x3e19999a,0x40000000,0x40000000);
    }
    else {
      _SWSetSpriteVisible(param_1,'\0');
      if (*(short *)PTR__gOldButtonIndex_001c4c44 == param_2) {
        _SWSetSpriteVisible(*(SpritePtr *)PTR__gNamesSpriteP_001c4b58,'\0');
      }
    }
    if (param_3 == 2) {
      _MMPlayStereoSound2(0,0x39,8,1,(int)(*(short *)PTR__gSoundVolume_001c48dc / 3),
                          0x3ff0000000000000,0);
    }
    else if (param_4 == 2) {
      _MMPlayStereoSound2(0,0x39,8,1,(int)(*(short *)PTR__gSoundVolume_001c48dc / 3),
                          0x3fe999999999999a,0);
    }
  }
  return;
}


// ---- _GetMansionNameFromIndex @ 0x4ff93 (240 bytes)

char * _GetMansionNameFromIndex(undefined2 param_1)

{
  char *local_10;
  
  switch(param_1) {
  case 0:
    local_10 = "\nHint House";
    break;
  case 1:
    local_10 = "\fFalcon Manor";
    break;
  case 2:
    local_10 = "\x11Nightmare Mansion";
    break;
  case 3:
    local_10 = "\x0eKnight Mansion";
    break;
  case 4:
    local_10 = "\x0fTomb of Princes";
    break;
  case 5:
    local_10 = "\x16House of Baron Garneau";
    break;
  case 6:
    local_10 = "\vCastle Doom";
    break;
  case 7:
    local_10 = "\x10Cathedral Towers";
    break;
  case 8:
    local_10 = "\x10The Great Escape";
    break;
  case 9:
    local_10 = "\x11Chateau de Medusa";
    break;
  case 10:
    local_10 = "\rSpider Palace";
    break;
  default:
    local_10 = "\x05Error";
  }
  return local_10;
}


// ---- _IsMansionLocked @ 0x50083 (134 bytes)

bool _IsMansionLocked(short param_1)

{
  char cVar1;
  
  if ((param_1 == 0) || (cVar1 = _IsMansionDemoMansion((int)param_1), cVar1 != '\0')) {
    return false;
  }
  if (param_1 < 5) {
    return *PTR__gEpisode1IsUnlocked_001c4744 == '\0';
  }
  if (param_1 == 10) {
    cVar1 = _CanPlayFinalMansion();
    return cVar1 == '\0';
  }
  return *PTR__gEpisode2IsUnlocked_001c48d0 == '\0';
}


// ---- _IsMansionDemoMansion @ 0x50109 (58 bytes)

undefined1 _IsMansionDemoMansion(short param_1)

{
  undefined1 local_14;
  
  if ((param_1 == 1) || (param_1 == 2)) {
    local_14 = 1;
  }
  else {
    local_14 = 0;
  }
  return local_14;
}


// ---- _CheckForEndOfDemoLimit @ 0x50143 (246 bytes)

undefined4 _CheckForEndOfDemoLimit(void)

{
  short sVar1;
  short local_12;
  short local_10 [4];
  
  _GEGetRowAndColFromLevelNumber((int)*(short *)PTR__gCurrentLevel_001c49bc,local_10,&local_12,200);
  sVar1 = _GEGetLevelResID((int)*(short *)PTR__gCurrentLevel_001c49bc,1,200);
  if (*(short *)PTR__gCurrentMansionNumber_001c4824 == 1) {
    if (0x5c < local_10[0]) {
      return 1;
    }
    if (((0x68 < local_12) || (local_12 < 0x56)) || (local_10[0] < 0x53)) {
      return 1;
    }
  }
  else if ((((*(short *)PTR__gCurrentMansionNumber_001c4824 == 2) && (0x53 < local_10[0])) &&
           (sVar1 != 0x42c9)) && (sVar1 != 0x42ca)) {
    return 1;
  }
  return 0;
}


// ---- _CanPlayFinalMansion @ 0x50239 (102 bytes)

undefined1 _CanPlayFinalMansion(void)

{
  char cVar1;
  short local_12;
  
  local_12 = 1;
  while( true ) {
    if (9 < local_12) {
      return 1;
    }
    cVar1 = _IsMansionInArray((int)(char)*PTR__gDifficultySetting_001c4870,(int)local_12);
    if (cVar1 == '\0') break;
    local_12 = local_12 + 1;
  }
  return 0;
}


// ---- _AreAnyMansionsBeaten @ 0x5029f (99 bytes)

undefined4 _AreAnyMansionsBeaten(void)

{
  char cVar1;
  short local_e;
  
  local_e = 0;
  while( true ) {
    if (10 < local_e) {
      return 0;
    }
    cVar1 = _IsMansionInArray((int)(char)*PTR__gDifficultySetting_001c4870,(int)local_e);
    if (cVar1 == '\x01') break;
    local_e = local_e + 1;
  }
  return 1;
}


// ---- _AreAnyMansionsBeatenOnAnyDifficulty @ 0x50302 (99 bytes)

undefined4 _AreAnyMansionsBeatenOnAnyDifficulty(void)

{
  char cVar1;
  short local_10;
  short local_e;
  
  local_e = 0;
  do {
    if (2 < local_e) {
      return 0;
    }
    for (local_10 = 0; local_10 < 0xb; local_10 = local_10 + 1) {
      cVar1 = _IsMansionInArray((int)local_e,(int)local_10);
      if (cVar1 == '\x01') {
        return 1;
      }
    }
    local_e = local_e + 1;
  } while( true );
}


// ---- _AreAllEpisode1MansionsBeaten @ 0x50365 (99 bytes)

undefined4 AreAllEpisode1MansionsBeaten(void)

{
  char cVar1;
  short sStack_e;
  
  sStack_e = 0;
  while( true ) {
    if (4 < sStack_e) {
      return 1;
    }
    cVar1 = _IsMansionInArray((int)(char)*PTR__gDifficultySetting_001c4870,(int)sStack_e);
    if (cVar1 == '\0') break;
    sStack_e = sStack_e + 1;
  }
  return 0;
}


// ---- _ExitSelectMansionScreen @ 0x503c8 (97 bytes)

void _ExitSelectMansionScreen(void)

{
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
  _UpdateMenuBar();
  *PTR__gSpriteWorldIsValid_001c48c8 = 0;
  _StopAndDisposeMusic();
  _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  _DisposeMapScreenSprites();
  *(undefined4 *)PTR__gNamesSpriteP_001c4b58 = 0;
  return;
}


// ---- _DoAboutScreen @ 0x50429 (215 bytes)

void DoAboutScreen(void)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = _GetScreensFolderPath();
  iVar2 = _LoadAndInstallBackgroundSprite(0,uVar1,"about.jpg",0xffffffff,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___29871_001c4b4c,0xed2);
  }
  _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 8;
  *PTR__gSpriteWorldIsValid_001c48c8 = 1;
  _UpdateMenuBar();
  if (*PTR__gFullScreenWindow_001c48ec == '\0') {
    _MMSetCursorForSystem();
  }
  _MMFlushEvents();
  _DoGameFadeToNormalFullAnim(0x1e);
  return;
}


// ---- _HandleAboutScreenCallback @ 0x50500 (96 bytes)

void _HandleAboutScreenCallback(void)

{
  char cVar1;
  
  _GiveOSTime();
  cVar1 = _IsMouseDown();
  if ((cVar1 == '\0') && (cVar1 = _WasAKeyPressed(), cVar1 == '\0')) {
    return;
  }
  _MMFlushEvents();
  _DoGameFadeToBlackFullAnim(0x1e,0);
  _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  _SetUpTitleScreen();
  return;
}


// ---- _DoTeaserScreen @ 0x50560 (1136 bytes)

undefined4 _DoTeaserScreen(undefined1 param_1)

{
  char cVar1;
  undefined4 uVar2;
  undefined4 local_24;
  SpritePtr local_18;
  char *local_14;
  int local_10;
  
  *PTR__gQuitWhenDoneTeaser_001c4c60 = param_1;
  cVar1 = _IsMMRegistered(1);
  if (cVar1 == '\0') {
    local_14 = "teaserScreenEpisode1.jpg";
    _MMHideCursor();
    _DoGameFadeToBlackFullAnim(0xf,0);
    _MMFlushEvents();
    _CleanUpAfterGame();
    uVar2 = _GetSpriteImagePath("teaserLearnMoreButton.png");
    local_10 = _LoadStandardButtonSpriteFromFile(PTR__payNowButtonSpriteP_001c4b30,uVar2,0);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                 PTR____FUNCTION___29900_001c4c3c,0xf62);
    }
    uVar2 = _GetSpriteImagePath("teaserNotNowButton.png");
    local_10 = _LoadStandardButtonSpriteFromFile(PTR__notNowButtonSpriteP_001c4b2c,uVar2,0);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                 PTR____FUNCTION___29900_001c4c3c,0xf64);
    }
    uVar2 = _CloneButtonSprite(*(undefined4 *)PTR__payNowButtonSpriteP_001c4b30,1);
    *(undefined4 *)PTR__payNowButtonCloneSpriteP_001c4b40 = uVar2;
    uVar2 = _CloneButtonSprite(*(undefined4 *)PTR__notNowButtonSpriteP_001c4b2c,2);
    *(undefined4 *)PTR__notNowCloneSpriteP_001c4bb8 = uVar2;
    if (*(int *)PTR__payNowButtonCloneSpriteP_001c4b40 != 0) {
      _SWSetSpriteScaledFactor
                (*(undefined4 *)PTR__payNowButtonCloneSpriteP_001c4b40,0x3eb33333,0x3eb33333);
      _SetUpButtonSprite(*(undefined4 *)PTR__payNowButtonCloneSpriteP_001c4b40,0,1,2,0);
      _InstallButtonProc(*(undefined4 *)PTR__payNowButtonCloneSpriteP_001c4b40,PTR_LAB_001c4034);
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,
                   *(SpritePtr *)PTR__payNowButtonCloneSpriteP_001c4b40);
      _SWSetSpriteLocation(*(SpritePtr *)PTR__payNowButtonCloneSpriteP_001c4b40,500.0,433.0);
    }
    if (*(int *)PTR__notNowCloneSpriteP_001c4bb8 != 0) {
      _SWSetSpriteScaledFactor
                (*(undefined4 *)PTR__notNowCloneSpriteP_001c4bb8,0x3eb33333,0x3eb33333);
      _SetUpButtonSprite(*(undefined4 *)PTR__notNowCloneSpriteP_001c4bb8,0,1,2,0);
      _InstallButtonProc(*(undefined4 *)PTR__notNowCloneSpriteP_001c4bb8,PTR_LAB_001c4034);
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,
                   *(SpritePtr *)PTR__notNowCloneSpriteP_001c4bb8);
      _SWSetSpriteLocation(*(SpritePtr *)PTR__notNowCloneSpriteP_001c4bb8,407.0,433.0);
    }
    if (*PTR__gMusicIsOn_001c47c0 != '\0') {
      _FadeMusicNow(1);
      while( true ) {
        cVar1 = _IsMusicPlaying();
        if (cVar1 == '\0') break;
        _SDL_Delay(100);
      }
      _StopAndDisposeMusic();
    }
    uVar2 = _GetScreensFolderPath();
    local_10 = _LoadAndInstallBackgroundSprite(&local_18,uVar2,local_14,0xffffffff,0);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                 PTR____FUNCTION___29900_001c4c3c,0xf91);
    }
    if (local_10 == 0) {
      _SWSetSpriteScaledSize
                (local_18,SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38),0
                               ),
                 SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a),0));
    }
    _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
    *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xd;
    *PTR__gSpriteWorldIsValid_001c48c8 = 1;
    _UpdateMenuBar();
    _GiveOSTime();
    _DoGameFadeToNormalFullAnim(0xf);
    _MMFlushEvents();
    _MMShieldCursor();
    _MMSetCursorForGame();
    *PTR__gCanQuit_001c4968 = 0;
    _StartMicrosecTimer();
    local_24 = 1;
  }
  else {
    local_24 = 0;
  }
  return local_24;
}


// ---- _HandleTeaserScreenCallback @ 0x509d0 (364 bytes)

void _HandleTeaserScreenCallback(void)

{
  int iVar1;
  undefined4 local_14;
  undefined1 local_f;
  short local_e;
  
  *PTR__gCanQuit_001c4968 = 1;
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  if (*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) != '\0') {
    local_f = _MMGetMouse(&local_14);
    _SWConvertCoordinates(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_14);
    local_e = _ProcessButtonSprites
                        (*(undefined4 *)PTR__gTopSpriteLayerP_001c41b4,
                         *(undefined4 *)PTR__gTopSpriteLayerP_001c41b4,local_14,local_f,0,0);
    if (local_e == 1) {
      _AnimateOneFrame(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
      _LaunchRegistrationWebsite();
    }
    else {
      if ((local_e != 2) && (iVar1 = _GetElapsedSeconds(), iVar1 < 0x7d1)) {
        return;
      }
      _MMFlushEvents();
      _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
      if (*PTR__gQuitWhenDoneTeaser_001c4c60 == '\0') {
        _SWDisposeSprite((SpritePtr *)PTR__payNowButtonSpriteP_001c4b30);
        _SWDisposeSprite((SpritePtr *)PTR__notNowButtonSpriteP_001c4b2c);
        _DoGameFadeToBlack(0x1e,1);
        _SetUpTitleScreen();
      }
      else {
        *PTR__gQuitGame_001c49d0 = 1;
      }
    }
  }
  return;
}


// ---- _DoGameOverScreen @ 0x50b3c (947 bytes)

void _DoGameOverScreen(void)

{
  undefined4 uVar1;
  SpritePtr local_18;
  undefined1 local_11;
  int local_10;
  
  if (*PTR__gEditorIsRunning_001c495c == '\0') {
    _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    _WriteRoomInfoToFile();
    _WriteCStringToFile("GAME OVER\r");
    _DoGameFadeToBlackFullAnim(0x37,1);
    uVar1 = _GetScreensFolderPath();
    local_10 = _LoadAndInstallBackgroundSprite(&local_18,uVar1,"gameover.jpg",0xffffffff,0);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                 PTR____FUNCTION___29997_001c4b54,0x1004);
    }
    _SWSetSpriteScaledSize(local_18,0,0);
    local_10 = _CreateSpriteFromPictResource
                         (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,PTR__gameOverSpriteP_001c4c54,0
                          ,&DAT_00001fa4,&DAT_00001fa4,1,4);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                 PTR____FUNCTION___29997_001c4b54,0x100f);
    }
    _SWSetSpriteScaledFactor(*(undefined4 *)PTR__gameOverSpriteP_001c4c54,0x3f0ccccd,0x3f0ccccd);
    *(undefined4 *)(*(int *)PTR__gameOverSpriteP_001c4c54 + 0xe8) = 0xed;
    _SWSetSpriteTranslucencyLevel(*(SpritePtr *)PTR__gameOverSpriteP_001c4c54,0);
    _SWSetSpriteFrameTime(*(SpritePtr *)PTR__gameOverSpriteP_001c4c54,0x21);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,
                 *(SpritePtr *)PTR__gameOverSpriteP_001c4c54);
    _SWCenterRect(*(int *)PTR__gameOverSpriteP_001c4c54 + 0x36,
                  *(int *)PTR__gSpriteWorldP_001c40d0 + 0x8c);
    _SWOffsetRect(*(int *)PTR__gameOverSpriteP_001c4c54 + 0x36,0,0xfffffff1);
    _SWMoveSprite(*(SpritePtr *)PTR__gameOverSpriteP_001c4c54,
                  (float)(int)*(short *)(*(int *)PTR__gameOverSpriteP_001c4c54 + 0x38),
                  (float)(int)*(short *)(*(int *)PTR__gameOverSpriteP_001c4c54 + 0x36));
    _SWSetSpriteLocation
              (*(SpritePtr *)PTR__gameOverSpriteP_001c4c54,
               (float)(int)*(short *)(*(int *)PTR__gameOverSpriteP_001c4c54 + 0x38),100.0);
    _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
    _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
    if (*PTR__gSoundIsTurnedOn_001c47fc != '\0') {
      local_11 = *PTR__gMusicIsOn_001c47c0;
      *PTR__gMusicIsOn_001c47c0 = 1;
      _MyPlayMusic("Game Over Music",(int)*(short *)PTR__gTitleMusicVolume_001c48bc,0,0);
      *PTR__gMusicIsOn_001c47c0 = local_11;
    }
    *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 7;
    *PTR__gSpriteWorldIsValid_001c48c8 = 1;
    _UpdateMenuBar();
    _GiveOSTime();
    _DoGameFadeToNormal(0x3c);
    _MMFlushEvents();
    _StartMicrosecTimer();
    *PTR__spriteHasStartedFadingIn_001c4c1c = 0;
    uVar1 = _GetElapsedMillisecs();
    _printf("Elapsed milliseconds at start of Game Over: %d",uVar1);
    *PTR__gPlayingGame_001c4924 = 0;
  }
  return;
}


// ---- _HandleGameOverScreenCallback @ 0x50eef (295 bytes)

void _HandleGameOverScreenCallback(void)

{
  char cVar1;
  uint uVar2;
  undefined *puVar3;
  undefined4 uVar4;
  
  if (*(int *)PTR__gameOverSpriteP_001c4c54 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
               PTR____FUNCTION___30045_001c4c40,0x1052);
  }
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  if ((*PTR__spriteHasStartedFadingIn_001c4c1c == '\0') &&
     (uVar2 = _GetElapsedMillisecs(), 1000 < uVar2)) {
    _SWSetSpriteFrameProc
              (*(SpritePtr *)PTR__gameOverSpriteP_001c4c54,
               (FrameProcPtr_t)PTR_FadeInFrameProc_001c4c04);
    *PTR__spriteHasStartedFadingIn_001c4c1c = 1;
  }
  cVar1 = _IsMouseDown();
  if (cVar1 != '\0') {
    _printf("Mouse clicked on Game over screen!");
  }
  cVar1 = _WasAKeyPressed();
  if (cVar1 != '\0') {
    _printf("Mouse clicked on Game over screen!");
  }
  cVar1 = _IsMouseDown();
  if (((cVar1 == '\0') && (cVar1 = _WasAKeyPressed(), cVar1 == '\0')) &&
     (puVar3 = (undefined *)_GetElapsedMillisecs(), puVar3 < &UNK_00002135)) {
    return;
  }
  uVar4 = _GetElapsedMillisecs();
  _printf("Elapsed milliseconds at end of Game Over: %d",uVar4);
  _FinishGameOverOrBeatMansionScreen(1);
  return;
}


// ---- _DoBeatTheMansionScreen @ 0x51016 (2086 bytes)

void _DoBeatTheMansionScreen(char param_1)

{
  short sVar1;
  undefined4 uVar2;
  SpritePtr local_23c;
  char local_235 [255];
  undefined2 local_136;
  undefined2 local_134;
  undefined2 local_132;
  undefined1 local_130 [256];
  undefined2 local_30;
  short local_2e;
  undefined1 local_2c;
  undefined1 local_2b;
  undefined2 local_2a;
  short local_28;
  short sStack_26;
  undefined4 local_24;
  undefined1 local_20 [2];
  short sStack_1e;
  undefined4 local_1c;
  char *local_18;
  int local_14;
  undefined1 local_d;
  
  local_136 = 0xffff;
  local_134 = 4;
  local_132 = 4;
  if (*PTR__gEditorIsRunning_001c495c == '\0') {
    _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    _WriteRoomInfoToFile();
    _WriteCStringToFile("BEAT MANSION!!!\r");
    if (*PTR__gFileIsOpen_001c4760 != '\0') {
      _fprintf(*(FILE **)PTR__gOutputFileP_001c4ba4,
               "Total time spent in mansion: %d minutes and %d seconds.\r",
               *(int *)PTR__gTimeSpentInMansion_001c46fc / 0x3c,
               *(int *)PTR__gTimeSpentInMansion_001c46fc % 0x3c);
    }
    if (*PTR__gIgnoreCheating_001c4b38 != '\0') {
      *PTR__gDidCheat_001c4774 = 0;
    }
    if (((*PTR__gMansionIsCustomMansion_001c48ac == '\0') &&
        (*(short *)PTR__gCurrentMansionNumber_001c4824 != -2)) && (*PTR__gDidCheat_001c4774 == '\0')
       ) {
      if (*(short *)PTR__gCurrentMansionNumber_001c4824 < 0) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                   PTR____FUNCTION___30088_001c4be0,0x10a0);
      }
      _AddMansionToArray((int)(char)*PTR__gDifficultySetting_001c4870,
                         (int)*(short *)PTR__gCurrentMansionNumber_001c4824);
    }
    if (*(short *)PTR__gCurrentMansionNumber_001c4824 == -2) {
      *PTR__gPlayTutorialMansion_001c4bdc = 0;
    }
    _WriteProfileToDisk();
    uVar2 = _GetScreensFolderPath();
    local_14 = _LoadAndInstallBackgroundSprite(&local_23c,uVar2,"mansioncomplete.jpg",0xffffffff,0);
    if (local_14 != 0) {
      _MMFatalErrorCallback
                (local_14,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                 PTR____FUNCTION___30088_001c4be0,0x10ad);
    }
    _SWSetSpriteScaledSize(local_23c,0,0);
    _SetPascalString(local_130,"\x12Comic Sans MS Bold");
    local_30 = 0;
    local_2e = 0x16;
    local_2c = 1;
    local_2b = 1;
    local_2a = 0;
    _SWSetRect(local_20,0x18b,0x102,0x280,0x118);
    local_24 = local_1c;
    local_28 = (short)local_1c - local_2e;
    sStack_26 = sStack_1e;
    _MyDrawCTextJustified("Mansion:",local_130,local_20,PTR__rgbWhiteColor_001c4bcc);
    _SWOffsetRect(local_20,0,0x17);
    _MyDrawCTextJustified
              (PTR__gMansionFilename_001c489c,local_130,local_20,PTR__rgbWhiteColor_001c4bcc);
    if (*(int *)PTR__gTimeSpentInMansion_001c46fc % 0x3c < 10) {
      local_18 = "0";
    }
    else {
      local_18 = "";
    }
    _sprintf(local_235,"Time: %d:%s%d",*(int *)PTR__gTimeSpentInMansion_001c46fc / 0x3c,local_18,
             *(int *)PTR__gTimeSpentInMansion_001c46fc % 0x3c);
    _SWOffsetRect(local_20,0,0x28);
    _MyDrawCTextJustified(local_235,local_130,local_20,PTR__rgbWhiteColor_001c4bcc);
    _SWOffsetRect(local_20,0,0x28);
    _sprintf(local_235,"Total Score: %d",*(undefined4 *)PTR__gScore_001c48a4);
    _MyDrawCTextJustified(local_235,local_130,local_20,PTR__rgbWhiteColor_001c4bcc);
    _SWOffsetRect(local_20,0,0x17);
    sVar1 = _GetPercentSecrets();
    _sprintf(local_235,"Secrets found: %d%%",(int)sVar1);
    _MyDrawCTextJustified(local_235,local_130,local_20,PTR__rgbWhiteColor_001c4bcc);
    _SWOffsetRect(local_20,0,0x28);
    _sprintf(local_235,"Lives left: %d",(int)*(short *)PTR__gNumLivesLeft_001c49e4);
    _MyDrawCTextJustified(local_235,local_130,local_20,PTR__rgbWhiteColor_001c4bcc);
    local_24 = CONCAT22(local_24._2_2_,(short)local_1c);
    if (*PTR__gDidCheat_001c4774 != '\0') {
      local_1c = CONCAT22(local_1c._2_2_,0xf0);
      local_28 = 0xf0 - local_2e;
      _MyDrawCTextJustified("USED CHEATS",local_130,local_20,&local_136);
    }
    _local_28 = CONCAT22(sStack_26 + -0x32,local_28);
    local_24 = 0x28001e0;
    _MMRenderSceneForWorkAreaSprite(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    _SWAutoScaleRect(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_28);
    local_14 = _SWCreateSpriteFromWorkArea
                         (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_23c,0,&local_28,0);
    if (local_14 == 0) {
      _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gTextSpriteLayerP_001c4804);
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gTextSpriteLayerP_001c4804,local_23c);
    }
    if (*PTR__gFullScreenWindow_001c48ec == '\0') {
      _MMSetCursorForGame();
    }
    _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
    _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
    if (param_1 == '\0') {
      if (*PTR__gSoundIsTurnedOn_001c47fc != '\0') {
        local_d = *PTR__gMusicIsOn_001c47c0;
        *PTR__gMusicIsOn_001c47c0 = 1;
        _MyPlayMusic("Beat The Mansion",(int)*(short *)PTR__gTitleMusicVolume_001c48bc,0,0);
        *PTR__gMusicIsOn_001c47c0 = local_d;
      }
      *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 9;
      *PTR__gSpriteWorldIsValid_001c48c8 = 1;
      _UpdateMenuBar();
      _GiveOSTime();
      _DoGameFadeToNormalFullAnim(0xb4);
      _StartMicrosecTimer();
      _MMFlushEvents();
      *PTR__gBeatMansionScreenWasDismissed_001c4acc = 0;
    }
    _puts("Drawing beat mansion screen.");
  }
  return;
}


// ---- _DrawTextUsingSprite @ 0x5183c (328 bytes)

SpritePtr DrawTextUsingSprite(int param_1,undefined4 param_2,undefined4 param_3,short *param_4)

{
  SpritePtr newSpriteP;
  
  newSpriteP = (SpritePtr)_NewBitmapTextSprite((int)(short)(param_4[3] - param_4[1]));
  if (newSpriteP != (SpritePtr)0x0) {
    _SWSetSpriteScaledFactor
              (newSpriteP,(float)(int)(*(short *)(param_1 + 0x38) / 0x280) * 0.3,
               (float)(int)(short)((short)(*(short *)(param_1 + 0x38) * 0x100008889 >> 0x18) -
                                  (*(short *)(param_1 + 0x38) >> 0xf)) * 0.3);
    _SetBMTextSpriteText(newSpriteP,param_2);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gCustomDialogSpriteLayerP_001c494c,newSpriteP);
    _SWSetSpriteLocation(newSpriteP,(float)(int)param_4[1],(float)(int)*param_4);
  }
  return newSpriteP;
}


// ---- _HandleBeatTheMansionScreenCallback @ 0x51984 (88 bytes)

void _HandleBeatTheMansionScreenCallback(void)

{
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  if (*PTR__gBeatMansionScreenWasDismissed_001c4acc != '\0') {
    _FinishGameOverOrBeatMansionScreen(0);
  }
  return;
}


// ---- _GetPercentSecrets @ 0x519dc (87 bytes)

int _GetPercentSecrets(void)

{
  undefined4 local_10;
  
  if (*(int *)PTR__gTotalSecretsInMansion_001c4884 < 1) {
    local_10 = 100;
  }
  else {
    local_10 = (int)(short)((*(int *)PTR__gNumSecretsFound_001c4990 * 100) /
                           *(int *)PTR__gTotalSecretsInMansion_001c4884);
  }
  return local_10;
}


// ---- _DoBeatTheGameSlideshow @ 0x51a33 (444 bytes)

void _DoBeatTheGameSlideshow(void)

{
  undefined4 uVar1;
  SpritePtr local_14;
  int local_10;
  
  if (*PTR__gEditorIsRunning_001c495c == '\0') {
    _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    uVar1 = _GetScreensFolderPath();
    local_10 = _LoadAndInstallBackgroundSprite(&local_14,uVar1,"beatgame1.jpg",0xffffffff,0);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                 PTR____FUNCTION___30238_001c4b80,0x11eb);
    }
    if (local_10 == 0) {
      _SWSetSpriteScaledSize
                (local_14,SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38),0
                               ),
                 SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a),0));
    }
    if (*PTR__gFullScreenWindow_001c48ec == '\0') {
      _MMRemoveGameCursor();
      _MMShieldCursor();
    }
    _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
    _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
    *PTR__gMusicWasOn_001c4b70 = *PTR__gMusicIsOn_001c47c0;
    *PTR__gMusicIsOn_001c47c0 = 1;
    _MyPlayMusic("Beat The Game Intro",(int)*(short *)PTR__gTitleMusicVolume_001c48bc,0,0);
    *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 10;
    *PTR__gSpriteWorldIsValid_001c48c8 = 1;
    _UpdateMenuBar();
    _GiveOSTime();
    _DoGameFadeToNormal(0xb4);
    _MMFlushEvents();
    *(undefined2 *)PTR__gCurrentEndingScreen_001c4bf8 = 1;
    _StartMicrosecTimer();
  }
  return;
}


// ---- _HandleBeatTheGameCallback @ 0x51bef (1343 bytes)

void _HandleBeatTheGameCallback(void)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 local_18;
  SpritePtr local_14;
  int local_10;
  
  local_10 = 0;
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  cVar1 = _IsMusicDone();
  if (cVar1 != '\0') {
    _MyPlayMusic("Beat The Game Main",(int)*(short *)PTR__gTitleMusicVolume_001c48bc,0,1);
  }
  if (*(short *)PTR__gCurrentEndingScreen_001c4bf8 == 1) {
    iVar2 = _GetElapsedSeconds();
    if (7 < iVar2) {
      _DoGameFadeToBlack(0x37,0);
      _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gBackgroundScreenSpriteLayerP_001c4780);
      uVar3 = _GetScreensFolderPath();
      local_10 = _LoadAndInstallBackgroundSprite(&local_14,uVar3,"beatgame2.jpg",0xffffffff,0);
      if (local_10 != 0) {
        _MMFatalErrorCallback
                  (local_10,
                   "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                   PTR____FUNCTION___30269_001c4b34,0x1225);
      }
      if (local_10 == 0) {
        _SWSetSpriteScaledSize
                  (local_14,SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38)
                                  ,0),
                   SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a),0));
      }
      _GiveOSTime();
      _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
      _DoGameFadeToNormal(0x37);
      _StartMicrosecTimer();
      *(short *)PTR__gCurrentEndingScreen_001c4bf8 =
           *(short *)PTR__gCurrentEndingScreen_001c4bf8 + 1;
    }
  }
  else if (*(short *)PTR__gCurrentEndingScreen_001c4bf8 == 2) {
    iVar2 = _GetElapsedSeconds();
    if (0xf < iVar2) {
      _DoGameFadeToBlack(0x37,0);
      _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gBackgroundScreenSpriteLayerP_001c4780);
      uVar3 = _GetScreensFolderPath();
      local_10 = _LoadAndInstallBackgroundSprite(&local_18,uVar3,"beatgame3.png",0xffffffff,0);
      if (local_10 != 0) {
        uVar3 = _GetScreensFolderPath();
        local_10 = _LoadAndInstallBackgroundSprite(&local_18,uVar3,"beatgame3.jpg",0xffffffff,0);
      }
      if (local_10 != 0) {
        _MMFatalErrorCallback
                  (local_10,
                   "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                   PTR____FUNCTION___30269_001c4b34,0x123d);
      }
      if (local_10 == 0) {
        _SWSetSpriteScaledFactor
                  (local_18,(float)(int)(*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38) /
                                        0x500),
                   (float)(int)(short)((short)(*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a
                                                         ) * 0x100008889 >> 0x19) -
                                      (*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a) >> 0xf
                                      )));
      }
      _SWCenterSpriteInRect(local_18,*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
      _GiveOSTime();
      _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
      _DoGameFadeToNormal(0x37);
      _StartMicrosecTimer();
      *(short *)PTR__gCurrentEndingScreen_001c4bf8 =
           *(short *)PTR__gCurrentEndingScreen_001c4bf8 + 1;
    }
  }
  else if (*(short *)PTR__gCurrentEndingScreen_001c4bf8 == 3) {
    iVar2 = _GetElapsedSeconds();
    if (3 < iVar2) {
      _DoGameFadeToBlack(0x37,0);
      _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gBackgroundScreenSpriteLayerP_001c4780);
      uVar3 = _GetScreensFolderPath();
      local_10 = _LoadAndInstallBackgroundSprite(&local_14,uVar3,"beatgame4.jpg",0xffffffff,0);
      if (local_10 != 0) {
        _MMFatalErrorCallback
                  (local_10,
                   "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                   PTR____FUNCTION___30269_001c4b34,0x1252);
      }
      if (local_10 == 0) {
        _SWSetSpriteScaledSize
                  (local_14,SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38)
                                  ,0),
                   SUB42((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a),0));
      }
      _GiveOSTime();
      _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
      _DoGameFadeToNormal(0xb4);
      _MMFlushEvents();
      *PTR__gBeatGameScreenWasDismissed_001c4ad4 = 0;
      *(short *)PTR__gCurrentEndingScreen_001c4bf8 =
           *(short *)PTR__gCurrentEndingScreen_001c4bf8 + 1;
    }
  }
  else if (*(short *)PTR__gCurrentEndingScreen_001c4bf8 == 4) {
    cVar1 = _IsMouseDown();
    if ((cVar1 != '\0') || (*PTR__gBeatGameScreenWasDismissed_001c4ad4 != '\0')) {
      if (*PTR__gMusicWasOn_001c4b70 == '\0') {
        _MyFadeMusicNow();
        *PTR__gMusicIsOn_001c47c0 = *PTR__gMusicWasOn_001c4b70;
      }
      _DoGameFadeToBlack(0x37,0);
      _DoBeatTheMansionScreen(0);
    }
  }
  return;
}


// ---- _DoRegisterNowDialog @ 0x5212e (133 bytes)

void _DoRegisterNowDialog(short param_1)

{
  char local_202 [500];
  short local_e;
  
  _sprintf(local_202,
           "You\'ve just explored %d%% of this mansion. You can\'t play any further until you purchase the game. Would you like to buy it now?"
           ,(int)param_1);
  local_e = _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,local_202,2);
  if (local_e == 2) {
    _LaunchRegistrationWebsite();
    _ShutDown();
  }
  return;
}


// ---- _FinishGameOverOrBeatMansionScreen @ 0x521b3 (433 bytes)

void _FinishGameOverOrBeatMansionScreen(char param_1)

{
  short sVar1;
  undefined4 local_34;
  undefined2 local_20;
  undefined1 local_1d;
  
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
  local_1d = '\0';
  *PTR__gCurrentlyInMansion_001c4908 = 0;
  _MMFlushEvents();
  _StopAndDisposeMusic();
  if (((*PTR__gMansionIsCustomMansion_001c48ac == '\0') &&
      (*(short *)PTR__gCurrentMansionNumber_001c4824 != -2)) && (*PTR__gDidCheat_001c4774 == '\0'))
  {
    sVar1 = _GetPercentSecrets();
    local_1d = _CheckForHighScore((int)*(short *)PTR__gCurrentMansionNumber_001c4824,
                                  (int)(char)*PTR__gDifficultySetting_001c4870,
                                  *(undefined4 *)PTR__gScore_001c48a4,(int)sVar1,param_1 == '\0');
  }
  _DoGameFadeToBlack(0x37,1);
  _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  if (*PTR__gDidCheat_001c4774 != '\0') {
    local_1d = '\0';
  }
  if (local_1d == '\0') {
    if (((*PTR__gMansionIsCustomMansion_001c48ac == '\0') && (param_1 == '\0')) &&
       (*(short *)PTR__gCurrentMansionNumber_001c4824 != 10)) {
      if (*(short *)PTR__gCurrentMansionNumber_001c4824 < 5) {
        local_20 = 1;
      }
      else {
        local_20 = 2;
      }
      _SetUpSelectMansionScreen(local_20);
    }
    else {
      _SetUpTitleScreen();
    }
  }
  else {
    if ((param_1 == '\0') && (*(short *)PTR__gCurrentMansionNumber_001c4824 != 10)) {
      local_34 = 1;
    }
    else {
      local_34 = 0;
    }
    _SetUpHighScoresScreen((int)*(short *)PTR__gCurrentMansionNumber_001c4824,1,local_34);
  }
  return;
}


// ---- _AddBackButtonToScreen @ 0x52364 (280 bytes)

void _AddBackButtonToScreen(SpriteLayerPtr param_1,short param_2)

{
  SpritePtr newSpriteP;
  longdouble extraout_ST0;
  
  newSpriteP = (SpritePtr)_NewButtonSprite(0xd6,0x32);
  if (newSpriteP != (SpritePtr)0x0) {
    _SetUpButtonSprite(newSpriteP,0,1,2,0);
    _InstallButtonProc(newSpriteP,PTR_LAB_001c4034);
    _SetButtonPushedOffset(newSpriteP,1,1);
    _AdjustCollisionRectForOffset(newSpriteP);
    _SWAddSprite(param_1,newSpriteP);
    _SWGetSpriteHeight(newSpriteP);
    _SWSetSpriteLocation(newSpriteP,2.0,((480.0 - (float)extraout_ST0) - 2.0) - (float)(int)param_2)
    ;
  }
  return;
}


// ---- _AddCustomCursorToScreen @ 0x5247c (343 bytes)

void _AddCustomCursorToScreen(void)

{
  SpritePtr pSVar1;
  
  if ((*(int *)PTR__gSpriteWorldP_001c40d0 != 0) && (*PTR__gMainSpritesAreLoaded_001c4178 != '\0'))
  {
    pSVar1 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gCustomCursorSpriteLayerP_001c49f4,
                              (SpritePtr)0x0);
    if (pSVar1 == (SpritePtr)0x0) {
      pSVar1 = (SpritePtr)_NewMiscSprite(0xdb);
      if (pSVar1 != (SpritePtr)0x0) {
        _SWAddSprite(*(SpriteLayerPtr *)PTR__gCustomCursorSpriteLayerP_001c49f4,pSVar1);
        _SWSetSpriteMoveProc(pSVar1,(MoveProcPtr_t)PTR_LAB_001c4b08);
        _SWSetSpriteNonScrolling(pSVar1,'\x01');
        _SWSetSpriteScaledFactor
                  (pSVar1,(float)(int)(*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38) /
                                      0x500),
                   (float)(int)(short)((short)(*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a
                                                         ) * 0x100008889 >> 0x19) -
                                      (*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a) >> 0xf
                                      )));
        _CustomCursorMoveProc(pSVar1);
      }
    }
    _SWUnpauseSpriteLayer(*(SpriteLayerPtr *)PTR__gCustomCursorSpriteLayerP_001c49f4);
  }
  return;
}


// ---- _MMStandardButtonProc @ 0x525d3 (335 bytes)

void MMStandardButtonProc(undefined4 param_1,short param_2,short param_3,short param_4)

{
  if (param_3 != param_4) {
    if (param_3 == 2) {
      _MMPlayStereoSound2(0,0x39,8,1,(int)(*(short *)PTR__gSoundVolume_001c48dc / 3),
                          0x3ff0000000000000,0);
    }
    else if (param_4 == 2) {
      _MMPlayStereoSound2(0,0x39,8,1,(int)(*(short *)PTR__gSoundVolume_001c48dc / 3),
                          0x3fe999999999999a,0);
    }
    if ((((param_2 == 0xdf) || (param_2 == 0xe0)) && (param_4 == 2)) && (param_3 == 1)) {
      _SetButtonDimmedMode(param_1,1);
    }
  }
  return;
}


// ---- _MMTitleScreenButtonProc @ 0x52722 (440 bytes)

void MMTitleScreenButtonProc(SpritePtr param_1,undefined4 param_2,short param_3,short param_4)

{
  if (param_3 != param_4) {
    if (param_3 == 2) {
      _MMPlayStereoSound2(0,0x39,8,1,(int)(*(short *)PTR__gSoundVolume_001c48dc / 3),
                          0x3ff0000000000000,0);
    }
    else if (param_4 == 2) {
      _MMPlayStereoSound2(0,0x39,8,1,(int)(*(short *)PTR__gSoundVolume_001c48dc / 3),
                          0x3fe999999999999a,0);
    }
    if (param_3 == 2) {
      _SWSetSpriteColor(param_1,0x8fff,0x8fff,0x8fff);
    }
    else {
      _SWSetSpriteColor(param_1,0xffff,0xffff,0xffff);
    }
    _MakeButtonBounceWhenSelected
              (param_1,(int)param_3,(int)param_4,0x3ecccccd,0x3f800000,0x3fc00000,0x400ccccd);
  }
  return;
}


// ---- _MMTabButtonProc @ 0x528da (474 bytes)

void MMTabButtonProc(SpritePtr param_1,short param_2,short param_3,short param_4)

{
  if (param_3 != param_4) {
    if (param_3 == 2) {
      _MMPlayStereoSound2(0,0x39,8,1,(int)(*(short *)PTR__gSoundVolume_001c48dc / 3),
                          0x3ff0000000000000,0);
    }
    else if (param_4 == 2) {
      _MMPlayStereoSound2(0,0x39,8,1,(int)(*(short *)PTR__gSoundVolume_001c48dc / 3),
                          0x3fe999999999999a,0);
    }
    if (param_3 == 2) {
      _SWSetSpriteColor(param_1,0x9fff,0x9fff,0x9fff);
    }
    else {
      _SWSetSpriteColor(param_1,0xffff,0xffff,0xffff);
    }
    if (param_2 == 10) {
      if (param_3 == 1) {
        _SWOffsetSprite((SpritePtr)param_1->misc2,0.0,-16.0);
      }
      else if (param_4 == 1) {
        _SWOffsetSprite((SpritePtr)param_1->misc2,0.0,16.0);
      }
    }
  }
  return;
}


// ---- _DifficultyScreenButtonProc @ 0x52ab4 (482 bytes)

void DifficultyScreenButtonProc(undefined4 param_1,short param_2,short param_3,short param_4)

{
  if ((param_3 == 1) || (param_3 == 2)) {
    if (*(int *)PTR__gIllustrationSpriteP_001c4be4 == 0) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameScreens.c",
                 PTR____FUNCTION___30562_001c4b7c,0x137f);
    }
    _SWSetSpriteVisible(*(SpritePtr *)PTR__gIllustrationSpriteP_001c4be4,'\x01');
    _SWSetCurrentFrameIndex(*(SpritePtr *)PTR__gIllustrationSpriteP_001c4be4,(int)param_2);
  }
  if (param_3 != param_4) {
    if (param_3 == 2) {
      _MMPlayStereoSound2(0,0x39,8,1,(int)(*(short *)PTR__gSoundVolume_001c48dc / 3),
                          0x3ff0000000000000,0);
    }
    else if (param_4 == 2) {
      _MMPlayStereoSound2(0,0x39,8,1,(int)(*(short *)PTR__gSoundVolume_001c48dc / 3),
                          0x3fe999999999999a,0);
    }
    _MakeButtonBounceWhenSelected
              (param_1,(int)param_3,(int)param_4,0x3e4ccccd,0x3e4ccccd,0x3fc00000,0x400ccccd);
  }
  return;
}


// ---- _MakeButtonBounceWhenSelected @ 0x52c96 (338 bytes)

void _MakeButtonBounceWhenSelected
               (SpriteRec *param_1,short param_2,short param_3,undefined4 param_4,undefined4 param_5
               ,undefined4 param_6,undefined4 param_7)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  if (*(float *)&param_1->f_164 == -1.0) {
    *(float *)&param_1->f_164 = param_1->drawData->scaledWidthFactor;
    if (*(float *)&param_1->f_164 == -1.0) {
      param_1->f_164 = '\0';
      param_1->f_165 = '\0';
      param_1->f_166 = 0x80;
      param_1->f_167 = '?';
    }
    _SWSetSpriteScalingMode(param_1,'\x01');
  }
  param_1->f_168 = (undefined1)param_4;
  param_1->f_169 = param_4._1_1_;
  param_1->f_16a = param_4._2_1_;
  param_1->f_16b = param_4._3_1_;
  param_1->f_158 = (undefined1)param_6;
  param_1->f_159 = param_6._1_1_;
  param_1->f_15a = param_6._2_1_;
  param_1->f_15b = param_6._3_1_;
  param_1->f_15c = (undefined1)param_7;
  param_1->f_15d = param_7._1_1_;
  param_1->f_15e = param_7._2_1_;
  param_1->f_15f = param_7._3_1_;
  param_1->f_160 = (undefined1)param_5;
  param_1->f_161 = param_5._1_1_;
  param_1->f_162 = param_5._2_1_;
  param_1->f_163 = param_5._3_1_;
  if (param_2 == 1) {
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_MMBounceButtonMoveProc_001c4b1c);
    param_1->f_150 = '\0';
    param_1->f_151 = '\0';
    param_1->f_152 = 0xb4;
    param_1->f_153 = 'B';
    param_1->f_154 = '\0';
    param_1->f_155 = '\0';
    param_1->f_156 = 0xf0;
    param_1->f_157 = 'B';
  }
  else if ((param_3 == 1) && (_SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)0x0), param_2 != 2)) {
    uVar1._0_1_ = param_1->f_164;
    uVar1._1_1_ = param_1->f_165;
    uVar1._2_1_ = param_1->f_166;
    uVar1._3_1_ = param_1->f_167;
    uVar2._0_1_ = param_1->f_164;
    uVar2._1_1_ = param_1->f_165;
    uVar2._2_1_ = param_1->f_166;
    uVar2._3_1_ = param_1->f_167;
    _SWSetSpriteScaledFactor(param_1,uVar2,uVar1);
  }
  return;
}


// ---- _MMBounceButtonMoveProc @ 0x52de8 (492 bytes)

void MMBounceButtonMoveProc(int param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  longdouble lVar6;
  longdouble lVar7;
  
  *(float *)(param_1 + 0x150) = *(float *)(param_1 + 0x158) + *(float *)(param_1 + 0x150);
  if (360.0 <= *(float *)(param_1 + 0x150)) {
    *(float *)(param_1 + 0x150) = *(float *)(param_1 + 0x150) - 360.0;
  }
  *(float *)(param_1 + 0x154) = *(float *)(param_1 + 0x15c) + *(float *)(param_1 + 0x154);
  if (360.0 <= *(float *)(param_1 + 0x154)) {
    *(float *)(param_1 + 0x154) = *(float *)(param_1 + 0x154) - 360.0;
  }
  fVar1 = *(float *)(param_1 + 0x164);
  fVar2 = *(float *)(param_1 + 0x168);
  lVar6 = (longdouble)_SWCos((double)*(float *)(param_1 + 0x150));
  fVar3 = *(float *)(param_1 + 0x160);
  fVar4 = *(float *)(param_1 + 0x164);
  fVar5 = *(float *)(param_1 + 0x168);
  lVar7 = (longdouble)_SWCos((double)*(float *)(param_1 + 0x154));
  _SWSetSpriteScaledFactor
            (param_1,(float)((double)fVar3 * ((double)lVar6 / 12.0) + (double)(fVar2 + fVar1)),
             (float)((double)*(float *)(param_1 + 0x160) * ((double)lVar7 / 6.0) +
                    (double)(fVar5 + fVar4)));
  return;
}


