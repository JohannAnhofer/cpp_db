#ifndef TEST_POSTGRES_CLASS_H
#define TEST_POSTGRES_CLASS_H

#include "abstract_test.h"
#include "connection.h"

#include <memory>

class test_postgres_class : public test::abstract_test
{
public:
    BEGIN_DECLARE_TEST_CLASS(test_postgres_class)
    DECLARE_TEST_FUNCTION(test_connection)
    DECLARE_TEST_FUNCTION(test_transaction)
    DECLARE_TEST_FUNCTION(test_execute)
    DECLARE_TEST_FUNCTION(test_result_single_row)
    DECLARE_TEST_FUNCTION(test_result_properties)
    END_DECLARE_TEST_CLASS()

protected:
    void init_class() override;
    void cleanup_class() override;
    void init() override;
    void cleanup() override;

    void test_connection();
    void test_transaction();
    void test_execute();
    void test_result_single_row();
    void test_result_properties();

private:
    std::shared_ptr<cpp_db::connection> con;
};

#endif // TEST_POSTGRES_CLASS_H
