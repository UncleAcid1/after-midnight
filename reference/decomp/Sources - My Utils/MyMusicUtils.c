// Ghidra decompilation of functions from original source file:
//   Sources - My Utils/MyMusicUtils.c

// ---- _InitMusic @ 0x46676 (144 bytes)

undefined4 _InitMusic(void)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = _Mix_OpenAudio(0xac44,0x8010,2,0x400);
  if (iVar1 == 0) {
    *PTR__gMusicWasInitialized_001c4abc = 1;
  }
  else {
    uVar2 = _SDL_GetError();
    _printf("Unable to initialize audio: %s\n",uVar2);
    *PTR__gMusicWasInitialized_001c4abc = 0;
  }
  if (*PTR__gMusicWasInitialized_001c4abc != '\0') {
    _Mix_AllocateChannels(2);
  }
  return 0;
}


// ---- _ExitMusic @ 0x46706 (19 bytes)

void ExitMusic(void)

{
  _StopAndDisposeMusic();
  return;
}


// ---- _LoadMusic @ 0x46719 (105 bytes)

undefined4 _LoadMusic(undefined4 param_1)

{
  undefined4 uVar1;
  undefined4 local_10;
  
  if (*PTR__gMusicWasInitialized_001c4abc == '\0') {
    local_10 = 0;
  }
  else {
    uVar1 = _Mix_LoadMUS(param_1);
    *(undefined4 *)PTR__gMusicP_001c4ac0 = uVar1;
    if (*(int *)PTR__gMusicP_001c4ac0 == 0) {
      local_10 = 0xffffffff;
    }
    else {
      local_10 = 0;
    }
  }
  return local_10;
}


// ---- _SetMusicLooping @ 0x46782 (40 bytes)

void _SetMusicLooping(undefined1 param_1)

{
  *PTR__gCurrentMusicIsLooping_001c4ab4 = param_1;
  return;
}


// ---- _StartMusic @ 0x467aa (117 bytes)

void _StartMusic(short param_1)

{
  undefined4 local_10;
  
  if (*PTR__gMusicWasInitialized_001c4abc != '\0') {
    if (*PTR__gCurrentMusicIsLooping_001c4ab4 == '\0') {
      local_10 = 1;
    }
    else {
      local_10 = 0xffffffff;
    }
    _SetMusicVolume((int)param_1);
    _Mix_PlayMusic(*(undefined4 *)PTR__gMusicP_001c4ac0,local_10);
  }
  return;
}


// ---- _SetMusicVolume @ 0x4681f (147 bytes)

void _SetMusicVolume(short param_1)

{
  short sVar1;
  
  if (*PTR__gMusicWasInitialized_001c4abc != '\0') {
    if (*PTR__gSofterMusic_001c4768 == '\0') {
      sVar1 = param_1;
      if (param_1 < 0) {
        sVar1 = param_1 + 7;
      }
      _Mix_VolumeMusic((sVar1 >> 3) + 1);
    }
    else {
      _Mix_VolumeMusic(param_1 / 0x12 + 1);
    }
    *(short *)PTR__gCurMusicVolume_001c4ab8 = param_1;
  }
  return;
}


// ---- _GetMusicVolume @ 0x468b2 (32 bytes)

int _GetMusicVolume(void)

{
  return (int)*(short *)PTR__gCurMusicVolume_001c4ab8;
}


// ---- _GetCurrentMusicReference @ 0x468d2 (30 bytes)

undefined4 _GetCurrentMusicReference(void)

{
  return *(undefined4 *)PTR__gMusicP_001c4ac0;
}


// ---- _IsMusicDone @ 0x468f0 (68 bytes)

bool _IsMusicDone(void)

{
  int iVar1;
  bool bVar2;
  
  if (*PTR__gMusicWasInitialized_001c4abc == '\0') {
    bVar2 = true;
  }
  else {
    iVar1 = _Mix_PlayingMusic();
    bVar2 = iVar1 == 0;
  }
  return bVar2;
}


