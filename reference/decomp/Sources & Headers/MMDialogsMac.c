// Ghidra decompilation of functions from original source file:
//   Sources & Headers/MMDialogsMac.c

// ---- _OpenMacDialogResFile @ 0x8ddc0 (303 bytes)

void _OpenMacDialogResFile(void)

{
  short sVar1;
  uint local_390;
  undefined1 local_384 [80];
  char local_334 [800];
  undefined4 local_14;
  int local_10;
  
  local_10 = 0;
  local_14 = _GetGameDataPath();
  _printf("GameDataPath: %s\n");
  _sprintf(local_334,"%s/Midnight Mansion Dialogs.rsrc");
  _printf("Loading resources from path: %s\n");
  sVar1 = _PathToFSRef();
  local_10 = (int)sVar1;
  if (local_10 == 0) {
    for (local_390 = 0; local_390 < 0x50; local_390 = local_390 + 1) {
      (&stack0xfffffc04)[local_390] = local_384[local_390];
    }
    local_10 = _MyOpenResFileUsingFSRef();
  }
  if (local_10 != 0) {
    _ErrorMessage();
    _printf("Path used: %s\n");
    _ExitToShell();
  }
  return;
}


// ---- _MMDoOpenGameDialog @ 0x8deef (248 bytes)

int _MMDoOpenGameDialog(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined1 local_b3;
  undefined1 local_b2 [80];
  undefined1 local_62 [81];
  char local_11;
  int local_10;
  
  local_11 = _IsMusicPlaying();
  local_10 = 0;
  _PauseMusic();
  _PrepareGameForDialogBox(0x11);
  local_10 = _FSPathMakeRef(param_1,local_62,&local_b3);
  local_10 = _MyNavOpenDialog(local_b2,0x4d414e53,0x53415645,local_62,
                              PTR_MyNavEventCallback_001c51e4,0);
  if (local_10 == 0) {
    local_10 = _GetCFilenameFromFSRef(local_b2,param_3);
    _FSRefMakePath(local_b2,param_2,500);
  }
  else if (local_11 != '\0') {
    _ResumeMusic(0);
  }
  _RestoreGameAfterDialogBox(0);
  return local_10;
}


// ---- _MMDoSaveGameDialog @ 0x8dfe7 (984 bytes)

char _MMDoSaveGameDialog(void)

{
  char cVar1;
  undefined4 uVar2;
  uint uVar3;
  char *pcVar4;
  undefined1 local_7dc [80];
  undefined1 local_78c [80];
  char local_73c [256];
  undefined4 local_63c;
  char local_637 [254];
  undefined2 uStack_539;
  char local_218 [256];
  undefined2 local_118;
  undefined1 local_116 [254];
  undefined4 local_18;
  char local_12;
  undefined1 local_11;
  int local_10;
  
  local_118 = 0;
  _memset(local_116,0,0xfe);
  local_10 = 0;
  _PrepareGameForDialogBox(0x12);
  local_12 = '\0';
  local_11 = 0;
  _strcpy(local_637,PTR__gCurOpenMansionFileName_001c4894);
  _StripStringFromEndOfCString(".zip",local_637,0);
  _StripStringFromEndOfCString(".ZIP",local_637,0);
  if (*PTR__gLastSaveFilename_001c4730 == '\0') {
    _GetPascalStringFromCString(&local_118,local_637);
    _SWConcatPascalStrings((char *)&local_118,"\x01 ");
  }
  else {
    _GetPascalStringFromCString(&local_118,PTR__gLastSaveFilename_001c4730);
    _RemoveNumberFromEndOfString(&local_118);
  }
  if (*PTR__gCurrentlyInMansion_001c4908 == '\0') {
    _SWConcatPascalStrings((char *)&local_118,"\tCompleted");
  }
  else {
    *(int *)PTR__gCurrentSaveIndex_001c4998 = *(int *)PTR__gCurrentSaveIndex_001c4998 + 1;
    _SWNumToString(*(undefined4 *)PTR__gCurrentSaveIndex_001c4998,local_73c);
    _SWConcatPascalStrings((char *)&local_118,local_73c);
  }
  if (local_10 == 0) {
    _SWCopyPascalStringToCString((char *)&local_118,local_218);
    _SWCopyPascalStringToCString((char *)&local_118,PTR__gLastSaveFilename_001c4730);
  }
  local_18 = _CFStringCreateWithPascalString(*(undefined4 *)PTR_001c51d4,&local_118,0);
  uVar2 = _GetSavedGamesFilePath(0,0);
  _CreateDirectory(uVar2,local_637);
  local_10 = _GetFSRefForSaveGameFolder(local_78c,1);
  if (local_10 == 0) {
    local_10 = _MyNavSaveDialog(local_7dc,0x4d414e53,0x53415645,local_78c,
                                PTR_MyNavEventCallback_001c51e4,local_18,&local_63c);
  }
  else {
    local_10 = _MyNavSaveDialog(local_7dc,0x4d414e53,0x53415645,0,PTR_MyNavEventCallback_001c51e4,
                                local_18,&local_63c);
  }
  if (local_10 == 0) {
    _CFStringGetCString(local_63c,PTR__gLastSaveFilename_001c4730,400,0);
    _FSRefMakePath(local_7dc,(int)&uStack_539 + 1,800);
    uVar3 = 0xffffffff;
    pcVar4 = (char *)((int)&uStack_539 + 1);
    do {
      if (uVar3 == 0) break;
      uVar3 = uVar3 - 1;
      cVar1 = *pcVar4;
      pcVar4 = pcVar4 + 1;
    } while (cVar1 != '\0');
    *(undefined2 *)((int)&uStack_539 + ~uVar3) = 0x2f;
    _strcat((char *)((int)&uStack_539 + 1),PTR__gLastSaveFilename_001c4730);
    _printf("\nPath for saved game file: %s \n",(int)&uStack_539 + 1);
    local_10 = _MMCreateSaveGameFile((int)&uStack_539 + 1);
    if (local_10 == 0) {
      local_12 = '\x01';
    }
  }
  if (local_12 == '\0') {
    *(int *)PTR__gCurrentSaveIndex_001c4998 = *(int *)PTR__gCurrentSaveIndex_001c4998 + -1;
  }
  _RestoreGameAfterDialogBox(1);
  return local_12;
}


// ---- _MMDoOpenMansionDialog @ 0x8e3bf (507 bytes)

int _MMDoOpenMansionDialog(void)

{
  char local_4de [800];
  undefined1 local_1be [256];
  undefined1 local_be [80];
  undefined1 local_6e [81];
  undefined1 local_1d;
  undefined4 local_1c;
  undefined1 *local_18;
  char local_11;
  int local_10;
  
  local_18 = (undefined1 *)0x0;
  local_11 = _IsMusicPlaying();
  local_10 = 0;
  _PauseMusic();
  _PrepareGameForDialogBox(0x13);
  local_1c = _GetCustomMansionsFolderPath("");
  local_10 = _FSPathMakeRef(local_1c,local_6e,&local_1d);
  if (local_10 != 0) {
    local_1c = _GetGameDirectory();
    local_10 = _FSPathMakeRef(local_1c,local_6e,&local_1d);
  }
  if (local_10 == 0) {
    local_18 = local_6e;
  }
  else {
    local_18 = (undefined1 *)0x0;
  }
  local_10 = _MyNavOpenDialog(local_be,0x4d414e53,0x4c45564c,local_18,
                              PTR_MyNavEventCallback_001c51e4,2);
  _RestoreGameAfterDialogBox(0);
  if (local_10 == 0) {
    local_10 = _GetCFilenameFromFSRef(local_be,local_1be);
    _FSRefMakePath(local_be,local_4de,800);
    _StripStringFromEndOfCString(".zip",local_4de,0);
    _StripStringFromEndOfCString(".ZIP",local_4de,0);
    _strcat(local_4de,"/");
    _printf("\nPath for open mansion file: %s \n",local_4de);
    _strcpy(PTR__gCustomMansionFolderPath_001c4fcc,local_4de);
    local_10 = _MMOpenCustomMansionUsingPathAndName(local_4de,local_1be);
  }
  else if (local_11 != '\0') {
    _ResumeMusic(0);
  }
  return local_10;
}


// ---- _MMDoOpenMovieDialog @ 0x8e5ba (276 bytes)

