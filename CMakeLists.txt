cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

message("CMake version: ${CMAKE_VERSION} ")
message("CMake make tool: ${CMAKE_MAKE_PROGRAM}")
message("CMake module path: ${CMAKE_MODULE_PATH}")

project(cppdb VERSION 0.1.0
                  DESCRIPTION "C++ database abstraction layer")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)


add_subdirectory(tools)
add_subdirectory(cpp_db)
add_subdirectory(firebird)
add_subdirectory(main)
if(WIN32)
	add_subdirectory(odbc)
endif()
add_subdirectory(postgres)
add_subdirectory(samples)
add_subdirectory(sqlite)
add_subdirectory(test)
if (MSVC)
	add_subdirectory(unittests)
endif()

add_custom_target(todo DEPENDS ALL SOURCES todo.txt)

enable_testing()