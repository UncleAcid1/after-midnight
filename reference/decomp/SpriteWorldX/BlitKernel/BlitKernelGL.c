// Ghidra decompilation of functions from original source file:
//   ../SpriteWorldX/BlitKernel/BlitKernelGL.c

// ---- _power2 @ 0x19526 (39 bytes)

uint _power2(uint param_1)

{
  undefined4 local_10;
  
  for (local_10 = 1; local_10 < param_1; local_10 = local_10 << 1) {
  }
  return local_10;
}


// ---- _BKCreateNewGLSurface @ 0x1954d (4242 bytes)

/* WARNING: Restarted to delay deadcode elimination for space: stack */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * _BKCreateNewGLSurface(SDL_Surface *surface,void *rect)

{
  void *pvVar1;
  short sVar2;
  undefined4 uVar3;
  uint uVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  undefined4 *local_104;
  char *local_100;
  int local_fc;
  char *local_f8;
  char *local_f4;
  char *local_f0;
  char *local_ec;
  char *local_e8;
  char *local_e4;
  undefined4 local_e0;
  undefined1 local_d4;
  undefined1 local_d3;
  undefined1 local_d2;
  undefined1 local_d1;
  SDL_Color local_d0;
  undefined4 local_cc;
  undefined4 local_c8;
  int local_c4;
  int local_c0;
  undefined4 local_bc;
  undefined4 local_b8;
  int local_b4;
  int local_b0;
  undefined4 *local_ac;
  SDL_Surface *local_a8;
  void *local_a4;
  int *local_a0;
  undefined4 local_9c;
  uint local_98;
  uint local_94;
  undefined4 local_90;
  int local_8c;
  undefined4 local_88;
  char *local_84;
  char *local_80;
  undefined4 local_7c;
  int local_78;
  int local_74;
  uint local_70;
  int local_6c;
  int local_68;
  int local_64;
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  int local_50;
  int local_4c;
  size_t local_48;
  void *local_44;
  void *local_40;
  void *local_3c;
  int local_38;
  float local_34;
  float local_30;
  int local_2c;
  int local_28;
  int local_24;
  short local_1e;
  
  local_ac = (undefined4 *)0x0;
  if (surface == (SDL_Surface *)0x0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x69,"surface != ((void *)0)");
  }
  local_2c = _glGetError();
  if (local_2c != 0) {
    uVar3 = _BKGetGLErrorMessage(local_2c);
    _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
             "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
             ,PTR____FUNCTION___19629_001c446c,0x6c,uVar3);
  }
  local_cc = 0;
  local_c8 = 0;
  local_c4 = surface->w;
  local_c0 = surface->h;
  local_a4 = _BKLoadGLfunctions();
  if (local_a4 == (void *)0x0) {
    _SDL_SetError("Failed to load GL functions");
    local_104 = (undefined4 *)0x0;
  }
  else {
    local_a0 = (int *)_BKLoadGLcapabilities();
    if (local_a0 == (int *)0x0) {
      _SDL_SetError("Failed to load GL capabilities");
      local_104 = (undefined4 *)0x0;
    }
    else {
      local_ac = (undefined4 *)_calloc(1,100);
      if (local_ac == (undefined4 *)0x0) {
        _SDL_SetError("Out of memory");
        local_104 = (undefined4 *)0x0;
      }
      else {
        if (rect == (void *)0x0) {
          rect = &local_cc;
        }
        local_78 = *(int *)((int)rect + 8);
        if (local_a0[2] != 0) {
          local_78 = _power2(local_78);
        }
        if (local_a0[1] < local_78) {
          local_78 = local_a0[1];
        }
        local_74 = *(int *)((int)rect + 0xc);
        if (local_a0[2] != 0) {
          local_74 = _power2(local_74);
        }
        if (local_a0[1] < local_74) {
          local_74 = local_a0[1];
        }
        local_70 = (uint)*(byte *)((int)surface->format + 8);
        local_68 = (*(int *)((int)rect + 8) + local_78 + -1) / local_78;
        local_6c = (*(int *)((int)rect + 0xc) + local_74 + -1) / local_74;
        local_48 = local_6c * local_68;
        local_44 = _calloc(local_48,4);
        if (local_44 == (void *)0x0) {
          _SDL_SetError("Out of memory");
          local_104 = (undefined4 *)0x0;
        }
        else {
          local_40 = _calloc(local_48 * 0xc,2);
          if (local_40 == (void *)0x0) {
            _SDL_SetError("Out of memory");
            local_104 = (undefined4 *)0x0;
          }
          else {
            local_3c = _calloc(local_48 * 0xc,4);
            if (local_3c == (void *)0x0) {
              _SDL_SetError("Out of memory");
              local_104 = (undefined4 *)0x0;
            }
            else {
              local_98 = surface->flags >> 0x10 & 1;
              local_94 = surface->flags >> 0x11 & 1;
              local_90 = 0;
              if ((local_98 == 0) && (local_94 == 0)) {
                local_100 = "work/Versions/A/Carbon";
              }
              else {
                local_100 = "ork/Versions/A/Carbon";
              }
              local_80 = local_100;
              if ((local_98 == 0) && (local_94 == 0)) {
                local_fc = 0x18;
              }
              else {
                local_fc = 0x20;
              }
              local_70 = local_fc;
              local_7c = 0x1401;
              if (local_fc == 0x10) {
                if (local_98 == 0) {
                  if (local_94 == 0) {
                    local_f4 = (char *)0x8050;
                  }
                  else {
                    local_f4 = (char *)0x8057;
                  }
                  local_f8 = local_f4;
                }
                else {
                  local_f8 = (char *)0x8056;
                }
                local_84 = local_f8;
              }
              else if (local_fc == 0x20) {
                if (local_98 == 0) {
                  if (local_94 == 0) {
                    local_ec = (char *)0x8051;
                  }
                  else {
                    local_ec = (char *)0x8058;
                  }
                  local_f0 = local_ec;
                }
                else {
                  local_f0 = (char *)0x8058;
                }
                local_84 = local_f0;
              }
              else {
                if (local_98 == 0) {
                  if (local_94 == 0) {
                    local_e4 = "work/Versions/A/Carbon";
                  }
                  else {
                    local_e4 = "ork/Versions/A/Carbon";
                  }
                  local_e8 = local_e4;
                }
                else {
                  local_e8 = "ork/Versions/A/Carbon";
                }
                local_84 = local_e8;
              }
              local_a8 = _BKCreateGLSurface(local_68 * local_78,local_6c * local_74,local_fc,0,
                                            local_94,local_98);
              if (local_a8 == (SDL_Surface *)0x0) {
                _free(local_ac);
                _free(local_44);
                _free(local_40);
                _free(local_3c);
                local_104 = (undefined4 *)0x0;
              }
              else {
                local_bc = 0;
                local_b8 = 0;
                local_b4 = local_68 * local_78;
                local_b0 = local_6c * local_74;
                if (local_94 == 0) {
                  if (local_98 == 0) {
                    uVar3 = _SDL_MapRGB(local_a8->format,0,0,0);
                    _SDL_FillRect(local_a8,&local_bc,uVar3);
                  }
                  else {
                    uVar3 = _SDL_MapRGBA(local_a8->format,0,0,0,0);
                    _SDL_FillRect(local_a8,&local_bc,uVar3);
                    _SDL_SetAlpha(local_a8,local_a8->flags & 0x10000,0xff);
                  }
                }
                else {
                  local_9c = _SDL_MapRGB(local_a8->format,0,0xff,0);
                  _SDL_FillRect(local_a8,&local_bc,local_9c);
                  _SDL_SetColorKey(local_a8,local_a8->flags & 0x20000,local_9c);
                }
                local_bc = 0;
                local_b8 = 0;
                local_b4 = *(int *)((int)rect + 8);
                local_b0 = *(int *)((int)rect + 0xc);
                _SDL_SetClipRect(local_a8,&local_bc);
                if (local_98 == 0) {
                  _SDL_UpperBlit(surface,rect,local_a8,&local_bc);
                }
                else {
                  for (local_5c = 0; local_5c < *(int *)((int)rect + 0xc); local_5c = local_5c + 1)
                  {
                    for (local_60 = 0; local_60 < *(int *)((int)rect + 8); local_60 = local_60 + 1)
                    {
                      pvVar1 = surface->format;
                    /* WARNING: Load size is inaccurate */
                      uVar4 = _BKGetPixel(surface,*rect + local_60,
                                          *(int *)((int)rect + 4) + local_5c);
                      _SDL_GetRGBA(uVar4,pvVar1,&local_d4,&local_d3,&local_d2,&local_d1);
                      uVar4 = _SDL_MapRGBA(local_a8->format,local_d4,local_d3,local_d2,local_d1);
                      _BKPutPixel(local_a8,local_60,local_5c,uVar4);
                    }
                  }
                }
                if (local_94 != 0) {
                  _BKConvertColorKeyToAlphaChannel(local_a8,&local_d0);
                }
                local_8c = 0;
                local_88 = 0;
                (**(code **)((int)local_a4 + 0x1c))(local_48,local_44);
                local_28 = _glGetError();
                if (local_28 != 0) {
                  uVar3 = _BKGetGLErrorMessage(local_28);
                  _fprintf((FILE *)(PTR_001c43e8 + 0xb0),
                           "\nGL error detected at %s, %s, %d, error: %s \n",
                           "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
                           ,PTR____FUNCTION___19629_001c446c,0x14f,uVar3);
                }
                if ((local_a8->flags >> 1 & 1) != 0) {
                  _SDL_LockSurface(local_a8);
                }
                (**(code **)((int)local_a4 + 0x44))(0xcf5,1);
                (**(code **)((int)local_a4 + 0x44))
                          (0xcf2,local_a8->pitch / (int)(uint)*(byte *)((int)local_a8->format + 9));
                if (local_8c == 0) {
                  if (local_a0[7] != 0) {
                    (**(code **)((int)local_a4 + 0x44))(0x85b2,0);
                  }
                }
                else {
                  (**(code **)((int)local_a4 + 0x44))(0x85b2,1);
                }
                local_54 = 0;
                local_4c = local_74;
                local_64 = 0;
                for (local_5c = 0; local_5c < local_6c; local_5c = local_5c + 1) {
                  local_58 = 0;
                  local_50 = local_78;
                  if (local_b0 < local_4c) {
                    local_4c = local_b0;
                  }
                  for (local_60 = 0; local_60 < local_68; local_60 = local_60 + 1) {
                    local_24 = (int)local_a8->pixels +
                               (uint)*(byte *)((int)local_a8->format + 9) * local_60 * local_78 +
                               local_a8->pitch * local_5c * local_74;
                    (**(code **)((int)local_a4 + 0x2c))
                              (*local_a0,*(undefined4 *)(local_64 * 4 + (int)local_44));
                    (**(code **)((int)local_a4 + 0x34))
                              (*local_a0,0,local_84,local_78,local_74,0,local_80,local_7c,local_24);
                    local_38 = (**(code **)((int)local_a4 + 0xc))();
                    if (local_38 != 0) {
                      _fprintf((FILE *)(PTR_001c43e8 + 0xb0),
                               "glTexImage2D %dx%dx%d (0x%04X) error: 0x%04X\n",local_78,local_74,
                               local_70,local_84,local_38);
                    }
                    if (local_b4 < local_50) {
                      local_50 = local_b4;
                    }
                    if ((local_68 + -1 == local_60) && (*(int *)((int)rect + 8) % local_78 != 0)) {
                      local_34 = (float)(*(int *)((int)rect + 8) % local_78) / (float)local_78;
                    }
                    else {
                      local_34 = 1.0;
                    }
                    if ((local_6c + -1 == local_5c) && (*(int *)((int)rect + 0xc) % local_74 != 0))
                    {
                      local_30 = (float)(*(int *)((int)rect + 0xc) % local_74) / (float)local_74;
                    }
                    else {
                      local_30 = 1.0;
                    }
                    if (*local_a0 == 0x84f5) {
                      local_34 = local_34 * (float)local_78;
                      local_30 = local_30 * (float)local_74;
                    }
                    sVar2 = (short)local_64 * 0xc;
                    uVar5 = (undefined2)local_58;
                    *(undefined2 *)(sVar2 * 2 + (int)local_40) = uVar5;
                    uVar6 = (undefined2)local_54;
                    *(undefined2 *)((short)(sVar2 + 1) * 2 + (int)local_40) = uVar6;
                    uVar7 = (undefined2)local_50;
                    *(undefined2 *)((short)(sVar2 + 2) * 2 + (int)local_40) = uVar7;
                    *(undefined2 *)((short)(sVar2 + 3) * 2 + (int)local_40) = uVar6;
                    *(undefined2 *)((short)(sVar2 + 4) * 2 + (int)local_40) = uVar5;
                    uVar8 = (undefined2)local_4c;
                    *(undefined2 *)((short)(sVar2 + 5) * 2 + (int)local_40) = uVar8;
                    *(undefined2 *)((short)(sVar2 + 6) * 2 + (int)local_40) = uVar7;
                    *(undefined2 *)((short)(sVar2 + 7) * 2 + (int)local_40) = uVar6;
                    *(undefined2 *)((short)(sVar2 + 8) * 2 + (int)local_40) = uVar5;
                    *(undefined2 *)((short)(sVar2 + 9) * 2 + (int)local_40) = uVar8;
                    *(undefined2 *)((short)(sVar2 + 10) * 2 + (int)local_40) = uVar7;
                    *(undefined2 *)((short)(sVar2 + 0xb) * 2 + (int)local_40) = uVar8;
                    local_1e = (short)local_64 * 0xc;
                    *(undefined4 *)(local_1e * 4 + (int)local_3c) = 0;
                    *(undefined4 *)((short)(local_1e + 1) * 4 + (int)local_3c) = 0;
                    *(float *)((short)(local_1e + 2) * 4 + (int)local_3c) = local_34;
                    *(undefined4 *)((short)(local_1e + 3) * 4 + (int)local_3c) = 0;
                    *(undefined4 *)((short)(local_1e + 4) * 4 + (int)local_3c) = 0;
                    *(float *)((short)(local_1e + 5) * 4 + (int)local_3c) = local_30;
                    *(float *)((short)(local_1e + 6) * 4 + (int)local_3c) = local_34;
                    *(undefined4 *)((short)(local_1e + 7) * 4 + (int)local_3c) = 0;
                    *(undefined4 *)((short)(local_1e + 8) * 4 + (int)local_3c) = 0;
                    *(float *)((short)(local_1e + 9) * 4 + (int)local_3c) = local_30;
                    *(float *)((short)(local_1e + 10) * 4 + (int)local_3c) = local_34;
                    *(float *)((short)(local_1e + 0xb) * 4 + (int)local_3c) = local_30;
                    local_1e = local_1e + 0xc;
                    local_58 = local_58 + local_78;
                    local_50 = local_50 + local_78;
                    local_64 = local_64 + 1;
                  }
                  local_54 = local_54 + local_74;
                  local_4c = local_4c + local_74;
                }
                if ((local_a8->flags >> 1 & 1) != 0) {
                  _SDL_UnlockSurface(local_a8);
                }
                *local_ac = local_a8;
                local_ac[1] = local_bc;
                local_ac[2] = local_b8;
                local_ac[3] = local_b4;
                local_ac[4] = local_b0;
                if ((local_98 == 0) && (local_94 == 0)) {
                  local_e0 = 0;
                }
                else {
                  local_e0 = 1;
                }
                local_ac[8] = local_e0;
                local_ac[9] = local_88;
                local_ac[5] = 0;
                local_ac[6] = 0;
                local_ac[7] = 0;
                local_ac[10] = local_78;
                local_ac[0xb] = local_74;
                local_ac[0xc] = *local_a0;
                local_ac[0xd] = local_84;
                local_ac[0xe] = local_80;
                local_ac[0xf] = local_7c;
                local_ac[0x10] = local_6c;
                local_ac[0x11] = local_68;
                local_ac[0x12] = local_48;
                local_ac[0x16] = local_44;
                local_ac[0x17] = local_40;
                local_ac[0x18] = local_3c;
                local_ac[0x13] = 0;
                local_ac[0x14] = 0;
                local_104 = local_ac;
              }
            }
          }
        }
      }
    }
  }
  return local_104;
}


