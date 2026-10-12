// Ghidra decompilation of functions from original source file:
//   Sources - My Utils/MyButtonSprite.c

// ---- _LoadStandardButtonSprite @ 0xf440 (229 bytes)

undefined4 _LoadStandardButtonSprite(undefined4 param_1,short param_2,int param_3)

{
  undefined4 uVar1;
  char *filename;
  SDL_Surface *pSVar2;
  undefined4 local_24;
  
  uVar1 = _GetSpriteImagePath(0);
  filename = (char *)_GetFileNameForResource(uVar1,(int)param_2);
  pSVar2 = _BKLoadSurface(filename);
  if (pSVar2 == (SDL_Surface *)0x0) {
    local_24 = 0x12f;
  }
  else {
    local_24 = _SWCreateSpriteFromSurfaceXY
                         (param_1,0,pSVar2,pSVar2->w,pSVar2->h / 3,0,0,0,3,param_3 != 0);
  }
  return local_24;
}


// ---- _LoadStandardButtonSpriteFromFile @ 0xf525 (187 bytes)

undefined4 _LoadStandardButtonSpriteFromFile(undefined4 param_1,char *param_2,undefined1 param_3)

{
  SDL_Surface *pSVar1;
  undefined4 local_24;
  
  pSVar1 = _BKLoadSurface(param_2);
  if (pSVar1 == (SDL_Surface *)0x0) {
    local_24 = 0x12f;
  }
  else {
    local_24 = _SWCreateSpriteFromSurfaceXY
                         (param_1,0,pSVar1,pSVar1->w,pSVar1->h / 3,0,0,0,3,param_3);
  }
  return local_24;
}


// ---- _LoadStandardButtonSpriteMask @ 0xf5e0 (236 bytes)

undefined4
LoadStandardButtonSpriteMask(undefined4 param_1,short param_2,undefined4 param_3,int param_4)

{
  undefined4 uVar1;
  char *filename;
  SDL_Surface *pSVar2;
  undefined4 uStack_28;
  
  uVar1 = _GetSpriteImagePath(0);
  filename = (char *)_GetFileNameForResource(uVar1,(int)param_2);
  pSVar2 = _BKLoadSurface(filename);
  if (pSVar2 == (SDL_Surface *)0x0) {
    uStack_28 = 0x12f;
  }
  else {
    uStack_28 = _SWCreateSpriteFromSurfaceXY
                          (param_1,0,pSVar2,pSVar2->w,pSVar2->h / 3,0,0,0,3,param_4 != 0);
  }
  return uStack_28;
}


// ---- _LoadStandardDimmableButtonSprite @ 0xf6cc (214 bytes)

undefined4 _LoadStandardDimmableButtonSprite(undefined4 param_1,short param_2,int param_3)

{
  undefined4 uVar1;
  char *filename;
  SDL_Surface *pSVar2;
  int iVar3;
  undefined4 local_24;
  
  uVar1 = _GetSpriteImagePath(0);
  filename = (char *)_GetFileNameForResource(uVar1,(int)param_2);
  pSVar2 = _BKLoadSurface(filename);
  if (pSVar2 == (SDL_Surface *)0x0) {
    local_24 = 0x12f;
  }
  else {
    iVar3 = pSVar2->h;
    if (iVar3 < 0) {
      iVar3 = iVar3 + 3;
    }
    local_24 = _SWCreateSpriteFromSurfaceXY
                         (param_1,0,pSVar2,pSVar2->w,iVar3 >> 2,0,0,0,4,param_3 != 0);
  }
  return local_24;
}


// ---- _LoadMiscButtonSprite @ 0xf7a2 (232 bytes)

undefined4
LoadMiscButtonSprite(undefined4 param_1,short param_2,short param_3,undefined4 param_4,int param_5)

{
  undefined4 uVar1;
  char *filename;
  SDL_Surface *pSVar2;
  undefined4 uStack_2c;
  
  uVar1 = _GetSpriteImagePath(0);
  filename = (char *)_GetFileNameForResource(uVar1,(int)param_3);
  pSVar2 = _BKLoadSurface(filename);
  if (pSVar2 == (SDL_Surface *)0x0) {
    uStack_2c = 0x12f;
  }
  else {
    uStack_2c = _SWCreateSpriteFromSurfaceXY
                          (param_1,0,pSVar2,pSVar2->w,pSVar2->h / (int)param_2,0,0,0,(int)param_2,
                           param_5 != 0);
  }
  return uStack_2c;
}


// ---- _LoadMiscButtonSpriteFromFile @ 0xf88a (177 bytes)

undefined4
_LoadMiscButtonSpriteFromFile(undefined4 param_1,short param_2,char *param_3,undefined1 param_4)

{
  SDL_Surface *pSVar1;
  undefined4 local_28;
  
  pSVar1 = _BKLoadSurface(param_3);
  if (pSVar1 == (SDL_Surface *)0x0) {
    local_28 = 0x12f;
  }
  else {
    local_28 = _SWCreateSpriteFromSurfaceXY
                         (param_1,0,pSVar1,pSVar1->w,pSVar1->h / (int)param_2,0,0,0,(int)param_2,
                          param_4);
  }
  return local_28;
}


// ---- _CloneButtonSprite @ 0xf93b (461 bytes)

int _CloneButtonSprite(undefined4 param_1,undefined2 param_2)

{
  int local_18;
  void *local_14;
  int local_10;
  
  local_10 = 0;
  local_14 = _calloc(1,400);
  if (local_14 == (void *)0x0) {
    local_10 = -1;
  }
  if (local_10 == 0) {
    local_10 = _SWCloneSprite(param_1,&local_18,local_14,400);
  }
  if (local_10 == 0) {
    *(undefined4 *)(local_18 + 0xe8) = 0x149f;
    *(undefined2 *)((int)local_14 + 0xf8) = 0;
    *(undefined2 *)((int)local_14 + 0xfc) = param_2;
    *(undefined2 *)((int)local_14 + 0xfe) = 0;
    *(undefined2 *)((int)local_14 + 0x100) = 0;
    *(undefined2 *)((int)local_14 + 0x102) = 0;
    *(undefined2 *)((int)local_14 + 0x108) = 0;
    *(undefined2 *)((int)local_14 + 0x10a) = 0;
    *(undefined2 *)((int)local_14 + 0x10c) = 0;
    *(undefined2 *)((int)local_14 + 0x10e) = 0;
    *(undefined1 *)((int)local_14 + 0x110) = 0;
    *(undefined4 *)((int)local_14 + 0x114) = 0;
    *(undefined1 *)((int)local_14 + 0x118) = 0;
    *(undefined1 *)((int)local_14 + 0x120) = 0;
    *(undefined1 *)((int)local_14 + 0x128) = 0;
    *(undefined2 *)((int)local_14 + 0x106) = 0xffff;
    *(undefined1 *)((int)local_14 + 0x130) = 0;
    *(undefined1 *)((int)local_14 + 0x148) = 0;
    *(undefined2 *)((int)local_14 + 0x14a) = 0;
    *(undefined2 *)((int)local_14 + 0x14c) = 0;
    *(undefined4 *)((int)local_14 + 0x164) = 0xbf800000;
    *(undefined4 *)((int)local_14 + 0x170) = 0;
    *(undefined4 *)((int)local_14 + 0x178) = 0;
    *(undefined1 *)((int)local_14 + 0x146) = 1;
    *(undefined1 *)((int)local_14 + 0x147) = 0;
    *(undefined1 *)((int)local_14 + 0x16c) = 1;
    *(undefined4 *)((int)local_14 + 0x18c) = 0;
  }
  else {
    local_18 = 0;
  }
  return local_18;
}


// ---- _SetButtonSpriteIndex @ 0xfb08 (109 bytes)

void SetButtonSpriteIndex(int param_1,undefined2 param_2)

{
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22713_001c430c,0x10a);
  }
  *(undefined2 *)(param_1 + 0xfc) = param_2;
  return;
}


// ---- _SetUpButtonSprite @ 0xfb75 (186 bytes)

void _SetUpButtonSprite(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
                       undefined2 param_5)

