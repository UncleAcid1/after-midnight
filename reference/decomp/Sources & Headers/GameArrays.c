// Ghidra decompilation of functions from original source file:
//   Sources & Headers/GameArrays.c

// ---- _InitObjectArray @ 0x6c099 (86 bytes)

void _InitObjectArray(void)

{
  int local_10;
  
  for (local_10 = 0; local_10 < 30000; local_10 = local_10 + 1) {
    *(undefined4 *)(PTR__gObjectArray_001c4e98 + local_10 * 4) = 0xffffffff;
  }
  *(undefined4 *)PTR__gNextFreeObjectIndex_001c4e74 = 0;
  *PTR__gArraysWereSaved_001c4e7c = 0;
  return;
}


// ---- _AddSpriteToObjectArray @ 0x6c0ef (31 bytes)

void _AddSpriteToObjectArray(int param_1)

{
  _AddObjectToArray(*(undefined4 *)(param_1 + 0xf8));
  return;
}


// ---- _RemoveSpriteFromObjectArray @ 0x6c10e (31 bytes)

void _RemoveSpriteFromObjectArray(int param_1)

{
  _RemoveObjectFromArray(*(undefined4 *)(param_1 + 0xf8));
  return;
}


// ---- _AddObjectToArray @ 0x6c12d (166 bytes)

void _AddObjectToArray(undefined4 param_1)

{
  int iVar1;
  char cVar2;
  
  cVar2 = _IsObjectInArray(param_1);
  if (cVar2 == '\0') {
    iVar1 = *(int *)PTR__gNextFreeObjectIndex_001c4e74;
    *(undefined4 *)(PTR__gObjectArray_001c4e98 + iVar1 * 4) = param_1;
    *(int *)PTR__gNextFreeObjectIndex_001c4e74 = iVar1 + 1;
    if (29999 < *(int *)PTR__gNextFreeObjectIndex_001c4e74) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameArrays.c",
                 PTR____FUNCTION___27452_001c4e8c,0x78);
    }
    if (29999 < *(int *)PTR__gNextFreeObjectIndex_001c4e74) {
      *(undefined4 *)PTR__gNextFreeObjectIndex_001c4e74 = 0;
    }
  }
  return;
}


// ---- _RemoveObjectFromArray @ 0x6c1d3 (194 bytes)

void _RemoveObjectFromArray(int param_1)

{
  char cVar1;
  int local_14;
  int local_10;
  
  cVar1 = _IsObjectInArray(param_1);
  if (cVar1 != '\0') {
    local_10 = *(int *)PTR__gNextFreeObjectIndex_001c4e74;
    for (local_14 = 0; local_14 < *(int *)PTR__gNextFreeObjectIndex_001c4e74;
        local_14 = local_14 + 1) {
      if (*(int *)(PTR__gObjectArray_001c4e98 + local_14 * 4) == param_1) {
        local_10 = local_14;
        break;
      }
    }
    for (local_14 = local_10; local_14 < *(int *)PTR__gNextFreeObjectIndex_001c4e74 + -1;
        local_14 = local_14 + 1) {
      *(undefined4 *)(PTR__gObjectArray_001c4e98 + local_14 * 4) =
           *(undefined4 *)(PTR__gObjectArray_001c4e98 + (local_14 + 1) * 4);
    }
    *(int *)PTR__gNextFreeObjectIndex_001c4e74 = *(int *)PTR__gNextFreeObjectIndex_001c4e74 + -1;
  }
  return;
}


// ---- _IsSpriteInObjectArray @ 0x6c295 (34 bytes)

undefined1 _IsSpriteInObjectArray(int param_1)

{
  undefined1 uVar1;
  
  uVar1 = _IsObjectInArray(*(undefined4 *)(param_1 + 0xf8));
  return uVar1;
}


// ---- _IsObjectInArray @ 0x6c2b7 (87 bytes)

undefined4 _IsObjectInArray(int param_1)

{
  int local_10;
  
  local_10 = 0;
  while( true ) {
    if (*(int *)PTR__gNextFreeObjectIndex_001c4e74 <= local_10) {
      return 0;
    }
    if (*(int *)(PTR__gObjectArray_001c4e98 + local_10 * 4) == param_1) break;
    local_10 = local_10 + 1;
  }
  return 1;
}