// ---- _BKCreateBlankGLSurface @ 0x1a5df (3044 bytes)

/* WARNING: Removing unreachable block (ram,0x0001a9ea) */
/* WARNING: Removing unreachable block (ram,0x0001a974) */
/* WARNING: Removing unreachable block (ram,0x0001a935) */
/* WARNING: Removing unreachable block (ram,0x0001a90a) */
/* WARNING: Removing unreachable block (ram,0x0001a998) */
/* WARNING: Removing unreachable block (ram,0x0001aa0b) */
/* WARNING: Removing unreachable block (ram,0x0001aa2f) */
/* WARNING: Removing unreachable block (ram,0x0001a9c6) */
/* WARNING: Removing unreachable block (ram,0x0001a9ba) */
/* WARNING: Removing unreachable block (ram,0x0001a9d2) */
/* WARNING: Removing unreachable block (ram,0x0001a9dc) */
/* WARNING: Removing unreachable block (ram,0x0001a968) */

undefined4 * _BKCreateBlankGLSurface(short param_1,short param_2,undefined4 param_3,int param_4)

{
  short sVar1;
  undefined4 uVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  undefined4 *local_e0;
  undefined4 local_bc;
  short local_b4;
  short local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  int local_a4;
  int local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  int local_94;
  int local_90;
  undefined4 *local_8c;
  void *local_88;
  int *local_84;
  int local_80;
  int local_7c;
  undefined4 local_78;
  int local_74;
  undefined4 local_70;
  char *local_6c;
  char *local_68;
  undefined4 local_64;
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  int local_50;
  int local_4c;
  int local_48;
  int local_44;
  int local_40;
  size_t local_3c;
  void *local_38;
  void *local_34;
  void *local_30;
  float local_2c;
  float local_28;
  size_t local_24;
  void *local_20;
  undefined4 *local_1c;
  int local_18;
  int local_14;
  short local_e;
  
  local_b0 = param_1;
  local_b4 = param_2;
  local_8c = (undefined4 *)0x0;
  local_18 = _glGetError();
  if (local_18 != 0) {
    uVar2 = _BKGetGLErrorMessage(local_18);
    _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"\nGL error detected at %s, %s, %d, error: %s \n",
             "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
             ,PTR____FUNCTION___20060_001c4488,0x231,uVar2);
  }
  local_ac = 0;
  local_a8 = 0;
  local_a4 = (int)param_1;
  local_a0 = (int)param_2;
  local_1c = &local_ac;
  local_88 = _BKLoadGLfunctions();
  if (local_88 == (void *)0x0) {
    _SDL_SetError("Failed to load GL functions");
    local_e0 = (undefined4 *)0x0;
  }
  else {
    local_84 = (int *)_BKLoadGLcapabilities();
    if (local_84 == (int *)0x0) {
      _SDL_SetError("Failed to load GL capabilities");
      local_e0 = (undefined4 *)0x0;
    }
    else {
      local_8c = (undefined4 *)_calloc(1,100);
      if (local_8c == (undefined4 *)0x0) {
        _SDL_SetError("Out of memory");
        local_e0 = (undefined4 *)0x0;
      }
      else {
        if (local_84[2] != 0) {
          local_b0 = _power2((int)param_1);
        }
        if ((local_84[1] < (int)local_b0) && (param_4 != 0)) {
          local_b0 = (short)local_84[1];
        }
        if (local_84[2] != 0) {
          local_b4 = _power2((int)param_2);
        }
        if ((local_84[1] < (int)local_b4) && (param_4 != 0)) {
          local_b4 = (short)local_84[1];
        }
        local_5c = (local_1c[2] + (int)local_b0 + -1) / (int)local_b0;
        local_60 = (local_1c[3] + (int)local_b4 + -1) / (int)local_b4;
        local_3c = local_60 * local_5c;
        local_38 = _calloc(local_3c,4);
        if (local_38 == (void *)0x0) {
          _SDL_SetError("Out of memory");
          local_e0 = (undefined4 *)0x0;
        }
        else {
          local_34 = _calloc(local_3c * 0xc,2);
          if (local_34 == (void *)0x0) {
            _SDL_SetError("Out of memory");
            local_e0 = (undefined4 *)0x0;
          }
          else {
            local_30 = _calloc(local_3c * 0xc,4);
            if (local_30 == (void *)0x0) {
              _SDL_SetError("Out of memory");
              local_e0 = (undefined4 *)0x0;
            }
            else {
              local_80 = 0;
              local_7c = 0;
              local_78 = 0;
              local_68 = "work/Versions/A/Carbon";
              local_64 = 0x1401;
              local_6c = "work/Versions/A/Carbon";
              local_9c = 0;
              local_98 = 0;
              local_94 = local_1c[2];
              local_90 = local_1c[3];
              local_74 = 0;
              local_70 = 0;
              (**(code **)((int)local_88 + 0x1c))(local_3c,local_38);
              local_14 = _glGetError();
              if (local_14 != 0) {
                uVar2 = _BKGetGLErrorMessage(local_14);
                _fprintf((FILE *)(PTR_001c43e8 + 0xb0),
                         "\nGL error detected at %s, %s, %d, error: %s \n",
                         "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
                         ,PTR____FUNCTION___20060_001c4488,0x29c,uVar2);
              }
              (**(code **)((int)local_88 + 0x44))(0xcf5,1);
              (**(code **)((int)local_88 + 0x44))(0xcf2,(int)local_b0);
              if (local_74 == 0) {
                if (local_84[7] != 0) {
                  (**(code **)((int)local_88 + 0x44))(0x85b2,0);
                }
              }
              else {
                (**(code **)((int)local_88 + 0x44))(0x85b2,1);
              }
              local_48 = 0;
              local_40 = (int)local_b4;
              local_24 = (int)local_b4 * (int)local_b0;
              local_20 = _calloc(local_24,4);
              local_58 = 0;
              for (local_50 = 0; local_50 < local_60; local_50 = local_50 + 1) {
                local_4c = 0;
                local_44 = (int)local_b0;
                if (local_90 < local_40) {
                  local_40 = local_90;
                }
                for (local_54 = 0; local_54 < local_5c; local_54 = local_54 + 1) {
                  (**(code **)((int)local_88 + 0x2c))
                            (*local_84,*(undefined4 *)(local_58 * 4 + (int)local_38));
                  _glTexImage2D(*local_84,0,"work/Versions/A/Carbon",(int)local_b0,(int)local_b4,0,
                                "work/Versions/A/Carbon",0x1401,local_20);
                  if (local_94 < local_44) {
                    local_44 = local_94;
                  }
                  if ((local_5c + -1 == local_54) && ((int)local_1c[2] % (int)local_b0 != 0)) {
                    local_2c = (float)((int)local_1c[2] % (int)local_b0) / (float)(int)local_b0;
                  }
                  else {
                    local_2c = 1.0;
                  }
                  if ((local_60 + -1 == local_50) && ((int)local_1c[3] % (int)local_b4 != 0)) {
                    local_28 = (float)((int)local_1c[3] % (int)local_b4) / (float)(int)local_b4;
                  }
                  else {
                    local_28 = 1.0;
                  }
                  if (*local_84 == 0x84f5) {
                    local_2c = local_2c * (float)(int)local_b0;
                    local_28 = local_28 * (float)(int)local_b4;
                  }
                  sVar1 = (short)local_58 * 0xc;
                  uVar3 = (undefined2)local_4c;
                  *(undefined2 *)(sVar1 * 2 + (int)local_34) = uVar3;
                  uVar4 = (undefined2)local_48;
                  *(undefined2 *)((short)(sVar1 + 1) * 2 + (int)local_34) = uVar4;
                  uVar5 = (undefined2)local_44;
                  *(undefined2 *)((short)(sVar1 + 2) * 2 + (int)local_34) = uVar5;
                  *(undefined2 *)((short)(sVar1 + 3) * 2 + (int)local_34) = uVar4;
                  *(undefined2 *)((short)(sVar1 + 4) * 2 + (int)local_34) = uVar3;
                  uVar6 = (undefined2)local_40;
                  *(undefined2 *)((short)(sVar1 + 5) * 2 + (int)local_34) = uVar6;
                  *(undefined2 *)((short)(sVar1 + 6) * 2 + (int)local_34) = uVar5;
                  *(undefined2 *)((short)(sVar1 + 7) * 2 + (int)local_34) = uVar4;
                  *(undefined2 *)((short)(sVar1 + 8) * 2 + (int)local_34) = uVar3;
                  *(undefined2 *)((short)(sVar1 + 9) * 2 + (int)local_34) = uVar6;
                  *(undefined2 *)((short)(sVar1 + 10) * 2 + (int)local_34) = uVar5;
                  *(undefined2 *)((short)(sVar1 + 0xb) * 2 + (int)local_34) = uVar6;
                  local_e = (short)local_58 * 0xc;
                  *(undefined4 *)(local_e * 4 + (int)local_30) = 0;
                  *(undefined4 *)((short)(local_e + 1) * 4 + (int)local_30) = 0;
                  *(float *)((short)(local_e + 2) * 4 + (int)local_30) = local_2c;
                  *(undefined4 *)((short)(local_e + 3) * 4 + (int)local_30) = 0;
                  *(undefined4 *)((short)(local_e + 4) * 4 + (int)local_30) = 0;
                  *(float *)((short)(local_e + 5) * 4 + (int)local_30) = local_28;
                  *(float *)((short)(local_e + 6) * 4 + (int)local_30) = local_2c;
                  *(undefined4 *)((short)(local_e + 7) * 4 + (int)local_30) = 0;
                  *(undefined4 *)((short)(local_e + 8) * 4 + (int)local_30) = 0;
                  *(float *)((short)(local_e + 9) * 4 + (int)local_30) = local_28;
                  *(float *)((short)(local_e + 10) * 4 + (int)local_30) = local_2c;
                  *(float *)((short)(local_e + 0xb) * 4 + (int)local_30) = local_28;
                  local_e = local_e + 0xc;
                  local_4c = local_4c + local_b0;
                  local_44 = local_44 + local_b0;
                  local_58 = local_58 + 1;
                }
                local_48 = local_48 + local_b4;
                local_40 = local_40 + local_b4;
              }
              _free(local_20);
              *local_8c = 0;
              local_8c[1] = local_9c;
              local_8c[2] = local_98;
              local_8c[3] = local_94;
              local_8c[4] = local_90;
              if ((local_80 == 0) && (local_7c == 0)) {
                local_bc = 0;
              }
              else {
                local_bc = 1;
              }
              local_8c[8] = local_bc;
              local_8c[9] = local_70;
              local_8c[5] = 0;
              local_8c[6] = 0;
              local_8c[7] = 0;
              local_8c[10] = (int)local_b0;
              local_8c[0xb] = (int)local_b4;
              local_8c[0xc] = *local_84;
              local_8c[0xd] = local_6c;
              local_8c[0xe] = local_68;
              local_8c[0xf] = local_64;
              local_8c[0x10] = local_60;
              local_8c[0x11] = local_5c;
              local_8c[0x12] = local_3c;
              local_8c[0x16] = local_38;
              local_8c[0x17] = local_34;
              local_8c[0x18] = local_30;
              local_8c[0x13] = 0;
              local_8c[0x14] = 0;
              _BKSetupVBOsForGLSurface(local_8c);
              local_e0 = local_8c;
            }
          }
        }
      }
    }
  }
  return local_e0;
}


