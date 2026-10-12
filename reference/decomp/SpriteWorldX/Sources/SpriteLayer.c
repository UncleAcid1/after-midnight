// Ghidra decompilation of functions from original source file:
//   ../SpriteWorldX/Sources/SpriteLayer.c

// ---- _SWCreateSpriteLayer @ 0x24420 (163 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWCreateSpriteLayer(SpriteLayerPtr *spriteLayerP)

{
  SpriteLayerPtr pSVar1;
  SWError local_14;
  
  local_14 = 0;
  *spriteLayerP = (SpriteLayerPtr)0x0;
  pSVar1 = (SpriteLayerPtr)_calloc(1,0x28);
  if (pSVar1 == (SpriteLayerPtr)0x0) {
    local_14 = 200;
  }
  else {
    pSVar1->parentSpriteWorldP = (SpriteWorldRec *)0x0;
    pSVar1->tileLayerBelow = -1;
    pSVar1->tileLayerAbove = -1;
    pSVar1->isPaused = 0;
    pSVar1->layerIsVisible = '\x01';
    pSVar1->layerIsNonScrolling = 0;
    pSVar1->headSpriteP = (SpriteRec *)0x0;
    pSVar1->sanityValue = 0x4ca7;
    *spriteLayerP = pSVar1;
  }
  _SWSetStickyIfError(local_14);
  return local_14;
}


// ---- _SWDisposeSpriteLayer @ 0x244c3 (48 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWDisposeSpriteLayer(SpriteLayerPtr *spriteLayerPP)

{
  if (*spriteLayerPP != (SpriteLayerPtr)0x0) {
    _free(*spriteLayerPP);
    *spriteLayerPP = (SpriteLayerPtr)0x0;
  }
  return;
}


// ---- _SWAddSprite @ 0x244f3 (294 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWAddSprite(SpriteLayerPtr spriteLayerP,SpritePtr newSpriteP)

{
  SpriteRec *pSVar1;
  int local_10;
  
  pSVar1 = spriteLayerP->tailSpriteP;
  local_10 = 0;
  if (spriteLayerP == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___20843_001c4620,0x68);
  }
  if (spriteLayerP->sanityValue != 0x4ca7) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___20843_001c4620,0x69);
  }
  if (newSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___20843_001c4620,0x6a);
  }
  if (newSpriteP->parentSpriteLayerP != (SpriteLayerRec *)0x0) {
    local_10 = 0x73;
  }
  if (local_10 == 0) {
    if (pSVar1 == (SpriteRec *)0x0) {
      spriteLayerP->headSpriteP = newSpriteP;
    }
    else {
      pSVar1->nextSpriteP = newSpriteP;
    }
    newSpriteP->prevSpriteP = pSVar1;
    newSpriteP->nextSpriteP = (SpriteRec *)0x0;
    spriteLayerP->tailSpriteP = newSpriteP;
    newSpriteP->needsToBeDrawn = '\x01';
    newSpriteP->parentSpriteLayerP = spriteLayerP;
  }
  _SWSetStickyIfError(local_10);
  return local_10;
}


// ---- _SWRemoveSprite @ 0x24619 (213 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWRemoveSprite(SpritePtr oldSpriteP)

{
  SpriteLayerRec *pSVar1;
  int local_10;
  
  local_10 = 0;
  if (oldSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___20869_001c4638,0x96);
  }
  pSVar1 = oldSpriteP->parentSpriteLayerP;
  if (pSVar1 == (SpriteLayerRec *)0x0) {
    local_10 = 0x72;
  }
  if (local_10 == 0) {
    if (oldSpriteP->nextSpriteP == (SpriteRec *)0x0) {
      pSVar1->tailSpriteP = oldSpriteP->prevSpriteP;
    }
    else {
      oldSpriteP->nextSpriteP->prevSpriteP = oldSpriteP->prevSpriteP;
    }
    if (oldSpriteP->prevSpriteP == (SpriteRec *)0x0) {
      pSVar1->headSpriteP = oldSpriteP->nextSpriteP;
    }
    else {
      oldSpriteP->prevSpriteP->nextSpriteP = oldSpriteP->nextSpriteP;
    }
    oldSpriteP->parentSpriteLayerP = (SpriteLayerRec *)0x0;
  }
  _SWSetStickyIfError(local_10);
  return local_10;
}


