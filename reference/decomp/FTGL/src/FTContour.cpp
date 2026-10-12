// Ghidra decompilation of functions from original source file:
//   FTGL/src/FTContour.cpp

// ---- __ZN9FTContourC1EP10FT_Vector_Pcj @ 0x38c7e (1860 bytes)

/* FTContour::FTContour(FT_Vector_*, char*, unsigned int) */

void __thiscall FTContour::FTContour(FTContour *this,FT_Vector_ *param_1,char *param_2,uint param_3)

{
  int iVar1;
  undefined4 *puVar2;
  FTContour **ppFVar3;
  longdouble lVar4;
  float afStack_1b0 [4];
  int local_1a0;
  FTContour *local_19c;
  int local_198 [4];
  char local_185;
  double local_184;
  double local_17c;
  double local_174;
  double local_16c;
  double local_164;
  double local_15c;
  char local_14d;
  double local_14c;
  double local_144;
  double local_13c;
  double local_134;
  double local_12c;
  double local_124;
  double local_11c;
  double local_114;
  double local_10c;
  double local_104;
  double local_fc;
  double local_f4;
  double local_ec;
  double local_e4;
  undefined4 local_dc;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined4 local_cc;
  undefined4 local_c8;
  undefined4 local_bc;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  char local_e;
  char local_d;
  
  ppFVar3 = &local_19c;
  local_19c = this;
  FTVector<FTPoint>::FTVector();
  for (local_14 = 0; local_14 < param_3; local_14 = local_14 + 1) {
    local_e = param_2[local_14];
    if ((param_2[local_14] == '\x01') || (param_3 < 2)) {
      iVar1 = *(int *)(param_1 + local_14 * 8);
      ((float *)ppFVar3)[2] = (float)*(int *)(param_1 + local_14 * 8 + 4);
      ((float *)ppFVar3)[1] = (float)iVar1;
      *ppFVar3 = this;
      ((float *)ppFVar3)[-1] = 3.26079e-40;
      ::FTContour::AddPoint((float)*ppFVar3,((float *)ppFVar3)[1]);
    }
    else {
      ((float *)ppFVar3)[1] = (float)(param_1 + local_14 * 8);
      *ppFVar3 = (FTContour *)&local_5c;
      ((float *)ppFVar3)[-1] = 3.2612e-40;
      FTPoint::FTPoint((FTPoint *)*ppFVar3,(FT_Vector_ *)((float *)ppFVar3)[1]);
      if (local_14 == 0) {
        ((float *)ppFVar3)[1] = (float)(param_1 + param_3 * 8 + -8);
        *ppFVar3 = (FTContour *)&local_74;
        ((float *)ppFVar3)[-1] = 3.26166e-40;
        FTPoint::FTPoint((FTPoint *)*ppFVar3,(FT_Vector_ *)((float *)ppFVar3)[1]);
      }
      else {
        *ppFVar3 = this;
        ((float *)ppFVar3)[-1] = 3.26184e-40;
        iVar1 = FTVector<FTPoint>::size();
        ((float *)ppFVar3)[1] = (float)(iVar1 - 1);
        *ppFVar3 = this;
        ((float *)ppFVar3)[-1] = 3.26207e-40;
        puVar2 = (undefined4 *)FTVector<FTPoint>::operator[]((ulong)*ppFVar3);
        local_74 = *puVar2;
        local_70 = puVar2[1];
        local_6c = puVar2[2];
        local_68 = puVar2[3];
        local_64 = puVar2[4];
        local_60 = puVar2[5];
      }
      if (param_3 - 1 == local_14) {
        ((float *)ppFVar3)[1] = 0.0;
        *ppFVar3 = this;
        ((float *)ppFVar3)[-1] = 3.26298e-40;
        puVar2 = (undefined4 *)FTVector<FTPoint>::operator[]((ulong)*ppFVar3);
        local_8c = *puVar2;
        local_88 = puVar2[1];
        local_84 = puVar2[2];
        local_80 = puVar2[3];
        local_7c = puVar2[4];
        local_78 = puVar2[5];
      }
      else {
        ((float *)ppFVar3)[1] = (float)(param_1 + local_14 * 8 + 8);
        *ppFVar3 = (FTContour *)&local_8c;
        ((float *)ppFVar3)[-1] = 3.26403e-40;
        FTPoint::FTPoint((FTPoint *)*ppFVar3,(FT_Vector_ *)((float *)ppFVar3)[1]);
      }
      if (local_e == '\0') {
        if (param_3 - 1 == local_14) {
          local_185 = *param_2;
        }
        else {
          local_185 = param_2[local_14 + 1];
        }
        local_d = local_185;
        while (local_d == '\0') {
          *(undefined4 **)((int)ppFVar3 + 8) = &local_8c;
          *(undefined4 **)((int)ppFVar3 + 4) = &local_5c;
          *ppFVar3 = (FTContour *)&local_dc;
          *(undefined4 *)((int)ppFVar3 + -4) = 0x38e40;
          FTPoint::operator+((FTPoint *)*ppFVar3,*(FTPoint **)((int)ppFVar3 + 4));
          local_44 = local_dc;
          local_40 = local_d8;
          local_3c = local_d4;
          local_38 = local_d0;
          local_34 = local_cc;
          local_30 = local_c8;
          ((int *)((int)ppFVar3 + 4))[0] = 0;
          ((int *)((int)ppFVar3 + 4))[1] = 0x3fe00000;
          *ppFVar3 = (FTContour *)&local_44;
          *(undefined4 **)((int)ppFVar3 + -4) = &local_dc;
          *(undefined4 *)((int)ppFVar3 + -8) = 0x38e9e;
          FTPoint::operator*(*(FTPoint **)((int)ppFVar3 + -4),*(double *)ppFVar3);
          local_8c = local_dc;
          local_88 = local_d8;
          local_84 = local_d4;
          local_80 = local_d0;
          local_7c = local_cc;
          local_78 = local_c8;
          *(undefined4 **)((int)ppFVar3 + -8) = &local_74;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x38ee8;
          lVar4 = (longdouble)FTPoint::X();
          local_184 = (double)lVar4;
          *(float *)(this + 0x10) = (float)lVar4;
          *(undefined4 **)((int)ppFVar3 + -8) = &local_74;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x38f09;
          lVar4 = (longdouble)FTPoint::Y();
          local_17c = (double)lVar4;
          *(float *)(this + 0x14) = (float)lVar4;
          *(undefined4 **)((int)ppFVar3 + -8) = &local_5c;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x38f2a;
          lVar4 = (longdouble)FTPoint::X();
          local_174 = (double)lVar4;
          *(float *)(this + 0x18) = (float)lVar4;
          *(undefined4 **)((int)ppFVar3 + -8) = &local_5c;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x38f4b;
          lVar4 = (longdouble)FTPoint::Y();
          local_16c = (double)lVar4;
          *(float *)(this + 0x1c) = (float)lVar4;
          *(undefined4 **)((int)ppFVar3 + -8) = &local_8c;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x38f6f;
          lVar4 = (longdouble)FTPoint::X();
          local_164 = (double)lVar4;
          *(float *)(this + 0x20) = (float)lVar4;
          *(undefined4 **)((int)ppFVar3 + -8) = &local_8c;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x38f93;
          lVar4 = (longdouble)FTPoint::Y();
          local_15c = (double)lVar4;
          *(float *)(this + 0x24) = (float)lVar4;
          *(FTContour **)((int)ppFVar3 + -8) = this;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x38fb4;
          ::FTContour::evaluateQuadraticCurve();
          local_14 = local_14 + 1;
          local_74 = local_8c;
          local_70 = local_88;
          local_6c = local_84;
          local_68 = local_80;
          local_64 = local_7c;
          local_60 = local_78;
          *(FT_Vector_ **)((int)ppFVar3 + -4) = param_1 + local_14 * 8;
          *(undefined4 **)((int)ppFVar3 + -8) = &local_2c;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x38ffb;
          FTPoint::FTPoint(*(FTPoint **)((int)ppFVar3 + -8),*(FT_Vector_ **)((int)ppFVar3 + -4));
          local_5c = local_2c;
          local_58 = local_28;
          local_54 = local_24;
          local_50 = local_20;
          local_4c = local_1c;
          local_48 = local_18;
          if (param_3 - 1 == local_14) {
            *(int *)((int)ppFVar3 + -4) = 0;
            *(FTContour **)((int)ppFVar3 + -8) = this;
            *(undefined4 *)((int)ppFVar3 + -0xc) = 0x3903b;
            puVar2 = (undefined4 *)FTVector<FTPoint>::operator[](*(ulong *)((int)ppFVar3 + -8));
            local_8c = *puVar2;
            local_88 = puVar2[1];
            local_84 = puVar2[2];
            local_80 = puVar2[3];
            local_7c = puVar2[4];
            local_78 = puVar2[5];
          }
          else {
            *(FT_Vector_ **)((int)ppFVar3 + -4) = param_1 + local_14 * 8 + 8;
            *(undefined4 **)((int)ppFVar3 + -8) = &local_8c;
            *(undefined4 *)((int)ppFVar3 + -0xc) = 0x39086;
            FTPoint::FTPoint(*(FTPoint **)((int)ppFVar3 + -8),*(FT_Vector_ **)((int)ppFVar3 + -4));
          }
          if (param_3 - 1 == local_14) {
            local_14d = *param_2;
          }
          else {
            local_14d = param_2[local_14 + 1];
          }
          ppFVar3 = (FTContour **)((int)ppFVar3 + -8);
          local_d = local_14d;
        }
        *ppFVar3 = (FTContour *)&local_74;
        *(undefined4 *)((int)ppFVar3 + -4) = 0x390cc;
        lVar4 = (longdouble)FTPoint::X();
        local_14c = (double)lVar4;
        *(float *)(this + 0x10) = (float)lVar4;
        *ppFVar3 = (FTContour *)&local_74;
        *(undefined4 *)((int)ppFVar3 + -4) = 0x390ed;
        lVar4 = (longdouble)FTPoint::Y();
        local_144 = (double)lVar4;
        *(float *)(this + 0x14) = (float)lVar4;
        *ppFVar3 = (FTContour *)&local_5c;
        *(undefined4 *)((int)ppFVar3 + -4) = 0x3910e;
        lVar4 = (longdouble)FTPoint::X();
        local_13c = (double)lVar4;
        *(float *)(this + 0x18) = (float)lVar4;
        *ppFVar3 = (FTContour *)&local_5c;
        *(undefined4 *)((int)ppFVar3 + -4) = 0x3912f;
        lVar4 = (longdouble)FTPoint::Y();
        local_134 = (double)lVar4;
        *(float *)(this + 0x1c) = (float)lVar4;
        *ppFVar3 = (FTContour *)&local_8c;
        *(undefined4 *)((int)ppFVar3 + -4) = 0x39153;
        lVar4 = (longdouble)FTPoint::X();
        local_12c = (double)lVar4;
        *(float *)(this + 0x20) = (float)lVar4;
        *ppFVar3 = (FTContour *)&local_8c;
        *(undefined4 *)((int)ppFVar3 + -4) = 0x39177;
        lVar4 = (longdouble)FTPoint::Y();
        local_124 = (double)lVar4;
        *(float *)(this + 0x24) = (float)lVar4;
        *ppFVar3 = this;
        *(undefined4 *)((int)ppFVar3 + -4) = 0x39198;
        ::FTContour::evaluateQuadraticCurve();
      }
      else if (local_e == '\x02') {
        local_a4 = local_8c;
        local_a0 = local_88;
        local_9c = local_84;
        local_98 = local_80;
        local_94 = local_7c;
        local_90 = local_78;
        if (param_3 - 2 == local_14) {
          ((float *)ppFVar3)[1] = 0.0;
          *ppFVar3 = this;
          ((float *)ppFVar3)[-1] = 3.27883e-40;
          puVar2 = (undefined4 *)FTVector<FTPoint>::operator[]((ulong)*ppFVar3);
          local_bc = *puVar2;
          local_b8 = puVar2[1];
          local_b4 = puVar2[2];
          local_b0 = puVar2[3];
          local_ac = puVar2[4];
          local_a8 = puVar2[5];
        }
        else {
          ((float *)ppFVar3)[1] = (float)(param_1 + local_14 * 8 + 0x10);
          *ppFVar3 = (FTContour *)&local_bc;
          ((float *)ppFVar3)[-1] = 3.28005e-40;
          FTPoint::FTPoint((FTPoint *)*ppFVar3,(FT_Vector_ *)((float *)ppFVar3)[1]);
        }
        *ppFVar3 = (FTContour *)&local_74;
        ((float *)ppFVar3)[-1] = 3.2802e-40;
        lVar4 = (longdouble)FTPoint::X();
        local_11c = (double)lVar4;
        *(float *)(this + 0x10) = (float)lVar4;
        *ppFVar3 = (FTContour *)&local_74;
        ((float *)ppFVar3)[-1] = 3.28066e-40;
        lVar4 = (longdouble)FTPoint::Y();
        local_114 = (double)lVar4;
        *(float *)(this + 0x14) = (float)lVar4;
        *ppFVar3 = (FTContour *)&local_5c;
        ((float *)ppFVar3)[-1] = 3.28113e-40;
        lVar4 = (longdouble)FTPoint::X();
        local_10c = (double)lVar4;
        *(float *)(this + 0x18) = (float)lVar4;
        *ppFVar3 = (FTContour *)&local_5c;
        ((float *)ppFVar3)[-1] = 3.28159e-40;
        lVar4 = (longdouble)FTPoint::Y();
        local_104 = (double)lVar4;
        *(float *)(this + 0x1c) = (float)lVar4;
        *ppFVar3 = (FTContour *)&local_a4;
        ((float *)ppFVar3)[-1] = 3.28209e-40;
        lVar4 = (longdouble)FTPoint::X();
        local_fc = (double)lVar4;
        *(float *)(this + 0x20) = (float)lVar4;
        *ppFVar3 = (FTContour *)&local_a4;
        ((float *)ppFVar3)[-1] = 3.2826e-40;
        lVar4 = (longdouble)FTPoint::Y();
        local_f4 = (double)lVar4;
        *(float *)(this + 0x24) = (float)lVar4;
        *ppFVar3 = (FTContour *)&local_bc;
        ((float *)ppFVar3)[-1] = 3.2831e-40;
        lVar4 = (longdouble)FTPoint::X();
        local_ec = (double)lVar4;
        *(float *)(this + 0x28) = (float)lVar4;
        *ppFVar3 = (FTContour *)&local_bc;
        ((float *)ppFVar3)[-1] = 3.28361e-40;
        lVar4 = (longdouble)FTPoint::Y();
        local_e4 = (double)lVar4;
        *(float *)(this + 0x2c) = (float)lVar4;
        *ppFVar3 = this;
        ((float *)ppFVar3)[-1] = 3.28407e-40;
        ::FTContour::evaluateCubicCurve();
        local_14 = local_14 + 1;
      }
    }
  }
  return;
}


