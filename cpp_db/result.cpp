#include "result.h"
#include "statement.h"
#include "db_exception.h"

#include "sqlite3.h"

#include <sstream>
#include <vector>
#include <unordered_map>

#include <cstdint>

namespace cpp_db
{

extern void throw_db_exception(int error_code, sqlite3 *db);

struct result::impl
{
	std::shared_ptr<sqlite3_stmt> stmt;
	int row_status;
	std::unordered_map<std::string, int> column_names;

    impl(const statement_handle &stmt_handle)
        : stmt(std::static_pointer_cast<sqlite3_stmt>(stmt_handle))
		, row_status(SQLITE_DONE)
	{
		next();
		for (int column = 0; column < get_column_count(); ++column)
			column_names.emplace(sqlite3_column_name(stmt.get(), column), column);
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

	bool is_eof() const
	{
		return row_status == SQLITE_DONE;
	}

	int get_column_count() const
	{
		return sqlite3_column_count(stmt.get());
	}

	std::string get_column_name(int column) const
	{
		if (const char *column_name = sqlite3_column_name(stmt.get(), column))
			return column_name;
		else
		{
			std::stringstream message;
			message << "Column " << column << " not found!";
			throw db_exception(message.str());
		}
	}

	value get_column_value(int column) const
	{
		sqlite3_stmt *pstmt = stmt.get();
		using blob = std::vector<uint8_t>;

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

	int get_column_index(const std::string &column_name) const
	{
		auto pos = column_names.find(column_name);
		if (pos != column_names.end())
			return pos->second;
		std::stringstream message;
		message << "Column '" << column_name << "' not found!";
		throw db_exception(message.str());
	}

};

result::result(const statement_handle &stmt_handle)
    : pimpl(new impl(stmt_handle))
{
}

result::result(result &&other)
    : pimpl(std::move(other.pimpl))
{
}

result::~result()
{
}

result &result::operator=(result &&other)
{
    if (this != &other)
        pimpl = std::move(other.pimpl);
    return *this;
}

int result::get_column_count() const
{
	return pimpl->get_column_count();
}

bool result::is_eof() const
{
	return pimpl->is_eof();
}

void result::move_first()
{
	pimpl->first();
}

void result::move_next()
{
	pimpl->next();
}

void result::move_prev()
{
	pimpl->prev();
}

value result::get_column_value(int column) const
{	
	return pimpl->get_column_value(column);
}

value result::get_column_value(const std::string &column_name) const
{
	return pimpl->get_column_value(get_column_index(column_name));
}

std::string result::get_column_name(int column) const
{
	return pimpl->get_column_name(column);
}

int result::get_column_index(const std::string &column_name) const
{
	return pimpl->get_column_index(column_name);
}

}
