// Ghidra decompilation of functions from original source file:
//   Sources & Headers/GameSupport.c

// ---- _ClearCheatCodeCache @ 0x78990 (44 bytes)

void _ClearCheatCodeCache(void)

{
  *(undefined2 *)PTR__gNextFreeCheatCodeElement_001c501c = 0;
  *PTR__gCheatCodeCache_001c4ff4 = 0;
  return;
}


// ---- _AddKeyToCheatCodeCache @ 0x789bc (233 bytes)

void _AddKeyToCheatCodeCache(undefined1 param_1)

{
  short sVar1;
  short local_e;
  
  sVar1 = *(short *)PTR__gNextFreeCheatCodeElement_001c501c;
  PTR__gCheatCodeCache_001c4ff4[sVar1] = param_1;
  *(short *)PTR__gNextFreeCheatCodeElement_001c501c = sVar1 + 1;
  PTR__gCheatCodeCache_001c4ff4[*(short *)PTR__gNextFreeCheatCodeElement_001c501c] = 0;
  if (*(short *)PTR__gNextFreeCheatCodeElement_001c501c == 0x31) {
    for (local_e = 0; local_e < 0x31; local_e = local_e + 1) {
      PTR__gCheatCodeCache_001c4ff4[local_e] = PTR__gCheatCodeCache_001c4ff4[local_e + 1];
    }
    *(short *)PTR__gNextFreeCheatCodeElement_001c501c =
         *(short *)PTR__gNextFreeCheatCodeElement_001c501c + -1;
    PTR__gCheatCodeCache_001c4ff4[*(short *)PTR__gNextFreeCheatCodeElement_001c501c] = 0;
  }
  _CheckCheatCodes();
  return;
}


// ---- _CheckCheatCodes @ 0x78aa5 (175 bytes)

void _CheckCheatCodes(void)

{
  char cVar1;
  short local_10;
  short local_e;
  
  for (local_e = 0; local_e < 0x13; local_e = local_e + 1) {
    for (local_10 = 0; local_10 < *(short *)PTR__gNextFreeCheatCodeElement_001c501c;
        local_10 = local_10 + 1) {
      cVar1 = _CompareCStrings(*(undefined4 *)(PTR__gCheatCodeArray_001c5014 + local_e * 4),
                               PTR__gCheatCodeCache_001c4ff4 + local_10);
      if (cVar1 != '\0') {
        _DoCheatCode((int)local_e);
        _WriteEventToMovieFile((char)local_e + '\x0e',1);
        _ClearCheatCodeCache();
      }
    }
  }
  return;
}


// ---- _DoCheatCode @ 0x78b54 (1893 bytes)

void _DoCheatCode(undefined2 param_1)

{
  char cVar1;
  short sVar2;
  undefined4 uVar3;
  SpritePtr local_14;
  
  switch(param_1) {
  case 0:
    cVar1 = _AddItemToStats(0);
    if (cVar1 != '\0') {
      *PTR__gDidCheat_001c4774 = 1;
      *(short *)PTR__gObjects_001c4910 = *(short *)PTR__gObjects_001c4910 + 1;
      _WriteCStringToFile("CHEAT: Got red key.\r");
      _MMPlaySound(0,8,1);
    }
    break;
  case 1:
    cVar1 = _AddItemToStats(1);
    if (cVar1 != '\0') {
      *PTR__gDidCheat_001c4774 = 1;
      *(short *)(PTR__gObjects_001c4910 + 2) = *(short *)(PTR__gObjects_001c4910 + 2) + 1;
      _WriteCStringToFile("CHEAT: Got blue key.\r");
      _MMPlaySound(0,8,1);
    }
    break;
  case 2:
    cVar1 = _AddItemToStats(2);
    if (cVar1 != '\0') {
      *PTR__gDidCheat_001c4774 = 1;
      *(short *)(PTR__gObjects_001c4910 + 4) = *(short *)(PTR__gObjects_001c4910 + 4) + 1;
      _WriteCStringToFile("CHEAT: Got green key.\r");
      _MMPlaySound(0,8,1);
    }
    break;
  case 3:
    cVar1 = _AddItemToStats(3);
    if (cVar1 != '\0') {
      *PTR__gDidCheat_001c4774 = 1;
      *(short *)(PTR__gObjects_001c4910 + 6) = *(short *)(PTR__gObjects_001c4910 + 6) + 1;
      _WriteCStringToFile("CHEAT: Got purple key.\r");
      _MMPlaySound(0,8,1);
    }
    break;
  case 4:
    cVar1 = _AddItemToStats(4);
    if (cVar1 != '\0') {
      *PTR__gDidCheat_001c4774 = 1;
      *(short *)(PTR__gObjects_001c4910 + 8) = *(short *)(PTR__gObjects_001c4910 + 8) + 1;
      _WriteCStringToFile("CHEAT: Got yellow key.\r");
      _MMPlaySound(0,8,1);
    }
    break;
  case 5:
    *PTR__gDidCheat_001c4774 = 1;
    *(short *)PTR__gNumLivesLeft_001c49e4 = *(short *)PTR__gNumLivesLeft_001c49e4 + 1;
    _WriteCStringToFile("CHEAT: Got extra life.\r");
    _MMPlaySound(0x23,9,1);
    break;
  case 6:
    cVar1 = _IsMMRegistered(1);
    if (cVar1 != '\0') {
      _WriteCStringToFile("CHEAT: Testing Beat Mansion.\r");
      *PTR__gBeatTheMansion_001c4888 = 1;
      _MMPlaySound(0x23,9,1);
    }
    break;
  case 7:
    *PTR__gDidCheat_001c4774 = 1;
    _WriteCStringToFile("CHEAT: Revealed in-game map, but not secrets.\r");
    _MMPlayStereoSound2(0,0x27,8,1,(int)*(short *)PTR__gSoundVolume_001c48dc,0x4000000000000000,0);
    _RevealGridInMap(0x80,0x7fff);
    break;
  case 8:
  case 9:
    *PTR__gDidCheat_001c4774 = 1;
    _WriteCStringToFile("CHEAT: Got 500 points.\r");
    *(int *)PTR__gScore_001c48a4 = *(int *)PTR__gScore_001c48a4 + 500;
    _MMPlaySound(0x19,8,1);
    break;
  case 10:
    *PTR__gDidCheat_001c4774 = 1;
    _WriteCStringToFile("CHEAT: Revealed secrets on map.\r");
    _MMPlaySound(0x3e,8,1);
    _RevealSecretRooms();
    *PTR__gShowSecrets_001c4878 = 1;
    break;
  case 0xb:
    cVar1 = _IsMMRegistered(1);
    if ((cVar1 != '\0') && (cVar1 = _IsMoviePlaying(), cVar1 != '\0')) {
      *PTR__gDidCheat_001c4774 = 1;
      _WriteCStringToFile("CHEAT: Entered movie!\r");
      _CloseGameMovieFile();
      _ResetKeys();
      _MMPlaySound(0x48,8,1);
    }
    break;
  case 0xc:
    *PTR__gDidCheat_001c4774 = 1;
    _WriteCStringToFile("CHEAT: Re-used a save pedestal!\r");
    for (local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gSavePedestalSpriteLayerP_001c4138,
                                     (SpritePtr)0x0); local_14 != (SpritePtr)0x0;
        local_14 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gSavePedestalSpriteLayerP_001c4138,
                                    local_14)) {
      _SWSetCurrentFrameIndex(local_14,0);
      _RemoveSpriteFromObjectArray(local_14);
      _RemoveSpriteFromDoOnceArray(local_14);
    }
    _MMPlaySound(0x48,8,1);
    break;
  case 0xd:
    *PTR__gDidCheat_001c4774 = 1;
    _WriteCStringToFile("CHEAT: Got lantern for entire map.\r");
    _MMPlaySound(0x3e,8,1);
    _GetLantern(0);
    break;
  case 0xe:
    _SWSetSpriteWorldMaxFPS(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x78);
    break;
  case 0xf:
    _SWSetSpriteWorldMaxFPS(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0);
    break;
  case 0x10:
    _SWSetSpriteWorldMaxFPS(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x3c);
    break;
  case 0x11:
    *PTR__gGameOver_001c4718 = 1;
    break;
  case 0x12:
    uVar3 = _GetHeroSprite();
    _MMPlaySound(9,10,1);
    _KillHero(uVar3);
    break;
  case 0x13:
    *PTR__gArrowKeysAreEnabledForTesting_001c49f0 = 1;
    *PTR__gDidCheat_001c4774 = 1;
    break;
  case 0x14:
    _MMPlayStereoSound2(0,0x27,8,1,(int)*(short *)PTR__gSoundVolume_001c48dc,0x4000000000000000,0);
    _RevealEntireMap();
    *PTR__gDidCheat_001c4774 = 1;
    break;
  case 0x15:
    if ((*PTR__gMansionIsCustomMansion_001c48ac == '\0') &&
       (*(short *)PTR__gCurrentMansionNumber_001c4824 != -2)) {
      *PTR__gDidCheat_001c4774 = 0;
      _ResumeGame();
      sVar2 = _GetPercentSecrets();
      _CheckForHighScore((int)*(short *)PTR__gCurrentMansionNumber_001c4824,
                         (int)(char)*PTR__gDifficultySetting_001c4870,
                         *(undefined4 *)PTR__gScore_001c48a4,(int)sVar2,1);
    }
  }
  return;
}


// ---- _PlayMovieFile @ 0x792b9 (2038 bytes)

int _PlayMovieFile(undefined4 param_1)

{
  undefined4 uVar1;
  SWMovementType local_144;
  SWMovementType local_140;
  undefined4 local_13c;
  float local_138;
  undefined2 local_134;
  short local_132;
  undefined4 local_130;
  undefined4 local_12c;
  char local_127;
  char local_126;
  undefined1 local_125;
  int local_124;
  int local_120;
  char local_11b [255];
  char *local_1c;
  int local_18;
  SpritePtr local_14;
  SpritePtr local_10;
  
  local_138 = -1000.0;
  local_13c = 0xc47a0000;
  local_18 = 0;
  if (*PTR__gMovieFileIsOpen_001c500c != '\0') {
    _CloseGameMovieFile();
  }
  uVar1 = _SDL_RWFromFile(param_1,"rb");
  *(undefined4 *)PTR__gMovieFileRef_001c4ffc = uVar1;
  if (*(int *)PTR__gMovieFileRef_001c4ffc == 0) {
    _MMDoErrorMessage("Sorry, but the movie file couldn\'t be opened.",0);
    local_18 = -1;
  }
  if (local_18 == 0) {
    *PTR__gMovieFileIsOpen_001c500c = 1;
    _SetUpNewGame();
    _MMPauseGameTimer();
    _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,&local_120);
    _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,&local_130);
    _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,1,&local_126);
    _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,1,&local_125);
    if (local_126 == '\0') {
      local_18 = _LoadSaveDataFromFile
                           (*(undefined4 *)PTR__gMovieFileRef_001c4ffc,&local_138,&local_13c,
                            &local_127,&local_12c);
      if (local_18 == 0) {
        _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,1,
                            PTR__gSilverFloorIsActive_001c4958);
        _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,1,
                            PTR__gGoldFloorIsActive_001c47a8);
        _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,&local_140);
        _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,&local_144);
        _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,2,&local_134);
        _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,2,&local_132);
        _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,&local_124);
        if (local_124 != 0x47db) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameSupport.c",
                     PTR____FUNCTION___27742_001c4fb0,0x1bb);
        }
      }
    }
    else {
      _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,0xff,
                          PTR__gMansionFilename_001c489c);
      _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,1,
                          PTR__gDifficultySetting_001c4870);
      _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,2,
                          PTR__gCurrentMansionNumber_001c4824);
      _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,&local_124);
      if (local_124 != 0x47db) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameSupport.c",
                   PTR____FUNCTION___27742_001c4fb0,0x1a8);
      }
    }
  }
  if (local_120 < 0x6e) {
    _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                      "This replay is in the old non-HD format. It can\'t be played by Midnight Mansion HD."
                      ,3);
    local_18 = -2;
  }
  if (local_18 == 0) {
    _SetCurMansionAsCustomMansion(*(short *)PTR__gCurrentMansionNumber_001c4824 == -1);
    _strcpy(local_11b,PTR__gMansionFilename_001c489c);
    _StripStringFromEndOfCString(".zip",local_11b,0);
    _StripStringFromEndOfCString(".ZIP",local_11b,0);
    local_1c = (char *)_GetPathForAnyMansionFile(local_11b,*PTR__gMansionIsCustomMansion_001c48ac);
    _strcpy(PTR__gCustomMansionFolderPath_001c4fcc,local_1c);
    if (local_18 == 0) {
      local_18 = _OpenMansionUsingFilePath(local_1c,local_11b);
    }
    if ((((local_18 != 0) && (*PTR__gMansionIsCustomMansion_001c48ac != '\0')) && (local_18 != 0))
       && (*PTR__gMansionIsCustomMansion_001c48ac != '\0')) {
      uVar1 = _GetMansionFolderPath(PTR__gMansionFilename_001c489c);
      local_18 = _OpenMansionUsingFilePath(uVar1,PTR__gMansionFilename_001c489c);
    }
    if (local_18 == 0) {
      _ExitTitleScreen();
      *(undefined4 *)PTR__gNextFrame_001c5004 = 0;
      *PTR__gNextFrameIsInitialized_001c4fe0 = 0;
      *PTR__gPlayingGameMovie_001c4fd8 = 1;
      _SetEasyJumpingMode(local_125);
      _ResetKeys();
      if (local_126 == '\0') {
        if (local_138 == -1000.0) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameSupport.c",
                     PTR____FUNCTION___27742_001c4fb0,0x1f8);
        }
        _ChangeWorldRectForGame
                  (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                   (int)*(short *)PTR__gStatsHeight_001c47cc);
        _SWSetPostEraseCallBack
                  (*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,(CallBackPtr_t)PTR_LAB_001c4848);
        local_18 = _HandleRestoreStuff(local_138,local_13c,local_127,local_12c);
        if (local_127 == '\0') {
          local_14 = (SpritePtr)_GetHeroSprite();
          local_10 = local_14;
          _GetKeyEventsFromMovie();
          if (local_14 != (SpritePtr)0x0) {
            _SWSetSpriteMoveDelta(local_14,local_140,local_144);
            if (local_132 == 1) {
              _SWSetSpriteFrameRange(local_14,0x12,0x19);
              _SWSetSpriteMoveProc(local_14,(MoveProcPtr_t)PTR__ClimbingSpriteMoveProc_001c4d34);
              _SWSetSpriteFrameAdvanceMode(local_14,(void *)0x0);
              _SWSetSpriteFrameAdvance(local_14,1);
              _SWSetSpriteFrameProc(local_14,(FrameProcPtr_t)0x0);
              local_10->f_179 = '\0';
              local_10->f_172 = '\0';
            }
            else if (local_132 == 2) {
              _GetOnVine(local_14);
              local_10->f_1b0 = (undefined1)local_134;
              local_10->f_1b1 = local_134._1_1_;
            }
            else if (local_132 == 3) {
              _GetOnPole(local_14,0);
            }
            _SaveStartingPosition(local_14);
            if (local_132 == 1) {
              _ClimbingSpriteMoveProc(local_14);
            }
            else if (local_132 == 2) {
              _VineClimbingMoveProc(local_14);
            }
            else if (local_132 == 3) {
              _PoleSlidingMoveProc(local_14);
            }
          }
        }
        if (local_18 != 0) {
          _MyRestoreWorldRect(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
        }
      }
      else {
        _StartGameNoEditor();
      }
      _printf("Random number seed loaded from movie file: %d \n",local_130);
      _SelectStream(0);
      _PutSeed(local_130);
    }
  }
  return local_18;
}


