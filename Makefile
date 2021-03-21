CUR_DIR := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
TARGETS_DIR := $(CUR_DIR)targets

TARGETS := efi visor static-resources

ISO := $(CUR_DIR)build/iso.iso

.PHONY: all
all: $(TARGETS) $(ISO)

.PHONY: clean
clean:
	@rm -r $(CUR_DIR)build

.PHONY: iso
iso: $(ISO)

$(ISO): $(TARGETS)
	genisoimage -o $@ -r -J $(CUR_DIR)build/out

$(TARGETS):
	$(MAKE) -f $(TARGETS_DIR)/$@/Makefile