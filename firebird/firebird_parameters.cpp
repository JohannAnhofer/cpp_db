#include "firebird_parameters.h"
#include "isc_status.h"
#include "parameter.h"

#include <algorithm>

namespace cpp_db
{

firebird_parameters::firebird_parameters(const handle &stmt_in)
    : stmt(std::static_pointer_cast<isc_stmt_handle>(stmt_in))
{
    guarded_execute([this](ISC_STATUS *status){isc_dsql_describe_bind(status, stmt.lock().get(), 1, isqlda.get());}, true);

	if (isqlda.resize_to_fit())
		guarded_execute([this](ISC_STATUS *status){isc_dsql_describe_bind(status, stmt.lock().get(), 1, isqlda.get());}, true);
}

int firebird_parameters::get_count() const
{
	return isqlda.get_var_count();
}

void firebird_parameters::bind(const parameter &param)
{
	int index{ -1 };
	if (param.has_index())
		index = param.get_index() - 1;
	else
		index = find_param_pos(param.get_name());

}

int firebird_parameters::find_param_pos(const std::string &name) const
{
	for (int var = 0; var < isqlda.get_var_count(); ++var)
	{
		if (name == isqlda.get_field_name(var))
			return var;
	}
    return -1;
}

} // namespace cpp_db
