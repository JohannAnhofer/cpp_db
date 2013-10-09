#include "test_tools_class.h"
#include "test_cpp_db_class.h"

#include <iostream>

int main(int , char *[])
{
    std::cout << std::endl;

    test_tools_class()();
    test_cpp_db_class()();

	return 0;
}