// ---- _InitDoOnceArray @ 0x6c30e (75 bytes)

void _InitDoOnceArray(void)

{
  int local_10;
  
  for (local_10 = 0; local_10 < 10000; local_10 = local_10 + 1) {
    *(undefined4 *)(PTR__gDoOnceArray_001c4e6c + local_10 * 4) = 0xffffffff;
  }
  *(undefined4 *)PTR__gNextDoOnceIndex_001c4e64 = 0;
  return;
}


// ---- _AddSpriteToDoOnceArray @ 0x6c359 (31 bytes)

void _AddSpriteToDoOnceArray(int param_1)

{
  _AddObjectToDoOnceArray(*(undefined4 *)(param_1 + 0xf8));
  return;
}


// ---- _RemoveSpriteFromDoOnceArray @ 0x6c378 (31 bytes)

void _RemoveSpriteFromDoOnceArray(int param_1)

{
  _RemoveObjectFromDoOnceArray(*(undefined4 *)(param_1 + 0xf8));
  return;
}


// ---- _AddObjectToDoOnceArray @ 0x6c397 (108 bytes)

void _AddObjectToDoOnceArray(undefined4 param_1)

{
  int iVar1;
  char cVar2;
  
  cVar2 = _IsObjectInDoOnceArray(param_1);
  if (cVar2 == '\0') {
    iVar1 = *(int *)PTR__gNextDoOnceIndex_001c4e64;
    *(undefined4 *)(PTR__gDoOnceArray_001c4e6c + iVar1 * 4) = param_1;
    *(int *)PTR__gNextDoOnceIndex_001c4e64 = iVar1 + 1;
    if (0x270e < *(int *)PTR__gNextDoOnceIndex_001c4e64) {
      *(undefined4 *)PTR__gNextDoOnceIndex_001c4e64 = 0;
    }
  }
  return;
}


// ---- _RemoveObjectFromDoOnceArray @ 0x6c403 (194 bytes)

void _RemoveObjectFromDoOnceArray(int param_1)

{
  char cVar1;
  int local_14;
  int local_10;
  
  cVar1 = _IsObjectInDoOnceArray(param_1);
  if (cVar1 != '\0') {
    local_10 = *(int *)PTR__gNextDoOnceIndex_001c4e64;
    for (local_14 = 0; local_14 < *(int *)PTR__gNextDoOnceIndex_001c4e64; local_14 = local_14 + 1) {
      if (*(int *)(PTR__gDoOnceArray_001c4e6c + local_14 * 4) == param_1) {
        local_10 = local_14;
        break;
      }
    }
    for (local_14 = local_10; local_14 < *(int *)PTR__gNextDoOnceIndex_001c4e64 + -1;
        local_14 = local_14 + 1) {
      *(undefined4 *)(PTR__gDoOnceArray_001c4e6c + local_14 * 4) =
           *(undefined4 *)(PTR__gDoOnceArray_001c4e6c + (local_14 + 1) * 4);
    }
    *(int *)PTR__gNextDoOnceIndex_001c4e64 = *(int *)PTR__gNextDoOnceIndex_001c4e64 + -1;
  }
  return;
}


// ---- _IsSpriteInDoOnceArray @ 0x6c4c5 (34 bytes)

undefined1 _IsSpriteInDoOnceArray(int param_1)

{
  undefined1 uVar1;
  
  uVar1 = _IsObjectInDoOnceArray(*(undefined4 *)(param_1 + 0xf8));
  return uVar1;
}


// ---- _IsObjectInDoOnceArray @ 0x6c4e7 (87 bytes)

undefined4 _IsObjectInDoOnceArray(int param_1)

{
  int local_10;
  
  local_10 = 0;
  while( true ) {
    if (*(int *)PTR__gNextDoOnceIndex_001c4e64 <= local_10) {
      return 0;
    }
    if (*(int *)(PTR__gDoOnceArray_001c4e6c + local_10 * 4) == param_1) break;
    local_10 = local_10 + 1;
  }
  return 1;
}


// ---- _InitRoomArray @ 0x6c53e (73 bytes)

void _InitRoomArray(void)

