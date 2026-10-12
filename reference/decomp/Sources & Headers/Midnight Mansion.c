// Ghidra decompilation of functions from original source file:
//   Sources & Headers/Midnight Mansion.c

// ---- _main @ 0x2d0cf (166 bytes)

undefined4 _main(void)

{
  char cVar1;
  
  _InitVariables();
  _Randomize();
  _OpenMacDialogResFile();
  _CreateAppSupportDirectories();
  _CheckForLicenseAndDataFiles();
  _UpdateHiddenDateFile();
  _ASWHighScoresInit(0x4d414e53);
  _OpenGameSupportFiles();
  _InitSpriteArray();
  _SWSetCleanUpFunction(PTR_LAB_00033b8e_4_001c4918);
  _LoadPreferences();
  cVar1 = _IsMMRegistered(2);
  if ((cVar1 == '\0') && (*PTR__gEditorIsRunning_001c495c != '\0')) {
    _DoEditorUnregisteredDialog();
  }
  if (*PTR__gEditorIsRunning_001c495c == '\0') {
    _StartUpGameCarbon();
  }
  else {
    _StartUpEditor();
  }
  return 0;
}


// ---- _CheckForLicenseAndDataFiles @ 0x2d175 (442 bytes)

void _CheckForLicenseAndDataFiles(void)

{
  char cVar1;
  int iVar2;
  undefined1 local_18 [16];
  
  iVar2 = _ReadEpisodeDataFile("MM HD Episode 1 data");
  *PTR__gEpisode1IsUnlocked_001c4744 = iVar2 == 0x38e9e3f;
  iVar2 = _ReadEpisodeDataFile("MM HD Episode 2 data");
  *PTR__gEpisode2IsUnlocked_001c48d0 = iVar2 == 0x58758e1;
  if (*PTR__gEpisode1IsUnlocked_001c4744 != '\0') {
    _puts("Episode 1 detected on this computer.");
  }
  if (*PTR__gEpisode2IsUnlocked_001c48d0 != '\0') {
    _puts("Episode 2 detected on this computer.");
  }
  if (*PTR__gEpisode1IsUnlocked_001c4744 == '\0') {
    cVar1 = _CheckForLicenseFilename(local_18,"Midnight Mansion HD Episode 1 License");
    if (cVar1 != '\0') {
      *PTR__gEpisode1IsUnlocked_001c4744 = 1;
      _CreateEpisodeDataFile("MM HD Episode 1 data",0x38e9e3f);
      goto LAB_0002d300;
    }
  }
  if (*PTR__gEpisode2IsUnlocked_001c48d0 == '\0') {
    cVar1 = _CheckForLicenseFilename(local_18,"Midnight Mansion HD Episode 2 License");
    if (cVar1 != '\0') {
      *PTR__gEpisode2IsUnlocked_001c48d0 = 1;
      _CreateEpisodeDataFile("MM HD Episode 2 data",0x58758e1);
      goto LAB_0002d300;
    }
  }
  cVar1 = _CheckForLicenseFilename(local_18,"Midnight Mansion HD License");
  if (cVar1 != '\0') {
    *PTR__gEpisode2IsUnlocked_001c48d0 = 1;
    *PTR__gEpisode1IsUnlocked_001c4744 = *PTR__gEpisode2IsUnlocked_001c48d0;
    _CreateEpisodeDataFile("MM HD Episode 1 data",0x38e9e3f);
    _CreateEpisodeDataFile("MM HD Episode 2 data",0x58758e1);
  }
LAB_0002d300:
  if ((*PTR__gEpisode1IsUnlocked_001c4744 != '\0') && (*PTR__gEpisode2IsUnlocked_001c48d0 != '\0'))
  {
    *PTR__gCanPlayBothEpisodes_001c4738 = 1;
  }
  return;
}


// ---- _CreateAppSupportDirectories @ 0x2d32f (274 bytes)

void _CreateAppSupportDirectories(void)

{
  undefined4 uVar1;
  char local_200 [504];
  
  uVar1 = _GetPlainAppSupportPath();
  _CreateDirectory(uVar1,"ActionSoft");
  uVar1 = _GetPlainAppSupportPath();
  _sprintf(local_200,"%s/ActionSoft",uVar1);
  _CreateDirectory(local_200,"Midnight Mansion");
  uVar1 = _GetAppSupportPath("");
  _CreateDirectory(uVar1,"Player Profiles");
  uVar1 = _GetAppSupportPath("");
  _CreateDirectory(uVar1,"Saved Games");
  uVar1 = _GetAppSupportPath("");
  _CreateDirectory(uVar1,"Suspended Game Data");
  uVar1 = _GetAppSupportPath("");
  _CreateDirectory(uVar1,"User Movies");
  uVar1 = _GetGameDataPath();
  _CreateDirectory(uVar1,"Mansion Data");
  return;
}


// ---- _InitVariables @ 0x2d441 (569 bytes)

void _InitVariables(void)

{
  *PTR__gEditorIsRunning_001c495c = 0;
  *(undefined2 *)PTR__gStatsHeight_001c47cc = 0x30;
  *PTR__gQuitGame_001c49d0 = 0;
  *PTR__gFileIsOpen_001c4760 = 0;
  *PTR__gCanUseLadderJump_001c4828 = 1;
  *PTR__gUseOutputFile_001c47f0 = 0;
  *PTR__gSpriteWorldIsValid_001c48c8 = 0;
  *PTR__gMainSpritesAreLoaded_001c4178 = 0;
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
  *PTR__gDialogBoxIsOpen_001c49c8 = 0;
  *PTR__gCustomDialogIsVisible_001c49b8 = 0;
  *PTR__gMenuBarIsVisible_001c4794 = 1;
  *(undefined4 *)PTR__gSpriteWorldP_001c40d0 = 0;
  *(undefined4 *)PTR__gStatsSpriteWorldP_001c48f4 = 0;
  *(undefined2 *)PTR__gLastOpenDialog_001c49c0 = 0x10;
  *PTR__gPlayingGame_001c4924 = 0;
  *PTR__gMMIsActive_001c49e0 = 1;
  *PTR__gUseHaloSprite_001c4700 = 1;
  *PTR__gArrowKeysAreEnabledForTesting_001c49f0 = 0;
  *(undefined2 *)PTR__gAppResFile_001c49b0 = 0xffff;
  *(undefined2 *)PTR__gGraphicsFile_001c48e4 = 0xffff;
  *(undefined2 *)PTR__gSpritesFile_001c473c = 0xffff;
  *(undefined2 *)PTR__gSoundsFile_001c4904 = 0xffff;
  *(undefined4 *)PTR__gOpeningMusicRefNum_001c471c = 0;
  *(undefined2 *)PTR__gCurrentMusicIndex_001c476c = 0;
  *PTR__gCanQuit_001c4968 = 1;
  *PTR__gDemoMode_001c48b8 = 0;
  *PTR__gFullScreenWindow_001c48ec = 0;
  *PTR__gWindowIsMinimized_001c4868 = 0;
  *PTR__gWasFullscreenWhenMinimized_001c47bc = 0;
  *PTR__gPheliosDistribution_001c4964 = 0;
  *PTR__gHasShownOrganScreen_001c4714 = 0;
  *(undefined2 *)PTR__gCurrentLevel_001c49bc = 1;
  *(undefined2 *)PTR__gNumItemsBeingCarried_001c48cc = 0;
  *PTR__gDisplayFPSAndLevelResID_001c4960 = 0;
  *PTR__gStartRecordingMovie_001c48d4 = 0;
  *PTR__gLayersArePausedForSaveAnim_001c487c = 0;
  *(undefined4 *)PTR__gTotalSecretsInMansion_001c4884 = 0;
  *PTR__gCursorIsInWindow_001c47a4 = 1;
  *PTR__gUseCustomCursor_001c4974 = 1;
  *(undefined4 *)PTR__gJumpSpeed_001c4708 = 0x411ccccd;
  *(undefined4 *)PTR__gJumpChange_001c4790 = 0x3f0ccccd;
  *(undefined4 *)PTR__gHorizontalBoost_001c47d0 = 0x3f8a3d71;
  *PTR__gDifficultySetting_001c4870 = 1;
  *PTR__gFasterInGameMap_001c4820 = 0;
  return;
}


// ---- _CreateAndAddSpriteLayers @ 0x2d67a (7350 bytes)

void _CreateAndAddSpriteLayers(void)

{
  SWError SVar1;
  
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gBackgroundScreenSpriteLayerP_001c4780);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x16b);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gRearBackgroundPictureSpriteLayerP_001c4980);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x16d);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gBackgroundPictureSpriteLayerP_001c475c);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x16f);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gLowerTileFringeSpriteLayerP_001c48c0);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x171);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gBottomSpriteLayerP_001c4140);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x173);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gExitSpriteLayerP_001c403c);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x175);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gBackgroundSpriteLayerP_001c4070);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x177);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gShadowSpriteLayerP_001c4978);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x179);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gBackLavaSplashSpriteLayerP_001c41cc);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x17b);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gFrontLavaSplashSpriteLayerP_001c4024);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x17d);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gLavaGlowSpriteLayerP_001c49d4);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x17f);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gSignSpriteLayerP_001c41f8);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x181);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gGlowSpriteLayerP_001c42a0);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x183);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gLanternGlowSpriteLayerP_001c4214);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x185);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gTorchSpriteLayerP_001c4144);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x187);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gFlameSpriteLayerP_001c4130);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x189);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gCandleSpriteLayerP_001c421c);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x18b);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gCandleHolderSpriteLayerP_001c417c);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x18d);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gFloorCandleStandSpriteLayerP_001c4808);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,399);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gLeverSpriteLayerP_001c41d4);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x191);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gSnakeSpriteLayerP_001c4208);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x193);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gMedusaSpriteLayerP_001c4094);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x195);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gGhostSpriteLayerP_001c496c);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x197);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gEyeballSpriteLayerP_001c41c8);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x199);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gBirdSpriteLayerP_001c41c4);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x19b);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gWebTrappedSpriteLayerP_001c4150);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x19d);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gSkeletonBonesSpriteLayerP_001c40a0);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x19f);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gEnemySpriteLayerP_001c40b0);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1a1);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gLadybugSpriteLayerP_001c4844);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1a3);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gSpyVsSpySpriteLayerP_001c4874);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1a5);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gBigSpiderSpriteLayerP_001c42a4);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1a7);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gDyingMedusaSpriteLayerP_001c49a8);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1a9);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gVertMovingPlatformSpriteLayerP_001c423c);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1ab);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1ad);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gFallingPlatformSpriteLayerP_001c499c);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1af);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gMovingPlatformBlockerSpriteLayerP_001c41ac);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1b1);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gDisappearingBlockSpriteLayerP_001c4930);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1b3);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gHeroOnPlatformSpriteLayerP_001c491c);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1b5);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gForegroundSpriteLayerP_001c41f0);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1b7);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gSavePedestalSpriteLayerP_001c4138);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1b9);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gStatueSpriteLayerP_001c4240);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1bb);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gBrickSwitchSpriteLayerP_001c4188);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1bd);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gTrapDoorSpriteLayerP_001c40e0);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1bf);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gOpenTrapDoorSpriteLayerP_001c47dc);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1c1);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1c3);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gOpenedDoorSpriteLayerP_001c49a4);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1c5);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gBloodDripSpriteLayerP_001c4218);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1c7);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gObjectSpriteLayerP_001c4268);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1c9);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gPuffOfSmokeSpriteLayerP_001c40ac);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1cb);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gBottomCircleOfLightSpriteLayerP_001c4290);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1cd);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1cf);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gKeyGlowSpriteLayerP_001c47d8);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1d1);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gQuestionMarkSpriteLayerP_001c4938);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1d3);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gTopCircleOfLightSpriteLayerP_001c4014);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1d5);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1d7);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gZapperGlowSpriteLayerP_001c4748);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1d9);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gZapperNodeSpriteLayerP_001c418c);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1db);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gZapperBeamSpriteLayerP_001c41ec);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1dd);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gRainSnowSpriteLayerP_001c49ac);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1df);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gHiddenTriggerSpriteLayerP_001c48b4);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1e1);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gMiscTriggerSpriteLayerP_001c480c);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1e3);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gSecretIndicatorSpriteLayerP_001c40d8);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1e5);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1e7);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gSignContentsSpriteLayerP_001c490c);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1e9);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gTipContentsSpriteLayerP_001c46f8);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1eb);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gFPSDisplaySpriteLayerP_001c41a8);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1ed);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gPausedSpriteLayerP_001c47f4);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1ef);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gCustomDialogSpriteLayerP_001c494c);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1f1);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gOptionsDialogPane1LayerP_001c4970);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,499);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gOptionsDialogPane2LayerP_001c4838);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1f5);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gTextSpriteLayerP_001c4804);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1f7);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gCustomCursorSpriteLayerP_001c49f4);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1f9);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gHaloSpriteLayerP_001c41e0);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1fb);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gHiddenTriggerItemsSpriteLayerP_001c4864);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1fd);
  }
  SVar1 = _SWCreateSpriteLayer((SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___89633_001c4940,0x1ff);
  }
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gBackgroundScreenSpriteLayerP_001c4780);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gRearBackgroundPictureSpriteLayerP_001c4980);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gBackgroundPictureSpriteLayerP_001c475c);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gShadowSpriteLayerP_001c4978);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gLowerTileFringeSpriteLayerP_001c48c0);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gBottomSpriteLayerP_001c4140);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gExitSpriteLayerP_001c403c);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gBackgroundSpriteLayerP_001c4070);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gGlowSpriteLayerP_001c42a0);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gLanternGlowSpriteLayerP_001c4214);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gTorchSpriteLayerP_001c4144);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gCandleHolderSpriteLayerP_001c417c);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gCandleSpriteLayerP_001c421c);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gFlameSpriteLayerP_001c4130);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gTrapDoorSpriteLayerP_001c40e0);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gOpenTrapDoorSpriteLayerP_001c47dc);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gDisappearingBlockSpriteLayerP_001c4930);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gVertMovingPlatformSpriteLayerP_001c423c);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gFallingPlatformSpriteLayerP_001c499c);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gMovingPlatformBlockerSpriteLayerP_001c41ac);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gHeroOnPlatformSpriteLayerP_001c491c);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gFloorCandleStandSpriteLayerP_001c4808);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gSignSpriteLayerP_001c41f8);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gLeverSpriteLayerP_001c41d4);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gForegroundSpriteLayerP_001c41f0);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gSavePedestalSpriteLayerP_001c4138);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gBottomCircleOfLightSpriteLayerP_001c4290);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gStatueSpriteLayerP_001c4240);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gTopCircleOfLightSpriteLayerP_001c4014);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gBrickSwitchSpriteLayerP_001c4188);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gBackLavaSplashSpriteLayerP_001c41cc);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gRainSnowSpriteLayerP_001c49ac);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gOpenedDoorSpriteLayerP_001c49a4);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gBloodDripSpriteLayerP_001c4218);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gSnakeSpriteLayerP_001c4208);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gWebTrappedSpriteLayerP_001c4150);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gSkeletonBonesSpriteLayerP_001c40a0);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gEnemySpriteLayerP_001c40b0);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gLadybugSpriteLayerP_001c4844);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gSpyVsSpySpriteLayerP_001c4874);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gEyeballSpriteLayerP_001c41c8);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gBirdSpriteLayerP_001c41c4);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gBigSpiderSpriteLayerP_001c42a4);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gKeyGlowSpriteLayerP_001c47d8);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gObjectSpriteLayerP_001c4268);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gPuffOfSmokeSpriteLayerP_001c40ac);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gZapperGlowSpriteLayerP_001c4748);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gZapperBeamSpriteLayerP_001c41ec);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gZapperNodeSpriteLayerP_001c418c);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gMedusaSpriteLayerP_001c4094);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gDyingMedusaSpriteLayerP_001c49a8);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gGhostSpriteLayerP_001c496c);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gHaloSpriteLayerP_001c41e0);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gLavaGlowSpriteLayerP_001c49d4);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gQuestionMarkSpriteLayerP_001c4938);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gFrontLavaSplashSpriteLayerP_001c4024);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gHiddenTriggerSpriteLayerP_001c48b4);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gMiscTriggerSpriteLayerP_001c480c);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gSecretIndicatorSpriteLayerP_001c40d8);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gTipContentsSpriteLayerP_001c46f8);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gSignContentsSpriteLayerP_001c490c);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gFPSDisplaySpriteLayerP_001c41a8);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gPausedSpriteLayerP_001c47f4);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gCustomDialogSpriteLayerP_001c494c);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gOptionsDialogPane1LayerP_001c4970);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gOptionsDialogPane2LayerP_001c4838);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gTextSpriteLayerP_001c4804);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gCustomCursorSpriteLayerP_001c49f4);
  _SWAddSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                    *(SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4);
  return;
}


// ---- _CreateSpriteWorld @ 0x2f330 (599 bytes)

void _CreateSpriteWorld(void)

{
  undefined4 uVar1;
  SWError SVar2;
  undefined1 local_24 [4];
  undefined4 local_20;
  undefined4 local_1c;
  SWError local_10;
  
  local_10 = _SWEnterSpriteWorld('\x01');
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90012_001c4a0c,0x265);
  }
  *(undefined2 *)PTR__gPrevWindowWidth_001c4840 = *(undefined2 *)PTR__gWindowedResolutionX_001c4880;
  *(undefined2 *)PTR__gPrevWindowHeight_001c49b4 = *(undefined2 *)PTR__gWindowedResolutionY_001c4854
  ;
  *PTR__gFullScreenWindow_001c48ec = *PTR__gFullScreenWindowPrefs_001c4920;
  if (*PTR__gFullScreenWindow_001c48ec != '\0') {
    _SDL_GetCurrentDisplayMode(0,local_24);
    *(short *)PTR__gPrevWindowWidth_001c4840 = (short)local_20;
    *(short *)PTR__gPrevWindowHeight_001c49b4 = (short)local_1c;
  }
  local_10 = _SWCreateSpriteWorld
                       (PTR__gSpriteWorldP_001c40d0,"Midnight Mansion",0x280,0x1e0,
                        (int)*(short *)PTR__gPrevWindowWidth_001c4840,
                        (int)*(short *)PTR__gPrevWindowHeight_001c49b4,0x20,
                        *PTR__gFullScreenWindow_001c48ec,1);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90012_001c4a0c,0x284);
  }
  _SWSetSpriteWorldStretchToFit
            (*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,*PTR__gStretchToFit_001c49dc);
  uVar1 = _SDL_GetWindowDisplay(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158));
  _SDL_GetCurrentDisplayMode(uVar1,local_24);
  _printf("\nCurrent display mode width = %d, height = %d \n",local_20,local_1c);
  *(undefined **)PTR__gSpriteDrawProc_001c47ec = PTR__SWStdSpriteDrawProc_001c45dc;
  *(undefined **)PTR__gTranslucentMaskDrawProc_001c411c = PTR__SWStdSpriteDrawProc_001c45dc;
  _SWSetSpriteWorldMaxFPS(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x3c);
  SVar2 = _SWStickyError();
  if (SVar2 != 0) {
    SVar2 = _SWStickyError();
    _MMFatalErrorCallback
              (SVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90012_001c4a0c,0x2a3);
  }
  return;
}


