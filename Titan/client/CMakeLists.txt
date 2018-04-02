cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)
project(Titan VERSION 0.1.0 LANGUAGES CXX)

set(SOURCE_FILES include/client.hpp src/server.cpp)
add_executable(client ${SOURCE_FILES})
