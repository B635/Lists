cmake_minimum_required(VERSION 3.10)
project(Lists LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")


add_subdirectory(backend)