// ---- _SWRemoveAllSpritesFromLayer @ 0x246ee (115 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWRemoveAllSpritesFromLayer(SpriteLayerPtr srcSpriteLayerP)

{
  SpritePtr oldSpriteP;
  
  if (srcSpriteLayerP == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___20899_001c4654,199);
  }
  while( true ) {
    oldSpriteP = _SWGetNextSprite(srcSpriteLayerP,(SpritePtr)0x0);
    if (oldSpriteP == (SpritePtr)0x0) break;
    _SWRemoveSprite(oldSpriteP);
  }
  return;
}


// ---- _SWDisposeAllSpritesInLayer @ 0x24761 (127 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWDisposeAllSpritesInLayer(SpriteLayerPtr spriteLayerP)

{
  SpriteRec *local_14;
  SpriteRec *local_10;
  
  if (spriteLayerP == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___20912_001c461c,0xd9);
  }
  local_14 = spriteLayerP->headSpriteP;
  while (local_14 != (SpritePtr)0x0) {
    local_10 = local_14->nextSpriteP;
    _SWRemoveSprite(local_14);
    _SWDisposeSprite(&local_14);
    local_14 = local_10;
  }
  return;
}


// ---- _SWCountNumSpritesInLayer @ 0x247e0 (113 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int SWCountNumSpritesInLayer(SpriteLayerRec *layerP)

{
  SpriteRec *pSStack_14;
  int iStack_10;
  
  iStack_10 = 0;
  if (layerP == (SpriteLayerRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___20928_001c4630,0xf4);
  }
  for (pSStack_14 = layerP->headSpriteP; pSStack_14 != (SpriteRec *)0x0;
      pSStack_14 = pSStack_14->nextSpriteP) {
    iStack_10 = iStack_10 + 1;
  }
  return iStack_10;
}


// ---- _SWSwapSprite @ 0x24851 (615 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError SWSwapSprite(SpritePtr srcSpriteP,SpritePtr dstSpriteP)

{
  SpriteLayerRec *pSVar1;
  SpriteRec *pSVar2;
  SWError SStack_24;
  
  if ((srcSpriteP == (SpritePtr)0x0) || (dstSpriteP == (SpritePtr)0x0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___20944_001c464c,0x113);
  }
  if (srcSpriteP == dstSpriteP) {
    SStack_24 = 0;
  }
  else {
    pSVar1 = srcSpriteP->parentSpriteLayerP;
    if ((dstSpriteP->parentSpriteLayerP == pSVar1) && (pSVar1 != (SpriteLayerRec *)0x0)) {
      if ((srcSpriteP->nextSpriteP == dstSpriteP) || (dstSpriteP->nextSpriteP == srcSpriteP)) {
        if (srcSpriteP->nextSpriteP == dstSpriteP) {
          if (srcSpriteP->prevSpriteP != (SpriteRec *)0x0) {
            srcSpriteP->prevSpriteP->nextSpriteP = dstSpriteP;
          }
          if (dstSpriteP->nextSpriteP != (SpriteRec *)0x0) {
            dstSpriteP->nextSpriteP->prevSpriteP = srcSpriteP;
          }
          dstSpriteP->prevSpriteP = srcSpriteP->prevSpriteP;
          srcSpriteP->nextSpriteP = dstSpriteP->nextSpriteP;
          dstSpriteP->nextSpriteP = srcSpriteP;
          srcSpriteP->prevSpriteP = dstSpriteP;
        }
        else {
          if (dstSpriteP->prevSpriteP != (SpriteRec *)0x0) {
            dstSpriteP->prevSpriteP->nextSpriteP = srcSpriteP;
          }
          if (srcSpriteP->nextSpriteP != (SpriteRec *)0x0) {
            srcSpriteP->nextSpriteP->prevSpriteP = dstSpriteP;
          }
          srcSpriteP->prevSpriteP = dstSpriteP->prevSpriteP;
          dstSpriteP->nextSpriteP = srcSpriteP->nextSpriteP;
          srcSpriteP->nextSpriteP = dstSpriteP;
          dstSpriteP->prevSpriteP = srcSpriteP;
        }
      }
      else {
        if (srcSpriteP->prevSpriteP != (SpriteRec *)0x0) {
          srcSpriteP->prevSpriteP->nextSpriteP = dstSpriteP;
        }
        if (dstSpriteP->prevSpriteP != (SpriteRec *)0x0) {
          dstSpriteP->prevSpriteP->nextSpriteP = srcSpriteP;
        }
        if (srcSpriteP->nextSpriteP != (SpriteRec *)0x0) {
          srcSpriteP->nextSpriteP->prevSpriteP = dstSpriteP;
        }
        if (dstSpriteP->nextSpriteP != (SpriteRec *)0x0) {
          dstSpriteP->nextSpriteP->prevSpriteP = srcSpriteP;
        }
        pSVar2 = srcSpriteP->nextSpriteP;
        srcSpriteP->nextSpriteP = dstSpriteP->nextSpriteP;
        dstSpriteP->nextSpriteP = pSVar2;
        pSVar2 = srcSpriteP->prevSpriteP;
        srcSpriteP->prevSpriteP = dstSpriteP->prevSpriteP;
        dstSpriteP->prevSpriteP = pSVar2;
      }
      if (srcSpriteP->nextSpriteP == (SpriteRec *)0x0) {
        pSVar1->tailSpriteP = srcSpriteP;
      }
      else if (srcSpriteP->prevSpriteP == (SpriteRec *)0x0) {
        pSVar1->headSpriteP = srcSpriteP;
      }
      if (dstSpriteP->nextSpriteP == (SpriteRec *)0x0) {
        pSVar1->tailSpriteP = dstSpriteP;
      }
      else if (dstSpriteP->prevSpriteP == (SpriteRec *)0x0) {
        pSVar1->headSpriteP = dstSpriteP;
      }
      SStack_24 = 0;
    }
    else {
      _SWSetStickyIfError(0x72);
      SStack_24 = 0x72;
    }
  }
  return SStack_24;
}


