#ifndef CPP_DB_SQLITE_PARAMETERS_H
#define CPP_DB_SQLITE_PARAMETERS_H

#include "usings.h"
#include "parameters_interface.h"

#include "sqlite3.h"

#include <memory>
#include <string>

namespace cpp_db
{

class sqlite_driver;

class sqlite_parameters : public parameters_interface
{
public:
    int get_count() const override;
    void bind(const parameter &param) override;
    handle get_handle() const override;

private:
    friend class sqlite_driver;
    explicit sqlite_parameters(const shared_statement_ptr &stmt);
    int find_param_pos(const std::string &name) const;
	sqlite3_stmt *get_stmt_handle() const;

    std::shared_ptr<statement_interface> stmt;
};

} // namespace cpp_db

#endif // CPP_DB_SQLITE_PARAMETERS_H