int _MMDoOpenMovieDialog(void)

{
  undefined1 local_49d;
  undefined1 local_49c [80];
  undefined1 local_44c [80];
  undefined1 local_3fc [500];
  undefined1 local_208 [500];
  undefined4 local_14;
  int local_10;
  
  local_49d = 1;
  local_10 = 0;
  _PrepareGameForDialogBox(0x14);
  local_14 = _GetMoviesFilePath(0);
  local_10 = _FSPathMakeRef(local_14,local_44c,&local_49d);
  local_10 = _MyNavOpenDialog(local_49c,0x4d414e53,0x4d4f5632,local_44c,
                              PTR_MyNavEventCallback_001c51e4,0);
  _RestoreGameAfterDialogBox(1);
  if (local_10 == 0) {
    local_10 = _GetCFilenameFromFSRef(local_49c,local_3fc);
    _FSRefMakePath(local_49c,local_208,500);
    local_10 = _PlayMovieFile(local_208,local_3fc);
  }
  return local_10;
}


// ---- _MyNavEventCallback @ 0x8e6ce (83 bytes)

void MyNavEventCallback(short *param_1)

{
  undefined4 uVar1;
  
  if (*param_1 == 0) {
    _KeepMusicPlaying();
  }
  else if (*param_1 == 6) {
    uVar1 = *(undefined4 *)(param_1 + 1);
    _BeginUpdate(uVar1);
    _KeepMusicPlaying();
    _EndUpdate(uVar1);
  }
  return;
}


// ---- _DoChoosePlayerDialog @ 0x8e721 (1182 bytes)

undefined1 _DoChoosePlayerDialog(void)

{
  char cVar1;
  undefined1 uVar2;
  char local_23c [256];
  char local_13c;
  undefined1 local_13b [255];
  undefined1 local_3c [8];
  int *local_34;
  undefined1 local_30 [6];
  undefined4 local_2a;
  undefined2 local_26;
  undefined1 local_24 [2];
  short local_22;
  int local_20;
  undefined4 local_1c;
  char local_15;
  int local_14;
  char local_f;
  short local_e;
  
  local_14 = 0;
  _PrepareGameForDialogBox(0x16);
  local_20 = _GetNewDialog(0x104,0,0xffffffff);
  if (local_20 == 0) {
    _MMDoErrorMessage("Error: Can\'t find dialog resource 260 for Choose Player dialog for player profiles. (MMDialogsMac.c)"
                      ,0);
    uVar2 = 0xff;
  }
  else {
    _SetPortDialogPort(local_20);
    _SetDialogDefaultItem(local_20,1);
    _SetDialogCancelItem(local_20,2);
    _GetDialogItem(local_20,5,local_24,local_30,local_3c);
    local_14 = _CreatePlayerListForDialog(local_20,local_3c,&local_34);
    if (local_14 == 0) {
      local_1c = _NewModalFilterUPP(PTR_ChoosePlayerDialogFilter_001c51fc);
      do {
        _ModalDialog(local_1c,&local_22);
        if (local_22 == 3) {
          _DrawFrameAndFocusForList(local_34,0);
          local_f = _DoEnterNameForNewProfileDialog(local_23c);
          _SetPortDialogPort(local_20);
          _DrawList(local_34);
          _DrawFrameAndFocusForList(local_34,1);
          if (local_f != '\0') {
            _LDispose(local_34);
            _SaveCurrentProfile();
            _CreateNewProfile(local_23c);
            local_14 = _CreatePlayerListForDialog(local_20,local_3c,&local_34);
            if (local_14 != 0) {
              _SysBeep(1);
              _DisposeDialog(local_20);
              return 2;
            }
            _RestoreCurrentProfile();
          }
        }
        if (local_22 == 4) {
          local_26 = 0xff;
          _SetPt(&local_2a,0,0);
          cVar1 = _LGetSelect(1,&local_2a,local_34);
          if (cVar1 == '\0') {
            _SysBeep(1);
            local_15 = '\0';
          }
          else {
            _LGetCell(local_13b,&local_26,local_2a,local_34);
            local_13c = (char)local_26;
            local_15 = _DoAreYouSureDeleteDialog(&local_13c);
          }
          _DrawList(local_34);
          if (local_15 == '\x01') {
            _SWCopyPascalStringToCString(&local_13c,local_23c);
            _DeleteProfile(local_23c);
            _SetPt(&local_2a,0,0);
            _LGetSelect(1,&local_2a,local_34);
            _LDelRow(1,(int)(short)local_2a,local_34);
            local_e = (*(short *)(*local_34 + 0x4c) - *(short *)(*local_34 + 0x48)) + -1;
            if (local_e < (short)local_2a) {
              local_2a = CONCAT22(local_2a._2_2_,local_e);
            }
            _LSetSelect(1,local_2a,local_34);
          }
        }
      } while ((local_22 != 2) && (local_22 != 1));
      local_26 = 0xff;
      _SetPt(&local_2a,0,0);
      cVar1 = _LGetSelect(1,&local_2a,local_34);
      if (cVar1 == '\0') {
        local_22 = 2;
      }
      else {
        _LGetCell(local_13b,&local_26,local_2a,local_34);
        local_13c = (char)local_26;
      }
      _LDispose(local_34);
      _DisposeModalFilterUPP(local_1c);
      _DisposeDialog(local_20);
      if (local_22 == 1) {
        local_14 = _LoadProfileFromDisk(&local_13c);
        if (local_14 != 0) {
          _MacStopAlert(0x9f);
        }
        _WritePreferencesFile();
        *(undefined2 *)PTR__gLastOpenDialog_001c49c0 = 0x10;
      }
      _RestoreGameAfterDialogBox(1);
      _MMSetCursorForGame();
      uVar2 = local_22 == 1;
    }
    else {
      _SysBeep(1);
      _DisposeDialog(local_20);
      uVar2 = 2;
    }
  }
  return uVar2;
}


// ---- _CreatePlayerListForDialog @ 0x8ebbf (905 bytes)

undefined4 _CreatePlayerListForDialog(undefined4 param_1,undefined4 *param_2,undefined4 *param_3)

{
  char cVar1;
  short sVar2;
  undefined4 uVar3;
  int *piVar4;
  undefined4 local_430;
  undefined1 local_420 [512];
  undefined1 local_220 [72];
  undefined1 auStack_1d8 [72];
  undefined1 local_190 [80];
  int local_140;
  undefined4 local_13c;
  undefined1 local_136 [8];
  undefined4 local_12e;
  undefined2 local_12a;
  short sStack_128;
  undefined1 local_126;
  char local_125 [255];
  undefined4 local_26;
  undefined4 local_22;
  undefined4 local_1e;
  short local_1a;
  int *local_18;
  undefined1 *local_14;
  int local_10;
  
  local_12e = *param_2;
  sStack_128 = (short)((uint)param_2[1] >> 0x10);
  _local_12a = CONCAT22(sStack_128 + -0x10,(short)param_2[1]);
  _SetRect(local_136,0,0,1,0);
  _SetPt(&local_1e,0,0);
  uVar3 = _GetDialogWindow(param_1);
  piVar4 = (int *)_LNew(&local_12e,local_136,local_1e,0,uVar3,1,0,0,1);
  if (piVar4 == (int *)0x0) {
    local_430 = 0xffffffff;
  }
  else {
    local_18 = piVar4;
    uVar3 = _GetDialogWindow(param_1);
    _SetWRefCon(uVar3,piVar4);
    *(undefined1 *)(*local_18 + 0x24) = 0x80;
    local_1a = 0;
    _SetPt(&local_26,0,0);
    _LSetDrawingMode(0,local_18);
    local_10 = 0;
    local_10 = _GetFSRefForPlayerProfilesFolder(local_190);
    if (local_10 == 0) {
      sVar2 = _FSOpenIterator(local_190,0,&local_13c);
      local_10 = (int)sVar2;
    }
    while (local_10 == 0) {
      sVar2 = _FSGetCatalogInfoBulk(local_13c,1,&local_140,0,0x800,local_220,0,0,local_420);
      local_10 = (int)sVar2;
      if ((local_140 != 0) && (local_10 == 0)) {
        local_14 = auStack_1d8;
        local_10 = _GetPascalStringFromHFSUni255(&local_126,local_420);
        if ((local_10 == 0) && (local_125[0] != '.')) {
          _LAddRow(1,(int)local_1a,local_18);
          _SetPt(&local_22,0,(int)local_1a);
          _LSetCell(local_125,local_126,local_22,local_18);
          if ((*PTR__gCurrentPlayerNameExists_001c4830 != '\0') &&
             (cVar1 = _EqualString(&local_126,PTR__gCurrentPlayerName_001c48e8,1,1), cVar1 != '\0'))
          {
            local_26 = local_22;
          }
          local_1a = local_1a + 1;
        }
      }
    }
    _FSCloseIterator(local_13c);
    _LSetDrawingMode(1,local_18);
    _DrawFrameAndFocusForList(local_18,1);
    _LSetSelect(1,local_26,local_18);
    _LAutoScroll(local_18);
    _DrawList(local_18);
    *param_3 = local_18;
    local_430 = 0;
  }
  return local_430;
}


