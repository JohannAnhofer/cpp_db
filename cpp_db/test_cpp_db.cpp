#include "test.h"

#include "db_exception.h"
#include "connection.h"
#include "sql_statement.h"

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
		TEST_FOR_EXCEPTION(cpp_db::sql_statement("select COL1, COL2, COL3 from TEST_TABLE;", con).execute(), cpp_db::db_exception);
        cpp_db::sql_statement stmt(con);
        TEST_FOR_NO_EXCPTION(stmt.prepare("insert into TEST_TABLE(COL1, COL2) VALUES(?, ?)"));
        TEST_FOR_EXCEPTION(stmt.execute_non_query(), cpp_db::db_exception);
		TEST_FOR_NO_EXCPTION(cpp_db::sql_statement("select COL1, COL2 from TEST_TABLE;", con).execute());
	}
	catch (const cpp_db::db_exception &e)
	{
		test_message(std::string("Exception: ") + e.what());
		TEST_VERIFY(false);
	}
	test_message("========== Database test ends ==========");
}
