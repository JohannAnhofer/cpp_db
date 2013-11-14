#include "execute.h"

#include <functional>

namespace cpp_db
{

template<typename Result>
static Result execute_helper(const std::function<Result(statement &stmt)> &function, connection &conn, const std::string &sql, std::initializer_list<parameter> params)
{
    statement stmt(conn);
    stmt.prepare(sql);
    parameters p(stmt.get_parameters());

    for (auto param: params)
        p.bind(param);

    return function(stmt);
}

void execute_non_query(connection &conn, const std::string &sql, std::initializer_list<parameter> params)
{
    execute_helper<void>([](statement &stmt){stmt.execute_non_query();}, conn, sql, params);
}

void execute_ddl(connection &conn, const std::string &sql, std::initializer_list<parameter> params)
{
    execute_helper<void>([](statement &stmt){stmt.execute_ddl();}, conn, sql, params);
}

value execute_scalar(connection &conn, const std::string &sql, std::initializer_list<parameter> params)
{
    return execute_helper<value>([](statement &stmt){return stmt.execute_scalar();}, conn, sql, params);
}

result execute(connection &conn, const std::string &sql, std::initializer_list<parameter> params)
{
    return execute_helper<result>([](statement &stmt){return stmt.execute();}, conn, sql, params);
}

}
