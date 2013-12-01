#ifndef CPP_DB_CONNECTION_H
#define CPP_DB_CONNECTION_H

#include "usings.h"
#include "no_authentication.h"

#include <memory>
#include <string>

namespace cpp_db
{

struct driver_interface;
struct connection_interface;

class statement;
class transaction;

class connection
{
public:
    explicit connection(const std::string &drivername);
    ~connection();

    connection(const connection &) = delete;
    connection &operator=(const connection &) = delete;

#if !defined(_MSC_VER) || (_MSC_FULL_VER > 180021005)
	connection(connection &&) = default;
    connection &operator=(connection &&) = default;
#else
	connection(connection && other);
	connection &operator=(connection && other);
#endif

	void open(const std::string &database, const authentication &auth = no_authentication{}, const key_value_pair & options = key_value_pair{});
	void close();
    bool is_open() const;
    handle get_handle() const;

private:
    friend class statement;
    friend class transaction;
    std::shared_ptr<driver_interface> driver_impl;
    std::shared_ptr<connection_interface> conn_impl;
};

}

#endif
