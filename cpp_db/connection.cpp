#include "connection.h"
#include "connection_interface.h"
#include "driver.h"
#include "check_pointer.h"

#include <stdexcept>

namespace cpp_db
{
    connection::connection(std::shared_ptr<driver_interface> sql_driver)
        : driver_impl(sql_driver)
		, conn_impl(sql_driver->make_connection())
    {
		if (!conn_impl)
			throw std::runtime_error("No connection object from driver!");
	}

	connection::~connection()
	{
	}

#if defined(_MSC_VER) && (_MSC_FULL_VER <= 180021005)
	connection::connection(connection && other)
		: driver_impl(std::move(other.driver_impl))
		, conn_impl(std::move(other.conn_impl))
	{
	}

	connection &connection::operator=(connection && other)
	{
		if (this != &other)
		{
			driver_impl = std::move(other.driver_impl);
			conn_impl = std::move(other.conn_impl);
		}
		return *this;
	}
#endif

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
}
