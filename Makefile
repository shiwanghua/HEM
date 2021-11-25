# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/169/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/169/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/swh/桌面/MyProgram/MatchingAlgorithmsComparison

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/swh/桌面/MyProgram/MatchingAlgorithmsComparison

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/snap/clion/169/bin/cmake/linux/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/snap/clion/169/bin/cmake/linux/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/swh/桌面/MyProgram/MatchingAlgorithmsComparison/CMakeFiles /home/swh/桌面/MyProgram/MatchingAlgorithmsComparison//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/swh/桌面/MyProgram/MatchingAlgorithmsComparison/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named run

# Build rule for target.
run: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 run
.PHONY : run

# fast build rule for target.
run/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/build
.PHONY : run/fast

AdaRein.o: AdaRein.cpp.o
.PHONY : AdaRein.o

# target to build an object file
AdaRein.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/AdaRein.cpp.o
.PHONY : AdaRein.cpp.o

AdaRein.i: AdaRein.cpp.i
.PHONY : AdaRein.i

# target to preprocess a source file
AdaRein.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/AdaRein.cpp.i
.PHONY : AdaRein.cpp.i

AdaRein.s: AdaRein.cpp.s
.PHONY : AdaRein.s

# target to generate assembly for a file
AdaRein.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/AdaRein.cpp.s
.PHONY : AdaRein.cpp.s

BGTree.o: BGTree.cpp.o
.PHONY : BGTree.o

# target to build an object file
BGTree.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BGTree.cpp.o
.PHONY : BGTree.cpp.o

BGTree.i: BGTree.cpp.i
.PHONY : BGTree.i

# target to preprocess a source file
BGTree.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BGTree.cpp.i
.PHONY : BGTree.cpp.i

BGTree.s: BGTree.cpp.s
.PHONY : BGTree.s

# target to generate assembly for a file
BGTree.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BGTree.cpp.s
.PHONY : BGTree.cpp.s

BIOP.o: BIOP.cpp.o
.PHONY : BIOP.o

# target to build an object file
BIOP.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP.cpp.o
.PHONY : BIOP.cpp.o

BIOP.i: BIOP.cpp.i
.PHONY : BIOP.i

# target to preprocess a source file
BIOP.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP.cpp.i
.PHONY : BIOP.cpp.i

BIOP.s: BIOP.cpp.s
.PHONY : BIOP.s

# target to generate assembly for a file
BIOP.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP.cpp.s
.PHONY : BIOP.cpp.s

BIOP1.o: BIOP1.cpp.o
.PHONY : BIOP1.o

# target to build an object file
BIOP1.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP1.cpp.o
.PHONY : BIOP1.cpp.o

BIOP1.i: BIOP1.cpp.i
.PHONY : BIOP1.i

# target to preprocess a source file
BIOP1.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP1.cpp.i
.PHONY : BIOP1.cpp.i

BIOP1.s: BIOP1.cpp.s
.PHONY : BIOP1.s

# target to generate assembly for a file
BIOP1.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP1.cpp.s
.PHONY : BIOP1.cpp.s

BIOP2.o: BIOP2.cpp.o
.PHONY : BIOP2.o

# target to build an object file
BIOP2.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP2.cpp.o
.PHONY : BIOP2.cpp.o

BIOP2.i: BIOP2.cpp.i
.PHONY : BIOP2.i

# target to preprocess a source file
BIOP2.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP2.cpp.i
.PHONY : BIOP2.cpp.i

BIOP2.s: BIOP2.cpp.s
.PHONY : BIOP2.s

# target to generate assembly for a file
BIOP2.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP2.cpp.s
.PHONY : BIOP2.cpp.s

BIOP3.o: BIOP3.cpp.o
.PHONY : BIOP3.o

# target to build an object file
BIOP3.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP3.cpp.o
.PHONY : BIOP3.cpp.o

BIOP3.i: BIOP3.cpp.i
.PHONY : BIOP3.i

# target to preprocess a source file
BIOP3.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP3.cpp.i
.PHONY : BIOP3.cpp.i

BIOP3.s: BIOP3.cpp.s
.PHONY : BIOP3.s

# target to generate assembly for a file
BIOP3.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP3.cpp.s
.PHONY : BIOP3.cpp.s

BIOP4.o: BIOP4.cpp.o
.PHONY : BIOP4.o

# target to build an object file
BIOP4.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP4.cpp.o
.PHONY : BIOP4.cpp.o

BIOP4.i: BIOP4.cpp.i
.PHONY : BIOP4.i

# target to preprocess a source file
BIOP4.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP4.cpp.i
.PHONY : BIOP4.cpp.i

BIOP4.s: BIOP4.cpp.s
.PHONY : BIOP4.s

# target to generate assembly for a file
BIOP4.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP4.cpp.s
.PHONY : BIOP4.cpp.s

BIOP5.o: BIOP5.cpp.o
.PHONY : BIOP5.o

# target to build an object file
BIOP5.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP5.cpp.o
.PHONY : BIOP5.cpp.o

BIOP5.i: BIOP5.cpp.i
.PHONY : BIOP5.i

