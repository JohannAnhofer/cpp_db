#include "test_sqlite_class.h"
#include "sqlite_driver.h"
#include "statement.h"
#include "result.h"
#include "db_exception.h"
#include "transaction.h"
#include "execute.h"

#include <cmath>

void test_sqlite_class::init_class()
{
    con = std::shared_ptr<cpp_db::connection>(new cpp_db::connection("sqlite"));
    TEST_FOR_NO_EXCEPTION(con->open(":memory:", cpp_db::no_authentication{}));
}

void test_sqlite_class::cleanup_class()
{
    con.reset();
}

void test_sqlite_class::test_connection()
{
    TEST_FOR_NO_EXCEPTION(cpp_db::statement("create table TEST_TABLE(COL1 INT NOT NULL, COL2 VARCHAR(50));", *con.get()).execute_non_query());
    TEST_FOR_NO_EXCEPTION(cpp_db::statement("insert into TEST_TABLE(COL1, COL2) VALUES(1, 'first')", *con.get()).execute_non_query());
    TEST_FOR_NO_EXCEPTION(cpp_db::statement("insert into TEST_TABLE(COL1, COL2) VALUES(2, 'second')", *con.get()).execute_non_query());
    TEST_FOR_NO_EXCEPTION(cpp_db::statement("insert into TEST_TABLE(COL1, COL2) VALUES(3, 'third')", *con.get()).execute_non_query());
    TEST_FOR_EXCEPTION(cpp_db::statement("select COL1, COL2, COL3 from TEST_TABLE;", *con.get()).execute_non_query(), cpp_db::db_exception);
}

void test_sqlite_class::test_statement()
{
    cpp_db::statement stmt(*con.get());
    TEST_FOR_NO_EXCEPTION(stmt.prepare("insert into TEST_TABLE(COL1, COL2) VALUES(?, ?)"));
    TEST_FOR_EXCEPTION(stmt.execute_non_query(), cpp_db::db_exception);
}

void test_sqlite_class::test_result()
{
    cpp_db::result r(cpp_db::statement("select COL1, COL2 from TEST_TABLE;", *con.get()).execute());
    TEST_EQUAL(r.get_column_count(), 2);
    TEST_EQUAL(r.get_column_name(0), "COL1");
    TEST_EQUAL(r.get_column_name(1), "COL2");
    TEST_EQUAL(r.get_column_index("COL1"), 0);
    TEST_EQUAL(r.get_column_index("COL2"), 1);
    TEST_VERIFY(!r.is_eof());
    TEST_EQUAL(cpp_db::value_of<int64_t>(r.get_column_value(0)), 1);
    TEST_EQUAL(cpp_db::value_of<std::string>(r.get_column_value(1)), "first");
    TEST_FOR_NO_EXCEPTION(r.move_next());
    TEST_VERIFY(!r.is_eof());
    TEST_EQUAL(cpp_db::value_of<int64_t>(r.get_column_value("COL1")), 2);
    TEST_EQUAL(cpp_db::value_of<std::string>(r.get_column_value("COL2")), "second");
    TEST_FOR_NO_EXCEPTION(r.move_next());
    TEST_VERIFY(!r.is_eof());
    TEST_EQUAL(cpp_db::value_of<int64_t>(r.get_column_value(0)), 3);
    TEST_EQUAL(cpp_db::value_of<std::string>(r.get_column_value(1)), "third");
    TEST_FOR_NO_EXCEPTION(r.move_next());
    TEST_VERIFY(r.is_eof());

    TEST_FOR_EXCEPTION(cpp_db::result(cpp_db::statement(*con.get()).execute()), cpp_db::db_exception);
}
void test_sqlite_class::test_parameter()
{
    cpp_db::statement stmt(*con.get());
    TEST_FOR_NO_EXCEPTION(stmt.prepare("insert into TEST_TABLE(COL1, COL2) VALUES(?, ?)"));
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query(4, "four"));
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query(5, "five"));
    TEST_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE where (COL1 = 4 and COL2 = 'four') OR (COL1 = 5 and COL2 = 'five')", *con.get()).execute_scalar()), 2);

    TEST_FOR_NO_EXCEPTION(stmt.prepare("insert into TEST_TABLE(COL1, COL2) VALUES(99, 'Unknown')"));
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query());
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query());
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query());

    TEST_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE where COL1=99 and COL2='Unknown'", *con.get()).execute_scalar()), 3);

    TEST_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE where COL1=? and COL2=?", *con.get()).execute_scalar(99, "Unknown")), 3);
}

