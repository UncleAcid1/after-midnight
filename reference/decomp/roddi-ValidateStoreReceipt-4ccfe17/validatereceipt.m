// Ghidra decompilation of functions from original source file:
//   ../roddi-ValidateStoreReceipt-4ccfe17/validatereceipt.m

// ---- _GLSetAttributes @ 0x9b2eb (710 bytes)

int _GLSetAttributes(void)

{
  char cVar1;
  undefined4 uVar2;
  int iVar3;
  cfstringStruct *pcVar4;
  undefined4 uVar5;
  int local_50;
  undefined4 local_48;
  cfstringStruct *local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 *local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  local_24 = 0;
  local_20 = _SecKeychainOpen("/System/Library/Keychains/SystemRootCertificates.keychain",&local_24)
  ;
  if (local_20 == 0) {
    local_1c = _CFArrayCreate(*(undefined4 *)PTR_001c51d4,&local_24,1,PTR_001c52bc);
    if (local_24 != 0) {
      _CFRelease(local_24);
    }
    local_28 = 0;
    local_20 = _SecKeychainSearchCreateFromAttributes(local_1c,0x80001000,0,&local_28);
    if (local_20 == 0) {
      local_2c = 0;
      local_18 = 0;
      local_20 = 0;
      while( true ) {
        iVar3 = _SecKeychainSearchCopyNext(local_28,&local_2c);
        if ((iVar3 != 0) || (local_18 != 0)) break;
        local_40 = 0x6c61626c;
        local_34 = 1;
        local_30 = &local_40;
        _SecKeychainItemCopyContent(local_2c,0,&local_34,0,0);
        local_14 = _objc_msgSend("NSData","dataWithBytesNoCopy:length:freeWhenDone:",local_38,
                                 local_3c,0);
        uVar2 = _objc_msgSend("NSString","alloc");
        uVar5 = 4;
        local_10 = _objc_msgSend(uVar2,"initWithData:encoding:",local_14,4);
        pcVar4 = &cf_AppleRootCA;
        cVar1 = _objc_msgSend(local_10,"isEqualToString:",&cf_AppleRootCA);
        if (cVar1 != '\0') {
          _SecCertificateGetData(local_2c,&local_48);
          pcVar4 = local_44;
          uVar5 = local_48;
          local_18 = _objc_msgSend("NSData","dataWithBytes:length:",local_44,local_48);
        }
        _SecKeychainItemFreeContent(&local_34,0);
        if ((local_2c != 0) && (local_2c != 0)) {
          _CFRelease(local_2c);
        }
        _objc_msgSend(local_10,"release",pcVar4,uVar5);
      }
      if (local_1c != 0) {
        _CFRelease(local_1c);
      }
      if (local_28 != 0) {
        _CFRelease(local_28);
      }
      local_50 = local_18;
    }
    else {
      if (local_28 != 0) {
        _CFRelease(local_28);
      }
      if (local_1c != 0) {
        _CFRelease(local_1c);
      }
      local_50 = 0;
    }
  }
  else {
    if (local_24 != 0) {
      _CFRelease(local_24);
    }
    local_50 = 0;
  }
  return local_50;
}


// ---- _CFCreateWindow @ 0x9b5b1 (1676 bytes)

