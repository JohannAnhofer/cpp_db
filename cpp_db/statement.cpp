#include "statement.h"
#include "statement_interface.h"
#include "connection.h"
#include "db_exception.h"
#include "result_interface.h"
#include "parameters_interface.h"
#include "driver_interface.h"
#include "lock_or_throw.h"

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

#if defined(_MSC_VER) && (_MSC_FULL_VER <= 180021005)

statement::statement(statement && other)
    : driver_impl(std::move(other.driver_impl))
    , stmt_impl(std::move(other.stmt_impl))
{
}

statement &statement::operator=(statement &&other)
{
    if (this != &other)
    {
        driver_impl = std::move(other.driver_impl);
        stmt_impl = std::move(other.stmt_impl);
    }
    return *this;
}

#endif

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
	r.result_impl.reset(tools::lock_or_throw(driver_impl, "Invalid database driver")->make_result(stmt_impl));
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
	params.params_impl.reset(tools::lock_or_throw(driver_impl, "Invalid database driver")->make_parameters(stmt_impl));
    return params;
}

void statement::reset()
{
	stmt_impl->reset();
}

static void bind_params(statement &stmt, std::initializer_list<parameter> params)
{
	stmt.reset();
	parameters p(stmt.get_parameters());

	for (auto param : params)
		p.bind(param);
}

void statement::execute_ddl(std::initializer_list<parameter> params)
{
	bind_params(*this, params);
	execute_ddl();
}

void statement::execute_non_query(std::initializer_list<parameter> params)
{
	bind_params(*this, params);
	execute_non_query();
}

value statement::execute_scalar(std::initializer_list<parameter> params)
{
	bind_params(*this, params);
	return execute_scalar();
}

result statement::execute(std::initializer_list<parameter> params)
{
	bind_params(*this, params);
	return execute();
}

}
