#ifndef CPP_DB_ODBC_CONNECTION_H
#define CPP_DB_ODBC_CONNECTION_H

#include "connection_interface.h"

#include <memory>

#if defined(WIN32)
#include <windows.h>
#endif

#include <sql.h>

namespace cpp_db
{

class odbc_driver;

struct odbc_handle;

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
	std::shared_ptr<odbc_handle> _handle;
	std::weak_ptr<transaction_interface> current_transaction;

	friend class odbc_driver;
	odbc_connection();
};

} // namespace cpp_db

#endif // CPP_DB_ODBC_CONNECTION_H
