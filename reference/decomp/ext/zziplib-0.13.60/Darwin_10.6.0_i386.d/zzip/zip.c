// Ghidra decompilation of functions from original source file:
//   /Users/vernjensen/zziplib-0.13.60/Darwin_10.6.0_i386.d/../zzip/zip.c

// ---- ___zzip_fetch_disk_trailer @ 0xeb77b (831 bytes)

undefined4
___zzip_fetch_disk_trailer
          (undefined4 param_1,uint param_2,int param_3,undefined4 *param_4,int param_5)

{
  ushort uVar1;
  byte bVar2;
  undefined4 uVar3;
  uint uVar4;
  char *pcVar5;
  int iVar6;
  int extraout_EDX;
  bool bVar7;
  uint local_41c;
  int local_418;
  undefined4 local_410;
  char local_40c [1024];
  
  uVar3 = 0x16;
  if (param_4 != (undefined4 *)0x0) {
    if ((param_3 < 1) && ((param_3 < 0 || (param_2 < 0x16)))) {
      uVar3 = 0xffffefe7;
    }
    else {
      local_41c = param_2;
      local_418 = param_3;
      while( true ) {
        iVar6 = (param_3 - local_418) - (uint)(param_2 < local_41c);
        if ((0 < iVar6) || ((-1 < iVar6 && (0x10000 < param_2 - local_41c)))) break;
        bVar2 = 1;
        if ((param_3 < 1) && ((param_3 < 0 || (param_2 < 0x201)))) {
          bVar2 = 0;
        }
        if ((bool)(bVar2 & (local_41c == param_2 && local_418 == param_3))) {
          bVar7 = 0x1ff < local_41c;
          local_41c = local_41c - 0x200;
          local_418 = local_418 + -1 + (uint)bVar7;
        }
        local_410 = local_41c;
        if ((local_418 < 1) && ((local_418 < 0 || (local_41c < 0x200)))) {
          local_418 = 0;
          local_41c = 0;
          local_410 = local_410 + 0x200;
        }
        else {
          bVar7 = 0x1ff < local_41c;
          local_41c = local_41c - 0x200;
          local_418 = local_418 + -1 + (uint)bVar7;
          uVar4 = local_41c & 0x1ff;
          local_410 = 0x400;
          if (uVar4 != 0) {
            local_410 = 0x200 - uVar4;
            bVar7 = CARRY4(local_41c,local_410);
            local_41c = local_41c + local_410;
            local_418 = (local_418 - (uint)(0x200 < uVar4)) + (uint)bVar7;
            local_410 = 0x400 - local_410;
          }
        }
        iVar6 = ((int)local_410 >> 0x1f) + local_418 + (uint)CARRY4(local_410,local_41c);
        if ((param_3 <= iVar6) && ((param_3 < iVar6 || (param_2 < local_410 + local_41c)))) {
          local_410 = param_2 - local_41c;
        }
        (**(code **)(param_5 + 0xc))(param_1,local_41c,local_418,0);
        if (extraout_EDX < 0) {
          return 0xffffefe9;
        }
        iVar6 = (**(code **)(param_5 + 8))(param_1,local_40c,local_410);
        if (iVar6 < (int)local_410) {
          return 0xffffefe8;
        }
        iVar6 = 1;
        for (pcVar5 = local_40c + (local_410 - 1); local_40c <= pcVar5; pcVar5 = pcVar5 + -1) {
          if ((*pcVar5 == 'P') && (0x13 < iVar6)) {
            if ((pcVar5[1] == 'K') && ((pcVar5[2] == '\x05' && (pcVar5[3] == '\x06')))) {
              param_4[2] = (uint)*(ushort *)(pcVar5 + 8);
              param_4[3] = 0;
              uVar1 = *(ushort *)(pcVar5 + 10);
              param_4[5] = 0;
              param_4[4] = (uint)uVar1;
              uVar3 = *(undefined4 *)(pcVar5 + 0x10);
              param_4[7] = 0;
              param_4[6] = uVar3;
              uVar3 = *(undefined4 *)(pcVar5 + 0xc);
              param_4[9] = 0;
              param_4[8] = uVar3;
              *param_4 = pcVar5 + (local_41c - (int)local_40c);
              param_4[1] = (int)(pcVar5 + (local_41c - (int)local_40c)) >> 0x1f;
              return 0;
            }
            if ((((0x35 < iVar6) && (pcVar5[1] == 'K')) && (pcVar5[2] == '\x06')) &&
               (pcVar5[3] == '\x06')) {
              *param_4 = pcVar5;
              iVar6 = *(int *)(pcVar5 + 0x18);
              param_4[3] = iVar6 >> 0x1f;
              param_4[2] = iVar6;
              iVar6 = *(int *)(pcVar5 + 0x20);
              param_4[4] = iVar6;
              param_4[5] = iVar6 >> 0x1f;
              uVar3 = *(undefined4 *)(pcVar5 + 0x34);
              param_4[6] = *(undefined4 *)(pcVar5 + 0x30);
              param_4[7] = uVar3;
              uVar3 = *(undefined4 *)(pcVar5 + 0x2c);
              param_4[8] = *(undefined4 *)(pcVar5 + 0x28);
              param_4[9] = uVar3;
              return 0;
            }
          }
          iVar6 = iVar6 + 1;
        }
        if ((local_418 < 0) || ((local_418 < 1 && (local_41c == 0)))) break;
      }
      uVar3 = 0xffffefe6;
    }
  }
  return uVar3;
}


