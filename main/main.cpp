#include "test_tools_class.h"
#include "test_cpp_db_class.h"
#include "test_test_class.h"
#include "test_app.h"
#include "test_firebird_class.h"
#include "test_sqlite_class.h"

#include "driver_registry.h"
#include "sqlite_driver.h"
#include "firebird_driver.h"

void register_drivers()
{
    cpp_db::driver_registry::register_driver("sqlite", []{return cpp_db::sqlite_driver::create();});
    cpp_db::driver_registry::register_driver("firebird", []{return cpp_db::firebird_driver::create();});
}

BEGIN_DECLARE_TEST_APP(register_drivers)
	DECLARE_TEST_CLASS(test_test_class)	
    DECLARE_TEST_CLASS(test_tools_class)
    DECLARE_TEST_CLASS(test_cpp_db_class)
    DECLARE_TEST_CLASS(test_sqlite_class)
	DECLARE_TEST_CLASS(test_firebird_class)
END_DECLARE_TEST_APP([]{})
