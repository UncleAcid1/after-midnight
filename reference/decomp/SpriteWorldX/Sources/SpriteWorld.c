// Ghidra decompilation of functions from original source file:
//   ../SpriteWorldX/Sources/SpriteWorld.c

// ---- _SWEnterSpriteWorld @ 0x2590a (138 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWEnterSpriteWorld(SWBoolean shouldInitSDL)

{
  int iVar1;
  undefined4 uVar2;
  SWError local_10;
  
  local_10 = 0;
  if (shouldInitSDL != '\0') {
    iVar1 = _SDL_Init(0x21);
    if (iVar1 < 0) {
      uVar2 = _SDL_GetError();
      _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"SDL_Init: %s\n",uVar2);
      local_10 = 300;
    }
  }
  *PTR__gSWInitializedSDL_001c46d4 = shouldInitSDL;
  _SWSetStickyIfError(local_10);
  return local_10;
}


// ---- _SWExitSpriteWorld @ 0x25994 (45 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWExitSpriteWorld(void)

{
  if (*PTR__gSWInitializedSDL_001c46d4 != '\0') {
    _SDL_Quit();
  }
  return;
}


// ---- _SWCreateSpriteWorld @ 0x259c1 (471 bytes)

SWError _SWCreateSpriteWorld
                  (SpriteWorldPtr *param_1,undefined4 param_2,short param_3,short param_4,
                  undefined4 param_5,undefined4 param_6,short param_7,char param_8,SWBoolean param_9
                  )

{
  uint uVar1;
  void *mainWindow;
  char *pcVar2;
  undefined4 *puVar3;
  SWError local_40;
  uint local_14;
  
  *param_1 = (SpriteWorldPtr)0x0;
  local_14 = 4;
  if (param_8 != '\0') {
    local_14 = 5;
  }
  uVar1 = local_14 | 0x20;
  if (param_9 != '\0') {
    _BKLoadGLfunctions();
    *PTR__gSWUseOpenGL_001c45e0 = 1;
    _SDL_GL_SetAttribute(0x11,3);
    _SDL_GL_SetAttribute(0x12,2);
    _SDL_GL_SetAttribute(5,1);
    _SDL_GL_SetAttribute(6,0x18);
    uVar1 = local_14 | 0x22;
  }
  local_14 = uVar1;
  mainWindow = (void *)_SDL_CreateWindow(param_2,0x2fff0000,0x2fff0000,param_5,param_6,local_14);
  if (mainWindow == (void *)0x0) {
    local_40 = -1;
  }
  else {
    pcVar2 = (char *)_SDL_GetError();
    if (*pcVar2 != '\0') {
      _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"SDL_VideoMode: %s\n",pcVar2);
    }
    local_40 = _SWCreateSpriteWorldFromSDLWindow
                         (param_1,mainWindow,param_3,(int)param_4,(int)param_7,param_9,(SWRect *)0x0
                          ,(SWRect *)0x0,0);
    puVar3 = (undefined4 *)_BKLoadGLcapabilities();
    if (puVar3 != (undefined4 *)0x0) {
      *(undefined4 *)PTR__gSWTexExtension_001c4550 = *puVar3;
    }
    _SWSetStickyIfError(local_40);
  }
  return local_40;
}


// ---- __setupGL @ 0x25b98 (286 bytes)

void __setupGL(SpriteWorldRec *param_1,int param_2,int param_3)

{
  void *pvVar1;
  
  pvVar1 = _BKLoadGLfunctions();
  if (pvVar1 == (void *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___20962_001c467c,0xd5);
  }
  _SWCalculateViewport(param_1,param_2,param_3);
  _SWResetGLSetup(param_1);
  _glMatrixMode(0x1701);
  _glLoadIdentity();
  (**(code **)((int)pvVar1 + 0x4c))(0x4000);
  _glOrtho(0,(double)param_2,(double)param_3,0,0xbff0000000000000,0x3ff0000000000000);
  (**(code **)((int)pvVar1 + 0x90))("/Versions/A/SDL");
  (**(code **)((int)pvVar1 + 0x18))(0xb71);
  (**(code **)((int)pvVar1 + 0x18))(0xbe2);
  return;
}


// ---- _SWResetGLSetup @ 0x25cb6 (77 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWResetGLSetup(SpriteWorldRec *spriteWorldP)

{
  _glViewport((int)spriteWorldP->viewportX,(int)spriteWorldP->viewportY,
              (int)spriteWorldP->viewportWidth,(int)spriteWorldP->viewportHeight);
  return;
}


// ---- _SWSetSpriteWorldZoom @ 0x25d03 (38 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteWorldZoom(SpriteWorldRec *spriteWorldP,float zoomH,float zoomV)

{
  spriteWorldP->zoomH = zoomH;
  spriteWorldP->zoomV = zoomV;
  return;
}


// ---- _SWCalculateViewport @ 0x25d29 (881 bytes)

void _SWCalculateViewport(int param_1,int param_2,int param_3)

{
  short sVar1;
  short sVar3;
  short sVar4;
  float fVar5;
  undefined4 local_20;
  undefined4 local_1c;
  undefined2 local_14;
  undefined2 local_12;
  short sVar2;
  
  local_20 = 1.0;
  local_1c = 1.0;
  if (*(short *)(param_1 + 0x38) != -1) {
    local_20 = (float)param_2 / (float)(int)*(short *)(param_1 + 0x38);
  }
  if (*(short *)(param_1 + 0x3a) != -1) {
    local_1c = (float)param_3 / (float)(int)*(short *)(param_1 + 0x3a);
  }
  fVar5 = (float)((int)*(short *)(param_1 + 0x62) - (int)*(short *)(param_1 + 0x5e)) /
          (float)((int)*(short *)(param_1 + 0x60) - (int)*(short *)(param_1 + 0x5c));
  sVar1 = (short)param_2;
  sVar2 = (short)param_3;
  if (fVar5 < (float)param_2 / (float)param_3) {
    local_14 = (short)(int)((float)param_3 * fVar5);
    local_12 = sVar2;
  }
  else {
    local_14 = sVar1;
    local_12 = (short)(int)((float)param_2 / fVar5);
  }
  if (*(char *)(param_1 + 0x4d) != '\0') {
    local_14 = sVar1;
    local_12 = sVar2;
  }
  sVar3 = (short)((param_2 - local_14) / 2);
  sVar4 = (short)((param_3 - local_12) / 2);
  *(short *)(param_1 + 0x28) = local_14;
  *(short *)(param_1 + 0x2a) = local_12;
  *(short *)(param_1 + 0x2c) = sVar3;
  *(short *)(param_1 + 0x2e) = sVar4;
  *(float *)(param_1 + 0x3c) = local_20;
  *(float *)(param_1 + 0x40) = local_1c;
  *(float *)(param_1 + 0x44) = (float)(int)local_14 / (float)(int)*(short *)(param_1 + 0x38);
  *(float *)(param_1 + 0x48) = (float)(int)local_12 / (float)(int)*(short *)(param_1 + 0x3a);
  if (*(char *)(param_1 + 0x4c) == '\0') {
    *(undefined2 *)(param_1 + 0x20) = 0;
    *(undefined2 *)(param_1 + 0x22) = 0;
    *(undefined2 *)(param_1 + 0x24) = *(undefined2 *)(param_1 + 0x38);
    *(undefined2 *)(param_1 + 0x26) = *(undefined2 *)(param_1 + 0x3a);
  }
  else {
    *(short *)(param_1 + 0x20) =
         (short)(int)((float)(int)*(short *)(param_1 + 0x76) * local_20 + (float)(int)sVar3);
    *(short *)(param_1 + 0x22) =
         (short)(int)((float)(int)sVar4 -
                     *(float *)(param_1 + 0x48) * (float)(int)*(short *)(param_1 + 0x74));
    *(short *)(param_1 + 0x24) = local_14;
    *(short *)(param_1 + 0x26) = local_12;
  }
  *(short *)(param_1 + 0x30) = *(short *)(param_1 + 0x76) + sVar3;
  *(short *)(param_1 + 0x32) =
       (short)(int)((float)-(int)sVar4 * 0.8 - (float)(int)*(short *)(param_1 + 0x74) * local_1c);
  *(short *)(param_1 + 0x34) = sVar1;
  *(short *)(param_1 + 0x36) = sVar2;
  *(undefined2 *)(param_1 + 0x84) = 0;
  *(undefined2 *)(param_1 + 0x86) = 0;
  *(short *)(param_1 + 0x88) = (short)(int)((float)(int)*(short *)(param_1 + 0x26) * local_1c);
  *(undefined2 *)(param_1 + 0x8a) = *(undefined2 *)(param_1 + 0x24);
  return;
}


// ---- _SWSetSpriteWorldAutoScaling @ 0x2609a (41 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteWorldAutoScaling(SpriteWorldRec *spriteWorldP,uchar autoScale)

{
  spriteWorldP->autoScaling = autoScale;
  _SWUpdateWindowSize(spriteWorldP);
  return;
}


// ---- _SWAutoScaleRect @ 0x260c3 (254 bytes)

void _SWAutoScaleRect(int param_1,short *param_2)

{
  float local_14;
  float local_10;
  
  local_14 = *(float *)(param_1 + 0x3c);
  local_10 = *(float *)(param_1 + 0x40);
  if (*(short *)(param_1 + 0x38) != -1) {
    local_14 = (float)(int)*(short *)(param_1 + 0x28) / (float)(int)*(short *)(param_1 + 0x38);
  }
  if (*(short *)(param_1 + 0x3a) != -1) {
    local_10 = (float)(int)*(short *)(param_1 + 0x2a) / (float)(int)*(short *)(param_1 + 0x3a);
  }
  param_2[3] = (short)(int)((float)(int)param_2[3] * local_14);
  param_2[1] = (short)(int)((float)(int)param_2[1] * local_14);
  *param_2 = (short)(int)((float)(int)*param_2 * local_10);
  param_2[2] = (short)(int)((float)(int)param_2[2] * local_10);
  return;
}


// ---- _SWCreateSpriteWorldFromSDLWindow @ 0x261c1 (722 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWCreateSpriteWorldFromSDLWindow
                  (SpriteWorldPtr *spriteWorldPP,void *mainWindow,short depth,int w,int h,
                  SWBoolean useOpenGL,SWRect *worldRectP,SWRect *backRectP,int maxDepth)

{
  SpriteWorldPtr pSVar1;
  void **ppvVar2;
  short local_48;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  FramePtr local_20;
  FramePtr local_1c;
  int local_18;
  undefined4 local_14;
  void *local_10;
  
  local_48 = (short)h;
  local_18 = 0;
  local_14 = 0;
  *spriteWorldPP = (SpriteWorldPtr)0x0;
  local_20 = (FramePtr)0x0;
  local_1c = (FramePtr)0x0;
  if (worldRectP == (SWRect *)0x0) {
    local_28 = 0;
    local_24 = CONCAT22(depth,(short)w);
  }
  else {
    local_24._0_2_ = worldRectP->bottom;
    local_24._2_2_ = worldRectP->right;
    local_28._0_2_ = worldRectP->top;
    local_28._2_2_ = worldRectP->left;
  }
  if ((maxDepth != 0) && (maxDepth < local_48)) {
    local_48 = (short)maxDepth;
  }
  if (backRectP == (SWRect *)0x0) {
    local_2c = local_24;
  }
  else {
    local_2c._0_2_ = backRectP->bottom;
    local_2c._2_2_ = backRectP->right;
  }
  local_30 = 0;
  if (useOpenGL != '\0') {
    local_10 = (void *)_SDL_GL_CreateContext(mainWindow);
  }
  if ((local_18 == 0) && (useOpenGL == '\0')) {
    local_18 = _SWCreateBlankFrame(&local_1c,(int)local_2c._2_2_,(int)(short)local_2c,
                                   (uchar)local_48,'\0');
  }
  if ((local_18 == 0) && (useOpenGL == '\0')) {
    local_18 = _SWCreateBlankFrame(&local_20,(int)local_2c._2_2_,(int)(short)local_2c,
                                   (uchar)local_48,'\0');
  }
  if (local_18 == 0) {
    local_18 = _SWCreateSpriteWorldFromFrames(spriteWorldPP,local_1c,local_20,&local_30,&local_28);
  }
  if (local_18 == 0) {
    (*spriteWorldPP)->mainWindow = mainWindow;
    (*spriteWorldPP)->glContext = local_10;
    (*spriteWorldPP)->designWidth = depth;
    (*spriteWorldPP)->designHeight = (short)w;
    pSVar1 = *spriteWorldPP;
    ppvVar2 = (void **)_calloc(10,4);
    pSVar1->tileLayerCallbacks = ppvVar2;
  }
  if (local_18 == 0) {
    if (useOpenGL == '\0') {
      (*spriteWorldPP)->useOpenGL = 0;
    }
    else {
      _SDL_GetWindowSize((*spriteWorldPP)->mainWindow,&local_34,&local_38);
      __setupGL(*spriteWorldPP,local_34,local_38);
      (*spriteWorldPP)->useOpenGL = 1;
    }
    _printf("OpenGL: %d\n",(*spriteWorldPP)->useOpenGL);
  }
  if (local_18 != 0) {
    if (local_1c != (FramePtr)0x0) {
      _SWDisposeFrame(&local_1c);
    }
    if (local_20 != (FramePtr)0x0) {
      _SWDisposeFrame(&local_20);
    }
  }
  _SWSetStickyIfError(local_18);
  return local_18;
}


