// Ghidra decompilation of functions from original source file:
//   Sources - My Utils/FTGLInterface.cpp

// ---- _FTGLPixmapFont_new @ 0x4567c (88 bytes)

FTGLPixmapFont * _FTGLPixmapFont_new(char *param_1)

{
  FTGLPixmapFont *this;
  
  this = (FTGLPixmapFont *)operator_new(0x78);
  FTGLPixmapFont::FTGLPixmapFont(this,param_1);
  return this;
}


// ---- _FTGLBitmapFont_new @ 0x456d4 (88 bytes)

FTGLBitmapFont * FTGLBitmapFont_new(char *param_1)

{
  FTGLBitmapFont *this;
  
  this = (FTGLBitmapFont *)operator_new(0x78);
  FTGLBitmapFont::FTGLBitmapFont(this,param_1);
  return this;
}


// ---- _FTGLTextureFont_new @ 0x4572c (88 bytes)

FTGLTextureFont * FTGLTextureFont_new(char *param_1)

{
  FTGLTextureFont *this;
  
  this = (FTGLTextureFont *)operator_new(0xb0);
  FTGLTextureFont::FTGLTextureFont(this,param_1);
  return this;
}


// ---- _FTFont_Error @ 0x45784 (26 bytes)

void _FTFont_Error(void)

{
  FTFont::Error();
  return;
}


// ---- _FTFont_FaceSize @ 0x4579e (50 bytes)

undefined1 _FTFont_FaceSize(int *param_1,undefined4 param_2)

{
  undefined1 uVar1;
  
  uVar1 = (**(code **)(*param_1 + 8))(param_1,param_2,0x48);
  return uVar1;
}


// ---- _FTFont_Render @ 0x457d0 (40 bytes)

void _FTFont_Render(int *param_1,undefined4 param_2)

{
  (**(code **)(*param_1 + 0x10))(param_1,param_2);
  return;
}


// ---- _FTFont_GetRenderWidth @ 0x457f8 (102 bytes)

longdouble _FTFont_GetRenderWidth(FTFont *param_1,char *param_2)

{
  float local_24;
  float local_20;
  float local_1c;
  float local_18;
  float local_14;
  float local_10 [3];
  
  FTFont::BBox(param_1,param_2,local_10,&local_14,&local_18,&local_1c,&local_20,&local_24);
  return (longdouble)(local_1c - local_10[0]);
}


// ---- __ZNK6FTFont5ErrorEv @ 0x4585e (20 bytes)

/* FTFont::Error() const */

undefined4 __thiscall FTFont::Error(FTFont *this)

{
  return *(undefined4 *)(this + 0x58);
}


