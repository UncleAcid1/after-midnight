// Ghidra decompilation of functions from original source file:
//   Sources & Headers/MyFileUtilsMac.c

// ---- _MyNavOpenDialog @ 0x917b5 (755 bytes)

int _MyNavOpenDialog(void *param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5,
                    undefined4 param_6)

{
  short sVar1;
  undefined1 local_1d4 [6];
  undefined4 local_1ce;
  undefined4 local_1ca;
  undefined1 local_d4 [4];
  undefined1 local_d0 [4];
  undefined1 local_cc [4];
  undefined4 local_c8;
  undefined4 local_c4;
  undefined1 local_c0 [80];
  char local_70 [4];
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined1 local_5e [2];
  undefined4 local_5c;
  cfstringStruct *local_50;
  cfstringStruct *local_40;
  undefined4 local_1c;
  int local_18;
  int local_14;
  int local_10;
  
  sVar1 = (short)param_6;
  local_18 = 0;
  local_14 = _NavGetDefaultDialogCreationOptions(local_5e);
  if (local_14 == 0) {
    local_5c = 0;
    if (param_3 == 0x53415645) {
      local_50 = &cf_OpenSavedGame;
      local_40 = &cf_Selectasavedgamefiletoopen;
    }
    else if (param_3 == 0x4d4f5632) {
      local_50 = &cf_OpenReplay;
      local_40 = &cf_Selectareplayfiletoopen;
    }
    else {
      local_50 = &cf_OpenCustomMansion;
      local_40 = &cf_Selectamansiontoopen;
      param_3 = 0;
    }
    if ((param_2 != 0) && (param_3 != 0)) {
      local_18 = _MyCreateNavTypeListHandle(param_2,1,&param_3);
    }
    if (local_18 != 0) {
      _HLock(local_18);
    }
    local_70[0] = '\0';
    local_6c = param_4;
    local_68 = param_5;
    local_1c = _NewNavEventUPP(PTR_LAB_001c5200);
    if (sVar1 == 0) {
      local_14 = _NavCreateChooseFileDialog(local_5e,local_18,local_1c,0,0,local_70,&local_64);
    }
    else if (sVar1 == 1) {
      local_14 = _NavCreateChooseFolderDialog(local_5e,local_1c,0,local_70,&local_64);
    }
    else {
      local_14 = _NavCreateChooseObjectDialog(local_5e,local_1c,0,0,local_70,&local_64);
    }
    if (local_18 != 0) {
      _HUnlock(local_18);
      _DisposeHandle(local_18);
    }
  }
  if (local_14 == 0) {
    local_14 = _NavDialogRun(local_64);
    do {
    } while (local_70[0] == '\0');
  }
  if (local_14 == 0) {
    local_10 = _NavDialogGetUserAction(local_64);
    if ((local_10 == 0) || (local_10 == 1)) {
      local_14 = -1;
    }
    else {
      local_14 = _NavDialogGetReply(local_64,local_1d4);
      if (local_14 == 0) {
        local_c8 = local_1ce;
        local_c4 = local_1ca;
        sVar1 = _AEGetNthPtr(&local_c8,1,0x66737266,local_d0,local_cc,local_c0,0x50,local_d4);
        local_14 = (int)sVar1;
        _memcpy(param_1,local_c0,0x50);
        _NavDisposeReply(local_1d4);
      }
    }
    _NavDialogDispose(local_64);
    _DisposeNavEventUPP(local_1c);
  }
  return local_14;
}


// ---- _MyNavSaveDialog @ 0x91aa8 (493 bytes)

int _MyNavSaveDialog(void *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                    undefined4 param_5,int param_6,undefined4 *param_7)

{
  short sVar1;
  undefined1 local_d0 [4];
  undefined1 local_cc [4];
  undefined1 local_c8 [4];
  undefined4 local_c4;
  undefined4 local_c0;
  char local_bc [4];
  undefined4 local_b8;
  undefined4 local_b4;
  undefined1 local_ae [80];
  undefined1 local_5e [14];
  cfstringStruct *local_50;
  int local_44;
  cfstringStruct *local_40;
  undefined4 local_1c;
  undefined1 local_15;
  int local_14;
  int local_10;
  
  local_1c = _NewNavEventUPP(PTR_LAB_001c5200);
  local_15 = 0;
  local_14 = 0;
  local_14 = _NavGetDefaultDialogCreationOptions(local_5e);
  if (local_14 == 0) {
    if (param_6 != 0) {
      local_44 = param_6;
    }
    local_50 = &cf_SaveGame;
    local_40 = &cf_Chooseanameandlocationforyoursavedgamefile;
    local_bc[0] = '\0';
    local_b8 = param_4;
    local_b4 = param_5;
    local_14 = _NavCreatePutFileDialog
                         (local_5e,param_3,param_2,local_1c,local_bc,PTR__gMyNavDialog_001c5204);
  }
  if (local_14 == 0) {
    local_14 = _NavDialogRun(*(undefined4 *)PTR__gMyNavDialog_001c5204);
    do {
    } while (local_bc[0] == '\0');
  }
  if (local_14 == 0) {
    local_10 = _NavDialogGetUserAction(*(undefined4 *)PTR__gMyNavDialog_001c5204);
    if ((local_10 == 0) || (local_10 == 1)) {
      local_14 = -1;
    }
    else {
      local_14 = _NavDialogGetReply(*(undefined4 *)PTR__gMyNavDialog_001c5204,
                                    PTR__gNavReply_001c5210);
      if (local_14 == 0) {
        local_c0 = *(undefined4 *)(PTR__gNavReply_001c5210 + 10);
        local_c4 = *(undefined4 *)(PTR__gNavReply_001c5210 + 6);
        sVar1 = _AEGetNthPtr(&local_c4,1,0x66737266,local_cc,local_c8,local_ae,0x50,local_d0);
        local_14 = (int)sVar1;
        _memcpy(param_1,local_ae,0x50);
        *param_7 = *(undefined4 *)(PTR__gNavReply_001c5210 + 0x18);
      }
    }
  }
  return local_14;
}


