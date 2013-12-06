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

struct driver_interface;

class parameter;
class connection;
class parameters;

struct statement_interface;

class statement
{
public:
    statement(const std::string &sqlcmd, connection &conn);
    explicit statement(const connection &conn);
    ~statement();

    statement(const statement & other) = delete;
    statement &operator=(const statement & other) = delete;

#if !defined(_MSC_VER) || (_MSC_FULL_VER > 180021005)
    statement(statement && other) = default;
    statement &operator=(statement && other) = default;
#else
    statement(statement && other);
    statement &operator=(statement &&other);
#endif

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
    std::weak_ptr<driver_interface> driver_impl;
    std::shared_ptr<statement_interface> stmt_impl;
};

}

#include "statement.inl"

#endif // CPP_DB_STATEMENT_H
