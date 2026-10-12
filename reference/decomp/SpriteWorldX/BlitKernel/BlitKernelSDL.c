// Ghidra decompilation of functions from original source file:
//   ../SpriteWorldX/BlitKernel/BlitKernelSDL.c

// ---- _BKLoadSurface @ 0x1671e (252 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SDL_Surface * _BKLoadSurface(char *filename)

{
  void *src;
  char *pcVar1;
  undefined4 uVar2;
  SDL_Surface *local_14;
  
  local_14 = (SDL_Surface *)0x0;
  src = (void *)_SDL_RWFromZZIP(filename,"rb");
  if (src != (void *)0x0) {
    pcVar1 = _strstr(filename,".tga");
    if (pcVar1 == (char *)0x0) {
      local_14 = (SDL_Surface *)_IMG_Load_RW(src,0);
      if (local_14 == (SDL_Surface *)0x0) {
        uVar2 = _SDL_GetError();
        _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"IMG_Load %s\n",uVar2);
      }
    }
    else {
      local_14 = _BKLoadTGA_RW(src,0);
      if (local_14 == (SDL_Surface *)0x0) {
        uVar2 = _SDL_GetError();
        _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"SDLLoadTGA %s\n",uVar2);
      }
    }
    (**(code **)((int)src + 0xc))(src);
  }
  return local_14;
}


// ---- _BKLoadTGA_RW @ 0x1681a (1644 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SDL_Surface * _BKLoadTGA_RW(void *src,int freesrc)

{
  undefined1 uVar1;
  int iVar2;
  uint uVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  undefined4 local_84;
  byte local_7b;
  undefined1 local_7a;
  undefined1 local_79;
  undefined1 local_78;
  undefined1 local_77;
  undefined2 local_76;
  ushort local_74;
  byte local_72;
  undefined1 local_71 [4];
  ushort local_6d;
  ushort local_6b;
  byte local_69;
  byte local_68;
  char local_67;
  byte local_66;
  char local_65;
  SDL_Surface *local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined1 *local_50;
  int local_4c;
  int local_48;
  int local_44;
  int local_40;
  ushort local_3c;
  ushort local_3a;
  byte local_38;
  byte local_37;
  undefined2 local_36;
  ushort local_34;
  byte local_31;
  int local_30;
  int local_2c;
  undefined1 *local_28;
  int local_24;
  undefined1 *local_20;
  
  local_64 = (SDL_Surface *)0x0;
  iVar2 = (**(code **)((int)src + 4))(src,&local_65,1,1);
  if ((((iVar2 != 0) && (iVar2 = (**(code **)((int)src + 4))(src,&local_67,1,1), iVar2 != 0)) &&
      (iVar2 = (**(code **)((int)src + 4))(src,&local_66,1,1), iVar2 != 0)) &&
     (iVar2 = (**(code **)((int)src + 4))(src,&local_76,1,5), iVar2 != 0)) {
    local_36 = local_76;
    local_34 = local_74;
    local_31 = local_72;
    iVar2 = (**(code **)((int)src + 4))(src,local_71,1,10);
    if (iVar2 != 0) {
      local_3c = local_6d;
      local_3a = local_6b;
      local_38 = local_69;
      local_37 = local_68;
      if ((((local_66 == 1) || (local_66 == 2)) ||
          ((local_66 == 3 || ((local_66 == 9 || (local_66 == 10)))))) || (local_66 == 0xb)) {
        if (local_65 != '\0') {
                    /* WARNING: Load size is inaccurate */
          (**src)(src,local_65,1);
        }
        if ((local_38 == 0x20) || (local_38 == 0x18)) {
          local_60 = 0xff0000;
          local_5c = 0xff00;
          local_58 = 0xff;
          if (local_38 == 0x20) {
            local_84 = 0xff000000;
          }
          else {
            local_84 = 0;
          }
          local_54 = local_84;
        }
        else {
          if (local_38 != 8) {
            _SDL_SetError("Unsupported Image Depth");
            goto LAB_0001699a;
          }
          local_54 = 0;
          local_58 = 0;
          local_5c = 0;
          local_60 = 0;
        }
        local_64 = (SDL_Surface *)
                   _SDL_CreateRGBSurface
                             (0,local_3c,local_3a,local_38,local_60,local_5c,local_58,local_54);
        if (local_64 != (SDL_Surface *)0x0) {
          if ((local_67 == '\0') || ((local_31 != 0x18 && (local_31 != 0x20)))) {
            if (local_66 == 1) {
              local_2c = *(int *)(*(int *)((int)local_64->format + 4) + 4);
              for (local_48 = 0; local_48 < 0x100; local_48 = local_48 + 1) {
                uVar1 = (undefined1)local_48;
                *(undefined1 *)(local_48 * 4 + local_2c) = uVar1;
                *(undefined1 *)(local_48 * 4 + local_2c + 1) = uVar1;
                *(undefined1 *)(local_48 * 4 + local_2c + 2) = uVar1;
              }
              **(undefined4 **)((int)local_64->format + 4) = 0x100;
            }
          }
          else {
            local_30 = *(int *)(*(int *)((int)local_64->format + 4) + 4);
            for (local_48 = 0; local_48 < (int)(uint)local_34; local_48 = local_48 + 1) {
              iVar2 = (**(code **)((int)src + 4))(src,&local_7a,local_31 >> 3,1);
              if (iVar2 == 0) goto LAB_0001699a;
              *(undefined1 *)(local_48 * 4 + local_30 + 2) = local_7a;
              *(undefined1 *)(local_48 * 4 + local_30 + 1) = local_79;
              *(undefined1 *)(local_48 * 4 + local_30) = local_78;
              if (local_31 == 0x20) {
                *(undefined1 *)(local_48 * 4 + local_30 + 3) = local_77;
              }
            }
            **(uint **)((int)local_64->format + 4) = (uint)local_34;
          }
          if ((local_64->flags >> 1 & 1) != 0) {
            _SDL_LockSurface(local_64);
          }
          local_50 = (undefined1 *)local_64->pixels;
          local_4c = local_64->pitch;
          if ((local_66 >> 3 & 1) == 0) {
            for (local_40 = 0; local_40 < (int)(uint)local_3a; local_40 = local_40 + 1) {
              iVar2 = (**(code **)((int)src + 4))(src,local_50,local_3c,local_38 >> 3);
              if (iVar2 == 0) goto LAB_0001699a;
              local_50 = local_50 + local_4c;
            }
          }
          else {
            for (local_40 = 0; local_40 < (int)(uint)local_3a; local_40 = local_40 + 1) {
              local_28 = local_50;
              for (local_44 = 0; local_44 < (int)(uint)local_3c; local_44 = local_44 + local_24) {
                iVar2 = (**(code **)((int)src + 4))(src,&local_7b,1,1);
                if (iVar2 == 0) goto LAB_0001699a;
                local_24 = (local_7b & 0x7f) + 1;
                if ((char)local_7b < '\0') {
                  local_20 = local_28;
                  iVar2 = (**(code **)((int)src + 4))(src,local_28,1,local_38 >> 3);
                  if (iVar2 == 0) goto LAB_0001699a;
                  local_28 = local_28 + (local_38 >> 3);
                  for (local_48 = 1; local_48 < local_24; local_48 = local_48 + 1) {
                    puVar4 = local_20;
                    puVar5 = local_28;
                    for (uVar3 = (uint)(local_38 >> 3); uVar3 != 0; uVar3 = uVar3 - 1) {
                      *puVar5 = *puVar4;
                      puVar4 = puVar4 + 1;
                      puVar5 = puVar5 + 1;
                    }
                    local_28 = local_28 + (local_38 >> 3);
                  }
                }
                else {
                  iVar2 = (**(code **)((int)src + 4))(src,local_28,local_24,local_38 >> 3);
                  if (iVar2 == 0) goto LAB_0001699a;
                  iVar2 = (uint)local_38 * local_24;
                  if (iVar2 < 0) {
                    iVar2 = iVar2 + 7;
                  }
                  local_28 = local_28 + (iVar2 >> 3);
                }
              }
              local_50 = local_50 + local_4c;
            }
          }
          if ((local_64->flags >> 1 & 1) != 0) {
            _SDL_UnlockSurface(local_64);
          }
          if ((local_37 >> 4 & 1) != 0) {
            _BKFlipHorizontal(local_64);
          }
          if ((local_37 >> 5 & 1) == 0) {
            _BKFlipVertical(local_64);
          }
          if (freesrc != 0) {
            (**(code **)((int)src + 0xc))(src);
          }
          return local_64;
        }
        _SDL_SetError("Could not create surface");
      }
      else {
        _SDL_SetError("Unsupported Image Type");
      }
    }
  }
LAB_0001699a:
  if (local_64 != (SDL_Surface *)0x0) {
    _SDL_FreeSurface(local_64);
  }
  if (freesrc != 0) {
    (**(code **)((int)src + 0xc))(src);
  }
  return (SDL_Surface *)0x0;
}


