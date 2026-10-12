// Ghidra decompilation of functions from original source file:
//   Sources & Headers/MMEvents.c

// ---- _ProcessSDLEvents @ 0x46f7c (42 bytes)

void _ProcessSDLEvents(void)

{
  int iVar1;
  undefined1 local_3c [56];
  
  while( true ) {
    iVar1 = _SDL_PollEvent(local_3c);
    if (iVar1 == 0) break;
    _MyHandleSDLEvent(local_3c);
  }
  return;
}


// ---- _MyHandleSDLEvent @ 0x46fa6 (2222 bytes)

void _MyHandleSDLEvent(uint *param_1)

{
  uint uVar1;
  char cVar2;
  short sVar3;
  int iVar4;
  undefined4 uVar5;
  char local_11;
  
  if ((*(int *)PTR__gMMEventCallback_001c4af0 == 0) ||
     (cVar2 = (**(code **)PTR__gMMEventCallback_001c4af0)(param_1), cVar2 == '\0')) {
    uVar1 = *param_1;
    if (uVar1 == 0x401) {
      *PTR__gMouseButton_001c4ae8 = 1;
      if ((*(int *)PTR__gCurrentGameMode_001c42d0 == 6) &&
         (*PTR__gIsEnteringMapScreen_001c4aec == '\0')) {
        _HandleMapScreenMouseButton
                  ((char)param_1[2],1,(int)(short)param_1[3],(int)(short)param_1[4]);
      }
      if (*(int *)PTR__gCurrentGameMode_001c42d0 == 9) {
        *PTR__gBeatMansionScreenWasDismissed_001c4acc = 1;
      }
    }
    else if (uVar1 < 0x402) {
      if (uVar1 == 0x300) {
        uVar5 = _ConvertKeyScanCode(param_1[3]);
        if ((param_1[5] & 0xc00) == 0) {
          if (*(char *)((int)param_1 + 9) == '\0') {
            _UpdateMovementKeys(uVar5,param_1[4],1,*(undefined1 *)((int)param_1 + 9));
          }
          if ((*(char *)((int)param_1 + 9) == '\0') &&
             (cVar2 = _HandleKeyUpDownEvent(uVar5,param_1[4],1,*(undefined1 *)((int)param_1 + 9)),
             cVar2 != '\0')) {
            return;
          }
        }
        if ((((param_1[5] & 0xc0) != 0) && (param_1[4] == 0x7a)) && ((param_1[5] & 0xc0) != 0)) {
          _SDL_MinimizeWindow(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158));
        }
      }
      else if (uVar1 < 0x301) {
        if (uVar1 == 0x100) {
          if ((*(int *)PTR__gCurrentGameMode_001c42d0 == 5) ||
             (*(int *)PTR__gCurrentGameMode_001c42d0 == 6)) {
            sVar3 = _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                                      "Are you sure you want to quit?",1);
            if (sVar3 == 2) {
              *PTR__gQuitGame_001c49d0 = 1;
            }
          }
          else {
            *PTR__gQuitGame_001c49d0 = 1;
          }
        }
        else if (uVar1 == 0x200) {
          switch((char)param_1[2]) {
          case '\x05':
          case '\b':
          case '\t':
            _puts("Window resized event.");
            _HandleWindowResizeEvent(param_1,param_1[3],param_1[4]);
            break;
          case '\n':
            *PTR__gCursorIsInWindow_001c47a4 = 1;
            _MMHandleCursorMovement();
            break;
          case '\v':
            *PTR__gCursorIsInWindow_001c47a4 = 0;
            _MMHandleCursorMovement();
            break;
          case '\f':
            _puts("Activate event.");
            _HandleActivateEvent();
            _SDL_SetModState(0);
            break;
          case '\r':
            _puts("Deactivate event.");
            _HandleDeactivateEvent();
          }
        }
      }
      else if (uVar1 == 0x301) {
        uVar5 = _ConvertKeyScanCode(param_1[3]);
        if (*(char *)((int)param_1 + 9) == '\0') {
          _UpdateMovementKeys(uVar5,param_1[4],0,*(undefined1 *)((int)param_1 + 9));
        }
        _HandleKeyUpDownEvent(uVar5,param_1[4],0,*(undefined1 *)((int)param_1 + 9));
      }
      else if (uVar1 == 0x400) {
        if ((*(int *)PTR__gCurrentGameMode_001c42d0 == 6) &&
           (*PTR__gIsEnteringMapScreen_001c4aec == '\0')) {
          _HandleMapScreenMouseMoved((int)(short)param_1[3],(int)(short)param_1[4]);
        }
        _UpdateMyMouseLoc((int)(short)param_1[3],(int)(short)param_1[4]);
      }
    }
    else if (uVar1 == 0x600) {
      if (*(char *)((int)param_1 + 5) == '\0') {
        PTR__gKeys_001c4af8[1] = 0;
        PTR__gKeys_001c4af8[3] = PTR__gKeys_001c4af8[1];
        if ((int)param_1[2] < -3000) {
          PTR__gKeys_001c4af8[3] = 1;
        }
        else if (3000 < (int)param_1[2]) {
          PTR__gKeys_001c4af8[1] = 1;
        }
      }
      if (*(char *)((int)param_1 + 5) == '\x01') {
        PTR__gKeys_001c4af8[2] = 0;
        *PTR__gKeys_001c4af8 = PTR__gKeys_001c4af8[2];
        if ((int)param_1[2] < -3000) {
          *PTR__gKeys_001c4af8 = 1;
        }
        else if (3000 < (int)param_1[2]) {
          PTR__gKeys_001c4af8[2] = 1;
        }
      }
    }
    else if (uVar1 < 0x601) {
      if (uVar1 == 0x402) {
        *PTR__gMouseButton_001c4ae8 = 0;
        if ((*(int *)PTR__gCurrentGameMode_001c42d0 == 6) &&
           (*PTR__gIsEnteringMapScreen_001c4aec == '\0')) {
          _HandleMapScreenMouseButton
                    ((char)param_1[2],0,(int)(short)param_1[3],(int)(short)param_1[4]);
        }
      }
      else if (((uVar1 == 0x403) && (*(int *)PTR__gCurrentGameMode_001c42d0 == 6)) &&
              (*PTR__gIsEnteringMapScreen_001c4aec == '\0')) {
        _HandleMapScreenScrollWheelEvent((float)(int)param_1[3]);
      }
    }
    else if (uVar1 - 0x603 < 2) {
      if (*(byte *)((int)param_1 + 5) < 4) {
        if (*(char *)((int)param_1 + 5) == '\x02') {
          PTR__gGamepad_001c4ac8[3] = *(undefined1 *)((int)param_1 + 6);
        }
        else if (*(char *)((int)param_1 + 5) == '\x03') {
          PTR__gGamepad_001c4ac8[1] = *(undefined1 *)((int)param_1 + 6);
        }
        else if (*(char *)((int)param_1 + 5) == '\0') {
          *PTR__gGamepad_001c4ac8 = *(undefined1 *)((int)param_1 + 6);
        }
        else if (*(char *)((int)param_1 + 5) == '\x01') {
          PTR__gGamepad_001c4ac8[2] = *(undefined1 *)((int)param_1 + 6);
        }
        PTR__gKeys_001c4af8[3] = PTR__gGamepad_001c4ac8[3];
        PTR__gKeys_001c4af8[1] = PTR__gGamepad_001c4ac8[1];
        *PTR__gKeys_001c4af8 = *PTR__gGamepad_001c4ac8;
        iVar4 = _GetHeroSprite();
        if (iVar4 == 0) {
          local_11 = '\x01';
        }
        else {
          local_11 = *(char *)(iVar4 + 0x17e);
        }
        if ((((PTR__gGamepad_001c4ac8[3] == '\0') && (PTR__gGamepad_001c4ac8[1] == '\0')) ||
            (*(int *)PTR__gCurrentGameMode_001c42d0 == 6)) || (local_11 == '\0')) {
          PTR__gKeys_001c4af8[2] = PTR__gGamepad_001c4ac8[2];
        }
        else {
          PTR__gKeys_001c4af8[2] = 0;
        }
      }
      if ((*(char *)((int)param_1 + 5) == '\x04') || (*(char *)((int)param_1 + 5) == '\x05')) {
        _UpdateMovementKeys(*(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x10),
                            *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x34),
                            *(undefined1 *)((int)param_1 + 6),0);
      }
      else if (*(char *)((int)param_1 + 5) == '\x06') {
        _UpdateMovementKeys(*(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x1c),
                            *(undefined4 *)(PTR__gKeyPrefs_001c46f4 + 0x40),
                            *(undefined1 *)((int)param_1 + 6),0);
      }
      if ((*(char *)((int)param_1 + 5) == '\a') && (*(char *)((int)param_1 + 6) == '\x01')) {
        if ((*(int *)PTR__gCurrentGameMode_001c42d0 == 6) &&
           (*PTR__gIsEnteringMapScreen_001c4aec == '\0')) {
          _ExitInGameMapScreen(1);
        }
        else if (*(int *)PTR__gCurrentGameMode_001c42d0 == 5) {
          _SetUpInGameMapScreen();
        }
      }
      if (*(int *)PTR__gCurrentGameMode_001c42d0 == 6) {
        if ((*(char *)((int)param_1 + 5) == '\v') || (*(char *)((int)param_1 + 5) == '\r')) {
          PTR__gKeys_001c4af8[9] = *(undefined1 *)((int)param_1 + 6);
        }
        else if ((*(char *)((int)param_1 + 5) == '\f') || (*(char *)((int)param_1 + 5) == '\x0e')) {
          PTR__gKeys_001c4af8[10] = *(undefined1 *)((int)param_1 + 6);
        }
      }
    }
  }
  return;
}


