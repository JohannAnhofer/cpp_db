#ifndef CPP_DB_PARAMETER_H
#define CPP_DB_PARAMETER_H

#include "db_exception.h"
#include "null.h"

#include <string>
#include <memory>
#include <typeinfo>
#include <typeindex>

namespace cpp_db
{

	class parameter
	{
	public:
		template<typename T>
		parameter(int pos, T value)
			: pholder(new concrete_holder<int, T>(pos, value))
		{
		}

		template<typename T>
		parameter(std::string name, T value)
			: pholder(new concrete_holder<std::string, T>(name, value))
		{
		}

		parameter(const parameter &other)
			: pholder(other.pholder->clone())
		{
		}

		parameter &operator=(const parameter &other)
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
			throw db_exception("Invalid value type for parameter.");
		}

		bool has_index() const
		{
			return pholder->get_index_type() == typeid(int);
		}

		template<typename T>
		bool has_value_of_type() const
		{
			return pholder->get_value_type() == typeid(T);
		}

		bool has_name() const
		{
			return pholder->get_index_type() == typeid(std::string);
		}

		int get_index() const
		{
			if (pholder->get_index_type() == typeid(int))
				return *reinterpret_cast<int const *>(pholder->get_index());
			throw db_exception("Invalid index type for parameter.");
		}

		std::string get_name() const
		{
			if (pholder->get_index_type() == typeid(std::string))
				return *reinterpret_cast<std::string const *>(pholder->get_index());
			throw db_exception("Invalid index type for parameter.");
		}

	private:
		struct abstract_holder
		{
			virtual ~abstract_holder() {}
			virtual void const * get_value() const = 0;
            virtual std::type_index get_value_type() const = 0;
			virtual void const * get_index() const = 0;
            virtual std::type_index get_index_type() const = 0;
			virtual abstract_holder *clone() const = 0;
        };

		template<typename IndexType, typename ValueType>
		struct concrete_holder : public abstract_holder
		{
			explicit concrete_holder(IndexType index_in, ValueType value_in)
				: index(index_in)
				, value(value_in)
				, index_type(typeid(index_in))
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

			void const * get_index() const override
			{
				return &index;
			}

			std::type_index get_index_type() const override
			{
				return index_type;
			}

			concrete_holder *clone() const override
			{
				return new concrete_holder(index, value);
			}

			IndexType index;
			ValueType value;
			std::type_index index_type, value_type;
		};

		std::unique_ptr<abstract_holder> pholder;
	};

    template<>
    tools::null_type parameter::get_value<tools::null_type>() const = delete;

	inline bool is_null(const parameter &arg)
	{
		return arg.has_value_of_type<tools::null_type>();
	}
}

#endif
