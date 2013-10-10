#include "statement.h"
#include "connection.h"
#include "db_exception.h"
#include "null.h"

#include "sqlite3.h"

#include <iostream>
#include <vector>
#include <string>

#include <cstring>

namespace cpp_db
{

extern void throw_db_exception(int error_code, sqlite3 *db);

struct statement::impl
{
    std::shared_ptr<sqlite3_stmt> stmt;
    std::weak_ptr<sqlite3> db;
    const char *tail;

	template<typename ElementType>
	static void delete_array(void *data)
	{
		delete [] reinterpret_cast<ElementType *>(data);
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
		using blob = std::vector<uint8_t>;

		int index = param.has_index() ? param.get_index() : find_param_pos(param.get_name());
		if (param.has_value_of_type<int>())
			sqlite3_bind_int(stmt.get(), index, param.get_value<int>());
		else if (param.has_value_of_type<double>())
			sqlite3_bind_double(stmt.get(), index, param.get_value<double>());
		else if (param.has_value_of_type<const char *>())
		{	
			const char *source = param.get_value<const char *>();
			char * value = new char[strlen(source) + 1];
			strcpy(value, source);
			sqlite3_bind_text(stmt.get(), index, value, strlen(source), delete_array<char *>);
		}
		else if (param.has_value_of_type<std::string>())
		{
			std::string source(param.get_value<std::string>());
			char * value = new char[source.length() + 1];
			strcpy(value, source.c_str());
			sqlite3_bind_text(stmt.get(), index, value, source.length(), delete_array<char *>);
		}
		else if (param.has_value_of_type<blob>())
		{
			blob source(param.get_value<blob>());
			uint8_t *value = new uint8_t[source.size()];
			memcpy(value, source.data(), source.size());
			sqlite3_bind_blob(stmt.get(), index, value, source.size(), delete_array<uint8_t *>);
		}
		else if (param.has_value_of_type<int64_t>())
			sqlite3_bind_int64(stmt.get(), index, param.get_value<int64_t>());
		else if (param.has_value_of_type<tools::null_type>())
			sqlite3_bind_null(stmt.get(), index);
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
