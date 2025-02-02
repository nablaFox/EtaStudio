# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/icecube/Projects/EtaStudio

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/icecube/Projects/EtaStudio

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components
.PHONY : list_install_components/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local/fast

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/icecube/Projects/EtaStudio/CMakeFiles /home/icecube/Projects/EtaStudio//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/icecube/Projects/EtaStudio/CMakeFiles 0
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
# Target rules for targets named EtaStudio

# Build rule for target.
EtaStudio: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 EtaStudio
.PHONY : EtaStudio

# fast build rule for target.
EtaStudio/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/build
.PHONY : EtaStudio/fast

#=============================================================================
# Target rules for targets named shaders

# Build rule for target.
shaders: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 shaders
.PHONY : shaders

# fast build rule for target.
shaders/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/shaders.dir/build.make CMakeFiles/shaders.dir/build
.PHONY : shaders/fast

#=============================================================================
# Target rules for targets named uninstall

# Build rule for target.
uninstall: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 uninstall
.PHONY : uninstall

# fast build rule for target.
uninstall/fast:
	$(MAKE) $(MAKESILENT) -f lib/glfw/CMakeFiles/uninstall.dir/build.make lib/glfw/CMakeFiles/uninstall.dir/build
.PHONY : uninstall/fast

#=============================================================================
# Target rules for targets named glfw

# Build rule for target.
glfw: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 glfw
.PHONY : glfw

# fast build rule for target.
glfw/fast:
	$(MAKE) $(MAKESILENT) -f lib/glfw/src/CMakeFiles/glfw.dir/build.make lib/glfw/src/CMakeFiles/glfw.dir/build
.PHONY : glfw/fast

#=============================================================================
# Target rules for targets named update_mappings

# Build rule for target.
update_mappings: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 update_mappings
.PHONY : update_mappings

# fast build rule for target.
update_mappings/fast:
	$(MAKE) $(MAKESILENT) -f lib/glfw/src/CMakeFiles/update_mappings.dir/build.make lib/glfw/src/CMakeFiles/update_mappings.dir/build
.PHONY : update_mappings/fast

#=============================================================================
# Target rules for targets named fmt

# Build rule for target.
fmt: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 fmt
.PHONY : fmt

# fast build rule for target.
fmt/fast:
	$(MAKE) $(MAKESILENT) -f lib/fmt/CMakeFiles/fmt.dir/build.make lib/fmt/CMakeFiles/fmt.dir/build
.PHONY : fmt/fast

#=============================================================================
# Target rules for targets named fastgltf

# Build rule for target.
fastgltf: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 fastgltf
.PHONY : fastgltf

# fast build rule for target.
fastgltf/fast:
	$(MAKE) $(MAKESILENT) -f lib/fastgltf/CMakeFiles/fastgltf.dir/build.make lib/fastgltf/CMakeFiles/fastgltf.dir/build
.PHONY : fastgltf/fast

# target to build an object file
cmake_pch.hxx.gch:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/cmake_pch.hxx.gch
.PHONY : cmake_pch.hxx.gch

# target to preprocess a source file
cmake_pch.hxx.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/cmake_pch.hxx.i
.PHONY : cmake_pch.hxx.i

# target to generate assembly for a file
cmake_pch.hxx.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/cmake_pch.hxx.s
.PHONY : cmake_pch.hxx.s

demos/main.o: demos/main.cpp.o
.PHONY : demos/main.o

# target to build an object file
demos/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/demos/main.cpp.o
.PHONY : demos/main.cpp.o

demos/main.i: demos/main.cpp.i
.PHONY : demos/main.i

# target to preprocess a source file
demos/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/demos/main.cpp.i
.PHONY : demos/main.cpp.i

demos/main.s: demos/main.cpp.s
.PHONY : demos/main.s

# target to generate assembly for a file
demos/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/demos/main.cpp.s
.PHONY : demos/main.cpp.s

lib/vkbootstrap/VkBootstrap.o: lib/vkbootstrap/VkBootstrap.cpp.o
.PHONY : lib/vkbootstrap/VkBootstrap.o

# target to build an object file
lib/vkbootstrap/VkBootstrap.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/lib/vkbootstrap/VkBootstrap.cpp.o
.PHONY : lib/vkbootstrap/VkBootstrap.cpp.o

lib/vkbootstrap/VkBootstrap.i: lib/vkbootstrap/VkBootstrap.cpp.i
.PHONY : lib/vkbootstrap/VkBootstrap.i

