
# OS specific commnads
ifeq ($(OS),Windows_NT)
	ifeq ($(shell uname -s),)
		CLEAN = del /S /F /Q
		MKDIR = mkdir
	else
		CLEAN = rm -rf
		MKDIR = mkdir -p
	endif
		TARGET_EXTENSION=exe
	else
		CLEAN = rm -rf
		MKDIR = mkdir -p
		TARGET_EXTENSION=out
endif

export MKDIR
export TARGET_EXTENSION

.PHONY: build test clean

# Paths
PATH_INCLUDE = include/
PATH_SRC = src/
PATH_TEST = test/

PATH_BUILD = build/
PATH_OBJECTS = $(PATH_BUILD)objs/
PATH_RESULTS = $(PATH_BUILD)results/
PATH_EXECUTABLES = $(PATH_BUILD)exec/

# Directories
DIRS_SRC = $(shell find $(PATH_SRC) -mindepth 1 -maxdepth 1 -type d -exec basename {} \;)

# Files
FILES_SRC = $(shell find $(PATH_SRC) -type f -name *.c)
FILES_INCLUDE = $(shell find $(PATH_INCLUDE) -type f -name *.h)

SO_FILE = $(PATH_OBJECTS)libclib.so

# Compiler
CC = gcc
SO_FLAGS = -fPIC -shared
DEBUG_FLAGS = -g -O0 -Wall -Werror -Wextra -pedantic
INCLUDE_FLAGS = -I$(PATH_INCLUDE) $(foreach dir,$(DIRS_SRC),-I$(PATH_INCLUDE)$(dir))

BREAK = "\n--------------------------------------------------\n"

export PATH_TEST
export PATH_OBJECTS
export PATH_RESULTS
export PATH_EXECUTABLES
export DIRS_SRC
export SO_FILE
export CC
export DEBUG_FLAGS
export INCLUDE_FLAGS
export BREAK

# Build the shared object
build: $(PATH_OBJECTS) $(FILES_SRC) $(FILES_INCLUDE)
	@echo "BUILDING SHARED OBJECT: $(SO_FILE)"
	$(CC) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) $(SO_FLAGS) -o $(SO_FILE) $(FILES_SRC)
	@echo $(BREAK)

# Unit tests
test: build
	$(MAKE) -f $(PATH_TEST)Makefile

# Create build directories
$(PATH_BUILD):
	$(MKDIR) $(PATH_BUILD)
	@echo $(BREAK)
$(PATH_OBJECTS):
	$(MKDIR) $(PATH_OBJECTS)
	@echo $(BREAK)	
$(PATH_RESULTS):
	$(MKDIR) $(PATH_RESULTS)
	@echo $(BREAK)

clean:
	@echo "CLEANING..."
	$(CLEAN) $(PATH_BUILD)*
	@echo $(BREAK)

.PRECIOUS: $(PATH_OBJECTS)%.o
