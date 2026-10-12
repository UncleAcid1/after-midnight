// Ghidra decompilation of functions from original source file:
//   Sources & Headers/MMRegCodeMisc.c

// ---- _GetUpdatedRegistrationCode @ 0x9ea57 (423 bytes)

bool _GetUpdatedRegistrationCode(undefined4 param_1,char *param_2,char *param_3)

{
  char local_210 [256];
  undefined4 local_110;
  char local_10c [4];
  char local_108 [4];
  char local_104 [4];
  char local_100 [4];
  char local_fc [4];
  char local_f8 [4];
  char local_f4 [4];
  char local_f0 [4];
  char local_ec [4];
  char local_e8 [4];
  char local_e4 [4];
  char local_e0 [4];
  char local_dc [4];
  char local_d8;
  undefined1 local_d7 [199];
  int local_10;
  
  local_110._0_1_ = '7';
  local_110._1_1_ = 'h';
  local_110._2_1_ = 't';
  local_110._3_1_ = 't';
  local_10c[0] = 'p';
  local_10c[1] = ':';
  local_10c[2] = '/';
  local_10c[3] = '/';
  local_108[0] = 'w';
  local_108[1] = 'w';
  local_108[2] = 'w';
  local_108[3] = '.';
  local_104[0] = 'a';
  local_104[1] = 'c';
  local_104[2] = 't';
  local_104[3] = 'i';
  local_100[0] = 'o';
  local_100[1] = 'n';
  local_100[2] = 's';
  local_100[3] = 'o';
  local_fc[0] = 'f';
  local_fc[1] = 't';
  local_fc[2] = '.';
  local_fc[3] = 'c';
  local_f8[0] = 'o';
  local_f8[1] = 'm';
  local_f8[2] = '/';
  local_f8[3] = 's';
  local_f4[0] = 'u';
  local_f4[1] = 'p';
  local_f4[2] = 'p';
  local_f4[3] = 'o';
  local_f0[0] = 'r';
  local_f0[1] = 't';
  local_f0[2] = '/';
  local_f0[3] = 'u';
  local_ec[0] = 'p';
  local_ec[1] = 'd';
  local_ec[2] = 'a';
  local_ec[3] = 't';
  local_e8[0] = 'e';
  local_e8[1] = '_';
  local_e8[2] = 'c';
  local_e8[3] = 'o';
  local_e4[0] = 'd';
  local_e4[1] = 'e';
  local_e4[2] = '.';
  local_e4[3] = 'p';
  local_e0[0] = 'h';
  local_e0[1] = 'p';
  local_e0[2] = '?';
  local_e0[3] = 'n';
  local_dc[0] = 'a';
  local_dc[1] = 'm';
  local_dc[2] = 'e';
  local_dc[3] = '=';
  local_d8 = '\0';
  _memset(local_d7,0,199);
  local_10 = 0;
  _ConvertSpacesToURLFormat(param_1,local_210);
  _SWConcatPascalStrings((char *)&local_110,local_210);
  _SWConcatPascalStrings((char *)&local_110,"\x06&code=");
  _SWConcatPascalStrings((char *)&local_110,param_2);
  _SWConcatPascalStrings((char *)&local_110,"\v&faceless=1");
  if ((char)local_110 == -1) {
    local_10 = 0xfffffffe;
  }
  else {
    *(undefined1 *)((int)&local_110 + (local_110 & 0xff) + 1) = 0;
  }
  local_10 = _GetCodeFromWebsite((int)&local_110 + 1,param_3);
  if (*param_2 != *param_3) {
    local_10 = -1;
  }
  return local_10 == 0;
}


// ---- _CheckIfRegCodeIsValid @ 0x9ebfe (430 bytes)

int _CheckIfRegCodeIsValid(undefined4 param_1,char *param_2,undefined4 param_3)