// ---- _printTileMap @ 0x47854 (300 bytes)

void printTileMap(short param_1)

{
  short *psVar1;
  short sStack_12;
  short sStack_10;
  
  psVar1 = *(short **)
            (*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) +
            *(short *)PTR__gCurTileLayerToPrint_001c4adc * 4);
  _printf("Tile layer: %d \n\n",(int)param_1);
  for (sStack_12 = 0; sStack_12 < *psVar1; sStack_12 = sStack_12 + 1) {
    for (sStack_10 = 0; sStack_10 < psVar1[1]; sStack_10 = sStack_10 + 1) {
      _printf("%d ",(int)*(short *)(*(int *)(*(int *)(psVar1 + 2) + sStack_12 * 4) + sStack_10 * 2))
      ;
    }
    _putchar(10);
  }
  *(short *)PTR__gCurTileLayerToPrint_001c4adc = *(short *)PTR__gCurTileLayerToPrint_001c4adc + 1;
  if (6 < *(short *)PTR__gCurTileLayerToPrint_001c4adc) {
    *(undefined2 *)PTR__gCurTileLayerToPrint_001c4adc = 0;
  }
  return;
}


// ---- _LoadRoomToSideOfCurRoom @ 0x47980 (243 bytes)

void LoadRoomToSideOfCurRoom(short param_1,short param_2)