{
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22726_001c434c,0x11d);
  }
  *(undefined2 *)(param_1 + 0xf8) = 0;
  *(undefined2 *)(param_1 + 0xfe) = param_2;
  *(undefined2 *)(param_1 + 0x100) = param_3;
  *(undefined2 *)(param_1 + 0x102) = param_4;
  *(undefined2 *)(param_1 + 0x104) = param_5;
  return;
}


// ---- _SetUpCheckboxSprite @ 0xfc2f (241 bytes)

void _SetUpCheckboxSprite
               (int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
               undefined2 param_5,undefined1 param_6)

{
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22740_001c432c,0x132);
  }
  *(undefined2 *)(param_1 + 0xf8) = 1;
  *(undefined2 *)(param_1 + 0x108) = param_2;
  *(undefined2 *)(param_1 + 0x10a) = param_3;
  *(undefined2 *)(param_1 + 0x10c) = param_4;
  *(undefined2 *)(param_1 + 0x10e) = param_5;
  *(undefined1 *)(param_1 + 0x110) = param_6;
  *(undefined1 *)(param_1 + 0x111) = param_6;
  _SetCorrectCheckboxFrame(param_1,0);
  return;
}


// ---- _SetUpKeyInputBox @ 0xfd20 (142 bytes)

void _SetUpKeyInputBox(int param_1,undefined2 param_2,undefined2 param_3)

{
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22751_001c42fc,0x149);
  }
  *(undefined2 *)(param_1 + 0xf8) = 2;
  *(undefined2 *)(param_1 + 0xfe) = param_2;
  *(undefined2 *)(param_1 + 0x102) = param_3;
  return;
}


// ---- _SetKeyInputBoxKeysToIgnore @ 0xfdae (126 bytes)

void _SetKeyInputBoxKeysToIgnore(int param_1,undefined1 param_2,undefined1 param_3)

{
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22762_001c433c,0x157);
  }
  *(undefined1 *)(param_1 + 0x146) = param_2;
  *(undefined1 *)(param_1 + 0x147) = param_3;
  return;
}


// ---- _SetCheckboxChecked @ 0xfe2c (139 bytes)

void _SetCheckboxChecked(int param_1,undefined1 param_2)

{
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22772_001c4344,0x165);
  }
  *(undefined1 *)(param_1 + 0x110) = param_2;
  *(undefined1 *)(param_1 + 0x111) = param_2;
  _SetCorrectCheckboxFrame(param_1,0);
  return;
}


// ---- _SetButtonSpriteText @ 0xfeb7 (236 bytes)

void _SetButtonSpriteText(SpritePtr param_1,int param_2)

{
  short sVar1;
  
  if (param_1->userData != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22782_001c4328,0x17c);
  }
  if ((*(int *)(param_2 + 0xe8) != 0x7edf) && (*(int *)(param_2 + 0xe8) != 0x8dac7c)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22782_001c4328,0x17d);
  }
  param_1->f_114 = (undefined1)param_2;
  param_1->f_115 = param_2._1_1_;
  param_1->f_116 = param_2._2_1_;
  param_1->f_117 = param_2._3_1_;
  _SWSetSpriteDrawProc(param_1,(DrawProcPtr_t)PTR_CustomButtonDrawProc_001c42ec);
  if (param_1->f_118 != '\0') {
    sVar1._0_1_ = param_1->f_0fa;
    sVar1._1_1_ = param_1->f_0fb;
    _ChangeButtonColor(param_1,(int)sVar1);
  }
  return;
}


// ---- _LinkCheckboxWithData @ 0xffa3 (139 bytes)

void _LinkCheckboxWithData(int param_1,undefined1 *param_2)

{
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22804_001c4324,0x194);
  }
  *(undefined1 **)(param_1 + 0x170) = param_2;
  *(undefined1 *)(param_1 + 0x174) = *param_2;
  _SetCheckboxChecked(param_1,*param_2);
  return;
}


// ---- _LinkKeyInputBoxWithData @ 0x1002e (438 bytes)

void _LinkKeyInputBoxWithData(int param_1,undefined4 *param_2,undefined4 *param_3,char *param_4)

{
  undefined4 uVar1;
  
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22819_001c4338,0x1a9);
  }
  *(undefined4 **)(param_1 + 0x178) = param_2;
  *(undefined4 *)(param_1 + 0x17c) = *param_2;
  *(undefined4 *)(param_1 + 0x134) = *param_2;
  *(undefined4 *)(param_1 + 0x138) = *param_3;
  _strcpy((char *)(param_1 + 0x13c),param_4);
  *(undefined4 **)(param_1 + 0x180) = param_3;
  *(undefined4 *)(param_1 + 0x184) = *param_3;
  *(char **)(param_1 + 0x188) = param_4;
  if (*param_4 == '\0') {
    if (*(int *)(*(int *)(param_1 + 0x114) + 0xe8) == 0x7edf) {
      uVar1 = _GetTextFromSDLKeyCode(*param_3);
      _SetBMTextSpriteText(*(undefined4 *)(param_1 + 0x114),uVar1);
    }
    else if (*(int *)(*(int *)(param_1 + 0x114) + 0xe8) == 0x8dac7c) {
      uVar1 = _GetTextFromSDLKeyCode(*param_3);
      _SetTextSpriteText(*(undefined4 *)(param_1 + 0x114),uVar1);
    }
  }
  else if (*(int *)(*(int *)(param_1 + 0x114) + 0xe8) == 0x7edf) {
    _SetBMTextSpriteText(*(undefined4 *)(param_1 + 0x114),param_4);
  }
  else if (*(int *)(*(int *)(param_1 + 0x114) + 0xe8) == 0x8dac7c) {
    _SetTextSpriteText(*(undefined4 *)(param_1 + 0x114),param_4);
  }
  return;
}


// ---- _UpdateDataFromControlsInLayers @ 0x101e4 (287 bytes)

void _UpdateDataFromControlsInLayers(SpriteLayerPtr param_1,SpriteLayerPtr param_2)

{
  short sVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  SpriteLayerPtr local_18;
  SpritePtr local_14;
  
  local_18 = param_1;
  while( true ) {
    if (local_18 == (SpriteLayerPtr)0x0) {
      return;
    }
    for (local_14 = _SWGetNextSprite(local_18,(SpritePtr)0x0); local_14 != (SpritePtr)0x0;
        local_14 = _SWGetNextSprite(local_18,local_14)) {
      if (local_14->userData == 0x149f) {
        sVar1._0_1_ = local_14->f_0f8;
        sVar1._1_1_ = local_14->f_0f9;
        if ((sVar1 == 1) &&
           (iVar3._0_1_ = local_14->f_170, iVar3._1_1_ = local_14->f_171,
           iVar3._2_1_ = local_14->f_172, iVar3._3_1_ = local_14->f_173, iVar3 != 0)) {
          **(uchar **)&local_14->f_170 = local_14->f_110;
        }
        else {
          sVar2._0_1_ = local_14->f_0f8;
          sVar2._1_1_ = local_14->f_0f9;
          if ((sVar2 == 2) &&
             (iVar4._0_1_ = local_14->f_178, iVar4._1_1_ = local_14->f_179,
             iVar4._2_1_ = local_14->f_17a, iVar4._3_1_ = local_14->f_17b, iVar4 != 0)) {
            **(undefined4 **)&local_14->f_178 = *(undefined4 *)&local_14->f_134;
            **(undefined4 **)&local_14->f_180 = *(undefined4 *)&local_14->f_138;
            _strcpy(*(char **)&local_14->f_188,(char *)&local_14->f_13c);
          }
        }
      }
    }
    if (local_18 == param_2) break;
    local_18 = local_18->nextSpriteLayerP;
  }
  return;
}


// ---- _HasControlChangedValues @ 0x10303 (220 bytes)

bool HasControlChangedValues(int param_1)

{
  bool bVar1;
  
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22904_001c4310,0x201);
  }
  if ((*(short *)(param_1 + 0xf8) == 1) && (*(int *)(param_1 + 0x170) != 0)) {
    bVar1 = *(char *)(param_1 + 0x174) == *(char *)(param_1 + 0x110);
  }
  else if ((*(short *)(param_1 + 0xf8) == 2) && (*(int *)(param_1 + 0x178) != 0)) {
    bVar1 = *(int *)(param_1 + 0x17c) == *(int *)(param_1 + 0x134);
  }
  else {
    bVar1 = false;
  }
  return bVar1;
}