// ---- _StopAndDisposeMusic @ 0x46934 (77 bytes)

void _StopAndDisposeMusic(void)

{
  if (*PTR__gMusicWasInitialized_001c4abc != '\0') {
    _Mix_HaltMusic();
    _Mix_FreeMusic(*(undefined4 *)PTR__gMusicP_001c4ac0);
    *(undefined4 *)PTR__gMusicP_001c4ac0 = 0;
  }
  return;
}


// ---- _PauseMusic @ 0x46981 (45 bytes)

void _PauseMusic(void)

{
  if (*PTR__gMusicWasInitialized_001c4abc != '\0') {
    _Mix_PauseMusic();
  }
  return;
}


// ---- _ResumeMusic @ 0x469ae (124 bytes)

void _ResumeMusic(char param_1)

{
  int iVar1;
  undefined1 local_d;
  
  if (*PTR__gMusicWasInitialized_001c4abc != '\0') {
    iVar1 = _Mix_PlayingMusic();
    if (iVar1 == 0) {
      if (*PTR__gCurrentMusicIsLooping_001c4ab4 == '\0') {
        local_d = 1;
      }
      else {
        local_d = 0xff;
      }
      _Mix_PlayMusic(*(undefined4 *)PTR__gMusicP_001c4ac0,local_d);
    }
    else {
      if (param_1 != '\0') {
        _Mix_RewindMusic();
      }
      _Mix_ResumeMusic();
    }
  }
  return;
}


// ---- _IsMusicPlaying @ 0x46a2a (55 bytes)

undefined1 _IsMusicPlaying(void)

{
  int iVar1;
  
  iVar1 = _Mix_PausedMusic();
  if ((iVar1 == 0) && (iVar1 = _Mix_PlayingMusic(), iVar1 != 0)) {
    return 1;
  }
  return 0;
}


// ---- _FadeMusicNow @ 0x46a61 (39 bytes)

void _FadeMusicNow(short param_1)

{
  _Mix_FadeOutMusic(param_1 * 200);
  return;
}


// ---- _FadeMusicNowNoAdjust @ 0x46a88 (33 bytes)

void _FadeMusicNowNoAdjust(short param_1)

{
  _Mix_FadeOutMusic((int)param_1);
  return;
}


// ---- _FadeMusicIn @ 0x46aa9 (104 bytes)

void _FadeMusicIn(short param_1)

{
  undefined4 local_10;
  
  if (*PTR__gCurrentMusicIsLooping_001c4ab4 == '\0') {
    local_10 = 1;
  }
  else {
    local_10 = 0xffffffff;
  }
  _Mix_FadeInMusic(*(undefined4 *)PTR__gMusicP_001c4ac0,local_10,param_1 * 200);
  return;
}


// ---- _StartMusicByFadingIn @ 0x46b11 (138 bytes)

void _StartMusicByFadingIn(undefined4 param_1,undefined4 param_2,short param_3)

{
  int iVar1;
  
  _MyPlayMusic(param_1,(int)param_3,0,1);
  if (*PTR__gMusicIsOn_001c47c0 != '\0') {
    _Mix_FadeOutMusic(0);
    do {
      iVar1 = _Mix_PlayingMusic();
    } while (iVar1 != 0);
    _Mix_FadeInMusic(*(undefined4 *)PTR__gMusicP_001c4ac0,0xffffffff,param_2);
  }
  return;
}


// ---- _StartOpeningMusic @ 0x46b9b (224 bytes)

