#include "connection.h"
#include "connection_interface.h"
#include "driver.h"
#include "check_pointer.h"

#include <stdexcept>

namespace cpp_db
{
    connection::connection(std::shared_ptr<driver> sql_driver)
        : driver_impl(sql_driver)
		, conn_impl(sql_driver->make_connection())
    {
		if (!conn_impl)
			throw std::runtime_error("No connection object from driver!");
	}

	connection::~connection()
	{
	}

    connection::connection(connection &&other)
        : driver_impl(other.driver_impl)
        , conn_impl(other.conn_impl.release())
    {
        other.driver_impl.reset();
    }

    connection & connection::operator=(connection &&other)
    {
        if (this != &other)
        {
            driver_impl = other.driver_impl;
            other.driver_impl.reset();
            conn_impl.reset(other.conn_impl.release());
        }
        return*this;
    }

	void connection::open(const std::string &database, const authentication &auth, const key_value_pair & options)
	{
		conn_impl->open(database, auth, options);
	}

	void connection::close()
	{
		conn_impl->close();
    }

    bool connection::is_open() const
    {
		return conn_impl->is_open();
    }

    handle connection::get_handle() const
    {
		return conn_impl->get_handle();
    }

	std::shared_ptr<driver> connection::get_driver() const
	{
		return driver_impl;
	}
}
