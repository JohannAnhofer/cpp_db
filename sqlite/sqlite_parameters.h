#ifndef CPP_DB_SQLITE_PARAMETERS_H
#define CPP_DB_SQLITE_PARAMETERS_H

#include "usings.h"
#include "parameters_interface.h"

#include "sqlite3.h"

#include <memory>
#include <string>

namespace cpp_db
{

class sqlite_parameters : public parameters_interface
{
public:
    explicit sqlite_parameters(const statement_handle &stmt);

    int get_count() const override;
    void bind(const parameter &param) override;

private:
    int find_param_pos(const std::string &name) const;
	sqlite3_stmt *get_stmt_handle() const;

    std::shared_ptr<statement_interface> stmt;
};

} // namespace cpp_db

#endif // CPP_DB_SQLITE_PARAMETERS_H
