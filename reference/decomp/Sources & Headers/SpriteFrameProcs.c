// Ghidra decompilation of functions from original source file:
//   Sources & Headers/SpriteFrameProcs.c

// ---- _HeroMiscFrameProc @ 0x52fd4 (96 bytes)

void HeroMiscFrameProc(int param_1)

{
  short sVar1;
  
  sVar1 = *(short *)(param_1 + 0xfc);
  *(short *)(param_1 + 0xfc) = sVar1 + -1;
  if (sVar1 < 1) {
    *(undefined2 *)(param_1 + 0xfc) = *(undefined2 *)(param_1 + 0x100);
    _MyAdvanceSpriteFrame(param_1,param_1 + 0xfe);
  }
  return;
}


// ---- _IdleHeroFrameProc @ 0x53034 (1550 bytes)

void IdleHeroFrameProc(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  short sVar7;
  short sVar8;
  undefined2 uVar9;
  int iVar10;
  short sStack_e;
  
  sStack_e = -1;
  sVar2._0_1_ = param_1->f_1a2;
  sVar2._1_1_ = param_1->f_1a3;
  if (sVar2 == 0) {
    *param_3 = 0x44;
    _SWSetSpriteFrameTime(param_1,0x8e);
    _SWSetSpriteFrameAdvance(param_1,0);
    uVar9 = _GetRandom(4,0xc);
    param_1->f_1a6 = (char)uVar9;
    param_1->f_1a7 = (char)((ushort)uVar9 >> 8);
    uVar9 = _GetRandom(6,0x14);
    param_1->f_1a8 = (char)uVar9;
    param_1->f_1a9 = (char)((ushort)uVar9 >> 8);
    uVar9 = _GetRandom(0x1e,200);
    param_1->f_1ac = (char)uVar9;
    param_1->f_1ad = (char)((ushort)uVar9 >> 8);
    param_1->f_1a2 = '\x01';
    param_1->f_1a3 = '\0';
    param_1->f_177 = '\x01';
  }
  else {
    sVar1._0_1_ = param_1->f_1a2;
    sVar1._1_1_ = param_1->f_1a3;
    if (sVar1 == 1) {
      *param_3 = 0x45;
      _SWSetSpriteFrameTime(param_1,200);
      sVar2 = *(short *)&param_1->f_1a6;
      *(short *)&param_1->f_1a6 = sVar2 + -1;
      if (sVar2 < 1) {
        *param_3 = 0x46;
        uVar9 = _GetRandom(4,0xc);
        param_1->f_1a6 = (char)uVar9;
        param_1->f_1a7 = (char)((ushort)uVar9 >> 8);
      }
      sVar2 = *(short *)&param_1->f_1a8;
      *(short *)&param_1->f_1a8 = sVar2 + -1;
      if ((sVar2 < 1) &&
         (sStack_e = 2, sVar3._0_1_ = param_1->f_1ac, sVar3._1_1_ = param_1->f_1ad, sVar3 < 1)) {
        sStack_e = 4;
      }
    }
    else {
      sVar4._0_1_ = param_1->f_1a2;
      sVar4._1_1_ = param_1->f_1a3;
      if (sVar4 == 2) {
        _SWSetSpriteFrameTime(param_1,0xfa);
        if (*param_3 == 0x47) {
          *param_3 = 0x45;
        }
        else {
          *param_3 = 0x47;
        }
        sVar2 = *(short *)&param_1->f_1aa;
        *(short *)&param_1->f_1aa = sVar2 + -1;
        if (sVar2 < 1) {
          sStack_e = 1;
        }
        sVar2 = *(short *)&param_1->f_1a6;
        *(short *)&param_1->f_1a6 = sVar2 + -1;
        if ((sVar2 < 1) && (*param_3 == 0x45)) {
          *param_3 = 0x46;
          uVar9 = _GetRandom(4,0xc);
          param_1->f_1a6 = (char)uVar9;
          param_1->f_1a7 = (char)((ushort)uVar9 >> 8);
        }
        iVar10 = _GetRandom(1,0xf);
        if (iVar10 == 1) {
          sStack_e = 3;
        }
      }
      else {
        sVar5._0_1_ = param_1->f_1a2;
        sVar5._1_1_ = param_1->f_1a3;
        if (sVar5 == 3) {
          if ((*param_3 == 0x4b) &&
             (sVar2 = *(short *)&param_1->f_1ae, *(short *)&param_1->f_1ae = sVar2 + -1, sVar2 < 2))
          {
            _SWSetSpriteFrameAdvance(param_1,0);
            sStack_e = 6;
          }
        }
        else {
          sVar6._0_1_ = param_1->f_1a2;
          sVar6._1_1_ = param_1->f_1a3;
          if (sVar6 == 6) {
            sStack_e = 1;
          }
          else {
            sVar7._0_1_ = param_1->f_1a2;
            sVar7._1_1_ = param_1->f_1a3;
            if (sVar7 == 4) {
              if (*param_3 == 0x4a) {
                _SWSetSpriteFrameAdvance(param_1,0);
                _SWSetSpriteFrameTime(param_1,0x38d);
                param_1->f_1a2 = '\x05';
                param_1->f_1a3 = '\0';
              }
            }
            else {
              sVar8._0_1_ = param_1->f_1a2;
              sVar8._1_1_ = param_1->f_1a3;
              if (sVar8 == 5) {
                _MakeZzzzFromJacksHead(param_1);
              }
            }
          }
        }
      }
    }
  }
  *(short *)&param_1->f_1ac = *(short *)&param_1->f_1ac + -1;
  if (sStack_e == 1) {
    param_1->f_1a2 = '\x01';
    param_1->f_1a3 = '\0';
    uVar9 = _GetRandom(6,0x14);
    param_1->f_1a8 = (char)uVar9;
    param_1->f_1a9 = (char)((ushort)uVar9 >> 8);
    uVar9 = _GetRandom(4,0xc);
    param_1->f_1a6 = (char)uVar9;
    param_1->f_1a7 = (char)((ushort)uVar9 >> 8);
    _SWSetSpriteFrameAdvance(param_1,0);
    _SWSetSpriteFrameRange(param_1,0x45,0x47);
    _SWSetCurrentFrameIndex(param_1,0x45);
  }
  else if (sStack_e == 2) {
    param_1->f_1a2 = '\x02';
    param_1->f_1a3 = '\0';
    uVar9 = _GetRandom(0xc,0x18);
    param_1->f_1aa = (char)uVar9;
    param_1->f_1ab = (char)((ushort)uVar9 >> 8);
    uVar9 = _GetRandom(4,0xc);
    param_1->f_1a6 = (char)uVar9;
    param_1->f_1a7 = (char)((ushort)uVar9 >> 8);
    _SWSetSpriteFrameAdvance(param_1,0);
    _SWSetSpriteFrameRange(param_1,0x45,0x47);
    _SWSetCurrentFrameIndex(param_1,0x47);
  }
  else if (sStack_e == 3) {
    param_1->f_1a2 = '\x03';
    param_1->f_1a3 = '\0';
    _SWSetSpriteFrameAdvance(param_1,1);
    _SWSetSpriteFrameAdvanceMode(param_1,(void *)0x0);
    _SWSetSpriteFrameTime(param_1,0x7d);
    _SWSetSpriteFrameRange(param_1,0x4b,0x50);
    _SWSetCurrentFrameIndex(param_1,0x4b);
    uVar9 = _GetRandom(4,10);
    param_1->f_1ae = (char)uVar9;
    param_1->f_1af = (char)((ushort)uVar9 >> 8);
  }
  else if (sStack_e == 6) {
    param_1->f_1a2 = '\x06';
    param_1->f_1a3 = '\0';
    _SWSetSpriteFrameAdvance(param_1,0);
    _SWSetSpriteFrameTime(param_1,500);
  }
  else if (sStack_e == 4) {
    param_1->f_1a2 = '\x04';
    param_1->f_1a3 = '\0';
    _SWSetSpriteFrameAdvance(param_1,1);
    _SWSetSpriteFrameAdvanceMode(param_1,(void *)0x0);
    _SWSetSpriteFrameTime(param_1,0x14d);
    _SWSetSpriteFrameRange(param_1,0x48,0x4a);
    _SWSetCurrentFrameIndex(param_1,0x48);
  }
  return;
}


