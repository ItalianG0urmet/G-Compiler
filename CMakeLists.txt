# Def
cmake_minimum_required(VERSION 3.10)
project(compiler)

# Clang
option(CLANG "Use Clang as the compiler" OFF)
if(CLANG)
  message(STATUS "Using Clang compiler")
  set(CMAKE_C_COMPILER clang CACHE STRING "" FORCE)
  set(CMAKE_CXX_COMPILER clang++ CACHE STRING "" FORCE)
endif()

# LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM CMake dir: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs
    core
    analysis
    executionengine
    native
    x86codegen
    x86desc
    x86info
    x86asmparser
    x86disassembler
    mcjit
    orcjit
    passes
    support
    target
)

# Enable debug and disable optimization for clean debug
option(DEBUG "Apply the -f -O0 -Wall" OFF)
if(DEBUG)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall") 
endif()

# Base
file(GLOB_RECURSE SOURCE "src/*.c")
add_executable(compiler ${SOURCE})
target_link_libraries(compiler PRIVATE ${llvm_libs})
target_include_directories(compiler PRIVATE include)

