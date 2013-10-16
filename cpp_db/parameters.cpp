#include "parameters.h"
#include "statement.h"
#include "db_exception.h"

#include "null.h"

#include "sqlite3.h"

#include <vector>
#include <cstdint>
#include <cstring>

namespace cpp_db
{

	extern void throw_db_exception(int error_code, sqlite3 *db);

	using blob = std::vector<uint8_t>;

	struct parameters::impl
	{
		std::weak_ptr<sqlite3_stmt> stmt;

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

		explicit impl(const statement &stmt)
			: stmt(std::static_pointer_cast<sqlite3_stmt>(stmt.get_handle()))
		{
		}

		int get_count() const
		{
			return sqlite3_bind_parameter_count(stmt.lock().get());
		}

		int find_param_pos(const std::string &name) const
		{
			if (int pos = sqlite3_bind_parameter_index(stmt.lock().get(), name.c_str()))
				return pos;
			else
				throw db_exception("Index for SQL parameter '" + name + "' not found!");
		}

		void bind(const parameter &param)
		{
			int index = param.has_index() ? param.get_index() : find_param_pos(param.get_name());
			if (param.has_value_of_type<int>())
				sqlite3_bind_int(stmt.lock().get(), index, param.get_value<int>());
			else if (param.has_value_of_type<double>())
				sqlite3_bind_double(stmt.lock().get(), index, param.get_value<double>());
			else if (param.has_value_of_type<const char *>())
			{
				const char *source = param.get_value<const char *>();
				char * value = new char[strlen(source) + 1];
				memcpy(value, source, strlen(source) + 1);
				sqlite3_bind_text(stmt.lock().get(), index, value, strlen(source), delete_array<char>);
			}
			else if (param.has_value_of_type<std::string>())
			{
				std::string source(param.get_value<std::string>());
				char * value = new char[source.length() + 1];
				memcpy(value, source.c_str(), source.length() + 1);
				sqlite3_bind_text(stmt.lock().get(), index, value, source.length(), delete_array<char>);
			}
			else if (param.has_value_of_type<blob>())
			{
				blob source(param.get_value<blob>());
				uint8_t *value = new uint8_t[source.size()];
				memcpy(value, source.data(), source.size());
				sqlite3_bind_blob(stmt.lock().get(), index, value, source.size(), delete_array<uint8_t>);
			}
			else if (param.has_value_of_type<int64_t>())
				sqlite3_bind_int64(stmt.lock().get(), index, param.get_value<int64_t>());
			else if (param.has_value_of_type<null_type>())
				sqlite3_bind_null(stmt.lock().get(), index);
		}

	};

	parameters::parameters(const statement &stmt)
		: pimpl(new impl(stmt))
	{
	}

	parameters::parameters(parameters && other)
		: pimpl(std::move(other.pimpl))
	{
	}

	parameters &parameters::operator=(parameters &&other)
	{
		if (this != &other)
			pimpl = std::move(other.pimpl);
		return *this;
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
		pimpl->bind(param);
	}
}