// ---- _MyFinishNavSave @ 0x91c95 (111 bytes)

void MyFinishNavSave(void)

{
  if (*(int *)PTR__gMyNavDialog_001c5204 != 0) {
    _NavCompleteSave(PTR__gNavReply_001c5210,0);
    _NavDisposeReply(PTR__gNavReply_001c5210);
    _NavDialogDispose(*(undefined4 *)PTR__gMyNavDialog_001c5204);
    *(undefined4 *)PTR__gMyNavDialog_001c5204 = 0;
  }
  return;
}


// ---- _MyCreateNavTypeListHandle @ 0x91d04 (188 bytes)

int * _MyCreateNavTypeListHandle(undefined4 param_1,short param_2,int param_3)

{
  int *local_14;
  
  local_14 = (int *)0x0;
  if (param_3 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MyFileUtilsMac.c",
               PTR____FUNCTION___89833_001c5208,0x18e);
  }
  if (0 < param_2) {
    local_14 = (int *)_NewHandle(param_2 * 4 + 0xc);
    if (local_14 != (int *)0x0) {
      *(undefined4 *)*local_14 = param_1;
      *(short *)(*local_14 + 6) = param_2;
      _BlockMoveData(param_3,*local_14 + 8,(int)param_2 << 2);
    }
  }
  return local_14;
}


// ---- _BlockMoveData @ 0x91dc0 (45 bytes)

void _BlockMoveData(void *param_1,void *param_2,size_t param_3)

{
  if (0 < (int)param_3) {
    _memmove(param_2,param_1,param_3);
  }
  return;
}


// ---- _HandleNavEvent @ 0x91ded (238 bytes)

void HandleNavEvent(int param_1,int param_2,undefined1 *param_3)

{
  short sVar1;
  undefined1 auStack_24 [8];
  undefined1 *puStack_1c;
  int iStack_18;
  short *psStack_14;
  undefined4 uStack_10;
  
  puStack_1c = param_3;
  if ((param_1 == 2) && (_FlushEvents(0x38,0), *(int *)(puStack_1c + 4) != 0)) {
    sVar1 = _AECreateDesc(0x66737266,*(undefined4 *)(puStack_1c + 4),0x50,auStack_24);
    iStack_18 = (int)sVar1;
    if (iStack_18 == 0) {
      sVar1 = _NavCustomControl(*(undefined4 *)(param_2 + 2),8,auStack_24);
      iStack_18 = (int)sVar1;
    }
  }
  if (param_1 == 0xc) {
    *puStack_1c = 1;
  }
  else if (param_1 == 0) {
    psStack_14 = *(short **)(param_2 + 0x1a);
    if (*(int *)(puStack_1c + 8) == 0) {
      if (*psStack_14 == 6) {
        uStack_10 = *(undefined4 *)(psStack_14 + 1);
        _BeginUpdate(uStack_10);
        _EndUpdate(uStack_10);
      }
    }
    else {
      (**(code **)(puStack_1c + 8))(psStack_14);
    }
  }
  return;
}


// ---- _DefaultNavEventCallback @ 0x91edb (61 bytes)

void DefaultNavEventCallback(short *param_1)

{
  undefined4 uVar1;
  
  if (*param_1 == 6) {
    uVar1 = *(undefined4 *)(param_1 + 1);
    _BeginUpdate(uVar1);
    _EndUpdate(uVar1);
  }
  return;
}


// ---- _MyCreateAndOpenFile @ 0x91f18 (533 bytes)

int MyCreateAndOpenFile(void *param_1,undefined4 param_2,char param_3,undefined4 param_4,
                       undefined4 param_5,undefined4 param_6)

