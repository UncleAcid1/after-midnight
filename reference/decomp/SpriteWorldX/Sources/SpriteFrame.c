// Ghidra decompilation of functions from original source file:
//   ../SpriteWorldX/Sources/SpriteFrame.c

// ---- _SWCreateFrame @ 0x232f8 (192 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWCreateFrame(FramePtr *newFrameP)

{
  FramePtr pFVar1;
  int local_14;
  
  local_14 = 0;
  pFVar1 = (FramePtr)_calloc(1,0x3c);
  if (pFVar1 == (FramePtr)0x0) {
    local_14 = 200;
  }
  if (local_14 == 0) {
    pFVar1->frameSurfaceP = (SDL_Surface *)0x0;
    pFVar1->originalSurfaceP = (SDL_Surface *)0x0;
    pFVar1->glSurfaceP = (void *)0x0;
    pFVar1->hotSpotH = 0;
    pFVar1->hotSpotV = 0;
    (pFVar1->collisionInset).top = 0;
    (pFVar1->collisionInset).left = 0;
    (pFVar1->collisionInset).bottom = 0;
    (pFVar1->collisionInset).right = 0;
    pFVar1->usesCollisionInset = '\0';
    *newFrameP = pFVar1;
  }
  else if (pFVar1 != (FramePtr)0x0) {
    _free(pFVar1);
  }
  return local_14;
}


// ---- _SWCreateFrameFromSurface @ 0x233b8 (116 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWCreateFrameFromSurface(FramePtr *newFrameP,SDL_Surface *theSurface,int isVideoSurface)

{
  FramePtr local_14;
  SWError local_10;
  
  local_10 = 0;
  local_14 = (FramePtr)0x0;
  local_10 = _SWCreateFrame(&local_14);
  if (local_10 == 0) {
    local_14->frameSurfaceP = theSurface;
    local_14->isVideoSurface = isVideoSurface;
    local_10 = _SWUpdateFrame(local_14);
  }
  if (local_10 == 0) {
    *newFrameP = local_14;
  }
  else if (local_14 != (FramePtr)0x0) {
    _free(local_14);
  }
  return local_10;
}


// ---- _SWCreateFrameFromSurfaceAndRect @ 0x2342c (302 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWCreateFrameFromSurfaceAndRect(FramePtr *newFrameP,SDL_Surface *surface,SWRect *frameRect)

{
  short sVar1;
  short sVar2;
  short sVar3;
  FramePtr local_14;
  SWError local_10;
  
  local_10 = 0;
  if (surface == (SDL_Surface *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteFrame.c",
               PTR____FUNCTION___20148_001c45f4,0x77);
  }
  if ((((frameRect->top < 0) || (frameRect->left < 0)) || (frameRect->right <= frameRect->left)) ||
     (frameRect->bottom <= frameRect->top)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteFrame.c",
               PTR____FUNCTION___20148_001c45f4,0x78);
  }
  local_14 = (FramePtr)0x0;
  *newFrameP = (FramePtr)0x0;
  local_10 = _SWCreateFrame(&local_14);
  if (local_10 == 0) {
    local_14->frameSurfaceP = surface;
    sVar2 = frameRect->bottom;
    sVar3 = frameRect->right;
    sVar1 = frameRect->left;
    (local_14->frameRect).top = frameRect->top;
    (local_14->frameRect).left = sVar1;
    (local_14->frameRect).bottom = sVar2;
    (local_14->frameRect).right = sVar3;
    local_14->sharesSurface = 1;
    local_10 = _SWUpdateFrame(local_14);
  }
  if (local_10 == 0) {
    *newFrameP = local_14;
  }
  else if (local_14 != (FramePtr)0x0) {
    _free(local_14);
  }
  return local_10;
}


// ---- _SWCreateBlankFrame @ 0x2355a (216 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWCreateBlankFrame(FramePtr *newFrameP,int w,int h,uchar depth,SWBoolean createAlphaChannel
                           )