// ---- _SkeletonCollapseFrameProc1 @ 0x53642 (99 bytes)

void SkeletonCollapseFrameProc1(SpritePtr param_1)

{
  short sVar1;
  
  sVar1 = *(short *)&param_1->f_192;
  *(short *)&param_1->f_192 = sVar1 + -1;
  if (sVar1 < 1) {
    param_1->f_192 = '\n';
    param_1->f_193 = '\0';
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_LAB_001c4cac);
  }
  return;
}


// ---- _SkeletonCollapseFrameProc2 @ 0x536a5 (258 bytes)

void SkeletonCollapseFrameProc2(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  short sVar1;
  undefined2 uVar2;
  
  sVar1 = *(short *)&param_1->f_192;
  *(short *)&param_1->f_192 = sVar1 + -1;
  if (sVar1 < 1) {
    *param_3 = *param_3 + 1;
    uVar2 = (undefined2)(int)((double)(*param_3 + -0x2a) / -1.7 + 9.0);
    param_1->f_192 = (char)uVar2;
    param_1->f_193 = (char)((ushort)uVar2 >> 8);
    if (*param_3 == 0x2c) {
      _MMPlaySound(8,9,1);
    }
    if (*param_3 == 0x31) {
      _MySetSpriteFrameTime(param_1,0xffffffff);
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
      if (param_1->f_17d == '\0') {
        *PTR__gRestartLevel_001c4858 = 1;
        param_1->f_17d = '\x01';
      }
    }
  }
  return;
}


// ---- _StoneCollapseFrameProc1 @ 0x537a7 (134 bytes)

void StoneCollapseFrameProc1(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  short sVar1;
  
  sVar1 = *(short *)&param_1->f_192;
  *(short *)&param_1->f_192 = sVar1 + -1;
  if (sVar1 < 1) {
    *param_3 = *param_3 + 1;
    param_1->f_192 = '\f';
    param_1->f_193 = '\0';
    if (*param_3 == 0x3d) {
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_LAB_001c4c88);
      param_1->f_192 = '\x1e';
      param_1->f_193 = '\0';
    }
  }
  return;
}


// ---- _StoneCollapseFrameProc2 @ 0x5382d (127 bytes)

void StoneCollapseFrameProc2(SpritePtr param_1)

{
  short sVar1;
  
  sVar1 = *(short *)&param_1->f_192;
  *(short *)&param_1->f_192 = sVar1 + -1;
  if (sVar1 < 1) {
    param_1->f_192 = '\n';
    param_1->f_193 = '\0';
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_StoneCollapseFrameProc3_001c4ce0);
    _MMPlaySound(0x35,9,1);
  }
  return;
}


// ---- _StoneCollapseFrameProc3 @ 0x538ac (206 bytes)

void StoneCollapseFrameProc3(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  short sVar1;
  undefined2 uVar2;
  
  sVar1 = *(short *)&param_1->f_192;
  *(short *)&param_1->f_192 = sVar1 + -1;
  if (sVar1 < 1) {
    *param_3 = *param_3 + 1;
    uVar2 = (undefined2)(int)((double)(*param_3 + -0x3d) / -1.7 + 10.0);
    param_1->f_192 = (char)uVar2;
    param_1->f_193 = (char)((ushort)uVar2 >> 8);
    if (*param_3 == 0x43) {
      _MySetSpriteFrameTime(param_1,0xffffffff);
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
      param_1->f_17d = '\x01';
      *PTR__gRestartLevel_001c4858 = 1;
    }
  }
  return;
}


// ---- _ElectrocutionDeathFrameProc @ 0x5397a (535 bytes)

void ElectrocutionDeathFrameProc(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  short sVar1;
  undefined *puVar2;
  
  sVar1 = *(short *)&param_1->f_196;
  *(short *)&param_1->f_196 = sVar1 + -1;
  if (sVar1 < 1) {
    param_1->f_196 = '\t';
    param_1->f_197 = '\0';
    if (*param_3 == 0x37) {
      *param_3 = 0x38;
    }
    else if (*param_3 == 0x39) {
      *param_3 = 0x3a;
    }
    else if (*param_3 == 0x38) {
      sVar1 = *(short *)&param_1->f_1be;
      *(short *)&param_1->f_1be = sVar1 + 1;
      if (sVar1 < 1) {
        *param_3 = 0x37;
      }
      else {
        param_1->f_1be = '\0';
        param_1->f_1bf = '\0';
        _SWSetSpriteFrameRange(param_1,0x39,0x3a);
        _SWSetCurrentFrameIndex(param_1,0x3a);
      }
    }
    else if (*param_3 == 0x3a) {
      sVar1 = *(short *)&param_1->f_1be;
      *(short *)&param_1->f_1be = sVar1 + 1;
      if (sVar1 < 1) {
        *param_3 = 0x39;
      }
      else {
        param_1->f_1be = '\0';
        param_1->f_1bf = '\0';
        _SWSetSpriteFrameRange(param_1,0x37,0x38);
        _SWSetCurrentFrameIndex(param_1,0x38);
      }
    }
    sVar1 = *(short *)&param_1->f_198;
    *(short *)&param_1->f_198 = sVar1 + 1;
    if (10 < sVar1) {
      puVar2 = (undefined *)_SWGetSpriteMoveProc(param_1);
      if (puVar2 == PTR_SpriteSinksIntoLavaMoveProc_001c4ccc) {
        _KillHeroCommon(param_1);
        _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
        *PTR__gRestartLevel_001c4858 = 1;
      }
      else {
        _KillHeroAndDoSkeletonCollapse(param_1,0);
        _SWSetSpriteMoveDelta(param_1,0.0,0.0);
        param_1->f_17d = '\x01';
        *PTR__gRestartLevel_001c4858 = 1;
      }
    }
  }
  return;
}


// ---- _BlinkingSpriteFrameProc @ 0x53b91 (44 bytes)

void BlinkingSpriteFrameProc(SpritePtr param_1)

{
  _SWSetSpriteVisible(param_1,param_1->isVisible == '\0');
  return;
}


// ---- _HeroLandedFrameProc @ 0x53bbd (166 bytes)

void HeroLandedFrameProc(SpritePtr param_1)

{
  short sVar1;
  undefined *puVar2;
  
  sVar1 = *(short *)&param_1->f_196;
  *(short *)&param_1->f_196 = sVar1 + -1;
  if (sVar1 < 1) {
    param_1->f_172 = '\0';
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
    puVar2 = (undefined *)_SWGetSpriteMoveProc(param_1);
    if (puVar2 == PTR_LAB_001c4c90) {
      _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4098);
    }
    if (param_1->f_17e != '\0') {
      _SetHeroGroundFrame(param_1);
    }
  }
  return;
}


// ---- _FellTooFarFrameProc @ 0x53c63 (618 bytes)

void FellTooFarFrameProc(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  short sVar1;
  SpritePtr oldSpriteP;
  SpritePtr dstSpriteP;
  
  sVar1 = *(short *)&param_1->f_196;
  *(short *)&param_1->f_196 = sVar1 + -1;
  if (sVar1 < 1) {
    *param_3 = *param_3 + 1;
    param_1->f_196 = '\x05';
    param_1->f_197 = '\0';
    if (*param_3 == 0x29) {
      oldSpriteP = (SpritePtr)_NewMainSprite();
      if (oldSpriteP != (SpritePtr)0x0) {
        _SWRemoveSprite(oldSpriteP);
        dstSpriteP = _SWGetPreviousSprite
                               (*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,
                                (SpritePtr)0x0);
        if (dstSpriteP == (SpritePtr)0x0) {
          _SWAddSprite(*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,oldSpriteP);
        }
        else {
          _SWInsertSpriteBeforeSprite(oldSpriteP,dstSpriteP);
        }
        oldSpriteP->f_17e = '\0';
        oldSpriteP->f_17a = '\x01';
        oldSpriteP->f_16c = '\x04';
        oldSpriteP->f_16d = '\0';
        oldSpriteP->f_17d = '\x01';
        _SWSetSpriteFrameRange(oldSpriteP,0x32,0x32);
        _SWSetSpriteFlipX(oldSpriteP,param_1->f_177 == '\0');
        _SWSetSpriteFrameProc(oldSpriteP,(FrameProcPtr_t)0x0);
        _SWSetSpriteFrameTime(oldSpriteP,-1);
        _SWSetSpriteMoveProc(oldSpriteP,(MoveProcPtr_t)PTR_LyingSkeletonMoveProc_001c4cc8);
        _SWSetSpriteLocation
                  (oldSpriteP,(float)(int)(param_1->destFrameRect).left,
                   (float)(int)(param_1->destFrameRect).top);
        *(SpritePtr *)&oldSpriteP->f_1cc = param_1;
        *(SpritePtr *)&param_1->f_1d0 = oldSpriteP;
      }
      _SWSetSpriteDrawProc(param_1,*(DrawProcPtr_t *)PTR__gTranslucentMaskDrawProc_001c411c);
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_DeadBodyFadeFrameProc_001c4c6c);
      _SWSetSpriteFrameAdvance(param_1,0);
      param_1->f_196 = '\n';
      param_1->f_197 = '\0';
      _MMPlayStereoSound2(0,0x20,10,1,(int)(*(short *)PTR__gSoundVolume_001c48dc / 2),
                          0x3feccccccccccccd,0);
    }
  }
  return;
}


