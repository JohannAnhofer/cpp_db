#include "test_tools_class.h"
#include "test_cpp_db_class.h"
#include "test_test_class.h"
#include "test_app.h"

BEGIN_DECLARE_TEST_APP([]{})
	DECLARE_TEST_CLASS(test_test_class)	
    DECLARE_TEST_CLASS(test_tools_class)
    DECLARE_TEST_CLASS(test_cpp_db_class)
END_DECLARE_TEST_APP([]{})
