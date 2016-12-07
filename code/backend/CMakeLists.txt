cmake_minimum_required(VERSION 3.3)
set (CMAKE_CXX_STANDARD 14)
project(backend)

find_package (Threads REQUIRED)

#Bring the headers into the project
include_directories(include)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(backend ${SOURCES})
target_link_libraries (backend ${CMAKE_THREAD_LIBS_INIT})