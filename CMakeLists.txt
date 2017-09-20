cmake_minimum_required(VERSION 3.8)
project(strong_enum)

set(CMAKE_CXX_STANDARD 14)
add_library(strong_enum INTERFACE)

set(TEST_SOURCE_FILES include/strong_enum.h tests/catch.hpp tests/tests_main.cpp tests/strong_enum_tests.cpp)
add_executable(strong_enum_tests ${TEST_SOURCE_FILES})