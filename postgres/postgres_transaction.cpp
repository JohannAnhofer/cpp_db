#include "postgres_transaction.h"
#include "connection_interface.h"
#include "postgres_exception.h"
#include "lock_or_throw.h"

namespace
{
    class pgresult
    {
    public:
        explicit pgresult(PGresult *res) : result(res)
        {
        }
        explicit operator PGresult *()
        {
            return result;
        }
        ~pgresult()
        {
            PQclear(result);
        }

    private:
        PGresult *result;
    };

    void execute_transaction_function(PGconn *conn, const char *command);
}

namespace cpp_db
{

postgres_transaction::postgres_transaction(const shared_connection_ptr &conn_handle)
    : conn_impl{conn_handle}
{
    if (conn_impl.expired())
        throw postgres_exception("No database connection!");
}

postgres_transaction::~postgres_transaction()
{
    try
    {
        rollback();
    }
    catch (...)
    {
    }
}

handle postgres_transaction::get_handle() const
{
    return handle{};    // postgres has no native transaction handle
}

void postgres_transaction::begin()
{
    if (!is_open())
        execute_transaction_function(get_db_handle(), "BEGIN TRANSACTION");
}

void postgres_transaction::commit()
{
    if (is_open())
        execute_transaction_function(get_db_handle(), "COMMIT TRANSACTION");
}

void postgres_transaction::rollback()
{
    if (is_open())
        execute_transaction_function(get_db_handle(), "ROLLBACK TRANSACTION");
}

bool postgres_transaction::is_open() const
{
    return PQtransactionStatus(get_db_handle()) != PQTRANS_IDLE;
}

PGconn *postgres_transaction::get_db_handle() const
{
    return std::static_pointer_cast<PGconn>(tools::lock_or_throw(conn_impl, "Invalid database connection")->get_handle()).get();
}

}

namespace
{

void execute_transaction_function(PGconn *conn, const char *command)
{
    pgresult result{PQexec(conn, command)};

    if (PQresultStatus(static_cast<PGresult *>(result)) != PGRES_COMMAND_OK)
        throw cpp_db::postgres_exception(PQerrorMessage(conn));
}

}
