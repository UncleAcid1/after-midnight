// Ghidra decompilation of functions from original source file:
//   ../SpriteWorldX/Sources/Sprite.c

// ---- _SWRemoveSpriteFromAnimation @ 0x1e1f2 (278 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWRemoveSpriteFromAnimation(SpritePtr spriteP,SWBoolean disposeOfSprite)

{
  SpriteWorldRec *pSVar1;
  SWBoolean SVar2;
  undefined3 in_stack_00000009;
  
  SVar2 = disposeOfSprite;
  if (spriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___20626_001c4598,0x2b);
  }
  if ((spriteP->spriteRemoval == 0) && (spriteP->parentSpriteLayerP != (SpriteLayerRec *)0x0)) {
    pSVar1 = spriteP->parentSpriteLayerP->parentSpriteWorldP;
    if (pSVar1 == (SpriteWorldRec *)0x0) {
      _SWRemoveSprite(spriteP);
      if (SVar2 != '\0') {
        _SWDisposeSprite(&spriteP);
      }
    }
    else {
      _SWSetSpriteFrameProc(spriteP,(FrameProcPtr_t)0x0);
      _SWSetSpriteMoveProc(spriteP,(MoveProcPtr_t)0x0);
      _SWSetSpriteVisible(spriteP,'\0');
      _SWRemoveSprite(spriteP);
      _SWAddSprite(pSVar1->deadSpriteLayerP,spriteP);
      if (SVar2 == '\0') {
        spriteP->spriteRemoval = 1;
      }
      else {
        spriteP->spriteRemoval = 2;
      }
    }
  }
  return;
}


// ---- _SWDisposeSprite @ 0x1e308 (267 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWDisposeSprite(SpritePtr *deadSpritePP)

{
  SpritePtr pSVar1;
  int iVar2;
  short local_10;
  char local_d;
  
  pSVar1 = *deadSpritePP;
  local_d = '\0';
  if (pSVar1 != (SpritePtr)0x0) {
    for (local_10 = 0; (int)local_10 < pSVar1->numFrames; local_10 = local_10 + 1) {
      iVar2 = _SWDisposeFrame(pSVar1->frameArray + local_10);
      local_d = (char)iVar2;
    }
    if ((local_d != '\0') && (pSVar1->sharedSurface != (SDL_Surface *)0x0)) {
      _SDL_FreeSurface(pSVar1->sharedSurface);
      pSVar1->sharedSurface = (SDL_Surface *)0x0;
    }
    if (pSVar1->mountedSpriteP != (SpriteRec *)0x0) {
      if (pSVar1->mountedSpriteP->parentSpriteLayerP != (SpriteLayerRec *)0x0) {
        _SWRemoveSprite(pSVar1->mountedSpriteP);
      }
      _SWDisposeSprite(&pSVar1->mountedSpriteP);
    }
    if (pSVar1->mountParentP != (SpriteRec *)0x0) {
      pSVar1->mountParentP->mountedSpriteP = (SpriteRec *)0x0;
    }
    _free(pSVar1->frameArray);
    _free(pSVar1->drawData);
    _free(pSVar1);
    *deadSpritePP = (SpritePtr)0x0;
  }
  return;
}


// ---- _SWLockSprite @ 0x1e413 (123 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWLockSprite(SpritePtr srcSpriteP)

{
  int local_10;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___20703_001c4504,0x9d);
  }
  for (local_10 = 0; local_10 < srcSpriteP->numFrames; local_10 = local_10 + 1) {
    _SWLockFrame(srcSpriteP->frameArray[local_10]);
  }
  return;
}


// ---- _SWUnlockSprite @ 0x1e48e (123 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWUnlockSprite(SpritePtr srcSpriteP)

{
  int local_10;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___20721_001c4560,0xaf);
  }
  for (local_10 = 0; local_10 < srcSpriteP->numFrames; local_10 = local_10 + 1) {
    _SWUnlockFrame(srcSpriteP->frameArray[local_10]);
  }
  return;
}


// ---- _SWAddFrame @ 0x1e509 (215 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWAddFrame(SpritePtr srcSpriteP,FramePtr newFrameP)

{
  SWError local_10;
  
  local_10 = 0;
  if ((srcSpriteP == (SpritePtr)0x0) || (newFrameP == (FramePtr)0x0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___20740_001c4584,0xc4);
  }
  if (srcSpriteP->numFrames < srcSpriteP->maxFrames) {
    srcSpriteP->frameArray[srcSpriteP->numFrames] = newFrameP;
    if (srcSpriteP->numFrames == 0) {
      srcSpriteP->curFrameP = newFrameP;
    }
    srcSpriteP->numFrames = srcSpriteP->numFrames + 1;
    newFrameP->useCount = newFrameP->useCount + 1;
  }
  else {
    local_10 = 0x65;
  }
  _SWSetStickyIfError(local_10);
  return local_10;
}


// ---- _SWInsertFrame @ 0x1e5e0 (272 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError SWInsertFrame(SpritePtr srcSpriteP,FramePtr newFrameP,int frameIndex)

{
  FrameRec **ppFVar1;
  int iStack_24;
  SWError SStack_10;
  
  SStack_10 = 0;
  if ((srcSpriteP == (SpritePtr)0x0) || (newFrameP == (FramePtr)0x0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___20773_001c4558,0xea);
  }
  ppFVar1 = srcSpriteP->frameArray;
  if (((frameIndex < srcSpriteP->maxFrames) && (-1 < frameIndex)) &&
     (srcSpriteP->numFrames < srcSpriteP->maxFrames)) {
    for (iStack_24 = srcSpriteP->numFrames; frameIndex < iStack_24; iStack_24 = iStack_24 + -1) {
      ppFVar1[iStack_24] = ppFVar1[iStack_24 + -1];
    }
    ppFVar1[frameIndex] = newFrameP;
    srcSpriteP->numFrames = srcSpriteP->numFrames + 1;
    newFrameP->useCount = newFrameP->useCount + 1;
  }
  else {
    SStack_10 = 0x65;
  }
  _SWSetStickyIfError(SStack_10);
  return SStack_10;
}


// ---- _SWRemoveFrame @ 0x1e6f0 (211 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWRemoveFrame(SpritePtr srcSpriteP,FramePtr oldFrameP)

{
  FrameRec **ppFVar1;
  int iStack_14;
  
  if ((srcSpriteP == (SpritePtr)0x0) || (oldFrameP == (FramePtr)0x0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___20815_001c451c,0x111);
  }
  iStack_14 = srcSpriteP->numFrames;
  ppFVar1 = srcSpriteP->frameArray;
  do {
    iStack_14 = iStack_14 + -1;
    if (iStack_14 == -1) {
      return;
    }
  } while (ppFVar1[iStack_14] != oldFrameP);
  srcSpriteP->numFrames = srcSpriteP->numFrames + -1;
  oldFrameP->useCount = oldFrameP->useCount - 1;
  for (; iStack_14 <= srcSpriteP->numFrames + -1; iStack_14 = iStack_14 + 1) {
    ppFVar1[iStack_14] = ppFVar1[iStack_14 + 1];
  }
  return;
}


// ---- _SWGetNextFrame @ 0x1e7c3 (181 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

FramePtr SWGetNextFrame(SpritePtr spriteP,FramePtr curFrameP)

{
  FramePtr pFStack_20;
  byte bStack_d;
  
  if (spriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___20854_001c44c0,0x135);
  }
  if (curFrameP == (FramePtr)0x0) {
    pFStack_20 = *spriteP->frameArray;
  }
  else {
    for (bStack_d = 0; (int)(uint)bStack_d < spriteP->numFrames + -2; bStack_d = bStack_d + 1) {
      if (spriteP->frameArray[bStack_d] == curFrameP) {
        return spriteP->frameArray[bStack_d + 1];
      }
    }
    pFStack_20 = (FramePtr)0x0;
  }
  return pFStack_20;
}


// ---- _SWDisposeSoftwareSurfaces @ 0x1e878 (181 bytes)

undefined4 SWDisposeSoftwareSurfaces(int param_1,int param_2)

{
  undefined4 uStack_20;
  byte bStack_d;
  
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___20888_001c4578,0x151);
  }
  if (param_2 == 0) {
    uStack_20 = **(undefined4 **)(param_1 + 0x78);
  }
  else {
    for (bStack_d = 0; (int)(uint)bStack_d < *(int *)(param_1 + 0x80) + -2; bStack_d = bStack_d + 1)
    {
      if (*(int *)(*(int *)(param_1 + 0x78) + (uint)bStack_d * 4) == param_2) {
        return *(undefined4 *)(*(int *)(param_1 + 0x78) + (uint)bStack_d * 4 + 4);
      }
    }
    uStack_20 = 0;
  }
  return uStack_20;
}


