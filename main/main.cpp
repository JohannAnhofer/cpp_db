#include "test_tools_class.h"
#include "test_cpp_db_class.h"
#include "test_test_class.h"
#include "test_app.h"
#include "test_firebird_class.h"
#include "test_sqlite_class.h"

BEGIN_DECLARE_TEST_APP()
	DECLARE_TEST_CLASS(test_test_class)	
    DECLARE_TEST_CLASS(test_tools_class)
    DECLARE_TEST_CLASS(test_cpp_db_class)
    DECLARE_TEST_CLASS(test_sqlite_class)
	DECLARE_TEST_CLASS(test_firebird_class)
END_DECLARE_TEST_APP()
