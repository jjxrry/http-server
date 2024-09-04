cmake_minimum_required(VERSION 3.10)
project(http-server)

set(CMAKE_CXX_STANDARD 17)

# Add source files
add_executable(http-server src/main.cpp)
