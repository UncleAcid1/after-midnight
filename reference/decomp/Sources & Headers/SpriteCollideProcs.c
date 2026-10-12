// Ghidra decompilation of functions from original source file:
//   Sources & Headers/SpriteCollideProcs.c

// ---- _CheckHeroWithLavaTiles @ 0x690f8 (617 bytes)

void _CheckHeroWithLavaTiles(void)

{
  SWBoolean SVar1;
  short sVar2;
  SpritePtr srcSpriteP;
  undefined *puVar3;
  
  srcSpriteP = (SpritePtr)_GetHeroSprite();
  if (srcSpriteP != (SpritePtr)0x0) {
    puVar3 = (undefined *)_SWGetSpriteMoveProc(srcSpriteP);
    if (puVar3 != PTR_HeroSinksIntoLavaMoveProc_001c4e48) {
      SVar1 = _SWCheckSpriteWithTiles
                        (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,srcSpriteP,(void *)0x0,
                         (SWRect *)&srcSpriteP->f_102,6,6,0x309,0x30c,'\0');
      if (SVar1 != '\0') {
        _WriteCStringToFile("Died by lava.\r");
        _SWSetSpriteDrawProc(srcSpriteP,(DrawProcPtr_t)PTR_ClipMainSpriteDrawProc_001c4e2c);
        _SWSetSpriteMoveProc(srcSpriteP,(MoveProcPtr_t)PTR_HeroSinksIntoLavaMoveProc_001c4e48);
        _SWSetSpriteFrameRange(srcSpriteP,0x33,0x36);
        _SWSetSpriteFlipX(srcSpriteP,srcSpriteP->f_177 == '\0');
        _SWSetSpriteFrameAdvance(srcSpriteP,0);
        _SWSetSpriteFrameTime(srcSpriteP,-1);
        _CreateSplashForSprite(srcSpriteP,1);
        _SWRemoveSprite(srcSpriteP);
        _SWAddSprite(*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,srcSpriteP);
        _KillHeroCommon(srcSpriteP);
        sVar2 = _GetLavaPixelRowFromSprite(srcSpriteP);
        *(short *)&srcSpriteP->f_1b4 = sVar2 + -3;
        _SWSetSpriteMoveDelta(srcSpriteP,0.0,2.3);
        srcSpriteP->f_1b8 = ')';
        srcSpriteP->f_1b9 = '\\';
        srcSpriteP->f_1ba = 0x8f;
        srcSpriteP->f_1bb = '?';
        _MMPlaySound(0x3c,9,1);
        _MMPlayStereoSound2(0,0x15,10,1,
                            (int)(short)(int)((double)(int)*(short *)PTR__gSoundVolume_001c48dc /
                                             1.5),0x3ff0000000000000,0);
      }
    }
  }
  return;
}


// ---- _CheckSpriteWithLavaTiles @ 0x69361 (958 bytes)

void _CheckSpriteWithLavaTiles(SpritePtr param_1,char param_2)

{
  int iVar1;
  SWBoolean SVar2;
  char cVar3;
  short sVar4;
  undefined *puVar5;
  longdouble extraout_ST0;
  char local_30;
  SWRect local_28;
  SWRect *local_20;
  char local_19;
  SpritePtr local_18;
  short local_12;
  SpritePtr local_10;
  
  local_30 = param_2;
  local_19 = '\0';
  if (param_1 == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c",
               PTR____FUNCTION___27445_001c4e04,0x48);
  }
  local_20 = (SWRect *)0x0;
  if (param_1->userData == 0) {
    local_18 = param_1;
    local_30 = '\0';
    local_19 = '\x01';
    local_20 = (SWRect *)&param_1->f_102;
    if (param_1->curFrameIndex == 0x32) {
      return;
    }
  }
  puVar5 = (undefined *)_SWGetSpriteMoveProc(param_1);
  if (puVar5 != PTR_SpriteSinksIntoLavaMoveProc_001c4ccc) {
    if ((0x4f < param_1->userData) && (param_1->userData < 0x5f)) {
      _SWGetSpriteWidth(param_1);
      local_12 = (short)(int)((float)extraout_ST0 / 2.0);
      _SWSetRect(&local_28,(int)(short)(local_12 + -4),0,(int)(short)(local_12 + 2),0);
      local_20 = &local_28;
    }
    SVar2 = _SWCheckSpriteWithTiles
                      (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,(void *)0x0,local_20,6
                       ,6,0x309,0x30c,'\0');
    if (SVar2 != '\0') {
      if (param_1 == (SpritePtr)0x0) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c"
                   ,PTR____FUNCTION___27445_001c4e04,0x72);
      }
      _SWSetSpriteDrawProc(param_1,(DrawProcPtr_t)PTR_ClipSpriteWithLavaDrawProc_001c4e58);
      _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_SpriteSinksIntoLavaMoveProc_001c4ccc);
      param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta / 65.0;
      param_1->drawData->horizMoveDelta = 0.0;
      if (param_1->drawData->vertMoveDelta < 0.0) {
        param_1->drawData->vertMoveDelta = 0.0;
      }
      if (((local_19 != '\0') &&
          (local_10 = param_1, sVar4._0_1_ = param_1->f_16c, sVar4._1_1_ = param_1->f_16d,
          sVar4 == 4)) &&
         (iVar1._0_1_ = param_1->f_1d0, iVar1._1_1_ = param_1->f_1d1, iVar1._2_1_ = param_1->f_1d2,
         iVar1._3_1_ = param_1->f_1d3, iVar1 != 0)) {
        _SWSetSpriteDrawProc
                  (*(SpritePtr *)&param_1->f_1d0,
                   (DrawProcPtr_t)PTR_ClipSpriteWithLavaDrawProc_001c4e58);
      }
      if (local_19 == '\0') {
        _CreateSplashForSprite(param_1,local_30);
      }
      else {
        _CreateSplashForSprite(param_1,1);
      }
      if ((local_30 == '\0') && (local_19 == '\0')) {
        _MMPlayStereoSoundAdjustedVol(param_1,0x3d,0xb,1,0x578,0x3ff0000000000000);
      }
      else {
        _MMPlayStereoSoundAdjustedVol(param_1,0x3c,0xb,1,0x578,0x3ff0000000000000);
      }
      if (local_30 != '\0') {
        _AddSpriteToObjectArray(param_1);
      }
      cVar3 = _IsSpriteAMedusaHead(param_1);
      if (cVar3 != '\0') {
        _SWSetSpriteFrameRange(param_1,3,5);
        sVar4 = _CountSounds(0x16);
        if (sVar4 < 2) {
          _MMPlaySound(0x16,0xb,1);
        }
      }
    }
  }
  return;
}


