#ifndef CPP_DB_STATEMENT_H
#define CPP_DB_STATEMENT_H

#include "parameter.h"

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
	bool is_prepared() const;
	handle get_handle() const;

    void execute_non_query();

    template<typename ...Args>
    void execute_non_query(Args&& ...args)
    {
		reset();
		bind_pos_param<1>([&](){execute_non_query(); }, args...);
    }

	void execute_ddl();

	template<typename ...Args>
	void execute_ddl(Args&& ...args)
	{
		reset();
		bind_pos_param<1>([&](){execute_non_ddl(); }, args...);
	}

    void bind_param(const parameter &param);

private:
	typedef void(statement::*member_function)();

	template<int pos, typename Function, typename Arg, typename...Args>
	void bind_pos_param(Function function, Arg &&arg, Args && ...args)
	{
		bind_param(parameter(pos, arg));
		bind_pos_param<pos+1>(function, args...);
	}

	template<int, typename Function>
	void bind_pos_param(Function function)
	{
		function();
	}

	void reset();

    struct impl;
    std::unique_ptr<impl> pimpl;
};

}

#endif // CPP_DB_STATEMENT_H
