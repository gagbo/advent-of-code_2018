cmake_minimum_required(VERSION 3.10)

project(AdventOfCodeDay15 VERSION 1.0
    DESCRIPTION "Day 15 of Advent of Code"
    LANGUAGES CXX)
set(MAIN_NAME "day15")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-pipe -Wpedantic -Wshadow -Wconversion \
-Wsign-conversion -Wall -Wextra -fexceptions")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -D_FORTIFY_SOURCE=3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -D_FORTIFY_SOURCE=2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -D_FORTIFY_SOURCE=2 -g -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -Og -g -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_COVERAGE "-O3 -Og -g -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_LINKER_FLAGS_DEBUG "-lasan")
set(CMAKE_CXX_LINKER_FLAGS_COVERAGE "-fprofile-arcs -ftest-coverage")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


add_library(baselib STATIC src/lib.cpp include/${MAIN_NAME}/lib.hpp)
target_include_directories(baselib PUBLIC include)
target_compile_features(baselib PUBLIC cxx_std_17)

add_library(entity STATIC src/entity.cpp include/${MAIN_NAME}/entity.hpp)
target_include_directories(entity PUBLIC include)
target_compile_features(entity PUBLIC cxx_std_17)

add_library(board STATIC src/board.cpp include/${MAIN_NAME}/board.hpp)
target_include_directories(board PUBLIC include)
target_link_libraries(board PUBLIC entity)
target_compile_features(board PUBLIC cxx_std_17)

add_executable("${MAIN_NAME}" src/main.cpp)
target_compile_features("${MAIN_NAME}" PUBLIC cxx_std_17)
target_include_directories("${MAIN_NAME}" PUBLIC include)
target_link_libraries("${MAIN_NAME}" PUBLIC board entity)

# Tests
option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
