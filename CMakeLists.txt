cmake_minimum_required(VERSION 3.3)
project(Calculator_wih_reverse_polish_notation)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp
        IsNumber.cpp
        )
add_executable(Calculator_wih_reverse_polish_notation ${SOURCE_FILES})