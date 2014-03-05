#ifndef CPP_DB_POSTGRES_EXCEPTION_H
#define CPP_DB_POSTGRES_EXCEPTION_H

#include "db_exception.h"

namespace cpp_db
{

class postgres_exception : public db_exception
{
public:
    explicit postgres_exception(const std::string &message);
};

}

#endif // CPP_DB_POSTGRES_EXCEPTION_H
