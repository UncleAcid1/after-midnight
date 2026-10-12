// Ghidra decompilation of functions from original source file:
//   Sources - My Utils/md5 edited.c

// ---- _md5 @ 0x93d46 (418 bytes)

void _md5(undefined4 param_1,undefined4 param_2,char *param_3)

{
  byte local_84;
  byte local_83;
  byte local_82;
  byte local_81;
  byte local_80;
  byte local_7f;
  byte local_7e;
  byte local_7d;
  byte local_7c;
  byte local_7b;
  byte local_7a;
  byte local_79;
  byte local_78;
  byte local_77;
  byte local_76;
  byte local_75;
  undefined1 local_74 [100];
  
  _MD5Init(local_74);
  _MD5Update(local_74,param_1,param_2);
  _MD5Final(&local_84,local_74);
  _sprintf(param_3,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",(uint)local_84
           ,(uint)local_83,(uint)local_82,(uint)local_81,(uint)local_80,(uint)local_7f,
           (uint)local_7e,(uint)local_7d,(uint)local_7c,(uint)local_7b,(uint)local_7a,(uint)local_79
           ,(uint)local_78,(uint)local_77,(uint)local_76,(uint)local_75);
  return;
}


// ---- _md5NoFormatting @ 0x93ee8 (77 bytes)

void _md5NoFormatting(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined1 local_64 [96];
  
  _MD5Init(local_64);
  _MD5Update(local_64,param_1,param_2);
  _MD5Final(param_3,local_64);
  *(undefined1 *)(param_3 + 0x10) = 0;
  return;
}


// ---- _formatMD5 @ 0x93f35 (373 bytes)

void _formatMD5(byte *param_1,char *param_2)

{
  _sprintf(param_2,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",(uint)*param_1
           ,(uint)param_1[1],(uint)param_1[2],(uint)param_1[3],(uint)param_1[4],(uint)param_1[5],
           (uint)param_1[6],(uint)param_1[7],(uint)param_1[8],(uint)param_1[9],(uint)param_1[10],
           (uint)param_1[0xb],(uint)param_1[0xc],(uint)param_1[0xd],(uint)param_1[0xe],
           (uint)param_1[0xf]);
  return;
}


// ---- _byteReverse @ 0x940aa (104 bytes)

void _byteReverse(undefined4 *param_1,int param_2)

{
  do {
    *param_1 = *param_1;
    param_1 = param_1 + 1;
    param_2 = param_2 + -1;
  } while (param_2 != 0);
  return;
}


// ---- _MD5Init @ 0x94112 (73 bytes)

void _MD5Init(undefined4 *param_1)

{
  *param_1 = 0x67452301;
  param_1[1] = 0xefcdab89;
  param_1[2] = 0x98badcfe;
  param_1[3] = 0x10325476;
  param_1[4] = 0;
  param_1[5] = 0;
  return;
}


// ---- _MD5Update @ 0x9415b (439 bytes)

void _MD5Update(int param_1,undefined4 *param_2,uint param_3)

