cmake_minimum_required(VERSION 3.8)
project(Titan)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES server/server.cpp server/client.cpp server/main.cpp includes/server.hpp)
add_executable(Titan ${SOURCE_FILES})
