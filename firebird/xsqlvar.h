#ifndef CPP_DB_XSQLVAR_H
#define CPP_DB_XSQLVAR_H

#include "value.h"

#include "ibase.h"

#include <string>
#include <cstring>

namespace cpp_db
{
class xsqlvar
{
public:
    explicit xsqlvar(XSQLVAR &var_in);

    bool can_be_null() const;
    bool is_null() const;
    void set_null();
    void set_not_null();

    int type() const;

    void allocate(ISC_SHORT is_null);
    void deallocate();

    void reset_value();

    value get_column_value() const;
    void set_column_value(const value &);

    std::string get_column_name() const;

    explicit operator XSQLVAR *();

private:
    void write_value_to_sql_var(const std::string &value);

    template<typename T>
    void write_value_to_sql_var(T value);

    XSQLVAR &var;
};

}

#endif // CPP_DB_XSQLVAR_H
