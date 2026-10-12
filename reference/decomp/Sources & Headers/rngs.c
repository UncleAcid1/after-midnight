// Ghidra decompilation of functions from original source file:
//   Sources & Headers/rngs.c

// ---- _CustomRandom @ 0x9ace4 (259 bytes)

/* WARNING: Restarted to delay deadcode elimination for space: stack */

longdouble _CustomRandom(void)

{
  int iVar1;
  
  iVar1 = (*(int *)(PTR__seed_001c52a4 + *(int *)PTR__stream_001c52a8 * 4) % 0xadc8) * 0xbc8f +
          (*(int *)(PTR__seed_001c52a4 + *(int *)PTR__stream_001c52a8 * 4) / 0xadc8) * -0xd47;
  if (iVar1 < 1) {
    *(int *)(PTR__seed_001c52a4 + *(int *)PTR__stream_001c52a8 * 4) = iVar1 + 0x7fffffff;
  }
  else {
    *(int *)(PTR__seed_001c52a4 + *(int *)PTR__stream_001c52a8 * 4) = iVar1;
  }
  return (longdouble)
         ((double)*(int *)(PTR__seed_001c52a4 + *(int *)PTR__stream_001c52a8 * 4) / 2147483647.0);
}


// ---- _PlantSeeds @ 0x9ade7 (271 bytes)

/* WARNING: Restarted to delay deadcode elimination for space: stack */

void _PlantSeeds(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  int local_14;
  
  *(undefined4 *)PTR__initialized_001c52ac = 1;
  uVar1 = *(undefined4 *)PTR__stream_001c52a8;
  _SelectStream(0);
  _PutSeed(param_1);
  *(undefined4 *)PTR__stream_001c52a8 = uVar1;
  for (local_14 = 1; local_14 < 0x100; local_14 = local_14 + 1) {
    iVar2 = (*(int *)(PTR__seed_001c52a4 + (local_14 + -1) * 4) % 0x16dea) * 0x598d +
            (*(int *)(PTR__seed_001c52a4 + (local_14 + -1) * 4) / 0x16dea) * -0x1c1d;
    if (iVar2 < 1) {
      *(int *)(PTR__seed_001c52a4 + local_14 * 4) = iVar2 + 0x7fffffff;
    }
    else {
      *(int *)(PTR__seed_001c52a4 + local_14 * 4) = iVar2;
    }
  }
  return;
}


// ---- _PutSeed @ 0x9aef6 (296 bytes)

void _PutSeed(uint param_1)

{
  bool bVar1;
  
  bVar1 = false;
  if (0 < (int)param_1) {
    param_1 = param_1 + ((int)((longlong)(int)param_1 * 0x40000001 >> 0x3d) - ((int)param_1 >> 0x1f)
                        ) * -0x7fffffff;
  }
  if ((int)param_1 < 0) {
    param_1 = _time((time_t *)0x0);
    param_1 = param_1 % 0x7fffffff;
  }
  if (param_1 == 0) {
    while (!bVar1) {
      _printf("\nEnter a positive integer seed (9 digits or less) >> ");
      _scanf("%ld",&param_1);
      if (((int)param_1 < 1) || (param_1 == 0x7fffffff)) {
        bVar1 = false;
      }
      else {
        bVar1 = true;
      }
      if (!bVar1) {
        _puts("\nInput out of range ... try again");
      }
    }
  }
  *(uint *)(PTR__seed_001c52a4 + *(int *)PTR__stream_001c52a8 * 4) = param_1;
  return;
}


// ---- _GetSeed @ 0x9b01e (46 bytes)

void _GetSeed(undefined4 *param_1)

{
  *param_1 = *(undefined4 *)(PTR__seed_001c52a4 + *(int *)PTR__stream_001c52a8 * 4);
  return;
}


// ---- _SelectStream @ 0x9b04c (79 bytes)

void _SelectStream(byte param_1)

{
  *(uint *)PTR__stream_001c52a8 = (uint)param_1;
  if ((*(int *)PTR__initialized_001c52ac == 0) && (*(int *)PTR__stream_001c52a8 != 0)) {
    _PlantSeeds(0x75bcd15);
  }
  return;
}


// ---- _TestRandom @ 0x9b09b (219 bytes)

void TestRandom(void)

{
  longdouble lVar1;
  char cStack_30;
  int iStack_24;
  int iStack_20;
  double dStack_1c;
  char cStack_d;
  
  cStack_d = 0;
  _SelectStream(0);
  _PutSeed(1);
  for (iStack_20 = 0; iStack_20 < 10000; iStack_20 = iStack_20 + 1) {
    lVar1 = (longdouble)_CustomRandom();
    dStack_1c = (double)lVar1;
  }
  _GetSeed(&iStack_24);
  cStack_d = iStack_24 == 0x17cc5ab9;
  _SelectStream(1);
  _PlantSeeds(1);
  _GetSeed(&iStack_24);
  if ((cStack_d == '\0') || (iStack_24 != 0x598d)) {
    cStack_30 = '\0';
  }
  else {
    cStack_30 = '\x01';
  }
  cStack_d = cStack_30;
  if (cStack_30 == '\0') {
    _puts("\n\a ERROR -- the implementation of rngs.c is not correct.\n");
  }
  else {
    _puts("\n The implementation of rngs.c is correct.\n");
  }
  return;
}