{
  FramePtr local_18;
  int local_14;
  SDL_Surface *local_10;
  
  local_14 = 0;
  local_18 = (FramePtr)0x0;
  local_10 = (SDL_Surface *)0x0;
  local_14 = _SWCreateFrame(&local_18);
  if (local_14 == 0) {
    local_10 = _BKCreateSurface(w,h,(uint)depth,0,0,(uint)createAlphaChannel);
    if (local_10 == (SDL_Surface *)0x0) {
      local_14 = 0x12e;
    }
  }
  if (local_14 == 0) {
    local_18->originalSurfaceP = local_10;
    local_14 = _SWUpdateFrame(local_18);
  }
  if (local_14 == 0) {
    *newFrameP = local_18;
  }
  else {
    if (local_10 != (SDL_Surface *)0x0) {
      _SDL_FreeSurface(local_10);
    }
    if (local_18 != (FramePtr)0x0) {
      _free(local_18);
    }
  }
  return local_14;
}


// ---- _SWCreateBlankGLFrame @ 0x23632 (188 bytes)

int _SWCreateBlankGLFrame(undefined4 *param_1,short param_2,short param_3,undefined1 param_4)

{
  FramePtr pFVar1;
  void *pvVar2;
  FramePtr local_14;
  SWError local_10;
  
  local_10 = 0;
  local_14 = (FramePtr)0x0;
  local_10 = _SWCreateFrame(&local_14);
  pFVar1 = local_14;
  if (local_10 == 0) {
    local_14->originalSurfaceP = (SDL_Surface *)0x0;
    pvVar2 = (void *)_BKCreateBlankGLSurface((int)param_2,(int)param_3,param_4,0);
    pFVar1->glSurfaceP = pvVar2;
    (local_14->frameRect).top = 0;
    (local_14->frameRect).left = 0;
    (local_14->frameRect).bottom = param_3;
    (local_14->frameRect).right = param_2;
  }
  if (local_10 == 0) {
    *param_1 = local_14;
  }
  else if (local_14 != (FramePtr)0x0) {
    _free(local_14);
  }
  return local_10;
}


// ---- _SWCreateFrameFromFile @ 0x236ee (197 bytes)

int _SWCreateFrameFromFile(undefined4 *param_1,char *param_2,char param_3)

{
  FramePtr local_18;
  int local_14;
  SDL_Surface *local_10;
  
  local_14 = 0;
  local_18 = (FramePtr)0x0;
  local_10 = (SDL_Surface *)0x0;
  local_14 = _SWCreateFrame(&local_18);
  if (local_14 == 0) {
    local_10 = _BKLoadSurface(param_2);
    if (local_10 == (SDL_Surface *)0x0) {
      local_14 = 0x12f;
    }
  }
  if ((local_14 == 0) && (param_3 != '\0')) {
    local_10 = (SDL_Surface *)_SWDoSelfMaskingForSurface(local_10);
  }
  if (local_14 == 0) {
    local_18->originalSurfaceP = local_10;
    local_14 = _SWUpdateFrame(local_18);
  }
  if (local_14 == 0) {
    *param_1 = local_18;
  }
  else {
    if (local_10 != (SDL_Surface *)0x0) {
      _SDL_FreeSurface(local_10);
    }
    if (local_18 != (FramePtr)0x0) {
      _free(local_18);
    }
  }
  return local_14;
}


// ---- _SWUpdateFrameFromFile @ 0x237b3 (172 bytes)

int _SWUpdateFrameFromFile(FramePtr param_1,char *param_2,char param_3)

