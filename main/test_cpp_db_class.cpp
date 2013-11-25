#include "test_cpp_db_class.h"

#include "null.h"
#include "value.h"
#include "parameter.h"
#include "type_of.h"

#include <stdexcept>
#include <cmath>

void test_cpp_db_class::test_is_null()
{
	cpp_db::null_type null;

    TEST_VERIFY(cpp_db::is_null(null));
	TEST_VERIFY(!cpp_db::is_null(1));

	cpp_db::value vnull(null);
    TEST_VERIFY(cpp_db::is_null(vnull));

	cpp_db::parameter pnull(0, null);
    TEST_VERIFY(cpp_db::is_null(pnull));

    TEST_FOR_EXCEPTION(cpp_db::type_of(null), std::logic_error);
    TEST_FOR_EXCEPTION(cpp_db::type_of(vnull), std::logic_error);
    TEST_FOR_EXCEPTION(cpp_db::type_of(pnull), std::logic_error);

    TEST_FOR_EXCEPTION(cpp_db::value_of(null), std::logic_error);
    TEST_FOR_EXCEPTION(cpp_db::value_of<int>(vnull), std::logic_error);
    TEST_FOR_EXCEPTION(cpp_db::value_of<int>(pnull), std::logic_error);
}

void test_cpp_db_class::test_value()
{
    TEST_FOR_NO_EXCEPTION((cpp_db::cast_to<int, int64_t>(cpp_db::value(10))));
    TEST_FOR_NO_EXCEPTION((cpp_db::cast_to<int, double>(cpp_db::value(10))));
    TEST_FOR_NO_EXCEPTION((cpp_db::cast_to<double, int64_t>(cpp_db::value(10.0))));
    TEST_FOR_NO_EXCEPTION((cpp_db::cast_to<double, int64_t>(cpp_db::value(10.0))));
    TEST_FOR_NO_EXCEPTION((cpp_db::cast_to<long double, float>(cpp_db::value(10.0l))));

    TEST_EQUAL(cpp_db::type_of(10), typeid(int));
    TEST_EQUAL(cpp_db::type_of(cpp_db::value{10}), typeid(int));
	TEST_EQUAL(cpp_db::type_of(cpp_db::parameter{ 0, 10 }), typeid(int));
}

void test_cpp_db_class::test_parameter()
{
	cpp_db::parameter param1(0, std::string("Hello world!")), param2(std::string("@test"), 27.85);

	TEST_VERIFY(param1.has_index());
	TEST_VERIFY(!param1.has_name());
	TEST_VERIFY(!param2.has_index());
	TEST_VERIFY(param2.has_name());
    TEST_EQUAL(cpp_db::type_of(param1), typeid(std::string));
    TEST_NOT_EQUAL(cpp_db::type_of(param1), typeid(double));
    TEST_NOT_EQUAL(cpp_db::type_of(param2), typeid(std::string));
    TEST_EQUAL(cpp_db::type_of(param2), typeid(double));

    TEST_EQUAL(cpp_db::value_of<std::string>(param1), "Hello world!");
    TEST_VERIFY(std::fabs(cpp_db::value_of<double>(param2) - 27.85) < 0.000001);

    TEST_FOR_EXCEPTION(cpp_db::value_of<int>(param1), std::runtime_error);
}

void test_cpp_db_class::test_conversions_i8()
{
    cpp_db::value val{int8_t{111}};

    TEST_EQUAL(cpp_db::value_of<int8_t>(val), 111);
    TEST_EQUAL(cpp_db::value_of<uint8_t>(val), 111);
    TEST_EQUAL(cpp_db::value_of<int16_t>(val), 111);
    TEST_EQUAL(cpp_db::value_of<uint16_t>(val), 111);
    TEST_EQUAL(cpp_db::value_of<int32_t>(val), 111);
    TEST_EQUAL(cpp_db::value_of<uint32_t>(val), 111);
    TEST_EQUAL(cpp_db::value_of<int64_t>(val), 111);
    TEST_EQUAL(cpp_db::value_of<uint64_t>(val), 111);
    TEST_VERIFY(std::fabs(cpp_db::value_of<float>(val) - 111) < 1e-6);
    TEST_VERIFY(std::fabs(cpp_db::value_of<double>(val) - 111) < 1e-6);
    TEST_VERIFY(std::fabs(cpp_db::value_of<long double>(val) - 111) < 1e-6);
}

