#ifndef CPP_DB_NULL_H
#define CPP_DB_NULL_H

#include "cpp11_defines.h"

namespace cpp_db
{

struct null_type {};

inline CONSTEXPR bool is_null(const null_type &)
{
    return true;
}

template<typename T>
CONSTEXPR bool is_null(const T &)
{
    return false;
}

}

#endif // CPP_DB_NULL_H
