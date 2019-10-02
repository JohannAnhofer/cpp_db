#define BOOST_TEST_MODULE cpp_db
#include <boost/test/unit_test.hpp>

#include "null.h"
#include "value.h"
#include "parameter.h"
#include "type_of.h"

#include <stdexcept>
#include <cmath>
#include <iostream>
#include <cstring>

namespace std
{
	ostream& operator << (ostream& os, type_index ti)
	{
		os << "{" << ti.name() << ", " << ti.hash_code() << "}";
		return os;
	}

	ostream& operator << (ostream& os, const std::wstring &ws)
	{
		return os;
	}
}

BOOST_AUTO_TEST_SUITE(test_cpp_db)

BOOST_AUTO_TEST_CASE(test_is_null)
{
	cpp_db::null_type null;

    BOOST_CHECK(cpp_db::is_null(null));
	BOOST_CHECK(!cpp_db::is_null(1));

	cpp_db::value vnull(null);
	BOOST_CHECK(cpp_db::is_null(vnull));

	cpp_db::parameter pnull(0, null);
	BOOST_CHECK(cpp_db::is_null(pnull));

	BOOST_CHECK_THROW(cpp_db::type_of(null), std::logic_error);
	BOOST_CHECK_THROW(cpp_db::type_of(vnull), std::logic_error);
	BOOST_CHECK_THROW(cpp_db::type_of(pnull), std::logic_error);

	BOOST_CHECK_THROW(cpp_db::value_of(null), std::logic_error);
	BOOST_CHECK_THROW(cpp_db::value_of<int>(vnull), std::logic_error);
	BOOST_CHECK_THROW(cpp_db::value_of<int>(pnull), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_type)
{
    BOOST_CHECK_EQUAL(cpp_db::type_of(10), typeid(int));
    BOOST_CHECK_EQUAL(cpp_db::type_of(cpp_db::value{10}), typeid(int));
	BOOST_CHECK_EQUAL(cpp_db::type_of(cpp_db::parameter{ 0, 10 }), typeid(int));
}

BOOST_AUTO_TEST_CASE(test_parameter)
{
	cpp_db::parameter param1(0, std::string("Hello world!")), param2(std::string("@test"), 27.85);

	BOOST_CHECK(param1.has_index());
	BOOST_CHECK(!param1.has_name());
	BOOST_CHECK(!param2.has_index());
	BOOST_CHECK(param2.has_name());
    BOOST_CHECK_EQUAL(cpp_db::type_of(param1), typeid(std::string));
    BOOST_CHECK_NE(cpp_db::type_of(param1), typeid(double));
    BOOST_CHECK_NE(cpp_db::type_of(param2), typeid(std::string));
    BOOST_CHECK_EQUAL(cpp_db::type_of(param2), typeid(double));

    BOOST_CHECK_EQUAL(cpp_db::value_of<std::string>(param1), "Hello world!");
	BOOST_CHECK(std::fabs(cpp_db::value_of<double>(param2) - 27.85) < 0.000001);

	BOOST_CHECK_THROW(cpp_db::value_of<int>(param1), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_conversions_i8)
{
    cpp_db::value val{int8_t{111}};
    int8_t vi8{111};
    uint8_t vui8{111};
    int16_t vi16{111};
    uint16_t vui16{111};
    int32_t vi32{111};
    uint32_t vui32{111};
    int64_t vi64{111};
    uint64_t vui64{111};
    float vf{111.0f};
    double vd{111.0};
    long double vld{111.0l};

    BOOST_CHECK_EQUAL(cpp_db::value_of<int8_t>(val), vi8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint8_t>(val), vui8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int16_t>(val), vi16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint16_t>(val), vui16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int32_t>(val), vi32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint32_t>(val), vui32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(val), vi64);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint64_t>(val), vui64);
	BOOST_CHECK(std::fabs(cpp_db::value_of<float>(val) - vf) < 1e-6);
	BOOST_CHECK(std::fabs(cpp_db::value_of<double>(val) - vd) < 1e-6);
	BOOST_CHECK(std::fabs(cpp_db::value_of<long double>(val) - vld) < 1e-6);
}

BOOST_AUTO_TEST_CASE(test_conversions_ui8)
{
    cpp_db::value val{uint8_t{200}};
    int8_t vi8{-56};
    uint8_t vui8{200};
    int16_t vi16{200};
    uint16_t vui16{200};
    int32_t vi32{200};
    uint32_t vui32{200};
    int64_t vi64{200};
    uint64_t vui64{200};
    float vf{200.0f};
    double vd{200.0};
    long double vld{200.0l};

    BOOST_CHECK_EQUAL(cpp_db::value_of<int8_t>(val), vi8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint8_t>(val), vui8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int16_t>(val), vi16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint16_t>(val), vui16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int32_t>(val), vi32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint32_t>(val), vui32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(val), vi64);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint64_t>(val), vui64);
    BOOST_CHECK(std::fabs(cpp_db::value_of<float>(val) - vf) < 1e-6);
    BOOST_CHECK(std::fabs(cpp_db::value_of<double>(val) - vd) < 1e-6);
    BOOST_CHECK(std::fabs(cpp_db::value_of<long double>(val) - vld) < 1e-6);
}