// ---- _ChangeWorldRectForGame @ 0x2f587 (111 bytes)

void _ChangeWorldRectForGame(int param_1,short param_2)

{
  short local_14;
  undefined2 uStack_12;
  undefined4 local_10;
  
  if (*PTR__worldRectIsSetForGame_001c493c == '\0') {
    local_10 = *(undefined4 *)(param_1 + 0x78);
    local_14 = (short)*(undefined4 *)(param_1 + 0x74);
    _local_14 = CONCAT22((short)((uint)*(undefined4 *)(param_1 + 0x74) >> 0x10),local_14 + param_2);
    _SWChangeWorldRect(param_1,&local_14,0);
    *PTR__worldRectIsSetForGame_001c493c = 1;
  }
  return;
}


// ---- _IsStatsAreaVisible @ 0x2f5f6 (34 bytes)

undefined1 _IsStatsAreaVisible(void)

{
  return *PTR__worldRectIsSetForGame_001c493c;
}


// ---- _MyRestoreWorldRect @ 0x2f618 (62 bytes)

void _MyRestoreWorldRect(undefined4 param_1)

{
  if (*PTR__worldRectIsSetForGame_001c493c != '\0') {
    _SWRestoreWorldRect(param_1);
    *PTR__worldRectIsSetForGame_001c493c = 0;
  }
  return;
}


// ---- _SetUpTiling @ 0x2f656 (348 bytes)

void _SetUpTiling(void)

{
  SWError SVar1;
  
  _SetUpLadderArray();
  SVar1 = _SWInitTiling(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x10,0x10,0xf7a);
  if (SVar1 != 0) {
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90078_001c48c4,0x304);
  }
  _SWSetTileResolution(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x20,0x20);
  _SWInstallTileLayerAboveSpriteLayer
            (0,*(SpriteLayerPtr *)PTR__gBackgroundPictureSpriteLayerP_001c475c);
  _SWInstallTileLayerBelowSpriteLayer
            (1,*(SpriteLayerPtr *)PTR__gLowerTileFringeSpriteLayerP_001c48c0);
  _SWInstallTileLayerAboveSpriteLayer
            (2,*(SpriteLayerPtr *)PTR__gLowerTileFringeSpriteLayerP_001c48c0);
  _SWInstallTileLayerAboveSpriteLayer
            (3,*(SpriteLayerPtr *)PTR__gDisappearingBlockSpriteLayerP_001c4930);
  _SWInstallTileLayerAboveSpriteLayer
            (4,*(SpriteLayerPtr *)PTR__gMovingPlatformBlockerSpriteLayerP_001c41ac);
  _SWInstallTileLayerAboveSpriteLayer
            (5,*(SpriteLayerPtr *)PTR__gHeroOnPlatformSpriteLayerP_001c491c);
  _SWInstallTileLayerAboveSpriteLayer(6,*(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164);
  _LoadTiles();
  _SetUpTileColorVariations();
  return;
}


// ---- _SetUpTileColorVariations @ 0x2f7b2 (1467 bytes)

void _SetUpTileColorVariations(void)

{
  uint uVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  longdouble lVar9;
  longdouble lVar10;
  longdouble lVar11;
  ushort local_3e;
  short local_38;
  short local_36;
  
  for (local_38 = 0; local_38 < 0x5a2; local_38 = local_38 + 1) {
    *(short *)(PTR__gOrigTileIDs_001c47c8 + local_38 * 2) = local_38;
  }
  for (local_3e = 0; (short)local_3e < 0xc; local_3e = local_3e + 1) {
    sVar2 = local_3e * 0xd2;
    sVar3 = sVar2 + 0x5a2;
    sVar4 = sVar2 + 0x673;
    _SWCloneTiles(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)sVar3,(int)sVar4,1);
    local_36 = 1;
    for (local_38 = sVar3; local_38 <= sVar4; local_38 = local_38 + 1) {
      *(short *)(PTR__gOrigTileIDs_001c47c8 + local_38 * 2) = local_36;
      local_36 = local_36 + 1;
    }
    iVar8 = (int)(short)local_3e;
    iVar7 = ((iVar8 * 0xffff) / 0x18 + iVar8 / 0x8001) - iVar8 / 0x8001;
    iVar8 = (int)(short)local_3e;
    iVar5 = (int)((double)(((iVar8 * 0xffff) / 0x30 + iVar8 / 0x8001) - iVar8 / 0x8001) + 43690.0);
    iVar8 = iVar5 * 3 + iVar7 + 0x7fff;
    if (iVar8 < 0) {
      iVar8 = iVar8 + 3;
    }
    uVar6 = iVar8 >> 2;
    uVar1 = (iVar7 + 0x27ffd) / 3;
    if (((local_3e ^ 1) & 1) == 0) {
      _SWSetTileColorForRange
                (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)sVar3,(int)sVar4,uVar6 & 0xffff,
                 uVar6 & 0xffff,uVar6 & 0xffff,0xffff);
    }
    else {
      iVar8 = (int)((short)local_3e / 2);
      if (5 < iVar8) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c"
                   ,PTR____FUNCTION___90113_001c479c,0x358);
      }
      _SWSetTileColorForRange
                (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)sVar3,(int)sVar4,
                 (int)(*(float *)(PTR__tileColorVars_001c4770 + iVar8 * 0xc) *
                      (float)(uVar6 & 0xffff)) & 0xffff,
                 (int)(*(float *)(PTR__tileColorVars_001c4770 + iVar8 * 0xc + 4) *
                      (float)(uVar6 & 0xffff)) & 0xffff,
                 (int)(*(float *)(PTR__tileColorVars_001c4770 + iVar8 * 0xc + 8) *
                      (float)(uVar6 & 0xffff)) & 0xffff,0xffff);
    }
    lVar9 = (longdouble)_GetRandomFloat(0x3f6e147b,0x3f800000);
    lVar10 = (longdouble)_GetRandomFloat(0x3f6e147b,0x3f800000);
    lVar11 = (longdouble)_GetRandomFloat(0x3f6e147b,0x3f800000);
    _SWSetTileColorForRange
              (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)(short)(sVar2 + 0x666),
               (int)(short)(sVar2 + 0x673),(int)((float)iVar5 * (float)lVar11) & 0xffff,
               (int)((float)iVar5 * (float)lVar10) & 0xffff,
               (int)((float)iVar5 * (float)lVar9) & 0xffff,0xffff);
    _SWSetTileColorForRange
              (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)(short)(sVar2 + 0x5cc),
               (int)(short)(sVar2 + 0x5e7),uVar6 & 0xffff,uVar6 & 0xffff,uVar6 & 0xffff,0xffff);
    _SWSetTileColorForRange
              (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)(short)(sVar2 + 0x612),
               (int)(short)(sVar2 + 0x62d),uVar6 & 0xffff,uVar6 & 0xffff,uVar6 & 0xffff,0xffff);
    _SWSetTileColorForRange
              (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)(short)(sVar2 + 0x62e),
               (int)(short)(sVar2 + 0x657),uVar1 & 0xffff,uVar1 & 0xffff,uVar1 & 0xffff,0xffff);
    _SWSetTileColorForRange
              (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)(short)(sVar2 + 0x658),
               (int)(short)(sVar2 + 0x665),0xffff,0xffff,0xffff,0xffff);
  }
  _SWSetTileColorForRange
            (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xd3,0x18a,0xd998,0xd998,0xd998,0xffff);
  _SetUpFringeArray();
  _SetUpTileFamilyArray();
  return;
}


// ---- _LoadTiles @ 0x2fd6d (4478 bytes)

void _LoadTiles(void)

{
  undefined4 uVar1;
  int iVar2;
  short local_2a;
  short local_28;
  
  if (*(int *)PTR__gSpriteWorldP_001c40d0 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x397);
  }
  *PTR__gWereAnyTilesReplaced_001c4850 = 0;
  uVar1 = *(undefined4 *)PTR__gCurrentGameMode_001c42d0;
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
  _SWSetTransparentColor(PTR__gSDLTealColor_001c41a0);
  iVar2 = _MyLoadTilesFromPict(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0,0,200,0xc9,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x3c0);
  }
  local_28 = 1;
  for (local_2a = 0xcd; local_2a < 0xd7; local_2a = local_2a + 1) {
    iVar2 = _MyLoadTilesFromPict
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_28,
                       (int)(short)(local_28 + 0xd),(int)local_2a,(int)local_2a,4,0,0,0);
    if (iVar2 != 0) {
      _MMFatalErrorCallback
                (iVar2,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
                 PTR____FUNCTION___90246_001c47b8,0x3d0);
    }
    _IncreaseProgressBar(1);
    local_28 = local_28 + 0xe;
  }
  local_28 = 0x8d;
  for (local_2a = 0xdc; local_2a < 0xe0; local_2a = local_2a + 1) {
    iVar2 = _MyLoadTilesFromPict
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_28,
                       (int)(short)(local_28 + 0xd),(int)local_2a,(int)local_2a,4,0,0,0);
    if (iVar2 != 0) {
      _MMFatalErrorCallback
                (iVar2,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
                 PTR____FUNCTION___90246_001c47b8,0x3e4);
    }
    _IncreaseProgressBar(1);
    local_28 = local_28 + 0xe;
  }
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xc5,0xd2,0xe2,0xe2,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x3f4);
  }
  _IncreaseProgressBar(1);
  local_28 = 0xd3;
  for (local_2a = 0x15e; local_2a < 0x161; local_2a = local_2a + 1) {
    iVar2 = _MyLoadTilesFromPict
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_28,
                       (int)(short)(local_28 + 0x23),(int)local_2a,(int)local_2a,4,0,0,0);
    if (iVar2 != 0) {
      _MMFatalErrorCallback
                (iVar2,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
                 PTR____FUNCTION___90246_001c47b8,0x407);
    }
    _IncreaseProgressBar(1);
    local_28 = local_28 + 0x24;
  }
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x13f,0x15a,0x168,0x168,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x417);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x15b,0x18a,0x16d,0x16d,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x426);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x18b,0x1a2,0x172,0x172,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x435);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x1a3,0x1ae,0x173,0x173,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x444);
  }
  _IncreaseProgressBar(1);
  local_28 = 0x1af;
  for (local_2a = 0xe6; local_2a < 0xea; local_2a = local_2a + 1) {
    iVar2 = _MyLoadTilesFromPict
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_28,
                       (int)(short)(local_28 + 0xd),(int)local_2a,(int)local_2a,4,0,0,0);
    if (iVar2 != 0) {
      _MMFatalErrorCallback
                (iVar2,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
                 PTR____FUNCTION___90246_001c47b8,0x457);
    }
    _IncreaseProgressBar(1);
    local_28 = local_28 + 0xe;
  }
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x1f9,0x20a,0xf5,0xf5,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x467);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x1e7,0x1f8,0xf0,0xf0,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x476);
  }
  _IncreaseProgressBar(1);
  local_28 = 0x20b;
  for (local_2a = 0xfa; local_2a < 0xfd; local_2a = local_2a + 1) {
    iVar2 = _MyLoadTilesFromPict
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_28,
                       (int)(short)(local_28 + 0x11),(int)local_2a,(int)local_2a,4,0,0,0);
    if (iVar2 != 0) {
      _MMFatalErrorCallback
                (iVar2,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
                 PTR____FUNCTION___90246_001c47b8,0x489);
    }
    _IncreaseProgressBar(1);
    local_28 = local_28 + 0x12;
  }
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x241,0x2c0,300,300,8,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x499);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x2c1,0x2e8,400,400,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x4a8);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x2e9,0x308,500,500,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x4b7);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x309,0x318,0x1fe,0x1fe,0x40,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x4c6);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x319,800,0x204,0x206,0x10,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x4d9);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x321,0x328,0x205,0x207,0x10,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x4ec);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x329,0x330,0x1ff,0x208,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x4fb);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x331,0x3b4,0x226,0x226,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x50a);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x3b5,0x4ae,0x227,0x227,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x519);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x4af,0x51e,0x230,0x230,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x528);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x51f,0x53e,0x231,0x231,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x537);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x53f,0x57a,0x22b,0x22b,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x546);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x57b,0x598,0x22c,0x22c,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x555);
  }
  _IncreaseProgressBar(1);
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x599,0x59a,0x235,0x235,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x564);
  }
  iVar2 = _MyLoadTilesFromPict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x59b,0x5a1,0x23a,0x23a,4,0,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90246_001c47b8,0x571);
  }
  _SWSetTransparentColor(PTR__gSDLWhiteColor_001c42a8);
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = uVar1;
  if (*PTR__gWereAnyTilesReplaced_001c4850 != '\0') {
    _SetUpTileColorVariations();
  }
  return;
}


// ---- _MyLoadTilesFromPict @ 0x30eeb (638 bytes)

undefined4 _MyLoadTilesFromPict(undefined4 param_1,short param_2,short param_3,short param_4)

{
  char cVar1;
  undefined4 uVar2;
  short in_stack_00000020;
  short in_stack_00000024;
  char local_222 [500];
  char local_2e;
  char local_2d;
  undefined4 local_2c;
  short local_26;
  int local_24;
  char *local_20;
  
  local_2e = 0;
  local_2c = 0;
  if (*PTR__tileFlagsWereInitialized_001c4900 == '\0') {
    for (local_24 = 0; local_24 < 0x5a2; local_24 = local_24 + 1) {
      *(undefined2 *)(PTR__tileFlags_001c48d8 + local_24 * 2) = 0xffff;
    }
    *PTR__tileFlagsWereInitialized_001c4900 = 1;
  }
  local_2d = '\x01';
  if (*(short *)(PTR__tileFlags_001c48d8 + param_2 * 2) == -1) {
    local_2e = '\x01';
    *(undefined2 *)(PTR__tileFlags_001c48d8 + param_2 * 2) = 0;
  }
  else {
    cVar1 = _DoesCustomPICTResourceExist(PTR__gCurOpenMansionFilePath_001c4124,(int)param_4);
    if (cVar1 == '\0') {
      if (*(short *)(PTR__tileFlags_001c48d8 + param_2 * 2) == 1) {
        local_2e = '\x01';
        *(undefined2 *)(PTR__tileFlags_001c48d8 + param_2 * 2) = 0;
        local_2d = '\x01';
        *PTR__gWereAnyTilesReplaced_001c4850 = 1;
      }
      else {
        local_2e = '\0';
      }
    }
    else {
      _sprintf(local_222,"%s\'%s\'/%05d.png",PTR__gCurOpenMansionFilePath_001c4124,"PICT",
               (int)param_4);
      local_2e = '\x01';
      *(undefined2 *)(PTR__tileFlags_001c48d8 + param_2 * 2) = 1;
      local_2d = '\0';
      *PTR__gWereAnyTilesReplaced_001c4850 = 1;
    }
  }
  if (local_2e != '\0') {
    if (local_2d == '\0') {
      local_20 = local_222;
    }
    else {
      uVar2 = _GetSpriteImagePath(0);
      local_20 = (char *)_GetFileNameForResource(uVar2,(int)param_4);
    }
    local_26 = (param_3 - param_2) + 1;
    local_2c = _SWLoadTilesFromSingleFileXY
                         (param_1,local_20,(int)in_stack_00000020,(int)in_stack_00000024,0,
                          (int)local_26,(int)param_2);
  }
  return local_2c;
}


// ---- _StartUpGameCarbon @ 0x31169 (697 bytes)

void _StartUpGameCarbon(void)

{
  char cVar1;
  undefined4 uVar2;
  undefined2 local_12;
  
  *PTR__gUsingEditor_001c48b0 = 0;
  _LoadHighScores();
  _GiveOSTime();
  _SetUpMenuBar();
  _CreateSpriteWorld();
  _CreateAndAddSpriteLayers();
  _initMMMenuBar();
  _CreateStatsArea();
  if (*PTR__gFullScreenWindow_001c48ec != '\0') {
    _MMHideCursor();
  }
  _MMFlushEvents();
  _InitMusic();
  _StartOpeningMusic(1);
  _SetUpActionSoftScreen();
  uVar2 = _GetSoundsFolderPath();
  _LoadAndSetupSounds(0x10,7,0x4f,uVar2);
  uVar2 = _GetCurrentMusicReference();
  *(undefined4 *)PTR__gOpeningMusicRefNum_001c471c = uVar2;
  _StartMicrosecTimer();
  _DoGameFadeToNormal(0);
  _DoActionSoftScreen();
  _WaitMicrosecondsOrMouseClick(0x9c4);
  cVar1 = _WasGameQuit();
  if (cVar1 == '\0') {
    _DoGameFadeToBlack(0x1e,0);
    _MMDisposeAllSpritesInSpriteWorld(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    cVar1 = _WasGameQuit();
    if (cVar1 == '\0') {
      _MMFlushEvents();
      uVar2 = _GetCurrentMusicReference();
      *(undefined4 *)PTR__gOpeningMusicRefNum_001c471c = uVar2;
      _DisplaySplashScreen();
      _DoGameFadeToNormal(0x1e);
      cVar1 = _WasGameQuit();
      if (cVar1 == '\0') {
        _SWSetCleanUpSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
        _ChangeSplashScreenLoadingText(0);
        _SetUpTiling();
        cVar1 = _WasGameQuit();
        if (cVar1 == '\0') {
          _ChangeSplashScreenLoadingText(1);
          _LoadSprites();
          _SWSetAssertionFailureCallback(PTR_LAB_001c4944);
          _printf("Finished loading sprites");
          cVar1 = _WasGameQuit();
          if (cVar1 == '\0') {
            if (*PTR__gMusicIsOn_001c47c0 == '\0') {
              local_12 = 6000;
            }
            else {
              local_12 = 0x2e18;
            }
            _ChangeSplashScreenLoadingText(2);
            _WaitMicrosecondsOrMouseClick(local_12);
            _DoGameFadeToBlack(0x1e,0);
            cVar1 = _WasGameQuit();
            if (cVar1 == '\0') {
              _MMFlushEvents();
              _InitGamePad();
              _printf("About to show title screen.");
              _SetUpTitleScreen();
              _UpdateMenuBar();
              if (*PTR__gShowEpisode2LinkedMessage_001c4a00 == '\0') {
                if (*PTR__gShowEpisode1LinkedMessage_001c49e8 != '\0') {
                  _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                                    "Midnight Mansion HD Episode 1 was detected on your computer. \r\rBoth episodes have been linked. You can now launch either copy to play the full game."
                                    ,3);
                  _puts("Showing message 1");
                }
              }
              else {
                _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                                  "Midnight Mansion HD Episode 2 was detected on your computer. \r\rBoth episodes have been linked.  You can now launch either copy to play the full game."
                                  ,3);
                _puts("Showing message 2");
              }
              _RunMainLoop();
            }
          }
        }
      }
    }
  }
  return;
}


