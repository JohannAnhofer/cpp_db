#include "test.h"

#include "db_exception.h"
#include "connection.h"
#include "statement.h"
#include "record.h"
#include "parameters.h"

#include <cmath>

void test_cpp_db()
{
	test_message("========== Database test starts ==========");

	try
	{
		cpp_db::connection con("sqlite");
		TEST_FOR_NO_EXCPTION(con.open(":memory:"));
        TEST_FOR_NO_EXCPTION(cpp_db::statement("create table TEST_TABLE(COL1 INT NOT NULL, COL2 VARCHAR(50));", con).execute_ddl());
        TEST_FOR_NO_EXCPTION(cpp_db::statement("insert into TEST_TABLE(COL1, COL2) VALUES(1, 'first')", con).execute_non_query());
        TEST_FOR_NO_EXCPTION(cpp_db::statement("insert into TEST_TABLE(COL1, COL2) VALUES(2, 'second')", con).execute_non_query());
        TEST_FOR_NO_EXCPTION(cpp_db::statement("insert into TEST_TABLE(COL1, COL2) VALUES(3, 'third')", con).execute_non_query());
        TEST_FOR_EXCEPTION(cpp_db::statement("select COL1, COL2, COL3 from TEST_TABLE;", con).execute_non_query(), cpp_db::db_exception);
        cpp_db::statement stmt(con);
        TEST_FOR_NO_EXCPTION(stmt.prepare("insert into TEST_TABLE(COL1, COL2) VALUES(?, ?)"));
        TEST_FOR_EXCEPTION(stmt.execute_non_query(), cpp_db::db_exception);
        cpp_db::record result(cpp_db::statement("select COL1, COL2 from TEST_TABLE;", con));
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

        TEST_FOR_EXCEPTION(cpp_db::record(cpp_db::statement(con)), cpp_db::db_exception);

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
	}
	catch (const cpp_db::db_exception &e)
	{
		test_message(std::string("Exception: ") + e.what());
		TEST_VERIFY(false);
	}
	test_message("========== Database test ends ==========");
}
