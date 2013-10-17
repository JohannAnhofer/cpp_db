#ifndef CPP_DB_PARAMETERS_INTERFACE_H
#define CPP_DB_PARAMETERS_INTERFACE_H

namespace cpp_db
{

class parameter;

struct parameters_interface
{
    virtual ~parameters_interface() {}
    virtual int get_count() const = 0;
    virtual void bind(const parameter &) = 0;
};

}

#endif // CPP_DB_PARAMETERS_INTERFACE_H
