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
    sql_statement(const std::string &sqlcmd, connection &conn);
    ~sql_statement();

    void executeDDL();
	void executeNonQuery();
	void execute();

private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

}

#endif // CPP_DB_SQL_STATEMENT_H