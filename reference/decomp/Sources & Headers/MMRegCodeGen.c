// Ghidra decompilation of functions from original source file:
//   Sources & Headers/MMRegCodeGen.c

// ---- _DrawSpriteWorldInDialog @ 0x9cc83 (370 bytes)

bool DrawSpriteWorldInDialog(undefined4 param_1,int param_2)

{
  short sVar1;
  short sStack_14;
  short sStack_12;
  short sStack_10;
  undefined1 uStack_d;
  
  sVar1 = _GetStringLength(param_1);
  if (4 < sVar1) {
    _CopyCString(param_1,param_2);
    _ConvertToUpperCase(param_2);
    _RemoveBadChars(param_2);
    _ConvertToLetters(param_2,0);
    _DoLetterConversion(param_2);
    _LengthenCode(param_2);
    _ShortenCode(param_2);
    _SwapLetters(param_2);
    _DoStageOne(param_2);
    _SwapLetters(param_2);
    _ConvertConfusingLetters(param_2);
    _DoStageTwo(param_2);
    _ConvertConfusingLetters(param_2);
    _DoStageThree(param_2);
    _ParsePixelsRLE16Bit(&sStack_10,&sStack_12,&sStack_14);
    _EmbedDate(param_2,(int)sStack_10,(int)sStack_12,(int)sStack_14);
    _DoFinalLetterConversion(param_2);
    _ConvertConfusingLettersToNumbers(param_2);
    uStack_d = _GetChecksumForDate((int)sStack_10,(int)sStack_12);
    *(undefined1 *)(param_2 + 0x10) = uStack_d;
    *(undefined1 *)(param_2 + 0x11) = 0;
    _DoFinalLetterSwap(param_2);
  }
  return 4 < sVar1;
}


// ---- _GetPrefixFromLicenseCode @ 0x9cdf5 (145 bytes)

void _GetPrefixFromLicenseCode(undefined4 param_1,undefined1 *param_2)

{
  char *pcVar1;
  char local_10f [255];
  char *local_10;
  
  _CopyCString(param_1,local_10f);
  _stringToUppercase(local_10f);
  pcVar1 = _strchr(local_10f,0x2d);
  if (pcVar1 == (char *)0x0) {
    *param_2 = 0;
  }
  else {
    local_10 = _strtok(local_10f,"-");
    _CopyCString(local_10,param_2);
  }
  return;
}


// ---- _ValidateRegistrationCodeWithPrefix @ 0x9ce86 (635 bytes)

undefined1
_ValidateRegistrationCodeWithPrefix
          (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
          undefined4 param_5)

