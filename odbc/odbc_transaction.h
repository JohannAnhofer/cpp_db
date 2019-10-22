#ifndef CPP_DB_ODBC_TRANSACTION_H
#define CPP_DB_ODBC_TRANSACTION_H

#include "transaction_interface.h"

namespace cpp_db
{

class odbc_driver;
struct connection_interface;

class odbc_transaction : public transaction_interface
{
public:
    ~odbc_transaction() override;

    void begin() override;
    void commit() override;
    void rollback() override;
    bool is_open() const override;
    handle get_handle() const override;

private:
	bool _enabled;
	std::weak_ptr<connection_interface> conn_impl;

	friend class odbc_driver;
    explicit odbc_transaction(const shared_connection_ptr &conn_handle);
};

} // namespace cpp_db

#endif // CPP_DB_ODBC_TRANSACTION_H
