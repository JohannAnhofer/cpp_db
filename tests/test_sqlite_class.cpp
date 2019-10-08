#define BOOST_TEST_MODULE cpp_db_sqlite
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>

#include "connection.h"
#include "sqlite_driver.h"
#include "statement.h"
#include "result.h"
#include "db_exception.h"
#include "transaction.h"
#include "execute.h"
#include "driver_registry.h"
#include "sqlite_driver.h"

#include <cmath>

struct SQLiteTestFixture
{
	SQLiteTestFixture()
	{
		con = std::shared_ptr<cpp_db::connection>(new cpp_db::connection("sqlite"));
        BOOST_CHECK_NO_THROW(con->open(":memory:", cpp_db::no_authentication{}));
    }

    void setup()
    {
        BOOST_CHECK_NO_THROW(cpp_db::statement("create table TEST_TABLE(COL1 INT NOT NULL, COL2 VARCHAR(50));", *con).execute_non_query());
        BOOST_CHECK_NO_THROW(cpp_db::statement("insert into TEST_TABLE(COL1, COL2) VALUES(1, 'first')", *con).execute_non_query());
        BOOST_CHECK_NO_THROW(cpp_db::statement("insert into TEST_TABLE(COL1, COL2) VALUES(2, 'second')", *con).execute_non_query());
        BOOST_CHECK_NO_THROW(cpp_db::statement("insert into TEST_TABLE(COL1, COL2) VALUES(3, 'third')", *con).execute_non_query());
    }

    void teardown()
    {
        BOOST_CHECK_NO_THROW(cpp_db::statement("drop table TEST_TABLE;", *con).execute_non_query());
    }

	~SQLiteTestFixture()
	{
		con.reset();
	}

	std::shared_ptr<cpp_db::connection> con;
};

BOOST_FIXTURE_TEST_SUITE(test_cpp_db_sqlite, SQLiteTestFixture)

BOOST_AUTO_TEST_CASE(test_connection)
{
    BOOST_CHECK_THROW(cpp_db::statement("select COL1, COL2, COL3 from TEST_TABLE;", *con).execute_non_query(), cpp_db::db_exception);
}

BOOST_AUTO_TEST_CASE(test_statement)
{
    cpp_db::statement stmt(*con);
    BOOST_CHECK_NO_THROW(stmt.prepare("insert into TEST_TABLE(COL1, COL2) VALUES(?, ?)"));
	BOOST_CHECK_THROW(stmt.execute_non_query(), cpp_db::db_exception);
}

BOOST_AUTO_TEST_CASE(test_result)
{
    cpp_db::result r(cpp_db::statement("select COL1, COL2 from TEST_TABLE;", *con).execute());
    BOOST_CHECK_EQUAL(r.get_column_count(), 2);
    BOOST_CHECK_EQUAL(r.get_column_name(0), "COL1");
    BOOST_CHECK_EQUAL(r.get_column_name(1), "COL2");
    BOOST_CHECK_EQUAL(r.get_column_index("COL1"), 0);
    BOOST_CHECK_EQUAL(r.get_column_index("COL2"), 1);
    BOOST_CHECK(!r.is_eof());
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(r.get_column_value(0)), 1);
    BOOST_CHECK_EQUAL(cpp_db::value_of<std::string>(r.get_column_value(1)), "first");
    BOOST_CHECK_NO_THROW(r.move_next());
    BOOST_CHECK(!r.is_eof());
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(r.get_column_value("COL1")), 2);
    BOOST_CHECK_EQUAL(cpp_db::value_of<std::string>(r.get_column_value("COL2")), "second");
    BOOST_CHECK_NO_THROW(r.move_next());
    BOOST_CHECK(!r.is_eof());
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(r.get_column_value(0)), 3);
    BOOST_CHECK_EQUAL(cpp_db::value_of<std::string>(r.get_column_value(1)), "third");
    BOOST_CHECK_NO_THROW(r.move_next());
    BOOST_CHECK(r.is_eof());

    BOOST_CHECK_THROW(cpp_db::result(cpp_db::statement(*con).execute()), cpp_db::db_exception);
}
BOOST_AUTO_TEST_CASE(test_parameter)
{
    cpp_db::statement stmt(*con);
    BOOST_CHECK_NO_THROW(stmt.prepare("insert into TEST_TABLE(COL1, COL2) VALUES(?, ?)"));
    BOOST_CHECK_NO_THROW(stmt.execute_non_query(4, "four"));
    BOOST_CHECK_NO_THROW(stmt.execute_non_query(5, "five"));
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE where (COL1 = 4 and COL2 = 'four') OR (COL1 = 5 and COL2 = 'five')", *con).execute_scalar()), 2);

    BOOST_CHECK_NO_THROW(stmt.prepare("insert into TEST_TABLE(COL1, COL2) VALUES(99, 'Unknown')"));
    BOOST_CHECK_NO_THROW(stmt.execute_non_query());
    BOOST_CHECK_NO_THROW(stmt.execute_non_query());

    BOOST_CHECK_NO_THROW(stmt.execute_non_query());

    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE where COL1=99 and COL2='Unknown'", *con).execute_scalar()), 3);

    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE where COL1=? and COL2=?", *con).execute_scalar(99, "Unknown")), 3);
}

