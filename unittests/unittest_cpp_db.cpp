#include "stdafx.h"
#include "CppUnitTest.h"

#include "null.h"
#include "value.h"
#include "parameter.h"
#include "type_of.h"

#include <sstream>

std::wstring to_wstring(const char *text)
{
	std::wostringstream stream;
	stream << text;
	return stream.str();
}

namespace Microsoft
{
	namespace VisualStudio 
	{
		namespace CppUnitTestFramework
		{
			template <> static std::wstring ToString<std::type_index>(const std::type_index& ti) { return to_wstring(ti.name()); }
			template <> static std::wstring ToString<std::type_index>(const std::type_index* ti) { return to_wstring(ti->name()); }
			template <> static std::wstring ToString<std::type_index>(std::type_index* ti)       { return to_wstring(ti->name()); }
		}
	}
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittests
{
	TEST_CLASS(UnitTest_cpp_db)
	{
	public:

		TEST_METHOD(test_is_null)
		{
			cpp_db::null_type null;

			Assert::IsTrue(is_null(null));
			Assert::IsFalse(cpp_db::is_null(1));

			cpp_db::value vnull(null);
			Assert::IsTrue(is_null(vnull));

			cpp_db::parameter pnull(0, null);
			Assert::IsTrue(is_null(pnull));
		}
		TEST_METHOD(test_value)
		{
			Assert::AreEqual(cpp_db::type_of(10), std::type_index(typeid(int)));
			Assert::AreEqual(cpp_db::type_of(cpp_db::value{ 10 }), std::type_index(typeid(int)));
			Assert::AreEqual(cpp_db::type_of(cpp_db::parameter{ 0, 10 }), std::type_index(typeid(int)));
		}

		TEST_METHOD(test_parameter)
		{
			cpp_db::parameter param1(0, std::string("Hello world!")), param2(std::string("@test"), 27.85);

			Assert::IsTrue(param1.has_index(), L"param1 has an index");
			Assert::IsFalse(param1.has_name(), L"param1 don't have a name");
			Assert::IsFalse(param2.has_index(), L"param2 don't have an index");
			Assert::IsTrue(param2.has_name(), L"param2 has a name");
			Assert::IsTrue(cpp_db::type_of(param1) == typeid(std::string), L"param1 has a value of type std::string");
			Assert::IsFalse(cpp_db::type_of(param1) == typeid(double), L"param1 don't have a value of type double");
			Assert::IsFalse(cpp_db::type_of(param2) == typeid(std::string), L"param2 don't have a value of type std::string");
			Assert::IsTrue(cpp_db::type_of(param2) == typeid(double), L"param2 has a value of type double");

			Assert::AreEqual(cpp_db::value_of<std::string>(param1), std::string("Hello world!"), L"param1 has a value of 'Hello world!'");
			Assert::IsTrue(std::fabs(cpp_db::value_of<double>(param2) - 27.85) < 0.000001, L"param2 has a value of 27.85");

//			TEST_FOR_EXCEPTION(param1.get_value<int>(), std::runtime_error);
		}
	};
}
