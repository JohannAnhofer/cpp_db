#ifndef CPP_DB_POSTGRES_PARAMETERS_H
#define CPP_DB_POSTGRES_PARAMETERS_H

#include "parameters_interface.h"
#include "usings.h"

namespace cpp_db
{

class postgres_driver;

class postgres_parameters : public parameters_interface
{
public:
    int get_count() const override;
    void bind(const parameter &param) override;
    handle get_handle() const override;

private:
    friend class postgres_driver;
    explicit postgres_parameters(const shared_statement_ptr &stmt);

    std::shared_ptr<statement_interface> stmt;
};


} // namespace cpp_db

#endif // CPP_DB_POSTGRES_PARAMETERS_H
