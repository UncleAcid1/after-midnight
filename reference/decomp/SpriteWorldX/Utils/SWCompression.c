// Ghidra decompilation of functions from original source file:
//   ../SpriteWorldX/Utils/SWCompression.c

// ---- _SWCompressInit @ 0x2b7a9 (79 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWCompressInit(void *newState)

{
  void *pvVar1;
  undefined4 local_20;
  
  *(undefined4 *)newState = 0;
  pvVar1 = _calloc(1,0xd440);
  if (pvVar1 == (void *)0x0) {
    local_20 = 200;
  }
  else {
    *(void **)newState = pvVar1;
    local_20 = 0;
  }
  return local_20;
}


// ---- _SWCompressExit @ 0x2b7f8 (48 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWCompressExit(void *oldState)

{
                    /* WARNING: Load size is inaccurate */
  if (*oldState != (void *)0x0) {
    _free(*oldState);
  }
  *(undefined4 *)oldState = 0;
  return;
}


// ---- _SWDecompressInit @ 0x2b828 (79 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWDecompressInit(void *newState)

{
  void *pvVar1;
  undefined4 local_20;
  
  *(undefined4 *)newState = 0;
  pvVar1 = _calloc(1,0x1011);
  if (pvVar1 == (void *)0x0) {
    local_20 = 200;
  }
  else {
    *(void **)newState = pvVar1;
    local_20 = 0;
  }
  return local_20;
}


// ---- _SWDecompressExit @ 0x2b877 (48 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWDecompressExit(void *oldState)

{
                    /* WARNING: Load size is inaccurate */
  if (*oldState != (void *)0x0) {
    _free(*oldState);
  }
  *(undefined4 *)oldState = 0;
  return;
}


// ---- _SWCompress @ 0x2b8a7 (195 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int SWCompress(void *inData,int inLength,void *outData,int outLength)

{
  void *pvStack_20;
  void *pvStack_1c;
  undefined4 *puStack_18;
  int iStack_14;
  int iStack_10;
  
  pvStack_1c = (void *)_SDL_RWFromConstMem(inData,inLength);
  puStack_18 = (undefined4 *)_SDL_RWFromMem(outData,outLength);
  iStack_14 = (*(code *)*puStack_18)(puStack_18,0,1);
  _SWCompressInit(&pvStack_20);
  _SWCompress_RW(pvStack_20,pvStack_1c,puStack_18);
  _SWCompressExit(&pvStack_20);
  iStack_10 = (*(code *)*puStack_18)(puStack_18,0,1);
  iStack_10 = iStack_10 - iStack_14;
  _SDL_FreeRW(pvStack_1c);
  _SDL_FreeRW(puStack_18);
  return iStack_10;
}


// ---- _SWDecompress @ 0x2b96a (195 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int SWDecompress(void *inData,int inLength,void *outData,int outLength)

{
  void *pvStack_20;
  void *pvStack_1c;
  undefined4 *puStack_18;
  int iStack_14;
  int iStack_10;
  
  pvStack_1c = (void *)_SDL_RWFromConstMem(inData,inLength);
  puStack_18 = (undefined4 *)_SDL_RWFromMem(outData,outLength);
  iStack_14 = (*(code *)*puStack_18)(puStack_18,0,1);
  _SWDecompressInit(&pvStack_20);
  _SWDecompress_RW(pvStack_20,pvStack_1c,puStack_18);
  _SWDecompressExit(&pvStack_20);
  iStack_10 = (*(code *)*puStack_18)(puStack_18,0,1);
  iStack_10 = iStack_10 - iStack_14;
  _SDL_FreeRW(pvStack_1c);
  _SDL_FreeRW(puStack_18);
  return iStack_10;
}


// ---- _InitTree @ 0x2ba2d (94 bytes)

void _InitTree(int param_1)

{
  undefined4 local_10;
  
  for (local_10 = 0x1001; local_10 < 0x1101; local_10 = local_10 + 1) {
    *(undefined4 *)(param_1 + 0x5038 + local_10 * 4) = 0x1000;
  }
  for (local_10 = 0; local_10 < 0x1000; local_10 = local_10 + 1) {
    *(undefined4 *)(param_1 + 0x943c + local_10 * 4) = 0x1000;
  }
  return;
}


// ---- _InsertNode @ 0x2ba8b (620 bytes)

