cmake_minimum_required(VERSION 3.10)
project(compiler)
#Enable debug and disable optimization for clean debug
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall") 
file(GLOB SOURCE "src/*.c")
add_executable(compiler ${SOURCE})
target_include_directories(compiler PRIVATE include)

