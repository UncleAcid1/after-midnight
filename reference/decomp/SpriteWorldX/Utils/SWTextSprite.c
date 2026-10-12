// Ghidra decompilation of functions from original source file:
//   ../SpriteWorldX/Utils/SWTextSprite.c

// ---- _CreateBMTextSprite @ 0x97bb1 (526 bytes)

SpritePtr _CreateBMTextSprite(int param_1,undefined2 param_2)

{
  SpritePtr local_1c;
  void *local_18;
  short local_12;
  int local_10;
  
  local_10 = 0;
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___20964_001c5268,0x20);
  }
  if (*(int *)(param_1 + 0x84) < 0x1a) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___20964_001c5268,0x21);
  }
  local_18 = _calloc(1,0x1694);
  if (local_18 == (void *)0x0) {
    local_10 = 200;
  }
  if (local_10 == 0) {
    local_10 = _SWCloneSprite(param_1,&local_1c,local_18,0x1694);
  }
  if (local_10 == 0) {
    _SWSetSpriteDrawProc(local_1c,(DrawProcPtr_t)PTR__BMTextSpriteDrawProc_001c5280);
    _SWSetCurrentFrameIndex(local_1c,local_1c->curFrameIndex);
    *(undefined1 *)((int)local_18 + 0xf8) = 0;
    *(undefined2 *)((int)local_18 + 0x4e4) = 0;
    *(undefined2 *)((int)local_18 + 0x4e6) = 0x1a;
    *(undefined2 *)((int)local_18 + 0x4e8) = 0x34;
    *(undefined1 *)((int)local_18 + 0x4e0) = 1;
    *(short *)((int)local_18 + 0x4ea) = *(short *)((int)local_18 + 0x4e8) + 10;
    *(undefined4 *)((int)local_18 + 0x4ec) = 0x212c2e20;
    *(undefined4 *)((int)local_18 + 0x4f0) = 0x2a252423;
    *(undefined4 *)((int)local_18 + 0x4f4) = 0x2b2d2928;
    *(undefined4 *)((int)local_18 + 0x4f8) = 0x27223f2f;
    *(undefined2 *)((int)local_18 + 0x4fc) = 0x3a3b;
    *(undefined1 *)((int)local_18 + 0x4fe) = 0;
    *(short *)((int)local_18 + 0x5ec) = *(short *)((int)local_18 + 0x4e8) + 0x1d;
    *(undefined1 *)((int)local_18 + 0x5ee) = 0;
    *(undefined1 *)((int)local_18 + 0x1690) = 0;
    *(undefined2 *)((int)local_18 + 0x4e2) = param_2;
    for (local_12 = 0; local_12 < 500; local_12 = local_12 + 1) {
      *(undefined4 *)((int)local_18 + local_12 * 4 + 0x6f0) = 0;
    }
    local_1c->userData = 0x7edf;
  }
  else {
    local_1c = (SpritePtr)0x0;
  }
  return local_1c;
}


// ---- _CloneBMTextSprite @ 0x97dbf (574 bytes)

SpritePtr CloneBMTextSprite(int param_1,undefined2 param_2)