{
  int local_14;
  SDL_Surface *local_10;
  
  local_14 = 0;
  local_10 = _BKLoadSurface(param_2);
  if (local_10 == (SDL_Surface *)0x0) {
    local_14 = 0x12f;
  }
  if ((local_14 == 0) && (param_3 != '\0')) {
    local_10 = (SDL_Surface *)_SWDoSelfMaskingForSurface(local_10);
  }
  if (local_14 == 0) {
    if (param_1->originalSurfaceP != (SDL_Surface *)0x0) {
      _SDL_FreeSurface(param_1->originalSurfaceP);
    }
    param_1->originalSurfaceP = local_10;
    local_14 = _SWUpdateFrame(param_1);
  }
  if ((local_14 != 0) && (local_10 != (SDL_Surface *)0x0)) {
    _SDL_FreeSurface(local_10);
  }
  return local_14;
}


// ---- _SWCreateFrameFromFileMask @ 0x2385f (544 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError SWCreateFrameFromFileMask(FramePtr *newFrameP,char *filename,char *maskname,SWBoolean alpha)

{
  int iVar1;
  undefined4 uVar2;
  SDL_Color SStack_34;
  FramePtr pFStack_30;
  int iStack_2c;
  SDL_Surface *pSStack_28;
  SDL_Surface *pSStack_24;
  SDL_Surface *pSStack_20;
  
  iStack_2c = 0;
  pFStack_30 = (FramePtr)0x0;
  pSStack_28 = (SDL_Surface *)0x0;
  pSStack_24 = (SDL_Surface *)0x0;
  pSStack_20 = (SDL_Surface *)0x0;
  iStack_2c = _SWCreateFrame(&pFStack_30);
  if (iStack_2c == 0) {
    pSStack_24 = _BKLoadSurface(filename);
    if (pSStack_24 == (SDL_Surface *)0x0) {
      iStack_2c = 0x12f;
    }
  }
  if ((iStack_2c == 0) && (maskname != (char *)0x0)) {
    pSStack_28 = _BKCreateSurface(pSStack_24->w,pSStack_24->h,
                                  (uint)*(byte *)((int)pSStack_24->format + 8),0,
                                  (uint)(alpha == '\0'),(uint)alpha);
    if (pSStack_28 == (SDL_Surface *)0x0) {
      iStack_2c = -2;
    }
    if (iStack_2c == 0) {
      iVar1 = _strcmp(filename,maskname);
      if (iVar1 == 0) {
        if (alpha == '\0') {
          _BKConvertColorKeyToAlphaChannel(pSStack_28,&SStack_34);
        }
        else {
          uVar2 = _SDL_MapRGB(pSStack_28->format,*PTR__gSWTransparentColor_001c45d8,
                              PTR__gSWTransparentColor_001c45d8[1],
                              PTR__gSWTransparentColor_001c45d8[2]);
          _SDL_SetColorKey(pSStack_28,0x20000,uVar2);
        }
      }
      else {
        pSStack_20 = _BKLoadSurface(maskname);
        if (pSStack_20 == (SDL_Surface *)0x0) {
          iStack_2c = 0x12f;
        }
        else {
          if (alpha == '\0') {
            _BKAddColorKeyFromMask(pSStack_28,pSStack_20);
          }
          else {
            _BKAddAlphaChannelFromMask(pSStack_28,pSStack_20);
          }
          _SDL_FreeSurface(pSStack_20);
        }
      }
    }
  }
  if (iStack_2c == 0) {
    pFStack_30->originalSurfaceP = pSStack_28;
    iStack_2c = _SWUpdateFrame(pFStack_30);
  }
  if (iStack_2c == 0) {
    *newFrameP = pFStack_30;
  }
  else {
    if (pSStack_28 != (SDL_Surface *)0x0) {
      _SDL_FreeSurface(pSStack_28);
    }
    if (pFStack_30 != (FramePtr)0x0) {
      _free(pFStack_30);
    }
  }
  return iStack_2c;
}


// ---- _SWCreateFrameFromWorkArea @ 0x23a7f (604 bytes)

int _SWCreateFrameFromWorkArea(int param_1,int *param_2,short *param_3)