// ---- _IsGamepadInitialized @ 0x31422 (34 bytes)

undefined1 _IsGamepadInitialized(void)

{
  return *PTR__gamepadInitialized_001c4810;
}


// ---- _InitGamePad @ 0x31444 (235 bytes)

void _InitGamePad(void)

{
  undefined4 uVar1;
  int iVar2;
  
  if ((*PTR__gUseGamePad_001c4740 != '\0') && (*PTR__gamepadInitialized_001c4810 == '\0')) {
    if ((*PTR__initedSubsystem_001c477c != '\0') && (*PTR__gamepadInitialized_001c4810 == '\0')) {
      _SDL_QuitSubSystem(0x200);
    }
    _puts("\nIniting SDL_INIT_JOYSTICK subsystem.");
    _SDL_InitSubSystem(0x200);
    *PTR__initedSubsystem_001c477c = 1;
    _puts("Success. Checking number of joysticks attached... ");
    uVar1 = _SDL_NumJoysticks();
    _printf("Num joysticks found: %d \n",uVar1);
    if (*PTR__gamepadInitialized_001c4810 == '\0') {
      iVar2 = _SDL_NumJoysticks();
      if (0 < iVar2) {
        iVar2 = _SDL_JoystickOpen(0);
        if (iVar2 != 0) {
          *PTR__gamepadInitialized_001c4810 = 1;
        }
      }
    }
  }
  return;
}


// ---- _RunMainLoop @ 0x3152f (464 bytes)

void _RunMainLoop(void)

{
  _puts("\n\nEntering RunMainLoop\n");
  _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
  do {
    _GiveOSTime();
    _ProcessSDLEvents();
    if (*PTR__gMMIsActive_001c49e0 != '\0') {
      if (*PTR__gGameIsPaused_001c484c == '\0') {
        _MMUpdateTimeSpentInMansion();
        if (*(int *)PTR__gCurrentGameMode_001c42d0 == 5) {
          _RunAnimationCore();
        }
        else if (*(int *)PTR__gCurrentGameMode_001c42d0 == 6) {
          _RunInGameMapScreen();
        }
        if (*(int *)PTR__gCurrentGameMode_001c42d0 == 2) {
          _HandleSelectMansionScreenCallback();
        }
        else if (*(int *)PTR__gCurrentGameMode_001c42d0 == 0xe) {
          _HandleReadingSignCallback();
        }
        else if (*(int *)PTR__gCurrentGameMode_001c42d0 == 1) {
          _HandleDifficultyScreenCallback();
        }
        else if (*(int *)PTR__gCurrentGameMode_001c42d0 == 0) {
          _HandleTitleScreenCallback();
        }
        else if (*(int *)PTR__gCurrentGameMode_001c42d0 == 7) {
          _HandleGameOverScreenCallback();
        }
        else if (*(int *)PTR__gCurrentGameMode_001c42d0 == 3) {
          _HandleHelpScreenCallback();
        }
        else if (*(int *)PTR__gCurrentGameMode_001c42d0 == 9) {
          _HandleBeatTheMansionScreenCallback();
        }
        else if (*(int *)PTR__gCurrentGameMode_001c42d0 == 10) {
          _HandleBeatTheGameCallback();
        }
        else if (*(int *)PTR__gCurrentGameMode_001c42d0 == 4) {
          _HandleHighScoresScreenCallback();
        }
        else if (*(int *)PTR__gCurrentGameMode_001c42d0 == 0xb) {
          _HandleOrganScreenCallback();
        }
        else if (*(int *)PTR__gCurrentGameMode_001c42d0 == 0xd) {
          _HandleTeaserScreenCallback();
        }
        else if (*(int *)PTR__gCurrentGameMode_001c42d0 == 8) {
          _HandleAboutScreenCallback();
        }
      }
      else {
        _HandlePausedScreenCallback();
      }
    }
  } while (*PTR__gQuitGame_001c49d0 == '\0');
  return;
}


// ---- _InitializePreferences @ 0x316ff (615 bytes)

void _InitializePreferences(void)

{
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0xc) = 0x5c;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 4) = 0x5e;
  *(undefined4 *)PTR__gKeyPrefs_001c46f4 = 0x60;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 8) = 0x5a;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x10) = 0x62;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x1c) = 0x28;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x14) = 0x5f;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x18) = 0x61;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x20) = 0x10;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x30) = 0x4000005c;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x28) = 0x4000005e;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x24) = 0x40000060;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x2c) = 0x4000005a;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x34) = 0x20;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x40) = 0xd;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x38) = 0x4000005f;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x3c) = 0x40000061;
  *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x44) = 0x6d;
  PTR__gKeyPrefs_001c46f4[0x48] = 0;
  PTR__gKeyPrefs_001c46f4[0x7a] = 0;
  PTR__gKeyPrefs_001c46f4[0xac] = 0;
  PTR__gKeyPrefs_001c46f4[0xde] = 0;
  PTR__gKeyPrefs_001c46f4[0x110] = 0;
  PTR__gKeyPrefs_001c46f4[0x142] = 0;
  PTR__gKeyPrefs_001c46f4[0x174] = 0;
  PTR__gKeyPrefs_001c46f4[0x1a6] = 0;
  PTR__gKeyPrefs_001c46f4[0x1d8] = 0;
  *PTR__gArrowKeysEnabled_001c48a0 = 1;
  *PTR__gMusicIsOn_001c47c0 = 1;
  *PTR__gSofterMusic_001c4768 = 0;
  *(undefined2 *)PTR__gTitleMusicVolume_001c48bc = 0xff;
  *(undefined2 *)PTR__gBackgroundMusicVolume_001c4704 = 0xdc;
  *PTR__gSoundIsTurnedOn_001c47fc = 1;
  *(undefined2 *)PTR__gSoundVolume_001c48dc = 0xff;
  *PTR__gFullScreenWindowPrefs_001c4920 = 0;
  *PTR__gStretchToFit_001c49dc = 0;
  *PTR__gUseGamePad_001c4740 = 0;
  *(undefined2 *)PTR__gWindowedResolutionX_001c4880 = 0x400;
  *(undefined2 *)PTR__gWindowedResolutionY_001c4854 = 0x300;
  *PTR__gLimitFullscreenResolution_001c4720 = 0;
  *PTR__gEasyJumping_001c4994 = 0;
  *PTR__gRecordMovies_001c48f8 = 1;
  *PTR__gFamilyFriendly_001c46f0 = 1;
  *PTR__gCurrentPlayerNameExists_001c4830 = 0;
  return;
}


// ---- _LoadPreferences @ 0x31966 (164 bytes)

void _LoadPreferences(void)

{
  int iVar1;
  
  _InitializePreferences();
  iVar1 = _ReadPreferencesFile();
  if (iVar1 == -1) {
    _InitializePreferences();
    _WritePreferencesFile();
  }
  if (*PTR__gCurrentPlayerNameExists_001c4830 != '\0') {
    iVar1 = _LoadProfileFromDisk(PTR__gCurrentPlayerName_001c48e8);
    if (iVar1 != 0) {
      *PTR__gCurrentPlayerNameExists_001c4830 = 0;
      if (iVar1 == -2) {
        _ErrorMessage("Sorry, this profile is in an old format. Midnight Mansion HD uses a new profile format that is incompatible with the old."
                      ,0);
      }
      else {
        _ErrorMessage("Your player profile has been corrupted or is missing. Please create or select a different player."
                      ,0);
      }
    }
  }
  return;
}


// ---- _WritePreferencesFile @ 0x31a0a (777 bytes)

undefined4 _WritePreferencesFile(void)

{
  undefined4 local_20;
  undefined4 local_1c;
  int local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  local_10 = 0;
  local_14 = _GetPrefsFilePath("Midnight Mansion HD Prefs");
  local_18 = _SDL_RWFromFile(local_14,"wb");
  if (local_18 == 0) {
    local_10 = 0xffffffff;
  }
  else {
    _ClearFileStickyError();
    local_1c = 0x6c;
    _MyWriteDataToFile(local_18,4,&local_1c);
    _MyWriteDataToFile(local_18,1,PTR__gCurrentPlayerNameExists_001c4830);
    _MyWriteDataToFile(local_18,0x100,PTR__gCurrentPlayerName_001c48e8);
    _MyWriteDataToFile(local_18,0x20c,PTR__gKeyPrefs_001c46f4);
    _MyWriteDataToFile(local_18,1,PTR__gArrowKeysEnabled_001c48a0);
    _MyWriteDataToFile(local_18,1,PTR__gMusicIsOn_001c47c0);
    _MyWriteDataToFile(local_18,1,PTR__gSofterMusic_001c4768);
    _MyWriteDataToFile(local_18,2,PTR__gTitleMusicVolume_001c48bc);
    _MyWriteDataToFile(local_18,2,PTR__gBackgroundMusicVolume_001c4704);
    _MyWriteDataToFile(local_18,1,PTR__gSoundIsTurnedOn_001c47fc);
    _MyWriteDataToFile(local_18,2,PTR__gSoundVolume_001c48dc);
    _MyWriteDataToFile(local_18,1,PTR__gEasyJumping_001c4994);
    _MyWriteDataToFile(local_18,1,PTR__gRecordMovies_001c48f8);
    _MyWriteDataToFile(local_18,1,PTR__gFamilyFriendly_001c46f0);
    _MyWriteDataToFile(local_18,1,PTR__gFullScreenWindowPrefs_001c4920);
    _MyWriteDataToFile(local_18,1,PTR__gStretchToFit_001c49dc);
    _MyWriteDataToFile(local_18,1,PTR__gUseGamePad_001c4740);
    _MyWriteDataToFile(local_18,1,PTR__gLimitFullscreenResolution_001c4720);
    _MyWriteDataToFile(local_18,2,PTR__gWindowedResolutionX_001c4880);
    _MyWriteDataToFile(local_18,2,PTR__gWindowedResolutionY_001c4854);
    local_20 = 0x71f7;
    _MyWriteDataToFile(local_18,4,&local_20);
    local_10 = _GetFileStickyError();
    (**(code **)(local_18 + 0xc))(local_18);
  }
  return local_10;
}


// ---- _ReadPreferencesFile @ 0x31d13 (890 bytes)

undefined4 _ReadPreferencesFile(void)

{
  undefined1 local_21;
  int local_20;
  int local_1c;
  int local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  local_10 = 0;
  local_14 = _GetPrefsFilePath("Midnight Mansion HD Prefs");
  local_18 = _SDL_RWFromFile(local_14,"rb");
  if (local_18 == 0) {
    local_10 = 0xffffffff;
  }
  else {
    _ClearFileStickyError();
    _MyReadDataFromFile(local_18,4,&local_1c);
    if (local_1c < 0x6a) {
      return 0xffffffff;
    }
    _MyReadDataFromFile(local_18,1,PTR__gCurrentPlayerNameExists_001c4830);
    _MyReadDataFromFile(local_18,0x100,PTR__gCurrentPlayerName_001c48e8);
    _MyReadDataFromFile(local_18,0x20c,PTR__gKeyPrefs_001c46f4);
    _MyReadDataFromFile(local_18,1,PTR__gArrowKeysEnabled_001c48a0);
    _MyReadDataFromFile(local_18,1,PTR__gMusicIsOn_001c47c0);
    _MyReadDataFromFile(local_18,1,PTR__gSofterMusic_001c4768);
    _MyReadDataFromFile(local_18,2,PTR__gTitleMusicVolume_001c48bc);
    _MyReadDataFromFile(local_18,2,PTR__gBackgroundMusicVolume_001c4704);
    _MyReadDataFromFile(local_18,1,PTR__gSoundIsTurnedOn_001c47fc);
    _MyReadDataFromFile(local_18,2,PTR__gSoundVolume_001c48dc);
    _MyReadDataFromFile(local_18,1,PTR__gEasyJumping_001c4994);
    _MyReadDataFromFile(local_18,1,PTR__gRecordMovies_001c48f8);
    _MyReadDataFromFile(local_18,1,PTR__gFamilyFriendly_001c46f0);
    _MyReadDataFromFile(local_18,1,PTR__gFullScreenWindowPrefs_001c4920);
    _MyReadDataFromFile(local_18,1,PTR__gStretchToFit_001c49dc);
    if (local_1c < 0x6b) {
      _MyReadDataFromFile(local_18,1,&local_21);
    }
    if (0x6a < local_1c) {
      _MyReadDataFromFile(local_18,1,PTR__gUseGamePad_001c4740);
    }
    if (0x6b < local_1c) {
      _MyReadDataFromFile(local_18,1,PTR__gLimitFullscreenResolution_001c4720);
    }
    _MyReadDataFromFile(local_18,2,PTR__gWindowedResolutionX_001c4880);
    _MyReadDataFromFile(local_18,2,PTR__gWindowedResolutionY_001c4854);
    _MyReadDataFromFile(local_18,4,&local_20);
    if (local_20 != 0x71f7) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
                 PTR____FUNCTION___90662_001c4758,0x849);
    }
    local_10 = _GetFileStickyError();
    (**(code **)(local_18 + 0xc))(local_18);
  }
  return local_10;
}


// ---- _CreateEpisodeDataFile @ 0x3208d (280 bytes)

undefined4 _CreateEpisodeDataFile(undefined4 param_1)

{
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  int local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  local_10 = 0;
  local_14 = _GetAppSupportPath(param_1);
  local_18 = _SDL_RWFromFile(local_14,"wb");
  if (local_18 == 0) {
    local_10 = 0xffffffff;
  }
  else {
    _ClearFileStickyError();
    local_1c = 100;
    _MyWriteDataToFile(local_18,4,&local_1c);
    _MyWriteDataToFile(local_18,4,&stack0x00000008);
    local_20 = _SDL_GetCPUCacheLineSize();
    local_24 = _SDL_GetCPUCount();
    local_28 = _getProcessorSpeed();
    _MyWriteDataToFile(local_18,4,&local_20);
    _MyWriteDataToFile(local_18,4,&local_24);
    _MyWriteDataToFile(local_18,4,&local_28);
    local_10 = _GetFileStickyError();
    (**(code **)(local_18 + 0xc))(local_18);
  }
  return local_10;
}


// ---- _ReadEpisodeDataFile @ 0x321a5 (320 bytes)

undefined4 _ReadEpisodeDataFile(undefined4 param_1)

{
  int iVar1;
  undefined4 local_30;
  int local_2c;
  int local_28;
  int local_24;
  undefined4 local_20;
  int local_1c;
  int local_18;
  undefined4 local_14;
  int local_10;
  
  local_10 = 0;
  local_14 = _GetAppSupportPath(param_1);
  local_18 = _SDL_RWFromFile(local_14,"rb");
  if (local_18 != 0) {
    _ClearFileStickyError();
    _MyReadDataFromFile(local_18,4,&local_1c);
    _MyReadDataFromFile(local_18,4,&local_20);
    _MyReadDataFromFile(local_18,4,&local_24);
    _MyReadDataFromFile(local_18,4,&local_28);
    _MyReadDataFromFile(local_18,4,&local_2c);
    local_10 = _GetFileStickyError();
    (**(code **)(local_18 + 0xc))(local_18);
  }
  if ((local_10 == 0) && (local_1c == 100)) {
    iVar1 = _SDL_GetCPUCacheLineSize();
    if (((iVar1 == local_24) && (iVar1 = _SDL_GetCPUCount(), iVar1 == local_28)) &&
       (iVar1 = _getProcessorSpeed(), iVar1 == local_2c)) {
      return local_20;
    }
    local_30 = 0xfffffffe;
  }
  else {
    local_30 = 0xffffffff;
  }
  return local_30;
}


// ---- _MyOpenStandardMansion @ 0x322e5 (534 bytes)

int _MyOpenStandardMansion(char *param_1,short param_2)

{
  char cVar1;
  byte bVar2;
  char local_408 [165];
  undefined1 local_363 [91];
  char local_308 [500];
  char local_114 [256];
  char *local_14;
  int local_10;
  
  local_10 = 0;
  *(short *)PTR__gCurrentMansionNumber_001c4824 = param_2;
  _SetCurMansionAsCustomMansion(0);
  _SWCopyPascalStringToCString(param_1,local_114);
  local_14 = (char *)_GetMansionFolderPath(local_114);
  _strcpy(local_308,local_14);
  if (((local_10 == 0) && (param_2 != -2)) &&
     (cVar1 = _DoesMansionPassChecksum(local_308,local_114), cVar1 == '\0')) {
    _memcpy(local_408,
            "It appears this mansion has been modified. You can still play it by using Open Mansion, but not as part of the game. To fix this, download a fresh copy of the game."
            ,0xa5);
    _memset(local_363,0,0x5b);
    _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,local_408,3);
    return -1;
  }
  if (local_10 == 0) {
    local_10 = _OpenMansionUsingFilePath(local_308,local_114);
  }
  if (local_10 == 0) {
    if ((param_2 == -2) && (bVar2 = _MMCountResources("TMAP"), 0x23 < bVar2)) {
      _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                        "This mansion has been modified. Please restore from an original copy.",3);
      return -2;
    }
    _WriteCStringToFile("Opened mansion:");
    _WriteCStringToFile(local_114);
    _WriteCarriageReturn(1);
  }
  else {
    _sprintf(local_408,
             "The mansion file \'%s\' could not be found. If this problem persists, try downloading the game again."
             ,local_114);
    _MMDoErrorMessage(local_408,0);
  }
  return local_10;
}


// ---- _OpenMansionUsingFilePath @ 0x324fb (458 bytes)

