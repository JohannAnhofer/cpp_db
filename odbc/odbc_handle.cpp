#include "odbc_handle.h"

namespace cpp_db
{

odbc_handle::odbc_handle() = default;

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

}