// ---- _SWInsertSpriteAfterSprite @ 0x24ab8 (215 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWInsertSpriteAfterSprite(SpritePtr newSpriteP,SpritePtr dstSpriteP)

{
  SpriteLayerRec *pSVar1;
  int local_10;
  
  local_10 = 0;
  if ((newSpriteP == (SpritePtr)0x0) || (dstSpriteP == (SpritePtr)0x0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21025_001c460c,0x176);
  }
  pSVar1 = dstSpriteP->parentSpriteLayerP;
  if (newSpriteP->parentSpriteLayerP != (SpriteLayerRec *)0x0) {
    local_10 = 0x73;
  }
  if (local_10 == 0) {
    if (dstSpriteP->nextSpriteP != (SpriteRec *)0x0) {
      dstSpriteP->nextSpriteP->prevSpriteP = newSpriteP;
    }
    newSpriteP->nextSpriteP = dstSpriteP->nextSpriteP;
    dstSpriteP->nextSpriteP = newSpriteP;
    newSpriteP->prevSpriteP = dstSpriteP;
    if (newSpriteP->nextSpriteP == (SpriteRec *)0x0) {
      pSVar1->tailSpriteP = newSpriteP;
    }
    newSpriteP->needsToBeDrawn = '\x01';
    newSpriteP->parentSpriteLayerP = pSVar1;
  }
  _SWSetStickyIfError(local_10);
  return local_10;
}


// ---- _SWInsertSpriteBeforeSprite @ 0x24b8f (215 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWInsertSpriteBeforeSprite(SpritePtr newSpriteP,SpritePtr dstSpriteP)

