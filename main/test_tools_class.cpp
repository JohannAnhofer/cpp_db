#include "test_tools_class.h"

#include "nullable_types.h"
#include "key_value_pair.h"
#include "coalesce.h"

#include <memory>
#include <functional>

void test_tools_class::test_test_functions()
{
    test_condition("TRUE-Test success", true);
    test_condition("TRUE-Test failed", false);
    test_condition("EXCEPTIONS-Test", []() {if (false) return true; else throw std::runtime_error("This is a test!"); });
    TEST_VERIFY_RESULT(if (false) return true; else throw std::runtime_error("This is a test!"));
    TEST_EQUAL(3, 4711);
    TEST_EQUAL(4, 4);
    TEST_NOT_EQUAL(3, 4711);
    TEST_NOT_EQUAL(4, 4);
}

void test_tools_class::test_char_compare()
{
	char l1 [] = "World";
	char r1 [] = "World";
	const char l2 [] = "World", r2 [] = "World";
	const char *l3 = "World";
	const char *r3 = "World";
	const char * const l4 = "World";
	const char * const r4 = "World";

	TEST_EQUAL("World", "World");
	TEST_EQUAL("World", r1);
	TEST_EQUAL("World", r2);
	TEST_EQUAL("World", r3);
	TEST_EQUAL("World", r4);

	TEST_EQUAL(l1, "World");
	TEST_EQUAL(l1, r1);
	TEST_EQUAL(l1, r2);
	TEST_EQUAL(l1, r3);
	TEST_EQUAL(l1, r4);

	TEST_EQUAL(l2, "World");
	TEST_EQUAL(l2, r1);
	TEST_EQUAL(l2, r2);
	TEST_EQUAL(l2, r3);
	TEST_EQUAL(l2, r4);

	TEST_EQUAL(l3, "World");
	TEST_EQUAL(l3, r1);
	TEST_EQUAL(l3, r2);
	TEST_EQUAL(l3, r3);
	TEST_EQUAL(l3, r4);

	TEST_EQUAL(l4, "World");
	TEST_EQUAL(l4, r1);
	TEST_EQUAL(l4, r2);
	TEST_EQUAL(l4, r3);
	TEST_EQUAL(l4, r4);
}

void test_tools_class::test_wchar_t_compare()
{
	wchar_t l1 [] = L"World";
	wchar_t r1 [] = L"World";
	const wchar_t l2 [] = L"World", r2 [] = L"World";
	const wchar_t *l3 = L"World";
	const wchar_t *r3 = L"World";
	const wchar_t * const l4 = L"World";
	const wchar_t * const r4 = L"World";

	TEST_EQUAL(L"World", L"World");
	TEST_EQUAL(L"World", r1);
	TEST_EQUAL(L"World", r2);
	TEST_EQUAL(L"World", r3);
	TEST_EQUAL(L"World", r4);

	TEST_EQUAL(l1, L"World");
	TEST_EQUAL(l1, r1);
	TEST_EQUAL(l1, r2);
	TEST_EQUAL(l1, r3);
	TEST_EQUAL(l1, r4);

	TEST_EQUAL(l2, L"World");
	TEST_EQUAL(l2, r1);
	TEST_EQUAL(l2, r2);
	TEST_EQUAL(l2, r3);
	TEST_EQUAL(l2, r4);

	TEST_EQUAL(l3, L"World");
	TEST_EQUAL(l3, r1);
	TEST_EQUAL(l3, r2);
	TEST_EQUAL(l3, r3);
	TEST_EQUAL(l3, r4);

	TEST_EQUAL(l4, L"World");
	TEST_EQUAL(l4, r1);
	TEST_EQUAL(l4, r2);
	TEST_EQUAL(l4, r3);
	TEST_EQUAL(l4, r4);
}

void test_tools_class::test_char_neq_compare()
{
	char l1 [] = "Hello";
	char r1 [] = "World";
	const char l2 [] = "Hello", r2 [] = "World";
	const char *l3 = "Hello";
	const char *r3 = "World";
	const char * const l4 = "Hello";
	const char * const r4 = "World";

	TEST_NOT_EQUAL("Hello", "World");
	TEST_NOT_EQUAL("Hello", r1);
	TEST_NOT_EQUAL("Hello", r2);
	TEST_NOT_EQUAL("Hello", r3);
	TEST_NOT_EQUAL("Hello", r4);

	TEST_NOT_EQUAL(l1, "World");
	TEST_NOT_EQUAL(l1, r1);
	TEST_NOT_EQUAL(l1, r2);
	TEST_NOT_EQUAL(l1, r3);
	TEST_NOT_EQUAL(l1, r4);

	TEST_NOT_EQUAL(l2, "World");
	TEST_NOT_EQUAL(l2, r1);
	TEST_NOT_EQUAL(l2, r2);
	TEST_NOT_EQUAL(l2, r3);
	TEST_NOT_EQUAL(l2, r4);

	TEST_NOT_EQUAL(l3, "World");
	TEST_NOT_EQUAL(l3, r1);
	TEST_NOT_EQUAL(l3, r2);
	TEST_NOT_EQUAL(l3, r3);
	TEST_NOT_EQUAL(l3, r4);

	TEST_NOT_EQUAL(l4, "World");
	TEST_NOT_EQUAL(l4, r1);
	TEST_NOT_EQUAL(l4, r2);
	TEST_NOT_EQUAL(l4, r3);
	TEST_NOT_EQUAL(l4, r4);
}