// ---- __ZN9FTContourC2EP10FT_Vector_Pcj @ 0x393c2 (1859 bytes)

/* FTContour::FTContour(FT_Vector_*, char*, unsigned int) */

void __thiscall FTContour::FTContour(FTContour *this,FT_Vector_ *param_1,char *param_2,uint param_3)

{
  int iVar1;
  undefined4 *puVar2;
  FTContour **ppFVar3;
  longdouble lVar4;
  float afStack_1b0 [4];
  int iStack_1a0;
  FTContour *pFStack_19c;
  int aiStack_198 [4];
  char cStack_185;
  double dStack_184;
  double dStack_17c;
  double dStack_174;
  double dStack_16c;
  double dStack_164;
  double dStack_15c;
  char cStack_14d;
  double dStack_14c;
  double dStack_144;
  double dStack_13c;
  double dStack_134;
  double dStack_12c;
  double dStack_124;
  double dStack_11c;
  double dStack_114;
  double dStack_10c;
  double dStack_104;
  double dStack_fc;
  double dStack_f4;
  double dStack_ec;
  double dStack_e4;
  undefined4 uStack_dc;
  undefined4 uStack_d8;
  undefined4 uStack_d4;
  undefined4 uStack_d0;
  undefined4 uStack_cc;
  undefined4 uStack_c8;
  undefined4 uStack_bc;
  undefined4 uStack_b8;
  undefined4 uStack_b4;
  undefined4 uStack_b0;
  undefined4 uStack_ac;
  undefined4 uStack_a8;
  undefined4 uStack_a4;
  undefined4 uStack_a0;
  undefined4 uStack_9c;
  undefined4 uStack_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  undefined4 uStack_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  uint uStack_14;
  char cStack_e;
  char cStack_d;
  
  ppFVar3 = &pFStack_19c;
  pFStack_19c = this;
  FTVector<FTPoint>::FTVector();
  for (uStack_14 = 0; uStack_14 < param_3; uStack_14 = uStack_14 + 1) {
    cStack_e = param_2[uStack_14];
    if ((param_2[uStack_14] == '\x01') || (param_3 < 2)) {
      iVar1 = *(int *)(param_1 + uStack_14 * 8);
      ((float *)ppFVar3)[2] = (float)*(int *)(param_1 + uStack_14 * 8 + 4);
      ((float *)ppFVar3)[1] = (float)iVar1;
      *ppFVar3 = this;
      ((float *)ppFVar3)[-1] = 3.28686e-40;
      ::FTContour::AddPoint((float)*ppFVar3,((float *)ppFVar3)[1]);
    }
    else {
      ((float *)ppFVar3)[1] = (float)(param_1 + uStack_14 * 8);
      *ppFVar3 = (FTContour *)&uStack_5c;
      ((float *)ppFVar3)[-1] = 3.28726e-40;
      FTPoint::FTPoint((FTPoint *)*ppFVar3,(FT_Vector_ *)((float *)ppFVar3)[1]);
      if (uStack_14 == 0) {
        ((float *)ppFVar3)[1] = (float)(param_1 + param_3 * 8 + -8);
        *ppFVar3 = (FTContour *)&uStack_74;
        ((float *)ppFVar3)[-1] = 3.28773e-40;
        FTPoint::FTPoint((FTPoint *)*ppFVar3,(FT_Vector_ *)((float *)ppFVar3)[1]);
      }
      else {
        *ppFVar3 = this;
        ((float *)ppFVar3)[-1] = 3.28791e-40;
        iVar1 = FTVector<FTPoint>::size();
        ((float *)ppFVar3)[1] = (float)(iVar1 - 1);
        *ppFVar3 = this;
        ((float *)ppFVar3)[-1] = 3.28813e-40;
        puVar2 = (undefined4 *)FTVector<FTPoint>::operator[]((ulong)*ppFVar3);
        uStack_74 = *puVar2;
        uStack_70 = puVar2[1];
        uStack_6c = puVar2[2];
        uStack_68 = puVar2[3];
        uStack_64 = puVar2[4];
        uStack_60 = puVar2[5];
      }
      if (param_3 - 1 == uStack_14) {
        ((float *)ppFVar3)[1] = 0.0;
        *ppFVar3 = this;
        ((float *)ppFVar3)[-1] = 3.28904e-40;
        puVar2 = (undefined4 *)FTVector<FTPoint>::operator[]((ulong)*ppFVar3);
        uStack_8c = *puVar2;
        uStack_88 = puVar2[1];
        uStack_84 = puVar2[2];
        uStack_80 = puVar2[3];
        uStack_7c = puVar2[4];
        uStack_78 = puVar2[5];
      }
      else {
        ((float *)ppFVar3)[1] = (float)(param_1 + uStack_14 * 8 + 8);
        *ppFVar3 = (FTContour *)&uStack_8c;
        ((float *)ppFVar3)[-1] = 3.29009e-40;
        FTPoint::FTPoint((FTPoint *)*ppFVar3,(FT_Vector_ *)((float *)ppFVar3)[1]);
      }
      if (cStack_e == '\0') {
        if (param_3 - 1 == uStack_14) {
          cStack_185 = *param_2;
        }
        else {
          cStack_185 = param_2[uStack_14 + 1];
        }
        cStack_d = cStack_185;
        while (cStack_d == '\0') {
          *(undefined4 **)((int)ppFVar3 + 8) = &uStack_8c;
          *(undefined4 **)((int)ppFVar3 + 4) = &uStack_5c;
          *ppFVar3 = (FTContour *)&uStack_dc;
          *(undefined4 *)((int)ppFVar3 + -4) = 0x39584;
          FTPoint::operator+((FTPoint *)*ppFVar3,*(FTPoint **)((int)ppFVar3 + 4));
          uStack_44 = uStack_dc;
          uStack_40 = uStack_d8;
          uStack_3c = uStack_d4;
          uStack_38 = uStack_d0;
          uStack_34 = uStack_cc;
          uStack_30 = uStack_c8;
          ((int *)((int)ppFVar3 + 4))[0] = 0;
          ((int *)((int)ppFVar3 + 4))[1] = 0x3fe00000;
          *ppFVar3 = (FTContour *)&uStack_44;
          *(undefined4 **)((int)ppFVar3 + -4) = &uStack_dc;
          *(undefined4 *)((int)ppFVar3 + -8) = 0x395e2;
          FTPoint::operator*(*(FTPoint **)((int)ppFVar3 + -4),*(double *)ppFVar3);
          uStack_8c = uStack_dc;
          uStack_88 = uStack_d8;
          uStack_84 = uStack_d4;
          uStack_80 = uStack_d0;
          uStack_7c = uStack_cc;
          uStack_78 = uStack_c8;
          *(undefined4 **)((int)ppFVar3 + -8) = &uStack_74;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x3962c;
          lVar4 = (longdouble)FTPoint::X();
          dStack_184 = (double)lVar4;
          *(float *)(this + 0x10) = (float)lVar4;
          *(undefined4 **)((int)ppFVar3 + -8) = &uStack_74;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x3964d;
          lVar4 = (longdouble)FTPoint::Y();
          dStack_17c = (double)lVar4;
          *(float *)(this + 0x14) = (float)lVar4;
          *(undefined4 **)((int)ppFVar3 + -8) = &uStack_5c;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x3966e;
          lVar4 = (longdouble)FTPoint::X();
          dStack_174 = (double)lVar4;
          *(float *)(this + 0x18) = (float)lVar4;
          *(undefined4 **)((int)ppFVar3 + -8) = &uStack_5c;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x3968f;
          lVar4 = (longdouble)FTPoint::Y();
          dStack_16c = (double)lVar4;
          *(float *)(this + 0x1c) = (float)lVar4;
          *(undefined4 **)((int)ppFVar3 + -8) = &uStack_8c;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x396b3;
          lVar4 = (longdouble)FTPoint::X();
          dStack_164 = (double)lVar4;
          *(float *)(this + 0x20) = (float)lVar4;
          *(undefined4 **)((int)ppFVar3 + -8) = &uStack_8c;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x396d7;
          lVar4 = (longdouble)FTPoint::Y();
          dStack_15c = (double)lVar4;
          *(float *)(this + 0x24) = (float)lVar4;
          *(FTContour **)((int)ppFVar3 + -8) = this;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x396f8;
          ::FTContour::evaluateQuadraticCurve();
          uStack_14 = uStack_14 + 1;
          uStack_74 = uStack_8c;
          uStack_70 = uStack_88;
          uStack_6c = uStack_84;
          uStack_68 = uStack_80;
          uStack_64 = uStack_7c;
          uStack_60 = uStack_78;
          *(FT_Vector_ **)((int)ppFVar3 + -4) = param_1 + uStack_14 * 8;
          *(undefined4 **)((int)ppFVar3 + -8) = &uStack_2c;
          *(undefined4 *)((int)ppFVar3 + -0xc) = 0x3973f;
          FTPoint::FTPoint(*(FTPoint **)((int)ppFVar3 + -8),*(FT_Vector_ **)((int)ppFVar3 + -4));
          uStack_5c = uStack_2c;
          uStack_58 = uStack_28;
          uStack_54 = uStack_24;
          uStack_50 = uStack_20;
          uStack_4c = uStack_1c;
          uStack_48 = uStack_18;
          if (param_3 - 1 == uStack_14) {
            *(int *)((int)ppFVar3 + -4) = 0;
            *(FTContour **)((int)ppFVar3 + -8) = this;
            *(undefined4 *)((int)ppFVar3 + -0xc) = 0x3977f;
            puVar2 = (undefined4 *)FTVector<FTPoint>::operator[](*(ulong *)((int)ppFVar3 + -8));
            uStack_8c = *puVar2;
            uStack_88 = puVar2[1];
            uStack_84 = puVar2[2];
            uStack_80 = puVar2[3];
            uStack_7c = puVar2[4];
            uStack_78 = puVar2[5];
          }
          else {
            *(FT_Vector_ **)((int)ppFVar3 + -4) = param_1 + uStack_14 * 8 + 8;
            *(undefined4 **)((int)ppFVar3 + -8) = &uStack_8c;
            *(undefined4 *)((int)ppFVar3 + -0xc) = 0x397ca;
            FTPoint::FTPoint(*(FTPoint **)((int)ppFVar3 + -8),*(FT_Vector_ **)((int)ppFVar3 + -4));
          }
          if (param_3 - 1 == uStack_14) {
            cStack_14d = *param_2;
          }
          else {
            cStack_14d = param_2[uStack_14 + 1];
          }
          ppFVar3 = (FTContour **)((int)ppFVar3 + -8);
          cStack_d = cStack_14d;
        }
        *ppFVar3 = (FTContour *)&uStack_74;
        *(undefined4 *)((int)ppFVar3 + -4) = 0x39810;
        lVar4 = (longdouble)FTPoint::X();
        dStack_14c = (double)lVar4;
        *(float *)(this + 0x10) = (float)lVar4;
        *ppFVar3 = (FTContour *)&uStack_74;
        *(undefined4 *)((int)ppFVar3 + -4) = 0x39831;
        lVar4 = (longdouble)FTPoint::Y();
        dStack_144 = (double)lVar4;
        *(float *)(this + 0x14) = (float)lVar4;
        *ppFVar3 = (FTContour *)&uStack_5c;
        *(undefined4 *)((int)ppFVar3 + -4) = 0x39852;
        lVar4 = (longdouble)FTPoint::X();
        dStack_13c = (double)lVar4;
        *(float *)(this + 0x18) = (float)lVar4;
        *ppFVar3 = (FTContour *)&uStack_5c;
        *(undefined4 *)((int)ppFVar3 + -4) = 0x39873;
        lVar4 = (longdouble)FTPoint::Y();
        dStack_134 = (double)lVar4;
        *(float *)(this + 0x1c) = (float)lVar4;
        *ppFVar3 = (FTContour *)&uStack_8c;
        *(undefined4 *)((int)ppFVar3 + -4) = 0x39897;
        lVar4 = (longdouble)FTPoint::X();
        dStack_12c = (double)lVar4;
        *(float *)(this + 0x20) = (float)lVar4;
        *ppFVar3 = (FTContour *)&uStack_8c;
        *(undefined4 *)((int)ppFVar3 + -4) = 0x398bb;
        lVar4 = (longdouble)FTPoint::Y();
        dStack_124 = (double)lVar4;
        *(float *)(this + 0x24) = (float)lVar4;
        *ppFVar3 = this;
        *(undefined4 *)((int)ppFVar3 + -4) = 0x398dc;
        ::FTContour::evaluateQuadraticCurve();
      }
      else if (cStack_e == '\x02') {
        uStack_a4 = uStack_8c;
        uStack_a0 = uStack_88;
        uStack_9c = uStack_84;
        uStack_98 = uStack_80;
        uStack_94 = uStack_7c;
        uStack_90 = uStack_78;
        if (param_3 - 2 == uStack_14) {
          ((float *)ppFVar3)[1] = 0.0;
          *ppFVar3 = this;
          ((float *)ppFVar3)[-1] = 3.30489e-40;
          puVar2 = (undefined4 *)FTVector<FTPoint>::operator[]((ulong)*ppFVar3);
          uStack_bc = *puVar2;
          uStack_b8 = puVar2[1];
          uStack_b4 = puVar2[2];
          uStack_b0 = puVar2[3];
          uStack_ac = puVar2[4];
          uStack_a8 = puVar2[5];
        }
        else {
          ((float *)ppFVar3)[1] = (float)(param_1 + uStack_14 * 8 + 0x10);
          *ppFVar3 = (FTContour *)&uStack_bc;
          ((float *)ppFVar3)[-1] = 3.30611e-40;
          FTPoint::FTPoint((FTPoint *)*ppFVar3,(FT_Vector_ *)((float *)ppFVar3)[1]);
        }
        *ppFVar3 = (FTContour *)&uStack_74;
        ((float *)ppFVar3)[-1] = 3.30627e-40;
        lVar4 = (longdouble)FTPoint::X();
        dStack_11c = (double)lVar4;
        *(float *)(this + 0x10) = (float)lVar4;
        *ppFVar3 = (FTContour *)&uStack_74;
        ((float *)ppFVar3)[-1] = 3.30673e-40;
        lVar4 = (longdouble)FTPoint::Y();
        dStack_114 = (double)lVar4;
        *(float *)(this + 0x14) = (float)lVar4;
        *ppFVar3 = (FTContour *)&uStack_5c;
        ((float *)ppFVar3)[-1] = 3.30719e-40;
        lVar4 = (longdouble)FTPoint::X();
        dStack_10c = (double)lVar4;
        *(float *)(this + 0x18) = (float)lVar4;
        *ppFVar3 = (FTContour *)&uStack_5c;
        ((float *)ppFVar3)[-1] = 3.30765e-40;
        lVar4 = (longdouble)FTPoint::Y();
        dStack_104 = (double)lVar4;
        *(float *)(this + 0x1c) = (float)lVar4;
        *ppFVar3 = (FTContour *)&uStack_a4;
        ((float *)ppFVar3)[-1] = 3.30816e-40;
        lVar4 = (longdouble)FTPoint::X();
        dStack_fc = (double)lVar4;
        *(float *)(this + 0x20) = (float)lVar4;
        *ppFVar3 = (FTContour *)&uStack_a4;
        ((float *)ppFVar3)[-1] = 3.30866e-40;
        lVar4 = (longdouble)FTPoint::Y();
        dStack_f4 = (double)lVar4;
        *(float *)(this + 0x24) = (float)lVar4;
        *ppFVar3 = (FTContour *)&uStack_bc;
        ((float *)ppFVar3)[-1] = 3.30917e-40;
        lVar4 = (longdouble)FTPoint::X();
        dStack_ec = (double)lVar4;
        *(float *)(this + 0x28) = (float)lVar4;
        *ppFVar3 = (FTContour *)&uStack_bc;
        ((float *)ppFVar3)[-1] = 3.30967e-40;
        lVar4 = (longdouble)FTPoint::Y();
        dStack_e4 = (double)lVar4;
        *(float *)(this + 0x2c) = (float)lVar4;
        *ppFVar3 = this;
        ((float *)ppFVar3)[-1] = 3.31013e-40;
        ::FTContour::evaluateCubicCurve();
        uStack_14 = uStack_14 + 1;
      }
    }
  }
  return;
}