int _OpenMansionUsingFilePath(char *param_1,char *param_2)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  int local_20;
  int local_1c;
  
  local_1c = 0;
  bVar1 = false;
  _printf("Attempting to open mansion at path: %s\n",param_1);
  _printf("...with name: %s\n",param_2);
  iVar2 = _MMGetMansionVersion("VERS",0x80,4,param_1);
  if (iVar2 == -1) {
    local_20 = -1;
  }
  else {
    if (iVar2 < 0x7d1) {
      _printf("MANSION VERSION: %d \n",iVar2);
      if (0x34f < iVar2) {
        bVar1 = true;
        _printf("Mansion version = %d\n",iVar2);
        _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                          "This mansion requires Midnight Mansion 2. You can\'t open it with Midnight Mansion 1."
                          ,3);
      }
    }
    else {
      _printf("ERROR: Mansion version = %d. Ignoring since invalid.\n",iVar2);
    }
    if (!bVar1) {
      _strcpy(PTR__gCurOpenMansionFilePath_001c4124,param_1);
      _strcpy(PTR__gCurOpenMansionFileName_001c4894,param_2);
      _strcpy(PTR__gMansionFilename_001c489c,param_2);
      _ScanDirectoryForResIDs(param_1,"\'TMAP\'");
      local_1c = _LoadMansionNameFromGAMEResource(PTR__gMansionName_001c47ac);
      if (local_1c == 0) {
        _LoadRestOfGAMEResource();
      }
      iVar2 = _SDL_GetTicks();
      _ScanDirectoryForPICTResIDs(PTR__gCurOpenMansionFilePath_001c4124);
      _LoadTiles();
      _LoadSprites();
      iVar3 = _SDL_GetTicks();
      _printf("Total ticks spent doing custom sprite/tile replacement/search: %d \n",iVar3 - iVar2);
    }
    local_20 = local_1c;
  }
  return local_20;
}


// ---- _MMOpenCustomMansionUsingPathAndName @ 0x326c5 (214 bytes)

int _MMOpenCustomMansionUsingPathAndName(undefined4 param_1,undefined4 param_2)

{
  char cVar1;
  short sVar2;
  int local_14;
  
  cVar1 = _IsMMRegistered(2);
  if (cVar1 == '\0') {
    sVar2 = _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                              "You can\'t play custom mansions until you register. Would you like to register now?"
                              ,2);
    if (sVar2 == 2) {
      _LaunchRegistrationWebsite();
    }
    local_14 = -1;
  }
  else {
    *PTR__gDifficultySetting_001c4870 = 1;
    _SetCurMansionAsCustomMansion(1);
    local_14 = _OpenMansionUsingFilePath(param_1,param_2);
    if (local_14 == -5) {
      _ErrorMessage("This mansion requires \'Midnight Mansion 2: The Haunted Hills\'. You can\'t open it with Midnight Mansion 1."
                    ,0);
    }
    else if (local_14 == -1) {
      _ErrorMessage("The mansion file couldn\'t be opened. Please make sure it is a HD format mansion."
                    ,0);
    }
  }
  return local_14;
}


// ---- _SetCurMansionAsCustomMansion @ 0x3279b (91 bytes)

void _SetCurMansionAsCustomMansion(char param_1)

{
  if (param_1 == '\0') {
    *PTR__gMansionIsCustomMansion_001c48ac = 0;
    *PTR__gCanUseLadderJump_001c4828 = 0;
  }
  else {
    *PTR__gMansionIsCustomMansion_001c48ac = 1;
    *(undefined2 *)PTR__gCurrentMansionNumber_001c4824 = 0xffff;
    *PTR__gCanUseLadderJump_001c4828 = 1;
  }
  return;
}


// ---- _StartTutorialMansion @ 0x327f6 (80 bytes)

void StartTutorialMansion(void)

{
  int iVar1;
  
  iVar1 = _MyOpenStandardMansion("\x10Tutorial Mansion",0xfffffffe);
  if (iVar1 == 0) {
    _StartGameNoEditor();
  }
  else {
    _SetUpDifficultyScreen();
    _StartOpeningMusic(0);
  }
  return;
}


// ---- _StartGameNoEditor @ 0x32846 (158 bytes)

void _StartGameNoEditor(void)

{
  short sVar1;
  undefined2 uVar2;
  
  sVar1 = _LoadLevelResIDFromGAMEResource();
  if (sVar1 == -1) {
    _ErrorMessage("ERROR: The GAME resource from this mansion couldn\'t be opened.",0);
    _SetUpTitleScreen();
  }
  else {
    uVar2 = _GEGetLevelNumber((int)sVar1,1,200);
    *(undefined2 *)PTR__gCurrentLevel_001c49bc = uVar2;
    _ResetKeys();
    _StartGame((int)sVar1,(int)*(short *)PTR__gCurrentLevel_001c49bc,1);
  }
  return;
}


// ---- _StartGame @ 0x328e4 (587 bytes)

void _StartGame(undefined4 param_1,short param_2)

{
  char cVar1;
  SWError SVar2;
  
  _LoadRestOfGAMEResource();
  _DisposeTitleScreenSprites();
  _StopAndDisposeMusic();
  _StopAllChannels();
  *(undefined2 *)PTR__gCurrentMusicIndex_001c476c = 0xffff;
  _SetUpNewGame();
  _ChangeWorldRectForGame
            (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)*(short *)PTR__gStatsHeight_001c47cc);
  _SWSetPostEraseCallBack
            (*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,(CallBackPtr_t)PTR_LAB_001c4848);
  SVar2 = _SWStickyError();
  if (SVar2 != 0) {
    SVar2 = _SWStickyError();
    _MMFatalErrorCallback
              (SVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90871_001c48fc,0xa09);
  }
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 5;
  *(undefined4 *)PTR__gRunningFrameCount_001c481c = 0;
  if (*PTR__gUseOutputFile_001c47f0 != '\0') {
    if (*PTR__gEditorIsRunning_001c495c == '\0') {
      cVar1 = _IsMoviePlaying();
      if (cVar1 == '\0') {
        cVar1 = _IsMoviePlaying();
        if (cVar1 == '\0') {
          _OpenOutputFile("Output File Log");
        }
      }
      else {
        _OpenOutputFile("Output File Log - Movie");
      }
    }
    else {
      _OpenOutputFile("Output File Log - Editor");
    }
  }
  if ((*PTR__gRecordMovies_001c48f8 != '\0') && (*PTR__gEditorIsRunning_001c495c == '\0')) {
    cVar1 = _IsMoviePlaying();
    if (cVar1 == '\0') {
      _OpenMovieFileForOutput(1,0,0,0);
    }
  }
  _EnableRecordMovieMenuItem(1);
  _LoadLevel((int)param_2,1);
  _SetUpLevelForPlaying();
  _SetCorrectGoldSilverTileImages(1);
  _DrawLevel();
  _SetStretchToFit(*PTR__gStretchToFit_001c49dc);
  *PTR__gSpriteWorldIsValid_001c48c8 = 1;
  _DoGameFadeToNormal(0x1e);
  SVar2 = _SWStickyError();
  if (SVar2 != 0) {
    SVar2 = _SWStickyError();
    _MMFatalErrorCallback
              (SVar2,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90871_001c48fc,0xa2a);
  }
  _RunAnimation();
  if (*PTR__gEditorIsRunning_001c495c != '\0') {
    _CleanUpAfterGame();
  }
  return;
}


// ---- _SetUpNewGame @ 0x32b2f (724 bytes)

void _SetUpNewGame(void)

{
  *PTR__gPlayingGame_001c4924 = 1;
  *PTR__gLastSaveFilename_001c4730 = 0;
  if (*PTR__gUsingEditor_001c48b0 == '\0') {
    _MMHideAllCursors();
  }
  else {
    _MMRemoveGameCursor();
    _MMShieldCursor();
  }
  _ResetKeys();
  _SWSetSpriteWorldMaxFPS(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x3c);
  *PTR__gCanControlWhileJumping_001c482c = 1;
  *PTR__gSilverFloorIsActive_001c4958 = 1;
  *PTR__gGoldFloorIsActive_001c47a8 = 0;
  _ResetConveyorBeltDir();
  _SWResetCurrentTileImages(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _ResetFloorTileAnimations(0,1);
  _SetCorrectGoldSilverTileImages(0);
  _Randomize();
  *(undefined4 *)PTR__gNumFrames_001c49d8 = 0;
  *(undefined4 *)PTR__gMyOldTicks_001c46e8 = 0;
  *PTR__gHeroIsOnConveyorBelt_001c49ec = 0;
  *PTR__gCurrentlyInMansion_001c4908 = 1;
  _MMStartGameTimer();
  *PTR__gRoomIsDark_001c483c = 0;
  *PTR__gRoomWasDark_001c4814 = 0;
  _SetStartingNumLives();
  *(undefined4 *)PTR__gScore_001c48a4 = 0;
  *(undefined4 *)PTR__gOldScore_001c4950 = 0;
  *PTR__gGameOver_001c4718 = 0;
  *PTR__gEndGame_001c4798 = 0;
  *PTR__gDemoMode_001c48b8 = 0;
  *PTR__gBeatTheMansion_001c4888 = 0;
  *PTR__gRestartLevel_001c4858 = 0;
  *PTR__gGameIsPaused_001c484c = 0;
  *PTR__gDidCheat_001c4774 = 0;
  *PTR__gShowSecrets_001c4878 = 0;
  *(undefined4 *)PTR__gNumSecretsFound_001c4990 = 0;
  *(undefined4 *)PTR__gCurrentWindDelta_001c4898 = 0;
  *(undefined4 *)PTR__gCurrentSaveIndex_001c4998 = 0;
  *(undefined2 *)PTR__gCurrentFPSValue_001c4728 = 0;
  *(undefined2 *)PTR__gDelayUntilUpdatingFPSValue_001c47a0 = 0;
  *(undefined4 *)PTR__gNumFramesSinceEnteringRoom_001c47c4 = 0;
  *PTR__gDoFrameSkipping_001c47e8 = 0;
  *PTR__gRemovedGlowsAndShadows_001c4948 = 0;
  *(undefined4 *)PTR__gLastSavePedestalObjectID_001c49f8 = 0;
  *(undefined2 *)PTR__gObjects_001c4910 = 0;
  *(undefined2 *)(PTR__gObjects_001c4910 + 2) = 0;
  *(undefined2 *)(PTR__gObjects_001c4910 + 4) = 0;
  *(undefined2 *)(PTR__gObjects_001c4910 + 6) = 0;
  *(undefined2 *)(PTR__gObjects_001c4910 + 8) = 0;
  *(undefined2 *)(PTR__gObjects_001c4910 + 10) = 0;
  *(undefined2 *)(PTR__gObjects_001c4910 + 0xc) = 0;
  PTR__gObjects_001c4910[0xe] = 0;
  PTR__gObjects_001c4910[0xf] = 0;
  _InitObjectArray();
  _InitDoOnceArray();
  _InitRoomArray();
  _InitMapArray();
  _InitLitRoomArray();
  _ResetStatsArea();
  _DisposeAllMapCachedRooms();
  _UnpauseAllSpriteLayers(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  *PTR__gLayersArePausedForSaveAnim_001c487c = 0;
  *PTR__gStatuesAreDoneFading_001c4934 = 0;
  _SDL_DisableScreenSaver();
  return;
}


// ---- _RunAnimation @ 0x32e03 (163 bytes)

void _RunAnimation(void)

{
  SWError SVar1;
  
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 5;
  _UpdateMenuBar();
  _ResetKeys();
  _SWSetTileChangeProc(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,PTR_LAB_001c4988);
  *(undefined4 *)PTR__gRunningFrameCount_001c481c = 0;
  *PTR__gEscapeKey_001c4734 = 0;
  SVar1 = _SWStickyError();
  if (SVar1 != 0) {
    SVar1 = _SWStickyError();
    _MMFatalErrorCallback
              (SVar1,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___90927_001c4a08,0xac0);
  }
  _UpdateMenuBar();
  return;
}


// ---- _RunAnimationCore @ 0x32ea6 (2609 bytes)

void _RunAnimationCore(void)

{
  char cVar1;
  SpritePtr pSVar2;
  int iVar3;
  SWError SVar4;
  uint uVar5;
  SpriteLayerPtr local_18;
  
  if (*PTR__gGameIsPaused_001c484c == '\0') {
    if (*(int *)PTR__gCurrentGameMode_001c42d0 != 5) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
                 PTR____FUNCTION___90941_001c4a04,0xae1);
    }
    if (*(int *)PTR__gSpriteWorldP_001c40d0 == 0) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
                 PTR____FUNCTION___90941_001c4a04,0xae2);
    }
    _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
    _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
    if (*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) != '\0') {
      if (*(undefined **)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xf8) != PTR_LAB_001c4988) {
        _SWSetTileChangeProc(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,PTR_LAB_001c4988);
      }
      if (*PTR__gDoFrameSkipping_001c47e8 != '\0') {
        *(int *)PTR__gNumFrames_001c49d8 = *(int *)PTR__gNumFrames_001c49d8 + 1;
        _AnimateOneFrame(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
      }
      *(undefined4 *)PTR__gNumTilesDrawnThisFrame_001c4750 = 0;
      pSVar2 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gHeroOnPlatformSpriteLayerP_001c491c,
                                (SpritePtr)0x0);
      if (pSVar2 == (SpritePtr)0x0) {
        local_18 = *(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164;
      }
      else {
        local_18 = *(SpriteLayerPtr *)PTR__gHeroOnPlatformSpriteLayerP_001c491c;
      }
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                 *(SpriteLayerPtr *)PTR__gMedusaSpriteLayerP_001c4094,
                 *(SpriteLayerPtr *)PTR__gFlameSpriteLayerP_001c4130);
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_18,
                 *(SpriteLayerPtr *)PTR__gEnemySpriteLayerP_001c40b0);
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_18,
                 *(SpriteLayerPtr *)PTR__gLadybugSpriteLayerP_001c4844);
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_18,
                 *(SpriteLayerPtr *)PTR__gBirdSpriteLayerP_001c41c4);
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_18,
                 *(SpriteLayerPtr *)PTR__gMedusaSpriteLayerP_001c4094);
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_18,
                 *(SpriteLayerPtr *)PTR__gEyeballSpriteLayerP_001c41c8);
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_18,
                 *(SpriteLayerPtr *)PTR__gSnakeSpriteLayerP_001c4208);
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_18,
                 *(SpriteLayerPtr *)PTR__gGhostSpriteLayerP_001c496c);
      iVar3 = _GetHeroSprite();
      if (iVar3 != 0) {
        _SWCollideSpriteWithLayer
                  (iVar3,*(undefined4 *)PTR__gSignSpriteLayerP_001c41f8,
                   PTR_HeroTipCollideProc_001c492c);
      }
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                 *(SpriteLayerPtr *)PTR__gBigSpiderSpriteLayerP_001c42a4,local_18);
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                 *(SpriteLayerPtr *)PTR__gBigSpiderSpriteLayerP_001c42a4,
                 *(SpriteLayerPtr *)PTR__gBirdSpriteLayerP_001c41c4);
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                 *(SpriteLayerPtr *)PTR__gBigSpiderSpriteLayerP_001c42a4,
                 *(SpriteLayerPtr *)PTR__gEyeballSpriteLayerP_001c41c8);
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                 *(SpriteLayerPtr *)PTR__gBigSpiderSpriteLayerP_001c42a4,
                 *(SpriteLayerPtr *)PTR__gMedusaSpriteLayerP_001c4094);
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                 *(SpriteLayerPtr *)PTR__gObjectSpriteLayerP_001c4268,local_18);
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                 *(SpriteLayerPtr *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,local_18);
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                 *(SpriteLayerPtr *)PTR__gZapperBeamSpriteLayerP_001c41ec,local_18);
      _SWCollideSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                 *(SpriteLayerPtr *)PTR__gSecretIndicatorSpriteLayerP_001c40d8,local_18);
      _CheckForPressedBrickSwitches();
      _CheckHeroWithLavaTiles();
      _CheckDeadHeroWithLavaTiles();
    }
    SVar4 = _SWStickyError();
    if (SVar4 != 0) {
      SVar4 = _SWStickyError();
      _MMFatalErrorCallback
                (SVar4,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
                 PTR____FUNCTION___90941_001c4a04,0xb67);
    }
    if (*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) != '\0') {
      while (iVar3 = (*(int *)PTR__gScore_001c48a4 / 1000) * 1000,
            iVar3 + (*(int *)PTR__gOldScore_001c4950 / 1000) * -1000 != 0 &&
            (*(int *)PTR__gOldScore_001c4950 / 1000) * 1000 <= iVar3) {
        _MMPlaySound(0x23,9,1);
        *(short *)PTR__gNumLivesLeft_001c49e4 = *(short *)PTR__gNumLivesLeft_001c49e4 + 1;
        *(int *)PTR__gOldScore_001c4950 = *(int *)PTR__gOldScore_001c4950 + 1000;
      }
      _UpdateStatsItems();
      *(undefined4 *)PTR__gOldScore_001c4950 = *(undefined4 *)PTR__gScore_001c48a4;
      *(int *)PTR__gNumFrames_001c49d8 = *(int *)PTR__gNumFrames_001c49d8 + 1;
      *(int *)PTR__gNumFramesSinceEnteringRoom_001c47c4 =
           *(int *)PTR__gNumFramesSinceEnteringRoom_001c47c4 + 1;
      if ((*PTR__skippedFPSValue_90940_001c49a0 == '\0') &&
         (uVar5 = _SWGetElapsedMilliseconds(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0),
         (uint)(*(int *)PTR__prevMillisec_90939_001c4710 * 3) < uVar5)) {
        *PTR__skippedFPSValue_90940_001c49a0 = 1;
      }
      else {
        iVar3 = _SWGetElapsedMilliseconds(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0);
        *(int *)PTR__fpsAccum_90938_001c4834 = iVar3 + *(int *)PTR__fpsAccum_90938_001c4834;
        *(short *)PTR__gDelayUntilUpdatingFPSValue_001c47a0 =
             *(short *)PTR__gDelayUntilUpdatingFPSValue_001c47a0 + -1;
        *PTR__skippedFPSValue_90940_001c49a0 = 0;
        iVar3 = _SWGetElapsedMilliseconds(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0);
        *(int *)PTR__prevMillisec_90939_001c4710 = iVar3;
      }
      if ((*(short *)PTR__gDelayUntilUpdatingFPSValue_001c47a0 < 1) &&
         (0 < *(int *)PTR__fpsAccum_90938_001c4834)) {
        *(short *)PTR__gCurrentFPSValue_001c4728 =
             (short)(30000 / (longlong)*(int *)PTR__fpsAccum_90938_001c4834);
        if ((*(int *)PTR__gNumFramesSinceEnteringRoom_001c47c4 < 0x14) ||
           (0x3c < *(int *)PTR__gNumFramesSinceEnteringRoom_001c47c4)) {
          if ((((0x3c < *(int *)PTR__gNumFramesSinceEnteringRoom_001c47c4) &&
               (*(int *)PTR__gNumFramesSinceEnteringRoom_001c47c4 < 0x38)) &&
              (*PTR__gDoFrameSkipping_001c47e8 == '\0')) &&
             (*(short *)PTR__gCurrentFPSValue_001c4728 < 0xdc)) {
            *PTR__gDoFrameSkipping_001c47e8 = 1;
            _SWSetSpriteWorldMaxFPS(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x1e);
          }
        }
        else if ((*PTR__gRemovedGlowsAndShadows_001c4948 == '\0') &&
                (*(short *)PTR__gCurrentFPSValue_001c4728 < 0x37)) {
          _RemoveAllSpriteGlowsAndShadows();
          *PTR__gRemovedGlowsAndShadows_001c4948 = 1;
        }
        *(undefined4 *)PTR__fpsAccum_90938_001c4834 = 0;
        *(undefined2 *)PTR__gDelayUntilUpdatingFPSValue_001c47a0 = 0x1e;
      }
      _MMUpdateChannelInfoForSpriteLayer
                (*(undefined4 *)PTR__gSnakeSpriteLayerP_001c4208,0xd,1,
                 (int)(short)(int)((double)(int)*(short *)PTR__gSoundVolume_001c48dc * 0.5),0x140,
                 300);
      _MMUpdateChannelInfoForSpriteLayer
                (*(undefined4 *)PTR__gFlameSpriteLayerP_001c4130,0x21,0,
                 (int)(short)(int)((double)(int)*(short *)PTR__gSoundVolume_001c48dc * 0.65),0x10e,
                 300);
      _MMUpdateChannelInfoForSpriteLayer
                (*(undefined4 *)PTR__gZapperBeamSpriteLayerP_001c41ec,0x22,3,
                 (int)(short)(int)((double)(int)*(short *)PTR__gSoundVolume_001c48dc * 0.9),300,300)
      ;
      if ((*PTR__gCurrentRoomHasLava_001c4800 != '\0') &&
         (cVar1 = _IsChannelAvailable(2), cVar1 != '\0')) {
        _MMPlayStereoSound2(0,(int)*(short *)PTR__gLavaSnd_001c474c,2,2,
                            (int)(short)(int)((double)(int)*(short *)PTR__gSoundVolume_001c48dc *
                                             0.85),0x3ff0000000000000,0xffffffff);
      }
      if (*PTR__gRestartLevel_001c4858 != '\0') {
        _RestartLevel();
        *PTR__gRestartLevel_001c4858 = 0;
      }
    }
    if (*PTR__gGameOver_001c4718 == '\0') {
      if (*PTR__gEndGame_001c4798 == '\0') {
        if (*PTR__gBeatTheMansion_001c4888 != '\0') {
          cVar1 = _IsMoviePlaying();
          _MMPauseGameTimer();
          if (cVar1 == '\0') {
            _DoGameFadeToBlack(0x37,1);
            _CleanUpAfterGame();
            if (*(short *)PTR__gCurrentMansionNumber_001c4824 == 10) {
              _DoBeatTheGameSlideshow();
            }
            else {
              _DoBeatTheMansionScreen(0);
            }
          }
          else {
            _DoGameFadeToBlack(0x37,1);
            _CleanUpAfterGame();
            _SetUpTitleScreen();
          }
        }
      }
      else {
        _DoEndGame(0);
      }
    }
    else if (*PTR__gUsingEditor_001c48b0 == '\0') {
      cVar1 = _IsMoviePlaying();
      _CleanUpAfterGame();
      if (cVar1 == '\0') {
        _DoGameOverScreen();
      }
      else {
        _DoGameFadeToBlack(0x37,1);
        _SetUpTitleScreen();
      }
    }
    else {
      _ExitToEditor();
    }
  }
  return;
}


