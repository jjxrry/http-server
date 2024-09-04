cmake_minimum_required(VERSION 3.10)
project(http-server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-pedantic-errors -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion)

add_executable(http-server src/main.cpp)
