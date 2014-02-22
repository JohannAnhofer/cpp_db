#ifndef CPP_DB_NULL_H
#define CPP_DB_NULL_H

#include "cpp11_defines.h"
#include "value_of.h"
#include "type_of.h"
#include "value_is_null.h"

#include <typeinfo>

namespace cpp_db
{

struct null_type
{
    const std::type_info &tag_type;
    null_type() : tag_type(typeid(void))
    {
    }
    template<typename TagType>
    null_type(const TagType &) : tag_type(typeid(TagType))
    {
    }

	null_type &operator =(const null_type &) = delete;
};

template<typename T>
null_type make_tagged_null(const T &t = T())
{
    return null_type(t);
}

inline CONSTEXPR bool is_null(const null_type &)
{
    return true;
}

template<typename T>
CONSTEXPR bool is_null(const T &)
{
    return false;
}

template<>
inline std::type_index type_of<null_type>(const null_type &)
{
    throw value_is_null{};
}

template<>
inline null_type value_of<null_type>(const null_type &)
{
    throw value_is_null{};
}

}

#endif // CPP_DB_NULL_H