// ---- _GetEventFromMovieFile @ 0x79aaf (442 bytes)

undefined1 _GetEventFromMovieFile(byte *param_1,undefined1 *param_2)

{
  byte local_12;
  undefined1 local_11;
  int local_10;
  
  local_10 = 0;
  local_11 = 0;
  if ((*PTR__gMovieFileIsOpen_001c500c != '\0') && (*PTR__gPlayingGameMovie_001c4fd8 != '\0')) {
    if (*PTR__gNextFrameIsInitialized_001c4fe0 == '\0') {
      local_10 = _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,
                                     PTR__gNextFrame_001c5004);
      *PTR__gNextFrameIsInitialized_001c4fe0 = 1;
    }
    if (*(int *)PTR__gNextFrame_001c5004 == *(int *)PTR__gRunningFrameCount_001c481c) {
      local_10 = _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,1,&local_12);
      if ((local_10 == 0) && (local_12 == 0x30)) {
        return 0x30;
      }
      if ((local_10 == 0) &&
         (local_10 = _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,
                                         PTR__gNextFrame_001c5004), local_10 != 0)) {
        return 0x30;
      }
      if (local_10 == 0) {
        if (local_12 < 0x7f) {
          *param_2 = 1;
          *param_1 = local_12;
        }
        else {
          *param_2 = 0;
          *param_1 = local_12 + 0x81;
        }
        local_11 = 1;
      }
    }
    else if (*(uint *)PTR__gNextFrame_001c5004 < *(uint *)PTR__gRunningFrameCount_001c481c) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameSupport.c",
                 PTR____FUNCTION___27880_001c4fa8,0x28c);
    }
  }
  if (local_10 == -1) {
    _StopGameMovie();
  }
  return local_11;
}


// ---- _GetDataFromMovieFile @ 0x79c69 (358 bytes)

undefined1 GetDataFromMovieFile(undefined1 *param_1,undefined4 param_2,undefined4 param_3)

{
  undefined1 uStack_12;
  undefined1 uStack_11;
  int iStack_10;
  
  iStack_10 = 0;
  uStack_11 = 0;
  if ((*PTR__gMovieFileIsOpen_001c500c != '\0') && (*PTR__gPlayingGameMovie_001c4fd8 != '\0')) {
    if (*PTR__gNextFrameIsInitialized_001c4fe0 == '\0') {
      iStack_10 = _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,
                                      PTR__gNextFrame_001c5004);
      *PTR__gNextFrameIsInitialized_001c4fe0 = 1;
    }
    if (*(int *)PTR__gNextFrame_001c5004 == *(int *)PTR__gRunningFrameCount_001c481c) {
      iStack_10 = _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,1,&uStack_12);
      if (iStack_10 == 0) {
        iStack_10 = _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,param_2,param_3);
      }
      if (iStack_10 == 0) {
        iStack_10 = _MyReadDataFromFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,
                                        PTR__gNextFrame_001c5004);
      }
      if (iStack_10 == 0) {
        *param_1 = uStack_12;
        uStack_11 = 1;
      }
    }
    else if (*(uint *)PTR__gNextFrame_001c5004 < *(uint *)PTR__gRunningFrameCount_001c481c) {
      _StopGameMovie();
    }
  }
  if (iStack_10 == -1) {
    _StopGameMovie();
  }
  return uStack_11;
}


// ---- _GetKeyEventsFromMovie @ 0x79dcf (536 bytes)

void _GetKeyEventsFromMovie(void)

{
  short sVar1;
  byte local_f;
  undefined1 local_e;
  char local_d;
  
  do {
    local_d = _GetEventFromMovieFile(&local_f,&local_e);
    if (local_d != '\0') {
      if (local_f == 0) {
        PTR__gKeys_001c4af8[3] = local_e;
      }
      else if (local_f == 1) {
        PTR__gKeys_001c4af8[1] = local_e;
      }
      else if (local_f == 3) {
        PTR__gKeys_001c4af8[2] = local_e;
      }
      else if (local_f == 2) {
        *PTR__gKeys_001c4af8 = local_e;
      }
      else if (local_f == 4) {
        PTR__gKeys_001c4af8[4] = local_e;
      }
      else if (local_f == 6) {
        PTR__gKeys_001c4af8[5] = local_e;
      }
      else if (local_f == 7) {
        PTR__gKeys_001c4af8[6] = local_e;
      }
      else if (local_f == 5) {
        PTR__gKeys_001c4af8[7] = local_e;
      }
      else if (local_f == 0xb) {
        _ResetKeys();
      }
      else if (local_f == 0xc) {
        _SetEasyJumpingMode(1);
      }
      else if (local_f == 0xd) {
        _SetEasyJumpingMode(0);
      }
      else if (local_f == 0x2d) {
        if (*PTR__gDemoMode_001c48b8 == '\0') {
          _MMPlaySound(0x4a,8,1);
        }
      }
      else if (local_f == 0x2e) {
        *PTR__gReadingSignWasDismissed_001c4ae0 = 1;
        _ExitSignReadingIfPossible();
      }
      else if ((local_f < 0xe) || (0x2c < local_f)) {
        if (local_f == 0x30) {
          _StopGameMovie();
        }
      }
      else {
        _DoCheatCode((int)(short)(local_f - 0xe));
        sVar1 = _FindSound(0x48);
        if (sVar1 == -1) {
          _MMPlaySound(0x48,8,1);
        }
      }
    }
  } while (local_d != '\0');
  return;
}


// ---- _AddKeyToMovie @ 0x79fe7 (593 bytes)

void _AddKeyToMovie(int param_1,char param_2,char param_3)

{
  if (*PTR__gArrowKeysEnabled_001c48a0 != '\0') {
    if (param_1 == 0x50) {
      _WriteEventToMovieFile(0,param_2);
    }
    else if (param_1 == 0x4f) {
      _WriteEventToMovieFile(1,param_2);
    }
    else if (param_1 == 0x51) {
      _WriteEventToMovieFile(3,param_2);
    }
    else if (param_1 == 0x52) {
      _WriteEventToMovieFile(2,param_2);
    }
    else if ((param_1 == 0x2c) && (param_3 == '\0')) {
      _WriteEventToMovieFile(4,param_2);
    }
  }
  if (*(int *)(PTR__gKeyPrefs_001c46f4 + 0xc) == param_1) {
    _WriteEventToMovieFile(0,param_2);
  }
  else if (*(int *)(PTR__gKeyPrefs_001c46f4 + 4) == param_1) {
    _WriteEventToMovieFile(1,param_2);
  }
  else if (*(int *)(PTR__gKeyPrefs_001c46f4 + 8) == param_1) {
    _WriteEventToMovieFile(3,param_2);
  }
  else if (*(int *)PTR__gKeyPrefs_001c46f4 == param_1) {
    _WriteEventToMovieFile(2,param_2);
  }
  else if (*(int *)(PTR__gKeyPrefs_001c46f4 + 0x10) == param_1) {
    _WriteEventToMovieFile(4,param_2);
  }
  else if (*(int *)(PTR__gKeyPrefs_001c46f4 + 0x14) == param_1) {
    _WriteEventToMovieFile(6,param_2);
  }
  else if (*(int *)(PTR__gKeyPrefs_001c46f4 + 0x18) == param_1) {
    _WriteEventToMovieFile(7,param_2);
  }
  else if (*(int *)(PTR__gKeyPrefs_001c46f4 + 0x1c) == param_1) {
    _WriteEventToMovieFile(5,param_2);
  }
  else if ((((*(int *)(PTR__gKeyPrefs_001c46f4 + 0x20) == param_1) &&
            (*PTR__gGameIsPaused_001c484c == '\0')) && (param_2 != '\0')) &&
          (*(int *)PTR__gCurrentGameMode_001c42d0 == 5)) {
    _WriteEventToMovieFile(0x2d,param_2);
  }
  return;
}


// ---- _OpenMovieFileForOutput @ 0x7a238 (1284 bytes)

/* WARNING: Type propagation algorithm not settling */

void _OpenMovieFileForOutput(char param_1,undefined1 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  undefined4 uVar2;
  char local_330 [4];
  undefined2 local_32c;
  undefined2 local_32a;
  undefined4 local_328;
  undefined4 local_324;
  time_t local_320 [3];
  char local_313 [255];
  char local_214 [500];
  undefined4 local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  
  local_330[0] = param_1;
  local_18 = 0;
  cVar1 = _IsMovieFileOpen();
  if (cVar1 != '\0') {
    cVar1 = _IsMovieFileOpenForRecording();
    if (cVar1 == '\0') {
      return;
    }
    _CloseGameMovieFile();
  }
  local_320[0] = _time((time_t *)0x0);
  _printf("Random number seed selected at start of movie: %d \n",local_320[0]);
  _SelectStream(0);
  _PutSeed(local_320[0]);
  local_20 = _GetMoviesFilePath(0);
  if (local_18 == 0) {
    _strcpy(local_313,PTR__gMansionFilename_001c489c);
    _StripStringFromEndOfCString(".zip",local_313,0);
    _StripStringFromEndOfCString(".ZIP",local_313,0);
    local_1c = _CountNumSuccessiveFiles(local_20,local_313,"");
    _sprintf(local_214,"%s%s %d",local_20,local_313,local_1c + 1);
  }
  _printf("Attempting to create movie file: %s\n",local_214);
  uVar2 = _SDL_RWFromFile(local_214,"wb");
  *(undefined4 *)PTR__gMovieFileRef_001c4ffc = uVar2;
  if (*(int *)PTR__gMovieFileRef_001c4ffc != 0) {
    _SetFileCreatorAndTypeForPath(local_214,0x4d414e53,0x4d4f5632);
    if (local_18 == 0) {
      *PTR__gMovieFileIsOpen_001c500c = 1;
      *PTR__gPlayingGameMovie_001c4fd8 = 0;
      local_320[2] = 0x6e;
      _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,local_320 + 2);
      _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,local_320);
      _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,1,local_330);
      _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,1,PTR__gEasyJumping_001c4994);
      if (local_330[0] == '\0') {
        _WriteSaveDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,param_3,param_4,param_2);
        _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,1,
                           PTR__gSilverFloorIsActive_001c4958);
        _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,1,
                           PTR__gGoldFloorIsActive_001c47a8);
        local_14 = _GetHeroSprite();
        local_32c = 0;
        if (local_14 == 0) {
          local_324 = 0;
          local_328 = 0;
          local_32a = 4;
        }
        else {
          local_324 = *(undefined4 *)(*(int *)(local_14 + 0xe0) + 0xc);
          local_328 = *(undefined4 *)(*(int *)(local_14 + 0xe0) + 0x10);
          local_32a = *(undefined2 *)(local_14 + 0x1b0);
        }
        local_10 = local_14;
        _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,&local_324);
        _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,&local_328);
        _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,2,&local_32a);
        if (local_14 != 0) {
          local_32c = _GetSpriteState(local_14);
        }
        _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,2,&local_32c);
        local_320[1] = 0x47db;
        _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,local_320 + 1);
      }
      else {
        _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,0xff,
                           PTR__gMansionFilename_001c489c);
        _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,1,
                           PTR__gDifficultySetting_001c4870);
        _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,2,
                           PTR__gCurrentMansionNumber_001c4824);
        local_320[1] = 0x47db;
        _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,local_320 + 1);
      }
    }
    _UpdateMenuBar();
  }
  return;
}


// ---- _WriteEventToMovieFile @ 0x7a73c (161 bytes)

void _WriteEventToMovieFile(char param_1,char param_2)

