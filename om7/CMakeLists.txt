find_package(OpenGL REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(om7
	src/main.cpp
	src/run_glfw.cpp
)

target_link_libraries(om7 PRIVATE heg)
target_link_libraries(om7 PRIVATE OpenGL::GL)
target_link_libraries(om7 PRIVATE OpenAL::OpenAL)
target_link_libraries(om7 PRIVATE glfw)
target_link_libraries(om7 PRIVATE glad::glad)
