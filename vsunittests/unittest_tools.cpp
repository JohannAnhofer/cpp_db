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

			int b = a.value();

			Assert::AreNotEqual(a.value(), 13);
			Assert::AreEqual(a.value(), 4711);
			Assert::AreEqual(b, a.value() );
			Assert::AreEqual(b, 4711);
			Assert::AreEqual(c.value(), 815);
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

			Assert::IsTrue(!tools::is_null(a) && a);
			Assert::IsTrue(tools::is_null(d) && !d);
			Assert::IsTrue(!tools::is_null(e) && e);
			Assert::IsTrue(tools::is_null(f) && !f);
			Assert::IsTrue(tools::is_null(tools::nullable_int()) && !tools::nullable_int());

			using tools::null;
			int ai = 13;
			tools::coalesce_trait<tools::null_type, tools::null_type, tools::null_type>::type x7 = null;
			Assert::IsTrue(!tools::is_null(1));
			Assert::IsTrue(!tools::is_null(ai));
			Assert::IsTrue(tools::is_null(tools::coalesce(null, null, null)));
			Assert::IsTrue(tools::is_null(x7));
			Assert::IsTrue(tools::is_null(null));
			Assert::IsTrue(tools::is_null(tools::null));
		}
	};
}