// ---- ___zzip_parse_root_directory @ 0xebaba (643 bytes)

undefined4
___zzip_parse_root_directory(undefined4 param_1,uint *param_2,undefined4 *param_3,int param_4)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  ushort uVar5;
  ushort uVar6;
  ushort uVar7;
  uint uVar8;
  undefined4 *puVar9;
  int iVar10;
  undefined4 *puVar11;
  undefined4 uVar12;
  uint uVar13;
  int extraout_EDX;
  bool bVar14;
  undefined4 *local_6c;
  undefined4 *local_68;
  uint local_64;
  uint local_60;
  uint local_5c;
  int local_58;
  uint local_4c;
  uint local_48;
  char local_3a;
  char local_39;
  char local_38;
  char local_37;
  undefined1 local_30;
  undefined4 local_2a;
  undefined4 local_26;
  undefined4 local_22;
  ushort local_1e;
  ushort local_1c;
  ushort local_1a;
  undefined4 local_10;
  
  uVar1 = param_2[2];
  uVar2 = param_2[3];
  local_4c = param_2[6];
  local_48 = param_2[7];
  uVar3 = param_2[8];
  uVar4 = param_2[9];
  uVar8 = *param_2 - uVar3;
  uVar13 = (param_2[1] - uVar4) - (uint)(*param_2 < uVar3);
  if (((int)uVar13 <= (int)local_48) && (((int)uVar13 < (int)local_48 || (uVar8 < local_4c)))) {
    local_4c = uVar8;
    local_48 = uVar13;
  }
  puVar9 = (undefined4 *)_malloc(uVar3);
  uVar12 = 0xffffefe5;
  if (puVar9 != (undefined4 *)0x0) {
    local_68 = (undefined4 *)0x0;
    local_64 = 0;
    local_60 = 0;
    local_5c = 0;
    local_58 = 0;
    local_6c = puVar9;
    while( true ) {
      if (((int)uVar2 < (int)local_60) || (((int)uVar2 <= (int)local_60 && (uVar1 <= local_64))))
      goto LAB_000ebcaf;
      (**(code **)(param_4 + 0xc))
                (param_1,local_4c + local_5c,local_48 + local_58 + (uint)CARRY4(local_4c,local_5c),0
                );
      if (extraout_EDX < 0) {
        return 0xffffefe9;
      }
      iVar10 = (**(code **)(param_4 + 8))(param_1,&local_3a,0x2e);
      uVar7 = local_1a;
      uVar6 = local_1c;
      uVar5 = local_1e;
      if (iVar10 < 0x2e) {
        return 0xffffefe8;
      }
      iVar10 = local_58 + (uint)(0xffffffd1 < local_5c);
      if (((((int)uVar4 < iVar10) ||
           (((((int)uVar4 <= iVar10 && (uVar3 < local_5c + 0x2e)) || (iVar10 < 0)) ||
            ((local_3a != 'P' || (local_39 != 'K')))))) || (local_38 != '\x01')) ||
         (local_37 != '\x02')) goto LAB_000ebcaf;
      local_6c[2] = local_2a;
      local_6c[1] = local_26;
      *local_6c = local_22;
      local_6c[3] = local_10;
      *(undefined1 *)(local_6c + 5) = local_30;
      uVar8 = local_1e + local_5c;
      iVar10 = local_58 + (uint)CARRY4((uint)local_1e,local_5c) + (uint)(0xffffffd1 < uVar8);
      if ((((int)uVar4 < iVar10) || (((int)uVar4 <= iVar10 && (uVar3 < uVar8 + 0x2e)))) ||
         (iVar10 < 0)) goto LAB_000ebcaf;
      uVar13 = (uint)local_1e;
      (**(code **)(param_4 + 8))(param_1,(int)local_6c + 0x15,uVar13);
      *(undefined1 *)(uVar13 + 0x15 + (int)local_6c) = 0;
      *(ushort *)((int)local_6c + 0x12) = uVar5;
      uVar8 = uVar6 + uVar13 + 0x2e + (uint)uVar7;
      bVar14 = CARRY4(local_5c,uVar8);
      local_5c = local_5c + uVar8;
      local_58 = local_58 + (uint)bVar14;
      if (((int)uVar4 <= local_58) && (((int)uVar4 < local_58 || (uVar3 < local_5c)))) break;
      local_68 = local_6c + 4;
      uVar8 = (int)local_6c + uVar13 + 0x19;
      uVar8 = uVar8 + (uVar8 & 1);
      puVar11 = (undefined4 *)(uVar8 + (uVar8 & 2));
      *(short *)(local_6c + 4) = (short)puVar11 - (short)local_6c;
      bVar14 = 0xfffffffe < local_64;
      local_64 = local_64 + 1;
      local_60 = local_60 + bVar14;
      local_6c = puVar11;
    }
    bVar14 = 0xfffffffe < local_64;
    local_64 = local_64 + 1;
    local_60 = local_60 + bVar14;
LAB_000ebcaf:
    if ((local_68 != (undefined4 *)0x0) &&
       (*(undefined2 *)local_68 = 0, param_3 != (undefined4 *)0x0)) {
      *param_3 = puVar9;
    }
    uVar12 = 0xffffefe1;
    if (local_60 == uVar2 && local_64 == uVar1) {
      uVar12 = 0;
    }
  }
  return uVar12;
}


