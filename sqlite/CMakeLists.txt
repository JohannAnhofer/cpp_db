
add_library(sqlite STATIC
    sqlite3.c
    sqlite3.h
    sqlite3ext.h
    sqlite_connection.cpp
    sqlite_connection.h
    sqlite_driver.cpp
    sqlite_driver.h
    sqlite_exception.cpp
    sqlite_exception.h
    sqlite_parameters.cpp
    sqlite_parameters.h
    sqlite_result.cpp
    sqlite_result.h
    sqlite_statement.cpp
    sqlite_statement.h
    sqlite_transaction.cpp
    sqlite_transaction.h
    #    shell.c
)

target_link_libraries(sqlite PUBLIC tools cpp_db)
target_include_directories(sqlite PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