void _InsertNode(int param_1,int param_2)

{
  char *pcVar1;
  byte *pbVar2;
  int local_1c;
  int local_18;
  int local_14;
  
  local_14 = 1;
  pbVar2 = (byte *)(param_1 + 0x19 + param_2);
  local_18 = *pbVar2 + 0x1001;
  *(undefined4 *)(param_1 + 0x1034 + param_2 * 4) = 0x1000;
  *(undefined4 *)(param_1 + 0x5038 + param_2 * 4) = *(undefined4 *)(param_1 + 0x1034 + param_2 * 4);
  pcVar1 = segment_command_0000101c.segname + param_1 + 0xc;
  pcVar1[0] = '\0';
  pcVar1[1] = '\0';
  pcVar1[2] = '\0';
  pcVar1[3] = '\0';
  do {
    do {
      if (local_14 < 0) {
        if (*(int *)(param_1 + 0x1034 + local_18 * 4) == 0x1000) {
          *(int *)(param_1 + 0x1034 + local_18 * 4) = param_2;
          *(int *)(param_1 + 0x943c + param_2 * 4) = local_18;
          return;
        }
        local_18 = *(int *)(param_1 + 0x1034 + local_18 * 4);
      }
      else {
        if (*(int *)(param_1 + 0x5038 + local_18 * 4) == 0x1000) {
          *(int *)(param_1 + 0x5038 + local_18 * 4) = param_2;
          *(int *)(param_1 + 0x943c + param_2 * 4) = local_18;
          return;
        }
        local_18 = *(int *)(param_1 + 0x5038 + local_18 * 4);
      }
      local_1c = 1;
      while ((local_1c < 0x12 &&
             (local_14 = (uint)pbVar2[local_1c] -
                         (uint)*(byte *)(local_18 + local_1c + 0x19 + param_1), local_14 == 0))) {
        local_1c = local_1c + 1;
      }
    } while (local_1c <= *(int *)(segment_command_0000101c.segname + param_1 + 0xc));
    *(int *)(segment_command_0000101c.segname + param_1 + 8) = local_18;
    *(int *)(segment_command_0000101c.segname + param_1 + 0xc) = local_1c;
  } while (*(int *)(segment_command_0000101c.segname + param_1 + 0xc) < 0x12);
  *(undefined4 *)(param_1 + 0x943c + param_2 * 4) = *(undefined4 *)(param_1 + 0x943c + local_18 * 4)
  ;
  *(undefined4 *)(param_1 + 0x1034 + param_2 * 4) = *(undefined4 *)(param_1 + 0x1034 + local_18 * 4)
  ;
  *(undefined4 *)(param_1 + 0x5038 + param_2 * 4) = *(undefined4 *)(param_1 + 0x5038 + local_18 * 4)
  ;
  *(int *)(param_1 + 0x943c + *(int *)(param_1 + 0x1034 + local_18 * 4) * 4) = param_2;
  *(int *)(param_1 + 0x943c + *(int *)(param_1 + 0x5038 + local_18 * 4) * 4) = param_2;
  if (*(int *)(param_1 + 0x5038 + *(int *)(param_1 + 0x943c + local_18 * 4) * 4) == local_18) {
    *(int *)(param_1 + 0x5038 + *(int *)(param_1 + 0x943c + local_18 * 4) * 4) = param_2;
  }
  else {
    *(int *)(param_1 + 0x1034 + *(int *)(param_1 + 0x943c + local_18 * 4) * 4) = param_2;
  }
  *(undefined4 *)(param_1 + 0x943c + local_18 * 4) = 0x1000;
  return;
}


// ---- _DeleteNode @ 0x2bcf7 (497 bytes)

void _DeleteNode(int param_1,int param_2)

