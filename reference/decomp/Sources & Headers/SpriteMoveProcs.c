// Ghidra decompilation of functions from original source file:
//   Sources & Headers/SpriteMoveProcs.c

// ---- _MainSpriteMoveProc @ 0x5ceb1 (6592 bytes)

void MainSpriteMoveProc(SpritePtr param_1)

{
  byte bVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  short sVar7;
  short sVar8;
  short sVar9;
  short sVar10;
  short sVar11;
  short sVar12;
  short sVar13;
  short sVar14;
  short sVar15;
  short sVar16;
  short sVar17;
  short sVar18;
  short sVar19;
  short sVar20;
  short sVar21;
  int iVar22;
  int iVar23;
  undefined4 uVar24;
  uchar uVar25;
  undefined1 uVar26;
  short sVar28;
  char cVar27;
  undefined2 uVar29;
  int iVar30;
  uint uVar31;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  longdouble extraout_ST0_01;
  longdouble extraout_ST0_02;
  SWMovementType SVar32;
  bool bStack_71;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined1 auStack_5c [8];
  undefined4 uStack_54;
  undefined4 uStack_50;
  SpritePtr pSStack_4c;
  short sStack_48;
  short sStack_46;
  char cStack_43;
  uchar uStack_42;
  char cStack_41;
  float fStack_40;
  float fStack_3c;
  SpritePtr pSStack_38;
  SpritePtr pSStack_34;
  short sStack_30;
  ushort uStack_2e;
  char cStack_2c;
  char cStack_2b;
  short sStack_2a;
  float fStack_28;
  short sStack_24;
  short sStack_22;
  SWMovementType SStack_20;
  short sStack_1a;
  short sStack_18;
  undefined1 uStack_16;
  bool bStack_15;
  float fStack_14;
  float fStack_10;
  
  pSStack_4c = param_1;
  if ((PTR__gKeys_001c4af8[7] == '\0') &&
     (sVar28._0_1_ = param_1->f_1bc, sVar28._1_1_ = param_1->f_1bd, sVar28 < 5)) {
    *(short *)&param_1->f_1bc = *(short *)&param_1->f_1bc + 1;
  }
  if (((param_1->frameChangeProc == (FrameProcPtr_t)PTR_IdleHeroFrameProc_001c4cfc) &&
      (sVar2._0_1_ = param_1->f_1a2, sVar2._1_1_ = param_1->f_1a3, sVar2 == 5)) &&
     (((PTR__gKeys_001c4af8[2] != '\0' ||
       (((PTR__gKeys_001c4af8[3] != '\0' || (PTR__gKeys_001c4af8[1] != '\0')) ||
        (PTR__gKeys_001c4af8[5] != '\0')))) ||
      ((PTR__gKeys_001c4af8[6] != '\0' || (PTR__gKeys_001c4af8[4] != '\0')))))) {
    _DeleteZzzzSprites();
    _MMPlaySound(0x47,9,1);
  }
  *PTR__gHeroLandedThisFrame_001c4c70 = 0;
  if ((pSStack_4c->f_172 != '\0') && (iVar30 = _SWGetSpriteFrameTime(param_1), iVar30 < 0)) {
    pSStack_4c->f_172 = '\0';
  }
  if (((pSStack_4c->f_171 != '\0') && (PTR__gKeys_001c4af8[2] == '\0')) &&
     (pSStack_4c->f_17e != '\0')) {
    pSStack_4c->f_171 = '\0';
    uVar25 = pSStack_4c->f_14d;
    pSStack_4c->f_102 = pSStack_4c->f_14c;
    pSStack_4c->f_103 = uVar25;
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
    _SWSetSpriteFrameTime(param_1,-1);
    _SWSetSpriteFrameAdvance(param_1,0);
    _SetHeroGroundFrame(param_1);
  }
  sStack_46 = 0;
  pSStack_4c->f_150 = '\0';
  pSStack_4c->f_151 = '\0';
  pSStack_4c->f_152 = '\0';
  pSStack_4c->f_153 = '\0';
  pSStack_4c->f_154 = '\0';
  pSStack_4c->f_155 = '\0';
  pSStack_4c->f_156 = '\0';
  pSStack_4c->f_157 = '\0';
  iVar30._0_1_ = pSStack_4c->f_1c4;
  iVar30._1_1_ = pSStack_4c->f_1c5;
  iVar30._2_1_ = pSStack_4c->f_1c6;
  iVar30._3_1_ = pSStack_4c->f_1c7;
  if (iVar30 == 0) {
    if (param_1->parentSpriteLayerP != *(SpriteLayerRec **)PTR__gHeroSpriteLayerP_001c4164) {
      _SWRemoveSprite(param_1);
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164,param_1);
    }
  }
  else {
    pSStack_38 = *(SpritePtr *)&pSStack_4c->f_1c4;
    if ((0.0 <= param_1->drawData->vertMoveDelta) &&
       (param_1->parentSpriteLayerP != *(SpriteLayerRec **)PTR__gHeroOnPlatformSpriteLayerP_001c491c
       )) {
      _SWRemoveSprite(param_1);
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gHeroOnPlatformSpriteLayerP_001c491c,param_1);
    }
    if (pSStack_38->isVisible == '\0') {
      pSStack_34 = pSStack_38;
      fStack_3c = *(float *)&pSStack_38->f_110;
      fStack_40 = *(float *)&pSStack_38->f_114;
    }
    else {
      fStack_3c = (float)((int)(pSStack_38->destFrameRect).left -
                         (int)(pSStack_38->oldFrameRect).left);
      fStack_40 = (float)((int)(pSStack_38->destFrameRect).top - (int)(pSStack_38->oldFrameRect).top
                         );
    }
    if ((fStack_3c != 0.0) || (NAN(fStack_3c))) {
      SVar32 = _SWGetSpriteHorizMoveDelta(pSStack_38);
      *(SWMovementType *)&pSStack_4c->f_150 = SVar32;
    }
    if (fStack_40 < 0.0) {
      fStack_40 = 0.0;
    }
    _SWOffsetSprite(param_1,fStack_3c,fStack_40);
    if (0.0 < fStack_3c) {
      _CheckForWall(param_1,2,&pSStack_4c->f_142,pSStack_4c->f_170,0,1);
    }
    else if (fStack_3c < 0.0) {
      _CheckForWall(param_1,4,&pSStack_4c->f_142,pSStack_4c->f_170,0,1);
    }
  }
  uStack_42 = pSStack_4c->f_17e;
  if (uStack_42 != '\0') {
    _SaveLastOnGroundPixelRow(param_1);
  }
  _SaveStartingPositionIfAppropriate(param_1,uStack_42);
  if (((((uStack_42 == '\0') || (cVar27 = _IsMoviePlaying(), cVar27 != '\0')) ||
       (*PTR__gUsingEditor_001c48b0 != '\0')) ||
      ((*PTR__gEpisode1IsUnlocked_001c4744 != '\0' ||
       (cVar27 = _IsMansionDemoMansion((int)*(short *)PTR__gCurrentMansionNumber_001c4824),
       cVar27 == '\0')))) || (cVar27 = _CheckForEndOfDemoLimit(), cVar27 == '\0')) {
    if (((PTR__gKeys_001c4af8[7] != '\0') && (uStack_42 != '\0')) &&
       ((sVar3._0_1_ = pSStack_4c->f_1bc, sVar3._1_1_ = pSStack_4c->f_1bd, 1 < sVar3 &&
        ((param_1->destFrameRect).bottom == (param_1->oldFrameRect).bottom)))) {
      uStack_2e = _PullLeverIfPossible(param_1);
      uStack_2e = uStack_2e & 0xff;
      if (uStack_2e != 0) {
        return;
      }
      _SWCollideSpriteWithLayer
                (param_1,*(undefined4 *)PTR__gSignSpriteLayerP_001c41f8,
                 PTR_HeroSignCollideProc_001c4d80);
    }
    sVar4._0_1_ = pSStack_4c->f_19e;
    sVar4._1_1_ = pSStack_4c->f_19f;
    if (0 < sVar4) {
      *(short *)&pSStack_4c->f_19e = *(short *)&pSStack_4c->f_19e + -1;
    }
    sVar5._0_1_ = pSStack_4c->f_1a0;
    sVar5._1_1_ = pSStack_4c->f_1a1;
    if (0 < sVar5) {
      *(short *)&pSStack_4c->f_1a0 = *(short *)&pSStack_4c->f_1a0 + -1;
    }
    if (*PTR__gKeys_001c4af8 == '\0') {
      pSStack_4c->f_19e = '\0';
      pSStack_4c->f_19f = '\0';
    }
    if (PTR__gKeys_001c4af8[2] == '\0') {
      pSStack_4c->f_1a0 = '\0';
      pSStack_4c->f_1a1 = '\0';
      pSStack_4c->f_16f = '\x01';
    }
    if ((PTR__gKeys_001c4af8[3] == '\0') && (PTR__gKeys_001c4af8[1] == '\0')) {
      pSStack_4c->f_16f = '\x01';
    }
    if ((uStack_42 != '\0') && ((*PTR__gKeys_001c4af8 != '\0' || (PTR__gKeys_001c4af8[7] != '\0'))))
    {
      _SWCollideSpriteWithLayer
                (param_1,*(undefined4 *)PTR__gExitSpriteLayerP_001c403c,
                 PTR_HeroExitCollideProc_001c4dc0);
    }
    if ((uStack_42 != '\0') &&
       ((((*PTR__gKeys_001c4af8 != '\0' && (PTR__gKeys_001c4af8[2] == '\0')) &&
         (sVar6._0_1_ = pSStack_4c->f_19e, sVar6._1_1_ = pSStack_4c->f_19f, sVar6 == 0)) ||
        (((PTR__gKeys_001c4af8[2] != '\0' || (PTR__gGamepad_001c4ac8[2] != '\0')) &&
         ((*PTR__gKeys_001c4af8 == '\0' &&
          (sVar7._0_1_ = pSStack_4c->f_1a0, sVar7._1_1_ = pSStack_4c->f_1a1, sVar7 == 0)))))))) {
      if (*PTR__gKeys_001c4af8 == '\0') {
        uStack_60._0_1_ = pSStack_4c->f_11e;
        uStack_60._1_1_ = pSStack_4c->f_11f;
        uStack_60._2_1_ = pSStack_4c->f_120;
        uStack_60._3_1_ = pSStack_4c->f_121;
        uStack_64._0_1_ = pSStack_4c->f_11a;
        uStack_64._1_1_ = pSStack_4c->f_11b;
        uStack_64._2_1_ = pSStack_4c->f_11c;
        uStack_64._3_1_ = pSStack_4c->f_11d;
      }
      else {
        uStack_60._0_1_ = pSStack_4c->f_116;
        uStack_60._1_1_ = pSStack_4c->f_117;
        uStack_60._2_1_ = pSStack_4c->f_118;
        uStack_60._3_1_ = pSStack_4c->f_119;
        uStack_64._0_1_ = pSStack_4c->f_112;
        uStack_64._1_1_ = pSStack_4c->f_113;
        uStack_64._2_1_ = pSStack_4c->f_114;
        uStack_64._3_1_ = pSStack_4c->f_115;
      }
      cStack_43 = _GetLadderRect(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1,&uStack_64,
                                 auStack_5c,5,0x20b,0x240);
      if (cStack_43 == '\0') {
        if (((PTR__gKeys_001c4af8[2] != '\0') && (pSStack_4c->f_16f != '\0')) &&
           ((param_1->frameChangeProc == (FrameProcPtr_t)0x0 ||
            ((param_1->frameChangeProc == (FrameProcPtr_t)PTR_LAB_001c4d70 ||
             (param_1->frameChangeProc == (FrameProcPtr_t)PTR_HeroMiscFrameProc_001c41d0)))))) {
          _SWSetSpriteFrameRange(param_1,0xf,0xf);
          _SWSetSpriteFlipX(param_1,pSStack_4c->f_177 == '\0');
          _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_PushupDelayFrameProc_001c4c8c);
          _SWSetSpriteFrameTime(param_1,0x10);
          uVar29 = _GetRandom(0xb4,0x1a4);
          pSStack_4c->f_188 = (char)uVar29;
          pSStack_4c->f_189 = (char)((ushort)uVar29 >> 8);
          pSStack_4c->f_171 = '\x01';
          pSStack_4c->f_172 = '\0';
          *(short *)&pSStack_4c->f_102 = *(short *)&pSStack_4c->f_14c + 0x10;
          _SWSetSpriteMoveDelta(param_1,0.0,0.0);
        }
      }
      else {
        uStack_50._0_2_ = (param_1->destFrameRect).bottom;
        uStack_50._2_2_ = (param_1->destFrameRect).right;
        uStack_54._0_2_ = (param_1->destFrameRect).top;
        uStack_54._2_2_ = (param_1->destFrameRect).left;
        _SWCenterRect(&uStack_54,auStack_5c);
        if ((param_1->destFrameRect).left + 2 < (int)uStack_54._2_2_) {
          if (-1 < (int)((uint)(byte)PTR__gKeys_001c4af8[1] - (uint)(byte)PTR__gKeys_001c4af8[3])) {
            sStack_46 = 1;
          }
        }
        else {
          if ((param_1->destFrameRect).left + -2 <= (int)uStack_54._2_2_) {
            pSStack_4c->f_1c4 = '\0';
            pSStack_4c->f_1c5 = '\0';
            pSStack_4c->f_1c6 = '\0';
            pSStack_4c->f_1c7 = '\0';
            pSStack_4c->f_15c = '\0';
            pSStack_4c->f_15d = '\0';
            pSStack_4c->f_15e = '\0';
            pSStack_4c->f_15f = '\0';
            pSStack_4c->f_160 = '\0';
            pSStack_4c->f_161 = '\0';
            pSStack_4c->f_162 = '\0';
            pSStack_4c->f_163 = '\0';
            pSStack_4c->f_150 = '\0';
            pSStack_4c->f_151 = '\0';
            pSStack_4c->f_152 = '\0';
            pSStack_4c->f_153 = '\0';
            pSStack_4c->f_154 = '\0';
            pSStack_4c->f_155 = '\0';
            pSStack_4c->f_156 = '\0';
            pSStack_4c->f_157 = '\0';
            pSStack_4c->f_158 = '\0';
            pSStack_4c->f_159 = '\0';
            pSStack_4c->f_15a = '\0';
            pSStack_4c->f_15b = '\0';
            _StopChannel(4);
            if (*PTR__gKeys_001c4af8 != '\0') {
              _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR__ClimbingSpriteMoveProc_001c4d34);
              _SWSetSpriteFrameRange(param_1,0x12,0x19);
              _SWSetSpriteFlipX(param_1,'\0');
              _SWSetSpriteFrameAdvanceMode(param_1,(void *)0x0);
              _SWSetSpriteFrameAdvance(param_1,1);
              _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
              pSStack_4c->f_17e = '\0';
              pSStack_4c->f_179 = '\0';
              pSStack_4c->f_172 = '\0';
              _ClimbingSpriteMoveProc(param_1);
              return;
            }
            if (PTR__gGamepad_001c4ac8[2] == '\0') {
              *(ushort *)&pSStack_4c->f_16a =
                   (ushort)(byte)PTR__gKeys_001c4af8[2] - (ushort)(byte)*PTR__gKeys_001c4af8;
            }
            else {
              bVar1 = PTR__gGamepad_001c4ac8[2];
              pSStack_4c->f_16a = (char)(ushort)bVar1;
              pSStack_4c->f_16b = (char)((ushort)bVar1 >> 8);
            }
            pSStack_4c->f_17e = '\0';
            pSStack_4c->f_172 = '\0';
            pSStack_4c->f_18e = '\x04';
            pSStack_4c->f_18f = '\0';
            _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR__FinishClimbingLadderMoveProc_001c4da4);
            _SWSetSpriteFrameAdvance(param_1,1);
            _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
            _FinishClimbingLadderMoveProc(param_1);
            return;
          }
          if ((uint)(byte)PTR__gKeys_001c4af8[1] == (uint)(byte)PTR__gKeys_001c4af8[3] ||
              (int)((uint)(byte)PTR__gKeys_001c4af8[1] - (uint)(byte)PTR__gKeys_001c4af8[3]) < 0) {
            sStack_46 = -1;
          }
        }
      }
    }
    if ((uStack_42 == '\0') || (PTR__gKeys_001c4af8[2] != '\0')) {
      cStack_2c = _GetOnVineIfAppropriate(param_1,uStack_42,4,1);
      if (cStack_2c == '\0') {
        cStack_2c = _GetOnVineIfAppropriate(param_1,uStack_42,5,1);
      }
      if (cStack_2c != '\0') {
        return;
      }
    }
    if ((uStack_42 != '\0') || (cStack_2b = _GetOnPoleIfAppropriate(param_1), cStack_2b == '\0')) {
      if ((uStack_42 != '\0') && ((pSStack_4c->f_170 == '\0' && (pSStack_4c->f_171 == '\0')))) {
        if ((sStack_46 == 0) || (sStack_46 == 99)) {
          if (sStack_46 == 99) {
            sStack_48 = 0;
          }
          else {
            sStack_48 = (ushort)(byte)PTR__gKeys_001c4af8[1] - (ushort)(byte)PTR__gKeys_001c4af8[3];
            if (PTR__gKeys_001c4af8[5] == '\0') {
              if (PTR__gKeys_001c4af8[6] != '\0') {
                sStack_48 = 1;
              }
            }
            else {
              sStack_48 = -1;
            }
          }
        }
        else {
          sStack_48 = sStack_46;
        }
        param_1->drawData->horizMoveDelta = (float)(int)sStack_48 * 2.5;
        if (sStack_48 != 0) {
          if (sStack_48 < 1) {
            pSStack_4c->f_177 = '\0';
          }
          else {
            pSStack_4c->f_177 = '\x01';
          }
          pSStack_4c->f_178 = pSStack_4c->f_177;
        }
        pSStack_4c->f_17e = '\x01';
        _SetHeroGroundFrame(param_1);
        SVar32 = _SWGetSpriteHorizMoveDelta(param_1);
        if (((SVar32 != 0.0) || (NAN(SVar32))) &&
           (sVar8._0_1_ = pSStack_4c->f_168, sVar8._1_1_ = pSStack_4c->f_169,
           param_1->curFrameIndex != (int)sVar8)) {
          sStack_2a = _GetRandom(4,6);
          if (param_1->curFrameIndex == 3) {
            uVar29 = (undefined2)param_1->curFrameIndex;
            pSStack_4c->f_168 = (char)uVar29;
            pSStack_4c->f_169 = (char)((ushort)uVar29 >> 8);
            _MMPlaySound((int)sStack_2a,9,1);
          }
          else if ((param_1->curFrameIndex == 7) && (*PTR__gSoundIsTurnedOn_001c47fc != '\0')) {
            uVar29 = (undefined2)param_1->curFrameIndex;
            pSStack_4c->f_168 = (char)uVar29;
            pSStack_4c->f_169 = (char)((ushort)uVar29 >> 8);
            _MMPlayStereoSound2(0,(int)sStack_2a,9,1,(int)*(short *)PTR__gSoundVolume_001c48dc,
                                0x3ff0000000000000,0);
          }
          uVar29 = (undefined2)param_1->curFrameIndex;
          pSStack_4c->f_168 = (char)uVar29;
          pSStack_4c->f_169 = (char)((ushort)uVar29 >> 8);
        }
      }
      if (((*PTR__gCanControlWhileJumping_001c482c != '\0') &&
          ((uStack_42 == '\0' || (pSStack_4c->f_170 != '\0')))) && (pSStack_4c->f_171 == '\0')) {
        fStack_28 = *(float *)&pSStack_4c->f_15c;
        sStack_48 = (ushort)(byte)PTR__gKeys_001c4af8[1] - (ushort)(byte)PTR__gKeys_001c4af8[3];
        if ((sStack_48 != 0) || (pSStack_4c->f_170 == '\0')) {
          param_1->drawData->horizMoveDelta = fStack_28 + (float)(int)sStack_48 * 2.5;
        }
        if (fStack_28 < param_1->drawData->horizMoveDelta) {
          pSStack_4c->f_177 = '\x01';
        }
        else if (param_1->drawData->horizMoveDelta < fStack_28) {
          pSStack_4c->f_177 = '\0';
        }
        else {
          pSStack_4c->f_177 = pSStack_4c->f_178;
        }
        pSStack_4c->f_178 = pSStack_4c->f_177;
        _SetHeroJumpingFrame(param_1);
      }
      if ((uStack_42 == '\0') && (pSStack_4c->f_170 == '\0')) {
        if (pSStack_4c->f_180 == '\0') {
          sVar9._0_1_ = pSStack_4c->f_184;
          sVar9._1_1_ = pSStack_4c->f_185;
          if ((int)sVar9 - (int)(param_1->oldFrameRect).top < 0) {
            sVar10._0_1_ = pSStack_4c->f_184;
            sVar10._1_1_ = pSStack_4c->f_185;
            iVar30 = (int)(param_1->oldFrameRect).top - (int)sVar10;
          }
          else {
            sVar11._0_1_ = pSStack_4c->f_184;
            sVar11._1_1_ = pSStack_4c->f_185;
            iVar30 = (int)sVar11 - (int)(param_1->oldFrameRect).top;
          }
          if (0x10 < iVar30) {
            _SWSetSpriteFrameRange(param_1,0xd,0xd);
            _SWSetSpriteFlipX(param_1,pSStack_4c->f_177 == '\0');
            _SWSetSpriteFrameTime(param_1,-1);
          }
        }
        else {
          uVar26 = *(undefined1 *)((int)&(param_1->oldFrameRect).top + 1);
          pSStack_4c->f_184 = *(undefined1 *)&(param_1->oldFrameRect).top;
          pSStack_4c->f_185 = uVar26;
        }
      }
      cStack_41 = _ProcessJumpIntention(param_1,uStack_42);
      if (cStack_41 != '\0') {
        _StartJump(param_1);
      }
      if (pSStack_4c->f_170 != '\0') {
        sStack_22 = ((param_1->destFrameRect).bottom - *(short *)&pSStack_4c->f_146) + -1;
        sVar12._0_1_ = pSStack_4c->f_182;
        sVar12._1_1_ = pSStack_4c->f_183;
        if (sVar12 < sStack_22) {
          uVar25 = pSStack_4c->f_14b;
          pSStack_4c->f_10a = pSStack_4c->f_14a;
          pSStack_4c->f_10b = uVar25;
          uVar25 = pSStack_4c->f_14d;
          pSStack_4c->f_102 = pSStack_4c->f_14c;
          pSStack_4c->f_103 = uVar25;
        }
        else {
          if ((((*(float *)&pSStack_4c->f_160 != 0.0) || (NAN(*(float *)&pSStack_4c->f_160))) &&
              (iVar22._0_1_ = pSStack_4c->f_164, iVar22._1_1_ = pSStack_4c->f_165,
              iVar22._2_1_ = pSStack_4c->f_166, iVar22._3_1_ = pSStack_4c->f_167, iVar22 != 0)) &&
             (((SStack_20 = _SWGetSpriteVertMoveDelta(*(SpritePtr *)&pSStack_4c->f_164),
               SStack_20 == 0.0 || ((*(float *)&pSStack_4c->f_160 < 0.0 && (0.0 < SStack_20)))) ||
              ((0.0 < *(float *)&pSStack_4c->f_160 && (SStack_20 < 0.0)))))) {
            pSStack_4c->f_160 = '\0';
            pSStack_4c->f_161 = '\0';
            pSStack_4c->f_162 = '\0';
            pSStack_4c->f_163 = '\0';
          }
          *(short *)&pSStack_4c->f_174 = *(short *)&pSStack_4c->f_174 + 1;
          sVar13._0_1_ = pSStack_4c->f_174;
          sVar13._1_1_ = pSStack_4c->f_175;
          param_1->drawData->vertMoveDelta =
               *(float *)&pSStack_4c->f_160 +
               ((float)(int)sVar13 * *(float *)PTR__gJumpChange_001c4790 -
               *(float *)PTR__gJumpSpeed_001c4708);
          sStack_24 = *(short *)&pSStack_4c->f_182 - sStack_22;
          *(short *)&pSStack_4c->f_10a = *(short *)&pSStack_4c->f_14a + sStack_24;
          *(short *)&pSStack_4c->f_102 = *(short *)&pSStack_4c->f_14c + sStack_24;
          if (0.0 < param_1->drawData->vertMoveDelta) {
            sVar14._0_1_ = pSStack_4c->f_10a;
            sVar14._1_1_ = pSStack_4c->f_10b;
            uVar29 = (undefined2)
                     (int)((float)(int)sVar14 - (param_1->drawData->vertMoveDelta + 6.0));
            pSStack_4c->f_10a = (char)uVar29;
            pSStack_4c->f_10b = (char)((ushort)uVar29 >> 8);
            sVar15._0_1_ = pSStack_4c->f_102;
            sVar15._1_1_ = pSStack_4c->f_103;
            uVar29 = (undefined2)
                     (int)((float)(int)sVar15 - (param_1->drawData->vertMoveDelta + 6.0));
            pSStack_4c->f_102 = (char)uVar29;
            pSStack_4c->f_103 = (char)((ushort)uVar29 >> 8);
          }
          sVar16._0_1_ = pSStack_4c->f_10a;
          sVar16._1_1_ = pSStack_4c->f_10b;
          _SWGetSpriteHeight(param_1);
          if ((float)extraout_ST0 <= (float)(int)sVar16) {
            _SWGetSpriteHeight(param_1);
            uVar29 = (undefined2)(int)((float)extraout_ST0_00 - 1.0);
            pSStack_4c->f_10a = (char)uVar29;
            pSStack_4c->f_10b = (char)((ushort)uVar29 >> 8);
          }
          sVar17._0_1_ = pSStack_4c->f_102;
          sVar17._1_1_ = pSStack_4c->f_103;
          _SWGetSpriteHeight(param_1);
          if ((float)extraout_ST0_01 <= (float)(int)sVar17) {
            _SWGetSpriteHeight(param_1);
            uVar29 = (undefined2)(int)((float)extraout_ST0_02 - 1.0);
            pSStack_4c->f_102 = (char)uVar29;
            pSStack_4c->f_103 = (char)((ushort)uVar29 >> 8);
          }
        }
      }
      if (uStack_42 == '\0') {
        SVar32 = _SWGetSpriteVertLoc(param_1);
        sVar18._0_1_ = pSStack_4c->f_186;
        sVar18._1_1_ = pSStack_4c->f_187;
        sStack_1a = (short)(int)(SVar32 - (float)(int)sVar18);
        iVar30 = sStack_1a + 8;
        if (iVar30 < 0) {
          iVar30 = sStack_1a + 0x17;
        }
        sStack_18 = (short)(iVar30 >> 4);
        sVar28 = _getHeroFallingDeathDistance(param_1);
        if ((sVar28 <= sStack_18) && (pSStack_4c->f_17f == '\0')) {
          _MMPlaySound(0x28,9,1);
          pSStack_4c->f_17f = '\x01';
        }
      }
      if (((uStack_42 == '\0') && (pSStack_4c->f_170 == '\0')) && (pSStack_4c->f_180 == '\x01')) {
        param_1->drawData->horizMoveDelta = 0.0;
      }
      param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 1.3;
      if (12.0 < param_1->drawData->vertMoveDelta) {
        param_1->drawData->vertMoveDelta = 12.0;
      }
      *PTR__gHeroIsOnConveyorBelt_001c49ec = 0;
      pSStack_4c->f_1c0 = '\0';
      pSStack_4c->f_1c1 = '\0';
      pSStack_4c->f_1c2 = '\0';
      pSStack_4c->f_1c3 = '\0';
      pSStack_4c->f_180 = uStack_42;
      uStack_42 = _MoveAndCollideMainSprite(param_1);
      pSStack_4c->f_17e = uStack_42;
      if (((uStack_42 == '\0') ||
          (pSStack_4c->f_15c = '\0', pSStack_4c->f_15d = '\0', pSStack_4c->f_15e = '\0',
          pSStack_4c->f_15f = '\0', *PTR__gHeroIsOnConveyorBelt_001c49ec == '\0')) ||
         (*(short *)PTR__gCurConveyorBeltDirection_001c470c == 0)) {
        _StopChannel(4);
      }
      else {
        sVar28 = _FindSound(0x3f);
        if (sVar28 == -1) {
          _MMPlayStereoSound2(0,0x3f,4,2,
                              (int)(short)(int)((double)(int)*(short *)PTR__gSoundVolume_001c48dc /
                                               1.5),0x3ff0000000000000,0xffffffff);
        }
      }
      if (((pSStack_4c->f_170 == '\0') || (param_1->drawData->vertMoveDelta < 0.0)) ||
         (uStack_42 == '\0')) {
        if (((0.0 <= param_1->drawData->vertMoveDelta) && (uStack_42 != '\0')) &&
           (pSStack_4c->f_180 == '\0')) {
          sVar19._0_1_ = pSStack_4c->f_184;
          sVar19._1_1_ = pSStack_4c->f_185;
          if ((int)sVar19 - (int)(param_1->oldFrameRect).top < 0) {
            sVar20._0_1_ = pSStack_4c->f_184;
            sVar20._1_1_ = pSStack_4c->f_185;
            iVar30 = (int)(param_1->oldFrameRect).top - (int)sVar20;
          }
          else {
            sVar21._0_1_ = pSStack_4c->f_184;
            sVar21._1_1_ = pSStack_4c->f_185;
            iVar30 = (int)sVar21 - (int)(param_1->oldFrameRect).top;
          }
          bStack_71 = 0x10 < iVar30;
          bStack_15 = bStack_71;
          _HandleHeroLanding(param_1,bStack_71,0);
        }
      }
      else {
        uStack_16 = _EndJump(param_1);
        sStack_48 = (ushort)(byte)PTR__gKeys_001c4af8[1] - (ushort)(byte)PTR__gKeys_001c4af8[3];
        param_1->drawData->horizMoveDelta = (float)(int)sStack_48 * 2.5;
        _SWSetSpriteFrameAdvance(param_1,1);
        *PTR__gHeroLandedThisFrame_001c4c70 = 1;
        _HandleHeroLanding(param_1,1,uStack_16);
      }
      if ((0.0 < param_1->drawData->vertMoveDelta) &&
         (*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x80) + 100 <=
          (int)(param_1->destFrameRect).top)) {
        _KillHero(param_1);
      }
      iVar23._0_1_ = pSStack_4c->f_1c0;
      iVar23._1_1_ = pSStack_4c->f_1c1;
      iVar23._2_1_ = pSStack_4c->f_1c2;
      iVar23._3_1_ = pSStack_4c->f_1c3;
      if ((iVar23 != 0) && (pSStack_4c->f_17a == '\0')) {
        uVar24._0_1_ = pSStack_4c->f_1c0;
        uVar24._1_1_ = pSStack_4c->f_1c1;
        uVar24._2_1_ = pSStack_4c->f_1c2;
        uVar24._3_1_ = pSStack_4c->f_1c3;
        _SaveHeroOnPedestal(param_1,uVar24);
      }
      if (((uStack_42 == '\0') ||
          ((uint)(byte)PTR__gKeys_001c4af8[3] + (uint)(byte)PTR__gKeys_001c4af8[1] != 0)) ||
         (PTR__gKeys_001c4af8[2] != '\0')) {
        uVar29 = _GetRandom(0xa0,300);
        pSStack_4c->f_1a4 = (char)uVar29;
        pSStack_4c->f_1a5 = (char)((ushort)uVar29 >> 8);
        if (param_1->frameChangeProc == (FrameProcPtr_t)PTR_IdleHeroFrameProc_001c4cfc) {
          _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
        }
      }
      else if ((param_1->frameChangeProc != (FrameProcPtr_t)PTR_IdleHeroFrameProc_001c4cfc) &&
              (sVar28 = *(short *)&pSStack_4c->f_1a4, *(short *)&pSStack_4c->f_1a4 = sVar28 + -1,
              sVar28 < 1)) {
        _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_IdleHeroFrameProc_001c4cfc);
        pSStack_4c->f_1a2 = '\0';
        pSStack_4c->f_1a3 = '\0';
      }
      _ExitRoomIfAppropriate(param_1);
      fStack_10 = 5.0;
      SVar32 = _SWGetSpriteHorizLoc(param_1);
      fStack_14 = (320.0 - SVar32) / 20.0;
      _SWSetGlobalMountOffset(*(SpriteWorldRec **)PTR__gSpriteWorldP_001c40d0,fStack_14,fStack_10);
      _UpdateHaloSpritePosition(param_1);
    }
  }
  else {
    _StopAllChannels();
    _MyFadeMusicNow();
    sVar28 = _GetNumRoomsExplored();
    uVar31 = _MMCountResources("TMAP");
    sStack_30 = (short)((sVar28 * 100) / (int)(uVar31 & 0xff));
    _DoRegisterNowDialog((int)sStack_30);
    *PTR__gEndGame_001c4798 = 1;
  }
  return;
}


