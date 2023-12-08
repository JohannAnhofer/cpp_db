find_package(Boost COMPONENTS unit_test_framework REQUIRED)

find_package(PostgreSQL QUIET)
if (PostgreSQL_FOUND)

    add_executable(cppdb_postgres
        test_postgres_class.cpp
    )
    target_link_libraries(cppdb_postgres
        PRIVATE
            tools
            postgres
            cpp_db
            Boost::unit_test_framework
    )

    add_test(NAME tst_cppdb_postgres COMMAND cppdb_postgres)

else()
    message("PostgreSQL not found, skipping postgresql tests.")
endif()

find_package(Firebird3 QUIET)
if (Firebird3_FOUND)

    add_executable(cppdb_firebird
        test_firebird_class.cpp
    )
    target_link_libraries(cppdb_firebird
        PRIVATE
            tools
            firebird
            cpp_db
            Boost::unit_test_framework
    )

    add_test(NAME tst_cppdb_firebird COMMAND cppdb_firebird)

else()
    message("Firebird not found, skipping firebird tests.")
endif()

find_package(SQLite3 QUIET)
if (SQLite3_FOUND)
    add_executable(cppdb_sqlite
        test_sqlite_class.cpp
    )
    target_link_libraries(cppdb_sqlite
        PRIVATE
        tools
        sqlite
        cpp_db
        Boost::unit_test_framework
    )

    add_test(NAME tst_cppdb_sqlite COMMAND cppdb_sqlite)

else()
    message("SQLite not found, skipping sqlite tests.")
endif()

add_executable(cppdb
    test_cpp_db_class.cpp
)
target_link_libraries(cppdb
    PRIVATE
        tools
        cpp_db
        Boost::unit_test_framework
)
add_test(NAME tst_cppdb COMMAND cppdb)

add_executable(cppdb_test
    test_test_class.cpp
)
target_link_libraries(cppdb_test
    PRIVATE
        tools
        cpp_db
        Boost::unit_test_framework
)
add_test(NAME cppdb_test COMMAND cppdb)

add_executable(cppdb_tools
    test_tools_class.cpp
)
target_link_libraries(cppdb_tools
    PRIVATE
        tools
        cpp_db
        Boost::unit_test_framework
)
add_test(NAME tst_cppdb_tools COMMAND cppdb_tools)
