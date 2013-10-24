#ifndef TEST_TEST_CLASS_STATISTICS
#define TEST_TEST_CLASS_STATISTICS

namespace test
{

struct test_class_statistics
{
	test_class_statistics();
	test_class_statistics &operator+=(const test_class_statistics &right);

	int function_count;
	int success_count;
	int fail_count;
	int exception_count;
	int expected_exception_count;
};

}

#endif
