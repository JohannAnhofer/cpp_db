cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0074 NEW) # allow environment overrides for find_package

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

message("CMake version: ${CMAKE_VERSION} ")
message("CMake make tool: ${CMAKE_MAKE_PROGRAM}")
message("CMake module path: ${CMAKE_MODULE_PATH}")

project(cppdb 
	VERSION 0.1.0
	DESCRIPTION "C++ database abstraction layer")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

if (MSVC)	
	string(REGEX REPLACE "/[wW][0-3xX]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	add_compile_options(/W4)
endif()

find_package(PostgreSQL QUIET)

add_subdirectory(tools)
add_subdirectory(cpp_db)
add_subdirectory(firebird)
enable_testing()
add_subdirectory(main)

if(WIN32)
	add_subdirectory(odbc)
endif()

if (PostgreSQL_FOUND)
	add_subdirectory(postgres)
else()
	message("PostgreSQL not found, skipping postgres package")
endif()

add_subdirectory(samples)
add_subdirectory(sqlite)

add_subdirectory(test)
if (MSVC)
	add_subdirectory(unittests)
endif()

add_custom_target(todo DEPENDS ALL SOURCES todo.txt)