// ---- _IsSpriteAMedusaHead @ 0x6971f (65 bytes)

undefined1 _IsSpriteAMedusaHead(int param_1)

{
  undefined1 local_10;
  
  if ((*(int *)(param_1 + 0xe8) == 8) || (*(int *)(param_1 + 0xe8) == 9)) {
    local_10 = 1;
  }
  else {
    local_10 = 0;
  }
  return local_10;
}


// ---- _CheckDeadHeroWithLavaTiles @ 0x69760 (221 bytes)

void _CheckDeadHeroWithLavaTiles(void)

{
  short sVar1;
  short sVar2;
  short sVar3;
  SpritePtr local_14;
  
  local_14 = (SpritePtr)0x0;
  do {
    local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,local_14);
    if (local_14 != (SpritePtr)0x0) {
      if (local_14->userData != 0) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c"
                   ,PTR____FUNCTION___27532_001c4e1c,0xcb);
      }
      if ((local_14->f_17e == '\0') &&
         (((sVar1._0_1_ = local_14->f_16c, sVar1._1_1_ = local_14->f_16d, sVar1 == 6 ||
           (sVar2._0_1_ = local_14->f_16c, sVar2._1_1_ = local_14->f_16d, sVar2 == 3)) ||
          (sVar3._0_1_ = local_14->f_16c, sVar3._1_1_ = local_14->f_16d, sVar3 == 4)))) {
        _CheckSpriteWithLavaTiles(local_14,0);
      }
    }
  } while (local_14 != (SpritePtr)0x0);
  return;
}


// ---- _MainSpriteCollideProc @ 0x6983d (848 bytes)

void MainSpriteCollideProc(int param_1,SpritePtr param_2)

{
  bool bVar1;
  bool bVar2;
  char cVar3;
  undefined *puVar4;
  undefined4 uVar5;
  
  bVar2 = true;
  puVar4 = (undefined *)_SWGetSpriteMoveProc(param_2);
  if (((puVar4 != PTR_SpriteSinksIntoLavaMoveProc_001c4ccc) && (*(char *)(param_1 + 0x17a) == '\0'))
     && (cVar3 = _SWBestMaskCollision(param_1,param_2), cVar3 != '\0')) {
    if (*(short *)(PTR__gObjects_001c4910 + 0xc) < 1) {
      cVar3 = _IsSpriteAMedusaHead(param_2);
      if (cVar3 == '\0') {
        if (param_2->userData == 10) {
          _KillHeroAndDoSmallSpiderDeath(param_1,param_2);
          bVar2 = false;
        }
        else {
          _KillHeroAndDoSkeletonCollapse(param_1,1);
        }
      }
      else {
        _KillHeroAndDoStoneCollapse(param_1);
      }
      switch(param_2->userData) {
      case 1:
        _WriteCStringToFile("Died by Skull Spider.");
        break;
      case 2:
        _WriteCStringToFile("Died by Scorpion.");
        break;
      case 3:
        _WriteCStringToFile("Died by Eyeball.");
        break;
      case 4:
        _WriteCStringToFile("Died by Blue Raven.");
        break;
      case 5:
        _WriteCStringToFile("Died by Red Raven.");
        break;
      case 6:
      case 7:
        _WriteCStringToFile("Died by Snake.");
        break;
      case 8:
        _WriteCStringToFile("Died by Medusa (non-respawning).");
        break;
      case 9:
        _WriteCStringToFile("Died by Medusa (respawning).");
        break;
      case 10:
        _WriteCStringToFile("Died by Small Spider.");
        break;
      case 0xc:
        _WriteCStringToFile("Died by Skeleton Fish.");
        break;
      case 0xd:
        _WriteCStringToFile("Died by Walking Skeleton.");
      }
      _WriteCarriageReturn(1);
      bVar1 = false;
    }
    else {
      _RemoveItemFromStats(6);
      *(short *)(PTR__gObjects_001c4910 + 0xc) = *(short *)(PTR__gObjects_001c4910 + 0xc) + -1;
      if (*PTR__gFileIsOpen_001c4760 != '\0') {
        _fprintf(*(FILE **)PTR__gOutputFileP_001c4ba4,"Used shield. %d shields left.\r",
                 (int)*(short *)(PTR__gObjects_001c4910 + 0xc));
      }
      if (param_2->userData == 3) {
        _MMPlaySound(0x12,0xb,1);
      }
      else if ((param_2->userData == 4) || (param_2->userData == 5)) {
        _MMPlaySound(0x31,0xb,1);
      }
      else {
        _MMPlaySound(0x14,0xb,1);
      }
      if (param_2->userData == 10) {
        _DeleteWebForSmallSpider(param_2);
      }
      bVar1 = true;
    }
    if (bVar2) {
      _SWRemoveSpriteFromAnimation(param_2,'\x01');
      if ((param_2->userData != 9) || (!bVar1)) {
        _AddSpriteToObjectArray(param_2);
      }
      uVar5 = _NewPuffOfSmokeSprite();
      _SWCenterSpriteOverSprite(uVar5,param_2);
    }
  }
  return;
}


// ---- _ObjectSpriteCollideProc @ 0x69b8d (236 bytes)

void ObjectSpriteCollideProc(int param_1,int param_2)

{
  char cVar1;
  
  if (*(int *)(param_1 + 0xe8) < 0x1e) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c",
               PTR____FUNCTION___27632_001c4e60,0x17b);
  }
  if (0x2c < *(int *)(param_1 + 0xe8)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c",
               PTR____FUNCTION___27632_001c4e60,0x17c);
  }
  if (*(int *)(param_2 + 0xe8) != 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c",
               PTR____FUNCTION___27632_001c4e60,0x17d);
  }
  if (*(char *)(param_2 + 0x17a) == '\0') {
    cVar1 = _SWBestMaskCollision(param_1,param_2);
    if (cVar1 != '\0') {
      _GetObject(param_1);
    }
  }
  return;
}


