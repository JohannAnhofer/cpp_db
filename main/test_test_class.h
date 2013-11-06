#ifndef TEST_TEST_CLASS_H
#define TEST_TEST_CLASS_H

#include "abstract_test.h"

class test_test_class :	public test::abstract_test
{
public:
	BEGIN_DECLARE_TEST_CLASS(test_test_class)
	DECLARE_TEST_FUNCTION(test_test_functions)
	DECLARE_TEST_FUNCTION(test_char_compare)
	DECLARE_TEST_FUNCTION(test_wchar_t_compare)
	DECLARE_TEST_FUNCTION(test_char_neq_compare)
	DECLARE_TEST_FUNCTION(test_wchar_t_neq_compare)
    DECLARE_TEST_FUNCTION(test_exceptions)
	END_DECLARE_TEST_CLASS()
protected:
	void test_test_functions();
	void test_char_compare();
	void test_wchar_t_compare();
	void test_char_neq_compare();
	void test_wchar_t_neq_compare();
    void test_exceptions();
};

#endif
