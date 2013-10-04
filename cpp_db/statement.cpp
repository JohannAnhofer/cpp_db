#include "statement.h"
#include "connection.h"
#include "db_exception.h"

#include "null.h"

#include "sqlite3.h"

#include <vector>
#include <cstdint>

namespace cpp_db
{

extern void throw_db_exception(int error_code, sqlite3 *db);

typedef std::vector<uint8_t> blob;

struct statement::impl
{
    std::shared_ptr<sqlite3_stmt> stmt;
    std::weak_ptr<sqlite3> db;
    const char *tail;

    impl(connection &con)
		: db(std::static_pointer_cast<sqlite3>(con.get_handle()))
        , tail(nullptr)
	{
        if (db.expired())
            throw db_exception("No database connection for statement!");
	}

    ~impl()
    {
        stmt.reset();
    }

	// bind positional parameter
	int bind_pos(int pos, double value)
	{
        return sqlite3_bind_double(stmt.get(), pos, value);
	}
	int bind_pos(int pos, int value)
	{
        return sqlite3_bind_int(stmt.get(), pos, value);
	}
	int bind_pos(int pos, long long value)
	{
        return sqlite3_bind_int64(stmt.get(), pos, value);
	}
	int bind_pos(int pos, const std::string &value)
	{
        return sqlite3_bind_text(stmt.get(), pos, value.c_str(), value.length(), SQLITE_TRANSIENT);
	}
	int bind_pos(int pos, const blob &value)
	{
        return sqlite3_bind_blob(stmt.get(), pos, value.data(), value.size(), SQLITE_TRANSIENT);
	}
	int bind_pos(int pos, const tools::null_type &)
	{
        return sqlite3_bind_null(stmt.get(), pos);
	}

	int find_param_pos(const std::string &name) const
	{
        if (int pos = sqlite3_bind_parameter_index(stmt.get(), name.c_str()))
			return pos;
		else
			throw db_exception("Index for SQL parameter '" + name + "' not found!");
	}

	template<typename T>
	void bind(int pos, T && value)
	{
        if (int error_code = bind_pos(pos, std::forward(value)))
            throw_db_exception(error_code, db.lock().get());
	}

	template<typename T>
	void bind(const std::string &name, T && value)
	{
        bind(find_param_pos(name), std::forward(value));
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

}
