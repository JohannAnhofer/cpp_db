#ifndef CPP_DB_CONNECTION_H
#define CPP_DB_CONNECTION_H

#include "usings.h"
#include "no_authentication.h"

#include <memory>
#include <string>

namespace cpp_db
{

class driver;
class statement;
class transaction;
struct connection_interface;

class connection
{
public:
	~connection();

	connection(const connection &other) = delete;
	connection &operator=(const connection &other) = delete;

#if !defined(_MSC_VER) || (_MSC_VER > 1800)
    connection(connection &&) = default;
    connection &operator=(connection &&) = default;
#else
    connection(connection &&other);
    connection &operator=(connection &&other);
#endif

	void open(const std::string &database, const authentication &auth = no_authentication{}, const key_value_pair & options = key_value_pair{});
	void close();
    bool is_open() const;
    handle get_handle() const;

    template<typename DriverType>
    static connection create()
    {
        std::shared_ptr<DriverType> driver(new DriverType);
        return connection(driver);
    }

private:
    explicit connection(std::shared_ptr<driver> sql_driver);
    friend class statement;
    friend class transaction;
	std::shared_ptr<driver> driver_impl;
    std::shared_ptr<connection_interface> conn_impl;
};

}

#endif
