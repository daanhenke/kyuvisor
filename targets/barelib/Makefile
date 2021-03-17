BARELIB_PROJECT_DIR := $(CUR_DIR)/../barelib

BARELIB_SOURCE_DIR := $(BARELIB_PROJECT_DIR)/source
BARELIB_INCLUDE_DIR := $(BARELIB_PROJECT_DIR)/include

BARELIB_SOURCES := \
	string.cc

BARELIB_OBJECTS := $(addprefix $(OBJECT_DIR)/barelib/,$(addsuffix .o,$(BARELIB_SOURCES)))

$(OBJECT_DIR)/barelib/%.cc.o: $(BARELIB_SOURCE_DIR)/%.cc
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -o $@ -c $<