// ---- _SWSetSpriteDrawProc @ 0x1e92d (33 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWSetSpriteDrawProc(SpritePtr srcSpriteP,DrawProcPtr_t drawProc)

{
  srcSpriteP->frameDrawProc = drawProc;
  return 0;
}


// ---- _SWStdSpriteDrawProc @ 0x1e94e (1266 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWStdSpriteDrawProc(SpriteRec *srcSpriteP,FrameRec *curFrameP,float horizLoc,float vertLoc)

{
  DrawDataRec *pDVar1;
  code *pcVar2;
  bool bVar3;
  void *pvVar4;
  int iVar5;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  
  if (srcSpriteP == (SpriteRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___20931_001c44c8,0x183);
  }
  pvVar4 = _BKLoadGLfunctions();
  _BKLoadGLcapabilities();
  bVar3 = false;
  pDVar1 = srcSpriteP->drawData;
  _glColor4us(pDVar1->colorRed,pDVar1->colorGreen,pDVar1->colorBlue,
              pDVar1->translucencyLevel & 0xffff);
  (**(code **)((int)pvVar4 + 0x74))(0x1700);
  (**(code **)((int)pvVar4 + 0x7c))();
  (**(code **)((int)pvVar4 + 0x78))();
  if (*(int *)PTR__gSWCurrentSpriteWorld_001c439c != 0) {
    if (pDVar1->autoScaling != '\0') {
      (**(code **)((int)pvVar4 + 0x88))
                (*(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x3c),
                 *(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x40),0x3f800000);
    }
    (**(code **)((int)pvVar4 + 0x88))
              (*(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0xb4),
               *(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0xb8),0x3f800000);
  }
  if (*(int *)PTR__gSWCurrentSpriteWorld_001c439c != 0) {
    (**(code **)((int)pvVar4 + 0x8c))
              (*(uint *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0xac) ^ 0x80000000,
               *(uint *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0xb0) ^ 0x80000000,0);
  }
  if (pDVar1->nonScrolling != '\0') {
    (**(code **)((int)pvVar4 + 0x8c))
              (*(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0xac),
               *(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0xb0),0);
  }
  (**(code **)((int)pvVar4 + 0x8c))(horizLoc,vertLoc,0);
  if (pDVar1->flippedVertical != '\0') {
    pcVar2 = *(code **)((int)pvVar4 + 0x8c);
    _SWGetSpriteHeight(srcSpriteP);
    (*pcVar2)(0,(float)extraout_ST0,0);
    (**(code **)((int)pvVar4 + 0x84))(0x43340000,0x3f800000,0,0);
  }
  if (pDVar1->flippedHorizontal != '\0') {
    pcVar2 = *(code **)((int)pvVar4 + 0x8c);
    _SWGetSpriteWidth(srcSpriteP);
    (*pcVar2)((float)extraout_ST0_00,0,0);
    (**(code **)((int)pvVar4 + 0x84))(0x43340000,0,0x3f800000,0);
  }
  if ((pDVar1->rotation != 0.0) || (NAN(pDVar1->rotation))) {
    (**(code **)((int)pvVar4 + 0x8c))(pDVar1->rotationOffsetHoriz,pDVar1->rotationOffsetVert,0);
    (**(code **)((int)pvVar4 + 0x84))(pDVar1->rotation,0,0,0x3f800000);
  }
  if ((((pDVar1->scaledHeightFactor != 1.0) || (NAN(pDVar1->scaledHeightFactor))) ||
      (pDVar1->scaledWidthFactor != 1.0)) || (NAN(pDVar1->scaledWidthFactor))) {
    (**(code **)((int)pvVar4 + 0x88))
              (pDVar1->scaledWidthFactor,pDVar1->scaledHeightFactor,0x3f800000);
  }
  iVar5 = _DoesSurfaceNeedBlend(curFrameP->glSurfaceP);
  if (((iVar5 != 0) || (pDVar1->translucencyLevel != 0xffff)) || (pDVar1->useBlendMode != '\0')) {
    (**(code **)((int)pvVar4 + 0x14))(0xbe2);
    (**(code **)((int)pvVar4 + 0x50))(pDVar1->srcBlendMode,pDVar1->dstBlendMode);
    bVar3 = true;
  }
  if (pDVar1->useClip == '\0') {
    _BKBlitGLSurface(curFrameP->glSurfaceP);
  }
  else {
    _BKBlitClippedGLSurface
              (curFrameP->glSurfaceP,pDVar1->clipLeft,pDVar1->clipTop,pDVar1->clipRight,
               pDVar1->clipBottom);
  }
  if (bVar3) {
    (**(code **)((int)pvVar4 + 0x18))(0xbe2);
  }
  if ((pDVar1->useClip != '\0') && (pDVar1->temporaryClip != '\0')) {
    pDVar1->useClip = '\0';
  }
  (**(code **)((int)pvVar4 + 0x80))();
  return;
}


// ---- _SWSolidColorSpriteDrawProc @ 0x1ee40 (728 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWSolidColorSpriteDrawProc
               (SpriteRec *srcSpriteP,FrameRec *curFrameP,float horizLoc,float vertLoc)

{
  DrawDataRec *pDVar1;
  void *pvVar2;
  undefined4 *puVar3;
  short sVar4;
  short sVar5;
  
  if (srcSpriteP == (SpriteRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21110_001c4524,0x20c);
  }
  pvVar2 = _BKLoadGLfunctions();
  puVar3 = (undefined4 *)_BKLoadGLcapabilities();
  pDVar1 = srcSpriteP->drawData;
  (**(code **)((int)pvVar2 + 0x30))(*puVar3,&DAT_00002800,&DAT_00002601);
  (**(code **)((int)pvVar2 + 0x30))(*puVar3,&DAT_00002801,&DAT_00002601);
  (**(code **)((int)pvVar2 + 0x18))(*(undefined4 *)PTR__gSWTexExtension_001c4550);
  (**(code **)((int)pvVar2 + 0x74))(0x1700);
  (**(code **)((int)pvVar2 + 0x7c))();
  (**(code **)((int)pvVar2 + 0x78))();
  if (*(int *)PTR__gSWCurrentSpriteWorld_001c439c != 0) {
    (**(code **)((int)pvVar2 + 0x88))
              (*(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x3c),
               *(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x40),0x3f800000);
  }
  (**(code **)((int)pvVar2 + 0x8c))(horizLoc,vertLoc,0);
  if ((((pDVar1->scaledHeightFactor != 1.0) || (NAN(pDVar1->scaledHeightFactor))) ||
      (pDVar1->scaledWidthFactor != 1.0)) || (NAN(pDVar1->scaledWidthFactor))) {
    (**(code **)((int)pvVar2 + 0x88))
              (pDVar1->scaledWidthFactor,pDVar1->scaledHeightFactor,0x3f800000);
  }
  _glColor4us(pDVar1->colorRed,pDVar1->colorGreen,pDVar1->colorBlue,
              pDVar1->translucencyLevel & 0xffff);
  (**(code **)((int)pvVar2 + 0x14))(0xbe2);
  (**(code **)((int)pvVar2 + 0x50))(pDVar1->srcBlendMode,pDVar1->dstBlendMode);
  sVar4 = (srcSpriteP->destFrameRect).right - (srcSpriteP->destFrameRect).left;
  sVar5 = (srcSpriteP->destFrameRect).bottom - (srcSpriteP->destFrameRect).top;
  _glBegin(7);
  (**(code **)((int)pvVar2 + 0x60))(0,0);
  (**(code **)((int)pvVar2 + 0x60))((int)sVar4,0);
  (**(code **)((int)pvVar2 + 0x60))((int)sVar4,(int)sVar5);
  (**(code **)((int)pvVar2 + 0x60))(0,(int)sVar5);
  (**(code **)((int)pvVar2 + 4))();
  (**(code **)((int)pvVar2 + 0x80))();
  (**(code **)((int)pvVar2 + 0x18))(0xbe2);
  (**(code **)((int)pvVar2 + 0x14))(*(undefined4 *)PTR__gSWTexExtension_001c4550);
  return;
}


// ---- _SWTempSpriteMoveProc @ 0x1f118 (73 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWTempSpriteMoveProc(SpriteRec *spriteP)

{
  spriteP->spriteMoveProc = spriteP->realSpriteMoveProc;
  if ((spriteP->needsToBeDrawn == '\0') && (spriteP->realSpriteMoveProc != (MoveProcPtr_t)0x0)) {
    (*spriteP->realSpriteMoveProc)(spriteP);
  }
  return;
}