{
  SpritePtr pSStack_20;
  void *pvStack_1c;
  int iStack_18;
  short sStack_12;
  int iStack_10;
  
  iStack_18 = param_1;
  iStack_10 = 0;
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___21006_001c5270,0x5d);
  }
  if (*(int *)(param_1 + 0xe8) != 0x7edf) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___21006_001c5270,0x5e);
  }
  pvStack_1c = _calloc(1,0x1694);
  if (pvStack_1c == (void *)0x0) {
    iStack_10 = 200;
  }
  if (iStack_10 == 0) {
    iStack_10 = _SWCloneSprite(param_1,&pSStack_20,pvStack_1c,0x1694);
  }
  if (iStack_10 == 0) {
    _SWSetSpriteDrawProc(pSStack_20,(DrawProcPtr_t)PTR__BMTextSpriteDrawProc_001c5280);
    *(undefined2 *)((int)pvStack_1c + 0x4e4) = *(undefined2 *)(iStack_18 + 0x4e4);
    *(undefined2 *)((int)pvStack_1c + 0x4e6) = *(undefined2 *)(iStack_18 + 0x4e6);
    *(undefined2 *)((int)pvStack_1c + 0x4e8) = *(undefined2 *)(iStack_18 + 0x4e8);
    *(undefined2 *)((int)pvStack_1c + 0x4ea) = *(undefined2 *)(iStack_18 + 0x4ea);
    *(undefined2 *)((int)pvStack_1c + 0x5ec) = *(undefined2 *)(iStack_18 + 0x5ec);
    *(undefined1 *)((int)pvStack_1c + 0x4e0) = *(undefined1 *)(iStack_18 + 0x4e0);
    *(char *)((int)pvStack_1c + 0x1690) = segment_command_00001690.segname[iStack_18 + -8];
    *(undefined2 *)((int)pvStack_1c + 0x4e2) = param_2;
    for (sStack_12 = 0; sStack_12 < 500; sStack_12 = sStack_12 + 1) {
      *(undefined4 *)(iStack_18 + 0x6f0 + sStack_12 * 4) =
           *(undefined4 *)((int)pvStack_1c + sStack_12 * 4 + 0x6f0);
    }
    _strcpy((char *)(iStack_18 + 0x4ec),(char *)((int)pvStack_1c + 0x4ec));
    _strcpy((char *)(iStack_18 + 0x5ee),(char *)((int)pvStack_1c + 0x5ee));
    _strcpy((char *)(iStack_18 + 0xf8),(char *)((int)pvStack_1c + 0xf8));
    pSStack_20->userData = 0x7edf;
  }
  else {
    pSStack_20 = (SpritePtr)0x0;
  }
  return pSStack_20;
}


// ---- _SetBMTextSpriteIndexes @ 0x97ffd (174 bytes)

void SetBMTextSpriteIndexes
               (int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
               undefined1 param_5)

{
  if (*(int *)(param_1 + 0xe8) != 0x7edf) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___21054_001c527c,0x97);
  }
  *(undefined2 *)(param_1 + 0x4e4) = param_2;
  *(undefined2 *)(param_1 + 0x4e6) = param_3;
  *(undefined2 *)(param_1 + 0x4e8) = param_4;
  *(undefined1 *)(param_1 + 0x4e0) = param_5;
  return;
}


// ---- _SetBMTextSpriteSpecialChars @ 0x980ab (210 bytes)

void _SetBMTextSpriteSpecialChars(int param_1,undefined2 param_2,char *param_3)

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  
  if (*(int *)(param_1 + 0xe8) != 0x7edf) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___21065_001c5258,0xa9);
  }
  uVar2 = 0xffffffff;
  pcVar3 = param_3;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + 1;
  } while (cVar1 != '\0');
  if (0xfe < ~uVar2 - 1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___21065_001c5258,0xaa);
  }
  _strcpy((char *)(param_1 + 0x4ec),param_3);
  *(undefined2 *)(param_1 + 0x4ea) = param_2;
  return;
}


// ---- _SetBMTextSpriteUnicodeChars @ 0x9817d (210 bytes)

void _SetBMTextSpriteUnicodeChars(int param_1,undefined2 param_2,char *param_3)

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  
  if (*(int *)(param_1 + 0xe8) != 0x7edf) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___21081_001c5260,0xba);
  }
  uVar2 = 0xffffffff;
  pcVar3 = param_3;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + 1;
  } while (cVar1 != '\0');
  if (0xfe < ~uVar2 - 1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___21081_001c5260,0xbb);
  }
  _strcpy((char *)(param_1 + 0x5ee),param_3);
  *(undefined2 *)(param_1 + 0x5ec) = param_2;
  return;
}


// ---- _SetBMTextSpriteText @ 0x9824f (173 bytes)

