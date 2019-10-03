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

	add_test(NAME tst_cppdb_postgres COMMAND cppdb_postgres)

else()
	message("PostgreSQL not found, skipping postgresql tests.")
endif()

find_package(Firebird3 QUIET)

if (Firebird3_FOUND)

	add_executable(cppdb_firebird
		main_firebird.cpp     
		test_firebird_class.cpp
		test_firebird_class.h
	)
	target_link_libraries(cppdb_firebird 
		PRIVATE
			tools
			testframework
			firebird
			cpp_db
	)

	add_test(NAME tst_cppdb_firebird COMMAND cppdb_firebird)

else()
	message("Firebird not found, skipping firebird tests.")
endif()

find_package(SQLite3 QUIET)

if (SQLite3_FOUND)
	add_executable(cppdb_sqlite
		main_sqlite.cpp     
		test_sqlite_class.cpp
		test_sqlite_class.h
	)
	target_link_libraries(cppdb_sqlite PRIVATE
		tools
		testframework
		sqlite
		cpp_db
	)

	add_test(NAME tst_cppdb_sqlite COMMAND cppdb_sqlite)
else()
	message("SQLite not found, skipping sqlite tests.")
endif()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(cppdb
    test_cpp_db_class.cpp
    test_tools_class.cpp
    test_test_class.cpp
)

target_include_directories(cppdb PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(cppdb
    PRIVATE
        tools
        testframework
        cpp_db
        Boost::unit_test_framework
)

add_test(NAME tst_cppdb COMMAND cppdb)