// ---- _BKCreateMasterSurfaceForTiles @ 0x1b1c3 (614 bytes)

undefined4 * _BKCreateMasterSurfaceForTiles(short param_1,short param_2)

{
  undefined4 uVar1;
  undefined4 *local_58;
  undefined4 local_48;
  undefined4 local_44;
  int local_40;
  int local_3c;
  undefined4 *local_38;
  SDL_Surface *local_34;
  void *local_30;
  undefined4 *local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  char *local_14;
  undefined4 local_10;
  
  local_38 = (undefined4 *)0x0;
  local_30 = _BKLoadGLfunctions();
  if (local_30 == (void *)0x0) {
    _SDL_SetError("Failed to load GL functions");
    local_58 = (undefined4 *)0x0;
  }
  else {
    local_2c = (undefined4 *)_BKLoadGLcapabilities();
    if (local_2c == (undefined4 *)0x0) {
      _SDL_SetError("Failed to load GL capabilities");
      local_58 = (undefined4 *)0x0;
    }
    else {
      local_38 = (undefined4 *)_calloc(1,100);
      if (local_38 == (undefined4 *)0x0) {
        _SDL_SetError("Out of memory");
        local_58 = (undefined4 *)0x0;
      }
      else {
        local_28 = 1;
        local_24 = 0;
        local_20 = 0;
        local_14 = "ork/Versions/A/Carbon";
        local_10 = 0x1401;
        local_18 = 0x8058;
        local_34 = _BKCreateGLSurface((int)param_1,(int)param_2,0x20,0,0,1);
        if (local_34 == (SDL_Surface *)0x0) {
          _free(local_38);
          local_58 = (undefined4 *)0x0;
        }
        else {
          local_48 = 0;
          local_44 = 0;
          local_40 = (int)param_1;
          local_3c = (int)param_2;
          uVar1 = _SDL_MapRGBA(local_34->format,0,0,0,0);
          _SDL_FillRect(local_34,&local_48,uVar1);
          _SDL_SetAlpha(local_34,local_34->flags & 0x10000,0xff);
          *local_38 = local_34;
          local_38[1] = local_48;
          local_38[2] = local_44;
          local_38[3] = local_40;
          local_38[4] = local_3c;
          local_38[8] = local_28;
          local_38[9] = local_1c;
          local_38[5] = 0;
          local_38[6] = 0;
          local_38[7] = 0;
          local_38[10] = (int)param_1;
          local_38[0xb] = (int)param_2;
          local_38[0xc] = *local_2c;
          local_38[0xd] = local_18;
          local_38[0xe] = local_14;
          local_38[0xf] = local_10;
          local_38[0x10] = 1;
          local_38[0x11] = 1;
          local_38[0x12] = 1;
          local_58 = local_38;
        }
      }
    }
  }
  return local_58;
}


