#include "postgres_statement.h"
#include "driver_interface.h"
#include "connection_interface.h"
#include "lock_or_throw.h"
#include "postgres_exception.h"

#include <functional>

namespace cpp_db
{

postgres_statement::postgres_statement(const shared_connection_ptr &conn)
    : conn_impl(conn)
    , prepared{false}
{
    if (conn_impl.expired())
        throw postgres_exception("No database connection for statement!");
}

postgres_statement::~postgres_statement()
{
}

PGconn *postgres_statement::get_db_handle() const
{
    return std::static_pointer_cast<PGconn>(tools::lock_or_throw(conn_impl, "Invalid database connection")->get_handle()).get();
}

void postgres_statement::prepare(const std::string &sqlcmd)
{
    sql = sqlcmd;
    prepared = true;
}

bool postgres_statement::is_prepared() const
{
    return prepared;
}

void postgres_statement::execute_ddl()
{
    execute_non_query();
}

void postgres_statement::execute_non_query()
{
    PGconn *conn = get_db_handle();
    stmt.reset(PQexec(conn, sql.c_str()), &PQclear);
    if (PQresultStatus(stmt.get()) != PGRES_COMMAND_OK)
        throw postgres_exception(PQerrorMessage(conn));
}

void postgres_statement::execute()
{
    // this should be replaced with send query
    PGconn *conn = get_db_handle();
    stmt.reset(PQexec(conn, sql.c_str()), &PQclear);
    if (PQresultStatus(stmt.get()) != PGRES_TUPLES_OK)
        throw postgres_exception(PQerrorMessage(conn));
}

void postgres_statement::reset()
{
    stmt.reset();
}

handle postgres_statement::get_handle() const
{
    handle{};
}

} // namespace cpp_db