// ---- _SetButtonColor @ 0x103df (252 bytes)

void _SetButtonColor(SpritePtr param_1,ushort param_2,ushort param_3,ushort param_4)

{
  int iVar1;
  
  if (param_1->userData != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22932_001c42f8,0x215);
  }
  param_1->f_118 = '\x01';
  param_1->f_11a = (undefined1)param_2;
  param_1->f_11b = param_2._1_1_;
  param_1->f_11c = (undefined1)param_3;
  param_1->f_11d = param_3._1_1_;
  param_1->f_11e = (undefined1)param_4;
  param_1->f_11f = param_4._1_1_;
  _SWSetSpriteColor(param_1,param_2,param_3,param_4);
  iVar1._0_1_ = param_1->f_114;
  iVar1._1_1_ = param_1->f_115;
  iVar1._2_1_ = param_1->f_116;
  iVar1._3_1_ = param_1->f_117;
  if (iVar1 != 0) {
    _SWSetSpriteColor(*(SpritePtr *)&param_1->f_114,param_2,param_3,param_4);
  }
  return;
}


// ---- _SetButtonHighlightedColor @ 0x104db (161 bytes)

void _SetButtonHighlightedColor
               (int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22954_001c4314,0x229);
  }
  *(undefined1 *)(param_1 + 0x120) = 1;
  *(undefined2 *)(param_1 + 0x122) = param_2;
  *(undefined2 *)(param_1 + 0x124) = param_3;
  *(undefined2 *)(param_1 + 0x126) = param_4;
  return;
}


// ---- _SetButtonPushedColor @ 0x1057c (161 bytes)

void _SetButtonPushedColor(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22966_001c42e4,0x239);
  }
  *(undefined1 *)(param_1 + 0x128) = 1;
  *(undefined2 *)(param_1 + 0x12a) = param_2;
  *(undefined2 *)(param_1 + 300) = param_3;
  *(undefined2 *)(param_1 + 0x12e) = param_4;
  return;
}


// ---- _SetButtonDimmedMode @ 0x1061d (202 bytes)

void _SetButtonDimmedMode(SpritePtr param_1,char param_2)

{
  short sVar1;
  short sVar2;
  
  if (param_1->userData != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22976_001c4318,0x24a);
  }
  if (param_2 == '\0') {
    if (param_1->f_148 == '\x01') {
      sVar2._0_1_ = param_1->f_0fe;
      sVar2._1_1_ = param_1->f_0ff;
      _SWSetCurrentFrameIndex(param_1,(int)sVar2);
      param_1->f_148 = '\0';
    }
  }
  else if (param_1->f_148 == '\0') {
    sVar1._0_1_ = param_1->f_104;
    sVar1._1_1_ = param_1->f_105;
    _SWSetCurrentFrameIndex(param_1,(int)sVar1);
    param_1->f_148 = '\x01';
  }
  return;
}


// ---- _SetButtonClickRadius @ 0x106e7 (109 bytes)

void _SetButtonClickRadius(int param_1,undefined2 param_2)

{
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___22999_001c42dc,0x267);
  }
  *(undefined2 *)(param_1 + 0x106) = param_2;
  return;
}


// ---- _GetButtonIndex @ 0x10754 (99 bytes)

int GetButtonIndex(int param_1)

{
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___23008_001c42d8,0x275);
  }
  return (int)*(short *)(param_1 + 0xfc);
}


// ---- _InstallButtonProc @ 0x107b7 (100 bytes)

void _InstallButtonProc(int param_1,undefined4 param_2)

{
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___23021_001c4308,0x287);
  }
  *(undefined4 *)(param_1 + 0x18c) = param_2;
  return;
}


// ---- _SetButtonPushedOffset @ 0x1081b (130 bytes)

void _SetButtonPushedOffset(int param_1,undefined2 param_2,undefined2 param_3)

{
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___23032_001c4340,0x296);
  }
  *(undefined2 *)(param_1 + 0x14a) = param_2;
  *(undefined2 *)(param_1 + 0x14c) = param_3;
  return;
}


// ---- _AdjustCollisionRectForOffset @ 0x1089d (173 bytes)

void _AdjustCollisionRectForOffset(int param_1)

{
  if (*(int *)(param_1 + 0xe8) != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___23041_001c4304,0x2a7);
  }
  _SWSetFrameCollisionInset
            (*(FramePtr *)(*(int *)(param_1 + 0x78) + *(short *)(param_1 + 0xfe) * 4),
             *(short *)(param_1 + 0x14a),*(short *)(param_1 + 0x14c),0,0);
  return;
}


// ---- _ProcessButtonSprites @ 0x1094a (874 bytes)

int _ProcessButtonSprites
              (SpriteLayerRec *param_1,SpriteLayerPtr param_2,SWPoint param_3,char param_4,
              char param_5,undefined4 *param_6)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  int iVar7;
  uchar uVar8;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SWMovementType SVar9;
  short local_2a;
  SpriteLayerRec *local_28;
  SpritePtr local_24;
  SWBoolean local_1d;
  SpritePtr local_1c;
  short local_16;
  short local_14;
  short local_12;
  short local_10;
  short local_e;
  
  local_2a = -1;
  *(SpriteLayerRec **)PTR__prevButtonFirstSpriteLayerP_001c4358 = param_1;
  *(SpriteLayerPtr *)PTR__prevButtonLastSpriteLayerP_001c4320 = param_2;
  if (param_6 != (undefined4 *)0x0) {
    *param_6 = 0;
  }
  *(undefined4 *)PTR__curHilightedSpriteP_001c42f4 = 0;
  for (local_28 = param_1; local_28 != (SpriteLayerRec *)0x0; local_28 = local_28->nextSpriteLayerP)
  {
    uVar8 = _SWGetSpriteLayerVisible(local_28);
    if (uVar8 != '\0') {
      for (local_24 = _SWGetNextSprite(local_28,(SpritePtr)0x0); local_24 != (SpritePtr)0x0;
          local_24 = _SWGetNextSprite(local_28,local_24)) {
        local_1c = local_24;
        if (local_24->userData == 0x149f) {
          if ((param_5 == '\0') ||
             ((sVar1._0_1_ = local_24->f_0f8, sVar1._1_1_ = local_24->f_0f9, sVar1 == 1 &&
              (iVar7._0_1_ = local_24->f_114, iVar7._1_1_ = local_24->f_115,
              iVar7._2_1_ = local_24->f_116, iVar7._3_1_ = local_24->f_117, iVar7 != 0)))) {
            local_1d = _SWIsPointInSprite(local_24,param_3);
          }
          else {
            local_1d = _SWIsPointInSpriteMask
                                 (local_24,(int)param_3.h,(int)param_3.v,PTR_LAB_001c42f0);
          }
          if ((local_1d != '\0') &&
             (sVar2._0_1_ = local_1c->f_106, sVar2._1_1_ = local_1c->f_107, sVar2 != -1)) {
            SVar9 = _SWGetSpriteHorizLoc(local_24);
            _SWGetSpriteWidth(local_24);
            local_16 = (short)(int)((float)extraout_ST0 / 2.0 + SVar9);
            SVar9 = _SWGetSpriteVertLoc(local_24);
            _SWGetSpriteHeight(local_24);
            local_14 = (short)(int)((float)extraout_ST0_00 / 2.0 + SVar9);
            local_10 = local_16 - param_3.h;
            local_e = local_14 - param_3.v;
            local_12 = (short)(int)SQRT((double)((int)local_10 * (int)local_10 +
                                                (int)local_e * (int)local_e));
            sVar3._0_1_ = local_1c->f_106;
            sVar3._1_1_ = local_1c->f_107;
            if (sVar3 < local_12) {
              local_1d = '\0';
            }
          }
          sVar4._0_1_ = local_1c->f_0f8;
          sVar4._1_1_ = local_1c->f_0f9;
          if (sVar4 == 0) {
            _ProcessNormalButtonSprite(local_28,local_24,local_1d,param_4,&local_2a,param_6);
          }
          else {
            sVar5._0_1_ = local_1c->f_0f8;
            sVar5._1_1_ = local_1c->f_0f9;
            if (sVar5 == 1) {
              _ProcessCheckBoxSprite(local_28,local_24,local_1d,param_4,&local_2a,param_6);
            }
            else {
              sVar6._0_1_ = local_1c->f_0f8;
              sVar6._1_1_ = local_1c->f_0f9;
              if (sVar6 == 2) {
                _ProcessKeyInputBoxSprite(local_28,local_24,local_1d,param_4,&local_2a,param_6);
              }
              else {
                (**(code **)PTR__gSWAssertFailProc_001c4170)
                          ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c"
                           ,PTR____FUNCTION___23081_001c4360,0x303);
              }
            }
          }
        }
      }
    }
    if (local_28 == param_2) break;
  }
  if (param_4 == '\0') {
    *(undefined4 *)PTR__prevSelectedSpriteP_001c42e0 = 0;
  }
  *PTR__prevButtonWasDown_001c4354 = param_4;
  return (int)local_2a;
}


