# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/build

# Include any dependencies generated for this target.
include third_party/cjson/CMakeFiles/cjson.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include third_party/cjson/CMakeFiles/cjson.dir/compiler_depend.make

# Include the progress variables for this target.
include third_party/cjson/CMakeFiles/cjson.dir/progress.make

# Include the compile flags for this target's objects.
include third_party/cjson/CMakeFiles/cjson.dir/flags.make

third_party/cjson/CMakeFiles/cjson.dir/cJSON.c.o: third_party/cjson/CMakeFiles/cjson.dir/flags.make
third_party/cjson/CMakeFiles/cjson.dir/cJSON.c.o: /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/third_party/cjson/cJSON.c
third_party/cjson/CMakeFiles/cjson.dir/cJSON.c.o: third_party/cjson/CMakeFiles/cjson.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object third_party/cjson/CMakeFiles/cjson.dir/cJSON.c.o"
	cd /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/build/third_party/cjson && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT third_party/cjson/CMakeFiles/cjson.dir/cJSON.c.o -MF CMakeFiles/cjson.dir/cJSON.c.o.d -o CMakeFiles/cjson.dir/cJSON.c.o -c /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/third_party/cjson/cJSON.c

third_party/cjson/CMakeFiles/cjson.dir/cJSON.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/cjson.dir/cJSON.c.i"
	cd /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/build/third_party/cjson && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/third_party/cjson/cJSON.c > CMakeFiles/cjson.dir/cJSON.c.i

third_party/cjson/CMakeFiles/cjson.dir/cJSON.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/cjson.dir/cJSON.c.s"
	cd /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/build/third_party/cjson && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/third_party/cjson/cJSON.c -o CMakeFiles/cjson.dir/cJSON.c.s

# Object files for target cjson
cjson_OBJECTS = \
"CMakeFiles/cjson.dir/cJSON.c.o"

# External object files for target cjson
cjson_EXTERNAL_OBJECTS =

third_party/cjson/libcjson.a: third_party/cjson/CMakeFiles/cjson.dir/cJSON.c.o
third_party/cjson/libcjson.a: third_party/cjson/CMakeFiles/cjson.dir/build.make
third_party/cjson/libcjson.a: third_party/cjson/CMakeFiles/cjson.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libcjson.a"
	cd /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/build/third_party/cjson && $(CMAKE_COMMAND) -P CMakeFiles/cjson.dir/cmake_clean_target.cmake
	cd /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/build/third_party/cjson && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cjson.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
third_party/cjson/CMakeFiles/cjson.dir/build: third_party/cjson/libcjson.a
.PHONY : third_party/cjson/CMakeFiles/cjson.dir/build

third_party/cjson/CMakeFiles/cjson.dir/clean:
	cd /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/build/third_party/cjson && $(CMAKE_COMMAND) -P CMakeFiles/cjson.dir/cmake_clean.cmake
.PHONY : third_party/cjson/CMakeFiles/cjson.dir/clean

third_party/cjson/CMakeFiles/cjson.dir/depend:
	cd /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/third_party/cjson /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/build /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/build/third_party/cjson /home/yassine-ahmed-ali/Documents/GooeyGUI_refactored/build/third_party/cjson/CMakeFiles/cjson.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : third_party/cjson/CMakeFiles/cjson.dir/depend