// ---- _FlinchOccasionallyFrameProc @ 0x53ecd (315 bytes)

void FlinchOccasionallyFrameProc(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  char cVar1;
  short sVar2;
  SpritePtr srcSpriteP;
  SWMovementType SVar3;
  SWMovementType SVar4;
  
  if (param_1->frameAdvance == 1) {
    if (*param_3 == 0x29) {
      _SWSetSpriteFrameAdvance(param_1,0);
      _SWSetSpriteFrameTime(param_1,0);
    }
  }
  else {
    srcSpriteP = (SpritePtr)_GetHeroSprite();
    if ((*PTR__gHeroLandedThisFrame_001c4c70 != '\0') && (srcSpriteP != (SpritePtr)0x0)) {
      SVar3 = _SWGetSpriteVertLoc(param_1);
      SVar4 = _SWGetSpriteVertLoc(srcSpriteP);
      sVar2 = (short)(int)(SVar3 - SVar4);
      if ((-8 < sVar2) && (sVar2 < 8)) {
        cVar1 = _SWBestMaskCollision(param_1,srcSpriteP);
        if (cVar1 != '\0') {
          _SWSetCurrentFrameIndex(param_1,0x27);
          _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
          _SWSetSpriteFrameAdvance(param_1,1);
          _MySetSpriteFrameTime(param_1,0x4c);
        }
      }
    }
  }
  return;
}


// ---- _RandomFlinchFrameProc @ 0x54008 (57 bytes)

void RandomFlinchFrameProc(undefined4 param_1,undefined4 param_2,undefined4 *param_3)

{
  int iVar1;
  
  *param_3 = 0;
  iVar1 = _GetRandom(1,0x23);
  if (iVar1 == 10) {
    *param_3 = 1;
  }
  return;
}


// ---- _LightningFlashFrameProc @ 0x54041 (265 bytes)

void LightningFlashFrameProc(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  int iVar1;
  
  if (param_1->isVisible == '\0') {
    iVar1 = _GetRandom(1,0x2d);
    if (iVar1 == 7) {
      _playThunderSound(param_1);
      _SWSetSpriteVisible(param_1,'\x01');
      _SWSetSpriteFrameAdvance(param_1,1);
      _SWSetSpriteFrameAdvanceMode(param_1,(void *)0x0);
    }
  }
  else {
    if ((*param_3 == 2) && (param_1->f_0fc == '\0')) {
      param_1->f_0fc = param_1->f_0fc + '\x01';
      *param_3 = 1;
    }
    if (*param_3 == 0) {
      _SWSetSpriteVisible(param_1,'\0');
      _SWSetSpriteFrameAdvance(param_1,0);
      _SWSetCurrentFrameIndex(param_1,0);
      param_1->f_0fc = '\0';
    }
  }
  return;
}


// ---- _TitleScreenFlashFrameProc @ 0x5414a (391 bytes)

void TitleScreenFlashFrameProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  undefined2 uVar3;
  SpritePtr srcSpriteP;
  
  sVar2._0_1_ = param_1->f_0fe;
  sVar2._1_1_ = param_1->f_0ff;
  if (sVar2 < 1) {
    sVar2 = *(short *)&param_1->f_100;
    *(short *)&param_1->f_100 = sVar2 + -1;
    if (sVar2 < 0) {
      uVar3 = _GetRandom(1,3);
      param_1->f_0fe = (char)uVar3;
      param_1->f_0ff = (char)((ushort)uVar3 >> 8);
      _playThunderSound(param_1);
      uVar3 = _GetRandom(10,0x1e);
      param_1->f_100 = (char)uVar3;
      param_1->f_101 = (char)((ushort)uVar3 >> 8);
    }
  }
  else {
    srcSpriteP = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164,(SpritePtr)0x0)
    ;
    if (param_1->isVisible == '\0') {
      _SWSetSpriteVisible(param_1,'\x01');
      if (srcSpriteP != (SpritePtr)0x0) {
        _SWSetSpriteFrameRange(srcSpriteP,3,3);
      }
    }
    else {
      _SWSetSpriteVisible(param_1,'\0');
      *(short *)&param_1->f_0fe = *(short *)&param_1->f_0fe + -1;
      if (srcSpriteP != (SpritePtr)0x0) {
        sVar1._0_1_ = param_1->f_0fe;
        sVar1._1_1_ = param_1->f_0ff;
        if (sVar1 < 1) {
          _SWSetSpriteFrameRange(srcSpriteP,0,2);
        }
        else {
          _SWSetSpriteFrameRange(srcSpriteP,2,2);
        }
      }
    }
  }
  return;
}


// ---- _playThunderSound @ 0x542d1 (296 bytes)

void _playThunderSound(undefined4 param_1)

{
  short sVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = _GetRandom(1,0x1e);
  iVar3 = *(short *)PTR__gSoundVolume_001c48dc * 3;
  if (iVar3 < 0) {
    iVar3 = iVar3 + 3;
  }
  sVar1 = _GetStereoPositionOfSprite(param_1,*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
  _MMPlaySoundDetailed
            (0x3a,-1 < sVar1,1,
             (int)(short)(int)(((double)iVar2 * 5.0) / -3.0 + (double)(iVar3 >> 2)),(int)sVar1,
             (double)iVar2 / -70.0 + 0.8,0);
  return;
}


// ---- _InGameLightningFrameProc @ 0x543f9 (634 bytes)

void InGameLightningFrameProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  int iVar5;
  int iVar6;
  
  if (param_1->isVisible == '\0') {
    iVar5 = _GetRandom(1,0x78);
    if (iVar5 == 7) {
      iVar6 = _GetRandom(1,0x1e);
      iVar5 = *(short *)PTR__gSoundVolume_001c48dc * 3;
      if (iVar5 < 0) {
        iVar5 = iVar5 + 3;
      }
      sVar4 = _GetStereoPositionOfSprite(param_1,*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7c);
      _MMPlaySoundDetailed
                (0x3a,-1 < sVar4,1,
                 (int)(short)(int)(((double)iVar6 * 5.0) / -3.0 + (double)(iVar5 >> 2)),(int)sVar4,
                 (double)iVar6 / -70.0 + 0.8,0);
      _SWSetSpriteVisible(param_1,'\x01');
      _SWSetSpriteFrameAdvance(param_1,1);
      _SWSetSpriteFrameAdvanceMode(param_1,(void *)0x0);
      sVar4 = _GetRandom(0,3);
      param_1->f_104 = (char)(sVar4 << 2);
      param_1->f_105 = (char)((ushort)(sVar4 << 2) >> 8);
      param_1->f_102 = '\0';
      param_1->f_103 = '\0';
      sVar4._0_1_ = param_1->f_104;
      sVar4._1_1_ = param_1->f_105;
      _SWSetCurrentFrameIndex(param_1,(int)sVar4);
    }
  }
  else {
    sVar1._0_1_ = param_1->f_102;
    sVar1._1_1_ = param_1->f_103;
    if (sVar1 < 3) {
      *(short *)&param_1->f_102 = *(short *)&param_1->f_102 + 1;
      sVar2._0_1_ = param_1->f_104;
      sVar2._1_1_ = param_1->f_105;
      sVar3._0_1_ = param_1->f_102;
      sVar3._1_1_ = param_1->f_103;
      _SWSetCurrentFrameIndex(param_1,(int)sVar2 + (int)sVar3);
    }
    else {
      _SWSetSpriteVisible(param_1,'\0');
      _SWSetSpriteFrameAdvance(param_1,0);
      _SWSetCurrentFrameIndex(param_1,0);
      param_1->f_0fc = '\0';
    }
  }
  return;
}


