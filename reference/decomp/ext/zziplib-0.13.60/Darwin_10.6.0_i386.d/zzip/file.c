// Ghidra decompilation of functions from original source file:
//   /Users/vernjensen/zziplib-0.13.60/Darwin_10.6.0_i386.d/../zzip/file.c

// ---- _zzip_file_close @ 0xea3c8 (188 bytes)

undefined4 _zzip_file_close(int *param_1)

{
  int iVar1;
  void *pvVar2;
  undefined4 uVar3;
  undefined1 local_10 [4];
  
  iVar1 = *param_1;
  if (param_1[2] != 0) {
    _inflateEnd((z_streamp)(param_1 + 0xc));
  }
  if (*(int *)(iVar1 + 0xc) == 0) {
    *(undefined1 **)(iVar1 + 0xc) = local_10;
  }
  pvVar2 = (void *)param_1[9];
  if (pvVar2 != (void *)0x0) {
    if ((local_10 == *(undefined1 **)(iVar1 + 0xc)) && (*(int *)(iVar1 + 0x14) == 0)) {
      *(void **)(iVar1 + 0x14) = pvVar2;
    }
    else {
      _free(pvVar2);
    }
  }
  if (param_1 == *(int **)(iVar1 + 0x20)) {
    *(undefined4 *)(iVar1 + 0x20) = 0;
  }
  *(int *)(iVar1 + 8) = *(int *)(iVar1 + 8) + -1;
  _memset(param_1,0,0x6c);
  if ((local_10 == *(undefined1 **)(iVar1 + 0xc)) && (*(int *)(iVar1 + 0x10) == 0)) {
    *(int **)(iVar1 + 0x10) = param_1;
  }
  else {
    _free(param_1);
  }
  if (local_10 == *(undefined1 **)(iVar1 + 0xc)) {
    *(undefined4 *)(iVar1 + 0xc) = 0;
  }
  uVar3 = 0;
  if (*(int *)(iVar1 + 8) == 0) {
    uVar3 = _zzip_dir_close(iVar1);
  }
  return uVar3;
}


// ---- _strrchr_basename @ 0xea484 (42 bytes)

char * _strrchr_basename(char *param_1)

{
  char *pcVar1;
  
  pcVar1 = _strrchr(param_1,0x2f);
  if (pcVar1 != (char *)0x0) {
    param_1 = pcVar1 + 1;
  }
  return param_1;
}


// ---- _dirsep_basename @ 0xea4ae (42 bytes)

char * _dirsep_basename(char *param_1)

{
  char *pcVar1;
  
  pcVar1 = _strrchr(param_1,0x2f);
  if (pcVar1 != (char *)0x0) {
    param_1 = pcVar1 + 1;
  }
  return param_1;
}


// ---- _dirsep_strcasecmp @ 0xea4d8 (117 bytes)

int _dirsep_strcasecmp(byte *param_1,byte *param_2)

{
  uint uVar1;
  uint uVar2;
  
  while( true ) {
    uVar1 = (uint)*param_1;
    if ((*param_1 == 0) || (uVar2 = (uint)*param_2, *param_2 == 0)) {
      return uVar1 - *param_2;
    }
    if ((uVar1 & 0xe0) == 0x40) {
      uVar1 = (uint)"@abcdefghijklmnopqrstuvwxyz[/]^_"[uVar1 & 0x1f];
    }
    if ((uVar2 & 0xe0) == 0x40) {
      uVar2 = (uint)"@abcdefghijklmnopqrstuvwxyz[/]^_"[uVar2 & 0x1f];
    }
    if (uVar1 != uVar2) break;
    param_1 = param_1 + 1;
    param_2 = param_2 + 1;
  }
  return uVar1 - uVar2;
}


// ---- _zzip_fclose @ 0xea54d (76 bytes)

undefined4 _zzip_fclose(int *param_1)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  if (param_1 != (int *)0x0) {
    if (*param_1 != 0) {
      uVar1 = _zzip_file_close();
      return uVar1;
    }
    uVar1 = (**(code **)(param_1[0x1a] + 4))(param_1[1]);
    _free(param_1);
  }
  return uVar1;
}


// ---- _zzip_close @ 0xea599 (9 bytes)

void _zzip_close(void)

{
  _zzip_fclose();
  return;
}


// ---- _zzip_tell @ 0xea5a2 (77 bytes)

