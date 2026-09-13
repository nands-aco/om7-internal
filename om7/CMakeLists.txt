find_package(OpenGL REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

#
# GLAD の参照
#
add_library(glad STATIC lib/glad/src/gl.c)
target_include_directories(glad PUBLIC lib/glad/include)
set_source_files_properties(lib/glad/src/gl.c PROPERTIES LANGUAGE C)

#
# 本体のコンパイル
#
add_executable(om7
	src/_main.cpp
	src/_glfw.cpp
	src/om7-app.cpp
	src/om7-hgl.cpp
)

#
# 本体のリンク
#
target_link_libraries(om7 PRIVATE heg)
target_link_libraries(om7 PRIVATE glad)
target_link_libraries(om7 PRIVATE OpenGL::GL)
target_link_libraries(om7 PRIVATE OpenAL::OpenAL)
target_link_libraries(om7 PRIVATE glfw)
