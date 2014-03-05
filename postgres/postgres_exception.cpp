#include "postgres_exception.h"

namespace cpp_db
{

postgres_exception::postgres_exception(const std::string &message)
    : db_exception(message)
{
}

}
