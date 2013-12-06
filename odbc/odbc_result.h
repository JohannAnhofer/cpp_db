#ifndef CPP_DB_ODBC_RESULT_H
#define CPP_DB_ODBC_RESULT_H

#include "result_interface.h"
#include "usings.h"

#include <memory>

namespace cpp_db
{

class odbc_driver;
class odbc_statement;

class odbc_result : public result_interface
{
public:
    ~odbc_result();

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
    friend class odbc_driver;
    explicit odbc_result(const shared_statement_ptr &stmt);

    std::shared_ptr<odbc_statement> stmt;
};

} // namespace cpp_db

#endif // CPP_DB_ODBC_RESULT_H
