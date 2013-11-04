#include "firebird_statement.h"
#include "isc_status.h"

#include "ibase.h"

namespace cpp_db
{

firebird_statement::firebird_statement(const handle &connection)
    : db(std::static_pointer_cast<isc_db_handle>(connection))
	, stmt(std::make_shared<isc_stmt_handle>())
    , prepared{false}
{
    if (db.expired())
        throw db_exception("No database connection for statement!");
    isc_status status;
    isc_dsql_allocate_statement(static_cast<ISC_STATUS *>(status), db.lock().get(), stmt.get());
    status.throw_db_exception_on_error();
}

firebird_statement::~firebird_statement()
{
	isc_status status;

	if (tr != nullptr)
	{
        isc_commit_transaction(static_cast<ISC_STATUS *>(status), tr.get());
		tr.reset();
		status.dump_on_error();
	}
    isc_dsql_free_statement(static_cast<ISC_STATUS *>(status), stmt.get(), DSQL_drop);
    status.dump_on_error();
}

void firebird_statement::prepare(const std::string &sqlcmd)
{
    isc_status status;
	tr = std::make_shared<isc_tr_handle>();
	isc_start_transaction(static_cast<ISC_STATUS *>(status), tr.get(), 1, db.lock().get(), 0, 0);
	status.throw_db_exception_on_error();

    isc_dsql_prepare(static_cast<ISC_STATUS *>(status), tr.get(), stmt.get(), 0, sqlcmd.c_str(), SQL_DIALECT_CURRENT, 0);
    if (status.has_error())
    {
        isc_status r_status;
        isc_rollback_transaction(static_cast<ISC_STATUS*>(r_status), tr.get());
        r_status.dump_on_error();
        tr.reset();
    }
    status.throw_db_exception_on_error();
    prepared = true;
}

bool firebird_statement::is_prepared() const
{
    return prepared;
}

void firebird_statement::execute()
{
    if (!is_prepared())
        throw db_exception("Statement not prepared!");
    isc_status status;
    isc_dsql_execute(static_cast<ISC_STATUS*>(status), tr.get(), stmt.get(), 0, 0);
    if (status.has_error())
    {
        isc_status r_status;
        isc_rollback_transaction(static_cast<ISC_STATUS*>(r_status), tr.get());
        r_status.dump_on_error();
        tr.reset();
    }
    status.throw_db_exception_on_error();
}

void firebird_statement::reset()
{
}

handle firebird_statement::get_handle() const
{
	return std::static_pointer_cast<void>(stmt);
}

} // namespace cpp_db