// ---- _SWCreateAdditionalSpriteWorld @ 0x26493 (300 bytes)

SWError _SWCreateAdditionalSpriteWorld(int *param_1,int param_2,undefined4 param_3)

{
  undefined4 local_18;
  undefined4 local_14;
  int local_10;
  
  local_10 = 0;
  *param_1 = 0;
  local_10 = _SWCreateSpriteWorldFromFrames
                       (param_1,*(undefined4 *)(param_2 + 0x18),*(undefined4 *)(param_2 + 0x14),
                        param_3,param_2 + 0x74);
  if (local_10 == 0) {
    *(undefined4 *)(*param_1 + 0x158) = *(undefined4 *)(param_2 + 0x158);
    *(undefined4 *)(*param_1 + 0x15c) = *(undefined4 *)(param_2 + 0x15c);
    *(undefined2 *)(*param_1 + 0x38) = *(undefined2 *)(param_2 + 0x38);
    *(undefined2 *)(*param_1 + 0x3a) = *(undefined2 *)(param_2 + 0x3a);
    *(undefined4 *)(*param_1 + 0x3c) = *(undefined4 *)(param_2 + 0x3c);
    *(undefined4 *)(*param_1 + 0x40) = *(undefined4 *)(param_2 + 0x40);
    *(undefined4 *)(*param_1 + 0x14c) = *(undefined4 *)(param_2 + 0x14c);
    *(undefined1 *)(*param_1 + 0x145) = 1;
    _SDL_GetWindowSize(*(undefined4 *)(param_2 + 0x158),&local_14,&local_18);
    _SWCalculateViewport(*param_1,local_14,local_18);
  }
  _SWSetStickyIfError(local_10);
  return local_10;
}


// ---- _SWCreateSpriteWorldFromSWSurface @ 0x265bf (512 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError SWCreateSpriteWorldFromSWSurface
                  (SpriteWorldPtr *spriteWorldPP,SDL_Surface *surfaceP,SWRect *worldRectP,
                  SWRect *backRectP,int maxDepth)

{
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  FramePtr pFStack_20;
  FramePtr pFStack_1c;
  SpriteWorldPtr pSStack_18;
  int iStack_14;
  uchar uStack_d;
  
  iStack_14 = 0;
  pSStack_18 = (SpriteWorldPtr)0x0;
  *spriteWorldPP = (SpriteWorldPtr)0x0;
  pFStack_20 = (FramePtr)0x0;
  pFStack_1c = (FramePtr)0x0;
  if (surfaceP == (SDL_Surface *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21284_001c46c8,0x2a8);
  }
  if (worldRectP == (SWRect *)0x0) {
    uStack_28 = 0;
    uStack_24 = CONCAT22((short)surfaceP->w,(short)surfaceP->h);
  }
  else {
    uStack_24._0_2_ = worldRectP->bottom;
    uStack_24._2_2_ = worldRectP->right;
    uStack_28._0_2_ = worldRectP->top;
    uStack_28._2_2_ = worldRectP->left;
  }
  if ((maxDepth == 0) || ((int)(uint)*(byte *)((int)surfaceP->format + 8) <= maxDepth)) {
    uStack_d = *(uchar *)((int)surfaceP->format + 8);
  }
  else {
    uStack_d = (uchar)maxDepth;
  }
  if (backRectP == (SWRect *)0x0) {
    uStack_2c = uStack_24;
  }
  else {
    uStack_2c._0_2_ = backRectP->bottom;
    uStack_2c._2_2_ = backRectP->right;
  }
  uStack_30 = 0;
  if (iStack_14 == 0) {
    iStack_14 = _SWCreateFrameFromSurface(&pFStack_1c,surfaceP,0);
  }
  if (iStack_14 == 0) {
    iStack_14 = _SWCreateBlankFrame(&pFStack_20,(int)uStack_2c._2_2_,(int)(short)uStack_2c,uStack_d,
                                    '\0');
  }
  if (iStack_14 == 0) {
    iStack_14 = _SWCreateSpriteWorldFromFrames
                          (&pSStack_18,pFStack_1c,pFStack_20,&uStack_30,&uStack_28);
  }
  if (iStack_14 == 0) {
    pSStack_18->useOpenGL = 0;
    *spriteWorldPP = pSStack_18;
  }
  else {
    if (pFStack_1c != (FramePtr)0x0) {
      _SWDisposeFrame(&pFStack_1c);
    }
    if (pFStack_20 != (FramePtr)0x0) {
      _SWDisposeFrame(&pFStack_20);
    }
  }
  _SWSetStickyIfError(iStack_14);
  return iStack_14;
}


// ---- _SWCreateSpriteWorldFromFrames @ 0x267bf (1105 bytes)

int _SWCreateSpriteWorldFromFrames
              (undefined4 *param_1,undefined4 param_2,undefined4 param_3,undefined4 *param_4,
              short *param_5)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int local_14;
  
  local_14 = 0;
  puVar2 = (undefined4 *)_calloc(1,0x164);
  if (puVar2 == (undefined4 *)0x0) {
    local_14 = 200;
  }
  if (local_14 == 0) {
    local_14 = _SWCreateSpriteLayer((SpriteLayerPtr *)(puVar2 + 2));
    if (local_14 != 0) {
      _free(puVar2);
    }
  }
  if (local_14 == 0) {
    *puVar2 = 0;
    puVar2[1] = 0;
    puVar2[6] = param_2;
    puVar2[5] = param_3;
    puVar2[7] = 0;
    puVar2[0x14] = 0;
    puVar2[0x15] = 0;
    uVar1 = *(undefined4 *)(param_5 + 2);
    puVar2[0x17] = *(undefined4 *)param_5;
    puVar2[0x18] = uVar1;
    uVar1 = param_4[1];
    puVar2[0x19] = *param_4;
    puVar2[0x1a] = uVar1;
    uVar1 = *(undefined4 *)(param_5 + 2);
    puVar2[0x1d] = *(undefined4 *)param_5;
    puVar2[0x1e] = uVar1;
    uVar1 = param_4[1];
    puVar2[0x1f] = *param_4;
    puVar2[0x20] = uVar1;
    *(undefined2 *)(puVar2 + 0xb) = 0;
    *(undefined2 *)((int)puVar2 + 0x2e) = 0;
    *(short *)(puVar2 + 10) = param_5[3] - param_5[1];
    *(short *)((int)puVar2 + 0x2a) = param_5[2] - *param_5;
    *(undefined2 *)(puVar2 + 0xe) = 0xffff;
    *(undefined2 *)((int)puVar2 + 0x3a) = 0xffff;
    puVar2[0xf] = 0x3f800000;
    puVar2[0x10] = 0x3f800000;
    *(undefined1 *)(puVar2 + 0x13) = 1;
    *(undefined1 *)((int)puVar2 + 0x4d) = 0;
    puVar2[0x2b] = 0;
    puVar2[0x2c] = 0;
    puVar2[0x2d] = 0x3f800000;
    puVar2[0x2e] = 0x3f800000;
    puVar2[3] = 0;
    *(undefined1 *)(puVar2 + 4) = 1;
    *(undefined2 *)(puVar2 + 0x29) = 0;
    *(undefined2 *)((int)puVar2 + 0xa6) = 0;
    *(undefined2 *)(puVar2 + 0x2a) = 0x7fff;
    *(undefined2 *)((int)puVar2 + 0xaa) = 0x7fff;
    *(undefined2 *)(puVar2 + 0x27) = 0;
    *(undefined2 *)((int)puVar2 + 0x9e) = 0;
    *(undefined2 *)(puVar2 + 0x28) = 0;
    *(undefined2 *)((int)puVar2 + 0xa2) = 0;
    puVar2[0x2f] = 0;
    *(undefined1 *)((int)puVar2 + 0x142) = 0;
    uVar1 = *(undefined4 *)(param_5 + 2);
    puVar2[0x23] = *(undefined4 *)param_5;
    puVar2[0x24] = uVar1;
    *(undefined2 *)(puVar2 + 0x23) = 0;
    *(undefined2 *)((int)puVar2 + 0x8e) = 0;
    *(short *)(puVar2 + 0x24) = *(short *)(puVar2 + 0x24) - *(short *)(puVar2 + 0x23);
    *(short *)((int)puVar2 + 0x92) = *(short *)((int)puVar2 + 0x92) - *(short *)((int)puVar2 + 0x8e)
    ;
    puVar2[0x25] = puVar2[0x23];
    puVar2[0x26] = puVar2[0x24];
    puVar2[0x31] = 0;
    puVar2[0x33] = 0;
    puVar2[0x3c] = 0;
    *(undefined1 *)(puVar2 + 0x32) = 0;
    *(undefined1 *)((int)puVar2 + 0xc9) = 0;
    puVar2[0x3b] = 0;
    puVar2[0x3e] = 0;
    puVar2[0x3f] = PTR__SWStdSpriteDrawProc_001c45dc;
    puVar2[0x40] = PTR_LAB_001c46b8;
    *(undefined2 *)(puVar2 + 0x42) = 0xffff;
    *(undefined2 *)((int)puVar2 + 0x10a) = 0xffff;
    *(undefined2 *)(puVar2 + 0x43) = 0xffff;
    *(undefined2 *)((int)puVar2 + 0x10e) = 0xffff;
    *(undefined1 *)(puVar2 + 0x44) = 0;
    *(undefined1 *)((int)puVar2 + 0x141) = 0;
    *(undefined2 *)(puVar2 + 0x47) = 0;
    puVar2[0x4a] = 0;
    *(undefined8 *)(puVar2 + 0x48) = 0xbff0000000000000;
    puVar2[0x4c] = 1000000;
    puVar2[0x4d] = 0;
    *(undefined8 *)(puVar2 + 0x4e) = 0;
    *(undefined1 *)(puVar2 + 0x50) = 0;
    *(undefined1 *)((int)puVar2 + 0x146) = 0;
    *(undefined1 *)((int)puVar2 + 0x147) = 0;
    *(undefined1 *)(puVar2 + 0x52) = 0;
    *(undefined1 *)((int)puVar2 + 0x143) = 0;
    *(undefined1 *)(puVar2 + 0x51) = 0;
    *(undefined1 *)((int)puVar2 + 0x145) = 0;
    _SWClearStickyError();
    *param_1 = puVar2;
  }
  _SWSetStickyIfError(local_14);
  return local_14;
}


// ---- _SWDisposeSpriteWorld @ 0x26c10 (264 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWDisposeSpriteWorld(SpriteWorldPtr *spriteWorldPP)

{
  SpritePtr local_18;
  SpriteLayerPtr local_14;
  SpriteWorldPtr local_10;
  
  local_10 = *spriteWorldPP;
  if (local_10 != (SpriteWorldPtr)0x0) {
    _SWUnlockSpriteWorld(local_10);
    while( true ) {
      local_14 = _SWGetNextSpriteLayer(local_10,(SpriteLayerPtr)0x0);
      if (local_14 == (SpriteLayerPtr)0x0) break;
      while( true ) {
        local_18 = _SWGetNextSprite(local_14,(SpritePtr)0x0);
        if (local_18 == (SpritePtr)0x0) break;
        _SWRemoveSprite(local_18);
        _SWDisposeSprite(&local_18);
      }
      _SWRemoveSpriteLayer(local_10,local_14);
      _SWDisposeSpriteLayer(&local_14);
    }
    _SWDisposeAllSpritesInLayer(local_10->deadSpriteLayerP);
    _SWDisposeSpriteLayer(&local_10->deadSpriteLayerP);
    _SWDisposeFrame(&local_10->backFrameP);
    _SDL_GL_DeleteContext(local_10->glContext);
    _SDL_DestroyWindow(local_10->mainWindow);
    _free(local_10->tileLayerCallbacks);
    _free(local_10);
    *spriteWorldPP = (SpriteWorldPtr)0x0;
  }
  return;
}


// ---- _SWSetDesignResolution @ 0x26d18 (262 bytes)

void _SWSetDesignResolution(int param_1,short param_2,short param_3)

{
  undefined4 local_14;
  undefined4 local_10 [2];
  
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21421_001c469c,0x3b3);
  }
  *(short *)(param_1 + 0x38) = param_2;
  *(short *)(param_1 + 0x3a) = param_3;
  *(short *)(param_1 + 0x82) = *(short *)(param_1 + 0x7e) + param_2;
  *(short *)(param_1 + 0x80) = *(short *)(param_1 + 0x7c) + param_3;
  *(short *)(param_1 + 0x92) = *(short *)(param_1 + 0x8e) + param_2;
  *(short *)(param_1 + 0x90) = *(short *)(param_1 + 0x8c) + param_3;
  _SDL_GetWindowSize(*(undefined4 *)(param_1 + 0x158),local_10,&local_14);
  __setupGL(param_1,local_10[0],local_14);
  return;
}


