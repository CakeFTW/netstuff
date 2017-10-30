cmake_minimum_required(VERSION 3.5)
project(gameServer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(SOURCE_FILES main.cpp Server.cpp Server.h Client.cpp Client.h CONSTANTS.h)
add_executable(gameServer ${SOURCE_FILES})