{
  char cVar1;
  short sStack_14;
  short sStack_12;
  short sStack_10;
  short sStack_e;
  
  _GEGetRowAndColFromLevelNumber
            ((int)*(short *)PTR__gCurrentLevel_001c49bc,&sStack_12,&sStack_14,200);
  sStack_12 = sStack_12 + param_2;
  sStack_14 = sStack_14 + param_1;
  sStack_e = _GEGetLevelNumberFromRowAndCol((int)sStack_12,(int)sStack_14,200);
  sStack_10 = _GEGetLevelResID((int)sStack_e,1,200);
  cVar1 = _Resource1Exists("TMAP",(int)sStack_10);
  if (cVar1 != '\0') {
    _LoadLevel((int)sStack_e,1);
    _SetUpLevelForPlaying();
    _DrawLevel();
    _DoGameFadeToNormal(0);
  }
  return;
}


// ---- _SetStretchToFit @ 0x47a73 (216 bytes)

void _SetStretchToFit(uchar param_1)

{
  undefined4 local_14;
  undefined4 local_10 [2];
  
  _SWSetSpriteWorldStretchToFit(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,param_1);
  if (*(int *)PTR__gStatsSpriteWorldP_001c48f4 != 0) {
    _SWSetSpriteWorldStretchToFit(*(SpriteWorldRec **)PTR__gStatsSpriteWorldP_001c48f4,param_1);
  }
  if (*PTR__gFullScreenWindow_001c48ec != '\0') {
    _SDL_GetWindowSize(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158),local_10,
                       &local_14);
    _DoWindowResize(local_10[0],local_14,0);
    _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
  }
  *PTR__gStretchToFit_001c49dc = param_1;
  return;
}


// ---- _SetMMEventCallback @ 0x47b4b (33 bytes)

void _SetMMEventCallback(undefined4 param_1)

{
  *(undefined4 *)PTR__gMMEventCallback_001c4af0 = param_1;
  return;
}


// ---- _HandleWindowResizeEvent @ 0x47b6c (212 bytes)

void _HandleWindowResizeEvent(int param_1,undefined4 param_2,undefined4 param_3)

{
  if (*(char *)(param_1 + 8) == '\x05') {
    param_2 = *(undefined4 *)(param_1 + 0xc);
    param_3 = *(undefined4 *)(param_1 + 0x10);
  }
  else {
    _SDL_GetWindowSize(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158),&param_2,
                       &param_3);
  }
  _printf("\nWindow resize event! width = %d, height = %d \n",param_2,param_3);
  _DoWindowResize(param_2,param_3,1);
  if (*PTR__gFullScreenWindow_001c48ec == '\0') {
    *(undefined2 *)PTR__gWindowedResolutionX_001c4880 =
         *(undefined2 *)PTR__gPrevWindowWidth_001c4840;
    *(undefined2 *)PTR__gWindowedResolutionY_001c4854 =
         *(undefined2 *)PTR__gPrevWindowHeight_001c49b4;
    _WritePreferencesFile();
  }
  return;
}


// ---- _DoWindowResize @ 0x47c40 (721 bytes)

void _DoWindowResize(int param_1,int param_2,undefined4 param_3)

