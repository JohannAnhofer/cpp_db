#ifndef CPP_DB_PARAMETERS_H
#define CPP_DB_PARAMETERS_H

#include "parameter.h"
#include "usings.h"

#include <memory>
#include <string>

namespace cpp_db
{

class statement;
struct parameters_interface;

class parameters
{
public:
	~parameters();
	parameters(const parameters &) = delete;
	parameters &operator=(const parameters &) = delete;

	parameters(parameters && other);
	parameters &operator=(parameters &&other);

	int get_count() const;

	template<typename T> void bind(int pos, T && value);
	template<typename T> void bind(const std::string &name, T && value);
	void bind(const parameter &param);

    handle get_handle() const;

private:
    parameters();
	friend class statement;

	std::unique_ptr<parameters_interface> params_impl;
};


template<typename T>
void parameters::bind(int pos, T && value)
{
	bind(parameter(pos, value));
}

template<typename T>
void parameters::bind(const std::string &name, T && value)
{
	bind(parameter(name, value));
}

}

#endif
