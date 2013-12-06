#ifndef CPP_DB_FIREBIRD_RESULT_H
#define CPP_DB_FIREBIRD_RESULT_H

#include "result_interface.h"
#include "usings.h"
#include "xsqlda.h"

#include "ibase.h"

#include <memory>

namespace cpp_db
{

class firebird_statement;
class firebird_driver;

class firebird_result : public result_interface
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
    isc_stmt_handle *get_statement_handle() const;

private:
    friend class firebird_driver;
    explicit firebird_result(const shared_statement_ptr &stmt);
    std::shared_ptr<firebird_statement> stmt;
    xsqlda *sqlda_fields;
    bool after_last_row;
};

} // namespace cpp_db

#endif // CPP_DB_FIREBIRD_RESULT_H