// ---- _SWGetWindowScalingFactor @ 0x26e1e (94 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWGetWindowScalingFactor(SpriteWorldRec *spriteWorldP,float *scaleH,float *scaleV)

{
  if (spriteWorldP == (SpriteWorldRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21441_001c4694,0x3cf);
  }
  *scaleH = spriteWorldP->windowScaleH;
  *scaleV = spriteWorldP->windowScaleV;
  return;
}


// ---- _SWChangeWindowSize @ 0x26e7c (130 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWChangeWindowSize(SpriteWorldRec *spriteWorldP,short width,short height)

{
  if (spriteWorldP == (SpriteWorldRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21452_001c46a0,0x3dc);
  }
  _SDL_SetWindowSize(spriteWorldP->mainWindow,(int)width,(int)height);
  _SWUpdateWindowSize(spriteWorldP);
  return;
}


// ---- _SWSetSpriteWorldStretchToFit @ 0x26efe (99 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteWorldStretchToFit(SpriteWorldRec *spriteWorldP,uchar stretch)

{
  if (spriteWorldP == (SpriteWorldRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21463_001c46a8,0x3ea);
  }
  spriteWorldP->stretchToFit = stretch;
  _SWUpdateWindowSize(spriteWorldP);
  return;
}


// ---- _SWUpdateWindowSize @ 0x26f61 (307 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWUpdateWindowSize(SpriteWorldRec *spriteWorldP)

{
  int local_14;
  int local_10;
  
  _SDL_GetWindowSize(spriteWorldP->mainWindow,&local_10,&local_14);
  if (spriteWorldP->isAdditionalWorld == '\0') {
    _SWCalculateViewport(spriteWorldP,local_10,local_14);
    _SWResetGLSetup(spriteWorldP);
    _glMatrixMode(0x1701);
    _glLoadIdentity();
    _glOrtho(0,(double)local_10,(double)local_14,0,0xbff0000000000000,0x3ff0000000000000);
  }
  else {
    _SWCalculateViewport(spriteWorldP,local_10,local_14);
    _glViewport((int)spriteWorldP->viewportX,(int)spriteWorldP->viewportY,
                (int)spriteWorldP->viewportWidth,(int)spriteWorldP->viewportHeight);
  }
  return;
}


// ---- _SWChangeWorldRect @ 0x27094 (502 bytes)

int _SWChangeWorldRect(SpriteWorldPtr param_1,short *param_2,char param_3)

{
  short sVar1;
  short sVar2;
  short sVar3;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  short local_14;
  short local_12;
  int local_10;
  
  local_10 = 0;
  if ((param_1 == (SpriteWorldPtr)0x0) || (param_2 == (short *)0x0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21504_001c46a4,0x425);
  }
  if (local_10 == 0) {
    local_18._0_2_ = (param_1->worldRect).bottom;
    local_18._2_2_ = (param_1->worldRect).right;
    local_1c._0_2_ = (param_1->worldRect).top;
    local_1c._2_2_ = (param_1->worldRect).left;
    local_14 = (local_18._2_2_ - local_1c._2_2_) - (param_2[3] - param_2[1]);
    local_12 = ((undefined2)local_18 - (undefined2)local_1c) - (param_2[2] - *param_2);
    sVar2 = param_2[2];
    sVar3 = param_2[3];
    sVar1 = param_2[1];
    (param_1->worldRect).top = param_2[0];
    (param_1->worldRect).left = sVar1;
    (param_1->worldRect).bottom = sVar2;
    (param_1->worldRect).right = sVar3;
    _SWResizeVisScrollRect(param_1,param_2[3] - param_2[1],param_2[2] - *param_2);
    if (param_3 != '\0') {
      if (param_1->backFrameP != (FrameRec *)0x0) {
        (param_1->backFrameP->frameRect).bottom = (param_1->backFrameP->frameRect).bottom - local_12
        ;
        (param_1->backFrameP->frameRect).right = (param_1->backFrameP->frameRect).right - local_14;
      }
      (param_1->workAreaRect).bottom = (param_1->workAreaRect).bottom - local_12;
      (param_1->workAreaRect).right = (param_1->workAreaRect).right - local_14;
    }
    if (param_1->isAdditionalWorld == '\0') {
      _SDL_GetWindowSize(param_1->mainWindow,&local_20,&local_24);
      __setupGL(param_1,local_20,local_24);
      _SWCalculateViewport(param_1,local_20,local_24);
    }
  }
  return local_10;
}


// ---- _SWRestoreWorldRect @ 0x2728a (265 bytes)

void _SWRestoreWorldRect(SpriteWorldPtr param_1)

{
  FrameRec *pFVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  local_10._0_2_ = (param_1->screenRect).bottom;
  local_10._2_2_ = (param_1->screenRect).right;
  local_14._0_2_ = (param_1->screenRect).top;
  local_14._2_2_ = (param_1->screenRect).left;
  if (param_1 == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21566_001c46c0,0x465);
  }
  (param_1->worldRect).top = (short)local_14;
  (param_1->worldRect).left = local_14._2_2_;
  (param_1->worldRect).bottom = (short)local_10;
  (param_1->worldRect).right = local_10._2_2_;
  sVar3 = (param_1->backRect).bottom;
  sVar4 = (param_1->backRect).right;
  sVar2 = (param_1->backRect).left;
  (param_1->workAreaRect).top = (param_1->backRect).top;
  (param_1->workAreaRect).left = sVar2;
  (param_1->workAreaRect).bottom = sVar3;
  (param_1->workAreaRect).right = sVar4;
  if (param_1->backFrameP != (FrameRec *)0x0) {
    pFVar1 = param_1->backFrameP;
    sVar3 = (param_1->backRect).bottom;
    sVar4 = (param_1->backRect).right;
    sVar2 = (param_1->backRect).left;
    (pFVar1->frameRect).top = (param_1->backRect).top;
    (pFVar1->frameRect).left = sVar2;
    (pFVar1->frameRect).bottom = sVar3;
    (pFVar1->frameRect).right = sVar4;
  }
  _SWResizeVisScrollRect(param_1,local_10._2_2_ - local_14._2_2_,(short)local_10 - (short)local_14);
  _SDL_GetWindowSize(param_1->mainWindow,&local_18,&local_1c);
  __setupGL(param_1,local_18,local_1c);
  return;
}


// ---- _SWEraseBackground @ 0x27393 (260 bytes)

void SWEraseBackground(SpriteWorldRec *param_1)

{
  undefined4 uStack_18;
  undefined4 uStack_14;
  void *pvStack_10;
  
  pvStack_10 = _BKLoadGLfunctions();
  _SDL_GetWindowSize(param_1->mainWindow,&uStack_14,&uStack_18);
  _glViewport(0,0,uStack_14,uStack_18);
  (**(code **)((int)pvStack_10 + 0x48))
            ((float)(param_1->backgroundColor).r * 0.003921569,
             (float)(param_1->backgroundColor).g * 0.003921569,
             (float)(param_1->backgroundColor).b * 0.003921569,0x3f800000);
  (**(code **)((int)pvStack_10 + 0x4c))(0x4000);
  _SWResetGLSetup(param_1);
  return;
}


// ---- _SWAddSpriteLayer @ 0x27497 (142 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWAddSpriteLayer(SpriteWorldPtr spriteWorldP,SpriteLayerPtr newSpriteLayerP)

{
  SpriteLayerRec *pSVar1;
  
  pSVar1 = spriteWorldP->tailSpriteLayerP;
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21617_001c46d8,0x49f);
  }
  newSpriteLayerP->parentSpriteWorldP = spriteWorldP;
  if (pSVar1 == (SpriteLayerRec *)0x0) {
    spriteWorldP->headSpriteLayerP = newSpriteLayerP;
  }
  else {
    pSVar1->nextSpriteLayerP = newSpriteLayerP;
  }
  newSpriteLayerP->prevSpriteLayerP = pSVar1;
  newSpriteLayerP->nextSpriteLayerP = (SpriteLayerRec *)0x0;
  spriteWorldP->tailSpriteLayerP = newSpriteLayerP;
  return;
}


// ---- _SWRemoveSpriteLayer @ 0x27525 (158 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWRemoveSpriteLayer(SpriteWorldPtr spriteWorldP,SpriteLayerPtr oldSpriteLayerP)

{
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21628_001c4668,0x4b8);
  }
  oldSpriteLayerP->parentSpriteWorldP = (SpriteWorldRec *)0x0;
  if (oldSpriteLayerP->nextSpriteLayerP == (SpriteLayerRec *)0x0) {
    spriteWorldP->tailSpriteLayerP = oldSpriteLayerP->prevSpriteLayerP;
  }
  else {
    oldSpriteLayerP->nextSpriteLayerP->prevSpriteLayerP = oldSpriteLayerP->prevSpriteLayerP;
  }
  if (oldSpriteLayerP->prevSpriteLayerP == (SpriteLayerRec *)0x0) {
    spriteWorldP->headSpriteLayerP = oldSpriteLayerP->nextSpriteLayerP;
  }
  else {
    oldSpriteLayerP->prevSpriteLayerP->nextSpriteLayerP = oldSpriteLayerP->nextSpriteLayerP;
  }
  return;
}


// ---- _SWSwapSpriteLayer @ 0x275c3 (582 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWSwapSpriteLayer(SpriteWorldPtr spriteWorldP,SpriteLayerPtr srcSpriteLayerP,
                      SpriteLayerPtr dstSpriteLayerP)

{
  SpriteLayerRec *pSVar1;
  
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21652_001c4680,0x4e1);
  }
  if ((srcSpriteLayerP == (SpriteLayerPtr)0x0) || (dstSpriteLayerP == (SpriteLayerPtr)0x0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21652_001c4680,0x4e2);
  }
  if (srcSpriteLayerP != dstSpriteLayerP) {
    if ((srcSpriteLayerP->nextSpriteLayerP == dstSpriteLayerP) ||
       (dstSpriteLayerP->nextSpriteLayerP == srcSpriteLayerP)) {
      if (srcSpriteLayerP->nextSpriteLayerP == dstSpriteLayerP) {
        if (srcSpriteLayerP->prevSpriteLayerP != (SpriteLayerRec *)0x0) {
          srcSpriteLayerP->prevSpriteLayerP->nextSpriteLayerP = dstSpriteLayerP;
        }
        if (dstSpriteLayerP->nextSpriteLayerP != (SpriteLayerRec *)0x0) {
          dstSpriteLayerP->nextSpriteLayerP->prevSpriteLayerP = srcSpriteLayerP;
        }
        dstSpriteLayerP->prevSpriteLayerP = srcSpriteLayerP->prevSpriteLayerP;
        srcSpriteLayerP->nextSpriteLayerP = dstSpriteLayerP->nextSpriteLayerP;
        dstSpriteLayerP->nextSpriteLayerP = srcSpriteLayerP;
        srcSpriteLayerP->prevSpriteLayerP = dstSpriteLayerP;
      }
      else {
        if (dstSpriteLayerP->prevSpriteLayerP != (SpriteLayerRec *)0x0) {
          dstSpriteLayerP->prevSpriteLayerP->nextSpriteLayerP = srcSpriteLayerP;
        }
        if (srcSpriteLayerP->nextSpriteLayerP != (SpriteLayerRec *)0x0) {
          srcSpriteLayerP->nextSpriteLayerP->prevSpriteLayerP = dstSpriteLayerP;
        }
        srcSpriteLayerP->prevSpriteLayerP = dstSpriteLayerP->prevSpriteLayerP;
        dstSpriteLayerP->nextSpriteLayerP = srcSpriteLayerP->nextSpriteLayerP;
        srcSpriteLayerP->nextSpriteLayerP = dstSpriteLayerP;
        dstSpriteLayerP->prevSpriteLayerP = srcSpriteLayerP;
      }
    }
    else {
      if (srcSpriteLayerP->prevSpriteLayerP != (SpriteLayerRec *)0x0) {
        srcSpriteLayerP->prevSpriteLayerP->nextSpriteLayerP = dstSpriteLayerP;
      }
      if (dstSpriteLayerP->prevSpriteLayerP != (SpriteLayerRec *)0x0) {
        dstSpriteLayerP->prevSpriteLayerP->nextSpriteLayerP = srcSpriteLayerP;
      }
      if (srcSpriteLayerP->nextSpriteLayerP != (SpriteLayerRec *)0x0) {
        srcSpriteLayerP->nextSpriteLayerP->prevSpriteLayerP = dstSpriteLayerP;
      }
      if (dstSpriteLayerP->nextSpriteLayerP != (SpriteLayerRec *)0x0) {
        dstSpriteLayerP->nextSpriteLayerP->prevSpriteLayerP = srcSpriteLayerP;
      }
      pSVar1 = srcSpriteLayerP->nextSpriteLayerP;
      srcSpriteLayerP->nextSpriteLayerP = dstSpriteLayerP->nextSpriteLayerP;
      dstSpriteLayerP->nextSpriteLayerP = pSVar1;
      pSVar1 = srcSpriteLayerP->prevSpriteLayerP;
      srcSpriteLayerP->prevSpriteLayerP = dstSpriteLayerP->prevSpriteLayerP;
      dstSpriteLayerP->prevSpriteLayerP = pSVar1;
    }
    if (srcSpriteLayerP->nextSpriteLayerP == (SpriteLayerRec *)0x0) {
      spriteWorldP->tailSpriteLayerP = srcSpriteLayerP;
    }
    else if (srcSpriteLayerP->prevSpriteLayerP == (SpriteLayerRec *)0x0) {
      spriteWorldP->headSpriteLayerP = srcSpriteLayerP;
    }
    if (dstSpriteLayerP->nextSpriteLayerP == (SpriteLayerRec *)0x0) {
      spriteWorldP->tailSpriteLayerP = dstSpriteLayerP;
    }
    else if (dstSpriteLayerP->prevSpriteLayerP == (SpriteLayerRec *)0x0) {
      spriteWorldP->headSpriteLayerP = dstSpriteLayerP;
    }
  }
  return;
}


