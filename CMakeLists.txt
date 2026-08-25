cmake_minimum_required(VERSION 3.25)
cmake_policy(SET CMP0072 NEW)
# CMakeのバージョン4以降では、これがないと警告が出るケースがある。
#cmake_policy(SET CMP0207 NEW)

project(om7 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	add_compile_options("/utf-8")
endif()

add_subdirectory(heg)
add_subdirectory(om7)