// ---- _DoAreYouSureDeleteDialog @ 0x8ef48 (222 bytes)

undefined1 _DoAreYouSureDeleteDialog(undefined4 param_1)

{
  undefined1 uVar1;
  short local_12;
  int local_10;
  
  local_10 = _GetNewDialog(0xe1,0,0xffffffff);
  if (local_10 == 0) {
    _MMDoErrorMessage("Error: Can\'t find dialog resource 225 for Are You Sure you want to delete a player dialog. (MMDialogsMac.c)"
                      ,0);
    uVar1 = 0xff;
  }
  else {
    _ParamText(param_1,&DAT_00121188,&DAT_00121188,&DAT_00121188);
    _SetDialogDefaultItem(local_10,1);
    _SetDialogCancelItem(local_10,2);
    _ModalDialog(0,&local_12);
    _DisposeDialog(local_10);
    uVar1 = local_12 == 1;
  }
  return uVar1;
}


// ---- _ChoosePlayerDialogFilter @ 0x8f026 (316 bytes)

char ChoosePlayerDialogFilter(undefined4 param_1,short *param_2,undefined4 param_3)

{
  undefined1 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined1 auStack_26 [8];
  undefined1 auStack_1e [2];
  undefined1 auStack_1c [4];
  undefined4 uStack_18;
  char cStack_11;
  int iStack_10;
  
  _GetDialogItem(param_1,5,auStack_1e,auStack_1c,auStack_26);
  uVar2 = _GetDialogWindow(param_1);
  uStack_18 = _GetWRefCon(uVar2);
  cStack_11 = _HandleEventInOpenDialog(param_1,param_2,param_3);
  if ((cStack_11 == '\0') && (*param_2 == 6)) {
    iStack_10 = *(int *)(param_2 + 1);
    _BeginUpdate(iStack_10);
    iVar3 = _GetDialogWindow(param_1);
    if (iVar3 == iStack_10) {
      _SetPortDialogPort(param_1);
      _DrawDialog(param_1);
      uVar2 = _GetDialogWindow(param_1);
      uVar1 = _IsWindowHilited(uVar2);
      _DrawFrameAndFocusForList(uStack_18,uVar1);
    }
    _EndUpdate(iStack_10);
    iVar3 = _GetDialogWindow(param_1);
    if (iVar3 == iStack_10) {
      _DrawList(uStack_18);
    }
    cStack_11 = '\x01';
  }
  if (cStack_11 == '\0') {
    _SetPortDialogPort(param_1);
    cStack_11 = _StdFilterProc(param_1,param_2,param_3);
  }
  return cStack_11;
}


// ---- _HandleEventInOpenDialog @ 0x8f162 (686 bytes)

undefined1 _HandleEventInOpenDialog(undefined4 param_1,short *param_2,undefined2 *param_3)

{
  short sVar1;
  char cVar2;
  undefined4 uVar3;
  int iVar4;
  undefined1 local_42 [8];
  undefined1 local_3a [8];
  undefined4 local_32;
  undefined1 local_2e [4];
  undefined1 local_2a [2];
  undefined4 local_28;
  undefined4 local_24;
  undefined1 local_20 [4];
  int local_1c;
  undefined4 local_18;
  short local_14;
  char local_12;
  undefined1 local_11;
  ushort local_10;
  ushort local_e;
  
  local_11 = 0;
  _GetDialogItem(param_1,5,local_2a,local_20,local_42);
  _GetDialogItem(param_1,1,local_2a,&local_24,local_3a);
  _GetDialogItem(param_1,4,local_2a,&local_28,local_3a);
  uVar3 = _GetDialogWindow(param_1);
  local_18 = _GetWRefCon(uVar3);
  sVar1 = *param_2;
  if ((sVar1 == 3) || (sVar1 == 5)) {
    local_10 = (ushort)*(undefined4 *)(param_2 + 1) & 0xff;
    local_e = (ushort)((uint)*(undefined4 *)(param_2 + 1) >> 8) & 0xff;
    if ((local_10 == 0x1e) || (local_10 == 0x1f)) {
      _HandleArrowKeyInList(local_18,local_10);
      local_11 = 1;
    }
  }
  else if (sVar1 == 1) {
    local_14 = _FindWindow(*(undefined4 *)(param_2 + 5),&local_1c);
    iVar4 = _GetDialogWindow(param_1);
    if (iVar4 == local_1c) {
      if (local_14 == 3) {
        local_32 = *(undefined4 *)(param_2 + 5);
        _SetPortDialogPort(param_1);
        _GlobalToLocal(&local_32);
        cVar2 = _PtInRect(local_32,local_42);
        if (cVar2 != '\0') {
          local_11 = 1;
          local_12 = _LClick(local_32,param_2[7],local_18);
          _SetPt(local_2e,0,0);
          if ((local_12 != '\0') && (cVar2 = _LGetSelect(1,local_2e,local_18), cVar2 == '\x01')) {
            *param_3 = 1;
          }
        }
      }
      else if (local_14 == 4) {
        _DragWindow(local_1c,*(undefined4 *)(param_2 + 5),0);
        local_11 = 1;
      }
    }
  }
  _SetPt(local_2e,0,0);
  cVar2 = _LGetSelect(1,local_2e,local_18);
  if (cVar2 == '\0') {
    _HiliteControl(local_24,0xff);
    _HiliteControl(local_28,0xff);
  }
  else {
    _HiliteControl(local_24,0);
    _HiliteControl(local_28,0);
  }
  return local_11;
}


// ---- _DoEnterNameForNewProfileDialog @ 0x8f410 (1044 bytes)

undefined1 _DoEnterNameForNewProfileDialog(undefined4 param_1)

{
  char cVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined1 uVar4;
  byte local_224;
  char local_223;
  undefined4 local_124;
  char local_11e [256];
  short local_1e;
  int local_1c;
  undefined4 local_18;
  int local_14;
  char local_d;
  
  local_d = *PTR__gDialogBoxIsOpen_001c49c8;
  if (local_d == '\0') {
    _PrepareGameForDialogBox(0x18);
  }
  local_14 = _GetNewDialog(0xd7,0,0xffffffff);
  if (local_14 == 0) {
    _MMDoErrorMessage("Error: Can\'t find dialog resource 215 for Enter name for New Profile dialog. (MMDialogsMac.c)"
                      ,0);
    uVar4 = 0xff;
  }
  else {
    _SetDialogCancelItem(local_14,2);
    _SetDialogTracksCursor(local_14,1);
    _GetDialogItemAsControl(local_14,1,&local_1c);
    _DeactivateControl(local_1c);
    uVar2 = _GetDialogWindow(local_14);
    _CreateRootControl(uVar2,&local_124);
    uVar2 = _GetDialogWindow(local_14);
    _GetRootControl(uVar2,&local_124);
    _GetDialogItemAsControl(local_14,3,&local_18);
    _EmbedControl(local_18,local_124);
    _GetDialogItemAsControl(local_14,3,&local_18);
    uVar2 = local_18;
    uVar3 = _GetDialogWindow(local_14);
    _SetKeyboardFocus(uVar3,uVar2,5);
    uVar2 = _GetDialogWindow(local_14);
    _ShowWindow(uVar2);
    _SetPortDialogPort(local_14);
    _GetDialogItemAsControl(local_14,3,&local_18);
    uVar2 = _GetDialogWindow(local_14);
    _SetKeyboardFocus(uVar2,local_18,5);
    do {
      _ModalDialog(0,&local_1e);
      if (local_1e == 3) {
        _MyGetDialogItemText(local_14,3,&local_224);
        if ((local_224 == 1) && ((local_223 == '\r' || (local_223 == '\x03')))) {
          local_224 = 0;
          _MySetDialogItemText(local_14,3,&local_224);
          _RedrawDialogItem(local_14,3);
          _SysBeep(1);
        }
        if (0x1f < local_224) {
          local_224 = 0x1f;
          _MySetDialogItemText(local_14,3,&local_224);
          _RedrawDialogItem(local_14,3);
          _SysBeep(1);
        }
        if (local_224 == 0) {
          if (local_1c != 0) {
            _DeactivateControl(local_1c);
          }
        }
        else if (local_1c != 0) {
          _ActivateControl(local_1c);
          _SetDialogDefaultItem(local_14,1);
        }
      }
      if (local_1e == 1) {
        _MyGetDialogItemText(local_14,3,local_11e);
        _SWCopyPascalStringToCString(local_11e,(char *)&local_224);
        cVar1 = _DoesProfileAlreadyExist(&local_224);
        if (cVar1 != '\0') {
          _SysBeep(1);
          _MacStopAlert(0x98);
          local_1e = -1;
        }
      }
    } while ((local_1e != 1) && (local_1e != 2));
    if (local_1e == 1) {
      _MyGetDialogItemText(local_14,3,param_1);
    }
    _DisposeDialog(local_14);
    if (local_d == '\0') {
      if (local_1e == 1) {
        _RestoreGameAfterDialogBox(0);
      }
      else {
        _RestoreGameAfterDialogBox(1);
      }
    }
    uVar4 = local_1e == 1;
  }
  return uVar4;
}