{
  char local_d [5];
  
  if ((*PTR__gMovieFileIsOpen_001c500c != '\0') && (*PTR__gPlayingGameMovie_001c4fd8 == '\0')) {
    _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,PTR__gRunningFrameCount_001c481c
                      );
    local_d[0] = param_1;
    if (param_2 == '\0') {
      local_d[0] = param_1 + '\x7f';
    }
    _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,1,local_d);
  }
  return;
}


// ---- _WriteDataToMovieFile @ 0x7a7dd (164 bytes)

void WriteDataToMovieFile(undefined1 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined1 auStack_10 [8];
  
  auStack_10[0] = param_1;
  if ((*PTR__gMovieFileIsOpen_001c500c != '\0') && (*PTR__gPlayingGameMovie_001c4fd8 == '\0')) {
    _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,4,PTR__gRunningFrameCount_001c481c
                      );
    _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,1,auStack_10);
    _MyWriteDataToFile(*(undefined4 *)PTR__gMovieFileRef_001c4ffc,param_2,param_3);
  }
  return;
}


// ---- _WriteCurrentKeysStateToMovie @ 0x7a881 (312 bytes)

void _WriteCurrentKeysStateToMovie(void)

{
  if (PTR__gKeys_001c4af8[3] != '\0') {
    _WriteEventToMovieFile(0,1);
  }
  if (PTR__gKeys_001c4af8[1] != '\0') {
    _WriteEventToMovieFile(1,1);
  }
  if (PTR__gKeys_001c4af8[2] != '\0') {
    _WriteEventToMovieFile(3,1);
  }
  if (*PTR__gKeys_001c4af8 != '\0') {
    _WriteEventToMovieFile(2,1);
  }
  if (PTR__gKeys_001c4af8[4] != '\0') {
    _WriteEventToMovieFile(4,1);
  }
  if (PTR__gKeys_001c4af8[5] != '\0') {
    _WriteEventToMovieFile(6,1);
  }
  if (PTR__gKeys_001c4af8[6] != '\0') {
    _WriteEventToMovieFile(7,1);
  }
  if (PTR__gKeys_001c4af8[7] != '\0') {
    _WriteEventToMovieFile(5,1);
  }
  return;
}


// ---- _StopGameMovie @ 0x7a9b9 (31 bytes)

void _StopGameMovie(void)

{
  *PTR__gEndGame_001c4798 = 1;
  return;
}


// ---- _IsMovieFileOpen @ 0x7a9d8 (34 bytes)

undefined1 _IsMovieFileOpen(void)

{
  return *PTR__gMovieFileIsOpen_001c500c;
}


// ---- _IsMoviePlaying @ 0x7a9fa (34 bytes)

undefined1 _IsMoviePlaying(void)

{
  return *PTR__gPlayingGameMovie_001c4fd8;
}


// ---- _IsMovieFileOpenForRecording @ 0x7aa1c (73 bytes)

undefined1 _IsMovieFileOpenForRecording(void)

{
  undefined1 local_10;
  
  if ((*PTR__gMovieFileIsOpen_001c500c == '\0') || (*PTR__gPlayingGameMovie_001c4fd8 != '\0')) {
    local_10 = 0;
  }
  else {
    local_10 = 1;
  }
  return local_10;
}


// ---- _CloseGameMovieFile @ 0x7aa65 (164 bytes)

void _CloseGameMovieFile(void)

{
  if (*PTR__gMovieFileIsOpen_001c500c != '\0') {
    if (*PTR__gPlayingGameMovie_001c4fd8 == '\0') {
      _WriteEventToMovieFile(0x30,0);
      _puts("Wrote kEndOfMovieEvent");
    }
    (**(code **)(*(int *)PTR__gMovieFileRef_001c4ffc + 0xc))
              (*(undefined4 *)PTR__gMovieFileRef_001c4ffc);
    *PTR__gMovieFileIsOpen_001c500c = 0;
    *PTR__gPlayingGameMovie_001c4fd8 = 0;
    *PTR__gNextFrameIsInitialized_001c4fe0 = 0;
    _puts("CloseGameMovieFile() called.");
  }
  return;
}


// ---- _OpenOutputFile @ 0x7ab09 (239 bytes)

void _OpenOutputFile(undefined4 param_1)

{
  FILE *pFVar1;
  short local_16;
  short local_14;
  short local_12;
  char *local_10;
  
  if (*PTR__gFileIsOpen_001c4760 != '\0') {
    _CloseOutputFile();
  }
  local_10 = (char *)_GetAppSupportPath(param_1);
  pFVar1 = _fopen(local_10,"w");
  *(FILE **)PTR__gOutputFileP_001c4ba4 = pFVar1;
  if (*(int *)PTR__gOutputFileP_001c4ba4 != 0) {
    *PTR__gFileIsOpen_001c4760 = 1;
    _MMGetCurrentDate(&local_12,&local_14,&local_16);
    _WriteCStringToFile("Date: ");
    _WriteNumberToFile((int)local_14);
    _WriteCStringToFile("/");
    _WriteNumberToFile((int)local_12);
    _WriteCStringToFile("/");
    _WriteNumberToFile((int)local_16);
    _WriteCarriageReturn(1);
  }
  return;
}


// ---- _MMGetCurrentDate @ 0x7abf8 (90 bytes)

void _MMGetCurrentDate(undefined2 *param_1,short *param_2,short *param_3)

{
  tm *ptVar1;
  time_t local_14 [4];
  
  local_14[0] = _time((time_t *)0x0);
  ptVar1 = _localtime(local_14);
  *param_1 = (short)ptVar1->tm_mday;
  *param_2 = (short)ptVar1->tm_mon + 1;
  *param_3 = (short)ptVar1->tm_year + 0x76c;
  return;
}


// ---- _WriteRoomInfoToFile @ 0x7ac52 (140 bytes)

void _WriteRoomInfoToFile(void)

{
  short sVar1;
  
  if (*PTR__gFileIsOpen_001c4760 != '\0') {
    sVar1 = _GEGetLevelResID((int)*(short *)PTR__gCurrentLevel_001c49bc,1,200);
    _WriteNumberToFile((int)sVar1);
    _WriteCStringToFile(" (");
    _WriteNumberToFile((int)*(short *)PTR__gNumLivesLeft_001c49e4);
    _WriteCStringToFile(" lives left.)\r");
  }
  return;
}


// ---- _WriteCarriageReturn @ 0x7acde (97 bytes)

void _WriteCarriageReturn(short param_1)

{
  undefined2 local_e;
  
  if (*PTR__gFileIsOpen_001c4760 != '\0') {
    for (local_e = 0; local_e < param_1; local_e = local_e + 1) {
      _fputc(0xd,*(FILE **)PTR__gOutputFileP_001c4ba4);
    }
  }
  return;
}


// ---- _WriteNumberToFile @ 0x7ad3f (75 bytes)

void _WriteNumberToFile(undefined4 param_1)

{
  if (*PTR__gFileIsOpen_001c4760 != '\0') {
    _fprintf(*(FILE **)PTR__gOutputFileP_001c4ba4,"%d",param_1);
  }
  return;
}


// ---- _WriteCStringToFile @ 0x7ad8a (65 bytes)

void _WriteCStringToFile(char *param_1)

{
  if (*PTR__gFileIsOpen_001c4760 != '\0') {
    _fputs(param_1,*(FILE **)PTR__gOutputFileP_001c4ba4);
  }
  return;
}


// ---- _IsOutputFileOpen @ 0x7adcb (34 bytes)

undefined1 IsOutputFileOpen(void)

{
  return *PTR__gFileIsOpen_001c4760;
}


// ---- _CloseOutputFile @ 0x7aded (81 bytes)

void _CloseOutputFile(void)

{
  if (*PTR__gFileIsOpen_001c4760 != '\0') {
    _WriteCarriageReturn(2);
    _fclose(*(FILE **)PTR__gOutputFileP_001c4ba4);
    *PTR__gFileIsOpen_001c4760 = 0;
  }
  return;
}


// ---- _SuspendGame @ 0x7ae3e (639 bytes)

void _SuspendGame(void)

{
  char cVar1;
  undefined1 local_522 [500];
  char local_32e [255];
  char local_22f [255];
  char local_130 [256];
  undefined4 local_30;
  undefined4 local_2c;
  undefined2 local_28;
  undefined2 local_26;
  SpritePtr local_24;
  SpritePtr local_20;
  int local_1c;
  SWMovementType local_18;
  SWMovementType local_14;
  int local_10;
  
  local_10 = 0;
  local_24 = (SpritePtr)_GetHeroForSuspendGame();
  if (local_24 != (SpritePtr)0x0) {
    local_20 = local_24;
    cVar1 = _IsMoviePlaying();
    if (cVar1 == '\0') {
      _strcpy(local_32e,PTR__gMansionFilename_001c489c);
      _StripStringFromEndOfCString(".zip",local_32e,0);
      _StripStringFromEndOfCString(".ZIP",local_32e,0);
      _GetPascalStringFromCString(local_130,local_32e);
      _SWConcatPascalStrings(local_130,"\n Suspended");
      _SWCopyPascalStringToCString(local_130,local_22f);
      _GetSuspendedGameFilePath(local_522,local_22f);
      if (local_10 == 0) {
        local_1c = _SDL_RWFromFile(local_522,"w+");
        if (local_1c == 0) {
          local_10 = -1;
          _MMDoErrorMessage("Sorry, but the suspended game couldn\'t be created.",0);
        }
      }
      local_18 = _SWGetSpriteHorizLoc(local_20);
      local_14 = _SWGetSpriteVertLoc(local_20);
      if (local_10 == 0) {
        local_2c = 100;
        _MyWriteDataToFile(local_1c,4,&local_2c);
        _WriteSaveDataToFile(local_1c,local_18,local_14,0);
        local_26._0_1_ = local_24->f_1b0;
        local_26._1_1_ = local_24->f_1b1;
        _MyWriteDataToFile(local_1c,2,&local_26);
        local_28 = _GetSpriteState(local_20);
        _MyWriteDataToFile(local_1c,2,&local_28);
        local_30 = 0x47db;
        _MyWriteDataToFile(local_1c,4,&local_30);
      }
      if (local_10 == 0) {
        *PTR__gSuspendedGameExists_001c4c34 = 1;
        _GetHFSUniFromCString(PTR__gSuspendedGameFilenameHFSUni_001c4b84,local_22f);
        (**(code **)(local_1c + 0xc))(local_1c);
        _SetFileCreatorAndTypeForPath(local_522,0x4d414e53,0x53555350);
        _WriteProfileToDisk();
      }
    }
  }
  return;
}


// ---- _OpenSuspendedGame @ 0x7b0bd (987 bytes)

int _OpenSuspendedGame(undefined4 param_1)

{
  undefined4 local_34;
  float local_30;
  undefined2 local_2a;
  short local_28;
  undefined1 local_25;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  SpritePtr local_14;
  SpritePtr local_10;
  
  local_1c = 0;
  local_30 = -1000.0;
  local_34 = 0xc47a0000;
  local_18 = _SDL_RWFromFile(param_1,"rb");
  if (local_18 == 0) {
    local_1c = -1;
    _printf("Suspended game file not found: %s \n",param_1);
  }
  else {
    _SetUpNewGame();
    _MMPauseGameTimer();
    _MyReadDataFromFile(local_18,4,&local_20);
    if (local_20 != 100) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameSupport.c",
                 PTR____FUNCTION___28459_001c5010,0x5ba);
    }
    local_1c = _LoadSaveDataFromFile(local_18,&local_30,&local_34,&local_25,&local_20);
    if (local_1c == 0) {
      _MyReadDataFromFile(local_18,2,&local_2a);
      _MyReadDataFromFile(local_18,2,&local_28);
      _MyReadDataFromFile(local_18,4,&local_24);
      if (local_24 != 0x47db) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameSupport.c",
                   PTR____FUNCTION___28459_001c5010,0x5c5);
      }
    }
  }
  if ((local_1c == 0) &&
     (_SetCurMansionAsCustomMansion(*(short *)PTR__gCurrentMansionNumber_001c4824 == -1),
     local_1c == 0)) {
    _ResetKeys();
    if (local_30 == -1000.0) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameSupport.c",
                 PTR____FUNCTION___28459_001c5010,0x5d9);
    }
    _ChangeWorldRectForGame
              (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)*(short *)PTR__gStatsHeight_001c47cc)
    ;
    _SWSetPostEraseCallBack
              (*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,(CallBackPtr_t)PTR_LAB_001c4848);
    local_1c = _HandleRestoreStuff(local_30,local_34,local_25,local_20);
    if (local_1c == 0) {
      local_14 = (SpritePtr)_GetHeroSprite();
      local_10 = local_14;
      if (*PTR__gMusicIsOn_001c47c0 != '\0') {
        _ResumeMusic(1);
      }
      if (local_14 != (SpritePtr)0x0) {
        if (local_28 == 1) {
          _SWSetSpriteFrameRange(local_14,0x12,0x19);
          _SWSetSpriteMoveProc(local_14,(MoveProcPtr_t)PTR__ClimbingSpriteMoveProc_001c4d34);
          local_10->f_179 = '\0';
          local_10->f_172 = '\0';
        }
        else if (local_28 == 2) {
          _GetOnVine(local_14);
          local_10->f_1b0 = (undefined1)local_2a;
          local_10->f_1b1 = local_2a._1_1_;
        }
        else if (local_28 == 3) {
          _GetOnPole(local_14,0);
        }
        _SaveStartingPosition(local_14);
        _SWSetSpriteMoveProc(local_14,(MoveProcPtr_t)PTR_LAB_001c4cbc);
        _SWSetSpriteFrameProc(local_14,(FrameProcPtr_t)PTR_LAB_001c4cb0);
        _SWSetSpriteFrameTime(local_14,0xfa);
        _SWSetSpriteFrameAdvance(local_14,0);
        _SWRemoveSprite(local_14);
        _SWAddSprite(*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,local_14);
      }
    }
    if (local_1c != 0) {
      _MyRestoreWorldRect(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    }
  }
  (**(code **)(local_18 + 0xc))(local_18);
  return local_1c;
}


