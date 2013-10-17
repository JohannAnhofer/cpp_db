#include "transaction.h"
#include "connection.h"
#include "driver.h"

namespace cpp_db
{

transaction::transaction(const connection &conn)
	: ptransaction(conn.get_driver()->make_transaction(conn.get_handle()))
{
}

transaction::~transaction()
{
}

void transaction::begin()
{
	ptransaction->begin();
}

void transaction::commit()
{
	ptransaction->commit();
}

void transaction::rollback()
{
	ptransaction->rollback();
}

bool transaction::is_open() const
{
	return ptransaction->is_open();
}

}
