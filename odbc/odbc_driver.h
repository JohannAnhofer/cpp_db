#ifndef CPP_DB_ODBC_DRIVER_H
#define CPP_DB_ODBC_DRIVER_H

#include "driver_interface.h"

namespace cpp_db
{

class connection;

class odbc_driver : public driver_interface
{
public:
    connection_interface *make_connection() const override;
    statement_interface *make_statement(const shared_connection_ptr &conn) const override;
    parameters_interface *make_parameters(const shared_statement_ptr &stmt) const override;
    result_interface *make_result(const shared_statement_ptr &stmt) const override;
    transaction_interface *make_transaction(const shared_connection_ptr &conn) const override;

private:
    odbc_driver() = default;
    friend class connection;
};

} // namespace cpp_db

#endif // CPP_DB_ODBC_DRIVER_H
