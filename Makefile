CUR_DIR := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
TARGETS_DIR := $(CUR_DIR)/targets

TARGETS := efi visor

.PHONY: all
all: $(TARGETS)

$(TARGETS):
	$(MAKE) -f $(TARGETS_DIR)/$@/Makefile