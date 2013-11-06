#include "test_firebird_class.h"
#include "firebird_driver.h"
#include "user_password_authentication.h"
#include "connection.h"
#include "transaction.h"
#include "statement.h"

void test_firebird_class::init_class()
{
    con = std::shared_ptr<cpp_db::connection>(new cpp_db::connection(cpp_db::connection::create<cpp_db::firebird_driver>()));

    TEST_FOR_NO_EXCEPTION(con->open("arthur.roche.com:/home2/anhoferj/be3_trunk/data/Database/be3db.fdb",  \
							cpp_db::user_password_authentication{ "SYSDBA", "@mo1di2@" }, \
							{ { "encoding", "UNICODE_FSS" }, {"role", "admin"} } \
						));
    cpp_db::transaction tr(*con);
    TEST_FOR_NO_EXCEPTION(tr.begin());
    cpp_db::statement stmt("insert into TBL_DEVICE(INSTRUMENT_SERIAL, INSTRUMENT_VERSION, DEV_TYP_ID) VALUES('9180', '2.0', 4)", *con);
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query());
    TEST_FOR_NO_EXCEPTION(tr.rollback());
}

void test_firebird_class::cleanup_class()
{
    TEST_FOR_NO_EXCEPTION(con->close());
}