void  test_cpp_db_class::test_conversions_ui8()
{
    cpp_db::value val{uint8_t{200}};

    TEST_NOT_EQUAL(cpp_db::value_of<int8_t>(val), 200);

    TEST_EQUAL(cpp_db::value_of<int8_t>(val), -56);

    TEST_EQUAL(cpp_db::value_of<uint8_t>(val), 200);
    TEST_EQUAL(cpp_db::value_of<int16_t>(val), 200);
    TEST_EQUAL(cpp_db::value_of<uint16_t>(val), 200);
    TEST_EQUAL(cpp_db::value_of<int32_t>(val), 200);
    TEST_EQUAL(cpp_db::value_of<uint32_t>(val), 200);
    TEST_EQUAL(cpp_db::value_of<int64_t>(val), 200);
    TEST_EQUAL(cpp_db::value_of<uint64_t>(val), 200);
    TEST_VERIFY(std::fabs(cpp_db::value_of<float>(val) - 200) < 1e-6);
    TEST_VERIFY(std::fabs(cpp_db::value_of<double>(val) - 200) < 1e-6);
    TEST_VERIFY(std::fabs(cpp_db::value_of<long double>(val) - 200) < 1e-6);
}

void test_cpp_db_class::test_conversions_i16()
{
    cpp_db::value val{int16_t{25287}};

    TEST_NOT_EQUAL(cpp_db::value_of<int8_t>(val), 25287);
    TEST_NOT_EQUAL(cpp_db::value_of<uint8_t>(val), 25287);

    TEST_EQUAL(cpp_db::value_of<int8_t>(val), -57);
    TEST_EQUAL(cpp_db::value_of<uint8_t>(val), 199);

    TEST_EQUAL(cpp_db::value_of<int16_t>(val), 25287);
    TEST_EQUAL(cpp_db::value_of<uint16_t>(val), 25287);
    TEST_EQUAL(cpp_db::value_of<int32_t>(val), 25287);
    TEST_EQUAL(cpp_db::value_of<uint32_t>(val), 25287);
    TEST_EQUAL(cpp_db::value_of<int64_t>(val), 25287);
    TEST_EQUAL(cpp_db::value_of<uint64_t>(val), 25287);
    TEST_VERIFY(std::fabs(cpp_db::value_of<float>(val) - 25287) < 1e-6);
    TEST_VERIFY(std::fabs(cpp_db::value_of<double>(val) - 25287) < 1e-6);
    TEST_VERIFY(std::fabs(cpp_db::value_of<long double>(val) - 25287) < 1e-6);
}

void test_cpp_db_class::test_conversions_ui16()
{
    cpp_db::value val{uint16_t{55555}};

    TEST_NOT_EQUAL(cpp_db::value_of<int8_t>(val), 55555);
    TEST_NOT_EQUAL(cpp_db::value_of<uint8_t>(val), 55555);
    TEST_NOT_EQUAL(cpp_db::value_of<int16_t>(val), 55555);

    TEST_EQUAL(cpp_db::value_of<int8_t>(val), 3);
    TEST_EQUAL(cpp_db::value_of<uint8_t>(val), 3);
    TEST_EQUAL(cpp_db::value_of<int16_t>(val), -9981);

    TEST_EQUAL(cpp_db::value_of<uint16_t>(val), 55555);
    TEST_EQUAL(cpp_db::value_of<int32_t>(val), 55555);
    TEST_EQUAL(cpp_db::value_of<uint32_t>(val), 55555);
    TEST_EQUAL(cpp_db::value_of<int64_t>(val), 55555);
    TEST_EQUAL(cpp_db::value_of<uint64_t>(val), 55555);
    TEST_VERIFY(std::fabs(cpp_db::value_of<float>(val) - 55555) < 1e-6);
    TEST_VERIFY(std::fabs(cpp_db::value_of<double>(val) - 55555) < 1e-6);
    TEST_VERIFY(std::fabs(cpp_db::value_of<long double>(val) - 55555) < 1e-6);
}

