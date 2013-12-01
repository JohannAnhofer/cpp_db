#include "firebird_driver.h"
#include "firebird_connection.h"
#include "firebird_transaction.h"
#include "firebird_statement.h"
#include "firebird_parameters.h"
#include "firebird_result.h"

namespace cpp_db
{
    //  firebird_driver

    firebird_driver::firebird_driver()
    {
    }

    firebird_driver::~firebird_driver()
    {
    }

    connection_interface *firebird_driver::make_connection() const
    {
        return new firebird_connection;
    }

    statement_interface *firebird_driver::make_statement(const shared_connection_ptr &conn) const
    {
        return new firebird_statement(conn, shared_transaction_ptr(make_transaction(conn)));
    }

    parameters_interface *firebird_driver::make_parameters(const shared_statement_ptr &stmt) const
    {
        return new firebird_parameters(stmt);
    }

    result_interface *firebird_driver::make_result(const shared_statement_ptr &stmt) const
    {
        return new firebird_result(stmt);
    }

    transaction_interface *firebird_driver::make_transaction(const shared_connection_ptr &conn) const
    {
        return new firebird_transaction(conn);
    }

    firebird_driver *firebird_driver::create()
    {
        return new firebird_driver;
    }
}
