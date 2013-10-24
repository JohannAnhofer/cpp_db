#include "test_class_statistics.h"

namespace test
{

test_class_statistics::test_class_statistics()
	: function_count(0)
	, success_count(0)
	, fail_count(0)
	, exception_count(0)
	, expected_exception_count(0)
{
}

test_class_statistics &test_class_statistics::operator += (const test_class_statistics &right)
{
	function_count += right.function_count;
	success_count += right.success_count;
	fail_count += right.fail_count;
	exception_count += right.exception_count;
	expected_exception_count += right.expected_exception_count;
	return *this;
}

}