// ---- _GetHeroForSuspendGame @ 0x7b498 (97 bytes)

SpritePtr _GetHeroForSuspendGame(void)

{
  SpritePtr local_10;
  
  local_10 = (SpritePtr)_GetHeroSprite();
  if (local_10 == (SpritePtr)0x0) {
    do {
      local_10 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,local_10);
      if (local_10 == (SpritePtr)0x0) {
        return (SpritePtr)0x0;
      }
    } while (local_10->spriteMoveProc != (MoveProcPtr_t)PTR_LAB_001c4cbc);
  }
  return local_10;
}


// ---- _CanSuspendGame @ 0x7b4f9 (220 bytes)

undefined1 _CanSuspendGame(void)

{
  char cVar1;
  int iVar2;
  undefined1 local_11;
  
  cVar1 = _IsMoviePlaying();
  if (cVar1 == '\0') {
    local_11 = 0;
    iVar2 = _GetHeroForSuspendGame();
    if ((((iVar2 != 0) && (*(int *)(iVar2 + 0x1c4) == 0)) &&
        (((*(char *)(iVar2 + 0x17e) != '\0' ||
          (((*(undefined **)(iVar2 + 200) == PTR__ClimbingSpriteMoveProc_001c4d34 ||
            (*(undefined **)(iVar2 + 200) == PTR__VineClimbingMoveProc_001c4d6c)) ||
           (*(undefined **)(iVar2 + 200) == PTR__PoleSlidingMoveProc_001c4d44)))) ||
         (*(undefined **)(iVar2 + 200) == PTR_LAB_001c4cbc)))) &&
       (*(undefined **)(iVar2 + 200) != PTR__FinishClimbingLadderMoveProc_001c4da4)) {
      local_11 = 1;
    }
  }
  else {
    local_11 = 0;
  }
  return local_11;
}


// ---- _DeleteSuspendedGame @ 0x7b5d5 (55 bytes)

void _DeleteSuspendedGame(char *param_1)

{
  _remove(param_1);
  *PTR__gSuspendedGameExists_001c4c34 = 0;
  _WriteProfileToDisk();
  return;
}


// ---- _CreateNewProfile @ 0x7b60c (171 bytes)

void _CreateNewProfile(char *param_1)

{
  char local_10b [259];
  
  *PTR__gPlayTutorialMansion_001c4bdc = 1;
  *PTR__gSuspendedGameExists_001c4c34 = 0;
  *PTR__gHasShownOrganScreen_001c4714 = 0;
  _InitCompletedMansionsArray();
  _SWCopyPascalString(param_1,PTR__gDefaultHighScoreName_001c4fe4);
  _SWCopyPascalString(param_1,PTR__gCurrentPlayerName_001c48e8);
  *PTR__gCurrentPlayerNameExists_001c4830 = 1;
  _WritePreferencesFile();
  _WriteProfileToDisk();
  _SWCopyPascalStringToCString(param_1,local_10b);
  _CreateSubdirectoriesForProfile(local_10b);
  return;
}


// ---- _CreateSubdirectoriesForProfile @ 0x7b6b7 (118 bytes)

void _CreateSubdirectoriesForProfile(undefined4 param_1)

{
  undefined4 uVar1;
  undefined1 local_200 [508];
  
  _GetPlainSuspendedGameFilePath(local_200,0);
  uVar1 = _GetPlainSavedGamesFilePath(0);
  _CreateDirectory(uVar1,param_1);
  _CreateDirectory(local_200,param_1);
  uVar1 = _GetUserMoviesPath(0);
  _CreateDirectory(uVar1,param_1);
  return;
}


// ---- _WriteProfileToDisk @ 0x7b72d (381 bytes)

void _WriteProfileToDisk(void)

{
  char local_11b [255];
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  int local_10;
  
  _SWCopyPascalStringToCString(PTR__gCurrentPlayerName_001c48e8,local_11b);
  local_14 = _GetPlayerProfilePath(local_11b);
  local_10 = _SDL_RWFromFile(local_14,"w+");
  if (local_10 != 0) {
    local_18 = 0x66;
    _MyWriteDataToFile(local_10,4,&local_18);
    _MyWriteDataToFile(local_10,1,PTR__gPlayTutorialMansion_001c4bdc);
    _MyWriteDataToFile(local_10,1,PTR__gSuspendedGameExists_001c4c34);
    _MyWriteDataToFile(local_10,0x200,PTR__gSuspendedGameFilenameHFSUni_001c4b84);
    _MyWriteDataToFile(local_10,1,PTR__gHasShownOrganScreen_001c4714);
    _MyWriteDataToFile(local_10,0x100,PTR__gDefaultHighScoreName_001c4fe4);
    _WriteMansionArrayToFile(local_10);
    local_1c = 0x47db;
    _MyWriteDataToFile(local_10,4,&local_1c);
    (**(code **)(local_10 + 0xc))(local_10);
    _SetFileCreatorAndTypeForPath(local_14,0x4d414e53,0x504c5952);
  }
  return;
}


// ---- _LoadProfileFromDisk @ 0x7b8aa (569 bytes)

undefined4 _LoadProfileFromDisk(char *param_1)

{
  char local_11f [255];
  int local_20;
  int local_1c;
  int local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  local_10 = 0;
  _SWCopyPascalStringToCString(param_1,local_11f);
  local_14 = _GetPlayerProfilePath(local_11f);
  local_18 = _SDL_RWFromFile(local_14,"rb");
  if (local_18 == 0) {
    local_10 = 0xffffffff;
  }
  else {
    _MyReadDataFromFile(local_18,4,&local_1c);
    if (local_1c < 0x65) {
      (**(code **)(local_18 + 0xc))(local_18);
      *PTR__gCurrentPlayerNameExists_001c4830 = 0;
      if (local_1c == 100) {
        return 0xfffffffe;
      }
      return 0xffffffff;
    }
    _MyReadDataFromFile(local_18,1,PTR__gPlayTutorialMansion_001c4bdc);
    _MyReadDataFromFile(local_18,1,PTR__gSuspendedGameExists_001c4c34);
    _MyReadDataFromFile(local_18,0x200,PTR__gSuspendedGameFilenameHFSUni_001c4b84);
    _MyReadDataFromFile(local_18,1,PTR__gHasShownOrganScreen_001c4714);
    if (local_1c < 0x66) {
      _SWCopyPascalString(param_1,PTR__gDefaultHighScoreName_001c4fe4);
    }
    else {
      _MyReadDataFromFile(local_18,0x100,PTR__gDefaultHighScoreName_001c4fe4);
    }
    _ReadMansionArrayFromFile(local_18);
    _MyReadDataFromFile(local_18,4,&local_20);
    if (local_20 != 0x47db) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameSupport.c",
                 PTR____FUNCTION___28608_001c4fac,0x6fb);
    }
    (**(code **)(local_18 + 0xc))(local_18);
    *PTR__gCurrentPlayerNameExists_001c4830 = 1;
    _SWCopyPascalString(param_1,PTR__gCurrentPlayerName_001c48e8);
    _CreateSubdirectoriesForProfile(local_11f);
  }
  return local_10;
}


// ---- _DeleteProfile @ 0x7bae3 (73 bytes)

void _DeleteProfile(undefined4 param_1)

{
  char *pcVar1;
  undefined4 uVar2;
  
  pcVar1 = (char *)_GetPlayerProfilePath(param_1);
  _remove(pcVar1);
  uVar2 = _GetPlayerProfilePath(param_1);
  _printf("Removing file: %s \n",uVar2);
  return;
}


// ---- _SaveCurrentProfile @ 0x7bb2c (85 bytes)

void _SaveCurrentProfile(void)

{
  *PTR__gProfileWasSaved_001c4fd0 = 1;
  *PTR__gSaveCurrentPlayerNameExists_001c4fec = *PTR__gCurrentPlayerNameExists_001c4830;
  _SWCopyPascalString(PTR__gCurrentPlayerName_001c48e8,PTR__gSaveCurrentPlayerName_001c4fdc);
  return;
}


// ---- _RestoreCurrentProfile @ 0x7bb81 (148 bytes)

void _RestoreCurrentProfile(void)

{
  char cVar1;
  
  if (*PTR__gProfileWasSaved_001c4fd0 != '\0') {
    cVar1 = _SWEqualString(PTR__gCurrentPlayerName_001c48e8,PTR__gSaveCurrentPlayerName_001c4fdc);
    if (cVar1 == '\0') {
      *PTR__gCurrentPlayerNameExists_001c4830 = *PTR__gSaveCurrentPlayerNameExists_001c4fec;
      _SWCopyPascalString(PTR__gSaveCurrentPlayerName_001c4fdc,PTR__gCurrentPlayerName_001c48e8);
      *PTR__gProfileWasSaved_001c4fd0 = 0;
      _LoadProfileFromDisk(PTR__gSaveCurrentPlayerName_001c4fdc);
    }
  }
  return;
}


// ---- _CountNumProfilesOnDisk @ 0x7bc15 (42 bytes)

int _CountNumProfilesOnDisk(void)

{
  short sVar1;
  undefined4 uVar2;
  
  uVar2 = _GetPlayerProfilePath(0);
  sVar1 = _CountNumFilesInFolder(uVar2);
  return (int)sVar1;
}


// ---- _DoesProfileAlreadyExist @ 0x7bc3f (145 bytes)

undefined1 _DoesProfileAlreadyExist(char *param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined1 local_d;
  
  local_d = 0;
  uVar1 = _GetPlayerProfilePath(0);
  iVar2 = _opendir(uVar1);
  if (iVar2 == 0) {
    local_d = 0;
  }
  else {
    do {
      do {
        iVar3 = _readdir(iVar2);
        if (iVar3 == 0) goto LAB_0007bcb9;
      } while (*(char *)(iVar3 + 6) != '\b');
      iVar3 = _strcmp((char *)(iVar3 + 8),param_1);
    } while (iVar3 != 0);
    local_d = 1;
LAB_0007bcb9:
    _closedir(iVar2);
  }
  return local_d;
}


// ---- _MMRestoreFromSaveGameFile @ 0x7bcd0 (518 bytes)

int _MMRestoreFromSaveGameFile(undefined4 param_1,char *param_2)

{
  char cVar1;
  int local_30;
  undefined1 local_21;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  int local_14;
  int local_10;
  
  local_10 = 0;
  _printf("About to restore from saved game file at: %s \n",param_1);
  local_14 = _SDL_RWFromFile(param_1,"rb");
  if (local_14 == 0) {
    local_30 = -1;
  }
  else {
    _SetUpNewGame();
    _MMPauseGameTimer();
    _strcpy(PTR__gLastSaveFilename_001c4730,param_2);
    local_10 = _LoadSaveDataFromFile(local_14,&local_1c,&local_20,&local_21,&local_18);
    if (local_10 == -2) {
      _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                        "Sorry, this saved game is in an old format that can\'t be used by Midnight Mansion HD."
                        ,3);
      local_30 = -0x2b67;
    }
    else {
      if ((*(short *)PTR__gCurrentMansionNumber_001c4824 != -2) &&
         (cVar1 = _IsMansionLocked((int)*(short *)PTR__gCurrentMansionNumber_001c4824),
         cVar1 != '\0')) {
        return -0x2bd7;
      }
      local_10 = (**(code **)(local_14 + 0xc))(local_14);
      if ((local_10 == 0) && (*PTR__gCurrentlyInMansion_001c4908 != '\0')) {
        _SetCurMansionAsCustomMansion(*(short *)PTR__gCurrentMansionNumber_001c4824 == -1);
        _RestoreJacksLives();
        _ChangeWorldRectForGame
                  (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                   (int)*(short *)PTR__gStatsHeight_001c47cc);
        _SWSetPostEraseCallBack
                  (*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,(CallBackPtr_t)PTR_LAB_001c4848);
        local_10 = _HandleRestoreStuff(local_1c,local_20,local_21,local_18);
        _printf("Restored from saved game file. err = %d \n",local_10);
        if (local_10 != 0) {
          _MyRestoreWorldRect(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
        }
      }
      local_30 = local_10;
    }
  }
  return local_30;
}


// ---- _HandleRestoreStuff @ 0x7bed6 (1261 bytes)

undefined4 _HandleRestoreStuff(float param_1,float param_2,char param_3,int param_4)