// ---- _BigWebFrameProc @ 0x54673 (204 bytes)

void BigWebFrameProc(SpritePtr param_1)

{
  int iVar1;
  double dVar2;
  
  iVar1 = _SWGetSpriteFrameTime(param_1);
  dVar2 = (double)iVar1 / 8.0 + (double)iVar1 + 1.8;
  _SWSetSpriteFrameTime(param_1,(int)dVar2);
  if (200.0 < dVar2) {
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
    _SWSetSpriteFrameTime(param_1,-1);
    _SWSetSpriteFrameAdvance(param_1,0);
  }
  return;
}


// ---- _RandomFlinchHeroFrameProc @ 0x5473f (227 bytes)

void RandomFlinchHeroFrameProc(int param_1,undefined4 param_2,int *param_3)

{
  short sVar1;
  int iVar2;
  
  if (*(int *)(param_1 + 0xe8) != 0xc3) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteFrameProcs.c",
               PTR____FUNCTION___27945_001c4cb4,0x318);
  }
  if (*param_3 == 0) {
    *param_3 = 0;
    iVar2 = _GetRandom(1,(int)*(short *)(param_1 + 0x108));
    if (iVar2 == 1) {
      *param_3 = 1;
    }
    sVar1 = *(short *)(param_1 + 0x10a);
    *(short *)(param_1 + 0x10a) = sVar1 + 1;
    if (4 < sVar1) {
      *(short *)(param_1 + 0x108) = *(short *)(param_1 + 0x108) + 1;
      *(undefined2 *)(param_1 + 0x10a) = 0;
    }
  }
  else {
    *param_3 = 0;
  }
  return;
}


// ---- _RandomFlinchEnemyFrameProc @ 0x54822 (114 bytes)

void RandomFlinchEnemyFrameProc(undefined4 param_1,undefined4 param_2,int *param_3)

{
  int iVar1;
  
  if (*param_3 == 1) {
    *param_3 = 1;
    iVar1 = _GetRandom(1,4);
    if (iVar1 == 1) {
      iVar1 = _GetRandom(1,2);
      if (iVar1 == 1) {
        *param_3 = 0;
      }
      else {
        *param_3 = 2;
      }
    }
  }
  else {
    *param_3 = 1;
  }
  return;
}


// ---- _PushupDelayFrameProc @ 0x54894 (398 bytes)

void PushupDelayFrameProc(SpritePtr param_1)

{
  short sVar1;
  
  _SWSetSpriteFrameTime(param_1,0x10);
  if (PTR__gKeys_001c4af8[2] == '\0') {
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
    _SWSetSpriteFrameTime(param_1,-1);
    _SWSetSpriteFrameAdvance(param_1,0);
  }
  else {
    _SWSetSpriteFrameRange(param_1,0xf,0xf);
    _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
    sVar1 = *(short *)&param_1->f_188;
    *(short *)&param_1->f_188 = sVar1 + -1;
    if (sVar1 < 1) {
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_DoPushupsFrameProc_001c4ce8);
      _SWSetSpriteFrameAdvance(param_1,1);
      _SWSetSpriteFrameTime(param_1,400);
      _SWSetSpriteFrameRange(param_1,0x10,0x11);
      _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
      param_1->f_18a = '\b';
      param_1->f_18b = '\0';
      _MMPlaySound(0x2d,9,1);
      param_1->f_18c = '.';
      param_1->f_18d = '\0';
    }
  }
  return;
}


// ---- _DoPushupsFrameProc @ 0x54a22 (371 bytes)

void DoPushupsFrameProc(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  short sVar1;
  undefined2 uVar2;
  int iVar3;
  
  sVar1 = *(short *)&param_1->f_18a;
  *(short *)&param_1->f_18a = sVar1 + -1;
  if (sVar1 < 1) {
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_PushupDelayFrameProc_001c4c8c);
    _MySetSpriteFrameTime(param_1,800);
    _SWSetSpriteFrameAdvance(param_1,0);
    uVar2 = _GetRandom(0x2d0,0x4b0);
    param_1->f_188 = (char)uVar2;
    param_1->f_189 = (char)((ushort)uVar2 >> 8);
    _SWSetCurrentFrameIndex(param_1,0x11);
    _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
  }
  else if (*param_3 == 0x10) {
    sVar1._0_1_ = param_1->f_18c;
    sVar1._1_1_ = param_1->f_18d;
    _MMPlaySound((int)sVar1,9,1);
    sVar1 = *(short *)&param_1->f_18c;
    *(short *)&param_1->f_18c = sVar1 + 1;
    if (0x2f < sVar1) {
      param_1->f_18a = '\0';
      param_1->f_18b = '\0';
    }
    iVar3 = _SWGetSpriteFrameTime(param_1);
    _SWSetSpriteFrameTime(param_1,(int)((double)iVar3 + 170.0));
  }
  return;
}


// ---- _PauseThenFadeStatueFrameProc @ 0x54b95 (979 bytes)

/* WARNING: Removing unreachable block (ram,0x00054c39) */

void PauseThenFadeStatueFrameProc(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  int iVar1;
  undefined4 uVar2;
  char cVar3;
  undefined1 uStack_1e;
  char cStack_1d;
  SpritePtr pSStack_1c;
  char cStack_16;
  char cStack_15;
  undefined2 uStack_14;
  char cStack_11;
  SpritePtr pSStack_10;
  
  pSStack_1c = param_1;
  if (param_1->userData != 0xc2) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteFrameProcs.c",
               PTR____FUNCTION___28039_001c4ca0,0x3a8);
  }
  if (*param_3 == 0) {
    _FadeMusicNow(3);
    _StopAllChannels();
    cVar3 = _IsMoviePlaying();
    if (cVar3 == '\0') {
      if (*PTR__gEditorIsRunning_001c495c == '\0') {
        uStack_14 = 2;
        cStack_16 = _MMDoSaveGameDialog();
        if (cStack_16 == '\0') {
          _WriteEventToMovieFile(9,1);
        }
        else {
          _WriteEventToMovieFile(8,1);
        }
      }
    }
    else {
      cStack_11 = _GetEventFromMovieFile(&cStack_1d,&uStack_1e);
      if ((cStack_11 == '\0') || (cStack_1d != '\b')) {
        if ((cStack_11 == '\0') || (cStack_1d != '\t')) {
          _StopGameMovie();
          cStack_16 = '\0';
        }
        else {
          cStack_16 = '\0';
        }
      }
      else {
        cStack_16 = '\x01';
      }
      _ResetKeys();
    }
    if (cStack_16 != '\0') {
      _RestoreJacksLives();
    }
    _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gStatsSpriteWorldP_001c48f4,1);
    _SWUpdateSpriteWorld(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,1);
    if (cStack_16 != '\0') {
      _WriteRoomInfoToFile();
      _WriteCStringToFile("SAVED GAME\r");
      _WriteCStringToFile("Time spent so far: ");
      _WriteNumberToFile(*(int *)PTR__gTimeSpentInMansion_001c46fc / 0x3c);
      _WriteCStringToFile(" minutes and ");
      _WriteNumberToFile(*(int *)PTR__gTimeSpentInMansion_001c46fc % 0x3c);
      _WriteCStringToFile(" seconds.\r");
      _MyDelay(0x1e);
    }
    cStack_15 = cStack_16;
  }
  else {
    cStack_15 = '\x01';
  }
  if (cStack_15 == '\0') {
    uVar2._0_1_ = pSStack_1c->f_114;
    uVar2._1_1_ = pSStack_1c->f_115;
    uVar2._2_1_ = pSStack_1c->f_116;
    uVar2._3_1_ = pSStack_1c->f_117;
    _RemoveSpriteFromObjectArray(uVar2);
    *(undefined4 *)PTR__gLastSavePedestalObjectID_001c49f8 = 0;
    _SWSetCurrentFrameIndex(param_1,0);
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_PauseThenUnfreezeFrameProc_001c4cf0);
    _SWSetSpriteFrameTime(param_1,0);
    _SWSetSpriteFrameAdvance(param_1,0);
    pSStack_1c->f_104 = '\x1e';
    pSStack_1c->f_105 = '\0';
    pSStack_10 = *(SpritePtr *)&param_1->f_110;
    if (pSStack_10 != (SpritePtr)0x0) {
      _SWRemoveSpriteFromAnimation(pSStack_10,'\x01');
    }
  }
  else {
    _MMPlaySound(0x33,9,1);
    _SWSetSpriteFrameAdvance(param_1,0);
    _SWSetSpriteFrameTime(param_1,0);
    pSStack_1c->f_104 = '\x02';
    pSStack_1c->f_105 = '\0';
    pSStack_1c->f_106 = '\x01';
    pSStack_1c->f_107 = '\0';
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_LAB_001c4cd8);
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4ca8);
    iVar1._0_1_ = pSStack_1c->f_110;
    iVar1._1_1_ = pSStack_1c->f_111;
    iVar1._2_1_ = pSStack_1c->f_112;
    iVar1._3_1_ = pSStack_1c->f_113;
    if (iVar1 == 0) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteFrameProcs.c",
                 PTR____FUNCTION___28039_001c4ca0,0x40b);
    }
    _SWSetSpriteMoveProc(*(SpritePtr *)&pSStack_1c->f_110,(MoveProcPtr_t)PTR_LAB_001c4cb8);
  }
  return;
}


