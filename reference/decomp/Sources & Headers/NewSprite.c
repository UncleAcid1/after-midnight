// Ghidra decompilation of functions from original source file:
//   Sources & Headers/NewSprite.c

// ---- _LoadSprites @ 0x33ac (237 bytes)

void _LoadSprites(void)

{
  undefined4 uVar1;
  short local_e;
  
  _InitSpriteArray();
  if (*PTR__gMainSpritesAreLoaded_001c4178 == '\0') {
    _LoadStatsSprites();
  }
  uVar1 = *(undefined4 *)PTR__gCurrentGameMode_001c42d0;
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = 0xf;
  _puts("\nLoading non-pixel-mask sprites... ");
  _LoadNonPixelMaskSprites();
  _puts("\nLoading pixel mask sprites...");
  _LoadPixelMaskPICTSprites();
  *(undefined4 *)PTR__gCurrentGameMode_001c42d0 = uVar1;
  _WriteCStringToFile("Loading Completed. About to lock sprites...");
  _WriteCarriageReturn(2);
  for (local_e = 0; local_e < 0xe3; local_e = local_e + 1) {
    _SWLockSprite(*(SpritePtr *)(PTR__gSpriteArray_001c40d4 + local_e * 4));
  }
  *PTR__gMainSpritesAreLoaded_001c4178 = 1;
  _WriteCStringToFile("LoadSprites completed successfully.\r");
  return;
}


// ---- _InitSpriteArray @ 0x3499 (127 bytes)

void _InitSpriteArray(void)

{
  short local_e;
  
  if (*PTR__gSpriteArrayWasInited_001c4224 == '\0') {
    for (local_e = 0; local_e < 0xe3; local_e = local_e + 1) {
      *(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_e * 4) = 0;
    }
    for (local_e = 0; local_e < 0xe; local_e = local_e + 1) {
      *(undefined4 *)(PTR__gTitleScreenSpriteArray_001c4160 + local_e * 4) = 0;
    }
    *PTR__gSpriteArrayWasInited_001c4224 = 1;
  }
  return;
}


// ---- _LoadPixelMaskPICTSprites @ 0x3518 (15526 bytes)

void _LoadPixelMaskPICTSprites(void)

{
  int iVar1;
  undefined4 uVar2;
  short local_10;
  short local_e;
  
  _WriteCStringToFile("LoadPixelMaskPICTSprites entered...\r");
  iVar1 = _MyLoadSpriteFromSinglePictXY
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0,0,0x80,0x80,0x80,0x80,0,0,0,0x54,4
                    );
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0xaa);
  }
  _MySetSpriteScaledSize(*(undefined4 *)PTR__gSpriteArray_001c40d4,0x40,0x40);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePictXY
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xc2,0,0x82,0x82,0x80,0x80,0,0,0,2,4
                    );
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0xb9);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x308),0x3f000000,0x3f000000);
  _KeepMusicPlaying();
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xc3,0,0x83,0x83,0x70,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0xc0);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x30c),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xc4,0,0x87,0x87,0x50,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0xc5);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x310),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,1,0,0x96,0x96,0x40,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0xcb);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 4),0x40,0x20);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,2,0,0x9b,0x9b,0x40,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0xd2);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 8),0x20,0x20);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,3,0,0xa0,0xa0,0x40,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0xd9);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0xc),0x20,0x20);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,4,0,0xa5,0xa5,0x54,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0xe0);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x10),0x2a,0x2a);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,5,0,0xa6,0xa6,0x54,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0xe7);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x14),0x2a,0x2a);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,6,0,0xaa,0xaa,0x40,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0xee);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x18),0x20,0x20);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,7,0,0xab,0xab,0x40,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0xf5);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x1c),0x20,0x20);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,8,0,0xae,0xae,0x54,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0xfc);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x20),0x34,0x34);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,9,0,0xaf,0xaf,0x54,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x103);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x24),0x34,0x34);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,10,0,0xb4,0xb4,0x40,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x10a);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x28),0x20,0x20);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xb,0,0xb5,0xb5,0x80,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x111);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x2c),0x40,0x40);
  _IncreaseProgressBar(1);
  _KeepMusicPlaying();
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xc,0,0xb9,0xb9,0x38,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x11a);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x30),0x28,0x28);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePictXY
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xd,0,0xbc,0xbc,0x60,0x60,0,0,0,0xe,
                     4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,299);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x34),0x40,0x40);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xbb,0,0x38e,0x38e,0x60,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x132);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x2ec),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x1a,0,0xbe,0xbe,0x20,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x137);
  }
  _MySetSpriteScaledFactor(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x68),0x3f000000,0x3f000000)
  ;
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x1b,0,0xbf,0xbf,0x20,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x13c);
  }
  _MySetSpriteScaledFactor(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x6c),0x3f000000,0x3f000000)
  ;
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x1c,0,0xc0,0xc0,0x20,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x141);
  }
  _MySetSpriteScaledFactor(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x70),0x3f000000,0x3f000000)
  ;
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x1d,0,0xc1,0xc1,0x20,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x146);
  }
  _MySetSpriteScaledFactor(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x74),0x3f000000,0x3f000000)
  ;
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePictXY
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xb9,0,0xc3,0xc3,0x20,0x20,0,0,0,
                     0x40,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x155);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x2e4),0x10,0x10);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePictXY
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xba,0,0xc4,0xc4,0x20,0x20,0,0,0,
                     0x40,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x164);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x2e8),0x10,0x10);
  _IncreaseProgressBar(1);
  _KeepMusicPlaying();
  iVar1 = _MyLoadSpriteFromPictResource
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x1e,0,800,800,1,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x16c);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x78),0x20,0x20);
  iVar1 = _MyLoadSpriteFromPictResource
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x1f,0,0x321,0x321,1,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x171);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x7c),0x20,0x20);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x20,0,0x322,0x322,0x20,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x176);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x80),0x10,0x10);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x21,0,0x323,0x323,0x20,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x17d);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x84),0x10,0x10);
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x22,0,0x32a,0x32a,0x40,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x184);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x88),0x20,0x20);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x23,0,0x32f,0x32f,0x46,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x189);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x8c),0x28,0x30);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x24,0,0x334,0x334,0x40,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x18e);
  }
  _MySetSpriteScaledFactor(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x90),0x3f000000,0x3f000000)
  ;
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x25,0,0x339,0x339,0x60,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x193);
  }
  _MySetSpriteScaledFactor(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x94),0x3f000000,0x3f000000)
  ;
  iVar1 = _MyLoadSpriteFromPictResource
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x26,0,0x33e,0x33e,1,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x198);
  }
  _MySetSpriteScaledFactor(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x98),0x3f000000,0x3f000000)
  ;
  _IncreaseProgressBar(1);
  local_e = 0x4b0;
  for (local_10 = 0x27; local_10 < 0x2d; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromSinglePict
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,0x20,0,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x1a2);
    }
    _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x10,0x20);
    _IncreaseProgressBar(1);
    local_e = local_e + 1;
  }
  local_e = 700;
  for (local_10 = 0x45; local_10 < 0x50; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromPictResource
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,1,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x1e2);
    }
    _MySetSpriteScaledFactor
              (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x3f000000,0x3f000000);
    local_e = local_e + 1;
  }
  _IncreaseProgressBar(1);
  local_e = 0x2ea;
  for (local_10 = 0x50; local_10 < 0x5c; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromPictResource
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,1,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x1ed);
    }
    _MySetSpriteScaledFactor
              (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x3f000000,0x3f000000);
    local_e = local_e + 1;
  }
  _IncreaseProgressBar(1);
  local_e = 0x2f8;
  for (local_10 = 0x5c; local_10 < 0x5f; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromSinglePict
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,0x30,0,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x1f7);
    }
    _MySetSpriteScaledFactor
              (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x3f000000,0x3f000000);
    local_e = local_e + 1;
  }
  _IncreaseProgressBar(1);
  _KeepMusicPlaying();
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x5f,0,0x302,0x302,0x87,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x200);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x17c),0x5a,0x55);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x60,0,0x303,0x303,0x87,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x205);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x180),0x5a,0x55);
  _IncreaseProgressBar(1);
  local_e = 0x348;
  for (local_10 = 0x61; local_10 < 0x65; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromPictResource
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,1,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x20e);
    }
    _MySetSpriteScaledFactor
              (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x3f000000,0x3f000000);
    local_e = local_e + 1;
  }
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x65,0,0x34c,0x34c,0x80,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x216);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x194),0x3f000000,0x3f000000);
  local_e = 0x352;
  for (local_10 = 0x66; local_10 < 0x70; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromPictResource
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,1,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x21d);
    }
    _MySetSpriteScaledFactor
              (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x3f000000,0x3f000000);
    local_e = local_e + 1;
  }
  local_e = 0x370;
  for (local_10 = 0x70; local_10 < 0x74; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromSinglePict
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,0x80,0,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x225);
    }
    _MySetSpriteScaledFactor
              (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x3f000000,0x3f000000);
    local_e = local_e + 1;
  }
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x74,0,0x37a,0x37a,0xc0,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x22b);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x1d0),0x3f000000,0x3f000000);
  local_e = 0x44c;
  for (local_10 = 0x85; local_10 < 0x9e; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromPictResource
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,1,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x233);
    }
    _MySetSpriteScaledFactor
              (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x3f000000,0x3f000000);
    local_e = local_e + 1;
  }
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x9e,0,0x3ac,0x3ac,0x30,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x23a);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x278),0x3f000000,0x3f000000);
  _KeepMusicPlaying();
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x9f,0,0x3b1,0x3b1,0x40,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x240);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x27c),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xa0,0,0x3b2,0x3b2,0x40,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x245);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x280),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xc1,0,0x3b6,0x3b6,0x40,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x24a);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x304),0x3f000000,0x3f000000);
  _IncreaseProgressBar(1);
  local_e = 0x3c0;
  for (local_10 = 0xa1; local_10 < 0xa4; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromSinglePict
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,0x20,0,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x253);
    }
    _MySetSpriteScaledFactor
              (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x3f000000,0x3f000000);
    local_e = local_e + 1;
  }
  local_e = 0x3ca;
  for (local_10 = 0xa4; local_10 < 0xa7; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromSinglePict
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,0x10,0,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x25b);
    }
    _MySetSpriteScaledFactor
              (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x3f000000,0x3f000000);
    local_e = local_e + 1;
  }
  local_e = 0x3d3;
  for (local_10 = 0xa7; local_10 < 0xb1; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromPictResource
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,1,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x263);
    }
    _MySetSpriteScaledFactor
              (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x3f000000,0x3f000000);
    local_e = local_e + 1;
  }
  local_e = 1000;
  for (local_10 = 0xe; local_10 < 0x12; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromSinglePict
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,0x60,0,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x26b);
    }
    _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x40,0x40);
    local_e = local_e + 1;
  }
  _SWSetTransparentColor(PTR__gSDLTealColor_001c41a0);
  local_e = 0x3f2;
  for (local_10 = 0x12; local_10 < 0x1a; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromSinglePict
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,0x50,0,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x275);
    }
    _MySetSpriteScaledFactor
              (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x3f000000,0x3f000000);
    local_e = local_e + 1;
  }
  local_e = 0x514;
  for (local_10 = 0x81; local_10 < 0x85; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromPictResource
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,1,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x27d);
    }
    _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x40,0x80);
    local_e = local_e + 1;
  }
  _SWSetTransparentColor(PTR__gSDLWhiteColor_001c42a8);
  _IncreaseProgressBar(1);
  _KeepMusicPlaying();
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xbc,0,900,900,0x28,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x287);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x2f0),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xbd,0,0x385,0x385,0x50,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x28c);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x2f4),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xbe,0,0x389,0x389,0x28,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x291);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x2f8),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xbf,0,0x38a,0x38a,0x50,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x296);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x2fc),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xc0,0,0x398,0x398,0x50,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x29b);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x300),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromSinglePictXY
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xc5,0,0x5dc,0x5dc,0x80,0x20,0,0,0,3
                     ,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x2b7);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x314),0x40,0x10);
  iVar1 = _MyLoadSpriteFromSinglePictXY
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xc6,0,0x5dd,0x5dd,0x80,0x20,0,0,0,3
                     ,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x2c8);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x318),0x40,0x10);
  local_e = 2000;
  for (local_10 = 0xb1; local_10 < 0xb7; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromPictResource
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,1,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x2cf);
    }
    if (local_10 == 0xb4) {
      _MySetSpriteScaledFactor
                (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x2d0),0x3f2b020c,0x3f2b020c);
    }
    else {
      _MySetSpriteScaledFactor
                (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x3f000000,0x3f000000);
    }
    local_e = local_e + 1;
  }
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,199,0,0x7da,0x7da,0x10,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x2dc);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x31c),0x3f000000,0x3f000000);
  _KeepMusicPlaying();
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xce,0,0x3a2,0x3a2,0x48,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x2e2);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x338),0x30,0x18);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xcf,0,0x3a3,0x3a3,0x48,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x2e7);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x33c),0x30,0x18);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xd0,0,0x3a7,0x3a7,0x30,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x2ec);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x340),0x20,0x10);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xd1,0,0x3a8,0x3a8,0x30,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x2f1);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x344),0x20,0x10);
  local_e = 0x35d;
  for (local_10 = 0x76; local_10 < 0x7f; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromPictResource
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,1,0);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x2fa);
    }
    _MySetSpriteScaledFactor
              (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_10 * 4),0x3f000000,0x3f000000);
    local_e = local_e + 1;
  }
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x75,0,0x36b,0x36b,0x82,0,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x301);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x1d4),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x7f,0,0x36c,0x36c,0x70,0,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x306);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x1fc),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0x80,0,0x36d,0x36d,0x70,0,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x30b);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x200),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromPictResource
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xb7,0,0x7e4,0x7e4,1,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x312);
  }
  _MySetSpriteScaledSize(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x2dc),0x60,0x6f);
  iVar1 = _MyLoadSpriteFromPictResource
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xcd,0,0x3ed,0x3ed,1,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x317);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x334),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromPictResource
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xb8,0,0x802,0x802,1,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x31c);
  }
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xd2,0,0x81,0x81,0x20,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,800);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x348),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromPictResource
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xd3,0,&DAT_0000238c,&DAT_0000238c,1
                     ,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x325);
  }
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xd4,0,&DAT_000023fa,&DAT_000023fa,
                     0x40,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x329);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x350),0x3f000000,0x3f000000);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xd5,0,&DAT_00002404,&DAT_00002404,
                     0x40,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x32e);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x354),0x3f000000,0x3f000000);
  iVar1 = _LoadStandardDimmableButtonSprite(PTR__gSpriteArray_001c40d4 + 0x358,0x1d4c,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x333);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x358),0x3f19999a,0x3f19999a);
  iVar1 = _LoadStandardButtonSprite(PTR__gSpriteArray_001c40d4 + 0x35c,"/lib/libgcc_s.1.dylib",4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x33e);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x35c),0x3f19999a,0x3f19999a);
  iVar1 = _MyLoadSpriteFromPictResource
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xdb,0,0x8c,0x8c,1,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x349);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x36c),0x3f000000,0x3f000000);
  _KeepMusicPlaying();
  uVar2 = _GetFontsFolderPath("BitmapFont.png");
  iVar1 = _SWCreateSpriteFromSingleFileXY
                    (PTR__gSpriteArray_001c40d4 + 0x370,0,uVar2,0x34,0x34,0,0,0,0x74,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x358);
  }
  uVar2 = _GetSpriteImagePath("optionsDialog/checkbox.png");
  iVar1 = _LoadMiscButtonSpriteFromFile(PTR__gSpriteArray_001c40d4 + 0x374,4,uVar2,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x35d);
  }
  uVar2 = _GetSpriteImagePath("optionsDialog/keyInputBox.png");
  iVar1 = _LoadMiscButtonSpriteFromFile(PTR__gSpriteArray_001c40d4 + 0x378,2,uVar2,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x361);
  }
  uVar2 = _GetSpriteImagePath("optionsDialog/controlsTab.png");
  iVar1 = _LoadMiscButtonSpriteFromFile(PTR__gSpriteArray_001c40d4 + 0x380,2,uVar2,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x365);
  }
  uVar2 = _GetSpriteImagePath("optionsDialog/optionsTab.png");
  iVar1 = _LoadMiscButtonSpriteFromFile(PTR__gSpriteArray_001c40d4 + 0x37c,2,uVar2,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___27458_001c4030,0x367);
  }
  uVar2 = _GetSpriteImagePath("episodeText.png");
  _SWCreateSpriteFromVerticalFile(PTR__gSpriteArray_001c40d4 + 0x388,0,uVar2,0,3,0);
  uVar2 = _GetSpriteImagePath("questionMark.png");
  iVar1 = _SWCreateSpriteFromFile(PTR__gSpriteArray_001c40d4 + 900,0,uVar2,1,0);
  if (iVar1 != 0) {
    _ReportError(iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___27458_001c4030,0x36f);
  }
  _SWSetSpriteScaledFactor(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 900),0x3f000000,0x3f000000);
  return;
}


