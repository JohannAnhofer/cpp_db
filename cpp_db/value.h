#ifndef CPP_DB_VALUE_H
#define CPP_DB_VALUE_H

#include "null.h"
#include "type_of.h"
#include "value_of.h"
#include "value_is_null.h"

#include <memory>
#include <string>

namespace cpp_db
{
	class value
	{
	public:
		value() = default;

		template<typename T>
        value(T val)
            : pholder(new concrete_holder<T>(val))
		{
		}

        value(const char *val)
            : pholder(new concrete_holder<std::string>(val))
        {
        }

        value(char *val)
            : pholder(new concrete_holder<std::string>(val))
        {
        }

        value(const wchar_t *val)
            : pholder(new concrete_holder<std::wstring>(val))
        {
        }

        value(wchar_t *val)
            : pholder(new concrete_holder<std::wstring>(val))
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

#if !defined(_MSC_VER) || (_MSC_FULL_VER > 180021005)
		value(value &&) = default;
        value &operator=(value &&) = default;
#else
		value(value && other)
			: pholder(std::move(other.pholder))
		{
		}

		value &operator=(value && other)
		{
			if (this != &other)
				pholder = std::move(other.pholder);
			return *this;
		}
#endif

        friend std::type_index type_of(const value &v);
		friend bool is_null(const value &v);
        template<typename T>
        friend T value_of(const value &val);

	private:
		struct abstract_holder
		{
			virtual ~abstract_holder() {}
			virtual void const * get_value() const = 0;
			virtual std::type_index get_type() const = 0;
			virtual abstract_holder *clone() const = 0;
		};

        template<typename ValueType>
        struct concrete_holder : public abstract_holder
        {
            explicit concrete_holder(ValueType value_in)
                : value(value_in)
            {
            }

            void const * get_value() const override
            {
                return &value;
            }

			std::type_index get_type() const override
			{
				return typeid(ValueType);
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
		};

		std::unique_ptr<abstract_holder> pholder;
	};

	inline std::type_index type_of(const value &v)
	{
        if (!v.pholder)
            throw std::runtime_error("Invalid value object");
        if (is_null(v))
            throw value_is_null{};
		return v.pholder->get_type();
	}

	inline bool is_null(const value &v)
	{
        if (!v.pholder)
            throw std::runtime_error("Invalid value object");
        return v.pholder->get_type() == typeid(null_type);
	}

    template<typename T>
    T value_of(const value &val)
    {
        if (type_of(val) == typeid(T))
            return *reinterpret_cast<T const *>(val.pholder->get_value());
        throw std::runtime_error(std::string("Invalid value type (")+typeid(T).name() + std::string(" / ") + type_of(val).name()+")");
    }

    template<typename T, typename U>
    U cast_to(const value &val)
    {
        return static_cast<U>(value_of<T>(val));
    }

}

#include "value.inl"

#endif
