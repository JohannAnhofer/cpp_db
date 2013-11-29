#ifndef CPP_DB_FIREBIRD_EXCEPTION_H
#define CPP_DB_FIREBIRD_EXCEPTION_H

#include "db_exception.h"

namespace cpp_db
{

class isc_status;

class firebird_exception : public db_exception
{
public:
    explicit firebird_exception(const isc_status &status);
};

}

#endif // CPP_DB_FIREBIRD_EXCEPTION_H
