#include "statement.h"
#include "connection.h"
#include "db_exception.h"
#include "value_is_null.h"
#include "null.h"

#include "sqlite3.h"

#include <iostream>
#include <vector>
#include <string>

#include <cstring>

namespace cpp_db
{

extern void throw_db_exception(int error_code, sqlite3 *db);
using blob = std::vector<uint8_t>;

struct statement::impl
{
	std::shared_ptr<sqlite3_stmt> stmt;
    std::weak_ptr<sqlite3> db;
    const char *tail;

	template<typename ElementType>
	static void delete_array(void *array)
	{
		delete [] reinterpret_cast<ElementType *>(array);
	}

	template<typename ObjectType>
	static void delete_object(void *object)
	{
		delete reinterpret_cast<ObjectType *>(object);
	}

    impl(connection &con)
		: db(std::static_pointer_cast<sqlite3>(con.get_handle()))
        , tail(nullptr)
	{
        if (db.expired())
            throw db_exception("No database connection for statement!");
	}

    void prepare(const std::string &sqlcmd)
    {
        sqlite3_stmt *stmt_new = nullptr;
        const char *tail_new = nullptr;

        int error_code = sqlite3_prepare_v2(db.lock().get(), sqlcmd.c_str(), sqlcmd.size(), &stmt_new, &tail_new);
        if (error_code != SQLITE_OK)
            throw_db_exception(error_code, db.lock().get());

        stmt.reset(stmt_new, sqlite3_finalize);
        tail = tail_new;
    }

    bool is_prepared() const
    {
        return stmt != nullptr;
    }

    void execute()
    {
		if (!is_prepared())
			throw db_exception("Statement not prepared!");

		if (int error_code = sqlite3_step(stmt.get()))
        {
            if (error_code != SQLITE_DONE && error_code != SQLITE_ROW)
                throw_db_exception(error_code, db.lock().get());
        }
    }


	int find_param_pos(const std::string &name) const
	{
		if (int pos = sqlite3_bind_parameter_index(stmt.get(), name.c_str()))
			return pos;
		else
			throw db_exception("Index for SQL parameter '" + name + "' not found!");
	}

    void bind(const parameter &param)
    {
		int index = param.has_index() ? param.get_index() : find_param_pos(param.get_name());
		if (param.has_value_of_type<int>())
			sqlite3_bind_int(stmt.get(), index, param.get_value<int>());
		else if (param.has_value_of_type<double>())
			sqlite3_bind_double(stmt.get(), index, param.get_value<double>());
		else if (param.has_value_of_type<const char *>())
		{	
			const char *source = param.get_value<const char *>();
			char * value = new char[strlen(source) + 1];
			memcpy(value, source, strlen(source) + 1);
			sqlite3_bind_text(stmt.get(), index, value, strlen(source), delete_array<char>);
		}
		else if (param.has_value_of_type<std::string>())
		{
			std::string source(param.get_value<std::string>());
			char * value = new char[source.length() + 1];
			memcpy(value, source.c_str(), source.length() + 1);
			sqlite3_bind_text(stmt.get(), index, value, source.length(), delete_array<char>);
		}
		else if (param.has_value_of_type<blob>())
		{
			blob source(param.get_value<blob>());
			uint8_t *value = new uint8_t[source.size()];
			memcpy(value, source.data(), source.size());
			sqlite3_bind_blob(stmt.get(), index, value, source.size(), delete_array<uint8_t>);
		}
		else if (param.has_value_of_type<int64_t>())
			sqlite3_bind_int64(stmt.get(), index, param.get_value<int64_t>());
		else if (param.has_value_of_type<null_type>())
			sqlite3_bind_null(stmt.get(), index);
	}

	value get_value_from_column(int column)
	{
		sqlite3_stmt *pstmt = stmt.get();

		switch (sqlite3_column_type(pstmt, column))
		{
		case SQLITE_INTEGER:	// int64
			return sqlite3_column_int64(pstmt, column);
		case SQLITE_FLOAT:		// double
			return sqlite3_column_double(pstmt, column);
		case SQLITE_BLOB:		// void *
			{
				const uint8_t *data = static_cast<const uint8_t*>(sqlite3_column_blob(pstmt, column));
				return blob(data, data + sqlite3_column_bytes(pstmt, column));
			}
		case SQLITE_TEXT:		// const char *
			return std::string(reinterpret_cast<const char *>(sqlite3_column_text(pstmt, column)), sqlite3_column_bytes(pstmt, column));
		case SQLITE_NULL:		// 
		default:
			break;
		}
		return null_type();
	}

	value fetch_first_column_off_first_row()
	{
		if (!is_prepared())
			throw db_exception("Statement not prepared!");

		int row_status = sqlite3_step(stmt.get());
		if (row_status != SQLITE_ROW && row_status != SQLITE_DONE)
			throw_db_exception(row_status, sqlite3_db_handle(stmt.get()));
		return get_value_from_column(0);
	}

	void reset()
	{
		sqlite3_reset(stmt.get());
	}
};

statement::statement(connection &conn)
    : pimpl(new impl(conn))
{
}

statement::statement(const std::string &sqlcmd, connection &conn)
    : statement(conn)
{
    prepare(sqlcmd);
}

statement::~statement()
{
}

void statement::prepare(const std::string &sqlcmd)
{
    pimpl->prepare(sqlcmd);
}

void statement::execute_ddl()
{
	pimpl->execute();
}

void statement::execute_non_query()
{
	pimpl->execute();
}

value statement::execute_scalar()
{
	return pimpl->fetch_first_column_off_first_row();
}

bool statement::is_prepared() const
{
    return pimpl->is_prepared();
}

statement::handle statement::get_handle() const
{
	return std::static_pointer_cast<void>(pimpl->stmt);
}

void statement::bind_param(const parameter &param)
{
    pimpl->bind(param);
}

void statement::reset()
{
	pimpl->reset();
}

}