// ---- _BKCreateSurface @ 0x16e86 (294 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SDL_Surface * _BKCreateSurface(int w,int h,int bpp,SDL_bool video,SDL_bool mask,SDL_bool alpha)

{
  SDL_Surface *pSVar1;
  uint local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  if (video == 0) {
    local_20 = 0;
  }
  else {
    local_20 = 0x8000001;
  }
  if (((bpp == 0x10) || (bpp == 0x20)) || (bpp == 8)) {
    local_1c = 0xff;
    local_18 = 0xff;
    local_14 = 0xff;
    local_10 = 0xff;
  }
  if (alpha == 0) {
    local_10 = 0;
  }
  if (mask != 0) {
    local_20 = local_20 | 0x20000;
  }
  if (alpha != 0) {
    local_20 = local_20 | 0x10000;
  }
  pSVar1 = (SDL_Surface *)
           _SDL_CreateRGBSurface(local_20,w,h,bpp,local_1c,local_18,local_14,local_10);
  if ((pSVar1 != (SDL_Surface *)0x0) && (alpha != 0)) {
    _SDL_SetAlpha(pSVar1,0x10000,0xff);
  }
  return pSVar1;
}


// ---- _BKCreateGLSurface @ 0x16fac (231 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SDL_Surface * _BKCreateGLSurface(int w,int h,int bpp,SDL_bool power2,SDL_bool mask,SDL_bool alpha)

{
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  if (power2 != 0) {
    for (local_14 = 1; local_14 < w; local_14 = local_14 << 1) {
    }
    w = local_14;
    for (local_10 = 1; local_10 < h; local_10 = local_10 << 1) {
    }
    h = local_10;
  }
  if ((mask == 0) && (alpha == 0)) {
    local_18 = (SDL_Surface *)_SDL_CreateRGBSurface(0,w,h,0x18,0xff,0xff00,0xff0000,0);
  }
  else {
    local_18 = (SDL_Surface *)_SDL_CreateRGBSurface(0,w,h,0x20,0xff,0xff00,0xff0000,0xff000000);
  }
  return local_18;
}


// ---- _BKFlipHorizontal @ 0x17093 (283 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int _BKFlipHorizontal(SDL_Surface *surface)

{
  uint pixel;
  uint pixel_00;
  int local_1c;
  int local_18;
  
  if ((surface->flags >> 1 & 1) != 0) {
    _SDL_LockSurface(surface);
  }
  for (local_18 = 0; local_18 < surface->h; local_18 = local_18 + 1) {
    for (local_1c = 0; local_1c < (surface->w + 1) / 2; local_1c = local_1c + 1) {
      pixel = _BKGetPixel(surface,local_1c,local_18);
      pixel_00 = _BKGetPixel(surface,(surface->w - local_1c) + -1,local_18);
      _BKPutPixel(surface,local_1c,local_18,pixel_00);
      _BKPutPixel(surface,(surface->w - local_1c) + -1,local_18,pixel);
    }
  }
  if ((surface->flags >> 1 & 1) != 0) {
    _SDL_UnlockSurface(surface);
  }
  return 0;
}


