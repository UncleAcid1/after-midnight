// Ghidra decompilation of functions from original source file:
//   Sources & Headers/MMSounds.c

// ---- _MMSetTempStereoFadeDist @ 0x9c37b (42 bytes)

void _MMSetTempStereoFadeDist(undefined2 param_1)

{
  *(undefined2 *)PTR__gTempStereoFadeDist_001c52cc = param_1;
  return;
}


// ---- _MMPlaySound @ 0x9c3a5 (118 bytes)

void _MMPlaySound(short param_1,short param_2,undefined4 param_3)

{
  _MMPlayStereoSound2(0,(int)param_1,(int)param_2,param_3,(int)*(short *)PTR__gSoundVolume_001c48dc,
                      0x3ff0000000000000,0);
  return;
}


// ---- _MMPlayStereoSound @ 0x9c41b (117 bytes)

void _MMPlayStereoSound(undefined4 param_1,short param_2,short param_3,undefined4 param_4)

{
  _MMPlayStereoSound2(param_1,(int)param_2,(int)param_3,param_4,
                      (int)*(short *)PTR__gSoundVolume_001c48dc,0x3ff0000000000000,0);
  return;
}


// ---- _MMPlayStereoSound2 @ 0x9c490 (304 bytes)

int _MMPlayStereoSound2(int param_1,short param_2,short param_3,undefined4 param_4,short param_5,
                       undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  short sVar1;
  int local_48;
  undefined1 local_2a [8];
  short local_22;
  int local_20;
  
  if (*PTR__gSoundIsTurnedOn_001c47fc == '\0') {
    local_48 = 0;
  }
  else {
    if (param_1 == 0) {
      local_22 = 0;
    }
    else {
      local_20 = _GetHeroSprite();
      if (local_20 == 0) {
        local_22 = 0;
      }
      else {
        _SWSetRect(local_2a,0,0,(int)*(short *)PTR__gTempStereoFadeDist_001c52cc,0x1e0);
        *(undefined2 *)PTR__gTempStereoFadeDist_001c52cc = 0x9c4;
        _SWCenterRect(local_2a,local_20 + 0x36);
        local_22 = _GetStereoPositionOfSprite(param_1,local_2a);
      }
    }
    sVar1 = _PlaySoundDetailed((int)param_2,(int)param_3,param_4,(int)param_5,(int)local_22,
                               CONCAT44(param_7,param_6),param_8);
    local_48 = (int)sVar1;
  }
  return local_48;
}


// ---- _MMPlaySoundDetailed @ 0x9c5c0 (161 bytes)

int _MMPlaySoundDetailed
              (short param_1,short param_2,undefined4 param_3,short param_4,short param_5,
              undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  short sVar1;
  undefined4 local_38;
  
  if (*PTR__gSoundIsTurnedOn_001c47fc == '\0') {
    local_38 = 0;
  }
  else {
    sVar1 = _PlaySoundDetailed((int)param_1,(int)param_2,param_3,(int)param_4,(int)param_5,
                               CONCAT44(param_7,param_6),param_8);
    local_38 = (int)sVar1;
  }
  return local_38;
}


// ---- _GetStereoPositionOfSprite @ 0x9c661 (84 bytes)

int _GetStereoPositionOfSprite(int param_1,undefined4 param_2)

{
  short sVar1;
  
  sVar1 = _GetStereoPositionOfColumn
                    ((int)(short)(*(short *)(param_1 + 0x38) +
                                 (short)(((int)*(short *)(param_1 + 0x3c) -
                                         (int)*(short *)(param_1 + 0x38)) / 2)),param_2);
  return (int)sVar1;
}


// ---- _GetStereoPositionOfColumn @ 0x9c6b5 (172 bytes)

/* WARNING: Restarted to delay deadcode elimination for space: stack */

int _GetStereoPositionOfColumn(int param_1,undefined4 *param_2)

{
  short sStack_1a;
  short sStack_16;
  
  sStack_16 = (short)((uint)param_2[1] >> 0x10);
  sStack_1a = (short)((uint)*param_2 >> 0x10);
  if (sStack_1a != 0) {
    sStack_16 = sStack_16 - sStack_1a;
    param_1 = param_1 - sStack_1a;
  }
  if (param_1 < 0) {
    param_1 = 0;
  }
  else if (sStack_16 < param_1) {
    param_1 = (int)sStack_16;
  }
  return (int)(short)(((param_1 - sStack_16 / 2) * 0x100) / (int)(sStack_16 / 2));
}


// ---- _MMPlayStereoSoundAdjustedVol @ 0x9c761 (309 bytes)

undefined4
_MMPlayStereoSoundAdjustedVol
          (int param_1,short param_2,short param_3,undefined4 param_4,short param_5,
          undefined4 param_6,undefined4 param_7)