int _zzip_tell(int *param_1)

{
  int iVar1;
  
  iVar1 = -1;
  if (param_1 != (int *)0x0) {
    if (*param_1 == 0) {
      iVar1 = (**(code **)(param_1[0x1a] + 0xc))(param_1[1],0,0,1);
    }
    else {
      iVar1 = param_1[5] - param_1[3];
    }
  }
  return iVar1;
}


// ---- _zzip_file_open @ 0xea5ef (892 bytes)

undefined4 * _zzip_file_open(int *param_1,undefined4 param_2,uint param_3)

{
  undefined4 *puVar1;
  char *pcVar2;
  uint uVar3;
  int iVar4;
  void *pvVar5;
  int iVar6;
  int extraout_EDX;
  int extraout_EDX_00;
  undefined4 *puVar7;
  z_streamp pzVar8;
  longlong lVar9;
  undefined8 uVar10;
  undefined4 *local_3c;
  code *local_38;
  code *local_34;
  undefined1 local_20 [12];
  undefined4 uStack_14;
  
  uStack_14 = 0xea5fa;
  puVar7 = (undefined4 *)param_1[6];
  local_34 = _dirsep_basename;
  local_38 = _dirsep_strcasecmp;
  if ((param_3 & 0x1000) == 0) {
    local_38 = (code *)PTR_001c5318;
    local_34 = _strrchr_basename;
  }
  if (*param_1 + 1U < 2) {
    param_1[1] = 9;
    return (undefined4 *)0x0;
  }
  if (puVar7 == (undefined4 *)0x0) {
    param_1[1] = 2;
    return (undefined4 *)0x0;
  }
  uVar3 = param_3 >> 0xd & 1;
  if (uVar3 != 0) {
    param_2 = (*local_34)(param_2);
  }
  while( true ) {
    iVar4 = (int)puVar7 + 0x15;
    if ((char)uVar3 != '\0') {
      iVar4 = (*local_34)(iVar4);
    }
    iVar4 = (*local_38)(iVar4,param_2);
    if (iVar4 == 0) break;
    if (*(ushort *)(puVar7 + 4) == 0) {
      param_1[1] = -0x101c;
      return (undefined4 *)0x0;
    }
    puVar7 = (undefined4 *)((int)puVar7 + (uint)*(ushort *)(puVar7 + 4));
  }
  if ((*(char *)(puVar7 + 5) != '\0') && (iVar4 = -0x101d, *(char *)(puVar7 + 5) != '\b'))
  goto LAB_000ea953;
  if (param_1[3] == 0) {
    param_1[3] = (int)local_20;
  }
  if ((local_20 == (undefined1 *)param_1[3]) && (param_1[4] != 0)) {
    local_3c = (undefined4 *)param_1[4];
    param_1[4] = 0;
  }
  else {
    iVar4 = -0x1014;
    local_3c = (undefined4 *)_calloc(1,0x6c);
    if (local_3c == (undefined4 *)0x0) goto LAB_000ea953;
  }
  *local_3c = param_1;
  local_3c[0x1a] = param_1[0x10];
  param_1[2] = param_1[2] + 1;
  if ((local_20 == (undefined1 *)param_1[3]) && (param_1[5] != 0)) {
    local_3c[9] = param_1[5];
    param_1[5] = 0;
LAB_000ea737:
    if (local_20 == (undefined1 *)param_1[3]) {
      param_1[3] = 0;
    }
    puVar1 = (undefined4 *)param_1[8];
    if (puVar1 == (undefined4 *)0x0) {
LAB_000ea7ab:
      local_3c[10] = puVar7[3];
      local_3c[0xb] = 0;
      param_1[8] = (int)local_3c;
      (**(code **)(param_1[0x10] + 0xc))(*param_1,puVar7[3],0,0);
      if (-1 < extraout_EDX) {
        pcVar2 = (char *)local_3c[9];
        iVar4 = -0x1018;
        iVar6 = (**(code **)(param_1[0x10] + 8))(*param_1,pcVar2,0x1e);
        if (0x1d < iVar6) {
          if ((((*pcVar2 == 'P') && (pcVar2[1] == 'K')) && (pcVar2[2] == '\x03')) &&
             (pcVar2[3] == '\x04')) {
            (**(code **)(param_1[0x10] + 0xc))
                      (*param_1,(uint)*(ushort *)(pcVar2 + 0x1a) + (uint)*(ushort *)(pcVar2 + 0x1c),
                       0,1);
            if (extraout_EDX_00 < 0) goto LAB_000ea943;
            uVar10 = (**(code **)(param_1[0x10] + 0xc))(*param_1,0,0,1);
            *(undefined8 *)(local_3c + 7) = uVar10;
            local_3c[5] = *puVar7;
            local_3c[6] = puVar7[1];
            local_3c[2] = (uint)*(byte *)(puVar7 + 5);
            local_3c[3] = *puVar7;
            if (local_3c[2] == 0) {
              return local_3c;
            }
            pzVar8 = (z_streamp)(local_3c + 0xc);
            for (iVar4 = 0xe; iVar4 != 0; iVar4 = iVar4 + -1) {
              pzVar8->next_in = (Bytef *)0x0;
              pzVar8 = (z_streamp)&pzVar8->avail_in;
            }
            iVar4 = _inflateInit2_((z_streamp)(local_3c + 0xc),-0xf,"1.2.3",0x38);
            if (iVar4 == 0) {
              local_3c[4] = puVar7[1];
              return local_3c;
            }
            _zzip_file_close(local_3c);
          }
          else {
            iVar4 = -0x101f;
          }
        }
        goto LAB_000ea948;
      }
    }
    else {
      lVar9 = (**(code **)(puVar1[0x1a] + 0xc))(*(undefined4 *)*puVar1,0,0,1);
      if (-1 < lVar9) {
        *(longlong *)(puVar1 + 10) = lVar9;
        goto LAB_000ea7ab;
      }
    }
LAB_000ea943:
    iVar4 = -0x1017;
  }
  else {
    iVar4 = -0x1014;
    pvVar5 = _malloc(0x8000);
    local_3c[9] = pvVar5;
    if (pvVar5 != (void *)0x0) goto LAB_000ea737;
  }
LAB_000ea948:
  _zzip_file_close(local_3c);
LAB_000ea953:
  param_1[1] = iVar4;
  return (undefined4 *)0x0;
}


