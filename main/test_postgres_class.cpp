#include "test_postgres_class.h"
#include "connection.h"
#include "user_password_authentication.h"

void test_postgres_class::init_class()
{
    con = std::shared_ptr<cpp_db::connection>(new cpp_db::connection("postgres"));
    TEST_FOR_NO_EXCEPTION(con->open("johny", cpp_db::user_password_authentication{"johny", "test"}, cpp_db::key_value_pair{{"host", "localhost"}} ));
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

void test_postgres_class::test_execute()
{
}
