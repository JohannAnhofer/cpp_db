#include "test_postgres_class.h"
#include "connection.h"
#include "user_password_authentication.h"
#include "transaction.h"

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
