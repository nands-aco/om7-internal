add_library(heg
	src/lib.cpp
)

target_include_directories(heg
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
