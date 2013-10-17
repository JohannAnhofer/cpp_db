#include "sqlite_connection.h"

#include "sqlite3.h"

#include <stdexcept>

namespace cpp_db
{

void throw_db_exception(int error_code, sqlite3 *db);

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

void sqlite_connection::open(const std::string &database)
{
    if (is_open())
        throw std::runtime_error("Database already open");

    sqlite3 *dbptr(nullptr);
    if (int error_code = sqlite3_open_v2(database.c_str(), &dbptr, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr))
        throw_db_exception(error_code, dbptr);
    db.reset(dbptr, [](sqlite3 *db)
    {
        if (int error_code = sqlite3_close(db))
            throw_db_exception(error_code, db);
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

}
