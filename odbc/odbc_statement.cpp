#include "odbc_statement.h"

namespace cpp_db
{

odbc_statement::odbc_statement(const shared_connection_ptr &connection)
    : conn_impl(connection)
{
}

odbc_statement::~odbc_statement()
{
}

void odbc_statement::prepare(const std::string &sqlcmd)
{
    (void)sqlcmd;
}

bool odbc_statement::is_prepared() const
{
    return false;
}

void odbc_statement::execute_ddl()
{
}

void odbc_statement::execute_non_query()
{
}

void odbc_statement::execute()
{
}

void odbc_statement::reset()
{
}

handle odbc_statement::get_handle() const
{
    return handle{};
}

} // namespace cpp_db
