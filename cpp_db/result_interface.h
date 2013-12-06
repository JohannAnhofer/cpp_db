#ifndef CPP_DB_RESULT_INTERFACE_H
#define CPP_DB_RESULT_INTERFACE_H

#include "usings.h"

#ifdef USE_BOOST_ANY
#include "value.h"
#endif

#include <string>

namespace cpp_db
{

#ifndef USE_BOOST_ANY
class value;
#endif

struct result_interface
{
    virtual ~result_interface() {}

    virtual int get_column_count() const = 0;
    virtual bool is_eof() const = 0;
    virtual void move_next() = 0;
    virtual value get_column_value(int column) const = 0;
    virtual value get_column_value(const std::string &column_name) const = 0;
    virtual std::string get_column_name(int column) const = 0;
    virtual int get_column_index(const std::string &column_name) const = 0;
    virtual bool is_column_null(int column) const = 0;
    virtual bool is_column_null(const std::string &column_name) const = 0;
    virtual handle get_handle() const = 0;
};

}

#endif // CPP_DB_RESULT_INTERFACE_H
