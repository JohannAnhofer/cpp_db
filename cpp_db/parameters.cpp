#include "parameters.h"
#include "parameters_interface.h"
#include "statement.h"
#include "driver.h"

namespace cpp_db
{
	parameters::parameters(const statement &stmt)
		: pparameters(stmt.get_driver()->make_parameters(stmt.get_handle()))
	{
	}

	parameters::parameters(parameters && other)
		: pparameters(std::move(other.pparameters))
	{
	}

	parameters &parameters::operator=(parameters &&other)
	{
		if (this != &other)
			pparameters = std::move(other.pparameters);
		return *this;
	}

	parameters::~parameters()
	{
	}

	int parameters::get_count() const
	{
		return pparameters->get_count();
	}

	void parameters::bind(const parameter &param)
	{
		pparameters->bind(param);
	}
}
