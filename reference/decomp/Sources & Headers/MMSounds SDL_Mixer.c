// Ghidra decompilation of functions from original source file:
//   Sources & Headers/MMSounds SDL_Mixer.c

// ---- _LoadAndSetupSounds @ 0x9fc20 (885 bytes)

undefined4
_LoadAndSetupSounds(undefined2 param_1,undefined2 param_2,short param_3,undefined4 param_4)

{
  void *pvVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 local_12c;
  char local_113 [255];
  int local_14;
  int local_10;
  
  _Mix_Init(8);
  *(undefined2 *)PTR__gNumChannels_001c5314 = param_1;
  *(undefined2 *)PTR__gNumReservedChannels_001c5310 = param_2;
  pvVar1 = _calloc((int)param_3,4);
  *(void **)PTR__gSounds_001c530c = pvVar1;
  if (*(int *)PTR__gSounds_001c530c == 0) {
    local_12c = 0xffffffff;
  }
  else {
    iVar2 = _Mix_OpenAudio(0xac44,0x8010,2,0x400);
    if (iVar2 == 0) {
      *PTR__gAudioWasInitialized_001c52d0 = 1;
    }
    else {
      uVar3 = _SDL_GetError();
      _printf("Unable to initialize audio: %s\n",uVar3);
      *PTR__gAudioWasInitialized_001c52d0 = 0;
    }
    _puts("Allocating sound channels...");
    if (*PTR__gAudioWasInitialized_001c52d0 != '\0') {
      _Mix_AllocateChannels((int)*(short *)PTR__gNumChannels_001c5314);
      _Mix_ReserveChannels((int)*(short *)PTR__gNumReservedChannels_001c5310);
    }
    _puts("Loading sounds...");
    if (*PTR__gAudioWasInitialized_001c52d0 != '\0') {
      for (local_14 = 0; local_14 < 0x4f; local_14 = local_14 + 1) {
        local_10 = local_14 + 0x80;
        _sprintf(local_113,"%s00%d.wav",param_4,local_10);
        uVar3 = _SDL_RWFromFile(local_113,"rb");
        uVar3 = _Mix_LoadWAV_RW(uVar3,1);
        *(undefined4 *)(local_14 * 4 + *(int *)PTR__gSounds_001c530c) = uVar3;
        if (*(int *)(local_14 * 4 + *(int *)PTR__gSounds_001c530c) == 0) {
          _sprintf(local_113,"%s00%d.ogg",param_4,local_10);
          uVar3 = _SDL_RWFromFile(local_113,"rb");
          uVar3 = _Mix_LoadWAV_RW(uVar3,1);
          *(undefined4 *)(local_14 * 4 + *(int *)PTR__gSounds_001c530c) = uVar3;
        }
        if (*(int *)(local_14 * 4 + *(int *)PTR__gSounds_001c530c) == 0) {
          _sprintf(local_113,"%s00%d.aif",param_4,local_10);
          _printf("  Failed to load WAV or OGG, so attempting to load AIFF: %s \n",local_113);
          uVar3 = _SDL_RWFromFile(local_113,"rb");
          uVar3 = _Mix_LoadWAV_RW(uVar3,1);
          *(undefined4 *)(local_14 * 4 + *(int *)PTR__gSounds_001c530c) = uVar3;
          if (*(int *)(local_14 * 4 + *(int *)PTR__gSounds_001c530c) != 0) {
            _puts("   Success loading .aif file!");
          }
        }
        if (*(int *)(local_14 * 4 + *(int *)PTR__gSounds_001c530c) == 0) {
          return 0xfffffffe;
        }
      }
    }
    local_12c = 0;
  }
  return local_12c;
}


// ---- _PlaySoundDetailed @ 0x9ff95 (350 bytes)

undefined4
_PlaySoundDetailed(short param_1,short param_2,int param_3,short param_4,short param_5,
                  undefined4 param_6,undefined4 param_7,int param_8)

{
  short sVar1;
  undefined4 uVar2;
  short local_10;
  short local_e;
  
  if ((*PTR__gSoundIsTurnedOn_001c47fc != '\0') && (*PTR__gAudioWasInitialized_001c52d0 != '\0')) {
    if (param_3 == 1) {
      local_e = _FindEmptyChannel();
      if (local_e == -1) {
        local_e = param_2;
      }
    }
    else {
      local_e = param_2;
      if (param_3 == 3) {
        local_e = _FindSound((int)param_1);
      }
    }
    if (param_8 == -1) {
      local_10 = -1;
    }
    else {
      local_10 = 0;
    }
    sVar1 = _Mix_PlayChannelTimed
                      ((int)local_e,*(undefined4 *)(param_1 * 4 + *(int *)PTR__gSounds_001c530c),
                       (int)local_10,0xffffffff);
    if (sVar1 == -1) {
      uVar2 = _SDL_GetError();
      _printf("Unable to play WAV file: %s\n",uVar2);
    }
    else {
      _SetVolumeAndStereoPosition((int)sVar1,(int)(param_4 / 2),(int)param_5);
    }
  }
  return 0;
}


