#ifndef CPP_DB_FIREBIRD_TRANSACTION_H
#define CPP_DB_FIREBIRD_TRANSACTION_H

#include "usings.h"
#include "transaction_interface.h"

#include "ibase.h"

#include <memory>

namespace cpp_db
{

class firebird_transaction : public transaction_interface
{
public:
    firebird_transaction(const handle &conn_handle);
    ~firebird_transaction();

    void begin() override;
    void commit() override;
    void rollback() override;
    bool is_open() const override;
    handle get_handle() const override;

private:
    std::weak_ptr<isc_db_handle> db;
    std::shared_ptr<isc_tr_handle> tr;
    isc_tr_handle transaction;
};

} // namespace cpp_db

#endif // CPP_DB_FIREBIRD_TRANSACTION_H
