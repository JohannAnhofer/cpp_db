#ifndef CPP_DB_SQLITE_TRANSACTION_H
#define CPP_DB_SQLITE_TRANSACTION_H

#include "usings.h"
#include "transaction_interface.h"

#include "sqlite3.h"

#include <memory>

namespace cpp_db
{

class sqlite_driver;

class sqlite_transaction : public transaction_interface
{
public:
    ~sqlite_transaction();

    void begin() override;
    void commit() override;
    void rollback() override;
    bool is_open() const override;
    virtual handle get_handle() const override;

private:
    friend class sqlite_driver;
    explicit sqlite_transaction(const shared_connection_ptr &conn_in);
    void execute(const char *sql);
	sqlite3 *get_db_handle() const;

    std::weak_ptr<connection_interface> conn;
    int open_count;
};

} // namespace cpp_db

#endif // CPP_DB_SQLITE_TRANSACTION_H
