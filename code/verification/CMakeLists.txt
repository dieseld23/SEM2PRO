cmake_minimum_required(VERSION 3.3)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "-Wall")
project(verification)

file(GLOB SOURCES "src/*.cpp")

add_executable(verification ${SOURCES})
