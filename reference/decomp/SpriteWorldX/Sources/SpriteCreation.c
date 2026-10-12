// Ghidra decompilation of functions from original source file:
//   ../SpriteWorldX/Sources/SpriteCreation.c

// ---- _SWSetTransparentColor @ 0x22141 (35 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetTransparentColor(void *RGB)

{
                    /* WARNING: Load size is inaccurate */
  *(undefined4 *)PTR__gSWTransparentColor_001c45d8 = *RGB;
  return;
}


// ---- _SWCreateSprite @ 0x22164 (836 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWCreateSprite(SpritePtr *newSpriteP,void *spriteStorage,int maxFrames)

{
  FrameRec **ppFVar1;
  DrawDataPtr pDVar2;
  SpritePtr local_20;
  SWError local_1c;
  
  local_1c = 0;
  *newSpriteP = (SpritePtr)0x0;
  if (spriteStorage == (void *)0x0) {
    local_20 = (SpritePtr)_calloc(1,0xf8);
  }
  else {
    local_20 = (SpritePtr)spriteStorage;
  }
  if (local_20 == (SpritePtr)0x0) {
    local_1c = 200;
  }
  else {
    if (maxFrames == 0) {
      maxFrames = 1;
    }
    ppFVar1 = (FrameRec **)_calloc(1,maxFrames << 2);
    local_20->frameArray = ppFVar1;
    if (local_20->frameArray == (FrameRec **)0x0) {
      local_1c = 200;
    }
    else {
      local_20->parentSpriteLayerP = (SpriteLayerRec *)0x0;
      local_20->nextSpriteP = (SpriteRec *)0x0;
      local_20->prevSpriteP = (SpriteRec *)0x0;
      local_20->spriteRecSize = 0xf8;
      local_20->isVisible = '\x01';
      local_20->needsToBeDrawn = '\x01';
      local_20->needsToBeErased = '\0';
      (local_20->destFrameRect).top = 0;
      (local_20->destFrameRect).left = 0;
      (local_20->destFrameRect).bottom = 0;
      (local_20->destFrameRect).right = 0;
      (local_20->oldFrameRect).top = 0;
      (local_20->oldFrameRect).left = 0;
      (local_20->oldFrameRect).bottom = 0;
      (local_20->oldFrameRect).right = 0;
      (local_20->deltaFrameRect).top = 0;
      (local_20->deltaFrameRect).left = 0;
      (local_20->deltaFrameRect).bottom = 0;
      (local_20->deltaFrameRect).right = 0;
      local_20->frameDrawProc = (DrawProcPtr_t)PTR__SWStdSpriteDrawProc_001c45dc;
      local_20->drawTilesOverSprite = '\0';
      local_20->curFrameP = (FrameRec *)0x0;
      local_20->numFrames = 0;
      local_20->maxFrames = maxFrames;
      local_20->frameTimeInterval = -1;
      local_20->timeOfLastFrameChange = 0;
      local_20->frameAdvance = 1;
      local_20->curFrameIndex = 0;
      local_20->firstFrameIndex = 0;
      local_20->lastFrameIndex = 0;
      local_20->frameChangeProc = (FrameProcPtr_t)0x0;
      local_20->moveTimeInterval = 0;
      local_20->timeOfLastMove = 0;
      local_20->pseudoMoveTimeInterval = -1.0;
      local_20->moveTimeAccum = 0.0;
      local_20->moveProcHasFired = '\0';
      (local_20->moveBoundsRect).top = 0;
      (local_20->moveBoundsRect).left = 0;
      (local_20->moveBoundsRect).bottom = 0;
      (local_20->moveBoundsRect).right = 0;
      local_20->spriteMoveProc = (MoveProcPtr_t)PTR_SWTempSpriteMoveProc_001c4564;
      local_20->realSpriteMoveProc = (MoveProcPtr_t)0x0;
      local_20->spriteCollideProc = (CollideProcPtr_t)0x0;
      local_20->sharedSurface = (SDL_Surface *)0x0;
      local_20->spriteRemoval = 0;
      local_20->frameAdvanceMode = 0;
      pDVar2 = _SWCreateDrawData();
      local_20->drawData = pDVar2;
      local_20->doFastMovingCheck = '\0';
      local_20->drawData->parentSpriteP = local_20;
      local_20->drawData->rotationOffsetHoriz = 0.0;
      local_20->drawData->rotationOffsetVert = 0.0;
      local_20->scalingMode = '\0';
      local_20->mountedSpriteP = (SpriteRec *)0x0;
      local_20->mountParentP = (SpriteRec *)0x0;
      local_20->userData = 0;
      *newSpriteP = local_20;
    }
  }
  return local_1c;
}


