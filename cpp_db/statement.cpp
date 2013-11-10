#include "statement.h"
#include "statement_interface.h"
#include "connection.h"
#include "db_exception.h"
#include "result_interface.h"
#include "parameters_interface.h"
#include "driver.h"

namespace cpp_db
{

statement::statement(const connection &conn)
    : driver_impl(conn.driver_impl)
    , stmt_impl(conn.driver_impl->make_statement(conn.conn_impl))
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

void statement::execute_ddl()
{
    stmt_impl->execute_ddl();
}

void statement::execute_non_query()
{
    stmt_impl->execute_non_query();
}

value statement::execute_scalar()
{
    return execute().get_column_value(0);
}

result statement::execute()
{
    result r;
    r.result_impl.reset(driver_impl.lock()->make_result(stmt_impl));
    return r;
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

    parameters params;
    params.params_impl.reset(driver_impl.lock()->make_parameters(stmt_impl));
    return params;
}

void statement::reset()
{
	stmt_impl->reset();
}

}
