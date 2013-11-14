#include "execute.h"

#include <functional>

namespace cpp_db
{

void execute_non_query(connection &conn, const std::string &sql, std::initializer_list<parameter> params)
{
	statement stmt(conn);
	stmt.prepare(sql);
	stmt.execute_non_query(params);
}

void execute_ddl(connection &conn, const std::string &sql, std::initializer_list<parameter> params)
{
	statement stmt(conn);
	stmt.prepare(sql);
	stmt.execute_ddl(params);
}

value execute_scalar(connection &conn, const std::string &sql, std::initializer_list<parameter> params)
{
	statement stmt(conn);
	stmt.prepare(sql);
	return stmt.execute_scalar(params);
}

result execute(connection &conn, const std::string &sql, std::initializer_list<parameter> params)
{
	statement stmt(conn);
	stmt.prepare(sql);
	return stmt.execute(params);
}

}