// ---- _BKAddTileToMasterSurface @ 0x1b429 (1296 bytes)

void _BKAddTileToMasterSurface
               (SDL_Surface *param_1,SDL_Surface *param_2,short *param_3,short *param_4)

{
  void *pvVar1;
  uint uVar2;
  undefined1 local_18;
  undefined1 local_17;
  undefined1 local_16;
  undefined1 local_15;
  short local_14;
  short local_12;
  short local_10;
  short local_e;
  
  local_10 = param_4[1];
  for (local_14 = param_3[1]; local_14 < param_3[3]; local_14 = local_14 + 1) {
    local_e = *param_4;
    for (local_12 = *param_3; local_12 < param_3[2]; local_12 = local_12 + 1) {
      pvVar1 = param_1->format;
      uVar2 = _BKGetPixel(param_1,(int)local_14,(int)local_12);
      _SDL_GetRGBA(uVar2,pvVar1,&local_18,&local_17,&local_16,&local_15);
      uVar2 = _SDL_MapRGBA(param_2->format,local_18,local_17,local_16,local_15);
      _BKPutPixel(param_2,(int)local_10,(int)local_e,uVar2);
      local_e = local_e + 1;
    }
    local_10 = local_10 + 1;
  }
  local_14 = param_3[1];
  local_10 = param_4[1] + -1;
  local_e = *param_4;
  for (local_12 = *param_3; local_12 < param_3[2]; local_12 = local_12 + 1) {
    pvVar1 = param_1->format;
    uVar2 = _BKGetPixel(param_1,(int)local_14,(int)local_12);
    _SDL_GetRGBA(uVar2,pvVar1,&local_18,&local_17,&local_16,&local_15);
    uVar2 = _SDL_MapRGBA(param_2->format,local_18,local_17,local_16,local_15);
    _BKPutPixel(param_2,(int)local_10,(int)local_e,uVar2);
    local_e = local_e + 1;
  }
  local_14 = param_3[3] + -1;
  local_10 = param_4[3];
  local_e = *param_4;
  for (local_12 = *param_3; local_12 < param_3[2]; local_12 = local_12 + 1) {
    pvVar1 = param_1->format;
    uVar2 = _BKGetPixel(param_1,(int)local_14,(int)local_12);
    _SDL_GetRGBA(uVar2,pvVar1,&local_18,&local_17,&local_16,&local_15);
    uVar2 = _SDL_MapRGBA(param_2->format,local_18,local_17,local_16,local_15);
    _BKPutPixel(param_2,(int)local_10,(int)local_e,uVar2);
    local_e = local_e + 1;
  }
  local_12 = *param_3;
  local_e = *param_4 + -1;
  local_10 = param_4[1];
  for (local_14 = param_3[1]; local_14 < param_3[3]; local_14 = local_14 + 1) {
    pvVar1 = param_1->format;
    uVar2 = _BKGetPixel(param_1,(int)local_14,(int)local_12);
    _SDL_GetRGBA(uVar2,pvVar1,&local_18,&local_17,&local_16,&local_15);
    uVar2 = _SDL_MapRGBA(param_2->format,local_18,local_17,local_16,local_15);
    _BKPutPixel(param_2,(int)local_10,(int)local_e,uVar2);
    local_10 = local_10 + 1;
  }
  local_12 = param_3[2] + -1;
  local_e = param_4[2];
  local_10 = param_4[1];
  for (local_14 = param_3[1]; local_14 < param_3[3]; local_14 = local_14 + 1) {
    pvVar1 = param_1->format;
    uVar2 = _BKGetPixel(param_1,(int)local_14,(int)local_12);
    _SDL_GetRGBA(uVar2,pvVar1,&local_18,&local_17,&local_16,&local_15);
    uVar2 = _SDL_MapRGBA(param_2->format,local_18,local_17,local_16,local_15);
    _BKPutPixel(param_2,(int)local_10,(int)local_e,uVar2);
    local_10 = local_10 + 1;
  }
  return;
}


// ---- _BKCreateGLSurfaceFromMaster @ 0x1b939 (1687 bytes)

undefined4 _BKCreateGLSurfaceFromMaster(int *param_1,short param_2,short param_3,short param_4)

{
  short sVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  undefined4 local_7c;
  short local_68;
  short local_64;
  short local_58;
  short local_56;
  short local_54;
  short local_52;
  void *local_50;
  int *local_4c;
  undefined4 local_48;
  undefined4 local_44;
  int local_40;
  int local_3c;
  float local_38;
  float local_34;
  undefined4 *local_30;
  void *local_2c;
  void *local_28;
  int local_24;
  short local_20;
  short local_1e;
  
  local_64 = param_3;
  local_68 = param_4;
  local_50 = _BKLoadGLfunctions();
  local_4c = (int *)_BKLoadGLcapabilities();
  if ((local_50 == (void *)0x0) || (local_4c == (int *)0x0)) {
    _SDL_SetError("BKCreateGLSurfaceFromMaster failed to load GL functions or capabilities");
    local_7c = 0xfffffffe;
  }
  else {
    local_30 = (undefined4 *)_calloc(1,4);
    if (local_30 == (undefined4 *)0x0) {
      _SDL_SetError("BKCreateGLSurfaceFromMaster failed. Out of memory");
      local_7c = 0xffffffff;
    }
    else {
      local_2c = _calloc(param_2 * 0xc,2);
      if (local_2c == (void *)0x0) {
        _SDL_SetError("BKCreateGLSurfaceFromMaster failed. Out of memory");
        local_7c = 0xffffffff;
      }
      else {
        local_28 = _calloc(param_2 * 0xc,4);
        if (local_28 == (void *)0x0) {
          _SDL_SetError("BKCreateGLSurfaceFromMaster failed. Out of memory");
          local_7c = 0xffffffff;
        }
        else {
          (**(code **)((int)local_50 + 0x1c))(1,local_30);
          local_24 = (**(code **)((int)local_50 + 0xc))();
          if (local_24 != 0) {
            _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"glGenTextures (%d) error: 0x%04X\n",1,local_24);
          }
          if ((*(uint *)*param_1 >> 1 & 1) != 0) {
            _SDL_LockSurface(*param_1);
          }
          (**(code **)((int)local_50 + 0x44))(0xcf5,1);
          (**(code **)((int)local_50 + 0x44))
                    (0xcf2,*(int *)(*param_1 + 0x10) /
                           (int)(uint)*(byte *)(*(int *)(*param_1 + 4) + 9));
          if (local_4c[7] != 0) {
            (**(code **)((int)local_50 + 0x44))(0x85b2,0);
          }
          (**(code **)((int)local_50 + 0x2c))(*local_4c,*local_30);
          (**(code **)((int)local_50 + 0x34))
                    (*local_4c,0,param_1[0xd],param_1[10],param_1[0xb],0,param_1[0xe],param_1[0xf],
                     *(undefined4 *)(*param_1 + 0x14));
          local_24 = (**(code **)((int)local_50 + 0xc))();
          if (local_24 != 0) {
            _fprintf((FILE *)(PTR_001c43e8 + 0xb0),
                     "BKCreateGLSurfaceFromMaster %dx%d (0x%04X) error: 0x%04X\n",param_1[10],
                     param_1[0xb],param_1[0xd],local_24);
          }
          local_44 = 0;
          local_48 = 0;
          local_40 = (int)param_3;
          local_3c = (int)param_4;
          local_38 = 1.0;
          local_34 = 1.0;
          if (*local_4c == 0x84f5) {
            local_38 = (float)(int)param_3 * 1.0;
            local_34 = (float)(int)param_4 * 1.0;
          }
          else {
            local_64 = 1;
            local_68 = 1;
          }
          for (local_1e = 0; local_1e < param_2; local_1e = local_1e + 1) {
            _BKGetTileRectForMasterSurface
                      (*param_1,(int)local_1e,(int)local_64,(int)local_68,&local_58);
            sVar1 = local_1e * 0xc;
            uVar2 = (undefined2)local_48;
            *(undefined2 *)(sVar1 * 2 + (int)local_2c) = uVar2;
            uVar3 = (undefined2)local_44;
            *(undefined2 *)((short)(sVar1 + 1) * 2 + (int)local_2c) = uVar3;
            uVar4 = (undefined2)local_40;
            *(undefined2 *)((short)(sVar1 + 2) * 2 + (int)local_2c) = uVar4;
            *(undefined2 *)((short)(sVar1 + 3) * 2 + (int)local_2c) = uVar3;
            *(undefined2 *)((short)(sVar1 + 4) * 2 + (int)local_2c) = uVar2;
            uVar5 = (undefined2)local_3c;
            *(undefined2 *)((short)(sVar1 + 5) * 2 + (int)local_2c) = uVar5;
            *(undefined2 *)((short)(sVar1 + 6) * 2 + (int)local_2c) = uVar4;
            *(undefined2 *)((short)(sVar1 + 7) * 2 + (int)local_2c) = uVar3;
            *(undefined2 *)((short)(sVar1 + 8) * 2 + (int)local_2c) = uVar2;
            *(undefined2 *)((short)(sVar1 + 9) * 2 + (int)local_2c) = uVar5;
            *(undefined2 *)((short)(sVar1 + 10) * 2 + (int)local_2c) = uVar4;
            *(undefined2 *)((short)(sVar1 + 0xb) * 2 + (int)local_2c) = uVar5;
            local_20 = local_1e * 0xc;
            *(float *)(local_20 * 4 + (int)local_28) = (float)(int)local_56;
            *(float *)((short)(local_20 + 1) * 4 + (int)local_28) = (float)(int)local_58;
            *(float *)((short)(local_20 + 2) * 4 + (int)local_28) = (float)(int)local_52;
            *(float *)((short)(local_20 + 3) * 4 + (int)local_28) = (float)(int)local_58;
            *(float *)((short)(local_20 + 4) * 4 + (int)local_28) = (float)(int)local_56;
            *(float *)((short)(local_20 + 5) * 4 + (int)local_28) = (float)(int)local_54;
            *(float *)((short)(local_20 + 6) * 4 + (int)local_28) = (float)(int)local_52;
            *(float *)((short)(local_20 + 7) * 4 + (int)local_28) = (float)(int)local_58;
            *(float *)((short)(local_20 + 8) * 4 + (int)local_28) = (float)(int)local_56;
            *(float *)((short)(local_20 + 9) * 4 + (int)local_28) = (float)(int)local_54;
            *(float *)((short)(local_20 + 10) * 4 + (int)local_28) = (float)(int)local_52;
            *(float *)((short)(local_20 + 0xb) * 4 + (int)local_28) = (float)(int)local_54;
            local_20 = local_20 + 0xc;
          }
          if ((*(uint *)*param_1 >> 1 & 1) != 0) {
            _SDL_UnlockSurface(*param_1);
          }
          param_1[0x16] = (int)local_30;
          param_1[0x17] = (int)local_2c;
          param_1[0x18] = (int)local_28;
          param_1[0x13] = 0;
          param_1[0x14] = 0;
          local_7c = 0;
        }
      }
    }
  }
  return local_7c;
}


