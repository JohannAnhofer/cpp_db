#ifndef CPP_DB_DB_EXCEPTION_H
#define CPP_DB_DB_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace cpp_db
{

class db_exception : public std::runtime_error
{
public:
    explicit db_exception(const std::string &message)
		: std::runtime_error(message)
	{
	}
};

}

#endif
