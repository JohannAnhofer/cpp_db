#ifndef CPP_DB_POSTGRES_RESULT_H
#define CPP_DB_POSTGRES_RESULT_H

#include "result_interface.h"
#include "usings.h"

#include <memory>

namespace cpp_db
{

class postgres_statement;
class postgres_driver;

class postgres_result : public result_interface
{
public:
    void move_next() override;
    bool is_eof() const override;
    int get_column_count() const override;
    std::string get_column_name(int column) const override;
    value get_column_value(int column) const override;
    int get_column_index(const std::string &column_name) const override;
    value get_column_value(const std::string &column_name) const override;
    bool is_column_null(int column) const override;
    bool is_column_null(const std::string &column_name) const override;
    handle get_handle() const override;

private:
    friend class postgres_driver;
    explicit postgres_result(const shared_statement_ptr &stmt);
    std::shared_ptr<postgres_statement> stmt;
};

} // namespace cpp_db

#endif // CPP_DB_POSTGRES_RESULT_H
