cmake_minimum_required (VERSION 3.3)
project (wifi_conn)
find_package (Boost REQUIRED)

if(Boost_FOUND)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-lboost_system -lpthread")
endif()

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(client ${SOURCES})
target_link_libraries(client ${Boost_LIBRARIES})