{
  char cVar1;
  undefined1 uVar2;
  char *pcVar3;
  uint uVar4;
  int iVar5;
  char *pcVar6;
  char *pcVar7;
  char *pcVar8;
  bool bVar9;
  char local_222 [255];
  undefined1 local_123 [255];
  char *local_24;
  short local_1e;
  
  _CopyCString(param_2,local_222);
  _CopyCString(param_2,local_123);
  _stringToUppercase(local_222);
  pcVar3 = _strchr(local_222,0x2d);
  if (pcVar3 != (char *)0x0) {
    local_24 = _strtok(local_222,"-");
    bVar9 = local_24 == (char *)0x0;
    if (bVar9) {
      local_1e = 0;
    }
    else {
      uVar4 = 0xffffffff;
      pcVar3 = local_24;
      do {
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        cVar1 = *pcVar3;
        pcVar3 = pcVar3 + 1;
      } while (cVar1 != '\0');
      bVar9 = ~uVar4 == 0;
      local_1e = (short)~uVar4;
    }
    iVar5 = 4;
    pcVar3 = local_24;
    pcVar7 = "EP1";
    do {
      pcVar6 = pcVar3;
      pcVar8 = pcVar7;
      if (iVar5 == 0) break;
      iVar5 = iVar5 + -1;
      pcVar8 = pcVar7 + 1;
      pcVar6 = pcVar3 + 1;
      bVar9 = *pcVar3 == *pcVar7;
      pcVar3 = pcVar6;
      pcVar7 = pcVar8;
    } while (bVar9);
    iVar5 = 0;
    if (!bVar9) {
      iVar5 = (uint)(byte)pcVar6[-1] - (uint)(byte)pcVar8[-1];
    }
    bVar9 = iVar5 == 0;
    if (bVar9) {
      _SetRegCodeConversionArray(PTR__regCodeConversionArrayEP1_001c52e8);
    }
    else {
      iVar5 = 4;
      pcVar3 = local_24;
      pcVar7 = "EP2";
      do {
        pcVar6 = pcVar3;
        pcVar8 = pcVar7;
        if (iVar5 == 0) break;
        iVar5 = iVar5 + -1;
        pcVar8 = pcVar7 + 1;
        pcVar6 = pcVar3 + 1;
        bVar9 = *pcVar3 == *pcVar7;
        pcVar3 = pcVar6;
        pcVar7 = pcVar8;
      } while (bVar9);
      iVar5 = 0;
      if (!bVar9) {
        iVar5 = (uint)(byte)pcVar6[-1] - (uint)(byte)pcVar8[-1];
      }
      bVar9 = iVar5 == 0;
      if (bVar9) {
        _SetRegCodeConversionArray(PTR__regCodeConversionArrayEP2_001c52f8);
      }
      else {
        iVar5 = 4;
        pcVar3 = local_24;
        pcVar7 = "MM1";
        do {
          pcVar6 = pcVar3;
          pcVar8 = pcVar7;
          if (iVar5 == 0) break;
          iVar5 = iVar5 + -1;
          pcVar8 = pcVar7 + 1;
          pcVar6 = pcVar3 + 1;
          bVar9 = *pcVar3 == *pcVar7;
          pcVar3 = pcVar6;
          pcVar7 = pcVar8;
        } while (bVar9);
        iVar5 = 0;
        if (!bVar9) {
          iVar5 = (uint)(byte)pcVar6[-1] - (uint)(byte)pcVar8[-1];
        }
        bVar9 = iVar5 == 0;
        if (bVar9) {
          _SetRegCodeConversionArray(PTR__regCodeConversionArrayMM1_001c52ec);
        }
        else {
          iVar5 = 4;
          pcVar3 = local_24;
          pcVar7 = "MM2";
          do {
            pcVar6 = pcVar3;
            pcVar8 = pcVar7;
            if (iVar5 == 0) break;
            iVar5 = iVar5 + -1;
            pcVar8 = pcVar7 + 1;
            pcVar6 = pcVar3 + 1;
            bVar9 = *pcVar3 == *pcVar7;
            pcVar3 = pcVar6;
            pcVar7 = pcVar8;
          } while (bVar9);
          iVar5 = 0;
          if (!bVar9) {
            iVar5 = (uint)(byte)pcVar6[-1] - (uint)(byte)pcVar8[-1];
          }
          if (iVar5 == 0) {
            _SetRegCodeConversionArray(PTR__regCodeConversionArrayMM2_001c52f0);
          }
        }
      }
    }
    _StripPrefixFromCString(local_123,(int)local_1e);
  }
  uVar2 = _DisposeDialogUPPProc(param_1,local_123,param_3,param_4,param_5);
  return uVar2;
}


// ---- _DisposeDialogUPPProc @ 0x9d101 (518 bytes)

undefined4
_DisposeDialogUPPProc
          (undefined4 param_1,undefined4 param_2,short *param_3,short *param_4,undefined4 param_5)

{
  char cVar1;
  short sVar2;
  undefined4 local_210;
  undefined1 local_20b [16];
  undefined1 local_1fb;
  undefined1 local_10c [255];
  undefined1 local_d;
  
  sVar2 = _GetStringLength(param_1);
  if (sVar2 < 5) {
    local_210 = 0;
  }
  else {
    _CopyCString(param_1,local_10c);
    _ConvertToUpperCase(local_10c);
    _RemoveBadChars(local_10c);
    _ConvertToLetters(local_10c,0);
    _DoLetterConversion(local_10c);
    _LengthenCode(local_10c);
    _ShortenCode(local_10c);
    _SwapLetters(local_10c);
    _DoStageOne(local_10c);
    _SwapLetters(local_10c);
    _ConvertConfusingLetters(local_10c);
    cVar1 = _CompareCStringsByLength(param_2,local_10c,4);
    if (cVar1 == '\0') {
      local_210 = 0;
    }
    else {
      _DoStageTwo(local_10c);
      _ConvertConfusingLetters(local_10c);
      cVar1 = _CompareCStringsByLength(param_2,local_10c,8);
      if (cVar1 == '\0') {
        local_210 = 0;
      }
      else {
        _DoStageThree(local_10c);
        _CopyCString(param_2,local_20b);
        _UndoFinalLetterSwap(local_20b);
        local_d = local_1fb;
        _ConvertNumbersBackToLetters(local_20b);
        _UndoFinalLetterConversion(local_20b);
        _ExtractDate(local_10c,local_20b,param_3,param_4,param_5);
        _GetChecksumForDate((int)*param_3,(int)*param_4);
        local_210 = 1;
      }
    }
  }
  return local_210;
}


