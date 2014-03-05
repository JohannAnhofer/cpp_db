#include "postgres_driver.h"
#include "postgres_connection.h"

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
        (void)conn;
        return nullptr;// new postgres_statement(conn, shared_transaction_ptr(make_transaction(conn)));
    }

    parameters_interface *postgres_driver::make_parameters(const shared_statement_ptr &stmt) const
    {
        (void)stmt;
        return nullptr; // new postgres_parameters(stmt);
    }

    result_interface *postgres_driver::make_result(const shared_statement_ptr &stmt) const
    {
        (void)stmt;
        return nullptr; // new postgres_result(stmt);
    }

    transaction_interface *postgres_driver::make_transaction(const shared_connection_ptr &conn) const
    {
        (void)conn;
        return nullptr; // new postgres_transaction(conn);
    }

    postgres_driver *postgres_driver::create()
    {
        return new postgres_driver;
    }
}
