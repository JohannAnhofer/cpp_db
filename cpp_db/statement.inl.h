#ifndef CPP_DB_STATEMENT_INL_H
#define CPP_DB_STATEMENT_INL_H

namespace cpp_db
{

template<typename ...Args>
void statement::execute_non_query(Args&& ...args)
{
	execute_with_params<void>([&]() {execute_non_query(); }, args...);
}

template<typename ...Args>
value statement::execute_scalar(Args&& ...args)
{
	return execute_with_params<value>([&](){return execute_scalar(); }, args...);
}

template<typename ...Args>
result statement::execute(Args&& ...args)
{
	return execute_with_params<result>([&](){return execute(); }, args...);
}

template<int pos, typename Arg, typename...Args>
void statement::bind_pos_param(Arg &&arg, Args && ...args)
{
	bind_param(parameter(pos, arg));
	bind_pos_param<pos + 1>(args...);
}

template<int>
void statement::bind_pos_param()
{
}

template<typename ResultType, typename FunctionType, typename ...Args>
ResultType statement::execute_with_params(FunctionType function, Args&& ...args)
{
	reset();
	bind_pos_param<1>(args...);
	return function();
}

}

#endif