// ---- _SWSetCurrentFrame @ 0x1f161 (201 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError SWSetCurrentFrame(SpritePtr srcSpriteP,FramePtr curFrameP)

{
  bool bVar1;
  short sStack_14;
  SWError SStack_10;
  
  bVar1 = false;
  SStack_10 = 0;
  if ((srcSpriteP == (SpritePtr)0x0) || (curFrameP == (FramePtr)0x0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21195_001c45a0,0x263);
  }
  sStack_14 = 0;
  do {
    if (srcSpriteP->numFrames <= (int)sStack_14) {
LAB_0001f209:
      if (!bVar1) {
        SStack_10 = 0x72;
      }
      _SWSetStickyIfError(SStack_10);
      return SStack_10;
    }
    if (srcSpriteP->frameArray[sStack_14] == curFrameP) {
      _SWSetCurrentFrameIndex(srcSpriteP,(int)sStack_14);
      bVar1 = true;
      goto LAB_0001f209;
    }
    sStack_14 = sStack_14 + 1;
  } while( true );
}


// ---- _SWSetCurrentFrameIndex @ 0x1f22a (753 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWSetCurrentFrameIndex(SpritePtr srcSpriteP,int frameIndex)

{
  short sVar1;
  short sVar2;
  FrameRec *pFVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  SWError local_14;
  
  local_14 = 0;
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21226_001c4538,0x283);
  }
  if ((frameIndex < 0) && (frameIndex < 0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21226_001c4538,0x285);
  }
  if (frameIndex < srcSpriteP->numFrames) {
    pFVar3 = srcSpriteP->frameArray[frameIndex];
    if (srcSpriteP->curFrameP == (FrameRec *)0x0) {
      srcSpriteP->curFrameP = pFVar3;
    }
    sVar1 = ((srcSpriteP->destFrameRect).left - (pFVar3->frameRect).left) +
            ((short)srcSpriteP->curFrameP->hotSpotH - (short)pFVar3->hotSpotH);
    sVar2 = ((srcSpriteP->destFrameRect).top - (pFVar3->frameRect).top) +
            ((short)srcSpriteP->curFrameP->hotSpotV - (short)pFVar3->hotSpotV);
    srcSpriteP->curFrameP = pFVar3;
    sVar4 = (pFVar3->frameRect).left;
    sVar5 = (pFVar3->frameRect).bottom;
    sVar6 = (pFVar3->frameRect).right;
    (srcSpriteP->destFrameRect).top = (pFVar3->frameRect).top;
    (srcSpriteP->destFrameRect).left = sVar4;
    (srcSpriteP->destFrameRect).bottom = sVar5;
    (srcSpriteP->destFrameRect).right = sVar6;
    if (0.0 < srcSpriteP->drawData->scaledWidth) {
      (srcSpriteP->destFrameRect).right =
           (short)(int)(srcSpriteP->drawData->scaledWidth +
                       (float)(int)(srcSpriteP->destFrameRect).left);
      srcSpriteP->drawData->scaledWidthFactor =
           srcSpriteP->drawData->scaledWidth /
           (float)((int)(pFVar3->frameRect).right - (int)(pFVar3->frameRect).left);
    }
    if (0.0 < srcSpriteP->drawData->scaledHeight) {
      (srcSpriteP->destFrameRect).bottom =
           (short)(int)(srcSpriteP->drawData->scaledHeight +
                       (float)(int)(srcSpriteP->destFrameRect).top);
      srcSpriteP->drawData->scaledHeightFactor =
           srcSpriteP->drawData->scaledHeight /
           (float)((int)(pFVar3->frameRect).bottom - (int)(pFVar3->frameRect).top);
    }
    srcSpriteP->drawData->frameHotSpotH = (short)srcSpriteP->curFrameP->hotSpotH;
    srcSpriteP->drawData->frameHotSpotV = (short)srcSpriteP->curFrameP->hotSpotV;
    (srcSpriteP->destFrameRect).left = (srcSpriteP->destFrameRect).left + sVar1;
    (srcSpriteP->destFrameRect).right = (srcSpriteP->destFrameRect).right + sVar1;
    (srcSpriteP->destFrameRect).top = (srcSpriteP->destFrameRect).top + sVar2;
    (srcSpriteP->destFrameRect).bottom = (srcSpriteP->destFrameRect).bottom + sVar2;
    srcSpriteP->curFrameIndex = frameIndex;
    srcSpriteP->needsToBeDrawn = '\x01';
  }
  else {
    local_14 = 0x66;
  }
  return local_14;
}


// ---- _SWSetSpriteCollideProc @ 0x1f51b (84 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteCollideProc(SpritePtr srcSpriteP,CollideProcPtr_t collideProc)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21328_001c4590,0x2c1);
  }
  srcSpriteP->spriteCollideProc = collideProc;
  return;
}


// ---- _SWSetSpriteFrameAdvance @ 0x1f56f (84 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteFrameAdvance(SpritePtr srcSpriteP,int frameAdvance)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21336_001c45a8,0x2da);
  }
  srcSpriteP->frameAdvance = frameAdvance;
  return;
}


// ---- _SWSetSpriteFrameAdvanceMode @ 0x1f5c3 (84 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteFrameAdvanceMode(SpritePtr srcSpriteP,void *advanceMode)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21344_001c44ec,0x2e7);
  }
  srcSpriteP->frameAdvanceMode = (int)advanceMode;
  return;
}


// ---- _SWSetSpriteFrameRange @ 0x1f617 (203 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteFrameRange(SpritePtr srcSpriteP,int firstFrameIndex,int lastFrameIndex)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21353_001c44f4,0x2f5);
  }
  if (firstFrameIndex < 0) {
    firstFrameIndex = 0;
  }
  if (srcSpriteP->maxFrames + -1 < lastFrameIndex) {
    lastFrameIndex = srcSpriteP->maxFrames + -1;
  }
  srcSpriteP->firstFrameIndex = firstFrameIndex;
  srcSpriteP->lastFrameIndex = lastFrameIndex;
  if (srcSpriteP->curFrameIndex < firstFrameIndex) {
    _SWSetCurrentFrameIndex(srcSpriteP,firstFrameIndex);
  }
  else if (lastFrameIndex < srcSpriteP->curFrameIndex) {
    _SWSetCurrentFrameIndex(srcSpriteP,lastFrameIndex);
  }
  return;
}


// ---- _SWSetSpriteFrameTime @ 0x1f6e2 (167 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteFrameTime(SpritePtr srcSpriteP,int timeInterval)

{
  SpriteWorldRec *local_10;
  
  local_10 = (SpriteWorldRec *)0x0;
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21376_001c4510,0x312);
  }
  if (srcSpriteP->frameTimeInterval != timeInterval) {
    srcSpriteP->frameTimeInterval = timeInterval;
    if (srcSpriteP->parentSpriteLayerP != (SpriteLayerRec *)0x0) {
      local_10 = srcSpriteP->parentSpriteLayerP->parentSpriteWorldP;
    }
    if (local_10 == (SpriteWorldRec *)0x0) {
      local_10 = *(SpriteWorldRec **)PTR__gSWCurrentSpriteWorld_001c439c;
    }
    if (local_10 != (SpriteWorldRec *)0x0) {
      srcSpriteP->timeOfLastFrameChange = local_10->runningTimeCount;
    }
  }
  return;
}


// ---- _SWSetSpriteFrameProc @ 0x1f789 (84 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteFrameProc(SpritePtr srcSpriteP,FrameProcPtr_t frameProc)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21396_001c4518,0x32c);
  }
  srcSpriteP->frameChangeProc = frameProc;
  return;
}


// ---- _SWSetSpriteLocation @ 0x1f7dd (305 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteLocation(SpritePtr srcSpriteP,SWMovementType horizLoc,SWMovementType vertLoc)