// ---- _HandleArrowKeyInList @ 0x8f824 (280 bytes)

void _HandleArrowKeyInList(int *param_1,short param_2)

{
  undefined4 uVar1;
  undefined4 local_18;
  undefined4 local_14;
  short local_10;
  char local_d;
  
  local_10 = (*(short *)(*param_1 + 0x4c) - *(short *)(*param_1 + 0x48)) + -1;
  _SetPt(&local_14,0,0);
  local_d = _LGetSelect(1,&local_14,param_1);
  local_18 = local_14;
  uVar1 = local_18;
  local_18._2_2_ = (undefined2)((uint)local_14 >> 0x10);
  if (local_d == '\0') {
    if (param_2 == 0x1e) {
      local_18 = CONCAT22(local_18._2_2_,local_10);
      uVar1 = local_18;
    }
    else if (param_2 == 0x1f) {
      local_14 = CONCAT22(local_18._2_2_,1);
    }
  }
  else if ((param_2 == 0x1e) && (0 < (short)local_14)) {
    local_18 = CONCAT22(local_18._2_2_,(short)local_14 + -1);
    uVar1 = local_18;
  }
  else if ((param_2 == 0x1f) && ((short)local_14 < local_10)) {
    local_18 = CONCAT22(local_18._2_2_,(short)local_14 + 1);
    uVar1 = local_18;
  }
  local_18 = uVar1;
  if ((short)local_18 != (short)local_14) {
    _LSetSelect(0,local_14,param_1);
    _LSetSelect(1,local_18,param_1);
    _LAutoScroll(param_1);
  }
  return;
}


// ---- _MMDoEnterNameDialog @ 0x8f93c (831 bytes)

uint _MMDoEnterNameDialog(undefined4 param_1,undefined4 param_2,char param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  uint local_134;
  byte local_124 [256];
  undefined4 local_24;
  short local_1e;
  undefined4 local_1c;
  int local_18;
  byte local_11;
  undefined4 local_10;
  
  _PrepareGameForDialogBox(0x18);
  local_18 = _GetNewDialog(0xd2,0,0xffffffff);
  if (local_18 == 0) {
    _MMDoErrorMessage("Error: Can\'t find dialog resource 210 for Enter Name dialog for high score. (MMDialogsMac.c)"
                      ,0);
    local_134 = 0xffffffff;
  }
  else {
    _SetDialogDefaultItem(local_18,1);
    _SetDialogCancelItem(local_18,2);
    _SetDialogTracksCursor(local_18,1);
    _MySetDialogItemText(local_18,4,param_1);
    if (param_3 == '\0') {
      _EnableMyControl(local_18,3,0);
    }
    else {
      _SetCheckBoxValue(local_18,3,1);
    }
    uVar1 = _GetDialogWindow(local_18);
    _CreateRootControl(uVar1,&local_24);
    uVar1 = _GetDialogWindow(local_18);
    _GetRootControl(uVar1,&local_24);
    _GetDialogItemAsControl(local_18,4,&local_1c);
    _EmbedControl(local_1c,local_24);
    _GetDialogItemAsControl(local_18,4,&local_1c);
    uVar1 = local_1c;
    uVar2 = _GetDialogWindow(local_18);
    _SetKeyboardFocus(uVar2,uVar1,5);
    uVar1 = _GetDialogWindow(local_18);
    _ShowWindow(uVar1);
    _GetDialogItemAsControl(local_18,4,&local_1c);
    uVar1 = _GetDialogWindow(local_18);
    _SetKeyboardFocus(uVar1,local_1c,5);
    local_10 = _NewModalFilterUPP(PTR_LAB_001c51f0);
    do {
      _ModalDialog(local_10,&local_1e);
      if (local_1e == 3) {
        _ProcessCheckBoxClick(local_18,3);
      }
      if (local_1e == 4) {
        _MyGetDialogItemText(local_18,4,local_124);
        if (0x11 < local_124[0]) {
          local_124[0] = 0x11;
          _MySetDialogItemText(local_18,4,local_124);
          _RedrawDialogItem(local_18,4);
          _SysBeep(1);
        }
      }
    } while ((local_1e != 1) && (local_1e != 2));
    _MyGetDialogItemText(local_18,4,param_2);
    local_11 = _GetCheckBoxValue(local_18,3);
    _DisposeDialog(local_18);
    _RestoreGameAfterDialogBox(0);
    if (local_1e == 2) {
      local_134 = 0xffffffff;
    }
    else {
      local_134 = (uint)local_11;
    }
  }
  return local_134;
}


// ---- _EnterNameDialogFilter @ 0x8fc7b (140 bytes)

char EnterNameDialogFilter(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  char acStack_10d [256];
  char cStack_d;
  
  cStack_d = '\0';
  _MyGetDialogItemText(param_1,4,acStack_10d);
  _EnableMyControl(param_1,1,acStack_10d[0] != '\0');
  if (cStack_d == '\0') {
    _SetPortDialogPort(param_1);
    cStack_d = _StdFilterProc(param_1,param_2,param_3);
  }
  return cStack_d;
}


// ---- _DoRegisterTitleScreenDialog @ 0x8fd07 (629 bytes)

void _DoRegisterTitleScreenDialog(void)

{
  undefined4 uVar1;
  char *local_334;
  char *local_330;
  undefined1 local_320 [255];
  char local_221 [255];
  char local_122 [256];
  short local_22;
  int local_20;
  undefined4 local_1c;
  char local_15;
  char *local_14;
  char *local_10;
  
  local_15 = '\0';
  if (*PTR__gUsingEditor_001c48b0 == '\0') {
    local_20 = _GetNewDialog(0xcd,0,0xffffffff);
    if (local_20 == 0) {
      _MMCantFindMacResource
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MMDialogsMac.c",
                 PTR____FUNCTION___90497_001c51f8,0x58a);
    }
    _PrepareGameForDialogBox(0x17);
    _SetPortDialogPort(local_20);
    _SetDialogDefaultItem(local_20,1);
    _SetDialogCancelItem(local_20,2);
    if (*PTR__gEpisode1IsUnlocked_001c4744 == '\0') {
      local_334 = "Locked";
    }
    else {
      local_334 = "Unlocked - Ready to play!";
    }
    local_14 = local_334;
    if (*PTR__gEpisode2IsUnlocked_001c48d0 == '\0') {
      local_330 = "Locked";
    }
    else {
      local_330 = "Unlocked - Ready to play!";
    }
    local_10 = local_330;
    _sprintf(local_221,"Current state of the game: \rEpisode 1: %s\rEpisode 2: %s",local_334,
             local_330);
    _SWCopyCStringToPascalString(local_221,local_122);
    _MySetDialogItemText(local_20,5,local_122);
    uVar1 = _GetDialogWindow(local_20);
    _ShowWindow(uVar1);
    local_1c = _NewModalFilterUPP(PTR_MyStdDialogFilter_001c51e0);
    do {
      _ModalDialog(local_1c,&local_22);
      if (local_22 == 3) {
        local_15 = _DoEnterRegistrationCodeDialog(0x109,local_320);
      }
    } while (((local_22 != 2) && (local_22 != 1)) && (local_15 == '\0'));
    _DisposeModalFilterUPP(local_1c);
    _DisposeDialog(local_20);
    _RestoreGameAfterDialogBox(1);
    if (local_15 == '\0') {
      if (local_22 == 1) {
        _LaunchRegistrationWebsite();
        _ShutDown();
      }
    }
    else {
      _MMDoCustomDialog(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,local_320,3);
      _ChangeHiddenPiracyValue(0);
      if (*PTR__gCanPlayBothEpisodes_001c4738 != '\0') {
        _ExitTitleScreen();
        _SetUpTitleScreen();
      }
    }
  }
  return;
}


