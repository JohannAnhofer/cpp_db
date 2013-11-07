#ifndef CPP_DB_FIREBIRD_STATEMENT_H
#define CPP_DB_FIREBIRD_STATEMENT_H

#include "statement_interface.h"

#include "ibase.h"

#include <memory>

namespace cpp_db
{

class driver;

class firebird_statement : public statement_interface
{
public:
    firebird_statement(const handle &connection, driver *current_driver);
	~firebird_statement();

    void prepare(const std::string &sqlcmd) override;
    bool is_prepared() const override;
    void execute() override;
    void reset() override;
    handle get_handle() const override;

private:
    std::weak_ptr<isc_db_handle> db;
	std::shared_ptr<isc_stmt_handle> stmt;
	std::shared_ptr<isc_tr_handle> tr;
    driver *driver_impl;
    bool prepared;
};

} // namespace cpp_db

#endif // CPP_DB_FIREBIRD_STATEMENT_H