// ---- _DeadHeroMoveProc @ 0x5e871 (1351 bytes)

void _DeadHeroMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  short sVar7;
  undefined4 uVar8;
  int iVar9;
  char cVar12;
  SWBoolean SVar13;
  short sVar14;
  undefined4 local_20;
  short local_1c;
  undefined2 uStack_1a;
  int local_18;
  SpritePtr local_14;
  uchar local_d;
  undefined1 uVar10;
  undefined1 uVar11;
  
  local_14 = param_1;
  uVar8._0_1_ = param_1->f_146;
  uVar8._1_1_ = param_1->f_147;
  uVar10 = param_1->f_148;
  uVar11 = param_1->f_149;
  uVar8._2_2_ = CONCAT11(uVar11,uVar10);
  local_20._0_1_ = param_1->f_142;
  local_20._1_1_ = param_1->f_143;
  local_20._2_1_ = param_1->f_144;
  local_20._3_1_ = param_1->f_145;
  local_1c = (short)uVar8;
  _local_1c = CONCAT22(uVar8._2_2_,local_1c + -1);
  cVar12 = _SWCheckSpriteWithSprites
                     (param_1,&local_18,3,&local_20,
                      *(undefined4 *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,
                      *(undefined4 *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c,1,0,0,0);
  if (cVar12 != '\0') {
    _SWOffsetSprite(param_1,(float)((int)*(short *)(local_18 + 0x38) -
                                   (int)*(short *)(local_18 + 0x40)),
                    (float)((int)*(short *)(local_18 + 0x36) - (int)*(short *)(local_18 + 0x3e)));
  }
  local_d = '\0';
  cVar12 = _CheckForWall(param_1,3,&local_14->f_142,0,1,0);
  if (cVar12 != '\0') {
    local_d = '\x01';
  }
  if (local_d != '\0') {
    param_1->drawData->horizMoveDelta = 0.0;
  }
  if ((local_14->f_170 != '\0') &&
     (sVar14._0_1_ = local_14->f_146, sVar14._1_1_ = local_14->f_147, sVar1._0_1_ = local_14->f_182,
     sVar1._1_1_ = local_14->f_183,
     ((int)(param_1->destFrameRect).bottom - (int)sVar14) + -1 <= (int)sVar1)) {
    *(short *)&local_14->f_174 = *(short *)&local_14->f_174 + 1;
    sVar2._0_1_ = local_14->f_174;
    sVar2._1_1_ = local_14->f_175;
    param_1->drawData->vertMoveDelta =
         (float)(int)sVar2 * *(float *)PTR__gJumpChange_001c4790 -
         *(float *)PTR__gJumpSpeed_001c4708;
  }
  param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 1.0;
  if (9.0 < param_1->drawData->vertMoveDelta) {
    param_1->drawData->vertMoveDelta = 9.0;
  }
  local_14->f_180 = local_d;
  local_d = _MoveAndCollideMainSprite(param_1);
  local_14->f_17e = local_d;
  if (((local_14->f_170 == '\0') || (param_1->drawData->vertMoveDelta < 0.0)) || (local_d == '\0'))
  {
    sVar3._0_1_ = local_14->f_16c;
    sVar3._1_1_ = local_14->f_16d;
    if ((((sVar3 == 1) && (0.0 <= param_1->drawData->vertMoveDelta)) &&
        ((local_d != '\0' && ((local_14->f_180 == '\0' && (local_14->f_17d != '\0')))))) &&
       (sVar14 = _FindSound(8), sVar14 == -1)) {
      _MMPlayStereoSound(param_1,0x17,9,1);
    }
  }
  else {
    local_14->f_170 = '\0';
    local_14->f_16e = '\x01';
  }
  sVar4._0_1_ = local_14->f_16c;
  sVar4._1_1_ = local_14->f_16d;
  if ((sVar4 == 1) && (local_14->f_17b == '\0')) {
    if (local_d == '\0') {
      sVar5._0_1_ = local_14->f_192;
      sVar5._1_1_ = local_14->f_193;
      if (0 < sVar5) {
        *(short *)&local_14->f_192 = *(short *)&local_14->f_192 + -1;
      }
    }
    else {
      local_14->f_17b = '\x01';
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_SkeletonCollapseFrameProc1_001c4d90);
      _SWSetSpriteFrameAdvance(param_1,0);
      local_14->f_16c = '\x06';
      local_14->f_16d = '\0';
    }
  }
  sVar6._0_1_ = local_14->f_16c;
  sVar6._1_1_ = local_14->f_16d;
  if (((sVar6 == 2) && (local_14->f_17c == '\0')) &&
     ((local_d != '\0' ||
      (SVar13 = _SWCheckSpriteWithTiles
                          (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,(void *)0x0,
                           (SWRect *)0x0,6,6,0x309,0x30c,'\0'), SVar13 != '\0')))) {
    local_14->f_17c = '\x01';
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_LAB_001c4dec);
    _SWSetSpriteFrameAdvance(param_1,0);
    local_14->f_16c = '\x06';
    local_14->f_16d = '\0';
    local_14->f_192 = '\f';
    local_14->f_193 = '\0';
    _MMPlaySound(0x36,9,1);
  }
  if ((0.0 < param_1->drawData->vertMoveDelta) &&
     (*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x80) <= (param_1->destFrameRect).top)) {
    sVar7._0_1_ = local_14->f_16c;
    sVar7._1_1_ = local_14->f_16d;
    if ((sVar7 == 4) &&
       (iVar9._0_1_ = local_14->f_1d0, iVar9._1_1_ = local_14->f_1d1, iVar9._2_1_ = local_14->f_1d2,
       iVar9._3_1_ = local_14->f_1d3, iVar9 != 0)) {
      _SWRemoveSpriteFromAnimation(*(SpritePtr *)&local_14->f_1d0,'\x01');
    }
    _SWRemoveSpriteFromAnimation(param_1,'\x01');
    if (local_14->f_17d == '\0') {
      *PTR__gRestartLevel_001c4858 = 1;
    }
  }
  return;
}


// ---- _LyingSkeletonMoveProc @ 0x5edb8 (177 bytes)

void LyingSkeletonMoveProc(SpritePtr param_1)

{
  int iVar1;
  SWMovementType vertLoc;
  SWMovementType horizLoc;
  
  iVar1._0_1_ = param_1->f_1cc;
  iVar1._1_1_ = param_1->f_1cd;
  iVar1._2_1_ = param_1->f_1ce;
  iVar1._3_1_ = param_1->f_1cf;
  if (iVar1 != 0) {
    if (*(int *)(*(int *)&param_1->f_1cc + 0x1d0) == 0) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
                 PTR____FUNCTION___28231_001c4de0,0x3a3);
    }
    vertLoc = _SWGetSpriteVertLoc(*(SpritePtr *)&param_1->f_1cc);
    horizLoc = _SWGetSpriteHorizLoc(*(SpritePtr *)&param_1->f_1cc);
    _SWMoveSprite(param_1,horizLoc,vertLoc);
  }
  return;
}


// ---- _MoveWhenNotTouchingZapperBeam @ 0x5ee69 (81 bytes)

void MoveWhenNotTouchingZapperBeam(undefined4 param_1)

{
  char cVar1;
  
  cVar1 = _IsSpriteCollidingWithLayer
                    (param_1,*(undefined4 *)PTR__gZapperBeamSpriteLayerP_001c41ec,1,1);
  if (cVar1 == '\0') {
    _DeadHeroMoveProc(param_1);
  }
  return;
}


// ---- _HeroSinksIntoLavaMoveProc @ 0x5eeba (528 bytes)

void HeroSinksIntoLavaMoveProc(SpritePtr param_1)

{
  short sVar1;
  SWMovementType SVar2;
  short sStack_e;
  
  if (param_1->userData != 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___28257_001c4df0,0x3c4);
  }
  sVar1._0_1_ = param_1->f_1b4;
  sVar1._1_1_ = param_1->f_1b5;
  SVar2 = _SWGetSpriteVertLoc(param_1);
  sVar1 = (short)(int)((float)(int)sVar1 - (SVar2 + 6.0));
  if (sVar1 < 1) {
    _KillHero(param_1);
  }
  else {
    if (0.12 < param_1->drawData->vertMoveDelta) {
      param_1->drawData->vertMoveDelta =
           param_1->drawData->vertMoveDelta / *(float *)&param_1->f_1b8;
    }
    *(float *)&param_1->f_1b8 = *(float *)&param_1->f_1b8 - 0.004;
    if (*(float *)&param_1->f_1b8 < 1.01) {
      param_1->f_1b8 = 0xae;
      param_1->f_1b9 = 'G';
      param_1->f_1ba = 0x81;
      param_1->f_1bb = '?';
    }
    if (param_1->drawData->vertMoveDelta < 0.12) {
      param_1->drawData->vertMoveDelta = 0.12;
    }
    _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
    if (sVar1 < 0x15) {
      sStack_e = 3;
    }
    else if (sVar1 < 0x1d) {
      sStack_e = 2;
    }
    else if (sVar1 < 0x25) {
      sStack_e = 1;
    }
    else {
      sStack_e = 0;
    }
    _SWSetCurrentFrameIndex(param_1,sStack_e + 0x33);
  }
  return;
}


// ---- _SpriteSinksIntoLavaMoveProc @ 0x5f0ca (450 bytes)

void SpriteSinksIntoLavaMoveProc(SpritePtr param_1)

{
  int iVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sStack_1a;
  short sStack_18;
  
  sVar2 = (param_1->destFrameRect).bottom;
  if (sVar2 < 0) {
    sVar2 = sVar2 + 0xf;
  }
  sVar3 = (param_1->destFrameRect).right;
  if (sVar3 < 0) {
    sVar3 = sVar3 + 0xf;
  }
  sVar4 = (param_1->destFrameRect).top;
  if (sVar4 < 0) {
    sVar4 = sVar4 + 0xf;
  }
  for (sStack_18 = (sVar4 >> 4) + -1; sStack_18 <= sVar2 >> 4; sStack_18 = sStack_18 + 1) {
    sVar4 = (param_1->destFrameRect).left;
    if (sVar4 < 0) {
      sVar4 = sVar4 + 0xf;
    }
    for (sStack_1a = (sVar4 >> 4) + -1; sStack_1a <= sVar3 >> 4; sStack_1a = sStack_1a + 1) {
      sVar4 = _MyGetTile(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,6,(int)sStack_18,(int)sStack_1a)
      ;
      if ((0x308 < sVar4) && (sVar4 < 0x331)) goto LAB_0005f1a1;
    }
  }
LAB_0005f1a1:
  if ((int)(param_1->destFrameRect).top < sStack_18 * 0x10) {
    param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 0.004;
    _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
  }
  else {
    if ((param_1->userData == 0) &&
       (iVar1._0_1_ = param_1->f_1d0, iVar1._1_1_ = param_1->f_1d1, iVar1._2_1_ = param_1->f_1d2,
       iVar1._3_1_ = param_1->f_1d3, iVar1 != 0)) {
      _SWRemoveSpriteFromAnimation(*(SpritePtr *)&param_1->f_1d0,'\x01');
    }
    if ((param_1->userData < 0x50) || (0x5b < param_1->userData)) {
      _SWRemoveSpriteFromAnimation(param_1,'\x01');
    }
    else {
      _MakeFallingPlatformReappearAfterDelay(param_1);
    }
  }
  return;
}


// ---- _MoveAndCollideMainSprite @ 0x5f28c (1142 bytes)

char _MoveAndCollideMainSprite(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  SWBoolean SVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  SWMovementType SVar8;
  undefined4 local_1c;
  SpritePtr local_18;
  float local_14;
  char local_d;
  
  local_d = '\0';
  if (*(int *)PTR__gSpriteWorldP_001c40d0 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___28371_001c4d7c,0x44c);
  }
  if (param_1 == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___28371_001c4d7c,0x44d);
  }
  local_18 = param_1;
  _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
  if ((local_18->f_170 == '\0') &&
     (iVar5._0_1_ = local_18->f_1c4, iVar5._1_1_ = local_18->f_1c5, iVar5._2_1_ = local_18->f_1c6,
     iVar5._3_1_ = local_18->f_1c7, iVar5 == 0)) {
    local_14 = *(float *)PTR__gCurrentWindDelta_001c4898;
  }
  else {
    local_14 = 0.0;
  }
  if (0.0 < param_1->drawData->vertMoveDelta) {
    sVar1._0_1_ = local_18->f_146;
    sVar1._1_1_ = local_18->f_147;
    iVar6 = (int)(param_1->oldFrameRect).bottom - (int)sVar1;
    iVar5 = iVar6 + -1;
    if (iVar5 < 0) {
      iVar5 = iVar6 + 0xe;
    }
    sVar2._0_1_ = local_18->f_146;
    sVar2._1_1_ = local_18->f_147;
    iVar7 = (int)(param_1->destFrameRect).bottom - (int)sVar2;
    iVar6 = iVar7 + -1;
    if (iVar6 < 0) {
      iVar6 = iVar7 + 0xe;
    }
    if (iVar5 >> 4 < iVar6 >> 4) {
      SVar3 = _SWCheckSpriteWithTiles
                        (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,(void *)0x3,
                         (SWRect *)&local_18->f_102,4,4,0x241,0x243,'\0');
      if (SVar3 != '\0') {
        local_14 = local_14 - (float)(int)*(short *)PTR__gCurConveyorBeltDirection_001c470c;
      }
      SVar3 = _SWCheckSpriteWithTiles
                        (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,(void *)0x3,
                         (SWRect *)&local_18->f_102,4,4,0x249,0x24b,'\0');
      if (SVar3 != '\0') {
        local_14 = local_14 + (float)(int)*(short *)PTR__gCurConveyorBeltDirection_001c470c;
      }
    }
    cVar4 = _CheckForWall(param_1,3,&local_18->f_142,0,0,1);
    if (cVar4 != '\0') {
      param_1->drawData->vertMoveDelta = 0.0;
      param_1->drawData->vertMoveDelta = 0.0;
      local_d = '\x01';
      _RoundOffSpriteVertLoc(param_1);
    }
  }
  SVar8 = _SWGetSpriteHorizMoveDelta(param_1);
  _SWOffsetSprite(param_1,SVar8 + local_14,0.0);
  if (local_d != '\0') {
    *(float *)&local_18->f_158 = local_14;
  }
  if (((local_18->f_17a == '\0') && (local_18->f_170 == '\0')) &&
     (cVar4 = _SWCheckSpriteWithSprites
                        (param_1,&local_1c,0,&local_18->f_10a,
                         *(undefined4 *)PTR__gDoorSpriteLayerP_001c41dc,
                         *(undefined4 *)PTR__gDoorSpriteLayerP_001c41dc,1,0,0,0), cVar4 != '\0')) {
    _OpenDoor(local_1c);
  }
  SVar8 = _SWGetSpriteHorizMoveDelta(param_1);
  if (0.0 < SVar8 + local_14) {
    _CheckForWall(param_1,2,&local_18->f_142,local_18->f_170,0,1);
  }
  else {
    SVar8 = _SWGetSpriteHorizMoveDelta(param_1);
    if (SVar8 + local_14 < 0.0) {
      _CheckForWall(param_1,4,&local_18->f_142,local_18->f_170,0,1);
    }
  }
  return local_d;
}


