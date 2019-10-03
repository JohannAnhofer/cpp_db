#include <boost/test/unit_test.hpp>

#include <stdexcept>

BOOST_AUTO_TEST_SUITE(test_test, *boost::unit_test::expected_failures(8))

BOOST_AUTO_TEST_CASE(test_test_functions, *boost::unit_test::expected_failures(3))
{
    BOOST_CHECK(true);
	BOOST_CHECK(false);
	BOOST_CHECK_EQUAL(3, 4711);
	BOOST_CHECK_EQUAL(4, 4);
	BOOST_CHECK_NE(3, 4711);
	BOOST_CHECK_NE(4, 4);
}

BOOST_AUTO_TEST_CASE(test_char_compare)
{
	char lca1[] = "World";
	char rca1[] = "World";
	const char lcca2[] = "World", rcca2[] = "World";
	const char *lccp3 = "World";
	const char *rccp3 = "World";
	const char * const lccpc4 = "World";
	const char * const rccpc4 = "World";

	BOOST_CHECK_EQUAL("World", "World");
	BOOST_CHECK_EQUAL("World", rca1);
	BOOST_CHECK_EQUAL("World", rcca2);
	BOOST_CHECK_EQUAL("World", rccp3);
	BOOST_CHECK_EQUAL("World", rccpc4);

	BOOST_CHECK_EQUAL(lca1, "World");
	BOOST_CHECK_EQUAL(lca1, rca1);
	BOOST_CHECK_EQUAL(lca1, rcca2);
	BOOST_CHECK_EQUAL(lca1, rccp3);
	BOOST_CHECK_EQUAL(lca1, rccpc4);

	BOOST_CHECK_EQUAL(lcca2, "World");
	BOOST_CHECK_EQUAL(lcca2, rca1);
	BOOST_CHECK_EQUAL(lcca2, rcca2);
	BOOST_CHECK_EQUAL(lcca2, rccp3);
	BOOST_CHECK_EQUAL(lcca2, rccpc4);

	BOOST_CHECK_EQUAL(lccp3, "World");
	BOOST_CHECK_EQUAL(lccp3, rca1);
	BOOST_CHECK_EQUAL(lccp3, rcca2);
	BOOST_CHECK_EQUAL(lccp3, rccp3);
	BOOST_CHECK_EQUAL(lccp3, rccpc4);

	BOOST_CHECK_EQUAL(lccpc4, "World");
	BOOST_CHECK_EQUAL(lccpc4, rca1);
	BOOST_CHECK_EQUAL(lccpc4, rcca2);
	BOOST_CHECK_EQUAL(lccpc4, rccp3);
	BOOST_CHECK_EQUAL(lccpc4, rccpc4);
}

BOOST_AUTO_TEST_CASE(test_wchar_t_compare)
{
	wchar_t lwa1[] = L"World";
	wchar_t rwa1[] = L"World";
	const wchar_t lcwa2[] = L"World", rcwa2[] = L"World";
	const wchar_t *lcwp3 = L"World";
	const wchar_t *rcwp3 = L"World";
	const wchar_t * const lcwpc4 = L"World";
	const wchar_t * const rcwpc4 = L"World";

	BOOST_CHECK_EQUAL(L"World", L"World");
	BOOST_CHECK_EQUAL(L"World", rwa1);
	BOOST_CHECK_EQUAL(L"World", rcwa2);
	BOOST_CHECK_EQUAL(L"World", rcwp3);
	BOOST_CHECK_EQUAL(L"World", rcwpc4);

	BOOST_CHECK_EQUAL(lwa1, L"World");
	BOOST_CHECK_EQUAL(lwa1, rwa1);
	BOOST_CHECK_EQUAL(lwa1, rcwa2);
	BOOST_CHECK_EQUAL(lwa1, rcwp3);
	BOOST_CHECK_EQUAL(lwa1, rcwpc4);

	BOOST_CHECK_EQUAL(lcwa2, L"World");
	BOOST_CHECK_EQUAL(lcwa2, rwa1);
	BOOST_CHECK_EQUAL(lcwa2, rcwa2);
	BOOST_CHECK_EQUAL(lcwa2, rcwp3);
	BOOST_CHECK_EQUAL(lcwa2, rcwpc4);

	BOOST_CHECK_EQUAL(lcwp3, L"World");
	BOOST_CHECK_EQUAL(lcwp3, rwa1);
	BOOST_CHECK_EQUAL(lcwp3, rcwa2);
	BOOST_CHECK_EQUAL(lcwp3, rcwp3);
	BOOST_CHECK_EQUAL(lcwp3, rcwpc4);

	BOOST_CHECK_EQUAL(lcwpc4, L"World");
	BOOST_CHECK_EQUAL(lcwpc4, rwa1);
	BOOST_CHECK_EQUAL(lcwpc4, rcwa2);
	BOOST_CHECK_EQUAL(lcwpc4, rcwp3);
	BOOST_CHECK_EQUAL(lcwpc4, rcwpc4);
}

