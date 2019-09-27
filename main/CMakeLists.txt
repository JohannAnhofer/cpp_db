find_package(PostgreSQL QUIET)
if (PostgreSQL_FOUND)
add_executable(cppdb_postgres 
	main_pg.cpp     
	test_postgres_class.cpp
    test_postgres_class.h
)
target_link_libraries(cppdb_postgres PRIVATE
    tools
    testframework
    postgres
    cpp_db
    )

add_test(NAME cppdb_postgres COMMAND cppdb_postgres)

else()
	message("PostgreSQL not found, skipping postgresql tests.")
endif()

add_executable(cppdb
    main.cpp
    test_cpp_db_class.cpp
    test_cpp_db_class.h
    test_firebird_class.cpp
    test_firebird_class.h
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
    firebird
    sqlite
    cpp_db
    )

add_test(NAME cppdb COMMAND cppdb)
