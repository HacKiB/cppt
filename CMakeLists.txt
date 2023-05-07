cmake_minimum_required(VERSION 3.21)
project(dig_some)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(dig_some main.cpp feature_test.h utils.h)
