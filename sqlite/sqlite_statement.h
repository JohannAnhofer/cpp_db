#ifndef CPP_DB_SQLITE_STATEMENT_H
#define CPP_DB_SQLITE_STATEMENT_H

#include "statement_interface.h"

#include "sqlite3.h"

#include <memory>

namespace cpp_db
{

class sqlite_driver;

class sqlite_statement  : public statement_interface
{
public:
    void prepare(const std::string &sqlcmd) override;
    bool is_prepared() const override;
    void execute_ddl() override;
    void execute_non_query() override;
    void execute() override;
    void reset() override;
    handle get_handle() const override;

private:
    friend class sqlite_driver;
    explicit sqlite_statement(const shared_connection_ptr &conn_in);
    sqlite3 *get_db_handle() const;

private:
    std::shared_ptr<sqlite3_stmt> stmt;
    std::weak_ptr<connection_interface> conn;
    const char *tail;
};

} // namespace cpp_db

#endif // CPP_DB_SQLITE_STATEMENT_H
