cmake_minimum_required(VERSION 3.3)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-Wall -std=c++11")
project(frontend)

find_package (Threads REQUIRED)

#Bring the headers into the project
include_directories(include)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(frontend ${SOURCES})
target_link_libraries (frontend ${CMAKE_THREAD_LIBS_INIT})
