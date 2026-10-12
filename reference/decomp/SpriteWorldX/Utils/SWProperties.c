// Ghidra decompilation of functions from original source file:
//   ../SpriteWorldX/Utils/SWProperties.c

// ---- _SWCreateProperties @ 0x2c44f (70 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError SWCreateProperties(void *newProperties)

{
  void *pvVar1;
  undefined4 uStack_20;
  
  pvVar1 = _calloc(1,8);
  *(void **)newProperties = pvVar1;
  if (pvVar1 == (void *)0x0) {
    uStack_20 = 200;
  }
  else {
    uStack_20 = 0;
  }
  return uStack_20;
}


// ---- _SWDisposeProperties @ 0x2c495 (118 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWDisposeProperties(void *oldProperties)

{
  void *pvVar1;
  undefined4 *puVar2;
  undefined4 *puStack_14;
  
                    /* WARNING: Load size is inaccurate */
  pvVar1 = *oldProperties;
  if (pvVar1 != (void *)0x0) {
    puStack_14 = *(undefined4 **)((int)pvVar1 + 4);
    while (puStack_14 != (undefined4 *)0x0) {
      _free((void *)puStack_14[1]);
      _free((void *)puStack_14[2]);
      puVar2 = (undefined4 *)*puStack_14;
      _free(puStack_14);
      puStack_14 = puVar2;
    }
    _free(pvVar1);
  }
  *(undefined4 *)oldProperties = 0;
  return;
}


// ---- _SWLoadProperties @ 0x2c50b (61 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError SWLoadProperties(void *properties,char *filename)

{
  void *ops;
  SWError SVar1;
  
  ops = (void *)_SDL_RWFromFile(filename,"r");
  SVar1 = _SWLoadProperties_RW(properties,ops);
  return SVar1;
}