{
  int local_10;
  
  for (local_10 = 0; local_10 < 10000; local_10 = local_10 + 1) {
    *(undefined2 *)(PTR__gRoomArray_001c4eac + local_10 * 2) = 0xffff;
  }
  *(undefined2 *)PTR__gNextRoomArrayIndex_001c4e84 = 0;
  return;
}


// ---- _AddRoomToArray @ 0x6c587 (124 bytes)

void _AddRoomToArray(short param_1)

{
  short sVar1;
  char cVar2;
  
  cVar2 = _IsRoomInArray((int)param_1);
  if (cVar2 == '\0') {
    sVar1 = *(short *)PTR__gNextRoomArrayIndex_001c4e84;
    *(short *)(PTR__gRoomArray_001c4eac + sVar1 * 2) = param_1;
    *(short *)PTR__gNextRoomArrayIndex_001c4e84 = sVar1 + 1;
    if (9999 < *(short *)PTR__gNextRoomArrayIndex_001c4e84) {
      *(undefined2 *)PTR__gNextRoomArrayIndex_001c4e84 = 0;
    }
  }
  return;
}


// ---- _IsRoomInArray @ 0x6c603 (98 bytes)

undefined4 _IsRoomInArray(short param_1)

{
  int local_10;
  
  local_10 = 0;
  while( true ) {
    if (*(short *)PTR__gNextRoomArrayIndex_001c4e84 <= local_10) {
      return 0;
    }
    if (*(short *)(PTR__gRoomArray_001c4eac + local_10 * 2) == param_1) break;
    local_10 = local_10 + 1;
  }
  return 1;
}


// ---- _GetNumRoomsExplored @ 0x6c665 (32 bytes)

int _GetNumRoomsExplored(void)

{
  return (int)*(short *)PTR__gNextRoomArrayIndex_001c4e84;
}


// ---- _GetRoomArrayElement @ 0x6c685 (98 bytes)

int _GetRoomArrayElement(short param_1)

{
  if (9999 < param_1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameArrays.c",
               PTR____FUNCTION___27635_001c4e90,0x177);
  }
  return (int)*(short *)(PTR__gRoomArray_001c4eac + param_1 * 2);
}


// ---- _InitLitRoomArray @ 0x6c6e7 (73 bytes)

void _InitLitRoomArray(void)

{
  int local_10;
  
  for (local_10 = 0; local_10 < 10000; local_10 = local_10 + 1) {
    *(undefined2 *)(PTR__gLitRoomArray_001c4eb8 + local_10 * 2) = 0xffff;
  }
  *(undefined2 *)PTR__gNextLitRoomArrayIndex_001c4e88 = 0;
  return;
}


// ---- _AddRoomToLitRoomArray @ 0x6c730 (124 bytes)

void AddRoomToLitRoomArray(short param_1)

{
  short sVar1;
  char cVar2;
  
  cVar2 = _IsRoomInLitRoomArray((int)param_1);
  if (cVar2 == '\0') {
    sVar1 = *(short *)PTR__gNextLitRoomArrayIndex_001c4e88;
    *(short *)(PTR__gLitRoomArray_001c4eb8 + sVar1 * 2) = param_1;
    *(short *)PTR__gNextLitRoomArrayIndex_001c4e88 = sVar1 + 1;
    if (9999 < *(short *)PTR__gNextLitRoomArrayIndex_001c4e88) {
      *(undefined2 *)PTR__gNextLitRoomArrayIndex_001c4e88 = 0;
    }
  }
  return;
}


// ---- _IsRoomResIDInLitRoomArray @ 0x6c7ac (68 bytes)

undefined1 IsRoomResIDInLitRoomArray(short param_1)

{
  undefined1 uVar1;
  short sVar2;
  
  sVar2 = _GEGetLevelNumber((int)param_1,1,200);
  uVar1 = _IsRoomInLitRoomArray((int)sVar2);
  return uVar1;
}


// ---- _IsRoomInLitRoomArray @ 0x6c7f0 (98 bytes)

undefined4 _IsRoomInLitRoomArray(short param_1)

