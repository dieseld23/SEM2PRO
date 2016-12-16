cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++0x")
message("CMAKE_CXX_FLAGS is ${CMAKE_CXX_FLAGS}")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")		#Zybo wants this
project(sensornodes)

#Bring the headers into the project
include_directories(include)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

find_package (Threads)
add_executable(sensornode ${SOURCES})
target_link_libraries (sensornode ${CMAKE_THREAD_LIBS_INIT})
