#include "transaction.h"
#include "transaction_interface.h"
#include "connection.h"
#include "driver.h"

#include <stdexcept>

namespace cpp_db
{

transaction::transaction(const connection &conn)
	: trans_impl(conn.get_driver()->make_transaction(conn.get_handle()))
    , driver_impl(conn.get_driver())
{
	if (!trans_impl)
		throw std::runtime_error("No transaction object from driver!");
}

transaction::~transaction()
{
    rollback();
}

void transaction::begin()
{
	trans_impl->begin();
    driver_impl.lock()->set_current_transaction(trans_impl->get_handle());
}

void transaction::commit()
{
	trans_impl->commit();
    driver_impl.lock()->set_current_transaction(0);
}

void transaction::rollback()
{
	trans_impl->rollback();
    driver_impl.lock()->set_current_transaction(0);
}

bool transaction::is_open() const
{
	return trans_impl->is_open();
}

}
