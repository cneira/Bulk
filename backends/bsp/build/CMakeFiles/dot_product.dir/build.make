# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /ufs/buurlage/bin/cmake-3.6.0-Linux-x86_64/bin/cmake

# The command to remove a file.
RM = /ufs/buurlage/bin/cmake-3.6.0-Linux-x86_64/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /ufs/buurlage/code/bulk/build

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /ufs/buurlage/code/bulk/build

# Include any dependencies generated for this target.
include CMakeFiles/dot_product.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dot_product.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dot_product.dir/flags.make

CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.o: CMakeFiles/dot_product.dir/flags.make
CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.o: /ufs/buurlage/code/bulk/examples/dot_product.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/ufs/buurlage/code/bulk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.o"
	/usr/lib64/ccache/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.o -c /ufs/buurlage/code/bulk/examples/dot_product.cpp

CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.i"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /ufs/buurlage/code/bulk/examples/dot_product.cpp > CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.i

CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.s"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /ufs/buurlage/code/bulk/examples/dot_product.cpp -o CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.s

CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.o.requires:

.PHONY : CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.o.requires

CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.o.provides: CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.o.requires
	$(MAKE) -f CMakeFiles/dot_product.dir/build.make CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.o.provides.build
.PHONY : CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.o.provides

CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.o.provides.build: CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.o


# Object files for target dot_product
dot_product_OBJECTS = \
"CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.o"

# External object files for target dot_product
dot_product_EXTERNAL_OBJECTS =

/ufs/buurlage/code/bulk/bin/dot_product: CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.o
/ufs/buurlage/code/bulk/bin/dot_product: CMakeFiles/dot_product.dir/build.make
/ufs/buurlage/code/bulk/bin/dot_product: CMakeFiles/dot_product.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/ufs/buurlage/code/bulk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /ufs/buurlage/code/bulk/bin/dot_product"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dot_product.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dot_product.dir/build: /ufs/buurlage/code/bulk/bin/dot_product

.PHONY : CMakeFiles/dot_product.dir/build

CMakeFiles/dot_product.dir/requires: CMakeFiles/dot_product.dir/ufs/buurlage/code/bulk/examples/dot_product.cpp.o.requires

.PHONY : CMakeFiles/dot_product.dir/requires

CMakeFiles/dot_product.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dot_product.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dot_product.dir/clean

CMakeFiles/dot_product.dir/depend:
	cd /ufs/buurlage/code/bulk/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /ufs/buurlage/code/bulk/build /ufs/buurlage/code/bulk/build /ufs/buurlage/code/bulk/build /ufs/buurlage/code/bulk/build /ufs/buurlage/code/bulk/build/CMakeFiles/dot_product.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dot_product.dir/depend
