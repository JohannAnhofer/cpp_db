#ifndef CPP_DB_VALUE_INL
#define CPP_DB_VALUE_INL

#include "value_is_null.h"

#include <cstdint>
#include <typeindex>
#include <typeinfo>
#include <sstream>

namespace cpp_db
{

template<typename T, typename U>
T integer_converter(U int_val)
{
    return static_cast<T>(int_val);
}

template<typename T, typename U>
T unsigned_converter(U uint_val)
{
    return static_cast<T>(uint_val);
}

template<typename T, typename U>
T floating_point_converter(U uint_val)
{
    return static_cast<T>(uint_val);
}

template<typename T>
T numeric_extractor(const value &val)
{
    std::type_index src_type{type_of(val)};

    if (!strcmp(src_type.name(), typeid(T).name()))
        return std::any_cast<T>(val);

    // signed integers
    if (src_type == typeid(int8_t))
        return integer_converter<T, int8_t>(value_of<int8_t>(val));
    if (src_type == typeid(int16_t))
        return integer_converter<T, int16_t>(value_of<int16_t>(val));
    if (src_type == typeid(int32_t))
        return integer_converter<T, int32_t>(value_of<int32_t>(val));
    if (src_type == typeid(int64_t))
        return integer_converter<T, int64_t>(value_of<int64_t>(val));

    // unsigned integers
    if (src_type == typeid(uint8_t))
        return unsigned_converter<T, uint8_t>(value_of<uint8_t>(val));
    if (src_type == typeid(uint16_t))
        return unsigned_converter<T, uint16_t>(value_of<uint16_t>(val));
    if (src_type == typeid(uint32_t))
        return unsigned_converter<T, uint32_t>(value_of<uint32_t>(val));
    if (src_type == typeid(uint64_t))
        return unsigned_converter<T, uint64_t>(value_of<uint64_t>(val));

    if (src_type == typeid(float))
        return floating_point_converter<T, float>(value_of<float>(val));
    if (src_type == typeid(double))
        return floating_point_converter<T, double>(value_of<double>(val));
    if (src_type == typeid(long double))
        return floating_point_converter<T, long double>(value_of<long double>(val));

    throw type_mismatch(src_type, typeid(T));
#ifdef _MSC_VER
	// no return here to suppress unreachable code message.
#else
	return 0;
#endif
}

// integers

template<>
inline int64_t value_of<int64_t>(const value &val)
{
    return numeric_extractor<int64_t>(val);
}

template<>
inline uint64_t value_of<uint64_t>(const value &val)
{
    return numeric_extractor<uint64_t>(val);
}

template<>
inline int32_t value_of<int32_t>(const value &val)
{
    return numeric_extractor<int32_t>(val);
}

template<>
inline uint32_t value_of<uint32_t>(const value &val)
{
    return numeric_extractor<uint32_t>(val);
}

template<>
inline int16_t value_of<int16_t>(const value &val)
{
    return numeric_extractor<int16_t>(val);
}

template<>
inline uint16_t value_of<uint16_t>(const value &val)
{
    return numeric_extractor<uint16_t>(val);
}

template<>
inline int8_t value_of<int8_t>(const value &val)
{
    return numeric_extractor<int8_t>(val);
}

template<>
inline uint8_t value_of<uint8_t>(const value &val)
{
    return numeric_extractor<uint8_t>(val);
}

// floating points

template<>
inline float value_of<float>(const value &val)
{
    return numeric_extractor<float>(val);
}

template<>
inline double value_of<double>(const value &val)
{
    return numeric_extractor<double>(val);
}

template<>
inline long double value_of<long double>(const value &val)
{
    return numeric_extractor<long double>(val);
}

// strings

template<>
inline std::string value_of<std::string>(const value &val)
{
    std::type_index src_type{type_of(val)};

    if (src_type == typeid(const char *))
        return value_of<const char *>(val);
    if (src_type == typeid(char *))
        return value_of<char *>(val);
    if (!strcmp(src_type.name(), typeid(std::string).name()))
        return std::any_cast<std::string>(val);

    throw type_mismatch(src_type, typeid(std::string));
#ifdef _MSC_VER
	// no return to suppress unreachable code warning
#else
	return std::string{};
#endif
}

template<>
inline std::wstring value_of<std::wstring>(const value &val)
{
    std::type_index src_type{type_of(val)};

    if (src_type == typeid(const wchar_t *))
        return value_of<const wchar_t *>(val);
    if (src_type == typeid(wchar_t *))
        return value_of<wchar_t *>(val);
    if (!strcmp(src_type.name(), typeid(std::wstring).name()))
        return std::any_cast<std::wstring>(val);

    throw type_mismatch(src_type, typeid(std::wstring));
#ifdef _MSC_VER
	// no return to suppress unreachable code warning
#else
	return std::wstring{};
#endif
}

template<>
inline std::string cast_to<std::string>(const value &val)
{
    std::type_index src_type{type_of(val)};

    if (src_type== typeid(int8_t)
            || src_type == typeid(int16_t)
            || src_type == typeid(int32_t)
            || src_type == typeid(int64_t))
    {
        std::stringstream result;
        result << value_of<int64_t>(val);
        return result.str();
    }

    if (src_type == typeid(uint8_t)
            || src_type == typeid(uint16_t)
            || src_type == typeid(uint32_t)
            || src_type == typeid(uint64_t))
    {
        std::stringstream result;
        result << value_of<uint64_t>(val);
        return result.str();
    }

    if (src_type == typeid(float)
            || src_type == typeid(double)
            || src_type == typeid(long double))
    {
        std::stringstream result;
        result << value_of<long double>(val);
        return result.str();
    }

    throw type_mismatch(src_type, typeid(std::string));
#ifdef _MSC_VER
	// no return to suppress unreachable code warning
#else
	return std::string{};
#endif
}

#ifdef _MSC_VER
#define WidePRId64 L"%lld"
#define WidePRIu64 L"%llu"
#elif !defined(WidePRId64)
#define WidePRId64 L"%" PRId64
#define WidePRIu64 L"%" PRIu64
#endif

template<>
inline std::wstring cast_to<std::wstring>(const value &val)
{
    std::type_index src_type{type_of(val)};

    if (src_type== typeid(int8_t)
            || src_type == typeid(int16_t)
            || src_type == typeid(int32_t)
            || src_type == typeid(int64_t))
    {
        std::wstringstream result;
        result << value_of<int64_t>(val);
        return result.str();
    }

    if (src_type == typeid(uint8_t)
            || src_type == typeid(uint16_t)
            || src_type == typeid(uint32_t)
            || src_type == typeid(uint64_t))
    {
        std::wstringstream result;
        result << value_of<uint64_t>(val);
        return result.str();
    }

    if (src_type == typeid(float)
            || src_type == typeid(double)
            || src_type == typeid(long double))
    {
        std::wstringstream result;
        result << value_of<long double>(val);
        return result.str();
    }

    throw type_mismatch(src_type, typeid(std::wstring));
#ifdef _MSC_VER
	// no return to suppress unreachable code warning
#else
	return std::wstring{};
#endif
}

}

#endif // CPP_DB_VALUE_INL
