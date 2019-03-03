
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

target_link_libraries(postgres INTERFACE tools cpp_db)