{
  char cVar1;
  char cVar2;
  undefined1 local_40 [12];
  int local_34;
  uint local_30;
  uint local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  char local_11;
  float local_10;
  
  cVar1 = (char)param_3;
  local_11 = '\0';
  local_10 = 1.3333334;
  local_30 = *(short *)PTR__gPrevWindowWidth_001c4840 - param_1;
  local_2c = *(short *)PTR__gPrevWindowHeight_001c49b4 - param_2;
  local_20 = (int)((float)param_1 / 1.3333334);
  local_1c = (int)((float)param_2 * 1.3333334);
  _SDL_GetDisplayBounds(0,local_40);
  local_18 = local_34 + -0x7e;
  if ((int)(((int)local_2c >> 0x1f ^ local_2c) - ((int)local_2c >> 0x1f)) <
      (int)(((int)local_30 >> 0x1f ^ local_30) - ((int)local_30 >> 0x1f))) {
    param_2 = local_20;
  }
  else {
    param_1 = local_1c;
  }
  if (local_18 < param_2) {
    param_1 = (int)((float)local_18 * local_10);
    local_11 = '\x01';
    param_2 = local_18;
  }
  if ((param_1 < 0x280) || (param_2 < 0x1e0)) {
    param_1 = 0x280;
    param_2 = 0x1e0;
  }
  _SDL_SetWindowSize(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158),param_1,param_2);
  local_28 = param_1;
  local_24 = param_2;
  _SDL_GetWindowSize(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158),&param_1,&param_2)
  ;
  if (param_2 < local_24) {
    param_1 = (int)((float)param_2 * local_10);
    _SDL_SetWindowSize(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158),param_1,param_2)
    ;
  }
  if (local_11 != '\0') {
    _SDL_SetWindowPosition
              (*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158),0x2fff0000,0x2fff0000);
  }
  *(short *)PTR__gPrevWindowWidth_001c4840 = (short)param_1;
  *(short *)PTR__gPrevWindowHeight_001c49b4 = (short)param_2;
  _SDL_FlushEvents(0x200,0x200);
  _SDL_GL_MakeCurrent(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158),
                      *(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x15c));
  _SWUpdateWindowSize(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0);
  cVar2 = _IsStatsAreaVisible();
  if ((cVar2 != '\0') && (*(int *)PTR__gStatsSpriteWorldP_001c48f4 != 0)) {
    _SWUpdateWindowSize(*(SpriteWorldRec **)PTR__gStatsSpriteWorldP_001c48f4);
  }
  if (cVar1 != '\0') {
    _DoGameFadeToBlack(0,0);
  }
  if (*(int *)PTR__gCurrentGameMode_001c42d0 == 4) {
    _DrawContentsForHighScoresScreen((int)*(short *)PTR__gCurrentHighScoresScreenIndex_001c4afc);
  }
  else {
    _DrawBackgroundTextureForCurRoom();
  }
  if (cVar1 != '\0') {
    _DoGameFadeToNormal(0);
  }
  return;
}


// ---- _HandleKeyUpDownEvent @ 0x47f11 (932 bytes)

undefined4 _HandleKeyUpDownEvent(uint param_1,int param_2,char param_3)

