#ifndef CPP_DB_SQLITE_RESULT_H
#define CPP_DB_SQLITE_RESULT_H

#include "result_interface.h"
#include "usings.h"

#include "sqlite3.h"

#include <unordered_map>
#include <memory>

namespace cpp_db
{

class sqlite_result : public result_interface
{
public:
    sqlite_result(const handle &stmt_handle);

    void move_next() override;
    void move_prev() override;
    void move_first() override;
    bool is_eof() const override;
    int get_column_count() const override;
    std::string get_column_name(int column) const override;
    value get_column_value(int column) const override;
    int get_column_index(const std::string &column_name) const override;
    value get_column_value(const std::string &column_name) const override;

private:
    std::shared_ptr<sqlite3_stmt> stmt;
    int row_status;
    std::unordered_map<std::string, int> column_names;
};

} // namespace cpp_db

#endif // CPP_DB_SQLITE_RESULT_H
