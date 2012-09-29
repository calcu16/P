# Copyright (c) 2012, Andrew Carter, Dietrich Langenbach, Xanda Schofield
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are 
# met: 
#
# 1. Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer. 
# 2. Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution. 
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# The views and conclusions contained in the software and documentation are those
# of the authors and should not be interpreted as representing official policies, 
# either expressed or implied, of the FreeBSD Project.

CXX		= g++
CXX_FLAGS	= -g -Werror -Wall -Wextra -pedantic -std=c++0x
TEX		= pdflatex
TEX_FLAGS	= -interaction nonstopmode
FILT		= bin/gSTLFilt.pl
FILT_FLAGS	= -hdr:L -cand:L -banner:N

DOC_FILES	= PStandard Thoughts PackratParser PRules
PACKRAT_FILES	= symbol parser ast pst build_tree
PRINTING_FILES	= pprinter
WRAPPER_FILES	= 
SRC_FILES	= main

SRC_DIR		= src
TEX_DIR		= tex
BUILD_DIR	= build
DEP_DIR		= $(BUILD_DIR)/deps
OBJ_DIR		= $(BUILD_DIR)/obj
LIB_DIR		= $(BUILD_DIR)/lib
DOC_DIR		= $(BUILD_DIR)/doc
TARGET_DIR	= $(BUILD_DIR)/bin
PACKRAT_DIR	= $(SRC_DIR)/Packrat
PRINTING_DIR	= $(SRC_DIR)/Printing
WRAPPER_DIR	= $(SRC_DIR)/Wrapper

NAMES		= $(PACKRAT_FILES) $(SRC_FILES) $(PRINTING_FILES)
FILES		= $(PACKRAT_FILES:%=$(PACKRAT_DIR)/%) \
			$(WRAPPER_FILES:%=$(WRAPPER_DIR)/%) \
			$(PRINTING_FILES:%=$(PRINTING_DIR)/%) \
			$(SRC_FILES:%=$(SRC_DIR)/%)

CPP_FILES	= $(FILES:%=%.cpp)
DEPENDS		= $(NAMES:%=$(DEP_DIR)/%.d)
OBJ_FILES	= $(NAMES:%=$(OBJ_DIR)/%.o)
PROGRAM		= $(TARGET_DIR)/pcc

DOC_TARGETS	= $(DOC_FILES:%=$(DOC_DIR)/%.pdf)
TARGETS		= documents compiler test

LOOKUP		= $(filter %$(1).cpp, $(CPP_FILES))

all: $(TARGETS)

documents: $(DOC_TARGETS)

compiler: $(PROGRAM)

test: | $(LIB_DIR)

clean:
	-rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir $@
$(DEP_DIR) $(OBJ_DIR) $(TARGET_DIR) $(DOC_DIR) $(LIB_DIR): | $(BUILD_DIR)
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

$(DOC_DIR)/%.pdf: | $(DOC_DIR)
	$(TEX) -draftmode $(TEX_FLAGS) $(TEX_DIR)/$(*F).tex >/dev/null 2>/dev/null; \
	$(TEX) $(TEX_FLAGS) $(TEX_DIR)/$(*F).tex && \
	mv $(*F).pdf $@ && \
	rm $(*F).{log,aux}

$(OBJ_DIR)/%.o: | $(OBJ_DIR)
	$(CXX) $(CXX_FLAGS) -c -o $@ $(call LOOKUP,$(*F))

# giving up and LaTeX has no magic
$(DOC_DIR)/PackratParser.pdf : $(TEX_DIR)/PackratParser.tex $(TEX_DIR)/PackratParser/usingAST.tex
$(DOC_DIR)/PStandard.pdf : $(TEX_DIR)/PStandard.tex $(TEX_DIR)/PStandard/operation.tex $(TEX_DIR)/PStandard/scope.tex $(TEX_DIR)/PStandard/terms.tex
$(DOC_DIR)/Thoughts.pdf : $(TEX_DIR)/Thoughts.tex $(TEX_DIR)/Thoughts/introduction.tex $(TEX_DIR)/Thoughts/optimizations.tex $(TEX_DIR)/Thoughts/semantics.tex
