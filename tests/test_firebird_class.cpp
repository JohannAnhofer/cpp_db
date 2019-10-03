#define BOOST_TEST_MODULE cpp_db_firebird
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>

#include "firebird_driver.h"
#include "user_password_authentication.h"
#include "connection.h"
#include "transaction.h"
#include "transaction_scope.h"
#include "statement.h"
#include "parameters.h"
#include "execute.h"
#include "driver_registry.h"
#include "firebird_driver.h"

BOOST_AUTO_TEST_SUITE(test_cpp_db_firebird)

struct FirebirdTestFixture
{
    FirebirdTestFixture()
    {
        con = std::shared_ptr<cpp_db::connection>(new cpp_db::connection("firebird"));

        BOOST_CHECK_NO_THROW(con->open("localhost:employee",  \
                                cpp_db::user_password_authentication{ "SYSDBA", "masterkey" }, \
                                { { "encoding", "UNICODE_FSS" }, {"role", "admin"} } \
                            ));
        const char *sql_create_test_table = R"(
                            CREATE TABLE test_table
                            (
                                ID       INTEGER PRIMARY KEY,
                                SERIAL   VARCHAR(30) NOT NULL,
                                VERSION  VARCHAR(30),
                                TYPE_ID  INTEGER NOT NULL
                            );
                )";
        BOOST_CHECK_NO_THROW(execute_ddl(*con, sql_create_test_table));

        const char * sql_create_config_table = R"(
                            CREATE TABLE config_table
                            (
                                ID             BIGINT PRIMARY KEY,
                                NAME           VARCHAR(100) NOT NULL,
                                PARAM          VARCHAR(50) NOT NULL,
                                "VALUE"        VARCHAR(10000),
                                DESCRIPTION    VARCHAR(100),
                                LID            INTEGER,
                                EXPORT         SMALLINT,
                                DEFAULT_VALUE  VARCHAR(10000)
                            );
                )";
        BOOST_CHECK_NO_THROW(execute_ddl(*con, sql_create_config_table));

        const char * sql_create_test_proc = R"(
                              create procedure test_proc (
                                  NUMBER varchar(30),
                                  OFFSET1 double precision,
                                  NUMERATOR double precision,
                                  DENUMERATOR double precision,
                                  DIGITS integer,
                                  HOW integer)
                              returns (
                                  NUMBER_FMT varchar(50)
                              )
                              as
                              begin
                                number_fmt = '3147.39';
                                suspend;
                              end
              )";
        BOOST_CHECK_NO_THROW(execute_ddl(*con, sql_create_test_proc));

        cpp_db::transaction tr(*con);
        cpp_db::transaction_scope trs(&tr);
        BOOST_CHECK_NO_THROW(execute_non_query(*con, "insert into test_table(ID, SERIAL, VERSION, TYPE_ID) VALUES(?, ?, ?, ?)", 1, "19999", "1.0", 4));

        cpp_db::statement stmt(R"(insert into config_table(ID, NAME, PARAM, "VALUE", DESCRIPTION, LID, EXPORT, DEFAULT_VALUE) VALUES(?, ?, ?, ?, ?, ?, ?, ?);)", *con);
        BOOST_CHECK_NO_THROW(stmt.execute_non_query(311, "settings", "class#1/key#1", "value#1", "desc#1", cpp_db::null_type{}, 0, "defvalue#1"));
        BOOST_CHECK_NO_THROW(stmt.reset());
        BOOST_CHECK_NO_THROW(stmt.execute_non_query(312, "settings", "class#2/key#2", "value#2", "desc#2", cpp_db::null_type{}, 0, "value#2d"));
        BOOST_CHECK_NO_THROW(stmt.reset());
        BOOST_CHECK_NO_THROW(stmt.execute_non_query(333, "settings", "class#3/key#3", "true", "desc#3", cpp_db::null_type{}, 0, "true"));
    }
    ~FirebirdTestFixture()
    {
        using cpp_db::execute_ddl;
        BOOST_CHECK_NO_THROW(execute_ddl(*con, "drop table test_table;"));
        BOOST_CHECK_NO_THROW(execute_ddl(*con, "drop table config_table;"));
        BOOST_CHECK_NO_THROW(execute_ddl(*con, "drop procedure test_proc;"));
        BOOST_CHECK_NO_THROW(con->close());
    }

    std::shared_ptr<cpp_db::connection> con;
};

