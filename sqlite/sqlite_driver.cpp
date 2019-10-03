#include "sqlite_driver.h"
#include "db_exception.h"
#include "sqlite_connection.h"
#include "sqlite_statement.h"
#include "sqlite_parameters.h"
#include "sqlite_result.h"
#include "sqlite_transaction.h"

#include "sqlite3.h"

#include <sstream>

namespace cpp_db
{
	//  sqlite_driver

    sqlite_driver::~sqlite_driver() = default;

    std::unique_ptr<connection_interface> sqlite_driver::make_connection() const
	{
        return std::unique_ptr<connection_interface>(new sqlite_connection);
	}

    std::unique_ptr<statement_interface> sqlite_driver::make_statement(const shared_connection_ptr &conn) const
	{
        return std::unique_ptr<statement_interface>{new sqlite_statement{conn}};
	}

    std::unique_ptr<parameters_interface> sqlite_driver::make_parameters(const shared_statement_ptr &stmt) const
	{
        return std::unique_ptr<parameters_interface>{new sqlite_parameters{stmt}};
	}

    std::unique_ptr<result_interface> sqlite_driver::make_result(const shared_statement_ptr &stmt) const
	{
        return std::unique_ptr<result_interface>{new sqlite_result{stmt}};
	}

    std::unique_ptr<transaction_interface> sqlite_driver::make_transaction(const shared_connection_ptr &conn) const
	{
        return std::unique_ptr<transaction_interface>{new sqlite_transaction{conn}};
    }

    std::unique_ptr<sqlite_driver> sqlite_driver::create()
    {
        return std::unique_ptr<sqlite_driver>{new sqlite_driver};
    }
}