// ---- _ParsePixelsRLE16Bit @ 0x9d307 (90 bytes)

void _ParsePixelsRLE16Bit(undefined2 *param_1,short *param_2,short *param_3)

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


// ---- _ConvertToUpperCase @ 0x9d361 (101 bytes)

void _ConvertToUpperCase(int param_1)

{
  undefined2 local_10;
  
  for (local_10 = 0; *(char *)(local_10 + param_1) != '\0'; local_10 = local_10 + 1) {
    if (('`' < *(char *)(local_10 + param_1)) && (*(char *)(local_10 + param_1) < '{')) {
      *(char *)(local_10 + param_1) = *(char *)(local_10 + param_1) + -0x20;
    }
  }
  return;
}


// ---- _ConvertToLetters @ 0x9d3c6 (136 bytes)

void _ConvertToLetters(int param_1,char param_2)

{
  short local_12;
  short local_10;
  
  for (local_12 = 0;
      (local_12 < 0x10 && ((param_2 != '\0' || (*(char *)(local_12 + param_1) != '\0'))));
      local_12 = local_12 + 1) {
    for (local_10 = (short)*(char *)(local_12 + param_1); 0x5a < local_10;
        local_10 = local_10 + -0x19) {
    }
    for (; local_10 < 0x41; local_10 = local_10 + 0x19) {
    }
    *(char *)(local_12 + param_1) = (char)local_10;
  }
  return;
}


// ---- _RemoveBadChars @ 0x9d44e (127 bytes)

void _RemoveBadChars(int param_1)

{
  char cVar1;
  short sVar2;
  undefined2 local_12;
  undefined2 local_10;
  
  sVar2 = _GetStringLength(param_1);
  local_10 = 0;
  for (local_12 = 0; local_12 < sVar2; local_12 = local_12 + 1) {
    cVar1 = *(char *)(local_12 + param_1);
    if ((('@' < cVar1) && (cVar1 < '[')) || (('`' < cVar1 && (cVar1 < '{')))) {
      *(char *)(local_10 + param_1) = cVar1;
      local_10 = local_10 + 1;
    }
  }
  *(undefined1 *)(local_10 + param_1) = 0;
  return;
}


// ---- _ConvertConfusingLetters @ 0x9d4cd (100 bytes)

void _ConvertConfusingLetters(int param_1)

{
  undefined1 local_f;
  undefined2 local_e;
  
  for (local_e = 0; *(char *)(local_e + param_1) != '\0'; local_e = local_e + 1) {
    local_f = *(char *)(local_e + param_1);
    if (local_f == 'I') {
      local_f = 'H';
    }
    if (local_f == 'L') {
      local_f = 'K';
    }
    if (local_f == 'O') {
      local_f = 'M';
    }
    *(char *)(local_e + param_1) = local_f;
  }
  return;
}


// ---- _ConvertConfusingLettersToNumbers @ 0x9d531 (100 bytes)

void _ConvertConfusingLettersToNumbers(int param_1)

{
  undefined1 local_f;
  undefined2 local_e;
  
  for (local_e = 0; *(char *)(local_e + param_1) != '\0'; local_e = local_e + 1) {
    local_f = *(char *)(local_e + param_1);
    if (local_f == 'I') {
      local_f = '5';
    }
    if (local_f == 'L') {
      local_f = '7';
    }
    if (local_f == 'O') {
      local_f = '9';
    }
    *(char *)(local_e + param_1) = local_f;
  }
  return;
}


// ---- _ConvertNumbersBackToLetters @ 0x9d595 (100 bytes)

void _ConvertNumbersBackToLetters(int param_1)

