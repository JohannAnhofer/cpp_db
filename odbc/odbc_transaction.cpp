#include "db_exception.h"
#include "odbc_connection.h"
#include "odbc_transaction.h"
#include "odbc_handle.h"

#include <sqlext.h>
#include <sqltypes.h>

namespace cpp_db
{

odbc_transaction::odbc_transaction(const shared_connection_ptr &conn_handle) :
	_enabled(false),
	conn_impl(conn_handle)
{
}

odbc_transaction::~odbc_transaction()
{
	try
	{
		// rollback function checks if transaction is open!
		rollback();
	}
	catch (...)
	{
		// Destructors must not throw!
	}
}

void odbc_transaction::begin()
{
	if (!_enabled)
	{
		shared_connection_ptr ptr_ = conn_impl.lock();
		handle handle_ = ptr_->get_handle();
		odbc_handle *db_ = static_cast<odbc_handle *>(handle_.get());
		SQLINTEGER isolation_ = 0;
		SQLUINTEGER ac_ = SQL_AUTOCOMMIT_OFF;
		SQLRETURN ret_ = ::SQLGetConnectAttr(db_->_hdbc, SQL_ATTR_TXN_ISOLATION,
			&isolation_, SQL_IS_POINTER, 0);

		if (ret_ != SQL_SUCCESS && ret_ != SQL_SUCCESS_WITH_INFO)
		{
			throw db_exception("Failed to get SQL_ATTR_TXN_ISOLATION.");
		}

		if (isolation_ < SQL_TXN_READ_COMMITTED)
		{
			// On SQL Server the isolation level should automatically
			// be set to at least SQL_TXN_READ_COMMITTED.  For Sybase
			// it is set lower, which will break code relying on
			// transactions for locking rows with an update and then
			// reading values back from that row.
			// Once set, we leave the isolation level at level
			// SQL_TXN_READ_COMMITTED for the current connection, as
			// attempting to set it back again in commit or rollback
			// could introduce a race condition with other transaction
			// objects, which could undo the row lock guarantee!
			ret_ = ::SQLSetConnectAttr(db_->_hdbc, SQL_ATTR_TXN_ISOLATION,
				reinterpret_cast<SQLPOINTER>(SQL_TXN_READ_COMMITTED),
				SQL_IS_UINTEGER);

			if (ret_ != SQL_SUCCESS && ret_ != SQL_SUCCESS_WITH_INFO)
			{
				throw db_exception("Failed to set SQL_TXN_READ_COMMITTED.");
			}
		}

		ret_ = ::SQLSetConnectAttr(db_->_hdbc, SQL_ATTR_AUTOCOMMIT,
			reinterpret_cast<SQLPOINTER>(ac_), SQL_IS_UINTEGER);

		if (ret_ != SQL_SUCCESS && ret_ != SQL_SUCCESS_WITH_INFO)
		{
			throw db_exception("Failed to begin transaction.");
		}

		_enabled = true;
	}
}

void odbc_transaction::commit()
{
	if (_enabled)
	{
		shared_connection_ptr ptr_ = conn_impl.lock();
		handle handle_ = ptr_->get_handle();
		odbc_handle *db_ = static_cast<odbc_handle *>(handle_.get());
		SQLRETURN ret_ = ::SQLEndTran(SQL_HANDLE_DBC, db_->_hdbc, SQL_COMMIT);
		SQLUINTEGER ac_ = SQL_AUTOCOMMIT_ON;

		if (ret_ != SQL_SUCCESS && ret_ != SQL_SUCCESS_WITH_INFO)
		{
			throw db_exception("Failed to commit transaction.");
		}

		// Turn back on auto commit!
		ret_ = ::SQLSetConnectAttr(db_->_hdbc, SQL_ATTR_AUTOCOMMIT,
			reinterpret_cast<SQLPOINTER>(ac_), SQL_IS_UINTEGER);

		if (ret_ != SQL_SUCCESS && ret_ != SQL_SUCCESS_WITH_INFO)
		{
			throw db_exception("Failed to turn AUTOCOMMIT back on.");
		}

		_enabled = false;
	}
}

void odbc_transaction::rollback()
{
	if (_enabled)
	{
		shared_connection_ptr ptr_ = conn_impl.lock();
		handle handle_ = ptr_->get_handle();
		odbc_handle *db_ = static_cast<odbc_handle *>(handle_.get());
		SQLRETURN ret_ = ::SQLEndTran(SQL_HANDLE_DBC, db_->_hdbc, SQL_ROLLBACK);
		SQLUINTEGER ac_ = SQL_AUTOCOMMIT_ON;

		if (ret_ != SQL_SUCCESS && ret_ != SQL_SUCCESS_WITH_INFO)
		{
			throw db_exception("Failed to rollback transaction.");
		}

		// Turn back on auto commit!
		ret_ = ::SQLSetConnectAttr(db_->_hdbc, SQL_ATTR_AUTOCOMMIT,
			reinterpret_cast<SQLPOINTER>(ac_), SQL_IS_UINTEGER);

		if (ret_ != SQL_SUCCESS && ret_ != SQL_SUCCESS_WITH_INFO)
		{
			throw db_exception("Failed to turn AUTOCOMMIT back on.");
		}

		_enabled = false;
	}
}

bool odbc_transaction::is_open() const
{
	return _enabled;
}

handle odbc_transaction::get_handle() const
{
    return handle{};
}

} // namespace cpp_db