{
  short sVar1;
  short sVar2;
  short sVar3;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21407_001c44bc,0x33c);
  }
  srcSpriteP->drawData->horizLoc = horizLoc;
  srcSpriteP->drawData->vertLoc = vertLoc;
  sVar2 = (short)(int)horizLoc - srcSpriteP->drawData->frameHotSpotH;
  sVar3 = (short)(int)vertLoc - srcSpriteP->drawData->frameHotSpotV;
  (srcSpriteP->destFrameRect).bottom =
       ((srcSpriteP->destFrameRect).bottom - (srcSpriteP->destFrameRect).top) + sVar3;
  (srcSpriteP->destFrameRect).right =
       ((srcSpriteP->destFrameRect).right - (srcSpriteP->destFrameRect).left) + sVar2;
  (srcSpriteP->destFrameRect).top = sVar3;
  (srcSpriteP->destFrameRect).left = sVar2;
  sVar3 = (srcSpriteP->destFrameRect).bottom;
  sVar1 = (srcSpriteP->destFrameRect).right;
  sVar2 = (srcSpriteP->destFrameRect).left;
  (srcSpriteP->deltaFrameRect).top = (srcSpriteP->destFrameRect).top;
  (srcSpriteP->deltaFrameRect).left = sVar2;
  (srcSpriteP->deltaFrameRect).bottom = sVar3;
  (srcSpriteP->deltaFrameRect).right = sVar1;
  sVar3 = (srcSpriteP->destFrameRect).bottom;
  sVar1 = (srcSpriteP->destFrameRect).right;
  sVar2 = (srcSpriteP->destFrameRect).left;
  (srcSpriteP->oldFrameRect).top = (srcSpriteP->destFrameRect).top;
  (srcSpriteP->oldFrameRect).left = sVar2;
  (srcSpriteP->oldFrameRect).bottom = sVar3;
  (srcSpriteP->oldFrameRect).right = sVar1;
  srcSpriteP->needsToBeDrawn = '\x01';
  return;
}


// ---- _SWSetSpriteMoveBounds @ 0x1f90e (95 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteMoveBounds(SpritePtr srcSpriteP,SWRect *moveBoundsRect)

{
  short sVar1;
  short sVar2;
  short sVar3;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21433_001c4508,0x35a);
  }
  sVar2 = moveBoundsRect->bottom;
  sVar3 = moveBoundsRect->right;
  sVar1 = moveBoundsRect->left;
  (srcSpriteP->moveBoundsRect).top = moveBoundsRect->top;
  (srcSpriteP->moveBoundsRect).left = sVar1;
  (srcSpriteP->moveBoundsRect).bottom = sVar2;
  (srcSpriteP->moveBoundsRect).right = sVar3;
  return;
}


// ---- _SWSetSpriteMoveDelta @ 0x1f96d (102 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteMoveDelta(SpritePtr srcSpriteP,SWMovementType horizDelta,SWMovementType vertDelta)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21443_001c455c,0x368);
  }
  srcSpriteP->drawData->horizMoveDelta = horizDelta;
  srcSpriteP->drawData->vertMoveDelta = vertDelta;
  return;
}


// ---- _SWSetSpriteMoveTime @ 0x1f9d3 (84 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteMoveTime(SpritePtr srcSpriteP,int timeInterval)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21453_001c45b0,0x378);
  }
  srcSpriteP->moveTimeInterval = timeInterval;
  return;
}


// ---- _SWGetSpriteMoveTime @ 0x1fa27 (81 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int SWGetSpriteMoveTime(SpritePtr srcSpriteP)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21460_001c44c4,899);
  }
  return srcSpriteP->moveTimeInterval;
}


// ---- _SWSetSpriteMoveProc @ 0x1fa78 (117 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteMoveProc(SpritePtr srcSpriteP,MoveProcPtr_t moveProc)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21470_001c459c,0x390);
  }
  srcSpriteP->realSpriteMoveProc = moveProc;
  if (srcSpriteP->spriteMoveProc != (MoveProcPtr_t)PTR_SWTempSpriteMoveProc_001c4564) {
    srcSpriteP->spriteMoveProc = moveProc;
  }
  return;
}


// ---- _SWSetSpritePseudoMoveTime @ 0x1faed (88 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWSetSpritePseudoMoveTime(SpritePtr srcSpriteP,int timeInterval)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21481_001c45c0,0x3a2);
  }
  srcSpriteP->pseudoMoveTimeInterval = (double)timeInterval;
  return;
}


// ---- _SWSetSpriteVisible @ 0x1fb45 (151 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteVisible(SpritePtr srcSpriteP,SWBoolean isVisible)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21490_001c45a4,0x3b2);
  }
  srcSpriteP->isVisible = isVisible;
  srcSpriteP->needsToBeDrawn = '\x01';
  srcSpriteP->needsToBeErased = isVisible == '\0';
  if ((srcSpriteP->mountedSpriteP != (SpriteRec *)0x0) && (srcSpriteP->inheritVisible != '\0')) {
    _SWSetSpriteVisible(srcSpriteP->mountedSpriteP,isVisible);
  }
  return;
}


// ---- _SWSetSpriteTranslucency @ 0x1fbdc (32 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteTranslucency(SpritePtr srcSpriteP,uint translucency)

{
  _SWSetSpriteTranslucencyLevel(srcSpriteP,translucency);
  return;
}


// ---- _SWSetSpriteTranslucencyLevel @ 0x1fbfc (129 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteTranslucencyLevel(SpritePtr srcSpriteP,uint level)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21511_001c457c,0x3d1);
  }
  srcSpriteP->drawData->translucencyLevel = level;
  if ((srcSpriteP->mountedSpriteP != (SpriteRec *)0x0) && (srcSpriteP->inheritTranslucency != '\0'))
  {
    _SWSetSpriteTranslucencyLevel(srcSpriteP->mountedSpriteP,level);
  }
  return;
}


// ---- _SWSetSpriteColor @ 0x1fc7d (205 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteColor(SpritePtr srcSpriteP,ushort red,ushort green,ushort blue)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21529_001c44cc,0x3e4);
  }
  srcSpriteP->drawData->colorRed = red;
  srcSpriteP->drawData->colorGreen = green;
  srcSpriteP->drawData->colorBlue = blue;
  if ((srcSpriteP->mountedSpriteP != (SpriteRec *)0x0) && (srcSpriteP->inheritAttr2 != '\0')) {
    _SWSetSpriteColor(srcSpriteP->mountedSpriteP,red,green,blue);
  }
  return;
}


// ---- _SWSetSpriteBlendMode @ 0x1fd4a (115 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteBlendMode(SpritePtr srcSpriteP,uint srcBlendMode,uint dstBlendMode)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21551_001c44e8,0x3ff);
  }
  srcSpriteP->drawData->useBlendMode = '\x01';
  srcSpriteP->drawData->srcBlendMode = srcBlendMode;
  srcSpriteP->drawData->dstBlendMode = dstBlendMode;
  return;
}


// ---- _SWSetSpriteTemporaryClip @ 0x1fdbd (103 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteTemporaryClip(SpriteRec *spriteP,short left,short top,short right,short bottom)

{
  spriteP->drawData->temporaryClip = '\x01';
  _SWSetSpriteClip(spriteP,(int)left,(int)top,(int)right,(int)bottom);
  return;
}


// ---- _SWSetSpriteClip @ 0x1fe24 (228 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteClip(SpriteRec *spriteP,int left,int top,int right,int bottom)

{
  DrawDataRec *pDVar1;
  
  if (spriteP == (SpriteRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21577_001c45b8,0x41b);
  }
  pDVar1 = spriteP->drawData;
  if (((((short)left == 0) && ((short)top == 0)) && ((short)right == 0)) && ((short)bottom == 0)) {
    pDVar1->useClip = '\0';
  }
  else {
    pDVar1->useClip = '\x01';
    pDVar1->clipLeft = (short)left;
    pDVar1->clipTop = (short)top;
    pDVar1->clipRight = (short)right;
    pDVar1->clipBottom = (short)bottom;
  }
  return;
}


// ---- _SWSetSpriteFlipX @ 0x1ff08 (137 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteFlipX(SpriteRec *spriteP,uchar flip)

{
  if (spriteP == (SpriteRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21597_001c45bc,0x440);
  }
  spriteP->drawData->flippedHorizontal = flip;
  if ((spriteP->mountedSpriteP != (SpriteRec *)0x0) && (spriteP->inheritFlip != '\0')) {
    _SWSetSpriteFlipX(spriteP->mountedSpriteP,flip);
  }
  return;
}


// ---- _SWSetSpriteFlipY @ 0x1ff91 (137 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteFlipY(SpriteRec *spriteP,uchar flip)

{
  if (spriteP == (SpriteRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21614_001c4588,0x44e);
  }
  spriteP->drawData->flippedVertical = flip;
  if ((spriteP->mountedSpriteP != (SpriteRec *)0x0) && (spriteP->inheritFlip != '\0')) {
    _SWSetSpriteFlipX(spriteP->mountedSpriteP,flip);
  }
  return;
}


// ---- _SWSetSpriteRotation @ 0x2001a (398 bytes)

void _SWSetSpriteRotation(int param_1,float param_2)