// ---- _MyPostEraseCallback @ 0x338d7 (164 bytes)

void MyPostEraseCallback(int param_1)

{
  char cVar1;
  
  cVar1 = _IsStatsAreaVisible();
  if ((cVar1 != '\0') && (*(int *)PTR__gStatsSpriteWorldP_001c48f4 != 0)) {
    _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gStatsSpriteWorldP_001c48f4,0);
  }
  _SDL_GL_MakeCurrent(*(undefined4 *)(param_1 + 0x158),*(undefined4 *)(param_1 + 0x15c));
  _glViewport((int)*(short *)(param_1 + 0x20),(int)*(short *)(param_1 + 0x22),
              (int)*(short *)(param_1 + 0x24),(int)*(short *)(param_1 + 0x26));
  return;
}


// ---- _MyTileLayer3Callback @ 0x3397b (27 bytes)

void MyTileLayer3Callback(void)

{
  return;
}


// ---- _SetMovingPlatformOverlapBottomInset @ 0x33996 (115 bytes)

void SetMovingPlatformOverlapBottomInset(SpriteLayerPtr param_1,short param_2)

{
  undefined4 uStack_10;
  
  for (uStack_10 = _SWGetNextSprite(param_1,(SpritePtr)0x0); uStack_10 != (SpriteRec *)0x0;
      uStack_10 = _SWGetNextSprite(param_1,uStack_10)) {
    _SWSetDrawTilesOverSpriteInsetRect(uStack_10,0,0,0,param_2);
  }
  return;
}


// ---- _CleanUpAfterGame @ 0x33a09 (252 bytes)

void _CleanUpAfterGame(void)

{
  *(undefined4 *)PTR__gLightSaberFloorTranslucency_001c488c = 0xffff;
  _CloseMansionFile();
  _CloseGameMovieFile();
  *PTR__gStartRecordingMovie_001c48d4 = 0;
  _DisposeAllMapCachedRooms();
  _SWSetPostEraseCallBack(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,(CallBackPtr_t)0x0);
  _MyRestoreWorldRect(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  _SWSetTileChangeProc(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,(void *)0x0);
  _DisposeCurrentRoom(1);
  _UnpauseAllSpriteLayers(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  *PTR__gLayersArePausedForSaveAnim_001c487c = 0;
  _StopAllChannels();
  *PTR__gRoomIsDark_001c483c = 0;
  _SWSetAllSpriteLayersVisible(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,1);
  _SWDisposeBackgroundTileTexture(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  _SDL_EnableScreenSaver();
  *PTR__gPlayingGame_001c4924 = 0;
  return;
}


// ---- _CloseMansionFile @ 0x33b05 (57 bytes)

void _CloseMansionFile(void)

{
  if (*PTR__gEditorIsRunning_001c495c == '\0') {
    *PTR__gCurOpenMansionFilePath_001c4124 = 0;
    *PTR__gCurOpenMansionFileName_001c4894 = 0;
  }
  return;
}


// ---- _ShutDown @ 0x33b3e (85 bytes)

void _ShutDown(void)

{
  _printf("ShutDown() called. Calling MyCleanUpFunction and exit(1).");
  _MyCleanUpFunction();
  if (*(int *)PTR__gSpriteWorldP_001c40d0 != 0) {
    _SWDisposeSpriteWorld((SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
    _SWExitSpriteWorld();
  }
                    /* WARNING: Subroutine does not return */
  _exit(1);
}


// ---- _MyCleanUpFunction @ 0x33b93 (81 bytes)

void _MyCleanUpFunction(void)

{
  _CloseOutputFile();
  _CloseGameMovieFile();
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0x1a;
  _MMInitCursor();
  _MMFlushEvents();
  _DoGameFadeToNormal(0);
  _StopAllChannels();
  _StopAndDisposeMusic();
  return;
}


// ---- _IsPlayingGame @ 0x33be4 (88 bytes)

undefined1 IsPlayingGame(void)

{
  undefined1 uStack_10;
  
  if (((*(int *)PTR__gCurrentGameMode_001c42d0 == 5) ||
      (*(int *)PTR__gCurrentGameMode_001c42d0 == 6)) ||
     (*(int *)PTR__gCurrentGameMode_001c42d0 == 0xe)) {
    uStack_10 = 1;
  }
  else {
    uStack_10 = 0;
  }
  return uStack_10;
}


// ---- _SetStartingNumLives @ 0x33c3c (60 bytes)

void _SetStartingNumLives(void)

{
  undefined2 uVar1;
  
  uVar1 = _GetStartingNumLives((int)(char)*PTR__gDifficultySetting_001c4870);
  *(undefined2 *)PTR__gNumLivesLeft_001c49e4 = uVar1;
  return;
}


// ---- _GetStartingNumLives @ 0x33c78 (66 bytes)

undefined2 _GetStartingNumLives(char param_1)

{
  undefined2 local_e;
  
  if ((*(short *)PTR__gCurrentMansionNumber_001c4824 == 0) && (param_1 == '\0')) {
    local_e = 0x5a;
  }
  else {
    local_e = 6;
  }
  return local_e;
}


// ---- _RestoreJacksLives @ 0x33cba (100 bytes)

void _RestoreJacksLives(void)

{
  short sVar1;
  undefined2 uVar2;
  
  sVar1 = _GetStartingNumLives((int)(char)*PTR__gDifficultySetting_001c4870);
  if (*(short *)PTR__gNumLivesLeft_001c49e4 < sVar1) {
    uVar2 = _GetStartingNumLives((int)(char)*PTR__gDifficultySetting_001c4870);
    *(undefined2 *)PTR__gNumLivesLeft_001c49e4 = uVar2;
  }
  return;
}


// ---- _MMStartGameTimer @ 0x33d1e (48 bytes)

void _MMStartGameTimer(void)

{
  *(undefined4 *)PTR__gTimeSpentInMansion_001c46fc = 0;
  *(undefined4 *)PTR__gMyOldNumFrames_001c4928 = 0;
  return;
}


// ---- _MMUpdateTimeSpentInMansion @ 0x33d4e (141 bytes)

void _MMUpdateTimeSpentInMansion(void)

{
  uint uVar1;
  
  uVar1 = (uint)(*(int *)PTR__gNumFrames_001c49d8 - *(int *)PTR__gMyOldNumFrames_001c4928) / 0x3c;
  if (uVar1 != 0) {
    *(uint *)PTR__gMyOldNumFrames_001c4928 = uVar1 * 0x3c + *(int *)PTR__gMyOldNumFrames_001c4928;
    *(uint *)PTR__gTimeSpentInMansion_001c46fc = *(int *)PTR__gTimeSpentInMansion_001c46fc + uVar1;
  }
  return;
}


// ---- _MMPauseGameTimer @ 0x33ddb (19 bytes)

void _MMPauseGameTimer(void)

{
  _MMUpdateTimeSpentInMansion();
  return;
}


// ---- _MMResumeGameTimer @ 0x33dee (14 bytes)

void _MMResumeGameTimer(void)

{
  return;
}


// ---- _MakeMedusaHeadsForHackers @ 0x33dfc (80 bytes)

void MakeMedusaHeadsForHackers(int param_1)

{
  int iVar1;
  
  iVar1 = _GetRandom(5,0x1e);
  if (iVar1 <= param_1) {
    _GetRandom(5,0x1e);
  }
  return;
}


// ---- _MMDisposeAllSpritesInSpriteWorld @ 0x33e4c (87 bytes)

void _MMDisposeAllSpritesInSpriteWorld(SpriteWorldPtr param_1)

{
  undefined4 local_10;
  
  local_10 = (SpriteLayerPtr)0x0;
  while( true ) {
    local_10 = _SWGetNextSpriteLayer(param_1,local_10);
    if (local_10 == (SpriteLayerPtr)0x0) break;
    if (local_10 != *(SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4) {
      _SWDisposeAllSpritesInLayer(local_10);
    }
  }
  return;
}


// ---- _TileChangeProc @ 0x33ea3 (3067 bytes)

void TileChangeProc(SpriteWorldPtr param_1)

{
  short sVar1;
  bool bVar2;
  char cVar3;
  undefined2 uStack_2e;
  short sStack_28;
  short sStack_26;
  short sStack_1a;
  short sStack_18;
  short sStack_16;
  short sStack_14;
  short sStack_12;
  short sStack_e;
  
  cVar3 = _IsMoviePlaying();
  if (cVar3 != '\0') {
    _GetKeyEventsFromMovie();
  }
  if ((*(int *)PTR__gCurrentGameMode_001c42d0 == 5) ||
     (*(int *)PTR__gCurrentGameMode_001c42d0 == 0xe)) {
    *(int *)PTR__gRunningFrameCount_001c481c = *(int *)PTR__gRunningFrameCount_001c481c + 1;
  }
  if (*PTR__gLayersArePausedForSaveAnim_001c487c == '\0') {
    bVar2 = false;
    sVar1 = (short)*(undefined4 *)PTR__gNumFrames_001c49d8 -
            (short)*(undefined4 *)PTR__gMyOldTicks_001c46e8;
    *(undefined4 *)PTR__gMyOldTicks_001c46e8 = *(undefined4 *)PTR__gNumFrames_001c49d8;
    if ((*(short *)PTR__gCurConveyorBeltDirection_001c470c != 0) &&
       (*PTR__gRoomIsDark_001c483c == '\0')) {
      *(short *)PTR__conveyorDelay_91331_001c4754 =
           sVar1 + *(short *)PTR__conveyorDelay_91331_001c4754;
    }
    if ((0 < *(short *)PTR__conveyorDelay_91331_001c4754) &&
       (*(short *)PTR__gCurConveyorBeltDirection_001c470c != 0)) {
      if (*(short *)PTR__gCurConveyorBeltDirection_001c470c < 1) {
        sStack_1a = 0x249;
        sStack_18 = 0x250;
        sStack_16 = 0x241;
        sStack_14 = 0x248;
      }
      else {
        sStack_1a = 0x241;
        sStack_18 = 0x248;
        sStack_16 = 0x249;
        sStack_14 = 0x250;
      }
      if (param_1->curTileImage[sStack_1a] + 8 < 0x2c0) {
        for (sStack_26 = sStack_1a; sStack_26 <= sStack_18; sStack_26 = sStack_26 + 1) {
          _SWChangeTileImageFast
                    (param_1,(int)sStack_26,(int)(short)(param_1->curTileImage[sStack_26] + 8));
        }
        _MyUpdateTilesOnScreenFaster(param_1,1,4,sStack_1a,sStack_18);
      }
      else {
        for (sStack_26 = sStack_1a; sStack_26 <= sStack_18; sStack_26 = sStack_26 + 1) {
          _SWChangeTileImageFast
                    (param_1,(int)sStack_26,(int)(short)((sStack_26 - sStack_1a) + 0x241));
        }
        _MyUpdateTilesOnScreenFaster(param_1,1,4,sStack_1a,sStack_18);
      }
      if (param_1->curTileImage[sStack_16] + -8 < 0x241) {
        for (sStack_26 = sStack_16; sStack_26 <= sStack_14; sStack_26 = sStack_26 + 1) {
          _SWChangeTileImageFast
                    (param_1,(int)sStack_26,(int)(short)((sStack_26 - sStack_16) + 0x2b9));
        }
        _MyUpdateTilesOnScreenFaster(param_1,1,4,sStack_16,sStack_14);
      }
      else {
        for (sStack_26 = sStack_16; sStack_26 <= sStack_14; sStack_26 = sStack_26 + 1) {
          _SWChangeTileImageFast
                    (param_1,(int)sStack_26,(int)(short)(param_1->curTileImage[sStack_26] + -8));
        }
        _MyUpdateTilesOnScreenFaster(param_1,1,4,sStack_16,sStack_14);
      }
      *(undefined2 *)PTR__conveyorDelay_91331_001c4754 = 0;
    }
    *(short *)PTR__gDisappearingFloorWait_001c4764 =
         sVar1 + *(short *)PTR__gDisappearingFloorWait_001c4764;
    if (0xb3 < *(short *)PTR__gDisappearingFloorWait_001c4764) {
      *(undefined2 *)PTR__gDisappearingFloorWait_001c4764 = 0;
      *(short *)PTR__gDisappearingFloorMode_001c48a8 =
           *(short *)PTR__gDisappearingFloorMode_001c48a8 + 1;
      if (3 < *(short *)PTR__gDisappearingFloorMode_001c48a8) {
        *(undefined2 *)PTR__gDisappearingFloorMode_001c48a8 = 0;
      }
      *(undefined2 *)PTR__gDisappearingFloorAnimDelay_001c48e0 = 3;
    }
    if (((*(short *)PTR__gDisappearingFloorMode_001c48a8 == 0) ||
        (*(short *)PTR__gDisappearingFloorMode_001c48a8 == 2)) &&
       (*(short *)PTR__gDisappearingFloorAnimDelay_001c48e0 =
             sVar1 + *(short *)PTR__gDisappearingFloorAnimDelay_001c48e0,
       2 < *(short *)PTR__gDisappearingFloorAnimDelay_001c48e0)) {
      if (*(short *)PTR__gDisappearingFloorMode_001c48a8 == 2) {
        sStack_12 = 8;
      }
      else {
        sStack_12 = -8;
      }
      if (((int)param_1->curTileImage[0x2c1] + (int)sStack_12 < 0x2c1) ||
         (0x2e7 < (int)param_1->curTileImage[0x2c1] + (int)sStack_12)) {
        if (*(short *)PTR__gDisappearingFloorMode_001c48a8 == 0) {
          *(undefined2 *)PTR__gDisappearingFloorMode_001c48a8 = 1;
        }
        else {
          *(undefined2 *)PTR__gDisappearingFloorMode_001c48a8 = 3;
        }
      }
      else {
        for (sStack_26 = 0x2c1; sStack_26 < 0x2c9; sStack_26 = sStack_26 + 1) {
          _SWChangeTileImageFast
                    (param_1,(int)sStack_26,
                     (int)(short)(param_1->curTileImage[sStack_26] + sStack_12));
        }
        _MyUpdateTilesOnScreenFaster(param_1,1,4,0x2c1,0x2c8);
      }
      *(undefined2 *)PTR__gDisappearingFloorAnimDelay_001c48e0 = 0;
    }
    *(short *)PTR__lavaAnimDelay_91332_001c4914 =
         sVar1 + *(short *)PTR__lavaAnimDelay_91332_001c4914;
    if (10 < *(short *)PTR__lavaAnimDelay_91332_001c4914) {
      for (sStack_26 = 0x309; sStack_26 < 0x30d; sStack_26 = sStack_26 + 1) {
        sStack_28 = param_1->curTileImage[sStack_26] + 4;
        if (0x318 < sStack_28) {
          sStack_28 = sStack_26;
        }
        _SWChangeTileImageFast(param_1,(int)sStack_26,(int)sStack_28);
      }
      for (sStack_26 = 0x319; sStack_26 < 0x31b; sStack_26 = sStack_26 + 1) {
        sStack_28 = param_1->curTileImage[sStack_26] + 2;
        if (800 < sStack_28) {
          sStack_28 = sStack_26;
        }
        _SWChangeTileImageFast(param_1,(int)sStack_26,(int)sStack_28);
      }
      for (sStack_26 = 0x321; sStack_26 < 0x323; sStack_26 = sStack_26 + 1) {
        sStack_28 = param_1->curTileImage[sStack_26] + 2;
        if (0x328 < sStack_28) {
          sStack_28 = sStack_26;
        }
        _SWChangeTileImageFast(param_1,(int)sStack_26,(int)sStack_28);
      }
      for (sStack_26 = 0x329; sStack_26 < 0x32b; sStack_26 = sStack_26 + 1) {
        sStack_28 = param_1->curTileImage[sStack_26] + 2;
        if (0x330 < sStack_28) {
          sStack_28 = sStack_26;
        }
        _SWChangeTileImageFast(param_1,(int)sStack_26,(int)sStack_28);
      }
      bVar2 = true;
      *(undefined2 *)PTR__lavaAnimDelay_91332_001c4914 = 0;
    }
    *(short *)PTR__lavaFlowDelay_91333_001c47b4 =
         sVar1 + *(short *)PTR__lavaFlowDelay_91333_001c47b4;
    if (3 < *(short *)PTR__lavaFlowDelay_91333_001c47b4) {
      bVar2 = true;
      *(undefined2 *)PTR__lavaFlowDelay_91333_001c47b4 = 0;
    }
    if (bVar2) {
      _SWUpdateTilesOnScreenFast(param_1,0x309,0x330);
    }
    if (*PTR__gRoomIsDark_001c483c == '\0') {
      *(short *)PTR__gLightSaberFloorWait_001c4818 =
           *(short *)PTR__gLightSaberFloorWait_001c4818 - sVar1;
    }
    if (*(short *)PTR__gLightSaberFloorWait_001c4818 < 1) {
      *(undefined2 *)PTR__gLightSaberFloorAnimDelay_001c49fc = 0;
      if (*(short *)PTR__gLightSaberFloorMode_001c4984 == 1) {
        uStack_2e = 3;
      }
      else {
        uStack_2e = 1;
      }
      *(undefined2 *)PTR__gLightSaberFloorMode_001c4984 = uStack_2e;
      if (*(short *)PTR__gLightSaberFloorMode_001c4984 == 3) {
        _HideLightSaberFloorsIfOff(param_1,1);
        *(undefined2 *)PTR__gLightSaberFloorWait_001c4818 = 200;
      }
      else {
        if (*(short *)PTR__gLightSaberFloorMode_001c4984 != 1) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c"
                     ,PTR____FUNCTION___91369_001c47e4,0xf28);
        }
        *(undefined2 *)PTR__gLightSaberCurTranslucencyOffset_001c49c4 = 2000;
        *(undefined4 *)PTR__gLightSaberFloorCurTranslucency_001c4724 = 0;
        _SWSetTileColorForRange
                  (param_1,0x2e9,0x2f0,0xffff,0xffff,0xffff,
                   *(uint *)PTR__gLightSaberFloorCurTranslucency_001c4724 & 0xffff);
        *(undefined2 *)PTR__gLightSaberFloorWait_001c4818 = 200;
      }
    }
    if ((*(short *)PTR__gLightSaberFloorMode_001c4984 == 1) && (*PTR__gRoomIsDark_001c483c == '\0'))
    {
      bVar2 = false;
      *(short *)PTR__gLightSaberFloorAnimDelay_001c49fc =
           sVar1 + *(short *)PTR__gLightSaberFloorAnimDelay_001c49fc;
      if (0 < *(short *)PTR__gLightSaberFloorAnimDelay_001c49fc) {
        *(undefined2 *)PTR__gLightSaberFloorAnimDelay_001c49fc = 0;
        *(int *)PTR__gLightSaberFloorCurTranslucency_001c4724 =
             (int)*(short *)PTR__gLightSaberCurTranslucencyOffset_001c49c4 +
             *(int *)PTR__gLightSaberFloorCurTranslucency_001c4724;
        if ((*(int *)PTR__gLightSaberFloorCurTranslucency_001c4724 < 0xd888) ||
           (*(short *)PTR__gLightSaberCurTranslucencyOffset_001c49c4 < 1)) {
          if ((*(int *)PTR__gLightSaberFloorCurTranslucency_001c4724 < 0x2556) &&
             (*(short *)PTR__gLightSaberCurTranslucencyOffset_001c49c4 < 0)) {
            *(undefined **)PTR__gLightSaberFloorCurTranslucency_001c4724 = &DAT_00002555;
            *(short *)PTR__gLightSaberCurTranslucencyOffset_001c49c4 =
                 -*(short *)PTR__gLightSaberCurTranslucencyOffset_001c49c4;
          }
        }
        else {
          *(undefined4 *)PTR__gLightSaberFloorCurTranslucency_001c4724 = 0xd888;
          *(short *)PTR__gLightSaberCurTranslucencyOffset_001c49c4 =
               -*(short *)PTR__gLightSaberCurTranslucencyOffset_001c49c4;
          bVar2 = true;
        }
        _SWSetTileTranslucencyForRange
                  (param_1,0x2e9,0x2f0,
                   *(uint *)PTR__gLightSaberFloorCurTranslucency_001c4724 & 0xffff);
        *(undefined4 *)PTR__gLightSaberFloorTranslucency_001c488c =
             *(undefined4 *)PTR__gLightSaberFloorCurTranslucency_001c4724;
        if (bVar2) {
          if (*(short *)PTR__gLightSaberFloorMode_001c4984 == 2) {
            sStack_e = 8;
          }
          else {
            sStack_e = -8;
          }
          if (((int)param_1->curTileImage[0x2e9] + (int)sStack_e < 0x2e9) ||
             (0x307 < (int)param_1->curTileImage[0x2e9] + (int)sStack_e)) {
            for (sStack_26 = 0x2e9; sStack_26 < 0x2f1; sStack_26 = sStack_26 + 1) {
              _SWChangeTileImageFast(param_1,(int)sStack_26,(int)(short)(sStack_26 + 0x18));
            }
            _MyUpdateTilesOnScreenFaster(param_1,1,4,0x2e9,0x2f0);
          }
          else {
            for (sStack_26 = 0x2e9; sStack_26 < 0x2f1; sStack_26 = sStack_26 + 1) {
              _SWChangeTileImageFast
                        (param_1,(int)sStack_26,
                         (int)(short)(param_1->curTileImage[sStack_26] + sStack_e));
            }
            _MyUpdateTilesOnScreenFaster(param_1,1,4,0x2e9,0x2f0);
          }
        }
        else {
          _MyUpdateTilesOnScreenFaster(param_1,1,4,0x2e9,0x2f0);
        }
      }
    }
  }
  return;
}


// ---- _MyDrawTile @ 0x34a9e (261 bytes)

void _MyDrawTile(SpriteWorldPtr param_1,short param_2,short param_3,short param_4,short param_5)

{
  TileMapStruct *pTVar1;
  
  if ((param_2 < 0) || (param_1->lastActiveTileLayer < param_2)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___91669_001c47d4,0xfac);
  }
  pTVar1 = param_1->tileLayerArray[param_2];
  if ((((-1 < param_3) && (param_3 < pTVar1->numRows)) && (-1 < param_4)) &&
     ((param_4 < pTVar1->numCols && (pTVar1->tileMap[param_3][param_4] != param_5)))) {
    _SWDrawTile(param_1,param_2,param_3,param_4,param_5);
  }
  return;
}


// ---- _ResetFloorTileAnimations @ 0x34ba3 (288 bytes)

void _ResetFloorTileAnimations(undefined1 param_1,char param_2)

{
  undefined2 local_e;
  
  *(undefined2 *)PTR__gDisappearingFloorWait_001c4764 = 0x5a;
  *(undefined2 *)PTR__gDisappearingFloorAnimDelay_001c48e0 = 0;
  *(undefined2 *)PTR__gDisappearingFloorMode_001c48a8 = 3;
  for (local_e = 0x2c1; local_e < 0x2c9; local_e = local_e + 1) {
    *(short *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xd8) + local_e * 2) = local_e + 0x20;
  }
  if (param_2 != '\0') {
    *(undefined2 *)PTR__gLightSaberFloorWait_001c4818 = 0xf;
    *(undefined2 *)PTR__gLightSaberFloorAnimDelay_001c49fc = 0;
    *(undefined4 *)PTR__gLightSaberFloorCurTranslucency_001c4724 = 0;
    *(undefined2 *)PTR__gLightSaberCurTranslucencyOffset_001c49c4 = 0;
    *(undefined2 *)PTR__gLightSaberFloorMode_001c4984 = 3;
    for (local_e = 0x2e9; local_e < 0x2f1; local_e = local_e + 1) {
      *(short *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xd8) + local_e * 2) = local_e;
    }
    _HideLightSaberFloorsIfOff(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1);
  }
  return;
}


// ---- _HideLightSaberFloorsIfOff @ 0x34cc3 (129 bytes)

void _HideLightSaberFloorsIfOff(undefined4 param_1)

{
  undefined2 local_e;
  
  if (*(short *)PTR__gLightSaberFloorMode_001c4984 != 1) {
    for (local_e = 0x2e9; local_e < 0x2f1; local_e = local_e + 1) {
      _SWSetTileColorForRange(param_1,0x2e9,0x2f0,0x46ff,0x46ff,0x46ff,0x94ff);
    }
  }
  return;
}


// ---- _MyGetTile @ 0x34d44 (228 bytes)

int _MyGetTile(int param_1,short param_2,short param_3,short param_4)

{
  short *psVar1;
  short sVar2;
  int local_2c;
  
  if ((param_2 < 0) || (*(short *)(param_1 + 0xca) < param_2)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___91753_001c485c,0x1008);
  }
  psVar1 = *(short **)(*(int *)(param_1 + 0xc4) + param_2 * 4);
  if ((((param_3 < 0) || (*psVar1 <= param_3)) || (param_4 < 0)) || (psVar1[1] <= param_4)) {
    local_2c = -1;
  }
  else {
    sVar2 = _ConvertTileID((int)*(short *)(*(int *)(*(int *)(psVar1 + 2) + param_3 * 4) +
                                          param_4 * 2));
    local_2c = (int)sVar2;
  }
  return local_2c;
}


