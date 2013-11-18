#include "odbc_connection.h"

namespace cpp_db
{

odbc_connection::odbc_connection()
{
}

odbc_connection::~odbc_connection()
{
}

void odbc_connection::open(const std::string &database, const authentication &auth, const key_value_pair &options)
{
    (void)database;
    (void)auth;
    (void)options;
}

void odbc_connection::close()
{
}

bool odbc_connection::is_open() const
{
    return false;
}

handle odbc_connection::get_handle() const
{
    return handle{};
}

void odbc_connection::set_current_transaction(const shared_transaction_ptr &trans)
{
    current_transaction = trans;
}

shared_transaction_ptr odbc_connection::get_current_transaction() const
{
    return current_transaction.lock();
}


} // namespace cpp_db