// ---- _CreateLicenseFileForNameAndCode @ 0x8ff7c (608 bytes)

void _CreateLicenseFileForNameAndCode(undefined4 param_1,char *param_2)

{
  int iVar1;
  char *pcVar2;
  char *pcVar3;
  char *pcVar4;
  char *pcVar5;
  undefined1 uVar6;
  bool bVar7;
  char local_226 [255];
  char local_127 [255];
  char *local_28;
  int local_24;
  char local_1d;
  
  uVar6 = &stack0x00000000 == (undefined1 *)0x36c;
  _SWCopyPascalStringToCString(param_2,local_127);
  _GetPrefixFromLicenseCode();
  iVar1 = 4;
  pcVar2 = local_226;
  pcVar4 = "EP1";
  do {
    pcVar3 = pcVar2;
    pcVar5 = pcVar4;
    if (iVar1 == 0) break;
    iVar1 = iVar1 + -1;
    pcVar5 = pcVar4 + 1;
    pcVar3 = pcVar2 + 1;
    uVar6 = *pcVar2 == *pcVar4;
    pcVar2 = pcVar3;
    pcVar4 = pcVar5;
  } while ((bool)uVar6);
  iVar1 = 0;
  if (!(bool)uVar6) {
    iVar1 = (uint)(byte)pcVar3[-1] - (uint)(byte)pcVar5[-1];
  }
  bVar7 = iVar1 == 0;
  if (bVar7) {
    local_28 = "Midnight Mansion HD Episode 1 License";
  }
  else {
    iVar1 = 4;
    pcVar2 = local_226;
    pcVar4 = "EP2";
    do {
      pcVar3 = pcVar2;
      pcVar5 = pcVar4;
      if (iVar1 == 0) break;
      iVar1 = iVar1 + -1;
      pcVar5 = pcVar4 + 1;
      pcVar3 = pcVar2 + 1;
      bVar7 = *pcVar2 == *pcVar4;
      pcVar2 = pcVar3;
      pcVar4 = pcVar5;
    } while (bVar7);
    iVar1 = 0;
    if (!bVar7) {
      iVar1 = (uint)(byte)pcVar3[-1] - (uint)(byte)pcVar5[-1];
    }
    bVar7 = iVar1 == 0;
    if (bVar7) {
      local_28 = "Midnight Mansion HD Episode 2 License";
    }
    else {
      iVar1 = 4;
      pcVar2 = local_226;
      pcVar4 = "MM1";
      do {
        pcVar3 = pcVar2;
        pcVar5 = pcVar4;
        if (iVar1 == 0) break;
        iVar1 = iVar1 + -1;
        pcVar5 = pcVar4 + 1;
        pcVar3 = pcVar2 + 1;
        bVar7 = *pcVar2 == *pcVar4;
        pcVar2 = pcVar3;
        pcVar4 = pcVar5;
      } while (bVar7);
      iVar1 = 0;
      if (!bVar7) {
        iVar1 = (uint)(byte)pcVar3[-1] - (uint)(byte)pcVar5[-1];
      }
      bVar7 = iVar1 == 0;
      if (bVar7) {
        local_28 = "Midnight Mansion HD License";
      }
      else {
        iVar1 = 4;
        pcVar2 = local_226;
        pcVar4 = "MM2";
        do {
          pcVar3 = pcVar2;
          pcVar5 = pcVar4;
          if (iVar1 == 0) break;
          iVar1 = iVar1 + -1;
          pcVar5 = pcVar4 + 1;
          pcVar3 = pcVar2 + 1;
          bVar7 = *pcVar2 == *pcVar4;
          pcVar2 = pcVar3;
          pcVar4 = pcVar5;
        } while (bVar7);
        iVar1 = 0;
        if (!bVar7) {
          iVar1 = (uint)(byte)pcVar3[-1] - (uint)(byte)pcVar5[-1];
        }
        if (iVar1 == 0) {
          local_28 = "Midnight Mansion 2 License";
        }
      }
    }
  }
  local_24 = _MMCreateLicenseFile();
  _SetMMRegistered();
  if (local_24 == 0) {
    local_1d = _CheckForLicenseFilename();
    if (local_1d == '\0') {
      local_24 = -2;
    }
    _CheckForLicenseAndDataFiles();
  }
  if (local_24 != 0) {
    _SWNumToString();
    _ParamText();
    _MacStopAlert();
    _ShutDown();
  }
  return;
}


// ---- _DoEditorUnregisteredDialog @ 0x901dc (341 bytes)

void _DoEditorUnregisteredDialog(void)

{
  undefined4 uVar1;
  undefined1 local_24 [4];
  undefined1 local_20 [8];
  undefined1 local_18 [2];
  short local_16;
  int local_14;
  undefined4 local_10;
  
  local_14 = _GetNewDialog(0x113,0,0xffffffff);
  if (local_14 == 0) {
    _MMCantFindMacResource
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MMDialogsMac.c",
               PTR____FUNCTION___90580_001c51f4,0x615);
  }
  _PrepareGameForDialogBox(0x17);
  _SetPortDialogPort(local_14);
  _SetDialogDefaultItem(local_14,1);
  _SetDialogCancelItem(local_14,2);
  uVar1 = _GetDialogWindow(local_14);
  _ShowWindow(uVar1);
  _GetDialogItem(local_14,4,local_18,local_24,local_20);
  local_10 = _NewModalFilterUPP(PTR_MyStdDialogFilter_001c51e0);
  do {
    _ModalDialog(local_10,&local_16);
    if (local_16 == 2) break;
  } while (local_16 != 1);
  _DisposeModalFilterUPP(local_10);
  _DisposeDialog(local_14);
  if (local_16 == 1) {
    _RestoreGameAfterDialogBox(0);
    _LaunchRegistrationWebsite();
    _ShutDown();
  }
  else {
    _RestoreGameAfterDialogBox(1);
  }
  return;
}


// ---- _DoEnterRegistrationCodeDialog @ 0x90331 (2630 bytes)

char _DoEnterRegistrationCodeDialog(short param_1,void *param_2)