{
  SpriteLayerRec *pSVar1;
  int local_10;
  
  local_10 = 0;
  if ((newSpriteP == (SpritePtr)0x0) || (dstSpriteP == (SpritePtr)0x0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21051_001c4650,0x1a0);
  }
  pSVar1 = dstSpriteP->parentSpriteLayerP;
  if (newSpriteP->parentSpriteLayerP != (SpriteLayerRec *)0x0) {
    local_10 = 0x73;
  }
  if (local_10 == 0) {
    if (dstSpriteP->prevSpriteP != (SpriteRec *)0x0) {
      dstSpriteP->prevSpriteP->nextSpriteP = newSpriteP;
    }
    newSpriteP->prevSpriteP = dstSpriteP->prevSpriteP;
    dstSpriteP->prevSpriteP = newSpriteP;
    newSpriteP->nextSpriteP = dstSpriteP;
    if (newSpriteP->prevSpriteP == (SpriteRec *)0x0) {
      pSVar1->headSpriteP = newSpriteP;
    }
    newSpriteP->needsToBeDrawn = '\x01';
    newSpriteP->parentSpriteLayerP = pSVar1;
  }
  _SWSetStickyIfError(local_10);
  return local_10;
}


// ---- _SWGetNextSprite @ 0x24c66 (101 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SpritePtr _SWGetNextSprite(SpriteLayerPtr spriteLayerP,SpritePtr curSpriteP)

{
  SpritePtr local_10;
  
  if (spriteLayerP == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21075_001c4610,0x1c7);
  }
  if (curSpriteP == (SpritePtr)0x0) {
    local_10 = spriteLayerP->headSpriteP;
  }
  else {
    local_10 = curSpriteP->nextSpriteP;
  }
  return local_10;
}


// ---- _SWGetPreviousSprite @ 0x24ccb (101 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SpritePtr _SWGetPreviousSprite(SpriteLayerPtr spriteLayerP,SpritePtr curSpriteP)

{
  SpritePtr local_10;
  
  if (spriteLayerP == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21089_001c4608,0x1d5);
  }
  if (curSpriteP == (SpritePtr)0x0) {
    local_10 = spriteLayerP->tailSpriteP;
  }
  else {
    local_10 = curSpriteP->prevSpriteP;
  }
  return local_10;
}


// ---- _SWLockSpriteLayer @ 0x24d30 (109 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWLockSpriteLayer(SpriteLayerPtr spriteLayerP)

{
  SpritePtr pSStack_10;
  
  if (spriteLayerP == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21103_001c4658,0x1e7);
  }
  for (pSStack_10 = spriteLayerP->headSpriteP; pSStack_10 != (SpritePtr)0x0;
      pSStack_10 = pSStack_10->nextSpriteP) {
    _SWLockSprite(pSStack_10);
  }
  return;
}


// ---- _SWUnlockSpriteLayer @ 0x24d9d (109 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWUnlockSpriteLayer(SpriteLayerPtr spriteLayerP)

{
  SpritePtr pSStack_10;
  
  if (spriteLayerP == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21114_001c4628,0x1fd);
  }
  for (pSStack_10 = spriteLayerP->headSpriteP; pSStack_10 != (SpritePtr)0x0;
      pSStack_10 = pSStack_10->nextSpriteP) {
    _SWUnlockSprite(pSStack_10);
  }
  return;
}


// ---- _SWCollideSpriteLayer @ 0x24e0a (1056 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWCollideSpriteLayer
               (SpriteWorldPtr spriteWorldP,SpriteLayerPtr srcSpriteLayerP,
               SpriteLayerPtr dstSpriteLayerP)

