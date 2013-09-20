#include "connection.h"
#include "db_exception.h"

#include "..\sqlite\sqlite3.h"

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

		if (int error_code = sqlite3_open(":memory", &pimpl->db))
			throw db_exception(sqlite3_errmsg(pimpl->db));
	}

	void connection::close()
	{
		if (int error_code = sqlite3_close(pimpl->db))
			throw db_exception(sqlite3_errmsg(pimpl->db));
		else
			pimpl->db = nullptr;
	}

}
