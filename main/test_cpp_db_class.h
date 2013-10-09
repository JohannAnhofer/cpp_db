#ifndef TEST_CPP_DB_CLASS_H
#define TEST_CPP_DB_CLASS_H

#include "abstract_test.h"
#include "connection.h"

#include <memory>

class test_cpp_db_class: public test::abstract_test
{
public:
    BEGIN_DECLARE_TEST_CLASS(test_cpp_db_class)
    DECLARE_TEST_FUNCTION(test_connection)
    DECLARE_TEST_FUNCTION(test_statement)
    DECLARE_TEST_FUNCTION(test_result)
    DECLARE_TEST_FUNCTION(test_parameter)
    END_DECLARE_TEST_CLASS()

protected:
    void init_class() override;
    void cleanup_class() override;
    void test_connection();
    void test_statement();
    void test_result();
    void test_parameter();

private:
    std::unique_ptr<cpp_db::connection> con;
};

#endif // TEST_CPP_DB_CLASS_H
