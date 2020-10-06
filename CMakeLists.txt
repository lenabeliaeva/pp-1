# cmake_minimum_required(VERSION <specify CMake version here>)
project(pp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(openmp_test_clion main.cpp parallizeoptions.h)