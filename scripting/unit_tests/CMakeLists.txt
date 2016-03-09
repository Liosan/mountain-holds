cmake_minimum_required(VERSION 3.2)
project(scripting-unit-tests)

FILE(GLOB SOURCE_FILES *.h *.hpp *.cpp)

add_executable (${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} foundation)
include_directories(${MH_foundation_INCLUDES})
target_link_libraries(${PROJECT_NAME} scripting)
include_directories(${MH_scripting_INCLUDES})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src")

target_link_libraries(
	${PROJECT_NAME}
	debug ${GTEST_LIBRARY_DEBUG}
	optimized ${GTEST_LIBRARY}
)
include_directories(${GTEST_INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
add_definitions(-DSCRIPTING_TEST_DATA_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/data")