// Ghidra decompilation of functions from original source file:
//   Sources - My Utils/MyUtils.c

// ---- _MyHideCursor @ 0x128a2 (48 bytes)

void _MyHideCursor(void)

{
  _SDL_ShowCursor(0);
  *PTR__gCursorIsVisible_001c436c = 0;
  return;
}


// ---- _MyInitCursor @ 0x128d2 (48 bytes)

void _MyInitCursor(void)

{
  _SDL_ShowCursor(1);
  *PTR__gCursorIsVisible_001c436c = 1;
  return;
}


// ---- _IsCursorVisible @ 0x12902 (29 bytes)

undefined1 _IsCursorVisible(void)

{
  undefined1 uVar1;
  
  uVar1 = _SDL_ShowCursor(0xffffffff);
  return uVar1;
}


// ---- _SetPascalString @ 0x1291f (64 bytes)

void _SetPascalString(int param_1,byte *param_2)

{
  short local_e;
  
  for (local_e = 0; local_e <= (short)(ushort)*param_2; local_e = local_e + 1) {
    *(byte *)(local_e + param_1) = param_2[local_e];
  }
  return;
}


// ---- _GetPascalStringFromCString @ 0x1295f (92 bytes)

void _GetPascalStringFromCString(char *param_1,int param_2)

{
  short local_e;
  
  *param_1 = '\0';
  for (local_e = 0; (*(char *)(local_e + param_2) != '\0' && (local_e < 0xff));
      local_e = local_e + 1) {
    *param_1 = *param_1 + '\x01';
    param_1[local_e + 1] = *(char *)(local_e + param_2);
  }
  return;
}


// ---- _GetHFSUniFromCString @ 0x129bb (138 bytes)

void _GetHFSUniFromCString(undefined1 *param_1,char *param_2)

{
  char cVar1;
  int iVar2;
  char *pcVar3;
  short local_12;
  short local_10;
  short local_e;
  
  iVar2 = -1;
  pcVar3 = param_2;
  do {
    if (iVar2 == 0) break;
    iVar2 = iVar2 + -1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + 1;
  } while (cVar1 != '\0');
  local_e = ~(ushort)iVar2 - 1;
  *param_1 = 0;
  param_1[1] = (char)local_e;
  local_12 = 0;
  local_10 = 2;
  for (; 0 < local_e; local_e = local_e + -1) {
    param_1[local_10] = 0;
    param_1[local_10 + 1] = param_2[local_12];
    local_12 = local_12 + 1;
    local_10 = local_10 + 2;
  }
  return;
}


// ---- _GetCStringFromHFSUniStr @ 0x12a45 (96 bytes)

void _GetCStringFromHFSUniStr(int param_1,int param_2)

{
  undefined2 local_12;
  undefined2 local_10;
  undefined2 local_e;
  
  local_12 = 3;
  local_10 = 0;
  for (local_e = (short)*(char *)(param_2 + 1); 0 < local_e; local_e = local_e + -1) {
    *(undefined1 *)(local_10 + param_1) = *(undefined1 *)(local_12 + param_2);
    local_12 = local_12 + 2;
    local_10 = local_10 + 1;
  }
  *(undefined1 *)(local_10 + param_1) = 0;
  return;
}


// ---- _GetPascalStringFromHFSUniStr @ 0x12aa5 (99 bytes)

void _GetPascalStringFromHFSUniStr(char *param_1,int param_2)

{
  char cVar1;
  short local_12;
  short local_10;
  short local_e;
  
  cVar1 = *(char *)(param_2 + 1);
  *param_1 = cVar1;
  local_12 = 3;
  local_10 = 1;
  for (local_e = (short)cVar1; 0 < local_e; local_e = local_e + -1) {
    param_1[local_10] = *(char *)(local_12 + param_2);
    local_12 = local_12 + 2;
    local_10 = local_10 + 1;
  }
  return;
}


// ---- _RemoveTrailingSpaces @ 0x12b08 (78 bytes)

void _RemoveTrailingSpaces(byte *param_1)

{
  char cVar1;
  ushort local_e;
  
  for (local_e = (ushort)*param_1; 0 < (short)local_e; local_e = local_e - 1) {
    cVar1 = _IsCharWhiteSpace((int)(char)param_1[(short)local_e]);
    if (cVar1 == '\0') break;
  }
  *param_1 = (byte)local_e;
  return;
}


