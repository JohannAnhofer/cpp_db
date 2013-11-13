#include "test_cpp_db_class.h"

#include "null.h"
#include "value.h"
#include "parameter.h"
#include "type_of.h"

#include <stdexcept>

void test_cpp_db_class::test_is_null()
{
	cpp_db::null_type null;

    TEST_VERIFY(is_null(null));
	TEST_VERIFY(!cpp_db::is_null(1));

	cpp_db::value vnull(null);
	TEST_VERIFY(is_null(vnull));

	cpp_db::parameter pnull(0, null);
    TEST_VERIFY(is_null(pnull));
}

void test_cpp_db_class::test_value()
{
	TEST_FOR_NO_EXCEPTION((cpp_db::value(10).cast_to<int, int64_t>()));
	TEST_FOR_NO_EXCEPTION((cpp_db::value(10).cast_to<int, double>()));
	TEST_FOR_NO_EXCEPTION((cpp_db::value(10.0).cast_to<double, int64_t>()));
	TEST_FOR_EXCEPTION((cpp_db::value(10).cast_to<double, int64_t>()), std::runtime_error);
    TEST_FOR_EXCEPTION((cpp_db::value(10.0).cast_to<long double, float>()), std::runtime_error);

    TEST_EQUAL(cpp_db::type_of(10), typeid(int));
    TEST_EQUAL(cpp_db::type_of(cpp_db::value{10}), typeid(int));
	TEST_EQUAL(cpp_db::type_of(cpp_db::parameter{ 0, 10 }), typeid(int));
}
