#include "test_postgres_class.h"
#include "connection.h"
#include "user_password_authentication.h"
#include "transaction.h"
#include "statement.h"
#include "execute.h"
#include "transaction_scope.h"

void test_postgres_class::init_class()
{
    con = std::shared_ptr<cpp_db::connection>(new cpp_db::connection("postgres"));
    TEST_FOR_NO_EXCEPTION(con->open("johny", cpp_db::no_authentication{}, cpp_db::key_value_pair{{"host", "localhost"}} ));
}

void test_postgres_class::cleanup_class()
{
    TEST_FOR_NO_EXCEPTION(con->close());
    TEST_VERIFY(!con->is_open());
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
    const char *sql = R"(
                create table if not exists test_table (
                    id int primary key,
                    name varchar(50),
                    age int
                );
            )";

    TEST_FOR_NO_EXCEPTION(cpp_db::execute_ddl(*con, sql));
    cpp_db::statement cmd("insert into test_table(id, name, age) values(1, 'dad', 45);", *con);
    TEST_FOR_NO_EXCEPTION(cmd.execute_ddl());
    TEST_FOR_NO_EXCEPTION(cmd.prepare("insert into test_table(id, name, age) values(2, 'mom', 41);"));
    TEST_FOR_NO_EXCEPTION(cmd.execute_ddl());

    cpp_db::transaction tr(*con);
    {
        cpp_db::transaction_scope trs(&tr);
        TEST_FOR_NO_EXCEPTION(cpp_db::execute_ddl(*con, "insert into test_table(id, name, age) values(3, 'son', 3);"));
        TEST_FOR_NO_EXCEPTION(cpp_db::execute_ddl(*con, "insert into test_table(id, name, age) values(4, 'daughter', 1);"));
    }
    {
        cpp_db::transaction_scope trs(&tr);
        TEST_FOR_NO_EXCEPTION(cpp_db::execute_ddl(*con, "insert into test_table(id, name, age) values(5, 'xxxxxx', 17);"));
        tr.rollback();
    }

    TEST_FOR_NO_EXCEPTION(cpp_db::execute_ddl(*con, "drop table if exists test_table;"));
}
