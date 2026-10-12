// Ghidra decompilation of functions from original source file:
//   Sources - My Utils/TextSprite.c

// ---- _LoadSystemFont @ 0x45872 (86 bytes)

void _LoadSystemFont(undefined4 param_1)

{
  char local_19c [404];
  
  _sprintf(local_19c,"%s%s","/Library/Fonts/",param_1);
  _LoadFontUsingPath(local_19c);
  return;
}


// ---- _LoadFontUsingPath @ 0x458c8 (660 bytes)

undefined4 _LoadFontUsingPath(char *param_1)

{
  short sVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 local_660;
  char local_64e [400];
  char local_4be [400];
  char local_32e [400];
  char local_19e [400];
  short local_e;
  
  _sprintf(local_19e,"%s.ttf",param_1);
  _sprintf(local_4be,"%s.ttc",param_1);
  _sprintf(local_64e,"%s.dfont",param_1);
  _strcpy(local_32e,param_1);
  local_e = _FindFTFont(param_1);
  if (local_e == -1) {
    if (*(short *)PTR__gNumFTFontsLoaded_001c4aa4 < 100) {
      sVar1 = *(short *)PTR__gNumFTFontsLoaded_001c4aa4;
      uVar2 = _FTGLPixmapFont_new(local_19e);
      *(undefined4 *)(PTR__gFTFonts_001c4a9c + sVar1 * 4) = uVar2;
      iVar3 = _FTFont_Error(*(undefined4 *)
                             (PTR__gFTFonts_001c4a9c + *(short *)PTR__gNumFTFontsLoaded_001c4aa4 * 4
                             ));
      if (iVar3 != 0) {
        sVar1 = *(short *)PTR__gNumFTFontsLoaded_001c4aa4;
        uVar2 = _FTGLPixmapFont_new(local_4be);
        *(undefined4 *)(PTR__gFTFonts_001c4a9c + sVar1 * 4) = uVar2;
      }
      iVar3 = _FTFont_Error(*(undefined4 *)
                             (PTR__gFTFonts_001c4a9c + *(short *)PTR__gNumFTFontsLoaded_001c4aa4 * 4
                             ));
      if (iVar3 != 0) {
        sVar1 = *(short *)PTR__gNumFTFontsLoaded_001c4aa4;
        uVar2 = _FTGLPixmapFont_new(local_64e);
        *(undefined4 *)(PTR__gFTFonts_001c4a9c + sVar1 * 4) = uVar2;
      }
      iVar3 = _FTFont_Error(*(undefined4 *)
                             (PTR__gFTFonts_001c4a9c + *(short *)PTR__gNumFTFontsLoaded_001c4aa4 * 4
                             ));
      if (iVar3 != 0) {
        sVar1 = *(short *)PTR__gNumFTFontsLoaded_001c4aa4;
        uVar2 = _FTGLPixmapFont_new(local_32e);
        *(undefined4 *)(PTR__gFTFonts_001c4a9c + sVar1 * 4) = uVar2;
      }
      iVar3 = _FTFont_Error(*(undefined4 *)
                             (PTR__gFTFonts_001c4a9c + *(short *)PTR__gNumFTFontsLoaded_001c4aa4 * 4
                             ));
      if (iVar3 == 0) {
        _strcpy(PTR__gFTFontNames_001c4aa8 + *(short *)PTR__gNumFTFontsLoaded_001c4aa4 * 0x32,
                param_1);
        *(short *)PTR__gNumFTFontsLoaded_001c4aa4 = *(short *)PTR__gNumFTFontsLoaded_001c4aa4 + 1;
        local_660 = *(undefined4 *)
                     (PTR__gFTFonts_001c4a9c + (*(short *)PTR__gNumFTFontsLoaded_001c4aa4 + -1) * 4)
        ;
      }
      else {
        local_660 = 0;
      }
    }
    else {
      local_660 = 0;
    }
  }
  else {
    local_660 = *(undefined4 *)(PTR__gFTFonts_001c4a9c + local_e * 4);
  }
  return local_660;
}


// ---- _LoadFontSafe @ 0x45b5c (162 bytes)

int _LoadFontSafe(undefined4 param_1)

