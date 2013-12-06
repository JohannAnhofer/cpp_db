#include "firebird_parameters.h"
#include "firebird_statement.h"
#include "isc_status.h"
#include "parameter.h"

#include <cstring>
#include <ctime>

namespace cpp_db
{

firebird_parameters::firebird_parameters(const shared_statement_ptr &stmt_in)
    : stmt(stmt_in)
{
    isqlda = dynamic_cast<firebird_statement *>(stmt_in.get())->access_sqlda_in();

    guarded_execute([this](ISC_STATUS *status){isc_dsql_describe_bind(status, get_statement_handle(), xsqlda::version, static_cast<XSQLDA*>(*isqlda));}, true);

    if (isqlda->resize_to_fit())
        guarded_execute([this](ISC_STATUS *status){isc_dsql_describe_bind(status, get_statement_handle(), xsqlda::version, static_cast<XSQLDA*>(*isqlda));}, true);

    isqlda->allocate_vars();
}

int firebird_parameters::get_count() const
{
    return isqlda->get_var_count();
}

void firebird_parameters::bind(const parameter &param)
{
	int index{ -1 };
	if (param.has_index())
		index = param.get_index() - 1;
	else
		index = find_param_pos(param.get_name());

    (*isqlda)[index].set_column_value(param.get_value());
}

int firebird_parameters::find_param_pos(const std::string &name) const
{
    for (int var = 0; var < isqlda->get_var_count(); ++var)
	{
        if (name == (*isqlda)[var].get_column_name())
			return var;
	}
    return -1;
}

handle firebird_parameters::get_handle() const
{
    return handle{};
}

isc_stmt_handle *firebird_parameters::get_statement_handle() const
{
    return std::static_pointer_cast<isc_stmt_handle>(stmt->get_handle()).get();
}

} // namespace cpp_db