{
  int local_10;
  
  local_10 = 0;
  while( true ) {
    if (*(short *)PTR__gNextLitRoomArrayIndex_001c4e88 <= local_10) {
      return 0;
    }
    if (*(short *)(PTR__gLitRoomArray_001c4eb8 + local_10 * 2) == param_1) break;
    local_10 = local_10 + 1;
  }
  return 1;
}


// ---- _InitMapArray @ 0x6c852 (73 bytes)

void _InitMapArray(void)

{
  int local_10;
  
  for (local_10 = 0; local_10 < 10000; local_10 = local_10 + 1) {
    *(undefined2 *)(PTR__gMapArray_001c4eb4 + local_10 * 2) = 0xffff;
  }
  *(undefined2 *)PTR__gNextMapArrayIndex_001c4ea0 = 0;
  return;
}


// ---- _AddRoomToMapArray @ 0x6c89b (124 bytes)

void _AddRoomToMapArray(short param_1)

{
  short sVar1;
  char cVar2;
  
  cVar2 = _IsRoomInMapArray((int)param_1);
  if (cVar2 == '\0') {
    sVar1 = *(short *)PTR__gNextMapArrayIndex_001c4ea0;
    *(short *)(PTR__gMapArray_001c4eb4 + sVar1 * 2) = param_1;
    *(short *)PTR__gNextMapArrayIndex_001c4ea0 = sVar1 + 1;
    if (9999 < *(short *)PTR__gNextMapArrayIndex_001c4ea0) {
      *(undefined2 *)PTR__gNextMapArrayIndex_001c4ea0 = 0;
    }
  }
  return;
}


// ---- _IsRoomInMapArray @ 0x6c917 (98 bytes)

undefined4 _IsRoomInMapArray(short param_1)

{
  int local_10;
  
  local_10 = 0;
  while( true ) {
    if (*(short *)PTR__gNextMapArrayIndex_001c4ea0 <= local_10) {
      return 0;
    }
    if (*(short *)(PTR__gMapArray_001c4eb4 + local_10 * 2) == param_1) break;
    local_10 = local_10 + 1;
  }
  return 1;
}


// ---- _GetNumRoomsInMapArray @ 0x6c979 (32 bytes)

int _GetNumRoomsInMapArray(void)

{
  return (int)*(short *)PTR__gNextMapArrayIndex_001c4ea0;
}


// ---- _GetMapArrayElement @ 0x6c999 (98 bytes)

int _GetMapArrayElement(short param_1)

{
  if (9999 < param_1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameArrays.c",
               PTR____FUNCTION___27734_001c4e78,0x20a);
  }
  return (int)*(short *)(PTR__gMapArray_001c4eb4 + param_1 * 2);
}


// ---- _InitCompletedMansionsArray @ 0x6c9fb (105 bytes)

void _InitCompletedMansionsArray(void)

{
  int local_14;
  int local_10;
  
  for (local_10 = 0; local_10 < 3; local_10 = local_10 + 1) {
    for (local_14 = 0; local_14 < 100; local_14 = local_14 + 1) {
      PTR__gCompletedMansionsArray_001c4e70[local_14 + local_10 * 100] = 0;
    }
    *(undefined2 *)(PTR__gNextCompletedMansionIndex_001c4e80 + local_10 * 2) = 0;
  }
  return;
}


// ---- _AddMansionToArray @ 0x6ca64 (404 bytes)

void _AddMansionToArray(short param_1,short param_2)

{
  short sVar1;
  char cVar2;
  int local_20;
  
  if (param_1 < 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameArrays.c",
               PTR____FUNCTION___27760_001c4ec4,0x22c);
  }
  if (2 < param_1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameArrays.c",
               PTR____FUNCTION___27760_001c4ec4,0x22d);
  }
  cVar2 = _IsMansionInArray((int)param_1,(int)param_2);
  if (cVar2 == '\0') {
    if (param_2 == 0) {
      for (local_20 = 0; local_20 < 3; local_20 = local_20 + 1) {
        cVar2 = _IsMansionInArray((int)(short)local_20,0);
        if (cVar2 == '\0') {
          sVar1 = *(short *)(PTR__gNextCompletedMansionIndex_001c4e80 + local_20 * 2);
          PTR__gCompletedMansionsArray_001c4e70[(int)sVar1 + local_20 * 100] = (char)param_2;
          *(short *)(PTR__gNextCompletedMansionIndex_001c4e80 + local_20 * 2) = sVar1 + 1;
        }
      }
    }
    else {
      sVar1 = *(short *)(PTR__gNextCompletedMansionIndex_001c4e80 + param_1 * 2);
      PTR__gCompletedMansionsArray_001c4e70[(int)sVar1 + param_1 * 100] = (char)param_2;
      *(short *)(PTR__gNextCompletedMansionIndex_001c4e80 + param_1 * 2) = sVar1 + 1;
      if (99 < *(short *)(PTR__gNextCompletedMansionIndex_001c4e80 + param_1 * 2)) {
        *(undefined2 *)(PTR__gNextCompletedMansionIndex_001c4e80 + param_1 * 2) = 0;
      }
    }
  }
  return;
}