// ---- _SWGetNextSpriteLayer @ 0x27809 (100 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SpriteLayerPtr _SWGetNextSpriteLayer(SpriteWorldPtr spriteWorldP,SpriteLayerPtr curSpriteLayerP)

{
  SpriteLayerPtr local_10;
  
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21728_001c4678,0x537);
  }
  if (curSpriteLayerP == (SpriteLayerPtr)0x0) {
    local_10 = spriteWorldP->headSpriteLayerP;
  }
  else {
    local_10 = curSpriteLayerP->nextSpriteLayerP;
  }
  return local_10;
}


// ---- _SWLockSpriteWorld @ 0x2786d (72 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWLockSpriteWorld(SpriteWorldPtr spriteWorldP)

{
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21741_001c46b0,0x548);
  }
  return;
}


// ---- _SWUnlockSpriteWorld @ 0x278b5 (72 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWUnlockSpriteWorld(SpriteWorldPtr spriteWorldP)

{
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21748_001c46ac,0x562);
  }
  return;
}


// ---- _SWSetScrollingWorldMoveBounds @ 0x278fd (80 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetScrollingWorldMoveBounds(SpriteWorldPtr spriteWorldP,SWRect *scrollRectMoveBounds)

{
  short sVar1;
  short sVar2;
  short sVar3;
  
  sVar2 = scrollRectMoveBounds->bottom;
  sVar3 = scrollRectMoveBounds->right;
  sVar1 = scrollRectMoveBounds->left;
  (spriteWorldP->scrollRectMoveBounds).top = scrollRectMoveBounds->top;
  (spriteWorldP->scrollRectMoveBounds).left = sVar1;
  (spriteWorldP->scrollRectMoveBounds).bottom = sVar2;
  (spriteWorldP->scrollRectMoveBounds).right = sVar3;
  _SWMoveVisScrollRect
            (spriteWorldP,(spriteWorldP->visScrollRect).left,(spriteWorldP->visScrollRect).top);
  return;
}


// ---- _SWResizeVisScrollRect @ 0x2794d (462 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWResizeVisScrollRect(SpriteWorldPtr spriteWorldP,short horizSize,short vertSize)

{
  short sVar1;
  short sVar2;
  short sVar3;
  
  if ((int)(spriteWorldP->scrollRectMoveBounds).bottom <
      (int)(spriteWorldP->visScrollRect).top + (int)vertSize) {
    (spriteWorldP->visScrollRect).bottom = (spriteWorldP->scrollRectMoveBounds).bottom;
    (spriteWorldP->visScrollRect).top = (spriteWorldP->visScrollRect).bottom - vertSize;
    sVar3 = (spriteWorldP->scrollRectMoveBounds).top - (spriteWorldP->visScrollRect).top;
    if (0 < sVar3) {
      (spriteWorldP->visScrollRect).top = (spriteWorldP->visScrollRect).top + sVar3;
      (spriteWorldP->visScrollRect).bottom = (spriteWorldP->visScrollRect).bottom + sVar3;
    }
  }
  else {
    (spriteWorldP->visScrollRect).bottom = (spriteWorldP->visScrollRect).top + vertSize;
  }
  if ((int)(spriteWorldP->scrollRectMoveBounds).right <
      (int)(spriteWorldP->visScrollRect).left + (int)horizSize) {
    (spriteWorldP->visScrollRect).right = (spriteWorldP->scrollRectMoveBounds).right;
    (spriteWorldP->visScrollRect).left = (spriteWorldP->visScrollRect).right - horizSize;
    sVar3 = (spriteWorldP->scrollRectMoveBounds).left - (spriteWorldP->visScrollRect).left;
    if (0 < sVar3) {
      (spriteWorldP->visScrollRect).left = (spriteWorldP->visScrollRect).left + sVar3;
      (spriteWorldP->visScrollRect).right = (spriteWorldP->visScrollRect).right + sVar3;
    }
  }
  else {
    (spriteWorldP->visScrollRect).right = (spriteWorldP->visScrollRect).left + horizSize;
  }
  sVar1 = (spriteWorldP->visScrollRect).bottom;
  sVar2 = (spriteWorldP->visScrollRect).right;
  sVar3 = (spriteWorldP->visScrollRect).left;
  (spriteWorldP->oldVisScrollRect).top = (spriteWorldP->visScrollRect).top;
  (spriteWorldP->oldVisScrollRect).left = sVar3;
  (spriteWorldP->oldVisScrollRect).bottom = sVar1;
  (spriteWorldP->oldVisScrollRect).right = sVar2;
  return;
}


// ---- _SWMoveVisScrollRect @ 0x27b1b (432 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWMoveVisScrollRect(SpriteWorldPtr spriteWorldP,short horizPos,short vertPos)

{
  short sVar1;
  short sVar2;
  
  sVar1 = (spriteWorldP->visScrollRect).bottom - (spriteWorldP->visScrollRect).top;
  sVar2 = (spriteWorldP->visScrollRect).right - (spriteWorldP->visScrollRect).left;
  if (vertPos < (spriteWorldP->scrollRectMoveBounds).top) {
    (spriteWorldP->visScrollRect).top = (spriteWorldP->scrollRectMoveBounds).top;
    (spriteWorldP->visScrollRect).bottom = (spriteWorldP->visScrollRect).top + sVar1;
  }
  else if ((int)(spriteWorldP->scrollRectMoveBounds).bottom < (int)vertPos + (int)sVar1) {
    (spriteWorldP->visScrollRect).bottom = (spriteWorldP->scrollRectMoveBounds).bottom;
    (spriteWorldP->visScrollRect).top = (spriteWorldP->visScrollRect).bottom - sVar1;
  }
  else {
    (spriteWorldP->visScrollRect).top = vertPos;
    (spriteWorldP->visScrollRect).bottom = vertPos + sVar1;
  }
  if (horizPos < (spriteWorldP->scrollRectMoveBounds).left) {
    (spriteWorldP->visScrollRect).left = (spriteWorldP->scrollRectMoveBounds).left;
    (spriteWorldP->visScrollRect).right = (spriteWorldP->visScrollRect).left + sVar2;
  }
  else if ((int)(spriteWorldP->scrollRectMoveBounds).right < (int)horizPos + (int)sVar2) {
    (spriteWorldP->visScrollRect).right = (spriteWorldP->scrollRectMoveBounds).right;
    (spriteWorldP->visScrollRect).left = (spriteWorldP->visScrollRect).right - sVar2;
  }
  else {
    (spriteWorldP->visScrollRect).left = horizPos;
    (spriteWorldP->visScrollRect).right = horizPos + sVar2;
  }
  return;
}


// ---- _SWSetPostEraseCallBack @ 0x27ccb (23 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetPostEraseCallBack(SpriteWorldRec *spriteWorldP,CallBackPtr_t callBack)

{
  spriteWorldP->postEraseCallBack = callBack;
  return;
}


// ---- _SWSetPostDrawCallBack @ 0x27ce2 (23 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWSetPostDrawCallBack(SpriteWorldRec *spriteWorldP,CallBackPtr_t callBack)

{
  spriteWorldP->postDrawCallBack = callBack;
  return;
}


// ---- _SWSetSpriteWorldMaxFPS @ 0x27cf9 (138 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteWorldMaxFPS(SpriteWorldPtr spriteWorldP,short framesPerSec)

{
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21873_001c4688,0x618);
  }
  if ((framesPerSec < 1) || (1000 < framesPerSec)) {
    spriteWorldP->fpsTimeInterval = 0;
  }
  else {
    spriteWorldP->fpsTimeInterval = (short)(1000 / (longlong)(int)framesPerSec);
  }
  return;
}


// ---- _SWSetSpriteWorldPseudoFPS @ 0x27d83 (98 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteWorldPseudoFPS(SpriteWorldPtr spriteWorldP,short framesPerSec)

{
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21888_001c468c,0x62f);
  }
  spriteWorldP->pseudoFPS = (double)(int)framesPerSec;
  return;
}


// ---- _SWSetMinimumFrameRate @ 0x27de5 (73 bytes)

/* WARNING: Restarted to delay deadcode elimination for space: stack */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetMinimumFrameRate(SpriteWorldPtr spriteWorldP,int minimumFrameRate)

{
  if (minimumFrameRate < 1) {
    spriteWorldP->minimumFrameRate = 0xffffffff;
  }
  else {
    spriteWorldP->minimumFrameRate = (uint)(1000000 / (longlong)minimumFrameRate);
  }
  return;
}


// ---- _SWSetBackgroundColor @ 0x27e2e (84 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWSetBackgroundColor(SpriteWorldPtr spriteWorldP,SDL_Color color)

{
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21906_001c466c,0x64e);
  }
  spriteWorldP->backgroundColor = color;
  return;
}


// ---- _SWUpdateSpriteWorld @ 0x27e82 (2100 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWUpdateSpriteWorld(SpriteWorldPtr spriteWorldP,int updateWindow)