BOOST_AUTO_TEST_CASE(test_named_parameter)
{
    BOOST_CHECK_NO_THROW(cpp_db::statement("insert into TEST_TABLE(COL1, COL2) VALUES(99, 'Unknown')", *con).execute_non_query());
    BOOST_CHECK_NO_THROW(cpp_db::statement("insert into TEST_TABLE(COL1, COL2) VALUES(99, 'Unknown')", *con).execute_non_query());
    BOOST_CHECK_NO_THROW(cpp_db::statement("insert into TEST_TABLE(COL1, COL2) VALUES(99, 'Unknown')", *con).execute_non_query());

    cpp_db::statement stmt("select count(*) from TEST_TABLE where COL1=@COL1 and COL2=@COL2", *con);
    cpp_db::parameters params(stmt.get_parameters());
    params.bind(cpp_db::parameter("@COL1", 99));
    params.bind(cpp_db::parameter("@COL2", "Unknown"));
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(stmt.execute_scalar()), 3);
}

BOOST_AUTO_TEST_CASE(test_transaction_commit)
{
    cpp_db::transaction tran(*con);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE", *con).execute_scalar()), 3);

    BOOST_CHECK_NO_THROW(tran.begin());
    cpp_db::statement stmt(*con);
    BOOST_CHECK_NO_THROW(stmt.prepare("delete from TEST_TABLE where COL1 = ?"));
    BOOST_CHECK_NO_THROW(stmt.execute_non_query(2));
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE", *con).execute_scalar()), 2);
    BOOST_CHECK_NO_THROW(tran.commit());
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE", *con).execute_scalar()), 2);
}

BOOST_AUTO_TEST_CASE(test_transaction_rollback)
{
    cpp_db::transaction tran(*con);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE", *con).execute_scalar()), 3);

    BOOST_CHECK_NO_THROW(tran.begin());
    cpp_db::statement stmt(*con);
    BOOST_CHECK_NO_THROW(stmt.prepare("delete from TEST_TABLE where COL1 > ?"));
    BOOST_CHECK_NO_THROW(stmt.execute_non_query(2));
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE", *con).execute_scalar()), 2);
    BOOST_CHECK_NO_THROW(tran.rollback());
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE", *con).execute_scalar()), 3);
}

BOOST_AUTO_TEST_CASE(test_execute)
{
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(cpp_db::execute_scalar(*con, "select count(*) from TEST_TABLE")), 3);
    cpp_db::result r = cpp_db::execute(*con, "select * from TEST_TABLE where COL1 in (?, ?)", 1, 2);
    int64_t sum = 0;
    std::string names;
    while(!r.is_eof())
    {
        sum += cpp_db::value_of<int64_t>(r.get_column_value(0));
        names.append(cpp_db::value_of<std::string>(r.get_column_value(1)));
        r.move_next();
    }
    BOOST_CHECK_EQUAL(sum, 3);
    BOOST_CHECK_EQUAL(names, "firstsecond");

    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(cpp_db::execute_scalar(*con, "select count(*) from TEST_TABLE where COL1 in (?, ?) and COL2 in (?, ?)", 1, 2, "first", "second")), 2);
    BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "delete from TEST_TABLE where COL1 = ?", 1));
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(cpp_db::execute_scalar(*con, "select count(*) from TEST_TABLE")), 2);
    BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "delete from TEST_TABLE"));
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(cpp_db::execute_scalar(*con, "select count(*) from TEST_TABLE")), 0);
}

BOOST_AUTO_TEST_CASE(test_result_emptys)
{
    cpp_db::result r(cpp_db::statement("select COL1, COL2 from TEST_TABLE where col1 = ?", *con).execute("empty result"));
    BOOST_CHECK(r.is_eof());

}

BOOST_AUTO_TEST_SUITE_END()

bool init_sqlite_driver()
{
	cpp_db::driver_registry::register_driver("sqlite", [] {return cpp_db::sqlite_driver::create(); });
	return true;
}

int main(int argc, char* argv[])
{
	return boost::unit_test::unit_test_main(&init_sqlite_driver, argc, argv);
}