{
  int local_314;
  char local_310 [256];
  char local_210 [256];
  undefined4 local_110;
  char local_10c [4];
  char local_108 [4];
  char local_104 [4];
  char local_100 [4];
  char local_fc [4];
  char local_f8 [4];
  char local_f4 [4];
  char local_f0 [4];
  char local_ec [4];
  char local_e8 [4];
  char local_e4 [4];
  char local_e0 [4];
  char local_dc [2];
  undefined1 local_da [202];
  int local_10;
  
  local_110._0_1_ = '4';
  local_110._1_1_ = 'h';
  local_110._2_1_ = 't';
  local_110._3_1_ = 't';
  local_10c[0] = 'p';
  local_10c[1] = ':';
  local_10c[2] = '/';
  local_10c[3] = '/';
  local_108[0] = 'w';
  local_108[1] = 'w';
  local_108[2] = 'w';
  local_108[3] = '.';
  local_104[0] = 'a';
  local_104[1] = 'c';
  local_104[2] = 't';
  local_104[3] = 'i';
  local_100[0] = 'o';
  local_100[1] = 'n';
  local_100[2] = 's';
  local_100[3] = 'o';
  local_fc[0] = 'f';
  local_fc[1] = 't';
  local_fc[2] = '.';
  local_fc[3] = 'c';
  local_f8[0] = 'o';
  local_f8[1] = 'm';
  local_f8[2] = '/';
  local_f8[3] = 'm';
  local_f4[0] = 'm';
  local_f4[1] = '/';
  local_f4[2] = 'v';
  local_f4[3] = 'a';
  local_f0[0] = 'l';
  local_f0[1] = 'i';
  local_f0[2] = 'd';
  local_f0[3] = 'a';
  local_ec[0] = 't';
  local_ec[1] = 'e';
  local_ec[2] = '_';
  local_ec[3] = 'c';
  local_e8[0] = 'o';
  local_e8[1] = 'd';
  local_e8[2] = 'e';
  local_e8[3] = '.';
  local_e4[0] = 'p';
  local_e4[1] = 'h';
  local_e4[2] = 'p';
  local_e4[3] = '?';
  local_e0[0] = 'n';
  local_e0[1] = 'a';
  local_e0[2] = 'm';
  local_e0[3] = 'e';
  local_dc[0] = '=';
  local_dc[1] = '\0';
  _memset(local_da,0,0xca);
  local_10 = 0;
  _ConvertSpacesToURLFormat(param_1,local_210);
  _SWConcatPascalStrings((char *)&local_110,local_210);
  _SWConcatPascalStrings((char *)&local_110,"\x06&code=");
  _SWConcatPascalStrings((char *)&local_110,param_2);
  if ((char)local_110 == -1) {
    local_10 = 0xfffffffe;
  }
  else {
    *(undefined1 *)((int)&local_110 + (local_110 & 0xff) + 1) = 0;
  }
  local_10 = _GetCodeFromWebsite((int)&local_110 + 1,local_310);
  if (local_310[0] == '\0') {
    local_10 = -3;
  }
  else {
    _SWStringToNum(local_310,&local_314);
  }
  if (local_10 == 0) {
    *(bool *)param_3 = 0 < local_314;
  }
  return local_10;
}


// ---- _CheckIfRegCodeWasPirated @ 0x9edac (450 bytes)

bool _CheckIfRegCodeWasPirated(undefined4 param_1,char *param_2)

