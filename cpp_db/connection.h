#ifndef CPP_DB_CONNECTION_H
#define CPP_DB_CONNECTION_H

#include "usings.h"
#include "no_authentication.h"

#include <memory>
#include <string>

namespace cpp_db
{

class driver;
struct connection_interface;

class connection
{
public:
	~connection();

	connection(const connection &other) = delete;
	connection &operator=(const connection &other) = delete;

    connection(connection &&);
    connection &operator=(connection &&);

	void open(const std::string &database, const authentication &auth = no_authentication{}, const key_value_pair & options = key_value_pair{});
	void close();
    bool is_open() const;
    handle get_handle() const;

	std::shared_ptr<driver> get_driver() const;

    template<typename DriverType>
    static connection create()
    {
        std::shared_ptr<DriverType> driver(new DriverType);
        return connection(driver);
    }

private:
    explicit connection(std::shared_ptr<driver> sql_driver);

	std::shared_ptr<driver> driver_impl;
	std::unique_ptr<connection_interface> conn_impl;
};

}

#endif
