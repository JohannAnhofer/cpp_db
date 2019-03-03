project(cpp_db)

add_library(cpp_db STATIC
    authentication.h
    connection.cpp
    connection.h
    connection_interface.h
    db_exception.h
    driver_factory.cpp
    driver_factory.h
    driver_interface.h
    driver_registry.cpp
    driver_registry.h
    execute.cpp
    execute.h
    no_authentication.h
    null.h
    parameters.cpp
    parameters.h
    parameters_interface.h
    result.cpp
    result.h
    result_interface.h
    statement.cpp
    statement.h
    statement.inl
    statement_interface.h
    transaction.cpp
    transaction.h
    transaction_interface.h
    transaction_scope.h
    type_mismatch.cpp
    type_mismatch.h
    type_of.h
    user_password_authentication.h
    usings.h
    value.h
    value.inl
    value_is_null.h
    value_of.h
    )

target_link_libraries(cpp_db INTERFACES tools)
