#include "test_cpp_db_class.h"

#include "null.h"
#include "value.h"
#include "parameter.h"
#include "type_of.h"

#include <stdexcept>
#include <cmath>

void test_cpp_db_class::test_is_null()
{
	cpp_db::null_type null;

    TEST_VERIFY(cpp_db::is_null(null));
	TEST_VERIFY(!cpp_db::is_null(1));

	cpp_db::value vnull(null);
    TEST_VERIFY(cpp_db::is_null(vnull));

	cpp_db::parameter pnull(0, null);
    TEST_VERIFY(cpp_db::is_null(pnull));

    TEST_FOR_EXCEPTION(cpp_db::type_of(null), std::logic_error);
    TEST_FOR_EXCEPTION(cpp_db::type_of(vnull), std::logic_error);
    TEST_FOR_EXCEPTION(cpp_db::type_of(pnull), std::logic_error);

    TEST_FOR_EXCEPTION(cpp_db::value_of(null), std::logic_error);
    TEST_FOR_EXCEPTION(cpp_db::value_of<int>(vnull), std::logic_error);
    TEST_FOR_EXCEPTION(cpp_db::value_of<int>(pnull), std::logic_error);
}

void test_cpp_db_class::test_value()
{
    TEST_FOR_NO_EXCEPTION((cpp_db::cast_to<int, int64_t>(cpp_db::value(10))));
    TEST_FOR_NO_EXCEPTION((cpp_db::cast_to<int, double>(cpp_db::value(10))));
    TEST_FOR_NO_EXCEPTION((cpp_db::cast_to<double, int64_t>(cpp_db::value(10.0))));
    TEST_FOR_EXCEPTION((cpp_db::cast_to<double, int64_t>(cpp_db::value(10))), std::runtime_error);
    TEST_FOR_EXCEPTION((cpp_db::cast_to<long double, float>(cpp_db::value(10.0))), std::runtime_error);

    TEST_EQUAL(cpp_db::type_of(10), typeid(int));
    TEST_EQUAL(cpp_db::type_of(cpp_db::value{10}), typeid(int));
	TEST_EQUAL(cpp_db::type_of(cpp_db::parameter{ 0, 10 }), typeid(int));
}

void test_cpp_db_class::test_parameter()
{
	cpp_db::parameter param1(0, std::string("Hello world!")), param2(std::string("@test"), 27.85);

	TEST_VERIFY(param1.has_index());
	TEST_VERIFY(!param1.has_name());
	TEST_VERIFY(!param2.has_index());
	TEST_VERIFY(param2.has_name());
    TEST_EQUAL(cpp_db::type_of(param1), typeid(std::string));
    TEST_NOT_EQUAL(cpp_db::type_of(param1), typeid(double));
    TEST_NOT_EQUAL(cpp_db::type_of(param2), typeid(std::string));
    TEST_EQUAL(cpp_db::type_of(param2), typeid(double));

    TEST_EQUAL(cpp_db::value_of<std::string>(param1), "Hello world!");
    TEST_VERIFY(std::fabs(cpp_db::value_of<double>(param2) - 27.85) < 0.000001);

    TEST_FOR_EXCEPTION(cpp_db::value_of<int>(param1), std::runtime_error);
}
