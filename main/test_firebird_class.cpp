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
    TEST_FOR_NO_EXCEPTION(stmt.prepare("insert into TBL_DEVICE(INSTRUMENT_SERIAL, INSTRUMENT_VERSION, DEV_TYP_ID) VALUES(?, ?, ?)"));
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query("9180", "2.0", 4ll));
    TEST_FOR_NO_EXCEPTION(tr.rollback());
}

void test_firebird_class::test_execute_non_query_with_parameters()
{
    cpp_db::statement stmt(*con);
    TEST_FOR_NO_EXCEPTION(stmt.prepare("insert into TBL_DEVICE(INSTRUMENT_SERIAL, INSTRUMENT_VERSION, DEV_TYP_ID) VALUES(?, ?, ?)"));
    cpp_db::parameters params(stmt.get_parameters());
    params.bind(cpp_db::parameter(1, "0815-4711"));
    params.bind(cpp_db::parameter(2, "2.0"));
    params.bind(cpp_db::parameter(3, 4ll));
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

    TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "delete from tbl_device where INSTRUMENT_SERIAL = ?", "0815-4711"));
    TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "delete from tbl_device where INSTRUMENT_SERIAL = ? and INSTRUMENT_VERSION = ? and DEV_TYP_ID = ?", { {1, "4711-0815"}, {2, "3.11"}, {3, 3}}));
    TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "delete from tbl_device where INSTRUMENT_SERIAL = 'xxxx-xxxx'"));
}

template<typename T>
std::string to_string(const cpp_db::value &val, const std::string &def = "<null>")
{
    if (is_null(val))
        return def;
    else
    {
        std::stringstream s;
        s << cpp_db::value_of<T>(val);
        return s.str();
    }
}

void test_firebird_class::test_result_single_row()
{
    cpp_db::result r(cpp_db::execute(*con, "execute procedure PRO_CONVERTFORMAT(?, ?, ?, ?, ?, ?)", "4711.0815", 10.0, 2.0, 3.0, 2, 3));
    TEST_VERIFY(!r.is_eof());
    TEST_EQUAL(r.get_column_count(), 1);
    TEST_EQUAL(r.get_column_name(0), "NUMBER_FMT");
    TEST_VERIFY(!cpp_db::is_null(r.get_column_value(0)));
    TEST_EQUAL(cpp_db::value_of<std::string>(r.get_column_value(0)), "3147.390000000000");

    cpp_db::value val = cpp_db::execute_scalar(*con, "select first(1) INSTRUMENT_SERIAL from TBL_DEVICE");
    TEST_VERIFY(!is_null(val));
    TEST_EQUAL(cpp_db::value_of<std::string>(val), "19999");

}

void test_firebird_class::test_result_multi_row()
{
    cpp_db::result r(cpp_db::execute(*con, "select first(3) * from TBL_MAINCONFIG WHERE CLASS_NAME = ? order by ID DESC", "ConfigurationDataLayer::DbSettings"));
    TEST_EQUAL(r.get_column_name(0), "ID");
    TEST_EQUAL(r.get_column_name(1), "CLASS_NAME");
    TEST_EQUAL(r.get_column_name(2), "CLASS_PARAM");
    TEST_EQUAL(r.get_column_name(3), "CLASS_VALUE");
    TEST_EQUAL(r.get_column_name(4), "DESCRIPTION");
    TEST_EQUAL(r.get_column_name(5), "LID");
    TEST_EQUAL(r.get_column_name(6), "EXPORT");
    TEST_EQUAL(r.get_column_name(7), "DEFAULT_VALUE");

    std::vector<int64_t> ids;
    std::vector<std::string> params, values, desc, defval;

    while(!r.is_eof())
    {
        TEST_FOR_NO_EXCEPTION(ids.push_back(cpp_db::value_of<int64_t>(r.get_column_value("ID"))));
        TEST_FOR_NO_EXCEPTION(params.push_back(cpp_db::value_of<std::string>(r.get_column_value("CLASS_PARAM"))));
        TEST_FOR_NO_EXCEPTION(values.push_back(cpp_db::value_of<std::string>(r.get_column_value("CLASS_VALUE"))));
        TEST_FOR_NO_EXCEPTION(desc.push_back(cpp_db::value_of<std::string>(r.get_column_value("DESCRIPTION"))));
        TEST_FOR_NO_EXCEPTION(defval.push_back(cpp_db::value_of<std::string>(r.get_column_value("DEFAULT_VALUE"))));
        TEST_EQUAL(cpp_db::value_of<std::string>(r.get_column_value("CLASS_NAME")), "ConfigurationDataLayer::DbSettings");
        TEST_VERIFY(is_null(r.get_column_value("LID")));
        TEST_EQUAL(cpp_db::value_of<short>(r.get_column_value("EXPORT")), 0);
        TEST_FOR_NO_EXCEPTION(r.move_next());
    }
    TEST_EQUAL(ids.size(), 3u);

    if (ids.size() > 0)
    {
        TEST_EQUAL(ids[0], 333);
        TEST_EQUAL(params[0], "PrintViaPostScriptConversionScript");
        TEST_EQUAL(values[0], "true");
        TEST_EQUAL(defval[0], "true");
        TEST_EQUAL(desc[0], "Printing via postscript file on/off");
    }

    if (ids.size() > 1)
    {
        TEST_EQUAL(ids[1], 312);
        TEST_EQUAL(params[1], "FtpUpdatePathOnServer");
        TEST_EQUAL(values[1], "/Update/Series/");
        TEST_EQUAL(defval[1], "/Update/Series/");
        TEST_EQUAL(desc[1], "Path to software update directory");
    }

    if (ids.size() > 2)
    {
        TEST_EQUAL(ids[2], 311);
        TEST_EQUAL(params[2], "FtpUpdateServer");
        TEST_EQUAL(values[2], "rgxmvs17.roche.com");
        TEST_EQUAL(defval[2], "rgxmvs17.roche.com");
        TEST_EQUAL(desc[2], "Internal ftp update server name or IP");
    }
}

void test_firebird_class::test_result_empty()
{
    cpp_db::result r(cpp_db::execute(*con, "select * from TBL_MAINCONFIG WHERE CLASS_NAME = ? order by ID DESC", "no rows"));
    TEST_VERIFY(r.is_eof());

}

void test_firebird_class::cleanup_class()
{
    TEST_FOR_NO_EXCEPTION(con->close());
}
