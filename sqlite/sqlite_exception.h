#ifndef CPP_DB_SQLITE_EXCEPTION_H
#define CPP_DB_SQLITE_EXCEPTION_H

#include "db_exception.h"

struct sqlite3;

namespace cpp_db
{

class sqlite_exception : public db_exception
{
public:
    sqlite_exception(int error, sqlite3 *db);

    int get_error_code() const;

private:
    int error_code;
};

} // namespace cpp_db

#endif // CPP_DB_SQLITE_EXCEPTION_H
