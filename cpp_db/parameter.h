#ifndef CPP_DB_PARAMETER_H
#define CPP_DB_PARAMETER_H

#include "null.h"
#include "type_of.h"
#include "value.h"

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
        parameter(int pos, const value &arg)
            : argument(arg)
            , index(pos)
		{
		}

        parameter(std::string name_in, const value &arg)
            : argument(arg)
            , index(-1)
            , name(name_in)
        {
        }

        parameter(const parameter &other) = default;
        parameter &operator=(const parameter &other) = default;
        parameter(parameter &&other) = default;
        parameter &operator=(parameter &&other) = default;

		template<typename T>
		T get_value() const
		{
            return argument.get_value<T>();
		}

		bool has_index() const
		{
            return index >= 0 && name.empty();
		}

		template<typename T>
		bool has_value_of_type() const
		{
            return argument.has_value_of_type<T>();
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

        template<typename T, typename U>
        U cast_to() const
        {
            return argument.cast_to<T, U>();
        }

		friend std::type_index type_of(const parameter &arg);
        friend bool is_null(const parameter &arg);

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

    template<>
    null_type parameter::get_value<null_type>() const = delete;

    template<>
    null_type parameter::cast_to<null_type, null_type>() const = delete;

    template<>
    bool parameter::has_value_of_type<null_type>() const = delete;
}

#endif
