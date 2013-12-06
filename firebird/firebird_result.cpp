#include "firebird_result.h"
#include "firebird_statement.h"
#include "null.h"
#include "value.h"
#include "firebird_exception.h"
#include "isc_status.h"

#include <ctime>

namespace cpp_db
{

firebird_result::firebird_result(const shared_statement_ptr &stmt_in)
    : stmt{std::static_pointer_cast<firebird_statement>(stmt_in)}
    , sqlda_fields(stmt->access_sqlda_out())
    , after_last_row{false}
{
    stmt->execute();
    if (stmt->is_select_statement())
        move_next();
}

isc_stmt_handle *firebird_result::get_statement_handle() const
{
    return std::static_pointer_cast<isc_stmt_handle>(stmt->get_handle()).get();
}

void firebird_result::move_next()
{
    if (!after_last_row)
    {
        if (stmt->is_select_statement())
        {
            isc_status status;

            after_last_row = isc_dsql_fetch(static_cast<ISC_STATUS *>(status), get_statement_handle(), xsqlda::version, static_cast<XSQLDA*>(*sqlda_fields)) == 100L;
            if (!after_last_row && status.has_error())
                throw firebird_exception(status);
        }
        else
        {
            after_last_row = true;
            sqlda_fields->reset_values();
        }
    }
}

bool firebird_result::is_eof() const
{
    return after_last_row;
}

int firebird_result::get_column_count() const
{
    return sqlda_fields->get_var_count();
}

std::string firebird_result::get_column_name(int column) const
{
    return (*sqlda_fields)[column].get_column_name();
}

value firebird_result::get_column_value(int column) const
{
    return (*sqlda_fields)[column].get_column_value();
}

int firebird_result::get_column_index(const std::string &column_name) const
{
    for (int col = 0; col < get_column_count(); ++col)
    {
        if (get_column_name(col) == column_name)
            return col;
    }
    throw db_exception("Column not found");
}

value firebird_result::get_column_value(const std::string &column_name) const
{
    return get_column_value(get_column_index(column_name));
}

bool firebird_result::is_column_null(int column) const
{
    return (*sqlda_fields)[column].is_null();
}

bool firebird_result::is_column_null(const std::string &column_name) const
{
    return is_column_null(get_column_index(column_name));
}

handle firebird_result::get_handle() const
{
    return handle{};
}

} // namespace cpp_db
