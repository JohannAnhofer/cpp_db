#include "test_tools_class.h"
#include "coalesce.h"
#include "nullable_types.h"
#include "key_value_pair.h"

#include <memory>
#include <functional>

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

    TEST_VERIFY(!tools::is_null(a) && !a.is_null());
    TEST_VERIFY(tools::is_null(d) && d.is_null());
    TEST_VERIFY(!tools::is_null(e) && !e.is_null());
    TEST_VERIFY(tools::is_null(f) && f.is_null());
    TEST_VERIFY(tools::is_null(tools::nullable_int()) && tools::nullable_int().is_null());

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