{
  char cVar1;
  undefined4 uVar2;
  undefined4 local_314;
  char local_30b [500];
  char local_117 [255];
  char *local_18;
  int local_14;
  SpritePtr local_10;
  
  local_14 = 0;
  if (param_4 < 0x6a) {
    _ErrorMessage("This saved game is in a format that is too old to open.",0);
    local_314 = 0xffffffff;
  }
  else {
    _strcpy(local_117,PTR__gMansionFilename_001c489c);
    _StripStringFromEndOfCString(".zip",local_117,0);
    _StripStringFromEndOfCString(".ZIP",local_117,0);
    _printf("HandleRestoreStuff entered. mansionNameNoZIPExt = %s\n",local_117);
    local_18 = (char *)_GetPathForAnyMansionFile(local_117,*PTR__gMansionIsCustomMansion_001c48ac);
    _printf("LOADING MANSION FROM PATH: %s \n",local_18);
    _strcpy(PTR__gCustomMansionFolderPath_001c4fcc,local_18);
    if ((((local_14 == 0) && (*PTR__gPlayingGameMovie_001c4fd8 == '\0')) &&
        (*PTR__gMansionIsCustomMansion_001c48ac == '\0')) &&
       ((*(short *)PTR__gCurrentMansionNumber_001c4824 != -2 &&
        (cVar1 = _DoesMansionPassChecksum(local_18,PTR__gMansionFilename_001c489c), cVar1 == '\0')))
       ) {
      return 0xffffcfc7;
    }
    if (local_14 == 0) {
      local_14 = _OpenMansionUsingFilePath(local_18,PTR__gMansionFilename_001c489c);
    }
    if ((local_14 != 0) && (*PTR__gMansionIsCustomMansion_001c48ac != '\0')) {
      uVar2 = _GetMansionFolderPath(PTR__gMansionFilename_001c489c);
      local_14 = _OpenMansionUsingFilePath(uVar2,PTR__gMansionFilename_001c489c);
    }
    if (local_14 == 0) {
      _RestoreStatsFromObjectsCarried();
      _PlayBackgroundMusic((int)*(short *)PTR__gCurrentMusicIndex_001c476c);
      _PauseMusic();
      *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 5;
      *(undefined4 *)PTR__gRunningFrameCount_001c481c = 0;
      if (((*PTR__gUseOutputFile_001c47f0 != '\0') && (*PTR__gEditorIsRunning_001c495c == '\0')) &&
         (cVar1 = _IsMoviePlaying(), cVar1 == '\0')) {
        _OpenOutputFile("Output File Log");
      }
      if (((*PTR__gRecordMovies_001c48f8 != '\0') && (*PTR__gEditorIsRunning_001c495c == '\0')) &&
         (cVar1 = _IsMoviePlaying(), cVar1 == '\0')) {
        _OpenMovieFileForOutput(0,param_3,param_1,param_2);
      }
      _EnableRecordMovieMenuItem(1);
      _ResetFloorTileAnimations(0,1);
      _SetCorrectGoldSilverTileImages(0);
      _LoadLevel((int)*(short *)PTR__gCurrentLevel_001c49bc,1);
      _SetUpLevelForPlaying();
      _WriteCStringToFile("RESTORED FROM SAVED OR SUSPENDED GAME.\r");
      _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164);
      _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gHeroOnPlatformSpriteLayerP_001c491c);
      if (param_3 == '\0') {
        local_10 = (SpritePtr)_NewMainSprite();
        if (local_10 != (SpritePtr)0x0) {
          _SWSetSpriteLocation(local_10,param_1,param_2);
          _SaveStartingPosition(local_10);
          _UpdateHaloSpritePosition(local_10);
        }
      }
      else {
        _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gStatueSpriteLayerP_001c4240);
        _CreateColorAndGrayStatues((int)(short)(int)param_1,(int)(short)(int)param_2,0);
        _PauseSpriteLayersForSaveAnim();
        _AddObjectToDoOnceArray(*(undefined4 *)PTR__gLastSavePedestalObjectID_001c49f8);
        *(undefined4 *)PTR__gLastSavePedestalObjectID_001c49f8 = 0;
      }
      _SaveHeroStartingLocation((int)(short)(int)param_1,(int)(short)(int)param_2);
      _StopLoopingSounds();
      if (*PTR__gUsingEditor_001c48b0 == '\0') {
        _MMHideAllCursors();
      }
      else {
        _MMRemoveGameCursor();
        _MMShieldCursor();
      }
      _DrawLevel();
      *PTR__gSpriteWorldIsValid_001c48c8 = 1;
      _DoGameFadeToNormal(0x1e);
      *PTR__gPlayingGame_001c4924 = 1;
      if (param_3 != '\0') {
        _MyDelay(0x1e);
      }
      _RunAnimation();
      local_314 = 0;
    }
    else {
      if (*PTR__gMansionIsCustomMansion_001c48ac == '\0') {
        _sprintf(local_30b,
                 " The mansion file \'%s\' could not be found. Please make sure this mansion is inside the Game Data -> Mansions folder and try again."
                 ,PTR__gMansionFilename_001c489c);
      }
      else {
        _sprintf(local_30b,
                 " The mansion file \'%s\' could not be found. Please make sure this mansion is inside the Custom Mansions folder and try again."
                 ,PTR__gMansionFilename_001c489c);
      }
      _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,local_30b,3);
      local_314 = 0xffffa932;
    }
  }
  return local_314;
}


// ---- _GetPathForAnyMansionFile @ 0x7c3c3 (79 bytes)

undefined4 _GetPathForAnyMansionFile(undefined4 param_1)

{
  undefined4 local_10;
  
  if (*PTR__gMansionIsCustomMansion_001c48ac == '\0') {
    local_10 = _GetMansionFolderPath(param_1);
  }
  else {
    local_10 = _GetCustomMansionsFolderPath(param_1);
  }
  return local_10;
}


// ---- _MMCreateSaveGameFile @ 0x7c412 (285 bytes)

int _MMCreateSaveGameFile(undefined4 param_1)

{
  int iVar1;
  SpritePtr srcSpriteP;
  SWMovementType local_1c;
  SWMovementType local_18;
  int local_10;
  
  iVar1 = _SDL_RWFromFile(param_1,"w+");
  if (iVar1 == 0) {
    local_10 = -1;
  }
  else {
    srcSpriteP = (SpritePtr)_GetHeroSprite();
    if (srcSpriteP == (SpritePtr)0x0) {
      local_1c = 0.0;
      local_18 = 0.0;
    }
    else {
      local_1c = _SWGetSpriteHorizLoc(srcSpriteP);
      local_18 = _SWGetSpriteVertLoc(srcSpriteP);
    }
    _ClearFileStickyError();
    _WriteSaveDataToFile(iVar1,local_1c,local_18,1);
    local_10 = _GetFileStickyError();
    (**(code **)(iVar1 + 0xc))(iVar1);
    if (local_10 == 0) {
      _printf("Save game file successfully created: %s \n",param_1);
    }
    _SetFileCreatorAndTypeForPath(param_1,0x4d414e53,0x53415645);
  }
  if (local_10 != 0) {
    _ErrorMessage("Sorry, the save game file couldn\'t be created. Function MMCreateSaveGameFile, error code %d."
                  ,local_10);
  }
  return local_10;
}


// ---- _DoSuspendGameDialog @ 0x7c52f (350 bytes)

int _DoSuspendGameDialog(void)

{
  char cVar1;
  bool bVar2;
  short local_e;
  
  cVar1 = _IsMoviePlaying();
  bVar2 = cVar1 == '\0';
  if ((*(int *)PTR__gCurrentGameMode_001c42d0 != 5) && (*(int *)PTR__gCurrentGameMode_001c42d0 != 6)
     ) {
    bVar2 = false;
  }
  if (bVar2) {
    if ((*(int *)PTR__gCurrentGameMode_001c42d0 == 6) && (*PTR__gGameIsPaused_001c484c == '\0')) {
      _DrawMapInOffscreenAreas();
    }
    cVar1 = _CanSuspendGame();
    if (cVar1 == '\0') {
      if (*(int *)PTR__gCurrentGameMode_001c42d0 == 6) {
        local_e = _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                                    "Are you sure you want to quit without suspending? (You can\'t suspend the game while looking at the map screen.)"
                                    ,1);
      }
      else {
        local_e = _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                                    "Are you sure you want to quit without suspending? (You can\'t suspend the game while Jack is jumping, dying, or riding on a moving platform.)"
                                    ,1);
      }
    }
    else {
      local_e = _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                                  "Suspend game so you can resume it later?",0);
      if (local_e == 2) {
        _SuspendGame();
      }
    }
    if ((local_e == 1) &&
       (((*(int *)PTR__gCurrentGameMode_001c42d0 == 5 ||
         (*(int *)PTR__gCurrentGameMode_001c42d0 == 6)) && (*PTR__gGameIsPaused_001c484c == '\0'))))
    {
      _MMHideCursor();
    }
  }
  else {
    local_e = 3;
  }
  return (int)local_e;
}


// ---- _MMDoErrorMessage @ 0x7c68d (154 bytes)

void _MMDoErrorMessage(char *param_1,int param_2)

{
  char local_138 [304];
  
  if (param_2 == 0) {
    _sprintf(local_138,param_1);
  }
  else {
    _sprintf(local_138,param_1,param_2);
  }
  if (*(int *)PTR__gSpriteWorldP_001c40d0 == 0) {
    _printf(local_138);
  }
  else {
    _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,local_138,3);
  }
  return;
}


// ---- _MMReportAdvancedError @ 0x7c727 (209 bytes)

void MMReportAdvancedError
               (char *param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5)

{
  char acStack_2c8 [400];
  char acStack_138 [304];
  
  if (param_2 == 0) {
    _sprintf(acStack_138,param_1);
  }
  else {
    _sprintf(acStack_138,param_1,param_2);
  }
  _sprintf(acStack_2c8,"%s  Function: %s   Line: %d",acStack_138,param_3,param_4,param_5);
  if (*(int *)PTR__gSpriteWorldP_001c40d0 == 0) {
    _printf(acStack_2c8);
  }
  else {
    _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,acStack_2c8,3);
  }
  return;
}


// ---- _HandleCustomDialogKeyEvent @ 0x7c7f8 (68 bytes)

void _HandleCustomDialogKeyEvent(int param_1,undefined1 param_2)

{
  if (param_1 == 0x28) {
    *PTR__gReturnKeyIsDown_001c4fbc = param_2;
  }
  else if (param_1 == 0x29) {
    *PTR__gEscKeyIsDown_001c4fa4 = param_2;
  }
  return;
}


// ---- _MMDoCustomDialog @ 0x7c83c (2811 bytes)

int _MMDoCustomDialog(SpriteWorldRec *param_1,undefined4 param_2,int param_3)

