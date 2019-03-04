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

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(cppdb PRIVATE
    tools
    test
    postgres
    firebird
    sqlite
    cpp_db
    pq
    Threads::Threads
    ${CMAKE_DL_LIBS}
    )
