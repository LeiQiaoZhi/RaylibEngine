cmake_minimum_required(VERSION 3.16)

project(all)

message(STATUS "-------------------------------------------------------------")
message(STATUS "PROJECT_NAME       = ${PROJECT_NAME}")
message(STATUS "CMAKE_SOURCE_DIR   = ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR   = ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR   = ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "-------------------------------------------------------------")

add_subdirectory(apps/editor ${CMAKE_CURRENT_BINARY_DIR}/editor_build)
add_subdirectory(apps/shader-graph ${CMAKE_CURRENT_BINARY_DIR}/shader-graph_build)
add_subdirectory(apps/cubemap ${CMAKE_CURRENT_BINARY_DIR}/cubemap_build)
add_subdirectory(apps/ui ${CMAKE_CURRENT_BINARY_DIR}/ui_build)
