#include "firebird_connection.h"
#include "db_exception.h"
#include "user_password_authentication.h"
#include "isc_status.h"

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

    static void add_value_to_dpb(ISC_SCHAR name, const std::string &value, std::vector<ISC_SCHAR> &params);
    static void add_option_to_dpb(const std::string &option_name, ISC_SCHAR dpb_otpion, const key_value_pair& options, std::vector<ISC_SCHAR> &params, const std::string &default_value = std::string{});

    firebird_connection::firebird_connection()
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

	void firebird_connection::open(const std::string &database, const authentication &auth, const key_value_pair & options)
	{
		if (is_open())
            throw db_exception("Database already open");

		std::vector<ISC_SCHAR> params;

		params.push_back(isc_dpb_version1);

		if (const user_password_authentication * upauth = dynamic_cast<const user_password_authentication *>(&auth))
		{
			add_value_to_dpb(isc_dpb_user_name, upauth->user_name(), params);
			add_value_to_dpb(isc_dpb_password, upauth->password(), params);
		}
		else
		{
			add_option_to_dpb(option_user_name, isc_dpb_user_name, options, params);
			add_option_to_dpb(option_password, isc_dpb_password, options, params);
		}
        add_option_to_dpb(option_encoding,  isc_dpb_lc_ctype,      options, params, "UNICODE_FSS");
        add_option_to_dpb(option_role,      isc_dpb_sql_role_name, options, params);

        std::unique_ptr<isc_db_handle> db_handle(new isc_db_handle{ 0 });
        guarded_execute([&](ISC_STATUS *status)
            {
                isc_attach_database(status, static_cast<short>(database.length()), database.c_str(), db_handle.get(), static_cast<short>(params.size()), params.data());
            }, true);

        db.reset(db_handle.release(), [&](isc_db_handle *db)
			{
                guarded_execute([&db](ISC_STATUS *status)
                    {
                        isc_detach_database(status, db);
                        delete db;
                    }, true);
            });
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

    void firebird_connection::set_current_transaction(const shared_transaction_ptr &trans)
    {
        current_transaction = trans;
    }

    shared_transaction_ptr firebird_connection::get_current_transaction() const
    {
        return current_transaction.lock();
    }

    // non-members

    static void add_value_to_dpb(ISC_SCHAR name, const std::string &value, std::vector<ISC_SCHAR> &params)
    {
        if (uint8_t len = value.size() > 255 ? 255 : static_cast<uint8_t>(value.size()))
        {
            params.push_back(name);
            params.push_back(len);
            auto end_it = std::begin(value);
            std::advance(end_it, len);
            std::copy(std::begin(value), end_it, std::back_inserter(params));
        }
    }

    static void add_option_to_dpb(const std::string &option_name, ISC_SCHAR dpb_otpion, const key_value_pair& options, std::vector<ISC_SCHAR> &params, const std::string &default_value)
    {
        auto pos = options.find(option_name);
        if (pos != std::end(options) || !default_value.empty())
        {
            const auto &value = (pos != std::end(options) && !pos->second.empty()) ? pos->second : default_value;
            add_value_to_dpb(dpb_otpion, value, params);
        }
    }

}
