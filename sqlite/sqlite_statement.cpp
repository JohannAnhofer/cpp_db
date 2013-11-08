#include "sqlite_statement.h"
#include "connection_interface.h"
#include "db_exception.h"

namespace cpp_db
{

void throw_db_exception(int error_code, sqlite3 *db);

sqlite_statement::sqlite_statement(const shared_connection_ptr &conn_in)
	: conn(conn_in)
    , tail(nullptr)
{
	if (conn.expired())
        throw db_exception("No database connection for statement!");
}

sqlite3 *sqlite_statement::get_db_handle() const
{
	return std::static_pointer_cast<sqlite3>(conn.lock()->get_handle()).get();
}

void sqlite_statement::prepare(const std::string &sqlcmd)
{
    sqlite3_stmt *stmt_new = nullptr;
    const char *tail_new = nullptr;

	int error_code = sqlite3_prepare_v2(get_db_handle(), sqlcmd.c_str(), sqlcmd.size(), &stmt_new, &tail_new);
    if (error_code != SQLITE_OK)
		throw_db_exception(error_code, get_db_handle());

    stmt.reset(stmt_new, sqlite3_finalize);
    tail = tail_new;
}

bool sqlite_statement::is_prepared() const
{
    return stmt != nullptr;
}

void sqlite_statement::execute()
{
    if (!is_prepared())
        throw db_exception("Statement not prepared!");

    if (int error_code = sqlite3_step(stmt.get()))
    {
        if (error_code != SQLITE_DONE && error_code != SQLITE_ROW)
			throw_db_exception(error_code, get_db_handle());
    }
}

void sqlite_statement::reset()
{
    sqlite3_reset(stmt.get());
}

handle sqlite_statement::get_handle() const
{
    return std::static_pointer_cast<void>(stmt);
}

} // namespace cpp_db
