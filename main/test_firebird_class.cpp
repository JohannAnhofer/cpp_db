#include "test_firebird_class.h"
#include "firebird_driver.h"

void test_firebird_class::init_class()
{
	con.reset(new cpp_db::connection(std::make_shared<cpp_db::firebird_driver>()));
    TEST_FOR_NO_EXCEPTION(con->open("arthur.roche.com:/home2/anhoferj/be3_trunk/data/Database/be3db.fdb", {{"user", "SYSDBA"}, {"password", "@mo1di2@"}}));
}

void test_firebird_class::cleanup_class()
{
    TEST_FOR_NO_EXCEPTION(con->close());
	con.reset();
}