// ---- _PauseThenUnfreezeFrameProc @ 0x54f68 (419 bytes)

void PauseThenUnfreezeFrameProc(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  short sVar1;
  int iVar2;
  SpritePtr pSVar3;
  SWMovementType vertLoc;
  SWMovementType horizLoc;
  
  sVar1 = *(short *)&param_1->f_104;
  *(short *)&param_1->f_104 = sVar1 + -1;
  if (sVar1 < 1) {
    if (*param_3 == 1) {
      pSVar3 = (SpritePtr)_NewMainSprite();
      if (pSVar3 != (SpritePtr)0x0) {
        vertLoc = _SWGetSpriteVertLoc(param_1);
        horizLoc = _SWGetSpriteHorizLoc(param_1);
        _SWMoveSprite(pSVar3,horizLoc,vertLoc);
        _UpdateHaloSpritePosition(pSVar3);
      }
      if (*(SpritePtr *)&param_1->f_110 != (SpritePtr)0x0) {
        _SWRemoveSpriteFromAnimation(*(SpritePtr *)&param_1->f_110,'\x01');
      }
      _SWRemoveSpriteFromAnimation(param_1,'\x01');
      iVar2._0_1_ = param_1->f_114;
      iVar2._1_1_ = param_1->f_115;
      iVar2._2_1_ = param_1->f_116;
      iVar2._3_1_ = param_1->f_117;
      if (iVar2 == 0) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteFrameProcs.c"
                   ,PTR____FUNCTION___28108_001c4c98,0x44c);
      }
      _SWSetCurrentFrameIndex(*(SpritePtr *)&param_1->f_114,1);
    }
    else {
      _SWRemoveSpriteFromAnimation(param_1,'\x01');
      pSVar3 = (SpritePtr)_GetHeroSprite();
      if (pSVar3 != (SpritePtr)0x0) {
        _SWSetSpriteVisible(pSVar3,'\x01');
      }
    }
    if (*PTR__gMusicIsOn_001c47c0 != '\0') {
      _FadeMusicIn(4);
    }
    _MMResumeGameTimer();
    _UnpauseAllSpriteLayers(*(undefined4 *)PTR__gSpriteWorldP_001c40d0);
    *PTR__gLayersArePausedForSaveAnim_001c487c = 0;
    *PTR__gStatuesAreDoneFading_001c4934 = 0;
  }
  return;
}


// ---- _StatueFadeFrameProc @ 0x5510b (146 bytes)

void StatueFadeFrameProc(SpritePtr param_1)

{
  short sVar1;
  uint uVar2;
  
  sVar1 = *(short *)&param_1->f_104;
  *(short *)&param_1->f_104 = sVar1 + -1;
  if (sVar1 < 1) {
    param_1->f_104 = '\x02';
    param_1->f_105 = '\0';
    uVar2 = _SWGetSpriteTranslucencyLevel(param_1);
    if (uVar2 < 0x5dd) {
      *PTR__gStatuesAreDoneFading_001c4934 = 1;
    }
    else {
      _SWSetSpriteTranslucencyLevel(param_1,uVar2 - 0x5dc);
      _SWInvalidateSprite(param_1);
    }
  }
  return;
}


// ---- _ZzzzFadeFrameProc @ 0x5519d (379 bytes)

void ZzzzFadeFrameProc(SpritePtr param_1)

{
  uint uVar1;
  float fVar2;
  float fVar3;
  
  _SWSetSpriteVisible(param_1,'\x01');
  uVar1 = _SWGetSpriteTranslucencyLevel(param_1);
  fVar2 = (float)(uVar1 >> 0x10) * 65536.0 + (float)(uVar1 & 0xffff);
  fVar3 = fVar2 / -65535.0 + 2.0;
  if (fVar2 <= 300.0) {
    _SWRemoveSpriteFromAnimation(param_1,'\x01');
  }
  else {
    _SWSetSpriteScaledFactor(param_1,fVar3 / 3.0,fVar3 / 3.0);
    fVar2 = fVar2 - 300.0;
    if (fVar2 <= 0.0) {
      fVar2 = 0.0;
    }
    _SWSetSpriteTranslucencyLevel
              (param_1,(int)(fVar2 - (float)(-(uint)(2.1474836e+09 <= fVar2) & 0x4f000000)) +
                       (uint)(2.1474836e+09 <= fVar2) * -0x80000000 |
                       -(uint)(4.2949673e+09 <= fVar2));
    _SWInvalidateSprite(param_1);
  }
  return;
}


// ---- _DeadBodyFadeFrameProc @ 0x55318 (276 bytes)

void DeadBodyFadeFrameProc(SpritePtr param_1)

{
  short sVar1;
  uint uVar2;
  
  if (param_1 == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteFrameProcs.c",
               PTR____FUNCTION___28181_001c4ca4,0x4b4);
  }
  sVar1 = *(short *)&param_1->f_196;
  *(short *)&param_1->f_196 = sVar1 + -1;
  if (sVar1 < 1) {
    param_1->f_196 = '\x01';
    param_1->f_197 = '\0';
    if (param_1->f_17d == '\0') {
      param_1->f_17d = '\x01';
      *PTR__gRestartLevel_001c4858 = 1;
    }
    uVar2 = _SWGetSpriteTranslucencyLevel(param_1);
    if (uVar2 < 0x7d1) {
      _SWSetSpriteDrawProc(param_1,(DrawProcPtr_t)PTR_DrawNothingDrawProc_001c4cd4);
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
    }
    else {
      uVar2 = _SWGetSpriteTranslucencyLevel(param_1);
      _SWSetSpriteTranslucencyLevel(param_1,uVar2 - 2000);
      _SWInvalidateSprite(param_1);
    }
  }
  return;
}


// ---- _FadeInFrameProc @ 0x5542c (256 bytes)

void FadeInFrameProc(SpritePtr param_1)

{
  uint uVar1;
  uint uStack_10;
  
  uVar1 = _SWGetSpriteTranslucencyLevel(param_1);
  if (uVar1 < 0xf82f) {
    uStack_10 = _SWGetSpriteTranslucencyLevel(param_1);
    if ((param_1->userData < 0x50) || (0x5e < param_1->userData)) {
      if ((param_1->userData < 0x45) || (0x4f < param_1->userData)) {
        if (param_1->userData == 0xed) {
          uStack_10 = uStack_10 + 0x1e0;
        }
        else {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteFrameProcs.c"
                     ,PTR____FUNCTION___28205_001c4c9c,0x506);
        }
      }
      else {
        uStack_10 = uStack_10 + 0x640;
      }
    }
    else {
      uStack_10 = uStack_10 + 0x514;
    }
    _SWSetSpriteTranslucencyLevel(param_1,uStack_10);
    _SWInvalidateSprite(param_1);
  }
  else {
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
  }
  return;
}


