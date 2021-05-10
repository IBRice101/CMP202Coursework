cmake_minimum_required(VERSION 3.17)
project(Mandelbrot)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(Mandelbrot main.cpp)