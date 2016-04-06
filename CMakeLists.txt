cmake_minimum_required(VERSION 3.2)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(MountainHolds)

if (WIN32)
	add_definitions("/W3 /WX /D_CRT_SECURE_NO_WARNINGS /wd4251 /nologo" )
endif (WIN32)

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

set(MH_DEPENDENCY_DIRECTORY "${CMAKE_SOURCE_DIR}/../msvc2015-x64")

set(SFML_ROOT "${MH_DEPENDENCY_DIRECTORY}/SFML-2.3.2")
set(SFML_REQUIRED_LIBRARIES system window graphics)
find_package(SFML 2.3 REQUIRED ${SFML_REQUIRED_LIBRARIES})
include_directories(${SFML_INCLUDE_DIR})

set(GTEST_ROOT "${MH_DEPENDENCY_DIRECTORY}/gtest-1.7.0")
find_package(GTEST 1.7 REQUIRED)

set(STACKLESS_PYTHON_ROOT "${MH_DEPENDENCY_DIRECTORY}/stackless-python-3.5")
find_package(StacklessPython 3.5 REQUIRED)

set(MH_LIBRARIES foundation scripting entities gui map rendering)
foreach (MH_LIBRARY ${MH_LIBRARIES})
	add_subdirectory(${MH_LIBRARY})
endforeach()

set(EXECUTABLE_NAME "MountainHolds")
FILE(GLOB EXECUTABLE_SOURCE_FILES src/*.h src/*.hpp src/*.cpp)
add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_SOURCE_FILES})
foreach (MH_LIBRARY ${MH_LIBRARIES})
	include_directories(${MH_LIBRARY}/include)
endforeach()
target_link_libraries(
	${EXECUTABLE_NAME} 
	${SFML_LIBRARIES} foundation map rendering scripting
)

foreach (SFML_LIBRARY ${SFML_REQUIRED_LIBRARIES})
	# Debug
	add_custom_command(
		TARGET ${EXECUTABLE_NAME} 
		POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_ROOT}/bin/sfml-${SFML_LIBRARY}-d-2.dll"
        $<TARGET_FILE_DIR:MountainHolds>)
	# Release
	add_custom_command(
		TARGET ${EXECUTABLE_NAME} 
		POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_ROOT}/bin/sfml-${SFML_LIBRARY}-2.dll"
        $<TARGET_FILE_DIR:MountainHolds>)
endforeach(SFML_LIBRARY ${SFML_REQUIRED_LIBRARIES})

set(GTEST_REQUIRED_DLLS gtest.dll gtest_main.dll gtestd.dll gtest_maind.dll)
foreach (GTEST_DLL ${GTEST_REQUIRED_DLLS})
	add_custom_command(
		TARGET ${EXECUTABLE_NAME} 
		POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GTEST_ROOT}/bin/${GTEST_DLL}"
        $<TARGET_FILE_DIR:MountainHolds>)
endforeach (GTEST_DLL ${GTEST_REQUIRED_DLLS})

set(STACKLESS_PYTHON_REQUIRED_DLLS python35_d.dll python35.dll)
foreach (STACKLESS_PYTHON_DLL ${STACKLESS_PYTHON_REQUIRED_DLLS})
	add_custom_command(
		TARGET ${EXECUTABLE_NAME} 
		POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${STACKLESS_PYTHON_ROOT}/bin/${STACKLESS_PYTHON_DLL}"
        $<TARGET_FILE_DIR:MountainHolds>)
endforeach (STACKLESS_PYTHON_DLL ${STACKLESS_PYTHON_REQUIRED_DLLS})