BOOST_FIXTURE_TEST_CASE(test_execute_non_query, FirebirdTestFixture)
{
    cpp_db::transaction tr(*con);
    BOOST_CHECK_NO_THROW(tr.begin());
    cpp_db::statement stmt(*con);
    BOOST_CHECK_NO_THROW(stmt.prepare("insert into test_table(ID, SERIAL, VERSION, TYPE_ID) VALUES(?, ?, ?, ?)"));
    BOOST_CHECK_NO_THROW(stmt.execute_non_query(2, "9180", "2.0", 4));
    BOOST_CHECK_NO_THROW(tr.rollback());
}

BOOST_FIXTURE_TEST_CASE(test_execute_non_query_with_parameters, FirebirdTestFixture)
{
    cpp_db::statement stmt(*con);
    BOOST_CHECK_NO_THROW(stmt.prepare("insert into test_table(ID, SERIAL, VERSION, TYPE_ID) VALUES(?, ?, ?, ?)"));
    cpp_db::parameters params(stmt.get_parameters());
    params.bind(cpp_db::parameter(1, 2));
    params.bind(cpp_db::parameter(2, "0815-4711"));
    params.bind(cpp_db::parameter(3, "2.0"));
    params.bind(cpp_db::parameter(4, 4));
    BOOST_CHECK_NO_THROW(stmt.execute_non_query());

    BOOST_CHECK_NO_THROW(stmt.reset());
    params.bind(cpp_db::parameter(1, 3));
    params.bind(cpp_db::parameter(2, "4711-0815"));
    params.bind(cpp_db::parameter(3, "3.11"));
    params.bind(cpp_db::parameter(4, 3));
    BOOST_CHECK_NO_THROW(stmt.execute_non_query());

    BOOST_CHECK_NO_THROW(stmt.reset());
    params.bind(cpp_db::parameter(1, 4));
    params.bind(cpp_db::parameter(2, "xxxx-xxxx"));
    params.bind(cpp_db::parameter(3, cpp_db::null_type{}));
    params.bind(cpp_db::parameter(4, 2));
    BOOST_CHECK_NO_THROW(stmt.execute_non_query());

    BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "delete from test_table where SERIAL = ?", "0815-4711"));
    BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "delete from test_table where SERIAL = ? and VERSION = ? and TYPE_ID = ?", { {1, "4711-0815"}, {2, "3.11"}, {3, 3}}));
    BOOST_CHECK_NO_THROW(cpp_db::execute_non_query(*con, "delete from test_table where SERIAL = 'xxxx-xxxx'"));
}

template<typename T>
std::string to_string(const cpp_db::value &val, const std::string &def = "<null>")
{
    if (cpp_db::is_null(val))
        return def;
    else
    {
        std::stringstream s;
        s << cpp_db::value_of<T>(val);
        return s.str();
    }
}

BOOST_FIXTURE_TEST_CASE(test_result_single_row, FirebirdTestFixture)
{
    cpp_db::result r(cpp_db::execute(*con, "execute procedure test_proc(?, ?, ?, ?, ?, ?)", "4711.0815", 10.0, 2.0, 3.0, 2, 3));
    BOOST_CHECK(!r.is_eof());
    BOOST_CHECK_EQUAL(r.get_column_count(), 1);
    BOOST_CHECK_EQUAL(r.get_column_name(0), "NUMBER_FMT");
    BOOST_CHECK(!cpp_db::is_null(r.get_column_value(0)));
    BOOST_CHECK_EQUAL(cpp_db::value_of<std::string>(r.get_column_value(0)), "3147.39");

    cpp_db::value val = cpp_db::execute_scalar(*con, "select first(1) SERIAL from test_table");
    BOOST_CHECK(!cpp_db::is_null(val));
    BOOST_CHECK_EQUAL(cpp_db::value_of<std::string>(val), "19999");

}

