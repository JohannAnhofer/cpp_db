#include "test_cpp_db_class.h"

#include "db_exception.h"
#include "statement.h"
#include "result.h"
#include "parameter.h"
#include "null.h"
#include "coalesce.h"
#include "value.h"
#include "transaction.h"
#include "execute.h"

#include <cmath>

/*
std::ostream &operator<<(std::ostream &output, cpp_db::result &r)
{
    r.move_first();
	output << "\t";
    for (int i = 0; i < r.get_column_count(); ++i)
        output << r.get_column_name(i) << "\t|\t";
	output << "\n" << std::string(80, '-') << "\n";

    while (!r.is_eof())
	{
		output << "\t";
        for (int i = 0; i < r.get_column_count(); ++i)
            output << r.get_column_value(i) << "\t|\t";
		output << "\n";
        r.move_next();
	}
	return output;
}
*/

void test_cpp_db_class::init_class()
{
    con.reset(new cpp_db::connection("sqlite"));
    TEST_FOR_NO_EXCPTION(con->open(":memory:"));
}

void test_cpp_db_class::cleanup_class()
{
    con.reset();
}

void test_cpp_db_class::test_coalesce()
{
	cpp_db::coalesce_trait<int, double, float>::type x0(0);
	cpp_db::coalesce_trait<cpp_db::null_type, double, float>::type x1(0.0);
	cpp_db::coalesce_trait<int, cpp_db::null_type, float>::type x2(0);
	cpp_db::coalesce_trait<cpp_db::null_type, cpp_db::null_type, float>::type x3(0.0f);
	cpp_db::coalesce_trait<int, double, cpp_db::null_type>::type x4(0);
	cpp_db::coalesce_trait<cpp_db::null_type, double, cpp_db::null_type>::type x5(0.0f);
	cpp_db::coalesce_trait<int, cpp_db::null_type, cpp_db::null_type>::type x6(0);
	cpp_db::coalesce_trait<cpp_db::null_type, cpp_db::null_type, cpp_db::null_type>::type x7;

	cpp_db::null_type null;

	TEST_EQUAL(cpp_db::coalesce(1, 2, 3, 4), 1);
	TEST_EQUAL(cpp_db::coalesce(cpp_db::null_type(), 2, 3, 4), 2);
	TEST_VERIFY(cpp_db::is_null(cpp_db::coalesce(null, null, null)));
	TEST_EQUAL(cpp_db::coalesce(1, null, null, null), 1);
	TEST_EQUAL(cpp_db::coalesce(null, 2, null, 4), 2);

	TEST_EQUAL(x0, 0);
	TEST_EQUAL(x1, 0.0);
	TEST_EQUAL(x2, 0);
	TEST_EQUAL(x3, 0.0f);
	TEST_EQUAL(x4, 0);
	TEST_EQUAL(x5, 0.0f);
	TEST_EQUAL(x6, 0);
	TEST_VERIFY(is_null(x7));
}

void test_cpp_db_class::test_is_null()
{
	cpp_db::null_type null;

	TEST_VERIFY(cpp_db::is_null(null));
	TEST_VERIFY(!cpp_db::is_null(1));

	cpp_db::value vnull(null);
	TEST_VERIFY(is_null(vnull));

	cpp_db::parameter pnull(0, null);
	TEST_VERIFY(is_null(pnull));
}

void test_cpp_db_class::test_connection()
{
    TEST_FOR_NO_EXCPTION(cpp_db::statement("create table TEST_TABLE(COL1 INT NOT NULL, COL2 VARCHAR(50));", *con.get()).execute_non_query());
    TEST_FOR_NO_EXCPTION(cpp_db::statement("insert into TEST_TABLE(COL1, COL2) VALUES(1, 'first')", *con.get()).execute_non_query());
    TEST_FOR_NO_EXCPTION(cpp_db::statement("insert into TEST_TABLE(COL1, COL2) VALUES(2, 'second')", *con.get()).execute_non_query());
    TEST_FOR_NO_EXCPTION(cpp_db::statement("insert into TEST_TABLE(COL1, COL2) VALUES(3, 'third')", *con.get()).execute_non_query());
    TEST_FOR_EXCEPTION(cpp_db::statement("select COL1, COL2, COL3 from TEST_TABLE;", *con.get()).execute_non_query(), cpp_db::db_exception);
}