// ---- _FadeOutFrameProc @ 0x5552c (126 bytes)

void FadeOutFrameProc(SpritePtr param_1)

{
  undefined *puVar1;
  
  puVar1 = (undefined *)_SWGetSpriteTranslucencyLevel(param_1);
  if (&DAT_00002328 < puVar1) {
    _SWSetSpriteTranslucencyLevel(param_1,(uint)(puVar1 + -9000));
    _SWInvalidateSprite(param_1);
  }
  else {
    _SWRemoveSpriteFromAnimation(param_1,'\x01');
    *(int *)PTR__gNumWeatherSpritesOnGround_001c4cc4 =
         *(int *)PTR__gNumWeatherSpritesOnGround_001c4cc4 + -1;
  }
  return;
}


// ---- _AnimatedPaintingFrameProc @ 0x555aa (290 bytes)

void AnimatedPaintingFrameProc(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  short sVar1;
  short sVar2;
  undefined2 uVar3;
  
  sVar1 = *(short *)&param_1->f_0fe;
  *(short *)&param_1->f_0fe = sVar1 + -1;
  if (sVar1 < 1) {
    if (param_1->frameAdvance == 0) {
      if (param_3 == (int *)0x0) {
        _SWSetSpriteFrameAdvance(param_1,1);
      }
      else {
        _SWSetSpriteFrameAdvance(param_1,-1);
      }
    }
    else if ((*param_3 == param_1->maxFrames + -1) || (*param_3 == 0)) {
      _SWSetSpriteFrameAdvance(param_1,0);
      sVar1._0_1_ = param_1->f_102;
      sVar1._1_1_ = param_1->f_103;
      sVar2._0_1_ = param_1->f_100;
      sVar2._1_1_ = param_1->f_101;
      uVar3 = _GetRandom((int)sVar2,(int)sVar1);
      param_1->f_0fe = (char)uVar3;
      param_1->f_0ff = (char)((ushort)uVar3 >> 8);
      if ((param_1->userData == 0x80) && (*param_3 == param_1->maxFrames + -1)) {
        uVar3 = _GetRandom(0xf,0x23);
        param_1->f_0fe = (char)uVar3;
        param_1->f_0ff = (char)((ushort)uVar3 >> 8);
      }
    }
  }
  return;
}


// ---- _RandomFrameProc @ 0x556cc (70 bytes)

void RandomFrameProc(int param_1,undefined4 param_2,int *param_3)

{
  short sVar1;
  
  do {
    sVar1 = _GetRandom(*(undefined4 *)(param_1 + 0x98),*(undefined4 *)(param_1 + 0x9c));
  } while ((int)sVar1 == *param_3);
  *param_3 = (int)sVar1;
  return;
}


// ---- _HaloFlickerFrameProc @ 0x55712 (178 bytes)

void HaloFlickerFrameProc(SpritePtr param_1)

{
  if (param_1->misc1 == 1) {
    param_1->userData = param_1->userData + 0x90;
    if (0xd8ff < param_1->userData) {
      param_1->misc1 = -param_1->misc1;
    }
  }
  else {
    param_1->userData = param_1->userData + -0x90;
    if (param_1->userData < 0xb8ff) {
      param_1->misc1 = -param_1->misc1;
    }
  }
  _SWSetSpriteTranslucency(param_1,param_1->userData);
  return;
}


// ---- _ResetSpriteFrameProc @ 0x557c4 (23 bytes)

void ResetSpriteFrameProc(undefined4 param_1,undefined4 param_2,undefined4 *param_3)

{
  *param_3 = 0;
  return;
}


// ---- _PauseThenReappearFrameProc @ 0x557db (688 bytes)

void PauseThenReappearFrameProc(SpritePtr param_1)

{
  short sVar1;
  
  sVar1 = *(short *)&param_1->f_194;
  *(short *)&param_1->f_194 = sVar1 + -1;
  if (sVar1 < 1) {
    if (*(short *)PTR__gCurrentLevel_001c49bc != *(short *)PTR__gLastLevelPositionWasSaved_001c4c78)
    {
      _SWRemoveSprite(param_1);
      _LoadLevel((int)*(short *)PTR__gLastLevelPositionWasSaved_001c4c78,0);
      _SetUpLevelForPlaying();
      _DrawLevel();
      _DoGameFadeToNormal(0x1e);
      if (*(short *)PTR__gCurrentMusicIndex_001c476c != *(short *)PTR__gLastMusicIndex_001c4c80) {
        *(undefined2 *)PTR__gCurrentMusicIndex_001c476c =
             *(undefined2 *)PTR__gLastMusicIndex_001c4c80;
        _PlayBackgroundMusic((int)*(short *)PTR__gCurrentMusicIndex_001c476c);
      }
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,param_1);
    }
    _SWSetSpriteLocation
              (param_1,(float)(int)*(short *)PTR__gStartingCol_001c4c74,
               (float)(int)*(short *)PTR__gStartingRow_001c4c7c);
    param_1->f_177 = *PTR__gStartingFacingRight_001c4c94;
    if (*(short *)PTR__gStartingSpriteState_001c4c68 == 1) {
      _SWSetSpriteFrameRange(param_1,0x12,0x19);
      _SWSetSpriteFlipX(param_1,'\0');
    }
    else if (*(short *)PTR__gStartingSpriteState_001c4c68 == 2) {
      _SWSetSpriteFrameRange(param_1,0x1e,0x21);
      _SWSetSpriteFlipX(param_1,'\0');
    }
    else if (*(short *)PTR__gStartingSpriteState_001c4c68 == 3) {
      _SWSetSpriteFrameRange(param_1,0x1f,0x1f);
      _SWSetSpriteFlipX(param_1,'\0');
    }
    else {
      _SWSetCurrentFrameIndex(param_1,0);
      _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
    }
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4cbc);
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_LAB_001c4cb0);
    _SWSetSpriteFrameTime(param_1,0xfa);
    _SWSetSpriteFrameAdvance(param_1,0);
    _ResetKeys();
    _WriteEventToMovieFile(0xb,1);
  }
  return;
}


// ---- _PuffOfSmokeFrameProc @ 0x55a8b (42 bytes)

void PuffOfSmokeFrameProc(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  if (*param_3 == 0) {
    _SWRemoveSpriteFromAnimation(param_1,'\x01');
  }
  return;
}


// ---- _ShieldSpriteFrameProc @ 0x55ab5 (104 bytes)

void ShieldSpriteFrameProc(int param_1,undefined4 param_2,int *param_3)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x104);
  *(int *)(param_1 + 0x104) = iVar1 + 1;
  if ((0x13 < iVar1) && (*param_3 = *param_3 + 1, *(int *)(param_1 + 0x80) <= *param_3)) {
    *param_3 = 0;
    *(undefined4 *)(param_1 + 0x104) = 0;
  }
  return;
}


// ---- _SkullSpiderFrameProc @ 0x55b1d (187 bytes)

void SkullSpiderFrameProc(int param_1)

{
  short sVar1;
  
  sVar1 = *(short *)(param_1 + 0x106);
  *(short *)(param_1 + 0x106) = sVar1 + -1;
  if (sVar1 < 1) {
    *(undefined2 *)(param_1 + 0x106) = *(undefined2 *)(param_1 + 0x10a);
    _MyAdvanceSpriteFrame(param_1,param_1 + 0x108);
    _MMSetTempStereoFadeDist(600);
    _MMPlayStereoSoundAdjustedVol
              (param_1,(int)*(short *)(param_1 + 0x104),0xd,3,0xfa,0x3ff0000000000000);
  }
  return;
}


// ---- _ScorpionFrameProc @ 0x55bd8 (88 bytes)

void ScorpionFrameProc(int param_1)

{
  short sVar1;
  
  sVar1 = *(short *)(param_1 + 0xfc);
  *(short *)(param_1 + 0xfc) = sVar1 + -1;
  if (sVar1 < 1) {
    *(undefined2 *)(param_1 + 0xfc) = 7;
    _MyAdvanceSpriteFrame(param_1,param_1 + 0xfe);
  }
  return;
}


