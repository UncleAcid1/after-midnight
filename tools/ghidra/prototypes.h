// Hand-written prototypes for functions not in the public SpriteWorldX headers:
// fork-only SpriteWorldX functions and the game's own functions.
// '!' forces the prototype even when Ghidra's parameter count differs.

// SpriteWorldX fork additions
!void SWMountSprite(SpriteRec * baseSpriteP, SpriteRec * riderSpriteP, float offsetH, float offsetV, uchar flag1, uchar flag2, uchar flag3);
!void SWSetMountInheritsAttributes(SpriteRec * spriteP, uchar translucency, uchar attr2, uchar attr3, uchar flip, uchar attr5, uchar visible);
!void SWSetSpriteMountOffset(SpriteRec * spriteP, float offsetH, float offsetV);
!void SWSetGlobalMountOffset(SpriteWorldRec * spriteWorldP, float offsetH, float offsetV);
!void SWSetDrawTilesOverSpriteInsetRect(SpriteRec * spriteP, short left, short top, short right, short bottom);
!void SWSetSpriteAutoScaling(SpriteRec * spriteP, uchar autoScale);
!void SWSetSpriteNonScrolling(SpriteRec * spriteP, uchar nonScrolling);
!void SWSetSpriteScalingMode(SpriteRec * spriteP, uchar mode);
!void SWSetSpriteFlipX(SpriteRec * spriteP, uchar flip);
!void SWSetSpriteFlipY(SpriteRec * spriteP, uchar flip);
!void SWSetSpriteTemporaryClip(SpriteRec * spriteP, short left, short top, short right, short bottom);
!void SWSetSpriteClip(SpriteRec * spriteP, int left, int top, int right, int bottom);
!void SWDrawSpriteWorldWithoutAnimating(SpriteWorldRec * spriteWorldP);
!void SWSetSpriteWorldZoom(SpriteWorldRec * spriteWorldP, float zoomH, float zoomV);
!void SWSetSpriteWorldAutoScaling(SpriteWorldRec * spriteWorldP, uchar autoScale);
!void SWSetSpriteWorldStretchToFit(SpriteWorldRec * spriteWorldP, uchar stretch);
!void SWGetWindowScalingFactor(SpriteWorldRec * spriteWorldP, float * scaleH, float * scaleV);
!void SWChangeWindowSize(SpriteWorldRec * spriteWorldP, short width, short height);
!void SWUpdateWindowSize(SpriteWorldRec * spriteWorldP);
!void SWResetGLSetup(SpriteWorldRec * spriteWorldP);
!void SWSetPostEraseCallBack(SpriteWorldRec * spriteWorldP, CallBackPtr_t callBack);
!void SWSetPostDrawCallBack(SpriteWorldRec * spriteWorldP, CallBackPtr_t callBack);
!uchar SWHasMoveProcFired(SpriteRec * spriteP);
!uchar SWHasPseudoFrameFired(SpriteWorldRec * spriteWorldP);
!int SWGetElapsedMilliseconds(SpriteWorldRec * spriteWorldP);
!void SWResetElapsedMilliseconds(SpriteWorldRec * spriteWorldP);
!void SWTempSpriteMoveProc(SpriteRec * spriteP);
!void SWStdSpriteMoveProc(SpriteRec * spriteP);
!void SWSetSpriteLayerVisible(SpriteLayerRec * layerP, uchar visible);
!uchar SWGetSpriteLayerVisible(SpriteLayerRec * layerP);
!int SWCountNumSpritesInLayer(SpriteLayerRec * layerP);
!void SWStdSpriteDrawProc(SpriteRec * srcSpriteP, FrameRec * curFrameP, float horizLoc, float vertLoc);
!void SWSolidColorSpriteDrawProc(SpriteRec * srcSpriteP, FrameRec * curFrameP, float horizLoc, float vertLoc);
!short SWDrawTileLayerInRect(SpriteWorldRec * spriteWorldP, short tileLayer, SWRect * updateRectP, uchar useChangedMap, uchar forceDraw);
!int BKBlitClippedGLSurface(void * glSurfaceP, short left, short top, short right, short bottom);
!int BKBlitCustomGLSurface(void * glSurfaceP, short tlH, short tlV, short trH, short trV, short blH, short blV, short brH, short brV);