{
  short sVar1;
  int iVar2;
  int iVar3;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SWMovementType SVar4;
  int local_90;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  SpritePtr local_4c;
  void *local_48;
  short local_44;
  char local_41;
  undefined1 local_40;
  undefined1 local_3f;
  char local_3e;
  uchar local_3d;
  undefined4 local_3c;
  undefined4 local_38;
  float local_34;
  float local_30;
  float local_2c;
  float local_28;
  int local_24;
  SpritePtr local_20;
  short local_1c;
  short local_1a;
  short local_18;
  short local_16;
  SpritePtr local_14;
  short local_e;
  
  local_3e = '\0';
  local_24 = 0;
  if (*PTR__gCustomDialogIsVisible_001c49b8 == '\0') {
    local_3d = *(uchar *)(*(int *)PTR__gFadeToBlackSpriteLayerP_001c43e4 + 0x1c);
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gFadeToBlackSpriteLayerP_001c43e4,'\0');
    local_2c = param_1->zoomH;
    local_28 = param_1->zoomV;
    _SWSetSpriteWorldZoom(param_1,1.0,1.0);
    local_3c = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xac);
    local_38 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xb0);
    *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xac) = 0;
    *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xb0) = 0;
    if (*(int *)PTR__gCurrentGameMode_001c42d0 == 6) {
      local_20 = (SpritePtr)_NewFadeSprite(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0);
      _SWRemoveSprite(local_20);
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gCustomDialogSpriteLayerP_001c494c,local_20);
      _SWSetTilingOn(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,'\0');
    }
    local_34 = (float)(int)(*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38) / 0x280);
    local_30 = (float)(int)(short)((short)(*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38) *
                                           0x100008889 >> 0x18) -
                                  (*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38) >> 0xf));
    local_44 = 1;
    *PTR__gReturnKeyIsDown_001c4fbc = 0;
    *PTR__gEscKeyIsDown_001c4fa4 = 0;
    local_48 = param_1->tileChangeProc;
    _SWSetTileChangeProc(param_1,(void *)0x0);
    if (*(int *)PTR__gCurrentGameMode_001c42d0 == 5) {
      _StopLoopingSounds();
    }
    local_41 = *PTR__gGameIsPaused_001c484c;
    _PauseAllSpriteLayers(param_1);
    _SWUnpauseSpriteLayer(*(SpriteLayerPtr *)PTR__gCustomDialogSpriteLayerP_001c494c);
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gCustomDialogSpriteLayerP_001c494c,'\x01');
    local_24 = _MMCreateSpriteFromVerticalFile
                         (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_4c,0,"custom dialog.png"
                          ,0,1,0);
    if ((local_24 == 0) && (local_4c != (SpritePtr)0x0)) {
      *PTR__gCustomDialogIsVisible_001c49b8 = 1;
      _MMSaveCursorState();
      _MMShieldCursor();
      _MMSetCursorForGame();
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gCustomDialogSpriteLayerP_001c494c,local_4c);
      _SWSetSpriteScaledFactor(local_4c,local_34 / 2.0,local_30 / 2.0);
      _SWCenterSpriteInRect(local_4c,&param_1->workAreaRect);
      _SWOffsetSprite(local_4c,0.0,-50.0);
      local_54._0_2_ = (local_4c->destFrameRect).bottom;
      local_54._2_2_ = (local_4c->destFrameRect).right;
      local_58._0_2_ = (local_4c->destFrameRect).top;
      local_58._2_2_ = (local_4c->destFrameRect).left;
      _SWOffsetRect(&local_58,(int)(short)(int)(local_34 * 30.0),(int)(short)(int)(local_30 * 24.0))
      ;
      sVar1 = (short)(int)(local_34 * -60.0 + (float)(int)local_54._2_2_);
      local_54 = CONCAT22(sVar1,(short)(int)(local_30 * -85.0 + (float)(int)(short)local_54));
      local_14 = (SpritePtr)_NewBitmapTextSprite((int)(short)(sVar1 - local_58._2_2_));
      if (local_14 != (SpritePtr)0x0) {
        _SWSetSpriteScaledFactor(local_14,local_34 * 0.3,local_30 * 0.3);
        _SetBMTextSpriteText(local_14,param_2);
        _SWAddSprite(*(SpriteLayerPtr *)PTR__gCustomDialogSpriteLayerP_001c494c,local_14);
        _SWSetSpriteLocation(local_14,(float)(int)local_58._2_2_,(float)(int)(short)local_58);
      }
      SVar4 = _SWGetSpriteVertLoc(local_4c);
      local_1a = (short)(int)SVar4;
      SVar4 = _SWGetSpriteHorizLoc(local_4c);
      local_1c = (short)(int)SVar4;
      _SWGetSpriteHeight(local_4c);
      local_18 = (short)(int)extraout_ST0;
      _SWGetSpriteWidth(local_4c);
      local_16 = (short)(int)extraout_ST0_00;
      iVar2 = (int)local_1a;
      iVar3 = (int)local_18;
      sVar1 = _GetButtonHeight(0xda);
      local_1a = (short)(int)(((float)iVar3 - (float)(sVar1 + 0x10) * local_30) + (float)iVar2);
      iVar2 = (int)local_1c;
      iVar3 = (int)local_16;
      sVar1 = _GetButtonWidth(0xda);
      local_1c = (short)(int)(((float)iVar3 - (float)(sVar1 + 0x13) * local_34) + (float)iVar2);
      if (param_3 == 0) {
        _AddButtonToDialog(0xda,2,*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,
                           (int)local_1c,(int)local_1a);
        iVar2 = (int)local_1c;
        sVar1 = _GetButtonWidth(0xda);
        local_1c = (short)(int)((float)iVar2 - (float)(sVar1 + 4) * local_34);
        _AddButtonToDialog(0xda,3,*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,
                           (int)local_1c,(int)local_1a);
        SVar4 = _SWGetSpriteHorizLoc(local_4c);
        local_1c = (short)(int)(local_34 * 30.0 + SVar4);
        _AddButtonToDialog(0xda,1,*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,
                           (int)local_1c,(int)local_1a);
      }
      else if (param_3 == 1) {
        _AddButtonToDialog(0xda,2,*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,
                           (int)local_1c,(int)local_1a);
        SVar4 = _SWGetSpriteHorizLoc(local_4c);
        local_1c = (short)(int)(local_34 * 30.0 + SVar4);
        _AddButtonToDialog(0xda,1,*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,
                           (int)local_1c,(int)local_1a);
      }
      else if (param_3 == 2) {
        _AddButtonToDialog(0xda,2,*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,
                           (int)local_1c,(int)local_1a);
        iVar2 = (int)local_1c;
        sVar1 = _GetButtonWidth(0xda);
        local_1c = (short)(int)((float)iVar2 - (float)(sVar1 + 4) * local_34);
        _AddButtonToDialog(0xda,3,*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,
                           (int)local_1c,(int)local_1a);
      }
      else if (param_3 == 3) {
        _AddButtonToDialog(0xda,0,*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,
                           (int)local_1c,(int)local_1a);
      }
      else {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameSupport.c",
                   PTR____FUNCTION___29047_001c5008,0xa06);
      }
      _SWUpdateSpriteWorld(param_1,1);
      _DoGameFadeToNormal(0);
      local_3f = _WasMouseClicked();
      local_3e = '\0';
      while ((local_3e == '\0' && (*PTR__gQuitGame_001c49d0 == '\0'))) {
        _SWProcessSpriteWorld(param_1);
        _SWAnimateSpriteWorld(param_1);
        _ProcessSDLEvents();
        local_40 = local_3f;
        local_3f = _MMGetMouse(&local_50);
        _SWConvertCoordinates(param_1,&local_50);
        if (param_1->frameHasOccurred != '\0') {
          local_e = _ProcessButtonSprites
                              (*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,
                               *(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,local_50,
                               local_3f,0,0);
          if (local_e == -1) {
            if (*PTR__gEscKeyIsDown_001c4fa4 == '\0') {
              if (*PTR__gReturnKeyIsDown_001c4fbc != '\0') {
                if (((param_3 == 0) || (param_3 == 1)) || (param_3 == 2)) {
                  local_e = 2;
                }
                else if (param_3 == 3) {
                  local_e = 0;
                }
              }
            }
            else if (param_3 == 2) {
              local_e = 3;
            }
            else if (param_3 != 3) {
              local_e = 1;
            }
          }
          if (local_e != -1) {
            _AnimateOneFrame(param_1);
            local_3e = '\x01';
            local_44 = local_e;
          }
        }
      }
      *PTR__gCustomDialogIsVisible_001c49b8 = 0;
      _MMRestoreCursorState();
    }
    *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xac) = local_3c;
    *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xb0) = local_38;
    param_1->zoomH = local_2c;
    param_1->zoomV = local_28;
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gFadeToBlackSpriteLayerP_001c43e4,local_3d);
    _SWSetTilingOn(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,'\x01');
    _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gCustomDialogSpriteLayerP_001c494c);
    _SWSetTileChangeProc(param_1,local_48);
    _SWUpdateSpriteWorld(param_1,1);
    _UnpauseAllSpriteLayers(param_1);
    if (local_41 != '\0') {
      _PauseSpriteLayersForPause();
    }
    _ResetKeys();
    _WriteEventToMovieFile(0xb,1);
    local_90 = (int)local_44;
  }
  else {
    local_90 = 1;
  }
  return local_90;
}


// ---- _MMDoOptionsDialog @ 0x7d337 (2082 bytes)

void _MMDoOptionsDialog(void)

{
  char cVar1;
  short sVar2;
  undefined4 local_44;
  undefined1 local_40 [4];
  SpritePtr local_3c;
  void *local_38;
  short local_32;
  undefined1 local_30;
  undefined1 local_2f;
  char local_2e;
  uchar local_2d;
  undefined4 local_2c;
  undefined4 local_28;
  float local_24;
  float local_20;
  undefined4 local_1c;
  undefined4 local_18;
  char local_13;
  char local_12;
  char local_11;
  int local_10;
  
  local_2e = '\0';
  local_10 = 0;
  if (*PTR__gMainSpritesAreLoaded_001c4178 != '\0') {
    local_2d = *(uchar *)(*(int *)PTR__gFadeToBlackSpriteLayerP_001c43e4 + 0x1c);
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gFadeToBlackSpriteLayerP_001c43e4,'\0');
    local_1c = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xb4);
    local_18 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xb8);
    _SWSetSpriteWorldZoom(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,1.0,1.0);
    local_2c = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xac);
    local_28 = *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xb0);
    *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xac) = 0;
    *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xb0) = 0;
    local_24 = (float)(int)(*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38) / 0x280);
    local_20 = (float)(int)(short)((short)(*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38) *
                                           0x100008889 >> 0x18) -
                                  (*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38) >> 0xf));
    *(undefined4 *)PTR__gControlsTabSpriteP_001c5000 = 0;
    *(undefined4 *)PTR__gOptionsTabSpriteP_001c4ff8 =
         *(undefined4 *)PTR__gControlsTabSpriteP_001c5000;
    *PTR__gReturnKeyIsDown_001c4fbc = 0;
    *PTR__gEscKeyIsDown_001c4fa4 = 0;
    local_13 = *PTR__gMusicIsOn_001c47c0;
    local_12 = *PTR__gFullScreenWindow_001c48ec;
    local_11 = *PTR__gStretchToFit_001c49dc;
    _SetMMEventCallback(PTR_HandleEventInKeyInputBox_001c4fb8);
    local_38 = *(void **)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xf8);
    _SWSetTileChangeProc(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,(void *)0x0);
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gCustomDialogSpriteLayerP_001c494c,'\x01');
    local_10 = _MMCreateSpriteFromVerticalFile
                         (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_3c,0,"custom dialog.png"
                          ,0,1,0);
    if ((local_10 == 0) && (local_3c != (SpritePtr)0x0)) {
      *PTR__gCustomDialogIsVisible_001c49b8 = 1;
      _MMSaveCursorState();
      _MMShieldCursor();
      _MMSetCursorForGame();
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gCustomDialogSpriteLayerP_001c494c,local_3c);
      _SWSetSpriteScaledFactor(local_3c,local_24 * 0.65,local_20 * 0.65);
      _SWCenterSpriteInRect(local_3c,*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
      if (*(int *)PTR__gCurrentGameMode_001c42d0 != 0) {
        _SWOffsetSprite(local_3c,0.0,-50.0);
      }
      *(undefined2 *)PTR__gCurOptionsDialogPane_001c4fe8 = 1;
      _AddOptionsItemsToOptionsDialog
                (*(undefined4 *)PTR__gOptionsDialogPane2LayerP_001c4838,&local_3c->destFrameRect);
      _AddControlsItemsToOptionsDialog
                (*(undefined4 *)PTR__gOptionsDialogPane1LayerP_001c4970,&local_3c->destFrameRect);
      _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gOptionsDialogPane1LayerP_001c4970,'\x01');
      _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gOptionsDialogPane2LayerP_001c4838,'\0');
      _AddStandardItemsToOptionsDialog
                (*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,local_3c,
                 &local_3c->destFrameRect);
      _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
      _DoGameFadeToNormal(0);
      local_2f = _WasMouseClicked();
      local_2e = '\0';
      while ((local_2e == '\0' && (*PTR__gQuitGame_001c49d0 == '\0'))) {
        _SWProcessSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
        _SWAnimateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0);
        _ProcessSDLEvents();
        local_30 = local_2f;
        local_2f = _MMGetMouse(&local_44);
        _SWConvertCoordinates(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_44);
        if (*(char *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x141) != '\0') {
          local_32 = _ProcessButtonSprites
                               (*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,
                                *(undefined4 *)PTR__gOptionsDialogPane2LayerP_001c4838,local_44,
                                local_2f,0,local_40);
          if (local_32 == -1) {
            if (*PTR__gEscKeyIsDown_001c4fa4 == '\0') {
              if (*PTR__gReturnKeyIsDown_001c4fbc != '\0') {
                local_32 = 0;
              }
            }
            else {
              local_32 = 1;
            }
          }
          if (local_32 == 5) {
            _ToggleMusic();
          }
          if (local_32 == 8) {
            _ToggleFullscreen();
          }
          if (local_32 == 9) {
            _SetStretchToFit(*PTR__gStretchToFit_001c49dc == '\0');
          }
          if (local_32 == 6) {
            sVar2 = _GetMusicVolume();
            _SetMusicVolume((int)sVar2);
          }
          if (local_32 == 0x17) {
            cVar1 = _IsGamepadInitialized();
            if (cVar1 == '\0') {
              _InitGamePad();
            }
          }
          if (local_32 == 0xdf) {
            if (*(short *)PTR__gCurOptionsDialogPane_001c4fe8 != 2) {
              *(undefined2 *)PTR__gCurOptionsDialogPane_001c4fe8 = 2;
              _SWSetSpriteLayerVisible
                        (*(SpriteLayerRec **)PTR__gOptionsDialogPane1LayerP_001c4970,'\0');
              _SWSetSpriteLayerVisible
                        (*(SpriteLayerRec **)PTR__gOptionsDialogPane2LayerP_001c4838,'\x01');
              _SetButtonDimmedMode(*(undefined4 *)PTR__gControlsTabSpriteP_001c5000,0);
            }
          }
          else if ((local_32 == 0xe0) && (*(short *)PTR__gCurOptionsDialogPane_001c4fe8 != 1)) {
            *(undefined2 *)PTR__gCurOptionsDialogPane_001c4fe8 = 1;
            _SWSetSpriteLayerVisible
                      (*(SpriteLayerRec **)PTR__gOptionsDialogPane1LayerP_001c4970,'\x01');
            _SWSetSpriteLayerVisible
                      (*(SpriteLayerRec **)PTR__gOptionsDialogPane2LayerP_001c4838,'\0');
            _SetButtonDimmedMode(*(undefined4 *)PTR__gOptionsTabSpriteP_001c4ff8,0);
          }
          if ((local_32 == 1) || (local_32 == 0)) {
            if (local_32 == 0) {
              _UpdateDataFromControlsInLayers
                        (*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,
                         *(undefined4 *)PTR__gOptionsDialogPane2LayerP_001c4838);
              *PTR__gFullScreenWindowPrefs_001c4920 = *PTR__gFullScreenWindow_001c48ec;
              _WritePreferencesFile();
            }
            sVar2 = _GetMusicVolume();
            _SetMusicVolume((int)sVar2);
            _AnimateOneFrame(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
            local_2e = '\x01';
          }
        }
      }
      *PTR__gCustomDialogIsVisible_001c49b8 = 0;
      _MMRestoreCursorState();
    }
    *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xac) = local_2c;
    *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xb0) = local_28;
    *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xb4) = local_1c;
    *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xb8) = local_18;
    _SWSetSpriteLayerVisible(*(SpriteLayerRec **)PTR__gFadeToBlackSpriteLayerP_001c43e4,local_2d);
    _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gCustomDialogSpriteLayerP_001c494c);
    _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gOptionsDialogPane1LayerP_001c4970);
    _SWDisposeAllSpritesInLayer(*(SpriteLayerPtr *)PTR__gOptionsDialogPane2LayerP_001c4838);
    _SWSetTileChangeProc(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_38);
    _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
    if (local_32 == 1) {
      if (local_13 != *PTR__gMusicIsOn_001c47c0) {
        _ToggleMusic();
      }
      if (local_12 != *PTR__gFullScreenWindow_001c48ec) {
        _ToggleFullscreen();
      }
      if (local_11 != *PTR__gStretchToFit_001c49dc) {
        _SetStretchToFit(local_11);
      }
    }
    _SetMMEventCallback(0);
  }
  return;
}