BOOST_AUTO_TEST_CASE(test_conversions_i16)
{
    cpp_db::value val{int16_t{25287}};
    int8_t vi8{-57};
    uint8_t vui8{199};
    int16_t vi16{25287};
    uint16_t vui16{25287};
    int32_t vi32{25287};
    uint32_t vui32{25287};
    int64_t vi64{25287};
    uint64_t vui64{25287};
    float vf{25287.0f};
    double vd{25287.0};
    long double vld{25287.0l};

    BOOST_CHECK_EQUAL(cpp_db::value_of<int8_t>(val), vi8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint8_t>(val), vui8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int16_t>(val), vi16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint16_t>(val), vui16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int32_t>(val), vi32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint32_t>(val), vui32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(val), vi64);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint64_t>(val), vui64);
    BOOST_CHECK(std::fabs(cpp_db::value_of<float>(val) - vf) < 1e-6);
    BOOST_CHECK(std::fabs(cpp_db::value_of<double>(val) - vd) < 1e-6);
    BOOST_CHECK(std::fabs(cpp_db::value_of<long double>(val) - vld) < 1e-6);
}

BOOST_AUTO_TEST_CASE(test_conversions_ui16)
{
    cpp_db::value val{uint16_t{55555}};
    int8_t vi8{3};
    uint8_t vui8{3};
    int16_t vi16{-9981};
    uint16_t vui16{55555};
    int32_t vi32{55555};
    uint32_t vui32{55555};
    int64_t vi64{55555};
    uint64_t vui64{55555};
    float vf{55555.0f};
    double vd{55555.0};
    long double vld{55555.0l};

    BOOST_CHECK_EQUAL(cpp_db::value_of<int8_t>(val), vi8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint8_t>(val), vui8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int16_t>(val), vi16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint16_t>(val), vui16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int32_t>(val), vi32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint32_t>(val), vui32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(val), vi64);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint64_t>(val), vui64);
    BOOST_CHECK(std::fabs(cpp_db::value_of<float>(val) - vf) < 1e-6);
    BOOST_CHECK(std::fabs(cpp_db::value_of<double>(val) - vd) < 1e-6);
    BOOST_CHECK(std::fabs(cpp_db::value_of<long double>(val) - vld) < 1e-6);
}

