#include "connection.h"
#include "no_authentication.h"
#include "execute.h"

#include <iostream>

void run_samples()
{
    try
    {
        cpp_db::connection conn("sqlite");
        conn.open(":memory", cpp_db::no_authentication{});

        cpp_db::execute_ddl(conn, R"(create table test_table (
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
