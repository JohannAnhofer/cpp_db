#include "odbc_driver.h"
#include "odbc_connection.h"
#include "odbc_statement.h"
#include "odbc_parameters.h"
#include "odbc_result.h"
#include "odbc_transaction.h"

namespace cpp_db
{

connection_interface *odbc_driver::make_connection() const
{
    return new odbc_connection;
}

statement_interface *odbc_driver::make_statement(const shared_connection_ptr &conn) const
{
    return new odbc_statement{conn};
}

parameters_interface *odbc_driver::make_parameters(const shared_statement_ptr &stmt) const
{
    return new odbc_parameters{stmt};
}

result_interface *odbc_driver::make_result(const shared_statement_ptr &stmt) const
{
    return new odbc_result{stmt};
}

transaction_interface *odbc_driver::make_transaction(const shared_connection_ptr &conn) const
{
    return new odbc_transaction{conn};
}

} // namespace cpp_db