// ---- _DrawNothingDrawProc @ 0x34e28 (14 bytes)

void DrawNothingDrawProc(void)

{
  return;
}


// ---- _DoorSpriteDrawProc @ 0x34e36 (756 bytes)

void DoorSpriteDrawProc(int param_1,int param_2)

{
  int iVar1;
  void *pvVar2;
  undefined4 *puVar3;
  int iVar4;
  
  iVar1 = *(int *)(param_1 + 0xe0);
  pvVar2 = _BKLoadGLfunctions();
  puVar3 = (undefined4 *)_BKLoadGLcapabilities();
  if (*(int *)(param_1 + 0xe8) < 0x2d) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___91806_001c4784,0x103f);
  }
  if (0x44 < *(int *)(param_1 + 0xe8)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___91806_001c4784,0x1040);
  }
  (**(code **)((int)pvVar2 + 0x30))(*puVar3,&DAT_00002800,&DAT_00002601);
  (**(code **)((int)pvVar2 + 0x30))(*puVar3,&DAT_00002801,&DAT_00002601);
  _glColor4us(*(undefined2 *)(iVar1 + 0x50),*(undefined2 *)(iVar1 + 0x52),
              *(undefined2 *)(iVar1 + 0x54),*(uint *)(iVar1 + 0x14) & 0xffff);
  (**(code **)((int)pvVar2 + 0x74))(0x1700);
  (**(code **)((int)pvVar2 + 0x7c))();
  (**(code **)((int)pvVar2 + 0x78))();
  if (*(int *)PTR__gSWCurrentSpriteWorld_001c439c != 0) {
    if (*(char *)(iVar1 + 0x4d) != '\0') {
      (**(code **)((int)pvVar2 + 0x88))
                (*(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x3c),
                 *(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x40),0x3f800000);
    }
    (**(code **)((int)pvVar2 + 0x88))
              (*(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0xb4),
               *(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0xb8),0x3f800000);
  }
  (**(code **)((int)pvVar2 + 0x8c))
            ((float)(int)*(short *)(param_1 + 0x38),(float)(int)*(short *)(param_1 + 0x36),0);
  if ((((*(float *)(iVar1 + 0x30) != 1.0) || (NAN(*(float *)(iVar1 + 0x30)))) ||
      (*(float *)(iVar1 + 0x2c) != 1.0)) || (NAN(*(float *)(iVar1 + 0x2c)))) {
    (**(code **)((int)pvVar2 + 0x88))
              (*(undefined4 *)(iVar1 + 0x2c),*(undefined4 *)(iVar1 + 0x30),0x3f800000);
  }
  iVar4 = _DoesSurfaceNeedBlend(*(undefined4 *)(param_2 + 0x34));
  if ((iVar4 != 0) || (*(int *)(iVar1 + 0x14) != 0xffff)) {
    (**(code **)((int)pvVar2 + 0x14))(0xbe2);
    (**(code **)((int)pvVar2 + 0x50))(*(undefined4 *)(iVar1 + 0x44),*(undefined4 *)(iVar1 + 0x48));
  }
  _BKBlitCustomDoorSurface
            (*(undefined4 *)(param_2 + 0x34),(int)*(short *)(param_1 + 0xfc),
             (int)*(short *)(param_1 + 0xfe));
  (**(code **)((int)pvVar2 + 0x80))();
  return;
}


// ---- _BKBlitCustomDoorSurface @ 0x3512a (1916 bytes)

undefined4 _BKBlitCustomDoorSurface(int *param_1,short param_2,short param_3)

{
  undefined4 *puVar1;
  undefined4 local_38;
  int local_20;
  undefined4 *local_1c;
  short *local_18;
  float *local_14;
  
  if (param_1 == (int *)0x0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               0x1085,"glsurface != ((void *)0)");
  }
  if (*param_1 == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               0x1086,"glsurface->textureSurface != ((void *)0)");
  }
  if (param_1[0x16] == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               0x1087,"glsurface->textureIDs != ((void *)0)");
  }
  if (param_1[0x17] == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               0x1088,"glsurface->pixelCoords != ((void *)0)");
  }
  if (param_1[0x18] == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               0x1089,"glsurface->textureCoords != ((void *)0)");
  }
  puVar1 = (undefined4 *)_BKLoadGLfunctions();
  if (puVar1 == (undefined4 *)0x0) {
    _SDL_SetError("Failed to load GL functions");
    local_38 = 0xffffffff;
  }
  else {
    local_1c = (undefined4 *)param_1[0x16];
    local_18 = (short *)param_1[0x17];
    local_14 = (float *)param_1[0x18];
    if (param_3 < 1) {
      for (local_20 = 0; local_20 < param_1[0x12]; local_20 = local_20 + 1) {
        (*(code *)puVar1[0xb])(param_1[0xc],*local_1c);
        (*(code *)*puVar1)(4);
        (*(code *)puVar1[0x1a])(*local_14,local_14[1] - (float)(int)param_2);
        (*(code *)puVar1[0x18])((int)*local_18,(int)local_18[1]);
        (*(code *)puVar1[0x1a])(local_14[2],local_14[3] - (float)(int)param_2);
        (*(code *)puVar1[0x18])((int)local_18[2],(int)local_18[3]);
        (*(code *)puVar1[0x1a])(local_14[4],local_14[5]);
        (*(code *)puVar1[0x18])((int)local_18[4],(int)local_18[5] + (int)param_2);
        (*(code *)puVar1[0x1a])(local_14[6],local_14[7] - (float)(int)param_2);
        (*(code *)puVar1[0x18])((int)local_18[6],(int)local_18[7]);
        (*(code *)puVar1[0x1a])(local_14[8],local_14[9]);
        (*(code *)puVar1[0x18])((int)local_18[8],(int)local_18[9] + (int)param_2);
        (*(code *)puVar1[0x1a])(local_14[10],local_14[0xb]);
        (*(code *)puVar1[0x18])((int)local_18[10],(int)local_18[0xb] + (int)param_2);
        (*(code *)puVar1[1])();
        local_1c = local_1c + 1;
        local_18 = local_18 + 4;
        local_14 = local_14 + 4;
      }
    }
    else {
      for (local_20 = 0; local_20 < param_1[0x12]; local_20 = local_20 + 1) {
        (*(code *)puVar1[0xb])(param_1[0xc],*local_1c);
        (*(code *)*puVar1)(4);
        (*(code *)puVar1[0x1a])(*local_14,local_14[1]);
        (*(code *)puVar1[0x18])((int)*local_18,(int)local_18[1]);
        (*(code *)puVar1[0x1a])(local_14[2],local_14[3]);
        (*(code *)puVar1[0x18])((int)local_18[2],(int)local_18[3]);
        (*(code *)puVar1[0x1a])(local_14[4],local_14[5] - (float)(int)param_3);
        (*(code *)puVar1[0x18])((int)local_18[4],(int)local_18[5] - (int)param_3);
        (*(code *)puVar1[0x1a])(local_14[6],local_14[7]);
        (*(code *)puVar1[0x18])((int)local_18[6],(int)local_18[7]);
        (*(code *)puVar1[0x1a])(local_14[8],local_14[9] - (float)(int)param_3);
        (*(code *)puVar1[0x18])((int)local_18[8],(int)local_18[9] - (int)param_3);
        (*(code *)puVar1[0x1a])(*local_14 + 9.0,local_14[0xb] - (float)(int)param_3);
        (*(code *)puVar1[0x18])(*local_18 + 9,(int)local_18[0xb] - (int)param_3);
        (*(code *)puVar1[0x1a])(local_14[6],local_14[7]);
        (*(code *)puVar1[0x18])((int)local_18[6],(int)local_18[7]);
        (*(code *)puVar1[0x1a])(*local_14 + 9.0,local_14[0xb] - (float)(int)param_3);
        (*(code *)puVar1[0x18])(*local_18 + 9,(int)local_18[0xb] - (int)param_3);
        (*(code *)puVar1[0x1a])(local_14[10],(local_14[0xb] - 7.0) - (float)(int)param_3);
        (*(code *)puVar1[0x18])((int)local_18[10],((int)local_18[0xb] - (int)param_3) + -7);
        (*(code *)puVar1[1])();
        local_1c = local_1c + 1;
        local_18 = local_18 + 4;
        local_14 = local_14 + 4;
      }
    }
    local_38 = 0;
  }
  return local_38;
}


// ---- _ClipMainSpriteDrawProc @ 0x358a6 (234 bytes)

void ClipMainSpriteDrawProc(SpriteRec *param_1,FrameRec *param_2,float param_3,float param_4)

{
  short sVar1;
  
  if (param_1->userData != 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/Midnight Mansion.c",
               PTR____FUNCTION___92179_001c478c,0x1138);
  }
  sVar1._0_1_ = param_1->f_1b4;
  sVar1._1_1_ = param_1->f_1b5;
  _SWSetSpriteTemporaryClip
            (param_1,0,0,0,
             (short)(int)((float)((int)(param_1->destFrameRect).bottom - (int)sVar1) *
                         (param_1->unscaledHeight / param_1->drawData->scaledHeight)));
  _SWStdSpriteDrawProc(param_1,param_2,param_3,param_4);
  return;
}


// ---- _ClipSpriteWithLavaDrawProc @ 0x35990 (385 bytes)