// ---- _BKGetTileRectForMasterSurface @ 0x1bfd0 (216 bytes)

void _BKGetTileRectForMasterSurface
               (int param_1,short param_2,short param_3,short param_4,short *param_5)

{
  short sVar1;
  short sVar2;
  
  sVar2 = (short)(*(int *)(param_1 + 8) / (int)(short)(param_3 + 2));
  sVar1 = (param_3 + 2) * (param_2 % sVar2);
  sVar2 = (param_4 + 2) * (param_2 / sVar2);
  *param_5 = sVar2;
  param_5[1] = sVar1;
  param_5[2] = sVar2 + param_4;
  param_5[3] = sVar1 + param_3;
  return;
}


// ---- _DoesSurfaceNeedBlend @ 0x1c0a8 (20 bytes)

undefined4 _DoesSurfaceNeedBlend(int param_1)

{
  return *(undefined4 *)(param_1 + 0x20);
}


// ---- _BKReloadGLSurface @ 0x1c0bc (894 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int BKReloadGLSurface(void *glsurface,void *rect)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  void *pvVar4;
  int iVar5;
  int iStack_68;
  int iStack_50;
  int iStack_4c;
  int iStack_48;
  int iStack_40;
  int iStack_3c;
  int iStack_38;
  int iStack_34;
  
  pvVar4 = _BKLoadGLfunctions();
  if (pvVar4 == (void *)0x0) {
    _SDL_SetError("Failed to load GL functions");
    iStack_68 = -1;
  }
  else {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x4e8,"textureSurface = ((void *)0)");
    if ((uRam00000000 >> 1 & 1) != 0) {
      _SDL_LockSurface(0);
    }
    if (rect == (void *)0x0) {
      rect = (void *)((int)glsurface + 4);
      iStack_40 = 0;
      iStack_3c = 0;
      iStack_38 = *(int *)((int)glsurface + 0x44);
      iStack_34 = *(int *)((int)glsurface + 0x40);
    }
    else {
                    /* WARNING: Load size is inaccurate */
      iStack_40 = *rect / *(int *)((int)glsurface + 0x28);
      iStack_3c = *(int *)((int)rect + 4) / *(int *)((int)glsurface + 0x2c);
                    /* WARNING: Load size is inaccurate */
      iStack_38 = (*rect + *(int *)((int)rect + 8)) / *(int *)((int)glsurface + 0x28);
      iStack_34 = (*(int *)((int)rect + 4) + *(int *)((int)rect + 0xc)) /
                  *(int *)((int)glsurface + 0x2c);
      if (iStack_40 < 0) {
        iStack_40 = 0;
      }
      if (iStack_3c < 0) {
        iStack_3c = 0;
      }
      if (*(int *)((int)glsurface + 0x44) < iStack_38) {
        iStack_38 = *(int *)((int)glsurface + 0x44);
      }
      if (*(int *)((int)glsurface + 0x40) < iStack_34) {
        iStack_34 = *(int *)((int)glsurface + 0x40);
      }
    }
    iStack_50 = 0;
    for (iStack_48 = iStack_3c; iStack_48 <= iStack_34; iStack_48 = iStack_48 + 1) {
      for (iStack_4c = iStack_40; iStack_4c <= iStack_38; iStack_4c = iStack_4c + 1) {
        iVar5 = iRam00000014 +
                iRam00000010 *
                (*(int *)((int)rect + 4) + *(int *)((int)glsurface + 0x2c) * iStack_48);
                    /* WARNING: Load size is inaccurate */
        iVar2 = *rect;
        iVar3 = *(int *)((int)glsurface + 0x28);
        bVar1 = *(byte *)(iRam00000004 + 9);
        (**(code **)((int)pvVar4 + 0x2c))
                  (*(undefined4 *)((int)glsurface + 0x30),
                   *(undefined4 *)(*(int *)((int)glsurface + 0x58) + iStack_50 * 4));
        (**(code **)((int)pvVar4 + 0x38))
                  (*(undefined4 *)((int)glsurface + 0x30),0,0,0,
                   *(undefined4 *)((int)glsurface + 0x28),*(undefined4 *)((int)glsurface + 0x2c),
                   *(undefined4 *)((int)glsurface + 0x38),*(undefined4 *)((int)glsurface + 0x3c),
                   iVar5 + (uint)bVar1 * (iVar2 + iVar3 * iStack_4c));
        iVar5 = (**(code **)((int)pvVar4 + 0xc))();
        if (iVar5 != 0) {
          _fprintf((FILE *)(PTR_001c43e8 + 0xb0),"glTexSubImage2D %dx%d error: %04X\n",
                   *(undefined4 *)((int)glsurface + 0x28),*(undefined4 *)((int)glsurface + 0x2c),
                   iVar5);
        }
        iStack_50 = iStack_50 + 1;
      }
    }
    if ((uRam00000000 >> 1 & 1) != 0) {
      _SDL_UnlockSurface(0);
    }
    iStack_68 = 0;
  }
  return iStack_68;
}


// ---- _BKCreatePartialGLSurface @ 0x1c43a (1106 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * BKCreatePartialGLSurface(void *glsurface,void *rect)

