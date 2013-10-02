#include "connection.h"
#include "db_exception.h"

#include "sqlite3.h"

#include <string>
#include <sstream>

namespace cpp_db
{

    void throw_db_exception(int error_code, sqlite3 *db)
    {
        std::stringstream sstr;
        sstr << sqlite3_errmsg(db) << " (" << error_code << ")";
        throw db_exception(sstr.str());
    }

	struct connection::impl
	{
		impl() 
			: db(nullptr)
		{
		}

        ~impl()
        {
            if (db)
                sqlite3_close(db);
        }

        void open(const std::string &database)
        {
            close();

            if (int error_code = sqlite3_open_v2(database.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr))
                throw_db_exception(error_code, db);
        }

        void close()
        {
            if (db)
            {
                if (int error_code = sqlite3_close(db))
                    throw_db_exception(error_code, db);
                else
                    db = nullptr;
            }
        }

        bool is_open() const
        {
            return db != nullptr;
        }

        void *get_handle() const
        {
            if (db)
                return db;
            else
                throw std::runtime_error("database not open");
        }

		sqlite3 *db;
	};

    connection::connection(const std::string &drivername)
		: pimpl(new impl)
    {
        if (drivername != "sqlite")
            throw db_exception("Only sqlite supported for now!");
	}

	connection::~connection()
	{
	}

	void connection::open(const std::string &database)
	{
        pimpl->open(database);
	}

	void connection::close()
	{
        pimpl->close();
    }

    bool connection::is_open() const
    {
        return pimpl->is_open();
    }

    connection::handle connection::get_handle() const
    {
        return pimpl->get_handle();
    }

}
