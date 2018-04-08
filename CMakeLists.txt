cmake_minimum_required(VERSION 3.4.3)

project(simple-jit)

set(LLVM_DIR "" CACHE STRING "Direcotry with LLVM cmake files.")

# Make LLVM available for usage.
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "USING LLVMConfig.cmake in: ${LLVM_DIR}")

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-std=c++17")
add_subdirectory(src)
