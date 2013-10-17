#ifndef CPP_DB_SQLITE_STATEMENT_H
#define CPP_DB_SQLITE_STATEMENT_H

#include "statement_interface.h"

#include <memory>

extern "C"
{
struct sqlite3_stmt;
struct sqlite3;
}

namespace cpp_db
{

class sqlite_statement  : public statement_interface
{
public:
    sqlite_statement(const handle &connection);

    void prepare(const std::string &sqlcmd) override;
    bool is_prepared() const override;
    void execute() override;
    void reset() override;
    handle get_handle() const override;

private:
    std::shared_ptr<sqlite3_stmt> stmt;
    std::weak_ptr<sqlite3> db;
    const char *tail;
};

} // namespace cpp_db

#endif // CPP_DB_SQLITE_STATEMENT_H
