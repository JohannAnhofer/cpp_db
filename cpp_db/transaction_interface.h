#ifndef CPP_DB_TRANSACTION_INTERFACE_H
#define CPP_DB_TRANSACTION_INTERFACE_H

namespace cpp_db
{

struct transaction_interface
{
    virtual ~transaction_interface() {}

    virtual void begin() = 0;
    virtual void commit() = 0;
    virtual void rollback() = 0;
    virtual bool is_open() const = 0;
};

}

#endif // CPP_DB_TRANSACTION_INTERFACE_H
