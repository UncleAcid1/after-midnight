// Ghidra decompilation of functions from original source file:
//   ../SpriteWorldX/Sources/Tiling.c

// ---- _SWInitTiling @ 0x81145 (918 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWInitTiling(SpriteWorldPtr spriteWorldP,short tileHeight,short tileWidth,short maxNumTiles
                     )

{
  FrameRec **ppFVar1;
  void *pvVar2;
  short *psVar3;
  TileMapStruct **ppTVar4;
  int local_14;
  short local_10;
  short local_e;
  
  local_14 = 0;
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___20899_001c50c8,0x45);
  }
  if (spriteWorldP->tilingIsInitialized != '\0') {
    local_14 = 0x6d;
  }
  if (local_14 == 0) {
    spriteWorldP->tilingIsOn = '\x01';
    spriteWorldP->tilingIsInitialized = '\x01';
    spriteWorldP->lastActiveTileLayer = 0;
    spriteWorldP->maxNumTiles = maxNumTiles;
    spriteWorldP->tileHeight = tileHeight;
    spriteWorldP->tileWidth = tileWidth;
    spriteWorldP->tileImageWidth = tileWidth;
    spriteWorldP->tileImageHeight = tileHeight;
    spriteWorldP->masterTileSurface = (void *)0x0;
    spriteWorldP->useVBOForTiling = '\0';
    for (local_e = 0; local_e < 0xc; local_e = local_e + 1) {
      *(undefined2 *)(PTR__gEmptyTileVertices_001c50c0 + local_e * 2) = 0x8001;
      *(undefined4 *)(PTR__gEmptyTileTexCoords_001c50d8 + local_e * 4) = 0;
    }
  }
  if (local_14 == 0) {
    ppFVar1 = (FrameRec **)_calloc((int)maxNumTiles,4);
    spriteWorldP->tileFrameArray = ppFVar1;
    if (spriteWorldP->tileFrameArray == (FrameRec **)0x0) {
      local_14 = 200;
    }
  }
  if (local_14 == 0) {
    pvVar2 = _calloc((int)maxNumTiles,10);
    spriteWorldP->tileColorArray = pvVar2;
    if (spriteWorldP->tileColorArray == (void *)0x0) {
      local_14 = 200;
    }
    if (spriteWorldP->tileColorArray != (void *)0x0) {
      for (local_e = 0; local_e < maxNumTiles; local_e = local_e + 1) {
        *(undefined1 *)((int)spriteWorldP->tileColorArray + local_e * 10) = 0;
        *(undefined2 *)((int)spriteWorldP->tileColorArray + local_e * 10 + 2) = 0xffff;
        *(undefined2 *)((int)spriteWorldP->tileColorArray + local_e * 10 + 4) = 0xffff;
        *(undefined2 *)((int)spriteWorldP->tileColorArray + local_e * 10 + 6) = 0xffff;
        *(undefined2 *)((int)spriteWorldP->tileColorArray + local_e * 10 + 8) = 0xffff;
      }
    }
  }
  if (local_14 == 0) {
    psVar3 = (short *)_calloc((int)maxNumTiles,2);
    spriteWorldP->curTileImage = psVar3;
    if (spriteWorldP->curTileImage == (short *)0x0) {
      local_14 = 200;
    }
    if (local_14 == 0) {
      for (local_10 = 0; local_10 < maxNumTiles; local_10 = local_10 + 1) {
        spriteWorldP->curTileImage[local_10] = local_10;
      }
    }
  }
  if (local_14 == 0) {
    psVar3 = (short *)_calloc((int)maxNumTiles,2);
    spriteWorldP->tileSourceIndex = psVar3;
    if (spriteWorldP->tileSourceIndex == (short *)0x0) {
      local_14 = 200;
    }
    if (local_14 == 0) {
      for (local_10 = 0; local_10 < maxNumTiles; local_10 = local_10 + 1) {
        spriteWorldP->tileSourceIndex[local_10] = local_10;
      }
    }
  }
  if (local_14 == 0) {
    ppTVar4 = (TileMapStruct **)_calloc(10,4);
    spriteWorldP->tileLayerArray = ppTVar4;
    if (spriteWorldP->tileLayerArray == (TileMapStruct **)0x0) {
      local_14 = 200;
    }
  }
  return local_14;
}


// ---- _SWExitTiling @ 0x814db (252 bytes)

/* WARNING: Removing unreachable block (ram,0x0008153c) */
/* WARNING: Removing unreachable block (ram,0x0008155f) */
/* WARNING: Removing unreachable block (ram,0x0008154c) */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWExitTiling(SpriteWorldPtr spriteWorldP)

{
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___21010_001c50e4,0xbc);
  }
  if (spriteWorldP->tilingIsInitialized != '\0') {
    _free(spriteWorldP->tileFrameArray);
    _free(spriteWorldP->curTileImage);
    _free(spriteWorldP->tileLayerArray);
    _free(spriteWorldP->changedTiles);
    spriteWorldP->changedTiles = (SWRect *)0x0;
    spriteWorldP->tilingIsInitialized = '\0';
    spriteWorldP->tilingIsOn = '\0';
  }
  return;
}


// ---- _SWSetTileResolution @ 0x815d7 (56 bytes)

void _SWSetTileResolution(int param_1,undefined2 param_2,undefined2 param_3)

{
  *(undefined2 *)(param_1 + 0xe6) = param_2;
  *(undefined2 *)(param_1 + 0xe8) = param_3;
  return;
}


// ---- _SWSetVBOForTiling @ 0x8160f (33 bytes)

void SWSetVBOForTiling(int param_1,undefined1 param_2)

{
  *(undefined1 *)(param_1 + 0x111) = param_2;
  return;
}


// ---- _SWCreateTileMap @ 0x81630 (744 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWCreateTileMap(TileMapStructPtr *tileMapStructPP,short numTileMapRows,short numTileMapCols
                        )

{
  short **ppsVar1;
  uchar **ppuVar2;
  short *psVar3;
  uchar *puVar4;
  TileMapStructPtr local_18;
  int local_14;
  int local_10;
  
  local_10 = 0;
  local_18 = (TileMapStructPtr)_calloc(1,0x34);
  if (local_18 == (TileMapStructPtr)0x0) {
    local_10 = 200;
  }
  if (local_10 == 0) {
    local_18->numRows = numTileMapRows;
    local_18->numCols = numTileMapCols;
    local_18->vbo = 0;
    *(undefined4 *)&local_18->field_0x24 = 0;
    *(undefined4 *)&local_18->field_0x2c = 0;
    local_18->vboMethod = '\x01';
    local_18->zoom = 1.0;
    local_18->isVisible = '\x01';
    local_18->drawEveryFrame = '\0';
    local_18->tilesChanged = '\0';
    ppsVar1 = (short **)_calloc((int)numTileMapRows,4);
    local_18->tileMap = ppsVar1;
    ppsVar1 = (short **)_calloc((int)numTileMapRows,4);
    local_18->changedTileMap = ppsVar1;
    ppuVar2 = (uchar **)_calloc((int)numTileMapRows,4);
    local_18->tileChangedFlags = ppuVar2;
    if (((local_18->tileMap == (short **)0x0) || (local_18->changedTileMap == (short **)0x0)) ||
       (local_18->tileChangedFlags == (uchar **)0x0)) {
      local_10 = 200;
    }
  }
  if (local_10 == 0) {
    for (local_14 = 0; local_14 < numTileMapRows; local_14 = local_14 + 1) {
      psVar3 = (short *)_calloc((int)numTileMapCols,2);
      local_18->tileMap[local_14] = psVar3;
      psVar3 = (short *)_calloc((int)numTileMapCols,2);
      local_18->changedTileMap[local_14] = psVar3;
      puVar4 = (uchar *)_calloc((int)numTileMapCols,1);
      local_18->tileChangedFlags[local_14] = puVar4;
      if (((local_18->tileMap[local_14] == (short *)0x0) ||
          (local_18->changedTileMap[local_14] == (short *)0x0)) ||
         (local_18->tileChangedFlags[local_14] == (uchar *)0x0)) {
        local_10 = 200;
        break;
      }
    }
  }
  if (local_10 == 0) {
    _SWResetTileMap(local_18);
  }
  else if (local_18 != (TileMapStructPtr)0x0) {
    for (local_14 = 0; local_14 < numTileMapRows; local_14 = local_14 + 1) {
      if (local_18->tileMap[local_14] != (short *)0x0) {
        _free(local_18->tileMap[local_14]);
      }
      if (local_18->changedTileMap[local_14] != (short *)0x0) {
        _free(local_18->changedTileMap[local_14]);
      }
    }
    _free(local_18->tileMap);
    _free(local_18->changedTileMap);
    _free(local_18);
    local_18 = (TileMapStructPtr)0x0;
  }
  *tileMapStructPP = local_18;
  return local_10;
}


// ---- _SWCloneTileMap @ 0x81918 (277 bytes)

SWError _SWCloneTileMap(short *param_1,TileMapStructPtr *param_2)

{
  TileMapStructPtr pTVar1;
  SWError SVar2;
  short local_14;
  short local_12;
  
  if ((*param_1 < 1) || (param_1[1] < 1)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___21167_001c5064,0x1da);
  }
  SVar2 = _SWCreateTileMap(param_2,*param_1,param_1[1]);
  if (SVar2 == 0) {
    _SWResetTileMap(*param_2);
    pTVar1 = *param_2;
    for (local_14 = 0; local_14 < pTVar1->numRows; local_14 = local_14 + 1) {
      for (local_12 = 0; local_12 < pTVar1->numCols; local_12 = local_12 + 1) {
        (*param_2)->tileMap[local_14][local_12] =
             *(short *)(*(int *)(*(int *)(param_1 + 2) + local_14 * 4) + local_12 * 2);
      }
    }
  }
  return SVar2;
}


// ---- _SWResetTileMapForLayer @ 0x81a2d (50 bytes)

void _SWResetTileMapForLayer(int param_1,short param_2)

{
  _SWResetTileMap(*(undefined4 *)(*(int *)(param_1 + 0xc4) + param_2 * 4));
  return;
}


// ---- _SWResetTileMap @ 0x81a5f (99 bytes)

void _SWResetTileMap(short *param_1)

{
  short local_10;
  short local_e;
  
  for (local_10 = 0; local_10 < *param_1; local_10 = local_10 + 1) {
    for (local_e = 0; local_e < param_1[1]; local_e = local_e + 1) {
      *(undefined2 *)(*(int *)(*(int *)(param_1 + 8) + local_10 * 4) + local_e * 2) = 0xffff;
    }
  }
  return;
}


// ---- _SWDisposeTileMap @ 0x81ac2 (371 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWDisposeTileMap(TileMapStructPtr *tileMapStructPP)

{
  TileMapStructPtr pTVar1;
  int iVar2;
  undefined4 uVar3;
  int local_18;
  
  pTVar1 = *tileMapStructPP;
  if (pTVar1 != (TileMapStructPtr)0x0) {
    for (local_18 = 0; local_18 < pTVar1->numRows; local_18 = local_18 + 1) {
      if (pTVar1->tileMap[local_18] != (short *)0x0) {
        _free(pTVar1->tileMap[local_18]);
      }
    }
    if (pTVar1->vbo != 0) {
      iVar2 = _glGetError();
      if (iVar2 != 0) {
        uVar3 = _BKGetGLErrorMessage(iVar2);
        _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
                 "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___21256_001c50bc,0x21a,uVar3);
      }
      _glDeleteBuffers(1,&pTVar1->vbo);
      iVar2 = _glGetError();
      if (iVar2 != 0) {
        uVar3 = _BKGetGLErrorMessage(iVar2);
        _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
                 "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___21256_001c50bc,0x21e,uVar3);
      }
    }
    _free(pTVar1->tileMap);
    _free(pTVar1);
    *tileMapStructPP = (TileMapStructPtr)0x0;
  }
  return;
}


// ---- _SWInstallTileMap @ 0x81c35 (222 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWInstallTileMap(SpriteWorldPtr spriteWorldP,TileMapStructPtr tileMapStructP,short tileLayer)

{
  short local_e;
  
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___21297_001c5078,0x234);
  }
  if (9 < tileLayer) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___21297_001c5078,0x235);
  }
  spriteWorldP->tileLayerArray[tileLayer] = tileMapStructP;
  spriteWorldP->lastActiveTileLayer = 0;
  for (local_e = 0; local_e < 10; local_e = local_e + 1) {
    if (spriteWorldP->tileLayerArray[local_e] != (TileMapStruct *)0x0) {
      spriteWorldP->lastActiveTileLayer = local_e;
    }
  }
  return;
}


// ---- _SWLoadTileMap @ 0x81d13 (272 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError _SWLoadTileMap(char *file,TileMapStructPtr *tileMapStructPP)