{
  short sVar1;
  undefined2 uStack_500;
  undefined1 auStack_4fe [510];
  undefined1 auStack_300 [72];
  undefined4 uStack_2b8;
  undefined4 uStack_2b4;
  undefined4 uStack_270;
  undefined4 uStack_26c;
  undefined1 auStack_266 [510];
  undefined1 auStack_68 [80];
  undefined4 uStack_18;
  int iStack_14;
  undefined4 *puStack_10;
  
  iStack_14 = 0;
  uStack_26c = _CFStringGetLength(param_2);
  uStack_270 = 0;
  uStack_18 = uStack_26c;
  _CFStringGetCharacters(param_2,0,uStack_26c,auStack_266);
  sVar1 = _FSCreateFileUnicode(param_1,uStack_18,auStack_266,0,0,auStack_68,0);
  iStack_14 = (int)sVar1;
  if (iStack_14 == 0) {
    _BlockZero(auStack_300,0x90);
    sVar1 = _FSGetCatalogInfo(auStack_68,0x800,auStack_300,0,0,0);
    iStack_14 = (int)sVar1;
    if (iStack_14 == 0) {
      puStack_10 = &uStack_2b8;
      uStack_2b4 = param_4;
      uStack_2b8 = param_5;
      sVar1 = _FSSetCatalogInfo(auStack_68,0x800,auStack_300);
      iStack_14 = (int)sVar1;
    }
  }
  if (iStack_14 == -0x30) {
    _CFStringToHFSUniStr(param_2,&uStack_500);
    sVar1 = _FSMakeFSRefUnicode(param_1,uStack_500,auStack_4fe,0xffff,auStack_68);
    iStack_14 = (int)sVar1;
  }
  if (iStack_14 == 0) {
    sVar1 = _FSGetDataForkName(&uStack_500);
    iStack_14 = (int)sVar1;
    if (iStack_14 == 0) {
      sVar1 = _FSOpenFork(auStack_68,uStack_500,auStack_4fe,(int)param_3,param_6);
      iStack_14 = (int)sVar1;
    }
    _memcpy(param_1,auStack_68,0x50);
  }
  return iStack_14;
}


// ---- _BlockZero @ 0x9212d (38 bytes)

void _BlockZero(void *param_1,size_t param_2)

{
  if (0 < (int)param_2) {
    _bzero(param_1,param_2);
  }
  return;
}


// ---- _MyOpenFile @ 0x92153 (120 bytes)

int MyOpenFile(void)

{
  char cVar1;
  short sVar2;
  undefined4 in_stack_00000054;
  undefined4 in_stack_00000058;
  undefined2 uStack_210;
  undefined1 auStack_20e [510];
  int iStack_10;
  
  cVar1 = (char)in_stack_00000054;
  iStack_10 = 0;
  sVar2 = _FSGetDataForkName(&uStack_210);
  iStack_10 = (int)sVar2;
  if (iStack_10 == 0) {
    sVar2 = _FSOpenFork(&stack0x00000004,uStack_210,auStack_20e,(int)cVar1,in_stack_00000058);
    iStack_10 = (int)sVar2;
  }
  return iStack_10;
}


// ---- _MyOpenResFileUsingFSRef @ 0x921cb (79 bytes)

int _MyOpenResFileUsingFSRef(void)

{
  short sVar1;
  undefined4 in_stack_00000054;
  short *in_stack_00000058;
  int local_14;
  
  local_14 = 0;
  sVar1 = _FSOpenResFile(&stack0x00000004,(int)(char)in_stack_00000054);
  if (sVar1 < 1) {
    local_14 = (int)sVar1;
  }
  else {
    *in_stack_00000058 = sVar1;
  }
  return local_14;
}


// ---- _MyCloseFile @ 0x9221a (33 bytes)

void MyCloseFile(short param_1)

{
  _FSCloseFork((int)param_1);
  return;
}


// ---- _MyDeleteFile @ 0x9223b (184 bytes)

int MyDeleteFile(void *param_1,undefined4 param_2)

{
  short sVar1;
  undefined2 uStack_464;
  undefined1 auStack_462 [510];
  undefined1 auStack_264 [512];
  undefined1 auStack_64 [80];
  int iStack_14;
  int iStack_10;
  
  iStack_14 = 0;
  _memcpy(auStack_64,param_1,0x50);
  _CFStringToHFSUniStr(param_2,auStack_264);
  iStack_14 = _AppendUnicodePathToFSRef(auStack_64,auStack_264);
  sVar1 = _FSGetResourceForkName(&uStack_464);
  iStack_10 = (int)sVar1;
  if (iStack_10 == 0) {
    _FSDeleteFork(auStack_64,uStack_464,auStack_462);
  }
  if (iStack_14 == 0) {
    sVar1 = _FSDeleteObject(auStack_64);
    iStack_14 = (int)sVar1;
  }
  return iStack_14;
}


// ---- _MyOffsetFileMarker @ 0x922f3 (58 bytes)

int MyOffsetFileMarker(short param_1,int param_2)

{
  short sVar1;
  
  sVar1 = _FSSetForkPosition((int)param_1,3,param_2,param_2 >> 0x1f);
  return (int)sVar1;
}


