#ifndef CPP_DB_STATEMENT_H
#define CPP_DB_STATEMENT_H

#include "value.h"
#include "result.h"
#include "parameters.h"

#include "usings.h"

#include <memory>
#include <string>

namespace cpp_db
{

class parameter;
class connection;
class driver;
class parameters;

struct statement_interface;

class statement
{
public:
    statement(const std::string &sqlcmd, connection &conn);
    explicit statement(const connection &conn);
    ~statement();

    statement(const statement&other) = delete;
    statement &operator=(const statement &other) = delete;

    void prepare(const std::string &sqlcmd);
	bool is_prepared() const;
	handle get_handle() const;

	void reset();

    void execute_ddl();
	void execute_non_query();
	value execute_scalar();
    result execute();

    template<typename ...Args> void execute_ddl(Args&& ...args);
    template<typename ...Args> void execute_non_query(Args&& ...args);
    template<typename ...Args> value execute_scalar(Args&& ...args);
	template<typename ...Args> result execute(Args&& ...args);

	void execute_ddl(std::initializer_list<parameter> params);
	void execute_non_query(std::initializer_list<parameter> params);
	value execute_scalar(std::initializer_list<parameter> params);
	result execute(std::initializer_list<parameter> params);

	parameters get_parameters() const;

private:
    std::weak_ptr<driver> driver_impl;
    std::shared_ptr<statement_interface> stmt_impl;
};

}

#include "statement.inl"

#endif // CPP_DB_STATEMENT_H
