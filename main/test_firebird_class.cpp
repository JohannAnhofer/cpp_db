#include "test_firebird_class.h"
#include "firebird_driver.h"
#include "user_password_authentication.h"
#include "connection.h"
#include "transaction.h"
#include "statement.h"
#include "parameters.h"
#include "execute.h"

void test_firebird_class::init_class()
{
    con = std::shared_ptr<cpp_db::connection>(new cpp_db::connection(cpp_db::connection::create<cpp_db::firebird_driver>()));

    TEST_FOR_NO_EXCEPTION(con->open("arthur.roche.com:/home2/anhoferj/be3_trunk/data/Database/be3db.fdb",  \
							cpp_db::user_password_authentication{ "SYSDBA", "@mo1di2@" }, \
							{ { "encoding", "UNICODE_FSS" }, {"role", "admin"} } \
						));
}

void test_firebird_class::test_execute_non_query()
{
    cpp_db::transaction tr(*con);
    TEST_FOR_NO_EXCEPTION(tr.begin());
    cpp_db::statement stmt(*con);
    TEST_FOR_NO_EXCEPTION(stmt.prepare("insert into TBL_DEVICE(INSTRUMENT_SERIAL, INSTRUMENT_VERSION, DEV_TYP_ID) VALUES('9180', '2.0', 4)"));
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query());
    TEST_FOR_NO_EXCEPTION(tr.rollback());
}

void test_firebird_class::test_execute_non_query_with_parameters()
{
    cpp_db::statement stmt(*con);
    TEST_FOR_NO_EXCEPTION(stmt.prepare("insert into TBL_DEVICE(INSTRUMENT_SERIAL, INSTRUMENT_VERSION, DEV_TYP_ID) VALUES(?, ?, ?)"));
    cpp_db::parameters params(stmt.get_parameters());
    params.bind(cpp_db::parameter(1, "0815-4711"));
    params.bind(cpp_db::parameter(2, "2.0"));
    params.bind(cpp_db::parameter(3, 4));
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query());

    TEST_FOR_NO_EXCEPTION(stmt.reset());
    params.bind(cpp_db::parameter(1, "4711-0815"));
    params.bind(cpp_db::parameter(2, "3.11"));
    params.bind(cpp_db::parameter(3, 3));
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query());

    TEST_FOR_NO_EXCEPTION(stmt.reset());
    params.bind(cpp_db::parameter(1, "xxxx-xxxx"));
    params.bind(cpp_db::parameter(2, cpp_db::null_type{}));
    params.bind(cpp_db::parameter(3, 2));
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query());
}

void test_firebird_class::test_result()
{
    cpp_db::result r(cpp_db::execute(*con, "execute procedure PRO_CONVERTFORMAT(?, ?, ?, ?, ?, ?)", "4711.0815", 10.0, 2.0, 3.0, 2, 3));
    for (int i = 0; i < r.get_column_count(); ++i)
        TEST_FOR_NO_EXCEPTION(std::clog << r.get_column_name(i) << " = ");
    // r.move_next();
    TEST_FOR_NO_EXCEPTION(std::clog << r.get_column_value(0).get_value<std::string>());
    std::clog << std::endl;
}

void test_firebird_class::cleanup_class()
{
    TEST_FOR_NO_EXCEPTION(con->close());
}
