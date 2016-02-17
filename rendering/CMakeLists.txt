cmake_minimum_required(VERSION 3.2)
project(rendering)
set(MH_rendering_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)

FILE(GLOB PUBLIC_FILES include/rendering/*.h include/rendering/*.hpp)
FILE(GLOB PRIVATE_FILES src/*.h src/*.hpp src/*.cpp)

add_library (
	${PROJECT_NAME} SHARED
	${PUBLIC_FILES}
	${PRIVATE_FILES}
)

include_directories("include/rendering")

target_link_libraries(${PROJECT_NAME} foundation)
include_directories(${MH_foundation_INCLUDES})
target_link_libraries(${PROJECT_NAME} map)
include_directories(${MH_map_INCLUDES})

target_link_libraries(
	${PROJECT_NAME}
	debug ${SFML_GRAPHICS_LIBRARY_DYNAMIC_DEBUG}
	optimized ${SFML_GRAPHICS_LIBRARY_DYNAMIC_RELEASE}
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

#add_subdirectory(unit_tests)