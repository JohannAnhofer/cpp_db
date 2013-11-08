#ifndef CPP_DB_FIREBIRD_CONNECTION_H
#define CPP_DB_FIREBIRD_CONNECTION_H

#include "connection_interface.h"

#include "ibase.h"

#include <memory>

namespace cpp_db
{
    class firebird_driver;

	class firebird_connection : public connection_interface
	{
	public:
		~firebird_connection();

		void open(const std::string &database, const authentication &auth = no_authentication{}, const key_value_pair & = key_value_pair{}) override;
		void close() override;
		bool is_open() const override;
		handle get_handle() const override;
        void set_current_transaction(const shared_transaction_ptr &trans) override;
        shared_transaction_ptr get_current_transaction() const override;

	private:
        firebird_connection();
        friend class firebird_driver;
        std::shared_ptr<isc_db_handle> db;
        std::weak_ptr<transaction_interface> current_transaction;
    };

}

#endif