{
  longdouble lVar1;
  longdouble lVar2;
  float fVar3;
  float fVar4;
  
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21635_001c453c,0x45d);
  }
  fVar3 = (float)(((int)*(short *)(param_1 + 0x3c) - (int)*(short *)(param_1 + 0x38)) / 2);
  fVar4 = (float)(((int)*(short *)(param_1 + 0x3a) - (int)*(short *)(param_1 + 0x36)) / 2);
  lVar1 = (longdouble)_SWSin((double)param_2);
  lVar2 = (longdouble)_SWCos((double)param_2);
  *(float *)(*(int *)(param_1 + 0xe0) + 0x1c) =
       -(float)lVar1 * fVar4 + -(float)lVar2 * fVar3 + fVar3;
  *(float *)(*(int *)(param_1 + 0xe0) + 0x20) = -(float)lVar2 * fVar4 + fVar3 * (float)lVar1 + fVar4
  ;
  *(float *)(*(int *)(param_1 + 0xe0) + 0x18) = param_2;
  if ((*(int *)(param_1 + 0x18) != 0) && (*(char *)(param_1 + 0x30) != '\0')) {
    _SWSetSpriteFlipX(*(SpriteRec **)(param_1 + 0x18),(uchar)(int)param_2);
  }
  return;
}


// ---- _SWSetSpriteScaledSize @ 0x201a8 (108 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteScaledSize(SpritePtr newSpriteP,short width,short height)

{
  if (newSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21685_001c45b4,0x476);
  }
  _SWSetSpriteScaledWidth(newSpriteP,width);
  _SWSetSpriteScaledHeight(newSpriteP,height);
  return;
}


// ---- _SWSetSpriteScaledWidth @ 0x20214 (296 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteScaledWidth(SpritePtr srcSpriteP,short width)

{
  float fVar1;
  undefined2 in_stack_0000000a;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21695_001c44f8,0x486);
  }
  fVar1 = srcSpriteP->drawData->scaledWidthFactor;
  srcSpriteP->drawData->scaledWidth = _width;
  _SWSetCurrentFrameIndex(srcSpriteP,srcSpriteP->curFrameIndex);
  if (srcSpriteP->scalingMode != '\0') {
    _SWOffsetSprite(srcSpriteP,
                    -((srcSpriteP->drawData->scaledWidthFactor * srcSpriteP->unscaledWidth -
                      srcSpriteP->unscaledWidth * fVar1) / 2.0),0.0);
  }
  if ((srcSpriteP->mountedSpriteP != (SpriteRec *)0x0) && (srcSpriteP->inheritAttr3 != '\0')) {
    _SWSetSpriteScaledWidth(srcSpriteP->mountedSpriteP,width);
  }
  return;
}


// ---- _SWSetSpriteScaledHeight @ 0x2033c (296 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteScaledHeight(SpritePtr srcSpriteP,short height)

{
  float fVar1;
  undefined2 in_stack_0000000a;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21728_001c44e4,0x4a2);
  }
  fVar1 = srcSpriteP->drawData->scaledHeightFactor;
  srcSpriteP->drawData->scaledHeight = _height;
  _SWSetCurrentFrameIndex(srcSpriteP,srcSpriteP->curFrameIndex);
  if (srcSpriteP->scalingMode != '\0') {
    _SWOffsetSprite(srcSpriteP,0.0,
                    -((srcSpriteP->drawData->scaledHeightFactor * srcSpriteP->unscaledHeight -
                      srcSpriteP->unscaledHeight * fVar1) / 2.0));
  }
  if ((srcSpriteP->mountedSpriteP != (SpriteRec *)0x0) && (srcSpriteP->inheritAttr3 != '\0')) {
    _SWSetSpriteScaledHeight(srcSpriteP->mountedSpriteP,height);
  }
  return;
}


// ---- _SWSetSpriteScaledWidthFactor @ 0x20464 (102 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteScaledWidthFactor(SpritePtr srcSpriteP,float widthFactor)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21759_001c44f0,0x4be);
  }
  _SWSetSpriteScaledWidth(srcSpriteP,SUB42(srcSpriteP->unscaledWidth * widthFactor,0));
  return;
}


// ---- _SWSetSpriteScaledHeightFactor @ 0x204ca (102 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteScaledHeightFactor(SpritePtr srcSpriteP,float heightFactor)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21769_001c454c,0x4cc);
  }
  _SWSetSpriteScaledHeight(srcSpriteP,SUB42(srcSpriteP->unscaledHeight * heightFactor,0));
  return;
}


// ---- _SWSetSpriteScaledFactor @ 0x20530 (138 bytes)

void _SWSetSpriteScaledFactor(SpritePtr param_1,float param_2,float param_3)

{
  if (param_1 == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21780_001c45ac,0x4dc);
  }
  _SWSetSpriteScaledWidth(param_1,SUB42(param_1->unscaledWidth * param_2,0));
  _SWSetSpriteScaledHeight(param_1,SUB42(param_1->unscaledHeight * param_3,0));
  return;
}


// ---- _SWSetSpriteScalingMode @ 0x205ba (33 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteScalingMode(SpriteRec *spriteP,uchar mode)

{
  spriteP->scalingMode = mode;
  return;
}


// ---- _SWSetSpriteFastMovingCheck @ 0x205db (88 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWSetSpriteFastMovingCheck(SpritePtr srcSpriteP,SWBoolean makeChecks)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21798_001c45c4,0x4fd);
  }
  srcSpriteP->doFastMovingCheck = makeChecks;
  return;
}


// ---- _SWInvalidateSprite @ 0x20633 (21 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWInvalidateSprite(SpritePtr srcSpriteP)

{
  srcSpriteP->needsToBeDrawn = '\x01';
  return;
}


// ---- _SWMountSprite @ 0x20648 (140 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWMountSprite(SpriteRec *baseSpriteP,SpriteRec *riderSpriteP,float offsetH,float offsetV,
                   uchar flag1,uchar flag2,uchar flag3)

{
  baseSpriteP->mountedSpriteP = riderSpriteP;
  riderSpriteP->mountParentP = baseSpriteP;
  riderSpriteP->mountOffsetH = offsetH;
  riderSpriteP->mountOffsetV = offsetV;
  baseSpriteP->mountFlag1 = flag1;
  baseSpriteP->mountFlag2 = flag2;
  baseSpriteP->mountFlag3 = flag3;
  baseSpriteP->inheritTranslucency = '\x01';
  baseSpriteP->inheritAttr2 = '\x01';
  baseSpriteP->inheritAttr3 = '\x01';
  baseSpriteP->inheritFlip = '\x01';
  baseSpriteP->inheritAttr5 = '\x01';
  baseSpriteP->inheritVisible = '\x01';
  return;
}


// ---- _SWSetMountInheritsAttributes @ 0x206d4 (135 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetMountInheritsAttributes
               (SpriteRec *spriteP,uchar translucency,uchar attr2,uchar attr3,uchar flip,uchar attr5
               ,uchar visible)

{
  spriteP->inheritTranslucency = translucency;
  spriteP->inheritAttr2 = attr2;
  spriteP->inheritAttr3 = attr3;
  spriteP->inheritFlip = flip;
  spriteP->inheritAttr5 = attr5;
  spriteP->inheritVisible = visible;
  return;
}


// ---- _SWSetSpriteMountOffset @ 0x2075b (32 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteMountOffset(SpriteRec *spriteP,float offsetH,float offsetV)

{
  spriteP->mountOffsetH = offsetH;
  spriteP->mountOffsetV = offsetV;
  return;
}


// ---- _SWSetGlobalMountOffset @ 0x2077b (38 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetGlobalMountOffset(SpriteWorldRec *spriteWorldP,float offsetH,float offsetV)

{
  spriteWorldP->globalMountOffsetH = offsetH;
  spriteWorldP->globalMountOffsetV = offsetV;
  return;
}


// ---- _SWSetDrawTilesOverSpriteInsetRect @ 0x207a1 (190 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetDrawTilesOverSpriteInsetRect
               (SpriteRec *spriteP,short left,short top,short right,short bottom)

{
  if (spriteP == (SpriteRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21840_001c4528,0x56f);
  }
  if ((((left == 0) && (top == 0)) && (right == 0)) && (bottom == 0)) {
    spriteP->drawTilesOverSprite = '\0';
  }
  else {
    spriteP->drawTilesOverSprite = '\x01';
    (spriteP->drawTilesInsetRect).top = top;
    (spriteP->drawTilesInsetRect).left = left;
    (spriteP->drawTilesInsetRect).bottom = bottom;
    (spriteP->drawTilesInsetRect).right = right;
  }
  return;
}


// ---- _SWSetSpriteAutoScaling @ 0x2085f (36 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteAutoScaling(SpriteRec *spriteP,uchar autoScale)

