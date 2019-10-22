#ifndef CPP_DB_FIREBIRD_DRIVER_H
#define CPP_DB_FIREBIRD_DRIVER_H

#include "driver_interface.h"

namespace cpp_db
{

class connection;

class firebird_driver : public driver_interface
{
public:
    ~firebird_driver() override;

    std::unique_ptr<connection_interface> make_connection() const override;
    std::unique_ptr<statement_interface> make_statement(const shared_connection_ptr &conn) const override;
    std::unique_ptr<parameters_interface> make_parameters(const shared_statement_ptr &stmt) const override;
    std::unique_ptr<result_interface> make_result(const shared_statement_ptr &stmt) const override;
    std::unique_ptr<transaction_interface> make_transaction(const shared_connection_ptr &conn) const override;

    static std::unique_ptr<firebird_driver> create();

private:
    firebird_driver();
};

}

#endif // CPP_DB_FIREBIRD_DRIVER_H
