#include "sqlite_result.h"
#include "statement_interface.h"
#include "sqlite_exception.h"
#include "value.h"

#include <sstream>

namespace cpp_db
{

sqlite_result::sqlite_result(const shared_statement_ptr &stmt_in)
	: stmt(stmt_in)
    , row_status(SQLITE_DONE)
{
    move_next();
    for (int column = 0; column < get_column_count(); ++column)
		column_names.emplace(sqlite3_column_name(get_stmt_handle(), column), column);
}

sqlite3_stmt *sqlite_result::get_stmt_handle() const
{
	return std::static_pointer_cast<sqlite3_stmt>(stmt->get_handle()).get();
}

void sqlite_result::move_next()
{
	row_status = sqlite3_step(get_stmt_handle());
    if (row_status != SQLITE_DONE && row_status != SQLITE_ROW)
        throw sqlite_exception(row_status, sqlite3_db_handle(get_stmt_handle()));
}

bool sqlite_result::is_eof() const
{
    return row_status == SQLITE_DONE;
}

int sqlite_result::get_column_count() const
{
	return sqlite3_column_count(get_stmt_handle());
}

std::string sqlite_result::get_column_name(int column) const
{
	if (const char *column_name = sqlite3_column_name(get_stmt_handle(), column))
        return column_name;
    else
    {
        std::stringstream message;
        message << "Column " << column << " not found!";
        throw db_exception(message.str());
    }
}

value sqlite_result::get_column_value(int column) const
{
	sqlite3_stmt *pstmt = get_stmt_handle();

    switch (sqlite3_column_type(pstmt, column))
    {
    case SQLITE_INTEGER:	// int64
        return static_cast<int64_t>(sqlite3_column_int64(pstmt, column));
    case SQLITE_FLOAT:		// double
        return sqlite3_column_double(pstmt, column);
    case SQLITE_BLOB:		// void *
    {
        const uint8_t *data = static_cast<const uint8_t*>(sqlite3_column_blob(pstmt, column));
        return blob(data, data + sqlite3_column_bytes(pstmt, column));
    }
    case SQLITE_TEXT:		// const char *
        return std::string(reinterpret_cast<const char *>(sqlite3_column_text(pstmt, column)), sqlite3_column_bytes(pstmt, column));
    case SQLITE_NULL:		//
    default:
        break;
    }
    return null_type();
}

int sqlite_result::get_column_index(const std::string &column_name) const
{
    auto pos = column_names.find(column_name);
    if (pos != column_names.end())
        return pos->second;
    std::stringstream message;
    message << "Column '" << column_name << "' not found!";
    throw db_exception(message.str());
}

value sqlite_result::get_column_value(const std::string &column_name) const
{
    return get_column_value(get_column_index(column_name));
}

bool sqlite_result::is_column_null(int column) const
{
    return sqlite3_column_type(get_stmt_handle(), column) == SQLITE_NULL;
}

bool sqlite_result::is_column_null(const std::string &column_name) const
{
    return is_column_null(get_column_index(column_name));
}

handle sqlite_result::get_handle() const
{
    return handle{};
}

} // namespace cpp_db