{
  TileMapStructPtr local_24;
  int local_20;
  short local_1a;
  short local_18;
  short local_16;
  short local_14;
  short local_12;
  int local_10;
  
  local_10 = 0;
  local_24 = *tileMapStructPP;
  local_20 = _SDL_RWFromZZIP(file,"rb");
  if (local_20 == 0) {
    local_10 = -1;
  }
  else {
    local_1a = _SDL_ReadBE16(local_20);
    local_18 = _SDL_ReadBE16(local_20);
  }
  if (local_10 == 0) {
    local_10 = _SWCreateTileMap(&local_24,local_1a,local_18);
  }
  if (local_10 == 0) {
    for (local_16 = 0; local_16 < local_1a; local_16 = local_16 + 1) {
      for (local_14 = 0; local_14 < local_18; local_14 = local_14 + 1) {
        local_12 = _SDL_ReadBE16(local_20);
        local_24->tileMap[local_16][local_14] = local_12;
      }
    }
    (**(code **)(local_20 + 0xc))(local_20);
    *tileMapStructPP = local_24;
  }
  return local_10;
}


// ---- _SWSaveTileMap @ 0x81e23 (328 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError SWSaveTileMap(char *file,TileMapStructPtr tileMapStructP)

{
  short sStack_1e;
  short sStack_1c;
  short sStack_1a;
  FILE *pFStack_18;
  short sStack_14;
  short sStack_12;
  int iStack_10;
  
  iStack_10 = 0;
  if (tileMapStructP == (TileMapStructPtr)0x0) {
    iStack_10 = 0x6e;
  }
  if (iStack_10 == 0) {
    pFStack_18 = _fopen(file,"wb");
    if (pFStack_18 == (FILE *)0x0) {
      iStack_10 = -1;
    }
    else {
      sStack_1a = tileMapStructP->numRows;
      sStack_1c = tileMapStructP->numCols;
      _fwrite(&sStack_1a,2,1,pFStack_18);
      _fwrite(&sStack_1c,2,1,pFStack_18);
      for (sStack_14 = 0; sStack_14 < sStack_1a; sStack_14 = sStack_14 + 1) {
        for (sStack_12 = 0; sStack_12 < sStack_1c; sStack_12 = sStack_12 + 1) {
          sStack_1e = tileMapStructP->tileMap[sStack_14][sStack_12];
          _fwrite(&sStack_1e,2,1,pFStack_18);
        }
      }
    }
    _fclose(pFStack_18);
  }
  _SWSetStickyIfError(iStack_10);
  return iStack_10;
}


// ---- _SWResizeTileMap @ 0x81f6b (643 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError SWResizeTileMap(TileMapStructPtr tileMapStructP,short numNewTileMapRows,
                       short numNewTileMapCols)

{
  short sVar1;
  uchar uVar2;
  undefined2 uVar3;
  undefined3 uVar4;
  int iVar5;
  short *psVar6;
  short *psVar7;
  short sStack_82;
  short sStack_7e;
  TileMapStructPtr pTStack_64;
  undefined4 uStack_60;
  short **ppsStack_5c;
  float fStack_58;
  undefined4 uStack_54;
  short **ppsStack_50;
  undefined4 uStack_4c;
  uchar **ppuStack_48;
  undefined4 uStack_44;
  uint uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  short sStack_2a;
  short sStack_28;
  short sStack_26;
  int iStack_24;
  int iStack_20;
  
  iStack_20 = 0;
  if ((numNewTileMapRows < 1) || (numNewTileMapCols < 1)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___21423_001c5090,0x2c7);
  }
  if (tileMapStructP == (TileMapStructPtr)0x0) {
    iStack_20 = 0x6e;
  }
  if (iStack_20 == 0) {
    if ((tileMapStructP->numRows == numNewTileMapRows) &&
       (tileMapStructP->numCols == numNewTileMapCols)) {
      return 0;
    }
    iStack_20 = _SWCreateTileMap(&pTStack_64,numNewTileMapRows,numNewTileMapCols);
  }
  if (iStack_20 == 0) {
    sStack_7e = tileMapStructP->numRows;
    if (numNewTileMapRows < sStack_7e) {
      sStack_7e = numNewTileMapRows;
    }
    sStack_28 = sStack_7e;
    sStack_82 = tileMapStructP->numCols;
    if (numNewTileMapCols < sStack_82) {
      sStack_82 = numNewTileMapCols;
    }
    sStack_26 = sStack_82;
    iStack_24 = sStack_82 * 2;
    for (sStack_2a = 0; sStack_2a < sStack_7e; sStack_2a = sStack_2a + 1) {
      psVar6 = tileMapStructP->tileMap[sStack_2a];
      psVar7 = pTStack_64->tileMap[sStack_2a];
      for (iVar5 = iStack_24; iVar5 != 0; iVar5 = iVar5 + -1) {
        *(char *)psVar7 = (char)*psVar6;
        psVar6 = (short *)((int)psVar6 + 1);
        psVar7 = (short *)((int)psVar7 + 1);
      }
    }
    uStack_60._0_2_ = tileMapStructP->numRows;
    uStack_60._2_2_ = tileMapStructP->numCols;
    ppsStack_5c = tileMapStructP->tileMap;
    fStack_58 = tileMapStructP->zoom;
    uStack_54._0_1_ = tileMapStructP->isVisible;
    uStack_54._1_1_ = tileMapStructP->drawEveryFrame;
    uStack_54._2_2_ = *(undefined2 *)&tileMapStructP->field_0xe;
    ppsStack_50 = tileMapStructP->changedTileMap;
    uStack_4c = *(undefined4 *)&tileMapStructP->tilesChanged;
    ppuStack_48 = tileMapStructP->tileChangedFlags;
    uStack_44 = *(undefined4 *)&tileMapStructP->field_0x1c;
    uStack_40 = tileMapStructP->vbo;
    uStack_3c = *(undefined4 *)&tileMapStructP->field_0x24;
    uStack_38 = *(undefined4 *)&tileMapStructP->field_0x28;
    uStack_34 = *(undefined4 *)&tileMapStructP->field_0x2c;
    uStack_30 = *(undefined4 *)&tileMapStructP->vboMethod;
    sVar1 = pTStack_64->numCols;
    tileMapStructP->numRows = pTStack_64->numRows;
    tileMapStructP->numCols = sVar1;
    tileMapStructP->tileMap = pTStack_64->tileMap;
    tileMapStructP->zoom = pTStack_64->zoom;
    uVar2 = pTStack_64->drawEveryFrame;
    uVar3 = *(undefined2 *)&pTStack_64->field_0xe;
    tileMapStructP->isVisible = pTStack_64->isVisible;
    tileMapStructP->drawEveryFrame = uVar2;
    *(undefined2 *)&tileMapStructP->field_0xe = uVar3;
    tileMapStructP->changedTileMap = pTStack_64->changedTileMap;
    uVar4 = *(undefined3 *)&pTStack_64->field_0x15;
    tileMapStructP->tilesChanged = pTStack_64->tilesChanged;
    *(undefined3 *)&tileMapStructP->field_0x15 = uVar4;
    tileMapStructP->tileChangedFlags = pTStack_64->tileChangedFlags;
    *(undefined4 *)&tileMapStructP->field_0x1c = *(undefined4 *)&pTStack_64->field_0x1c;
    tileMapStructP->vbo = pTStack_64->vbo;
    *(undefined4 *)&tileMapStructP->field_0x24 = *(undefined4 *)&pTStack_64->field_0x24;
    *(undefined4 *)&tileMapStructP->field_0x28 = *(undefined4 *)&pTStack_64->field_0x28;
    *(undefined4 *)&tileMapStructP->field_0x2c = *(undefined4 *)&pTStack_64->field_0x2c;
    uVar4 = *(undefined3 *)&pTStack_64->field_0x31;
    tileMapStructP->vboMethod = pTStack_64->vboMethod;
    *(undefined3 *)&tileMapStructP->field_0x31 = uVar4;
    pTStack_64->numRows = (undefined2)uStack_60;
    pTStack_64->numCols = uStack_60._2_2_;
    pTStack_64->tileMap = ppsStack_5c;
    pTStack_64->zoom = fStack_58;
    pTStack_64->isVisible = (undefined1)uStack_54;
    pTStack_64->drawEveryFrame = uStack_54._1_1_;
    *(undefined2 *)&pTStack_64->field_0xe = uStack_54._2_2_;
    pTStack_64->changedTileMap = ppsStack_50;
    *(undefined4 *)&pTStack_64->tilesChanged = uStack_4c;
    pTStack_64->tileChangedFlags = ppuStack_48;
    *(undefined4 *)&pTStack_64->field_0x1c = uStack_44;
    pTStack_64->vbo = uStack_40;
    *(undefined4 *)&pTStack_64->field_0x24 = uStack_3c;
    *(undefined4 *)&pTStack_64->field_0x28 = uStack_38;
    *(undefined4 *)&pTStack_64->field_0x2c = uStack_34;
    *(undefined4 *)&pTStack_64->vboMethod = uStack_30;
    _SWDisposeTileMap(&pTStack_64);
  }
  return iStack_20;
}


// ---- _SWInstallTileLayerBelowSpriteLayer @ 0x821ee (90 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWInstallTileLayerBelowSpriteLayer(short tileLayer,SpriteLayerPtr spriteLayerP)

{
  if (spriteLayerP == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___21472_001c504c,0x304);
  }
  spriteLayerP->tileLayerBelow = tileLayer;
  return;
}


// ---- _SWInstallTileLayerAboveSpriteLayer @ 0x82248 (90 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWInstallTileLayerAboveSpriteLayer(short tileLayer,SpriteLayerPtr spriteLayerP)

{
  if (spriteLayerP == (SpriteLayerPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___21480_001c5054,0x310);
  }
  spriteLayerP->tileLayerAbove = tileLayer;
  return;
}


// ---- _SWFillTileMapWithValue @ 0x822a2 (147 bytes)

void SWFillTileMapWithValue(short *param_1,undefined2 param_2)

{
  short sStack_10;
  short sStack_e;
  
  for (sStack_10 = 0; sStack_10 < *param_1; sStack_10 = sStack_10 + 1) {
    for (sStack_e = 0; sStack_e < param_1[1]; sStack_e = sStack_e + 1) {
      *(undefined2 *)(*(int *)(*(int *)(param_1 + 2) + sStack_10 * 4) + sStack_e * 2) = param_2;
      *(undefined2 *)(*(int *)(*(int *)(param_1 + 8) + sStack_10 * 4) + sStack_e * 2) = param_2;
    }
  }
  *(undefined1 *)(param_1 + 10) = 0;
  return;
}


// ---- _SWSetTileMapVBOMethod @ 0x82335 (30 bytes)

void SWSetTileMapVBOMethod(int param_1,undefined1 param_2)

{
  *(undefined1 *)(param_1 + 0x30) = param_2;
  return;
}


// ---- _SWSetTileMapZoom @ 0x82353 (23 bytes)

void SWSetTileMapZoom(int param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 8) = param_2;
  return;
}


// ---- _SWSetTileLayersVisible @ 0x8236a (113 bytes)

void _SWSetTileLayersVisible(int param_1,short param_2,short param_3,undefined1 param_4)

{
  undefined2 local_e;
  
  for (local_e = param_2; local_e <= param_3; local_e = local_e + 1) {
    if (*(int *)(*(int *)(param_1 + 0xc4) + local_e * 4) != 0) {
      *(undefined1 *)(*(int *)(*(int *)(param_1 + 0xc4) + local_e * 4) + 0xc) = param_4;
    }
  }
  return;
}


// ---- _SWSetTileLayerToDrawEveryFrame @ 0x823db (55 bytes)

void _SWSetTileLayerToDrawEveryFrame(int param_1,short param_2,undefined1 param_3)

{
  *(undefined1 *)(*(int *)(*(int *)(param_1 + 0xc4) + param_2 * 4) + 0xd) = param_3;
  return;
}


// ---- _SWSetTileTranslucency @ 0x82412 (131 bytes)

void SWSetTileTranslucency(SpriteWorldPtr param_1,short param_2,undefined2 param_3)

{
  *(undefined1 *)((int)param_1->tileColorArray + param_2 * 10) = 1;
  *(undefined2 *)((int)param_1->tileColorArray + param_2 * 10 + 8) = param_3;
  if ((param_1->masterTileSurface != (void *)0x0) ||
     (param_1->backgroundTileSpriteP != (SpriteRec *)0x0)) {
    _SWUpdateTileOnScreen(param_1,param_2);
  }
  return;
}


// ---- _SWSetTileTranslucencyForRange @ 0x82495 (172 bytes)

void _SWSetTileTranslucencyForRange
               (SpriteWorldPtr param_1,short param_2,short param_3,undefined2 param_4)

{
  short local_e;
  
  for (local_e = param_2; local_e <= param_3; local_e = local_e + 1) {
    *(undefined1 *)((int)param_1->tileColorArray + local_e * 10) = 1;
    *(undefined2 *)((int)param_1->tileColorArray + local_e * 10 + 8) = param_4;
  }
  if ((param_1->masterTileSurface != (void *)0x0) ||
     (param_1->backgroundTileSpriteP != (SpriteRec *)0x0)) {
    _SWUpdateTilesOnScreenFast(param_1,param_2,param_3);
  }
  return;
}


// ---- _SWSetTileColor @ 0x82541 (216 bytes)

void SWSetTileColor(SpriteWorldPtr param_1,short param_2,undefined2 param_3,undefined2 param_4,
                   undefined2 param_5)