// ---- _IsCharWhiteSpace @ 0x12b56 (67 bytes)

undefined1 _IsCharWhiteSpace(char param_1)

{
  undefined1 local_14;
  
  if ((((param_1 == ' ') || (param_1 == '\t')) || (param_1 == '\n')) || (param_1 == '\r')) {
    local_14 = 1;
  }
  else {
    local_14 = 0;
  }
  return local_14;
}


// ---- _RemoveLeadingSpaces @ 0x12b99 (162 bytes)

void _RemoveLeadingSpaces(byte *param_1)

{
  char cVar1;
  int iVar2;
  byte *pbVar3;
  byte *pbVar4;
  short local_12;
  
  for (local_12 = 1; local_12 <= (short)(ushort)*param_1; local_12 = local_12 + 1) {
    cVar1 = _IsCharWhiteSpace((int)(char)param_1[local_12]);
    if (cVar1 == '\0') break;
  }
  if ((1 < local_12) && (iVar2 = ((uint)*param_1 - (int)local_12) + 1, iVar2 != 0)) {
    pbVar3 = param_1 + local_12;
    pbVar4 = param_1;
    for (; pbVar4 = pbVar4 + 1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *pbVar4 = *pbVar3;
      pbVar3 = pbVar3 + 1;
    }
  }
  *param_1 = (*param_1 - (char)local_12) + 1;
  return;
}


// ---- _RemoveLeadingInvisibleString @ 0x12c3b (172 bytes)

void _RemoveLeadingInvisibleString(byte *param_1)

{
  short sVar1;
  int iVar2;
  byte *pbVar3;
  byte *pbVar4;
  ushort local_12;
  
  for (local_12 = (ushort)*param_1;
      ((0 < (short)local_12 && (param_1[(short)local_12] != 10)) &&
      (param_1[(short)local_12] != 0xd)); local_12 = local_12 - 1) {
  }
  sVar1 = local_12 + 1;
  if ((1 < sVar1) && (iVar2 = ((uint)param_1[sVar1] - (int)sVar1) + 1, iVar2 != 0)) {
    pbVar3 = param_1 + sVar1;
    pbVar4 = param_1;
    for (; pbVar4 = pbVar4 + 1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *pbVar4 = *pbVar3;
      pbVar3 = pbVar3 + 1;
    }
  }
  *param_1 = (*param_1 - (char)sVar1) + 1;
  return;
}


// ---- _StripPrefixFromString @ 0x12ce7 (105 bytes)

void StripPrefixFromString(byte *param_1,short param_2)

{
  byte *pbVar1;
  int iVar2;
  byte *pbVar3;
  
  iVar2 = (uint)*param_1 - (int)param_2;
  if (iVar2 != 0) {
    pbVar1 = param_1 + param_2;
    pbVar3 = param_1;
    while( true ) {
      pbVar3 = pbVar3 + 1;
      pbVar1 = pbVar1 + 1;
      if (iVar2 == 0) break;
      iVar2 = iVar2 + -1;
      *pbVar3 = *pbVar1;
    }
  }
  *param_1 = *param_1 - (char)param_2;
  return;
}


// ---- _StripPrefixFromCString @ 0x12d50 (107 bytes)

void _StripPrefixFromCString(char *param_1,short param_2)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  char *pcVar6;
  
  uVar2 = 0xffffffff;
  pcVar5 = param_1;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    cVar1 = *pcVar5;
    pcVar5 = pcVar5 + 1;
  } while (cVar1 != '\0');
  iVar3 = (~uVar2 - 1) - (int)param_2;
  if (iVar3 != 0) {
    pcVar5 = param_1 + param_2;
    pcVar6 = param_1;
    for (iVar4 = iVar3; iVar4 != 0; iVar4 = iVar4 + -1) {
      *pcVar6 = *pcVar5;
      pcVar5 = pcVar5 + 1;
      pcVar6 = pcVar6 + 1;
    }
  }
  param_1[iVar3] = '\0';
  return;
}


// ---- _StripStringFromStartOfString @ 0x12dbb (152 bytes)

