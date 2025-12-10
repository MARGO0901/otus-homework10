cmake_minimum_required(VERSION 3.10)
project(async_cli)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib)

add_executable(async_cli main.cpp)
target_link_libraries(async_cli libasync)