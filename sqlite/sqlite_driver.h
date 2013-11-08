#ifndef CPP_DB_SQLITE_DRIVER_H
#define CPP_DB_SQLITE_DRIVER_H

#include "driver.h"

namespace cpp_db
{

class connection;

class sqlite_driver : public driver
{
public:
	virtual ~sqlite_driver();

	connection_interface *make_connection() const override;
    statement_interface *make_statement(const connection_handle &conn) const override;
    parameters_interface *make_parameters(const statement_handle &stmt) const override;
    result_interface *make_result(const statement_handle &stmt) const override;
    transaction_interface *make_transaction(const connection_handle &conn) const override;

private:
    sqlite_driver();
    friend class connection;
};

}

#endif