// ---- _MyCreateAndOpenResFile @ 0x9232d (560 bytes)

int MyCreateAndOpenResFile
              (void *param_1,undefined4 param_2,char param_3,undefined4 param_4,undefined4 param_5,
              undefined4 param_6)

{
  short sVar1;
  undefined2 uStack_700;
  undefined1 auStack_6fe [510];
  undefined2 uStack_500;
  undefined1 auStack_4fe [510];
  undefined1 auStack_300 [72];
  undefined4 uStack_2b8;
  undefined4 uStack_2b4;
  undefined4 uStack_270;
  undefined4 uStack_26c;
  undefined1 auStack_266 [510];
  undefined1 auStack_68 [80];
  undefined4 uStack_18;
  int iStack_14;
  undefined4 *puStack_10;
  
  iStack_14 = 0;
  uStack_26c = _CFStringGetLength(param_2);
  uStack_270 = 0;
  uStack_18 = uStack_26c;
  _CFStringGetCharacters(param_2,0,uStack_26c,auStack_266);
  sVar1 = _FSGetResourceForkName(&uStack_500);
  iStack_14 = (int)sVar1;
  if (iStack_14 == 0) {
    sVar1 = _FSCreateResourceFile
                      (param_1,uStack_18,auStack_266,0,0,uStack_500,auStack_4fe,auStack_68,0);
    iStack_14 = (int)sVar1;
  }
  if (iStack_14 == 0) {
    _BlockZero(auStack_300,0x90);
    sVar1 = _FSGetCatalogInfo(auStack_68,0x800,auStack_300,0,0,0);
    iStack_14 = (int)sVar1;
    puStack_10 = &uStack_2b8;
    uStack_2b4 = param_4;
    uStack_2b8 = param_5;
    sVar1 = _FSSetCatalogInfo(auStack_68,0x800,auStack_300);
    iStack_14 = (int)sVar1;
  }
  if (iStack_14 == -0x30) {
    _CFStringToHFSUniStr(param_2,&uStack_700);
    sVar1 = _FSMakeFSRefUnicode(param_1,uStack_700,auStack_6fe,0xffff,auStack_68);
    iStack_14 = (int)sVar1;
  }
  if (iStack_14 == 0) {
    sVar1 = _FSOpenResourceFile(auStack_68,uStack_500,auStack_4fe,(int)param_3,param_6);
    iStack_14 = (int)sVar1;
    _memcpy(param_1,auStack_68,0x50);
  }
  return iStack_14;
}


// ---- _DuplicateResources @ 0x9255d (186 bytes)

int DuplicateResources(undefined4 param_1,short param_2,short param_3)

{
  short sVar1;
  int iVar2;
  int iStack_14;
  
  iVar2 = _Get1Resource(param_1,(int)param_2);
  if (iVar2 == 0) {
    iStack_14 = -0xc0;
  }
  else {
    _DeleteResource(param_1,(int)param_3);
    _DetachResource(iVar2);
    _AddResource(iVar2,param_1,(int)param_3,&DAT_0012118a);
    sVar1 = _ResError();
    iStack_14 = (int)sVar1;
    _ReleaseResource(iVar2);
    sVar1 = _CurResFile();
    _UpdateResFile((int)sVar1);
  }
  return iStack_14;
}


// ---- _ChangeAllResourceTypes @ 0x92617 (258 bytes)

int ChangeAllResourceTypes(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  short sVar1;
  int iVar2;
  int iStack_18;
  short sStack_12;
  
  iStack_18 = 0;
  for (sStack_12 = 0; sStack_12 != 0x7fff; sStack_12 = sStack_12 + 1) {
    if (param_4 != 0) {
      _SetDialogItemNumber(param_4,(int)sStack_12);
      _DrawDialog(param_3);
      _GiveOSTime();
    }
    iVar2 = _Get1Resource(param_1,(int)sStack_12);
    if (iVar2 != 0) {
      _DeleteResource(param_2,(int)sStack_12);
      _DetachResource(iVar2);
      _AddResource(iVar2,param_2,(int)sStack_12,&DAT_0012118a);
      if (iStack_18 == 0) {
        sVar1 = _ResError();
        iStack_18 = (int)sVar1;
      }
      _ReleaseResource(iVar2);
      sVar1 = _CurResFile();
      _UpdateResFile((int)sVar1);
      _DeleteResource(param_1,(int)sStack_12);
    }
  }
  return iStack_18;
}


// ---- _SetDialogItemNumber @ 0x92719 (59 bytes)

void _SetDialogItemNumber(undefined4 param_1,undefined4 param_2)

{
  undefined1 local_10c [264];
  
  _NumToString(param_2,local_10c);
  _SetDialogItemText(param_1,local_10c);
  return;
}


// ---- _CFStringToHFSUniStr @ 0x92754 (142 bytes)

void _CFStringToHFSUniStr(int param_1,undefined2 *param_2)

