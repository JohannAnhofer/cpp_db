#ifndef CPP_DB_FIREBIRD_STATEMENT_H
#define CPP_DB_FIREBIRD_STATEMENT_H

#include "statement_interface.h"
#include "usings.h"

#include "ibase.h"

#include <memory>

namespace cpp_db
{

class firebird_statement : public statement_interface
{
public:
    firebird_statement(const shared_connection_ptr &connection, shared_transaction_ptr trans_in);
	~firebird_statement();

    void prepare(const std::string &sqlcmd) override;
    bool is_prepared() const override;
    void execute() override;
    void reset() override;
    handle get_handle() const override;

private:
    isc_db_handle *get_db_handle() const;
    isc_tr_handle *get_current_transaction_handle() const;
    isc_tr_handle *get_local_transaction_handle() const;
    bool has_current_transaction() const;
    bool has_local_transaction() const;

private:    
    std::weak_ptr<connection_interface> conn_impl;
    std::shared_ptr<transaction_interface> tr;
    std::shared_ptr<isc_stmt_handle> stmt;
    bool prepared;
};

} // namespace cpp_db

#endif // CPP_DB_FIREBIRD_STATEMENT_H
