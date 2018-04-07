cmake_minimum_required(VERSION 3.4.3)

project(simple-jit)

# this set is temporary added
set(LLVM_DIR /home/eldar/Contrib/llvm-6.0.0/lib/cmake/llvm)

# Make LLVM available for usage.
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "USING LLVMConfig.cmake in: ${LLVM_DIR}")

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-std=c++17")
add_subdirectory(src)
