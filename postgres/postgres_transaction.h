#ifndef CPP_DB_POSTGRES_TRANSACTION_H
#define CPP_DB_POSTGRES_TRANSACTION_H

#include "usings.h"
#include "transaction_interface.h"

#include <libpq-fe.h>

#include <memory>

namespace cpp_db
{

class postgres_driver;

class postgres_transaction : public transaction_interface
{
public:
    ~postgres_transaction();

    void begin() override;
    void commit() override;
    void rollback() override;
    bool is_open() const override;
    handle get_handle() const override;

private:
    friend class postgres_driver;
    explicit postgres_transaction(const shared_connection_ptr &conn_handle);
    PGconn *get_db_handle() const;

private:
    std::weak_ptr<connection_interface> conn_impl;
};

} // namespace cpp_db

#endif // CPP_DB_POSTGRES_TRANSACTION_H
