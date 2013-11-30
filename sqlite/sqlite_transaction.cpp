#include "sqlite_transaction.h"
#include "connection_interface.h"
#include "sqlite_exception.h"
#include "lock_or_throw.h"

#include <cstring>

namespace cpp_db
{

sqlite_transaction::sqlite_transaction(const shared_connection_ptr &conn_in)
	: conn(conn_in)
    , open_count(0)
{
	if (conn.expired())
        throw db_exception("No database connection!");
}

sqlite3 *sqlite_transaction::get_db_handle() const
{
	return std::static_pointer_cast<sqlite3>(tools::lock_or_throw(conn, "Invalid database connection")->get_handle()).get();
}

sqlite_transaction::~sqlite_transaction()
{
    try
    {
        while (is_open())
            rollback();
    }
    catch (...)
    {
    }
}

void sqlite_transaction::execute(const char *sql)
{
    const char *tail = nullptr;
    sqlite3_stmt *stmt = 0;
	int status = sqlite3_prepare(get_db_handle(), sql, static_cast<int>(strlen(sql)), &stmt, &tail);
    if (status != SQLITE_OK && status != SQLITE_DONE)
        throw sqlite_exception(status, get_db_handle());
    auto deleter = [](sqlite3_stmt*s){sqlite3_finalize(s); };
    std::unique_ptr<sqlite3_stmt, decltype(deleter)> pstmt(stmt, deleter);
    status = sqlite3_step(stmt);
    if (status != SQLITE_OK && status != SQLITE_DONE)
        throw sqlite_exception(status, get_db_handle());
}

void sqlite_transaction::begin()
{
    execute("BEGIN TRANSACTION");
    ++open_count;
}

void sqlite_transaction::commit()
{
    if (is_open())
    {
        execute("COMMIT TRANSACTION");
        --open_count;
    }
}

void sqlite_transaction::rollback()
{
    if (is_open())
    {
        execute("ROLLBACK TRANSACTION");
        --open_count;
    }
}

bool sqlite_transaction::is_open() const
{
    return open_count > 0;
}

handle sqlite_transaction::get_handle() const
{
    return handle{};
}

} // namespace cpp_db
