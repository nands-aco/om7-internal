cmake_minimum_required(VERSION 3.16)

add_executable(om7
	src/main.cpp
)

target_link_libraries(om7
	PRIVATE heg
)
