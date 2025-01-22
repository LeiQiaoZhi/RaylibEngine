cmake_minimum_required(VERSION 3.16)

project(raygui-example)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(raylib)

file(GLOB_RECURSE SOURCES "raygui/examples/scroll_panel/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})

# Add raylib include directory
target_include_directories(${PROJECT_NAME} PRIVATE raylib/src)

# Add raygui include directory
target_include_directories(${PROJECT_NAME} PRIVATE raygui/src)


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
if (USE_ANGLE)
    target_link_libraries(${PROJECT_NAME} ${ANGLE_GLESv2_LIBRARY} ${ANGLE_EGL_LIBRARY})
endif ()

target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE PROJECT_DIR="${CMAKE_SOURCE_DIR}"
)

add_compile_options(-Wno-deprecated-declarations)