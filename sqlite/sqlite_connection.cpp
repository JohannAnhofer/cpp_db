#include "sqlite_connection.h"
#include "sqlite_exception.h"

namespace cpp_db
{

sqlite_connection::~sqlite_connection()
{
    try
    {
        db.reset();
    }
    catch (...)
    {
    }
}

void sqlite_connection::open(const std::string &database, const authentication &, const key_value_pair &)
{
    if (is_open())
        throw db_exception("Database already open");

    sqlite3 *dbptr(nullptr);
    if (int error_code = sqlite3_open_v2(database.c_str(), &dbptr, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr))
        throw sqlite_exception(error_code, dbptr);
    db.reset(dbptr, [](sqlite3 *db)
    {
        if (int error_code = sqlite3_close(db))
            throw sqlite_exception(error_code, db);
    }
    );
}

void sqlite_connection::close()
{
    db.reset();
}

bool sqlite_connection::is_open() const
{
    return db != nullptr;
}

handle sqlite_connection::get_handle() const
{
    return std::static_pointer_cast<void>(db);
}

void sqlite_connection::set_current_transaction(const shared_transaction_ptr &trans)
{
    current_transaction = trans;
}

shared_transaction_ptr sqlite_connection::get_current_transaction() const
{
    return current_transaction.lock();
}

}
