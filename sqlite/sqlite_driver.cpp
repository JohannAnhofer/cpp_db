#include "sqlite_driver.h"
#include "db_exception.h"
#include "parameter.h"
#include "null.h"
#include "value.h"
#include "usings.h"

#include "connection_interface.h"
#include "statement_interface.h"
#include "parameters_interface.h"
#include "result_interface.h"
#include "transaction_interface.h"

#include "sqlite3.h"

#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <cstring>
#include <unordered_map>

namespace cpp_db
{
	void throw_db_exception(int error_code, sqlite3 *db)
	{
		std::stringstream sstr;
		sstr << sqlite3_errmsg(db) << " (" << error_code << ")";
		throw db_exception(sstr.str());
	}

	struct sqlite_connection : public connection_interface
	{
		std::shared_ptr<sqlite3> db;

		~sqlite_connection()
		{
			try
			{
				db.reset();
			}
			catch (...)
			{
			}
		}

		void open(const std::string &database) override
		{
			if (is_open())
				throw std::runtime_error("Database already open");

			sqlite3 *dbptr(nullptr);
			if (int error_code = sqlite3_open_v2(database.c_str(), &dbptr, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr))
				throw_db_exception(error_code, dbptr);
			db.reset(dbptr, [](sqlite3 *db)
			{
				if (int error_code = sqlite3_close(db))
					throw_db_exception(error_code, db);
			}
			);
		}

		void close() override
		{
			db.reset();
		}

		bool is_open() const override
		{
			return db != nullptr;
		}

		handle get_handle() const override
		{
			return std::static_pointer_cast<void>(db);
		}
	};

	struct sqlite_statement  : public statement_interface
	{
		std::shared_ptr<sqlite3_stmt> stmt;
		std::weak_ptr<sqlite3> db;
		const char *tail;

		sqlite_statement(const handle &connection)
			: db(std::static_pointer_cast<sqlite3>(connection))
			, tail(nullptr)
		{
			if (db.expired())
				throw db_exception("No database connection for statement!");
		}

		void prepare(const std::string &sqlcmd)
		{
			sqlite3_stmt *stmt_new = nullptr;
			const char *tail_new = nullptr;

			int error_code = sqlite3_prepare_v2(db.lock().get(), sqlcmd.c_str(), sqlcmd.size(), &stmt_new, &tail_new);
			if (error_code != SQLITE_OK)
				throw_db_exception(error_code, db.lock().get());

			stmt.reset(stmt_new, sqlite3_finalize);
			tail = tail_new;
		}

		bool is_prepared() const
		{
			return stmt != nullptr;
		}

		void execute()
		{
			if (!is_prepared())
				throw db_exception("Statement not prepared!");

			if (int error_code = sqlite3_step(stmt.get()))
			{
				if (error_code != SQLITE_DONE && error_code != SQLITE_ROW)
					throw_db_exception(error_code, db.lock().get());
			}
		}

		void reset()
		{
			sqlite3_reset(stmt.get());
		}

		handle get_handle() const
		{
			return std::static_pointer_cast<void>(stmt);
		}
	};

	struct sqlite_transaction : public transaction_interface
	{
		std::weak_ptr<sqlite3> db;
		int open_count;

		sqlite_transaction(const handle &conn_handle)
			: db(std::static_pointer_cast<sqlite3>(conn_handle))
			, open_count(0)
		{
			if (db.expired())
				throw db_exception("No database connection!");
		}

		~sqlite_transaction()
		{
			try
			{
				while (is_open())
					rollback();
			}
			catch (...)
			{
			}
		}

		void execute(const char *sql)
		{
			const char *tail = nullptr;
			sqlite3_stmt *stmt = 0;
			int status = sqlite3_prepare(db.lock().get(), sql, strlen(sql), &stmt, &tail);
			if (status != SQLITE_OK && status != SQLITE_DONE)
				throw_db_exception(status, db.lock().get());
			auto deleter = [](sqlite3_stmt*s){sqlite3_finalize(s); };
			std::unique_ptr<sqlite3_stmt, decltype(deleter)> pstmt(stmt, deleter);
			status = sqlite3_step(stmt);
			if (status != SQLITE_OK && status != SQLITE_DONE)
				throw_db_exception(status, db.lock().get());
		}

		void begin() override
		{
			execute("BEGIN TRANSACTION");
			++open_count;
		}

		void commit() override
		{
			if (is_open())
			{
				execute("COMMIT TRANSACTION");
				--open_count;
			}
		}

		void rollback() override
		{
			if (is_open())
			{
				execute("ROLLBACK TRANSACTION");
				--open_count;
			}
		}

		bool is_open() const override
		{
			return open_count > 0;
		}
	};

	struct sqlite_parameters : public parameters_interface
	{
		std::weak_ptr<sqlite3_stmt> stmt;

		template<typename ElementType>
		static void delete_array(void *array)
		{
			delete[] reinterpret_cast<ElementType *>(array);
		}

		template<typename ObjectType>
		static void delete_object(void *object)
		{
			delete reinterpret_cast<ObjectType *>(object);
		}

		explicit sqlite_parameters(const handle &stmt)
			: stmt(std::static_pointer_cast<sqlite3_stmt>(stmt))
		{
		}

		int get_count() const override
		{
			return sqlite3_bind_parameter_count(stmt.lock().get());
		}