// ---- __ZN7FTPointC1ERK10FT_Vector_ @ 0x39b06 (72 bytes)

/* FTPoint::FTPoint(FT_Vector_ const&) */

void __thiscall FTPoint::FTPoint(FTPoint *this,FT_Vector_ *param_1)

{
  *(double *)this = (double)*(int *)param_1;
  *(double *)(this + 8) = (double)*(int *)(param_1 + 4);
  *(undefined8 *)(this + 0x10) = 0;
  return;
}


// ---- __ZN7FTPointplERKS_ @ 0x39b4e (106 bytes)

/* FTPoint::TEMPNAMEPLACEHOLDERVALUE(FTPoint const&) */

FTPoint * __thiscall FTPoint::operator+(FTPoint *this,FTPoint *param_1)

{
  double *in_stack_0000000c;
  
  ::FTPoint::FTPoint(this);
  *(double *)this = *in_stack_0000000c + *(double *)param_1;
  *(double *)(this + 8) = in_stack_0000000c[1] + *(double *)(param_1 + 8);
  *(double *)(this + 0x10) = in_stack_0000000c[2] + *(double *)(param_1 + 0x10);
  return this;
}


// ---- __ZN7FTPointmlEd @ 0x39bb8 (98 bytes)

/* FTPoint::TEMPNAMEPLACEHOLDERVALUE(double) */