{
  short sVar1;
  undefined4 local_38;
  undefined2 local_16;
  undefined4 local_14;
  undefined4 local_10;
  
  local_10 = 0;
  if (*PTR__gSoundIsTurnedOn_001c47fc == '\0') {
    local_38 = 0;
  }
  else {
    local_14 = (SpritePtr)_GetHeroSprite();
    if (local_14 == (SpritePtr)0x0) {
      local_14 = _SWGetPreviousSprite
                           (*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,(SpritePtr)0x0);
    }
    if ((local_14 == (SpritePtr)0x0) || (param_1 == 0)) {
      local_16 = 0;
    }
    else {
      sVar1 = _GetDistBetweenSprites(local_14,param_1);
      local_16 = (short)(((int)*(short *)PTR__gSoundVolume_001c48dc * ((int)param_5 - (int)sVar1)) /
                        (int)param_5);
    }
    if (5 < local_16) {
      local_10 = _MMPlayStereoSound2(param_1,(int)param_2,(int)param_3,param_4,(int)local_16,
                                     CONCAT44(param_7,param_6),0);
    }
    local_38 = local_10;
  }
  return local_38;
}


// ---- _MMUpdateChannelInfoForSpriteLayer @ 0x9c896 (1005 bytes)

void _MMUpdateChannelInfoForSpriteLayer
               (SpriteLayerPtr param_1,short param_2,short param_3,short param_4,short param_5,
               short param_6)

{
  char cVar1;
  short sVar2;
  short local_76;
  int local_74;
  int local_6c;
  int local_64;
  undefined1 local_46 [8];
  short local_3e;
  short local_3c;
  short local_3a;
  float local_38;
  float local_34;
  float local_30;
  float local_2c;
  SpritePtr local_28;
  SpritePtr local_24;
  short local_20;
  short local_1e;
  
  local_34 = 0.0;
  local_3a = 0;
  if ((*PTR__gSoundIsTurnedOn_001c47fc != '\0') && (*PTR__gAudioWasInitialized_001c52d0 != '\0')) {
    if (param_1->isPaused == 0) {
      local_38 = 0.0;
      local_30 = 0.0;
      local_2c = 0.0;
      local_24 = (SpritePtr)_GetHeroSprite();
      if (local_24 == (SpritePtr)0x0) {
        local_24 = _SWGetPreviousSprite
                             (*(SpriteLayerPtr *)PTR__gDeadHeroSpriteLayerP_001c497c,(SpritePtr)0x0)
        ;
      }
      if (local_24 != (SpritePtr)0x0) {
        for (local_28 = _SWGetNextSprite(param_1,(SpritePtr)0x0); local_28 != (SpritePtr)0x0;
            local_28 = _SWGetNextSprite(param_1,local_28)) {
          if (local_28->isVisible != '\0') {
            local_3e = _GetDistBetweenSprites(local_24,local_28);
            local_3e = local_3e + -0xf;
            if (local_3e < 0) {
              local_3e = 0;
            }
            local_3c = (short)(((int)param_4 * ((int)param_6 - (int)local_3e)) / (int)param_6);
            _SWSetRect(local_46,0,0,(int)param_5,0x1e0);
            _SWCenterRect(local_46,&local_24->destFrameRect);
            sVar2 = _GetStereoPositionOfSprite(local_28,local_46);
            local_38 = (float)(int)sVar2;
            local_76 = local_3a;
            if (local_3a < local_3c) {
              local_76 = local_3c;
            }
            local_3a = local_76;
            local_74 = (int)param_6 - (int)local_3e;
            if (local_74 < 1) {
              local_74 = 1;
            }
            local_30 = (float)local_74 / (float)(int)param_6;
            local_2c = local_2c + local_30;
            local_34 = local_34 + local_38 * local_30;
          }
        }
      }
      local_34 = local_34 / local_2c;
      if (local_3a < 1) {
        if (local_3a < 1) {
          cVar1 = _IsChannelAvailable((int)param_3);
          if ((cVar1 == '\0') &&
             (param_1 == *(SpriteLayerPtr *)PTR__gZapperBeamSpriteLayerP_001c41ec)) {
            _StopChannel((int)param_3);
          }
          local_3a = 0;
        }
      }
      else {
        cVar1 = _IsChannelAvailable((int)param_3);
        if (cVar1 != '\0') {
          _MMPlayStereoSound2(0,(int)param_2,(int)param_3,2,(int)local_3a,0x3ff0000000000000,
                              0xffffffff);
        }
      }
    }
    else {
      local_3a = 0;
    }
    local_3a = (short)(int)((float)(int)local_3a * 0.7);
    _SetVolumeAndStereoPosition((int)param_3,(int)local_3a,(int)(short)(int)local_34);
    cVar1 = _IsChannelAvailable((int)param_3);
    if (cVar1 == '\0') {
      local_20 = _GetChannelVolume((int)param_3);
      if (local_3a < local_20) {
        local_6c = (int)local_3a;
        if ((int)local_3a < local_20 + -5) {
          local_6c = local_20 + -5;
        }
        local_1e = (short)local_6c;
      }
      else {
        local_64 = (int)local_3a;
        if (local_20 + 5 < (int)local_3a) {
          local_64 = local_20 + 5;
        }
        local_1e = (short)local_64;
      }
      _SetVolumeAndStereoPosition((int)param_3,(int)local_1e,(int)(short)(int)local_34);
    }
  }
  return;
}


