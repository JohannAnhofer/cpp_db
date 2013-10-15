#include "transaction.h"
#include "connection.h"
#include "db_exception.h"

#include "sqlite3.h"

#include <cstring>

namespace cpp_db
{

extern void throw_db_exception(int error_code, sqlite3 *db);

struct transaction::impl
{
	std::weak_ptr<sqlite3> db;
    int open_count;

	impl(const connection &con)
		: db(std::static_pointer_cast<sqlite3>(con.get_handle()))
        , open_count(0)
	{
		if (db.expired())
			throw db_exception("No database connection!");
	}

    ~impl()
    {
        try
        {
            while(is_open())
                rollback();
        }
        catch(...)
        {
        }
    }

    void execute(const char *sql)
    {
        const char *tail = nullptr;
        sqlite3_stmt *stmt = 0;
        int status = sqlite3_prepare(db.lock().get(), sql, strlen(sql), &stmt, &tail);
		if (status != SQLITE_OK && status != SQLITE_DONE)
            throw_db_exception(status, db.lock().get());
        auto deleter = [](sqlite3_stmt*s){sqlite3_finalize(s);};
        std::unique_ptr<sqlite3_stmt, decltype(deleter)> pstmt(stmt, deleter);
        status = sqlite3_step(stmt);
		if (status != SQLITE_OK && status != SQLITE_DONE)
			throw_db_exception(status, db.lock().get());
    }

	void begin()
	{
        execute("BEGIN TRANSACTION");
        ++open_count;
	}
	void commit()
	{
        if (is_open())
        {
            execute("COMMIT TRANSACTION");
            --open_count;
        }
    }
    void rollback()
	{
        if (is_open())
        {
            execute("ROLLBACK TRANSACTION");
            --open_count;
        }
    }

	bool is_open() const
	{
        return open_count > 0;
	}
};

transaction::transaction(const connection &conn)
	: pimpl(new impl(conn))
{
}

transaction::~transaction()
{
}

void transaction::begin()
{
	pimpl->begin();
}

void transaction::commit()
{
	pimpl->commit();
}

void transaction::rollback()
{
	pimpl->rollback();
}

bool transaction::is_open() const
{
	return pimpl->is_open();
}

}
