#include "odbc_transaction.h"

namespace cpp_db
{

odbc_transaction::odbc_transaction(const shared_connection_ptr &conn_handle)
    : conn_impl(conn_handle)
{
}

odbc_transaction::~odbc_transaction()
{
}

void odbc_transaction::begin()
{
}

void odbc_transaction::commit()
{
}

void odbc_transaction::rollback()
{
}

bool odbc_transaction::is_open() const
{
    return false;
}

handle odbc_transaction::get_handle() const
{
    return handle{};
}

} // namespace cpp_db
