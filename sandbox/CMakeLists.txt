cmake_minimum_required(VERSION 3.27)
project(sandbox)

set(CMAKE_CXX_STANDARD 23)

add_executable(sandbox main.cpp)

target_include_directories(sandbox PRIVATE ..)

target_link_libraries(sandbox tiny_autograd)