{
  int local_314;
  char local_310 [256];
  char local_210 [256];
  undefined4 local_110;
  char local_10c [4];
  char local_108 [4];
  char local_104 [4];
  char local_100 [4];
  char local_fc [4];
  char local_f8 [4];
  char local_f4 [4];
  char local_f0 [4];
  char local_ec [4];
  char local_e8 [4];
  char local_e4 [4];
  char local_e0 [4];
  char local_dc [2];
  undefined1 local_da [202];
  int local_10;
  
  local_110._0_1_ = '4';
  local_110._1_1_ = 'h';
  local_110._2_1_ = 't';
  local_110._3_1_ = 't';
  local_10c[0] = 'p';
  local_10c[1] = ':';
  local_10c[2] = '/';
  local_10c[3] = '/';
  local_108[0] = 'w';
  local_108[1] = 'w';
  local_108[2] = 'w';
  local_108[3] = '.';
  local_104[0] = 'a';
  local_104[1] = 'c';
  local_104[2] = 't';
  local_104[3] = 'i';
  local_100[0] = 'o';
  local_100[1] = 'n';
  local_100[2] = 's';
  local_100[3] = 'o';
  local_fc[0] = 'f';
  local_fc[1] = 't';
  local_fc[2] = '.';
  local_fc[3] = 'c';
  local_f8[0] = 'o';
  local_f8[1] = 'm';
  local_f8[2] = '/';
  local_f8[3] = 'm';
  local_f4[0] = 'm';
  local_f4[1] = '/';
  local_f4[2] = 'v';
  local_f4[3] = 'a';
  local_f0[0] = 'l';
  local_f0[1] = 'i';
  local_f0[2] = 'd';
  local_f0[3] = 'a';
  local_ec[0] = 't';
  local_ec[1] = 'e';
  local_ec[2] = '_';
  local_ec[3] = 'c';
  local_e8[0] = 'o';
  local_e8[1] = 'd';
  local_e8[2] = 'e';
  local_e8[3] = '.';
  local_e4[0] = 'p';
  local_e4[1] = 'h';
  local_e4[2] = 'p';
  local_e4[3] = '?';
  local_e0[0] = 'n';
  local_e0[1] = 'a';
  local_e0[2] = 'm';
  local_e0[3] = 'e';
  local_dc[0] = '=';
  local_dc[1] = '\0';
  _memset(local_da,0,0xca);
  local_10 = 0;
  _ConvertSpacesToURLFormat(param_1,local_210);
  _SWConcatPascalStrings((char *)&local_110,local_210);
  _SWConcatPascalStrings((char *)&local_110,"\x06&code=");
  _SWConcatPascalStrings((char *)&local_110,param_2);
  if ((char)local_110 == -1) {
    local_10 = 0xfffffffe;
  }
  else {
    *(undefined1 *)((int)&local_110 + (local_110 & 0xff) + 1) = 0;
  }
  local_10 = _GetCodeFromWebsite((int)&local_110 + 1,local_310);
  if (local_310[0] == '\0') {
    local_10 = -3;
  }
  else {
    _SWStringToNum(local_310,&local_314);
  }
  return local_10 == 0 && local_314 == -99;
}


// ---- _StripAllWhiteSpace @ 0x9ef6e (200 bytes)

void _StripAllWhiteSpace(int param_1,undefined1 *param_2)

{
  char cVar1;
  short local_12;
  short local_10;
  short local_e;
  
  local_10 = 1;
  local_e = 0;
  _WriteCStringToFile("Unmodified data:");
  _WriteCStringToFile(param_1);
  local_12 = 0;
  while (cVar1 = _IsWhiteSpace((int)*(char *)(local_12 + param_1)), cVar1 != '\0') {
    local_12 = local_12 + 1;
  }
  for (; ((cVar1 = _IsWhiteSpace((int)*(char *)(local_12 + param_1)), cVar1 == '\0' &&
          (*(char *)(local_12 + param_1) != '\0')) && (local_e < 0xff)); local_e = local_e + 1) {
    param_2[local_10] = *(undefined1 *)(local_12 + param_1);
    local_12 = local_12 + 1;
    local_10 = local_10 + 1;
  }
  *param_2 = (char)local_e;
  return;
}


// ---- _IsWhiteSpace @ 0x9f036 (67 bytes)

undefined1 _IsWhiteSpace(char param_1)

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


// ---- _ConvertSpacesToURLFormat @ 0x9f079 (201 bytes)

void _ConvertSpacesToURLFormat(byte *param_1,byte *param_2)

{
  int iVar1;
  int local_14;
  int local_10;
  
  *param_2 = *param_1;
  local_10 = 1;
  local_14 = 1;
  while( true ) {
    if ((int)(uint)*param_1 < local_14) {
      return;
    }
    if (param_1[local_14] == 0x20) {
      param_2[local_10] = 0x25;
      iVar1 = local_10 + 1;
      if (iVar1 < 0xff) {
        param_2[iVar1] = 0x32;
        *param_2 = *param_2 + 1;
        iVar1 = local_10 + 2;
      }
      local_10 = iVar1;
      if (local_10 < 0xff) {
        param_2[local_10] = 0x30;
        local_10 = local_10 + 1;
        *param_2 = *param_2 + 1;
      }
    }
    else {
      param_2[local_10] = param_1[local_14];
      local_10 = local_10 + 1;
    }
    if (0xfe < local_10) break;
    local_14 = local_14 + 1;
  }
  return;
}


// ---- _GetNumDaysBeforeMonth @ 0x9f142 (86 bytes)

int _GetNumDaysBeforeMonth(short param_1)