// ---- _ProcessNormalButtonSprite @ 0x10cb4 (1032 bytes)

void _ProcessNormalButtonSprite
               (undefined4 param_1,SpritePtr param_2,char param_3,char param_4,undefined2 *param_5,
               undefined4 *param_6)

{
  short sVar1;
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
  short sVar22;
  short sVar23;
  short sVar24;
  short sVar25;
  short sVar26;
  short sVar27;
  int iVar28;
  int iVar29;
  int iVar30;
  short local_e;
  
  if (param_2->f_148 == '\0') {
    if (param_3 == '\0') {
      sVar19._0_1_ = param_2->f_0fa;
      sVar19._1_1_ = param_2->f_0fb;
      param_2->f_0fa = '\0';
      param_2->f_0fb = '\0';
      sVar20._0_1_ = param_2->f_0fe;
      sVar20._1_1_ = param_2->f_0ff;
      if (param_2->curFrameIndex != (int)sVar20) {
        sVar21._0_1_ = param_2->f_0fe;
        sVar21._1_1_ = param_2->f_0ff;
        _SWSetCurrentFrameIndex(param_2,(int)sVar21);
      }
      sVar22._0_1_ = param_2->f_0fa;
      sVar22._1_1_ = param_2->f_0fb;
      if (sVar22 != sVar19) {
        sVar23._0_1_ = param_2->f_0fa;
        sVar23._1_1_ = param_2->f_0fb;
        _ChangeButtonColor(param_2,(int)sVar23);
      }
      iVar30._0_1_ = param_2->f_18c;
      iVar30._1_1_ = param_2->f_18d;
      iVar30._2_1_ = param_2->f_18e;
      iVar30._3_1_ = param_2->f_18f;
      if (iVar30 != 0) {
        sVar24._0_1_ = param_2->f_0fa;
        sVar24._1_1_ = param_2->f_0fb;
        sVar25._0_1_ = param_2->f_0fc;
        sVar25._1_1_ = param_2->f_0fd;
        (**(code **)&param_2->f_18c)(param_2,(int)sVar25,(int)sVar24,(int)sVar19);
      }
      if (sVar19 == 2) {
        sVar26._0_1_ = param_2->f_14c;
        sVar26._1_1_ = param_2->f_14d;
        sVar27._0_1_ = param_2->f_14a;
        sVar27._1_1_ = param_2->f_14b;
        _SWOffsetSprite(param_2,(float)-(int)sVar27,(float)-(int)sVar26);
      }
    }
    else if (param_4 == '\0') {
      if (*(int *)PTR__curHilightedSpriteP_001c42f4 == 0) {
        *(SpritePtr *)PTR__curHilightedSpriteP_001c42f4 = param_2;
        _UnhighlightAllOtherButtonSprites(param_1,param_2,0);
        sVar10._0_1_ = param_2->f_0fa;
        sVar10._1_1_ = param_2->f_0fb;
        param_2->f_0fa = '\x01';
        param_2->f_0fb = '\0';
        sVar11._0_1_ = param_2->f_100;
        sVar11._1_1_ = param_2->f_101;
        if (param_2->curFrameIndex != (int)sVar11) {
          sVar12._0_1_ = param_2->f_100;
          sVar12._1_1_ = param_2->f_101;
          _SWSetCurrentFrameIndex(param_2,(int)sVar12);
        }
        sVar13._0_1_ = param_2->f_0fa;
        sVar13._1_1_ = param_2->f_0fb;
        if (sVar13 != sVar10) {
          sVar14._0_1_ = param_2->f_0fa;
          sVar14._1_1_ = param_2->f_0fb;
          _ChangeButtonColor(param_2,(int)sVar14);
        }
        iVar29._0_1_ = param_2->f_18c;
        iVar29._1_1_ = param_2->f_18d;
        iVar29._2_1_ = param_2->f_18e;
        iVar29._3_1_ = param_2->f_18f;
        if (iVar29 != 0) {
          sVar15._0_1_ = param_2->f_0fa;
          sVar15._1_1_ = param_2->f_0fb;
          sVar16._0_1_ = param_2->f_0fc;
          sVar16._1_1_ = param_2->f_0fd;
          (**(code **)&param_2->f_18c)(param_2,(int)sVar16,(int)sVar15,(int)sVar10);
        }
        if (sVar10 == 2) {
          sVar17._0_1_ = param_2->f_14c;
          sVar17._1_1_ = param_2->f_14d;
          sVar18._0_1_ = param_2->f_14a;
          sVar18._1_1_ = param_2->f_14b;
          _SWOffsetSprite(param_2,(float)-(int)sVar18,(float)-(int)sVar17);
          *param_5 = *(undefined2 *)&param_2->f_0fc;
          if (param_6 != (undefined4 *)0x0) {
            *param_6 = param_2;
          }
        }
      }
    }
    else {
      local_e._0_1_ = param_2->f_0fa;
      local_e._1_1_ = param_2->f_0fb;
      if ((((*(int *)PTR__prevSelectedSpriteP_001c42e0 == 0) &&
           (*PTR__prevButtonWasDown_001c4354 == '\0')) ||
          (param_2 == *(SpritePtr *)PTR__prevSelectedSpriteP_001c42e0)) &&
         (*(SpritePtr *)PTR__prevSelectedSpriteP_001c42e0 = param_2, sVar1._0_1_ = param_2->f_0fa,
         sVar1._1_1_ = param_2->f_0fb, sVar1 != 2)) {
        local_e._0_1_ = param_2->f_0fa;
        local_e._1_1_ = param_2->f_0fb;
        param_2->f_0fa = '\x02';
        param_2->f_0fb = '\0';
        sVar2._0_1_ = param_2->f_102;
        sVar2._1_1_ = param_2->f_103;
        if (param_2->curFrameIndex != (int)sVar2) {
          sVar3._0_1_ = param_2->f_102;
          sVar3._1_1_ = param_2->f_103;
          _SWSetCurrentFrameIndex(param_2,(int)sVar3);
        }
        sVar4._0_1_ = param_2->f_14c;
        sVar4._1_1_ = param_2->f_14d;
        sVar5._0_1_ = param_2->f_14a;
        sVar5._1_1_ = param_2->f_14b;
        _SWOffsetSprite(param_2,(float)(int)sVar5,(float)(int)sVar4);
      }
      sVar6._0_1_ = param_2->f_0fa;
      sVar6._1_1_ = param_2->f_0fb;
      if (sVar6 != local_e) {
        sVar7._0_1_ = param_2->f_0fa;
        sVar7._1_1_ = param_2->f_0fb;
        _ChangeButtonColor(param_2,(int)sVar7);
      }
      iVar28._0_1_ = param_2->f_18c;
      iVar28._1_1_ = param_2->f_18d;
      iVar28._2_1_ = param_2->f_18e;
      iVar28._3_1_ = param_2->f_18f;
      if (iVar28 != 0) {
        sVar8._0_1_ = param_2->f_0fa;
        sVar8._1_1_ = param_2->f_0fb;
        sVar9._0_1_ = param_2->f_0fc;
        sVar9._1_1_ = param_2->f_0fd;
        (**(code **)&param_2->f_18c)(param_2,(int)sVar9,(int)sVar8,(int)local_e);
      }
    }
  }
  return;
}