// ---- _SWCreateDrawData @ 0x224a8 (334 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

DrawDataPtr _SWCreateDrawData(void)

{
  DrawDataPtr pDVar1;
  
  pDVar1 = (DrawDataPtr)_calloc(1,0x58);
  if (pDVar1 == (DrawDataPtr)0x0) {
    _SWSetStickyIfError(200);
  }
  else {
    pDVar1->parentSpriteP = (SpriteRec *)0x0;
    pDVar1->horizLoc = 0.0;
    pDVar1->vertLoc = 0.0;
    pDVar1->horizMoveDelta = 0.0;
    pDVar1->vertMoveDelta = 0.0;
    pDVar1->flippedHorizontal = '\0';
    pDVar1->flippedVertical = '\0';
    pDVar1->frameHotSpotH = 0;
    pDVar1->frameHotSpotV = 0;
    pDVar1->rotation = 0.0;
    pDVar1->scaledWidth = -1.0;
    pDVar1->scaledHeight = -1.0;
    pDVar1->scaledWidthFactor = 1.0;
    pDVar1->scaledHeightFactor = 1.0;
    pDVar1->useClip = '\0';
    pDVar1->temporaryClip = '\0';
    pDVar1->srcBlendMode = 0x302;
    pDVar1->dstBlendMode = 0x303;
    pDVar1->useBlendMode = '\0';
    pDVar1->autoScaling = '\x01';
    pDVar1->nonScrolling = '\0';
    pDVar1->translucencyLevel = 0xffff;
    pDVar1->colorRed = 0xffff;
    pDVar1->colorGreen = 0xffff;
    pDVar1->colorBlue = 0xffff;
  }
  return pDVar1;
}


// ---- _SWCreateSpriteFromSingleFile @ 0x225f6 (307 bytes)

undefined4
_SWCreateSpriteFromSingleFile
          (undefined4 param_1,undefined4 param_2,char *param_3,short param_4,short param_5,
          undefined1 param_6)

{
  SDL_Surface *pSVar1;
  undefined4 local_3c;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  
  pSVar1 = _BKLoadSurface(param_3);
  if (pSVar1 == (SDL_Surface *)0x0) {
    local_3c = 0x12f;
  }
  else {
    if (pSVar1->w < pSVar1->h) {
      local_20 = pSVar1->w;
      local_1c = (int)param_4;
      local_18 = 0;
      local_14 = (int)param_5;
      local_10 = (pSVar1->h + (int)param_5) / ((int)param_4 + (int)param_5);
    }
    else {
      local_20 = (int)param_4;
      local_1c = pSVar1->h;
      local_14 = 0;
      local_18 = (int)param_5;
      local_10 = (pSVar1->w + (int)param_5) / ((int)param_4 + (int)param_5);
    }
    local_3c = _SWCreateSpriteFromSurfaceXY
                         (param_1,param_2,pSVar1,local_20,local_1c,local_18,local_14,0,local_10,
                          param_6);
  }
  return local_3c;
}


// ---- _SWCreateSpriteFromSingleFileXY @ 0x22729 (130 bytes)

undefined4
_SWCreateSpriteFromSingleFileXY
          (undefined4 param_1,undefined4 param_2,char *param_3,undefined4 param_4,undefined4 param_5
          ,undefined4 param_6,undefined4 param_7,undefined4 param_8,undefined4 param_9,
          undefined1 param_10)

{
  SDL_Surface *pSVar1;
  undefined4 local_24;
  
  pSVar1 = _BKLoadSurface(param_3);
  if (pSVar1 == (SDL_Surface *)0x0) {
    local_24 = 0x12f;
  }
  else {
    local_24 = _SWCreateSpriteFromSurfaceXY
                         (param_1,param_2,pSVar1,param_4,param_5,param_6,param_7,param_8,param_9,
                          param_10);
  }
  return local_24;
}


// ---- _SWCreateSpriteFromVerticalFile @ 0x227ab (165 bytes)

/* WARNING: Restarted to delay deadcode elimination for space: stack */

undefined4
_SWCreateSpriteFromVerticalFile
          (undefined4 param_1,undefined4 param_2,char *param_3,undefined4 param_4,int param_5,
          undefined1 param_6)

