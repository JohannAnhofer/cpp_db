#ifndef CPP_DB_POSTGRES_STATEMENT_H
#define CPP_DB_POSTGRES_STATEMENT_H

#include "statement_interface.h"
#include "usings.h"

#include <libpq-fe.h>

#include <memory>
#include <string>

namespace cpp_db
{

class postgres_driver;

class postgres_statement : public statement_interface
{
public:
    ~postgres_statement() override;

    void prepare(const std::string &sqlcmd) override;
    bool is_prepared() const override;
    void execute_ddl() override;
    void execute_non_query() override;
    void execute() override;
    void reset() override;
    handle get_handle() const override;

private:
    friend class postgres_driver;
    friend class postgres_result;
    postgres_statement(const shared_connection_ptr &connection);
    PGconn *get_db_handle() const;

private:    
    std::weak_ptr<connection_interface> conn_impl;
    std::shared_ptr<PGresult> stmt;
    bool prepared;
    std::string sql;
};

} // namespace cpp_db

#endif // CPP_DB_POSTGRES_STATEMENT_H
