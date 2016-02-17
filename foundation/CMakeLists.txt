cmake_minimum_required(VERSION 3.2)
project(foundation)
set(MH_foundation_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)

FILE(GLOB PUBLIC_FILES include/foundation/*.h include/foundation/*.hpp)
FILE(GLOB PRIVATE_FILES src/*.h src/*.hpp src/*.cpp)

add_library (
	${PROJECT_NAME} SHARED
	${PUBLIC_FILES}
	${PRIVATE_FILES}
)

include_directories("include/foundation")

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

#add_subdirectory(unit_tests)