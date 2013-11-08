#ifndef CPP_DB_CONNECTION_INTERFACE_H
#define CPP_DB_CONNECTION_INTERFACE_H

#include "usings.h"
#include "no_authentication.h"

#include <string>
#include <unordered_map>

namespace cpp_db
{

struct connection_interface
{
	virtual ~connection_interface() {}
	virtual void open(const std::string &, const authentication &auth = no_authentication{}, const key_value_pair & = key_value_pair{}) = 0;
    virtual void close() = 0;
    virtual bool is_open() const = 0;
    virtual handle get_handle() const = 0;
    virtual void set_current_transaction(const shared_transaction_ptr &trans) = 0;
    virtual shared_transaction_ptr get_current_transaction() const = 0;
};

}

#endif // CPP_DB_CONNECTION_INTERFACE_H