void _SetBMTextSpriteText(int param_1,char *param_2)

{
  short sVar1;
  longdouble lVar2;
  
  if (*(int *)(param_1 + 0xe8) != 0x7edf) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___21096_001c526c,0xc9);
  }
  _strcpy((char *)(param_1 + 0xf8),param_2);
  sVar1 = *(short *)(param_1 + 0x38);
  lVar2 = (longdouble)_GetWidthOfBMText(param_1,param_2);
  *(short *)(param_1 + 0x3c) = (short)(int)((float)(int)sVar1 + (float)lVar2);
  return;
}


// ---- _SetBMTextSpriteVertOffsetForChars @ 0x982fc (101 bytes)

void _SetBMTextSpriteVertOffsetForChars(undefined4 param_1,char *param_2,undefined4 param_3)

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  uint local_10;
  
  local_10 = 0;
  do {
    uVar2 = 0xffffffff;
    pcVar3 = param_2;
    do {
      if (uVar2 == 0) break;
      uVar2 = uVar2 - 1;
      cVar1 = *pcVar3;
      pcVar3 = pcVar3 + 1;
    } while (cVar1 != '\0');
    if (~uVar2 - 1 <= local_10) {
      return;
    }
    _SetBMTextSpriteVertOffsetForChar(param_1,(int)param_2[local_10],param_3);
    local_10 = local_10 + 1;
  } while( true );
}


// ---- _SetBMTextSpriteVertOffsetForChar @ 0x98361 (227 bytes)

void _SetBMTextSpriteVertOffsetForChar(int param_1,char param_2,undefined4 param_3)

{
  short sVar1;
  char *pcVar2;
  
  if (*(int *)(param_1 + 0xe8) != 0x7edf) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___21131_001c5278,0xe8);
  }
  pcVar2 = _strchr((char *)(param_1 + 0x4ec),(int)param_2);
  if (pcVar2 != (char *)0x0) {
    sVar1 = ((short)pcVar2 - ((short)param_1 + 0x4ec)) + *(short *)(param_1 + 0x4ea);
    if (499 < sVar1) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
                 PTR____FUNCTION___21131_001c5278,0xee);
    }
    *(undefined4 *)(param_1 + 0xec0 + sVar1 * 4) = param_3;
  }
  return;
}


// ---- _SetBMTextSpriteCharSpacing @ 0x98444 (246 bytes)

void _SetBMTextSpriteCharSpacing(int param_1,int param_2,short param_3,float param_4)

{
  short local_e;
  
  if (*(int *)(param_1 + 0xe8) != 0x7edf) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___21159_001c525c,0x100);
  }
  if (499 < param_3) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___21159_001c525c,0x101);
  }
  segment_command_00001690.segname[param_1 + -8] = '\x01';
  for (local_e = 0; local_e < param_3; local_e = local_e + 1) {
    *(float *)(param_1 + 0x6f0 + local_e * 4) = *(float *)(local_e * 4 + param_2) * param_4;
  }
  for (; local_e < 500; local_e = local_e + 1) {
    *(float *)(param_1 + 0x6f0 + local_e * 4) = param_4;
  }
  return;
}


// ---- _GetTextSpriteText @ 0x9853a (90 bytes)

int GetTextSpriteText(int param_1)

{
  if (*(int *)(param_1 + 0xe8) != 0x7edf) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___21184_001c5288,0x114);
  }
  return param_1 + 0xf8;
}


// ---- _BMTextSpriteDrawProc @ 0x98594 (1052 bytes)

void _BMTextSpriteDrawProc(SpritePtr param_1,undefined4 param_2,float param_3,float param_4)

