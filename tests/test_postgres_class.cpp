#define BOOST_TEST_MODULE cpp_db_postgres
#define BOOST_TEST_NO_MAIN
#include <boost/test/included/unit_test.hpp>

#include "connection.h"
#include "user_password_authentication.h"
#include "transaction.h"
#include "statement.h"
#include "execute.h"
#include "transaction_scope.h"
#include "postgres_exception.h"
#include "driver_registry.h"
#include "postgres_driver.h"
#include "postgres_exception.h"

#include <cstdlib>
#include <string>

struct PostgresTestFixture
{
    PostgresTestFixture()
    {
        con = std::shared_ptr<cpp_db::connection>(new cpp_db::connection("postgres"));
        BOOST_CHECK_NO_THROW(con->open("postgres", cpp_db::user_password_authentication{"postgres", "masterkey"}, cpp_db::key_value_pair{{"host", "localhost"}} ));
    }

    ~PostgresTestFixture()
    {
        BOOST_CHECK_NO_THROW(con->close());
        BOOST_CHECK(!con->is_open());
    }

    void setup()
    {
        constexpr auto sql = R"(
                    create table if not exists test_table (
                        id int primary key,
                        name varchar(50),
                        age int
                    );
                )";

        BOOST_CHECK_NO_THROW(cpp_db::execute_ddl(*con, sql));
    }

    void teardown()
    {
        BOOST_CHECK_NO_THROW(cpp_db::execute_ddl(*con, "drop table if exists test_table;"));
    }
    std::shared_ptr<cpp_db::connection> con;
};

BOOST_FIXTURE_TEST_SUITE(test_cpp_db_postgres, PostgresTestFixture)

BOOST_AUTO_TEST_CASE(test_connection)
{
    BOOST_CHECK(con->is_open());
}

BOOST_AUTO_TEST_CASE(test_transaction)
{
    cpp_db::transaction tr(*con);
    BOOST_CHECK(!tr.is_open());
    BOOST_CHECK_NO_THROW(tr.begin());
    BOOST_CHECK(tr.is_open());
    BOOST_CHECK_NO_THROW(tr.commit());
    BOOST_CHECK(!tr.is_open());
    BOOST_CHECK_NO_THROW(tr.begin());
    BOOST_CHECK(tr.is_open());
    BOOST_CHECK_NO_THROW(tr.rollback());
    BOOST_CHECK(!tr.is_open());
}

BOOST_AUTO_TEST_CASE(test_execute)
{
    cpp_db::statement cmd("insert into test_table(id, name, age) values(1, 'dad', 46);", *con);
    BOOST_CHECK_NO_THROW(cmd.execute_non_query());
    BOOST_CHECK_NO_THROW(cmd.prepare("insert into test_table(id, name, age) values(2, 'mom', 41);"));
    BOOST_CHECK_NO_THROW(cmd.execute_non_query());

    cpp_db::transaction tr(*con);
    {
        cpp_db::transaction_scope trs(&tr);
        BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(3, 'son', 3);"));
        BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(4, 'daughter', 1);"));
    }
    {
        cpp_db::transaction_scope trs(&tr);
        BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(5, 'xxxxxx', 17);"));
        tr.rollback();
    }
}

BOOST_AUTO_TEST_CASE(test_result_single_row)
{
    {
        cpp_db::transaction tr{ *con };
        cpp_db::transaction_scope trans{ &tr };

        BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(1, 'dad', 46);"));
        BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(2, 'mom', 41);"));
        BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(3, 'son', 3);"));
        BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(4, 'daughter', 1);"));
    }

    BOOST_CHECK_EQUAL(cpp_db::value_of<std::string>(cpp_db::execute_scalar(*con, "select name from test_table order by age desc limit 1")), "dad");
}

BOOST_AUTO_TEST_CASE(test_result_properties)
{
    cpp_db::statement stmt{"select id, name, age from test_table order by age desc", *con};
    cpp_db::result res(stmt.execute());

    BOOST_CHECK(res.is_eof());
    BOOST_CHECK_EQUAL(res.get_column_count(), 3);
    BOOST_CHECK_EQUAL(res.get_column_name(1), "name");
    BOOST_CHECK_EQUAL(res.get_column_index("age"), 2);

    BOOST_CHECK_THROW(res.is_column_null("name"), cpp_db::postgres_exception);
    BOOST_CHECK_THROW(res.is_column_null(0), cpp_db::postgres_exception);
}

BOOST_AUTO_TEST_CASE(test_result_multi_rows)
{
    BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(1, 'dad', 46);"));
    BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(2, 'mom', 41);"));
    BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(3, 'son', 3);"));
    BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(4, 'daughter', 1);"));
    BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "insert into test_table(id, name, age) values(5, null, null);"));

    cpp_db::result res{cpp_db::execute(*con, "select name, age from test_table order by age desc")};

    BOOST_CHECK(!res.is_eof());
    std::string names;
    std::string ages;
    BOOST_CHECK(res.is_column_null("name"));
    BOOST_CHECK(res.is_column_null(1));
    res.move_next();
    BOOST_CHECK(!res.is_column_null("name"));
    BOOST_CHECK(!res.is_column_null(1));

    while(!res.is_eof())
    {
        names += cpp_db::value_of<std::string>(res.get_column_value("name"));
        ages += cpp_db::value_of<std::string>(res.get_column_value(1));
        res.move_next();
    }
    BOOST_CHECK_EQUAL(names, "dadmomsondaughter");
    BOOST_CHECK_EQUAL(ages, "464131");
}

BOOST_AUTO_TEST_SUITE_END()

boost::unit_test::test_suite *init_postgres_driver(int, char **)
{
    cpp_db::driver_registry::register_driver("postgres", []{return cpp_db::postgres_driver::create();});
    return nullptr;
}

int main(int argc, char* argv[])
{
    return boost::unit_test::unit_test_main( &init_postgres_driver, argc, argv );
}
