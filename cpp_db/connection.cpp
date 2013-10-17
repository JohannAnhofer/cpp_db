#include "connection.h"
#include "driver.h"

namespace cpp_db
{
    connection::connection(driver *sql_driver)
		: pdriver(sql_driver)
		, pconnection(sql_driver->make_connection())
    {
	}

	connection::~connection()
	{
	}

	void connection::open(const std::string &database)
	{
		pconnection->open(database);
	}

	void connection::close()
	{
		pconnection->close();
    }

    bool connection::is_open() const
    {
		return pconnection->is_open();
    }

    handle connection::get_handle() const
    {
		return pconnection->get_handle();
    }

	std::shared_ptr<driver> connection::get_driver() const
	{
		return pdriver;
	}
}
