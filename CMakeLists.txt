cmake_minimum_required(VERSION 3.10)
project(compiler)
file(GLOB SOURCE "src/*.c")
add_executable(compiler ${SOURCE})
target_include_directories(compiler PRIVATE include)