{
  char cVar1;
  short sVar2;
  undefined4 uVar3;
  int iVar4;
  char *pcVar5;
  char *pcVar6;
  char *pcVar7;
  char *pcVar8;
  undefined1 uVar9;
  bool bVar10;
  undefined4 local_784;
  byte local_777;
  undefined1 auStack_776 [255];
  undefined1 local_677 [256];
  char local_577;
  short local_576;
  undefined1 local_574 [2];
  undefined1 local_572 [2];
  undefined1 local_570 [2];
  short local_56e;
  short local_56c;
  short local_56a;
  short local_568;
  short local_566;
  short local_564;
  byte local_561;
  undefined1 auStack_560 [255];
  byte local_461;
  undefined1 auStack_460 [255];
  char local_361 [20];
  char local_34d [255];
  undefined1 local_24e [256];
  char local_14e [256];
  short local_4e;
  int local_4c;
  short local_46;
  int local_44;
  int local_40;
  undefined4 local_3c;
  int local_38;
  char local_34;
  char local_33;
  char local_31;
  int local_30;
  int local_2c;
  int local_28;
  char local_21;
  int local_20;
  
  local_38 = 0;
  local_33 = '\0';
  local_31 = '\x01';
  local_4c = _GetNewDialog((int)param_1,0,0xffffffff);
  if (local_4c == 0) {
    _MMCantFindMacResource
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MMDialogsMac.c",
               PTR____FUNCTION___90616_001c51ec,0x661);
  }
  _MySetDialogItemText(local_4c,4,&DAT_00121188);
  local_40 = _GetRegNameFromClipboard("Registered To:");
  if (local_40 != 0) {
    _GetPascalStringFromCString(local_24e,local_40);
    _RemoveTrailingSpaces(local_24e);
    _MySetDialogItemText(local_4c,4,local_24e);
    local_31 = '\0';
  }
  local_44 = _GetRegCodeFromClipboard("Registration code:");
  if (local_44 != 0) {
    _GetPascalStringFromCString(local_24e,local_44);
    _MySetDialogItemText(local_4c,6,local_24e);
  }
  if (local_31 != '\0') {
    _SelectDialogItemText(local_4c,4,0,0x7fff);
  }
  _SetDialogDefaultItem(local_4c,1);
  _SetDialogCancelItem(local_4c,2);
  uVar3 = _GetDialogWindow(local_4c);
  _ShowWindow(uVar3);
  _MMInitCursor();
  local_3c = _NewModalFilterUPP(PTR_MyStdDialogFilter_001c51e0);
  do {
    local_34 = '\0';
    do {
      _ModalDialog(local_3c,&local_4e);
      if ((local_4e == 2) || (local_4e == 1)) break;
    } while (local_4e != 8);
    if (local_4e == 1) {
      _MyGetDialogItemText(local_4c,4,&local_461);
      _MyGetDialogItemText(local_4c,6,&local_561);
      _RemoveLeadingInvisibleString(&local_461);
      _RemoveLeadingInvisibleString(&local_561);
      _RemoveLeadingSpaces(&local_461);
      _RemoveLeadingSpaces(&local_561);
      _RemoveTrailingSpaces(&local_461);
      _RemoveTrailingSpaces(&local_561);
      auStack_460[local_461] = 0;
      auStack_560[local_561] = 0;
      _SWCopyPascalString((char *)&local_561,local_14e);
      _SWCopyPascalStringToCString((char *)&local_561,local_34d);
      _stringToUppercase(local_34d);
      _GetPrefixFromLicenseCode(local_34d,local_361);
      iVar4 = -1;
      pcVar5 = local_361;
      do {
        if (iVar4 == 0) break;
        iVar4 = iVar4 + -1;
        cVar1 = *pcVar5;
        pcVar5 = pcVar5 + 1;
      } while (cVar1 != '\0');
      local_46 = ~(ushort)iVar4 - 1;
      uVar9 = local_46 == 0;
      if ((bool)uVar9) {
        _MacStopAlert(0xa1);
        local_34 = '\x01';
      }
      iVar4 = 4;
      pcVar5 = local_361;
      pcVar7 = "MM2";
      do {
        pcVar6 = pcVar5;
        pcVar8 = pcVar7;
        if (iVar4 == 0) break;
        iVar4 = iVar4 + -1;
        pcVar8 = pcVar7 + 1;
        pcVar6 = pcVar5 + 1;
        uVar9 = *pcVar5 == *pcVar7;
        pcVar5 = pcVar6;
        pcVar7 = pcVar8;
      } while ((bool)uVar9);
      iVar4 = 0;
      if (!(bool)uVar9) {
        iVar4 = (uint)(byte)pcVar6[-1] - (uint)(byte)pcVar8[-1];
      }
      if (iVar4 == 0) {
        _MacStopAlert(0xa2);
        local_34 = '\x01';
      }
      if ((local_34 == '\0') && (local_561 != 0x15)) {
        _SWNumToString(local_561,local_677);
        _SWNumToString(0x15,&local_777);
        _ParamText(local_677,&local_777,&DAT_00121188,&DAT_00121188);
        local_34 = '\x01';
        _MacStopAlert(0x9e);
      }
      else {
        if (local_34 == '\0') {
          cVar1 = _ValidateRegistrationCodeWithPrefix
                            (auStack_460,auStack_560,&local_564,&local_566,&local_568);
          if (cVar1 != '\0') {
            cVar1 = _IsNamePirated(&local_461);
            if ((cVar1 == '\0') || (0x7d7 < local_568)) {
              bVar10 = false;
            }
            else {
              bVar10 = true;
            }
            if (!bVar10) {
              _ParsePixelsRLE16Bit(&local_56a,&local_56c,&local_56e);
              iVar4 = (int)local_568;
              sVar2 = _GetNumDaysBeforeMonth((int)local_566);
              local_30 = iVar4 * 0x16d + (int)sVar2 + (int)local_564;
              iVar4 = (int)local_56e;
              sVar2 = _GetNumDaysBeforeMonth((int)local_56c);
              local_2c = iVar4 * 0x16d + (int)sVar2 + (int)local_56a;
              local_30 = local_30 + -1;
              if (local_2c - local_30 < 0x22) {
                _GetDateFromHiddenFile(local_570,local_572,local_574,&local_576);
                cVar1 = _IsComputerClockValid();
                if (((cVar1 == '\0') || (local_2c < local_30)) || (local_576 != 0)) {
                  uVar3 = _GetDialogWindow(local_4c);
                  _HideWindow(uVar3);
                  cVar1 = _IsComputerClockValid();
                  if ((cVar1 == '\0') || (local_2c < local_30)) {
                    local_784 = 0;
                  }
                  else {
                    local_784 = 1;
                  }
                  cVar1 = _DoValidateCodeDialog(local_784);
                  if (cVar1 != '\0') {
                    local_28 = _GetNewDialog(0x11d,0,0xffffffff);
                    if (local_28 == 0) {
                      _MMCantFindMacResource
                                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MMDialogsMac.c"
                                 ,PTR____FUNCTION___90616_001c51ec,0x702);
                    }
                    uVar3 = _GetDialogWindow(local_28);
                    _ShowWindow(uVar3);
                    _DrawDialog(local_28);
                    local_38 = _CheckIfRegCodeIsValid(&local_461,local_14e,&local_577);
                    _DisposeDialog(local_28);
                    if (local_38 == 0) {
                      local_33 = local_577;
                      if (local_577 == '\0') {
                        _MacStopAlert(0x9d);
                      }
                    }
                    else {
                      _MacStopAlert(0x9c);
                    }
                  }
                }
                else {
                  local_33 = '\x01';
                }
              }
              else {
                uVar3 = _GetDialogWindow(local_4c);
                _HideWindow(uVar3);
                cVar1 = _DoGetUpdatedCodeDialog();
                if (cVar1 != '\0') {
                  local_20 = _GetNewDialog(0x11d,0,0xffffffff);
                  if (local_20 == 0) {
                    _MMCantFindMacResource
                              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MMDialogsMac.c"
                               ,PTR____FUNCTION___90616_001c51ec,0x72c);
                  }
                  uVar3 = _GetDialogWindow(local_20);
                  _ShowWindow(uVar3);
                  _DrawDialog(local_20);
                  _GiveOSTime();
                  local_21 = _GetUpdatedRegistrationCode(&local_461,local_14e,&local_777);
                  _DisposeDialog(local_20);
                  auStack_776[local_777] = 0;
                  if (local_21 != '\0') {
                    cVar1 = _ValidateRegistrationCodeWithPrefix
                                      (auStack_460,auStack_776,&local_564,&local_566,&local_568);
                    if (cVar1 != '\0') {
                      local_33 = '\x01';
                    }
                  }
                  if (local_33 == '\0') {
                    cVar1 = _DoGetUpdateFailedDialog();
                    if (cVar1 != '\0') {
                      _ASWLaunchURL("http://www.actionsoft.com/support/update_code.php");
                      _ShutDown();
                    }
                  }
                }
              }
              goto LAB_00090b47;
            }
          }
        }
        if (local_34 == '\0') {
          local_34 = '\x01';
          _MacStopAlert(0x99);
        }
      }
    }