// ---- _LoadNonPixelMaskSprites @ 0x71be (2207 bytes)

void _LoadNonPixelMaskSprites(void)

{
  int iVar1;
  short local_10;
  short local_e;
  
  _WriteCStringToFile("LoadNonPixelMaskSprites entered...\r");
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xca,0,0xbbd,0xbbd,0x32,0,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28023_001c426c,0x38b);
  }
  iVar1 = _MyLoadSpriteFromPictResource
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xcb,0,&DAT_000023f0,&DAT_000023f0,1
                     ,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28023_001c426c,0x38e);
  }
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,200,0,0xbc2,0xbc2,0x28,0,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28023_001c426c,0x395);
  }
  iVar1 = _MyLoadSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xc9,0,0xbc7,0xbc7,0x14,0,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28023_001c426c,0x39a);
  }
  _IncreaseProgressBar(1);
  local_e = 600;
  for (local_10 = 0x31; local_10 < 0x35; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromPictResource
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,1,0x10);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28023_001c426c,0x3a2);
    }
    local_e = local_e + 1;
  }
  _IncreaseProgressBar(1);
  _KeepMusicPlaying();
  local_e = 0x262;
  for (local_10 = 0x39; local_10 < 0x3d; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromPictResource
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,1,0x10);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28023_001c426c,0x3ac);
    }
    local_e = local_e + 1;
  }
  _IncreaseProgressBar(1);
  local_e = 0x26c;
  for (local_10 = 0x2d; local_10 < 0x31; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromPictResource
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,1,0x10);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28023_001c426c,0x3b5);
    }
    local_e = local_e + 1;
  }
  _IncreaseProgressBar(1);
  local_e = 0x276;
  for (local_10 = 0x3d; local_10 < 0x41; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromPictResource
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,1,0x10);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28023_001c426c,0x3be);
    }
    local_e = local_e + 1;
  }
  _IncreaseProgressBar(1);
  local_e = 0x280;
  for (local_10 = 0x35; local_10 < 0x39; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromPictResource
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,1,0x10);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28023_001c426c,0x3c7);
    }
    local_e = local_e + 1;
  }
  _IncreaseProgressBar(1);
  local_e = 0x28a;
  for (local_10 = 0x41; local_10 < 0x45; local_10 = local_10 + 1) {
    iVar1 = _MyLoadSpriteFromPictResource
                      (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,(int)local_10,0,(int)local_e,
                       (int)local_e,1,0x10);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28023_001c426c,0x3d0);
    }
    local_e = local_e + 1;
  }
  _IncreaseProgressBar(1);
  iVar1 = _MyLoadSpriteFromPictResource
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,0xcc,0,0xc1f,0xc1f,1,0x10);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28023_001c426c,0x3d7);
  }
  iVar1 = _MMCreateSpriteFromVerticalFile
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,PTR__gSpriteArray_001c40d4 + 0x360,0
                     ,"paused dialog.png",0,1,0);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28023_001c426c,0x3dc);
  }
  iVar1 = _MMCreateSpriteFromSingleFileXY
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,PTR__gSpriteArray_001c40d4 + 0x364,0
                     ,"paused dialog buttons.png",0,0x86,0x40,0xf,0x20);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28023_001c426c,0x3e1);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x364),0x3f19999a,0x3f19999a);
  iVar1 = _MMCreateSpriteFromSingleFileXY
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,PTR__gSpriteArray_001c40d4 + 0x368,0
                     ,"other buttons.png",0,0x86,0x40,0xc,0x20);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28023_001c426c,999);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x368),0x3f19999a,0x3f19999a);
  _IncreaseProgressBar(1);
  _KeepMusicPlaying();
  return;
}


// ---- _MySetSpriteScaledFactor @ 0x7a5d (39 bytes)

void _MySetSpriteScaledFactor(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  _SWSetSpriteScaledFactor(param_1,param_2,param_3);
  return;
}


// ---- _MySetSpriteScaledSize @ 0x7a84 (67 bytes)

void _MySetSpriteScaledSize(SpritePtr param_1,short param_2,short param_3)

{
  _SWSetSpriteScaledSize(param_1,SUB42((float)(int)param_2,0),SUB42((float)(int)param_3,0));
  return;
}


// ---- _MyLoadSpriteFromSinglePict @ 0x7ac7 (357 bytes)

undefined4
_MyLoadSpriteFromSinglePict
          (undefined4 param_1,short param_2,undefined4 param_3,short param_4,short param_5,
          short param_6,short param_7,int param_8)

{
  char cVar1;
  undefined4 uVar2;
  undefined1 local_244;
  undefined1 local_21e [500];
  char local_2a;
  undefined1 local_29;
  undefined4 local_28;
  undefined1 *local_24;
  SpritePtr *local_20;
  
  if ((param_4 == param_5) && (param_8 != 0)) {
    local_244 = 1;
  }
  else {
    local_244 = 0;
  }
  local_29 = local_244;
  local_28 = 0;
  cVar1 = _CheckIfSpriteShouldBeLoaded((int)param_4,(int)param_2,&local_2a,local_21e);
  if (cVar1 != '\0') {
    local_20 = (SpritePtr *)(PTR__gSpriteArray_001c40d4 + param_2 * 4);
    if (*local_20 != (SpritePtr)0x0) {
      _SWDisposeSprite(local_20);
    }
    if (local_2a == '\0') {
      uVar2 = _GetSpriteImagePath(0);
      local_24 = (undefined1 *)_GetFileNameForResource(uVar2,(int)param_4);
    }
    else {
      local_24 = local_21e;
    }
    local_28 = _SWCreateSpriteFromSingleFile
                         (local_20,param_3,local_24,(int)param_6,(int)param_7,local_29);
  }
  return local_28;
}


// ---- _MyLoadSpriteFromPictResource @ 0x7c2c (385 bytes)

undefined4
_MyLoadSpriteFromPictResource
          (undefined4 param_1,short param_2,undefined4 param_3,short param_4,short param_5,
          short param_6,int param_7)

{
  char cVar1;
  undefined4 uVar2;
  undefined1 local_230;
  undefined1 local_20e [500];
  char local_1a;
  undefined1 local_19;
  undefined4 local_18;
  undefined1 *local_14;
  SpritePtr *local_10;
  
  if ((param_4 == param_5) && (param_7 != 0)) {
    local_230 = 1;
  }
  else {
    local_230 = 0;
  }
  local_19 = local_230;
  local_18 = 0;
  if (param_6 != 1) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28191_001c42b8,0x444);
  }
  cVar1 = _CheckIfSpriteShouldBeLoaded((int)param_4,(int)param_2,&local_1a,local_20e);
  if (cVar1 != '\0') {
    local_10 = (SpritePtr *)(PTR__gSpriteArray_001c40d4 + param_2 * 4);
    if (*local_10 != (SpritePtr)0x0) {
      _SWDisposeSprite(local_10);
    }
    if (local_1a == '\0') {
      uVar2 = _GetSpriteImagePath(0);
      local_14 = (undefined1 *)_GetFileNameForResource(uVar2,(int)param_4);
    }
    else {
      local_14 = local_20e;
    }
    local_18 = _SWCreateSpriteFromFile(local_10,param_3,local_14,(int)param_6,local_19);
  }
  return local_18;
}


// ---- _MyLoadSpriteFromSinglePictXY @ 0x7dad (541 bytes)

undefined4
_MyLoadSpriteFromSinglePictXY
          (undefined4 param_1,short param_2,undefined4 param_3,short param_4,short param_5,
          short param_6,short param_7,short param_8,short param_9,undefined1 param_10,short param_11
          ,int param_12)

{
  char cVar1;
  undefined4 uVar2;
  undefined1 local_25c;
  undefined1 local_221 [500];
  char local_2d;
  undefined4 local_2c;
  undefined1 local_25;
  SpritePtr *local_24;
  undefined1 *local_20;
  
  local_2c = 0;
  if ((param_4 == param_5) && (param_12 != 0)) {
    local_25c = 1;
  }
  else {
    local_25c = 0;
  }
  local_25 = local_25c;
  cVar1 = _CheckIfSpriteShouldBeLoaded((int)param_4,(int)param_2,&local_2d,local_221);
  if (cVar1 != '\0') {
    local_24 = (SpritePtr *)(PTR__gSpriteArray_001c40d4 + param_2 * 4);
    if (*local_24 != (SpritePtr)0x0) {
      _SWDisposeSprite(local_24);
    }
    if (local_2d == '\0') {
      uVar2 = _GetSpriteImagePath(0);
      local_20 = (undefined1 *)_GetFileNameForResource(uVar2,(int)param_4);
    }
    else {
      local_20 = local_221;
    }
    local_2c = _SWCreateSpriteFromSingleFileXY
                         (local_24,param_3,local_20,(int)param_6,(int)param_7,(int)param_8,
                          (int)param_9,param_10,(int)param_11,local_25);
  }
  return local_2c;
}


// ---- _CheckIfSpriteShouldBeLoaded @ 0x7fca (327 bytes)

undefined1
_CheckIfSpriteShouldBeLoaded(short param_1,short param_2,undefined1 *param_3,char *param_4)

{
  char cVar1;
  undefined1 local_11;
  int local_10;
  
  if (*PTR__spriteFlagsWereInitialized_001c4028 == '\0') {
    for (local_10 = 0; local_10 < 0xe3; local_10 = local_10 + 1) {
      *(undefined2 *)(PTR__spriteFlags_001c4250 + local_10 * 2) = 0xffff;
    }
    *PTR__spriteFlagsWereInitialized_001c4028 = 1;
  }
  *param_3 = 0;
  if (*(short *)(PTR__spriteFlags_001c4250 + param_2 * 2) == -1) {
    local_11 = 1;
    *(undefined2 *)(PTR__spriteFlags_001c4250 + param_2 * 2) = 0;
  }
  else {
    cVar1 = _DoesCustomPICTResourceExist(PTR__gCurOpenMansionFilePath_001c4124,(int)param_1);
    if (cVar1 == '\0') {
      if (*(short *)(PTR__spriteFlags_001c4250 + param_2 * 2) == 1) {
        local_11 = 1;
        *(undefined2 *)(PTR__spriteFlags_001c4250 + param_2 * 2) = 0;
      }
      else {
        local_11 = 0;
      }
    }
    else {
      _sprintf(param_4,"%s\'%s\'/%05d.png",PTR__gCurOpenMansionFilePath_001c4124,"PICT",(int)param_1
              );
      local_11 = 1;
      *(undefined2 *)(PTR__spriteFlags_001c4250 + param_2 * 2) = 1;
      *param_3 = 1;
    }
  }
  return local_11;
}


// ---- _ScanDirectoryForPICTResIDs @ 0x8111 (348 bytes)

undefined4 _ScanDirectoryForPICTResIDs(char *param_1)

{
  short sVar1;
  undefined4 local_210;
  char local_20c [500];
  int local_18;
  int local_14;
  int local_10;
  
  *(undefined2 *)PTR__gLastValidPICTResIDIndex_001c41d8 = 0;
  _sprintf(local_20c,"%s%s",param_1,"\'PICT\'");
  local_18 = _opendir(local_20c);
  if (local_18 == 0) {
    local_210 = 0xffffffff;
  }
  else {
    _printf("Scanning path for custom Tile/Sprite PICT res IDs... %s \n",local_20c);
    _strcpy(PTR__scannedPath_001c429c,param_1);
    do {
      do {
        local_14 = _readdir(local_18);
        if (local_14 == 0) goto LAB_00008246;
      } while (*(char *)(local_14 + 6) != '\b');
      local_10 = _atoi((char *)(local_14 + 8));
      if (0 < local_10) {
        sVar1 = *(short *)PTR__gLastValidPICTResIDIndex_001c41d8;
        *(short *)(PTR__gValidPICTResIDs_001c405c + sVar1 * 2) = (short)local_10;
        *(short *)PTR__gLastValidPICTResIDIndex_001c41d8 = sVar1 + 1;
      }
    } while (*(short *)PTR__gLastValidPICTResIDIndex_001c41d8 < 5000);
    _printf("ERROR! ScanDirectoryForPICTResIDs exceeded array bounds!");
    *(undefined2 *)PTR__gLastValidPICTResIDIndex_001c41d8 = 0;
LAB_00008246:
    _closedir(local_18);
    local_210 = 0;
  }
  return local_210;
}


// ---- _DoesCustomPICTResourceExist @ 0x826d (257 bytes)

undefined1 _DoesCustomPICTResourceExist(char *param_1,short param_2)