{
  undefined1 local_f;
  undefined2 local_e;
  
  for (local_e = 0; *(char *)(local_e + param_1) != '\0'; local_e = local_e + 1) {
    local_f = *(char *)(local_e + param_1);
    if (local_f == '5') {
      local_f = 'I';
    }
    if (local_f == '7') {
      local_f = 'L';
    }
    if (local_f == '9') {
      local_f = 'O';
    }
    *(char *)(local_e + param_1) = local_f;
  }
  return;
}


// ---- _SetRegCodeConversionArray @ 0x9d5f9 (33 bytes)

void _SetRegCodeConversionArray(undefined4 param_1)

{
  *(undefined4 *)PTR__conversionArray_001c52e4 = param_1;
  return;
}


// ---- _DoLetterConversion @ 0x9d61a (137 bytes)

void _DoLetterConversion(int param_1)

{
  undefined2 local_12;
  undefined2 local_10;
  
  for (local_12 = 0; *(char *)(local_12 + param_1) != '\0'; local_12 = local_12 + 1) {
    local_10 = *(char *)(local_12 + param_1) + -0x41;
    if ((-1 < local_10) && (local_10 < 0x1a)) {
      local_10 = (short)*(char *)((int)local_10 + *(int *)PTR__conversionArray_001c52e4);
    }
    *(char *)(local_12 + param_1) = (char)local_10 + 'A';
  }
  return;
}


// ---- _DoFinalLetterConversion @ 0x9d6a3 (137 bytes)

void _DoFinalLetterConversion(int param_1)

{
  undefined2 local_12;
  undefined2 local_10;
  
  for (local_12 = 8; *(char *)(local_12 + param_1) != '\0'; local_12 = local_12 + 1) {
    local_10 = *(char *)(local_12 + param_1) + -0x41;
    if ((-1 < local_10) && (local_10 < 0x1a)) {
      local_10 = (short)*(char *)((int)local_10 + *(int *)PTR__conversionArray_001c52e4);
    }
    *(char *)(local_12 + param_1) = (char)local_10 + 'A';
  }
  return;
}


// ---- _UndoFinalLetterConversion @ 0x9d72c (136 bytes)

void _UndoFinalLetterConversion(int param_1)

{
  char cVar1;
  undefined2 local_14;
  undefined2 local_12;
  
  for (local_14 = 8; *(char *)(local_14 + param_1) != '\0'; local_14 = local_14 + 1) {
    local_12 = *(char *)(local_14 + param_1) + -0x41;
    if ((-1 < local_12) && (local_12 < 0x1a)) {
      cVar1 = _FindValueInArray((int)(char)local_12);
      if (cVar1 != -1) {
        local_12 = (short)cVar1;
      }
    }
    *(char *)(local_14 + param_1) = (char)local_12 + 'A';
  }
  return;
}


// ---- _FindValueInArray @ 0x9d7b4 (115 bytes)

int _FindValueInArray(char param_1)

{
  short local_e;
  
  local_e = 0;
  while( true ) {
    if (*(char *)((int)local_e + *(int *)PTR__conversionArray_001c52e4) == -1) {
      return -1;
    }
    if (*(char *)((int)local_e + *(int *)PTR__conversionArray_001c52e4) == param_1) break;
    local_e = local_e + 1;
  }
  return (int)(char)local_e;
}


// ---- _LengthenCode @ 0x9d827 (574 bytes)

void _LengthenCode(char *param_1)

{
  short local_e;
  
  local_e = _GetStringLength(param_1);
  if (local_e == 5) {
    param_1[5] = *param_1 + param_1[2] + param_1[4];
    local_e = 6;
  }
  if (local_e == 6) {
    param_1[6] = param_1[2] + param_1[5];
    local_e = 7;
  }
  if (local_e == 7) {
    param_1[7] = param_1[1] + param_1[6];
    local_e = 8;
  }
  if (local_e == 8) {
    param_1[8] = *param_1 + param_1[3];
    local_e = 9;
  }
  if (local_e == 9) {
    param_1[9] = param_1[3] - param_1[4];
    local_e = 10;
  }
  if (local_e == 10) {
    param_1[10] = param_1[7] - param_1[1];
    local_e = 0xb;
  }
  if (local_e == 0xb) {
    param_1[0xb] = (param_1[9] + param_1[10]) - param_1[2];
    local_e = 0xc;
  }
  if (local_e == 0xc) {
    param_1[0xc] = (param_1[3] + param_1[2]) - param_1[10];
    local_e = 0xd;
  }
  if (local_e == 0xd) {
    param_1[0xd] = (param_1[1] + param_1[2]) - param_1[3];
    local_e = 0xe;
  }
  if (local_e == 0xe) {
    param_1[0xe] = (param_1[4] + param_1[7]) - param_1[6];
    local_e = 0xf;
  }
  if (local_e == 0xf) {
    param_1[0xf] = (param_1[0xd] + param_1[10]) - param_1[9];
    local_e = 0x10;
  }
  param_1[local_e] = '\0';
  _ConvertToLetters(param_1,1);
  return;
}


