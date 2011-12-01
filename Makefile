CXX		= g++
CXX_FLAGS	= -g -Wall -Wextra -pedantic -std=c++0x
FILT		= bin/gSTLFilt.pl
FILT_FLAGS	= -hdr:L -cand:L -banner:N

PACKRAT_FILES	= symbol parser ast
WRAPPER_FILES	= 
SRC_FILES	= main

SRC_DIR		= src
BUILD_DIR	= build
DEP_DIR		= $(BUILD_DIR)/deps
OBJ_DIR		= $(BUILD_DIR)/obj
TARGET_DIR	= $(BUILD_DIR)/bin
PACKRAT_DIR	= $(SRC_DIR)/Packrat
WRAPPER_DIR	= $(SRC_DIR)/Wrapper

NAMES		= $(PACKRAT_FILES) $(SRC_FILES)
FILES		= $(PACKRAT_FILES:%=$(PACKRAT_DIR)/%) \
			$(WRAPPER_FILES:%=$(WRAPPER_DIR)/%) \
			$(SRC_FILES:%=$(SRC_DIR)/%)

CPP_FILES	= $(FILES:%=%.cpp)
DEPENDS		= $(NAMES:%=$(DEP_DIR)/%.d)
OBJ_FILES	= $(NAMES:%=$(OBJ_DIR)/%.o)

TARGET		= $(TARGET_DIR)/main

LOOKUP		= $(filter %$(1).cpp, $(CPP_FILES))

all: $(TARGET)

clean:
	-rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir $@
$(DEP_DIR) $(OBJ_DIR) $(TARGET_DIR): | $(BUILD_DIR)
	mkdir $@

$(DEP_DIR)/%.d: | $(DEP_DIR)
	$(CXX) $(CXX_FLAGS) -MM -E $(call LOOKUP,$(*F)) -o $@ 2>&1 | \
	$(FILT) $(FILT_FLAGS) && \
	sed -i 's|\(.*\)[.]o|$$(OBJ_DIR)/& $$(DEP_DIR)/\1.d|' $@

$(TARGET): $(OBJ_FILES) | $(TARGET_DIR)
	$(CXX) $(CXX_FLAGS) -o $(TARGET) $(OBJ_FILES) 2>&1 | \
	$(FILT) $(FILT_FLAGS)

-include $(DEPENDS)
	
$(OBJ_DIR)/%.o: | $(OBJ_DIR)
	$(CXX) $(CXX_FLAGS) -c -o $@ $(call LOOKUP,$(*F))
