#ifndef TEST_CPP_DB_CLASS_H
#define TEST_CPP_DB_CLASS_H

#include "abstract_test.h"

class test_cpp_db_class: public test::abstract_test
{
public:
    BEGIN_DECLARE_TEST_CLASS(test_cpp_db_class)
	DECLARE_TEST_FUNCTION(test_is_null)
    DECLARE_TEST_FUNCTION(test_type)
	DECLARE_TEST_FUNCTION(test_parameter)
    DECLARE_TEST_FUNCTION(test_conversions_i8)
    DECLARE_TEST_FUNCTION(test_conversions_ui8)
    DECLARE_TEST_FUNCTION(test_conversions_i16)
    DECLARE_TEST_FUNCTION(test_conversions_ui16)
    DECLARE_TEST_FUNCTION(test_conversions_i32)
    DECLARE_TEST_FUNCTION(test_conversions_ui32)
    DECLARE_TEST_FUNCTION(test_conversions_i64)
    DECLARE_TEST_FUNCTION(test_conversions_ui64)
    DECLARE_TEST_FUNCTION(test_conversions_floating_point)
    DECLARE_TEST_FUNCTION(test_conversions_strings)
    DECLARE_TEST_FUNCTION(test_cast_to)
    END_DECLARE_TEST_CLASS()

protected:
	void test_is_null();
    void test_type();
	void test_parameter();
    void test_conversions_i8();
    void test_conversions_ui8();
    void test_conversions_i16();
    void test_conversions_ui16();
    void test_conversions_i32();
    void test_conversions_ui32();
    void test_conversions_i64();
    void test_conversions_ui64();
    void test_conversions_floating_point();
    void test_conversions_strings();
    void test_cast_to();
};

#endif // TEST_CPP_DB_CLASS_H