// ---- _ProcessCheckBoxSprite @ 0x110bc (1026 bytes)

void _ProcessCheckBoxSprite
               (undefined4 param_1,SpritePtr param_2,char param_3,char param_4,undefined2 *param_5,
               undefined4 *param_6)

{
  short sVar1;
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
  int iVar24;
  short local_e;
  
  if (param_3 == '\0') {
    sVar15._0_1_ = param_2->f_0fa;
    sVar15._1_1_ = param_2->f_0fb;
    param_2->f_0fa = '\0';
    param_2->f_0fb = '\0';
    _SetCorrectCheckboxFrame(param_2,0);
    param_2->f_111 = param_2->f_110;
    sVar16._0_1_ = param_2->f_0fa;
    sVar16._1_1_ = param_2->f_0fb;
    if (sVar16 != sVar15) {
      sVar17._0_1_ = param_2->f_0fa;
      sVar17._1_1_ = param_2->f_0fb;
      _ChangeButtonColor(param_2,(int)sVar17);
    }
    iVar24._0_1_ = param_2->f_18c;
    iVar24._1_1_ = param_2->f_18d;
    iVar24._2_1_ = param_2->f_18e;
    iVar24._3_1_ = param_2->f_18f;
    if (iVar24 != 0) {
      sVar18._0_1_ = param_2->f_0fa;
      sVar18._1_1_ = param_2->f_0fb;
      sVar19._0_1_ = param_2->f_0fc;
      sVar19._1_1_ = param_2->f_0fd;
      (**(code **)&param_2->f_18c)(param_2,(int)sVar19,(int)sVar18,(int)sVar15);
    }
    if (sVar15 == 2) {
      sVar20._0_1_ = param_2->f_14c;
      sVar20._1_1_ = param_2->f_14d;
      sVar21._0_1_ = param_2->f_14a;
      sVar21._1_1_ = param_2->f_14b;
      _SWOffsetSprite(param_2,(float)-(int)sVar21,(float)-(int)sVar20);
    }
  }
  else if (param_4 == '\0') {
    if (*(int *)PTR__curHilightedSpriteP_001c42f4 == 0) {
      *(SpritePtr *)PTR__curHilightedSpriteP_001c42f4 = param_2;
      _UnhighlightAllOtherButtonSprites(param_1,param_2,0);
      sVar8._0_1_ = param_2->f_0fa;
      sVar8._1_1_ = param_2->f_0fb;
      param_2->f_0fa = '\x01';
      param_2->f_0fb = '\0';
      param_2->f_110 = param_2->f_111;
      _SetCorrectCheckboxFrame(param_2,1);
      sVar9._0_1_ = param_2->f_0fa;
      sVar9._1_1_ = param_2->f_0fb;
      if (sVar9 != sVar8) {
        sVar10._0_1_ = param_2->f_0fa;
        sVar10._1_1_ = param_2->f_0fb;
        _ChangeButtonColor(param_2,(int)sVar10);
      }
      iVar23._0_1_ = param_2->f_18c;
      iVar23._1_1_ = param_2->f_18d;
      iVar23._2_1_ = param_2->f_18e;
      iVar23._3_1_ = param_2->f_18f;
      if (iVar23 != 0) {
        sVar11._0_1_ = param_2->f_0fa;
        sVar11._1_1_ = param_2->f_0fb;
        sVar12._0_1_ = param_2->f_0fc;
        sVar12._1_1_ = param_2->f_0fd;
        (**(code **)&param_2->f_18c)(param_2,(int)sVar12,(int)sVar11,(int)sVar8);
      }
      if (sVar8 == 2) {
        sVar13._0_1_ = param_2->f_14c;
        sVar13._1_1_ = param_2->f_14d;
        sVar14._0_1_ = param_2->f_14a;
        sVar14._1_1_ = param_2->f_14b;
        _SWOffsetSprite(param_2,(float)-(int)sVar14,(float)-(int)sVar13);
        *param_5 = *(undefined2 *)&param_2->f_0fc;
        if (param_6 != (undefined4 *)0x0) {
          *param_6 = param_2;
        }
      }
    }
  }
  else {
    local_e._0_1_ = param_2->f_0fa;
    local_e._1_1_ = param_2->f_0fb;
    if ((((*(int *)PTR__prevSelectedSpriteP_001c42e0 == 0) &&
         (*PTR__prevButtonWasDown_001c4354 == '\0')) ||
        (param_2 == *(SpritePtr *)PTR__prevSelectedSpriteP_001c42e0)) &&
       (*(SpritePtr *)PTR__prevSelectedSpriteP_001c42e0 = param_2, sVar1._0_1_ = param_2->f_0fa,
       sVar1._1_1_ = param_2->f_0fb, sVar1 != 2)) {
      local_e._0_1_ = param_2->f_0fa;
      local_e._1_1_ = param_2->f_0fb;
      param_2->f_0fa = '\x02';
      param_2->f_0fb = '\0';
      param_2->f_110 = param_2->f_110 == '\0';
      _SetCorrectCheckboxFrame(param_2,1);
      param_2->f_111 = param_2->f_110;
      param_2->f_110 = param_2->f_110 == '\0';
      sVar2._0_1_ = param_2->f_14c;
      sVar2._1_1_ = param_2->f_14d;
      sVar3._0_1_ = param_2->f_14a;
      sVar3._1_1_ = param_2->f_14b;
      _SWOffsetSprite(param_2,(float)(int)sVar3,(float)(int)sVar2);
    }
    sVar4._0_1_ = param_2->f_0fa;
    sVar4._1_1_ = param_2->f_0fb;
    if (sVar4 != local_e) {
      sVar5._0_1_ = param_2->f_0fa;
      sVar5._1_1_ = param_2->f_0fb;
      _ChangeButtonColor(param_2,(int)sVar5);
    }
    iVar22._0_1_ = param_2->f_18c;
    iVar22._1_1_ = param_2->f_18d;
    iVar22._2_1_ = param_2->f_18e;
    iVar22._3_1_ = param_2->f_18f;
    if (iVar22 != 0) {
      sVar6._0_1_ = param_2->f_0fa;
      sVar6._1_1_ = param_2->f_0fb;
      sVar7._0_1_ = param_2->f_0fc;
      sVar7._1_1_ = param_2->f_0fd;
      (**(code **)&param_2->f_18c)(param_2,(int)sVar7,(int)sVar6,(int)local_e);
    }
  }
  return;
}


// ---- _UnhighlightAllOtherButtonSprites @ 0x114be (129 bytes)

void _UnhighlightAllOtherButtonSprites(SpriteLayerPtr param_1,SpritePtr param_2,short param_3)

{
  short sVar1;
  SpritePtr local_14;
  
  for (local_14 = _SWGetNextSprite(param_1,(SpritePtr)0x0); local_14 != (SpritePtr)0x0;
      local_14 = _SWGetNextSprite(param_1,local_14)) {
    if (((local_14 != param_2) && (local_14->userData == 0x149f)) &&
       (sVar1._0_1_ = local_14->f_0f8, sVar1._1_1_ = local_14->f_0f9, sVar1 == param_3)) {
      _DeselectButton(local_14);
    }
  }
  return;
}


// ---- _DeselectButton @ 0x1153f (341 bytes)

