#include "odbc_result.h"
#include "odbc_statement.h"
#include "value.h"

namespace cpp_db
{

odbc_result::odbc_result(const shared_statement_ptr &stmt_in)
    : stmt{std::static_pointer_cast<odbc_statement>(stmt_in)}
{
}

odbc_result::~odbc_result()
{
}

void odbc_result::move_next()
{
}

bool odbc_result::is_eof() const
{
    return false;
}

int odbc_result::get_column_count() const
{
    return 0;
}

std::string odbc_result::get_column_name(int column) const
{
    (void)column;
    return std::string{};
}

value odbc_result::get_column_value(int column) const
{
    (void)column;
    return null_type{};
}

int odbc_result::get_column_index(const std::string &column_name) const
{
    (void)column_name;
    return -1;
}

value odbc_result::get_column_value(const std::string &column_name) const
{
    (void)column_name;
    return null_type{};
}

bool odbc_result::is_column_null(int column) const
{
    (void)column;
    return false;
}

bool odbc_result::is_column_null(const std::string &column_name) const
{
    (void)column_name;
    return false;
}

handle odbc_result::get_handle() const
{
    return handle{};
}

} // namespace cpp_db