{
  int iVar1;
  longdouble extraout_ST0;
  longdouble lVar2;
  undefined4 local_92c;
  undefined1 auStack_928 [100];
  char *apcStack_8c4 [100];
  char local_734 [400];
  char local_5a4 [400];
  char local_414 [1000];
  char local_2c;
  char local_2b;
  short local_2a;
  char *local_28;
  char *local_24;
  short local_1e;
  short local_1c;
  short local_1a;
  float local_18;
  float local_14;
  SpritePtr local_10;
  
  local_10 = param_1;
  if (param_1->userData != 0x7edf) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___21215_001c5264,0x137);
  }
  iVar1 = (int)param_4;
  local_92c = CONCAT22((short)(int)param_3,(short)iVar1);
  if (*(short *)((int)&local_10[2].drawData + 2) < 1) {
    _DrawBMLineOfText(param_1,&local_10->f_0f8,param_3,param_4);
  }
  else {
    _SWCopyCString((char *)&local_10->f_0f8,local_414);
    _SWGetSpriteHeight(param_1);
    local_18 = (float)extraout_ST0 + 2.0;
    local_1e = 0;
    local_1c = 0;
    local_24 = local_414;
    do {
      local_24 = _strchr(local_24,0xd);
      if (local_24 != (char *)0x0) {
        local_24 = local_24 + 1;
        if (*local_24 == '\r') {
          auStack_928[local_1c] = 1;
          local_1c = local_1c + 1;
          local_24 = local_24 + 1;
        }
        else {
          auStack_928[local_1c] = 0;
          local_1c = local_1c + 1;
        }
      }
    } while (local_24 != (char *)0x0);
    local_1c = 0;
    local_24 = _strtok(local_414,"\r");
    do {
      apcStack_8c4[local_1c] = local_24;
      local_1c = local_1c + 1;
      local_24 = _strtok((char *)0x0,"\r");
    } while (local_24 != (char *)0x0);
    local_1e = 0;
    local_28 = _strtok(apcStack_8c4[0]," ");
    local_2c = '\0';
    while (local_2c == '\0') {
      local_1a = (short)(int)local_18;
      local_5a4[0] = '\0';
      _strcpy(local_5a4,local_28);
      local_2b = '\0';
      while (local_2b == '\0') {
        local_28 = _strtok((char *)0x0," ");
        while (local_28 == (char *)0x0) {
          local_1e = local_1e + 1;
          if (local_1c <= local_1e) {
            local_2c = '\x01';
            goto LAB_000988e0;
          }
          local_28 = _strtok(apcStack_8c4[local_1e]," ");
          if (auStack_928[local_1e + -1] != '\0') {
            local_1a = (short)(int)((float)(int)local_1a + local_18);
          }
          local_2b = '\x01';
          if (local_28 != (char *)0x0) goto LAB_000988e0;
        }
        _sprintf(local_734,"%s %s",local_5a4,local_28);
        lVar2 = (longdouble)_GetWidthOfBMText(param_1,local_734);
        local_2a = (short)(int)lVar2;
        if (local_2a < *(short *)((int)&local_10[2].drawData + 2)) {
          _strcpy(local_5a4,local_734);
        }
        else {
          local_2b = '\x01';
        }
      }
LAB_000988e0:
      if (local_5a4[0] == '\0') break;
      lVar2 = (longdouble)_DrawBMLineOfText(param_1,local_5a4,param_3,param_4);
      local_14 = (float)lVar2;
      param_4 = param_4 + (float)(int)local_1a;
      if ((float)(int)local_92c._2_2_ < param_3 + local_14) {
        local_92c = CONCAT22((short)(int)(param_3 + local_14),(short)local_92c);
      }
      if ((float)(int)(short)local_92c < param_4) {
        local_92c = CONCAT22(local_92c._2_2_,(short)(int)param_4);
      }
    }
    (param_1->destFrameRect).top = (short)iVar1;
    (param_1->destFrameRect).left = (short)(int)param_3;
    (param_1->destFrameRect).bottom = (short)local_92c;
    (param_1->destFrameRect).right = local_92c._2_2_;
  }
  return;
}


// ---- _GetWidthOfBMText @ 0x989b0 (646 bytes)

longdouble _GetWidthOfBMText(int param_1,char *param_2)

