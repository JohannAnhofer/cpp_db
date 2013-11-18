#ifndef CPP_DB_ODBC_CONNECTION_H
#define CPP_DB_ODBC_CONNECTION_H

#include "connection_interface.h"

#include <memory>

namespace cpp_db
{

class odbc_driver;

class odbc_connection : public connection_interface
{
public:
public:
    ~odbc_connection();

    void open(const std::string &database, const authentication &auth = no_authentication{}, const key_value_pair & = key_value_pair{}) override;
    void close() override;
    bool is_open() const override;
    handle get_handle() const override;
    void set_current_transaction(const shared_transaction_ptr &trans) override;
    shared_transaction_ptr get_current_transaction() const override;

private:
    odbc_connection();
    friend class odbc_driver;
    std::weak_ptr<transaction_interface> current_transaction;
};

} // namespace cpp_db

#endif // CPP_DB_ODBC_CONNECTION_H
