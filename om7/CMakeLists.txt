cmake_minimum_required(VERSION 3.25)
project(om7 LANGUAGES C CXX)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
# 依存ライブラリを探します。
#
find_package(OpenGL REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
#
# GLADのソースを参照します。
#
add_library(glad STATIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/src/gl.c)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/src/gl.c PROPERTIES LANGUAGE C)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/include)
#
# 本体をコンパイルします。
#
add_executable(om7
	${CMAKE_CURRENT_SOURCE_DIR}/src/_main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/_glfw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/om7-app.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/om7-hgl.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/om7-hal.cpp
)
#
# 本体をリンクします。
#
target_link_libraries(om7 PRIVATE heg)
target_link_libraries(om7 PRIVATE glad)
target_link_libraries(om7 PRIVATE OpenGL::GL)
target_link_libraries(om7 PRIVATE OpenAL::OpenAL)
target_link_libraries(om7 PRIVATE glfw)
