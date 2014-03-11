#include "postgres_result.h"
#include "postgres_statement.h"
#include "null.h"
#include "value.h"
#include "postgres_exception.h"

namespace cpp_db
{

postgres_result::postgres_result(const shared_statement_ptr &stmt_in)
    : stmt{std::static_pointer_cast<postgres_statement>(stmt_in)}
{
    stmt->execute();
}

void postgres_result::move_next()
{
    throw postgres_exception("Not implemented, yet!");
}

bool postgres_result::is_eof() const
{
    throw postgres_exception("Not implemented, yet!");
}

int postgres_result::get_column_count() const
{
    return 0;
}

std::string postgres_result::get_column_name(int column) const
{
    (void)column;
    return std::string{};
}

value postgres_result::get_column_value(int column) const
{
    (void)column;
    return null_type{};
}

int postgres_result::get_column_index(const std::string &column_name) const
{
    (void)column_name;
    throw db_exception("Column not found");
}

value postgres_result::get_column_value(const std::string &column_name) const
{
    (void)column_name;
    return null_type{};
}

bool postgres_result::is_column_null(int column) const
{
    (void)column;
    return true;
}

bool postgres_result::is_column_null(const std::string &column_name) const
{
    (void)column_name;
    return true;
}

handle postgres_result::get_handle() const
{
    return handle{};
}

} // namespace cpp_db
