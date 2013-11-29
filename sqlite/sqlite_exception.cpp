#include "sqlite_exception.h"

#include "sqlite3.h"

#include <sstream>

namespace cpp_db
{

std::string get_error_text(int error_code, sqlite3 *db)
{
    std::stringstream sstr;
    sstr << sqlite3_errmsg(db) << " (" << error_code << ")";
    return sstr.str();
}

sqlite_exception::sqlite_exception(int error, sqlite3 *db)
    : db_exception(get_error_text(error, db))
    , error_code(error)
{
}

int sqlite_exception::get_error_code() const
{
    return error_code;
}

} // namespace cpp_db
