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
        std::shared_ptr<sqlite3> db;

        ~impl()
        {
            try
            {
                db.reset();
            }
            catch(...)
            {
            }
        }

        void open(const std::string &database)
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

        void close()
        {
            db.reset();
        }

        bool is_open() const
        {
            return db != nullptr;
        }
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
        return std::static_pointer_cast<void>(pimpl->db);
    }

}