// ---- _zzip_dir_free @ 0xebd3d (114 bytes)

int _zzip_dir_free(int *param_1)

{
  int iVar1;
  
  iVar1 = param_1[2];
  if (iVar1 == 0) {
    if (-1 < *param_1) {
      (**(code **)(param_1[0x10] + 4))(*param_1);
    }
    if ((void *)param_1[6] != (void *)0x0) {
      _free((void *)param_1[6]);
    }
    if (param_1[4] != 0) {
      _free((void *)param_1[4]);
    }
    if (param_1[5] != 0) {
      _free((void *)param_1[5]);
    }
    if ((void *)param_1[0xe] != (void *)0x0) {
      _free((void *)param_1[0xe]);
    }
    _free(param_1);
    iVar1 = 0;
  }
  return iVar1;
}


// ---- _zzip_dir_close @ 0xebdaf (22 bytes)

void _zzip_dir_close(int param_1)

{
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xefffffff;
  _zzip_dir_free();
  return;
}


// ---- _zzip_dir_read @ 0xebdc5 (107 bytes)

undefined4 _zzip_dir_read(int param_1,uint *param_2)

{
  ushort uVar1;
  undefined4 uVar2;
  
  if ((param_1 == 0) || (*(int *)(param_1 + 0x1c) == 0 || param_2 == (uint *)0x0)) {
    uVar2 = 0;
  }
  else {
    *param_2 = (uint)*(byte *)(*(int *)(param_1 + 0x1c) + 0x14);
    param_2[1] = *(uint *)(*(int *)(param_1 + 0x1c) + 4);
    param_2[2] = **(uint **)(param_1 + 0x1c);
    param_2[3] = *(int *)(param_1 + 0x1c) + 0x15;
    uVar1 = *(ushort *)(*(int *)(param_1 + 0x1c) + 0x10);
    if (uVar1 == 0) {
      *(undefined4 *)(param_1 + 0x1c) = 0;
    }
    else {
      *(uint *)(param_1 + 0x1c) = *(int *)(param_1 + 0x1c) + (uint)uVar1;
    }
    uVar2 = 1;
  }
  return uVar2;
}


// ---- _zzip_dir_alloc_ext_io @ 0xebe30 (98 bytes)

void * _zzip_dir_alloc_ext_io(undefined **param_1,int param_2)

{
  void *pvVar1;
  
  pvVar1 = _calloc(1,0x44);
  if (pvVar1 != (void *)0x0) {
    if (param_1 == (undefined **)0x0) {
      param_1 = &_ext_4303;
    }
    *(undefined ***)((int)pvVar1 + 0x3c) = param_1;
    if (param_2 == 0) {
      param_2 = _zzip_get_default_io();
    }
    *(int *)((int)pvVar1 + 0x40) = param_2;
  }
  return pvVar1;
}