# target to preprocess a source file
BIOP5.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP5.cpp.i
.PHONY : BIOP5.cpp.i

BIOP5.s: BIOP5.cpp.s
.PHONY : BIOP5.s

# target to generate assembly for a file
BIOP5.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOP5.cpp.s
.PHONY : BIOP5.cpp.s

BIOPSC.o: BIOPSC.cpp.o
.PHONY : BIOPSC.o

# target to build an object file
BIOPSC.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOPSC.cpp.o
.PHONY : BIOPSC.cpp.o

BIOPSC.i: BIOPSC.cpp.i
.PHONY : BIOPSC.i

# target to preprocess a source file
BIOPSC.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOPSC.cpp.i
.PHONY : BIOPSC.cpp.i

BIOPSC.s: BIOPSC.cpp.s
.PHONY : BIOPSC.s

# target to generate assembly for a file
BIOPSC.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOPSC.cpp.s
.PHONY : BIOPSC.cpp.s

BIOPSR.o: BIOPSR.cpp.o
.PHONY : BIOPSR.o

# target to build an object file
BIOPSR.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOPSR.cpp.o
.PHONY : BIOPSR.cpp.o

BIOPSR.i: BIOPSR.cpp.i
.PHONY : BIOPSR.i

# target to preprocess a source file
BIOPSR.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOPSR.cpp.i
.PHONY : BIOPSR.cpp.i

BIOPSR.s: BIOPSR.cpp.s
.PHONY : BIOPSR.s

# target to generate assembly for a file
BIOPSR.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/BIOPSR.cpp.s
.PHONY : BIOPSR.cpp.s

OpIndex.o: OpIndex.cpp.o
.PHONY : OpIndex.o

# target to build an object file
OpIndex.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/OpIndex.cpp.o
.PHONY : OpIndex.cpp.o

OpIndex.i: OpIndex.cpp.i
.PHONY : OpIndex.i

# target to preprocess a source file
OpIndex.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/OpIndex.cpp.i
.PHONY : OpIndex.cpp.i

OpIndex.s: OpIndex.cpp.s
.PHONY : OpIndex.s

# target to generate assembly for a file
OpIndex.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/OpIndex.cpp.s
.PHONY : OpIndex.cpp.s

Rein.o: Rein.cpp.o
.PHONY : Rein.o

# target to build an object file
Rein.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/Rein.cpp.o
.PHONY : Rein.cpp.o

Rein.i: Rein.cpp.i
.PHONY : Rein.i

# target to preprocess a source file
Rein.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/Rein.cpp.i
.PHONY : Rein.cpp.i

Rein.s: Rein.cpp.s
.PHONY : Rein.s

# target to generate assembly for a file
Rein.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/Rein.cpp.s
.PHONY : Rein.cpp.s

Simple.o: Simple.cpp.o
.PHONY : Simple.o

# target to build an object file
Simple.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/Simple.cpp.o
.PHONY : Simple.cpp.o

Simple.i: Simple.cpp.i
.PHONY : Simple.i

# target to preprocess a source file
Simple.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/Simple.cpp.i
.PHONY : Simple.cpp.i

Simple.s: Simple.cpp.s
.PHONY : Simple.s

# target to generate assembly for a file
Simple.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/Simple.cpp.s
.PHONY : Simple.cpp.s

Simple2.o: Simple2.cpp.o
.PHONY : Simple2.o

# target to build an object file
Simple2.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/Simple2.cpp.o
.PHONY : Simple2.cpp.o

Simple2.i: Simple2.cpp.i
.PHONY : Simple2.i

# target to preprocess a source file
Simple2.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/Simple2.cpp.i
.PHONY : Simple2.cpp.i

Simple2.s: Simple2.cpp.s
.PHONY : Simple2.s

# target to generate assembly for a file
Simple2.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/Simple2.cpp.s
.PHONY : Simple2.cpp.s

Tama.o: Tama.cpp.o
.PHONY : Tama.o

# target to build an object file
Tama.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/Tama.cpp.o
.PHONY : Tama.cpp.o

Tama.i: Tama.cpp.i
.PHONY : Tama.i

# target to preprocess a source file
Tama.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/Tama.cpp.i
.PHONY : Tama.cpp.i

Tama.s: Tama.cpp.s
.PHONY : Tama.s

# target to generate assembly for a file
Tama.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/Tama.cpp.s
.PHONY : Tama.cpp.s

constant.o: constant.cpp.o
.PHONY : constant.o

# target to build an object file
constant.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/constant.cpp.o
.PHONY : constant.cpp.o

constant.i: constant.cpp.i
.PHONY : constant.i

# target to preprocess a source file
constant.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/constant.cpp.i
.PHONY : constant.cpp.i

constant.s: constant.cpp.s
.PHONY : constant.s

# target to generate assembly for a file
constant.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/constant.cpp.s
.PHONY : constant.cpp.s

generator.o: generator.cpp.o
.PHONY : generator.o

# target to build an object file
generator.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/generator.cpp.o
.PHONY : generator.cpp.o