{
  undefined1 uVar1;
  int iVar2;
  char local_204 [500];
  int local_10;
  
  if ((0 < *(short *)PTR__gLastValidPICTResIDIndex_001c41d8) &&
     (iVar2 = _strcmp(PTR__scannedPath_001c429c,param_1), iVar2 == 0)) {
    local_10 = 0;
    while( true ) {
      if (*(short *)PTR__gLastValidPICTResIDIndex_001c41d8 <= local_10) {
        return 0;
      }
      if (*(short *)(PTR__gValidPICTResIDs_001c405c + local_10 * 2) == param_2) break;
      local_10 = local_10 + 1;
    }
    _sprintf(local_204,"%s\'%s\'/%05d.png",PTR__gCurOpenMansionFilePath_001c4124,"PICT",(int)param_2
            );
    uVar1 = _DoesFileExist(local_204);
    return uVar1;
  }
  return 0;
}


// ---- _LoadStatsSprites @ 0x836e (436 bytes)

void _LoadStatsSprites(void)

{
  int iVar1;
  
  if (*PTR__gStatsAreaSpritesAreValid_001c41fc != '\0') {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28433_001c4260,0x538);
  }
  iVar1 = _CreateSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,PTR__gStatsSpriteArray_001c4230,0,
                     0x13ec,0x13ec,0x28,0,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28433_001c4260,0x53d);
  }
  _MySetSpriteScaledFactor(*(undefined4 *)PTR__gStatsSpriteArray_001c4230,0x3f000000,0x3f000000);
  iVar1 = _CreateSpriteFromSinglePict
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,PTR__gStatsSpriteArray_001c4230 + 4,
                     0,0x1450,0x1451,0x3c,0,0x28);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28433_001c4260,0x543);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gStatsSpriteArray_001c4230 + 4),0x3f000000,0x3f000000);
  *PTR__gStatsAreaSpritesAreValid_001c41fc = 1;
  return;
}


// ---- _LoadMapScreenSprites @ 0x8522 (1245 bytes)

void _LoadMapScreenSprites(void)

{
  undefined4 uVar1;
  char local_117 [255];
  int local_18;
  short local_12;
  short local_e;
  
  if (*PTR__gMapScreenSpritesAreValid_001c4298 != '\0') {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28457_001c4088,0x555);
  }
  local_e = 0;
  for (local_12 = 0; local_12 < 0xb; local_12 = local_12 + 1) {
    _sprintf(local_117,"map_mansion_%d.png",(int)local_e);
    uVar1 = _GetMapScreenFolderPath(local_117);
    local_18 = _SWCreateSpriteFromFile
                         (PTR__gMapScreenSpriteArray_001c412c + local_12 * 4,0,uVar1,1,0);
    if (local_18 != 0) {
      _MMFatalErrorCallback
                (local_18,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28457_001c4088,0x565);
    }
    _SWSetSpriteScaledFactor
              (*(undefined4 *)(PTR__gMapScreenSpriteArray_001c412c + local_12 * 4),0x3f000000,
               0x3f000000);
    if (local_18 != 0) {
      _MMFatalErrorCallback
                (local_18,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28457_001c4088,0x567);
    }
    local_e = local_e + 1;
  }
  uVar1 = _GetMapScreenFolderPath("map_screen_mansion_names.png");
  local_18 = _SWCreateSpriteFromSingleFile
                       (PTR__gMapScreenSpriteArray_001c412c + 0x2c,0,uVar1,0x30,0,0);
  if (local_18 != 0) {
    _MMFatalErrorCallback
              (local_18,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28457_001c4088,0x572);
  }
  _SWSetSpriteScaledFactor
            (*(undefined4 *)(PTR__gMapScreenSpriteArray_001c412c + 0x2c),0x3f000000,0x3f000000);
  if (local_18 != 0) {
    _MMFatalErrorCallback
              (local_18,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28457_001c4088,0x575);
  }
  local_18 = _CreateSpriteFromPictResource
                       (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                        PTR__gMapScreenSpriteArray_001c412c + 0x30,0,0x186a,0x186a,1,4);
  if (local_18 != 0) {
    _MMFatalErrorCallback
              (local_18,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28457_001c4088,0x57c);
  }
  _MySetSpriteScaledFactor
            (*(undefined4 *)(PTR__gMapScreenSpriteArray_001c412c + 0x30),0x3f3ae148,0x3f3ae148);
  uVar1 = _GetMapScreenFolderPath("select_a_mansion.png");
  local_18 = _SWCreateSpriteFromFile(PTR__gMapScreenSpriteArray_001c412c + 0x34,0,uVar1,1,0);
  if (local_18 != 0) {
    _MMFatalErrorCallback
              (local_18,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28457_001c4088,0x588);
  }
  _SWSetSpriteScaledFactor
            (*(undefined4 *)(PTR__gMapScreenSpriteArray_001c412c + 0x34),0x3f000000,0x3f000000);
  uVar1 = _GetMapScreenFolderPath("start_here.png");
  local_18 = _SWCreateSpriteFromFile(PTR__gMapScreenSpriteArray_001c412c + 0x3c,0,uVar1,1,0);
  if (local_18 != 0) {
    _MMFatalErrorCallback
              (local_18,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28457_001c4088,0x58e);
  }
  _SWSetSpriteScaledFactor
            (*(undefined4 *)(PTR__gMapScreenSpriteArray_001c412c + 0x3c),0x3f000000,0x3f000000);
  if (local_18 != 0) {
    _MMFatalErrorCallback
              (local_18,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28457_001c4088,0x591);
  }
  local_18 = _CreateSpriteFromPictResource
                       (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                        PTR__gMapScreenSpriteArray_001c412c + 0x38,0,&dylib_command_00001874,
                        &dylib_command_00001874,1,4);
  if (local_18 != 0) {
    _MMFatalErrorCallback
              (local_18,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28457_001c4088,0x598);
  }
  *PTR__gMapScreenSpritesAreValid_001c4298 = 1;
  return;
}


// ---- _LoadTitleScreenSprites @ 0x89ff (1923 bytes)

void _LoadTitleScreenSprites(void)

{
  undefined4 uVar1;
  SWError SVar2;
  char local_117 [255];
  int local_18;
  int local_14;
  int local_10;
  
  if (*PTR__gTitleScreenSpritesAreValid_001c4074 == '\0') {
    _WriteCStringToFile("LoadTitleScreenSprites entered...\r");
    if (*PTR__gTitleScreenSpritesAreValid_001c4074 != '\0') {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28517_001c4120,0x5af);
    }
    uVar1 = _GetTitleScreenFolderPath("buttons.png");
    local_10 = _SWCreateSpriteFromSingleFileXY
                         (PTR__gTitleScreenSpriteArray_001c4160,0,uVar1,0x168,0x3c,0,0,0,0xe,0);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28517_001c4120,0x5c9);
    }
    _MySetSpriteScaledSize(*(undefined4 *)PTR__gTitleScreenSpriteArray_001c4160,0x3c0,0x230);
    uVar1 = _GetTitleScreenFolderPath("tab_buttons.png");
    local_10 = _SWCreateSpriteFromSingleFileXY
                         (PTR__gTitleScreenSpriteArray_001c4160 + 8,0,uVar1,0xf0,0x48,0,0,0,8,0);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28517_001c4120,0x5d7);
    }
    uVar1 = _GetTitleScreenFolderPath("tab_button_glow.png");
    local_10 = _SWCreateSpriteFromFile(PTR__gTitleScreenSpriteArray_001c4160 + 0xc,0,uVar1,1,0);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28517_001c4120,0x5dd);
    }
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28517_001c4120,0x5df);
    }
    _KeepMusicPlaying();
    uVar1 = _GetTitleScreenFolderPath("logo.png");
    local_10 = _SWCreateSpriteFromFile(PTR__gTitleScreenSpriteArray_001c4160 + 4,0,uVar1,1,0);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28517_001c4120,0x608);
    }
    _SWSetSpriteScaledSize(*(SpritePtr *)(PTR__gTitleScreenSpriteArray_001c4160 + 4),-0x4000,0);
    uVar1 = _GetTitleScreenFolderPath("mansion_bottom_and_ground.png");
    local_10 = _SWCreateSpriteFromFile(PTR__gTitleScreenSpriteArray_001c4160 + 0x10,0,uVar1,1,0);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28517_001c4120,0x60e);
    }
    _SWSetSpriteScaledSize(*(SpritePtr *)(PTR__gTitleScreenSpriteArray_001c4160 + 0x10),0,-0x8000);
    uVar1 = _GetTitleScreenFolderPath("mansion_top.png");
    local_10 = _SWCreateSpriteFromFile(PTR__gTitleScreenSpriteArray_001c4160 + 0x14,0,uVar1,1,0);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28517_001c4120,0x614);
    }
    _SWSetSpriteScaledSize(*(SpritePtr *)(PTR__gTitleScreenSpriteArray_001c4160 + 0x14),0x4000,0);
    uVar1 = _GetTitleScreenFolderPath("foreground_fog.png");
    local_10 = _SWCreateSpriteFromFile(PTR__gTitleScreenSpriteArray_001c4160 + 0x18,0,uVar1,1,0);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28517_001c4120,0x61a);
    }
    _SWSetSpriteScaledSize(*(SpritePtr *)(PTR__gTitleScreenSpriteArray_001c4160 + 0x18),0,-0x8000);
    uVar1 = _GetTitleScreenFolderPath("full_moon.png");
    local_10 = _SWCreateSpriteFromFile(PTR__gTitleScreenSpriteArray_001c4160 + 0x1c,0,uVar1,1,0);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28517_001c4120,0x620);
    }
    local_18 = 1;
    for (local_14 = 8; local_14 < 0xc; local_14 = local_14 + 1) {
      _sprintf(local_117,"cloud_%d.png",local_18);
      uVar1 = _GetTitleScreenFolderPath(local_117);
      local_10 = _SWCreateSpriteFromVerticalFile
                           (PTR__gTitleScreenSpriteArray_001c4160 + local_14 * 4,0,uVar1,0,2,0);
      if (local_10 != 0) {
        _MMFatalErrorCallback
                  (local_10,
                   "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                   PTR____FUNCTION___28517_001c4120,0x629);
      }
      _SWSetSpriteScaledFactor
                (*(undefined4 *)(PTR__gTitleScreenSpriteArray_001c4160 + local_14 * 4),0x3fb6dc5d,
                 0x3fb6dc5d);
      local_18 = local_18 + 1;
    }
    uVar1 = _GetTitleScreenFolderPath("jack_animated.png");
    local_10 = _SWCreateSpriteFromVerticalFile
                         (PTR__gTitleScreenSpriteArray_001c4160 + 0x30,0,uVar1,0,3,0);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28517_001c4120,0x630);
    }
    uVar1 = _GetTitleScreenFolderPath("flag_animated.png");
    local_10 = _SWCreateSpriteFromVerticalFile
                         (PTR__gTitleScreenSpriteArray_001c4160 + 0x34,0,uVar1,0,3,0);
    if (local_10 != 0) {
      _MMFatalErrorCallback
                (local_10,
                 "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28517_001c4120,0x635);
    }
    _WriteCStringToFile("LoadTitleScreenSprites completed with no problems.\r");
    *PTR__gTitleScreenSpritesAreValid_001c4074 = 1;
    SVar2 = _SWStickyError();
    if (SVar2 != 0) {
      SVar2 = _SWStickyError();
      _MMFatalErrorCallback
                (SVar2,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28517_001c4120,0x648);
    }
  }
  return;
}


// ---- _LoadDifficultyScreenSprites @ 0x9182 (615 bytes)

void _LoadDifficultyScreenSprites(void)

{
  int iVar1;
  short local_10;
  short local_e;
  
  if (*PTR__gDifficultyScreenSpritesAreValid_001c4194 != '\0') {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28605_001c4064,0x655);
  }
  local_e = 0x1c98;
  for (local_10 = 0; local_10 < 3; local_10 = local_10 + 1) {
    iVar1 = _LoadStandardButtonSprite
                      (PTR__gDifficultyScreenSpriteArray_001c42ac + local_10 * 4,(int)local_e,4);
    if (iVar1 != 0) {
      _MMFatalErrorCallback
                (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                 PTR____FUNCTION___28605_001c4064,0x65d);
    }
    _SWSetSpriteScaledFactor
              (*(undefined4 *)(PTR__gDifficultyScreenSpriteArray_001c42ac + local_10 * 4),0x3f200000
               ,0x3f200000);
    local_e = local_e + 1;
  }
  _KeepMusicPlaying();
  iVar1 = _CreateSpriteFromPictResource
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                     PTR__gDifficultyScreenSpriteArray_001c42ac + 0xc,0,
                     "orks/SDL_image.framework/Versions/A/SDL_image",
                     "orks/SDL_image.framework/Versions/A/SDL_image",1,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28605_001c4064,0x668);
  }
  _SWSetSpriteScaledFactor
            (*(undefined4 *)(PTR__gDifficultyScreenSpriteArray_001c42ac + 0xc),0x3f200000,0x3f200000
            );
  iVar1 = _CreateSpriteFromPictResource
                    (*(undefined4 *)PTR__gSpriteWorldP_001c40d0,
                     PTR__gDifficultyScreenSpriteArray_001c42ac + 0x10,0,"image","image",3,4);
  if (iVar1 != 0) {
    _MMFatalErrorCallback
              (iVar1,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28605_001c4064,0x670);
  }
  _SWSetSpriteScaledFactor
            (*(undefined4 *)(PTR__gDifficultyScreenSpriteArray_001c42ac + 0x10),0x3f666666,
             0x3f666666);
  _KeepMusicPlaying();
  *PTR__gDifficultyScreenSpritesAreValid_001c4194 = 1;
  return;
}


// ---- _LoadOrganScreenSprites @ 0x93e9 (622 bytes)

void _LoadOrganScreenSprites(void)

