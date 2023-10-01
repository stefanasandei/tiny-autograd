cmake_minimum_required(VERSION 3.27)
project(tiny_autograd)

set(CMAKE_CXX_STANDARD 23)

add_library(tiny_autograd STATIC library.cpp)