// ---- _BKFlipVertical @ 0x171ae (281 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int _BKFlipVertical(SDL_Surface *surface)

{
  uint pixel;
  uint pixel_00;
  int local_1c;
  int local_18;
  
  if ((surface->flags >> 1 & 1) != 0) {
    _SDL_LockSurface(surface);
  }
  for (local_1c = 0; local_1c < surface->w; local_1c = local_1c + 1) {
    for (local_18 = 0; local_18 < (surface->h + 1) / 2; local_18 = local_18 + 1) {
      pixel = _BKGetPixel(surface,local_1c,local_18);
      pixel_00 = _BKGetPixel(surface,local_1c,(surface->h - local_18) + -1);
      _BKPutPixel(surface,local_1c,local_18,pixel_00);
      _BKPutPixel(surface,local_1c,(surface->h - local_18) + -1,pixel);
    }
  }
  if ((surface->flags >> 1 & 1) != 0) {
    _SDL_UnlockSurface(surface);
  }
  return 0;
}


// ---- _BKInvertAlphaChannel @ 0x172c7 (332 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int BKInvertAlphaChannel(SDL_Surface *surface)

{
  int iVar1;
  int iStack_20;
  int iStack_1c;
  int iStack_18;
  int iStack_14;
  char *pcStack_10;
  
  if (*(char *)((int)surface->format + 8) == ' ') {
    if ((*(int *)((int)surface->format + 0x18) == -0x1000000) ||
       (*(int *)((int)surface->format + 0x18) == 0xff)) {
      if (*(int *)((int)surface->format + 0x18) == -0x1000000) {
        iStack_14 = 3;
      }
      else {
        iStack_14 = 0;
      }
      if (((surface->flags >> 1 & 1) != 0) && (iVar1 = _SDL_LockSurface(surface), iVar1 < 0)) {
        return -1;
      }
      iStack_1c = surface->h;
      while (iStack_1c = iStack_1c + -1, iStack_1c != -1) {
        iStack_18 = surface->w;
        pcStack_10 = (char *)((int)surface->pixels + iStack_14 + surface->pitch * iStack_1c);
        while (iStack_18 = iStack_18 + -1, iStack_18 != -1) {
          *pcStack_10 = -1 - *pcStack_10;
          pcStack_10 = pcStack_10 + 4;
        }
      }
      if ((surface->flags >> 1 & 1) != 0) {
        _SDL_UnlockSurface(surface);
      }
      iStack_20 = 0;
    }
    else {
      _SDL_SetError("Unsupported surface alpha mask format");
      iStack_20 = -1;
    }
  }
  else {
    _SDL_SetError("Unsupported surface bit depth");
    iStack_20 = -1;
  }
  return iStack_20;
}


// ---- _BKDitherAlphaChannel @ 0x17413 (779 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int BKDitherAlphaChannel(SDL_Surface *surface)

{
  int iVar1;
  int iStack_44;
  int iStack_40;
  int aiStack_3c [5];
  int iStack_28;
  uint uStack_24;
  int iStack_20;
  int iStack_1c;
  int iStack_18;
  void *pvStack_14;
  void *pvStack_10;
  
  if ((surface->flags >> 0x10 & 1) == 0) {
    _SDL_SetError("Surface has no alpha channel");
    iStack_44 = -1;
  }
  else if (*(char *)((int)surface->format + 8) == ' ') {
    if (((surface->flags >> 1 & 1) != 0) && (iVar1 = _SDL_LockSurface(surface), iVar1 < 0)) {
      return 1;
    }
    pvStack_14 = _calloc(surface->w * 2 + 4,4);
    if (pvStack_14 == (void *)0x0) {
      _SDL_SetError("Could not allocate dithering error buffer");
      iStack_44 = -1;
    }
    else {
      aiStack_3c[0] = (int)pvStack_14 + 8;
      aiStack_3c[1] = (int)pvStack_14 + surface->w * 4 + 0x10;
      uStack_24 = 0;
      aiStack_3c[4] = *(uint *)((int)surface->format + 0x18);
      iStack_28 = (uint)*(byte *)((int)surface->format + 0x23) -
                  (uint)*(byte *)((int)surface->format + 0x1f);
      if (*(char *)((int)surface->format + 8) == ' ') {
        pvStack_10 = surface->pixels;
        for (aiStack_3c[3] = 0; aiStack_3c[3] < surface->h; aiStack_3c[3] = aiStack_3c[3] + 1) {
          iStack_20 = 0;
          for (aiStack_3c[2] = 0; aiStack_3c[2] < surface->w; aiStack_3c[2] = aiStack_3c[2] + 1) {
            iStack_1c = (0xff - ((*(uint *)(aiStack_3c[2] * 4 + (int)pvStack_10) & aiStack_3c[4]) >>
                                ((byte)iStack_28 & 0x1f))) + iStack_20;
            if (iStack_1c < 0x80) {
              iStack_40 = 0;
            }
            else {
              iStack_40 = 0xff;
            }
            iStack_18 = iStack_40;
            iVar1 = iStack_1c - iStack_40;
            *(int *)(aiStack_3c[uStack_24] + aiStack_3c[2] * 4 + -8) =
                 *(int *)(aiStack_3c[uStack_24] + aiStack_3c[2] * 4 + -8) + (iVar1 * 3 >> 4);
            *(int *)(aiStack_3c[uStack_24] + aiStack_3c[2] * 4 + -4) =
                 *(int *)(aiStack_3c[uStack_24] + aiStack_3c[2] * 4 + -4) + (iVar1 * 5 >> 4);
            *(int *)(aiStack_3c[uStack_24] + aiStack_3c[2] * 4) = iVar1 >> 4;
            *(uint *)(aiStack_3c[2] * 4 + (int)pvStack_10) =
                 *(uint *)(aiStack_3c[2] * 4 + (int)pvStack_10) & ~aiStack_3c[4] |
                 0xff - iStack_40 << ((byte)iStack_28 & 0x1f) & aiStack_3c[4];
            iStack_20 = (iVar1 * 7 >> 4) + *(int *)(aiStack_3c[uStack_24 == 0] + aiStack_3c[2] * 4);
          }
          pvStack_10 = (void *)(surface->pitch + (int)pvStack_10);
          uStack_24 = (uint)(uStack_24 == 0);
        }
      }
      if ((surface->flags >> 1 & 1) != 0) {
        _SDL_UnlockSurface(surface);
      }
      _free(pvStack_14);
      iStack_44 = 0;
    }
  }
  else {
    _SDL_SetError("Unsupported surface bit depth");
    iStack_44 = -1;
  }
  return iStack_44;
}


