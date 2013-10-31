#ifndef CPP_DB_FIREBIRD_CONNECTION_H
#define CPP_DB_FIREBIRD_CONNECTION_H

#include "connection_interface.h"

#include "ibase.h"

#include <memory>

namespace cpp_db
{

	class firebird_connection : public connection_interface
	{
	public:
        firebird_connection();
        ~firebird_connection();

        void open(const std::string &database, const key_value_pair & = key_value_pair{}) override;
		void close() override;
		bool is_open() const override;
		handle get_handle() const override;

	private:
		std::shared_ptr<void> db;
        isc_db_handle db_handle;
    };

}

#endif
