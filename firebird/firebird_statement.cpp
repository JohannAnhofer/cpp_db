#include "firebird_statement.h"

#include "db_exception.h"

#include "ibase.h"

namespace cpp_db
{

extern void throw_firebird_exception(ISC_STATUS status[20]);

firebird_statement::firebird_statement(const handle &connection)
    : db(std::static_pointer_cast<isc_db_handle>(connection))
{
    if (db.expired())
        throw db_exception("No database connection for statement!");
}

void firebird_statement::prepare(const std::string &sqlcmd)
{
}

bool firebird_statement::is_prepared() const
{
    return false;
}

void firebird_statement::execute()
{
    if (!is_prepared())
        throw db_exception("Statement not prepared!");
}

void firebird_statement::reset()
{
}

handle firebird_statement::get_handle() const
{
    return handle{};
}

} // namespace cpp_db