{
  undefined4 uVar1;
  int local_3c;
  int local_38;
  undefined1 local_34 [4];
  short local_30;
  short local_2e;
  short local_2c;
  short local_2a;
  int local_28;
  int local_24;
  int local_20;
  
  local_24 = 0;
  local_20 = _glGetError();
  if (local_20 != 0) {
    uVar1 = _BKGetGLErrorMessage(local_20);
    _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
             "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteFrame.c",
             PTR____FUNCTION___20864_001c45e4,0x1c8,uVar1);
  }
  _SDL_GL_MakeCurrent(*(undefined4 *)(param_1 + 0x158),*(undefined4 *)(param_1 + 0x15c));
  _glViewport((int)*(short *)(param_1 + 0x20),(int)*(short *)(param_1 + 0x22),
              (int)*(short *)(param_1 + 0x24),(int)*(short *)(param_1 + 0x26));
  _SDL_GetWindowSize(*(undefined4 *)(param_1 + 0x158),local_34,&local_38);
  local_30 = param_3[3] - param_3[1];
  local_2e = param_3[2] - *param_3;
  if (*(char *)(param_1 + 0x4c) == '\0') {
    local_2c = *(short *)(param_1 + 0x74);
    local_2a = param_3[1];
  }
  else {
    local_2c = (short)(int)(((float)(local_38 - param_3[2]) -
                            *(float *)(param_1 + 0x48) * (float)(int)*(short *)(param_1 + 0x74)) -
                           (float)(int)*(short *)(param_1 + 0x2e));
    local_2a = param_3[1] + *(short *)(param_1 + 0x2c);
  }
  local_24 = _SWCreateBlankGLFrame
                       (&local_3c,(int)local_30,(int)local_2e,*(undefined1 *)(param_1 + 0x11a));
  if (local_24 == 0) {
    local_28 = *(int *)(local_3c + 0x34);
    _glBindTexture(*(undefined4 *)(local_28 + 0x30),**(undefined4 **)(local_28 + 0x58));
    _glCopyTexSubImage2D
              (*(undefined4 *)(local_28 + 0x30),0,0,0,(int)local_2a,(int)local_2c,(int)local_30,
               (int)local_2e);
    *param_2 = local_3c;
  }
  return local_24;
}


// ---- _SWUpdateFrameFromWorkArea @ 0x23cdb (333 bytes)

void _SWUpdateFrameFromWorkArea(int param_1,int param_2,short *param_3)

{
  int local_28;
  undefined1 local_24 [4];
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  
  _SDL_GetWindowSize(*(undefined4 *)(param_1 + 0x158),local_24,&local_28);
  local_20 = (int)param_3[3] - (int)param_3[1];
  local_1c = (int)param_3[2] - (int)*param_3;
  if (*(char *)(param_1 + 0x4c) == '\0') {
    local_18 = (int)*(short *)(param_1 + 0x74);
    local_14 = (int)param_3[1];
  }
  else {
    local_18 = (int)((float)(int)*(short *)(param_1 + 0x2e) +
                    ((float)(local_28 - param_3[2]) -
                    *(float *)(param_1 + 0x40) * (float)(int)*(short *)(param_1 + 0x74)));
    local_14 = (int)param_3[1] + (int)*(short *)(param_1 + 0x2c);
  }
  local_10 = *(int *)(param_2 + 0x34);
  _glBindTexture(*(undefined4 *)(local_10 + 0x30),**(undefined4 **)(local_10 + 0x58));
  _glCopyTexSubImage2D(*(undefined4 *)(local_10 + 0x30),0,0,0,local_14,local_18,local_20,local_1c);
  return;
}


// ---- _SWDisposeFrame @ 0x23e28 (233 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int _SWDisposeFrame(FramePtr *oldFramePP)

