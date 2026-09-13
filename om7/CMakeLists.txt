find_package(OpenGL REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_library(glad STATIC src/glad/src/gl.c)
target_include_directories(glad PUBLIC src/glad/include)
set_source_files_properties(src/glad/src/gl.c PROPERTIES LANGUAGE C)

add_executable(om7
	src/_main.cpp
	src/_glfw.cpp
	src/om7-app.cpp
	src/om7-hgl.cpp
)

target_link_libraries(om7 PRIVATE heg)
target_link_libraries(om7 PRIVATE glad)
target_link_libraries(om7 PRIVATE OpenGL::GL)
target_link_libraries(om7 PRIVATE OpenAL::OpenAL)
target_link_libraries(om7 PRIVATE glfw)
