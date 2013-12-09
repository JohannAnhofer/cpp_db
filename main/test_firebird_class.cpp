#include "test_firebird_class.h"
#include "firebird_driver.h"
#include "user_password_authentication.h"
#include "connection.h"
#include "transaction.h"
#include "transaction_scope.h"
#include "statement.h"
#include "parameters.h"
#include "execute.h"

void test_firebird_class::init_class()
{
    con = std::shared_ptr<cpp_db::connection>(new cpp_db::connection("firebird"));

    TEST_FOR_NO_EXCEPTION(con->open("localhost:employee",  \
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
    TEST_FOR_NO_EXCEPTION(execute_ddl(*con, sql_create_test_table));

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
	TEST_FOR_NO_EXCEPTION(execute_ddl(*con, sql_create_config_table));

	const char * sql_create_test_proc = R"(
                          create procedure test_proc (
                              NUMBER varchar(30),
                              OFFSET double precision,
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
    TEST_FOR_NO_EXCEPTION(execute_ddl(*con, sql_create_test_proc));

    cpp_db::transaction tr(*con);
    cpp_db::transaction_scope trs(&tr);
    TEST_FOR_NO_EXCEPTION(execute_non_query(*con, "insert into test_table(ID, SERIAL, VERSION, TYPE_ID) VALUES(?, ?, ?, ?)", 1, "19999", "1.0", 4));

    cpp_db::statement stmt(R"(insert into config_table(ID, NAME, PARAM, "VALUE", DESCRIPTION, LID, EXPORT, DEFAULT_VALUE) VALUES(?, ?, ?, ?, ?, ?, ?, ?);)", *con);
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query(311, "settings", "class#1/key#1", "value#1", "desc#1", cpp_db::null_type{}, 0, "defvalue#1"));
    TEST_FOR_NO_EXCEPTION(stmt.reset());
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query(312, "settings", "class#2/key#2", "value#2", "desc#2", cpp_db::null_type{}, 0, "value#2d"));
    TEST_FOR_NO_EXCEPTION(stmt.reset());
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query(333, "settings", "class#3/key#3", "true", "desc#3", cpp_db::null_type{}, 0, "true"));
}

void test_firebird_class::test_execute_non_query()
{
    cpp_db::transaction tr(*con);
    TEST_FOR_NO_EXCEPTION(tr.begin());
    cpp_db::statement stmt(*con);
    TEST_FOR_NO_EXCEPTION(stmt.prepare("insert into test_table(ID, SERIAL, VERSION, TYPE_ID) VALUES(?, ?, ?, ?)"));
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query(2, "9180", "2.0", 4));
    TEST_FOR_NO_EXCEPTION(tr.rollback());
}

void test_firebird_class::test_execute_non_query_with_parameters()
{
    cpp_db::statement stmt(*con);
    TEST_FOR_NO_EXCEPTION(stmt.prepare("insert into test_table(ID, SERIAL, VERSION, TYPE_ID) VALUES(?, ?, ?, ?)"));
    cpp_db::parameters params(stmt.get_parameters());
    params.bind(cpp_db::parameter(1, 2));
    params.bind(cpp_db::parameter(2, "0815-4711"));
    params.bind(cpp_db::parameter(3, "2.0"));
    params.bind(cpp_db::parameter(4, 4));
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query());

    TEST_FOR_NO_EXCEPTION(stmt.reset());
    params.bind(cpp_db::parameter(1, 3));
    params.bind(cpp_db::parameter(2, "4711-0815"));
    params.bind(cpp_db::parameter(3, "3.11"));
    params.bind(cpp_db::parameter(4, 3));
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query());

    TEST_FOR_NO_EXCEPTION(stmt.reset());
    params.bind(cpp_db::parameter(1, 4));
    params.bind(cpp_db::parameter(2, "xxxx-xxxx"));
    params.bind(cpp_db::parameter(3, cpp_db::null_type{}));
    params.bind(cpp_db::parameter(4, 2));
    TEST_FOR_NO_EXCEPTION(stmt.execute_non_query());

    TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "delete from test_table where SERIAL = ?", "0815-4711"));
    TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "delete from test_table where SERIAL = ? and VERSION = ? and TYPE_ID = ?", { {1, "4711-0815"}, {2, "3.11"}, {3, 3}}));
    TEST_FOR_NO_EXCEPTION(cpp_db::execute_non_query(*con, "delete from test_table where SERIAL = 'xxxx-xxxx'"));
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

