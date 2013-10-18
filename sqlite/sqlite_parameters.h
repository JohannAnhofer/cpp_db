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
    explicit sqlite_parameters(const handle &stmt);
    sqlite_parameters();

    int get_count() const override;
    void bind(const parameter &param) override;

private:
    int find_param_pos(const std::string &name) const;

    std::weak_ptr<sqlite3_stmt> stmt;
};

} // namespace cpp_db

#endif // CPP_DB_SQLITE_PARAMETERS_H
