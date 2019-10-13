#pragma once

#if defined(WIN32)
#include <windows.h>
#endif

#include <sql.h>

namespace cpp_db
{

struct odbc_handle
{
	SQLHANDLE _henv = nullptr;
	SQLHANDLE _hdbc = nullptr;

	odbc_handle(const odbc_handle& rhs_) = delete;
	odbc_handle& operator=(const odbc_handle& rhs) = delete;

	odbc_handle();
	~odbc_handle();
	void close();
};

}
