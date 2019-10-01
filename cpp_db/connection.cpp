#include "connection.h"
#include "connection_interface.h"
#include "driver_interface.h"
#include "check_pointer.h"
#include "driver_registry.h"
#include "db_exception.h"

#include <stdexcept>

namespace cpp_db
{
    connection::connection(const std::string &drivername)
        : driver_impl(driver_registry::make_driver(drivername))
        , conn_impl(driver_impl->make_connection())
    {
		if (!conn_impl)
			throw db_exception("No connection object from driver!");
	}

	connection::~connection() = default;
	
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