BOOST_FIXTURE_TEST_CASE(test_result_multi_row, FirebirdTestFixture)
{
    cpp_db::result r(cpp_db::execute(*con, "select first(3) * from config_table WHERE NAME = ? order by ID DESC", "settings"));
    BOOST_CHECK_EQUAL(r.get_column_name(0), "ID");
    BOOST_CHECK_EQUAL(r.get_column_name(1), "NAME");
    BOOST_CHECK_EQUAL(r.get_column_name(2), "PARAM");
    BOOST_CHECK_EQUAL(r.get_column_name(3), "VALUE");
    BOOST_CHECK_EQUAL(r.get_column_name(4), "DESCRIPTION");
    BOOST_CHECK_EQUAL(r.get_column_name(5), "LID");
    BOOST_CHECK_EQUAL(r.get_column_name(6), "EXPORT");
    BOOST_CHECK_EQUAL(r.get_column_name(7), "DEFAULT_VALUE");

    std::vector<int64_t> ids;
    std::vector<std::string> params, values, desc, defval;

    while(!r.is_eof())
    {
        BOOST_CHECK_NO_THROW(ids.push_back(cpp_db::value_of<int64_t>(r.get_column_value("ID"))));
        BOOST_CHECK_NO_THROW(params.push_back(cpp_db::value_of<std::string>(r.get_column_value("PARAM"))));
        BOOST_CHECK_NO_THROW(values.push_back(cpp_db::value_of<std::string>(r.get_column_value("VALUE"))));
        BOOST_CHECK_NO_THROW(desc.push_back(cpp_db::value_of<std::string>(r.get_column_value("DESCRIPTION"))));
        BOOST_CHECK_NO_THROW(defval.push_back(cpp_db::value_of<std::string>(r.get_column_value("DEFAULT_VALUE"))));
        BOOST_CHECK_EQUAL(cpp_db::value_of<std::string>(r.get_column_value("NAME")), "settings");
        BOOST_CHECK(cpp_db::is_null(r.get_column_value("LID")));
        BOOST_CHECK_EQUAL(cpp_db::value_of<short>(r.get_column_value("EXPORT")), 0);
        BOOST_CHECK_NO_THROW(r.move_next());
    }
    BOOST_CHECK_EQUAL(ids.size(), 3u);

    if (ids.size() > 0)
    {
        BOOST_CHECK_EQUAL(ids[0], 333);
        BOOST_CHECK_EQUAL(params[0], "class#3/key#3");
        BOOST_CHECK_EQUAL(values[0], "true");
        BOOST_CHECK_EQUAL(defval[0], "true");
        BOOST_CHECK_EQUAL(desc[0], "desc#3");
    }

    if (ids.size() > 1)
    {
        BOOST_CHECK_EQUAL(ids[1], 312);
        BOOST_CHECK_EQUAL(params[1], "class#2/key#2");
        BOOST_CHECK_EQUAL(values[1], "value#2");
        BOOST_CHECK_EQUAL(defval[1], "value#2d");
        BOOST_CHECK_EQUAL(desc[1], "desc#2");
    }

    if (ids.size() > 2)
    {
        BOOST_CHECK_EQUAL(ids[2], 311);
        BOOST_CHECK_EQUAL(params[2], "class#1/key#1");
        BOOST_CHECK_EQUAL(values[2], "value#1");
        BOOST_CHECK_EQUAL(defval[2], "defvalue#1");
        BOOST_CHECK_EQUAL(desc[2], "desc#1");
    }
}

BOOST_FIXTURE_TEST_CASE(test_result_empty, FirebirdTestFixture)
{
    cpp_db::result r(cpp_db::execute(*con, "select * from config_table WHERE NAME = ? order by ID DESC", "no rows"));
    BOOST_CHECK(r.is_eof());
}

BOOST_AUTO_TEST_SUITE_END()

bool init_firebird_driver()
{
    cpp_db::driver_registry::register_driver("firebird", [] {return cpp_db::firebird_driver::create(); });
    return true;
}

int main(int argc, char* argv[])
{
    return boost::unit_test::unit_test_main( &init_firebird_driver, argc, argv );
}
