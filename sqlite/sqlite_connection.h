#ifndef CPP_DB_SQLITE_CONNECTION_H
#define CPP_DB_SQLITE_CONNECTION_H

#include "connection_interface.h"
#include "sqlite3.h"

#include <memory>

namespace cpp_db
{

class sqlite_connection : public connection_interface
{
public:
    ~sqlite_connection();

    void open(const std::string &database) override;
    void close() override;
    bool is_open() const override;
    handle get_handle() const override;

private:
    std::shared_ptr<sqlite3> db;
};

}

#endif // CPP_DB_SQLITE_CONNECTION_H