{
  int local_18;
  
  if (param_1 == 0) {
    _BlockZero(param_2,0x200);
  }
  else {
    local_18 = _CFStringGetLength(param_1);
    if (0xff < local_18) {
      local_18 = 0xff;
    }
    *param_2 = (short)local_18;
    _CFStringGetCharacters(param_1,0,local_18,param_2 + 1);
  }
  return;
}


// ---- _CStringToHFSUniStr @ 0x927e2 (90 bytes)

void _CStringToHFSUniStr(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  
  uVar1 = _CFStringCreateWithCString(*(undefined4 *)PTR_001c51d4,param_1,0);
  _CFStringToHFSUniStr(uVar1,param_2);
  _CFRelease(uVar1);
  return;
}


// ---- _PascalStringToHFSUniStr @ 0x9283c (90 bytes)

void _PascalStringToHFSUniStr(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  
  uVar1 = _CFStringCreateWithPascalString(*(undefined4 *)PTR_001c51d4,param_1,0);
  _CFStringToHFSUniStr(uVar1,param_2);
  _CFRelease(uVar1);
  return;
}


// ---- _DeleteResource @ 0x92896 (98 bytes)

void _DeleteResource(undefined4 param_1,short param_2)

{
  short sVar1;
  int iVar2;
  
  _SetResLoad(0);
  iVar2 = _Get1Resource(param_1,(int)param_2);
  _SetResLoad(1);
  if (iVar2 != 0) {
    _RemoveResource(iVar2);
  }
  sVar1 = _CurResFile();
  _UpdateResFile((int)sVar1);
  return;
}


// ---- _CountNumSuccessiveResources @ 0x928f8 (76 bytes)

int CountNumSuccessiveResources(undefined4 param_1,short param_2)

{
  char cVar1;
  undefined2 uStack_10;
  undefined2 uStack_e;
  
  uStack_10 = param_2;
  uStack_e = 0;
  while( true ) {
    cVar1 = _ResourceExists(param_1,(int)uStack_10);
    if (cVar1 == '\0') break;
    uStack_e = uStack_e + 1;
    uStack_10 = uStack_10 + 1;
  }
  return (int)uStack_e;
}


// ---- _CreateDirectory @ 0x92944 (224 bytes)

int _CreateDirectory(undefined4 param_1,undefined4 param_2)

{
  char cVar1;
  short sVar2;
  undefined1 local_260 [80];
  undefined2 local_210;
  undefined1 auStack_20e [510];
  int local_10;
  
  local_10 = 0;
  _CStringToHFSUniStr(param_2,&local_210);
  cVar1 = _DoesFileOrFolderExistInPath(param_1,param_2);
  if (cVar1 == '\0') {
    _printf("Creating directory: %s/%s\n",param_1,param_2);
    _PathToFSRef(param_1,local_260);
    sVar2 = _FSCreateDirectoryUnicode(local_260,local_210,auStack_20e,0,0,local_260,0,0);
    local_10 = (int)sVar2;
  }
  return local_10;
}


// ---- _DoesFileOrFolderExistInPath @ 0x92a24 (139 bytes)

bool _DoesFileOrFolderExistInPath(undefined4 param_1,undefined4 param_2)

{
  short sVar1;
  undefined1 local_2b0 [80];
  undefined1 local_260 [80];
  undefined2 local_210;
  undefined1 auStack_20e [510];
  undefined4 local_10;
  
  local_10 = 0;
  _CStringToHFSUniStr(param_2,&local_210);
  _PathToFSRef(param_1,local_260);
  sVar1 = _FSMakeFSRefUnicode(local_260,local_210,auStack_20e,0xffff,local_2b0);
  return sVar1 != -0x2b;
}


// ---- _SetFileCreatorAndTypeForPath @ 0x92aaf (112 bytes)

void _SetFileCreatorAndTypeForPath(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 local_dc;
  undefined1 *local_d8;
  undefined1 *local_d4;
  undefined4 local_8c;
  undefined4 local_88;
  undefined1 *local_74;
  uint local_70;
  undefined1 local_5f;
  undefined1 local_5e [80];
  short local_e;
  
  local_d4 = &local_5f;
  local_d8 = local_5e;
  local_dc = param_1;
  local_e = _FSPathMakeRef();
  if (local_e == 0) {
    local_88 = param_3;
    local_8c = param_2;
    local_74 = local_5e;
    for (local_70 = 0; local_70 < 0x50; local_70 = local_70 + 1) {
      *(undefined1 *)((int)&local_dc + local_70) = local_74[local_70];
    }
    _SetFileCreatorAndType();
  }
  return;
}


// ---- _SetFileCreatorAndType @ 0x92b1f (171 bytes)

void _SetFileCreatorAndType(void)