void StripStringFromStartOfString(int param_1,byte *param_2,byte *param_3)

{
  int iVar1;
  int iVar2;
  byte *pbVar3;
  byte *pbVar4;
  uint uStack_20;
  uint uStack_10;
  
  if (*param_2 == 0xff) {
    uStack_20 = 0xff;
  }
  else {
    uStack_20 = (uint)*param_2;
  }
  for (uStack_10 = 1;
      (uStack_10 < uStack_20 && (*(byte *)(uStack_10 + param_1) == param_2[uStack_10]));
      uStack_10 = uStack_10 + 1) {
  }
  iVar1 = (uStack_20 - uStack_10) + 1;
  if (iVar1 != 0) {
    pbVar3 = param_2 + uStack_10;
    pbVar4 = param_3;
    for (iVar2 = iVar1; pbVar4 = pbVar4 + 1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *pbVar4 = *pbVar3;
      pbVar3 = pbVar3 + 1;
    }
    *param_3 = (byte)iVar1;
  }
  return;
}


// ---- _StripStringFromEndOfCString @ 0x12e53 (206 bytes)

void _StripStringFromEndOfCString(char *param_1,char *param_2,char *param_3)

{
  char cVar1;
  uint uVar2;
  uint uVar3;
  char *pcVar4;
  char *local_10;
  
  if ((param_2 == param_3) || (param_3 == (char *)0x0)) {
    param_3 = param_2;
  }
  else {
    _strcpy(param_3,param_2);
  }
  local_10 = _strstr(param_3,param_1);
LAB_00012f15:
  do {
    if (local_10 == (char *)0x0) {
      return;
    }
    uVar2 = 0xffffffff;
    pcVar4 = param_1;
    do {
      if (uVar2 == 0) break;
      uVar2 = uVar2 - 1;
      cVar1 = *pcVar4;
      pcVar4 = pcVar4 + 1;
    } while (cVar1 != '\0');
    uVar3 = 0xffffffff;
    pcVar4 = local_10;
    do {
      if (uVar3 == 0) break;
      uVar3 = uVar3 - 1;
      cVar1 = *pcVar4;
      pcVar4 = pcVar4 + 1;
    } while (cVar1 != '\0');
    if (~uVar2 - 1 == ~uVar3 - 1) {
      *local_10 = '\0';
      goto LAB_00012f15;
    }
    uVar2 = 0xffffffff;
    pcVar4 = param_1;
    do {
      if (uVar2 == 0) break;
      uVar2 = uVar2 - 1;
      cVar1 = *pcVar4;
      pcVar4 = pcVar4 + 1;
    } while (cVar1 != '\0');
    local_10 = _strstr(local_10 + (~uVar2 - 1),param_1);
  } while( true );
}


// ---- _StripStringFromEndOfString @ 0x12f21 (227 bytes)

void _StripStringFromEndOfString(byte *param_1,byte *param_2,byte *param_3)

{
  bool bVar1;
  ushort local_20;
  ushort local_1e;
  ushort local_12;
  ushort local_10;
  
  if (param_2 != param_3) {
    _SWCopyPascalString((char *)param_2,(char *)param_3);
  }
  if (*param_3 == 0xff) {
    local_20 = 0xff;
  }
  else {
    local_20 = (ushort)*param_3;
  }
  if (*param_1 == 0xff) {
    local_1e = 0xff;
  }
  else {
    local_1e = (ushort)*param_1;
  }
  bVar1 = true;
  local_10 = local_1e;
  local_12 = local_20;
  while( true ) {
    if (((short)local_12 < 1) || ((short)local_10 < 1)) goto LAB_00012fe3;
    if (param_1[(short)local_10] != param_3[(short)local_12]) break;
    local_10 = local_10 - 1;
    local_12 = local_12 - 1;
  }
  bVar1 = false;
LAB_00012fe3:
  if (bVar1) {
    *param_3 = *param_3 - (char)local_1e;
  }
  return;
}


// ---- _RemoveNumberFromEndOfString @ 0x13004 (83 bytes)

void _RemoveNumberFromEndOfString(byte *param_1)

