#include "result.h"
#include "statement.h"
#include "driver.h"

namespace cpp_db
{

result::result(const statement &stmt)
	: presult(stmt.get_driver()->make_result(stmt.get_handle()))
{
}

result::result(result &&other)
: presult(std::move(other.presult))
{
}

result::~result()
{
}

result &result::operator=(result &&other)
{
    if (this != &other)
		presult = std::move(other.presult);
    return *this;
}

int result::get_column_count() const
{
	return presult->get_column_count();
}

bool result::is_eof() const
{
	return presult->is_eof();
}

void result::move_first()
{
	presult->move_first();
}

void result::move_next()
{
	presult->move_next();
}

void result::move_prev()
{
	presult->move_prev();
}

value result::get_column_value(int column) const
{	
	return presult->get_column_value(column);
}

value result::get_column_value(const std::string &column_name) const
{
	return presult->get_column_value(column_name);
}

std::string result::get_column_name(int column) const
{
	return presult->get_column_name(column);
}

int result::get_column_index(const std::string &column_name) const
{
	return presult->get_column_index(column_name);
}

}
