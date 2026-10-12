// Ghidra decompilation of functions from original source file:
//   /Users/vernjensen/zziplib-0.13.60/Darwin_10.6.0_i386.d/../zzip/plugin.c

// ---- _zzip_filesize @ 0xeb6d2 (50 bytes)

undefined8 _zzip_filesize(int param_1)

{
  int iVar1;
  __darwin_time_t _Var2;
  long lVar3;
  long local_3c;
  __darwin_time_t local_38;
  
  iVar1 = _fstat(param_1,(stat *)&stack0xffffff94);
  _Var2 = -1;
  lVar3 = -1;
  if (-1 < iVar1) {
    _Var2 = local_38;
    lVar3 = local_3c;
  }
  return CONCAT44(_Var2,lVar3);
}


// ---- _zzip_get_default_io @ 0xeb704 (17 bytes)

undefined4 * _zzip_get_default_io(void)

{
  return &_default_io;
}


// ---- _zzip_init_io @ 0xeb715 (102 bytes)

undefined4 _zzip_init_io(undefined4 *param_1,undefined4 param_2)

{
  undefined4 uVar1;
  
  uVar1 = 0xfffff000;
  if (param_1 != (undefined4 *)0x0) {
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    param_1[3] = 0;
    param_1[4] = _zzip_filesize;
    param_1[5] = 1;
    param_1[6] = 1;
    param_1[7] = 0;
    param_1[5] = param_2;
    uVar1 = 0;
  }
  return uVar1;
}


