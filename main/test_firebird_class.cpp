#include "test_firebird_class.h"
#include "firebird_driver.h"
#include "user_password_authentication.h"

void test_firebird_class::init_class()
{
	con.reset(new cpp_db::connection(std::make_shared<cpp_db::firebird_driver>()));
	TEST_FOR_NO_EXCEPTION(con->open("arthur.roche.com:/home2/anhoferj/be3_trunk/data/Database/be3db.fdb", 
							cpp_db::user_password_authentication{ "SYSDBA", "@mo1di2@" },
							{ { "encoding", "UNICODE_FSS" }, {"role", "admin"} }
						));
}

void test_firebird_class::cleanup_class()
{
    TEST_FOR_NO_EXCEPTION(con->close());
	con.reset();
}
