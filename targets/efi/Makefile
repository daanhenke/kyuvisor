CUR_DIR := $(dir $(firstword $(MAKEFILE_LIST)))

TARGET_NAMES := kyuloader.efi
BUILD_POSTFIX := efi

SOURCES := \
	main.cc \
	loader.cc \
	graphics.cc \
	log.cc \
	console.cc \
	pub.cc

include $(CUR_DIR)/../../make/common.mk
include $(CUR_DIR)/../barelib/Makefile

LD = lld-link

VISOR_INCLUDE_DIR=$(CUR_DIR)/../visor/include

CFLAGS = -target x86_64-pc-win32-coff -fno-stack-protector -fshort-wchar -mno-red-zone -I$(INCLUDE_DIR) -I$(BARELIB_INCLUDE_DIR) -I$(VISOR_INCLUDE_DIR) -fno-threadsafe-statics 
CXXFLAGS = $(CFLAGS) -std=c++20 -fno-exceptions
LDFLAGS = -subsystem:efi_application -nodefaultlib -dll -entry:efi_main

$(OUT_DIR)/kyuloader.efi: $(OBJECTS) $(BARELIB_OBJECTS)
	@mkdir -p $(dir $@)
	@$(LD) $(LDFLAGS) -out:$@ $^
	@mkdir -p $(OUT_DIR)/boot
	@cp $@ $(OUT_DIR)/boot/BOOTX64.efi

$(OBJECT_DIR)/%.cc.o: $(SOURCE_DIR)/%.cc
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -o $@ -c $<
