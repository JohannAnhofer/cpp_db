#ifndef CPP_DB_VALUE_INL
#define CPP_DB_VALUE_INL

#include <cstdint>
#include <typeindex>
#include <typeinfo>

#include "value_is_null.h"

namespace cpp_db
{

template<>
struct value::concrete_holder<null_type> : public abstract_holder
{
    explicit concrete_holder(null_type)
    {
    }

    void const * get_value() const override
	{
		throw value_is_null{};
    }

    std::type_index get_type() const override
    {
        return typeid(null_type);
    }

    concrete_holder *clone() const override
    {
        return new concrete_holder<null_type>(null_type{});
    }
};

inline void throw_type_mismatch(const std::type_index &src_type, const std::type_index &dest_type)
{
    throw std::runtime_error(std::string("Value type '")+dest_type.name() + std::string("' is not convertible to '") + src_type.name()+"'");
}

template<>
inline const void * value_of<const void *>(const value &val)
{
    return val.pholder->get_value();
}

template<typename T>
T numeric_extractor(const value &val)
{
    std::type_index src_type{type_of(val)};

    if (src_type == typeid(T))
        return *reinterpret_cast<T const *>(value_of<const void *>(val));

    // signed integers
    if (src_type == typeid(int8_t))
        return value_of<int8_t>(val);
    if (src_type == typeid(int16_t))
        return value_of<int16_t>(val);
    if (src_type == typeid(int32_t))
        return value_of<int32_t>(val);
    if (src_type == typeid(int64_t))
        return value_of<int64_t>(val);

    // unsigned integers
    if (src_type == typeid(uint8_t))
        return value_of<uint8_t>(val);
    if (src_type == typeid(uint16_t))
        return value_of<uint16_t>(val);
    if (src_type == typeid(uint32_t))
        return value_of<uint32_t>(val);
    if (src_type == typeid(uint64_t))
        return value_of<uint64_t>(val);

    if (src_type == typeid(float))
        return value_of<float>(val);
    if (src_type == typeid(double))
        return value_of<double>(val);
    if (src_type == typeid(long double))
        return value_of<long double>(val);

    throw_type_mismatch(src_type, typeid(T));
    return 0;
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
inline const char * value_of<const char *>(const value &val);
template<>
inline const wchar_t * value_of<const wchar_t *>(const value &val);

template<>
inline std::string value_of<std::string>(const value &val)
{
    std::type_index src_type{type_of(val)};

    if (src_type == typeid(const char *))
        return value_of<const char *>(val);
    if (src_type == typeid(char *))
        return value_of<char *>(val);
    if (src_type == typeid(std::string))
        return *reinterpret_cast<std::string const *>(value_of<const void *>(val));

    throw_type_mismatch(src_type, typeid(std::string));
    return std::string{};
}

template<>
inline std::wstring value_of<std::wstring>(const value &val)
{
    std::type_index src_type{type_of(val)};

    if (src_type == typeid(const wchar_t *))
        return value_of<const wchar_t *>(val);
    if (src_type == typeid(wchar_t *))
        return value_of<wchar_t *>(val);
    if (src_type == typeid(std::wstring))
        return *reinterpret_cast<std::wstring const *>(value_of<const void *>(val));

    throw_type_mismatch(src_type, typeid(std::wstring));
    return std::wstring{};
}

template<>
inline const char * value_of<const char *>(const value &val)
{
    std::type_index src_type{type_of(val)};

    if (src_type == typeid(std::string))
        return value_of<std::string>(val).c_str();
    if (src_type == typeid(const char *))
        return *reinterpret_cast<const char * const *>(value_of<const void *>(val));

    throw_type_mismatch(src_type, typeid(const char *));
    return nullptr;
}

template<>
inline const wchar_t * value_of<const wchar_t *>(const value &val)
{
    std::type_index src_type{type_of(val)};

    if (src_type == typeid(std::wstring))
        return value_of<std::wstring>(val).c_str();
    if (src_type == typeid(const wchar_t *))
        return *reinterpret_cast<const wchar_t * const *>(value_of<const void *>(val));

    throw_type_mismatch(src_type, typeid(const wchar_t *));
    return nullptr;
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
        int64_t result = value_of<int64_t>(val);
        char number[50];
        snprintf(number, sizeof(number)/sizeof(number[0]), "%lli", result);
        return number;
    }

    if (src_type == typeid(uint8_t)
            || src_type == typeid(uint16_t)
            || src_type == typeid(uint32_t)
            || src_type == typeid(uint64_t))
    {
        uint64_t result = value_of<uint64_t>(val);
        char number[50];
        snprintf(number, sizeof(number)/sizeof(number[0]), "%llu", result);
        return number;
    }

    if (src_type == typeid(float)
            || src_type == typeid(double)
            || src_type == typeid(long double))
    {
        long double result = value_of<long double>(val);
        char number[50];
        snprintf(number, sizeof(number)/sizeof(number[0]), "%Lg", result);
        return number;
    }

    throw_type_mismatch(src_type, typeid(std::string));
    return std::string{};
}

template<>
inline std::wstring cast_to<std::wstring>(const value &val)
{
    std::type_index src_type{type_of(val)};

    if (src_type== typeid(int8_t)
            || src_type == typeid(int16_t)
            || src_type == typeid(int32_t)
            || src_type == typeid(int64_t))
    {
        int64_t result = value_of<int64_t>(val);
        wchar_t number[50];
        swprintf(number, sizeof(number)/sizeof(number[0]), L"%lli", result);
        return number;
    }

    if (src_type == typeid(uint8_t)
            || src_type == typeid(uint16_t)
            || src_type == typeid(uint32_t)
            || src_type == typeid(uint64_t))
    {
        uint64_t result = value_of<uint64_t>(val);
        wchar_t number[50];
        swprintf(number, sizeof(number)/sizeof(number[0]), L"%llu", result);
        return number;
    }

    if (src_type == typeid(float)
            || src_type == typeid(double)
            || src_type == typeid(long double))
    {
        long double result = value_of<long double>(val);
        wchar_t number[50];
        swprintf(number, sizeof(number)/sizeof(number[0]), L"%Lg", result);
        return number;
    }

    throw_type_mismatch(src_type, typeid(std::wstring));
    return std::wstring{};
}

}

#endif // CPP_DB_VALUE_INL
