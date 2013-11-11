#include "firebird_statement.h"
#include "isc_status.h"
#include "driver.h"
#include "transaction_interface.h"
#include "connection_interface.h"
#include "driver.h"
#include "lock_or_throw.h"

#include "ibase.h"

#include <functional>
#include <stdexcept>

namespace cpp_db
{

firebird_statement::firebird_statement(const shared_connection_ptr &conn, shared_transaction_ptr trans)
    : conn_impl(conn)
    , tr(trans)
	, stmt(std::make_shared<isc_stmt_handle>())
    , prepared{false}
    , statement_type{-1}
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
    return std::static_pointer_cast<isc_db_handle>(tools::lock_or_throw(conn_impl)->get_handle()).get();
}

isc_tr_handle *firebird_statement::get_current_transaction_handle() const
{
    shared_transaction_ptr curr_trans(tools::lock_or_throw(conn_impl)->get_current_transaction());

    if (curr_trans != nullptr)
        return std::static_pointer_cast<isc_db_handle>(curr_trans->get_handle()).get();
    else
        throw db_exception("Invalid current transaction");
}

isc_tr_handle *firebird_statement::get_local_transaction_handle() const
{
    return std::static_pointer_cast<isc_tr_handle>(tr->get_handle()).get();
}

bool firebird_statement::has_current_transaction() const
{
    return tools::lock_or_throw(conn_impl)->get_current_transaction() != nullptr;
}

bool firebird_statement::has_local_transaction() const
{
    return tr->is_open();
}

void firebird_statement::prepare(const std::string &sqlcmd)
{
    if (prepared)
    {
        prepared = false;
        sqlda_params_in.resize(1);
        sqlda_fields_out.resize(1);
        statement_type = -1;
    }

    if (!has_current_transaction())
        tr->begin();    // start local transaction

    guarded_execute([this, &sqlcmd](ISC_STATUS *status)
        {
            isc_tr_handle *trans = has_local_transaction() ? get_local_transaction_handle() : get_current_transaction_handle();

            isc_dsql_prepare(status, trans, stmt.get(), 0, sqlcmd.c_str(), SQL_DIALECT_CURRENT, sqlda_fields_out.get());
            if (isc_status::has_error(status) && has_local_transaction())
                tr->rollback();
        }, true);

    prepared = true;

    if (sqlda_fields_out.resize_to_fit())
    {
        guarded_execute([this](ISC_STATUS *status)
            {
                isc_dsql_describe_bind(status, stmt.get(), xsqlda::version, sqlda_fields_out.get());
            }, true);
    }
    sqlda_fields_out.init();

    char ac_buffer[9];
    guarded_execute([&](ISC_STATUS *status)
        {
            char q_type = isc_info_sql_stmt_type;
            isc_dsql_sql_info(status, stmt.get(), 1, &q_type, sizeof(ac_buffer), ac_buffer);
        }, true);
    int i_length = isc_vax_integer(&ac_buffer[1], 2);
    statement_type = isc_vax_integer(&ac_buffer[3], i_length);
}

bool firebird_statement::is_prepared() const
{
    return prepared;
}

void firebird_statement::execute_ddl()
{
    execute();
}

void firebird_statement::execute_non_query()
{
    execute();
}

void firebird_statement::execute()
{
    if (!is_prepared())
        throw db_exception("Statement not prepared!");

    if (!has_current_transaction() && !has_local_transaction())
        tr->begin();    // start local transaction

    guarded_execute([this](ISC_STATUS *status)
        {
            isc_tr_handle *trans = has_local_transaction() ? get_local_transaction_handle() : get_current_transaction_handle();
            isc_dsql_execute2(status, trans, stmt.get(), xsqlda::version, sqlda_params_in.get(), sqlda_fields_out.get());
            if (isc_status::has_error(status) && has_local_transaction())
                tr->rollback();
        }, true);

    if (has_local_transaction())
        tr->commit();
}

void firebird_statement::reset()
{
    sqlda_params_in.reset_values();

    if (!has_current_transaction() && !has_local_transaction())
        tr->begin();    // start local transaction
}

handle firebird_statement::get_handle() const
{
    return std::static_pointer_cast<void>(stmt);
}

xsqlda *firebird_statement::access_sqlda_in()
{
    return &sqlda_params_in;
}

xsqlda *firebird_statement::access_sqlda_out()
{
    return &sqlda_fields_out;
}

} // namespace cpp_db