{
  SpriteRec *srcSpriteP;
  SpriteRec *pSVar1;
  short sVar2;
  short sVar3;
  UpdateRectStruct *pUVar4;
  short sVar5;
  undefined4 uVar6;
  SpriteLayerRec *local_54;
  SpriteRec *local_50;
  SWRect local_40;
  UpdateRectStruct *local_38;
  UpdateRectStruct *local_34;
  short local_30;
  undefined1 local_2d;
  int local_2c;
  SpriteRec *local_28;
  void *local_24;
  char local_1d;
  
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___21922_001c4684,0x77a);
  }
  *(SpriteWorldPtr *)PTR__gSWCurrentSpriteWorld_001c439c = spriteWorldP;
  local_30 = 0;
  local_2c = _glGetError();
  if (local_2c != 0) {
    uVar6 = _BKGetGLErrorMessage(local_2c);
    _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
             "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
             PTR____FUNCTION___21922_001c4684,0x77f,uVar6);
  }
  _glEnable(*(undefined4 *)PTR__gSWTexExtension_001c4550);
  _SWResetAllChangedTilesMaps(spriteWorldP);
  if (spriteWorldP->useOpenGL != 0) {
    _SDL_GL_MakeCurrent(spriteWorldP->mainWindow,spriteWorldP->glContext);
    if (spriteWorldP->backgroundTileSpriteP == (SpriteRec *)0x0) {
      _glViewport((int)spriteWorldP->viewportX,(int)spriteWorldP->viewportY,
                  (int)spriteWorldP->viewportWidth,(int)spriteWorldP->viewportHeight);
      if (spriteWorldP->isAdditionalWorld == '\0') {
        local_24 = _BKLoadGLfunctions();
        (**(code **)((int)local_24 + 0x48))
                  ((float)(spriteWorldP->backgroundColor).r * 0.003921569,
                   (float)(spriteWorldP->backgroundColor).g * 0.003921569,
                   (float)(spriteWorldP->backgroundColor).b * 0.003921569,0x3f800000);
        (**(code **)((int)local_24 + 0x4c))(0x4000);
      }
    }
    else {
      local_28 = spriteWorldP->backgroundTileSpriteP;
      _glViewport((int)spriteWorldP->fullViewportX,(int)spriteWorldP->fullViewportY,
                  (int)spriteWorldP->fullViewportWidth,(int)spriteWorldP->fullViewportHeight);
      _SWStdSpriteDrawProc(local_28,local_28->curFrameP,0.0,0.0);
      _glViewport((int)spriteWorldP->viewportX,(int)spriteWorldP->viewportY,
                  (int)spriteWorldP->viewportWidth,(int)spriteWorldP->viewportHeight);
    }
  }
  if (spriteWorldP->postEraseCallBack != (CallBackPtr_t)0x0) {
    (*spriteWorldP->postEraseCallBack)(spriteWorldP);
  }
  *(SpriteWorldPtr *)PTR__gSWCurrentSpriteWorld_001c439c = spriteWorldP;
  for (local_54 = spriteWorldP->headSpriteLayerP; local_54 != (SpriteLayerRec *)0x0;
      local_54 = local_54->nextSpriteLayerP) {
    if (((spriteWorldP->tilingIsOn != '\0') && (local_54->tileLayerBelow != -1)) &&
       (spriteWorldP->tileLayerArray[local_54->tileLayerBelow] != (TileMapStruct *)0x0)) {
      sVar5 = _SWDrawTileLayerInRect
                        (spriteWorldP,local_54->tileLayerBelow,&spriteWorldP->visScrollRect,'\x01',
                         '\0');
      local_2d = (undefined1)sVar5;
      local_30 = local_54->tileLayerBelow + 1;
      if (spriteWorldP->tileLayerCallbacks[local_54->tileLayerBelow] != (void *)0x0) {
        (*(code *)spriteWorldP->tileLayerCallbacks[local_54->tileLayerBelow])
                  (spriteWorldP,(int)local_54->tileLayerBelow,local_2d);
      }
    }
    if (local_54->layerIsVisible != '\0') {
      for (local_50 = local_54->headSpriteP; local_50 != (SpriteRec *)0x0;
          local_50 = local_50->nextSpriteP) {
        if (local_50->isVisible != '\0') {
          local_1d = '\0';
          if ((spriteWorldP->autoScaling != '\0') && (local_50->drawData->autoScaling == '\0')) {
            _glViewport((int)spriteWorldP->fullViewportX,(int)spriteWorldP->fullViewportY,
                        (int)spriteWorldP->fullViewportWidth,(int)spriteWorldP->fullViewportHeight);
            local_1d = '\x01';
          }
          (*local_50->frameDrawProc)
                    (local_50,local_50->curFrameP,local_50->drawData->horizLoc,
                     local_50->drawData->vertLoc);
          if (local_1d != '\0') {
            _glViewport((int)spriteWorldP->viewportX,(int)spriteWorldP->viewportY,
                        (int)spriteWorldP->viewportWidth,(int)spriteWorldP->viewportHeight);
          }
          if (((local_50->mountedSpriteP != (SpriteRec *)0x0) &&
              (local_50->mountedSpriteP->isVisible != '\0')) && (local_50->mountFlag2 != '\0')) {
            srcSpriteP = local_50->mountedSpriteP;
            srcSpriteP->needsToBeDrawn = '\x01';
            (*srcSpriteP->frameDrawProc)
                      (srcSpriteP,srcSpriteP->curFrameP,srcSpriteP->drawData->horizLoc,
                       srcSpriteP->drawData->vertLoc);
            pSVar1 = local_50->mountedSpriteP;
            sVar2 = (pSVar1->destFrameRect).bottom;
            sVar3 = (pSVar1->destFrameRect).right;
            sVar5 = (pSVar1->destFrameRect).left;
            (srcSpriteP->deltaFrameRect).top = (pSVar1->destFrameRect).top;
            (srcSpriteP->deltaFrameRect).left = sVar5;
            (srcSpriteP->deltaFrameRect).bottom = sVar2;
            (srcSpriteP->deltaFrameRect).right = sVar3;
            pSVar1 = local_50->mountedSpriteP;
            sVar2 = (pSVar1->destFrameRect).bottom;
            sVar3 = (pSVar1->destFrameRect).right;
            sVar5 = (pSVar1->destFrameRect).left;
            (srcSpriteP->oldFrameRect).top = (pSVar1->destFrameRect).top;
            (srcSpriteP->oldFrameRect).left = sVar5;
            (srcSpriteP->oldFrameRect).bottom = sVar2;
            (srcSpriteP->oldFrameRect).right = sVar3;
            srcSpriteP->needsToBeDrawn = '\0';
            srcSpriteP->needsToBeErased = '\0';
          }
          if ((spriteWorldP->tilingIsOn != '\0') && (local_30 <= spriteWorldP->lastActiveTileLayer))
          {
            local_40.top = (local_50->destFrameRect).top;
            local_40.left = (local_50->destFrameRect).left;
            local_40.bottom = (local_50->destFrameRect).bottom;
            local_40.right = (local_50->destFrameRect).right;
            if (local_40.top < (spriteWorldP->visScrollRect).top) {
              local_40.top = (spriteWorldP->visScrollRect).top;
            }
            if ((spriteWorldP->visScrollRect).bottom < local_40.bottom) {
              local_40.bottom = (spriteWorldP->visScrollRect).bottom;
            }
            if (local_40.left < (spriteWorldP->visScrollRect).left) {
              local_40.left = (spriteWorldP->visScrollRect).left;
            }
            if ((spriteWorldP->visScrollRect).right < local_40.right) {
              local_40.right = (spriteWorldP->visScrollRect).right;
            }
            if (local_50->drawTilesOverSprite != '\0') {
              local_40.left = local_40.left + (local_50->drawTilesInsetRect).left;
              local_40.top = local_40.top + (local_50->drawTilesInsetRect).top;
              local_40.right = local_40.right - (local_50->drawTilesInsetRect).right;
              local_40.bottom = local_40.bottom - (local_50->drawTilesInsetRect).bottom;
            }
            _SWDrawTilesAboveSprite(spriteWorldP,&local_40,local_30);
          }
        }
        sVar5 = (local_50->destFrameRect).left;
        sVar2 = (local_50->destFrameRect).bottom;
        sVar3 = (local_50->destFrameRect).right;
        (local_50->deltaFrameRect).top = (local_50->destFrameRect).top;
        (local_50->deltaFrameRect).left = sVar5;
        (local_50->deltaFrameRect).bottom = sVar2;
        (local_50->deltaFrameRect).right = sVar3;
        sVar5 = (local_50->destFrameRect).left;
        sVar2 = (local_50->destFrameRect).bottom;
        sVar3 = (local_50->destFrameRect).right;
        (local_50->oldFrameRect).top = (local_50->destFrameRect).top;
        (local_50->oldFrameRect).left = sVar5;
        (local_50->oldFrameRect).bottom = sVar2;
        (local_50->oldFrameRect).right = sVar3;
        local_50->needsToBeDrawn = '\0';
        local_50->needsToBeErased = '\0';
      }
    }
    if (((spriteWorldP->tilingIsOn != '\0') && (local_54->tileLayerAbove != -1)) &&
       (spriteWorldP->tileLayerArray[local_54->tileLayerAbove] != (TileMapStruct *)0x0)) {
      sVar5 = _SWDrawTileLayerInRect
                        (spriteWorldP,local_54->tileLayerAbove,&spriteWorldP->visScrollRect,'\x01',
                         '\0');
      local_2d = (undefined1)sVar5;
      local_30 = local_54->tileLayerAbove + 1;
      if (spriteWorldP->tileLayerCallbacks[local_54->tileLayerAbove] != (void *)0x0) {
        (*(code *)spriteWorldP->tileLayerCallbacks[local_54->tileLayerAbove])
                  (spriteWorldP,(int)local_54->tileLayerAbove,local_2d);
      }
    }
  }
  if (spriteWorldP->postDrawCallBack != (CallBackPtr_t)0x0) {
    (*spriteWorldP->postDrawCallBack)(spriteWorldP);
  }
  _SWPaintRectToSidesOfWorldRect(spriteWorldP);
  if ((updateWindow != 0) && (spriteWorldP->useOpenGL != 0)) {
    _SDL_GL_SwapWindow(spriteWorldP->mainWindow);
  }
  local_34 = spriteWorldP->headUpdateRectP;
  while (pUVar4 = local_34, local_34 != (UpdateRectStruct *)0x0) {
    local_38 = local_34;
    local_34 = local_34->nextRectStructP;
    _free(pUVar4);
  }
  spriteWorldP->headUpdateRectP = (UpdateRectStruct *)0x0;
  spriteWorldP->numTilesChanged = 0;
  _SWResetMovementTimer(spriteWorldP);
  *(undefined4 *)PTR__gSWCurrentSpriteWorld_001c439c = 0;
  return;
}


// ---- _SWProcessSpriteWorld @ 0x286b6 (615 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWProcessSpriteWorld(SpriteWorldPtr spriteWorldP)

{
  uint uVar1;
  SpriteLayerPtr local_20;
  double local_14;
  
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___22196_001c46b4,0x88b);
  }
  *(SpriteWorldPtr *)PTR__gSWCurrentSpriteWorld_001c439c = spriteWorldP;
  uVar1 = _SWGetMilliseconds(spriteWorldP);
  spriteWorldP->runningTimeCount = uVar1;
  if (spriteWorldP->runningTimeCount - spriteWorldP->timeOfLastFrame <
      (uint)(int)spriteWorldP->fpsTimeInterval) {
    spriteWorldP->frameHasOccurred = '\0';
  }
  else {
    spriteWorldP->millisecsSinceLastFrame =
         spriteWorldP->runningTimeCount - spriteWorldP->timeOfLastFrame;
    if (spriteWorldP->minimumFrameRate < spriteWorldP->millisecsSinceLastFrame) {
      spriteWorldP->millisecsSinceLastFrame = spriteWorldP->minimumFrameRate;
    }
    spriteWorldP->frameHasOccurred = '\x01';
    spriteWorldP->timeOfLastFrame = spriteWorldP->runningTimeCount;
    spriteWorldP->pseudoFrameAccum =
         (double)(int)(spriteWorldP->millisecsSinceLastFrame + 0x80000000) + 2147483648.0 +
         spriteWorldP->pseudoFrameAccum;
    if (0.0 <= spriteWorldP->pseudoFPS) {
      local_14 = spriteWorldP->pseudoFPS;
    }
    else {
      local_14 = (double)(int)spriteWorldP->fpsTimeInterval;
    }
    if (local_14 <= spriteWorldP->pseudoFrameAccum) {
      spriteWorldP->pseudoFrameHasFired = '\x01';
      spriteWorldP->pseudoFrameAccum = spriteWorldP->pseudoFrameAccum - local_14;
    }
    else {
      spriteWorldP->pseudoFrameHasFired = '\0';
    }
    if (spriteWorldP->deadSpriteLayerP->headSpriteP != (SpriteRec *)0x0) {
      _SWFindSpritesToBeRemoved(spriteWorldP);
    }
    if (spriteWorldP->tileChangeProc != (void *)0x0) {
      (*(code *)spriteWorldP->tileChangeProc)(spriteWorldP);
    }
    spriteWorldP->thereAreNonScrollingLayers = '\0';
    for (local_20 = spriteWorldP->headSpriteLayerP; local_20 != (SpriteLayerPtr)0x0;
        local_20 = local_20->nextSpriteLayerP) {
      if (local_20->layerIsNonScrolling == 0) {
        _SWProcessSpriteLayer(spriteWorldP,local_20);
      }
      else {
        spriteWorldP->thereAreNonScrollingLayers = '\x01';
      }
    }
    *(undefined4 *)PTR__gSWCurrentSpriteWorld_001c439c = 0;
  }
  return;
}


// ---- _SWProcessSpriteWorldForOneLayer @ 0x2891d (568 bytes)

void _SWProcessSpriteWorldForOneLayer(SpriteWorldPtr param_1,SpriteLayerPtr param_2)

{
  uint uVar1;
  double local_14;
  
  if (param_1 == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___22257_001c4674,0x8e6);
  }
  *(SpriteWorldPtr *)PTR__gSWCurrentSpriteWorld_001c439c = param_1;
  uVar1 = _SWGetMilliseconds(param_1);
  param_1->runningTimeCount = uVar1;
  if (param_1->runningTimeCount - param_1->timeOfLastFrame < (uint)(int)param_1->fpsTimeInterval) {
    param_1->frameHasOccurred = '\0';
  }
  else {
    param_1->millisecsSinceLastFrame = param_1->runningTimeCount - param_1->timeOfLastFrame;
    if (param_1->minimumFrameRate < param_1->millisecsSinceLastFrame) {
      param_1->millisecsSinceLastFrame = param_1->minimumFrameRate;
    }
    param_1->frameHasOccurred = '\x01';
    param_1->timeOfLastFrame = param_1->runningTimeCount;
    param_1->pseudoFrameAccum =
         (double)(int)(param_1->millisecsSinceLastFrame + 0x80000000) + 2147483648.0 +
         param_1->pseudoFrameAccum;
    if (0.0 <= param_1->pseudoFPS) {
      local_14 = param_1->pseudoFPS;
    }
    else {
      local_14 = (double)(int)param_1->fpsTimeInterval;
    }
    if (local_14 <= param_1->pseudoFrameAccum) {
      param_1->pseudoFrameHasFired = '\x01';
      param_1->pseudoFrameAccum = param_1->pseudoFrameAccum - local_14;
    }
    else {
      param_1->pseudoFrameHasFired = '\0';
    }
    if (param_1->deadSpriteLayerP->headSpriteP != (SpriteRec *)0x0) {
      _SWFindSpritesToBeRemoved(param_1);
    }
    if (param_1->tileChangeProc != (void *)0x0) {
      (*(code *)param_1->tileChangeProc)(param_1);
    }
    param_1->thereAreNonScrollingLayers = '\0';
    _SWProcessSpriteLayer(param_1,param_2);
    *(undefined4 *)PTR__gSWCurrentSpriteWorld_001c439c = 0;
  }
  return;
}


// ---- _SWProcessSpriteLayer @ 0x28b55 (307 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWProcessSpriteLayer(SpriteWorldPtr spriteWorldP,SpriteLayerPtr curSpriteLayerP)

