#ifndef CPP_DB_POSTGRES_CONNECTION_H
#define CPP_DB_POSTGRES_CONNECTION_H

#include "connection_interface.h"

#include <libpq-fe.h>

#include <memory>

namespace cpp_db
{
    class postgres_driver;

    class postgres_connection : public connection_interface
	{
	public:
        ~postgres_connection() override;

		void open(const std::string &database, const authentication &auth = no_authentication{}, const key_value_pair & = key_value_pair{}) override;
		void close() override;
		bool is_open() const override;
		handle get_handle() const override;
        void set_current_transaction(const shared_transaction_ptr &trans) override;
        shared_transaction_ptr get_current_transaction() const override;

	private:
        postgres_connection();
        friend class postgres_driver;
        std::shared_ptr<PGconn> db;
        std::weak_ptr<transaction_interface> current_transaction;
    };

}

#endif
