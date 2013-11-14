#ifndef CPP_DB_XSQLDA_H
#define CPP_DB_XSQLDA_H

#include "xsqlvar.h"

#include "ibase.h"

namespace cpp_db
{

class xsqlda
{
public:
    explicit xsqlda(bool for_input_params);
	~xsqlda();

	int get_var_count() const;
	bool resize_to_fit();
    void allocate_vars();
    void reset_values();
    void clear();

    xsqlvar operator[](int index) const;
    explicit operator XSQLDA *();

    static const int version = SQLDA_VERSION1;

private:
	void allocate(int vars_count);
	void release();
    void resize(int var_count);

private:
	XSQLDA *sqlda;
    bool is_input_da;
};

}

#endif
