#ifndef CPP_DB_NULLABLE_TYPES_H
#define CPP_DB_NULLABLE_TYPES_H

#include "nullable.h"

#include <string>

namespace cpp_db
{
    // char types
    typedef nullable<char>          nullable_char;
    typedef nullable<unsigned char> nullable_uchar;
    typedef nullable<signed char>   nullable_schar;
    typedef nullable<wchar_t>       nullable_wchar_t;

    // signed integer types
    typedef nullable<short>     nullable_short;
    typedef nullable<int>       nullable_int;
    typedef nullable<long>      nullable_long;
    typedef nullable<long long> nullable_long_long;

    // unsigned integer types
    typedef nullable<unsigned short>     nullable_ushort;
    typedef nullable<unsigned int>       nullable_uint;
    typedef nullable<unsigned long>      nullable_ulong;
    typedef nullable<unsigned long long> nullable_ulong_long;

    // floating point types
    typedef nullable<float>         nullable_float;
    typedef nullable<double>        nullable_double;
    typedef nullable<long double>   nullable_long_double;

    // string types
    typedef nullable<std::string>   nullable_string;
    typedef nullable<std::wstring>  nullable_wstring;

    // date/time types
}

#endif // CPP_DB_NULLABLE_TYPES_H