{
  undefined4 uVar1;
  int iVar2;
  short local_12;
  
  if (*PTR__gOrganScreenSpritesAreValid_001c422c != '\0') {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28640_001c40f0,0x692);
  }
  uVar1 = _GetOrganScreenFolderPath("organ_dude.png");
  iVar2 = _SWCreateSpriteFromSingleFile(PTR__gOrganScreenSpriteArray_001c42b4,0,uVar1,0x168,0,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28640_001c40f0,0x698);
  }
  _SWSetSpriteScaledFactor
            (*(undefined4 *)PTR__gOrganScreenSpriteArray_001c42b4,0x3f000000,0x3f000000);
  uVar1 = _GetOrganScreenFolderPath("organScreenText.png");
  iVar2 = _SWCreateSpriteFromVerticalFile(PTR__gOrganScreenSpriteArray_001c42b4 + 8,0,uVar1,0,8,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28640_001c40f0,0x6aa);
  }
  _SWSetSpriteScaledFactor
            (*(undefined4 *)(PTR__gOrganScreenSpriteArray_001c42b4 + 8),0x3f000000,0x3f000000);
  uVar1 = _GetOrganScreenFolderPath("animated_keys.png");
  iVar2 = _SWCreateSpriteFromVerticalFile(PTR__gOrganScreenSpriteArray_001c42b4 + 4,0,uVar1,0,4,0);
  if (iVar2 != 0) {
    _MMFatalErrorCallback
              (iVar2,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28640_001c40f0,0x6b4);
  }
  _SWSetSpriteScaledFactor
            (*(undefined4 *)(PTR__gOrganScreenSpriteArray_001c42b4 + 4),0x3f000000,0x3f000000);
  for (local_12 = 0; local_12 < 3; local_12 = local_12 + 1) {
    _SWLockSprite(*(SpritePtr *)(PTR__gOrganScreenSpriteArray_001c42b4 + local_12 * 4));
  }
  *PTR__gOrganScreenSpritesAreValid_001c422c = 1;
  return;
}


// ---- _DisposeMapScreenSprites @ 0x9657 (100 bytes)

void _DisposeMapScreenSprites(void)

{
  short local_e;
  
  if (*PTR__gMapScreenSpritesAreValid_001c4298 != '\0') {
    for (local_e = 0; local_e < 0x10; local_e = local_e + 1) {
      _SWDisposeSprite((SpritePtr *)(PTR__gMapScreenSpriteArray_001c412c + local_e * 4));
    }
    *PTR__gMapScreenSpritesAreValid_001c4298 = 0;
  }
  return;
}


// ---- _DisposeTitleScreenSprites @ 0x96bb (100 bytes)

void _DisposeTitleScreenSprites(void)

{
  short local_e;
  
  if (*PTR__gTitleScreenSpritesAreValid_001c4074 != '\0') {
    for (local_e = 0; local_e < 0xe; local_e = local_e + 1) {
      _SWDisposeSprite((SpritePtr *)(PTR__gTitleScreenSpriteArray_001c4160 + local_e * 4));
    }
    *PTR__gTitleScreenSpritesAreValid_001c4074 = 0;
  }
  return;
}


// ---- _DisposeDifficultyScreenSprites @ 0x971f (100 bytes)

void _DisposeDifficultyScreenSprites(void)

{
  short local_e;
  
  if (*PTR__gDifficultyScreenSpritesAreValid_001c4194 != '\0') {
    for (local_e = 0; local_e < 5; local_e = local_e + 1) {
      _SWDisposeSprite((SpritePtr *)(PTR__gDifficultyScreenSpriteArray_001c42ac + local_e * 4));
    }
    *PTR__gDifficultyScreenSpritesAreValid_001c4194 = 0;
  }
  return;
}


// ---- _DisposeOrganScreenSprites @ 0x9783 (100 bytes)

void _DisposeOrganScreenSprites(void)

{
  short local_e;
  
  if (*PTR__gOrganScreenSpritesAreValid_001c422c != '\0') {
    for (local_e = 0; local_e < 3; local_e = local_e + 1) {
      _SWDisposeSprite((SpritePtr *)(PTR__gOrganScreenSpriteArray_001c42b4 + local_e * 4));
    }
    *PTR__gOrganScreenSpritesAreValid_001c422c = 0;
  }
  return;
}


// ---- _DisposeSprites @ 0x97e7 (150 bytes)

void DisposeSprites(void)

{
  short sStack_e;
  
  for (sStack_e = 0; sStack_e < 0xe3; sStack_e = sStack_e + 1) {
    _SWDisposeSprite((SpritePtr *)(PTR__gSpriteArray_001c40d4 + sStack_e * 4));
  }
  if (*PTR__gStatsAreaSpritesAreValid_001c41fc != '\0') {
    for (sStack_e = 0; sStack_e < 2; sStack_e = sStack_e + 1) {
      _SWDisposeSprite((SpritePtr *)(PTR__gStatsSpriteArray_001c4230 + sStack_e * 4));
    }
    *PTR__gStatsAreaSpritesAreValid_001c41fc = 0;
  }
  return;
}


// ---- _IncreaseProgressBar @ 0x987d (21 bytes)

void _IncreaseProgressBar(void)

{
  return;
}


// ---- _NewMapScreenSprite @ 0x9892 (678 bytes)

SpritePtr _NewMapScreenSprite(short param_1)

{
  SpritePtr local_14;
  int local_10;
  
  local_10 = 0;
  if (*PTR__gMapScreenSpritesAreValid_001c4298 == '\0') {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28753_001c4270,0x748);
  }
  if ((param_1 < 0) || (0xf < param_1)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28753_001c4270,0x749);
  }
  if ((param_1 < 0) || (10 < param_1)) {
    if (param_1 == 0xb) {
      local_14 = (SpritePtr)
                 _CloneButtonSprite(*(undefined4 *)(PTR__gMapScreenSpriteArray_001c412c + 0x2c),0);
      if (local_14 == (SpritePtr)0x0) {
        local_10 = -1;
      }
      if (local_10 != 0) {
        _MMFatalErrorCallback
                  (local_10,
                   "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                   PTR____FUNCTION___28753_001c4270,0x75b);
      }
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,local_14);
    }
    else if (((param_1 == 0xc) || (param_1 == 0xe)) || (param_1 == 0xf)) {
      local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gMapScreenSpriteArray_001c412c + param_1 * 4),
                                &local_14,0,0);
      if (local_10 != 0) {
        _MMFatalErrorCallback
                  (local_10,
                   "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                   PTR____FUNCTION___28753_001c4270,0x762);
      }
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gForegroundSpriteLayerP_001c41f0,local_14);
    }
    else {
      local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gMapScreenSpriteArray_001c412c + param_1 * 4),
                                &local_14,0,0);
      if (local_10 != 0) {
        _MMFatalErrorCallback
                  (local_10,
                   "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
                   PTR____FUNCTION___28753_001c4270,0x76a);
      }
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,local_14);
    }
  }
  else {
    local_14 = (SpritePtr)
               _CloneButtonSprite(*(undefined4 *)(PTR__gMapScreenSpriteArray_001c412c + param_1 * 4)
                                  ,(int)param_1);
    if (local_14 != (SpritePtr)0x0) {
      _SetUpButtonSprite(local_14,0,0,0,0);
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gBottomSpriteLayerP_001c4140,local_14);
    }
  }
  return local_14;
}


// ---- _NewTitleScreenButtonSprite @ 0x9b38 (163 bytes)

SpritePtr _NewTitleScreenButtonSprite(short param_1,short param_2)

{
  SpritePtr newSpriteP;
  
  if (*PTR__gTitleScreenSpritesAreValid_001c4074 == '\0') {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28814_001c4274,0x77c);
  }
  newSpriteP = (SpritePtr)
               _CloneButtonSprite(*(undefined4 *)
                                   (PTR__gTitleScreenSpriteArray_001c4160 + param_1 * 4),
                                  (int)param_2);
  if (newSpriteP != (SpritePtr)0x0) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,newSpriteP);
  }
  return newSpriteP;
}


// ---- _NewTitleScreenSprite @ 0x9bdb (425 bytes)

SpritePtr _NewTitleScreenSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  if (*PTR__gTitleScreenSpritesAreValid_001c4074 == '\0') {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28834_001c406c,0x793);
  }
  if ((param_1 < 0) || (0xd < param_1)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28834_001c406c,0x794);
  }
  local_14 = _calloc(1,300);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28834_001c406c,0x799);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gTitleScreenSpriteArray_001c4160 + param_1 * 4),
                            &local_18,local_14,300);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28834_001c406c,0x79c);
  }
  if (param_1 == 6) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBackgroundSpriteLayerP_001c4070,local_18);
  }
  if (param_1 == 1) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164,local_18);
  }
  else if (local_18->parentSpriteLayerP == (SpriteLayerRec *)0x0) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,local_18);
  }
  local_18->userData = (int)param_1;
  return local_18;
}


// ---- _NewDifficultyScreenButtonSprite @ 0x9d84 (156 bytes)

SpritePtr _NewDifficultyScreenButtonSprite(short param_1)

{
  SpritePtr newSpriteP;
  
  if (*PTR__gDifficultyScreenSpritesAreValid_001c4194 == '\0') {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28874_001c4154,0x7f0);
  }
  newSpriteP = (SpritePtr)
               _CloneButtonSprite(*(undefined4 *)
                                   (PTR__gDifficultyScreenSpriteArray_001c42ac + param_1 * 4),
                                  (int)param_1);
  if (newSpriteP != (SpritePtr)0x0) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,newSpriteP);
  }
  return newSpriteP;
}


// ---- _NewDifficultyScreenSprite @ 0x9e20 (350 bytes)

SpritePtr _NewDifficultyScreenSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  if (*PTR__gDifficultyScreenSpritesAreValid_001c4194 == '\0') {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28894_001c4228,0x805);
  }
  if ((param_1 < 0) || (4 < param_1)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28894_001c4228,0x806);
  }
  local_14 = _calloc(1,300);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28894_001c4228,0x80b);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)
                             (PTR__gDifficultyScreenSpriteArray_001c42ac + param_1 * 4),&local_18,
                            local_14,300);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28894_001c4228,0x80e);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,local_18);
  local_18->userData = (int)param_1;
  return local_18;
}


// ---- _NewOrganScreenSprite @ 0x9f7e (418 bytes)

SpritePtr _NewOrganScreenSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  if (*PTR__gOrganScreenSpritesAreValid_001c422c == '\0') {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28923_001c40c8,0x823);
  }
  if ((param_1 < 0) || (2 < param_1)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28923_001c40c8,0x824);
  }
  local_14 = _calloc(1,300);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28923_001c40c8,0x829);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gOrganScreenSpriteArray_001c42b4 + param_1 * 4),
                            &local_18,local_14,300);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28923_001c40c8,0x82c);
  }
  if (param_1 == 0) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164,local_18);
  }
  else if (param_1 == 1) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBottomSpriteLayerP_001c4140,local_18);
  }
  else {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopSpriteLayerP_001c41b4,local_18);
  }
  local_18->userData = (int)param_1;
  return local_18;
}


// ---- _NewMainSprite @ 0xa120 (1594 bytes)

SpritePtr _NewMainSprite(void)

{
  SpritePtr local_30;
  void *local_2c;
  short local_26;
  short local_24;
  short local_22;
  int local_20;
  
  local_2c = _calloc(1,0x1d8);
  if (local_2c == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28965_001c40b8,0x84b);
  }
  local_20 = _SWCloneSprite(*(undefined4 *)PTR__gSpriteArray_001c40d4,&local_30,local_2c,0x1d8);
  if (local_20 != 0) {
    _MMFatalErrorCallback
              (local_20,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___28965_001c40b8,0x84e);
  }
  _SWSetSpriteDrawProc(local_30,(DrawProcPtr_t)PTR_LAB_001c4048);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gHeroSpriteLayerP_001c4164,local_30);
  _SWSetSpriteMoveProc(local_30,(MoveProcPtr_t)PTR_LAB_001c4098);
  _SWSetSpriteCollideProc(local_30,(CollideProcPtr_t)PTR_LAB_001c4040);
  _SWSetSpriteFrameAdvanceMode(local_30,(void *)0x0);
  local_30->userData = 0;
  local_24 = (local_30->destFrameRect).bottom - (local_30->destFrameRect).top;
  local_22 = (short)(((int)(local_30->destFrameRect).right - (int)(local_30->destFrameRect).left) /
                    2);
  _SWSetRect((int)local_2c + 0x10a,0x13,0xd,0x12,0);
  _SWSetRect((int)local_2c + 0x102,0x14,0xf,0x19,0);
  _SWSetRect((int)local_2c + 0x13a,0x1a,0xf,0x19,10);
  _SWSetRect((int)local_2c + 0x142,0x12,(int)(short)(local_24 + -10),0x19,0);
  *(undefined2 *)((int)local_2c + 0x14a) = *(undefined2 *)((int)local_2c + 0x10a);
  *(undefined2 *)((int)local_2c + 0x14c) = *(undefined2 *)((int)local_2c + 0x102);
  _SWSetRect((int)local_2c + 0x112,0xc,3,0xc,0);
  _SWSetRect((int)local_2c + 0x11a,0x10,(int)(short)(local_24 + 8),0x10,0xfffffff2);
  _SWSetRect((int)local_2c + 0x122,0,0x28,0,0);
  _SWSetRect((int)local_2c + 0x12a,0,(int)(short)(local_24 + 8),0,0xfffffff2);
  _SWSetRect((int)local_2c + 0x132,(int)(short)(local_22 + -2),3,(int)(short)(local_22 + -2),0);
  for (local_26 = 0; (int)local_26 < local_30->maxFrames; local_26 = local_26 + 1) {
    if ((local_26 < 0xf) || (0x11 < local_26)) {
      _SWSetFrameCollisionInset
                (local_30->frameArray[local_26],*(short *)((int)local_2c + 0x104),
                 *(short *)((int)local_2c + 0x102),*(short *)((int)local_2c + 0x108),
                 *(short *)((int)local_2c + 0x106));
    }
    else {
      _SWSetFrameCollisionInset
                (local_30->frameArray[local_26],0,*(short *)((int)local_2c + 0x102) * 2,0,
                 *(short *)((int)local_2c + 0x106));
    }
  }
  *(undefined4 *)((int)local_2c + 0x150) = 0;
  *(undefined4 *)((int)local_2c + 0x158) = 0;
  *(undefined1 *)((int)local_2c + 0x16e) = 1;
  *(undefined1 *)((int)local_2c + 0x16f) = 1;
  *(undefined1 *)((int)local_2c + 0x170) = 0;
  *(undefined1 *)((int)local_2c + 0x171) = 0;
  *(undefined2 *)((int)local_2c + 0x174) = 0;
  *(undefined1 *)((int)local_2c + 0x176) = 0;
  *(undefined1 *)((int)local_2c + 0x177) = 1;
  *(undefined1 *)((int)local_2c + 0x17b) = 0;
  *(undefined1 *)((int)local_2c + 0x17c) = 0;
  *(undefined1 *)((int)local_2c + 0x17d) = 0;
  *(undefined1 *)((int)local_2c + 0x17a) = 0;
  *(undefined1 *)((int)local_2c + 0x17e) = 0;
  *(undefined2 *)((int)local_2c + 0x16a) = 0;
  *(undefined1 *)((int)local_2c + 0x172) = 0;
  *(undefined1 *)((int)local_2c + 0x17e) = 1;
  *(undefined4 *)((int)local_2c + 0x1c8) = 0;
  *(undefined1 *)((int)local_2c + 0x17f) = 0;
  *(undefined4 *)((int)local_2c + 0x1c0) = 0;
  *(undefined4 *)((int)local_2c + 0x1cc) = 0;
  *(undefined4 *)((int)local_2c + 0x1d0) = 0;
  *(undefined2 *)((int)local_2c + 0x1bc) = 1;
  *(undefined2 *)((int)local_2c + 0x19a) = 0;
  *(undefined2 *)((int)local_2c + 0x19e) = 0;
  *(undefined2 *)((int)local_2c + 0x1a0) = 0;
  *(undefined1 *)((int)local_2c + 0x1d4) = 0;
  *(undefined2 *)((int)local_2c + 0x1a4) = 300;
  *(undefined2 *)((int)local_2c + 0x1a6) = 200;
  *(undefined4 *)((int)local_2c + 0xf8) = 0xffffffff;
  _SWSetSpriteFrameRange(local_30,0,2);
  _SWSetSpriteFrameAdvanceMode(local_30,(void *)0x1);
  _SWSetSpriteFrameProc(local_30,(FrameProcPtr_t)PTR_HeroMiscFrameProc_001c41d0);
  _SWSetSpriteFrameAdvance(local_30,0);
  _SWSetSpriteFrameTime(local_30,0);
  *(undefined2 *)((int)local_2c + 0xfc) = 0xf;
  *(undefined2 *)((int)local_2c + 0x100) = 0xf;
  *(undefined2 *)((int)local_2c + 0xfe) = 1;
  return local_30;
}