void _DeselectButton(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  short sVar7;
  short sVar8;
  int iVar9;
  
  if (param_1 != (SpritePtr)0x0) {
    if (param_1->userData != 0x149f) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
                 PTR____FUNCTION___23415_001c42e8,0x419);
    }
    if (param_1->f_148 == '\0') {
      sVar1._0_1_ = param_1->f_0fa;
      sVar1._1_1_ = param_1->f_0fb;
      param_1->f_0fa = '\0';
      param_1->f_0fb = '\0';
      sVar2._0_1_ = param_1->f_0f8;
      sVar2._1_1_ = param_1->f_0f9;
      if (sVar2 == 2) {
        _SetKeyInputBoxSelected(param_1,0);
      }
      sVar3._0_1_ = param_1->f_0fe;
      sVar3._1_1_ = param_1->f_0ff;
      if (param_1->curFrameIndex != (int)sVar3) {
        sVar4._0_1_ = param_1->f_0fe;
        sVar4._1_1_ = param_1->f_0ff;
        _SWSetCurrentFrameIndex(param_1,(int)sVar4);
      }
      sVar5._0_1_ = param_1->f_0fa;
      sVar5._1_1_ = param_1->f_0fb;
      if (sVar5 != sVar1) {
        sVar6._0_1_ = param_1->f_0fa;
        sVar6._1_1_ = param_1->f_0fb;
        _ChangeButtonColor(param_1,(int)sVar6);
      }
      iVar9._0_1_ = param_1->f_18c;
      iVar9._1_1_ = param_1->f_18d;
      iVar9._2_1_ = param_1->f_18e;
      iVar9._3_1_ = param_1->f_18f;
      if (iVar9 != 0) {
        sVar7._0_1_ = param_1->f_0fa;
        sVar7._1_1_ = param_1->f_0fb;
        sVar8._0_1_ = param_1->f_0fc;
        sVar8._1_1_ = param_1->f_0fd;
        (**(code **)&param_1->f_18c)(param_1,(int)sVar8,(int)sVar7,(int)sVar1);
      }
    }
  }
  return;
}


// ---- _ChangeButtonColor @ 0x11694 (620 bytes)

void _ChangeButtonColor(SpritePtr param_1,short param_2)

{
  ushort blue;
  ushort green;
  ushort red;
  ushort blue_00;
  ushort green_00;
  ushort red_00;
  ushort blue_01;
  ushort green_01;
  ushort red_01;
  ushort blue_02;
  ushort green_02;
  ushort red_02;
  ushort blue_03;
  ushort green_03;
  ushort red_03;
  ushort blue_04;
  ushort green_04;
  ushort red_04;
  int iVar1;
  int iVar2;
  int iVar3;
  
  if (param_1->userData != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___23454_001c4350,0x43a);
  }
  if (param_2 == 2) {
    if (param_1->f_128 != '\0') {
      blue._0_1_ = param_1->f_12e;
      blue._1_1_ = param_1->f_12f;
      green._0_1_ = param_1->f_12c;
      green._1_1_ = param_1->f_12d;
      red._0_1_ = param_1->f_12a;
      red._1_1_ = param_1->f_12b;
      _SWSetSpriteColor(param_1,red,green,blue);
      iVar1._0_1_ = param_1->f_114;
      iVar1._1_1_ = param_1->f_115;
      iVar1._2_1_ = param_1->f_116;
      iVar1._3_1_ = param_1->f_117;
      if (iVar1 != 0) {
        blue_00._0_1_ = param_1->f_12e;
        blue_00._1_1_ = param_1->f_12f;
        green_00._0_1_ = param_1->f_12c;
        green_00._1_1_ = param_1->f_12d;
        red_00._0_1_ = param_1->f_12a;
        red_00._1_1_ = param_1->f_12b;
        _SWSetSpriteColor(*(SpritePtr *)&param_1->f_114,red_00,green_00,blue_00);
      }
    }
  }
  else if (param_2 == 1) {
    if (param_1->f_120 != '\0') {
      blue_01._0_1_ = param_1->f_126;
      blue_01._1_1_ = param_1->f_127;
      green_01._0_1_ = param_1->f_124;
      green_01._1_1_ = param_1->f_125;
      red_01._0_1_ = param_1->f_122;
      red_01._1_1_ = param_1->f_123;
      _SWSetSpriteColor(param_1,red_01,green_01,blue_01);
      iVar2._0_1_ = param_1->f_114;
      iVar2._1_1_ = param_1->f_115;
      iVar2._2_1_ = param_1->f_116;
      iVar2._3_1_ = param_1->f_117;
      if (iVar2 != 0) {
        blue_02._0_1_ = param_1->f_126;
        blue_02._1_1_ = param_1->f_127;
        green_02._0_1_ = param_1->f_124;
        green_02._1_1_ = param_1->f_125;
        red_02._0_1_ = param_1->f_122;
        red_02._1_1_ = param_1->f_123;
        _SWSetSpriteColor(*(SpritePtr *)&param_1->f_114,red_02,green_02,blue_02);
      }
    }
  }
  else if (param_1->f_118 != '\0') {
    blue_03._0_1_ = param_1->f_11e;
    blue_03._1_1_ = param_1->f_11f;
    green_03._0_1_ = param_1->f_11c;
    green_03._1_1_ = param_1->f_11d;
    red_03._0_1_ = param_1->f_11a;
    red_03._1_1_ = param_1->f_11b;
    _SWSetSpriteColor(param_1,red_03,green_03,blue_03);
    iVar3._0_1_ = param_1->f_114;
    iVar3._1_1_ = param_1->f_115;
    iVar3._2_1_ = param_1->f_116;
    iVar3._3_1_ = param_1->f_117;
    if (iVar3 != 0) {
      blue_04._0_1_ = param_1->f_11e;
      blue_04._1_1_ = param_1->f_11f;
      green_04._0_1_ = param_1->f_11c;
      green_04._1_1_ = param_1->f_11d;
      red_04._0_1_ = param_1->f_11a;
      red_04._1_1_ = param_1->f_11b;
      _SWSetSpriteColor(*(SpritePtr *)&param_1->f_114,red_04,green_04,blue_04);
    }
  }
  return;
}


// ---- _SetCorrectCheckboxFrame @ 0x11900 (170 bytes)

void _SetCorrectCheckboxFrame(SpritePtr param_1,char param_2)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  
  if (param_2 == '\0') {
    if (param_1->f_110 == '\0') {
      sVar4._0_1_ = param_1->f_108;
      sVar4._1_1_ = param_1->f_109;
      _SWSetCurrentFrameIndex(param_1,(int)sVar4);
    }
    else {
      sVar3._0_1_ = param_1->f_10c;
      sVar3._1_1_ = param_1->f_10d;
      _SWSetCurrentFrameIndex(param_1,(int)sVar3);
    }
  }
  else if (param_1->f_110 == '\0') {
    sVar2._0_1_ = param_1->f_10a;
    sVar2._1_1_ = param_1->f_10b;
    _SWSetCurrentFrameIndex(param_1,(int)sVar2);
  }
  else {
    sVar1._0_1_ = param_1->f_10e;
    sVar1._1_1_ = param_1->f_10f;
    _SWSetCurrentFrameIndex(param_1,(int)sVar1);
  }
  return;
}


// ---- _DisposeButtonSpritesMatchingIndex @ 0x119aa (193 bytes)

void _DisposeButtonSpritesMatchingIndex(SpriteLayerPtr param_1,short param_2)

{
  short sVar1;
  SpritePtr pSVar2;
  SpritePtr local_18;
  
  if (param_1 == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___23552_001c4348,0x47c);
  }
  pSVar2 = _SWGetNextSprite(param_1,(SpritePtr)0x0);
  while (local_18 = pSVar2, local_18 != (SpritePtr)0x0) {
    pSVar2 = _SWGetNextSprite(param_1,local_18);
    if ((local_18->userData == 0x149f) &&
       (sVar1._0_1_ = local_18->f_0fc, sVar1._1_1_ = local_18->f_0fd, sVar1 == param_2)) {
      _SWRemoveSpriteFromAnimation(local_18,'\x01');
    }
  }
  return;
}


// ---- _ProcessKeyInputBoxSprite @ 0x11a6b (937 bytes)

void _ProcessKeyInputBoxSprite
               (undefined4 param_1,SpritePtr param_2,char param_3,char param_4,undefined2 *param_5,
               undefined4 *param_6)