{
  SDL_Surface *pSVar1;
  undefined4 local_24;
  
  pSVar1 = _BKLoadSurface(param_3);
  if (pSVar1 == (SDL_Surface *)0x0) {
    local_24 = 0x12f;
  }
  else {
    local_24 = _SWCreateSpriteFromSurfaceXY
                         (param_1,param_2,pSVar1,pSVar1->w,pSVar1->h / param_5,0,param_4,0,param_5,
                          param_6);
  }
  return local_24;
}


// ---- _SWCreateSpriteFromSurfaceXY @ 0x22850 (690 bytes)

/* WARNING: Restarted to delay deadcode elimination for space: stack */

SWError _SWCreateSpriteFromSurfaceXY
                  (SpritePtr *param_1,void *param_2,SDL_Surface *param_3,int param_4,int param_5,
                  int param_6,int param_7,int param_8,int param_9,char param_10)

{
  short sVar1;
  short sVar2;
  FramePtr local_2c;
  SWRect local_28;
  SpritePtr local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  
  local_1c = 0;
  if (param_3 == (SDL_Surface *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteCreation.c"
               ,PTR____FUNCTION___20701_001c45d4,0x145);
  }
  if (param_10 != '\0') {
    param_3 = (SDL_Surface *)_SWDoSelfMaskingForSurface(param_3);
  }
  if (param_9 == 0) {
    param_9 = (param_3->h / param_5) * (param_3->w / param_4);
  }
  if (param_8 == 0) {
    local_18 = 0;
    local_14 = 0;
  }
  else {
    local_18 = param_6 / 2;
    local_14 = param_7 / 2;
  }
  local_1c = _SWCreateSprite(param_1,param_2,param_9);
  if (local_1c == 0) {
    local_20 = *param_1;
    local_20->sharedSurface = param_3;
    local_28.left = (short)local_18;
    local_28.top = (short)local_14;
    sVar1 = (short)param_4;
    local_28.right = local_28.left + sVar1;
    sVar2 = (short)param_5;
    local_28.bottom = local_28.top + sVar2;
    for (local_10 = 0; local_10 < param_9; local_10 = local_10 + 1) {
      local_1c = _SWCreateFrameFromSurfaceAndRect(&local_2c,param_3,&local_28);
      if (local_1c == 0) {
        local_1c = _SWAddFrame(local_20,local_2c);
        if (local_10 < param_9 + -1) {
          local_28.left = local_28.left + sVar1 + (short)param_6;
          local_28.right = local_28.left + sVar1;
          if (param_3->w < (int)local_28.right) {
            local_28.left = (short)local_18;
            local_28.right = local_28.left + sVar1;
            local_28.top = local_28.top + sVar2 + (short)param_7;
            local_28.bottom = local_28.top + sVar2;
            if (param_3->h < (int)local_28.bottom) {
              local_1c = 0x6b;
            }
          }
        }
      }
      if (local_1c != 0) {
        _SWDisposeFrame(&local_2c);
        _SWDisposeSprite(&local_20);
        break;
      }
    }
  }
  if (local_1c == 0) {
    _SWSetSpriteFrameRange(local_20,0,param_9 + -1);
    local_1c = _SWSetCurrentFrameIndex(local_20,0);
    local_20->unscaledWidth = (float)param_4;
    local_20->unscaledHeight = (float)param_5;
  }
  _SWSetStickyIfError(local_1c);
  return local_1c;
}


// ---- _SWCreateSpriteFromFileSequence @ 0x22b02 (382 bytes)

SWError _SWCreateSpriteFromFileSequence
                  (undefined4 *param_1,undefined4 param_2,char *param_3,int param_4,int param_5,
                  undefined1 param_6)

