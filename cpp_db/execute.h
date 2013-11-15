#ifndef CPP_DB_EXECUTE_H
#define CPP_DB_EXECUTE_H

#include "statement.h"
#include "value.h"
#include "result.h"

#include <string>
#include <initializer_list>

namespace cpp_db
{
	class connection;

    template<typename ...Args>
    void execute_ddl(connection &conn, const std::string &sql, Args && ...args)
    {
		statement(sql, conn).execute_ddl(std::forward<Args>(args)...);
    }

    template<typename ...Args>
    void execute_non_query(connection &conn, const std::string &sql, Args && ...args)
	{
		statement(sql, conn).execute_non_query(std::forward<Args>(args)...);
	}

	template<typename ...Args>
    value execute_scalar(connection &conn, const std::string &sql, Args &&...args)
	{
		return statement(sql, conn).execute_scalar(std::forward<Args>(args)...);
	}

	template<typename ...Args>
    result execute(connection &conn, const std::string &sql, Args && ...args)
	{
		return statement(sql, conn).execute(std::forward<Args>(args)...);
	}

    void execute_ddl(connection &conn, const std::string &sql, std::initializer_list<parameter> params);
    void execute_non_query(connection &conn, const std::string &sql, std::initializer_list<parameter> params);
    value execute_scalar(connection &conn, const std::string &sql, std::initializer_list<parameter> params);
    result execute(connection &conn, const std::string &sql, std::initializer_list<parameter> params);

}

#endif
