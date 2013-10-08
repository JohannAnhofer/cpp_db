#ifndef CPP_DB_PARAMETERS_H
#define CPP_DB_PARAMETERS_H

#include "parameter.h"

#include <memory>
#include <string>

namespace cpp_db
{

class statement;

class parameters
{
public:
	explicit parameters(const statement &stmt);
	~parameters();

	int get_count() const;

	template<typename T>
	void bind(int pos, T && value)
	{
		bind(parameter(pos, value));
	}

	template<typename T>
	void bind(const std::string &name, T && value)
	{
		bind(parameter(name, value));
	}

private:
	struct impl;
	std::unique_ptr<impl> pimpl;

	void bind(const parameter &param);
};

}

#endif
