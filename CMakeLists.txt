cmake_minimum_required(VERSION 3.16)

project(raylib-starter)

set(CMAKE_CXX_STANDARD 17)

# Add Lua
add_subdirectory(external/lua)

add_subdirectory(vendor/raylib)
add_subdirectory(vendor/json)

add_library(common_defs INTERFACE)
target_compile_definitions(common_defs INTERFACE
        PROJECT_DIR="${CMAKE_SOURCE_DIR}"
        ASSET_DIR="${CMAKE_SOURCE_DIR}/assets"
        INTERNAL_ASSET_DIR="${CMAKE_SOURCE_DIR}/internal-assets"
        SHADER_EXAMPLES_ASSET_DIR="${CMAKE_SOURCE_DIR}/vendor/raylib/examples/shaders/resources"
)

file(GLOB_RECURSE COMMON_SOURCES "src/common/*.cpp")
add_executable(${PROJECT_NAME} ${COMMON_SOURCES} src/main.cpp)

# Add raylib include directory
target_include_directories(${PROJECT_NAME} PRIVATE vendor/raylib/src)

# Add raygui include directory
target_include_directories(${PROJECT_NAME} PRIVATE vendor/raygui/src)

target_include_directories(${PROJECT_NAME} PRIVATE vendor/sol2/include)


# Define path to ANGLE libraries if using ANGLE
set(USE_ANGLE OFF)
if (USE_ANGLE)
    set(ANGLE_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/libs")

    # Find ANGLE libraries
    find_library(ANGLE_GLESv2_LIBRARY libGLESv2.dylib PATHS ${ANGLE_LIBRARY_DIR})
    find_library(ANGLE_EGL_LIBRARY libEGL.dylib PATHS ${ANGLE_LIBRARY_DIR})

    # Check if the libraries were found
    if (NOT ANGLE_GLESv2_LIBRARY OR NOT ANGLE_EGL_LIBRARY)
        message(FATAL_ERROR "ANGLE libraries not found in ${ANGLE_LIBRARY_DIR}")
    endif ()

    # Set runtime path for ANGLE libraries
    set_target_properties(${PROJECT_NAME} PROPERTIES
            BUILD_RPATH ${ANGLE_LIBRARY_DIR}
    )
endif ()


# Link against raylib libraries and ANGLE if using ANGLE
target_link_libraries(${PROJECT_NAME} raylib)
target_link_libraries(${PROJECT_NAME} lua::lib)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)

if (USE_ANGLE)
    target_link_libraries(${PROJECT_NAME} ${ANGLE_GLESv2_LIBRARY} ${ANGLE_EGL_LIBRARY})
endif ()

target_link_libraries(${PROJECT_NAME} common_defs)


add_compile_options(-Wno-deprecated-declarations)

if(MSVC)
    add_compile_options(/Zc:preprocessor)  # for better C/C++ compatibility
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)  # disable warnings about secure CRT functions
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")  # more permissive mode
endif()

# Shader editor executable using raylib and raygui only.
file(GLOB_RECURSE SHADER_GRAPH_SOURCES "src/shader-graph/*.cpp")
add_executable(shader_editor ${SHADER_GRAPH_SOURCES} ${COMMON_SOURCES})

target_include_directories(shader_editor PRIVATE vendor/raylib/src)
target_include_directories(shader_editor PRIVATE vendor/raygui/src)
target_include_directories(shader_editor PRIVATE vendor/sol2/include)
target_include_directories(shader_editor PRIVATE /src)

target_link_libraries(shader_editor raylib)
target_link_libraries(shader_editor lua::lib)
target_link_libraries(shader_editor nlohmann_json::nlohmann_json)
target_link_libraries(shader_editor common_defs)

