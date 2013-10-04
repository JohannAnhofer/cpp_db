#ifndef CPP_DB_STATEMENT_H
#define CPP_DB_STATEMENT_H

#include <memory>
#include <string>

namespace cpp_db
{

class connection;

class statement
{
public:
	typedef std::shared_ptr<void> handle;

    statement(const std::string &sqlcmd, connection &conn);
    explicit statement(connection &conn);
    ~statement();

    statement(const statement&other) = delete;
    statement &operator=(const statement &other) = delete;

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

#endif // CPP_DB_STATEMENT_H
