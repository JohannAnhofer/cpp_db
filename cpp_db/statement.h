#ifndef CPP_DB_STATEMENT_H
#define CPP_DB_STATEMENT_H

#include "parameter.h"
#include "value.h"

#include <memory>
#include <string>

namespace cpp_db
{

class connection;

class statement
{
public:
    using handle = std::shared_ptr<void>;

    statement(const std::string &sqlcmd, connection &conn);
    explicit statement(connection &conn);
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

    template<typename ...Args>
    void execute_ddl(Args&& ...args)
    {
        execute_with_params<void>([&](){execute_ddl();}, args...);
    }

    template<typename ...Args>
    void execute_non_query(Args&& ...args)
    {
		execute_with_params<void>([&]() {execute_non_query();}, args...);
    }

    template<typename ...Args>
    value execute_scalar(Args&& ...args)
    {
        return execute_with_params<value>([&](){return execute_scalar();}, args...);
    }

    void bind_param(const parameter &param);

private:
    template<int pos, typename Arg, typename...Args>
    void bind_pos_param(Arg &&arg, Args && ...args)
	{
		bind_param(parameter(pos, arg));
        bind_pos_param<pos+1>(args...);
	}

    template<int>
    void bind_pos_param()
	{
	}

	template<typename ResultType, typename FunctionType, typename ...Args>
    ResultType execute_with_params(FunctionType function, Args&& ...args)
    {
        reset();
        bind_pos_param<1>(args...);
        return function();
    }

    struct impl;
    std::unique_ptr<impl> pimpl;
};

}

#endif // CPP_DB_STATEMENT_H