// ---- _ShortenCode @ 0x9da65 (173 bytes)

void _ShortenCode(int param_1)

{
  undefined2 local_10;
  undefined2 local_e;
  
  local_e = 5;
  local_10 = _GetStringLength(param_1);
  while (0x10 < (short)local_10) {
    if ((local_10 & 1) == 0) {
      *(char *)(local_e + param_1) =
           *(char *)(local_e + param_1) - *(char *)((short)local_10 + param_1);
    }
    else {
      *(char *)(local_e + param_1) =
           *(char *)(local_e + param_1) + *(char *)((short)local_10 + param_1);
    }
    local_10 = local_10 - 1;
    local_e = local_e + 1;
    if (0xf < local_e) {
      local_e = 0;
    }
  }
  *(undefined1 *)((short)local_10 + param_1) = 0;
  _ConvertToLetters(param_1,1);
  return;
}


// ---- _SwapLetters @ 0x9db12 (130 bytes)

void _SwapLetters(int param_1)

{
  char cVar1;
  undefined1 uVar2;
  short local_12;
  
  for (local_12 = 0; (local_12 < 0x10 && (PTR__swapArray_001c52d8[local_12] != -1));
      local_12 = local_12 + 1) {
    cVar1 = PTR__swapArray_001c52d8[local_12];
    uVar2 = *(undefined1 *)(local_12 + param_1);
    *(undefined1 *)(local_12 + param_1) = *(undefined1 *)((short)cVar1 + param_1);
    *(undefined1 *)((short)cVar1 + param_1) = uVar2;
  }
  return;
}


// ---- _DoFinalLetterSwap @ 0x9db94 (136 bytes)

void _DoFinalLetterSwap(int param_1)

{
  char cVar1;
  undefined1 uVar2;
  short local_12;
  
  local_12 = 8;
  while ((local_12 < 0x11 && (PTR__finalSwapArray_001c52dc[local_12 + -8] != -1))) {
    cVar1 = PTR__finalSwapArray_001c52dc[local_12 + -8];
    uVar2 = *(undefined1 *)(local_12 + param_1);
    *(undefined1 *)(local_12 + param_1) = *(undefined1 *)((short)cVar1 + param_1);
    *(undefined1 *)((short)cVar1 + param_1) = uVar2;
    local_12 = local_12 + 1;
  }
  return;
}


// ---- _UndoFinalLetterSwap @ 0x9dc1c (157 bytes)

void _UndoFinalLetterSwap(int param_1)

{
  char cVar1;
  undefined1 uVar2;
  short local_12;
  
  local_12 = 8;
  while ((local_12 < 0x11 && (PTR__finalSwapArray_001c52dc[local_12 + -8] != -1))) {
    local_12 = local_12 + 1;
  }
  while (local_12 = local_12 + -1, 7 < local_12) {
    cVar1 = PTR__finalSwapArray_001c52dc[local_12 + -8];
    uVar2 = *(undefined1 *)(local_12 + param_1);
    *(undefined1 *)(local_12 + param_1) = *(undefined1 *)((short)cVar1 + param_1);
    *(undefined1 *)((short)cVar1 + param_1) = uVar2;
  }
  return;
}


// ---- _DoStageOne @ 0x9dcb9 (513 bytes)

void _DoStageOne(int param_1)

