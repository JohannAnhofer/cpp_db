#ifndef CPP_DB_STATEMENT_INL_H
#define CPP_DB_STATEMENT_INL_H

namespace cpp_db
{


template<int pos, typename Arg, typename...Args>
void bind_pos_param(parameters &params, Arg &&arg, Args && ...args)
{
	params.bind(pos, arg);
	bind_pos_param<pos + 1>(params, args...);
}

template<int>
void bind_pos_param(parameters &)
{
}

template<typename ResultType, typename FunctionType, typename ...Args>
ResultType execute_with_params(statement & stmt, FunctionType function, Args&& ...args)
{
	stmt.reset();
	parameters params(stmt.get_parameters());
	bind_pos_param<1>(params, args...);
	return function();
}

template<typename ...Args>
void statement::execute_ddl(Args&& ...args)
{
    execute_with_params<void>(*this, [&]() {execute_ddl(); }, args...);
}

template<typename ...Args>
void statement::execute_non_query(Args&& ...args)
{
	execute_with_params<void>(*this, [&]() {execute_non_query(); }, args...);
}

template<typename ...Args>
value statement::execute_scalar(Args&& ...args)
{
	return execute_with_params<value>(*this, [&](){return execute_scalar(); }, args...);
}

template<typename ...Args>
result statement::execute(Args&& ...args)
{
	return execute_with_params<result>(*this, [&](){return execute(); }, args...);
}

}

#endif
