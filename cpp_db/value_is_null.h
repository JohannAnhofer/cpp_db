#ifndef CPP_DB_VALUE_IS_NULL_H
#define CPP_DB_VALUE_IS_NULL_H

#include <stdexcept>

namespace cpp_db
{

class value_is_null : public std::logic_error
{
public:
    value_is_null()
        : std::logic_error("Value is null")
    {
    }
};

} // namespace cpp_db

#endif // CPP_DB_VALUE_IS_NULL_H
