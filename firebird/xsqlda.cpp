#include "xsqlda.h"

#include <stdexcept>
#include <cstdlib>

namespace cpp_db
{

    xsqlda::xsqlda(bool for_input_params)
		: sqlda{nullptr}
        , is_input_da(for_input_params)
	{
		allocate(1);
	}

	xsqlda::~xsqlda()
	{
		release();
	}

    xsqlda::operator XSQLDA *()
    {
		return sqlda;
	}

    xsqlvar xsqlda::operator[](int index) const
	{
		if (index < 0 || index > sqlda->sqln)
			throw std::overflow_error("No sql variable at given index!");
        return xsqlvar(sqlda->sqlvar[index]);
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
        sqlda->version = version;
		sqlda->sqln = vars_count;
	}

    void xsqlda::allocate_vars()
	{
		for (int var_idx = 0; var_idx < sqlda->sqln; ++var_idx)
            (*this)[var_idx].allocate(is_input_da ? -1 : 0);
    }

    void xsqlda::reset_values()
    {
        for (int var_idx = 0; var_idx < sqlda->sqln; ++var_idx)
            (*this)[var_idx].reset_value();
    }

    void xsqlda::clear()
    {
        release();
        allocate(1);
    }

	void xsqlda::release()
	{
        for (int var_idx = 0; var_idx < sqlda->sqln; ++var_idx)
            (*this)[var_idx].deallocate();
		free(sqlda);
        sqlda = nullptr;
    }

}