// ---- _NewHaloSprite @ 0xa75a (286 bytes)

SpritePtr _NewHaloSprite(void)

{
  SpritePtr local_1c [3];
  int local_10;
  
  local_1c[0] = (SpritePtr)0x0;
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x34c),local_1c,0,0);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29067_001c407c,0x8e8);
  }
  _SWSetSpriteBlendMode(local_1c[0],0x306,0x303);
  local_1c[0]->misc1 = 1;
  local_1c[0]->userData = 0xcfff;
  _SWSetSpriteTranslucency(local_1c[0],local_1c[0]->userData);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gHaloSpriteLayerP_001c41e0,local_1c[0]);
  local_1c[0]->miscFloat = 5.15;
  _SWSetSpriteScaledFactor(local_1c[0],local_1c[0]->miscFloat,local_1c[0]->miscFloat);
  return local_1c[0];
}


// ---- _NewSkullSpiderSprite @ 0xa878 (544 bytes)

SpritePtr _NewSkullSpiderSprite(void)

{
  SpritePtr local_2c;
  void *local_28;
  int local_24;
  int local_20;
  
  local_28 = _calloc(1,0x10c);
  if (local_28 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29096_001c4068,0x90e);
  }
  local_20 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 4),&local_2c,local_28,0x10c
                           );
  if (local_20 != 0) {
    _MMFatalErrorCallback
              (local_20,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29096_001c4068,0x911);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gEnemySpriteLayerP_001c40b0,local_2c);
  _SWSetSpriteMoveProc(local_2c,(MoveProcPtr_t)PTR_SkullSpiderMoveProc_001c41b0);
  local_2c->userData = 1;
  _SWSetSpriteFrameAdvanceMode(local_2c,(void *)0x1);
  _SWSetSpriteFrameProc(local_2c,(FrameProcPtr_t)PTR_LAB_001c42bc);
  _SWSetRect((int)local_28 + 0xfc,0xf,0,0xf,0);
  for (local_24 = 0; local_24 < local_2c->maxFrames; local_24 = local_24 + 1) {
    _SWSetFrameCollisionInset
              (local_2c->frameArray[local_24],*(short *)((int)local_28 + 0xfe),
               *(short *)((int)local_28 + 0xfc),*(short *)((int)local_28 + 0x102),
               *(short *)((int)local_28 + 0x100));
  }
  _SWSetSpriteFrameAdvance(local_2c,0);
  _SWSetSpriteFrameTime(local_2c,0);
  *(undefined2 *)((int)local_28 + 0x10a) = 10;
  *(undefined2 *)((int)local_28 + 0x106) = 10;
  *(undefined2 *)((int)local_28 + 0x108) = 1;
  return local_2c;
}


// ---- _NewScorpionSprite @ 0xaa98 (532 bytes)

SpritePtr _NewScorpionSprite(void)

{
  SpritePtr local_2c;
  void *local_28;
  int local_24;
  int local_20;
  
  local_28 = _calloc(1,0x108);
  if (local_28 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29142_001c4210,0x942);
  }
  local_20 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 8),&local_2c,local_28,0x108
                           );
  if (local_20 != 0) {
    _MMFatalErrorCallback
              (local_20,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29142_001c4210,0x945);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gEnemySpriteLayerP_001c40b0,local_2c);
  _SWSetSpriteMoveProc(local_2c,(MoveProcPtr_t)PTR_ScorpionMoveProc_001c40ec);
  local_2c->userData = 2;
  _SWSetSpriteFrameAdvanceMode(local_2c,(void *)0x0);
  _SWSetRect((int)local_28 + 0x100,4,0,4,0);
  for (local_24 = 0; local_24 < local_2c->maxFrames; local_24 = local_24 + 1) {
    _SWSetFrameCollisionInset
              (local_2c->frameArray[local_24],*(short *)((int)local_28 + 0x102),
               *(short *)((int)local_28 + 0x100),*(short *)((int)local_28 + 0x106),
               *(short *)((int)local_28 + 0x104));
  }
  _SWSetSpriteFrameProc(local_2c,(FrameProcPtr_t)PTR_ScorpionFrameProc_001c40c4);
  _SWSetSpriteFrameAdvance(local_2c,0);
  _SWSetSpriteFrameTime(local_2c,0);
  *(undefined2 *)((int)local_28 + 0xfc) = 7;
  *(undefined2 *)((int)local_28 + 0xfe) = 1;
  return local_2c;
}


// ---- _NewEyeballSprite @ 0xacac (524 bytes)

SpritePtr _NewEyeballSprite(void)

{
  int frameIndex;
  SpritePtr local_2c;
  void *local_28;
  int local_24;
  int local_20;
  
  local_28 = _calloc(1,0x17c);
  if (local_28 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29188_001c424c,0x975);
  }
  local_20 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0xc),&local_2c,local_28,
                            0x17c);
  if (local_20 != 0) {
    _MMFatalErrorCallback
              (local_20,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29188_001c424c,0x978);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gEyeballSpriteLayerP_001c41c8,local_2c);
  _SWSetSpriteMoveProc(local_2c,(MoveProcPtr_t)PTR_LAB_001c40bc);
  _SWSetSpriteFrameProc(local_2c,(FrameProcPtr_t)PTR_EyeballFrameProc_001c41e4);
  _SWSetSpriteFrameTime(local_2c,0x53);
  _SWSetSpriteFrameAdvance(local_2c,0);
  local_2c->userData = 3;
  frameIndex = _GetRandom(0,6);
  _SWSetCurrentFrameIndex(local_2c,frameIndex);
  _SWSetRect((int)local_28 + 0xfc,0,0,0,0);
  for (local_24 = 0; local_24 < local_2c->maxFrames; local_24 = local_24 + 1) {
    _SWSetFrameCollisionInset
              (local_2c->frameArray[local_24],*(short *)((int)local_28 + 0xfe),
               *(short *)((int)local_28 + 0xfc),*(short *)((int)local_28 + 0x102),
               *(short *)((int)local_28 + 0x100));
  }
  return local_2c;
}


// ---- _NewRavenSprite @ 0xaeb8 (613 bytes)

SpritePtr _NewRavenSprite(short param_1)

{
  SpritePtr local_2c;
  void *local_28;
  int local_24;
  int local_20;
  
  local_28 = _calloc(1,0x128);
  if (local_28 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29234_001c4038,0x9a1);
  }
  local_24 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_2c,
                            local_28,0x128);
  if (local_24 != 0) {
    _MMFatalErrorCallback
              (local_24,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29234_001c4038,0x9a4);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gBirdSpriteLayerP_001c41c4,local_2c);
  local_2c->userData = (int)param_1;
  if (param_1 == 4) {
    _SWSetSpriteMoveProc(local_2c,(MoveProcPtr_t)PTR_LAB_001c415c);
  }
  else {
    _SWSetSpriteMoveProc(local_2c,(MoveProcPtr_t)PTR_RedRavenMoveProc_001c4238);
  }
  _SWSetSpriteFrameAdvanceMode(local_2c,(void *)0x1);
  _SWSetSpriteFrameProc(local_2c,(FrameProcPtr_t)PTR_BirdFrameProc_001c4184);
  _SWSetSpriteFrameAdvance(local_2c,0);
  _SWSetSpriteFrameTime(local_2c,0);
  *(undefined2 *)((int)local_28 + 0x104) = 6;
  *(undefined2 *)((int)local_28 + 0x106) = 1;
  _SWSetRect((int)local_28 + 0xfc,4,0,6,0);
  for (local_20 = 0; local_20 < local_2c->maxFrames; local_20 = local_20 + 1) {
    _SWSetFrameCollisionInset
              (local_2c->frameArray[local_20],*(short *)((int)local_28 + 0xfe),
               *(short *)((int)local_28 + 0xfc),*(short *)((int)local_28 + 0x102),
               *(short *)((int)local_28 + 0x100));
  }
  *(undefined8 *)((int)local_28 + 0x10c) = 0;
  *(undefined4 *)((int)local_28 + 0x108) = 0xbf800000;
  return local_2c;
}


// ---- _NewSnakeSprite @ 0xb11d (547 bytes)

SpritePtr _NewSnakeSprite(short param_1)

{
  SpritePtr local_2c;
  void *local_28;
  short local_22;
  int local_20;
  
  local_28 = _calloc(1,0x108);
  if (local_28 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29287_001c420c,0x9e4);
  }
  local_20 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_2c,
                            local_28,0x108);
  if (local_20 != 0) {
    _MMFatalErrorCallback
              (local_20,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29287_001c420c,0x9e7);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gSnakeSpriteLayerP_001c4208,local_2c);
  _SWSetSpriteMoveProc(local_2c,(MoveProcPtr_t)PTR_SnakeMoveProc_001c40a8);
  local_2c->userData = (int)param_1;
  _SWSetSpriteFrameAdvanceMode(local_2c,(void *)0x1);
  _SWSetSpriteFrameProc(local_2c,(FrameProcPtr_t)PTR_SnakeFrameProc_001c4100);
  _SWSetSpriteFrameAdvance(local_2c,0);
  _SWSetSpriteFrameTime(local_2c,0);
  *(undefined2 *)((int)local_28 + 0xfc) = 0xc;
  *(undefined2 *)((int)local_28 + 0xfe) = 1;
  _SWSetRect((int)local_28 + 0x100,5,0,5,0xfffffffe);
  for (local_22 = 0; (int)local_22 < local_2c->maxFrames; local_22 = local_22 + 1) {
    _SWSetFrameCollisionInset
              (local_2c->frameArray[local_22],*(short *)((int)local_28 + 0x102),
               *(short *)((int)local_28 + 0x100),*(short *)((int)local_28 + 0x106),
               *(short *)((int)local_28 + 0x104));
  }
  return local_2c;
}


// ---- _NewMedusasHeadSprite @ 0xb340 (450 bytes)

SpritePtr _NewMedusasHeadSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x11c);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29335_001c4204,0xa14);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0x11c);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29335_001c4204,0xa17);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gMedusaSpriteLayerP_001c4094,local_18);
  _SWSetSpriteMoveProc(local_18,(MoveProcPtr_t)PTR_MedusasHeadMoveProc_001c414c);
  _SWSetSpriteCollideProc(local_18,(CollideProcPtr_t)PTR_LAB_001c40e4);
  local_18->userData = (int)param_1;
  _SWSetSpriteFrameAdvanceMode(local_18,(void *)0x1);
  _SWSetSpriteFrameRange(local_18,0,2);
  *(undefined2 *)((int)local_14 + 0x10a) = 0;
  _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_MedusaFrameProc_001c4288);
  _SWSetSpriteFrameAdvance(local_18,0);
  _SWSetSpriteFrameTime(local_18,0);
  *(undefined2 *)((int)local_14 + 0xfc) = 0x14;
  *(undefined2 *)((int)local_14 + 0xfe) = 0x14;
  *(undefined2 *)((int)local_14 + 0x100) = 1;
  *(undefined1 *)((int)local_14 + 0x118) = 1;
  return local_18;
}


// ---- _NewSpiderSprite @ 0xb502 (718 bytes)

SpritePtr _NewSpiderSprite(short param_1)

{
  SpritePtr local_2c;
  void *local_28;
  int local_24;
  int local_20;
  
  local_2c = (SpritePtr)0x0;
  local_28 = _calloc(1,0x140);
  if (local_28 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29364_001c41e8,0xa4b);
  }
  local_20 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_2c,
                            local_28,0x140);
  if (local_20 != 0) {
    _MMFatalErrorCallback
              (local_20,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29364_001c41e8,0xa4e);
  }
  local_2c->userData = (int)param_1;
  _SWSetSpriteFrameProc(local_2c,(FrameProcPtr_t)PTR_SpiderFrameProc_001c4058);
  _SWSetSpriteFrameAdvance(local_2c,0);
  _SWSetSpriteFrameTime(local_2c,0);
  *(undefined2 *)((int)local_28 + 0x114) = 1;
  if (param_1 == 0xb) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBigSpiderSpriteLayerP_001c42a4,local_2c);
    _SWSetSpriteMoveProc(local_2c,(MoveProcPtr_t)PTR_LAB_001c401c);
    _SWSetSpriteCollideProc(local_2c,(CollideProcPtr_t)PTR_BigSpiderCollideProc_001c4254);
    *(undefined2 *)((int)local_28 + 0x110) = 10;
    *(undefined2 *)((int)local_28 + 0x112) = 10;
    *(undefined2 *)((int)local_28 + 0x108) = 4;
  }
  else {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gEnemySpriteLayerP_001c40b0,local_2c);
    _SWSetSpriteMoveProc(local_2c,(MoveProcPtr_t)PTR_SmallSpiderMoveProc_001c427c);
    *(undefined2 *)((int)local_28 + 0x110) = 0xf;
    *(undefined2 *)((int)local_28 + 0x112) = 0xf;
    *(undefined2 *)((int)local_28 + 0x108) = 0;
    _SWSetRect((int)local_28 + 0xfc,0,0,0,2);
    for (local_24 = 0; local_24 < local_2c->maxFrames; local_24 = local_24 + 1) {
      _SWSetFrameCollisionInset
                (local_2c->frameArray[local_24],*(short *)((int)local_28 + 0xfe),
                 *(short *)((int)local_28 + 0xfc),*(short *)((int)local_28 + 0x102),
                 *(short *)((int)local_28 + 0x100));
    }
  }
  _SWSetSpriteFrameAdvanceMode(local_2c,(void *)0x1);
  *(undefined2 *)((int)local_28 + 0x104) = 0xffff;
  *(undefined4 *)((int)local_28 + 0x10c) = 0;
  return local_2c;
}


