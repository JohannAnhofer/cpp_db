#ifndef TOOLS_NULL_H
#define TOOLS_NULL_H

#include "cpp11_defines.h"

namespace tools
{

struct null_type {};

inline CONSTEXPR bool is_null(null_type)
{
    return true;
}

template<typename T>
CONSTEXPR bool is_null(T &&)
{
    return false;
}

}

#endif // TOOLS_NULL_H
