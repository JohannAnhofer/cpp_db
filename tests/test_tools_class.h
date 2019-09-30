#ifndef TEST_TOOLS_CLASS_H
#define TEST_TOOLS_CLASS_H

#include "abstract_test.h"

class test_tools_class : public test::abstract_test
{
public:
    BEGIN_DECLARE_TEST_CLASS(test_tools_class)
    DECLARE_TEST_FUNCTION(test_nullable)
    DECLARE_TEST_FUNCTION(test_is_null)
	DECLARE_TEST_FUNCTION(test_coalesce)
	DECLARE_TEST_FUNCTION(test_key_value_pair)
	END_DECLARE_TEST_CLASS()

protected:
    void test_nullable();
    void test_is_null();
	void test_coalesce();
	void test_key_value_pair();
};

#endif // TEST_TOOLS_CLASS_H