# target to preprocess a source file
lib/vkbootstrap/VkBootstrap.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/lib/vkbootstrap/VkBootstrap.cpp.i
.PHONY : lib/vkbootstrap/VkBootstrap.cpp.i

lib/vkbootstrap/VkBootstrap.s: lib/vkbootstrap/VkBootstrap.cpp.s
.PHONY : lib/vkbootstrap/VkBootstrap.s

# target to generate assembly for a file
lib/vkbootstrap/VkBootstrap.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/lib/vkbootstrap/VkBootstrap.cpp.s
.PHONY : lib/vkbootstrap/VkBootstrap.cpp.s

src/eta_bindings.o: src/eta_bindings.cpp.o
.PHONY : src/eta_bindings.o

# target to build an object file
src/eta_bindings.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_bindings.cpp.o
.PHONY : src/eta_bindings.cpp.o

src/eta_bindings.i: src/eta_bindings.cpp.i
.PHONY : src/eta_bindings.i

# target to preprocess a source file
src/eta_bindings.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_bindings.cpp.i
.PHONY : src/eta_bindings.cpp.i

src/eta_bindings.s: src/eta_bindings.cpp.s
.PHONY : src/eta_bindings.s

# target to generate assembly for a file
src/eta_bindings.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_bindings.cpp.s
.PHONY : src/eta_bindings.cpp.s

src/eta_descriptor.o: src/eta_descriptor.cpp.o
.PHONY : src/eta_descriptor.o

# target to build an object file
src/eta_descriptor.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_descriptor.cpp.o
.PHONY : src/eta_descriptor.cpp.o

src/eta_descriptor.i: src/eta_descriptor.cpp.i
.PHONY : src/eta_descriptor.i

# target to preprocess a source file
src/eta_descriptor.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_descriptor.cpp.i
.PHONY : src/eta_descriptor.cpp.i

src/eta_descriptor.s: src/eta_descriptor.cpp.s
.PHONY : src/eta_descriptor.s

# target to generate assembly for a file
src/eta_descriptor.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_descriptor.cpp.s
.PHONY : src/eta_descriptor.cpp.s

src/eta_device.o: src/eta_device.cpp.o
.PHONY : src/eta_device.o

# target to build an object file
src/eta_device.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_device.cpp.o
.PHONY : src/eta_device.cpp.o

src/eta_device.i: src/eta_device.cpp.i
.PHONY : src/eta_device.i

# target to preprocess a source file
src/eta_device.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_device.cpp.i
.PHONY : src/eta_device.cpp.i

src/eta_device.s: src/eta_device.cpp.s
.PHONY : src/eta_device.s

# target to generate assembly for a file
src/eta_device.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_device.cpp.s
.PHONY : src/eta_device.cpp.s

src/eta_engine.o: src/eta_engine.cpp.o
.PHONY : src/eta_engine.o

# target to build an object file
src/eta_engine.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_engine.cpp.o
.PHONY : src/eta_engine.cpp.o

src/eta_engine.i: src/eta_engine.cpp.i
.PHONY : src/eta_engine.i

# target to preprocess a source file
src/eta_engine.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_engine.cpp.i
.PHONY : src/eta_engine.cpp.i

src/eta_engine.s: src/eta_engine.cpp.s
.PHONY : src/eta_engine.s

# target to generate assembly for a file
src/eta_engine.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_engine.cpp.s
.PHONY : src/eta_engine.cpp.s

src/eta_graphics_pipeline.o: src/eta_graphics_pipeline.cpp.o
.PHONY : src/eta_graphics_pipeline.o

# target to build an object file
src/eta_graphics_pipeline.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_graphics_pipeline.cpp.o
.PHONY : src/eta_graphics_pipeline.cpp.o

src/eta_graphics_pipeline.i: src/eta_graphics_pipeline.cpp.i
.PHONY : src/eta_graphics_pipeline.i

# target to preprocess a source file
src/eta_graphics_pipeline.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_graphics_pipeline.cpp.i
.PHONY : src/eta_graphics_pipeline.cpp.i

src/eta_graphics_pipeline.s: src/eta_graphics_pipeline.cpp.s
.PHONY : src/eta_graphics_pipeline.s

# target to generate assembly for a file
src/eta_graphics_pipeline.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_graphics_pipeline.cpp.s
.PHONY : src/eta_graphics_pipeline.cpp.s

src/eta_input_system.o: src/eta_input_system.cpp.o
.PHONY : src/eta_input_system.o

# target to build an object file
src/eta_input_system.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_input_system.cpp.o
.PHONY : src/eta_input_system.cpp.o