{
  *(undefined1 *)((int)param_1->tileColorArray + param_2 * 10) = 1;
  *(undefined2 *)((int)param_1->tileColorArray + param_2 * 10 + 2) = param_3;
  *(undefined2 *)((int)param_1->tileColorArray + param_2 * 10 + 4) = param_4;
  *(undefined2 *)((int)param_1->tileColorArray + param_2 * 10 + 6) = param_5;
  if ((param_1->masterTileSurface != (void *)0x0) ||
     (param_1->backgroundTileSpriteP != (SpriteRec *)0x0)) {
    _SWUpdateTileOnScreen(param_1,param_2);
  }
  return;
}


// ---- _SWSetTileColorForRange @ 0x82619 (320 bytes)

void _SWSetTileColorForRange
               (SpriteWorldPtr param_1,short param_2,short param_3,undefined2 param_4,
               undefined2 param_5,undefined2 param_6,undefined2 param_7)

{
  short local_e;
  
  for (local_e = param_2; local_e <= param_3; local_e = local_e + 1) {
    *(undefined1 *)((int)param_1->tileColorArray + local_e * 10) = 1;
    *(undefined2 *)((int)param_1->tileColorArray + local_e * 10 + 2) = param_4;
    *(undefined2 *)((int)param_1->tileColorArray + local_e * 10 + 4) = param_5;
    *(undefined2 *)((int)param_1->tileColorArray + local_e * 10 + 6) = param_6;
    *(undefined2 *)((int)param_1->tileColorArray + local_e * 10 + 8) = param_7;
  }
  if ((param_1->masterTileSurface != (void *)0x0) ||
     (param_1->backgroundTileSpriteP != (SpriteRec *)0x0)) {
    _SWUpdateTilesOnScreenFast(param_1,param_2,param_3);
  }
  return;
}


// ---- _SWSetTileLayerCallback @ 0x82759 (99 bytes)

void _SWSetTileLayerCallback(int param_1,short param_2,undefined4 param_3)

{
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___21692_001c50ac,0x3bb);
  }
  *(undefined4 *)(*(int *)(param_1 + 0x58) + param_2 * 4) = param_3;
  return;
}


// ---- _SWSetStdTileColor @ 0x827bc (181 bytes)

void _SWSetStdTileColor(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
                       undefined2 param_5,undefined1 param_6)

{
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___21709_001c5048,0x3cf);
  }
  *(undefined2 *)(param_1 + 0x108) = param_2;
  *(undefined2 *)(param_1 + 0x10a) = param_3;
  *(undefined2 *)(param_1 + 0x10c) = param_4;
  *(undefined2 *)(param_1 + 0x10e) = param_5;
  *(undefined1 *)(param_1 + 0x110) = param_6;
  return;
}


// ---- _SWCloneTiles @ 0x82871 (534 bytes)

void _SWCloneTiles(SpriteWorldPtr param_1,short param_2,short param_3,short param_4)

{
  short local_10;
  short local_e;
  
  local_10 = param_4;
  for (local_e = param_2; local_e <= param_3; local_e = local_e + 1) {
    if (param_1->maxNumTiles <= local_e) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___21721_001c509c,0x3ee);
    }
    if (param_1->masterTileSurface == (void *)0x0) {
      if (param_1->tileFrameArray[local_e] != (FrameRec *)0x0) {
        _SWDisposeTile(param_1,local_e);
      }
      param_1->tileFrameArray[local_e] = param_1->tileFrameArray[local_10];
      param_1->tileFrameArray[local_e]->useCount = param_1->tileFrameArray[local_e]->useCount + 1;
    }
    param_1->tileSourceIndex[local_e] = local_10;
    *(undefined1 *)((int)param_1->tileColorArray + local_e * 10) =
         *(undefined1 *)((int)param_1->tileColorArray + local_10 * 10);
    *(undefined2 *)((int)param_1->tileColorArray + local_e * 10 + 2) =
         *(undefined2 *)((int)param_1->tileColorArray + local_10 * 10 + 2);
    *(undefined2 *)((int)param_1->tileColorArray + local_e * 10 + 4) =
         *(undefined2 *)((int)param_1->tileColorArray + local_10 * 10 + 4);
    *(undefined2 *)((int)param_1->tileColorArray + local_e * 10 + 6) =
         *(undefined2 *)((int)param_1->tileColorArray + local_10 * 10 + 6);
    local_10 = local_10 + 1;
  }
  return;
}


// ---- _SWLoadTilesFromSingleFileXY @ 0x82a87 (720 bytes)

SWError _SWLoadTilesFromSingleFileXY
                  (SpriteWorldPtr param_1,char *param_2,int param_3,int param_4,char param_5,
                  int param_6,short param_7)

{
  FramePtr local_34;
  SWRect local_30;
  int local_28;
  SDL_Surface *local_24;
  int local_20;
  int local_1c;
  int local_18;
  short local_12;
  short local_10;
  short local_e;
  
  local_12 = param_7;
  local_28 = 0;
  local_20 = 0;
  local_1c = 0;
  local_34 = (FrameRec *)0x0;
  local_10 = param_1->tileImageWidth;
  local_e = param_1->tileImageHeight;
  if (param_6 < 1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___21828_001c50b4,0x433);
  }
  local_24 = _BKLoadSurface(param_2);
  if (local_24 == (SDL_Surface *)0x0) {
    local_28 = 0x12f;
  }
  if (local_28 == 0) {
    if (param_5 == '\0') {
      local_20 = 0;
      local_1c = 0;
    }
    else {
      local_20 = param_3 / 2;
      local_1c = param_4 / 2;
    }
  }
  if (local_28 == 0) {
    local_30.left = (short)local_20;
    local_30.top = (short)local_1c;
    local_30.right = local_30.left + local_10;
    local_30.bottom = local_30.top + local_e;
    for (local_18 = 0; local_18 < param_6; local_18 = local_18 + 1) {
      if (param_1->maxNumTiles <= local_12) {
        local_28 = 0x6b;
      }
      if (local_28 == 0) {
        if (param_1->masterTileSurface == (void *)0x0) {
          local_28 = _SWCreateFrameFromSurfaceAndRect(&local_34,local_24,&local_30);
        }
        else {
          local_28 = _SWAddTileToMasterSurface(param_1,(int)local_12,local_24,&local_30);
        }
      }
      if (local_28 == 0) {
        if (local_34 != (FrameRec *)0x0) {
          if (param_1->tileFrameArray[local_12] != (FrameRec *)0x0) {
            _SWDisposeTile(param_1,local_12);
          }
          param_1->tileFrameArray[local_12] = local_34;
          local_34->useCount = local_34->useCount + 1;
        }
        if (local_18 < param_6 + -1) {
          local_30.left = local_30.left + (short)param_3 + local_10;
          local_30.right = local_30.left + local_10;
          if (local_24->w < (int)local_30.right) {
            local_30.left = (short)local_20;
            local_30.right = local_30.left + local_10;
            local_30.top = local_30.top + (short)param_4 + local_e;
            local_30.bottom = local_30.top + local_e;
            if (local_24->h < (int)local_30.bottom) {
              local_28 = 0x6b;
            }
          }
        }
      }
      if ((local_28 != 0) && (local_34 != (FrameRec *)0x0)) {
        _SWDisposeTile(param_1,local_12);
        break;
      }
      local_12 = local_12 + 1;
    }
  }
  if (param_1->masterTileSurface != (void *)0x0) {
    _SDL_FreeSurface(local_24);
  }
  _SWSetStickyIfError(local_28);
  return local_28;
}


// ---- _SWCreateMasterTileSurface @ 0x82d57 (161 bytes)

undefined4 SWCreateMasterTileSurface(int param_1,short param_2,short param_3)

{
  undefined4 uVar1;
  undefined4 uStack_10;
  
  uStack_10 = 0;
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___21931_001c508c,0x4a5);
  }
  if (*(int *)(param_1 + 0x114) == 0) {
    uVar1 = _BKCreateMasterSurfaceForTiles((int)param_2,(int)param_3);
    *(undefined4 *)(param_1 + 0x114) = uVar1;
    *(undefined2 *)(param_1 + 0x118) = 0;
  }
  else {
    uStack_10 = 0xffffffff;
  }
  return uStack_10;
}


// ---- _SWAddTileToMasterSurface @ 0x82df8 (301 bytes)

undefined4
_SWAddTileToMasterSurface(int param_1,short param_2,undefined4 param_3,undefined4 param_4)

{
  undefined1 local_1c [4];
  short local_18;
  int local_14;
  undefined4 local_10;
  
  local_10 = 0;
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___21953_001c50a0,0x4bd);
  }
  local_14 = **(int **)(param_1 + 0x114);
  _BKGetTileRectForMasterSurface
            (local_14,(int)param_2,(int)*(short *)(param_1 + 0xe6),(int)*(short *)(param_1 + 0xe8),
             local_1c);
  if (*(int *)(local_14 + 0xc) < (int)local_18) {
    _printf("\nTileID %d could not be added to master surface, because no more room in the master surface! "
            ,(int)param_2);
    _printf("tile\'s destRect.bottom: %d. masterSurface\'s height: %d \n",(int)local_18,
            *(undefined4 *)(local_14 + 0xc));
    local_10 = 0x6b;
  }
  else {
    _BKAddTileToMasterSurface(param_3,local_14,param_4,local_1c);
    *(short *)(param_1 + 0x118) = *(short *)(param_1 + 0x118) + 1;
  }
  return local_10;
}


// ---- _SWFinishMasterTileSurface @ 0x82f25 (170 bytes)

undefined4 SWFinishMasterTileSurface(int param_1)

{
  undefined4 uStack_10;
  
  if (param_1 == 0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___21980_001c5094,0x4de);
  }
  if (*(int *)(param_1 + 0x114) == 0) {
    uStack_10 = 0;
  }
  else {
    uStack_10 = _BKCreateGLSurfaceFromMaster
                          (*(undefined4 *)(param_1 + 0x114),(int)*(short *)(param_1 + 0x118),
                           (int)*(short *)(param_1 + 0xe6),(int)*(short *)(param_1 + 0xe8));
  }
  return uStack_10;
}


// ---- _SWLoadTilesFromFileSequence @ 0x82fcf (533 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError SWLoadTilesFromFileSequence
                  (char *filename,int start,int numTiles,SpriteWorldPtr spriteWorldP,
                  short startTileID)

{
  FrameRec *pFStack_11c;
  char acStack_117 [255];
  int iStack_18;
  int iStack_14;
  short sStack_e;
  
  sStack_e = startTileID;
  iStack_18 = 0;
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___22009_001c50dc,0x4fd);
  }
  if (filename == (char *)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___22009_001c50dc,0x4fe);
  }
  if (numTiles < 1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___22009_001c50dc,0x4ff);
  }
  if ((startTileID < 0) || (spriteWorldP->maxNumTiles <= startTileID)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___22009_001c50dc,0x500);
  }
  iStack_14 = 0;
  do {
    if (numTiles <= iStack_14) {
LAB_000831cd:
      _SWSetStickyIfError(iStack_18);
      return iStack_18;
    }
    _sprintf(acStack_117,filename,start + iStack_14);
    if (spriteWorldP->maxNumTiles <= sStack_e) {
      iStack_18 = 0x6b;
    }
    if (iStack_18 == 0) {
      iStack_18 = _SWCreateFrameFromFile(&pFStack_11c,acStack_117,0);
    }
    if (iStack_18 == 0) {
      if (spriteWorldP->tileFrameArray[sStack_e] != (FrameRec *)0x0) {
        _SWDisposeTile(spriteWorldP,sStack_e);
      }
      spriteWorldP->tileFrameArray[sStack_e] = pFStack_11c;
      pFStack_11c->useCount = pFStack_11c->useCount + 1;
    }
    if (iStack_18 != 0) {
      _SWDisposeTile(spriteWorldP,sStack_e);
      goto LAB_000831cd;
    }
    sStack_e = sStack_e + 1;
    iStack_14 = iStack_14 + 1;
  } while( true );
}


// ---- _SWDisposeTile @ 0x831e4 (341 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWDisposeTile(SpriteWorldPtr spriteWorldP,short tileID)

{
  short local_12;
  int local_10;
  
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___22065_001c507c,0x555);
  }
  if (spriteWorldP->tilingIsInitialized == '\0') {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___22065_001c507c,0x556);
  }
  local_10 = 0;
  local_12 = spriteWorldP->maxNumTiles;
  while (local_12 = local_12 + -1, local_12 != -1) {
    if (((local_12 != tileID) && (spriteWorldP->tileFrameArray[local_12] != (FrameRec *)0x0)) &&
       (spriteWorldP->tileFrameArray[local_12]->frameSurfaceP ==
        spriteWorldP->tileFrameArray[tileID]->frameSurfaceP)) {
      local_10 = 1;
    }
  }
  spriteWorldP->tileFrameArray[tileID]->sharesSurface = local_10;
  _SWDisposeFrame(spriteWorldP->tileFrameArray + tileID);
  spriteWorldP->tileFrameArray[tileID] = (FrameRec *)0x0;
  return;
}


// ---- _SWLockTiles @ 0x83339 (183 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWLockTiles(SpriteWorldPtr spriteWorldP)

{
  short sStack_e;
  
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___22123_001c5068,0x57e);
  }
  if (spriteWorldP->tilingIsInitialized != '\0') {
    for (sStack_e = 0; sStack_e < spriteWorldP->maxNumTiles; sStack_e = sStack_e + 1) {
      if ((spriteWorldP->tileFrameArray != (FrameRec **)0x0) &&
         (spriteWorldP->tileFrameArray[sStack_e] != (FrameRec *)0x0)) {
        _SWLockFrame(spriteWorldP->tileFrameArray[sStack_e]);
      }
    }
  }
  return;
}


