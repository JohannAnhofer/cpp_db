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
	~parameters();
	parameters(const parameters &) = delete;
	parameters &operator=(const parameters &) = delete;

	parameters(parameters && other);
	parameters &operator=(parameters &&other);

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

	void bind(const parameter &param);

private:
	explicit parameters(const statement &stmt);
	friend class statement;

	struct impl;
	std::unique_ptr<impl> pimpl;
};

}

#endif