{
  short local_10;
  short local_e;
  
  local_e = 0;
  for (local_10 = 0; (int)local_10 < param_1 + -1; local_10 = local_10 + 1) {
    local_e = local_e + *(short *)(PTR__daysInMonth_001c52fc + local_10 * 2);
  }
  return (int)local_e;
}


// ---- _MMCreateLicenseFile @ 0x9f198 (458 bytes)

int _MMCreateLicenseFile(char *param_1,byte *param_2,undefined4 param_3)

{
  char local_41c [256];
  byte local_31c [256];
  byte local_21c;
  undefined1 auStack_21b [255];
  byte local_11c [256];
  undefined4 local_1c;
  short local_16;
  int local_14;
  undefined4 local_10;
  
  local_14 = 0;
  local_1c = 0x82;
  _SWNumToString(0x82,local_11c);
  _ChangeNumbersToLetters(local_11c);
  local_1c = _getProcessorSpeed();
  _SWNumToString(local_1c,&local_21c);
  _SWCopyPascalString(param_1,(char *)local_31c);
  _ConvertStringToUpperCase(local_31c);
  local_16 = 0;
  _SWCopyPascalString((char *)local_11c,local_41c);
  local_16 = (ushort)local_11c[0] + local_16 + 1;
  _SWCopyPascalString((char *)param_2,local_41c + local_16);
  local_16 = (ushort)*param_2 + local_16 + 1;
  _SWCopyPascalString((char *)local_31c,local_41c + local_16);
  local_16 = (ushort)local_31c[0] + local_16 + 1;
  _SWCopyPascalString((char *)&local_21c,local_41c + local_16);
  local_16 = (ushort)local_21c + local_16 + 1;
  auStack_21b[local_21c] = 0;
  local_41c[local_16] = '\x01';
  local_16 = local_16 + 1;
  _ConvertStringLetters(local_41c,0xff);
  _SwapStringChars(local_41c);
  local_10 = _MMGetPathForLocalLicenseFolder(param_3);
  if (local_14 == 0) {
    _MMWriteLicenseFile(local_41c,local_10);
  }
  _CloseOutputFile();
  return local_14;
}


// ---- _MMWriteLicenseFile @ 0x9f362 (162 bytes)

void _MMWriteLicenseFile(undefined4 param_1,undefined4 param_2)

{
  undefined4 local_18;
  int local_14;
  undefined4 local_10;
  
  _printf("Creating license file at path: %s\n",param_2);
  local_14 = _SDL_RWFromFile(param_2,"wb");
  if (local_14 != 0) {
    _ClearFileStickyError();
    _MyWriteDataToFile(local_14,0xff,param_1);
    local_18 = 0x204d67b0;
    _MyWriteDataToFile(local_14,4,&local_18);
    local_10 = _GetFileStickyError();
    (**(code **)(local_14 + 0xc))(local_14);
  }
  return;
}


// ---- _MMReadLicenseFile @ 0x9f404 (618 bytes)

undefined4 _MMReadLicenseFile(char *param_1,char *param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 local_530;
  int local_528;
  int local_524;
  byte local_51e [256];
  char local_41e [256];
  char local_31e [256];
  char local_21e [256];
  char local_11e [256];
  short local_1e;
  int local_18;
  undefined4 local_14;
  int local_10;
  
  local_14 = _MMGetPathForLicenseFolder(param_4);
  local_10 = _SDL_RWFromFile(local_14,"rb");
  if (local_10 != 0) {
    _ClearFileStickyError();
    _MyReadDataFromFile(local_10,0xff,local_51e);
    _MyReadDataFromFile(local_10,4,param_3);
    local_18 = _GetFileStickyError();
    (**(code **)(local_10 + 0xc))(local_10);
  }
  if (local_18 == 0) {
    _SwapStringChars(local_51e);
    _ConvertStringLetters(local_51e,0xff);
    local_1e = 0;
    _SWCopyPascalString((char *)local_51e,local_11e);
    local_1e = (ushort)local_51e[local_1e] + local_1e + 1;
    _SWCopyPascalString((char *)(local_51e + local_1e),local_31e);
    local_1e = (ushort)local_51e[local_1e] + local_1e + 1;
    _SWCopyPascalString((char *)(local_51e + local_1e),local_21e);
    local_1e = (ushort)local_51e[local_1e] + local_1e + 1;
    _SWCopyPascalString((char *)(local_51e + local_1e),local_41e);
    local_1e = (ushort)local_51e[local_1e] + local_1e + 1;
    _ChangeLettersToNumbers(local_11e);
    _SWStringToNum(local_11e,&local_524);
    if (local_524 < 0x82) {
      local_530 = 0;
    }
    else {
      _SWStringToNum(local_41e,&local_528);
      iVar1 = _getProcessorSpeed();
      if (iVar1 == local_528) {
        _SWCopyPascalString(local_31e,param_2);
        _SWCopyPascalString(local_21e,param_1);
        local_530 = 1;
      }
      else {
        local_530 = 0;
      }
    }
  }
  else {
    local_530 = 0;
  }
  return local_530;
}