// ---- _SWUnlockTiles @ 0x833f0 (183 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWUnlockTiles(SpriteWorldPtr spriteWorldP)

{
  short sStack_e;
  
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___22155_001c50d0,0x59c);
  }
  if (spriteWorldP->tilingIsInitialized != '\0') {
    for (sStack_e = 0; sStack_e < spriteWorldP->maxNumTiles; sStack_e = sStack_e + 1) {
      if ((spriteWorldP->tileFrameArray != (FrameRec **)0x0) &&
         (spriteWorldP->tileFrameArray[sStack_e] != (FrameRec *)0x0)) {
        _SWUnlockFrame(spriteWorldP->tileFrameArray[sStack_e]);
      }
    }
  }
  return;
}


// ---- _SWSetTilingOn @ 0x834a7 (91 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetTilingOn(SpriteWorldPtr spriteWorldP,SWBoolean tilingIsOn)

{
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___22187_001c5088,0x5e7);
  }
  spriteWorldP->tilingIsOn = tilingIsOn;
  return;
}


// ---- _SWChangeTileSize @ 0x83502 (117 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWError SWChangeTileSize(SpriteWorldPtr spriteWorldP,short tileHeight,short tileWidth)

{
  SWError SStack_10;
  
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___22197_001c5058,0x5f6);
  }
  spriteWorldP->tileHeight = tileHeight;
  spriteWorldP->tileWidth = tileWidth;
  return SStack_10;
}


// ---- _SWSetCustomTileDrawProc @ 0x83577 (84 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWSetCustomTileDrawProc(SpriteWorldPtr spriteWorldP,void *customTileDrawProc)

{
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___22207_001c50d4,0x62f);
  }
  spriteWorldP->screenDrawProc = (DrawProcPtr_t)customTileDrawProc;
  return;
}


// ---- _SWStdCustomTileDrawProc @ 0x835cb (97 bytes)

void SWStdCustomTileDrawProc(void)

{
  (**(code **)PTR__gSWAssertFailProc_001c4170)
            ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
             PTR____FUNCTION___22222_001c505c,0x647);
  return;
}


// ---- _SWSetTileChangeProc @ 0x8362c (84 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWSetTileChangeProc(SpriteWorldPtr spriteWorldP,void *tileChangeProc)

{
  if (spriteWorldP == (SpriteWorldPtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___22228_001c50b8,0x658);
  }
  spriteWorldP->tileChangeProc = tileChangeProc;
  return;
}


// ---- _SWValidateAllTileLayers @ 0x83680 (63 bytes)

void SWValidateAllTileLayers(int param_1)

{
  undefined2 uStack_e;
  
  for (uStack_e = 0; uStack_e <= *(short *)(param_1 + 0xca); uStack_e = uStack_e + 1) {
    _SWValidateTileLayer(param_1,(int)uStack_e);
  }
  return;
}


// ---- _SWValidateTileLayer @ 0x836bf (229 bytes)

void _SWValidateTileLayer(int param_1,short param_2)

{
  short sVar1;
  short *psVar2;
  short local_12;
  short local_10;
  
  psVar2 = *(short **)(*(int *)(param_1 + 0xc4) + param_2 * 4);
  for (local_12 = 0; local_12 < *psVar2; local_12 = local_12 + 1) {
    for (local_10 = 0; local_10 < psVar2[1]; local_10 = local_10 + 1) {
      sVar1 = *(short *)(*(int *)(*(int *)(psVar2 + 2) + local_12 * 4) + local_10 * 2);
      if ((sVar1 < -1) || (*(short *)(param_1 + 0xe0) <= sVar1)) {
        _printf("\n  TileID is invalid! layer %d, row %d, col %d, tileID %d. \n",(int)param_2,
                (int)local_12,(int)local_10,(int)sVar1);
      }
    }
  }
  return;
}


// ---- _SWResetVBOForAllTileLayers @ 0x837a4 (255 bytes)

void _SWResetVBOForAllTileLayers(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  short local_12;
  
  for (local_12 = 0; local_12 <= *(short *)(param_1 + 0xca); local_12 = local_12 + 1) {
    if (*(int *)(*(int *)(*(int *)(param_1 + 0xc4) + local_12 * 4) + 0x20) != 0) {
      _glDeleteBuffers(1,*(int *)(*(int *)(param_1 + 0xc4) + local_12 * 4) + 0x20);
      iVar1 = _glGetError();
      if (iVar1 != 0) {
        uVar2 = _BKGetGLErrorMessage(iVar1);
        _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
                 "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___22286_001c5050,0x691,uVar2);
      }
    }
    *(undefined4 *)(*(int *)(*(int *)(param_1 + 0xc4) + local_12 * 4) + 0x20) = 0;
  }
  return;
}


// ---- _SWDrawTile @ 0x838a3 (1490 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWDrawTile(SpriteWorldPtr spriteWorldP,short dstTileLayer,short tileRow,short tileCol,
                short tileID)

{
  TileMapStruct *pTVar1;
  int iVar2;
  short local_3c;
  short local_3a;
  short local_38;
  short local_36;
  short local_34;
  short local_32;
  short local_30;
  short local_2e;
  short local_2c;
  short local_2a;
  short local_28;
  short local_26;
  short local_24;
  short local_22;
  short local_20;
  short local_1e;
  int local_1c;
  TileMapStruct *local_18;
  int local_14;
  short local_10;
  short local_e;
  
  if (spriteWorldP->tileLayerArray[dstTileLayer] != (TileMapStruct *)0x0) {
    if (spriteWorldP == (SpriteWorldPtr)0x0) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___22332_001c5074,0x6dc);
    }
    if (spriteWorldP->tilingIsInitialized == '\0') {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___22332_001c5074,0x6dd);
    }
    if (dstTileLayer < 0) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___22332_001c5074,0x6e0);
    }
    if (spriteWorldP->lastActiveTileLayer < dstTileLayer) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___22332_001c5074,0x6e1);
    }
    if ((spriteWorldP->maxNumTiles <= tileID) &&
       (_printf("TileID out of range! tileID is %d while maxNumTiles is %d \n",(int)tileID,
                (int)spriteWorldP->maxNumTiles), spriteWorldP->maxNumTiles <= tileID)) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___22332_001c5074,0x6e6);
    }
    local_18 = spriteWorldP->tileLayerArray[dstTileLayer];
    if (local_18 == (TileMapStruct *)0x0) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___22332_001c5074,0x6ea);
    }
    if (local_18->numRows <= tileRow) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___22332_001c5074,0x6eb);
    }
    if (local_18->numCols <= tileCol) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___22332_001c5074,0x6ec);
    }
    local_10 = local_18->tileMap[tileRow][tileCol];
    local_18->tileMap[tileRow][tileCol] = tileID;
    if ((spriteWorldP->backgroundTileSpriteP == (SpriteRec *)0x0) ||
       ((local_18->isVisible != '\0' && (local_18->vbo != 0)))) {
      if (local_18->vbo != 0) {
        if (tileID < 0) {
          if (-1 < local_10) {
            _glBindBuffer(0x8892,local_18->vbo);
            iVar2 = (int)tileRow * (int)local_18->numCols + (int)tileCol;
            local_14 = iVar2 * 0xc;
            _glBufferSubData(0x8892,iVar2 * 0x18,0x18,PTR__gEmptyTileVertices_001c50c0);
            _glBufferSubData(0x8892,*(int *)&local_18->field_0x24 + local_14 * 4,0x30,
                             PTR__gEmptyTileTexCoords_001c50d8);
          }
        }
        else {
          local_1c = *(int *)(*(int *)&local_18->field_0x2c + 0x60) +
                     spriteWorldP->curTileImage[tileID] * 0x30;
          local_14 = ((int)tileRow * (int)local_18->numCols + (int)tileCol) * 0x30;
          _glBindBuffer(0x8892,local_18->vbo);
          _glBufferSubData(0x8892,*(int *)&local_18->field_0x24 + local_14,0x30,local_1c);
          if (local_10 < 0) {
            local_3a = spriteWorldP->tileHeight * tileRow;
            local_3c = spriteWorldP->tileHeight * tileCol;
            local_32 = local_3a + spriteWorldP->tileHeight;
            local_38 = local_3c + spriteWorldP->tileWidth;
            local_14 = ((int)tileRow * (int)local_18->numCols + (int)tileCol) * 0x18;
            local_36 = local_3a;
            local_34 = local_3c;
            local_30 = local_38;
            local_2e = local_3a;
            local_2c = local_3c;
            local_2a = local_32;
            local_28 = local_38;
            local_26 = local_32;
            local_24 = local_3a;
            local_22 = local_3c;
            local_20 = local_32;
            local_1e = local_38;
            _glBufferSubData(0x8892,local_14,0x18,&local_3c);
          }
        }
      }
    }
    else {
      local_18->changedTileMap[tileRow][tileCol] = tileID;
      local_18->tilesChanged = '\x01';
      for (local_e = dstTileLayer + 1; local_e <= spriteWorldP->lastActiveTileLayer;
          local_e = local_e + 1) {
        pTVar1 = spriteWorldP->tileLayerArray[local_e];
        if (pTVar1->tileMap[tileRow][tileCol] != -1) {
          pTVar1->changedTileMap[tileRow][tileCol] = pTVar1->tileMap[tileRow][tileCol];
          pTVar1->tilesChanged = '\x01';
        }
      }
    }
  }
  return;
}


// ---- _SWDrawTileNoVBO @ 0x83e75 (2060 bytes)

void SWDrawTileNoVBO(int param_1,short param_2,short param_3,short param_4,short param_5)

