#include "db_exception.h"
#include "odbc_connection.h"
#include <sqlext.h>

namespace cpp_db
{
odbc_handle::odbc_handle() :
	_henv(nullptr),
	_hdbc(nullptr)
{
}

odbc_handle::~odbc_handle()
{
	close();
}

void odbc_handle::close()
{
	SQLRETURN ret_ = 0;

	if (_hdbc)
	{
		ret_ = ::SQLDisconnect(_hdbc);
		ret_ = ::SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
		_hdbc = nullptr;
	}

	if (_henv)
	{
		ret_ = ::SQLFreeHandle(SQL_HANDLE_ENV, _henv);
		_henv = nullptr;
	}
}
	
odbc_connection::odbc_connection()
{
}

odbc_connection::~odbc_connection()
{
	close();
}

void odbc_connection::open(const std::string &database, const authentication &auth, const key_value_pair &options)
{
	SQLRETURN ret_ = SQL_SUCCESS;

	_handle.reset(new odbc_handle);

	ret_ = ::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_handle->_henv);

	if (ret_ != SQL_SUCCESS && ret_ != SQL_SUCCESS_WITH_INFO)
	{
		throw db_exception("Failed to open DB Environment");
	}

	// Not sure this is a good idea. We reverted back to ODBC 2
	// at work last week in order to be bug compatible with clients.
	ret_ = ::SQLSetEnvAttr(_handle->_henv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3, SQL_IS_UINTEGER);

	if (ret_ != SQL_SUCCESS && ret_ != SQL_SUCCESS_WITH_INFO)
	{
		throw db_exception("Failed to set ODBC version to 3.");
	}

	ret_ = ::SQLAllocHandle(SQL_HANDLE_DBC, _handle->_henv, &_handle->_hdbc);

	if (ret_ != SQL_SUCCESS && ret_ != SQL_SUCCESS_WITH_INFO)
	{
		throw db_exception("Failed to ger HDBC.");
	}

	ret_ = ::SQLConnectA(_handle->_hdbc,
		reinterpret_cast<unsigned char *>(const_cast<char *>(database.c_str())),
		SQL_NTS, nullptr, 0, nullptr, 0);

	if (ret_ != SQL_SUCCESS && ret_ != SQL_SUCCESS_WITH_INFO)
	{
		throw db_exception("Failed to open db.");
	}

	(void)auth;
	(void)options;
}

void odbc_connection::close()
{
	if (_handle)
	{
		_handle->close();
	}
}

bool odbc_connection::is_open() const
{
	return _handle && _handle->_hdbc != nullptr;
}

handle odbc_connection::get_handle() const
{
	return std::static_pointer_cast<void>(_handle);
}

void odbc_connection::set_current_transaction(const shared_transaction_ptr &trans)
{
    current_transaction = trans;
}

shared_transaction_ptr odbc_connection::get_current_transaction() const
{
    return current_transaction.lock();
}
} // namespace cpp_db
