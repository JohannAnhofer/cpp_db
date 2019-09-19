cmake_minimum_required(VERSION 3.10)
message("CMake version: ${CMAKE_VERSION} ")
message("CMake make tool: ${CMAKE_MAKE_PROGRAM}")
project(cppdb VERSION 0.1.0
                  DESCRIPTION "C++ database abstraction layer")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules")

message("CMake module path: ${CMAKE_MODULE_PATH}")

add_subdirectory(tools)
add_subdirectory(cpp_db)
add_subdirectory(firebird)
add_subdirectory(main)
#add_subdirectory(odbc)
add_subdirectory(postgres)
add_subdirectory(samples)
add_subdirectory(sqlite)
add_subdirectory(test)
#add_subdirectory(unittests)

add_custom_target(todo DEPENDS ALL SOURCES todo.txt)