// ---- _zzip_open_shared_io @ 0xea96b (806 bytes)

void * _zzip_open_shared_io
                 (int *param_1,char *param_2,uint param_3,uint param_4,int param_5,
                 undefined4 *param_6)

{
  int iVar1;
  void *pvVar2;
  int *piVar3;
  size_t sVar4;
  int iVar5;
  char *pcVar6;
  undefined4 *local_424;
  char local_410 [1024];
  int local_10;
  
  if ((param_1 != (int *)0x0) && (iVar1 = *param_1, iVar1 != 0)) {
    if (param_5 == 0) {
      param_5 = *(int *)(iVar1 + 0x3c);
    }
    if (param_6 != (undefined4 *)0x0) goto LAB_000ea9af;
    param_6 = *(undefined4 **)(iVar1 + 0x40);
  }
  if (param_6 == (undefined4 *)0x0) {
    param_6 = (undefined4 *)_zzip_get_default_io();
  }
LAB_000ea9af:
  if ((param_4 & 0x14000) != 0) goto LAB_000eaa40;
  while( true ) {
    local_424 = param_6;
    if ((param_4 & 0x40000) != 0) {
      local_424 = (undefined4 *)_zzip_get_default_io();
    }
    iVar1 = (*(code *)*local_424)(param_2,param_3);
    if (iVar1 != -1) {
      pvVar2 = _calloc(1,0x6c);
      if (pvVar2 != (void *)0x0) {
        *(int *)((int)pvVar2 + 4) = iVar1;
        *(undefined4 **)((int)pvVar2 + 0x68) = local_424;
        return pvVar2;
      }
      (*(code *)local_424[1])(iVar1);
      return (void *)0x0;
    }
    if ((param_4 & 0x4000) != 0) {
      return (void *)0x0;
    }
LAB_000eaa40:
    if ((param_3 & 0x201) != 0) break;
    param_3 = param_3 & 0xfffffffd;
    sVar4 = _strlen(param_2);
    if (0x3ff < (int)sVar4) {
      piVar3 = ___error();
      *piVar3 = 0x3f;
      return (void *)0x0;
    }
    _memcpy(local_410,param_2,sVar4 + 1);
    if (((param_1 != (int *)0x0) && (iVar1 = *param_1, iVar1 != 0)) &&
       (pcVar6 = *(char **)(iVar1 + 0x38), pcVar6 != (char *)0x0)) {
      sVar4 = _strlen(pcVar6);
      iVar5 = _memcmp(param_2,pcVar6,sVar4);
      if (((iVar5 == 0) && (param_2[sVar4] == '/')) && (param_2[sVar4 + 1] != '\0')) {
        pvVar2 = (void *)_zzip_file_open(iVar1,param_2 + sVar4 + 1,param_4);
        if (pvVar2 == (void *)0x0) {
          iVar1 = _zzip_errno(*(undefined4 *)(*param_1 + 4));
          piVar3 = ___error();
          *piVar3 = iVar1;
          return (void *)0x0;
        }
        return pvVar2;
      }
    }
    while (pcVar6 = _strrchr(local_410,0x2f), pcVar6 != (char *)0x0) {
      local_10 = 0;
      *pcVar6 = '\0';
      iVar1 = ___zzip_try_open(local_410,param_3,param_5,param_6);
      if (iVar1 != -1) {
        iVar5 = _zzip_dir_fdopen_ext_io(iVar1,&local_10,param_5,param_6);
        if (local_10 != 0) {
          iVar5 = _zzip_errno(local_10);
          piVar3 = ___error();
          *piVar3 = iVar5;
          (*(code *)param_6[1])(iVar1);
          return (void *)0x0;
        }
        pvVar2 = (void *)_zzip_file_open(iVar5,param_2 + (int)(pcVar6 + (1 - (int)local_410)),
                                         param_4);
        if (pvVar2 == (void *)0x0) {
          iVar1 = _zzip_errno(*(undefined4 *)(iVar5 + 4));
          piVar3 = ___error();
          *piVar3 = iVar1;
        }
        else if (*(int *)(iVar5 + 0x38) == 0) {
          pcVar6 = _strdup(local_410);
          *(char **)(iVar5 + 0x38) = pcVar6;
        }
        _zzip_dir_close(iVar5);
        return pvVar2;
      }
    }
    if ((param_4 & 0x4000) == 0) {
      piVar3 = ___error();
      *piVar3 = 2;
      return (void *)0x0;
    }
  }
  piVar3 = ___error();
  *piVar3 = 0x16;
  return (void *)0x0;
}