{
  undefined4 uVar1;
  int local_10;
  
  uVar1 = _GetFontsFolderPath(param_1);
  local_10 = _LoadFontUsingPath(uVar1);
  if (local_10 == 0) {
    local_10 = _LoadSystemFont(param_1);
  }
  if (local_10 == 0) {
    local_10 = _LoadSystemFont("Arial");
  }
  if (local_10 == 0) {
    local_10 = _LoadSystemFont("Georgia");
  }
  if (local_10 == 0) {
    local_10 = _LoadSystemFont("Verdana");
  }
  if (local_10 == 0) {
    local_10 = _LoadSystemFont("Comic Sans MS");
  }
  return local_10;
}


// ---- _FontExists @ 0x45bfe (67 bytes)

undefined1 _FontExists(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = _LoadFontUsingPath(param_1);
  if ((iVar1 == 0) && (iVar1 = _LoadSystemFont(param_1), iVar1 == 0)) {
    return 0;
  }
  return 1;
}


// ---- _FindFTFont @ 0x45c41 (114 bytes)

int _FindFTFont(undefined4 param_1)

{
  char cVar1;
  short local_e;
  
  local_e = 0;
  while( true ) {
    if (*(short *)PTR__gNumFTFontsLoaded_001c4aa4 <= local_e) {
      return -1;
    }
    cVar1 = _CompareStrings(param_1,PTR__gFTFontNames_001c4aa8 + local_e * 0x32);
    if (cVar1 != '\0') break;
    local_e = local_e + 1;
  }
  return (int)local_e;
}


// ---- _CompareStrings @ 0x45cb3 (85 bytes)

undefined4 _CompareStrings(int param_1,int param_2)

{
  short local_e;
  
  local_e = 0;
  while( true ) {
    if (*(char *)(local_e + param_2) == '\0') {
      return 1;
    }
    if (*(char *)(local_e + param_1) != *(char *)(local_e + param_2)) break;
    local_e = local_e + 1;
  }
  return 0;
}


// ---- _SWCreateTextSprite @ 0x45d08 (448 bytes)

SWError _SWCreateTextSprite(SpritePtr *param_1,char *param_2,undefined4 param_3,short param_4,
                           undefined2 param_5,undefined4 *param_6,undefined1 param_7)

{
  char cVar1;
  void *spriteStorage;
  undefined4 uVar2;
  int iVar3;
  SWError local_3c;
  int local_24;
  
  spriteStorage = _calloc(1,0x308);
  if (spriteStorage == (void *)0x0) {
    local_3c = -1;
  }
  else {
    local_3c = _SWCreateSprite(param_1,spriteStorage,1);
    if (local_3c == 0) {
      for (local_24 = 0;
          (local_24 < 500 &&
          (*(char *)(local_24 + 0xf8 + (int)spriteStorage) = param_2[local_24],
          param_2[local_24] != '\0')); local_24 = local_24 + 1) {
      }
      _SWSetSpriteDrawProc(*param_1,(DrawProcPtr_t)PTR__SWTextSpriteDrawProc_001c4aac);
      uVar2 = _LoadFontSafe(param_3);
      *(undefined4 *)((int)spriteStorage + 0x2ec) = uVar2;
      if (*(int *)((int)spriteStorage + 0x2ec) == 0) {
        _SWDisposeSprite(param_1);
        local_3c = -1;
      }
      else {
        *(short *)((int)spriteStorage + 0x2f0) = param_4;
        *(undefined2 *)((int)spriteStorage + 0x2f2) = param_5;
        *(undefined4 *)((int)spriteStorage + 0x303) = *param_6;
        *(undefined1 *)((int)spriteStorage + 0x307) = param_7;
        *(undefined1 *)((int)spriteStorage + 0x2f4) = 0;
        ((*param_1)->destFrameRect).bottom = ((*param_1)->destFrameRect).bottom + param_4;
        iVar3 = -1;
        do {
          if (iVar3 == 0) break;
          iVar3 = iVar3 + -1;
          cVar1 = *param_2;
          param_2 = param_2 + 1;
        } while (cVar1 != '\0');
        ((*param_1)->destFrameRect).right =
             (~(ushort)iVar3 - 1) * param_4 + ((*param_1)->destFrameRect).right;
        (*param_1)->userData = 0x8dac7c;
      }
    }
  }
  return local_3c;
}


// ---- _SetTextSpriteText @ 0x45ec8 (80 bytes)

void _SetTextSpriteText(int param_1,int param_2)

{
  undefined2 local_e;
  
  for (local_e = 0;
      (local_e < 500 &&
      (*(undefined1 *)(local_e + 0xf8 + param_1) = *(undefined1 *)(local_e + param_2),
      *(char *)(local_e + param_2) != '\0')); local_e = local_e + 1) {
  }
  return;
}