FTPoint * __thiscall FTPoint::operator*(FTPoint *this,double param_1)

{
  undefined4 in_stack_00000010;
  
  ::FTPoint::FTPoint(this);
  *(double *)this = *param_1._0_4_ * (double)CONCAT44(in_stack_00000010,param_1._4_4_);
  *(double *)(this + 8) = param_1._0_4_[1] * (double)CONCAT44(in_stack_00000010,param_1._4_4_);
  *(double *)(this + 0x10) = param_1._0_4_[2] * (double)CONCAT44(in_stack_00000010,param_1._4_4_);
  return this;
}


// ---- __ZNK8FTVectorI7FTPointE4sizeEv @ 0x39c1a (20 bytes)

/* FTVector<FTPoint>::size() const */

undefined4 __thiscall FTVector<FTPoint>::size(FTVector<FTPoint> *this)

{
  return *(undefined4 *)(this + 8);
}


// ---- __ZNK8FTVectorI7FTPointE5emptyEv @ 0x39c2e (34 bytes)

/* FTVector<FTPoint>::empty() const */

bool FTVector<FTPoint>::empty(void)

{
  int iVar1;
  
  iVar1 = FTVector<FTPoint>::size();
  return iVar1 == 0;
}


// ---- __ZN8FTVectorI7FTPointEC1Ev @ 0x39c50 (68 bytes)

