# After Midnight for the PS3, with the PSL1GHT SDK.
#
# Builds in the ps3dev container (the same one GitHub Actions uses; see docs/PS3.md). From the
# repository root:
#
#   docker run --rm -v "$PWD":/src -w /src hldtux/ps3dev-sdl2 make -f platform/ps3/Makefile -j4
#
# Leaves build/ps3/After-Midnight-<version>-PS3.pkg (install it with multiMAN, the PS3's
# Package Manager, or RPCS3's File > Install Packages) and build/ps3/EBOOT.elf.
# "make -f platform/ps3/Makefile elf" builds the program alone, without packing the game data.

ifeq ($(strip $(PSL1GHT)),)
$(error PSL1GHT is not set: build in the ps3dev container (see docs/PS3.md))
endif

ROOT        := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../..)

include $(PSL1GHT)/ppu_rules

PLATFORM    := $(ROOT)/platform/ps3
OUT         := $(ROOT)/build/ps3
OBJ         := $(OUT)/obj
GAMEDATA    ?= $(ROOT)/original/Midnight Mansion HD.app/Contents/Resources/Game Data

TITLE       := After Midnight
APPID       := AMHD00001
CONTENTID   := UP0001-$(APPID)_00-0000000000000000
# "1.0" from CHANGELOG.md's first "## After Midnight X" heading; APP_VER wants "01.00"
VERSION     := $(shell sed -n 's/^## After Midnight \([0-9][0-9.]*\).*/\1/p' $(ROOT)/CHANGELOG.md | head -n 1)
APPVER      := $(shell echo "$(VERSION)" | awk -F. '{ printf "%02d.%02d", $$1, $$2 }')
PKGNAME     := After-Midnight-$(VERSION)-PS3

