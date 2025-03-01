cmake_minimum_required(VERSION 3.16)

project(raylib-starter)

set(CMAKE_CXX_STANDARD 17)

# Add Lua
add_subdirectory(external/lua)
#set(LUA_INCLUDE_DIR "/opt/homebrew/include")
#set(LUA_LIBRARY "/opt/homebrew/lib/liblua.dylib")
#include_directories(${LUA_INCLUDE_DIR})

add_subdirectory(vendor/raylib)
add_subdirectory(vendor/json)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# Add raylib include directory
target_include_directories(${PROJECT_NAME} PRIVATE vendor/raylib/src)

# Add raygui include directory
target_include_directories(${PROJECT_NAME} PRIVATE vendor/raygui/src)

target_include_directories(${PROJECT_NAME} PRIVATE vendor/sol2/single)


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

target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE PROJECT_DIR="${CMAKE_SOURCE_DIR}"
        PRIVATE ASSET_DIR="${CMAKE_SOURCE_DIR}/assets"
        PRIVATE INTERNAL_ASSET_DIR="${CMAKE_SOURCE_DIR}/internal-assets"
        PRIVATE SHADER_EXAMPLES_ASSET_DIR="${CMAKE_SOURCE_DIR}/vendor/raylib/examples/shaders/resources"
)

add_compile_options(-Wno-deprecated-declarations)

if(MSVC)
    add_compile_options(/Zc:preprocessor)  # for better C/C++ compatibility
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)  # disable warnings about secure CRT functions
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")  # more permissive mode
endif()