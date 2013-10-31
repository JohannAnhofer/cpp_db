#include "firebird_connection.h"
#include "db_exception.h"

#include "ibase.h"

#include <stdexcept>
#include <sstream>
#include <vector>
#include <cstdint>
#include <algorithm>

namespace cpp_db
{
	firebird_connection::~firebird_connection()
	{
		try
		{
			db.reset();
		}
		catch (...)
		{
		}
	}

	inline bool has_error(ISC_STATUS status[20])
	{
		return status[0] == 1 && status[1] > 0;
	}

	void throw_firebird_exception(ISC_STATUS status[20])
	{
		char message_buffer[512];
		ISC_LONG sqlcode = isc_sqlcode(status);
		isc_sql_interprete(static_cast<short>(sqlcode), message_buffer, sizeof(message_buffer) / sizeof(message_buffer[0]));

		std::stringstream msg;
		msg << message_buffer;

		const ISC_STATUS *pvector = status;
		while (fb_interpret(message_buffer, sizeof(message_buffer) / sizeof(message_buffer[0]), &pvector))
			msg << " - " << message_buffer;

		throw cpp_db::db_exception(msg.str());
	}

    static const char option_user_name[] = "user";
    static const char option_password[] = "password";
    static const char option_encoding[] = "encoding";
    static const char option_role[] = "role";

    void firebird_connection::open(const std::string &database, const key_value_pair & options)
	{
		if (is_open())
			throw std::runtime_error("Database already open");

		std::vector<ISC_SCHAR> params;

		params.push_back(isc_dpb_version1);

        auto pos = options.find(option_user_name);
        if (pos != std::end(options))
        {
            const auto &username = pos->second;
            params.push_back(isc_dpb_user_name);
            params.push_back(std::min(username.size(), 255u));
            std::copy(std::begin(username), std::end(username), std::back_inserter(params));
        }

        pos = options.find(option_password);
        if (pos != std::end(options))
        {
            const auto &password = pos->second;
            params.push_back(isc_dpb_password);
            params.push_back(std::min(password.size(), 255u));
            std::copy(std::begin(password), std::end(password), std::back_inserter(params));
        }

        std::string encoding("UNICODE_FSS");
        pos = options.find(option_encoding);
        if (pos != std::end(options))
            encoding = pos->second;

        if (!encoding.empty())
        {
            params.push_back(isc_dpb_lc_ctype);
            params.push_back(std::min(encoding.size(), 255u));
            std::copy(std::begin(encoding), std::end(encoding), std::back_inserter(params));
        }

        pos = options.find(option_role);
        if (pos != std::end(options))
        {
            const auto &role = pos->second;
            params.push_back(isc_dpb_sql_role_name);
			params.push_back(std::min(role.size(), 255u));
			std::copy(std::begin(role), std::end(role), std::back_inserter(params));
		}

		ISC_STATUS status[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		isc_db_handle db_handle{ nullptr };
		isc_attach_database(status, database.length(), database.c_str(), &db_handle, params.size(), params.data());
		if (has_error(status))
			throw_firebird_exception(status);

		db.reset(db_handle, [](isc_db_handle db)
			{
				ISC_STATUS status[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				isc_detach_database(status, &db);
				if (has_error(status))
					throw_firebird_exception(status);
			}
		);
	}

	void firebird_connection::close()
	{
		db.reset();
	}

	bool firebird_connection::is_open() const
	{
		return db != nullptr;
	}

	handle firebird_connection::get_handle() const
	{
		return std::static_pointer_cast<void>(db);
	}

}
