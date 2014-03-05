#include "postgres_connection.h"
#include "postgres_exception.h"
#include "user_password_authentication.h"

#include <vector>

namespace
{
    void add_to_constr(std::string &connstr, const std::string &key, const std::string &value);
}

namespace cpp_db
{
    postgres_connection::postgres_connection()
    {
    }

    postgres_connection::~postgres_connection()
    {
        try
        {
            db.reset();
        }
        catch(...)
        {
        }
    }

    void postgres_connection::open(const std::string &database, const authentication &auth, const key_value_pair & options)
	{
		if (is_open())
            throw postgres_exception("Database already open");

        std::string conn_str;

        add_to_constr(conn_str, "dbname", database);

        if (const user_password_authentication * upauth = dynamic_cast<const user_password_authentication *>(&auth))
        {
            add_to_constr(conn_str, "user", upauth->user_name());
            add_to_constr(conn_str, "password", upauth->password());
        }

        for(auto it: options)
            add_to_constr(conn_str, it.first, it.second);

        db.reset(PQconnectdb(conn_str.c_str()), [](PGconn *conn_ptr) { PQfinish(conn_ptr); });   // should be called, even in case of failure to connect

        if (PQstatus(db.get()) != CONNECTION_OK)
            throw postgres_exception(PQerrorMessage(db.get()));
	}

    void postgres_connection::close()
	{
        db.reset();
	}

    bool postgres_connection::is_open() const
	{
        return db != nullptr;
    }

    handle postgres_connection::get_handle() const
	{
        return std::static_pointer_cast<void>(db);
    }

    void postgres_connection::set_current_transaction(const shared_transaction_ptr &trans)
    {
        current_transaction = trans;
    }

    shared_transaction_ptr postgres_connection::get_current_transaction() const
    {
        return current_transaction.lock();
    }
}

namespace
{

void add_to_constr(std::string &connstr, const std::string &key, const std::string &value)
{
    if (!connstr.empty())
        connstr+=" ";
    connstr+=key;
    connstr+="='";
    connstr+=value;
    connstr+="'";
}

}
