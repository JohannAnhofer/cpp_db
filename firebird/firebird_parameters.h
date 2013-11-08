#ifndef CPP_DB_FIREBIRD_PARAMETERS_H
#define CPP_DB_FIREBIRD_PARAMETERS_H

#include "parameters_interface.h"
#include "usings.h"
#include "xsqlda.h"

namespace cpp_db
{

class firebird_parameters : public parameters_interface
{
public:
    explicit firebird_parameters(const shared_statement_ptr &stmt);

    int get_count() const override;
    void bind(const parameter &param) override;

private:
    int find_param_pos(const std::string &name) const;
    isc_stmt_handle *get_statement_handle() const;

    std::shared_ptr<statement_interface> stmt;
	xsqlda isqlda;
};


} // namespace cpp_db

#endif // CPP_DB_FIREBIRD_PARAMETERS_H
