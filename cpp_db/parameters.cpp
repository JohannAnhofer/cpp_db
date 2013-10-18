#include "parameters.h"
#include "parameters_interface.h"
#include "statement.h"
#include "driver.h"

#include <stdexcept>

namespace cpp_db
{
	parameters::parameters(const statement &stmt)
		: params_impl(stmt.get_driver()->make_parameters(stmt.get_handle()))
	{
		if (!params_impl)
			throw std::runtime_error("No parameters object from driver!");
	}

	parameters::parameters(parameters && other)
		: params_impl(std::move(other.params_impl))
	{
	}

	parameters &parameters::operator=(parameters &&other)
	{
		if (this != &other)
			params_impl = std::move(other.params_impl);
		return *this;
	}

	parameters::~parameters()
	{
	}

	int parameters::get_count() const
	{
		return params_impl->get_count();
	}

	void parameters::bind(const parameter &param)
	{
		params_impl->bind(param);
	}
}