src/eta_input_system.i: src/eta_input_system.cpp.i
.PHONY : src/eta_input_system.i

# target to preprocess a source file
src/eta_input_system.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_input_system.cpp.i
.PHONY : src/eta_input_system.cpp.i

src/eta_input_system.s: src/eta_input_system.cpp.s
.PHONY : src/eta_input_system.s

# target to generate assembly for a file
src/eta_input_system.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_input_system.cpp.s
.PHONY : src/eta_input_system.cpp.s

src/eta_material.o: src/eta_material.cpp.o
.PHONY : src/eta_material.o

# target to build an object file
src/eta_material.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_material.cpp.o
.PHONY : src/eta_material.cpp.o

src/eta_material.i: src/eta_material.cpp.i
.PHONY : src/eta_material.i

# target to preprocess a source file
src/eta_material.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_material.cpp.i
.PHONY : src/eta_material.cpp.i

src/eta_material.s: src/eta_material.cpp.s
.PHONY : src/eta_material.s

# target to generate assembly for a file
src/eta_material.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_material.cpp.s
.PHONY : src/eta_material.cpp.s

src/eta_mesh.o: src/eta_mesh.cpp.o
.PHONY : src/eta_mesh.o

# target to build an object file
src/eta_mesh.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_mesh.cpp.o
.PHONY : src/eta_mesh.cpp.o

src/eta_mesh.i: src/eta_mesh.cpp.i
.PHONY : src/eta_mesh.i

# target to preprocess a source file
src/eta_mesh.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_mesh.cpp.i
.PHONY : src/eta_mesh.cpp.i

src/eta_mesh.s: src/eta_mesh.cpp.s
.PHONY : src/eta_mesh.s

# target to generate assembly for a file
src/eta_mesh.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_mesh.cpp.s
.PHONY : src/eta_mesh.cpp.s

src/eta_model.o: src/eta_model.cpp.o
.PHONY : src/eta_model.o

# target to build an object file
src/eta_model.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_model.cpp.o
.PHONY : src/eta_model.cpp.o

src/eta_model.i: src/eta_model.cpp.i
.PHONY : src/eta_model.i

# target to preprocess a source file
src/eta_model.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_model.cpp.i
.PHONY : src/eta_model.cpp.i

src/eta_model.s: src/eta_model.cpp.s
.PHONY : src/eta_model.s

# target to generate assembly for a file
src/eta_model.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_model.cpp.s
.PHONY : src/eta_model.cpp.s

src/eta_pipeline.o: src/eta_pipeline.cpp.o
.PHONY : src/eta_pipeline.o

# target to build an object file
src/eta_pipeline.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_pipeline.cpp.o
.PHONY : src/eta_pipeline.cpp.o

src/eta_pipeline.i: src/eta_pipeline.cpp.i
.PHONY : src/eta_pipeline.i

# target to preprocess a source file
src/eta_pipeline.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_pipeline.cpp.i
.PHONY : src/eta_pipeline.cpp.i

src/eta_pipeline.s: src/eta_pipeline.cpp.s
.PHONY : src/eta_pipeline.s

# target to generate assembly for a file
src/eta_pipeline.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_pipeline.cpp.s
.PHONY : src/eta_pipeline.cpp.s

src/eta_rendering_system.o: src/eta_rendering_system.cpp.o
.PHONY : src/eta_rendering_system.o

# target to build an object file
src/eta_rendering_system.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_rendering_system.cpp.o
.PHONY : src/eta_rendering_system.cpp.o

src/eta_rendering_system.i: src/eta_rendering_system.cpp.i
.PHONY : src/eta_rendering_system.i

# target to preprocess a source file
src/eta_rendering_system.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_rendering_system.cpp.i
.PHONY : src/eta_rendering_system.cpp.i

src/eta_rendering_system.s: src/eta_rendering_system.cpp.s
.PHONY : src/eta_rendering_system.s

# target to generate assembly for a file
src/eta_rendering_system.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_rendering_system.cpp.s
.PHONY : src/eta_rendering_system.cpp.s

src/eta_scene.o: src/eta_scene.cpp.o
.PHONY : src/eta_scene.o

# target to build an object file
src/eta_scene.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_scene.cpp.o
.PHONY : src/eta_scene.cpp.o

src/eta_scene.i: src/eta_scene.cpp.i
.PHONY : src/eta_scene.i

# target to preprocess a source file
src/eta_scene.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_scene.cpp.i
.PHONY : src/eta_scene.cpp.i

