#ifndef CPP_DB_EXECUTE_H
#define CPP_DB_EXECUTE_H

#include "statement.h"
#include "value.h"
#include "result.h"

#include <string>

namespace cpp_db
{
	class connection;

	template<typename ...Args>
    void execute_non_query(connection &conn, const std::string &sql, Args && ...args)
	{
		statement stmt(conn);
		stmt.prepare(sql);
        stmt.execute_non_query(args...);
	}

	template<typename ...Args>
    value execute_scalar(connection &conn, const std::string &sql, Args &&...args)
	{
		statement stmt(conn);
		stmt.prepare(sql);
        return stmt.execute_scalar(args...);
	}

	template<typename ...Args>
    result execute(connection &conn, const std::string &sql, Args && ...args)
	{
		statement stmt(conn);
		stmt.prepare(sql);
        return stmt.execute(args...);
	}
}

#endif
