# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pdancak/school/MrPain_T/PA010/proj2/noise

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pdancak/school/MrPain_T/PA010/proj2/noise/build

# Include any dependencies generated for this target.
include pa010_projects/noise/CMakeFiles/pa010_noise.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include pa010_projects/noise/CMakeFiles/pa010_noise.dir/compiler_depend.make

# Include the progress variables for this target.
include pa010_projects/noise/CMakeFiles/pa010_noise.dir/progress.make

# Include the compile flags for this target's objects.
include pa010_projects/noise/CMakeFiles/pa010_noise.dir/flags.make

pa010_projects/noise/CMakeFiles/pa010_noise.dir/application.cpp.o: pa010_projects/noise/CMakeFiles/pa010_noise.dir/flags.make
pa010_projects/noise/CMakeFiles/pa010_noise.dir/application.cpp.o: ../pa010_projects/noise/application.cpp
pa010_projects/noise/CMakeFiles/pa010_noise.dir/application.cpp.o: pa010_projects/noise/CMakeFiles/pa010_noise.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pdancak/school/MrPain_T/PA010/proj2/noise/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object pa010_projects/noise/CMakeFiles/pa010_noise.dir/application.cpp.o"
	cd /home/pdancak/school/MrPain_T/PA010/proj2/noise/build/pa010_projects/noise && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT pa010_projects/noise/CMakeFiles/pa010_noise.dir/application.cpp.o -MF CMakeFiles/pa010_noise.dir/application.cpp.o.d -o CMakeFiles/pa010_noise.dir/application.cpp.o -c /home/pdancak/school/MrPain_T/PA010/proj2/noise/pa010_projects/noise/application.cpp

pa010_projects/noise/CMakeFiles/pa010_noise.dir/application.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pa010_noise.dir/application.cpp.i"
	cd /home/pdancak/school/MrPain_T/PA010/proj2/noise/build/pa010_projects/noise && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pdancak/school/MrPain_T/PA010/proj2/noise/pa010_projects/noise/application.cpp > CMakeFiles/pa010_noise.dir/application.cpp.i

pa010_projects/noise/CMakeFiles/pa010_noise.dir/application.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pa010_noise.dir/application.cpp.s"
	cd /home/pdancak/school/MrPain_T/PA010/proj2/noise/build/pa010_projects/noise && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pdancak/school/MrPain_T/PA010/proj2/noise/pa010_projects/noise/application.cpp -o CMakeFiles/pa010_noise.dir/application.cpp.s

pa010_projects/noise/CMakeFiles/pa010_noise.dir/application_details.cpp.o: pa010_projects/noise/CMakeFiles/pa010_noise.dir/flags.make
pa010_projects/noise/CMakeFiles/pa010_noise.dir/application_details.cpp.o: ../pa010_projects/noise/application_details.cpp
pa010_projects/noise/CMakeFiles/pa010_noise.dir/application_details.cpp.o: pa010_projects/noise/CMakeFiles/pa010_noise.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pdancak/school/MrPain_T/PA010/proj2/noise/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object pa010_projects/noise/CMakeFiles/pa010_noise.dir/application_details.cpp.o"
	cd /home/pdancak/school/MrPain_T/PA010/proj2/noise/build/pa010_projects/noise && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT pa010_projects/noise/CMakeFiles/pa010_noise.dir/application_details.cpp.o -MF CMakeFiles/pa010_noise.dir/application_details.cpp.o.d -o CMakeFiles/pa010_noise.dir/application_details.cpp.o -c /home/pdancak/school/MrPain_T/PA010/proj2/noise/pa010_projects/noise/application_details.cpp

pa010_projects/noise/CMakeFiles/pa010_noise.dir/application_details.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pa010_noise.dir/application_details.cpp.i"
	cd /home/pdancak/school/MrPain_T/PA010/proj2/noise/build/pa010_projects/noise && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pdancak/school/MrPain_T/PA010/proj2/noise/pa010_projects/noise/application_details.cpp > CMakeFiles/pa010_noise.dir/application_details.cpp.i

pa010_projects/noise/CMakeFiles/pa010_noise.dir/application_details.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pa010_noise.dir/application_details.cpp.s"
	cd /home/pdancak/school/MrPain_T/PA010/proj2/noise/build/pa010_projects/noise && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pdancak/school/MrPain_T/PA010/proj2/noise/pa010_projects/noise/application_details.cpp -o CMakeFiles/pa010_noise.dir/application_details.cpp.s

pa010_projects/noise/CMakeFiles/pa010_noise.dir/main.cpp.o: pa010_projects/noise/CMakeFiles/pa010_noise.dir/flags.make
pa010_projects/noise/CMakeFiles/pa010_noise.dir/main.cpp.o: ../pa010_projects/noise/main.cpp
pa010_projects/noise/CMakeFiles/pa010_noise.dir/main.cpp.o: pa010_projects/noise/CMakeFiles/pa010_noise.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pdancak/school/MrPain_T/PA010/proj2/noise/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object pa010_projects/noise/CMakeFiles/pa010_noise.dir/main.cpp.o"
	cd /home/pdancak/school/MrPain_T/PA010/proj2/noise/build/pa010_projects/noise && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT pa010_projects/noise/CMakeFiles/pa010_noise.dir/main.cpp.o -MF CMakeFiles/pa010_noise.dir/main.cpp.o.d -o CMakeFiles/pa010_noise.dir/main.cpp.o -c /home/pdancak/school/MrPain_T/PA010/proj2/noise/pa010_projects/noise/main.cpp