// ---- _NewSkeletonFishSprite @ 0xb7d0 (334 bytes)

SpritePtr _NewSkeletonFishSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x118);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29418_001c413c,0xa98);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0x118);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29418_001c413c,0xa9b);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gEnemySpriteLayerP_001c40b0,local_18);
  _SWSetSpriteMoveProc(local_18,(MoveProcPtr_t)PTR_LAB_001c4200);
  local_18->userData = (int)param_1;
  _SWSetSpriteFrameAdvance(local_18,0);
  _SWSetSpriteFrameTime(local_18,0);
  *(undefined2 *)((int)local_14 + 0x104) = 1;
  *(undefined2 *)((int)local_14 + 0xfe) = 0;
  *(undefined1 *)((int)local_14 + 0x114) = 0;
  return local_18;
}


// ---- _NewWalkingSkeletonSprite @ 0xb91e (308 bytes)

SpritePtr _NewWalkingSkeletonSprite(void)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x11c);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29442_001c4148,0xabb);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x34),&local_18,local_14,
                            0x11c);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29442_001c4148,0xabe);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gSkeletonBonesSpriteLayerP_001c40a0,local_18);
  local_18->userData = 0xd;
  _SWSetRect((int)local_14 + 0x102,0x12,0,0x16,0);
  *(undefined1 *)((int)local_14 + 0x10a) = 0;
  *(undefined1 *)((int)local_14 + 0x10b) = 0;
  *(undefined2 *)((int)local_14 + 0x118) = 0;
  return local_18;
}


// ---- _NewObjectSprite @ 0xba52 (904 bytes)

SpritePtr _NewObjectSprite(short param_1)

{
  int frameIndex;
  undefined4 uVar1;
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  if ((param_1 < 0x1e) || (0x2c < param_1)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29462_001c4220,0xada);
  }
  local_14 = _calloc(1,0x118);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29462_001c4220,0xadf);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0x118);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29462_001c4220,0xae2);
  }
  _SWSetSpriteCollideProc(local_18,(CollideProcPtr_t)PTR_LAB_001c40b4);
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gObjectSpriteLayerP_001c4268,local_18);
  local_18->userData = (int)param_1;
  frameIndex = _GetRandom(0,local_18->maxFrames + -1);
  _SWSetCurrentFrameIndex(local_18,frameIndex);
  _SWSetRect((int)local_14 + 0xfc,0,0,0,0);
  if (((param_1 == 0x22) || (param_1 == 0x23)) || (param_1 == 0x25)) {
    if (param_1 == 0x25) {
      _MySetSpriteFrameTime(local_18,0x7d);
    }
    else {
      _MySetSpriteFrameTime(local_18,100);
    }
    _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_LAB_001c4198);
    _SWSetSpriteFrameAdvance(local_18,0);
    _SWSetCurrentFrameIndex(local_18,0);
    uVar1 = _GetRandom(0,0x13);
    *(undefined4 *)((int)local_14 + 0x104) = uVar1;
    if (param_1 == 0x23) {
      _SWSetSpriteMoveProc(local_18,(MoveProcPtr_t)PTR_FallingObjectMoveProc_001c41b8);
    }
  }
  else if ((param_1 < 0x27) || (0x2c < param_1)) {
    if ((param_1 < 0x1e) || (0x1f < param_1)) {
      if (param_1 == 0x24) {
        _SWSetSpriteFrameAdvance(local_18,0);
        _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_LAB_001c40a4);
        _MySetSpriteFrameTime(local_18,0x5a);
      }
      else if (param_1 != 0x25) {
        if (param_1 == 0x26) {
          _SWSetSpriteCollideProc(local_18,(CollideProcPtr_t)PTR_LAB_001c40b4);
          _SWRemoveSprite(local_18);
          _SWAddSprite(*(SpriteLayerPtr *)PTR__gSignSpriteLayerP_001c41f8,local_18);
        }
        else {
          _SWSetSpriteFrameAdvanceMode(local_18,(void *)0x0);
          _MySetSpriteFrameTime(local_18,100);
        }
      }
    }
    else {
      _SWSetSpriteMoveProc(local_18,(MoveProcPtr_t)PTR_FallingObjectMoveProc_001c41b8);
    }
  }
  else {
    _SWSetSpriteFrameAdvanceMode(local_18,(void *)0x0);
    _MySetSpriteFrameTime(local_18,0x47);
  }
  *(undefined4 *)((int)local_14 + 0x108) = 0;
  return local_18;
}


// ---- _NewZapperNodeSprite @ 0xbdda (437 bytes)

SpritePtr _NewZapperNodeSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x11c);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29539_001c42d4,0xb32);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0x11c);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29539_001c42d4,0xb35);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gZapperNodeSpriteLayerP_001c418c,local_18);
  local_18->userData = (int)param_1;
  if ((param_1 == 0x1a) || (param_1 == 0x1d)) {
    _SWSetSpriteMoveProc(local_18,(MoveProcPtr_t)PTR_LAB_001c4234);
    _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_ZapperNodeFrameProc_001c4278);
    _SWSetSpriteFrameTime(local_18,100);
  }
  else {
    _SWSetSpriteFrameTime(local_18,100);
  }
  *(undefined1 *)((int)local_14 + 0x105) = 0;
  *(undefined2 *)((int)local_14 + 0x102) = 0;
  *(undefined2 *)((int)local_14 + 0x106) = 0;
  *(undefined2 *)((int)local_14 + 0x108) = 0;
  *(undefined4 *)((int)local_14 + 0x10c) = 0;
  *(undefined4 *)((int)local_14 + 0x110) = 0;
  *(undefined4 *)((int)local_14 + 0x114) = 0;
  *(undefined4 *)((int)local_14 + 0x118) = 0;
  return local_18;
}


// ---- _NewZapperBeamSprite @ 0xbf8f (342 bytes)

SpritePtr _NewZapperBeamSprite(char param_1)

{
  SpritePtr local_1c;
  void *local_18;
  short local_12;
  int local_10;
  
  local_18 = _calloc(1,0x104);
  if (local_18 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29569_001c416c,0xb63);
  }
  if (param_1 == '\0') {
    local_12 = 0xba;
  }
  else {
    local_12 = 0xb9;
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + local_12 * 4),&local_1c,
                            local_18,0x104);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29569_001c416c,0xb6b);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gZapperBeamSpriteLayerP_001c41ec,local_1c);
  _SWSetSpriteMoveProc(local_1c,(MoveProcPtr_t)PTR_LAB_001c4294);
  _SWSetSpriteCollideProc(local_1c,(CollideProcPtr_t)PTR_LAB_001c42c4);
  local_1c->userData = (int)local_12;
  *(char *)((int)local_18 + 0x102) = param_1;
  *(undefined2 *)((int)local_18 + 0x100) = 0;
  *(undefined4 *)((int)local_18 + 0xf8) = 0xffffffff;
  return local_1c;
}


// ---- _NewMovingPlatformSprite @ 0xc0e5 (509 bytes)

SpritePtr _NewMovingPlatformSprite(short param_1)

{
  SpritePtr local_2c;
  void *local_28;
  short local_22;
  int local_20;
  
  local_28 = _calloc(1,0x118);
  if (local_28 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29596_001c4128,0xb8a);
  }
  local_20 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_2c,
                            local_28,0x118);
  if (local_20 != 0) {
    _MMFatalErrorCallback
              (local_20,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29596_001c4128,0xb8d);
  }
  local_2c->userData = (int)param_1;
  if (param_1 == 0x4f) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gMovingPlatformBlockerSpriteLayerP_001c41ac,local_2c);
    _SWSetSpriteVisible(local_2c,'\0');
  }
  _SWSetRect((int)local_28 + 0xfc,1,8,8,0);
  if (param_1 == 0x4e) {
    *(short *)((int)local_28 + 0xfc) = *(short *)((int)local_28 + 0xfc) + 2;
    *(short *)((int)local_28 + 0x102) = *(short *)((int)local_28 + 0x102) + 1;
  }
  for (local_22 = 0; (int)local_22 < local_2c->maxFrames; local_22 = local_22 + 1) {
    _SWSetFrameCollisionInset
              (local_2c->frameArray[local_22],*(short *)((int)local_28 + 0xfe),
               *(short *)((int)local_28 + 0xfc),*(short *)((int)local_28 + 0x102),
               *(short *)((int)local_28 + 0x100));
  }
  *(undefined1 *)((int)local_28 + 0x104) = 1;
  return local_2c;
}


// ---- _NewFallingPlatformSprite @ 0xc2e2 (432 bytes)

SpritePtr _NewFallingPlatformSprite(short param_1)

{
  SpritePtr local_2c;
  void *local_28;
  short local_22;
  int local_20;
  
  local_28 = _calloc(1,0x118);
  if (local_28 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29649_001c40e8,0xc25);
  }
  local_20 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_2c,
                            local_28,0x118);
  if (local_20 != 0) {
    _MMFatalErrorCallback
              (local_20,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29649_001c40e8,0xc28);
  }
  local_2c->userData = (int)param_1;
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gVertMovingPlatformSpriteLayerP_001c423c,local_2c);
  _SWSetRect((int)local_28 + 0xfc,0,8,7,0);
  for (local_22 = 0; (int)local_22 < local_2c->maxFrames; local_22 = local_22 + 1) {
    _SWSetFrameCollisionInset
              (local_2c->frameArray[local_22],*(short *)((int)local_28 + 0xfe),
               *(short *)((int)local_28 + 0xfc),*(short *)((int)local_28 + 0x102),
               *(short *)((int)local_28 + 0x100));
  }
  *(undefined2 *)((int)local_28 + 0x106) = 0xffff;
  return local_2c;
}


// ---- _NewTrapDoorSprite @ 0xc492 (432 bytes)

SpritePtr _NewTrapDoorSprite(short param_1)

{
  SpritePtr local_28;
  void *local_24;
  int local_20;
  
  local_24 = _calloc(1,0x104);
  if (local_24 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29692_001c4244,0xc4f);
  }
  local_20 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_28,
                            local_24,0x104);
  if (local_20 != 0) {
    _MMFatalErrorCallback
              (local_20,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29692_001c4244,0xc52);
  }
  local_28->userData = (int)param_1;
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gTrapDoorSpriteLayerP_001c40e0,local_28);
  if (param_1 == 0x5f) {
    _SWSetRect((int)local_24 + 0xfc,0xc,0xd,8,0);
  }
  else {
    _SWSetRect((int)local_24 + 0xfc,10,0xd,10,0);
  }
  _SWSetFrameCollisionInset
            (*local_28->frameArray,*(short *)((int)local_24 + 0xfe),*(short *)((int)local_24 + 0xfc)
             ,*(short *)((int)local_24 + 0x102),*(short *)((int)local_24 + 0x100));
  return local_28;
}


// ---- _NewDoorSprite @ 0xc642 (452 bytes)

SpritePtr _NewDoorSprite(short param_1)

{
  longdouble extraout_ST0;
  SpritePtr local_1c;
  void *local_18;
  ushort local_14;
  byte local_11;
  int local_10;
  
  if ((param_1 < 0x2d) || (0x44 < param_1)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29731_001c425c,0xc73);
  }
  local_18 = _calloc(1,0x128);
  if (local_18 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29731_001c425c,0xc78);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_1c,
                            local_18,0x128);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29731_001c425c,0xc7b);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gDoorSpriteLayerP_001c41dc,local_1c);
  _SWSetSpriteDrawProc(local_1c,(DrawProcPtr_t)PTR_DoorSpriteDrawProc_001c4044);
  local_1c->userData = (int)param_1;
  local_14 = param_1 - 0x2d;
  local_11 = (byte)local_14 & 1;
  if ((local_14 & 1) == 0) {
    *(undefined1 *)((int)local_18 + 0x105) = 1;
  }
  else {
    *(undefined1 *)((int)local_18 + 0x105) = 0;
  }
  *(undefined2 *)((int)local_18 + 0xfc) = 0;
  *(undefined2 *)((int)local_18 + 0xfe) = 0;
  *(undefined1 *)((int)local_18 + 0x100) = 0;
  *(undefined1 *)((int)local_18 + 0x104) = 0;
  _SWGetSpriteHeight(local_1c);
  *(short *)((int)local_18 + 0x122) = (short)(int)extraout_ST0;
  _InitializeDoorMovementVariables(local_1c);
  return local_1c;
}


// ---- _InitializeDoorMovementVariables @ 0xc806 (161 bytes)

void _InitializeDoorMovementVariables(int param_1)

{
  if (param_1 != 0) {
    *(undefined8 *)(param_1 + 0x108) = 0;
    *(undefined8 *)(param_1 + 0x110) = 0;
    *(undefined8 *)(param_1 + 0x118) = 0;
    if (*(char *)(param_1 + 0x105) == '\0') {
      *(short *)(param_1 + 0x102) = *(short *)(param_1 + 0x122) + -8;
    }
    else {
      *(short *)(param_1 + 0x102) = *(short *)(param_1 + 0x122) + -1;
    }
  }
  return;
}


// ---- _NewLeverSprite @ 0xc8a7 (262 bytes)

SpritePtr _NewLeverSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x118);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29780_001c4114,0xcc2);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0x118);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29780_001c4114,0xcc5);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gLeverSpriteLayerP_001c41d4,local_18);
  local_18->userData = (int)param_1;
  *(undefined1 *)((int)local_14 + 0x104) = 1;
  *(undefined4 *)((int)local_14 + 0x114) = 0;
  return local_18;
}


// ---- _NewBrickSwitchSprite @ 0xc9ad (376 bytes)

SpritePtr _NewBrickSwitchSprite(short param_1)

{
  SpritePtr local_1c;
  void *local_18;
  short local_12;
  int local_10;
  
  local_18 = _calloc(1,0x118);
  if (local_18 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29802_001c40fc,0xce1);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_1c,
                            local_18,0x118);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29802_001c40fc,0xce4);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gBrickSwitchSpriteLayerP_001c4188,local_1c);
  _SWSetSpriteFrameTime(local_1c,0);
  _SWSetSpriteFrameAdvance(local_1c,0);
  local_1c->userData = (int)param_1;
  *(undefined1 *)((int)local_18 + 0x104) = 1;
  for (local_12 = 0; (int)local_12 < local_1c->maxFrames; local_12 = local_12 + 1) {
    _SWSetFrameCollisionInset(local_1c->frameArray[local_12],0,0x10,7,0);
  }
  return local_1c;
}


// ---- _NewBackgroundSprite @ 0xcb25 (643 bytes)

SpritePtr _NewBackgroundSprite(short param_1)