{
  FramePtr pFVar1;
  int local_14;
  
  local_14 = 0;
  pFVar1 = *oldFramePP;
  if (pFVar1 != (FramePtr)0x0) {
    if (pFVar1->useCount < 2) {
      local_14 = 1;
      if (pFVar1->isVideoSurface == 0) {
        if (pFVar1->frameSurfaceP != (SDL_Surface *)0x0) {
          if (pFVar1->sharesSurface == 0) {
            _SDL_FreeSurface(pFVar1->frameSurfaceP);
          }
          pFVar1->frameSurfaceP = (SDL_Surface *)0x0;
        }
        if (pFVar1->originalSurfaceP != (SDL_Surface *)0x0) {
          if (pFVar1->sharesSurface == 0) {
            _SDL_FreeSurface(pFVar1->originalSurfaceP);
          }
          pFVar1->originalSurfaceP = (SDL_Surface *)0x0;
        }
      }
      if (pFVar1->glSurfaceP != (void *)0x0) {
        _BKFreeGLSurface(pFVar1->glSurfaceP);
        pFVar1->glSurfaceP = (void *)0x0;
      }
      _free(pFVar1);
      *oldFramePP = (FramePtr)0x0;
    }
    else {
      pFVar1->useCount = pFVar1->useCount - 1;
    }
  }
  return local_14;
}


// ---- _SWLockFrame @ 0x23f11 (90 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWLockFrame(FramePtr srcFrameP)

{
  if (srcFrameP == (FramePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteFrame.c",
               PTR____FUNCTION___21013_001c45f0,0x27b);
  }
  if (srcFrameP->isFrameLocked == '\0') {
    srcFrameP->isFrameLocked = '\x01';
  }
  return;
}


// ---- _SWUnlockFrame @ 0x23f6b (90 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWUnlockFrame(FramePtr srcFrameP)

{
  if (srcFrameP == (FramePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteFrame.c",
               PTR____FUNCTION___21023_001c45ec,0x28a);
  }
  if (srcFrameP->isFrameLocked != '\0') {
    srcFrameP->isFrameLocked = '\0';
  }
  return;
}


// ---- _SWUpdateFrame @ 0x23fc5 (743 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWUpdateFrame(FramePtr srcFrameP)

