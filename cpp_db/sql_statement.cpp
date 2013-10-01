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
};

sql_statement::sql_statement(const std::string &sqlcmd, connection &conn)
    : pimpl(new impl)
{
    pimpl->db = static_cast<sqlite3*>(conn.get_handle());
    int error_code = sqlite3_prepare_v2(pimpl->db, sqlcmd.c_str(), sqlcmd.size(), &pimpl->stmt, &pimpl->tail);
    if (error_code != SQLITE_OK)
        throw_db_exception(error_code, pimpl->db);
}

sql_statement::~sql_statement()
{
    if (pimpl->stmt != nullptr)
        sqlite3_finalize(pimpl->stmt);
}

void sql_statement::execute()
{
	if (int error_code = sqlite3_step(pimpl->stmt))
	{
		if (error_code != SQLITE_DONE)
			throw_db_exception(error_code, pimpl->db);
	}
}

}
