#ifndef CPP_DB_CONNECTION_H
#define CPP_DB_CONNECTION_H

#include "usings.h"

#include <memory>
#include <string>

namespace cpp_db
{

class driver;
struct connection_interface;

class connection
{
public:
    explicit connection(const std::shared_ptr<driver> &sql_driver);
	~connection();

	connection(const connection &other) = delete;
	connection &operator=(const connection &other) = delete;

	void open(const std::string &database);
	void close();
    bool is_open() const;
    handle get_handle() const;

	std::shared_ptr<driver> get_driver() const;

private:
	std::shared_ptr<driver> driver_impl;
	std::unique_ptr<connection_interface> conn_impl;
};

}

#endif