pa010_projects/noise/CMakeFiles/pa010_noise.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pa010_noise.dir/main.cpp.i"
	cd /home/pdancak/school/MrPain_T/PA010/proj2/noise/build/pa010_projects/noise && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pdancak/school/MrPain_T/PA010/proj2/noise/pa010_projects/noise/main.cpp > CMakeFiles/pa010_noise.dir/main.cpp.i

pa010_projects/noise/CMakeFiles/pa010_noise.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pa010_noise.dir/main.cpp.s"
	cd /home/pdancak/school/MrPain_T/PA010/proj2/noise/build/pa010_projects/noise && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pdancak/school/MrPain_T/PA010/proj2/noise/pa010_projects/noise/main.cpp -o CMakeFiles/pa010_noise.dir/main.cpp.s

# Object files for target pa010_noise
pa010_noise_OBJECTS = \
"CMakeFiles/pa010_noise.dir/application.cpp.o" \
"CMakeFiles/pa010_noise.dir/application_details.cpp.o" \
"CMakeFiles/pa010_noise.dir/main.cpp.o"

# External object files for target pa010_noise
pa010_noise_EXTERNAL_OBJECTS =

pa010_projects/noise/pa010_noise: pa010_projects/noise/CMakeFiles/pa010_noise.dir/application.cpp.o
pa010_projects/noise/pa010_noise: pa010_projects/noise/CMakeFiles/pa010_noise.dir/application_details.cpp.o
pa010_projects/noise/pa010_noise: pa010_projects/noise/CMakeFiles/pa010_noise.dir/main.cpp.o
pa010_projects/noise/pa010_noise: pa010_projects/noise/CMakeFiles/pa010_noise.dir/build.make
pa010_projects/noise/pa010_noise: pa010_projects/framework/core/libFRAMEWORK_CORE.a
pa010_projects/noise/pa010_noise: pa010_projects/framework/opengl_3_3/libFRAMEWORK_OPENGL_3_3.a
pa010_projects/noise/pa010_noise: pa010_projects/framework/core/libFRAMEWORK_CORE.a
pa010_projects/noise/pa010_noise: vcpkg_installed/x64-linux/lib/libimgui.a
pa010_projects/noise/pa010_noise: vcpkg_installed/x64-linux/lib/libglad.a
pa010_projects/noise/pa010_noise: vcpkg_installed/x64-linux/lib/libglfw3.a
pa010_projects/noise/pa010_noise: /usr/lib64/librt.so
pa010_projects/noise/pa010_noise: /usr/lib64/libm.so
pa010_projects/noise/pa010_noise: /usr/lib64/libX11.so
pa010_projects/noise/pa010_noise: vcpkg_installed/x64-linux/lib/libtinyobjloader.a
pa010_projects/noise/pa010_noise: vcpkg_installed/x64-linux/lib/libgtest.a
pa010_projects/noise/pa010_noise: pa010_projects/noise/CMakeFiles/pa010_noise.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pdancak/school/MrPain_T/PA010/proj2/noise/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable pa010_noise"
	cd /home/pdancak/school/MrPain_T/PA010/proj2/noise/build/pa010_projects/noise && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pa010_noise.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
pa010_projects/noise/CMakeFiles/pa010_noise.dir/build: pa010_projects/noise/pa010_noise
.PHONY : pa010_projects/noise/CMakeFiles/pa010_noise.dir/build

pa010_projects/noise/CMakeFiles/pa010_noise.dir/clean:
	cd /home/pdancak/school/MrPain_T/PA010/proj2/noise/build/pa010_projects/noise && $(CMAKE_COMMAND) -P CMakeFiles/pa010_noise.dir/cmake_clean.cmake
.PHONY : pa010_projects/noise/CMakeFiles/pa010_noise.dir/clean

pa010_projects/noise/CMakeFiles/pa010_noise.dir/depend:
	cd /home/pdancak/school/MrPain_T/PA010/proj2/noise/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pdancak/school/MrPain_T/PA010/proj2/noise /home/pdancak/school/MrPain_T/PA010/proj2/noise/pa010_projects/noise /home/pdancak/school/MrPain_T/PA010/proj2/noise/build /home/pdancak/school/MrPain_T/PA010/proj2/noise/build/pa010_projects/noise /home/pdancak/school/MrPain_T/PA010/proj2/noise/build/pa010_projects/noise/CMakeFiles/pa010_noise.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : pa010_projects/noise/CMakeFiles/pa010_noise.dir/depend
