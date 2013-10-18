#include "statement.h"
#include "statement_interface.h"
#include "connection.h"
#include "db_exception.h"
#include "result.h"
#include "driver.h"

namespace cpp_db
{

statement::statement(const connection &conn)
	: driver_impl(conn.get_driver())	// a connection without a valid driver, cannot be constructed, so no need to check here 
	, stmt_impl(driver_impl.lock()->make_statement(conn.get_handle()))
{
	if (!stmt_impl)
		throw std::runtime_error("No statement object from driver!");
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
	stmt_impl->prepare(sqlcmd);
}

void statement::execute_non_query()
{
	if (!is_prepared())
		throw db_exception("Statement not prepared!");

	stmt_impl->execute();
}

value statement::execute_scalar()
{
    if (!is_prepared())
        throw db_exception("Statement not prepared!");

    return result(*this).get_column_value(0);
}

result statement::execute()
{
    if (!is_prepared())
        throw db_exception("Statement not prepared!");

    return result(*this);
}

bool statement::is_prepared() const
{
	return stmt_impl->is_prepared();
}

handle statement::get_handle() const
{
	return stmt_impl->get_handle();
}

parameters statement::get_parameters() const
{
	if (!is_prepared())
		throw db_exception("Statement not prepared!");

	return parameters(*this);
}

void statement::reset()
{
	stmt_impl->reset();
}

std::shared_ptr<driver> statement::get_driver() const
{
	return driver_impl.lock();
}

}
