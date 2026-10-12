// Ghidra decompilation of functions from original source file:
//   Sources & Headers/GameEditorCarbonSafe.c

// ---- _GEGetLevelResID @ 0x6dc2d (53 bytes)

int _GEGetLevelResID(short param_1,short param_2,short param_3)

{
  return (int)(short)((param_1 * param_2 + param_3) - param_2);
}


// ---- _GEGetLevelNumber @ 0x6dc62 (60 bytes)

int _GEGetLevelNumber(short param_1,short param_2,short param_3)

{
  return (int)(short)((short)(((int)param_1 - (int)param_3) / (int)param_2) + 1);
}


// ---- _GEGetRoomResID @ 0x6dc9e (119 bytes)

int GEGetRoomResID(short param_1,short param_2)

{
  int iStack_18;
  
  if (param_1 < 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameEditorCarbonSafe.c"
               ,PTR____FUNCTION___20913_001c4ec8,0x2b);
  }
  if (param_1 == 0) {
    iStack_18 = 0;
  }
  else {
    iStack_18 = (int)(short)(param_1 + param_2 + -1);
  }
  return iStack_18;
}


// ---- _GEGetRoomNumber @ 0x6dd15 (63 bytes)

int GEGetRoomNumber(short param_1,short param_2)

{
  undefined4 uStack_18;
  
  if (param_1 == 0) {
    uStack_18 = 0;
  }
  else {
    uStack_18 = (int)(short)((param_1 - param_2) + 1);
  }
  return uStack_18;
}


// ---- _GEGetResIDFromLevelAndRoom @ 0x6dd54 (185 bytes)

int GEGetResIDFromLevelAndRoom(short param_1,short param_2,short param_3,short param_4)

{
  short sVar1;
  
  if (param_2 < 1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameEditorCarbonSafe.c"
               ,PTR____FUNCTION___20939_001c4ecc,0x49);
  }
  if (param_1 < 1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/GameEditorCarbonSafe.c"
               ,PTR____FUNCTION___20939_001c4ecc,0x4a);
  }
  sVar1 = _GEGetLevelResID((int)param_1,(int)param_3,(int)param_4);
  return (int)(short)(sVar1 + param_2 + -1);
}


// ---- _GEGetRowAndColFromLevelNumber @ 0x6de0d (89 bytes)

void _GEGetRowAndColFromLevelNumber(short param_1,short *param_2,short *param_3,short param_4)

{
  *param_2 = (short)(param_1 + -1) / param_4;
  *param_3 = (param_1 + -1) - *param_2 * param_4;
  return;
}


// ---- _GEGetRowAndColFromResID @ 0x6de66 (129 bytes)

void GEGetRowAndColFromResID
               (short param_1,short *param_2,short *param_3,short param_4,short param_5,
               short param_6)

{
  short sVar1;
  
  sVar1 = (short)(((int)param_1 - (int)param_6) / (int)param_5);
  *param_2 = sVar1 / param_4;
  *param_3 = sVar1 - *param_2 * param_4;
  return;
}


// ---- _GEGetLevelNumberFromRowAndCol @ 0x6dee7 (50 bytes)

int _GEGetLevelNumberFromRowAndCol(short param_1,short param_2,short param_3)

{
  return (int)(short)(param_1 * param_3 + param_2 + 1);
}


