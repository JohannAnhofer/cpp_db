#pragma once

#include <optional>

namespace tools
{

using null_type = std::nullopt_t;
inline constexpr null_type null = std::nullopt;

// first we need a trait structure to deduce the return type of coalesce

// head is non null
template<typename T, typename...Ts>
struct coalesce_trait
{
	typedef T type;
};

// head is null, analyze next type
template<typename...Ts>
struct coalesce_trait<null_type, Ts...>
{
	typedef typename coalesce_trait<Ts...>::type type;
};

// tail is null
template<>
struct coalesce_trait<null_type >
{
	typedef null_type type;
};

// tail is non null
template<typename T>
struct coalesce_trait<T>
{
	typedef T type;
};

// no we define a coalesce function, and a few overloads for it

// head is non null --> return the head
template<typename T, typename...Ts>
T coalesce(T&& t, Ts...)
{
    return std::forward<T>(t);
}

// tail is non null --> return the tail
template<typename T>
T coalesce(T&& t)
{
    return std::forward<T>(t);
}

// head is null --> analyze tail
template<typename...Ts>
typename coalesce_trait<Ts...>::type coalesce(null_type, Ts ...ts)
{
    return coalesce(ts...);
}

// tail is null case --> return null, because all other types where null
constexpr null_type coalesce(null_type nt)
{
	return nt;
}

inline constexpr bool is_null(null_type)
{
    return true;
}

template<typename T>
constexpr bool is_null(const T &)
{
    return false;
}

template<typename T>
constexpr bool is_null(const std::optional<T> &value)
{
	return !value;
}

template<>
constexpr bool is_null<std::nullopt_t>(const std::nullopt_t &)
{
	return true;
}

}

