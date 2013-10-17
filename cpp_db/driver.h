#ifndef CPP_DB_DRIVER_H
#define CPP_DB_DRIVER_H

#include <string>
#include <memory>

namespace cpp_db
{
using handle = std::shared_ptr<void>;

class driver;
class parameter;
class value;

struct connection_interface
{
	virtual ~connection_interface() {}
	virtual void open(const std::string &) = 0;
	virtual void close() = 0;
	virtual bool is_open() const = 0;
	virtual handle get_handle() const = 0;
};

struct statement_interface
{
	virtual ~statement_interface() {}

	virtual void prepare(const std::string &sqlcmd) = 0;
	virtual bool is_prepared() const = 0;
	virtual void execute() = 0;
	virtual void reset() = 0;
	virtual handle get_handle() const = 0;
};

struct parameters_interface
{
	virtual ~parameters_interface() {}
	virtual int get_count() const = 0;
	virtual void bind(const parameter &) = 0;
};

struct result_interface
{
	virtual ~result_interface() {}

	virtual int get_column_count() const = 0;
	virtual bool is_eof() const = 0;
	virtual void move_first() = 0;
	virtual void move_next() = 0;
	virtual void move_prev() = 0;
	virtual value get_column_value(int column) const = 0;
	virtual value get_column_value(const std::string &column_name) const = 0;
	virtual std::string get_column_name(int column) const = 0;
	virtual int get_column_index(const std::string &column_name) const = 0;
};

struct transaction_interface
{
	virtual ~transaction_interface() {}

	virtual void begin() = 0;
	virtual void commit() = 0;
	virtual void rollback() = 0;
	virtual bool is_open() const = 0;
};

class driver
{
public:
	virtual ~driver() {}

    virtual connection_interface *make_connection() const = 0;
	virtual statement_interface *make_statement(const handle &conn_handle) const = 0;
	virtual parameters_interface *make_parameters(const handle &stmt_handle) const = 0;
	virtual result_interface *make_result(const handle &stmt_handle) const = 0;
	virtual transaction_interface *make_transaction(const handle &conn_handle) const = 0;
};

}

#endif // CPP_DB_DRIVER_H