/* FTVector<FTPoint>::FTVector() */

void __thiscall FTVector<FTPoint>::FTVector(FTVector<FTPoint> *this)

{
  *(undefined **)this = PTR_001c4a24 + 8;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 4) = *(undefined4 *)(this + 8);
  *(undefined4 *)(this + 0xc) = 0;
  return;
}


// ---- __ZN8FTVectorI7FTPointE5clearEv @ 0x39c94 (80 bytes)

/* FTVector<FTPoint>::clear() */

void __thiscall FTVector<FTPoint>::clear(FTVector<FTPoint> *this)

{
  if (*(int *)(this + 4) != 0) {
    if (*(int *)(this + 0xc) != 0) {
      operator_delete__(*(void **)(this + 0xc));
    }
    *(undefined4 *)(this + 8) = 0;
    *(undefined4 *)(this + 4) = *(undefined4 *)(this + 8);
    *(undefined4 *)(this + 0xc) = 0;
  }
  return;
}


// ---- __ZN8FTVectorI7FTPointED0Ev @ 0x39ce4 (72 bytes)

/* FTVector<FTPoint>::~FTVector() */

void __thiscall FTVector<FTPoint>::~FTVector(FTVector<FTPoint> *this)

{
  *(undefined **)this = PTR_001c4a24 + 8;
  FTVector<FTPoint>::clear();
  operator_delete(this);
  return;
}