BOOST_AUTO_TEST_CASE(test_char_neq_compare)
{
	char lca1[] = "Hello";
	char rca1[] = "World";
	const char lcca2[] = "Hello", rcca2[] = "World";
	const char *lccp3 = "Hello";
	const char *rccp3 = "World";
	const char * const lccpc4 = "Hello";
	const char * const rccpc4 = "World";

	BOOST_CHECK_NE("Hello", "World");
	BOOST_CHECK_NE("Hello", rca1);
	BOOST_CHECK_NE("Hello", rcca2);
	BOOST_CHECK_NE("Hello", rccp3);
	BOOST_CHECK_NE("Hello", rccpc4);

	BOOST_CHECK_NE(lca1, "World");
	BOOST_CHECK_NE(lca1, rca1);
	BOOST_CHECK_NE(lca1, rcca2);
	BOOST_CHECK_NE(lca1, rccp3);
	BOOST_CHECK_NE(lca1, rccpc4);

	BOOST_CHECK_NE(lcca2, "World");
	BOOST_CHECK_NE(lcca2, rca1);
	BOOST_CHECK_NE(lcca2, rcca2);
	BOOST_CHECK_NE(lcca2, rccp3);
	BOOST_CHECK_NE(lcca2, rccpc4);

	BOOST_CHECK_NE(lccp3, "World");
	BOOST_CHECK_NE(lccp3, rca1);
	BOOST_CHECK_NE(lccp3, rcca2);
	BOOST_CHECK_NE(lccp3, rccp3);
	BOOST_CHECK_NE(lccp3, rccpc4);

	BOOST_CHECK_NE(lccpc4, "World");
	BOOST_CHECK_NE(lccpc4, rca1);
	BOOST_CHECK_NE(lccpc4, rcca2);
	BOOST_CHECK_NE(lccpc4, rccp3);
	BOOST_CHECK_NE(lccpc4, rccpc4);
}

BOOST_AUTO_TEST_CASE(test_wchar_t_neq_compare)
{
	wchar_t lwa1[] = L"Hello";
	wchar_t rwa1[] = L"World";
	const wchar_t lcwa2[] = L"Hello", rcwa2[] = L"World";
	const wchar_t *lcwp3 = L"Hello";
	const wchar_t *rcwp3 = L"World";
	const wchar_t * const lcwpc4 = L"Hello";
	const wchar_t * const rcwpc4 = L"World";

	BOOST_CHECK_NE(L"Hello", rcwa2);
	BOOST_CHECK_NE(L"Hello", L"World");
	BOOST_CHECK_NE(L"Hello", rwa1);
	BOOST_CHECK_NE(L"Hello", rcwp3);
	BOOST_CHECK_NE(L"Hello", rcwpc4);

	BOOST_CHECK_NE(lwa1, L"World");
	BOOST_CHECK_NE(lwa1, rwa1);
	BOOST_CHECK_NE(lwa1, rcwa2);
	BOOST_CHECK_NE(lwa1, rcwp3);
	BOOST_CHECK_NE(lwa1, rcwpc4);

	BOOST_CHECK_NE(lcwa2, L"World");
	BOOST_CHECK_NE(lcwa2, rwa1);
	BOOST_CHECK_NE(lcwa2, rcwa2);
	BOOST_CHECK_NE(lcwa2, rcwp3);
	BOOST_CHECK_NE(lcwa2, rcwpc4);

	BOOST_CHECK_NE(lcwp3, L"World");
	BOOST_CHECK_NE(lcwp3, rwa1);
	BOOST_CHECK_NE(lcwp3, rcwa2);
	BOOST_CHECK_NE(lcwp3, rcwp3);
	BOOST_CHECK_NE(lcwp3, rcwpc4);

	BOOST_CHECK_NE(lcwpc4, L"World");
	BOOST_CHECK_NE(lcwpc4, rwa1);
	BOOST_CHECK_NE(lcwpc4, rcwa2);
	BOOST_CHECK_NE(lcwpc4, rcwp3);
    BOOST_CHECK_NE(lcwpc4, rcwpc4);
}

struct A
{
    explicit A(bool do_throw)
    {
        if (do_throw) throw std::runtime_error("This is a test!");
    }
    bool operator==(const A &) const
    {
        return true;
    }
    friend std::ostream & operator<<(std::ostream &out, const A &a)
    {
        out << "A{}";
        return out;
    }
};

BOOST_AUTO_TEST_CASE(test_exceptions, *boost::unit_test::expected_failures(5))
{
    BOOST_CHECK(true);
    BOOST_CHECK_EQUAL(A(false), A(true));
    BOOST_CHECK_NE(A(true), A(false));
	BOOST_CHECK(([]() -> bool {A a(true), b(false); return a == b;}()));
	BOOST_CHECK_NO_THROW([] {A(true); }());
	BOOST_CHECK_THROW([] {A(true); }(), std::exception);
	BOOST_CHECK_THROW([] {A(false); }(), std::exception);
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