void test_firebird_class::test_result_single_row()
{
    cpp_db::result r(cpp_db::execute(*con, "execute procedure test_proc(?, ?, ?, ?, ?, ?)", "4711.0815", 10.0, 2.0, 3.0, 2, 3));
    TEST_VERIFY(!r.is_eof());
    TEST_EQUAL(r.get_column_count(), 1);
    TEST_EQUAL(r.get_column_name(0), "NUMBER_FMT");
    TEST_VERIFY(!cpp_db::is_null(r.get_column_value(0)));
    TEST_EQUAL(cpp_db::value_of<std::string>(r.get_column_value(0)), "3147.39");

    cpp_db::value val = cpp_db::execute_scalar(*con, "select first(1) SERIAL from test_table");
    TEST_VERIFY(!cpp_db::is_null(val));
    TEST_EQUAL(cpp_db::value_of<std::string>(val), "19999");

}

void test_firebird_class::test_result_multi_row()
{
    cpp_db::result r(cpp_db::execute(*con, "select first(3) * from config_table WHERE NAME = ? order by ID DESC", "settings"));
    TEST_EQUAL(r.get_column_name(0), "ID");
    TEST_EQUAL(r.get_column_name(1), "NAME");
    TEST_EQUAL(r.get_column_name(2), "PARAM");
    TEST_EQUAL(r.get_column_name(3), "VALUE");
    TEST_EQUAL(r.get_column_name(4), "DESCRIPTION");
    TEST_EQUAL(r.get_column_name(5), "LID");
    TEST_EQUAL(r.get_column_name(6), "EXPORT");
    TEST_EQUAL(r.get_column_name(7), "DEFAULT_VALUE");

    std::vector<int64_t> ids;
    std::vector<std::string> params, values, desc, defval;

    while(!r.is_eof())
    {
        TEST_FOR_NO_EXCEPTION(ids.push_back(cpp_db::value_of<int64_t>(r.get_column_value("ID"))));
        TEST_FOR_NO_EXCEPTION(params.push_back(cpp_db::value_of<std::string>(r.get_column_value("PARAM"))));
        TEST_FOR_NO_EXCEPTION(values.push_back(cpp_db::value_of<std::string>(r.get_column_value("VALUE"))));
        TEST_FOR_NO_EXCEPTION(desc.push_back(cpp_db::value_of<std::string>(r.get_column_value("DESCRIPTION"))));
        TEST_FOR_NO_EXCEPTION(defval.push_back(cpp_db::value_of<std::string>(r.get_column_value("DEFAULT_VALUE"))));
        TEST_EQUAL(cpp_db::value_of<std::string>(r.get_column_value("NAME")), "settings");
        TEST_VERIFY(cpp_db::is_null(r.get_column_value("LID")));
        TEST_EQUAL(cpp_db::value_of<short>(r.get_column_value("EXPORT")), 0);
        TEST_FOR_NO_EXCEPTION(r.move_next());
    }
    TEST_EQUAL(ids.size(), 3u);

    if (ids.size() > 0)
    {
        TEST_EQUAL(ids[0], 333);
        TEST_EQUAL(params[0], "class#3/key#3");
        TEST_EQUAL(values[0], "true");
        TEST_EQUAL(defval[0], "true");
        TEST_EQUAL(desc[0], "desc#3");
    }

    if (ids.size() > 1)
    {
        TEST_EQUAL(ids[1], 312);
        TEST_EQUAL(params[1], "class#2/key#2");
        TEST_EQUAL(values[1], "value#2");
        TEST_EQUAL(defval[1], "value#2d");
        TEST_EQUAL(desc[1], "desc#2");
    }

    if (ids.size() > 2)
    {
        TEST_EQUAL(ids[2], 311);
        TEST_EQUAL(params[2], "class#1/key#1");
        TEST_EQUAL(values[2], "value#1");
        TEST_EQUAL(defval[2], "defvalue#1");
        TEST_EQUAL(desc[2], "desc#1");
    }
}

void test_firebird_class::test_result_empty()
{
    cpp_db::result r(cpp_db::execute(*con, "select * from config_table WHERE NAME = ? order by ID DESC", "no rows"));
    TEST_VERIFY(r.is_eof());

}

void test_firebird_class::cleanup_class()
{
    TEST_FOR_NO_EXCEPTION(execute_ddl(*con, "drop table test_table;"));
    TEST_FOR_NO_EXCEPTION(execute_ddl(*con, "drop table config_table;"));
    TEST_FOR_NO_EXCEPTION(execute_ddl(*con, "drop procedure test_proc;"));
    TEST_FOR_NO_EXCEPTION(con->close());
}
