#ifndef TOOLS_NULLABLE_TYPES_H
#define TOOLS_NULLABLE_TYPES_H

#include "nullable.h"

#include <string>

namespace tools
{
    // char types
    using nullable_char = nullable<char>;
    using nullable_uchar = nullable<unsigned char>;
    using nullable_schar = nullable<signed char>;
    using nullable_wchar_t = nullable<wchar_t>;

    // signed integer types
    using nullable_short = nullable<short>;
    using nullable_int = nullable<int>;
    using nullable_long = nullable<long>;
    using nullable_long_long = nullable<long long>;

    // unsigned integer types
    using nullable_ushort = nullable<unsigned short>;
    using nullable_uint = nullable<unsigned int>;
    using nullable_ulong = nullable<unsigned long>;
    using nullable_ulong_long = nullable<unsigned long long>;

    // floating point types
    using nullable_float = nullable<float>;
    using nullable_double = nullable<double>;
    using nullable_long_double = nullable<long double>;

    // string types
    using nullable_string = nullable<std::string>;
    using nullable_wstring = nullable<std::wstring>;

    // date/time types
}

#endif // TOOLS_NULLABLE_TYPES_H