void _StartOpeningMusic(char param_1)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar2 = _GetCurrentMusicReference();
  if ((iVar2 == *(int *)PTR__gOpeningMusicRefNum_001c471c) &&
     (cVar1 = _IsMusicDone(), cVar1 == '\0')) {
    return;
  }
  _StopAndDisposeMusic();
  if ((*PTR__gDoOpeningMusic_001c4ac4 == '\0') && (param_1 == '\0')) {
    _MMPlayStereoSound2(0,0x4c,6,2,0xff,0x3ff0000000000000,0xffffffff);
  }
  else {
    _MyPlayMusic("Opening Music",(int)*(short *)PTR__gTitleMusicVolume_001c48bc,0,1);
  }
  uVar3 = _GetCurrentMusicReference();
  *(undefined4 *)PTR__gOpeningMusicRefNum_001c471c = uVar3;
  return;
}


// ---- _PlayBackgroundMusic @ 0x46c7b (85 bytes)

void _PlayBackgroundMusic(short param_1)

{
  if (0 < param_1) {
    _MyPlayMusic("Background Music",(int)*(short *)PTR__gBackgroundMusicVolume_001c4704,(int)param_1
                 ,1);
  }
  return;
}


// ---- _MyPlayMusic @ 0x46cd0 (594 bytes)

void _MyPlayMusic(undefined4 param_1,short param_2,short param_3,undefined1 param_4)

{
  undefined4 uVar1;
  undefined4 uVar2;
  char local_404 [500];
  char local_210 [256];
  char local_110 [256];
  int local_10;
  
  local_10 = 0;
  if (*PTR__gUsingEditor_001c48b0 == '\0') {
    _StopAndDisposeMusic();
    uVar2 = param_1;
    if (param_3 < 1) {
      *(undefined2 *)PTR__gCurrentMusicIndex_001c476c = 0;
      uVar1 = _GetMusicFolderPath();
      _sprintf(local_110,"%s%s.mp3",uVar1,param_1);
      uVar1 = _GetMusicFolderPath();
      _sprintf(local_210,"%s%s.ogg",uVar1,param_1);
    }
    else {
      *(short *)PTR__gCurrentMusicIndex_001c476c = param_3;
      uVar1 = _GetMusicFolderPath();
      _sprintf(local_110,"%s%s %d.mp3",uVar1,param_1,(int)param_3);
      uVar1 = _GetMusicFolderPath();
      _sprintf(local_210,"%s%s %d.ogg",uVar1,param_1,(int)param_3);
    }
    local_10 = _LoadMusic(local_110);
    if (local_10 != 0) {
      local_10 = _LoadMusic(local_210);
    }
    if ((*PTR__gMusicIsOn_001c47c0 != '\0') && (local_10 != 0)) {
      if (param_3 < 1) {
        _sprintf(local_404,
                 "The music couldn\'t be loaded. Please make sure the file \r%s.mp3\r is inside the Game Data/Music folder."
                 ,param_1,uVar2);
      }
      else {
        _sprintf(local_404,
                 "The music couldn\'t be loaded. Please make sure the file \r%s %d.mp3\r is inside the Game Data/Music folder."
                 ,param_1,(int)param_3);
      }
      _ErrorMessage(local_404,0);
      *PTR__gMusicIsOn_001c47c0 = 0;
    }
    if (*PTR__gMusicIsOn_001c47c0 == '\0') {
      _SetMusicLooping(param_4);
      _SetMusicVolume((int)param_2);
    }
    else {
      _SetMusicLooping(param_4);
      _StartMusic((int)param_2);
    }
  }
  return;
}


// ---- _MyFadeMusicNow @ 0x46f22 (89 bytes)

void _MyFadeMusicNow(void)

{
  char cVar1;
  int iVar2;
  
  if (*PTR__gMusicIsOn_001c47c0 != '\0') {
    cVar1 = _IsMusicPlaying();
    if (cVar1 != '\0') {
      _FadeMusicNow(3);
      while( true ) {
        iVar2 = _Mix_PlayingMusic();
        if (iVar2 == 0) break;
        _SDL_Delay(100);
      }
      _StopAndDisposeMusic();
    }
  }
  return;
}


