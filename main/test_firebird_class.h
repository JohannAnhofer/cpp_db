#ifndef TEST_FIREBIRD_CLASS_H
#define TEST_FIREBIRD_CLASS_H

#include "abstract_test.h"
#include "connection.h"

#include <memory>

class test_firebird_class : public test::abstract_test
{
public:
	BEGIN_DECLARE_TEST_CLASS(test_firebird_class)
    DECLARE_TEST_FUNCTION(test_execute_non_query)
    DECLARE_TEST_FUNCTION(test_execute_non_query_with_parameters)
    DECLARE_TEST_FUNCTION(test_result_single_row)
    DECLARE_TEST_FUNCTION(test_result_multi_row)
    DECLARE_TEST_FUNCTION(test_result_empty)
    END_DECLARE_TEST_CLASS()

protected:
	void init_class() override;
	void cleanup_class() override;
    void test_execute_non_query();
    void test_execute_non_query_with_parameters();
    void test_result_single_row();
    void test_result_multi_row();
    void test_result_empty();

private:
    std::shared_ptr<cpp_db::connection> con;
};

#endif // TEST_FIREBIRD_CLASS_H
