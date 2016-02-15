cmake_minimum_required(VERSION 3.2)
project(map)

FILE(GLOB PUBLIC_FILES include/*.h include/*.hpp)
FILE(GLOB PRIVATE_FILES src/*.h src/*.hpp src/*.cpp)

add_library (
	${PROJECT_NAME} SHARED
	${PUBLIC_FILES}
	${PRIVATE_FILES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	LINKER_LANGUAGE CXX
)

#add_subdirectory(unit_tests)