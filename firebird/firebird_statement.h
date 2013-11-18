#ifndef CPP_DB_FIREBIRD_STATEMENT_H
#define CPP_DB_FIREBIRD_STATEMENT_H

#include "statement_interface.h"
#include "usings.h"
#include "xsqlda.h"

#include "ibase.h"

#include <memory>

namespace cpp_db
{

enum class firebird_statement_type;
class firebird_driver;

class firebird_statement : public statement_interface
{
public:
	~firebird_statement();

    void prepare(const std::string &sqlcmd) override;
    bool is_prepared() const override;
    void execute_ddl() override;
    void execute_non_query() override;
    void execute() override;
    void reset() override;
    handle get_handle() const override;

    xsqlda *access_sqlda_in();
    xsqlda *access_sqlda_out();

    bool is_select_statement() const;

private:
    friend class firebird_driver;
    firebird_statement(const shared_connection_ptr &connection, shared_transaction_ptr trans_in);
    isc_db_handle *get_db_handle() const;
    isc_tr_handle *get_current_transaction_handle() const;
    isc_tr_handle *get_local_transaction_handle() const;
    bool has_current_transaction() const;
    bool has_local_transaction() const;
    firebird_statement_type determine_statement_type() const;

private:    
    std::weak_ptr<connection_interface> conn_impl;
    std::shared_ptr<transaction_interface> tr;
    std::shared_ptr<isc_stmt_handle> stmt;
    bool prepared;
    xsqlda sqlda_params_in, sqlda_fields_out;
    firebird_statement_type statement_type;
};

} // namespace cpp_db

#endif // CPP_DB_FIREBIRD_STATEMENT_H
