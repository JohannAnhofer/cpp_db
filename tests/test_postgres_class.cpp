#include "test_postgres_class.h"
#include "connection.h"
#include "user_password_authentication.h"
#include "transaction.h"
#include "statement.h"
#include "execute.h"
#include "transaction_scope.h"
#include "postgres_exception.h"
#include "driver_registry.h"
#include "postgres_driver.h"

#include <stdlib.h>
#include <string>

void test_postgres_class::init_class()
{
	cpp_db::driver_registry::register_driver("postgres", []{return cpp_db::postgres_driver::create();});

	con = std::shared_ptr<cpp_db::connection>(new cpp_db::connection("postgres"));
    TEST_FOR_NO_EXCEPTION(con->open("postgres", cpp_db::user_password_authentication{"postgres", "masterkey"}, cpp_db::key_value_pair{{"host", "localhost"}} ));
}

void test_postgres_class::cleanup_class()
{
    TEST_FOR_NO_EXCEPTION(con->close());
    TEST_VERIFY(!con->is_open());
}

void test_postgres_class::init()
{
    const char *sql = R"(
                create table if not exists test_table (
                    id int primary key,
                    name varchar(50),
                    age int
                );
            )";

    TEST_FOR_NO_EXCEPTION(cpp_db::execute_ddl(*con, sql));
}

void test_postgres_class::cleanup()
{
    TEST_FOR_NO_EXCEPTION(cpp_db::execute_ddl(*con, "drop table if exists test_table;"));
}

void test_postgres_class::test_connection()
{
    TEST_VERIFY(con->is_open());
}

void test_postgres_class::test_transaction()
{
    cpp_db::transaction tr(*con);
    TEST_VERIFY(!tr.is_open());
    TEST_FOR_NO_EXCEPTION(tr.begin());
    TEST_VERIFY(tr.is_open());
    TEST_FOR_NO_EXCEPTION(tr.commit());
    TEST_VERIFY(!tr.is_open());
    TEST_FOR_NO_EXCEPTION(tr.begin());
    TEST_VERIFY(tr.is_open());
    TEST_FOR_NO_EXCEPTION(tr.rollback());
    TEST_VERIFY(!tr.is_open());
}

void test_postgres_class::test_execute()
{
    cpp_db::statement cmd("insert into test_table(id, name, age) values(1, 'dad', 46);", *con);
    TEST_FOR_NO_EXCEPTION(cmd.execute_non_query());
    TEST_FOR_NO_EXCEPTION(cmd.prepare("insert into test_table(id, name, age) values(2, 'mom', 41);"));
    TEST_FOR_NO_EXCEPTION(cmd.execute_non_query());

    cpp_db::transaction tr(*con);
    {
        cpp_db::transaction_scope trs(&tr);
        TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(3, 'son', 3);"));
        TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(4, 'daughter', 1);"));
    }
    {
        cpp_db::transaction_scope trs(&tr);
        TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(5, 'xxxxxx', 17);"));
        tr.rollback();
    }
}

void test_postgres_class::test_result_single_row()
{
	{
		cpp_db::transaction tr{ *con };
		cpp_db::transaction_scope trans{ &tr };

		TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(1, 'dad', 46);"));
		TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(2, 'mom', 41);"));
		TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(3, 'son', 3);"));
		TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(4, 'daughter', 1);"));
	}

    TEST_EQUAL(cpp_db::value_of<std::string>(cpp_db::execute_scalar(*con, "select name from test_table order by age desc limit 1")), "dad");
}

void test_postgres_class::test_result_properties()
{
    cpp_db::statement stmt{"select id, name, age from test_table order by age desc", *con};
    cpp_db::result res(stmt.execute());

    TEST_VERIFY(res.is_eof());
    TEST_EQUAL(res.get_column_count(), 3);
    TEST_EQUAL(res.get_column_name(1), "name");
    TEST_EQUAL(res.get_column_index("age"), 2);

    TEST_FOR_EXCEPTION(res.is_column_null("name"), cpp_db::postgres_exception);
    TEST_FOR_EXCEPTION(res.is_column_null(0), cpp_db::postgres_exception);
}

void test_postgres_class::test_result_multi_rows()
{
    TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(1, 'dad', 46);"));
    TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(2, 'mom', 41);"));
    TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(3, 'son', 3);"));
    TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(4, 'daughter', 1);"));
    TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(5, null, null);"));

    cpp_db::result res{cpp_db::execute(*con, "select name, age from test_table order by age desc")};

    TEST_VERIFY(!res.is_eof());
    std::string names;
    std::string ages;
    TEST_VERIFY(res.is_column_null("name"));
    TEST_VERIFY(res.is_column_null(1));
    res.move_next();
    TEST_VERIFY(!res.is_column_null("name"));
    TEST_VERIFY(!res.is_column_null(1));

    while(!res.is_eof())
    {
        names += cpp_db::value_of<std::string>(res.get_column_value("name"));
        ages += cpp_db::value_of<std::string>(res.get_column_value(1));
        res.move_next();
    }
    TEST_EQUAL(names, "dadmomsondaughter");
    TEST_EQUAL(ages, "464131");
}