// ---- _SnakeFrameProc @ 0x55c30 (88 bytes)

void SnakeFrameProc(int param_1)

{
  short sVar1;
  
  sVar1 = *(short *)(param_1 + 0xfc);
  *(short *)(param_1 + 0xfc) = sVar1 + -1;
  if (sVar1 < 1) {
    *(undefined2 *)(param_1 + 0xfc) = 0xc;
    _MyAdvanceSpriteFrame(param_1,param_1 + 0xfe);
  }
  return;
}


// ---- _BirdFrameProc @ 0x55c88 (208 bytes)

void BirdFrameProc(int param_1,undefined4 param_2,int *param_3)

{
  short sVar1;
  
  sVar1 = *(short *)(param_1 + 0x104);
  *(short *)(param_1 + 0x104) = sVar1 + -1;
  if (sVar1 < 1) {
    *(undefined2 *)(param_1 + 0x104) = 6;
    _MyAdvanceSpriteFrame(param_1,param_1 + 0x106);
    if ((*(int *)PTR__gCurrentGameMode_001c42d0 != 3) && (*param_3 == *(int *)(param_1 + 0x84) + -1)
       ) {
      _MMSetTempStereoFadeDist(500);
      _MMPlayStereoSoundAdjustedVol(param_1,7,0xb,3,400,0x3ff0000000000000);
    }
  }
  return;
}


// ---- _MedusaFrameProc @ 0x55d58 (96 bytes)

void MedusaFrameProc(int param_1)

{
  short sVar1;
  
  sVar1 = *(short *)(param_1 + 0xfe);
  *(short *)(param_1 + 0xfe) = sVar1 + -1;
  if (sVar1 < 1) {
    *(undefined2 *)(param_1 + 0xfe) = *(undefined2 *)(param_1 + 0xfc);
    _MyAdvanceSpriteFrame(param_1,param_1 + 0x100);
  }
  return;
}


// ---- _SpiderFrameProc @ 0x55db8 (96 bytes)

void SpiderFrameProc(int param_1)

{
  short sVar1;
  
  sVar1 = *(short *)(param_1 + 0x112);
  *(short *)(param_1 + 0x112) = sVar1 + -1;
  if (sVar1 < 1) {
    *(undefined2 *)(param_1 + 0x112) = *(undefined2 *)(param_1 + 0x110);
    _MyAdvanceSpriteFrame(param_1,param_1 + 0x114);
  }
  return;
}


// ---- _SkeletonFishFrameProc @ 0x55e18 (96 bytes)

void SkeletonFishFrameProc(int param_1)

{
  short sVar1;
  
  sVar1 = *(short *)(param_1 + 0x102);
  *(short *)(param_1 + 0x102) = sVar1 + -1;
  if (sVar1 < 1) {
    *(undefined2 *)(param_1 + 0x102) = *(undefined2 *)(param_1 + 0x100);
    _MyAdvanceSpriteFrame(param_1,param_1 + 0x104);
  }
  return;
}


// ---- _FishTurnAroundFrameProc @ 0x55e78 (240 bytes)

void FishTurnAroundFrameProc(SpritePtr param_1,undefined4 param_2,undefined4 *param_3)

{
  short sVar1;
  short sVar2;
  uchar uVar3;
  
  sVar1 = *(short *)&param_1->f_102;
  *(short *)&param_1->f_102 = sVar1 + -1;
  if (sVar1 < 1) {
    uVar3 = param_1->f_101;
    param_1->f_102 = param_1->f_100;
    param_1->f_103 = uVar3;
    _MyAdvanceSpriteFrame(param_1,&param_1->f_104);
    if ((short)((short)*param_3 - *(short *)&param_1->f_0fe) == 7) {
      sVar1._0_1_ = param_1->f_0fe;
      sVar1._1_1_ = param_1->f_0ff;
      sVar2._0_1_ = param_1->f_0fe;
      sVar2._1_1_ = param_1->f_0ff;
      _SWSetSpriteFrameRange(param_1,sVar2 + 7,sVar1 + 8);
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_SkeletonFishFrameProc_001c4c64);
      param_1->f_100 = '\x06';
      param_1->f_101 = '\0';
      param_1->f_102 = '\x06';
      param_1->f_103 = '\0';
    }
  }
  return;
}


// ---- _EyeballFrameProc @ 0x55f68 (170 bytes)

void EyeballFrameProc(SpritePtr param_1,undefined4 param_2,undefined4 *param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = _GetRandom(1,5);
  if (iVar1 == 1) {
    uVar2 = _GetRandom(0,6);
    *param_3 = uVar2;
    iVar1 = _GetRandom(1,0xf);
    if (iVar1 == 1) {
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_EyeballBlinkFrameProc_001c4c84);
      _SWSetSpriteFrameAdvance(param_1,1);
      *param_3 = 10;
      _SWSetSpriteFrameTime(param_1,100);
    }
  }
  return;
}


// ---- _EyeballBlinkFrameProc @ 0x56012 (149 bytes)

void EyeballBlinkFrameProc(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  if (*param_3 < 0xd) {
    if ((*param_3 < 10) && (param_1->frameAdvance < 0)) {
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_EyeballFrameProc_001c41e4);
      _SWSetSpriteFrameAdvance(param_1,0);
      *param_3 = 3;
      _SWSetSpriteFrameTime(param_1,0x53);
    }
  }
  else {
    _SWSetSpriteFrameAdvance(param_1,-1);
  }
  return;
}


// ---- _SkeletonUncollapseFrameProc @ 0x560a7 (249 bytes)

void SkeletonUncollapseFrameProc(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  short sVar1;
  undefined2 uVar2;
  
  sVar1 = *(short *)&param_1->f_0fe;
  *(short *)&param_1->f_0fe = sVar1 + -1;
  if (sVar1 < 1) {
    _MyAdvanceSpriteFrame(param_1,&param_1->f_100);
    uVar2 = (undefined2)(int)((double)-*param_3 / -1.7 + 7.0);
    param_1->f_0fe = (char)uVar2;
    param_1->f_0ff = (char)((ushort)uVar2 >> 8);
    if (*param_3 == 1) {
      _MMPlaySound(0x34,0xb,3);
    }
    if (*param_3 == 7) {
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_SkeletonPauseThenShakeFrameProc_001c4cd0);
      uVar2 = _GetRandom(0x14,0x1e);
      param_1->f_0fe = (char)uVar2;
      param_1->f_0ff = (char)((ushort)uVar2 >> 8);
    }
  }
  return;
}


// ---- _SkeletonPauseThenShakeFrameProc @ 0x561a0 (482 bytes)

void SkeletonPauseThenShakeFrameProc(SpritePtr param_1)

{
  short sVar1;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SWMovementType SVar2;
  SWMovementType SVar3;
  SpritePtr pSStack_14;
  
  sVar1 = *(short *)&param_1->f_0fe;
  *(short *)&param_1->f_0fe = sVar1 + -1;
  if (sVar1 < 1) {
    pSStack_14 = (SpritePtr)_GetHeroSprite();
    if (pSStack_14 == (SpritePtr)0x0) {
      pSStack_14 = _SWGetPreviousSprite
                             (*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,(SpritePtr)0x0)
      ;
    }
    if (pSStack_14 == (SpritePtr)0x0) {
      param_1->f_10a = '\0';
      _SWSetSpriteFrameRange(param_1,8,9);
    }
    else {
      SVar2 = _SWGetSpriteHorizLoc(param_1);
      _SWGetSpriteWidth(param_1);
      SVar3 = _SWGetSpriteHorizLoc(pSStack_14);
      _SWGetSpriteWidth(pSStack_14);
      _SWSetSpriteFrameRange(param_1,8,9);
      if ((float)(int)(short)(int)((float)extraout_ST0_00 / 2.0 + SVar3) + *(float *)&param_1->f_114
          < (float)(int)(short)(int)((float)extraout_ST0 / 2.0 + SVar2)) {
        param_1->f_10a = '\0';
      }
      else {
        param_1->f_10a = '\x01';
      }
      _SWSetSpriteFlipX(param_1,param_1->f_10a);
    }
    param_1->f_10c = '\x06';
    param_1->f_10d = '\0';
    _SWRemoveSprite(param_1);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gEnemySpriteLayerP_001c40b0,param_1);
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_SkeletonShakeOffFrameProc_001c4cdc);
    param_1->f_0fe = '\x05';
    param_1->f_0ff = '\0';
  }
  return;
}


