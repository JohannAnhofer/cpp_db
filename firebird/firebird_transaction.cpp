#include "firebird_transaction.h"
#include "connection_interface.h"
#include "isc_status.h"
#include "lock_or_throw.h"

namespace cpp_db
{

firebird_transaction::firebird_transaction(const shared_connection_ptr &conn_handle)
    : conn_impl{conn_handle}
{
    if (conn_impl.expired())
        throw db_exception("No database connection!");
}

firebird_transaction::~firebird_transaction()
{
    try
    {
        rollback();
    }
    catch (...)
    {
    }
}

handle firebird_transaction::get_handle() const
{
    return std::static_pointer_cast<void>(tr);
}

isc_db_handle *firebird_transaction::get_db_handle() const
{
	return std::static_pointer_cast<isc_db_handle>(tools::lock_or_throw(conn_impl, "Invalid database connection")->get_handle()).get();
}

void firebird_transaction::begin()
{
    if (!is_open())
    {
        guarded_execute([this](ISC_STATUS *status)
            {
                std::unique_ptr<isc_tr_handle> transaction(new isc_tr_handle{0});
                isc_start_transaction(status, transaction.get(), 1, get_db_handle(), 0, 0);
                if (!isc_status::has_error(status))
                    tr.reset(transaction.release());
            }, true);
    }
}

void firebird_transaction::commit()
{
    if (is_open())
    {
        guarded_execute([this](ISC_STATUS *status)
            {
                isc_commit_transaction(status, tr.get());
                tr.reset();
            }, true);
    }
}

void firebird_transaction::rollback()
{
    if (is_open())
    {
        guarded_execute([this](ISC_STATUS *status)
            {
                isc_rollback_transaction(status, tr.get());
                tr.reset();
            }, true);
    }
}

bool firebird_transaction::is_open() const
{
	return tr != nullptr;
}

}