// ---- _IsNamePirated @ 0x9f66e (150 bytes)

undefined4 _IsNamePirated(char *param_1)

{
  int iVar1;
  char local_10f [255];
  int local_10;
  
  _SWCopyPascalStringToCString(param_1,local_10f);
  _stringToLowercase(local_10f);
  local_10 = 0;
  while( true ) {
    if (7 < local_10) {
      return 0;
    }
    iVar1 = _strcmp(local_10f,*(char **)(PTR__gPiratedNames_001c5304 + local_10 * 4));
    if (iVar1 == 0) break;
    local_10 = local_10 + 1;
  }
  return 1;
}


// ---- _stringToLowercase @ 0x9f704 (82 bytes)

void _stringToLowercase(char *param_1)

{
  int iVar1;
  __darwin_ct_rune_t _Var2;
  
  for (; *param_1 != '\0'; param_1 = param_1 + 1) {
    iVar1 = ___istype((int)*param_1,0x8000);
    if (iVar1 != 0) {
      _Var2 = ___tolower((int)*param_1);
      *param_1 = (char)_Var2;
    }
  }
  return;
}


// ---- ___istype @ 0x9f756 (98 bytes)

int ___istype(__darwin_ct_rune_t _c,ulong _f)

{
  uint uVar1;
  uint local_10;
  
  if ((_c & 0xffffff80U) == 0) {
    uVar1 = *(uint *)(PTR_001c46e4 + _c * 4 + 0x34) & _f;
  }
  else {
    uVar1 = ___maskrune(_c,_f);
  }
  local_10 = (uint)(uVar1 != 0);
  return local_10;
}


// ---- _stringToUppercase @ 0x9f7b8 (82 bytes)

void _stringToUppercase(char *param_1)

{
  int iVar1;
  __darwin_ct_rune_t _Var2;
  
  for (; *param_1 != '\0'; param_1 = param_1 + 1) {
    iVar1 = ___istype((int)*param_1,0x1000);
    if (iVar1 != 0) {
      _Var2 = ___toupper((int)*param_1);
      *param_1 = (char)_Var2;
    }
  }
  return;
}


// ---- _MMDeleteLicenseFile @ 0x9f80a (39 bytes)

void _MMDeleteLicenseFile(undefined4 param_1)

{
  char *pcVar1;
  
  pcVar1 = (char *)_MMGetPathForLocalLicenseFolder(param_1);
  _remove(pcVar1);
  return;
}


// ---- _MMGetPathForLicenseFolder @ 0x9f831 (25 bytes)

void _MMGetPathForLicenseFolder(undefined4 param_1)

{
  _MMGetPathForLocalLicenseFolder(param_1);
  return;
}


// ---- _MMGetPathForLocalLicenseFolder @ 0x9f84a (25 bytes)

void _MMGetPathForLocalLicenseFolder(undefined4 param_1)

{
  _GetAppSupportPath(param_1);
  return;
}


// ---- _ConvertStringToUpperCase @ 0x9f863 (109 bytes)

void _ConvertStringToUpperCase(byte *param_1)

{
  short local_10;
  
  for (local_10 = 1; local_10 <= (short)(ushort)*param_1; local_10 = local_10 + 1) {
    if ((0x60 < param_1[local_10]) && (param_1[local_10] < 0x7b)) {
      param_1[local_10] = param_1[local_10] - 0x20;
    }
  }
  return;
}


// ---- _ChangeNumbersToLetters @ 0x9f8d0 (90 bytes)

void _ChangeNumbersToLetters(byte *param_1)

