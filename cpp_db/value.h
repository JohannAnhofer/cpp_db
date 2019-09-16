#ifndef CPP_DB_VALUE_H
#define CPP_DB_VALUE_H

#include "null.h"
#include "type_of.h"
#include "value_of.h"
#include "value_is_null.h"
#include "type_mismatch.h"

#include <any>

#include <memory>
#include <string>

namespace cpp_db
{
    using value = std::any;

    inline bool is_null(const value &v)
    {
        if (!v.has_value())
            throw std::runtime_error("Invalid value object");
        return !strcmp(v.type().name(), typeid(null_type).name());
    }

    inline std::type_index type_of(const value &v)
    {
		if (!v.has_value())
            throw std::runtime_error("Invalid value object");
        if (is_null(v))
            throw value_is_null{};
        return v.type();
    }

    template<typename T>
    T value_of(const value &val)
    {
		if (!strcmp(val.type().name(), typeid(T).name()))
            return std::any_cast<T>(val);
        throw type_mismatch(type_of(val), typeid(T));
    }

    // default behaviour for cast_to is the same as value_of
    template<typename T>
    T cast_to(const value &val)
    {
        return value_of<T>(val);
    }

}

#include "value.inl"

#endif