{
  short sVar1;
  short sStack_6c;
  short sStack_50;
  short sStack_4e;
  short sStack_4c;
  short sStack_4a;
  undefined4 uStack_48;
  undefined4 uStack_44;
  short sStack_40;
  short sStack_3e;
  short sStack_3c;
  short sStack_3a;
  short *psStack_38;
  short *psStack_34;
  short sStack_30;
  short sStack_2e;
  int iStack_2c;
  uint uStack_28;
  uint uStack_24;
  uint uStack_20;
  
  psStack_38 = (short *)(param_1 + 0x8c);
  psStack_34 = (short *)(param_1 + 0x7c);
  sStack_30 = *(short *)(param_1 + 0xe4);
  sStack_2e = *(short *)(param_1 + 0xe2);
  iStack_2c = 0;
  if (param_5 < 0) {
    uStack_24 = 1;
  }
  else {
    iStack_2c = *(int *)(*(int *)(param_1 + 0xd4) +
                        *(short *)(*(int *)(param_1 + 0xd8) + param_5 * 2) * 4);
    if (iStack_2c == 0) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___22561_001c5080,0x766);
    }
    uStack_24 = (uint)(*(int *)(iStack_2c + 0x14) != 0);
  }
  sStack_3a = param_2;
  if (uStack_24 != 0) {
    sStack_3a = 0;
  }
  sStack_40 = param_3 * sStack_30;
  sStack_3e = param_4 * sStack_2e;
  sStack_4c = sStack_30 + sStack_40;
  sStack_4a = sStack_2e + sStack_3e;
  sStack_50 = sStack_40;
  if (sStack_40 < *psStack_38) {
    sStack_50 = *psStack_38;
  }
  uStack_28 = (uint)(sStack_40 < *psStack_38);
  sStack_4e = sStack_3e;
  if (sStack_3e < psStack_38[1]) {
    uStack_28 = 1;
    sStack_4e = psStack_38[1];
  }
  if (psStack_38[2] < sStack_4c) {
    sStack_4c = psStack_38[2];
    uStack_28 = 1;
  }
  if (psStack_38[3] < sStack_4a) {
    sStack_4a = psStack_38[3];
    uStack_28 = 1;
  }
  if ((sStack_4e < sStack_4a) && (sStack_50 < sStack_4c)) {
    if (**(int **)(param_1 + 0xc4) == 0) {
      sStack_6c = -1;
    }
    else {
      sStack_6c = *(short *)(*(int *)(*(int *)(**(int **)(param_1 + 0xc4) + 4) + param_3 * 4) +
                            param_4 * 2);
    }
    if (sStack_6c < 0) {
      uStack_24 = 0;
    }
    else {
      iStack_2c = *(int *)(*(int *)(param_1 + 0xd4) +
                          *(short *)(*(int *)(param_1 + 0xd8) + sStack_6c * 2) * 4);
      if (iStack_2c == 0) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                   PTR____FUNCTION___22561_001c5080,0x7ad);
      }
      uStack_24 = (uint)(*(int *)(iStack_2c + 0x14) != 0);
    }
    sStack_50 = sStack_50 - *(short *)(param_1 + 0x9e);
    sStack_4c = sStack_4c - *(short *)(param_1 + 0x9e);
    sStack_4e = sStack_4e - *(short *)(param_1 + 0x9c);
    sStack_4a = sStack_4a - *(short *)(param_1 + 0x9c);
    if (psStack_34[2] < sStack_4c) {
      sStack_50 = sStack_50 - psStack_34[2];
      sStack_4c = sStack_4c - psStack_34[2];
    }
    else if (sStack_50 < *psStack_34) {
      sStack_50 = sStack_50 + psStack_34[2];
      sStack_4c = sStack_4c + psStack_34[2];
    }
    if (psStack_34[3] < sStack_4a) {
      sStack_4e = sStack_4e - psStack_34[3];
      sStack_4a = sStack_4a - psStack_34[3];
    }
    else if (sStack_4e < psStack_34[1]) {
      sStack_4e = sStack_4e + psStack_34[3];
      sStack_4a = sStack_4a + psStack_34[3];
    }
    if (-1 < sStack_6c) {
      uStack_44 = *(undefined4 *)(iStack_2c + 0x1c);
      uStack_48 = *(undefined4 *)(iStack_2c + 0x18);
      if (sStack_40 < *psStack_38) {
        uStack_48._2_2_ = (short)((uint)uStack_48 >> 0x10);
        uStack_48 = CONCAT22(uStack_48._2_2_,(short)uStack_48 + (*psStack_38 - sStack_40));
      }
      if (sStack_3e < psStack_38[1]) {
        uStack_48 = CONCAT22(uStack_48._2_2_ + (psStack_38[1] - sStack_3e),(short)uStack_48);
      }
      if ((int)psStack_38[2] < (int)sStack_40 + (int)sStack_30) {
        uStack_44._2_2_ = (short)((uint)uStack_44 >> 0x10);
        uStack_44 = CONCAT22(uStack_44._2_2_,
                             (short)uStack_44 - ((sStack_40 + sStack_30) - psStack_38[2]));
      }
      if ((int)psStack_38[3] < (int)sStack_3e + (int)sStack_2e) {
        uStack_44 = CONCAT22(uStack_44._2_2_ - ((sStack_3e + sStack_2e) - psStack_38[3]),
                             (short)uStack_44);
      }
      (**(code **)(param_1 + 0x100))
                (param_1,iStack_2c,&uStack_48,&sStack_50,0,(int)sStack_6c,
                 (int)*(short *)(*(int *)(param_1 + 0xd8) + sStack_6c * 2),uStack_24);
    }
    if (sStack_3a == 0) {
      sStack_3a = 1;
    }
    for (sStack_3c = sStack_3a; sStack_3c <= *(short *)(param_1 + 0xca); sStack_3c = sStack_3c + 1)
    {
      if (*(int *)(*(int *)(param_1 + 0xc4) + sStack_3c * 4) != 0) {
        if (**(short **)(*(int *)(param_1 + 0xc4) + sStack_3c * 4) <= param_3) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                     PTR____FUNCTION___22561_001c5080,0x7f7);
        }
        if (*(short *)(*(int *)(*(int *)(param_1 + 0xc4) + sStack_3c * 4) + 2) <= param_4) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                     PTR____FUNCTION___22561_001c5080,0x7f8);
        }
        sVar1 = *(short *)(*(int *)(*(int *)(*(int *)(*(int *)(param_1 + 0xc4) + sStack_3c * 4) + 4)
                                   + param_3 * 4) + param_4 * 2);
        if (-1 < sVar1) {
          if (*(short *)(param_1 + 0xe0) <= sVar1) {
            (**(code **)PTR__gSWAssertFailProc_001c4170)
                      ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c"
                       ,PTR____FUNCTION___22561_001c5080,0x7fe);
          }
          iStack_2c = *(int *)(*(int *)(param_1 + 0xd4) +
                              *(short *)(*(int *)(param_1 + 0xd8) + sVar1 * 2) * 4);
          if (iStack_2c == 0) {
            (**(code **)PTR__gSWAssertFailProc_001c4170)
                      ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c"
                       ,PTR____FUNCTION___22561_001c5080,0x801);
          }
          uStack_44 = *(undefined4 *)(iStack_2c + 0x1c);
          uStack_48 = *(undefined4 *)(iStack_2c + 0x18);
          if (sStack_40 < *psStack_38) {
            uStack_48._2_2_ = (short)((uint)uStack_48 >> 0x10);
            uStack_48 = CONCAT22(uStack_48._2_2_,(short)uStack_48 + (*psStack_38 - sStack_40));
          }
          if (sStack_3e < psStack_38[1]) {
            uStack_48 = CONCAT22(uStack_48._2_2_ + (psStack_38[1] - sStack_3e),(short)uStack_48);
          }
          if ((int)psStack_38[2] < (int)sStack_40 + (int)sStack_30) {
            uStack_44._2_2_ = (short)((uint)uStack_44 >> 0x10);
            uStack_44 = CONCAT22(uStack_44._2_2_,
                                 (short)uStack_44 - ((sStack_40 + sStack_30) - psStack_38[2]));
          }
          if ((int)psStack_38[3] < (int)sStack_3e + (int)sStack_2e) {
            uStack_44 = CONCAT22(uStack_44._2_2_ - ((sStack_3e + sStack_2e) - psStack_38[3]),
                                 (short)uStack_44);
          }
          uStack_20 = (uint)(*(int *)(iStack_2c + 0x14) != 0);
          (**(code **)(param_1 + 0x100))
                    (param_1,iStack_2c,&uStack_48,&sStack_50,(int)sStack_3c,(int)sVar1,
                     (int)*(short *)(*(int *)(param_1 + 0xd8) + sVar1 * 2),uStack_20);
        }
      }
    }
  }
  return;
}


// ---- _SWCreateBackgroundTextureFromTiles @ 0x84681 (374 bytes)

void _SWCreateBackgroundTextureFromTiles(SpriteWorldPtr param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = _glGetError();
  if (iVar1 != 0) {
    uVar2 = _BKGetGLErrorMessage(iVar1);
    _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
             "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
             PTR____FUNCTION___22881_001c50c4,0x833,uVar2);
  }
  if (param_1->backgroundTileSpriteP != (SpriteRec *)0x0) {
    _SWDisposeSprite(&param_1->backgroundTileSpriteP);
    param_1->backgroundTileSpriteP = (SpriteRec *)0x0;
  }
  _SWUpdateSpriteWorld(param_1,0);
  iVar1 = _glGetError();
  if (iVar1 != 0) {
    uVar2 = _BKGetGLErrorMessage(iVar1);
    _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
             "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
             PTR____FUNCTION___22881_001c50c4,0x83c,uVar2);
  }
  iVar1 = _SWCreateSpriteFromWorkArea
                    (param_1,&param_1->backgroundTileSpriteP,0,&param_1->backgroundTileRect,0);
  if (iVar1 == 0) {
    _SWSetSpriteScaledWidthFactor(param_1->backgroundTileSpriteP,1.0);
    _SWSetSpriteScaledHeightFactor(param_1->backgroundTileSpriteP,1.0);
  }
  return;
}


// ---- _SWDisposeBackgroundTileTexture @ 0x847f7 (48 bytes)

void _SWDisposeBackgroundTileTexture(int param_1)

{
  if (*(int *)(param_1 + 0x1c) != 0) {
    _SWDisposeSprite((SpritePtr *)(param_1 + 0x1c));
    *(undefined4 *)(param_1 + 0x1c) = 0;
  }
  return;
}


// ---- _SWDrawTileLayersInRect @ 0x84827 (96 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWDrawTileLayersInRect(SpriteWorldPtr spriteWorldP,SWRect *updateRectP)

{
  short sStack_e;
  
  if (spriteWorldP->backgroundTileSpriteP == (SpriteRec *)0x0) {
    for (sStack_e = 0; sStack_e <= spriteWorldP->lastActiveTileLayer; sStack_e = sStack_e + 1) {
      _SWDrawTileLayerInRect(spriteWorldP,sStack_e,updateRectP,'\x01','\0');
    }
  }
  return;
}


// ---- _SWDrawTileLayerInRect @ 0x84887 (2177 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

short _SWDrawTileLayerInRect
                (SpriteWorldRec *spriteWorldP,short tileLayer,SWRect *updateRectP,
                uchar useChangedMap,uchar forceDraw)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  TileMapStruct *pTVar7;
  undefined2 uVar10;
  undefined2 uVar11;
  FrameRec *pFVar8;
  void *pvVar9;
  bool bVar12;
  short sVar13;
  short sVar14;
  void *pvVar15;
  int iVar16;
  bool bVar17;
  short local_60;
  short sStack_5e;
  short local_5c;
  short sStack_5a;
  short local_48;
  short local_46;
  short local_44;
  short local_42;
  short local_40;
  void *local_2c;
  short **local_28;
  float local_24;
  float local_20;
  
  if ((spriteWorldP == (SpriteWorldRec *)0x0) || (spriteWorldP->tilingIsInitialized == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___22934_001c50a8,0x88d);
  }
  pTVar7 = spriteWorldP->tileLayerArray[tileLayer];
  if (pTVar7 == (TileMapStruct *)0x0) {
    sVar1 = 0;
  }
  else if ((((pTVar7->isVisible == '\0') && (pTVar7->tilesChanged == '\0')) && (forceDraw == '\0'))
          && (pTVar7->field_0x1c == '\0')) {
    sVar1 = 0;
  }
  else {
    _glEnable(*(undefined4 *)PTR__gSWTexExtension_001c4550);
    if (((spriteWorldP->useVBOForTiling == '\0') || (spriteWorldP->masterTileSurface == (void *)0x0)
        ) || (pTVar7->isVisible == '\0')) {
      uVar5 = updateRectP->bottom;
      uVar10 = updateRectP->right;
      uVar6 = updateRectP->top;
      uVar11 = updateRectP->left;
      sVar1 = spriteWorldP->tileWidth;
      sVar2 = spriteWorldP->tileHeight;
      bVar12 = false;
      bVar17 = spriteWorldP->tileWidth != spriteWorldP->tileImageWidth;
      if (bVar17) {
        local_24 = (float)(int)spriteWorldP->tileWidth / (float)(int)spriteWorldP->tileImageWidth;
        local_20 = (float)(int)spriteWorldP->tileHeight / (float)(int)spriteWorldP->tileImageHeight;
      }
      if (((spriteWorldP->backgroundTileSpriteP == (SpriteRec *)0x0) || (useChangedMap == '\0')) ||
         (pTVar7->drawEveryFrame != '\0')) {
        local_28 = pTVar7->tileMap;
      }
      else {
        local_28 = pTVar7->changedTileMap;
        if ((pTVar7->tilesChanged == '\0') && (pTVar7->field_0x1c == '\0')) {
          return 0;
        }
      }
      if (spriteWorldP->masterTileSurface != (void *)0x0) {
        _glBindTexture(*(undefined4 *)((int)spriteWorldP->masterTileSurface + 0x30),
                       **(undefined4 **)((int)spriteWorldP->masterTileSurface + 0x58));
        _glEnable(0xbe2);
        _glBlendFunc(0x302,0x303);
      }
      local_44 = (short)uVar6 / sVar2;
      sVar13 = (short)(((short)uVar5 + -1) / (int)sVar2);
      sVar14 = (short)(((short)uVar10 + -1) / (int)sVar1);
      if (pTVar7->numRows <= sVar13) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                   PTR____FUNCTION___22934_001c50a8,0x8ee);
      }
      if (pTVar7->numCols <= sVar14) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                   PTR____FUNCTION___22934_001c50a8,0x8ef);
      }
      pvVar15 = _BKLoadGLfunctions();
      _BKLoadGLcapabilities();
      (**(code **)((int)pvVar15 + 0x7c))();
      _glColor4us(spriteWorldP->stdTileRed,spriteWorldP->stdTileGreen,spriteWorldP->stdTileBlue,
                  spriteWorldP->stdTileAlpha);
      local_48 = local_44 * sVar2;
      for (; local_46 = ((short)uVar11 / sVar1) * sVar1, local_42 = (short)uVar11 / sVar1,
          local_44 <= sVar13; local_44 = local_44 + 1) {
        for (; local_42 <= sVar14; local_42 = local_42 + 1) {
          if ((useChangedMap == '\0') || (pTVar7->tileChangedFlags[local_44][local_42] == '\0')) {
            local_40 = local_28[local_44][local_42];
          }
          else {
            local_40 = pTVar7->tileMap[local_44][local_42];
          }
          if ((spriteWorldP->maxNumTiles <= local_40) &&
             (_printf("\n\nERROR: dstIndex = %d but maxNumTiles = %d \n",(int)local_40,
                      (int)spriteWorldP->maxNumTiles), spriteWorldP->maxNumTiles <= local_40)) {
            (**(code **)PTR__gSWAssertFailProc_001c4170)
                      ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c"
                       ,PTR____FUNCTION___22934_001c50a8,0x90d);
          }
          if (-1 < local_40) {
            if (spriteWorldP->masterTileSurface == (void *)0x0) {
              pFVar8 = spriteWorldP->tileFrameArray[spriteWorldP->curTileImage[local_40]];
              if (pFVar8 == (FrameRec *)0x0) {
                _printf("tileFrameP is NULL! tileID is %d and curTileImage for it is %d \n",
                        (int)local_40,(int)spriteWorldP->curTileImage[local_40]);
                _printf("maxNumTiles is %d \n",(int)spriteWorldP->maxNumTiles);
                (**(code **)PTR__gSWAssertFailProc_001c4170)
                          ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c"
                           ,PTR____FUNCTION___22934_001c50a8,0x91a);
              }
              local_2c = pFVar8->glSurfaceP;
            }
            else {
              local_2c = spriteWorldP->masterTileSurface;
            }
            sVar3 = spriteWorldP->tileOffsetV;
            sVar4 = spriteWorldP->tileOffsetH;
            (**(code **)((int)pvVar15 + 0x78))();
            (**(code **)((int)pvVar15 + 0x88))
                      (spriteWorldP->designScaleH,spriteWorldP->designScaleV,0x3f800000);
            (**(code **)((int)pvVar15 + 0x8c))
                      ((float)(int)(short)(local_46 - sVar4),(float)(int)(short)(local_48 - sVar3),0
                      );
            iVar16 = _DoesSurfaceNeedBlend(local_2c);
            if (iVar16 != 0) {
              (**(code **)((int)pvVar15 + 0x14))(0xbe2);
              (**(code **)((int)pvVar15 + 0x50))(0x302,0x303);
            }
            if (spriteWorldP->useStdTileColor == '\0') {
              if (*(char *)((int)spriteWorldP->tileColorArray + local_40 * 10) == '\0') {
                if (bVar12) {
                  _glColor4us(spriteWorldP->stdTileRed,spriteWorldP->stdTileGreen,
                              spriteWorldP->stdTileBlue,spriteWorldP->stdTileAlpha);
                  bVar12 = false;
                }
              }
              else {
                pvVar9 = spriteWorldP->tileColorArray;
                if ((*PTR__gIsDrawingTilesAboveSprite_001c5044 != '\0') &&
                   (*(short *)((int)pvVar9 + local_40 * 10 + 8) != -1)) goto LAB_000850b0;
                _glColor4us(*(undefined2 *)((int)pvVar9 + local_40 * 10 + 2),
                            *(undefined2 *)((int)pvVar9 + local_40 * 10 + 4),
                            *(undefined2 *)((int)pvVar9 + local_40 * 10 + 6),
                            *(undefined2 *)((int)pvVar9 + local_40 * 10 + 8));
                bVar12 = true;
              }
            }
            if (bVar17) {
              (**(code **)((int)pvVar15 + 0x88))(local_24,local_20,0x3f800000);
            }
            if (spriteWorldP->masterTileSurface == (void *)0x0) {
              _BKBlitGLSurface(local_2c);
            }
            else {
              _BKBlitTileFromMasterSurface
                        (local_2c,(int)spriteWorldP->curTileImage
                                       [spriteWorldP->tileSourceIndex[local_40]]);
            }
            *(int *)PTR__gNumTilesDrawnThisFrame_001c4750 =
                 *(int *)PTR__gNumTilesDrawnThisFrame_001c4750 + 1;
          }