// ---- _SecretSpriteCollideProc @ 0x69c79 (245 bytes)

void SecretSpriteCollideProc(SpritePtr param_1,int param_2)

{
  if (param_1->userData != 0xb8) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c",
               PTR____FUNCTION___27657_001c4e38,0x192);
  }
  if (*(int *)(param_2 + 0xe8) != 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c",
               PTR____FUNCTION___27657_001c4e38,0x193);
  }
  if (*(char *)(param_2 + 0x17a) == '\0') {
    *(int *)PTR__gNumSecretsFound_001c4990 = *(int *)PTR__gNumSecretsFound_001c4990 + 1;
    _MMPlaySound(0x4e,8,1);
    _WriteCStringToFile("Found secret!\r");
    _SWRemoveSpriteFromAnimation(param_1,'\x01');
    _AddSpriteToObjectArray(param_1);
  }
  return;
}


// ---- _HeroExitCollideProc @ 0x69d6e (162 bytes)

void HeroExitCollideProc(int param_1,int param_2)

{
  if (*(int *)(param_1 + 0xe8) != 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c",
               PTR____FUNCTION___27677_001c4e3c,0x1ad);
  }
  if (0xb7 < *(int *)(param_2 + 0xe8)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c",
               PTR____FUNCTION___27677_001c4e3c,0x1ae);
  }
  if (*PTR__gUsingEditor_001c48b0 == '\0') {
    *PTR__gBeatTheMansion_001c4888 = 1;
  }
  return;
}


// ---- _HeroSignCollideProc @ 0x69e10 (1208 bytes)

void HeroSignCollideProc(int param_1,int param_2)

{
  char cVar1;
  SpritePtr pSVar2;
  int iVar3;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  undefined1 auStack_242 [256];
  undefined2 uStack_142;
  undefined2 uStack_140;
  undefined1 uStack_13e;
  undefined1 uStack_13d;
  undefined2 uStack_13c;
  char acStack_139 [255];
  undefined2 uStack_3a;
  undefined2 uStack_38;
  undefined2 uStack_36;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  SpritePtr pSStack_24;
  int iStack_20;
  int iStack_1c;
  int iStack_18;
  undefined1 uStack_11;
  char *pcStack_10;
  
  iStack_20 = param_1;
  iStack_1c = param_2;
  iStack_18 = 0;
  if (*(int *)(param_1 + 0xe8) != 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c",
               PTR____FUNCTION___27704_001c4dfc,0x1f1);
  }
  pSVar2 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gSignContentsSpriteLayerP_001c490c,
                            (SpritePtr)0x0);
  if ((pSVar2 == (SpritePtr)0x0) && (*(char *)(iStack_1c + 0x115) == '\0')) {
    iStack_18 = _CreateSpriteFromPictResource
                          (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&pSStack_24,0,
                           (int)*(short *)(iStack_1c + 0x10e),(int)*(short *)(iStack_1c + 0x10e),1,4
                          );
    if (iStack_18 == 0) {
      _SWGetSpriteHeight(pSStack_24);
      *(short *)PTR__gOrigSignHeight_001c4e14 = (short)(int)extraout_ST0;
      _SWGetSpriteWidth(pSStack_24);
      *(short *)PTR__gOrigSignWidth_001c4e34 = (short)(int)extraout_ST0_00;
      _StopAllChannels();
      _MMPlaySound(0x40,9,1);
      uStack_11 = _IsMusicPlaying();
      _SWLockSprite(pSStack_24);
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gSignContentsSpriteLayerP_001c490c,pSStack_24);
      uStack_28._0_2_ = (pSStack_24->destFrameRect).bottom;
      uStack_28._2_2_ = (pSStack_24->destFrameRect).right;
      uStack_2c._0_2_ = (pSStack_24->destFrameRect).top;
      uStack_2c._2_2_ = (pSStack_24->destFrameRect).left;
      _SWCenterRect(&uStack_2c,*(int *)PTR__gSpriteWorldP_001c40d0 + 0x8c);
      _SWSetSpriteLocation(pSStack_24,(float)(int)uStack_2c._2_2_,(float)(int)(short)uStack_2c);
      if (uStack_28._2_2_ <= uStack_2c._2_2_) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c"
                   ,PTR____FUNCTION___27704_001c4dfc,0x214);
      }
      if ((short)uStack_28 <= (short)uStack_2c) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c"
                   ,PTR____FUNCTION___27704_001c4dfc,0x215);
      }
      uStack_3a = 0xf000;
      uStack_38 = 0xf000;
      uStack_36 = 0xf000;
      uStack_34 = uStack_2c;
      uStack_30 = uStack_28;
      _SWInsetRect(&uStack_34,0x1c,0x1c);
      cVar1 = _FontExists("Chalkboard");
      if (cVar1 == '\0') {
        _SetPascalString(auStack_242,"\x12Comic Sans MS Bold");
        uStack_140 = 0xe;
      }
      else {
        _SetPascalString(auStack_242,"\nChalkboard");
        uStack_140 = 0xf;
      }
      uStack_142 = 1;
      uStack_13e = 1;
      uStack_13d = 1;
      uStack_13c = 0;
      pcStack_10 = (char *)_MMGetCStringFromTEXTResource((int)*(short *)(iStack_1c + 0x10c));
      if (pcStack_10 == (char *)0x0) {
        _sprintf(acStack_139,
                 "Error: The text for this sign could not be found. Please make sure there is a file in the mansion\'s \'TEXT\' folder that is named %05d.txt"
                 ,(int)*(short *)(iStack_1c + 0x10c));
        pcStack_10 = acStack_139;
      }
      pSVar2 = pSStack_24;
      if (pcStack_10 != (char *)0x0) {
        iVar3 = _MyDrawTextBox(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,pcStack_10,auStack_242,
                               &uStack_34,&uStack_3a,
                               *(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,1);
        pSVar2->userData = iVar3;
        if (pSStack_24->userData != 0) {
          _SWRemoveSprite((SpritePtr)pSStack_24->userData);
          _SWAddSprite(*(SpriteLayerPtr *)PTR__gSignContentsSpriteLayerP_001c490c,
                       (SpritePtr)pSStack_24->userData);
        }
      }
      _free(pcStack_10);
      _ResetKeys();
      _WriteEventToMovieFile(0xb,1);
      *PTR__gReadingSignWasDismissed_001c4ae0 = 0;
      *(undefined4 *)PTR__gSaveGameModeAtSign_001c4e44 =
           *(undefined4 *)PTR__gCurrentGameMode_001c42d0;
      *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xe;
      *(undefined2 *)(iStack_20 + 0x1bc) = 0;
      *(undefined2 *)PTR__gSignScaledSize_001c4e10 = 0;
      _SWSetSpriteVisible(pSStack_24,'\0');
      _SWSetSpriteFrameProc(pSStack_24,(FrameProcPtr_t)PTR_LAB_001c4e54);
      _SWSetSpriteFrameTime(pSStack_24,0);
      _PauseAllSpriteLayers(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
      _SWUnpauseSpriteLayer(*(SpriteLayerPtr *)PTR__gSignContentsSpriteLayerP_001c490c);
    }
  }
  return;
}