// ---- __ZN8FTVectorI7FTPointED1Ev @ 0x39d2c (72 bytes)

/* WARNING: Removing unreachable block (ram,0x00039d63) */
/* FTVector<FTPoint>::~FTVector() */

void __thiscall FTVector<FTPoint>::~FTVector(FTVector<FTPoint> *this)

{
  *(undefined **)this = PTR_001c4a24 + 8;
  FTVector<FTPoint>::clear();
  return;
}


// ---- __ZN8FTVectorI7FTPointE5beginEv @ 0x39d74 (20 bytes)

/* FTVector<FTPoint>::begin() */

undefined4 __thiscall FTVector<FTPoint>::begin(FTVector<FTPoint> *this)

{
  return *(undefined4 *)(this + 0xc);
}


// ---- __ZN8FTVectorI7FTPointEixEm @ 0x39d88 (42 bytes)

/* FTVector<FTPoint>::operator[](unsigned long) */

int __thiscall FTVector<FTPoint>::operator[](FTVector<FTPoint> *this,ulong param_1)

{
  int iVar1;
  
  iVar1 = FTVector<FTPoint>::begin();
  return iVar1 + param_1 * 0x18;
}


// ---- __ZNK8FTVectorI7FTPointE8capacityEv @ 0x39db2 (20 bytes)

