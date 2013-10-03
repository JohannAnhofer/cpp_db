#include "test.h"

#include "db_exception.h"
#include "connection.h"
#include "sql_statement.h"
#include "record.h"

void test_cpp_db()
{
	test_message("========== Database test starts ==========");

	try
	{
		cpp_db::connection con("sqlite");
		TEST_FOR_NO_EXCPTION(con.open(":memory:"));
        TEST_FOR_NO_EXCPTION(cpp_db::sql_statement("create table TEST_TABLE(COL1 INT NOT NULL, COL2 VARCHAR(50));", con).execute_ddl());
        TEST_FOR_NO_EXCPTION(cpp_db::sql_statement("insert into TEST_TABLE(COL1, COL2) VALUES(1, 'first')", con).execute_non_query());
        TEST_FOR_NO_EXCPTION(cpp_db::sql_statement("insert into TEST_TABLE(COL1, COL2) VALUES(2, 'second')", con).execute_non_query());
        TEST_FOR_NO_EXCPTION(cpp_db::sql_statement("insert into TEST_TABLE(COL1, COL2) VALUES(3, 'third')", con).execute_non_query());
        TEST_FOR_EXCEPTION(cpp_db::sql_statement("select COL1, COL2, COL3 from TEST_TABLE;", con).execute_non_query(), cpp_db::db_exception);
        {
            cpp_db::sql_statement stmt(con);
            TEST_FOR_NO_EXCPTION(stmt.prepare("insert into TEST_TABLE(COL1, COL2) VALUES(?, ?)"));
            TEST_FOR_EXCEPTION(stmt.execute_non_query(), cpp_db::db_exception);
        }
        cpp_db::record result(cpp_db::sql_statement("select COL1, COL2 from TEST_TABLE;", con));
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

        TEST_FOR_EXCEPTION(cpp_db::record(cpp_db::sql_statement(con)), cpp_db::db_exception);
	}
	catch (const cpp_db::db_exception &e)
	{
		test_message(std::string("Exception: ") + e.what());
		TEST_VERIFY(false);
	}
	test_message("========== Database test ends ==========");
}
