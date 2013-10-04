#include "record.h"
#include "sql_statement.h"
#include "db_exception.h"

#include "sqlite3.h"

namespace cpp_db
{

extern void throw_db_exception(int error_code, sqlite3 *db);

struct record::impl
{
	std::shared_ptr<sqlite3_stmt> stmt;
	int row_status;

	impl(const sql_statement &sqlstmt)
		: stmt(std::static_pointer_cast<sqlite3_stmt>(sqlstmt.get_handle()))
		, row_status(SQLITE_DONE)
	{
    }

	void next()
	{
        row_status = sqlite3_step(stmt.get());
        if (row_status != SQLITE_DONE && row_status != SQLITE_ROW)
            throw_db_exception(row_status, sqlite3_db_handle(stmt.get()));
	}

	void prev()
	{
		throw db_exception("Not supported!");
	}

	void first()
	{
		if (int error_code = sqlite3_reset(stmt.get()))
			throw_db_exception(error_code, sqlite3_db_handle(stmt.get()));
		next();
	}

	void last()
	{
	}

	int get_column_count() const
	{
		return sqlite3_column_count(stmt.get());
	}
};

record::record(const sql_statement &stmt)
	: pimpl(new impl(stmt))
{
    if (!stmt.is_prepared())
        throw db_exception("Statement not prepared!");

    pimpl->next();
}

record::~record()
{
}

int record::get_column_count() const
{
	return pimpl->get_column_count();
}

bool record::is_eof() const
{
	return pimpl->row_status == SQLITE_DONE;
}

void record::move_first()
{
	pimpl->first();
}

void record::move_next()
{
	pimpl->next();
}

void record::move_prev()
{
	pimpl->prev();
}

std::string record::get_field_value(int field) const
{	
	return reinterpret_cast<const char *>(sqlite3_column_text(pimpl->stmt.get(), field));
}

std::string record::get_field_value(const std::string &field) const
{
    (void)field;
	return std::string();
}

std::string record::get_field_name(int field) const
{
	if (const char *field_name = sqlite3_column_name(pimpl->stmt.get(), field))
		return field_name;
	else
		throw db_exception(std::string("Field not found!"));
}

}