		int find_param_pos(const std::string &name) const
		{
			if (int pos = sqlite3_bind_parameter_index(stmt.lock().get(), name.c_str()))
				return pos;
			else
				throw db_exception("Index for SQL parameter '" + name + "' not found!");
		}

		void bind(const parameter &param) override
		{
			int index = param.has_index() ? param.get_index() : find_param_pos(param.get_name());
			if (param.has_value_of_type<int>())
				sqlite3_bind_int(stmt.lock().get(), index, param.get_value<int>());
			else if (param.has_value_of_type<double>())
				sqlite3_bind_double(stmt.lock().get(), index, param.get_value<double>());
			else if (param.has_value_of_type<const char *>())
			{
				const char *source = param.get_value<const char *>();
				char * value = new char[strlen(source) + 1];
				memcpy(value, source, strlen(source) + 1);
				sqlite3_bind_text(stmt.lock().get(), index, value, strlen(source), delete_array<char>);
			}
			else if (param.has_value_of_type<std::string>())
			{
				std::string source(param.get_value<std::string>());
				char * value = new char[source.length() + 1];
				memcpy(value, source.c_str(), source.length() + 1);
				sqlite3_bind_text(stmt.lock().get(), index, value, source.length(), delete_array<char>);
			}
			else if (param.has_value_of_type<blob>())
			{
				blob source(param.get_value<blob>());
				uint8_t *value = new uint8_t[source.size()];
				memcpy(value, source.data(), source.size());
				sqlite3_bind_blob(stmt.lock().get(), index, value, source.size(), delete_array<uint8_t>);
			}
			else if (param.has_value_of_type<int64_t>())
				sqlite3_bind_int64(stmt.lock().get(), index, param.get_value<int64_t>());
			else if (param.has_value_of_type<null_type>())
				sqlite3_bind_null(stmt.lock().get(), index);
		}

	};

	struct sqlite_result : public result_interface
	{
		std::shared_ptr<sqlite3_stmt> stmt;
		int row_status;
		std::unordered_map<std::string, int> column_names;

		sqlite_result(const std::shared_ptr<void> &stmt_handle)
			: stmt(std::static_pointer_cast<sqlite3_stmt>(stmt_handle))
			, row_status(SQLITE_DONE)
		{
			move_next();
			for (int column = 0; column < get_column_count(); ++column)
				column_names.emplace(sqlite3_column_name(stmt.get(), column), column);
		}

		void move_next() override
		{
			row_status = sqlite3_step(stmt.get());
			if (row_status != SQLITE_DONE && row_status != SQLITE_ROW)
				throw_db_exception(row_status, sqlite3_db_handle(stmt.get()));
		}

		void move_prev() override
		{
			throw db_exception("Not supported!");
		}

		void move_first() override
		{
			if (int error_code = sqlite3_reset(stmt.get()))
				throw_db_exception(error_code, sqlite3_db_handle(stmt.get()));
			move_next();
		}

		bool is_eof() const override
		{
			return row_status == SQLITE_DONE;
		}

		int get_column_count() const override
		{
			return sqlite3_column_count(stmt.get());
		}

		std::string get_column_name(int column) const override
		{
			if (const char *column_name = sqlite3_column_name(stmt.get(), column))
				return column_name;
			else
			{
				std::stringstream message;
				message << "Column " << column << " not found!";
				throw db_exception(message.str());
			}
		}

		value get_column_value(int column) const override
		{
			sqlite3_stmt *pstmt = stmt.get();

			switch (sqlite3_column_type(pstmt, column))
			{
			case SQLITE_INTEGER:	// int64
				return sqlite3_column_int64(pstmt, column);
			case SQLITE_FLOAT:		// double
				return sqlite3_column_double(pstmt, column);
			case SQLITE_BLOB:		// void *
			{
										const uint8_t *data = static_cast<const uint8_t*>(sqlite3_column_blob(pstmt, column));
										return blob(data, data + sqlite3_column_bytes(pstmt, column));
			}
			case SQLITE_TEXT:		// const char *
				return std::string(reinterpret_cast<const char *>(sqlite3_column_text(pstmt, column)), sqlite3_column_bytes(pstmt, column));
			case SQLITE_NULL:		// 
			default:
				break;
			}
			return null_type();
		}

		int get_column_index(const std::string &column_name) const override
		{
			auto pos = column_names.find(column_name);
			if (pos != column_names.end())
				return pos->second;
			std::stringstream message;
			message << "Column '" << column_name << "' not found!";
			throw db_exception(message.str());
		}

		value get_column_value(const std::string &column_name) const override
		{
			return get_column_value(get_column_index(column_name));
		}
	};

	//  sqlite_driver

	sqlite_driver::sqlite_driver()
	{
	}


	sqlite_driver::~sqlite_driver()
	{
	}

	connection_interface *sqlite_driver::make_connection() const 
	{
		return new sqlite_connection;
	}

	statement_interface *sqlite_driver::make_statement(const handle &conn_handle) const 
	{
		return new sqlite_statement(conn_handle);
	}

	parameters_interface *sqlite_driver::make_parameters(const handle &stmt_handle) const
	{
		return new sqlite_parameters(stmt_handle);
	}

	result_interface *sqlite_driver::make_result(const handle &stmt_handle) const
	{
		return new sqlite_result(stmt_handle);
	}

	transaction_interface *sqlite_driver::make_transaction(const handle &conn_handle) const
	{
		return new sqlite_transaction(conn_handle);
	}

}