{
  spriteP->drawData->autoScaling = autoScale;
  return;
}


// ---- _SWSetSpriteNonScrolling @ 0x20883 (36 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteNonScrolling(SpriteRec *spriteP,uchar nonScrolling)

{
  spriteP->drawData->nonScrolling = nonScrolling;
  return;
}


// ---- _SWGetSpriteFrameTime @ 0x208a7 (81 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int _SWGetSpriteFrameTime(SpritePtr srcSpriteP)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21863_001c44fc,0x5ab);
  }
  return srcSpriteP->frameTimeInterval;
}


// ---- _SWGetSpriteVertLoc @ 0x208f8 (100 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWMovementType _SWGetSpriteVertLoc(SpritePtr srcSpriteP)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21872_001c4520,0x5b8);
  }
  return srcSpriteP->drawData->vertLoc;
}


// ---- _SWGetSpriteHorizLoc @ 0x2095c (154 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWMovementType _SWGetSpriteHorizLoc(SpritePtr srcSpriteP)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21882_001c44dc,0x5c4);
  }
  if (srcSpriteP->drawData == (DrawDataRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21882_001c44dc,0x5c5);
  }
  return srcSpriteP->drawData->horizLoc;
}


// ---- _SWGetSpriteHorizMoveDelta @ 0x209f6 (100 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWMovementType _SWGetSpriteHorizMoveDelta(SpritePtr srcSpriteP)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21896_001c4500,0x5d0);
  }
  return srcSpriteP->drawData->horizMoveDelta;
}


// ---- _SWGetSpriteVertMoveDelta @ 0x20a5a (100 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWMovementType _SWGetSpriteVertMoveDelta(SpritePtr srcSpriteP)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21906_001c44d8,0x5dc);
  }
  return srcSpriteP->drawData->vertMoveDelta;
}


// ---- _SWGetSpriteWidth @ 0x20abe (160 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

short _SWGetSpriteWidth(SpritePtr srcSpriteP)

{
  short sVar1;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21916_001c4514,0x5e9);
  }
  if (0.0 < srcSpriteP->drawData->scaledWidth) {
    sVar1 = (short)srcSpriteP->drawData;
  }
  else {
    sVar1 = (srcSpriteP->destFrameRect).right - (srcSpriteP->destFrameRect).left;
  }
  return sVar1;
}


// ---- _SWGetSpriteHeight @ 0x20b5e (160 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

short _SWGetSpriteHeight(SpritePtr srcSpriteP)

{
  short sVar1;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21935_001c4544,0x5f8);
  }
  if (0.0 < srcSpriteP->drawData->scaledHeight) {
    sVar1 = (short)srcSpriteP->drawData;
  }
  else {
    sVar1 = (srcSpriteP->destFrameRect).bottom - (srcSpriteP->destFrameRect).top;
  }
  return sVar1;
}


// ---- _SWGetSpriteTranslucencyLevel @ 0x20bfe (84 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

uint _SWGetSpriteTranslucencyLevel(SpritePtr srcSpriteP)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21954_001c4570,0x607);
  }
  return srcSpriteP->drawData->translucencyLevel;
}


// ---- _SWGetSpriteVisible @ 0x20c52 (82 bytes)

undefined1 SWGetSpriteVisible(int param_1)

{
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21964_001c452c,0x612);
  }
  return *(undefined1 *)(param_1 + 0x32);
}


// ---- _SWGetCurrentFrameIndex @ 0x20ca4 (81 bytes)

undefined4 SWGetCurrentFrameIndex(int param_1)

{
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21974_001c4534,0x61e);
  }
  return *(undefined4 *)(param_1 + 0x94);
}


// ---- _SWMoveSprite @ 0x20cf5 (295 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWMoveSprite(SpritePtr srcSpriteP,SWMovementType horizLoc,SWMovementType vertLoc)

{
  short sVar1;
  short sVar2;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___21987_001c4540,0x633);
  }
  srcSpriteP->drawData->horizLoc = horizLoc;
  srcSpriteP->drawData->vertLoc = vertLoc;
  sVar1 = (short)(int)horizLoc - srcSpriteP->drawData->frameHotSpotH;
  sVar2 = (short)(int)vertLoc - srcSpriteP->drawData->frameHotSpotV;
  if (((srcSpriteP->destFrameRect).left != sVar1) || ((srcSpriteP->destFrameRect).top != sVar2)) {
    (srcSpriteP->destFrameRect).bottom =
         ((srcSpriteP->destFrameRect).bottom - (srcSpriteP->destFrameRect).top) + sVar2;
    (srcSpriteP->destFrameRect).right =
         ((srcSpriteP->destFrameRect).right - (srcSpriteP->destFrameRect).left) + sVar1;
    (srcSpriteP->destFrameRect).top = sVar2;
    (srcSpriteP->destFrameRect).left = sVar1;
    srcSpriteP->needsToBeDrawn = '\x01';
  }
  return;
}


// ---- _SWOffsetSprite @ 0x20e1c (219 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWOffsetSprite(SpritePtr srcSpriteP,SWMovementType horizOffset,SWMovementType vertOffset)

{
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22017_001c45c8,0x653);
  }
  if ((((horizOffset != 0.0) || (NAN(horizOffset))) || (vertOffset != 0.0)) || (NAN(vertOffset))) {
    srcSpriteP->drawData->horizLoc = srcSpriteP->drawData->horizLoc + horizOffset;
    srcSpriteP->drawData->vertLoc = srcSpriteP->drawData->vertLoc + vertOffset;
    _SWMoveSprite(srcSpriteP,srcSpriteP->drawData->horizLoc,srcSpriteP->drawData->vertLoc);
  }
  return;
}


// ---- _SWOffsetSpriteBasedOnTime @ 0x20ef7 (293 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWOffsetSpriteBasedOnTime
               (SpritePtr theSpriteP,SWMovementType horizOffset,SWMovementType vertOffset)

{
  longdouble lVar1;
  longdouble lVar2;
  SpriteWorldRec *local_28;
  double local_14;
  
  local_28 = (SpriteWorldRec *)0x0;
  if (theSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22045_001c44e0,0x66d);
  }
  if (theSpriteP->parentSpriteLayerP != (SpriteLayerRec *)0x0) {
    local_28 = theSpriteP->parentSpriteLayerP->parentSpriteWorldP;
  }
  if (local_28 != (SpriteWorldRec *)0x0) {
    if (0.0 < theSpriteP->pseudoMoveTimeInterval) {
      local_14 = 1000.0 / theSpriteP->pseudoMoveTimeInterval;
    }
    else {
      local_14 = local_28->pseudoFPS;
    }
    lVar1 = (longdouble)_SWConvertOffsetBasedOnTime(local_28,(double)horizOffset,local_14);
    lVar2 = (longdouble)_SWConvertOffsetBasedOnTime(local_28,(double)vertOffset,local_14);
    _SWOffsetSprite(theSpriteP,(float)lVar1,(float)lVar2);
  }
  return;
}


// ---- _SWConvertOffsetBasedOnTime @ 0x2101c (150 bytes)

longdouble
_SWConvertOffsetBasedOnTime
          (int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5)

{
  return (longdouble)
         (((double)CONCAT44(param_3,param_2) *
           ((double)(*(int *)(param_1 + 0x134) + -0x80000000) + 2147483648.0) *
          (double)CONCAT44(param_5,param_4)) / 1000.0);
}


// ---- _SWResetSpriteFrameTimer @ 0x210b2 (32 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWResetSpriteFrameTimer(SpriteWorldPtr spriteWorldP,SpritePtr srcSpriteP)

{
  srcSpriteP->timeOfLastFrameChange = spriteWorldP->runningTimeCount;
  return;
}


// ---- _SWHasMoveProcFired @ 0x210d2 (85 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

uchar SWHasMoveProcFired(SpriteRec *spriteP)

{
  if (spriteP == (SpriteRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22088_001c4554,0x6a5);
  }
  return spriteP->moveProcHasFired;
}


// ---- _SWBounceSprite @ 0x21127 (666 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWBoolean SWBounceSprite(SpritePtr srcSpriteP)