{
  undefined2 uVar1;
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x108);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29838_001c428c,0xd07);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0x108);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29838_001c428c,0xd0a);
  }
  if ((param_1 < 0x70) || (0x71 < param_1)) {
    if ((param_1 < 0x72) || (0x73 < param_1)) {
      if (param_1 == 0x74) {
        _SWSetSpriteMoveProc(local_18,(MoveProcPtr_t)PTR_BigWebMoveProc_001c42cc);
        _SWSetSpriteFrameAdvance(local_18,0);
        *(undefined1 *)((int)local_14 + 0xfc) = 1;
      }
      else if (((param_1 == 0x7f) || (param_1 == 0x75)) || (param_1 == 0x80)) {
        _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_AnimatedPaintingFrameProc_001c4060);
        _SWSetSpriteFrameAdvance(local_18,0);
        _SWSetSpriteFrameAdvanceMode(local_18,(void *)0x1);
        if (param_1 == 0x80) {
          _SWSetSpriteFrameTime(local_18,200);
          uVar1 = _GetRandom(0x23,100);
          *(undefined2 *)((int)local_14 + 0xfe) = uVar1;
          *(undefined2 *)((int)local_14 + 0x100) = 0x41;
          *(undefined2 *)((int)local_14 + 0x102) = 200;
        }
        else {
          _SWSetSpriteFrameTime(local_18,0xfa);
          uVar1 = _GetRandom(8,0x20);
          *(undefined2 *)((int)local_14 + 0xfe) = uVar1;
          *(undefined2 *)((int)local_14 + 0x100) = 8;
          *(undefined2 *)((int)local_14 + 0x102) = 0x28;
        }
      }
    }
    else {
      _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_RandomFlinchFrameProc_001c4090);
      _SWSetSpriteFrameTime(local_18,0x8e);
      _SWSetSpriteFrameAdvance(local_18,0);
    }
  }
  else {
    _SWSetSpriteFrameTime(local_18,1000);
  }
  local_18->userData = (int)param_1;
  return local_18;
}


// ---- _NewPedestalOrStatueSprite @ 0xcda8 (395 bytes)

SpritePtr _NewPedestalOrStatueSprite(short param_1)

{
  SpritePtr local_1c;
  void *local_18;
  short local_12;
  int local_10;
  
  local_18 = _calloc(1,0x108);
  if (local_18 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29891_001c4174,0xd50);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_1c,
                            local_18,0x108);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29891_001c4174,0xd53);
  }
  if (param_1 == 0x65) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gSavePedestalSpriteLayerP_001c4138,local_1c);
    _SWSetSpriteMoveProc(local_1c,(MoveProcPtr_t)PTR_PedestalMoveProc_001c419c);
  }
  else {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gForegroundSpriteLayerP_001c41f0,local_1c);
  }
  for (local_12 = 0; (int)local_12 < local_1c->maxFrames; local_12 = local_12 + 1) {
    _SWSetFrameCollisionInset(local_1c->frameArray[local_12],0,5,2,0);
  }
  local_1c->userData = (int)param_1;
  *(undefined1 *)((int)local_18 + 0x104) = 1;
  return local_1c;
}


// ---- _NewCandleHolderSprite @ 0xcf33 (239 bytes)

SpritePtr _NewCandleHolderSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0xfc);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29931_001c40f8,0xd77);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0xfc);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29931_001c40f8,0xd7a);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gCandleHolderSpriteLayerP_001c417c,local_18);
  local_18->userData = (int)param_1;
  return local_18;
}


// ---- _NewTorchSprite @ 0xd022 (307 bytes)

SpritePtr _NewTorchSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x104);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29952_001c408c,0xd91);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0x104);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29952_001c408c,0xd94);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gTorchSpriteLayerP_001c4144,local_18);
  local_18->userData = (int)param_1;
  _SWSetSpriteFrameAdvance(local_18,0);
  _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_FlameFrameProc_001c4018);
  *(undefined4 *)((int)local_14 + 0xfc) = 0;
  *(undefined4 *)((int)local_14 + 0x100) = 0;
  return local_18;
}


// ---- _NewCandleSprite @ 0xd155 (313 bytes)

SpritePtr _NewCandleSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x100);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29975_001c404c,0xdb2);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0x100);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29975_001c404c,0xdb5);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gCandleSpriteLayerP_001c421c,local_18);
  local_18->userData = (int)param_1;
  _SWSetSpriteFrameAdvance(local_18,0);
  _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_LAB_001c4180);
  _MySetSpriteFrameTime(local_18,0x5a);
  *(undefined4 *)((int)local_14 + 0xfc) = 0;
  return local_18;
}


// ---- _NewFlameSprite @ 0xd28e (292 bytes)

SpritePtr _NewFlameSprite(void)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0xfc);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29999_001c4118,0xdd3);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x304),&local_18,local_14,
                            0xfc);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___29999_001c4118,0xdd6);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gFlameSpriteLayerP_001c4130,local_18);
  local_18->userData = 0xc1;
  _SWSetSpriteFrameAdvance(local_18,0);
  _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_FlameFrameProc_001c4018);
  _MySetSpriteFrameTime(local_18,0x4c);
  return local_18;
}


// ---- _NewPuffOfSmokeSprite @ 0xd3b2 (308 bytes)

SpritePtr _NewPuffOfSmokeSprite(void)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x100);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30021_001c4168,0xdf2);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x2ec),&local_18,local_14,
                            0x100);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30021_001c4168,0xdf5);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gPuffOfSmokeSpriteLayerP_001c40ac,local_18);
  local_18->userData = 0xbb;
  _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_LAB_001c4104);
  _MySetSpriteFrameTime(local_18,100);
  _SWResetSpriteFrameTimer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_18);
  *(undefined1 *)((int)local_14 + 0xfc) = 0;
  return local_18;
}


// ---- _NewLavaSplashSprite @ 0xd4e6 (359 bytes)

SpritePtr _NewLavaSplashSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x100);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30044_001c40c0,0xe13);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0x100);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30044_001c40c0,0xe16);
  }
  if ((param_1 == 0xce) || (param_1 == 0xd0)) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBackLavaSplashSpriteLayerP_001c41cc,local_18);
  }
  else {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gFrontLavaSplashSpriteLayerP_001c4024,local_18);
  }
  local_18->userData = (int)param_1;
  _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_LAB_001c4104);
  _MySetSpriteFrameTime(local_18,0x7d);
  _SWResetSpriteFrameTimer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_18);
  *(undefined1 *)((int)local_14 + 0xfc) = 0;
  return local_18;
}


// ---- _NewBloodOrGooDripSprite @ 0xd64d (329 bytes)

SpritePtr _NewBloodOrGooDripSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x108);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30075_001c4078,0xe38);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0x108);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30075_001c4078,0xe3b);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gBloodDripSpriteLayerP_001c4218,local_18);
  _SWSetSpriteMoveProc(local_18,(MoveProcPtr_t)PTR_BloodDripMoveProc_001c4084);
  _MySetSpriteFrameTime(local_18,100);
  local_18->userData = (int)param_1;
  _SWSetRect((int)local_14 + 0xfc,0,0,0,0);
  return local_18;
}


// ---- _NewBloodOrGooSplashSprite @ 0xd796 (364 bytes)

SpritePtr _NewBloodOrGooSplashSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x108);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30099_001c40dc,0xe56);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0x108);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30099_001c40dc,0xe59);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gBloodDripSpriteLayerP_001c4218,local_18);
  _MySetSpriteFrameTime(local_18,0x47);
  _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_LAB_001c4104);
  _SWResetSpriteFrameTimer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_18);
  local_18->userData = (int)param_1;
  _SWSetRect((int)local_14 + 0xfc,0,0,0,0);
  *(undefined1 *)((int)local_14 + 0x104) = 0;
  return local_18;
}


// ---- _NewSlimeSprite @ 0xd902 (369 bytes)

SpritePtr _NewSlimeSprite(void)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x108);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30125_001c42c8,0xe76);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x300),&local_18,local_14,
                            0x108);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30125_001c42c8,0xe79);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gBloodDripSpriteLayerP_001c4218,local_18);
  _MySetSpriteFrameTime(local_18,0x7d);
  _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_LAB_001c4104);
  _SWResetSpriteFrameTimer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_18);
  local_18->userData = 0xc0;
  _SWSetRect((int)local_14 + 0xfc,0,0,0,0);
  *(undefined1 *)((int)local_14 + 0x104) = 0;
  *(undefined4 *)((int)local_14 + 0xf8) = 0xffffffff;
  return local_18;
}


// ---- _NewStatueSprite @ 0xda73 (282 bytes)

SpritePtr _NewStatueSprite(void)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x118);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30149_001c41f4,0xe98);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x308),&local_18,local_14,
                            0x118);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30149_001c41f4,0xe9b);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gStatueSpriteLayerP_001c4240,local_18);
  local_18->userData = 0xc2;
  *(undefined4 *)((int)local_14 + 0x108) = 0;
  *(undefined2 *)((int)local_14 + 0x10c) = 0;
  *(undefined4 *)((int)local_14 + 0x110) = 0;
  *(undefined4 *)((int)local_14 + 0x114) = 0;
  return local_18;
}


// ---- _NewWebTrappedSprite @ 0xdb8d (419 bytes)

SpritePtr _NewWebTrappedSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x10c);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30168_001c4054,0xeb8);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0x10c);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30168_001c4054,0xebb);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gWebTrappedSpriteLayerP_001c4150,local_18);
  local_18->userData = (int)param_1;
  if (param_1 == 0xc3) {
    _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_LAB_001c42b0);
    _SWSetSpriteFrameTime(local_18,100);
    _SWSetSpriteFrameAdvance(local_18,0);
  }
  else {
    _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_RandomFlinchEnemyFrameProc_001c4280);
    _SWSetSpriteFrameTime(local_18,100);
    _SWSetSpriteFrameAdvance(local_18,0);
  }
  *(undefined2 *)((int)local_14 + 0x104) = 2;
  *(undefined2 *)((int)local_14 + 0x106) = 0;
  *(undefined2 *)((int)local_14 + 0x108) = 2;
  *(undefined2 *)((int)local_14 + 0x10a) = 0;
  return local_18;
}


// ---- _NewCircleOfLightSprite @ 0xdd30 (348 bytes)

SpritePtr _NewCircleOfLightSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x114);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30198_001c40f4,0xee5);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0x114);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30198_001c40f4,0xee8);
  }
  _SWSetSpriteMoveProc(local_18,(MoveProcPtr_t)PTR_LAB_001c4110);
  _SWSetSpriteDrawProc(local_18,*(DrawProcPtr_t *)PTR__gTranslucentMaskDrawProc_001c411c);
  local_18->userData = (int)param_1;
  if (param_1 == 0xc5) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gBottomCircleOfLightSpriteLayerP_001c4290,local_18);
  }
  else {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gTopCircleOfLightSpriteLayerP_001c4014,local_18);
  }
  *(undefined4 *)((int)local_14 + 0x104) = 0;
  *(undefined4 *)((int)local_14 + 0x108) = 0;
  return local_18;
}


// ---- _NewTreasureSprite @ 0xde8c (253 bytes)

SpritePtr _NewTreasureSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x108);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30227_001c4020,0xf0e);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0x108);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30227_001c4020,0xf11);
  }
  if ((0xb0 < param_1) && (param_1 < 0xb5)) {
    _SWSetSpriteMoveProc(local_18,(MoveProcPtr_t)PTR_LAB_001c4264);
  }
  local_18->userData = (int)param_1;
  return local_18;
}


// ---- _NewExitSprite @ 0xdf89 (231 bytes)

SpritePtr _NewExitSprite(void)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x108);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30251_001c4248,0xf2c);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x2dc),&local_18,local_14,
                            0x108);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30251_001c4248,0xf2f);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gExitSpriteLayerP_001c403c,local_18);
  local_18->userData = 0xb7;
  return local_18;
}


// ---- _NewGoldSparkleSprite @ 0xe070 (296 bytes)

SpritePtr _NewGoldSparkleSprite(void)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0x100);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30270_001c409c,0xf47);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x31c),&local_18,local_14,
                            0x100);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30270_001c409c,0xf4a);
  }
  local_18->userData = 199;
  _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_LAB_001c4104);
  _MySetSpriteFrameTime(local_18,0x8e);
  _SWResetSpriteFrameTimer(*(SpriteWorldPtr *)PTR__gSpriteWorldP_001c40d0,local_18);
  *(undefined1 *)((int)local_14 + 0xfc) = 0;
  *(undefined4 *)((int)local_14 + 0xf8) = 0xffffffff;
  return local_18;
}


// ---- _NewGlowSprite @ 0xe198 (344 bytes)

SpritePtr _NewGlowSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,0xfc);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30291_001c402c,0xf68);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,0xfc);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30291_001c402c,0xf6b);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gGlowSpriteLayerP_001c42a0,local_18);
  local_18->userData = (int)param_1;
  _SWSetSpriteFrameAdvance(local_18,0);
  _SWSetSpriteBlendMode(local_18,0x302,1);
  _SWSetSpriteScaledWidthFactor(local_18,1.2);
  _SWSetSpriteScaledHeightFactor(local_18,1.2);
  *(undefined4 *)((int)local_14 + 0xf8) = 0xffffffff;
  return local_18;
}


// ---- _NewMiscSprite @ 0xe2f0 (302 bytes)

SpritePtr _NewMiscSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,300);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30316_001c4284,0xf89);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&local_18,
                            local_14,300);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30316_001c4284,0xf8c);
  }
  local_18->userData = (int)param_1;
  *(undefined4 *)((int)local_14 + 0xf8) = 0xffffffff;
  if (param_1 == 0xe1) {
    _SWAddSprite(*(SpriteLayerPtr *)PTR__gTorchSpriteLayerP_001c4144,local_18);
    _SWSetSpriteFrameProc(local_18,(FrameProcPtr_t)PTR_LAB_001c4258);
    _SWSetSpriteFrameTime(local_18,0x10);
  }
  return local_18;
}


// ---- _NewBatPlatformArrowSprite @ 0xe41e (343 bytes)

SpritePtr NewBatPlatformArrowSprite(short param_1)

{
  SpritePtr pSStack_18;
  void *pvStack_14;
  int iStack_10;
  
  pvStack_14 = _calloc(1,300);
  if (pvStack_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30341_001c4108,0xfc8);
  }
  iStack_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),&pSStack_18,
                             pvStack_14,300);
  if (iStack_10 != 0) {
    _MMFatalErrorCallback
              (iStack_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c"
               ,PTR____FUNCTION___30341_001c4108,0xfcb);
  }
  pSStack_18->userData = (int)param_1;
  *(undefined4 *)((int)pvStack_14 + 0xf8) = 0xffffffff;
  _SWSetSpriteFrameAdvanceMode(pSStack_18,(void *)0x1);
  _SWSetSpriteFrameProc(pSStack_18,(FrameProcPtr_t)PTR_MiscSpriteFrameProc_001c4134);
  _SWSetSpriteFrameAdvance(pSStack_18,0);
  _SWSetSpriteFrameTime(pSStack_18,0);
  *(undefined2 *)((int)pvStack_14 + 0x112) = 0xf;
  *(undefined2 *)((int)pvStack_14 + 0x110) = 0xf;
  *(undefined2 *)((int)pvStack_14 + 0x114) = 1;
  return pSStack_18;
}


