CXX		= g++
CXX_FLAGS	= -g -Wall -Wextra -pedantic -std=c++0x

PACKRAT_FILES	= match symbol parser
SRC_FILES	= main

SRC_DIR		= src
BUILD_DIR	= build
TARGET_DIR	= bin
PACKRAT_DIR	= $(SRC_DIR)/Packrat

NAMES		= $(PACKRAT_FILES) $(SRC_FILES)
FILES		= $(PACKRAT_FILES:%=$(PACKRAT_DIR)/%) \
			$(SRC_FILES:%=$(SRC_DIR)/%)

CPP_FILES	= $(FILES:%=%.cpp)
OBJ_FILES	= $(NAMES:%=$(BUILD_DIR)/%.o)
DEPENDS		= $(NAMES:%=$(BUILD_DIR)/%.d)

TARGET		= $(TARGET_DIR)/main

LOOKUP		= $(filter %$(1).cpp, $(CPP_FILES))

all: $(TARGET)

clean:
	-rm -rf $(BUILD_DIR) $(TARGET_DIR)

$(BUILD_DIR) $(TARGET_DIR):
	mkdir $@

$(BUILD_DIR)/%.d: | $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) -MM -E $(call LOOKUP,$(1)) | \
	sed 's|\(.*\)[.]o|$$(BUILD_DIR)/& $$(BUILD_DIR)/\1.d|' >$@	

$(TARGET): $(OBJ_FILES) | $(TARGET_DIR)
	$(CXX) $(CXX_FLAGS) -o $(TARGET) $(OBJ_FILES)

-include $(DEPENDS)
	
$(BUILD_DIR)/%.o: | $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) -c -o $@ $(call LOOKUP,$(*F))

