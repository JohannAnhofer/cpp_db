
find_package(PostgreSQL REQUIRED)

add_library(postgres STATIC
    postgres_connection.cpp
    postgres_connection.h
    postgres_driver.cpp
    postgres_driver.h
    postgres_exception.cpp
    postgres_exception.h
    postgres_parameters.cpp
    postgres_parameters.h
    postgres_result.cpp
    postgres_result.h
    postgres_statement.cpp
    postgres_statement.h
    postgres_transaction.cpp
    postgres_transaction.h
)

target_link_libraries(postgres PUBLIC tools cpp_db)
target_link_libraries(postgres PRIVATE ${PostgreSQL_LIBRARY})
target_include_directories(postgres PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE ${PostgreSQL_INCLUDE_DIR})


