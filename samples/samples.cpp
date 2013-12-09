#include "connection.h"
#include "execute.h"
#include "transaction.h"
#include "transaction_scope.h"

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
                                    AGE integer,
                                    SALARY double precision
                                );
                            )");

        std::cout << "test_table successfully created!" << std::endl;

        // insert 3 records
        statement stmt("insert into test_table(ID, NAME, AGE, SALARY) values(?, ?, ?, ?);", conn);
        stmt.execute_non_query(1, "Bilbo Baggins", 121, 1000.0);
        stmt.reset();
        stmt.execute_non_query(2, "Frodo Baggins", 33, 500.0);
        stmt.reset();
        stmt.execute_non_query(3, "Samwise Gamgee", 21, 250.0);
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
                      << value_of<int>(res["AGE"]) << "\t"
                      << value_of<double>(res[3])
                      << std::endl;

            res.move_next();
        }

        std::cout << "Cumulated salary: " << value_of<int>(execute_scalar(conn, "select sum(SALARY) from test_table where AGE between ? and ?", 10, 100)) << std::endl;

        transaction_scope trs(&trans);
        stmt.execute_non_query(4, "Gandalf the grey", 9899, 5000.0);

    }
    catch(const std::exception &ex)
    {
        std::cerr << "EXCEPTION: " << ex.what() << std::endl;
    }
}
