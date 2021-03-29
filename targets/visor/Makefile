CUR_DIR := $(dir $(firstword $(MAKEFILE_LIST)))

TARGET_NAMES := kyuvisor.elf
BUILD_POSTFIX := visor

SOURCES := \
	main.cc \
	featurecheck.cc \
	utilities.cc \
	vcpu/VirtualizedCPU.cc \
	vcpu/CPUContext_Methods.asm \
	mm/MemoryAllocator.cc \
	mm/PageBitmap.cc \
	asm/instructions.asm

include $(CUR_DIR)/../../make/common.mk
include $(CUR_DIR)/../barelib/Makefile

ASFLAGS = -g -f elf64 -i $(INCLUDE_DIR)
CFLAGS = -fpic -fno-stack-protector -mno-red-zone -I$(INCLUDE_DIR) -I$(BARELIB_INCLUDE_DIR) -fno-threadsafe-statics
CXXFLAGS = $(CFLAGS) -std=c++20 -fno-exceptions
LDFLAGS = -z nodefaultlib -e entrypoint 

$(OUT_DIR)/kyuvisor.elf: $(OBJECTS) $(BARELIB_OBJECTS)
	@mkdir -p $(dir $@)
	@$(LD) $(LDFLAGS) -o $@ $^

$(OBJECT_DIR)/%.cc.o: $(SOURCE_DIR)/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(OBJECT_DIR)/%.c.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJECT_DIR)/%.asm.o: $(SOURCE_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<
