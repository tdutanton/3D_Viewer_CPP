SHELL = /bin/sh

CC = gcc -std=c++17
CFLAGS = -Wall -Werror -Wextra -g 
LFLAGS = -lpthread -lgtest -pthread -lstdc++ -lm
GFLAGS = -fprofile-arcs -ftest-coverage

LIB_NAME = s21_viewer.a

TEST_FILE_NAME = test_viewer

LIB_SRC = $(wildcard model/**/*.cpp)
TEST_SRC = $(wildcard tests/*.cpp)

LIB_O = $(LIB_SRC:.cpp=.o)
TEST_O = $(TEST_SRC:.cpp=.o)

GCOV_NAME = gcov_tests.info

DOCS_FOLDER = documentation
DIST_FOLDER = 3D_Viewer

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

.PHONY: all
all: clean tests

.PHONY: install
install:
	@mkdir -p build/
	@cd gui && qmake && make -s clean && make -s
	@mv gui/3D_Viewer build/3D_Viewer
	@cd gui && make -s clean

.PHONY: uninstall
uninstall:
	@rm -rf build

.PHONY: run
run:
	@cd build && ./3D_Viewer

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

.PHONY: gcov_report
gcov_report: clean
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

.PHONY: clean
clean:
	@rm -f $(TEST_FILE_NAME) *.o .clang-format
	@rm -rf $(LIB_O) ./tests/*.o *.gcno *.gcda ./report
	@rm -rf $(TEST_FILE_NAME)
	@rm -rf $(GCOV_NAME)
	@rm -rf $(LIB_NAME)
	@rm -rf *.out
	@rm -rf $(UML_FILE_NAME)
	@rm -rf out.png
	@rm -rf $(UML_PNG_FILE_NAME)
	@rm -rf $(DOCS_FOLDER)
	@rm -rf build
	@rm -rf $(REPORT_DIR)

.PHONY: style_check
style_check:
	@cp ../materials/linters/.clang-format .clang-format
	@find . -name '*.cpp' -o -name '*.h' | xargs clang-format -Werror -n
	@rm -rf .clang-format

.PHONY: style_fix
style_fix:
	@cp ../materials/linters/.clang-format .clang-format
	find . -name '*.c' -exec clang-format -i {} \;
	find . -name '*.cpp' -exec clang-format -i {} \;
	find . -name '*.h' -exec clang-format -i {} \;
	@rm -rf .clang-format

.PHONY: rebuild
rebuild: clean tests

.PHONY: valgrind_test
valgrind_test: rebuild
	valgrind --tool=memcheck --leak-check=yes --show-leak-kinds=all --track-origins=yes -s ./$(TEST_FILE_NAME)

%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: uml_diagram
uml_diagram: pip3_check_lib uml_check_lib plantuml_check_lib
	@$(UML_LIB) -i "model/**/*.h" -i "gui/*.h" -o $(UML_FILE_NAME) -d
	@plantuml $(UML_FILE_NAME)
	@echo "UML diagram saved to $(UML_PNG_FILE_NAME), puml file saved to $(UML_FILE_NAME)"

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

.PHONY: dvi
dvi: clean doxygen_check_lib xetex_check_lib cyrillic_check_lib
	@mkdir -p $(DOCS_FOLDER)
	@doxygen Doxyfile > /dev/null 2>&1
	@cd $(DOCS_FOLDER)/latex && xelatex -interaction=batchmode refman.tex > /dev/null 2>&1
	@if [ $$? -eq 0 ]; then echo "DVI make success. See $(DOCS_FOLDER)/latex/refman.pdf for pdf and $(DOCS_FOLDER)/html/index.html for html"; else echo "DVI ERROR"; fi

.PHONY: dist
dist: uninstall install
	@mkdir -p $(DIST_FOLDER)/
	@cp build/3D_Viewer $(DIST_FOLDER)/3D_Viewer
	tar cvzf 3DViewer.tgz $(DIST_FOLDER)/
	@rm -rf $(DIST_FOLDER)/

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
