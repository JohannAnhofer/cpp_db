#ifndef TEST_CPP_DB_CLASS_H
#define TEST_CPP_DB_CLASS_H

#include "abstract_test.h"
#include "connection.h"

#include <memory>

class test_cpp_db_class: public test::abstract_test
{
public:
    BEGIN_DECLARE_TEST_CLASS(test_cpp_db_class)
	DECLARE_TEST_FUNCTION(test_is_null)
	DECLARE_TEST_FUNCTION(test_connection)
    DECLARE_TEST_FUNCTION(test_statement)
    DECLARE_TEST_FUNCTION(test_result)
    DECLARE_TEST_FUNCTION(test_parameter)
	DECLARE_TEST_FUNCTION(test_named_parameter)
	DECLARE_TEST_FUNCTION(test_transaction_commit)
	DECLARE_TEST_FUNCTION(test_transaction_rollback)
    DECLARE_TEST_FUNCTION(test_execute)
	DECLARE_TEST_FUNCTION(test_value)
    END_DECLARE_TEST_CLASS()

protected:
    void init_class() override;
    void cleanup_class() override;
	void test_is_null();
	void test_connection();
    void test_statement();
    void test_result();
    void test_parameter();
	void test_transaction_commit();
	void test_transaction_rollback();
    void test_execute();
	void test_named_parameter();
	void test_value();

private:
    std::shared_ptr<cpp_db::connection> con;
};

#endif // TEST_CPP_DB_CLASS_H