{
  undefined4 local_10;
  
  if (*(int *)(param_1 + 0x943c + param_2 * 4) != 0x1000) {
    if (*(int *)(param_1 + 0x5038 + param_2 * 4) == 0x1000) {
      local_10 = *(int *)(param_1 + 0x1034 + param_2 * 4);
    }
    else if (*(int *)(param_1 + 0x1034 + param_2 * 4) == 0x1000) {
      local_10 = *(int *)(param_1 + 0x5038 + param_2 * 4);
    }
    else {
      local_10 = *(int *)(param_1 + 0x1034 + param_2 * 4);
      if (*(int *)(param_1 + 0x5038 + local_10 * 4) != 0x1000) {
        do {
          local_10 = *(int *)(param_1 + 0x5038 + local_10 * 4);
        } while (*(int *)(param_1 + 0x5038 + local_10 * 4) != 0x1000);
        *(undefined4 *)(param_1 + 0x5038 + *(int *)(param_1 + 0x943c + local_10 * 4) * 4) =
             *(undefined4 *)(param_1 + 0x1034 + local_10 * 4);
        *(undefined4 *)(param_1 + 0x943c + *(int *)(param_1 + 0x1034 + local_10 * 4) * 4) =
             *(undefined4 *)(param_1 + 0x943c + local_10 * 4);
        *(undefined4 *)(param_1 + 0x1034 + local_10 * 4) =
             *(undefined4 *)(param_1 + 0x1034 + param_2 * 4);
        *(int *)(param_1 + 0x943c + *(int *)(param_1 + 0x1034 + param_2 * 4) * 4) = local_10;
      }
      *(undefined4 *)(param_1 + 0x5038 + local_10 * 4) =
           *(undefined4 *)(param_1 + 0x5038 + param_2 * 4);
      *(int *)(param_1 + 0x943c + *(int *)(param_1 + 0x5038 + param_2 * 4) * 4) = local_10;
    }
    *(undefined4 *)(param_1 + 0x943c + local_10 * 4) =
         *(undefined4 *)(param_1 + 0x943c + param_2 * 4);
    if (*(int *)(param_1 + 0x5038 + *(int *)(param_1 + 0x943c + param_2 * 4) * 4) == param_2) {
      *(int *)(param_1 + 0x5038 + *(int *)(param_1 + 0x943c + param_2 * 4) * 4) = local_10;
    }
    else {
      *(int *)(param_1 + 0x1034 + *(int *)(param_1 + 0x943c + param_2 * 4) * 4) = local_10;
    }
    *(undefined4 *)(param_1 + 0x943c + param_2 * 4) = 0x1000;
  }
  return;
}


// ---- _SDL_getc @ 0x2bee8 (78 bytes)

uint _SDL_getc(int param_1)

{
  int iVar1;
  uint local_20;
  byte local_d [9];
  
  iVar1 = (**(code **)(param_1 + 4))(param_1,local_d,1,1);
  if (iVar1 == 1) {
    local_20 = (uint)local_d[0];
  }
  else {
    local_20 = 0xffffffff;
  }
  return local_20;
}


// ---- _SDL_putc @ 0x2bf36 (84 bytes)

uint _SDL_putc(byte param_1,int param_2)

{
  int iVar1;
  uint local_20;
  byte local_d [9];
  
  local_d[0] = param_1;
  iVar1 = (**(code **)(param_2 + 8))(param_2,local_d,1,1);
  if (iVar1 == 1) {
    local_20 = (uint)local_d[0];
  }
  else {
    local_20 = 0xffffffff;
  }
  return local_20;
}