{
  byte bVar1;
  bool bVar2;
  
  do {
    bVar1 = *param_1;
    if ((param_1[(short)(ushort)bVar1] < 0x30) || (0x39 < param_1[(short)(ushort)bVar1])) {
      bVar2 = false;
    }
    else {
      *param_1 = bVar1 - 1;
      bVar2 = true;
    }
  } while (bVar2);
  return;
}


// ---- _CompareCStrings @ 0x13057 (102 bytes)

undefined1 _CompareCStrings(int param_1,int param_2)

{
  short local_10;
  
  local_10 = 0;
  while( true ) {
    if (local_10 == 0x7fff) {
      return 0;
    }
    if (*(char *)(local_10 + param_1) == '\0') {
      return 1;
    }
    if (*(char *)(local_10 + param_2) == '\0') {
      return 0;
    }
    if (*(char *)(local_10 + param_1) != *(char *)(local_10 + param_2)) break;
    local_10 = local_10 + 1;
  }
  return 0;
}


// ---- _CompareCStringsForLength @ 0x130bd (81 bytes)

undefined1 CompareCStringsForLength(int param_1,int param_2,short param_3)

{
  undefined2 uStack_10;
  undefined1 uStack_d;
  
  uStack_d = 1;
  for (uStack_10 = 0; uStack_10 < param_3; uStack_10 = uStack_10 + 1) {
    if (*(char *)(uStack_10 + param_1) != *(char *)(uStack_10 + param_2)) {
      uStack_d = 0;
    }
  }
  return uStack_d;
}


// ---- _strstrIgnoreCase @ 0x1310e (213 bytes)

char * _strstrIgnoreCase(char *param_1,char *param_2)

{
  __darwin_ct_rune_t _Var1;
  __darwin_ct_rune_t _Var2;
  char *local_20;
  char *local_14;
  char *local_10;
  
  if (*param_2 == '\0') {
    local_20 = param_1;
  }
  else {
    for (; *param_1 != '\0'; param_1 = param_1 + 1) {
      _Var1 = ___toupper((int)*param_1);
      _Var2 = ___toupper((int)*param_2);
      if (_Var1 == _Var2) {
        local_14 = param_1;
        for (local_10 = param_2; (*local_14 != '\0' && (*local_10 != '\0')); local_10 = local_10 + 1
            ) {
          _Var1 = ___toupper((int)*local_14);
          _Var2 = ___toupper((int)*local_10);
          if (_Var1 != _Var2) break;
          local_14 = local_14 + 1;
        }
        if (*local_10 == '\0') {
          return param_1;
        }
      }
    }
    local_20 = (char *)0x0;
  }
  return local_20;
}


// ---- _StripNDirectoriesFromEndOfPath @ 0x131e3 (56 bytes)

void _StripNDirectoriesFromEndOfPath(undefined4 param_1,short param_2)

{
  undefined2 local_e;
  
  for (local_e = 0; local_e < param_2; local_e = local_e + 1) {
    _StripEndingFromPath(param_1);
  }
  return;
}


// ---- _StripEndingFromPath @ 0x1321b (48 bytes)

void _StripEndingFromPath(char *param_1)

{
  char *pcVar1;
  
  pcVar1 = _strrchr(param_1,0x2f);
  if (pcVar1 != (char *)0x0) {
    *pcVar1 = '\0';
  }
  return;
}


// ---- _GetFilenameFromPath @ 0x1324b (33 bytes)

void GetFilenameFromPath(char *param_1)

{
  _strrchr(param_1,0x2f);
  return;
}


// ---- _MyWriteDataToFile @ 0x1326c (160 bytes)

void _MyWriteDataToFile(int param_1,int param_2,undefined4 *param_3)

{
  undefined4 local_14;
  undefined4 local_10;
  
  local_14 = 0;
  if (0 < param_2) {
    if (param_2 == 2) {
      local_10 = _SDL_WriteBE16(param_1,*(undefined2 *)param_3);
    }
    else if (param_2 == 4) {
      local_10 = _SDL_WriteBE32(param_1,*param_3);
    }
    else {
      local_10 = (**(code **)(param_1 + 8))(param_1,param_3,param_2,1);
    }
    if (local_10 == 0) {
      local_14 = 0xffffffff;
    }
    _SetFileStickyIfError(local_14);
  }
  return;
}


// ---- _MyReadDataFromFile @ 0x1330c (150 bytes)