// ---- _AddStandardItemsToOptionsDialog @ 0x7db59 (1015 bytes)

void _AddStandardItemsToOptionsDialog(undefined4 param_1,SpritePtr param_2)

{
  short sVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  undefined4 uVar5;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  longdouble extraout_ST0_01;
  float fVar6;
  SWMovementType SVar7;
  SWMovementType SVar8;
  
  iVar4 = (int)(*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38) / 0x280);
  fVar6 = (float)iVar4;
  sVar2 = *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38);
  SVar7 = _SWGetSpriteVertLoc(param_2);
  SVar8 = _SWGetSpriteHorizLoc(param_2);
  _SWGetSpriteHeight(param_2);
  _SWGetSpriteWidth(param_2);
  sVar1 = _GetButtonHeight(0xda);
  sVar2 = (short)(int)(((float)(int)(short)(int)extraout_ST0 -
                       (float)(sVar1 + 0x13) *
                       (float)(int)(short)((short)(sVar2 * 0x100008889 >> 0x18) - (sVar2 >> 0xf))) +
                      (float)(int)(short)(int)SVar7);
  sVar1 = _GetButtonWidth(0xda);
  sVar1 = (short)(int)(((float)(int)(short)(int)extraout_ST0_00 - (float)(sVar1 + 0x13) * fVar6) +
                      (float)(int)(short)(int)SVar8);
  _AddButtonToDialog(0xda,0,*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,(int)sVar1,
                     (int)sVar2);
  sVar3 = _GetButtonWidth(0xda);
  _AddButtonToDialog(0xda,1,*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,
                     (int)(short)(int)((float)(int)sVar1 - (float)(sVar3 + 4) * fVar6),(int)sVar2);
  SVar7 = _SWGetSpriteVertLoc(param_2);
  sVar2 = (short)(int)(SVar7 - 20.0);
  SVar7 = _SWGetSpriteHorizLoc(param_2);
  sVar1 = _GetButtonWidth(0xe0);
  sVar1 = (short)(int)((float)iVar4 * (float)(int)sVar1 * -0.5 +
                      (float)(int)(short)((short)(int)extraout_ST0_00 / 2) + SVar7);
  uVar5 = _AddButtonToDialog(0xe0,0,*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,
                             (int)sVar1,(int)sVar2);
  *(undefined4 *)PTR__gControlsTabSpriteP_001c5000 = uVar5;
  _SetButtonPushedOffset(*(undefined4 *)PTR__gControlsTabSpriteP_001c5000,0,2);
  if (*(short *)PTR__gCurOptionsDialogPane_001c4fe8 == 1) {
    _SetButtonDimmedMode(*(undefined4 *)PTR__gControlsTabSpriteP_001c5000,1);
  }
  _SWGetSpriteWidth(*(SpritePtr *)PTR__gControlsTabSpriteP_001c5000);
  uVar5 = _AddButtonToDialog(0xdf,0,*(undefined4 *)PTR__gCustomDialogSpriteLayerP_001c494c,
                             (int)(short)(int)((float)(int)sVar1 + (float)extraout_ST0_01),
                             (int)sVar2);
  *(undefined4 *)PTR__gOptionsTabSpriteP_001c4ff8 = uVar5;
  _SetButtonPushedOffset(*(undefined4 *)PTR__gOptionsTabSpriteP_001c4ff8,0,2);
  if (*(short *)PTR__gCurOptionsDialogPane_001c4fe8 == 2) {
    _SetButtonDimmedMode(*(undefined4 *)PTR__gOptionsTabSpriteP_001c4ff8,1);
  }
  return;
}


// ---- _AddOptionsItemsToOptionsDialog @ 0x7df50 (1533 bytes)

void _AddOptionsItemsToOptionsDialog(SpriteLayerPtr param_1,short *param_2)

{
  short sVar1;
  float fVar2;
  short sVar3;
  short sVar4;
  SpritePtr pSVar5;
  float fVar6;
  SWMovementType horizLoc;
  
  fVar6 = (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38) / 1280.0;
  fVar2 = fVar6 * 1.05;
  sVar3 = (short)(int)(fVar6 * 90.0 + (float)(int)param_2[1]);
  sVar4 = (short)(int)(fVar6 * 180.0 + (float)(int)*param_2);
  pSVar5 = (SpritePtr)_NewBitmapTextSprite(2000);
  if (pSVar5 != (SpritePtr)0x0) {
    _SWSetSpriteScaledFactor(pSVar5,fVar6 * 1.2,fVar6 * 1.2);
    _SetBMTextSpriteText(pSVar5,"Options");
    _SWAddSprite(param_1,pSVar5);
    _SWCenterSpriteInRect(pSVar5,param_2);
    sVar1 = *param_2;
    horizLoc = _SWGetSpriteHorizLoc(pSVar5);
    _SWSetSpriteLocation(pSVar5,horizLoc,(float)(int)sVar1 + fVar6 * 70.0);
  }
  _AddCheckboxToOptionsDialog
            ("Music (Command-M)",5,PTR__gMusicIsOn_001c47c0,param_1,fVar2,(int)sVar3,(int)sVar4);
  sVar4 = (short)(int)(fVar6 * 60.0 + (float)(int)sVar4);
  _AddCheckboxToOptionsDialog
            ("Sound effects",7,PTR__gSoundIsTurnedOn_001c47fc,param_1,fVar2,(int)sVar3,(int)sVar4);
  sVar4 = (short)(int)(fVar6 * 60.0 + (float)(int)sVar4);
  _AddCheckboxToOptionsDialog
            ("Fullscreen",8,PTR__gFullScreenWindow_001c48ec,param_1,fVar2,(int)sVar3,(int)sVar4);
  sVar4 = (short)(int)(fVar6 * 60.0 + (float)(int)sVar4);
  _AddCheckboxToOptionsDialog
            ("Stretch to fit",9,PTR__gStretchToFit_001c49dc,param_1,fVar2,(int)sVar3,(int)sVar4);
  pSVar5 = (SpritePtr)_NewBitmapTextSprite((int)(short)(int)(fVar6 * 650.0));
  if (pSVar5 != (SpritePtr)0x0) {
    _SWSetSpriteScaledFactor(pSVar5,fVar6 * 0.49,fVar6 * 0.49);
    _SetBMTextSpriteText(pSVar5,"TIP: To resize the window, drag the lower-right corner.");
    _SWAddSprite(param_1,pSVar5);
    _SWSetSpriteLocation
              (pSVar5,(float)(int)sVar3,
               (float)(int)(short)(int)(fVar6 * 60.0 +
                                       (float)(int)(short)(int)(fVar6 * 60.0 + (float)(int)sVar4)));
  }
  sVar3 = (short)(int)(fVar6 * 450.0 + (float)(int)sVar3);
  sVar4 = (short)(int)(fVar6 * 180.0 + (float)(int)*param_2);
  _AddCheckboxToOptionsDialog
            ("Softer music",6,PTR__gSofterMusic_001c4768,param_1,fVar2,(int)sVar3,(int)sVar4);
  sVar4 = (short)(int)(fVar6 * 60.0 + (float)(int)sVar4);
  _AddCheckboxToOptionsDialog
            ("Limit Fullscreen Resolution",10,PTR__gLimitFullscreenResolution_001c4720,param_1,fVar2
             ,(int)sVar3,(int)sVar4);
  sVar4 = (short)(int)(fVar6 * 60.0 + (float)(int)sVar4);
  _AddCheckboxToOptionsDialog
            ("Create replays of each game",0xb,PTR__gRecordMovies_001c48f8,param_1,fVar2,(int)sVar3,
             (int)sVar4);
  _AddCheckboxToOptionsDialog
            ("Family friendly (no blood)",0xc,PTR__gFamilyFriendly_001c46f0,param_1,fVar2,(int)sVar3
             ,(int)(short)(int)(fVar6 * 60.0 + (float)(int)sVar4));
  return;
}


// ---- _AddCheckboxToOptionsDialog @ 0x7e54d (148 bytes)

SpritePtr _AddCheckboxToOptionsDialog
                    (undefined4 param_1,short param_2,undefined4 param_3,SpriteLayerPtr param_4,
                    undefined4 param_5,short param_6,short param_7)

{
  SpritePtr newSpriteP;
  
  newSpriteP = (SpritePtr)_NewCustomCheckbox(param_1,param_5,(int)param_2);
  if (newSpriteP != (SpritePtr)0x0) {
    _SWAddSprite(param_4,newSpriteP);
    _SWSetSpriteLocation(newSpriteP,(float)(int)param_6,(float)(int)param_7);
    _LinkCheckboxWithData(newSpriteP,param_3);
  }
  return newSpriteP;
}


// ---- _AddControlsItemsToOptionsDialog @ 0x7e5e1 (1979 bytes)

void _AddControlsItemsToOptionsDialog(SpriteLayerPtr param_1,short *param_2)

{
  short sVar1;
  short sVar2;
  short sVar3;
  SpritePtr newSpriteP;
  undefined4 uVar4;
  float fVar5;
  SWMovementType horizLoc;
  
  fVar5 = (float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38) / 1280.0;
  sVar2 = (short)(int)(fVar5 * 220.0 + (float)(int)param_2[1]);
  sVar3 = (short)(int)(fVar5 * 165.0 + (float)(int)*param_2);
  newSpriteP = (SpritePtr)_NewBitmapTextSprite(2000);
  if (newSpriteP != (SpritePtr)0x0) {
    _SWSetSpriteScaledFactor(newSpriteP,fVar5 * 1.2,fVar5 * 1.2);
    _SetBMTextSpriteText(newSpriteP,"Controls");
    _SWAddSprite(param_1,newSpriteP);
    _SWCenterSpriteInRect(newSpriteP,param_2);
    sVar1 = *param_2;
    horizLoc = _SWGetSpriteHorizLoc(newSpriteP);
    _SWSetSpriteLocation(newSpriteP,horizLoc,(float)(int)sVar1 + fVar5 * 70.0);
  }
  _AddKeyInputBoxToOptionsDialog
            ("Left:",0xd,PTR__gKeyPrefs_001c46f4 + 0xc,PTR__gKeyPrefs_001c46f4 + 0x30,
             PTR__gKeyPrefs_001c46f4 + 0xde,param_1,(int)sVar2,(int)sVar3);
  sVar3 = (short)(int)(fVar5 * 60.0 + (float)(int)sVar3);
  _AddKeyInputBoxToOptionsDialog
            ("Right:",0xe,PTR__gKeyPrefs_001c46f4 + 4,PTR__gKeyPrefs_001c46f4 + 0x28,
             PTR__gKeyPrefs_001c46f4 + 0x7a,param_1,(int)sVar2,(int)sVar3);
  sVar3 = (short)(int)(fVar5 * 60.0 + (float)(int)sVar3);
  _AddKeyInputBoxToOptionsDialog
            ("Up:",0xf,PTR__gKeyPrefs_001c46f4,PTR__gKeyPrefs_001c46f4 + 0x24,
             PTR__gKeyPrefs_001c46f4 + 0x48,param_1,(int)sVar2,(int)sVar3);
  sVar3 = (short)(int)(fVar5 * 60.0 + (float)(int)sVar3);
  _AddKeyInputBoxToOptionsDialog
            ("Down:",0x10,PTR__gKeyPrefs_001c46f4 + 8,PTR__gKeyPrefs_001c46f4 + 0x2c,
             PTR__gKeyPrefs_001c46f4 + 0xac,param_1,(int)sVar2,(int)sVar3);
  _AddKeyInputBoxToOptionsDialog
            ("Jump:",0x11,PTR__gKeyPrefs_001c46f4 + 0x10,PTR__gKeyPrefs_001c46f4 + 0x34,
             PTR__gKeyPrefs_001c46f4 + 0x110,param_1,(int)sVar2,
             (int)(short)(int)(fVar5 * 60.0 + (float)(int)sVar3));
  sVar2 = (short)(int)(fVar5 * 560.0 + (float)(int)sVar2);
  sVar3 = (short)(int)(fVar5 * 165.0 + (float)(int)*param_2);
  _AddKeyInputBoxToOptionsDialog
            ("Jump left:",0x12,PTR__gKeyPrefs_001c46f4 + 0x14,PTR__gKeyPrefs_001c46f4 + 0x38,
             PTR__gKeyPrefs_001c46f4 + 0x142,param_1,(int)sVar2,(int)sVar3);
  sVar3 = (short)(int)(fVar5 * 60.0 + (float)(int)sVar3);
  _AddKeyInputBoxToOptionsDialog
            ("Jump right:",0x13,PTR__gKeyPrefs_001c46f4 + 0x18,PTR__gKeyPrefs_001c46f4 + 0x3c,
             PTR__gKeyPrefs_001c46f4 + 0x174,param_1,(int)sVar2,(int)sVar3);
  sVar3 = (short)(int)(fVar5 * 60.0 + (float)(int)sVar3);
  _AddKeyInputBoxToOptionsDialog
            ("Action:",0x14,PTR__gKeyPrefs_001c46f4 + 0x1c,PTR__gKeyPrefs_001c46f4 + 0x40,
             PTR__gKeyPrefs_001c46f4 + 0x1a6,param_1,(int)sVar2,(int)sVar3);
  sVar3 = (short)(int)(fVar5 * 60.0 + (float)(int)sVar3);
  _AddKeyInputBoxToOptionsDialog
            ("Map:",0x15,PTR__gKeyPrefs_001c46f4 + 0x20,PTR__gKeyPrefs_001c46f4 + 0x44,
             PTR__gKeyPrefs_001c46f4 + 0x1d8,param_1,(int)sVar2,(int)sVar3);
  sVar2 = (short)(int)(fVar5 * 110.0 + (float)(int)param_2[1]);
  sVar3 = (short)(int)(fVar5 * 90.0 + (float)(int)(short)(int)(fVar5 * 60.0 + (float)(int)sVar3));
  _AddCheckboxToOptionsDialog
            ("Enable arrow keys",0x16,PTR__gArrowKeysEnabled_001c48a0,param_1,fVar5 * 1.1,(int)sVar2
             ,(int)sVar3);
  uVar4 = _AddCheckboxToOptionsDialog
                    ("Enable GamePad",0x17,PTR__gUseGamePad_001c4740,param_1,fVar5 * 1.1,(int)sVar2,
                     (int)(short)(int)(fVar5 * 40.0 + (float)(int)sVar3));
  *(undefined4 *)PTR__gGamepadCheckboxSpriteP_001c4fa0 = uVar4;
  return;
}


