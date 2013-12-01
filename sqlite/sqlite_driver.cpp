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

	sqlite_driver::~sqlite_driver()
	{
	}

	connection_interface *sqlite_driver::make_connection() const 
	{
		return new sqlite_connection;
	}

    statement_interface *sqlite_driver::make_statement(const shared_connection_ptr &conn) const
	{
        return new sqlite_statement(conn);
	}

    parameters_interface *sqlite_driver::make_parameters(const shared_statement_ptr &stmt) const
	{
        return new sqlite_parameters(stmt);
	}

    result_interface *sqlite_driver::make_result(const shared_statement_ptr &stmt) const
	{
        return new sqlite_result(stmt);
	}

    transaction_interface *sqlite_driver::make_transaction(const shared_connection_ptr &conn) const
	{
        return new sqlite_transaction(conn);
    }

    sqlite_driver *sqlite_driver::create()
    {
        return new sqlite_driver;
    }
}