// ---- _zzip_dir_fdopen_ext_io @ 0xebe92 (218 bytes)

undefined4 *
_zzip_dir_fdopen_ext_io(undefined4 param_1,int *param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 *puVar1;
  int iVar2;
  longlong lVar3;
  undefined1 local_34 [40];
  
  puVar1 = (undefined4 *)_zzip_dir_alloc_ext_io(param_3,param_4);
  if (puVar1 == (undefined4 *)0x0) {
    iVar2 = -0x1014;
  }
  else {
    *puVar1 = param_1;
    lVar3 = (**(code **)(puVar1[0x10] + 0x10))(param_1);
    if (lVar3 < 0) {
      iVar2 = -0x1016;
    }
    else {
      iVar2 = ___zzip_fetch_disk_trailer(*puVar1,lVar3,local_34,puVar1[0x10]);
      if ((iVar2 == 0) &&
         (iVar2 = ___zzip_parse_root_directory(*puVar1,local_34,puVar1 + 6,puVar1[0x10]), iVar2 == 0
         )) {
        puVar1[2] = puVar1[2] | 0x10000000;
        puVar1[7] = puVar1[6];
        if (param_2 == (int *)0x0) {
          return puVar1;
        }
        *param_2 = 0;
        return puVar1;
      }
    }
    _zzip_dir_free(puVar1);
  }
  if (param_2 != (int *)0x0) {
    *param_2 = iVar2;
  }
  return (undefined4 *)0x0;
}


// ---- _zzip_dir_fdopen @ 0xebf6c (42 bytes)

void _zzip_dir_fdopen(undefined4 param_1,undefined4 param_2)

{
  _zzip_dir_fdopen_ext_io(param_1,param_2,0,0);
  return;
}


// ---- _zzip_dir_alloc @ 0xebf96 (27 bytes)

void _zzip_dir_alloc(undefined4 param_1)

{
  _zzip_dir_alloc_ext_io(param_1,0);
  return;
}


// ---- ___zzip_try_open @ 0xebfb1 (189 bytes)

int ___zzip_try_open(char *param_1,undefined4 param_2,undefined **param_3,undefined4 *param_4)

{
  size_t sVar1;
  int *piVar2;
  int iVar3;
  char local_41c [1032];
  undefined4 uStack_14;
  
  uStack_14 = 0xebfbc;
  sVar1 = _strlen(param_1);
  if (sVar1 + 4 < 0x400) {
    _memcpy(local_41c,param_1,sVar1 + 1);
    if (param_4 == (undefined4 *)0x0) {
      param_4 = (undefined4 *)_zzip_get_default_io();
    }
    if (param_3 == (undefined **)0x0) {
      param_3 = &_ext_4303;
    }
    for (; *param_3 != (char *)0x0; param_3 = param_3 + 1) {
      _strcpy(local_41c + sVar1,*param_3);
      iVar3 = (*(code *)*param_4)(local_41c,param_2);
      if (iVar3 != -1) {
        return iVar3;
      }
    }
  }
  else {
    piVar2 = ___error();
    *piVar2 = 0x3f;
  }
  return -1;
}


// ---- _zzip_dir_open_ext_io @ 0xec06e (173 bytes)

undefined4
_zzip_dir_open_ext_io
          (undefined4 param_1,undefined4 *param_2,undefined **param_3,undefined4 *param_4)

{
  int iVar1;
  undefined4 uVar2;
  
  if (param_4 == (undefined4 *)0x0) {
    param_4 = (undefined4 *)_zzip_get_default_io();
  }
  if (param_3 == (undefined **)0x0) {
    param_3 = &_ext_4303;
  }
  iVar1 = (*(code *)*param_4)(param_1,0);
  if (iVar1 == -1) {
    iVar1 = ___zzip_try_open(param_1,0,param_3,param_4);
    if (iVar1 == -1) {
      if (param_2 != (undefined4 *)0x0) {
        *param_2 = 0xffffefeb;
      }
      return 0;
    }
  }
  uVar2 = _zzip_dir_fdopen_ext_io();
  return uVar2;
}


// ---- _zzip_dir_open @ 0xec11b (42 bytes)

void _zzip_dir_open(undefined4 param_1,undefined4 param_2)

{
  _zzip_dir_open_ext_io(param_1,param_2,0,0);
  return;
}