// ---- _BKConvertColorKeyToAlphaChannel @ 0x1771e (681 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int _BKConvertColorKeyToAlphaChannel(SDL_Surface *surface,SDL_Color *colorkey)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int local_30;
  int local_28;
  int local_24;
  uint local_20;
  void *local_14;
  void *local_10;
  
  if ((surface->flags >> 0x10 & 1) == 0) {
    _SDL_SetError("Surface has no alpha channel");
    local_30 = -1;
  }
  else if (((surface->flags >> 1 & 1) == 0) || (iVar1 = _SDL_LockSurface(surface), -1 < iVar1)) {
    uVar2 = *(uint *)((int)surface->format + 0x14) |
            *(uint *)((int)surface->format + 0xc) | *(uint *)((int)surface->format + 0x10);
    uVar3 = *(uint *)((int)surface->format + 0x18) &
            0xff << (*(byte *)((int)surface->format + 0x23) & 0x1f);
    if (colorkey == (SDL_Color *)0x0) {
      local_20 = _SDL_MapRGB(surface->format,0,0xff,0);
    }
    else {
      local_20 = _SDL_MapRGB(surface->format,colorkey->r,colorkey->g,colorkey->b);
    }
    if (*(char *)((int)surface->format + 8) == '\x10') {
      local_14 = surface->pixels;
      for (local_24 = 0; local_24 < surface->h; local_24 = local_24 + 1) {
        for (local_28 = 0; local_28 < surface->w; local_28 = local_28 + 1) {
          if ((*(ushort *)(local_28 * 2 + (int)local_14) & uVar2) == (local_20 & uVar2)) {
            *(undefined2 *)(local_28 * 2 + (int)local_14) = 0;
          }
          else {
            *(ushort *)(local_28 * 2 + (int)local_14) =
                 *(ushort *)(local_28 * 2 + (int)local_14) | (ushort)uVar3;
          }
        }
        local_14 = (void *)(surface->pitch + (int)local_14);
      }
    }
    else {
      if (*(char *)((int)surface->format + 8) != ' ') {
        _SDL_SetError("Unsupported surface bit depth");
        return -1;
      }
      local_10 = surface->pixels;
      for (local_24 = 0; local_24 < surface->h; local_24 = local_24 + 1) {
        for (local_28 = 0; local_28 < surface->w; local_28 = local_28 + 1) {
          if ((*(uint *)(local_28 * 4 + (int)local_10) & uVar2) == (local_20 & uVar2)) {
            *(undefined4 *)(local_28 * 4 + (int)local_10) = 0;
          }
          else {
            *(uint *)(local_28 * 4 + (int)local_10) =
                 *(uint *)(local_28 * 4 + (int)local_10) | uVar3;
          }
        }
        local_10 = (void *)(surface->pitch + (int)local_10);
      }
    }
    if ((surface->flags >> 1 & 1) != 0) {
      _SDL_UnlockSurface(surface);
    }
    local_30 = 0;
  }
  else {
    local_30 = 1;
  }
  return local_30;
}


// ---- _BKConvertAlphaChannelToColorKey @ 0x179c7 (632 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int BKConvertAlphaChannelToColorKey(SDL_Surface *surface,SDL_Color *colorkey)

{
  uint uVar1;
  int iVar2;
  byte bVar3;
  int iStack_30;
  int iStack_2c;
  int iStack_28;
  undefined4 uStack_24;
  void *pvStack_14;
  void *pvStack_10;
  
  if ((surface->flags >> 0x10 & 1) == 0) {
    _SDL_SetError("Surface has no alpha channel");
    iStack_30 = -1;
  }
  else if ((*(char *)((int)surface->format + 8) == '\x10') ||
          (*(char *)((int)surface->format + 8) == ' ')) {
    if (((surface->flags >> 1 & 1) == 0) || (iVar2 = _SDL_LockSurface(surface), -1 < iVar2)) {
      uVar1 = *(uint *)((int)surface->format + 0x18);
      bVar3 = *(char *)((int)surface->format + 0x23) - *(char *)((int)surface->format + 0x1f);
      if (colorkey == (SDL_Color *)0x0) {
        uStack_24 = _SDL_MapRGB(surface->format,0,0xff,0);
      }
      else {
        uStack_24 = _SDL_MapRGB(surface->format,colorkey->r,colorkey->g,colorkey->b);
      }
      if (*(char *)((int)surface->format + 8) == '\x10') {
        pvStack_14 = surface->pixels;
        for (iStack_28 = 0; iStack_28 < surface->h; iStack_28 = iStack_28 + 1) {
          for (iStack_2c = 0; iStack_2c < surface->w; iStack_2c = iStack_2c + 1) {
            if ((*(ushort *)(iStack_2c * 2 + (int)pvStack_14) & uVar1) >> (bVar3 & 0x1f) == 0) {
              *(short *)(iStack_2c * 2 + (int)pvStack_14) = (short)uStack_24;
            }
          }
          pvStack_14 = (void *)(surface->pitch + (int)pvStack_14);
        }
      }
      else if (*(char *)((int)surface->format + 8) == ' ') {
        pvStack_10 = surface->pixels;
        for (iStack_28 = 0; iStack_28 < surface->h; iStack_28 = iStack_28 + 1) {
          for (iStack_2c = 0; iStack_2c < surface->w; iStack_2c = iStack_2c + 1) {
            if ((*(uint *)(iStack_2c * 4 + (int)pvStack_10) & uVar1) >> (bVar3 & 0x1f) == 0) {
              *(undefined4 *)(iStack_2c * 4 + (int)pvStack_10) = uStack_24;
            }
          }
          pvStack_10 = (void *)(surface->pitch + (int)pvStack_10);
        }
      }
      if ((surface->flags >> 1 & 1) != 0) {
        _SDL_UnlockSurface(surface);
      }
      iStack_30 = 0;
    }
    else {
      iStack_30 = 1;
    }
  }
  else {
    _SDL_SetError("Unsupported surface bit depth");
    iStack_30 = -1;
  }
  return iStack_30;
}


