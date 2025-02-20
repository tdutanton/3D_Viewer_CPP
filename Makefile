SHELL = /bin/sh

CC = gcc -std=c++17
CFLAGS = -Wall -Werror -Wextra -g 
LFLAGS = -lpthread -lgtest -pthread -lstdc++ -lm
GFLAGS = -fprofile-arcs -ftest-coverage

EXE = 3D_Viewer
LIB_NAME = s21_viewer.a

TEST_FILE_NAME = test_viewer

LIB_SRC = $(wildcard model/**/*.cpp)
TEST_SRC = $(wildcard tests/*.cpp)

LIB_O = $(LIB_SRC:.cpp=.o)
TEST_O = $(TEST_SRC:.cpp=.o)

GCOV_NAME = gcov_tests.info

DOCS_DIR = documentation
DIST_DIR = 3D_Viewer
DESTDIR = bin

REPORT_DIR = report

UML_LIB = hpp2plantuml
PLANTUML_LIB = plantuml
PYTHON_LIB = python3-pip
XETEX_LIB = texlive-xetex
CYRILLIC_LIB = texlive-lang-cyrillic
DOXYGEN_LIB = doxygen

CHECK_UML_UTIL = $(shell which $(UML_LIB))
CHECK_PIP3_LIB = $(shell which pip3)
CHECK_PLANTUML_LIB = $(shell which $(PLANTUML_LIB))
CHECK_XETEX_LIB = $(shell dpkg -l | grep $(XETEX_LIB))
CHECK_CYRILLIC_LIB = $(shell dpkg -l | grep $(CYRILLIC_LIB))
CHECK_DOXYGEN_LIB = $(shell which $(DOXYGEN_LIB))

INSTALL_UML_LIB = pip install $(UML_LIB)
INSTALL_PLANTUML_LIB = sudo apt install -y $(PLANTUML_LIB)
INSTALL_PYTHON_LIB = sudo apt install -y $(PYTHON_LIB)
INSTALL_XETEX_LIB = sudo apt install -y $(XETEX_LIB)
INSTALL_CYRILLIC_LIB = sudo apt install -y $(CYRILLIC_LIB)
INSTALL_DOXYGEN_LIB = sudo apt install -y $(DOXYGEN_LIB)

UML_FILE_NAME = viewer_uml.puml
UML_PNG_FILE_NAME = viewer_uml.png

#################################################################################
#  MAIN TARGETS - ALL INSTALL UNINSTALL CLEAN DVI DIST TESTS GCOV_REPORT CHECK  #
#################################################################################

.PHONY: all
all:
	@echo "BUILDING 3D_VIEWER"
	@make -s build
	@echo "BUILDING COMPLETED. BINARY EXECUTABLE FILE - $(EXE). ENTER 'MAKE RUN' TO START THE PROGRAM"

.PHONY: build
build:
	@cd gui && qmake && make -s clean && make -s
	@mv gui/$(EXE) .
	@cd gui && make -s clean

# Type 'make install DESTDIR=[folder name]' or just 'make install' to install to the default folder 'bin'
.PHONY: install
install:
	@echo "TYPE THE FOLDER NAME OR JUST PUSH ENTER TO INSTALL IN DEFAULT FOLDER - ($(DESTDIR)):"
	@read folder; \
	if [ -z "$$folder" ]; then \
		folder=$(DESTDIR); \
	fi;
	@echo "INSTALLING INTO $(DESTDIR)"
	@make -s build
	@install -d $(DESTDIR)
	@install -m 755 $(EXE) $(DESTDIR)/$(EXE)
	@echo "INSTALLED 3D_VIEWER IN $(DESTDIR) FOLDER"

.PHONY: uninstall
uninstall:
	@rm -rf $(DESTDIR)/$(EXE)

.PHONY: run
run:
	@chmod 0700 /run/user/1000/
	@./3D_Viewer

.PHONY: s21_viewer.a
s21_viewer.a: $(LIB_O)
	@ar rcs $(LIB_NAME) $(LIB_O)
	@ranlib $(LIB_NAME)
	@rm -f $(LIB_O)

.PHONY: tests
tests: $(TEST_O) $(LIB_NAME)
	@echo "Start testing"
	@rm -f $(TEST_FILE_NAME)
	@$(CC) $(CFLAGS) $(TEST_O) -DTEST -o $(TEST_FILE_NAME) -L. -l:$(LIB_NAME) $(LFLAGS)
	@./$(TEST_FILE_NAME) || exit 1

.PHONY: check
check: tests

.PHONY: gcov_report
gcov_report:
	@echo "Start making gcov report"
	@$(CC) $(CFLAGS) -c $(LIB_SRC) --coverage
	@$(CC) $(CFLAGS) -c $(TEST_SRC)
	@$(CC) *.o -o $(TEST_FILE_NAME) $(LFLAGS) $(GFLAGS)
	@./$(TEST_FILE_NAME)
	@lcov -t "gcov_tests" -o $(GCOV_NAME) -c -d . \
		--exclude '/usr/include/c++/11/*' \
		--exclude '/usr/include/c++/11/bits/*' \
		--exclude '/usr/include/c++/11/ext/*' \
		--exclude '/usr/include/gtest/*' \
		--exclude '/usr/include/gtest/internal/*' \
		--exclude 'tests/*'
	@genhtml -o report $(GCOV_NAME)
	@rm -f *.gcno *.gcda $(GCOV_NAME) *.o
	@rm -f $(TEST_FILE_NAME)

.PHONY: clean
clean:
	@rm -f $(TEST_FILE_NAME) *.o
	@rm -rf $(LIB_O) ./tests/*.o *.gcno *.gcda ./report
	@rm -rf $(TEST_FILE_NAME)
	@rm -rf $(GCOV_NAME)
	@rm -rf $(LIB_NAME)
	@rm -rf *.out
	@rm -rf $(UML_FILE_NAME)
	@rm -rf out.png
	@rm -rf $(UML_PNG_FILE_NAME)
	@rm -rf $(DOCS_DIR)
	@rm -rf $(DESTDIR)/$(EXE)
	@make -s clean_empty_dir
	@rm -rf $(REPORT_DIR)
	@rm -rf $(EXE)

dvi: doxygen_check_lib xetex_check_lib cyrillic_check_lib
	@mkdir -p $(DOCS_DIR)
	@doxygen Doxyfile > /dev/null 2>&1
	@cd $(DOCS_DIR)/latex && xelatex -interaction=batchmode refman.tex > /dev/null 2>&1
	@mv $(DOCS_DIR)/html/index.html $(DOCS_DIR)/html/_documentation.html
	@mv $(DOCS_DIR)/latex/refman.pdf $(DOCS_DIR)/latex/_documentation.pdf
	@if [ $$? -eq 0 ]; then echo "DVI make success.\n\
	Open $(DOCS_DIR)/latex/$(DOCS_NAME).pdf for PDF version.\n\
	Open $(DOCS_DIR)/html/$(DOCS_NAME).html for HTML version."; else echo "DVI ERROR"; fi
.PHONY: dvi

.PHONY: dist
dist: build
	@mkdir -p $(DIST_DIR)/
	@cp $(EXE) $(DIST_DIR)/$(EXE)
	tar cvzf 3DViewer.tgz $(DIST_DIR)/
	@rm -rf $(DIST_DIR)/

######################
# ADDITIONAL TARGETS #
######################

.PHONY: clean_empty_dir
clean_empty_dir:
	@if [ -d "$(DESTDIR)" ] && [ -z "$$(find $(DESTDIR) -mindepth 1 -print -quit)" ]; then \
		rmdir $(DESTDIR); \
	fi

.PHONY: style_check
style_check:
	@find . -name '*.cpp' -o -name '*.h' | xargs clang-format -Werror -n

.PHONY: style_fix
style_fix:
	find . -name '*.c' -exec clang-format -i {} \;
	find . -name '*.cpp' -exec clang-format -i {} \;
	find . -name '*.h' -exec clang-format -i {} \;

.PHONY: rebuild
rebuild: clean build

.PHONY: valgrind_test
valgrind_test: tests
	valgrind --tool=memcheck --leak-check=yes --show-leak-kinds=all --track-origins=yes -s ./$(TEST_FILE_NAME)

%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

# Create UML diagram
.PHONY: uml_diagram
uml_diagram: pip3_check_lib uml_check_lib plantuml_check_lib
	@$(UML_LIB) -i "model/**/*.h" -i "gui/*.h" -o $(UML_FILE_NAME) -d
	@plantuml $(UML_FILE_NAME)
	@echo "UML diagram saved to $(UML_PNG_FILE_NAME), puml file saved to $(UML_FILE_NAME)"

# If You have to make report with gcovr instead of lcov - try Make gcov_report_gcovr
.PHONY: gcov_report_gcovr 
gcov_report_gcovr: clean
	@echo "Start making gcov report"
	@mkdir -p $(REPORT_DIR)
	@$(CC) $(CFLAGS) -c $(LIB_SRC) --coverage
	@$(CC) $(CFLAGS) -c $(TEST_SRC)
	@$(CC) *.o -o $(TEST_FILE_NAME) $(LFLAGS) $(GFLAGS)
	@./$(TEST_FILE_NAME)
	@gcovr -r . --html --html-details -o $(REPORT_DIR)/report.html \
		--exclude-directories='/usr/include/c\+\+/11' \
		--exclude-directories='/usr/include/gtest' \
		--exclude-directories='tests'
	@rm -f *.gcno *.gcda *.o
	@rm -f $(TEST_FILE_NAME)

##################################################
# CHECKING IF NEEDED LIBS INSTALLED AND INSTALL  #
##################################################

.PHONY: uml_check_lib
uml_check_lib:
	@if [ -z "$(CHECK_UML_UTIL)" ]; then \
		echo "Installing $(UML_LIB)"; \
		$(INSTALL_UML_LIB); \
	fi

.PHONY: pip3_check_lib
pip3_check_lib:
	@if [ -z "$(CHECK_PIP3_LIB)" ]; then \
		echo "Installing $(PYTHON_LIB)"; \
		$(INSTALL_PYTHON_LIB); \
	fi

.PHONY: plantuml_check_lib
plantuml_check_lib:
	@if [ -z "$(CHECK_PLANTUML_LIB)" ]; then \
		echo "Installing $(PLANTUML_LIB)"; \
		$(INSTALL_PLANTUML_LIB); \
	fi

.PHONY: doxygen_check_lib
doxygen_check_lib:
	@if [ -z "$(CHECK_DOXYGEN_LIB)" ]; then \
		echo "Installing $(DOXYGEN_LIB)"; \
		$(INSTALL_DOXYGEN_LIB); \
	fi

.PHONY: xetex_check_lib
xetex_check_lib:
	@if [ -z "$(CHECK_XETEX_LIB)" ]; then \
		echo "Installing $(XETEX_LIB)"; \
		$(INSTALL_XETEX_LIB); \
	fi

.PHONY: cyrillic_check_lib
cyrillic_check_lib:
	@if [ -z "$(CHECK_CYRILLIC_LIB)" ]; then \
		echo "Installing $(CYRILLIC_LIB)"; \
		$(INSTALL_CYRILLIC_LIB); \
	fi