// ---- _ClimbingSpriteMoveProc @ 0x5f702 (3040 bytes)

void _ClimbingSpriteMoveProc(SpritePtr param_1)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  short *psVar7;
  undefined4 uVar8;
  undefined2 uVar9;
  undefined2 uVar10;
  undefined2 uVar11;
  undefined2 uVar12;
  float vertOffset;
  bool bVar13;
  char cVar14;
  uchar uVar15;
  short sVar16;
  short sVar17;
  short sVar18;
  short sVar19;
  short sVar20;
  short sVar21;
  short sVar22;
  undefined2 local_4e;
  short local_40;
  short sStack_3e;
  short local_3c;
  short sStack_3a;
  short local_2a;
  
  if (PTR__gGamepad_001c4ac8[2] != '\0') {
    PTR__gKeys_001c4af8[2] = PTR__gGamepad_001c4ac8[2];
  }
  _SaveStartingPositionIfAppropriate(param_1,0);
  if (param_1->parentSpriteLayerP != *(SpriteLayerRec **)PTR__gHeroSpriteLayerP_001c4164) {
    _SWRemoveSprite(param_1);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164,param_1);
  }
  bVar13 = false;
  param_1->drawData->horizMoveDelta = 0.0;
  psVar7 = *(short **)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + 0x14);
  uVar8 = *(undefined4 *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + 0x10);
  vertOffset = (float)(int)((uint)(byte)PTR__gKeys_001c4af8[2] - (uint)(byte)*PTR__gKeys_001c4af8) *
               2.2;
  if (((vertOffset != 0.0) || (NAN(vertOffset))) &&
     (sVar16._0_1_ = param_1->f_16a, sVar16._1_1_ = param_1->f_16b, sVar16 != 0)) {
    sVar22._0_1_ = param_1->f_16a;
    sVar22._1_1_ = param_1->f_16b;
    sVar16 = _sign((int)sVar22);
    sVar22 = _sign((int)(short)(int)vertOffset);
    if (sVar16 != sVar22) {
      if (param_1->frameAdvance == 1) {
        local_4e = 0xffff;
      }
      else {
        local_4e = 1;
      }
      param_1->f_0fe = (undefined1)local_4e;
      param_1->f_0ff = local_4e._1_1_;
    }
  }
  if ((vertOffset == 0.0) ||
     (param_1->frameChangeProc == (FrameProcPtr_t)PTR_HeroMiscFrameProc_001c41d0)) {
    if (vertOffset == 0.0) {
      _SWSetSpriteFrameTime(param_1,-1);
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
    }
  }
  else {
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_HeroMiscFrameProc_001c41d0);
    _SWSetSpriteFrameAdvance(param_1,0);
    _SWSetSpriteFrameTime(param_1,0);
    param_1->f_0fc = '\x03';
    param_1->f_0fd = '\0';
    param_1->f_100 = '\x03';
    param_1->f_101 = '\0';
    sVar18._0_1_ = param_1->f_0fe;
    sVar18._1_1_ = param_1->f_0ff;
    if (sVar18 == 0) {
      param_1->f_0fe = '\x01';
      param_1->f_0ff = '\0';
    }
  }
  _SWOffsetSprite(param_1,0.0,vertOffset);
  if ((vertOffset != 0.0) || (NAN(vertOffset))) {
    param_1->f_16a = (char)(short)(int)vertOffset;
    param_1->f_16b = (char)((ushort)(short)(int)vertOffset >> 8);
  }
  uVar1 = (param_1->destFrameRect).bottom;
  uVar9 = (param_1->destFrameRect).right;
  uVar2 = (param_1->destFrameRect).top;
  uVar10 = (param_1->destFrameRect).left;
  sVar16 = (uVar1 - *(short *)&param_1->f_146) + -0x15;
  if (((vertOffset < 0.0) && (param_1->f_179 != '\0')) &&
     ((int)sVar16 / (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4) <
      (int)(short)(((param_1->oldFrameRect).bottom - *(short *)&param_1->f_146) + -0x15) /
      (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4))) {
    local_2a = (short)(((int)(short)uVar10 +
                        (int)(short)(((int)(short)uVar9 - (int)(short)uVar10) / 2) + -4) /
                      (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe2));
    sVar18 = sVar16 / *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4);
    sVar19 = _GetTileID(psVar7,(int)sVar18,(int)local_2a);
    sVar20 = _GetTileID(psVar7,(int)(short)(sVar18 + -1),(int)local_2a);
    sVar21 = _GetTileID(psVar7,(int)(short)(sVar18 + 1),(int)local_2a);
    sVar22 = *psVar7;
    _GetTileID(uVar8,(int)(short)(sVar18 + 1),(int)local_2a);
    _GetTileID(uVar8,(int)sVar18,(int)local_2a);
    sVar17 = _GetTileID(psVar7,(int)(short)(((short)uVar2 + 0x19) /
                                           (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 +
                                                          0xe4)),(int)local_2a);
    if ((sVar18 < sVar22) && (sVar22 = _GetLadderValue((int)sVar19), sVar22 == 1)) {
      uVar3 = (param_1->destFrameRect).bottom;
      uVar4 = (param_1->destFrameRect).top;
      _SWMoveSprite(param_1,(float)(int)(param_1->destFrameRect).left,
                    (float)(int)(short)(((sVar16 / *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 +
                                                             0xe4)) * 0x10 -
                                        ((uVar3 - *(short *)&param_1->f_146) - uVar4)) + 0x25));
      param_1->f_16a = 0xff;
      param_1->f_16b = 0xff;
      param_1->f_18e = '\x04';
      param_1->f_18f = '\0';
      _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR__FinishClimbingLadderMoveProc_001c4da4);
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
    }
    else {
      sVar16 = _GetLadderValue((int)sVar19);
      if (((sVar16 == 0) &&
          ((sVar16 = _GetLadderValue((int)sVar21), sVar16 == 0 &&
           (sVar16 = _GetLadderValue((int)sVar20), sVar16 == 0)))) &&
         (sVar16 = _GetLadderValue((int)sVar17), sVar16 == 0)) {
        bVar13 = true;
      }
    }
  }
  uVar5 = (param_1->destFrameRect).bottom;
  uVar11 = (param_1->destFrameRect).right;
  uVar6 = (param_1->destFrameRect).top;
  uVar12 = (param_1->destFrameRect).left;
  sVar16 = uVar5 - *(short *)&param_1->f_146;
  if ((vertOffset <= 0.0) ||
     ((int)sVar16 / (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4) <=
      (int)(short)((param_1->oldFrameRect).bottom - *(short *)&param_1->f_146) /
      (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4))) {
    if (0.0 < vertOffset) {
      sVar16 = sVar16 / *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4);
      sVar22 = _GetTileID(psVar7,(int)sVar16,(int)local_2a);
      sVar16 = _GetTileID(psVar7,(int)(short)(sVar16 + 1),(int)local_2a);
      sVar22 = _GetLadderValue((int)sVar22);
      if (((sVar22 == 0) && (sVar16 = _GetLadderValue((int)sVar16), sVar16 == 0)) &&
         (cVar14 = _SWCheckSpriteWithSprites
                             (param_1,0,3,&param_1->f_102,
                              *(undefined4 *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,
                              *(undefined4 *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c,0,1,0,1)
         , cVar14 != '\0')) {
        bVar13 = true;
        _MMPlaySound(3,9,3);
      }
    }
  }
  else {
    sVar22 = (short)(((int)(short)uVar12 +
                      (int)(short)(((int)(short)uVar11 - (int)(short)uVar12) / 2) + -4) /
                    (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe2));
    sVar18 = _GetTileID(psVar7,(int)(short)(((short)uVar6 + 0x19) /
                                           (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 +
                                                          0xe4)),(int)sVar22);
    sVar16 = sVar16 / *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4);
    sVar19 = _GetTileID(psVar7,(int)sVar16,(int)sVar22);
    sVar20 = _GetTileID(psVar7,(int)(short)(sVar16 + -1),(int)sVar22);
    _GetTileID(psVar7,(int)(short)(sVar16 + 1),(int)sVar22);
    sVar21 = _GetTileID(uVar8,(int)sVar16,(int)sVar22);
    sVar16 = _GetTileID(uVar8,(int)sVar16,(int)(short)(sVar22 + 1));
    sVar22 = _GetLadderValue((int)sVar20);
    if (((sVar22 == 0) || (sVar22 = _GetLadderValue((int)sVar20), sVar22 == 3)) &&
       ((cVar14 = _IsTileFloorTile((int)sVar21,1), cVar14 != '\0' ||
        (cVar14 = _IsTileFloorTile((int)sVar16,1), cVar14 != '\0')))) {
      _SWOffsetSprite(param_1,0.0,-2.2);
      bVar13 = true;
      _MMPlaySound(3,9,3);
    }
    else {
      sVar16 = _GetLadderValue((int)sVar18);
      if (((sVar16 == 0) || (sVar16 = _GetLadderValue((int)sVar18), sVar16 == 3)) &&
         ((sVar16 = _GetLadderValue((int)sVar20), sVar16 == 0 &&
          (sVar16 = _GetLadderValue((int)sVar19), sVar16 != 1)))) {
        bVar13 = true;
        cVar14 = _CheckForWall(param_1,3,&param_1->f_102,0,0,1);
        if (cVar14 != '\0') {
          _MMPlaySound(3,9,3);
        }
      }
    }
  }
  if (bVar13) {
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4098);
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
    _SWSetSpriteMoveDelta(param_1,0.0,0.0);
    if (*PTR__gCanUseLadderJump_001c4828 == '\0') {
      uVar15 = _MoveSpriteDownToFloor(param_1);
      param_1->f_17e = uVar15;
      _SaveLastOnGroundPixelRow(param_1);
      if (param_1->f_17e != '\0') {
        _SetHeroGroundFrame(param_1);
      }
    }
    else {
      _MoveSpriteDownToFloor(param_1);
      param_1->f_17e = '\x01';
      _SetHeroGroundFrame(param_1);
    }
    if (*PTR__gKeys_001c4af8 == '\0') {
      if (PTR__gKeys_001c4af8[2] != '\0') {
        param_1->f_1a0 = '\r';
        param_1->f_1a1 = '\0';
        param_1->f_16f = '\0';
      }
    }
    else {
      param_1->f_19e = '\r';
      param_1->f_19f = '\0';
    }
    param_1->f_16a = '\0';
    param_1->f_16b = '\0';
  }
  param_1->f_179 = '\x01';
  _ExitRoomIfAppropriate(param_1);
  _UpdateHaloSpritePosition(param_1);
  return;
}


// ---- _FinishClimbingLadderMoveProc @ 0x602e2 (1262 bytes)

void _FinishClimbingLadderMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  bool bVar3;
  bool bVar4;
  uchar uVar5;
  short sVar6;
  short local_14;
  short local_12;
  short local_e;
  
  _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
  if (param_1->parentSpriteLayerP != *(SpriteLayerRec **)PTR__gHeroSpriteLayerP_001c4164) {
    _SWRemoveSprite(param_1);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164,param_1);
  }
  sVar6 = *(short *)&param_1->f_18e;
  *(short *)&param_1->f_18e = sVar6 + -1;
  if (sVar6 < 1) {
    param_1->f_18e = '\x04';
    param_1->f_18f = '\0';
    sVar6 = (short)param_1->curFrameIndex;
    bVar4 = false;
    bVar3 = false;
    _SWSetSpriteFrameAdvance(param_1,0);
    if ((sVar6 < 0x1a) || (0x1d < sVar6)) {
      _SWSetSpriteFrameRange(param_1,0x1a,0x1d);
      sVar6._0_1_ = param_1->f_16a;
      sVar6._1_1_ = param_1->f_16b;
      if (sVar6 < 1) {
        local_14 = 0x1a;
        _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
        local_12 = 0;
      }
      else {
        local_14 = 0x1d;
        _SWSetSpriteFlipX(param_1,param_1->f_177 == '\0');
        local_12 = 8;
      }
    }
    else {
      if ((PTR__gKeys_001c4af8[2] != '\0') || (*PTR__gKeys_001c4af8 != '\0')) {
        if (PTR__gKeys_001c4af8[2] == '\0') {
          param_1->f_16a = 0xff;
          param_1->f_16b = 0xff;
        }
        else {
          param_1->f_16a = '\x01';
          param_1->f_16b = '\0';
        }
      }
      local_e = sVar6 + -0x1a;
      sVar1._0_1_ = param_1->f_16a;
      sVar1._1_1_ = param_1->f_16b;
      if (sVar1 < 0) {
        local_e = sVar6 + -0x19;
      }
      if (local_e < 0) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c"
                   ,PTR____FUNCTION___28852_001c4dd0,0x641);
      }
      if (5 < local_e) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c"
                   ,PTR____FUNCTION___28852_001c4dd0,0x642);
      }
      local_12 = *(short *)(PTR__ladderOffsetArray_001c4da0 + local_e * 2);
      sVar2._0_1_ = param_1->f_16a;
      sVar2._1_1_ = param_1->f_16b;
      if (sVar2 < 1) {
        local_12 = -local_12;
        local_14 = sVar6 + 1;
        if (sVar6 == 0x1d) {
          bVar3 = true;
        }
      }
      else {
        local_14 = sVar6 + -1;
        if (sVar6 == 0x1a) {
          bVar4 = true;
        }
      }
    }
    if ((!bVar3) && (!bVar4)) {
      _SWSetCurrentFrameIndex(param_1,(int)local_14);
    }
    _SWOffsetSprite(param_1,0.0,(float)(int)local_12);
    if (bVar3) {
      _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4098);
      _SWSetSpriteFrameAdvance(param_1,1);
      _SWSetSpriteMoveDelta(param_1,0.0,0.0);
      if (*PTR__gCanUseLadderJump_001c4828 == '\0') {
        uVar5 = _MoveSpriteDownToFloor(param_1);
        param_1->f_17e = uVar5;
        _SaveLastOnGroundPixelRow(param_1);
        if (param_1->f_17e != '\0') {
          _SetHeroGroundFrame(param_1);
        }
      }
      else {
        param_1->f_17e = '\x01';
        _SetHeroGroundFrame(param_1);
      }
      if (*PTR__gKeys_001c4af8 == '\0') {
        if (PTR__gKeys_001c4af8[2] != '\0') {
          param_1->f_1a0 = '\r';
          param_1->f_1a1 = '\0';
          param_1->f_16f = '\0';
        }
      }
      else {
        param_1->f_19e = '\r';
        param_1->f_19f = '\0';
      }
    }
    else if (bVar4) {
      _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR__ClimbingSpriteMoveProc_001c4d34);
      _SWResetSpriteMovementTimer(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1);
      _SWSetSpriteFrameTime(param_1,-1);
      _SWResetSpriteFrameTimer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1);
      _SWSetSpriteFrameAdvance(param_1,1);
      if (param_1->curFrameIndex == 0x1a) {
        local_14 = 0x14;
      }
      _SWSetSpriteFrameRange(param_1,0x12,0x19);
      _SWSetSpriteFlipX(param_1,'\0');
      param_1->f_179 = '\0';
      _SWSetCurrentFrameIndex(param_1,(int)local_14);
      _SWOffsetSprite(param_1,0.0,1.0);
      _ClimbingSpriteMoveProc(param_1);
    }
    _UpdateHaloSpritePosition(param_1);
  }
  return;
}


// ---- _VineClimbingMoveProc @ 0x607d0 (2214 bytes)

void _VineClimbingMoveProc(SpritePtr param_1)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  float vertOffset;
  bool bVar8;
  char cVar9;
  short sVar10;
  short sVar11;
  short sVar12;
  short sVar13;
  short sVar14;
  short sVar15;
  SWMovementType horizOffset;
  short local_40;
  short sStack_3e;
  short local_3c;
  short sStack_3a;
  
  if (PTR__gGamepad_001c4ac8[2] != '\0') {
    PTR__gKeys_001c4af8[2] = PTR__gGamepad_001c4ac8[2];
  }
  if (param_1->parentSpriteLayerP != *(SpriteLayerRec **)PTR__gHeroSpriteLayerP_001c4164) {
    _SWRemoveSprite(param_1);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164,param_1);
  }
  bVar8 = false;
  sVar12._0_1_ = param_1->f_1b0;
  sVar12._1_1_ = param_1->f_1b1;
  uVar3 = *(undefined4 *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + sVar12 * 4);
  sVar15._0_1_ = param_1->f_1b0;
  sVar15._1_1_ = param_1->f_1b1;
  uVar4 = *(undefined4 *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + sVar15 * 4);
  uVar5 = *(undefined4 *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + 0x10);
  _SaveStartingPositionIfAppropriate(param_1,0);
  if ((((PTR__gKeys_001c4af8[4] == '\0') || (PTR__gKeys_001c4af8[3] == PTR__gKeys_001c4af8[1])) &&
      (PTR__gKeys_001c4af8[5] == '\0')) && (PTR__gKeys_001c4af8[6] == '\0')) {
    param_1->drawData->horizMoveDelta = 0.0;
    vertOffset = (float)(int)((uint)(byte)PTR__gKeys_001c4af8[2] - (uint)(byte)*PTR__gKeys_001c4af8)
                 * 2.2;
    if ((vertOffset != 0.0) || (NAN(vertOffset))) {
      _MySetSpriteFrameTime(param_1,0x53);
    }
    else {
      _MySetSpriteFrameTime(param_1,0xffffffff);
    }
    horizOffset = _SWGetSpriteHorizMoveDelta(param_1);
    _SWOffsetSprite(param_1,horizOffset,vertOffset);
    uVar1 = (param_1->destFrameRect).bottom;
    uVar6 = (param_1->destFrameRect).right;
    uVar2 = (param_1->destFrameRect).top;
    uVar7 = (param_1->destFrameRect).left;
    sVar12 = uVar2 + *(short *)&param_1->f_122;
    sVar15 = uVar1 - *(short *)&param_1->f_126;
    if ((vertOffset < 0.0) &&
       ((int)sVar15 / (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4) <
        (int)(short)((param_1->oldFrameRect).bottom - *(short *)&param_1->f_126) /
        (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4))) {
      sVar11 = (short)(((int)(short)uVar7 +
                       (int)(short)(((int)(short)uVar6 - (int)(short)uVar7) / 2)) /
                      (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe2));
      sVar13 = sVar15 / *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4);
      _GetTileID(uVar3,(int)sVar13,(int)sVar11);
      sVar14 = _GetTileID(uVar3,(int)(short)(sVar13 + -1),(int)sVar11);
      sVar10 = _GetTileID(uVar4,(int)(short)(sVar13 + 1),(int)sVar11);
      sVar13 = _GetTileID(uVar5,(int)(short)(sVar13 + 1),(int)sVar11);
      sVar11 = _GetTileID(uVar3,(int)(sVar12 / *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4
                                                         )),(int)sVar11);
      _FixHeroPositionOnVine(param_1);
      if (((sVar14 < 0x1f9) || (0x20a < sVar14)) && ((sVar11 < 0x1f9 || (0x20a < sVar11)))) {
        bVar8 = true;
        cVar9 = _IsTileFloorTile((int)sVar10,1);
        if ((cVar9 != '\0') || (cVar9 = _IsTileFloorTile((int)sVar13,1), cVar9 != '\0')) {
          _MMPlaySound(3,9,3);
        }
      }
      else {
        cVar9 = _IsTileFloorTile((int)sVar10,1);
        if (((cVar9 != '\0') || (cVar9 = _IsTileFloorTile((int)sVar13,1), cVar9 != '\0')) &&
           ((sVar11 < 0x1f9 || (0x20a < sVar11)))) {
          bVar8 = true;
          _MMPlaySound(3,9,3);
        }
      }
    }
    if (0.0 < vertOffset) {
      sVar11 = (short)(((int)(short)uVar7 +
                       (int)(short)(((int)(short)uVar6 - (int)(short)uVar7) / 2)) /
                      (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe2));
      sVar12 = _GetTileID(uVar3,(int)(sVar12 / *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4
                                                         )),(int)sVar11);
      sVar15 = sVar15 / *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4);
      _GetTileID(uVar3,(int)sVar15,(int)sVar11);
      sVar13 = _GetTileID(uVar3,(int)(short)(sVar15 + -1),(int)sVar11);
      sVar14 = _GetTileID(uVar4,(int)sVar15,(int)sVar11);
      sVar15 = _GetTileID(uVar5,(int)sVar15,(int)sVar11);
      cVar9 = _IsTileFloorNotVineTile((int)sVar14);
      if ((cVar9 != '\0') || (cVar9 = _IsTileFloorNotVineTile((int)sVar15), cVar9 != '\0')) {
        _SWOffsetSprite(param_1,0.0,-vertOffset - 2.2);
        _MoveSpriteDownToFloor(param_1);
        bVar8 = true;
        _MMPlaySound(3,9,3);
      }
      if (((sVar12 < 0x1f9) || (0x20a < sVar12)) && ((sVar13 < 0x1f9 || (0x20a < sVar13)))) {
        bVar8 = true;
      }
    }
    if (bVar8) {
      _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4098);
      _SWSetSpriteMoveDelta(param_1,0.0,0.0);
      _MoveSpriteDownToFloor(param_1);
      _MoveSpriteDownToFloor(param_1);
      _MoveSpriteDownToFloor(param_1);
      param_1->f_17e = '\x01';
      _SetHeroGroundFrame(param_1);
      *PTR__gKeys_001c4af8 = 0;
      PTR__gKeys_001c4af8[2] = 0;
    }
    _ExitRoomIfAppropriate(param_1);
    _UpdateHaloSpritePosition(param_1);
  }
  else {
    if (PTR__gKeys_001c4af8[5] == '\0') {
      if (PTR__gKeys_001c4af8[6] == '\0') {
        param_1->drawData->horizMoveDelta =
             (float)(int)((uint)(byte)PTR__gKeys_001c4af8[1] - (uint)(byte)PTR__gKeys_001c4af8[3]);
      }
      else {
        param_1->drawData->horizMoveDelta = 1.0;
      }
    }
    else {
      param_1->drawData->horizMoveDelta = -1.0;
    }
    param_1->drawData->horizMoveDelta = param_1->drawData->horizMoveDelta * 2.5;
    if (0.0 < param_1->drawData->horizMoveDelta) {
      param_1->f_177 = '\x01';
    }
    else {
      param_1->f_177 = '\0';
    }
    _StartJump(param_1);
    _SWOffsetSprite(param_1,param_1->drawData->horizMoveDelta,0.0);
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4098);
  }
  return;
}


// ---- _PoleSlidingMoveProc @ 0x61076 (669 bytes)

void _PoleSlidingMoveProc(SpritePtr param_1)

