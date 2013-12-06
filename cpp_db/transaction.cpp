#include "transaction.h"
#include "transaction_interface.h"
#include "connection.h"
#include "connection_interface.h"
#include "driver_interface.h"
#include "lock_or_throw.h"

#include <stdexcept>

namespace cpp_db
{

transaction::transaction(const connection &conn)
    : conn_impl(conn.conn_impl)
    , trans_impl(conn.driver_impl->make_transaction(conn.conn_impl))
{
	if (!trans_impl)
		throw std::runtime_error("No transaction object from driver!");
}

#if defined(_MSC_VER) && (_MSC_FULL_VER <= 180021005)

transaction::transaction(transaction && other)
    : conn_impl(std::move(other.conn_impl))
    , trans_impl(std::move(other.trans_impl))
{
}

transaction &transaction::operator=(transaction &&other)
{
    if (this != &other)
    {
        conn_impl = std::move(other.conn_impl);
        trans_impl = std::move(other.trans_impl);
    }
    return *this;
}

#endif

transaction::~transaction()
{
    rollback();
}

void transaction::begin()
{
	trans_impl->begin();
    tools::lock_or_throw(conn_impl, "Invalid database connection")->set_current_transaction(trans_impl);
}

void transaction::commit()
{
	trans_impl->commit();
	tools::lock_or_throw(conn_impl, "Invalid database connection")->set_current_transaction(0);
}

void transaction::rollback()
{
	trans_impl->rollback();
	tools::lock_or_throw(conn_impl, "Invalid database connection")->set_current_transaction(0);
}

bool transaction::is_open() const
{
	return trans_impl->is_open();
}

handle transaction::get_handle() const
{
    return trans_impl->get_handle();
}

}
