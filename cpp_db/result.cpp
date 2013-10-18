#include "result.h"
#include "result_interface.h"
#include "statement.h"
#include "driver.h"

#include <stdexcept>

namespace cpp_db
{

result::result(const statement &stmt)
	: result_impl(stmt.get_driver()->make_result(stmt.get_handle()))
{
	if (!result_impl)
		throw std::runtime_error("No result object from driver!");
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

void result::move_first()
{
	result_impl->move_first();
}

void result::move_next()
{
	result_impl->move_next();
}

void result::move_prev()
{
	result_impl->move_prev();
}

value result::get_column_value(int column) const
{	
	return result_impl->get_column_value(column);
}

value result::get_column_value(const std::string &column_name) const
{
	return result_impl->get_column_value(column_name);
}

std::string result::get_column_name(int column) const
{
	return result_impl->get_column_name(column);
}

int result::get_column_index(const std::string &column_name) const
{
	return result_impl->get_column_index(column_name);
}

}
