#include "firebird_exception.h"
#include "isc_status.h"

#include "ibase.h"

#include <sstream>

namespace cpp_db
{

static std::string get_firebird_error_text(const ISC_STATUS *status)
{
    char message_buffer[512];
    ISC_LONG sqlcode = isc_sqlcode(status);
    isc_sql_interprete(static_cast<short>(sqlcode), message_buffer, sizeof(message_buffer) / sizeof(message_buffer[0]));

    std::stringstream msg;
    msg << message_buffer;

    while (fb_interpret(message_buffer, sizeof(message_buffer) / sizeof(message_buffer[0]), &status))
        msg << " - " << message_buffer;

    return msg.str();
}

firebird_exception::firebird_exception(const isc_status &status)
    : db_exception(get_firebird_error_text(static_cast<const ISC_STATUS *>(status)))
{
}

}
