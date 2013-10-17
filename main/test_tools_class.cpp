#include "test_tools_class.h"
#include "coalesce.h"
#include "nullable_types.h"
#include "key_value_pair.h"

#include <memory>
#include <functional>

void test_tools_class::test_test_functions()
{
    test_condition("TRUE-Test success", true);
    test_condition("TRUE-Test failed", false);
	test_condition("EXCEPTIONS-Test", []() -> bool {throw std::runtime_error("This is a test!"); });
	TEST_VERIFY_RESULT(throw std::runtime_error("This is a test!"));
    TEST_EQUAL(3, 4711);
    TEST_EQUAL(4, 4);
    TEST_NOT_EQUAL(3, 4711);
    TEST_NOT_EQUAL(4, 4);
}

void test_tools_class::test_char_compare()
{
    char lca1 [] = "World";
    char rca1 [] = "World";
    const char lcca2 [] = "World", rcca2 [] = "World";
    const char *lccp3 = "World";
    const char *rccp3 = "World";
    const char * const lccpc4 = "World";
    const char * const rccpc4 = "World";

	TEST_EQUAL("World", "World");
    TEST_EQUAL("World", rca1);
    TEST_EQUAL("World", rcca2);
    TEST_EQUAL("World", rccp3);
    TEST_EQUAL("World", rccpc4);

    TEST_EQUAL(lca1, "World");
    TEST_EQUAL(lca1, rca1);
    TEST_EQUAL(lca1, rcca2);
    TEST_EQUAL(lca1, rccp3);
    TEST_EQUAL(lca1, rccpc4);

    TEST_EQUAL(lcca2, "World");
    TEST_EQUAL(lcca2, rca1);
    TEST_EQUAL(lcca2, rcca2);
    TEST_EQUAL(lcca2, rccp3);
    TEST_EQUAL(lcca2, rccpc4);

    TEST_EQUAL(lccp3, "World");
    TEST_EQUAL(lccp3, rca1);
    TEST_EQUAL(lccp3, rcca2);
    TEST_EQUAL(lccp3, rccp3);
    TEST_EQUAL(lccp3, rccpc4);

    TEST_EQUAL(lccpc4, "World");
    TEST_EQUAL(lccpc4, rca1);
    TEST_EQUAL(lccpc4, rcca2);
    TEST_EQUAL(lccpc4, rccp3);
    TEST_EQUAL(lccpc4, rccpc4);
}

void test_tools_class::test_wchar_t_compare()
{
    wchar_t lwa1 [] = L"World";
    wchar_t rwa1 [] = L"World";
    const wchar_t lcwa2 [] = L"World", rcwa2 [] = L"World";
    const wchar_t *lcwp3 = L"World";
    const wchar_t *rcwp3 = L"World";
    const wchar_t * const lcwpc4 = L"World";
    const wchar_t * const rcwpc4 = L"World";

	TEST_EQUAL(L"World", L"World");
    TEST_EQUAL(L"World", rwa1);
    TEST_EQUAL(L"World", rcwa2);
    TEST_EQUAL(L"World", rcwp3);
    TEST_EQUAL(L"World", rcwpc4);

    TEST_EQUAL(lwa1, L"World");
    TEST_EQUAL(lwa1, rwa1);
    TEST_EQUAL(lwa1, rcwa2);
    TEST_EQUAL(lwa1, rcwp3);
    TEST_EQUAL(lwa1, rcwpc4);

    TEST_EQUAL(lcwa2, L"World");
    TEST_EQUAL(lcwa2, rwa1);
    TEST_EQUAL(lcwa2, rcwa2);
    TEST_EQUAL(lcwa2, rcwp3);
    TEST_EQUAL(lcwa2, rcwpc4);

    TEST_EQUAL(lcwp3, L"World");
    TEST_EQUAL(lcwp3, rwa1);
    TEST_EQUAL(lcwp3, rcwa2);
    TEST_EQUAL(lcwp3, rcwp3);
    TEST_EQUAL(lcwp3, rcwpc4);

    TEST_EQUAL(lcwpc4, L"World");
    TEST_EQUAL(lcwpc4, rwa1);
    TEST_EQUAL(lcwpc4, rcwa2);
    TEST_EQUAL(lcwpc4, rcwp3);
    TEST_EQUAL(lcwpc4, rcwpc4);
}