// ---- _zzip_open_ext_io @ 0xeac91 (55 bytes)

void _zzip_open_ext_io(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                      undefined4 param_5)

{
  _zzip_open_shared_io(0,param_1,param_2,param_3,param_4,param_5);
  return;
}


// ---- _zzip_freopen @ 0xeacc8 (603 bytes)

undefined4 _zzip_freopen(undefined4 param_1,char *param_2,int param_3)

{
  uint uVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  
  uVar4 = 0x1b4;
  if (param_2 == (char *)0x0) {
    param_2 = "rb";
  }
  uVar3 = 0;
  do {
    if (*param_2 == '\0') {
      uVar2 = _zzip_open_shared_io(param_3,param_1,uVar3,uVar4,0,0);
      if ((((byte)(uVar4 >> 0x11) ^ 1) & param_3 != 0) != 0) {
        _zzip_file_close(param_3);
      }
      return uVar2;
    }
    switch(*param_2) {
    case '*':
      uVar4 = uVar4 | 0x2000;
      break;
    case 'g':
      uVar4 = uVar4 & 0xffffffc7;
      uVar1 = param_2[1] * 8 - 0x180U & 0x38;
      goto LAB_000eaea4;
    case 'i':
      uVar4 = uVar4 | 0x1000;
      break;
    case 'n':
      uVar3 = uVar3 | 4;
      break;
    case 'o':
      uVar4 = uVar4 & 0xfffffff8;
      uVar1 = (int)param_2[1] & 7;
      goto LAB_000eaea4;
    case 'q':
      uVar4 = uVar4 | 0x20000;
      break;
    case 'r':
      uVar3 = uVar3 | (uint)(param_2[1] == '+') * 2;
      break;
    case 's':
      uVar3 = uVar3 | 0x80;
      break;
    case 'u':
      uVar4 = uVar4 & 0xfffffe3f;
      uVar1 = ((int)param_2[1] & 7U) << 6;
LAB_000eaea4:
      uVar4 = uVar4 | uVar1;
      break;
    case 'w':
      uVar3 = uVar3 | (param_2[1] == '+') + 1 | 0x400;
      break;
    case 'x':
      uVar3 = uVar3 | 0x800;
    }
    param_2 = param_2 + 1;
  } while( true );
}