// ---- _HeroTipCollideProc @ 0x6a2c8 (659 bytes)

void HeroTipCollideProc(int param_1,int param_2)

{
  short sStack_2c;
  short sStack_2a;
  ushort uStack_28;
  ushort uStack_26;
  int iStack_24;
  int iStack_20;
  SpritePtr pSStack_1c;
  SpritePtr pSStack_18;
  int iStack_14;
  ushort uStack_e;
  
  iStack_24 = param_1;
  iStack_20 = param_2;
  if (*(int *)(param_1 + 0xe8) != 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c",
               PTR____FUNCTION___27805_001c4e28,0x277);
  }
  if (*(int *)(param_2 + 0xe8) != 0x26) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c",
               PTR____FUNCTION___27805_001c4e28,0x278);
  }
  if ((*(char *)(iStack_20 + 0x115) != '\0') && (*(char *)(iStack_24 + 0x17e) != '\0')) {
    pSStack_18 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gTipContentsSpriteLayerP_001c46f8,
                                  (SpritePtr)0x0);
    if (pSStack_18 != (SpritePtr)0x0) {
      if ((pSStack_18->userData == 0x7edf) && (pSStack_18->misc2 == param_2)) {
        return;
      }
      _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gTipContentsSpriteLayerP_001c46f8);
    }
    if (*(short *)(iStack_20 + 0x10e) == 0x2008) {
      uStack_e = 0xf0;
    }
    else if (*(short *)(iStack_20 + 0x10e) == 0x2009) {
      uStack_e = 0x140;
    }
    else if (*(short *)(iStack_20 + 0x10e) == 0x200a) {
      uStack_e = 0x17c;
    }
    else {
      uStack_e = 0x1fe;
    }
    sStack_2c = 0;
    sStack_2a = 0;
    uStack_28 = uStack_e / 2;
    uStack_26 = uStack_e;
    _SWCenterRect(&sStack_2c,*(int *)PTR__gSpriteWorldP_001c40d0 + 0x8c);
    iStack_14 = _MMGetCStringFromTEXTResource((int)*(short *)(iStack_20 + 0x10c));
    if ((iStack_14 != 0) &&
       (pSStack_1c = (SpritePtr)_NewBitmapTextSprite((int)(short)uStack_e),
       pSStack_1c != (SpritePtr)0x0)) {
      _SetBMTextSpriteText(pSStack_1c,iStack_14);
      _SWSetSpriteLocation(pSStack_1c,(float)(int)sStack_2a,(float)(int)sStack_2c);
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gTipContentsSpriteLayerP_001c46f8,pSStack_1c);
      _SWSetSpriteScaledFactor(pSStack_1c,0x3ea3d70a,0x3ea3d70a);
      _SWSetSpriteMoveProc(pSStack_1c,(MoveProcPtr_t)PTR_LAB_001c4e50);
      pSStack_1c->misc1 = 0;
      pSStack_1c->misc2 = param_2;
      _SWSetSpriteTranslucency(pSStack_1c,0);
    }
  }
  return;
}


// ---- _HandleReadingSignCallback @ 0x6a55b (81 bytes)

void _HandleReadingSignCallback(void)

{
  _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
  if (*PTR__gReadingSignWasDismissed_001c4ae0 != '\0') {
    _ExitSignReadingIfPossible();
  }
  return;
}


// ---- _ExitSignReadingIfPossible @ 0x6a5ac (60 bytes)

void _ExitSignReadingIfPossible(void)

{
  if (*(short *)PTR__gSignScaledSize_001c4e10 < 100) {
    *PTR__gReadingSignWasDismissed_001c4ae0 = 0;
  }
  else {
    _CleanUpAfterSign();
  }
  return;
}


// ---- _CleanUpAfterSign @ 0x6a5e8 (181 bytes)

void _CleanUpAfterSign(void)

{
  SpritePtr srcSpriteP;
  
  srcSpriteP = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gSignContentsSpriteLayerP_001c490c,
                                (SpritePtr)0x0);
  if (srcSpriteP == (SpritePtr)0x0) {
    *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 5;
  }
  else {
    *(undefined2 *)PTR__gSignScaledSize_001c4e10 = 100;
    _SWSetSpriteFrameProc(srcSpriteP,(FrameProcPtr_t)PTR_SignScaleOutFrameProc_001c4e30);
    _SWSetSpriteFrameTime(srcSpriteP,0);
    _UnpauseAllSpriteLayers(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = *(undefined4 *)PTR__gSaveGameModeAtSign_001c4e44
    ;
    PTR__gKeys_001c4af8[7] = 0;
  }
  return;
}


// ---- _SignScaleInFrameProc @ 0x6a69d (631 bytes)

void SignScaleInFrameProc(SpritePtr param_1)

