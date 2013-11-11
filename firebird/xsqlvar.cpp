#include "xsqlvar.h"

#include <algorithm>

namespace cpp_db
{

const ISC_SHORT sql_ind_used = 1;

template<typename T>
void xsqlvar::write_value_to_sql_var(T value)
{
    memcpy(var.sqldata, &value, var.sqllen);
}

xsqlvar::xsqlvar(XSQLVAR &var_in)
    : var(var_in)
{
}

std::string xsqlvar::get_column_name() const
{
    return std::string(var.sqlname, std::max(static_cast<ISC_SHORT>(sizeof(var.sqlname) / sizeof(var.sqlname[0])), var.sqlname_length));
}

cpp_db::xsqlvar::operator XSQLVAR *()
{
    return &var;
}

bool xsqlvar::can_be_null() const
{
    return (var.sqltype & sql_ind_used) == sql_ind_used;
}

bool xsqlvar::is_null() const
{
    if (can_be_null())
        return var.sqlind[0] == -1;
    return false;
}

int xsqlvar::type() const
{
    return var.sqltype & ~sql_ind_used;
}

void xsqlvar::set_null()
{
    if (can_be_null())
        var.sqlind[0] = -1;
}

void xsqlvar::set_not_null()
{
    if (can_be_null())
        var.sqlind[0] = 0;
}

void xsqlvar::write_value_to_sql_var(const std::string &value)
{
    ISC_SCHAR *destination = var.sqldata;
    uint16_t len{ 0 };
    ISC_SCHAR fill_char{ ' ' };

    if (value.length() > static_cast<unsigned>(var.sqllen))
        len = var.sqllen;
    else
        len = value.length();

    if (type() == SQL_VARYING)
    {
        memcpy(destination, &len, sizeof(len));
        destination += sizeof(len);
        fill_char = 0;
    }
    memcpy(destination, value.c_str(), len);
    if (len < var.sqllen)
        memset(destination + len, fill_char, var.sqllen - len);
}

void xsqlvar::allocate()
{
    switch (type())
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

void xsqlvar::deallocate()
{
    delete [] var.sqldata;
    delete [] var.sqlind;
}

void xsqlvar::reset_value()
{
    if (can_be_null())
        var.sqlind[0] = -1;

    switch(type())
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

value xsqlvar::get_column_value() const
{
    if (is_null())
        return null_type{};

    switch(type())
    {
    case  SQL_TEXT:
        return std::string(var.sqldata, var.sqllen);
    case  SQL_VARYING:
        return std::string(var.sqldata+sizeof(short), *reinterpret_cast<short *>(var.sqldata));
    case  SQL_SHORT:
        return *reinterpret_cast<short *>(var.sqldata);
    case  SQL_LONG:
        return *reinterpret_cast<long *>(var.sqldata);
    case  SQL_INT64:
        return *reinterpret_cast<int64_t *>(var.sqldata);
    case  SQL_FLOAT:
        return *reinterpret_cast<float *>(var.sqldata);
    case  SQL_DOUBLE:
        return *reinterpret_cast<double *>(var.sqldata);
    case  SQL_TIMESTAMP:
        {
            tm timestamp;
            isc_decode_timestamp(reinterpret_cast<ISC_TIMESTAMP *>(var.sqldata), &timestamp);
            return timestamp;
        }
        break;
    case  SQL_TYPE_TIME:
        {
            tm timestamp;
            isc_decode_sql_time(reinterpret_cast<ISC_TIME *>(var.sqldata), &timestamp);
            return timestamp;
        }
    case  SQL_TYPE_DATE:
        {
            tm timestamp;
            isc_decode_sql_date(reinterpret_cast<ISC_DATE *>(var.sqldata), &timestamp);
            return timestamp;
        }
        break;
    case  SQL_D_FLOAT:
    case  SQL_QUAD:
    case  SQL_BLOB:
    case  SQL_ARRAY:
    default:
        return null_type{};
    }
}

void xsqlvar::set_column_value(const value &val)
{
    if (can_be_null())
    {
        if (is_null())
        {
            set_null();
            return;
        }
        else
            set_not_null();
    }

    switch(type())
    {
    case  SQL_TEXT:
    case  SQL_VARYING:
        write_value_to_sql_var(val.get_value<std::string>());
        break;
    case  SQL_SHORT:
        write_value_to_sql_var(val.get_value<int16_t>());
        break;
    case  SQL_LONG:
        write_value_to_sql_var(val.get_value<int32_t>());
        break;
    case  SQL_INT64:
        write_value_to_sql_var(val.get_value<int64_t>());
        break;
    case  SQL_FLOAT:
        write_value_to_sql_var(val.get_value<float>());
        break;
    case  SQL_DOUBLE:
        write_value_to_sql_var(val.get_value<double>());
        break;
    case  SQL_TIMESTAMP:
        {
            tm timestamp = val.get_value<tm>();
            isc_encode_timestamp(&timestamp, reinterpret_cast<ISC_TIMESTAMP *>(var.sqldata));
        }
        break;
    case  SQL_TYPE_TIME:
        {
            tm timestamp = val.get_value<tm>();
            isc_encode_sql_time(&timestamp, reinterpret_cast<ISC_TIME *>(var.sqldata));
        }
    case  SQL_TYPE_DATE:
        {
            tm timestamp = val.get_value<tm>();
            isc_encode_sql_date(&timestamp, reinterpret_cast<ISC_DATE *>(var.sqldata));
        }
        break;
    case  SQL_D_FLOAT:
    case  SQL_QUAD:
    case  SQL_BLOB:
    case  SQL_ARRAY:
    default:
        break;
    }
}

}
