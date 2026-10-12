// Ghidra decompilation of functions from original source file:
//   Sources & Headers/MMChecksumStuff.c

// ---- _CreateChecksumsForZIPFilesInPath @ 0x9a0d5 (403 bytes)

void CreateChecksumsForZIPFilesInPath(char *param_1)

{
  char cVar1;
  char *pcVar2;
  int iVar3;
  uint uVar4;
  char acStack_432 [50];
  char acStack_400 [500];
  char acStack_20c [500];
  int iStack_18;
  int iStack_14;
  int iStack_10;
  
  _strcpy(acStack_400,param_1);
  iStack_18 = _opendir(acStack_400);
  if (iStack_18 == 0) {
    _printf("ERROR: Directory not found by CreateChecksumsForZIPFilesInPath: %s\n",acStack_400);
    return;
  }
  do {
    do {
      do {
        iStack_14 = _readdir(iStack_18);
        if (iStack_14 == 0) {
          _closedir(iStack_18);
          return;
        }
      } while ((*(char *)(iStack_14 + 6) != '\b') ||
              ((pcVar2 = _strstr((char *)(iStack_14 + 8),".zip"), pcVar2 == (char *)0x0 &&
               (pcVar2 = _strstr((char *)(iStack_14 + 8),".ZIP"), pcVar2 == (char *)0x0))));
      iVar3 = iStack_14 + 8;
      _sprintf(acStack_20c,"%s%s",acStack_400,iVar3);
      iStack_10 = _GetChecksumForFileAtPath(acStack_20c,acStack_432,1);
    } while (iStack_10 != 0);
    _sprintf(acStack_20c,"%s Data",iStack_14 + 8,iVar3);
    uVar4 = 0xffffffff;
    pcVar2 = acStack_432;
    do {
      if (uVar4 == 0) break;
      uVar4 = uVar4 - 1;
      cVar1 = *pcVar2;
      pcVar2 = pcVar2 + 1;
    } while (cVar1 != '\0');
    _CreateChecksumFile(acStack_20c,acStack_432,~uVar4 - 1);
  } while( true );
}


// ---- _CreateChecksumForMansionsInFolder @ 0x9a268 (698 bytes)

void CreateChecksumForMansionsInFolder(char *param_1)

{
  char cVar1;
  char *pcVar2;
  int iVar3;
  uint uVar4;
  char acStack_468 [50];
  char acStack_436 [50];
  char acStack_404 [500];
  char acStack_210 [500];
  int iStack_1c;
  int iStack_18;
  short sStack_12;
  int iStack_10;
  
  _strcpy(acStack_210,param_1);
  iStack_1c = _opendir(acStack_210);
  if (iStack_1c == 0) {
    _printf("ERROR: Directory not found by CreateChecksumForMansionsInFolder: %s\n",acStack_210);
  }
  else {
LAB_0009a4f5:
    iStack_18 = _readdir(iStack_1c);
    if (iStack_18 != 0) {
      if (*(char *)(iStack_18 + 6) == '\b') {
        pcVar2 = _strstr((char *)(iStack_18 + 8),".zip");
        if (pcVar2 == (char *)0x0) {
          pcVar2 = _strstr((char *)(iStack_18 + 8),".ZIP");
          if (pcVar2 == (char *)0x0) goto LAB_0009a4f5;
        }
        iVar3 = iStack_18 + 8;
        _sprintf(acStack_404,"%s%s",acStack_210,iVar3);
        _printf("Creating checksum for ZIPped mansion: %s\n",acStack_404);
        iStack_10 = _GetChecksumForFileAtPath(acStack_404,acStack_436,1);
        if (iStack_10 == 0) {
          _sprintf(acStack_404,"%s Data",iStack_18 + 8,iVar3);
          uVar4 = 0xffffffff;
          pcVar2 = acStack_436;
          do {
            if (uVar4 == 0) break;
            uVar4 = uVar4 - 1;
            cVar1 = *pcVar2;
            pcVar2 = pcVar2 + 1;
          } while (cVar1 != '\0');
          _CreateChecksumFile(acStack_404,acStack_436,~uVar4 - 1);
        }
        goto LAB_0009a4f5;
      }
      if ((*(char *)(iStack_18 + 6) == '\x04') && (*(char *)(iStack_18 + 8) != '.')) {
        iVar3 = iStack_18 + 8;
        _sprintf(acStack_404,"%s%s/",acStack_210,iVar3);
        for (sStack_12 = 0; sStack_12 < 0x22; sStack_12 = sStack_12 + 1) {
          acStack_436[sStack_12] = '\0';
        }
        iStack_10 = _CreateChecksumForMansionFolder(acStack_404,acStack_436);
        if (iStack_10 == 0) {
          _formatMD5(acStack_436,acStack_468);
          _sprintf(acStack_404,"%s Data",iStack_18 + 8,iVar3);
          uVar4 = 0xffffffff;
          pcVar2 = acStack_468;
          do {
            if (uVar4 == 0) break;
            uVar4 = uVar4 - 1;
            cVar1 = *pcVar2;
            pcVar2 = pcVar2 + 1;
          } while (cVar1 != '\0');
          _CreateChecksumFile(acStack_404,acStack_468,~uVar4 - 1);
        }
      }
      goto LAB_0009a4f5;
    }
    _closedir(iStack_1c);
  }
  return;
}