{
  int iVar1;
  int local_50;
  int local_48;
  int local_40;
  int local_38;
  int local_30;
  SWRect local_28;
  SpriteRec *local_20;
  SpriteRec *local_1c;
  SpriteRec *local_18;
  SpriteRec *local_14;
  int local_10;
  
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21132_001c4648,0x21b);
  }
  if ((srcSpriteLayerP == (SpriteLayerPtr)0x0) || (dstSpriteLayerP == (SpriteLayerPtr)0x0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21132_001c4648,0x21c);
  }
  if (spriteWorldP->frameHasOccurred != '\0') {
    local_20 = srcSpriteLayerP->headSpriteP;
    while (local_20 != (SpriteRec *)0x0) {
      local_18 = dstSpriteLayerP->headSpriteP;
      local_1c = local_20->nextSpriteP;
      while (local_18 != (SpriteRec *)0x0) {
        local_14 = local_18->nextSpriteP;
        if ((((local_20 != local_18) &&
             ((local_20->destFrameRect).top < (local_18->destFrameRect).bottom)) &&
            ((local_18->destFrameRect).top < (local_20->destFrameRect).bottom)) &&
           (((local_20->destFrameRect).left < (local_18->destFrameRect).right &&
            ((local_18->destFrameRect).left < (local_20->destFrameRect).right)))) {
          if ((local_20->curFrameP->usesCollisionInset == '\0') &&
             (local_18->curFrameP->usesCollisionInset == '\0')) {
            local_10 = 1;
          }
          else {
            if ((((int)(local_20->destFrameRect).top +
                  (int)(local_20->curFrameP->collisionInset).top <
                  (int)(local_18->destFrameRect).bottom -
                  (int)(local_18->curFrameP->collisionInset).bottom) &&
                ((int)(local_18->destFrameRect).top + (int)(local_18->curFrameP->collisionInset).top
                 < (int)(local_20->destFrameRect).bottom -
                   (int)(local_20->curFrameP->collisionInset).bottom)) &&
               (((int)(local_20->destFrameRect).left +
                 (int)(local_20->curFrameP->collisionInset).left <
                 (int)(local_18->destFrameRect).right -
                 (int)(local_18->curFrameP->collisionInset).right &&
                ((int)(local_18->destFrameRect).left +
                 (int)(local_18->curFrameP->collisionInset).left <
                 (int)(local_20->destFrameRect).right -
                 (int)(local_20->curFrameP->collisionInset).right)))) {
              local_50 = 1;
            }
            else {
              local_50 = 0;
            }
            local_10 = local_50;
          }
          if ((local_10 != 0) && (local_20->spriteCollideProc != (CollideProcPtr_t)0x0)) {
            local_48 = (int)(local_18->destFrameRect).left +
                       (int)(local_18->curFrameP->collisionInset).left;
            iVar1 = (int)(local_20->destFrameRect).left +
                    (int)(local_20->curFrameP->collisionInset).left;
            if (local_48 < iVar1) {
              local_48 = iVar1;
            }
            local_28.left = (short)local_48;
            local_40 = (int)(local_18->destFrameRect).top +
                       (int)(local_18->curFrameP->collisionInset).top;
            iVar1 = (int)(local_20->destFrameRect).top +
                    (int)(local_20->curFrameP->collisionInset).top;
            if (local_40 < iVar1) {
              local_40 = iVar1;
            }
            local_28.top = (short)local_40;
            local_38 = (int)(local_18->destFrameRect).right -
                       (int)(local_18->curFrameP->collisionInset).right;
            iVar1 = (int)(local_20->destFrameRect).right -
                    (int)(local_20->curFrameP->collisionInset).right;
            if (iVar1 < local_38) {
              local_38 = iVar1;
            }
            local_28.right = (short)local_38;
            local_30 = (int)(local_18->destFrameRect).bottom -
                       (int)(local_18->curFrameP->collisionInset).bottom;
            iVar1 = (int)(local_20->destFrameRect).bottom -
                    (int)(local_20->curFrameP->collisionInset).bottom;
            if (iVar1 < local_30) {
              local_30 = iVar1;
            }
            local_28.bottom = (short)local_30;
            (*local_20->spriteCollideProc)(local_20,local_18,&local_28);
            if ((local_20->spriteRemoval != 0) ||
               (local_20->parentSpriteLayerP == (SpriteLayerRec *)0x0)) break;
          }
        }
        if ((local_14 == (SpriteRec *)0x0) || (local_14->spriteRemoval == 0)) {
          local_18 = local_14;
        }
        else {
          local_18 = dstSpriteLayerP->headSpriteP;
        }
      }
      if ((local_1c == (SpriteRec *)0x0) || (local_1c->spriteRemoval == 0)) {
        local_20 = local_1c;
      }
      else {
        local_20 = srcSpriteLayerP->headSpriteP;
      }
    }
  }
  return;
}


// ---- _SWSpriteCollidesWithLayer @ 0x2522a (283 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int SWSpriteCollidesWithLayer(SpritePtr srcSpriteP,SpriteLayerPtr spriteLayerP,void *collideFunc)