// ---- _BKAddColorKeyFromMask @ 0x17c3f (603 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int _BKAddColorKeyFromMask(SDL_Surface *surface,SDL_Surface *mask)

{
  uint uVar1;
  uint pixel;
  uint uVar2;
  int local_20;
  int local_1c;
  
  if (((surface->flags >> 0x11 ^ 1) & 1) != 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelSDL.c"
               ,0x338,"surface->flags & 0x00020000");
  }
  if (surface->w != mask->w) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelSDL.c"
               ,0x339,"surface->w == mask->w");
  }
  if (surface->h != mask->h) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelSDL.c"
               ,0x33a,"surface->h == mask->h");
  }
  if ((surface->flags >> 1 & 1) != 0) {
    _SDL_LockSurface(surface);
  }
  if ((mask->flags >> 1 & 1) != 0) {
    _SDL_LockSurface(mask);
  }
  uVar1 = _SDL_MapRGB(mask->format,0,0,0);
  _SDL_MapRGB(mask->format,0xff,0xff,0xff);
  pixel = _SDL_MapRGB(surface->format,0,0xff,0);
  for (local_1c = 0; local_1c < surface->h; local_1c = local_1c + 1) {
    for (local_20 = 0; local_20 < surface->w; local_20 = local_20 + 1) {
      uVar2 = _BKGetPixel(mask,local_20,local_1c);
      if (uVar2 != uVar1) {
        _BKPutPixel(surface,local_20,local_1c,pixel);
      }
    }
  }
  if ((mask->flags >> 1 & 1) != 0) {
    _SDL_UnlockSurface(mask);
  }
  if ((surface->flags >> 1 & 1) != 0) {
    _SDL_UnlockSurface(surface);
  }
  return 0;
}


// ---- _BKAddAlphaChannelFromMask @ 0x17e9a (691 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int _BKAddAlphaChannelFromMask(SDL_Surface *surface,SDL_Surface *mask)

{
  bool bVar1;
  uint uVar2;
  byte local_20;
  byte local_1f;
  byte local_1e [2];
  int local_1c;
  int local_18;
  uint local_14;
  uint local_10;
  
  if (((surface->flags >> 0x10 ^ 1) & 1) != 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelSDL.c"
               ,0x365,"surface->flags & 0x00010000");
  }
  if (surface->w != mask->w) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelSDL.c"
               ,0x366,"surface->w == mask->w");
  }
  if (surface->h != mask->h) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelSDL.c"
               ,0x367,"surface->h == mask->h");
  }
  if ((surface->flags >> 1 & 1) != 0) {
    _SDL_LockSurface(surface);
  }
  if ((mask->flags >> 1 & 1) != 0) {
    _SDL_LockSurface(mask);
  }
  for (local_18 = 0; local_18 < surface->h; local_18 = local_18 + 1) {
    for (local_1c = 0; local_1c < surface->w; local_1c = local_1c + 1) {
      local_10 = _BKGetPixel(mask,local_1c,local_18);
      _SDL_GetRGB(local_10,mask->format,&local_20,&local_1f,local_1e);
      if ((local_20 == local_1f) && (local_1f == local_1e[0])) {
        bVar1 = false;
      }
      else {
        bVar1 = true;
      }
      if (bVar1) {
        ___eprintf("%s:%u: failed assertion `%s\'\n",
                   "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelSDL.c"
                   ,0x375,"grey.r == grey.g && grey.g == grey.b");
      }
      uVar2 = _BKGetPixel(surface,local_1c,local_18);
      local_10 = ((int)(uint)local_1f >> (*(byte *)((int)surface->format + 0x1f) & 0x1f)) <<
                 (*(byte *)((int)surface->format + 0x23) & 0x1f);
      local_14 = uVar2 & ~*(uint *)((int)surface->format + 0x18) |
                 *(uint *)((int)surface->format + 0x18) & local_10;
      _BKPutPixel(surface,local_1c,local_18,local_14);
    }
  }
  if ((mask->flags >> 1 & 1) != 0) {
    _SDL_UnlockSurface(mask);
  }
  if ((surface->flags >> 1 & 1) != 0) {
    _SDL_UnlockSurface(surface);
  }
  return 0;
}


// ---- _BKCollideBoundingBox @ 0x1814d (116 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SDL_bool _BKCollideBoundingBox(SDL_Surface *sa,int ax,int ay,SDL_Surface *sb,int bx,int by)

{
  SDL_bool local_10;
  
  if (sb->w + bx < ax) {
    local_10 = 0;
  }
  else if (sa->w + ax < bx) {
    local_10 = 0;
  }
  else if (sb->h + by < ay) {
    local_10 = 0;
  }
  else if (sa->h + ay < by) {
    local_10 = 0;
  }
  else {
    local_10 = 1;
  }
  return local_10;
}


// ---- _BKTransparentPixel @ 0x181c1 (205 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SDL_bool _BKTransparentPixel(SDL_Surface *surface,int x,int y)