{
  short sVar1;
  undefined4 in_stack_00000054;
  undefined4 in_stack_00000058;
  undefined1 local_a4 [72];
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 *local_14;
  int local_10;
  
  local_10 = 0;
  _BlockZero(local_a4,0x90);
  sVar1 = _FSGetCatalogInfo(&stack0x00000004,0x800,local_a4,0,0,0);
  local_10 = (int)sVar1;
  if (local_10 == 0) {
    local_14 = &local_5c;
    local_58 = in_stack_00000054;
    local_5c = in_stack_00000058;
    _FSSetCatalogInfo(&stack0x00000004,0x800,local_a4);
  }
  return;
}


// ---- _SetFileInvisibleForPath @ 0x92bca (98 bytes)

void SetFileInvisibleForPath(undefined4 param_1)

{
  undefined4 uStack_cc;
  undefined1 *puStack_c8;
  undefined1 *puStack_c4;
  undefined1 *puStack_74;
  uint uStack_70;
  undefined1 uStack_5f;
  undefined1 auStack_5e [80];
  short sStack_e;
  
  puStack_c4 = &uStack_5f;
  puStack_c8 = auStack_5e;
  uStack_cc = param_1;
  sStack_e = _FSPathMakeRef();
  if (sStack_e == 0) {
    puStack_74 = auStack_5e;
    for (uStack_70 = 0; uStack_70 < 0x50; uStack_70 = uStack_70 + 1) {
      *(undefined1 *)((int)&uStack_cc + uStack_70) = puStack_74[uStack_70];
    }
    _SetFileInvisible();
  }
  return;
}


// ---- _SetFileInvisible @ 0x92c2c (173 bytes)

void _SetFileInvisible(void)

{
  short sVar1;
  undefined1 local_a4 [72];
  undefined1 auStack_5c [8];
  ushort local_54;
  undefined1 *local_14;
  int local_10;
  
  local_10 = 0;
  _BlockZero(local_a4,0x90);
  sVar1 = _FSGetCatalogInfo(&stack0x00000004,0x800,local_a4,0,0,0);
  local_10 = (int)sVar1;
  if (local_10 == 0) {
    local_14 = auStack_5c;
    local_54 = local_54 | 0x4000;
    _FSSetCatalogInfo(&stack0x00000004,0x800,local_a4);
  }
  return;
}


// ---- _GetFSRefForUserFolder @ 0x92cd9 (107 bytes)

int GetFSRefForUserFolder(undefined4 param_1,int param_2)

{
  short sVar1;
  undefined4 uStack_10;
  
  if (param_2 == 0x4150504c) {
    uStack_10 = _GetApplicationFSRef(param_1);
  }
  else {
    sVar1 = _FSFindFolder(0xffff8005,param_2,0,param_1);
    uStack_10 = (int)sVar1;
    if (uStack_10 != 0) {
      uStack_10 = _GetApplicationFSRef(param_1);
    }
  }
  return uStack_10;
}


// ---- _CreateFolderAndGetFSRef @ 0x92d44 (265 bytes)

int CreateFolderAndGetFSRef(int param_1,int param_2)

{
  char cVar1;
  short sVar2;
  int iStack_28c;
  ushort *puStack_288;
  undefined1 *puStack_284;
  undefined4 uStack_280;
  int iStack_27c;
  int iStack_278;
  undefined4 uStack_274;
  undefined4 uStack_270;
  int iStack_23c;
  uint uStack_224;
  int iStack_220;
  ushort uStack_210;
  undefined1 auStack_20e [510];
  int iStack_10;
  
  iStack_10 = 0;
  puStack_288 = &uStack_210;
  iStack_28c = param_2;
  _PascalStringToHFSUniStr();
  iStack_23c = param_2;
  iStack_220 = param_1;
  for (uStack_224 = 0; uStack_224 < 0x50; uStack_224 = uStack_224 + 1) {
    *(undefined1 *)((int)&iStack_28c + uStack_224) = *(undefined1 *)(param_1 + uStack_224);
  }
  cVar1 = _DoesFileOrFolderExist();
  if (cVar1 == '\0') {
    puStack_288 = (ushort *)(uint)uStack_210;
    uStack_270 = 0;
    uStack_274 = 0;
    iStack_278 = param_1;
    iStack_27c = 0;
    uStack_280 = 0;
    puStack_284 = auStack_20e;
    iStack_28c = param_1;
    sVar2 = _FSCreateDirectoryUnicode();
  }
  else {
    puStack_288 = (ushort *)(uint)uStack_210;
    iStack_27c = param_1;
    uStack_280 = 0xffff;
    puStack_284 = auStack_20e;
    iStack_28c = param_1;
    sVar2 = _FSMakeFSRefUnicode();
  }
  iStack_10 = (int)sVar2;
  return iStack_10;
}


// ---- _AppendPathToFSRef @ 0x92e4d (105 bytes)

int AppendPathToFSRef(undefined4 param_1,undefined4 param_2)

