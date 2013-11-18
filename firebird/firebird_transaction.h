#ifndef CPP_DB_FIREBIRD_TRANSACTION_H
#define CPP_DB_FIREBIRD_TRANSACTION_H

#include "usings.h"
#include "transaction_interface.h"

#include "ibase.h"

#include <memory>

namespace cpp_db
{

class firebird_driver;

class firebird_transaction : public transaction_interface
{
public:
    ~firebird_transaction();

    void begin() override;
    void commit() override;
    void rollback() override;
    bool is_open() const override;
    handle get_handle() const override;

private:
    friend class firebird_driver;
    explicit firebird_transaction(const shared_connection_ptr &conn_handle);
    isc_db_handle *get_db_handle() const;

private:
    std::weak_ptr<connection_interface> conn_impl;
    std::shared_ptr<isc_tr_handle> tr;
};

} // namespace cpp_db

#endif // CPP_DB_FIREBIRD_TRANSACTION_H