// ---- _NewSecretIndicatorSprite @ 0xe575 (286 bytes)

SpritePtr _NewSecretIndicatorSprite(void)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  local_14 = _calloc(1,300);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30364_001c40cc,0xfed);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x2e0),&local_18,local_14,
                            300);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30364_001c40cc,0xff0);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gSecretIndicatorSpriteLayerP_001c40d8,local_18);
  _SWSetSpriteCollideProc(local_18,(CollideProcPtr_t)PTR_LAB_001c41a4);
  _SWSetSpriteVisible(local_18,'\0');
  local_18->userData = 0xb8;
  *(undefined4 *)((int)local_14 + 0xf8) = 0xffffffff;
  return local_18;
}


// ---- _CreateGlowOverSprite @ 0xe693 (481 bytes)

void _CreateGlowOverSprite(int param_1)

{
  short sVar1;
  SpritePtr srcSpriteP;
  undefined4 local_14;
  
  sVar1 = (short)*(undefined4 *)(param_1 + 0xe8);
  if ((sVar1 < 0xa1) || (0xa6 < sVar1)) {
    srcSpriteP = (SpritePtr)_NewGlowSprite(0xca);
    _SWCenterSpriteOverSprite(srcSpriteP,param_1);
    _MySetSpriteFrameTime(srcSpriteP,0x4c);
    _SWSetSpriteFrameProc(srcSpriteP,(FrameProcPtr_t)PTR_FlameFrameProc_001c4018);
    if (sVar1 == 0x24) {
      *(SpritePtr *)(param_1 + 0x108) = srcSpriteP;
      _SWRemoveSprite(srcSpriteP);
      _SWAddSprite(*(SpriteLayerPtr *)PTR__gLanternGlowSpriteLayerP_001c4214,srcSpriteP);
    }
    else {
      if (((sVar1 == 0x9e) || (sVar1 == 0x9f)) || (sVar1 == 0xa0)) {
        *(SpritePtr *)(param_1 + 0x100) = srcSpriteP;
      }
      if (sVar1 == 0x9e) {
        _SWOffsetSprite(srcSpriteP,0.0,-26.0);
      }
      else {
        _SWOffsetSprite(srcSpriteP,0.0,-20.0);
      }
    }
  }
  else {
    if ((sVar1 < 0xa4) || (0xa6 < sVar1)) {
      local_14 = (SpritePtr)_NewGlowSprite(200);
    }
    else {
      local_14 = (SpritePtr)_NewGlowSprite(0xc9);
    }
    _SWCenterSpriteOverSprite(local_14,param_1);
    if ((sVar1 < 0xa4) || (0xa6 < sVar1)) {
      _SWOffsetSprite(local_14,0.0,-12.0);
    }
    else {
      _SWOffsetSprite(local_14,0.0,-6.0);
    }
    *(SpritePtr *)(param_1 + 0xfc) = local_14;
  }
  return;
}


// ---- _NewSmallDigitsSprite @ 0xe874 (195 bytes)

SpritePtr _NewSmallDigitsSprite(short param_1,undefined1 param_2)

{
  SpritePtr newSpriteP;
  
  newSpriteP = (SpritePtr)
               _CreateStatsSpriteClone
                         (*(undefined4 *)PTR__gStatsSpriteArray_001c4230,(int)param_1,0,param_2);
  if (newSpriteP == (SpritePtr)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30419_001c41bc,0x1050);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gFPSDisplaySpriteLayerP_001c41a8,newSpriteP);
  _SWSetSpriteMoveProc(newSpriteP,(MoveProcPtr_t)PTR__SmallDigitsMoveProc_001c4190);
  newSpriteP->userData = 0;
  return newSpriteP;
}


// ---- _NewStatsDigitsSprite @ 0xe937 (195 bytes)

SpritePtr _NewStatsDigitsSprite(short param_1,undefined1 param_2)

{
  SpritePtr newSpriteP;
  
  newSpriteP = (SpritePtr)
               _CreateStatsSpriteClone
                         (*(undefined4 *)PTR__gStatsSpriteArray_001c4230,(int)param_1,0,param_2);
  if (newSpriteP == (SpritePtr)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30434_001c4050,0x106e);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gDigitsStatsSpriteLayerP_001c41c0,newSpriteP);
  _SWSetSpriteMoveProc(newSpriteP,(MoveProcPtr_t)PTR_StatsDigitsMoveProc_001c4158);
  newSpriteP->userData = 0;
  return newSpriteP;
}


// ---- _NewStatsItemSprite @ 0xe9fa (309 bytes)

SpritePtr _NewStatsItemSprite(short param_1)

{
  SpritePtr local_18;
  void *local_14;
  int local_10;
  
  if ((param_1 < 0) || (8 < param_1)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30450_001c4080,0x1082);
  }
  local_14 = _calloc(1,0xfc);
  if (local_14 == (void *)0x0) {
    _MMFatalErrorCallback
              (0xffffffff,
               "/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30450_001c4080,0x1087);
  }
  local_10 = _SWCloneSprite(*(undefined4 *)(PTR__gStatsSpriteArray_001c4230 + 4),&local_18,local_14,
                            0xfc);
  if (local_10 != 0) {
    _MMFatalErrorCallback
              (local_10,"/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30450_001c4080,0x108a);
  }
  _SWAddSprite(*(SpriteLayerPtr *)PTR__gStatsSpriteLayerP_001c4010,local_18);
  _SWSetCurrentFrameIndex(local_18,(int)param_1);
  local_18->userData = (int)param_1;
  return local_18;
}


// ---- _NewButtonSprite @ 0xeb2f (155 bytes)

undefined4 _NewButtonSprite(short param_1,short param_2)

{
  undefined4 uVar1;
  
  if ((*(int *)(PTR__gSpriteArray_001c40d4 + param_1 * 4) == 0) &&
     (*(int *)(PTR__gSpriteArray_001c40d4 + param_1 * 4) == 0)) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources & Headers/NewSprite.c",
               PTR____FUNCTION___30475_001c410c,0x10a2);
  }
  uVar1 = _CloneButtonSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + param_1 * 4),(int)param_2)
  ;
  return uVar1;
}


// ---- _AddButtonToDialog @ 0xebca (788 bytes)

SpritePtr _AddButtonToDialog(short param_1,short param_2,SpriteLayerPtr param_3,short param_4,
                            short param_5)

{
  short sVar1;
  short sVar2;
  SpritePtr newSpriteP;
  undefined2 local_e;
  
  local_e = param_1;
  if ((param_1 == 0xda) || (param_1 == 0xd9)) {
    local_e = param_2;
  }
  newSpriteP = (SpritePtr)_NewButtonSprite((int)param_1,(int)local_e);
  if (newSpriteP != (SpritePtr)0x0) {
    sVar1 = *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38);
    sVar2 = *(short *)(*(int *)PTR__gSpriteWorldP_001c40d0 + 0x38);
    _SWAddSprite(param_3,newSpriteP);
    _SWSetCurrentFrameIndex(newSpriteP,(int)param_2);
    if ((param_1 == 0xda) || (param_1 == 0xd9)) {
      if (param_1 == 0xda) {
        _SetUpButtonSprite(newSpriteP,(int)param_2,(int)(short)(param_2 + 4),(int)param_2,0);
      }
      else {
        _SetUpButtonSprite(newSpriteP,(int)param_2,(int)(short)(param_2 + 5),(int)param_2,0);
      }
      _SetButtonColor(newSpriteP,0xffff,0xffff,0xffff);
      _SetButtonPushedColor(newSpriteP,0x8fff,0x8fff,0x8fff);
      _SetButtonPushedOffset(newSpriteP,1,1);
    }
    else {
      _SetUpButtonSprite(newSpriteP,0,1,1,1);
      _SetButtonColor(newSpriteP,0xffff,0xffff,0xffff);
      _SetButtonHighlightedColor(newSpriteP,0xffff,0xffff,0xffff);
      _SetButtonPushedColor(newSpriteP,0xafff,0xafff,0xafff);
    }
    _InstallButtonProc(newSpriteP,PTR_LAB_001c4034);
    _SWSetSpriteScaledFactor
              (newSpriteP,(float)(int)(sVar1 / 0x280) / 1.75,
               (float)(int)(short)((short)(sVar2 * 0x100008889 >> 0x18) - (sVar2 >> 0xf)) / 1.75);
    _SWSetSpriteLocation(newSpriteP,(float)(int)param_4,(float)(int)param_5);
  }
  return newSpriteP;
}


// ---- _GetButtonHeight @ 0xeede (64 bytes)

int _GetButtonHeight(short param_1)

{
  longdouble extraout_ST0;
  
  _SWGetSpriteHeight(*(SpritePtr *)(PTR__gSpriteArray_001c40d4 + param_1 * 4));
  return (int)(short)(int)extraout_ST0;
}


// ---- _GetButtonWidth @ 0xef1e (64 bytes)

int _GetButtonWidth(short param_1)

{
  longdouble extraout_ST0;
  
  _SWGetSpriteWidth(*(SpritePtr *)(PTR__gSpriteArray_001c40d4 + param_1 * 4));
  return (int)(short)(int)extraout_ST0;
}


// ---- _NewBitmapTextSprite @ 0xef5e (247 bytes)

int _NewBitmapTextSprite(short param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  char *pcVar4;
  
  iVar2 = _CreateBMTextSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x370),(int)param_1);
  _SetBMTextSpriteSpecialChars(iVar2,0x3e," .,!#$%*()-+/?\"\';:`=[]\\@<>^&");
  iVar3 = -1;
  pcVar4 = " .,!#$%*()-+/?\"\';:`=[]\\@<>^&";
  do {
    if (iVar3 == 0) break;
    iVar3 = iVar3 + -1;
    cVar1 = *pcVar4;
    pcVar4 = pcVar4 + 1;
  } while (cVar1 != '\0');
  _SetBMTextSpriteUnicodeChars(iVar2,(int)(short)(~(ushort)iVar3 + 0x3d),s__001174e0);
  _SetBMTextSpriteCharSpacing
            (iVar2,PTR__fontSpacingArray_001c42c0,(int)(short)*(undefined4 *)(iVar2 + 0x84),
             0x3f68f5c3);
  _SetBMTextSpriteVertOffsetForChars(iVar2,".,:;",0x3dcccccd);
  return iVar2;
}


// ---- _NewCustomCheckbox @ 0xf055 (458 bytes)

SpritePtr _NewCustomCheckbox(undefined4 param_1,undefined4 param_2,short param_3)

{
  SpritePtr srcSpriteP;
  SpritePtr srcSpriteP_00;
  longdouble extraout_ST0;
  
  srcSpriteP = (SpritePtr)
               _CloneButtonSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x374),(int)param_3);
  if (srcSpriteP != (SpritePtr)0x0) {
    _SetButtonPushedOffset(srcSpriteP,1,1);
    _AdjustCollisionRectForOffset(srcSpriteP);
    _InstallButtonProc(srcSpriteP,PTR_LAB_001c4034);
    _SetUpCheckboxSprite(srcSpriteP,0,1,2,3,0);
    _SetButtonColor(srcSpriteP,0xffff,0xffff,0x5fff);
    _SetButtonHighlightedColor(srcSpriteP,0x5fff,0xffff,0xffff);
    _SetButtonPushedColor(srcSpriteP,0x9fff,0x9fff,0x1fff);
    _SWSetSpriteScaledFactor(srcSpriteP,param_2,param_2);
    srcSpriteP_00 = (SpritePtr)_NewBitmapTextSprite(0x280);
    if (srcSpriteP_00 != (SpritePtr)0x0) {
      _SetBMTextSpriteText(srcSpriteP_00,param_1);
      _SWGetSpriteHeight(srcSpriteP);
      _SWSetSpriteScaledHeight(srcSpriteP_00,SUB42((float)extraout_ST0 * 0.8,0));
      _SWSetSpriteScaledWidthFactor(srcSpriteP_00,srcSpriteP_00->drawData->scaledHeightFactor);
    }
    _SetButtonSpriteText(srcSpriteP,srcSpriteP_00);
  }
  return srcSpriteP;
}


// ---- _NewKeyInputBox @ 0xf21f (545 bytes)

SpritePtr _NewKeyInputBox(undefined4 param_1,float param_2,short param_3)

{
  SpritePtr srcSpriteP;
  SpritePtr srcSpriteP_00;
  longdouble extraout_ST0;
  
  srcSpriteP = (SpritePtr)
               _CloneButtonSprite(*(undefined4 *)(PTR__gSpriteArray_001c40d4 + 0x378),(int)param_3);
  if (srcSpriteP != (SpritePtr)0x0) {
    _SetButtonPushedOffset(srcSpriteP,1,1);
    _AdjustCollisionRectForOffset(srcSpriteP);
    _InstallButtonProc(srcSpriteP,PTR_LAB_001c4034);
    _SetUpKeyInputBox(srcSpriteP,0,1);
    _SetKeyInputBoxKeysToIgnore(srcSpriteP,1,1);
    _SetButtonColor(srcSpriteP,0xffff,0xffff,0x5fff);
    _SetButtonHighlightedColor(srcSpriteP,0x5fff,0xffff,0xffff);
    _SetButtonPushedColor(srcSpriteP,0x9fff,0x9fff,0x1fff);
    _SWSetSpriteScaledFactor(srcSpriteP,param_2 * 2.2,param_2);
    srcSpriteP_00 = (SpritePtr)_NewBitmapTextSprite(0);
    if (srcSpriteP_00 != (SpritePtr)0x0) {
      _SetBMTextSpriteText(srcSpriteP_00,param_1);
      _SetBMTextSpriteCharSpacing
                (srcSpriteP_00,PTR__fontSpacingArray_001c42c0,(int)(short)srcSpriteP_00->maxFrames,
                 0x3f4ccccd);
      _SWGetSpriteHeight(srcSpriteP);
      _SWSetSpriteScaledHeight(srcSpriteP_00,SUB42((float)extraout_ST0 * 0.52,0));
      _SWSetSpriteScaledWidthFactor(srcSpriteP_00,srcSpriteP_00->drawData->scaledHeightFactor);
    }
    _SetButtonSpriteText(srcSpriteP,srcSpriteP_00);
  }
  return srcSpriteP;
}