// ---- _IsMansionInArray @ 0x6cbf8 (225 bytes)

undefined4 _IsMansionInArray(short param_1,ushort param_2)

{
  int local_10;
  
  if (param_1 < 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameArrays.c",
               PTR____FUNCTION___27806_001c4e94,0x250);
  }
  if (2 < param_1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameArrays.c",
               PTR____FUNCTION___27806_001c4e94,0x251);
  }
  local_10 = 0;
  while( true ) {
    if (*(short *)(PTR__gNextCompletedMansionIndex_001c4e80 + param_1 * 2) <= local_10) {
      return 0;
    }
    if ((byte)PTR__gCompletedMansionsArray_001c4e70[local_10 + param_1 * 100] == param_2) break;
    local_10 = local_10 + 1;
  }
  return 1;
}


// ---- _WriteMansionArrayToFile @ 0x6ccd9 (87 bytes)

void _WriteMansionArrayToFile(undefined4 param_1)

{
  _MyWriteDataToFile(param_1,6,PTR__gNextCompletedMansionIndex_001c4e80);
  _MyWriteDataToFile(param_1,300,PTR__gCompletedMansionsArray_001c4e70);
  return;
}


// ---- _ReadMansionArrayFromFile @ 0x6cd30 (87 bytes)

void _ReadMansionArrayFromFile(undefined4 param_1)

{
  _MyReadDataFromFile(param_1,6,PTR__gNextCompletedMansionIndex_001c4e80);
  _MyReadDataFromFile(param_1,300,PTR__gCompletedMansionsArray_001c4e70);
  return;
}


// ---- _SaveAllGameArrays @ 0x6cd87 (242 bytes)

void SaveAllGameArrays(void)

{
  int iStack_10;
  
  for (iStack_10 = 0; iStack_10 < 30000; iStack_10 = iStack_10 + 1) {
    *(undefined4 *)(PTR__gSaveObjectArray_001c4e68 + iStack_10 * 4) =
         *(undefined4 *)(PTR__gObjectArray_001c4e98 + iStack_10 * 4);
  }
  *(undefined4 *)PTR__gSaveNextFreeObjectIndex_001c4ec0 =
       *(undefined4 *)PTR__gNextFreeObjectIndex_001c4e74;
  for (iStack_10 = 0; iStack_10 < 10000; iStack_10 = iStack_10 + 1) {
    *(undefined4 *)(PTR__gSaveDoOnceArray_001c4ebc + iStack_10 * 4) =
         *(undefined4 *)(PTR__gDoOnceArray_001c4e6c + iStack_10 * 4);
  }
  *(undefined4 *)PTR__gSaveNextDoOnceIndex_001c4ea8 = *(undefined4 *)PTR__gNextDoOnceIndex_001c4e64;
  for (iStack_10 = 0; iStack_10 < 10000; iStack_10 = iStack_10 + 1) {
    *(undefined2 *)(PTR__gSaveRoomArray_001c4e9c + iStack_10 * 2) =
         *(undefined2 *)(PTR__gRoomArray_001c4eac + iStack_10 * 2);
  }
  *(undefined2 *)PTR__gSaveNextRoomArrayIndex_001c4ea4 =
       *(undefined2 *)PTR__gNextRoomArrayIndex_001c4e84;
  return;
}


// ---- _RestoreAllGameArrays @ 0x6ce79 (242 bytes)