{
  char cVar1;
  undefined4 local_28;
  
  if (*PTR__gCustomDialogIsVisible_001c49b8 != '\0') {
    _HandleCustomDialogKeyEvent(param_1,param_3);
  }
  if ((param_3 == '\0') || (*(int *)PTR__gCurrentGameMode_001c42d0 != 9)) {
    if ((param_3 == '\0') || (*(int *)PTR__gCurrentGameMode_001c42d0 != 10)) {
      if ((param_3 == '\0') || (*(int *)PTR__gCurrentGameMode_001c42d0 != 0xe)) {
        if ((param_3 != '\0') && (*(int *)PTR__gCurrentGameMode_001c42d0 == 0xb)) {
          *PTR__gOrganScreenWasDismissed_001c4ad0 = 1;
          return 1;
        }
        if ((param_3 != '\0') && (*(int *)PTR__gCurrentGameMode_001c42d0 == 0xc)) {
          *PTR__gStoryScreenWasDismissed_001c4ae4 = 1;
          return 1;
        }
      }
      else {
        *PTR__gReadingSignWasDismissed_001c4ae0 = 1;
        _WriteEventToMovieFile(0x2e,0);
      }
    }
    else {
      *PTR__gBeatGameScreenWasDismissed_001c4ad4 = 1;
    }
  }
  else {
    *PTR__gBeatMansionScreenWasDismissed_001c4acc = 1;
  }
  if (*PTR__gDialogBoxIsOpen_001c49c8 == '\0') {
    if (((*PTR__gGameIsPaused_001c484c != '\0') && (param_3 != '\0')) &&
       (((0x60 < param_2 && (param_1 < 0x7b)) || (param_2 == 0x20)))) {
      _AddKeyToCheatCodeCache(param_2);
    }
    if ((param_2 == 0x4000004f) && (cVar1 = _IsMoviePlaying(), cVar1 == '\x01')) {
      if (param_3 == '\0') {
        _SWSetSpriteWorldMaxFPS(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x3c);
      }
      else {
        _SWSetSpriteWorldMaxFPS(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x78);
      }
    }
    if ((param_2 == 0x40000052) && (cVar1 = _IsMoviePlaying(), cVar1 == '\x01')) {
      if (param_3 == '\0') {
        _SWSetSpriteWorldMaxFPS(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0x3c);
      }
      else {
        _SWSetSpriteWorldMaxFPS(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,0);
      }
    }
    if (*PTR__gGameIsPaused_001c484c == '\0') {
      if (param_3 != '\0') {
        if (*(int *)PTR__gCurrentGameMode_001c42d0 == 6) {
          if ((((*(uint *)(PTR__gKeyPrefs_001c46f4 + 0x20) == param_1) ||
               (*(uint *)(PTR__gKeyPrefs_001c46f4 + 0x1c) == param_1)) ||
              ((param_2 == 0xd || ((param_2 == 0x20 || (param_2 == 0x1b)))))) &&
             (*PTR__gIsEnteringMapScreen_001c4aec == '\0')) {
            _ExitInGameMapScreen(1);
            return 1;
          }
        }
        else if (((*(int *)PTR__gCurrentGameMode_001c42d0 == 5) &&
                 (*(uint *)(PTR__gKeyPrefs_001c46f4 + 0x20) == param_1)) &&
                (*PTR__gCustomDialogIsVisible_001c49b8 == '\0')) {
          _SetUpInGameMapScreen();
        }
      }
      if (((param_2 == 0x3d) || (param_2 == 0x2b)) || (param_2 == 0x40000057)) {
        if (*(int *)PTR__gCurrentGameMode_001c42d0 == 6) {
          PTR__gKeys_001c4af8[9] = param_3;
        }
      }
      else if ((((param_2 == 0x2d) || (param_2 == 0x40000056)) || (param_2 == 0x40000067)) &&
              (*(int *)PTR__gCurrentGameMode_001c42d0 == 6)) {
        PTR__gKeys_001c4af8[10] = param_3;
      }
    }
    if ((((param_2 == 0x1b) && (*PTR__gCustomDialogIsVisible_001c49b8 == '\0')) &&
        ((*PTR__gEscapeKey_001c4734 = param_3, *(int *)PTR__gCurrentGameMode_001c42d0 == 5 ||
         (*(int *)PTR__gCurrentGameMode_001c42d0 == 0xe)))) && (param_3 != '\0')) {
      if (*PTR__gEditorIsRunning_001c495c == '\0') {
        _TogglePause();
      }
      else {
        _ExitToEditor();
      }
    }
    local_28 = 1;
  }
  else {
    local_28 = 0;
  }
  return local_28;
}


// ---- _UpdateMovementKeys @ 0x482b5 (569 bytes)

void _UpdateMovementKeys(int param_1,undefined4 param_2,undefined1 param_3,char param_4)

{
  char cVar1;
  
  cVar1 = _IsMovieFileOpenForRecording();
  if (((cVar1 != '\0') && (*PTR__gDialogBoxIsOpen_001c49c8 == '\0')) &&
     ((*(int *)PTR__gCurrentGameMode_001c42d0 == 5 ||
      (*(int *)PTR__gCurrentGameMode_001c42d0 == 0xe)))) {
    _AddKeyToMovie(param_1,param_3,param_4);
  }
  cVar1 = _IsMoviePlaying();
  if ((cVar1 == '\0') || (*(int *)PTR__gCurrentGameMode_001c42d0 == 6)) {
    if (*PTR__gArrowKeysEnabled_001c48a0 != '\0') {
      if (param_1 == 0x50) {
        PTR__gKeys_001c4af8[3] = param_3;
      }
      else if (param_1 == 0x4f) {
        PTR__gKeys_001c4af8[1] = param_3;
      }
      else if (param_1 == 0x51) {
        PTR__gKeys_001c4af8[2] = param_3;
      }
      else if (param_1 == 0x52) {
        *PTR__gKeys_001c4af8 = param_3;
      }
      else if ((param_1 == 0x2c) && (param_4 == '\0')) {
        PTR__gKeys_001c4af8[4] = param_3;
      }
    }
    if (*(int *)(PTR__gKeyPrefs_001c46f4 + 0xc) == param_1) {
      PTR__gKeys_001c4af8[3] = param_3;
    }
    else if (*(int *)(PTR__gKeyPrefs_001c46f4 + 4) == param_1) {
      PTR__gKeys_001c4af8[1] = param_3;
    }
    else if (*(int *)(PTR__gKeyPrefs_001c46f4 + 8) == param_1) {
      PTR__gKeys_001c4af8[2] = param_3;
    }
    else if (*(int *)PTR__gKeyPrefs_001c46f4 == param_1) {
      *PTR__gKeys_001c4af8 = param_3;
    }
    else if ((*(int *)(PTR__gKeyPrefs_001c46f4 + 0x10) == param_1) && (param_4 == '\0')) {
      PTR__gKeys_001c4af8[4] = param_3;
    }
    else if ((*(int *)(PTR__gKeyPrefs_001c46f4 + 0x14) == param_1) && (param_4 == '\0')) {
      PTR__gKeys_001c4af8[5] = param_3;
    }
    else if ((*(int *)(PTR__gKeyPrefs_001c46f4 + 0x18) == param_1) && (param_4 == '\0')) {
      PTR__gKeys_001c4af8[6] = param_3;
    }
    else if (*(int *)(PTR__gKeyPrefs_001c46f4 + 0x1c) == param_1) {
      PTR__gKeys_001c4af8[7] = param_3;
    }
  }
  return;
}


