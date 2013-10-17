#ifndef CPP_DB_DRIVER_H
#define CPP_DB_DRIVER_H

#include "usings.h"

namespace cpp_db
{

struct connection_interface;
struct statement_interface;
struct parameters_interface;
struct result_interface;
struct transaction_interface;

class driver
{
public:
	virtual ~driver() {}

    virtual connection_interface *make_connection() const = 0;
	virtual statement_interface *make_statement(const handle &conn_handle) const = 0;
	virtual parameters_interface *make_parameters(const handle &stmt_handle) const = 0;
	virtual result_interface *make_result(const handle &stmt_handle) const = 0;
	virtual transaction_interface *make_transaction(const handle &conn_handle) const = 0;
};

}

#endif // CPP_DB_DRIVER_H
