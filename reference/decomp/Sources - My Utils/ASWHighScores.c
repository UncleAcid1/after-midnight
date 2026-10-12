// Ghidra decompilation of functions from original source file:
//   Sources - My Utils/ASWHighScores.c

// ---- _ASWHighScoresInit @ 0x936b6 (48 bytes)

void _ASWHighScoresInit(undefined4 param_1)

{
  _ICStart(PTR__gASWICInstance_001c5214,param_1);
  return;
}


// ---- _ASWLaunchHighScore @ 0x936e6 (418 bytes)

void _ASWLaunchHighScore(undefined4 param_1,short param_2,short param_3,undefined4 param_4,
                        undefined4 param_5,undefined4 param_6)

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  undefined2 local_52c [2];
  short local_528 [2];
  short local_524 [2];
  short local_520 [2];
  undefined1 local_51c [256];
  char local_41c [256];
  char local_31c [512];
  undefined1 local_11c [268];
  
  local_520[0] = param_2;
  local_524[0] = param_3;
  local_528[0] = (short)param_5;
  local_52c[0] = (undefined2)param_6;
  _local_ValidateInput(local_520,local_524,&param_4,local_528,local_52c);
  _local_EncodeInput((int)local_520[0],(int)local_524[0],param_4,(int)local_528[0],local_52c[0],
                     local_11c);
  _ASWEncodeString(local_11c);
  _sprintf(local_41c,"%s%s",param_1,local_11c);
  uVar2 = 0xffffffff;
  pcVar3 = local_41c;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + 1;
  } while (cVar1 != '\0');
  _md5(local_41c,~uVar2 - 1,local_51c);
  _ASWURLEncode(param_1,0x100);
  _sprintf(local_31c,"http://www.actionsoft.com/mm/hdscores/scores.php?cmd=add&d=%s&l=%s&n=%s",
           local_51c,local_11c,param_1);
  _ASWLaunchURL(local_31c);
  return;
}


// ---- _ASWLaunchURL @ 0x93888 (126 bytes)

/* WARNING: Type propagation algorithm not settling */

void _ASWLaunchURL(char *param_1)

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  int local_18 [3];
  
  local_18[1] = 0;
  uVar2 = 0xffffffff;
  pcVar3 = param_1;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + 1;
  } while (cVar1 != '\0');
  local_18[0] = ~uVar2 - 1;
  local_18[2] = local_18[0];
  _ICLaunchURL(*(undefined4 *)PTR__gASWICInstance_001c5214,&DAT_00121190,param_1,local_18[0],
               local_18 + 1,local_18);
  return;
}


// ---- _ASWEncodeString @ 0x93906 (108 bytes)

void _ASWEncodeString(char *param_1)

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  uint local_10;
  
  local_10 = 0;
  do {
    uVar2 = 0xffffffff;
    pcVar3 = param_1;
    do {
      if (uVar2 == 0) break;
      uVar2 = uVar2 - 1;
      cVar1 = *pcVar3;
      pcVar3 = pcVar3 + 1;
    } while (cVar1 != '\0');
    if (~uVar2 - 1 <= local_10) {
      _ASWStrRev(param_1);
      return;
    }
    cVar1 = _ASWEncodeCharacter((int)param_1[local_10]);
    param_1[local_10] = cVar1;
    local_10 = local_10 + 1;
  } while( true );
}


// ---- _ASWEncodeCharacter @ 0x93972 (40 bytes)

int _ASWEncodeCharacter(char param_1)

{
  __darwin_ct_rune_t _Var1;
  
  _Var1 = ___toupper(param_1 * 2 + 4);
  return (int)(char)_Var1;
}


// ---- _ASWURLEncode @ 0x9399a (289 bytes)