void test_cpp_db_class::test_statement()
{
    cpp_db::statement stmt(*con.get());
    TEST_FOR_NO_EXCPTION(stmt.prepare("insert into TEST_TABLE(COL1, COL2) VALUES(?, ?)"));
    TEST_FOR_EXCEPTION(stmt.execute_non_query(), cpp_db::db_exception);
}

void test_cpp_db_class::test_result()
{
    cpp_db::result r(cpp_db::statement("select COL1, COL2 from TEST_TABLE;", *con.get()).execute());
    TEST_EQUAL(r.get_column_count(), 2);
    TEST_EQUAL(r.get_column_name(0), "COL1");
    TEST_EQUAL(r.get_column_name(1), "COL2");
    TEST_EQUAL(r.get_column_index("COL1"), 0);
    TEST_EQUAL(r.get_column_index("COL2"), 1);
    TEST_VERIFY(!r.is_eof());
    TEST_EQUAL(r.get_column_value(0).get_value<int64_t>(), 1);
    TEST_EQUAL(r.get_column_value(1).get_value<std::string>(), "first");
    TEST_FOR_NO_EXCPTION(r.move_next());
    TEST_VERIFY(!r.is_eof());
    TEST_EQUAL(r.get_column_value("COL1").get_value<int64_t>(), 2);
    TEST_EQUAL(r.get_column_value("COL2").get_value<std::string>(), "second");
    TEST_FOR_NO_EXCPTION(r.move_next());
    TEST_VERIFY(!r.is_eof());
    TEST_EQUAL(r.get_column_value(0).get_value<int64_t>(), 3);
    TEST_EQUAL(r.get_column_value(1).get_value<std::string>(), "third");
    TEST_FOR_NO_EXCPTION(r.move_next());
    TEST_VERIFY(r.is_eof());
    TEST_FOR_NO_EXCPTION(r.move_first());
    TEST_EQUAL(r.get_column_value(0).get_value<int64_t>(), 1);
    TEST_EQUAL(r.get_column_value(1).get_value<std::string>(), "first");

    TEST_FOR_EXCEPTION(cpp_db::result(cpp_db::statement(*con.get()).execute()), cpp_db::db_exception);
}

void test_cpp_db_class::test_parameter()
{
    cpp_db::parameter param1(0, std::string("Hello world!")), param2(std::string("@test"), 27.85);

    TEST_VERIFY(param1.has_index());
    TEST_VERIFY(!param1.has_name());
    TEST_VERIFY(!param2.has_index());
    TEST_VERIFY(param2.has_name());
    TEST_VERIFY(param1.has_value_of_type<std::string>());
    TEST_VERIFY(!param1.has_value_of_type<double>());
    TEST_VERIFY(!param2.has_value_of_type<std::string>());
    TEST_VERIFY(param2.has_value_of_type<double>());

    TEST_EQUAL(param1.get_value<std::string>(), "Hello world!");
    TEST_VERIFY(std::fabs(param2.get_value<double>() - 27.85) < 0.000001);

    TEST_FOR_EXCEPTION(param1.get_value<int>(), cpp_db::db_exception);

    cpp_db::statement stmt(*con.get());
    TEST_FOR_NO_EXCPTION(stmt.prepare("insert into TEST_TABLE(COL1, COL2) VALUES(?, ?)"));
	TEST_FOR_NO_EXCPTION(stmt.execute_non_query(4, "four"));
	TEST_FOR_NO_EXCPTION(stmt.execute_non_query(5, "five"));
	TEST_EQUAL(cpp_db::statement("select count(*) from TEST_TABLE where (COL1 = 4 and COL2 = 'four') OR (COL1 = 5 and COL2 = 'five')", *con.get()).execute_scalar().get_value<int64_t>(), 2);

	TEST_FOR_NO_EXCPTION(stmt.prepare("insert into TEST_TABLE(COL1, COL2) VALUES(99, 'Unknown')"));
	TEST_FOR_NO_EXCPTION(stmt.execute_non_query());
	TEST_FOR_NO_EXCPTION(stmt.execute_non_query());
	TEST_FOR_NO_EXCPTION(stmt.execute_non_query());

	TEST_EQUAL(cpp_db::statement("select count(*) from TEST_TABLE where COL1=99 and COL2='Unknown'", *con.get()).execute_scalar().get_value<int64_t>(), 3);

	TEST_EQUAL(cpp_db::statement("select count(*) from TEST_TABLE where COL1=? and COL2=?", *con.get()).execute_scalar(99, "Unknown").get_value<int64_t>(), 3);
}