void ClipSpriteWithLavaDrawProc(SpriteRec *param_1,FrameRec *param_2,float param_3,float param_4)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sStack_1e;
  short sStack_1c;
  
  sVar1 = (param_1->destFrameRect).bottom;
  if (sVar1 < 0) {
    sVar1 = sVar1 + 0xf;
  }
  sVar2 = (param_1->destFrameRect).right;
  if (sVar2 < 0) {
    sVar2 = sVar2 + 0xf;
  }
  sVar3 = (param_1->destFrameRect).top;
  if (sVar3 < 0) {
    sVar3 = sVar3 + 0xf;
  }
  for (sStack_1e = (sVar3 >> 4) + -1; sStack_1e <= sVar1 >> 4; sStack_1e = sStack_1e + 1) {
    sVar3 = (param_1->destFrameRect).left;
    if (sVar3 < 0) {
      sVar3 = sVar3 + 0xf;
    }
    for (sStack_1c = (sVar3 >> 4) + -1; sStack_1c <= sVar2 >> 4; sStack_1c = sStack_1c + 1) {
      sVar3 = _MyGetTile(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,6,(int)sStack_1e,(int)sStack_1c)
      ;
      if ((0x308 < sVar3) && (sVar3 < 0x331)) goto LAB_00035a6f;
    }
  }
LAB_00035a6f:
  _SWSetSpriteTemporaryClip
            (param_1,0,0,0,
             (short)(int)((float)((int)(param_1->destFrameRect).bottom -
                                 (int)(short)(sStack_1e * 0x10 + -3)) *
                         (param_1->unscaledHeight / param_1->drawData->scaledHeight)));
  _SWStdSpriteDrawProc(param_1,param_2,param_3,param_4);
  return;
}


// ---- _AddShadowDrawProc @ 0x35b11 (46 bytes)

void AddShadowDrawProc(SpriteRec *param_1,FrameRec *param_2,float param_3,float param_4)

{
  _SWStdSpriteDrawProc(param_1,param_2,param_3,param_4);
  return;
}


// ---- _EnemyShadowDrawProc @ 0x35b3f (424 bytes)

void EnemyShadowDrawProc(SpriteRec *param_1,FrameRec *param_2,float param_3,float param_4)

{
  DrawDataRec *pDVar1;
  
  pDVar1 = param_1->drawData;
  if (*(short *)PTR__gOldBackgroundPictResID_001c47b0 == 1) {
    _glMatrixMode(0x1700);
    _glPushMatrix();
    _glLoadIdentity();
    if (*(int *)PTR__gSWCurrentSpriteWorld_001c439c != 0) {
      _glScalef(*(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x3c),
                *(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x40),0x3f800000);
    }
    _glTranslatef(param_3 + 7.0,param_4 - 5.0,0);
    if ((((pDVar1->scaledHeightFactor != 1.0) || (NAN(pDVar1->scaledHeightFactor))) ||
        (pDVar1->scaledWidthFactor != 1.0)) || (NAN(pDVar1->scaledWidthFactor))) {
      _glScalef(pDVar1->scaledWidthFactor,pDVar1->scaledHeightFactor,0x3f800000);
    }
    _glEnable(0xbe2);
    _glBlendFunc(pDVar1->srcBlendMode,pDVar1->dstBlendMode);
    _glColor4us(0,0,0,0x5fff);
    _BKBlitGLSurface(param_2->glSurfaceP);
    _glPopMatrix();
  }
  _SWStdSpriteDrawProc(param_1,param_2,param_3,param_4);
  return;
}


// ---- _DrawMasterTileTextureDrawProc @ 0x35ce7 (976 bytes)

void DrawMasterTileTextureDrawProc
               (int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined2 *puVar2;
  undefined4 *puVar3;
  
  iVar1 = *(int *)(param_1 + 0xe0);
  _glPushMatrix();
  _glLoadIdentity();
  if (*(int *)PTR__gSWCurrentSpriteWorld_001c439c != 0) {
    _glScalef(*(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x3c),
              *(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x40),0x3f800000);
  }
  _glTranslatef(param_3,param_4,0);
  _glEnable(0xbe2);
  _glBlendFunc(*(undefined4 *)(iVar1 + 0x44),*(undefined4 *)(iVar1 + 0x48));
  **(undefined2 **)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x114) + 0x5c) = 0;
  puVar2 = *(undefined2 **)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x114) + 0x5c);
  puVar3 = *(undefined4 **)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x114) + 0x60);
  *puVar2 = 0;
  puVar2[1] = 0;
  puVar2[2] = 0x400;
  puVar2[3] = 0;
  puVar2[4] = 0;
  puVar2[5] = 0x400;
  puVar2[6] = 0x400;
  puVar2[7] = 0;
  puVar2[8] = 0;
  puVar2[9] = 0x400;
  puVar2[10] = 0x400;
  puVar2[0xb] = 0x400;
  *puVar3 = 0;
  puVar3[1] = 0;
  puVar3[2] = 0x44800000;
  puVar3[3] = 0;
  puVar3[4] = 0;
  puVar3[5] = 0x44800000;
  puVar3[6] = 0x44800000;
  puVar3[7] = 0;
  puVar3[8] = 0;
  puVar3[9] = 0x44800000;
  puVar3[10] = 0x44800000;
  puVar3[0xb] = 0x44800000;
  _BKBlitGLSurface(*(void **)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x114));
  _glPopMatrix();
  return;
}


// ---- _TogglePause @ 0x360b7 (72 bytes)

void _TogglePause(void)

{
  if (*(int *)PTR__gCurrentGameMode_001c42d0 == 0xe) {
    _CleanUpAfterSign();
  }
  if (*PTR__gGameIsPaused_001c484c == '\0') {
    _PauseGame();
  }
  else {
    _ResumeGame();
  }
  return;
}


// ---- _PauseGame @ 0x360ff (1837 bytes)

void _PauseGame(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined1 uVar3;
  char cVar4;
  int iVar5;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  longdouble extraout_ST0_01;
  longdouble lVar6;
  SWMovementType SVar7;
  char local_133 [255];
  SpritePtr local_34;
  SpritePtr local_30;
  SWError local_2c;
  short local_28;
  short local_26;
  short local_24;
  short local_22;
  char *local_20;
  
  if (((*PTR__gEditorIsRunning_001c495c == '\0') && (*PTR__gDialogBoxIsOpen_001c49c8 == '\0')) &&
     (*PTR__gGameIsPaused_001c484c == '\0')) {
    if (*(int *)PTR__gCurrentGameMode_001c42d0 == 0xe) {
      _CleanUpAfterSign();
    }
    uVar3 = _IsMusicPlaying();
    *PTR__gMusicWasPlaying_001c4778 = uVar3;
    *PTR__gResetMusic_001c46ec = 0;
    *(undefined4 *)PTR__gSaveTileChangeProc_001c4860 =
         *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xf8);
    _SWSetTileChangeProc(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,(void *)0x0);
    _UpdateMenuBar();
    _PauseMusic();
    _MMPauseGameTimer();
    _ClearCheatCodeCache();
    *PTR__layersWerePausedForSaveAnim_001c47f8 = *PTR__gLayersArePausedForSaveAnim_001c487c;
    _PauseSpriteLayersForPause();
    _StopAllChannels();
    cVar4 = _IsMoviePlaying();
    if (cVar4 == '\0') {
      _ResetKeys();
    }
    _SDL_EnableScreenSaver();
    *PTR__gGameIsPaused_001c484c = 1;
    if (*(int *)PTR__gCurrentGameMode_001c42d0 == 6) {
      _DrawMapInOffscreenAreas();
    }
    if (((*(int *)PTR__gCurrentGameMode_001c42d0 == 5) ||
        (*(int *)PTR__gCurrentGameMode_001c42d0 == 6)) ||
       (*(int *)PTR__gCurrentGameMode_001c42d0 == 2)) {
      local_2c = _SWCreateSprite(&local_34,(void *)0x0,0);
      if (local_2c == 0) {
        _SWSetSpriteDrawProc(local_34,(DrawProcPtr_t)PTR_SWSolidColorSpriteDrawProc_001c43ac);
        uVar1 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x80);
        uVar2 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
        (local_34->destFrameRect).top = (short)uVar2;
        (local_34->destFrameRect).left = (short)((uint)uVar2 >> 0x10);
        (local_34->destFrameRect).bottom = (short)uVar1;
        (local_34->destFrameRect).right = (short)((uint)uVar1 >> 0x10);
        _SWAddSprite(*(SpriteLayerPtr *)PTR__gPausedSpriteLayerP_001c47f4,local_34);
        _SWSetSpriteColor(local_34,0,0,0);
        _SWSetSpriteTranslucencyLevel(local_34,0x5555);
      }
    }
    if ((*(int *)PTR__gCurrentGameMode_001c42d0 == 5) ||
       (*(int *)PTR__gCurrentGameMode_001c42d0 == 6)) {
      _MMPlaySound(0x38,0,1);
      local_30 = (SpritePtr)_NewMiscSprite(0xd8);
      if (local_30 != (SpritePtr)0x0) {
        _SWSetSpriteScaledFactor(local_30,0x3f666666,0x3f666666);
        _SWAddSprite(*(SpriteLayerPtr *)PTR__gPausedSpriteLayerP_001c47f4,local_30);
        _SWCenterSpriteInRect(local_30,*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
        _SWOffsetSprite(local_30,0.0,-50.0);
        SVar7 = _SWGetSpriteVertLoc(local_30);
        _SWGetSpriteHeight(local_30);
        local_24 = (short)(int)(SVar7 + (float)extraout_ST0);
        SVar7 = _SWGetSpriteHorizLoc(local_30);
        local_22 = (short)(int)SVar7;
        local_26 = 0x14;
        for (local_28 = 0; local_28 < 5; local_28 = local_28 + 1) {
          local_34 = (SpritePtr)_NewButtonSprite(0xd9,(int)local_28);
          if (local_34 != (SpritePtr)0x0) {
            _SWAddSprite(*(SpriteLayerPtr *)PTR__gPausedSpriteLayerP_001c47f4,local_34);
            _SWSetCurrentFrameIndex(local_34,(int)local_28);
            _SetUpButtonSprite(local_34,(int)local_28,(int)(short)(local_28 + 5),(int)local_28,
                               (int)(short)(local_28 + 10));
            _InstallButtonProc(local_34,PTR_LAB_001c4034);
            _SetButtonColor(local_34,0xffff,0xffff,0xffff);
            _SetButtonPushedColor(local_34,0x8fff,0x8fff,0x8fff);
            _SetButtonPushedOffset(local_34,1,1);
            if (local_28 == 1) {
              cVar4 = _CanSuspendGame();
              if (cVar4 == '\0') {
                _SetButtonDimmedMode(local_34,1);
              }
            }
            iVar5 = (int)local_24;
            _SWGetSpriteHeight(local_34);
            _SWSetSpriteLocation
                      (local_34,(float)((int)local_22 + (int)local_26),
                       (float)extraout_ST0_00 * -1.6 + (float)iVar5);
            iVar5 = (int)local_26;
            _SWGetSpriteWidth(local_34);
            local_26 = (short)(int)((float)extraout_ST0_01 + 5.0 + (float)iVar5);
          }
        }
        local_34 = (SpritePtr)_NewBitmapTextSprite(2000);
        if (local_34 != (SpritePtr)0x0) {
          if (*(int *)PTR__gTimeSpentInMansion_001c46fc % 0x3c < 10) {
            local_20 = "0";
          }
          else {
            local_20 = "";
          }
          _sprintf(local_133,"Elapsed time: %d:%s%d",
                   *(int *)PTR__gTimeSpentInMansion_001c46fc / 0x3c,local_20,
                   *(int *)PTR__gTimeSpentInMansion_001c46fc % 0x3c);
          _SetBMTextSpriteText(local_34,local_133);
          _SWSetSpriteScaledFactor(local_34,0x3e851eb8,0x3e851eb8);
          _SWAddSprite(*(SpriteLayerPtr *)PTR__gPausedSpriteLayerP_001c47f4,local_34);
          _SWCenterSpriteInRect(local_34,&local_30->destFrameRect);
          lVar6 = (longdouble)_GetWidthOfBMText(local_34,local_133);
          _SWOffsetSprite(local_34,(float)lVar6 / -2.0,40.0);
        }
      }
    }
    _MMSetCursorForGame();
    _MMShieldCursor();
    _DoGameFadeToNormal(0);
    _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
  }
  return;
}


// ---- _ResumeGame @ 0x3682c (322 bytes)

void _ResumeGame(void)

{
  char cVar1;
  
  if (*PTR__gGameIsPaused_001c484c != '\0') {
    _ExitPausedMode();
    if (*PTR__gMusicWasPlaying_001c4778 != '\0') {
      _ResumeMusic(*PTR__gResetMusic_001c46ec);
    }
    _MMResumeGameTimer();
    _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gPausedSpriteLayerP_001c47f4);
    _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gTextSpriteLayerP_001c4804);
    cVar1 = _IsMoviePlaying();
    if (cVar1 == '\0') {
      _ResetKeys();
    }
    if ((*(int *)PTR__gCurrentGameMode_001c42d0 == 5) ||
       (*(int *)PTR__gCurrentGameMode_001c42d0 == 6)) {
      _MMHideAllCursors();
    }
    if (*(int *)PTR__gCurrentGameMode_001c42d0 == 5) {
      _DrawLevel();
    }
    else if (*(int *)PTR__gCurrentGameMode_001c42d0 == 6) {
      _DrawMapInOffscreenAreas();
      _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
    }
    else if (*PTR__gSpriteWorldIsValid_001c48c8 != '\0') {
      _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
    }
    if ((*(int *)PTR__gCurrentGameMode_001c42d0 == 6) ||
       (*(int *)PTR__gCurrentGameMode_001c42d0 == 5)) {
      _DrawStatsArea();
    }
  }
  return;
}


// ---- _PauseSpriteLayersForPause @ 0x3696e (79 bytes)

void _PauseSpriteLayersForPause(void)

{
  _PauseAllSpriteLayers(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
  _SWUnpauseSpriteLayer(*(SpriteLayerPtr *)PTR__gPausedSpriteLayerP_001c47f4);
  _SWUnpauseSpriteLayer(*(SpriteLayerPtr *)PTR__gCustomCursorSpriteLayerP_001c49f4);
  return;
}


// ---- _HandlePausedScreenCallback @ 0x369bd (323 bytes)

void _HandlePausedScreenCallback(void)

{
  undefined4 local_14;
  undefined1 local_f;
  short local_e;
  
  _KeepMusicPlaying();
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  if (*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) != '\0') {
    local_f = _MMGetMouse(&local_14);
    _SWConvertCoordinates(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_14);
    local_e = _ProcessButtonSprites
                        (*(undefined4 *)PTR__gPausedSpriteLayerP_001c47f4,
                         *(undefined4 *)PTR__gPausedSpriteLayerP_001c47f4,local_14,local_f,0,0);
    if (local_e != -1) {
      _AnimateOneFrame(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
      if (local_e == 0) {
        _ResumeGame();
      }
      else if (local_e == 1) {
        _ExitPausedMode();
        _SuspendGame();
        _DoEndGame(0);
      }
      else if (local_e == 2) {
        _MMDoOptionsDialog();
      }
      else if (local_e == 3) {
        _DoEndGame(0);
      }
      else if (local_e == 4) {
        _DoQuitCommand(0);
      }
    }
  }
  return;
}


// ---- _ExitPausedMode @ 0x36b00 (165 bytes)

void _ExitPausedMode(void)

{
  char cVar1;
  
  if (*PTR__gGameIsPaused_001c484c != '\0') {
    *PTR__gGameIsPaused_001c484c = 0;
    _SWSetTileChangeProc
              (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
               *(void **)PTR__gSaveTileChangeProc_001c4860);
    _UnpauseAllSpriteLayers(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    if (*PTR__layersWerePausedForSaveAnim_001c47f8 != '\0') {
      _PauseSpriteLayersForSaveAnim();
    }
    cVar1 = _IsMoviePlaying();
    if (cVar1 == '\0') {
      _ResetKeys();
      _WriteEventToMovieFile(0xb,1);
    }
    _InitGamePad();
    _SDL_DisableScreenSaver();
  }
  return;
}


// ---- _DoEndGame @ 0x36ba5 (203 bytes)

void _DoEndGame(char param_1)

{
  char cVar1;
  undefined2 local_10;
  
  if (param_1 == '\0') {
    local_10 = 3;
  }
  else {
    local_10 = _DoSuspendGameDialog();
  }
  if ((local_10 == 2) || (local_10 == 3)) {
    cVar1 = _IsMoviePlaying();
    if (*(int *)PTR__gCurrentGameMode_001c42d0 == 6) {
      _ExitInGameMapScreen(0);
    }
    _DoGameFadeToBlack(0x1e,1);
    *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
    *PTR__gSpriteWorldIsValid_001c48c8 = 0;
    _MMHideCursor();
    _CleanUpAfterGame();
    _ExitPausedMode();
    _UnpauseAllSpriteLayers(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    if (cVar1 == '\0') {
      _SetUpTitleScreen();
    }
    else {
      _SetUpTitleScreen();
    }
  }
  return;
}


// ---- _DoQuitCommand @ 0x36c70 (259 bytes)

void _DoQuitCommand(char param_1)

{
  char cVar1;
  char local_20;
  short local_10;
  
  local_20 = param_1;
  if (*PTR__gUsingEditor_001c48b0 != '\0') {
    local_20 = '\0';
  }
  if (local_20 == '\0') {
    local_10 = 3;
  }
  else {
    local_10 = _DoSuspendGameDialog();
  }
  if ((local_10 == 2) || (local_10 == 3)) {
    _ExitPausedMode();
    if ((*(int *)PTR__gCurrentGameMode_001c42d0 == 5) ||
       (*(int *)PTR__gCurrentGameMode_001c42d0 == 6)) {
      _WriteCStringToFile("Quit Midnight Mansion.\r");
    }
    if (*PTR__gUsingEditor_001c48b0 == '\0') {
      if (*(int *)PTR__gCurrentGameMode_001c42d0 == 0xd) {
        if (*PTR__gCanQuit_001c4968 != '\0') {
          *PTR__gQuitGame_001c49d0 = 1;
        }
      }
      else {
        _MMFlushEvents();
        *PTR__gQuitGame_001c49d0 = 1;
        cVar1 = _DoTeaserScreen(1);
        if (cVar1 != '\0') {
          *PTR__gQuitGame_001c49d0 = 0;
        }
      }
    }
    else {
      *PTR__gQuitGame_001c49d0 = 1;
    }
  }
  return;
}


// ---- _DoGameFadeToBlack @ 0x36d73 (119 bytes)

void _DoGameFadeToBlack(short param_1,char param_2)

{
  if (*(int *)PTR__gSpriteWorldP_001c40d0 != 0) {
    if (param_2 != '\0') {
      if (param_1 < 2) {
        _StopAndDisposeMusic();
      }
      else {
        _FadeMusicNowNoAdjust((int)(short)(param_1 << 4));
      }
    }
    _DoFadeToBlack((int)param_1,0);
    if (param_2 != '\0') {
      _StopAndDisposeMusic();
    }
  }
  return;
}


// ---- _DoGameFadeToBlackFullAnim @ 0x36dea (119 bytes)

void _DoGameFadeToBlackFullAnim(short param_1,char param_2)

{
  if (*(int *)PTR__gSpriteWorldP_001c40d0 != 0) {
    if (param_2 != '\0') {
      if (param_1 < 2) {
        _StopAndDisposeMusic();
      }
      else {
        _FadeMusicNowNoAdjust((int)(short)(param_1 << 4));
      }
    }
    _DoFadeToBlack((int)param_1,1);
    if (param_2 != '\0') {
      _StopAndDisposeMusic();
    }
  }
  return;
}


// ---- _DoGameFadeToNormal @ 0x36e61 (66 bytes)

void _DoGameFadeToNormal(short param_1)

{
  if (*(int *)PTR__gSpriteWorldP_001c40d0 != 0) {
    _DoFadeToNormal((int)param_1,0);
  }
  return;
}


// ---- _DoGameFadeToNormalFullAnim @ 0x36ea3 (66 bytes)

void _DoGameFadeToNormalFullAnim(short param_1)

{
  if (*(int *)PTR__gSpriteWorldP_001c40d0 != 0) {
    _DoFadeToNormal((int)param_1,1);
  }
  return;
}


// ---- _DoFadeToBlack @ 0x36ee5 (459 bytes)

void _DoFadeToBlack(short param_1,char param_2)

{
  void *tileChangeProc;
  char cVar1;
  SpritePtr pSVar2;
  
  _SWUnpauseSpriteLayer(*(SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4);
  pSVar2 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4,(SpritePtr)0x0
                           );
  if (pSVar2 == (SpritePtr)0x0) {
    tileChangeProc = *(void **)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xf8);
    _SWSetTileChangeProc(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,(void *)0x0);
    _NewFadeSprite(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)param_1);
    _NewFadeSprite(*(undefined4 *)PTR__gStatsSpriteWorldP_001c48f4,(int)param_1);
    while( true ) {
      cVar1 = _IsCurrentlyFadingToBlack();
      if ((cVar1 == '\0') || (*PTR__gQuitGame_001c49d0 != '\0')) break;
      if (param_2 == '\0') {
        _SWProcessSpriteWorldForOneLayer
                  (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                   *(undefined4 *)PTR__gFadeToBlackSpriteLayerP_001c43e4);
        _SWProcessSpriteLayer
                  (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                   *(SpriteLayerPtr *)PTR__gCustomCursorSpriteLayerP_001c49f4);
        cVar1 = _IsStatsAreaVisible();
        if (cVar1 != '\0') {
          _SWProcessSpriteWorldForOneLayer
                    (*(undefined4 *)PTR__gStatsSpriteWorldP_001c48f4,
                     *(undefined4 *)PTR__gStatsFadeSpriteLayerP_001c49cc);
        }
      }
      else {
        _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
        cVar1 = _IsStatsAreaVisible();
        if (cVar1 != '\0') {
          _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gStatsSpriteWorldP_001c48f4);
        }
      }
      if (*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) == '\0') {
        _KeepMusicPlaying();
      }
      else {
        _ProcessSDLEvents();
      }
      _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
    }
    _SWSetTileChangeProc(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,tileChangeProc);
  }
  return;
}