{
  uint uVar1;
  uint uVar2;
  undefined4 *puVar3;
  undefined1 *puVar4;
  
  uVar1 = *(uint *)(param_1 + 0x10);
  *(uint *)(param_1 + 0x10) = param_3 * 8 + uVar1;
  if (*(uint *)(param_1 + 0x10) < uVar1) {
    *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + 1;
  }
  *(uint *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + (param_3 >> 0x1d);
  uVar1 = uVar1 >> 3 & 0x3f;
  if (uVar1 != 0) {
    puVar4 = (undefined1 *)(param_1 + 0x18 + uVar1);
    uVar1 = 0x40 - uVar1;
    uVar2 = uVar1;
    puVar3 = param_2;
    if (param_3 < uVar1) {
      for (; param_3 != 0; param_3 = param_3 - 1) {
        *puVar4 = *(undefined1 *)param_2;
        param_2 = (undefined4 *)((int)param_2 + 1);
        puVar4 = puVar4 + 1;
      }
      return;
    }
    for (; uVar2 != 0; uVar2 = uVar2 - 1) {
      *puVar4 = *(undefined1 *)puVar3;
      puVar3 = (undefined4 *)((int)puVar3 + 1);
      puVar4 = puVar4 + 1;
    }
    _byteReverse(param_1 + 0x18,0x10);
    _MD5Transform(param_1,param_1 + 0x18);
    param_2 = (undefined4 *)((int)param_2 + uVar1);
    param_3 = param_3 - uVar1;
  }
  for (; 0x3f < param_3; param_3 = param_3 - 0x40) {
    *(undefined4 *)(param_1 + 0x18) = *param_2;
    *(undefined4 *)(param_1 + 0x1c) = param_2[1];
    *(undefined4 *)(param_1 + 0x20) = param_2[2];
    *(undefined4 *)(param_1 + 0x24) = param_2[3];
    *(undefined4 *)(param_1 + 0x28) = param_2[4];
    *(undefined4 *)(param_1 + 0x2c) = param_2[5];
    *(undefined4 *)(param_1 + 0x30) = param_2[6];
    *(undefined4 *)(param_1 + 0x34) = param_2[7];
    *(undefined4 *)(param_1 + 0x38) = param_2[8];
    *(undefined4 *)(param_1 + 0x3c) = param_2[9];
    *(undefined4 *)(param_1 + 0x40) = param_2[10];
    *(undefined4 *)(param_1 + 0x44) = param_2[0xb];
    *(undefined4 *)(param_1 + 0x48) = param_2[0xc];
    *(undefined4 *)(param_1 + 0x4c) = param_2[0xd];
    *(undefined4 *)(param_1 + 0x50) = param_2[0xe];
    *(undefined4 *)(param_1 + 0x54) = param_2[0xf];
    _byteReverse(param_1 + 0x18,0x10);
    _MD5Transform(param_1,param_1 + 0x18);
    param_2 = param_2 + 0x10;
  }
  puVar4 = (undefined1 *)(param_1 + 0x18);
  for (; param_3 != 0; param_3 = param_3 - 1) {
    *puVar4 = *(undefined1 *)param_2;
    param_2 = (undefined4 *)((int)param_2 + 1);
    puVar4 = puVar4 + 1;
  }
  return;
}


// ---- _MD5Final @ 0x94312 (319 bytes)

void _MD5Final(undefined4 *param_1,undefined4 *param_2)

{
  uint uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined1 *puVar4;
  
  uVar1 = (uint)param_2[4] >> 3 & 0x3f;
  puVar4 = (undefined1 *)((int)param_2 + uVar1 + 0x18);
  *puVar4 = 0x80;
  puVar4 = puVar4 + 1;
  iVar2 = -uVar1;
  uVar1 = iVar2 + 0x3f;
  if (uVar1 < 8) {
    for (; uVar1 != 0; uVar1 = uVar1 - 1) {
      *puVar4 = 0;
      puVar4 = puVar4 + 1;
    }
    _byteReverse(param_2 + 6,0x10);
    _MD5Transform(param_2,param_2 + 6);
    puVar3 = param_2 + 6;
    for (iVar2 = 0xe; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar3 = 0;
      puVar3 = puVar3 + 1;
    }
  }
  else {
    for (iVar2 = iVar2 + 0x37; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar4 = 0;
      puVar4 = puVar4 + 1;
    }
  }
  _byteReverse(param_2 + 6,0xe);
  param_2[0x14] = param_2[4];
  param_2[0x15] = param_2[5];
  _MD5Transform(param_2,param_2 + 6);
  _byteReverse(param_2,4);
  *param_1 = *param_2;
  param_1[1] = param_2[1];
  param_1[2] = param_2[2];
  param_1[3] = param_2[3];
  *param_2 = 0;
  return;
}


// ---- _MD5Transform @ 0x94451 (3005 bytes)

void _MD5Transform(int *param_1,int *param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  uVar4 = param_1[1];
  uVar3 = param_1[2];
  uVar2 = param_1[3];
  uVar1 = (uVar2 ^ (uVar3 ^ uVar2) & uVar4) + *param_2 + *param_1 + 0xd76aa478;
  uVar1 = (uVar1 >> 0x19 | uVar1 * 0x80) + uVar4;
  uVar2 = (uVar3 ^ (uVar4 ^ uVar3) & uVar1) + param_2[1] + uVar2 + 0xe8c7b756;
  uVar2 = (uVar2 >> 0x14 | uVar2 * 0x1000) + uVar1;
  uVar3 = (uVar4 ^ (uVar1 ^ uVar4) & uVar2) + param_2[2] + uVar3 + 0x242070db;
  uVar3 = (uVar3 >> 0xf | uVar3 * 0x20000) + uVar2;
  uVar4 = (uVar1 ^ (uVar2 ^ uVar1) & uVar3) + param_2[3] + uVar4 + 0xc1bdceee;
  uVar4 = (uVar4 >> 10 | uVar4 * 0x400000) + uVar3;
  uVar1 = (uVar2 ^ (uVar3 ^ uVar2) & uVar4) + param_2[4] + uVar1 + 0xf57c0faf;
  uVar1 = (uVar1 >> 0x19 | uVar1 * 0x80) + uVar4;
  uVar2 = (uVar3 ^ (uVar4 ^ uVar3) & uVar1) + param_2[5] + uVar2 + 0x4787c62a;
  uVar2 = (uVar2 >> 0x14 | uVar2 * 0x1000) + uVar1;
  uVar3 = (uVar4 ^ (uVar1 ^ uVar4) & uVar2) + param_2[6] + uVar3 + 0xa8304613;
  uVar3 = (uVar3 >> 0xf | uVar3 * 0x20000) + uVar2;
  uVar4 = (uVar1 ^ (uVar2 ^ uVar1) & uVar3) + param_2[7] + uVar4 + 0xfd469501;
  uVar4 = (uVar4 >> 10 | uVar4 * 0x400000) + uVar3;
  uVar1 = (uVar2 ^ (uVar3 ^ uVar2) & uVar4) + param_2[8] + uVar1 + 0x698098d8;
  uVar1 = (uVar1 >> 0x19 | uVar1 * 0x80) + uVar4;
  uVar2 = (uVar3 ^ (uVar4 ^ uVar3) & uVar1) + param_2[9] + uVar2 + 0x8b44f7af;
  uVar2 = (uVar2 >> 0x14 | uVar2 * 0x1000) + uVar1;
  uVar3 = ((uVar4 ^ (uVar1 ^ uVar4) & uVar2) + param_2[10] + uVar3) - 0xa44f;
  uVar3 = (uVar3 >> 0xf | uVar3 * 0x20000) + uVar2;
  uVar4 = (uVar1 ^ (uVar2 ^ uVar1) & uVar3) + param_2[0xb] + uVar4 + 0x895cd7be;
  uVar4 = (uVar4 >> 10 | uVar4 * 0x400000) + uVar3;
  uVar1 = (uVar2 ^ (uVar3 ^ uVar2) & uVar4) + param_2[0xc] + uVar1 + 0x6b901122;
  uVar1 = (uVar1 >> 0x19 | uVar1 * 0x80) + uVar4;
  uVar2 = (uVar3 ^ (uVar4 ^ uVar3) & uVar1) + param_2[0xd] + uVar2 + 0xfd987193;
  uVar2 = (uVar2 >> 0x14 | uVar2 * 0x1000) + uVar1;
  uVar3 = (uVar4 ^ (uVar1 ^ uVar4) & uVar2) + param_2[0xe] + uVar3 + 0xa679438e;
  uVar3 = (uVar3 >> 0xf | uVar3 * 0x20000) + uVar2;
  uVar4 = (uVar1 ^ (uVar2 ^ uVar1) & uVar3) + param_2[0xf] + uVar4 + 0x49b40821;
  uVar4 = (uVar4 >> 10 | uVar4 * 0x400000) + uVar3;
  uVar1 = (uVar3 ^ (uVar4 ^ uVar3) & uVar2) + param_2[1] + uVar1 + 0xf61e2562;
  uVar1 = (uVar1 >> 0x1b | uVar1 * 0x20) + uVar4;
  uVar2 = (uVar4 ^ (uVar1 ^ uVar4) & uVar3) + param_2[6] + uVar2 + 0xc040b340;
  uVar2 = (uVar2 >> 0x17 | uVar2 * 0x200) + uVar1;
  uVar3 = (uVar1 ^ (uVar2 ^ uVar1) & uVar4) + param_2[0xb] + uVar3 + 0x265e5a51;
  uVar3 = (uVar3 >> 0x12 | uVar3 * 0x4000) + uVar2;
  uVar4 = (uVar2 ^ (uVar3 ^ uVar2) & uVar1) + *param_2 + uVar4 + 0xe9b6c7aa;
  uVar4 = (uVar4 >> 0xc | uVar4 * 0x100000) + uVar3;
  uVar1 = (uVar3 ^ (uVar4 ^ uVar3) & uVar2) + param_2[5] + uVar1 + 0xd62f105d;
  uVar1 = (uVar1 >> 0x1b | uVar1 * 0x20) + uVar4;
  uVar2 = (uVar4 ^ (uVar1 ^ uVar4) & uVar3) + param_2[10] + uVar2 + 0x2441453;
  uVar2 = (uVar2 >> 0x17 | uVar2 * 0x200) + uVar1;
  uVar3 = (uVar1 ^ (uVar2 ^ uVar1) & uVar4) + param_2[0xf] + uVar3 + 0xd8a1e681;
  uVar3 = (uVar3 >> 0x12 | uVar3 * 0x4000) + uVar2;
  uVar4 = (uVar2 ^ (uVar3 ^ uVar2) & uVar1) + param_2[4] + uVar4 + 0xe7d3fbc8;
  uVar4 = (uVar4 >> 0xc | uVar4 * 0x100000) + uVar3;
  uVar1 = (uVar3 ^ (uVar4 ^ uVar3) & uVar2) + param_2[9] + uVar1 + 0x21e1cde6;
  uVar1 = (uVar1 >> 0x1b | uVar1 * 0x20) + uVar4;
  uVar2 = (uVar4 ^ (uVar1 ^ uVar4) & uVar3) + param_2[0xe] + uVar2 + 0xc33707d6;
  uVar2 = (uVar2 >> 0x17 | uVar2 * 0x200) + uVar1;
  uVar3 = (uVar1 ^ (uVar2 ^ uVar1) & uVar4) + param_2[3] + uVar3 + 0xf4d50d87;
  uVar3 = (uVar3 >> 0x12 | uVar3 * 0x4000) + uVar2;
  uVar4 = (uVar2 ^ (uVar3 ^ uVar2) & uVar1) + param_2[8] + uVar4 + 0x455a14ed;
  uVar4 = (uVar4 >> 0xc | uVar4 * 0x100000) + uVar3;
  uVar1 = (uVar3 ^ (uVar4 ^ uVar3) & uVar2) + param_2[0xd] + uVar1 + 0xa9e3e905;
  uVar1 = (uVar1 >> 0x1b | uVar1 * 0x20) + uVar4;
  uVar2 = (uVar4 ^ (uVar1 ^ uVar4) & uVar3) + param_2[2] + uVar2 + 0xfcefa3f8;
  uVar2 = (uVar2 >> 0x17 | uVar2 * 0x200) + uVar1;
  uVar3 = (uVar1 ^ (uVar2 ^ uVar1) & uVar4) + param_2[7] + uVar3 + 0x676f02d9;
  uVar3 = (uVar3 >> 0x12 | uVar3 * 0x4000) + uVar2;
  uVar4 = (uVar2 ^ (uVar3 ^ uVar2) & uVar1) + param_2[0xc] + uVar4 + 0x8d2a4c8a;
  uVar4 = (uVar4 >> 0xc | uVar4 * 0x100000) + uVar3;
  uVar1 = ((uVar2 ^ uVar4 ^ uVar3) + param_2[5] + uVar1) - 0x5c6be;
  uVar1 = (uVar1 >> 0x1c | uVar1 * 0x10) + uVar4;
  uVar2 = (uVar3 ^ uVar1 ^ uVar4) + param_2[8] + uVar2 + 0x8771f681;
  uVar2 = (uVar2 >> 0x15 | uVar2 * 0x800) + uVar1;
  uVar3 = (uVar4 ^ uVar2 ^ uVar1) + param_2[0xb] + uVar3 + 0x6d9d6122;
  uVar3 = (uVar3 >> 0x10 | uVar3 * 0x10000) + uVar2;
  uVar4 = (uVar1 ^ uVar3 ^ uVar2) + param_2[0xe] + uVar4 + 0xfde5380c;
  uVar4 = (uVar4 >> 9 | uVar4 * 0x800000) + uVar3;
  uVar1 = (uVar2 ^ uVar4 ^ uVar3) + param_2[1] + uVar1 + 0xa4beea44;
  uVar1 = (uVar1 >> 0x1c | uVar1 * 0x10) + uVar4;
  uVar2 = (uVar3 ^ uVar1 ^ uVar4) + param_2[4] + uVar2 + 0x4bdecfa9;
  uVar2 = (uVar2 >> 0x15 | uVar2 * 0x800) + uVar1;
  uVar3 = (uVar4 ^ uVar2 ^ uVar1) + param_2[7] + uVar3 + 0xf6bb4b60;
  uVar3 = (uVar3 >> 0x10 | uVar3 * 0x10000) + uVar2;
  uVar4 = (uVar1 ^ uVar3 ^ uVar2) + param_2[10] + uVar4 + 0xbebfbc70;
  uVar4 = (uVar4 >> 9 | uVar4 * 0x800000) + uVar3;
  uVar1 = (uVar2 ^ uVar4 ^ uVar3) + param_2[0xd] + uVar1 + 0x289b7ec6;
  uVar1 = (uVar1 >> 0x1c | uVar1 * 0x10) + uVar4;
  uVar2 = (uVar3 ^ uVar1 ^ uVar4) + *param_2 + uVar2 + 0xeaa127fa;
  uVar2 = (uVar2 >> 0x15 | uVar2 * 0x800) + uVar1;
  uVar3 = (uVar4 ^ uVar2 ^ uVar1) + param_2[3] + uVar3 + 0xd4ef3085;
  uVar3 = (uVar3 >> 0x10 | uVar3 * 0x10000) + uVar2;
  uVar4 = (uVar1 ^ uVar3 ^ uVar2) + param_2[6] + uVar4 + 0x4881d05;
  uVar4 = (uVar4 >> 9 | uVar4 * 0x800000) + uVar3;
  uVar1 = (uVar2 ^ uVar4 ^ uVar3) + param_2[9] + uVar1 + 0xd9d4d039;
  uVar1 = (uVar1 >> 0x1c | uVar1 * 0x10) + uVar4;
  uVar2 = (uVar3 ^ uVar1 ^ uVar4) + param_2[0xc] + uVar2 + 0xe6db99e5;
  uVar2 = (uVar2 >> 0x15 | uVar2 * 0x800) + uVar1;
  uVar3 = (uVar4 ^ uVar2 ^ uVar1) + param_2[0xf] + uVar3 + 0x1fa27cf8;
  uVar3 = (uVar3 >> 0x10 | uVar3 * 0x10000) + uVar2;
  uVar4 = (uVar1 ^ uVar3 ^ uVar2) + param_2[2] + uVar4 + 0xc4ac5665;
  uVar4 = (uVar4 >> 9 | uVar4 * 0x800000) + uVar3;
  uVar1 = (uVar3 ^ (~uVar2 | uVar4)) + *param_2 + uVar1 + 0xf4292244;
  uVar1 = (uVar1 >> 0x1a | uVar1 * 0x40) + uVar4;
  uVar2 = (uVar4 ^ (~uVar3 | uVar1)) + param_2[7] + uVar2 + 0x432aff97;
  uVar2 = (uVar2 >> 0x16 | uVar2 * 0x400) + uVar1;
  uVar3 = (uVar1 ^ (~uVar4 | uVar2)) + param_2[0xe] + uVar3 + 0xab9423a7;
  uVar3 = (uVar3 >> 0x11 | uVar3 * 0x8000) + uVar2;
  uVar4 = (uVar2 ^ (~uVar1 | uVar3)) + param_2[5] + uVar4 + 0xfc93a039;
  uVar4 = (uVar4 >> 0xb | uVar4 * 0x200000) + uVar3;
  uVar1 = (uVar3 ^ (~uVar2 | uVar4)) + param_2[0xc] + uVar1 + 0x655b59c3;
  uVar1 = (uVar1 >> 0x1a | uVar1 * 0x40) + uVar4;
  uVar2 = (uVar4 ^ (~uVar3 | uVar1)) + param_2[3] + uVar2 + 0x8f0ccc92;
  uVar2 = (uVar2 >> 0x16 | uVar2 * 0x400) + uVar1;
  uVar3 = ((uVar1 ^ (~uVar4 | uVar2)) + param_2[10] + uVar3) - 0x100b83;
  uVar3 = (uVar3 >> 0x11 | uVar3 * 0x8000) + uVar2;
  uVar4 = (uVar2 ^ (~uVar1 | uVar3)) + param_2[1] + uVar4 + 0x85845dd1;
  uVar4 = (uVar4 >> 0xb | uVar4 * 0x200000) + uVar3;
  uVar1 = (uVar3 ^ (~uVar2 | uVar4)) + param_2[8] + uVar1 + 0x6fa87e4f;
  uVar1 = (uVar1 >> 0x1a | uVar1 * 0x40) + uVar4;
  uVar2 = (uVar4 ^ (~uVar3 | uVar1)) + param_2[0xf] + uVar2 + 0xfe2ce6e0;
  uVar2 = (uVar2 >> 0x16 | uVar2 * 0x400) + uVar1;
  uVar3 = (uVar1 ^ (~uVar4 | uVar2)) + param_2[6] + uVar3 + 0xa3014314;
  uVar3 = (uVar3 >> 0x11 | uVar3 * 0x8000) + uVar2;
  uVar4 = (uVar2 ^ (~uVar1 | uVar3)) + param_2[0xd] + uVar4 + 0x4e0811a1;
  uVar4 = (uVar4 >> 0xb | uVar4 * 0x200000) + uVar3;
  uVar1 = (uVar3 ^ (~uVar2 | uVar4)) + param_2[4] + uVar1 + 0xf7537e82;
  uVar1 = (uVar1 >> 0x1a | uVar1 * 0x40) + uVar4;
  uVar2 = (uVar4 ^ (~uVar3 | uVar1)) + param_2[0xb] + uVar2 + 0xbd3af235;
  uVar2 = (uVar2 >> 0x16 | uVar2 * 0x400) + uVar1;
  uVar3 = (uVar1 ^ (~uVar4 | uVar2)) + param_2[2] + uVar3 + 0x2ad7d2bb;
  uVar3 = (uVar3 >> 0x11 | uVar3 * 0x8000) + uVar2;
  uVar4 = (uVar2 ^ (~uVar1 | uVar3)) + param_2[9] + uVar4 + 0xeb86d391;
  *param_1 = *param_1 + uVar1;
  param_1[1] = param_1[1] + (uVar4 >> 0xb | uVar4 * 0x200000) + uVar3;
  param_1[2] = param_1[2] + uVar3;
  param_1[3] = param_1[3] + uVar2;
  return;
}