src/eta_scene.s: src/eta_scene.cpp.s
.PHONY : src/eta_scene.s

# target to generate assembly for a file
src/eta_scene.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_scene.cpp.s
.PHONY : src/eta_scene.cpp.s

src/eta_swapchain.o: src/eta_swapchain.cpp.o
.PHONY : src/eta_swapchain.o

# target to build an object file
src/eta_swapchain.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_swapchain.cpp.o
.PHONY : src/eta_swapchain.cpp.o

src/eta_swapchain.i: src/eta_swapchain.cpp.i
.PHONY : src/eta_swapchain.i

# target to preprocess a source file
src/eta_swapchain.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_swapchain.cpp.i
.PHONY : src/eta_swapchain.cpp.i

src/eta_swapchain.s: src/eta_swapchain.cpp.s
.PHONY : src/eta_swapchain.s

# target to generate assembly for a file
src/eta_swapchain.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_swapchain.cpp.s
.PHONY : src/eta_swapchain.cpp.s

src/eta_texture.o: src/eta_texture.cpp.o
.PHONY : src/eta_texture.o

# target to build an object file
src/eta_texture.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_texture.cpp.o
.PHONY : src/eta_texture.cpp.o

src/eta_texture.i: src/eta_texture.cpp.i
.PHONY : src/eta_texture.i

# target to preprocess a source file
src/eta_texture.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_texture.cpp.i
.PHONY : src/eta_texture.cpp.i

src/eta_texture.s: src/eta_texture.cpp.s
.PHONY : src/eta_texture.s

# target to generate assembly for a file
src/eta_texture.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/EtaStudio.dir/build.make CMakeFiles/EtaStudio.dir/src/eta_texture.cpp.s
.PHONY : src/eta_texture.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... install"
	@echo "... install/local"
	@echo "... install/strip"
	@echo "... list_install_components"
	@echo "... rebuild_cache"
	@echo "... shaders"
	@echo "... uninstall"
	@echo "... update_mappings"
	@echo "... EtaStudio"
	@echo "... fastgltf"
	@echo "... fmt"
	@echo "... glfw"
	@echo "... cmake_pch.hxx.gch"
	@echo "... cmake_pch.hxx.i"
	@echo "... cmake_pch.hxx.s"
	@echo "... demos/main.o"
	@echo "... demos/main.i"
	@echo "... demos/main.s"
	@echo "... lib/vkbootstrap/VkBootstrap.o"
	@echo "... lib/vkbootstrap/VkBootstrap.i"
	@echo "... lib/vkbootstrap/VkBootstrap.s"
	@echo "... src/eta_bindings.o"
	@echo "... src/eta_bindings.i"
	@echo "... src/eta_bindings.s"
	@echo "... src/eta_descriptor.o"
	@echo "... src/eta_descriptor.i"
	@echo "... src/eta_descriptor.s"
	@echo "... src/eta_device.o"
	@echo "... src/eta_device.i"
	@echo "... src/eta_device.s"
	@echo "... src/eta_engine.o"
	@echo "... src/eta_engine.i"
	@echo "... src/eta_engine.s"
	@echo "... src/eta_graphics_pipeline.o"
	@echo "... src/eta_graphics_pipeline.i"
	@echo "... src/eta_graphics_pipeline.s"
	@echo "... src/eta_input_system.o"
	@echo "... src/eta_input_system.i"
	@echo "... src/eta_input_system.s"
	@echo "... src/eta_material.o"
	@echo "... src/eta_material.i"
	@echo "... src/eta_material.s"
	@echo "... src/eta_mesh.o"
	@echo "... src/eta_mesh.i"
	@echo "... src/eta_mesh.s"
	@echo "... src/eta_model.o"
	@echo "... src/eta_model.i"
	@echo "... src/eta_model.s"
	@echo "... src/eta_pipeline.o"
	@echo "... src/eta_pipeline.i"
	@echo "... src/eta_pipeline.s"
	@echo "... src/eta_rendering_system.o"
	@echo "... src/eta_rendering_system.i"
	@echo "... src/eta_rendering_system.s"
	@echo "... src/eta_scene.o"
	@echo "... src/eta_scene.i"
	@echo "... src/eta_scene.s"
	@echo "... src/eta_swapchain.o"
	@echo "... src/eta_swapchain.i"
	@echo "... src/eta_swapchain.s"
	@echo "... src/eta_texture.o"
	@echo "... src/eta_texture.i"
	@echo "... src/eta_texture.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

