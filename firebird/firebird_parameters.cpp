#include "firebird_parameters.h"
#include "firebird_statement.h"
#include "isc_status.h"
#include "parameter.h"

#include <cstring>

namespace cpp_db
{

static void write_value_to_sql_var(XSQLVAR &var, const std::string &value);

template<typename T>
void write_value_to_sql_var(XSQLVAR &var, T value)
{
	memcpy(var.sqldata, &value, var.sqllen);
}

const ISC_SHORT sql_ind_used = 1;


firebird_parameters::firebird_parameters(const shared_statement_ptr &stmt_in)
    : stmt(stmt_in)
{
    isqlda = dynamic_cast<firebird_statement *>(stmt_in.get())->access_sqlda_in();

    guarded_execute([this](ISC_STATUS *status){isc_dsql_describe_bind(status, get_statement_handle(), xsqlda::version, isqlda->get());}, true);

    if (isqlda->resize_to_fit())
        guarded_execute([this](ISC_STATUS *status){isc_dsql_describe_bind(status, get_statement_handle(), xsqlda::version, isqlda->get());}, true);

    isqlda->init();
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

    auto &var = (*isqlda)[index];

    if ((*isqlda)[index].sqltype & sql_ind_used)
	{
		if (is_null(param))
		{
            var.sqlind[0] = -1;
			return;
		}
	}
    switch(var.sqltype & ~sql_ind_used)
    {
	case  SQL_TEXT:
	case  SQL_VARYING:
        write_value_to_sql_var(var, param.get_value<std::string>());
		break;
	case  SQL_SHORT:
        write_value_to_sql_var(var, param.get_value<int16_t>());
		break;
	case  SQL_LONG:
        write_value_to_sql_var(var, param.get_value<int32_t>());
		break;
	case  SQL_INT64:
        write_value_to_sql_var(var, param.get_value<int64_t>());
		break;
	case  SQL_FLOAT:
        write_value_to_sql_var(var, param.get_value<float>());
		break;
	case  SQL_DOUBLE:
        write_value_to_sql_var(var, param.get_value<double>());
		break;
	case  SQL_TIMESTAMP:
	case  SQL_TYPE_TIME:
	case  SQL_TYPE_DATE:
		break;
	case  SQL_D_FLOAT:
	case  SQL_QUAD:
	case  SQL_BLOB:
	case  SQL_ARRAY:	
	default:
		break;
	}
}

int firebird_parameters::find_param_pos(const std::string &name) const
{
    for (int var = 0; var < isqlda->get_var_count(); ++var)
	{
        if (name == isqlda->get_field_name(var))
			return var;
	}
    return -1;
}

isc_stmt_handle *firebird_parameters::get_statement_handle() const
{
    return std::static_pointer_cast<isc_stmt_handle>(stmt->get_handle()).get();
}

void write_value_to_sql_var(XSQLVAR &var, const std::string &value)
{
	ISC_SCHAR *destination = var.sqldata;
	uint16_t len{ 0 };
	ISC_SCHAR fill_char{ ' ' };

	if (value.length() > static_cast<unsigned>(var.sqllen))
		len = var.sqllen;
	else
		len = value.length();

	if ((var.sqltype & sql_ind_used) == SQL_VARYING)
	{		 
		memcpy(destination, &len, sizeof(len));
		destination += sizeof(len);
		fill_char = 0;
	}
	memcpy(destination, value.c_str(), len);
	if (len < var.sqllen)
		memset(destination + len, fill_char, var.sqllen - len);
}

} // namespace cpp_db
