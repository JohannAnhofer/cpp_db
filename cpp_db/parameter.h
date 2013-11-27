#ifndef CPP_DB_PARAMETER_H
#define CPP_DB_PARAMETER_H

#include "null.h"
#include "type_of.h"
#include "value.h"
#include "value_of.h"

#include <string>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <stdexcept>

namespace cpp_db
{

	class parameter
	{
	public:
        parameter(int pos, value arg)
            : argument(arg)
            , index(pos)
		{
		}

        parameter(std::string name_in, value arg)
            : argument(arg)
            , index(-1)
            , name(name_in)
        {
        }

        parameter(const parameter &) = default;
        parameter &operator=(const parameter &) = default;

#if !defined(_MSC_VER) || (_MSC_FULL_VER > 180021005)
        parameter(parameter &&) = default;
        parameter &operator=(parameter &&) = default;
#else
		parameter(parameter && other)
			: argument(std::move(other.argument))
			, index(other.index)
			, name(std::move(other.name))

		{
			other.index = -1;
		}

		parameter &operator=(parameter && other)
		{
			if (this != &other)
			{
				argument = std::move(other.argument);
				index = other.index;
				name = std::move(other.name);
				other.index = -1;
			}
			return *this;
		}
#endif

        value get_value() const
        {
            return argument;
        }

		bool has_index() const
		{
            return index >= 0 && name.empty();
		}

		bool has_name() const
		{
            return index == -1 && !name.empty();
		}

		int get_index() const
		{
            if (has_index())
                return index;
			throw std::runtime_error("Invalid index type for parameter.");
		}

		std::string get_name() const
		{
            if (has_name())
                return name;
			throw std::runtime_error("Invalid index type for parameter.");
		}

		friend std::type_index type_of(const parameter &arg);
        friend bool is_null(const parameter &arg);
        template<typename T>
        friend T value_of(const parameter &arg);
        template<typename T>
        friend T cast_to(const parameter &arg);

	private:
        value argument;
        int index;
        std::string name;
	};

	inline bool is_null(const parameter &arg)
	{
        return is_null(arg.argument);
	}

	inline std::type_index type_of(const parameter &arg)
	{
        return type_of(arg.argument);
	}

    template<typename T>
    T value_of(const parameter &arg)
    {
        return value_of<T>(arg.argument);
    }

    template<typename T>
    T cast_to(const parameter &arg)
    {
        return cast_to<T>(arg.argument);
    }
}

#endif
