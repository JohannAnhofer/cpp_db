#include "sqlite_transaction.h"
#include "db_exception.h"

#include <cstring>

namespace cpp_db
{

void throw_db_exception(int error_code, sqlite3 *db);

sqlite_transaction::sqlite_transaction(const handle &conn_handle)
    : db(std::static_pointer_cast<sqlite3>(conn_handle))
    , open_count(0)
{
    if (db.expired())
        throw db_exception("No database connection!");
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
    int status = sqlite3_prepare(db.lock().get(), sql, strlen(sql), &stmt, &tail);
    if (status != SQLITE_OK && status != SQLITE_DONE)
        throw_db_exception(status, db.lock().get());
    auto deleter = [](sqlite3_stmt*s){sqlite3_finalize(s); };
    std::unique_ptr<sqlite3_stmt, decltype(deleter)> pstmt(stmt, deleter);
    status = sqlite3_step(stmt);
    if (status != SQLITE_OK && status != SQLITE_DONE)
        throw_db_exception(status, db.lock().get());
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
