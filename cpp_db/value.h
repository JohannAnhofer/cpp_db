#ifndef CPP_DB_VALUE_H
#define CPP_DB_VALUE_H

#include "null.h"

#include <memory>
#include <typeinfo>
#include <typeindex>
#include <stdexcept>
#include <string>

namespace cpp_db
{
	class value
	{
	public:
		template<typename T>
        value(T val)
            : pholder(new concrete_holder<T>(val))
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

#if _MSC_VER > 1800
        value(value &&) = default;
        value &operator=(value &&) = default;
#endif

		template<typename T>
		T get_value() const
		{
			if (pholder->get_value_type() == typeid(T))
				return *reinterpret_cast<T const *>(pholder->get_value());
            throw std::runtime_error(std::string("Invalid value type (")+typeid(T).name() + std::string(" / ") + pholder->get_value_type().name()+")");
		}

		template<typename T, typename U>
		U cast_to() const
		{
			if (pholder->get_value_type() == typeid(T))
                return static_cast<concrete_holder<T> *>(pholder.get())->template cast_to<U>();
			throw std::runtime_error(std::string("Invalid value type (") + +typeid(T).name() + std::string(" != ") + pholder->get_value_type().name() + ")");
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

			template<typename CastType>
			CastType cast_to() const
			{
				return value;
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

#include "value.inl"

#endif
