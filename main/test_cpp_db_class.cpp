#include "test_cpp_db_class.h"

#include "db_exception.h"
#include "statement.h"
#include "record.h"
#include "parameters.h"

#include <cmath>

std::ostream &operator<<(std::ostream &output, cpp_db::record &record)
{
	record.move_first();
	output << "\t";
	for (int i = 0; i < record.get_column_count(); ++i)
		output << record.get_field_name(i) << "\t|\t";
	output << "\n" << std::string(80, '-') << "\n";

	while (!record.is_eof())
	{
		output << "\t";
		for (int i = 0; i < record.get_column_count(); ++i)
			output << record.get_field_value(i) << "\t|\t";
		output << "\n";
		record.move_next();
	}
	return output;
}

void test_cpp_db_class::init_class()
{
    con.reset(new cpp_db::connection("sqlite"));
    TEST_FOR_NO_EXCPTION(con->open(":memory:"));
}

void test_cpp_db_class::cleanup_class()
{
    con.reset();
}

void test_cpp_db_class::test_connection()
{
    TEST_FOR_NO_EXCPTION(cpp_db::statement("create table TEST_TABLE(COL1 INT NOT NULL, COL2 VARCHAR(50));", *con.get()).execute_ddl());
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
    cpp_db::record result(cpp_db::statement("select COL1, COL2 from TEST_TABLE;", *con.get()));
    TEST_EQUAL(result.get_column_count(), 2);
    TEST_EQUAL(result.get_field_name(0), "COL1");
    TEST_EQUAL(result.get_field_name(1), "COL2");
    TEST_VERIFY(!result.is_eof());
    TEST_EQUAL(result.get_field_value(0), "1");
    TEST_EQUAL(result.get_field_value(1), "first");
    TEST_FOR_NO_EXCPTION(result.move_next());
    TEST_VERIFY(!result.is_eof());
    TEST_EQUAL(result.get_field_value(0), "2");
    TEST_EQUAL(result.get_field_value(1), "second");
    TEST_FOR_NO_EXCPTION(result.move_next());
    TEST_VERIFY(!result.is_eof());
    TEST_EQUAL(result.get_field_value(0), "3");
    TEST_EQUAL(result.get_field_value(1), "third");
    TEST_FOR_NO_EXCPTION(result.move_next());
    TEST_VERIFY(result.is_eof());
    TEST_FOR_NO_EXCPTION(result.move_first());
    TEST_EQUAL(result.get_field_value(0), "1");
    TEST_EQUAL(result.get_field_value(1), "first");

    TEST_FOR_EXCEPTION(cpp_db::record(cpp_db::statement(*con.get())), cpp_db::db_exception);
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