{
  int local_10;
  
  for (local_10 = 1; local_10 <= (int)(uint)*param_1; local_10 = local_10 + 1) {
    if ((0x2f < param_1[local_10]) && (param_1[local_10] < 0x3a)) {
      param_1[local_10] = param_1[local_10] + 0x11;
    }
  }
  return;
}


// ---- _ChangeLettersToNumbers @ 0x9f92a (90 bytes)

void _ChangeLettersToNumbers(byte *param_1)

{
  int local_10;
  
  for (local_10 = 1; local_10 <= (int)(uint)*param_1; local_10 = local_10 + 1) {
    if ((0x40 < param_1[local_10]) && (param_1[local_10] < 0x4b)) {
      param_1[local_10] = param_1[local_10] - 0x11;
    }
  }
  return;
}


// ---- _ConvertStringLetters @ 0x9f984 (88 bytes)

void _ConvertStringLetters(int param_1,short param_2)

{
  char cVar1;
  undefined2 local_10;
  
  for (local_10 = 1; local_10 < param_2; local_10 = local_10 + 1) {
    cVar1 = _GetMatchingLetter((int)*(char *)(local_10 + param_1));
    if (cVar1 != -1) {
      *(char *)(local_10 + param_1) = cVar1;
    }
  }
  return;
}


// ---- _GetMatchingLetter @ 0x9f9dc (168 bytes)

int _GetMatchingLetter(char param_1)

{
  short local_10;
  short local_e;
  
  local_e = 0;
  do {
    if (1 < local_e) {
      return -1;
    }
    for (local_10 = 0; local_10 < 0xd; local_10 = local_10 + 1) {
      if (PTR__letterArray_001c5308[(int)local_e + local_10 * 2] == param_1) {
        if (local_e == 0) {
          return (int)(char)PTR__letterArray_001c5308[local_10 * 2 + 1];
        }
        return (int)(char)PTR__letterArray_001c5308[local_10 * 2];
      }
    }
    local_e = local_e + 1;
  } while( true );
}


// ---- _SwapStringChars @ 0x9fa84 (129 bytes)

void _SwapStringChars(int param_1)

{
  undefined1 uVar1;
  short sVar2;
  short local_12;
  
  for (local_12 = 0; local_12 < 0xb; local_12 = local_12 + 1) {
    sVar2 = *(short *)(PTR__swapValues_001c5300 + local_12 * 4 + 2);
    uVar1 = *(undefined1 *)(*(short *)(PTR__swapValues_001c5300 + local_12 * 4) + param_1);
    *(undefined1 *)(*(short *)(PTR__swapValues_001c5300 + local_12 * 4) + param_1) =
         *(undefined1 *)(sVar2 + param_1);
    *(undefined1 *)(sVar2 + param_1) = uVar1;
  }
  return;
}


// ---- _GetRegNameFromClipboard @ 0x9fb05 (144 bytes)

char * _GetRegNameFromClipboard(char *param_1)

{
  char cVar1;
  int iVar2;
  char *pcVar3;
  uint uVar4;
  
  iVar2 = _SDL_GetClipboardText();
  if ((iVar2 == 0) || (iVar2 = _strstrIgnoreCase(iVar2,param_1), iVar2 == 0)) {
    return (char *)0x0;
  }
  uVar4 = 0xffffffff;
  do {
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    cVar1 = *param_1;
    param_1 = param_1 + 1;
  } while (cVar1 != '\0');
  pcVar3 = _strtok((char *)(~uVar4 + iVar2),"\n\r");
  return pcVar3;
}


// ---- _GetRegCodeAfterPrefixFromClipboard @ 0x9fb95 (114 bytes)

int GetRegCodeAfterPrefixFromClipboard(char *param_1)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  
  iVar2 = _SDL_GetClipboardText();
  if ((iVar2 == 0) || (iVar2 = _strstrIgnoreCase(iVar2,param_1), iVar2 == 0)) {
    return 0;
  }
  uVar3 = 0xffffffff;
  do {
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    cVar1 = *param_1;
    param_1 = param_1 + 1;
  } while (cVar1 != '\0');
  *(undefined1 *)(~uVar3 + iVar2 + 0x10) = 0;
  return iVar2;
}


// ---- _GetRegCodeFromClipboard @ 0x9fc07 (25 bytes)

void _GetRegCodeFromClipboard(undefined4 param_1)

{
  _GetRegNameFromClipboard(param_1);
  return;
}