LAB_00090b47:
  } while ((local_4e == 1) && (local_34 != '\0'));
  if (local_4e == 8) {
    _ShutDown();
  }
  _DisposeModalFilterUPP(local_3c);
  _DisposeDialog(local_4c);
  if (local_33 != '\0') {
    _CreateLicenseFileForNameAndCode(&local_461,&local_561);
    bVar10 = param_2 == (void *)0x0;
    if (!bVar10) {
      iVar4 = 4;
      pcVar5 = local_361;
      pcVar7 = "EP1";
      do {
        pcVar6 = pcVar5;
        pcVar8 = pcVar7;
        if (iVar4 == 0) break;
        iVar4 = iVar4 + -1;
        pcVar8 = pcVar7 + 1;
        pcVar6 = pcVar5 + 1;
        bVar10 = *pcVar5 == *pcVar7;
        pcVar5 = pcVar6;
        pcVar7 = pcVar8;
      } while (bVar10);
      iVar4 = 0;
      if (!bVar10) {
        iVar4 = (uint)(byte)pcVar6[-1] - (uint)(byte)pcVar8[-1];
      }
      bVar10 = iVar4 == 0;
      if (bVar10) {
        _memcpy(param_2,
                "Thank you for purchasing Episode 1! All mansions in Episode 1 are now fully unlocked."
                ,0x56);
      }
      else {
        iVar4 = 4;
        pcVar5 = local_361;
        pcVar7 = "EP2";
        do {
          pcVar6 = pcVar5;
          pcVar8 = pcVar7;
          if (iVar4 == 0) break;
          iVar4 = iVar4 + -1;
          pcVar8 = pcVar7 + 1;
          pcVar6 = pcVar5 + 1;
          bVar10 = *pcVar5 == *pcVar7;
          pcVar5 = pcVar6;
          pcVar7 = pcVar8;
        } while (bVar10);
        iVar4 = 0;
        if (!bVar10) {
          iVar4 = (uint)(byte)pcVar6[-1] - (uint)(byte)pcVar8[-1];
        }
        bVar10 = iVar4 == 0;
        if (bVar10) {
          _memcpy(param_2,
                  "Thank you for purchasing Episode 2! All mansions in Episode 2 are now fully unlocked."
                  ,0x56);
        }
        else {
          iVar4 = 4;
          pcVar5 = local_361;
          pcVar7 = "MM1";
          do {
            pcVar6 = pcVar5;
            pcVar8 = pcVar7;
            if (iVar4 == 0) break;
            iVar4 = iVar4 + -1;
            pcVar8 = pcVar7 + 1;
            pcVar6 = pcVar5 + 1;
            bVar10 = *pcVar5 == *pcVar7;
            pcVar5 = pcVar6;
            pcVar7 = pcVar8;
          } while (bVar10);
          iVar4 = 0;
          if (!bVar10) {
            iVar4 = (uint)(byte)pcVar6[-1] - (uint)(byte)pcVar8[-1];
          }
          bVar10 = iVar4 == 0;
          if (bVar10) {
            _memcpy(param_2,
                    "Thank you for purchasing Episodes 1 and 2! The entire game is now fully unlocked."
                    ,0x52);
          }
          else {
            iVar4 = 4;
            pcVar5 = local_361;
            pcVar7 = "MM2";
            do {
              pcVar6 = pcVar5;
              pcVar8 = pcVar7;
              if (iVar4 == 0) break;
              iVar4 = iVar4 + -1;
              pcVar8 = pcVar7 + 1;
              pcVar6 = pcVar5 + 1;
              bVar10 = *pcVar5 == *pcVar7;
              pcVar5 = pcVar6;
              pcVar7 = pcVar8;
            } while (bVar10);
            iVar4 = 0;
            if (!bVar10) {
              iVar4 = (uint)(byte)pcVar6[-1] - (uint)(byte)pcVar8[-1];
            }
            if (iVar4 == 0) {
              _memcpy(param_2,
                      "Thank you for purchasing Midnight Mansion 2: The Haunted Hills! The game is now fully unlocked."
                      ,0x60);
            }
          }
        }
      }
    }
  }
  return local_33;
}


// ---- _DoGetUpdatedCodeDialog @ 0x90d77 (269 bytes)

bool _DoGetUpdatedCodeDialog(void)

{
  undefined4 uVar1;
  short local_16;
  int local_14;
  undefined4 local_10;
  
  local_14 = _GetNewDialog(0x118,0,0xffffffff);
  if (local_14 == 0) {
    _MMCantFindMacResource
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MMDialogsMac.c",
               PTR____FUNCTION___90824_001c51e8,0x78f);
  }
  _PrepareGameForDialogBox(0x17);
  _SetPortDialogPort(local_14);
  _SetDialogDefaultItem(local_14,1);
  _SetDialogCancelItem(local_14,2);
  uVar1 = _GetDialogWindow(local_14);
  _ShowWindow(uVar1);
  local_10 = _NewModalFilterUPP(PTR_MyStdDialogFilter_001c51e0);
  do {
    _ModalDialog(local_10,&local_16);
    if (local_16 == 2) break;
  } while (local_16 != 1);
  _DisposeModalFilterUPP(local_10);
  _DisposeDialog(local_14);
  return local_16 == 1;
}


// ---- _DoValidateCodeDialog @ 0x90e84 (295 bytes)

bool _DoValidateCodeDialog(char param_1)

{
  undefined4 uVar1;
  short local_1a;
  int local_18;
  undefined2 local_12;
  undefined4 local_10;
  
  if (param_1 == '\0') {
    local_12 = 0x127;
  }
  else {
    local_12 = 300;
  }
  local_18 = _GetNewDialog(local_12,0,0xffffffff);
  if (local_18 == 0) {
    _MMCantFindMacResource
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MMDialogsMac.c",
               PTR____FUNCTION___90845_001c51dc,0x7b9);
  }
  _PrepareGameForDialogBox(0x17);
  _SetPortDialogPort(local_18);
  _SetDialogDefaultItem(local_18,1);
  _SetDialogCancelItem(local_18,2);
  uVar1 = _GetDialogWindow(local_18);
  _ShowWindow(uVar1);
  local_10 = _NewModalFilterUPP(PTR_MyStdDialogFilter_001c51e0);
  do {
    _ModalDialog(local_10,&local_1a);
    if (local_1a == 2) break;
  } while (local_1a != 1);
  _DisposeModalFilterUPP(local_10);
  _DisposeDialog(local_18);
  return local_1a == 1;
}


// ---- _DoGetUpdateFailedDialog @ 0x90fab (269 bytes)

bool _DoGetUpdateFailedDialog(void)

{
  undefined4 uVar1;
  short local_16;
  int local_14;
  undefined4 local_10;
  
  local_14 = _GetNewDialog(0x122,0,0xffffffff);
  if (local_14 == 0) {
    _MMCantFindMacResource
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MMDialogsMac.c",
               PTR____FUNCTION___90869_001c51d8,0x7df);
  }
  _PrepareGameForDialogBox(0x17);
  _SetPortDialogPort(local_14);
  _SetDialogDefaultItem(local_14,1);
  _SetDialogCancelItem(local_14,2);
  uVar1 = _GetDialogWindow(local_14);
  _ShowWindow(uVar1);
  local_10 = _NewModalFilterUPP(PTR_MyStdDialogFilter_001c51e0);
  do {
    _ModalDialog(local_10,&local_16);
    if (local_16 == 2) break;
  } while (local_16 != 1);
  _DisposeModalFilterUPP(local_10);
  _DisposeDialog(local_14);
  return local_16 == 1;
}


// ---- _MyStdDialogFilter @ 0x910b8 (141 bytes)

char MyStdDialogFilter(undefined4 param_1,short *param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  char cStack_11;
  
  cStack_11 = '\0';
  if (*param_2 == 6) {
    iVar1 = *(int *)(param_2 + 1);
    _BeginUpdate(iVar1);
    iVar2 = _GetDialogWindow(param_1);
    if (iVar2 == iVar1) {
      _DrawDialog(param_1);
    }
    _EndUpdate(iVar1);
    cStack_11 = '\x01';
  }
  if (cStack_11 == '\0') {
    _SetPortDialogPort(param_1);
    cStack_11 = _StdFilterProc(param_1,param_2,param_3);
  }
  return cStack_11;
}


// ---- _GetFSRefForSaveGameFolder @ 0x91145 (68 bytes)

void _GetFSRefForSaveGameFolder(undefined4 param_1)

