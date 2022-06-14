# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nhed/delilah-eval

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nhed/delilah-eval

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/nhed/delilah-eval/CMakeFiles /home/nhed/delilah-eval/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/nhed/delilah-eval/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named delilah-eval

# Build rule for target.
delilah-eval: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 delilah-eval
.PHONY : delilah-eval

# fast build rule for target.
delilah-eval/fast:
	$(MAKE) -f CMakeFiles/delilah-eval.dir/build.make CMakeFiles/delilah-eval.dir/build
.PHONY : delilah-eval/fast

#=============================================================================
# Target rules for targets named program_version

# Build rule for target.
program_version: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 program_version
.PHONY : program_version

# fast build rule for target.
program_version/fast:
	$(MAKE) -f programs/CMakeFiles/program_version.dir/build.make programs/CMakeFiles/program_version.dir/build
.PHONY : program_version/fast

#=============================================================================
# Target rules for targets named program_file

# Build rule for target.
program_file: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 program_file
.PHONY : program_file

# fast build rule for target.
program_file/fast:
	$(MAKE) -f programs/CMakeFiles/program_file.dir/build.make programs/CMakeFiles/program_file.dir/build
.PHONY : program_file/fast

#=============================================================================
# Target rules for targets named programs

# Build rule for target.
programs: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 programs
.PHONY : programs

# fast build rule for target.
programs/fast:
	$(MAKE) -f programs/CMakeFiles/programs.dir/build.make programs/CMakeFiles/programs.dir/build
.PHONY : programs/fast

delilah.o: delilah.c.o

.PHONY : delilah.o

# target to build an object file
delilah.c.o:
	$(MAKE) -f CMakeFiles/delilah-eval.dir/build.make CMakeFiles/delilah-eval.dir/delilah.c.o
.PHONY : delilah.c.o

delilah.i: delilah.c.i

.PHONY : delilah.i

# target to preprocess a source file
delilah.c.i:
	$(MAKE) -f CMakeFiles/delilah-eval.dir/build.make CMakeFiles/delilah-eval.dir/delilah.c.i
.PHONY : delilah.c.i

delilah.s: delilah.c.s

.PHONY : delilah.s

# target to generate assembly for a file
delilah.c.s:
	$(MAKE) -f CMakeFiles/delilah-eval.dir/build.make CMakeFiles/delilah-eval.dir/delilah.c.s
.PHONY : delilah.c.s

hermes.o: hermes.c.o

.PHONY : hermes.o

# target to build an object file
hermes.c.o:
	$(MAKE) -f CMakeFiles/delilah-eval.dir/build.make CMakeFiles/delilah-eval.dir/hermes.c.o
.PHONY : hermes.c.o

hermes.i: hermes.c.i

.PHONY : hermes.i

# target to preprocess a source file
hermes.c.i:
	$(MAKE) -f CMakeFiles/delilah-eval.dir/build.make CMakeFiles/delilah-eval.dir/hermes.c.i
.PHONY : hermes.c.i

hermes.s: hermes.c.s

.PHONY : hermes.s

# target to generate assembly for a file
hermes.c.s:
	$(MAKE) -f CMakeFiles/delilah-eval.dir/build.make CMakeFiles/delilah-eval.dir/hermes.c.s
.PHONY : hermes.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... delilah-eval"
	@echo "... program_version"
	@echo "... program_file"
	@echo "... programs"
	@echo "... delilah.o"
	@echo "... delilah.i"
	@echo "... delilah.s"
	@echo "... hermes.o"
	@echo "... hermes.i"
	@echo "... hermes.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

