#include "nullable.h"
#include "nullable_types.h"
#include "key_value_pair.h"
#include "coalesce.h"

#include <iostream>
#include <memory>

std::ostream &operator<<(std::ostream &out, cpp_db1::null_type)
{
	out << "<NULL>";
	return out;
}

int main(int argc, char *argv[])
{
    std::cout << std::endl;

    try
    {
        cpp_db::nullable_int a, c(815);

        a = 4711;

        if (a==13)
            std::cout << "Strange!!!\n";
        else
            std::cout << "Equal!!!\n";

        int b = a;

        std::cout << "b=" << b << ", c=" << c << std::endl;

		{
			using namespace cpp_db1;
			null_type null;

			coalesce_trait<int, double, float>::type x0;
			coalesce_trait<null_type, double, float>::type x1;
			coalesce_trait<int, null_type, float>::type x2;
			coalesce_trait<null_type, null_type, float>::type x3;
			coalesce_trait<int, double, null_type>::type x4;
			coalesce_trait<null_type, double, null_type>::type x5;
			coalesce_trait<int, null_type, null_type>::type x6;
			coalesce_trait<null_type, null_type, null_type>::type x7;

			auto v = coalesce(null_type(), 2, 3, 4);

			std::cout << "Coalesce: \n-No nulls: " << coalesce(1, 2, 3, 4)
				<< "\n-Only nulls: " << coalesce(null, null, null)
				<< "\n-First non null: " << coalesce(1, null, null, null)
				<< "\n-First Null: " << coalesce(null, 2, 3, 4)
				<< std::endl;

			std::cout << std::boolalpha << is_null(null) << " " << is_null(1) << std::endl;
		}
		//		std::cout << cpp_db::coalesce(cpp_db::nullable_int(), cpp_db::nullable_int(), 1) << std::endl;

        cpp_db::null_type null;

        std::cout << "Is null ? " << std::boolalpha << null.is_null() << std::endl;

        std::unique_ptr<cpp_db::connection_option> p(cpp_db::make_key_value_pair(std::string("driver"), std::string("QIBASE")));

        std::cout << p->key<std::string, std::string>()
                  << "=\"" << p->value<std::string, std::string>() << "\""
                  << std::endl;

//		std::cout << cpp_db::coalesce(cpp_db::nullable_double(), cpp_db::nullable_int(), cpp_db::null_type(), 3.54, 1) << std::endl;
    }
    catch(const std::exception &ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
    }
}