{
  SpriteRec *pSVar1;
  SpriteRec *local_14;
  
  if (curSpriteLayerP->isPaused == 0) {
    pSVar1 = curSpriteLayerP->headSpriteP;
    while (local_14 = pSVar1, local_14 != (SpriteRec *)0x0) {
      pSVar1 = local_14->nextSpriteP;
      _SWProcessSprite(spriteWorldP,local_14);
      if (local_14->mountedSpriteP != (SpriteRec *)0x0) {
        if (local_14->mountFlag3 == '\0') {
          _SWMoveSprite(local_14->mountedSpriteP,
                        local_14->mountedSpriteP->mountOffsetH + local_14->drawData->horizLoc,
                        local_14->drawData->vertLoc + local_14->mountedSpriteP->mountOffsetV);
        }
        else {
          _SWMoveSprite(local_14->mountedSpriteP,
                        spriteWorldP->globalMountOffsetH + local_14->drawData->horizLoc,
                        local_14->drawData->vertLoc + spriteWorldP->globalMountOffsetV);
        }
        _SWProcessSprite(spriteWorldP,local_14->mountedSpriteP);
      }
    }
  }
  return;
}


// ---- _SWProcessSprite @ 0x28c88 (1081 bytes)

void _SWProcessSprite(int param_1,SpritePtr param_2)

{
  int local_20;
  double local_14;
  
  param_2->moveTimeAccum =
       (double)(*(int *)(param_1 + 0x134) + -0x80000000) + 2147483648.0 + param_2->moveTimeAccum;
  if (0.0 <= param_2->pseudoMoveTimeInterval) {
    local_14 = param_2->pseudoMoveTimeInterval;
  }
  else if (0.0 <= *(double *)(param_1 + 0x120)) {
    local_14 = 1000.0 / *(double *)(param_1 + 0x120);
  }
  else {
    local_14 = (double)(int)*(short *)(param_1 + 0x11c);
  }
  if (local_14 <= param_2->moveTimeAccum) {
    param_2->moveProcHasFired = '\x01';
    param_2->moveTimeAccum = param_2->moveTimeAccum - local_14;
  }
  else {
    param_2->moveProcHasFired = '\0';
  }
  if ((uint)(param_2->frameTimeInterval * 10) <
      *(int *)(param_1 + 0x128) - param_2->timeOfLastFrameChange) {
    param_2->timeOfLastFrameChange = *(uint *)(param_1 + 0x128);
  }
  do {
    if ((param_2->frameTimeInterval < 0) ||
       (*(int *)(param_1 + 0x128) - param_2->timeOfLastFrameChange <
        (uint)param_2->frameTimeInterval)) break;
    if (param_2->frameTimeInterval == 0) {
      param_2->timeOfLastFrameChange = *(uint *)(param_1 + 0x128);
    }
    else {
      param_2->timeOfLastFrameChange = param_2->timeOfLastFrameChange + param_2->frameTimeInterval;
    }
    local_20 = param_2->curFrameIndex + param_2->frameAdvance;
    if (local_20 < param_2->firstFrameIndex) {
      if (param_2->frameAdvanceMode == 0) {
        local_20 = param_2->lastFrameIndex;
      }
      else {
        param_2->frameAdvance = -param_2->frameAdvance;
        local_20 = local_20 + param_2->frameAdvance + param_2->frameAdvance;
      }
    }
    else if (param_2->lastFrameIndex < local_20) {
      if (param_2->frameAdvanceMode == 0) {
        local_20 = param_2->firstFrameIndex;
      }
      else {
        param_2->frameAdvance = -param_2->frameAdvance;
        local_20 = local_20 + param_2->frameAdvance + param_2->frameAdvance;
      }
    }
    param_2->curFrameIndex = local_20;
    if (param_2->frameChangeProc != (FrameProcPtr_t)0x0) {
      (*param_2->frameChangeProc)(param_2,param_2->frameArray[local_20],&param_2->curFrameIndex);
    }
    if (param_2->curFrameIndex < 0) {
      param_2->curFrameIndex = 0;
    }
    else if (param_2->maxFrames <= param_2->curFrameIndex) {
      param_2->curFrameIndex = param_2->maxFrames + -1;
    }
    if (param_2->curFrameP != param_2->frameArray[param_2->curFrameIndex]) {
      _SWSetCurrentFrameIndex(param_2,param_2->curFrameIndex);
    }
  } while (param_2->frameTimeInterval != 0);
  if (((param_2->mountedSpriteP != (SpriteRec *)0x0) && (param_2->mountFlag1 != '\0')) &&
     (param_2->mountedSpriteP->curFrameIndex != param_2->curFrameIndex)) {
    _SWSetCurrentFrameIndex(param_2->mountedSpriteP,param_2->curFrameIndex);
  }
  if (((-1 < param_2->moveTimeInterval) &&
      ((uint)param_2->moveTimeInterval <= *(int *)(param_1 + 0x128) - param_2->timeOfLastMove)) &&
     (param_2->timeOfLastMove = *(uint *)(param_1 + 0x128),
     param_2->spriteMoveProc != (MoveProcPtr_t)0x0)) {
    (*param_2->spriteMoveProc)(param_2);
  }
  return;
}


// ---- _SWProcessNonScrollingLayers @ 0x290c1 (91 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWProcessNonScrollingLayers(SpriteWorldPtr spriteWorldP)

{
  SpriteLayerPtr pSStack_10;
  
  if (spriteWorldP->thereAreNonScrollingLayers != '\0') {
    for (pSStack_10 = spriteWorldP->headSpriteLayerP; pSStack_10 != (SpriteLayerPtr)0x0;
        pSStack_10 = pSStack_10->nextSpriteLayerP) {
      if (pSStack_10->layerIsNonScrolling != 0) {
        _SWProcessSpriteLayer(spriteWorldP,pSStack_10);
      }
    }
    spriteWorldP->thereAreNonScrollingLayers = '\0';
  }
  return;
}


// ---- _SWAnimateSpriteWorld @ 0x2911c (3176 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWAnimateSpriteWorld(SpriteWorldPtr spriteWorldP)

