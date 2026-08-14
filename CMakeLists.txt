cmake_minimum_required(VERSION 3.20)

project(cpp_tetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

add_executable(tetris
  src/main.cpp
)

target_link_libraries(tetris PRIVATE SDL3::SDL3)