LAB_000850b0:
          local_46 = local_46 + sVar1;
        }
        local_48 = local_48 + sVar2;
      }
      (**(code **)((int)pvVar15 + 0x80))();
      sVar1 = 1;
    }
    else {
      if (pTVar7->vbo == 0) {
        _SWComputeVBOForTileLayer(spriteWorldP,(int)tileLayer,0);
      }
      _SWDrawTileLayerUsingVBO(spriteWorldP,(int)tileLayer);
      sVar1 = 1;
    }
  }
  return sVar1;
}


// ---- _SWComputeVBOForTileLayer @ 0x85108 (2099 bytes)

void _SWComputeVBOForTileLayer(int param_1,short param_2,short *param_3)

{
  short sVar1;
  short sVar2;
  short *psVar3;
  int iVar4;
  short sVar5;
  short sVar6;
  short sVar7;
  int iVar8;
  undefined4 uVar9;
  void *pvVar10;
  void *pvVar11;
  short local_70;
  short local_6e;
  short local_6c;
  short local_6a;
  short local_66;
  short local_64;
  short local_62;
  short local_60;
  short local_5c;
  int local_58;
  int local_54;
  int local_34;
  int local_30;
  
  sVar1 = *(short *)(param_1 + 0xe2);
  sVar2 = *(short *)(param_1 + 0xe4);
  if ((param_1 == 0) || (*(char *)(param_1 + 200) == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___23264_001c506c,0x9d5);
  }
  if (*(int *)(*(int *)(param_1 + 0xc4) + param_2 * 4) != 0) {
    iVar8 = _glGetError();
    if (iVar8 != 0) {
      uVar9 = _BKGetGLErrorMessage(iVar8);
      _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___23264_001c506c,0x9e1,uVar9);
    }
    psVar3 = *(short **)(*(int *)(param_1 + 0xc4) + param_2 * 4);
    iVar8 = *(int *)(*(int *)(*(int *)(param_1 + 0xc4) + param_2 * 4) + 4);
    if (param_3 == (short *)0x0) {
      local_66 = 0;
      local_64 = 0;
      local_62 = *psVar3 + -1;
      local_60 = psVar3[1] + -1;
    }
    else {
      local_66 = *param_3 / sVar2;
      local_64 = param_3[1] / sVar1;
      local_62 = (short)((param_3[2] + -1) / (int)sVar2);
      local_60 = (short)((param_3[3] + -1) / (int)sVar1);
      if (*psVar3 <= local_62) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                   PTR____FUNCTION___23264_001c506c,0x9f6);
      }
      if (psVar3[1] <= local_60) {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                   PTR____FUNCTION___23264_001c506c,0x9f7);
      }
    }
    if (*(int *)(param_1 + 0x114) == 0) {
      psVar3[0x16] = 0;
      psVar3[0x17] = 0;
    }
    else {
      *(undefined4 *)(psVar3 + 0x16) = *(undefined4 *)(param_1 + 0x114);
    }
    if ((char)psVar3[0x18] == '\0') {
      local_58 = 0;
      for (local_6c = local_66; local_6c <= local_62; local_6c = local_6c + 1) {
        for (local_6a = local_64; local_6a <= local_60; local_6a = local_6a + 1) {
          sVar7 = *(short *)(*(int *)(local_6c * 4 + iVar8) + local_6a * 2);
          if ((-1 < sVar7) && (sVar7 < *(short *)(param_1 + 0xe0))) {
            local_58 = local_58 + 1;
          }
        }
      }
    }
    else {
      local_58 = (int)psVar3[1] * (int)*psVar3;
    }
    if (local_58 == 0) {
      psVar3[0x10] = 0;
      psVar3[0x11] = 0;
    }
    else {
      pvVar10 = _calloc(local_58 * 0xc,2);
      pvVar11 = _calloc(local_58 * 0xc,4);
      iVar4 = local_58 * 0x18;
      *(int *)(psVar3 + 0x12) = iVar4;
      if ((pvVar10 == (void *)0x0) || (pvVar11 == (void *)0x0)) {
        _printf(
               "\nSWComputeVBOForTileLayer failed to allocate system memory for vertex / texCoord array. Exited without creating anything. "
               );
        _printf("Attempted to allocate sizes of %d and %d. \n",iVar4,local_58 * 0x30);
      }
      else {
        _BKLoadGLfunctions();
        _BKLoadGLcapabilities();
        local_34 = 0;
        local_30 = 0;
        local_70 = local_66 * sVar2;
        local_54 = 0;
        for (local_6c = local_66; local_6c <= local_62; local_6c = local_6c + 1) {
          local_6e = local_64 * sVar1;
          for (local_6a = local_64; local_6a <= local_60; local_6a = local_6a + 1) {
            sVar7 = *(short *)(*(int *)(local_6c * 4 + iVar8) + local_6a * 2);
            if ((*(short *)(param_1 + 0xe0) <= sVar7) &&
               (_printf("\n\nERROR! Out of range tileID! layer %d, row %d, col %d, The ID is %d while the maxNumTiles is %d\n"
                        ,(int)param_2,(int)local_6c,(int)local_6a,(int)sVar7,
                        (int)*(short *)(param_1 + 0xe0)), *(short *)(param_1 + 0xe0) <= sVar7)) {
              (**(code **)PTR__gSWAssertFailProc_001c4170)
                        ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c"
                         ,PTR____FUNCTION___23264_001c506c,0xa4e);
            }
            if (sVar7 < 0) {
              if ((char)psVar3[0x18] != '\0') {
                for (local_5c = 0; local_5c < 0xc; local_5c = local_5c + 1) {
                  *(undefined2 *)(local_34 * 2 + (int)pvVar10) = 0x8001;
                  local_34 = local_34 + 1;
                  *(undefined4 *)(local_30 * 4 + (int)pvVar11) = 0;
                  local_30 = local_30 + 1;
                }
                local_54 = local_54 + 1;
              }
            }
            else {
              sVar7 = *(short *)(*(int *)(param_1 + 0xd8) + sVar7 * 2);
              sVar5 = sVar2 + local_70;
              sVar6 = sVar1 + local_6e;
              *(short *)(local_34 * 2 + (int)pvVar10) = local_6e;
              *(short *)((local_34 + 1) * 2 + (int)pvVar10) = local_70;
              *(short *)((local_34 + 2) * 2 + (int)pvVar10) = sVar6;
              *(short *)((local_34 + 3) * 2 + (int)pvVar10) = local_70;
              *(short *)((local_34 + 4) * 2 + (int)pvVar10) = local_6e;
              *(short *)((local_34 + 5) * 2 + (int)pvVar10) = sVar5;
              *(short *)((local_34 + 6) * 2 + (int)pvVar10) = sVar6;
              *(short *)((local_34 + 7) * 2 + (int)pvVar10) = local_70;
              *(short *)((local_34 + 8) * 2 + (int)pvVar10) = local_6e;
              *(short *)((local_34 + 9) * 2 + (int)pvVar10) = sVar5;
              *(short *)((local_34 + 10) * 2 + (int)pvVar10) = sVar6;
              *(short *)((local_34 + 0xb) * 2 + (int)pvVar10) = sVar5;
              local_34 = local_34 + 0xc;
              sVar7 = sVar7 * 0xc;
              for (local_5c = sVar7; (int)local_5c < sVar7 + 0xc; local_5c = local_5c + 1) {
                *(undefined4 *)(local_30 * 4 + (int)pvVar11) =
                     *(undefined4 *)(*(int *)(*(int *)(psVar3 + 0x16) + 0x60) + local_5c * 4);
                local_30 = local_30 + 1;
              }
              local_54 = local_54 + 1;
            }
            local_6e = local_6e + sVar1;
          }
          local_70 = local_70 + sVar2;
        }
        *(int *)(psVar3 + 0x14) = local_54;
        if (local_54 != local_58) {
          (**(code **)PTR__gSWAssertFailProc_001c4170)
                    ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                     PTR____FUNCTION___23264_001c506c,0xaa6);
        }
        if (*(int *)(psVar3 + 0x10) != 0) {
          _glDeleteBuffers(1,psVar3 + 0x10);
        }
        _glGenBuffers(1,psVar3 + 0x10);
        _glBindBuffer(0x8892,*(undefined4 *)(psVar3 + 0x10));
        _glBufferData(0x8892,local_58 * 0x48,0,0x88e4);
        _glBufferSubData(0x8892,0,iVar4,pvVar10);
        _glBufferSubData(0x8892,iVar4,local_58 * 0x30,pvVar11);
        iVar8 = _glGetError();
        if (iVar8 != 0) {
          uVar9 = _BKGetGLErrorMessage(iVar8);
          _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
                   "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                   PTR____FUNCTION___23264_001c506c,0xad0,uVar9);
        }
        _free(pvVar10);
        _free(pvVar11);
      }
    }
  }
  return;
}


// ---- _SWDrawTileLayerUsingVBO @ 0x8593b (986 bytes)

void _SWDrawTileLayerUsingVBO(int param_1,short param_2)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  
  if ((param_1 == 0) || (*(char *)(param_1 + 200) == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___23529_001c50e0,0xae6);
  }
  if ((*(int *)(*(int *)(param_1 + 0xc4) + param_2 * 4) != 0) &&
     (iVar4 = *(int *)(*(int *)(param_1 + 0xc4) + param_2 * 4), *(int *)(iVar4 + 0x20) != 0)) {
    iVar1 = *(int *)(iVar4 + 0x2c);
    if (*(int *)(iVar4 + 0x2c) == 0) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___23529_001c50e0,0xaf4);
    }
    if (*(int *)(iVar4 + 0x24) < 1) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___23529_001c50e0,0xaf5);
    }
    if (*(int *)(iVar4 + 0x28) < 1) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___23529_001c50e0,0xaf6);
    }
    iVar2 = _glGetError();
    if (iVar2 != 0) {
      uVar3 = _BKGetGLErrorMessage(iVar2);
      _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___23529_001c50e0,0xaf8,uVar3);
    }
    _glEnable(0xbe2);
    _glBlendFunc(0x302,0x303);
    _glColor4us(0xffff,0xffff,0xffff,0xffff);
    _glBindBuffer(0x8892,*(undefined4 *)(iVar4 + 0x20));
    _glEnable(*(undefined4 *)PTR__gSWTexExtension_001c4550);
    _glBindTexture(*(undefined4 *)(iVar1 + 0x30),**(undefined4 **)(iVar1 + 0x58));
    _glTexCoordPointer(2,0x1406,0,*(undefined4 *)(iVar4 + 0x24));
    _glVertexPointer(2,0x1402,0,0);
    _glEnableClientState(0x8074);
    _glEnableClientState(0x8078);
    _glMatrixMode(0x1700);
    _glPushMatrix();
    _glLoadIdentity();
    _glScalef(*(undefined4 *)(param_1 + 0x3c),*(undefined4 *)(param_1 + 0x40),0x3f800000);
    _glTranslatef((float)(int)*(short *)(param_1 + 0x8e),(float)(int)*(short *)(param_1 + 0x8c),0);
    if ((*(float *)(iVar4 + 8) != 1.0) || (NAN(*(float *)(iVar4 + 8)))) {
      _glScalef(*(undefined4 *)(iVar4 + 8),*(undefined4 *)(iVar4 + 8),0x3f800000);
    }
    _glDrawArrays(4,0,*(int *)(iVar4 + 0x28) * 6);
    _glDisableClientState(0x8074);
    _glDisableClientState(0x8078);
    iVar4 = _glGetError();
    if (iVar4 != 0) {
      uVar3 = _BKGetGLErrorMessage(iVar4);
      _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___23529_001c50e0,0xb42,uVar3);
    }
    _glPopMatrix();
  }
  return;
}


