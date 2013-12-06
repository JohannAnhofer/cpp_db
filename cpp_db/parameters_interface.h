#ifndef CPP_DB_PARAMETERS_INTERFACE_H
#define CPP_DB_PARAMETERS_INTERFACE_H

#include "usings.h"

namespace cpp_db
{

class parameter;

struct parameters_interface
{
    virtual ~parameters_interface() {}
    virtual int get_count() const = 0;
    virtual void bind(const parameter &) = 0;
    virtual handle get_handle() const = 0;
};

}

#endif // CPP_DB_PARAMETERS_INTERFACE_H
