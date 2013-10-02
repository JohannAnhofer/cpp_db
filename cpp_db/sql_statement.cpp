#include "sql_statement.h"
#include "connection.h"
#include "db_exception.h"

#include "sqlite3.h"

namespace cpp_db
{

extern void throw_db_exception(int error_code, sqlite3 *db);

struct sql_statement::impl
{
    impl()
        : stmt(nullptr)
        , db(nullptr)
        , tail(nullptr)
    {
    }
    sqlite3_stmt *stmt;
    sqlite3 *db;
    const char *tail;

    void finalize()
    {
        if (stmt != nullptr)
        {
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }
    }
};

sql_statement::sql_statement(connection &conn)
    : pimpl(new impl)
{
    pimpl->db = static_cast<sqlite3*>(conn.get_handle());
    if (!pimpl->db)
        throw db_exception("No database connection for statement!");
}

sql_statement::sql_statement(const std::string &sqlcmd, connection &conn)
    : sql_statement(conn)
{
    prepare(sqlcmd);
}

sql_statement::~sql_statement()
{
    pimpl->finalize();
}

void sql_statement::prepare(const std::string &sqlcmd)
{
    sqlite3_stmt *stmt = nullptr;
    const char *tail = nullptr;

    int error_code = sqlite3_prepare_v2(pimpl->db, sqlcmd.c_str(), sqlcmd.size(), &stmt, &tail);
    if (error_code != SQLITE_OK)
        throw_db_exception(error_code, pimpl->db);

    pimpl->finalize();
    pimpl->stmt = stmt;
    pimpl->tail = tail;
}

void sql_statement::execute_ddl()
{
	execute();
}

void sql_statement::execute_non_query()
{
	execute();
}

void sql_statement::execute()
{
    if (!is_prepared())
        throw db_exception("Statement not prepared!");

	if (int error_code = sqlite3_step(pimpl->stmt))
	{
		if (error_code != SQLITE_DONE && error_code != SQLITE_ROW)
			throw_db_exception(error_code, pimpl->db);
	}
}

bool sql_statement::is_prepared() const
{
    return pimpl->stmt != nullptr;
}

}
