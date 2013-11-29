#ifndef CPP_DB_DRIVER_H
#define CPP_DB_DRIVER_H

#include <memory>
#include "usings.h"

namespace cpp_db
{

struct driver_interface
{
    virtual ~driver_interface() {}
    virtual connection_interface *make_connection() const = 0;
    virtual statement_interface *make_statement(const shared_connection_ptr &conn) const = 0;
    virtual parameters_interface *make_parameters(const shared_statement_ptr &stmt) const = 0;
    virtual result_interface *make_result(const shared_statement_ptr &stmt) const = 0;
    virtual transaction_interface *make_transaction(const shared_connection_ptr &conn) const = 0;
};

}

#endif // CPP_DB_DRIVER_H
