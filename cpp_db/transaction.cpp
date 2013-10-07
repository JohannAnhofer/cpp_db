#include "transaction.h"
#include "connection.h"
#include "db_exception.h"

#include "sqlite3.h"

namespace cpp_db
{

struct transaction::impl
{
	std::weak_ptr<sqlite3> db;

	impl(const connection &con)
		: db(std::static_pointer_cast<sqlite3>(con.get_handle()))
	{
		if (db.expired())
			throw db_exception("No database connection!");
	}

	void begin()
	{
		throw std::runtime_error("Not implemented!");
	}
	void commit()
	{
		throw std::runtime_error("Not implemented!");
	}
	void rollback()
	{
		throw std::runtime_error("Not implemented!");
	}

	bool is_open() const
	{
		return false;
	}
};

transaction::transaction(const connection &conn)
	: pimpl(new impl(conn))
{
}

void transaction::begin()
{
	pimpl->begin();
}

void transaction::commit()
{
	pimpl->commit();
}

void transaction::rollback()
{
	pimpl->rollback();
}

bool transaction::is_open() const
{
	return pimpl->is_open();
}

}
