cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)
project(Titan VERSION 0.1.0 LANGUAGES CXX)

#target_include_directories(${PROJECT_NAME} PUBLIC
#	$<BUILD_INTERFACE>:${CMAKE_CURRENT_SOURCE_DIR}/include>
#	$<INSTALL_INTERFACE:include>
#	PRIVATE src/
#)	

set(SOURCE_FILES include/havester.hpp src/harvester.cpp)
add_executable(harvester ${SOURCE_FILES})
