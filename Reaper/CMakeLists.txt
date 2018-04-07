cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)
project(Reaper VERSION 0.1.0 LANGUAGES CXX)

#target_include_directories(${PROJECT_NAME} PUBLIC
#	$<BUILD_INTERFACE>:${CMAKE_CURRENT_SOURCE_DIR}/include>
#	$<INSTALL_INTERFACE:include>
#	PRIVATE src/
#)	

find_package(OpenCL REQUIRED)

set(SOURCE_FILES include/detect.hpp include/reaper.hpp src/reaper.cpp src/detect.cpp)
add_executable(reaper ${SOURCE_FILES})
target_link_libraries(reaper OpenCL::OpenCL)
