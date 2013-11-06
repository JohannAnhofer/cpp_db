#include "firebird_statement.h"
#include "isc_status.h"
#include "driver.h"

#include "ibase.h"

#include <functional>

namespace cpp_db
{

static void rollback(std::shared_ptr<isc_tr_handle> &tr, bool throw_on_fail = false);
static void commit(std::shared_ptr<isc_tr_handle> &tr, bool throw_on_fail = false);

firebird_statement::firebird_statement(const handle &connection, driver *current_driver)
    : db(std::static_pointer_cast<isc_db_handle>(connection))
	, stmt(std::make_shared<isc_stmt_handle>())
    , driver_impl{current_driver}
    , prepared{false}
{
    if (db.expired())
        throw db_exception("No database connection for statement!");
    guarded_execute([this](ISC_STATUS *status){isc_dsql_allocate_statement(status, db.lock().get(), stmt.get());}, true);
}

firebird_statement::~firebird_statement()
{
    rollback(tr, false);
    guarded_execute([this](ISC_STATUS *status){isc_dsql_free_statement(status, stmt.get(), DSQL_drop);}, false);
}

void firebird_statement::prepare(const std::string &sqlcmd)
{
    if (driver_impl->get_current_transaction() == nullptr)
    {
        std::unique_ptr<isc_tr_handle> transaction(new isc_tr_handle{ 0 });

        guarded_execute([this, &transaction](ISC_STATUS *status){isc_start_transaction(status, transaction.get(), 1, db.lock().get(), 0, 0); }, true);
        tr.reset(transaction.release());
    }

    guarded_execute([this, &sqlcmd](ISC_STATUS *status)
        {
            isc_tr_handle *trans = tr.get();
            if (trans == nullptr)
                trans = std::static_pointer_cast<isc_tr_handle>(driver_impl->get_current_transaction()).get();

            isc_dsql_prepare(status, trans, stmt.get(), 0, sqlcmd.c_str(), SQL_DIALECT_CURRENT, 0);
            if (isc_status::has_error(status))
                rollback(tr, false);
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
            isc_tr_handle *trans = tr.get();
            if (trans == nullptr)
                trans = std::static_pointer_cast<isc_tr_handle>(driver_impl->get_current_transaction()).get();

            isc_dsql_execute(status, trans, stmt.get(), 0, 0);
            if (isc_status::has_error(status))
                rollback(tr, false);
        }, true);
    commit(tr, true);
}

void firebird_statement::reset()
{
}

handle firebird_statement::get_handle() const
{
	return std::static_pointer_cast<void>(stmt);
}

// non-members
static void rollback(std::shared_ptr<isc_tr_handle> &tr, bool throw_on_fail)
{
    if (tr != nullptr)
        guarded_execute([&tr](ISC_STATUS *status)
            {
                isc_rollback_transaction(status, tr.get());
                tr.reset();
            }, throw_on_fail);
}

static void commit(std::shared_ptr<isc_tr_handle> &tr, bool throw_on_fail)
{
    if (tr != nullptr)
        guarded_execute([&tr](ISC_STATUS *status)
            {
                isc_commit_transaction(status, tr.get());
                tr.reset();
            }, throw_on_fail);

}

} // namespace cpp_db
