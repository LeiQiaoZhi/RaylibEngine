cmake_minimum_required(VERSION 3.16)

project(core)

message(STATUS "-------------------------------------------------------------")
message(STATUS "PROJECT_NAME       = ${PROJECT_NAME}")
message(STATUS "CMAKE_SOURCE_DIR   = ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR   = ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR   = ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "-------------------------------------------------------------")

if (NOT TARGET lua::lib)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../external/lua ${CMAKE_CURRENT_BINARY_DIR}/lua_build)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../vendor/raylib ${CMAKE_CURRENT_BINARY_DIR}/raylib_build)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../vendor/json ${CMAKE_CURRENT_BINARY_DIR}/json_build)
endif ()

if (NOT TARGET ${PROJECT_NAME})
    file(GLOB_RECURSE COMMON_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
    add_library(${PROJECT_NAME} ${COMMON_SOURCES})
endif ()

target_compile_features(core PUBLIC cxx_std_17)

target_compile_definitions(${PROJECT_NAME} PUBLIC
        PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../.."
        ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../../assets"
        INTERNAL_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../../internal-assets"
        SHADER_EXAMPLES_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../../vendor/raylib/examples/shaders/resources"
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../vendor/raylib/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../vendor/raygui/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../vendor/sol2/include)

target_link_libraries(${PROJECT_NAME} PUBLIC raylib)
target_link_libraries(${PROJECT_NAME} PUBLIC lua::lib)
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)