{
  char cVar1;
  float fVar2;
  bool bVar3;
  short sVar4;
  char *pcVar5;
  int iVar6;
  float fVar7;
  short sStack_34;
  short sStack_30;
  short local_2e;
  short local_2c;
  float local_24;
  
  if (*(int *)(param_1 + 0xe8) != 0x7edf) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___21308_001c5284,0x1cc);
  }
  local_24 = 0.0;
  sStack_30 = (short)((uint)*(undefined4 *)(**(int **)(param_1 + 0x78) + 0x1c) >> 0x10);
  sStack_34 = (short)((uint)*(undefined4 *)(**(int **)(param_1 + 0x78) + 0x18) >> 0x10);
  fVar7 = *(float *)(*(int *)(param_1 + 0xe0) + 0x2c) * (float)((int)sStack_30 - (int)sStack_34);
  iVar6 = -1;
  pcVar5 = param_2;
  do {
    if (iVar6 == 0) break;
    iVar6 = iVar6 + -1;
    cVar1 = *pcVar5;
    pcVar5 = pcVar5 + 1;
  } while (cVar1 != '\0');
  for (local_2e = 0; local_2e < (short)(~(ushort)iVar6 - 1); local_2e = local_2e + 1) {
    bVar3 = false;
    cVar1 = param_2[local_2e];
    if ((cVar1 < 'A') || ('Z' < cVar1)) {
      if ((cVar1 < 'a') || ('z' < cVar1)) {
        if ((cVar1 < '0') || ('9' < cVar1)) {
          pcVar5 = _strchr((char *)(param_1 + 0x4ec),(int)cVar1);
          if (pcVar5 == (char *)0x0) {
            sVar4 = _strchrUnicode(param_1 + 0x5ee,(int)*(short *)(param_2 + local_2e));
            if (sVar4 == -1) {
              bVar3 = true;
            }
            else {
              local_2c = *(short *)(param_1 + 0x5ec) + sVar4;
            }
          }
          else {
            local_2c = ((short)pcVar5 - ((short)param_1 + 0x4ec)) + *(short *)(param_1 + 0x4ea);
          }
        }
        else {
          local_2c = (short)cVar1 + *(short *)(param_1 + 0x4e8) + -0x30;
        }
      }
      else if (*(char *)(param_1 + 0x4e0) == '\0') {
        local_2c = cVar1 + -0x61 + *(short *)(param_1 + 0x4e6);
      }
      else {
        local_2c = cVar1 + -0x61 + *(short *)(param_1 + 0x4e4);
      }
    }
    else {
      local_2c = (short)cVar1 + *(short *)(param_1 + 0x4e6) + -0x41;
    }
    fVar2 = fVar7;
    if ((segment_command_00001690.segname[param_1 + -8] != '\0') && (!bVar3)) {
      fVar2 = *(float *)(param_1 + 0x6f0 + local_2c * 4) * fVar7;
    }
    local_24 = local_24 + fVar2;
  }
  return (longdouble)local_24;
}


// ---- _DrawBMLineOfText @ 0x98c36 (947 bytes)

longdouble _DrawBMLineOfText(SpriteRec *param_1,char *param_2,float param_3,float param_4)