{
  uint uVar1;
  uint uVar2;
  uint local_20;
  
  if ((((x < 0) || (surface->w <= x)) || (y < 0)) || (surface->h <= y)) {
    local_20 = 1;
  }
  else {
    uVar1 = _BKGetPixel(surface,x,y);
    if ((surface->flags >> 0x11 & 1) == 0) {
      if ((surface->flags >> 0x10 & 1) == 0) {
        local_20 = 1;
      }
      else {
        local_20 = (uint)((*(uint *)((int)surface->format + 0x18) & uVar1) == 0);
      }
    }
    else {
      uVar2 = _SDL_MapRGB(surface->format,0,0xff,0);
      local_20 = (uint)(uVar2 == uVar1);
    }
  }
  return local_20;
}


// ---- _BKCollidePixels @ 0x1828e (2495 bytes)

undefined4
_BKCollidePixels(SDL_Surface *param_1,short *param_2,SDL_Surface *param_3,short *param_4,
                short *param_5,short *param_6,float param_7,float param_8,float param_9,
                float param_10,int param_11,int param_12,int param_13,int param_14)

{
  SDL_bool SVar1;
  int iVar2;
  short local_9a;
  short local_96;
  short local_92;
  short local_8e;
  short local_8c;
  short sStack_8a;
  short local_88;
  short sStack_86;
  short local_84;
  short sStack_82;
  short local_80;
  short sStack_7e;
  int local_6c;
  int local_68;
  float local_64;
  float local_60;
  float local_5c;
  float local_58;
  float local_54;
  float local_50;
  float local_4c;
  float local_48;
  float local_44;
  float local_40;
  
  SVar1 = _BKCollideBoundingBox
                    (param_1,(int)param_2[1],(int)*param_2,param_3,(int)param_4[1],(int)*param_4);
  if (((SVar1 != 0) &&
      (((param_1->flags >> 1 & 1) == 0 || (iVar2 = _SDL_LockSurface(param_1), -1 < iVar2)))) &&
     (((param_3->flags >> 1 & 1) == 0 || (iVar2 = _SDL_LockSurface(param_3), -1 < iVar2)))) {
    local_5c = 1.0 / param_7;
    local_58 = 1.0 / param_9;
    local_4c = 1.0 / param_8;
    local_48 = 1.0 / param_10;
    sStack_8a = (short)((uint)*(undefined4 *)param_4 >> 0x10);
    sStack_82 = (short)((uint)*(undefined4 *)param_2 >> 0x10);
    local_9a = sStack_8a;
    if (sStack_8a < sStack_82) {
      local_9a = sStack_82;
    }
    iVar2 = (int)local_9a;
    local_8c = (short)*(undefined4 *)param_4;
    local_84 = (short)*(undefined4 *)param_2;
    local_96 = local_8c;
    if (local_8c < local_84) {
      local_96 = local_84;
    }
    local_68 = (int)local_96;
    sStack_86 = (short)((uint)*(undefined4 *)(param_4 + 2) >> 0x10);
    sStack_7e = (short)((uint)*(undefined4 *)(param_2 + 2) >> 0x10);
    local_92 = sStack_86;
    if (sStack_7e < sStack_86) {
      local_92 = sStack_7e;
    }
    local_88 = (short)*(undefined4 *)(param_4 + 2);
    local_80 = (short)*(undefined4 *)(param_2 + 2);
    local_8e = local_88;
    if (local_80 < local_88) {
      local_8e = local_80;
    }
    if (param_11 == 0) {
      local_44 = (float)(iVar2 - sStack_82) * local_5c + (float)(int)param_5[1];
    }
    else {
      local_44 = ((float)((int)sStack_7e - (int)sStack_82) - (float)(iVar2 - sStack_82)) * local_5c
                 + (float)(int)param_5[1];
      local_5c = -local_5c;
    }
    if (param_12 == 0) {
      local_40 = (float)(iVar2 - sStack_8a) * local_58 + (float)(int)param_6[1];
    }
    else {
      local_40 = ((float)((int)sStack_86 - (int)sStack_8a) - (float)(iVar2 - sStack_8a)) * local_58
                 + (float)(int)param_6[1];
      local_58 = -local_58;
    }
    if (param_13 == 0) {
      local_54 = (float)(local_68 - local_84) * local_4c + (float)(int)*param_5;
    }
    else {
      local_54 = ((float)((int)local_80 - (int)local_84) - (float)(local_68 - local_84)) * local_4c
                 + (float)(int)*param_5;
      local_4c = -local_4c;
    }
    if (param_14 == 0) {
      local_50 = (float)(local_68 - local_8c) * local_48 + (float)(int)*param_6;
    }
    else {
      local_50 = ((float)((int)local_88 - (int)local_8c) - (float)(local_68 - local_8c)) * local_48
                 + (float)(int)*param_6;
      local_48 = -local_48;
    }
    if ((((*(char *)((int)param_1->format + 9) == '\x04') &&
         (*(char *)((int)param_3->format + 9) == '\x04')) &&
        (((byte)(param_1->flags >> 0x10) & 1) == 1)) && (((byte)(param_3->flags >> 0x10) & 1) == 1))
    {
      for (; ((0.0 < local_4c && (local_54 < (float)(int)*param_5)) ||
             ((0.0 < local_48 && (local_50 < (float)(int)*param_6))));
          local_50 = local_50 + local_48) {
        local_68 = local_68 + 1;
        local_54 = local_54 + local_4c;
      }
      for (; ((local_4c < 0.0 && ((float)(int)param_5[2] <= local_54)) ||
             ((local_48 < 0.0 && ((float)(int)param_6[2] <= local_50))));
          local_50 = local_50 + local_48) {
        local_68 = local_68 + 1;
        local_54 = local_54 + local_4c;
      }
      for (; local_68 < local_8e; local_68 = local_68 + 1) {
        local_64 = local_44;
        local_60 = local_40;
        if (((float)param_1->h <= local_54) || (local_6c = iVar2, (float)param_3->h <= local_50))
        break;
        for (; ((0.0 < local_5c && (local_64 < (float)(int)param_5[1])) ||
               ((0.0 < local_58 && (local_60 < (float)(int)param_6[1]))));
            local_60 = local_60 + local_58) {
          local_64 = local_64 + local_5c;
          local_6c = local_6c + 1;
        }
        for (; ((local_5c < 0.0 && ((float)(int)param_5[3] <= local_64)) ||
               ((local_58 < 0.0 && ((float)(int)param_6[3] <= local_60))));
            local_60 = local_60 + local_58) {
          local_6c = local_6c + 1;
          local_64 = local_64 + local_5c;
        }
        for (; local_6c < local_92; local_6c = local_6c + 1) {
          if ((param_1->w <= (int)(short)(int)local_64) || (param_3->w <= (int)(short)(int)local_60)
             ) break;
          if (((*(uint *)((int)param_1->pixels +
                         (short)(int)local_64 * 4 + param_1->pitch * (int)(short)(int)local_54) &
               *(uint *)((int)param_1->format + 0x18)) != 0) &&
             ((*(uint *)((int)param_3->pixels +
                        (short)(int)local_60 * 4 + param_3->pitch * (int)(short)(int)local_50) &
              *(uint *)((int)param_3->format + 0x18)) != 0)) {
            return 1;
          }
          local_64 = local_64 + local_5c;
          local_60 = local_60 + local_58;
        }
        local_54 = local_54 + local_4c;
        local_50 = local_50 + local_48;
      }
    }
    else {
      for (; local_68 < local_8e; local_68 = local_68 + 1) {
        local_64 = local_44;
        local_60 = local_40;
        for (local_6c = iVar2; local_6c < local_92; local_6c = local_6c + 1) {
          SVar1 = _BKTransparentPixel(param_1,(int)local_64,(int)local_54);
          if ((SVar1 == 0) &&
             (SVar1 = _BKTransparentPixel(param_3,(int)local_60,(int)local_50), SVar1 == 0)) {
            return 1;
          }
          local_64 = local_64 + local_5c;
          local_60 = local_60 + local_58;
        }
        local_54 = local_54 + local_4c;
        local_50 = local_50 + local_48;
      }
    }
    if ((param_1->flags >> 1 & 1) != 0) {
      _SDL_UnlockSurface(param_1);
    }
    if ((param_3->flags >> 1 & 1) != 0) {
      _SDL_UnlockSurface(param_3);
    }
  }
  return 0;
}