void test_tools_class::test_char_neq_compare()
{
    char lca1 [] = "Hello";
    char rca1 [] = "World";
    const char lcca2 [] = "Hello", rcca2 [] = "World";
    const char *lccp3 = "Hello";
    const char *rccp3 = "World";
    const char * const lccpc4 = "Hello";
    const char * const rccpc4 = "World";

	TEST_NOT_EQUAL("Hello", "World");
    TEST_NOT_EQUAL("Hello", rca1);
    TEST_NOT_EQUAL("Hello", rcca2);
    TEST_NOT_EQUAL("Hello", rccp3);
    TEST_NOT_EQUAL("Hello", rccpc4);

    TEST_NOT_EQUAL(lca1, "World");
    TEST_NOT_EQUAL(lca1, rca1);
    TEST_NOT_EQUAL(lca1, rcca2);
    TEST_NOT_EQUAL(lca1, rccp3);
    TEST_NOT_EQUAL(lca1, rccpc4);

    TEST_NOT_EQUAL(lcca2, "World");
    TEST_NOT_EQUAL(lcca2, rca1);
    TEST_NOT_EQUAL(lcca2, rcca2);
    TEST_NOT_EQUAL(lcca2, rccp3);
    TEST_NOT_EQUAL(lcca2, rccpc4);

    TEST_NOT_EQUAL(lccp3, "World");
    TEST_NOT_EQUAL(lccp3, rca1);
    TEST_NOT_EQUAL(lccp3, rcca2);
    TEST_NOT_EQUAL(lccp3, rccp3);
    TEST_NOT_EQUAL(lccp3, rccpc4);

    TEST_NOT_EQUAL(lccpc4, "World");
    TEST_NOT_EQUAL(lccpc4, rca1);
    TEST_NOT_EQUAL(lccpc4, rcca2);
    TEST_NOT_EQUAL(lccpc4, rccp3);
    TEST_NOT_EQUAL(lccpc4, rccpc4);
}

void test_tools_class::test_wchar_t_neq_compare()
{
    wchar_t lwa1 [] = L"Hello";
    wchar_t rwa1 [] = L"World";
    const wchar_t lcwa2 [] = L"Hello", rcwa2 [] = L"World";
    const wchar_t *lcwp3 = L"Hello";
    const wchar_t *rcwp3 = L"World";
    const wchar_t * const lcwpc4 = L"Hello";
    const wchar_t * const rcwpc4 = L"World";

    TEST_NOT_EQUAL(L"Hello", rcwa2);
	TEST_NOT_EQUAL(L"Hello", L"World");
    TEST_NOT_EQUAL(L"Hello", rwa1);
    TEST_NOT_EQUAL(L"Hello", rcwp3);
    TEST_NOT_EQUAL(L"Hello", rcwpc4);

    TEST_NOT_EQUAL(lwa1, L"World");
    TEST_NOT_EQUAL(lwa1, rwa1);
    TEST_NOT_EQUAL(lwa1, rcwa2);
    TEST_NOT_EQUAL(lwa1, rcwp3);
    TEST_NOT_EQUAL(lwa1, rcwpc4);

    TEST_NOT_EQUAL(lcwa2, L"World");
    TEST_NOT_EQUAL(lcwa2, rwa1);
    TEST_NOT_EQUAL(lcwa2, rcwa2);
    TEST_NOT_EQUAL(lcwa2, rcwp3);
    TEST_NOT_EQUAL(lcwa2, rcwpc4);

    TEST_NOT_EQUAL(lcwp3, L"World");
    TEST_NOT_EQUAL(lcwp3, rwa1);
    TEST_NOT_EQUAL(lcwp3, rcwa2);
    TEST_NOT_EQUAL(lcwp3, rcwp3);
    TEST_NOT_EQUAL(lcwp3, rcwpc4);

    TEST_NOT_EQUAL(lcwpc4, L"World");
    TEST_NOT_EQUAL(lcwpc4, rwa1);
    TEST_NOT_EQUAL(lcwpc4, rcwa2);
    TEST_NOT_EQUAL(lcwpc4, rcwp3);
    TEST_NOT_EQUAL(lcwpc4, rcwpc4);
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
    int ai = 13;
    tools::coalesce_trait<tools::null_type, tools::null_type, tools::null_type>::type x7;
    TEST_VERIFY(!tools::is_null(1));
    TEST_VERIFY(!tools::is_null(ai));
    TEST_VERIFY(tools::is_null(tools::coalesce(null, null, null)));
    TEST_VERIFY(tools::is_null(x7));
    TEST_VERIFY(tools::is_null(null));
    TEST_VERIFY(tools::is_null(tools::null_type()));
}

void test_tools_class::test_key_value_pair()
{
    std::unique_ptr<tools::connection_option> p(tools::make_key_value_pair(std::string("driver"), std::string("QIBASE")));
    TEST_EQUAL((p->key<std::string, std::string>()), "driver");
    TEST_EQUAL((p->value<std::string, std::string>()), std::string("QIBASE"));
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

	tools::null_type null;

	TEST_EQUAL(tools::coalesce(1, 2, 3, 4), 1);
    TEST_EQUAL(tools::coalesce(null, 2, 3, 4), 2);
	TEST_EQUAL(tools::coalesce(1, null, null, null), 1);
	TEST_EQUAL(tools::coalesce(null, 2, null, 4), 2);

	TEST_EQUAL(x0, 0);
	TEST_EQUAL(x1, 0.0);
	TEST_EQUAL(x2, 0);
	TEST_EQUAL(x3, 0.0f);
	TEST_EQUAL(x4, 0);
	TEST_EQUAL(x5, 0.0f);
	TEST_EQUAL(x6, 0);
}
