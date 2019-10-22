#include "db_exception.h"
#include "odbc_connection.h"
#include "odbc_statement.h"
#include "odbc_handle.h"

#include <sqlext.h>

namespace cpp_db
{
odbc_stmt::odbc_stmt() 
	: _hstmt(nullptr)
{
}

odbc_stmt::~odbc_stmt()
{
	close();
}

void odbc_stmt::close()
{
	if (_hstmt)
	{
		::SQLFreeHandle(SQL_HANDLE_STMT, _hstmt);
	}

	_hstmt = nullptr;
}

odbc_statement::odbc_statement(const shared_connection_ptr &connection) 
	: _prepared(false)
	, conn_impl(connection)
{
}

odbc_statement::~odbc_statement() = default;

void odbc_statement::prepare(const std::string &sqlcmd)
{
	SQLRETURN ret_ = SQL_SUCCESS;
	shared_connection_ptr ptr_ = conn_impl.lock();
	handle handle_ = ptr_->get_handle();
	odbc_handle *db_ = static_cast<odbc_handle *>(handle_.get());

	_stmt.reset(new odbc_stmt);
	ret_ = ::SQLAllocHandle(SQL_HANDLE_STMT, db_->_hdbc, &_stmt->_hstmt);
	ret_ = ::SQLPrepareA(_stmt->_hstmt, const_cast<unsigned char *>
		(reinterpret_cast<const unsigned char *>(sqlcmd.c_str())),
        static_cast<SQLINTEGER>(sqlcmd.length()));

	if (ret_ != SQL_SUCCESS && ret_ != SQL_SUCCESS_WITH_INFO)
	{
		throw db_exception("SQLPrepare() failed in odbc_statement::prepare().");
	}

	_prepared = true;
}

bool odbc_statement::is_prepared() const
{
    return _prepared;
}

void odbc_statement::execute_ddl()
{
	SQLRETURN ret_ = ::SQLExecute(_stmt->_hstmt);

	if (ret_ != SQL_SUCCESS && ret_ != SQL_SUCCESS_WITH_INFO)
	{
		throw db_exception("SQLExecute() failed in odbc_statement::execute_ddl().");
	}
}

std::string error_msg(const SQLSMALLINT handle_type_,
	const SQLHANDLE handle_)
{
	std::string error_;
	SQLINTEGER num_records_ = 0;
	SQLRETURN ret_ = ::SQLGetDiagField(handle_type_, handle_, 0,
        SQL_DIAG_NUMBER, &num_records_, SQL_IS_INTEGER, nullptr);
	SQLCHAR buffer_[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLCHAR sqlstate_[SQL_SQLSTATE_SIZE + 1] = { 0 };
	SQLINTEGER sqlcode_ = 0;
	SQLSMALLINT length_ = 0;

	for (SQLSMALLINT i_ = 1; i_ <= num_records_; ++i_)
	{
		ret_ = ::SQLGetDiagRecA(handle_type_, handle_, i_, sqlstate_,
			&sqlcode_, buffer_, SQL_MAX_MESSAGE_LENGTH, &length_);

/*		if (reinterpret_cast<const char *>(sqlstate_) == std::string("40001"))
		{
			throw db_deadlock(reinterpret_cast<const char *>(buffer_));
		}*/

		if (error_.length()) error_ += "\r\n";

		error_ += reinterpret_cast<const char *>(buffer_);
	}

	return error_;
}

void odbc_statement::execute_non_query()
{
	SQLRETURN ret_ = ::SQLExecute(_stmt->_hstmt);

	if (ret_ != SQL_SUCCESS && ret_ != SQL_SUCCESS_WITH_INFO)
	{
		throw db_exception(("SQLExecute() failed in odbc_statement::execute_non_query(). " +
			error_msg(SQL_HANDLE_STMT, _stmt->_hstmt)).c_str());
	}
}

void odbc_statement::execute()
{
}

void odbc_statement::reset()
{
	_stmt->_params_in.clear();
	::SQLFreeStmt(_stmt->_hstmt, SQL_UNBIND);
}

handle odbc_statement::get_handle() const
{
	return std::static_pointer_cast<void>(_stmt);
}

} // namespace cpp_db