{
  short sVar1;
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
  int iVar24;
  short local_e;
  
  if (param_2->f_148 == '\0') {
    if (param_3 == '\0') {
      if (param_2->f_130 == '\0') {
        sVar15._0_1_ = param_2->f_0fa;
        sVar15._1_1_ = param_2->f_0fb;
        param_2->f_0fa = '\0';
        param_2->f_0fb = '\0';
        sVar16._0_1_ = param_2->f_0fa;
        sVar16._1_1_ = param_2->f_0fb;
        if (sVar16 != sVar15) {
          sVar17._0_1_ = param_2->f_0fa;
          sVar17._1_1_ = param_2->f_0fb;
          _ChangeButtonColor(param_2,(int)sVar17);
        }
        iVar24._0_1_ = param_2->f_18c;
        iVar24._1_1_ = param_2->f_18d;
        iVar24._2_1_ = param_2->f_18e;
        iVar24._3_1_ = param_2->f_18f;
        if (iVar24 != 0) {
          sVar18._0_1_ = param_2->f_0fa;
          sVar18._1_1_ = param_2->f_0fb;
          sVar19._0_1_ = param_2->f_0fc;
          sVar19._1_1_ = param_2->f_0fd;
          (**(code **)&param_2->f_18c)(param_2,(int)sVar19,(int)sVar18,(int)sVar15);
        }
        if (sVar15 == 2) {
          sVar20._0_1_ = param_2->f_14c;
          sVar20._1_1_ = param_2->f_14d;
          sVar21._0_1_ = param_2->f_14a;
          sVar21._1_1_ = param_2->f_14b;
          _SWOffsetSprite(param_2,(float)-(int)sVar21,(float)-(int)sVar20);
        }
      }
    }
    else if (param_4 == '\0') {
      if (*(int *)PTR__curHilightedSpriteP_001c42f4 == 0) {
        *(SpritePtr *)PTR__curHilightedSpriteP_001c42f4 = param_2;
        _UnhighlightAllOtherButtonSprites(param_1,param_2,0);
        sVar8._0_1_ = param_2->f_0fa;
        sVar8._1_1_ = param_2->f_0fb;
        param_2->f_0fa = '\x01';
        param_2->f_0fb = '\0';
        sVar9._0_1_ = param_2->f_0fa;
        sVar9._1_1_ = param_2->f_0fb;
        if (sVar9 != sVar8) {
          sVar10._0_1_ = param_2->f_0fa;
          sVar10._1_1_ = param_2->f_0fb;
          _ChangeButtonColor(param_2,(int)sVar10);
        }
        iVar23._0_1_ = param_2->f_18c;
        iVar23._1_1_ = param_2->f_18d;
        iVar23._2_1_ = param_2->f_18e;
        iVar23._3_1_ = param_2->f_18f;
        if (iVar23 != 0) {
          sVar11._0_1_ = param_2->f_0fa;
          sVar11._1_1_ = param_2->f_0fb;
          sVar12._0_1_ = param_2->f_0fc;
          sVar12._1_1_ = param_2->f_0fd;
          (**(code **)&param_2->f_18c)(param_2,(int)sVar12,(int)sVar11,(int)sVar8);
        }
        if (sVar8 == 2) {
          sVar13._0_1_ = param_2->f_14c;
          sVar13._1_1_ = param_2->f_14d;
          sVar14._0_1_ = param_2->f_14a;
          sVar14._1_1_ = param_2->f_14b;
          _SWOffsetSprite(param_2,(float)-(int)sVar14,(float)-(int)sVar13);
          _UnhighlightAllOtherButtonSprites(param_1,param_2,2);
          _SetKeyInputBoxSelected(param_2,1);
          *param_5 = *(undefined2 *)&param_2->f_0fc;
          if (param_6 != (undefined4 *)0x0) {
            *param_6 = param_2;
          }
        }
      }
    }
    else {
      local_e._0_1_ = param_2->f_0fa;
      local_e._1_1_ = param_2->f_0fb;
      if ((((*(int *)PTR__prevSelectedSpriteP_001c42e0 == 0) &&
           (*PTR__prevButtonWasDown_001c4354 == '\0')) ||
          (param_2 == *(SpritePtr *)PTR__prevSelectedSpriteP_001c42e0)) &&
         (*(SpritePtr *)PTR__prevSelectedSpriteP_001c42e0 = param_2, sVar1._0_1_ = param_2->f_0fa,
         sVar1._1_1_ = param_2->f_0fb, sVar1 != 2)) {
        local_e._0_1_ = param_2->f_0fa;
        local_e._1_1_ = param_2->f_0fb;
        param_2->f_0fa = '\x02';
        param_2->f_0fb = '\0';
        sVar2._0_1_ = param_2->f_14c;
        sVar2._1_1_ = param_2->f_14d;
        sVar3._0_1_ = param_2->f_14a;
        sVar3._1_1_ = param_2->f_14b;
        _SWOffsetSprite(param_2,(float)(int)sVar3,(float)(int)sVar2);
      }
      sVar4._0_1_ = param_2->f_0fa;
      sVar4._1_1_ = param_2->f_0fb;
      if (sVar4 != local_e) {
        sVar5._0_1_ = param_2->f_0fa;
        sVar5._1_1_ = param_2->f_0fb;
        _ChangeButtonColor(param_2,(int)sVar5);
      }
      iVar22._0_1_ = param_2->f_18c;
      iVar22._1_1_ = param_2->f_18d;
      iVar22._2_1_ = param_2->f_18e;
      iVar22._3_1_ = param_2->f_18f;
      if (iVar22 != 0) {
        sVar6._0_1_ = param_2->f_0fa;
        sVar6._1_1_ = param_2->f_0fb;
        sVar7._0_1_ = param_2->f_0fc;
        sVar7._1_1_ = param_2->f_0fd;
        (**(code **)&param_2->f_18c)(param_2,(int)sVar7,(int)sVar6,(int)local_e);
      }
    }
  }
  return;
}


// ---- _HandleEventInKeyInputBox @ 0x11e14 (529 bytes)

undefined4 HandleEventInKeyInputBox(int *param_1)

{
  undefined1 auStack_448 [1010];
  char acStack_56 [50];
  int iStack_24;
  int iStack_20;
  int iStack_1c;
  short sStack_16;
  int iStack_14;
  char *pcStack_10;
  
  if (((*(int *)PTR__prevButtonFirstSpriteLayerP_001c4358 != 0) && (*param_1 == 0x300)) &&
     (iStack_24 = _GetCurrentlySelectedKeyInputBox(), iStack_24 != 0)) {
    iStack_14 = iStack_24;
    if (*(char *)(iStack_24 + 0x16c) == '\0') {
      iStack_20 = param_1[3];
    }
    else {
      iStack_20 = _ConvertKeyScanCode(param_1[3]);
    }
    iStack_1c = param_1[4];
    if (((iStack_20 != 0x39) && ((*(char *)(iStack_14 + 0x146) == '\0' || (iStack_20 != 0xe3)))) &&
       ((*(char *)(iStack_14 + 0x147) == '\0' || (iStack_20 != 0x29)))) {
      *(int *)(iStack_14 + 0x134) = iStack_20;
      *(int *)(iStack_14 + 0x138) = iStack_1c;
      pcStack_10 = (char *)_SDL_GetKeyName(param_1[4]);
      pcStack_10 = (char *)_ConvertKeyName(pcStack_10);
      _strcpy(acStack_56,pcStack_10);
      if (*(int *)(*(int *)(iStack_14 + 0x114) + 0xe8) == 0x7edf) {
        _SetBMTextSpriteText(*(undefined4 *)(iStack_14 + 0x114),acStack_56);
      }
      else if (*(int *)(*(int *)(iStack_14 + 0x114) + 0xe8) == 0x8dac7c) {
        _SetTextSpriteText(*(undefined4 *)(iStack_14 + 0x114),acStack_56);
      }
      _strcpy((char *)(iStack_14 + 0x13c),acStack_56);
      sStack_16 = _SDL_PeepEvents(auStack_448,0x14,1,0x300,0x301);
      _printf("numEvents left in queue: %d\n",(int)sStack_16);
      if (sStack_16 == 0) {
        _SetKeyInputBoxSelected(iStack_24,0);
      }
    }
    return 1;
  }
  return 0;
}


// ---- _DoesEventExist @ 0x12025 (79 bytes)

bool DoesEventExist(undefined4 param_1)

{
  int iVar1;
  undefined1 auStack_400 [1020];
  
  _SDL_PumpEvents();
  iVar1 = _SDL_PeepEvents(auStack_400,0x14,1,param_1,param_1);
  return 0 < iVar1;
}


// ---- _GetCurrentlySelectedKeyInputBox @ 0x12074 (198 bytes)

SpritePtr _GetCurrentlySelectedKeyInputBox(void)