// ---- _SkeletonShakeOffFrameProc @ 0x56382 (301 bytes)

void SkeletonShakeOffFrameProc(SpritePtr param_1)

{
  short sVar1;
  
  sVar1 = *(short *)&param_1->f_0fe;
  *(short *)&param_1->f_0fe = sVar1 + -1;
  if (sVar1 < 1) {
    param_1->f_0fe = '\x05';
    param_1->f_0ff = '\0';
    _MyAdvanceSpriteFrame(param_1,&param_1->f_100);
    sVar1._0_1_ = param_1->f_10c;
    sVar1._1_1_ = param_1->f_10d;
    if ((*(short *)&param_1->f_10c / 2) * 2 == (int)sVar1) {
      sVar1 = _CountSounds(0x17);
      if (sVar1 < 3) {
        _MMPlayStereoSound(param_1,0x17,0xb,1);
      }
      else {
        _MMPlayStereoSound(param_1,0x17,0xb,3);
      }
    }
    sVar1 = *(short *)&param_1->f_10c;
    *(short *)&param_1->f_10c = sVar1 + -1;
    if (sVar1 < 0) {
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_LAB_001c4ce4);
      param_1->f_0fe = '\x1e';
      param_1->f_0ff = '\0';
    }
  }
  return;
}


// ---- _SkeletonPauseThenWalkFrameProc @ 0x564af (227 bytes)

void SkeletonPauseThenWalkFrameProc(SpritePtr param_1)

{
  short sVar1;
  undefined2 uVar2;
  short sStack_e;
  
  sVar1 = *(short *)&param_1->f_0fe;
  *(short *)&param_1->f_0fe = sVar1 + -1;
  if (sVar1 < 1) {
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4cec);
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_WalkingSkeletonFrameProc_001c4cc0);
    sStack_e = (short)(int)(*(float *)&param_1->f_114 + *(float *)&param_1->f_114 + 4.0);
    if (0xb < sStack_e) {
      sStack_e = 0xc;
    }
    uVar2 = (undefined2)(0x3c / (longlong)(int)sStack_e);
    param_1->f_0fc = (char)uVar2;
    param_1->f_0fd = (char)((ushort)uVar2 >> 8);
    uVar2 = (undefined2)(0x3c / (longlong)(int)sStack_e);
    param_1->f_0fe = (char)uVar2;
    param_1->f_0ff = (char)((ushort)uVar2 >> 8);
  }
  return;
}


// ---- _WalkingSkeletonFrameProc @ 0x56592 (96 bytes)

void WalkingSkeletonFrameProc(int param_1)

{
  short sVar1;
  
  sVar1 = *(short *)(param_1 + 0xfe);
  *(short *)(param_1 + 0xfe) = sVar1 + -1;
  if (sVar1 < 1) {
    *(undefined2 *)(param_1 + 0xfe) = *(undefined2 *)(param_1 + 0xfc);
    _MyAdvanceSpriteFrame(param_1,param_1 + 0x100);
  }
  return;
}


// ---- _MiscSpriteFrameProc @ 0x565f2 (96 bytes)

void MiscSpriteFrameProc(int param_1)

{
  short sVar1;
  
  sVar1 = *(short *)(param_1 + 0x110);
  *(short *)(param_1 + 0x110) = sVar1 + -1;
  if (sVar1 < 1) {
    *(undefined2 *)(param_1 + 0x110) = *(undefined2 *)(param_1 + 0x112);
    _MyAdvanceSpriteFrame(param_1,param_1 + 0x114);
  }
  return;
}


// ---- _KeyGlowFrameProc @ 0x56652 (385 bytes)

void KeyGlowFrameProc(SpritePtr param_1)

{
  SpriteRec *pSVar1;
  float fVar2;
  short sStack_12;
  float fStack_10;
  
  pSVar1 = param_1->mountParentP;
  if (pSVar1 != (SpriteRec *)0x0) {
    sStack_12 = (short)pSVar1->curFrameIndex;
    if (pSVar1->maxFrames / 2 <= (int)sStack_12) {
      sStack_12 = sStack_12 - (short)(pSVar1->maxFrames / 2);
    }
    if (sStack_12 == 0) {
      fStack_10 = 1.2;
    }
    else if ((sStack_12 == 1) || (sStack_12 == 7)) {
      fStack_10 = 1.1;
    }
    else if ((sStack_12 == 2) || (sStack_12 == 6)) {
      fStack_10 = 1.0;
    }
    else if ((sStack_12 == 3) || (sStack_12 == 5)) {
      fStack_10 = 0.9;
    }
    else if (sStack_12 == 4) {
      fStack_10 = 0.8;
    }
    fVar2 = (float)param_1->misc1 * ((fStack_10 + 0.95) / 2.0);
    if (fVar2 <= 0.0) {
      fVar2 = 0.0;
    }
    _SWSetSpriteTranslucency
              (param_1,(int)(fVar2 - (float)(-(uint)(2.1474836e+09 <= fVar2) & 0x4f000000)) +
                       (uint)(2.1474836e+09 <= fVar2) * -0x80000000 |
                       -(uint)(4.2949673e+09 <= fVar2));
  }
  return;
}


// ---- _HintQuestionMarkFrameProc @ 0x567d3 (419 bytes)

void HintQuestionMarkFrameProc(SpritePtr param_1)

{
  float fVar1;
  longdouble lVar2;
  
  lVar2 = (longdouble)_SWCos((double)*(float *)&param_1->f_11c);
  _SWOffsetSprite(param_1,0.0,(float)((double)lVar2 / 11.0));
  *(float *)&param_1->f_11c = *(float *)&param_1->f_11c + 3.0;
  if (360.0 <= *(float *)&param_1->f_11c) {
    *(float *)&param_1->f_11c = *(float *)&param_1->f_11c - 360.0;
  }
  fVar1 = param_1->drawData->rotation;
  lVar2 = (longdouble)_SWCos((double)*(float *)&param_1->f_118);
  _SWSetSpriteRotation(param_1,(float)((double)lVar2 / 8.0 + (double)fVar1));
  *(float *)&param_1->f_118 = *(float *)&param_1->f_118 + 2.0;
  if (360.0 <= *(float *)&param_1->f_118) {
    *(float *)&param_1->f_118 = *(float *)&param_1->f_118 - 360.0;
  }
  return;
}


// ---- _FlameFrameProc @ 0x56976 (69 bytes)

void FlameFrameProc(int param_1,undefined4 param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *param_3;
  do {
    iVar2 = _GetRandom(0,*(int *)(param_1 + 0x84) + -1);
    *param_3 = iVar2;
  } while (*param_3 == (int)(short)iVar1);
  return;
}


// ---- _CandleFrameProc @ 0x569bb (108 bytes)

void CandleFrameProc(int param_1,undefined4 param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *param_3;
  do {
    iVar2 = _GetRandom(0,3);
    *param_3 = iVar2;
  } while (*param_3 == (int)(short)iVar1);
  if (*(int *)(param_1 + 0xfc) != 0) {
    _SWSetCurrentFrameIndex(*(SpritePtr *)(param_1 + 0xfc),*param_3);
  }
  return;
}


// ---- _LanternFrameProc @ 0x56a27 (63 bytes)

void LanternFrameProc(undefined4 param_1,undefined4 param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *param_3;
  do {
    iVar2 = _GetRandom(0,3);
    *param_3 = iVar2;
  } while (*param_3 == (int)(short)iVar1);
  return;
}


// ---- _TrapDoorFrameProc @ 0x56a66 (71 bytes)

void TrapDoorFrameProc(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  if (*param_3 == param_1->maxFrames + -1) {
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
    _SWSetSpriteFrameTime(param_1,-1);
  }
  return;
}


// ---- _SkullFallingPlatformFrameProc @ 0x56aad (80 bytes)

void SkullFallingPlatformFrameProc(SpritePtr param_1,undefined4 param_2,int *param_3)

{
  if ((*param_3 == 0) || (*param_3 == param_1->maxFrames + -1)) {
    _SWSetSpriteFrameTime(param_1,-1);
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
  }
  return;
}


