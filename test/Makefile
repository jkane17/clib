
.PHONY: run

PREFIX = test_

_PATH_RESULTS = $(PATH_RESULTS)test/
_PATH_OBJECTS = $(PATH_OBJECTS)test/
_PATH_EXECUTABLES = $(PATH_EXECUTABLES)test/
PATH_UNITY = $(PATH_TEST)unity/

FILES = $(shell find $(PATH_TEST) -type f -name *$(PREFIX)*.c)
RESULTS = $(patsubst $(PATH_TEST)%.c,$(_PATH_RESULTS)%.txt,$(FILES))

_INCLUDE_FLAGS = $(INCLUDE_FLAGS) -I$(PATH_UNITY)

PASSED = `find $(_PATH_RESULTS) -type f -name "*.txt" -exec grep -sh "PASS" {} +`
FAIL = `find $(_PATH_RESULTS) -type f -name "*.txt" -exec grep -sh "FAIL" {} +`
IGNORE = `find $(_PATH_RESULTS) -type f -name "*.txt" -exec grep -sh "IGNORE" {} +`

run: $(_PATH_RESULTS) $(_PATH_OBJECTS) $(_PATH_EXECUTABLES) $(RESULTS)
	@echo "-----------------------\nIGNORES:\n-----------------------"
	@echo "$(IGNORE)"
	@echo "-----------------------\nFAILURES:\n-----------------------"
	@echo "$(FAIL)"
	@echo "-----------------------\nPASSED:\n-----------------------"
	@echo "$(PASSED)"
	@echo "\nDONE"

# Create build directories
$(_PATH_RESULTS):
	$(MKDIR) $(_PATH_RESULTS)
	$(foreach dir,$(DIRS_SRC),$(MKDIR) $(_PATH_RESULTS)$(dir);)
	@echo $(BREAK)
$(_PATH_OBJECTS):
	$(MKDIR) $(_PATH_OBJECTS)
	$(foreach dir,$(DIRS_SRC),$(MKDIR) $(_PATH_OBJECTS)$(dir);)
	@echo $(BREAK)
$(_PATH_EXECUTABLES):
	$(MKDIR) $(_PATH_EXECUTABLES)
	$(foreach dir,$(DIRS_SRC),$(MKDIR) $(_PATH_EXECUTABLES)$(dir);)
	@echo $(BREAK)

# Execute unit tests
$(_PATH_RESULTS)%.txt: $(_PATH_EXECUTABLES)%.$(TARGET_EXTENSION)
	@echo "EXECUTING TEST : $<"
	-./$< > $@ 2>&1
	@echo $(BREAK)

# Build test files
$(_PATH_EXECUTABLES)%.$(TARGET_EXTENSION): $(_PATH_OBJECTS)%.o $(PATH_OBJECTS)unity.o
	@echo "BUILDING TEST FILE : $@"
	$(CC) $(DEBUG_FLAGS) -o $@ $^ $(SO_FILE)
	@echo $(BREAK)

# Build object files
$(_PATH_OBJECTS)%.o: $(PATH_TEST)%.c
	@echo "BUILDING OBJECT FILE : $@"
	$(CC) -c $(DEBUG_FLAGS) $(_INCLUDE_FLAGS) $< -o $@
	@echo $(BREAK)

# Build unity library object files
$(PATH_OBJECTS)unity.o: $(PATH_UNITY)*.c $(PATH_UNITY)*.h
	@echo "BUILDING UNITY OBJECT FILE : $@"
	$(CC) -c $(DEBUG_FLAGS) -I$(PATH_UNITY) $(PATH_UNITY)*.c -o $@
	@echo $(BREAK)

.PRECIOUS: $(_PATH_OBJECTS)%.o
.PRECIOUS: $(_PATH_EXECUTABLES)%.$(TARGET_EXTENSION)