{
  SpritePtr pSVar1;
  SpritePtr pSVar2;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SpritePtr local_11c;
  FramePtr local_118;
  char local_113 [255];
  int local_14;
  int local_10;
  
  local_14 = 0;
  local_11c = (SpritePtr)*param_1;
  local_14 = _SWCreateSprite(&local_11c,(void *)0x0,param_5);
  if (local_14 == 0) {
    for (local_10 = 0; local_10 < param_5; local_10 = local_10 + 1) {
      _sprintf(local_113,param_3,param_4 + local_10);
      local_14 = _SWCreateFrameFromFile(&local_118,local_113,param_6);
      if (local_14 == 0) {
        local_14 = _SWAddFrame(local_11c,local_118);
      }
      if (local_14 != 0) {
        _SWDisposeFrame(&local_118);
        _SWDisposeSprite(&local_11c);
        break;
      }
    }
  }
  if (local_14 == 0) {
    _SWSetSpriteFrameRange(local_11c,0,param_5 + -1);
    local_14 = _SWSetCurrentFrameIndex(local_11c,0);
    pSVar1 = local_11c;
    *param_1 = local_11c;
    _SWGetSpriteWidth(local_11c);
    pSVar2 = local_11c;
    pSVar1->unscaledWidth = (float)extraout_ST0;
    _SWGetSpriteHeight(local_11c);
    pSVar2->unscaledHeight = (float)extraout_ST0_00;
  }
  return local_14;
}


// ---- _SWCreateSpriteFromFile @ 0x22c80 (215 bytes)

int _SWCreateSpriteFromFile
              (undefined4 *param_1,void *param_2,undefined4 param_3,int param_4,undefined1 param_5)

{
  SpritePtr pSVar1;
  SpritePtr pSVar2;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SpritePtr local_18;
  FramePtr local_14;
  SWError local_10;
  
  local_10 = 0;
  local_14 = (FramePtr)0x0;
  local_18 = (SpritePtr)0x0;
  local_10 = _SWCreateSprite(&local_18,param_2,param_4);
  if (local_10 == 0) {
    local_10 = _SWCreateFrameFromFile(&local_14,param_3,param_5);
  }
  if (local_10 == 0) {
    local_10 = _SWAddFrame(local_18,local_14);
  }
  if (local_10 == 0) {
    local_10 = _SWSetCurrentFrameIndex(local_18,0);
  }
  pSVar1 = local_18;
  if (local_10 == 0) {
    _SWGetSpriteWidth(local_18);
    pSVar2 = local_18;
    pSVar1->unscaledWidth = (float)extraout_ST0;
    _SWGetSpriteHeight(local_18);
    pSVar2->unscaledHeight = (float)extraout_ST0_00;
    *param_1 = local_18;
  }
  return local_10;
}


// ---- _SWUpdateSpriteFromFile @ 0x22d57 (62 bytes)

undefined4 SWUpdateSpriteFromFile(int param_1,undefined4 param_2,undefined1 param_3)

{
  undefined4 uVar1;
  
  uVar1 = _SWUpdateFrameFromFile(*(undefined4 *)(param_1 + 0x7c),param_2,param_3);
  return uVar1;
}


// ---- _SWCreateSpriteFromWorkArea @ 0x22d95 (502 bytes)

int _SWCreateSpriteFromWorkArea(int param_1,undefined4 *param_2,void *param_3,short *param_4)

{
  SpritePtr pSVar1;
  SpritePtr pSVar2;
  undefined4 uVar3;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  SpritePtr local_1c;
  FramePtr local_18;
  SWError local_14;
  int local_10;
  
  local_14 = 0;
  local_18 = (FramePtr)0x0;
  local_1c = (SpritePtr)0x0;
  local_10 = _glGetError();
  if (local_10 != 0) {
    uVar3 = _BKGetGLErrorMessage(local_10);
    _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
             "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteCreation.c",
             PTR____FUNCTION___20888_001c45cc,0x237,uVar3);
  }
  local_14 = _SWCreateSprite(&local_1c,param_3,1);
  if (local_14 == 0) {
    if (param_4 == (short *)0x0) {
      _SWCreateFrameFromWorkArea(param_1,&local_18,param_1 + 0x7c);
    }
    else {
      _SWCreateFrameFromWorkArea(param_1,&local_18,param_4);
    }
  }
  if (local_14 == 0) {
    local_14 = _SWAddFrame(local_1c,local_18);
    if (local_14 == 0) {
      local_14 = _SWSetCurrentFrameIndex(local_1c,0);
    }
    pSVar1 = local_1c;
    if (local_14 == 0) {
      _SWGetSpriteWidth(local_1c);
      pSVar2 = local_1c;
      pSVar1->unscaledWidth = (float)extraout_ST0;
      _SWGetSpriteHeight(local_1c);
      pSVar2->unscaledHeight = (float)extraout_ST0_00;
      _SWSetSpriteFlipY(local_1c,'\x01');
      _SWSetSpriteAutoScaling(local_1c,'\0');
      if (param_4 != (short *)0x0) {
        _SWSetSpriteLocation
                  (local_1c,(float)(((int)param_4[1] + (int)*(short *)(param_1 + 0x30)) -
                                   (int)*(short *)(param_1 + 0x2c)),
                   (float)((int)*param_4 + (int)*(short *)(param_1 + 0x32) +
                          (int)*(short *)(param_1 + 0x2e)));
      }
    }
  }
  if (local_14 == 0) {
    *param_2 = local_1c;
  }
  return local_14;
}