void test_cpp_db_class::test_conversions_i32()
{
    cpp_db::value val{int32_t{1111111111}};

    TEST_NOT_EQUAL(cpp_db::value_of<int8_t>(val), 1111111111);
    TEST_NOT_EQUAL(cpp_db::value_of<uint8_t>(val), 1111111111);
    TEST_NOT_EQUAL(cpp_db::value_of<int16_t>(val), 1111111111);
    TEST_NOT_EQUAL(cpp_db::value_of<uint16_t>(val), 1111111111);
    TEST_VERIFY(std::fabs(cpp_db::value_of<float>(val) - 1111111111) > 1e-6);

    TEST_EQUAL(cpp_db::value_of<int8_t>(val), -57);
    TEST_EQUAL(cpp_db::value_of<uint8_t>(val), 199);
    TEST_EQUAL(cpp_db::value_of<int16_t>(val), 13767);
    TEST_EQUAL(cpp_db::value_of<uint16_t>(val), 13767);

    TEST_EQUAL(cpp_db::value_of<int32_t>(val), 1111111111);
    TEST_EQUAL(cpp_db::value_of<uint32_t>(val), 1111111111);
    TEST_EQUAL(cpp_db::value_of<int64_t>(val), 1111111111);
    TEST_EQUAL(cpp_db::value_of<uint64_t>(val), 1111111111);
    TEST_VERIFY(std::fabs(cpp_db::value_of<double>(val) - 1111111111) < 1e-6);
    TEST_VERIFY(std::fabs(cpp_db::value_of<long double>(val) - 1111111111) < 1e-6);
}

void test_cpp_db_class::test_conversions_ui32()
{
    cpp_db::value val{uint32_t{2222222222}};

    TEST_NOT_EQUAL(cpp_db::value_of<int8_t>(val), 2222222222);
    TEST_NOT_EQUAL(cpp_db::value_of<uint8_t>(val), 2222222222);
    TEST_NOT_EQUAL(cpp_db::value_of<int16_t>(val), 2222222222);
    TEST_NOT_EQUAL(cpp_db::value_of<uint16_t>(val), 2222222222);
    TEST_NOT_EQUAL(cpp_db::value_of<int32_t>(val), 2222222222);
    TEST_VERIFY(std::fabs(cpp_db::value_of<float>(val) - 2222222222) > 1e-6);

    TEST_EQUAL(cpp_db::value_of<int8_t>(val), -114);
    TEST_EQUAL(cpp_db::value_of<uint8_t>(val), 142);
    TEST_EQUAL(cpp_db::value_of<int16_t>(val), 27534);
    TEST_EQUAL(cpp_db::value_of<uint16_t>(val), 27534);
    TEST_EQUAL(cpp_db::value_of<int32_t>(val), -2072745074);

    TEST_EQUAL(cpp_db::value_of<uint32_t>(val), 2222222222);
    TEST_EQUAL(cpp_db::value_of<int64_t>(val), 2222222222);
    TEST_EQUAL(cpp_db::value_of<uint64_t>(val), 2222222222);
    TEST_VERIFY(std::fabs(cpp_db::value_of<double>(val) - 2222222222) < 1e-6);
    TEST_VERIFY(std::fabs(cpp_db::value_of<long double>(val) - 2222222222) < 1e-6);
}

void test_cpp_db_class::test_conversions_i64()
{
    cpp_db::value val{uint64_t{5555555555}};

    TEST_NOT_EQUAL(cpp_db::value_of<int8_t>(val), 5555555555);
    TEST_NOT_EQUAL(cpp_db::value_of<uint8_t>(val), 5555555555);
    TEST_NOT_EQUAL(cpp_db::value_of<int16_t>(val), 5555555555);
    TEST_NOT_EQUAL(cpp_db::value_of<uint16_t>(val), 5555555555);
    TEST_NOT_EQUAL(cpp_db::value_of<int32_t>(val), 5555555555);
    TEST_NOT_EQUAL(cpp_db::value_of<uint32_t>(val), 5555555555);

//    TEST_VERIFY(std::fabs(cpp_db::value_of<float>(val) - 5555555555) > 1e-6);

    TEST_EQUAL(cpp_db::value_of<int8_t>(val), -29);
    TEST_EQUAL(cpp_db::value_of<uint8_t>(val), 227);
    TEST_EQUAL(cpp_db::value_of<int16_t>(val), 3299);
    TEST_EQUAL(cpp_db::value_of<uint16_t>(val), 3299);
    TEST_EQUAL(cpp_db::value_of<int32_t>(val), 1260588259);
    TEST_EQUAL(cpp_db::value_of<uint32_t>(val), 1260588259);

    TEST_EQUAL(cpp_db::value_of<int64_t>(val), 5555555555);
    TEST_EQUAL(cpp_db::value_of<uint64_t>(val), 5555555555);
    TEST_VERIFY(std::fabs(cpp_db::value_of<double>(val) - 5555555555) < 1e-6);
    TEST_VERIFY(std::fabs(cpp_db::value_of<long double>(val) - 5555555555) < 1e-6);
}