{
  char cVar1;
  SpritePtr pSStack_10;
  
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21312_001c462c,0x278);
  }
  if (spriteLayerP == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21312_001c462c,0x279);
  }
  pSStack_10 = spriteLayerP->headSpriteP;
  while( true ) {
    if (pSStack_10 == (SpritePtr)0x0) {
      return 0;
    }
    if ((((srcSpriteP != pSStack_10) &&
         ((srcSpriteP->destFrameRect).top < (pSStack_10->destFrameRect).bottom)) &&
        ((pSStack_10->destFrameRect).top < (srcSpriteP->destFrameRect).bottom)) &&
       ((((srcSpriteP->destFrameRect).left < (pSStack_10->destFrameRect).right &&
         ((pSStack_10->destFrameRect).left < (srcSpriteP->destFrameRect).right)) &&
        ((collideFunc == (void *)0x0 ||
         (cVar1 = (*(code *)collideFunc)(srcSpriteP,pSStack_10), cVar1 != '\0')))))) break;
    pSStack_10 = pSStack_10->nextSpriteP;
  }
  return 1;
}


// ---- __compareSpriteBottom @ 0x25345 (36 bytes)

int _compareSpriteBottom(int param_1,int param_2)

{
  return (int)(short)(*(short *)(param_1 + 0x3a) - *(short *)(param_2 + 0x3a));
}


// ---- _SWSortSpriteLayer @ 0x25369 (48 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWSortSpriteLayer(SpriteLayerPtr theLayer)

{
  _SWCustomSortSpriteLayer(theLayer,(SpriteCompareProcPtr_t)PTR_LAB_001c4600);
  return;
}


// ---- _SWCustomSortSpriteLayer @ 0x25399 (390 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWCustomSortSpriteLayer(SpriteLayerPtr theLayer,SpriteCompareProcPtr_t compareFunc)

{
  bool bVar1;
  short sVar2;
  SpritePtr local_1c;
  SpritePtr local_18;
  SpritePtr local_14;
  
  if (theLayer == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21366_001c4634,0x2b5);
  }
  if (compareFunc == (SpriteCompareProcPtr_t)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21366_001c4634,0x2b6);
  }
  local_18 = theLayer->headSpriteP;
  local_1c = _SWGetNextSprite(theLayer,local_18);
  while (local_1c != (SpritePtr)0x0) {
    sVar2 = (*compareFunc)(local_18,local_1c);
    if (sVar2 < 1) {
      local_18 = local_1c;
      local_1c = local_1c->nextSpriteP;
    }
    else {
      local_14 = local_1c->prevSpriteP;
      bVar1 = false;
      while ((local_14 != (SpritePtr)0x0 && (!bVar1))) {
        sVar2 = (*compareFunc)(local_14,local_1c);
        if (sVar2 < 1) {
          bVar1 = true;
        }
        else {
          local_14 = local_14->prevSpriteP;
        }
      }
      if (local_14 == (SpritePtr)0x0) {
        _SWRemoveSprite(local_1c);
        _SWInsertSpriteBeforeSprite(local_1c,theLayer->headSpriteP);
        local_1c = _SWGetNextSprite(theLayer,local_18);
      }
      else {
        _SWRemoveSprite(local_1c);
        _SWInsertSpriteAfterSprite(local_1c,local_14);
        local_1c = _SWGetNextSprite(theLayer,local_18);
      }
    }
  }
  return;
}


// ---- _SWPauseSpriteLayer @ 0x2551f (82 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWPauseSpriteLayer(SpriteLayerPtr spriteLayerP)

{
  if (spriteLayerP == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21398_001c4604,0x2e9);
  }
  spriteLayerP->isPaused = 1;
  return;
}


// ---- _SWUnpauseSpriteLayer @ 0x25571 (82 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWUnpauseSpriteLayer(SpriteLayerPtr spriteLayerP)

{
  if (spriteLayerP == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21405_001c465c,0x2f6);
  }
  spriteLayerP->isPaused = 0;
  return;
}


// ---- _SWSetSpriteLayerVisible @ 0x255c3 (88 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetSpriteLayerVisible(SpriteLayerRec *layerP,uchar visible)

{
  if (layerP == (SpriteLayerRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21413_001c463c,0x302);
  }
  layerP->layerIsVisible = visible;
  return;
}


// ---- _SWGetSpriteLayerVisible @ 0x2561b (82 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

uchar _SWGetSpriteLayerVisible(SpriteLayerRec *layerP)

