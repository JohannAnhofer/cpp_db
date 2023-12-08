
add_library(sqlite STATIC
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
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(SQLite3 REQUIRED)
target_include_directories(sqlite
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_link_libraries(sqlite 
    PUBLIC
        cpp_db
    PRIVATE
        tools
        SQLite::SQLite3
        ${CMAKE_DL_LIBS}
        Threads::Threads
    )