{
  undefined4 uVar1;
  bool bVar2;
  char cVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  longdouble extraout_ST0;
  
  bVar2 = false;
  _SaveStartingPositionIfAppropriate(param_1,0);
  if ((int)(param_1->destFrameRect).top <
      *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x80) + 100) {
    _SWOffsetSprite(param_1,0.0,6.0);
  }
  _ProcessJumpIntention(param_1,param_1->f_17e);
  cVar3 = _CheckForWall(param_1,3,&param_1->f_132,0,0,1);
  if (cVar3 == '\0') {
    uVar1 = *(undefined4 *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + 0x10);
    _SWGetSpriteWidth(param_1);
    sVar6 = (short)(((int)(param_1->destFrameRect).left +
                    (int)(short)(int)((float)extraout_ST0 / 2.0)) /
                   (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe2));
    sVar4 = (short)(((param_1->destFrameRect).bottom + -0xf) /
                   (int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4));
    sVar5 = _GetTileID(uVar1,(int)sVar4,(int)sVar6);
    if ((sVar5 < 0x1e8) || (0x1f7 < sVar5)) {
      sVar6 = _GetTileID(uVar1,(int)(short)(sVar4 + -1),(int)sVar6);
      if ((sVar6 < 0x1e8) || (0x1f7 < sVar6)) {
        bVar2 = true;
        param_1->f_17e = '\0';
      }
    }
  }
  else {
    bVar2 = true;
    param_1->f_17e = '\x01';
    _MMPlaySound(3,9,1);
    _SetHeroGroundFrame(param_1);
  }
  if (bVar2) {
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4098);
    _SWSetSpriteMoveDelta(param_1,0.0,0.0);
    PTR__gKeys_001c4af8[4] = 0;
    PTR__gKeys_001c4af8[5] = 0;
    PTR__gKeys_001c4af8[6] = 0;
    _SaveLastOnGroundPixelRow(param_1);
  }
  _ExitRoomIfAppropriate(param_1);
  _UpdateHaloSpritePosition(param_1);
  return;
}


// ---- _BlinkingSpriteMoveProc @ 0x61313 (478 bytes)

void BlinkingSpriteMoveProc(SpritePtr param_1)

{
  undefined2 uVar1;
  
  if (((((PTR__gKeys_001c4af8[3] != '\0') || (PTR__gKeys_001c4af8[1] != '\0')) ||
       (*PTR__gKeys_001c4af8 != '\0')) ||
      ((PTR__gKeys_001c4af8[2] != '\0' || (PTR__gKeys_001c4af8[4] != '\0')))) ||
     ((PTR__gKeys_001c4af8[5] != '\0' ||
      ((PTR__gKeys_001c4af8[6] != '\0' || (PTR__gKeys_001c4af8[7] != '\0')))))) {
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
    _SWSetSpriteFrameTime(param_1,-1);
    _SWSetSpriteFrameAdvance(param_1,1);
    _SWSetSpriteVisible(param_1,'\x01');
    _SWRemoveSprite(param_1);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164,param_1);
    if (*(short *)PTR__gStartingSpriteState_001c4c68 == 1) {
      _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR__ClimbingSpriteMoveProc_001c4d34);
      _SWSetSpriteFrameAdvanceMode(param_1,(void *)0x0);
      param_1->f_179 = '\0';
      param_1->f_172 = '\0';
    }
    else if (*(short *)PTR__gStartingSpriteState_001c4c68 == 2) {
      _GetOnVine(param_1);
      uVar1 = *(undefined2 *)PTR__gStartingVineTileLayer_001c4cf4;
      param_1->f_1b0 = (char)uVar1;
      param_1->f_1b1 = (char)((ushort)uVar1 >> 8);
    }
    else if (*(short *)PTR__gStartingSpriteState_001c4c68 == 3) {
      _GetOnPole(param_1,0);
    }
    else {
      _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4098);
    }
  }
  _UpdateHaloSpritePosition(param_1);
  return;
}


// ---- _FixHeroPositionOnVine @ 0x614f1 (422 bytes)

void _FixHeroPositionOnVine(SpritePtr param_1)

{
  char cVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  int iVar7;
  longdouble extraout_ST0;
  short local_1c;
  short local_18;
  
  _SWGetSpriteWidth(param_1);
  iVar7 = (int)(param_1->destFrameRect).left + (int)(short)(int)((float)extraout_ST0 / 2.0);
  if (iVar7 < 0) {
    iVar7 = iVar7 + 0xf;
  }
  sVar2 = (short)(iVar7 >> 4);
  local_1c = 1;
  sVar3 = (param_1->destFrameRect).top + 0x10;
  for (local_18 = sVar3; (int)local_18 <= sVar3 + 0x28; local_18 = local_18 + 0x10) {
    sVar4 = local_18 / *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4);
    sVar5 = _MyGetTile(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,4,(int)sVar4,(int)sVar2);
    sVar6._0_1_ = param_1->f_1b0;
    sVar6._1_1_ = param_1->f_1b1;
    sVar6 = _MyGetTile(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)sVar6,(int)(short)(sVar4 + 1)
                       ,(int)sVar2);
    cVar1 = _IsWallTile((int)sVar5);
    if (((cVar1 != '\0') && (0x1f8 < sVar6)) && (sVar6 < 0x20b)) {
      _SWOffsetSprite(param_1,0.0,
                      (float)(int)(short)(*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xe4) *
                                          (sVar4 + local_1c) - local_18));
    }
    local_1c = local_1c + 1;
  }
  return;
}


// ---- _GetOnVineIfAppropriate @ 0x61697 (787 bytes)

undefined1 _GetOnVineIfAppropriate(SpritePtr param_1,short param_2,short param_3,char param_4)

{
  short sVar1;
  short sVar2;
  uint uVar3;
  SWMovementType SVar4;
  undefined4 local_24;
  undefined4 local_20;
  undefined1 local_1c [8];
  SpritePtr local_14;
  short local_10;
  char local_e;
  undefined1 local_d;
  
  local_14 = param_1;
  local_d = 0;
  param_1->f_1b0 = (undefined1)param_3;
  param_1->f_1b1 = param_3._1_1_;
  if (param_2 == 0) {
    local_e = _GetLadderRect(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1,&param_1->f_122,
                             local_1c,(int)param_3,0x1fb,0x20a);
  }
  else {
    local_e = _GetLadderRect(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1,&param_1->f_12a,
                             local_1c,(int)param_3,0x1fb,0x1fc);
  }
  if (local_e != '\0') {
    local_20._0_2_ = (param_1->destFrameRect).bottom;
    local_20._2_2_ = (param_1->destFrameRect).right;
    local_24._0_2_ = (param_1->destFrameRect).top;
    local_24._2_2_ = (param_1->destFrameRect).left;
    _SWCenterRect(&local_24,local_1c);
    local_10 = local_24._2_2_ - (param_1->destFrameRect).left;
    if ((-0x11 < local_10) && (local_10 < 0x11)) {
      sVar1 = _sign((int)local_10);
      SVar4 = _SWGetSpriteHorizMoveDelta(param_1);
      sVar2 = _sign((int)(short)(int)SVar4);
      if ((sVar1 == sVar2) || (SVar4 = _SWGetSpriteHorizMoveDelta(param_1), SVar4 == 0.0)) {
        _StopMySound(0x28);
        _StopChannel(4);
        if ((param_2 == 0) && (param_4 != '\0')) {
          _MMPlaySound(10,9,1);
        }
        _EndJump(param_1);
        if (param_2 == 0) {
          uVar3 = (int)local_10 >> 0x1f;
          _SWOffsetSprite(param_1,0.0,(float)(int)(((int)local_10 ^ uVar3) - uVar3));
          do {
            local_e = _GetLadderRect(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1,
                                     &local_14->f_122,local_1c,(int)param_3,0x1fb,0x20a);
            if (local_e == '\0') {
              _SWOffsetSprite(param_1,0.0,-2.0);
            }
          } while (local_e == '\0');
        }
        _CheckForWall(param_1,3,&local_14->f_142,local_14->f_170,0,1);
        _SWOffsetSprite(param_1,(float)(int)local_10,0.0);
        _GetOnVine(param_1);
        _FixHeroPositionOnVine(param_1);
        local_d = 1;
      }
    }
  }
  return local_d;
}


// ---- _GetOnPoleIfAppropriate @ 0x619aa (733 bytes)

char _GetOnPoleIfAppropriate(SpritePtr param_1)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  undefined1 uVar4;
  char cVar5;
  short sVar6;
  short sVar7;
  SWMovementType SVar8;
  undefined4 local_30;
  undefined4 local_2c;
  undefined1 local_28 [8];
  undefined4 local_20;
  undefined4 local_1c;
  SpritePtr local_18;
  char local_14;
  char local_13;
  short local_12;
  undefined4 local_10;
  
  local_18 = param_1;
  local_13 = '\0';
  if (param_1->userData != 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___29416_001c4dc4,0x8cb);
  }
  local_14 = _GetPoleRect(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1,&local_18->f_122,
                          local_28,4,0x1e7,0x1f8);
  if (local_14 == '\0') {
    local_14 = _GetPoleRect(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1,&local_18->f_122,
                            local_28,5,0x1e7,0x1f8);
  }
  if (local_14 != '\0') {
    local_1c._0_2_ = (param_1->destFrameRect).bottom;
    local_1c._2_2_ = (param_1->destFrameRect).right;
    local_20._0_2_ = (param_1->destFrameRect).top;
    local_20._2_2_ = (param_1->destFrameRect).left;
    _SWCenterRect(&local_20,local_28);
    local_12 = local_20._2_2_ - (param_1->destFrameRect).left;
    if (local_18->f_170 == '\0') {
      if ((-0x11 < local_12) && (local_12 < 0x11)) {
        _SWOffsetSprite(param_1,(float)(int)local_12,0.0);
        local_2c._0_1_ = local_18->f_106;
        local_2c._1_1_ = local_18->f_107;
        uVar1 = local_18->f_108;
        uVar3 = local_18->f_109;
        local_2c._2_2_ = CONCAT11(uVar3,uVar1);
        local_30._0_1_ = local_18->f_102;
        local_30._1_1_ = local_18->f_103;
        uVar2 = local_18->f_104;
        uVar4 = local_18->f_105;
        local_30._2_2_ = CONCAT11(uVar4,uVar2);
        if (local_12 < 0) {
          local_10 = 4;
          local_30 = CONCAT22(local_30._2_2_ + 0x10,(undefined2)local_30);
        }
        else {
          local_10 = 2;
          local_2c = CONCAT22(local_2c._2_2_ + 0x10,(undefined2)local_2c);
        }
        cVar5 = _CheckForWall(param_1,local_10,&local_30,local_18->f_170,0,0);
        if (cVar5 == '\0') {
          local_13 = '\x01';
          local_12 = 0;
        }
        else {
          local_13 = '\0';
          _SWOffsetSprite(param_1,(float)-(int)local_12,0.0);
        }
      }
    }
    else if ((-0xd < local_12) && (local_12 < 0xd)) {
      SVar8 = _SWGetSpriteHorizMoveDelta(param_1);
      if ((SVar8 != 0.0) || (NAN(SVar8))) {
        sVar6 = _sign((int)local_12);
        SVar8 = _SWGetSpriteHorizMoveDelta(param_1);
        sVar7 = _sign((int)(short)(int)SVar8);
        if (sVar6 == sVar7) {
          local_13 = '\x01';
        }
      }
    }
    if (local_13 != '\0') {
      _GetOnPole(param_1,(int)local_12);
      _PoleSlidingMoveProc(param_1);
    }
  }
  return local_13;
}


// ---- _GetOnVine @ 0x61c87 (297 bytes)

void _GetOnVine(SpritePtr param_1)

{
  _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR__VineClimbingMoveProc_001c4d6c);
  _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
  _SWSetSpriteFrameRange(param_1,0x1e,0x21);
  _SWSetSpriteFlipX(param_1,'\0');
  _SWSetSpriteFrameAdvanceMode(param_1,(void *)0x0);
  _SWSetSpriteFrameAdvance(param_1,1);
  param_1->f_179 = '\0';
  param_1->f_172 = '\0';
  param_1->f_17f = '\0';
  param_1->f_1c4 = '\0';
  param_1->f_1c5 = '\0';
  param_1->f_1c6 = '\0';
  param_1->f_1c7 = '\0';
  param_1->f_15c = '\0';
  param_1->f_15d = '\0';
  param_1->f_15e = '\0';
  param_1->f_15f = '\0';
  param_1->f_160 = '\0';
  param_1->f_161 = '\0';
  param_1->f_162 = '\0';
  param_1->f_163 = '\0';
  param_1->f_150 = '\0';
  param_1->f_151 = '\0';
  param_1->f_152 = '\0';
  param_1->f_153 = '\0';
  param_1->f_154 = '\0';
  param_1->f_155 = '\0';
  param_1->f_156 = '\0';
  param_1->f_157 = '\0';
  param_1->f_158 = '\0';
  param_1->f_159 = '\0';
  param_1->f_15a = '\0';
  param_1->f_15b = '\0';
  param_1->f_19a = '\0';
  param_1->f_19b = '\0';
  return;
}


// ---- _GetOnPole @ 0x61db0 (274 bytes)

void _GetOnPole(SpritePtr param_1,short param_2)

{
  _SWSetSpriteFrameRange(param_1,0x1f,0x1f);
  _SWSetSpriteFlipX(param_1,'\0');
  _EndJump(param_1);
  param_1->f_19a = '\0';
  param_1->f_19b = '\0';
  _StopMySound(0x28);
  _StopChannel(4);
  _SWOffsetSprite(param_1,(float)(int)param_2,0.0);
  _SWSetSpriteMoveDelta(param_1,0.0,0.0);
  _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR__PoleSlidingMoveProc_001c4d44);
  _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
  _SWSetSpriteFrameTime(param_1,-1);
  param_1->f_17f = '\0';
  return;
}


// ---- _SkullSpiderMoveProc @ 0x61ec2 (2314 bytes)

void SkullSpiderMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  undefined2 uVar3;
  undefined1 uVar9;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined2 uVar4;
  undefined2 uVar10;
  undefined2 uVar11;
  undefined4 uVar7;
  undefined1 uVar13;
  SWBoolean SVar14;
  char cVar15;
  int iVar16;
  int iVar17;
  int iVar18;
  SWMovementType SVar19;
  undefined2 uStack_3a;
  short sStack_38;
  undefined4 uStack_36;
  undefined4 uStack_32;
  undefined4 uStack_2e;
  short sStack_2a;
  short sStack_28;
  short sStack_26;
  short sStack_24;
  short sStack_22;
  short sStack_20;
  short sStack_1e;
  float fStack_1c;
  undefined4 uStack_18;
  SpritePtr pSStack_14;
  char cStack_e;
  char cStack_d;
  undefined1 uVar8;
  undefined1 uVar12;
  
  pSStack_14 = param_1;
  cStack_d = '\0';
  uVar5._0_1_ = param_1->f_100;
  uVar5._1_1_ = param_1->f_101;
  uVar5._2_1_ = param_1->f_102;
  uVar5._3_1_ = param_1->f_103;
  uVar6._0_1_ = param_1->f_0fc;
  uVar6._1_1_ = param_1->f_0fd;
  uVar6._2_1_ = param_1->f_0fe;
  uVar6._3_1_ = param_1->f_0ff;
  cStack_e = _HandleSpriteWithMovingPlatforms(param_1,uVar6,uVar5);
  uStack_18 = *(undefined4 *)(*(int *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xc4) + 0x10);
  if (param_1->drawData->vertMoveDelta <= 1.0) {
    param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 1.0;
  }
  else {
    param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 0.43333334;
  }
  if (12.0 < param_1->drawData->vertMoveDelta) {
    param_1->drawData->vertMoveDelta = 12.0;
  }
  _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
  fStack_1c = *(float *)PTR__gCurrentWindDelta_001c4898;
  if (0.0 < param_1->drawData->vertMoveDelta) {
    sVar1._0_1_ = pSStack_14->f_100;
    sVar1._1_1_ = pSStack_14->f_101;
    iVar17 = (int)(param_1->oldFrameRect).bottom - (int)sVar1;
    iVar16 = iVar17 + -1;
    if (iVar16 < 0) {
      iVar16 = iVar17 + 0xe;
    }
    sVar2._0_1_ = pSStack_14->f_100;
    sVar2._1_1_ = pSStack_14->f_101;
    iVar18 = (int)(param_1->destFrameRect).bottom - (int)sVar2;
    iVar17 = iVar18 + -1;
    if (iVar17 < 0) {
      iVar17 = iVar18 + 0xe;
    }
    if (iVar16 >> 4 < iVar17 >> 4) {
      SVar14 = _SWCheckSpriteWithTiles
                         (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,(void *)0x3,
                          (SWRect *)&pSStack_14->f_0fc,4,4,0x241,0x243,'\0');
      if (SVar14 != '\0') {
        fStack_1c = fStack_1c - (float)(int)*(short *)PTR__gCurConveyorBeltDirection_001c470c;
      }
      SVar14 = _SWCheckSpriteWithTiles
                         (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,(void *)0x3,
                          (SWRect *)&pSStack_14->f_0fc,4,4,0x249,0x24b,'\0');
      if (SVar14 != '\0') {
        fStack_1c = fStack_1c + (float)(int)*(short *)PTR__gCurConveyorBeltDirection_001c470c;
      }
    }
    cVar15 = _CheckForWall(param_1,3,&pSStack_14->f_0fc,0,0,1);
    if (cVar15 != '\0') {
      param_1->drawData->vertMoveDelta = 0.0;
      cStack_d = '\x01';
    }
  }
  SVar19 = _SWGetSpriteHorizMoveDelta(param_1);
  _SWOffsetSprite(param_1,SVar19 + fStack_1c,0.0);
  uVar3 = (param_1->destFrameRect).bottom;
  uVar10 = (param_1->destFrameRect).right;
  uVar4 = (param_1->destFrameRect).top;
  uVar11 = (param_1->destFrameRect).left;
  sStack_2a = uVar4 + *(short *)&pSStack_14->f_0fc;
  sStack_28 = uVar11 + *(short *)&pSStack_14->f_0fe;
  sStack_26 = uVar3 - *(short *)&pSStack_14->f_100;
  sStack_24 = uVar10 - *(short *)&pSStack_14->f_102;
  _SWOffsetRect(&sStack_2a,0xfffffffe,0);
  uVar7._0_1_ = pSStack_14->f_100;
  uVar7._1_1_ = pSStack_14->f_101;
  uVar8 = pSStack_14->f_102;
  uVar12 = pSStack_14->f_103;
  uVar7._2_2_ = CONCAT11(uVar12,uVar8);
  uStack_32._0_1_ = pSStack_14->f_0fc;
  uStack_32._1_1_ = pSStack_14->f_0fd;
  uVar9 = pSStack_14->f_0fe;
  uVar13 = pSStack_14->f_0ff;
  uStack_32._2_2_ = CONCAT11(uVar13,uVar9);
  uStack_2e._0_2_ = (short)uVar7;
  uStack_2e = CONCAT22(uVar7._2_2_,(short)uStack_2e + -0x10);
  uStack_36 = uStack_2e;
  _uStack_3a = CONCAT22(uStack_32._2_2_ + -8,(short)uStack_32);
  SVar19 = _SWGetSpriteHorizMoveDelta(param_1);
  if (0.0 < SVar19 + fStack_1c) {
    iVar16 = sStack_24 + 1;
    if (iVar16 < 0) {
      iVar16 = sStack_24 + 0x10;
    }
    sStack_20 = (short)(iVar16 >> 4);
    iVar16 = sStack_26 + 1;
    if (iVar16 < 0) {
      iVar16 = sStack_26 + 0x10;
    }
    sStack_22 = (short)(iVar16 >> 4);
    sStack_1e = _GetTileID(uStack_18,(int)sStack_22,(int)sStack_20);
    cVar15 = _CheckForWall(param_1,2,&pSStack_14->f_0fc,0,0,1);
    if (((cVar15 != '\0') ||
        (((cVar15 = _IsTileFloorTile((int)sStack_1e,0), cVar15 == '\0' &&
          (cVar15 = _SWCheckSpriteWithSprites
                              (param_1,0,2,&uStack_32,
                               *(undefined4 *)PTR__gTrapDoorSpriteLayerP_001c40e0,
                               *(undefined4 *)PTR__gTrapDoorSpriteLayerP_001c40e0,0,1,1,0),
          cVar15 == '\0')) &&
         (cVar15 = _SWCheckSpriteWithSprites
                             (param_1,0,0,&uStack_3a,
                              *(undefined4 *)PTR__gBrickSwitchSpriteLayerP_001c4188,
                              *(undefined4 *)PTR__gBrickSwitchSpriteLayerP_001c4188,0,1,1,0),
         cVar15 == '\0')))) &&
       ((SVar19 = _SWGetSpriteHorizMoveDelta(param_1), 0.0 < SVar19 && (cStack_e == '\0')))) {
      param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
    }
  }
  else {
    SVar19 = _SWGetSpriteHorizMoveDelta(param_1);
    if (SVar19 + fStack_1c < 0.0) {
      sStack_20 = sStack_28;
      if (sStack_28 < 0) {
        sStack_20 = sStack_28 + 0xf;
      }
      sStack_20 = sStack_20 >> 4;
      iVar16 = sStack_26 + 1;
      if (iVar16 < 0) {
        iVar16 = sStack_26 + 0x10;
      }
      sStack_22 = (short)(iVar16 >> 4);
      sStack_1e = _GetTileID(uStack_18,(int)sStack_22,(int)sStack_20);
      cVar15 = _CheckForWall(param_1,4,&pSStack_14->f_0fc,0,0,1);
      if (((cVar15 != '\0') ||
          (((cVar15 = _IsTileFloorTile((int)sStack_1e,0), cVar15 == '\0' &&
            (cVar15 = _SWCheckSpriteWithSprites
                                (param_1,0,4,&uStack_32,
                                 *(undefined4 *)PTR__gTrapDoorSpriteLayerP_001c40e0,
                                 *(undefined4 *)PTR__gTrapDoorSpriteLayerP_001c40e0,0,1,1,0),
            cVar15 == '\0')) &&
           (cVar15 = _SWCheckSpriteWithSprites
                               (param_1,0,0,&uStack_3a,
                                *(undefined4 *)PTR__gBrickSwitchSpriteLayerP_001c4188,
                                *(undefined4 *)PTR__gBrickSwitchSpriteLayerP_001c4188,0,1,1,0),
           cVar15 == '\0')))) &&
         ((SVar19 = _SWGetSpriteHorizMoveDelta(param_1), SVar19 < 0.0 && (cStack_e == '\0')))) {
        param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
      }
    }
  }
  if ((*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7e) <= sStack_28) ||
     (SVar19 = _SWGetSpriteHorizMoveDelta(param_1), 0.0 <= SVar19)) {
    if ((*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x82) < sStack_24) &&
       (SVar19 = _SWGetSpriteHorizMoveDelta(param_1), 0.0 < SVar19)) {
      param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
    }
  }
  else {
    param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
  }
  SVar19 = _SWGetSpriteHorizMoveDelta(param_1);
  if (0.0 < SVar19) {
    _SWSetSpriteFlipX(param_1,'\x01');
  }
  else {
    _SWSetSpriteFlipX(param_1,'\0');
  }
  _RemoveSpriteIfOutsideRoom(param_1);
  if (cStack_d == '\0') {
    _CheckSpriteWithLavaTiles(param_1,1);
  }
  return;
}


// ---- _ScorpionMoveProc @ 0x627cc (1523 bytes)

void ScorpionMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  bool bVar7;
  SWBoolean SVar8;
  char cVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  SWMovementType SVar13;
  short sStack_1e;
  short sStack_1a;
  float fStack_18;
  
  uVar3._0_1_ = param_1->f_104;
  uVar3._1_1_ = param_1->f_105;
  uVar3._2_1_ = param_1->f_106;
  uVar3._3_1_ = param_1->f_107;
  uVar4._0_1_ = param_1->f_100;
  uVar4._1_1_ = param_1->f_101;
  uVar4._2_1_ = param_1->f_102;
  uVar4._3_1_ = param_1->f_103;
  _HandleSpriteWithMovingPlatforms(param_1,uVar4,uVar3);
  bVar7 = false;
  if (param_1->drawData->vertMoveDelta <= 1.0) {
    param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 1.0;
  }
  else {
    param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 0.43333334;
  }
  if (8.0 < param_1->drawData->vertMoveDelta) {
    param_1->drawData->vertMoveDelta = 8.0;
  }
  _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
  fStack_18 = *(float *)PTR__gCurrentWindDelta_001c4898;
  if (0.0 < param_1->drawData->vertMoveDelta) {
    sVar1._0_1_ = param_1->f_104;
    sVar1._1_1_ = param_1->f_105;
    iVar11 = (int)(param_1->oldFrameRect).bottom - (int)sVar1;
    iVar10 = iVar11 + -1;
    if (iVar10 < 0) {
      iVar10 = iVar11 + 0xe;
    }
    sVar2._0_1_ = param_1->f_104;
    sVar2._1_1_ = param_1->f_105;
    iVar12 = (int)(param_1->destFrameRect).bottom - (int)sVar2;
    iVar11 = iVar12 + -1;
    if (iVar11 < 0) {
      iVar11 = iVar12 + 0xe;
    }
    if (iVar10 >> 4 < iVar11 >> 4) {
      SVar8 = _SWCheckSpriteWithTiles
                        (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,(void *)0x3,
                         (SWRect *)&param_1->f_100,4,4,0x241,0x243,'\0');
      if (SVar8 != '\0') {
        fStack_18 = fStack_18 - (float)(int)*(short *)PTR__gCurConveyorBeltDirection_001c470c;
      }
      SVar8 = _SWCheckSpriteWithTiles
                        (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,(void *)0x3,
                         (SWRect *)&param_1->f_100,4,4,0x249,0x24b,'\0');
      if (SVar8 != '\0') {
        fStack_18 = fStack_18 + (float)(int)*(short *)PTR__gCurConveyorBeltDirection_001c470c;
      }
    }
    cVar9 = _CheckForWall(param_1,3,&param_1->f_100,0,0,1);
    if (cVar9 != '\0') {
      param_1->drawData->vertMoveDelta = 0.0;
      bVar7 = true;
    }
  }
  SVar13 = _SWGetSpriteHorizMoveDelta(param_1);
  _SWOffsetSprite(param_1,SVar13 + fStack_18,0.0);
  SVar13 = _SWGetSpriteHorizMoveDelta(param_1);
  if (0.0 < SVar13 + fStack_18) {
    cVar9 = _CheckForWall(param_1,2,&param_1->f_100,0,0,1);
    if ((cVar9 != '\0') && (SVar13 = _SWGetSpriteHorizMoveDelta(param_1), 0.0 < SVar13)) {
      param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
    }
  }
  else {
    SVar13 = _SWGetSpriteHorizMoveDelta(param_1);
    if (((SVar13 + fStack_18 < 0.0) &&
        (cVar9 = _CheckForWall(param_1,4,&param_1->f_100,0,0,1), cVar9 != '\0')) &&
       (param_1->drawData->horizMoveDelta < 0.0)) {
      param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
    }
  }
  uVar5 = (param_1->destFrameRect).right;
  uVar6 = (param_1->destFrameRect).left;
  if ((*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7e) <=
       (short)(uVar6 + *(short *)&param_1->f_102)) ||
     (SVar13 = _SWGetSpriteHorizMoveDelta(param_1), 0.0 <= SVar13)) {
    if ((*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x82) < (short)uVar5) &&
       (0.0 < param_1->drawData->horizMoveDelta)) {
      param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
    }
  }
  else {
    param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
  }
  SVar13 = _SWGetSpriteHorizMoveDelta(param_1);
  if (0.0 < SVar13) {
    _SWSetSpriteFlipX(param_1,'\x01');
  }
  else {
    _SWSetSpriteFlipX(param_1,'\0');
  }
  _RemoveSpriteIfOutsideRoom(param_1);
  if (!bVar7) {
    _CheckSpriteWithLavaTiles(param_1,1);
  }
  return;
}


// ---- _EyeballMoveProc @ 0x62dbf (1342 bytes)

void EyeballMoveProc(SpritePtr param_1)

{
  float fVar1;
  char cVar2;
  short sVar3;
  SpritePtr srcSpriteP;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SWMovementType horizLoc;
  SWMovementType SVar4;
  SWMovementType SVar5;
  
  param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 0.21666667;
  _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
  fVar1 = *(float *)PTR__gCurrentWindDelta_001c4898;
  if (0.0 < param_1->drawData->vertMoveDelta) {
    horizLoc = _SWGetSpriteHorizLoc(param_1);
    SVar4 = _SWGetSpriteVertLoc(param_1);
    cVar2 = _CheckForWall(param_1,3,&param_1->f_0fc,0,0,1);
    if (cVar2 != '\0') {
      SVar5 = _SWGetSpriteVertLoc(param_1);
      _SWMoveSprite(param_1,horizLoc,SVar4 - param_1->drawData->vertMoveDelta);
      sVar3 = _GetRandom(0x10,0x11);
      _MMPlayStereoSound(param_1,(int)sVar3,0xb,1);
      SVar4 = _SWGetSpriteHorizLoc(param_1);
      *(SWMovementType *)&param_1->f_110 = SVar4;
      SVar4 = _SWGetSpriteVertLoc(param_1);
      *(SWMovementType *)&param_1->f_10c = SVar4;
      *(float *)&param_1->f_104 = param_1->drawData->vertMoveDelta;
      _SWGetSpriteHeight(param_1);
      param_1->f_108 = (char)(short)(int)extraout_ST0;
      param_1->f_109 = (char)((ushort)(short)(int)extraout_ST0 >> 8);
      _SWGetSpriteWidth(param_1);
      param_1->f_10a = (char)(short)(int)extraout_ST0_00;
      param_1->f_10b = (char)((ushort)(short)(int)extraout_ST0_00 >> 8);
      param_1->f_178 = '\0';
      param_1->f_179 = '\0';
      if (8.0 <= param_1->drawData->vertMoveDelta) {
        param_1->drawData->vertMoveDelta = 8.0;
      }
      else {
        param_1->drawData->vertMoveDelta = 7.0;
      }
      param_1->f_17a = '\x01';
      param_1->f_17b = '\0';
      _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4d94);
      srcSpriteP = (SpritePtr)_NewSlimeSprite();
      _SWSetSpriteLocation(srcSpriteP,horizLoc,SVar5 + 17.0);
    }
  }
  _SWOffsetSprite(param_1,param_1->drawData->horizMoveDelta + fVar1,0.0);
  if (0.0 < param_1->drawData->horizMoveDelta + fVar1) {
    cVar2 = _CheckForWall(param_1,2,&param_1->f_0fc,0,0,1);
    if ((cVar2 != '\0') && (0.0 < param_1->drawData->horizMoveDelta)) {
      param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
    }
  }
  else if (((param_1->drawData->horizMoveDelta + fVar1 < 0.0) &&
           (cVar2 = _CheckForWall(param_1,4,&param_1->f_0fc,0,0,1), cVar2 != '\0')) &&
          (param_1->drawData->horizMoveDelta < 0.0)) {
    param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
  }
  if (((int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7e) <=
       (int)(param_1->destFrameRect).left + (int)(param_1->curFrameP->collisionInset).left) ||
     (0.0 <= param_1->drawData->horizMoveDelta)) {
    if (((int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x82) <
         (int)(param_1->destFrameRect).right - (int)(param_1->curFrameP->collisionInset).right) &&
       (0.0 < param_1->drawData->horizMoveDelta)) {
      param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
    }
  }
  else {
    param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
  }
  _RemoveSpriteIfOutsideRoom(param_1);
  _CheckSpriteWithLavaTiles(param_1,1);
  return;
}


// ---- _SquishEyeballMoveProc @ 0x632fd (1162 bytes)

void SquishEyeballMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  short sStack_16;
  
  sVar6 = *(short *)&param_1->f_17a;
  *(short *)&param_1->f_17a = sVar6 + -1;
  if (sVar6 < 1) {
    param_1->f_17a = '\x01';
    param_1->f_17b = '\0';
    sVar6._0_1_ = param_1->f_178;
    sVar6._1_1_ = param_1->f_179;
    if ((0 < sVar6) || (0.0 <= param_1->drawData->vertMoveDelta)) {
      _SWGetSpriteHeight(param_1);
      if (0.0 < param_1->drawData->vertMoveDelta) {
        sStack_16 = (short)(int)((float)(int)(short)(int)extraout_ST0 -
                                param_1->drawData->vertMoveDelta);
        param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta / 1.5;
        sVar6 = *(short *)&param_1->f_178;
        *(short *)&param_1->f_178 = sVar6 + 1;
        *(short *)(&param_1->f_114 + sVar6 * 2) = sStack_16;
      }
      else {
        *(short *)&param_1->f_178 = *(short *)&param_1->f_178 + -1;
        sVar3._0_1_ = param_1->f_178;
        sVar3._1_1_ = param_1->f_179;
        if (sVar3 < 0) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c"
                     ,PTR____FUNCTION___29983_001c4dd8,0xb3c);
        }
        if (*(short *)(&param_1->f_114 + sVar3 * 2) < 0) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c"
                     ,PTR____FUNCTION___29983_001c4dd8,0xb3d);
        }
        if (999 < *(short *)(&param_1->f_114 + sVar3 * 2)) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c"
                     ,PTR____FUNCTION___29983_001c4dd8,0xb3e);
        }
        sStack_16 = *(short *)(&param_1->f_114 + sVar3 * 2);
      }
      if ((0.0 < param_1->drawData->vertMoveDelta) && (param_1->drawData->vertMoveDelta <= 2.0)) {
        param_1->drawData->vertMoveDelta = -param_1->drawData->vertMoveDelta;
        *(short *)&param_1->f_178 = *(short *)&param_1->f_178 + -1;
      }
      _SWGetSpriteWidth(param_1);
      sVar4._0_1_ = param_1->f_10a;
      sVar4._1_1_ = param_1->f_10b;
      sVar5._0_1_ = param_1->f_108;
      sVar5._1_1_ = param_1->f_109;
      sVar6 = (short)(int)((double)((int)sVar5 - (int)sStack_16) / 1.5 + (double)(int)sVar4);
      _SWSetSpriteScaledHeight(param_1,SUB42((float)(int)sStack_16,0));
      _SWSetSpriteScaledWidth(param_1,SUB42((float)(int)sVar6,0));
      _SWOffsetSprite(param_1,(float)(((int)(short)(int)extraout_ST0_00 - (int)sVar6) / 2),
                      (float)((int)(short)(int)extraout_ST0 - (int)sStack_16));
    }
    else {
      _SWGetSpriteHeight(param_1);
      _SWGetSpriteWidth(param_1);
      param_1->drawData->vertMoveDelta = -*(float *)&param_1->f_104;
      _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c40bc);
      _SWSetSpriteDrawProc(param_1,*(DrawProcPtr_t *)PTR__gSpriteDrawProc_001c47ec);
      sVar1._0_1_ = param_1->f_108;
      sVar1._1_1_ = param_1->f_109;
      _SWSetSpriteScaledHeight(param_1,SUB42((float)(int)sVar1,0));
      sVar2._0_1_ = param_1->f_10a;
      sVar2._1_1_ = param_1->f_10b;
      _SWSetSpriteScaledWidth(param_1,SUB42((float)(int)sVar2,0));
      _SWSetSpriteMoveTime(param_1,0);
      _SWMoveSprite(param_1,*(SWMovementType *)&param_1->f_110,*(SWMovementType *)&param_1->f_10c);
    }
  }
  return;
}


// ---- _BlueRavenMoveProc @ 0x63787 (657 bytes)

void BlueRavenMoveProc(SpritePtr param_1)

{
  char cVar1;
  SWMovementType SVar2;
  
  _SWOffsetSprite(param_1,*(float *)PTR__gCurrentWindDelta_001c4898 +
                          param_1->drawData->horizMoveDelta,0.0);
  if (0.0 < param_1->drawData->horizMoveDelta + *(float *)PTR__gCurrentWindDelta_001c4898) {
    cVar1 = _CheckForWall(param_1,2,&param_1->f_0fc,0,0,1);
    if (cVar1 != '\0') {
      param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
    }
  }
  else {
    cVar1 = _CheckForWall(param_1,4,&param_1->f_0fc,0,0,1);
    if (cVar1 != '\0') {
      param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
    }
  }
  if (((int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7e) <=
       (int)(param_1->destFrameRect).left + (int)(param_1->curFrameP->collisionInset).left) ||
     (0.0 <= param_1->drawData->horizMoveDelta)) {
    if (((int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x82) <
         (int)(param_1->destFrameRect).right - (int)(param_1->curFrameP->collisionInset).right) &&
       (0.0 < param_1->drawData->horizMoveDelta)) {
      param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
    }
  }
  else {
    param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
  }
  SVar2 = _SWGetSpriteHorizMoveDelta(param_1);
  if (0.0 < SVar2) {
    _SWSetSpriteFlipX(param_1,'\x01');
  }
  else {
    _SWSetSpriteFlipX(param_1,'\0');
  }
  _RemoveSpriteIfOutsideRoom(param_1);
  return;
}


// ---- _RedRavenMoveProc @ 0x63a18 (936 bytes)

void RedRavenMoveProc(SpritePtr param_1)

{
  float fVar1;
  double dVar2;
  undefined8 uVar3;
  char cVar4;
  longdouble lVar5;
  SWMovementType SVar6;
  SWMovementType SVar7;
  
  if (*(float *)&param_1->f_108 == -1.0) {
    SVar6 = _SWGetSpriteVertLoc(param_1);
    *(SWMovementType *)&param_1->f_108 = SVar6;
  }
  *(double *)&param_1->f_10c = *(double *)&param_1->f_11c + *(double *)&param_1->f_10c;
  if (360.0 <= *(double *)&param_1->f_10c) {
    *(double *)&param_1->f_10c = *(double *)&param_1->f_10c - 360.0;
  }
  fVar1 = *(float *)&param_1->f_108;
  dVar2 = *(double *)&param_1->f_114;
  uVar3._0_1_ = param_1->f_10c;
  uVar3._1_1_ = param_1->f_10d;
  uVar3._2_1_ = param_1->f_10e;
  uVar3._3_1_ = param_1->f_10f;
  uVar3._4_1_ = param_1->f_110;
  uVar3._5_1_ = param_1->f_111;
  uVar3._6_1_ = param_1->f_112;
  uVar3._7_1_ = param_1->f_113;
  lVar5 = (longdouble)_MySin(uVar3);
  SVar6 = _SWGetSpriteHorizLoc(param_1);
  SVar7 = _SWGetSpriteHorizMoveDelta(param_1);
  _SWMoveSprite(param_1,*(float *)PTR__gCurrentWindDelta_001c4898 + SVar6 + SVar7,
                (float)(dVar2 * (double)lVar5 + (double)fVar1));
  if (param_1->f_124 == '\0') {
    if (0.0 < param_1->drawData->horizMoveDelta + *(float *)PTR__gCurrentWindDelta_001c4898) {
      cVar4 = _CheckForWall(param_1,2,&param_1->f_0fc,0,0,1);
      if (cVar4 != '\0') {
        param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
      }
    }
    else {
      cVar4 = _CheckForWall(param_1,4,&param_1->f_0fc,0,0,1);
      if (cVar4 != '\0') {
        param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
      }
    }
  }
  if (((int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x7e) <=
       (int)(param_1->destFrameRect).left + (int)(param_1->curFrameP->collisionInset).left) ||
     (0.0 <= param_1->drawData->horizMoveDelta)) {
    if (((int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x82) <
         (int)(param_1->destFrameRect).right - (int)(param_1->curFrameP->collisionInset).right) &&
       (0.0 < param_1->drawData->horizMoveDelta)) {
      param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
    }
  }
  else {
    param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
  }
  SVar6 = _SWGetSpriteHorizMoveDelta(param_1);
  if (0.0 < SVar6) {
    _SWSetSpriteFlipX(param_1,'\x01');
  }
  else {
    _SWSetSpriteFlipX(param_1,'\0');
  }
  _RemoveSpriteIfOutsideRoom(param_1);
  return;
}


// ---- _SnakeMoveProc @ 0x63dc0 (1152 bytes)

void SnakeMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  SWBoolean SVar5;
  char cVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  short sStack_12;
  
  uVar3._0_1_ = param_1->f_104;
  uVar3._1_1_ = param_1->f_105;
  uVar3._2_1_ = param_1->f_106;
  uVar3._3_1_ = param_1->f_107;
  uVar4._0_1_ = param_1->f_100;
  uVar4._1_1_ = param_1->f_101;
  uVar4._2_1_ = param_1->f_102;
  uVar4._3_1_ = param_1->f_103;
  _HandleSpriteWithMovingPlatforms(param_1,uVar4,uVar3);
  if (param_1->drawData->vertMoveDelta <= 1.0) {
    param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 2.0;
  }
  else {
    param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 0.325;
  }
  if (8.0 < param_1->drawData->vertMoveDelta) {
    param_1->drawData->vertMoveDelta = 8.0;
  }
  _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
  sStack_12 = 0;
  if (0.0 < param_1->drawData->vertMoveDelta) {
    sVar1._0_1_ = param_1->f_104;
    sVar1._1_1_ = param_1->f_105;
    iVar8 = (int)(param_1->oldFrameRect).bottom - (int)sVar1;
    iVar7 = iVar8 + -1;
    if (iVar7 < 0) {
      iVar7 = iVar8 + 0xe;
    }
    sVar2._0_1_ = param_1->f_104;
    sVar2._1_1_ = param_1->f_105;
    iVar9 = (int)(param_1->destFrameRect).bottom - (int)sVar2;
    iVar8 = iVar9 + -1;
    if (iVar8 < 0) {
      iVar8 = iVar9 + 0xe;
    }
    if (iVar7 >> 4 < iVar8 >> 4) {
      SVar5 = _SWCheckSpriteWithTiles
                        (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,(void *)0x3,
                         (SWRect *)&param_1->f_100,4,4,0x241,0x243,'\0');
      if (SVar5 != '\0') {
        sStack_12 = -*(short *)PTR__gCurConveyorBeltDirection_001c470c;
      }
      SVar5 = _SWCheckSpriteWithTiles
                        (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,(void *)0x3,
                         (SWRect *)&param_1->f_100,4,4,0x249,0x24b,'\0');
      if (SVar5 != '\0') {
        sStack_12 = sStack_12 + *(short *)PTR__gCurConveyorBeltDirection_001c470c;
      }
    }
    cVar6 = _CheckForWall(param_1,3,&param_1->f_100,0,0,1);
    if (cVar6 != '\0') {
      param_1->drawData->vertMoveDelta = 0.0;
    }
  }
  _SWOffsetSprite(param_1,(float)(int)sStack_12 + param_1->drawData->horizMoveDelta,0.0);
  if (0.0 < param_1->drawData->horizMoveDelta + (float)(int)sStack_12) {
    cVar6 = _CheckForWall(param_1,2,&param_1->f_100,0,0,1);
    if ((cVar6 != '\0') && (0.0 < param_1->drawData->horizMoveDelta)) {
      param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
    }
  }
  else if (((param_1->drawData->horizMoveDelta + (float)(int)sStack_12 < 0.0) &&
           (cVar6 = _CheckForWall(param_1,4,&param_1->f_100,0,0,1), cVar6 != '\0')) &&
          (param_1->drawData->horizMoveDelta < 0.0)) {
    param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
  }
  _RemoveSpriteIfOutsideRoom(param_1);
  _CheckSpriteWithLavaTiles(param_1,1);
  return;
}


// ---- _MedusasHeadMoveProc @ 0x64240 (1293 bytes)

void MedusasHeadMoveProc(SpritePtr param_1)

{
  DrawDataRec *pDVar1;
  short sVar2;
  short sVar3;
  SpritePtr srcSpriteP;
  int iVar4;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  longdouble extraout_ST0_01;
  longdouble extraout_ST0_02;
  longdouble lVar5;
  longdouble lVar6;
  SWMovementType SVar7;
  SWMovementType SVar8;
  SWMovementType SVar9;
  SWMovementType SVar10;
  int iStack_40;
  short sStack_26;
  
  sStack_26 = 200;
  SVar7 = _SWGetSpriteHorizLoc(param_1);
  _SWGetSpriteWidth(param_1);
  SVar8 = _SWGetSpriteVertLoc(param_1);
  _SWGetSpriteHeight(param_1);
  srcSpriteP = (SpritePtr)_GetHeroSprite();
  if (srcSpriteP != (SpritePtr)0x0) {
    SVar9 = _SWGetSpriteHorizLoc(srcSpriteP);
    _SWGetSpriteWidth(srcSpriteP);
    SVar10 = _SWGetSpriteVertLoc(srcSpriteP);
    _SWGetSpriteHeight(srcSpriteP);
    sVar2 = (short)(int)((float)extraout_ST0_01 / 2.0 + SVar9) -
            (short)(int)((float)extraout_ST0 / 2.0 + SVar7);
    sVar3 = (short)(int)((float)extraout_ST0_02 / 2.0 + SVar10) -
            (short)(int)((float)extraout_ST0_00 / 2.0 + SVar8);
    sStack_26 = (short)(int)SQRT((double)((int)sVar2 * (int)sVar2 + (int)sVar3 * (int)sVar3));
    lVar5 = (longdouble)_GetAngleFromXandY((double)(int)sVar2,(double)(int)sVar3);
    pDVar1 = param_1->drawData;
    lVar6 = (longdouble)_GetXFromAngleAndVelocity((double)lVar5,(double)*(float *)&param_1->f_10c);
    pDVar1->horizMoveDelta = (float)lVar6;
    pDVar1 = param_1->drawData;
    lVar5 = (longdouble)_GetYFromAngleAndVelocity((double)lVar5,(double)*(float *)&param_1->f_10c);
    pDVar1->vertMoveDelta = (float)lVar5;
    _SWOffsetSprite(param_1,*(float *)PTR__gCurrentWindDelta_001c4898 +
                            param_1->drawData->horizMoveDelta,param_1->drawData->vertMoveDelta);
    if (param_1->f_118 == '\0') {
      if (0.0 < param_1->drawData->horizMoveDelta + *(float *)PTR__gCurrentWindDelta_001c4898) {
        _CheckForWall(param_1,2,&param_1->f_102,0,0,1);
      }
      else {
        _CheckForWall(param_1,4,&param_1->f_102,0,0,1);
      }
      if (0.0 < param_1->drawData->vertMoveDelta) {
        _CheckForWall(param_1,3,&param_1->f_102,0,0,1);
      }
      else {
        _CheckForWall(param_1,1,&param_1->f_102,0,0,1);
      }
    }
  }
  SVar7 = _SWGetSpriteHorizMoveDelta(param_1);
  if (0.0 < SVar7) {
    _SWSetSpriteFlipX(param_1,'\x01');
  }
  else {
    _SWSetSpriteFlipX(param_1,'\0');
  }
  sVar2 = *(short *)&param_1->f_10a;
  *(short *)&param_1->f_10a = sVar2 + 1;
  if (0x59 < sVar2) {
    iVar4 = _GetRandom(1,sStack_26 + 100);
    if (iVar4 == 1) {
      sVar2 = _FindSound(0xb);
      if (sVar2 == -1) {
        sVar2 = _FindSound(0xc);
        if (sVar2 == -1) {
          sVar2 = _FindSound(9);
          if (sVar2 == -1) {
            sVar2 = _GetRandom(0xb,0xc);
            _MMPlayStereoSound(param_1,(int)sVar2,0xb,1);
            param_1->f_10a = '\0';
            param_1->f_10b = '\0';
          }
        }
      }
    }
  }
  if (sStack_26 < 0x1a) {
    iStack_40 = 2;
  }
  else {
    iStack_40 = (int)(sStack_26 / 0xd);
  }
  _MakeSpriteDripBloodOrGoo(param_1,iStack_40,1,0xbe);
  _RemoveSpriteIfOutsideRoom(param_1);
  return;
}


// ---- _MedusaOnFireMoveProc @ 0x6474d (369 bytes)

void MedusaOnFireMoveProc(SpritePtr param_1)