// ---- _SWLoadProperties_RW @ 0x2c548 (1118 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWLoadProperties_RW(void *properties,void *ops)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  char *pcVar5;
  SWError local_80;
  char local_7a [82];
  char *local_28;
  char *local_24;
  char *local_20;
  char *local_1c;
  char *local_18;
  char *local_14;
  int local_10;
  
  local_14 = (char *)0x0;
  local_10 = 0;
  if ((properties == (void *)0x0) || (ops == (void *)0x0)) {
    local_80 = 999;
  }
  else {
LAB_0002c57c:
    do {
      do {
        local_28 = (char *)_SDL_RWgets(ops,local_7a + 2,0x50);
        if (local_28 == (char *)0x0) {
          if (local_14 != (char *)0x0) {
            _free(local_14);
          }
          (**(code **)((int)ops + 0xc))(ops);
          return 0;
        }
      } while ((*local_28 == '#') || (local_7a[2] == '/'));
      local_20 = _strchr(local_28,0x3b);
      if (local_20 != (char *)0x0) {
        *local_20 = '\0';
      }
      if (*local_28 == '[') {
        local_28 = local_28 + 1;
        uVar3 = 0xffffffff;
        pcVar5 = local_7a + 2;
        do {
          if (uVar3 == 0) break;
          uVar3 = uVar3 - 1;
          cVar1 = *pcVar5;
          pcVar5 = pcVar5 + 1;
        } while (cVar1 != '\0');
        local_1c = local_7a + ~uVar3;
        while ((iVar2 = ___istype((int)*local_1c,0x4000), iVar2 != 0 && (local_28 < local_1c))) {
          *local_1c = '\0';
          local_1c = local_1c + -1;
        }
        if (*local_1c == ']') {
          *local_1c = '\0';
          local_1c = local_1c + -1;
          while ((iVar2 = ___istype((int)*local_28,0x4000), iVar2 != 0 && (*local_28 != '\0'))) {
            local_28 = local_28 + 1;
          }
          while ((iVar2 = ___istype((int)*local_1c,0x4000), iVar2 != 0 && (local_28 < local_1c))) {
            *local_1c = '\0';
            local_1c = local_1c + -1;
          }
          if (local_14 != (char *)0x0) {
            _free(local_14);
          }
          local_14 = (char *)_local_strdup(local_28);
          goto LAB_0002c57c;
        }
      }
      local_24 = _strchr(local_7a + 2,0x3d);
      if (local_24 == (char *)0x0) goto LAB_0002c57c;
      *local_24 = '\0';
      local_24 = local_24 + 1;
      while ((iVar2 = ___istype((int)*local_28,0x4000), iVar2 != 0 && (*local_28 != '\0'))) {
        local_28 = local_28 + 1;
      }
      local_1c = local_24 + -2;
      while ((iVar2 = ___istype((int)*local_1c,0x4000), iVar2 != 0 && (local_28 < local_1c))) {
        *local_1c = '\0';
        local_1c = local_1c + -1;
      }
      if (local_1c < local_28) goto LAB_0002c57c;
      while ((iVar2 = ___istype((int)*local_24,0x4000), iVar2 != 0 && (*local_24 != '\0'))) {
        local_24 = local_24 + 1;
      }
      uVar3 = 0xffffffff;
      pcVar5 = local_24;
      do {
        if (uVar3 == 0) break;
        uVar3 = uVar3 - 1;
        cVar1 = *pcVar5;
        pcVar5 = pcVar5 + 1;
      } while (cVar1 != '\0');
      local_1c = local_24 + (~uVar3 - 2);
      while ((iVar2 = ___istype((int)*local_1c,0x4000), iVar2 != 0 && (local_24 < local_1c))) {
        *local_1c = '\0';
        local_1c = local_1c + -1;
      }
      if (local_1c < local_24) goto LAB_0002c57c;
      if (local_14 == (char *)0x0) {
        local_10 = _SWSetPropertyString(properties,local_28,local_24);
      }
      else {
        uVar3 = 0xffffffff;
        pcVar5 = local_14;
        do {
          if (uVar3 == 0) break;
          uVar3 = uVar3 - 1;
          cVar1 = *pcVar5;
          pcVar5 = pcVar5 + 1;
        } while (cVar1 != '\0');
        uVar4 = 0xffffffff;
        pcVar5 = local_28;
        do {
          if (uVar4 == 0) break;
          uVar4 = uVar4 - 1;
          cVar1 = *pcVar5;
          pcVar5 = pcVar5 + 1;
        } while (cVar1 != '\0');
        local_18 = (char *)_malloc(~uVar3 + ~uVar4);
        if (local_18 != (char *)0x0) {
          *local_18 = '\0';
          local_1c = local_18;
          _strcat(local_18,local_14);
          uVar3 = 0xffffffff;
          pcVar5 = local_14;
          do {
            if (uVar3 == 0) break;
            uVar3 = uVar3 - 1;
            cVar1 = *pcVar5;
            pcVar5 = pcVar5 + 1;
          } while (cVar1 != '\0');
          local_1c[~uVar3 - 1] = '/';
          local_1c = local_1c + (~uVar3 - 1) + 1;
          *local_1c = '\0';
          _strcat(local_1c,local_28);
          uVar3 = 0xffffffff;
          pcVar5 = local_28;
          do {
            if (uVar3 == 0) break;
            uVar3 = uVar3 - 1;
            cVar1 = *pcVar5;
            pcVar5 = pcVar5 + 1;
          } while (cVar1 != '\0');
          local_1c = local_1c + (~uVar3 - 1);
          *local_1c = '\0';
          local_10 = _SWSetPropertyString(properties,local_18,local_24);
          _free(local_18);
        }
      }
    } while (local_10 == 0);
    local_80 = local_10;
  }
  return local_80;
}


// ---- ___istype @ 0x2c9a6 (98 bytes)

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


// ---- _SWSaveProperties @ 0x2ca08 (61 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError SWSaveProperties(void *properties,char *filename)

{
  void *ops;
  SWError SVar1;
  
  ops = (void *)_SDL_RWFromFile(filename,"w");
  SVar1 = _SWSaveProperties_RW(properties,ops);
  return SVar1;
}