{
  float fVar1;
  char cVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  DrawDataRec *pDVar5;
  FrameRec **ppFVar6;
  FrameRec *pFVar7;
  undefined2 uVar8;
  undefined2 uVar9;
  bool bVar10;
  short sVar11;
  char *pcVar12;
  int iVar13;
  float fVar14;
  short local_50;
  short sStack_4e;
  short local_4c;
  short sStack_4a;
  short local_40;
  short local_3e;
  float local_30;
  float local_2c;
  
  pDVar5 = param_1->drawData;
  if (param_1->userData != 0x7edf) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c",
               PTR____FUNCTION___21414_001c5274,0x216);
  }
  local_2c = 0.0;
  ppFVar6 = param_1->frameArray;
  pFVar7 = *ppFVar6;
  uVar3 = (pFVar7->frameRect).bottom;
  uVar8 = (pFVar7->frameRect).right;
  uVar4 = (pFVar7->frameRect).top;
  uVar9 = (pFVar7->frameRect).left;
  fVar14 = pDVar5->scaledWidthFactor * (float)((int)(short)uVar8 - (int)(short)uVar9);
  fVar1 = pDVar5->scaledHeightFactor;
  if (*(int *)PTR__gSWCurrentSpriteWorld_001c439c != 0) {
    param_3 = param_3 + (float)(int)*(short *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x8e);
    param_4 = param_4 + (float)(int)*(short *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x8c);
  }
  iVar13 = -1;
  pcVar12 = param_2;
  do {
    if (iVar13 == 0) break;
    iVar13 = iVar13 + -1;
    cVar2 = *pcVar12;
    pcVar12 = pcVar12 + 1;
  } while (cVar2 != '\0');
  for (local_40 = 0; local_40 < (short)(~(ushort)iVar13 - 1); local_40 = local_40 + 1) {
    bVar10 = false;
    cVar2 = param_2[local_40];
    if ((cVar2 < 'A') || ('Z' < cVar2)) {
      if ((cVar2 < 'a') || ('z' < cVar2)) {
        if ((cVar2 < '0') || ('9' < cVar2)) {
          pcVar12 = _strchr((char *)&param_1[2].misc1,(int)cVar2);
          if (pcVar12 == (char *)0x0) {
            sVar11 = _strchrUnicode(&param_1[2].f_1ee,(int)*(short *)(param_2 + local_40));
            if (sVar11 == -1) {
              bVar10 = true;
            }
            else {
              local_3e = *(short *)&param_1[2].f_1ec + sVar11;
            }
          }
          else {
            local_3e = ((short)pcVar12 - ((short)param_1 + 0x4ec)) +
                       *(short *)((int)&param_1[2].userData + 2);
          }
        }
        else {
          local_3e = (short)cVar2 + (short)param_1[2].userData + -0x30;
        }
      }
      else if (*(char *)&param_1[2].drawData == '\0') {
        local_3e = cVar2 + -0x61 + *(short *)&param_1[2].field_0xe6;
      }
      else {
        local_3e = cVar2 + -0x61 + *(short *)&param_1[2].scalingMode;
      }
    }
    else {
      local_3e = (short)cVar2 + *(short *)&param_1[2].field_0xe6 + -0x41;
    }
    if (!bVar10) {
      if (param_1->maxFrames <= (int)local_3e) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Utils/SWTextSprite.c"
                   ,PTR____FUNCTION___21414_001c5274,0x259);
      }
      _SWStdSpriteDrawProc
                (param_1,ppFVar6[local_3e],param_3,
                 *(float *)(&param_1[7].moveBoundsRect.left + local_3e * 2) *
                 fVar1 * (float)((int)(short)uVar3 - (int)(short)uVar4) + param_4);
    }
    local_30 = fVar14;
    if (((char)param_1[0xb].frameAdvance != '\0') && (!bVar10)) {
      local_30 = (float)(&param_1[3].misc2)[local_3e] * fVar14;
    }
    param_3 = param_3 + local_30;
    local_2c = local_2c + local_30;
  }
  return (longdouble)local_2c;
}


// ---- _strchrUnicode @ 0x98fe9 (147 bytes)

int _strchrUnicode(char *param_1,short param_2)

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  int local_10;
  
  uVar2 = 0xffffffff;
  pcVar3 = param_1;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + 1;
  } while (cVar1 != '\0');
  for (local_10 = 0;
      (local_10 < (short)(~uVar2 - 1 >> 1) && (*(short *)(param_1 + local_10 * 2) != 0));
      local_10 = local_10 + 1) {
    if (*(short *)(param_1 + local_10 * 2) == param_2) {
      return (int)(short)local_10;
    }
  }
  return -1;
}