void test_tools_class::test_wchar_t_neq_compare()
{
	wchar_t l1 [] = L"Hello";
	wchar_t r1 [] = L"World";
	const wchar_t l2 [] = L"Hello", r2 [] = L"World";
	const wchar_t *l3 = L"Hello";
	const wchar_t *r3 = L"World";
	const wchar_t * const l4 = L"Hello";
	const wchar_t * const r4 = L"World";

	TEST_NOT_EQUAL(L"Hello", L"World");
	TEST_NOT_EQUAL(L"Hello", r1);
	TEST_NOT_EQUAL(L"Hello", r2);
	TEST_NOT_EQUAL(L"Hello", r3);
	TEST_NOT_EQUAL(L"Hello", r4);

	TEST_NOT_EQUAL(l1, L"World");
	TEST_NOT_EQUAL(l1, r1);
	TEST_NOT_EQUAL(l1, r2);
	TEST_NOT_EQUAL(l1, r3);
	TEST_NOT_EQUAL(l1, r4);

	TEST_NOT_EQUAL(l2, L"World");
	TEST_NOT_EQUAL(l2, r1);
	TEST_NOT_EQUAL(l2, r2);
	TEST_NOT_EQUAL(l2, r3);
	TEST_NOT_EQUAL(l2, r4);

	TEST_NOT_EQUAL(l3, L"World");
	TEST_NOT_EQUAL(l3, r1);
	TEST_NOT_EQUAL(l3, r2);
	TEST_NOT_EQUAL(l3, r3);
	TEST_NOT_EQUAL(l3, r4);

	TEST_NOT_EQUAL(l4, L"World");
	TEST_NOT_EQUAL(l4, r1);
	TEST_NOT_EQUAL(l4, r2);
	TEST_NOT_EQUAL(l4, r3);
	TEST_NOT_EQUAL(l4, r4);
}

void test_tools_class::test_nullable()
{
    tools::nullable_int a, c(815);

    a = 4711;

    int b = a;

    TEST_NOT_EQUAL(a, 13);
    TEST_EQUAL(a, 4711);
    TEST_EQUAL(b, a);
    TEST_EQUAL(b, 4711);
    TEST_EQUAL(c, 815);
}

void test_tools_class::test_is_null()
{
    tools::nullable_int a(4711);
    tools::nullable_int d;
    const tools::nullable_int e(10), f;

    test_condition("a is non null: ", !tools::is_null(a) && !a.is_null());
    test_condition("d is null: ", tools::is_null(d) && d.is_null());
    test_condition("e is non null: ", !tools::is_null(e) && !e.is_null());
    test_condition("f is null: ", tools::is_null(f) && f.is_null());
    test_condition("null is null: ", tools::is_null(tools::nullable_int()) && tools::nullable_int().is_null());

    tools::null_type null;

    TEST_VERIFY(tools::is_null(null));
    TEST_VERIFY(!tools::is_null(1));
}

void test_tools_class::test_coalesce()
{
    tools::coalesce_trait<int, double, float>::type x0(0);
    tools::coalesce_trait<tools::null_type, double, float>::type x1(0.0);
    tools::coalesce_trait<int, tools::null_type, float>::type x2(0);
    tools::coalesce_trait<tools::null_type, tools::null_type, float>::type x3(0.0f);
    tools::coalesce_trait<int, double, tools::null_type>::type x4(0);
    tools::coalesce_trait<tools::null_type, double, tools::null_type>::type x5(0.0f);
    tools::coalesce_trait<int, tools::null_type, tools::null_type>::type x6(0);
    tools::coalesce_trait<tools::null_type, tools::null_type, tools::null_type>::type x7;

    tools::null_type null;

    TEST_EQUAL(tools::coalesce(1, 2, 3, 4), 1);
    TEST_EQUAL(tools::coalesce(tools::null_type(), 2, 3, 4), 2);
    TEST_VERIFY(tools::is_null(tools::coalesce(null, null, null)));
    TEST_EQUAL(tools::coalesce(1, null, null, null), 1);
    TEST_EQUAL(tools::coalesce(null, 2, null, 4), 2);

    TEST_EQUAL(x0, 0);
    TEST_EQUAL(x1, 0.0);
    TEST_EQUAL(x2, 0);
    TEST_EQUAL(x3, 0.0f);
    TEST_EQUAL(x4, 0);
    TEST_EQUAL(x5, 0.0f);
    TEST_EQUAL(x6, 0);
    TEST_VERIFY(is_null(x7));
}

void test_tools_class::test_key_value_pair()
{
    std::unique_ptr<tools::connection_option> p(tools::make_key_value_pair(std::string("driver"), std::string("QIBASE")));
    TEST_EQUAL((p->key<std::string, std::string>()), "driver");
    TEST_EQUAL((p->value<std::string, std::string>()), std::string("QIBASE"));
}