// ---- _SWSaveProperties_RW @ 0x2ca45 (487 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWSaveProperties_RW(void *properties,void *ops)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  char *pcVar4;
  SWError local_70;
  char local_6c;
  char acStack_6b [79];
  undefined4 *local_1c;
  char *local_18;
  char *local_14;
  char *local_10;
  
  if ((properties == (void *)0x0) || (ops == (void *)0x0)) {
    local_70 = 999;
  }
  else {
    local_18 = (char *)0x0;
    for (local_1c = *(undefined4 **)((int)properties + 4); local_1c != (undefined4 *)0x0;
        local_1c = (undefined4 *)*local_1c) {
      local_14 = (char *)local_1c[1];
      local_10 = _strrchr(local_14,0x2f);
      if (local_10 != (char *)0x0) {
        if (local_18 == (char *)0x0) {
LAB_0002cade:
          _snprintf(&local_6c,0x50,"[%s]\n",local_14);
          local_10[(int)(acStack_6b + -(int)local_14)] = ']';
          local_10[(int)(acStack_6b + (1 - (int)local_14))] = '\n';
          local_10[(int)(acStack_6b + (2 - (int)local_14))] = '\0';
          uVar3 = 0xffffffff;
          pcVar4 = &local_6c;
          do {
            if (uVar3 == 0) break;
            uVar3 = uVar3 - 1;
            cVar1 = *pcVar4;
            pcVar4 = pcVar4 + 1;
          } while (cVar1 != '\0');
          (**(code **)((int)ops + 8))(ops,&local_6c,1,~uVar3 - 1);
          local_18 = local_14;
        }
        else {
          iVar2 = _strncmp(local_14,local_18,(int)local_10 - (int)local_14);
          if (iVar2 != 0) goto LAB_0002cade;
        }
        local_14 = local_10 + 1;
      }
      _snprintf(&local_6c,0x50,"%s = %s\n",local_14,local_1c[2]);
      uVar3 = 0xffffffff;
      pcVar4 = &local_6c;
      do {
        if (uVar3 == 0) break;
        uVar3 = uVar3 - 1;
        cVar1 = *pcVar4;
        pcVar4 = pcVar4 + 1;
      } while (cVar1 != '\0');
      (**(code **)((int)ops + 8))(ops,&local_6c,1,~uVar3 - 1);
    }
    (**(code **)((int)ops + 0xc))(ops);
    local_70 = 0;
  }
  return local_70;
}


// ---- _SWGetPropertyString @ 0x2cc2c (96 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * _SWGetPropertyString(void *properties,char *name,char *defaultValue)

{
  int iVar1;
  undefined4 *local_14;
  
  if ((properties != (void *)0x0) && (name != (char *)0x0)) {
    for (local_14 = *(undefined4 **)((int)properties + 4); local_14 != (undefined4 *)0x0;
        local_14 = (undefined4 *)*local_14) {
      iVar1 = _strcmp(name,(char *)local_14[1]);
      if (iVar1 == 0) {
        return (char *)local_14[2];
      }
    }
  }
  return defaultValue;
}


// ---- _SWGetPropertyInteger @ 0x2cc8c (90 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int SWGetPropertyInteger(void *properties,char *name,int defaultValue)

{
  char *pcVar1;
  undefined4 uStack_20;
  
  pcVar1 = _SWGetPropertyString(properties,name,(char *)0x0);
  if (pcVar1 == (char *)0x0) {
    uStack_20 = defaultValue;
  }
  else {
    uStack_20 = _strtol(pcVar1,(char **)0x0,10);
  }
  return uStack_20;
}


// ---- _SWGetPropertyFloat @ 0x2cce6 (102 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

float SWGetPropertyFloat(void *properties,char *name,float defaultValue)

{
  char *pcVar1;
  undefined4 uStack_20;
  
  pcVar1 = _SWGetPropertyString(properties,name,(char *)0x0);
  if (pcVar1 == (char *)0x0) {
    uStack_20 = defaultValue;
  }
  else {
    uStack_20 = _strtof(pcVar1,(char **)0x0);
  }
  return uStack_20;
}


// ---- _SWGetPropertyBoolean @ 0x2cd4c (110 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWBoolean SWGetPropertyBoolean(void *properties,char *name,SWBoolean defaultValue)

{
  char *pcVar1;
  int iVar2;
  
  pcVar1 = _SWGetPropertyString(properties,name,(char *)0x0);
  if (pcVar1 != (char *)0x0) {
    iVar2 = _strcmp(pcVar1,"true");
    defaultValue = iVar2 == 0;
  }
  return defaultValue;
}