{
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  
  if (param_1->isVisible == '\0') {
    _SWSetSpriteVisible(param_1,'\x01');
  }
  *(short *)PTR__gSignScaledSize_001c4e10 = *(short *)PTR__gSignScaledSize_001c4e10 + 5;
  if (100 < *(short *)PTR__gSignScaledSize_001c4e10) {
    *(undefined2 *)PTR__gSignScaledSize_001c4e10 = 100;
  }
  _SWSetSpriteScaledWidthFactor(param_1,(float)(int)*(short *)PTR__gSignScaledSize_001c4e10 / 100.0)
  ;
  _SWSetSpriteScaledHeightFactor
            (param_1,(float)(int)*(short *)PTR__gSignScaledSize_001c4e10 / 100.0);
  uStack_10._0_2_ = (param_1->destFrameRect).bottom;
  uStack_10._2_2_ = (param_1->destFrameRect).right;
  uStack_14._0_2_ = (param_1->destFrameRect).top;
  uStack_14._2_2_ = (param_1->destFrameRect).left;
  _SWCenterRect(&uStack_14,*(int *)PTR__gSpriteWorldP_001c40d0 + 0x8c);
  _SWMoveSprite(param_1,(float)(int)uStack_14._2_2_,(float)(int)(short)uStack_14);
  if (param_1->userData != 0) {
    _SWSetSpriteScaledWidthFactor
              ((SpritePtr)param_1->userData,
               (float)(int)*(short *)PTR__gSignScaledSize_001c4e10 / 100.0);
    _SWSetSpriteScaledHeightFactor
              ((SpritePtr)param_1->userData,
               (float)(int)*(short *)PTR__gSignScaledSize_001c4e10 / 100.0);
    uStack_18 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x90);
    uStack_1c = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x8c);
    _SWAutoScaleRect(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&uStack_1c);
    uStack_10 = *(undefined4 *)(param_1->userData + 0x3a);
    uStack_14 = *(undefined4 *)(param_1->userData + 0x36);
    _SWCenterRect(&uStack_14,&uStack_1c);
    _SWMoveSprite((SpritePtr)param_1->userData,(float)(int)uStack_14._2_2_,
                  (float)(int)(short)uStack_14);
  }
  if (99 < *(short *)PTR__gSignScaledSize_001c4e10) {
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
    _SWSetSpriteFrameTime(param_1,-1);
  }
  return;
}


// ---- _SignScaleOutFrameProc @ 0x6a914 (577 bytes)

void SignScaleOutFrameProc(SpritePtr param_1)

{
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  
  *(short *)PTR__gSignScaledSize_001c4e10 = *(short *)PTR__gSignScaledSize_001c4e10 + -5;
  _SWSetSpriteScaledWidthFactor(param_1,(float)(int)*(short *)PTR__gSignScaledSize_001c4e10 / 100.0)
  ;
  _SWSetSpriteScaledHeightFactor
            (param_1,(float)(int)*(short *)PTR__gSignScaledSize_001c4e10 / 100.0);
  uStack_10._0_2_ = (param_1->destFrameRect).bottom;
  uStack_10._2_2_ = (param_1->destFrameRect).right;
  uStack_14._0_2_ = (param_1->destFrameRect).top;
  uStack_14._2_2_ = (param_1->destFrameRect).left;
  _SWCenterRect(&uStack_14,*(int *)PTR__gSpriteWorldP_001c40d0 + 0x8c);
  _SWMoveSprite(param_1,(float)(int)uStack_14._2_2_,(float)(int)(short)uStack_14);
  if (param_1->userData != 0) {
    _SWSetSpriteScaledWidthFactor
              ((SpritePtr)param_1->userData,
               (float)(int)*(short *)PTR__gSignScaledSize_001c4e10 / 100.0);
    _SWSetSpriteScaledHeightFactor
              ((SpritePtr)param_1->userData,
               (float)(int)*(short *)PTR__gSignScaledSize_001c4e10 / 100.0);
    uStack_18 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x90);
    uStack_1c = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x8c);
    _SWAutoScaleRect(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&uStack_1c);
    uStack_10 = *(undefined4 *)(param_1->userData + 0x3a);
    uStack_14 = *(undefined4 *)(param_1->userData + 0x36);
    _SWCenterRect(&uStack_14,&uStack_1c);
    _SWMoveSprite((SpritePtr)param_1->userData,(float)(int)uStack_14._2_2_,
                  (float)(int)(short)uStack_14);
  }
  if (*(short *)PTR__gSignScaledSize_001c4e10 < 2) {
    _SWRemoveSpriteFromAnimation((SpritePtr)param_1->userData,'\x01');
    _SWRemoveSpriteFromAnimation(param_1,'\x01');
  }
  return;
}


// ---- _MedusaCollideProc @ 0x6ab55 (303 bytes)

void MedusaCollideProc(SpritePtr param_1,undefined4 param_2)

{
  char cVar1;
  short sVar2;
  short sStack_e;
  
  cVar1 = _SWBestMaskCollision(param_1,param_2);
  if (cVar1 != '\0') {
    _SWRemoveSprite(param_1);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gDyingMedusaSpriteLayerP_001c49a8,param_1);
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4e24);
    param_1->f_0fc = '\x04';
    param_1->f_0fd = '\0';
    param_1->f_0fe = '\x04';
    param_1->f_0ff = '\0';
    if (param_1->userData == 8) {
      _AddSpriteToObjectArray(param_1);
    }
    param_1->f_110 = '\n';
    param_1->f_111 = 0xd7;
    param_1->f_112 = 0xa3;
    param_1->f_113 = ';';
    param_1->f_114 = '\0';
    param_1->f_115 = '\0';
    param_1->f_116 = '\0';
    param_1->f_117 = '\0';
    sStack_e = _FindSound(0xb);
    if (sStack_e == -1) {
      sStack_e = _FindSound(0xc);
    }
    if (sStack_e != -1) {
      _StopChannel((int)sStack_e);
    }
    sVar2 = _CountSounds(0x16);
    if (sVar2 < 2) {
      _MMPlaySound(0x16,0xb,1);
    }
  }
  return;
}


// ---- _BigSpiderCollideProc @ 0x6ac84 (479 bytes)

void BigSpiderCollideProc(int param_1,SpritePtr param_2)

