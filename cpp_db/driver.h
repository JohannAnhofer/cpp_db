#ifndef CPP_DB_DRIVER_H
#define CPP_DB_DRIVER_H

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
    virtual ~driver() = 0;

    connection_interface *make_connection() const = 0;
    statement_interface *make_statement() const = 0;
    parameters_interface *make_parameters() const = 0;
    result_interface *make_result() const = 0;
    transaction_interface *make_transaction() const = 0;
};

driver::~driver()
{
}

}

#endif // CPP_DB_DRIVER_H