{
  SWBoolean SVar1;
  short sStack_10;
  short sStack_e;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22100_001c4594,0x6b3);
  }
  if (((srcSpriteP->moveBoundsRect).left <= (srcSpriteP->destFrameRect).left) ||
     (0.0 <= srcSpriteP->drawData->horizMoveDelta)) {
    if (((srcSpriteP->destFrameRect).right <= (srcSpriteP->moveBoundsRect).right) ||
       (srcSpriteP->drawData->horizMoveDelta <= 0.0)) {
      sStack_10 = 0;
    }
    else {
      srcSpriteP->drawData->horizMoveDelta = -srcSpriteP->drawData->horizMoveDelta;
      sStack_10 = ((srcSpriteP->moveBoundsRect).right - (srcSpriteP->destFrameRect).right) * 2;
    }
  }
  else {
    srcSpriteP->drawData->horizMoveDelta = -srcSpriteP->drawData->horizMoveDelta;
    sStack_10 = ((srcSpriteP->moveBoundsRect).left - (srcSpriteP->destFrameRect).left) * 2;
  }
  if (((srcSpriteP->moveBoundsRect).top <= (srcSpriteP->destFrameRect).top) ||
     (0.0 <= srcSpriteP->drawData->vertMoveDelta)) {
    if (((srcSpriteP->destFrameRect).bottom <= (srcSpriteP->moveBoundsRect).bottom) ||
       (srcSpriteP->drawData->vertMoveDelta <= 0.0)) {
      sStack_e = 0;
    }
    else {
      srcSpriteP->drawData->vertMoveDelta = -srcSpriteP->drawData->vertMoveDelta;
      sStack_e = ((srcSpriteP->moveBoundsRect).bottom - (srcSpriteP->destFrameRect).bottom) * 2;
    }
  }
  else {
    srcSpriteP->drawData->vertMoveDelta = -srcSpriteP->drawData->vertMoveDelta;
    sStack_e = ((srcSpriteP->moveBoundsRect).top - (srcSpriteP->destFrameRect).top) * 2;
  }
  if ((sStack_10 == 0) && (sStack_e == 0)) {
    SVar1 = '\0';
  }
  else {
    _SWOffsetSprite(srcSpriteP,(float)(int)sStack_10,(float)(int)sStack_e);
    SVar1 = '\x01';
  }
  return SVar1;
}


// ---- _SWWrapSprite @ 0x213c1 (67 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWBoolean _SWWrapSprite(SpritePtr srcSpriteP)

{
  char cVar1;
  
  cVar1 = _SWWrapSpriteHoriz(srcSpriteP);
  if ((cVar1 == '\0') && (cVar1 = _SWWrapSpriteVert(srcSpriteP), cVar1 == '\0')) {
    return '\0';
  }
  return '\x01';
}


// ---- _SWWrapSpriteHoriz @ 0x21404 (407 bytes)

undefined4 _SWWrapSpriteHoriz(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  undefined4 local_20;
  float local_10;
  
  if (param_1 == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22202_001c44d4,0x6f6);
  }
  if (((param_1->moveBoundsRect).right < (param_1->oldFrameRect).left) &&
     ((param_1->oldFrameRect).left < (param_1->destFrameRect).left)) {
    local_10 = (float)(((int)(param_1->destFrameRect).left - (int)(param_1->destFrameRect).right) -
                      ((int)(param_1->moveBoundsRect).right - (int)(param_1->moveBoundsRect).left));
  }
  else if (((param_1->oldFrameRect).right < (param_1->moveBoundsRect).left) &&
          ((param_1->destFrameRect).left < (param_1->oldFrameRect).left)) {
    local_10 = (float)(((int)(param_1->moveBoundsRect).right - (int)(param_1->moveBoundsRect).left)
                      + ((int)(param_1->destFrameRect).right - (int)(param_1->destFrameRect).left));
  }
  else {
    local_10 = 0.0;
  }
  if ((local_10 != 0.0) || (NAN(local_10))) {
    _SWOffsetSprite(param_1,local_10,0.0);
    sVar2 = (param_1->destFrameRect).bottom;
    sVar3 = (param_1->destFrameRect).right;
    sVar1 = (param_1->destFrameRect).left;
    (param_1->deltaFrameRect).top = (param_1->destFrameRect).top;
    (param_1->deltaFrameRect).left = sVar1;
    (param_1->deltaFrameRect).bottom = sVar2;
    (param_1->deltaFrameRect).right = sVar3;
    sVar2 = (param_1->destFrameRect).bottom;
    sVar3 = (param_1->destFrameRect).right;
    sVar1 = (param_1->destFrameRect).left;
    (param_1->oldFrameRect).top = (param_1->destFrameRect).top;
    (param_1->oldFrameRect).left = sVar1;
    (param_1->oldFrameRect).bottom = sVar2;
    (param_1->oldFrameRect).right = sVar3;
    local_20 = 1;
  }
  else {
    local_20 = 0;
  }
  return local_20;
}


// ---- _SWWrapSpriteVert @ 0x2159b (407 bytes)

undefined4 _SWWrapSpriteVert(SpritePtr param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  undefined4 local_20;
  float local_10;
  
  if (param_1 == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22254_001c44d0,0x720);
  }
  if (((param_1->moveBoundsRect).bottom < (param_1->oldFrameRect).top) &&
     ((param_1->oldFrameRect).top < (param_1->destFrameRect).top)) {
    local_10 = (float)(((int)(param_1->destFrameRect).top - (int)(param_1->destFrameRect).bottom) -
                      ((int)(param_1->moveBoundsRect).bottom - (int)(param_1->moveBoundsRect).top));
  }
  else if (((param_1->oldFrameRect).bottom < (param_1->moveBoundsRect).top) &&
          ((param_1->destFrameRect).top < (param_1->oldFrameRect).top)) {
    local_10 = (float)(((int)(param_1->moveBoundsRect).bottom - (int)(param_1->moveBoundsRect).top)
                      + ((int)(param_1->destFrameRect).bottom - (int)(param_1->destFrameRect).top));
  }
  else {
    local_10 = 0.0;
  }
  if ((local_10 != 0.0) || (NAN(local_10))) {
    _SWOffsetSprite(param_1,0.0,local_10);
    sVar2 = (param_1->destFrameRect).bottom;
    sVar3 = (param_1->destFrameRect).right;
    sVar1 = (param_1->destFrameRect).left;
    (param_1->deltaFrameRect).top = (param_1->destFrameRect).top;
    (param_1->deltaFrameRect).left = sVar1;
    (param_1->deltaFrameRect).bottom = sVar2;
    (param_1->deltaFrameRect).right = sVar3;
    sVar2 = (param_1->destFrameRect).bottom;
    sVar3 = (param_1->destFrameRect).right;
    sVar1 = (param_1->destFrameRect).left;
    (param_1->oldFrameRect).top = (param_1->destFrameRect).top;
    (param_1->oldFrameRect).left = sVar1;
    (param_1->oldFrameRect).bottom = sVar2;
    (param_1->oldFrameRect).right = sVar3;
    local_20 = 1;
  }
  else {
    local_20 = 0;
  }
  return local_20;
}


// ---- _SWStdSpriteMoveProc @ 0x21732 (57 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWStdSpriteMoveProc(SpriteRec *spriteP)

{
  _SWOffsetSprite(spriteP,spriteP->drawData->horizMoveDelta,spriteP->drawData->vertMoveDelta);
  return;
}


// ---- _SWWrapSpriteMoveProc @ 0x2176b (68 bytes)

void SWWrapSpriteMoveProc(SpritePtr param_1)

{
  _SWOffsetSprite(param_1,param_1->drawData->horizMoveDelta,param_1->drawData->vertMoveDelta);
  _SWWrapSprite(param_1);
  return;
}


// ---- _SWWrapSpriteHorizMoveProc @ 0x217af (68 bytes)

void SWWrapSpriteHorizMoveProc(SpritePtr param_1)

{
  _SWOffsetSprite(param_1,param_1->drawData->horizMoveDelta,param_1->drawData->vertMoveDelta);
  _SWWrapSpriteHoriz(param_1);
  return;
}


// ---- _SWWrapSpriteVertMoveProc @ 0x217f3 (68 bytes)

void SWWrapSpriteVertMoveProc(SpritePtr param_1)

{
  _SWOffsetSprite(param_1,param_1->drawData->horizMoveDelta,param_1->drawData->vertMoveDelta);
  _SWWrapSpriteVert(param_1);
  return;
}


// ---- _SWInPixelMask @ 0x21837 (191 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWBoolean SWInPixelMask(SpritePtr spriteP,int testPointH,int testPointV)

{
  SDL_bool SVar1;
  
  if (spriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22335_001c4548,0x779);
  }
  if (spriteP->curFrameP == (FrameRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22335_001c4548,0x77a);
  }
  SVar1 = _BKTransparentPixel(spriteP->curFrameP->frameSurfaceP,
                              testPointH - (spriteP->destFrameRect).left,
                              testPointV - (spriteP->destFrameRect).top);
  return SVar1 == 0;
}


