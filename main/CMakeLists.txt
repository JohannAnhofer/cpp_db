add_executable(cppdb
    main.cpp
    test_cpp_db_class.cpp
    test_cpp_db_class.h
    test_firebird_class.cpp
    test_firebird_class.h
    test_postgres_class.cpp
    test_postgres_class.h
    test_sqlite_class.cpp
    test_sqlite_class.h
    test_test_class.cpp
    test_test_class.h
    test_tools_class.cpp
    test_tools_class.h
)

target_link_libraries(cppdb PRIVATE
    tools
    testframework
    postgres
    firebird
    sqlite
    cpp_db
    )

add_test(NAME cppdb COMMAND cppdb)
