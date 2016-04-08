cmake_minimum_required(VERSION 3.2)
project(map)
set(MH_map_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)

FILE(GLOB PUBLIC_FILES include/map/*.h include/map/*.hpp)
FILE(GLOB PRIVATE_FILES src/*.h src/*.hpp src/*.cpp)

add_library (
	${PROJECT_NAME} SHARED
	${PUBLIC_FILES}
	${PRIVATE_FILES}
)

include_directories("include/map")

target_link_libraries(${PROJECT_NAME} foundation)
include_directories(${MH_foundation_INCLUDES})
target_link_libraries(${PROJECT_NAME} scripting)
include_directories(${MH_scripting_INCLUDES})

include_directories(${SFML_INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(unit_tests)