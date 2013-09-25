#include "connection.h"
#include "db_exception.h"

#include "sqlite3.h"

#include <string>
#include <sstream>

namespace cpp_db
{

    void throw_db_exception(int error_code, sqlite3 *db)
    {
        std::stringstream sstr;
        sstr << sqlite3_errmsg(db) << " (" << error_code << ")";
        throw db_exception(sstr.str());
    }

	struct connection::impl
	{
		impl() 
			: db(nullptr)
		{
		}
		sqlite3 *db;
	};

    connection::connection(const std::string &drivername)
		: pimpl(new impl)
    {
        if (drivername != "sqlite")
            throw db_exception("Only sqlite supported for now!");
	}

	connection::~connection()
	{
		if (pimpl->db)
			sqlite3_close(pimpl->db);
	}

	void connection::open(const std::string &database)
	{
		if (pimpl->db)
			close();

        if (int error_code = sqlite3_open_v2(database.c_str(), &pimpl->db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr))
            throw_db_exception(error_code, pimpl->db);
	}

	void connection::close()
	{
        if (int error_code = sqlite3_close(pimpl->db))
            throw_db_exception(error_code, pimpl->db);
		else
            pimpl->db = nullptr;
    }

    bool connection::is_open() const
    {
        return pimpl->db != nullptr;
    }

    connection::handle connection::get_handle() const
    {
        if (pimpl->db)
            return pimpl->db;
        else
            throw std::runtime_error("database not open");
    }

}