// ---- _zzip_fopen @ 0xeaf23 (34 bytes)

void _zzip_fopen(undefined4 param_1,undefined4 param_2)

{
  _zzip_freopen(param_1,param_2,0);
  return;
}


// ---- _zzip_open @ 0xeaf45 (81 bytes)

void _zzip_open(undefined4 param_1,uint param_2)

{
  uint uVar1;
  
  uVar1 = 0x1b4;
  if ((param_2 & 8) != 0) {
    param_2 = param_2 ^ 8;
    uVar1 = 0x11b4;
  }
  if ((param_2 & 0x400) != 0) {
    param_2 = param_2 ^ 0x400;
    uVar1 = uVar1 | 0x2000;
  }
  _zzip_open_shared_io(0,param_1,param_2,uVar1,0,0);
  return;
}


// ---- _zzip_file_read @ 0xeaf96 (434 bytes)

int _zzip_file_read(int *param_1,int param_2,uint param_3)

{
  undefined4 *puVar1;
  uint uVar2;
  int *piVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int extraout_EDX;
  longlong lVar7;
  
  if ((param_1 == (int *)0x0) || (puVar1 = (undefined4 *)*param_1, puVar1 == (undefined4 *)0x0)) {
    return 0;
  }
  uVar2 = param_1[3];
  if (uVar2 <= param_3) {
    param_3 = uVar2;
  }
  if (uVar2 == 0) {
    return 0;
  }
  piVar3 = (int *)puVar1[8];
  if (param_1 != piVar3) {
    if (piVar3 != (int *)0x0) {
      lVar7 = (**(code **)(piVar3[0x1a] + 0xc))(*(undefined4 *)*piVar3,0,0,1);
      if (lVar7 < 0) goto LAB_000eb031;
      *(longlong *)(piVar3 + 10) = lVar7;
    }
    (**(code **)(param_1[0x1a] + 0xc))(*puVar1,param_1[10],param_1[0xb],0);
    if (extraout_EDX < 0) {
LAB_000eb031:
      puVar1[1] = 0xffffefe9;
      return -1;
    }
    puVar1[8] = param_1;
  }
  if (param_1[2] != 0) {
    param_1[0x10] = param_3;
    param_1[0xf] = param_2;
    do {
      uVar2 = param_1[4];
      if ((uVar2 != 0) && (param_1[0xd] == 0)) {
        uVar4 = 0x8000;
        if (uVar2 < 0x8001) {
          uVar4 = uVar2;
        }
        iVar5 = (**(code **)(param_1[0x1a] + 8))(*puVar1,param_1[9],uVar4);
        if (iVar5 < 1) {
          puVar1[1] = 0xffffefe8;
          return -1;
        }
        param_1[4] = param_1[4] - iVar5;
        param_1[0xd] = iVar5;
        param_1[0xc] = param_1[9];
      }
      iVar5 = param_1[0x11];
      iVar6 = _inflate((z_streamp)(param_1 + 0xc),0);
      if (iVar6 == 1) {
        param_1[3] = 0;
      }
      else {
        if (iVar6 != 0) {
          puVar1[1] = iVar6;
          return -1;
        }
        param_1[3] = param_1[3] - (param_1[0x11] - iVar5);
      }
    } while ((param_1[3] != 0) && (param_1[0x10] != 0));
    return param_3 - param_1[0x10];
  }
  iVar5 = (**(code **)(param_1[0x1a] + 8))(*puVar1,param_2,param_3);
  if (0 < iVar5) {
    param_1[3] = param_1[3] - iVar5;
    return iVar5;
  }
  if (iVar5 < 0) {
    puVar1[1] = 0xffffefe8;
    return iVar5;
  }
  return iVar5;
}


// ---- _zzip_read @ 0xeb148 (109 bytes)

