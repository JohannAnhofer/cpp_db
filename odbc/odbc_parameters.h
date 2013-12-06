#ifndef CPP_DB_ODBC_PARAMETERS_H
#define CPP_DB_ODBC_PARAMETERS_H

#include "parameters_interface.h"
#include "usings.h"

#include <memory>

namespace cpp_db
{

class odbc_driver;
struct statement_interface;

class odbc_parameters : public parameters_interface
{
public:
    ~odbc_parameters();

    int get_count() const override;
    void bind(const parameter &param) override;
    handle get_handle() const override;

private:
    friend class odbc_driver;
    explicit odbc_parameters(const shared_statement_ptr &stmt);

    std::shared_ptr<statement_interface> stmt;
};

} // namespace cpp_db

#endif // CPP_DB_ODBC_PARAMETERS_H
