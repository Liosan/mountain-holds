cmake_minimum_required(VERSION 3.2)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(Mountainhomes)

set(OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/Output")
set(ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})
foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_PATH})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_PATH})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_PATH})
endforeach (OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

set(SFML_ROOT "../SFML-2.3.2")
set(SFML_REQUIRED_LIBRARIES system window graphics)
find_package(SFML 2.3 REQUIRED ${SFML_REQUIRED_LIBRARIES})
include_directories(${SFML_INCLUDE_DIR})

set(MH_LIBRARIES foundation scripting entities gui map rendering)
foreach (MH_LIBRARY ${MH_LIBRARIES})
	add_subdirectory(${MH_LIBRARY})
endforeach()

set(EXECUTABLE_NAME "Mountainhomes")
FILE(GLOB EXECUTABLE_SOURCE_FILES src/*.h src/*.hpp src/*.cpp)
add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_SOURCE_FILES})
foreach (MH_LIBRARY ${MH_LIBRARIES})
	include_directories(${MH_LIBRARY}/include)
endforeach()
target_link_libraries(
	${EXECUTABLE_NAME} 
	${SFML_LIBRARIES} foundation
)

foreach (SFML_LIBRARY ${SFML_REQUIRED_LIBRARIES})
	# Debug
	add_custom_command(
		TARGET ${EXECUTABLE_NAME} 
		POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_ROOT}/bin/sfml-${SFML_LIBRARY}-d-2.dll"
        $<TARGET_FILE_DIR:Mountainhomes>)
	# Release
	add_custom_command(
		TARGET ${EXECUTABLE_NAME} 
		POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_ROOT}/bin/sfml-${SFML_LIBRARY}-2.dll"
        $<TARGET_FILE_DIR:Mountainhomes>)
endforeach(SFML_LIBRARY ${SFML_REQUIRED_LIBRARIES})