void test_cpp_db_class::test_conversions_ui64()
{
    cpp_db::value val{uint64_t{0xccccaaaaeeeeffff}};

    TEST_NOT_EQUAL(cpp_db::value_of<int8_t>(val), 0xccccaaaaeeeeffff);
    TEST_NOT_EQUAL(cpp_db::value_of<uint8_t>(val), 0xccccaaaaeeeeffff);
    TEST_NOT_EQUAL(cpp_db::value_of<int16_t>(val), 0xccccaaaaeeeeffff);
    TEST_NOT_EQUAL(cpp_db::value_of<uint16_t>(val), 0xccccaaaaeeeeffff);
    TEST_NOT_EQUAL(cpp_db::value_of<int32_t>(val), 0xccccaaaaeeeeffff);
    TEST_NOT_EQUAL(cpp_db::value_of<uint32_t>(val), 0xccccaaaaeeeeffff);
    next_test_is_expected_to_fail("uint64_t converts silently to int64_t?");
    TEST_NOT_EQUAL(cpp_db::value_of<int64_t>(val), 0xccccaaaaeeeeffff);

//    TEST_VERIFY(std::fabs(cpp_db::value_of<float>(val) - 0xccccaaaaeeeeffff) > 1e-6);

    TEST_EQUAL(cpp_db::value_of<int8_t>(val), -1);
    TEST_EQUAL(cpp_db::value_of<uint8_t>(val), 255);
    TEST_EQUAL(cpp_db::value_of<int16_t>(val), -1);
    TEST_EQUAL(cpp_db::value_of<uint16_t>(val), 65535);
    TEST_EQUAL(cpp_db::value_of<int32_t>(val), -286326785);
    TEST_EQUAL(cpp_db::value_of<uint32_t>(val), 4008640511);
    next_test_is_expected_to_fail("uint64_t converts silently to int64_t?");
    TEST_EQUAL(cpp_db::value_of<int64_t>(val), -3333555511110001);

    TEST_EQUAL(cpp_db::value_of<uint64_t>(val), 0xccccaaaaeeeeffff);
    TEST_VERIFY(std::fabs(cpp_db::value_of<double>(val) - 0xccccaaaaeeeeffff) < 1e-6);
    TEST_VERIFY(std::fabs(cpp_db::value_of<long double>(val) - 0xccccaaaaeeeeffff) < 1e-6);
}

void test_cpp_db_class::test_conversions_strings()
{
    cpp_db::value val_s{std::string("is a string")};
    cpp_db::value val_ccs{"is a const char *"};
    char cs[] = "is a char *";
    cpp_db::value val_cs{cs};

    cpp_db::value val_ws{std::wstring(L"is a wstring")};
    cpp_db::value val_cws{L"is a const wchar_t *"};
    wchar_t wcs[] = L"is a wchar_t *";
    cpp_db::value val_wcs{wcs};

    TEST_EQUAL(cpp_db::value_of<std::string>(val_s), std::string("is a string"));
    TEST_EQUAL(cpp_db::value_of<std::string>(val_ccs), std::string("is a const char *"));
    TEST_EQUAL(cpp_db::value_of<std::string>(val_cs), std::string("is a char *"));

    TEST_EQUAL(cpp_db::value_of<std::wstring>(val_ws), std::wstring(L"is a wstring"));
    TEST_EQUAL(cpp_db::value_of<std::wstring>(val_cws), std::wstring(L"is a const wchar_t *"));
    TEST_EQUAL(cpp_db::value_of<std::wstring>(val_wcs), std::wstring(L"is a wchar_t *"));
}