{
  char cVar1;
  short sVar2;
  undefined4 uVar3;
  
  if ((*(int *)(param_1 + 0x10c) == 0) &&
     (cVar1 = _SWBestMaskCollision(param_2,param_1), cVar1 != '\0')) {
    if (param_2->userData == 0) {
      *(undefined2 *)(param_1 + 0x108) = 1;
      if (param_2->f_17a == '\0') {
        _KillHeroOnBigSpider(param_2,param_1);
        _MMPlaySound(0x15,0xb,1);
      }
    }
    else {
      if ((((param_2->userData != 5) && (param_2->userData != 4)) &&
          (cVar1 = _IsSpriteAMedusaHead(param_2), cVar1 == '\0')) && (param_2->userData != 3)) {
        return;
      }
      *(undefined2 *)(param_1 + 0x108) = 1;
      _SWRemoveSpriteFromAnimation(param_2,'\x01');
      _AddSpriteToObjectArray(param_1);
      _AddSpriteToObjectArray(param_2);
      uVar3 = _NewWebTrappedSprite(0xc4);
      *(undefined4 *)(param_1 + 0x10c) = uVar3;
      _MovePreyToSpiderLocation(uVar3,param_1);
      if ((param_2->userData == 5) || (param_2->userData == 4)) {
        _MMPlaySound(0x31,0xb,1);
      }
      else {
        cVar1 = _IsSpriteAMedusaHead(param_2);
        if (cVar1 == '\0') {
          if (param_2->userData == 3) {
            _MMPlaySound(0x12,0xb,1);
          }
        }
        else {
          sVar2 = _CountSounds(0x16);
          if (sVar2 < 2) {
            _MMPlaySound(0x16,0xb,1);
          }
        }
      }
    }
  }
  return;
}


// ---- _ZapperBeamSpriteCollideProc @ 0x6ae63 (151 bytes)

void ZapperBeamSpriteCollideProc(int param_1,int param_2)

{
  char cVar1;
  
  if (*(int *)(param_2 + 0xe8) != 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c",
               PTR____FUNCTION___28070_001c4e5c,0x3e2);
  }
  if ((*(char *)(param_1 + 0x32) != '\0') && (*(char *)(param_2 + 0x17a) == '\0')) {
    cVar1 = _SWBestMaskCollision(param_1,param_2);
    if (cVar1 != '\0') {
      _WriteCStringToFile("Died by zapper beam.\r");
      _KillHeroAndDoElectrocutionDeath(param_2);
    }
  }
  return;
}


// ---- _KillHero @ 0x6aefa (66 bytes)

void _KillHero(SpritePtr param_1)

{
  _KillHeroCommon(param_1);
  _SWRemoveSpriteFromAnimation(param_1,'\x01');
  *PTR__gRestartLevel_001c4858 = 1;
  return;
}


// ---- _KillHeroCommon @ 0x6af3c (239 bytes)

void _KillHeroCommon(int param_1)

{
  SpritePtr srcSpriteP;
  
  *(undefined1 *)(param_1 + 0x17a) = 1;
  _StopChannel(4);
  srcSpriteP = *(SpritePtr *)(param_1 + 0x1c8);
  if (srcSpriteP != (SpritePtr)0x0) {
    if (srcSpriteP->userData < 0xe) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c"
                 ,PTR____FUNCTION___28093_001c4e4c,0x415);
    }
    if (0x11 < srcSpriteP->userData) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c"
                 ,PTR____FUNCTION___28093_001c4e4c,0x416);
    }
    if (srcSpriteP->frameAdvance == -1) {
      _SWSetCurrentFrameIndex(srcSpriteP,3);
    }
    else {
      _SWSetCurrentFrameIndex(srcSpriteP,0);
    }
  }
  return;
}


// ---- _KillHeroAndDoSkeletonCollapse @ 0x6b02b (312 bytes)

void _KillHeroAndDoSkeletonCollapse(SpritePtr param_1,char param_2)

{
  if (param_2 != '\0') {
    _MMPlaySound(9,10,1);
  }
  _SWRemoveSprite(param_1);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,param_1);
  _SWSetSpriteFrameRange(param_1,0x2a,0x31);
  _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
  _SWSetCurrentFrameIndex(param_1,0x2a);
  _SWSetSpriteFrameAdvance(param_1,0);
  _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4c90);
  _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
  _SWSetSpriteFrameTime(param_1,0);
  param_1->f_192 = '\x14';
  param_1->f_193 = '\0';
  param_1->f_16c = '\x01';
  param_1->f_16d = '\0';
  *PTR__gHeroLandedThisFrame_001c4c70 = 0;
  _KillHeroCommon(param_1);
  return;
}


// ---- _KillHeroAndDoStoneCollapse @ 0x6b163 (285 bytes)

void _KillHeroAndDoStoneCollapse(SpritePtr param_1)

{
  _SWRemoveSprite(param_1);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,param_1);
  _SWSetSpriteFrameRange(param_1,0x3b,0x43);
  _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
  _SWSetCurrentFrameIndex(param_1,0x3b);
  _SWSetSpriteFrameAdvance(param_1,0);
  _MySetSpriteFrameTime(param_1,0);
  _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4c90);
  _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
  _SWResetSpriteFrameTimer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1);
  param_1->f_16c = '\x02';
  param_1->f_16d = '\0';
  *PTR__gHeroLandedThisFrame_001c4c70 = 0;
  _KillHeroCommon(param_1);
  return;
}


// ---- _KillHeroAndDoElectrocutionDeath @ 0x6b280 (336 bytes)

void _KillHeroAndDoElectrocutionDeath(SpritePtr param_1)

{
  _MMPlaySound(0x43,10,1);
  _SWRemoveSprite(param_1);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,param_1);
  _SWSetSpriteFrameRange(param_1,0x37,0x38);
  _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
  _SWSetCurrentFrameIndex(param_1,0x38);
  _SWSetSpriteFrameAdvance(param_1,0);
  _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_ElectrocutionDeathFrameProc_001c4e0c);
  _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4e40);
  _SWSetSpriteFrameTime(param_1,0);
  _SWSetSpriteMoveTime(param_1,0);
  param_1->f_196 = '\t';
  param_1->f_197 = '\0';
  param_1->f_198 = '\0';
  param_1->f_199 = '\0';
  param_1->f_1be = '\0';
  param_1->f_1bf = '\0';
  param_1->f_16c = '\x03';
  param_1->f_16d = '\0';
  *PTR__gHeroLandedThisFrame_001c4c70 = 0;
  return;
}


// ---- _KillHeroAndDoSmallSpiderDeath @ 0x6b3d0 (468 bytes)

void _KillHeroAndDoSmallSpiderDeath(SpritePtr param_1,SpritePtr param_2)

