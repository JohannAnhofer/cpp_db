#ifndef CPP_DB_ODBC_STATEMENT_H
#define CPP_DB_ODBC_STATEMENT_H

#include <map>
#include <memory>

#if defined(WIN32)
#include <Windows.h>
#endif

#include <sql.h>
#include "statement_interface.h"
#include "value.h"

namespace cpp_db
{

class odbc_driver;
struct connection_interface;

struct odbc_stmt
{
	SQLHANDLE _hstmt;
	std::map<int, value> _params_in;

	odbc_stmt();
	~odbc_stmt();

	void close();
};

class odbc_statement : public statement_interface
{
public:
    ~odbc_statement() override;

    void prepare(const std::string &sqlcmd) override;
    bool is_prepared() const override;
    void execute_ddl() override;
    void execute_non_query() override;
    void execute() override;
    void reset() override;
    handle get_handle() const override;

private:
	std::shared_ptr<odbc_stmt> _stmt;
	bool _prepared;
	std::weak_ptr<connection_interface> conn_impl;

	friend class odbc_driver;
    odbc_statement(const shared_connection_ptr &connection);
};

} // namespace cpp_db

#endif // CPP_DB_ODBC_STATEMENT_H