{
  short sVar1;
  short local_10;
  short local_e;
  
  local_10 = 0;
  while ((local_10 < 0x10 &&
         ((((PTR__stageOneArray_001c52f4[local_10 * 4] != '\0' ||
            (PTR__stageOneArray_001c52f4[local_10 * 4 + 1] != '\0')) ||
           (PTR__stageOneArray_001c52f4[local_10 * 4 + 2] != '\0')) ||
          (PTR__stageOneArray_001c52f4[local_10 * 4 + 3] != '\0'))))) {
    sVar1 = (short)*(char *)(local_10 + param_1);
    if (PTR__stageOneArray_001c52f4[local_10 * 4 + 2] == '\0') {
      local_e = (sVar1 + *(char *)((char)PTR__stageOneArray_001c52f4[local_10 * 4] + param_1)) -
                (short)*(char *)((char)PTR__stageOneArray_001c52f4[local_10 * 4 + 1] + param_1);
    }
    else if (PTR__stageOneArray_001c52f4[local_10 * 4 + 2] == '\x01') {
      local_e = (sVar1 - *(char *)((char)PTR__stageOneArray_001c52f4[local_10 * 4] + param_1)) +
                (short)*(char *)((char)PTR__stageOneArray_001c52f4[local_10 * 4 + 1] + param_1);
    }
    else {
      local_e = (sVar1 + (short)*(char *)((char)PTR__stageOneArray_001c52f4[local_10 * 4] + param_1)
                         * (short)(char)PTR__stageOneArray_001c52f4[local_10 * 4 + 3]) -
                (short)*(char *)((char)PTR__stageOneArray_001c52f4[local_10 * 4 + 1] + param_1);
    }
    for (; 0xff < local_e; local_e = local_e + -0xff) {
    }
    for (; local_e < 0; local_e = local_e + 0xff) {
    }
    *(char *)(local_10 + param_1) = (char)local_e;
    local_10 = local_10 + 1;
  }
  _ConvertToLetters(param_1,1);
  return;
}


// ---- _DoStageTwo @ 0x9deba (548 bytes)

void _DoStageTwo(int param_1)

{
  short sVar1;
  short sVar2;
  short local_12;
  short local_10;
  
  local_12 = 4;
  while ((local_12 < 0x10 &&
         ((((sVar1 = local_12 + -4, PTR__stageTwoArray_001c52e0[sVar1 * 4] != '\0' ||
            (PTR__stageTwoArray_001c52e0[sVar1 * 4 + 1] != '\0')) ||
           (PTR__stageTwoArray_001c52e0[sVar1 * 4 + 2] != '\0')) ||
          (PTR__stageTwoArray_001c52e0[sVar1 * 4 + 3] != '\0'))))) {
    sVar2 = (short)*(char *)(local_12 + param_1);
    if (PTR__stageTwoArray_001c52e0[sVar1 * 4 + 2] == '\0') {
      local_10 = (sVar2 + *(char *)((char)PTR__stageTwoArray_001c52e0[sVar1 * 4] + param_1) * 2) -
                 (*(char *)((char)PTR__stageTwoArray_001c52e0[sVar1 * 4 + 1] + param_1) * 2 +
                 (short)*(char *)((char)PTR__stageTwoArray_001c52e0[sVar1 * 4 + 1] + param_1));
    }
    else if (PTR__stageTwoArray_001c52e0[sVar1 * 4 + 2] == '\x01') {
      local_10 = (sVar2 - (*(char *)((char)PTR__stageTwoArray_001c52e0[sVar1 * 4] + param_1) * 2 +
                          (short)*(char *)((char)PTR__stageTwoArray_001c52e0[sVar1 * 4] + param_1)))
                 + *(char *)((char)PTR__stageTwoArray_001c52e0[sVar1 * 4 + 1] + param_1) * 4;
    }
    else {
      local_10 = sVar2 + (short)*(char *)((char)PTR__stageTwoArray_001c52e0[sVar1 * 4] + param_1) *
                         (short)(char)PTR__stageTwoArray_001c52e0[sVar1 * 4 + 3] +
                 *(char *)((char)PTR__stageTwoArray_001c52e0[sVar1 * 4 + 1] + param_1) * -2;
    }
    for (; 0xff < local_10; local_10 = local_10 + -0xff) {
    }
    for (; local_10 < 0; local_10 = local_10 + 0xff) {
    }
    *(char *)(local_12 + param_1) = (char)local_10;
    local_12 = local_12 + 1;
  }
  _ConvertToLetters(param_1,1);
  return;
}


// ---- _DoStageThree @ 0x9e0de (548 bytes)

void _DoStageThree(int param_1)