BOOST_AUTO_TEST_CASE(test_conversions_i32)
{
    cpp_db::value val{int32_t{1111111111}};
    int8_t vi8{-57};
    uint8_t vui8{199};
    int16_t vi16{13767};
    uint16_t vui16{13767};
    int32_t vi32{1111111111};
    uint32_t vui32{1111111111};
    int64_t vi64{1111111111};
    uint64_t vui64{1111111111};
    float vf{1111111111.0f};
    double vd{1111111111.0};
    long double vld{1111111111.0l};

    BOOST_CHECK_EQUAL(cpp_db::value_of<int8_t>(val), vi8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint8_t>(val), vui8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int16_t>(val), vi16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint16_t>(val), vui16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int32_t>(val), vi32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint32_t>(val), vui32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(val), vi64);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint64_t>(val), vui64);
    BOOST_CHECK(std::fabs(cpp_db::value_of<float>(val) - vf) < 1e-6);
    BOOST_CHECK(std::fabs(cpp_db::value_of<double>(val) - vd) < 1e-6);
    BOOST_CHECK(std::fabs(cpp_db::value_of<long double>(val) - vld) < 1e-6);
}

BOOST_AUTO_TEST_CASE(test_conversions_ui32)
{
    cpp_db::value val{uint32_t{2222222222}};
    int8_t vi8{-114};
    uint8_t vui8{142};
    int16_t vi16{27534};
    uint16_t vui16{27534};
    int32_t vi32{-2072745074};
    uint32_t vui32{2222222222};
    int64_t vi64{2222222222};
    uint64_t vui64{2222222222};
    float vf{2222222222.0f};
    double vd{2222222222.0};
    long double vld{2222222222.0l};

    BOOST_CHECK_EQUAL(cpp_db::value_of<int8_t>(val), vi8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint8_t>(val), vui8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int16_t>(val), vi16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint16_t>(val), vui16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int32_t>(val), vi32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint32_t>(val), vui32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(val), vi64);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint64_t>(val), vui64);
    BOOST_CHECK(std::fabs(cpp_db::value_of<float>(val) - vf) < 1e-6);
    BOOST_CHECK(std::fabs(cpp_db::value_of<double>(val) - vd) < 1e-6);
    BOOST_CHECK(std::fabs(cpp_db::value_of<long double>(val) - vld) < 1e-6);
}

BOOST_AUTO_TEST_CASE(test_conversions_i64)
{
    cpp_db::value val{int64_t{5555555555}};
    int8_t vi8{-29};
    uint8_t vui8{227};
    int16_t vi16{3299};
    uint16_t vui16{3299};
    int32_t vi32{1260588259};
    uint32_t vui32{1260588259};
    int64_t vi64{5555555555};
    uint64_t vui64{5555555555};
    float vf{5555555555.0f};
    double vd{5555555555.0};
    long double vld{5555555555.0l};

    BOOST_CHECK_EQUAL(cpp_db::value_of<int8_t>(val), vi8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint8_t>(val), vui8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int16_t>(val), vi16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint16_t>(val), vui16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int32_t>(val), vi32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint32_t>(val), vui32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(val), vi64);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint64_t>(val), vui64);
    BOOST_CHECK(std::fabs(cpp_db::value_of<float>(val) - vf) < 1e-6);
    BOOST_CHECK(std::fabs(cpp_db::value_of<double>(val) - vd) < 1e-6);
    BOOST_CHECK(std::fabs(cpp_db::value_of<long double>(val) - vld) < 1e-6);
}

BOOST_AUTO_TEST_CASE(test_conversions_ui64)
{
    cpp_db::value val{uint64_t{14757357730116075519u}};
    int8_t vi8{-1};
    uint8_t vui8{255};
    int16_t vi16{-1};
    uint16_t vui16{65535};
    int32_t vi32{-286326785};
    uint32_t vui32{4008640511};
    int64_t vi64{-3689386343593476097};
    uint64_t vui64{14757357730116075519u};
    float vf{14757357730116075519.0f};
    double vd{14757357730116075519.0};
    long double vld{14757357730116075519.0l};

    BOOST_CHECK_EQUAL(cpp_db::value_of<int8_t>(val), vi8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint8_t>(val), vui8);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int16_t>(val), vi16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint16_t>(val), vui16);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int32_t>(val), vi32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint32_t>(val), vui32);
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(val), vi64);
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint64_t>(val), vui64);
    BOOST_CHECK(std::fabs(cpp_db::value_of<float>(val) - vf) < 1e-6);
    BOOST_CHECK(std::fabs(cpp_db::value_of<double>(val) - vd) < 1e-6);
    BOOST_CHECK(std::fabs(cpp_db::value_of<long double>(val) - vld) < 1e-6);
}