{
  short sVar1;
  undefined2 uStack_210;
  undefined1 auStack_20e [510];
  undefined4 uStack_10;
  
  uStack_10 = 0;
  _PascalStringToHFSUniStr(param_2,&uStack_210);
  sVar1 = _FSMakeFSRefUnicode(param_1,uStack_210,auStack_20e,0xffff,param_1);
  return (int)sVar1;
}


// ---- _DoesFileOrFolderExist @ 0x92eb6 (115 bytes)

bool _DoesFileOrFolderExist(void)

{
  short sVar1;
  undefined4 in_stack_00000054;
  undefined1 local_260 [80];
  undefined2 local_210;
  undefined1 auStack_20e [510];
  undefined4 local_10;
  
  local_10 = 0;
  _PascalStringToHFSUniStr(in_stack_00000054,&local_210);
  sVar1 = _FSMakeFSRefUnicode(&stack0x00000004,local_210,auStack_20e,0xffff,local_260);
  return sVar1 != -0x2b;
}


// ---- _DoesFileOrFolderExistHSFUni @ 0x92f29 (85 bytes)

bool _DoesFileOrFolderExistHSFUni(void)

{
  short sVar1;
  undefined2 in_stack_00000054;
  undefined1 local_60 [80];
  undefined4 local_10;
  
  local_10 = 0;
  sVar1 = _FSMakeFSRefUnicode(&stack0x00000004,in_stack_00000054,&stack0x00000056,0xffff,local_60);
  return sVar1 != -0x2b;
}


// ---- _AppendUnicodePathToFSRef @ 0x92f7e (135 bytes)

int _AppendUnicodePathToFSRef(undefined4 param_1,undefined2 *param_2)

{
  short sVar1;
  
  if (param_2 == (undefined2 *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/MyFileUtilsMac.c",
               PTR____FUNCTION___90327_001c520c,0x67e);
  }
  sVar1 = _FSMakeFSRefUnicode(param_1,*param_2,param_2 + 1,0xffff,param_1);
  return (int)sVar1;
}


// ---- _PathToFSRef @ 0x93005 (47 bytes)

int _PathToFSRef(undefined4 param_1,undefined4 param_2)

{
  short sVar1;
  undefined1 local_f [11];
  
  sVar1 = _FSPathMakeRef(param_1,param_2,local_f);
  return (int)sVar1;
}


// ---- _FSRefToPath @ 0x93034 (48 bytes)

int FSRefToPath(undefined4 param_1,undefined4 param_2,short param_3)

{
  short sVar1;
  
  sVar1 = _FSRefMakePath(param_1,param_2,(int)param_3);
  return (int)sVar1;
}


// ---- _GetDomainForPrefs @ 0x93064 (19 bytes)

undefined4 GetDomainForPrefs(void)

{
  return 0xffff8005;
}


// ---- _GetApplicationFSRef @ 0x93077 (109 bytes)

undefined4 _GetApplicationFSRef(undefined4 param_1)

{
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  local_18 = 0;
  local_14 = 2;
  local_10 = 0;
  local_10 = _GetProcessBundleLocation(&local_18,param_1);
  _FSGetCatalogInfo(param_1,0,0,0,0,param_1);
  return local_10;
}


// ---- _GetFSSpecFromFSRef @ 0x930e4 (71 bytes)

int GetFSSpecFromFSRef(undefined4 param_1,undefined4 param_2)

{
  short sVar1;
  
  sVar1 = _FSGetCatalogInfo(param_1,0,0,0,param_2,0);
  return (int)sVar1;
}


// ---- _GetFilenameFromFSRef @ 0x9312b (71 bytes)

int GetFilenameFromFSRef(undefined4 param_1,undefined4 param_2)

{
  short sVar1;
  
  sVar1 = _FSGetCatalogInfo(param_1,0,0,param_2,0,0);
  return (int)sVar1;
}


// ---- _GetCFilenameFromFSRef @ 0x93172 (128 bytes)

int _GetCFilenameFromFSRef(undefined4 param_1,char *param_2)

{
  short sVar1;
  char local_310 [256];
  undefined1 local_210 [512];
  int local_10;
  
  sVar1 = _FSGetCatalogInfo(param_1,0,0,local_210,0,0);
  local_10 = (int)sVar1;
  if (local_10 == 0) {
    _MyGetPascalStringFromHFSUni255(local_310,local_210);
    _SWCopyPascalStringToCString(local_310,param_2);
  }
  return local_10;
}


// ---- _GetPascalStringFromCFString @ 0x931f2 (76 bytes)

undefined4 _GetPascalStringFromCFString(undefined4 param_1,undefined4 param_2)

{
  char cVar1;
  undefined4 local_20;
  
  cVar1 = _CFStringGetPascalString(param_2,param_1,0xff,0);
  if (cVar1 == '\0') {
    local_20 = 0xffffffff;
  }
  else {
    local_20 = 0;
  }
  return local_20;
}


// ---- _GetCStringFromCFString @ 0x9323e (76 bytes)

undefined4 GetCStringFromCFString(undefined4 param_1,undefined4 param_2)