// ---- _SWCloneSprite @ 0x22f8b (660 bytes)

int _SWCloneSprite(void *param_1,undefined4 *param_2,undefined4 *param_3,undefined4 param_4)

{
  int iVar1;
  void *pvVar2;
  undefined4 *local_24;
  undefined4 local_20;
  short local_16;
  int local_10;
  
  local_10 = 0;
  if (param_1 == (void *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteCreation.c"
               ,PTR____FUNCTION___20956_001c45d0,0x2ae);
  }
  if (param_3 == (undefined4 *)0x0) {
    local_24 = (undefined4 *)_calloc(1,*(size_t *)((int)param_1 + 0x14));
  }
  else {
    local_24 = param_3;
  }
  if (local_24 == (undefined4 *)0x0) {
    local_10 = 200;
  }
  if (local_10 == 0) {
    _memcpy(local_24,param_1,0xf8);
    if (param_3 == (undefined4 *)0x0) {
      local_20 = *(undefined4 *)((int)param_1 + 0x14);
    }
    else {
      local_20 = param_4;
    }
    local_24[5] = local_20;
    *local_24 = 0;
    local_24[1] = 0;
    local_24[2] = 0;
    local_24[0x32] = PTR_SWTempSpriteMoveProc_001c4564;
    local_24[0x33] = *(undefined4 *)((int)param_1 + 0xcc);
    pvVar2 = _calloc(1,0x58);
    local_24[0x38] = pvVar2;
    local_24[0x14] = *(undefined4 *)((int)param_1 + 0x50);
    local_24[0x15] = *(undefined4 *)((int)param_1 + 0x54);
    _memcpy((void *)local_24[0x38],*(void **)((int)param_1 + 0xe0),0x58);
    *(undefined4 **)local_24[0x38] = local_24;
  }
  if (local_10 == 0) {
    pvVar2 = _calloc(1,*(int *)((int)param_1 + 0x84) << 2);
    local_24[0x1e] = pvVar2;
    if (local_24[0x1e] == 0) {
      local_10 = 200;
      _free(local_24);
    }
    else {
      for (local_16 = 0; (int)local_16 < *(int *)((int)param_1 + 0x84); local_16 = local_16 + 1) {
        *(undefined4 *)(local_24[0x1e] + local_16 * 4) =
             *(undefined4 *)(*(int *)((int)param_1 + 0x78) + local_16 * 4);
        if (*(int *)(local_24[0x1e] + local_16 * 4) == 0) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteCreation.c"
                     ,PTR____FUNCTION___20956_001c45d0,0x2dd);
        }
        iVar1 = *(int *)(local_24[0x1e] + local_16 * 4);
        *(short *)(iVar1 + 0x32) = *(short *)(iVar1 + 0x32) + 1;
      }
    }
  }
  if (local_10 == 0) {
    *param_2 = local_24;
  }
  _SWSetStickyIfError(local_10);
  return local_10;
}


// ---- _SWDoSelfMaskingForSurface @ 0x2321f (217 bytes)

SDL_Surface * _SWDoSelfMaskingForSurface(SDL_Surface *param_1)

{
  SDL_Surface *local_30;
  undefined4 local_20;
  undefined4 local_1c;
  int local_18;
  int local_14;
  SDL_Surface *local_10;
  
  if ((param_1->flags >> 0x10 & 1) == 0) {
    local_10 = _BKCreateGLSurface(param_1->w,param_1->h,0x20,0,0,1);
    local_1c = 0;
    local_20 = 0;
    local_18 = param_1->w;
    local_14 = param_1->h;
    _SDL_UpperBlit(param_1,&local_20,local_10,&local_20);
    _SDL_FreeSurface(param_1);
    _BKConvertColorKeyToAlphaChannel(local_10,(SDL_Color *)PTR__gSWTransparentColor_001c45d8);
    local_30 = local_10;
  }
  else {
    local_30 = param_1;
  }
  return local_30;
}


