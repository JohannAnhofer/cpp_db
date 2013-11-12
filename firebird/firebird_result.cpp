#include "firebird_result.h"
#include "firebird_statement.h"
#include "null.h"
#include "value.h"
#include "db_exception.h"
#include "isc_status.h"

#include <ctime>
#include <iostream>

namespace cpp_db
{

firebird_result::firebird_result(const shared_statement_ptr &stmt_in)
    : stmt{stmt_in}
    , sqlda_fields(std::static_pointer_cast<firebird_statement>(stmt_in)->access_sqlda_out())
    , after_last_row{false}
{
//    move_next();
}

isc_stmt_handle *firebird_result::get_statement_handle() const
{
    return std::static_pointer_cast<isc_stmt_handle>(stmt->get_handle()).get();
}

void firebird_result::move_next()
{
    isc_status status;

    after_last_row = isc_dsql_fetch(static_cast<ISC_STATUS *>(status), get_statement_handle(), xsqlda::version, static_cast<XSQLDA*>(*sqlda_fields)) == 100L;
    if (!after_last_row)
        status.throw_db_exception_on_error();
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

} // namespace cpp_db
