#ifndef CPP_DB_FIREBIRD_PARAMETERS_H
#define CPP_DB_FIREBIRD_PARAMETERS_H

#include "parameters_interface.h"
#include "usings.h"

#include "ibase.h"

namespace cpp_db
{

class firebird_parameters : public parameters_interface
{
public:
    explicit firebird_parameters(const handle &stmt);

    int get_count() const override;
    void bind(const parameter &param) override;

private:
    int find_param_pos(const std::string &name) const;
    void allocate_sqlda(int vars_count);

    std::weak_ptr<isc_stmt_handle> stmt;
    std::unique_ptr<XSQLDA, void(*)(void *)> isqlda;
};


} // namespace cpp_db

#endif // CPP_DB_FIREBIRD_PARAMETERS_H