{
  if (param_1->userData != 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c",
               PTR____FUNCTION___28138_001c4e18,0x495);
  }
  if (param_2->userData != 10) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c",
               PTR____FUNCTION___28138_001c4e18,0x496);
  }
  _MMPlaySound(0x15,10,1);
  _SWRemoveSprite(param_1);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,param_1);
  _SWSetSpriteFrameRange(param_1,0x51,0x53);
  _SWSetSpriteFlipX(param_1,'\0');
  _SWSetCurrentFrameIndex(param_1,0x51);
  _SWSetSpriteFrameAdvance(param_1,1);
  _SWSetSpriteFrameAdvanceMode(param_1,(void *)0x1);
  _MySetSpriteFrameTime(param_1,500);
  _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)0x0);
  _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
  _SWSetSpriteMoveProc(param_2,(MoveProcPtr_t)PTR_SmallSpiderDragsHeroMoveProc_001c4e20);
  *(SpritePtr *)&param_2->f_10c = param_1;
  _AddSpriteToObjectArray(param_2);
  param_1->f_16c = '\x05';
  param_1->f_16d = '\0';
  _LayWebUpToCeiling(param_2,8,4,0x5a0);
  _KillHeroCommon(param_1);
  return;
}


// ---- _KillHeroOnBigSpider @ 0x6b5a4 (136 bytes)

void _KillHeroOnBigSpider(SpritePtr param_1,int param_2)

{
  undefined4 uVar1;
  
  _WriteCStringToFile("Died by big spider.\r");
  _KillHeroCommon(param_1);
  _SWRemoveSpriteFromAnimation(param_1,'\x01');
  uVar1 = _NewWebTrappedSprite(0xc3);
  _AddSpriteToObjectArray(param_2);
  _AddSpriteToDoOnceArray(param_2);
  *(undefined4 *)(param_2 + 0x10c) = uVar1;
  _MovePreyToSpiderLocation(uVar1,param_2);
  return;
}


// ---- _KillHeroForFallingTooFar @ 0x6b62c (307 bytes)

void _KillHeroForFallingTooFar(SpritePtr param_1)

{
  _MMPlaySound(0x20,10,1);
  _WriteCStringToFile("Died by falling too far.\r");
  _SWRemoveSprite(param_1);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,param_1);
  _SWSetSpriteFrameRange(param_1,0x26,0x29);
  _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
  _SWSetCurrentFrameIndex(param_1,0x26);
  _SWSetSpriteFrameAdvance(param_1,0);
  _SWSetSpriteFrameTime(param_1,0);
  param_1->f_196 = '\x05';
  param_1->f_197 = '\0';
  _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_LAB_001c4e08);
  _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4c90);
  param_1->f_16c = '\x04';
  param_1->f_16d = '\0';
  _KillHeroCommon(param_1);
  return;
}


// ---- _GetObject @ 0x6b75f (1424 bytes)

void _GetObject(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  char local_17;
  
  local_17 = '\0';
  sVar3 = (short)param_1->userData;
  switch(sVar3) {
  case 0x1e:
    *(int *)PTR__gScore_001c48a4 = *(int *)PTR__gScore_001c48a4 + 100;
    local_17 = '\x01';
    break;
  case 0x1f:
    *(int *)PTR__gScore_001c48a4 = *(int *)PTR__gScore_001c48a4 + 200;
    local_17 = '\x01';
    break;
  case 0x20:
    *(int *)PTR__gScore_001c48a4 = *(int *)PTR__gScore_001c48a4 + 0x32;
    local_17 = '\x01';
    break;
  case 0x21:
    *(int *)PTR__gScore_001c48a4 = *(int *)PTR__gScore_001c48a4 + 0x19;
    local_17 = '\x01';
    break;
  case 0x22:
    local_17 = _AddItemToStats(6);
    if (local_17 != '\0') {
      *(short *)(PTR__gObjects_001c4910 + 0xc) = *(short *)(PTR__gObjects_001c4910 + 0xc) + 1;
      _WriteCStringToFile("Got shield.\r");
    }
    break;
  case 0x23:
    if (PTR__gObjects_001c4910[0xf] == '\0') {
      local_17 = '\x01';
      _AddBackpackToStatsArea(1);
      _WriteCStringToFile("Got backpack.\r");
    }
    else {
      local_17 = '\x01';
      *(int *)PTR__gScore_001c48a4 = *(int *)PTR__gScore_001c48a4 + 500;
    }
    break;
  case 0x24:
    if (PTR__gObjects_001c4910[0xe] == '\0') {
      local_17 = '\x01';
      _GetLantern(param_1);
      _WriteCStringToFile("Got lantern.\r");
      if (*(SpritePtr *)&param_1->f_108 != (SpritePtr)0x0) {
        _SWRemoveSpriteFromAnimation(*(SpritePtr *)&param_1->f_108,'\x01');
      }
    }
    break;
  case 0x25:
    _WriteCStringToFile("Got map item.\r");
    _MMPlayStereoSound2(0,0x27,8,1,(int)*(short *)PTR__gSoundVolume_001c48dc,0x4000000000000000,0);
    sVar1._0_1_ = param_1->f_112;
    sVar1._1_1_ = param_1->f_113;
    sVar2._0_1_ = param_1->f_110;
    sVar2._1_1_ = param_1->f_111;
    _RevealGridInMap((int)sVar2,(int)sVar1);
    local_17 = '\x01';
    break;
  default:
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteCollideProcs.c",
               PTR____FUNCTION___28186_001c4e00,0x59a);
    break;
  case 0x27:
    local_17 = _AddItemToStats(0);
    if (local_17 != '\0') {
      *(short *)PTR__gObjects_001c4910 = *(short *)PTR__gObjects_001c4910 + 1;
      _WriteCStringToFile("Got red key.\r");
    }
    break;
  case 0x28:
    local_17 = _AddItemToStats(1);
    if (local_17 != '\0') {
      *(short *)(PTR__gObjects_001c4910 + 2) = *(short *)(PTR__gObjects_001c4910 + 2) + 1;
      _WriteCStringToFile("Got blue key.\r");
    }
    break;
  case 0x29:
    local_17 = _AddItemToStats(2);
    if (local_17 != '\0') {
      *(short *)(PTR__gObjects_001c4910 + 4) = *(short *)(PTR__gObjects_001c4910 + 4) + 1;
      _WriteCStringToFile("Got green key.\r");
    }
    break;
  case 0x2a:
    local_17 = _AddItemToStats(3);
    if (local_17 != '\0') {
      *(short *)(PTR__gObjects_001c4910 + 6) = *(short *)(PTR__gObjects_001c4910 + 6) + 1;
      _WriteCStringToFile("Got purple key.\r");
    }
    break;
  case 0x2b:
    local_17 = _AddItemToStats(4);
    if (local_17 != '\0') {
      *(short *)(PTR__gObjects_001c4910 + 8) = *(short *)(PTR__gObjects_001c4910 + 8) + 1;
      _WriteCStringToFile("Got yellow key.\r");
    }
    break;
  case 0x2c:
    local_17 = _AddItemToStats(5);
    if (local_17 != '\0') {
      *(short *)(PTR__gObjects_001c4910 + 10) = *(short *)(PTR__gObjects_001c4910 + 10) + 1;
      _WriteCStringToFile("Got master key.\r");
    }
  }
  if (local_17 != '\0') {
    if ((sVar3 < 0x27) || (0x2c < sVar3)) {
      if (sVar3 == 0x22) {
        _MMPlaySound(0x13,8,1);
      }
      else if ((sVar3 == 0x1e) || (sVar3 == 0x1f)) {
        _MMPlaySound(0x19,8,1);
      }
      else if (sVar3 == 0x23) {
        _MMPlaySound(0x27,8,1);
      }
      else if ((sVar3 == 0x21) || (sVar3 == 0x20)) {
        _MMPlaySound(0x37,8,3);
      }
      else if (sVar3 == 0x24) {
        _MMPlaySound(0x3e,8,3);
      }
    }
    else {
      _MMPlaySound(0,8,1);
    }
  }
  if (local_17 != '\0') {
    _SWRemoveSpriteFromAnimation(param_1,'\x01');
    _AddSpriteToObjectArray(param_1);
  }
  return;
}