{
  undefined2 uVar1;
  undefined2 uVar2;
  SpriteRec *srcSpriteP;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  short sVar9;
  short sVar10;
  UpdateRectStruct *pUVar11;
  short sVar12;
  short local_82;
  short local_7e;
  short local_7a;
  short local_76;
  SpriteLayerRec *local_74;
  SpriteRec *local_70;
  SWRect local_60;
  UpdateRectStruct *local_58;
  UpdateRectStruct *local_54;
  SpriteRec *local_50;
  SpriteRec *local_4c;
  SpriteRec *local_48;
  SpriteRec *local_44;
  SWRect *local_40;
  short local_3c;
  short local_3a;
  int local_38;
  undefined1 local_31;
  void *local_30;
  void *local_2c;
  SpriteRec *local_28;
  void *local_24;
  char local_1d;
  
  local_50 = (SpriteRec *)0x0;
  local_4c = (SpriteRec *)0x0;
  local_48 = (SpriteRec *)0x0;
  local_44 = (SpriteRec *)0x0;
  local_30 = _BKLoadGLfunctions();
  local_2c = _BKLoadGLcapabilities();
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___22521_001c4660,0xa1c);
  }
  *(SpriteWorldPtr *)PTR__gSWCurrentSpriteWorld_001c439c = spriteWorldP;
  local_3a = 0;
  if (spriteWorldP->frameHasOccurred != '\0') {
    if (spriteWorldP->deadSpriteLayerP->headSpriteP != (SpriteRec *)0x0) {
      _SWAddSpriteLayer(spriteWorldP,spriteWorldP->deadSpriteLayerP);
    }
    (**(code **)((int)local_30 + 0x14))(*(undefined4 *)PTR__gSWTexExtension_001c4550);
    _SWResetAllChangedTilesMaps(spriteWorldP);
    if (spriteWorldP->useOpenGL == 0) {
      for (local_74 = spriteWorldP->headSpriteLayerP; local_74 != (SpriteLayerRec *)0x0;
          local_74 = local_74->nextSpriteLayerP) {
        if (local_74->layerIsVisible != '\0') {
          for (local_70 = local_74->headSpriteP; local_70 != (SpriteRec *)0x0;
              local_70 = local_70->nextSpriteP) {
            if ((((local_70->needsToBeDrawn == '\0') || (local_70->isVisible == '\0')) &&
                ((local_70->needsToBeErased == '\0' || (local_70->isVisible != '\0')))) &&
               (((local_70->mountedSpriteP == (SpriteRec *)0x0 ||
                 (local_70->mountedSpriteP->needsToBeDrawn == '\0')) ||
                (local_70->mountedSpriteP->isVisible == '\0')))) {
              if (local_70->isVisible != '\0') {
                if (local_4c == (SpriteRec *)0x0) {
                  local_4c = local_70;
                }
                if (local_44 != (SpriteRec *)0x0) {
                  local_44->nextIdleSpriteP = local_70;
                }
                local_44 = local_70;
              }
            }
            else {
              if (local_50 == (SpriteRec *)0x0) {
                local_50 = local_70;
              }
              if (local_48 != (SpriteRec *)0x0) {
                local_48->nextActiveSpriteP = local_70;
              }
              local_48 = local_70;
              local_38 = 0;
              if (((local_70->doFastMovingCheck != '\0') ||
                  (spriteWorldP->doFastMovingSpritesChecks != '\0')) &&
                 ((((local_70->destFrameRect).bottom < (local_70->oldFrameRect).top ||
                   (((local_70->oldFrameRect).bottom < (local_70->destFrameRect).top ||
                    ((local_70->destFrameRect).right < (local_70->oldFrameRect).left)))) ||
                  ((local_70->oldFrameRect).right < (local_70->destFrameRect).left)))) {
                local_38 = 1;
              }
              if (local_38 == 0) {
                local_82 = (local_70->destFrameRect).top;
                sVar12 = (local_70->oldFrameRect).top;
                if (sVar12 < local_82) {
                  local_82 = sVar12;
                }
                (local_70->deltaFrameRect).top = local_82;
                local_7e = (local_70->destFrameRect).left;
                sVar12 = (local_70->oldFrameRect).left;
                if (sVar12 < local_7e) {
                  local_7e = sVar12;
                }
                (local_70->deltaFrameRect).left = local_7e;
                local_7a = (local_70->destFrameRect).bottom;
                sVar12 = (local_70->oldFrameRect).bottom;
                if (local_7a < sVar12) {
                  local_7a = sVar12;
                }
                (local_70->deltaFrameRect).bottom = local_7a;
                local_76 = (local_70->destFrameRect).right;
                sVar12 = (local_70->oldFrameRect).right;
                if (local_76 < sVar12) {
                  local_76 = sVar12;
                }
                (local_70->deltaFrameRect).right = local_76;
              }
              else {
                sVar12 = (local_70->destFrameRect).left;
                sVar9 = (local_70->destFrameRect).bottom;
                sVar10 = (local_70->destFrameRect).right;
                (local_70->deltaFrameRect).top = (local_70->destFrameRect).top;
                (local_70->deltaFrameRect).left = sVar12;
                (local_70->deltaFrameRect).bottom = sVar9;
                (local_70->deltaFrameRect).right = sVar10;
                _SWFlagRectAsChanged(spriteWorldP,&local_70->oldFrameRect);
              }
            }
          }
        }
      }
      if (local_48 != (SpriteRec *)0x0) {
        local_48->nextActiveSpriteP = (SpriteRec *)0x0;
      }
      if (local_44 != (SpriteRec *)0x0) {
        local_44->nextIdleSpriteP = (SpriteRec *)0x0;
      }
      local_58 = spriteWorldP->headUpdateRectP;
    }
    else {
      _SDL_GL_MakeCurrent(spriteWorldP->mainWindow,spriteWorldP->glContext);
      if (spriteWorldP->backgroundTileSpriteP == (SpriteRec *)0x0) {
        _glViewport((int)spriteWorldP->viewportX,(int)spriteWorldP->viewportY,
                    (int)spriteWorldP->viewportWidth,(int)spriteWorldP->viewportHeight);
        if (spriteWorldP->isAdditionalWorld == '\0') {
          local_24 = _BKLoadGLfunctions();
          (**(code **)((int)local_24 + 0x48))
                    ((float)(spriteWorldP->backgroundColor).r * 0.003921569,
                     (float)(spriteWorldP->backgroundColor).g * 0.003921569,
                     (float)(spriteWorldP->backgroundColor).b * 0.003921569,0x3f800000);
          (**(code **)((int)local_24 + 0x4c))(0x4000);
        }
      }
      else {
        local_28 = spriteWorldP->backgroundTileSpriteP;
        _glViewport((int)spriteWorldP->fullViewportX,(int)spriteWorldP->fullViewportY,
                    (int)spriteWorldP->fullViewportWidth,(int)spriteWorldP->fullViewportHeight);
        _SWStdSpriteDrawProc(local_28,local_28->curFrameP,0.0,0.0);
        _glViewport((int)spriteWorldP->viewportX,(int)spriteWorldP->viewportY,
                    (int)spriteWorldP->viewportWidth,(int)spriteWorldP->viewportHeight);
      }
    }
    if (spriteWorldP->postEraseCallBack != (CallBackPtr_t)0x0) {
      (*spriteWorldP->postEraseCallBack)(spriteWorldP);
    }
    *(SpriteWorldPtr *)PTR__gSWCurrentSpriteWorld_001c439c = spriteWorldP;
    for (local_74 = spriteWorldP->headSpriteLayerP; local_74 != (SpriteLayerRec *)0x0;
        local_74 = local_74->nextSpriteLayerP) {
      if (((spriteWorldP->tilingIsOn != '\0') && (local_74->tileLayerBelow != -1)) &&
         (spriteWorldP->tileLayerArray[local_74->tileLayerBelow] != (TileMapStruct *)0x0)) {
        sVar12 = _SWDrawTileLayerInRect
                           (spriteWorldP,local_74->tileLayerBelow,&spriteWorldP->visScrollRect,
                            '\x01','\0');
        local_31 = (undefined1)sVar12;
        local_3a = local_74->tileLayerBelow + 1;
        if (spriteWorldP->tileLayerCallbacks[local_74->tileLayerBelow] != (void *)0x0) {
          (*(code *)spriteWorldP->tileLayerCallbacks[local_74->tileLayerBelow])
                    (spriteWorldP,(int)local_74->tileLayerBelow,local_31);
        }
      }
      if (local_74->layerIsVisible != '\0') {
        for (local_70 = local_74->headSpriteP; local_70 != (SpriteRec *)0x0;
            local_70 = local_70->nextSpriteP) {
          if (local_70->isVisible != '\0') {
            if ((local_70->needsToBeDrawn != '\0') || (spriteWorldP->useOpenGL != 0)) {
              local_1d = '\0';
              if ((spriteWorldP->autoScaling != '\0') && (local_70->drawData->autoScaling == '\0'))
              {
                _glViewport((int)spriteWorldP->fullViewportX,(int)spriteWorldP->fullViewportY,
                            (int)spriteWorldP->fullViewportWidth,
                            (int)spriteWorldP->fullViewportHeight);
                local_1d = '\x01';
              }
              (*local_70->frameDrawProc)
                        (local_70,local_70->curFrameP,local_70->drawData->horizLoc,
                         local_70->drawData->vertLoc);
              if (local_1d != '\0') {
                _glViewport((int)spriteWorldP->viewportX,(int)spriteWorldP->viewportY,
                            (int)spriteWorldP->viewportWidth,(int)spriteWorldP->viewportHeight);
              }
              if (((local_70->mountedSpriteP != (SpriteRec *)0x0) &&
                  (local_70->mountedSpriteP->isVisible != '\0')) && (local_70->mountFlag2 != '\0'))
              {
                srcSpriteP = local_70->mountedSpriteP;
                (*srcSpriteP->frameDrawProc)
                          (srcSpriteP,srcSpriteP->curFrameP,srcSpriteP->drawData->horizLoc,
                           srcSpriteP->drawData->vertLoc);
                sVar12 = (srcSpriteP->destFrameRect).left;
                sVar9 = (srcSpriteP->destFrameRect).bottom;
                sVar10 = (srcSpriteP->destFrameRect).right;
                (srcSpriteP->deltaFrameRect).top = (srcSpriteP->destFrameRect).top;
                (srcSpriteP->deltaFrameRect).left = sVar12;
                (srcSpriteP->deltaFrameRect).bottom = sVar9;
                (srcSpriteP->deltaFrameRect).right = sVar10;
                sVar12 = (srcSpriteP->destFrameRect).left;
                sVar9 = (srcSpriteP->destFrameRect).bottom;
                sVar10 = (srcSpriteP->destFrameRect).right;
                (srcSpriteP->oldFrameRect).top = (srcSpriteP->destFrameRect).top;
                (srcSpriteP->oldFrameRect).left = sVar12;
                (srcSpriteP->oldFrameRect).bottom = sVar9;
                (srcSpriteP->oldFrameRect).right = sVar10;
              }
              if ((spriteWorldP->tilingIsOn != '\0') &&
                 (local_3a <= spriteWorldP->lastActiveTileLayer)) {
                local_60.top = (local_70->destFrameRect).top;
                local_60.left = (local_70->destFrameRect).left;
                local_60.bottom = (local_70->destFrameRect).bottom;
                local_60.right = (local_70->destFrameRect).right;
                if (local_60.top < (spriteWorldP->visScrollRect).top) {
                  local_60.top = (spriteWorldP->visScrollRect).top;
                }
                if ((spriteWorldP->visScrollRect).bottom < local_60.bottom) {
                  local_60.bottom = (spriteWorldP->visScrollRect).bottom;
                }
                if (local_60.left < (spriteWorldP->visScrollRect).left) {
                  local_60.left = (spriteWorldP->visScrollRect).left;
                }
                if ((spriteWorldP->visScrollRect).right < local_60.right) {
                  local_60.right = (spriteWorldP->visScrollRect).right;
                }
                if (local_70->drawTilesOverSprite != '\0') {
                  local_60.left = local_60.left + (local_70->drawTilesInsetRect).left;
                  local_60.top = local_60.top + (local_70->drawTilesInsetRect).top;
                  local_60.right = local_60.right - (local_70->drawTilesInsetRect).right;
                  local_60.bottom = local_60.bottom - (local_70->drawTilesInsetRect).bottom;
                }
                _SWDrawTilesAboveSprite(spriteWorldP,&local_60,local_3a);
              }
            }
            if (spriteWorldP->useOpenGL != 0) {
              sVar12 = (local_70->destFrameRect).left;
              sVar9 = (local_70->destFrameRect).bottom;
              sVar10 = (local_70->destFrameRect).right;
              (local_70->deltaFrameRect).top = (local_70->destFrameRect).top;
              (local_70->deltaFrameRect).left = sVar12;
              (local_70->deltaFrameRect).bottom = sVar9;
              (local_70->deltaFrameRect).right = sVar10;
              sVar12 = (local_70->destFrameRect).left;
              sVar9 = (local_70->destFrameRect).bottom;
              sVar10 = (local_70->destFrameRect).right;
              (local_70->oldFrameRect).top = (local_70->destFrameRect).top;
              (local_70->oldFrameRect).left = sVar12;
              (local_70->oldFrameRect).bottom = sVar9;
              (local_70->oldFrameRect).right = sVar10;
              local_70->needsToBeDrawn = '\0';
              local_70->needsToBeErased = '\0';
            }
          }
        }
      }
      if (((spriteWorldP->tilingIsOn != '\0') && (local_74->tileLayerAbove != -1)) &&
         (spriteWorldP->tileLayerArray[local_74->tileLayerAbove] != (TileMapStruct *)0x0)) {
        sVar12 = _SWDrawTileLayerInRect
                           (spriteWorldP,local_74->tileLayerAbove,&spriteWorldP->visScrollRect,
                            '\x01','\0');
        local_31 = (undefined1)sVar12;
        local_3a = local_74->tileLayerAbove + 1;
        if (spriteWorldP->tileLayerCallbacks[local_74->tileLayerAbove] != (void *)0x0) {
          (*(code *)spriteWorldP->tileLayerCallbacks[local_74->tileLayerAbove])
                    (spriteWorldP,(int)local_74->tileLayerAbove,local_31);
        }
      }
    }
    if (spriteWorldP->postDrawCallBack != (CallBackPtr_t)0x0) {
      (*spriteWorldP->postDrawCallBack)(spriteWorldP);
    }
    _SWPaintRectToSidesOfWorldRect(spriteWorldP);
    if (spriteWorldP->useOpenGL == 0) {
      local_70 = local_50;
      local_40 = spriteWorldP->changedTiles;
      for (local_3c = 0; (int)local_3c < spriteWorldP->numTilesChanged; local_3c = local_3c + 1) {
        uVar3._0_2_ = local_40->bottom;
        uVar3._2_2_ = local_40->right;
        uVar4._0_2_ = local_40->top;
        uVar4._2_2_ = local_40->left;
        _SWMergeUpdateRect(spriteWorldP,uVar4,uVar3,0);
        local_40 = local_40 + 1;
      }
      for (; local_70 != (SpriteRec *)0x0; local_70 = local_70->nextActiveSpriteP) {
        uVar5._0_2_ = (local_70->deltaFrameRect).top;
        uVar5._2_2_ = (local_70->deltaFrameRect).left;
        uVar6._0_2_ = (local_70->deltaFrameRect).bottom;
        uVar6._2_2_ = (local_70->deltaFrameRect).right;
        _SWMergeUpdateRect(spriteWorldP,uVar5,uVar6,0);
        sVar12 = (local_70->destFrameRect).left;
        sVar9 = (local_70->destFrameRect).bottom;
        sVar10 = (local_70->destFrameRect).right;
        (local_70->deltaFrameRect).top = (local_70->destFrameRect).top;
        (local_70->deltaFrameRect).left = sVar12;
        (local_70->deltaFrameRect).bottom = sVar9;
        (local_70->deltaFrameRect).right = sVar10;
        sVar12 = (local_70->destFrameRect).left;
        sVar9 = (local_70->destFrameRect).bottom;
        sVar10 = (local_70->destFrameRect).right;
        (local_70->oldFrameRect).top = (local_70->destFrameRect).top;
        (local_70->oldFrameRect).left = sVar12;
        (local_70->oldFrameRect).bottom = sVar9;
        (local_70->oldFrameRect).right = sVar10;
        local_70->needsToBeDrawn = '\0';
        local_70->needsToBeErased = '\0';
      }
      if (((spriteWorldP->worldRect).left == 0) && ((spriteWorldP->worldRect).top == 0)) {
        for (local_58 = spriteWorldP->headUpdateRectP; local_58 != (UpdateRectStruct *)0x0;
            local_58 = local_58->nextRectStructP) {
        }
      }
      else {
        for (local_58 = spriteWorldP->headUpdateRectP; local_58 != (UpdateRectStruct *)0x0;
            local_58 = local_58->nextRectStructP) {
          uVar1 = (local_58->updateRect).bottom;
          uVar7 = (local_58->updateRect).right;
          uVar2 = (local_58->updateRect).top;
          uVar8 = (local_58->updateRect).left;
          local_60.left = uVar8 + (spriteWorldP->worldRect).left;
          local_60.top = uVar2 + (spriteWorldP->worldRect).top;
          local_60.right = uVar7 + (spriteWorldP->worldRect).left;
          local_60.bottom = uVar1 + (spriteWorldP->worldRect).top;
        }
      }
    }
    else {
      _SDL_GL_SwapWindow(spriteWorldP->mainWindow);
    }
    local_54 = spriteWorldP->headUpdateRectP;
    while (pUVar11 = local_54, local_54 != (UpdateRectStruct *)0x0) {
      local_58 = local_54;
      local_54 = local_54->nextRectStructP;
      _free(pUVar11);
    }
    spriteWorldP->headUpdateRectP = (UpdateRectStruct *)0x0;
    spriteWorldP->numTilesChanged = 0;
    if (spriteWorldP->deadSpriteLayerP->headSpriteP != (SpriteRec *)0x0) {
      _SWRemoveSpriteLayer(spriteWorldP,spriteWorldP->deadSpriteLayerP);
    }
    *(undefined4 *)PTR__gSWCurrentSpriteWorld_001c439c = 0;
  }
  return;
}


// ---- _SWCollectIdleSpriteIntersectingRects @ 0x29d84 (537 bytes)

void SWCollectIdleSpriteIntersectingRects
               (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
               short *param_5)

{
  bool bVar1;
  short sStack_4a;
  short sStack_46;
  short sStack_42;
  short sStack_3e;
  short sStack_3a;
  short sStack_36;
  short sStack_32;
  short sStack_2e;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  
  if ((((param_5[1] == 0) && (param_5[3] == 0)) && (*param_5 == 0)) && (param_5[2] == 0)) {
    bVar1 = true;
  }
  else {
    bVar1 = false;
  }
  if ((((short)param_1 < (short)param_4) && ((short)param_3 < (short)param_2)) &&
     ((param_1._2_2_ < param_4._2_2_ && (param_3._2_2_ < param_2._2_2_)))) {
    sStack_4a = param_3._2_2_;
    if (param_3._2_2_ < param_1._2_2_) {
      sStack_4a = param_1._2_2_;
    }
    sStack_46 = (short)param_3;
    if ((short)param_3 < (short)param_1) {
      sStack_46 = (short)param_1;
    }
    uStack_20 = CONCAT22(sStack_4a,sStack_46);
    sStack_42 = param_4._2_2_;
    if (param_2._2_2_ < param_4._2_2_) {
      sStack_42 = param_2._2_2_;
    }
    sStack_3e = (short)param_4;
    if ((short)param_2 < (short)param_4) {
      sStack_3e = (short)param_2;
    }
    uStack_1c = CONCAT22(sStack_42,sStack_3e);
    if (bVar1) {
      uStack_18 = uStack_20;
      uStack_14 = uStack_1c;
    }
    else {
      sStack_3a = sStack_4a;
      if (param_5[1] < sStack_4a) {
        sStack_3a = param_5[1];
      }
      sStack_36 = sStack_46;
      if (*param_5 < sStack_46) {
        sStack_36 = *param_5;
      }
      uStack_18 = CONCAT22(sStack_3a,sStack_36);
      sStack_32 = sStack_42;
      if (sStack_42 < param_5[3]) {
        sStack_32 = param_5[3];
      }
      sStack_2e = sStack_3e;
      if (sStack_3e < param_5[2]) {
        sStack_2e = param_5[2];
      }
      uStack_14 = CONCAT22(sStack_32,sStack_2e);
    }
    *(undefined4 *)param_5 = uStack_18;
    *(undefined4 *)(param_5 + 2) = uStack_14;
  }
  return;
}


