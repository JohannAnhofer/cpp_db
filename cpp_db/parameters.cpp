#include "parameters.h"
#include "statement.h"
#include "db_exception.h"

#include "null.h"

#include "sqlite3.h"

#include <vector>
#include <cstdint>

namespace cpp_db
{

	extern void throw_db_exception(int error_code, sqlite3 *db);

	using blob = std::vector<uint8_t>;

	struct parameters::impl
	{
		std::weak_ptr<sqlite3_stmt> stmt;

		explicit impl(const statement &stmt)
			: stmt(std::static_pointer_cast<sqlite3_stmt>(stmt.get_handle()))
		{
		}

		int get_count() const
		{
			return sqlite3_bind_parameter_count(stmt.lock().get());
		}
	
		// bind positional parameter
		int bind_pos(int pos, double value)
		{
			return sqlite3_bind_double(stmt.lock().get(), pos, value);
		}
		int bind_pos(int pos, int value)
		{
			return sqlite3_bind_int(stmt.lock().get(), pos, value);
		}
		int bind_pos(int pos, long long value)
		{
			return sqlite3_bind_int64(stmt.lock().get(), pos, value);
		}
		int bind_pos(int pos, const std::string &value)
		{
			return sqlite3_bind_text(stmt.lock().get(), pos, value.c_str(), value.length(), SQLITE_TRANSIENT);
		}
		int bind_pos(int pos, const blob &value)
		{
			return sqlite3_bind_blob(stmt.lock().get(), pos, value.data(), value.size(), SQLITE_TRANSIENT);
		}
		int bind_pos(int pos, const null_type &)
		{
			return sqlite3_bind_null(stmt.lock().get(), pos);
		}

		int find_param_pos(const std::string &name) const
		{
			if (int pos = sqlite3_bind_parameter_index(stmt.lock().get(), name.c_str()))
				return pos;
			else
				throw db_exception("Index for SQL parameter '" + name + "' not found!");
		}

		template<typename T>
		void bind(int pos, T && value)
		{
			if (int error_code = bind_pos(pos, std::forward(value)))
                throw_db_exception(error_code, sqlite3_db_handle(stmt.lock().get()));
		}

		template<typename T>
		void bind(const std::string &name, T && value)
		{
			bind(find_param_pos(name), std::forward(value));
		}
	};

	parameters::parameters(const statement &stmt)
		: pimpl(new impl(stmt))
	{
	}

	parameters::~parameters()
	{
	}

	int parameters::get_count() const
	{
		return pimpl->get_count();
	}

	void parameters::bind(const parameter &param)
	{
		(void) param;
	}
}
