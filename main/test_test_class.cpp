#include "test_test_class.h"

void test_test_class::test_test_functions()
{
    TEST_VERIFY(true);
    TEST_VERIFY(false);
    TEST_VERIFY([]() -> bool {throw std::runtime_error("This is a test!");}());
	TEST_EQUAL(3, 4711);
	TEST_EQUAL(4, 4);
	TEST_NOT_EQUAL(3, 4711);
	TEST_NOT_EQUAL(4, 4);
}

void test_test_class::test_char_compare()
{
	char lca1[] = "World";
	char rca1[] = "World";
	const char lcca2[] = "World", rcca2[] = "World";
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

void test_test_class::test_wchar_t_compare()
{
	wchar_t lwa1[] = L"World";
	wchar_t rwa1[] = L"World";
	const wchar_t lcwa2[] = L"World", rcwa2[] = L"World";
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

void test_test_class::test_char_neq_compare()
{
	char lca1[] = "Hello";
	char rca1[] = "World";
	const char lcca2[] = "Hello", rcca2[] = "World";
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

void test_test_class::test_wchar_t_neq_compare()
{
	wchar_t lwa1[] = L"Hello";
	wchar_t rwa1[] = L"World";
	const wchar_t lcwa2[] = L"Hello", rcwa2[] = L"World";
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

