// Ghidra decompilation of functions from original source file:
//   /Users/vernjensen/zziplib-0.13.60/Darwin_10.6.0_i386.d/../zzip/err.c

// ---- _zzip_strerror @ 0xea301 (92 bytes)

void _zzip_strerror(int param_1)

{
  int *piVar1;
  
  piVar1 = &_errlist;
  if (param_1 + 0x101fU < 0x1f) {
    for (; piVar1[1] != 0; piVar1 = piVar1 + 2) {
      if (param_1 == *piVar1) {
        return;
      }
    }
    param_1 = 0x16;
  }
  else if (param_1 < 0) {
    if (param_1 != -1) {
      _zError(param_1);
      return;
    }
    param_1 = -1;
  }
  _strerror(param_1);
  return;
}


// ---- _zzip_strerror_of @ 0xea35d (41 bytes)

void _zzip_strerror_of(int param_1)

{
  int *piVar1;
  
  if (param_1 == 0) {
    piVar1 = ___error();
    _strerror(*piVar1);
    return;
  }
  _zzip_strerror();
  return;
}


// ---- _zzip_errno @ 0xea386 (65 bytes)

int _zzip_errno(int param_1)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = &_errnolist;
  if (param_1 < -1) {
    for (; *piVar1 != 0; piVar1 = piVar1 + 2) {
      if (param_1 == *piVar1) {
        return piVar1[1];
      }
    }
    iVar2 = 0x16;
  }
  else {
    piVar1 = ___error();
    iVar2 = *piVar1;
  }
  return iVar2;
}


