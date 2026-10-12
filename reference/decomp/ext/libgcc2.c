// Ghidra decompilation of functions from original source file:
//   /var/tmp/gcc_40/gcc_40-5494~207/src/gcc/libgcc2.c

// ---- ___eprintf @ 0x116b00 (77 bytes)

void ___eprintf(char *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined *puVar1;
  
  puVar1 = PTR_001c5364;
  _fprintf(*(FILE **)PTR_001c5364,param_1,param_2,param_3,param_4);
  _fflush(*(FILE **)puVar1);
                    /* WARNING: Subroutine does not return */
  _abort();
}


