# compiled from:
# 1. A Super-Simple Makefile for Medium-Sized C/C++ Projects
# https://spin.atomicobject.com/2016/08/26/makefile-c-projects/
# 2. Auto-Dependency Generation
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/

# features:
# * out of source builds (build artifacts go to a folder separate from the
# source);
# * atomatic list of source files;
# * automatic dependencies;
# should build the project with the following structure:
# <project>/src/  // folder with source files and internal headers
# <project>/inc/  // folder containing headers with external interface
# build artifacts go to build/ folder which may be separate from the project
# location;
# makefile can be separate from the project location

PROJ_DIR = Astar_bidirectional
BUILD_DIR := .build
SRC_DIR := $(PROJ_DIR)/src

TARGET_BIN := $(BUILD_DIR)/a.out

# make the list of source files
SRC_FILES := $(shell find $(SRC_DIR) -name \*.cpp)
OBJ_FILES := $(SRC_FILES:%.cpp=$(BUILD_DIR)/%.o)
DEP_FILES := $(OBJ_FILES:.o=.d)
# flags for automatic dependencies generation
# write dependencies to temporary *.tmpd files first and rename to *.d files
# after successful compilation (to avoid corrupted dependency files in case
# of compilation failure)
DEPFLAGS = -MMD -MP -MF $(BUILD_DIR)/$*.tmpd

# comment this out if there is no include directories
#INC_DIRS := $(PROJ_DIR)/inc
# make include flags if there are include directories
ifdef INC_DIRS
CPPFLAGS := $(addprefix -I,$(INC_DIRS))
endif

CXXFLAGS = -O2 -std=c++14

$(TARGET_BIN): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

# make build/ and subdirectories if they do not exist
# the subdirectory structure of build/ is the same as src/
#
# @ is at the start of the shell command to supress echo of the command to the 
# output
#
# after the successful compilation rename the temporary dependency file to the
# final *.d
#
# touch object file to avoid it being older than dependency file which causes
# unnecessary rebuilds
$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
	mv -f $(BUILD_DIR)/$*.tmpd $(BUILD_DIR)/$*.d && touch $@

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)

-include $(DEP_FILES)