undefined4 _MyReadDataFromFile(int param_1,int param_2,undefined4 *param_3)

{
  undefined2 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 local_20;
  undefined4 local_14;
  
  local_14 = 0;
  if (param_2 < 1) {
    local_20 = 0;
  }
  else {
    if (param_2 == 2) {
      uVar1 = _SDL_ReadBE16(param_1);
      *(undefined2 *)param_3 = uVar1;
    }
    else if (param_2 == 4) {
      uVar2 = _SDL_ReadBE32(param_1);
      *param_3 = uVar2;
    }
    else {
      iVar3 = (**(code **)(param_1 + 4))(param_1,param_3,param_2,1);
      if (iVar3 == 0) {
        local_14 = 0xffffffff;
      }
    }
    local_20 = local_14;
  }
  return local_20;
}


// ---- _ClearFileStickyError @ 0x133a2 (34 bytes)

void _ClearFileStickyError(void)

{
  *(undefined4 *)PTR__gFileStickyError_001c4368 = 0;
  return;
}


// ---- _GetFileStickyError @ 0x133c4 (30 bytes)

undefined4 _GetFileStickyError(void)

{
  return *(undefined4 *)PTR__gFileStickyError_001c4368;
}


// ---- _SetFileStickyIfError @ 0x133e2 (39 bytes)

void _SetFileStickyIfError(int param_1)

{
  if (param_1 != 0) {
    *(int *)PTR__gFileStickyError_001c4368 = param_1;
  }
  return;
}


// ---- _WasASpecificKeyPressed @ 0x13409 (26 bytes)

undefined4 WasASpecificKeyPressed(void)

{
  return 0;
}


// ---- _IsRectInRect @ 0x13423 (175 bytes)

undefined1 IsRectInRect(short *param_1,short *param_2)

{
  undefined1 uStack_10;
  
  if ((param_1 == (short *)0x0) || (param_2 == (short *)0x0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyUtils.c",
               PTR____FUNCTION___21862_001c4364,0x619);
  }
  if ((((*param_1 < param_2[2]) && (*param_2 < param_1[2])) && (param_1[1] < param_2[3])) &&
     (param_2[1] < param_1[3])) {
    uStack_10 = 1;
  }
  else {
    uStack_10 = 0;
  }
  return uStack_10;
}


// ---- _IsRectFullyInRect @ 0x134d2 (175 bytes)

undefined1 IsRectFullyInRect(short *param_1,short *param_2)

{
  undefined1 uStack_10;
  
  if ((param_1 == (short *)0x0) || (param_2 == (short *)0x0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyUtils.c",
               PTR____FUNCTION___21889_001c4374,0x62a);
  }
  if ((((*param_1 < *param_2) || (param_2[2] < param_1[2])) || (param_1[1] < param_2[1])) ||
     (param_2[3] < param_1[3])) {
    uStack_10 = 0;
  }
  else {
    uStack_10 = 1;
  }
  return uStack_10;
}


// ---- _MoveRect @ 0x13581 (190 bytes)

void MoveRect(short *param_1,short param_2,short param_3)

{
  short sVar1;
  short sVar2;
  
  if (param_1 == (short *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyUtils.c",
               PTR____FUNCTION___21919_001c4370,0x63d);
  }
  sVar1 = *param_1;
  sVar2 = param_1[1];
  param_1[1] = param_2;
  *param_1 = param_3;
  param_1[3] = param_2 + (param_1[3] - sVar2);
  param_1[2] = param_3 + (param_1[2] - sVar1);
  return;
}


// ---- _SwapVariables @ 0x1363f (46 bytes)

void _SwapVariables(undefined2 *param_1,undefined2 *param_2)

{
  undefined2 uVar1;
  
  uVar1 = *param_1;
  *param_1 = *param_2;
  *param_2 = uVar1;
  return;
}


// ---- _sign @ 0x1366d (47 bytes)

undefined4 _sign(short param_1)

{
  undefined4 local_14;
  
  if (param_1 < 0) {
    local_14 = 0xffffffff;
  }
  else {
    local_14 = 1;
  }
  return local_14;
}


// ---- _mAbs @ 0x1369c (41 bytes)

int _mAbs(int param_1)

