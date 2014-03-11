#include "postgres_driver.h"
#include "postgres_connection.h"
#include "postgres_transaction.h"
#include "postgres_statement.h"
#include "postgres_result.h"
#include "postgres_parameters.h"

namespace cpp_db
{
    //  postgres_driver

    postgres_driver::postgres_driver()
    {
    }

    postgres_driver::~postgres_driver()
    {
    }

    connection_interface *postgres_driver::make_connection() const
    {
        return new postgres_connection;
    }

    statement_interface *postgres_driver::make_statement(const shared_connection_ptr &conn) const
    {
        return new postgres_statement(conn);
    }

    parameters_interface *postgres_driver::make_parameters(const shared_statement_ptr &stmt) const
    {
        return new postgres_parameters(stmt);
    }

    result_interface *postgres_driver::make_result(const shared_statement_ptr &stmt) const
    {
        return new postgres_result(stmt);
    }

    transaction_interface *postgres_driver::make_transaction(const shared_connection_ptr &conn) const
    {
        return new postgres_transaction(conn);
    }

    postgres_driver *postgres_driver::create()
    {
        return new postgres_driver;
    }
}