// ---- _SWResetAllChangedTilesMaps @ 0x85d15 (100 bytes)

void _SWResetAllChangedTilesMaps(int param_1)

{
  undefined2 local_e;
  
  if ((*(char *)(param_1 + 0xc9) != '\0') && (*(int *)(param_1 + 0xc4) != 0)) {
    for (local_e = 0; local_e <= *(short *)(param_1 + 0xca); local_e = local_e + 1) {
      _SWResetChangedTilesMap(*(undefined4 *)(*(int *)(param_1 + 0xc4) + local_e * 4));
    }
  }
  return;
}


// ---- _SWResetChangedTilesMap @ 0x85d79 (108 bytes)

void _SWResetChangedTilesMap(short *param_1)

{
  short local_10;
  short local_e;
  
  if (param_1 != (short *)0x0) {
    for (local_10 = 0; local_10 < *param_1; local_10 = local_10 + 1) {
      for (local_e = 0; local_e < param_1[1]; local_e = local_e + 1) {
        *(undefined1 *)(*(int *)(*(int *)(param_1 + 0xc) + local_10 * 4) + (int)local_e) = 0;
      }
    }
    *(undefined1 *)(param_1 + 0xe) = 0;
  }
  return;
}


// ---- _SWDrawTilesAboveSprite @ 0x85de5 (243 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWDrawTilesAboveSprite(SpriteWorldPtr spriteWorldP,SWRect *updateRectP,short startLayer)

{
  short local_e;
  
  local_e = startLayer;
  if (spriteWorldP->backgroundTileSpriteP != (SpriteRec *)0x0) {
    *PTR__gIsDrawingTilesAboveSprite_001c5044 = 1;
    for (; local_e <= spriteWorldP->lastActiveTileLayer; local_e = local_e + 1) {
      if (((spriteWorldP->tileLayerArray[local_e] != (TileMapStruct *)0x0) &&
          (((spriteWorldP->tileLayerArray[local_e]->isVisible == '\0' ||
            (spriteWorldP->useVBOForTiling == '\0')) ||
           (spriteWorldP->masterTileSurface == (void *)0x0)))) &&
         (spriteWorldP->tileLayerArray[local_e]->drawEveryFrame == '\0')) {
        _SWMarkTilesToBeRedrawn(spriteWorldP,(int)local_e,updateRectP);
      }
    }
    *PTR__gIsDrawingTilesAboveSprite_001c5044 = 0;
  }
  return;
}


// ---- _SWMarkTilesToBeRedrawn @ 0x85ed8 (462 bytes)

void _SWMarkTilesToBeRedrawn(int param_1,short param_2,short *param_3)

{
  short sVar1;
  short sVar2;
  short *psVar3;
  short sVar4;
  short sVar5;
  short local_18;
  short local_16;
  
  if ((param_1 == 0) || (*(char *)(param_1 + 200) == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___23690_001c50a4,0xc2b);
  }
  psVar3 = *(short **)(*(int *)(param_1 + 0xc4) + param_2 * 4);
  if (psVar3 != (short *)0x0) {
    local_18 = *param_3 / *(short *)(param_1 + 0xe4);
    sVar1 = param_3[1];
    sVar2 = *(short *)(param_1 + 0xe2);
    sVar4 = (short)((param_3[2] + -1) / (int)*(short *)(param_1 + 0xe4));
    sVar5 = (short)((param_3[3] + -1) / (int)*(short *)(param_1 + 0xe2));
    if (*psVar3 <= sVar4) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___23690_001c50a4,0xc3b);
    }
    if (psVar3[1] <= sVar5) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                 PTR____FUNCTION___23690_001c50a4,0xc3c);
    }
    for (; local_16 = sVar1 / sVar2, local_18 <= sVar4; local_18 = local_18 + 1) {
      for (; local_16 <= sVar5; local_16 = local_16 + 1) {
        *(undefined1 *)(*(int *)(*(int *)(psVar3 + 0xc) + local_18 * 4) + (int)local_16) = 1;
      }
    }
    *(undefined1 *)(psVar3 + 0xe) = 1;
  }
  return;
}


// ---- _SWAddChangedRect @ 0x860a6 (957 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void SWAddChangedRect(SpriteWorldPtr spriteWorldP,SWRect *changedRectP)

{
  SWRect *pSVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sStack_3a;
  short sStack_36;
  short sStack_32;
  short sStack_2e;
  short sStack_22;
  SWRect *pSStack_20;
  
  if ((spriteWorldP == (SpriteWorldPtr)0x0) || (spriteWorldP->tilingIsInitialized == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___23760_001c5070,0xc55);
  }
  if ((((changedRectP->top < 0) || (changedRectP->left < 0)) ||
      (changedRectP->right <= changedRectP->left)) || (changedRectP->bottom <= changedRectP->top)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___23760_001c5070,0xc57);
  }
  pSStack_20 = spriteWorldP->changedTiles;
  for (sStack_22 = 0; (int)sStack_22 < spriteWorldP->numTilesChanged; sStack_22 = sStack_22 + 1) {
    if (((pSStack_20->left <= changedRectP->left) && (pSStack_20->top <= changedRectP->top)) &&
       ((changedRectP->right <= pSStack_20->right && (changedRectP->bottom <= pSStack_20->bottom))))
    {
      return;
    }
    pSStack_20 = pSStack_20 + 1;
  }
  pSStack_20 = spriteWorldP->changedTiles;
  sStack_22 = 0;
  while( true ) {
    if (spriteWorldP->numTilesChanged <= (int)sStack_22) {
      if (spriteWorldP->numTilesChanged < *(int *)&spriteWorldP->field_0xf4) {
        sVar3 = changedRectP->left;
        sVar4 = changedRectP->bottom;
        sVar5 = changedRectP->right;
        iVar2 = spriteWorldP->numTilesChanged;
        pSVar1 = spriteWorldP->changedTiles + iVar2;
        pSVar1->top = changedRectP->top;
        pSVar1->left = sVar3;
        pSVar1->bottom = sVar4;
        pSVar1->right = sVar5;
        spriteWorldP->numTilesChanged = iVar2 + 1;
      }
      else {
        (**(code **)PTR__gSWAssertFailProc_001c4170)
                  ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
                   PTR____FUNCTION___23760_001c5070,0xc8f);
        _SWFlagRectAsChanged(spriteWorldP,changedRectP);
      }
      return;
    }
    if (((pSStack_20->top == changedRectP->top) && (pSStack_20->bottom == changedRectP->bottom)) &&
       (((changedRectP->left <= pSStack_20->left && (pSStack_20->left <= changedRectP->right)) ||
        ((changedRectP->left <= pSStack_20->right && (pSStack_20->right <= changedRectP->right))))))
    break;
    if (((pSStack_20->left == changedRectP->left) && (pSStack_20->right == changedRectP->right)) &&
       (((changedRectP->top <= pSStack_20->top && (pSStack_20->top <= changedRectP->bottom)) ||
        ((changedRectP->top <= pSStack_20->bottom && (pSStack_20->bottom <= changedRectP->bottom))))
       )) {
      sStack_32 = changedRectP->top;
      if (pSStack_20->top < changedRectP->top) {
        sStack_32 = pSStack_20->top;
      }
      pSStack_20->top = sStack_32;
      sStack_2e = changedRectP->bottom;
      if (changedRectP->bottom < pSStack_20->bottom) {
        sStack_2e = pSStack_20->bottom;
      }
      pSStack_20->bottom = sStack_2e;
      return;
    }
    sStack_22 = sStack_22 + 1;
    pSStack_20 = pSStack_20 + 1;
  }
  sStack_3a = changedRectP->left;
  if (pSStack_20->left < changedRectP->left) {
    sStack_3a = pSStack_20->left;
  }
  pSStack_20->left = sStack_3a;
  sStack_36 = changedRectP->right;
  if (changedRectP->right < pSStack_20->right) {
    sStack_36 = pSStack_20->right;
  }
  pSStack_20->right = sStack_36;
  return;
}


// ---- _SWChangeTileImage @ 0x86463 (294 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWChangeTileImage(SpriteWorldPtr spriteWorldP,short tileID,short currentFrame)

{
  if ((spriteWorldP == (SpriteWorldPtr)0x0) || (spriteWorldP->tilingIsInitialized == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___23877_001c5084,0xc9e);
  }
  if (spriteWorldP->maxNumTiles <= tileID) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___23877_001c5084,0xc9f);
  }
  if (spriteWorldP->maxNumTiles <= currentFrame) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___23877_001c5084,0xca0);
  }
  spriteWorldP->curTileImage[tileID] = currentFrame;
  if (((spriteWorldP->useVBOForTiling != '\0') && (spriteWorldP->masterTileSurface != (void *)0x0))
     || (spriteWorldP->backgroundTileSpriteP != (SpriteRec *)0x0)) {
    _SWUpdateTileOnScreen(spriteWorldP,tileID);
  }
  return;
}


// ---- _SWUpdateTileOnScreen @ 0x86589 (509 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWUpdateTileOnScreen(SpriteWorldPtr spriteWorldP,short tileID)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  short sVar7;
  short local_1a;
  short local_18;
  short local_16;
  
  if ((spriteWorldP == (SpriteWorldPtr)0x0) || (spriteWorldP->tilingIsInitialized == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___23915_001c50e8,0xcb6);
  }
  sVar7 = (spriteWorldP->visScrollRect).top / spriteWorldP->tileHeight;
  sVar1 = (spriteWorldP->visScrollRect).left;
  sVar2 = spriteWorldP->tileWidth;
  sVar3 = (spriteWorldP->visScrollRect).bottom;
  sVar4 = spriteWorldP->tileHeight;
  sVar5 = (spriteWorldP->visScrollRect).right;
  sVar6 = spriteWorldP->tileWidth;
  for (local_16 = 0; local_16 <= spriteWorldP->lastActiveTileLayer; local_16 = local_16 + 1) {
    if ((spriteWorldP->tileLayerArray[local_16] != (TileMapStruct *)0x0) &&
       ((local_1a = sVar7, spriteWorldP->tileLayerArray[local_16]->vbo != 0 ||
        (spriteWorldP->backgroundTileSpriteP != (SpriteRec *)0x0)))) {
      for (; local_18 = sVar1 / sVar2, local_1a <= (short)((sVar3 + -1) / (int)sVar4);
          local_1a = local_1a + 1) {
        for (; local_18 <= (short)((sVar5 + -1) / (int)sVar6); local_18 = local_18 + 1) {
          if (spriteWorldP->tileLayerArray[local_16]->tileMap[local_1a][local_18] == tileID) {
            _SWDrawTile(spriteWorldP,local_16,local_1a,local_18,tileID);
          }
        }
      }
    }
  }
  return;
}


// ---- _SWUpdateTilesOnScreenFast @ 0x86786 (538 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWUpdateTilesOnScreenFast(SpriteWorldPtr spriteWorldP,short firstTileID,short lastTileID)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  short tileID;
  short sVar7;
  short local_1c;
  short local_1a;
  short local_18;
  
  if ((spriteWorldP == (SpriteWorldPtr)0x0) || (spriteWorldP->tilingIsInitialized == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___24008_001c5098,0xce3);
  }
  sVar7 = (spriteWorldP->visScrollRect).top / spriteWorldP->tileHeight;
  sVar1 = (spriteWorldP->visScrollRect).left;
  sVar2 = spriteWorldP->tileWidth;
  sVar3 = (spriteWorldP->visScrollRect).bottom;
  sVar4 = spriteWorldP->tileHeight;
  sVar5 = (spriteWorldP->visScrollRect).right;
  sVar6 = spriteWorldP->tileWidth;
  for (local_18 = 0; local_18 <= spriteWorldP->lastActiveTileLayer; local_18 = local_18 + 1) {
    if ((spriteWorldP->tileLayerArray[local_18] != (TileMapStruct *)0x0) &&
       ((local_1c = sVar7, spriteWorldP->tileLayerArray[local_18]->vbo != 0 ||
        (spriteWorldP->backgroundTileSpriteP != (SpriteRec *)0x0)))) {
      for (; local_1a = sVar1 / sVar2, local_1c <= (short)((sVar3 + -1) / (int)sVar4);
          local_1c = local_1c + 1) {
        for (; local_1a <= (short)((sVar5 + -1) / (int)sVar6); local_1a = local_1a + 1) {
          tileID = spriteWorldP->tileLayerArray[local_18]->tileMap[local_1c][local_1a];
          if ((firstTileID <= tileID) && (tileID <= lastTileID)) {
            _SWDrawTile(spriteWorldP,local_18,local_1c,local_1a,tileID);
          }
        }
      }
    }
  }
  return;
}


