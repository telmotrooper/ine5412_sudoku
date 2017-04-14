cmake_minimum_required(VERSION 3.7)
project(Sudoku)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lpthread")

set(SOURCE_FILES sudoku.c)

add_executable(Sudoku ${SOURCE_FILES})