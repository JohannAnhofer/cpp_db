#ifndef CPP_DB_ODBC_STATEMENT_H
#define CPP_DB_ODBC_STATEMENT_H

#include "statement_interface.h"

#include <memory>

namespace cpp_db
{

class odbc_driver;
struct connection_interface;

class odbc_statement : public statement_interface
{
public:
    ~odbc_statement();

    void prepare(const std::string &sqlcmd) override;
    bool is_prepared() const override;
    void execute_ddl() override;
    void execute_non_query() override;
    void execute() override;
    void reset() override;
    handle get_handle() const override;

private:
    friend class odbc_driver;
    odbc_statement(const shared_connection_ptr &connection);

    std::weak_ptr<connection_interface> conn_impl;
};

} // namespace cpp_db

#endif // CPP_DB_ODBC_STATEMENT_H
