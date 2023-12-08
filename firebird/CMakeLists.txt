
add_library(firebird STATIC
    firebird_connection.cpp
    firebird_connection.h
    firebird_driver.cpp
    firebird_driver.h
    firebird_exception.cpp
    firebird_exception.h
    firebird_parameters.cpp
    firebird_parameters.h
    firebird_result.cpp
    firebird_result.h
    firebird_statement.cpp
    firebird_statement.h
    firebird_transaction.cpp
    firebird_transaction.h
    isc_status.h
    xsqlda.cpp
    xsqlda.h
    xsqlvar.cpp
    xsqlvar.h
)

find_package(Firebird3 REQUIRED)
target_include_directories(firebird 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(firebird 
    PUBLIC
        cpp_db
    PRIVATE
        tools
        Firebird::Firebird3
)
