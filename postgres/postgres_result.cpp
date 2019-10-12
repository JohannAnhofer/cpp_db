#include "postgres_result.h"
#include "postgres_statement.h"
#include "null.h"
#include "value.h"
#include "postgres_exception.h"

namespace cpp_db
{

postgres_result::postgres_result(const shared_statement_ptr &stmt_in)
    : stmt{std::static_pointer_cast<postgres_statement>(stmt_in)}
    , number_of_rows{0}
    , row_position{0}
{
    stmt->execute();
    number_of_rows = PQntuples(get_pg_result());
}

void postgres_result::move_next()
{
    if (row_position < number_of_rows)
        row_position++;
    else
        throw postgres_exception("End of result set reached!");
}

bool postgres_result::is_eof() const
{
    return row_position >= number_of_rows;
}

int postgres_result::get_column_count() const
{
    return PQnfields(get_pg_result());
}

std::string postgres_result::get_column_name(int column) const
{
    const char *name = PQfname(get_pg_result(), column);

    if (name == nullptr)
        throw postgres_exception("Invalid column number!");
    else
        return name;
}

value postgres_result::get_column_value(int column) const
{
    if (is_eof())
        throw postgres_exception("End of result set reached!");

    return std::string(PQgetvalue(get_pg_result(), row_position, column));
}

int postgres_result::get_column_index(const std::string &column_name) const
{
    int column_number = PQfnumber(get_pg_result(), column_name.c_str());
    if (column_number == -1)
        throw postgres_exception("Invalid column name!");
    else
        return column_number;
}

value postgres_result::get_column_value(const std::string &column_name) const
{
    return get_column_value(get_column_index(column_name));
}

bool postgres_result::is_column_null(int column) const
{
    if (is_eof())
        throw postgres_exception("End of result set reached!");
    return PQgetisnull(get_pg_result(), row_position, column) != 0;
}

bool postgres_result::is_column_null(const std::string &column_name) const
{
    return is_column_null(get_column_index(column_name));
}

handle postgres_result::get_handle() const
{
    return std::static_pointer_cast<void>(stmt->stmt);
}

const PGresult *postgres_result::get_pg_result() const
{
    return stmt->stmt.get();
}

} // namespace cpp_db
