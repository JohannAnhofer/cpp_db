#include "firebird_driver.h"
/*
#include "firebird_connection.h"
#include "firebird_statement.h"
#include "firebird_parameters.h"
#include "firebird_result.h"
#include "firebird_transaction.h"
*/

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
        return nullptr;
//        return new firebird_connection;
    }

    statement_interface *firebird_driver::make_statement(const handle &conn_handle) const
    {
        (void)conn_handle;
        return nullptr;
//        return new firebird_statement(conn_handle);
    }

    parameters_interface *firebird_driver::make_parameters(const handle &stmt_handle) const
    {
        (void)stmt_handle;
        return nullptr;
//        return new firebird_parameters(stmt_handle);
    }

    result_interface *firebird_driver::make_result(const handle &stmt_handle) const
    {
        (void)stmt_handle;
        return nullptr;
//        return new firebird_result(stmt_handle);
    }

    transaction_interface *firebird_driver::make_transaction(const handle &conn_handle) const
    {
        (void)conn_handle;
        return nullptr;
//        return new firebird_transaction(conn_handle);
    }

}