{
  void *pvVar1;
  int iVar2;
  int iVar3;
  size_t sVar4;
  undefined4 *puStack_60;
  int iStack_50;
  int iStack_4c;
  int iStack_48;
  int iStack_44;
  int iStack_40;
  int iStack_3c;
  int iStack_38;
  void *pvStack_18;
  void *pvStack_14;
  void *pvStack_10;
  
  if (glsurface == (void *)0x0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x54b,"glsurface != ((void *)0)");
  }
  pvVar1 = _BKLoadGLfunctions();
  if (pvVar1 == (void *)0x0) {
    _SDL_SetError("Failed to load GL functions");
    puStack_60 = (undefined4 *)0x0;
  }
  else {
    if (rect == (void *)0x0) {
      iStack_44 = 0;
      iStack_40 = 0;
      iStack_3c = *(int *)((int)glsurface + 0x44);
      iStack_38 = *(int *)((int)glsurface + 0x40);
    }
    else {
                    /* WARNING: Load size is inaccurate */
      iStack_44 = *rect / *(int *)((int)glsurface + 0x28);
      iStack_40 = *(int *)((int)rect + 4) / *(int *)((int)glsurface + 0x2c);
                    /* WARNING: Load size is inaccurate */
      iStack_3c = (*rect + *(int *)((int)rect + 8)) / *(int *)((int)glsurface + 0x28);
      iStack_38 = (*(int *)((int)rect + 4) + *(int *)((int)rect + 0xc)) /
                  *(int *)((int)glsurface + 0x2c);
      if (iStack_44 < 0) {
        iStack_44 = 0;
      }
      if (iStack_40 < 0) {
        iStack_40 = 0;
      }
      if (*(int *)((int)glsurface + 0x44) < iStack_3c) {
        iStack_3c = *(int *)((int)glsurface + 0x44);
      }
      if (*(int *)((int)glsurface + 0x40) < iStack_38) {
        iStack_38 = *(int *)((int)glsurface + 0x40);
      }
    }
    puStack_60 = (undefined4 *)_calloc(1,100);
    if (puStack_60 == (undefined4 *)0x0) {
      _SDL_SetError("Out of memory");
      puStack_60 = (undefined4 *)0x0;
    }
    else {
      iVar2 = (iStack_3c - iStack_44) + 1;
      iVar3 = (iStack_38 - iStack_40) + 1;
      sVar4 = iVar3 * iVar2;
      pvStack_18 = _calloc(sVar4,4);
      if (pvStack_18 == (void *)0x0) {
        _SDL_SetError("Out of memory");
        puStack_60 = (undefined4 *)0x0;
      }
      else {
        pvStack_14 = _calloc(sVar4 * 4,2);
        if (pvStack_14 == (void *)0x0) {
          _SDL_SetError("Out of memory");
          puStack_60 = (undefined4 *)0x0;
        }
        else {
          pvStack_10 = _calloc(sVar4 * 4,4);
          if (pvStack_10 == (void *)0x0) {
            _SDL_SetError("Out of memory");
            puStack_60 = (undefined4 *)0x0;
          }
          else {
            iStack_50 = 0;
            for (iStack_48 = iStack_40; iStack_48 <= iStack_38; iStack_48 = iStack_48 + 1) {
              for (iStack_4c = iStack_44; iStack_4c <= iStack_3c; iStack_4c = iStack_4c + 1) {
                *(undefined4 *)(iStack_50 * 4 + (int)pvStack_18) = 0;
                *(undefined4 *)(iStack_50 * 4 + (int)pvStack_18) = 0;
                *(undefined4 *)(iStack_50 * 4 + (int)pvStack_18) = 0;
                *(undefined4 *)(iStack_50 * 4 + (int)pvStack_18) = 0;
                pvStack_18 = (void *)((int)pvStack_18 + 4);
                pvStack_14 = (void *)((int)pvStack_14 + 0x18);
                pvStack_10 = (void *)((int)pvStack_10 + 0x30);
                iStack_50 = iStack_50 + 1;
              }
            }
                    /* WARNING: Load size is inaccurate */
            *puStack_60 = *glsurface;
            puStack_60[1] = *(undefined4 *)((int)glsurface + 4);
            puStack_60[2] = *(undefined4 *)((int)glsurface + 8);
            puStack_60[3] = *(undefined4 *)((int)glsurface + 0xc);
            puStack_60[4] = *(undefined4 *)((int)glsurface + 0x10);
            puStack_60[8] = *(undefined4 *)((int)glsurface + 0x20);
            puStack_60[9] = *(undefined4 *)((int)glsurface + 0x24);
            puStack_60[5] = 1;
            puStack_60[6] = glsurface;
            puStack_60[10] = *(undefined4 *)((int)glsurface + 0x28);
            puStack_60[0xb] = *(undefined4 *)((int)glsurface + 0x2c);
            puStack_60[0xc] = *(undefined4 *)((int)glsurface + 0x30);
            puStack_60[0xd] = *(undefined4 *)((int)glsurface + 0x34);
            puStack_60[0xe] = *(undefined4 *)((int)glsurface + 0x38);
            puStack_60[0xf] = *(undefined4 *)((int)glsurface + 0x3c);
            puStack_60[0x10] = iVar3;
            puStack_60[0x11] = iVar2;
            puStack_60[0x13] = *(undefined4 *)((int)glsurface + 0x4c);
            puStack_60[0x14] = *(undefined4 *)((int)glsurface + 0x50);
            puStack_60[0x12] = sVar4;
            puStack_60[0x16] = pvStack_18;
            puStack_60[0x17] = pvStack_14;
            puStack_60[0x18] = pvStack_10;
            *(int *)((int)glsurface + 0x1c) = *(int *)((int)glsurface + 0x1c) + 1;
          }
        }
      }
    }
  }
  return puStack_60;
}


// ---- _BKFreeGLSurface @ 0x1c88c (336 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void _BKFreeGLSurface(void *glsurface)

{
  void *pvVar1;
  
  pvVar1 = _BKLoadGLfunctions();
  if (pvVar1 == (void *)0x0) {
    _SDL_SetError("Failed to load GL functions");
  }
  else if (glsurface != (void *)0x0) {
    if (*(int *)((int)glsurface + 0x5c) != 0) {
      _free(*(void **)((int)glsurface + 0x5c));
      *(undefined4 *)((int)glsurface + 0x5c) = 0;
    }
    if (*(int *)((int)glsurface + 0x60) != 0) {
      _free(*(void **)((int)glsurface + 0x60));
      *(undefined4 *)((int)glsurface + 0x60) = 0;
    }
    if (*(int *)((int)glsurface + 0x58) != 0) {
      if (*(int *)((int)glsurface + 0x14) == 0) {
        (**(code **)((int)pvVar1 + 0x28))
                  (*(undefined4 *)((int)glsurface + 0x48),*(undefined4 *)((int)glsurface + 0x58));
      }
      _free(*(void **)((int)glsurface + 0x58));
    }
                    /* WARNING: Load size is inaccurate */
    if (*glsurface != 0) {
      if (*(int *)((int)glsurface + 0x14) == 0) {
                    /* WARNING: Load size is inaccurate */
        _SDL_FreeSurface(*glsurface);
      }
      *(undefined4 *)glsurface = 0;
    }
    if (*(int *)((int)glsurface + 0x18) != 0) {
      *(int *)(*(int *)((int)glsurface + 0x18) + 0x1c) =
           *(int *)(*(int *)((int)glsurface + 0x18) + 0x1c) + 1;
    }
    if (*(int *)((int)glsurface + 0x4c) != 0) {
      _glDeleteBuffers(1,(int)glsurface + 0x4c);
    }
    if (*(int *)((int)glsurface + 0x50) != 0) {
      _glDeleteBuffers(1,(int)glsurface + 0x50);
    }
    _free(glsurface);
  }
  return;
}


// ---- _BKGetSDLSurface @ 0x1c9dc (19 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

SDL_Surface * BKGetSDLSurface(void *glsurface)

{
                    /* WARNING: Load size is inaccurate */
  return *glsurface;
}


// ---- _SetBKDebugDrawColor @ 0x1c9ef (85 bytes)

void SetBKDebugDrawColor(undefined1 param_1,undefined1 param_2,undefined1 param_3)

{
  *PTR__debugRedColor_001c44ac = param_1;
  *PTR__debugGreenColor_001c4428 = param_2;
  *PTR__debugBlueColor_001c4420 = param_3;
  return;
}


// ---- _BKBlitGLSurface @ 0x1ca44 (736 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int _BKBlitGLSurface(void *surface)

{
  int iVar1;
  undefined4 *puVar2;
  int local_30;
  int local_1c;
  short *local_14;
  undefined4 *local_10;
  
  if (surface == (void *)0x0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x63c,"glsurface != ((void *)0)");
  }
  if (*(int *)((int)surface + 0x58) == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x63e,"glsurface->textureIDs != ((void *)0)");
  }
  if (*(int *)((int)surface + 0x5c) == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x63f,"glsurface->pixelCoords != ((void *)0)");
  }
  if (*(int *)((int)surface + 0x60) == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x640,"glsurface->textureCoords != ((void *)0)");
  }
  puVar2 = (undefined4 *)_BKLoadGLfunctions();
  if (puVar2 == (undefined4 *)0x0) {
    _SDL_SetError("Failed to load GL functions");
    local_30 = -1;
  }
  else {
    iVar1 = *(int *)((int)surface + 0x58);
    local_14 = *(short **)((int)surface + 0x5c);
    local_10 = *(undefined4 **)((int)surface + 0x60);
    for (local_1c = 0; local_1c < *(int *)((int)surface + 0x48); local_1c = local_1c + 1) {
      (*(code *)puVar2[0xb])
                (*(undefined4 *)((int)surface + 0x30),*(undefined4 *)(local_1c * 4 + iVar1));
      if (*(int *)((int)surface + 0x4c) == 0) {
        (*(code *)*puVar2)(7);
        (*(code *)puVar2[0x1a])(*local_10,local_10[1]);
        (*(code *)puVar2[0x18])((int)*local_14,(int)local_14[1]);
        (*(code *)puVar2[0x1a])(local_10[2],local_10[3]);
        (*(code *)puVar2[0x18])((int)local_14[2],(int)local_14[3]);
        (*(code *)puVar2[0x1a])(local_10[10],local_10[0xb]);
        (*(code *)puVar2[0x18])((int)local_14[10],(int)local_14[0xb]);
        (*(code *)puVar2[0x1a])(local_10[4],local_10[5]);
        (*(code *)puVar2[0x18])((int)local_14[4],(int)local_14[5]);
        (*(code *)puVar2[1])();
        local_14 = local_14 + 0xc;
        local_10 = local_10 + 0xc;
      }
      else {
        _BKBlitGLSurfaceUsingVBO(surface,(int)(short)local_1c);
      }
    }
    local_30 = 0;
  }
  return local_30;
}


// ---- _BKSetupVBOsForGLSurface @ 0x1cd24 (14 bytes)

void _BKSetupVBOsForGLSurface(void)

{
  return;
}


// ---- _BKBlitGLSurfaceUsingVBO @ 0x1cd32 (201 bytes)

void _BKBlitGLSurfaceUsingVBO(int param_1,short param_2)