void RestoreAllGameArrays(void)

{
  int iStack_10;
  
  for (iStack_10 = 0; iStack_10 < 30000; iStack_10 = iStack_10 + 1) {
    *(undefined4 *)(PTR__gObjectArray_001c4e98 + iStack_10 * 4) =
         *(undefined4 *)(PTR__gSaveObjectArray_001c4e68 + iStack_10 * 4);
  }
  *(undefined4 *)PTR__gNextFreeObjectIndex_001c4e74 =
       *(undefined4 *)PTR__gSaveNextFreeObjectIndex_001c4ec0;
  for (iStack_10 = 0; iStack_10 < 10000; iStack_10 = iStack_10 + 1) {
    *(undefined4 *)(PTR__gDoOnceArray_001c4e6c + iStack_10 * 4) =
         *(undefined4 *)(PTR__gSaveDoOnceArray_001c4ebc + iStack_10 * 4);
  }
  *(undefined4 *)PTR__gNextDoOnceIndex_001c4e64 = *(undefined4 *)PTR__gSaveNextDoOnceIndex_001c4ea8;
  for (iStack_10 = 0; iStack_10 < 10000; iStack_10 = iStack_10 + 1) {
    *(undefined2 *)(PTR__gRoomArray_001c4eac + iStack_10 * 2) =
         *(undefined2 *)(PTR__gSaveRoomArray_001c4e9c + iStack_10 * 2);
  }
  *(undefined2 *)PTR__gNextRoomArrayIndex_001c4e84 =
       *(undefined2 *)PTR__gSaveNextRoomArrayIndex_001c4ea4;
  return;
}


// ---- _WriteSaveDataToFile @ 0x6cf6b (1487 bytes)

/* WARNING: Type propagation algorithm not settling */