generator.i: generator.cpp.i
.PHONY : generator.i

# target to preprocess a source file
generator.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/generator.cpp.i
.PHONY : generator.cpp.i

generator.s: generator.cpp.s
.PHONY : generator.s

# target to generate assembly for a file
generator.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/generator.cpp.s
.PHONY : generator.cpp.s

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/main.cpp.s
.PHONY : main.cpp.s

pRein.o: pRein.cpp.o
.PHONY : pRein.o

# target to build an object file
pRein.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/pRein.cpp.o
.PHONY : pRein.cpp.o

pRein.i: pRein.cpp.i
.PHONY : pRein.i

# target to preprocess a source file
pRein.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/pRein.cpp.i
.PHONY : pRein.cpp.i

pRein.s: pRein.cpp.s
.PHONY : pRein.s

# target to generate assembly for a file
pRein.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/pRein.cpp.s
.PHONY : pRein.cpp.s

printer.o: printer.cpp.o
.PHONY : printer.o

# target to build an object file
printer.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/printer.cpp.o
.PHONY : printer.cpp.o

printer.i: printer.cpp.i
.PHONY : printer.i

# target to preprocess a source file
printer.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/printer.cpp.i
.PHONY : printer.cpp.i

printer.s: printer.cpp.s
.PHONY : printer.s

# target to generate assembly for a file
printer.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/printer.cpp.s
.PHONY : printer.cpp.s

run_models.o: run_models.cpp.o
.PHONY : run_models.o

# target to build an object file
run_models.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/run_models.cpp.o
.PHONY : run_models.cpp.o

run_models.i: run_models.cpp.i
.PHONY : run_models.i

# target to preprocess a source file
run_models.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/run_models.cpp.i
.PHONY : run_models.cpp.i

run_models.s: run_models.cpp.s
.PHONY : run_models.s

# target to generate assembly for a file
run_models.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/run_models.cpp.s
.PHONY : run_models.cpp.s

test.o: test.cpp.o
.PHONY : test.o

# target to build an object file
test.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/test.cpp.o
.PHONY : test.cpp.o

test.i: test.cpp.i
.PHONY : test.i

# target to preprocess a source file
test.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/test.cpp.i
.PHONY : test.cpp.i

test.s: test.cpp.s
.PHONY : test.s

# target to generate assembly for a file
test.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/test.cpp.s
.PHONY : test.cpp.s

util.o: util.cpp.o
.PHONY : util.o

# target to build an object file
util.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/util.cpp.o
.PHONY : util.cpp.o

util.i: util.cpp.i
.PHONY : util.i

# target to preprocess a source file
util.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/util.cpp.i
.PHONY : util.cpp.i

util.s: util.cpp.s
.PHONY : util.s

# target to generate assembly for a file
util.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/util.cpp.s
.PHONY : util.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... run"
	@echo "... AdaRein.o"
	@echo "... AdaRein.i"
	@echo "... AdaRein.s"
	@echo "... BGTree.o"
	@echo "... BGTree.i"
	@echo "... BGTree.s"
	@echo "... BIOP.o"
	@echo "... BIOP.i"
	@echo "... BIOP.s"
	@echo "... BIOP1.o"
	@echo "... BIOP1.i"
	@echo "... BIOP1.s"
	@echo "... BIOP2.o"
	@echo "... BIOP2.i"
	@echo "... BIOP2.s"
	@echo "... BIOP3.o"
	@echo "... BIOP3.i"
	@echo "... BIOP3.s"
	@echo "... BIOP4.o"
	@echo "... BIOP4.i"
	@echo "... BIOP4.s"
	@echo "... BIOP5.o"
	@echo "... BIOP5.i"
	@echo "... BIOP5.s"
	@echo "... BIOPSC.o"
	@echo "... BIOPSC.i"
	@echo "... BIOPSC.s"
	@echo "... BIOPSR.o"
	@echo "... BIOPSR.i"
	@echo "... BIOPSR.s"
	@echo "... OpIndex.o"
	@echo "... OpIndex.i"
	@echo "... OpIndex.s"
	@echo "... Rein.o"
	@echo "... Rein.i"
	@echo "... Rein.s"
	@echo "... Simple.o"
	@echo "... Simple.i"
	@echo "... Simple.s"
	@echo "... Simple2.o"
	@echo "... Simple2.i"
	@echo "... Simple2.s"
	@echo "... Tama.o"
	@echo "... Tama.i"
	@echo "... Tama.s"
	@echo "... constant.o"
	@echo "... constant.i"
	@echo "... constant.s"
	@echo "... generator.o"
	@echo "... generator.i"
	@echo "... generator.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... pRein.o"
	@echo "... pRein.i"
	@echo "... pRein.s"
	@echo "... printer.o"
	@echo "... printer.i"
	@echo "... printer.s"
	@echo "... run_models.o"
	@echo "... run_models.i"
	@echo "... run_models.s"
	@echo "... test.o"
	@echo "... test.i"
	@echo "... test.s"
	@echo "... util.o"
	@echo "... util.i"
	@echo "... util.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

