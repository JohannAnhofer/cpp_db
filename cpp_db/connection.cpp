#include "connection.h"
#include "db_exception.h"

#include "sqlite3.h"

#include <string>

namespace cpp_db
{

	struct connection::impl
	{
		impl() 
			: db(nullptr)
		{

		}
		sqlite3 *db;
	};

	connection::connection()
		: pimpl(new impl)
	{		
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

        if (sqlite3_open(database.c_str(), &pimpl->db))
			throw db_exception(sqlite3_errmsg(pimpl->db));
	}

	void connection::close()
	{
        if (sqlite3_close(pimpl->db))
			throw db_exception(sqlite3_errmsg(pimpl->db));
		else
			pimpl->db = nullptr;       
	}

}
