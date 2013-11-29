#ifndef CPP_DB_ISC_STATUS_H
#define CPP_DB_ISC_STATUS_H

#include "firebird_exception.h"

#include "ibase.h"

#include <sstream>
#include <string>
#include <functional>
#include <array>

namespace cpp_db
{

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

    explicit operator const ISC_STATUS *() const
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
    if (throw_on_fail && status.has_error())
        throw cpp_db::firebird_exception(status);
    else
        status.dump_on_error();
}

}

#endif // CPP_DB_ISC_STATUS_H