// ---- _SetTextSpriteRect @ 0x45f18 (184 bytes)

void _SetTextSpriteRect(SpritePtr param_1,undefined4 *param_2,undefined2 param_3,uchar param_4)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  *(undefined1 *)&param_1[1].miscFloat = 1;
  uVar1 = param_2[1];
  *(undefined4 *)((int)&param_1[1].miscFloat + 2) = *param_2;
  param_1[1].f_0fa = (char)uVar1;
  param_1[1].f_0fb = (char)((uint)uVar1 >> 8);
  param_1[1].f_0fc = (char)((uint)uVar1 >> 0x10);
  param_1[1].f_0fd = (char)((uint)uVar1 >> 0x18);
  *(short *)&param_1[1].f_100 = *(short *)((int)param_2 + 6) - *(short *)((int)param_2 + 2);
  param_1[1].f_0fe = (undefined1)param_3;
  param_1[1].f_0ff = param_3._1_1_;
  param_1[1].f_102 = param_4;
  uVar1 = param_2[1];
  uVar2 = *param_2;
  (param_1->destFrameRect).top = (short)uVar2;
  (param_1->destFrameRect).left = (short)((uint)uVar2 >> 0x10);
  (param_1->destFrameRect).bottom = (short)uVar1;
  (param_1->destFrameRect).right = (short)((uint)uVar1 >> 0x10);
  _SWSetSpriteLocation
            (param_1,(float)(int)*(short *)((int)param_2 + 2),(float)(int)*(short *)(param_2 + 1));
  return;
}


// ---- _SWTextSpriteDrawProc @ 0x45fd0 (1421 bytes)

void _SWTextSpriteDrawProc(int *param_1)

