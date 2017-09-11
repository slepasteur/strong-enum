cmake_minimum_required(VERSION 3.8)
project(strong_enum)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES include/strong_enum.h)
add_library(strong_enum ${SOURCE_FILES})