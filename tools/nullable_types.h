#pragma once

#include <string>
#include <optional>

namespace tools
{	
    // char types
    using nullable_char = std::optional<char>;
    using nullable_uchar = std::optional<unsigned char>;
    using nullable_schar = std::optional<signed char>;
    using nullable_wchar_t = std::optional<wchar_t>;

    // signed integer types
    using nullable_short = std::optional<short>;
    using nullable_int = std::optional<int>;
    using nullable_long = std::optional<long>;
    using nullable_long_long = std::optional<long long>;

    // unsigned integer types
    using nullable_ushort = std::optional<unsigned short>;
    using nullable_uint = std::optional<unsigned int>;
    using nullable_ulong = std::optional<unsigned long>;
    using nullable_ulong_long = std::optional<unsigned long long>;

    // floating point types
    using nullable_float = std::optional<float>;
    using nullable_double = std::optional<double>;
    using nullable_long_double = std::optional<long double>;

    // string types
    using nullable_string = std::optional<std::string>;
    using nullable_wstring = std::optional<std::wstring>;

    // date/time types
}