// ---- _SWCompress_RW @ 0x2bf8a (842 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWCompress_RW(void *state,void *in,void *out)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  int local_2c;
  int local_24;
  uint local_20;
  uint local_1c;
  int local_14;
  byte local_d;
  
  _InitTree(state);
  *(undefined1 *)((int)state + 8) = 0;
  local_d = 1;
  local_14 = 1;
  local_1c = 0;
  local_20 = 0xfee;
  for (local_2c = 0; local_2c < 0xfee; local_2c = local_2c + 1) {
    *(undefined1 *)(local_2c + 0x19 + (int)state) = 0;
  }
  local_24 = 0;
  while ((local_24 < 0x12 && (iVar2 = _SDL_getc(in), iVar2 != -1))) {
    *(char *)(local_24 + 0x1007 + (int)state) = (char)iVar2;
    local_24 = local_24 + 1;
  }
  *(int *)state = local_24;
                    /* WARNING: Load size is inaccurate */
  if (*state != 0) {
    for (local_2c = 1; local_2c < 0x13; local_2c = local_2c + 1) {
      _InsertNode(state,0xfee - local_2c);
    }
    _InsertNode(state,0xfee);
    do {
      if (local_24 < *(int *)((int)state + 0x1030)) {
        *(int *)((int)state + 0x1030) = local_24;
      }
      if (*(int *)((int)state + 0x1030) < 3) {
        *(undefined4 *)((int)state + 0x1030) = 1;
        *(byte *)((int)state + 8) = *(byte *)((int)state + 8) | local_d;
        *(undefined1 *)((int)state + local_14 + 8) = *(undefined1 *)(local_20 + 0x19 + (int)state);
        local_14 = local_14 + 1;
      }
      else {
        *(char *)((int)state + local_14 + 8) = (char)*(undefined4 *)((int)state + 0x102c);
        *(byte *)((int)state + local_14 + 9) =
             (char)*(undefined4 *)((int)state + 0x1030) - 3U |
             (byte)(*(int *)((int)state + 0x102c) >> 4) & 0xf0;
        local_14 = local_14 + 2;
      }
      local_d = local_d << 1;
      if (local_d == 0) {
        for (local_2c = 0; local_2c < local_14; local_2c = local_2c + 1) {
          _SDL_putc(*(undefined1 *)((int)state + local_2c + 8),out);
        }
        *(int *)((int)state + 4) = *(int *)((int)state + 4) + local_14;
        *(undefined1 *)((int)state + 8) = 0;
        local_d = 1;
        local_14 = 1;
      }
      iVar2 = *(int *)((int)state + 0x1030);
      local_2c = 0;
      while ((local_2c < iVar2 && (iVar3 = _SDL_getc(in), iVar3 != -1))) {
        _DeleteNode(state,local_1c);
        *(char *)(local_1c + 0x19 + (int)state) = (char)iVar3;
        if (local_1c < 0x11) {
          *(char *)(local_1c + 0x1019 + (int)state) = (char)iVar3;
        }
        local_1c = local_1c + 1 & 0xfff;
        local_20 = local_20 + 1 & 0xfff;
        _InsertNode(state,local_20);
        local_2c = local_2c + 1;
      }
      while (bVar1 = local_2c < iVar2, local_2c = local_2c + 1, bVar1) {
        _DeleteNode(state,local_1c);
        local_1c = local_1c + 1 & 0xfff;
        local_20 = local_20 + 1 & 0xfff;
        local_24 = local_24 + -1;
        if (local_24 != 0) {
          _InsertNode(state,local_20);
        }
      }
    } while (0 < local_24);
    if (1 < local_14) {
      for (local_2c = 0; local_2c < local_14; local_2c = local_2c + 1) {
        _SDL_putc(*(undefined1 *)((int)state + local_2c + 8),out);
      }
      *(int *)((int)state + 4) = *(int *)((int)state + 4) + local_14;
    }
  }
  return;
}


// ---- _SWDecompress_RW @ 0x2c2d4 (379 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWDecompress_RW(void *state,void *in,void *out)

{
  undefined1 uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int local_24;
  int local_1c;
  uint local_18;
  uint local_10;
  
  local_10 = 0;
  local_18 = 0xfee;
  for (local_24 = 0; local_24 < 0xfee; local_24 = local_24 + 1) {
    *(undefined1 *)((int)state + local_24) = 0;
  }
  while( true ) {
    while( true ) {
      local_10 = local_10 >> 1;
      if ((((byte)(local_10 >> 8) ^ 1) & 1) != 0) {
        iVar2 = _SDL_getc(in);
        if (iVar2 == -1) {
          return;
        }
        local_10 = CONCAT22((short)((uint)iVar2 >> 0x10),CONCAT11(0xff,(char)iVar2));
      }
      if ((local_10 & 1) == 0) break;
      iVar2 = _SDL_getc(in);
      if (iVar2 == -1) {
        return;
      }
      _SDL_putc(iVar2,out);
      *(char *)((int)state + local_18) = (char)iVar2;
      local_18 = local_18 + 1 & 0xfff;
    }
    uVar3 = _SDL_getc(in);
    if (uVar3 == 0xffffffff) {
      return;
    }
    uVar4 = _SDL_getc(in);
    if (uVar4 == 0xffffffff) break;
    for (local_1c = 0; local_1c <= (int)((uVar4 & 0xf) + 2); local_1c = local_1c + 1) {
      uVar1 = *(undefined1 *)((int)state + (local_1c + (uVar3 | (uVar4 & 0xf0) << 4) & 0xfff));
      _SDL_putc(uVar1,out);
      *(undefined1 *)((int)state + local_18) = uVar1;
      local_18 = local_18 + 1 & 0xfff;
    }
  }
  return;
}


