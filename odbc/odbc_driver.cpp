#include "odbc_driver.h"
#include "odbc_connection.h"
#include "odbc_statement.h"
#include "odbc_parameters.h"
#include "odbc_result.h"
#include "odbc_transaction.h"

namespace cpp_db
{

std::unique_ptr<connection_interface> odbc_driver::make_connection() const
{
    return std::unique_ptr<connection_interface>{new odbc_connection};
}

std::unique_ptr<statement_interface> odbc_driver::make_statement(const shared_connection_ptr &conn) const
{
    return std::unique_ptr<statement_interface>{new odbc_statement{conn}};
}

std::unique_ptr<parameters_interface> odbc_driver::make_parameters(const shared_statement_ptr &stmt) const
{
    return std::unique_ptr<parameters_interface>{new odbc_parameters{stmt}};
}

std::unique_ptr<result_interface> odbc_driver::make_result(const shared_statement_ptr &stmt) const
{
    return std::unique_ptr<result_interface>{new odbc_result{stmt}};
}

std::unique_ptr<transaction_interface> odbc_driver::make_transaction(const shared_connection_ptr &conn) const
{
    return std::unique_ptr<transaction_interface>{new odbc_transaction{conn}};
}

std::unique_ptr<odbc_driver> odbc_driver::create()
{
    return std::unique_ptr<odbc_driver>{new odbc_driver};
}

} // namespace cpp_db