void _WriteSaveDataToFile
               (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined1 local_20 [8];
  int local_18 [4];
  
  local_20[0] = (undefined1)param_4;
  local_18[1] = 0x70;
  _MyWriteDataToFile(param_1,4,local_18 + 1);
  _MyWriteDataToFile(param_1,4,PTR__gCurrentSaveIndex_001c4998);
  local_18[0] = *(short *)PTR__gNextLitRoomArrayIndex_001c4e88 * 2;
  _MyWriteDataToFile(param_1,4,local_18);
  _MyWriteDataToFile(param_1,2,PTR__gNextLitRoomArrayIndex_001c4e88);
  _MyWriteDataToFile(param_1,local_18[0],PTR__gLitRoomArray_001c4eb8);
  _MyWriteDataToFile(param_1,1,local_20);
  _MyWriteDataToFile(param_1,0xff,PTR__gMansionFilename_001c489c);
  _printf("Writing gMansionFilename to save file as: %s\n",PTR__gMansionFilename_001c489c);
  _MyWriteDataToFile(param_1,4,PTR__gNumSecretsFound_001c4990);
  _MyWriteDataToFile(param_1,2,PTR__gCurrentMansionNumber_001c4824);
  _MyWriteDataToFile(param_1,1,PTR__gDidCheat_001c4774);
  _MyWriteDataToFile(param_1,1,PTR__gCurrentlyInMansion_001c4908);
  _MyWriteDataToFile(param_1,2,PTR__gCurrentMusicIndex_001c476c);
  _MyWriteDataToFile(param_1,4,PTR__gTimeSpentInMansion_001c46fc);
  _MyWriteDataToFile(param_1,1,PTR__gDifficultySetting_001c4870);
  _MyWriteDataToFile(param_1,1,PTR__gSilverFloorIsActive_001c4958);
  _MyWriteDataToFile(param_1,1,PTR__gGoldFloorIsActive_001c47a8);
  _MyWriteDataToFile(param_1,2,PTR__gNumLivesLeft_001c49e4);
  _MyWriteDataToFile(param_1,4,&param_2);
  _MyWriteDataToFile(param_1,4,&param_3);
  _MyWriteDataToFile(param_1,2,PTR__gCurrentLevel_001c49bc);
  _MyWriteDataToFile(param_1,2,PTR__gObjects_001c4910);
  _MyWriteDataToFile(param_1,2,PTR__gObjects_001c4910 + 2);
  _MyWriteDataToFile(param_1,2,PTR__gObjects_001c4910 + 4);
  _MyWriteDataToFile(param_1,2,PTR__gObjects_001c4910 + 6);
  _MyWriteDataToFile(param_1,2,PTR__gObjects_001c4910 + 8);
  _MyWriteDataToFile(param_1,2,PTR__gObjects_001c4910 + 10);
  _MyWriteDataToFile(param_1,2,PTR__gObjects_001c4910 + 0xc);
  _MyWriteDataToFile(param_1,1,PTR__gObjects_001c4910 + 0xe);
  _MyWriteDataToFile(param_1,1,PTR__gObjects_001c4910 + 0xf);
  _MyWriteDataToFile(param_1,4,PTR__gScore_001c48a4);
  _MyWriteDataToFile(param_1,4,PTR__gLastSavePedestalObjectID_001c49f8);
  local_18[0] = *(int *)PTR__gNextFreeObjectIndex_001c4e74 << 2;
  _MyWriteDataToFile(param_1,4,local_18);
  _MyWriteDataToFile(param_1,4,PTR__gNextFreeObjectIndex_001c4e74);
  _MyWriteDataToFile(param_1,local_18[0],PTR__gObjectArray_001c4e98);
  local_18[0] = *(int *)PTR__gNextDoOnceIndex_001c4e64 << 2;
  _MyWriteDataToFile(param_1,4,local_18);
  _MyWriteDataToFile(param_1,4,PTR__gNextDoOnceIndex_001c4e64);
  _MyWriteDataToFile(param_1,local_18[0],PTR__gDoOnceArray_001c4e6c);
  local_18[0] = *(short *)PTR__gNextRoomArrayIndex_001c4e84 * 2;
  _MyWriteDataToFile(param_1,4,local_18);
  _MyWriteDataToFile(param_1,2,PTR__gNextRoomArrayIndex_001c4e84);
  _MyWriteDataToFile(param_1,local_18[0],PTR__gRoomArray_001c4eac);
  local_18[0] = *(short *)PTR__gNextMapArrayIndex_001c4ea0 * 2;
  _MyWriteDataToFile(param_1,4,local_18);
  _MyWriteDataToFile(param_1,2,PTR__gNextMapArrayIndex_001c4ea0);
  _MyWriteDataToFile(param_1,local_18[0],PTR__gMapArray_001c4eb4);
  local_18[2] = 0x7424;
  _MyWriteDataToFile(param_1,4,local_18 + 2);
  return;
}


// ---- _LoadSaveDataFromFile @ 0x6d53a (1779 bytes)

undefined4
_LoadSaveDataFromFile
          (undefined4 param_1,undefined4 *param_2,undefined4 *param_3,undefined1 *param_4,
          int *param_5)

