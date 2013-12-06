#include "parameters.h"
#include "parameters_interface.h"
#include "statement.h"
#include "driver_interface.h"

#include <stdexcept>

namespace cpp_db
{
    parameters::parameters()
	{
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

    handle parameters::get_handle() const
    {
        return params_impl->get_handle();
    }
}
