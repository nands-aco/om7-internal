cmake_minimum_required(VERSION 3.25)
project(heg LANGUAGES C CXX)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
#
#
add_library(heg
	${CMAKE_CURRENT_SOURCE_DIR}/src/heg.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/heg-hgl.cpp
)
#
#
#
target_include_directories(heg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