{
  _glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x4c));
  _glTexCoordPointer(2,0x1406,0,*(undefined4 *)(param_1 + 0x54));
  _glVertexPointer(2,0x1402,0,0);
  _glEnableClientState(0x8074);
  _glEnableClientState(0x8078);
  _glDrawArrays(4,param_2 * 6,6);
  _glDisableClientState(0x8074);
  _glDisableClientState(0x8078);
  return;
}


// ---- _BKBlitClippedGLSurface @ 0x1cdfb (1213 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int _BKBlitClippedGLSurface(void *glSurfaceP,short left,short top,short right,short bottom)

{
  undefined4 *puVar1;
  int local_50;
  int local_30;
  undefined4 *local_2c;
  short *local_28;
  float *local_24;
  
  if (glSurfaceP == (void *)0x0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x729,"glsurface != ((void *)0)");
  }
  if (*(int *)((int)glSurfaceP + 0x58) == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x72b,"glsurface->textureIDs != ((void *)0)");
  }
  if (*(int *)((int)glSurfaceP + 0x5c) == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x72c,"glsurface->pixelCoords != ((void *)0)");
  }
  if (*(int *)((int)glSurfaceP + 0x60) == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x72d,"glsurface->textureCoords != ((void *)0)");
  }
  puVar1 = (undefined4 *)_BKLoadGLfunctions();
  if (puVar1 == (undefined4 *)0x0) {
    _SDL_SetError("Failed to load GL functions");
    local_50 = -1;
  }
  else {
    local_2c = *(undefined4 **)((int)glSurfaceP + 0x58);
    local_28 = *(short **)((int)glSurfaceP + 0x5c);
    local_24 = *(float **)((int)glSurfaceP + 0x60);
    for (local_30 = 0; local_30 < *(int *)((int)glSurfaceP + 0x48); local_30 = local_30 + 1) {
      (*(code *)puVar1[0xb])(*(undefined4 *)((int)glSurfaceP + 0x30),*local_2c);
      (*(code *)*puVar1)(4);
      (*(code *)puVar1[0x1a])((float)(int)left + *local_24,local_24[1] + (float)(int)top);
      (*(code *)puVar1[0x18])((int)*local_28 + (int)left,(int)local_28[1] + (int)top);
      (*(code *)puVar1[0x1a])(local_24[2] - (float)(int)right,local_24[3] + (float)(int)top);
      (*(code *)puVar1[0x18])((int)local_28[2] - (int)right,(int)local_28[3] + (int)top);
      (*(code *)puVar1[0x1a])((float)(int)left + local_24[4],local_24[5] - (float)(int)bottom);
      (*(code *)puVar1[0x18])((int)local_28[4] + (int)left,(int)local_28[5] - (int)bottom);
      (*(code *)puVar1[0x1a])(local_24[6] - (float)(int)right,local_24[7] + (float)(int)top);
      (*(code *)puVar1[0x18])((int)local_28[6] - (int)right,(int)local_28[7] + (int)top);
      (*(code *)puVar1[0x1a])((float)(int)left + local_24[8],local_24[9] - (float)(int)bottom);
      (*(code *)puVar1[0x18])((int)local_28[8] + (int)left,(int)local_28[9] - (int)bottom);
      (*(code *)puVar1[0x1a])(local_24[10] - (float)(int)right,local_24[0xb] - (float)(int)bottom);
      (*(code *)puVar1[0x18])((int)local_28[10] - (int)right,(int)local_28[0xb] - (int)bottom);
      (*(code *)puVar1[1])();
      local_2c = local_2c + 1;
      local_28 = local_28 + 0xc;
      local_24 = local_24 + 0xc;
    }
    local_50 = 0;
  }
  return local_50;
}


// ---- _BKBlitCustomGLSurface @ 0x1d2b8 (1029 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int BKBlitCustomGLSurface
              (void *glSurfaceP,short tlH,short tlV,short trH,short trV,short blH,short blV,
              short brH,short brV)

{
  undefined4 *puVar1;
  int iStack_60;
  int iStack_30;
  undefined4 *puStack_2c;
  short *psStack_28;
  undefined4 *puStack_24;
  
  if (glSurfaceP == (void *)0x0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x78b,"glsurface != ((void *)0)");
  }
  if (*(int *)((int)glSurfaceP + 0x58) == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x78d,"glsurface->textureIDs != ((void *)0)");
  }
  if (*(int *)((int)glSurfaceP + 0x5c) == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x78e,"glsurface->pixelCoords != ((void *)0)");
  }
  if (*(int *)((int)glSurfaceP + 0x60) == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x78f,"glsurface->textureCoords != ((void *)0)");
  }
  puVar1 = (undefined4 *)_BKLoadGLfunctions();
  if (puVar1 == (undefined4 *)0x0) {
    _SDL_SetError("Failed to load GL functions");
    iStack_60 = -1;
  }
  else {
    puStack_2c = *(undefined4 **)((int)glSurfaceP + 0x58);
    psStack_28 = *(short **)((int)glSurfaceP + 0x5c);
    puStack_24 = *(undefined4 **)((int)glSurfaceP + 0x60);
    for (iStack_30 = 0; iStack_30 < *(int *)((int)glSurfaceP + 0x48); iStack_30 = iStack_30 + 1) {
      (*(code *)puVar1[0xb])(*(undefined4 *)((int)glSurfaceP + 0x30),*puStack_2c);
      (*(code *)*puVar1)(4);
      (*(code *)puVar1[0x1a])(*puStack_24,puStack_24[1]);
      (*(code *)puVar1[0x18])((int)*psStack_28 + (int)tlH,(int)psStack_28[1] + (int)tlV);
      (*(code *)puVar1[0x1a])(puStack_24[2],puStack_24[3]);
      (*(code *)puVar1[0x18])((int)psStack_28[2] + (int)trH,(int)psStack_28[3] + (int)trV);
      (*(code *)puVar1[0x1a])(puStack_24[4],puStack_24[5]);
      (*(code *)puVar1[0x18])((int)psStack_28[4] + (int)blH,(int)psStack_28[5] + (int)blV);
      (*(code *)puVar1[0x1a])(puStack_24[6],puStack_24[7]);
      (*(code *)puVar1[0x18])((int)psStack_28[6] + (int)trH,(int)psStack_28[7] + (int)trV);
      (*(code *)puVar1[0x1a])(puStack_24[8],puStack_24[9]);
      (*(code *)puVar1[0x18])((int)psStack_28[8] + (int)blH,(int)psStack_28[9] + (int)blV);
      (*(code *)puVar1[0x1a])(puStack_24[10],puStack_24[0xb]);
      (*(code *)puVar1[0x18])((int)psStack_28[10] + (int)brH,(int)psStack_28[0xb] + (int)brV);
      (*(code *)puVar1[1])();
      puStack_2c = puStack_2c + 1;
      psStack_28 = psStack_28 + 0xc;
      puStack_24 = puStack_24 + 0xc;
    }
    iStack_60 = 0;
  }
  return iStack_60;
}


// ---- _BKBlitTileFromMasterSurface @ 0x1d6bd (697 bytes)

undefined4 _BKBlitTileFromMasterSurface(int param_1,short param_2)

{
  undefined4 *puVar1;
  undefined4 local_34;
  int local_1c;
  short *local_14;
  undefined4 *local_10;
  
  if (param_1 == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x7ed,"glsurface != ((void *)0)");
  }
  if (*(int *)(param_1 + 0x58) == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x7ee,"glsurface->textureIDs != ((void *)0)");
  }
  if (*(int *)(param_1 + 0x5c) == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x7ef,"glsurface->pixelCoords != ((void *)0)");
  }
  if (*(int *)(param_1 + 0x60) == 0) {
    ___eprintf("%s:%u: failed assertion `%s\'\n",
               "/Users/vernjensen/Simple - Midnight Mansion/../SpriteWorldX/BlitKernel/BlitKernelGL.c"
               ,0x7f0,"glsurface->textureCoords != ((void *)0)");
  }
  puVar1 = (undefined4 *)_BKLoadGLfunctions();
  if (puVar1 == (undefined4 *)0x0) {
    _SDL_SetError("Failed to load GL functions");
    local_34 = 0xffffffff;
  }
  else {
    local_14 = *(short **)(param_1 + 0x5c);
    local_10 = (undefined4 *)(*(int *)(param_1 + 0x60) + param_2 * 0x30);
    for (local_1c = 0; local_1c < *(int *)(param_1 + 0x48); local_1c = local_1c + 1) {
      (*(code *)*puVar1)(7);
      (*(code *)puVar1[0x1a])(*local_10,local_10[1]);
      (*(code *)puVar1[0x18])((int)*local_14,(int)local_14[1]);
      (*(code *)puVar1[0x1a])(local_10[2],local_10[3]);
      (*(code *)puVar1[0x18])((int)local_14[2],(int)local_14[3]);
      (*(code *)puVar1[0x1a])(local_10[10],local_10[0xb]);
      (*(code *)puVar1[0x18])((int)local_14[10],(int)local_14[0xb]);
      (*(code *)puVar1[0x1a])(local_10[4],local_10[5]);
      (*(code *)puVar1[0x18])((int)local_14[4],(int)local_14[5]);
      (*(code *)puVar1[1])();
      local_14 = local_14 + 0xc;
      local_10 = local_10 + 0xc;
    }
    local_34 = 0;
  }
  return local_34;
}


// ---- _BKLoadGLfunctions @ 0x1d976 (949 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * _BKLoadGLfunctions(void)

