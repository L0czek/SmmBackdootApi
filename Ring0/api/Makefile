CXX         ?= g++
SOURCE_DIR   = $(shell pwd)
SRC_DIR      = $(SOURCE_DIR)/src
INC_DIR      = $(SOURCE_DIR)/inc
OBJ_DIR      = $(SOURCE_DIR)/obj
BUILD_DIR    = $(SOURCE_DIR)/build
OUT_BINARY   = $(BUILD_DIR)/api

ADD_INC_DIRS?= 
INCLUDES     = $(INC_DIR) $(ADD_INC_DIRS)
INC_PARAMS   = $(foreach path, $(INCLUDES), -I$(path))
SRC_FILES    = $(wildcard src/*.cc)
OBJ_FILES    = $(SRC_FILES:src%.cc=obj%.o)

CXX_FLAGS    = -Wall -Wextra -std=c++17 $(INC_PARAMS) -ggdb
LD_FLAGS     = -lflashrom 

GREEN_COLOR = \\033[0;32m
RED_COLOR   = \\033[0;31m
NC          = \\033[0;m

.PHONY: build
build: $(OBJ_FILES)
	@if ! [ -d build ]; then \
		mkdir $(BUILD_DIR); \
	fi;
	@echo "   LD   $(OUT_BINARY)"
	@$(CXX) $(OBJ_FILES) -o $(OUT_BINARY) $(LD_FLAGS) 


obj/%.o: src/%.cc
	@if ! [ -d $(OBJ_DIR) ]; then \
		mkdir $(OBJ_DIR); \
	fi;
	@echo "  CXX   $^"
	@$(CXX) -c $(CXX_FLAGS) $^ -o $@


.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)
	rm -f $(OUT_BINARY)

.PHONY: compile-commands
compile-commands: clean
	@bear -- $(MAKE) build