{
  undefined4 local_10;
  
  if (param_1 < 0) {
    local_10 = -param_1;
  }
  else {
    local_10 = param_1;
  }
  return local_10;
}


// ---- _IsNumberEven @ 0x136c5 (34 bytes)

byte IsNumberEven(byte param_1)

{
  return (param_1 ^ 1) & 1;
}


// ---- _IsNumberOdd @ 0x136e7 (31 bytes)

byte IsNumberOdd(byte param_1)

{
  return param_1 & 1;
}


// ---- _GetAngleFromXandY @ 0x13706 (269 bytes)

longdouble
_GetAngleFromXandY(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  longdouble lVar1;
  undefined8 local_14;
  
  lVar1 = (longdouble)_MyATan((double)CONCAT44(param_4,param_3) / (double)CONCAT44(param_2,param_1))
  ;
  local_14 = (double)lVar1;
  if ((double)CONCAT44(param_2,param_1) <= 0.0 && (double)CONCAT44(param_2,param_1) != 0.0) {
    local_14 = local_14 + 180.0;
  }
  else if ((double)CONCAT44(param_4,param_3) <= 0.0 && (double)CONCAT44(param_4,param_3) != 0.0) {
    local_14 = local_14 + 360.0;
  }
  for (; local_14 < 0.0; local_14 = local_14 + 360.0) {
  }
  for (; 360.0 < local_14; local_14 = local_14 - 360.0) {
  }
  return (longdouble)local_14;
}


// ---- _GetXFromAngleAndVelocity @ 0x13813 (74 bytes)

longdouble
_GetXFromAngleAndVelocity
          (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  longdouble lVar1;
  
  lVar1 = (longdouble)_MyCos(CONCAT44(param_2,param_1));
  return (longdouble)((double)lVar1 * (double)CONCAT44(param_4,param_3));
}


// ---- _GetYFromAngleAndVelocity @ 0x1385d (74 bytes)

longdouble
_GetYFromAngleAndVelocity
          (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  longdouble lVar1;
  
  lVar1 = (longdouble)_MySin(CONCAT44(param_2,param_1));
  return (longdouble)((double)lVar1 * (double)CONCAT44(param_4,param_3));
}


// ---- _MyCos @ 0x138a7 (80 bytes)

void _MyCos(undefined4 param_1,undefined4 param_2)

{
  _cos((360.0 - (double)CONCAT44(param_2,param_1)) * 0.017453292522222223);
  return;
}


// ---- _MySin @ 0x138f7 (80 bytes)

void _MySin(undefined4 param_1,undefined4 param_2)

{
  _sin((360.0 - (double)CONCAT44(param_2,param_1)) * 0.017453292522222223);
  return;
}


// ---- _MyATan @ 0x13947 (96 bytes)

longdouble _MyATan(undefined4 param_1,undefined4 param_2)

{
  longdouble lVar1;
  
  lVar1 = (longdouble)_atan(CONCAT44(param_2,param_1));
  return (longdouble)((double)lVar1 * -57.29577950560105 + 360.0);
}


// ---- _AddBooleanToByte @ 0x139a7 (50 bytes)

void AddBooleanToByte(char param_1,byte *param_2,byte param_3)

{
  *param_2 = *param_2 | param_1 << (param_3 & 0x1f);
  return;
}


// ---- _ReadBooleanFromByte @ 0x139d9 (43 bytes)

uint ReadBooleanFromByte(char param_1,byte param_2)

{
  return (int)param_1 >> (param_2 & 0x1f) & 1;
}


// ---- _GetCustomRandom @ 0x13a04 (114 bytes)

int _GetCustomRandom(int param_1,int param_2)

{
  int iVar1;
  longdouble lVar2;
  
  lVar2 = (longdouble)_CustomRandom();
  iVar1 = (int)((float)((param_2 - param_1) + 1) * (float)lVar2) + param_1;
  _printf("Random int: %d (between min/max of %d, %d)\n",iVar1,param_1,param_2);
  return iVar1;
}


// ---- _GetCustomRandomFloat @ 0x13a76 (86 bytes)

longdouble _GetCustomRandomFloat(float param_1,float param_2)

{
  longdouble lVar1;
  
  lVar1 = (longdouble)_CustomRandom();
  return (longdouble)((param_2 - param_1) * (float)lVar1 + param_1);
}


