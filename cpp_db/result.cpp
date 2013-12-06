#include "result.h"
#include "result_interface.h"
#include "statement.h"
#include "driver_interface.h"

namespace cpp_db
{

result::result()
{
}

result::result(result &&other)
	: result_impl(std::move(other.result_impl))
{
}

result::~result()
{
}

result &result::operator=(result &&other)
{
    if (this != &other)
		result_impl = std::move(other.result_impl);
    return *this;
}

int result::get_column_count() const
{
	return result_impl->get_column_count();
}

bool result::is_eof() const
{
	return result_impl->is_eof();
}

void result::move_next()
{
	result_impl->move_next();
}

value result::get_column_value(int column) const
{	
	return result_impl->get_column_value(column);
}

value result::get_column_value(const std::string &column_name) const
{
	return result_impl->get_column_value(column_name);
}

value result::operator[](int column) const
{
    return get_column_value(column);
}

value result::operator[](const std::string &column) const
{
    return get_column_value(column);
}

std::string result::get_column_name(int column) const
{
	return result_impl->get_column_name(column);
}

int result::get_column_index(const std::string &column_name) const
{
    return result_impl->get_column_index(column_name);
}

bool result::is_column_null(int column) const
{
    return result_impl->is_column_null(column);
}

bool result::is_column_null(const std::string &column_name) const
{
    return result_impl->is_column_null(column_name);
}

handle result::get_handle() const
{
    return result_impl->get_handle();
}

}
