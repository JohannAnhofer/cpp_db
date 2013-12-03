#include "connection.h"
#include "no_authentication.h"
#include "execute.h"
#include "statement.h"
#include "result.h"
#include "value.h"
#include "transaction.h"

#include <iostream>

void run_samples()
{
    using namespace cpp_db;

    try
    {
        // connect to in-memory database
        connection conn("sqlite");
        conn.open(":memory:", no_authentication{});

        std::cout << "Successfully connected!" << std::endl;

        transaction trans(conn);
        trans.begin();

        // create a table
        execute_ddl(conn, R"(
                            create table test_table (
                                    ID integer primary key,
                                    NAME varchar(50),
                                    AGE integer
                                );
                            )");

        std::cout << "test_table successfully created!" << std::endl;

        // insert 3 records
        statement stmt("insert into test_table(ID, NAME, AGE) values(?, ?, ?);", conn);
        stmt.execute_non_query(1, "Bilbo Baggins", 121);
        stmt.reset();
        stmt.execute_non_query(2, "Frodo Baggins", 33);
        stmt.reset();
        stmt.execute_non_query(3, "Samwise Gamgee", 21);
        trans.commit();
        std::cout << "Successfully inserted 3 records." << std::endl;

        // dump records
        result res(execute(conn, "select * from test_table where AGE < ?", 100));

        // dump column names
        for (int i = 0; i < res.get_column_count(); ++i)
            std::cout << res.get_column_name(i) << "\t";
        std::cout << std::endl;

        // dump data
        while(!res.is_eof())
        {
            std::cout << cast_to<std::string>(res.get_column_value("ID")) << "\t"
                      <<  value_of<std::string>(res.get_column_value(1)) << "\t"
                      << value_of<int>(res["AGE"])
                      << std::endl;

            res.move_next();
        }

        std::cout << "Cumulated ages: " << value_of<int>(execute_scalar(conn, "select sum(AGE) from test_table")) << std::endl;
    }
    catch(const std::exception &ex)
    {
        std::cerr << "EXCEPTION: " << ex.what() << std::endl;
    }
}
