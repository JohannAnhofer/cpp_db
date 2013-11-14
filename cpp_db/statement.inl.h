#ifndef CPP_DB_STATEMENT_INL_H
#define CPP_DB_STATEMENT_INL_H

namespace cpp_db
{


template<int pos, typename Arg, typename...Args>
void bind_pos_param(parameters &params, Arg &&arg, Args && ...args)
{
	params.bind(pos, arg);
    bind_pos_param<pos + 1>(params, std::forward<Args>(args)...);
}

template<int>
void bind_pos_param(parameters &)
{
}

template<typename ...Args>
void bind_params(statement & stmt, Args&& ...args)
{
	stmt.reset();
	parameters params(stmt.get_parameters());
    bind_pos_param<1>(params, std::forward<Args>(args)...);
}

template<typename ...Args>
void statement::execute_ddl(Args&& ...args)
{
	bind_params(*this, std::forward<Args>(args)...);
	execute_ddl();
}

template<typename ...Args>
void statement::execute_non_query(Args&& ...args)
{
	bind_params(*this, std::forward<Args>(args)...);
	execute_non_query();
}

template<typename ...Args>
value statement::execute_scalar(Args&& ...args)
{
	bind_params(*this, std::forward<Args>(args)...);
	return execute_scalar();
}

template<typename ...Args>
result statement::execute(Args&& ...args)
{
	bind_params(*this, std::forward<Args>(args)...);
	return execute();
}

}

#endif