// ---- _SWResetCurrentTileImages @ 0x869a0 (154 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _SWResetCurrentTileImages(SpriteWorldPtr spriteWorldP)

{
  short local_e;
  
  if ((spriteWorldP == (SpriteWorldPtr)0x0) || (spriteWorldP->tilingIsInitialized == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___24093_001c5060,0xd0b);
  }
  if (spriteWorldP->tilingIsInitialized != '\0') {
    for (local_e = 0; local_e < spriteWorldP->maxNumTiles; local_e = local_e + 1) {
      spriteWorldP->curTileImage[local_e] = local_e;
    }
  }
  return;
}


// ---- _SWReturnTileUnderPixel @ 0x86a3a (387 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

short SWReturnTileUnderPixel
                (SpriteWorldPtr spriteWorldP,short tileLayer,short pixelCol,short pixelRow)

{
  short sVar1;
  short sVar2;
  
  if ((spriteWorldP == (SpriteWorldPtr)0x0) || (spriteWorldP->tilingIsInitialized == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___24119_001c50cc,0xd21);
  }
  if (9 < tileLayer) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___24119_001c50cc,0xd22);
  }
  if (spriteWorldP->tileLayerArray[tileLayer] == (TileMapStruct *)0x0) {
    sVar1 = -1;
  }
  else {
    sVar1 = pixelRow / spriteWorldP->tileHeight;
    sVar2 = pixelCol / spriteWorldP->tileWidth;
    if ((((sVar1 < 0) || (spriteWorldP->tileLayerArray[tileLayer]->numRows <= sVar1)) || (sVar2 < 0)
        ) || (spriteWorldP->tileLayerArray[tileLayer]->numCols <= sVar2)) {
      sVar1 = -1;
    }
    else {
      sVar1 = spriteWorldP->tileLayerArray[tileLayer]->tileMap[sVar1][sVar2];
    }
  }
  return sVar1;
}


// ---- _SWCheckSpriteWithTiles @ 0x86bbd (3077 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SWBoolean _SWCheckSpriteWithTiles
                    (SpriteWorldPtr spriteWorldP,SpritePtr srcSpriteP,void *searchType,
                    SWRect *insetRectP,short startTileLayer,short endTileLayer,short firstTileID,
                    short lastTileID,SWBoolean fixPosition)

{
  short sVar1;
  bool bVar2;
  bool bVar3;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  short local_3c;
  short local_3a;
  short local_38;
  short local_36;
  short local_34;
  short local_32;
  SWBoolean local_29;
  short local_22;
  short local_20;
  short local_1e;
  
  local_29 = '\0';
  if ((spriteWorldP == (SpriteWorldPtr)0x0) || (spriteWorldP->tilingIsInitialized == '\0')) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___24210_001c50b0,0xd4c);
  }
  if ((startTileLayer < 0) || (9 < endTileLayer)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___24210_001c50b0,0xd4d);
  }
  if (srcSpriteP == (SpritePtr)0x0) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/Sources/Tiling.c",
               PTR____FUNCTION___24210_001c50b0,0xd4e);
  }
  local_3c = -1;
  local_3a = -1;
  local_40._0_2_ = (srcSpriteP->oldFrameRect).bottom;
  local_40._2_2_ = (srcSpriteP->oldFrameRect).right;
  local_44._0_2_ = (srcSpriteP->oldFrameRect).top;
  local_44._2_2_ = (srcSpriteP->oldFrameRect).left;
  local_48._0_2_ = (srcSpriteP->destFrameRect).bottom;
  local_48._2_2_ = (srcSpriteP->destFrameRect).right;
  local_4c._0_2_ = (srcSpriteP->destFrameRect).top;
  local_4c._2_2_ = (srcSpriteP->destFrameRect).left;
  if (insetRectP != (SWRect *)0x0) {
    local_44 = CONCAT22(local_44._2_2_ + insetRectP->left,(short)local_44 + insetRectP->top);
    local_40 = CONCAT22(local_40._2_2_ - insetRectP->right,(short)local_40 - insetRectP->bottom);
    local_4c = CONCAT22(local_4c._2_2_ + insetRectP->left,(short)local_4c + insetRectP->top);
    local_48 = CONCAT22(local_48._2_2_ - insetRectP->right,(short)local_48 - insetRectP->bottom);
  }
  if ((short)local_44 < 0) {
    local_44 = CONCAT22(local_44._2_2_,(short)local_44 - spriteWorldP->tileHeight);
  }
  if ((short)local_40 < 1) {
    local_40 = CONCAT22(local_40._2_2_,(short)local_40 - spriteWorldP->tileHeight);
  }
  if (local_44._2_2_ < 1) {
    local_44 = CONCAT22(local_44._2_2_ - spriteWorldP->tileWidth,(short)local_44);
  }
  if (local_40._2_2_ < 1) {
    local_40 = CONCAT22(local_40._2_2_ - spriteWorldP->tileWidth,(short)local_40);
  }
  if (local_4c < 0) {
    local_4c = CONCAT22(local_4c._2_2_ - spriteWorldP->tileWidth,(short)local_4c);
  }
  if (local_48._2_2_ < 1) {
    local_48 = CONCAT22(local_48._2_2_ - spriteWorldP->tileWidth,(short)local_48);
  }
  if ((short)local_4c < 0) {
    local_4c = CONCAT22(local_4c._2_2_,(short)local_4c - spriteWorldP->tileHeight);
  }
  if ((short)local_48 < 1) {
    local_48 = CONCAT22(local_48._2_2_,(short)local_48 - spriteWorldP->tileHeight);
  }
  if (searchType == (void *)0x1) {
    local_38 = (short)local_44 / spriteWorldP->tileHeight;
    local_36 = (short)local_4c / spriteWorldP->tileHeight;
    local_34 = local_4c._2_2_ / spriteWorldP->tileWidth;
    local_32 = (short)((local_48._2_2_ + -1) / (int)spriteWorldP->tileWidth);
    if (fixPosition != '\0') {
      local_38 = local_38 + -1;
    }
    if (local_38 < local_36) {
      return '\0';
    }
  }
  else if (searchType == (void *)0x3) {
    local_38 = (short)(((short)local_40 + -1) / (int)spriteWorldP->tileHeight);
    local_36 = (short)(((short)local_48 + -1) / (int)spriteWorldP->tileHeight);
    local_34 = local_4c._2_2_ / spriteWorldP->tileWidth;
    local_32 = (short)((local_48._2_2_ + -1) / (int)spriteWorldP->tileWidth);
    if (fixPosition != '\0') {
      local_38 = local_38 + 1;
    }
    if (local_36 < local_38) {
      return '\0';
    }
  }
  else if (searchType == (void *)0x2) {
    local_34 = (short)((local_40._2_2_ + -1) / (int)spriteWorldP->tileWidth);
    local_32 = (short)((local_48._2_2_ + -1) / (int)spriteWorldP->tileWidth);
    local_38 = (short)local_4c / spriteWorldP->tileHeight;
    local_36 = (short)(((short)local_48 + -1) / (int)spriteWorldP->tileHeight);
    if (fixPosition != '\0') {
      local_34 = local_34 + 1;
    }
    if (local_32 < local_34) {
      return '\0';
    }
  }
  else if (searchType == (void *)0x4) {
    local_34 = local_44._2_2_ / spriteWorldP->tileWidth;
    local_32 = local_4c._2_2_ / spriteWorldP->tileWidth;
    local_38 = (short)local_4c / spriteWorldP->tileHeight;
    local_36 = (short)(((short)local_48 + -1) / (int)spriteWorldP->tileHeight);
    if (fixPosition != '\0') {
      local_34 = local_34 + -1;
    }
    if (local_34 < local_32) {
      return '\0';
    }
  }
  else {
    local_38 = (short)local_4c / spriteWorldP->tileHeight;
    local_36 = (short)(((short)local_48 + -1) / (int)spriteWorldP->tileHeight);
    local_34 = local_4c._2_2_ / spriteWorldP->tileWidth;
    local_32 = (short)((local_48._2_2_ + -1) / (int)spriteWorldP->tileWidth);
  }
  if (local_36 < local_38) {
    local_20 = -1;
  }
  else {
    local_20 = 1;
  }
  local_22 = startTileLayer;
  if (local_32 < local_34) {
    local_1e = -1;
  }
  else {
    local_1e = 1;
  }
  for (; (local_22 <= endTileLayer &&
         (spriteWorldP->tileLayerArray[local_22] == (TileMapStruct *)0x0)); local_22 = local_22 + 1)
  {
  }
  if (local_20 < 1) {
    if (local_38 < 0) {
      return '\0';
    }
    if (spriteWorldP->tileLayerArray[local_22]->numRows <= local_38) {
      local_38 = spriteWorldP->tileLayerArray[local_22]->numRows + -1;
    }
    if (local_36 < 0) {
      local_36 = 0;
    }
    else if (spriteWorldP->tileLayerArray[local_22]->numRows <= local_36) {
      return '\0';
    }
  }
  else {
    if (local_36 < 0) {
      return '\0';
    }
    if (spriteWorldP->tileLayerArray[local_22]->numRows <= local_36) {
      local_36 = spriteWorldP->tileLayerArray[local_22]->numRows + -1;
    }
    if (local_38 < 0) {
      local_38 = 0;
    }
    else if (spriteWorldP->tileLayerArray[local_22]->numRows <= local_38) {
      return '\0';
    }
  }
  if (local_1e < 1) {
    if (local_34 < 0) {
      return '\0';
    }
    if (spriteWorldP->tileLayerArray[local_22]->numCols <= local_34) {
      local_34 = spriteWorldP->tileLayerArray[local_22]->numCols + -1;
    }
    if (local_32 < 0) {
      local_32 = 0;
    }
    else if (spriteWorldP->tileLayerArray[local_22]->numCols <= local_32) {
      return '\0';
    }
  }
  else {
    if (local_32 < 0) {
      return '\0';
    }
    if (spriteWorldP->tileLayerArray[local_22]->numCols <= local_32) {
      local_32 = spriteWorldP->tileLayerArray[local_22]->numCols + -1;
    }
    if (local_34 < 0) {
      local_34 = 0;
    }
    else if (spriteWorldP->tileLayerArray[local_22]->numCols <= local_34) {
      return '\0';
    }
  }
  local_22 = startTileLayer;
  if (((searchType == (void *)0x1) || (searchType == (void *)0x3)) || (searchType == (void *)0x0)) {
    for (; local_22 <= endTileLayer; local_22 = local_22 + 1) {
      if (spriteWorldP->tileLayerArray[local_22] != (TileMapStruct *)0x0) {
        bVar2 = true;
        local_3c = local_38;
        while (bVar2) {
          bVar2 = local_3c != local_36;
          bVar3 = true;
          local_3a = local_34;
          while (bVar3) {
            bVar3 = local_3a != local_32;
            sVar1 = spriteWorldP->tileLayerArray[local_22]->tileMap[local_3c][local_3a];
            if ((firstTileID <= sVar1) && (sVar1 <= lastTileID)) {
              local_29 = '\x01';
              goto LAB_00087658;
            }
            local_3a = local_3a + local_1e;
          }
          local_3c = local_3c + local_20;
        }
      }
    }
  }
  else {
    for (; local_22 <= endTileLayer; local_22 = local_22 + 1) {
      if (spriteWorldP->tileLayerArray[local_22] != (TileMapStruct *)0x0) {
        bVar2 = true;
        local_3a = local_34;
        while (bVar2) {
          bVar2 = local_3a != local_32;
          bVar3 = true;
          local_3c = local_38;
          while (bVar3) {
            bVar3 = local_3c != local_36;
            sVar1 = spriteWorldP->tileLayerArray[local_22]->tileMap[local_3c][local_3a];
            if ((firstTileID <= sVar1) && (sVar1 <= lastTileID)) {
              local_29 = '\x01';
              goto LAB_00087658;
            }
            local_3c = local_3c + local_20;
          }
          local_3a = local_3a + local_1e;
        }
      }
    }
  }
LAB_00087658:
  if ((local_29 != '\0') && (fixPosition != '\0')) {
    if (searchType == (void *)0x1) {
      _SWOffsetSprite(srcSpriteP,0.0,
                      (float)(int)(short)(spriteWorldP->tileHeight * (local_3c + 1) -
                                         (short)local_4c));
    }
    else if (searchType == (void *)0x3) {
      _SWOffsetSprite(srcSpriteP,0.0,
                      (float)-(int)(short)((short)local_48 - spriteWorldP->tileHeight * local_3c));
    }
    else if (searchType == (void *)0x2) {
      _SWOffsetSprite(srcSpriteP,
                      (float)-(int)(short)(local_48._2_2_ - spriteWorldP->tileWidth * local_3a),0.0)
      ;
    }
    else if (searchType == (void *)0x4) {
      _SWOffsetSprite(srcSpriteP,
                      (float)(int)(short)(spriteWorldP->tileWidth * (local_3a + 1) - local_4c._2_2_)
                      ,0.0);
    }
  }
  return local_29;
}


