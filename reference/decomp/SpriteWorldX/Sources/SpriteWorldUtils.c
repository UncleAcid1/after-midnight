// Ghidra decompilation of functions from original source file:
//   ../SpriteWorldX/Sources/SpriteWorldUtils.c

// ---- _SWCenterRect @ 0x2a9d0 (244 bytes)

void _SWCenterRect(short *param_1,short *param_2)

{
  short sVar1;
  short sVar2;
  
  sVar1 = param_1[1];
  sVar2 = *param_1;
  param_1[1] = param_2[1] +
               ((short)(((int)param_2[3] - (int)param_2[1]) / 2) - (short)(param_1[3] - sVar1) / 2);
  *param_1 = *param_2 +
             ((short)(((int)param_2[2] - (int)*param_2) / 2) - (short)(param_1[2] - sVar2) / 2);
  param_1[3] = param_1[1] + (param_1[3] - sVar1);
  param_1[2] = *param_1 + (param_1[2] - sVar2);
  return;
}


// ---- _SWSetRect @ 0x2aac4 (90 bytes)

void _SWSetRect(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
               undefined2 param_5)

{
  param_1[1] = param_2;
  *param_1 = param_3;
  param_1[3] = param_4;
  param_1[2] = param_5;
  return;
}


// ---- _SWOffsetRect @ 0x2ab1e (106 bytes)

void _SWOffsetRect(short *param_1,short param_2,short param_3)

{
  param_1[1] = param_1[1] + param_2;
  param_1[3] = param_1[3] + param_2;
  *param_1 = *param_1 + param_3;
  param_1[2] = param_1[2] + param_3;
  return;
}


// ---- _SWInsetRect @ 0x2ab88 (106 bytes)

void _SWInsetRect(short *param_1,short param_2,short param_3)

{
  param_1[1] = param_1[1] + param_2;
  param_1[3] = param_1[3] - param_2;
  *param_1 = *param_1 + param_3;
  param_1[2] = param_1[2] - param_3;
  return;
}


// ---- _SWEqualRect @ 0x2abf2 (111 bytes)

undefined1 _SWEqualRect(short *param_1,short *param_2)

{
  undefined1 local_10;
  
  if ((((*param_1 == *param_2) && (param_1[1] == param_2[1])) && (param_1[3] == param_2[3])) &&
     (param_1[2] == param_2[2])) {
    local_10 = 1;
  }
  else {
    local_10 = 0;
  }
  return local_10;
}


// ---- _Randomize @ 0x2ac61 (34 bytes)

void _Randomize(void)

{
  uint uVar1;
  
  uVar1 = _time((time_t *)0x0);
  _srand(uVar1);
  return;
}


// ---- _GetRandom @ 0x2ac83 (101 bytes)

int _GetRandom(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = _rand();
  return (int)((float)((param_2 - param_1) + 1) * ((float)iVar1 / 2.1474836e+09)) + param_1;
}


// ---- _GetRandomFloat @ 0x2ace8 (119 bytes)

longdouble _GetRandomFloat(float param_1,float param_2)

{
  int iVar1;
  
  iVar1 = _rand();
  return (longdouble)((param_2 - param_1) * ((float)iVar1 / 2.1474836e+09) + param_1);
}


// ---- _SWCopyPascalString @ 0x2ad5f (64 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWCopyPascalString(char *sourceString,char *targetString)

{
  short local_e;
  
  for (local_e = 0; local_e <= (short)(ushort)(byte)*sourceString; local_e = local_e + 1) {
    targetString[local_e] = sourceString[local_e];
  }
  return;
}


// ---- _SWCopyCString @ 0x2ad9f (71 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWCopyCString(char *sourceCString,char *targetCString)

{
  short local_e;
  
  for (local_e = 0; sourceCString[local_e] != '\0'; local_e = local_e + 1) {
    targetCString[local_e] = sourceCString[local_e];
  }
  targetCString[local_e] = '\0';
  return;
}


// ---- _SWEqualString @ 0x2ade6 (163 bytes)

undefined4 _SWEqualString(byte *param_1,byte *param_2)