{
  if (*(int *)PTR__GL_functions_loaded_001c4440 == 0) {
    _memset(PTR__GL_functions_001c4490,0,0xac);
    *(undefined **)PTR__GL_functions_001c4490 = PTR_001c4474;
    *(undefined **)(PTR__GL_functions_001c4490 + 4) = PTR_001c44b4;
    *(undefined **)(PTR__GL_functions_001c4490 + 8) = PTR_001c4424;
    *(undefined **)(PTR__GL_functions_001c4490 + 0xc) = PTR_001c44a4;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x10) = PTR_001c43f8;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x14) = PTR_001c443c;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x18) = PTR_001c4494;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x1c) = PTR_001c4478;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x20) = PTR_001c4448;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x24) = PTR_001c4444;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x28) = PTR_001c44a0;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x2c) = PTR_001c4484;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x30) = PTR_001c4418;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x34) = PTR_001c444c;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x38) = PTR_001c449c;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x3c) = PTR_001c4450;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x40) = PTR_001c4410;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x44) = PTR_001c4400;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x48) = PTR_001c4468;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x4c) = PTR_001c4438;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x50) = PTR_001c44b0;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x58) = PTR_001c43fc;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x54) = PTR_001c4460;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x5c) = PTR_001c440c;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x60) = PTR_001c43f4;
    *(undefined **)(PTR__GL_functions_001c4490 + 100) = PTR_001c4434;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x68) = PTR_001c447c;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x6c) = PTR_001c4408;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x70) = PTR_001c44b8;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x74) = PTR_001c4404;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x78) = PTR_001c4414;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x7c) = PTR_001c4430;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x80) = PTR_001c43f0;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x84) = PTR_001c43ec;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x88) = PTR_001c4454;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x8c) = PTR_001c4458;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x90) = PTR_001c445c;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x94) = PTR_001c4498;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x98) = PTR_001c448c;
    *(undefined **)(PTR__GL_functions_001c4490 + 0x9c) = PTR_001c4480;
    *(undefined **)(PTR__GL_functions_001c4490 + 0xa0) = PTR_001c441c;
    *(undefined **)(PTR__GL_functions_001c4490 + 0xa4) = PTR_001c4464;
    *(undefined **)(PTR__GL_functions_001c4490 + 0xa8) = PTR_001c44a8;
    *(undefined4 *)PTR__GL_functions_loaded_001c4440 = 1;
  }
  return PTR__GL_functions_001c4490;
}


// ---- _BKLoadGLcapabilities @ 0x1dd2b (1048 bytes)

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * _BKLoadGLcapabilities(void)

{
  void *pvVar1;
  char *pcVar2;
  char *pcVar3;
  char *pcVar4;
  int iVar5;
  undefined4 *puVar6;
  
  if (*(int *)PTR__GL_capabilities_loaded_001c4470 == 0) {
    puVar6 = (undefined4 *)PTR__GL_capabilities_001c442c;
    for (iVar5 = 10; iVar5 != 0; iVar5 = iVar5 + -1) {
      *puVar6 = 0;
      puVar6 = puVar6 + 1;
    }
    pvVar1 = _BKLoadGLfunctions();
    if (pvVar1 == (void *)0x0) {
      return (void *)0x0;
    }
    (**(code **)(PTR__GL_functions_001c4490 + 0x20))(0xd33,PTR__GL_capabilities_001c442c + 4);
    pcVar2 = (char *)(**(code **)(PTR__GL_functions_001c4490 + 0x10))
                               ("/ApplicationServices.framework/Versions/A/ApplicationServices");
    pcVar3 = (char *)(**(code **)(PTR__GL_functions_001c4490 + 0x10))
                               ("ApplicationServices.framework/Versions/A/ApplicationServices");
    if ((pcVar2 != (char *)0x0) && (pcVar3 != (char *)0x0)) {
      pcVar4 = _strstr(pcVar2,"1.2");
      if ((pcVar4 == (char *)0x0) &&
         ((pcVar4 = _strstr(pcVar2,"1.3"), pcVar4 == (char *)0x0 &&
          (pcVar4 = _strstr(pcVar3,"GL_EXT_texture_edge_clamp"), pcVar4 == (char *)0x0)))) {
        *(undefined4 *)(PTR__GL_capabilities_001c442c + 0xc) = 0;
      }
      else {
        *(undefined4 *)(PTR__GL_capabilities_001c442c + 0xc) = 1;
      }
      pcVar4 = _strstr(pcVar3,"GL_EXT_texture_rectangle");
      if ((pcVar4 == (char *)0x0) &&
         (pcVar4 = _strstr(pcVar3,"GL_ARB_texture_rectangle"), pcVar4 == (char *)0x0)) {
        *(undefined4 *)(PTR__GL_capabilities_001c442c + 0x10) = 0;
      }
      else {
        *(undefined4 *)(PTR__GL_capabilities_001c442c + 0x10) = 1;
      }
      pcVar4 = _strstr(pcVar2,"1.2");
      if (((pcVar4 == (char *)0x0) && (pcVar4 = _strstr(pcVar2,"1.3"), pcVar4 == (char *)0x0)) &&
         (pcVar4 = _strstr(pcVar3,"GL_EXT_bgra"), pcVar4 == (char *)0x0)) {
        *(undefined4 *)(PTR__GL_capabilities_001c442c + 0x14) = 0;
      }
      else {
        *(undefined4 *)(PTR__GL_capabilities_001c442c + 0x14) = 1;
      }
      pcVar4 = _strstr(pcVar2,"1.2");
      if (((pcVar4 == (char *)0x0) && (pcVar4 = _strstr(pcVar2,"1.3"), pcVar4 == (char *)0x0)) &&
         ((pcVar4 = _strstr(pcVar3,"GL_APPLE_packed_pixel"), pcVar4 == (char *)0x0 &&
          (pcVar4 = _strstr(pcVar3,"GL_APPLE_packed_pixels"), pcVar4 == (char *)0x0)))) {
        *(undefined4 *)(PTR__GL_capabilities_001c442c + 0x18) = 0;
      }
      else {
        *(undefined4 *)(PTR__GL_capabilities_001c442c + 0x18) = 1;
      }
      pcVar4 = _strstr(pcVar3,"GL_APPLE_client_storage");
      if (pcVar4 == (char *)0x0) {
        *(undefined4 *)(PTR__GL_capabilities_001c442c + 0x1c) = 0;
      }
      else {
        *(undefined4 *)(PTR__GL_capabilities_001c442c + 0x1c) = 1;
      }
      pcVar4 = _strstr(pcVar3,"GL_APPLE_texture_range");
      if (pcVar4 == (char *)0x0) {
        *(undefined4 *)(PTR__GL_capabilities_001c442c + 0x20) = 0;
      }
      else {
        *(undefined4 *)(PTR__GL_capabilities_001c442c + 0x20) = 1;
      }
      pcVar2 = _strstr(pcVar2,"2.0");
      if ((pcVar2 == (char *)0x0) &&
         (pcVar2 = _strstr(pcVar3,"GL_ARB_texture_non_power_of_two"), pcVar2 == (char *)0x0)) {
        *(undefined4 *)(PTR__GL_capabilities_001c442c + 0x24) = 0;
      }
      else {
        *(undefined4 *)(PTR__GL_capabilities_001c442c + 0x24) = 0;
      }
    }
    if ((*(int *)(PTR__GL_capabilities_001c442c + 0x10) == 0) ||
       (*(int *)(PTR__GL_capabilities_001c442c + 0x24) != 0)) {
      *(undefined4 *)PTR__GL_capabilities_001c442c = 0xde1;
    }
    else {
      *(undefined4 *)PTR__GL_capabilities_001c442c = 0x84f5;
    }
    if ((*(int *)(PTR__GL_capabilities_001c442c + 0x10) == 0) &&
       (*(int *)(PTR__GL_capabilities_001c442c + 0x24) == 0)) {
      *(undefined4 *)(PTR__GL_capabilities_001c442c + 8) = 1;
    }
    else {
      *(undefined4 *)(PTR__GL_capabilities_001c442c + 8) = 0;
    }
    if (*(int *)(PTR__GL_capabilities_001c442c + 4) == 0) {
      *(undefined4 *)(PTR__GL_capabilities_001c442c + 4) = 0x100;
    }
    if (0x800 < *(int *)(PTR__GL_capabilities_001c442c + 4)) {
      *(undefined4 *)(PTR__GL_capabilities_001c442c + 4) = 0x800;
    }
    *(undefined4 *)PTR__GL_capabilities_loaded_001c4470 = 1;
  }
  return PTR__GL_capabilities_001c442c;
}


// ---- _BKGetGLErrorMessage @ 0x1e143 (175 bytes)

char * _BKGetGLErrorMessage(int param_1)

{
  char *local_10;
  
  if (param_1 == 0x500) {
    local_10 = "GL_INVALID_ENUM -- An unacceptable value is specified for an enumerated argument.";
  }
  else if (param_1 == 0x501) {
    local_10 = "GL_INVALID_VALUE -- A numeric argument is out of range.";
  }
  else if (param_1 == 0x502) {
    local_10 = 
    "GL_INVALID_OPERATION -- The specified operation is not allowed in the current state.";
  }
  else if (param_1 == 0x503) {
    local_10 = "GL_STACK_OVERFLOW -- This command would cause a stack overflow.";
  }
  else if (param_1 == 0x504) {
    local_10 = "GL_STACK_UNDERFLOW -- This command would cause a stack underflow.";
  }
  else if (param_1 == 0x505) {
    local_10 = "GL_OUT_OF_MEMORY -- There is not enough memory left to execute the command.";
  }
  else if (param_1 == 0x8031) {
    local_10 = 
    "GL_TABLE_TOO_LARGE -- The specified table exceeds the implementation\'s maximum supported table size."
    ;
  }
  else {
    local_10 = "Unknown OpenGL error code";
  }
  return local_10;
}


