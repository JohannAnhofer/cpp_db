#include "firebird_statement.h"
#include "isc_status.h"
#include "driver.h"
#include "transaction_interface.h"
#include "connection_interface.h"
#include "driver.h"

#include "ibase.h"

#include <functional>

namespace cpp_db
{

firebird_statement::firebird_statement(const shared_connection_ptr &conn, shared_transaction_ptr trans)
    : conn_impl(conn)
    , tr(trans)
	, stmt(std::make_shared<isc_stmt_handle>())
    , prepared{false}
{
    if (conn_impl.expired())
        throw db_exception("No database connection for statement!");
    guarded_execute([this](ISC_STATUS *status){isc_dsql_allocate_statement(status, get_db_handle(), stmt.get());}, true);
}

firebird_statement::~firebird_statement()
{
    try
    {
        if (tr->is_open())
            tr->rollback();
    }
    catch(...)
    {
    }

    guarded_execute([this](ISC_STATUS *status){isc_dsql_free_statement(status, stmt.get(), DSQL_drop);}, false);
}

isc_db_handle *firebird_statement::get_db_handle() const
{
    return std::static_pointer_cast<isc_db_handle>(conn_impl.lock()->get_handle()).get();
}

isc_tr_handle *firebird_statement::get_current_transaction_handle() const
{
    return std::static_pointer_cast<isc_db_handle>(conn_impl.lock()->get_current_transaction()->get_handle()).get();
}

isc_tr_handle *firebird_statement::get_local_transaction_handle() const
{
    return std::static_pointer_cast<isc_tr_handle>(tr->get_handle()).get();
}

bool firebird_statement::has_current_transaction() const
{
    return conn_impl.lock()->get_current_transaction() != nullptr;
}

bool firebird_statement::has_local_transaction() const
{
    return tr->is_open();
}

void firebird_statement::prepare(const std::string &sqlcmd)
{
    if (!has_current_transaction())
        tr->begin();    // start local transaction

    guarded_execute([this, &sqlcmd](ISC_STATUS *status)
        {
            isc_tr_handle *trans = has_local_transaction() ? get_local_transaction_handle() : get_current_transaction_handle();

            isc_dsql_prepare(status, trans, stmt.get(), 0, sqlcmd.c_str(), SQL_DIALECT_CURRENT, 0);
            if (isc_status::has_error(status) && has_local_transaction())
                tr->rollback();
        }, true);

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

    guarded_execute([this](ISC_STATUS *status)
        {
            isc_tr_handle *trans = has_local_transaction() ? get_local_transaction_handle() : get_current_transaction_handle();

            isc_dsql_execute(status, trans, stmt.get(), 0, 0);

            if (isc_status::has_error(status) && has_local_transaction())
                tr->rollback();
        }, true);

    if (has_local_transaction())
        tr->commit();
}

void firebird_statement::reset()
{
}

handle firebird_statement::get_handle() const
{
	return std::static_pointer_cast<void>(stmt);
}

} // namespace cpp_db
