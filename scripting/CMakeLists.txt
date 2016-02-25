cmake_minimum_required(VERSION 3.2)
project(scripting)
set(MH_scripting_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)

FILE(GLOB PUBLIC_FILES include/scripting/*.h include/scripting/*.hpp)
FILE(GLOB PRIVATE_FILES src/*.h src/*.hpp src/*.cpp)

add_library (
	${PROJECT_NAME} SHARED
	${PUBLIC_FILES}
	${PRIVATE_FILES}
)

include_directories("include/scripting")

target_link_libraries(${PROJECT_NAME} foundation)
include_directories(${MH_foundation_INCLUDES})

target_link_libraries(
	${PROJECT_NAME}
	debug ${STACKLESS_PYTHON_DEBUG_LIBRARIES}
	optimized ${STACKLESS_PYTHON_LIBRARIES}
)
include_directories(${STACKLESS_PYTHON_INCLUDE_DIRS})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(unit_tests)