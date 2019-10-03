#include "postgres_driver.h"
#include "postgres_connection.h"
#include "postgres_transaction.h"
#include "postgres_statement.h"
#include "postgres_result.h"
#include "postgres_parameters.h"

namespace cpp_db
{
    //  postgres_driver

    postgres_driver::postgres_driver() = default;
    postgres_driver::~postgres_driver() = default;

    std::unique_ptr<connection_interface> postgres_driver::make_connection() const
    {
        return std::unique_ptr<connection_interface>{new postgres_connection};
    }

    std::unique_ptr<statement_interface> postgres_driver::make_statement(const shared_connection_ptr &conn) const
    {
        return std::unique_ptr<statement_interface>{new postgres_statement(conn)};
    }

    std::unique_ptr<parameters_interface> postgres_driver::make_parameters(const shared_statement_ptr &stmt) const
    {
        return std::unique_ptr<parameters_interface>{new postgres_parameters(stmt)};
    }

    std::unique_ptr<result_interface> postgres_driver::make_result(const shared_statement_ptr &stmt) const
    {
        return std::unique_ptr<result_interface>{new postgres_result(stmt)};
    }

    std::unique_ptr<transaction_interface> postgres_driver::make_transaction(const shared_connection_ptr &conn) const
    {
        return std::unique_ptr<transaction_interface>{new postgres_transaction(conn)};
    }

    std::unique_ptr<postgres_driver> postgres_driver::create()
    {
        return std::unique_ptr<postgres_driver>{new postgres_driver};
    }
}
