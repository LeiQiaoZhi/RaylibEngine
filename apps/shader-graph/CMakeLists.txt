cmake_minimum_required(VERSION 3.16)

project(shader-graph)

message(STATUS "-------------------------------------------------------------")
message(STATUS "PROJECT_NAME       = ${PROJECT_NAME}")
message(STATUS "CMAKE_SOURCE_DIR   = ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR   = ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR   = ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "-------------------------------------------------------------")

add_subdirectory(../../modules/core ${CMAKE_CURRENT_BINARY_DIR}/core_build)

file(GLOB_RECURSE COMMON_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${PROJECT_NAME} ${COMMON_SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} core)