// ---- _StartTicksTimer @ 0x13acc (14 bytes)

void StartTicksTimer(void)

{
  return;
}


// ---- _WaitTicks @ 0x13ada (14 bytes)

void WaitTicks(void)

{
  return;
}


// ---- _WaitTicksOrMouseClick @ 0x13ae8 (14 bytes)

void WaitTicksOrMouseClick(void)

{
  return;
}


// ---- _MyDelay @ 0x13af6 (14 bytes)

void _MyDelay(void)

{
  return;
}


// ---- _MyWaitForKeyDelay @ 0x13b04 (14 bytes)

void MyWaitForKeyDelay(void)

{
  return;
}


// ---- _StartMicrosecTimer @ 0x13b12 (42 bytes)

void _StartMicrosecTimer(void)

{
  undefined4 uVar1;
  
  uVar1 = _SDL_GetTicks();
  *(undefined4 *)PTR__gLastMilliseconds_001c4378 = uVar1;
  return;
}


// ---- _WaitMicroseconds @ 0x13b3c (94 bytes)

void WaitMicroseconds(uint param_1)

{
  int iVar1;
  
  do {
    iVar1 = _SDL_GetTicks();
    if ((uint)(iVar1 - *(int *)PTR__gLastMilliseconds_001c4378) < param_1 - 0x14) {
      _SDL_Delay(1);
    }
  } while ((uint)(iVar1 - *(int *)PTR__gLastMilliseconds_001c4378) < param_1);
  return;
}


// ---- _GetElapsedSeconds @ 0x13b9a (38 bytes)

uint _GetElapsedSeconds(void)

{
  uint uVar1;
  
  uVar1 = _GetElapsedMillisecs();
  return uVar1 / 1000;
}


// ---- _GetElapsedMillisecs @ 0x13bc0 (48 bytes)

int _GetElapsedMillisecs(void)

{
  int iVar1;
  
  iVar1 = _SDL_GetTicks();
  return iVar1 - *(int *)PTR__gLastMilliseconds_001c4378;
}


// ---- _WaitMicrosecondsOrKeyPress @ 0x13bf0 (100 bytes)

void WaitMicrosecondsOrKeyPress(uint param_1)

{
  char cVar1;
  int iVar2;
  
  do {
    iVar2 = _SDL_GetTicks();
    if ((uint)(iVar2 - *(int *)PTR__gLastMilliseconds_001c4378) < param_1 - 0x14) {
      cVar1 = _WasAKeyPressed();
      if (cVar1 != '\0') {
        return;
      }
      cVar1 = _WasGameQuit();
      if (cVar1 != '\0') {
        return;
      }
    }
    if (param_1 <= (uint)(iVar2 - *(int *)PTR__gLastMilliseconds_001c4378)) {
      return;
    }
  } while( true );
}


// ---- _WaitMicrosecondsOrMouseClick @ 0x13c54 (109 bytes)

void _WaitMicrosecondsOrMouseClick(uint param_1)

{
  char cVar1;
  int iVar2;
  
  do {
    iVar2 = _SDL_GetTicks();
    if ((uint)(iVar2 - *(int *)PTR__gLastMilliseconds_001c4378) < param_1 - 0x14) {
      cVar1 = _WasMouseClicked();
      if (cVar1 != '\0') {
        return;
      }
      cVar1 = _WasAKeyPressed();
      if (cVar1 != '\0') {
        return;
      }
      cVar1 = _WasGameQuit();
      if (cVar1 != '\0') {
        return;
      }
    }
    if (param_1 <= (uint)(iVar2 - *(int *)PTR__gLastMilliseconds_001c4378)) {
      return;
    }
  } while( true );
}


// ---- _OffsetRectForFrame @ 0x13cc1 (104 bytes)

void OffsetRectForFrame(short *param_1,int param_2)

{
  *param_1 = *param_1 + *(short *)(param_2 + 0x18);
  param_1[2] = param_1[2] + *(short *)(param_2 + 0x18);
  param_1[1] = param_1[1] + *(short *)(param_2 + 0x1a);
  param_1[3] = param_1[3] + *(short *)(param_2 + 0x1a);
  return;
}


