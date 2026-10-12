// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTPoint.cpp

// ---- __ZeqRK7FTPointS1_ @ 0x427ea (98 bytes)

/* TEMPNAMEPLACEHOLDERVALUE(FTPoint const&, FTPoint const&) */

undefined1 operator==(FTPoint *param_1,FTPoint *param_2)

{
  undefined1 uStack_d;
  
  if ((((*(double *)param_1 != *(double *)param_2) ||
       (NAN(*(double *)param_1) || NAN(*(double *)param_2))) ||
      (*(double *)(param_1 + 8) != *(double *)(param_2 + 8))) ||
     (((NAN(*(double *)(param_1 + 8)) || NAN(*(double *)(param_2 + 8)) ||
       (*(double *)(param_1 + 0x10) != *(double *)(param_2 + 0x10))) ||
      (NAN(*(double *)(param_1 + 0x10)) || NAN(*(double *)(param_2 + 0x10)))))) {
    uStack_d = 0;
  }
  else {
    uStack_d = 1;
  }
  return uStack_d;
}


// ---- __ZneRK7FTPointS1_ @ 0x4284c (100 bytes)

/* TEMPNAMEPLACEHOLDERVALUE(FTPoint const&, FTPoint const&) */

undefined1 operator!=(FTPoint *param_1,FTPoint *param_2)

{
  undefined1 local_d;
  
  if ((((*(double *)param_1 != *(double *)param_2) ||
       (NAN(*(double *)param_1) || NAN(*(double *)param_2))) ||
      (*(double *)(param_1 + 8) != *(double *)(param_2 + 8))) ||
     (((NAN(*(double *)(param_1 + 8)) || NAN(*(double *)(param_2 + 8)) ||
       (*(double *)(param_1 + 0x10) != *(double *)(param_2 + 0x10))) ||
      (NAN(*(double *)(param_1 + 0x10)) || NAN(*(double *)(param_2 + 0x10)))))) {
    local_d = 1;
  }
  else {
    local_d = 0;
  }
  return local_d;
}


// ---- __ZmldR7FTPoint @ 0x428b0 (104 bytes)

/* TEMPNAMEPLACEHOLDERVALUE(double, FTPoint&) */

undefined4 * operator*(double param_1,FTPoint *param_2)

{
  undefined4 in_stack_00000010;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_14;
  FTPoint *pFStack_10;
  
  uStack_14 = param_1._4_4_;
  pFStack_10 = param_2;
  FTPoint::operator*((FTPoint *)&uStack_3c,(double)CONCAT44(param_1._4_4_,in_stack_00000010));
  *param_1._0_4_ = uStack_3c;
  param_1._0_4_[1] = uStack_38;
  param_1._0_4_[2] = uStack_34;
  param_1._0_4_[3] = uStack_30;
  param_1._0_4_[4] = uStack_2c;
  param_1._0_4_[5] = uStack_28;
  return param_1._0_4_;
}