// ---- _SWFindSpritesToBeRemoved @ 0x29f9d (197 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWFindSpritesToBeRemoved(SpriteWorldPtr spriteWorldP)

{
  SpritePtr local_14;
  SpriteRec *local_10;
  
  local_14 = spriteWorldP->deadSpriteLayerP->headSpriteP;
  while (local_14 != (SpriteRec *)0x0) {
    local_10 = local_14->nextSpriteP;
    if (local_14->spriteRemoval == 0) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c"
                 ,PTR____FUNCTION___22983_001c46c4,0xcbd);
    }
    if ((local_14->needsToBeErased == '\0') || (spriteWorldP->useOpenGL != 0)) {
      _SWRemoveSprite(local_14);
      if (local_14->spriteRemoval == 2) {
        _SWDisposeSprite(&local_14);
      }
      else {
        local_14->spriteRemoval = 0;
      }
    }
    local_14 = local_10;
  }
  return;
}


// ---- _SWFlagRectAsChanged @ 0x2a062 (62 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWFlagRectAsChanged(SpriteWorldPtr spriteWorldP,SWRect *theChangedRect)

{
  undefined4 uVar1;
  undefined4 uVar2;
  SWError SVar3;
  
  uVar1._0_2_ = theChangedRect->bottom;
  uVar1._2_2_ = theChangedRect->right;
  uVar2._0_2_ = theChangedRect->top;
  uVar2._2_2_ = theChangedRect->left;
  SVar3 = _SWMergeUpdateRect(spriteWorldP,uVar2,uVar1,0);
  return SVar3;
}


// ---- _SWMergeUpdateRect @ 0x2a0a0 (559 bytes)

int _SWMergeUpdateRect(SpriteWorldPtr param_1,undefined4 param_2,undefined4 param_3,int *param_4)

{
  bool bVar1;
  UpdateRectStruct *pUVar2;
  short local_3a;
  short local_36;
  short local_32;
  short local_2e;
  int local_20;
  UpdateRectStruct *local_1c;
  int local_10;
  
  local_20 = 0;
  bVar1 = true;
  local_10 = 0;
  pUVar2 = (UpdateRectStruct *)_calloc(1,0xc);
  if (pUVar2 == (UpdateRectStruct *)0x0) {
    local_20 = 200;
    _SWSetStickyIfError(200);
  }
  if ((local_20 == 0) && (param_1->useUpdateRectOptimizations != '\0')) {
LAB_0002a258:
    if (bVar1) {
      bVar1 = false;
      for (local_1c = param_1->headUpdateRectP; local_1c != (UpdateRectStruct *)0x0;
          local_1c = local_1c->nextRectStructP) {
        if (((((short)param_2 < (local_1c->updateRect).bottom) &&
             ((local_1c->updateRect).top < (short)param_3)) &&
            (param_2._2_2_ < (local_1c->updateRect).right)) &&
           ((local_1c->updateRect).left < param_3._2_2_)) {
          local_10 = local_10 + 1;
          local_3a = (local_1c->updateRect).top;
          if ((short)param_2 < local_3a) {
            local_3a = (short)param_2;
          }
          local_36 = (local_1c->updateRect).left;
          if (param_2._2_2_ < local_36) {
            local_36 = param_2._2_2_;
          }
          param_2 = CONCAT22(local_36,local_3a);
          local_32 = (local_1c->updateRect).bottom;
          if (local_32 < (short)param_3) {
            local_32 = (short)param_3;
          }
          local_2e = (local_1c->updateRect).right;
          if (local_2e < param_3._2_2_) {
            local_2e = param_3._2_2_;
          }
          param_3 = CONCAT22(local_2e,local_32);
          _SWRemoveUpdateRectStruct(param_1,local_1c);
          bVar1 = true;
          break;
        }
      }
      goto LAB_0002a258;
    }
  }
  if (local_20 == 0) {
    (pUVar2->updateRect).top = (short)param_2;
    (pUVar2->updateRect).left = param_2._2_2_;
    (pUVar2->updateRect).bottom = (short)param_3;
    (pUVar2->updateRect).right = param_3._2_2_;
    pUVar2->nextRectStructP = (UpdateRectStruct *)0x0;
    if (param_1->headUpdateRectP == (UpdateRectStruct *)0x0) {
      param_1->headUpdateRectP = pUVar2;
    }
    else {
      for (local_1c = param_1->headUpdateRectP; local_1c->nextRectStructP != (UpdateRectStruct *)0x0
          ; local_1c = local_1c->nextRectStructP) {
      }
      local_1c->nextRectStructP = pUVar2;
    }
  }
  if (param_4 != (int *)0x0) {
    *param_4 = local_10;
  }
  return local_20;
}


// ---- _SWRemoveUpdateRectStruct @ 0x2a2cf (101 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWRemoveUpdateRectStruct(SpriteWorldPtr spriteWorldP,void *removeRect)

{
  UpdateRectStruct *local_10;
  
  if (spriteWorldP->headUpdateRectP == (UpdateRectStruct *)removeRect) {
    spriteWorldP->headUpdateRectP = *(UpdateRectStruct **)((int)removeRect + 8);
  }
  else {
    for (local_10 = spriteWorldP->headUpdateRectP; local_10 != (UpdateRectStruct *)0x0;
        local_10 = local_10->nextRectStructP) {
      if (local_10->nextRectStructP == (UpdateRectStruct *)removeRect) {
        local_10->nextRectStructP = *(UpdateRectStruct **)((int)removeRect + 8);
        break;
      }
    }
  }
  _free(removeRect);
  return;
}


// ---- _SWGetMilliseconds @ 0x2a334 (19 bytes)

void _SWGetMilliseconds(void)

{
  _SDL_GetTicks();
  return;
}


// ---- _SWResetElapsedMilliseconds @ 0x2a347 (27 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWResetElapsedMilliseconds(SpriteWorldRec *spriteWorldP)

{
  spriteWorldP->millisecsSinceLastFrame = 0;
  return;
}


// ---- _SWGetElapsedMilliseconds @ 0x2a362 (23 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int _SWGetElapsedMilliseconds(SpriteWorldRec *spriteWorldP)

{
  return spriteWorldP->millisecsSinceLastFrame;
}


// ---- _SWResetMovementTimer @ 0x2a379 (14 bytes)

void _SWResetMovementTimer(void)

{
  return;
}


// ---- _SWHasPseudoFrameFired @ 0x2a387 (85 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

uchar SWHasPseudoFrameFired(SpriteWorldRec *spriteWorldP)

{
  if (spriteWorldP == (SpriteWorldRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___23120_001c46cc,0xda3);
  }
  return spriteWorldP->pseudoFrameHasFired;
}


// ---- _SWGetCurrentFPS @ 0x2a3dc (117 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

float SWGetCurrentFPS(SpriteWorldPtr spriteWorldP)

{
  int iVar1;
  uint uVar2;
  
  iVar1 = _SWGetMilliseconds(spriteWorldP);
  uVar2 = iVar1 - spriteWorldP->timeOfLastFrame;
  return 1000.0 / ((float)(uVar2 >> 0x10) * 65536.0 + (float)(uVar2 & 0xffff));
}


// ---- _SWSetCleanUpSpriteWorld @ 0x2a451 (33 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetCleanUpSpriteWorld(SpriteWorldPtr spriteWorldP)

{
  *(SpriteWorldPtr *)PTR__gSWCleanUpSpriteWorldP_001c4664 = spriteWorldP;
  return;
}


// ---- _SWSetCleanUpFunction @ 0x2a472 (33 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetCleanUpFunction(void *callBackP)

{
  *(void **)PTR__gSWCleanUpCallBackP_001c4698 = callBackP;
  return;
}


// ---- _SWGetSpriteWorldVersion @ 0x2a493 (19 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

uint SWGetSpriteWorldVersion(void)

{
  return 0x806001;
}


// ---- _SWGetSpriteWorldPixelDepth @ 0x2a4a6 (85 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

short SWGetSpriteWorldPixelDepth(SpriteWorldPtr spriteWorldP)

{
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___23150_001c4690,0xde7);
  }
  return (short)spriteWorldP->pixelDepth;
}


// ---- _SWPaintRectToSidesOfWorldRect @ 0x2a4fb (778 bytes)

void _SWPaintRectToSidesOfWorldRect(int param_1)

{
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  short local_10;
  short local_e;
  
  if (*(char *)(param_1 + 0x145) == '\0') {
    if ((*(short *)(param_1 + 0x2c) == 0) || (*(char *)(param_1 + 0x4c) == '\0')) {
      if ((*(short *)(param_1 + 0x2e) != 0) && (*(char *)(param_1 + 0x4c) != '\0')) {
        _glColor3ub(*(undefined1 *)(param_1 + 0x146),*(undefined1 *)(param_1 + 0x147),
                    *(undefined1 *)(param_1 + 0x148));
        _glDisable(*(undefined4 *)PTR__gSWTexExtension_001c4550);
        _SDL_GetWindowSize(*(undefined4 *)(param_1 + 0x158),&local_1c,&local_20);
        _glViewport(0,0,local_1c,local_20);
        _glRecti(0,0,(int)*(short *)(param_1 + 0x24),(int)*(short *)(param_1 + 0x2e));
        local_e = *(short *)(param_1 + 0x2a) + *(short *)(param_1 + 0x2e);
        _glRecti(0,(int)local_e,(int)*(short *)(param_1 + 0x24),
                 (int)local_e + (int)*(short *)(param_1 + 0x2e));
        _glViewport((int)*(short *)(param_1 + 0x20),(int)*(short *)(param_1 + 0x22),
                    (int)*(short *)(param_1 + 0x24),(int)*(short *)(param_1 + 0x26));
        _glEnable(*(undefined4 *)PTR__gSWTexExtension_001c4550);
      }
    }
    else {
      _glDisable(*(undefined4 *)PTR__gSWTexExtension_001c4550);
      _glColor3ub(*(undefined1 *)(param_1 + 0x146),*(undefined1 *)(param_1 + 0x147),
                  *(undefined1 *)(param_1 + 0x148));
      _SDL_GetWindowSize(*(undefined4 *)(param_1 + 0x158),&local_14,&local_18);
      _glViewport(0,0,local_14,local_18);
      _glRecti(0,0,(int)*(short *)(param_1 + 0x2c),(int)*(short *)(param_1 + 0x26));
      local_10 = *(short *)(param_1 + 0x28) + *(short *)(param_1 + 0x2c);
      _glRecti((int)local_10,0,(int)local_10 + (int)*(short *)(param_1 + 0x2c),
               (int)*(short *)(param_1 + 0x26));
      _glViewport((int)*(short *)(param_1 + 0x20),(int)*(short *)(param_1 + 0x22),
                  (int)*(short *)(param_1 + 0x24),(int)*(short *)(param_1 + 0x26));
      _glEnable(*(undefined4 *)PTR__gSWTexExtension_001c4550);
    }
  }
  return;
}


// ---- _SWDrawRectangle @ 0x2a805 (193 bytes)

void SWDrawRectangle(int param_1,short param_2,short param_3,short param_4,short param_5)

{
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteWorld.c",
               PTR____FUNCTION___23252_001c46bc,0xe37);
  }
  _glColor3ub(*(undefined1 *)(param_1 + 0x146),*(undefined1 *)(param_1 + 0x147),
              *(undefined1 *)(param_1 + 0x148));
  _glRecti((int)param_2,(int)param_3,(int)param_4,(int)param_5);
  return;
}


// ---- _SWClearStickyError @ 0x2a8c6 (34 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWClearStickyError(void)

{
  *(undefined4 *)PTR__gSWStickyError_001c4670 = 0;
  return;
}


// ---- _SWStickyError @ 0x2a8e8 (30 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWStickyError(void)

{
  return *(SWError *)PTR__gSWStickyError_001c4670;
}


// ---- _SWSetStickyIfError @ 0x2a906 (39 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetStickyIfError(SWError errNum)

{
  if (errNum != 0) {
    *(SWError *)PTR__gSWStickyError_001c4670 = errNum;
  }
  return;
}


// ---- _SWSetAssertionFailureCallback @ 0x2a92d (33 bytes)

void _SWSetAssertionFailureCallback(undefined4 param_1)

{
  *(undefined4 *)PTR__gAssertFailCallback_001c46d0 = param_1;
  return;
}


// ---- _SWAssertFail @ 0x2a94e (130 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWAssertFail(char *filename,char *functionName,int lineNum)

{
  _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nASSERT FAILED: \n File: %s \nfunction:%s \nline: %d\n",
           filename,functionName,lineNum);
  if (*(int *)PTR__gAssertFailCallback_001c46d0 != 0) {
    (**(code **)PTR__gAssertFailCallback_001c46d0)(filename,functionName,lineNum);
  }
                    /* WARNING: Subroutine does not return */
  _exit(1);
}