{
  byte bVar1;
  ushort local_24;
  ushort local_22;
  short local_10;
  
  if (*param_1 < *param_2) {
    bVar1 = *param_1;
  }
  else {
    bVar1 = *param_2;
  }
  local_22 = (ushort)bVar1;
  local_24 = local_22;
  if (0xff < local_22) {
    local_24 = 0xff;
  }
  local_10 = 1;
  while( true ) {
    if ((short)local_24 < local_10) {
      return 1;
    }
    if (param_1[local_10] != param_2[local_10]) break;
    local_10 = local_10 + 1;
  }
  return 0;
}


// ---- _SWCopyCStringToPascalString @ 0x2ae89 (91 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWCopyCStringToPascalString(char *sourceCString,char *targetPString)

{
  short local_e;
  
  for (local_e = 0; local_e < 0xff && sourceCString[local_e] != '\0'; local_e = local_e + 1) {
    targetPString[local_e + 1] = sourceCString[local_e];
  }
  *targetPString = (char)local_e;
  return;
}


// ---- _SWCopyPascalStringToCString @ 0x2aee4 (109 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWCopyPascalStringToCString(char *sourcePString,char *targetCString)

{
  ushort local_1e;
  short local_e;
  
  if (*sourcePString == -1) {
    local_1e = 0xff;
  }
  else {
    local_1e = (ushort)(byte)*sourcePString;
  }
  for (local_e = 0; local_e < (short)local_1e; local_e = local_e + 1) {
    targetCString[local_e] = sourcePString[local_e + 1];
  }
  targetCString[local_e] = '\0';
  return;
}


// ---- _SWConcatPascalStrings @ 0x2af51 (165 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWConcatPascalStrings(char *targetString,char *appendString)

{
  byte bVar1;
  uint local_20;
  short local_e;
  
  local_20 = 0xff - (byte)*targetString;
  if ((int)(uint)(byte)*appendString < (int)(0xff - (byte)*targetString)) {
    local_20 = (uint)(byte)*appendString;
  }
  bVar1 = *targetString;
  if (0 < (short)local_20) {
    for (local_e = 0; local_e < (short)local_20; local_e = local_e + 1) {
      targetString[(int)local_e + (int)(short)(bVar1 + 1)] = appendString[local_e + 1];
    }
    *targetString = *targetString + (char)local_20;
  }
  return;
}


// ---- _SWStringToNum @ 0x2aff6 (59 bytes)

void _SWStringToNum(char *param_1,long *param_2)

{
  long lVar1;
  char local_10d [265];
  
  _SWCopyPascalStringToCString(param_1,local_10d);
  lVar1 = _atol(local_10d);
  *param_2 = lVar1;
  return;
}


// ---- _SWNumToString @ 0x2b031 (83 bytes)

void _SWNumToString(undefined4 param_1,char *param_2)

{
  char local_10b [259];
  
  _sprintf(local_10b,"%d",param_1);
  _SWCopyCStringToPascalString(local_10b,param_2);
  return;
}


// ---- _SWCos @ 0x2b084 (80 bytes)

void _SWCos(undefined4 param_1,undefined4 param_2)

{
  _cos((360.0 - (double)CONCAT44(param_2,param_1)) * 0.017453292522222223);
  return;
}


// ---- _SWSin @ 0x2b0d4 (80 bytes)

void _SWSin(undefined4 param_1,undefined4 param_2)

{
  _sin((360.0 - (double)CONCAT44(param_2,param_1)) * 0.017453292522222223);
  return;
}


// ---- _SWATan @ 0x2b124 (96 bytes)

longdouble SWATan(undefined4 param_1,undefined4 param_2)

{
  longdouble lVar1;
  
  lVar1 = (longdouble)_atan(CONCAT44(param_2,param_1));
  return (longdouble)((double)lVar1 * -57.29577950560105 + 360.0);
}


// ---- _ReportError @ 0x2b184 (91 bytes)

void _ReportError(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  _fprintf((FILE *)(PTR_001c43e8 + 0xb0),
           "\n\nFatal error (#%d).\n  File:%s\n  Function:%s\n  Line:%d\n",param_1,param_2,param_3,
           param_4);
                    /* WARNING: Subroutine does not return */
  _exit(1);
}