{
  _SWSetSpriteFrameRange(param_1,3,5);
  *(float *)&param_1->f_114 = *(float *)&param_1->f_114 + 0.001;
  *(float *)&param_1->f_110 = *(float *)&param_1->f_114 + *(float *)&param_1->f_110;
  if (param_1->drawData->horizMoveDelta < 0.0) {
    param_1->drawData->horizMoveDelta =
         *(float *)&param_1->f_114 / -4.0 + param_1->drawData->horizMoveDelta;
  }
  else {
    param_1->drawData->horizMoveDelta =
         *(float *)&param_1->f_114 / 4.0 + param_1->drawData->horizMoveDelta;
  }
  _SWOffsetSprite(param_1,*(float *)PTR__gCurrentWindDelta_001c4898 +
                          param_1->drawData->horizMoveDelta,*(SWMovementType *)&param_1->f_110);
  _RemoveSpriteIfOutsideRoom(param_1);
  return;
}


// ---- _SmallSpiderMoveProc @ 0x648be (1491 bytes)

void SmallSpiderMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  short sVar7;
  uchar uVar8;
  char cVar9;
  SWBoolean SVar10;
  longdouble lVar11;
  SWMovementType SVar12;
  SWRect SStack_24;
  SpritePtr pSStack_1c;
  float fStack_18;
  float fStack_14;
  float fStack_10;
  
  pSStack_1c = param_1;
  sVar7._0_1_ = param_1->f_104;
  sVar7._1_1_ = param_1->f_105;
  if (sVar7 == -1) {
    SVar12 = _SWGetSpriteVertLoc(param_1);
    pSStack_1c->f_104 = (char)(short)(int)SVar12;
    pSStack_1c->f_105 = (char)((ushort)(short)(int)SVar12 >> 8);
  }
  sVar1._0_1_ = pSStack_1c->f_108;
  sVar1._1_1_ = pSStack_1c->f_109;
  if (sVar1 == 0) {
    SStack_24.top = 0;
    SStack_24.left = 0;
    SStack_24.bottom = 0;
    SStack_24.right = 0;
    param_1->drawData->vertMoveDelta =
         *(float *)&pSStack_1c->f_120 + param_1->drawData->vertMoveDelta;
    _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
    if (*(float *)&pSStack_1c->f_134 < param_1->drawData->vertMoveDelta) {
      *(float *)&pSStack_1c->f_138 = param_1->drawData->vertMoveDelta;
      pSStack_1c->f_108 = '\x03';
      pSStack_1c->f_109 = '\0';
    }
    cVar9 = _CheckForWall(param_1,3,&pSStack_1c->f_0fc,0,0,1);
    if ((cVar9 != '\0') ||
       (*(short *)(PTR__gRoomMoveBoundsRect_001c4d5c + 4) <= (param_1->destFrameRect).bottom)) {
      pSStack_1c->f_108 = '\x01';
      pSStack_1c->f_109 = '\0';
    }
    _SWOffsetSprite(param_1,0.0,32.0);
    SVar10 = _SWCheckSpriteWithTiles
                       (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,(void *)0x3,
                        &SStack_24,6,6,0x309,0x330,'\0');
    if (SVar10 != '\0') {
      pSStack_1c->f_108 = '\x01';
      pSStack_1c->f_109 = '\0';
    }
    _SWOffsetSprite(param_1,0.0,-32.0);
  }
  else {
    sVar2._0_1_ = pSStack_1c->f_108;
    sVar2._1_1_ = pSStack_1c->f_109;
    if (sVar2 == 3) {
      param_1->drawData->vertMoveDelta =
           param_1->drawData->vertMoveDelta - *(float *)&pSStack_1c->f_120;
      _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
      if (param_1->drawData->vertMoveDelta <
          *(float *)&pSStack_1c->f_130 * -*(float *)&pSStack_1c->f_138) {
        pSStack_1c->f_108 = '\0';
        pSStack_1c->f_109 = '\0';
        fStack_18 = (*(float *)&pSStack_1c->f_128 - *(float *)&pSStack_1c->f_124) / 4.0;
        fStack_14 = *(float *)&pSStack_1c->f_124;
        fStack_10 = *(float *)&pSStack_1c->f_128;
        if (*(float *)&pSStack_1c->f_134 < *(float *)&pSStack_1c->f_124 + fStack_18) {
          fStack_14 = fStack_14 + fStack_18;
        }
        else {
          fStack_10 = fStack_10 - fStack_18;
        }
        lVar11 = (longdouble)_GetMovieCompatibleRandomFloat(fStack_14,fStack_10);
        *(float *)&pSStack_1c->f_134 = (float)lVar11;
      }
      cVar9 = _CheckForWall(param_1,3,&pSStack_1c->f_0fc,0,0,1);
      if (cVar9 != '\0') {
        pSStack_1c->f_108 = '\x01';
        pSStack_1c->f_109 = '\0';
      }
    }
    else {
      sVar3._0_1_ = pSStack_1c->f_108;
      sVar3._1_1_ = pSStack_1c->f_109;
      if (sVar3 == 1) {
        if (-*(float *)&pSStack_1c->f_11c <= param_1->drawData->vertMoveDelta) {
          param_1->drawData->vertMoveDelta =
               *(float *)&pSStack_1c->f_120 * -3.0 + param_1->drawData->vertMoveDelta;
        }
        if (param_1->drawData->vertMoveDelta <= -*(float *)&pSStack_1c->f_11c) {
          param_1->drawData->vertMoveDelta = -*(float *)&pSStack_1c->f_11c;
        }
        _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
        SVar12 = _SWGetSpriteVertLoc(param_1);
        sVar4._0_1_ = pSStack_1c->f_104;
        sVar4._1_1_ = pSStack_1c->f_105;
        if (SVar12 <= (float)(int)sVar4) {
          SVar12 = _SWGetSpriteVertLoc(param_1);
          sVar5._0_1_ = pSStack_1c->f_104;
          sVar5._1_1_ = pSStack_1c->f_105;
          _SWOffsetSprite(param_1,0.0,SVar12 - (float)(int)sVar5);
          pSStack_1c->f_108 = '\x02';
          pSStack_1c->f_109 = '\0';
          uVar8 = pSStack_1c->f_12d;
          pSStack_1c->f_106 = pSStack_1c->f_12c;
          pSStack_1c->f_107 = uVar8;
        }
      }
      else {
        sVar6._0_1_ = pSStack_1c->f_108;
        sVar6._1_1_ = pSStack_1c->f_109;
        if ((sVar6 == 2) &&
           (sVar7 = *(short *)&pSStack_1c->f_106, *(short *)&pSStack_1c->f_106 = sVar7 + -1,
           sVar7 < 1)) {
          pSStack_1c->f_108 = '\0';
          pSStack_1c->f_109 = '\0';
          _SWSetSpriteMoveDelta(param_1,0.0,0.0);
        }
      }
    }
  }
  _LayWebForSpider(param_1,8,4,0x5a0);
  _RemoveSpriteIfOutsideRoom(param_1);
  return;
}


// ---- _BigSpiderMoveProc @ 0x64e91 (1550 bytes)

void BigSpiderMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  short sVar7;
  int iVar8;
  int iVar9;
  undefined4 uVar10;
  bool bVar11;
  char cVar12;
  int iVar13;
  SWMovementType SVar14;
  SpritePtr pSStack_1c;
  SWMovementType SStack_14;
  char cStack_d;
  
  bVar11 = false;
  sVar6._0_1_ = param_1->f_104;
  sVar6._1_1_ = param_1->f_105;
  if (sVar6 == -1) {
    SVar14 = _SWGetSpriteVertLoc(param_1);
    param_1->f_104 = (char)(short)(int)SVar14;
    param_1->f_105 = (char)((ushort)(short)(int)SVar14 >> 8);
  }
  sVar1._0_1_ = param_1->f_108;
  sVar1._1_1_ = param_1->f_109;
  if (sVar1 == 0) {
    if (param_1->f_118 == '\0') {
      _SWOffsetSprite(param_1,0.0,1.8);
    }
    else {
      _SWOffsetSprite(param_1,0.0,2.8);
    }
    cVar12 = _CheckForWall(param_1,3,&param_1->f_0fc,0,0,1);
    if (((cVar12 != '\0') ||
        (cVar12 = _MyCheckSpriteWithWallTiles
                            (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,param_1,3,&param_1->f_0fc,6,
                             6,0x309,0x330,1), cVar12 != '\0')) ||
       (*(short *)(PTR__gRoomMoveBoundsRect_001c4d5c + 4) <= (param_1->destFrameRect).bottom)) {
      param_1->f_108 = '\x01';
      param_1->f_109 = '\0';
      param_1->f_110 = '\n';
      param_1->f_111 = '\0';
      param_1->f_112 = '\n';
      param_1->f_113 = '\0';
    }
  }
  else {
    sVar2._0_1_ = param_1->f_108;
    sVar2._1_1_ = param_1->f_109;
    if (sVar2 == 1) {
      iVar8._0_1_ = param_1->f_10c;
      iVar8._1_1_ = param_1->f_10d;
      iVar8._2_1_ = param_1->f_10e;
      iVar8._3_1_ = param_1->f_10f;
      if (iVar8 == 0) {
        SStack_14 = 0.6;
      }
      else {
        SStack_14 = 1.4;
      }
      _SWOffsetSprite(param_1,0.0,-SStack_14);
      SVar14 = _SWGetSpriteVertLoc(param_1);
      sVar3._0_1_ = param_1->f_104;
      sVar3._1_1_ = param_1->f_105;
      if (SVar14 <= (float)(int)sVar3) {
        SVar14 = _SWGetSpriteVertLoc(param_1);
        sVar4._0_1_ = param_1->f_104;
        sVar4._1_1_ = param_1->f_105;
        _SWOffsetSprite(param_1,0.0,SVar14 - (float)(int)sVar4);
        param_1->f_108 = '\x02';
        param_1->f_109 = '\0';
        param_1->f_106 = '<';
        param_1->f_107 = '\0';
      }
    }
    else {
      sVar5._0_1_ = param_1->f_108;
      sVar5._1_1_ = param_1->f_109;
      if (sVar5 == 2) {
        sVar6 = *(short *)&param_1->f_106;
        *(short *)&param_1->f_106 = sVar6 + -1;
        if (sVar6 < 1) {
          param_1->f_108 = '\x04';
          param_1->f_109 = '\0';
          param_1->f_110 = '\n';
          param_1->f_111 = '\0';
          param_1->f_112 = '\n';
          param_1->f_113 = '\0';
        }
        iVar13._0_1_ = param_1->f_10c;
        iVar13._1_1_ = param_1->f_10d;
        iVar13._2_1_ = param_1->f_10e;
        iVar13._3_1_ = param_1->f_10f;
        if (iVar13 != 0) {
          _MMPlayStereoSound2(0,0x32,0xb,1,(int)*(short *)PTR__gSoundVolume_001c48dc,
                              0x3ff0000000000000,0xffffffff);
          _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_BigSpiderEatsPreyMoveProc_001c4da8);
          if (*(int *)(*(int *)&param_1->f_10c + 0xe8) == 0xc3) {
            _SWSetSpriteMoveProc(*(SpritePtr *)&param_1->f_10c,(MoveProcPtr_t)PTR_LAB_001c4d9c);
          }
          param_1->f_116 = '\0';
          param_1->f_117 = '\0';
        }
      }
      else {
        sVar7._0_1_ = param_1->f_108;
        sVar7._1_1_ = param_1->f_109;
        if (sVar7 == 4) {
          cStack_d = '\0';
          iVar13 = _GetHeroSprite();
          if (iVar13 != 0) {
            cStack_d = _CheckBigSpiderWithSprite(param_1,iVar13,0x32);
          }
          if (cStack_d == '\0') {
            pSStack_1c = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gBirdSpriteLayerP_001c41c4,
                                          (SpritePtr)0x0);
            while ((pSStack_1c != (SpritePtr)0x0 && (cStack_d == '\0'))) {
              if ((pSStack_1c->userData != 4) && (pSStack_1c->userData != 5)) {
                (**(code **)PTR__gSWAssertFailProc_001c4170)
                          ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c"
                           ,PTR____FUNCTION___30633_001c4db0,0xd9e);
              }
              cStack_d = _CheckBigSpiderWithSprite(param_1,pSStack_1c,0x5a);
              if (cStack_d != '\0') {
                bVar11 = true;
              }
              pSStack_1c = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gBirdSpriteLayerP_001c41c4,
                                            pSStack_1c);
            }
          }
          if (cStack_d == '\0') {
            pSStack_1c = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gMedusaSpriteLayerP_001c4094,
                                          (SpritePtr)0x0);
            while ((pSStack_1c != (SpritePtr)0x0 && (cStack_d == '\0'))) {
              cStack_d = _CheckBigSpiderWithSprite(param_1,pSStack_1c,0x14);
              pSStack_1c = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gMedusaSpriteLayerP_001c4094,
                                            pSStack_1c);
            }
          }
          if (cStack_d == '\0') {
            pSStack_1c = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gEyeballSpriteLayerP_001c41c8,
                                          (SpritePtr)0x0);
            while ((pSStack_1c != (SpritePtr)0x0 && (cStack_d == '\0'))) {
              cStack_d = _CheckBigSpiderWithSprite(param_1,pSStack_1c,0x4b);
              pSStack_1c = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gEyeballSpriteLayerP_001c41c8,
                                            pSStack_1c);
            }
          }
          if (cStack_d != '\0') {
            if (bVar11) {
              param_1->f_118 = '\x01';
            }
            else {
              param_1->f_118 = '\0';
            }
            param_1->f_108 = '\0';
            param_1->f_109 = '\0';
            param_1->f_110 = '\x06';
            param_1->f_111 = '\0';
            param_1->f_112 = '\x06';
            param_1->f_113 = '\0';
          }
        }
      }
    }
  }
  _LayWebForSpider(param_1,0x18,10,0x5a1);
  iVar9._0_1_ = param_1->f_10c;
  iVar9._1_1_ = param_1->f_10d;
  iVar9._2_1_ = param_1->f_10e;
  iVar9._3_1_ = param_1->f_10f;
  if (iVar9 != 0) {
    uVar10._0_1_ = param_1->f_10c;
    uVar10._1_1_ = param_1->f_10d;
    uVar10._2_1_ = param_1->f_10e;
    uVar10._3_1_ = param_1->f_10f;
    _MovePreyToSpiderLocation(uVar10,param_1);
  }
  _RemoveSpriteIfOutsideRoom(param_1);
  return;
}


// ---- _SkeletonFishMoveProc @ 0x6549f (788 bytes)

void SkeletonFishMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  short sVar7;
  uchar uVar8;
  SWMovementType SVar9;
  float fStack_28;
  
  sVar1._0_1_ = param_1->f_108;
  sVar1._1_1_ = param_1->f_109;
  if (sVar1 < 1) {
    param_1->drawData->vertMoveDelta = *(float *)&param_1->f_110 + param_1->drawData->vertMoveDelta;
    _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
    if ((param_1->drawData->vertMoveDelta < 0.0) && (param_1->f_114 == '\0')) {
      if (param_1->drawData->vertMoveDelta < 0.0) {
        fStack_28 = -param_1->drawData->vertMoveDelta;
      }
      else {
        fStack_28 = param_1->drawData->vertMoveDelta;
      }
      if (fStack_28 / *(float *)&param_1->f_110 < 17.0) {
        sVar3._0_1_ = param_1->f_0fe;
        sVar3._1_1_ = param_1->f_0ff;
        sVar4._0_1_ = param_1->f_0fe;
        sVar4._1_1_ = param_1->f_0ff;
        _SWSetSpriteFrameRange(param_1,sVar4 + 2,sVar3 + 7);
        sVar5._0_1_ = param_1->f_0fe;
        sVar5._1_1_ = param_1->f_0ff;
        _SWSetCurrentFrameIndex(param_1,sVar5 + 2);
        _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_FishTurnAroundFrameProc_001c4dac);
        _SWSetSpriteFrameAdvance(param_1,0);
        param_1->f_104 = '\x01';
        param_1->f_105 = '\0';
        param_1->f_100 = '\x04';
        param_1->f_101 = '\0';
        param_1->f_102 = '\x04';
        param_1->f_103 = '\0';
        param_1->f_114 = '\x01';
      }
    }
    SVar9 = _SWGetSpriteVertLoc(param_1);
    sVar6._0_1_ = param_1->f_0fc;
    sVar6._1_1_ = param_1->f_0fd;
    if ((float)(int)sVar6 <= SVar9) {
      SVar9 = _SWGetSpriteHorizLoc(param_1);
      sVar7._0_1_ = param_1->f_0fc;
      sVar7._1_1_ = param_1->f_0fd;
      _SWMoveSprite(param_1,(float)(int)(short)(int)SVar9,(float)(int)sVar7);
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)0x0);
      _SWSetSpriteFrameRange(param_1,0,1);
      uVar8 = param_1->f_107;
      param_1->f_108 = param_1->f_106;
      param_1->f_109 = uVar8;
      param_1->f_114 = '\0';
      _MMSetTempStereoFadeDist(300);
      _MMPlayStereoSoundAdjustedVol(param_1,0x41,0xb,1,0x15e,0x3fe3333333333333);
    }
  }
  else {
    *(short *)&param_1->f_108 = *(short *)&param_1->f_108 + -1;
    sVar2._0_1_ = param_1->f_108;
    sVar2._1_1_ = param_1->f_109;
    if (sVar2 == 0) {
      _StartSkeletonFishJump(param_1);
    }
  }
  return;
}


// ---- _SkeletonWaitingToUncollapseMoveProc @ 0x657b3 (254 bytes)

void SkeletonWaitingToUncollapseMoveProc(int param_1)

{
  short sVar1;
  int iVar2;
  
  if (*(short *)(param_1 + 0x10e) == 1) {
    sVar1 = *(short *)(param_1 + 0x118);
    *(short *)(param_1 + 0x118) = sVar1 + -1;
    if (sVar1 < 1) {
      _MakeSkeletonDoUncollapse(param_1);
    }
  }
  else if (*(short *)(param_1 + 0x10e) == 2) {
    iVar2 = _GetHeroSprite();
    if (iVar2 != 0) {
      sVar1 = _GetDistBetweenSprites(param_1,iVar2);
      if (sVar1 < *(short *)(param_1 + 0x112)) {
        _MakeSkeletonDoUncollapse(param_1);
      }
    }
  }
  else if (*(short *)(param_1 + 0x10e) == 3) {
    iVar2 = _GetHeroSprite();
    if (iVar2 != 0) {
      sVar1 = _GetDistBetweenSprites(param_1,iVar2);
      if (sVar1 < *(short *)(param_1 + 0x112)) {
        *(undefined2 *)(param_1 + 0x10e) = 1;
      }
    }
  }
  return;
}


// ---- _WalkingSkeletonMoveProc @ 0x658b1 (1546 bytes)

void WalkingSkeletonMoveProc(SpritePtr param_1)

{
  float fVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  SWBoolean SVar4;
  char cVar5;
  short sVar6;
  short sVar7;
  int iVar8;
  SpritePtr srcSpriteP;
  int iVar9;
  int iVar10;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SWMovementType SVar11;
  short sStack_12;
  
  uVar2._0_1_ = param_1->f_106;
  uVar2._1_1_ = param_1->f_107;
  uVar2._2_1_ = param_1->f_108;
  uVar2._3_1_ = param_1->f_109;
  uVar3._0_1_ = param_1->f_102;
  uVar3._1_1_ = param_1->f_103;
  uVar3._2_1_ = param_1->f_104;
  uVar3._3_1_ = param_1->f_105;
  _HandleSpriteWithMovingPlatforms(param_1,uVar3,uVar2);
  if (param_1->drawData->vertMoveDelta <= 1.0) {
    param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 1.0;
  }
  else {
    param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 0.43333334;
  }
  if (8.0 < param_1->drawData->vertMoveDelta) {
    param_1->drawData->vertMoveDelta = 8.0;
  }
  _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
  sStack_12 = 0;
  if (0.0 < param_1->drawData->vertMoveDelta) {
    sVar6._0_1_ = param_1->f_106;
    sVar6._1_1_ = param_1->f_107;
    iVar9 = (int)(param_1->oldFrameRect).bottom - (int)sVar6;
    iVar8 = iVar9 + -1;
    if (iVar8 < 0) {
      iVar8 = iVar9 + 0xe;
    }
    sVar7._0_1_ = param_1->f_106;
    sVar7._1_1_ = param_1->f_107;
    iVar10 = (int)(param_1->destFrameRect).bottom - (int)sVar7;
    iVar9 = iVar10 + -1;
    if (iVar9 < 0) {
      iVar9 = iVar10 + 0xe;
    }
    if (iVar8 >> 4 < iVar9 >> 4) {
      SVar4 = _SWCheckSpriteWithTiles
                        (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,(void *)0x3,
                         (SWRect *)&param_1->f_102,4,4,0x241,0x243,'\0');
      if (SVar4 != '\0') {
        sStack_12 = -*(short *)PTR__gCurConveyorBeltDirection_001c470c;
      }
      SVar4 = _SWCheckSpriteWithTiles
                        (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,(void *)0x3,
                         (SWRect *)&param_1->f_102,4,4,0x249,0x24b,'\0');
      if (SVar4 != '\0') {
        sStack_12 = sStack_12 + *(short *)PTR__gCurConveyorBeltDirection_001c470c;
      }
    }
    cVar5 = _CheckForWall(param_1,3,&param_1->f_102,0,0,1);
    if (cVar5 != '\0') {
      param_1->drawData->vertMoveDelta = 0.0;
    }
  }
  srcSpriteP = (SpritePtr)_GetHeroSprite();
  if (srcSpriteP == (SpritePtr)0x0) {
    param_1->drawData->horizMoveDelta = 0.0;
  }
  else {
    SVar11 = _SWGetSpriteHorizLoc(param_1);
    _SWGetSpriteWidth(param_1);
    sVar6 = (short)(int)((float)extraout_ST0 / 2.0 + SVar11);
    SVar11 = _SWGetSpriteHorizLoc(srcSpriteP);
    _SWGetSpriteWidth(srcSpriteP);
    sVar7 = (short)(int)((float)extraout_ST0_00 / 2.0 + SVar11);
    if ((float)(int)sVar7 + *(float *)&param_1->f_114 < (float)(int)sVar6) {
      param_1->drawData->horizMoveDelta = -*(float *)&param_1->f_114;
      param_1->f_10a = '\0';
    }
    else if ((float)(int)sVar6 < (float)(int)sVar7 - *(float *)&param_1->f_114) {
      param_1->drawData->horizMoveDelta = *(float *)&param_1->f_114;
      param_1->f_10a = '\x01';
    }
    else {
      param_1->drawData->horizMoveDelta = 0.0;
    }
  }
  _SWOffsetSprite(param_1,(float)(int)sStack_12 + param_1->drawData->horizMoveDelta,0.0);
  if (0.0 < param_1->drawData->horizMoveDelta + (float)(int)sStack_12) {
    _CheckForWall(param_1,2,&param_1->f_102,0,0,1);
  }
  else if (param_1->drawData->horizMoveDelta + (float)(int)sStack_12 < 0.0) {
    _CheckForWall(param_1,4,&param_1->f_102,0,0,1);
  }
  fVar1 = param_1->drawData->horizMoveDelta;
  if ((fVar1 != 0.0) || (NAN(fVar1))) {
    _SWSetSpriteFrameRange(param_1,10,0xd);
    _SWSetSpriteFlipX(param_1,0.0 < param_1->drawData->horizMoveDelta);
  }
  else {
    _SWSetSpriteFrameRange(param_1,8,8);
    _SWSetSpriteFlipX(param_1,param_1->f_10a);
  }
  _RemoveSpriteIfOutsideRoom(param_1);
  _CheckSpriteWithLavaTiles(param_1,1);
  return;
}


// ---- _HandleSpriteWithMovingPlatforms @ 0x65ebb (289 bytes)

undefined4 _HandleSpriteWithMovingPlatforms(SpritePtr param_1,undefined4 param_2,short param_3)

