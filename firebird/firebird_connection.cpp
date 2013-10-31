#include "firebird_connection.h"
#include "db_exception.h"

#include "ibase.h"

#include <stdexcept>
#include <sstream>
#include <vector>
#include <cstdint>
#include <iterator>

namespace cpp_db
{
    static const char option_user_name[] = "user";
    static const char option_password[] = "password";
    static const char option_encoding[] = "encoding";
    static const char option_role[] = "role";

    static inline bool has_error(ISC_STATUS status[20])
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

    static void add_option_to_dpb(const std::string &option_name, ISC_SCHAR dpb_otpion, const key_value_pair& options, std::vector<ISC_SCHAR> &params, const std::string &default_value = std::string{})
    {
        auto pos = options.find(option_name);
        if (pos != std::end(options) || !default_value.empty())
        {
            const auto &value = (pos != std::end(options) && !pos->second.empty()) ? pos->second : default_value;
            uint8_t len = value.size() > 255 ? 255 : value.size();

            params.push_back(dpb_otpion);
            params.push_back(len);
            auto end_it = std::begin(value);
            std::advance(end_it, len);
            std::copy(std::begin(value), end_it, std::back_inserter(params));
        }
    }
    
    firebird_connection::firebird_connection()
        : db_handle{0}
    {
    }

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

    void firebird_connection::open(const std::string &database, const key_value_pair & options)
	{
		if (is_open())
			throw std::runtime_error("Database already open");

		std::vector<ISC_SCHAR> params;

		params.push_back(isc_dpb_version1);

        add_option_to_dpb(option_user_name, isc_dpb_user_name,     options, params);
        add_option_to_dpb(option_password,  isc_dpb_password,      options, params);
        add_option_to_dpb(option_encoding,  isc_dpb_lc_ctype,      options, params, "UNICODE_FSS");
        add_option_to_dpb(option_role,      isc_dpb_sql_role_name, options, params);

		ISC_STATUS status[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		isc_attach_database(status, database.length(), database.c_str(), &db_handle, params.size(), params.data());
		if (has_error(status))
			throw_firebird_exception(status);

        db.reset(&db_handle, [&](isc_db_handle *db)
			{
				ISC_STATUS status[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
                isc_detach_database(status, db);
                db_handle = 0;
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
