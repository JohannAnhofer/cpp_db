#include "connection.h"
#include "no_authentication.h"
#include "execute.h"

#include <iostream>

void run_samples()
{
    using namespace cpp_db;

    try
    {
        connection conn("sqlite");
        conn.open(":memory", no_authentication{});

        execute_ddl(conn, R"(create table test_table (
                                            ID integer primary key,
                                            NAME varchar(50),
                                            AGE integer
                                        );
                                    )");

        std::cout << "Successfully connected!" << std::endl;
    }
    catch(const std::exception &ex)
    {
        std::cerr << "EXCEPTION: " << ex.what() << std::endl;
    }
}