{
  char cVar1;
  undefined4 local_20;
  int local_14;
  int local_10;
  
  param_3 = param_3 + -1;
  cVar1 = _SWCheckSpriteWithSprites
                    (param_1,&local_14,3,&param_2,
                     *(undefined4 *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,
                     *(undefined4 *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c,1,1,0,0);
  if (cVar1 == '\0') {
    local_20 = 0;
  }
  else {
    if (*(char *)(local_14 + 0x32) == '\0') {
      local_10 = local_14;
      _SWOffsetSprite(param_1,*(SWMovementType *)(local_14 + 0x110),
                      *(SWMovementType *)(local_14 + 0x114));
    }
    else {
      _SWOffsetSprite(param_1,(float)((int)*(short *)(local_14 + 0x38) -
                                     (int)*(short *)(local_14 + 0x40)),
                      (float)((int)*(short *)(local_14 + 0x36) - (int)*(short *)(local_14 + 0x3e)));
    }
    local_20 = 1;
  }
  return local_20;
}


// ---- _FallingObjectMoveProc @ 0x65fdc (1263 bytes)

void FallingObjectMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  SWBoolean SVar5;
  char cVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  float fStack_10;
  
  uVar3._0_1_ = param_1->f_100;
  uVar3._1_1_ = param_1->f_101;
  uVar3._2_1_ = param_1->f_102;
  uVar3._3_1_ = param_1->f_103;
  uVar4._0_1_ = param_1->f_0fc;
  uVar4._1_1_ = param_1->f_0fd;
  uVar4._2_1_ = param_1->f_0fe;
  uVar4._3_1_ = param_1->f_0ff;
  _HandleSpriteWithMovingPlatforms(param_1,uVar4,uVar3);
  if (param_1->drawData->vertMoveDelta <= 1.0) {
    param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 1.0;
  }
  else {
    param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 0.43333334;
  }
  if (8.0 < param_1->drawData->vertMoveDelta) {
    param_1->drawData->vertMoveDelta = 8.0;
  }
  _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
  fStack_10 = 0.0;
  if (0.0 < param_1->drawData->vertMoveDelta) {
    sVar1._0_1_ = param_1->f_100;
    sVar1._1_1_ = param_1->f_101;
    iVar8 = (int)(param_1->oldFrameRect).bottom - (int)sVar1;
    iVar7 = iVar8 + -1;
    if (iVar7 < 0) {
      iVar7 = iVar8 + 0xe;
    }
    sVar2._0_1_ = param_1->f_100;
    sVar2._1_1_ = param_1->f_101;
    iVar9 = (int)(param_1->destFrameRect).bottom - (int)sVar2;
    iVar8 = iVar9 + -1;
    if (iVar8 < 0) {
      iVar8 = iVar9 + 0xe;
    }
    if (iVar7 >> 4 < iVar8 >> 4) {
      SVar5 = _SWCheckSpriteWithTiles
                        (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,(void *)0x3,
                         (SWRect *)&param_1->f_0fc,4,4,0x241,0x243,'\0');
      if (SVar5 != '\0') {
        fStack_10 = 0.0 - (float)(int)*(short *)PTR__gCurConveyorBeltDirection_001c470c;
      }
      SVar5 = _SWCheckSpriteWithTiles
                        (*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,param_1,(void *)0x3,
                         (SWRect *)&param_1->f_0fc,4,4,0x249,0x24b,'\0');
      if (SVar5 != '\0') {
        fStack_10 = fStack_10 + (float)(int)*(short *)PTR__gCurConveyorBeltDirection_001c470c;
      }
    }
    cVar6 = _CheckForWall(param_1,3,&param_1->f_0fc,0,0,1);
    if (cVar6 != '\0') {
      if (((param_1->userData == 0x1e) || (param_1->userData == 0x1f)) &&
         (3.9 <= param_1->drawData->vertMoveDelta)) {
        _MMPlayStereoSound(param_1,0x1a,0xc,1);
      }
      param_1->drawData->vertMoveDelta = 0.0;
    }
  }
  _SWOffsetSprite(param_1,param_1->drawData->horizMoveDelta + fStack_10,0.0);
  if (0.0 < param_1->drawData->horizMoveDelta + fStack_10) {
    cVar6 = _CheckForWall(param_1,2,&param_1->f_0fc,0,0,1);
    if ((cVar6 != '\0') && (0.0 < param_1->drawData->horizMoveDelta)) {
      param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
    }
  }
  else if (((param_1->drawData->horizMoveDelta + fStack_10 < 0.0) &&
           (cVar6 = _CheckForWall(param_1,4,&param_1->f_0fc,0,0,1), cVar6 != '\0')) &&
          (param_1->drawData->horizMoveDelta < 0.0)) {
    param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
  }
  _RemoveSpriteIfOutsideRoom(param_1);
  _CheckSpriteWithLavaTiles(param_1,0);
  return;
}


// ---- _ZzzzSpriteMoveProc @ 0x664cb (57 bytes)

void ZzzzSpriteMoveProc(SpritePtr param_1)

{
  _SWOffsetSprite(param_1,param_1->drawData->horizMoveDelta,param_1->drawData->vertMoveDelta);
  return;
}


// ---- _SmallSpiderDragsHeroMoveProc @ 0x66504 (525 bytes)

void SmallSpiderDragsHeroMoveProc(SpritePtr param_1)

{
  SpritePtr srcSpriteP;
  longdouble extraout_ST0;
  SWMovementType SVar1;
  
  srcSpriteP = *(SpritePtr *)&param_1->f_10c;
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___31162_001c4d84,0xf7f);
  }
  if (-*(float *)&param_1->f_11c <= param_1->drawData->vertMoveDelta) {
    param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta - *(float *)&param_1->f_120;
  }
  param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta - 0.017;
  _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
  SVar1 = _SWGetSpriteVertLoc(srcSpriteP);
  _SWGetSpriteHeight(srcSpriteP);
  if (SVar1 + (float)extraout_ST0 < -30.0) {
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4dbc);
    param_1->f_13c = '\0';
    param_1->f_13d = '\0';
    param_1->f_13e = '\0';
    param_1->f_13f = '\0';
  }
  _SWCenterSpriteOverSprite(srcSpriteP,param_1);
  _SWOffsetSprite(srcSpriteP,0.0,40.0);
  if (0.0 < param_1->drawData->vertMoveDelta) {
    _LayWebForSpider(param_1,8,4,0x5a0);
  }
  _EraseWebForSpider(param_1,8,4,0x5a0);
  return;
}


// ---- _SmallSpiderEatJackMoveProc @ 0x66711 (301 bytes)

void SmallSpiderEatJackMoveProc(SpritePtr param_1)

{
  SpritePtr spriteP;
  int iVar1;
  int iVar2;
  int iVar3;
  int iStack_20;
  
  spriteP = *(SpritePtr *)&param_1->f_10c;
  if (spriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___31203_001c4d74,0xfae);
  }
  iVar1._0_1_ = param_1->f_13c;
  iVar1._1_1_ = param_1->f_13d;
  iVar1._2_1_ = param_1->f_13e;
  iVar1._3_1_ = param_1->f_13f;
  if (iVar1 < 0x12) {
    iStack_20 = 2;
  }
  else {
    iStack_20 = (int)(short)(*(int *)&param_1->f_13c / 9);
  }
  _MakeSpriteDripBloodOrGoo(spriteP,iStack_20,1,0xbc);
  *(int *)&param_1->f_13c = *(int *)&param_1->f_13c + 1;
  iVar2._0_1_ = param_1->f_13c;
  iVar2._1_1_ = param_1->f_13d;
  iVar2._2_1_ = param_1->f_13e;
  iVar2._3_1_ = param_1->f_13f;
  if (iVar2 == 0x5a) {
    *PTR__gRestartLevel_001c4858 = 1;
  }
  iVar3._0_1_ = param_1->f_13c;
  iVar3._1_1_ = param_1->f_13d;
  iVar3._2_1_ = param_1->f_13e;
  iVar3._3_1_ = param_1->f_13f;
  if (0x9f < iVar3) {
    _SWRemoveSpriteFromAnimation(param_1,'\x01');
    _SWRemoveSpriteFromAnimation(spriteP,'\x01');
  }
  return;
}


// ---- _BigSpiderEatsPreyMoveProc @ 0x6683e (186 bytes)

void BigSpiderEatsPreyMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  
  *(short *)&param_1->f_116 = *(short *)&param_1->f_116 + 1;
  sVar1._0_1_ = param_1->f_116;
  sVar1._1_1_ = param_1->f_117;
  if ((sVar1 == 0xa0) && (*(int *)(*(int *)&param_1->f_10c + 0xe8) == 0xc3)) {
    *PTR__gRestartLevel_001c4858 = 1;
  }
  sVar2._0_1_ = param_1->f_116;
  sVar2._1_1_ = param_1->f_117;
  if (0xef < sVar2) {
    _StopMySound(0x32);
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)0x0);
    param_1->f_110 = '\x0f';
    param_1->f_111 = '\0';
    param_1->f_112 = '\x0f';
    param_1->f_113 = '\0';
  }
  return;
}


// ---- _ShadowSpriteMoveProc @ 0x668f8 (285 bytes)

void ShadowSpriteMoveProc(SpriteRec *param_1)

{
  SpritePtr srcSpriteP;
  SpritePtr srcSpriteP_00;
  longdouble lVar1;
  SWMovementType SVar2;
  SWMovementType SVar3;
  SWMovementType SVar4;
  SWMovementType SVar5;
  float fStack_10;
  
  srcSpriteP = param_1->mountParentP;
  srcSpriteP_00 = (SpritePtr)_GetHeroSprite();
  if ((srcSpriteP != (SpritePtr)0x0) && (srcSpriteP_00 != (SpritePtr)0x0)) {
    SVar2 = _SWGetSpriteHorizLoc(srcSpriteP);
    SVar3 = _SWGetSpriteHorizLoc(srcSpriteP_00);
    lVar1 = (longdouble)_floatAbs(SVar2 - SVar3);
    fStack_10 = (float)lVar1;
    if (fStack_10 < 90.0) {
      fStack_10 = 90.0;
    }
    SVar4 = _SWGetSpriteVertLoc(srcSpriteP);
    SVar5 = _SWGetSpriteVertLoc(srcSpriteP_00);
    _SWSetSpriteMountOffset(param_1,(SVar2 - SVar3) / 20.0,(SVar4 - SVar5) / (fStack_10 / 8.0));
  }
  return;
}


// ---- _floatAbs @ 0x66a15 (82 bytes)

longdouble _floatAbs(float param_1)

{
  undefined4 local_10;
  
  if (0.0 < param_1) {
    local_10 = param_1;
  }
  else {
    local_10 = -param_1;
  }
  return (longdouble)local_10;
}


// ---- _ArrowButtonColorMoveProc @ 0x66a67 (281 bytes)

void ArrowButtonColorMoveProc(SpritePtr param_1)

{
  int iVar1;
  uint uStack_10;
  
  uStack_10 = (uint)param_1->drawData->colorBlue;
  iVar1 = param_1->misc2;
  param_1->misc2 = iVar1 + 1;
  if ((0x5a < iVar1) || (uStack_10 != 0xffff)) {
    param_1->misc2 = 0;
    if (param_1->misc1 < 0) {
      uStack_10 = uStack_10 + 0x8ff;
      if (0xffff < uStack_10) {
        uStack_10 = 0xffff;
        param_1->misc1 = 1;
      }
    }
    else {
      uStack_10 = uStack_10 - 0x8ff;
      if ((int)uStack_10 < 0x4000) {
        uStack_10 = 0x4000;
        param_1->misc1 = -1;
      }
    }
    if (param_1->curFrameIndex == 0) {
      _SWSetSpriteColor(param_1,0xffff,(ushort)uStack_10,(ushort)uStack_10);
    }
  }
  if (0 < param_1->curFrameIndex) {
    _SWSetSpriteColor(param_1,0xffff,0xffff,0xffff);
  }
  return;
}


// ---- _StartSkeletonFishJump @ 0x66b80 (358 bytes)

void _StartSkeletonFishJump(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  int iVar3;
  
  iVar3 = _GetRandom(0,100);
  if (iVar3 < 0x33) {
    param_1->f_0fe = '\0';
    param_1->f_0ff = '\0';
  }
  else {
    param_1->f_0fe = '\t';
    param_1->f_0ff = '\0';
  }
  _SWSetSpriteMoveDelta(param_1,0.0,-*(SWMovementType *)&param_1->f_10c);
  sVar1._0_1_ = param_1->f_0fe;
  sVar1._1_1_ = param_1->f_0ff;
  sVar2._0_1_ = param_1->f_0fe;
  sVar2._1_1_ = param_1->f_0ff;
  _SWSetSpriteFrameRange(param_1,(int)sVar2,sVar1 + 1);
  _SWSetCurrentFrameIndex(param_1,0);
  param_1->f_108 = '\0';
  param_1->f_109 = '\0';
  _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_SkeletonFishFrameProc_001c4c64);
  _SWSetSpriteFrameAdvance(param_1,0);
  param_1->f_104 = '\x01';
  param_1->f_105 = '\0';
  param_1->f_100 = '\x06';
  param_1->f_101 = '\0';
  param_1->f_102 = '\x06';
  param_1->f_103 = '\0';
  _MMSetTempStereoFadeDist(300);
  _MMPlayStereoSoundAdjustedVol(param_1,0x41,0xb,1,0x15e,0x3ff0000000000000);
  return;
}


// ---- _BloodDripMoveProc @ 0x66ce6 (565 bytes)

void BloodDripMoveProc(SpritePtr param_1)

{
  char cVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  int iVar5;
  SpritePtr pSStack_14;
  
  if ((param_1->userData != 0xbc) && (param_1->userData != 0xbe)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___31327_001c4de8,0x107a);
  }
  param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 0.1;
  if (4.0 <= param_1->drawData->vertMoveDelta) {
    param_1->drawData->vertMoveDelta = 4.0;
  }
  _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
  cVar1 = _CheckForWall(param_1,3,&param_1->f_0fc,0,0,1);
  if (cVar1 != '\0') {
    _SWRemoveSpriteFromAnimation(param_1,'\x01');
    if (param_1->userData == 0xbc) {
      pSStack_14 = (SpritePtr)_NewBloodOrGooSplashSprite(0xbd);
    }
    else {
      pSStack_14 = (SpritePtr)_NewBloodOrGooSplashSprite(0xbf);
    }
    _SWCenterSpriteOverSprite(pSStack_14,param_1);
    iVar5 = _GetRandom(2,6);
    _SWOffsetSprite(pSStack_14,0.0,(float)-iVar5);
    if (param_1->f_105 != '\0') {
      sVar2 = _GetRandom(0xe,0xf);
      sVar3 = _CountSounds(0xe);
      sVar4 = _CountSounds(0xf);
      if ((int)sVar3 + (int)sVar4 < 3) {
        _MMPlayStereoSound(param_1,(int)sVar2,0xb,1);
      }
    }
  }
  _RemoveSpriteIfOutsideRoom(param_1);
  return;
}


// ---- _CaughtHeroMoveProc @ 0x66f1b (234 bytes)

void CaughtHeroMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  
  if (param_1->userData != 0xc3) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___31379_001c4db4,0x10ab);
  }
  sVar1._0_1_ = param_1->f_104;
  sVar1._1_1_ = param_1->f_105;
  _MakeSpriteDripBloodOrGoo(param_1,(int)sVar1,0,0xbc);
  sVar1 = *(short *)&param_1->f_106;
  *(short *)&param_1->f_106 = sVar1 + 1;
  if (5 < sVar1) {
    *(short *)&param_1->f_104 = *(short *)&param_1->f_104 + 1;
    param_1->f_106 = '\0';
    param_1->f_107 = '\0';
    sVar2._0_1_ = param_1->f_104;
    sVar2._1_1_ = param_1->f_105;
    if (0x45 < sVar2) {
      _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)0x0);
    }
  }
  return;
}


// ---- _ColorStatueMoveProc @ 0x67005 (352 bytes)

void ColorStatueMoveProc(SpritePtr param_1)

{
  int iVar1;
  SpritePtr pSVar2;
  
  if (param_1->userData != 0xc2) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___31400_001c4dcc,0x10c5);
  }
  if (*PTR__gStatuesAreDoneFading_001c4934 != '\0') {
    pSVar2 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gTopCircleOfLightSpriteLayerP_001c4014,
                              (SpritePtr)0x0);
    if (pSVar2 == (SpritePtr)0x0) {
      pSVar2 = _SWGetNextSprite(*(SpriteLayerPtr *)PTR__gBottomCircleOfLightSpriteLayerP_001c4290,
                                (SpritePtr)0x0);
      if (pSVar2 == (SpritePtr)0x0) {
        _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)0x0);
        _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_PauseThenUnfreezeFrameProc_001c4cf0);
        _SWSetSpriteFrameAdvance(param_1,0);
        _SWSetSpriteFrameTime(param_1,0);
        param_1->f_104 = '\x1e';
        param_1->f_105 = '\0';
        iVar1._0_1_ = param_1->f_114;
        iVar1._1_1_ = param_1->f_115;
        iVar1._2_1_ = param_1->f_116;
        iVar1._3_1_ = param_1->f_117;
        if (iVar1 == 0) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c"
                     ,PTR____FUNCTION___31400_001c4dcc,0x10db);
        }
        _SWSetCurrentFrameIndex(*(SpritePtr *)&param_1->f_114,1);
      }
    }
  }
  return;
}


// ---- _CreateCirclesOfLightMoveProc @ 0x67165 (662 bytes)

void CreateCirclesOfLightMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  int iVar3;
  short sVar4;
  SpritePtr srcSpriteP;
  SpritePtr srcSpriteP_00;
  longdouble extraout_ST0;
  
  if (param_1->userData != 0xc2) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___31431_001c4db8,0x10ef);
  }
  if (*PTR__gStatuesAreDoneFading_001c4934 == '\0') {
    iVar3 = *(int *)&param_1->f_108;
    *(int *)&param_1->f_108 = iVar3 + -1;
    if (iVar3 < 1) {
      sVar1 = (param_1->destFrameRect).left;
      sVar4 = (param_1->destFrameRect).bottom + -5;
      srcSpriteP = (SpritePtr)_NewCircleOfLightSprite(0xc6);
      _SWSetSpriteLocation(srcSpriteP,(float)(int)sVar1,(float)(int)sVar4);
      sVar2._0_1_ = param_1->f_10c;
      sVar2._1_1_ = param_1->f_10d;
      _SWSetCurrentFrameIndex(srcSpriteP,(int)sVar2);
      _SWSetSpriteTranslucencyLevel(srcSpriteP,0);
      *(short *)&srcSpriteP->f_10c = sVar4;
      _SWSetSpriteBlendMode(srcSpriteP,0x302,1);
      _SWGetSpriteHeight(srcSpriteP);
      srcSpriteP_00 = (SpritePtr)_NewCircleOfLightSprite(0xc5);
      _SWSetSpriteLocation
                (srcSpriteP_00,(float)(int)sVar1,(float)((int)sVar4 - (int)(short)(int)extraout_ST0)
                );
      sVar1._0_1_ = param_1->f_10c;
      sVar1._1_1_ = param_1->f_10d;
      _SWSetCurrentFrameIndex(srcSpriteP_00,(int)sVar1);
      _SWSetSpriteTranslucencyLevel(srcSpriteP_00,0);
      *(short *)&srcSpriteP_00->f_10c = sVar4 - (short)(int)extraout_ST0;
      _SWSetSpriteBlendMode(srcSpriteP_00,0x302,1);
      *(SpritePtr *)&srcSpriteP->f_108 = srcSpriteP_00;
      *(SpritePtr *)&srcSpriteP_00->f_108 = srcSpriteP;
      *(SpritePtr *)&srcSpriteP->f_104 = param_1;
      *(SpritePtr *)&srcSpriteP_00->f_104 = param_1;
      srcSpriteP_00->f_110 = 0xcd;
      srcSpriteP_00->f_111 = 0xcc;
      srcSpriteP_00->f_112 = 'L';
      srcSpriteP_00->f_113 = '>';
      srcSpriteP->f_110 = 0xcd;
      srcSpriteP->f_111 = 0xcc;
      srcSpriteP->f_112 = 'L';
      srcSpriteP->f_113 = '>';
      *(short *)&param_1->f_10c = *(short *)&param_1->f_10c + 1;
      sVar4._0_1_ = param_1->f_10c;
      sVar4._1_1_ = param_1->f_10d;
      if (srcSpriteP_00->maxFrames <= (int)sVar4) {
        param_1->f_10c = '\0';
        param_1->f_10d = '\0';
      }
      param_1->f_108 = '\x14';
      param_1->f_109 = '\0';
      param_1->f_10a = '\0';
      param_1->f_10b = '\0';
    }
  }
  else {
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)0x0);
  }
  return;
}


// ---- _CircleOfLightMoveProc @ 0x673fb (524 bytes)

void CircleOfLightMoveProc(SpritePtr param_1)

{
  int iVar1;
  uint level;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SWMovementType SVar2;
  short sStack_18;
  
  if (param_1->userData == 0xc6) {
    _SWGetSpriteHeight(param_1);
    SVar2 = _SWGetSpriteVertLoc(param_1);
    sStack_18 = (short)(int)SVar2;
    SVar2 = _SWGetSpriteVertLoc(*(SpritePtr *)&param_1->f_104);
    _SWGetSpriteHeight(*(SpritePtr *)&param_1->f_104);
    if ((int)sStack_18 < (int)(short)(int)SVar2 + (int)(short)((short)(int)extraout_ST0_00 / 2)) {
      sStack_18 = sStack_18 - (*(short *)&param_1->f_10c - (short)(int)extraout_ST0_00);
    }
    else {
      sStack_18 = *(short *)&param_1->f_10c - sStack_18;
    }
    if (sStack_18 <= (short)(int)extraout_ST0) {
      if (sStack_18 < 0) {
        iVar1._0_1_ = param_1->f_108;
        iVar1._1_1_ = param_1->f_109;
        iVar1._2_1_ = param_1->f_10a;
        iVar1._3_1_ = param_1->f_10b;
        if (iVar1 == 0) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c"
                     ,PTR____FUNCTION___31486_001c4d98,0x1142);
        }
        _SWRemoveSpriteFromAnimation(param_1,'\x01');
        _SWRemoveSpriteFromAnimation(*(SpritePtr *)&param_1->f_108,'\x01');
      }
      else {
        level = (int)sStack_18 * (int)(0xffff / (longlong)(int)(short)(int)extraout_ST0);
        _SWSetSpriteTranslucencyLevel(param_1,level);
        _SWSetSpriteTranslucencyLevel(*(SpritePtr *)&param_1->f_108,level);
      }
    }
  }
  *(float *)&param_1->f_110 = *(float *)&param_1->f_110 + 0.01;
  _SWOffsetSprite(param_1,0.0,-*(SWMovementType *)&param_1->f_110);
  return;
}


// ---- _CreateSparklesMoveProc @ 0x67607 (295 bytes)

void CreateSparklesMoveProc(undefined4 *param_1)

{
  char cVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  SpritePtr newSpriteP;
  undefined2 uStack_e;
  
  iVar4 = param_1[0x3a] + -0xb1;
  if (iVar4 == 1) {
    uStack_e = 0xf;
    goto LAB_0006763c;
  }
  if (iVar4 < 2) {
    if (param_1[0x3a] == 0xb1) {
      uStack_e = 0x19;
      goto LAB_0006763c;
    }
  }
  else {
    if (iVar4 == 2) {
      uStack_e = 9;
      goto LAB_0006763c;
    }
    if (iVar4 == 3) {
      uStack_e = 2;
      goto LAB_0006763c;
    }
  }
  uStack_e = 1;
LAB_0006763c:
  iVar4 = _GetRandom(1,uStack_e);
  if (iVar4 == 1) {
    newSpriteP = (SpritePtr)_NewGoldSparkleSprite();
    _SWAddSprite((SpriteLayerPtr)*param_1,newSpriteP);
    do {
      sVar2 = _GetRandom((int)*(short *)(param_1 + 0xe),*(short *)(param_1 + 0xf) + -6);
      sVar3 = _GetRandom((int)*(short *)((int)param_1 + 0x36),*(short *)((int)param_1 + 0x3a) + -6);
      _SWSetSpriteLocation(newSpriteP,(float)(int)sVar2,(float)(int)sVar3);
      cVar1 = _SWBestMaskCollision(newSpriteP,param_1);
    } while (cVar1 == '\0');
  }
  return;
}


