#ifndef CPP_DB_FIREBIRD_STATEMENT_H
#define CPP_DB_FIREBIRD_STATEMENT_H

#include "statement_interface.h"

#include "ibase.h"

namespace cpp_db
{

class firebird_statement : public statement_interface
{
public:
    firebird_statement(const handle &connection);

    void prepare(const std::string &sqlcmd) override;
    bool is_prepared() const override;
    void execute() override;
    void reset() override;
    handle get_handle() const override;

private:
    std::weak_ptr<isc_db_handle> db;
};

} // namespace cpp_db

#endif // CPP_DB_FIREBIRD_STATEMENT_H
