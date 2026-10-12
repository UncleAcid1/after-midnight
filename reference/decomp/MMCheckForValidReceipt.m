// Ghidra decompilation of functions from original source file:
//   MMCheckForValidReceipt.m

// ---- _MMCheckForValidReceipt @ 0x9b176 (373 bytes)

void MMCheckForValidReceipt(void)

{
  char cVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  cfstringStruct *pcVar5;
  undefined4 uVar6;
  
  uVar2 = _objc_msgSend("NSAutoreleasePool","alloc");
  uVar2 = _objc_msgSend(uVar2,"init");
  uVar3 = _objc_msgSend("NSBundle","mainBundle");
  uVar3 = _objc_msgSend(uVar3,"bundlePath");
  uVar3 = _objc_msgSend(uVar3,"stringByAppendingPathComponent:",&cf_Contents__MASReceipt_receipt);
  uVar4 = _objc_msgSend("NSString","alloc");
  uVar6 = 0x1e;
  uVar4 = _objc_msgSend(uVar4,"initWithCString:encoding:","1.0.1",0x1e);
  pcVar5 = &cf_com_actionsoft_midnightmansionepisode1;
  _printf("Validating Mac App Store receipt for Midnight Mansion HD Episode %d, version %s\n",1,
          "1.0.1");
  cVar1 = _MMOpenFileAtPathUsingName(uVar3,uVar4,&cf_com_actionsoft_midnightmansionepisode1);
  if (cVar1 == '\0') {
    _printf(
           "Validation failed! Exiting 173 to let MacOS prompt user to sign in to Mac App Store... "
           );
                    /* WARNING: Subroutine does not return */
    _exit(0xad);
  }
  _puts("Validation passed! ");
  _objc_msgSend(uVar2,"release",pcVar5,uVar6);
  return;
}


