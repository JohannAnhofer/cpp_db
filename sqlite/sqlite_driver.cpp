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
	void throw_db_exception(int error_code, sqlite3 *db)
	{
		std::stringstream sstr;
        sstr << sqlite3_errmsg(db) << " (" << error_code << ")";
		throw db_exception(sstr.str());
	}

	//  sqlite_driver

	sqlite_driver::sqlite_driver()
	{
	}


	sqlite_driver::~sqlite_driver()
	{
	}

	connection_interface *sqlite_driver::make_connection() const 
	{
		return new sqlite_connection;
	}

    statement_interface *sqlite_driver::make_statement(const connection_handle &conn) const
	{
        return new sqlite_statement(conn);
	}

    parameters_interface *sqlite_driver::make_parameters(const statement_handle &stmt) const
	{
        return new sqlite_parameters(stmt);
	}

    result_interface *sqlite_driver::make_result(const statement_handle &stmt) const
	{
        return new sqlite_result(stmt);
	}

    transaction_interface *sqlite_driver::make_transaction(const connection_handle &conn) const
	{
        return new sqlite_transaction(conn);
	}
}
