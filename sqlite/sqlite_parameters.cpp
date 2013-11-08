#include "sqlite_parameters.h"
#include "sqlite_statement.h"
#include "parameter.h"
#include "db_exception.h"

#include <cstring>

namespace cpp_db
{

template<typename ElementType>
static void delete_array(void *array)
{
    delete[] reinterpret_cast<ElementType *>(array);
}

template<typename ObjectType>
static void delete_object(void *object)
{
    delete reinterpret_cast<ObjectType *>(object);
}

sqlite_parameters::sqlite_parameters(const shared_statement_ptr &stmt_in)
	: stmt(stmt_in)
{
}

sqlite3_stmt *sqlite_parameters::get_stmt_handle() const
{
	return std::static_pointer_cast<sqlite3_stmt>(stmt->get_handle()).get();
}

int sqlite_parameters::get_count() const
{
	return sqlite3_bind_parameter_count(get_stmt_handle());
}

int sqlite_parameters::find_param_pos(const std::string &name) const
{
	if (int pos = sqlite3_bind_parameter_index(get_stmt_handle(), name.c_str()))
        return pos;
    else
        throw db_exception("Index for SQL parameter '" + name + "' not found!");
}

void sqlite_parameters::bind(const parameter &param)
{
    int index = param.has_index() ? param.get_index() : find_param_pos(param.get_name());
    if (param.has_value_of_type<int>())
		sqlite3_bind_int(get_stmt_handle(), index, param.get_value<int>());
    else if (param.has_value_of_type<double>())
		sqlite3_bind_double(get_stmt_handle(), index, param.get_value<double>());
    else if (param.has_value_of_type<const char *>())
    {
        const char *source = param.get_value<const char *>();
        char * value = new char[strlen(source) + 1];
        memcpy(value, source, strlen(source) + 1);
		sqlite3_bind_text(get_stmt_handle(), index, value, strlen(source), delete_array<char>);
    }
    else if (param.has_value_of_type<std::string>())
    {
        std::string source(param.get_value<std::string>());
        char * value = new char[source.length() + 1];
        memcpy(value, source.c_str(), source.length() + 1);
		sqlite3_bind_text(get_stmt_handle(), index, value, source.length(), delete_array<char>);
    }
    else if (param.has_value_of_type<blob>())
    {
        blob source(param.get_value<blob>());
        uint8_t *value = new uint8_t[source.size()];
        memcpy(value, source.data(), source.size());
		sqlite3_bind_blob(get_stmt_handle(), index, value, source.size(), delete_array<uint8_t>);
    }
    else if (param.has_value_of_type<int64_t>())
		sqlite3_bind_int64(get_stmt_handle(), index, param.get_value<int64_t>());
    else if (is_null(param))
		sqlite3_bind_null(get_stmt_handle(), index);
}

} // namespace cpp_db