// ---- _SWSetPropertyString @ 0x2cdba (285 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWSetPropertyString(void *properties,char *name,char *value)

{
  int iVar1;
  void *pvVar2;
  undefined4 uVar3;
  SWError local_20;
  undefined4 *local_14;
  undefined4 *local_10;
  
  if ((properties == (void *)0x0) || (name == (char *)0x0)) {
    local_20 = 999;
  }
  else {
    local_10 = (undefined4 *)0x0;
    for (local_14 = *(undefined4 **)((int)properties + 4); local_14 != (undefined4 *)0x0;
        local_14 = (undefined4 *)*local_14) {
      local_10 = local_14;
      iVar1 = _strcmp(name,(char *)local_14[1]);
      if (iVar1 == 0) {
        if (local_14[2] != 0) {
          _free((void *)local_14[2]);
        }
        uVar3 = _local_strdup(value);
        local_14[2] = uVar3;
        return 0;
      }
    }
    pvVar2 = _calloc(1,0xc);
    if (pvVar2 == (void *)0x0) {
      local_20 = 200;
    }
    else {
      uVar3 = _local_strdup(name);
      *(undefined4 *)((int)pvVar2 + 4) = uVar3;
      uVar3 = _local_strdup(value);
      *(undefined4 *)((int)pvVar2 + 8) = uVar3;
      if (local_10 == (undefined4 *)0x0) {
        *(void **)((int)properties + 4) = pvVar2;
      }
      else {
        *local_10 = pvVar2;
      }
                    /* WARNING: Load size is inaccurate */
      *(int *)properties = *properties + 1;
      local_20 = 0;
    }
  }
  return local_20;
}


// ---- _SWSetPropertyInteger @ 0x2ced7 (86 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError SWSetPropertyInteger(void *properties,char *name,int value)

{
  SWError SVar1;
  char acStack_1c [20];
  
  _snprintf(acStack_1c,0x10,"%d",value);
  SVar1 = _SWSetPropertyString(properties,name,acStack_1c);
  return SVar1;
}


// ---- _SWSetPropertyFloat @ 0x2cf2d (102 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError SWSetPropertyFloat(void *properties,char *name,float value)

{
  SWError SVar1;
  char acStack_10c [260];
  
  _snprintf(acStack_10c,0x100,"%f",(double)value);
  SVar1 = _SWSetPropertyString(properties,name,acStack_10c);
  return SVar1;
}


// ---- _SWSetPropertyBoolean @ 0x2cf93 (88 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError SWSetPropertyBoolean(void *properties,char *name,SWBoolean value)

{
  SWError SVar1;
  char *pcStack_24;
  
  if (value == '\0') {
    pcStack_24 = "false";
  }
  else {
    pcStack_24 = "true";
  }
  SVar1 = _SWSetPropertyString(properties,name,pcStack_24);
  return SVar1;
}


// ---- _local_strdup @ 0x2cfeb (85 bytes)

char * _local_strdup(char *param_1)

{
  char cVar1;
  char *pcVar2;
  uint uVar3;
  
  uVar3 = 0xffffffff;
  pcVar2 = param_1;
  do {
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    cVar1 = *pcVar2;
    pcVar2 = pcVar2 + 1;
  } while (cVar1 != '\0');
  pcVar2 = (char *)_malloc(~uVar3);
  if (pcVar2 != (char *)0x0) {
    _strcpy(pcVar2,param_1);
  }
  return pcVar2;
}


// ---- _SDL_RWgets @ 0x2d040 (143 bytes)

int _SDL_RWgets(int param_1,int param_2,int param_3)

{
  int iVar1;
  undefined4 local_20;
  undefined4 local_10;
  
  local_10 = 0;
  while (local_10 < param_3 + -1) {
    iVar1 = (**(code **)(param_1 + 4))(param_1,local_10 + param_2,1,1);
    if (((iVar1 != 1) || (local_10 = local_10 + 1, *(char *)(local_10 + param_2 + -1) == '\r')) ||
       (*(char *)(local_10 + param_2 + -1) == '\n')) break;
  }
  *(undefined1 *)(local_10 + param_2) = 0;
  if (local_10 < 1) {
    local_20 = 0;
  }
  else {
    local_20 = param_2;
  }
  return local_20;
}