// ---- _HandleActivateEvent @ 0x484ee (325 bytes)

void _HandleActivateEvent(void)

{
  char cVar1;
  undefined1 local_20;
  int local_14;
  int local_10 [2];
  
  _MMFlushEvents();
  if (*PTR__gWasFullscreenWhenMinimized_001c47bc != '\0') {
    _puts("Window WAS fullscreen when deactivated, so toggling fullscreen!");
    _ProcessSDLEvents();
    if (*PTR__gFullScreenWindow_001c48ec == '\0') {
      _ToggleFullscreen();
    }
    *PTR__gWasFullscreenWhenMinimized_001c47bc = 0;
  }
  _SDL_GetMouseState(local_10,&local_14);
  if ((local_10[0] == *(int *)PTR__gMouseLocX_001c4b00) &&
     (local_14 == *(int *)PTR__gMouseLocY_001c4ad8)) {
    local_20 = 0;
  }
  else {
    local_20 = 1;
  }
  *PTR__gCursorIsInWindow_001c47a4 = local_20;
  if ((*PTR__musicWasPlaying_001c4af4 != '\0') && (*PTR__gGameIsPaused_001c484c == '\0')) {
    _ResumeMusic(0);
  }
  if (*PTR__gUsingEditor_001c48b0 == '\0') {
    if (*PTR__gDialogBoxIsOpen_001c49c8 == '\0') {
      cVar1 = _CurrentScreenHasCustomCursor();
      if ((cVar1 != '\0') || (*PTR__gGameIsPaused_001c484c != '\0')) {
        _MMSetCursorForGame();
        _MMShieldCursor();
      }
    }
  }
  else {
    _MMRemoveGameCursor();
    _MMShieldCursor();
  }
  if (*(int *)PTR__gCurrentGameMode_001c42d0 == 0) {
    _PlayCricketSound();
  }
  *PTR__gMMIsActive_001c49e0 = 1;
  return;
}


// ---- _HandleDeactivateEvent @ 0x48633 (152 bytes)

void _HandleDeactivateEvent(void)

{
  undefined1 uVar1;
  
  if (*PTR__gMMIsActive_001c49e0 != '\0') {
    if (*PTR__gPlayingGame_001c4924 == '\0') {
      uVar1 = _IsMusicPlaying();
      *PTR__musicWasPlaying_001c4af4 = uVar1;
      _PauseMusic();
      _StopAllChannels();
      _printf("Music was playing: %d\n",(uint)(byte)*PTR__musicWasPlaying_001c4af4);
    }
    else if (*PTR__gCustomDialogIsVisible_001c49b8 == '\0') {
      _PauseGame();
    }
    _MMSetCursorForSystem();
    *PTR__gMMIsActive_001c49e0 = 0;
  }
  return;
}


// ---- _WasMouseClicked @ 0x486cb (154 bytes)

undefined4 _WasMouseClicked(void)

{
  int local_404 [252];
  int local_14;
  int local_10;
  
  _SDL_PumpEvents();
  local_14 = _SDL_PeepEvents(local_404,0x14,1,0x401,0x401);
  local_10 = 0;
  while( true ) {
    if (local_14 <= local_10) {
      return 0;
    }
    if (local_404[local_10 * 0xc] == 0x401) break;
    local_10 = local_10 + 1;
  }
  return 1;
}


// ---- _WasAKeyPressed @ 0x48765 (217 bytes)

undefined4 _WasAKeyPressed(void)

