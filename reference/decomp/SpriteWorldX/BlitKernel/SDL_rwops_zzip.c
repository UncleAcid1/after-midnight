// Ghidra decompilation of functions from original source file:
//   ../SpriteWorldX/BlitKernel/SDL_rwops_zzip.c

// ---- __zzip_seek @ 0x9907c (60 bytes)

void _zzip_seek(int param_1,int param_2,undefined4 param_3)

{
  _zzip_seek(*(undefined4 *)(param_1 + 0x14),param_2,param_2 >> 0x1f,param_3);
  return;
}


// ---- _read @ 0x990b8 (74 bytes)

/* WARNING: Restarted to delay deadcode elimination for space: stack */

int read(int param_1,undefined4 param_2,int param_3,int param_4)

{
  int iVar1;
  
  iVar1 = _zzip_read(*(undefined4 *)(param_1 + 0x14),param_2,param_3 * param_4);
  return iVar1 / param_3;
}


// ---- _write @ 0x99102 (19 bytes)

undefined4 write(void)

{
  return 0;
}


// ---- __zzip_close @ 0x99115 (64 bytes)

undefined4 _zzip_close(int param_1)

{
  if (param_1 != 0) {
    _zzip_close(*(undefined4 *)(param_1 + 0x14));
    _SDL_FreeRW(param_1);
  }
  return 0;
}


// ---- _SDL_RWFromZZIP @ 0x99155 (228 bytes)

undefined4 * _SDL_RWFromZZIP(undefined4 param_1,char *param_2)

{
  char *pcVar1;
  int iVar2;
  int *piVar3;
  undefined4 *local_18;
  
  pcVar1 = _strchr(param_2,0x72);
  if (pcVar1 == (char *)0x0) {
    local_18 = (undefined4 *)_SDL_RWFromFile(param_1,param_2);
  }
  else {
    iVar2 = _zzip_fopen(param_1,param_2);
    if (iVar2 == 0) {
      local_18 = (undefined4 *)0x0;
    }
    else {
      local_18 = (undefined4 *)_SDL_AllocRW();
      if (local_18 == (undefined4 *)0x0) {
        piVar3 = ___error();
        *piVar3 = 0xc;
        _zzip_close(iVar2);
        local_18 = (undefined4 *)0x0;
      }
      else {
        local_18[5] = iVar2;
        local_18[1] = PTR_read_001c5298;
        local_18[2] = PTR_write_001c528c;
        *local_18 = PTR__zzip_seek_001c5294;
        local_18[3] = PTR_LAB_001c5290;
      }
    }
  }
  return local_18;
}


