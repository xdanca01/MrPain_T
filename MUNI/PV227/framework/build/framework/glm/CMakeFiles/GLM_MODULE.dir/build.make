# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pdancak/school/MrPain_T/MUNI/PV227/framework

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pdancak/school/MrPain_T/MUNI/PV227/framework/build

# Include any dependencies generated for this target.
include framework/glm/CMakeFiles/GLM_MODULE.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include framework/glm/CMakeFiles/GLM_MODULE.dir/compiler_depend.make

# Include the progress variables for this target.
include framework/glm/CMakeFiles/GLM_MODULE.dir/progress.make

# Include the compile flags for this target's objects.
include framework/glm/CMakeFiles/GLM_MODULE.dir/flags.make

framework/glm/CMakeFiles/GLM_MODULE.dir/src/empty.cpp.o: framework/glm/CMakeFiles/GLM_MODULE.dir/flags.make
framework/glm/CMakeFiles/GLM_MODULE.dir/src/empty.cpp.o: ../framework/glm/src/empty.cpp
framework/glm/CMakeFiles/GLM_MODULE.dir/src/empty.cpp.o: framework/glm/CMakeFiles/GLM_MODULE.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pdancak/school/MrPain_T/MUNI/PV227/framework/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object framework/glm/CMakeFiles/GLM_MODULE.dir/src/empty.cpp.o"
	cd /home/pdancak/school/MrPain_T/MUNI/PV227/framework/build/framework/glm && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT framework/glm/CMakeFiles/GLM_MODULE.dir/src/empty.cpp.o -MF CMakeFiles/GLM_MODULE.dir/src/empty.cpp.o.d -o CMakeFiles/GLM_MODULE.dir/src/empty.cpp.o -c /home/pdancak/school/MrPain_T/MUNI/PV227/framework/framework/glm/src/empty.cpp

framework/glm/CMakeFiles/GLM_MODULE.dir/src/empty.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GLM_MODULE.dir/src/empty.cpp.i"
	cd /home/pdancak/school/MrPain_T/MUNI/PV227/framework/build/framework/glm && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pdancak/school/MrPain_T/MUNI/PV227/framework/framework/glm/src/empty.cpp > CMakeFiles/GLM_MODULE.dir/src/empty.cpp.i

framework/glm/CMakeFiles/GLM_MODULE.dir/src/empty.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GLM_MODULE.dir/src/empty.cpp.s"
	cd /home/pdancak/school/MrPain_T/MUNI/PV227/framework/build/framework/glm && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pdancak/school/MrPain_T/MUNI/PV227/framework/framework/glm/src/empty.cpp -o CMakeFiles/GLM_MODULE.dir/src/empty.cpp.s

# Object files for target GLM_MODULE
GLM_MODULE_OBJECTS = \
"CMakeFiles/GLM_MODULE.dir/src/empty.cpp.o"

# External object files for target GLM_MODULE
GLM_MODULE_EXTERNAL_OBJECTS =

framework/glm/libGLM_MODULE.a: framework/glm/CMakeFiles/GLM_MODULE.dir/src/empty.cpp.o
framework/glm/libGLM_MODULE.a: framework/glm/CMakeFiles/GLM_MODULE.dir/build.make
framework/glm/libGLM_MODULE.a: framework/glm/CMakeFiles/GLM_MODULE.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pdancak/school/MrPain_T/MUNI/PV227/framework/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libGLM_MODULE.a"
	cd /home/pdancak/school/MrPain_T/MUNI/PV227/framework/build/framework/glm && $(CMAKE_COMMAND) -P CMakeFiles/GLM_MODULE.dir/cmake_clean_target.cmake
	cd /home/pdancak/school/MrPain_T/MUNI/PV227/framework/build/framework/glm && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GLM_MODULE.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
framework/glm/CMakeFiles/GLM_MODULE.dir/build: framework/glm/libGLM_MODULE.a
.PHONY : framework/glm/CMakeFiles/GLM_MODULE.dir/build

framework/glm/CMakeFiles/GLM_MODULE.dir/clean:
	cd /home/pdancak/school/MrPain_T/MUNI/PV227/framework/build/framework/glm && $(CMAKE_COMMAND) -P CMakeFiles/GLM_MODULE.dir/cmake_clean.cmake
.PHONY : framework/glm/CMakeFiles/GLM_MODULE.dir/clean

framework/glm/CMakeFiles/GLM_MODULE.dir/depend:
	cd /home/pdancak/school/MrPain_T/MUNI/PV227/framework/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pdancak/school/MrPain_T/MUNI/PV227/framework /home/pdancak/school/MrPain_T/MUNI/PV227/framework/framework/glm /home/pdancak/school/MrPain_T/MUNI/PV227/framework/build /home/pdancak/school/MrPain_T/MUNI/PV227/framework/build/framework/glm /home/pdancak/school/MrPain_T/MUNI/PV227/framework/build/framework/glm/CMakeFiles/GLM_MODULE.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : framework/glm/CMakeFiles/GLM_MODULE.dir/depend
