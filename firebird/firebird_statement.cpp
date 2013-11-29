#include "firebird_statement.h"
#include "isc_status.h"
#include "driver_interface.h"
#include "transaction_interface.h"
#include "connection_interface.h"
#include "lock_or_throw.h"

#include "ibase.h"

#include <functional>
#include <stdexcept>

namespace cpp_db
{

enum class firebird_statement_type
{
    stmt_invalid        = -1,
    stmt_select         = isc_info_sql_stmt_select,
    stmt_insert         = isc_info_sql_stmt_insert,
    stmt_update         = isc_info_sql_stmt_update,
    stmt_delete         = isc_info_sql_stmt_delete,
    stmt_ddl            = isc_info_sql_stmt_ddl,
    stmt_get_segment    = isc_info_sql_stmt_get_segment,
    stmt_put_segment    = isc_info_sql_stmt_put_segment,
    stmt_exec_procedure = isc_info_sql_stmt_exec_procedure,
    stmt_start_trans    = isc_info_sql_stmt_start_trans,
    stmt_commit         = isc_info_sql_stmt_commit,
    stmt_rollback       = isc_info_sql_stmt_rollback,
    stmt_select_for_upd = isc_info_sql_stmt_select_for_upd,
    stmt_set_generator  = isc_info_sql_stmt_set_generator,
    stmt_savepoint      = isc_info_sql_stmt_savepoint
};

enum class firebird_data_type
{
    sql_text        = SQL_TEXT,
    sql_var_text    = SQL_VARYING,
    sql_short       = SQL_SHORT,
    sql_long        = SQL_LONG,
    sql_float       = SQL_FLOAT,
    sql_double      = SQL_DOUBLE,
    sql_d_float     = SQL_D_FLOAT,
    sql_timestamp   = SQL_TIMESTAMP,
    sql_blob        = SQL_BLOB,
    sql_array       = SQL_ARRAY,
    sql_quad        = SQL_QUAD,
    sql_time        = SQL_TYPE_TIME,
    sql_date        = SQL_TYPE_DATE,
    sql_int64       = SQL_INT64,
    sql_null        = SQL_NULL
};

firebird_statement::firebird_statement(const shared_connection_ptr &conn, shared_transaction_ptr trans)
    : conn_impl(conn)
    , tr(trans)
    , stmt(new isc_stmt_handle{0})
    , prepared{false}
    , sqlda_params_in(true)
    , sqlda_fields_out(false)
    , statement_type{firebird_statement_type::stmt_invalid}
{
    if (conn_impl.expired())
        throw db_exception("No database connection for statement!");
    guarded_execute([this](ISC_STATUS *status){isc_dsql_alloc_statement2(status, get_db_handle(), stmt.get());}, true);
}

firebird_statement::~firebird_statement()
{
    try
    {
        if (tr->is_open())
            tr->commit();
    }
    catch(...)
    {
    }

    guarded_execute([this](ISC_STATUS *status){isc_dsql_free_statement(status, stmt.get(), DSQL_drop);}, false);
}

isc_db_handle *firebird_statement::get_db_handle() const
{
    return std::static_pointer_cast<isc_db_handle>(tools::lock_or_throw(conn_impl, "Invalid database connection")->get_handle()).get();
}

isc_tr_handle *firebird_statement::get_current_transaction_handle() const
{
	shared_transaction_ptr curr_trans(tools::lock_or_throw(conn_impl, "Invalid database connection")->get_current_transaction());

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
	return tools::lock_or_throw(conn_impl, "Invalid database connection")->get_current_transaction() != nullptr;
}

bool firebird_statement::has_local_transaction() const
{
    return tr->is_open();
}

firebird_statement_type firebird_statement::determine_statement_type() const
{
    char ac_buffer[9];
    guarded_execute([&](ISC_STATUS *status)
        {
            char q_type = isc_info_sql_stmt_type;
            isc_dsql_sql_info(status, stmt.get(), 1, &q_type, sizeof(ac_buffer), ac_buffer);
        }, true);
    int i_length = isc_vax_integer(&ac_buffer[1], 2);
    return static_cast<firebird_statement_type>(isc_vax_integer(&ac_buffer[3], i_length));
}

void firebird_statement::prepare(const std::string &sqlcmd)
{
    if (prepared)
    {
        prepared = false;
        sqlda_params_in.clear();
        sqlda_fields_out.clear();
        statement_type = firebird_statement_type::stmt_invalid;
    }

    if (!has_current_transaction())
        tr->begin();    // start local transaction

    guarded_execute([this, &sqlcmd](ISC_STATUS *status)
        {
            isc_tr_handle *trans = has_local_transaction() ? get_local_transaction_handle() : get_current_transaction_handle();

            isc_dsql_prepare(status, trans, stmt.get(), 0, sqlcmd.c_str(), SQL_DIALECT_CURRENT, static_cast<XSQLDA*>(sqlda_fields_out));
            if (isc_status::has_error(status) && has_local_transaction())
                tr->rollback();
        }, true);

    prepared = true;

    if (sqlda_fields_out.resize_to_fit())
    {
        guarded_execute([this](ISC_STATUS *status)
            {
                isc_dsql_describe(status, stmt.get(), xsqlda::version, static_cast<XSQLDA*>(sqlda_fields_out));
            }, true);
    }
    sqlda_fields_out.allocate_vars();

    statement_type = determine_statement_type();
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
            if (statement_type == firebird_statement_type::stmt_exec_procedure)
                isc_dsql_execute2(status, trans, stmt.get(), xsqlda::version, static_cast<XSQLDA*>(sqlda_params_in), static_cast<XSQLDA*>(sqlda_fields_out));
            else
                isc_dsql_execute(status, trans, stmt.get(), xsqlda::version, static_cast<XSQLDA*>(sqlda_params_in));
            if (isc_status::has_error(status) && has_local_transaction())
                tr->rollback();
        }, true);

    // if exec procedure returns only one row, the local transaction can be committed after isc_dsql_execute2
    if (!(statement_type == firebird_statement_type::stmt_select || statement_type == firebird_statement_type::stmt_exec_procedure) && has_local_transaction())
        tr->commit();
}

void firebird_statement::reset()
{
    sqlda_params_in.reset_values();

    // shouldn't we commit the last transaction here? If execute was already performed?
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

bool firebird_statement::is_select_statement() const
{
    return statement_type == firebird_statement_type::stmt_select;
}

} // namespace cpp_db