undefined4 _CFCreateWindow(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  long len;
  BIO_METHOD *type;
  undefined4 local_80;
  STACK *local_70;
  long local_6c;
  int local_68;
  uchar *local_64;
  long local_60;
  int local_5c;
  int local_58;
  STACK *local_54;
  undefined4 local_50;
  char *local_4c;
  FILE *local_48;
  PKCS7 *local_44;
  int local_40;
  X509_STORE *local_3c;
  PKCS7_SIGNED *local_38;
  STACK *local_34;
  undefined4 local_30;
  X509 *local_2c;
  BIO *local_28;
  STACK *local_24;
  uint local_20;
  uint local_1c;
  int local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  local_50 = _GLSetAttributes();
  _ERR_load_PKCS7_strings();
  _ERR_load_X509_strings();
  _OpenSSL_add_all_digests();
  uVar1 = _objc_msgSend(param_1,"stringByStandardizingPath");
  local_4c = (char *)_objc_msgSend(uVar1,"fileSystemRepresentation");
  local_48 = _fopen(local_4c,"rb");
  if (local_48 == (FILE *)0x0) {
    local_80 = 0;
  }
  else {
    local_44 = _d2i_PKCS7_fp(local_48,(PKCS7 **)0x0);
    _fclose(local_48);
    if (local_44 == (PKCS7 *)0x0) {
      local_80 = 0;
    }
    else {
      iVar2 = _OBJ_obj2nid(local_44->type);
      if (iVar2 == 0x16) {
        iVar2 = _OBJ_obj2nid(((local_44->d).sign)->contents->type);
        if (iVar2 == 0x15) {
          local_40 = 0;
          local_3c = _X509_STORE_new();
          if (local_3c != (X509_STORE *)0x0) {
            local_64 = (uchar *)_objc_msgSend(local_50,"bytes");
            len = _objc_msgSend(local_50,"length");
            local_2c = _d2i_X509((X509 **)0x0,&local_64,len);
            if (local_2c != (X509 *)0x0) {
              type = _BIO_s_mem();
              local_28 = _BIO_new(type);
              _X509_STORE_add_cert(local_3c,local_2c);
              if (local_28 != (BIO *)0x0) {
                local_40 = _PKCS7_verify(local_44,(STACK *)0x0,local_3c,(BIO *)0x0,local_28,0);
                _BIO_free(local_28);
              }
              _X509_free(local_2c);
            }
            _X509_STORE_free(local_3c);
          }
          _EVP_cleanup();
          if (local_40 == 1) {
            local_38 = (((local_44->d).sign)->contents->d).sign;
            local_54 = local_38->cert;
            local_34 = (STACK *)((int)&local_54->num + (int)local_38->version);
            local_58 = 0;
            local_5c = 0;
            local_60 = 0;
            _ASN1_get_object((uchar **)&local_54,&local_60,&local_58,&local_5c,
                             (int)local_34 - (int)local_54);
            if (local_58 == 0x11) {
              local_30 = _objc_msgSend("NSMutableDictionary","dictionary");
              while ((local_54 < local_34 &&
                     (_ASN1_get_object((uchar **)&local_54,&local_60,&local_58,&local_5c,
                                       (int)local_34 - (int)local_54), local_58 == 0x10))) {
                local_24 = (STACK *)((int)&local_54->num + local_60);
                local_20 = 0;
                local_1c = 0;
                _ASN1_get_object((uchar **)&local_54,&local_60,&local_58,&local_5c,
                                 (int)local_24 - (int)local_54);
                if ((local_58 == 2) && (local_60 == 1)) {
                  local_20 = (uint)(byte)local_54->num;
                }
                local_54 = (STACK *)((int)&local_54->num + local_60);
                _ASN1_get_object((uchar **)&local_54,&local_60,&local_58,&local_5c,
                                 (int)local_24 - (int)local_54);
                if ((local_58 == 2) && (local_60 == 1)) {
                  local_1c = (uint)(byte)local_54->num;
                }
                local_54 = (STACK *)((int)&local_54->num + local_60);
                if ((1 < (int)local_20) && ((int)local_20 < 6)) {
                  local_18 = 0;
                  _ASN1_get_object((uchar **)&local_54,&local_60,&local_58,&local_5c,
                                   (int)local_24 - (int)local_54);
                  if (local_58 == 4) {
                    local_14 = _objc_msgSend("NSData","dataWithBytes:length:",local_54,local_60);
                    if (((local_20 == 2) || (local_20 == 4)) || (local_20 == 5)) {
                      if (local_20 == 4) {
                        local_18 = *(int *)PTR__kReceiptOpaqueValue_001c52b8;
                      }
                      else if (local_20 == 5) {
                        local_18 = *(int *)PTR__kReceiptHash_001c52c8;
                      }
                      else if (local_20 == 2) {
                        local_18 = *(int *)PTR__kReceiptBundleIdentifierData_001c52b4;
                      }
                      if (local_18 != 0) {
                        _objc_msgSend(local_30,"setObject:forKey:",local_14,local_18);
                      }
                    }
                    if ((local_20 == 2) || (local_20 == 3)) {
                      local_68 = 0;
                      local_6c = 0;
                      local_70 = local_54;
                      _ASN1_get_object((uchar **)&local_70,&local_6c,&local_68,&local_5c,
                                       (int)local_24 - (int)local_54);
                      if (local_68 == 0xc) {
                        if (local_20 == 2) {
                          local_18 = *(int *)PTR__kReceiptBundleIdentifier_001c52b0;
                        }
                        else if (local_20 == 3) {
                          local_18 = *(int *)PTR__kReceiptVersion_001c52c0;
                        }
                        if (local_18 != 0) {
                          uVar1 = _objc_msgSend("NSString","alloc");
                          local_10 = _objc_msgSend(uVar1,"initWithBytes:length:encoding:",local_70,
                                                   local_6c,4);
                          _objc_msgSend(local_30,"setObject:forKey:",local_10,local_18);
                          _objc_msgSend(local_10,"release");
                        }
                      }
                    }
                  }
                  local_54 = (STACK *)((int)&local_54->num + local_60);
                }
                for (; local_54 < local_24; local_54 = (STACK *)((int)&local_54->num + local_60)) {
                  _ASN1_get_object((uchar **)&local_54,&local_60,&local_58,&local_5c,
                                   (int)local_24 - (int)local_54);
                }
              }
              _PKCS7_free(local_44);
              local_80 = local_30;
            }
            else {
              _PKCS7_free(local_44);
              local_80 = 0;
            }
          }
          else {
            _PKCS7_free(local_44);
            local_80 = 0;
          }
        }
        else {
          _PKCS7_free(local_44);
          local_80 = 0;
        }
      }
      else {
        _PKCS7_free(local_44);
        local_80 = 0;
      }
    }
  }
  return local_80;
}