{
  short sVar1;
  short sVar2;
  short local_12;
  short local_10;
  
  local_12 = 8;
  while ((local_12 < 0x10 &&
         ((((sVar1 = local_12 + -8, PTR__stageThreeArray_001c52d4[sVar1 * 4] != '\0' ||
            (PTR__stageThreeArray_001c52d4[sVar1 * 4 + 1] != '\0')) ||
           (PTR__stageThreeArray_001c52d4[sVar1 * 4 + 2] != '\0')) ||
          (PTR__stageThreeArray_001c52d4[sVar1 * 4 + 3] != '\0'))))) {
    sVar2 = (short)*(char *)(local_12 + param_1);
    if (PTR__stageThreeArray_001c52d4[sVar1 * 4 + 2] == '\0') {
      local_10 = (sVar2 + *(char *)((char)PTR__stageThreeArray_001c52d4[sVar1 * 4] + param_1) * 2) -
                 (*(char *)((char)PTR__stageThreeArray_001c52d4[sVar1 * 4 + 1] + param_1) * 2 +
                 (short)*(char *)((char)PTR__stageThreeArray_001c52d4[sVar1 * 4 + 1] + param_1));
    }
    else if (PTR__stageThreeArray_001c52d4[sVar1 * 4 + 2] == '\x01') {
      local_10 = (sVar2 - (*(char *)((char)PTR__stageThreeArray_001c52d4[sVar1 * 4] + param_1) * 2 +
                          (short)*(char *)((char)PTR__stageThreeArray_001c52d4[sVar1 * 4] + param_1)
                          )) +
                 *(char *)((char)PTR__stageThreeArray_001c52d4[sVar1 * 4 + 1] + param_1) * 4;
    }
    else {
      local_10 = sVar2 + (short)*(char *)((char)PTR__stageThreeArray_001c52d4[sVar1 * 4] + param_1)
                         * (short)(char)PTR__stageThreeArray_001c52d4[sVar1 * 4 + 3] +
                 *(char *)((char)PTR__stageThreeArray_001c52d4[sVar1 * 4 + 1] + param_1) * -2;
    }
    for (; 0xff < local_10; local_10 = local_10 + -0xff) {
    }
    for (; local_10 < 0; local_10 = local_10 + 0xff) {
    }
    *(char *)(local_12 + param_1) = (char)local_10;
    local_12 = local_12 + 1;
  }
  _ConvertToLetters(param_1,1);
  return;
}


// ---- _EmbedDate @ 0x9e302 (866 bytes)

void _EmbedDate(int param_1,short param_2,short param_3,short param_4)

{
  short sVar1;
  short sVar2;
  undefined2 local_16;
  
  *(char *)(param_1 + 8) =
       *(char *)(param_1 + 8) + (char)(param_2 / 10) + (char)((param_2 / 10) / 10) * -10;
  *(char *)(param_1 + 9) = *(char *)(param_1 + 9) + (char)param_2 + (char)(param_2 / 10) * -10;
  *(char *)(param_1 + 10) =
       *(char *)(param_1 + 10) + ((char)(param_3 / 10) + (char)((param_3 / 10) / 10) * -10) * '\x02'
  ;
  *(char *)(param_1 + 0xb) = *(char *)(param_1 + 0xb) + (char)param_3 + (char)(param_3 / 10) * -10;
  sVar1 = param_4 / 10;
  sVar2 = sVar1 / 10;
  *(char *)(param_1 + 0xc) =
       *(char *)(param_1 + 0xc) + (char)(sVar2 / 10) + (char)((sVar2 / 10) / 10) * -10 + '\f';
  *(char *)(param_1 + 0xd) = *(char *)(param_1 + 0xd) + (char)sVar2 + (char)(sVar2 / 10) * -10;
  *(char *)(param_1 + 0xe) = *(char *)(param_1 + 0xe) + (char)sVar1 + (char)(sVar1 / 10) * -10;
  *(char *)(param_1 + 0xf) =
       *(char *)(param_1 + 0xf) + ((char)param_4 + (char)(param_4 / 10) * -10) * '\x02';
  for (local_16 = 8; local_16 < 0x10; local_16 = local_16 + 1) {
    _FixCharIfTooHigh(local_16 + param_1);
  }
  return;
}


// ---- _ExtractDate @ 0x9e664 (485 bytes)

void _ExtractDate(int param_1,int param_2,short *param_3,short *param_4,short *param_5)