void test_sqlite_class::test_named_parameter()
{
    cpp_db::statement stmt("select count(*) from TEST_TABLE where COL1=@COL1 and COL2=@COL2", *con.get());
    cpp_db::parameters params(stmt.get_parameters());
    params.bind(cpp_db::parameter("@COL1", 99));
    params.bind(cpp_db::parameter("@COL2", "Unknown"));
    TEST_EQUAL(cpp_db::value_of<int64_t>(stmt.execute_scalar()), 3);
}

void test_sqlite_class::test_transaction_commit()
{
    cpp_db::transaction tran(*con.get());
    TEST_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE", *con.get()).execute_scalar()), 8);

    TEST_FOR_NO_EXCEPTION(tran.begin());
    cpp_db::statement stmt(*con.get());
    TEST_FOR_NO_EXCEPTION(stmt.prepare("delete from TEST_TABLE where COL1 = ?"));
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query(99));
    TEST_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE", *con.get()).execute_scalar()), 5);
    TEST_FOR_NO_EXCEPTION(tran.commit());
    TEST_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE", *con.get()).execute_scalar()), 5);
}

void test_sqlite_class::test_transaction_rollback()
{
    cpp_db::transaction tran(*con.get());
    TEST_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE", *con.get()).execute_scalar()), 5);

    TEST_FOR_NO_EXCEPTION(tran.begin());
    cpp_db::statement stmt(*con.get());
    TEST_FOR_NO_EXCEPTION(stmt.prepare("delete from TEST_TABLE where COL1 > ?"));
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query(3));
    TEST_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE", *con.get()).execute_scalar()), 3);
    TEST_FOR_NO_EXCEPTION(tran.rollback());
    TEST_EQUAL(cpp_db::value_of<int64_t>(cpp_db::statement("select count(*) from TEST_TABLE", *con.get()).execute_scalar()), 5);
}

void test_sqlite_class::test_execute()
{
    TEST_EQUAL(cpp_db::value_of<int64_t>(cpp_db::execute_scalar(*con.get(), "select count(*) from TEST_TABLE")), 5);
    cpp_db::result r = cpp_db::execute(*con.get(), "select * from TEST_TABLE where COL1 in (?, ?)", 1, 2);
    int64_t sum = 0;
    std::string names;
    while(!r.is_eof())
    {
        sum += cpp_db::value_of<int64_t>(r.get_column_value(0));
        names.append(cpp_db::value_of<std::string>(r.get_column_value(1)));
        r.move_next();
    }
    TEST_EQUAL(sum, 3);
    TEST_EQUAL(names, "firstsecond");

    TEST_EQUAL(cpp_db::value_of<int64_t>(cpp_db::execute_scalar(*con.get(), "select count(*) from TEST_TABLE where COL1 in (?, ?) and COL2 in (?, ?)", 1, 2, "first", "second")), 2);
    TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con.get(), "delete from TEST_TABLE where COL1 = ?", 1));
    TEST_EQUAL(cpp_db::value_of<int64_t>(cpp_db::execute_scalar(*con.get(), "select count(*) from TEST_TABLE")), 4);
    TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con.get(), "delete from TEST_TABLE"));
    TEST_EQUAL(cpp_db::value_of<int64_t>(cpp_db::execute_scalar(*con.get(), "select count(*) from TEST_TABLE")), 0);
}

void test_sqlite_class::test_result_empty()
{
    cpp_db::result r(cpp_db::statement("select COL1, COL2 from TEST_TABLE where col1 = ?", *con.get()).execute("empty result"));
    TEST_VERIFY(r.is_eof());

}