# The game's objects sit between GameDataBegin and GameDataEnd (see GameDataBegin.c)
GAMESOURCES := $(wildcard $(ROOT)/src/game/*.c) $(wildcard $(ROOT)/src/swx/*.c)
PS3SOURCES  := $(wildcard $(ROOT)/src/ps3/*.c)
SOURCES     := $(GAMESOURCES) $(PS3SOURCES)
SHADERS     := ps3gl_vp.vpo ps3gl_color_fp.fpo ps3gl_texture_fp.fpo ps3gl_repeat_fp.fpo ps3gl_opaque_fp.fpo
OBJECTS     := $(OBJ)/GameDataBegin.o $(patsubst $(ROOT)/src/%.c,$(OBJ)/%.o,$(GAMESOURCES)) $(OBJ)/GameDataEnd.o \
               $(patsubst $(ROOT)/src/%.c,$(OBJ)/%.o,$(PS3SOURCES)) $(patsubst %,$(OBJ)/shaders/%.o,$(SHADERS))

CFLAGS      := -O2 -g -std=gnu99 -mcpu=cell $(MACHDEP) -fsigned-char -DMM_PS3=1 \
               -Wall -Wno-unused-function -Wno-multichar -Wno-missing-braces -Wno-unused-variable \
               -Wno-pointer-sign -Wno-format-truncation \
               -I$(ROOT)/src/ps3 -I$(ROOT)/src/swx -I$(ROOT)/src/game -I$(OBJ)/shaders \
               -I$(ROOT)/third_party/stb -I$(ROOT)/third_party/dr_libs \
               $(LIBPSL1GHT_INC) -I$(PORTLIBS)/include -I$(PORTLIBS)/include/SDL2
LDFLAGS     := $(MACHDEP) -Wl,--gc-sections -Wl,-Map,$(OUT)/EBOOT.map
LIBS        := -L$(PORTLIBS)/lib $(LIBPSL1GHT_LIB) -lSDL2 -lgcm_sys -lrsx -lsysutil -lio -laudio -lrt -llv2 -lnet -lsysmodule -lm

.PHONY: all elf pkg gltest clean
.SECONDARY:
all: pkg
elf: $(OUT)/EBOOT.elf
pkg: $(OUT)/$(PKGNAME).pkg
gltest: $(OUT)/gltest.self

$(OBJ)/GameData%.o: $(PLATFORM)/GameData%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Every game file sees PS3Platform.h, which stands in for SDL's video and sound (see there)
$(OBJ)/game/%.o $(OBJ)/swx/%.o: CFLAGS += -include PS3Platform.h
$(OBJ)/%.o: $(ROOT)/src/%.c | $(OBJ)/shaders/headers
	@mkdir -p $(dir $@)
	@echo $(notdir $<)
	@$(CC) -MMD -MP $(CFLAGS) -c $< -o $@

# Shaders: Cg -> RSX microcode (cgcomp) -> an object with the bytes, and a header naming them
$(OBJ)/shaders/%.vpo: $(ROOT)/src/ps3/shaders/%.vcg
	@mkdir -p $(dir $@)
	@echo $(notdir $<)
	@$(CGCOMP) -v $< $@
$(OBJ)/shaders/%.fpo: $(ROOT)/src/ps3/shaders/%.fcg
	@mkdir -p $(dir $@)
	@echo $(notdir $<)
	@$(CGCOMP) -f $< $@
define shader_object
	@cd $(OBJ)/shaders && bin2s -a 64 $(notdir $<) | $(AS) -o $@
	@cd $(OBJ)/shaders && n=$$(echo $(notdir $<) | tr . _) && \
	  { echo "extern const u8 $${n}_end[];"; echo "extern const u8 $${n}[];"; echo "extern const u32 $${n}_size;"; } > $${n}.h
endef
$(OBJ)/shaders/%.vpo.o: $(OBJ)/shaders/%.vpo
	$(shader_object)
$(OBJ)/shaders/%.fpo.o: $(OBJ)/shaders/%.fpo
	$(shader_object)
$(OBJ)/shaders/headers: $(patsubst %,$(OBJ)/shaders/%.o,$(SHADERS))
	@touch $@

$(OUT)/EBOOT.elf: $(OBJECTS)
	@echo linking ... $(notdir $@)
	@$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@

# A test picture for the PS3 drawing alone (platform/ps3/gltest/gltest.c)
$(OUT)/gltest.elf: $(OBJ)/gltest.o $(OBJ)/ps3/PS3GL.o $(patsubst %,$(OBJ)/shaders/%.o,$(SHADERS))
	@$(CC) $^ $(LDFLAGS) $(LIBS) -o $@
$(OBJ)/gltest.o: $(PLATFORM)/gltest/gltest.c | $(OBJ)/shaders/headers
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
$(OUT)/gltest.self: $(OUT)/gltest.elf
	@$(STRIP) $< -o $(OUT)/gltest.stripped.elf
	@$(SPRX) $(OUT)/gltest.stripped.elf
	@$(SELF) $(OUT)/gltest.stripped.elf $@ > /dev/null

# The package: USRDIR holds the program, the original game's data and this version's extras
$(OUT)/$(PKGNAME).pkg: $(OUT)/EBOOT.elf $(PLATFORM)/ICON0.PNG $(PLATFORM)/PIC1.PNG
	@echo packing ... $(notdir $@)
	@rm -rf $(OUT)/pkg && mkdir -p $(OUT)/pkg/USRDIR
	@$(STRIP) $(OUT)/EBOOT.elf -o $(OUT)/EBOOT.stripped.elf
	@$(SPRX) $(OUT)/EBOOT.stripped.elf
	@$(SELF_NPDRM) $(OUT)/EBOOT.stripped.elf $(OUT)/pkg/USRDIR/EBOOT.BIN $(CONTENTID) > /dev/null
	@cp $(PLATFORM)/ICON0.PNG $(PLATFORM)/PIC1.PNG $(OUT)/pkg/
	@sed -e "s/@TITLE@/$(TITLE)/" -e "s/@APPID@/$(APPID)/" -e "s/@APPVER@/$(APPVER)/" \
	  $(PLATFORM)/sfo.xml > $(OUT)/sfo.xml
	@$(SFO) --title "$(TITLE)" --appid "$(APPID)" -f $(OUT)/sfo.xml $(OUT)/pkg/PARAM.SFO
	@cp -R "$(GAMEDATA)" "$(OUT)/pkg/USRDIR/Game Data"
	@cp -R "$(ROOT)/extras" "$(OUT)/pkg/USRDIR/Game Data Extras"
	@# PSL1GHT's pkg.py grows the package with "bytes +=", which takes hours for the game's
	@# 120,000 files; the same script with a bytearray makes the identical package in a minute
	@sed -e 's/dataToEncrypt = b""/dataToEncrypt = bytearray()/' \
	  -e 's/crypt(context, dataToEncrypt, header.dataSize)/crypt(context, bytes(dataToEncrypt), header.dataSize)/' \
	  $(PS3DEV)/bin/pkg.py > $(OUT)/pkg_fast.py
	@PYTHONPATH=$(PS3DEV)/bin python3 $(OUT)/pkg_fast.py --contentid $(CONTENTID) $(OUT)/pkg/ $@ > /dev/null
	@echo done: $@

clean:
	rm -rf $(OUT)

-include $(patsubst $(ROOT)/src/%.c,$(OBJ)/%.d,$(SOURCES))