BOOST_AUTO_TEST_CASE(test_conversions_floating_point)
{
    cpp_db::value val_f{133.97f}, val_d{4711.0815}, val_ld{15.323l};

    float vf{133.97f};
    double vd{133.97};
    long double vld{133.97l};
    BOOST_CHECK_EQUAL(cpp_db::value_of<int8_t>(val_f), int8_t{-123});
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint8_t>(val_f), uint8_t{133});
    BOOST_CHECK_EQUAL(cpp_db::value_of<int16_t>(val_f), int16_t{133});
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint16_t>(val_f), uint16_t{133});
    BOOST_CHECK_EQUAL(cpp_db::value_of<int32_t>(val_f), int32_t{133});
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint32_t>(val_f), uint32_t{133});
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(val_f), int64_t{133});
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint64_t>(val_f), uint64_t{133});
    BOOST_CHECK(std::fabs(cpp_db::value_of<float>(val_f) - vf) < 1e-5);
    BOOST_CHECK(std::fabs(cpp_db::value_of<double>(val_f) - vd) < 1e-5);
    BOOST_CHECK(std::fabs(cpp_db::value_of<long double>(val_f) - vld) < 1e-5);

    vf = 4711.0815f;
    vd = 4711.0815;
    vld = 4711.0815l;
    BOOST_CHECK_EQUAL(cpp_db::value_of<int8_t>(val_d), int8_t{103});
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint8_t>(val_d), uint8_t{103});
    BOOST_CHECK_EQUAL(cpp_db::value_of<int16_t>(val_d), int16_t{4711});
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint16_t>(val_d), uint16_t{4711});
    BOOST_CHECK_EQUAL(cpp_db::value_of<int32_t>(val_d), int32_t{4711});
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint32_t>(val_d), uint32_t{4711});
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(val_d), int64_t{4711});
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint64_t>(val_d), uint64_t{4711});
    BOOST_CHECK(std::fabs(cpp_db::value_of<float>(val_d) - vf) < 1e-5);
    BOOST_CHECK(std::fabs(cpp_db::value_of<double>(val_d) - vd) < 1e-5);
    BOOST_CHECK(std::fabs(cpp_db::value_of<long double>(val_d) - vld) < 1e-5);

    vf = 15.323f;
    vd = 15.323;
    vld = 15.323l;
    BOOST_CHECK_EQUAL(cpp_db::value_of<int8_t>(val_ld), int8_t{15});
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint8_t>(val_ld), uint8_t{15});
    BOOST_CHECK_EQUAL(cpp_db::value_of<int16_t>(val_ld), int16_t{15});
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint16_t>(val_ld), uint16_t{15});
    BOOST_CHECK_EQUAL(cpp_db::value_of<int32_t>(val_ld), int32_t{15});
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint32_t>(val_ld), uint32_t{15});
    BOOST_CHECK_EQUAL(cpp_db::value_of<int64_t>(val_ld), int64_t{15});
    BOOST_CHECK_EQUAL(cpp_db::value_of<uint64_t>(val_ld), uint64_t{15});
    BOOST_CHECK(std::fabs(cpp_db::value_of<float>(val_ld) - vf) < 1e-5);
    BOOST_CHECK(std::fabs(cpp_db::value_of<double>(val_ld) - vd) < 1e-5);
    BOOST_CHECK(std::fabs(cpp_db::value_of<long double>(val_ld) - vld) < 1e-5);
}