// ---- _SWRadiusCollision @ 0x218f6 (303 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWBoolean SWRadiusCollision(SpritePtr srcSpriteP,SpritePtr dstSpriteP)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  
  if ((srcSpriteP == (SpritePtr)0x0) || (dstSpriteP == (SpritePtr)0x0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22364_001c4530,0x793);
  }
  sVar1 = (short)(((int)(srcSpriteP->destFrameRect).right - (int)(srcSpriteP->destFrameRect).left) /
                 2);
  sVar4 = (short)(((int)(dstSpriteP->destFrameRect).right - (int)(dstSpriteP->destFrameRect).left) /
                 2);
  sVar3 = ((srcSpriteP->destFrameRect).left + sVar1) - ((dstSpriteP->destFrameRect).left + sVar4);
  sVar2 = ((srcSpriteP->destFrameRect).top + sVar1) - ((dstSpriteP->destFrameRect).top + sVar4);
  return (int)sVar3 * (int)sVar3 + (int)sVar2 * (int)sVar2 <
         ((int)sVar1 + (int)sVar4) * ((int)sVar1 + (int)sVar4);
}


// ---- _SWPixelCollision @ 0x21a25 (593 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWBoolean _SWPixelCollision(SpritePtr srcSpriteP,SpritePtr dstSpriteP)

{
  float fVar1;
  float fVar2;
  float fVar3;
  uchar uVar4;
  uchar uVar5;
  uchar uVar6;
  uchar uVar7;
  SWBoolean SVar8;
  longdouble extraout_ST0;
  longdouble extraout_ST0_00;
  longdouble extraout_ST0_01;
  longdouble extraout_ST0_02;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22417_001c4574,0x7af);
  }
  if (dstSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22417_001c4574,0x7b0);
  }
  if (srcSpriteP->curFrameP == (FrameRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22417_001c4574,0x7b1);
  }
  if (dstSpriteP->curFrameP == (FrameRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22417_001c4574,0x7b2);
  }
  uVar4 = dstSpriteP->drawData->flippedVertical;
  uVar5 = srcSpriteP->drawData->flippedVertical;
  uVar6 = dstSpriteP->drawData->flippedHorizontal;
  uVar7 = srcSpriteP->drawData->flippedHorizontal;
  _SWGetSpriteHeight(dstSpriteP);
  fVar1 = dstSpriteP->unscaledHeight;
  _SWGetSpriteWidth(dstSpriteP);
  fVar2 = dstSpriteP->unscaledWidth;
  _SWGetSpriteHeight(srcSpriteP);
  fVar3 = srcSpriteP->unscaledHeight;
  _SWGetSpriteWidth(srcSpriteP);
  SVar8 = _BKCollidePixels(srcSpriteP->curFrameP->frameSurfaceP,&srcSpriteP->destFrameRect,
                           dstSpriteP->curFrameP->frameSurfaceP,&dstSpriteP->destFrameRect,
                           &srcSpriteP->curFrameP->frameRect,&dstSpriteP->curFrameP->frameRect,
                           (float)extraout_ST0_02 / srcSpriteP->unscaledWidth,
                           (float)extraout_ST0_01 / fVar3,(float)extraout_ST0_00 / fVar2,
                           (float)extraout_ST0 / fVar1,uVar7,uVar6,uVar5,uVar4);
  return SVar8;
}


// ---- _SWIsSpriteInSprite @ 0x21c76 (364 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWBoolean SWIsSpriteInSprite(SpritePtr srcSpriteP,SpritePtr dstSpriteP)

{
  SWBoolean SStack_10;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22474_001c458c,2000);
  }
  if (dstSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22474_001c458c,0x7d1);
  }
  if (((((int)(srcSpriteP->destFrameRect).top + (int)(srcSpriteP->curFrameP->collisionInset).top <
         (int)(dstSpriteP->destFrameRect).bottom -
         (int)(dstSpriteP->curFrameP->collisionInset).bottom) &&
       ((int)(dstSpriteP->destFrameRect).top + (int)(dstSpriteP->curFrameP->collisionInset).top <
        (int)(srcSpriteP->destFrameRect).bottom -
        (int)(srcSpriteP->curFrameP->collisionInset).bottom)) &&
      ((int)(srcSpriteP->destFrameRect).left + (int)(srcSpriteP->curFrameP->collisionInset).left <
       (int)(dstSpriteP->destFrameRect).right - (int)(dstSpriteP->curFrameP->collisionInset).right))
     && ((int)(dstSpriteP->destFrameRect).left + (int)(dstSpriteP->curFrameP->collisionInset).left <
         (int)(srcSpriteP->destFrameRect).right - (int)(srcSpriteP->curFrameP->collisionInset).right
        )) {
    SStack_10 = '\x01';
  }
  else {
    SStack_10 = '\0';
  }
  return SStack_10;
}


// ---- _SWIsSpriteInRect @ 0x21de2 (234 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWBoolean _SWIsSpriteInRect(SpritePtr srcSpriteP,SWRect *testRect)

{
  SWBoolean local_10;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22543_001c4568,0x7e2);
  }
  if (((((int)(srcSpriteP->destFrameRect).top + (int)(srcSpriteP->curFrameP->collisionInset).top <
         (int)testRect->bottom) &&
       ((int)testRect->top <
        (int)(srcSpriteP->destFrameRect).bottom -
        (int)(srcSpriteP->curFrameP->collisionInset).bottom)) &&
      ((int)(srcSpriteP->destFrameRect).left + (int)(srcSpriteP->curFrameP->collisionInset).left <
       (int)testRect->right)) &&
     ((int)testRect->left <
      (int)(srcSpriteP->destFrameRect).right - (int)(srcSpriteP->curFrameP->collisionInset).right))
  {
    local_10 = '\x01';
  }
  else {
    local_10 = '\0';
  }
  return local_10;
}


// ---- _SWIsSpriteFullyInRect @ 0x21ecc (234 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWBoolean _SWIsSpriteFullyInRect(SpritePtr srcSpriteP,SWRect *testRect)

{
  SWBoolean local_10;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22593_001c4580,0x7f3);
  }
  if (((((int)(srcSpriteP->destFrameRect).top + (int)(srcSpriteP->curFrameP->collisionInset).top <
         (int)testRect->top) ||
       ((int)testRect->bottom <
        (int)(srcSpriteP->destFrameRect).bottom -
        (int)(srcSpriteP->curFrameP->collisionInset).bottom)) ||
      ((int)(srcSpriteP->destFrameRect).left + (int)(srcSpriteP->curFrameP->collisionInset).left <
       (int)testRect->left)) ||
     ((int)testRect->right <
      (int)(srcSpriteP->destFrameRect).right - (int)(srcSpriteP->curFrameP->collisionInset).right))
  {
    local_10 = '\0';
  }
  else {
    local_10 = '\x01';
  }
  return local_10;
}


// ---- _SWIsPointInSprite @ 0x21fb6 (243 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWBoolean _SWIsPointInSprite(SpritePtr srcSpriteP,SWPoint testPoint)

{
  SWBoolean local_10;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22643_001c450c,0x805);
  }
  if (((((int)testPoint.h <
         (int)(srcSpriteP->destFrameRect).left + (int)(srcSpriteP->curFrameP->collisionInset).left)
       || ((int)(srcSpriteP->destFrameRect).right -
           (int)(srcSpriteP->curFrameP->collisionInset).right <= (int)testPoint.h)) ||
      ((int)testPoint.v <
       (int)(srcSpriteP->destFrameRect).top + (int)(srcSpriteP->curFrameP->collisionInset).top)) ||
     ((int)(srcSpriteP->destFrameRect).bottom - (int)(srcSpriteP->curFrameP->collisionInset).bottom
      <= (int)testPoint.v)) {
    local_10 = '\0';
  }
  else {
    local_10 = '\x01';
  }
  return local_10;
}


// ---- _SWIsPointInSpriteMask @ 0x220a9 (152 bytes)

SWBoolean _SWIsPointInSpriteMask
                    (SpritePtr param_1,undefined4 param_2,undefined4 param_3,code *param_4)

{
  SWPoint testPoint;
  SWBoolean SVar1;
  
  if (param_1 == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Sprite.c",
               PTR____FUNCTION___22695_001c456c,0x818);
  }
  if (param_4 == (code *)0x0) {
    testPoint.h = (short)param_2;
    testPoint.v = (short)param_3;
    SVar1 = _SWIsPointInSprite(param_1,testPoint);
  }
  else {
    SVar1 = (*param_4)(param_1,param_2,param_3);
  }
  return SVar1;
}