{
  char cVar1;
  undefined4 uStack_20;
  
  cVar1 = _CFStringGetCString(param_2,param_1,0xff,0);
  if (cVar1 == '\0') {
    uStack_20 = 0xffffffff;
  }
  else {
    uStack_20 = 0;
  }
  return uStack_20;
}


// ---- _CreateCFStringFromHFSUniStr @ 0x9328a (73 bytes)

void _CreateCFStringFromHFSUniStr(undefined2 *param_1,undefined4 *param_2)

{
  undefined4 uVar1;
  
  uVar1 = _CFStringCreateWithCharacters(*(undefined4 *)PTR_001c51d4,param_1 + 1,*param_1);
  *param_2 = uVar1;
  return;
}


// ---- _MyGetPascalStringFromHFSUni255 @ 0x932d3 (90 bytes)

undefined4 _MyGetPascalStringFromHFSUni255(undefined4 param_1,undefined4 param_2)

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


// ---- _SearchFolderForSubfile @ 0x9332d (637 bytes)

undefined4 SearchFolderForSubfile(void *param_1)

{
  char cVar1;
  short sVar2;
  undefined1 auStack_2ac [520];
  undefined1 *puStack_a4;
  uint uStack_a0;
  undefined1 *puStack_9c;
  undefined1 *puStack_98;
  uint uStack_94;
  undefined4 uStack_90;
  undefined1 auStack_88 [80];
  uint uStack_38;
  int iStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  int iStack_24;
  int iStack_20;
  undefined4 uStack_1c;
  char cStack_15;
  uint uStack_14;
  ushort *puStack_10;
  
  uStack_2c = 0x802;
  uStack_28 = 0x71;
  uStack_1c = 0;
  sVar2 = _FSOpenIterator();
  iStack_30 = (int)sVar2;
  if (iStack_30 == 0) {
    iStack_24 = _NewPtr();
    iStack_20 = _NewPtr();
    if (iStack_24 == 0) {
      iStack_30 = -0x6c;
    }
    else {
      do {
        sVar2 = _FSGetCatalogInfoBulk();
        iStack_30 = (int)sVar2;
        if ((iStack_30 == 0) || (iStack_30 == -0x589)) {
          for (uStack_14 = 0; uStack_14 < uStack_38; uStack_14 = uStack_14 + 1) {
            puStack_10 = (ushort *)(uStack_14 * 0x90 + iStack_24);
            _memcpy(auStack_88,(void *)(uStack_14 * 0x50 + iStack_20),0x50);
            if ((*puStack_10 >> 4 & 1) != 0) {
              puStack_98 = auStack_2ac;
              puStack_9c = &stack0x00000008;
              for (uStack_94 = 0; uStack_94 < 0x200; uStack_94 = uStack_94 + 1) {
                puStack_98[uStack_94] = puStack_9c[uStack_94];
              }
              puStack_a4 = auStack_88;
              for (uStack_a0 = 0; uStack_a0 < 0x50; uStack_a0 = uStack_a0 + 1) {
                (&stack0xfffffd04)[uStack_a0] = puStack_a4[uStack_a0];
              }
              cVar1 = _DoesFileOrFolderExistHSFUni();
              if (cVar1 != '\0') {
                _memcpy(param_1,auStack_88,0x50);
                cStack_15 = '\x01';
                break;
              }
            }
          }
        }
      } while (iStack_30 == 0);
      if (iStack_30 == -0x589) {
        iStack_30 = 0;
      }
      _DisposePtr();
      _DisposePtr();
    }
  }
  _FSCloseIterator();
  if (cStack_15 == '\0') {
    uStack_90 = 0xffffffff;
  }
  else {
    uStack_90 = 0;
  }
  return uStack_90;
}


// ---- _getProcessorSpeed @ 0x935aa (118 bytes)

int _getProcessorSpeed(void)

{
  short sVar1;
  int local_18 [2];
  int local_10;
  
  local_10 = 0;
  sVar1 = _Gestalt(0x70636c6b,local_18);
  if (((sVar1 == 0) && (local_18[0] != 0)) &&
     (local_10 = (int)((float)local_18[0] / 1e+06), local_10 < 0)) {
    local_10 = 0;
  }
  return local_10;
}


// ---- _GetCodeFromWebsite @ 0x93620 (150 bytes)

int _GetCodeFromWebsite(undefined4 param_1,undefined4 param_2)

{
  short sVar1;
  undefined4 *puVar2;
  undefined4 local_14;
  
  puVar2 = (undefined4 *)_NewHandle(0);
  if (puVar2 == (undefined4 *)0x0) {
    sVar1 = _MemError();
    local_14 = (int)sVar1;
  }
  else {
    local_14 = _URLSimpleDownload(param_1,0,puVar2,0,0,0);
  }
  if (local_14 == 0) {
    _HLock(puVar2);
    _StripAllWhiteSpace(*puVar2,param_2);
  }
  _DisposeHandle(puVar2);
  return local_14;
}


