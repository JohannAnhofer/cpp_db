#ifndef CPP_DB_STATEMENT_INTERFACE_H
#define CPP_DB_STATEMENT_INTERFACE_H

#include "usings.h"

#include <string>

namespace cpp_db
{

struct statement_interface
{
    virtual ~statement_interface() {}

    virtual void prepare(const std::string &sqlcmd) = 0;
    virtual bool is_prepared() const = 0;
    virtual void execute_ddl() = 0;
    virtual void execute_non_query() = 0;
    virtual void execute() = 0;
    virtual void reset() = 0;
    virtual handle get_handle() const = 0;
};

}

#endif // CPP_DB_STATEMENT_INTERFACE_H
