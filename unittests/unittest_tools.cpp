#include "stdafx.h"
#include "CppUnitTest.h"

#include "coalesce.h"
#include "nullable_types.h"
#include "key_value_pair.h"

#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittests
{		
	TEST_CLASS(UnitTest_Tools)
	{
	public:
		
		TEST_METHOD(test_is_null)
		{
			tools::nullable_int a, c(815);

			a = 4711;

			int b = a;

			Assert::AreNotEqual(static_cast<int>(a), 13);
			Assert::AreEqual(static_cast<int>(a), 4711);
			Assert::AreEqual(b, static_cast<int>(a) );
			Assert::AreEqual(static_cast<int>(b), 4711);
			Assert::AreEqual(static_cast<int>(c), 815);
		}

		TEST_METHOD(test_key_value_pair)
		{
			std::unique_ptr<tools::connection_option> p(tools::make_key_value_pair(std::string("driver"), std::string("QIBASE")));
			Assert::AreEqual((p->key<std::string, std::string>()), std::string("driver"));
			Assert::AreEqual((p->value<std::string, std::string>()), std::string("QIBASE"));
		}

		TEST_METHOD(test_coalesce)
		{
			tools::nullable_int a(4711);
			tools::nullable_int d;
			const tools::nullable_int e(10), f;

			Assert::IsTrue(!tools::is_null(a) && !a.is_null());
			Assert::IsTrue(tools::is_null(d) && d.is_null());
			Assert::IsTrue(!tools::is_null(e) && !e.is_null());
			Assert::IsTrue(tools::is_null(f) && f.is_null());
			Assert::IsTrue(tools::is_null(tools::nullable_int()) && tools::nullable_int().is_null());

			tools::null_type null;
			int ai = 13;
			tools::coalesce_trait<tools::null_type, tools::null_type, tools::null_type>::type x7;
			Assert::IsTrue(!tools::is_null(1));
			Assert::IsTrue(!tools::is_null(ai));
			Assert::IsTrue(tools::is_null(tools::coalesce(null, null, null)));
			Assert::IsTrue(tools::is_null(x7));
			Assert::IsTrue(tools::is_null(null));
			Assert::IsTrue(tools::is_null(tools::null_type()));
		}
	};
}