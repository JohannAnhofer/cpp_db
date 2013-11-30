#ifndef CPP_DB_VALUE_INL
#define CPP_DB_VALUE_INL

#include "value_is_null.h"

#include <cstdint>
#include <typeindex>
#include <typeinfo>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

namespace cpp_db
{

#ifndef USE_BOOST_ANY
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
#endif

inline std::string build_type_mismatch_message(const std::type_index &src_type, const std::type_index &dest_type)
{
    return std::string("Value type '")
		+dest_type.name() 
		+ std::string("' is not convertible to '") 
		+ src_type.name()+"'";
}

#ifndef USE_BOOST_ANY
template<>
inline const void * value_of<const void *>(const value &val)
{
    return val.pholder->get_value();
}
#endif

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

    if (src_type == typeid(T))
#ifdef USE_BOOST_ANY
        return boost::any_cast<T>(val);
#else
        return *reinterpret_cast<T const *>(value_of<const void *>(val));
#endif

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

	throw std::runtime_error(build_type_mismatch_message(src_type, typeid(T)));
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

#ifndef USE_BOOST_ANY
template<>
inline const char * value_of<const char *>(const value &val);
template<>
inline const wchar_t * value_of<const wchar_t *>(const value &val);
#endif

template<>
inline std::string value_of<std::string>(const value &val)
{
    std::type_index src_type{type_of(val)};

    if (src_type == typeid(const char *))
        return value_of<const char *>(val);
    if (src_type == typeid(char *))
        return value_of<char *>(val);
    if (src_type == typeid(std::string))
#ifdef USE_BOOST_ANY
        return boost::any_cast<std::string>(val);
#else
        return *reinterpret_cast<std::string const *>(value_of<const void *>(val));
#endif

	throw std::runtime_error(build_type_mismatch_message(src_type, typeid(std::string)));
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
    if (src_type == typeid(std::wstring))
#ifdef USE_BOOST_ANY
        return boost::any_cast<std::wstring>(val);
#else
        return *reinterpret_cast<std::wstring const *>(value_of<const void *>(val));
#endif

	throw std::runtime_error(build_type_mismatch_message(src_type, typeid(std::wstring)));
#ifdef _MSC_VER
	// no return to suppress unreachable code warning
#else
	return std::wstring{};
#endif
}

#ifndef USE_BOOST_ANY
template<>
inline const char * value_of<const char *>(const value &val)
{
    std::type_index src_type{type_of(val)};

    if (src_type == typeid(std::string))
        return reinterpret_cast<const std::string *>(value_of<const void *>(val))->c_str();
    if (src_type == typeid(const char *))
        return *reinterpret_cast<const char * const *>(value_of<const void *>(val));

	throw std::runtime_error(build_type_mismatch_message(src_type, typeid(const char *)));
#ifdef _MSC_VER
	// no return to suppress unreachable code warning
#else
	return nullptr;
#endif
}

template<>
inline const wchar_t * value_of<const wchar_t *>(const value &val)
{
    std::type_index src_type{type_of(val)};

    if (src_type == typeid(std::wstring))
        return reinterpret_cast<const std::wstring *>(value_of<const void *>(val))->c_str();
    if (src_type == typeid(const wchar_t *))
        return *reinterpret_cast<const wchar_t * const *>(value_of<const void *>(val));

	throw std::runtime_error(build_type_mismatch_message(src_type, typeid(const wchar_t *)));
#ifdef _MSC_VER
	// no return to suppress unreachable code warning
#else
	return nullptr;
#endif
}
#endif

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

	throw std::runtime_error(build_type_mismatch_message(src_type, typeid(std::string)));
#ifdef _MSC_VER
	// no return to suppress unreachable code warning
#else
	return std::string{};
#endif
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

	throw std::runtime_error(build_type_mismatch_message(src_type, typeid(std::wstring)));
#ifdef _MSC_VER
	// no return to suppress unreachable code warning
#else
	return std::wstring{};
#endif
}

}

#endif // CPP_DB_VALUE_INL