{
  short sVar1;
  SpriteLayerPtr local_18;
  SpritePtr local_14;
  
  local_18 = *(SpriteLayerPtr *)PTR__prevButtonFirstSpriteLayerP_001c4358;
  do {
    if (local_18 == (SpriteLayerPtr)0x0) {
      return (SpritePtr)0x0;
    }
    for (local_14 = _SWGetNextSprite(local_18,(SpritePtr)0x0); local_14 != (SpritePtr)0x0;
        local_14 = _SWGetNextSprite(local_18,local_14)) {
      if (((local_14->userData == 0x149f) &&
          (sVar1._0_1_ = local_14->f_0f8, sVar1._1_1_ = local_14->f_0f9, sVar1 == 2)) &&
         (local_14->f_130 != '\0')) {
        return local_14;
      }
    }
    if (local_18 == *(SpriteLayerPtr *)PTR__prevButtonLastSpriteLayerP_001c4320) {
      return (SpritePtr)0x0;
    }
    local_18 = local_18->nextSpriteLayerP;
  } while( true );
}


// ---- _ConvertKeyScanCode @ 0x1213a (95 bytes)

int _ConvertKeyScanCode(int param_1)

{
  undefined4 local_10;
  
  if (param_1 == 0xe4) {
    local_10 = 0xe0;
  }
  else if (param_1 == 0xe5) {
    local_10 = 0xe1;
  }
  else if (param_1 == 0xe6) {
    local_10 = 0xe2;
  }
  else if (param_1 == 0xe7) {
    local_10 = 0xe3;
  }
  else {
    local_10 = param_1;
  }
  return local_10;
}


// ---- _ConvertKeyName @ 0x12199 (284 bytes)

char * _ConvertKeyName(char *param_1)

{
  char *pcVar1;
  
  pcVar1 = _strstr(param_1,"Left Shift");
  if ((pcVar1 != (char *)0x0) || (pcVar1 = _strstr(param_1,"Right Shift"), pcVar1 != (char *)0x0)) {
    return "Shift";
  }
  pcVar1 = _strstr(param_1,"Left Ctrl");
  if ((pcVar1 != (char *)0x0) || (pcVar1 = _strstr(param_1,"Right Ctrl"), pcVar1 != (char *)0x0)) {
    return "Control";
  }
  pcVar1 = _strstr(param_1,"Left Option");
  if ((pcVar1 != (char *)0x0) || (pcVar1 = _strstr(param_1,"Right Option"), pcVar1 != (char *)0x0))
  {
    return "Option";
  }
  pcVar1 = _strstr(param_1,"Left Alt");
  if ((pcVar1 == (char *)0x0) && (pcVar1 = _strstr(param_1,"Right Alt"), pcVar1 == (char *)0x0)) {
    return param_1;
  }
  return "Alt";
}


// ---- _SetKeyInputBoxSelected @ 0x122b5 (167 bytes)

void _SetKeyInputBoxSelected(SpritePtr param_1,uchar param_2)

{
  short sVar1;
  short sVar2;
  
  if (param_1->userData != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___23806_001c4300,0x5d2);
  }
  if (param_2 == '\0') {
    sVar2._0_1_ = param_1->f_0fe;
    sVar2._1_1_ = param_1->f_0ff;
    _SWSetCurrentFrameIndex(param_1,(int)sVar2);
  }
  else {
    sVar1._0_1_ = param_1->f_102;
    sVar1._1_1_ = param_1->f_103;
    _SWSetCurrentFrameIndex(param_1,(int)sVar1);
  }
  param_1->f_130 = param_2;
  return;
}


// ---- _CustomButtonDrawProc @ 0x1235c (1133 bytes)

void CustomButtonDrawProc(SpritePtr param_1,FrameRec *param_2,float param_3,float param_4)

{
  short sVar1;
  short sVar2;
  SpritePtr srcSpriteP;
  int iVar3;
  longdouble lVar4;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  longdouble extraout_ST0_01;
  longdouble extraout_ST0_02;
  longdouble extraout_ST0_03;
  longdouble extraout_ST0_04;
  longdouble extraout_ST0_05;
  longdouble extraout_ST0_06;
  short sStack_e;
  
  srcSpriteP = *(SpritePtr *)&param_1->f_114;
  if (param_1->userData != 0x149f) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___23828_001c435c,0x5fa);
  }
  if ((*(int *)(*(int *)&param_1->f_114 + 0xe8) != 0x7edf) &&
     (*(int *)(*(int *)&param_1->f_114 + 0xe8) != 0x8dac7c)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___23828_001c435c,0x5fb);
  }
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
               PTR____FUNCTION___23828_001c435c,0x5fc);
  }
  if (srcSpriteP->userData == 0x7edf) {
    lVar4 = (longdouble)_GetWidthOfBMText(srcSpriteP,&srcSpriteP->f_0f8);
    sStack_e = (short)(int)lVar4;
  }
  else {
    sStack_e = _GetTextSpriteWidth(srcSpriteP);
  }
  sVar1._0_1_ = param_1->f_0f8;
  sVar1._1_1_ = param_1->f_0f9;
  if (sVar1 == 1) {
    _SWGetSpriteWidth(param_1);
    (param_1->destFrameRect).right =
         (param_1->destFrameRect).left + (short)(int)((float)(int)sStack_e + (float)extraout_ST0);
    _SWStdSpriteDrawProc(param_1,param_2,param_3,param_4);
    _SWGetSpriteHeight(param_1);
    _SWGetSpriteWidth(param_1);
    _BMTextSpriteDrawProc
              (srcSpriteP,param_2,(float)extraout_ST0_01 + param_3 + 3.0,
               (float)extraout_ST0_00 * 0.17 + param_4);
  }
  else {
    sVar2._0_1_ = param_1->f_0f8;
    sVar2._1_1_ = param_1->f_0f9;
    if (sVar2 == 2) {
      _SWStdSpriteDrawProc(param_1,param_2,param_3,param_4);
      if (srcSpriteP->userData == 0x7edf) {
        _SWGetSpriteHeight(param_1);
        _SWGetSpriteHeight(srcSpriteP);
        _SWGetSpriteWidth(param_1);
        _BMTextSpriteDrawProc
                  (srcSpriteP,param_2,
                   ((float)extraout_ST0_04 / 2.0 + param_3) - (float)(int)(sStack_e / 2),
                   (float)extraout_ST0_02 / 2.0 + param_4 + (float)extraout_ST0_03 / -2.0);
      }
      else if (srcSpriteP->userData == 0x8dac7c) {
        _SWGetSpriteHeight(param_1);
        iVar3 = srcSpriteP[1].misc2;
        _SWGetSpriteWidth(param_1);
        _SWSetSpriteLocation
                  (srcSpriteP,((float)extraout_ST0_06 / 2.0 + param_3) - (float)(int)(sStack_e / 2),
                   (float)(int)(short)((short)iVar3 / 2) + param_4 + (float)extraout_ST0_05 / 2.5);
        _SWTextSpriteDrawProc(srcSpriteP,param_2,0,0);
      }
    }
    else {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/MyButtonSprite.c",
                 PTR____FUNCTION___23828_001c435c,0x62a);
    }
  }
  return;
}


// ---- _GetTextFromSDLKeyCode @ 0x127c9 (217 bytes)

char * _GetTextFromSDLKeyCode(int param_1)

{
  char *local_20;
  int local_10;
  
  *PTR__charCString_001c4334 = 0;
  if ((param_1 < 0x61) || (0x7a < param_1)) {
    if ((param_1 < 0x31) || (0x39 < param_1)) {
      if (param_1 == 0x30) {
        local_20 = "0";
      }
      else {
        for (local_10 = 0; *(int *)(PTR__gKeyCodeArray_001c431c + local_10 * 4) != -1;
            local_10 = local_10 + 1) {
          if (*(int *)(PTR__gKeyCodeArray_001c431c + local_10 * 4) == param_1) {
            return *(char **)(PTR__gKeyCodeTextArray_001c4330 + local_10 * 4);
          }
        }
        local_20 = "Unknown";
      }
    }
    else {
      *PTR__charCString_001c4334 = (char)param_1;
      local_20 = PTR__charCString_001c4334;
    }
  }
  else {
    *PTR__charCString_001c4334 = (char)param_1 + -0x20;
    local_20 = PTR__charCString_001c4334;
  }
  return local_20;
}