// ---- _DoFadeToNormal @ 0x370b0 (962 bytes)

void _DoFadeToNormal(short param_1,char param_2)

{
  char cVar1;
  SpritePtr local_18;
  SpritePtr local_14;
  void *local_10;
  
  _SWUnpauseSpriteLayer(*(SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4);
  local_18 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4,
                              (SpritePtr)0x0);
  local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gStatsFadeSpriteLayerP_001c49cc,(SpritePtr)0x0
                             );
  if ((local_14 == (SpritePtr)0x0) && (cVar1 = _IsStatsAreaVisible(), cVar1 != '\0')) {
    _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gStatsFadeSpriteLayerP_001c49cc);
    local_14 = (SpritePtr)
               _NewFadeSprite(*(undefined4 *)PTR__gStatsSpriteWorldP_001c48f4,(int)param_1);
  }
  if (local_14 != (SpritePtr)0x0) {
    _SWSetSpriteTranslucencyLevel(local_14,0xffff);
  }
  if (local_18 != (SpritePtr)0x0) {
    _SWSetRect(&local_18->destFrameRect,0,0,
               (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38),
               (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x3a));
    if (param_1 < 1) {
      _SWRemoveSprite(local_18);
      _SWDisposeSprite(&local_18);
      _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4);
      _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
      if (local_14 == (SpritePtr)0x0) {
        return;
      }
      _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gStatsFadeSpriteLayerP_001c49cc);
      _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gStatsSpriteWorldP_001c48f4,0);
      return;
    }
    local_18->userData = (int)(0xffff / (longlong)(int)param_1);
    _SWSetSpriteMoveProc(local_18,(MoveProcPtr_t)PTR_FadeToNormalMoveProc_001c48f0);
    _SWSetSpriteMoveTime(local_18,0x10);
  }
  if (local_14 != (SpritePtr)0x0) {
    if (param_1 < 1) {
      _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gStatsFadeSpriteLayerP_001c49cc);
      _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gStatsSpriteWorldP_001c48f4,0);
      return;
    }
    local_14->userData = (int)(0xffff / (longlong)(int)param_1);
    _SWSetSpriteMoveProc(local_14,(MoveProcPtr_t)PTR_FadeToNormalMoveProc_001c48f0);
    _SWSetSpriteMoveTime(local_14,0x10);
  }
  local_10 = *(void **)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xf8);
  _SWSetTileChangeProc(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,(void *)0x0);
  while ((cVar1 = _IsCurrentlyFadingToBlack(), cVar1 != '\0' && (*PTR__gQuitGame_001c49d0 == '\0')))
  {
    if (param_2 == '\0') {
      _SWProcessSpriteWorldForOneLayer
                (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                 *(undefined4 *)PTR__gFadeToBlackSpriteLayerP_001c43e4);
      _SWProcessSpriteLayer
                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,
                 *(SpriteLayerPtr *)PTR__gCustomCursorSpriteLayerP_001c49f4);
      if (local_14 != (SpritePtr)0x0) {
        _SWProcessSpriteWorldForOneLayer
                  (*(undefined4 *)PTR__gStatsSpriteWorldP_001c48f4,
                   *(undefined4 *)PTR__gStatsFadeSpriteLayerP_001c49cc);
      }
    }
    else {
      _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
      if (local_14 != (SpritePtr)0x0) {
        _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gStatsSpriteWorldP_001c48f4);
      }
    }
    if (*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) == '\0') {
      _KeepMusicPlaying();
    }
    else {
      _ProcessSDLEvents();
    }
    _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  }
  _SWSetTileChangeProc(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_10);
  _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4);
  _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gStatsFadeSpriteLayerP_001c49cc);
  return;
}


// ---- _FadeToBlackMoveProc @ 0x37472 (146 bytes)

void FadeToBlackMoveProc(SpritePtr param_1)

{
  uint uVar1;
  
  uVar1 = _SWGetSpriteTranslucencyLevel(param_1);
  if (uVar1 < 0xffffU - param_1->userData) {
    _SWSetSpriteTranslucencyLevel(param_1,param_1->userData + uVar1);
    _SWInvalidateSprite(param_1);
  }
  else {
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)0x0);
    _SWSetSpriteFrameTime(param_1,-1);
    _SWSetSpriteTranslucencyLevel(param_1,0xffff);
  }
  return;
}


// ---- _FadeToNormalMoveProc @ 0x37504 (108 bytes)

void FadeToNormalMoveProc(SpritePtr param_1)

{
  uint uVar1;
  
  uVar1 = _SWGetSpriteTranslucencyLevel(param_1);
  if ((uint)param_1->userData < uVar1) {
    _SWSetSpriteTranslucencyLevel(param_1,uVar1 - param_1->userData);
    _SWInvalidateSprite(param_1);
  }
  else {
    _SWRemoveSprite(param_1);
    _SWDisposeSprite(&param_1);
  }
  return;
}


// ---- _IsCurrentlyFadingToBlack @ 0x37570 (92 bytes)

bool _IsCurrentlyFadingToBlack(void)

{
  SpritePtr pSVar1;
  bool bVar2;
  
  pSVar1 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4,(SpritePtr)0x0
                           );
  if (pSVar1 == (SpritePtr)0x0) {
    bVar2 = false;
  }
  else {
    bVar2 = pSVar1->spriteMoveProc != (MoveProcPtr_t)0x0;
  }
  return bVar2;
}


// ---- _NewFadeSprite @ 0x375cc (383 bytes)

SpritePtr _NewFadeSprite(int param_1,short param_2)

{
  SpritePtr local_14;
  SWError local_10;
  
  local_14 = (SpritePtr)0x0;
  local_10 = _SWCreateSprite(&local_14,(void *)0x0,0);
  if (local_10 == 0) {
    _SWSetSpriteDrawProc(local_14,(DrawProcPtr_t)PTR_SWSolidColorSpriteDrawProc_001c43ac);
    _SWSetRect(&local_14->destFrameRect,0,0,(int)*(short *)(param_1 + 0x38),
               (int)*(short *)(param_1 + 0x3a));
    if (param_1 == *(int *)PTR__gSpriteWorldP_001c40d0) {
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gFadeToBlackSpriteLayerP_001c43e4,local_14);
    }
    else {
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gStatsFadeSpriteLayerP_001c49cc,local_14);
    }
    _SWSetSpriteColor(local_14,0,0,0);
    _SWSetSpriteTranslucencyLevel(local_14,0);
    if (param_2 < 1) {
      _SWSetSpriteTranslucencyLevel(local_14,0xffff);
    }
    else {
      local_14->userData = (int)(0xffff / (longlong)(int)param_2);
      _SWSetSpriteMoveProc(local_14,(MoveProcPtr_t)PTR_FadeToBlackMoveProc_001c47e0);
      _SWSetSpriteMoveTime(local_14,0x10);
    }
  }
  return local_14;
}


// ---- _MMSetCursorForGame @ 0x3774b (83 bytes)

void _MMSetCursorForGame(void)

{
  if (*PTR__gUseCustomCursor_001c4974 == '\0') {
    _MMInitCursor();
  }
  else {
    _AddCustomCursorToScreen();
    *PTR__gCustomCursorIsVisible_001c472c = 1;
    if (*PTR__gCursorIsShielded_001c4890 == '\0') {
      _MMHideCursor();
    }
  }
  return;
}


// ---- _MMSetCursorForSystem @ 0x3779e (24 bytes)

void _MMSetCursorForSystem(void)

{
  _MMRemoveGameCursor();
  _MMInitCursor();
  return;
}


// ---- _MMRemoveGameCursor @ 0x377b6 (68 bytes)

void _MMRemoveGameCursor(void)

{
  if (*(int *)PTR__gSpriteWorldP_001c40d0 != 0) {
    _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gCustomCursorSpriteLayerP_001c49f4);
    *PTR__gCustomCursorIsVisible_001c472c = 0;
  }
  return;
}


// ---- _MMSaveCursorState @ 0x377fa (131 bytes)

void _MMSaveCursorState(void)

{
  undefined1 uVar1;
  SpritePtr pSVar2;
  
  if (*(int *)PTR__gSpriteWorldP_001c40d0 == 0) {
    *PTR__gSaveCustomCursorIsVisible_001c4788 = 0;
  }
  else {
    pSVar2 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gCustomCursorSpriteLayerP_001c49f4,
                              (SpritePtr)0x0);
    *PTR__gSaveCustomCursorIsVisible_001c4788 = pSVar2 != (SpritePtr)0x0;
  }
  uVar1 = _IsCursorVisible();
  *PTR__gSaveSystemCursorIsVisible_001c498c = uVar1;
  *PTR__gSaveSystemCursorIsShielded_001c4954 = *PTR__gCursorIsShielded_001c4890;
  return;
}


// ---- _MMRestoreCursorState @ 0x3787d (140 bytes)

void _MMRestoreCursorState(void)

{
  char cVar1;
  
  if (*PTR__gSaveCustomCursorIsVisible_001c4788 == '\0') {
    if (*PTR__gSaveSystemCursorIsVisible_001c498c == '\0') {
      _MMHideAllCursors();
    }
    else {
      _MMRemoveGameCursor();
      if (*PTR__gSaveSystemCursorIsShielded_001c4954 == '\0') {
        _MMSetCursorForSystem();
      }
      else {
        _MMHideCursor();
      }
    }
  }
  else {
    if (*PTR__gSaveSystemCursorIsShielded_001c4954 != '\0') {
      _MMShieldCursor();
    }
    _MMSetCursorForGame();
  }
  cVar1 = _CurrentScreenHasCustomCursor();
  if (cVar1 != '\0') {
    _MMSetCursorForGame();
    _MMShieldCursor();
  }
  return;
}


// ---- _MMHideAllCursors @ 0x37909 (73 bytes)

void _MMHideAllCursors(void)

{
  if (*(int *)PTR__gSpriteWorldP_001c40d0 != 0) {
    _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gCustomCursorSpriteLayerP_001c49f4);
  }
  *PTR__gCustomCursorIsVisible_001c472c = 0;
  _MMHideCursor();
  return;
}


// ---- _MMInitCursor @ 0x37952 (41 bytes)

void _MMInitCursor(void)

{
  *PTR__gCursorIsShielded_001c4890 = 0;
  _MyInitCursor();
  return;
}


// ---- _MMHideCursor @ 0x3797b (63 bytes)

void _MMHideCursor(void)

{
  if (*PTR__gFullScreenWindow_001c48ec == '\0') {
    _MMShieldCursor();
  }
  else {
    *PTR__gCursorIsShielded_001c4890 = 0;
    _MyHideCursor();
  }
  return;
}


// ---- _MMShieldCursor @ 0x379ba (41 bytes)

void _MMShieldCursor(void)

{
  *PTR__gCursorIsShielded_001c4890 = 1;
  _MMHandleCursorMovement();
  return;
}


// ---- _IsCursorShielded @ 0x379e3 (34 bytes)

undefined1 IsCursorShielded(void)

{
  return *PTR__gCursorIsShielded_001c4890;
}


// ---- _MMHandleCursorMovement @ 0x37a05 (160 bytes)

void _MMHandleCursorMovement(void)

{
  SpritePtr srcSpriteP;
  
  if (*PTR__gCursorIsShielded_001c4890 != '\0') {
    srcSpriteP = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gCustomCursorSpriteLayerP_001c49f4,
                                  (SpritePtr)0x0);
    if (*PTR__gCursorIsInWindow_001c47a4 == '\0') {
      _SDL_ShowCursor(1);
      if (srcSpriteP != (SpritePtr)0x0) {
        _SWSetSpriteVisible(srcSpriteP,'\0');
      }
    }
    else {
      _SDL_ShowCursor(0);
      if (srcSpriteP != (SpritePtr)0x0) {
        _SWSetSpriteVisible(srcSpriteP,'\x01');
      }
    }
  }
  return;
}


// ---- _CurrentScreenHasCustomCursor @ 0x37aa5 (118 bytes)

undefined1 _CurrentScreenHasCustomCursor(void)

{
  undefined1 local_10;
  
  if ((((*(int *)PTR__gCurrentGameMode_001c42d0 == 5) ||
       (*(int *)PTR__gCurrentGameMode_001c42d0 == 6)) ||
      (*(int *)PTR__gCurrentGameMode_001c42d0 == 0xe)) ||
     ((*(int *)PTR__gCurrentGameMode_001c42d0 == 0xb ||
      (*(int *)PTR__gCurrentGameMode_001c42d0 == 9)))) {
    local_10 = 0;
  }
  else {
    local_10 = 1;
  }
  return local_10;
}


// ---- _MyGameBeep @ 0x37b1b (42 bytes)

void MyGameBeep(void)

{
  _MMPlaySound(0,8,3);
  return;
}


// ---- _MMAssertFailCallback @ 0x37b45 (229 bytes)

void MMAssertFailCallback(char *param_1,undefined4 param_2,undefined4 param_3)

{
  char acStack_204 [500];
  char *pcStack_10;
  
  pcStack_10 = _strrchr(param_1,0x2f);
  pcStack_10 = pcStack_10 + 1;
  if (pcStack_10 == (char *)0x0) {
    pcStack_10 = param_1;
  }
  if (*(int *)PTR__gSpriteWorldP_001c40d0 != 0) {
    _sprintf(acStack_204,
             "Sorry! An assertion error has occurred. This program must now quit. Please write down this information and send it to \rsupport@actionsoft.com: \r\rFile: %s \rFunction: %s \rLine: %d\n"
             ,pcStack_10,param_2,param_3);
    if ((*(int *)(PTR__gSpriteArray_001c40d4 + 0x370) != 0) &&
       (*(int *)(PTR__gSpriteArray_001c40d4 + 0x368) != 0)) {
      _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,acStack_204,3);
                    /* WARNING: Subroutine does not return */
      _exit(1);
    }
    _printf(acStack_204);
  }
  return;
}


// ---- _MMFatalErrorCallback @ 0x37c2a (236 bytes)

void _MMFatalErrorCallback(undefined4 param_1,char *param_2,undefined4 param_3,undefined4 param_4)

{
  char local_204 [500];
  char *local_10;
  
  local_10 = _strrchr(param_2,0x2f);
  local_10 = local_10 + 1;
  if (local_10 == (char *)0x0) {
    local_10 = param_2;
  }
  if (*(int *)PTR__gSpriteWorldP_001c40d0 != 0) {
    _sprintf(local_204,
             "Sorry! A fatal error has occurred. Contact ActionSoft support for help, and send this information: \r\rError code: %d \rFile: %s \rFunction: %s \rLine: %d\n"
             ,param_1,local_10,param_3,param_4);
    if ((*(int *)(PTR__gSpriteArray_001c40d4 + 0x370) != 0) &&
       (*(int *)(PTR__gSpriteArray_001c40d4 + 0x368) != 0)) {
      _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,local_204,3);
                    /* WARNING: Subroutine does not return */
      _exit(1);
    }
    _printf(local_204);
  }
  return;
}


// ---- _ErrorMessage @ 0x37d16 (154 bytes)

void _ErrorMessage(char *param_1,int param_2)

{
  char local_200 [504];
  
  if (param_2 == 0) {
    _sprintf(local_200,param_1);
  }
  else {
    _sprintf(local_200,param_1,param_2);
  }
  if (*(int *)PTR__gSpriteWorldP_001c40d0 == 0) {
    _printf(local_200);
  }
  else {
    _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,local_200,3);
  }
  return;
}


