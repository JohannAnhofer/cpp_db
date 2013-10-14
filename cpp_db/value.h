#ifndef CPP_DB_VALUE_H
#define CPP_DB_VALUE_H

#include "null.h"

#include <memory>
#include <typeinfo>
#include <typeindex>
#include <stdexcept>

namespace cpp_db
{
	class value
	{
	public:
		template<typename T>
		value(T value)
			: pholder(new concrete_holder<T>(value))
		{
		}

		value(const value  &other)
			: pholder(other.pholder->clone())
		{
		}

		value &operator=(const value &other)
		{
			if (this != &other)
				pholder.reset(other.pholder->clone());
			return *this;
		}

		template<typename T>
		T get_value() const
		{
			if (pholder->get_value_type() == typeid(T))
				return *reinterpret_cast<T const *>(pholder->get_value());
			throw std::runtime_error("Invalid value type.");
		}

		template<typename T>
		bool has_value_of_type() const
		{
			return pholder->get_value_type() == typeid(T);
		}

	private:
		struct abstract_holder
		{
			virtual ~abstract_holder() {}
			virtual void const * get_value() const = 0;
			virtual std::type_index get_value_type() const = 0;
			virtual abstract_holder *clone() const = 0;
		};

		template<typename ValueType>
		struct concrete_holder : public abstract_holder
		{
			explicit concrete_holder(ValueType value_in)
				: value(value_in)
				, value_type(typeid(value_in))
			{
			}

			void const * get_value() const override
			{
				return &value;
			}

			std::type_index get_value_type() const override
			{
				return value_type;
			}

			concrete_holder *clone() const override
			{
				return new concrete_holder(value);
			}

			ValueType value;
			std::type_index value_type;
		};

		std::unique_ptr<abstract_holder> pholder;
	};

    template<>
    null_type value::get_value<null_type>() const = delete;

	inline bool is_null(const value &data)
	{
		return data.has_value_of_type<null_type>();
	}

}

#endif
