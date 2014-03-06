#ifndef CPP_DB_POSTGRES_STATEMENT_H
#define CPP_DB_POSTGRES_STATEMENT_H

#include "statement_interface.h"
#include "usings.h"

#include <libpq-fe.h>

#include <memory>

namespace cpp_db
{

class postgres_driver;

class postgres_statement : public statement_interface
{
public:
    ~postgres_statement();

    void prepare(const std::string &sqlcmd) override;
    bool is_prepared() const override;
    void execute_ddl() override;
    void execute_non_query() override;
    void execute() override;
    void reset() override;
    handle get_handle() const override;

private:
    friend class postgres_driver;
    postgres_statement(const shared_connection_ptr &connection);
    PGconn *get_db_handle() const;

private:    
    std::weak_ptr<connection_interface> conn_impl;
    bool prepared;
};

} // namespace cpp_db

#endif // CPP_DB_POSTGRES_STATEMENT_H
