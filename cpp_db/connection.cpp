#include "connection.h"
#include "connection_interface.h"
#include "driver.h"
#include "check_pointer.h"

#include <stdexcept>

namespace cpp_db
{
	connection::connection(const std::shared_ptr<driver> &sql_driver)
		: driver_impl(tools::check_pointer(sql_driver, "No driver object!"))
		, conn_impl(sql_driver->make_connection())
    {
		if (!conn_impl)
			throw std::runtime_error("No connection object from driver!");
	}

	connection::~connection()
	{
	}

	void connection::open(const std::string &database)
	{
		conn_impl->open(database);
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
