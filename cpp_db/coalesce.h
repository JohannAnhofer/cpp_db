#ifndef CPP_DB_COALESCE_H
#define CPP_DB_COALESCE_H

namespace cpp_db1
{

struct null_type {};

// head is non null
template<typename T, typename...Ts>
struct coalesce_trait
{
	typedef T type;
};

// head is null
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

// head is null
template<typename...Ts>
auto coalesce(null_type, Ts... ts) -> decltype(typename coalesce_trait<ts...>::type)
{
	return coalesce(ts...);
}

// head is non null
template<typename T, typename...Ts>
T coalesce(T&& t, Ts... ts)
{
	return std::forward<T>(t);
}

// last argument is non null 
template<typename T>
T coalesce(T&& t)
{
	return std::forward<T>(t);
}

// last argument is null case
null_type coalesce(null_type nt)
{
	return nt;
}

bool is_null(null_type)
{
	return true;
}

template<typename T>
bool is_null(T &&)
{
	return false;
}

}

#endif 
