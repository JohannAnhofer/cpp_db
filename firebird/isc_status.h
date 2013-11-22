#ifndef CPP_DB_ISC_STATUS_H
#define CPP_DB_ISC_STATUS_H

#include "db_exception.h"

#include "ibase.h"

#include <sstream>
#include <string>
#include <functional>
#include <array>

class isc_status
{
public:

	isc_status() 
#ifndef _MSC_VER
        : status{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}
#endif
	{
#ifdef _MSC_VER
		std::fill(std::begin(status), std::end(status), 0);
#endif
	}

    bool has_error() const
    {
        return has_error(status.data());
    }

    static bool has_error(const ISC_STATUS status[20])
    {
        return status[0] == 1 && status[1] > 0;
    }

    void throw_db_exception_on_error() const
    {
        if (has_error())
        {
            char message_buffer[512];
            ISC_LONG sqlcode = isc_sqlcode(status.data());
            isc_sql_interprete(static_cast<short>(sqlcode), message_buffer, sizeof(message_buffer) / sizeof(message_buffer[0]));

            std::stringstream msg;
            msg << message_buffer;

            const ISC_STATUS *pvector = status.data();
            while (fb_interpret(message_buffer, sizeof(message_buffer) / sizeof(message_buffer[0]), &pvector))
                msg << " - " << message_buffer;

            throw cpp_db::db_exception(msg.str());
        }
    }

    void dump_on_error() const
    {
        if (has_error())
        {
            isc_print_status(status.data());
            ISC_LONG sqlcode = isc_sqlcode(status.data());
            isc_print_sqlerror(static_cast<ISC_SHORT>(sqlcode), status.data());
        }
    }

    explicit operator ISC_STATUS *()
    {
        return status.data();
    }

private:
    std::array<ISC_STATUS, 20> status;
};

inline void guarded_execute(const std::function<void(ISC_STATUS*)> &function, bool throw_on_fail)
{
    isc_status status;
    function(static_cast<ISC_STATUS *>(status));
    if (throw_on_fail)
        status.throw_db_exception_on_error();
    else
        status.dump_on_error();
}

#endif // ISC_STATUS_H