// ---- _SWCreateXMLSprite @ 0x9bc3d (406 bytes)

int _SWCreateXMLSprite(void)

{
  int local_30;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  
  local_10 = 0;
  local_1c = _IOMasterPort(0,&local_20);
  if (local_1c == 0) {
    local_18 = _IOBSDNameMatching(local_20,0,"en0");
    if (local_18 == 0) {
      _puts("IOBSDNameMatching returned empty dictionary");
      local_30 = 0;
    }
    else {
      local_1c = _IOServiceGetMatchingServices(local_20,local_18,&local_24);
      if (local_1c == 0) {
        while( true ) {
          local_14 = _IOIteratorNext(local_24);
          if (local_14 == 0) break;
          local_1c = _IORegistryEntryGetParentEntry(local_14,"IOService",&local_28);
          if (local_1c == 0) {
            if (local_10 != 0) {
              _CFRelease(local_10);
            }
            local_10 = _IORegistryEntryCreateCFProperty
                                 (local_28,&cf_IOMACAddress,*(undefined4 *)PTR_001c51d4,0);
            _IOObjectRelease(local_28);
          }
          else {
            _printf("IORegistryEntryGetParentEntry returned %d\n",local_1c);
          }
          _IOObjectRelease(local_14);
        }
        local_30 = local_10;
      }
      else {
        _printf("IOServiceGetMatchingServices returned %d\n",local_1c);
        local_30 = 0;
      }
    }
  }
  else {
    _printf("IOMasterPort returned %d\n",local_1c);
    local_30 = 0;
  }
  return local_30;
}


// ---- _MMOpenFileAtPathUsingName @ 0x9bdd3 (1448 bytes)

