CXX		= g++
CXX_FLAGS	= -g -Wall -Wextra -pedantic -std=c++0x
TEX		= pdflatex
TEX_FLAGS	= -interaction nonstopmode
FILT		= bin/gSTLFilt.pl
FILT_FLAGS	= -hdr:L -cand:L -banner:N

DOC_FILES	= PStandard Thoughts PackratParser PRules
PACKRAT_FILES	= symbol parser ast
WRAPPER_FILES	= 
SRC_FILES	= main

SRC_DIR		= src
TEX_DIR		= tex
BUILD_DIR	= build
DEP_DIR		= $(BUILD_DIR)/deps
OBJ_DIR		= $(BUILD_DIR)/obj
DOC_DIR		= $(BUILD_DIR)/doc
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
PROGRAM		= $(TARGET_DIR)/pc

DOC_TARGETS	= $(DOC_FILES:%=$(DOC_DIR)/%.pdf)
TARGETS		= documents compiler

LOOKUP		= $(filter %$(1).cpp, $(CPP_FILES))

all: $(TARGETS)

documents: $(DOC_TARGETS)

compiler: $(PROGRAM)

clean:
	-rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir $@
$(DEP_DIR) $(OBJ_DIR) $(TARGET_DIR) $(DOC_DIR): | $(BUILD_DIR)
	mkdir $@

$(DEP_DIR)/%.d: | $(DEP_DIR)
	$(CXX) $(CXX_FLAGS) -MM -E $(call LOOKUP,$(*F)) -o $@ 2>&1 | \
	$(FILT) $(FILT_FLAGS) && \
	sed -i 's|\(.*\)[.]o|$$(OBJ_DIR)/& $$(DEP_DIR)/\1.d|' $@

$(PROGRAM): $(OBJ_FILES) | $(TARGET_DIR)
	bin/linecheck -Ro --fix-endings --fix-last-line src && \
	bin/cppcheck -v --enable=all --std=c99 --std=c++11 src && \
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJ_FILES) 2>&1 | \
	$(FILT) $(FILT_FLAGS)

-include $(DEPENDS)

$(DOC_DIR)/%.pdf: $(DOC_DIR)
	$(TEX) -draftmode $(TEX_FLAGS) $(TEX_DIR)/$(*F).tex >/dev/null 2>/dev/null; \
	$(TEX) $(TEX_FLAGS) $(TEX_DIR)/$(*F).tex && \
	mv $(*F).pdf $@ && \
	rm $(*F).{log,aux}

$(OBJ_DIR)/%.o: | $(OBJ_DIR)
	$(CXX) $(CXX_FLAGS) -c -o $@ $(call LOOKUP,$(*F))

# giving up and LaTeX has no mahic
$(DOC_DIR)/PackratParser.tex : $(TEX_DIR)/PackratParser.tex $(TEX_DIR)/PackratParser/usingAST.tex
$(DOC_DIR)/PStandard.tex : $(TEX_DIR)/PStandard.tex $(TEX_DIR)/PStandard/operation.tex $(TEX_DIR)/PStandard/scope.tex $(TEX_DIR)/PStandard/terms.tex
$(DOC_DIR)/Thoughts.tex : $(TEX_DIR)/Thoughts.tex $(TEX_DIR)/Thoughts/introduction.tex $(TEX_DIR)/Thoughts/optimizations.tex $(TEX_DIR)/Thoughts/semantics.tex
