#include "firebird_driver.h"
#include "firebird_connection.h"
#include "firebird_transaction.h"
#include "firebird_statement.h"
#include "firebird_parameters.h"
#include "firebird_result.h"

namespace cpp_db
{
    //  firebird_driver

    firebird_driver::firebird_driver() = default;
    firebird_driver::~firebird_driver() = default;

    std::unique_ptr<connection_interface> firebird_driver::make_connection() const
    {
        return std::unique_ptr<connection_interface>{new firebird_connection};
    }

    std::unique_ptr<statement_interface> firebird_driver::make_statement(const shared_connection_ptr &conn) const
    {
        return std::unique_ptr<statement_interface>{new firebird_statement(conn, shared_transaction_ptr(make_transaction(conn)))};
    }

    std::unique_ptr<parameters_interface> firebird_driver::make_parameters(const shared_statement_ptr &stmt) const
    {
        return std::unique_ptr<parameters_interface>{new firebird_parameters(stmt)};
    }

    std::unique_ptr<result_interface> firebird_driver::make_result(const shared_statement_ptr &stmt) const
    {
        return std::unique_ptr<result_interface>{new firebird_result(stmt)};
    }

    std::unique_ptr<transaction_interface> firebird_driver::make_transaction(const shared_connection_ptr &conn) const
    {
        return std::unique_ptr<transaction_interface>{new firebird_transaction(conn)};
    }

    std::unique_ptr<firebird_driver> firebird_driver::create()
    {
        return std::unique_ptr<firebird_driver>{new firebird_driver};
    }
}