{
  char cVar1;
  int local_404 [252];
  int local_14;
  int local_10;
  
  _SDL_PumpEvents();
  local_14 = _SDL_PeepEvents(local_404,0x14,1,0x300,0x300);
  local_10 = 0;
  while( true ) {
    if (local_14 <= local_10) {
      return 0;
    }
    if ((local_404[local_10 * 0xc] == 0x300) &&
       (cVar1 = _IsKeyModifier(local_404[local_10 * 0xc + 3]), cVar1 == '\0')) break;
    local_10 = local_10 + 1;
  }
  _printf("A key was pushed!");
  return 1;
}


// ---- _IsKeyModifier @ 0x4883e (109 bytes)

undefined1 _IsKeyModifier(int param_1)

{
  undefined1 local_10;
  
  if (((((param_1 == 0xe0) || (param_1 == 0xe1)) || (param_1 == 0xe2)) ||
      ((param_1 == 0xe3 || (param_1 == 0xe4)))) ||
     ((param_1 == 0xe5 || ((param_1 == 0xe6 || (param_1 == 0xe7)))))) {
    local_10 = 1;
  }
  else {
    local_10 = 0;
  }
  return local_10;
}


// ---- _WasGameQuit @ 0x488ab (346 bytes)

undefined1 _WasGameQuit(void)

{
  int iVar1;
  int local_404 [2];
  char acStack_3fc [8];
  int iStack_3f4;
  ushort auStack_3f0 [494];
  int local_14;
  int local_10;
  
  _SDL_PumpEvents();
  local_14 = _SDL_PeepEvents(local_404,0x14,1,0,0xffff);
  for (local_10 = 0; local_10 < local_14; local_10 = local_10 + 1) {
    iVar1 = local_404[local_10 * 0xc];
    if (iVar1 == 0x200) {
      if (acStack_3fc[local_10 * 0x30] == '\x0e') {
        *PTR__gQuitGame_001c49d0 = 1;
      }
    }
    else if (iVar1 == 0x300) {
      if (((&iStack_3f4)[local_10 * 0xc] == 0x71) && ((auStack_3f0[local_10 * 0x18] & 0xc00) != 0))
      {
        *PTR__gQuitGame_001c49d0 = 1;
      }
    }
    else if (iVar1 == 0x100) {
      *PTR__gQuitGame_001c49d0 = 1;
    }
  }
  return *PTR__gQuitGame_001c49d0;
}


// ---- _MMFlushEvents @ 0x48a05 (34 bytes)

void _MMFlushEvents(void)

{
  _SDL_FlushEvents(0,0xffff);
  return;
}


// ---- _UpdateMyMouseLoc @ 0x48a27 (62 bytes)

void _UpdateMyMouseLoc(short param_1,short param_2)

{
  *(int *)PTR__gMouseLocX_001c4b00 = (int)param_1;
  *(int *)PTR__gMouseLocY_001c4ad8 = (int)param_2;
  return;
}


// ---- _ResetMyMouseLocAndButton @ 0x48a65 (64 bytes)

void ResetMyMouseLocAndButton(void)

{
  *PTR__gMouseButton_001c4ae8 = 0;
  _SDL_GetMouseState(PTR__gMouseLocX_001c4b00,PTR__gMouseLocY_001c4ad8);
  return;
}


// ---- _GetMyMouse @ 0x48aa5 (154 bytes)

undefined1 GetMyMouse(undefined2 *param_1)

{
  float fStack_14;
  float afStack_10 [2];
  
  _SWGetWindowScalingFactor(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,afStack_10,&fStack_14);
  param_1[1] = (short)(int)((float)*(int *)PTR__gMouseLocX_001c4b00 / afStack_10[0]);
  *param_1 = (short)(int)((float)*(int *)PTR__gMouseLocY_001c4ad8 / fStack_14);
  return *PTR__gMouseButton_001c4ae8;
}


// ---- _MMGetMouse @ 0x48b3f (154 bytes)

byte _MMGetMouse(undefined2 *param_1)

{
  byte bVar1;
  float local_20;
  float local_1c;
  int local_18;
  int local_14 [3];
  
  _SWGetWindowScalingFactor(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,&local_1c,&local_20);
  bVar1 = _SDL_GetMouseState(local_14,&local_18);
  param_1[1] = (short)(int)((float)local_14[0] / local_1c);
  *param_1 = (short)(int)((float)local_18 / local_20);
  return bVar1 & 1;
}


// ---- _IsMouseDown @ 0x48bd9 (38 bytes)

uint _IsMouseDown(void)

{
  uint uVar1;
  undefined1 local_14 [4];
  undefined1 local_10 [12];
  
  uVar1 = _SDL_GetMouseState(local_10,local_14);
  return uVar1 & 1;
}


// ---- _ResetKeys @ 0x48bff (196 bytes)

void _ResetKeys(void)