BOOST_AUTO_TEST_CASE(test_conversions_strings)
{
    cpp_db::value val_s{std::string("is a string")};
    cpp_db::value val_ccs{"is a const char *"};
    char cs[] = "is a char *";
    cpp_db::value val_cs{cs};

    cpp_db::value val_ws{std::wstring(L"is a wstring")};
    cpp_db::value val_cws{L"is a const wchar_t *"};
    wchar_t wcs[] = L"is a wchar_t *";
    cpp_db::value val_wcs{wcs};

    BOOST_CHECK_EQUAL(cpp_db::value_of<std::string>(val_s), std::string("is a string"));
    BOOST_CHECK_EQUAL(cpp_db::value_of<std::string>(val_ccs), std::string("is a const char *"));
    BOOST_CHECK_EQUAL(cpp_db::value_of<std::string>(val_cs), std::string("is a char *"));

    BOOST_CHECK_EQUAL(cpp_db::value_of<std::wstring>(val_ws), std::wstring(L"is a wstring"));
    BOOST_CHECK_EQUAL(cpp_db::value_of<std::wstring>(val_cws), std::wstring(L"is a const wchar_t *"));
    BOOST_CHECK_EQUAL(cpp_db::value_of<std::wstring>(val_wcs), std::wstring(L"is a wchar_t *"));
}

BOOST_AUTO_TEST_CASE(test_cast_to)
{
    cpp_db::value val1{int8_t{111}};
    cpp_db::value val2{uint8_t{200}};
    cpp_db::value val3{int16_t{25287}};
    cpp_db::value val4{uint16_t{55555}};
    cpp_db::value val5{int32_t{1111111111}};
    cpp_db::value val6{uint32_t{2222222222}};
    cpp_db::value val7{int64_t{5555555555}};
    cpp_db::value val8{uint64_t{14757357730116075519u}};
    cpp_db::value val9{133.97f};
    cpp_db::value vala{4711.0815};
    cpp_db::value valb{15.323l};

    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::string>(val1), "111");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::string>(val2), "200");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::string>(val3), "25287");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::string>(val4), "55555");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::string>(val5), "1111111111");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::string>(val6), "2222222222");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::string>(val7), "5555555555");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::string>(val8), "14757357730116075519");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::string>(val9), "133.97");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::string>(vala), "4711.08");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::string>(valb), "15.323");

    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::wstring>(val1), L"111");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::wstring>(val2), L"200");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::wstring>(val3), L"25287");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::wstring>(val4), L"55555");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::wstring>(val5), L"1111111111");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::wstring>(val6), L"2222222222");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::wstring>(val7), L"5555555555");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::wstring>(val8), L"14757357730116075519");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::wstring>(val9), L"133.97");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::wstring>(vala), L"4711.08");
    BOOST_CHECK_EQUAL(cpp_db::cast_to<std::wstring>(valb), L"15.323");
}

BOOST_AUTO_TEST_CASE(test_null_int)
{
    cpp_db::null_type ni{cpp_db::make_tagged_null<int>(0)};
    cpp_db::null_type nv;

    BOOST_CHECK_EQUAL(ni.tag_type, typeid(int));
    BOOST_CHECK_EQUAL(nv.tag_type, typeid(void));

    BOOST_CHECK(cpp_db::is_null(ni));
    BOOST_CHECK_THROW(cpp_db::value_of(ni), cpp_db::value_is_null);
    BOOST_CHECK_THROW(cpp_db::type_of(ni), cpp_db::value_is_null);

    cpp_db::value val(ni);
    cpp_db::parameter param(0, ni);
    BOOST_CHECK(cpp_db::is_null(val));
    BOOST_CHECK(cpp_db::is_null(param));
    BOOST_CHECK_THROW(cpp_db::value_of<int>(val), cpp_db::value_is_null);
    BOOST_CHECK_THROW(cpp_db::type_of(val), cpp_db::value_is_null);
    BOOST_CHECK_THROW(cpp_db::value_of<int>(param), cpp_db::value_is_null);
    BOOST_CHECK_THROW(cpp_db::type_of(param), cpp_db::value_is_null);
}

BOOST_AUTO_TEST_SUITE_END()