{
  undefined4 local_30;
  uint local_24;
  undefined1 local_1d;
  int local_1c;
  int local_18;
  undefined4 local_14;
  undefined4 local_10 [2];
  
  _MyReadDataFromFile(param_1,4,&local_1c);
  if (local_1c < 0x70) {
    local_30 = 0xfffffffe;
  }
  else {
    _MyReadDataFromFile(param_1,4,PTR__gCurrentSaveIndex_001c4998);
    _InitLitRoomArray();
    _MyReadDataFromFile(param_1,4,&local_24);
    if (0x4e21 < local_24) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameArrays.c",
                 PTR____FUNCTION___27931_001c4eb0,0x344);
    }
    _MyReadDataFromFile(param_1,2,PTR__gNextLitRoomArrayIndex_001c4e88);
    _MyReadDataFromFile(param_1,local_24,PTR__gLitRoomArray_001c4eb8);
    _MyReadDataFromFile(param_1,1,&local_1d);
    _MyReadDataFromFile(param_1,0xff,PTR__gMansionFilename_001c489c);
    _MyReadDataFromFile(param_1,4,PTR__gNumSecretsFound_001c4990);
    _MyReadDataFromFile(param_1,2,PTR__gCurrentMansionNumber_001c4824);
    _MyReadDataFromFile(param_1,1,PTR__gDidCheat_001c4774);
    _MyReadDataFromFile(param_1,1,PTR__gCurrentlyInMansion_001c4908);
    _MyReadDataFromFile(param_1,2,PTR__gCurrentMusicIndex_001c476c);
    _MyReadDataFromFile(param_1,4,PTR__gTimeSpentInMansion_001c46fc);
    local_24 = 1;
    _MyReadDataFromFile(param_1,1,PTR__gDifficultySetting_001c4870);
    _MyReadDataFromFile(param_1,1,PTR__gSilverFloorIsActive_001c4958);
    _MyReadDataFromFile(param_1,1,PTR__gGoldFloorIsActive_001c47a8);
    _MyReadDataFromFile(param_1,2,PTR__gNumLivesLeft_001c49e4);
    _MyReadDataFromFile(param_1,4,local_10);
    _MyReadDataFromFile(param_1,4,&local_14);
    _MyReadDataFromFile(param_1,2,PTR__gCurrentLevel_001c49bc);
    _MyReadDataFromFile(param_1,2,PTR__gObjects_001c4910);
    _MyReadDataFromFile(param_1,2,PTR__gObjects_001c4910 + 2);
    _MyReadDataFromFile(param_1,2,PTR__gObjects_001c4910 + 4);
    _MyReadDataFromFile(param_1,2,PTR__gObjects_001c4910 + 6);
    _MyReadDataFromFile(param_1,2,PTR__gObjects_001c4910 + 8);
    _MyReadDataFromFile(param_1,2,PTR__gObjects_001c4910 + 10);
    _MyReadDataFromFile(param_1,2,PTR__gObjects_001c4910 + 0xc);
    _MyReadDataFromFile(param_1,1,PTR__gObjects_001c4910 + 0xe);
    _MyReadDataFromFile(param_1,1,PTR__gObjects_001c4910 + 0xf);
    _MyReadDataFromFile(param_1,4,PTR__gScore_001c48a4);
    _MyReadDataFromFile(param_1,4,PTR__gLastSavePedestalObjectID_001c49f8);
    *(undefined4 *)PTR__gOldScore_001c4950 = *(undefined4 *)PTR__gScore_001c48a4;
    _InitObjectArray();
    _InitDoOnceArray();
    _InitRoomArray();
    _InitMapArray();
    _MyReadDataFromFile(param_1,4,&local_24);
    if (0x1d4c3 < local_24) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameArrays.c",
                 PTR____FUNCTION___27931_001c4eb0,899);
    }
    _MyReadDataFromFile(param_1,4,PTR__gNextFreeObjectIndex_001c4e74);
    _MyReadDataFromFile(param_1,local_24,PTR__gObjectArray_001c4e98);
    _MyReadDataFromFile(param_1,4,&local_24);
    if (0x9c43 < local_24) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameArrays.c",
                 PTR____FUNCTION___27931_001c4eb0,0x389);
    }
    _MyReadDataFromFile(param_1,4,PTR__gNextDoOnceIndex_001c4e64);
    _MyReadDataFromFile(param_1,local_24,PTR__gDoOnceArray_001c4e6c);
    _MyReadDataFromFile(param_1,4,&local_24);
    if (0x4e21 < local_24) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameArrays.c",
                 PTR____FUNCTION___27931_001c4eb0,0x38f);
    }
    _MyReadDataFromFile(param_1,2,PTR__gNextRoomArrayIndex_001c4e84);
    _MyReadDataFromFile(param_1,local_24,PTR__gRoomArray_001c4eac);
    _MyReadDataFromFile(param_1,4,&local_24);
    if (0x4e21 < local_24) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameArrays.c",
                 PTR____FUNCTION___27931_001c4eb0,0x395);
    }
    _MyReadDataFromFile(param_1,2,PTR__gNextMapArrayIndex_001c4ea0);
    _MyReadDataFromFile(param_1,local_24,PTR__gMapArray_001c4eb4);
    _MyReadDataFromFile(param_1,4,&local_18);
    if (local_18 != 0x7424) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameArrays.c",
                 PTR____FUNCTION___27931_001c4eb0,0x39b);
    }
    *param_4 = local_1d;
    *param_5 = local_1c;
    *param_2 = local_10[0];
    *param_3 = local_14;
    local_30 = 0;
  }
  return local_30;
}