void _ASWURLEncode(char *param_1,short param_2)

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  char local_110 [256];
  short local_10;
  short local_e;
  
  local_e = 0;
  _strcpy(local_110,param_1);
  local_10 = 0;
  do {
    uVar2 = 0xffffffff;
    pcVar3 = local_110;
    do {
      if (uVar2 == 0) break;
      uVar2 = uVar2 - 1;
      cVar1 = *pcVar3;
      pcVar3 = pcVar3 + 1;
    } while (cVar1 != '\0');
    if ((~uVar2 - 1 <= (uint)(int)local_10) || (param_2 <= local_e)) {
      param_1[local_e] = '\0';
      return;
    }
    if (local_110[local_10] == ' ') {
      local_110[local_10] = '+';
    }
    if (local_110[local_10] < ' ') {
      param_1[local_e] = '%';
      local_e = local_e + 1;
      _ASWASCIIToHex(local_110 + local_10,param_1 + local_e,1);
      local_e = local_e + 2;
    }
    else {
      param_1[local_e] = local_110[local_10];
      local_e = local_e + 1;
    }
    local_10 = local_10 + 1;
  } while( true );
}


// ---- _ASWASCIIToHex @ 0x93abb (188 bytes)

void _ASWASCIIToHex(int param_1,int param_2,uint param_3)

{
  byte bVar1;
  byte bVar2;
  undefined1 local_1e;
  undefined1 local_1d;
  undefined4 local_14;
  undefined4 local_10;
  
  local_10 = 0;
  for (local_14 = 0; local_14 < param_3; local_14 = local_14 + 1) {
    bVar1 = *(char *)(local_14 + param_1) >> 4 & 0xf;
    bVar2 = *(byte *)(local_14 + param_1) & 0xf;
    if (bVar1 < 10) {
      local_1e = '0';
    }
    else {
      local_1e = '7';
    }
    if (bVar2 < 10) {
      local_1d = '0';
    }
    else {
      local_1d = '7';
    }
    *(byte *)(local_10 + param_2) = bVar1 + local_1e;
    *(byte *)(local_10 + 1 + param_2) = bVar2 + local_1d;
    local_10 = local_10 + 2;
  }
  return;
}


// ---- _ASWStrRev @ 0x93b77 (114 bytes)

void _ASWStrRev(char *param_1)

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  char *local_18;
  char *local_14;
  
  uVar2 = 0xffffffff;
  pcVar3 = param_1;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + 1;
  } while (cVar1 != '\0');
  local_14 = param_1 + (~uVar2 - 2);
  for (local_18 = param_1; local_18 < local_14; local_18 = local_18 + 1) {
    cVar1 = *local_18;
    *local_18 = *local_14;
    *local_14 = cVar1;
    local_14 = local_14 + -1;
  }
  return;
}


// ---- _local_ValidateInput @ 0x93be9 (178 bytes)

bool _local_ValidateInput(short *param_1,short *param_2,undefined4 param_3,short *param_4)

{
  short sVar1;
  undefined1 local_d;
  
  *param_1 = *param_1 + 1;
  sVar1 = *param_1;
  if (sVar1 < 1) {
    *param_1 = 1;
  }
  local_d = sVar1 >= 1;
  if (99 < *param_1) {
    local_d = false;
    *param_1 = 99;
  }
  if (*param_2 < 0) {
    local_d = false;
    *param_2 = 0;
  }
  if (9 < *param_2) {
    local_d = false;
    *param_2 = 9;
  }
  if (*param_4 < 0) {
    local_d = false;
    *param_4 = 0;
  }
  if (100 < *param_4) {
    local_d = false;
    *param_4 = 100;
  }
  return local_d;
}


// ---- _local_EncodeInput @ 0x93c9b (171 bytes)

void _local_EncodeInput(short param_1,short param_2,undefined4 param_3,short param_4,uint param_5,
                       char *param_6)

{
  char local_22 [12];
  char local_16 [10];
  
  _sprintf(local_16,"%0.5hu",param_5 & 0xffff);
  _sprintf(local_22,"%ld",param_3);
  _sprintf(param_6,"%hd%5s%0.3hd%0.2hd%s",(int)param_2,local_16,(int)param_4,(int)param_1,local_22);
  return;
}


