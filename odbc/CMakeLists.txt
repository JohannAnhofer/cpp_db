
add_library(odbc STATIC
    odbc_connection.cpp
    odbc_connection.h
    odbc_driver.cpp
    odbc_driver.h
    odbc_parameters.cpp
    odbc_parameters.h
    odbc_result.cpp
    odbc_result.h
    odbc_statement.cpp
    odbc_statement.h
    odbc_transaction.cpp
    odbc_transaction.h
)

target_link_libraries(odbc PUBLIC tools cpp_db)
