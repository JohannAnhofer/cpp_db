#ifndef TEST_CPP_DB_CLASS_H
#define TEST_CPP_DB_CLASS_H

#include "abstract_test.h"

class test_cpp_db_class: public test::abstract_test
{
public:
    BEGIN_DECLARE_TEST_CLASS(test_cpp_db_class)
	DECLARE_TEST_FUNCTION(test_is_null)
	DECLARE_TEST_FUNCTION(test_value)
    END_DECLARE_TEST_CLASS()

protected:
	void test_is_null();
	void test_value();
};

#endif // TEST_CPP_DB_CLASS_H
