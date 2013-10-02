#include "nullable_types.h"
#include "key_value_pair.h"
#include "coalesce.h"
#include "test.h"

#include <iostream>
#include <memory>
#include <functional>

void test_cpp_db();

int main(int , char *[])
{
    std::cout << std::endl;

    test_stream(&std::cerr);

	test_message("---------- Test testfunctions ----------");
	test_condition("TRUE-Test success", true);
	test_condition("TRUE-Test failed", false);
    test_condition("EXCEPTIONS-Test", []() {if (false) return true; else throw std::runtime_error("This is a test!"); });
    TEST_VERIFY_RESULT(if (false) return true; else throw std::runtime_error("This is a test!"));
	TEST_EQUAL(3, 4711);
	TEST_EQUAL(4, 4);
	TEST_NOT_EQUAL(3, 4711);
	TEST_NOT_EQUAL(4, 4);

	test_message("========== Test starts ==========");

    try
    {
        cpp_db::nullable_int a, c(815);

        a = 4711;

        int b = a;
        cpp_db::nullable_int d;
        const cpp_db::nullable_int e(10), f;

		test_message("---------- Test nullable ----------");

		TEST_NOT_EQUAL(a, 13);
		TEST_EQUAL(a, 4711);
		TEST_EQUAL(b, a);
		TEST_EQUAL(b, 4711);
		TEST_EQUAL(c, 815);

		test_message("---------- Test is_null() ----------");

		test_condition("a is non null: ", !cpp_db::is_null(a) && !a.is_null());
		test_condition("d is null: ", cpp_db::is_null(d) && d.is_null());
		test_condition("e is non null: ", !cpp_db::is_null(e) && !e.is_null());
		test_condition("f is null: ", cpp_db::is_null(f) && f.is_null());
		test_condition("null is null: ", cpp_db::is_null(cpp_db::nullable_int()) && cpp_db::nullable_int().is_null());

		cpp_db::null_type null;

		TEST_VERIFY(cpp_db::is_null(null));
		TEST_VERIFY(!cpp_db::is_null(1));

		test_message("---------- Test coalesce ----------");

        cpp_db::coalesce_trait<int, double, float>::type x0(0);
        cpp_db::coalesce_trait<cpp_db::null_type, double, float>::type x1(0.0);
        cpp_db::coalesce_trait<int, cpp_db::null_type, float>::type x2(0);
        cpp_db::coalesce_trait<cpp_db::null_type, cpp_db::null_type, float>::type x3(0.0f);
        cpp_db::coalesce_trait<int, double, cpp_db::null_type>::type x4(0);
        cpp_db::coalesce_trait<cpp_db::null_type, double, cpp_db::null_type>::type x5(0.0f);
        cpp_db::coalesce_trait<int, cpp_db::null_type, cpp_db::null_type>::type x6(0);
		cpp_db::coalesce_trait<cpp_db::null_type, cpp_db::null_type, cpp_db::null_type>::type x7;

		TEST_EQUAL(cpp_db::coalesce(1, 2, 3, 4), 1);
		TEST_EQUAL(cpp_db::coalesce(cpp_db::null_type(), 2, 3, 4), 2);
		TEST_VERIFY(cpp_db::is_null(cpp_db::coalesce(null, null, null)));
		TEST_EQUAL(cpp_db::coalesce(1, null, null, null), 1);
		TEST_EQUAL(cpp_db::coalesce(null, 2, null, 4), 2);
            
		TEST_EQUAL(x0, 0);
		TEST_EQUAL(x1, 0.0);
		TEST_EQUAL(x2, 0);
		TEST_EQUAL(x3, 0.0f);
		TEST_EQUAL(x4, 0);
		TEST_EQUAL(x5, 0.0f);
		TEST_EQUAL(x6, 0);
		TEST_VERIFY(is_null(x7));

		std::unique_ptr<cpp_db::connection_option> p(cpp_db::make_key_value_pair(std::string("driver"), std::string("QIBASE")));
		TEST_EQUAL( (p->key<std::string, std::string>()), "driver");
		TEST_EQUAL( (p->value<std::string, std::string>()) , std::string("QIBASE"));

		test_message("========== Test ends ==========");

		test_cpp_db();
	}
    catch(const std::exception &ex)
    {
		test_message(std::string("Exception: ") + ex.what());
	}
}