{
  int iVar1;
  void *pvVar2;
  undefined4 uVar3;
  int *local_40;
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  SDL_Surface *local_28;
  SDL_Surface *local_24;
  void *local_20;
  
  local_2c = 0;
  local_28 = (SDL_Surface *)0x0;
  if (srcFrameP == (FramePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteFrame.c",
               PTR____FUNCTION___21038_001c45fc,0x2ee);
  }
  if (srcFrameP->originalSurfaceP != (SDL_Surface *)0x0) {
    local_24 = srcFrameP->originalSurfaceP;
    iVar1 = _SDL_GetVideoSurface();
    if (iVar1 == 0) {
      local_20 = local_24->format;
      local_28 = (SDL_Surface *)
                 _SDL_CreateRGBSurfaceFrom
                           (local_24->pixels,local_24->w,local_24->h,
                            *(undefined1 *)((int)local_20 + 8),local_24->pitch,
                            *(undefined4 *)((int)local_20 + 0xc),
                            *(undefined4 *)((int)local_20 + 0x10),
                            *(undefined4 *)((int)local_20 + 0x14),
                            *(undefined4 *)((int)local_20 + 0x18));
    }
    else if ((local_24->flags >> 0x10 & 1) == 0) {
      local_28 = (SDL_Surface *)_SDL_DisplayFormat(local_24);
    }
    else {
      local_28 = (SDL_Surface *)_SDL_DisplayFormatAlpha(local_24);
    }
    if (local_28 == (SDL_Surface *)0x0) {
      local_2c = 0x132;
    }
    if (local_2c == 0) {
      if (srcFrameP->frameSurfaceP != (SDL_Surface *)0x0) {
        _SDL_FreeSurface(srcFrameP->frameSurfaceP);
      }
      srcFrameP->frameSurfaceP = local_28;
    }
  }
  if (((*PTR__gSWUseOpenGL_001c45e0 != '\0') && (local_2c == 0)) && (srcFrameP->isVideoSurface == 0)
     ) {
    if (srcFrameP->glSurfaceP != (void *)0x0) {
      _BKFreeGLSurface(srcFrameP->glSurfaceP);
    }
    local_3c = (int)(srcFrameP->frameRect).left;
    local_38 = (int)(srcFrameP->frameRect).top;
    local_34 = (int)(srcFrameP->frameRect).right - (int)(srcFrameP->frameRect).left;
    local_30 = (int)(srcFrameP->frameRect).bottom - (int)(srcFrameP->frameRect).top;
    if (srcFrameP->sharesSurface == 0) {
      local_40 = (int *)0x0;
    }
    else {
      local_40 = &local_3c;
    }
    pvVar2 = _BKCreateNewGLSurface(srcFrameP->frameSurfaceP,local_40);
    srcFrameP->glSurfaceP = pvVar2;
    if (srcFrameP->glSurfaceP == (void *)0x0) {
      uVar3 = _SDL_GetError();
      _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"BKCreateNewGLSurface: %s\n",uVar3);
      local_2c = 0x132;
    }
  }
  if (local_2c == 0) {
    if (srcFrameP->sharesSurface == 0) {
      (srcFrameP->frameRect).top = 0;
      (srcFrameP->frameRect).left = 0;
      (srcFrameP->frameRect).bottom = (short)srcFrameP->frameSurfaceP->h;
      (srcFrameP->frameRect).right = (short)srcFrameP->frameSurfaceP->w;
    }
    if ((srcFrameP->frameSurfaceP->flags >> 0x10 & 1) == 0) {
      srcFrameP->maskType = 0;
    }
    else {
      srcFrameP->maskType = 1;
    }
  }
  else if (local_28 != (SDL_Surface *)0x0) {
    _SDL_FreeSurface(local_28);
  }
  return local_2c;
}


// ---- _SWFreeGLTextureForFrame @ 0x242ac (48 bytes)

void SWFreeGLTextureForFrame(int param_1)

{
  if (*(int *)(param_1 + 0x34) != 0) {
    _BKFreeGLSurface(*(void **)(param_1 + 0x34));
    *(undefined4 *)(param_1 + 0x34) = 0;
  }
  return;
}


// ---- _SWSetFrameHotSpot @ 0x242dc (134 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetFrameHotSpot(FramePtr srcFrameP,DrawDataPtr parentSpriteDrawDataP,short hotSpotH,
                       short hotSpotV)

{
  if (srcFrameP == (FramePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteFrame.c",
               PTR____FUNCTION___21156_001c45f8,0x361);
  }
  srcFrameP->hotSpotH = (int)hotSpotH;
  srcFrameP->hotSpotV = (int)hotSpotV;
  if (parentSpriteDrawDataP != (DrawDataPtr)0x0) {
    parentSpriteDrawDataP->frameHotSpotH = hotSpotH;
    parentSpriteDrawDataP->frameHotSpotV = hotSpotV;
  }
  return;
}


// ---- _SWSetFrameCollisionInset @ 0x24362 (190 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetFrameCollisionInset
               (FramePtr srcFrameP,short leftInset,short topInset,short rightInset,short bottomInset
               )

{
  if (srcFrameP == (FramePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteFrame.c",
               PTR____FUNCTION___21171_001c45e8,0x377);
  }
  if ((((leftInset == 0) && (topInset == 0)) && (rightInset == 0)) && (bottomInset == 0)) {
    srcFrameP->usesCollisionInset = '\0';
  }
  else {
    srcFrameP->usesCollisionInset = '\x01';
  }
  (srcFrameP->collisionInset).top = topInset;
  (srcFrameP->collisionInset).left = leftInset;
  (srcFrameP->collisionInset).bottom = bottomInset;
  (srcFrameP->collisionInset).right = rightInset;
  return;
}