undefined4 _MMOpenFileAtPathUsingName(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  char cVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  uchar *md;
  size_t n;
  uchar *d;
  
  uVar2 = _objc_msgSend("NSAutoreleasePool","alloc");
  uVar2 = _objc_msgSend(uVar2,"init");
  uVar3 = _objc_msgSend("NSBundle","mainBundle");
  uVar3 = _objc_msgSend(uVar3,"objectForInfoDictionaryKey:",&cf_CFBundleShortVersionString);
  cVar1 = _objc_msgSend(param_2,"isEqualToString:",uVar3);
  if (cVar1 == '\0') {
    uVar4 = _objc_msgSend("NSAssertionHandler","currentHandler",uVar3);
    uVar5 = _objc_msgSend("NSString","stringWithCString:",
                          "/Users/vernjensen/Simple - Midnight Mansion/../roddi-ValidateStoreReceipt-4ccfe17/validatereceipt.m"
                         );
    uVar3 = _objc_msgSend("NSString","stringWithCString:",PTR____PRETTY_FUNCTION___94186_001c52c4);
    _objc_msgSend(uVar4,"handleFailureInFunction:file:lineNumber:description:",uVar3,uVar5,0x1a5,
                  &
                  cf_validateReceiptAtPathASSERTFAILED_passedbundleVersionisnotthesameastheCFBundleShortVersionStringintheplist_
                  ,0,0,0,0,0);
  }
  uVar3 = _objc_msgSend("NSBundle","mainBundle",uVar3);
  uVar3 = _objc_msgSend(uVar3,"bundleIdentifier");
  cVar1 = _objc_msgSend(param_3,"isEqualToString:",uVar3);
  if (cVar1 == '\0') {
    uVar4 = _objc_msgSend("NSAssertionHandler","currentHandler",uVar3);
    uVar5 = _objc_msgSend("NSString","stringWithCString:",
                          "/Users/vernjensen/Simple - Midnight Mansion/../roddi-ValidateStoreReceipt-4ccfe17/validatereceipt.m"
                         );
    uVar3 = _objc_msgSend("NSString","stringWithCString:",PTR____PRETTY_FUNCTION___94186_001c52c4);
    _objc_msgSend(uVar4,"handleFailureInFunction:file:lineNumber:description:",uVar3,uVar5,0x1a7,
                  &
                  cf_validateReceiptAtPathASSERTFAILED_passedbundleIdentifierisnotthesameastheoneintheplist_
                  ,0,0,0,0,0);
  }
  iVar6 = _CFCreateWindow(param_1);
  if (iVar6 == 0) {
    _objc_msgSend(uVar2,"release",uVar3);
  }
  else {
    iVar7 = _SWCreateXMLSprite();
    _objc_msgSend(iVar7,"autorelease",uVar3);
    if (iVar7 == 0) {
      _objc_msgSend(uVar2,"release");
    }
    else {
      uVar3 = _objc_msgSend("NSMutableData","data");
      _objc_msgSend(uVar3,"appendData:",iVar7);
      uVar4 = _objc_msgSend(iVar6,"objectForKey:",*(undefined4 *)PTR__kReceiptOpaqueValue_001c52b8);
      _objc_msgSend(uVar3,"appendData:",uVar4);
      uVar4 = _objc_msgSend(iVar6,"objectForKey:",
                            *(undefined4 *)PTR__kReceiptBundleIdentifierData_001c52b4);
      _objc_msgSend(uVar3,"appendData:",uVar4);
      uVar4 = _objc_msgSend("NSMutableData","dataWithLength:",0x14);
      md = (uchar *)_objc_msgSend(uVar4,"mutableBytes");
      n = _objc_msgSend(uVar3,"length");
      d = (uchar *)_objc_msgSend(uVar3,"bytes");
      _SHA1(d,n,md);
      uVar3 = _objc_msgSend(iVar6,"objectForKey:",*(undefined4 *)PTR__kReceiptHash_001c52c8);
      cVar1 = _objc_msgSend(uVar4,"isEqualToData:",uVar3);
      if (cVar1 != '\0') {
        uVar3 = _objc_msgSend(iVar6,"objectForKey:",*(undefined4 *)PTR__kReceiptVersion_001c52c0);
        cVar1 = _objc_msgSend(param_2,"isEqualToString:",uVar3);
        if (cVar1 != '\0') {
          uVar3 = _objc_msgSend(iVar6,"objectForKey:",
                                *(undefined4 *)PTR__kReceiptBundleIdentifier_001c52b0);
          cVar1 = _objc_msgSend(param_3,"isEqualToString:",uVar3);
          if (cVar1 != '\0') {
            _objc_msgSend(uVar2,"release",uVar3);
            return 1;
          }
        }
      }
      _objc_msgSend(uVar2,"release",uVar3);
    }
  }
  return 0;
}