{
  undefined1 local_11;
  undefined4 local_10;
  
  local_10 = _GetSavedGamesFilePath(0,1);
  _FSPathMakeRef(local_10,param_1,&local_11);
  return;
}


// ---- _GetFSRefForPlayerProfilesFolder @ 0x91189 (54 bytes)

void _GetFSRefForPlayerProfilesFolder(undefined4 param_1)

{
  undefined1 local_11;
  undefined4 local_10;
  
  local_10 = _GetPlayerProfilePath(0);
  _FSPathMakeRef(local_10,param_1,&local_11);
  return;
}


// ---- _MyGetDialogControl @ 0x911bf (71 bytes)

void MyGetDialogControl(undefined4 param_1,short param_2,undefined4 *param_3)

{
  undefined4 uStack_1c;
  undefined1 auStack_16 [8];
  undefined1 auStack_e [10];
  
  _GetDialogItem(param_1,(int)param_2,auStack_e,&uStack_1c,auStack_16);
  *param_3 = uStack_1c;
  return;
}


// ---- _MySetDialogItemNumber @ 0x91206 (119 bytes)

void MySetDialogItemNumber(undefined4 param_1,short param_2,undefined4 param_3)

{
  undefined1 auStack_11c [256];
  int iStack_1c;
  undefined1 auStack_16 [8];
  undefined1 auStack_e [10];
  
  _GetDialogItem(param_1,(int)param_2,auStack_e,&iStack_1c,auStack_16);
  if (iStack_1c != 0) {
    _SWNumToString(param_3,auStack_11c);
    _SetDialogItemText(iStack_1c,auStack_11c);
  }
  return;
}


// ---- _GetDialogItemNumber @ 0x9127d (68 bytes)

undefined4 GetDialogItemNumber(undefined4 param_1)

{
  undefined4 uStack_110;
  undefined1 auStack_10c [264];
  
  _GetDialogItemText(param_1,auStack_10c);
  _SWStringToNum(auStack_10c,&uStack_110);
  return uStack_110;
}


// ---- _MySetDialogItemText @ 0x912c1 (86 bytes)

void _MySetDialogItemText(undefined4 param_1,short param_2,undefined4 param_3)

{
  int local_1c;
  undefined1 local_16 [8];
  undefined1 local_e [10];
  
  _GetDialogItem(param_1,(int)param_2,local_e,&local_1c,local_16);
  if (local_1c != 0) {
    _SetDialogItemText(local_1c,param_3);
  }
  return;
}


// ---- _MyGetDialogItemText @ 0x91317 (86 bytes)

void _MyGetDialogItemText(undefined4 param_1,short param_2,undefined4 param_3)

{
  int local_1c;
  undefined1 local_16 [8];
  undefined1 local_e [10];
  
  _GetDialogItem(param_1,(int)param_2,local_e,&local_1c,local_16);
  if (local_1c != 0) {
    _GetDialogItemText(local_1c,param_3);
  }
  return;
}


// ---- _RedrawDialogItem @ 0x9136d (89 bytes)

void _RedrawDialogItem(undefined4 param_1,short param_2)

{
  short sVar1;
  undefined4 uVar2;
  undefined4 local_14;
  int local_10;
  
  sVar1 = _GetDialogItemAsControl(param_1,(int)param_2,&local_14);
  local_10 = (int)sVar1;
  if (local_10 == 0) {
    _Draw1Control(local_14);
  }
  else {
    uVar2 = _GetDialogWindow(param_1);
    _DrawControls(uVar2);
  }
  return;
}


// ---- _SetCheckBoxValue @ 0x913c6 (93 bytes)

void _SetCheckBoxValue(undefined4 param_1,short param_2,undefined1 param_3)

{
  int local_1c;
  undefined1 local_16 [8];
  undefined1 local_e [10];
  
  _GetDialogItem(param_1,(int)param_2,local_e,&local_1c,local_16);
  if (local_1c != 0) {
    _SetControlValue(local_1c,param_3);
  }
  return;
}


// ---- _EnableMyControl @ 0x91423 (120 bytes)

void _EnableMyControl(undefined4 param_1,short param_2,char param_3)

{
  int local_1c;
  undefined1 local_16 [8];
  undefined1 local_e [10];
  
  _GetDialogItem(param_1,(int)param_2,local_e,&local_1c,local_16);
  if (local_1c != 0) {
    if (param_3 == '\0') {
      _HiliteControl(local_1c,0xff);
    }
    else {
      _HiliteControl(local_1c,0);
    }
  }
  return;
}


// ---- _ProcessCheckBoxClick @ 0x9149b (107 bytes)

void _ProcessCheckBoxClick(undefined4 param_1,short param_2)

{
  int local_1c;
  undefined1 local_18 [8];
  undefined1 local_10 [3];
  char local_d;
  
  _GetDialogItem(param_1,(int)param_2,local_10,&local_1c,local_18);
  if (local_1c != 0) {
    local_d = _GetControlValue(local_1c);
    _SetControlValue(local_1c,local_d == '\0');
  }
  return;
}


// ---- _GetCheckBoxValue @ 0x91506 (96 bytes)

bool _GetCheckBoxValue(undefined4 param_1,short param_2)

{
  int local_1c;
  undefined1 local_18 [8];
  undefined1 local_10 [3];
  char local_d;
  
  local_d = '\0';
  _GetDialogItem(param_1,(int)param_2,local_10,&local_1c,local_18);
  if (local_1c != 0) {
    local_d = _GetControlValue(local_1c);
  }
  return local_d == '\x01';
}


// ---- _MyInvalDialog @ 0x91566 (74 bytes)

void MyInvalDialog(undefined4 param_1)

{
  undefined4 uVar1;
  undefined1 auStack_18 [8];
  undefined4 uStack_10;
  
  uStack_10 = _GetDialogWindow(param_1);
  uVar1 = _GetWindowPort(uStack_10);
  _GetPortBounds(uVar1,auStack_18);
  _InvalWindowRect(uStack_10,auStack_18);
  return;
}


// ---- _DrawList @ 0x915b0 (102 bytes)

void _DrawList(undefined4 param_1)

{
  undefined4 local_12;
  char local_d;
  
  _SetPt(&local_12,0,0);
  do {
    _LDraw(local_12,param_1);
    local_d = _LNextCell(1,1,&local_12,param_1);
  } while (local_d == '\x01');
  return;
}


// ---- _DrawFrameAndFocusForList @ 0x91616 (133 bytes)

void _DrawFrameAndFocusForList(undefined4 param_1,char param_2)

{
  undefined1 local_14 [6];
  short local_e;
  
  _GetListViewBounds(param_1,local_14);
  local_e = local_e + 0x10;
  _DrawThemeFocusRect(local_14,0);
  if (param_2 == '\0') {
    _DrawThemeListBoxFrame(local_14,0);
  }
  else {
    _DrawThemeListBoxFrame(local_14,1);
  }
  _DrawThemeFocusRect(local_14,param_2);
  return;
}


// ---- _GetPascalStringFromHFSUni255 @ 0x9169b (90 bytes)

undefined4 _GetPascalStringFromHFSUni255(undefined4 param_1,undefined4 param_2)

{
  int local_14;
  undefined4 local_10;
  
  local_10 = 0;
  _CreateCFStringFromHFSUniStr(param_2,&local_14);
  if (local_14 == 0) {
    local_10 = 0xffffffff;
  }
  else {
    local_10 = _GetPascalStringFromCFString(param_1,local_14);
    _CFRelease(local_14);
  }
  return local_10;
}


// ---- _MMCantFindMacResource @ 0x916f5 (84 bytes)

void _MMCantFindMacResource(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  _fprintf((FILE *)(PTR_001c43e8 + 0xb0),
           "\nCAN\'T FIND DIALOG RESOURCE: \n File: %s \nfunction:%s \nline: %d\n",param_1,param_2,
           param_3);
                    /* WARNING: Subroutine does not return */
  _exit(1);
}


// ---- _MacStopAlert @ 0x91749 (108 bytes)

void _MacStopAlert(short param_1)

{
  char cVar1;
  
  cVar1 = *PTR__gDialogBoxIsOpen_001c49c8;
  _PrepareGameForDialogBox(0x17);
  _DoGameFadeToNormal(1);
  _StopAlert((int)param_1,0);
  if (cVar1 == '\0') {
    _RestoreGameAfterDialogBox(1);
  }
  return;
}


