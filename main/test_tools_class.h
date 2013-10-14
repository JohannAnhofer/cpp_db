#ifndef TEST_TOOLS_CLASS_H
#define TEST_TOOLS_CLASS_H

#include "abstract_test.h"

class test_tools_class : public test::abstract_test
{
public:
    BEGIN_DECLARE_TEST_CLASS(test_tools_class)
    DECLARE_TEST_FUNCTION(test_test_functions)
    DECLARE_TEST_FUNCTION(test_nullable)
    DECLARE_TEST_FUNCTION(test_is_null)
    DECLARE_TEST_FUNCTION(test_key_value_pair)
	DECLARE_TEST_FUNCTION(test_char_compare)
	DECLARE_TEST_FUNCTION(test_wchar_t_compare)
	DECLARE_TEST_FUNCTION(test_char_neq_compare)
	DECLARE_TEST_FUNCTION(test_wchar_t_neq_compare)
	END_DECLARE_TEST_CLASS()

protected:
    void test_test_functions();
    void test_nullable();
    void test_is_null();
    void test_key_value_pair();
	void test_char_compare();
	void test_wchar_t_compare();
	void test_char_neq_compare();
	void test_wchar_t_neq_compare();
};

#endif // TEST_TOOLS_CLASS_H