{
  char local_10;
  char local_f;
  char local_e;
  char local_d [9];
  
  local_e = *(char *)(param_2 + 8) - *(char *)(param_1 + 8);
  local_d[0] = *(char *)(param_2 + 9) - *(char *)(param_1 + 9);
  _FixNumberIfTooLow(&local_e);
  _FixNumberIfTooLow(local_d);
  *param_3 = local_e * 10 + (short)local_d[0];
  local_e = *(char *)(param_2 + 10) - *(char *)(param_1 + 10);
  local_d[0] = *(char *)(param_2 + 0xb) - *(char *)(param_1 + 0xb);
  _FixNumberIfTooLow(&local_e);
  _FixNumberIfTooLow(local_d);
  *param_4 = (local_e / '\x02') * 10 + (short)local_d[0];
  local_10 = *(char *)(param_2 + 0xc) - *(char *)(param_1 + 0xc);
  local_f = *(char *)(param_2 + 0xd) - *(char *)(param_1 + 0xd);
  local_e = *(char *)(param_2 + 0xe) - *(char *)(param_1 + 0xe);
  local_d[0] = *(char *)(param_2 + 0xf) - *(char *)(param_1 + 0xf);
  _FixNumberIfTooLow(&local_10);
  _FixNumberIfTooLow(&local_f);
  _FixNumberIfTooLow(&local_e);
  _FixNumberIfTooLow(local_d);
  *param_5 = (char)(local_10 + -0xc) * 1000 + local_f * 100 + local_e * 10 +
             (short)(local_d[0] / '\x02');
  return;
}


// ---- _GetChecksumForDate @ 0x9e849 (252 bytes)

int _GetChecksumForDate(short param_1,short param_2)

{
  int iVar1;
  undefined4 local_10;
  
  iVar1 = (int)param_1 + (int)param_2;
  if (iVar1 < 0) {
    iVar1 = iVar1 + 3;
  }
  local_10 = (((int)param_1 * (int)param_2 + (int)(param_1 / 2)) - (int)(param_2 / 3)) -
             (iVar1 >> 2);
  if (local_10 < 0) {
    local_10 = 0;
  }
  for (; 100 < local_10; local_10 = local_10 / 10) {
  }
  for (; 0x19 < local_10; local_10 = local_10 + -0x19) {
  }
  local_10 = local_10 + 0x41;
  if (local_10 == 0x49) {
    local_10 = 0x48;
  }
  if (local_10 == 0x4c) {
    local_10 = 0x4b;
  }
  if (local_10 == 0x4f) {
    local_10 = 0x4d;
  }
  return (int)(char)local_10;
}


// ---- _FixCharIfTooHigh @ 0x9e945 (38 bytes)

void _FixCharIfTooHigh(char *param_1)

{
  if ('Z' < *param_1) {
    *param_1 = *param_1 + -0x19;
  }
  return;
}


// ---- _FixNumberIfTooLow @ 0x9e96b (38 bytes)

void _FixNumberIfTooLow(char *param_1)

{
  if (*param_1 < '\0') {
    *param_1 = *param_1 + '\x19';
  }
  return;
}


// ---- _GetStringLength @ 0x9e991 (46 bytes)

int _GetStringLength(int param_1)

{
  undefined2 local_e;
  
  for (local_e = 0; *(char *)(local_e + param_1) != '\0'; local_e = local_e + 1) {
  }
  return (int)local_e;
}


// ---- _CopyCString @ 0x9e9bf (71 bytes)

void _CopyCString(int param_1,int param_2)

{
  undefined2 local_e;
  
  for (local_e = 0; *(char *)(local_e + param_1) != '\0'; local_e = local_e + 1) {
    *(undefined1 *)(local_e + param_2) = *(undefined1 *)(local_e + param_1);
  }
  *(undefined1 *)(local_e + param_2) = 0;
  return;
}


// ---- _CompareCStringsByLength @ 0x9ea06 (81 bytes)

undefined1 _CompareCStringsByLength(int param_1,int param_2,short param_3)

{
  undefined2 local_10;
  undefined1 local_d;
  
  local_d = 1;
  for (local_10 = 0; local_10 < param_3; local_10 = local_10 + 1) {
    if (*(char *)(local_10 + param_1) != *(char *)(local_10 + param_2)) {
      local_d = 0;
    }
  }
  return local_d;
}


