#ifndef CPP_DB_SQLITE_CONNECTION_H
#define CPP_DB_SQLITE_CONNECTION_H

#include "connection_interface.h"
#include "sqlite3.h"

#include <memory>

namespace cpp_db
{

class sqlite_driver;

class sqlite_connection : public connection_interface
{
public:
	~sqlite_connection();

	void open(const std::string &database, const authentication &auth = no_authentication{}, const key_value_pair & = key_value_pair{}) override;
    void close() override;
    bool is_open() const override;
    handle get_handle() const override;
    void set_current_transaction(const shared_transaction_ptr &tr) override;
    shared_transaction_ptr get_current_transaction() const override;

private:
    friend class sqlite_driver;
    sqlite_connection() = default;
    std::shared_ptr<sqlite3> db;
    std::weak_ptr<transaction_interface> current_transaction;

};

}

#endif // CPP_DB_SQLITE_CONNECTION_H
