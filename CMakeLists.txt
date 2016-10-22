cmake_minimum_required(VERSION 3.6)
project(MikeProblems)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp expressionCalculator.cpp types.h expressionCalculator.h)
add_executable(MikeProblems ${SOURCE_FILES})