// ---- _CreateChecksumForMansionFolder @ 0x9a522 (666 bytes)

undefined4 _CreateChecksumForMansionFolder(undefined4 param_1,int param_2)

{
  char *pcVar1;
  undefined4 local_250;
  char local_242 [50];
  char local_210 [500];
  int local_1c;
  int local_18;
  short local_12;
  undefined4 local_10;
  
  local_10 = 0;
  local_1c = _opendir(param_1);
  if (local_1c == 0) {
    _printf("ERROR: Directory not found by CreateChecksumForMansionFolder: %s\n",param_1);
    local_250 = 0xffffffff;
  }
  else {
    while (local_18 = _readdir(local_1c), local_18 != 0) {
      if (*(char *)(local_18 + 6) == '\b') {
        _sprintf(local_210,"%s%s",param_1,local_18 + 8);
        _GetChecksumForFileAtPath(local_210,local_242,0);
        for (local_12 = 0; local_12 < 0x10; local_12 = local_12 + 1) {
          *(char *)(local_12 + param_2) = *(char *)(local_12 + param_2) + local_242[local_12];
        }
      }
      else if ((((((*(char *)(local_18 + 6) == '\x04') && (*(char *)(local_18 + 8) != '.')) &&
                 (pcVar1 = _strstr((char *)(local_18 + 8),"\'PICT\'"), pcVar1 == (char *)0x0)) &&
                ((pcVar1 = _strstr((char *)(local_18 + 8),"\'Pict\'"), pcVar1 == (char *)0x0 &&
                 (pcVar1 = _strstr((char *)(local_18 + 8),"\'BGND\'"), pcVar1 == (char *)0x0)))) &&
               ((pcVar1 = _strstr((char *)(local_18 + 8),"\'TMAP\'"), pcVar1 == (char *)0x0 &&
                ((pcVar1 = _strstr((char *)(local_18 + 8),"\'TMAQ\'"), pcVar1 == (char *)0x0 &&
                 (pcVar1 = _strstr((char *)(local_18 + 8),"\'TMAR\'"), pcVar1 == (char *)0x0))))))
              && ((pcVar1 = _strstr((char *)(local_18 + 8),"\'TMAS\'"), pcVar1 == (char *)0x0 &&
                  ((pcVar1 = _strstr((char *)(local_18 + 8),"\'TMAV\'"), pcVar1 == (char *)0x0 &&
                   (pcVar1 = _strstr((char *)(local_18 + 8),"\'TEXT\'"), pcVar1 == (char *)0x0))))))
      {
        _sprintf(local_210,"%s%s/",param_1,local_18 + 8);
        local_10 = _CreateChecksumForMansionFolder(local_210,param_2);
      }
    }
    _closedir(local_1c);
    local_250 = local_10;
  }
  return local_250;
}


// ---- _GetChecksumForFileAtPath @ 0x9a7bc (320 bytes)

undefined4 _GetChecksumForFileAtPath(undefined4 param_1,undefined4 param_2,char param_3)

{
  undefined4 local_64;
  undefined1 local_4e [50];
  undefined4 *local_1c;
  size_t local_18;
  undefined4 local_14;
  void *local_10;
  
  local_10 = (void *)0x0;
  local_1c = (undefined4 *)_SDL_RWFromFile(param_1,"rb");
  if (local_1c == (undefined4 *)0x0) {
    local_64 = 0xffffffff;
  }
  else {
    local_18 = (*(code *)*local_1c)(local_1c,0,2);
    (*(code *)*local_1c)(local_1c,0,0);
    local_10 = _calloc(1,local_18);
    if (local_10 != (void *)0x0) {
      local_14 = (*(code *)local_1c[1])(local_1c,local_10,1,local_18);
    }
    if (param_3 == '\0') {
      _md5NoFormatting(local_10,local_14,param_2);
    }
    else {
      _md5(local_10,local_14,param_2);
    }
    _formatMD5(param_2,local_4e);
    _free(local_10);
    (*(code *)local_1c[3])(local_1c);
    local_64 = 0;
  }
  return local_64;
}


