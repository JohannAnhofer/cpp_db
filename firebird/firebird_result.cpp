#include "firebird_result.h"
#include "statement_interface.h"
#include "null.h"
#include "value.h"

namespace cpp_db
{

firebird_result::firebird_result(const shared_statement_ptr &stmt_in)
    : stmt(stmt_in)
{
}

isc_stmt_handle *firebird_result::get_statement_handle() const
{
    return std::static_pointer_cast<isc_stmt_handle>(stmt->get_handle()).get();
}

void firebird_result::move_next()
{
}

void firebird_result::move_prev()
{
}

void firebird_result::move_first()
{
}

bool firebird_result::is_eof() const
{
    return true;
}

int firebird_result::get_column_count() const
{
    return 0;
}

std::string firebird_result::get_column_name(int column) const
{
    (void)column;
    return std::string{};
}

value firebird_result::get_column_value(int column) const
{
    (void)column;
    return null_type{};
}

int firebird_result::get_column_index(const std::string &column_name) const
{
    (void)column_name;
    return -1;
}

value firebird_result::get_column_value(const std::string &column_name) const
{
    (void)column_name;
    return null_type{};
}

} // namespace cpp_db