int _zzip_read(int *param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  
  iVar3 = 0;
  if (param_1 != (int *)0x0) {
    if (*param_1 == 0) {
                    /* WARNING: Could not recover jumptable at 0x000eb17c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      iVar3 = (**(code **)(param_1[0x1a] + 8))();
      return iVar3;
    }
    iVar3 = _zzip_file_read(param_1,param_2,param_3);
    if (iVar3 == -1) {
      iVar1 = _zzip_errno(*(undefined4 *)(*param_1 + 4));
      piVar2 = ___error();
      *piVar2 = iVar1;
    }
  }
  return iVar3;
}


// ---- _zzip_fread @ 0xeb1b5 (58 bytes)

uint _zzip_fread(undefined4 param_1,uint param_2,int param_3,undefined4 param_4)

{
  uint uVar1;
  
  if (param_2 == 0) {
    param_2 = 1;
  }
  uVar1 = _zzip_read(param_4,param_1,param_2 * param_3);
  return uVar1 / param_2;
}


// ---- _zzip_rewind @ 0xeb1ef (275 bytes)

int _zzip_rewind(int *param_1)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  int extraout_EDX;
  longlong lVar4;
  
  if (param_1 != (int *)0x0) {
    puVar1 = (undefined4 *)*param_1;
    if (puVar1 == (undefined4 *)0x0) {
      (**(code **)(param_1[0x1a] + 0xc))(param_1[1],0,0,0);
      return 0;
    }
    piVar2 = (int *)puVar1[8];
    if (param_1 != piVar2) {
      if (piVar2 != (int *)0x0) {
        lVar4 = (**(code **)(piVar2[0x1a] + 0xc))(*(undefined4 *)*piVar2,0,0,1);
        if (lVar4 < 0) {
          puVar1[1] = 0xffffefe9;
          return -1;
        }
        *(longlong *)(piVar2 + 10) = lVar4;
      }
      puVar1[8] = param_1;
    }
    (**(code **)(param_1[0x1a] + 0xc))(*puVar1,param_1[7],param_1[8],0);
    if (-1 < extraout_EDX) {
      param_1[0xb] = param_1[8];
      param_1[3] = param_1[5];
      param_1[10] = param_1[7];
      if (param_1[2] == 0) {
        return 0;
      }
      iVar3 = _inflateReset((z_streamp)(param_1 + 0xc));
      if (iVar3 != 0) {
        _zzip_file_close(param_1);
        return iVar3;
      }
      param_1[0xd] = 0;
      param_1[4] = param_1[6];
      return 0;
    }
  }
  return -1;
}


// ---- _zzip_seek @ 0xeb302 (726 bytes)

/* WARNING: Removing unreachable block (ram,0x000eb4d3) */

ulonglong _zzip_seek(int *param_1,uint param_2,int param_3,int param_4)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  void *pvVar4;
  uint uVar5;
  int extraout_EDX;
  bool bVar6;
  longlong lVar7;
  ulonglong uVar8;
  uint local_1c;
  uint local_14;
  int local_10;
  
  if (param_1 == (int *)0x0) {
    return 0xffffffffffffffff;
  }
  if (*param_1 == 0) {
    iVar3 = param_1[0x1a];
LAB_000eb5a8:
                    /* WARNING: Could not recover jumptable at 0x000eb5b5. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar8 = (**(code **)(iVar3 + 0xc))();
    return uVar8;
  }
  uVar5 = param_1[5];
  local_1c = uVar5 - param_1[3];
  if (param_4 != 1) {
    if (param_4 == 2) {
      bVar6 = CARRY4(uVar5,param_2);
      param_2 = uVar5 + param_2;
      param_3 = param_3 + (uint)bVar6;
    }
    else if (param_4 != 0) {
      return 0xffffffffffffffff;
    }
    bVar6 = param_2 < local_1c;
    param_2 = param_2 - local_1c;
    param_3 = param_3 - (uint)bVar6;
  }
  if (param_3 == 0 && param_2 == 0) {
    return (ulonglong)local_1c;
  }
  local_14 = param_2;
  local_10 = param_3;
  if (param_3 < 0) {
    iVar3 = _zzip_rewind(param_1);
    if (iVar3 == -1) {
      return 0xffffffffffffffff;
    }
    local_14 = param_2 + local_1c;
    local_10 = param_3 + (uint)CARRY4(param_2,local_1c);
    if (local_10 < 0) {
      return 0xffffffffffffffff;
    }
    local_1c = 0;
  }
  iVar3 = local_10 + (uint)CARRY4(local_14,local_1c);
  if (0 < iVar3) {
    return 0xffffffffffffffff;
  }
  if ((-1 < iVar3) && ((uint)param_1[5] < local_14 + local_1c)) {
    return 0xffffffffffffffff;
  }
  if (local_10 == 0 && local_14 == 0) {
    return (ulonglong)local_1c;
  }
  puVar1 = (undefined4 *)*param_1;
  piVar2 = (int *)puVar1[8];
  if (param_1 == piVar2) {
LAB_000eb47b:
    if (param_1[2] == 0) {
      (**(code **)(param_1[0x1a] + 0xc))(*puVar1,0,0,1);
      uVar8 = (**(code **)(param_1[0x1a] + 0xc))(*puVar1,local_14,local_10,1);
      if (0 < (longlong)uVar8) {
        iVar3 = (uint)uVar8 - param_1[7];
        param_1[3] = param_1[5] - iVar3;
        return CONCAT44(((int)(uVar8 >> 0x20) - param_1[8]) - (uint)((uint)uVar8 < (uint)param_1[7])
                        ,iVar3);
      }
      return uVar8;
    }
    pvVar4 = _malloc(0x8000);
    if (pvVar4 != (void *)0x0) {
      do {
        if (local_10 < 1) {
          if ((local_10 < 0) || (local_14 == 0)) {
            _free(pvVar4);
            if (*param_1 != 0) {
              return (ulonglong)(uint)(param_1[5] - param_1[3]);
            }
            iVar3 = param_1[0x1a];
            goto LAB_000eb5a8;
          }
          if ((0 < local_10) || ((uVar5 = local_14, -1 < local_10 && (0x7fff < local_14))))
          goto LAB_000eb530;
        }
        else {
LAB_000eb530:
          uVar5 = 0x8000;
        }
        uVar5 = _zzip_file_read(param_1,pvVar4,uVar5);
        iVar3 = (int)uVar5 >> 0x1f;
        if ((-1 < (int)uVar5 || iVar3 < 0) && ((iVar3 < 0 || (uVar5 == 0)))) {
          _free(pvVar4);
          return 0xffffffffffffffff;
        }
        bVar6 = local_14 < uVar5;
        local_14 = local_14 - uVar5;
        local_10 = (local_10 - iVar3) - (uint)bVar6;
      } while( true );
    }
  }
  else {
    if (piVar2 == (int *)0x0) {
LAB_000eb433:
      (**(code **)(param_1[0x1a] + 0xc))(*puVar1,param_1[10],param_1[0xb],0);
      if (-1 < extraout_EDX) {
        puVar1[8] = param_1;
        goto LAB_000eb47b;
      }
    }
    else {
      lVar7 = (**(code **)(piVar2[0x1a] + 0xc))(*(undefined4 *)*piVar2,0,0,1);
      if (-1 < lVar7) {
        *(longlong *)(piVar2 + 10) = lVar7;
        goto LAB_000eb433;
      }
    }
    puVar1[1] = 0xffffefe9;
  }
  return 0xffffffffffffffff;
}


// ---- _zzip_seek32 @ 0xeb5d8 (118 bytes)

int _zzip_seek32(undefined4 param_1,int param_2,undefined4 param_3)

{
  int *piVar1;
  longlong lVar2;
  int local_10;
  
  lVar2 = _zzip_seek(param_1,param_2,param_2 >> 0x1f,param_3);
  local_10 = -1;
  if (-1 < lVar2) {
    local_10 = (int)lVar2;
    if ((int)lVar2 >> 0x1f != (int)((ulonglong)lVar2 >> 0x20)) {
      piVar1 = ___error();
      *piVar1 = 0x54;
      local_10 = -1;
    }
  }
  return local_10;
}


// ---- _zzip_tell32 @ 0xeb64e (132 bytes)

int _zzip_tell32(int *param_1)

{
  int *piVar1;
  ulonglong uVar2;
  int local_14;
  int local_10;
  
  if (param_1 != (int *)0x0) {
    if (*param_1 == 0) {
      uVar2 = (**(code **)(param_1[0x1a] + 0xc))(param_1[1],0,0,1);
    }
    else {
      uVar2 = (ulonglong)(uint)(param_1[5] - param_1[3]);
    }
    local_10 = (int)(uVar2 >> 0x20);
    local_14 = (int)uVar2;
    if (-1 < (longlong)uVar2) {
      if (local_10 == local_14 >> 0x1f) {
        return local_14;
      }
      piVar1 = ___error();
      *piVar1 = 0x54;
    }
  }
  return -1;
}


