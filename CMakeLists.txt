cmake_minimum_required(VERSION 3.23)
project(kalkumulator)

set(CMAKE_CXX_STANDARD 23)

add_executable(kalkumulator main.cpp tokens.hpp types.hpp scanner.hpp error.hpp utils.hpp utils.cpp error.cpp parser.hpp expressions.hpp)