{
  undefined *puVar1;
  undefined *puVar2;
  
  PTR__gKeys_001c4af8[3] = 0;
  PTR__gKeys_001c4af8[1] = 0;
  PTR__gKeys_001c4af8[2] = 0;
  *PTR__gKeys_001c4af8 = 0;
  PTR__gKeys_001c4af8[4] = 0;
  PTR__gKeys_001c4af8[5] = 0;
  PTR__gKeys_001c4af8[6] = 0;
  PTR__gKeys_001c4af8[7] = 0;
  PTR__gKeys_001c4af8[9] = 0;
  PTR__gKeys_001c4af8[10] = 0;
  puVar2 = PTR__gKeys_001c4af8;
  puVar1 = PTR__gGamepad_001c4ac8;
  *(undefined4 *)PTR__gGamepad_001c4ac8 = *(undefined4 *)PTR__gKeys_001c4af8;
  *(undefined4 *)(puVar1 + 4) = *(undefined4 *)(puVar2 + 4);
  *(undefined2 *)(puVar1 + 8) = *(undefined2 *)(puVar2 + 8);
  puVar1[10] = puVar2[10];
  *PTR__gEscapeKey_001c4734 = 0;
  return;
}


// ---- _ToggleFullscreen @ 0x48cc3 (435 bytes)

void _ToggleFullscreen(void)

{
  char cVar1;
  undefined4 uVar2;
  undefined1 local_20 [4];
  int local_1c;
  int local_18;
  
  while (cVar1 = _IsMouseDown(), cVar1 != '\0') {
    _ProcessSDLEvents();
  }
  *PTR__gFullScreenWindow_001c48ec = *PTR__gFullScreenWindow_001c48ec == '\0';
  uVar2 = _SDL_GetWindowDisplay(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158));
  _SDL_GetDesktopDisplayMode(uVar2,local_20);
  if (*PTR__gLimitFullscreenResolution_001c4720 != '\0') {
    if ((float)local_1c / (float)local_18 == 1.6) {
      local_1c = 0x400;
      local_18 = 0x280;
    }
    else {
      local_1c = 0x400;
      local_18 = 0x300;
    }
  }
  _SDL_SetWindowDisplayMode(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158),local_20);
  _SDL_SetWindowFullscreen
            (*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158),
             *PTR__gFullScreenWindow_001c48ec);
  if (*PTR__gFullScreenWindow_001c48ec == '\0') {
    _SDL_SetWindowSize(*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158),
                       (int)*(short *)PTR__gWindowedResolutionX_001c4880,
                       (int)*(short *)PTR__gWindowedResolutionY_001c4854);
    _DoWindowResize((int)*(short *)PTR__gWindowedResolutionX_001c4880,
                    (int)*(short *)PTR__gWindowedResolutionY_001c4854,0);
    _SDL_SetWindowPosition
              (*(undefined4 *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x158),0x2fff0000,0x2fff0000);
  }
  return;
}


// ---- _ToggleMusic @ 0x48e76 (165 bytes)

void _ToggleMusic(void)

{
  char cVar1;
  
  if (*PTR__gGameIsPaused_001c484c == '\0') {
    cVar1 = _IsMusicPlaying();
    if (cVar1 == '\0') {
      _ResumeMusic(1);
      *PTR__gMusicIsOn_001c47c0 = 1;
    }
    else {
      _PauseMusic();
      *PTR__gMusicIsOn_001c47c0 = 0;
    }
  }
  else {
    *PTR__gMusicWasPlaying_001c4778 = *PTR__gMusicWasPlaying_001c4778 == '\0';
    *PTR__gResetMusic_001c46ec = 1;
    *PTR__gMusicIsOn_001c47c0 = *PTR__gMusicIsOn_001c47c0 == '\0';
  }
  _WritePreferencesFile();
  _UpdateMenuBar();
  return;
}


// ---- _ToggleStats @ 0x48f1b (106 bytes)

void _ToggleStats(void)

{
  *PTR__gDisplayFPSAndLevelResID_001c4960 = *PTR__gDisplayFPSAndLevelResID_001c4960 == '\0';
  if (*(int *)PTR__gCurrentGameMode_001c42d0 == 5) {
    if (*PTR__gDisplayFPSAndLevelResID_001c4960 == '\0') {
      _RemoveAllSpritesInLayer(*(undefined4 *)PTR__gFPSDisplaySpriteLayerP_001c41a8);
    }
    else {
      _CreateFPSAndLevelResIDSprites();
    }
  }
  return;
}


// ---- _GiveOSTime @ 0x48f85 (26 bytes)

void _GiveOSTime(void)

{
  _SDL_Delay(1);
  return;
}


// ---- _KeepMusicPlaying @ 0x48f9f (26 bytes)

void _KeepMusicPlaying(void)

{
  _SDL_Delay(10);
  return;
}


