#ifndef CPP_DB_SQLITE_TRANSACTION_H
#define CPP_DB_SQLITE_TRANSACTION_H

#include "usings.h"
#include "transaction_interface.h"

extern "C"
{
typedef struct sqlite3 sqlite3;
}

#include <memory>

namespace cpp_db
{

class sqlite_transaction : public transaction_interface
{
public:
    sqlite_transaction(const handle &conn_handle);
    ~sqlite_transaction();

    void begin() override;
    void commit() override;
    void rollback() override;
    bool is_open() const override;

private:
    void execute(const char *sql);

    std::weak_ptr<sqlite3> db;
    int open_count;
};

} // namespace cpp_db

#endif // CPP_DB_SQLITE_TRANSACTION_H