{
  if (layerP == (SpriteLayerRec *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21420_001c4618,0x30e);
  }
  return layerP->layerIsVisible;
}


// ---- _SWSetSpriteLayersVisible @ 0x2566d (117 bytes)

void _SWSetSpriteLayersVisible(int param_1,int param_2,undefined1 param_3)

{
  int local_10;
  
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21433_001c4614,0x31c);
  }
  local_10 = param_1;
  do {
    *(undefined1 *)(local_10 + 0x1c) = param_3;
    if (local_10 == param_2) {
      return;
    }
    local_10 = *(int *)(local_10 + 4);
  } while (local_10 != 0);
  return;
}


// ---- _SWSetAllSpriteLayersVisible @ 0x256e2 (53 bytes)

void _SWSetAllSpriteLayersVisible(int *param_1,undefined1 param_2)

{
  undefined4 local_10;
  
  local_10 = *param_1;
  do {
    *(undefined1 *)(local_10 + 0x1c) = param_2;
    local_10 = *(int *)(local_10 + 4);
  } while (local_10 != 0);
  return;
}


// ---- _SWSetLayerAsNonScrolling @ 0x25717 (81 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWSetLayerAsNonScrolling(SpriteLayerPtr spriteLayerP,int layerIsNonScrolling)

{
  if (spriteLayerP == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21452_001c4640,0x347);
  }
  spriteLayerP->layerIsNonScrolling = layerIsNonScrolling;
  return;
}


// ---- _SWFindSpriteByPoint @ 0x25768 (54 bytes)

void SWFindSpriteByPoint(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4
                        )

{
  _SWTopSpriteFromPoint(param_1,param_2,param_3,param_4,0);
  return;
}


// ---- _SWTopSpriteFromPoint @ 0x2579e (161 bytes)

SpritePtr _SWTopSpriteFromPoint
                    (SpriteLayerPtr param_1,SpritePtr param_2,undefined4 param_3,undefined4 param_4,
                    undefined4 param_5)

{
  char cVar1;
  SpritePtr local_10;
  
  if (param_1 == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21473_001c4624,0x366);
  }
  local_10 = _SWGetPreviousSprite(param_1,param_2);
  while( true ) {
    if (local_10 == (SpritePtr)0x0) {
      return (SpritePtr)0x0;
    }
    cVar1 = _SWIsPointInSpriteMask(local_10,param_3,param_4,param_5);
    if (cVar1 != '\0') break;
    local_10 = _SWGetPreviousSprite(param_1,local_10);
  }
  return local_10;
}


// ---- _SWMoveSpriteLayerToTail @ 0x2583f (203 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWMoveSpriteLayerToTail(SpriteWorldPtr spriteWorldP,SpriteLayerPtr moveSpriteLayerP)

{
  SpriteLayerRec *pSVar1;
  
  pSVar1 = spriteWorldP->tailSpriteLayerP;
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/SpriteLayer.c",
               PTR____FUNCTION___21491_001c4644,0x386);
  }
  if (pSVar1 != moveSpriteLayerP) {
    if (moveSpriteLayerP->nextSpriteLayerP == (SpriteLayerRec *)0x0) {
      spriteWorldP->tailSpriteLayerP = moveSpriteLayerP->prevSpriteLayerP;
    }
    else {
      moveSpriteLayerP->nextSpriteLayerP->prevSpriteLayerP = moveSpriteLayerP->prevSpriteLayerP;
    }
    if (moveSpriteLayerP->prevSpriteLayerP == (SpriteLayerRec *)0x0) {
      spriteWorldP->headSpriteLayerP = moveSpriteLayerP->nextSpriteLayerP;
    }
    else {
      moveSpriteLayerP->prevSpriteLayerP->nextSpriteLayerP = moveSpriteLayerP->nextSpriteLayerP;
    }
    pSVar1->nextSpriteLayerP = moveSpriteLayerP;
    moveSpriteLayerP->prevSpriteLayerP = pSVar1;
    moveSpriteLayerP->nextSpriteLayerP = (SpriteLayerRec *)0x0;
    spriteWorldP->tailSpriteLayerP = moveSpriteLayerP;
  }
  return;
}


