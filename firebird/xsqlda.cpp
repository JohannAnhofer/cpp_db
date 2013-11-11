#include "xsqlda.h"

#include <cstdlib>
#include <cstring>

#include <stdexcept>
#include <algorithm>

namespace cpp_db
{

	xsqlda::xsqlda()
		: sqlda{nullptr}
	{
		allocate(1);
	}

	xsqlda::~xsqlda()
	{
		release();
	}

	XSQLDA *xsqlda::get() const
	{
		return sqlda;
	}

	XSQLVAR &xsqlda::operator[](int index)
	{
		if (index < 0 || index > sqlda->sqln)
			throw std::overflow_error("No sql variable at given index!");
		return sqlda->sqlvar[index];
	}

	const XSQLVAR &xsqlda::operator[](int index) const
	{
		if (index < 0 || index > sqlda->sqln)
			throw std::overflow_error("No sql variable at given index!");
		return sqlda->sqlvar[index];
	}

	int xsqlda::get_var_count() const
	{
		return sqlda->sqln;
	}

	bool xsqlda::resize_to_fit()
	{
		if (sqlda->sqld > sqlda->sqln)
		{
            resize(sqlda->sqld);
			return true;
		}
        return false;
    }

    void xsqlda::resize(int var_count)
    {
        release();
        allocate(var_count);

    }

	void xsqlda::allocate(int vars_count)
	{
		size_t size_of_xsqlda_in_bytes = XSQLDA_LENGTH(vars_count);
        sqlda = reinterpret_cast<XSQLDA *>(malloc(size_of_xsqlda_in_bytes));
		if (sqlda == nullptr)
			throw std::runtime_error("Can't allocated XSQLDA!");
		memset(sqlda, 0, size_of_xsqlda_in_bytes);
		sqlda->version = SQLDA_VERSION1;
		sqlda->sqln = vars_count;
	}

    void xsqlda::allocate_vars()
	{
		for (int var_idx = 0; var_idx < sqlda->sqln; ++var_idx)
		{
			auto & var = sqlda->sqlvar[var_idx];

			switch (var.sqltype & ~sql_ind_used)
			{
			case SQL_INT64:
			case SQL_LONG:
			case SQL_SHORT:
			case SQL_FLOAT:
			case SQL_DOUBLE:
			case SQL_TIMESTAMP:
			case SQL_TYPE_TIME:
			case SQL_TYPE_DATE:
			case SQL_TEXT:
			case SQL_BLOB:
				var.sqldata = new char[var.sqllen];
				break;
			case SQL_ARRAY:
				var.sqldata = new char[sizeof(ISC_QUAD)];
				memset(var.sqldata, 0, sizeof(ISC_QUAD));
				break;
			case SQL_VARYING:
				var.sqldata = new char[var.sqllen + sizeof(short)];
				break;
			default:
				// not supported - do not bind.
				var.sqldata = nullptr;
				break;
			}

			if (var.sqltype & sql_ind_used)
			{
				var.sqlind = new short[1];
                var.sqlind[0] = -1;
			}
			else
				var.sqlind = nullptr;
        }
    }

    void xsqlda::reset_values()
    {
        for (int var_idx = 0; var_idx < sqlda->sqln; ++var_idx)
        {
            auto &var = sqlda->sqlvar[var_idx];
            if (var.sqltype & sql_ind_used)
                var.sqlind[0] = -1;
            switch(var.sqltype & ~sql_ind_used)
            {
            case SQL_INT64:
            case SQL_LONG:
            case SQL_SHORT:
            case SQL_FLOAT:
            case SQL_DOUBLE:
            case SQL_TIMESTAMP:
            case SQL_TYPE_TIME:
            case SQL_TYPE_DATE:
            case SQL_TEXT:
            case SQL_BLOB:
                memset(var.sqldata, 0, var.sqllen);
                break;
            case SQL_ARRAY:
                memset(var.sqldata, 0, sizeof(ISC_QUAD));
                break;
                break;
            case SQL_VARYING:
                memset(var.sqldata, 0, var.sqllen + sizeof(short));
                break;
            default:
                break;
            }
        }
    }

    void xsqlda::clear()
    {
        release();
        allocate(1);
    }

	void xsqlda::release()
	{
		for (int var = 0; var < sqlda->sqln; ++var)
		{
			delete[] sqlda->sqlvar[var].sqldata;
			delete[] sqlda->sqlvar[var].sqlind;
		}
		free(sqlda);
		sqlda = nullptr;
	}

	std::string xsqlda::get_field_name(int index) const
	{
		auto & var = sqlda->sqlvar[index];
		return std::string(var.sqlname, std::max(static_cast<ISC_SHORT>(sizeof(var.sqlname) / sizeof(var.sqlname[0])), var.sqlname_length));
	}
}
