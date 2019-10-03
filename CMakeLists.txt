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

add_subdirectory(tools)
add_subdirectory(cpp_db)

if(WIN32)
	add_subdirectory(odbc)
endif()

find_package(Firebird3 QUIET)
if (Firebird3_FOUND)
	add_subdirectory(firebird)
else()
	message("Firebird not found, skipping firebird package")
endif()

find_package(PostgreSQL QUIET)
if (PostgreSQL_FOUND)
	add_subdirectory(postgres)
else()
	message("PostgreSQL not found, skipping postgres package")
endif()

find_package(SQLite3 QUIET)
if (SQLite3_FOUND)
	add_subdirectory(sqlite)
	add_subdirectory(samples)
else()
	message("SQLite not found, skipping sqlite driver and samples")
endif()

enable_testing()
add_subdirectory(tests)

add_custom_target(additional_files DEPENDS ALL
    SOURCES
        todo.txt
        N3886.pdf
        README.md
)
