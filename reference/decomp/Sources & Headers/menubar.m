// Ghidra decompilation of functions from original source file:
//   Sources & Headers/menubar.m

// ---- _initMMMenuBar @ 0x99239 (1993 bytes)

void _initMMMenuBar(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  
  uVar1 = _objc_msgSend("NSAutoreleasePool","alloc");
  uVar1 = _objc_msgSend(uVar1,"init");
  uVar2 = _objc_msgSend("MMMenuController","alloc");
  uVar2 = _objc_msgSend(uVar2,"init");
  uVar3 = _objc_msgSend(*(undefined4 *)PTR_001c52a0,"mainMenu");
  uVar3 = _objc_msgSend(uVar3,"itemAtIndex:",0);
  uVar3 = _objc_msgSend(uVar3,"submenu");
  _objc_msgSend(uVar3,"removeItemAtIndex:",0);
  _objc_msgSend(uVar3,"removeItemAtIndex:",0);
  _objc_msgSend(uVar3,"removeItemAtIndex:",0);
  _objc_msgSend(uVar3,"removeItemAtIndex:",0);
  _objc_msgSend(uVar3,"removeItemAtIndex:",0);
  _objc_msgSend(uVar3,"removeItemAtIndex:",0);
  _objc_msgSend(uVar3,"removeItemAtIndex:",0);
  _objc_msgSend(uVar3,"removeItemAtIndex:",0);
  _objc_msgSend(uVar3,"removeItemAtIndex:",0);
  uVar4 = _objc_msgSend(*(undefined4 *)PTR_001c52a0,"mainMenu");
  _objc_msgSend(uVar4,"removeItemAtIndex:",1);
  uVar4 = _objc_msgSend(&cf_Quit,"stringByAppendingString:",&cf_MidnightMansionHD);
  uVar4 = _objc_msgSend(uVar3,"insertItemWithTitle:action:keyEquivalent:atIndex:",uVar4,
                        "handleQuit:",&cf_q,0);
  _objc_msgSend(uVar4,"setTarget:",uVar2);
  uVar4 = _objc_msgSend("NSMenuItem","separatorItem");
  _objc_msgSend(uVar3,"insertItem:atIndex:",uVar4,0);
  _objc_msgSend(uVar3,"insertItemWithTitle:action:keyEquivalent:atIndex:",&cf_ShowAll,
                "unhideAllApplications:",&cf___,0);
  _objc_msgSend(uVar3,"insertItemWithTitle:action:keyEquivalent:atIndex:",&cf_HideOthers,
                "hideOtherApplications:",&cf___,0);
  uVar4 = _objc_msgSend(&cf_Hide,"stringByAppendingString:",&cf_MidnightMansionHD);
  _objc_msgSend(uVar3,"insertItemWithTitle:action:keyEquivalent:atIndex:",uVar4,"hide:",&cf_h,0);
  uVar4 = _objc_msgSend("NSMenuItem","separatorItem");
  _objc_msgSend(uVar3,"insertItem:atIndex:",uVar4,0);
  uVar4 = _objc_msgSend(uVar3,"insertItemWithTitle:action:keyEquivalent:atIndex:",&cf_DisplayStats,
                        "handleDisplayStats:",&cf_d,0);
  _objc_msgSend(uVar4,"setTarget:",uVar2);
  uVar4 = _objc_msgSend(uVar3,"insertItemWithTitle:action:keyEquivalent:atIndex:",&cf_MusicOn,
                        "handleMusicOn:",&cf_m,0);
  _objc_msgSend(uVar4,"setTarget:",uVar2);
  uVar4 = _objc_msgSend(uVar3,"insertItemWithTitle:action:keyEquivalent:atIndex:",&cf_Fullscreen,
                        "handleFullscreen:",&cf_f,0);
  _objc_msgSend(uVar4,"setTarget:",uVar2);
  uVar4 = _objc_msgSend("NSMenuItem","separatorItem");
  _objc_msgSend(uVar3,"insertItem:atIndex:",uVar4,0);
  uVar4 = _objc_msgSend(uVar3,"insertItemWithTitle:action:keyEquivalent:atIndex:",&cf_OpenReplay___,
                        "handleOpenReplay:",&cf_m,0);
  _objc_msgSend(uVar4,"setTarget:",uVar2);
  uVar4 = _objc_msgSend(uVar3,"insertItemWithTitle:action:keyEquivalent:atIndex:",
                        &cf_StartRecordingReplay,"handleStartRecordingReplay:",&cf_r,0);
  _objc_msgSend(uVar4,"setTarget:",uVar2);
  uVar4 = _objc_msgSend("NSMenuItem","separatorItem");
  _objc_msgSend(uVar3,"insertItem:atIndex:",uVar4,0);
  uVar4 = _objc_msgSend(uVar3,"insertItemWithTitle:action:keyEquivalent:atIndex:",&cf_AbortGame___,
                        "handleAbortGame:",&cf_a,0);
  _objc_msgSend(uVar4,"setTarget:",uVar2);
  uVar4 = _objc_msgSend("NSMenuItem","separatorItem");
  _objc_msgSend(uVar3,"insertItem:atIndex:",uVar4,0);
  uVar4 = _objc_msgSend(uVar3,"insertItemWithTitle:action:keyEquivalent:atIndex:",&cf_Preferences___
                        ,"handlePreferences:",&cf__,0);
  _objc_msgSend(uVar4,"setTarget:",uVar2);
  _objc_msgSend(uVar3,"setAutoenablesItems:",0);
  _objc_msgSend(uVar2,"updateMenuBar");
  *(undefined4 *)PTR__gMenuController_001c529c = uVar2;
  _objc_msgSend(uVar1,"release");
  return;
}


// ---- _UpdateMenuBar @ 0x99a02 (149 bytes)

void _UpdateMenuBar(void)

{
  undefined4 uVar1;
  
  uVar1 = _objc_msgSend("NSAutoreleasePool","alloc");
  uVar1 = _objc_msgSend(uVar1,"init");
  if (*(int *)PTR__gMenuController_001c529c != 0) {
    _objc_msgSend(*(undefined4 *)PTR__gMenuController_001c529c,"updateMenuBar");
  }
  _objc_msgSend(uVar1,"release");
  return;
}


