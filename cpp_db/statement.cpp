#include "statement.h"
#include "statement_interface.h"
#include "connection.h"
#include "db_exception.h"
#include "result.h"
#include "driver.h"

namespace cpp_db
{

statement::statement(const connection &conn)
: pdriver(conn.get_driver().get())
, pstatement(pdriver->make_statement(conn.get_handle()))
{
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
	pstatement->prepare(sqlcmd);
}

void statement::execute_non_query()
{
	pstatement->execute();
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
	return pstatement->is_prepared();
}

handle statement::get_handle() const
{
	return pstatement->get_handle();
}

parameters statement::get_parameters() const
{
	if (!is_prepared())
		throw db_exception("Statement not prepared!");

	return parameters(*this);
}

void statement::reset()
{
	pstatement->reset();
}

driver * statement::get_driver() const
{
	return pdriver;
}

}