{
  longdouble lVar1;
  float local_40;
  float local_38;
  float local_34;
  undefined4 local_30;
  undefined4 local_2c;
  int *local_28;
  float local_24;
  SpriteWorldRec *local_20;
  short local_1a;
  int local_18;
  float local_14;
  float local_10;
  
  local_34 = 1.0;
  local_38 = 1.0;
  local_18 = param_1[0x38];
  if (param_1[0x3a] != 0x8dac7c) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/TextSprite.c",
               PTR____FUNCTION___22522_001c4aa0,0x145);
  }
  if (*(int *)(local_18 + 0x14) != 0) {
    if (*param_1 == 0) {
      local_20 = *(SpriteWorldRec **)PTR__gSWCurrentSpriteWorld_001c439c;
    }
    else {
      local_20 = *(SpriteWorldRec **)*param_1;
    }
    local_28 = param_1;
    if (local_20 != (SpriteWorldRec *)0x0) {
      _SWGetWindowScalingFactor(local_20,&local_34,&local_38);
    }
    _glDisable(*(undefined4 *)PTR__gSWTexExtension_001c4550);
    _glPushMatrix();
    _glLoadIdentity();
    if (*(int *)PTR__gSWCurrentSpriteWorld_001c439c == 0) {
      (**(code **)PTR__gSWAssertFailProc_001c4170)
                ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/TextSprite.c",
                 PTR____FUNCTION___22522_001c4aa0,0x15c);
    }
    _glScalef(*(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x3c),
              *(undefined4 *)(*(int *)PTR__gSWCurrentSpriteWorld_001c439c + 0x40),0x3f800000);
    _FTFont_FaceSize(local_28[0xbb],(int)(short)local_28[0xbc]);
    if ((char)local_28[0xbd] == '\0') {
      local_30 = *(undefined4 *)((int)param_1 + 0x36);
      local_2c._0_2_ = (short)*(undefined4 *)((int)param_1 + 0x3a);
      local_2c = CONCAT22((short)((uint)*(undefined4 *)((int)param_1 + 0x3a) >> 0x10),
                          (short)local_2c - (short)local_28[0xbc]);
    }
    else {
      local_2c = *(undefined4 *)((int)local_28 + 0x2fa);
      local_30 = *(undefined4 *)((int)local_28 + 0x2f6);
      lVar1 = (longdouble)_FTFont_GetRenderWidth(local_28[0xbb],local_28 + 0x3e);
      local_24 = (float)lVar1;
      if (*(short *)((int)local_28 + 0x2fe) == 1) {
        local_30 = CONCAT22((short)(int)(((float)(int)(short)local_28[0xc0] - local_24) +
                                        (float)(int)local_30._2_2_),(short)local_30);
      }
      else if (*(short *)((int)local_28 + 0x2fe) == 2) {
        local_30 = CONCAT22((short)(int)((((float)(int)(short)local_28[0xc0] + local_24) / 2.0 -
                                         local_24) + (float)(int)local_30._2_2_),(short)local_30);
      }
    }
    if (*(char *)((int)local_28 + 0x302) == '\0') {
      local_1a = (short)local_2c;
    }
    else {
      local_1a = (short)local_30;
    }
    if ((local_34 != 1.0) || (NAN(local_34))) {
      if (local_34 < local_38) {
        local_40 = local_34;
      }
      else {
        local_40 = local_38;
      }
      local_14 = local_40;
      local_40 = (float)(int)(short)local_28[0xbc] * local_40;
      if (local_40 <= 0.0) {
        local_40 = 0.0;
      }
      _FTFont_FaceSize(local_28[0xbb],
                       (int)(local_40 - (float)(-(uint)(2.1474836e+09 <= local_40) & 0x4f000000)) +
                       (uint)(2.1474836e+09 <= local_40) * -0x80000000 |
                       -(uint)(4.2949673e+09 <= local_40));
    }
    if (*(char *)((int)local_28 + 0x307) != '\0') {
      local_10 = (float)(int)(short)local_28[0xbc] / 9.0;
      _glColor4ub(0,0,0,*(uint *)(local_18 + 0x14) >> 8 & 0xff);
      _glRasterPos2f((float)(int)local_30._2_2_ + local_10,(float)(int)local_1a + local_10);
      _FTFont_Render(local_28[0xbb],local_28 + 0x3e);
    }
    _glColor4ub(*(undefined1 *)((int)local_28 + 0x303),(char)local_28[0xc1],
                *(undefined1 *)((int)local_28 + 0x305),*(uint *)(local_18 + 0x14) >> 8 & 0xff);
    _glRasterPos2f((float)(int)local_30._2_2_,(float)(int)local_1a);
    _FTFont_Render(local_28[0xbb],local_28 + 0x3e);
    if ((*(short *)((int)local_28 + 0x2f2) == 1) || (*(short *)((int)local_28 + 0x2f2) == 2)) {
      _glRasterPos2f((float)(int)local_30._2_2_ - 0.5,(float)(int)local_1a - 0.5);
      _FTFont_Render(local_28[0xbb],local_28 + 0x3e);
    }
    if (*(short *)((int)local_28 + 0x2f2) == 2) {
      _glRasterPos2f((float)(local_30._2_2_ + -1),(float)(local_1a + -1));
      _FTFont_Render(local_28[0xbb],local_28 + 0x3e);
    }
    _glEnable(*(undefined4 *)PTR__gSWTexExtension_001c4550);
    _glPopMatrix();
  }
  return;
}


// ---- _GetTextSpriteWidth @ 0x4655d (179 bytes)

int _GetTextSpriteWidth(int param_1)

{
  int iVar1;
  longdouble lVar2;
  int local_24;
  
  iVar1 = *(int *)(param_1 + 0x2ec);
  if (*(int *)(param_1 + 0xe8) != 0x8dac7c) {
    (**(code **)PTR__gSWAssertFailProc_001c4170)
              ("/Users/vernjensen/Simple - Midnight Mansion/Sources - My Utils/TextSprite.c",
               PTR____FUNCTION___22687_001c4ab0,0x1e6);
  }
  if (iVar1 == 0) {
    local_24 = -1;
  }
  else {
    _FTFont_FaceSize(iVar1,(int)*(short *)(param_1 + 0x2f0));
    lVar2 = (longdouble)_FTFont_GetRenderWidth(iVar1,param_1 + 0xf8);
    local_24 = (int)(short)(int)lVar2;
  }
  return local_24;
}


// ---- _GetTextSpriteRenderWidth @ 0x46610 (102 bytes)

int _GetTextSpriteRenderWidth(undefined4 param_1,short param_2,undefined4 param_3)

{
  int iVar1;
  longdouble lVar2;
  undefined4 local_28;
  
  iVar1 = _LoadFontSafe(param_1);
  if (iVar1 == 0) {
    local_28 = -1;
  }
  else {
    _FTFont_FaceSize(iVar1,(int)param_2);
    lVar2 = (longdouble)_FTFont_GetRenderWidth(iVar1,param_3);
    local_28 = (int)(short)(int)lVar2;
  }
  return local_28;
}