// ---- _GetLantern @ 0x6bcef (420 bytes)

void _GetLantern(void)

{
  char cVar1;
  short sVar2;
  SpriteLayerPtr local_1c;
  SpritePtr local_18;
  
  if (PTR__gObjects_001c4910[0xe] == '\0') {
    PTR__gObjects_001c4910[0xe] = 1;
    cVar1 = *PTR__gRoomIsDark_001c483c;
    _AddLanternToStatsArea();
    _DisposeAllMapCachedRooms();
    if (cVar1 != '\0') {
      sVar2 = _GEGetLevelResID((int)*(short *)PTR__gCurrentLevel_001c49bc,1,200);
      _MMLoadExtraBackFrameForRoom(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)sVar2);
      _MakeRoomDarkOrLight(0,1);
      for (local_1c = _SWGetNextSpriteLayer
                                (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,(SpriteLayerPtr)0x0)
          ; local_1c != (SpriteLayerPtr)0x0;
          local_1c = _SWGetNextSpriteLayer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_1c))
      {
        for (local_18 = _SWGetNextSprite(local_1c,(SpritePtr)0x0); local_18 != (SpritePtr)0x0;
            local_18 = _SWGetNextSprite(local_1c,local_18)) {
          sVar2 = (short)local_18->userData;
          if ((((sVar2 != 0x4f) && (sVar2 != 0xb8)) && (sVar2 != 0xb9)) && (sVar2 != 0xba)) {
            if ((0x2c < sVar2) && (sVar2 < 0x45)) {
              _SWSetSpriteTranslucency(local_18,0xffff);
            }
            _SWSetSpriteVisible(local_18,'\x01');
          }
        }
      }
      _DrawLevel();
    }
  }
  return;
}


// ---- _CreateSplashForSprite @ 0x6be93 (281 bytes)

void _CreateSplashForSprite(undefined4 param_1,char param_2)

{
  short sVar1;
  SpritePtr pSVar2;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SWMovementType SVar3;
  undefined2 local_10;
  
  if (param_2 == '\0') {
    local_10 = 0xd0;
  }
  else {
    local_10 = 0xce;
  }
  sVar1 = _GetLavaPixelRowFromSprite(param_1);
  pSVar2 = (SpritePtr)_NewLavaSplashSprite(local_10);
  _SWCenterSpriteOverSprite(pSVar2,param_1);
  _SWGetSpriteHeight(pSVar2);
  SVar3 = _SWGetSpriteHorizLoc(pSVar2);
  _SWMoveSprite(pSVar2,SVar3,(float)(int)sVar1 - (float)extraout_ST0);
  pSVar2 = (SpritePtr)_NewLavaSplashSprite(local_10 + 1);
  _SWCenterSpriteOverSprite(pSVar2,param_1);
  _SWGetSpriteHeight(pSVar2);
  SVar3 = _SWGetSpriteHorizLoc(pSVar2);
  _SWMoveSprite(pSVar2,SVar3,(float)(int)sVar1 - (float)extraout_ST0_00);
  return;
}


// ---- _GetLavaPixelRowFromSprite @ 0x6bfac (237 bytes)

int _GetLavaPixelRowFromSprite(int param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  undefined2 local_14;
  undefined2 local_12;
  
  sVar1 = *(short *)(param_1 + 0x3a);
  if (sVar1 < 0) {
    sVar1 = sVar1 + 0xf;
  }
  sVar2 = *(short *)(param_1 + 0x3c);
  if (sVar2 < 0) {
    sVar2 = sVar2 + 0xf;
  }
  sVar3 = *(short *)(param_1 + 0x36);
  if (sVar3 < 0) {
    sVar3 = sVar3 + 0xf;
  }
  for (local_14 = (sVar3 >> 4) + -1; local_14 <= sVar1 >> 4; local_14 = local_14 + 1) {
    sVar3 = *(short *)(param_1 + 0x38);
    if (sVar3 < 0) {
      sVar3 = sVar3 + 0xf;
    }
    for (local_12 = (sVar3 >> 4) + -1; local_12 <= sVar2 >> 4; local_12 = local_12 + 1) {
      sVar3 = _MyGetTile(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,6,(int)local_14,(int)local_12);
      if ((0x308 < sVar3) && (sVar3 < 0x331)) goto LAB_0006c083;
    }
  }
LAB_0006c083:
  return (int)(short)(local_14 * 0x10 + -1);
}