/* FTVector<FTPoint>::capacity() const */

undefined4 __thiscall FTVector<FTPoint>::capacity(FTVector<FTPoint> *this)

{
  return *(undefined4 *)(this + 4);
}


// ---- __ZN8FTVectorI7FTPointE3endEv @ 0x39dc6 (58 bytes)

/* FTVector<FTPoint>::end() */

int FTVector<FTPoint>::end(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FTVector<FTPoint>::begin();
  iVar2 = FTVector<FTPoint>::size();
  return iVar1 + iVar2 * 0x18;
}


// ---- __ZN8FTVectorI7FTPointE6expandEm @ 0x39e00 (302 bytes)

/* FTVector<FTPoint>::expand(unsigned long) */

void __thiscall FTVector<FTPoint>::expand(FTVector<FTPoint> *this,ulong param_1)

{
  int iVar1;
  FTPoint *pFVar2;
  undefined4 *puVar3;
  uint uStack_3c;
  FTPoint *pFStack_34;
  uint uStack_30;
  uint uStack_20;
  undefined4 *puStack_18;
  FTPoint *pFStack_10;
  
  iVar1 = FTVector<FTPoint>::capacity();
  if (iVar1 == 0) {
    uStack_3c = 0x100;
  }
  else {
    iVar1 = FTVector<FTPoint>::capacity();
    uStack_3c = iVar1 * 2;
  }
  uStack_20 = uStack_3c;
  if (param_1 != 0) {
    for (; uStack_20 < param_1; uStack_20 = uStack_20 << 1) {
    }
  }
  pFVar2 = (FTPoint *)operator_new__(uStack_20 * 0x18);
  pFStack_34 = pFVar2;
  uStack_30 = uStack_20;
  while (uStack_30 = uStack_30 - 1, uStack_30 != 0xffffffff) {
    FTPoint::FTPoint(pFStack_34);
    pFStack_34 = pFStack_34 + 0x18;
  }
  puStack_18 = (undefined4 *)FTVector<FTPoint>::begin();
  puVar3 = (undefined4 *)FTVector<FTPoint>::end();
  pFStack_10 = pFVar2;
  for (; puStack_18 != puVar3; puStack_18 = puStack_18 + 6) {
    *(undefined4 *)pFStack_10 = *puStack_18;
    *(undefined4 *)(pFStack_10 + 4) = puStack_18[1];
    *(undefined4 *)(pFStack_10 + 8) = puStack_18[2];
    *(undefined4 *)(pFStack_10 + 0xc) = puStack_18[3];
    *(undefined4 *)(pFStack_10 + 0x10) = puStack_18[4];
    *(undefined4 *)(pFStack_10 + 0x14) = puStack_18[5];
    pFStack_10 = pFStack_10 + 0x18;
  }
  if ((*(int *)(this + 4) != 0) && (*(int *)(this + 0xc) != 0)) {
    operator_delete__(*(void **)(this + 0xc));
  }
  *(FTPoint **)(this + 0xc) = pFVar2;
  *(uint *)(this + 4) = uStack_20;
  return;
}


