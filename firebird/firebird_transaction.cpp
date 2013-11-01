#include "firebird_transaction.h"
#include "db_exception.h"

namespace cpp_db
{

bool has_error(ISC_STATUS status[20]);
void throw_firebird_exception(ISC_STATUS status[20]);

firebird_transaction::firebird_transaction(const handle &conn_handle)
    : db{std::static_pointer_cast<isc_db_handle>(conn_handle)}
    , transaction{0}
{
    if (db.expired())
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
    return tr;
}

void firebird_transaction::begin()
{
    if (!is_open())
    {
        ISC_STATUS status[20];
        isc_start_transaction(status, &transaction, 1, 0, 0, 0);
        if (has_error(status))
            throw_firebird_exception(status);
        tr.reset(&transaction);
    }
}

void firebird_transaction::commit()
{
    if (is_open())
    {
        ISC_STATUS status[20];
        isc_commit_transaction(status, &transaction);
        if (has_error(status))
            throw_firebird_exception(status);
        tr.reset();
        transaction = 0;
    }
}

void firebird_transaction::rollback()
{
    if (is_open())
    {
        ISC_STATUS status[20];
        isc_rollback_transaction(status, &transaction);
        if (has_error(status))
            throw_firebird_exception(status);
        tr.reset();
        transaction = 0;
    }
}

bool firebird_transaction::is_open() const
{
    return transaction != 0;
}

}
