#ifndef CPP_DB_SQL_STATEMENT_H
#define CPP_DB_SQL_STATEMENT_H

#include <memory>
#include <string>

namespace cpp_db
{

class connection;

class sql_statement
{
public:
	typedef std::shared_ptr<void> handle;

    sql_statement(const std::string &sqlcmd, connection &conn);
    explicit sql_statement(connection &conn);
    ~sql_statement();

	sql_statement(const sql_statement&other) = delete;
	sql_statement &operator=(const sql_statement &other) = delete;

    void prepare(const std::string &sqlcmd);

    void execute_ddl();
    void execute_non_query();

    bool is_prepared() const;
	handle get_handle() const;

private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

}

#endif // CPP_DB_SQL_STATEMENT_H
