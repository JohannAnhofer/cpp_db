#include "test_tools_class.h"
#include "test_cpp_db_class.h"

#include <iostream>

int main(int , char *[])
{
    std::cout << std::endl;

    test_tools_class tt;
    test_cpp_db_class tcd;

//    tt.set_tiny_mode(true);
//    tt.set_test_stream(&std::cerr);
//    tcd.set_tiny_mode(true);

    tcd();
    tt();

	return 0;
}