// ---- _CreateChecksumFile @ 0x9a8fc (199 bytes)

undefined4 _CreateChecksumFile(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 local_1c;
  int local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  local_10 = 0;
  local_14 = _GetMansionDataFolderPath(param_1);
  local_18 = _SDL_RWFromFile(local_14,"wb");
  if (local_18 == 0) {
    local_10 = 0xffffffff;
  }
  else {
    _ClearFileStickyError();
    local_1c = 100;
    _MyWriteDataToFile(local_18,4,&local_1c);
    _MyWriteDataToFile(local_18,4,&param_3);
    _MyWriteDataToFile(local_18,param_3,param_2);
    local_10 = _GetFileStickyError();
    (**(code **)(local_18 + 0xc))(local_18);
  }
  return local_10;
}


// ---- _GetChecksumFromFile @ 0x9a9c3 (215 bytes)

undefined4 _GetChecksumFromFile(undefined4 param_1,undefined4 param_2)

{
  undefined4 local_20;
  int local_1c;
  int local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  local_10 = 0;
  local_14 = _GetMansionDataFolderPath(param_1);
  local_18 = _SDL_RWFromFile(local_14,"rb");
  if (local_18 == 0) {
    local_10 = 0xffffffff;
  }
  else {
    _ClearFileStickyError();
    _MyReadDataFromFile(local_18,4,&local_1c);
    if (local_1c != 100) {
      return 0xfffffffe;
    }
    _MyReadDataFromFile(local_18,4,&local_20);
    _MyReadDataFromFile(local_18,local_20,param_2);
    local_10 = _GetFileStickyError();
    (**(code **)(local_18 + 0xc))(local_18);
  }
  return local_10;
}


// ---- _DoesMansionPassChecksum @ 0x9aa9a (586 bytes)

char _DoesMansionPassChecksum(char *param_1,undefined4 param_2)

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  char local_686 [498];
  char acStack_494 [502];
  char local_29e [500];
  char local_aa [50];
  undefined1 local_78 [50];
  char local_46 [50];
  short local_14;
  char local_11;
  int local_10;
  
  local_11 = '\0';
  _strcpy(acStack_494 + 2,param_1);
  uVar2 = 0xffffffff;
  pcVar3 = acStack_494 + 2;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + 1;
  } while (cVar1 != '\0');
  acStack_494[~uVar2] = '\0';
  pcVar3 = ".zip";
  _sprintf(local_29e,"%s%s",acStack_494 + 2,".zip");
  for (local_14 = 0; local_14 < 0x32; local_14 = local_14 + 1) {
    local_aa[local_14] = '\0';
  }
  local_10 = _GetChecksumForFileAtPath(local_29e,local_46,1);
  if (local_10 == 0) {
    _sprintf(local_686,"%s.zip Data",param_2,pcVar3);
    local_10 = _GetChecksumFromFile(local_686,local_aa);
  }
  else {
    _printf("Attempting to read checksum from data file for folder-based mansion: %s\n",param_2);
    for (local_14 = 0; local_14 < 0x22; local_14 = local_14 + 1) {
      local_78[local_14] = 0;
    }
    local_10 = _CreateChecksumForMansionFolder(param_1,local_78);
    if (local_10 == 0) {
      _formatMD5(local_78,local_46);
    }
    if (local_10 == 0) {
      _sprintf(local_686,"%s Data",param_2);
      local_10 = _GetChecksumFromFile(local_686,local_aa);
    }
  }
  if (local_10 == 0) {
    _printf("Checksum of mansion file/folder: %s\n",local_46);
    _printf("Checksum of data file: %s\n",local_aa);
    local_11 = '\x01';
    for (local_14 = 0; local_14 < 0x20; local_14 = local_14 + 1) {
      if (local_46[local_14] != local_aa[local_14]) {
        local_11 = '\0';
      }
    }
  }
  if (local_11 == '\0') {
    _puts("Checksum FAILED! They are not identical.");
  }
  else {
    _puts("Checksum passed!");
  }
  return local_11;
}


