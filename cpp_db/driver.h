#ifndef CPP_DB_DRIVER_H
#define CPP_DB_DRIVER_H

#include <memory>
#include "usings.h"

namespace cpp_db
{

class driver
{
public:
    virtual ~driver() {}

    virtual connection_interface *make_connection() const = 0;
    virtual statement_interface *make_statement(const connection_handle &conn) const = 0;
    virtual parameters_interface *make_parameters(const statement_handle &stmt) const = 0;
    virtual result_interface *make_result(const statement_handle &stmt) const = 0;
    virtual transaction_interface *make_transaction(const connection_handle &conn) const = 0;
};

}

#endif // CPP_DB_DRIVER_H
