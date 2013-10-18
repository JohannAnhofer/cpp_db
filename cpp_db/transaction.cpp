#include "transaction.h"
#include "transaction_interface.h"
#include "connection.h"
#include "driver.h"

#include <stdexcept>

namespace cpp_db
{

transaction::transaction(const connection &conn)
	: trans_impl(conn.get_driver()->make_transaction(conn.get_handle()))
{
	if (!trans_impl)
		throw std::runtime_error("No transaction object from driver!");
}

transaction::~transaction()
{
}

void transaction::begin()
{
	trans_impl->begin();
}

void transaction::commit()
{
	trans_impl->commit();
}

void transaction::rollback()
{
	trans_impl->rollback();
}

bool transaction::is_open() const
{
	return trans_impl->is_open();
}

}