void test_cpp_db_class::test_transaction_commit()
{
	cpp_db::transaction tran(*con.get());
	TEST_EQUAL(cpp_db::statement("select count(*) from TEST_TABLE", *con.get()).execute_scalar().get_value<int64_t>(), 8);

	TEST_FOR_NO_EXCPTION(tran.begin());
	cpp_db::statement stmt(*con.get());
	TEST_FOR_NO_EXCPTION(stmt.prepare("delete from TEST_TABLE where COL1 = ?"));
	TEST_FOR_NO_EXCPTION(stmt.execute_non_query(99));
	TEST_EQUAL(cpp_db::statement("select count(*) from TEST_TABLE", *con.get()).execute_scalar().get_value<int64_t>(), 5);
	TEST_FOR_NO_EXCPTION(tran.commit());
	TEST_EQUAL(cpp_db::statement("select count(*) from TEST_TABLE", *con.get()).execute_scalar().get_value<int64_t>(), 5);
}

void test_cpp_db_class::test_transaction_rollback()
{
	cpp_db::transaction tran(*con.get());
	TEST_EQUAL(cpp_db::statement("select count(*) from TEST_TABLE", *con.get()).execute_scalar().get_value<int64_t>(), 5);

	TEST_FOR_NO_EXCPTION(tran.begin());
	cpp_db::statement stmt(*con.get());
	TEST_FOR_NO_EXCPTION(stmt.prepare("delete from TEST_TABLE where COL1 > ?"));
	TEST_FOR_NO_EXCPTION(stmt.execute_non_query(3));
	TEST_EQUAL(cpp_db::statement("select count(*) from TEST_TABLE", *con.get()).execute_scalar().get_value<int64_t>(), 3);
	TEST_FOR_NO_EXCPTION(tran.rollback());
	TEST_EQUAL(cpp_db::statement("select count(*) from TEST_TABLE", *con.get()).execute_scalar().get_value<int64_t>(), 5);
}

void test_cpp_db_class::test_execute()
{
    TEST_EQUAL(cpp_db::execute_scalar(*con.get(), "select count(*) from TEST_TABLE").get_value<int64_t>(), 5);
    cpp_db::result r = cpp_db::execute(*con.get(), "select * from TEST_TABLE where COL1 in (?, ?)", 1, 2);
	int64_t sum = 0;
    std::string names;
    while(!r.is_eof())
    {
        sum += r.get_column_value(0).get_value<int64_t>();
        names.append(r.get_column_value(1).get_value<std::string>());
        r.move_next();
    }
    TEST_EQUAL(sum, 3);
    TEST_EQUAL(names, "firstsecond");

    TEST_EQUAL(cpp_db::execute_scalar(*con.get(), "select count(*) from TEST_TABLE where COL1 in (?, ?) and COL2 in (?, ?)", 1, 2, "first", "second").get_value<int64_t>(), 2);
    TEST_FOR_NO_EXCPTION(cpp_db::execute_non_query(*con.get(), "delete from TEST_TABLE where COL1 = ?", 1));
    TEST_EQUAL(cpp_db::execute_scalar(*con.get(), "select count(*) from TEST_TABLE").get_value<int64_t>(), 4);
    TEST_FOR_NO_EXCPTION(cpp_db::execute_non_query(*con.get(), "delete from TEST_TABLE"));
    TEST_EQUAL(cpp_db::execute_scalar(*con.get(), "select count(*) from TEST_TABLE").get_value<int64_t>(), 0);
}
