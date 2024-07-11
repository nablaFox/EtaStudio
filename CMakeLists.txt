cmake_minimum_required(VERSION 3.10)
project(EtaStudio)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Paths
set(SRC_PATH src)
set(INCLUDE_PATH include)
set(BIN_PATH bin)
set(SHADERS_PATH assets/shaders)

# Include directories
include_directories(
    lib/glm
    lib/glfw/include
    lib/vkbootstrap
    lib/vma/include
    lib/stb_image
    lib/fastgltf
    lib/fastgltf/include
    lib/fmt/include
    lib/entt
)

# Source files
file(GLOB_RECURSE SRC_FILES
    "${SRC_PATH}/*.cpp"
    "${SRC_PATH}/*.hpp"
    "lib/vkbootstrap/VkBootstrap.cpp"
)

# Shader files
file(GLOB SHADER_FILES
    "${SHADERS_PATH}/*.vert"
    "${SHADERS_PATH}/*.frag"
    "${SHADERS_PATH}/*.comp"
)

# Add executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Add precompiled header
target_precompile_headers(${PROJECT_NAME} PRIVATE ${INCLUDE_PATH}/eta_pch.hpp)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/lib/glfw/src/libglfw3.a
    ${CMAKE_SOURCE_DIR}/lib/fmt/libfmt.a
    ${CMAKE_SOURCE_DIR}/lib/fastgltf/libfastgltf.a
    dl
    pthread
    X11
    Xxf86vm
    Xrandr
    Xi
    vulkan
)

# Custom command to compile shaders
set(SPV_FILES "")
foreach(SHADER ${SHADER_FILES})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    set(SPV_FILE ${SHADERS_PATH}/${FILE_NAME}.spv)
    add_custom_command(
        OUTPUT ${SPV_FILE}
        COMMAND glslc ${SHADER} -o ${SPV_FILE}
        DEPENDS ${SHADER}
        COMMENT "Compiling shader ${FILE_NAME}"
    )
    list(APPEND SPV_FILES ${SPV_FILE})
endforeach()

# Add custom target for shaders
add_custom_target(shaders ALL DEPENDS ${SPV_FILES})

# Ensure shaders are built before the main executable
add_dependencies(${PROJECT_NAME} shaders)

# Ensure shaders are cleaned with the main executable
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${SPV_FILES}")
