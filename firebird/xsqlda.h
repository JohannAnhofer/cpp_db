#ifndef CPP_DB_XSQLDA_H
#define CPP_DB_XSQLDA_H

#include "ibase.h"

#include <string>

namespace cpp_db
{

class xsqlda
{
public:
	xsqlda();
	~xsqlda();

	XSQLDA *get() const;

	int get_var_count() const;
	bool resize_to_fit();
	void init();
    void reset();

	XSQLVAR &operator[](int index);
	const XSQLVAR &operator[](int index) const;

	std::string get_field_name(int index) const;

    static const int version = 1;

private:
	void allocate(int vars_count);
	void release();

    const ISC_SHORT sql_ind_used = 1;

private:
	XSQLDA *sqlda;
};

}

#endif