// ---- __ZN8FTVectorI7FTPointE9push_backERKS0_ @ 0x39f2e (152 bytes)

/* FTVector<FTPoint>::push_back(FTPoint const&) */

void __thiscall FTVector<FTPoint>::push_back(FTVector<FTPoint> *this,FTPoint *param_1)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  
  iVar1 = FTVector<FTPoint>::size();
  iVar2 = FTVector<FTPoint>::capacity();
  if (iVar1 == iVar2) {
    FTVector<FTPoint>::expand((ulong)this);
  }
  FTVector<FTPoint>::size();
  puVar3 = (undefined4 *)FTVector<FTPoint>::operator[]((ulong)this);
  *puVar3 = *(undefined4 *)param_1;
  puVar3[1] = *(undefined4 *)(param_1 + 4);
  puVar3[2] = *(undefined4 *)(param_1 + 8);
  puVar3[3] = *(undefined4 *)(param_1 + 0xc);
  puVar3[4] = *(undefined4 *)(param_1 + 0x10);
  puVar3[5] = *(undefined4 *)(param_1 + 0x14);
  *(int *)(this + 8) = *(int *)(this + 8) + 1;
  return;
}


// ---- __ZN9FTContour8AddPointE7FTPoint @ 0x39fc6 (112 bytes)

/* FTContour::AddPoint(FTPoint) */

void FTContour::AddPoint(FTPoint *param_1)

{
  bool bVar1;
  char cVar2;
  FTPoint *pFVar3;
  
  cVar2 = FTVector<FTPoint>::empty();
  if (cVar2 == '\0') {
    FTVector<FTPoint>::size();
    pFVar3 = (FTPoint *)FTVector<FTPoint>::operator[]((ulong)param_1);
    cVar2 = operator!=((FTPoint *)&stack0x00000008,pFVar3);
    if (cVar2 == '\0') {
      bVar1 = false;
      goto LAB_0003a019;
    }
  }
  bVar1 = true;
LAB_0003a019:
  if (bVar1) {
    FTVector<FTPoint>::push_back(param_1);
  }
  return;
}


// ---- __ZN9FTContour8AddPointEff @ 0x3a036 (128 bytes)

/* FTContour::AddPoint(float, float) */

void __thiscall FTContour::AddPoint(FTContour *this,float param_1,float param_2)

{
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  
  FTPoint::FTPoint((FTPoint *)&uStack_24,(double)param_1,(double)param_2,0.0);
  ::FTContour::AddPoint(this,uStack_24,uStack_20,uStack_1c,uStack_18,uStack_14,uStack_10);
  return;
}


// ---- __ZN9FTContour22evaluateQuadraticCurveEv @ 0x3a0b6 (502 bytes)

/* FTContour::evaluateQuadraticCurve() */

void __thiscall FTContour::evaluateQuadraticCurve(FTContour *this)

{
  float fVar1;
  uint uStack_14;
  
  for (uStack_14 = 0;
      (float)(uStack_14 >> 0x10) * 65536.0 + (float)(uStack_14 & 0xffff) <=
      1.0 / *(float *)PTR__BEZIER_STEP_SIZE_001c4a20; uStack_14 = uStack_14 + 1) {
    fVar1 = *(float *)PTR__BEZIER_STEP_SIZE_001c4a20 *
            ((float)(uStack_14 >> 0x10) * 65536.0 + (float)(uStack_14 & 0xffff));
    ::FTContour::AddPoint
              ((float)this,
               (*(float *)(this + 0x20) * fVar1 + (1.0 - fVar1) * *(float *)(this + 0x18)) * fVar1 +
               (1.0 - fVar1) *
               (*(float *)(this + 0x18) * fVar1 + (1.0 - fVar1) * *(float *)(this + 0x10)));
  }
  return;
}


// ---- __ZN9FTContour18evaluateCubicCurveEv @ 0x3a2ac (789 bytes)

/* FTContour::evaluateCubicCurve() */

void __thiscall FTContour::evaluateCubicCurve(FTContour *this)

{
  float fVar1;
  float fVar2;
  uint uStack_14;
  
  for (uStack_14 = 0;
      (float)(uStack_14 >> 0x10) * 65536.0 + (float)(uStack_14 & 0xffff) <=
      1.0 / *(float *)PTR__BEZIER_STEP_SIZE_001c4a20; uStack_14 = uStack_14 + 1) {
    fVar1 = *(float *)PTR__BEZIER_STEP_SIZE_001c4a20 *
            ((float)(uStack_14 >> 0x10) * 65536.0 + (float)(uStack_14 & 0xffff));
    fVar2 = *(float *)(this + 0x20) * fVar1 + (1.0 - fVar1) * *(float *)(this + 0x18);
    ::FTContour::AddPoint
              ((float)this,
               ((*(float *)(this + 0x28) * fVar1 + (1.0 - fVar1) * *(float *)(this + 0x20)) * fVar1
               + (1.0 - fVar1) * fVar2) * fVar1 +
               (1.0 - fVar1) *
               (fVar2 * fVar1 +
               (1.0 - fVar1) *
               (*(float *)(this + 0x18) * fVar1 + (1.0 - fVar1) * *(float *)(this + 0x10))));
  }
  return;
}


