#define BOOST_TEST_MODULE cppdb_tools
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "coalesce.h"
#include "nullable_types.h"
#include "key_value_pair.h"

#include <memory>
#include <functional>

// TODO: check throwing

namespace std
{
    ostream& operator<<(ostream& os, tools::nullable_int ni)
    {
        if (ni)
            os << "null";
        else
            os << *ni;
        return os;
    }
}

struct ToolsTestFixture {};

BOOST_FIXTURE_TEST_SUITE(test_tools, ToolsTestFixture)

BOOST_AUTO_TEST_CASE(test_nullable)
{
    tools::nullable_int a, c(815);

    a = 4711;

    int b = a.value();

    BOOST_CHECK_NE(a, 13);
    BOOST_CHECK_EQUAL(a, 4711);
    BOOST_CHECK_EQUAL(b, a);
    BOOST_CHECK_EQUAL(b, 4711);
    BOOST_CHECK_EQUAL(c, 815);
}

BOOST_AUTO_TEST_CASE(test_is_null_tools)
{
    tools::nullable_int a(4711);
    tools::nullable_int d;
    const tools::nullable_int e(10), f;

    BOOST_CHECK(!tools::is_null(a) && a);
    BOOST_CHECK(tools::is_null(d) && !d);
    BOOST_CHECK(!tools::is_null(e) && e);
    BOOST_CHECK(tools::is_null(f) && !f);
    BOOST_CHECK(tools::is_null(tools::nullable_int()) && !tools::nullable_int());

    using tools::null;

    int ai = 13;
    tools::coalesce_trait<tools::null_type, tools::null_type, tools::null_type>::type x7 = null;
    BOOST_CHECK(!tools::is_null(1));
    BOOST_CHECK(!tools::is_null(ai));
    BOOST_CHECK(tools::is_null(tools::coalesce(null, null, null)));
    BOOST_CHECK(tools::is_null(x7));
    BOOST_CHECK(tools::is_null(null));
}

BOOST_AUTO_TEST_CASE(test_key_value_pair)
{
    std::unique_ptr<tools::connection_option> p(tools::make_key_value_pair(std::string("driver"), std::string("QIBASE")));
    BOOST_CHECK_EQUAL((p->key<std::string, std::string>()), "driver");
    BOOST_CHECK_EQUAL((p->value<std::string, std::string>()), std::string("QIBASE"));
}

BOOST_AUTO_TEST_CASE(test_coalesce)
{
    tools::coalesce_trait<int, double, float>::type x0(0);
    tools::coalesce_trait<tools::null_type, double, float>::type x1(0.0);
    tools::coalesce_trait<int, tools::null_type, float>::type x2(0);
    tools::coalesce_trait<tools::null_type, tools::null_type, float>::type x3(0.0f);
    tools::coalesce_trait<int, double, tools::null_type>::type x4(0);
    tools::coalesce_trait<tools::null_type, double, tools::null_type>::type x5(0.0f);
    tools::coalesce_trait<int, tools::null_type, tools::null_type>::type x6(0);

    using tools::null;

    BOOST_CHECK_EQUAL(tools::coalesce(1, 2, 3, 4), 1);
    BOOST_CHECK_EQUAL(tools::coalesce(null, 2, 3, 4), 2);
    BOOST_CHECK_EQUAL(tools::coalesce(1, null, null, null), 1);
    BOOST_CHECK_EQUAL(tools::coalesce(null, 2, null, 4), 2);

    BOOST_CHECK_EQUAL(x0, 0);
    BOOST_CHECK_EQUAL(x1, 0.0);
    BOOST_CHECK_EQUAL(x2, 0);
    BOOST_CHECK_EQUAL(x3, 0.0f);
    BOOST_CHECK_EQUAL(x4, 0);
    BOOST_CHECK_EQUAL(x5, 0.0f);
    BOOST_CHECK_EQUAL(x6, 0);
}

BOOST_AUTO_TEST_SUITE_END()
