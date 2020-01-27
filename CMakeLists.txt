cmake_minimum_required(VERSION 3.15)
project(PyRT C)

set(CMAKE_C_STANDARD 11)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

add_library(PyRT pyrt.c)