// ---- _BigWebMoveProc @ 0x6772e (344 bytes)

void BigWebMoveProc(SpritePtr param_1)

{
  char cVar1;
  int iVar2;
  undefined1 auStack_18 [8];
  SpritePtr pSStack_10;
  
  pSStack_10 = param_1;
  if (param_1->userData != 0x74) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___31575_001c4ddc,0x1185);
  }
  _SWSetRect(auStack_18,10,0,10,0);
  iVar2 = _SWGetSpriteFrameTime(param_1);
  if (iVar2 == -1) {
    cVar1 = _SWCheckSpriteWithSprites
                      (param_1,0,0,auStack_18,*(undefined4 *)PTR__gHeroSpriteLayerP_001c4164,
                       *(undefined4 *)PTR__gHeroSpriteLayerP_001c4164,0,0,0,0);
    if (cVar1 == '\0') {
      pSStack_10->f_0fc = '\x01';
    }
    else if (pSStack_10->f_0fc != '\0') {
      _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_LAB_001c4d88);
      _SWSetSpriteFrameTime(param_1,0x19);
      _SWSetSpriteFrameAdvance(param_1,1);
      pSStack_10->f_0fc = '\0';
    }
  }
  return;
}


// ---- _PedestalMoveProc @ 0x67886 (255 bytes)

void PedestalMoveProc(int param_1)

{
  char cVar1;
  undefined1 auStack_18 [8];
  int iStack_10;
  
  iStack_10 = param_1;
  if (*(int *)(param_1 + 0xe8) != 0x65) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___31597_001c4df4,0x11a9);
  }
  if (*(char *)(iStack_10 + 0x104) == '\0') {
    _SWSetRect(auStack_18,0,0,0,0);
    cVar1 = _SWCheckSpriteWithSprites
                      (param_1,0,0,auStack_18,*(undefined4 *)PTR__gHeroSpriteLayerP_001c4164,
                       *(undefined4 *)PTR__gHeroSpriteLayerP_001c4164,0,0,0,0);
    if (cVar1 == '\0') {
      *(undefined1 *)(iStack_10 + 0x104) = 1;
    }
  }
  return;
}


// ---- _TipSpriteMoveProc @ 0x67985 (1019 bytes)

void TipSpriteMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  int iVar3;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SWMovementType SVar4;
  SWMovementType SVar5;
  undefined4 uStack_24;
  undefined4 uStack_20;
  short sStack_1c;
  short sStack_1a;
  short sStack_18;
  short sStack_16;
  SpritePtr pSStack_14;
  SpritePtr pSStack_10;
  
  pSStack_14 = (SpritePtr)_GetHeroSprite();
  pSStack_10 = (SpritePtr)param_1->misc2;
  if (pSStack_10 == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___31620_001c4dd4,0x11c6);
  }
  if (pSStack_14 != (SpritePtr)0x0) {
    SVar4 = _SWGetSpriteHorizLoc(pSStack_14);
    SVar5 = _SWGetSpriteHorizLoc(pSStack_10);
    sStack_1c = _mAbs((int)(SVar4 - SVar5));
    SVar4 = _SWGetSpriteVertLoc(pSStack_14);
    SVar5 = _SWGetSpriteVertLoc(pSStack_10);
    sStack_1a = _mAbs((int)(SVar4 - SVar5));
    if (60.0 < SQRT((double)((int)sStack_1c * (int)sStack_1c + (int)sStack_1a * (int)sStack_1a))) {
      _SWRemoveSpriteFromAnimation(param_1,'\x01');
    }
    if (param_1->misc1 == 0) {
      uStack_20._0_2_ = (param_1->destFrameRect).bottom;
      uStack_20._2_2_ = (param_1->destFrameRect).right;
      uStack_24._0_2_ = (param_1->destFrameRect).top;
      uStack_24._2_2_ = (param_1->destFrameRect).left;
      _SWCenterRect(&uStack_24,&pSStack_10->destFrameRect);
      iVar3 = (int)(short)uStack_24;
      sVar1 = (param_1->destFrameRect).bottom;
      sVar2 = (param_1->destFrameRect).top;
      _SWGetSpriteHeight(pSStack_10);
      _SWMoveSprite(param_1,(float)(int)uStack_24._2_2_,
                    (float)(iVar3 - ((int)sVar1 - (int)sVar2)) + (float)extraout_ST0 / -2.0);
      SVar4 = _SWGetSpriteVertLoc(param_1);
      sStack_16 = (short)(int)((float)((int)(param_1->destFrameRect).bottom -
                                      (int)(param_1->destFrameRect).top) + SVar4);
      SVar4 = _SWGetSpriteHorizLoc(param_1);
      sStack_18 = (short)(int)((float)((int)(param_1->destFrameRect).right -
                                      (int)(param_1->destFrameRect).left) + SVar4);
      SVar4 = _SWGetSpriteVertLoc(param_1);
      if (SVar4 < 0.0) {
        SVar4 = _SWGetSpriteVertLoc(pSStack_10);
        _SWGetSpriteHeight(pSStack_10);
        _SWMoveSprite(param_1,(float)(int)uStack_24._2_2_,SVar4 + (float)extraout_ST0_00 + 5.0);
      }
      if (*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x80) < sStack_16) {
        SVar4 = _SWGetSpriteVertLoc(pSStack_10);
        _SWMoveSprite(param_1,(float)(int)uStack_24._2_2_,
                      (SVar4 - (float)((int)(param_1->destFrameRect).bottom -
                                      (int)(param_1->destFrameRect).top)) - 5.0);
      }
      SVar4 = _SWGetSpriteHorizLoc(param_1);
      if (SVar4 < 0.0) {
        SVar4 = _SWGetSpriteHorizLoc(param_1);
        _SWOffsetSprite(param_1,15.0 - SVar4,0.0);
      }
      else if (*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x82) <= sStack_18) {
        _SWOffsetSprite(param_1,(float)(((int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x82)
                                        - (int)sStack_18) + -0x14),0.0);
      }
      _SWSetSpriteTranslucency(param_1,0xffff);
      param_1->misc1 = 1;
    }
  }
  return;
}


// ---- _StartHereBounceMoveProc @ 0x67d80 (177 bytes)

void StartHereBounceMoveProc(SpritePtr param_1)

{
  float fVar1;
  longdouble lVar2;
  SWMovementType horizLoc;
  
  param_1->misc1 = param_1->misc1 + 5;
  if (0x168 < param_1->misc1) {
    param_1->misc1 = param_1->misc1 + -0x168;
  }
  fVar1 = param_1->miscFloat;
  lVar2 = (longdouble)_SWCos((double)param_1->misc1);
  horizLoc = _SWGetSpriteHorizLoc(param_1);
  _SWMoveSprite(param_1,horizLoc,fVar1 + (float)lVar2);
  return;
}


// ---- _MovingPlatformHorizMoveProc @ 0x67e31 (850 bytes)

void MovingPlatformHorizMoveProc(SpritePtr param_1)

{
  char cVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  undefined4 uStack_14;
  
  if (param_1->f_104 != '\0') {
    _SWOffsetSprite(param_1,param_1->drawData->horizMoveDelta,0.0);
    *(float *)&param_1->f_110 = param_1->drawData->horizMoveDelta;
    param_1->f_114 = '\0';
    param_1->f_115 = '\0';
    param_1->f_116 = '\0';
    param_1->f_117 = '\0';
    if (0.0 < param_1->drawData->horizMoveDelta) {
      uStack_14 = 2;
    }
    else {
      uStack_14 = 4;
    }
    cVar1 = _CheckForWall(param_1,uStack_14,&param_1->f_0fc,0,0,1);
    cVar2 = _SWCheckSpriteWithSprites
                      (param_1,0,uStack_14,&param_1->f_0fc,
                       *(undefined4 *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,
                       *(undefined4 *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,0,1,0,1);
    cVar3 = _SWCheckSpriteWithSprites
                      (param_1,0,uStack_14,&param_1->f_0fc,
                       *(undefined4 *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c,
                       *(undefined4 *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c,0,1,0,1);
    cVar4 = _SWCheckSpriteWithSprites
                      (param_1,0,uStack_14,&param_1->f_0fc,
                       *(undefined4 *)PTR__gMovingPlatformBlockerSpriteLayerP_001c41ac,
                       *(undefined4 *)PTR__gMovingPlatformBlockerSpriteLayerP_001c41ac,0,1,0,1);
    if ((((cVar1 != '\0') || (cVar2 != '\0')) || (cVar3 != '\0')) || (cVar4 != '\0')) {
      param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
    }
    if (((int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xa6) <=
         (int)(param_1->destFrameRect).left + (int)(param_1->curFrameP->collisionInset).left) ||
       (0.0 <= param_1->drawData->horizMoveDelta)) {
      if (((int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xaa) <
           (int)(param_1->destFrameRect).right - (int)(param_1->curFrameP->collisionInset).right) &&
         (0.0 < param_1->drawData->horizMoveDelta)) {
        param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
      }
    }
    else {
      param_1->drawData->horizMoveDelta = -param_1->drawData->horizMoveDelta;
    }
    _RemoveSpriteIfOutsideRoom(param_1);
  }
  return;
}


// ---- _MovingPlatformVertMoveProc @ 0x68183 (850 bytes)

void MovingPlatformVertMoveProc(SpritePtr param_1)

{
  char cVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  undefined4 uStack_14;
  
  if (param_1->f_104 != '\0') {
    _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
    param_1->f_110 = '\0';
    param_1->f_111 = '\0';
    param_1->f_112 = '\0';
    param_1->f_113 = '\0';
    *(float *)&param_1->f_114 = param_1->drawData->vertMoveDelta;
    if (0.0 < param_1->drawData->vertMoveDelta) {
      uStack_14 = 3;
    }
    else {
      uStack_14 = 1;
    }
    cVar1 = _CheckForWall(param_1,uStack_14,&param_1->f_0fc,0,0,1);
    cVar2 = _SWCheckSpriteWithSprites
                      (param_1,0,uStack_14,&param_1->f_0fc,
                       *(undefined4 *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,
                       *(undefined4 *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,0,1,0,1);
    cVar3 = _SWCheckSpriteWithSprites
                      (param_1,0,uStack_14,&param_1->f_0fc,
                       *(undefined4 *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c,
                       *(undefined4 *)PTR__gHorizMovingPlatformSpriteLayerP_001c486c,0,1,0,1);
    cVar4 = _SWCheckSpriteWithSprites
                      (param_1,0,uStack_14,&param_1->f_0fc,
                       *(undefined4 *)PTR__gMovingPlatformBlockerSpriteLayerP_001c41ac,
                       *(undefined4 *)PTR__gMovingPlatformBlockerSpriteLayerP_001c41ac,0,1,0,1);
    if ((((cVar1 != '\0') || (cVar2 != '\0')) || (cVar3 != '\0')) || (cVar4 != '\0')) {
      param_1->drawData->vertMoveDelta = -param_1->drawData->vertMoveDelta;
    }
    if (((int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xa4) <=
         (int)(param_1->destFrameRect).top + (int)(param_1->curFrameP->collisionInset).top) ||
       (0.0 <= param_1->drawData->vertMoveDelta)) {
      if (((int)*(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0xa8) <
           (int)(param_1->destFrameRect).bottom - (int)(param_1->curFrameP->collisionInset).bottom)
         && (0.0 < param_1->drawData->vertMoveDelta)) {
        param_1->drawData->vertMoveDelta = -param_1->drawData->vertMoveDelta;
      }
    }
    else {
      param_1->drawData->vertMoveDelta = -param_1->drawData->vertMoveDelta;
    }
    _RemoveSpriteIfOutsideRoom(param_1);
  }
  return;
}


// ---- _FallingPlatformMoveProc @ 0x684d5 (524 bytes)

void FallingPlatformMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  SWBoolean SVar3;
  SWMovementType SVar4;
  SWRect SStack_20;
  SpritePtr pSStack_18;
  int iStack_14;
  int iStack_10;
  
  pSStack_18 = param_1;
  sVar1._0_1_ = param_1->f_10a;
  sVar1._1_1_ = param_1->f_10b;
  if (sVar1 < 1) {
    if (param_1->parentSpriteLayerP !=
        *(SpriteLayerRec **)PTR__gFallingPlatformSpriteLayerP_001c499c) {
      _SWRemoveSprite(param_1);
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gFallingPlatformSpriteLayerP_001c499c,param_1);
    }
    sVar2._0_1_ = pSStack_18->f_106;
    sVar2._1_1_ = pSStack_18->f_107;
    if (sVar2 == -1) {
      SVar4 = _SWGetSpriteVertLoc(param_1);
      pSStack_18->f_106 = (char)(short)(int)SVar4;
      pSStack_18->f_107 = (char)((ushort)(short)(int)SVar4 >> 8);
    }
    param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 0.1;
    _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
    if ((0x5b < param_1->userData) && (param_1->userData < 0x5f)) {
      iStack_14 = _GetHeroSprite();
      if ((iStack_14 != 0) &&
         ((iStack_10 = iStack_14, *(SpritePtr *)(iStack_14 + 0x1c4) != param_1 &&
          (param_1->frameAdvance == 1)))) {
        _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_LAB_001c4d00);
        _SWSetSpriteFrameAdvance(param_1,-1);
        _SWSetSpriteFrameTime(param_1,100);
      }
    }
    SStack_20._4_4_ = *(undefined4 *)(PTR__gRoomMoveBoundsRect_001c4d5c + 4);
    SStack_20._0_4_ = *(undefined4 *)PTR__gRoomMoveBoundsRect_001c4d5c;
    SVar3 = _SWIsSpriteInRect(param_1,&SStack_20);
    if (SVar3 == '\0') {
      if ((param_1->userData < 0x5c) || (0x5e < param_1->userData)) {
        _MakeFallingPlatformReappearAfterDelay(param_1);
      }
      else {
        _SWRemoveSpriteFromAnimation(param_1,'\x01');
      }
    }
    else {
      _CheckSpriteWithLavaTiles(param_1,1);
    }
  }
  else {
    *(short *)&param_1->f_10a = *(short *)&param_1->f_10a + -1;
  }
  return;
}


// ---- _MakeFallingPlatformReappearAfterDelay @ 0x686e1 (160 bytes)

void _MakeFallingPlatformReappearAfterDelay(SpritePtr param_1)

{
  short sVar1;
  SWMovementType SVar2;
  
  sVar1 = *(short *)(PTR__gRoomMoveBoundsRect_001c4d5c + 4);
  SVar2 = _SWGetSpriteVertLoc(param_1);
  _SWOffsetSprite(param_1,0.0,(float)(int)sVar1 - SVar2);
  _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)PTR_LAB_001c4de4);
  _SWSetSpriteVisible(param_1,'\0');
  param_1->f_108 = 0x87;
  param_1->f_109 = '\0';
  return;
}


// ---- _FallingPlatformWaitMoveProc @ 0x68781 (501 bytes)

void FallingPlatformWaitMoveProc(SpritePtr param_1)

{
  short sVar1;
  SWError SVar2;
  SWMovementType SVar3;
  
  param_1->drawData->vertMoveDelta = param_1->drawData->vertMoveDelta + 0.1;
  _SWOffsetSprite(param_1,0.0,param_1->drawData->vertMoveDelta);
  sVar1 = *(short *)&param_1->f_108;
  *(short *)&param_1->f_108 = sVar1 + -1;
  if (sVar1 < 1) {
    SVar3 = _SWGetSpriteHorizLoc(param_1);
    sVar1._0_1_ = param_1->f_106;
    sVar1._1_1_ = param_1->f_107;
    _SWSetSpriteLocation(param_1,(float)(int)(short)(int)SVar3,(float)(int)sVar1);
    _SWRemoveSprite(param_1);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,param_1);
    _SWCustomSortSpriteLayer
              (*(SpriteLayerPtr *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,
               (SpriteCompareProcPtr_t)PTR_CompareSpriteLeft_001c4d8c);
    _SWSetSpriteMoveDelta(param_1,0.0,0.0);
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)0x0);
    _SWSetSpriteVisible(param_1,'\x01');
    _SWSetSpriteFrameProc(param_1,(FrameProcPtr_t)PTR_FadeInFrameProc_001c4c04);
    _SWSetSpriteFrameTime(param_1,0x10);
    _SWSetSpriteTranslucencyLevel(param_1,0);
    SVar2 = _SWSetSpriteDrawProc(param_1,*(DrawProcPtr_t *)PTR__gTranslucentMaskDrawProc_001c411c);
    if (SVar2 != 0) {
      _MMFatalErrorCallback
                (SVar2,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
                 PTR____FUNCTION___31981_001c4dc8,0x1378);
    }
  }
  return;
}


// ---- _DoorSpriteMoveProc @ 0x68976 (775 bytes)

void DoorSpriteMoveProc(SpritePtr param_1)

{
  short bottomInset;
  short sVar1;
  short sVar2;
  
  if (param_1->userData < 0x2d) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___32010_001c4df8,5000);
  }
  if (0x44 < param_1->userData) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___32010_001c4df8,0x1389);
  }
  *(double *)&param_1->f_108 = *(double *)&param_1->f_110 + *(double *)&param_1->f_108;
  *(double *)&param_1->f_110 = *(double *)&param_1->f_118 + *(double *)&param_1->f_110;
  *(double *)&param_1->f_118 = *(double *)&param_1->f_118 + 0.003;
  sVar2 = (short)(int)*(double *)&param_1->f_108;
  *(double *)&param_1->f_108 = *(double *)&param_1->f_108 - (double)(int)sVar2;
  if (param_1->f_105 == '\0') {
    (param_1->destFrameRect).top = (param_1->destFrameRect).top + sVar2;
    *(short *)&param_1->f_0fe = *(short *)&param_1->f_0fe + sVar2;
  }
  else {
    (param_1->destFrameRect).bottom = (param_1->destFrameRect).bottom - sVar2;
    *(short *)&param_1->f_0fc = *(short *)&param_1->f_0fc - sVar2;
    bottomInset._0_1_ = param_1->f_0fc;
    bottomInset._1_1_ = param_1->f_0fd;
    _SWSetFrameCollisionInset(param_1->curFrameP,0,0,0,bottomInset);
  }
  param_1->needsToBeDrawn = '\x01';
  *(short *)&param_1->f_102 = *(short *)&param_1->f_102 - sVar2;
  sVar2._0_1_ = param_1->f_102;
  sVar2._1_1_ = param_1->f_103;
  if (sVar2 < 1) {
    sVar1._0_1_ = param_1->f_102;
    sVar1._1_1_ = param_1->f_103;
    if (sVar1 < 0) {
      sVar2 = *(short *)&param_1->f_102;
      if (param_1->f_105 == '\0') {
        (param_1->destFrameRect).top = (param_1->destFrameRect).top + sVar2;
        *(short *)&param_1->f_0fe = *(short *)&param_1->f_0fe + sVar2;
      }
      else {
        (param_1->destFrameRect).bottom = (param_1->destFrameRect).bottom - sVar2;
        *(short *)&param_1->f_0fc = *(short *)&param_1->f_0fc - sVar2;
      }
    }
    _SWRemoveSprite(param_1);
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gOpenedDoorSpriteLayerP_001c49a4,param_1);
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)0x0);
    _SWSetFrameCollisionInset(param_1->curFrameP,0,0,0,0);
    param_1->f_104 = '\x01';
  }
  return;
}


// ---- _CloseDoorSpriteMoveProc @ 0x68c7d (771 bytes)

void CloseDoorSpriteMoveProc(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  
  if (param_1->userData < 0x2d) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___32073_001c4d78,0x13cf);
  }
  if (0x44 < param_1->userData) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/SpriteMoveProcs.c",
               PTR____FUNCTION___32073_001c4d78,0x13d0);
  }
  *(double *)&param_1->f_108 = *(double *)&param_1->f_110 + *(double *)&param_1->f_108;
  *(double *)&param_1->f_110 = *(double *)&param_1->f_118 + *(double *)&param_1->f_110;
  *(double *)&param_1->f_118 = *(double *)&param_1->f_118 + 0.003;
  sVar2 = (short)(int)*(double *)&param_1->f_108;
  *(double *)&param_1->f_108 = *(double *)&param_1->f_108 - (double)(int)sVar2;
  sVar1 = -sVar2;
  if (param_1->f_105 == '\0') {
    (param_1->destFrameRect).top = (param_1->destFrameRect).top + sVar1;
    *(short *)&param_1->f_0fe = *(short *)&param_1->f_0fe + sVar1;
  }
  else {
    (param_1->destFrameRect).bottom = (param_1->destFrameRect).bottom + sVar2;
    *(short *)&param_1->f_0fc = *(short *)&param_1->f_0fc + sVar2;
    _SWSetFrameCollisionInset(param_1->curFrameP,0,0,0,-8 - *(short *)&param_1->f_102);
  }
  param_1->needsToBeDrawn = '\x01';
  *(short *)&param_1->f_102 = *(short *)&param_1->f_102 + sVar1;
  sVar1._0_1_ = param_1->f_102;
  sVar1._1_1_ = param_1->f_103;
  if (sVar1 < 1) {
    sVar2._0_1_ = param_1->f_102;
    sVar2._1_1_ = param_1->f_103;
    if (sVar2 < 0) {
      sVar1 = *(short *)&param_1->f_102;
      if (param_1->f_105 == '\0') {
        (param_1->destFrameRect).top = (param_1->destFrameRect).top + -sVar1;
        *(short *)&param_1->f_0fe = *(short *)&param_1->f_0fe + -sVar1;
      }
      else {
        (param_1->destFrameRect).bottom = (param_1->destFrameRect).bottom + sVar1;
        *(short *)&param_1->f_0fc = *(short *)&param_1->f_0fc + sVar1;
      }
    }
    _SWSetFrameCollisionInset(param_1->curFrameP,0,0,0,0);
    _SWSetSpriteMoveProc(param_1,(MoveProcPtr_t)0x0);
    param_1->f_104 = '\0';
    _InitializeDoorMovementVariables(param_1);
  }
  return;
}


// ---- _SmallDigitsMoveProc @ 0x68f80 (153 bytes)

void _SmallDigitsMoveProc(int param_1)

{
  short sVar1;
  undefined4 local_10;
  
  if (*(short *)(param_1 + 0xfc) < 5) {
    local_10 = (int)*(short *)PTR__gCurrentFPSValue_001c4728;
  }
  else if (*(short *)(param_1 + 0xfc) < 7) {
    sVar1 = _GEGetLevelResID((int)*(short *)PTR__gCurrentLevel_001c49bc,1,200);
    local_10 = (int)sVar1;
  }
  else {
    local_10 = *(int *)PTR__gNumSecretsFound_001c4990;
  }
  _SetStatsSpriteNumber(param_1,local_10);
  return;
}


// ---- _CustomCursorMoveProc @ 0x69019 (163 bytes)

void _CustomCursorMoveProc(SpritePtr param_1)

{
  short local_10;
  short local_e;
  
  _MMGetMouse(&local_10);
  _SWConvertCoordinates(*(undefined4 *)PTR__gSpriteWorldP_001c40d0,&local_10);
  if ((local_e == 0) && (local_10 == 0)) {
    _SWMoveSprite(param_1,10000.0,10000.0);
  }
  else {
    _SWMoveSprite(param_1,(float)(local_e + -4),(float)(local_10 + -4));
  }
  return;
}


// ---- _StdSpriteMoveProc @ 0x690bc (57 bytes)

void StdSpriteMoveProc(SpritePtr param_1)

{
  _SWOffsetSprite(param_1,param_1->drawData->horizMoveDelta,param_1->drawData->vertMoveDelta);
  return;
}