// ---- _BKPutPixel @ 0x18c4d (272 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _BKPutPixel(SDL_Surface *surface,int x,int y,uint pixel)

{
  uint *puVar1;
  uint uVar2;
  
  if ((((-1 < x) || (x < surface->w)) || (-1 < y)) || (y < surface->h)) {
    uVar2 = (uint)*(byte *)((int)surface->format + 9);
    puVar1 = (uint *)((int)surface->pixels + x * uVar2 + surface->pitch * y);
    if (uVar2 == 2) {
      *(short *)puVar1 = (short)pixel;
    }
    else {
      if (2 < uVar2) goto LAB_00018cfc;
      if (uVar2 == 1) {
        *(char *)puVar1 = (char)pixel;
      }
      else {
        do {
          ___eprintf("%s:%u: failed assertion `%s\'\n",
                     "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelSDL.c"
                     ,0x4f2,"0");
LAB_00018cfc:
          if (uVar2 == 3) {
            *(char *)puVar1 = (char)pixel;
            *(char *)((int)puVar1 + 1) = (char)(pixel >> 8);
            *(char *)((int)puVar1 + 2) = (char)(pixel >> 0x10);
            return;
          }
        } while (uVar2 != 4);
        *puVar1 = pixel;
      }
    }
  }
  return;
}


// ---- _BKGetPixel @ 0x18d5d (289 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

uint _BKGetPixel(SDL_Surface *surface,int x,int y)

{
  uint *puVar1;
  uint uVar2;
  uint local_14;
  
  local_14 = 0;
  if ((((-1 < x) || (x < surface->w)) || (-1 < y)) || (y < surface->h)) {
    uVar2 = (uint)*(byte *)((int)surface->format + 9);
    puVar1 = (uint *)((int)surface->pixels + x * uVar2 + surface->pitch * y);
    if (uVar2 == 2) {
      local_14 = (uint)(ushort)*puVar1;
    }
    else {
      if (2 < uVar2) goto LAB_00018e13;
      if (uVar2 == 1) {
        local_14 = (uint)(byte)*puVar1;
      }
      else {
        do {
          ___eprintf("%s:%u: failed assertion `%s\'\n",
                     "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelSDL.c"
                     ,0x520,"0");
LAB_00018e13:
          if (uVar2 == 3) {
            return (uint)(uint3)*puVar1;
          }
        } while (uVar2 != 4);
        local_14 = *puVar1;
      }
    }
  }
  return local_14;
}


// ---- _BKDrawLine @ 0x18e7e (484 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _BKDrawLine(SDL_Surface *surface,int x1,int y1,int x2,int y2,uint pixel)