// ---- _AddKeyInputBoxToOptionsDialog @ 0x7ed9c (563 bytes)

SpritePtr _AddKeyInputBoxToOptionsDialog
                    (undefined4 param_1,short param_2,undefined4 param_3,undefined4 param_4,
                    undefined4 param_5,SpriteLayerPtr param_6,short param_7,short param_8)

{
  SpritePtr newSpriteP;
  SpritePtr srcSpriteP;
  longdouble extraout_ST0;
  longdouble lVar1;
  
  newSpriteP = (SpritePtr)
               _NewKeyInputBox("ignored",
                               ((float)(int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38) /
                               1280.0) * 0.8,(int)param_2);
  if (newSpriteP != (SpritePtr)0x0) {
    _SWAddSprite(param_6,newSpriteP);
    _SWSetSpriteLocation(newSpriteP,(float)(int)param_7,(float)(int)param_8);
    _LinkKeyInputBoxWithData(newSpriteP,param_3,param_4,param_5);
  }
  srcSpriteP = (SpritePtr)_NewBitmapTextSprite(2000);
  if (srcSpriteP != (SpritePtr)0x0) {
    _SWGetSpriteHeight(newSpriteP);
    _SWSetSpriteScaledHeight(srcSpriteP,SUB42((float)extraout_ST0 * 0.63,0));
    _SWSetSpriteScaledWidthFactor(srcSpriteP,srcSpriteP->drawData->scaledHeightFactor);
    _SetBMTextSpriteText(srcSpriteP,param_1);
    _SWSetSpriteColor(srcSpriteP,newSpriteP->drawData->colorRed,newSpriteP->drawData->colorGreen,
                      newSpriteP->drawData->colorBlue);
    _SWAddSprite(param_6,srcSpriteP);
    lVar1 = (longdouble)_GetWidthOfBMText(srcSpriteP,param_1);
    _SWSetSpriteLocation
              (srcSpriteP,((float)(int)param_7 - (float)lVar1) - 20.0,(float)(param_8 + 3));
  }
  return newSpriteP;
}


// ---- _PrepareGameForDialogBox @ 0x7efcf (357 bytes)

void _PrepareGameForDialogBox(undefined4 param_1)

{
  char cVar1;
  
  if (*PTR__gDialogBoxIsOpen_001c49c8 == '\0') {
    *PTR__wasFullscreen_001c4fd4 = *PTR__gFullScreenWindow_001c48ec;
    if (*PTR__gFullScreenWindow_001c48ec != '\0') {
      *(undefined4 *)PTR__saveTileBackgroundP_001c4ff0 =
           *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x1c);
      *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x1c) = 0;
      _ToggleFullscreen();
      _ProcessSDLEvents();
      _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
    }
    *(undefined4 *)PTR__gSaveCurrentGameMode_001c4f9c =
         *(undefined4 *)PTR__gCurrentGameMode_001c42d0;
    *PTR__gDialogBoxIsOpen_001c49c8 = 1;
    *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = param_1;
    _MMFlushEvents();
    _MMSaveCursorState();
    _MMSetCursorForSystem();
    _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
    _InitCursor();
    while( true ) {
      cVar1 = _IsMouseDown();
      if ((((cVar1 == '\0') && (PTR__gKeys_001c4af8[1] == '\0')) && (PTR__gKeys_001c4af8[3] == '\0')
          ) && (((*PTR__gKeys_001c4af8 == '\0' && (PTR__gKeys_001c4af8[2] == '\0')) &&
                ((PTR__gKeys_001c4af8[7] == '\0' && (PTR__gKeys_001c4af8[4] == '\0')))))) break;
      _ProcessSDLEvents();
    }
  }
  return;
}


// ---- _RestoreGameAfterDialogBox @ 0x7f134 (276 bytes)

void _RestoreGameAfterDialogBox(void)

{
  char cVar1;
  
  if (*PTR__gDialogBoxIsOpen_001c49c8 != '\0') {
    _SDL_RaiseWindow(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158));
    if (*PTR__wasFullscreen_001c4fd4 != '\0') {
      _ToggleFullscreen();
      _SWUpdateWindowSize(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0);
      _SWUpdateWindowSize(*(SpriteWorldRec **)PTR__gStatsSpriteWorldP_001c48f4);
      if (*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x1c) != 0) {
        _SWDisposeSprite((SpritePtr *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x1c));
      }
      *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x1c) =
           *(undefined4 *)PTR__saveTileBackgroundP_001c4ff0;
      _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
    }
    cVar1 = _IsMoviePlaying();
    if (cVar1 == '\0') {
      _ResetKeys();
    }
    *(undefined4 *)PTR__gCurrentGameMode_001c42d0 =
         *(undefined4 *)PTR__gSaveCurrentGameMode_001c4f9c;
    *PTR__gDialogBoxIsOpen_001c49c8 = 0;
    _MMRestoreCursorState();
    _MMFlushEvents();
  }
  return;
}


// ---- _IsMMRegistered @ 0x7f248 (181 bytes)

undefined1 _IsMMRegistered(short param_1)

{
  undefined1 local_18;
  undefined1 local_14;
  
  if (param_1 == 1) {
    if (((*PTR__gEpisode1IsUnlocked_001c4744 == '\0') &&
        (*PTR__gEpisode2IsUnlocked_001c48d0 == '\0')) &&
       (*PTR__gCanPlayBothEpisodes_001c4738 == '\0')) {
      local_18 = 0;
      local_14 = local_18;
    }
    else {
      local_18 = 1;
      local_14 = local_18;
    }
  }
  else if (((*PTR__gEpisode1IsUnlocked_001c4744 == '\0') ||
           (*PTR__gEpisode2IsUnlocked_001c48d0 == '\0')) &&
          (*PTR__gCanPlayBothEpisodes_001c4738 == '\0')) {
    local_14 = 0;
  }
  else {
    local_14 = 1;
  }
  return local_14;
}


// ---- _CheckForLicenseFilename @ 0x7f2fd (256 bytes)

char _CheckForLicenseFilename(undefined4 *param_1,undefined4 param_2)

{
  char cVar1;
  short local_214;
  undefined1 local_212 [2];
  undefined1 local_210 [3];
  byte local_20d;
  undefined1 auStack_20c [255];
  byte local_10d;
  undefined1 auStack_10c [255];
  char local_d;
  
  local_d = _MMReadLicenseFile(&local_10d,&local_20d,param_1,param_2);
  auStack_10c[local_10d] = 0;
  auStack_20c[local_20d] = 0;
  if ((local_d == '\0') ||
     (cVar1 = _ValidateRegistrationCodeWithPrefix
                        (auStack_10c,auStack_20c,local_210,local_212,&local_214), cVar1 == '\0')) {
    return '\0';
  }
  cVar1 = _IsNamePirated(&local_10d);
  if (cVar1 == '\0') {
    return local_d;
  }
  if (0x7d7 < local_214) {
    return local_d;
  }
  _MMDeleteLicenseFile(param_2);
  *param_1 = 0xffffffff;
  *PTR__gIsRegistered_001c4fc8 = 0;
  return local_d;
}


// ---- _UpdateHiddenDateFile @ 0x7f3fd (395 bytes)

void _UpdateHiddenDateFile(void)

{
  short sVar1;
  undefined2 local_30;
  undefined1 local_2e [2];
  short local_2c;
  short local_2a;
  short local_28;
  short local_26;
  short local_24;
  short local_22;
  undefined4 local_20;
  int local_1c;
  int local_18;
  int local_14;
  undefined4 local_10;
  
  local_20 = 0;
  _GetDateFromHiddenFile(&local_22,&local_24,&local_26,local_2e);
  local_10 = _GetRealPrefsFilePath("MM HD Data New");
  local_14 = _SDL_RWFromFile(local_10,"r+");
  if (local_14 != 0) {
    _MMGetCurrentDate(&local_28,&local_2a,&local_2c);
    sVar1 = _GetNumDaysBeforeMonth((int)local_24);
    local_1c = local_26 * 0x16d + (int)sVar1 + (int)local_22;
    sVar1 = _GetNumDaysBeforeMonth((int)local_2a);
    local_18 = local_2c * 0x16d + (int)sVar1 + (int)local_28;
    if (local_1c < local_18) {
      _ClearFileStickyError();
      local_30 = 0x1c9d;
      _MyWriteDataToFile(local_14,2,&local_30);
      _MyWriteDataToFile(local_14,2,&local_28);
      _MyWriteDataToFile(local_14,2,&local_2a);
      _MyWriteDataToFile(local_14,2,&local_2c);
      _MyWriteDataToFile(local_14,2,local_2e);
      local_20 = _GetFileStickyError();
    }
    (**(code **)(local_14 + 0xc))(local_14);
  }
  return;
}


// ---- _GetDateFromHiddenFile @ 0x7f588 (369 bytes)

void _GetDateFromHiddenFile
               (undefined2 *param_1,undefined2 *param_2,undefined2 *param_3,undefined2 *param_4)

{
  short local_1a;
  undefined4 local_18;
  int local_14;
  undefined4 local_10;
  
  local_18 = 0;
  *param_4 = 0;
  *param_3 = *param_4;
  *param_2 = *param_3;
  *param_1 = *param_2;
  local_10 = _GetRealPrefsFilePath("MM HD Data New");
  local_14 = _SDL_RWFromFile(local_10,"rb");
  if (local_14 != 0) {
    _ClearFileStickyError();
    _MyReadDataFromFile(local_14,2,&local_1a);
    if (local_1a == 0x1c9d) {
      _MyReadDataFromFile(local_14,2,param_1);
      _MyReadDataFromFile(local_14,2,param_2);
      _MyReadDataFromFile(local_14,2,param_3);
      _MyReadDataFromFile(local_14,2,param_4);
    }
    else {
      _puts("ERROR: Hidden file is corrupted!");
    }
    if ((*(short *)PTR__savedDay_001c4fb4 == 0) ||
       ((*(short *)PTR__savedMonth_001c4fc4 == 0 && (*(short *)PTR__savedYear_001c5018 == 0)))) {
      *PTR__didReadDateFromFile_001c4fc0 = 0;
    }
    local_18 = _GetFileStickyError();
    (**(code **)(local_14 + 0xc))(local_14);
  }
  return;
}


// ---- _ChangeHiddenPiracyValue @ 0x7f6f9 (344 bytes)

void _ChangeHiddenPiracyValue(byte param_1)

{
  undefined2 local_22;
  ushort local_20;
  short local_1e;
  short local_1c;
  short local_1a;
  undefined4 local_18;
  int local_14;
  undefined4 local_10;
  
  local_18 = 0;
  _GetDateFromHiddenFile(&local_1a,&local_1c,&local_1e,&local_20);
  if (((local_1a == 0) && (local_1c == 0)) && (local_1e == 0)) {
    _MMGetCurrentDate(&local_1a,&local_1c,&local_1e);
  }
  local_20 = (ushort)param_1;
  local_10 = _GetRealPrefsFilePath("MM HD Data New");
  local_14 = _SDL_RWFromFile(local_10,"r+");
  if (local_14 != 0) {
    _ClearFileStickyError();
    local_22 = 0x1c9d;
    _MyWriteDataToFile(local_14,2,&local_22);
    _MyWriteDataToFile(local_14,2,&local_1a);
    _MyWriteDataToFile(local_14,2,&local_1c);
    _MyWriteDataToFile(local_14,2,&local_1e);
    _MyWriteDataToFile(local_14,2,&local_20);
    local_18 = _GetFileStickyError();
    (**(code **)(local_14 + 0xc))(local_14);
  }
  return;
}


// ---- _IsComputerClockValid @ 0x7f851 (166 bytes)

bool _IsComputerClockValid(void)

{
  short sVar1;
  undefined1 local_22 [2];
  short local_20;
  short local_1e;
  short local_1c;
  short local_1a;
  short local_18;
  short local_16;
  int local_14;
  
  _GetDateFromHiddenFile(&local_16,&local_18,&local_1a,local_22);
  _MMGetCurrentDate(&local_1c,&local_1e,&local_20);
  sVar1 = _GetNumDaysBeforeMonth((int)local_18);
  local_14 = local_1a * 0x16d + (int)sVar1 + (int)local_16;
  sVar1 = _GetNumDaysBeforeMonth((int)local_1e);
  return local_14 <= local_20 * 0x16d + (int)sVar1 + (int)local_1c;
}


// ---- _SetMMRegistered @ 0x7f8f7 (40 bytes)

void _SetMMRegistered(undefined1 param_1)

{
  *PTR__gIsRegistered_001c4fc8 = param_1;
  return;
}


// ---- _LaunchRegistrationWebsite @ 0x7f91f (59 bytes)

void _LaunchRegistrationWebsite(void)

{
  if (*PTR__gFullScreenWindow_001c48ec != '\0') {
    _ToggleFullscreen();
  }
  _ASWLaunchURL("http://www.actionsoft.com/games/midnightmansionhd/register.php");
  return;
}


