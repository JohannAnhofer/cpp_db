#ifndef CPP_DB_FIREBIRD_DRIVER_H
#define CPP_DB_FIREBIRD_DRIVER_H

#include "driver.h"

namespace cpp_db
{

class connection;

class firebird_driver : public driver
{
public:
    virtual ~firebird_driver();

    connection_interface *make_connection() const override;
    statement_interface *make_statement(const handle &conn_handle) const override;
    parameters_interface *make_parameters(const handle &stmt_handle) const override;
    result_interface *make_result(const handle &stmt_handle) const override;
    transaction_interface *make_transaction(const handle &conn_handle) const override;
private:
    firebird_driver();
    friend class connection;
};

}

#endif // CPP_DB_FIREBIRD_DRIVER_H