// ---- _CountSounds @ 0xa00f3 (144 bytes)

int _CountSounds(short param_1)

{
  int iVar1;
  int iVar2;
  undefined2 local_14;
  undefined2 local_12;
  
  local_12 = 0;
  if (*PTR__gAudioWasInitialized_001c52d0 != '\0') {
    iVar1 = *(int *)(param_1 * 4 + *(int *)PTR__gSounds_001c530c);
    for (local_14 = 0; local_14 < 0x10; local_14 = local_14 + 1) {
      iVar2 = _Mix_Playing((int)local_14);
      if (iVar2 != 0) {
        iVar2 = _Mix_GetChunk((int)local_14);
        if (iVar2 == iVar1) {
          local_12 = local_12 + 1;
        }
      }
    }
  }
  return (int)local_12;
}


// ---- _FindSound @ 0xa0183 (151 bytes)

int _FindSound(short param_1)

{
  int iVar1;
  int iVar2;
  short local_16;
  
  if (*PTR__gAudioWasInitialized_001c52d0 != '\0') {
    iVar1 = *(int *)(param_1 * 4 + *(int *)PTR__gSounds_001c530c);
    for (local_16 = 0; local_16 < 0x10; local_16 = local_16 + 1) {
      iVar2 = _Mix_Playing((int)local_16);
      if ((iVar2 != 0) && (iVar2 = _Mix_GetChunk((int)local_16), iVar2 == iVar1)) {
        return (int)local_16;
      }
    }
  }
  return -1;
}


// ---- _StopMySound @ 0xa021a (96 bytes)

void _StopMySound(short param_1)

{
  short sVar1;
  
  if (((*PTR__gAudioWasInitialized_001c52d0 != '\0') && (-1 < param_1)) && (param_1 < 0x4f)) {
    sVar1 = _FindSound((int)param_1);
    if (sVar1 != -1) {
      _Mix_HaltChannel((int)sVar1);
    }
  }
  return;
}


// ---- _GetChannelVolume @ 0xa027a (34 bytes)

int _GetChannelVolume(undefined4 param_1)

{
  short sVar1;
  
  sVar1 = _Mix_Volume(param_1,0xffffffff);
  return (int)sVar1;
}


// ---- _StopChannel @ 0xa029c (59 bytes)

void _StopChannel(short param_1)

{
  if (*PTR__gAudioWasInitialized_001c52d0 != '\0') {
    _Mix_HaltChannel((int)param_1);
  }
  return;
}


// ---- _StopAllChannels @ 0xa02d7 (52 bytes)

void _StopAllChannels(void)

{
  if (*PTR__gAudioWasInitialized_001c52d0 != '\0') {
    _Mix_HaltChannel(0xffffffff);
  }
  return;
}


// ---- _IsChannelAvailable @ 0xa030b (74 bytes)

bool _IsChannelAvailable(undefined4 param_1)

{
  int iVar1;
  bool bVar2;
  
  if (*PTR__gAudioWasInitialized_001c52d0 == '\0') {
    bVar2 = false;
  }
  else {
    iVar1 = _Mix_Playing(param_1);
    bVar2 = iVar1 == 0;
  }
  return bVar2;
}


// ---- _FindEmptyChannel @ 0xa0355 (90 bytes)

int _FindEmptyChannel(void)

{
  int iVar1;
  short local_e;
  
  local_e = *(short *)PTR__gNumReservedChannels_001c5310;
  while( true ) {
    if (0xf < local_e) {
      return -1;
    }
    iVar1 = _Mix_Playing((int)local_e);
    if (iVar1 == 0) break;
    local_e = local_e + 1;
  }
  return (int)local_e;
}


// ---- _SetVolumeAndStereoPosition @ 0xa03af (235 bytes)

void _SetVolumeAndStereoPosition(short param_1,short param_2,short param_3)

{
  undefined2 local_28;
  undefined2 local_24;
  undefined2 local_12;
  undefined2 local_10;
  
  local_24 = param_2;
  local_28 = param_3;
  if (param_2 < 0) {
    local_24 = 0;
  }
  if (param_3 < -0xff) {
    local_28 = -0xff;
  }
  else if (0xff < param_3) {
    local_28 = 0xff;
  }
  local_10 = (short)(((int)local_24 * (int)local_28) / 0xff);
  local_12 = local_24 - local_10;
  local_10 = local_10 + local_24;
  if (0xff < local_10) {
    local_10 = 0xff;
  }
  if (0xff < local_12) {
    local_12 = 0xff;
  }
  _Mix_Volume((int)param_1,(int)local_24);
  _Mix_SetPanning((int)param_1,(char)local_12,(char)local_10);
  return;
}


