CUR_DIR := $(dir $(firstword $(MAKEFILE_LIST)))

TARGET_NAMES := 
BUILD_POSTFIX := visor

SOURCES := 

include $(CUR_DIR)/../../make/common.mk

IMAGES_DIR := images
IMAGES_NAMES := \
	images/font12.ff \
	images/crusty-smiley.ff
IMAGES_TARGETS := $(addprefix $(OUT_DIR)/,$(IMAGES_NAMES))

all: $(IMAGES_TARGETS)

$(OUT_DIR)/%: $(CUR_DIR)/%
	@mkdir -p $(dir $@)
	cp $< $@