{
  bool bVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int local_54;
  int local_50;
  int local_38;
  uint local_34;
  uint local_30;
  int local_2c;
  uint local_28;
  uint local_24;
  int local_20;
  int local_14;
  int local_10;
  
  if (((((-1 < x1) || (-1 < x2)) && ((x1 < surface->w || (x2 < surface->w)))) &&
      ((-1 < y1 || (-1 < y2)))) && ((y1 < surface->h || (y2 < surface->h)))) {
    if (x1 < x2) {
      local_54 = x2 - x1;
    }
    else {
      local_54 = x1 - x2;
    }
    if (y1 < y2) {
      local_50 = y2 - y1;
    }
    else {
      local_50 = y1 - y2;
    }
    bVar1 = local_54 < local_50;
    if (bVar1) {
      local_20 = local_54 * 2 - local_50;
      iVar6 = local_54 - local_50;
      iVar2 = local_54;
      local_54 = local_50;
    }
    else {
      local_20 = local_50 * 2 - local_54;
      iVar6 = local_50 - local_54;
      iVar2 = local_50;
    }
    local_10 = local_54 + 1;
    local_24 = 1;
    local_28 = (uint)bVar1;
    local_30 = 1;
    local_34 = (uint)!bVar1;
    if (x2 < x1) {
      local_34 = -local_34;
      local_30 = 0xffffffff;
    }
    if (y2 < y1) {
      local_28 = -local_28;
      local_24 = 0xffffffff;
    }
    local_38 = x1;
    local_2c = y1;
    for (local_14 = 0; local_14 < local_10; local_14 = local_14 + 1) {
      _BKPutPixel(surface,local_38,local_2c,pixel);
      uVar3 = local_30;
      uVar4 = local_24;
      iVar5 = iVar6;
      if (local_20 < 0) {
        uVar3 = local_34;
        uVar4 = local_28;
        iVar5 = iVar2;
      }
      local_20 = local_20 + iVar5 * 2;
      local_2c = local_2c + uVar4;
      local_38 = local_38 + uVar3;
    }
  }
  return;
}


// ---- _BKDrawOval @ 0x19062 (1220 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void BKDrawOval(SDL_Surface *surface,void *rect,uint pixel,SDL_bool filled)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iStack_50;
  int iStack_4c;
  int iStack_48;
  int iStack_44;
  int iStack_40;
  
  if (((rect != (void *)0x0) && (*(int *)((int)rect + 8) != 0)) && (*(int *)((int)rect + 0xc) != 0))
  {
                    /* WARNING: Load size is inaccurate */
    iVar3 = *rect;
    iVar4 = *(int *)((int)rect + 4);
    iStack_50 = *(int *)((int)rect + 8) + iVar3 + -1;
    iStack_4c = *(int *)((int)rect + 0xc) + iVar4 + -1;
    if (((((-1 < iVar3) || (-1 < iStack_50)) && ((iVar3 < surface->w || (iStack_50 < surface->w))))
        && ((-1 < iVar4 || (-1 < iStack_4c)))) && ((iVar4 < surface->h || (iStack_4c < surface->h)))
       ) {
      iVar1 = (iVar3 + iStack_50) / 2;
      iVar2 = (iVar4 + iStack_4c) / 2;
      if (iVar3 < iStack_50) {
        iStack_50 = iStack_50 - iVar3;
      }
      else {
        iStack_50 = iVar3 - iStack_50;
      }
      iStack_50 = iStack_50 / 2;
      if (iVar4 < iStack_4c) {
        iStack_4c = iStack_4c - iVar4;
      }
      else {
        iStack_4c = iVar4 - iStack_4c;
      }
      iStack_4c = iStack_4c / 2;
      iVar3 = iStack_50 * iStack_50;
      iVar4 = iStack_4c * iStack_4c;
      iStack_40 = iVar4 * 2 + (iStack_4c * -2 + 1) * iVar3;
      iStack_44 = iStack_4c;
      for (iStack_48 = 0; iStack_48 * iVar4 < iStack_44 * iVar3; iStack_48 = iStack_48 + 1) {
        if (filled == 0) {
          _BKPutPixel(surface,iStack_48 + iVar1,iStack_44 + iVar2,pixel);
          _BKPutPixel(surface,iVar1 - iStack_48,iStack_44 + iVar2,pixel);
          _BKPutPixel(surface,iStack_48 + iVar1,iVar2 - iStack_44,pixel);
          _BKPutPixel(surface,iVar1 - iStack_48,iVar2 - iStack_44,pixel);
        }
        else {
          _BKDrawLine(surface,iStack_48 + iVar1,iStack_44 + iVar2,iVar1 - iStack_48,
                      iStack_44 + iVar2,pixel);
          _BKDrawLine(surface,iStack_48 + iVar1,iVar2 - iStack_44,iVar1 - iStack_48,
                      iVar2 - iStack_44,pixel);
        }
        if (-1 < iStack_40) {
          iStack_40 = iStack_40 + (iVar3 - iStack_44 * iVar3) * 4;
          iStack_44 = iStack_44 + -1;
        }
        iStack_40 = iStack_40 + iVar4 * 6 + iStack_48 * iVar4 * 4;
      }
      iStack_40 = iVar3 * 2 + (iStack_50 * -2 + 1) * iVar4;
      iStack_48 = iStack_50;
      for (iStack_44 = 0;
          iStack_44 * iVar3 - iStack_48 * iVar4 == 0 || iStack_44 * iVar3 < iStack_48 * iVar4;
          iStack_44 = iStack_44 + 1) {
        if (filled == 0) {
          _BKPutPixel(surface,iStack_48 + iVar1,iStack_44 + iVar2,pixel);
          _BKPutPixel(surface,iVar1 - iStack_48,iStack_44 + iVar2,pixel);
          _BKPutPixel(surface,iStack_48 + iVar1,iVar2 - iStack_44,pixel);
          _BKPutPixel(surface,iVar1 - iStack_48,iVar2 - iStack_44,pixel);
        }
        else {
          _BKDrawLine(surface,iStack_48 + iVar1,iStack_44 + iVar2,iVar1 - iStack_48,
                      iStack_44 + iVar2,pixel);
          _BKDrawLine(surface,iStack_48 + iVar1,iVar2 - iStack_44,iVar1 - iStack_48,
                      iVar2 - iStack_44,pixel);
        }
        if (-1 < iStack_40) {
          iStack_40 = iStack_40 + (iVar4 - iStack_48 * iVar4) * 4;
          iStack_48 = iStack_48 + -1;
        }
        iStack_40 = iStack_40 + iVar3 * 6 + iStack_44 * iVar3 * 4;
      }
    }
  }
  return;
}


